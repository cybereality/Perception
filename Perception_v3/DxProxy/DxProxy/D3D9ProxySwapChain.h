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

#ifndef D3DPROXYSWAPCHAIN_H_INCLUDED
#define D3DPROXYSWAPCHAIN_H_INCLUDED

#include <d3d9.h>
#include <vector>
#include "Direct3DSwapChain9.h"
#include "D3D9ProxySurface.h"
#include "D3DProxyDevice.h"


class D3DProxyDevice;

/**
*  Direct 3D proxy swap chain class. 
*  Overwrites BaseDirect3DSwapChain9 and imbeds the back buffers and front buffer data.
*/
class D3D9ProxySwapChain : public BaseDirect3DSwapChain9
{
public:
	D3D9ProxySwapChain(IDirect3DSwapChain9* pActualSwapChain, D3DProxyDevice* pWrappedOwningDevice, bool isAdditionalChain);
	virtual ~D3D9ProxySwapChain();	
	
	/*** IDirect3DSwapChain9 methods ***/
    virtual HRESULT WINAPI Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    virtual HRESULT WINAPI GetFrontBufferData(IDirect3DSurface9* pDestSurface);
    virtual HRESULT WINAPI GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
	
private:
	/**
	* Currently not used front buffer proxy surface.
	***/
	D3D9ProxySurface* m_pWrappedFrontBufferData;
	/**
	* Stored indexed proxy back buffer surfaces.
	***/
	std::vector<D3D9ProxySurface*> m_backBuffers;
};
#endif
