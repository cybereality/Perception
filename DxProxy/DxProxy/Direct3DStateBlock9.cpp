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

#include "Direct3DStateBlock9.h"
#include <assert.h>

BaseDirect3DStateBlock9::BaseDirect3DStateBlock9(IDirect3DStateBlock9* pActualStateBlock, IDirect3DDevice9 *pOwningDevice) :
	m_pActualStateBlock(pActualStateBlock),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualStateBlock != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

BaseDirect3DStateBlock9::~BaseDirect3DStateBlock9()
{
	if(m_pActualStateBlock) 
		m_pActualStateBlock->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

HRESULT WINAPI BaseDirect3DStateBlock9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualStateBlock->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DStateBlock9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DStateBlock9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}


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

HRESULT WINAPI BaseDirect3DStateBlock9::Capture()
{
	return m_pActualStateBlock->Capture();
}

HRESULT WINAPI BaseDirect3DStateBlock9::Apply()
{
	return m_pActualStateBlock->Apply();
}