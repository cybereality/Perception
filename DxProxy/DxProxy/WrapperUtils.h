/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#ifndef WRAPPERUTILS_H_INCLUDED
#define WRAPPERUTILS_H_INCLUDED

#include <d3dx9.h>
#include <assert.h>
#include "D3D9ProxyTexture.h"
#include "D3D9ProxyVolumeTexture.h"
#include "D3D9ProxyCubeTexture.h"


namespace wrapperUtils {

	/*
		Returns actualy textures from wrapped texture.
		pWrappedTexture - Input, wrapped texture
		ppActualLeftTexture, ppActualRightTexture - Output. Will be set to the actual texztures from pWrappedTexture. Left should never be NULL. Right maybe NULL if texture isn't stereo
	 */
    void UnWrapTexture(IDirect3DBaseTexture9* pWrappedTexture, IDirect3DBaseTexture9** ppActualLeftTexture, IDirect3DBaseTexture9** ppActualRightTexture);
};

#endif