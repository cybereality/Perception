/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3D9.cpp> and
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

#include "D3D9ProxyDirect3D.h"
#include "D3DProxyDevice.h"
#include "D3DProxyDeviceDebug.h"
#include "D3DProxyDeviceEgo.h"
#include "D3DProxyDeviceAdv.h"
#include "DataGatherer.h"
/**
* Constructor. 
* @param pD3D Imbed actual Direct3D object. 
***/
D3D9ProxyDirect3D::D3D9ProxyDirect3D(IDirect3D9* pD3D , IDirect3D9Ex* pD3DEx , cConfig& cfg ) :
	actual(pD3D),
	actualEx(pD3DEx),
	m_nRefCount(1) ,
	config(cfg)
{
}

/**
* Destructor. 
* Releases embedded object m_pD3D. 
***/
D3D9ProxyDirect3D::~D3D9ProxyDirect3D()
{
	if(actual)
		actual->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI D3D9ProxyDirect3D::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return actual->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI D3D9ProxyDirect3D::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI D3D9ProxyDirect3D::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}


/**
* Create D3D device proxy. 
* First it creates the device, then it loads the game configuration
* calling the ProxyHelper class. Last it creates and returns the
* device proxy calling D3DProxyDeviceFactory::Get().
***/
METHOD_IMPL( HRESULT  , WINAPI , D3D9ProxyDirect3D , ProxyCreateDevice , UINT , Adapter , D3DDEVTYPE , DeviceType , HWND , hFocusWindow , DWORD , BehaviorFlags , D3DPRESENT_PARAMETERS* , pPresentationParameters , D3DDISPLAYMODEEX* , pFullscreenDisplayMode , IDirect3DDevice9** , ppReturnedDeviceInterface , IDirect3DDevice9Ex** , ppReturnedDeviceInterfaceEx )
	// Create real interface

	IDirect3DDevice9*   dev   = 0;
	IDirect3DDevice9Ex* devEx = 0;
	HRESULT             result;

	if( ppReturnedDeviceInterface ){
		result = actual  ->CreateDevice  (Adapter, DeviceType, hFocusWindow, BehaviorFlags,pPresentationParameters, &dev );
	}else
	if( ppReturnedDeviceInterfaceEx ){
		result = actualEx->CreateDeviceEx(Adapter, DeviceType, hFocusWindow, BehaviorFlags,pPresentationParameters,pFullscreenDisplayMode, &devEx );
		dev    = devEx;
	}else{
		return D3DERR_INVALIDCALL;
	}

	if( FAILED(result) ){
		return result;
	}


	OutputDebugStringA("[OK] Normal D3D device created\n");

	char buf[64];
	sprintf_s(buf, "Number of back buffers = %d\n", pPresentationParameters->BackBufferCount);
	OutputDebugStringA(buf);
	sprintf_s(buf, "Format of back buffers = %x\n", pPresentationParameters->BackBufferFormat);
	OutputDebugStringA(buf);

	// for debug reasons, output the d3dswapeffect parameter
	switch (pPresentationParameters->SwapEffect)
	{
	case D3DSWAPEFFECT::D3DSWAPEFFECT_COPY:
		OutputDebugStringA("D3DSWAPEFFECT_COPY");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD:
		OutputDebugStringA("D3DSWAPEFFECT_DISCARD");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_FLIP:
		OutputDebugStringA("D3DSWAPEFFECT_FLIP");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_FLIPEX:
		OutputDebugStringA("D3DSWAPEFFECT_FLIPEX");
		break;
	case D3DSWAPEFFECT::D3DSWAPEFFECT_OVERLAY:
		OutputDebugStringA("D3DSWAPEFFECT_OVERLAY");
		break;
	default:
		char buf[256];
		sprintf_s(buf, 256, "D3DPRESENT_PARAMETERS::SwapEffect %x", pPresentationParameters->SwapEffect);
		OutputDebugStringA(buf);
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

	if( config.shader.isEmpty() ) {
		return result;
	}


	D3DProxyDevice* newDev;

	// if game profile = game type + 10000 -> return DataGatherer (=shader analyzer)
	if( config.game_type == D3DProxyDevice::GAMEBRYO ){
		newDev = new D3DProxyDeviceAdv(dev, devEx , this , config );
	}else
	if( config.game_type == D3DProxyDevice::DEBUG_LOG_FILE ){
		newDev = new D3DProxyDeviceDebug(dev, devEx , this , config );
	}else
	if( config.game_type > 10000 ){
		newDev = new DataGatherer(dev, devEx , this , config );
	}else{
		newDev = new D3DProxyDevice(dev, devEx , this , config );
	}

	if( ppReturnedDeviceInterface ){
		*ppReturnedDeviceInterface = newDev;
	}

	if( ppReturnedDeviceInterfaceEx ){
		*ppReturnedDeviceInterfaceEx = newDev;
	}

	return result;
}



METHOD_IMPL( HRESULT  , WINAPI , D3D9ProxyDirect3D , CreateDevice , UINT , Adapter , D3DDEVTYPE , DeviceType , HWND , hFocusWindow , DWORD , BehaviorFlags , D3DPRESENT_PARAMETERS* , pPresentationParameters , IDirect3DDevice9** , ppReturnedDeviceInterface )
	return ProxyCreateDevice( Adapter , DeviceType , hFocusWindow , BehaviorFlags , pPresentationParameters, 0 , ppReturnedDeviceInterface , 0 );
}

METHOD_IMPL( HRESULT  , WINAPI , D3D9ProxyDirect3D , CreateDeviceEx , UINT , Adapter , D3DDEVTYPE , DeviceType , HWND , hFocusWindow , DWORD , BehaviorFlags , D3DPRESENT_PARAMETERS* , pPresentationParameters , D3DDISPLAYMODEEX* , pFullscreenDisplayMode , IDirect3DDevice9Ex** , ppReturnedDeviceInterface )
	return ProxyCreateDevice( Adapter , DeviceType , hFocusWindow , BehaviorFlags , pPresentationParameters, pFullscreenDisplayMode , 0 ,  ppReturnedDeviceInterface );
}




METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , RegisterSoftwareDevice , void* , pInitializeFunction)
METHOD_THRU   ( UINT     , WINAPI , D3D9ProxyDirect3D , GetAdapterCount)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , GetAdapterIdentifier , UINT , Adapter , DWORD , Flags , D3DADAPTER_IDENTIFIER9* , pIdentifier)
METHOD_THRU   ( UINT     , WINAPI , D3D9ProxyDirect3D , GetAdapterModeCount , UINT , Adapter , D3DFORMAT , Format)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , EnumAdapterModes , UINT , Adapter , D3DFORMAT , Format , UINT , Mode , D3DDISPLAYMODE* , pMode)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , GetAdapterDisplayMode , UINT , Adapter , D3DDISPLAYMODE* , pMode)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , CheckDeviceType , UINT , Adapter , D3DDEVTYPE , DevType , D3DFORMAT , AdapterFormat , D3DFORMAT , BackBufferFormat , BOOL , bWindowed)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , CheckDeviceFormat , UINT , Adapter , D3DDEVTYPE , DeviceType , D3DFORMAT , AdapterFormat , DWORD , Usage , D3DRESOURCETYPE , RType , D3DFORMAT , CheckFormat)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , CheckDeviceMultiSampleType , UINT , Adapter , D3DDEVTYPE , DeviceType , D3DFORMAT , SurfaceFormat , BOOL , Windowed , D3DMULTISAMPLE_TYPE , MultiSampleType , DWORD* , pQualityLevels)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , CheckDepthStencilMatch , UINT , Adapter , D3DDEVTYPE , DeviceType , D3DFORMAT , AdapterFormat , D3DFORMAT , RenderTargetFormat , D3DFORMAT , DepthStencilFormat)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , CheckDeviceFormatConversion , UINT , Adapter , D3DDEVTYPE , DeviceType , D3DFORMAT , SourceFormat , D3DFORMAT , TargetFormat)
METHOD_THRU   ( HRESULT  , WINAPI , D3D9ProxyDirect3D , GetDeviceCaps , UINT , Adapter , D3DDEVTYPE , DeviceType , D3DCAPS9* , pCaps)
METHOD_THRU   ( HMONITOR , WINAPI , D3D9ProxyDirect3D , GetAdapterMonitor , UINT , Adapter)
METHOD_THRU_EX( UINT     , WINAPI , D3D9ProxyDirect3D , GetAdapterModeCountEx , UINT , Adapter , CONST D3DDISPLAYMODEFILTER* , pFilter )
METHOD_THRU_EX( HRESULT  , WINAPI , D3D9ProxyDirect3D , EnumAdapterModesEx , UINT , Adapter , CONST D3DDISPLAYMODEFILTER* , pFilter , UINT , Mode , D3DDISPLAYMODEEX* , pMode)
METHOD_THRU_EX( HRESULT  , WINAPI , D3D9ProxyDirect3D , GetAdapterDisplayModeEx , UINT , Adapter , D3DDISPLAYMODEEX* , pMode , D3DDISPLAYROTATION* , pRotation)

METHOD_THRU_EX( HRESULT  , WINAPI , D3D9ProxyDirect3D , GetAdapterLUID , UINT , Adapter , LUID* , pLUID)

/*

HRESULT WINAPI D3D9ProxyDirect3D::RegisterSoftwareDevice(void* pInitializeFunction)
UINT    D3D9ProxyDirect3D::GetAdapterCount()
HRESULT WINAPI D3D9ProxyDirect3D::GetAdapterIdentifier(UINT Adapter, DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
UINT    WINAPI D3D9ProxyDirect3D::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
HRESULT WINAPI D3D9ProxyDirect3D::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode,D3DDISPLAYMODE* pMode)
HRESULT WINAPI D3D9ProxyDirect3D::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
HRESULT WINAPI D3D9ProxyDirect3D::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType,D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
HRESULT WINAPI D3D9ProxyDirect3D::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
HRESULT WINAPI D3D9ProxyDirect3D::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
HRESULT WINAPI D3D9ProxyDirect3D::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
HRESULT WINAPI D3D9ProxyDirect3D::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
HRESULT WINAPI D3D9ProxyDirect3D::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
HMONITOR WINAPI D3D9ProxyDirect3D::GetAdapterMonitor(UINT Adapter)
	UINT    WINAPI GetAdapterModeCountEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter );
    HRESULT WINAPI EnumAdapterModesEx(UINT Adapter,CONST D3DDISPLAYMODEFILTER* pFilter,UINT Mode,D3DDISPLAYMODEEX* pMode);
    HRESULT WINAPI GetAdapterDisplayModeEx(UINT Adapter,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation);
    HRESULT WINAPI CreateDeviceEx(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX* pFullscreenDisplayMode,IDirect3DDevice9Ex** ppReturnedDeviceInterface);
    HRESULT WINAPI GetAdapterLUID(UINT Adapter,LUID * pLUID);


METHOD_IMPL
*/