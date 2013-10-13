/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
  and Joshua Brown 

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

#ifndef STEREOBACKBUFFER_H_INCLUDED
#define STEREOBACKBUFFER_H_INCLUDED

#include <d3d9.h>
#include "D3D9ProxySurface.h"
#include "Direct3DDevice9.h"
#include "IStereoCapableWrapper.h"
#include <stdio.h>


/**
* Proxy surface class with own Release() function.
*
* If the Proxy surface is in a container it will have a combined ref count with it's container
* and that count is managed by forwarding release and addref to the container. In this case the
* container must delete this surface when the ref count reaches 0.
*/
class StereoBackBuffer : public D3D9ProxySurface
{
public:
	StereoBackBuffer(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice);
	virtual ~StereoBackBuffer();
	
	/*** StereoBackBuffer public methods ***/
	virtual ULONG WINAPI Release();
};
#endif
