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

#ifndef DMT_IDIRECT3DTEXTURE9_TABLE
#define DMT_IDIRECT3DTEXTURE9_TABLE

/*** IUnknown methods ***/
HRESULT              WINAPI D3D9_IDirect3DTexture9_QueryInterface(IDirect3DTexture9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DTexture9->QueryInterface(pDevice, riid, ppvObj);}
ULONG                WINAPI D3D9_IDirect3DTexture9_AddRef(IDirect3DTexture9* pDevice){ return pDCL_IDirect3DTexture9->AddRef(pDevice);}
ULONG                WINAPI D3D9_IDirect3DTexture9_Release(IDirect3DTexture9* pDevice){ return pDCL_IDirect3DTexture9->Release(pDevice);}

/*** IDirect3DResource9 methods ***/
HRESULT              WINAPI D3D9_IDirect3DTexture9_GetDevice(IDirect3DTexture9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DTexture9->GetDevice(pcThis, ppDevice);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_SetPrivateData(IDirect3DTexture9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){ return pDCL_IDirect3DTexture9->SetPrivateData(pcThis, refguid, pData, SizeOfData, Flags);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_GetPrivateData(IDirect3DTexture9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData){ return pDCL_IDirect3DTexture9->GetPrivateData(pcThis, refguid, pData, pSizeOfData);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_FreePrivateData(IDirect3DTexture9* pcThis, REFGUID refguid){ return pDCL_IDirect3DTexture9->FreePrivateData(pcThis, refguid);}
DWORD                WINAPI D3D9_IDirect3DTexture9_SetPriority(IDirect3DTexture9* pcThis, DWORD PriorityNew){ return pDCL_IDirect3DTexture9->SetPriority(pcThis, PriorityNew);}
DWORD                WINAPI D3D9_IDirect3DTexture9_GetPriority(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GetPriority(pcThis);}
void                 WINAPI D3D9_IDirect3DTexture9_PreLoad(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->PreLoad(pcThis);}
D3DRESOURCETYPE      WINAPI D3D9_IDirect3DTexture9_GetType(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GetType(pcThis);}

/*** IDirect3DBaseTexture9 methods ***/
DWORD                WINAPI D3D9_IDirect3DTexture9_SetLOD(IDirect3DTexture9* pcThis, DWORD LODNew){ return pDCL_IDirect3DTexture9->SetLOD(pcThis, LODNew);}
DWORD                WINAPI D3D9_IDirect3DTexture9_GetLOD(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GetLOD(pcThis);}
DWORD                WINAPI D3D9_IDirect3DTexture9_GetLevelCount(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GetLevelCount(pcThis);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_SetAutoGenFilterType(IDirect3DTexture9* pcThis, D3DTEXTUREFILTERTYPE FilterType){ return pDCL_IDirect3DTexture9->SetAutoGenFilterType(pcThis, FilterType);}
D3DTEXTUREFILTERTYPE WINAPI D3D9_IDirect3DTexture9_GetAutoGenFilterType(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GetAutoGenFilterType(pcThis);}
void                 WINAPI D3D9_IDirect3DTexture9_GenerateMipSubLevels(IDirect3DTexture9* pcThis){ return pDCL_IDirect3DTexture9->GenerateMipSubLevels(pcThis);}

/*** IDirect3DTexture9 methods ***/
HRESULT              WINAPI D3D9_IDirect3DTexture9_GetLevelDesc(IDirect3DTexture9* pcThis, UINT Level, D3DSURFACE_DESC *pDesc){ return pDCL_IDirect3DTexture9->GetLevelDesc(pcThis, Level, pDesc);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_GetSurfaceLevel(IDirect3DTexture9* pcThis, UINT Level,IDirect3DSurface9** ppSurfaceLevel){ return pDCL_IDirect3DTexture9->GetSurfaceLevel(pcThis, Level, ppSurfaceLevel);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_LockRect(IDirect3DTexture9* pcThis, UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags){ return pDCL_IDirect3DTexture9->LockRect(pcThis, Level, pLockedRect, pRect, Flags);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_UnlockRect(IDirect3DTexture9* pcThis, UINT Level){ return pDCL_IDirect3DTexture9->UnlockRect(pcThis, Level);}
HRESULT              WINAPI D3D9_IDirect3DTexture9_AddDirtyRect(IDirect3DTexture9* pcThis, CONST RECT* pDirtyRect){ return pDCL_IDirect3DTexture9->AddDirtyRect(pcThis, pDirtyRect);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DTexture9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::QueryInterface],                    (UINT_PTR)D3D9_IDirect3DTexture9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::AddRef],                            (UINT_PTR)D3D9_IDirect3DTexture9_AddRef);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::Release],                           (UINT_PTR)D3D9_IDirect3DTexture9_Release);

	/*** IDirect3DResource9 super methods ***/
	//OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],               (UINT_PTR)D3D9_IDirect3DTexture9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],          (UINT_PTR)D3D9_IDirect3DTexture9_SetPrivateData);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],          (UINT_PTR)D3D9_IDirect3DTexture9_GetPrivateData);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData],         (UINT_PTR)D3D9_IDirect3DTexture9_FreePrivateData);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],             (UINT_PTR)D3D9_IDirect3DTexture9_SetPriority);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],             (UINT_PTR)D3D9_IDirect3DTexture9_GetPriority);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],                 (UINT_PTR)D3D9_IDirect3DTexture9_PreLoad);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],                 (UINT_PTR)D3D9_IDirect3DTexture9_GetType);

	/*** IDirect3DBaseTexture9 methods ***/
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD],               (UINT_PTR)D3D9_IDirect3DTexture9_SetLOD);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD],               (UINT_PTR)D3D9_IDirect3DTexture9_GetLOD);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount],        (UINT_PTR)D3D9_IDirect3DTexture9_GetLevelCount);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType], (UINT_PTR)D3D9_IDirect3DTexture9_SetAutoGenFilterType);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType], (UINT_PTR)D3D9_IDirect3DTexture9_GetAutoGenFilterType);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels], (UINT_PTR)D3D9_IDirect3DTexture9_GenerateMipSubLevels);

	/*** IDirect3DTexture9 methods ***/
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetLevelDesc],             (UINT_PTR)D3D9_IDirect3DTexture9_GetLevelDesc);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetSurfaceLevel],          (UINT_PTR)D3D9_IDirect3DTexture9_GetSurfaceLevel);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::LockRect],                 (UINT_PTR)D3D9_IDirect3DTexture9_LockRect);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::UnlockRect],               (UINT_PTR)D3D9_IDirect3DTexture9_UnlockRect);
	OverrideVTable(&D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::AddDirtyRect],             (UINT_PTR)D3D9_IDirect3DTexture9_AddDirtyRect);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DTexture9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::QueryInterface] =                  (UINT_PTR)D3D9_IDirect3DTexture9_QueryInterface;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::AddRef] =                          (UINT_PTR)D3D9_IDirect3DTexture9_AddRef;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::Release] =                         (UINT_PTR)D3D9_IDirect3DTexture9_Release;

	/*** IDirect3DTexture9 super methods ***/
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice] =             (UINT_PTR)D3D9_IDirect3DTexture9_GetDevice;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData] =        (UINT_PTR)D3D9_IDirect3DTexture9_SetPrivateData;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData] =        (UINT_PTR)D3D9_IDirect3DTexture9_GetPrivateData;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData] =       (UINT_PTR)D3D9_IDirect3DTexture9_FreePrivateData;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority] =           (UINT_PTR)D3D9_IDirect3DTexture9_SetPriority;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority] =           (UINT_PTR)D3D9_IDirect3DTexture9_GetPriority;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad] =               (UINT_PTR)D3D9_IDirect3DTexture9_PreLoad;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType] =               (UINT_PTR)D3D9_IDirect3DTexture9_GetType;

	/*** IDirect3DBaseTexture9 methods ***/
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD] =               (UINT_PTR)D3D9_IDirect3DTexture9_SetLOD;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD] =               (UINT_PTR)D3D9_IDirect3DTexture9_GetLOD;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount] =        (UINT_PTR)D3D9_IDirect3DTexture9_GetLevelCount;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType] = (UINT_PTR)D3D9_IDirect3DTexture9_SetAutoGenFilterType;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType] = (UINT_PTR)D3D9_IDirect3DTexture9_GetAutoGenFilterType;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels] = (UINT_PTR)D3D9_IDirect3DTexture9_GenerateMipSubLevels;

	/*** IDirect3DTexture9 methods ***/
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetLevelDesc] =             (UINT_PTR)D3D9_IDirect3DTexture9_GetLevelDesc;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetSurfaceLevel] =          (UINT_PTR)D3D9_IDirect3DTexture9_GetSurfaceLevel;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::LockRect] =                 (UINT_PTR)D3D9_IDirect3DTexture9_LockRect;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::UnlockRect] =               (UINT_PTR)D3D9_IDirect3DTexture9_UnlockRect;
	anD3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::AddDirtyRect] =             (UINT_PTR)D3D9_IDirect3DTexture9_AddDirtyRect;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DTexture9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DTexture9_VMTable[0],
		(void*)&anD3D9_IDirect3DTexture9_VMTable[0],
		(D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DTexture9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::QueryInterface],                    (PBYTE)D3D9_IDirect3DTexture9_QueryInterface,        JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::AddRef],                            (PBYTE)D3D9_IDirect3DTexture9_AddRef,                JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IUNKNOWN::Release],                           (PBYTE)D3D9_IDirect3DTexture9_Release,               JMP32_SZ));

	/*** IDirect3DResource9 super methods ***/
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetDevice,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],               (PBYTE)D3D9_IDirect3DTexture9_GetDevice,             JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPrivateData,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],          (PBYTE)D3D9_IDirect3DTexture9_SetPrivateData,        JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPrivateData,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],          (PBYTE)D3D9_IDirect3DTexture9_GetPrivateData,        JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::FreePrivateData,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData],         (PBYTE)D3D9_IDirect3DTexture9_FreePrivateData,       JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPriority,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],             (PBYTE)D3D9_IDirect3DTexture9_SetPriority,           JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPriority,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],             (PBYTE)D3D9_IDirect3DTexture9_GetPriority,           JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::PreLoad,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],                 (PBYTE)D3D9_IDirect3DTexture9_PreLoad,               JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetType,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],                 (PBYTE)D3D9_IDirect3DTexture9_GetType,               JMP32_SZ));

	/*** IDirect3DBaseTexture9 methods ***/
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::SetLOD,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetLOD],               (PBYTE)D3D9_IDirect3DTexture9_SetLOD,               JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetLOD,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLOD],               (PBYTE)D3D9_IDirect3DTexture9_GetLOD,               JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetLevelCount,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetLevelCount],        (PBYTE)D3D9_IDirect3DTexture9_GetLevelCount,        JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::SetAutoGenFilterType], (PBYTE)D3D9_IDirect3DTexture9_SetAutoGenFilterType, JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GetAutoGenFilterType], (PBYTE)D3D9_IDirect3DTexture9_GetAutoGenFilterType, JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DBASETEXTURE9::GenerateMipSubLevels], (PBYTE)D3D9_IDirect3DTexture9_GenerateMipSubLevels, JMP32_SZ));

	/*** IDirect3DTexture9 methods ***/
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DTEXTURE9::GetLevelDesc,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetLevelDesc],             (PBYTE)D3D9_IDirect3DTexture9_GetLevelDesc,         JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DTEXTURE9::GetSurfaceLevel,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::GetSurfaceLevel],          (PBYTE)D3D9_IDirect3DTexture9_GetSurfaceLevel,      JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DTEXTURE9::LockRect,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::LockRect],                 (PBYTE)D3D9_IDirect3DTexture9_LockRect,             JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DTEXTURE9::UnlockRect,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::UnlockRect],               (PBYTE)D3D9_IDirect3DTexture9_UnlockRect,           JMP32_SZ));
	pDCL_IDirect3DTexture9->SetSuperFunctionPointer(VMT_IDIRECT3DTEXTURE9::AddDirtyRect,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DTexture9_VMTable[VMT_IDIRECT3DTEXTURE9::AddDirtyRect],             (PBYTE)D3D9_IDirect3DTexture9_AddDirtyRect,         JMP32_SZ));
}

#endif