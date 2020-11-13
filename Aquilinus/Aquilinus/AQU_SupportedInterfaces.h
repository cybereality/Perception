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
#include<Windows.h>
#include"AQU_GlobalTypes.h"
#include"ITA_D3D9Interfaces.h"
#include"ITA_D3D10Interfaces.h"
#include"ITA_D3D11Interfaces.h"
#include"ITA_DXGIInterfaces.h"

#ifndef AQUILINUS_SUPPORTEDINTERFACES
#define AQUILINUS_SUPPORTEDINTERFACES

/*** The maximum length for MS C++ identifiers. ***/
#define MAX_IDENTIFIER_LENGTH 2048
/*** The number of supported interfaces. ***/
#define SUPPORTED_INTERFACES_NUMBER 31
/*** The first and last DirectX 9 interface index ***/
#define DX9_FIRST_INTERFACE 0
#define DX9_LAST_INTERFACE 15
/*** The first and last DirectX 9.29 interface index ***/
#define DX929_FIRST_INTERFACE 16
#define DX929_LAST_INTERFACE 16
/*** The first and last DirectX 10.x interface index ***/
#define DX10_FIRST_INTERFACE 17
#define DX10_LAST_INTERFACE 18
/*** The first and last DirectX 11.x interface index ***/
#define DX11_FIRST_INTERFACE 19
#define DX11_LAST_INTERFACE 22

namespace AQU_SUPPORTEDINTERFACES
{

	/**
	* D3D interface data structure.
	***/
	struct AQU_D3DInterface
	{
		/**
		* The class name of the inteface.
		* Used for GUI output.
		***/
		wchar_t szName[MAX_IDENTIFIER_LENGTH];
		/**
		* True if injection technique is supported for that interface.
		* Index equals AQU_InjectionTechniques enumeration.
		***/
		bool bInjectionTechnique[4];
		/**
		* The DirectX version of that interface.
		***/
		AQU_Direct3DVersion eD3DVersion;
		/**
		* The interface table index. Currently not used.
		***/
		int eInterface; 
		/**
		* The number of methods this interface conatins.
		***/
		UINT dwMethodsNumber;

	};

	/**
	* ALL currently supported D3D interfaces.
	* Enumeration equals g_sSupportedInterfaces index.
	**/
	enum AQU_SupportedInterfaces
	{
		IDirect3DBaseTexture9,
		IDirect3DCubeTexture9,
		IDirect3DDevice9,
		IDirect3DIndexBuffer9,
		IDirect3DPixelShader9,
		IDirect3DQuery9,
		IDirect3DResource9,
		IDirect3DStateBlock9,
		IDirect3DSurface9,
		IDirect3DSwapChain9,
		IDirect3DTexture9,
		IDirect3DVertexBuffer9,
		IDirect3DVertexDeclaration9,
		IDirect3DVertexShader9,
		IDirect3DVolume9,
		IDirect3DVolumeTexture9,
		IDirect3DDevice9Ex,
		ID3D10Device,
		ID3D10Device1,
		ID3D11Device,
		ID3D11Device1,
		ID3D11Device2,
		ID3D11Device3,
		IDXGISwapChain,
		IDXGISwapChain1,
		IDXGISwapChain2,
		IDXGISwapChain3,
		ID3D11DeviceContext,
		ID3D11DeviceContext1,
		ID3D11DeviceContext2,
		ID3D11DeviceContext3,
	};

	/**
	* The list of supported D3D interfaces.
	* Index equals AQU_SupportedInterfaces enumeration.
	***/
	const AQU_D3DInterface g_sSupportedInterfaces[] =
	{
		{ L"IDirect3DBaseTexture9",       { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9,       D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER },
		{ L"IDirect3DCubeTexture9",       { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DCubeTexture9,       D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER },
		{ L"IDirect3DDevice9",            { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9,            D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER },
		{ L"IDirect3DIndexBuffer9",       { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DIndexBuffer9,       D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER },
		{ L"IDirect3DPixelShader9",       { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DPixelShader9,       D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER },
		{ L"IDirect3DQuery9",             { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9,             D3D9_IDIRECT3DQUERY9_METHODS_NUMBER },
		{ L"IDirect3DResource9",          { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DResource9,          D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER },
		{ L"IDirect3DStateBlock9",        { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9,        D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER },
		{ L"IDirect3DSurface9",           { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSurface9,           D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER },
		{ L"IDirect3DSwapChain9",         { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9,         D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER },
		{ L"IDirect3DTexture9",           { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DTexture9,           D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER },
		{ L"IDirect3DVertexBuffer9",      { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexBuffer9,      D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER },
		{ L"IDirect3DVertexDeclaration9", { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexDeclaration9, D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER },
		{ L"IDirect3DVertexShader9",      { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexShader9,      D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER },
		{ L"IDirect3DVolume9",            { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolume9,            D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER },
		{ L"IDirect3DVolumeTexture9",     { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0,  ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolumeTexture9,     D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER },
		{ L"IDirect3DDevice9Ex",          { true, true, true, false }     , AQU_Direct3DVersion::DirectX_9_0c, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9Ex,          D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER },
		{ L"ID3D10Device",                { true, true, true, false }     , AQU_Direct3DVersion::DirectX_10,   ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device,              D3D10_ID3D10DEVICE_METHODS_NUMBER },
		{ L"ID3D10Device1",               { true, true, true, false }     , AQU_Direct3DVersion::DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1,             D3D10_ID3D10DEVICE1_METHODS_NUMBER },
		{ L"ID3D11Device",                { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11,   ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device,              D3D11_ID3D11DEVICE_METHODS_NUMBER },
		{ L"ID3D11Device1",               { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11_1, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device1,             D3D11_ID3D11DEVICE1_METHODS_NUMBER },
		{ L"ID3D11Device2",               { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11_2, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device2,             D3D11_ID3D11DEVICE2_METHODS_NUMBER },
		{ L"ID3D11Device3",               { true, false, false, false }   , AQU_Direct3DVersion::DirectX_11_3, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device3,             D3D11_ID3D11DEVICE3_METHODS_NUMBER },
		{ L"IDXGISwapChain",              { true, true, true, false }     , AQU_Direct3DVersion::DirectX_10,   ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain,              DXGI_IDXGISWAPCHAIN_METHODS_NUMBER },
		{ L"IDXGISwapChain1",             { true, true, true, false }     , AQU_Direct3DVersion::DirectX_10,   ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1,             DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER },
		{ L"IDXGISwapChain2",             { true, true, true, false }     , AQU_Direct3DVersion::DirectX_10,   ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain2,             DXGI_IDXGISWAPCHAIN2_METHODS_NUMBER },
		{ L"IDXGISwapChain3",             { true, false, false, false }   , AQU_Direct3DVersion::DirectX_10,   ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain3,             DXGI_IDXGISWAPCHAIN3_METHODS_NUMBER },
		{ L"ID3D11DeviceContext",         { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11,   ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext,       D3D11_ID3D11DEVICECONTEXT_METHODS_NUMBER },
		{ L"ID3D11DeviceContext1",        { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11_1, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext1,      D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER },
		{ L"ID3D11DeviceContext2",        { true, true, true, false }     , AQU_Direct3DVersion::DirectX_11_2, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext2,      D3D11_ID3D11DEVICECONTEXT2_METHODS_NUMBER },
		{ L"ID3D11DeviceContext3",        { true, false, false, false }   , AQU_Direct3DVersion::DirectX_11_3, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext3,      D3D11_ID3D11DEVICECONTEXT3_METHODS_NUMBER },
	};

};

#endif