/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVolumeTexture.h> and
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

#pragma once
#include <d3d9.h>
#include <unordered_map>
#include "D3D9ProxyVolume.h"
#include "D3DProxyDevice.h"
#include <cBase.h>

/**
*  Direct 3D proxy volume texture class. 
*  Overwrites BaseDirect3DVolumeTexture9 and imbeds wrapped volume levels.
*/
class D3D9ProxyVolumeTexture : public cBase<IDirect3DVolumeTexture9>
{
public:
	D3D9ProxyVolumeTexture(IDirect3DVolumeTexture9* pActualVolumeTexture, D3DProxyDevice* pOwningDevice);
	~D3D9ProxyVolumeTexture();

	/*** IDirect3DBaseTexture9 methods ***/
	DWORD                WINAPI GetPriority();
	D3DRESOURCETYPE      WINAPI GetType();
	HRESULT              WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	HRESULT              WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	HRESULT              WINAPI FreePrivateData(REFGUID refguid);
	DWORD                WINAPI SetPriority(DWORD PriorityNew);
	void                 WINAPI PreLoad();
	DWORD                WINAPI SetLOD(DWORD LODNew);
	DWORD                WINAPI GetLOD();
	DWORD                WINAPI GetLevelCount();
	HRESULT              WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType();
	void                 WINAPI GenerateMipSubLevels();
	HRESULT              WINAPI GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc);
	HRESULT              WINAPI GetVolumeLevel(UINT Level, IDirect3DVolume9 **ppVolumeLevel);
	HRESULT              WINAPI LockBox(UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
	HRESULT              WINAPI UnlockBox(UINT Level);
	HRESULT              WINAPI AddDirtyBox(const D3DBOX *pDirtyBox);

protected:
	std::unordered_map<UINT, D3D9ProxyVolume*> m_wrappedVolumeLevels;

};
