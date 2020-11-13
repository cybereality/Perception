/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d11.h>
Copyright © 2015 Denis Reischl

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

#ifndef AQU_D3D11_HEAD
#define AQU_D3D11_HEAD

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <D3D11.h>
#include "PCL_ID3D11Device.h"
// #include "..\AQU_d3d10\PCL_IDXGISwapChain.h"

#pragma region AQU_d3d11 exported functions
UINT D3D11CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT MipLevels) { return MipSlice + ArraySlice * MipLevels; }
HRESULT WINAPI D3D11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext);
HRESULT WINAPI D3D11CreateDeviceAndSwapChain(
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	IDXGISwapChain** ppSwapChain,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext);
#pragma endregion

#pragma region AQU_d3d9 global fields
//PCL_ID3D11Device*   g_pPCL_ID3D10Device;
//PCL_IDXGISwapChain* g_pPCL_IDXGISwapChain;
HMODULE             g_hD3D10;
#pragma endregion

#endif