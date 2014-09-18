/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DVertexBuffer9.cpp> and
Class <D3D9ProxyVertexBuffer> :
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

#include "D3D9ProxyVertexBuffer.h"


/**
* Constructor. 
* @param pActualVertexBuffer Imbed actual vertex buffer. 
* @param pOwningDevice Pointer to the device that owns the buffer. 
***/
D3D9ProxyVertexBuffer::D3D9ProxyVertexBuffer(IDirect3DVertexBuffer9* pActualVertexBuffer, D3DProxyDevice* pOwningDevice) :
	cBase( pActualVertexBuffer , pOwningDevice )
{
}


HRESULT WINAPI D3D9ProxyVertexBuffer::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return actual->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI D3D9ProxyVertexBuffer::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return actual->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI D3D9ProxyVertexBuffer::FreePrivateData(REFGUID refguid)
{
	return actual->FreePrivateData(refguid);
}


DWORD WINAPI D3D9ProxyVertexBuffer::SetPriority(DWORD PriorityNew)
{
	return actual->SetPriority(PriorityNew);
}


DWORD WINAPI D3D9ProxyVertexBuffer::GetPriority()
{
	return actual->GetPriority();
}


void WINAPI D3D9ProxyVertexBuffer::PreLoad()
{
	return actual->PreLoad();
}

D3DRESOURCETYPE WINAPI D3D9ProxyVertexBuffer::GetType()
{
	return actual->GetType();
}


HRESULT WINAPI D3D9ProxyVertexBuffer::Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags)
{
	return actual->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}


HRESULT WINAPI D3D9ProxyVertexBuffer::Unlock()
{
	return actual->Unlock();
}


HRESULT WINAPI D3D9ProxyVertexBuffer::GetDesc(D3DVERTEXBUFFER_DESC *pDesc)
{
	return actual->GetDesc(pDesc);
}
