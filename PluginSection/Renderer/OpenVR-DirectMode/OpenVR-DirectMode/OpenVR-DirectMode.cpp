/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OpenVR DirectMode - Open Virtual Reality Direct Mode Rendering Node
Copyright (C) 2016 Denis Reischl

File <OpenVR-DirectMode.cpp> and
Class <OpenVR-DirectMode> :
Copyright (C) 2016 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin
and Neil Schneider

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

#include"OpenVR-DirectMode.h"

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
OpenVR_DirectMode::OpenVR_DirectMode() : AQU_Nodus(),
m_pcDeviceTemporary(nullptr),
m_pcContextTemporary(nullptr),
m_pcSwapChainTemporary(nullptr),
m_pcBackBufferTemporary(nullptr),
m_bInit(false),
m_ppHMD(nullptr),
m_ulOverlayHandle(0),
m_ulOverlayThumbnailHandle(0),
m_pcTex11Copy(nullptr),
m_pcTex11CopyRTV(nullptr),
m_bHotkeySwitch(false),
m_pbZoomOut(nullptr)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;
	/*m_pcTex11Copy[0] = nullptr;
	m_pcTex11Copy[1] = nullptr;*/

	/*m_pcFrameTexture[0] = nullptr;
	m_pcFrameTexture[1] = nullptr;
	m_pcFrameTextureSRView[0] = nullptr;
	m_pcFrameTextureSRView[1] = nullptr;*/

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
OpenVR_DirectMode::~OpenVR_DirectMode()
{

}

/**
* Return the name of the  OpenVR DirectMode node.
***/
const char* OpenVR_DirectMode::GetNodeType()
{
	return "OpenVR DirectMode";
}

/**
* Returns a global unique identifier for the OpenVR DirectMode node.
***/
UINT OpenVR_DirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 321
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OpenVR DirectMode node.
***/
LPWSTR OpenVR_DirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the OpenVR DirectMode node.
***/
HBITMAP OpenVR_DirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OpenVR-DirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OpenVR DirectMode node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OpenVR_DirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OpenVR_DirectMode::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((OpenVR_Decommanders)dwDecommanderIndex)
	{
		case OpenVR_Decommanders::LeftTexture11:
			return L"Left Texture";
		case OpenVR_Decommanders::RightTexture11:
			return L"Right Texture";
		case OpenVR_Decommanders::IVRSystem:
			return L"IVRSystem";
		case OpenVR_Decommanders::ZoomOut:
			return L"Zoom Out";
	}

	return L"x";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OpenVR_DirectMode::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((OpenVR_Decommanders)dwDecommanderIndex)
	{
		case OpenVR_Decommanders::LeftTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case OpenVR_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case OpenVR_Decommanders::IVRSystem:
			return NOD_Plugtype::AQU_HANDLE;
		case OpenVR_Decommanders::ZoomOut:
			return NOD_Plugtype::AQU_BOOL;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OpenVR_DirectMode::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((OpenVR_Decommanders)dwDecommanderIndex)
	{
		case OpenVR_Decommanders::LeftTexture11:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case OpenVR_Decommanders::RightTexture11:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case OpenVR_Decommanders::IVRSystem:
			m_ppHMD = (vr::IVRSystem**)pData;
			break;
		case OpenVR_Decommanders::ZoomOut:
			m_pbZoomOut = (BOOL*)pData;
			break;
	}
}

/**
* DirectMode supports any calls.
***/
bool OpenVR_DirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OpenVR tracking.
***/
void* OpenVR_DirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{

	static float fAspectRatio = 1.0f;
	static float bAspectRatio = false;
	static float fHorizontalRatioCorrectionLeft = 0.0f, fHorizontalRatioCorrectionRight = 0.0f;
	static float fHorizontalOffsetCorrectionLeft = 0.0f, fHorizontalOffsetCorrectionRight = 0.0f;

	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;
	/*if (!m_bHotkeySwitch)
	{
	if (GetAsyncKeyState(VK_F11))
	{
	m_bHotkeySwitch = true;
	}
	return nullptr;
	}*/

	// calculate aspect ratio + offset correction
	if (!bAspectRatio)
	{
		// texture connected ?
		if ((m_ppcTexView11[0]) && (*m_ppcTexView11[0]))
		{
			ID3D11Resource* pcResource = nullptr;
			(*m_ppcTexView11[0])->GetResource(&pcResource);
			if (pcResource)
			{
				// get the aspect ratio
				D3D11_TEXTURE2D_DESC sDesc;
				((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);
				fAspectRatio = (float)sDesc.Width / (float)sDesc.Height;
				pcResource->Release();

				// compute left eye 
				float fLeft, fRight, fTop, fBottom;
				(*m_ppHMD)->GetProjectionRaw(vr::Eye_Left, &fLeft, &fRight, &fTop, &fBottom);
				float fHorizontal = fRight - fLeft;
				float fVertical = fBottom - fTop;
				float fAspectHMD = fHorizontal / fVertical;
				fHorizontalRatioCorrectionLeft = (1.0f - (fAspectHMD / fAspectRatio)) / 2.0f;
				fHorizontalOffsetCorrectionLeft = abs(fRight) - abs(fLeft);
				fHorizontalOffsetCorrectionLeft /= fHorizontal;
				fHorizontalOffsetCorrectionLeft *= fAspectHMD / fAspectRatio;

				// compute right eye 
				(*m_ppHMD)->GetProjectionRaw(vr::Eye_Right, &fLeft, &fRight, &fTop, &fBottom);
				fHorizontal = fRight - fLeft;
				fVertical = fBottom - fTop;
				fAspectHMD = fHorizontal / fVertical;
				fHorizontalRatioCorrectionRight = (1.0f - (fAspectHMD / fAspectRatio)) / 2.0f;
				fHorizontalOffsetCorrectionRight = abs(fRight) - abs(fLeft);
				fHorizontalOffsetCorrectionRight /= fHorizontal;
				fHorizontalOffsetCorrectionRight *= fAspectHMD / fAspectRatio;

				bAspectRatio = true;
			}
		}
	}

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

	if (m_ppHMD)
	{
		if (*m_ppHMD)
		{
			if (!m_bInit)
			{
#pragma region Init
				if (!vr::VRCompositor())
				{
					OutputDebugString(L"OpenVR: Compositor initialization failed.\n");
					return nullptr;
				}

				// create the overlay
				vr::VROverlayError overlayError = vr::VROverlay()->CreateDashboardOverlay(OPENVR_OVERLAY_NAME, OPENVR_OVERLAY_FRIENDLY_NAME, &m_ulOverlayHandle, &m_ulOverlayThumbnailHandle);
				if (overlayError != vr::VROverlayError_None)
				{
					OutputDebugString(L"OpenVR: Failed to create overlay.");
					return nullptr;
				}

				// set overlay options
				vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 3.0f);
				vr::VROverlay()->SetOverlayInputMethod(m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse);
				vr::VROverlay()->SetOverlayAlpha(m_ulOverlayHandle, 1.0f);
				vr::VROverlay()->SetOverlayColor(m_ulOverlayHandle, 1.0f, 1.0f, 1.0f);

				m_bInit = true;
#pragma endregion
			}
			else
			if (vr::VRCompositor()->CanRenderScene())
			{
#pragma region Render overlay
				if (vr::VROverlay() && vr::VROverlay()->IsOverlayVisible(m_ulOverlayHandle))
				{
					// create all bool
					bool bAllCreated = true;

					// create vertex shader
					if (!m_pcVertexShader11)
					{
						if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
						{
							bAllCreated = false;
						}
					}
					// create pixel shader... 
					if (!m_pcPixelShader11)
					{
						if (FAILED(CreateSimplePixelShader(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
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
						if (FAILED(pcDevice->CreateSamplerState(&sSampDesc, &m_pcSamplerState)))
							bAllCreated = false;
					}

					// texture connected ?
					if ((m_ppcTexView11[0]) && (*m_ppcTexView11[0]))
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

						if (bAllCreated)
						{

							if (!m_pcTex11Copy)
							{
								ID3D11Resource* pcResource = nullptr;
								(*m_ppcTexView11[0])->GetResource(&pcResource);
								if (pcResource)
								{
									// get the description and create the copy texture
									D3D11_TEXTURE2D_DESC sDesc;
									((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);
									pcResource->Release();

									sDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
									if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&m_pcTex11Copy)))
									{
										OutputDebugString(L"OpenVR : Failed to create overlay texture !");
										return nullptr;
									}
									pcDevice->CreateRenderTargetView((ID3D11Resource*)m_pcTex11Copy, NULL, &m_pcTex11CopyRTV);
								}
							}


							// set and clear render target
							FLOAT afColorRgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
							pcContext->ClearRenderTargetView(m_pcTex11CopyRTV, afColorRgba);
							pcContext->OMSetRenderTargets(1, &m_pcTex11CopyRTV, nullptr);

							// set texture, sampler state
							pcContext->PSSetShaderResources(0, 1, m_ppcTexView11[0]);
							pcContext->PSSetSamplers(0, 1, &m_pcSamplerState);

							// set shaders
							pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
							pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

							// Render a triangle
							pcContext->Draw(6, 0);

							// fill openvr texture struct
							vr::Texture_t sLeftEyeTexture = { (void*)m_pcTex11Copy, vr::API_DirectX, vr::ColorSpace_Gamma };
							vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &sLeftEyeTexture);
						}
					}
				}
#pragma endregion
#pragma region Render
				else
				if (!vr::VROverlay()->IsDashboardVisible())
				{
					// call WaitGetPoses here to get the scene focus
					vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

					// texture connected ?
					if ((m_ppcTexView11[0]) && (*m_ppcTexView11[0]))
					{
						ID3D11Resource* pcResource = nullptr;
						(*m_ppcTexView11[0])->GetResource(&pcResource);
						if (pcResource)
						{
							// fill openvr texture struct
							vr::Texture_t sLeftEyeTexture = { (void*)pcResource, vr::API_DirectX, vr::ColorSpace_Gamma };

							// adjust aspect ratio
							vr::VRTextureBounds_t sBounds;
							sBounds.uMin = fHorizontalRatioCorrectionLeft + fHorizontalOffsetCorrectionLeft;
							sBounds.uMax = 1.0f - fHorizontalRatioCorrectionLeft + fHorizontalOffsetCorrectionLeft;
							sBounds.vMin = 0.f;
							sBounds.vMax = 1.f;

							// submit left texture
							vr::VRCompositor()->Submit(vr::Eye_Left, &sLeftEyeTexture, &sBounds);

							pcResource->Release();
						}
					}

					// texture connected ?
					if ((m_ppcTexView11[1]) && (*m_ppcTexView11[1]))
					{
						ID3D11Resource* pcResource = nullptr;
						(*m_ppcTexView11[1])->GetResource(&pcResource);
						if (pcResource)
						{
							// fill openvr texture struct
							vr::Texture_t sRightEyeTexture = { (void*)pcResource, vr::API_DirectX, vr::ColorSpace_Gamma };

							// adjust aspect ratio
							vr::VRTextureBounds_t sBounds;
							sBounds.uMin = fHorizontalRatioCorrectionRight + fHorizontalOffsetCorrectionRight;
							sBounds.uMax = 1.0f - fHorizontalRatioCorrectionRight + fHorizontalOffsetCorrectionRight;
							sBounds.vMin = 0.f;
							sBounds.vMax = 1.f;

							// submit right texture
							vr::VRCompositor()->Submit(vr::Eye_Right, &sRightEyeTexture, &sBounds);

							pcResource->Release();
						}
					}
				}
#pragma endregion
			}
			else
				OutputDebugString(L"OpenVR: Current process has NOT the scene focus.");
		}
	}

	// release d3d11 device + context... 
	pcContext->Release();
	pcDevice->Release();

	return nullptr;
}