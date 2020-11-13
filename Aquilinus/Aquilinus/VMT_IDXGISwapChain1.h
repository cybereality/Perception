/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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
#ifndef VMT_IDXGISWAPCHAIN1_ENUM
#define VMT_IDXGISWAPCHAIN1_ENUM

#include "VMT_IDXGISwapChain.h"

namespace VMT_IDXGISWAPCHAIN1
{

	/**
	* Virtual Methods Table for IDXGISwapChain1.
	***/
	enum VMT_IDXGISwapChain1
	{
		/*** IDXGISwapChain1 methods ***/
		GetDesc1                 = 18, 
		GetFullscreenDesc        = 19, 
		GetHwnd                  = 20, 
		GetCoreWindow            = 21, 
		Present1                 = 22, 
		IsTemporaryMonoSupported = 23,
		GetRestrictToOutput      = 24, 
		SetBackgroundColor       = 25, 
		GetBackgroundColor       = 26, 
		SetRotation              = 27, 
		GetRotation              = 28, 

		/*IDXGISwapChain1 : public IDXGISwapChain
		{
		public:
		virtual HRESULT STDMETHODCALLTYPE GetDesc1( 
		DXGI_SWAP_CHAIN_DESC1 *pDesc) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetFullscreenDesc( 
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pDesc) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetHwnd( 
		HWND *pHwnd) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetCoreWindow( 
		REFIID refiid,
		void **ppUnk) = 0;

		virtual HRESULT STDMETHODCALLTYPE Present1( 
		UINT SyncInterval,
		UINT PresentFlags,
		const DXGI_PRESENT_PARAMETERS *pPresentParameters) = 0;

		virtual BOOL STDMETHODCALLTYPE IsTemporaryMonoSupported( void) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetRestrictToOutput( 
		IDXGIOutput **ppRestrictToOutput) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetBackgroundColor( 
		const DXGI_RGBA *pColor) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetBackgroundColor( 
		DXGI_RGBA *pColor) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetRotation( 
		DXGI_MODE_ROTATION Rotation) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetRotation( 
		DXGI_MODE_ROTATION *pRotation) = 0;

		};*/

	};
}

#endif