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

#ifndef DIRECT3DTEXTURE9_H_INCLUDED
#define DIRECT3DTEXTURE9_H_INCLUDED

#include <d3d9.h>
#include <stdio.h>

class BaseDirect3DTexture9 : public IDirect3DTexture9
{
public:
	BaseDirect3DTexture9(IDirect3DTexture9* pTexture);
	virtual ~BaseDirect3DTexture9();

	// IUnknown methosa
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG WINAPI AddRef();
	virtual ULONG WINAPI Release();
	
	
	// texture methods
	virtual HRESULT WINAPI GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc);
	virtual HRESULT WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel);
    virtual HRESULT WINAPI LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect(UINT Level);
	virtual HRESULT WINAPI AddDirtyRect(CONST RECT* pDirtyRect);


	// IDirect3DResource9 methods
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice);
	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual   DWORD WINAPI SetPriority(DWORD PriorityNew);
	virtual   DWORD WINAPI GetPriority();
	virtual    void WINAPI PreLoad();
	virtual      D3DRESOURCETYPE WINAPI GetType();
	
	//base texture methods
	virtual   DWORD WINAPI SetLOD(DWORD LODNew);
	virtual   DWORD WINAPI GetLOD();
	virtual   DWORD WINAPI GetLevelCount();
	virtual HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	virtual    void WINAPI GenerateMipSubLevels();
	virtual D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType();
    
    



protected:
	IDirect3DTexture9* const m_pActualTexture;

private:
	ULONG m_nRefCount;

};

#endif
