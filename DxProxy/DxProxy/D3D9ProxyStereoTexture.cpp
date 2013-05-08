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

#include "D3D9ProxyStereoTexture.h"
#include <assert.h>

D3D9ProxyStereoTexture::D3D9ProxyStereoTexture(IDirect3DTexture9* pActualTextureLeft, IDirect3DTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice) :
	D3D9ProxyTexture(pActualTextureLeft, pOwningDevice),
	m_pActualTextureRight(pActualTextureRight)
{
}

D3D9ProxyStereoTexture::~D3D9ProxyStereoTexture()
{
	if (m_pActualTextureRight)
		m_pActualTextureRight->Release();
}









HRESULT WINAPI D3D9ProxyStereoTexture::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	// Have we already got a Proxy for this surface level?
	if (m_wrappedSurfaceLevels.count(Level) == 1) { // yes

		// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
		// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

		*ppSurfaceLevel = m_wrappedSurfaceLevels[Level];
		return D3D_OK;
	}
	else {
		// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
		//  wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
		IDirect3DSurface9* pActualSurfaceLevelLeft;
		HRESULT result = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurfaceLevelLeft);

		IDirect3DSurface9* pActualSurfaceLevelRight;
		HRESULT resultRight = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurfaceLevelRight);

		assert( result == resultRight);


		if (SUCCEEDED(result)) {

			D3D9ProxyStereoSurface* pWrappedSurfaceLevel = new D3D9ProxyStereoSurface(pActualSurfaceLevelLeft, pActualSurfaceLevelRight, m_pOwningDevice, this);

			if(!m_wrappedSurfaceLevels.insert(std::pair<ULONG, D3D9ProxyStereoSurface*>(Level, pWrappedSurfaceLevel)).second) {
				// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
				*ppSurfaceLevel = pWrappedSurfaceLevel;
				return D3D_OK;
			}
			else {
				// Failure to insert should not be possible. In this case we could still return the wrapped surface,
				// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
				// at some point. Better to fail fast.
				OutputDebugString(__FUNCTION__);
				OutputDebugString("\n");
				OutputDebugString("Unable to store surface level\n");
				assert(false);

				return D3DERR_INVALIDCALL;
			}
		}
		else { 
			OutputDebugString(__FUNCTION__);
			OutputDebugString("\n");
			OutputDebugString("Error fetching actual surface level.\n");
			return result;
		}
	}

	//return m_pActualTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}




HRESULT WINAPI D3D9ProxyStereoTexture::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	if (IsStereo())
		m_pActualTextureRight->LockRect(Level, pLockedRect, pRect, Flags);

	return m_pActualTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI D3D9ProxyStereoTexture::UnlockRect(UINT Level)
{
	if (IsStereo())
		m_pActualTextureRight->UnlockRect(Level);

	return m_pActualTexture->UnlockRect(Level);
}

HRESULT WINAPI D3D9ProxyStereoTexture::AddDirtyRect(CONST RECT* pDirtyRect)
{
	if (IsStereo())
		m_pActualTextureRight->AddDirtyRect(pDirtyRect);

	return m_pActualTexture->AddDirtyRect(pDirtyRect);
}






HRESULT WINAPI D3D9ProxyStereoTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	if (IsStereo())
		m_pActualTextureRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}



HRESULT WINAPI D3D9ProxyStereoTexture::FreePrivateData(REFGUID refguid)
{
	if (IsStereo())
		m_pActualTextureRight->FreePrivateData(refguid);

	return m_pActualTexture->FreePrivateData(refguid);
}

DWORD WINAPI D3D9ProxyStereoTexture::SetPriority(DWORD PriorityNew)
{
	if (IsStereo())
		m_pActualTextureRight->SetPriority(PriorityNew);

	return m_pActualTexture->SetPriority(PriorityNew);
}



void WINAPI D3D9ProxyStereoTexture::PreLoad()
{
	if (IsStereo())
		m_pActualTextureRight->PreLoad();

	return m_pActualTexture->PreLoad();
}

DWORD WINAPI D3D9ProxyStereoTexture::SetLOD(DWORD LODNew)
{
	if (IsStereo())
		m_pActualTextureRight->SetLOD(LODNew);

	return m_pActualTexture->SetLOD(LODNew);
}



HRESULT WINAPI D3D9ProxyStereoTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	if (IsStereo())
		m_pActualTextureRight->SetAutoGenFilterType(FilterType);

	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI D3D9ProxyStereoTexture::GenerateMipSubLevels()
{
	if (IsStereo())
		m_pActualTextureRight->GenerateMipSubLevels();

	return m_pActualTexture->GenerateMipSubLevels();
}




bool D3D9ProxyStereoTexture::IsStereo() 
{
	return (m_pActualTextureRight != NULL);
}

IDirect3DTexture9* D3D9ProxyStereoTexture::getMonoTexture()
{
	return getLeftTexture();
}

IDirect3DTexture9* D3D9ProxyStereoTexture::getLeftTexture()
{
	return m_pActualTexture;
}

IDirect3DTexture9* D3D9ProxyStereoTexture::getRightTexture()
{
	return m_pActualTextureRight;
}