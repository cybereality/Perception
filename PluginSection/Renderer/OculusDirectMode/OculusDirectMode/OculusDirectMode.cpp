/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift DirectMode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.cpp> and
Class <OculusDirectMode> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from the Oculus Rift sample
>OculusRoomTiny< (Author Tom Heath - Copyright :
Copyright 2015 Oculus, Inc. All Rights reserved.
http://www.apache.org/licenses/LICENSE-2.0)
and from the Vireio source code originally authored by Simon Brown.
(class OculusDirectToRiftView v3.0.0 2015).

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

#include"OculusDirectMode.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

#define INTERFACE_IDIRECT3DDEVICE9           8
#define INTERFACE_IDIRECT3DSWAPCHAIN9       15
#define INTERFACE_IDXGISWAPCHAIN            29

#define	METHOD_IDIRECT3DDEVICE9_PRESENT     17
#define	METHOD_IDIRECT3DDEVICE9_ENDSCENE    42
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT   3
#define METHOD_IDXGISWAPCHAIN_PRESENT        8

/**
* Constructor.
***/
OculusDirectMode::OculusDirectMode() : AQU_Nodus(),
m_pcDeviceTemporary(nullptr),
m_pcContextTemporary(nullptr),
m_pcSwapChainTemporary(nullptr),
m_pcBackBufferTemporary(nullptr),
m_bInit(false),
m_pcMirrorTexture(nullptr)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;
	m_pcTex11Copy[0] = nullptr;
	m_pcTex11Copy[1] = nullptr;

	m_pcFrameTexture[0] = nullptr;
	m_pcFrameTexture[1] = nullptr;
	m_pcFrameTextureSRView[0] = nullptr;
	m_pcFrameTextureSRView[1] = nullptr;

	m_pcVertexShader11 = nullptr;
	m_pcPixelShader11 = nullptr;
	m_pcVertexLayout11 = nullptr;
	m_pcVertexBuffer11 = nullptr;
	m_pcConstantBufferDirect11 = nullptr;
	m_pcSamplerState = nullptr;
}

/**
* Destructor.
***/
OculusDirectMode::~OculusDirectMode()
{
	if (m_pcBackBufferRTVTemporary) m_pcBackBufferRTVTemporary->Release();
	if (m_pcBackBufferTemporary) m_pcBackBufferTemporary->Release();
	if (m_pcSwapChainTemporary) m_pcSwapChainTemporary->Release();
	if (m_pcContextTemporary) m_pcContextTemporary->Release();
	if (m_pcDeviceTemporary) m_pcDeviceTemporary->Release();

	if (m_pcVertexShader11) m_pcVertexShader11->Release();
	if (m_pcPixelShader11) m_pcPixelShader11->Release();
}

/**
* Return the name of the  Oculus Renderer node.
***/
const char* OculusDirectMode::GetNodeType()
{
	return "Oculus Direct Mode";
}

/**
* Returns a global unique identifier for the Oculus Renderer node.
***/
UINT OculusDirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 259
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Oculus Renderer node.
***/
LPWSTR OculusDirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the Oculus Renderer node.
***/
HBITMAP OculusDirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OculusDirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Oculus Renderer node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OculusDirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OculusDirectMode::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture:
			return L"Left Texture";
		case ODM_Decommanders::RightTexture:
			return L"Right Texture";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OculusDirectMode::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case ODM_Decommanders::RightTexture:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OculusDirectMode::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case ODM_Decommanders::RightTexture:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
	}
}

/**
* DirectMode supports IDXGISwapChain->Present().
***/
bool OculusDirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	// TODO !! ->Present() only
	return true;
}

/**
* Render the Oculus Rift View.
***/
void* OculusDirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;

	// cast swapchain
	IDXGISwapChain* pcSwapChain = (IDXGISwapChain*)pThis;
	if (!pcSwapChain)
	{
		OutputDebugString(L"Oculus Direct Mode Node : No swapchain !");
		return nullptr;
	}
	// get device
	ID3D11Device* pcDevice = nullptr;
	pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
	if (!pcDevice)
	{
		OutputDebugString(L"Oculus Direct Mode Node : No d3d 11 device !");
		return nullptr;
	}
	// get context
	ID3D11DeviceContext* pcContext = nullptr;
	pcDevice->GetImmediateContext(&pcContext);
	if (!pcContext)
	{
		OutputDebugString(L"Oculus Direct Mode Node : No device context !");
		return nullptr;
	}

	if (!m_bInit)
	{
#pragma region Init
		// Initialize LibOVR
		ovrResult result = ovr_Initialize(nullptr);
		if (!OVR_SUCCESS(result))
		{
			OutputDebugString(L"Failed to initialize libOVR.");
			return nullptr;
		}

		// create hmd (or session) handle
		result = ovr_Create(&m_hHMD, &m_sLuid);
		if (!OVR_SUCCESS(result))
			return nullptr;

		// get HMD description
		m_sHMDDesc = ovr_GetHmdDesc(m_hHMD);

		// create temporary (OVR) device
		IDXGIFactory * DXGIFactory;
		IDXGIAdapter * Adapter;
		if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
			return(false);
		if (FAILED(DXGIFactory->EnumAdapters(0, &Adapter)))
			return(false);
		if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pcDeviceTemporary, NULL, &m_pcContextTemporary)))
			return(false);
		DXGIFactory->Release();

		// Set max frame latency to 1
		IDXGIDevice1* DXGIDevice1 = NULL;
		HRESULT hr = m_pcDeviceTemporary->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
		if (FAILED(hr) | (DXGIDevice1 == NULL)) return(false);
		DXGIDevice1->SetMaximumFrameLatency(1);
		DXGIDevice1->Release();

		// Set a standard blend state, ie transparency from alpha
		D3D11_BLEND_DESC bm;
		memset(&bm, 0, sizeof(bm));
		bm.RenderTarget[0].BlendEnable = TRUE;
		bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState * BlendState;
		m_pcDeviceTemporary->CreateBlendState(&bm, &BlendState);
		m_pcContextTemporary->OMSetBlendState(BlendState, NULL, 0xffffffff);

		// Make the eye render buffers (caution if actual size < requested due to HW limits). 
		for (int eye = 0; eye < 2; ++eye)
		{
			ovrSizei sIdealSize = ovr_GetFovTextureSize(m_hHMD, (ovrEyeType)eye, m_sHMDDesc.DefaultEyeFov[eye], 1.0f);

			m_psEyeRenderTexture[eye] = new OculusTexture();
			if (!m_psEyeRenderTexture[eye]->Init(m_pcDeviceTemporary, m_hHMD, sIdealSize.w, sIdealSize.h))
			{
				OutputDebugString(L"Failed to create eye texture.");
				m_bInit = true;
				return nullptr;
			}
			m_psEyeRenderViewport[eye].Pos.x = 0;
			m_psEyeRenderViewport[eye].Pos.y = 0;
			m_psEyeRenderViewport[eye].Size = sIdealSize;
			if (!m_psEyeRenderTexture[eye]->TextureSet)
			{
				OutputDebugString(L"Failed to create texture.");
				m_bInit = true;
				return nullptr;
			}
		}

		// Setup VR components, filling out description
		m_psEyeRenderDesc[0] = ovr_GetRenderDesc(m_hHMD, ovrEye_Left, m_sHMDDesc.DefaultEyeFov[0]);
		m_psEyeRenderDesc[1] = ovr_GetRenderDesc(m_hHMD, ovrEye_Right, m_sHMDDesc.DefaultEyeFov[1]);

		m_bInit = true;
#pragma endregion
	}
	else
	{
#pragma region Render

		// get eye render pose and other fields
		ovrVector3f      asHmdToEyeViewOffset[2] = { m_psEyeRenderDesc[0].HmdToEyeViewOffset,
			m_psEyeRenderDesc[1].HmdToEyeViewOffset };
		ovrPosef         asEyeRenderPose[2];
		ovrPosef         ZeroPose; ZeroMemory(&ZeroPose, sizeof(ovrPosef));
		ovrTrackingState sHmdState = ovr_GetTrackingState(m_hHMD, ovr_GetTimeInSeconds(), false);
		ovr_CalcEyePoses(sHmdState.HeadPose.ThePose, asHmdToEyeViewOffset, asEyeRenderPose);
		FLOAT colorBlack[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		double sensorSampleTime = ovr_GetTimeInSeconds();

		// render
		for (int eye = 0; eye < 2; eye++)
		{
			// Increment to use next texture, just before writing
			m_psEyeRenderTexture[eye]->AdvanceToNextTexture();

			// set viewport
			D3D11_VIEWPORT sD3Dvp;
			sD3Dvp.Width = (float)m_psEyeRenderViewport[eye].Size.w;
			sD3Dvp.Height = (float)m_psEyeRenderViewport[eye].Size.h;
			sD3Dvp.MinDepth = 0;
			sD3Dvp.MaxDepth = 1;
			sD3Dvp.TopLeftX = (float)m_psEyeRenderViewport[eye].Pos.x;
			sD3Dvp.TopLeftY = (float)m_psEyeRenderViewport[eye].Pos.y;
			m_pcContextTemporary->RSSetViewports(1, &sD3Dvp);

			// clear and set render target
			int texIndex = m_psEyeRenderTexture[eye]->TextureSet->CurrentIndex;
			m_pcContextTemporary->OMSetRenderTargets(1, &m_psEyeRenderTexture[eye]->TexRtv[texIndex], nullptr);
			m_pcContextTemporary->ClearRenderTargetView(m_psEyeRenderTexture[eye]->TexRtv[texIndex], colorBlack);

			// texture connected ?
			if ((m_ppcTexView11[eye]) && (*m_ppcTexView11[eye]))
			{
				ID3D11Resource* pcResource = nullptr;
				(*m_ppcTexView11[eye])->GetResource(&pcResource);

				if (!m_pcTex11Copy[eye])
				{
					// get the description and create the copy texture
					D3D11_TEXTURE2D_DESC sDesc;
					((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);
					sDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
					sDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
					if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&m_pcTex11Copy[eye])))
					{
						OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
						break;
					}

					// get shared handle
					IDXGIResource* pcDXGIResource(NULL);
					m_pcTex11Copy[eye]->QueryInterface(__uuidof(IDXGIResource), (void**)&pcDXGIResource);
					HANDLE sharedHandle;
					if (pcDXGIResource)
					{
						pcDXGIResource->GetSharedHandle(&sharedHandle);
						pcDXGIResource->Release();
					} else OutputDebugString(L"Failed to query IDXGIResource.");

					// open the shared handle with the temporary device
					ID3D11Resource* pcResourceShared;
					m_pcDeviceTemporary->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&pcResourceShared));
					if (pcResourceShared)
					{
						pcResourceShared->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_pcFrameTexture[eye]));
						pcResourceShared->Release();
					} else OutputDebugString(L"Could not open shared resource.");

					// create shader resource view
					if (m_pcFrameTexture[eye])
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC sDescSRV;
						ZeroMemory(&sDescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
						sDescSRV.Format = sDesc.Format;
						sDescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						sDescSRV.Texture2D.MostDetailedMip = 0;
						sDescSRV.Texture2D.MipLevels = 1;
						if (FAILED(m_pcDeviceTemporary->CreateShaderResourceView(m_pcFrameTexture[eye], &sDescSRV, &m_pcFrameTextureSRView[eye])))
							OutputDebugString(L"Failed to create shader resource view.");
					}
					else OutputDebugString(L"No Texture available.");
				}
				else
				{
					// copy the frame tex to shared texture
					pcContext->CopyResource(m_pcTex11Copy[eye], pcResource);

					// create all bool
					bool bAllCreated = true;

					// create vertex shader
					if (!m_pcVertexShader11)
					{
						if (FAILED(Create2DVertexShader(m_pcDeviceTemporary, &m_pcVertexShader11, &m_pcVertexLayout11)))
						{
							OutputDebugString(L"FAILED");
							bAllCreated = false;
						}
					}

					// create pixel shader... 
					if (!m_pcPixelShader11)
					{
						if (FAILED(CreateSimplePixelShader(m_pcDeviceTemporary, &m_pcPixelShader11, PixelShaderTechnique::FullscreenChangeAspectRatio)))
							bAllCreated = false;
					}

					// Create vertex buffer
					if (!m_pcVertexBuffer11)
					{
						if (FAILED(CreateFullScreenVertexBuffer(m_pcDeviceTemporary, &m_pcVertexBuffer11)))
							bAllCreated = false;
					}

					// create constant buffer
					if (!m_pcConstantBufferDirect11)
					{
						if (FAILED(CreateMatrixConstantBuffer(m_pcDeviceTemporary, &m_pcConstantBufferDirect11)))
							bAllCreated = false;
					}

					// sampler ?
					if (!m_pcSamplerState)
					{
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
						if (FAILED(m_pcDeviceTemporary->CreateSamplerState(&sSampDesc, &m_pcSamplerState)))
						{
							OutputDebugString(L"Failed to create Sampler State.");
							bAllCreated = false;
						}
					}

					if (bAllCreated)
					{
						// Set the input layout
						m_pcContextTemporary->IASetInputLayout(m_pcVertexLayout11);

						// Set vertex buffer
						UINT stride = sizeof(TexturedVertex);
						UINT offset = 0;
						m_pcContextTemporary->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

						// Set constant buffer, first update it... scale and translate the left and right image
						OVR::Matrix4f proj = OVR::Matrix4f::Translation(0.41f - eye*0.82f, 0, 0);
						m_pcContextTemporary->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &proj, 0, 0);
						m_pcContextTemporary->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

						// Set primitive topology
						m_pcContextTemporary->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						// set texture, sampler state
						m_pcContextTemporary->PSSetShaderResources(0, 1, &m_pcFrameTextureSRView[eye]);
						m_pcContextTemporary->PSSetSamplers(0, 1, &m_pcSamplerState);

						// set shaders
						m_pcContextTemporary->VSSetShader(m_pcVertexShader11, 0, 0);
						m_pcContextTemporary->PSSetShader(m_pcPixelShader11, 0, 0);

						// Render a triangle
						m_pcContextTemporary->Draw(6, 0);
					}


				}

				if (pcResource) pcResource->Release();
			}
		}

		// Initialize our single full screen Fov layer.
		ovrLayerEyeFov ld = {};
		ld.Header.Type = ovrLayerType_EyeFov;
		ld.Header.Flags = 0;

		for (int eye = 0; eye < 2; ++eye)
		{
			ld.ColorTexture[eye] = m_psEyeRenderTexture[eye]->TextureSet;
			ld.Viewport[eye] = m_psEyeRenderViewport[eye];
			ld.Fov[eye] = m_sHMDDesc.DefaultEyeFov[eye];
			ld.RenderPose[eye] = asEyeRenderPose[eye];
			ld.SensorSampleTime = sensorSampleTime;
		}
		ovrLayerHeader* layers = &ld.Header;
		ovrResult result = ovr_SubmitFrame(m_hHMD, 0, nullptr, &layers, 1);
#pragma endregion
	}

	// release d3d11 device + context... 
	pcContext->Release();
	pcDevice->Release();

	return nullptr;
}