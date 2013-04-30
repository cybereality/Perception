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

#include "Direct3DSurface9.h"
#include "Direct3DDevice9.h"
#include "Main.h"
#include "D3DProxyDeviceFactory.h"

BaseDirect3DSurface9::BaseDirect3DSurface9(IDirect3DSurface9* pSurface) :
	m_pSurface(pSurface),
	m_nRefCount(1)
{
}

BaseDirect3DSurface9::~BaseDirect3DSurface9()
{
	if(m_pSurface)
		m_pSurface->Release();
}

HRESULT WINAPI BaseDirect3DSurface9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pSurface->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DSurface9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DSurface9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}


HRESULT WINAPI BaseDirect3DSurface9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pSurface->GetDevice(ppDevice);
}

HRESULT WINAPI BaseDirect3DSurface9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI BaseDirect3DSurface9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pSurface->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT WINAPI BaseDirect3DSurface9::FreePrivateData(REFGUID refguid)
{
	return m_pSurface->FreePrivateData(refguid);
}

DWORD WINAPI BaseDirect3DSurface9::SetPriority(DWORD PriorityNew)
{
	return m_pSurface->SetPriority(PriorityNew);
}

DWORD WINAPI BaseDirect3DSurface9::GetPriority()
{
	return m_pSurface->GetPriority();
}

void WINAPI BaseDirect3DSurface9::PreLoad()
{
	return m_pSurface->PreLoad();
}

HRESULT WINAPI BaseDirect3DSurface9::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	return m_pSurface->GetContainer(riid, ppContainer);
}

HRESULT WINAPI BaseDirect3DSurface9::GetDesc(D3DSURFACE_DESC *pDesc)
{
	return m_pSurface->GetDesc(pDesc);
}

HRESULT WINAPI BaseDirect3DSurface9::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pSurface->LockRect(pLockedRect, pRect, Flags);
}

HRESULT WINAPI BaseDirect3DSurface9::UnlockRect()
{
	return m_pSurface->UnlockRect();
}

HRESULT WINAPI BaseDirect3DSurface9::GetDC(HDC *phdc)
{
	return m_pSurface->GetDC(phdc);
}

HRESULT WINAPI BaseDirect3DSurface9::ReleaseDC(HDC hdc)
{
	return m_pSurface->ReleaseDC(hdc);
}

D3DRESOURCETYPE WINAPI BaseDirect3DSurface9::GetType()
{
	return m_pSurface->GetType();
}

