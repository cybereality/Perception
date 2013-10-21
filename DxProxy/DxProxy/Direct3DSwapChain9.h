/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DSwapChain9.h> and
Class <BaseDirect3DSwapChain9> :
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

#ifndef DIRECT3DSwapChain9_H_INCLUDED
#define DIRECT3DSwapChain9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

/**
*  Direct 3D swap chain class. 
*  Overwrites IDirect3DSwapChain9 and imbeds the actual swap chain.
*/
class BaseDirect3DSwapChain9 : public IDirect3DSwapChain9
{
public:
	BaseDirect3DSwapChain9(IDirect3DSwapChain9* pSwapChain, BaseDirect3DDevice9* pOwningDevice, bool isAdditionalChain);
	virtual ~BaseDirect3DSwapChain9();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();
		
	/*** IDirect3DSwapChain9 methods ***/
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    virtual HRESULT WINAPI GetFrontBufferData(IDirect3DSurface9* pDestSurface);
    virtual HRESULT WINAPI GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    virtual HRESULT WINAPI GetRasterStatus(D3DRASTER_STATUS* pRasterStatus);
    virtual HRESULT WINAPI GetDisplayMode(D3DDISPLAYMODE* pMode);
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
    virtual HRESULT WINAPI GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);

	/*** BaseDirect3DSwapChain9 methods ***/
	void Destroy();

protected:
	/**
	* The actual swap chain embedded. 
	***/
	IDirect3DSwapChain9* m_pActualSwapChain;
	/**
	* Pointer to the D3D device that owns the swap chain. 
	***/
	BaseDirect3DDevice9* m_pOwningDevice;
	/**
	* Bool to ensure only additional chains are destroyed on release.
	***/
	bool m_bIsAdditionalChain;

private:
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;

};

#endif
