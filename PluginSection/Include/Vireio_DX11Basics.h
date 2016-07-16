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

#pragma region shader data structures
/**
* Texture vertex structure.
***/
struct TexturedVertex
{
	D3DXVECTOR4 sPos;
	D3DXVECTOR2 sTex;
};

/**
* Textured vertex structure with normal.
* Matches OpenVR render model vertex.
**/
struct TexturedNormalVertex
{
	D3DXVECTOR3 sPosition;
	D3DXVECTOR3 sNormal;
	D3DXVECTOR2 sTextureCoord;
};

/**
* DX11 simple model structure.
***/
struct RenderModel_D3D11
{
	ID3D11Buffer* pcVertexBuffer;               /**< Vertex buffer for the mesh **/
	UINT32 unVertexCount;						/**< Number of vertices in the vertex data **/
	ID3D11Buffer* pcIndexBuffer;                /**< Indices into the vertex data for each triangle **/
	UINT32 unTriangleCount;					    /**< Number of triangles in the mesh. Index count is 3 * TriangleCount **/
	ID3D11Texture2D* pcTexture;                 /**< Texture **/
	ID3D11ShaderResourceView* pcTextureSRV;     /**< Texture SRV **/
	ID3D11PixelShader* pcEffect;                /**< Pixel shader effect for that render model, NULL for main effect and main texture **/
};

/**
* Geometry constant buffer structure.
***/
struct GeometryConstantBuffer
{
	D3DXCOLOR   sMaterialAmbientColor; /**< ambient material color **/
	D3DXCOLOR   sMaterialDiffuseColor; /**< diffuse material color **/

	D3DXVECTOR4 sLightDir;             /**< light direction vector **/
	D3DXCOLOR   sLightDiffuse;         /**< diffuse light color **/
	D3DXCOLOR   sLightAmbient;         /**< ambient light color **/

	D3DXMATRIX  sWorldViewProjection;  /**< world view projection matrix **/
	D3DXMATRIX  sWorld;                /**< world matrix **/

	D3DXVECTOR3 sResolution;           /**< viewport resolution (in pixels) **/
	float       fGlobalTime;           /**< shader playback time (in seconds) **/
	D3DXVECTOR4 sMouse;                /**< mouse pixel coords. xy: current (if MLB down), zw: click **/
};
#pragma endregion
#pragma region vertex shader
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
"float4 sMaterialAmbientColor;\n"
"float4 sMaterialDiffuseColor;\n"

"float4 sLightDir;\n"
"float4 sLightDiffuse;\n"
"float4 sLightAmbient;\n"

"float4x4 sWorldViewProjection;\n"
"float4x4 sWorld;\n"

// input / output structures
"struct VS_INPUT\n"
"{\n"
"	float4 vPosition : POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

// vertex shader
"VS_OUTPUT VS(VS_INPUT Input)\n"
"{\n"
"	VS_OUTPUT Output;\n"

"	Output.vPosition = mul(Input.vPosition, sWorldViewProjection);\n"
"	Output.vNormal = mul(Input.vNormal, sWorld);\n"
"	Output.vTexcoord = Input.vTexcoord;\n"

"	return Output;\n"
"}\n";
#pragma endregion
#pragma region pixel shader basic
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
#pragma endregion
#pragma region pixel shader enhanced
#pragma region PS3D
/**
* 3D Pixel Shader.
***/
static const char* PS3D =
// constant buffer
"float4 sMaterialAmbientColor;\n"
"float4 sMaterialDiffuseColor;\n"

"float4 sLightDir;\n"
"float4 sLightDiffuse;\n"
"float4 sLightAmbient;\n"

"float4x4 sWorldViewProjection;\n"
"float4x4 sWorld;\n"

// textures and samplers
"Texture2D	g_txDiffuse : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

// input / output structures
"struct PS_INPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"
"	float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);\n"

"	float4 fLighting = saturate(dot(sLightDir, Input.vNormal));\n"
"	fLighting = max(fLighting, sLightAmbient);\n"

"	return vDiffuse * fLighting;\n"
"}\n";
#pragma endregion
#pragma region PS3D_FABRIC
/**
* 3D Pixel Shader Fabric.
***/
static const char* PS3D_FABRIC =
// constant buffer
"float4 sMaterialAmbientColor;\n"
"float4 sMaterialDiffuseColor;\n"

"float4 sLightDir;\n"
"float4 sLightDiffuse;\n"
"float4 sLightAmbient;\n"

"float4x4 sWorldViewProjection;\n"
"float4x4 sWorld;\n"

// textures and samplers
"Texture2D	g_txDiffuse : register(t0);\n"
// "Texture2D	g_txNoise : register(t1);\n"
"SamplerState g_samLinear : register(s0);\n"

// input / output structures
"struct PS_INPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

"float gaussian(float d2, float radius)\n"
"{\n"
"	return exp(-d2 / radius);\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

// create some noise grid
"float2 noisy;\n"
"noisy.x = gaussian(Input.vTexcoord.x / 12.0f, 0.06f);\n"
"noisy.y = gaussian(Input.vTexcoord.y / 10.0f, 0.08f);\n"

"float2 fp = frac(0.06 * Input.vTexcoord + 1.92 * (2 * noisy - 1));\n"
"fp *= (1 - fp);\n"

// get texel
"float4 diffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord) * (fp.x + fp.y + noisy.x);\n"

"float4 fLighting = saturate(dot(sLightDir, Input.vNormal));\n"
"fLighting = max(fLighting, sLightAmbient);\n"

"return diffuse * fLighting;\n"

"}\n";
#pragma endregion
#pragma region PS3D_BUMP
/**
* 3D Pixel Shader bump mapping, compute normal map.
***/
static const char* PS3D_BUMP =
// constant buffer
"float4 sMaterialAmbientColor;\n"
"float4 sMaterialDiffuseColor;\n"

"float4 sLightDir;\n"
"float4 sLightDiffuse;\n"
"float4 sLightAmbient;\n"

"float4x4 sWorldViewProjection;\n"
"float4x4 sWorld;\n"

// textures and samplers
"Texture2D	g_txDiffuse : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

// input / output structures
"struct PS_INPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"	float fOffset = 1.0 / 92.0;\n"

// Take all neighbor samples
"	float4 s00 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(-fOffset, -fOffset));\n"
"	float4 s01 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(0.0f, -fOffset));\n"
"	float4 s02 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(fOffset, -fOffset));\n"

"	float4 s10 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(-fOffset, 0.0f));\n"
"	float4 s12 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(fOffset, 0.0f));\n"

"	float4 s20 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(-fOffset, fOffset));\n"
"	float4 s21 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(0.0f, fOffset));\n"
"	float4 s22 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord + float2(fOffset, fOffset));\n"

// Slope in X direction
"	float4 sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;\n"
// Slope in Y direction
"	float4 sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;\n"

// Weight the slope in all channels, we use grayscale as height
"	float4 lightness = float4(1.0, 1.0, 1.0, 0.0);\n"
"	float sx = dot(sobelX, lightness);\n"
"	float sy = dot(sobelY, lightness);\n"

// Compose the normal
"	float3 fvNormal = normalize(float3(sx, sy, 1.0f));\n"

// Pack [-1, 1] into [0, 1]
"	float4 vNormal = float4(fvNormal * 0.5f + 0.5f, 1.0f);\n"

// color + lighting
"	float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);\n"

"	float4 fLighting = saturate(dot(sLightDir, vNormal)) * 2.0;\n"
"   float4 fLightingNormal = saturate(dot(sLightDir, Input.vNormal));\n"
"	fLighting = max(fLighting, fLightingNormal);\n"

"	return vDiffuse * fLighting;\n"

"}\n";
#pragma endregion
#pragma region PS_STRING_THEORY
/**
* Pixel Shader from shadertoy.com
* "String Theory" by nimitz (twitter: @stormoid)
* https://www.shadertoy.com/view/XdSSz1
* This work is licenced under :
* https://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US
***/
static const char* PS_STRING_THEORY =
// constant buffer
"float4 sMaterialAmbientColor;\n"
"float4 sMaterialDiffuseColor;\n"

"float4 sLightDir;\n"
"float4 sLightDiffuse;\n"
"float4 sLightAmbient;\n"

"float4x4 sWorldViewProjection;\n"
"float4x4 sWorld;\n"

// shadertoy constant buffer fields
"float3    sResolution;\n"           // viewport resolution (in pixels)
"float     fGlobalTime;\n"           // shader playback time (in seconds)
"float4    sMouse;\n"                // mouse pixel coords. xy: current (if MLB down), zw: click

// textures and samplers
"Texture2D	g_txDiffuse : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

// input / output structures
"struct PS_INPUT\n"
"{\n"
"	float4 vPosition : SV_POSITION;\n"
"	float4 vNormal : NORMAL;\n"
"	float2 vTexcoord : TEXCOORD0;\n"
"};\n"

//String Theory by nimitz (twitter: @stormoid)

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"

"#define BASE_ANGLE 3.5\n"
"#define ANGLE_DELTA 0.02\n"
"#define XOFF .7\n"

"#define time fGlobalTime\n"

"mat2 mm2(float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }\n"

"float f(vec2 p, float fSize)\n"
"{\n"
"	p.x = sin(p.x*1. + time*1.2)*sin(time + p.x*0.1)*3.;\n"
"	p += sin(p.x*1.5)*.1;\n"
"	return smoothstep(-0.0, fSize, abs(p.y));\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"float aspect = sResolution.x / sResolution.y;\n"
"float featureSize = 60. / ((sResolution.x*aspect + sResolution.y));\n"

//"	vec2 p = Input.vPosition.xy / sResolution.xy*6.5 - 3.25;\n" /**< original computation from shadertoy.com for fullscreen output ***/
"	vec2 p = Input.vTexcoord * 6.5 - 3.25;\n"

"	p.x *= aspect;\n"
"	p.y = abs(p.y);\n"

"	vec3 col = vec3(0.0, 0.0, 0.0);\n"
"	for (float i = 0.; i<26.; i++)\n"
"	{\n"
"		vec3 col2 = (sin(vec3(3.3, 2.5, 2.2) + i*0.15)*0.5 + 0.54)*(1. - f(p, featureSize));\n"
"		col = max(col, col2);\n"

"		p.x -= XOFF;\n"
"		p.y -= sin(time*0.11 + 1.5)*1.5 + 1.5;\n"
"       mat2 temp = mm2(i*ANGLE_DELTA + BASE_ANGLE);\n"
"       p = mul( p, temp );\n"

"		vec2 pa = vec2(abs(p.x - .9), abs(p.y));\n"
"		vec2 pb = vec2(p.x, abs(p.y));\n"

"		p = mix(pa, pb, smoothstep(-.07, .07, sin(time*0.24) + .1));\n"
"	}\n"

"   return vec4(col, 1.0);\n"
"}\n";
#pragma endregion
#pragma endregion

/**
* Simple enumeration of available pixel shaders.
***/
enum PixelShaderTechnique
{
	FullscreenSimple,          /**< TexturedVertex **/
	WarpSimple,                /**< TexturedVertex **/
	DistortSimple,             /**< TexturedVertex **/
	FullscreenGammaCorrection, /**< TexturedVertex **/
	GeometryDiffuseTextured,   /**< TexturedNormalVertex : simple lighting **/
	Fabric,                    /**< TexturedNormalVertex : fabric effect **/
	BumpComputeNormal,         /**< TexturedNormalVertex : bump mapping, computes per pixel normals **/
	StringTheory,              /**< TexturedNormalVertex : "String Theory" effect from shadertoy.com **/
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
* Creates a pixel shader effect.
***/
HRESULT CreatePixelShaderEffect(ID3D11Device* pcDevice, ID3D11PixelShader** ppcPixelShader, PixelShaderTechnique eTechnique)
{
	if ((!pcDevice) || (!ppcPixelShader)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;
	switch (eTechnique)
	{
		case FullscreenSimple:
			break;
		case WarpSimple:
			break;
		case DistortSimple:
			break;
		case FullscreenGammaCorrection:
			break;
		case GeometryDiffuseTextured:
			break;
		case Fabric:
			break;
		case BumpComputeNormal:
			break;
		case StringTheory:
			break;
		default:
			break;
	}
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
		case Fabric:
			hr = D3DX10CompileFromMemory(PS3D_FABRIC, strlen(PS3D_FABRIC), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case BumpComputeNormal:
			hr = D3DX10CompileFromMemory(PS3D_BUMP, strlen(PS3D_BUMP), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case StringTheory:
			hr = D3DX10CompileFromMemory(PS_STRING_THEORY, strlen(PS_STRING_THEORY), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		default:
			return E_INVALIDARG;
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
* Creates a simple matrix constant buffer.
***/
HRESULT CreateGeometryConstantBuffer(ID3D11Device* pcDevice, ID3D11Buffer** ppcBuffer, UINT unSize)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = unSize;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Create the buffer.
	return pcDevice->CreateBuffer(&cbDesc, nullptr, ppcBuffer);
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




