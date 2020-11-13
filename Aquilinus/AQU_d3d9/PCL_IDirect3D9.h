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

#include "d3d9.h"
#include "..\Aquilinus\AQU_TransferSite.h"

/**
* Wrapped IDirect3D9 class, or proxy class.
***/
class PCL_IDirect3D9 : public IDirect3D9
{
public:
	PCL_IDirect3D9(IDirect3D9 *pOriginal, AQU_TransferSite *pTransferSite);
	virtual ~PCL_IDirect3D9(void);

	/*** IUnknown methods ***/
	HRESULT  WINAPI QueryInterface(REFIID riid, void** ppvObj);
	ULONG    WINAPI AddRef(void);
	ULONG    WINAPI Release(void);

	/*** IDirect3D9 methods ***/
	HRESULT  WINAPI RegisterSoftwareDevice(void* pInitializeFunction);
	UINT     WINAPI GetAdapterCount(void);
	HRESULT  WINAPI GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) ;
	UINT     WINAPI GetAdapterModeCount(UINT Adapter, D3DFORMAT Format);
	HRESULT  WINAPI EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) ;
	HRESULT  WINAPI GetAdapterDisplayMode( UINT Adapter, D3DDISPLAYMODE* pMode) ;
	HRESULT  WINAPI CheckDeviceType(UINT iAdapter, D3DDEVTYPE DevType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) ;
	HRESULT  WINAPI CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) ;
	HRESULT  WINAPI CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) ;
	HRESULT  WINAPI CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) ;
	HRESULT  WINAPI CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
	HRESULT  WINAPI GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) ;
	HMONITOR WINAPI GetAdapterMonitor(UINT Adapter) ;
	HRESULT  WINAPI CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) ;

private:
	/**
	* The actual D3D object.
	***/
	IDirect3D9 *m_pIDirect3D9;
	/**
	* The Aquilinus transfer site class.
	***/
	AQU_TransferSite *m_pTransferSite;
};