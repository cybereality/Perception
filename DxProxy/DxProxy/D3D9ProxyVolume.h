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

#ifndef D3D9PROXYVOLUME_H_INCLUDED
#define D3D9PROXYvOLUME_H_INCLUDED

#include <d3d9.h>
#include "Direct3DVolume9.h"
#include "Direct3DDevice9.h"
#include <stdio.h>


/* See D3D9ProxySurface for notes on reference counting when in container */ 
class D3D9ProxyVolume : public BaseDirect3DVolume9
{
public:

	D3D9ProxyVolume(IDirect3DVolume9* pActualVolume, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer);
	virtual ~D3D9ProxyVolume();
	
	IDirect3DVolume9* getActualVolume();


	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();


	// IDirect3Dvolume9
	virtual HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer);
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);


protected:

	//Container this Volume is part of. VolumeTexture, (other?) NULL if standalone.
	IUnknown* const m_pWrappedContainer;
	BaseDirect3DDevice9* const m_pOwningDevice;

	
};

#endif
