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
#ifndef NOD_IDIRECT3DBASETEXTURE9
#define NOD_IDIRECT3DBASETEXTURE9

#include "VMT_IDirect3DBaseTexture9.h"

#pragma region IDirect3DBaseTexture9 nodes

/**
* HRESULT WINAPI QueryInterface (REFIID riid, void** ppvObj);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_QueryInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI AddRef(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_AddRef, 200, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI Release(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_Release, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::Release");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT         WINAPI GetDevice       (IDirect3DDevice9** ppDevice);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetDevice, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetDevice");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDIRECT3DDEVICE9, L"ppDevice");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::GetDevice);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT         WINAPI SetPrivateData  (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_SetPrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::SetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"refguid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SizeOfData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Flags");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::SetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT         WINAPI GetPrivateData  (REFGUID refguid,void* pData,DWORD* pSizeOfData);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetPrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"refguid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pSizeOfData");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::GetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT         WINAPI FreePrivateData (REFGUID refguid);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_FreePrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::FreePrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"refguid");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::FreePrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD           WINAPI SetPriority     (DWORD PriorityNew);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_SetPriority, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::SetPriority");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"PriorityNew");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::SetPriority);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD           WINAPI GetPriority     (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetPriority, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetPriority");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::GetPriority);
AQU_D3D_NODE_FOOTER___;

/**
* void            WINAPI PreLoad         (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_PreLoad, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::PreLoad");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::PreLoad);
AQU_D3D_NODE_FOOTER___;

/**
* D3DRESOURCETYPE WINAPI GetType         (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetType, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetType");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DRESOURCE9::GetType);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD                WINAPI SetLOD               (DWORD LODNew);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_SetLOD, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::SetLOD");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"LODNew");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::SetLOD);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD                WINAPI GetLOD               (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetLOD, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetLOD");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::GetLOD);
AQU_D3D_NODE_FOOTER___;

/**
* DWORD                WINAPI GetLevelCount        (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetLevelCount, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetLevelCount");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::GetLevelCount);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT              WINAPI SetAutoGenFilterType (D3DTEXTUREFILTERTYPE FilterType);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_SetAutoGenFilterType, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::SetAutoGenFilterType");
AQU_D3D_NODE_COMMANDER(AQU_D3DTEXTUREFILTERTYPE, L"FilterType");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType);
AQU_D3D_NODE_FOOTER___;

/**
* D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GetAutoGenFilterType, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GetAutoGenFilterType");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType);
AQU_D3D_NODE_FOOTER___;

/**
* void                 WINAPI GenerateMipSubLevels (void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDirect3DBaseTexture9_GenerateMipSubLevels, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDirect3DBaseTexture9::GenerateMipSubLevels");
AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9, VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDirect3DBaseTexture9 node provider method

/**
* Node provider class.
***/
class NOD_IDirect3DBaseTexture9
{
public:
	/**
	* Provides IDirect3DBaseTexture9 node by index.
	***/
	NOD_Basic* Get_IDirect3DBaseTexture9_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::GenerateMipSubLevels) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_IDirect3DBaseTexture9_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		if (dwIndex <= (UINT)VMT_IDIRECT3DRESOURCE9::GetType) return Get_IDirect3DBaseTexture9_Node((VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9)dwIndex, nX, nY);
		return Get_IDirect3DBaseTexture9_Node((VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DBaseTexture9_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::QueryInterface:
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::AddRef:
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::Release:
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDirect3DResource9 node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DBaseTexture9_Node(VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::GetDevice :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetDevice(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::SetPrivateData :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_SetPrivateData(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::GetPrivateData :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetPrivateData(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::FreePrivateData :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_FreePrivateData(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::SetPriority :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_SetPriority(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::GetPriority :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetPriority(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::PreLoad :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_PreLoad(nX, nY);
			break;
		case VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9::GetType :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetType(nX, nY);
			break;

		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDirect3DBaseTexture9 node by enumeration.
	***/
	NOD_Basic* Get_IDirect3DBaseTexture9_Node(VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::SetLOD :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_SetLOD(nX, nY);
			break;
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::GetLOD :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetLOD(nX, nY);
			break;
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::GetLevelCount :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetLevelCount(nX, nY);
			break;
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::SetAutoGenFilterType :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_SetAutoGenFilterType(nX, nY);
			break;
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::GetAutoGenFilterType :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GetAutoGenFilterType(nX, nY);
			break;
		case VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9::GenerateMipSubLevels :
			return (NOD_Basic*)new NOD_IDirect3DBaseTexture9_GenerateMipSubLevels(nX, nY);
			break;

		default:
			break;
		}

		return nullptr;
	}
};

#pragma endregion

#endif