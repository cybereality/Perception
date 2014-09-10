/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyCubeTexture.h> and
Class <D3D9ProxyCubeTexture> :
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

#ifndef D3D9PROXYCUBETEXTURE_H_INCLUDED
#define D3D9PROXYCUBETEXTURE_H_INCLUDED

#include <d3d9.h>
#include <unordered_map>
#include "Direct3DCubeTexture9.h"
#include "D3D9ProxySurface.h"
#include "IStereoCapableWrapper.h"
#include <functional>
#include <utility>

/**
* Pair to use as key for storing surface levels.
***/
typedef std::pair<D3DCUBEMAP_FACES,UINT> CubeSurfaceKey;

/**
* Hash for surface level key.
***/
struct hash_CubeSurfaceKey {
    size_t operator()(const CubeSurfaceKey &cubeSurfaceKey ) const
    {
        return std::hash<int>()(cubeSurfaceKey.first) ^ std::hash<UINT>()(cubeSurfaceKey.second);
    }
};

/**
*  Direct 3D proxy Cube Texture class. 
*  Overwrites BaseDirect3DCubeTexture9 and imbeds the wrapped surface levels.
*/
class D3D9ProxyCubeTexture : public BaseDirect3DCubeTexture9, public IStereoCapableWrapper<IDirect3DCubeTexture9>
{
public:
	D3D9ProxyCubeTexture(IDirect3DCubeTexture9* pActualTextureLeft, IDirect3DCubeTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice);
	virtual ~D3D9ProxyCubeTexture();	
	
	/*** IDirect3DBaseTexture9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);	
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual DWORD   WINAPI SetPriority(DWORD PriorityNew);
	virtual void    WINAPI PreLoad();
	virtual DWORD   WINAPI SetLOD(DWORD LODNew);
	virtual HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	virtual void    WINAPI GenerateMipSubLevels();
	virtual HRESULT WINAPI GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface);
    virtual HRESULT WINAPI LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level);
	virtual HRESULT WINAPI AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect);
	
	// IStereoCapableWrapper	
	virtual IDirect3DCubeTexture9* getActualMono();
	virtual IDirect3DCubeTexture9* getActualLeft();
	virtual IDirect3DCubeTexture9* getActualRight();
	virtual bool                   IsStereo();

protected:
	/**
	* Wrapped Surface levels.
	***/
	std::unordered_map<CubeSurfaceKey, D3D9ProxySurface*, hash_CubeSurfaceKey> m_wrappedSurfaceLevels;
	/**
	* The owning device.
	* @see D3D9ProxySurface::m_pOwningDevice
	***/
	BaseDirect3DDevice9* const m_pOwningDevice;
	/**
	* The actual right cube texture embedded. 
	***/
	IDirect3DCubeTexture9* const m_pActualTextureRight;
};
#endif
