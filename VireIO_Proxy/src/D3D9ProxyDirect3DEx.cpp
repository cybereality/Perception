/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3D9Ex.cpp> and
Class <D3D9ProxyDirect3DEx> :
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

#include "Direct3D9Ex.h"
#include "D3DProxyDevice.h"
#include "D3DProxyDeviceFactory.h"

/**
* Constructor. 
* @param pD3D Imbed actual Direct3D object. 
***/
D3D9ProxyDirect3DEx::D3D9ProxyDirect3DEx(IDirect3D9Ex* pD3DEx , cConfig& cfg ) :
	m_pD3DEx(pD3DEx),
	m_nRefCount(1),
	config(cfg)
{
}

/**
* Destructor. 
* Releases embedded object m_pD3DEx. 
***/
D3D9ProxyDirect3DEx::~D3D9ProxyDirect3DEx()
{
	if(m_pD3DEx)
		m_pD3DEx->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pD3DEx->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI D3D9ProxyDirect3DEx::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI D3D9ProxyDirect3DEx::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}

/**
* Base GetAdapterCount functionality.
***/
UINT D3D9ProxyDirect3DEx::GetAdapterCount()
{
	return m_pD3DEx->GetAdapterCount();
}

/**
* Base GetAdapterIdentifier functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::GetAdapterIdentifier(UINT Adapter, DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pD3DEx->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

/**
* Base GetAdapterModeCount functionality.
***/
UINT WINAPI D3D9ProxyDirect3DEx::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pD3DEx->GetAdapterModeCount(Adapter, Format);
}

/**
* Base GetAdapterDisplayMode functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pD3DEx->GetAdapterDisplayMode(Adapter, pMode);
}

/**
* Base CheckDeviceType functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType,D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pD3DEx->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

/**
* Base CheckDeviceFormat functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3DEx->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType,
		CheckFormat);
}

/**
* Base CheckDeviceMultiSampleType functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
	return m_pD3DEx->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed,
		MultiSampleType, pQualityLevels);
}

/**
* Base CheckDepthStencilMatch functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3DEx->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat,
		DepthStencilFormat);
}

/**
* Base CheckDeviceFormatConversion functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pD3DEx->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

/**
* Base GetDeviceCaps functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{	
	return m_pD3DEx->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

/**
* Base GetAdapterMonitor functionality.
***/
HMONITOR WINAPI D3D9ProxyDirect3DEx::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3DEx->GetAdapterMonitor(Adapter);
}

/**
* Create D3D device ex proxy. 
* First it creates the device, then it loads the game configuration
* calling the ProxyHelper class. Last it creates and returns the
* device proxy calling D3DProxyDeviceFactory::Get().
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
	// Create real interface
	HRESULT hResult = m_pD3DEx->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags,
		pPresentationParameters, ppReturnedDeviceInterface);
	if(FAILED(hResult))
		return hResult;

	if( config.shader.isEmpty() ){
		OutputDebugStringA("[WARN] stereo_mode == disabled. Returning normal D3DDevice. Vireio will not be active.\n");
		return hResult;
	}

	// Create and return proxy TODO !!
	//*ppReturnedDeviceInterface = D3DProxyDeviceFactory::Get(cfg, *ppReturnedDeviceInterface, this);

	return hResult;
}

/**
* Base GetAdapterModeCountEx functionality.
***/
UINT WINAPI D3D9ProxyDirect3DEx::GetAdapterModeCountEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter )
{
	return m_pD3DEx->GetAdapterModeCountEx(Adapter, pFilter);
}

/**
* Base EnumAdapterModesEx functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::EnumAdapterModesEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter,UINT Mode,D3DDISPLAYMODEEX* pMode)
{	
	return m_pD3DEx->EnumAdapterModesEx(Adapter, pFilter, Mode, pMode);
}

/**
* Base GetAdapterDisplayModeEx functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::GetAdapterDisplayModeEx(UINT Adapter,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation)
{	
	return m_pD3DEx->GetAdapterDisplayModeEx(Adapter, pMode, pRotation);
}

/**
* Base CreateDeviceEx functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::CreateDeviceEx(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX* pFullscreenDisplayMode,IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{	
	return m_pD3DEx->CreateDeviceEx( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
}

/**
* Base GetAdapterLUID functionality.
***/
HRESULT WINAPI D3D9ProxyDirect3DEx::GetAdapterLUID(UINT Adapter,LUID * pLUID)
{	
	return m_pD3DEx->GetAdapterLUID(Adapter, pLUID);
}
