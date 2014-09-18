/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3D9.h> and
Class <D3D9ProxyDirect3D> :
Copyright (C) 2012 Andres Hernandez

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
#include <cConfig.h>

class D3D9ProxyDirect3D : public IDirect3D9 {
public:
	D3D9ProxyDirect3D(IDirect3D9* pD3D , cConfig& cfg );
	virtual ~D3D9ProxyDirect3D();

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	ULONG   WINAPI AddRef();
	ULONG   WINAPI Release();

	/*** IDirect3D9 methods ***/
	HRESULT	 WINAPI RegisterSoftwareDevice(void* pInitializeFunction);
	UINT     WINAPI GetAdapterCount();
	HRESULT	 WINAPI GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier);
	UINT     WINAPI GetAdapterModeCount(UINT Adapter, D3DFORMAT Format);
	HRESULT	 WINAPI EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
	HRESULT	 WINAPI GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode);
	HRESULT	 WINAPI CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
	HRESULT	 WINAPI CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
	HRESULT	 WINAPI CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
	HRESULT	 WINAPI CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
	HRESULT	 WINAPI CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,	D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
	HRESULT	 WINAPI GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
	HMONITOR WINAPI GetAdapterMonitor(UINT Adapter);
	HRESULT	 WINAPI CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,	DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);

private:
	cConfig&    config;
	IDirect3D9* m_pD3D;
	ULONG       m_nRefCount;
};
