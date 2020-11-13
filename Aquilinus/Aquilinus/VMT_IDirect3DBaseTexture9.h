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
#ifndef VMT_IDIRECT3DBASETEXTURE9_ENUM
#define VMT_IDIRECT3DBASETEXTURE9_ENUM

#include "VMT_IDirect3DResource9.h"

namespace VMT_IDIRECT3DBASETEXTURE9
{

	/**
	* Virtual Methods Table for IDirect3DBaseTexture9.
	***/
	enum VMT_IDirect3DBaseTexture9
	{
		/*** IDirect3DBaseTexture9 methods ***/
		SetLOD               = 11,
		GetLOD               = 12,
		GetLevelCount        = 13,
		SetAutoGenFilterType = 14,
		GetAutoGenFilterType = 15,
		GenerateMipSubLevels = 16,

		/*** IDirect3DResource9 methods ***/
		/*
		STDMETHOD_(DWORD, SetLOD)(THIS_ DWORD LODNew) PURE;
		STDMETHOD_(DWORD, GetLOD)(THIS) PURE;
		STDMETHOD_(DWORD, GetLevelCount)(THIS) PURE;
		STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
		STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
		STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;*/
	};
}

#endif