/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVolumeTexture.cpp> and
Class <D3D9ProxyVolumeTexture> :
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

#include "D3D9ProxyVolumeTexture.h"
#include <assert.h>

/**
* Constructor.
* @see D3D9ProxySurface::D3D9ProxySurface
***/
D3D9ProxyVolumeTexture::D3D9ProxyVolumeTexture(IDirect3DVolumeTexture9* pActualVolumeTexture, BaseDirect3DDevice9* pOwningDevice) :
	BaseDirect3DVolumeTexture9(pActualVolumeTexture),
	m_wrappedVolumeLevels(),
	m_pOwningDevice(pOwningDevice),
	lockableSysMemVolume(NULL)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::D3D9ProxyVolumeTexture");
	assert (pOwningDevice != NULL);

	m_pOwningDevice->AddRef();
}

/**
* Destructor.
* Deletes wrapped volumes.
***/
D3D9ProxyVolumeTexture::~D3D9ProxyVolumeTexture()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::~D3D9ProxyVolumeTexture");
	// delete all surfaces in m_levels
	auto it = m_wrappedVolumeLevels.begin();
	while (it != m_wrappedVolumeLevels.end()) {
		// we have to explicitly delete the Volume here as the Release behaviour of the volume would get stuck in a loop
		// calling back to the container Release.
		delete it->second;
		it = m_wrappedVolumeLevels.erase(it);
	}

	if (lockableSysMemVolume)
		lockableSysMemVolume->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

/**
* GetDevice on the underlying IDirect3DVolumeTexture9 will return the device used to create it. 
* Which is the actual device and not the wrapper. Therefore we have to keep track of the 
* wrapper device and return that instead.
* 
* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
* memory leak.
*/
HRESULT D3D9ProxyVolumeTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetDevice");
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}


/**
* If proxy volume is already stored on this level, return this one, otherwise create it.
* To create a new stored volume level, call the method on both (left/right) actual volumes.
***/
HRESULT D3D9ProxyVolumeTexture::GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetVolumeLevel");
	HRESULT finalResult;

	// Have we already got a Proxy for this surface level?
	if (m_wrappedVolumeLevels.count(Level) == 1) { // yes

		// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
		// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

		*ppVolumeLevel = m_wrappedVolumeLevels[Level];
		(*ppVolumeLevel)->AddRef();

		finalResult = D3D_OK;
	}
	else {
		// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
		//  wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
		IDirect3DVolume9* pActualVolumeLevel = NULL;

		HRESULT result = m_pActualTexture->GetVolumeLevel(Level, &pActualVolumeLevel);

		if (SUCCEEDED(result)) {

			D3D9ProxyVolume* pWrappedVolumeLevel = new D3D9ProxyVolume(pActualVolumeLevel, m_pOwningDevice, this);

			if(m_wrappedVolumeLevels.insert(std::pair<ULONG, D3D9ProxyVolume*>(Level, pWrappedVolumeLevel)).second) {
				// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
				*ppVolumeLevel = pWrappedVolumeLevel;
				(*ppVolumeLevel)->AddRef();
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
			finalResult = result;
		}
	}

	return finalResult;
}


HRESULT              D3D9ProxyVolumeTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::SetPrivateData");
	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT              D3D9ProxyVolumeTexture::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetPrivateData");
	return m_pActualTexture->GetPrivateData(refguid, pData, pSizeOfData);

}

HRESULT              D3D9ProxyVolumeTexture::FreePrivateData(REFGUID refguid)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::FreePrivateData");
	return m_pActualTexture->FreePrivateData(refguid);

}

DWORD                D3D9ProxyVolumeTexture::SetPriority(DWORD PriorityNew)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::SetPriority");
	return m_pActualTexture->SetPriority(PriorityNew);

}

DWORD                D3D9ProxyVolumeTexture::GetPriority()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetPriority");
	return m_pActualTexture->GetPriority();

}

void                 D3D9ProxyVolumeTexture::PreLoad()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::PreLoad");
	return m_pActualTexture->PreLoad();

}

D3DRESOURCETYPE      D3D9ProxyVolumeTexture::GetType()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetType");
	return m_pActualTexture->GetType();

}

DWORD                D3D9ProxyVolumeTexture::SetLOD(DWORD LODNew)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::SetLOD");
	return m_pActualTexture->SetLOD(LODNew);

}

DWORD                D3D9ProxyVolumeTexture::GetLOD()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetLOD");
	return m_pActualTexture->GetLOD();

}

DWORD                D3D9ProxyVolumeTexture::GetLevelCount()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetLevelCount");
	return m_pActualTexture->GetLevelCount();

}

HRESULT              D3D9ProxyVolumeTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::SetAutoGenFilterType");
	return m_pActualTexture->SetAutoGenFilterType(FilterType);

}

D3DTEXTUREFILTERTYPE D3D9ProxyVolumeTexture::GetAutoGenFilterType()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetAutoGenFilterType");
	return m_pActualTexture->GetAutoGenFilterType();

}

void                 D3D9ProxyVolumeTexture::GenerateMipSubLevels()
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GenerateMipSubLevels");
	return m_pActualTexture->GenerateMipSubLevels();

}

HRESULT              D3D9ProxyVolumeTexture::GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::GetLevelDesc");
	return m_pActualTexture->GetLevelDesc(Level, pDesc);

}

HRESULT    D3D9ProxyVolumeTexture::LockBox(UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::LockBox");

	D3DVOLUME_DESC desc;
	m_pActualTexture->GetLevelDesc(Level, &desc);
	if (desc.Pool != D3DPOOL_DEFAULT)
	{
		//Can't really handle stereo for this, so just lock on the original texture
		return m_pActualTexture->LockBox(Level, pLockedVolume, pBox, Flags);
	}

	//Create lockable system memory surfaces
	HRESULT hr = D3DERR_INVALIDCALL;
//	bool createdTexture = false;
//	IDirect3DVolume9 *pVolume= NULL;
	if (!lockableSysMemVolume)
	{
		hr = m_pOwningDevice->getActual()->CreateVolumeTexture(desc.Width, desc.Height, desc.Depth, 1, 0, 
			desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemVolume, NULL);
		if (FAILED(hr))
			return hr;
//		createdTexture = true;
	}

//	hr = lockableSysMemVolumes[Level]->GetVolumeLevel(0, &pVolume);
//	if (FAILED(hr))
//		return hr;

/*	if (createdTexture)
	{
		IDirect3DSurface9 *pActualSurface = NULL;
		hr = m_pActualTexture->GetCubeMapSurface(FaceType, Level, &pActualSurface);
		if (FAILED(hr))
			return hr;
		hr = m_pOwningDevice->getActual()->GetVol(pActualSurface, pSurface);
//		if (FAILED(hr))
//			OutputDebugString("D3DProxySurface::LockRect: Could not GetRenderTargetData");
		pActualSurface->Release();
	}*/

	if (((Flags|D3DLOCK_NO_DIRTY_UPDATE) != D3DLOCK_NO_DIRTY_UPDATE) &&
		((Flags|D3DLOCK_READONLY) != D3DLOCK_READONLY))
		hr = m_pActualTexture->AddDirtyBox(pBox);

	hr = lockableSysMemVolume->LockBox(Level, pLockedVolume, pBox, Flags);
	if (FAILED(hr))
		return hr;

//	pVolume->Release();

	return hr;
}

HRESULT D3D9ProxyVolumeTexture::UnlockBox(UINT Level)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::UnlockBox");
	D3DVOLUME_DESC desc;
	m_pActualTexture->GetLevelDesc(Level, &desc);
	if (desc.Pool != D3DPOOL_DEFAULT)
	{
		return m_pActualTexture->UnlockBox(Level);
	}

//	IDirect3DVolume9 *pVolume= NULL;
//	HRESULT hr = lockableSysMemVolumes[Level] ? lockableSysMemVolumes[Level]->GetVolumeLevel(0, &pVolume) : D3DERR_INVALIDCALL;
//	if (FAILED(hr))
//		return hr;

	HRESULT hr = lockableSysMemVolume->UnlockBox(Level);
	if (FAILED(hr))
		return hr;

//	IDirect3DSurface9 *pActualSurface = NULL;
//	hr = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurface);
//	if (FAILED(hr))
//		return hr;
//	if (fullSurfaces[Level])
	{
		hr = m_pOwningDevice->getActual()->UpdateTexture(lockableSysMemVolume, m_pActualTexture);
		if (FAILED(hr))
			return hr;
	}
/*	else
	{
		std::vector<RECT>::iterator rectIter = lockedRects[Level].begin();
		while (rectIter != lockedRects[Level].end())
		{
			POINT p;
			p.x = rectIter->left;
			p.y = rectIter->top;
			hr = m_pOwningDevice->getActual()->UpdateSurface(pSurface, &(*rectIter), pActualSurface, &p);
			if (FAILED(hr))
				return hr;
			rectIter++;
		}
	}*/

	//Release everything
	//pActualSurface->Release();
//	pSurface->Release();
	
//	fullSurfaces[Level] = false;
	return hr;
}

HRESULT              D3D9ProxyVolumeTexture::AddDirtyBox(const D3DBOX *pDirtyBox)
{
	SHOW_CALL("D3D9ProxyVolumeTexture::AddDirtyBox");
	return m_pActualTexture->AddDirtyBox(pDirtyBox);
}

/**
* Returns the actual volume texture.
***/
IDirect3DVolumeTexture9* D3D9ProxyVolumeTexture::getActual()
{
	return m_pActualTexture;
}