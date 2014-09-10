/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DSwapChain9.cpp> and
Class <BaseDirect3DSwapChain9> :
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

#include "Direct3DSwapChain9.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualSwapChain Imbed actual swap chain. 
* @param pOwningDevice Pointer to the device that owns the chain. 
* @param isAdditionalChain Bool to ensure only additional chains are destroyed on release.
***/
BaseDirect3DSwapChain9::BaseDirect3DSwapChain9(IDirect3DSwapChain9* pActualSwapChain, BaseDirect3DDevice9 *pOwningDevice, bool isAdditionalChain) :
	m_pActualSwapChain(pActualSwapChain),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1),
	m_bIsAdditionalChain(isAdditionalChain)
{
	assert (pActualSwapChain != NULL);
	assert (pOwningDevice != NULL);

	pOwningDevice->AddRef();
}

/**
* Destructor. 
* Releases embedded swap chain. 
***/
BaseDirect3DSwapChain9::~BaseDirect3DSwapChain9()
{
	if(m_pActualSwapChain) 
		m_pActualSwapChain->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DSwapChain9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualSwapChain->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DSwapChain9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Releases only additional swap chains.
* Primary swap chain isn't destroyed at ref count 0, stays alive. Other swap chains are destoyed if ref count reaches 0.
* BUT backbuffers aren't destroyed at 0 either AND this applies to primary and additional chains.
* So for additional chains we release the actual to destroy the actual. But we don't delete the proxy because it is
* our container for the stereo backbuffers.
* Because of this all swap chains have to be forcibly destroyed just before a reset.
***/
ULONG WINAPI BaseDirect3DSwapChain9::Release()
{	
	if (m_nRefCount > 0) { 
		if(--m_nRefCount == 0)
		{

			if (m_bIsAdditionalChain) {
				if(m_pActualSwapChain) 
					m_pActualSwapChain->Release();

				m_pActualSwapChain = NULL;
			}
		}
	}

	return m_nRefCount;
}

/**
* Base Present functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return m_pActualSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/**
* Base GetFrontBufferData functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9* pDestSurface) 
{
#ifdef _DEBUG
	OutputDebugString(__FUNCTION__);
	OutputDebugString("\n");
	OutputDebugString("GetSwapChain. Danger Will Robinson. Surface not wrapped.\n");
#endif
	return m_pActualSwapChain->GetFrontBufferData(pDestSurface);
}

/**
* Base functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) 
{
#ifdef _DEBUG
	OutputDebugString(__FUNCTION__);
	OutputDebugString("\n");
	OutputDebugString("GetSwapChain. Danger Will Robinson. Surface not wrapped.\n");
#endif
	return m_pActualSwapChain->GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
}

/**
* Base functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return m_pActualSwapChain->GetRasterStatus(pRasterStatus);
}

/**
* Base GetDisplayMode functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return m_pActualSwapChain->GetDisplayMode(pMode);
}

/**
* Base GetDevice functionality.
***/
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

/**
* Base GetPresentParameters functionality.
***/
HRESULT WINAPI BaseDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return m_pActualSwapChain->GetPresentParameters(pPresentationParameters);
}

/**
* Simple "delete this" function.
***/
void BaseDirect3DSwapChain9::Destroy() 
{
	delete this;
}