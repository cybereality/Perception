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

#include "Direct3DSurface9.h"


BaseDirect3DSurface9::BaseDirect3DSurface9(IDirect3DSurface9* pActualSurface) :
	m_pActualSurface(pActualSurface),
	m_nRefCount(1)
{
}

BaseDirect3DSurface9::~BaseDirect3DSurface9()
{
	if(m_pActualSurface) {
		m_pActualSurface->Release();
		m_pActualSurface = NULL;
	}
}

HRESULT WINAPI BaseDirect3DSurface9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualSurface->QueryInterface(riid, ppv);
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
	return m_pActualSurface->GetDevice(ppDevice);
}


HRESULT WINAPI BaseDirect3DSurface9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI BaseDirect3DSurface9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualSurface->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI BaseDirect3DSurface9::FreePrivateData(REFGUID refguid)
{
	return m_pActualSurface->FreePrivateData(refguid);
}


DWORD WINAPI BaseDirect3DSurface9::SetPriority(DWORD PriorityNew)
{
	return m_pActualSurface->SetPriority(PriorityNew);
}


DWORD WINAPI BaseDirect3DSurface9::GetPriority()
{
	return m_pActualSurface->GetPriority();
}


void WINAPI BaseDirect3DSurface9::PreLoad()
{
	return m_pActualSurface->PreLoad();
}


D3DRESOURCETYPE WINAPI BaseDirect3DSurface9::GetType()
{
	return m_pActualSurface->GetType();
}







HRESULT WINAPI BaseDirect3DSurface9::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	return m_pActualSurface->GetContainer(riid, ppContainer);
}


HRESULT WINAPI BaseDirect3DSurface9::GetDesc(D3DSURFACE_DESC *pDesc)
{
	return m_pActualSurface->GetDesc(pDesc);
}

HRESULT WINAPI BaseDirect3DSurface9::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pActualSurface->LockRect(pLockedRect, pRect, Flags);
}

HRESULT WINAPI BaseDirect3DSurface9::UnlockRect()
{
	return m_pActualSurface->UnlockRect();
}

HRESULT WINAPI BaseDirect3DSurface9::GetDC(HDC *phdc)
{
	return m_pActualSurface->GetDC(phdc);
}

HRESULT WINAPI BaseDirect3DSurface9::ReleaseDC(HDC hdc)
{
	return m_pActualSurface->ReleaseDC(hdc);
}


