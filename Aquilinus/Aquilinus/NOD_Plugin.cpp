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
#include"NOD_Plugin.h"
#include "..\..\Perception\dependecies\imgui\imgui_helpers.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_INT(a) { wchar_t buf[128]; wsprintf(buf, L"%d", a); OutputDebugString(buf); }
#define DEBUG_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }

/// <summary>
/// Constructor.
/// </summary>
NOD_Plugin::NOD_Plugin(LONG nX, LONG nY, std::wstring szFilePath) :NOD_Basic(nX, nY, 140, 100),
m_dwUpdateCounter(0)
{
	// load library
	m_hm = LoadLibraryW(szFilePath.c_str());

	if (m_hm != NULL)
	{
		typedef AQU_Nodus* (*AQU_Nodus_Create)(ImGuiContext* sCtx);
		AQU_Nodus_Create m_pAQU_Nodus_Create;

		// get plugin creation method
		m_pAQU_Nodus_Create = (AQU_Nodus_Create)GetProcAddress(m_hm, "AQU_Nodus_Create");

		// create the nodus class... first ensure that imgui context is actually created
		if (!ImGui::GetCurrentContext())
			ImGui::CreateContext();
		m_pNodus = m_pAQU_Nodus_Create(ImGui::GetCurrentContext());

		// set fields by plugin data
		m_eNodeProvokingType = (AQU_NodeProvokingType)m_pNodus->GetProvokingType();

		// plugin replaces command ?
		m_bReturn = m_pNodus->GetMethodReplacement();

		// set size
		m_sSize = m_pNodus->GetNodeSize();

		// set name
		std::string strA(m_pNodus->GetNodeType());
		std::wstring strW = std::wstring(strA.begin(), strA.end());
		m_szTitle = new wchar_t[strW.size() + 1];
		CopyMemory((void*)&m_szTitle[0], (void*)strW.c_str(), (strW.size() + 1) * sizeof(wchar_t));

		// set file name
		std::wstring strPath(szFilePath);
		std::wstring strBase = strPath.substr(strPath.find_last_of(L"/\\") + 1);
		if (strBase.length() > 63)
		{
			// this would fail eventually... file name too long
			CopyMemory((void*)&m_szFileName[0], (void*)strBase.c_str(), 63 * sizeof(wchar_t));
			m_szFileName[63] = 0;

			OutputDebugString(L"Aquilinus : Plugin filename TOO LONG ! Game Profile won't save !");
		}
		else
		{
			CopyMemory((void*)&m_szFileName[0], (void*)strBase.c_str(), strBase.length() * sizeof(wchar_t));
			m_szFileName[strBase.length()] = 0;
		}

		// get logo bitmap, control bitmap is always null at start
		m_hBitmapLogo = m_pNodus->GetLogo();
		m_dwLogoID = NULL;
		m_hBitmapControl = nullptr;
		m_dwControlID = NULL;
		ZeroMemory(&m_rcControl, sizeof(RECT));

		// get commanders
		for (DWORD i = 0; i < m_pNodus->GetCommandersNumber(); i++)
		{
			NOD_Commander* pC = new NOD_Commander();
			pC->m_ePlugtype = (int)m_pNodus->GetCommanderType(i);
			pC->m_szTitle = m_pNodus->GetCommanderName(i);
			pC->m_pOutput = m_pNodus->GetOutputPointer(i);
			pC->m_paDecommanders.clear();
			m_paCommanders.push_back(pC);
		}

		// get decommanders
		for (DWORD i = 0; i < m_pNodus->GetDecommandersNumber(); i++)
		{
			NOD_Decommander* pD = new NOD_Decommander();
			pD->m_ePlugtype = (int)m_pNodus->GetDecommanderType(i);
			pD->m_szTitle = m_pNodus->GetDecommanderName(i);
			pD->m_pInput = nullptr;
			pD->m_paCommanders.clear();
			pD->m_lDecommanderIndex = (LONG)i;
			m_paDecommanders.push_back(pD);
		}

		OutputDebugString(L"Node created");
		OutputDebugStringA(m_pNodus->GetNodeType());
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to load plugin dll -");
		OutputDebugString(szFilePath.c_str());
	}

	// set slot space to zero, so the plugin itself creates the actual node size (x)
	m_fSlotSpace = 0.f;
}

/// <summary>
/// Destructor.
/// </summary>
NOD_Plugin::~NOD_Plugin()
{
	FreeLibrary(m_hm);
	m_hm = NULL;
}

/// <summary>
/// Draws the node.
/// The plugin node has additional drawing options like a logo or a control.
/// </summary>
HRESULT NOD_Plugin::Update(float fZoom)
{
	// draw the logo
	if (m_hBitmapLogo)
	{
		// create GL texture
		if (!m_dwLogoID)
			ImGui::CreateTextureFromBitmap(m_hBitmapLogo, &m_dwLogoID, &m_sImageSize.nW, &m_sImageSize.nH);

		// draw image....reverse V
		if (m_dwLogoID)
		{
			const float fLocalZoom = 0.5f;
			ImGui::Image((void*)(intptr_t)m_dwLogoID, ImVec2((float)m_sImageSize.nW * fZoom * fLocalZoom, (float)m_sImageSize.nH * fZoom * fLocalZoom), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		}

	}

	return S_OK;
}

/// <summary>
/// Returns nodus dll class method.
/// </summary>
bool NOD_Plugin::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	return m_pNodus->SupportsD3DMethod(eD3D, eD3DInterface, eD3DMethod);
}

/// <summary>
/// Calls super method and sets all plugin input pointers new.
/// </summary>
void NOD_Plugin::ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex)
{
	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		m_pNodus->SetInputPointer((DWORD)i, m_paDecommanders[i]->m_pInput);

	// call super method
	NOD_Basic::ConnectInvoker(pNode, nDestNodeIndex);
}

/// <summary>
/// Node Provoke - Draw Text.
/// </summary>
void* NOD_Plugin::Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
{
	// update plugin replace command
	m_bReturn = m_pNodus->GetMethodReplacement();

	// Provoke
	int nIndex = 0;

	m_pvReturn = m_pNodus->Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, 0, nIndex);

	// next cycle behavior ?
	if (nIndex & int(AQU_PluginFlags::DoubleCallFlag))
		m_eNextNodeCall = AQU_NextNodeCall::DoubleCall;

	// immediate return with method replacement ?
	if (nIndex & int(AQU_PluginFlags::ImmediateReturnFlag))
	{
		m_bReturn = true;
		return m_pvReturn;
	}

	return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
}

/// <summary>
/// Get the plugin nodes save data block size.
/// </summary>
DWORD NOD_Plugin::GetSaveDataSize()
{
	return m_pNodus->GetSaveDataSize();
}

/// <summary>
/// Get the plugin nodes save data block.
/// </summary>
char* NOD_Plugin::GetSaveData(UINT* pdwSizeOfData)
{
	return m_pNodus->GetSaveData(pdwSizeOfData);
}

/// <summary>
/// Init the plugin nodes load data block.
/// </summary>
void NOD_Plugin::InitNodeData(char* pData, UINT dwSizeOfData)
{
	m_pNodus->InitNodeData(pData, dwSizeOfData);
}

/// <summary>
/// Verifies the connections of the plugin node.
/// </summary>
void NOD_Plugin::VerifyConnections(std::vector<NOD_Basic*>* ppaNodes)
{
	// node to be verified ?
	if (m_nNodeIndexToVerify >= 0)
	{
		if (m_nNodeIndexToVerify < (int)(*ppaNodes).size())
			(*ppaNodes)[m_nNodeIndexToVerify]->VerifyConnections(ppaNodes);
		m_nNodeIndexToVerify = -1;
	}

	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
	{
		// verify pointers
		if (m_paDecommanders[i]->m_paCommanders.size() == 0)
			m_paDecommanders[i]->m_pInput = nullptr;
		else
			m_paDecommanders[i]->m_pInput = m_paDecommanders[i]->m_paCommanders.back()->m_pOutput;

		// set plugin input pointer new
		m_pNodus->SetInputPointer((DWORD)i, m_paDecommanders[i]->m_pInput);
	}
}

/// <summary>
/// Calls the plugin method to update the controls.
/// </summary>
/// <param name="fZoom">Current zoom factor</param>
void NOD_Plugin::UpdatePluginControl(float fZoom)
{
	m_pNodus->UpdateImGuiControl(fZoom);
}




