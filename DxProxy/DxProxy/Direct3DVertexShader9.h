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

#ifndef DIRECT3DVERTEXSHADER9_H_INCLUDED
#define DIRECT3DVERTEXSHADER9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

/**
*  Direct 3D vertex shader class. 
*  Overwrites IDirect3DVertexShader9 and imbeds the actual vertex shader.
*/
class BaseDirect3DVertexShader9 : public IDirect3DVertexShader9
{
public:
	BaseDirect3DVertexShader9(IDirect3DVertexShader9* pActualVertexShader, IDirect3DDevice9* pOwningDevice);
	virtual ~BaseDirect3DVertexShader9();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();
	
	/*** IDirect3DVertexShader9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9 **ppDevice);
	virtual HRESULT WINAPI GetFunction(void *pDate, UINT *pSizeOfData);

	/*** BaseDirect3DVertexShader9 methods ***/
	IDirect3DVertexShader9* getActual();

protected:
	/**
	* The actual vertex shader embedded. 
	***/
	IDirect3DVertexShader9* const m_pActualVertexShader;
	/**
	* Pointer to the D3D device that owns the shader. 
	***/
	IDirect3DDevice9* m_pOwningDevice;
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
};

#endif
