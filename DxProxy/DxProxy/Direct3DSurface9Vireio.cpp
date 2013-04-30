/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#include "Direct3DSurface9Vireio.h"
#include "Direct3DDevice9.h"
#include "Main.h"
#include "D3DProxyDeviceFactory.h"

Direct3DSurface9Vireio::Direct3DSurface9Vireio(IDirect3DSurface9* pLeftSurface, IDirect3DSurface9* pRightSurface) : BaseDirect3DSurface9(pLeftSurface)
{
	m_pRightSurface = pRightSurface;
}

Direct3DSurface9Vireio::~Direct3DSurface9Vireio()
{
	if(m_pRightSurface)
		m_pRightSurface->Release();
}


bool Direct3DSurface9Vireio::IsStereo()
{
	return m_pRightSurface != NULL;
}



IDirect3DSurface9* Direct3DSurface9Vireio::getMonoSurface()
{
	return getLeftSurface();
}

IDirect3DSurface9* Direct3DSurface9Vireio::getLeftSurface()
{
	return m_pSurface;
}

IDirect3DSurface9* Direct3DSurface9Vireio::getRightSurface()
{
	return m_pRightSurface;
}




HRESULT WINAPI Direct3DSurface9Vireio::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	if (IsStereo())
		m_pRightSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return BaseDirect3DSurface9::SetPrivateData(refguid, pData, SizeOfData, Flags);
}



HRESULT WINAPI Direct3DSurface9Vireio::FreePrivateData(REFGUID refguid)
{
	if (IsStereo())
		m_pRightSurface->FreePrivateData(refguid);

	return BaseDirect3DSurface9::FreePrivateData(refguid);
}

DWORD WINAPI Direct3DSurface9Vireio::SetPriority(DWORD PriorityNew)
{
	if (IsStereo())
		m_pRightSurface->SetPriority(PriorityNew);

	return BaseDirect3DSurface9::SetPriority(PriorityNew);
}


void WINAPI Direct3DSurface9Vireio::PreLoad()
{
	if (IsStereo())
		m_pRightSurface->PreLoad();

	return BaseDirect3DSurface9::PreLoad();
}


HRESULT WINAPI Direct3DSurface9Vireio::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	if (IsStereo())
		m_pRightSurface->LockRect(pLockedRect, pRect, Flags);

	return BaseDirect3DSurface9::LockRect(pLockedRect, pRect, Flags);
}

HRESULT WINAPI Direct3DSurface9Vireio::UnlockRect()
{
	if (IsStereo())
		m_pRightSurface->UnlockRect();

	return BaseDirect3DSurface9::UnlockRect();
}



HRESULT WINAPI Direct3DSurface9Vireio::ReleaseDC(HDC hdc)
{
	if (IsStereo())
		m_pRightSurface->ReleaseDC(hdc);

	return BaseDirect3DSurface9::ReleaseDC(hdc);
}



