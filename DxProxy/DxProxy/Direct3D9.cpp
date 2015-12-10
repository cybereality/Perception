/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3D9.cpp> and
Class <BaseDirect3D9> :
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

#include "Direct3D9.h"
#include "Direct3DDevice9.h"
#include "Main.h"
#include "D3DProxyDeviceFactory.h"
#include "Vireio.h"
#include <cstdio>
#include <windows.h>

using namespace vireio;



/**
* Constructor. 
* @param pD3D Imbed actual Direct3D object. 
***/
BaseDirect3D9::BaseDirect3D9(IDirect3D9* pD3D) :
	m_pD3D(pD3D),
	m_nRefCount(1),
	m_perceptionRunning(false)
{
	cfg = new ProxyConfig();
	
#ifdef _WIN64
	if(!ProxyHelper::IsProcessRunning("Perception_x64.exe")) {
#else
	if (!ProxyHelper::IsProcessRunning("Perception_Win32.exe"))	{
#endif
		OutputDebugString("[WARN] Perception Application is not running. Vireio will not be active.\n");
	}
	else
	{
		//record that the perception app is running
		m_perceptionRunning = true;

		// load configuration file
		ProxyHelper helper;
		ProxyHelper::OculusProfile oculusProfile;
		configLoaded = true;
		if(!helper.LoadConfig(*cfg, oculusProfile)) {
			OutputDebugString("[ERR] Config loading failed, config could not be loaded. Returning normal D3DDevice. Vireio will not be active.\n");
			configLoaded = false;
		}

		if(!helper.LoadUserConfig(userConfig)) {
			OutputDebugString("[ERR] User config loading failed, user config could not be loaded. Returning normal D3DDevice. Vireio will not be active.\n");
			configLoaded = false;
		}

		if (cfg->display_adapter > m_pD3D->GetAdapterCount() ||
			!configLoaded)
		{
			OutputDebugString("[ERR] Invalid Display Adapter ID - Using Primary adapter\n");
			cfg->display_adapter = D3DADAPTER_DEFAULT;
		}
	}
}

/**
* Destructor. 
* Releases embedded object m_pD3D. 
***/
BaseDirect3D9::~BaseDirect3D9()
{
	if(cfg) {
		delete cfg;
		cfg = NULL;
	}
	if(m_pD3D)
		m_pD3D->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3D9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pD3D->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3D9::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3D9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}

/**
* Base RegisterSoftwareDevice functionality.
***/
HRESULT WINAPI BaseDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

/**
* Base GetAdapterCount functionality.
***/
UINT BaseDirect3D9::GetAdapterCount()
{
	return m_pD3D->GetAdapterCount();
}

/**
* Base GetAdapterIdentifier functionality.
***/
HRESULT WINAPI BaseDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pD3D->GetAdapterIdentifier(m_perceptionRunning ? cfg->display_adapter : Adapter, Flags, pIdentifier);
}

/**
* Base GetAdapterModeCount functionality.
***/
UINT WINAPI BaseDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pD3D->GetAdapterModeCount(m_perceptionRunning ? cfg->display_adapter : Adapter, Format);
}

/**
* Base EnumAdapterModes functionality.
***/
HRESULT WINAPI BaseDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode,D3DDISPLAYMODE* pMode)
{
	return m_pD3D->EnumAdapterModes(m_perceptionRunning ? cfg->display_adapter : Adapter, Format, Mode, pMode);
}

/**
* Base GetAdapterDisplayMode functionality.
***/
HRESULT WINAPI BaseDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pD3D->GetAdapterDisplayMode(m_perceptionRunning ? cfg->display_adapter : Adapter, pMode);
}

/**
* Base CheckDeviceType functionality.
***/
HRESULT WINAPI BaseDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType,D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pD3D->CheckDeviceType(m_perceptionRunning ? cfg->display_adapter : Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

/**
* Base CheckDeviceFormat functionality.
***/
HRESULT WINAPI BaseDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3D->CheckDeviceFormat(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, AdapterFormat, Usage, RType,
		CheckFormat);
}

/**
* Base CheckDeviceMultiSampleType functionality.
***/
HRESULT WINAPI BaseDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
	return m_pD3D->CheckDeviceMultiSampleType(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, SurfaceFormat, Windowed,
		MultiSampleType, pQualityLevels);
}

/**
* Base CheckDepthStencilMatch functionality.
***/
HRESULT WINAPI BaseDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3D->CheckDepthStencilMatch(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, AdapterFormat, RenderTargetFormat,
		DepthStencilFormat);
}

/**
* Base CheckDeviceFormatConversion functionality.
***/
HRESULT WINAPI BaseDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pD3D->CheckDeviceFormatConversion(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, SourceFormat, TargetFormat);
}

/**
* Base GetDeviceCaps functionality.
***/
HRESULT WINAPI BaseDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{	
	return m_pD3D->GetDeviceCaps(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, pCaps);
}

/**
* Base GetAdapterMonitor functionality.
***/
HMONITOR WINAPI BaseDirect3D9::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3D->GetAdapterMonitor(m_perceptionRunning ? cfg->display_adapter : Adapter);
}

/**
* Create D3D device proxy. 
* First it creates the device, then it loads the game configuration
* calling the ProxyHelper class. Last it creates and returns the
* device proxy calling D3DProxyDeviceFactory::Get().
***/
HRESULT WINAPI BaseDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
	OutputDebugString("BaseDirect3D9::CreateDevice\n");

	HRESULT hResult = S_OK;
	IDirect3D9Ex *pDirect3D9Ex = NULL;
	if (SUCCEEDED(m_pD3D->QueryInterface(IID_IDirect3D9Ex, reinterpret_cast<void**>(&pDirect3D9Ex))))
	{
		//Force no VSYNC in DX9Ex mode
		D3DPRESENT_PARAMETERS presentationParameters = *pPresentationParameters;
		presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hResult = pDirect3D9Ex->CreateDevice(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, hFocusWindow, BehaviorFlags,
			&presentationParameters, ppReturnedDeviceInterface);

		if (*ppReturnedDeviceInterface)
		{
			IDirect3DDevice9Ex *pDirect3DDevice9Ex = NULL;
			if (SUCCEEDED((*ppReturnedDeviceInterface)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&pDirect3DDevice9Ex))))
			{
				if (SUCCEEDED(hResult))
					OutputDebugString("[OK] Direct3DDevice9Ex created\n");
	
				pDirect3DDevice9Ex->Release();
			}
			else
			{
				OutputDebugString("[OK] Normal D3D device created\n");
			}
		}
	}
	else
	{
		OutputDebugString("F\n");
		hResult = m_pD3D->CreateDevice(m_perceptionRunning ? cfg->display_adapter : Adapter, DeviceType, hFocusWindow, BehaviorFlags,
		pPresentationParameters, ppReturnedDeviceInterface);
		if (SUCCEEDED(hResult))
			OutputDebugString("[OK] Normal D3D device created\n");
	}

	if(FAILED(hResult))
	{
		OutputDebugString("[ERROR] No D3DDevice9 Created\n");
		return hResult;
	}


	debugf("Number of back buffers = %d\n", pPresentationParameters->BackBufferCount);
	debugf("Format of back buffers = %x\n", pPresentationParameters->BackBufferFormat);

	// for debug reasons, output the d3dswapeffect parameter
	switch (pPresentationParameters->SwapEffect)
	{
	case D3DSWAPEFFECT::D3DSWAPEFFECT_COPY:
		OutputDebugString("D3DSWAPEFFECT_COPY");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD:
		OutputDebugString("D3DSWAPEFFECT_DISCARD");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_FLIP:
		OutputDebugString("D3DSWAPEFFECT_FLIP");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_FLIPEX:
		OutputDebugString("D3DSWAPEFFECT_FLIPEX");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_OVERLAY:
		OutputDebugString("D3DSWAPEFFECT_OVERLAY");
		break;
	default:
		debugf("D3DPRESENT_PARAMETERS::SwapEffect %x", pPresentationParameters->SwapEffect);
		break;
	}

	// if no back buffer present (=D3DFMT_UNKNOWN), return base device - TODO !!! 
	// DOES NOT WORK since the shaders will not be wrapped by the base device
	// create a special device for that.....
	/*if ((pPresentationParameters->BackBufferCount == 0) && (pPresentationParameters->BackBufferFormat == D3DFORMAT::D3DFMT_UNKNOWN))
	{
		*ppReturnedDeviceInterface = new BaseDirect3DDevice9(*ppReturnedDeviceInterface, this);
		return hResult;
	}*/

	if (!configLoaded)
		return hResult;

	OutputDebugString("[OK] Config loading - OK\n");

	if(cfg->stereo_mode == StereoView::DISABLED) {
		OutputDebugString("[WARN] stereo_mode == disabled. Returning normal D3DDevice. Vireio will not be active.\n");
		return hResult;
	}

	if(!m_perceptionRunning) {
		OutputDebugString("[WARN] Perception application is not running. Returning normal D3DDevice. Vireio will not be active.\n");
		return hResult;
	}

	OutputDebugString("[OK] Stereo mode is enabled.\n");

	debugf("Config type: %s", cfg->game_type.c_str());
	debugf("Stereo: %i", userConfig.mode);

	// Create and return proxy
	*ppReturnedDeviceInterface = D3DProxyDeviceFactory::Get(*cfg, userConfig, *ppReturnedDeviceInterface, this);

	OutputDebugString("[OK] Vireio D3D device created.\n");

	return hResult;
}