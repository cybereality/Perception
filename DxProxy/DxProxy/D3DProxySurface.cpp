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

#include "D3DProxySurface.h"


D3DProxySurface::D3DProxySurface(IDirect3DSurface9* pSurfaceBeingWrapped, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer) :
	BaseDirect3DSurface9(pSurfaceBeingWrapped),
	m_pOwningDevice(pOwningDevice),
	m_pWrappedContainer(pWrappedContainer)
{
	// We're keeping this reference to the device in this class so AddRef
	// This is how Direct3D behaves normally.
	if (m_pOwningDevice)
		m_pOwningDevice->AddRef();

	if (m_pWrappedContainer) {
		m_pWrappedContainer->AddRef();
	}
}

D3DProxySurface::~D3DProxySurface()
{

	if (m_pOwningDevice) {
		m_pOwningDevice->Release();
		m_pOwningDevice = NULL;
	}

	if (m_pWrappedContainer) {
		m_pOwningDevice->Release(); //TODO make sure this is correct. containers are trickey
		m_pOwningDevice = NULL;
	}
}







/*
	GetDevice on the underlying IDirect3DSurface9 will return the device used to create it. Which is the actual
	device and not the wrapper. Therefore we have to keep track of the wrapper device and return that instead.
*/
HRESULT WINAPI D3DProxySurface::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}








// "Provides access to the parent cube texture or texture (mipmap) object, if this surface is a child level of a cube texture or a mipmap. This method can 

/*  "If the surface is created using CreateRenderTarget or CreateOffscreenPlainSurface or 
	   CreateDepthStencilSurface, the surface is considered stand alone. In this case, GetContainer 
	   will return the Direct3D device used to create the surface."
	   http://msdn.microsoft.com/en-us/library/windows/desktop/bb205893%28v=vs.85%29.aspx */
HRESULT WINAPI D3DProxySurface::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	if (!m_pWrappedContainer) {
		m_pOwningDevice->AddRef();
		*ppContainer = m_pOwningDevice;
		return D3D_OK; //can't figure out if this should be an OK or an Invalid call. Haven't come up with a way to test.
	}					// Probably OK beause a pointer is returned and that appears to be the deffinition of success here.

	void *pContainer = NULL;
	HRESULT queryResult = m_pWrappedContainer->QueryInterface(riid, &pContainer);

	if (queryResult == S_OK) {
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

	// Like GetDevice we need to return the wrapper rather than the underlying container 
	//return m_pActualSurface->GetContainer(riid, ppContainer);
}
