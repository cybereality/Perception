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

/**
* Constructor.
***/
NOD_Plugin::NOD_Plugin(LONG nX, LONG nY, LPWSTR szFilePath) :NOD_Basic(nX, nY, 140, 100),
//m_pcDrawer(nullptr),
m_dwUpdateCounter(0)
{
	// convert to LPCSTR
	int size = (int)wcslen(szFilePath);
	size += 2;
	char *szDll = (char *)malloc(size);
	wcstombs_s(NULL, szDll, size,
		szFilePath, size);

	m_hm = LoadLibraryA(szDll);

	if (m_hm != NULL)
	{
		typedef AQU_Nodus* (*AQU_Nodus_Create)();
		AQU_Nodus_Create m_pAQU_Nodus_Create;

		// get plugin creation method
		m_pAQU_Nodus_Create = (AQU_Nodus_Create)GetProcAddress(m_hm, "AQU_Nodus_Create");

		// create the nodus class
		m_pNodus = m_pAQU_Nodus_Create();

		// set fields by plugin data
		m_eNodeProvokingType = (AQU_NodeProvokingType)m_pNodus->GetProvokingType();

		// plugin replaces command ?
		m_bReturn = m_pNodus->GetMethodReplacement();

		// set size
		m_sSize.x = (float)m_pNodus->GetNodeWidth();
		m_sSize.y = (float)m_pNodus->GetNodeHeight();

		// set name
		std::string strA(m_pNodus->GetNodeType());
		std::wstring strW = std::wstring(strA.begin(), strA.end());
		m_szTitle = new wchar_t[strW.size() + 1];
		CopyMemory((void*)&m_szTitle[0], (void*)strW.c_str(), (strW.size() + 1)*sizeof(wchar_t));

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
			CopyMemory((void*)&m_szFileName[0], (void*)strBase.c_str(), strBase.length()*sizeof(wchar_t));
			m_szFileName[strBase.length()] = 0;
		}

		// get logo bitmap, control bitmap is always null at start
		m_hBitmapLogo = m_pNodus->GetLogo();
		m_hBitmapControl = nullptr;
		m_dwLogoID = NULL;
		m_dwControlID = NULL;
		ZeroMemory(&m_rcControl, sizeof(RECT));

		// get commanders
		for (DWORD i = 0; i < m_pNodus->GetCommandersNumber(); i++)
		{
			NOD_Commander* pC = new NOD_Commander();
			pC->m_ePlugtype = (NOD_Plugtype)m_pNodus->GetCommanderType(i);
			pC->m_szTitle = m_pNodus->GetCommanderName(i);
			pC->m_pOutput = m_pNodus->GetOutputPointer(i);
			pC->m_paDecommanders.clear();
			m_paCommanders.push_back(pC);
		}

		// get decommanders
		for (DWORD i = 0; i < m_pNodus->GetDecommandersNumber(); i++)
		{
			NOD_Decommander* pD = new NOD_Decommander();
			pD->m_ePlugtype = (NOD_Plugtype)m_pNodus->GetDecommanderType(i);
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
		OutputDebugString(szFilePath);
	}

	// for plugins, the header size is constant 64 + 4 (border)
	m_dwHeaderSize = 64 + 4;
}

/**
* Destructor.
***/
NOD_Plugin::~NOD_Plugin()
{
	/*if (m_pcDrawer)
	{
		m_pcDrawer->ReleaseBitmap(m_dwControlID);
		m_pcDrawer->ReleaseBitmap(m_dwLogoID);
	}*/
	FreeLibrary(m_hm);
	m_hm = NULL;
}

/**
* Draws the node.
* The plugin node has additional drawing options like a logo or a control.
* @param pcDrawer The aquilinus drawing interface.
* @param vcOrigin The origin vector for the drawing call, in pixel space.
***/
HRESULT NOD_Plugin::Draw(POINT vcOrigin)
{
	/*DWORD dwWidth;

	// backup the drawer
	if (!m_pcDrawer) m_pcDrawer = pcDrawer;

	// clear node background
	pcDrawer->DrawHLine(m_vecPos, vcOrigin, m_vecSize.cx, m_vecSize.cy, AquilinusColor::Color1, fZoom);

	// draw the border
	POINT pt = m_vecPos;
	pcDrawer->DrawHLine(pt, vcOrigin, m_vecSize.cx, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pt.y += m_vecSize.cy - 4;
	pcDrawer->DrawHLine(pt, vcOrigin, m_vecSize.cx, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pcDrawer->DrawVLine(pt, vcOrigin, m_vecSize.cy, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pt.x += m_vecSize.cx - 4;
	pcDrawer->DrawVLine(pt, vcOrigin, m_vecSize.cy, 4, AquilinusColor::Color3, fZoom);

	// draw the text or the logo
	if (m_hBitmapLogo)
	{
		if (!m_dwLogoID)
			m_dwLogoID = pcDrawer->RegisterBitmap(m_hBitmapLogo);

		if (m_dwLogoID)
		{
			// get size of bitmap
			BITMAP bm;
			GetObject(m_hBitmapLogo, sizeof(bm), &bm);

			// clamp size
			if (m_vecSize.cx < ((bm.bmWidth >> 2) + 8)) m_vecSize.cx = (bm.bmWidth >> 2) + 8;
			if (m_vecSize.cy < ((bm.bmHeight >> 2) + 32)) m_vecSize.cy = (bm.bmHeight >> 2) + 32;

			// draw bitmap
			RECT rcDest;
			pt = m_vecPos;
			pt.y += 4;
			pt.x += 4;
			SetRect(&rcDest, (int)pt.x, (int)pt.y, (int)pt.x + (bm.bmWidth >> 2), (int)pt.y + (bm.bmHeight >> 2));
			pcDrawer->DrawBitmap(m_dwLogoID, rcDest, vcOrigin, fZoom);
			pt.y += (bm.bmHeight >> 2) + 8;
		}
	}
	else
	{
		// get text width and render text
		dwWidth = pcDrawer->GetTextWidth(m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		pt = m_vecPos;
		pt.y += 16;
		pt.x += (m_vecSize.cx >> 1) - ((LONG)dwWidth >> 1);
		pcDrawer->RenderText(m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		pt.y += 24;

		// clamp node size
		if (m_vecSize.cx < (LONG)(dwWidth + 8)) m_vecSize.cx = dwWidth + 8;
	}

	// any bitmap updates for the control ?
	m_hBitmapControl = m_pNodus->GetControl();
	if (m_hBitmapControl)
	{
		if (!m_dwControlID)
			m_dwControlID = pcDrawer->RegisterBitmap(m_hBitmapControl);
		else
		{
			
			pcDrawer->UpdateBitmap(m_hBitmapControl, m_dwControlID);
			
		}

		// get size of bitmap
		BITMAP bm;
		GetObject(m_hBitmapControl, sizeof(bm), &bm);

		// clamp size
		if (m_vecSize.cx < ((bm.bmWidth >> 2) + 8)) m_vecSize.cx = (bm.bmWidth >> 2) + 8;
		if (m_vecSize.cy < ((bm.bmHeight >> 2) + pt.y - 4 - m_vecPos.y)) m_vecSize.cy = (bm.bmHeight >> 2) + pt.y - 4 - m_vecPos.y;

		// set image rectangle
		SetRect(&m_rcControl, 0, 0, (int)bm.bmWidth, (int)bm.bmHeight);
	}

	if (m_dwControlID)
	{
		// render the surface
		pt.y -= 8;
		RECT rcDest;
		SetRect(&rcDest, (int)pt.x, (int)pt.y, (int)pt.x + (m_rcControl.right >> 2), (int)pt.y + (m_rcControl.bottom >> 2));

		pcDrawer->DrawBitmap(m_dwControlID, rcDest, vcOrigin, fZoom);
		pt.y += 8;
	}

	// draw the decommanders
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
	{
		// draw the joint
		pt.x = m_vecPos.x;
		pcDrawer->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color4, fZoom);

		// set the decommander y pos
		m_paDecommanders[i]->m_lNodeYPos = pt.y - m_vecPos.y;

		// draw text only if no control present
		if (!m_dwControlID)
		{
			// get text width, set x pos
			dwWidth = pcDrawer->GetTextWidth(m_paDecommanders[i]->m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
			pt.y -= 2;
			pt.x += 12;

			// verify node width
			if (m_vecSize.cx < ((LONG)dwWidth + 24)) m_vecSize.cx = (LONG)dwWidth + 24;

			pcDrawer->RenderText(m_paDecommanders[i]->m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color3, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		}

		pt.y += 16;
	}

	// draw the commanders
	for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
	{
		// draw the joint
		pt.x = m_vecPos.x + m_vecSize.cx - 8;
		pcDrawer->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color4, fZoom);

		// set the decommander y pos
		m_paCommanders[i]->m_lNodeYPos = pt.y - m_vecPos.y;

		// draw text only if no control present
		if (!m_dwControlID)
		{
			// get text width, set x pos
			dwWidth = pcDrawer->GetTextWidth(m_paCommanders[i]->m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
			pt.y -= 2;
			pt.x = m_vecPos.x + m_vecSize.cx - (LONG)dwWidth - 16;

			// verify node width
			if (m_vecSize.cx < ((LONG)dwWidth + 24)) m_vecSize.cx = (LONG)dwWidth + 24;

			pcDrawer->RenderText(m_paCommanders[i]->m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color3, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		}

		pt.y += 16;
	}

	if ((m_eNodeProvokingType == AQU_NodeProvokingType::OnlyInvoker) || (m_eNodeProvokingType == AQU_NodeProvokingType::Both))
	{
		// draw the invoker joint
		pt.x = m_vecPos.x;
		pcDrawer->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color2, fZoom);
	}

	if ((m_eNodeProvokingType == AQU_NodeProvokingType::OnlyProvoker) || (m_eNodeProvokingType == AQU_NodeProvokingType::Both))
	{
		// draw the provoker joint
		pt.x = m_vecPos.x + m_vecSize.cx - 8;
		pcDrawer->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color2, fZoom);
	}

	if (m_eNodeProvokingType != AQU_NodeProvokingType::None)
	{
		// set provoker/invoker y pos
		m_cInvoker.m_lNodeYPos = pt.y - m_vecPos.y;
		m_cProvoker.m_lNodeYPos = pt.y - m_vecPos.y;

		pt.y += 16;
	}

	// draw text only if no control present
	if (!m_dwControlID)
	{
		// verify node height
		if (m_vecSize.cy < (pt.y - m_vecPos.y + 16)) m_vecSize.cy = pt.y - m_vecPos.y + 16;
	}*/

	return S_OK;
}

/**
* Window event for that node.
***/
AQU_NodeBehavior NOD_Plugin::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
// update the names of commander/decommanders
	for (DWORD i = 0; i < (DWORD)m_paCommanders.size(); i++)
		m_paCommanders[i]->m_szTitle = m_pNodus->GetCommanderName(i);
	for (DWORD i = 0; i < (DWORD)m_paDecommanders.size(); i++)
		m_paDecommanders[i]->m_szTitle = m_pNodus->GetDecommanderName(i);
	
	// get the local mouse as lParam
	LPARAM lParamLocal = (LPARAM)(m_vecLocalMouseCursor.x & 0xffff) + (((m_vecLocalMouseCursor.y - (LONG)m_dwHeaderSize) & 0xffff) << 16);

	// call plugin method
	if (m_vecLocalMouseCursor.y >= (LONG)m_dwHeaderSize)
		m_pNodus->WindowsEvent(msg, wParam, lParamLocal);

	// .. and return super method
	return NOD_Basic::WindowsEvent(msg, wParam, lParam);
}

/**
* Returns nodus dll class method.
***/
bool NOD_Plugin::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	return m_pNodus->SupportsD3DMethod(eD3D, eD3DInterface, eD3DMethod);
}

/**
* Calls super method and sets all plugin input pointers new.
***/
void NOD_Plugin::ConnectCommander(NOD_Basic* pNode, LONG nThisNodeIndex)
{
	// call super method
	NOD_Basic::ConnectCommander(pNode, nThisNodeIndex);

	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		m_pNodus->SetInputPointer((DWORD)i, m_paDecommanders[i]->m_pInput);
}

/**
* Calls super method and sets all plugin input pointers new.
***/
void NOD_Plugin::ConnectProvoker(NOD_Basic* pNode, LONG nThisNodeIndex)
{
	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		m_pNodus->SetInputPointer((DWORD)i, m_paDecommanders[i]->m_pInput);

	// call super method
	NOD_Basic::ConnectProvoker(pNode, nThisNodeIndex);
}

/**
* Calls super method and sets all plugin input pointers new.
***/
void NOD_Plugin::ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex)
{
	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		m_pNodus->SetInputPointer((DWORD)i, m_paDecommanders[i]->m_pInput);

	// call super method
	NOD_Basic::ConnectInvoker(pNode, nDestNodeIndex);
}

/**
* Node Provoke - Draw Text.
***/
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

/**
* Get the plugin nodes save data block size.
***/
DWORD NOD_Plugin::GetSaveDataSize()
{
	return m_pNodus->GetSaveDataSize();
}

/**
* Get the plugin nodes save data block.
***/
char* NOD_Plugin::GetSaveData(UINT* pdwSizeOfData)
{
	return m_pNodus->GetSaveData(pdwSizeOfData);
}

/**
* Init the plugin nodes load data block.
***/
void NOD_Plugin::InitNodeData(char* pData, UINT dwSizeOfData)
{
	m_pNodus->InitNodeData(pData, dwSizeOfData);
}

/**
* Verifies the connections of the plugin node.
***/
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


