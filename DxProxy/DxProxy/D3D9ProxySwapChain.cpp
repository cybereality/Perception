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

#include "D3D9ProxySwapChain.h"
#include <assert.h>

D3D9ProxySwapChain::D3D9ProxySwapChain(IDirect3DSwapChain9* pActualSwapChain, D3DProxyDevice* pWrappedOwningDevice, bool isAdditionalChain) : 
		BaseDirect3DSwapChain9(pActualSwapChain, pWrappedOwningDevice, isAdditionalChain),
		m_backBuffers()
{
	// Get creation parameters for backbuffers.
	D3DPRESENT_PARAMETERS params;
	pActualSwapChain->GetPresentParameters(&params);

	UINT bbCount = params.BackBufferCount;
	if (bbCount == 0)
		bbCount = 1;
	m_backBuffers.reserve(bbCount);

	IDirect3DSurface9* pTempActualBackBuffer;
	pActualSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pTempActualBackBuffer);
		
	D3DSURFACE_DESC backDesc;
	pTempActualBackBuffer->GetDesc(&backDesc);
	pTempActualBackBuffer->Release();
	pTempActualBackBuffer = NULL;

	// Create stereo backbuffers to use in place of actual backbuffers
	for (UINT i = 0; i < bbCount; i++) {
		IDirect3DSurface9* pTemp;
		pWrappedOwningDevice->CreateRenderTarget(backDesc.Width, backDesc.Height, backDesc.Format, backDesc.MultiSampleType, backDesc.MultiSampleQuality, false, &pTemp, NULL, true);
		m_backBuffers.push_back(static_cast<D3D9ProxySurface*>(pTemp));
	}
}

void releaseCheckO(char* object, int newRefCount)
{
	if (newRefCount > 0) {
		char buf[128];
		sprintf_s(buf, "Error: %s count = %d\n", object, newRefCount);
		OutputDebugString(buf);
	}
}

D3D9ProxySwapChain::~D3D9ProxySwapChain()
{
	auto it = m_backBuffers.begin();
	while (it != m_backBuffers.end()) {
		if (*it) {
			releaseCheckO("back buffer count (swapchain wrapper destruction)", (*it)->Release());

			delete (*it);
		}

		++it;
		//it = m_backBuffers.erase(it); // NS2 crashes here
	}

	m_backBuffers.clear();
}



HRESULT WINAPI D3D9ProxySwapChain::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	// Test only, StereoView needs to be properly integrated as part of SwapChain.
	// This test allowed deus ex menus and videos to work correctly. Lots of model rendering issues in game though
	D3DProxyDevice* pD3DProxyDev = static_cast<D3DProxyDevice*>(m_pOwningDevice);

	IDirect3DSurface9* pWrappedBackBuffer;

	try {
		GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pWrappedBackBuffer);

		if (pD3DProxyDev->stereoView->initialized)
			pD3DProxyDev->stereoView->Draw(static_cast<D3D9ProxySurface*>(pWrappedBackBuffer));

		pWrappedBackBuffer->Release();
	}
	catch (std::out_of_range) {
		OutputDebugString("Present: No primary swap chain found. (Present probably called before device has been reset)");
	}


	return m_pActualSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

HRESULT WINAPI D3D9ProxySwapChain::GetFrontBufferData(IDirect3DSurface9* pDestSurface) 
{
	D3D9ProxySurface* pWrappedDestSurface = static_cast<D3D9ProxySurface*>(pDestSurface);

	HRESULT result;
	if (!pWrappedDestSurface) {
		result = m_pActualSwapChain->GetFrontBufferData(NULL);
	}
	else {
		result = m_pActualSwapChain->GetFrontBufferData(pWrappedDestSurface->getActualLeft());

		if (SUCCEEDED(result) && pWrappedDestSurface->getActualRight()) {

			if (FAILED(m_pActualSwapChain->GetFrontBufferData(pWrappedDestSurface->getActualRight()))) {
				OutputDebugString("SwapChain::GetFrontBufferData; right problem - left ok\n");
			}
		}
	}
	
	// TODO Might be able to use a frame delayed backbuffer (copy last back buffer?) to get proper left/right images. Much pondering required, and some testing
	OutputDebugString("SwapChain::GetFrontBufferData; Caution Will Robinson. The result of this method at the moment is wrapped surfaces containing what the user would see on a monitor. Example: A side-by-side warped image for the rift in the left and right surfaces of the output surface.\n");
	return result;
}

HRESULT WINAPI D3D9ProxySwapChain::GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) 
{
	if ((iBackBuffer < 0) || (iBackBuffer >= m_backBuffers.size())) 
		return D3DERR_INVALIDCALL;

	m_backBuffers[iBackBuffer]->AddRef();
	*ppBackBuffer = m_backBuffers[iBackBuffer];

	return D3D_OK;
}
