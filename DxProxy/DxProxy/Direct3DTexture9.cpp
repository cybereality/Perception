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

#include "Direct3DTexture9.h"

BaseDirect3DTexture9::BaseDirect3DTexture9(IDirect3DTexture9* pTexture) :
	m_pTexture(pTexture),
	m_nRefCount(1)
{
}

BaseDirect3DTexture9::~BaseDirect3DTexture9()
{
	OutputDebugString("Release Left Texture\n");
	if(m_pTexture) {
		int newRefCount = m_pTexture->Release();

		if (newRefCount > 0) {
			char buf[256];
			sprintf_s(buf, "Error: count = %d\n", newRefCount);
			OutputDebugString(buf);
		}

		m_pTexture = NULL;
	}
}

HRESULT WINAPI BaseDirect3DTexture9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pTexture->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DTexture9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DTexture9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}





HRESULT WINAPI BaseDirect3DTexture9::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
{
	return m_pTexture->GetLevelDesc(Level, pDesc);
}

HRESULT WINAPI BaseDirect3DTexture9::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	return m_pTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}

HRESULT WINAPI BaseDirect3DTexture9::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI BaseDirect3DTexture9::UnlockRect(UINT Level)
{
	return m_pTexture->UnlockRect(Level);
}

HRESULT WINAPI BaseDirect3DTexture9::AddDirtyRect(CONST RECT* pDirtyRect)
{
	return m_pTexture->AddDirtyRect(pDirtyRect);
}






HRESULT WINAPI BaseDirect3DTexture9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pTexture->GetDevice(ppDevice);
}

HRESULT WINAPI BaseDirect3DTexture9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI BaseDirect3DTexture9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT WINAPI BaseDirect3DTexture9::FreePrivateData(REFGUID refguid)
{
	return m_pTexture->FreePrivateData(refguid);
}

DWORD WINAPI BaseDirect3DTexture9::SetPriority(DWORD PriorityNew)
{
	return m_pTexture->SetPriority(PriorityNew);
}

DWORD WINAPI BaseDirect3DTexture9::GetPriority()
{
	return m_pTexture->GetPriority();
}

void WINAPI BaseDirect3DTexture9::PreLoad()
{
	return m_pTexture->PreLoad();
}

DWORD WINAPI BaseDirect3DTexture9::SetLOD(DWORD LODNew)
{
	return m_pTexture->SetLOD(LODNew);
}

DWORD WINAPI BaseDirect3DTexture9::GetLOD()
{
	return m_pTexture->GetLOD();
}

DWORD WINAPI BaseDirect3DTexture9::GetLevelCount()
{
	return m_pTexture->GetLevelCount();
}

HRESULT WINAPI BaseDirect3DTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI BaseDirect3DTexture9::GenerateMipSubLevels()
{
	return m_pTexture->GenerateMipSubLevels();
}

D3DTEXTUREFILTERTYPE WINAPI BaseDirect3DTexture9::GetAutoGenFilterType()
{
	return m_pTexture->GetAutoGenFilterType();
}

D3DRESOURCETYPE WINAPI BaseDirect3DTexture9::GetType()
{
	return m_pTexture->GetType();
}
