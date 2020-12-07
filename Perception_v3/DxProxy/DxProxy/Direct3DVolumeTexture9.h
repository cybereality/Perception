/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DVolumeTexture9.h> and
Class <BaseDirect3DVolumeTexture9> :
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

#ifndef DIRECT3DVOLUMETEXTURE9_H_INCLUDED
#define DIRECT3DVOLUMETEXTURE9_H_INCLUDED

#include <d3d9.h>

/**
*  Direct 3D volume texture class. 
*  Overwrites IDirect3DVolumeTexture9 and imbeds the actual volume texture.
*/
class BaseDirect3DVolumeTexture9 : public IDirect3DVolumeTexture9
{
public:
	BaseDirect3DVolumeTexture9(IDirect3DVolumeTexture9* pVolumeTexture);
	virtual ~BaseDirect3DVolumeTexture9();

	//*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG	WINAPI AddRef();
	virtual ULONG	WINAPI Release();

	/*** IDirect3DBaseTexture9 methods ***/
	virtual HRESULT              WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT              WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT              WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	virtual HRESULT              WINAPI FreePrivateData(REFGUID refguid);
	virtual DWORD                WINAPI SetPriority(DWORD PriorityNew);
	virtual DWORD                WINAPI GetPriority();
	virtual void                 WINAPI PreLoad();
	virtual D3DRESOURCETYPE      WINAPI GetType();
	virtual DWORD                WINAPI SetLOD(DWORD LODNew);
	virtual DWORD                WINAPI GetLOD();
	virtual DWORD                WINAPI GetLevelCount();
	virtual HRESULT              WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	virtual D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType();
	virtual void                 WINAPI GenerateMipSubLevels();
	virtual HRESULT              WINAPI GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc);
	virtual HRESULT              WINAPI GetVolumeLevel(UINT Level, IDirect3DVolume9 **ppVolumeLevel);
	virtual HRESULT              WINAPI LockBox(UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
	virtual HRESULT              WINAPI UnlockBox(UINT Level);
	virtual HRESULT              WINAPI AddDirtyBox(const D3DBOX *pDirtyBox);

protected:
	/**
	* The actual texture embedded. 
	***/
	IDirect3DVolumeTexture9* const m_pActualTexture;

private:
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
};

#endif