/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxySurface.cpp> and
Class <D3D9ProxySurface> :
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
#include "D3D9ProxySurface.h"

/**
* Constructor.
* If the Proxy surface is in a container it will have a combined ref count with it's container
* and that count is managed by forwarding release and addref to the container. In this case the
* container must delete this surface when the ref count reaches 0.
***/ 
D3D9ProxySurface::D3D9ProxySurface(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice, IUnknown* pWrappedContainer) :
	BaseDirect3DSurface9(pActualSurfaceLeft),
	m_pActualSurfaceRight(pActualSurfaceRight),
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
D3D9ProxySurface::~D3D9ProxySurface()
{
	if (!m_pWrappedContainer) { 
		m_pOwningDevice->Release();
	}

	if (m_pActualSurfaceRight)
		m_pActualSurfaceRight->Release();
}

/**
* Behaviour determined through observing D3D with various test cases.
*
* Creating a surface should only increase the device ref count iff the surface has no parent container.
* (The container adds one ref to the device for it and all its surfaces)

* If a surface has a container then adding references to the surface should increase the ref count on
* the container instead of the surface. The surface shares a total ref count with the container, when
* it reaches 0 the container and its surfaces are destroyed. This is handled by sending all Add/Release
* on to the container when there is one.
***/
ULONG WINAPI D3D9ProxySurface::AddRef()
{
	// if surface is in a container increase count on container instead of the surface
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->AddRef();
	}
	else {
		// otherwise track references normally
		return BaseDirect3DSurface9::AddRef();
	}
}

/**
* Releases wrapped container if present else the base surface.
***/
ULONG WINAPI D3D9ProxySurface::Release()
{
	if (m_pWrappedContainer) {
		return m_pWrappedContainer->Release(); 
	}
	else {
		return BaseDirect3DSurface9::Release();
	}
}

/**
* GetDevice on the underlying IDirect3DSurface9 will return the device used to create it. 
* Which is the actual device and not the wrapper. Therefore we have to keep track of the 
* wrapper device and return that instead.
* 
* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
* memory leak.
*/
HRESULT WINAPI D3D9ProxySurface::GetDevice(IDirect3DDevice9** ppDevice)
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
* Sets private data on both (left/right) surfaces.
***/
HRESULT WINAPI D3D9ProxySurface::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	if (IsStereo())
		m_pActualSurfaceRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Frees private data on both (left/right) surfaces.
***/
HRESULT WINAPI D3D9ProxySurface::FreePrivateData(REFGUID refguid)
{
	if (IsStereo())
		m_pActualSurfaceRight->FreePrivateData(refguid);

	return m_pActualSurface->FreePrivateData(refguid);
}

/**
* Sets priority on both (left/right) surfaces.
***/
DWORD WINAPI D3D9ProxySurface::SetPriority(DWORD PriorityNew)
{
	if (IsStereo())
		m_pActualSurfaceRight->SetPriority(PriorityNew);

	return m_pActualSurface->SetPriority(PriorityNew);
}

/**
* Calls method on both (left/right) surfaces.
***/
void WINAPI D3D9ProxySurface::PreLoad()
{
	if (IsStereo())
		m_pActualSurfaceRight->PreLoad();

	return m_pActualSurface->PreLoad();
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
***/
HRESULT WINAPI D3D9ProxySurface::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	if (!m_pWrappedContainer) {
		m_pOwningDevice->AddRef();
		*ppContainer = m_pOwningDevice;
		return D3D_OK;
	}					

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

/**
* Locks rectangle on both (left/right) surfaces.
***/
HRESULT WINAPI D3D9ProxySurface::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	if (IsStereo())
		m_pActualSurfaceRight->LockRect(pLockedRect, pRect, Flags);

	return BaseDirect3DSurface9::LockRect(pLockedRect, pRect, Flags);
}

/**
* Unlocks rectangle on both (left/right) surfaces.
***/
HRESULT WINAPI D3D9ProxySurface::UnlockRect()
{
	if (IsStereo())
		m_pActualSurfaceRight->UnlockRect();

	return BaseDirect3DSurface9::UnlockRect();
}

/**
* Releases device context on both (left/right) surfaces.
***/
HRESULT WINAPI D3D9ProxySurface::ReleaseDC(HDC hdc)
{
	if (IsStereo())
		m_pActualSurfaceRight->ReleaseDC(hdc);

	return BaseDirect3DSurface9::ReleaseDC(hdc);
}

/**
* Returns the left surface.
***/
IDirect3DSurface9* D3D9ProxySurface::getActualMono()
{
	return getActualLeft();
}

/**
* Returns the left surface.
***/
IDirect3DSurface9* D3D9ProxySurface::getActualLeft()
{
	return m_pActualSurface;
}

/**
* Returns the right surface.
***/
IDirect3DSurface9* D3D9ProxySurface::getActualRight()
{
	return m_pActualSurfaceRight;
}

/**
* True if right surface present.
***/
bool D3D9ProxySurface::IsStereo()
{
	return m_pActualSurfaceRight != NULL;
}