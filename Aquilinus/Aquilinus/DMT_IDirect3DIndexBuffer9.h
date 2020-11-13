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

#ifndef DMT_IDIRECT3DINDEXBUFFER9_TABLE
#define DMT_IDIRECT3DINDEXBUFFER9_TABLE

/*** IUnknown methods ***/
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_QueryInterface(IDirect3DIndexBuffer9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DIndexBuffer9->QueryInterface(pDevice, riid, ppvObj);}
ULONG           WINAPI D3D9_IDirect3DIndexBuffer9_AddRef(IDirect3DIndexBuffer9* pDevice){ return pDCL_IDirect3DIndexBuffer9->AddRef(pDevice);}
ULONG           WINAPI D3D9_IDirect3DIndexBuffer9_Release(IDirect3DIndexBuffer9* pDevice){ return pDCL_IDirect3DIndexBuffer9->Release(pDevice);}

/*** IDirect3DResource9 methods ***/
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_GetDevice(IDirect3DIndexBuffer9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DIndexBuffer9->GetDevice(pcThis, ppDevice);}
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_SetPrivateData(IDirect3DIndexBuffer9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){ return pDCL_IDirect3DIndexBuffer9->SetPrivateData(pcThis, refguid, pData, SizeOfData, Flags);}
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_GetPrivateData(IDirect3DIndexBuffer9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData){ return pDCL_IDirect3DIndexBuffer9->GetPrivateData(pcThis, refguid, pData, pSizeOfData);}
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_FreePrivateData(IDirect3DIndexBuffer9* pcThis, REFGUID refguid){ return pDCL_IDirect3DIndexBuffer9->FreePrivateData(pcThis, refguid);}
DWORD           WINAPI D3D9_IDirect3DIndexBuffer9_SetPriority(IDirect3DIndexBuffer9* pcThis, DWORD PriorityNew){ return pDCL_IDirect3DIndexBuffer9->SetPriority(pcThis, PriorityNew);}
DWORD           WINAPI D3D9_IDirect3DIndexBuffer9_GetPriority(IDirect3DIndexBuffer9* pcThis){ return pDCL_IDirect3DIndexBuffer9->GetPriority(pcThis);}
void            WINAPI D3D9_IDirect3DIndexBuffer9_PreLoad(IDirect3DIndexBuffer9* pcThis){ return pDCL_IDirect3DIndexBuffer9->PreLoad(pcThis);}
D3DRESOURCETYPE WINAPI D3D9_IDirect3DIndexBuffer9_GetType(IDirect3DIndexBuffer9* pcThis){ return pDCL_IDirect3DIndexBuffer9->GetType(pcThis);}

/*** IDirect3DIndexBuffer9 methods ***/
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_Lock(IDirect3DIndexBuffer9* pcThis, UINT OffsetToLock,UINT SizeToLock,void** ppbData,DWORD Flags){ return pDCL_IDirect3DIndexBuffer9->Lock(pcThis, OffsetToLock, SizeToLock, ppbData, Flags);}
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_Unlock(IDirect3DIndexBuffer9* pcThis){ return pDCL_IDirect3DIndexBuffer9->Unlock(pcThis);}
HRESULT         WINAPI D3D9_IDirect3DIndexBuffer9_GetDesc(IDirect3DIndexBuffer9* pcThis, D3DINDEXBUFFER_DESC *pDesc){ return pDCL_IDirect3DIndexBuffer9->GetDesc(pcThis, pDesc);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DIndexBuffer9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::QueryInterface],            (UINT_PTR)D3D9_IDirect3DIndexBuffer9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::AddRef],                    (UINT_PTR)D3D9_IDirect3DIndexBuffer9_AddRef);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::Release],                   (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Release);

	/*** IDirect3DResource9 super methods ***/
	//OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],       (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],  (UINT_PTR)D3D9_IDirect3DIndexBuffer9_SetPrivateData);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],  (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetPrivateData);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData], (UINT_PTR)D3D9_IDirect3DIndexBuffer9_FreePrivateData);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],     (UINT_PTR)D3D9_IDirect3DIndexBuffer9_SetPriority);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],     (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetPriority);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_PreLoad);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetType);

	/*** IDirect3DIndexBuffer9 methods ***/
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Lock],         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Lock);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Unlock],       (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Unlock);
	OverrideVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::GetDesc],      (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetDesc);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DIndexBuffer9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::QueryInterface] =            (UINT_PTR)D3D9_IDirect3DIndexBuffer9_QueryInterface;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::AddRef] =                    (UINT_PTR)D3D9_IDirect3DIndexBuffer9_AddRef;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::Release] =                   (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Release;

	/*** IDirect3DIndexBuffer9 super methods ***/
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice] =       (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetDevice;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData] =  (UINT_PTR)D3D9_IDirect3DIndexBuffer9_SetPrivateData;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData] =  (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetPrivateData;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData] = (UINT_PTR)D3D9_IDirect3DIndexBuffer9_FreePrivateData;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority] =     (UINT_PTR)D3D9_IDirect3DIndexBuffer9_SetPriority;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority] =     (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetPriority;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad] =         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_PreLoad;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType] =         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetType;

	/*** IDirect3DIndexBuffer9 methods ***/
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Lock] =         (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Lock;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Unlock] =       (UINT_PTR)D3D9_IDirect3DIndexBuffer9_Unlock;
	anD3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::GetDesc] =      (UINT_PTR)D3D9_IDirect3DIndexBuffer9_GetDesc;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DIndexBuffer9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DIndexBuffer9_VMTable[0],
		(void*)&anD3D9_IDirect3DIndexBuffer9_VMTable[0],
		(D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DIndexBuffer9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::QueryInterface],            (PBYTE)D3D9_IDirect3DIndexBuffer9_QueryInterface,  JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::AddRef],                    (PBYTE)D3D9_IDirect3DIndexBuffer9_AddRef,          JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IUNKNOWN::Release],                   (PBYTE)D3D9_IDirect3DIndexBuffer9_Release,         JMP32_SZ));

	/*** IDirect3DResource9 super methods ***/
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetDevice,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],       (PBYTE)D3D9_IDirect3DIndexBuffer9_GetDevice,       JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],  (PBYTE)D3D9_IDirect3DIndexBuffer9_SetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],  (PBYTE)D3D9_IDirect3DIndexBuffer9_GetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::FreePrivateData, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData], (PBYTE)D3D9_IDirect3DIndexBuffer9_FreePrivateData, JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPriority,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],     (PBYTE)D3D9_IDirect3DIndexBuffer9_SetPriority,     JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPriority,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],     (PBYTE)D3D9_IDirect3DIndexBuffer9_GetPriority,     JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::PreLoad,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],         (PBYTE)D3D9_IDirect3DIndexBuffer9_PreLoad,         JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetType,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],         (PBYTE)D3D9_IDirect3DIndexBuffer9_GetType,         JMP32_SZ));

	/*** IDirect3DIndexBuffer9 methods ***/
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DINDEXBUFFER9::Lock,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Lock],         (PBYTE)D3D9_IDirect3DIndexBuffer9_Lock,            JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DINDEXBUFFER9::Unlock,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::Unlock],       (PBYTE)D3D9_IDirect3DIndexBuffer9_Unlock,          JMP32_SZ));
	pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointer(VMT_IDIRECT3DINDEXBUFFER9::GetDesc,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DIndexBuffer9_VMTable[VMT_IDIRECT3DINDEXBUFFER9::GetDesc],      (PBYTE)D3D9_IDirect3DIndexBuffer9_GetDesc,         JMP32_SZ));
}


#endif