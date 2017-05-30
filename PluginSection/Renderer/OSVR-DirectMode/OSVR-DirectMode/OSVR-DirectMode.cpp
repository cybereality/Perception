/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Direct Mode - Open Source Virtual Reality Direct Mode Node
Copyright (C) 2016 Denis Reischl

File <OSVR-DirectMode.cpp> and
Class <OSVR-DirectMode> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include"OSVR-DirectMode.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

#define INTERFACE_IDIRECT3DDEVICE9                                           8
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

#pragma region OSVR_DirectMode static fields
ID3D11VertexShader* OSVR_DirectMode::m_pcVertexShader11;
ID3D11PixelShader* OSVR_DirectMode::m_pcPixelShader11;
ID3D11InputLayout* OSVR_DirectMode::m_pcVertexLayout11;
ID3D11Buffer* OSVR_DirectMode::m_pcVertexBuffer11;
ID3D11Buffer* OSVR_DirectMode::m_pcConstantBufferDirect11;
ID3D11SamplerState* OSVR_DirectMode::m_pcSamplerState;
OSVR_DirectMode::StereoTextureViews OSVR_DirectMode::m_sStereoTextureViews;
OSVR_DirectMode::StereoTextureCopies OSVR_DirectMode::m_sStereoTextureCopies;
OSVR_DirectMode::StereoFrameTextures OSVR_DirectMode::m_sStereoFrameTextures;
OSVR_DirectMode::StereoFrameTextureSRViews OSVR_DirectMode::m_sSteroFrameTextureSRViews;
OSVR_DirectModeMethods OSVR_DirectMode::m_eMethod;
ID3D11Device* OSVR_DirectMode::m_pcGameDevice;
ID3D11DeviceContext* OSVR_DirectMode::m_pcGameDeviceContext;
BOOL* OSVR_DirectMode::m_pbZoomOut;
D3DMATRIX* OSVR_DirectMode::m_psProjection[2];
UINT32* OSVR_DirectMode::m_punTexResolutionWidth;
UINT32* OSVR_DirectMode::m_punTexResolutionHeight;
#pragma endregion

/**
* Constructor.
***/
OSVR_DirectMode::OSVR_DirectMode() :AQU_Nodus(),
m_pcRenderManager(nullptr),
m_bHotkeySwitch(false)
{
	m_pcVertexShader11 = nullptr;
	m_pcPixelShader11 = nullptr;
	m_pcVertexLayout11 = nullptr;
	m_pcVertexBuffer11 = nullptr;
	m_pcConstantBufferDirect11 = nullptr;
	m_pcSamplerState = nullptr;
	m_sStereoTextureViews.m_ppcTexView11[0] = nullptr;
	m_sStereoTextureViews.m_ppcTexView11[1] = nullptr;
	m_sStereoTextureCopies.m_pcTex11Copy[0] = nullptr;
	m_sStereoTextureCopies.m_pcTex11Copy[1] = nullptr;
	m_sStereoFrameTextures.m_pcFrameTexture[0] = nullptr;
	m_sStereoFrameTextures.m_pcFrameTexture[1] = nullptr;
	m_sSteroFrameTextureSRViews.m_pcFrameTextureSRView[0] = nullptr;
	m_sSteroFrameTextureSRViews.m_pcFrameTextureSRView[1] = nullptr;
	m_pcGameDevice = nullptr;
	m_pcGameDeviceContext = nullptr;
	m_pbZoomOut = nullptr;
	m_psProjection[0] = nullptr;
	m_psProjection[1] = nullptr;
	m_punTexResolutionHeight = nullptr;
	m_punTexResolutionWidth = nullptr;

	m_eMethod = OSVR_DirectModeMethods::OSVR_D3D11_own_Device;

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	m_eMethod = (OSVR_DirectModeMethods)GetIniFileSetting((DWORD)m_eMethod, "OSVR", "eMethod", szFilePathINI, bFileExists);
	DWORD bUseHotkeyF11 = 0;
	bUseHotkeyF11 = GetIniFileSetting(bUseHotkeyF11, "OSVR", "bUseHotkeyF11", szFilePathINI, bFileExists);
	if (bUseHotkeyF11 == 0) m_bHotkeySwitch = true;

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "NOT IMPLEMENTED NOW !!";
	{
		static float fDummy = 0.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "NOT IMPLEMENTED NOW !!";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 1.0f;
		sEntry.fMaximum = 30.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &fDummy;
		sEntry.fValue = fDummy;
		m_sMenu.asEntries.push_back(sEntry);
	}
}

/**
* Destructor.
***/
OSVR_DirectMode::~OSVR_DirectMode()
{
	SAFE_RELEASE(m_pcVertexShader11);
	SAFE_RELEASE(m_pcPixelShader11);
	SAFE_RELEASE(m_pcVertexLayout11);
	SAFE_RELEASE(m_pcVertexBuffer11);
	SAFE_RELEASE(m_pcConstantBufferDirect11);
	SAFE_RELEASE(m_pcSamplerState);
}

/**
* Return the name of the  OSVR Direct Mode node.
***/
const char* OSVR_DirectMode::GetNodeType()
{
	return "OSVR Direct Mode";
}

/**
* Returns a global unique identifier for the OSVR Direct Mode node.
***/
UINT OSVR_DirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 289
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OSVR Direct Mode node.
***/
LPWSTR OSVR_DirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the OSVR Direct Mode node.
***/
HBITMAP OSVR_DirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OSVR-DirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OSVR Direct Mode node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OSVR_DirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR OSVR_DirectMode::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case VireioMenu:
			return L"Vireio Menu";
	}

	return L"XXX";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OSVR_DirectMode::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((OSVR_Decommanders)dwDecommanderIndex)
	{
		case OSVR_Decommanders::LeftTexture11:
			return L"Left Texture";
		case OSVR_Decommanders::RightTexture11:
			return L"Right Texture";
		case OSVR_Decommanders::ZoomOut:
			return L"Zoom Out";
		case OSVR_Decommanders::ProjectionLeft:
			return L"Projection Left";
		case OSVR_Decommanders::ProjectionRight:
			return L"Projection Right";
		case OSVR_Decommanders::TargetWidth:
			return L"Target Width";
		case OSVR_Decommanders::TargetHeight:
			return L"Target Height";
	}

	return L"x";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD OSVR_DirectMode::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case VireioMenu:
			return NOD_Plugtype::AQU_VOID;
	}

	return 0;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OSVR_DirectMode::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((OSVR_Decommanders)dwDecommanderIndex)
	{
		case OSVR_Decommanders::LeftTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case OSVR_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case OSVR_Decommanders::ZoomOut:
			return NOD_Plugtype::AQU_BOOL;
		case OSVR_Decommanders::ProjectionLeft:
		case OSVR_Decommanders::ProjectionRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
		case OSVR_Decommanders::TargetWidth:
		case OSVR_Decommanders::TargetHeight:
			return NOD_Plugtype::AQU_UINT;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* OSVR_DirectMode::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case OSVR_Commanders::VireioMenu:
			return (void*)&m_sMenu;
		default:
			break;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OSVR_DirectMode::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((OSVR_Decommanders)dwDecommanderIndex)
	{
		case OSVR_Decommanders::LeftTexture11:
			m_sStereoTextureViews.m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case OSVR_Decommanders::RightTexture11:
			m_sStereoTextureViews.m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case OSVR_Decommanders::ZoomOut:
			m_pbZoomOut = (BOOL*)pData;
			break;
		case OSVR_Decommanders::ProjectionLeft:
			m_psProjection[0] = (D3DMATRIX*)pData;
			break;
		case OSVR_Decommanders::ProjectionRight:
			m_psProjection[1] = (D3DMATRIX*)pData;
			break;
		case OSVR_Decommanders::TargetWidth:
			m_punTexResolutionWidth = (UINT32*)pData;
			break;
		case OSVR_Decommanders::TargetHeight:
			m_punTexResolutionHeight = (UINT32*)pData;
			break;
	}
}

/**
* DirectMode supports IDXGISwapChain->Present().
***/
bool OSVR_DirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	// TODO !! ->Present() only
	return true;
}

/**
* Handle OSVR direct mode.
***/
void* OSVR_DirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;
	if (!m_bHotkeySwitch)
	{
		if (GetAsyncKeyState(VK_F11))
		{
			m_bHotkeySwitch = true;
		}
		return nullptr;
	}

	// Get an OSVR client context to use to access the devices
	// that we need.
	static osvr::clientkit::ClientContext m_pcClientContext
		= osvr::clientkit::ClientContext("com.mtbs3d.vireio.osvr.directmode");

	if (m_pcRenderManager == nullptr)
	{
		// get device and context
		ID3D11Device* pcDevice = nullptr;
		ID3D11DeviceContext* pcContext = nullptr;
		switch (m_eMethod)
		{
			case OSVR_undefined:
				// TODO !! determine method by used DX version
				return nullptr;
			case OSVR_D3D11_use_Game_Device:
				if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
				{
					OutputDebugString(L"OSVR-DirectMode: Failed to get d3d11 device + context");
					return nullptr;
				}
				break;
			case OSVR_D3D11_own_Device:
			case OSVR_D3D10_own_Device:
			case OSVR_D3D9_own_Device:
			{
										 // get game device + context
										 if (m_eMethod == OSVR_DirectModeMethods::OSVR_D3D11_own_Device)
										 {
											 if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &m_pcGameDevice, &m_pcGameDeviceContext)))
											 {
												 OutputDebugString(L"OSVR-DirectMode: Failed to get d3d11 device + context");
												 return nullptr;
											 }
											 m_pcGameDevice->Release();
											 m_pcGameDeviceContext->Release();
										 }

										 // Be sure to get D3D11 and have set
										 // D3D11_CREATE_DEVICE_BGRA_SUPPORT in the device/context
										 D3D_FEATURE_LEVEL acceptibleAPI = D3D_FEATURE_LEVEL_11_0;
										 D3D_FEATURE_LEVEL foundAPI;
										 auto hr =
											 D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
											 D3D11_CREATE_DEVICE_BGRA_SUPPORT, &acceptibleAPI, 1,
											 D3D11_SDK_VERSION, &m_pcDevice, &foundAPI, &m_pcDeviceContext);
										 if (FAILED(hr))
										 {
											 OutputDebugString(L"Could not create D3D11 device and context");
											 return nullptr;
										 }
										 pcDevice = m_pcDevice;
										 pcContext = m_pcDeviceContext;

			}
				break;
			default:
				break;
		}


		// Put the device and context into a structure to let RenderManager
		// know to use this one rather than creating its own.
		osvr::renderkit::GraphicsLibrary cLibrary;
		cLibrary.D3D11 = new osvr::renderkit::GraphicsLibraryD3D11;
		cLibrary.D3D11->device = pcDevice;
		cLibrary.D3D11->context = pcContext;
		if (m_eMethod == OSVR_DirectModeMethods::OSVR_D3D11_use_Game_Device)
		{
			if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
			if (pcContext) { pcContext->Release(); pcContext = nullptr; }
		}

		// Open Direct3D and set up the context for rendering to
		// an HMD.  Do this using the OSVR RenderManager interface,
		// which maps to the nVidia or other vendor direct mode
		// to reduce the latency.
		m_pcRenderManager = osvr::renderkit::createRenderManager(m_pcClientContext.get(), "Direct3D11", cLibrary);

		if ((m_pcRenderManager == nullptr) || (!m_pcRenderManager->doingOkay()))
		{
			// Error
			OutputDebugString(L"[OSVR] No render manager available !");
		}
		else
		{
			OutputDebugString(L"[OSVR] Set callback methods. ");

			// Set callback to handle setting up rendering in a display
			m_pcRenderManager->SetDisplayCallback((osvr::renderkit::DisplayCallback)&OSVR_DirectMode::SetupDisplay);

			// Register callback to render things in world space.
			m_pcRenderManager->AddRenderCallback("/", (osvr::renderkit::RenderCallback)&OSVR_DirectMode::DrawWorld);

			// Open the display and make sure this worked.
			osvr::renderkit::RenderManager::OpenResults ret = m_pcRenderManager->OpenDisplay();
			if (ret.status == osvr::renderkit::RenderManager::OpenStatus::FAILURE)
			{
				OutputDebugString(L"[OSVR] Could not open display");
			}
			if (ret.library.D3D11 == nullptr)
			{
				OutputDebugString(L"[OSVR] Attempted to run a Direct3D11 program with a config file that specified a different renderling library.");
			}

			// Do a call to get the information we need to construct our
			// color and depth render-to-texture buffers.
			std::vector<osvr::renderkit::RenderInfo> asRenderInfo;
			m_pcClientContext.update();
			asRenderInfo = m_pcRenderManager->GetRenderInfo();

			// Create the sampler state
			D3D11_SAMPLER_DESC sSampDesc;
			ZeroMemory(&sSampDesc, sizeof(sSampDesc));
			sSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sSampDesc.MinLOD = 0;
			sSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			if (FAILED(asRenderInfo[0].library.D3D11->device->CreateSamplerState(&sSampDesc, &m_pcSamplerState)))
				OutputDebugString(L"[OSVR] Failed to create Sampler State.");
		}
	}
	else
	{
		// get device and context
		ID3D11Device* pcDevice = nullptr;
		ID3D11DeviceContext* pcContext = nullptr;

		switch (m_eMethod)
		{
			case OSVR_undefined:
				return nullptr;
			case OSVR_D3D11_use_Game_Device:
				if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
				{
					OutputDebugString(L"[OSVR] Failed to get d3d11 device + context");
					// release frame texture+view
					if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
					if (pcContext) { pcContext->Release(); pcContext = nullptr; }
					return nullptr;
				}
				break;
			case OSVR_D3D11_own_Device:
			case OSVR_D3D10_own_Device:
			case OSVR_D3D9_own_Device:
				pcDevice = m_pcDevice;
				pcContext = m_pcDeviceContext;
				break;
			default:
				break;
		}

		// backup device states, if game device is used
		D3DX11_STATE_BLOCK sStateBlock;
		if (m_eMethod == OSVR_DirectModeMethods::OSVR_D3D11_use_Game_Device)
		{
			// backup all states
			CreateStateblock(pcContext, &sStateBlock);

			// clear all states
			pcContext->ClearState();
		}

		// Update the context so we get our callbacks called and
		// update tracker state.
		m_pcClientContext.update();

		if (!m_pcRenderManager->Render())
		{
			OutputDebugString(L"[OSVR] Render() returned false, maybe because it was asked to quit");
		}

		// apply state block, if game device is used
		if (m_eMethod == OSVR_DirectModeMethods::OSVR_D3D11_use_Game_Device)
		{
			// set back device
			ApplyStateblock(pcContext, &sStateBlock);

			if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
			if (pcContext) { pcContext->Release(); pcContext = nullptr; }
		}
	}
	return nullptr;
}

/**
* Callback to set up for rendering into a given display (which may have one or more eyes).
***/
void OSVR_DirectMode::SetupDisplay(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers)
{
	static UINT32 nRenderTargetWidth = 0;
	static UINT32 nRenderTargetHeight = 0;

	// Make sure our pointers are filled in correctly.  The config file selects
	// the graphics library to use, and may not match our needs.
	if (cLibrary.D3D11 == nullptr)
	{
		std::cerr << "SetupDisplay: No D3D11 GraphicsLibrary" << std::endl;
		return;
	}
	if (cBuffers.D3D11 == nullptr)
	{
		std::cerr << "SetupDisplay: No D3D11 RenderBuffer" << std::endl;
		return;
	}

	auto pcContext = cLibrary.D3D11->context;
	auto renderTargetView = cBuffers.D3D11->colorBufferView;
	auto depthStencilView = cBuffers.D3D11->depthStencilView;

	// Set up to render to the textures for this eye
	// RenderManager will have already set our render target to this
	// eye's buffer, so we don't need to do that here.
	FLOAT colorRgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pcContext->ClearRenderTargetView(renderTargetView, colorRgba);
	pcContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// render target widht/height ?
	if ((!nRenderTargetWidth) || (!nRenderTargetHeight))
	{
		// get only if requested by connected node
		if ((m_punTexResolutionHeight) && (m_punTexResolutionWidth))
		{
			ID3D11Texture2D* pcTexture = nullptr;
			if (renderTargetView)
				renderTargetView->GetResource((ID3D11Resource**)&pcTexture);
			if (pcTexture)
			{
				// get render target texture description
				D3D11_TEXTURE2D_DESC sDesc;
				pcTexture->GetDesc(&sDesc);

				// set width/height 
				nRenderTargetWidth = sDesc.Width;
				nRenderTargetHeight = sDesc.Height;
				*m_punTexResolutionWidth = nRenderTargetWidth;
				*m_punTexResolutionHeight = nRenderTargetHeight;

				// relase the texture
				pcTexture->Release();
			}
		}
	}
}

/**
* Callback to draw things in world space.
***/
void OSVR_DirectMode::DrawWorld(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers,
	osvr::renderkit::OSVR_ViewportDescription sViewport, OSVR_PoseState pose, osvr::renderkit::OSVR_ProjectionMatrix sProjection, OSVR_TimeValue deadline)
{
	static int nEye = 0;
	static float fAspect = 1.0f;

	// Make sure our pointers are filled in correctly.  The config file selects
	// the graphics library to use, and may not match our needs.
	if (cLibrary.D3D11 == nullptr)
	{
		std::cerr << "SetupDisplay: No D3D11 GraphicsLibrary" << std::endl;
		return;
	}
	if (cBuffers.D3D11 == nullptr)
	{
		std::cerr << "SetupDisplay: No D3D11 RenderBuffer" << std::endl;
		return;
	}

	// set projection matrix if node is connected
	if (m_psProjection[nEye])
	{
		D3DXMatrixPerspectiveOffCenterLH((D3DXMATRIX*)m_psProjection[nEye],
			(float)sProjection.left,
			(float)sProjection.right,
			(float)sProjection.bottom,
			(float)sProjection.top,
			(float)sProjection.nearClip,
			(float)sProjection.farClip);
	}

	// auto pcContext = cLibrary.D3D11->context;
	auto pcDevice = cLibrary.D3D11->device;
	auto pcContext = cLibrary.D3D11->context;

	// create all bool
	bool bAllCreated = true;

	// create vertex shader
	if (!m_pcVertexShader11)
	{
		if (FAILED(CreateVertexShaderTechnique(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11, VertexShaderTechnique::PosUV2D)))
		{
			OutputDebugString(L"FAILED");
			bAllCreated = false;
		}
	}
	// create pixel shader... 
	if (!m_pcPixelShader11)
	{
		if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
			bAllCreated = false;
	}
	// Create vertex buffer
	if (!m_pcVertexBuffer11)
	{
		if (FAILED(CreateFullScreenVertexBuffer(pcDevice, &m_pcVertexBuffer11)))
			bAllCreated = false;
	}
	// create constant buffer
	if (!m_pcConstantBufferDirect11)
	{
		if (FAILED(CreateMatrixConstantBuffer(pcDevice, &m_pcConstantBufferDirect11)))
			bAllCreated = false;
	}
	// sampler ?
	if (!m_pcSamplerState)
	{
		bAllCreated = false;
	}

	if ((bAllCreated) && (m_sStereoTextureViews.m_ppcTexView11[nEye]))
	{
		// Set the input layout
		pcContext->IASetInputLayout(m_pcVertexLayout11);

		// Set vertex buffer
		UINT stride = sizeof(TexturedVertex);
		UINT offset = 0;
		pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

		// get identity matrix here... we render fullscreen
		D3DXMATRIX sProj;
		D3DXMatrixIdentity(&sProj);

		// zoom out ?
		if (m_pbZoomOut)
		{
			if (*m_pbZoomOut)
			{
				sProj.m[0][0] /= 2.0f;
				sProj.m[1][1] /= 2.0f;
			}
		}

		// Set constant buffer, first update it... scale and translate the left and right image
		pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
		pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

		// Set primitive topology
		pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// texture connected ?
		if ((m_sStereoTextureViews.m_ppcTexView11[nEye]) && (*m_sStereoTextureViews.m_ppcTexView11[nEye]))
		{
			if (m_eMethod == OSVR_DirectModeMethods::OSVR_D3D11_use_Game_Device)
			{
				// set texture, sampler state
				pcContext->PSSetShaderResources(0, 1, m_sStereoTextureViews.m_ppcTexView11[nEye]);
				pcContext->PSSetSamplers(0, 1, &m_pcSamplerState);
			}
			else
			{
				ID3D11Resource* pcResource = nullptr;
				(*m_sStereoTextureViews.m_ppcTexView11[nEye])->GetResource(&pcResource);

				if (!m_sStereoTextureCopies.m_pcTex11Copy[nEye])
				{
					// get the description and create the copy texture
					D3D11_TEXTURE2D_DESC sDesc;
					((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);
					sDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
					sDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
					if (FAILED(((ID3D11Device*)m_pcGameDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&m_sStereoTextureCopies.m_pcTex11Copy[nEye])))
					{
						OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
						return;
					}

					// aspect ratio
					fAspect = (float)sDesc.Width / (float)sDesc.Height;

					// TODO !! DX9 // DX10 !!

					// get shared handle
					IDXGIResource* pcDXGIResource(NULL);
					m_sStereoTextureCopies.m_pcTex11Copy[nEye]->QueryInterface(__uuidof(IDXGIResource), (void**)&pcDXGIResource);
					HANDLE sharedHandle;
					if (pcDXGIResource)
					{
						pcDXGIResource->GetSharedHandle(&sharedHandle);
						pcDXGIResource->Release();
					}
					else OutputDebugString(L"Failed to query IDXGIResource.");

					// open the shared handle with the temporary device
					ID3D11Resource* pcResourceShared;
					pcDevice->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&pcResourceShared));
					if (pcResourceShared)
					{
						pcResourceShared->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_sStereoFrameTextures.m_pcFrameTexture[nEye]));
						pcResourceShared->Release();
					}
					else OutputDebugString(L"Could not open shared resource.");

					// create shader resource view
					if (m_sStereoFrameTextures.m_pcFrameTexture[nEye])
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC sDescSRV;
						ZeroMemory(&sDescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
						sDescSRV.Format = sDesc.Format;
						sDescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						sDescSRV.Texture2D.MostDetailedMip = 0;
						sDescSRV.Texture2D.MipLevels = 1;
						if (FAILED(pcDevice->CreateShaderResourceView(m_sStereoFrameTextures.m_pcFrameTexture[nEye], &sDescSRV, &m_sSteroFrameTextureSRViews.m_pcFrameTextureSRView[nEye])))
							OutputDebugString(L"Failed to create shader resource view.");
					}
					else OutputDebugString(L"No Texture available.");
				}
				else
				{
					// copy the frame tex to shared texture
					m_pcGameDeviceContext->CopyResource(m_sStereoTextureCopies.m_pcTex11Copy[nEye], pcResource);
					if (pcResource) pcResource->Release();

					// set texture, sampler state
					pcContext->PSSetShaderResources(0, 1, &m_sSteroFrameTextureSRViews.m_pcFrameTextureSRView[nEye]);
					pcContext->PSSetSamplers(0, 1, &m_pcSamplerState);
				}
			}
		}

		// set shaders
		pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
		pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

		// Render a triangle
		pcContext->Draw(6, 0);

		// switch eye for next call
		nEye = !nEye;
	}
}