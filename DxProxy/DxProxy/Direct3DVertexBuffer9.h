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

#ifndef DIRECT3DVERTEXBUFFER9_H_INCLUDED
#define DIRECT3DVERTEXBUFFER9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

class BaseDirect3DVertexBuffer9 : public IDirect3DVertexBuffer9
{
public:
	BaseDirect3DVertexBuffer9(IDirect3DVertexBuffer9* pActualVertexBuffer, IDirect3DDevice9* pOwningDevice);
	virtual ~BaseDirect3DVertexBuffer9();

	// IUnknown
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();


	// IDirect3DResource9
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual   DWORD WINAPI SetPriority(DWORD PriorityNew);
	virtual   DWORD WINAPI GetPriority();
	virtual    void WINAPI PreLoad();
	virtual D3DRESOURCETYPE WINAPI GetType();



	// IDirect3DVertexBuffer9
	virtual HRESULT WINAPI GetDesc(D3DVERTEXBUFFER_DESC *pDesc);
	virtual HRESULT WINAPI Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags);
	virtual HRESULT WINAPI Unlock();

protected:

	IDirect3DVertexBuffer9* const m_pActualVertexBuffer;
	IDirect3DDevice9* m_pOwningDevice;
	ULONG m_nRefCount;
};

#endif
