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

#include <assert.h>
#include "D3D9ProxyVolume.h"


/**
* Constructor.
* If the Proxy volume is in a container it will have a combined ref count with it's container
* and that count is managed by forwarding release and addref to the container. In this case the
* container must delete this volume when the ref count reaches 0.
***/ 
D3D9ProxyVolume::D3D9ProxyVolume(IDirect3DVolume9* pActualVolume, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer) :
	BaseDirect3DVolume9(pActualVolume),
	m_pOwningDevice(pOwningDevice),
	m_pWrappedContainer(pWrappedContainer)
{
	assert (pOwningDevice != NULL);


	if (!pWrappedContainer)
		pOwningDevice->AddRef();
	// else - We leave the device ref count changes to the container

	// pWrappedContainer->AddRef(); is not called here as container add/release is handled
	// by the container. The ref could be added here but as the release and destruction is
	// hanlded by the container we leave it all in the same place (the container)	
}

/**
* Destructor.
* (else - m_pWrappedContainer does not have released called on it because the container manages
* the device reference)
***/
D3D9ProxyVolume::~D3D9ProxyVolume()
{
	if (!m_pWrappedContainer) { 
		m_pOwningDevice->Release();
	}	
}

/**
* Behaviour determined through observing D3D with various test cases.
*
* Creating a volume should only increase the device ref count iff the volume has no parent container.
* (The container adds one ref to the device for it and all its volumes)

* If a volume has a container then adding references to the volume should increase the ref count on
* the container instead of the volume. The volume shares a total ref count with the container, when
* it reaches 0 the container and its volumes are destroyed. This is handled by sending all Add/Release
* on to the container when there is one.
***/
ULONG WINAPI D3D9ProxyVolume::AddRef()
{
	// if volume is in a container increase count on container instead of the volume
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->AddRef();
	}
	else {
		// otherwise track references normally
		return BaseDirect3DVolume9::AddRef();
	}
}

/**
* Releases wrapped container if present else the base surface.
***/
ULONG WINAPI D3D9ProxyVolume::Release()
{
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->Release(); 
	}
	else {
		return BaseDirect3DVolume9::Release();
	}
}

/**
* GetDevice on the underlying IDirect3DVolume9 will return the device used to create it. 
* Which is the actual device and not the wrapper. Therefore we have to keep track of the wrapper 
* device and return that instead.

* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
* memory leak.
***/
HRESULT WINAPI D3D9ProxyVolume::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}

/**
* Provides acces to parent object.
* "Provides access to the parent cube texture or texture (mipmap) object, if this surface is a child 
* level of a cube texture or a mipmap. This method can also provide access to the parent swap chain 
* if the surface is a back-buffer child."
*
* "If the surface is created using CreateRenderTarget or CreateOffscreenPlainSurface or 
* CreateDepthStencilSurface, the surface is considered stand alone. In this case, GetContainer 
* will return the Direct3D device used to create the surface."
* <http://msdn.microsoft.com/en-us/library/windows/desktop/bb205893%28v=vs.85%29.aspx>
*
* If the call succeeds, the reference count of the container is increased by one.
* @return Owning device if no wrapped container present, otherwise the container.
* @see D3D9ProxySurface::GetContainer()
***/
HRESULT WINAPI D3D9ProxyVolume::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	if (!m_pWrappedContainer) {
		m_pOwningDevice->AddRef();
		*ppContainer = m_pOwningDevice;
		return D3D_OK;
	}					


	void *pContainer = NULL;
	HRESULT queryResult = m_pWrappedContainer->QueryInterface(riid, &pContainer);

	if (SUCCEEDED(queryResult)) {
		*ppContainer = m_pWrappedContainer;
		m_pWrappedContainer->AddRef();

		return D3D_OK;
	} 
	else if (queryResult == E_NOINTERFACE) {

		return E_NOINTERFACE;
	}
	else {
		return D3DERR_INVALIDCALL;
	}
}

/**
* Gets the actual (parent) volume.
***/
IDirect3DVolume9* D3D9ProxyVolume::getActualVolume()
{
	return m_pActualVolume;
}