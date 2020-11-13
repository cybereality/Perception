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
#ifndef DCL_IDXGISWAPCHAIN1_SUPER
#define DCL_IDXGISWAPCHAIN1_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D10_QueryInterface           ) (IDXGISwapChain1* pcThis, REFIID riid, void** ppvObject);
typedef ULONG (WINAPI* D3D10_AddRef                   ) (IDXGISwapChain1* pcThis );
typedef ULONG (WINAPI* D3D10_Release                  ) (IDXGISwapChain1* pcThis );

/*** IDXGIObject method prototypes ***/
typedef HRESULT (WINAPI* D3D10_SetPrivateData           ) (IDXGISwapChain1* pcThis, REFGUID Name, UINT DataSize, void* pData);
typedef HRESULT (WINAPI* D3D10_SetPrivateDataInterface  ) (IDXGISwapChain1* pcThis, REFGUID Name, IUnknown* pUnknown);
typedef HRESULT (WINAPI* D3D10_GetPrivateData           ) (IDXGISwapChain1* pcThis, REFGUID Name, UINT *pDataSize, void *pData );
typedef HRESULT (WINAPI* D3D10_GetParent                ) (IDXGISwapChain1* pcThis, REFIID riid, void** ppParent);

/*** IDXGIDeviceSubObject method prototypes ***/
typedef HRESULT (WINAPI* D3D10_GetDevice                ) (IDXGISwapChain1* pcThis, REFIID riid, void** ppDevice);

/*** IDXGISwapChain method prototypes ***/
typedef HRESULT (WINAPI* D3D10_Present                  ) (IDXGISwapChain1* pcThis, UINT SyncInterval, UINT Flags);
typedef HRESULT (WINAPI* D3D10_GetBuffer                ) (IDXGISwapChain1* pcThis, UINT Buffer, REFIID riid, void** ppSurface);
typedef HRESULT (WINAPI* D3D10_SetFullscreenState       ) (IDXGISwapChain1* pcThis, BOOL Fullscreen, IDXGIOutput* pTarget);
typedef HRESULT (WINAPI* D3D10_GetFullscreenState       ) (IDXGISwapChain1* pcThis, BOOL* pFullscreen, IDXGIOutput** ppTarget);
typedef HRESULT (WINAPI* D3D10_GetDesc                  ) (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC* pDesc);
typedef HRESULT (WINAPI* D3D10_ResizeBuffers            ) (IDXGISwapChain1* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef HRESULT (WINAPI* D3D10_ResizeTarget             ) (IDXGISwapChain1* pcThis, DXGI_MODE_DESC* pNewTargetParameters);
typedef HRESULT (WINAPI* D3D10_GetContainingOutput      ) (IDXGISwapChain1* pcThis, IDXGIOutput** ppOutput);
typedef HRESULT (WINAPI* D3D10_GetFrameStatistics       ) (IDXGISwapChain1* pcThis, DXGI_FRAME_STATISTICS* pStats);
typedef HRESULT (WINAPI* D3D10_GetLastPresentCount      ) (IDXGISwapChain1* pcThis, UINT* pLastPresentCount);

/*** IDXGISwapChain1 method prototypes ***/
typedef HRESULT (WINAPI* D3D10_GetDesc1) (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC1* pDesc);
typedef HRESULT (WINAPI* D3D10_GetFullscreenDesc) (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc);
typedef HRESULT (WINAPI* D3D10_GetHwnd) (IDXGISwapChain1* pcThis, HWND* pHwnd);
typedef HRESULT (WINAPI* D3D10_GetCoreWindow) (IDXGISwapChain1* pcThis, REFIID refiid, void** ppUnk);
typedef HRESULT (WINAPI* D3D10_Present1) (IDXGISwapChain1* pcThis, UINT SyncInterval, UINT PresentFlags, DXGI_PRESENT_PARAMETERS* pPresentParameters);
typedef BOOL (WINAPI* D3D10_IsTemporaryMonoSupported) (IDXGISwapChain1* pcThis );
typedef HRESULT (WINAPI* D3D10_GetRestrictToOutput) (IDXGISwapChain1* pcThis, IDXGIOutput** ppRestrictToOutput);
typedef HRESULT (WINAPI* D3D10_SetBackgroundColor) (IDXGISwapChain1* pcThis, DXGI_RGBA* pColor);
typedef HRESULT (WINAPI* D3D10_GetBackgroundColor) (IDXGISwapChain1* pcThis, DXGI_RGBA* pColor);
typedef HRESULT (WINAPI* D3D10_SetRotation) (IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION Rotation);
typedef HRESULT (WINAPI* D3D10_GetRotation) (IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION* pRotation);

/*** IUnknown super methods ***/
D3D10_QueryInterface               D3D10_IDXGISwapChain1_QueryInterface_Super;
D3D10_AddRef                       D3D10_IDXGISwapChain1_AddRef_Super;
D3D10_Release                      D3D10_IDXGISwapChain1_Release_Super;

/*** IDXGIObject super methods ***/
D3D10_SetPrivateData               D3D10_IDXGISwapChain1_SetPrivateData_Super;
D3D10_SetPrivateDataInterface      D3D10_IDXGISwapChain1_SetPrivateDataInterface_Super;
D3D10_GetPrivateData               D3D10_IDXGISwapChain1_GetPrivateData_Super;
D3D10_GetParent                    D3D10_IDXGISwapChain1_GetParent_Super;

/*** IDXGIDeviceSubObject super methods ***/
D3D10_GetDevice                    D3D10_IDXGISwapChain1_GetDevice_Super;

/*** IDXGISwapChain super methods ***/
D3D10_Present                      D3D10_IDXGISwapChain1_Present_Super;
D3D10_GetBuffer                    D3D10_IDXGISwapChain1_GetBuffer_Super;
D3D10_SetFullscreenState           D3D10_IDXGISwapChain1_SetFullscreenState_Super;
D3D10_GetFullscreenState           D3D10_IDXGISwapChain1_GetFullscreenState_Super;
D3D10_GetDesc                      D3D10_IDXGISwapChain1_GetDesc_Super;
D3D10_ResizeBuffers                D3D10_IDXGISwapChain1_ResizeBuffers_Super;
D3D10_ResizeTarget                 D3D10_IDXGISwapChain1_ResizeTarget_Super;
D3D10_GetContainingOutput          D3D10_IDXGISwapChain1_GetContainingOutput_Super;
D3D10_GetFrameStatistics           D3D10_IDXGISwapChain1_GetFrameStatistics_Super;
D3D10_GetLastPresentCount          D3D10_IDXGISwapChain1_GetLastPresentCount_Super;

/*** IDXGISwapChain1 super methods ***/
D3D10_GetDesc1    D3D10_IDXGISwapChain1_GetDesc1_Super;
D3D10_GetFullscreenDesc    D3D10_IDXGISwapChain1_GetFullscreenDesc_Super;
D3D10_GetHwnd    D3D10_IDXGISwapChain1_GetHwnd_Super;
D3D10_GetCoreWindow    D3D10_IDXGISwapChain1_GetCoreWindow_Super;
D3D10_Present1    D3D10_IDXGISwapChain1_Present1_Super;
D3D10_IsTemporaryMonoSupported    D3D10_IDXGISwapChain1_IsTemporaryMonoSupported_Super;
D3D10_GetRestrictToOutput    D3D10_IDXGISwapChain1_GetRestrictToOutput_Super;
D3D10_SetBackgroundColor    D3D10_IDXGISwapChain1_SetBackgroundColor_Super;
D3D10_GetBackgroundColor    D3D10_IDXGISwapChain1_GetBackgroundColor_Super;
D3D10_SetRotation    D3D10_IDXGISwapChain1_SetRotation_Super;
D3D10_GetRotation    D3D10_IDXGISwapChain1_GetRotation_Super;

#endif
