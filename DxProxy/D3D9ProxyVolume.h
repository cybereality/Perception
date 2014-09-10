/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVolume.h> and
Class <D3D9ProxyVolume> :
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

#ifndef D3D9PROXYVOLUME_H_INCLUDED
#define D3D9PROXYvOLUME_H_INCLUDED

#include <d3d9.h>
#include "Direct3DVolume9.h"
#include "Direct3DDevice9.h"
#include <stdio.h>


/**
* Direct 3D proxy volume class. 
* Overwrites BaseDirect3DVolume9 and imbeds the wrapped container this volume is part of.
*
* See D3D9ProxySurface for notes on reference counting when in container.
* @see D3D9ProxySurface
***/ 
class D3D9ProxyVolume : public BaseDirect3DVolume9
{
public:
	D3D9ProxyVolume(IDirect3DVolume9* pActualVolume, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer);
	virtual ~D3D9ProxyVolume();
	
	/*** IUnknown methods ***/
	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();
	
	/*** IDirect3DVolume9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer);

	/*** D3D9ProxyVolume public methods ***/
	IDirect3DVolume9* getActualVolume();

protected:
	/**
	* Container this Volume is part of. 
	* VolumeTexture, (other?) NULL if standalone.
	***/
	IUnknown* const m_pWrappedContainer;
	/**
	* The owning device.
	* @see D3D9ProxySurface::m_pOwningDevice
	***/
	BaseDirect3DDevice9* const m_pOwningDevice;
};
#endif
