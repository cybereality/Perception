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
#include"NOD_HelloWorldDx10.h"
#include"AQU_GlobalTypes.h"
#include"Resources.h"

/**
* Simple texture vertex structure.
***/
struct TexturedVertex
{
	D3DXVECTOR4 sPos;
	D3DXVECTOR2 sTex;
};

/**
* Font Vertex Shader DX10.
***/
static const char* VS2DFont =
	"struct VS_IN\n"  
	"{\n"  
	"float4 Position  : POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"struct VS_OUT\n"  
	"{\n"  
	"float4 Position  : SV_POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"VS_OUT VS( VS_IN vtx )\n"
	"{\n"
	"    VS_OUT Out = (VS_OUT)0;\n"
	"    Out.Position = vtx.Position;\n"
	"    Out.TexCoord = vtx.TexCoord;\n"
	"    return Out;\n"
	"}\n";

/**
* Font Pixel Shader DX10.
***/
static const char* PS2DFont = 
	"Texture2D fontTexture : register(t0);\n"
	"SamplerState fontSampler : register(s0);\n"

	"struct VS_OUT\n"  
	"{\n"  
	"float4 Position  : SV_POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"float4 PS( VS_OUT vtx ) : SV_Target\n"
	"{\n"
	"    return fontTexture.Sample( fontSampler, vtx.TexCoord );\n"
	//"    return float4(0.59, 0.4, 0.3, 0.5);\n"
	"}\n";

/**
* Constructor.
* @param nX X Position of the node (in full zoom pixel space).
* @param nY Y Position of the node (in full zoom pixel space).
* @param nWidth Width of the node (in full zoom pixel space).
* @param nHeight Height of the node (in full zoom pixel space).
***/
NOD_HelloWorldDx10::NOD_HelloWorldDx10(LONG nX, LONG nY):NOD_Basic(nX, nY, 140, 100),
	m_pcVertexShader(nullptr),
	m_pcPixelShader(nullptr),
	m_pcVertexLayout(nullptr),
	m_pcVertexBuffer(nullptr),
	m_pcTexture(nullptr),
	m_pcBackBufferView(nullptr),
	m_pcTextureView(nullptr),
	m_pcStateBlock(nullptr),
	m_bUnderline(false),
	m_bInit(false),
	m_eDeviceVersion(NOD_HelloWorldDx10::D3DDeviceVersion::D3D10_Version)
{ 
	m_eNodeProvokingType = AQU_NodeProvokingType::Both;
	m_bReturn = false;

	ZeroMemory(&m_sFontTextureSize, sizeof(SIZE));
	ZeroMemory(&m_sStateBlock11, sizeof(D3DX11_STATE_BLOCK));

	m_paDecommanders.clear();
	m_szTitle = L"Hello World Dx10"; 

	NOD_Decommander * pD = new NOD_Decommander();
	pD->m_ePlugtype = NOD_Plugtype::AQU_BOOL;
	pD->m_szTitle = L"bUnderline";
	pD->m_pInput = nullptr;
	pD->m_paCommanders.clear();
	pD->m_lDecommanderIndex = 0;
	m_paDecommanders.push_back(pD);
}

/**
* Destructor.
***/
NOD_HelloWorldDx10::~NOD_HelloWorldDx10()
{
	if (m_pcVertexShader) m_pcVertexShader->Release();
	if (m_pcPixelShader) m_pcPixelShader->Release();
	if (m_pcVertexLayout) m_pcVertexLayout->Release();
	if (m_pcVertexBuffer) m_pcVertexBuffer->Release();
	if (m_pcStateBlock) m_pcStateBlock->Release();
	if (m_pcTexture) m_pcTexture->Release();
	if (m_pcTextureView) m_pcTextureView->Release();
	if (m_pcBackBufferView) m_pcBackBufferView->Release();

	if (m_pcVertexShader11) m_pcVertexShader11->Release();
	if (m_pcPixelShader11) m_pcPixelShader11->Release();
}

/**
* This node supports IDXGISwapChain->Present().
***/
bool NOD_HelloWorldDx10::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	return true;
}

/**
* Node Provoke - Draw Text.
***/
void* NOD_HelloWorldDx10::Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
{
	//if (GetAsyncKeyState(VK_LSHIFT)) m_bInit = true;
	//if (!m_bInit) return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, paNodes);

	// get node data... cast bool using int to avoid warning C4800
	if (m_paDecommanders[0]->m_pInput != nullptr)
		m_bUnderline = (((int*)*(int**)m_paDecommanders[0]->m_pInput)!=0);
	else m_bUnderline = false;

	// cast swapchain
	IDXGISwapChain* pcSwapChain = (IDXGISwapChain*)pcThis;
	if (!pcSwapChain) 
	{
		OutputDebugString(L"HelloWorld Node : No swapchain !");
		return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
	}

	// directx 10 or 11 ?
	switch (m_eDeviceVersion)
	{
#pragma region d3d10
	case NOD_HelloWorldDx10::D3DDeviceVersion::D3D10_Version:
		{
			// get device
			ID3D10Device* pcDevice = nullptr;
			pcSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&pcDevice);
			if (!pcDevice)
			{
				OutputDebugString(L"HelloWorld Node : No device !");
				m_eDeviceVersion = NOD_HelloWorldDx10::D3DDeviceVersion::D3D11_Version;
				return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
			}

			// get the render target
			ID3D10RenderTargetView* pRenderTargetView = nullptr;
			ID3D10DepthStencilView* pDepthStencilView = nullptr;
			pcDevice->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);
			if (!pRenderTargetView)
			{
				OutputDebugString(L"HelloWorld Node : No render target !");
				if (pDepthStencilView) pDepthStencilView->Release();
				pcDevice->Release();
				return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
			}
			UINT dwNumViewports = 1;
			D3D10_VIEWPORT sViewport[16];
			pcDevice->RSGetViewports(&dwNumViewports, sViewport);

			// backup all states
			D3D10_STATE_BLOCK_MASK sMask;
			D3D10StateBlockMaskEnableAll(&sMask);
			if (!m_pcStateBlock) 
				D3D10CreateStateBlock(pcDevice, &sMask, &m_pcStateBlock);
			if (!m_pcStateBlock)
			{
				if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
				if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }
				pcDevice->Release();
				OutputDebugString(L"HelloWorld Node : Failed to create state block !");
				return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
			}
			m_pcStateBlock->Capture();

			// clear all states, set targets
			pcDevice->ClearState();
			pcDevice->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
			pcDevice->RSSetViewports(dwNumViewports, sViewport);

			// create all
			bool bAllCreated = true;

			// create vertex shader
			if (!m_pcVertexShader)
			{
				ID3D10Blob* pcShader;

				// compile and create shader
				if (SUCCEEDED(D3DX10CompileFromMemory(VS2DFont,strlen(VS2DFont),NULL,NULL,NULL,"VS","vs_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
				{
					OutputDebugString(L"HelloWorldDx10 Node : Vertex Shader compiled !");
					pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcVertexShader);

					if (!m_pcVertexShader) 
						bAllCreated = false;
					else
					{
						// Define the input layout
						D3D10_INPUT_ELEMENT_DESC layout[] =
						{
							{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
							{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
						};
						UINT numElements = sizeof( layout ) / sizeof( layout[0] );

						pcDevice->CreateInputLayout( layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcVertexLayout );
						if (!m_pcVertexLayout) bAllCreated = false;
					}
					pcShader->Release();
				}
				else bAllCreated = false;
			}

			// create pixel shader
			if (!m_pcPixelShader)
			{
				ID3D10Blob* pcShader;

				// compile and create shader
				if (SUCCEEDED(D3DX10CompileFromMemory(PS2DFont,strlen(PS2DFont),NULL,NULL,NULL,"PS","ps_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
				{
					OutputDebugString(L"HelloWorldDx10 Node : Pixel Shader compiled !");
					pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcPixelShader);
					pcShader->Release();
				}
				if (!m_pcPixelShader) bAllCreated = false;
			}

			// Create vertex buffer
			if (!m_pcVertexBuffer)
			{
				// Create vertex buffer
				TexturedVertex vertices[] =
				{
					{ D3DXVECTOR4( 0.0f, 1.0f, 0.5f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
					{ D3DXVECTOR4(  1.0f, -1.0f, 0.5f, 1.0f  ), D3DXVECTOR2( 1.0f, 0.0f ) },
					{ D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },
				};
				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = sizeof( TexturedVertex ) * 3;
				bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D10_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = vertices;
				pcDevice->CreateBuffer( &bd, &InitData, &m_pcVertexBuffer );
				if (!m_pcVertexBuffer) bAllCreated = false;
			}

			// and render, pass first frame by creating tex
			if (false)//!m_pcTexture)
			{
#ifdef _WIN64
				HMODULE hModule = GetModuleHandle(L"Aquilinus_x64.dll");
#else
				HMODULE hModule = GetModuleHandle(L"Aquilinus_Win32.dll");
#endif
				HBITMAP hbFont = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_FONT02));
				if(hbFont)
				{
					// get size of bitmap
					BITMAP bm;
					GetObject(hbFont, sizeof(bm), &bm);

					// set font texture size
					m_sFontTextureSize.cx = bm.bmWidth;
					m_sFontTextureSize.cy = bm.bmHeight;

					// create byte buffer
					char *pchBytes = (char*)malloc(bm.bmWidth*bm.bmHeight*4);

					// set font rectangle
					SetRect(&m_rcFont, 0, 0, bm.bmWidth, bm.bmHeight);

					// get source hdc
					HDC hdcImage = CreateCompatibleDC(NULL);
					HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, hbFont);
					int x = 0; int y = 0;
					FillMemory(&pchBytes, bm.bmWidth*bm.bmHeight*4, 0x1f);
					//for (DWORD dwAddr = 0; dwAddr < (DWORD)(bm.bmWidth*bm.bmHeight*4); dwAddr += 4)
					//{
					//	COLORREF dwColor = GetPixel(hdcImage, x, y);
					//	pchBytes[dwAddr] = BYTE(dwColor & 0xff);
					//	pchBytes[dwAddr+1] = BYTE(dwColor & 0xff00 >> 8);
					//	pchBytes[dwAddr+2] = BYTE(dwColor & 0xff0000 >> 16);
					//	pchBytes[dwAddr+3] = BYTE(dwColor & 0xff000000 >> 24);
					//	x++; if (x >= bm.bmWidth) { x = 0; y++; }
					//}

					// release bitmap
					DeleteDC(hdcImage);
					DeleteObject(hbFont);

					// create font texture
					D3D10_TEXTURE2D_DESC sDesc;
					ZeroMemory(&sDesc, sizeof(sDesc));
					sDesc.Width = bm.bmWidth;
					sDesc.Height = bm.bmHeight;
					sDesc.MipLevels = sDesc.ArraySize = 1;
					sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					sDesc.SampleDesc.Count = 1;
					sDesc.Usage = D3D10_USAGE_DEFAULT;
					sDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
					D3D10_SUBRESOURCE_DATA sData;
					ZeroMemory(&sData, sizeof(sData));
					sData.pSysMem = pchBytes;
					sData.SysMemPitch = bm.bmWidth * 4;
					if (FAILED(pcDevice->CreateTexture2D( &sDesc, &sData, &m_pcTexture )))
						OutputDebugString(L"Failed to create Texture DEFAULT.");

					free((void*)pchBytes);

					if (m_pcTexture)
					{
						OutputDebugString(L"HelloWorld Node DX10 : Created font texture !");
					}
					else
					{
						OutputDebugString(L"Aquilinus : Can't create font texture.");
					}

					DeleteObject(hbFont);
				}

				if (m_pcTexture)
				{
					D3D10_SHADER_RESOURCE_VIEW_DESC sDesc;
					ZeroMemory(&sDesc, sizeof(sDesc));
					sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					sDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
					sDesc.Texture2D.MostDetailedMip = 0;
					sDesc.Texture2D.MipLevels = 1;

					if ((FAILED(pcDevice->CreateShaderResourceView((ID3D10Resource*)m_pcTexture, &sDesc, &m_pcTextureView))))
						OutputDebugString(L"Failed to create texture view!");
				}
			}
			else if (bAllCreated)
			{
				// Set the input layout
				pcDevice->IASetInputLayout( m_pcVertexLayout );

				// Set vertex buffer
				UINT stride = sizeof( TexturedVertex );
				UINT offset = 0;
				pcDevice->IASetVertexBuffers( 0, 1, &m_pcVertexBuffer, &stride, &offset );

				// Set primitive topology
				pcDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

				// set texture
				//pcDevice->PSSetShaderResources(0, 1, &m_pcTextureView);

				// Clear ?
				float ClearColor[4] = { 0.6f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
				pcDevice->ClearRenderTargetView( pRenderTargetView, ClearColor );
				pcDevice->ClearDepthStencilView( pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0, 0 );

				// set shaders
				pcDevice->VSSetShader(m_pcVertexShader);
				pcDevice->PSSetShader(m_pcPixelShader);

				// Render a triangle
				pcDevice->Draw( 3, 0 );
			}

			// finish up
			if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
			if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }

			// set back device
			m_pcStateBlock->Apply();

			// release d3d10 device
			pcDevice->Release();
		}
		break;
#pragma endregion
#pragma region d3d11
	case NOD_HelloWorldDx10::D3DDeviceVersion::D3D11_Version:
		{
			// get device
			ID3D11Device* pcDevice = nullptr;
			pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
			if (!pcDevice)
			{
				OutputDebugString(L"HelloWorld Node : No d3d 11 device !");
				m_eDeviceVersion = NOD_HelloWorldDx10::D3DDeviceVersion::D3D10_Version;
				return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
			}
			// get context
			ID3D11DeviceContext* pcContext = nullptr;
			pcDevice->GetImmediateContext(&pcContext);
			if (!pcContext)
			{
				OutputDebugString(L"HelloWorld Node : No device context !");
				return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
			}

			// get the render target
			ID3D11RenderTargetView* ppRenderTargetView[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView* pDepthStencilView = nullptr;
			pcContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetView, &pDepthStencilView);
			UINT dwNumViewports = 1;
			D3D11_VIEWPORT sViewport[16];
			pcContext->RSGetViewports(&dwNumViewports, sViewport);

			// backup all states
			CreateStateblock(pcContext, &m_sStateBlock11);

			// clear all states, set targets
			pcContext->ClearState();

			// no render target present ? get from swapchain
			if (!ppRenderTargetView[0])
			{
				if (!m_pcBackBufferView)
				{
					OutputDebugString(L"No primary render target set !");
					ID3D11Texture2D* pcBackBuffer = NULL;
					pcSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pcBackBuffer );
					if (pcBackBuffer)
					{
						OutputDebugString(L"Got backbuffer!");
						pcDevice->CreateRenderTargetView( pcBackBuffer, NULL, &m_pcBackBufferView );
						if (m_pcBackBufferView)
						{
							OutputDebugString(L"Created render target view!");
							pcContext->OMSetRenderTargets(1, &m_pcBackBufferView, pDepthStencilView);
						}
						pcBackBuffer->Release();
					}
				}
				else
					pcContext->OMSetRenderTargets(1, &m_pcBackBufferView, pDepthStencilView);
			}
			else
				pcContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetView, pDepthStencilView);
			pcContext->RSSetViewports(dwNumViewports, sViewport);

			// create all
			bool bAllCreated = true;

			// create vertex shader
			if (!m_pcVertexShader)
			{
				ID3D10Blob* pcShader;

				// compile and create shader
				if (SUCCEEDED(D3DX10CompileFromMemory(VS2DFont,strlen(VS2DFont),NULL,NULL,NULL,"VS","vs_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
				{
					OutputDebugString(L"HelloWorldDx10 Node : Vertex Shader compiled !");
					pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcVertexShader11);

					if (!m_pcVertexShader) 
						bAllCreated = false;
					else
					{
						// Define the input layout
						D3D11_INPUT_ELEMENT_DESC layout[] =
						{
							{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
							{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						};
						UINT numElements = sizeof( layout ) / sizeof( layout[0] );

						pcDevice->CreateInputLayout( layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcVertexLayout11 );
						if (!m_pcVertexLayout11) bAllCreated = false;
					}
					pcShader->Release();
				}
				else bAllCreated = false;
			}

			// create pixel shader
			if (!m_pcPixelShader)
			{
				ID3D10Blob* pcShader;

				// compile and create shader
				if (SUCCEEDED(D3DX10CompileFromMemory(PS2DFont,strlen(PS2DFont),NULL,NULL,NULL,"PS","ps_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
				{
					OutputDebugString(L"HelloWorldDx10 Node : Pixel Shader compiled !");
					pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcPixelShader11);
					pcShader->Release();
				}
				if (!m_pcPixelShader) bAllCreated = false;
			}

			// Create vertex buffer
			if (!m_pcVertexBuffer)
			{
				// Create vertex buffer
				TexturedVertex vertices[] =
				{
					{ D3DXVECTOR4( -0.3f, 0.15f, 0.5f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
					{ D3DXVECTOR4(  0.3f, 0.14f, 0.5f, 1.0f  ), D3DXVECTOR2( 1.0f, 0.0f ) },
					{ D3DXVECTOR4( -0.01f, -0.3f, 0.5f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },

					{ D3DXVECTOR4( 0.01f, 0.3f, 0.5f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
					{ D3DXVECTOR4(  0.3f, -0.14f, 0.5f, 1.0f  ), D3DXVECTOR2( 1.0f, 0.0f ) },
					{ D3DXVECTOR4( -0.3f, -0.15f, 0.5f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },
				};
				D3D11_BUFFER_DESC bd;
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof( TexturedVertex ) * 6;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = vertices;
				pcDevice->CreateBuffer( &bd, &InitData, &m_pcVertexBuffer11 );
				if (!m_pcVertexBuffer11) bAllCreated = false;
			}

			// and render, pass first frame by creating tex
			if (!m_pcTexture11)
			{
#ifdef _WIN64
				HMODULE hModule = GetModuleHandle(L"Aquilinus_x64.dll");
#else
				HMODULE hModule = GetModuleHandle(L"Aquilinus_Win32.dll");
#endif
				HBITMAP hbFont = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_FONT02));
				if(hbFont)
				{
					// get size of bitmap
					BITMAP bm;
					GetObject(hbFont, sizeof(bm), &bm);

					// set font texture size
					m_sFontTextureSize.cx = bm.bmWidth;
					m_sFontTextureSize.cy = bm.bmHeight;

					// create byte buffer
					char *pchBytes = (char*)malloc(bm.bmWidth*bm.bmHeight*4);

					// set font rectangle
					SetRect(&m_rcFont, 0, 0, bm.bmWidth, bm.bmHeight);

					// get source hdc
					HDC hdcImage = CreateCompatibleDC(NULL);
					HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, hbFont);
					int x = 0; int y = 0;

					BITMAPINFO MyBMInfo = {0};
					MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

					// Get the BITMAPINFO structure from the bitmap
					if(0 == GetDIBits(hdcImage, hbFont, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS))
					{
						OutputDebugString(L"HelloWorldDx10 : Error GetDIBits");
					}

					MyBMInfo.bmiHeader.biBitCount = 32;
					MyBMInfo.bmiHeader.biCompression = BI_RGB;
					MyBMInfo.bmiHeader.biHeight = abs(MyBMInfo.bmiHeader.biHeight);

					// Call GetDIBits a second time, this time to store the actual bitmap data
					if(0 == GetDIBits(hdcImage, hbFont, 0, MyBMInfo.bmiHeader.biHeight,
						pchBytes, &MyBMInfo, DIB_RGB_COLORS))
					{
						OutputDebugString(L"HelloWorldDx10 : Error GetDIBits");
					}

					// release bitmap
					DeleteDC(hdcImage);
					DeleteObject(hbFont);

					// create font texture
					D3D11_TEXTURE2D_DESC sDesc;
					ZeroMemory(&sDesc, sizeof(sDesc));
					sDesc.Width = bm.bmWidth;
					sDesc.Height = bm.bmHeight;
					sDesc.MipLevels = sDesc.ArraySize = 1;
					sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					sDesc.SampleDesc.Count = 1;
					sDesc.Usage = D3D11_USAGE_DEFAULT;
					sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					D3D11_SUBRESOURCE_DATA sData;
					ZeroMemory(&sData, sizeof(sData));
					sData.pSysMem = pchBytes;
					sData.SysMemPitch = bm.bmWidth * 4;
					if (FAILED(pcDevice->CreateTexture2D( &sDesc, &sData, &m_pcTexture11 )))
						OutputDebugString(L"Failed to create Texture DEFAULT.");

					free((void*)pchBytes);

					if (m_pcTexture)
					{
						OutputDebugString(L"HelloWorld Node DX10 : Created font texture !");
					}
					else
					{
						OutputDebugString(L"Aquilinus : Can't create font texture.");
					}

					DeleteObject(hbFont);
				}

				if (m_pcTexture)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
					ZeroMemory(&sDesc, sizeof(sDesc));
					sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					sDesc.Texture2D.MostDetailedMip = 0;
					sDesc.Texture2D.MipLevels = 1;

					if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTexture11, &sDesc, &m_pcTextureView11))))
						OutputDebugString(L"Failed to create texture view!");
				}
			}
			else if (bAllCreated)
			{
				// Set the input layout
				pcContext->IASetInputLayout( m_pcVertexLayout11 );

				// Set vertex buffer
				UINT stride = sizeof( TexturedVertex );
				UINT offset = 0;
				pcContext->IASetVertexBuffers( 0, 1, &m_pcVertexBuffer11, &stride, &offset );

				// Set primitive topology
				pcContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

				// set texture
				pcContext->PSSetShaderResources(0, 1, &m_pcTextureView11);

				// Clear ?
				if (false)
				{
					float ClearColor[4] = { 0.6f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
					for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
					{
						if (ppRenderTargetView[i])
							pcContext->ClearRenderTargetView( ppRenderTargetView[i], ClearColor );
					}
				}
				pcContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0 );

				// set shaders
				pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
				pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

				// Render a triangle
				pcContext->Draw( 6, 0 );
			}

			// finish up
			for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			{
				if (ppRenderTargetView[i]) { ppRenderTargetView[i]->Release(); ppRenderTargetView[i] = nullptr; }
			}
			if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }

			// set back device
			ApplyStateblock(pcContext, &m_sStateBlock11);

			// release d3d11 device + context
			pcContext->Release();
			pcDevice->Release();
		}
		break;
#pragma endregion
	}

	return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
}

/**
* Create dx11 stateblocks.
***/
void NOD_HelloWorldDx10::CreateStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	memset(sb, 0, sizeof(D3DX11_STATE_BLOCK));

	dc->VSGetShader(&sb->VS, sb->VSInterfaces, &sb->VSInterfaceCount);
	dc->VSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->VSSamplers);
	dc->VSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->VSShaderResources);
	dc->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->VSConstantBuffers);

	dc->GSGetShader(&sb->GS, sb->GSInterfaces, &sb->GSInterfaceCount);
	dc->GSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->GSSamplers);
	dc->GSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->GSShaderResources);
	dc->GSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->GSConstantBuffers);

	dc->HSGetShader(&sb->HS, sb->HSInterfaces, &sb->HSInterfaceCount);
	dc->HSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->HSSamplers);
	dc->HSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->HSShaderResources);
	dc->HSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->HSConstantBuffers);

	dc->DSGetShader(&sb->DS, sb->DSInterfaces, &sb->DSInterfaceCount);
	dc->DSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->DSSamplers);
	dc->DSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->DSShaderResources);
	dc->DSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->DSConstantBuffers);

	dc->PSGetShader(&sb->PS, sb->PSInterfaces, &sb->PSInterfaceCount);
	dc->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->PSSamplers);
	dc->PSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->PSShaderResources);
	dc->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->PSConstantBuffers);

	dc->CSGetShader(&sb->CS, sb->CSInterfaces, &sb->CSInterfaceCount);
	dc->CSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->CSSamplers);
	dc->CSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->CSShaderResources);
	dc->CSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->CSConstantBuffers);
	dc->CSGetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews);

	dc->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	dc->IAGetIndexBuffer(&sb->IAIndexBuffer, &sb->IAIndexBufferFormat, &sb->IAIndexBufferOffset);
	dc->IAGetInputLayout(&sb->IAInputLayout);
	dc->IAGetPrimitiveTopology(&sb->IAPrimitiveTopology);

	dc->OMGetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, &sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews);
	dc->OMGetDepthStencilState(&sb->OMDepthStencilState, &sb->OMDepthStencilRef);
	dc->OMGetBlendState(&sb->OMBlendState, sb->OMBlendFactor, &sb->OMSampleMask);

	sb->RSViewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetViewports(&sb->RSViewportCount, sb->RSViewports);
	sb->RSScissorRectCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetScissorRects(&sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSGetState(&sb->RSRasterizerState);

	dc->SOGetTargets(4, sb->SOBuffers);
	dc->GetPredication(&sb->Predication, &sb->PredicationValue);
}

/**
* Apply dx11 stateblocks.
***/
void NOD_HelloWorldDx10::ApplyStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	UINT minus_one[D3D11_PS_CS_UAV_REGISTER_COUNT];
	memset(minus_one, -1, sizeof(minus_one));
	dc->VSSetShader(sb->VS, sb->VSInterfaces, sb->VSInterfaceCount);
	UINT VSSamplerCount = calc_count(sb->VSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (VSSamplerCount)
		dc->VSSetSamplers(0, VSSamplerCount, sb->VSSamplers);
	UINT VSShaderResourceCount = calc_count(sb->VSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (VSShaderResourceCount)
		dc->VSSetShaderResources(0, VSShaderResourceCount, sb->VSShaderResources);
	UINT VSConstantBufferCount = calc_count(sb->VSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (VSConstantBufferCount)
		dc->VSSetConstantBuffers(0, VSConstantBufferCount, sb->VSConstantBuffers);

	dc->GSSetShader(sb->GS, sb->GSInterfaces, sb->GSInterfaceCount);
	UINT GSSamplerCount = calc_count(sb->GSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (GSSamplerCount)
		dc->GSSetSamplers(0, GSSamplerCount, sb->GSSamplers);
	UINT GSShaderResourceCount = calc_count(sb->GSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (GSShaderResourceCount)
		dc->GSSetShaderResources(0, GSShaderResourceCount, sb->GSShaderResources);
	UINT GSConstantBufferCount = calc_count(sb->GSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (GSConstantBufferCount)
		dc->GSSetConstantBuffers(0, GSConstantBufferCount, sb->GSConstantBuffers);

	dc->HSSetShader(sb->HS, sb->HSInterfaces, sb->HSInterfaceCount);
	UINT HSSamplerCount = calc_count(sb->HSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (HSSamplerCount)
		dc->HSSetSamplers(0, HSSamplerCount, sb->HSSamplers);
	UINT HSShaderResourceCount = calc_count(sb->HSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (HSShaderResourceCount)
		dc->HSSetShaderResources(0, HSShaderResourceCount, sb->HSShaderResources);
	UINT HSConstantBufferCount = calc_count(sb->HSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (HSConstantBufferCount)
		dc->HSSetConstantBuffers(0, HSConstantBufferCount, sb->HSConstantBuffers);

	dc->DSSetShader(sb->DS, sb->DSInterfaces, sb->DSInterfaceCount);
	UINT DSSamplerCount = calc_count(sb->DSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (DSSamplerCount)
		dc->DSSetSamplers(0, DSSamplerCount, sb->DSSamplers);
	UINT DSShaderResourceCount = calc_count(sb->DSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (DSShaderResourceCount)
		dc->DSSetShaderResources(0, DSShaderResourceCount, sb->DSShaderResources);
	UINT DSConstantBufferCount = calc_count(sb->DSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (DSConstantBufferCount)
		dc->DSSetConstantBuffers(0, DSConstantBufferCount, sb->DSConstantBuffers);

	dc->PSSetShader(sb->PS, sb->PSInterfaces, sb->PSInterfaceCount);
	UINT PSSamplerCount = calc_count(sb->PSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (PSSamplerCount)
		dc->PSSetSamplers(0, PSSamplerCount, sb->PSSamplers);
	UINT PSShaderResourceCount = calc_count(sb->PSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (PSShaderResourceCount)
		dc->PSSetShaderResources(0, PSShaderResourceCount, sb->PSShaderResources);
	UINT PSConstantBufferCount = calc_count(sb->PSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (PSConstantBufferCount)
		dc->PSSetConstantBuffers(0, PSConstantBufferCount, sb->PSConstantBuffers);

	dc->CSSetShader(sb->CS, sb->CSInterfaces, sb->CSInterfaceCount);
	UINT CSSamplerCount = calc_count(sb->CSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (CSSamplerCount)
		dc->CSSetSamplers(0, CSSamplerCount, sb->CSSamplers);
	UINT CSShaderResourceCount = calc_count(sb->CSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (CSShaderResourceCount)
		dc->CSSetShaderResources(0, CSShaderResourceCount, sb->CSShaderResources);
	UINT CSConstantBufferCount = calc_count(sb->CSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (CSConstantBufferCount)
		dc->CSSetConstantBuffers(0, CSConstantBufferCount, sb->CSConstantBuffers);
	//dc->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews, minus_one);

	UINT IAVertexBufferCount = calc_count(sb->IAVertexBuffers, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
	if (IAVertexBufferCount)
		dc->IASetVertexBuffers(0, IAVertexBufferCount, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	dc->IASetIndexBuffer(sb->IAIndexBuffer, sb->IAIndexBufferFormat, sb->IAIndexBufferOffset);
	dc->IASetInputLayout(sb->IAInputLayout);
	dc->IASetPrimitiveTopology(sb->IAPrimitiveTopology);

	//dc->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews, minus_one);
	UINT OMRenderTargetCount = calc_count(sb->OMRenderTargets, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
	if (OMRenderTargetCount)
		dc->OMSetRenderTargets(OMRenderTargetCount, sb->OMRenderTargets, sb->OMRenderTargetStencilView);
	dc->OMSetDepthStencilState(sb->OMDepthStencilState, sb->OMDepthStencilRef);
	dc->OMSetBlendState(sb->OMBlendState, sb->OMBlendFactor, sb->OMSampleMask);

	dc->RSSetViewports(sb->RSViewportCount, sb->RSViewports);
	dc->RSSetScissorRects(sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSSetState(sb->RSRasterizerState);

	UINT SOBuffersOffsets[4] = { 0 }; /* (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 0,
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 1, 
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 2, 
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 3 };*/

	UINT SOBufferCount = calc_count(sb->SOBuffers, 4);
	if (SOBufferCount)
		dc->SOSetTargets(SOBufferCount, sb->SOBuffers, SOBuffersOffsets);
	dc->SetPredication(sb->Predication, sb->PredicationValue);
}