/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVolumeTexture.h> and
Class <D3D9ProxyVolumeTexture> :
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

#ifndef D3D9PROXYVOLUMETEXTURE_H_INCLUDED
#define D3D9PROXYVOLUMETEXTURE_H_INCLUDED

#include <d3d9.h>
#include <unordered_map>
#include "Direct3DVolumeTexture9.h"
#include "D3D9ProxyVolume.h"

/**
*  Direct 3D proxy volume texture class. 
*  Overwrites BaseDirect3DVolumeTexture9 and imbeds wrapped volume levels.
*/
class D3D9ProxyVolumeTexture : public BaseDirect3DVolumeTexture9
{
public:
	D3D9ProxyVolumeTexture(IDirect3DVolumeTexture9* pActualVolumeTexture, BaseDirect3DDevice9* pOwningDevice);
	virtual ~D3D9ProxyVolumeTexture();
		
	/*** IDirect3DBaseTexture9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel);

	/*** IDirect3DVolumeTexture9 public methods ***/
	IDirect3DVolumeTexture9* getActual();	

protected:
	/**
	* Wrapped Volume levels. 
	***/
	std::unordered_map<UINT, D3D9ProxyVolume*> m_wrappedVolumeLevels;
	/**
	* The owning device.
	* @see D3D9ProxySurface::m_pOwningDevice
	***/
	BaseDirect3DDevice9* const m_pOwningDevice;
};
#endif
