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

#ifndef DMT_IDIRECT3DSURFACE9_TABLE
#define DMT_IDIRECT3DSURFACE9_TABLE

/*** IUnknown methods ***/
HRESULT         WINAPI D3D9_IDirect3DSurface9_QueryInterface(IDirect3DSurface9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DSurface9->QueryInterface(pDevice, riid, ppvObj);}
ULONG           WINAPI D3D9_IDirect3DSurface9_AddRef(IDirect3DSurface9* pDevice){ return pDCL_IDirect3DSurface9->AddRef(pDevice);}
ULONG           WINAPI D3D9_IDirect3DSurface9_Release(IDirect3DSurface9* pDevice){ return pDCL_IDirect3DSurface9->Release(pDevice);}

/*** IDirect3DResource9 methods ***/
HRESULT         WINAPI D3D9_IDirect3DSurface9_GetDevice(IDirect3DSurface9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DSurface9->GetDevice(pcThis, ppDevice);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_SetPrivateData(IDirect3DSurface9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){ return pDCL_IDirect3DSurface9->SetPrivateData(pcThis, refguid, pData, SizeOfData, Flags);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_GetPrivateData(IDirect3DSurface9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData){ return pDCL_IDirect3DSurface9->GetPrivateData(pcThis, refguid, pData, pSizeOfData);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_FreePrivateData(IDirect3DSurface9* pcThis, REFGUID refguid){ return pDCL_IDirect3DSurface9->FreePrivateData(pcThis, refguid);}
DWORD           WINAPI D3D9_IDirect3DSurface9_SetPriority(IDirect3DSurface9* pcThis, DWORD PriorityNew){ return pDCL_IDirect3DSurface9->SetPriority(pcThis, PriorityNew);}
DWORD           WINAPI D3D9_IDirect3DSurface9_GetPriority(IDirect3DSurface9* pcThis){ return pDCL_IDirect3DSurface9->GetPriority(pcThis);}
void            WINAPI D3D9_IDirect3DSurface9_PreLoad(IDirect3DSurface9* pcThis){ return pDCL_IDirect3DSurface9->PreLoad(pcThis);}
D3DRESOURCETYPE WINAPI D3D9_IDirect3DSurface9_GetType(IDirect3DSurface9* pcThis){ return pDCL_IDirect3DSurface9->GetType(pcThis);}

/*** IDirect3DSurface9 methods ***/
HRESULT         WINAPI D3D9_IDirect3DSurface9_LockRect(IDirect3DSurface9* pcThis, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags){ return pDCL_IDirect3DSurface9->LockRect(pcThis, pLockedRect, pRect, Flags);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_UnlockRect(IDirect3DSurface9* pcThis){ return pDCL_IDirect3DSurface9->UnlockRect(pcThis);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_GetDC(IDirect3DSurface9* pcThis, HDC *phdc){ return pDCL_IDirect3DSurface9->GetDC(pcThis, phdc);}
HRESULT         WINAPI D3D9_IDirect3DSurface9_ReleaseDC(IDirect3DSurface9* pcThis, HDC hdc){ return pDCL_IDirect3DSurface9->ReleaseDC(pcThis, hdc);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DSurface9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::QueryInterface],            (UINT_PTR)D3D9_IDirect3DSurface9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::AddRef],                    (UINT_PTR)D3D9_IDirect3DSurface9_AddRef);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::Release],                   (UINT_PTR)D3D9_IDirect3DSurface9_Release);

	/*** IDirect3DResource9 super methods ***/
	//OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],       (UINT_PTR)D3D9_IDirect3DSurface9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],  (UINT_PTR)D3D9_IDirect3DSurface9_SetPrivateData);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],  (UINT_PTR)D3D9_IDirect3DSurface9_GetPrivateData);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData], (UINT_PTR)D3D9_IDirect3DSurface9_FreePrivateData);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],     (UINT_PTR)D3D9_IDirect3DSurface9_SetPriority);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],     (UINT_PTR)D3D9_IDirect3DSurface9_GetPriority);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],         (UINT_PTR)D3D9_IDirect3DSurface9_PreLoad);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],         (UINT_PTR)D3D9_IDirect3DSurface9_GetType);

	/*** IDirect3DSurface9 methods ***/
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::LockRect],         (UINT_PTR)D3D9_IDirect3DSurface9_LockRect);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::UnlockRect],       (UINT_PTR)D3D9_IDirect3DSurface9_UnlockRect);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::GetDC],            (UINT_PTR)D3D9_IDirect3DSurface9_GetDC);
	OverrideVTable(&D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::ReleaseDC],        (UINT_PTR)D3D9_IDirect3DSurface9_ReleaseDC);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DSurface9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::QueryInterface] =            (UINT_PTR)D3D9_IDirect3DSurface9_QueryInterface;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::AddRef] =                    (UINT_PTR)D3D9_IDirect3DSurface9_AddRef;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::Release] =                   (UINT_PTR)D3D9_IDirect3DSurface9_Release;

	/*** IDirect3DSurface9 super methods ***/
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice] =       (UINT_PTR)D3D9_IDirect3DSurface9_GetDevice;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData] =  (UINT_PTR)D3D9_IDirect3DSurface9_SetPrivateData;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData] =  (UINT_PTR)D3D9_IDirect3DSurface9_GetPrivateData;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData] = (UINT_PTR)D3D9_IDirect3DSurface9_FreePrivateData;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority] =     (UINT_PTR)D3D9_IDirect3DSurface9_SetPriority;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority] =     (UINT_PTR)D3D9_IDirect3DSurface9_GetPriority;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad] =         (UINT_PTR)D3D9_IDirect3DSurface9_PreLoad;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType] =         (UINT_PTR)D3D9_IDirect3DSurface9_GetType;

	/*** IDirect3DSurface9 methods ***/
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::LockRect] =         (UINT_PTR)D3D9_IDirect3DSurface9_LockRect;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::UnlockRect] =       (UINT_PTR)D3D9_IDirect3DSurface9_UnlockRect;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::GetDC] =            (UINT_PTR)D3D9_IDirect3DSurface9_GetDC;
	anD3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::ReleaseDC] =        (UINT_PTR)D3D9_IDirect3DSurface9_ReleaseDC;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DSurface9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DSurface9_VMTable[0],
		(void*)&anD3D9_IDirect3DSurface9_VMTable[0],
		(D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DSurface9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::QueryInterface],            (PBYTE)D3D9_IDirect3DSurface9_QueryInterface,  JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::AddRef],                    (PBYTE)D3D9_IDirect3DSurface9_AddRef,          JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IUNKNOWN::Release],                   (PBYTE)D3D9_IDirect3DSurface9_Release,         JMP32_SZ));

	/*** IDirect3DResource9 super methods ***/
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetDevice,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetDevice],       (PBYTE)D3D9_IDirect3DSurface9_GetDevice,       JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPrivateData],  (PBYTE)D3D9_IDirect3DSurface9_SetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPrivateData],  (PBYTE)D3D9_IDirect3DSurface9_GetPrivateData,  JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::FreePrivateData, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::FreePrivateData], (PBYTE)D3D9_IDirect3DSurface9_FreePrivateData, JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::SetPriority,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::SetPriority],     (PBYTE)D3D9_IDirect3DSurface9_SetPriority,     JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetPriority,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetPriority],     (PBYTE)D3D9_IDirect3DSurface9_GetPriority,     JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::PreLoad,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::PreLoad],         (PBYTE)D3D9_IDirect3DSurface9_PreLoad,         JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DRESOURCE9::GetType,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DRESOURCE9::GetType],         (PBYTE)D3D9_IDirect3DSurface9_GetType,         JMP32_SZ));

	/*** IDirect3DSurface9 methods ***/
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DSURFACE9::LockRect,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::LockRect],         (PBYTE)D3D9_IDirect3DSurface9_LockRect,        JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DSURFACE9::UnlockRect,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::UnlockRect],       (PBYTE)D3D9_IDirect3DSurface9_UnlockRect,      JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DSURFACE9::GetDC,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::GetDC],            (PBYTE)D3D9_IDirect3DSurface9_GetDC,           JMP32_SZ));
	pDCL_IDirect3DSurface9->SetSuperFunctionPointer(VMT_IDIRECT3DSURFACE9::ReleaseDC,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSurface9_VMTable[VMT_IDIRECT3DSURFACE9::ReleaseDC],        (PBYTE)D3D9_IDirect3DSurface9_ReleaseDC,       JMP32_SZ));
}


#endif