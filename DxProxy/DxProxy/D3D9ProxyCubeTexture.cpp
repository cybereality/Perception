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

#include "D3D9ProxyCubeTexture.h"
#include <assert.h>


D3D9ProxyCubeTexture::D3D9ProxyCubeTexture(IDirect3DCubeTexture9* pActualTextureLeft, IDirect3DCubeTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice) :
	BaseDirect3DCubeTexture9(pActualTextureLeft),
	m_pActualTextureRight(pActualTextureRight),
	m_wrappedSurfaceLevels(),
	m_pOwningDevice(pOwningDevice)
{
	assert (pOwningDevice != NULL);

	m_pOwningDevice->AddRef();
}

D3D9ProxyCubeTexture::~D3D9ProxyCubeTexture()
{
	// delete all surfaces in m_levels
	auto it = m_wrappedSurfaceLevels.begin();
	while (it != m_wrappedSurfaceLevels.end()) {
		// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
		// calling back to the container Release.
		delete it->second;
		it = m_wrappedSurfaceLevels.erase(it);
	}

	if (m_pActualTextureRight)
		m_pActualTextureRight->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}









bool D3D9ProxyCubeTexture::IsStereo() 
{
	return (m_pActualTextureRight != NULL);
}

IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualMono()
{
	return getActualLeft();
}

IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualLeft()
{
	return m_pActualTexture;
}

IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualRight()
{
	return m_pActualTextureRight;
}








HRESULT WINAPI D3D9ProxyCubeTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}

HRESULT WINAPI D3D9ProxyCubeTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	if (IsStereo())
		m_pActualTextureRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI D3D9ProxyCubeTexture::FreePrivateData(REFGUID refguid)
{
	if (IsStereo())
		m_pActualTextureRight->FreePrivateData(refguid);

	return m_pActualTexture->FreePrivateData(refguid);
}

DWORD WINAPI D3D9ProxyCubeTexture::SetPriority(DWORD PriorityNew)
{
	if (IsStereo())
		m_pActualTextureRight->SetPriority(PriorityNew);

	return m_pActualTexture->SetPriority(PriorityNew);
}

void WINAPI D3D9ProxyCubeTexture::PreLoad()
{
	if (IsStereo())
		m_pActualTextureRight->PreLoad();

	return m_pActualTexture->PreLoad();
}







DWORD WINAPI D3D9ProxyCubeTexture::SetLOD(DWORD LODNew)
{
	if (IsStereo())
		m_pActualTextureRight->SetLOD(LODNew);

	return m_pActualTexture->SetLOD(LODNew);
}

HRESULT WINAPI D3D9ProxyCubeTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	if (IsStereo())
		m_pActualTextureRight->SetAutoGenFilterType(FilterType);

	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI D3D9ProxyCubeTexture::GenerateMipSubLevels()
{
	if (IsStereo())
		m_pActualTextureRight->GenerateMipSubLevels();

	return m_pActualTexture->GenerateMipSubLevels();
}







HRESULT WINAPI D3D9ProxyCubeTexture::GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface)
{
	HRESULT finalResult;

	CubeSurfaceKey key = CubeSurfaceKey(FaceType,Level);
	// Have we already got a Proxy for this surface level?
	if (m_wrappedSurfaceLevels.count(key) == 1) { // yes

		*ppCubeMapSurface = m_wrappedSurfaceLevels[key];
		(*ppCubeMapSurface)->AddRef();

		finalResult = D3D_OK;
	}
	else {
		// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
		//  wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
		IDirect3DSurface9* pActualSurfaceLevelLeft = NULL;
		IDirect3DSurface9* pActualSurfaceLevelRight = NULL;

		HRESULT leftResult = m_pActualTexture->GetCubeMapSurface(FaceType, Level, &pActualSurfaceLevelLeft);

		if (IsStereo()) {
			HRESULT resultRight = m_pActualTextureRight->GetCubeMapSurface(FaceType, Level, &pActualSurfaceLevelRight);
			assert (leftResult == resultRight);
		}


		if (SUCCEEDED(leftResult)) {

			D3D9ProxySurface* pWrappedSurfaceLevel = new D3D9ProxySurface(pActualSurfaceLevelLeft, pActualSurfaceLevelRight, m_pOwningDevice, this);

			if(m_wrappedSurfaceLevels.insert(std::pair<CubeSurfaceKey, D3D9ProxySurface*>(key, pWrappedSurfaceLevel)).second) {
				// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
				*ppCubeMapSurface = pWrappedSurfaceLevel;
				(*ppCubeMapSurface)->AddRef();
				finalResult = D3D_OK;
			}
			else {
				// Failure to insert should not be possible. In this case we could still return the wrapped surface,
				// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
				// at some point. Better to fail fast.
				OutputDebugString(__FUNCTION__);
				OutputDebugString("\n");
				OutputDebugString("Unable to store surface level.\n");
				assert(false);

				finalResult = D3DERR_INVALIDCALL;
			}
		}
		else { 
			OutputDebugString(__FUNCTION__);
			OutputDebugString("\n");
			OutputDebugString("Error fetching actual surface level.\n");
			finalResult = leftResult;
		}
	}

	return finalResult;
}



HRESULT WINAPI D3D9ProxyCubeTexture::LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	if (IsStereo())
		m_pActualTextureRight->LockRect(FaceType, Level, pLockedRect, pRect, Flags);

	return m_pActualTexture->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI D3D9ProxyCubeTexture::UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level)
{
	if (IsStereo())
		m_pActualTextureRight->UnlockRect(FaceType, Level);

	return m_pActualTexture->UnlockRect(FaceType, Level);
}

HRESULT WINAPI D3D9ProxyCubeTexture::AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
{
	if (IsStereo())
		m_pActualTextureRight->AddDirtyRect(FaceType, pDirtyRect);

	return m_pActualTexture->AddDirtyRect(FaceType, pDirtyRect);
}




