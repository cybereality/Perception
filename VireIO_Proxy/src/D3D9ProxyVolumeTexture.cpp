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

#include <assert.h>
#include "D3DProxyDevice.h"

/**
* Constructor.
* @see D3D9ProxySurface::D3D9ProxySurface
***/
D3D9ProxyVolumeTexture::D3D9ProxyVolumeTexture(IDirect3DVolumeTexture9* pActualVolumeTexture, D3DProxyDevice* pOwningDevice) :
	m_pActualTexture(pActualVolumeTexture),
	m_nRefCount(1),
	m_wrappedVolumeLevels(),
	m_pOwningDevice(pOwningDevice)
{
	assert (pOwningDevice != NULL);
	assert (m_pActualTexture != NULL);

	m_pOwningDevice->AddRef();
}

/**
* Destructor.
* Deletes wrapped volumes.
***/
D3D9ProxyVolumeTexture::~D3D9ProxyVolumeTexture()
{
	// delete all surfaces in m_levels
	auto it = m_wrappedVolumeLevels.begin();
	while (it != m_wrappedVolumeLevels.end()) {
		// we have to explicitly delete the Volume here as the Release behaviour of the volume would get stuck in a loop
		// calling back to the container Release.
		delete it->second;
		it = m_wrappedVolumeLevels.erase(it);
	}

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
	
	if (m_pActualTexture)
		m_pActualTexture->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualTexture->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI D3D9ProxyVolumeTexture::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI D3D9ProxyVolumeTexture::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
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
HRESULT WINAPI D3D9ProxyVolumeTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}



/**
* Base SetPrivateData functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Base GetPrivateData functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

/**
* Base FreePrivateData functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::FreePrivateData(REFGUID refguid)
{
	return m_pActualTexture->FreePrivateData(refguid);
}

/**
* Base SetPriority functionality. 
***/
DWORD WINAPI D3D9ProxyVolumeTexture::SetPriority(DWORD PriorityNew)
{
	return m_pActualTexture->SetPriority(PriorityNew);
}

/**
* Base GetPriority functionality. 
***/
DWORD WINAPI D3D9ProxyVolumeTexture::GetPriority()
{
	return m_pActualTexture->GetPriority();
}

/**
* Base PreLoad functionality. 
***/
void WINAPI D3D9ProxyVolumeTexture::PreLoad()
{
	return m_pActualTexture->PreLoad();
}

/**
* Base GetType functionality. 
***/
D3DRESOURCETYPE WINAPI D3D9ProxyVolumeTexture::GetType()
{
	return m_pActualTexture->GetType();
}

/**
* Base SetLOD functionality. 
***/
DWORD WINAPI D3D9ProxyVolumeTexture::SetLOD(DWORD LODNew)
{
	return m_pActualTexture->SetLOD(LODNew);
}

/**
* Base GetLOD functionality. 
***/
DWORD WINAPI D3D9ProxyVolumeTexture::GetLOD()
{
	return m_pActualTexture->GetLOD();
}

/**
* Base GetLevelCount functionality. 
***/
DWORD WINAPI D3D9ProxyVolumeTexture::GetLevelCount()
{
	return m_pActualTexture->GetLevelCount();
}

/**
* Base SetAutoGenFilterType functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

/**
* Base GetAutoGenFilterType functionality. 
***/
D3DTEXTUREFILTERTYPE WINAPI D3D9ProxyVolumeTexture::GetAutoGenFilterType()
{
	return m_pActualTexture->GetAutoGenFilterType();
}

/**
* Base GenerateMipSubLevels functionality. 
***/
void WINAPI D3D9ProxyVolumeTexture::GenerateMipSubLevels()
{
	return m_pActualTexture->GenerateMipSubLevels();
}

/**
* Base GetLevelDesc functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc)
{
	return m_pActualTexture->GetLevelDesc(Level, pDesc);
}


/**
* If proxy volume is already stored on this level, return this one, otherwise create it.
* To create a new stored volume level, call the method on both (left/right) actual volumes.
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel)
{
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


/**
* Base LockBox functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::LockBox(UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
{
	return m_pActualTexture->LockBox(Level, pLockedVolume, pBox, Flags);
}
	
/**
* Base UnlockBox functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::UnlockBox(UINT Level)
{
	return m_pActualTexture->UnlockBox(Level);
}

/**
* Base AddDirtyBox functionality. 
***/
HRESULT WINAPI D3D9ProxyVolumeTexture::AddDirtyBox(const D3DBOX *pDirtyBox)
{
	return m_pActualTexture->AddDirtyBox(pDirtyBox);
}


/**
* Returns the actual volume texture.
***/
IDirect3DVolumeTexture9* D3D9ProxyVolumeTexture::getActual()
{
	return m_pActualTexture;
}