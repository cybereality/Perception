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

#include <assert.h>
#include "StereoBackBuffer.h"

/**
* Constructor, creates parent D3D9ProxySurface.
***/
StereoBackBuffer::StereoBackBuffer(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice) :
	D3D9ProxySurface(pActualSurfaceLeft, pActualSurfaceRight, pOwningDevice, NULL)
{
}

/**
* Empty destructor.
***/
StereoBackBuffer::~StereoBackBuffer()
{
}

/**
* Back buffers don't appear to be destroyed when the refcount reaches 0. 
* So the wrapper must be forcibly destroyed by the swap chain just before reset.
* See SwapChain::Destroy() and ProxySwapChain destructor.
***/
ULONG WINAPI StereoBackBuffer::Release()
{	
	if (m_nRefCount > 0) { 
		--m_nRefCount;
	}

	return m_nRefCount;
}