/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxySurface.h> and
Class <D3D9ProxySurface> :
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

#ifndef D3D9PROXYSURFACE_H_INCLUDED
#define D3D9PROXYSURFACE_H_INCLUDED

#include <d3d9.h>
#include "Direct3DSurface9.h"
#include "Direct3DDevice9.h"
#include "IStereoCapableWrapper.h"
#include <stdio.h>


/**
*  Direct 3D proxy surface class. 
*  Overwrites wrapped surface class (BaseDirect3DSurface9) and imbeds additional right surface.
*/
class D3D9ProxySurface : public BaseDirect3DSurface9, public IStereoCapableWrapper<IDirect3DSurface9>
{
public:
	D3D9ProxySurface(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer);
	virtual ~D3D9ProxySurface();

	/*** IUnknown methods ***/
	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();

	/*** IDirect3DResource9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual DWORD   WINAPI SetPriority(DWORD PriorityNew);
	virtual void    WINAPI PreLoad();
	virtual HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer);
	virtual HRESULT WINAPI LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect();
	virtual HRESULT WINAPI ReleaseDC(HDC hdc);

	/*** D3D9ProxySurface methods ***/
	virtual IDirect3DSurface9* getActualMono();
	virtual IDirect3DSurface9* getActualLeft();
	virtual IDirect3DSurface9* getActualRight();
	virtual bool               IsStereo();

protected:
	/**
	* Container this surface is part of. Texture, CubeTexture, SwapChain, (other?) NULL if standalone surface.
	*
	* If a surface is in a container then they use the containers ref count as a shared total ref count.
	* The container is responsible for deleting any surfaces it contains when the ref count reaches 0.
	* Surfaces must be deleted before the container.
	***/
	IUnknown* const m_pWrappedContainer;
	/** 
	* Device that created this surface.
	* If in a container we don't add a ref or release the device when done because...
	* D3D only keeps one ref to the device for the container and all its surfaces. A contained
	* Surface doesn't exist without its container, if the container is destroyed so is the surface.
	* The container handles the device ref count changes.
	*
	* This device pointer is maintained here to simplify GetDevice. Normally keeping the pointer would need
	* an add ref, but that would produce different results compared to D3Ds normal behvaiour.
	* As the surface is destroyed with the container this should be safe enough.
	* 
	* It would probably be more correct to remove this pointer and use QueryInterface to check for 
	* IDirect3DResource9 or Direct3DISwapChain9 then cast the container to the appropriate interface 
	* and the use GetDevice to fetch the device. 
	***/
	BaseDirect3DDevice9* const m_pOwningDevice;
	/**
	* Right surface. 
	* NULL for surfaces that aren't being duplicated.
	***/
	IDirect3DSurface9* const m_pActualSurfaceRight;
};
#endif
