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

D3D9ProxyStereoTexture::D3D9ProxyStereoTexture(IDirect3DTexture9* leftTexture, IDirect3DTexture9* rightTexture) :
	BaseDirect3DTexture9(leftTexture),
	m_levels(1, NULL)
{

}

D3D9ProxyStereoTexture::~D3D9ProxyStereoTexture()
{
}





ULONG WINAPI D3D9ProxyStereoTexture::AddRef()
{
	m_pActualTextureRight->AddRef();
	return m_pActualTexture->AddRef();
}


ULONG WINAPI D3D9ProxyStereoTexture::Release()
{
	ULONG rightRefCount = m_pActualTextureRight->Release();
	ULONG refCount = m_pActualTexture->Release();

	if (rightRefCount != refCount)
		OutputDebugString("Error: StereoSurface - Ref count Releasing left and right surfaces does not match.\n");

	if (refCount <= 0) {
		delete this;
	}

	return refCount;
}



HRESULT WINAPI D3D9ProxyStereoTexture::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
{
	return m_pActualTexture->GetLevelDesc(Level, pDesc);
}

HRESULT WINAPI D3D9ProxyStereoTexture::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	// return a stereo surface 


	return m_pActualTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}

HRESULT WINAPI D3D9ProxyStereoTexture::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pActualTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI D3D9ProxyStereoTexture::UnlockRect(UINT Level)
{
	return m_pActualTexture->UnlockRect(Level);
}

HRESULT WINAPI D3D9ProxyStereoTexture::AddDirtyRect(CONST RECT* pDirtyRect)
{
	return m_pActualTexture->AddDirtyRect(pDirtyRect);
}






HRESULT WINAPI D3D9ProxyStereoTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pActualTexture->GetDevice(ppDevice);
}

HRESULT WINAPI D3D9ProxyStereoTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI D3D9ProxyStereoTexture::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT WINAPI D3D9ProxyStereoTexture::FreePrivateData(REFGUID refguid)
{
	return m_pActualTexture->FreePrivateData(refguid);
}

DWORD WINAPI D3D9ProxyStereoTexture::SetPriority(DWORD PriorityNew)
{
	return m_pActualTexture->SetPriority(PriorityNew);
}

DWORD WINAPI D3D9ProxyStereoTexture::GetPriority()
{
	return m_pActualTexture->GetPriority();
}

void WINAPI D3D9ProxyStereoTexture::PreLoad()
{
	return m_pActualTexture->PreLoad();
}

DWORD WINAPI D3D9ProxyStereoTexture::SetLOD(DWORD LODNew)
{
	return m_pActualTexture->SetLOD(LODNew);
}

DWORD WINAPI D3D9ProxyStereoTexture::GetLOD()
{
	return m_pActualTexture->GetLOD();
}

DWORD WINAPI D3D9ProxyStereoTexture::GetLevelCount()
{
	return m_pActualTexture->GetLevelCount();
}

HRESULT WINAPI D3D9ProxyStereoTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI D3D9ProxyStereoTexture::GenerateMipSubLevels()
{
	return m_pActualTexture->GenerateMipSubLevels();
}

D3DTEXTUREFILTERTYPE WINAPI D3D9ProxyStereoTexture::GetAutoGenFilterType()
{
	return m_pActualTexture->GetAutoGenFilterType();
}

D3DRESOURCETYPE WINAPI D3D9ProxyStereoTexture::GetType()
{
	return m_pActualTexture->GetType();
}
