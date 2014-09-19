/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxySwapChain.cpp> and
Class <D3D9ProxySwapChain> :
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

#include "D3D9ProxySwapChain.h"
#include "StereoView.h"

/**
* Debug output helper.
***/
void releaseCheckO(char* object, int newRefCount)
{
	if (newRefCount > 0) {
		char buf[128];
		sprintf_s(buf, "Error: %s count = %d\n", object, newRefCount);
		OutputDebugStringA(buf);
	}
}

/**
* Constructor.
* Creates stereo backbuffers to use in place of actual backbuffers.
* @see m_backBuffers
***/
D3D9ProxySwapChain::D3D9ProxySwapChain(IDirect3DSwapChain9* pActualSwapChain, D3DProxyDevice* pWrappedOwningDevice, bool isAdditionalChain) : 
	cBase( pActualSwapChain , pWrappedOwningDevice ) ,
	m_bIsAdditionalChain(isAdditionalChain)
{
	assert (pActualSwapChain != NULL);

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
		pWrappedOwningDevice->ProxyCreateRenderTarget(backDesc.Width, backDesc.Height, backDesc.Format, backDesc.MultiSampleType, backDesc.MultiSampleQuality, false, &pTemp, NULL, 0 , true , 0 );
		m_backBuffers.push_back(static_cast<D3D9ProxySurface*>(pTemp));
	}
}

/**
* Destructor.
* Releases stored proxy backbuffers.
***/
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



/**
* Releases only additional swap chains.
* Primary swap chain isn't destroyed at ref count 0, stays alive. Other swap chains are destoyed if ref count reaches 0.
* BUT backbuffers aren't destroyed at 0 either AND this applies to primary and additional chains.
* So for additional chains we release the actual to destroy the actual. But we don't delete the proxy because it is
* our container for the stereo backbuffers.
* Because of this all swap chains have to be forcibly destroyed just before a reset.
***/
ULONG WINAPI D3D9ProxySwapChain::Release(){	
	if( refCount > 0 ) { 
		if( --refCount == 0 ){
			if( m_bIsAdditionalChain ){
				if( actual ){
					actual->Release();
					actual = 0;
				}
			}
		}
	}

	return refCount;
}


/**
* Here, the stereo view render method is called.
* @see StereoView::Draw()
***/
HRESULT WINAPI D3D9ProxySwapChain::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
#ifdef _DEBUG
	OutputDebugStringA(__FUNCTION__);
	OutputDebugStringA("\n");
#endif;

	// Test only, StereoView needs to be properly integrated as part of SwapChain.
	// This test allowed deus ex menus and videos to work correctly. Lots of model rendering issues in game though

	IDirect3DSurface9* pWrappedBackBuffer;

	try {
		GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pWrappedBackBuffer);

		if (device->stereoView->initialized)
			device->stereoView->Draw(static_cast<D3D9ProxySurface*>(pWrappedBackBuffer));

		pWrappedBackBuffer->Release();
	}
	catch (std::out_of_range) {
		OutputDebugStringA("Present: No primary swap chain found. (Present probably called before device has been reset)");
	}

	// call proxy device present update
	device->HandleUpdateExtern();

	return actual->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/**
* Gets the front buffer data from both (left/right) surface.
* TODO Might be able to use a frame delayed backbuffer (copy last back buffer?) to get proper 
* left/right images. Much pondering required, and some testing.
*	
***/
HRESULT WINAPI D3D9ProxySwapChain::GetFrontBufferData(IDirect3DSurface9* pDestSurface) 
{
	D3D9ProxySurface* pWrappedDestSurface = static_cast<D3D9ProxySurface*>(pDestSurface);

	HRESULT result;
	if (!pWrappedDestSurface) {
		result = actual->GetFrontBufferData(NULL);
	}
	else {
		result = actual->GetFrontBufferData(pWrappedDestSurface->actual);

		if (SUCCEEDED(result) && pWrappedDestSurface->right) {

			if (FAILED(actual->GetFrontBufferData(pWrappedDestSurface->right))) {
				OutputDebugStringA("SwapChain::GetFrontBufferData; right problem - left ok\n");
			}
		}
	}
	
	// TODO Might be able to use a frame delayed backbuffer (copy last back buffer?) to get proper left/right images. Much pondering required, and some testing
	OutputDebugStringA("SwapChain::GetFrontBufferData; Caution Will Robinson. The result of this method at the moment is wrapped surfaces containing what the user would see on a monitor. Example: A side-by-side warped image for the rift in the left and right surfaces of the output surface.\n");
	return result;
}

/**
* Provides the stored proxy back buffer.
***/
HRESULT WINAPI D3D9ProxySwapChain::GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) 
{
	if ((iBackBuffer < 0) || (iBackBuffer >= m_backBuffers.size())) 
		return D3DERR_INVALIDCALL;

	m_backBuffers[iBackBuffer]->AddRef();
	*ppBackBuffer = m_backBuffers[iBackBuffer];

	return D3D_OK;
}


/**
* Simple "delete this" function.
***/
void D3D9ProxySwapChain::Destroy() 
{
	delete this;
}



METHOD_THRU( HRESULT , WINAPI , D3D9ProxySwapChain , GetRasterStatus , D3DRASTER_STATUS* , pRasterStatus )
METHOD_THRU( HRESULT , WINAPI , D3D9ProxySwapChain , GetDisplayMode , D3DDISPLAYMODE* , pMode )
METHOD_THRU( HRESULT , WINAPI , D3D9ProxySwapChain , GetPresentParameters , D3DPRESENT_PARAMETERS* , pPresentationParameters )