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

#ifndef D3DPROXYSTEREOTEXTURE_H_INCLUDED
#define D3DPROXYSTEREOTEXTURE_H_INCLUDED

#include <d3d9.h>
#include <vector>
#include "Direct3DTexture9.h"
#include "D3DProxyStereoSurface.h"


class D3DProxyStereoTexture : public BaseDirect3DTexture9
{
public:
	D3DProxyStereoTexture(IDirect3DTexture9* leftTexture, IDirect3DTexture9* rightTexture);
	virtual ~D3DProxyStereoTexture();

	
	
	// texture methods
	virtual HRESULT WINAPI GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc);
	virtual HRESULT WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel); // if stereo texture then return stereo surface. if not return the normal surface
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
	
    
    
	bool IsStereo();

	IDirect3DSurface9* getMonoSurface(UINT Level);
	IDirect3DSurface9* getLeftSurface(UINT Level);
	IDirect3DSurface9* getRightSurface(UINT Level);

	IDirect3DSurface9* getMonoTexture();
	IDirect3DSurface9* getLeftTexture();
	IDirect3DSurface9* getRightTexture();




protected:
	IDirect3DTexture9* m_pTextureRight;

	std::vector<D3DProxyStereoSurface*> m_levels;

	// Getting a level from a D3D texture increases the ref count on the texture so we need to do that here as well.
	// Releasing a surface that is aquired in this fashion also reduces the ref count on the texture it was aquired from.
	// The reverse applies as well. Releasing the texture would also reduce the ref count on the surface.
	// This behaviour needs to be replicated for the Stereo texture and surface.

	// We also need to maintain stereo wrapped versions of the surfaces that are retrieved from the texture to maintain
	// consistency with the behaviour of D3D. Calling GetSurfaceLevel repeatedly will return a pointer to the same
	// IDirect3DSurface9* even if mip levels are regenerated inbetween. This means we can't just return a new 
	// StereoSurface reference everytime wrapping the current surfaces returned from Direct3D. If we did the pointers
	// would no longer be referncing the same object which would be inconsistent with Direct3D behaviour.

	// Given that we need to maintain these stereo surfaces we also need to know when they are released to Release
	// them from the maintained list so that the underlying surfaces' are released as well.

	// tldr; Stereo Textures need to maintain references to the Stereo Surfaces they (contain?) and vice versa. (This is needed for proper GetContainer implementation on surfaces as well)
	//       Each has to notify the other if their ref count reaches 0.

};

#endif
