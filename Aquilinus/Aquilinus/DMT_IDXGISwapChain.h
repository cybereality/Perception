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

#ifndef DMT_IDXGISWAPCHAIN_TABLE
#define DMT_IDXGISWAPCHAIN_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain_QueryInterface(IDXGISwapChain* pSwapChain, REFIID riid, void **ppvObject) { return pDCL_IDXGISwapChain->QueryInterface(pSwapChain, riid, ppvObject); }
ULONG   WINAPI D3D10_IDXGISwapChain_AddRef(IDXGISwapChain* pSwapChain) { return pDCL_IDXGISwapChain->AddRef(pSwapChain); }
ULONG   WINAPI D3D10_IDXGISwapChain_Release(IDXGISwapChain* pSwapChain) { return pDCL_IDXGISwapChain->Release(pSwapChain); }

/*** IDXGIObject methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain_SetPrivateData(IDXGISwapChain* pSwapChain, REFGUID Name, UINT DataSize, const void *pData) { return pDCL_IDXGISwapChain->SetPrivateData(pSwapChain, Name, DataSize, pData); }
HRESULT WINAPI D3D10_IDXGISwapChain_SetPrivateDataInterface(IDXGISwapChain* pSwapChain, REFGUID Name, const IUnknown *pUnknown) { return pDCL_IDXGISwapChain->SetPrivateDataInterface(pSwapChain, Name, pUnknown); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetPrivateData(IDXGISwapChain* pSwapChain, REFGUID Name,UINT *pDataSize, void *pData) { return pDCL_IDXGISwapChain->GetPrivateData(pSwapChain, Name, pDataSize, pData); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetParent(IDXGISwapChain* pSwapChain, REFIID riid, void **ppParent) { return pDCL_IDXGISwapChain->GetParent(pSwapChain, riid, ppParent); }

/*** IDXGIDeviceSubObject methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain_GetDevice(IDXGISwapChain* pSwapChain, REFIID riid, void **ppDevice) { return pDCL_IDXGISwapChain->GetDevice(pSwapChain, riid, ppDevice); }

/*** IDXGISwapChain methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) { return pDCL_IDXGISwapChain->Present(pSwapChain, SyncInterval, Flags); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetBuffer(IDXGISwapChain* pSwapChain, UINT Buffer, REFIID riid, void **ppSurface) { return pDCL_IDXGISwapChain->GetBuffer(pSwapChain, Buffer, riid, ppSurface); }
HRESULT WINAPI D3D10_IDXGISwapChain_SetFullscreenState(IDXGISwapChain* pSwapChain, BOOL Fullscreen, IDXGIOutput *pTarget) { return pDCL_IDXGISwapChain->SetFullscreenState(pSwapChain, Fullscreen, pTarget); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetFullscreenState(IDXGISwapChain* pSwapChain, BOOL *pFullscreen, IDXGIOutput **ppTarget) { return pDCL_IDXGISwapChain->GetFullscreenState(pSwapChain, pFullscreen, ppTarget); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetDesc(IDXGISwapChain* pSwapChain, DXGI_SWAP_CHAIN_DESC *pDesc) { return pDCL_IDXGISwapChain->GetDesc(pSwapChain, pDesc); }
HRESULT WINAPI D3D10_IDXGISwapChain_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) { return pDCL_IDXGISwapChain->ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags); }
HRESULT WINAPI D3D10_IDXGISwapChain_ResizeTarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC *pNewTargetParameters) { return pDCL_IDXGISwapChain->ResizeTarget(pSwapChain, pNewTargetParameters); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetContainingOutput(IDXGISwapChain* pSwapChain, IDXGIOutput **ppOutput) { return pDCL_IDXGISwapChain->GetContainingOutput(pSwapChain, ppOutput); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetFrameStatistics(IDXGISwapChain* pSwapChain, DXGI_FRAME_STATISTICS *pStats) { return pDCL_IDXGISwapChain->GetFrameStatistics(pSwapChain, pStats); }
HRESULT WINAPI D3D10_IDXGISwapChain_GetLastPresentCount(IDXGISwapChain* pSwapChain, UINT *pLastPresentCount) { return pDCL_IDXGISwapChain->GetLastPresentCount(pSwapChain, pLastPresentCount); }

/**
* Overrides the Virtual Methods Table of the DirectX 10 DXGISwapChain
* I assume that most methods here are unstable when overridden so i commented them out.
***/
void Override_D3D10_IDXGISwapChain_VMTable()
{
	/*** IUnknown super methods ***/
	/*OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IUNKNOWN::QueryInterface],                (UINT_PTR)D3D10_IDXGISwapChain_QueryInterface);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IUNKNOWN::AddRef],                        (UINT_PTR)D3D10_IDXGISwapChain_AddRef);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IUNKNOWN::Release],                       (UINT_PTR)D3D10_IDXGISwapChain_Release);*/

	/*** IDXGIObject super methods ***/
	/*OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::SetPrivateData],             (UINT_PTR)D3D10_IDXGISwapChain_SetPrivateData);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface],    (UINT_PTR)D3D10_IDXGISwapChain_SetPrivateDataInterface);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::GetPrivateData],             (UINT_PTR)D3D10_IDXGISwapChain_GetPrivateData);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::GetParent],                  (UINT_PTR)D3D10_IDXGISwapChain_GetParent);*/

	/*** IDXGIDeviceSubObject super methods ***/
	/*OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice],         (UINT_PTR)D3D10_IDXGISwapChain_GetDevice);*/

	/*** IDXGISwapChain super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::Present],                 (UINT_PTR)D3D10_IDXGISwapChain_Present);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetBuffer],               (UINT_PTR)D3D10_IDXGISwapChain_GetBuffer);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState],      (UINT_PTR)D3D10_IDXGISwapChain_SetFullscreenState);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState],      (UINT_PTR)D3D10_IDXGISwapChain_GetFullscreenState);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetDesc],                 (UINT_PTR)D3D10_IDXGISwapChain_GetDesc);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers],           (UINT_PTR)D3D10_IDXGISwapChain_ResizeBuffers);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::ResizeTarget],            (UINT_PTR)D3D10_IDXGISwapChain_ResizeTarget);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput],     (UINT_PTR)D3D10_IDXGISwapChain_GetContainingOutput);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics],      (UINT_PTR)D3D10_IDXGISwapChain_GetFrameStatistics);
	OverrideVTable(&D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount],     (UINT_PTR)D3D10_IDXGISwapChain_GetLastPresentCount);
}

/// <summary>
/// Overrides the Virtual Methods Table of the DirectX 11 Device
/// </summary>
void Detour_D3D10_IDXGISwapChain_VMTable()
{
	OutputDebugString(L"[AQU] Detour_D3D10_IDXGISwapChain_VMTable");

	/*** IUnknown super methods ***/
	/*pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::QueryInterface], (void*)D3D11_ID3D11Device_QueryInterface));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::AddRef], (void*)D3D11_ID3D11Device_AddRef));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IUNKNOWN::Release, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::Release], (void*)D3D11_ID3D11Device_Release));*/

	/*** IDXGIObject super methods ***/
	/*pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGIOBJECT::SetPrivateData, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::SetPrivateData], (void*)D3D10_IDXGISwapChain_SetPrivateData));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGIOBJECT::SetPrivateDataInterface, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface], (void*)D3D10_IDXGISwapChain_SetPrivateDataInterface));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGIOBJECT::GetPrivateData, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::GetPrivateData], (void*)D3D10_IDXGISwapChain_GetPrivateData));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGIOBJECT::GetParent, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGIOBJECT::GetParent], (void*)D3D10_IDXGISwapChain_GetParent));*/

	/*** IDXGIDeviceSubObject super methods ***/
	/*pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGIDEVICESUBOBJECT::GetDevice, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice], (void*)D3D10_IDXGISwapChain_GetDevice));*/

	/*** IDXGISwapChain super methods ***/
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::Present, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::Present], (void*)D3D10_IDXGISwapChain_Present));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetBuffer, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetBuffer], (void*)D3D10_IDXGISwapChain_GetBuffer));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::SetFullscreenState, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState], (void*)D3D10_IDXGISwapChain_SetFullscreenState));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetFullscreenState, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState], (void*)D3D10_IDXGISwapChain_GetFullscreenState));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetDesc, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetDesc], (void*)D3D10_IDXGISwapChain_GetDesc));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::ResizeBuffers, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers], (void*)D3D10_IDXGISwapChain_ResizeBuffers));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::ResizeTarget, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::ResizeTarget], (void*)D3D10_IDXGISwapChain_ResizeTarget));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetContainingOutput, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput], (void*)D3D10_IDXGISwapChain_GetContainingOutput));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetFrameStatistics, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics], (void*)D3D10_IDXGISwapChain_GetFrameStatistics));
	pDCL_IDXGISwapChain->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetLastPresentCount, (UINT_PTR)DetourFuncMinHook((void*)D3D10_IDXGISwapChain_VMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount], (void*)D3D10_IDXGISwapChain_GetLastPresentCount));
}

#endif