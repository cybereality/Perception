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

#ifndef DIRECT3DPIXELSHADER9_H_INCLUDED
#define DIRECT3DPIXELSHADER9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

class BaseDirect3DPixelShader9 : public IDirect3DPixelShader9
{
public:
	BaseDirect3DPixelShader9(IDirect3DPixelShader9* pActualPixelShader, IDirect3DDevice9* pOwningDevice);
	virtual ~BaseDirect3DPixelShader9();

	// IUnknown
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();

	// IDirect3DPixelShader9
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9 **ppDevice);
	virtual HRESULT WINAPI GetFunction(void *pDate, UINT *pSizeOfData);

protected:

	IDirect3DPixelShader9* const m_pActualPixelShader;
	IDirect3DDevice9* m_pOwningDevice;
	ULONG m_nRefCount;
};

#endif
