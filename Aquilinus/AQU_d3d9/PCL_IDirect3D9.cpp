/***************************************************************
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
****************************************************************/

#include "PCL_IDirect3D9.h"
#include "PCL_IDirect3DDevice9.h"

/**
* Constructor.
***/
PCL_IDirect3D9::PCL_IDirect3D9(IDirect3D9 *pOriginal, AQU_TransferSite *pTransferSite)
{
    m_pIDirect3D9 = pOriginal;
	m_pTransferSite = pTransferSite;
}

/**
* Empty deconstructor.
***/
PCL_IDirect3D9::~PCL_IDirect3D9(void)
{
}

/**
* Tests riid to return wrapped interfaces.
***/
HRESULT WINAPI PCL_IDirect3D9::QueryInterface(REFIID riid, void** ppvObj)
{
	HRESULT hRes = m_pIDirect3D9->QueryInterface(riid, ppvObj); 

	// TODO !! test riid and return different interfaces
	
	return hRes;
}

/**
* Base functionality.
***/
ULONG WINAPI PCL_IDirect3D9::AddRef(void)
{
    return m_pIDirect3D9->AddRef();
}

/***
* Wrapped Release functionality.
***/
ULONG WINAPI PCL_IDirect3D9::Release(void)
{
    extern PCL_IDirect3D9* g_pPCL_IDirect3D9;

	ULONG nRefCount = m_pIDirect3D9->Release();
	
	if (nRefCount == 0) 
	{
		g_pPCL_IDirect3D9 = NULL;
  	    delete this; 
	}

	return nRefCount;
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
    return m_pIDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

/**
* Base functionality.
***/
UINT WINAPI PCL_IDirect3D9::GetAdapterCount(void)
{
    return m_pIDirect3D9->GetAdapterCount();
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
    return m_pIDirect3D9->GetAdapterIdentifier(Adapter,Flags,pIdentifier);
}

/**
* Base functionality.
***/
UINT WINAPI PCL_IDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
    return m_pIDirect3D9->GetAdapterModeCount(Adapter, Format);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode)
{
    return m_pIDirect3D9->EnumAdapterModes(Adapter,Format,Mode,pMode);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::GetAdapterDisplayMode( UINT Adapter,D3DDISPLAYMODE* pMode)
{
    return m_pIDirect3D9->GetAdapterDisplayMode(Adapter,pMode);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::CheckDeviceType(UINT iAdapter,D3DDEVTYPE DevType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
    return m_pIDirect3D9->CheckDeviceType(iAdapter,DevType,DisplayFormat,BackBufferFormat,bWindowed);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
    return m_pIDirect3D9->CheckDeviceFormat(Adapter,DeviceType,AdapterFormat,Usage,RType,CheckFormat);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
    return m_pIDirect3D9->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed,MultiSampleType,pQualityLevels);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
    return m_pIDirect3D9->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat)
{
    return m_pIDirect3D9->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat);
}

/**
* Base functionality.
***/
HRESULT WINAPI PCL_IDirect3D9::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
    return m_pIDirect3D9->GetDeviceCaps(Adapter,DeviceType,pCaps);
}

/**
* Base functionality.
***/
HMONITOR WINAPI PCL_IDirect3D9::GetAdapterMonitor(UINT Adapter)
{
    return m_pIDirect3D9->GetAdapterMonitor(Adapter);
}

/**
* Create and return wrapped device.
***/
HRESULT WINAPI PCL_IDirect3D9::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
    // declare device extern
	extern PCL_IDirect3DDevice9* g_pPCL_IDirect3DDevice9;

	// create actual device
	HRESULT hres = m_pIDirect3D9->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	// create and return wrapped (or proxy) device
	g_pPCL_IDirect3DDevice9 = new PCL_IDirect3DDevice9(*ppReturnedDeviceInterface, m_pTransferSite);
	*ppReturnedDeviceInterface = g_pPCL_IDirect3DDevice9;

	return hres; 
}
  
