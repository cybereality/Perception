/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyTexture.h> and
Class <D3D9ProxyTexture> :
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

#ifndef D3DPROXYTEXTURE_H_INCLUDED
#define D3DPROXYTEXTURE_H_INCLUDED

#include <d3d9.h>
#include <unordered_map>
#include "Direct3DTexture9.h"
#include "D3D9ProxySurface.h"
#include "IStereoCapableWrapper.h"

/**
*  Direct 3D proxy texture class. 
*  Overwrites IDirect3DTexture9 and imbeds the additional right texture pointer.
*/
class D3D9ProxyTexture : public BaseDirect3DTexture9, public IStereoCapableWrapper<IDirect3DTexture9>
{
public:
	D3D9ProxyTexture(IDirect3DTexture9* pActualTextureLeft, IDirect3DTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice);
	virtual ~D3D9ProxyTexture();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	
	/*** IDirect3DBaseTexture9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);	
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual DWORD   WINAPI SetPriority(DWORD PriorityNew);
	virtual void    WINAPI PreLoad();
	virtual DWORD   WINAPI SetLOD(DWORD LODNew);
	virtual HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	virtual void    WINAPI GenerateMipSubLevels();
	virtual HRESULT WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel); 
	virtual HRESULT WINAPI LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect(UINT Level);
	virtual HRESULT WINAPI AddDirtyRect(CONST RECT* pDirtyRect);

	/*** IStereoCapableWrapper methods ***/
	virtual IDirect3DTexture9* getActualMono();
	virtual IDirect3DTexture9* getActualLeft();
	virtual IDirect3DTexture9* getActualRight();
	virtual bool               IsStereo();

protected:
	/**
	* Wrapped Surface levels.
	***/
	std::unordered_map<UINT, D3D9ProxySurface*> m_wrappedSurfaceLevels;
	/**
	* The owning device.
	* @see D3D9ProxySurface::m_pOwningDevice
	***/
	BaseDirect3DDevice9* const m_pOwningDevice;
	/**
	* The actual right texture embedded. 
	***/
	IDirect3DTexture9* const m_pActualTextureRight;
};
#endif
