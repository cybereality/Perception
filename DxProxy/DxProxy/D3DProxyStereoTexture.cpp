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

#include "D3DProxyStereoTexture.h"

D3DProxyStereoTexture::D3DProxyStereoTexture(IDirect3DTexture9* leftTexture, IDirect3DTexture9* rightTexture) :
	BaseDirect3DTexture9(leftTexture),
	m_levels(1, NULL)
{

}

D3DProxyStereoTexture::~D3DProxyStereoTexture()
{
	OutputDebugString("Release Right Texture\n");
	if(m_pTexture) {
		int newRefCount = m_pTexture->Release();

		if (newRefCount > 0) {
			char buf[128];
			sprintf_s(buf, "Error: count = %d\n", newRefCount);
			OutputDebugString(buf);
		}

		m_pTexture = NULL;
	}
}






HRESULT WINAPI D3DProxyStereoTexture::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
{
	return m_pTexture->GetLevelDesc(Level, pDesc);
}

HRESULT WINAPI D3DProxyStereoTexture::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	// return a stereo surface 


	return m_pTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}

HRESULT WINAPI D3DProxyStereoTexture::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI D3DProxyStereoTexture::UnlockRect(UINT Level)
{
	return m_pTexture->UnlockRect(Level);
}

HRESULT WINAPI D3DProxyStereoTexture::AddDirtyRect(CONST RECT* pDirtyRect)
{
	return m_pTexture->AddDirtyRect(pDirtyRect);
}






HRESULT WINAPI D3DProxyStereoTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pTexture->GetDevice(ppDevice);
}

HRESULT WINAPI D3DProxyStereoTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI D3DProxyStereoTexture::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT WINAPI D3DProxyStereoTexture::FreePrivateData(REFGUID refguid)
{
	return m_pTexture->FreePrivateData(refguid);
}

DWORD WINAPI D3DProxyStereoTexture::SetPriority(DWORD PriorityNew)
{
	return m_pTexture->SetPriority(PriorityNew);
}

DWORD WINAPI D3DProxyStereoTexture::GetPriority()
{
	return m_pTexture->GetPriority();
}

void WINAPI D3DProxyStereoTexture::PreLoad()
{
	return m_pTexture->PreLoad();
}

DWORD WINAPI D3DProxyStereoTexture::SetLOD(DWORD LODNew)
{
	return m_pTexture->SetLOD(LODNew);
}

DWORD WINAPI D3DProxyStereoTexture::GetLOD()
{
	return m_pTexture->GetLOD();
}

DWORD WINAPI D3DProxyStereoTexture::GetLevelCount()
{
	return m_pTexture->GetLevelCount();
}

HRESULT WINAPI D3DProxyStereoTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI D3DProxyStereoTexture::GenerateMipSubLevels()
{
	return m_pTexture->GenerateMipSubLevels();
}

D3DTEXTUREFILTERTYPE WINAPI D3DProxyStereoTexture::GetAutoGenFilterType()
{
	return m_pTexture->GetAutoGenFilterType();
}

D3DRESOURCETYPE WINAPI D3DProxyStereoTexture::GetType()
{
	return m_pTexture->GetType();
}
