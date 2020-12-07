/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DVolume9.cpp> and
Class <BaseDirect3DVolume9> :
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

#include "Direct3DVolume9.h"
#include <assert.h>

/**
* Constructor. 
* @param pActualVolume Imbed actual volume. 
* @param pOwningDevice Pointer to the device that owns the volume. 
***/
BaseDirect3DVolume9::BaseDirect3DVolume9(IDirect3DVolume9* pActualVolume) :
	m_pActualVolume(pActualVolume),
	m_nRefCount(1)
{
	assert (pActualVolume != NULL);
}

/**
* Destructor. 
* Releases embedded volume. 
***/
BaseDirect3DVolume9::~BaseDirect3DVolume9()
{
	if(m_pActualVolume) {
		m_pActualVolume->Release();
	}
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DVolume9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualVolume->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DVolume9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DVolume9::Release()
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
HRESULT WINAPI BaseDirect3DVolume9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return m_pActualVolume->GetDevice(ppDevice);
}

/**
* Base SetPrivateData functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pActualVolume->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Base GetPrivateData functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return m_pActualVolume->GetPrivateData(refguid, pData, pSizeOfData);
}

/**
* Base FreePrivateData functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::FreePrivateData(REFGUID refguid)
{
	return m_pActualVolume->FreePrivateData(refguid);
}

/**
* Base GetContainer functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::GetContainer(REFIID riid, LPVOID* ppContainer)
{
	return m_pActualVolume->GetContainer(riid, ppContainer);
}

/**
* Base GetDesc functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::GetDesc(D3DVOLUME_DESC *pDesc)
{
	return m_pActualVolume->GetDesc(pDesc);
}

/**
* Base LockBox functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
{
	return m_pActualVolume->LockBox(pLockedVolume, pBox, Flags);
}

/**
* Base UnlockBox functionality.
***/
HRESULT WINAPI BaseDirect3DVolume9::UnlockBox()
{
	return m_pActualVolume->UnlockBox();
}
