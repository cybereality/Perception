/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_IDirect3D9.h> and
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