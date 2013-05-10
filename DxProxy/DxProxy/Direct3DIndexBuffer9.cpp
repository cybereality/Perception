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

#include "Direct3DIndexBuffer9.h"
#include <assert.h>

BaseDirect3DIndexBuffer9::BaseDirect3DIndexBuffer9(IDirect3DIndexBuffer9* pActualIndexBuffer, IDirect3DDevice9* pOwningDevice) :
	m_pActualIndexBuffer(pActualIndexBuffer),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualIndexBuffer != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

BaseDirect3DIndexBuffer9::~BaseDirect3DIndexBuffer9()
{
	if(m_pActualIndexBuffer) {
		m_pActualIndexBuffer->Release();
	}

	if (m_pOwningDevice) {
		m_pOwningDevice->Release();
	}
}

HRESULT WINAPI BaseDirect3DIndexBuffer9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualIndexBuffer->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DIndexBuffer9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DIndexBuffer9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}










HRESULT WINAPI BaseDirect3DIndexBuffer9::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef(); 
		return D3D_OK;
	}
}


HRESULT WINAPI BaseDirect3DIndexBuffer9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualIndexBuffer->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI BaseDirect3DIndexBuffer9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualIndexBuffer->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI BaseDirect3DIndexBuffer9::FreePrivateData(REFGUID refguid)
{
	return m_pActualIndexBuffer->FreePrivateData(refguid);
}


DWORD WINAPI BaseDirect3DIndexBuffer9::SetPriority(DWORD PriorityNew)
{
	return m_pActualIndexBuffer->SetPriority(PriorityNew);
}


DWORD WINAPI BaseDirect3DIndexBuffer9::GetPriority()
{
	return m_pActualIndexBuffer->GetPriority();
}


void WINAPI BaseDirect3DIndexBuffer9::PreLoad()
{
	return m_pActualIndexBuffer->PreLoad();
}


D3DRESOURCETYPE WINAPI BaseDirect3DIndexBuffer9::GetType()
{
	return m_pActualIndexBuffer->GetType();
}








HRESULT WINAPI BaseDirect3DIndexBuffer9::GetDesc(D3DINDEXBUFFER_DESC *pDesc)
{
	return m_pActualIndexBuffer->GetDesc(pDesc);
}

HRESULT WINAPI BaseDirect3DIndexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags)
{
	return m_pActualIndexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}

HRESULT WINAPI BaseDirect3DIndexBuffer9::Unlock()
{
	return m_pActualIndexBuffer->Unlock();
}


