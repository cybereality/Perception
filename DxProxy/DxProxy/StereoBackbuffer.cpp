/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <StereoBackBuffer.cpp> and
Class <StereoBackBuffer> :
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

#include <assert.h>
#include "StereoBackBuffer.h"

/**
* Constructor, creates parent D3D9ProxySurface.
***/
StereoBackBuffer::StereoBackBuffer(UINT Width, UINT Height, D3DFORMAT Format, bool lockable, IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice, HANDLE SharedHandleLeft, HANDLE SharedHandleRight) :
	D3D9ProxySurface(Width, Height, Format, lockable, pActualSurfaceLeft, pActualSurfaceRight, pOwningDevice, NULL, SharedHandleLeft, SharedHandleRight)
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