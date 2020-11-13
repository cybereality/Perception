/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/
#pragma region AQU_WorkingArea include/define
#include"AQU_WorkingArea.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_INT(a) { wchar_t buf[128]; wsprintf(buf, L"%d", a); OutputDebugString(buf); }

#define TOP_MENU_FILE_SIZE 80
#define TOP_MENU_OPTIONS_SIZE 100
#define SUB_MENU_FILE_SIZE 240
#define SUB_MENU_OPTIONS_SIZE 200
#define MINIMUM_WINDOW_WIDTH TOP_MENU_FILE_SIZE+TOP_MENU_OPTIONS_SIZE+300
#define MINIMUM_WINDOW_HEIGHT 200
#define ZOOM_CONTROL_LEFT (m_nWindowWidth-350)
#define ZOOM_CONTROL_RIGHT (m_nWindowWidth-20)
#pragma endregion

#pragma region AQU_WorkingArea static fields
AQU_Drawer*                                   AQU_WorkingArea::m_pDirectDraw;
WNDCLASSEX                                    AQU_WorkingArea::m_wc;
HWND                                          AQU_WorkingArea::m_hwnd;
HINSTANCE                                     AQU_WorkingArea::m_hInstance;
int                                           AQU_WorkingArea::m_nScreenWidth;
int                                           AQU_WorkingArea::m_nScreenHeight;
int                                           AQU_WorkingArea::m_nWindowWidth;
int                                           AQU_WorkingArea::m_nWindowHeight;
POINT                                         AQU_WorkingArea::m_ptMouseCursor;
float                                         AQU_WorkingArea::m_fZoom;
float                                         AQU_WorkingArea::m_fXScrollPos;
float                                         AQU_WorkingArea::m_fYScrollPos;
float                                         AQU_WorkingArea::m_fYScrollDataSheet;
SIZE                                          AQU_WorkingArea::m_vcWorkspaceSize;
SIZE                                          AQU_WorkingArea::m_vcDataSheetSize;
int                                           AQU_WorkingArea::m_nDataSheetCategorySelection;
int                                           AQU_WorkingArea::m_nDataSheetEntrySelection;
std::vector<NOD_Basic*>                       AQU_WorkingArea::m_paNodes;
int                                           AQU_WorkingArea::m_nActiveNodeIndex;
int                                           AQU_WorkingArea::m_nActiveDataSheetEntry;
int                                           AQU_WorkingArea::m_nActiveTopMenuFileEntry;
int                                           AQU_WorkingArea::m_nActiveTopMenuOptionsEntry;
AQU_NodeBehavior                              AQU_WorkingArea::m_eActiveNodeBehavior;
AQU_WorkingAreaStatus                         AQU_WorkingArea::m_eWorkingAreaStatus;
AQU_TransferSite*                             AQU_WorkingArea::m_pcTransferSite;
AQU_WorkingAreaElementSizes                   AQU_WorkingArea::m_sWorkingAreaElementSizes;
std::vector<LPSTR>                            AQU_WorkingArea::m_vcPluginNames;
std::vector<LPWSTR>                           AQU_WorkingArea::m_vcPluginCategories;
std::vector<LPWSTR>                           AQU_WorkingArea::m_vcPluginFilePathes;
std::vector<UINT>                             AQU_WorkingArea::m_vcPluginIDs;
std::vector<AQU_Nodus*>                       AQU_WorkingArea::m_vcPlugins;
std::vector<HMODULE>                          AQU_WorkingArea::m_vcPluginHandles;
BOOL*                                         AQU_WorkingArea::m_pbHighlightEntryFile[AQU_TopMenuBarFileEntries::NUMBER_OF_FILE_ENTRIES];
BOOL*                                         AQU_WorkingArea::m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::NUMBER_OF_OPTIONS_ENTRIES];
BOOL                                          AQU_WorkingArea::m_bDrawDataLines;
BOOL                                          AQU_WorkingArea::m_bDrawDataSlings;
BOOL                                          AQU_WorkingArea::m_bDrawDataPipelines;
BOOL                                          AQU_WorkingArea::m_bDrawDataDotted;
BOOL                                          AQU_WorkingArea::m_bDrawStreamLines;
BOOL                                          AQU_WorkingArea::m_bDrawStreamSlings;
BOOL                                          AQU_WorkingArea::m_bDrawStreamPipelines;
BOOL                                          AQU_WorkingArea::m_bDrawStreamDotted;
#pragma endregion

#pragma region AQU_WorkingArea global fields
/**
* Top menu >File< entries.
***/
const wchar_t* g_aszTopMenuFileEntries[] =
{
	L"Save Working Area...",
	L"Compile Profile...",
	L"Save Externally",
};

/**
* Top menu >Options< entries.
***/
const wchar_t* g_aszTopMenuOptionsEntries[] =
{
	L"Reinstate Interfaces",
	L"Draw Data Lines",
	L"Draw Data Slings",
	L"Draw Data Pipelines",
	L"Draw Data Dotted",
	L"Draw Stream Lines",
	L"Draw Stream Slings",
	L"Draw Stream Pipelines",
	L"Draw Stream Dotted",
	L"Delete Unconnected",
};
#pragma endregion

#pragma region AQU_WorkingArea con-/destructor
/**
* Small control helper.
***/
bool InRect(RECT rc, POINT pt)
{
	return (pt.x >= rc.left) && (pt.y >= rc.top) && (pt.x <= rc.right) && (pt.y <= rc.bottom);
}

/**
* Constructor.
* Sets screen and window size, then creates the workin area window.
***/
AQU_WorkingArea::AQU_WorkingArea(HINSTANCE hInstance, AQU_TransferSite* pcTransferSite)
{
	// set screen and window size
	m_nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	m_nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	m_nWindowWidth = m_nScreenWidth >> 1;
	m_nWindowHeight = m_nScreenHeight;

	// set zoom, workspace size and position TODO !! get current zoom setting and workspace size from configuration
	m_fZoom = 1.0f;
	m_vcWorkspaceSize.cx = 1024;
	m_vcWorkspaceSize.cy = 1024;
	m_fXScrollPos = 0.0f;
	m_fYScrollPos = 0.0f;
	m_fYScrollDataSheet = 0.0f;

	// set working area border sizes (later we should make this resizeable)
	m_sWorkingAreaElementSizes.dwMenu = 32;
	m_sWorkingAreaElementSizes.dwDataSheet = 196;
	m_sWorkingAreaElementSizes.dwScrollBarRight = 32;
	m_sWorkingAreaElementSizes.dwScrollBarBottom = 32;

	// set DataSheet size
	m_vcDataSheetSize.cx = (LONG)m_sWorkingAreaElementSizes.dwDataSheet;
	m_vcDataSheetSize.cy = (LONG)m_nWindowHeight - m_sWorkingAreaElementSizes.dwMenu;

	// set module handle and config
	m_hInstance = hInstance;
	m_pcTransferSite = pcTransferSite;
	m_pcTransferSite->m_bIsWorkingArea = true;

	m_pDirectDraw = NULL;
	m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
	m_nActiveNodeIndex = -1;
	m_eActiveNodeBehavior = AQU_NodeBehavior::Inactive;

	// data sheet selection
	m_nDataSheetEntrySelection = -1;
	m_nDataSheetCategorySelection = -1;

	// top menu entry selection
	m_nActiveTopMenuFileEntry = -1;
	m_nActiveTopMenuOptionsEntry = -1;

	// set connection drawing styles
	m_bDrawDataLines = TRUE;
	m_bDrawDataSlings = FALSE;
	m_bDrawDataPipelines = FALSE;
	m_bDrawDataDotted = FALSE;
	m_bDrawStreamLines = TRUE;
	m_bDrawStreamSlings = FALSE;
	m_bDrawStreamPipelines = FALSE;
	m_bDrawStreamDotted = FALSE;

	// create menu highlight bools (constant false)
	static BOOL bConstantFalse = false;
	for (int i = 0; i < (int)AQU_TopMenuBarFileEntries::NUMBER_OF_FILE_ENTRIES; i++)
		m_pbHighlightEntryFile[i] = &bConstantFalse;
	for (int i = 0; i < (int)AQU_TopMenuBarOptionsEntries::NUMBER_OF_OPTIONS_ENTRIES; i++)
		m_pbHighlightEntryOptions[i] = &bConstantFalse;

	// set highlight bools
	m_pbHighlightEntryFile[AQU_TopMenuBarFileEntries::SAVE_EXTERNALLY] = &m_pcTransferSite->m_pConfig->bExternalSave;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_DATA_LINES] = &m_bDrawDataLines;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_DATA_SLINGS] = &m_bDrawDataSlings;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_DATA_PIPELINES] = &m_bDrawDataPipelines;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_DATA_DOTTED] = &m_bDrawDataDotted;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_STREAM_LINES] = &m_bDrawStreamLines;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_STREAM_SLINGS] = &m_bDrawStreamSlings;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_STREAM_PIPELINES] = &m_bDrawStreamPipelines;
	m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::DRAW_STREAM_DOTTED] = &m_bDrawStreamDotted;

	// enumerate plugins
	s_EnumeratePlugins(pcTransferSite->m_pFileManager->GetPluginPath());

	// clear nodes vector
	m_paNodes.clear();

	// set transfer site node vector address
	pcTransferSite->m_ppaNodes = &m_paNodes;

	// clear data sheet vector and supported interface indices vector
	m_pcTransferSite->m_paDataSheetCategories.clear();
	m_pcTransferSite->m_aInterfaceIndices.clear();

	// get node provider
	AQU_NodeProvider* pProvider = new AQU_NodeProvider();

	// loop through all possible interfaces
	for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
	{
		// interface is hooked up ?
		if (m_pcTransferSite->m_pConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
		{
			// add interface index
			m_pcTransferSite->m_aInterfaceIndices.push_back((UINT)i);

			// create category
			AQU_DataSheetCategory* psCategory = new AQU_DataSheetCategory();
			psCategory->m_szTitle = AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].szName;
			psCategory->m_bIsOpen = false;
			psCategory->m_paEntries.clear();

			int lenClass = (int)wcslen(psCategory->m_szTitle) + 2;

			// loop through methods
			for (int j = 0; j < (int)AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].dwMethodsNumber; j++)
			{
				// get example node
				NOD_Basic* pNode = pProvider->Get_D3D_Node((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)i, j, 0, 0);

				if (pNode)
				{
					// create entry
					AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
					pEntry->m_bIsOpen = false;

					// set title
					int len = (int)wcslen(pNode->m_szTitle) + 1 - lenClass;
					if (len > 0)
					{
						pEntry->m_szTitle = new wchar_t[len];
						CopyMemory((void*)&pEntry->m_szTitle[0], (void*)&pNode->m_szTitle[lenClass], len*sizeof(wchar_t));
					}

					// create sub entries
					pEntry->m_dwSubEntriesNumber = (DWORD)pNode->m_paCommanders.size();
					pEntry->m_paSubEntries = new LPCWSTR[pEntry->m_dwSubEntriesNumber];
					for (int k = 0; k < (int)pEntry->m_dwSubEntriesNumber; k++)
					{
						len = (int)wcslen(pNode->m_paCommanders[k]->m_szTitle) + 1;
						pEntry->m_paSubEntries[k] = new wchar_t[len];
						CopyMemory((void*)&pEntry->m_paSubEntries[k][0], (void*)&pNode->m_paCommanders[k]->m_szTitle[0], len*sizeof(wchar_t));
					}

					// delete node
					delete pNode;

					// add entry
					psCategory->m_paEntries.push_back(pEntry);
				}
			}
			m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

		}
	}

	// create data sheet vertex shader tab, first create category and set tab index
	m_pcTransferSite->m_nVertexShaderTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	AQU_DataSheetCategory* psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Vertex Shaders";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();

	// add vertex shader category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// create data sheet pixel shader tab, first create category and set tab index
	m_pcTransferSite->m_nPixelShaderTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Pixel Shaders";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();

	// add vertex shader category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// create an elementary nodes tab, first create category and set tab index
	m_pcTransferSite->m_nElementaryNodesTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Elementary Nodes";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();
	for (int i = 0; i < (int)AQU_ElementaryNodes::EN_NUMBER_OF_ELEMENTARY_NODES; i++)
	{
		// create entry
		AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
		pEntry->m_bIsOpen = false;

		// get name
		pEntry->m_szTitle = pProvider->Get_Elementary_Node_Name((AQU_ElementaryNodes)i);

		// add entry
		psCategory->m_paEntries.push_back(pEntry);
	}

	// add elementary nodes category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// delete provider
	delete pProvider;

	// clear plugin indices vectors
	m_pcTransferSite->m_aPluginCategoryIndices.clear();
	m_pcTransferSite->m_aPluginEntryIndices.clear();

	// loop through enumerated plugins, create plugin entries and indices
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		UINT dwCategoryIndex = 0;

		// first, set category index
		for (int j = (int)m_pcTransferSite->m_nElementaryNodesTabIndex + 1; j < (int)m_pcTransferSite->m_paDataSheetCategories.size(); j++)
		{
			// compare name of the plugin enumeration category with the existing categories
			if (wcscmp(m_vcPluginCategories[i], m_pcTransferSite->m_paDataSheetCategories[j]->m_szTitle) == 0)
			{
				// set category index
				dwCategoryIndex = (UINT)j;

				// add indices
				m_pcTransferSite->m_aPluginCategoryIndices.push_back(dwCategoryIndex);
				m_pcTransferSite->m_aPluginEntryIndices.push_back((UINT)m_pcTransferSite->m_paDataSheetCategories[j]->m_paEntries.size());

				// create entry
				AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();

				// set title
				int len = (int)strlen(m_vcPluginNames[i]) + 1;
				if (len > 0)
				{
					pEntry->m_szTitle = new wchar_t[len];
					std::string s(m_vcPluginNames[i]);
					std::wstring sW = std::wstring(s.begin(), s.end());

					CopyMemory((void*)&pEntry->m_szTitle[0], (void*)sW.c_str(), len*sizeof(wchar_t));
				}

				// add entry to category
				m_pcTransferSite->m_paDataSheetCategories[j]->m_paEntries.push_back(pEntry);
			}
		}

		// no category ? create new
		if (dwCategoryIndex == 0)
		{
			// set indices
			m_pcTransferSite->m_aPluginCategoryIndices.push_back((UINT)m_pcTransferSite->m_paDataSheetCategories.size());
			m_pcTransferSite->m_aPluginEntryIndices.push_back(0);

			// create category
			psCategory = new AQU_DataSheetCategory();
			if (wcslen(m_vcPluginCategories[i]) == 0)
				psCategory->m_szTitle = L"Uncategorized";
			else
				psCategory->m_szTitle = (LPCWSTR)m_vcPluginCategories[i];
			psCategory->m_bIsOpen = false;
			psCategory->m_paEntries.clear();

			// create entry
			AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();

			// set title
			int len = (int)strlen(m_vcPluginNames[i]) + 1;
			if (len > 0)
			{
				pEntry->m_szTitle = new wchar_t[len];
				std::string s(m_vcPluginNames[i]);
				std::wstring sW = std::wstring(s.begin(), s.end());

				CopyMemory((void*)&pEntry->m_szTitle[0], (void*)sW.c_str(), len*sizeof(wchar_t));
			}

			// add entry to category
			psCategory->m_paEntries.push_back(pEntry);

			// add category
			m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);
		}
	}

	// create the main window loop thread
	CreateThread(0, 0, s_WorkingAreaMsgThread, 0, 0, 0);
}

/**
* Destructor.
***/
AQU_WorkingArea::~AQU_WorkingArea()
{
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginNames.clear();

	auto it = m_paNodes.begin();
	while (it != m_paNodes.end())
	{
		if ((*it) != NULL)
			delete (*it);
		it = m_paNodes.erase(it);
	}
	auto it1 = m_vcPlugins.begin();
	while (it1 != m_vcPlugins.end())
	{
		if ((*it1) != NULL)
			delete (*it1);
		it1 = m_vcPlugins.erase(it1);
	}
	auto it2 = m_vcPluginHandles.begin();
	while (it2 != m_vcPluginHandles.end())
	{
		if ((*it2) != NULL)
			FreeLibrary(*it2);
		it2 = m_vcPluginHandles.erase(it2);
	}
}
#pragma endregion

#pragma region AQU_WorkingArea methods
/**
* Static working area main window proc.
***/
LRESULT CALLBACK AQU_WorkingArea::s_WorkingAreaWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int y = 0;
	POINT vcZeroOrigin;
	vcZeroOrigin.x = 0; vcZeroOrigin.y = 0;

	// set the current origin of the working space by the element sizes
	POINT vcWorkspaceOrigin;
	vcWorkspaceOrigin.x = m_sWorkingAreaElementSizes.dwDataSheet - (LONG)(m_fXScrollPos * m_fZoom);
	vcWorkspaceOrigin.y = m_sWorkingAreaElementSizes.dwMenu - (LONG)(m_fYScrollPos * m_fZoom);

	// get the actual size of the workspace in the window
	SIZE vcWorkspaceWindowSize;
	vcWorkspaceWindowSize.cx = m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight - m_sWorkingAreaElementSizes.dwDataSheet - 8;
	vcWorkspaceWindowSize.cy = m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom - m_sWorkingAreaElementSizes.dwMenu - 8;

	// get the size considering workspace zoom
	SIZE vcWorkspaceZoomWindowSize;
	vcWorkspaceZoomWindowSize.cx = (LONG)((float)vcWorkspaceWindowSize.cx / m_fZoom);
	vcWorkspaceZoomWindowSize.cy = (LONG)((float)vcWorkspaceWindowSize.cy / m_fZoom);

	// clamp workspace size
	if (m_vcWorkspaceSize.cx < (vcWorkspaceZoomWindowSize.cx + (LONG)m_fXScrollPos + 1)) m_vcWorkspaceSize.cx = vcWorkspaceZoomWindowSize.cx + (LONG)m_fXScrollPos + 1;
	if (m_vcWorkspaceSize.cy < (vcWorkspaceZoomWindowSize.cy + (LONG)m_fYScrollPos + 1)) m_vcWorkspaceSize.cy = vcWorkspaceZoomWindowSize.cy + (LONG)m_fYScrollPos + 1;

	LPARAM hotkeylParam = 0;
	
	switch (msg)
	{
		case WM_HOTKEY:
			m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
			if (m_nActiveNodeIndex > -1)
			{
				m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
				if (wParam == 1)
					s_UpdateActiveNode(WM_KEYDOWN, VK_PRIOR, hotkeylParam);
				else if (wParam == 2)
					s_UpdateActiveNode(WM_KEYDOWN, VK_NEXT, hotkeylParam);
			}
			break;
#pragma region WM_CREATE
		case WM_CREATE:

			// init the controls

			break;
#pragma endregion
#pragma region WM_LBUTTONDOWN
		case WM_LBUTTONDOWN:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// waiting for idle ?
			if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::ToIdle)
				m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;

			// idle status ? 
			if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::Idle)
			{
#pragma region idle
				// go through controls
				if ((m_ptMouseCursor.y <= (LONG)m_sWorkingAreaElementSizes.dwMenu) && (m_ptMouseCursor.x < TOP_MENU_FILE_SIZE))
				{
					// top menu file
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_File;
				}
				else if ((m_ptMouseCursor.y <= (LONG)m_sWorkingAreaElementSizes.dwMenu) && (m_ptMouseCursor.x < TOP_MENU_FILE_SIZE + TOP_MENU_OPTIONS_SIZE))
				{
					// top menu options
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
				}
				else if ((m_ptMouseCursor.x >(m_nWindowWidth - 32)) && (m_ptMouseCursor.y > (m_nWindowHeight - 32)))
				{
					// window resizt
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::WindowResize;
				}
				else if ((m_ptMouseCursor.x >= (ZOOM_CONTROL_LEFT + 50)) && (m_ptMouseCursor.x <= (ZOOM_CONTROL_RIGHT - 26)) && (m_ptMouseCursor.y >= 6) && (m_ptMouseCursor.y <= 26))
				{
					// workspace zoom
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::WorkspaceZoom;
				}
				else if ((m_ptMouseCursor.y >= (LONG)(m_nWindowHeight - (int)m_sWorkingAreaElementSizes.dwScrollBarBottom)) &&
					(m_ptMouseCursor.x >= (LONG)m_sWorkingAreaElementSizes.dwDataSheet) &&
					(m_ptMouseCursor.x <= (LONG)(m_nWindowWidth - (int)m_sWorkingAreaElementSizes.dwScrollBarRight)))
				{
					// scroll X
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::WorkspaceScrollX;

					// get the bar size and position
					float fBarSizeX;
					fBarSizeX = ((float)vcWorkspaceZoomWindowSize.cx / (float)m_vcWorkspaceSize.cx) * vcWorkspaceWindowSize.cx;
					float fBarPosX;
					fBarPosX = (m_fXScrollPos / (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx)) * ((float)vcWorkspaceWindowSize.cx - fBarSizeX);

					if ((m_ptMouseCursor.x < (LONG)m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX))
					{
						// new position
						fBarPosX -= fBarSizeX / 2.0f;
					}
					else if (m_ptMouseCursor.x > (LONG)m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX + (LONG)fBarSizeX)
					{
						// new position
						fBarPosX += fBarSizeX / 2.0f;
					}

					// set new scroll position
					m_fXScrollPos = ((float)fBarPosX / ((float)vcWorkspaceWindowSize.cx - fBarSizeX)) * (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx);

					// clamp position
					float fMaxXPos = (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx);
					if (m_fXScrollPos > fMaxXPos) m_fXScrollPos = fMaxXPos;
					if (m_fXScrollPos < 0.0f) m_fXScrollPos = 0.0f;
				}
				else if ((m_ptMouseCursor.x >= (LONG)(m_nWindowWidth - (int)m_sWorkingAreaElementSizes.dwScrollBarRight)) &&
					(m_ptMouseCursor.y >= (LONG)m_sWorkingAreaElementSizes.dwMenu) &&
					(m_ptMouseCursor.y <= (LONG)(m_nWindowHeight - (int)m_sWorkingAreaElementSizes.dwScrollBarBottom)))
				{
					// scroll y
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::WorkspaceScrollY;

					// get the bar size and position
					float fBarSizeY;
					fBarSizeY = ((float)vcWorkspaceZoomWindowSize.cy / (float)m_vcWorkspaceSize.cy) * vcWorkspaceWindowSize.cy;
					float fBarPosY;
					fBarPosY = (m_fYScrollPos / (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy)) * ((float)vcWorkspaceWindowSize.cy - fBarSizeY);

					if ((m_ptMouseCursor.y < (LONG)m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY))
					{
						// new position
						fBarPosY -= fBarSizeY / 2.0f;
					}
					else if (m_ptMouseCursor.y > (LONG)m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY + (LONG)fBarSizeY)
					{
						// new position
						fBarPosY += fBarSizeY / 2.0f;
					}

					// set new scroll position
					m_fYScrollPos = ((float)fBarPosY / ((float)vcWorkspaceWindowSize.cy - fBarSizeY)) * (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy);

					// clamp position
					float fMaxYPos = (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy);
					if (m_fYScrollPos > fMaxYPos) m_fYScrollPos = fMaxYPos;
					if (m_fYScrollPos < 0.0f) m_fYScrollPos = 0.0f;

				}
				else if (m_ptMouseCursor.y > (LONG)m_sWorkingAreaElementSizes.dwMenu)
				{
					if (m_ptMouseCursor.x < 16)
					{
						// data sheet category open/close
						bool b = false;
						int i = 0;
						while ((!b) && (i < (int)m_pcTransferSite->m_paDataSheetCategories.size()))
						{
							if ((m_ptMouseCursor.y >= m_pcTransferSite->m_paDataSheetCategories[i]->m_lYHeight) &&
								(m_ptMouseCursor.y <= m_pcTransferSite->m_paDataSheetCategories[i]->m_lYHeight + 16))
							{
								b = true;
								m_pcTransferSite->m_paDataSheetCategories[i]->m_bIsOpen = !m_pcTransferSite->m_paDataSheetCategories[i]->m_bIsOpen;
							}
							i++;
						}
					}
					else if (m_ptMouseCursor.x < 32)
					{
						// loop through categories
						for (std::vector<AQU_DataSheetEntry*>::size_type h = 0; h != m_pcTransferSite->m_paDataSheetCategories.size(); h++)
						{
							if (m_pcTransferSite->m_paDataSheetCategories[h]->m_bIsOpen)
							{
								// data sheet entry open/close
								bool b = false;
								int i = 0;
								while ((!b) && (i < (int)m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries.size()))
								{
									if ((m_ptMouseCursor.y >= m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_lYHeight) &&
										(m_ptMouseCursor.y <= m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_lYHeight + 16))
									{
										b = true;
										m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_bIsOpen = !m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_bIsOpen;
									}
									i++;
								}
							}
						}
					}
					else if (m_ptMouseCursor.x < (LONG)m_sWorkingAreaElementSizes.dwDataSheet)
					{
						bool b = false;

						// loop through categories
						for (std::vector<AQU_DataSheetEntry*>::size_type h = 0; h != m_pcTransferSite->m_paDataSheetCategories.size(); h++)
						{
							if (m_pcTransferSite->m_paDataSheetCategories[h]->m_bIsOpen)
							{
								// data sheet entry open/close
								int i = 0;
								while ((!b) && (i < (int)m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries.size()))
								{
									LONG lHeight = m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_lYHeight + 16;
									if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_bIsOpen) lHeight += (LONG)(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber * 16);
									if ((m_ptMouseCursor.y >= m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_lYHeight) &&
										(m_ptMouseCursor.y <= lHeight))
									{
										b = true;
										m_nDataSheetEntrySelection = (int)i;
										m_nDataSheetCategorySelection = (int)h;
									}
									i++;
								}
							}
						}

						// set no entry selection 
						if (b == false)
						{
							m_nDataSheetEntrySelection = -1;
							m_nDataSheetCategorySelection = -1;
						}
					}
					else
					{
						// if there are no other controls, get node index
						m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
					}
				}
				if (m_nActiveNodeIndex > -1)
				{
					// first update mouse cursor by calling node->IsLocal()
					m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
					s_UpdateActiveNode(msg, wParam, lParam);
				}
#pragma endregion
			}
			else
			{
#pragma region not idle
				// mouse events when not idle
				switch (m_eWorkingAreaStatus)
				{
#pragma region node active
					// active node ? 
					case AQU_WorkingAreaStatus::NodeActive:
						if (m_nActiveNodeIndex == -1)
							m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
						else
						{
							// first update mouse cursor by calling node->IsLocal()
							m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
							s_UpdateActiveNode(msg, wParam, lParam);
						}
						break;
#pragma endregion
#pragma region top menu bar file
					case AQU_WorkingAreaStatus::TopMenuBar_File:
						m_eWorkingAreaStatus = AQU_WorkingAreaStatus::ToIdle;

						// menu entry selected ?
						if (m_nActiveTopMenuFileEntry >= 0)
						{
							switch ((AQU_TopMenuBarFileEntries)m_nActiveTopMenuFileEntry)
							{
								case AQU_TopMenuBarFileEntries::SAVE_WORKING_AREA:
									// and save...
									m_pcTransferSite->m_pFileManager->SaveWorkingArea(m_pcTransferSite->m_pConfig, &m_paNodes, MAX_INTERFACES_NUMBER);
									break;
								case AQU_TopMenuBarFileEntries::COMPILE_PROFILE:
									m_pcTransferSite->m_pFileManager->CompileProfile(m_pcTransferSite->m_pConfig, &m_paNodes, MAX_INTERFACES_NUMBER);
									break;
								case AQU_TopMenuBarFileEntries::SAVE_EXTERNALLY:
									// set bool vice versa, do not kill the file menu so set back to file menu status
									m_pcTransferSite->m_pConfig->bExternalSave = !m_pcTransferSite->m_pConfig->bExternalSave;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_File;
								default:
									break;
							}
						}
						break;
#pragma endregion
#pragma region top menu bar options
					case AQU_WorkingAreaStatus::TopMenuBar_Options:
						m_eWorkingAreaStatus = AQU_WorkingAreaStatus::ToIdle;

						// menu entry selected ?
						if (m_nActiveTopMenuOptionsEntry >= 0)
						{
							switch ((AQU_TopMenuBarOptionsEntries)m_nActiveTopMenuOptionsEntry)
							{
								case AQU_TopMenuBarOptionsEntries::REINSTATE_INTERFACES:
									CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
									//CreateThread(NULL, 0, m_pcTransferSite->m_pD3D929ReinstateInterfaces, NULL, 0, NULL);
									CreateThread(NULL, 0, m_pcTransferSite->m_pD3D10ReinstateInterfaces, NULL, 0, NULL);
									CreateThread(NULL, 0, m_pcTransferSite->m_pD3D11ReinstateInterfaces, NULL, 0, NULL);
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_DATA_LINES:
									m_bDrawDataLines = true;
									m_bDrawDataSlings = false;
									m_bDrawDataPipelines = false;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_DATA_SLINGS:
									m_bDrawDataLines = false;
									m_bDrawDataSlings = true;
									m_bDrawDataPipelines = false;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_DATA_PIPELINES:
									m_bDrawDataLines = false;
									m_bDrawDataSlings = false;
									m_bDrawDataPipelines = true;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_DATA_DOTTED:
									m_bDrawDataDotted = !m_bDrawDataDotted;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_STREAM_LINES:
									m_bDrawStreamLines = true;
									m_bDrawStreamSlings = false;
									m_bDrawStreamPipelines = false;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_STREAM_SLINGS:
									m_bDrawStreamLines = false;
									m_bDrawStreamSlings = true;
									m_bDrawStreamPipelines = false;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_STREAM_PIPELINES:
									m_bDrawStreamLines = false;
									m_bDrawStreamSlings = false;
									m_bDrawStreamPipelines = true;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DRAW_STREAM_DOTTED:
									m_bDrawStreamDotted = !m_bDrawStreamDotted;
									m_eWorkingAreaStatus = AQU_WorkingAreaStatus::TopMenuBar_Options;
									break;
								case AQU_TopMenuBarOptionsEntries::DELETE_UNCONNECTED:
									// loop through nodes, delete all unconnected
									for (UINT unI = 0; unI < (UINT)m_paNodes.size(); unI++)
									{
										bool bConnected = false;
										// test all decommanders for connection
										for (UINT unJ = 0; unJ < (UINT)m_paNodes[unI]->m_paDecommanders.size(); unJ++)
											if (m_paNodes[unI]->m_paDecommanders[unJ]->m_paCommanders.size())
												bConnected = true;
										// test all commanders for connection
										for (UINT unJ = 0; unJ < (UINT)m_paNodes[unI]->m_paCommanders.size(); unJ++)
											if (m_paNodes[unI]->m_paCommanders[unJ]->m_paDecommanders.size())
												bConnected = true;
										// test provoker, invoker
										if (m_paNodes[unI]->GetProvokerConnectionsNumber() || m_paNodes[unI]->GetInvokerConnectionsNumber())
											bConnected = true;

										// exchange index with last node
										if ((!bConnected) && (unI < (m_paNodes.size() - 1)))
										{
											NOD_Basic* pcNodeToDelete = m_paNodes[unI];
											NOD_Basic* pcNodeToKeep = m_paNodes[m_paNodes.size() - 1];

											// set new index
											pcNodeToKeep->SetNewIndex((DWORD)unI);

											// set nodes new
											m_paNodes[unI] = pcNodeToKeep;
											m_paNodes[m_paNodes.size() - 1] = pcNodeToDelete;
										}

										if (!bConnected)
										{
											// delete last
											m_paNodes.erase(m_paNodes.end() - 1);

											// back one index to also verify the node to be kept
											unI--;
										}
									}
									break;
								default:
									break;
							}
						}
						break;
#pragma endregion
						// no action for WindowResize status here
				}
#pragma endregion
			}
			break;
#pragma endregion
#pragma region WM_LBUTTONUP
		case WM_LBUTTONUP:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			switch (m_eWorkingAreaStatus)
			{
				// active node ? 
				case AQU_WorkingAreaStatus::NodeActive:
					if (m_nActiveNodeIndex == -1)
						m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
					else
					{
						// are we in connection mode ? connect nodes here
						if (m_eActiveNodeBehavior == AQU_NodeBehavior::ConnectCommander)
						{
							// get destination node index
							int	nDestNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);

							// only connect if this in NOT the same node
							if ((nDestNodeIndex != m_nActiveNodeIndex) && (nDestNodeIndex > -1))
								m_paNodes[nDestNodeIndex]->ConnectCommander(m_paNodes[m_nActiveNodeIndex], nDestNodeIndex);
						}

						// are we in connection mode ? connect nodes here
						if (m_eActiveNodeBehavior == AQU_NodeBehavior::ConnectProvoker)
						{
							// get destination node index
							int	nDestNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);

							// only connect if this in NOT the same node
							if ((nDestNodeIndex != m_nActiveNodeIndex) && (nDestNodeIndex > -1))
								m_paNodes[nDestNodeIndex]->ConnectProvoker(m_paNodes[m_nActiveNodeIndex], nDestNodeIndex);
						}

						// update active node, first update mouse cursor by calling node->IsLocal()
						m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
						s_UpdateActiveNode(msg, wParam, lParam);
					}
					break;
				case AQU_WorkingAreaStatus::WindowResize:
				case AQU_WorkingAreaStatus::WorkspaceZoom:
				case AQU_WorkingAreaStatus::WorkspaceScrollX:
				case AQU_WorkingAreaStatus::WorkspaceScrollY:
					m_eWorkingAreaStatus = AQU_WorkingAreaStatus::ToIdle;
					break;
			}
			break;
#pragma endregion
#pragma region WM_LBUTTONDBLCLK
		case WM_LBUTTONDBLCLK:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// if there are no other controls, get node index
			m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);

			// update node
			if (m_nActiveNodeIndex > -1)
			{
				// first update mouse cursor by calling node->IsLocal()
				m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
				s_UpdateActiveNode(msg, wParam, lParam);
			}
			else
			{
				// create node if d3d interface method selected in the data sheet
				if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1) && (m_nDataSheetCategorySelection < (int)m_pcTransferSite->m_aInterfaceIndices.size()))
				{
					// node already present ?
					if (!m_pcTransferSite->IsD3DNodePresent((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection], m_nDataSheetEntrySelection))
					{
						// get node provider
						AQU_NodeProvider* pProvider = new AQU_NodeProvider();

						// create node
						NOD_Basic* pNode = pProvider->Get_D3D_Node((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection],
							(UINT)m_nDataSheetEntrySelection,
							(LONG)(float(m_ptMouseCursor.x - vcWorkspaceOrigin.x) / m_fZoom),
							(LONG)(float(m_ptMouseCursor.y - vcWorkspaceOrigin.y) / m_fZoom));

						// delete provider
						delete pProvider;

						// register the node
						m_pcTransferSite->RegisterD3DNode(pNode,
							(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection],
							m_nDataSheetEntrySelection);

						// add node
						m_paNodes.push_back(pNode);

					}
				}
				// create elementary node ?
				else if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1) && (m_nDataSheetCategorySelection == m_pcTransferSite->m_nElementaryNodesTabIndex))
				{
					// get node provider
					AQU_NodeProvider* pProvider = new AQU_NodeProvider();

					// create node
					NOD_Basic* pNode = pProvider->Get_Elementary_Node((AQU_ElementaryNodes)m_nDataSheetEntrySelection,
						(LONG)(float(m_ptMouseCursor.x - vcWorkspaceOrigin.x) / m_fZoom),
						(LONG)(float(m_ptMouseCursor.y - vcWorkspaceOrigin.y) / m_fZoom));

					// delete provider
					delete pProvider;

					// add node
					m_paNodes.push_back(pNode);
				}
				// create plugin node ?
				else if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1))
				{
					// get plugin node index
					DWORD dwIndex = 0;
					for (int i = 0; i < (int)m_pcTransferSite->m_aPluginCategoryIndices.size(); i++)
					{
						if (m_pcTransferSite->m_aPluginCategoryIndices[i] == m_nDataSheetCategorySelection)
						{
							if (m_pcTransferSite->m_aPluginEntryIndices[i] == m_nDataSheetEntrySelection)
								dwIndex = (DWORD)i;
						}
					}

					// create plugin node
					NOD_Plugin* pPlugin = new NOD_Plugin((LONG)(float(m_ptMouseCursor.x - vcWorkspaceOrigin.x) / m_fZoom),
						(LONG)(float(m_ptMouseCursor.y - vcWorkspaceOrigin.y) / m_fZoom),
						m_vcPluginFilePathes[dwIndex]);

					// add node
					m_paNodes.push_back(pPlugin);
				}
			}
			break;
#pragma endregion
#pragma region WM_RBUTTONDOWN
		case WM_RBUTTONDOWN:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// if there are no other controls, get node index
			m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);

			// update node
			if (m_nActiveNodeIndex > -1)
			{
				// first update mouse cursor by calling node->IsLocal()
				m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
				s_UpdateActiveNode(msg, wParam, lParam);
			}
			break;
#pragma endregion 
#pragma region WM_RBUTTONDBLCLK
		case WM_RBUTTONDBLCLK:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// if there are no other controls, get node index
			m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);

			// update node
			if (m_nActiveNodeIndex > -1)
			{
				bool bConnected = false;
				// test all decommanders for connection
				for (UINT unJ = 0; unJ < (UINT)m_paNodes[m_nActiveNodeIndex]->m_paDecommanders.size(); unJ++)
					if (m_paNodes[m_nActiveNodeIndex]->m_paDecommanders[unJ]->m_paCommanders.size())
						bConnected = true;
				// test all commanders for connection
				for (UINT unJ = 0; unJ < (UINT)m_paNodes[m_nActiveNodeIndex]->m_paCommanders.size(); unJ++)
					if (m_paNodes[m_nActiveNodeIndex]->m_paCommanders[unJ]->m_paDecommanders.size())
						bConnected = true;
				// test provoker, invoker
				if (m_paNodes[m_nActiveNodeIndex]->GetProvokerConnectionsNumber() || m_paNodes[m_nActiveNodeIndex]->GetInvokerConnectionsNumber())
					bConnected = true;

				// exchange index with last node
				if ((!bConnected) && (m_nActiveNodeIndex < (int)(m_paNodes.size() - 1)))
				{
					NOD_Basic* pcNodeToDelete = m_paNodes[m_nActiveNodeIndex];
					NOD_Basic* pcNodeToKeep = m_paNodes[m_paNodes.size() - 1];

					// set new index
					pcNodeToKeep->SetNewIndex((DWORD)m_nActiveNodeIndex);

					// set nodes new
					m_paNodes[m_nActiveNodeIndex] = pcNodeToKeep;
					m_paNodes[m_paNodes.size() - 1] = pcNodeToDelete;
				}

				if (!bConnected)
				{
					// delete last
					m_paNodes.erase(m_paNodes.end() - 1);
				}

				// get node index again for safety
				m_nActiveNodeIndex = s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
			}
			break;
#pragma endregion
#pragma region WM_MOUSEWHEEL
		case WM_MOUSEWHEEL:
			// get the mouse cursor
			m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// for some reason mouse coords for mouse wheel event are screen based
			RECT rcWnd;
			GetWindowRect(hwnd, &rcWnd);
			m_ptMouseCursor.x -= rcWnd.left;
			m_ptMouseCursor.y -= rcWnd.top;

			// idle status ? 
			if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::Idle)
			{
				// get the wheel delta
				short nDelta = GET_WHEEL_DELTA_WPARAM(wParam);

				if (m_ptMouseCursor.x < (LONG)m_sWorkingAreaElementSizes.dwDataSheet)
				{
					// data sheet scroll
					m_fYScrollDataSheet += (float)nDelta / 8.0f;

					if (m_fYScrollDataSheet > 0.0f) m_fYScrollDataSheet = 0.0f;
				}
				else
				{
					// get old float workspace size
					float fXWorkspaceSize = (float)vcWorkspaceWindowSize.cx / m_fZoom;
					float fYWorkspaceSize = (float)vcWorkspaceWindowSize.cy / m_fZoom;

					// set new zoom
					m_fZoom += (float)nDelta / 1000.0f;

					// clamp zoom
					if (m_fZoom < 0.5f) m_fZoom = 0.5f;
					if (m_fZoom > 3.05f) m_fZoom = 3.05f;

					// get new float workspace size
					float fXWorkspaceSizeNew = (float)vcWorkspaceWindowSize.cx / m_fZoom;
					float fYWorkspaceSizeNew = (float)vcWorkspaceWindowSize.cy / m_fZoom;

					// center zoom - first get center by mouse position
					LONG nCursorX = m_ptMouseCursor.x - m_sWorkingAreaElementSizes.dwDataSheet - 4;
					LONG nCursorY = m_ptMouseCursor.y - m_sWorkingAreaElementSizes.dwMenu - 4;
					if (nCursorX < 0) nCursorX = 0;
					if (nCursorY < 0) nCursorY = 0;
					float fXZenter = float(nCursorX) / float(vcWorkspaceWindowSize.cx);
					float fYZenter = float(nCursorY) / float(vcWorkspaceWindowSize.cy);
					m_fXScrollPos += fXWorkspaceSize * fXZenter;
					m_fXScrollPos -= fXWorkspaceSizeNew * fXZenter;
					m_fYScrollPos += fYWorkspaceSize * fYZenter;
					m_fYScrollPos -= fYWorkspaceSizeNew * fYZenter;

					// clamp position
					if (m_fXScrollPos < 0.0f) m_fXScrollPos = 0.0f;
					if (m_fYScrollPos < 0.0f) m_fYScrollPos = 0.0f;
				}
			}
			break;
#pragma endregion
#pragma region WM_MOUSEMOVE
		case WM_MOUSEMOVE:

			// avoid out-of-window mouse button up mistakes
			if (!GetAsyncKeyState(VK_LBUTTON))
			{
				switch (m_eWorkingAreaStatus)
				{
					case AQU_WorkingAreaStatus::WindowResize:
					case AQU_WorkingAreaStatus::WorkspaceZoom:
					case AQU_WorkingAreaStatus::WorkspaceScrollX:
					case AQU_WorkingAreaStatus::WorkspaceScrollY:
						// get the mouse cursor
						m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
						m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

						// set idle
						m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
						break;
					default:
						break;
				}
			}

			// active node ?
			if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::NodeActive)
			{
				// get the mouse cursor
				m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
				m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

				// first update mouse cursor by calling node->IsLocal()
				m_paNodes[m_nActiveNodeIndex]->IsLocal(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
				s_UpdateActiveNode(msg, wParam, lParam);
			}
			else
				// mouse button down while moving ?
				if (wParam&MK_LBUTTON)
				{
					// get new mouse cursor point
					LONG nX = (LONG)GET_X_LPARAM(lParam);
					LONG nY = (LONG)GET_Y_LPARAM(lParam);

					// Working Area Status ??
					switch (m_eWorkingAreaStatus)
					{
#pragma region window resize
						case AQU_WorkingAreaStatus::WindowResize:
							// set window size fields
							m_nWindowWidth = nX + 30;
							m_nWindowHeight = nY + 30;

							// minimum window size
							if (m_nWindowWidth < 600) m_nWindowWidth = 600;
							if (m_nWindowHeight < 200) m_nWindowHeight = 200;

							// resize the back buffer
							m_pDirectDraw->Resize(m_nWindowWidth, m_nWindowHeight);

							// set the new window size
							SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, m_nWindowWidth, m_nWindowHeight, SWP_NOMOVE);
							break;
#pragma endregion
#pragma region workspace zoom
						case AQU_WorkingAreaStatus::WorkspaceZoom:
							if (true)
							{
								// get old float workspace size
								float fXWorkspaceSize = (float)vcWorkspaceWindowSize.cx / m_fZoom;
								float fYWorkspaceSize = (float)vcWorkspaceWindowSize.cy / m_fZoom;

								// set new zoom
								m_fZoom = float(nX - ZOOM_CONTROL_LEFT) / 100.0f;

								// clamp zoom
								if (m_fZoom < 0.5f) m_fZoom = 0.5f;
								if (m_fZoom > 3.05f) m_fZoom = 3.05f;

								// get new float workspace size
								float fXWorkspaceSizeNew = (float)vcWorkspaceWindowSize.cx / m_fZoom;
								float fYWorkspaceSizeNew = (float)vcWorkspaceWindowSize.cy / m_fZoom;

								// center zoom
								m_fXScrollPos += fXWorkspaceSize / 2.0f;
								m_fXScrollPos -= fXWorkspaceSizeNew / 2.0f;
								m_fYScrollPos += fYWorkspaceSize / 2.0f;
								m_fYScrollPos -= fYWorkspaceSizeNew / 2.0f;

								// clamp position
								float fMaxXPos = (float)m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx;
								float fMaxYPos = (float)m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy;
								if (m_fXScrollPos > fMaxXPos) m_fXScrollPos = fMaxXPos;
								if (m_fXScrollPos < 0.0f) m_fXScrollPos = 0.0f;
								if (m_fYScrollPos > fMaxYPos) m_fYScrollPos = fMaxYPos;
								if (m_fYScrollPos < 0.0f) m_fYScrollPos = 0.0f;

								// call getnodeindex to update new zoom to nodes
								s_GetNodeIndex(m_ptMouseCursor, vcWorkspaceOrigin, m_fZoom);
							}
							break;
#pragma endregion
#pragma region workspace scroll x
						case AQU_WorkingAreaStatus::WorkspaceScrollX:
							if (true)
							{
								// get the bar size and position
								float fBarSizeX;
								fBarSizeX = ((float)vcWorkspaceZoomWindowSize.cx / (float)m_vcWorkspaceSize.cx) * vcWorkspaceWindowSize.cx;
								float fBarPosX;
								fBarPosX = (m_fXScrollPos / (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx)) * ((float)vcWorkspaceWindowSize.cx - fBarSizeX);

								if ((nX >= (LONG)m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX) &&
									(nX <= (LONG)m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX + (LONG)fBarSizeX))
								{
									// new position
									fBarPosX -= (m_ptMouseCursor.x - nX);

									// set new scroll position
									m_fXScrollPos = ((float)fBarPosX / ((float)vcWorkspaceWindowSize.cx - fBarSizeX)) * (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx);

									// clamp position
									float fMaxXPos = (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx);
									if (m_fXScrollPos > fMaxXPos) m_fXScrollPos = fMaxXPos;
									if (m_fXScrollPos < 0.0f) m_fXScrollPos = 0.0f;
								}

								// get the mouse cursor
								m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
								m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);
							}
							break;
#pragma endregion
#pragma region workspace scroll y
						case AQU_WorkingAreaStatus::WorkspaceScrollY:
							if (true)
							{
								// get the bar size and position
								float fBarSizeY;
								fBarSizeY = ((float)vcWorkspaceZoomWindowSize.cy / (float)m_vcWorkspaceSize.cy) * vcWorkspaceWindowSize.cy;
								float fBarPosY;
								fBarPosY = (m_fYScrollPos / (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy)) * ((float)vcWorkspaceWindowSize.cy - fBarSizeY);

								if ((nY >= (LONG)m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY) &&
									(nY <= (LONG)m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY + (LONG)fBarSizeY))
								{
									// new position
									fBarPosY -= (m_ptMouseCursor.y - nY);

									// set new scroll position
									m_fYScrollPos = ((float)fBarPosY / ((float)vcWorkspaceWindowSize.cy - fBarSizeY)) * (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy);

									// clamp position
									float fMaxYPos = (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy);
									if (m_fYScrollPos > fMaxYPos) m_fYScrollPos = fMaxYPos;
									if (m_fYScrollPos < 0.0f) m_fYScrollPos = 0.0f;
								}

								// get the mouse cursor
								m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
								m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);
							}
							break;
#pragma endregion
#pragma region idle
						case AQU_WorkingAreaStatus::Idle:
							// get the difference to the old position set only in WM_LBUTTONDOWN
							nX -= m_ptMouseCursor.x;
							nY -= m_ptMouseCursor.y;

							// mouse cursor in working area ?
							if ((m_ptMouseCursor.x > (LONG)m_sWorkingAreaElementSizes.dwDataSheet) &&
								(m_ptMouseCursor.x < (LONG)(m_nWindowWidth - (int)m_sWorkingAreaElementSizes.dwScrollBarRight)) &&
								(m_ptMouseCursor.y >= (LONG)m_sWorkingAreaElementSizes.dwMenu) &&
								(m_ptMouseCursor.y <= (LONG)(m_nWindowHeight - (int)m_sWorkingAreaElementSizes.dwScrollBarBottom)))
							{
								// move working area				
								m_fXScrollPos -= (float)nX / m_fZoom;
								m_fYScrollPos -= (float)nY / m_fZoom;
								if (m_fXScrollPos < 0.0f) m_fXScrollPos = 0.0f;
								if (m_fYScrollPos < 0.0f) m_fYScrollPos = 0.0f;

								// get the mouse cursor
								m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
								m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);
							}
							else
							{
								// get the old window position
								RECT rcWnd;
								GetWindowRect(hwnd, &rcWnd);

								// set the new window position
								SetWindowPos(hwnd, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
							}
							break;
#pragma endregion
#pragma region to idle
						case AQU_WorkingAreaStatus::ToIdle:
							// get the mouse cursor
							m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
							m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

							// set idle
							m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
							break;
#pragma endregion
					}
				}
				else
				{
					// get the mouse cursor
					m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
					m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

					// waiting for idle ?
					if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::ToIdle)
						m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
				}
			break;
#pragma endregion
#pragma region WM_CLOSE
		case WM_CLOSE:
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = NULL;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
#pragma endregion
#pragma region WM_DESTROY
		case WM_DESTROY:
			for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
			{
				delete m_paNodes[i];
			}
			m_paNodes.clear();
			m_pDirectDraw->Release();
			PostQuitMessage(0);
			break;
#pragma endregion
#pragma region WM_PAINT
		case WM_PAINT:
		{
			if (!m_pDirectDraw) break;

			// check for lost surfaces
			m_pDirectDraw->VerifyAPI();

			// clear the back buffer
			m_pDirectDraw->Clear(m_nWindowWidth, m_nWindowHeight);

#pragma region grid
			// draw the grid
			for (LONG nX = 0; nX < (vcWorkspaceZoomWindowSize.cx + 200); nX += (LONG)200)
			{
				for (LONG nY = 0; nY < (vcWorkspaceZoomWindowSize.cy + 200); nY += (LONG)200)
				{
					POINT sTop = { nX + (LONG)(m_fXScrollPos / 200.0f) * 200, nY + (LONG)(m_fYScrollPos / 200.0f) * 200 };
					m_pDirectDraw->DrawHLine(sTop, vcWorkspaceOrigin, 4, 4, AquilinusColor::Color1, m_fZoom);
				}
			}
#pragma endregion

#pragma region background info
			// draw the background information.. TODO !! do this using m_pcTransferSite->m_aInterfaceIndices.
			y = 16;
			for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
			{
				if (m_pcTransferSite->m_pConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
				{
					wchar_t buffer[64];
					wsprintfW(buffer, L"%d", m_pcTransferSite->m_anInterfaceRefCount[i]);
					m_pDirectDraw->RenderText(buffer, 16, y, vcWorkspaceOrigin, AquilinusColor::Color4, AquilinusColor::Color0, AQUILINUS_TINY_TEXT_SIZE, m_fZoom);
					m_pDirectDraw->RenderText(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].szName, 64, y, vcWorkspaceOrigin, AquilinusColor::Color4, AquilinusColor::Color0, AQUILINUS_TINY_TEXT_SIZE, m_fZoom);
					y += 16;
				}
			}
#pragma endregion

#pragma region connectors+nodes
			// draw the connectors, first set drawing styles
			AQU_ConnectionDrawingStyles eDataStyle;
			AQU_ConnectionDrawingStyles eStreamStyle;

			if (m_bDrawDataLines)
			{
				if (m_bDrawDataDotted)
					eDataStyle = AQU_ConnectionDrawingStyles::DottedLine;
				else
					eDataStyle = AQU_ConnectionDrawingStyles::Line;
			}
			else if (m_bDrawDataSlings)
			{
				if (m_bDrawDataDotted)
					eDataStyle = AQU_ConnectionDrawingStyles::DottedSling;
				else
					eDataStyle = AQU_ConnectionDrawingStyles::Sling;
			}
			else
			{
				if (m_bDrawDataDotted)
					eDataStyle = AQU_ConnectionDrawingStyles::DottedPipeline;
				else
					eDataStyle = AQU_ConnectionDrawingStyles::Pipeline;
			}

			if (m_bDrawStreamLines)
			{
				if (m_bDrawStreamDotted)
					eStreamStyle = AQU_ConnectionDrawingStyles::DottedLine;
				else
					eStreamStyle = AQU_ConnectionDrawingStyles::Line;
			}
			else if (m_bDrawStreamSlings)
			{
				if (m_bDrawStreamDotted)
					eStreamStyle = AQU_ConnectionDrawingStyles::DottedSling;
				else
					eStreamStyle = AQU_ConnectionDrawingStyles::Sling;
			}
			else
			{
				if (m_bDrawStreamDotted)
					eStreamStyle = AQU_ConnectionDrawingStyles::DottedPipeline;
				else
					eStreamStyle = AQU_ConnectionDrawingStyles::Pipeline;
			}

			// first, the currently held connector
			if ((m_eActiveNodeBehavior == AQU_NodeBehavior::ConnectCommander) ||
				(m_eActiveNodeBehavior == AQU_NodeBehavior::ConnectProvoker))
			{
				// get cursor (correct zoom,window rect, origin), draw edge
				POINT vecC;
				GetCursorPos(&vecC);
				RECT rcWin;
				GetWindowRect(hwnd, &rcWin);
				vecC.x = (LONG)((float)(vecC.x - rcWin.left - vcWorkspaceOrigin.x) / m_fZoom);
				vecC.y = (LONG)((float)(vecC.y - rcWin.top - vcWorkspaceOrigin.y) / m_fZoom);

				// get connection origin
				POINT vecO = m_paNodes[m_nActiveNodeIndex]->GetConnectionOrigin();

				// adjust y to joint middle
				vecO.y += 2;
				vecC.y += 2;

				// draw the connection depending wether it is a commander connection or a provoker connection
				if (m_eActiveNodeBehavior == AQU_NodeBehavior::ConnectCommander)
				{
					switch (eDataStyle)
					{
						case Line:
							m_pDirectDraw->DrawLine(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, m_fZoom);
							break;
						case Sling:
							//m_pDirectDraw->DrawEdge(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, true, m_fZoom);
							break;
						case Pipeline:
							m_pDirectDraw->DrawPipeline(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, m_fZoom);
							break;
						case DottedLine:
							m_pDirectDraw->DrawLine(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, m_fZoom);
							break;
						case DottedSling:
							//m_pDirectDraw->DrawEdge(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, true, m_fZoom);
							break;
						case DottedPipeline:
							m_pDirectDraw->DrawPipeline(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color4, m_fZoom);
							break;
						default:
							break;
					}
				}
				else
				{
					switch (eStreamStyle)
					{
						case Line:
							m_pDirectDraw->DrawLine(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, m_fZoom);
							break;
						case Sling:
							//m_pDirectDraw->DrawEdge(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, true, m_fZoom);
							break;
						case Pipeline:
							m_pDirectDraw->DrawPipeline(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, m_fZoom);
							break;
						case DottedLine:
							m_pDirectDraw->DrawLine(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, m_fZoom);
							break;
						case DottedSling:
							//m_pDirectDraw->DrawEdge(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, true, m_fZoom);
							break;
						case DottedPipeline:
							m_pDirectDraw->DrawPipeline(vecO, vecC, vcWorkspaceOrigin, 4, AquilinusColor::Color2, m_fZoom);
							break;
						default:
							break;
					}
				}
			}

			// loop throug nodes, draw all connectors
			for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
			{
				m_paNodes[i]->DrawConnections(m_pDirectDraw, vcWorkspaceOrigin, m_fZoom, &m_paNodes, eDataStyle, eStreamStyle);
			}

			// draw the nodes
			for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
			{
				m_paNodes[i]->Draw(m_pDirectDraw, vcWorkspaceOrigin, m_fZoom);
			}
#pragma endregion

#pragma region datasheet
			// draw the bounding border
			RECT rcDest;

			// clear background of data sheet
			SetRect(&rcDest, 0, m_sWorkingAreaElementSizes.dwMenu - 1, m_sWorkingAreaElementSizes.dwDataSheet + 1, m_nWindowHeight);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);

			// draw the datasheet
			LONG nY;
			nY = m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)m_fYScrollDataSheet;

			// set maximum text width to the datasheet width
			m_pDirectDraw->SetTextRange(m_sWorkingAreaElementSizes.dwDataSheet);

			// loop through categories
			for (std::vector<AQU_DataSheetEntry*>::size_type h = 0; h != m_pcTransferSite->m_paDataSheetCategories.size(); h++)
			{
				// render title, set entry height
				m_pDirectDraw->RenderText(m_pcTransferSite->m_paDataSheetCategories[h]->m_szTitle, 16, nY, vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, AQUILINUS_SMALLER_TEXT_SIZE, 1.0f);
				m_pcTransferSite->m_paDataSheetCategories[h]->m_lYHeight = nY;

				// render minus
				SetRect(&rcDest, 4, nY + 6, 12, nY + 8);
				m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);

				if (m_pcTransferSite->m_paDataSheetCategories[h]->m_bIsOpen)
				{
					nY += 16;

					// loop through entries
					for (std::vector<AQU_DataSheetEntry*>::size_type i = 0; i != m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries.size(); i++)
					{
						// is this the current selection ?
						AquilinusColor ac = AquilinusColor::Color0;
						if ((m_nDataSheetCategorySelection == (int)h) && (m_nDataSheetEntrySelection == (int)i))
						{
							if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_bIsOpen)
								SetRect(&rcDest, 1, nY, (LONG)m_sWorkingAreaElementSizes.dwDataSheet - 4, nY + 16 + (LONG)(16 * m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber));
							else
								SetRect(&rcDest, 1, nY, (LONG)m_sWorkingAreaElementSizes.dwDataSheet - 4, nY + 16);
							m_pDirectDraw->Clear(rcDest, AquilinusColor::Color1);
							ac = AquilinusColor::Color1;
						}

						// render title, set entry height
						m_pDirectDraw->RenderText(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_szTitle, 32, nY, vcZeroOrigin, AquilinusColor::Color3, ac, AQUILINUS_SMALLER_TEXT_SIZE, 1.0f);
						m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_lYHeight = nY;

						// sub entries ?
						if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber > 0)
						{
							// render minus
							SetRect(&rcDest, 20, nY + 6, 28, nY + 8);
							m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);

							if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_bIsOpen)
							{
								nY += 16;

								// render sub-entries
								for (int j = 0; j < (int)m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber; j++)
								{
									m_pDirectDraw->RenderText(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_paSubEntries[j], 48, nY, vcZeroOrigin, AquilinusColor::Color3, ac, AQUILINUS_SMALLER_TEXT_SIZE, 1.0f);
									nY += 16;
								}
							}
							else
							{
								// render plus 
								SetRect(&rcDest, 23, nY + 3, 25, nY + 11);
								m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);

								nY += 16;
							}
						}
						else nY += 16;
					}
				}
				else
				{
					// render plus 
					SetRect(&rcDest, 7, nY + 3, 9, nY + 11);
					m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);

					nY += 16;
				}
			}
#pragma endregion

#pragma region controls
			// set max text width back (zero = window width)
			m_pDirectDraw->SetTextRange(0);

			// clear all other backgrounds
			SetRect(&rcDest, m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight - 1, m_sWorkingAreaElementSizes.dwMenu, m_nWindowWidth, m_nWindowHeight);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);
			SetRect(&rcDest, 0, 0, m_nWindowWidth, m_sWorkingAreaElementSizes.dwMenu + 1);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);
			SetRect(&rcDest, m_sWorkingAreaElementSizes.dwDataSheet + 1, m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom - 1, m_nWindowWidth, m_nWindowHeight);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);

			// draw lines
			SetRect(&rcDest, m_sWorkingAreaElementSizes.dwDataSheet - 3, m_sWorkingAreaElementSizes.dwMenu - 1, m_sWorkingAreaElementSizes.dwDataSheet - 1, m_nWindowHeight);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2); rcDest.left += 4; rcDest.right += 4; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2);
			SetRect(&rcDest, m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight - 3, m_sWorkingAreaElementSizes.dwMenu, m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight - 1, m_nWindowHeight);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2); rcDest.left += 4; rcDest.right += 4; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2);
			SetRect(&rcDest, 0, m_sWorkingAreaElementSizes.dwMenu - 3, m_nWindowWidth, m_sWorkingAreaElementSizes.dwMenu - 1);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2); rcDest.top += 4; rcDest.bottom += 4; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2);
			SetRect(&rcDest, m_sWorkingAreaElementSizes.dwDataSheet - 1, m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom - 3, m_nWindowWidth, m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom - 1);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2); rcDest.top += 4; rcDest.bottom += 4; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color2);

			// resizing sign
			SetRect(&rcDest, m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight + 8, m_nWindowHeight - 10, m_nWindowWidth - 8, m_nWindowHeight - 8);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3); rcDest.top -= 5; rcDest.bottom -= 5; rcDest.right -= 5; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);
			SetRect(&rcDest, m_nWindowWidth - 10, m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom + 8, m_nWindowWidth - 8, m_nWindowHeight - 8);
			m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3); rcDest.left -= 5; rcDest.bottom -= 5; rcDest.right -= 5; m_pDirectDraw->Clear(rcDest, AquilinusColor::Color3);

			// draw a global window border
			SetRect(&rcDest, 0, 0, m_nWindowWidth - 1, m_nWindowHeight - 1);
			m_pDirectDraw->DrawBox(rcDest, vcZeroOrigin, 1, AquilinusColor::Color2, 1.0f);

			// draw the zoom slider 
			m_pDirectDraw->RenderText(L"- <", ZOOM_CONTROL_LEFT + 16, 0, vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, AQUILINUS_MEDIUM_TEXT_SIZE, 1.0f);
			m_pDirectDraw->RenderText(L"> +", ZOOM_CONTROL_RIGHT - 12, 0, vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, AQUILINUS_MEDIUM_TEXT_SIZE, 1.0f);
			POINT vcZoomLine;
			vcZoomLine.x = ZOOM_CONTROL_LEFT + 50;
			vcZoomLine.y = 14;
			m_pDirectDraw->DrawHLine(vcZoomLine, vcZeroOrigin, 256, 4, AquilinusColor::Color3, 1.0f);
			vcZoomLine.y = 6;
			vcZoomLine.x = ZOOM_CONTROL_LEFT + (LONG)(100.0f * m_fZoom);
			m_pDirectDraw->DrawVLine(vcZoomLine, vcZeroOrigin, 20, 4, AquilinusColor::Color4, 1.0f);
#pragma endregion

#pragma region top menu bar
			// before drawing the menu bar, set entry selections to -1
			m_nActiveTopMenuFileEntry = -1;
			m_nActiveTopMenuOptionsEntry = -1;

			// draw the top menu bar...first >File<; get the line size and set the destination rectangle origin
			DWORD dwLineSize = (DWORD)((float)AQUILINUS_SMALL_TEXT_SIZE*AQUILINUS_FONT_PIXELSIZE);
			SetRect(&rcDest, 1, 1, TOP_MENU_FILE_SIZE, m_sWorkingAreaElementSizes.dwMenu - 4);

			// if >idle< just output the top menu bar name, else the whole menu
			if ((m_eWorkingAreaStatus == AQU_WorkingAreaStatus::Idle) && (InRect(rcDest, m_ptMouseCursor)))
			{
				m_pDirectDraw->Clear(rcDest, AquilinusColor::Color1);
				m_pDirectDraw->RenderText(L"File", 10, 3, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);
			}
			else if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::TopMenuBar_File)
			{
				m_pDirectDraw->RenderText(L"File", 10, 3, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);
				m_pDirectDraw->DrawBox(rcDest, vcZeroOrigin, 1, AquilinusColor::Color2, 1.0f);

				// draw entries... first draw the whole box for the entries
				rcDest.top += m_sWorkingAreaElementSizes.dwMenu;
				rcDest.bottom = rcDest.top + (DWORD)AQU_TopMenuBarFileEntries::NUMBER_OF_FILE_ENTRIES*(DWORD)((float)AQUILINUS_SMALL_TEXT_SIZE*AQUILINUS_FONT_PIXELSIZE);
				rcDest.right = SUB_MENU_FILE_SIZE;
				m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);
				m_pDirectDraw->DrawBox(rcDest, vcZeroOrigin, 1, AquilinusColor::Color2, 1.0f);

				// set the first entry box, loop through entries
				rcDest.bottom = rcDest.top - 1 + (DWORD)((float)AQUILINUS_SMALL_TEXT_SIZE*AQUILINUS_FONT_PIXELSIZE);
				for (int i = 0; i < (int)AQU_TopMenuBarFileEntries::NUMBER_OF_FILE_ENTRIES; i++)
				{
					// draw the text a little more right if this entry is highlighted
					LONG nX = 10;
					if (*m_pbHighlightEntryFile[i]) nX += 16;

					// draw a box if mouse cursor hits that entry, render the text
					if (InRect(rcDest, m_ptMouseCursor))
					{
						m_pDirectDraw->Clear(rcDest, AquilinusColor::Color1);
						m_pDirectDraw->RenderText(g_aszTopMenuFileEntries[i], nX, rcDest.top + 4, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);

						// set entry selection
						m_nActiveTopMenuFileEntry = i;
					}
					else
						m_pDirectDraw->RenderText(g_aszTopMenuFileEntries[i], nX, rcDest.top + 4, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);

					// draw a highlight indicator
					if (*m_pbHighlightEntryFile[i])
					{
						RECT rcIndicator;
						SetRect(&rcIndicator,
							rcDest.left + 4,
							rcDest.top + (dwLineSize >> 2),
							rcDest.left + 16,
							rcDest.bottom - (dwLineSize >> 2));
						m_pDirectDraw->Clear(rcIndicator, AquilinusColor::Color2);
					}

					// set new entry box
					rcDest.top = rcDest.bottom + 1;
					rcDest.bottom += dwLineSize;
				}
			}
			else
				m_pDirectDraw->RenderText(L"File", 10, 3, vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);

			// next....>Options<; update the origin rectangle
			SetRect(&rcDest, TOP_MENU_FILE_SIZE + 1, 1, TOP_MENU_FILE_SIZE + TOP_MENU_OPTIONS_SIZE, m_sWorkingAreaElementSizes.dwMenu - 4);

			// if >idle< just output the top menu bar name, else the whole menu
			if ((m_eWorkingAreaStatus == AQU_WorkingAreaStatus::Idle) && (InRect(rcDest, m_ptMouseCursor)))
			{
				m_pDirectDraw->Clear(rcDest, AquilinusColor::Color1);
				m_pDirectDraw->RenderText(L"Options", TOP_MENU_FILE_SIZE + 10, 3, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);
			}
			else if (m_eWorkingAreaStatus == AQU_WorkingAreaStatus::TopMenuBar_Options)
			{
				m_pDirectDraw->RenderText(L"Options", TOP_MENU_FILE_SIZE + 10, 3, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);
				m_pDirectDraw->DrawBox(rcDest, vcZeroOrigin, 1, AquilinusColor::Color2, 1.0f);

				// draw entries... first draw the whole box for the entries
				rcDest.top += m_sWorkingAreaElementSizes.dwMenu;
				rcDest.bottom = rcDest.top + (DWORD)AQU_TopMenuBarOptionsEntries::NUMBER_OF_OPTIONS_ENTRIES*(DWORD)((float)AQUILINUS_SMALL_TEXT_SIZE*AQUILINUS_FONT_PIXELSIZE);
				rcDest.right = TOP_MENU_FILE_SIZE + SUB_MENU_FILE_SIZE;
				m_pDirectDraw->Clear(rcDest, AquilinusColor::Color0);
				m_pDirectDraw->DrawBox(rcDest, vcZeroOrigin, 1, AquilinusColor::Color2, 1.0f);

				// set the first entry box, loop through entries
				rcDest.bottom = rcDest.top - 1 + (DWORD)((float)AQUILINUS_SMALL_TEXT_SIZE*AQUILINUS_FONT_PIXELSIZE);
				for (int i = 0; i < (int)AQU_TopMenuBarOptionsEntries::NUMBER_OF_OPTIONS_ENTRIES; i++)
				{
					// draw the text a little more right if this entry is highlighted
					LONG nX = 10;
					if (*m_pbHighlightEntryOptions[i]) nX += 16;

					// draw a box if mouse cursor hits that entry, render the text
					if (InRect(rcDest, m_ptMouseCursor))
					{
						m_pDirectDraw->Clear(rcDest, AquilinusColor::Color1);
						m_pDirectDraw->RenderText(g_aszTopMenuOptionsEntries[i], TOP_MENU_FILE_SIZE + nX, rcDest.top + 4, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);

						// set entry selection
						m_nActiveTopMenuOptionsEntry = i;
					}
					else
						m_pDirectDraw->RenderText(g_aszTopMenuOptionsEntries[i], TOP_MENU_FILE_SIZE + nX, rcDest.top + 4, vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);

					// draw a highlight indicator
					if (*m_pbHighlightEntryOptions[i])
					{
						RECT rcIndicator;
						SetRect(&rcIndicator,
							rcDest.left + 4,
							rcDest.top + (dwLineSize >> 2),
							rcDest.left + 16,
							rcDest.bottom - (dwLineSize >> 2));
						m_pDirectDraw->Clear(rcIndicator, AquilinusColor::Color2);
					}

					// set new entry box
					rcDest.top = rcDest.bottom + 1;
					rcDest.bottom += dwLineSize;
				}
			}
			else
				m_pDirectDraw->RenderText(L"Options", TOP_MENU_FILE_SIZE + 10, 3, vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, AQUILINUS_SMALL_TEXT_SIZE, 1.0f);
#pragma endregion

#pragma region scroll bars
			// get the size of the scroll bars
			float fBarSizeX;
			fBarSizeX = ((float)vcWorkspaceZoomWindowSize.cx / (float)m_vcWorkspaceSize.cx) * vcWorkspaceWindowSize.cx;
			float fBarSizeY;
			fBarSizeY = ((float)vcWorkspaceZoomWindowSize.cy / (float)m_vcWorkspaceSize.cy) * vcWorkspaceWindowSize.cy;

			// get the position of the scroll bars
			float fBarPosX;
			fBarPosX = (m_fXScrollPos / (float)(m_vcWorkspaceSize.cx - vcWorkspaceZoomWindowSize.cx)) * ((float)vcWorkspaceWindowSize.cx - fBarSizeX);
			float fBarPosY;
			fBarPosY = (m_fYScrollPos / (float)(m_vcWorkspaceSize.cy - vcWorkspaceZoomWindowSize.cy)) * ((float)vcWorkspaceWindowSize.cy - fBarSizeY);

			// draw (scroll bars)
			RECT rcBar;
			SetRect(&rcBar,
				m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX,
				m_nWindowHeight - m_sWorkingAreaElementSizes.dwScrollBarBottom + 10,
				m_sWorkingAreaElementSizes.dwDataSheet + 4 + (LONG)fBarPosX + (LONG)fBarSizeX,
				m_nWindowHeight - 8);
			m_pDirectDraw->Clear(rcBar, AquilinusColor::Color3);
			SetRect(&rcBar,
				m_nWindowWidth - m_sWorkingAreaElementSizes.dwScrollBarRight + 10,
				m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY,
				m_nWindowWidth - 8,
				m_sWorkingAreaElementSizes.dwMenu + 4 + (LONG)fBarPosY + (LONG)fBarSizeY);
			m_pDirectDraw->Clear(rcBar, AquilinusColor::Color3);
#pragma endregion

			// present
			m_pDirectDraw->Present();
		}
		break;
#pragma endregion
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/**
* Working area window thread.
***/
DWORD WINAPI AQU_WorkingArea::s_WorkingAreaMsgThread(void* param)
{
	// register the window class
	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_DBLCLKS;
	m_wc.lpfnWndProc = s_WorkingAreaWndProc;
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = m_hInstance;
	m_wc.hIcon = NULL;
	m_wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	m_wc.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
	m_wc.lpszMenuName = NULL;
	m_wc.lpszClassName = L"Aquilinus DX Working Area";
	m_wc.hIconSm = NULL;

	// class registration failed
	if (!RegisterClassEx(&m_wc))
	{
		MessageBox(NULL, L"Aquilinus Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		UnregisterClass(L"Aquilinus Working Area", m_hInstance);
		FreeLibraryAndExitThread(m_hInstance, 0);
	}

	// create the window
	m_hwnd = CreateWindowEx(
		WS_EX_COMPOSITED,
		L"Aquilinus DX Working Area",
		L"Aquilinus Working Area",
		WS_POPUP,
		(m_nScreenWidth)-m_nWindowWidth,
		0,
		m_nWindowWidth,
		m_nWindowHeight,
		0,
		0,
		m_hInstance,
		0);

	// window creation failed
	if (m_hwnd == NULL)
	{
		MessageBox(NULL, L"Aquilinus Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		UnregisterClass(L"Aquilinus Working Area", m_hInstance);
		FreeLibraryAndExitThread(m_hInstance, 0);
	}

	// register hotkeys to send to workspace window
	if (RegisterHotKey(
		m_hwnd,
		1,
		MOD_NOREPEAT,
		VK_PRIOR))
	{
		OutputDebugString(L"Hotkey 'Page up' registered, using MOD_NOREPEAT flag");
	}
	
	if (RegisterHotKey(
		m_hwnd,
		2,
		MOD_NOREPEAT,
		VK_NEXT))
	{
		OutputDebugString(L"Hotkey 'Page down' registered, using MOD_NOREPEAT flag");
	}

	// create and init direct draw
	m_pDirectDraw = new AQU_Drawer(AQUILINUS_DRAWING_API);
#ifdef _WIN64
	if (FAILED(m_pDirectDraw->Init(m_hwnd, L"Aquilinus_x64.dll", m_nWindowWidth, m_nWindowHeight, true, m_pcTransferSite->m_pConfig->dwColorSchemeIndex, false, nullptr, nullptr, 0)))
#else
	if (FAILED(m_pDirectDraw->Init(m_hwnd, L"Aquilinus_Win32.dll", m_nWindowWidth, m_nWindowHeight, true, m_pcTransferSite->m_pConfig->dwColorSchemeIndex, false, nullptr, nullptr, 0)))
#endif
	{
		OutputDebugString(L"Failed to init direct draw !");
		return 0;
	}

	// show window
	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);

	// message loop
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	FreeLibraryAndExitThread(m_hInstance, 0);

	return (DWORD)msg.wParam;
}

/**
* Loads all workspace data from the file specified in the aquilinus configuration.
***/
HRESULT AQU_WorkingArea::s_LoadWorkSpace()
{
	// set d3d override to true
	m_pcTransferSite->m_bForceD3D = true;

	// get a data stream
	std::stringstream sstrDataStream;

	// load the stream
	HRESULT hr = m_pcTransferSite->m_pFileManager->LoadWorkingArea(m_pcTransferSite->m_pConfig->szWorkspaceFilePath, sstrDataStream);

	// and decode the stream
	if (SUCCEEDED(hr))
	{
		// first, read the game name to see wether this is an empty profile
		wchar_t szEntryName[MAX_JOLIET_FILENAME];
		sstrDataStream.read((char*)szEntryName, ENTRY_SIZE);

		// empty profile ?
		if (szEntryName[0] == 0)
			m_pcTransferSite->m_pConfig->bEmptyProcess = TRUE;
		else
			m_pcTransferSite->m_pConfig->bEmptyProcess = FALSE;

		// now, ignore the size of the additional option data block
		sstrDataStream.ignore(sizeof(DWORD));

		// ignore picture boolean and the path if true
		BOOL bPicture;
		sstrDataStream.read((char*)&bPicture, sizeof(BOOL));
		if (bPicture)
			sstrDataStream.ignore(MAX_PATH * sizeof(wchar_t));

		// ignore the detour time delay
		sstrDataStream.ignore(sizeof(DWORD));

		// now, ignore the injection techniques
		DWORD dwSupportedInterfacesNumber;
		sstrDataStream.read((char*)&dwSupportedInterfacesNumber, sizeof(DWORD));
		sstrDataStream.ignore(dwSupportedInterfacesNumber*sizeof(int));

		// read the number of nodes, resize node vector
		UINT dwNodeNumber;
		sstrDataStream.read((char*)&dwNodeNumber, sizeof(UINT));
		m_paNodes.resize(dwNodeNumber);

		// get a node provider
		AQU_NodeProvider* pProvider = new AQU_NodeProvider();

		// loop through the nodes to add node data 
		for (UINT i = 0; i != dwNodeNumber; i++)
		{
			OutputDebugString(L"New Node!");

			// get the node hash
			UINT id;
			sstrDataStream.read((char*)&id, sizeof(UINT));

			// get the node position
			POINT pos;
			sstrDataStream.read((char*)&pos.x, sizeof(LONG));
			sstrDataStream.read((char*)&pos.y, sizeof(LONG));

			// load plugin info (if plugin node)
			UINT idPlugin = 0;
			wchar_t szFileName[64];
			wchar_t szFilePath[MAX_PATH];
			if (id == ELEMENTARY_NODE_PLUGIN)
			{
				// and write to stream
				sstrDataStream.read((char*)&idPlugin, sizeof(UINT));
				sstrDataStream.read((char*)&szFileName[0], sizeof(wchar_t) * 64);

				OutputDebugString(szFileName);

				// create full path
				wsprintf(szFilePath, L"%s%s", m_pcTransferSite->m_pFileManager->GetPluginPath(), szFileName);

				OutputDebugString(szFilePath);
			}

			// read node data
			UINT dwDataSize = 0;
			sstrDataStream.read((char*)&dwDataSize, sizeof(UINT));

			// read the data
			char* pcData = new char[dwDataSize];
			if (dwDataSize)
				sstrDataStream.read((char*)pcData, dwDataSize);

			// get a node pointer
			NOD_Basic* pNode;
			if (FAILED(pProvider->Get_Node(pNode, id, pos.x, pos.y, idPlugin, szFilePath)))
			{
				// unregister all nodes
				m_pcTransferSite->UnregisterAllNodes();

				// debug output
				wchar_t buf[64];
				wsprintf(buf, L"Aquilinus : Unknown node type %u", id);
				OutputDebugString(buf);

				for (UINT j = 0; j < i; j++)
					delete m_paNodes[j];

				m_paNodes.clear();
				delete pProvider;
				sstrDataStream.clear();
				return E_FAIL;
			}

			// init node data
			pNode->InitNodeData(pcData, dwDataSize);

			// register this node
			m_pcTransferSite->RegisterD3DNode(pNode, id);

			// and add the node
			m_paNodes[i] = pNode;
			delete pcData;
		}

		delete pProvider;

		// loop through the nodes to add node connections
		for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
		{
			// get the number of commanders
			DWORD dwCommandersNumber;
			sstrDataStream.read((char*)&dwCommandersNumber, sizeof(DWORD));

			// loop through commanders, get the number of connections and the connection indices
			for (DWORD j = 0; j < dwCommandersNumber; j++)
			{
				// get the commander connections number
				DWORD dwConnectionsNumber;
				sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(DWORD));

				// loop through decommanders, set indices
				for (DWORD k = 0; k < dwConnectionsNumber; k++)
				{
					// get decommander node index
					LONG lNodeIndex;
					sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));
					// get decommander index
					LONG lDecommanderIndex;
					sstrDataStream.read((char*)&lDecommanderIndex, sizeof(LONG));

					// output debug data
					OutputDebugString(L"Connect Decommander");
					wchar_t buf[64]; wsprintf(buf, L"nodes : %u %u cix : %u dix : %u", i, lNodeIndex, j, lDecommanderIndex); OutputDebugString(buf);

					// and connect
					m_paNodes[i]->ConnectDecommander(m_paNodes[lNodeIndex], lNodeIndex, j, lDecommanderIndex);
				}
			}

			// get the provoker connections number
			DWORD dwConnectionsNumber;
			sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(DWORD));

			// loop through decommanders, return indices
			for (int j = 0; j < (int)dwConnectionsNumber; j++)
			{
				LONG lNodeIndex;
				sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));

				// output debug data
				OutputDebugString(L"Connect Invoker");
				wchar_t buf[64]; wsprintf(buf, L"nodes : %u %u", i, lNodeIndex); OutputDebugString(buf);

				// and connect
				m_paNodes[i]->ConnectInvoker(m_paNodes[lNodeIndex], lNodeIndex);
			}
		}

		// loop through the nodes again to call the connect commander method 
		for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
			m_paNodes[i]->ConnectCommander(nullptr, 0);
	}

	// set d3d override to false
	m_pcTransferSite->m_bForceD3D = false;

	return hr;
}

/**
* Enumerates all Node-plugins.
***/
HRESULT AQU_WorkingArea::s_EnumeratePlugins(LPCWSTR szDllPath)
{
	WIN32_FIND_DATA ffd;
	wchar_t szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// clear enumeration vectors
	m_vcPluginNames.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();
	m_vcPlugins.clear();
	m_vcPluginHandles.clear();

	// create plugins path
	wsprintf(szDir, L"%s%s", szDllPath, L"*");
#ifdef _DEBUG
	OutputDebugString(szDir);
#endif

	// find first file
	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
		return E_FAIL;

	// loop through files
	do
	{
		// get the extension
		const wchar_t *dot = wcsrchr(ffd.cFileName, '.');
		if (!dot || dot == ffd.cFileName) dot = L"";

		// ".dll" ?? 
		if (wcscmp(dot, L".dll") == NULL)
		{
			wchar_t szDllDir[MAX_PATH];

			OutputDebugString(L"Dll found :");
			OutputDebugString(ffd.cFileName);

			// create plugin path and load module
			wsprintf(szDllDir, L"%s%s", szDllPath, ffd.cFileName);

			// convert to LPCSTR
			int size = (int)wcslen(szDllDir);
			size += 2;
			char *szDll = (char *)malloc(size);
			wcstombs_s(NULL, szDll, size,
				szDllDir, size);

			HMODULE hm = LoadLibraryA(szDll);

			if (hm != NULL)
			{
				// define plugin creation type
				typedef AQU_Nodus* (*AQU_Nodus_Create)();
				AQU_Nodus_Create m_pAQU_Nodus_Create;

				// get plugin creation method
				m_pAQU_Nodus_Create = (AQU_Nodus_Create)GetProcAddress(hm, "AQU_Nodus_Create");

				if (m_pAQU_Nodus_Create)
				{
					// create a test nodus class to test for directx version support
					AQU_Nodus* pNodus = m_pAQU_Nodus_Create();

					// get the plugin name
					LPSTR szName = new char[64];
					const char* szSrcName = pNodus->GetNodeType();
					UINT dwLen = (UINT)strlen(szSrcName);
					if (dwLen > 63) dwLen = 63;
					CopyMemory((void*)szName, (void*)szSrcName, dwLen + 1);

					// get the path name
					LPWSTR szPathName = new wchar_t[MAX_PATH];
					dwLen = (UINT)wcslen(szDllDir);
					if (dwLen > MAX_PATH - 1) dwLen = MAX_PATH - 1;
					CopyMemory((void*)szPathName, (void*)szDllDir, (dwLen + 1)*sizeof(wchar_t));

					// get the category name
					LPWSTR szCategoryName = new wchar_t[64];
					LPWSTR szSrcNameW = pNodus->GetCategory();
					dwLen = (UINT)wcslen(szSrcNameW);
					if (dwLen > 64 - 1) dwLen = 64 - 1;
					CopyMemory((void*)szCategoryName, (void*)szSrcNameW, (dwLen + 1)*sizeof(wchar_t));

					// enumerate all necessary data
					m_vcPluginNames.push_back(szName);
					m_vcPluginFilePathes.push_back(szPathName);
					m_vcPluginIDs.push_back(pNodus->GetNodeTypeId());
					m_vcPluginCategories.push_back(szCategoryName);

					// keep the prototypes to delete them at class destruction
					m_vcPlugins.push_back(pNodus);
				}

				m_vcPluginHandles.push_back(hm);
			}
			else MessageBox(NULL, ffd.cFileName, L"Failed to load !", MB_OK);

		}

	}
	while (FindNextFile(hFind, &ffd) != 0);

	// output debug data
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		OutputDebugStringA(m_vcPluginNames[i]);
		OutputDebugStringW(m_vcPluginFilePathes[i]);
		OutputDebugStringW(m_vcPluginCategories[i]);
	}

	return S_OK;
}

/**
* Get the node for the current mouse position.
* @param nX X position of the mouse cursor.
* @param nY Y position of the mouse cursor.
* @returns The node index for the cursor position, -1 if there is no node there.
***/
int AQU_WorkingArea::s_GetNodeIndex(POINT vecCursor, POINT vecOrigin, float fZoom)
{
	for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
	{
		if (m_paNodes[i]->IsLocal(vecCursor, vecOrigin, fZoom)) return (int)i;
	}

	return -1;
}

/**
* Updates if there is an active node.
***/
void AQU_WorkingArea::s_UpdateActiveNode(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// get the new behavior of the chosen node
	m_eActiveNodeBehavior = m_paNodes[m_nActiveNodeIndex]->WindowsEvent(msg, wParam, lParam);

	// verify all connections
	m_paNodes[m_nActiveNodeIndex]->VerifyConnections(&m_paNodes);

	// set status to >active node< if node is active, only if node is active
	if (m_eActiveNodeBehavior != AQU_NodeBehavior::Inactive)
		m_eWorkingAreaStatus = AQU_WorkingAreaStatus::NodeActive;
	else
	{
		m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;
		m_nActiveNodeIndex = -1;
	}
}
#pragma endregion