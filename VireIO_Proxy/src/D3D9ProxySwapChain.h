/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxySwapChain.h> and
Class <D3D9ProxySwapChain> :
Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#pragma once
#include "D3D9ProxySurface.h"
#include "D3DProxyDevice.h"
#include <cBase.h>
#include <vector>

class D3D9ProxySwapChain : public cBase<IDirect3DSwapChain9>{
public:
	D3D9ProxySwapChain(IDirect3DSwapChain9* pActualSwapChain, D3DProxyDevice* pWrappedOwningDevice, bool isAdditionalChain);
	~D3D9ProxySwapChain();	
	
	ULONG   WINAPI Release();
	HRESULT WINAPI Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    HRESULT WINAPI GetFrontBufferData(IDirect3DSurface9* pDestSurface);
    HRESULT WINAPI GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    HRESULT WINAPI GetRasterStatus(D3DRASTER_STATUS* pRasterStatus);
    HRESULT WINAPI GetDisplayMode(D3DDISPLAYMODE* pMode);
    HRESULT WINAPI GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
	void           Destroy();


private:
	//Bool to ensure only additional chains are destroyed on release.
	bool m_bIsAdditionalChain;

	//Stored indexed proxy back buffer surfaces.
	std::vector<D3D9ProxySurface*> m_backBuffers;
};
