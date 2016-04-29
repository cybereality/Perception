/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.cpp> and
Class <VireioStereoPresenter> :
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

#include"VireioStereoPresenter.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_IDIRECT3DDEVICE9                                           8
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

/**
* Constructor.
***/
StereoPresenter::StereoPresenter() :AQU_Nodus(),
m_pcBackBufferView(nullptr),
m_pcVertexShader10(nullptr),
m_pcPixelShader10(nullptr),
m_pcVertexLayout10(nullptr),
m_pcVertexBuffer10(nullptr),
m_pcConstantBufferDirect10(nullptr),
m_bHotkeySwitch(false),
m_eStereoMode(VireioMonitorStereoModes::Vireio_Mono)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;

	m_pfEuler[0] = nullptr;
	m_pfEuler[1] = nullptr;
	m_pfEuler[2] = nullptr;
	m_pfPosition[0] = nullptr;
	m_pfPosition[1] = nullptr;
	m_pfPosition[2] = nullptr;
}

/**
* Destructor.
***/
StereoPresenter::~StereoPresenter()
{
	SAFE_RELEASE(m_pcVertexShader10);
	SAFE_RELEASE(m_pcPixelShader10);
	SAFE_RELEASE(m_pcVertexLayout10);
	SAFE_RELEASE(m_pcVertexBuffer10);
	SAFE_RELEASE(m_pcBackBufferView);
	SAFE_RELEASE(m_pcConstantBufferDirect10);
}

/**
* Return the name of the  Stereo Presenter node.
***/
const char* StereoPresenter::GetNodeType()
{
	return "Stereo Presenter";
}

/**
* Returns a global unique identifier for the Stereo Presenter node.
***/
UINT StereoPresenter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 70
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Stereo Presenter node.
***/
LPWSTR StereoPresenter::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Stereo Presenter node.
***/
HBITMAP StereoPresenter::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioStereoPresenter.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Stereo Presenter node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP StereoPresenter::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR StereoPresenter::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			return L"Left Texture DX11";
		case STP_Decommanders::RightTexture11:
			return L"Right Texture DX11";
		case STP_Decommanders::LeftTexture10:
			return L"Left Texture DX10";
		case STP_Decommanders::RightTexture10:
			return L"Right Texture DX10";
		case STP_Decommanders::LeftTexture9:
			return L"Left Texture DX9";
		case STP_Decommanders::RightTexture9:
			return L"Right Texture DX9";
		case STP_Decommanders::ViewAdjustments:
			return L"ViewAdjustments";
		case STP_Decommanders::Yaw:
			return L"Yaw";
		case STP_Decommanders::Pitch:
			return L"Pitch";
		case STP_Decommanders::Roll:
			return L"Roll";
		case STP_Decommanders::XPosition:
			return L"X-Position";
		case STP_Decommanders::YPosition:
			return L"Y-Position";
		case STP_Decommanders::ZPosition:
			return L"Z-Position";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoPresenter::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
		case STP_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case STP_Decommanders::LeftTexture10:
		case STP_Decommanders::RightTexture10:
			return NOD_Plugtype::AQU_PNT_ID3D10SHADERRESOURCEVIEW;
		case STP_Decommanders::LeftTexture9:
		case STP_Decommanders::RightTexture9:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DTEXTURE9;
		case STP_Decommanders::ViewAdjustments:
			return NOD_Plugtype::AQU_SHAREDPOINTER;
		case STP_Decommanders::Yaw:
		case STP_Decommanders::Pitch:
		case STP_Decommanders::Roll:
		case STP_Decommanders::XPosition:
		case STP_Decommanders::YPosition:
		case STP_Decommanders::ZPosition:
			return NOD_Plugtype::AQU_PNT_FLOAT;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoPresenter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case STP_Decommanders::RightTexture11:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case STP_Decommanders::LeftTexture10:
		case STP_Decommanders::RightTexture10:
		case STP_Decommanders::LeftTexture9:
		case STP_Decommanders::RightTexture9:
			break;
		case STP_Decommanders::ViewAdjustments:
			m_ppcShaderViewAdjustment = (std::shared_ptr<ViewAdjustment>*)pData;
			break;
		case STP_Decommanders::Yaw:
			m_pfEuler[0] = (float*)pData;
			break;
		case STP_Decommanders::Pitch:
			m_pfEuler[1] = (float*)pData;
			break;
		case STP_Decommanders::Roll:
			m_pfEuler[2] = (float*)pData;
			break;
		case STP_Decommanders::XPosition:
			m_pfPosition[0] = (float*)pData;
			break;
		case STP_Decommanders::YPosition:
			m_pfPosition[1] = (float*)pData;
			break;
		case STP_Decommanders::ZPosition:
			m_pfPosition[2] = (float*)pData;
			break;
	}
}

/**
* Stereo Presenter supports any call since it is on the end of the line.
* (verifies for supported methods in provoke call)
***/
bool StereoPresenter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	//if ((nD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (nD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
	return true;

	//return false;
}

/**
* Handle Stereo Drawing.
***/
void* StereoPresenter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;

	if (m_ppcShaderViewAdjustment)
	{
		if (*m_ppcShaderViewAdjustment)
		{
			// update roll, update view transforms
			if (m_pfEuler[2])
				(*m_ppcShaderViewAdjustment).get()->UpdateRoll(-(*m_pfEuler[2]));
			(*m_ppcShaderViewAdjustment).get()->ComputeViewTransforms();
		}
	}

	if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
	{
#pragma region draw (optionally)
		// toggle stereo mode
		if (GetAsyncKeyState(VK_F12))
		{
			m_bHotkeySwitch = true;
		}
		else
		if (m_bHotkeySwitch)
		{
			if (m_eStereoMode) m_eStereoMode = VireioMonitorStereoModes::Vireio_Mono; else m_eStereoMode = VireioMonitorStereoModes::Vireio_SideBySide;
			m_bHotkeySwitch = false;
		}

		// draw stereo target to screen (optionally)
		if (m_eStereoMode)
		{
			// DX 11
			if ((m_ppcTexView11[0]) && (m_ppcTexView11[1]))
			{
				// get device and context
				ID3D11Device* pcDevice = nullptr;
				ID3D11DeviceContext* pcContext = nullptr;
				if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
				{
					// release frame texture+view
					if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
					if (pcContext) { pcContext->Release(); pcContext = nullptr; }
					return nullptr;
				}

				// get the viewport
				UINT dwNumViewports = 1;
				D3D11_VIEWPORT psViewport[16];
				pcContext->RSGetViewports(&dwNumViewports, psViewport);

				// backup all states
				D3DX11_STATE_BLOCK sStateBlock;
				CreateStateblock(pcContext, &sStateBlock);

				// clear all states, set targets
				ClearContextState(pcContext);

				// set first active render target - the stored back buffer - get the stored private data view
				ID3D11Texture2D* pcBackBuffer = nullptr;
				((IDXGISwapChain*)pThis)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
				ID3D11RenderTargetView* pcView = nullptr;
				UINT dwSize = sizeof(pcView);
				pcBackBuffer->GetPrivateData(PDIID_ID3D11TextureXD_RenderTargetView, &dwSize, (void*)&pcView);
				if (dwSize)
				{
					pcContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&pcView, NULL);
					pcView->Release();
				}
				pcContext->RSSetViewports(dwNumViewports, psViewport);
				pcBackBuffer->Release();

				// create all bool
				bool bAllCreated = true;

				// create vertex shader
				if (!m_pcVertexShader11)
				{
					if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
						bAllCreated = false;
				}
				// create pixel shader... TODO !! add option to switch output
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

				if (bAllCreated)
				{
					// left/right eye
					for (int nEye = 0; nEye < 2; nEye++)
					{
						// Set the input layout
						pcContext->IASetInputLayout(m_pcVertexLayout11);

						// Set vertex buffer
						UINT stride = sizeof(TexturedVertex);
						UINT offset = 0;
						pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

						// Set constant buffer, first update it... scale and translate the left and right image
						D3DXMATRIX sScale;
						D3DXMatrixScaling(&sScale, 0.5f, 1.0f, 1.0f);
						D3DXMATRIX sTrans;
						if (nEye == 0)
							D3DXMatrixTranslation(&sTrans, -0.5f, 0.0f, 0.0f);
						else
							D3DXMatrixTranslation(&sTrans, 0.5f, 0.0f, 0.0f);
						D3DXMatrixTranspose(&sTrans, &sTrans);
						D3DXMATRIX sProj;
						D3DXMatrixMultiply(&sProj, &sTrans, &sScale);
						pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
						pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

						// Set primitive topology
						pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						// set texture
						pcContext->PSSetShaderResources(0, 1, m_ppcTexView11[nEye]);

						// set shaders
						pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
						pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

						// Render a triangle
						pcContext->Draw(6, 0);
					}
				}

				// set back device
				ApplyStateblock(pcContext, &sStateBlock);

				if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
				if (pcContext) { pcContext->Release(); pcContext = nullptr; }
			}
		}
#pragma endregion
	}

	return nullptr;
}