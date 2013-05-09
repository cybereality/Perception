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

#include "Direct3DVolume9.h"
#include <assert.h>


BaseDirect3DVolume9::BaseDirect3DVolume9(IDirect3DVolume9* pActualVolume) :
	m_pActualVolume(pActualVolume),
	m_nRefCount(1)
{
	assert (pActualVolume != NULL);
}

BaseDirect3DVolume9::~BaseDirect3DVolume9()
{
	if(m_pActualVolume) {
		m_pActualVolume->Release();
	}
}

HRESULT WINAPI BaseDirect3DVolume9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualVolume->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DVolume9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DVolume9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}






HRESULT WINAPI BaseDirect3DVolume9::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	return m_pActualVolume->GetContainer(riid, ppContainer);
}

HRESULT WINAPI BaseDirect3DVolume9::GetDesc(D3DVOLUME_DESC *pDesc)
{
	return m_pActualVolume->GetDesc(pDesc);
}

HRESULT WINAPI BaseDirect3DVolume9::LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
{
	return m_pActualVolume->LockBox(pLockedVolume, pBox, Flags);
}

HRESULT WINAPI BaseDirect3DVolume9::UnlockBox()
{
	return m_pActualVolume->UnlockBox();
}

HRESULT WINAPI BaseDirect3DVolume9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pActualVolume->GetDevice(ppDevice);
}

HRESULT WINAPI BaseDirect3DVolume9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualVolume->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI BaseDirect3DVolume9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualVolume->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI BaseDirect3DVolume9::FreePrivateData(REFGUID refguid)
{
	return m_pActualVolume->FreePrivateData(refguid);
}
