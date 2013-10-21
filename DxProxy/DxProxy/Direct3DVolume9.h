/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DVolume9.h> and
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

#ifndef DIRECT3DVOLUME9_H_INCLUDED
#define DIRECT3DVOLUME9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

/**
*  Direct 3D volume class. 
*  Overwrites IDirect3DVolume9 and imbeds the actual volume.
*/
class BaseDirect3DVolume9 : public IDirect3DVolume9
{
public:
	BaseDirect3DVolume9(IDirect3DVolume9* pActualVolume);
	virtual ~BaseDirect3DVolume9();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();

	/*** IDirect3DVolume9 methods ***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer);
	virtual HRESULT WINAPI GetDesc(D3DVOLUME_DESC *pDesc);
	virtual HRESULT WINAPI LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
	virtual HRESULT WINAPI UnlockBox();

protected:
	/**
	* The actual volume embedded. 
	***/
	IDirect3DVolume9* const m_pActualVolume;
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
};

#endif
