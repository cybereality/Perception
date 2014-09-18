/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyTexture.h> and
Class <D3D9ProxyTexture> :
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
#include "D3D9ProxySurface.h"
#include "D3DProxyDevice.h"
#include <cBase.h>


/**
*  Direct 3D proxy texture class. 
*  Overwrites IDirect3DTexture9 and imbeds the additional right texture pointer.
*/
class D3D9ProxyTexture : public cBase<IDirect3DTexture9> {
public:
	D3D9ProxyTexture(IDirect3DTexture9* pActualTextureLeft, IDirect3DTexture9* pActualTextureRight, D3DProxyDevice* pOwningDevice);
	~D3D9ProxyTexture();

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);

	/*** IDirect3DBaseTexture9 methods ***/
	HRESULT              WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	HRESULT              WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	HRESULT              WINAPI FreePrivateData(REFGUID refguid);
	DWORD                WINAPI SetPriority(DWORD PriorityNew);
	DWORD                WINAPI GetPriority();
	void                 WINAPI PreLoad();
	D3DRESOURCETYPE      WINAPI GetType();
	DWORD                WINAPI SetLOD(DWORD LODNew);
	DWORD                WINAPI GetLOD();
	DWORD                WINAPI GetLevelCount();
	HRESULT              WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType();
	void                 WINAPI GenerateMipSubLevels();
	HRESULT              WINAPI GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc);
	HRESULT              WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel);
	HRESULT              WINAPI LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	HRESULT              WINAPI UnlockRect(UINT Level);
	HRESULT              WINAPI AddDirtyRect(CONST RECT* pDirtyRect);

	std::unordered_map<UINT, D3D9ProxySurface*> m_wrappedSurfaceLevels;
	IDirect3DTexture9* right;
};