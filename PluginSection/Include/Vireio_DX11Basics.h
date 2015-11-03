/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_DX11Basics.h> :
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

#ifndef __d3d11_1_h__
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")
#endif

#ifndef __d3d11_h__
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#endif

#include"Vireio_DX11StateBlock.h"

/**
* Simple texture vertex structure.
***/
struct TexturedVertex
{
	D3DXVECTOR4 sPos;
	D3DXVECTOR2 sTex;
};

/**
* 2D Vertex Shader DX10+.
***/
static const char* VS2D =
	"float4x4 ProjView;\n"
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
	//"    Out.Position = vtx.Position;\n"
	"    Out.Position = mul( vtx.Position, ProjView );\n"
	"    Out.TexCoord = vtx.TexCoord;\n"
	"    return Out;\n"
	"}\n";

/**
* 2D Pixel Shader DX10+.
***/
static const char* PS2D =
	"Texture2D fontTexture : register(t0);\n"
	"SamplerState fontSampler : register(s0);\n"

	"struct VS_OUT\n"
	"{\n"
	"float4 Position  : SV_POSITION;\n"
	"float2 TexCoord : TEXCOORD0;\n"
	"};\n"

	"float4 PS( VS_OUT vtx ) : SV_Target\n"
	"{\n"
	//"    return fontTexture.Sample( fontSampler, vtx.TexCoord );\n"
	"    return float4(fontTexture.Sample( fontSampler, vtx.TexCoord ).xyz, 1.0);\n"
	//"    return float4(1.0, 0.4, 0.3, 1.0);\n"
	"}\n";

/**
* Sets the first render target by swap chain.
* @pararm ppcBackBufferView [in, out] Returns the back buffer view taken from the provided swapchain.
***/
void SetFirstRenderTargetBySwapChain(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapChain, ID3D11RenderTargetView** ppcBackBufferView, ID3D11DepthStencilView* pcDepthStencilView)
{
	if (!ppcBackBufferView) return;

	ID3D11Texture2D* pcBackBuffer = NULL;
	pcSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
	if (pcBackBuffer)
	{
		pcDevice->CreateRenderTargetView(pcBackBuffer, NULL, ppcBackBufferView);
		if (!*ppcBackBufferView)
		{
			pcContext->OMSetRenderTargets(1, ppcBackBufferView, pcDepthStencilView);
		}
		pcBackBuffer->Release();
	}
}

/**
* Creates a simple 2D vertex shader with an input layout.
***/
HRESULT Create2DVertexShader(ID3D11Device* pcDevice, ID3D11VertexShader** ppcVertexShader, ID3D11InputLayout** ppcInputLayout)
{
	if ((!ppcVertexShader) || (!ppcInputLayout) || (!pcDevice)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;

	// compile and create shader
	if (SUCCEEDED(hr = D3DX10CompileFromMemory(VS2D, strlen(VS2D), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL)))
	{
#ifdef _DEBUG
		OutputDebugString(L"HelloWorldDx10 Node : Vertex Shader compiled !");
#endif
		hr = pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, ppcVertexShader);

		if (FAILED(hr))
			return hr;
		else
		{
			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = sizeof(layout) / sizeof(layout[0]);

			hr = pcDevice->CreateInputLayout(layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), ppcInputLayout);
			if (FAILED(hr))
			{
				if (*ppcVertexShader)
				{
					(*ppcVertexShader)->Release(); (*ppcVertexShader) = nullptr;
				}
				return hr;
			}
		}
		pcShader->Release();
	}
	else return hr;

	return S_OK;
}

/**
* Creates a simple pixel shader
***/
HRESULT CreateSimplePixelShader(ID3D11Device* pcDevice, ID3D11PixelShader** ppcPixelShader)
{
	if ((!pcDevice) || (!ppcPixelShader)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;

	// compile and create shader
	if (SUCCEEDED(hr = D3DX10CompileFromMemory(PS2D, strlen(PS2D), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL)))
	{
		OutputDebugString(L"HelloWorldDx10 Node : Pixel Shader compiled !");
		pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, ppcPixelShader);
		pcShader->Release();

		return S_OK;
	}
	else return hr;
}

/**
* Creates a full screen vertex buffer.
***/
HRESULT CreateFullScreenVertexBuffer(ID3D11Device* pcDevice, ID3D11Buffer** ppcBuffer)
{
	// Create vertex buffer
	TexturedVertex vertices[] =
	{
		{ D3DXVECTOR4(-1.0f, 1.0f, 0.5f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) }, // top left
		{ D3DXVECTOR4(1.0f, 1.0f, 0.5f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) }, // top right
		{ D3DXVECTOR4(-1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) }, // bottom left

		{ D3DXVECTOR4(-1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) }, // bottom left
		{ D3DXVECTOR4(1.0f, 1.0f, 0.5f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) }, // top right
		{ D3DXVECTOR4(1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) }, // bottom right
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TexturedVertex)* 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	// create both, direct mode and mirror buffer
	return pcDevice->CreateBuffer(&bd, &InitData, ppcBuffer);
}

/**
* Creates a simple matrix constant buffer.
***/
HRESULT CreateMatrixConstantBuffer(ID3D11Device* pcDevice, ID3D11Buffer** ppcBuffer)
{
	// init data
	D3DXMATRIX sWorldViewProj;
	D3DXMatrixIdentity(&sWorldViewProj);

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof( D3DMATRIX );
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &sWorldViewProj;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	return pcDevice->CreateBuffer( &cbDesc, &InitData, ppcBuffer);
}

/**
* Get device and context from the swapchain.
***/
HRESULT GetDeviceAndContext(IDXGISwapChain* pcSwapChain, ID3D11Device** ppcDevice, ID3D11DeviceContext** ppcContext)
{
	if ((!pcSwapChain) || (!ppcDevice) || (!ppcContext)) return E_INVALIDARG;

	HRESULT hr;

	// get device
	hr = pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)ppcDevice);
	if (!*ppcDevice)
	{
		OutputDebugString(L"Vireio Node : No d3d 11 device !");
		return hr;
	}
	// get context
	(*ppcDevice)->GetImmediateContext(ppcContext);
	if (!*ppcContext)
	{
		OutputDebugString(L"HelloWorld Node : No device context !");
		return E_FAIL;
	}

	return hr;
}

/**
* Get the current render target, depth stencil and the first viewport.
***/
void GetRenderTarget(ID3D11DeviceContext* pcContext, ID3D11RenderTargetView** ppcRenderTargetView, ID3D11DepthStencilView** ppcDepthStencilView, UINT* pdwNumViewports, D3D11_VIEWPORT* psViewport)
{
	pcContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppcRenderTargetView, ppcDepthStencilView);
	pcContext->RSGetViewports(pdwNumViewports, psViewport);
}




