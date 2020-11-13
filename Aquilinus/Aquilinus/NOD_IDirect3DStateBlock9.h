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
#ifndef NOD_IDIRECT3DSTATEBLOCK9
#define NOD_IDIRECT3DSTATEBLOCK9

#include "VMT_IDirect3DStateBlock9.h"

#pragma region IDirect3DStateBlock9 nodes

/**
* HRESULT WINAPI QueryInterface (REFIID riid, void** ppvObj);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_QueryInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI AddRef(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_AddRef, 200, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI Release(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_Release, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::Release");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetDevice      (IDirect3DStateBlock9* pcThis, IDirect3DDevice9** ppDevice);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_GetDevice, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::GetDevice");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDIRECT3DDEVICE9, L"ppDevice");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IDIRECT3DSTATEBLOCK9::GetDevice);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI Capture        (IDirect3DStateBlock9* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_Capture, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::Capture");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IDIRECT3DSTATEBLOCK9::Capture);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI Apply          (IDirect3DStateBlock9* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DStateBlock9_Apply, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DStateBlock9::Apply");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, VMT_IDIRECT3DSTATEBLOCK9::Apply);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDirect3DStateBlock9 node provider method

/**
* Node provider class.
***/
class NOD_IDirect3DStateBlock9
{
public:
	/**
	* Provides IDirect3DStateBlock9 node by index.
	***/
	NOD_Basic* Get_IDirect3DStateBlock9_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9::Apply) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_IDirect3DStateBlock9_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_IDirect3DStateBlock9_Node((VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DStateBlock9_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::QueryInterface:
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::AddRef:
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::Release:
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDirect3DStateBlock9 node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DStateBlock9_Node(VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9::GetDevice :
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_GetDevice(nX, nY);
			break;
		case VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9::Capture :
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_Capture(nX, nY);
			break;
		case VMT_IDIRECT3DSTATEBLOCK9::VMT_IDirect3DStateBlock9::Apply :
			return (NOD_Basic*)new NOD_IDirect3DStateBlock9_Apply(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}
};

#pragma endregion

#endif