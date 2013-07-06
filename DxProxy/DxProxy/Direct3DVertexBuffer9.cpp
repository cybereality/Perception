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

#include "Direct3DVertexBuffer9.h"
#include <assert.h>

BaseDirect3DVertexBuffer9::BaseDirect3DVertexBuffer9(IDirect3DVertexBuffer9* pActualVertexBuffer, IDirect3DDevice9* pOwningDevice) :
	m_pActualVertexBuffer(pActualVertexBuffer),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualVertexBuffer != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

BaseDirect3DVertexBuffer9::~BaseDirect3DVertexBuffer9()
{
	if(m_pActualVertexBuffer) {
		m_pActualVertexBuffer->Release();
	}

	if (m_pOwningDevice) {
		m_pOwningDevice->Release();
	}
}

HRESULT WINAPI BaseDirect3DVertexBuffer9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualVertexBuffer->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DVertexBuffer9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DVertexBuffer9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}




IDirect3DVertexBuffer9* BaseDirect3DVertexBuffer9::getActual()
{
	return m_pActualVertexBuffer;
}





HRESULT WINAPI BaseDirect3DVertexBuffer9::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef(); 
		return D3D_OK;
	}
}


HRESULT WINAPI BaseDirect3DVertexBuffer9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualVertexBuffer->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI BaseDirect3DVertexBuffer9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualVertexBuffer->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI BaseDirect3DVertexBuffer9::FreePrivateData(REFGUID refguid)
{
	return m_pActualVertexBuffer->FreePrivateData(refguid);
}


DWORD WINAPI BaseDirect3DVertexBuffer9::SetPriority(DWORD PriorityNew)
{
	return m_pActualVertexBuffer->SetPriority(PriorityNew);
}


DWORD WINAPI BaseDirect3DVertexBuffer9::GetPriority()
{
	return m_pActualVertexBuffer->GetPriority();
}


void WINAPI BaseDirect3DVertexBuffer9::PreLoad()
{
	return m_pActualVertexBuffer->PreLoad();
}


D3DRESOURCETYPE WINAPI BaseDirect3DVertexBuffer9::GetType()
{
	return m_pActualVertexBuffer->GetType();
}








HRESULT WINAPI BaseDirect3DVertexBuffer9::GetDesc(D3DVERTEXBUFFER_DESC *pDesc)
{
	return m_pActualVertexBuffer->GetDesc(pDesc);
}

HRESULT WINAPI BaseDirect3DVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags)
{
	return m_pActualVertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}

HRESULT WINAPI BaseDirect3DVertexBuffer9::Unlock()
{
	return m_pActualVertexBuffer->Unlock();
}


