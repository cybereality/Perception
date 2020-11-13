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
#ifndef DCL_IDIRECT3DSWAPCHAIN9_SUPER
#define DCL_IDIRECT3DSWAPCHAIN9_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D9_QueryInterface)       (IDirect3DSwapChain9* pcThis,REFIID riid, void** ppvObj);
typedef ULONG   (WINAPI* D3D9_AddRef)               (IDirect3DSwapChain9* pcThis);
typedef ULONG   (WINAPI* D3D9_Release)              (IDirect3DSwapChain9* pcThis);

/*** IDirect3DSwapChain9 method prototypes ***/
typedef HRESULT (WINAPI* D3D9_Present)              (IDirect3DSwapChain9* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags);
typedef HRESULT (WINAPI* D3D9_GetFrontBufferData)   (IDirect3DSwapChain9* pcThis, IDirect3DSurface9* pDestSurface);
typedef HRESULT (WINAPI* D3D9_GetBackBuffer)        (IDirect3DSwapChain9* pcThis, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
typedef HRESULT (WINAPI* D3D9_GetRasterStatus)      (IDirect3DSwapChain9* pcThis, D3DRASTER_STATUS* pRasterStatus);
typedef HRESULT (WINAPI* D3D9_GetDisplayMode)       (IDirect3DSwapChain9* pcThis, D3DDISPLAYMODE* pMode);
typedef HRESULT (WINAPI* D3D9_GetDevice)            (IDirect3DSwapChain9* pcThis, IDirect3DDevice9** ppDevice);
typedef HRESULT (WINAPI* D3D9_GetPresentParameters) (IDirect3DSwapChain9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters);

/*** IUnknown super methods ***/
D3D9_QueryInterface       D3D9_IDirect3DSwapChain9_QueryInterface_Super;
D3D9_AddRef               D3D9_IDirect3DSwapChain9_AddRef_Super;
D3D9_Release              D3D9_IDirect3DSwapChain9_Release_Super;

/*** IDirect3DSwapChain9 super methods ***/
D3D9_Present              D3D9_IDirect3DSwapChain9_Present_Super;
D3D9_GetFrontBufferData   D3D9_IDirect3DSwapChain9_GetFrontBufferData_Super;
D3D9_GetBackBuffer        D3D9_IDirect3DSwapChain9_GetBackBuffer_Super;
D3D9_GetRasterStatus      D3D9_IDirect3DSwapChain9_GetRasterStatus_Super;
D3D9_GetDisplayMode       D3D9_IDirect3DSwapChain9_GetDisplayMode_Super;
D3D9_GetDevice            D3D9_IDirect3DSwapChain9_GetDevice_Super;
D3D9_GetPresentParameters D3D9_IDirect3DSwapChain9_GetPresentParameters_Super;

#endif