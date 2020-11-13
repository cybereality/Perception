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
#ifndef VMT_IDIRECT3DSURFACE9_ENUM
#define VMT_IDIRECT3DSURFACE9_ENUM

#include "VMT_IDirect3DResource9.h"

namespace VMT_IDIRECT3DSURFACE9
{

	/**
	* Virtual Methods Table for IDirect3DSurface9.
	***/
	enum VMT_IDirect3DSurface9
	{
		/*** IDirect3DSurface9 methods ***/
		LockRect   = 11,
		UnlockRect = 12,
		GetDC      = 13,
		ReleaseDC  = 14,

		/*STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
		STDMETHOD(UnlockRect)(THIS) PURE;
		STDMETHOD(GetDC)(THIS_ HDC *phdc) PURE;
		STDMETHOD(ReleaseDC)(THIS_ HDC hdc) PURE;*/

	};
}

#endif