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

#pragma once
#include <d3d9.h>
#include <stdio.h>
#include "D3DProxyDevice.h"
#include <cBase.h>


/**
* Direct 3D proxy volume class. 
* Overwrites BaseDirect3DVolume9 and imbeds the wrapped container this volume is part of.
*
* See D3D9ProxySurface for notes on reference counting when in container.
* @see D3D9ProxySurface
***/ 
class D3D9ProxyVolume : public cBase<IDirect3DVolume9>{
public:
	D3D9ProxyVolume(IDirect3DVolume9* pActualVolume, D3DProxyDevice* pOwningDevice, IUnknown* pWrappedContainer);

	/*** IDirect3DVolume9 methods ***/
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual HRESULT WINAPI GetDesc(D3DVOLUME_DESC *pDesc);
	virtual HRESULT WINAPI LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
	virtual HRESULT WINAPI UnlockBox();
};
