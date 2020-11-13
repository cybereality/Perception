/***************************************************************
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
****************************************************************/

#ifndef PCL_IDXGISWAPCHAIN_CLASS
#define PCL_IDXGISWAPCHAIN_CLASS

#include <dxgi.h>
//#include <stdio.h>

/**
* Wrapped IDXGISwapChain class, or proxy class.
***/
class PCL_IDXGISwapChain : public IDXGISwapChain
{
public:
	PCL_IDXGISwapChain(IDXGISwapChain* pIDXGISwapChain);
	virtual ~PCL_IDXGISwapChain(void);

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface           (REFIID riid, void **ppvObject);
	ULONG   WINAPI AddRef                   (void);
	ULONG   WINAPI Release                  (void);

	/*** IDXGIObject methods ***/
	HRESULT WINAPI SetPrivateData           (REFGUID Name, UINT DataSize, const void *pData);
	HRESULT WINAPI SetPrivateDataInterface  (REFGUID Name, const IUnknown *pUnknown);
	HRESULT WINAPI GetPrivateData           (REFGUID Name,UINT *pDataSize, void *pData);
	HRESULT WINAPI GetParent                (REFIID riid, void **ppParent);

	/*** IDXGIDeviceSubObject methods ***/
	HRESULT WINAPI GetDevice                (REFIID riid, void **ppDevice);

	/*** IDXGISwapChain methods ***/
	HRESULT WINAPI Present                  (UINT SyncInterval, UINT Flags);
	HRESULT WINAPI GetBuffer                (UINT Buffer, REFIID riid, void **ppSurface);
	HRESULT WINAPI SetFullscreenState       (BOOL Fullscreen, IDXGIOutput *pTarget);
	HRESULT WINAPI GetFullscreenState       (BOOL *pFullscreen, IDXGIOutput **ppTarget);
	HRESULT WINAPI GetDesc                  (DXGI_SWAP_CHAIN_DESC *pDesc);
	HRESULT WINAPI ResizeBuffers            (UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	HRESULT WINAPI ResizeTarget             (const DXGI_MODE_DESC *pNewTargetParameters);
	HRESULT WINAPI GetContainingOutput      (IDXGIOutput **ppOutput);
	HRESULT WINAPI GetFrameStatistics       (DXGI_FRAME_STATISTICS *pStats);
	HRESULT WINAPI GetLastPresentCount      (UINT *pLastPresentCount);

private:
	/**
	* The actual D3D10 swapchain.
	***/
	IDXGISwapChain* m_pIDXGISwapChain;
};

#endif

