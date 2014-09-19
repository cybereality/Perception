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

#include "D3DProxyDevice.h"
#include "D3D9ProxyVolumeTexture.h"
/**
* Constructor.
* @see D3D9ProxySurface::D3D9ProxySurface
***/
D3D9ProxyVolumeTexture::D3D9ProxyVolumeTexture(IDirect3DVolumeTexture9* pActualVolumeTexture, D3DProxyDevice* pOwningDevice) :
	cBase( pActualVolumeTexture , pOwningDevice )
{
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

		HRESULT result = actual->GetVolumeLevel(Level, &pActualVolumeLevel);

		if (SUCCEEDED(result)) {

			D3D9ProxyVolume* pWrappedVolumeLevel = new D3D9ProxyVolume(pActualVolumeLevel,device, this);

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
				OutputDebugStringA(__FUNCTION__);
				OutputDebugStringA("\n");
				OutputDebugStringA("Unable to store surface level.\n");
				assert(false);

				finalResult = D3DERR_INVALIDCALL;
			}
		}
		else { 
			OutputDebugStringA(__FUNCTION__);
			OutputDebugStringA("\n");
			OutputDebugStringA("Error fetching actual surface level.\n");
			finalResult = result;
		}
	}

	return finalResult;
}



METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , SetPrivateData , REFGUID , refguid , CONST void* , pData , DWORD , SizeOfData , DWORD , Flags )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , GetPrivateData , REFGUID , refguid , void* , pData , DWORD* , pSizeOfData )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , FreePrivateData , REFGUID , refguid )
METHOD_THRU( DWORD                , WINAPI , D3D9ProxyVolumeTexture , SetPriority , DWORD , PriorityNew )
METHOD_THRU( void                 , WINAPI , D3D9ProxyVolumeTexture , PreLoad )
METHOD_THRU( DWORD                , WINAPI , D3D9ProxyVolumeTexture , SetLOD , DWORD , LODNew )
METHOD_THRU( DWORD                , WINAPI , D3D9ProxyVolumeTexture , GetLOD )
METHOD_THRU( DWORD                , WINAPI , D3D9ProxyVolumeTexture , GetLevelCount )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , SetAutoGenFilterType , D3DTEXTUREFILTERTYPE , FilterType )
METHOD_THRU( D3DTEXTUREFILTERTYPE , WINAPI , D3D9ProxyVolumeTexture , GetAutoGenFilterType )
METHOD_THRU( void                 , WINAPI , D3D9ProxyVolumeTexture , GenerateMipSubLevels )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , GetLevelDesc , UINT , Level , D3DVOLUME_DESC* , pDesc )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , LockBox , UINT , Level, D3DLOCKED_BOX* ,  pLockedVolume , const D3DBOX* , pBox , DWORD , Flags )
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , UnlockBox , UINT , Level)
METHOD_THRU( HRESULT              , WINAPI , D3D9ProxyVolumeTexture , AddDirtyBox , const D3DBOX* , pDirtyBox )
METHOD_THRU( DWORD                , WINAPI , D3D9ProxyVolumeTexture , GetPriority )
METHOD_THRU( D3DRESOURCETYPE      , WINAPI , D3D9ProxyVolumeTexture , GetType )
