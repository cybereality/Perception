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
#ifndef DCL_IDIRECT3DQUERY9_SUPER
#define DCL_IDIRECT3DQUERY9_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT      (WINAPI* D3D9_QueryInterface) (IDirect3DQuery9* pcThis,REFIID riid, void** ppvObj);
typedef ULONG        (WINAPI* D3D9_AddRef)         (IDirect3DQuery9* pcThis);
typedef ULONG        (WINAPI* D3D9_Release)        (IDirect3DQuery9* pcThis);

/*** IDirect3DQuery9 method prototypes ***/
typedef HRESULT      (WINAPI* D3D9_GetDevice)      (IDirect3DQuery9* pcThis, IDirect3DDevice9** ppDevice);
typedef D3DQUERYTYPE (WINAPI* D3D9_GetType)        (IDirect3DQuery9* pcThis);
typedef DWORD        (WINAPI* D3D9_GetDataSize)    (IDirect3DQuery9* pcThis);
typedef HRESULT      (WINAPI* D3D9_Issue)          (IDirect3DQuery9* pcThis, DWORD dwIssueFlags);
typedef HRESULT      (WINAPI* D3D9_GetData)        (IDirect3DQuery9* pcThis, void* pData,DWORD dwSize,DWORD dwGetDataFlags);

/*** IUnknown super methods ***/
D3D9_QueryInterface D3D9_IDirect3DQuery9_QueryInterface_Super;
D3D9_AddRef         D3D9_IDirect3DQuery9_AddRef_Super;
D3D9_Release        D3D9_IDirect3DQuery9_Release_Super;

/*** IDirect3DQuery9 super methods ***/
D3D9_GetDevice      D3D9_IDirect3DQuery9_GetDevice_Super;
D3D9_GetType        D3D9_IDirect3DQuery9_GetType_Super;
D3D9_GetDataSize    D3D9_IDirect3DQuery9_GetDataSize_Super;
D3D9_Issue          D3D9_IDirect3DQuery9_Issue_Super;
D3D9_GetData        D3D9_IDirect3DQuery9_GetData_Super;

#endif