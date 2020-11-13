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

#ifndef DMT_IDIRECT3DSTATEBLOCK9_TABLE
#define DMT_IDIRECT3DSTATEBLOCK9_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D9_IDirect3DStateBlock9_QueryInterface(IDirect3DStateBlock9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DStateBlock9->QueryInterface(pDevice, riid, ppvObj);}
ULONG   WINAPI D3D9_IDirect3DStateBlock9_AddRef(IDirect3DStateBlock9* pDevice){ return pDCL_IDirect3DStateBlock9->AddRef(pDevice);}
ULONG   WINAPI D3D9_IDirect3DStateBlock9_Release(IDirect3DStateBlock9* pDevice){ return pDCL_IDirect3DStateBlock9->Release(pDevice);}

/*** IDirect3DStateBlock9 methods ***/
HRESULT WINAPI D3D9_IDirect3DStateBlock9_GetDevice(IDirect3DStateBlock9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DStateBlock9->GetDevice(pcThis, ppDevice);}
HRESULT WINAPI D3D9_IDirect3DStateBlock9_Capture(IDirect3DStateBlock9* pcThis){ return pDCL_IDirect3DStateBlock9->Capture(pcThis);}
HRESULT WINAPI D3D9_IDirect3DStateBlock9_Apply(IDirect3DStateBlock9* pcThis){ return pDCL_IDirect3DStateBlock9->Apply(pcThis);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DStateBlock9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::QueryInterface],        (UINT_PTR)D3D9_IDirect3DStateBlock9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::AddRef],                (UINT_PTR)D3D9_IDirect3DStateBlock9_AddRef);
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::Release],               (UINT_PTR)D3D9_IDirect3DStateBlock9_Release);

	/*** IDirect3DStateBlock9 super methods ***/
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::GetDevice], (UINT_PTR)D3D9_IDirect3DStateBlock9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Capture],   (UINT_PTR)D3D9_IDirect3DStateBlock9_Capture);
	OverrideVTable(&D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Apply],     (UINT_PTR)D3D9_IDirect3DStateBlock9_Apply);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DStateBlock9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::QueryInterface] =        (UINT_PTR)D3D9_IDirect3DStateBlock9_QueryInterface;
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::AddRef] =                (UINT_PTR)D3D9_IDirect3DStateBlock9_AddRef;
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::Release] =               (UINT_PTR)D3D9_IDirect3DStateBlock9_Release;

	/*** IDirect3DStateBlock9 super methods ***/
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::GetDevice] = (UINT_PTR)D3D9_IDirect3DStateBlock9_GetDevice;
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Capture] =   (UINT_PTR)D3D9_IDirect3DStateBlock9_Capture;
	anD3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Apply] =     (UINT_PTR)D3D9_IDirect3DStateBlock9_Apply;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DStateBlock9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DStateBlock9_VMTable[0],
		(void*)&anD3D9_IDirect3DStateBlock9_VMTable[0],
		(D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DStateBlock9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::QueryInterface],        (PBYTE)D3D9_IDirect3DStateBlock9_QueryInterface, JMP32_SZ));
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::AddRef],                (PBYTE)D3D9_IDirect3DStateBlock9_AddRef,         JMP32_SZ));
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IUNKNOWN::Release],               (PBYTE)D3D9_IDirect3DStateBlock9_Release,        JMP32_SZ));

	/*** IDirect3DStateBlock9 super methods ***/
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IDIRECT3DSTATEBLOCK9::GetDevice, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::GetDevice], (PBYTE)D3D9_IDirect3DStateBlock9_GetDevice,      JMP32_SZ));
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IDIRECT3DSTATEBLOCK9::Capture,   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Capture],   (PBYTE)D3D9_IDirect3DStateBlock9_Capture,        JMP32_SZ));
	pDCL_IDirect3DStateBlock9->SetSuperFunctionPointer(VMT_IDIRECT3DSTATEBLOCK9::Apply,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DStateBlock9_VMTable[VMT_IDIRECT3DSTATEBLOCK9::Apply],     (PBYTE)D3D9_IDirect3DStateBlock9_Apply,          JMP32_SZ));
}


#endif