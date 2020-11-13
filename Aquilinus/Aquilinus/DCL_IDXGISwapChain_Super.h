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
#ifndef DCL_IDXGISWAPCHAIN_SUPER
#define DCL_IDXGISWAPCHAIN_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D10_QueryInterface)          (IDXGISwapChain* pSwapChain, REFIID riid, void **ppvObject);
typedef ULONG   (WINAPI* D3D10_AddRef)                  (IDXGISwapChain* pSwapChain);
typedef ULONG   (WINAPI* D3D10_Release)                 (IDXGISwapChain* pSwapChain);

/*** IDXGIObject method prototypes ***/
typedef HRESULT (WINAPI* D3D10_SetPrivateData)          (IDXGISwapChain* pSwapChain, REFGUID Name, UINT DataSize, const void *pData);
typedef HRESULT (WINAPI* D3D10_SetPrivateDataInterface) (IDXGISwapChain* pSwapChain, REFGUID Name, const IUnknown *pUnknown);
typedef HRESULT (WINAPI* D3D10_GetPrivateData)          (IDXGISwapChain* pSwapChain, REFGUID Name,UINT *pDataSize, void *pData);
typedef HRESULT (WINAPI* D3D10_GetParent)               (IDXGISwapChain* pSwapChain, REFIID riid, void **ppParent);

/*** IDXGIDeviceSubObject method prototypes ***/
typedef HRESULT (WINAPI* D3D10_GetDevice)               (IDXGISwapChain* pSwapChain, REFIID riid, void **ppDevice);

/*** IDXGISwapChain method prototypes ***/
typedef HRESULT (WINAPI* D3D10_Present)                 (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT (WINAPI* D3D10_GetBuffer)               (IDXGISwapChain* pSwapChain, UINT Buffer, REFIID riid, void **ppSurface);
typedef HRESULT (WINAPI* D3D10_SetFullscreenState)      (IDXGISwapChain* pSwapChain, BOOL Fullscreen, IDXGIOutput *pTarget);
typedef HRESULT (WINAPI* D3D10_GetFullscreenState)      (IDXGISwapChain* pSwapChain, BOOL *pFullscreen, IDXGIOutput **ppTarget);
typedef HRESULT (WINAPI* D3D10_GetDesc)                 (IDXGISwapChain* pSwapChain, DXGI_SWAP_CHAIN_DESC *pDesc);
typedef HRESULT (WINAPI* D3D10_ResizeBuffers)           (IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef HRESULT (WINAPI* D3D10_ResizeTarget)            (IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC *pNewTargetParameters);
typedef HRESULT (WINAPI* D3D10_GetContainingOutput)     (IDXGISwapChain* pSwapChain, IDXGIOutput **ppOutput);
typedef HRESULT (WINAPI* D3D10_GetFrameStatistics)      (IDXGISwapChain* pSwapChain, DXGI_FRAME_STATISTICS *pStats);
typedef HRESULT (WINAPI* D3D10_GetLastPresentCount)     (IDXGISwapChain* pSwapChain, UINT *pLastPresentCount);

/*** IUnknown super methods ***/
D3D10_QueryInterface          D3D10_IDXGISwapChain_QueryInterface_Super = NULL;
D3D10_AddRef                  D3D10_IDXGISwapChain_AddRef_Super = NULL;
D3D10_Release                 D3D10_IDXGISwapChain_Release_Super = NULL;

/*** IDXGIObject super methods ***/
D3D10_SetPrivateData          D3D10_IDXGISwapChain_SetPrivateData_Super = NULL;
D3D10_SetPrivateDataInterface D3D10_IDXGISwapChain_SetPrivateDataInterface_Super = NULL;
D3D10_GetPrivateData          D3D10_IDXGISwapChain_GetPrivateData_Super = NULL;
D3D10_GetParent               D3D10_IDXGISwapChain_GetParent_Super = NULL;

/*** IDXGIDeviceSubObject super methods ***/
D3D10_GetDevice               D3D10_IDXGISwapChain_GetDevice_Super = NULL;

/*** IDXGISwapChain super methods ***/
D3D10_Present                 D3D10_IDXGISwapChain_Present_Super = NULL;
D3D10_GetBuffer               D3D10_IDXGISwapChain_GetBuffer_Super = NULL;
D3D10_SetFullscreenState      D3D10_IDXGISwapChain_SetFullscreenState_Super = NULL;
D3D10_GetFullscreenState      D3D10_IDXGISwapChain_GetFullscreenState_Super = NULL;
D3D10_GetDesc                 D3D10_IDXGISwapChain_GetDesc_Super = NULL;
D3D10_ResizeBuffers           D3D10_IDXGISwapChain_ResizeBuffers_Super = NULL;
D3D10_ResizeTarget            D3D10_IDXGISwapChain_ResizeTarget_Super = NULL;
D3D10_GetContainingOutput     D3D10_IDXGISwapChain_GetContainingOutput_Super = NULL;
D3D10_GetFrameStatistics      D3D10_IDXGISwapChain_GetFrameStatistics_Super = NULL;
D3D10_GetLastPresentCount     D3D10_IDXGISwapChain_GetLastPresentCount_Super = NULL;

#endif