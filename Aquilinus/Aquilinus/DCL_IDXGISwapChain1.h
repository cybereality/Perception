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
#ifndef DCL_IDXGISWAPCHAIN1_CLASS
#define DCL_IDXGISWAPCHAIN1_CLASS

#include <DXGI1_2.h>
#include "VMT_IDXGISwapChain1.h"
#include "AQU_TransferSite.h"

/**
* IDXGISwapChain1 detour class.
***/
class DCL_IDXGISwapChain1
{
public:
	DCL_IDXGISwapChain1(AQU_TransferSite* pcTransferSite);
	~DCL_IDXGISwapChain1();

	/*** IUnknown public methods ***/
	HRESULT WINAPI QueryInterface           (IDXGISwapChain1* pcThis, REFIID riid, void** ppvObject);
	ULONG WINAPI AddRef                   (IDXGISwapChain1* pcThis );
	ULONG WINAPI Release                  (IDXGISwapChain1* pcThis );
	/*** IDXGIObject public methods ***/
	HRESULT WINAPI SetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name, UINT DataSize, void* pData);
	HRESULT WINAPI SetPrivateDataInterface  (IDXGISwapChain1* pcThis, REFGUID Name, IUnknown* pUnknown);
	HRESULT WINAPI GetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name, UINT *pDataSize, void *pData );
	HRESULT WINAPI GetParent                (IDXGISwapChain1* pcThis, REFIID riid, void** ppParent);
	/*** IDXGIDeviceSubObject public methods ***/
	HRESULT WINAPI GetDevice                (IDXGISwapChain1* pcThis, REFIID riid, void** ppDevice);
	/*** IDXGISwapChain public methods ***/
	HRESULT WINAPI Present                  (IDXGISwapChain1* pcThis, UINT SyncInterval, UINT Flags);
	HRESULT WINAPI GetBuffer                (IDXGISwapChain1* pcThis, UINT Buffer, REFIID riid, void** ppSurface);
	HRESULT WINAPI SetFullscreenState       (IDXGISwapChain1* pcThis, BOOL Fullscreen, IDXGIOutput* pTarget);
	HRESULT WINAPI GetFullscreenState       (IDXGISwapChain1* pcThis, BOOL* pFullscreen, IDXGIOutput** ppTarget);
	HRESULT WINAPI GetDesc                  (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC* pDesc);
	HRESULT WINAPI ResizeBuffers            (IDXGISwapChain1* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	HRESULT WINAPI ResizeTarget             (IDXGISwapChain1* pcThis, DXGI_MODE_DESC* pNewTargetParameters);
	HRESULT WINAPI GetContainingOutput      (IDXGISwapChain1* pcThis, IDXGIOutput** ppOutput);
	HRESULT WINAPI GetFrameStatistics       (IDXGISwapChain1* pcThis, DXGI_FRAME_STATISTICS* pStats);
	HRESULT WINAPI GetLastPresentCount      (IDXGISwapChain1* pcThis, UINT* pLastPresentCount);
	/*** IDXGISwapChain1 public methods ***/
	HRESULT WINAPI GetDesc1(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC1* pDesc);
	HRESULT WINAPI GetFullscreenDesc(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc);
	HRESULT WINAPI GetHwnd(IDXGISwapChain1* pcThis, HWND* pHwnd);
	HRESULT WINAPI GetCoreWindow(IDXGISwapChain1* pcThis, REFIID refiid, void** ppUnk);
	HRESULT WINAPI Present1(IDXGISwapChain1* pcThis, UINT SyncInterval, UINT PresentFlags, DXGI_PRESENT_PARAMETERS* pPresentParameters);
	BOOL WINAPI IsTemporaryMonoSupported(IDXGISwapChain1* pcThis );
	HRESULT WINAPI GetRestrictToOutput(IDXGISwapChain1* pcThis, IDXGIOutput** ppRestrictToOutput);
	HRESULT WINAPI SetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor);
	HRESULT WINAPI GetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor);
	HRESULT WINAPI SetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION Rotation);
	HRESULT WINAPI GetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION* pRotation);

	/*** DCL_IDXGISwapChain1 public methods ***/
	HRESULT SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_IDXGIOBJECT::VMT_IDXGIObject eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1 eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointers(PUINT_PTR pVMTable);

	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
};

#endif
