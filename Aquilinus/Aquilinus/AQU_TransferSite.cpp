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
#include "AQU_TransferSite.h"

/**
* Constructor.
***/
AQU_TransferSite::AQU_TransferSite(AquilinusCfg* pConfig) :
	m_pConfig(pConfig),
	m_bForceD3D(false),
	m_dwMainThreadId(0),
	m_dwThreadsActive(0),
	m_bNodesInitialized(false),
	m_bIsWorkingArea(false),
	m_bCreateShaderHash(false),
	m_bGatherVShaderBySet(false),
	m_bGatherPShaderBySet(false),
	m_nVertexShaderTabIndex(-1),
	m_nPixelShaderTabIndex(-1),
	m_ppNOD_IDirect3DDevice9(nullptr),
	m_ppNOD_IDirect3DTexture9(nullptr),
	m_ppNOD_IDirect3DBaseTexture9(nullptr),
	m_ppNOD_IDirect3DResource9(nullptr),
	m_ppNOD_IDirect3DCubeTexture9(nullptr),
	m_ppNOD_IDirect3DVolumeTexture9(nullptr),
	m_ppNOD_IDirect3DVolume9(nullptr),
	m_ppNOD_IDirect3DSurface9(nullptr),
	m_ppNOD_IDirect3DSwapChain9(nullptr),
	m_ppNOD_IDirect3DIndexBuffer9(nullptr),
	m_ppNOD_IDirect3DVertexBuffer9(nullptr),
	m_ppNOD_IDirect3DPixelShader9(nullptr),
	m_ppNOD_IDirect3DVertexShader9(nullptr),
	m_ppNOD_IDirect3DQuery9(nullptr),
	m_ppNOD_IDirect3DStateBlock9(nullptr),
	m_ppNOD_IDirect3DVertexDeclaration9(nullptr),
	m_ppNOD_IDirect3DDevice9Ex(nullptr),
	m_pNOD_D3DX9(nullptr),
	m_pD3D9ReinstateInterfaces(nullptr),
	m_pD3D929ReinstateInterfaces(nullptr),
	m_pD3D10ReinstateInterfaces(nullptr),
	m_pD3D10_1ReinstateInterfaces(nullptr),
	m_pD3D11ReinstateInterfaces(nullptr),
	m_pD3D11_1ReinstateInterfaces(nullptr),
	m_pD3D11_2ReinstateInterfaces(nullptr),
	m_pIDirect3DCubeTexture9(nullptr),
	m_pIDirect3DIndexBuffer9(nullptr),
	m_pIDirect3DPixelShader9(nullptr),
	m_pIDirect3DQuery9(nullptr),
	m_pIDirect3DStateBlock9(nullptr),
	m_pIDirect3DSurface9(nullptr),
	m_pIDirect3DSwapChain9(nullptr),
	m_pIDirect3DTexture9(nullptr),
	m_pIDirect3DVertexBuffer9(nullptr),
	m_pIDirect3DVertexShader9(nullptr),
	m_pIDirect3DVolumeTexture9(nullptr),
	m_ppNOD_ID3D10Device(nullptr),
	m_ppNOD_ID3D10Device1(nullptr),
	m_ppNOD_IDXGISwapChain(nullptr),
	m_ppNOD_IDXGISwapChain1(nullptr),
	m_ppNOD_IDXGISwapChain2(nullptr),
	m_ppNOD_IDXGISwapChain3(nullptr),
	m_ppNOD_ID3D11Device(nullptr),
	m_ppNOD_ID3D11Device1(nullptr),
	m_ppNOD_ID3D11Device2(nullptr),
	m_ppNOD_ID3D11Device3(nullptr),
	m_ppNOD_ID3D11DeviceContext(nullptr),
	m_ppNOD_ID3D11DeviceContext1(nullptr),
	m_ppNOD_ID3D11DeviceContext2(nullptr),
	m_ppNOD_ID3D11DeviceContext3(nullptr)
{
	// clear ref counter array
	for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
		m_anInterfaceRefCount[i] = 0;

	// clear vectors
	m_aPixelShaderHashcodes.clear();
	m_aVertexShaderHashcodes.clear();

	// create file manager class
	m_pFileManager = new AQU_FileManager(false);

	// set custom path if chosen in config
	if (m_pConfig->bUseDllPath)
	{
		m_pFileManager->SetCustomDirectoryPath(m_pConfig->szAquilinusPath);
	}

	// get number of CPUs present and create multithreading vector
	SYSTEM_INFO sSysinfo;
	GetSystemInfo(&sSysinfo);
	m_dwNumberOfProcessors = sSysinfo.dwNumberOfProcessors;
	m_adwCurrentThreadIds = std::vector<DWORD>(m_dwNumberOfProcessors, 0);
}

/**
*
***/
AQU_TransferSite::~AQU_TransferSite()
{
	if (m_ppNOD_IDirect3DDevice9) delete [] m_ppNOD_IDirect3DDevice9;
	if (m_ppNOD_IDirect3DTexture9) delete [] m_ppNOD_IDirect3DTexture9;
	if (m_ppNOD_IDirect3DBaseTexture9) delete [] m_ppNOD_IDirect3DBaseTexture9;
	if (m_ppNOD_IDirect3DResource9) delete [] m_ppNOD_IDirect3DResource9;
	if (m_ppNOD_IDirect3DCubeTexture9) delete [] m_ppNOD_IDirect3DCubeTexture9;
	if (m_ppNOD_IDirect3DVolumeTexture9) delete [] m_ppNOD_IDirect3DVolumeTexture9;
	if (m_ppNOD_IDirect3DVolume9) delete [] m_ppNOD_IDirect3DVolume9;
	if (m_ppNOD_IDirect3DSurface9) delete [] m_ppNOD_IDirect3DSurface9;
	if (m_ppNOD_IDirect3DSwapChain9) delete [] m_ppNOD_IDirect3DSwapChain9;
	if (m_ppNOD_IDirect3DIndexBuffer9) delete [] m_ppNOD_IDirect3DIndexBuffer9;
	if (m_ppNOD_IDirect3DVertexBuffer9) delete [] m_ppNOD_IDirect3DVertexBuffer9;
	if (m_ppNOD_IDirect3DPixelShader9) delete [] m_ppNOD_IDirect3DPixelShader9;
	if (m_ppNOD_IDirect3DVertexShader9) delete [] m_ppNOD_IDirect3DVertexShader9;
	if (m_ppNOD_IDirect3DQuery9) delete [] m_ppNOD_IDirect3DQuery9;
	if (m_ppNOD_IDirect3DStateBlock9) delete [] m_ppNOD_IDirect3DStateBlock9;
	if (m_ppNOD_IDirect3DVertexDeclaration9) delete [] m_ppNOD_IDirect3DVertexDeclaration9;
	if (m_ppNOD_IDirect3DDevice9Ex) delete [] m_ppNOD_IDirect3DDevice9Ex;
	if (m_ppNOD_ID3D10Device) delete [] m_ppNOD_ID3D10Device;
	if (m_ppNOD_ID3D10Device1) delete [] m_ppNOD_ID3D10Device1;
	if (m_ppNOD_IDXGISwapChain) delete [] m_ppNOD_IDXGISwapChain;
	if (m_ppNOD_IDXGISwapChain1) delete [] m_ppNOD_IDXGISwapChain1;
	if (m_ppNOD_IDXGISwapChain2) delete [] m_ppNOD_IDXGISwapChain2;
	if (m_ppNOD_IDXGISwapChain3) delete [] m_ppNOD_IDXGISwapChain3;
	if (m_ppNOD_ID3D11Device) delete [] m_ppNOD_ID3D11Device;
	if (m_ppNOD_ID3D11Device1) delete [] m_ppNOD_ID3D11Device1;
	if (m_ppNOD_ID3D11Device2) delete [] m_ppNOD_ID3D11Device2;
	if (m_ppNOD_ID3D11Device3) delete [] m_ppNOD_ID3D11Device3;
	if (m_ppNOD_ID3D11DeviceContext) delete [] m_ppNOD_ID3D11DeviceContext;
	if (m_ppNOD_ID3D11DeviceContext1) delete [] m_ppNOD_ID3D11DeviceContext1;
	if (m_ppNOD_ID3D11DeviceContext2) delete [] m_ppNOD_ID3D11DeviceContext2;
	if (m_ppNOD_ID3D11DeviceContext3) delete [] m_ppNOD_ID3D11DeviceContext3;
}

/**
* Inits D3D node arrays for all D3D interfaces.
***/
void AQU_TransferSite::InitD3DNodes()
{
	m_bNodesInitialized = true;

	// IDirect3DDevice9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DDevice9 = new NOD_Basic*[D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DDevice9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER);
	}

	// IDirect3DTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DTexture9 = new NOD_Basic*[D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DBaseTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DBaseTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DBaseTexture9 = new NOD_Basic*[D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DBaseTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DResource9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DResource9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DResource9 = new NOD_Basic*[D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DResource9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER);
	}

	// IDirect3DCubeTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DCubeTexture9 = new NOD_Basic*[D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DCubeTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DVolumeTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DVolumeTexture9 = new NOD_Basic*[D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DVolumeTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DVolume9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DVolume9 = new NOD_Basic*[D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DVolume9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER);
	}

	// IDirect3DSurface9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DSurface9 = new NOD_Basic*[D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DSurface9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER);
	}

	// IDirect3DSwapChain9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DSwapChain9 = new NOD_Basic*[D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DSwapChain9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER);
	}

	// IDirect3DIndexBuffer9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DIndexBuffer9 = new NOD_Basic*[D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DIndexBuffer9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER);
	}

	// IDirect3DVertexBuffer9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DVertexBuffer9 = new NOD_Basic*[D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DVertexBuffer9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER);
	}

	// IDirect3DPixelShader9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DPixelShader9 = new NOD_Basic*[D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DPixelShader9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER);
	}

	// IDirect3DVertexShader9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DVertexShader9 = new NOD_Basic*[D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DVertexShader9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER);
	}

	// IDirect3DQuery9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DQuery9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DQuery9 = new NOD_Basic*[D3D9_IDIRECT3DQUERY9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DQuery9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DQUERY9_METHODS_NUMBER);
	}

	// IDirect3DStateBlock9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DStateBlock9 = new NOD_Basic*[D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DStateBlock9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER);
	}

	// IDirect3DVertexDeclaration9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexDeclaration9] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DVertexDeclaration9 = new NOD_Basic*[D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DVertexDeclaration9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER);
	}

	// IDirect3DDevice9Ex
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDirect3DDevice9Ex = new NOD_Basic*[D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDirect3DDevice9Ex[0], sizeof(NOD_Basic*)*D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER);
	}

	// ID3D10Device
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D10Device = new NOD_Basic*[D3D10_ID3D10DEVICE_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D10Device[0], sizeof(NOD_Basic*)*D3D10_ID3D10DEVICE_METHODS_NUMBER);
	}

	// ID3D10Device1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device1] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D10Device1 = new NOD_Basic*[D3D10_ID3D10DEVICE1_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D10Device1[0], sizeof(NOD_Basic*)*D3D10_ID3D10DEVICE1_METHODS_NUMBER);
	}

	// ID3D11Device
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11Device = new NOD_Basic*[D3D11_ID3D11DEVICE_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11Device[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE_METHODS_NUMBER);
	}

	// ID3D11Device1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device1] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11Device1 = new NOD_Basic*[D3D11_ID3D11DEVICE1_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11Device[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE1_METHODS_NUMBER);
	}

	// ID3D11Device2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device2] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11Device2 = new NOD_Basic*[D3D11_ID3D11DEVICE2_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11Device2[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE2_METHODS_NUMBER);
	}

	// ID3D11Device3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device3] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11Device3 = new NOD_Basic*[D3D11_ID3D11DEVICE3_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11Device3[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE3_METHODS_NUMBER);
	}

	// IDXGISwapChain
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDXGISwapChain = new NOD_Basic*[DXGI_IDXGISWAPCHAIN_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDXGISwapChain[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN_METHODS_NUMBER);
	}

	// IDXGISwapChain1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain1] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDXGISwapChain1 = new NOD_Basic*[DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDXGISwapChain1[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER);
	}

	// IDXGISwapChain2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain2] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDXGISwapChain2 = new NOD_Basic*[DXGI_IDXGISWAPCHAIN2_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDXGISwapChain2[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN2_METHODS_NUMBER);
	}

	// IDXGISwapChain3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain3] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_IDXGISwapChain3 = new NOD_Basic*[DXGI_IDXGISWAPCHAIN3_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_IDXGISwapChain3[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN3_METHODS_NUMBER);
	}

	// ID3D11DeviceContext
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11DeviceContext = new NOD_Basic*[D3D11_ID3D11DEVICECONTEXT_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT_METHODS_NUMBER);
	}

	// ID3D11DeviceContext1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext1] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11DeviceContext1 = new NOD_Basic*[D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext1[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER);
	}

	// ID3D11DeviceContext2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext2] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11DeviceContext2 = new NOD_Basic*[D3D11_ID3D11DEVICECONTEXT2_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext2[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT2_METHODS_NUMBER);
	}

	// ID3D11DeviceContext3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext3] != AQU_InjectionTechniques::NoInjection)
	{
		m_ppNOD_ID3D11DeviceContext3 = new NOD_Basic*[D3D11_ID3D11DEVICECONTEXT3_METHODS_NUMBER];
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext3[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT3_METHODS_NUMBER);
	}
}

/**
* True if the node of the specified D3D Interace Method is already present, false if not.
***/
bool AQU_TransferSite::IsD3DNodePresent(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces eInterfaceIndex, int nMethodIndex)
{
	switch (eInterfaceIndex)
	{
	case AQU_SUPPORTEDINTERFACES::IDirect3DBaseTexture9:
		if (m_ppNOD_IDirect3DBaseTexture9)
		{
			if (m_ppNOD_IDirect3DBaseTexture9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DCubeTexture9:
		if (m_ppNOD_IDirect3DCubeTexture9)
		{
			if (m_ppNOD_IDirect3DCubeTexture9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9:
		if (m_ppNOD_IDirect3DDevice9)
		{
			if (m_ppNOD_IDirect3DDevice9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DIndexBuffer9:
		if (m_ppNOD_IDirect3DIndexBuffer9)
		{
			if (m_ppNOD_IDirect3DIndexBuffer9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DPixelShader9:
		if (m_ppNOD_IDirect3DPixelShader9)
		{
			if (m_ppNOD_IDirect3DPixelShader9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DQuery9:
		if (m_ppNOD_IDirect3DQuery9)
		{
			if (m_ppNOD_IDirect3DQuery9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DResource9:
		if (m_ppNOD_IDirect3DResource9)
		{
			if (m_ppNOD_IDirect3DResource9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DStateBlock9:
		if (m_ppNOD_IDirect3DStateBlock9)
		{
			if (m_ppNOD_IDirect3DStateBlock9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DSurface9:
		if (m_ppNOD_IDirect3DSurface9)
		{
			if (m_ppNOD_IDirect3DSurface9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DSwapChain9:
		if (m_ppNOD_IDirect3DSwapChain9)
		{
			if (m_ppNOD_IDirect3DSwapChain9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DTexture9:
		if (m_ppNOD_IDirect3DTexture9)
		{
			if (m_ppNOD_IDirect3DTexture9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexBuffer9:
		if (m_ppNOD_IDirect3DVertexBuffer9)
		{
			if (m_ppNOD_IDirect3DVertexBuffer9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexDeclaration9:
		if (m_ppNOD_IDirect3DVertexDeclaration9)
		{
			if (m_ppNOD_IDirect3DVertexDeclaration9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexShader9:
		if (m_ppNOD_IDirect3DVertexShader9)
		{
			if (m_ppNOD_IDirect3DVertexShader9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVolume9:
		if (m_ppNOD_IDirect3DVolume9)
		{
			if (m_ppNOD_IDirect3DVolume9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVolumeTexture9:
		if (m_ppNOD_IDirect3DVolumeTexture9)
		{
			if (m_ppNOD_IDirect3DVolumeTexture9[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9Ex:
		if (m_ppNOD_IDirect3DDevice9Ex)
		{
			if (m_ppNOD_IDirect3DDevice9Ex[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D10Device:
		if (m_ppNOD_ID3D10Device)
		{
			if (m_ppNOD_ID3D10Device[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D10Device1:
		if (m_ppNOD_ID3D10Device1)
		{
			if (m_ppNOD_ID3D10Device1[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device:
		if (m_ppNOD_ID3D11Device)
		{
			if (m_ppNOD_ID3D11Device[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device1:
		if (m_ppNOD_ID3D11Device1)
		{
			if (m_ppNOD_ID3D11Device1[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device2:
		if (m_ppNOD_ID3D11Device2)
		{
			if (m_ppNOD_ID3D11Device2[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device3:
		if (m_ppNOD_ID3D11Device3)
		{
			if (m_ppNOD_ID3D11Device3[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain:
		if (m_ppNOD_IDXGISwapChain)
		{
			if (m_ppNOD_IDXGISwapChain[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain1:
		if (m_ppNOD_IDXGISwapChain1)
		{
			if (m_ppNOD_IDXGISwapChain1[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain2:
		if (m_ppNOD_IDXGISwapChain2)
		{
			if (m_ppNOD_IDXGISwapChain2[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain3:
		if (m_ppNOD_IDXGISwapChain3)
		{
			if (m_ppNOD_IDXGISwapChain3[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext:
		if (m_ppNOD_ID3D11DeviceContext)
		{
			if (m_ppNOD_ID3D11DeviceContext[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext1:
		if (m_ppNOD_ID3D11DeviceContext1)
		{
			if (m_ppNOD_ID3D11DeviceContext1[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext2:
		if (m_ppNOD_ID3D11DeviceContext2)
		{
			if (m_ppNOD_ID3D11DeviceContext2[nMethodIndex]) return true;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext3:
		if (m_ppNOD_ID3D11DeviceContext3)
		{
			if (m_ppNOD_ID3D11DeviceContext3[nMethodIndex]) return true;
		}
		break;
	default:
		break;
	}

	return false;
}

/**
* Registers the specified D3D node to the node register useable by the detour class.
***/
void AQU_TransferSite::RegisterD3DNode(NOD_Basic* pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces eInterfaceIndex, int nMethodIndex)
{
	switch (eInterfaceIndex)
	{
	case AQU_SUPPORTEDINTERFACES::IDirect3DBaseTexture9:
		if (m_ppNOD_IDirect3DBaseTexture9)
		{
			m_ppNOD_IDirect3DBaseTexture9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DCubeTexture9:
		if (m_ppNOD_IDirect3DCubeTexture9)
		{
			m_ppNOD_IDirect3DCubeTexture9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9:
		if (m_ppNOD_IDirect3DDevice9)
		{
			m_ppNOD_IDirect3DDevice9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DIndexBuffer9:
		if (m_ppNOD_IDirect3DIndexBuffer9)
		{
			m_ppNOD_IDirect3DIndexBuffer9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DPixelShader9:
		if (m_ppNOD_IDirect3DPixelShader9)
		{
			m_ppNOD_IDirect3DPixelShader9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DQuery9:
		if (m_ppNOD_IDirect3DQuery9)
		{
			m_ppNOD_IDirect3DQuery9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DResource9:
		if (m_ppNOD_IDirect3DResource9)
		{
			m_ppNOD_IDirect3DResource9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DStateBlock9:
		if (m_ppNOD_IDirect3DStateBlock9)
		{
			m_ppNOD_IDirect3DStateBlock9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DSurface9:
		if (m_ppNOD_IDirect3DSurface9)
		{
			m_ppNOD_IDirect3DSurface9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DSwapChain9:
		if (m_ppNOD_IDirect3DSwapChain9)
		{
			m_ppNOD_IDirect3DSwapChain9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DTexture9:
		if (m_ppNOD_IDirect3DTexture9)
		{
			m_ppNOD_IDirect3DTexture9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexBuffer9:
		if (m_ppNOD_IDirect3DVertexBuffer9)
		{
			m_ppNOD_IDirect3DVertexBuffer9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexDeclaration9:
		if (m_ppNOD_IDirect3DVertexDeclaration9)
		{
			m_ppNOD_IDirect3DVertexDeclaration9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVertexShader9:
		if (m_ppNOD_IDirect3DVertexShader9)
		{
			m_ppNOD_IDirect3DVertexShader9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVolume9:
		if (m_ppNOD_IDirect3DVolume9)
		{
			m_ppNOD_IDirect3DVolume9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DVolumeTexture9:
		if (m_ppNOD_IDirect3DVolumeTexture9)
		{
			m_ppNOD_IDirect3DVolumeTexture9[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9Ex:
		if (m_ppNOD_IDirect3DDevice9Ex)
		{
			m_ppNOD_IDirect3DDevice9Ex[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D10Device:
		if (m_ppNOD_ID3D10Device)
		{
			m_ppNOD_ID3D10Device[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D10Device1:
		if (m_ppNOD_ID3D10Device1)
		{
			m_ppNOD_ID3D10Device1[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device:
		if (m_ppNOD_ID3D11Device)
		{
			m_ppNOD_ID3D11Device[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device1:
		if (m_ppNOD_ID3D11Device1)
		{
			m_ppNOD_ID3D11Device1[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device2:
		if (m_ppNOD_ID3D11Device2)
		{
			m_ppNOD_ID3D11Device2[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11Device3:
		if (m_ppNOD_ID3D11Device3)
		{
			m_ppNOD_ID3D11Device3[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain:
		if (m_ppNOD_IDXGISwapChain)
		{
			m_ppNOD_IDXGISwapChain[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain1:
		if (m_ppNOD_IDXGISwapChain1)
		{
			m_ppNOD_IDXGISwapChain1[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain2:
		if (m_ppNOD_IDXGISwapChain2)
		{
			m_ppNOD_IDXGISwapChain2[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::IDXGISwapChain3:
		if (m_ppNOD_IDXGISwapChain3)
		{
			m_ppNOD_IDXGISwapChain3[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext:
		if (m_ppNOD_ID3D11DeviceContext)
		{
			m_ppNOD_ID3D11DeviceContext[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext1:
		if (m_ppNOD_ID3D11DeviceContext1)
		{
			m_ppNOD_ID3D11DeviceContext1[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext2:
		if (m_ppNOD_ID3D11DeviceContext2)
		{
			m_ppNOD_ID3D11DeviceContext2[nMethodIndex] = pNode;
		}
		break;
	case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext3:
		if (m_ppNOD_ID3D11DeviceContext3)
		{
			m_ppNOD_ID3D11DeviceContext3[nMethodIndex] = pNode;
		}
		break;
	default:
		break;
	}
}

/**
* Registers the specified D3D node to the node register useable by the detour class.
***/
void AQU_TransferSite::RegisterD3DNode(NOD_Basic* pNode, UINT dwID)
{
	// read the interface .... 
	UINT dv = dwID >> 24;
	UINT in = (dwID >> 16) & 0x00ff;
	UINT mth = dwID & 0x0000ffff;

	// register the node (if D3D node)
	if (dv == DirectX_9_0)
	{
		switch(in)
		{
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DBaseTexture9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DCubeTexture9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DIndexBuffer9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DPixelShader9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DQuery9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DResource9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DResource9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSurface9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DTexture9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexBuffer9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexDeclaration9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexDeclaration9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexShader9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolume9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolumeTexture9:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9, mth);
			break;
		case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9Ex:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex, mth);
			break;
		}
	} 
	else if (dv == DirectX_10)
	{
		switch(in)
		{
		case (UINT)ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device, mth);
			break;
		case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain, mth);
			break;
		case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain1, mth);
			break;
		case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain2:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain2, mth);
			break;
		case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain3:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain3, mth);
			break;
		}
	}
	else if(dv == DirectX_10_1)
	{
		switch(in)
		{
		case (UINT)ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device1, mth);
			break;
		}
	}
	else if (dv == DirectX_11)
	{
		switch(in)
		{
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device, mth);
			break;
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext, mth);
			break;
		}
	}
	else if (dv == DirectX_11_1)
	{
		switch(in)
		{
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device1:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device1, mth);
			break;
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext1:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext1, mth);
			break;
		}
	}
	else if (dv == DirectX_11_2)
	{
		switch(in)
		{
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device2:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device2, mth);
			break;
		case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext2:
			RegisterD3DNode(pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext2, mth);
			break;
		}
	}
	else if (dwID = D3DOPS_NODE_D3DX9)
	{
		m_pNOD_D3DX9 = pNode;
	}
}

/**
* Unregisters all nodes in the transfer site class.
***/
void AQU_TransferSite::UnregisterAllNodes()
{
	// nodes initialized ?
	if (!m_bNodesInitialized) return;

	// IDirect3DDevice9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DDevice9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER);
	}

	// IDirect3DTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DBaseTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DBaseTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DBaseTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DResource9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DResource9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DResource9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER);
	}

	// IDirect3DCubeTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DCubeTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DVolumeTexture9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DVolumeTexture9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER);
	}

	// IDirect3DVolume9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DVolume9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER);
	}

	// IDirect3DSurface9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DSurface9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER);
	}

	// IDirect3DSwapChain9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DSwapChain9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER);
	}

	// IDirect3DIndexBuffer9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DIndexBuffer9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER);
	}

	// IDirect3DVertexBuffer9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DVertexBuffer9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER);
	}

	// IDirect3DPixelShader9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DPixelShader9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER);
	}

	// IDirect3DVertexShader9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DVertexShader9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER);
	}

	// IDirect3DQuery9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DQuery9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DQuery9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DQUERY9_METHODS_NUMBER);
	}

	// IDirect3DStateBlock9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DStateBlock9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER);
	}

	// IDirect3DVertexDeclaration9
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexDeclaration9] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DVertexDeclaration9[0], sizeof(NOD_Basic*)*D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER);
	}

	// IDirect3DDevice9Ex
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDirect3DDevice9Ex[0], sizeof(NOD_Basic*)*D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER);
	}
	// ID3D10Device
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D10Device[0], sizeof(NOD_Basic*)*D3D10_ID3D10DEVICE_METHODS_NUMBER);
	}

	// ID3D10Device1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device1] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D10Device1[0], sizeof(NOD_Basic*)*D3D10_ID3D10DEVICE1_METHODS_NUMBER);
	}

	// ID3D11Device
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11Device[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE_METHODS_NUMBER);
	}

	// ID3D11Device1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device1] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11Device[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE1_METHODS_NUMBER);
	}

	// ID3D11Device2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device2] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11Device2[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE2_METHODS_NUMBER);
	}

	// ID3D11Device3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device3] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11Device3[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICE3_METHODS_NUMBER);
	}

	// IDXGISwapChain
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDXGISwapChain[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN_METHODS_NUMBER);
	}

	// IDXGISwapChain1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain1] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDXGISwapChain1[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER);
	}

	// IDXGISwapChain2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain2] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDXGISwapChain2[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN2_METHODS_NUMBER);
	}

	// IDXGISwapChain3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain3] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_IDXGISwapChain3[0], sizeof(NOD_Basic*)*DXGI_IDXGISWAPCHAIN3_METHODS_NUMBER);
	}

	// ID3D11DeviceContext
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT_METHODS_NUMBER);
	}

	// ID3D11DeviceContext1
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext1] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext1[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER);
	}

	// ID3D11DeviceContext2
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext2] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext2[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT2_METHODS_NUMBER);
	}

	// ID3D11DeviceContext3
	if(m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext3] != AQU_InjectionTechniques::NoInjection)
	{
		ZeroMemory(&m_ppNOD_ID3D11DeviceContext3[0], sizeof(NOD_Basic*)*D3D11_ID3D11DEVICECONTEXT3_METHODS_NUMBER);
	}

	m_pNOD_D3DX9 = nullptr;
}

/**
* Registers a recorded vertex shader data sheet entry.
***/
void AQU_TransferSite::RegisterDataSheetVertexShader(LPCWSTR szName, LPCWSTR* pszEntries, UINT dwEntryCount, UINT dwHash)
{
	if (m_nVertexShaderTabIndex < 0) return;

	// create entry
	AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
	pEntry->m_bIsOpen = false;
	pEntry->m_szTitle = szName;
	pEntry->m_dwSubEntriesNumber = dwEntryCount;
	pEntry->m_paSubEntries = pszEntries;

	// add to vertex shader category and hash code table
	m_paDataSheetCategories[m_nVertexShaderTabIndex]->m_paEntries.push_back(pEntry);
	m_aVertexShaderHashcodes.push_back(dwHash);
}

/**
* Registers a recorded pixel shader data sheet entry.
***/
void AQU_TransferSite::RegisterDataSheetPixelShader(LPCWSTR szName, LPCWSTR* pszEntries, UINT dwEntryCount, UINT dwHash)
{
	if (m_nPixelShaderTabIndex < 0) return;

	// create entry
	AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
	pEntry->m_bIsOpen = false;
	pEntry->m_szTitle = szName;
	pEntry->m_dwSubEntriesNumber = dwEntryCount;
	pEntry->m_paSubEntries = pszEntries;

	// add to pixel shader category and hash code table
	m_paDataSheetCategories[m_nPixelShaderTabIndex]->m_paEntries.push_back(pEntry);
	m_aPixelShaderHashcodes.push_back(dwHash);
}

/**
* True if specified vertex shader is already present.
***/
bool AQU_TransferSite::VertexShaderPresent(UINT dwHash)
{
	if(std::find(m_aVertexShaderHashcodes.begin(), m_aVertexShaderHashcodes.end(), dwHash) != m_aVertexShaderHashcodes.end()) 
		return true;
	else 
		return false;
}

/**
* True if specified pixel shader is already present.
***/
bool AQU_TransferSite::PixelShaderPresent(UINT dwHash)
{
	if(std::find(m_aPixelShaderHashcodes.begin(), m_aPixelShaderHashcodes.end(), dwHash) != m_aPixelShaderHashcodes.end()) 
		return true;
	else 
		return false;
}


