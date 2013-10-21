/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DCubeTexture9.cpp> and
Class <BaseDirect3DCubeTexture9> :
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

#include "Direct3DCubeTexture9.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualTexture Imbed actual texture. 
***/
BaseDirect3DCubeTexture9::BaseDirect3DCubeTexture9(IDirect3DCubeTexture9* pActualTexture) :
	m_pActualTexture(pActualTexture),
	m_nRefCount(1)
{
	assert (pActualTexture != NULL);
}

/**
* Destructor. 
* Releases embedded texture. 
***/
BaseDirect3DCubeTexture9::~BaseDirect3DCubeTexture9()
{
	if (m_pActualTexture)
		m_pActualTexture->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualTexture->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DCubeTexture9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DCubeTexture9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}

/**
* Base GetDevice functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pActualTexture->GetDevice(ppDevice);
}

/**
* Base SetPrivateData functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Base GetPrivateData functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualTexture->GetPrivateData(refguid, pData, pSizeOfData);
}

/**
* Base FreePrivateData functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::FreePrivateData(REFGUID refguid)
{
	return m_pActualTexture->FreePrivateData(refguid);
}

/**
* Base SetPriority functionality. 
***/
DWORD WINAPI BaseDirect3DCubeTexture9::SetPriority(DWORD PriorityNew)
{
	return m_pActualTexture->SetPriority(PriorityNew);
}

/**
* Base GetPriority functionality. 
***/
DWORD WINAPI BaseDirect3DCubeTexture9::GetPriority()
{
	return m_pActualTexture->GetPriority();
}

/**
* Base PreLoad functionality. 
***/
void WINAPI BaseDirect3DCubeTexture9::PreLoad()
{
	return m_pActualTexture->PreLoad();
}

/**
* Base GetType functionality. 
***/
D3DRESOURCETYPE WINAPI BaseDirect3DCubeTexture9::GetType()
{
	return m_pActualTexture->GetType();
}

/**
* Base SetLOD functionality. 
***/
DWORD WINAPI BaseDirect3DCubeTexture9::SetLOD(DWORD LODNew)
{
	return m_pActualTexture->SetLOD(LODNew);
}

/**
* Base GetLOD functionality. 
***/
DWORD WINAPI BaseDirect3DCubeTexture9::GetLOD()
{
	return m_pActualTexture->GetLOD();
}

/**
* Base GetLevelCount functionality. 
***/
DWORD WINAPI BaseDirect3DCubeTexture9::GetLevelCount()
{
	return m_pActualTexture->GetLevelCount();
}

/**
* Base SetAutoGenFilterType functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

/**
* Base GetAutoGenFilterType functionality. 
***/
D3DTEXTUREFILTERTYPE WINAPI BaseDirect3DCubeTexture9::GetAutoGenFilterType()
{
	return m_pActualTexture->GetAutoGenFilterType();
}

/**
* Base GenerateMipSubLevels functionality. 
***/
void WINAPI BaseDirect3DCubeTexture9::GenerateMipSubLevels()
{
	return m_pActualTexture->GenerateMipSubLevels();
}

/**
* Base GetLevelDesc functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
{
	return m_pActualTexture->GetLevelDesc(Level, pDesc);
}

/**
* Base GetCubeMapSurface functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface)
{
	return m_pActualTexture->GetCubeMapSurface(FaceType, Level, ppCubeMapSurface);
}

/**
* Base LockRect functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return m_pActualTexture->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
}

/**
* Base UnlockRect functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level)
{
	return m_pActualTexture->UnlockRect(FaceType, Level);
}

/**
* Base AddDirtyRect functionality. 
***/
HRESULT WINAPI BaseDirect3DCubeTexture9::AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
{
	return m_pActualTexture->AddDirtyRect(FaceType, pDirtyRect);
}