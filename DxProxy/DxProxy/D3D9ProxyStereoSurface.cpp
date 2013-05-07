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
#include "D3D9ProxyStereoSurface.h"

D3D9ProxyStereoSurface::D3D9ProxyStereoSurface(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice, IUnknown* pStereoContainer) : 
	D3D9ProxySurface(pActualSurfaceLeft, pOwningDevice, pStereoContainer),
	m_pActualSurfaceRight(pActualSurfaceRight)
{
	
	if (pActualSurfaceRight){
		if (pStereoContainer != NULL) {
			// Stereo surface is in a container
			//TODO If Debug get container type and cast to appropriate stereo container to verify stereo
			// containers are being used with stereo surfaces
			//assert (pStereoContainer.IsStereo() != NULL);
		}
	}
}

D3D9ProxyStereoSurface::~D3D9ProxyStereoSurface()
{
	if (m_pActualSurfaceRight)
		m_pActualSurfaceRight->Release();
}


bool D3D9ProxyStereoSurface::IsStereo()
{
	return m_pActualSurfaceRight != NULL;
}



IDirect3DSurface9* D3D9ProxyStereoSurface::getMonoSurface()
{
	return getLeftSurface();
}

IDirect3DSurface9* D3D9ProxyStereoSurface::getLeftSurface()
{
	return m_pActualSurface;
}

IDirect3DSurface9* D3D9ProxyStereoSurface::getRightSurface()
{
	return m_pActualSurfaceRight;
}






HRESULT WINAPI D3D9ProxyStereoSurface::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	if (IsStereo())
		m_pActualSurfaceRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}



HRESULT WINAPI D3D9ProxyStereoSurface::FreePrivateData(REFGUID refguid)
{
	if (IsStereo())
		m_pActualSurfaceRight->FreePrivateData(refguid);

	return m_pActualSurface->FreePrivateData(refguid);
}

DWORD WINAPI D3D9ProxyStereoSurface::SetPriority(DWORD PriorityNew)
{
	if (IsStereo())
		m_pActualSurfaceRight->SetPriority(PriorityNew);

	return m_pActualSurface->SetPriority(PriorityNew);
}


void WINAPI D3D9ProxyStereoSurface::PreLoad()
{
	if (IsStereo())
		m_pActualSurfaceRight->PreLoad();

	return m_pActualSurface->PreLoad();
}


HRESULT WINAPI D3D9ProxyStereoSurface::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	if (IsStereo())
		m_pActualSurfaceRight->LockRect(pLockedRect, pRect, Flags);

	return BaseDirect3DSurface9::LockRect(pLockedRect, pRect, Flags);
}

HRESULT WINAPI D3D9ProxyStereoSurface::UnlockRect()
{
	if (IsStereo())
		m_pActualSurfaceRight->UnlockRect();

	return BaseDirect3DSurface9::UnlockRect();
}



HRESULT WINAPI D3D9ProxyStereoSurface::ReleaseDC(HDC hdc)
{
	if (IsStereo())
		m_pActualSurfaceRight->ReleaseDC(hdc);

	return BaseDirect3DSurface9::ReleaseDC(hdc);
}



