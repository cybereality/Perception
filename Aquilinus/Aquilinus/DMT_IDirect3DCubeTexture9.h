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
#include"AQU_Detour.h"

#ifndef DMT_IDIRECT3DCUBETEXTURE9_TABLE
#define DMT_IDIRECT3DCUBETEXTURE9_TABLE

/*** IUnknown methods ***/
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_QueryInterface(IDirect3DCubeTexture9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DCubeTexture9->QueryInterface(pDevice, riid, ppvObj);}
ULONG                WINAPI D3D9_IDirect3DCubeTexture9_AddRef(IDirect3DCubeTexture9* pDevice){ return pDCL_IDirect3DCubeTexture9->AddRef(pDevice);}
ULONG                WINAPI D3D9_IDirect3DCubeTexture9_Release(IDirect3DCubeTexture9* pDevice){ return pDCL_IDirect3DCubeTexture9->Release(pDevice);}

/*** IDirect3DResource9 methods ***/
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_GetDevice(IDirect3DCubeTexture9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DCubeTexture9->GetDevice(pcThis, ppDevice);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_SetPrivateData(IDirect3DCubeTexture9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){ return pDCL_IDirect3DCubeTexture9->SetPrivateData(pcThis, refguid, pData, SizeOfData, Flags);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_GetPrivateData(IDirect3DCubeTexture9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData){ return pDCL_IDirect3DCubeTexture9->GetPrivateData(pcThis, refguid, pData, pSizeOfData);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_FreePrivateData(IDirect3DCubeTexture9* pcThis, REFGUID refguid){ return pDCL_IDirect3DCubeTexture9->FreePrivateData(pcThis, refguid);}
DWORD                WINAPI D3D9_IDirect3DCubeTexture9_SetPriority(IDirect3DCubeTexture9* pcThis, DWORD PriorityNew){ return pDCL_IDirect3DCubeTexture9->SetPriority(pcThis, PriorityNew);}
DWORD                WINAPI D3D9_IDirect3DCubeTexture9_GetPriority(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GetPriority(pcThis);}
void                 WINAPI D3D9_IDirect3DCubeTexture9_PreLoad(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->PreLoad(pcThis);}
D3DRESOURCETYPE      WINAPI D3D9_IDirect3DCubeTexture9_GetType(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GetType(pcThis);}

/*** IDirect3DBaseTexture9 methods ***/
DWORD                WINAPI D3D9_IDirect3DCubeTexture9_SetLOD(IDirect3DCubeTexture9* pcThis, DWORD LODNew){ return pDCL_IDirect3DCubeTexture9->SetLOD(pcThis, LODNew);}
DWORD                WINAPI D3D9_IDirect3DCubeTexture9_GetLOD(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GetLOD(pcThis);}
DWORD                WINAPI D3D9_IDirect3DCubeTexture9_GetLevelCount(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GetLevelCount(pcThis);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_SetAutoGenFilterType(IDirect3DCubeTexture9* pcThis, D3DTEXTUREFILTERTYPE FilterType){ return pDCL_IDirect3DCubeTexture9->SetAutoGenFilterType(pcThis, FilterType);}
D3DTEXTUREFILTERTYPE WINAPI D3D9_IDirect3DCubeTexture9_GetAutoGenFilterType(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GetAutoGenFilterType(pcThis);}
void                 WINAPI D3D9_IDirect3DCubeTexture9_GenerateMipSubLevels(IDirect3DCubeTexture9* pcThis){ return pDCL_IDirect3DCubeTexture9->GenerateMipSubLevels(pcThis);}

/*** IDirect3DCubeTexture9 methods ***/
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_GetLevelDesc(IDirect3DCubeTexture9* pcThis, UINT Level,D3DSURFACE_DESC *pDesc){ return pDCL_IDirect3DCubeTexture9->GetLevelDesc(pcThis, Level, pDesc);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_GetCubeMapSurface(IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType,UINT Level,IDirect3DSurface9** ppCubeMapSurface){ return pDCL_IDirect3DCubeTexture9->GetCubeMapSurface(pcThis, FaceType, Level, ppCubeMapSurface);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_LockRect(IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType,UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags){ return pDCL_IDirect3DCubeTexture9->LockRect(pcThis, FaceType, Level, pLockedRect, pRect, Flags);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_UnlockRect(IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType,UINT Level){ return pDCL_IDirect3DCubeTexture9->UnlockRect(pcThis, FaceType, Level);}
HRESULT              WINAPI D3D9_IDirect3DCubeTexture9_AddDirtyRect(IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType,CONST RECT* pDirtyRect){ return pDCL_IDirect3DCubeTexture9->AddDirtyRect(pcThis, FaceType, pDirtyRect);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DCubeTexture9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::QueryInterface],                    (UINT_PTR)D3D9_IDirect3DCubeTexture9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::AddRef],                            (UINT_PTR)D3D9_IDirect3DCubeTexture9_AddRef);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::Release],                           (UINT_PTR)D3D9_IDirect3DCubeTexture9_Release);

	/*** IDirect3DResource9 super methods ***/
	//OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],               (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],          (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetPrivateData);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],          (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetPrivateData);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData],         (UINT_PTR)D3D9_IDirect3DCubeTexture9_FreePrivateData);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],             (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetPriority);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],             (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetPriority);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],                 (UINT_PTR)D3D9_IDirect3DCubeTexture9_PreLoad);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],                 (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetType);

	/*** IDirect3DBaseTexture9 methods ***/
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD],               (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetLOD);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD],               (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLOD);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount],        (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLevelCount);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType], (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetAutoGenFilterType);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType], (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetAutoGenFilterType);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels], (UINT_PTR)D3D9_IDirect3DCubeTexture9_GenerateMipSubLevels);

	/*** IDirect3DCubeTexture9 methods ***/
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetLevelDesc],         (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLevelDesc);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetCubeMapSurface],    (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetCubeMapSurface);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::LockRect],             (UINT_PTR)D3D9_IDirect3DCubeTexture9_LockRect);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::UnlockRect],           (UINT_PTR)D3D9_IDirect3DCubeTexture9_UnlockRect);
	OverrideVTable(&D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::AddDirtyRect],         (UINT_PTR)D3D9_IDirect3DCubeTexture9_AddDirtyRect);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DCubeTexture9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::QueryInterface] =                   (UINT_PTR)D3D9_IDirect3DCubeTexture9_QueryInterface;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::AddRef] =                           (UINT_PTR)D3D9_IDirect3DCubeTexture9_AddRef;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::Release] =                          (UINT_PTR)D3D9_IDirect3DCubeTexture9_Release;

	/*** IDirect3DCubeTexture9 super methods ***/
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice] =              (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetDevice;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData] =         (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetPrivateData;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData] =         (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetPrivateData;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData] =        (UINT_PTR)D3D9_IDirect3DCubeTexture9_FreePrivateData;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority] =            (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetPriority;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority] =            (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetPriority;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad] =                (UINT_PTR)D3D9_IDirect3DCubeTexture9_PreLoad;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType] =                (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetType;

	/*** IDirect3DBaseTexture9 methods ***/
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD] =               (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetLOD;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD] =               (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLOD;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount] =        (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLevelCount;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType] = (UINT_PTR)D3D9_IDirect3DCubeTexture9_SetAutoGenFilterType;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType] = (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetAutoGenFilterType;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels] = (UINT_PTR)D3D9_IDirect3DCubeTexture9_GenerateMipSubLevels;

	/*** IDirect3DCubeTexture9 methods ***/
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetLevelDesc] =         (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetLevelDesc;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetCubeMapSurface] =    (UINT_PTR)D3D9_IDirect3DCubeTexture9_GetCubeMapSurface;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::LockRect] =             (UINT_PTR)D3D9_IDirect3DCubeTexture9_LockRect;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::UnlockRect] =           (UINT_PTR)D3D9_IDirect3DCubeTexture9_UnlockRect;
	anD3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::AddDirtyRect] =         (UINT_PTR)D3D9_IDirect3DCubeTexture9_AddDirtyRect;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DCubeTexture9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DCubeTexture9_VMTable[0],
		(void*)&anD3D9_IDirect3DCubeTexture9_VMTable[0],
		(D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DCubeTexture9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::QueryInterface],                    (PBYTE)D3D9_IDirect3DCubeTexture9_QueryInterface,           JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::AddRef],                            (PBYTE)D3D9_IDirect3DCubeTexture9_AddRef,                   JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IUNKNOWN::Release],                           (PBYTE)D3D9_IDirect3DCubeTexture9_Release,                  JMP32_SZ));

	/*** IDirect3DResource9 super methods ***/
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetDevice,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],               (PBYTE)D3D9_IDirect3DCubeTexture9_GetDevice,                JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPrivateData,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],          (PBYTE)D3D9_IDirect3DCubeTexture9_SetPrivateData,           JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPrivateData,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],          (PBYTE)D3D9_IDirect3DCubeTexture9_GetPrivateData,           JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::FreePrivateData,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData],         (PBYTE)D3D9_IDirect3DCubeTexture9_FreePrivateData,          JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPriority,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],             (PBYTE)D3D9_IDirect3DCubeTexture9_SetPriority,              JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPriority,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],             (PBYTE)D3D9_IDirect3DCubeTexture9_GetPriority,              JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::PreLoad,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],                 (PBYTE)D3D9_IDirect3DCubeTexture9_PreLoad,                  JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetType,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],                 (PBYTE)D3D9_IDirect3DCubeTexture9_GetType,                  JMP32_SZ));

	/*** IDirect3DBaseTexture9 methods ***/
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::SetLOD,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD],               (PBYTE)D3D9_IDirect3DCubeTexture9_SetLOD,                  JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetLOD,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD],               (PBYTE)D3D9_IDirect3DCubeTexture9_GetLOD,                  JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetLevelCount,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount],        (PBYTE)D3D9_IDirect3DCubeTexture9_GetLevelCount,           JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType], (PBYTE)D3D9_IDirect3DCubeTexture9_SetAutoGenFilterType,    JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType], (PBYTE)D3D9_IDirect3DCubeTexture9_GetAutoGenFilterType,    JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels], (PBYTE)D3D9_IDirect3DCubeTexture9_GenerateMipSubLevels,    JMP32_SZ));

	/*** IDirect3DCubeTexture9 methods ***/
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DCUBETEXTURE9::GetLevelDesc,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetLevelDesc],         (PBYTE)D3D9_IDirect3DCubeTexture9_GetLevelDesc,             JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DCUBETEXTURE9::GetCubeMapSurface,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::GetCubeMapSurface],    (PBYTE)D3D9_IDirect3DCubeTexture9_GetCubeMapSurface,        JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DCUBETEXTURE9::LockRect,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::LockRect],             (PBYTE)D3D9_IDirect3DCubeTexture9_LockRect,                 JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DCUBETEXTURE9::UnlockRect,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::UnlockRect],           (PBYTE)D3D9_IDirect3DCubeTexture9_UnlockRect,               JMP32_SZ));
	pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DCUBETEXTURE9::AddDirtyRect,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DCubeTexture9_VMTable[VMT_IDIRECT3DCUBETEXTURE9::AddDirtyRect],         (PBYTE)D3D9_IDirect3DCubeTexture9_AddDirtyRect,             JMP32_SZ));
}

#endif