/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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
#ifndef DCL_IDIRECT3DCUBETEXTURE9_CLASS
#define DCL_IDIRECT3DCUBETEXTURE9_CLASS

#include <d3d9.h>
#include "VMT_IDirect3DCubeTexture9.h"
#include "AQU_TransferSite.h"

/**
* IDirect3DCubeTexture9 detour class.
***/
class DCL_IDirect3DCubeTexture9
{
public:
	DCL_IDirect3DCubeTexture9(AQU_TransferSite* pcTransferSite);
	~DCL_IDirect3DCubeTexture9();

	/*** IUnknown methods ***/
	HRESULT              WINAPI QueryInterface           (IDirect3DCubeTexture9* pcThis, REFIID riid, void** ppvObj);
	ULONG                WINAPI AddRef                   (IDirect3DCubeTexture9* pcThis);
	ULONG                WINAPI Release                  (IDirect3DCubeTexture9* pcThis);

	/*** IDirect3DResource9 methods ***/
	HRESULT              WINAPI GetDevice                (IDirect3DCubeTexture9* pcThis, IDirect3DDevice9** ppDevice);
	HRESULT              WINAPI SetPrivateData           (IDirect3DCubeTexture9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
	HRESULT              WINAPI GetPrivateData           (IDirect3DCubeTexture9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData);
	HRESULT              WINAPI FreePrivateData          (IDirect3DCubeTexture9* pcThis, REFGUID refguid);
	DWORD                WINAPI SetPriority              (IDirect3DCubeTexture9* pcThis, DWORD PriorityNew);
	DWORD                WINAPI GetPriority              (IDirect3DCubeTexture9* pcThis);
	void                 WINAPI PreLoad                  (IDirect3DCubeTexture9* pcThis);
	D3DRESOURCETYPE      WINAPI GetType                  (IDirect3DCubeTexture9* pcThis);

	/*** IDirect3DBaseTexture9 methods ***/
	DWORD                WINAPI SetLOD                   (IDirect3DCubeTexture9* pcThis, DWORD LODNew);
	DWORD                WINAPI GetLOD                   (IDirect3DCubeTexture9* pcThis);
	DWORD                WINAPI GetLevelCount            (IDirect3DCubeTexture9* pcThis);
	HRESULT              WINAPI SetAutoGenFilterType     (IDirect3DCubeTexture9* pcThis, D3DTEXTUREFILTERTYPE FilterType);
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType     (IDirect3DCubeTexture9* pcThis);
	void                 WINAPI GenerateMipSubLevels     (IDirect3DCubeTexture9* pcThis);

	/*** IDirect3DCubeTexture methods ***/
	HRESULT              WINAPI GetLevelDesc             (IDirect3DCubeTexture9* pcThis, UINT Level, D3DSURFACE_DESC *pDesc);
	HRESULT              WINAPI GetCubeMapSurface        (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface);
	HRESULT              WINAPI LockRect                 (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	HRESULT              WINAPI UnlockRect               (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level);
	HRESULT              WINAPI AddDirtyRect             (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect);

	/*** DCL_IDirect3DCubeTexture9 public methods ***/
	HRESULT                     SetSuperFunctionPointer  (VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT                     SetSuperFunctionPointer  (VMT_IDIRECT3DRESOURCE9::VMT_IDirect3DResource9 eFunc, UINT_PTR dwFunc);
	HRESULT                     SetSuperFunctionPointer  (VMT_IDIRECT3DBASETEXTURE9::VMT_IDirect3DBaseTexture9 eFunc, UINT_PTR dwFunc);
	HRESULT                     SetSuperFunctionPointer  (VMT_IDIRECT3DCUBETEXTURE9::VMT_IDirect3DCubeTexture9 eFunc, UINT_PTR dwFunc);
	HRESULT                     SetSuperFunctionPointers (PUINT_PTR pVMTable);

	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
};

#endif