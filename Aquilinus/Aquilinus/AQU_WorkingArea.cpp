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
#include"AQU_2DData.h"
#include "..\dependecies\imgui\imgui_helpers.h"


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
WNDCLASSEX                                    AQU_WorkingArea::m_wc;
HWND                                          AQU_WorkingArea::m_hwnd;
HINSTANCE                                     AQU_WorkingArea::m_hInstance;
AQU_WorkingArea::WindowControl                AQU_WorkingArea::m_sWindowControl;
int                                           AQU_WorkingArea::m_nWindowWidth;
int                                           AQU_WorkingArea::m_nWindowHeight;
POINT                                         AQU_WorkingArea::m_ptMouseCursor;
std::vector<NOD_Basic*>                       AQU_WorkingArea::m_paNodes;
AQU_WorkingAreaStatus                         AQU_WorkingArea::m_eWorkingAreaStatus;
AQU_TransferSite* AQU_WorkingArea::m_pcTransferSite;
std::vector<LPSTR>                            AQU_WorkingArea::m_vcPluginNames;
std::vector<LPWSTR>                           AQU_WorkingArea::m_vcPluginCategories;
std::vector<LPWSTR>                           AQU_WorkingArea::m_vcPluginFilePathes;
std::vector<UINT>                             AQU_WorkingArea::m_vcPluginIDs;
std::vector<AQU_Nodus*>                       AQU_WorkingArea::m_vcPlugins;
std::vector<HMODULE>                          AQU_WorkingArea::m_vcPluginHandles;
int                                           AQU_WorkingArea::m_nDataSheetCategorySelection;
int                                           AQU_WorkingArea::m_nDataSheetEntrySelection;
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
	// set window size
	m_nWindowWidth = GetSystemMetrics(SM_CXFULLSCREEN) >> 1;
	m_nWindowHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	m_sWindowControl = {};

	// set module handle and config
	m_hInstance = hInstance;
	m_pcTransferSite = pcTransferSite;
	m_pcTransferSite->m_bIsWorkingArea = true;
	m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;

	// data sheet selection
	m_nDataSheetEntrySelection = -1;
	m_nDataSheetCategorySelection = -1;

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
						CopyMemory((void*)&pEntry->m_szTitle[0], (void*)&pNode->m_szTitle[lenClass], len * sizeof(wchar_t));
					}

					// create sub entries
					pEntry->m_dwSubEntriesNumber = (DWORD)pNode->m_paCommanders.size();
					pEntry->m_paSubEntries = new LPCWSTR[pEntry->m_dwSubEntriesNumber];
					for (int k = 0; k < (int)pEntry->m_dwSubEntriesNumber; k++)
					{
						len = (int)wcslen(pNode->m_paCommanders[k]->m_szTitle) + 1;
						pEntry->m_paSubEntries[k] = new wchar_t[len];
						CopyMemory((void*)&pEntry->m_paSubEntries[k][0], (void*)&pNode->m_paCommanders[k]->m_szTitle[0], len * sizeof(wchar_t));
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

					CopyMemory((void*)&pEntry->m_szTitle[0], (void*)sW.c_str(), len * sizeof(wchar_t));
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

				CopyMemory((void*)&pEntry->m_szTitle[0], (void*)sW.c_str(), len * sizeof(wchar_t));
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
* Working area window thread.
***/
DWORD WINAPI AQU_WorkingArea::s_WorkingAreaMsgThread(void* param)
{
	// glfw: initialize and configure
	if (!glfwInit()) { OutputDebugString(L"Inicio: GLFW Init fail !"); return 1; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	glfwWindowHint(GLFW_DECORATED, 0);
	GLFWwindow* window = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, "Aquilinus", NULL, NULL);
	if (window == NULL)
	{
		OutputDebugString(L"Inicio: GLFW window creation failed !");
		glfwTerminate();
		return -1;
	}

	// set size, position, callback methods, enable vsync
	glfwSetWindowSizeLimits(window, m_nWindowWidth, m_nWindowHeight, m_nWindowWidth, m_nWindowHeight);
	glfwSetWindowPos(window, m_nWindowWidth, 0);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, s_Viewport_callback);
	glfwSetCursorPosCallback(window, s_Cursor_position_callback);
	glfwSetMouseButtonCallback(window, s_Mouse_button_callback);

	// and init glew
	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		OutputDebugString(L"Inicio: Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context, font
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig sConfig = {};
	sConfig.SizePixels = 16;
	ImFont* psFontSmall = io.Fonts->AddFontDefault(&sConfig);
	sConfig.SizePixels = 22;
	ImFont* psFontMedium = io.Fonts->AddFontDefault(&sConfig);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(nullptr);

	OutputDebugString(L"Aquilinus : Injectd window creation succeeded !");

	// set color scheme
	ImGui::ColorSchemeHex sScheme = {};
	CopyMemory(&sScheme.uColor[0], &g_colorSchemes[m_pcTransferSite->m_pConfig->dwColorSchemeIndex].uColor[0], sizeof(UINT[5]));
	ImGui::StyleColorsByScheme(sScheme);
	ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events, update window fields
		glfwPollEvents();
		glfwGetWindowPos(window, &m_sWindowControl.nW_posx, &m_sWindowControl.nW_posy);
		glfwSetWindowPos(window, m_sWindowControl.nW_posx + m_sWindowControl.nOffset_cpx, m_sWindowControl.nW_posy + m_sWindowControl.nOffset_cpy);
		m_sWindowControl.nControlEvent = 0;
		m_sWindowControl.nOffset_cpx = 0;
		m_sWindowControl.nOffset_cpy = 0;
		m_sWindowControl.nCp_x += m_sWindowControl.nOffset_cpx;
		m_sWindowControl.nCp_y += m_sWindowControl.nOffset_cpy;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// main window set flags and zero pos
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2((float)m_nWindowWidth, (float)m_nWindowHeight), ImGuiCond_FirstUseEver);

		// main window
		if (ImGui::Begin("Aquilinus", nullptr, window_flags))
		{
			// get small font, create main menu bar
			ImGui::PushFont(psFontSmall);
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Load", "CTRL+L")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			// get start y position, output description and a separator line... menu bar height ~ 1.2xFontSize
			float fStartY = ImGui::GetFontSize();
			ImVec2 sStart_rect_max = { 0.0f, fStartY * 1.2f };
			ImVec2 sStart_rect_text = { fStartY * .5f, fStartY * 2.5f };
			ImGui::SetCursorPos(sStart_rect_text);
			ImGui::Text("Aquilinus : Vireio Perception 3D Modification Studio");
			ImGui::Separator();

			// output injected interfaces info
			for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
			{
				if (m_pcTransferSite->m_pConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
				{
					// create text buffers
					char buffer[64];
					wsprintfA(buffer, "%d", m_pcTransferSite->m_anInterfaceRefCount[i]);
					std::wstring acBuffW(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].szName, 0, 64);
					std::string acBuffA;
					for (wchar_t c : acBuffW) acBuffA += (char)c;

					// output interface ref count
					ImGui::Text(buffer);

					// adjust spacing and output interface name
					// ImVec2 sSize = ImGui::GetItemRectSize();
					ImGui::SameLine(50.0f); ImGui::Text(acBuffA.c_str());
				}
			}

			// get last y position
			ImVec2 sChild_rect_max = ImGui::GetItemRectMax();
			sChild_rect_max.x = 0.0f;
			sChild_rect_max.y += ImGui::GetFontSize();
			static bool s_bInit = false;

			// loop through categories
			for (std::vector<AQU_DataSheetEntry*>::size_type h = 0; h != m_pcTransferSite->m_paDataSheetCategories.size(); h++)
			{
				if (!s_bInit)
				{
					// set next window start position
					ImGui::SetNextWindowPos(sChild_rect_max);
					ImVec2 sSize = { 240.0f, 100.0f };
					ImGui::SetNextWindowSize(sSize);
					sChild_rect_max.y += sSize.y;
				}

				// new category window
				std::string acTitleA;
				std::wstring acTitleW(m_pcTransferSite->m_paDataSheetCategories[h]->m_szTitle);
				for (wchar_t c : acTitleW) acTitleA += (char)c;
				if (ImGui::Begin(acTitleA.c_str()))
				{
					// loop through entries
					for (std::vector<AQU_DataSheetEntry*>::size_type i = 0; i != m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries.size(); i++)
					{
						// entry as selectable, first convert to std::string()
						std::wstring acSelectableW(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_szTitle);
						std::string acSelectableA;
						for (wchar_t c : acSelectableW) acSelectableA += (char)c;
						if (ImGui::Selectable(acSelectableA.c_str(), (m_nDataSheetEntrySelection == (int)i) && (m_nDataSheetCategorySelection == (int)h)))
						{
							m_nDataSheetEntrySelection = (int)i;
							m_nDataSheetCategorySelection = (int)h;
						}

						// sub entries ?
						if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber > 0)
						{
							// output sub entries if hovered
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								// output sub-entries
								for (int j = 0; j < (int)m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_dwSubEntriesNumber; j++)
								{
									std::wstring acSubW(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_paSubEntries[j]);
									std::string acSubA;
									for (wchar_t c : acSubW) acSubA += (char)c;
									ImGui::Text(acSubA.c_str());
								}
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
						}
					}
				}

				// adjust window minimum y position
				ImVec2 sPos = ImGui::GetWindowPos();
				if (sPos.y < sStart_rect_max.y) { sPos.y = sStart_rect_max.y; ImGui::SetWindowPos(sPos); }

				ImGui::End();

			}

			s_bInit = true;
			ImGui::PopFont();
		}
		else
		{
			OutputDebugString(L"Aquilinus: ImGui failed to create window !");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		ImGui::End();



		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup ImGui + GL
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	// cleanup nodes vector
	for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
	{
		delete m_paNodes[i];
	}
	m_paNodes.clear();

	return (DWORD)0;
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
		sstrDataStream.ignore(dwSupportedInterfacesNumber * sizeof(int));

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
		const wchar_t* dot = wcsrchr(ffd.cFileName, '.');
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
			char* szDll = (char*)malloc(size);
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
					CopyMemory((void*)szPathName, (void*)szDllDir, (dwLen + 1) * sizeof(wchar_t));

					// get the category name
					LPWSTR szCategoryName = new wchar_t[64];
					LPWSTR szSrcNameW = pNodus->GetCategory();
					dwLen = (UINT)wcslen(szSrcNameW);
					if (dwLen > 64 - 1) dwLen = 64 - 1;
					CopyMemory((void*)szCategoryName, (void*)szSrcNameW, (dwLen + 1) * sizeof(wchar_t));

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

	} while (FindNextFile(hFind, &ffd) != 0);

	// output debug data
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		OutputDebugStringA(m_vcPluginNames[i]);
		OutputDebugStringW(m_vcPluginFilePathes[i]);
		OutputDebugStringW(m_vcPluginCategories[i]);
	}

	return S_OK;
}
#pragma endregion