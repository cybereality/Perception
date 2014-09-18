/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DIndexBuffer9.cpp> and
Class <D3D9ProxyIndexBuffer> :
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

#include "D3D9ProxyIndexBuffer.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualIndexBuffer Imbed actual index buffer. 
* @param pOwningDevice Pointer to the device that owns the buffer. 
***/
D3D9ProxyIndexBuffer::D3D9ProxyIndexBuffer(IDirect3DIndexBuffer9* pActualIndexBuffer, IDirect3DDevice9* pOwningDevice) :
	m_pActualIndexBuffer(pActualIndexBuffer),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualIndexBuffer != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

/**
* Destructor. 
* Releases embedded index buffer. 
***/
D3D9ProxyIndexBuffer::~D3D9ProxyIndexBuffer()
{
	if(m_pActualIndexBuffer) {
		m_pActualIndexBuffer->Release();
	}

	if (m_pOwningDevice) {
		m_pOwningDevice->Release();
	}
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualIndexBuffer->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI D3D9ProxyIndexBuffer::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI D3D9ProxyIndexBuffer::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}

/**
* Base GetDevice functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::GetDevice(IDirect3DDevice9** ppDevice)
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
* Base SetPrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualIndexBuffer->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Base GetPrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualIndexBuffer->GetPrivateData(refguid, pData, pSizeOfData);
}

/**
* Base FreePrivateData functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::FreePrivateData(REFGUID refguid)
{
	return m_pActualIndexBuffer->FreePrivateData(refguid);
}

/**
* Base SetPriority functionality.
***/
DWORD WINAPI D3D9ProxyIndexBuffer::SetPriority(DWORD PriorityNew)
{
	return m_pActualIndexBuffer->SetPriority(PriorityNew);
}

/**
* Base GetPriority functionality.
***/
DWORD WINAPI D3D9ProxyIndexBuffer::GetPriority()
{
	return m_pActualIndexBuffer->GetPriority();
}

/**
* Base PreLoad functionality.
***/
void WINAPI D3D9ProxyIndexBuffer::PreLoad()
{
	return m_pActualIndexBuffer->PreLoad();
}

/**
* Base GetType functionality.
***/
D3DRESOURCETYPE WINAPI D3D9ProxyIndexBuffer::GetType()
{
	return m_pActualIndexBuffer->GetType();
}

/**
* Base Lock functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags)
{
	return m_pActualIndexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}

/**
* Base Unlock functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::Unlock()
{
	return m_pActualIndexBuffer->Unlock();
}

/**
* Base GetDesc functionality.
***/
HRESULT WINAPI D3D9ProxyIndexBuffer::GetDesc(D3DINDEXBUFFER_DESC *pDesc)
{
	return m_pActualIndexBuffer->GetDesc(pDesc);
}

/**
* Returns the actual embedded buffer pointer.
***/
IDirect3DIndexBuffer9* D3D9ProxyIndexBuffer::getActual()
{
	return m_pActualIndexBuffer;
}
