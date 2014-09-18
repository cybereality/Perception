/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVolume.cpp> and
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

#include "D3DProxyDevice.h"
#include "D3D9ProxyVolume.h"

/**
* Constructor.
* If the Proxy volume is in a container it will have a combined ref count with it's container
* and that count is managed by forwarding release and addref to the container. In this case the
* container must delete this volume when the ref count reaches 0.
***/ 
D3D9ProxyVolume::D3D9ProxyVolume(IDirect3DVolume9* pActualVolume, D3DProxyDevice* pOwningDevice, IUnknown* pWrappedContainer) :
	cBase( pActualVolume , pOwningDevice , pWrappedContainer )
{
}

/**
* Base SetPrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return actual->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Base GetPrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return actual->GetPrivateData(refguid, pData, pSizeOfData);
}

/**
* Base FreePrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::FreePrivateData(REFGUID refguid)
{
	return actual->FreePrivateData(refguid);
}




/**
* Base GetDesc functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::GetDesc(D3DVOLUME_DESC *pDesc)
{
	return actual->GetDesc(pDesc);
}

/**
* Base LockBox functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
{
	return actual->LockBox(pLockedVolume, pBox, Flags);
}

/**
* Base UnlockBox functionality.
***/
HRESULT WINAPI D3D9ProxyVolume::UnlockBox()
{
	return actual->UnlockBox();
}