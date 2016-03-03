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

#define INTERFACE_IDIRECT3DDEVICE9           8

#pragma region OSVR_DirectMode static fields
ID3D11VertexShader* OSVR_DirectMode::m_pcVertexShader11;
ID3D11PixelShader* OSVR_DirectMode::m_pcPixelShader11;
ID3D11InputLayout* OSVR_DirectMode::m_pcVertexLayout11;
ID3D11Buffer* OSVR_DirectMode::m_pcVertexBuffer11;
ID3D11Buffer* OSVR_DirectMode::m_pcConstantBufferDirect11;
ID3D11Texture2D* OSVR_DirectMode::m_pcTextureTest;
ID3D11ShaderResourceView* OSVR_DirectMode::m_pcTextureTestView;
ID3D11SamplerState* OSVR_DirectMode::m_pcSamplerState;
#pragma endregion

/**
* Constructor.
***/
OSVR_DirectMode::OSVR_DirectMode() :AQU_Nodus(),
m_pcRenderManager(nullptr)
{
	m_pcVertexShader11 = nullptr;
	m_pcPixelShader11 = nullptr;
	m_pcVertexLayout11 = nullptr;
	m_pcVertexBuffer11 = nullptr;
	m_pcConstantBufferDirect11 = nullptr;
	m_pcTextureTest = nullptr;
	m_pcTextureTestView = nullptr;
	m_pcSamplerState = nullptr;
}

/**
* Destructor.
***/
OSVR_DirectMode::~OSVR_DirectMode()
{
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
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OSVR_DirectMode::GetCommanderType(DWORD dwCommanderIndex)
{
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return 0;
}

/**
* Provides the pointer of the requested commander.
***/
void* OSVR_DirectMode::GetOutputPointer(DWORD dwCommanderIndex)
{
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return nullptr;
}

/**
* DirectMode supports any calls.
***/
bool OSVR_DirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OSVR direct mode.
***/
void* OSVR_DirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	// Get an OSVR client context to use to access the devices
	// that we need.
	static osvr::clientkit::ClientContext m_pcClientContext = osvr::clientkit::ClientContext(
		"com.mtbs3d.vireio.osvr.directmode");

	if (m_pcRenderManager == nullptr)
	{
		// Open Direct3D and set up the context for rendering to
		// an HMD.  Do this using the OSVR RenderManager interface,
		// which maps to the nVidia or other vendor direct mode
		// to reduce the latency.
		m_pcRenderManager = osvr::renderkit::createRenderManager(m_pcClientContext.get(), "Direct3D11");

		if ((m_pcRenderManager == nullptr) || (!m_pcRenderManager->doingOkay()))
		{
			// Error
			OutputDebugString(L"OSVR-DirectMode: [Error] No Render Manager available !");
		}
		else
		{
			// Set callback to handle setting up rendering in a display
			m_pcRenderManager->SetDisplayCallback((osvr::renderkit::DisplayCallback)&OSVR_DirectMode::SetupDisplay);

			// Register callback to render things in world space.
			m_pcRenderManager->AddRenderCallback("/", (osvr::renderkit::RenderCallback)&OSVR_DirectMode::DrawWorld);

			// Open the display and make sure this worked.
			osvr::renderkit::RenderManager::OpenResults ret = m_pcRenderManager->OpenDisplay();
			if (ret.status == osvr::renderkit::RenderManager::OpenStatus::FAILURE)
			{
				OutputDebugString(L"Could not open display");
			}
			if (ret.library.D3D11 == nullptr)
			{
				OutputDebugString(L"Attempted to run a Direct3D11 program with a config file that specified a different renderling library.");
			}

			// Do a call to get the information we need to construct our
			// color and depth render-to-texture buffers.
			std::vector<osvr::renderkit::RenderInfo> asRenderInfo;
			m_pcClientContext.update();
			asRenderInfo = m_pcRenderManager->GetRenderInfo();

			// Set up the vector of textures to render to and any framebuffer
			// we need to group them.
			std::vector<osvr::renderkit::RenderBuffer> renderBuffers;
			std::vector<ID3D11Texture2D*> depthStencilTextures;
			std::vector<ID3D11DepthStencilView*> depthStencilViews;

			// Construct test texture
			D3DCOLOR tex_pixels[256 * 256];
			int k = 1;
			for (int j = 0; j < 256; j++)
			for (int i = 0; i < 256; i++)
			{
				if (k == 0) tex_pixels[j * 256 + i] =
					(((i >> 7) ^ (j >> 7)) & 1) ?
					D3DCOLOR_ARGB(180, 180, 180, 255) : D3DCOLOR_ARGB(80, 80, 80, 255);
				if (k == 1) tex_pixels[j * 256 + i] =
					(((j / 4 & 15) == 0) || (((i / 4 & 15) == 0) && ((((i / 4 & 31) == 0) ^ ((j / 4 >> 4) & 1)) == 0))) ?
					D3DCOLOR_ARGB(60, 60, 60, 255) : D3DCOLOR_ARGB(180, 180, 180, 255);
				if (k == 2) tex_pixels[j * 256 + i] =
					(i / 4 == 0 || j / 4 == 0) ?
					D3DCOLOR_ARGB(80, 80, 80, 255) : D3DCOLOR_ARGB(180, 180, 180, 255);
				if (k == 3) tex_pixels[j * 256 + i] =
					D3DCOLOR_ARGB(128, 128, 128, 255);
			}

			//  Note that this texture format must be RGBA and unsigned byte,
			// so that we can present it to Direct3D for DirectMode.
			unsigned width = static_cast<int>(asRenderInfo[0].viewport.width);
			unsigned height = static_cast<int>(asRenderInfo[0].viewport.height);

			// Initialize a new texture description.
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = 256;
			textureDesc.Height = 256;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags =
				D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA sData;
			ZeroMemory(&sData, sizeof(sData));
			sData.pSysMem = tex_pixels;
			sData.SysMemPitch = 256 * sizeof(BYTE)* 4;

			// Create a new render target texture to use.
			HRESULT hr = asRenderInfo[0].library.D3D11->device->CreateTexture2D(
				&textureDesc, &sData, &m_pcTextureTest);

			if (SUCCEEDED(hr))
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC sSRVDesc;
				memset(&sSRVDesc, 0, sizeof(sSRVDesc));
				sSRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				sSRVDesc.Texture2D.MipLevels = 1;

				if (FAILED(asRenderInfo[0].library.D3D11->device->CreateShaderResourceView(m_pcTextureTest, &sSRVDesc, &m_pcTextureTestView)))
					OutputDebugString(L"Failed to create Texture Resource View.");

				// Create the sample state
				D3D11_SAMPLER_DESC sSampDesc;
				ZeroMemory(&sSampDesc, sizeof(sSampDesc));
				sSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				sSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				sSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				sSampDesc.MinLOD = 0;
				sSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
				hr = asRenderInfo[0].library.D3D11->device->CreateSamplerState(&sSampDesc, &m_pcSamplerState);
				if (FAILED(hr))
					OutputDebugString(L"Failed to create Sampler State.");
			}
			else
			{
				OutputDebugString(L"Failed to create texture !");
			}
		}
	}
	else
	{
		// Update the context so we get our callbacks called and
		// update tracker state.
		m_pcClientContext.update();

		if (!m_pcRenderManager->Render())
		{
			OutputDebugString(L"Render() returned false, maybe because it was asked to quit");
		}
	}
	return nullptr;
}

/**
* Callback to set up for rendering into a given display (which may have one or more eyes).
***/
void OSVR_DirectMode::SetupDisplay(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers)
{
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

	// Perform a random colorfill.  This does not have to be random, but
	// random draws attention if we leave any background showing.
	FLOAT red = static_cast<FLOAT>((double)rand() / (double)RAND_MAX);
	FLOAT green = static_cast<FLOAT>((double)rand() / (double)RAND_MAX);
	FLOAT blue = static_cast<FLOAT>((double)rand() / (double)RAND_MAX);
	FLOAT colorRgba[4] = { 0.3f * red, 0.3f * green, 0.3f * blue, 1.0f };
	pcContext->ClearRenderTargetView(renderTargetView, colorRgba);
	pcContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/**
* Callback to draw things in world space.
***/
void OSVR_DirectMode::DrawWorld(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers,
	osvr::renderkit::OSVR_ViewportDescription sViewport, OSVR_PoseState pose, osvr::renderkit::OSVR_ProjectionMatrix sProjection, OSVR_TimeValue deadline)
{
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

	// auto pcContext = cLibrary.D3D11->context;
	auto pcDevice = cLibrary.D3D11->device;
	auto pcContext = cLibrary.D3D11->context;

	// create all bool
	bool bAllCreated = true;

	// create vertex shader
	if (!m_pcVertexShader11)
	{
		if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
		{
			OutputDebugString(L"FAILED");
			bAllCreated = false;
		}
	}
	// create pixel shader... 
	if (!m_pcPixelShader11)
	{
		if (FAILED(CreateSimplePixelShader(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::SideBySide)))
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
	// texture ?
	if (!m_pcTextureTest)
	{
		bAllCreated = false;
	}
	// sampler ?
	if (!m_pcSamplerState)
	{
		bAllCreated = false;
	}


	if (bAllCreated)
	{
		// Set the input layout
		pcContext->IASetInputLayout(m_pcVertexLayout11);

		// Set vertex buffer
		UINT stride = sizeof(TexturedVertex);
		UINT offset = 0;
		pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

		// Set constant buffer, first update it... scale and translate the left and right image
		D3DXMATRIX sProj;
		D3DXMatrixIdentity(&sProj);
		pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
		pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

		// Set primitive topology
		pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// set texture, sampler state
		pcContext->PSSetShaderResources(0, 1, &m_pcTextureTestView);
		pcContext->PSSetSamplers(0, 1, &m_pcSamplerState);

		// set shaders
		pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
		pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

		// Render a triangle
		pcContext->Draw(6, 0);
	}
}