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

#include "Direct3DSwapChain9.h"
#include <assert.h>

BaseDirect3DSwapChain9::BaseDirect3DSwapChain9(IDirect3DSwapChain9* pActualSwapChain, IDirect3DDevice9 *pOwningDevice) :
	m_pActualSwapChain(pActualSwapChain),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1)
{
	assert (pActualSwapChain != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

BaseDirect3DSwapChain9::~BaseDirect3DSwapChain9()
{
	if(m_pActualSwapChain) 
		m_pActualSwapChain->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

HRESULT WINAPI BaseDirect3DSwapChain9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualSwapChain->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DSwapChain9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DSwapChain9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}


HRESULT WINAPI BaseDirect3DSwapChain9::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef(); 
		return D3D_OK;
	}
}


HRESULT WINAPI BaseDirect3DSwapChain9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return m_pActualSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

HRESULT WINAPI BaseDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9* pDestSurface) 
{
	return m_pActualSwapChain->GetFrontBufferData(pDestSurface);
}

HRESULT WINAPI BaseDirect3DSwapChain9::GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) 
{
	return m_pActualSwapChain->GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
}

HRESULT WINAPI BaseDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return m_pActualSwapChain->GetRasterStatus(pRasterStatus);
}
 
HRESULT WINAPI BaseDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return m_pActualSwapChain->GetDisplayMode(pMode);
}

HRESULT WINAPI BaseDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return m_pActualSwapChain->GetPresentParameters(pPresentationParameters);
}