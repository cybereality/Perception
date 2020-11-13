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
#pragma region include
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

// #include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

// #include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")

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
AQU_TransferSite*       g_pAQU_TransferSite;    /**< The transfer site. **/
AQU_WorkingArea*        g_pAQU_WorkingArea;     /**< The working area. **/
std::vector<NOD_Basic*> g_paNodes;              /**< The nodes vector used for complemented profiles. **/
int                     g_area_width = 600;    /**< The default width of the working area window. **/
int                     g_area_height = 200;    /**< The default height of the working area window. **/
int                     g_profile_width = 600; /**< The default width of the profile window. **/
int                     g_profile_height = 200; /**< The default height of the profile window. **/
#pragma endregion

#pragma region Aquilinus detour classes
// DirectX 9.0
DCL_IDirect3DBaseTexture9*       pDCL_IDirect3DBaseTexture9;
DCL_IDirect3DCubeTexture9*       pDCL_IDirect3DCubeTexture9;
DCL_IDirect3DDevice9*            pDCL_IDirect3DDevice9;
DCL_IDirect3DIndexBuffer9*       pDCL_IDirect3DIndexBuffer9;
DCL_IDirect3DPixelShader9*       pDCL_IDirect3DPixelShader9;
DCL_IDirect3DQuery9*             pDCL_IDirect3DQuery9;
DCL_IDirect3DResource9*          pDCL_IDirect3DResource9;
DCL_IDirect3DStateBlock9*        pDCL_IDirect3DStateBlock9;
DCL_IDirect3DSurface9*           pDCL_IDirect3DSurface9;
DCL_IDirect3DSwapChain9*         pDCL_IDirect3DSwapChain9;
DCL_IDirect3DTexture9*           pDCL_IDirect3DTexture9;
DCL_IDirect3DVertexBuffer9*      pDCL_IDirect3DVertexBuffer9;
DCL_IDirect3DVertexDeclaration9* pDCL_IDirect3DVertexDeclaration9;
DCL_IDirect3DVertexShader9*      pDCL_IDirect3DVertexShader9;
DCL_IDirect3DVolume9*            pDCL_IDirect3DVolume9;
DCL_IDirect3DVolumeTexture9*     pDCL_IDirect3DVolumeTexture9;
DCL_IDirect3DDevice9Ex*          pDCL_IDirect3DDevice9Ex;

// DirectX 10
DCL_ID3D10Device*        pDCL_ID3D10Device;
DCL_ID3D10Device1*       pDCL_ID3D10Device1;
DCL_IDXGISwapChain*      pDCL_IDXGISwapChain;
DCL_IDXGISwapChain1*     pDCL_IDXGISwapChain1;

// DirectX 11
DCL_ID3D11Device*         pDCL_ID3D11Device;
DCL_ID3D11Device1*        pDCL_ID3D11Device1;
DCL_ID3D11DeviceContext*  pDCL_ID3D11DeviceContext;
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
HRESULT     WINAPI                  D3D9_CreateDevice_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface);
HRESULT                             D3D9_CreateAll(LPDIRECT3D9 pParent, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
DWORD       WINAPI                  D3D9_Detour(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D9_VMT_Repatch(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D10_VMT_Repatch(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Mask(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Create(LPVOID Param);
DWORD       WINAPI                  D3D11_VMT_Repatch(LPVOID Param);

typedef IDirect3D9*(WINAPI* D3D9_Direct3DCreate9)(UINT SDKVersion);
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
#pragma endregion

#pragma region Aquilinus D3DX methods
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileA_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileW_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCWSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromResourceA_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromResourceW_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileInMemory_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcData, UINT SrcDataSize, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromSurface_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromMemory_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcPitch, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileA_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileW_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCWSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromResourceA_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromResourceW_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileInMemory_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcData, UINT SrcDataSize, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromVolume_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey);
HRESULT WINAPI D3D9_D3DXLoadVolumeFromMemory_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcRowPitch, UINT SrcSlicePitch, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey);
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
HRESULT WINAPI D3D9_D3DXCreateTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
HRESULT WINAPI D3D9_D3DXSaveTextureToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
HRESULT WINAPI D3D9_D3DXSaveTextureToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
HRESULT WINAPI D3D9_D3DXSaveTextureToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
HRESULT WINAPI D3D9_D3DXFilterTexture_Detour(LPDIRECT3DBASETEXTURE9 pBaseTexture, CONST PALETTEENTRY* pPalette, UINT SrcLevel, DWORD Filter);
HRESULT WINAPI D3D9_D3DXFillTexture_Detour(LPDIRECT3DTEXTURE9 pTexture, LPD3DXFILL2D pFunction, LPVOID pData);
HRESULT WINAPI D3D9_D3DXFillCubeTexture_Detour(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXFILL3D pFunction, LPVOID pData);
HRESULT WINAPI D3D9_D3DXFillVolumeTexture_Detour(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXFILL3D pFunction, LPVOID pData);
HRESULT WINAPI D3D9_D3DXFillTextureTX_Detour(LPDIRECT3DTEXTURE9 pTexture, LPD3DXTEXTURESHADER pTextureShader);
HRESULT WINAPI D3D9_D3DXFillCubeTextureTX_Detour(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXTEXTURESHADER pTextureShader);
HRESULT WINAPI D3D9_D3DXFillVolumeTextureTX_Detour(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXTEXTURESHADER pTextureShader);
HRESULT WINAPI D3D9_D3DXComputeNormalMap_Detour(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DTEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette, DWORD Flags, DWORD Channel, FLOAT Amplitude);

typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromFileA)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromFileW)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCWSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromResourceA)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromResourceW)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromFileInMemory)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcData, UINT SrcDataSize, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromSurface)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI* D3D9_D3DXLoadSurfaceFromMemory)(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcPitch, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI* D3D9_D3DXSaveSurfaceToFileA)(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
typedef HRESULT(WINAPI* D3D9_D3DXSaveSurfaceToFileW)(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
typedef HRESULT(WINAPI* D3D9_D3DXSaveSurfaceToFileInMemory)(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromFileA)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromFileW)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCWSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromResourceA)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromResourceW)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromFileInMemory)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcData, UINT SrcDataSize, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromVolume)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI* D3D9_D3DXLoadVolumeFromMemory)(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcRowPitch, UINT SrcSlicePitch, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI* D3D9_D3DXSaveVolumeToFileA)(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
typedef HRESULT(WINAPI* D3D9_D3DXSaveVolumeToFileW)(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
typedef HRESULT(WINAPI* D3D9_D3DXSaveVolumeToFileInMemory)(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTexture)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTexture)(LPDIRECT3DDEVICE9 pDevice, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTexture)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromResourceA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromResourceW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromResourceA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromResourceW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromResourceA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromResourceW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileExA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileExW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileExA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileExW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileExA)(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileExW)(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromResourceExA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromResourceExW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromResourceExA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromResourceExW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromResourceExA)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromResourceExW)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileInMemory)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileInMemory)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileInMemory)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateTextureFromFileInMemoryEx)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateCubeTextureFromFileInMemoryEx)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);
typedef HRESULT(WINAPI* D3D9_D3DXSaveTextureToFileA)(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
typedef HRESULT(WINAPI* D3D9_D3DXSaveTextureToFileW)(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
typedef HRESULT(WINAPI* D3D9_D3DXSaveTextureToFileInMemory)(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette);
typedef HRESULT(WINAPI* D3D9_D3DXFilterTexture)(LPDIRECT3DBASETEXTURE9 pBaseTexture, CONST PALETTEENTRY* pPalette, UINT SrcLevel, DWORD Filter);
typedef HRESULT(WINAPI* D3D9_D3DXFillTexture)(LPDIRECT3DTEXTURE9 pTexture, LPD3DXFILL2D pFunction, LPVOID pData);
typedef HRESULT(WINAPI* D3D9_D3DXFillCubeTexture)(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXFILL3D pFunction, LPVOID pData);
typedef HRESULT(WINAPI* D3D9_D3DXFillVolumeTexture)(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXFILL3D pFunction, LPVOID pData);
typedef HRESULT(WINAPI* D3D9_D3DXFillTextureTX)(LPDIRECT3DTEXTURE9 pTexture, LPD3DXTEXTURESHADER pTextureShader);
typedef HRESULT(WINAPI* D3D9_D3DXFillCubeTextureTX)(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXTEXTURESHADER pTextureShader);
typedef HRESULT(WINAPI* D3D9_D3DXFillVolumeTextureTX)(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXTEXTURESHADER pTextureShader);
typedef HRESULT(WINAPI* D3D9_D3DXComputeNormalMap)(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DTEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette, DWORD Flags, DWORD Channel, FLOAT Amplitude);

D3D9_D3DXLoadSurfaceFromFileA D3D9_D3DXLoadSurfaceFromFileA_Super = NULL;
D3D9_D3DXLoadSurfaceFromFileW D3D9_D3DXLoadSurfaceFromFileW_Super = NULL;
D3D9_D3DXLoadSurfaceFromResourceA D3D9_D3DXLoadSurfaceFromResourceA_Super = NULL;
D3D9_D3DXLoadSurfaceFromResourceW D3D9_D3DXLoadSurfaceFromResourceW_Super = NULL;
D3D9_D3DXLoadSurfaceFromFileInMemory D3D9_D3DXLoadSurfaceFromFileInMemory_Super = NULL;
D3D9_D3DXLoadSurfaceFromSurface D3D9_D3DXLoadSurfaceFromSurface_Super = NULL;
D3D9_D3DXLoadSurfaceFromMemory D3D9_D3DXLoadSurfaceFromMemory_Super = NULL;
D3D9_D3DXSaveSurfaceToFileA D3D9_D3DXSaveSurfaceToFileA_Super = NULL;
D3D9_D3DXSaveSurfaceToFileW D3D9_D3DXSaveSurfaceToFileW_Super = NULL;
D3D9_D3DXSaveSurfaceToFileInMemory D3D9_D3DXSaveSurfaceToFileInMemory_Super = NULL;
D3D9_D3DXLoadVolumeFromFileA D3D9_D3DXLoadVolumeFromFileA_Super = NULL;
D3D9_D3DXLoadVolumeFromFileW D3D9_D3DXLoadVolumeFromFileW_Super = NULL;
D3D9_D3DXLoadVolumeFromResourceA D3D9_D3DXLoadVolumeFromResourceA_Super = NULL;
D3D9_D3DXLoadVolumeFromResourceW D3D9_D3DXLoadVolumeFromResourceW_Super = NULL;
D3D9_D3DXLoadVolumeFromFileInMemory D3D9_D3DXLoadVolumeFromFileInMemory_Super = NULL;
D3D9_D3DXLoadVolumeFromVolume D3D9_D3DXLoadVolumeFromVolume_Super = NULL;
D3D9_D3DXLoadVolumeFromMemory D3D9_D3DXLoadVolumeFromMemory_Super = NULL;
D3D9_D3DXSaveVolumeToFileA D3D9_D3DXSaveVolumeToFileA_Super = NULL;
D3D9_D3DXSaveVolumeToFileW D3D9_D3DXSaveVolumeToFileW_Super = NULL;
D3D9_D3DXSaveVolumeToFileInMemory D3D9_D3DXSaveVolumeToFileInMemory_Super = NULL;
D3D9_D3DXCreateTexture D3D9_D3DXCreateTexture_Super = NULL;
D3D9_D3DXCreateCubeTexture D3D9_D3DXCreateCubeTexture_Super = NULL;
D3D9_D3DXCreateVolumeTexture D3D9_D3DXCreateVolumeTexture_Super = NULL;
D3D9_D3DXCreateTextureFromFileA D3D9_D3DXCreateTextureFromFileA_Super = NULL;
D3D9_D3DXCreateTextureFromFileW D3D9_D3DXCreateTextureFromFileW_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileA D3D9_D3DXCreateCubeTextureFromFileA_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileW D3D9_D3DXCreateCubeTextureFromFileW_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileA D3D9_D3DXCreateVolumeTextureFromFileA_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileW D3D9_D3DXCreateVolumeTextureFromFileW_Super = NULL;
D3D9_D3DXCreateTextureFromResourceA D3D9_D3DXCreateTextureFromResourceA_Super = NULL;
D3D9_D3DXCreateTextureFromResourceW D3D9_D3DXCreateTextureFromResourceW_Super = NULL;
D3D9_D3DXCreateCubeTextureFromResourceA D3D9_D3DXCreateCubeTextureFromResourceA_Super = NULL;
D3D9_D3DXCreateCubeTextureFromResourceW D3D9_D3DXCreateCubeTextureFromResourceW_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromResourceA D3D9_D3DXCreateVolumeTextureFromResourceA_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromResourceW D3D9_D3DXCreateVolumeTextureFromResourceW_Super = NULL;
D3D9_D3DXCreateTextureFromFileExA D3D9_D3DXCreateTextureFromFileExA_Super = NULL;
D3D9_D3DXCreateTextureFromFileExW D3D9_D3DXCreateTextureFromFileExW_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileExA D3D9_D3DXCreateCubeTextureFromFileExA_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileExW D3D9_D3DXCreateCubeTextureFromFileExW_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileExA D3D9_D3DXCreateVolumeTextureFromFileExA_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileExW D3D9_D3DXCreateVolumeTextureFromFileExW_Super = NULL;
D3D9_D3DXCreateTextureFromResourceExA D3D9_D3DXCreateTextureFromResourceExA_Super = NULL;
D3D9_D3DXCreateTextureFromResourceExW D3D9_D3DXCreateTextureFromResourceExW_Super = NULL;
D3D9_D3DXCreateCubeTextureFromResourceExA D3D9_D3DXCreateCubeTextureFromResourceExA_Super = NULL;
D3D9_D3DXCreateCubeTextureFromResourceExW D3D9_D3DXCreateCubeTextureFromResourceExW_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromResourceExA D3D9_D3DXCreateVolumeTextureFromResourceExA_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromResourceExW D3D9_D3DXCreateVolumeTextureFromResourceExW_Super = NULL;
D3D9_D3DXCreateTextureFromFileInMemory D3D9_D3DXCreateTextureFromFileInMemory_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileInMemory D3D9_D3DXCreateCubeTextureFromFileInMemory_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileInMemory D3D9_D3DXCreateVolumeTextureFromFileInMemory_Super = NULL;
D3D9_D3DXCreateTextureFromFileInMemoryEx D3D9_D3DXCreateTextureFromFileInMemoryEx_Super = NULL;
D3D9_D3DXCreateCubeTextureFromFileInMemoryEx D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Super = NULL;
D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Super = NULL;
D3D9_D3DXSaveTextureToFileA D3D9_D3DXSaveTextureToFileA_Super = NULL;
D3D9_D3DXSaveTextureToFileW D3D9_D3DXSaveTextureToFileW_Super = NULL;
D3D9_D3DXSaveTextureToFileInMemory D3D9_D3DXSaveTextureToFileInMemory_Super = NULL;
D3D9_D3DXFilterTexture D3D9_D3DXFilterTexture_Super = NULL;
D3D9_D3DXFillTexture D3D9_D3DXFillTexture_Super = NULL;
D3D9_D3DXFillCubeTexture D3D9_D3DXFillCubeTexture_Super = NULL;
D3D9_D3DXFillVolumeTexture D3D9_D3DXFillVolumeTexture_Super = NULL;
D3D9_D3DXFillTextureTX D3D9_D3DXFillTextureTX_Super = NULL;
D3D9_D3DXFillCubeTextureTX D3D9_D3DXFillCubeTextureTX_Super = NULL;
D3D9_D3DXFillVolumeTextureTX D3D9_D3DXFillVolumeTextureTX_Super = NULL;
D3D9_D3DXComputeNormalMap D3D9_D3DXComputeNormalMap_Super = NULL;

namespace MT_D3DX9
{
	/**
	* Methods table of D3DX9 methods.
	***/
	enum MT_D3DX9
	{
		D3D9_D3DXLoadSurfaceFromFileA,
		D3D9_D3DXLoadSurfaceFromFileW,
		D3D9_D3DXLoadSurfaceFromResourceA,
		D3D9_D3DXLoadSurfaceFromResourceW,
		D3D9_D3DXLoadSurfaceFromFileInMemory,
		D3D9_D3DXLoadSurfaceFromSurface,
		D3D9_D3DXLoadSurfaceFromMemory,
		D3D9_D3DXSaveSurfaceToFileA,
		D3D9_D3DXSaveSurfaceToFileW,
		D3D9_D3DXSaveSurfaceToFileInMemory,
		D3D9_D3DXLoadVolumeFromFileA,
		D3D9_D3DXLoadVolumeFromFileW,
		D3D9_D3DXLoadVolumeFromResourceA,
		D3D9_D3DXLoadVolumeFromResourceW,
		D3D9_D3DXLoadVolumeFromFileInMemory,
		D3D9_D3DXLoadVolumeFromVolume,
		D3D9_D3DXLoadVolumeFromMemory,
		D3D9_D3DXSaveVolumeToFileA,
		D3D9_D3DXSaveVolumeToFileW,
		D3D9_D3DXSaveVolumeToFileInMemory,
		D3D9_D3DXCreateTexture,
		D3D9_D3DXCreateCubeTexture,
		D3D9_D3DXCreateVolumeTexture,
		D3D9_D3DXCreateTextureFromFileA,
		D3D9_D3DXCreateTextureFromFileW,
		D3D9_D3DXCreateCubeTextureFromFileA,
		D3D9_D3DXCreateCubeTextureFromFileW,
		D3D9_D3DXCreateVolumeTextureFromFileA,
		D3D9_D3DXCreateVolumeTextureFromFileW,
		D3D9_D3DXCreateTextureFromResourceA,
		D3D9_D3DXCreateTextureFromResourceW,
		D3D9_D3DXCreateCubeTextureFromResourceA,
		D3D9_D3DXCreateCubeTextureFromResourceW,
		D3D9_D3DXCreateVolumeTextureFromResourceA,
		D3D9_D3DXCreateVolumeTextureFromResourceW,
		D3D9_D3DXCreateTextureFromFileExA,
		D3D9_D3DXCreateTextureFromFileExW,
		D3D9_D3DXCreateCubeTextureFromFileExA,
		D3D9_D3DXCreateCubeTextureFromFileExW,
		D3D9_D3DXCreateVolumeTextureFromFileExA,
		D3D9_D3DXCreateVolumeTextureFromFileExW,
		D3D9_D3DXCreateTextureFromResourceExA,
		D3D9_D3DXCreateTextureFromResourceExW,
		D3D9_D3DXCreateCubeTextureFromResourceExA,
		D3D9_D3DXCreateCubeTextureFromResourceExW,
		D3D9_D3DXCreateVolumeTextureFromResourceExA,
		D3D9_D3DXCreateVolumeTextureFromResourceExW,
		D3D9_D3DXCreateTextureFromFileInMemory,
		D3D9_D3DXCreateCubeTextureFromFileInMemory,
		D3D9_D3DXCreateVolumeTextureFromFileInMemory,
		D3D9_D3DXCreateTextureFromFileInMemoryEx,
		D3D9_D3DXCreateCubeTextureFromFileInMemoryEx,
		D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx,
		D3D9_D3DXSaveTextureToFileA,
		D3D9_D3DXSaveTextureToFileW,
		D3D9_D3DXSaveTextureToFileInMemory,
		D3D9_D3DXFilterTexture,
		D3D9_D3DXFillTexture,
		D3D9_D3DXFillCubeTexture,
		D3D9_D3DXFillVolumeTexture,
		D3D9_D3DXFillTextureTX,
		D3D9_D3DXFillCubeTextureTX,
		D3D9_D3DXFillVolumeTextureTX,
		D3D9_D3DXComputeNormalMap,
	};
};
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
#define                             D3D11_DEVICECONTEXT_METHODS_NUMBER 111
UINT_PTR                             anD3D11_ID3D11DeviceContext_VMTable[D3D11_DEVICECONTEXT_METHODS_NUMBER];
#define                             D3D11_DEVICECONTEXT1_METHODS_NUMBER 130
UINT_PTR                             anD3D11_ID3D11DeviceContext1_VMTable[D3D11_DEVICECONTEXT1_METHODS_NUMBER];
#pragma endregion

#pragma region Aquilinus Detour defines
#define JMP32_SZ 5   /**< the size of JMP <address> **/
#define NOP 0x90     /**< opcode for NOP **/
#define JMP 0xE9     /**< opcode for JUMP **/
#pragma endregion


