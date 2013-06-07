/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

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



#include "Vireio.h"


namespace vireio {
    
	// Get actual textures from the various wrapper texture types
	void UnWrapTexture(IDirect3DBaseTexture9* pWrappedTexture, IDirect3DBaseTexture9** ppActualLeftTexture, IDirect3DBaseTexture9** ppActualRightTexture)
	{
		if (!pWrappedTexture)
			assert (false);

		D3DRESOURCETYPE type = pWrappedTexture->GetType();

		*ppActualLeftTexture = NULL;
		*ppActualRightTexture = NULL;
	
		switch (type)
		{
			case D3DRTYPE_TEXTURE:
			{
				D3D9ProxyTexture* pDerivedTexture = static_cast<D3D9ProxyTexture*> (pWrappedTexture);
				*ppActualLeftTexture = pDerivedTexture->getActualLeft();
				*ppActualRightTexture = pDerivedTexture->getActualRight();

				break;
			}
			case D3DRTYPE_VOLUMETEXTURE:
			{
				D3D9ProxyVolumeTexture* pDerivedTexture = static_cast<D3D9ProxyVolumeTexture*> (pWrappedTexture);
				*ppActualLeftTexture = pDerivedTexture->getActual();
				break;
			}
			case D3DRTYPE_CUBETEXTURE:
			{
				D3D9ProxyCubeTexture* pDerivedTexture = static_cast<D3D9ProxyCubeTexture*> (pWrappedTexture);
				*ppActualLeftTexture = pDerivedTexture->getActualLeft();
				*ppActualRightTexture = pDerivedTexture->getActualRight();
				break;
			}

			default:
				OutputDebugString("Unhandled texture type in SetTexture\n");
				break;
		}

		if ((*ppActualLeftTexture) == NULL) {
			OutputDebugString("No left texture? Unpossible!\n");
			assert (false);
		}
	}
};