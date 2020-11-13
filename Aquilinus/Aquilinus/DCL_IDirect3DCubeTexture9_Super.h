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
#ifndef DCL_IDIRECT3DCUBETEXTURE9_SUPER
#define DCL_IDIRECT3DCUBETEXTURE9_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT              (WINAPI* D3D9_QueryInterface)       (IDirect3DCubeTexture9* pcThis,REFIID riid, void** ppvObj);
typedef ULONG                (WINAPI* D3D9_AddRef)               (IDirect3DCubeTexture9* pcThis);
typedef ULONG                (WINAPI* D3D9_Release)              (IDirect3DCubeTexture9* pcThis);

/*** IDirect3DResource9 method prototypes ***/
typedef HRESULT              (WINAPI* D3D9_GetDevice)            (IDirect3DCubeTexture9* pcThis, IDirect3DDevice9** ppDevice);
typedef HRESULT              (WINAPI* D3D9_SetPrivateData)       (IDirect3DCubeTexture9* pcThis, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
typedef HRESULT              (WINAPI* D3D9_GetPrivateData)       (IDirect3DCubeTexture9* pcThis, REFGUID refguid,void* pData,DWORD* pSizeOfData);
typedef HRESULT              (WINAPI* D3D9_FreePrivateData)      (IDirect3DCubeTexture9* pcThis, REFGUID refguid);
typedef DWORD                (WINAPI* D3D9_SetPriority)          (IDirect3DCubeTexture9* pcThis, DWORD PriorityNew);
typedef DWORD                (WINAPI* D3D9_GetPriority)          (IDirect3DCubeTexture9* pcThis);
typedef void                 (WINAPI* D3D9_PreLoad)              (IDirect3DCubeTexture9* pcThis);
typedef D3DRESOURCETYPE      (WINAPI* D3D9_GetType)              (IDirect3DCubeTexture9* pcThis);

/*** IDirect3DBaseTexture9 method prototypes ***/
typedef DWORD                (WINAPI* D3D9_SetLOD)               (IDirect3DCubeTexture9* pcThis, DWORD LODNew);
typedef DWORD                (WINAPI* D3D9_GetLOD)               (IDirect3DCubeTexture9* pcThis);
typedef DWORD                (WINAPI* D3D9_GetLevelCount)        (IDirect3DCubeTexture9* pcThis);
typedef HRESULT              (WINAPI* D3D9_SetAutoGenFilterType) (IDirect3DCubeTexture9* pcThis, D3DTEXTUREFILTERTYPE FilterType);
typedef D3DTEXTUREFILTERTYPE (WINAPI* D3D9_GetAutoGenFilterType) (IDirect3DCubeTexture9* pcThis);
typedef void                 (WINAPI* D3D9_GenerateMipSubLevels) (IDirect3DCubeTexture9* pcThis);

/*** IDirect3DCubeTexture9 method prototypes ***/
typedef HRESULT              (WINAPI* D3D9_GetLevelDesc)         (IDirect3DCubeTexture9* pcThis, UINT Level, D3DSURFACE_DESC *pDesc);
typedef HRESULT              (WINAPI* D3D9_GetCubeMapSurface)    (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface);
typedef HRESULT              (WINAPI* D3D9_LockRect)             (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
typedef HRESULT              (WINAPI* D3D9_UnlockRect)           (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, UINT Level);
typedef HRESULT              (WINAPI* D3D9_AddDirtyRect)         (IDirect3DCubeTexture9* pcThis, D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect);

/*** IUnknown super methods ***/
D3D9_QueryInterface       D3D9_IDirect3DCubeTexture9_QueryInterface_Super;
D3D9_AddRef               D3D9_IDirect3DCubeTexture9_AddRef_Super;
D3D9_Release              D3D9_IDirect3DCubeTexture9_Release_Super;

/*** IDirect3DResource9 super methods ***/
D3D9_GetDevice            D3D9_IDirect3DCubeTexture9_GetDevice_Super;
D3D9_SetPrivateData       D3D9_IDirect3DCubeTexture9_SetPrivateData_Super;
D3D9_GetPrivateData       D3D9_IDirect3DCubeTexture9_GetPrivateData_Super;
D3D9_FreePrivateData      D3D9_IDirect3DCubeTexture9_FreePrivateData_Super;
D3D9_SetPriority          D3D9_IDirect3DCubeTexture9_SetPriority_Super;
D3D9_GetPriority          D3D9_IDirect3DCubeTexture9_GetPriority_Super;
D3D9_PreLoad              D3D9_IDirect3DCubeTexture9_PreLoad_Super;
D3D9_GetType              D3D9_IDirect3DCubeTexture9_GetType_Super;

/*** IDirect3DBaseTexture9 super methods ***/
D3D9_SetLOD               D3D9_IDirect3DCubeTexture9_SetLOD_Super;
D3D9_GetLOD               D3D9_IDirect3DCubeTexture9_GetLOD_Super;
D3D9_GetLevelCount        D3D9_IDirect3DCubeTexture9_GetLevelCount_Super;
D3D9_SetAutoGenFilterType D3D9_IDirect3DCubeTexture9_SetAutoGenFilterType_Super;
D3D9_GetAutoGenFilterType D3D9_IDirect3DCubeTexture9_GetAutoGenFilterType_Super;
D3D9_GenerateMipSubLevels D3D9_IDirect3DCubeTexture9_GenerateMipSubLevels_Super;

/*** IDirect3DCubeTexture9 super methods ***/
D3D9_GetLevelDesc         D3D9_IDirect3DCubeTexture9_GetLevelDesc_Super;
D3D9_GetCubeMapSurface    D3D9_IDirect3DCubeTexture9_GetCubeMapSurface_Super;
D3D9_LockRect             D3D9_IDirect3DCubeTexture9_LockRect_Super;
D3D9_UnlockRect           D3D9_IDirect3DCubeTexture9_UnlockRect_Super;
D3D9_AddDirtyRect         D3D9_IDirect3DCubeTexture9_AddDirtyRect_Super;

#endif