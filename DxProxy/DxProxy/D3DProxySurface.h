/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#ifndef D3DPROXYSURFACE_H_INCLUDED
#define D3DPROXYSURFACE_H_INCLUDED

#include <d3d9.h>
#include "Direct3DSurface9.h"
#include "Direct3DDevice9.h"

class D3DProxySurface : public BaseDirect3DSurface9
{
public:
	D3DProxySurface(IDirect3DSurface9* pSurface, BaseDirect3DDevice9* pOwningDevice, IUnknown* pContainer);
	virtual ~D3DProxySurface();


	

	// IDirect3DResource9
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	//virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	//virtual HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	//virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	//virtual   DWORD WINAPI SetPriority(DWORD PriorityNew);
	//virtual   DWORD WINAPI GetPriority();
	//virtual    void WINAPI PreLoad();
	//virtual D3DRESOURCETYPE WINAPI GetType();


	

	// IDirect3DSurface9
	virtual HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer);
	//virtual HRESULT WINAPI GetDesc(D3DSURFACE_DESC *pDesc);
	//virtual HRESULT WINAPI LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	//virtual HRESULT WINAPI UnlockRect();
	//virtual HRESULT WINAPI GetDC(HDC *phdc);
	//virtual HRESULT WINAPI ReleaseDC(HDC hdc);

protected:

	// Container this surface came from. Texture, CubeTexture, SwapChain, (other?) NULL if standalone surface.
	IUnknown* m_pWrappedContainer;

	// Device that created this surface
	BaseDirect3DDevice9* m_pOwningDevice;
};

#endif
