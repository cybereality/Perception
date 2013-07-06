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

#ifndef D3DPROXYTEXTURE_H_INCLUDED
#define D3DPROXYTEXTURE_H_INCLUDED

#include <d3d9.h>
#include <unordered_map>
#include "Direct3DTexture9.h"
#include "D3D9ProxySurface.h"
#include "IStereoCapableWrapper.h"


class D3D9ProxyTexture : public BaseDirect3DTexture9, public IStereoCapableWrapper<IDirect3DTexture9>
{
public:
	D3D9ProxyTexture(IDirect3DTexture9* pActualTextureLeft, IDirect3DTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice);
	virtual ~D3D9ProxyTexture();

	// IStereoCapableWrapper
	virtual bool IsStereo();
	virtual IDirect3DTexture9* getActualMono();
	virtual IDirect3DTexture9* getActualLeft();
	virtual IDirect3DTexture9* getActualRight();
	
	
	// IDirect3DResource9 methods
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);	
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual   DWORD WINAPI SetPriority(DWORD PriorityNew);
	virtual    void WINAPI PreLoad();


	//base texture methods
	virtual   DWORD WINAPI SetLOD(DWORD LODNew);
	virtual HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	virtual    void WINAPI GenerateMipSubLevels();


	// texture methods
	virtual HRESULT WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel); 
    virtual HRESULT WINAPI LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect(UINT Level);
	virtual HRESULT WINAPI AddDirtyRect(CONST RECT* pDirtyRect);
	

protected:

	/* Wrapped Surface levels */
	std::unordered_map<UINT, D3D9ProxySurface*> m_wrappedSurfaceLevels;

	BaseDirect3DDevice9* const m_pOwningDevice;
	IDirect3DTexture9* const m_pActualTextureRight;
};

#endif
