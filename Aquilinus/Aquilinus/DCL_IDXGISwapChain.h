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
#ifndef DCL_IDXGISWAPCHAIN_CLASS
#define DCL_IDXGISWAPCHAIN_CLASS

#include <d3d11.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include "VMT_IDXGISwapChain.h"
#include "AQU_TransferSite.h"

/**
* DXGISwapChain detour class.
***/
class DCL_IDXGISwapChain
{
public:
	DCL_IDXGISwapChain(AQU_TransferSite* pcTransferSite);
	~DCL_IDXGISwapChain();

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface           (IDXGISwapChain* pcThis, REFIID riid, void **ppvObject);
	ULONG   WINAPI AddRef                   (IDXGISwapChain* pcThis);
	ULONG   WINAPI Release                  (IDXGISwapChain* pcThis);

	/*** IDXGIObject methods ***/
	HRESULT WINAPI SetPrivateData           (IDXGISwapChain* pcThis, REFGUID Name, UINT DataSize, const void *pData);
	HRESULT WINAPI SetPrivateDataInterface  (IDXGISwapChain* pcThis, REFGUID Name, const IUnknown *pUnknown);
	HRESULT WINAPI GetPrivateData           (IDXGISwapChain* pcThis, REFGUID Name,UINT *pDataSize, void *pData);
	HRESULT WINAPI GetParent                (IDXGISwapChain* pcThis, REFIID riid, void **ppParent);

	/*** IDXGIDeviceSubObject methods ***/
	HRESULT WINAPI GetDevice                (IDXGISwapChain* pcThis, REFIID riid, void **ppDevice);

	/*** IDXGISwapChain methods ***/
	HRESULT WINAPI Present                  (IDXGISwapChain* pcThis, UINT SyncInterval, UINT Flags);
	HRESULT WINAPI GetBuffer                (IDXGISwapChain* pcThis, UINT Buffer, REFIID riid, void **ppSurface);
	HRESULT WINAPI SetFullscreenState       (IDXGISwapChain* pcThis, BOOL Fullscreen, IDXGIOutput *pTarget);
	HRESULT WINAPI GetFullscreenState       (IDXGISwapChain* pcThis, BOOL *pFullscreen, IDXGIOutput **ppTarget);
	HRESULT WINAPI GetDesc                  (IDXGISwapChain* pcThis, DXGI_SWAP_CHAIN_DESC *pDesc);
	HRESULT WINAPI ResizeBuffers            (IDXGISwapChain* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	HRESULT WINAPI ResizeTarget             (IDXGISwapChain* pcThis, const DXGI_MODE_DESC *pNewTargetParameters);
	HRESULT WINAPI GetContainingOutput      (IDXGISwapChain* pcThis, IDXGIOutput **ppOutput);
	HRESULT WINAPI GetFrameStatistics       (IDXGISwapChain* pcThis, DXGI_FRAME_STATISTICS *pStats);
	HRESULT WINAPI GetLastPresentCount      (IDXGISwapChain* pcThis, UINT *pLastPresentCount);

	/*** DCL_IDXGISwapChain public methods ***/
	HRESULT        SetSuperFunctionPointers (PUINT_PTR pVMTable);
	HRESULT        GetD3D10Device           (ID3D10Device** ppDevice);
	HRESULT        GetD3D11Device           (ID3D11Device** ppDevice);

private:
	/**
	* The stored DX10 device pointer.
	***/
	ID3D10Device* m_pID3D10Device;
	/**
	* The stored DX11 device pointer.
	***/
	ID3D11Device* m_pID3D11Device;
	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
};

#endif