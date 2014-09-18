/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <StereoBackBuffer.h> and
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

#pragma once
#include "D3D9ProxySurface.h"


/**
* Proxy surface class with own Release() function.
*
* If the Proxy surface is in a container it will have a combined ref count with it's container
* and that count is managed by forwarding release and addref to the container. In this case the
* container must delete this surface when the ref count reaches 0.
*/
class StereoBackBuffer : public D3D9ProxySurface{
public:
	StereoBackBuffer(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, D3DProxyDevice* pOwningDevice);
	~StereoBackBuffer();
	
	/*** StereoBackBuffer public methods ***/
	ULONG WINAPI Release();
};
