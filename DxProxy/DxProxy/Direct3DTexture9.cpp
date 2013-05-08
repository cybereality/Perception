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
#include <assert.h>

BaseDirect3DTexture9::BaseDirect3DTexture9(IDirect3DTexture9* pActualTexture) :
	m_pActualTexture(pActualTexture),
	m_nRefCount(1)
{
	assert (pActualTexture != NULL);
}

BaseDirect3DTexture9::~BaseDirect3DTexture9()
{
	if (m_pActualTexture)
		m_pActualTexture->Release();
}

HRESULT WINAPI BaseDirect3DTexture9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualTexture->QueryInterface(riid, ppv);
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
	return m_pActualTexture->GetLevelDesc(Level, pDesc);
}

HRESULT WINAPI BaseDirect3DTexture9::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	return m_pActualTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}

HRESULT WINAPI BaseDirect3DTexture9::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pActualTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
	
HRESULT WINAPI BaseDirect3DTexture9::UnlockRect(UINT Level)
{
	return m_pActualTexture->UnlockRect(Level);
}

HRESULT WINAPI BaseDirect3DTexture9::AddDirtyRect(CONST RECT* pDirtyRect)
{
	return m_pActualTexture->AddDirtyRect(pDirtyRect);
}






HRESULT WINAPI BaseDirect3DTexture9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pActualTexture->GetDevice(ppDevice);
}

HRESULT WINAPI BaseDirect3DTexture9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT WINAPI BaseDirect3DTexture9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT WINAPI BaseDirect3DTexture9::FreePrivateData(REFGUID refguid)
{
	return m_pActualTexture->FreePrivateData(refguid);
}

DWORD WINAPI BaseDirect3DTexture9::SetPriority(DWORD PriorityNew)
{
	return m_pActualTexture->SetPriority(PriorityNew);
}

DWORD WINAPI BaseDirect3DTexture9::GetPriority()
{
	return m_pActualTexture->GetPriority();
}

void WINAPI BaseDirect3DTexture9::PreLoad()
{
	return m_pActualTexture->PreLoad();
}

DWORD WINAPI BaseDirect3DTexture9::SetLOD(DWORD LODNew)
{
	return m_pActualTexture->SetLOD(LODNew);
}

DWORD WINAPI BaseDirect3DTexture9::GetLOD()
{
	return m_pActualTexture->GetLOD();
}

DWORD WINAPI BaseDirect3DTexture9::GetLevelCount()
{
	return m_pActualTexture->GetLevelCount();
}

HRESULT WINAPI BaseDirect3DTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

void WINAPI BaseDirect3DTexture9::GenerateMipSubLevels()
{
	return m_pActualTexture->GenerateMipSubLevels();
}

D3DTEXTUREFILTERTYPE WINAPI BaseDirect3DTexture9::GetAutoGenFilterType()
{
	return m_pActualTexture->GetAutoGenFilterType();
}

D3DRESOURCETYPE WINAPI BaseDirect3DTexture9::GetType()
{
	return m_pActualTexture->GetType();
}
