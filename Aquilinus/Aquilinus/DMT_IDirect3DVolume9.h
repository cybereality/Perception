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

#ifndef DMT_IDIRECT3DVOLUME9_TABLE
#define DMT_IDIRECT3DVOLUME9_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D9_IDirect3DVolume9_QueryInterface(IDirect3DVolume9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DVolume9->QueryInterface(pDevice, riid, ppvObj);}
ULONG   WINAPI D3D9_IDirect3DVolume9_AddRef(IDirect3DVolume9* pDevice){ return pDCL_IDirect3DVolume9->AddRef(pDevice);}
ULONG   WINAPI D3D9_IDirect3DVolume9_Release(IDirect3DVolume9* pDevice){ return pDCL_IDirect3DVolume9->Release(pDevice);}

/*** IDirect3DVolume9 methods ***/
HRESULT WINAPI D3D9_IDirect3DVolume9_GetDevice(IDirect3DVolume9* pcThis,  IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DVolume9->GetDevice(pcThis, ppDevice);}
HRESULT WINAPI D3D9_IDirect3DVolume9_SetPrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){ return pDCL_IDirect3DVolume9->SetPrivateData(pcThis, refguid, pData, SizeOfData, Flags);}
HRESULT WINAPI D3D9_IDirect3DVolume9_GetPrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid,void* pData,DWORD* pSizeOfData){ return pDCL_IDirect3DVolume9->GetPrivateData(pcThis, refguid, pData, pSizeOfData);}
HRESULT WINAPI D3D9_IDirect3DVolume9_FreePrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid){ return pDCL_IDirect3DVolume9->FreePrivateData(pcThis, refguid);}
HRESULT WINAPI D3D9_IDirect3DVolume9_GetContainer(IDirect3DVolume9* pcThis,  REFIID riid, void** ppContainer){ return pDCL_IDirect3DVolume9->GetContainer(pcThis, riid, ppContainer);}
HRESULT WINAPI D3D9_IDirect3DVolume9_GetDesc(IDirect3DVolume9* pcThis,  D3DVOLUME_DESC *pDesc){ return pDCL_IDirect3DVolume9->GetDesc(pcThis, pDesc);}
HRESULT WINAPI D3D9_IDirect3DVolume9_LockBox(IDirect3DVolume9* pcThis,  D3DLOCKED_BOX * pLockedVolume,CONST D3DBOX* pBox,DWORD Flags){ return pDCL_IDirect3DVolume9->LockBox(pcThis, pLockedVolume, pBox, Flags);}
HRESULT WINAPI D3D9_IDirect3DVolume9_UnlockBox(IDirect3DVolume9* pcThis){ return pDCL_IDirect3DVolume9->UnlockBox(pcThis);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DVolume9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::QueryInterface],          (UINT_PTR)D3D9_IDirect3DVolume9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::AddRef],                  (UINT_PTR)D3D9_IDirect3DVolume9_AddRef);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::Release],                 (UINT_PTR)D3D9_IDirect3DVolume9_Release);

	/*** IDirect3DVolume9 super methods ***/
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDevice],       (UINT_PTR)D3D9_IDirect3DVolume9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::SetPrivateData],  (UINT_PTR)D3D9_IDirect3DVolume9_SetPrivateData);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetPrivateData],  (UINT_PTR)D3D9_IDirect3DVolume9_GetPrivateData);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::FreePrivateData], (UINT_PTR)D3D9_IDirect3DVolume9_FreePrivateData);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetContainer],    (UINT_PTR)D3D9_IDirect3DVolume9_GetContainer);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDesc],         (UINT_PTR)D3D9_IDirect3DVolume9_GetDesc);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::LockBox],         (UINT_PTR)D3D9_IDirect3DVolume9_LockBox);
	OverrideVTable(&D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::UnlockBox],       (UINT_PTR)D3D9_IDirect3DVolume9_UnlockBox);

}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DVolume9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::QueryInterface] =          (UINT_PTR)D3D9_IDirect3DVolume9_QueryInterface;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::AddRef] =                  (UINT_PTR)D3D9_IDirect3DVolume9_AddRef;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::Release] =                 (UINT_PTR)D3D9_IDirect3DVolume9_Release;

	/*** IDirect3DVolume9 super methods ***/
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDevice] =       (UINT_PTR)D3D9_IDirect3DVolume9_GetDevice;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::SetPrivateData] =  (UINT_PTR)D3D9_IDirect3DVolume9_SetPrivateData;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetPrivateData] =  (UINT_PTR)D3D9_IDirect3DVolume9_GetPrivateData;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::FreePrivateData] = (UINT_PTR)D3D9_IDirect3DVolume9_FreePrivateData;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetContainer] =    (UINT_PTR)D3D9_IDirect3DVolume9_GetContainer;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDesc] =         (UINT_PTR)D3D9_IDirect3DVolume9_GetDesc;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::LockBox] =         (UINT_PTR)D3D9_IDirect3DVolume9_LockBox;
	anD3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::UnlockBox] =       (UINT_PTR)D3D9_IDirect3DVolume9_UnlockBox;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DVolume9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DVolume9_VMTable[0],
		(void*)&anD3D9_IDirect3DVolume9_VMTable[0],
		(D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DVolume9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::QueryInterface],          (PBYTE)D3D9_IDirect3DVolume9_QueryInterface,  JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::AddRef],                  (PBYTE)D3D9_IDirect3DVolume9_AddRef,          JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IUNKNOWN::Release],                 (PBYTE)D3D9_IDirect3DVolume9_Release,         JMP32_SZ));

	/*** IDirect3DVolume9 super methods ***/
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::GetDevice,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDevice],       (PBYTE)D3D9_IDirect3DVolume9_GetDevice,       JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::SetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::SetPrivateData],  (PBYTE)D3D9_IDirect3DVolume9_SetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::GetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetPrivateData],  (PBYTE)D3D9_IDirect3DVolume9_GetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::FreePrivateData, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::FreePrivateData], (PBYTE)D3D9_IDirect3DVolume9_FreePrivateData, JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::GetContainer,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetContainer],    (PBYTE)D3D9_IDirect3DVolume9_GetContainer,    JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::GetDesc,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::GetDesc],         (PBYTE)D3D9_IDirect3DVolume9_GetDesc,         JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::LockBox,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::LockBox],         (PBYTE)D3D9_IDirect3DVolume9_LockBox,         JMP32_SZ));
	pDCL_IDirect3DVolume9->SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::UnlockBox,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DVolume9_VMTable[VMT_IDIRECT3DVOLUME9::UnlockBox],       (PBYTE)D3D9_IDirect3DVolume9_UnlockBox,       JMP32_SZ));
}


#endif