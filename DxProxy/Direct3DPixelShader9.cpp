/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DPixelShader9.cpp> and
Class <BaseDirect3DPixelShader9> :
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

#include "Direct3DPixelShader9.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualPixelShader Imbed actual pixel shader. 
* @param pOwningDevice Pointer to the device that owns the shader. 
***/
BaseDirect3DPixelShader9::BaseDirect3DPixelShader9(IDirect3DPixelShader9* pActualPixelShader, IDirect3DDevice9 *pOwningDevice) :
	m_pActualPixelShader(pActualPixelShader),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualPixelShader != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

/**
* Destructor. 
* Releases embedded pixel shader. 
***/
BaseDirect3DPixelShader9::~BaseDirect3DPixelShader9()
{
	if(m_pActualPixelShader) 
		m_pActualPixelShader->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DPixelShader9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualPixelShader->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DPixelShader9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DPixelShader9::Release()
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
* TODO D3D behaviour. Docs don't have the notice that is usually there about a refcount increase
***/
HRESULT WINAPI BaseDirect3DPixelShader9::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		//m_pOwningDevice->AddRef(); //TODO D3D behaviour. Docs don't have the notice that is usually there about a refcount increase
		return D3D_OK;
	}
}

/**
* Base GetFunction functionality.
***/
HRESULT WINAPI BaseDirect3DPixelShader9::GetFunction(void *pDate, UINT *pSizeOfData)
{
	return m_pActualPixelShader->GetFunction(pDate, pSizeOfData);
}

/**
* Returns the actual embedded shader pointer.
***/
IDirect3DPixelShader9* BaseDirect3DPixelShader9::getActual()
{
	return m_pActualPixelShader;
}