/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_IDirect3D9.cpp> and
Class <PCL_IDirect3D9> :
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
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
  
