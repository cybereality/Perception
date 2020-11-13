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
#ifndef NOD_IDIRECT3DQUERY9
#define NOD_IDIRECT3DQUERY9

#include "VMT_IDirect3DQuery9.h"

#pragma region IDirect3DQuery9 nodes

/**
* HRESULT WINAPI QueryInterface (REFIID riid, void** ppvObj);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_QueryInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI AddRef(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_AddRef, 200, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI Release(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_Release, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::Release");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT      WINAPI GetDevice              (IDirect3DQuery9* pcThis, IDirect3DDevice9** ppDevice);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_GetDevice, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::GetDevice");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDIRECT3DDEVICE9, L"ppDevice");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IDIRECT3DQUERY9::GetDevice);
AQU_D3D_NODE_FOOTER___;

/**
* D3DQUERYTYPE WINAPI GetType                (IDirect3DQuery9* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_GetType, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::GetType");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IDIRECT3DQUERY9::GetType);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD        WINAPI GetDataSize            (IDirect3DQuery9* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_GetDataSize, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::GetDataSize");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IDIRECT3DQUERY9::GetDataSize);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT      WINAPI Issue                  (IDirect3DQuery9* pcThis, DWORD dwIssueFlags);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_Issue, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::Issue");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"dwIssueFlags");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IDIRECT3DQUERY9::Issue);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT      WINAPI GetData                (IDirect3DQuery9* pcThis, void* pData,DWORD dwSize,DWORD dwGetDataFlags);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DQuery9_GetData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DQuery9::GetData");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"dwSize");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"dwGetDataFlags");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9, VMT_IDIRECT3DQUERY9::GetData);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDirect3DQuery9 node provider method

/**
* Node provider class.
***/
class NOD_IDirect3DQuery9
{
public:
	/**
	* Provides IDirect3DQuery9 node by index.
	***/
	NOD_Basic* Get_IDirect3DQuery9_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::GetData) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_IDirect3DQuery9_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_IDirect3DQuery9_Node((VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DQuery9_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::QueryInterface:
			return (NOD_Basic*)new NOD_IDirect3DQuery9_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::AddRef:
			return (NOD_Basic*)new NOD_IDirect3DQuery9_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::Release:
			return (NOD_Basic*)new NOD_IDirect3DQuery9_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDirect3DQuery9 node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DQuery9_Node(VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::GetDevice :
			return (NOD_Basic*)new NOD_IDirect3DQuery9_GetDevice(nX, nY);
			break;
		case VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::GetType :
			return (NOD_Basic*)new NOD_IDirect3DQuery9_GetType(nX, nY);
			break;
		case VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::GetDataSize :
			return (NOD_Basic*)new NOD_IDirect3DQuery9_GetDataSize(nX, nY);
			break;
		case VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::Issue :
			return (NOD_Basic*)new NOD_IDirect3DQuery9_Issue(nX, nY);
			break;
		case VMT_IDIRECT3DQUERY9::VMT_IDirect3DQuery9::GetData :
			return (NOD_Basic*)new NOD_IDirect3DQuery9_GetData(nX, nY);
			break;

		default:
			break;
		}

		return nullptr;
	}
};

#pragma endregion

#endif