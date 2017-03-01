/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <D3D9Ex_D3DX9_Methods.h>:
Copyright (C) 2017 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015
by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin and Neil Schneider

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

#ifndef D3D9EX_D3DX9_METHODS
#define D3D9EX_D3DX9_METHODS

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#pragma region D3DXFillTexture
/**
* D3DXFillTexture compatible with the D3D9Ex device.
***/
HRESULT D3D9Ex_D3DXFillTexture(LPDIRECT3DTEXTURE9 pTexture, LPD3DXFILL2D pFunction, LPVOID pData)
{
	if (!pTexture) return D3DERR_INVALIDCALL;

	// get device
	IDirect3DDevice9* pcDevice = nullptr;
	pTexture->GetDevice(&pcDevice);
	if (!pcDevice) return D3DERR_INVALIDCALL;

	// get surface desc
	D3DSURFACE_DESC sDesc = {};
	pTexture->GetLevelDesc(0, &sDesc);

	// create lockable texture.. do we need A32R32G32B32 here ?
	IDirect3DTexture9* pcTexLockable = nullptr;
	pcDevice->CreateTexture(sDesc.Width, sDesc.Height, 1, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pcTexLockable, NULL);
	if (!pcTexLockable) return D3DERR_INVALIDCALL;

	// get surface (level 0)
	IDirect3DSurface9 *pcSurface = NULL;
	pcTexLockable->GetSurfaceLevel(0, &pcSurface);

	if (pcSurface)
	{
		// lock the surface
		D3DLOCKED_RECT sRect = {};
		if (SUCCEEDED(pcSurface->LockRect(&sRect, NULL, NULL)))
		{
			// call fill method for each texel
			D3DXVECTOR4 sTexelOut = {};
			POINT sTexelCoords = {};
			D3DXVECTOR2 sTexCoord = {};
			D3DXVECTOR2 sTexelSize = {};
			sTexelSize.x = (1.0f / (float)sDesc.Width);
			sTexelSize.y = (1.0f / (float)sDesc.Height);
			for (sTexCoord.x = 0.0f; sTexCoord.x < 1.0f; sTexCoord.x += sTexelSize.x)
			{
				for (sTexCoord.y = 0.0f; sTexCoord.y < 1.0f; sTexCoord.y += sTexelSize.y)
				{
					// get texel index
					LONG nIx = sTexelCoords.x * sizeof(D3DCOLOR)+sTexelCoords.y *(LONG)sRect.Pitch;

					// call the method
					pFunction((D3DXVECTOR4*)&sTexelOut, (const D3DXVECTOR2*)&sTexCoord, (const D3DXVECTOR2*)&sTexelSize, pData);

					// convert to A8R8G8B8 format
					((BYTE*)sRect.pBits)[nIx] = (BYTE)(sTexelOut.z * 255.0f);
					((BYTE*)sRect.pBits)[nIx + 1] = (BYTE)(sTexelOut.y * 255.0f);
					((BYTE*)sRect.pBits)[nIx + 2] = (BYTE)(sTexelOut.x * 255.0f);
					((BYTE*)sRect.pBits)[nIx + 3] = (BYTE)(sTexelOut.w * 255.0f);

					// increase the texel coords y
					sTexelCoords.y++;
					if (sTexelCoords.y == (LONG)sDesc.Width) sTexCoord.y = 1.0f;
				}

				// increase the texel coords x
				sTexelCoords.y = 0;
				sTexelCoords.x++;
				if (sTexelCoords.x == (LONG)sDesc.Height) sTexCoord.x = 1.0f;
			}

			pcSurface->UnlockRect();
		}

		// update the actual surface 0
		IDirect3DSurface9* pcSurfaceDest = nullptr;
		pTexture->GetSurfaceLevel(0, &pcSurfaceDest);
		if (pcSurfaceDest)
		{
			// update the level 0 surface
			D3DXLoadSurfaceFromSurface(pcSurfaceDest, NULL, NULL, pcSurface, NULL, NULL, 0, 0);
			pcSurfaceDest->Release();
		}

		pcSurface->Release();
	}
	else return D3DERR_INVALIDCALL;

	// generate mip sub levels
	pTexture->GenerateMipSubLevels();

	// release and return
	pcTexLockable->Release();
	pcDevice->Release();
	return S_OK;
}
#pragma endregion
#pragma region D3DXFillVolumeTexture
/**
* D3DXFillVolumeTexture compatible with the D3D9Ex device.
***/
HRESULT D3D9Ex_D3DXFillVolumeTexture(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXFILL3D pFunction, LPVOID pData)
{
	if (!pVolumeTexture) return D3DERR_INVALIDCALL;

	// get device
	IDirect3DDevice9* pcDevice = nullptr;
	pVolumeTexture->GetDevice(&pcDevice);
	if (!pcDevice) return D3DERR_INVALIDCALL;

	// get surface desc
	D3DVOLUME_DESC sDesc = {};
	pVolumeTexture->GetLevelDesc(0, &sDesc);

	// create lockable texture.. do we need A32R32G32B32 here ?
	IDirect3DVolumeTexture9* pcTexLockable = nullptr;
	pcDevice->CreateVolumeTexture(sDesc.Width, sDesc.Height, sDesc.Depth, 1, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pcTexLockable, NULL);
	if (!pcTexLockable) return D3DERR_INVALIDCALL;

	// get surface (level 0)
	IDirect3DVolume9 *pcVolume = NULL;
	pcTexLockable->GetVolumeLevel(0, &pcVolume);

	if (pcVolume)
	{
		// lock the surface
		D3DLOCKED_BOX sBox = {};
		if (SUCCEEDED(pcVolume->LockBox(&sBox, NULL, NULL)))
		{
			// call fill method for each texel
			D3DXVECTOR4 sTexelOut = {};
			struct VEC3 { LONG x, y, z; } sTexelCoords = {};
			D3DXVECTOR3 sTexCoord = {};
			D3DXVECTOR3 sTexelSize = {};
			sTexelSize.x = (1.0f / (float)sDesc.Width);
			sTexelSize.y = (1.0f / (float)sDesc.Height);
			sTexelSize.z = (1.0f / (float)sDesc.Depth);
			for (sTexCoord.x = 0.0f; sTexCoord.x < 1.0f; sTexCoord.x += sTexelSize.x)
			{
				for (sTexCoord.y = 0.0f; sTexCoord.y < 1.0f; sTexCoord.y += sTexelSize.y)
				{
					for (sTexCoord.z = 0.0f; sTexCoord.z < 1.0f; sTexCoord.z += sTexelSize.z)
					{
						// get texel index
						LONG nIx = sTexelCoords.x * sizeof(D3DCOLOR)+sTexelCoords.y *(LONG)sBox.RowPitch + sTexelCoords.z * (LONG)sBox.SlicePitch;

						// call the method
						pFunction((D3DXVECTOR4*)&sTexelOut, (const D3DXVECTOR3*)&sTexCoord, (const D3DXVECTOR3*)&sTexelSize, pData);

						// convert to A8R8G8B8 format
						((BYTE*)sBox.pBits)[nIx] = (BYTE)(sTexelOut.z * 255.0f);
						((BYTE*)sBox.pBits)[nIx + 1] = (BYTE)(sTexelOut.y * 255.0f);
						((BYTE*)sBox.pBits)[nIx + 2] = (BYTE)(sTexelOut.x * 255.0f);
						((BYTE*)sBox.pBits)[nIx + 3] = (BYTE)(sTexelOut.w * 255.0f);

						// increase the texel coords z
						sTexelCoords.z++;
						if (sTexelCoords.z == (LONG)sDesc.Width) sTexCoord.z = 1.0f;
					}

					// increase the texel coords y
					sTexelCoords.z = 0;
					sTexelCoords.y++;
					if (sTexelCoords.y == (LONG)sDesc.Width) sTexCoord.y = 1.0f;
				}

				// increase the texel coords x
				sTexelCoords.y = 0;
				sTexelCoords.x++;
				if (sTexelCoords.x == (LONG)sDesc.Height) sTexCoord.x = 1.0f;
			}

			pcVolume->UnlockBox();
		}

		// update the actual surface 0
		IDirect3DVolume9* pcVolumeDest = nullptr;
		pVolumeTexture->GetVolumeLevel(0, &pcVolumeDest);
		if (pcVolumeDest)
		{
			// update the level 0 volume
			D3DXLoadVolumeFromVolume(pcVolumeDest, NULL, NULL, pcVolume, NULL, NULL, 0, 0);
			pcVolumeDest->Release();
		}

		pcVolume->Release();
	}
	else return D3DERR_INVALIDCALL;

	// generate mip sub levels
	pVolumeTexture->GenerateMipSubLevels();

	// release and return
	pcTexLockable->Release();
	pcDevice->Release();
	return S_OK;
}
#pragma endregion

#endif