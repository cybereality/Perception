/********************************************************************
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
********************************************************************/

#pragma warning( disable : 26812 )
#pragma warning( disable : 26495 )

#pragma region include

#ifndef _NO_DIRECTXMATH
#include<DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10_1.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "Dxgi.lib")
#else
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
#endif

// MinHook
#include "..\..\Perception\dependecies\minhook\include\MinHook.h"

#include <stdio.h>
#include "Resources.h"
#include "AquilinusCfg.h"
#include "AQU_TransferSite.h"
#include "AQU_FileManager.h"
#include "AQU_WorkingArea.h"
#include "VMT_ID3D10Device.h"
#include "VMT_ID3D10Device1.h"
#include "VMT_ID3D11Device.h"
#include "VMT_ID3D11Device1.h"
#include "VMT_ID3D11DeviceContext.h"
#include "VMT_ID3D11DeviceContext1.h"
#include "VMT_IDXGISwapChain.h"
#include "VMT_IDXGISwapChain1.h"
#include "DCL_ID3D11DeviceContext.h"
#include "DCL_ID3D11DeviceContext1.h"
#include "DCL_ID3D11Device.h"
#include "DCL_ID3D11Device1.h"
#include "DCL_IDXGISwapChain.h"
#include "DCL_IDXGISwapChain1.h"
#include "DCL_ID3D10Device.h"
#include "DCL_ID3D10Device1.h"

#pragma region DirectX 9.0 includes
// detour classes
#include "DCL_IDirect3DBaseTexture9.h"
#include "DCL_IDirect3DCubeTexture9.h"
#include "DCL_IDirect3DDevice9.h"
#include "DCL_IDirect3DIndexBuffer9.h"
#include "DCL_IDirect3DPixelShader9.h"
#include "DCL_IDirect3DQuery9.h"
#include "DCL_IDirect3DResource9.h"
#include "DCL_IDirect3DStateBlock9.h"
#include "DCL_IDirect3DSurface9.h"
#include "DCL_IDirect3DSwapChain9.h"
#include "DCL_IDirect3DTexture9.h"
#include "DCL_IDirect3DVertexBuffer9.h"
#include "DCL_IDirect3DVertexDeclaration9.h"
#include "DCL_IDirect3DVertexShader9.h"
#include "DCL_IDirect3DVolume9.h"
#include "DCL_IDirect3DVolumeTexture9.h"
#include "DCL_IDirect3DDevice9ex.h"
// vmtables
#include "VMT_IDirect3DDevice9.h"
#include "VMT_IDirect3DBaseTexture9.h"
#include "VMT_IDirect3DCubeTexture9.h"
#include "VMT_IDirect3DDevice9.h"
#include "VMT_IDirect3DIndexBuffer9.h"
#include "VMT_IDirect3DPixelShader9.h"
#include "VMT_IDirect3DQuery9.h"
#include "VMT_IDirect3DResource9.h"
#include "VMT_IDirect3DStateBlock9.h"
#include "VMT_IDirect3DSurface9.h"
#include "VMT_IDirect3DSwapChain9.h"
#include "VMT_IDirect3DTexture9.h"
#include "VMT_IDirect3DVertexBuffer9.h"
#include "VMT_IDirect3DVertexDeclaration9.h"
#include "VMT_IDirect3DVertexShader9.h"
#include "VMT_IDirect3DVolume9.h"
#include "VMT_IDirect3DVolumeTexture9.h"
#include "VMT_IDirect3DDevice9Ex.h"
#pragma endregion

#pragma endregion

#pragma region Aquilinus global fields
AQU_TransferSite* g_pAQU_TransferSite;    /**< The transfer site. **/
AQU_WorkingArea* g_pAQU_WorkingArea;     /**< The working area. **/
std::vector<NOD_Basic*> g_paNodes;              /**< The nodes vector used for complemented profiles. **/
int                     g_area_width = 600;     /**< The default width of the working area window. **/
int                     g_area_height = 200;    /**< The default height of the working area window. **/
int                     g_profile_width = 600;  /**< The default width of the profile window. **/
int                     g_profile_height = 200; /**< The default height of the profile window. **/
#pragma endregion

#pragma region Aquilinus detour classes
// DirectX 9.0
DCL_IDirect3DBaseTexture9* pDCL_IDirect3DBaseTexture9;
DCL_IDirect3DCubeTexture9* pDCL_IDirect3DCubeTexture9;
DCL_IDirect3DDevice9* pDCL_IDirect3DDevice9;
DCL_IDirect3DIndexBuffer9* pDCL_IDirect3DIndexBuffer9;
DCL_IDirect3DPixelShader9* pDCL_IDirect3DPixelShader9;
DCL_IDirect3DQuery9* pDCL_IDirect3DQuery9;
DCL_IDirect3DResource9* pDCL_IDirect3DResource9;
DCL_IDirect3DStateBlock9* pDCL_IDirect3DStateBlock9;
DCL_IDirect3DSurface9* pDCL_IDirect3DSurface9;
DCL_IDirect3DSwapChain9* pDCL_IDirect3DSwapChain9;
DCL_IDirect3DTexture9* pDCL_IDirect3DTexture9;
DCL_IDirect3DVertexBuffer9* pDCL_IDirect3DVertexBuffer9;
DCL_IDirect3DVertexDeclaration9* pDCL_IDirect3DVertexDeclaration9;
DCL_IDirect3DVertexShader9* pDCL_IDirect3DVertexShader9;
DCL_IDirect3DVolume9* pDCL_IDirect3DVolume9;
DCL_IDirect3DVolumeTexture9* pDCL_IDirect3DVolumeTexture9;
DCL_IDirect3DDevice9Ex* pDCL_IDirect3DDevice9Ex;

// DirectX 10
DCL_ID3D10Device* pDCL_ID3D10Device;
DCL_ID3D10Device1* pDCL_ID3D10Device1;
DCL_IDXGISwapChain* pDCL_IDXGISwapChain;
DCL_IDXGISwapChain1* pDCL_IDXGISwapChain1;

// DirectX 11
DCL_ID3D11Device* pDCL_ID3D11Device;
DCL_ID3D11Device1* pDCL_ID3D11Device1;
DCL_ID3D11DeviceContext* pDCL_ID3D11DeviceContext;
DCL_ID3D11DeviceContext1* pDCL_ID3D11DeviceContext1;
#pragma endregion

#pragma region Aquilinus dll methods
HRESULT     WINAPI                  AquilinusInit(VOID);
HRESULT     WINAPI                  AquilinusInitProject(HINSTANCE hInstance);
IDirect3D9* WINAPI                  D3D9_Direct3DCreate9_Detour(UINT SDKVersion);
HRESULT     WINAPI                  D3D9_QueryInterface_Detour(LPDIRECT3D9 pcD3D, REFIID riid, void** ppvObj);
HRESULT     WINAPI                  D3D9_CheckDeviceType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
HRESULT     WINAPI                  D3D9_CheckDeviceFormat_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
HRESULT     WINAPI                  D3D9_CheckDeviceMultiSampleType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
HRESULT     WINAPI                  D3D9_CheckDepthStencilMatch_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
HRESULT     WINAPI                  D3D9_CheckDeviceFormatConversion_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
HRESULT     WINAPI                  D3D9_CreateDevice_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
HRESULT                             D3D9_CreateAll(LPDIRECT3D9 pParent, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
DWORD       WINAPI                  D3D9_Detour(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Repatch(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Repatch(LPVOID Param);
DWORD       WINAPI                  D3D11_Detour(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Repatch(LPVOID Param);

typedef IDirect3D9* (WINAPI* D3D9_Direct3DCreate9)(UINT SDKVersion);
D3D9_Direct3DCreate9 D3D9_Direct3DCreate9_Super = NULL;
typedef HRESULT(WINAPI* D3D9_QueryInterface)(LPDIRECT3D9 pcD3D, REFIID riid, void** ppvObj);
D3D9_QueryInterface D3D9_QueryInterface_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CheckDeviceType)(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
D3D9_CheckDeviceType D3D9_CheckDeviceType_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CheckDeviceFormat)(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
D3D9_CheckDeviceFormat D3D9_CheckDeviceFormat_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CheckDeviceMultiSampleType)(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
D3D9_CheckDeviceMultiSampleType D3D9_CheckDeviceMultiSampleType_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CheckDepthStencilMatch)(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
D3D9_CheckDepthStencilMatch D3D9_CheckDepthStencilMatch_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CheckDeviceFormatConversion)(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
D3D9_CheckDeviceFormatConversion D3D9_CheckDeviceFormatConversion_Super = NULL;
typedef HRESULT(WINAPI* D3D9_CreateDevice) (LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
D3D9_CreateDevice D3D9_CreateDevice_Super = NULL;
typedef HRESULT(WINAPI* D3D11_CreateDevice) (IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, CONST D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
D3D11_CreateDevice D3D11CreateDevice_Super = NULL;
typedef HRESULT(WINAPI* D3D11_CreateDeviceAndSwapChain) (IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, CONST D3D_FEATURE_LEVEL*, UINT, UINT, CONST DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
D3D11_CreateDeviceAndSwapChain D3D11CreateDeviceAndSwapChain_Super = NULL;
typedef HRESULT(WINAPI* DXGI_CreateSwapChain) (IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
DXGI_CreateSwapChain CreateSwapChain_Super = NULL;
#pragma endregion

#pragma region Aquilinus VMTable pointers
/*** D3D9 VMTable pointers ***/
PUINT_PTR                            D3D9_IDirect3D9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DBaseTexture9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DCubeTexture9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DDevice9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DIndexBuffer9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DPixelShader9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DQuery9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DResource9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DStateBlock9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DSurface9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DSwapChain9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DTexture9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DVertexBuffer9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DVertexDeclaration9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DVertexShader9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DVolume9_VMTable = NULL;
PUINT_PTR                            D3D9_IDirect3DVolumeTexture9_VMTable = NULL;

/*** D3D9.29 VMTable pointers ***/
PUINT_PTR                            D3D929_IDirect3DDevice9Ex_VMTable = NULL;

/*** D3D10 VMTable pointers ***/
PUINT_PTR                            D3D10_ID3D10Device_VMTable = NULL;
PUINT_PTR                            D3D10_ID3D10Device1_VMTable = NULL;
PUINT_PTR                            D3D10_IDXGISwapChain_VMTable = NULL;
PUINT_PTR                            D3D10_IDXGISwapChain1_VMTable = NULL;

/*** D3D11 VMTable pointers ***/
PUINT_PTR                            D3D11_ID3D11Device_VMTable = NULL;
PUINT_PTR                            D3D11_ID3D11Device1_VMTable = NULL;
PUINT_PTR                            D3D11_ID3D11DeviceContext_VMTable = NULL;
PUINT_PTR                            D3D11_ID3D11DeviceContext1_VMTable = NULL;
#pragma endregion

#pragma region Aquilinus VMTable function arrays
/*** D3D9 function arrays ***/
UINT_PTR                             anD3D9_IDirect3DBaseTexture9_VMTable[D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DCubeTexture9_VMTable[D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DDevice9_VMTable[D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DIndexBuffer9_VMTable[D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DPixelShader9_VMTable[D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DQuery9_VMTable[D3D9_IDIRECT3DQUERY9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DResource9_VMTable[D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DStateBlock9_VMTable[D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DSurface9_VMTable[D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DSwapChain9_VMTable[D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DTexture9_VMTable[D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DVertexBuffer9_VMTable[D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DVertexDeclaration9_VMTable[D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DVertexShader9_VMTable[D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DVolume9_VMTable[D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER];
UINT_PTR                             anD3D9_IDirect3DVolumeTexture9_VMTable[D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER];
/*** D3D9.29 function array ***/
UINT_PTR                             anD3D929_IDirect3DDevice9Ex_VMTable[D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER];
/*** D3D10 function arrays ***/
#define                             D3D10_DEVICE_METHODS_NUMBER 98
UINT_PTR                             anD3D10_ID3D10Device_VMTable[D3D10_DEVICE_METHODS_NUMBER];
#define                             D3D10_DEVICE1_METHODS_NUMBER 101
UINT_PTR                             anD3D10_ID3D10Device1_VMTable[D3D10_DEVICE1_METHODS_NUMBER];
#define                             DXGI_SWAPCHAIN_METHODS_NUMBER 18
UINT_PTR                             anD3D10_IDXGISwapChain_VMTable[DXGI_SWAPCHAIN_METHODS_NUMBER];
#define                             DXGI_SWAPCHAIN1_METHODS_NUMBER 29
UINT_PTR                             anD3D10_IDXGISwapChain1_VMTable[DXGI_SWAPCHAIN1_METHODS_NUMBER];
/*** D3D11 function arrays ***/
#define                             D3D11_DEVICE_METHODS_NUMBER 43
UINT_PTR                             anD3D11_ID3D11Device_VMTable[D3D11_DEVICE_METHODS_NUMBER];
#define                             D3D11_DEVICE1_METHODS_NUMBER 50
UINT_PTR                             anD3D11_ID3D11Device1_VMTable[D3D11_DEVICE1_METHODS_NUMBER];
#define                             D3D11_DEVICECONTEXT_METHODS_NUMBER 115
UINT_PTR                             anD3D11_ID3D11DeviceContext_VMTable[D3D11_DEVICECONTEXT_METHODS_NUMBER];
#define                             D3D11_DEVICECONTEXT1_METHODS_NUMBER 130
UINT_PTR                             anD3D11_ID3D11DeviceContext1_VMTable[D3D11_DEVICECONTEXT1_METHODS_NUMBER];
#pragma endregion

#pragma region Aquilinus Detour defines
#define JMP32_SZ 5   /**< the size of JMP <address> **/
#define NOP 0x90     /**< opcode for NOP **/
#define JMP 0xE9     /**< opcode for JUMP **/
#pragma endregion


