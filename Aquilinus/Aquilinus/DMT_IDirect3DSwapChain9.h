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

#ifndef DMT_IDIRECT3DSWAPCHAIN9_TABLE
#define DMT_IDIRECT3DSWAPCHAIN9_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D9_IDirect3DSwapChain9_QueryInterface(IDirect3DSwapChain9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DSwapChain9->QueryInterface(pDevice, riid, ppvObj);}
ULONG   WINAPI D3D9_IDirect3DSwapChain9_AddRef(IDirect3DSwapChain9* pDevice){ return pDCL_IDirect3DSwapChain9->AddRef(pDevice);}
ULONG   WINAPI D3D9_IDirect3DSwapChain9_Release(IDirect3DSwapChain9* pDevice){ return pDCL_IDirect3DSwapChain9->Release(pDevice);}

/*** IDirect3DSwapChain9 methods ***/
HRESULT WINAPI D3D9_IDirect3DSwapChain9_Present(IDirect3DSwapChain9* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)  { return pDCL_IDirect3DSwapChain9->Present(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetFrontBufferData(IDirect3DSwapChain9* pcThis, IDirect3DSurface9* pDestSurface)  { return pDCL_IDirect3DSwapChain9->GetFrontBufferData(pcThis, pDestSurface);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetBackBuffer(IDirect3DSwapChain9* pcThis, UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)  { return pDCL_IDirect3DSwapChain9->GetBackBuffer(pcThis, iBackBuffer, Type, ppBackBuffer);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetRasterStatus(IDirect3DSwapChain9* pcThis, D3DRASTER_STATUS* pRasterStatus)  { return pDCL_IDirect3DSwapChain9->GetRasterStatus(pcThis, pRasterStatus);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetDisplayMode(IDirect3DSwapChain9* pcThis, D3DDISPLAYMODE* pMode)  { return pDCL_IDirect3DSwapChain9->GetDisplayMode(pcThis, pMode);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetDevice(IDirect3DSwapChain9* pcThis, IDirect3DDevice9** ppDevice)  { return pDCL_IDirect3DSwapChain9->GetDevice(pcThis, ppDevice);}
HRESULT WINAPI D3D9_IDirect3DSwapChain9_GetPresentParameters(IDirect3DSwapChain9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters)  { return pDCL_IDirect3DSwapChain9->GetPresentParameters(pcThis, pPresentationParameters);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DSwapChain9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::QueryInterface],                  (UINT_PTR)D3D9_IDirect3DSwapChain9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::AddRef],                          (UINT_PTR)D3D9_IDirect3DSwapChain9_AddRef);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::Release],                         (UINT_PTR)D3D9_IDirect3DSwapChain9_Release);

	/*** IDirect3DSwapChain9 super methods ***/
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::Present],              (UINT_PTR)D3D9_IDirect3DSwapChain9_Present);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData],   (UINT_PTR)D3D9_IDirect3DSwapChain9_GetFrontBufferData);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer],        (UINT_PTR)D3D9_IDirect3DSwapChain9_GetBackBuffer);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus],      (UINT_PTR)D3D9_IDirect3DSwapChain9_GetRasterStatus);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode],       (UINT_PTR)D3D9_IDirect3DSwapChain9_GetDisplayMode);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDevice],            (UINT_PTR)D3D9_IDirect3DSwapChain9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters], (UINT_PTR)D3D9_IDirect3DSwapChain9_GetPresentParameters);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DSwapChain9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::QueryInterface] =                  (UINT_PTR)D3D9_IDirect3DSwapChain9_QueryInterface;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::AddRef] =                          (UINT_PTR)D3D9_IDirect3DSwapChain9_AddRef;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::Release] =                         (UINT_PTR)D3D9_IDirect3DSwapChain9_Release;

	/*** IDirect3DSwapChain9 super methods ***/
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::Present] =              (UINT_PTR)D3D9_IDirect3DSwapChain9_Present;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData] =   (UINT_PTR)D3D9_IDirect3DSwapChain9_GetFrontBufferData;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer] =        (UINT_PTR)D3D9_IDirect3DSwapChain9_GetBackBuffer;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus] =      (UINT_PTR)D3D9_IDirect3DSwapChain9_GetRasterStatus;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode] =       (UINT_PTR)D3D9_IDirect3DSwapChain9_GetDisplayMode;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDevice] =            (UINT_PTR)D3D9_IDirect3DSwapChain9_GetDevice;
	anD3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters] = (UINT_PTR)D3D9_IDirect3DSwapChain9_GetPresentParameters;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DSwapChain9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DSwapChain9_VMTable[0],
		(void*)&anD3D9_IDirect3DSwapChain9_VMTable[0],
		(D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DSwapChain9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::QueryInterface],                  (PBYTE)D3D9_IDirect3DSwapChain9_QueryInterface,       JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::AddRef],                          (PBYTE)D3D9_IDirect3DSwapChain9_AddRef,               JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IUNKNOWN::Release],                         (PBYTE)D3D9_IDirect3DSwapChain9_Release,              JMP32_SZ));

	/*** IDirect3DSwapChain9 super methods ***/
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::Present,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::Present],              (PBYTE)D3D9_IDirect3DSwapChain9_Present,              JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData,   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData],   (PBYTE)D3D9_IDirect3DSwapChain9_GetFrontBufferData,   JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer],        (PBYTE)D3D9_IDirect3DSwapChain9_GetBackBuffer,        JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus],      (PBYTE)D3D9_IDirect3DSwapChain9_GetRasterStatus,      JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode],       (PBYTE)D3D9_IDirect3DSwapChain9_GetDisplayMode,       JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetDevice,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDevice],            (PBYTE)D3D9_IDirect3DSwapChain9_GetDevice,            JMP32_SZ));
	pDCL_IDirect3DSwapChain9->SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DSwapChain9_VMTable[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters], (PBYTE)D3D9_IDirect3DSwapChain9_GetPresentParameters, JMP32_SZ));
}


#endif