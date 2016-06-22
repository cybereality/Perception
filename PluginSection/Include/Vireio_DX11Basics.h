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
* Texture vertex structure.
***/
struct TexturedVertex
{
	D3DXVECTOR4 sPos;
	D3DXVECTOR2 sTex;
};

/**
* Diffuse textured vertex structure.
* Matches OpenVR render model vertex.
**/
struct TexturedDiffuseVertex
{
	D3DXVECTOR3 sPosition;
	D3DXVECTOR3 sNormal;
	D3DXVECTOR2 sTextureCoord;
};

/**
* Geometry constant buffer structure.
***/
struct GeometryConstantBuffer
{
	/*D3DXVECTOR4 m_sMaterialAmbientColor;
	D3DXVECTOR4 m_sMaterialDiffuseColor;

	D3DXVECTOR3 m_sLightDir;
	D3DXVECTOR4 m_sLightDiffuse;
	D3DXVECTOR4 m_sLightAmbient;*/

	D3DXMATRIX m_sWorldViewProjection;
	D3DXMATRIX m_sWorld;
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
* 3D Vertex Shader.
***/
static const char* VS3D =
// constant buffer
"cbuffer cbPerObject : register(b0)\n"
"{\n"
"	matrix		g_mWorldViewProjection : packoffset(c0);\n"
"	matrix		g_mWorld : packoffset(c4);\n"
"};\n"

// input / output structures
"struct VS_INPUT\n"
"{\n"
"	float3 vPosition : POSITION;\n"
"	float3 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float3 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

// vertex shader
"VS_OUTPUT VS(VS_INPUT Input)\n"
"{\n"
"	VS_OUTPUT Output;\n"

"	Output.vPosition = mul(Input.vPosition, g_mWorldViewProjection);\n"
"	Output.vNormal = mul(Input.vNormal, (float3x3)g_mWorld);\n"
"	Output.vTexcoord = Input.vTexcoord;\n"

"	return Output;\n"
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
"    return float4(fontTexture.Sample( fontSampler, vtx.TexCoord ).xyz, 1.0);\n"
"}\n";

/**
* 2D Pixel Shader DX10+, gamma correction.
***/
static const char* PS2D_GAMMA_CORRECTION =
"Texture2D fontTexture : register(t0);\n"
"SamplerState fontSampler : register(s0);\n"

"struct VS_OUT\n"
"{\n"
"float4 Position  : SV_POSITION;\n"
"float2 TexCoord : TEXCOORD0;\n"
"};\n"

"float4 PS( VS_OUT vtx ) : SV_Target\n"
"{\n"
"    float4 fColor = pow (float4(fontTexture.Sample( fontSampler, vtx.TexCoord ).xyz, 1.0), 2.2);\n"
"    return fColor;"
"}\n";

/**
* Simple Warp Shader.
* Warp method taken from old DIY-Rift shader code.
***/
static const char* PS_WARP_SIMPLE =
"Texture2D fontTexture : register(t0);\n"
"SamplerState fontSampler : register(s0);\n"

"struct VS_OUT\n"
"{\n"
"   float4 Position  : SV_POSITION;\n"
"   float2 TexCoord : TEXCOORD0;\n"
"};\n"

"float2 Warp(float2 Tex : TEXCOORD0)\n"
"{\n"
"float2 newPos = Tex;\n"
"	float c = -60.0f / 10.0f;            // Distortion coefficient of some sort\n"
"	float u = Tex.x*2.0f - 1.0f;         // Texture coordinates converted to -1.0 to 1.0 range\n"
"	float v = Tex.y*2.0f - 1.0f;\n"
"	newPos.x = c*u / (pow(v, 2) + c);    // Distortion\n"
"	newPos.y = c*v / (pow(u, 2) + c);\n"
"	newPos.x = (newPos.x + 1.0f)*0.5f;	 // Convert range back to 0.0 to 1.0 (roughly, more like -1/14 to +15/14) which is why you get the clamped\n"
"	newPos.y = (newPos.y + 1.0f)*0.5f;	 // textures repeating to the edge where you're out of the texture coord range of 0.0 to 1.0\n"
"	return newPos;\n"
"}\n"

"float4 PS( VS_OUT vtx ) : SV_Target\n"
"{\n"
"   return float4(fontTexture.Sample( fontSampler, Warp(vtx.TexCoord) ).xyz, 1.0);\n"
"}\n";

/**
* Simple Distortion Shader.
* Improved distortion method taken from OSVR core repository.
***/
static const char* PS_DIST_SIMPLE =
"Texture2D fontTexture : register(t0);\n"
"SamplerState fontSampler : register(s0);\n"

"struct VS_OUT\n"
"{\n"
"   float4 Position  : SV_POSITION;\n"
"   float2 TexCoord : TEXCOORD0;\n"
"};\n"

"float2 Distort(float2 p : TEXCOORD0)\n"
"{\n"
"	p.x = p.x * 2.0f - 1.0f;             // Texture coordinates converted to -1.0 to 1.0 range\n"
"	p.y = p.y * 2.0f - 1.0f;\n"
"   float r2 = p.x * p.x + p.y * p.y;    // Distortion \n"
"	float newRadius = (1 + 0.3f * r2);   // -> K1 \n"
"	p.x = p.x * newRadius * 0.8f;         // Aspect ratio fix. \n"
"	p.y = p.y * newRadius * 1.2f;        // Aspect ratio fix. \n"
"	p.x = (p.x + 1.0f) * 0.5f;	         // Convert range back to 0.0 to 1.0\n"
"	p.y = (p.y + 1.0f) * 0.5f; "
"	return p;\n"
"}\n"

"float4 PS( VS_OUT vtx ) : SV_Target\n"
"{\n"
"   float2 fDist = Distort(vtx.TexCoord);\n"
"   if ((fDist.x < 0.0f) || (fDist.x > 1.0f) || (fDist.y < 0.0f) || (fDist.y > 1.0f)) return float4(0.0f, 0.0f, 0.0f, 0.0f);\n"
"   return float4(fontTexture.Sample( fontSampler, fDist ).xyz, 1.0);\n"
"}\n";

/**
* 3D Pixel Shader.
***/
static const char* PS3D =
// constant buffer
/*"cbuffer cbPerObject : register(b0)\n"
"{\n"
"	float4		g_vObjectColor : packoffset(c0);\n"
"};\n"

"cbuffer cbPerFrame : register(b1)\n"
"{\n"
"	float3		g_vLightDir : packoffset(c0);\n"
"	float		g_fAmbient : packoffset(c0.w);\n"
"};\n"*/

// textures and samplers
"Texture2D	g_txDiffuse : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

// input / output structures
"struct PS_INPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float3 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"
"	float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);\n"

// "	float fLighting = saturate(dot(g_vLightDir, Input.vNormal));\n"
// "	fLighting = max(fLighting, g_fAmbient);\n"

"	return vDiffuse;\n" // * fLighting;\n"
"}\n";

/*"texture g_MeshTexture;"
"sampler MeshTextureSampler =\n"
"sampler_state\n"
"{\n"
"	Texture = <g_MeshTexture>;\n"
"	MipFilter = LINEAR;\n"
"	MinFilter = LINEAR;\n"
"	MagFilter = LINEAR;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"	float4 Position : POSITION;\n"
"	float4 Diffuse : COLOR0;\n"
"	float2 TextureUV : TEXCOORD0;\n"
"};\n"

"struct PS_OUTPUT\n"
"{\n"
"	float4 RGBColor : COLOR0;\n"
"};\n"

"PS_OUTPUT PS(VS_OUTPUT In)\n"
"{\n"
"	PS_OUTPUT Output;\n"

"	Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV) * In.Diffuse;\n"

"	return Output;\n"
"}\n";*/

/**
* Simple enumeration of available pixel shaders.
***/
enum PixelShaderTechnique
{
	FullscreenSimple,
	WarpSimple,
	DistortSimple,
	FullscreenGammaCorrection,
	GeometryDiffuseTextured,
};

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
* Creates a simple 3D vertex shader with an input layout.
***/
HRESULT Create3DVertexShader(ID3D11Device* pcDevice, ID3D11VertexShader** ppcVertexShader, ID3D11InputLayout** ppcInputLayout)
{
	if ((!ppcVertexShader) || (!ppcInputLayout) || (!pcDevice)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;

	// compile and create shader
	if (SUCCEEDED(hr = D3DX10CompileFromMemory(VS3D, strlen(VS3D), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL)))
	{
		//#ifdef _DEBUG
		OutputDebugString(L"Vireio Perception : Vertex Shader compiled !");
		//#endif
		hr = pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, ppcVertexShader);

		if (FAILED(hr))
		{
			OutputDebugString(L"Vireio Perception : Failed to create Vertex Shader !");
			return hr;
		}
		else
		{
			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC aLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT unNumElements = sizeof(aLayout) / sizeof(aLayout[0]);

			hr = pcDevice->CreateInputLayout(aLayout, unNumElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), ppcInputLayout);
			if (FAILED(hr))
			{
				OutputDebugString(L"Vireio Perception : Failed to create input layout !");
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
HRESULT CreateSimplePixelShader(ID3D11Device* pcDevice, ID3D11PixelShader** ppcPixelShader, PixelShaderTechnique eTechnique)
{
	if ((!pcDevice) || (!ppcPixelShader)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;

	// compile selecting technique
	switch (eTechnique)
	{
		case FullscreenSimple:
			hr = D3DX10CompileFromMemory(PS2D, strlen(PS2D), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case WarpSimple:
			hr = D3DX10CompileFromMemory(PS_WARP_SIMPLE, strlen(PS_WARP_SIMPLE), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case DistortSimple:
			hr = D3DX10CompileFromMemory(PS_DIST_SIMPLE, strlen(PS_DIST_SIMPLE), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case FullscreenGammaCorrection:
			hr = D3DX10CompileFromMemory(PS2D_GAMMA_CORRECTION, strlen(PS2D_GAMMA_CORRECTION), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case GeometryDiffuseTextured:
			hr = D3DX10CompileFromMemory(PS3D, strlen(PS3D), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
	}

	// succeded ?
	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"Vireio Perception : Pixel Shader compiled !");
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
	bd.ByteWidth = sizeof(TexturedVertex) * 6;
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
	cbDesc.ByteWidth = sizeof(D3DMATRIX);
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
	return pcDevice->CreateBuffer(&cbDesc, &InitData, ppcBuffer);
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




