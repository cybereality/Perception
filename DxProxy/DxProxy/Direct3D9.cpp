/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

BaseDirect3D9::BaseDirect3D9(IDirect3D9* pD3D) :
	m_pD3D(pD3D),
	m_nRefCount(1)
{
}

BaseDirect3D9::~BaseDirect3D9()
{
	if(m_pD3D)
		m_pD3D->Release();
}

HRESULT WINAPI BaseDirect3D9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pD3D->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3D9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3D9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}

HRESULT WINAPI BaseDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

UINT BaseDirect3D9::GetAdapterCount()
{
	return m_pD3D->GetAdapterCount();
}

HRESULT WINAPI BaseDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags,
	D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT WINAPI BaseDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pD3D->GetAdapterModeCount(Adapter, Format);
}

HRESULT WINAPI BaseDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode,
	D3DDISPLAYMODE* pMode)
{
	return m_pD3D->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT WINAPI BaseDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pD3D->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT WINAPI BaseDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType,
	D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pD3D->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT WINAPI BaseDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType,
	D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType,
		CheckFormat);
}

HRESULT WINAPI BaseDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType,
	D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,
	DWORD* pQualityLevels)
{
	return m_pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed,
		MultiSampleType, pQualityLevels);
}

HRESULT WINAPI BaseDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType,
	D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat,
		DepthStencilFormat);
}

HRESULT WINAPI BaseDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType,
	D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pD3D->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT WINAPI BaseDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
	HRESULT hResult = m_pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);

	if(FAILED(hResult))
		return hResult;

	return hResult;
}

HMONITOR WINAPI BaseDirect3D9::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3D->GetAdapterMonitor(Adapter);
}

HRESULT WINAPI BaseDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
	DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,
	IDirect3DDevice9** ppReturnedDeviceInterface)
{
	// Create real interface
	HRESULT hResult = m_pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags,
		pPresentationParameters, ppReturnedDeviceInterface);
	if(FAILED(hResult))
		return hResult;

	OutputDebugString("created device success\n");

	// load configuration file
	ProxyHelper helper = ProxyHelper();
	ProxyHelper::ProxyConfig cfg;
	if(!helper.LoadConfig(cfg))
		return hResult;

	OutputDebugString("loaded config success\n");

	if(cfg.stereo_mode == StereoView::DISABLED)
		return hResult;

	OutputDebugString("stereo mode not disabled\n");

	char buf[32];
	LPCSTR psz = NULL;

	wsprintf(buf,"Config type: %d", cfg.game_type);
	psz = buf;
	OutputDebugString(psz);
	OutputDebugString("\n");

	// Create and return proxy
	*ppReturnedDeviceInterface = D3DProxyDeviceFactory::Get(cfg, *ppReturnedDeviceInterface);
	return hResult;
}