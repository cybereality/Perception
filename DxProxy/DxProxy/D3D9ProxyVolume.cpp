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

#include <assert.h>
#include "D3D9ProxyVolume.h"



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

D3D9ProxyVolume::~D3D9ProxyVolume()
{
	if (!m_pWrappedContainer) { 
		m_pOwningDevice->Release();
	}

	// else - m_pWrappedContainer does not have released called on it because the container manages
	// the device reference
}




IDirect3DVolume9* D3D9ProxyVolume::getActualVolume()
{
	return m_pActualVolume;
}





ULONG WINAPI D3D9ProxyVolume::AddRef()
{
	// if surface is in a container increase count on container instead of the surface
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->AddRef();
	}
	else {
		// otherwise track references normally
		return BaseDirect3DVolume9::AddRef();
	}
}

ULONG WINAPI D3D9ProxyVolume::Release()
{
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->Release(); 
	}
	else {
		return BaseDirect3DVolume9::Release();
	}
}







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


/*
	GetDevice on the underlying IDirect3DVolume9 will return the device used to create it. Which is the actual
	device and not the wrapper. Therefore we have to keep track of the wrapper device and return that instead.

	Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
	Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a memory leak.
*/
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