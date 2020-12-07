/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3D9Ex.h> and
Class <BaseDirect3D9Ex> :
Copyright (C) 2013 Denis Reischl

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

#ifndef DIRECT3D9EX_H_INCLUDED
#define DIRECT3D9EX_H_INCLUDED

#include <d3d9.h>
#include "ProxyHelper.h"


/**
* Direct 3D extended. 
* Overwrites IDirect3D9 and imbeds the actual D3D object pointer in m_pD3D.
* IMPLEMENTED FOR FUTURE USE, NOT COMPLETE !!
*/
class __declspec(dllexport) BaseDirect3D9Ex : public IDirect3D9Ex
{
public:
	BaseDirect3D9Ex(IDirect3D9Ex* pD3DEx);
	virtual ~BaseDirect3D9Ex();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();

	/*** IDirect3D9 methods ***/
	virtual UINT     WINAPI GetAdapterCount();
	virtual HRESULT	 WINAPI GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier);
	virtual UINT     WINAPI GetAdapterModeCount(UINT Adapter, D3DFORMAT Format);    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) PURE;
	virtual HRESULT	 WINAPI GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode);
	virtual HRESULT	 WINAPI CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
	virtual HRESULT	 WINAPI CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);    
	virtual HRESULT	 WINAPI CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
	virtual HRESULT	 WINAPI CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
	virtual HRESULT	 WINAPI CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,	D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
	virtual HRESULT	 WINAPI GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
	virtual HMONITOR WINAPI GetAdapterMonitor(UINT Adapter);
	virtual HRESULT	 WINAPI CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,	DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
	virtual UINT     WINAPI GetAdapterModeCountEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter );
	virtual HRESULT  WINAPI EnumAdapterModesEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter,UINT Mode,D3DDISPLAYMODEEX* pMode);
	virtual HRESULT  WINAPI GetAdapterDisplayModeEx(UINT Adapter,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation);
	virtual HRESULT  WINAPI CreateDeviceEx(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX* pFullscreenDisplayMode,IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	virtual HRESULT  WINAPI GetAdapterLUID(UINT Adapter,LUID * pLUID);

private:
	/**
	* The actual device embedded. 
	***/
	IDirect3D9Ex* m_pD3DEx;
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
};

#endif