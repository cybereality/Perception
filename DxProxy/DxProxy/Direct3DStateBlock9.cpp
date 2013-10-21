/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DStateBlock9.cpp> and
Class <BaseDirect3DStateBlock9> :
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

#include "Direct3DStateBlock9.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualStateBlock Imbed actual state block. 
* @param pOwningDevice Pointer to the device that owns the block. 
***/
BaseDirect3DStateBlock9::BaseDirect3DStateBlock9(IDirect3DStateBlock9* pActualStateBlock, IDirect3DDevice9 *pOwningDevice) :
	m_pActualStateBlock(pActualStateBlock),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

/**
* Destructor. 
* Releases embedded pixel shader. 
***/
BaseDirect3DStateBlock9::~BaseDirect3DStateBlock9()
{
	if(m_pActualStateBlock) 
		m_pActualStateBlock->Release();

	m_pOwningDevice->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DStateBlock9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	if (!m_pActualStateBlock) {
		OutputDebugString("Proxy state block is missing actual state block.\n");
		return D3DERR_INVALIDCALL;
	}

	return m_pActualStateBlock->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DStateBlock9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DStateBlock9::Release()
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
HRESULT WINAPI BaseDirect3DStateBlock9::GetDevice(IDirect3DDevice9** ppDevice)
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
* Base Capture functionality.
***/
HRESULT WINAPI BaseDirect3DStateBlock9::Capture()
{
	if (!m_pActualStateBlock) {
		OutputDebugString("Proxy state block is missing actual state block.\n");
		return D3DERR_INVALIDCALL;
	}

	return m_pActualStateBlock->Capture();
}

/**
* Base Apply functionality.
***/
HRESULT WINAPI BaseDirect3DStateBlock9::Apply()
{
	if (!m_pActualStateBlock) {
		OutputDebugString("Proxy state block is missing actual state block.\n");
		return D3DERR_INVALIDCALL;
	}

	return m_pActualStateBlock->Apply();
}