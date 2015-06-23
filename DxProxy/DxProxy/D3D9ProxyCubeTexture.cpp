/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyCubeTexture.cpp> and
Class <D3D9ProxyCubeTexture> :
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

#include "D3D9ProxyCubeTexture.h"
#include <assert.h>

/**
* Constructor.
* @see D3D9ProxySurface::D3D9ProxySurface
***/
D3D9ProxyCubeTexture::D3D9ProxyCubeTexture(IDirect3DCubeTexture9* pActualTextureLeft, IDirect3DCubeTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice) :
	BaseDirect3DCubeTexture9(pActualTextureLeft),
	m_pActualTextureRight(pActualTextureRight),
	m_wrappedSurfaceLevels(),
	m_pOwningDevice(pOwningDevice),
	lockableSysMemTexture(NULL)
{
	SHOW_CALL("D3D9ProxyCubeTexture()");
	assert (pOwningDevice != NULL);

	m_pOwningDevice->AddRef();
}

/**
* Destructor.
* Deletes wrapped surface levels, releases texures.
***/
D3D9ProxyCubeTexture::~D3D9ProxyCubeTexture()
{
	SHOW_CALL("~D3D9ProxyCubeTexture()");
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

/**
* GetDevice on the underlying IDirect3DCubeTexture9 will return the device used to create it. 
* Which is the actual device and not the wrapper. Therefore we have to keep track of the 
* wrapper device and return that instead.
* 
* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
* memory leak.
*/
HRESULT WINAPI D3D9ProxyCubeTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	SHOW_CALL("D3D9ProxyCubeTexture::GetDevice()");
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}

/**
* Sets private data on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	SHOW_CALL("D3D9ProxyCubeTexture::SetPrivateData");
	if (IsStereo())
		m_pActualTextureRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Frees private data on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::FreePrivateData(REFGUID refguid)
{
	SHOW_CALL("D3D9ProxyCubeTexture::FreePrivateData");
	if (IsStereo())
		m_pActualTextureRight->FreePrivateData(refguid);

	return m_pActualTexture->FreePrivateData(refguid);
}

/**
* Sets priority on both (left/right) textures.
***/
DWORD WINAPI D3D9ProxyCubeTexture::SetPriority(DWORD PriorityNew)
{
	SHOW_CALL("D3D9ProxyCubeTexture::SetPriority");
	if (IsStereo())
		m_pActualTextureRight->SetPriority(PriorityNew);

	return m_pActualTexture->SetPriority(PriorityNew);
}

/**
* Calls method on both (left/right) textures.
***/
void WINAPI D3D9ProxyCubeTexture::PreLoad()
{
	SHOW_CALL("D3D9ProxyCubeTexture::PreLoad");
	if (IsStereo())
		m_pActualTextureRight->PreLoad();

	return m_pActualTexture->PreLoad();
}

/**
* Sets LOD on both (left/right) texture.
***/
DWORD WINAPI D3D9ProxyCubeTexture::SetLOD(DWORD LODNew)
{
	SHOW_CALL("D3D9ProxyCubeTexture::SetLOD");
	if (IsStereo())
		m_pActualTextureRight->SetLOD(LODNew);

	return m_pActualTexture->SetLOD(LODNew);
}

/**
* Sets filter type on both (left/right) texture.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	SHOW_CALL("D3D9ProxyCubeTexture::SetAutoGenFilterType");
	if (IsStereo())
		m_pActualTextureRight->SetAutoGenFilterType(FilterType);

	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

/**
* Generates sub levels on both (left/right) texture.
***/
void WINAPI D3D9ProxyCubeTexture::GenerateMipSubLevels()
{
	SHOW_CALL("D3D9ProxyCubeTexture::GenerateMipSubLevels");
	if (IsStereo())
		m_pActualTextureRight->GenerateMipSubLevels();

	return m_pActualTexture->GenerateMipSubLevels();
}

/**
* If proxy surface is already stored on this level, return this one, otherwise create it.
* To create a new stored surface level, call the method on both (left/right) actual textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface)
{
	SHOW_CALL("D3D9ProxyCubeTexture::GetCubeMapSurface");
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

			D3D9ProxySurface* pWrappedSurfaceLevel = new D3D9ProxySurface(pActualSurfaceLevelLeft, pActualSurfaceLevelRight, m_pOwningDevice, this, NULL, NULL);

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

/**
* Locks rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	SHOW_CALL("D3D9ProxyCubeTexture::LockRect");

	D3DSURFACE_DESC desc;
	m_pActualTexture->GetLevelDesc(Level, &desc);
	if (desc.Pool != D3DPOOL_DEFAULT)
	{
		//Can't really handle stereo for this, so just lock on the original texture
		return m_pActualTexture->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
	}

	//Guard against multithreaded access as this could be causing us problems
	std::lock_guard<std::mutex> lck (m_mtx);

	UINT key = (10 * Level) + (UINT)(FaceType);
	if (newSurface.find(key) == newSurface.end())
	{
		newSurface[key] = true;
	}

	HRESULT hr = D3DERR_INVALIDCALL;
	if (!lockableSysMemTexture)
	{
		hr = m_pOwningDevice->getActual()->CreateCubeTexture(desc.Width, 
			m_pActualTexture->GetLevelCount(), 0, 
			desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemTexture, NULL);
		if (FAILED(hr))
			return hr;
	}

	if (newSurface[key])
	{
		IDirect3DSurface9 *pActualSurface = NULL;
		hr = m_pActualTexture->GetCubeMapSurface(FaceType, Level, &pActualSurface);
		if (FAILED(hr))
			return hr;
		IDirect3DSurface9 *pSurface = NULL;
		hr = lockableSysMemTexture->GetCubeMapSurface(FaceType, Level, &pSurface);
		if (FAILED(hr))
			return hr;

		hr = D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, pActualSurface, NULL, NULL, D3DX_DEFAULT, 0);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			vireio::debugf("Failed: D3DXLoadSurfaceFromSurface hr = 0x%0.8x", hr);
#endif
		}

		pSurface->Release();
		pActualSurface->Release();
	}

	if (((Flags|D3DLOCK_NO_DIRTY_UPDATE) != D3DLOCK_NO_DIRTY_UPDATE) &&
		((Flags|D3DLOCK_READONLY) != D3DLOCK_READONLY))
		hr = m_pActualTexture->AddDirtyRect(FaceType, pRect);

	hr = lockableSysMemTexture->LockRect(FaceType, Level, pLockedRect, pRect, Flags);

	return hr;
}

/**
* Unlocks rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level)
{
	SHOW_CALL("D3D9ProxyCubeTexture::UnlockRect");

	D3DSURFACE_DESC desc;
	m_pActualTexture->GetLevelDesc(Level, &desc);
	if (desc.Pool != D3DPOOL_DEFAULT)
	{
		return m_pActualTexture->UnlockRect(FaceType, Level);
	}

	//Guard against multithreaded access as this could be causing us problems
	std::lock_guard<std::mutex> lck (m_mtx);

	if (newSurface.size() == 0)
		return S_OK;

	HRESULT hr = lockableSysMemTexture->UnlockRect(FaceType, Level);
	if (FAILED(hr))
		return hr;

	if (IsStereo())
	{
		hr = m_pOwningDevice->getActual()->UpdateTexture(lockableSysMemTexture, m_pActualTextureRight);
		if (FAILED(hr))
			return hr;
	}

	hr = m_pOwningDevice->getActual()->UpdateTexture(lockableSysMemTexture, m_pActualTexture);
	if (FAILED(hr))
		return hr;
	
	return hr;
}

/**
* Adds dirty rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyCubeTexture::AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
{
	SHOW_CALL("D3D9ProxyCubeTexture::AddDirtyRect");
	if (IsStereo())
		m_pActualTextureRight->AddDirtyRect(FaceType, pDirtyRect);

	return m_pActualTexture->AddDirtyRect(FaceType, pDirtyRect);
}

/**
* Returns the left texture.
***/
IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualMono()
{
	return getActualLeft();
}

/**
* Returns the left texture.
***/
IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualLeft()
{
	return m_pActualTexture;
}

/**
* Returns the right texture.
***/
IDirect3DCubeTexture9* D3D9ProxyCubeTexture::getActualRight()
{
	return m_pActualTextureRight;
}

/**
* True if right texture present.
***/
bool D3D9ProxyCubeTexture::IsStereo() 
{
	return (m_pActualTextureRight != NULL);
}