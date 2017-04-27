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

#include <DirectXMath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include"Vireio_DX11StateBlock.h"

using namespace DirectX;
#define float2 XMFLOAT2
#define float3 XMFLOAT3
#define float4 XMFLOAT4
#define float4x4 XMMATRIX
#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

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
	struct Vireio_Resolution
	{
		UINT32 unWidth;
		UINT32 unHeight;
	} sResolution;                              /**< Resolution of the effect render target. **/
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
	float       fGamma;                /**< Gamma correction value **/
	float       fReserved[3];          /**< Reserved data to ensure right buffer size **/

	D3DXCOLOR   sColorFX[8];           /**< Colors to be used by a shader effect **/
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

/**
* 3D Vertex Shader.
***/
static const char* VS3D_TEXT =
"cbuffer Constants0 : register(b0)\n"
"{\n"
// constant buffer
"	float4 sMaterialAmbientColor;\n"
"	float4 sMaterialDiffuseColor;\n"

"	float4 sLightDir;\n"
"	float4 sLightDiffuse;\n"
"	float4 sLightAmbient;\n"

"	float4x4 sWorldViewProjection;\n"
"	float4x4 sWorld;\n"

// shadertoy constant buffer fields
"	float3    sResolution;\n"
"	float     fGlobalTime;\n"
"	float4    sMouse;\n"

// gamma correction
"	float     fGamma;\n"
"};\n"

"cbuffer ConstantsGlyph : register(b1)\n"
"{\n"
"	uint unCharacter;\n"
"	float fULeft;\n"
"	float fVTop;\n"
"	float fURight;\n"
"	float fVBottom;\n"
"	float fXOffset;\n"
"	float fYOffset;\n"
"	float fXAdvance;\n"
"};\n"

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

// pass uv
"	if (Input.vTexcoord.x < 0.5f) Output.vTexcoord.x = fULeft; else Output.vTexcoord.x = fURight;\n"
"	if (Input.vTexcoord.y < 0.5f) Output.vTexcoord.y = fVTop; else Output.vTexcoord.y = fVBottom;\n"

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
* 3D Pixel Shader simple lighting.
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

// shadertoy constant buffer fields
"float3    sResolution;\n"           // viewport resolution (in pixels)
"float     fGlobalTime;\n"           // shader playback time (in seconds)
"float4    sMouse;\n"                // mouse pixel coords. xy: current (if MLB down), zw: click

// gamma correction
"float     fGamma;\n"                // Gamma correction value 

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

"	float4 vLighting = saturate(dot(sLightDir, Input.vNormal));\n"
"	vLighting = max(vLighting, sLightAmbient);\n"

"	return vDiffuse * vLighting;\n"
"}\n";
#pragma endregion
#pragma region PS3D_MOUSE
/**
* 3D Pixel Shader with mouse cursor drawn.
* Mouse cursor technique from shadertoy.com pixel shader "CursorTest" by jherico.
* "CursorTest" is licenced under :
* https://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US
***/
static const char* PS3D_MOUSE =
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

// gamma correction
"float     fGamma;\n"                // Gamma correction value 

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

"float easeInOutCubic(float f) {\n"
"	const float d = 1.0;\n"
"	const float b = 0.0;\n"
"	const float c = 1.0;\n"
"	float t = f;\n"
"	if ((t /= d / 2.0) < 1.0) return c / 2.0 * t * t * t + b;\n"
"	return c / 2.0 * ((t -= 2.0) * t * t + 2.0) + b;\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"
"	float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);\n"

"	float4 vLighting = saturate(dot(sLightDir, Input.vNormal));\n"
"	vLighting = max(vLighting, sLightAmbient);\n"

"   float fMouseX = abs(Input.vTexcoord.x - sMouse.x);\n"
"   float fMouseY = abs(Input.vTexcoord.y - sMouse.y);\n"
"   if (fMouseX < 0.01 && fMouseY < 0.01) {\n"

"   float2 fAspect = float2(1.0, sResolution.y / sResolution.x);\n"
"   float dist = distance(Input.vTexcoord * fAspect, sMouse.xy * fAspect);\n"
"	float glowIntensity = 1.0 - (dist / 0.005);\n"
"   float4 glowColor = pow(float4(0.2, 1.0, 0.5, 1.0), float4(1.0 - glowIntensity, 1.0 - glowIntensity, 1.0 - glowIntensity, 1.0));\n"
"   glowIntensity = easeInOutCubic(glowIntensity); \n"
"   glowIntensity *= 0.9; \n"
"   if (glowIntensity > 0.0)\n"
"   { return lerp(vDiffuse, glowColor, glowIntensity); } }\n"

"   return pow (vDiffuse * vLighting, fGamma);\n"
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

"float4 vLighting = saturate(dot(sLightDir, Input.vNormal));\n"
"vLighting = max(vLighting, sLightAmbient);\n"

"return diffuse * vLighting;\n"

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

"	float4 vLighting = saturate(dot(sLightDir, vNormal)) * 2.0;\n"
"   float4 vLightingNormal = saturate(dot(sLightDir, Input.vNormal));\n"
"	vLighting = max(vLighting, vLightingNormal);\n"

"	return vDiffuse * vLighting;\n"

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
"   p.x *= sResolution.x / sResolution.y;\n"

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
#pragma region PS_BUBBLES
/**
* Pixel Shader from shadertoy.com
* "Bubbles!" by weyland
* Created by inigo quilez - iq/2013 : https://www.shadertoy.com/view/4dl3zn
* License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
* Messed up by Weyland
***/
static const char* PS_BUBBLES =
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

// "Bubbles!" by weyland

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

// vec2 uv = -1.0 + 2.0*fragCoord.xy / sResolution.xy; // original code
// uv.x *= sResolution.x / sResolution.y;
"   vec2 uv = -1.0 + 2.0 * Input.vTexcoord;\n"
"   uv.x *= sResolution.x / sResolution.y;\n"

// background	 
"   vec3 color = vec3(1.0, 1.0, 1.0);\n"

// bubbles	
"   for (int i = 0; i<64; i++)\n"
"   {\n"
// bubble seeds
"       float pha = sin(float(i)*546.13 + 1.0)*0.5 + 0.5;\n"
"       float siz = pow(sin(float(i)*651.74 + 5.0)*0.5 + 0.5, 4.0);\n"
"       float pox = sin(float(i)*321.55 + 4.1) * sResolution.x / sResolution.y;\n"

// bubble size, position and color
"       float rad = 0.1 + 0.5*siz + sin(fGlobalTime / 6. + pha*500.0 + siz) / 20.0;\n"
"       vec2  pos = vec2(pox + sin(fGlobalTime / 10. + pha + siz), -1.0 - rad + (2.0 + 2.0*rad)\n"
"	      	*mod(pha + 0.1*(fGlobalTime / 5.0)*(0.2 + 0.8*siz), 1.0));\n"
"       float dis = length(uv - pos);\n"

"       float fTemp = 0.5 + 0.5*sin(float(i)*1.2 + 1.9);"
"       vec3  col = mix(vec3(0.194*sin(fGlobalTime / 6.0), 0.3, 0.0),\n"
"   		vec3(1.1*sin(fGlobalTime / 9.0), 0.4, 0.8),\n"
"	     	vec3(fTemp, fTemp, fTemp));\n"
// render
"       float f = length(uv - pos) / rad;\n"
"       f = sqrt(clamp(1.0 + (sin((fGlobalTime / 7.0) + pha*500.0 + siz)*0.5) - f*f, 0.0, 1.0));\n"
"       color -= col.zyx *(1.0 - smoothstep(rad*0.95, rad, dis)) * f;\n"
"   }\n"
"   return vec4(color, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_C64_PLASMA
/**
* Pixel Shader from shadertoy.com
* 2D plasma in C64 graphics style
*
* Version 1.0 (2013-03-31)
* Simon Stelling-de San Antonio
* This work is licenced under :
* https://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US
***/
static const char* PS_C64_PLASMA =
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

// "C64 plasma" by ssdsa

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"float camtime = 1.23*fGlobalTime;\n"

// vec2 p = fragCoord.xy / sResolution.xy; (original code)
"vec2 p = -1.0 + 2.0 * Input.vTexcoord;\n"
"p.x *= sResolution.x / sResolution.y;\n"

"p.y = 1.0 - p.y;\n"
"p *= 200.0;\n"
"p.x *= (sResolution.x / sResolution.y);\n"
"p.x /= 2.0;\n"
"p = floor(p);\n"
"p.x *= 2.0;\n"
"float a = p.x + 30.0*sin(p.x / 21.0 + 0.3*sin(0.4*camtime)) + 20.0*cos(p.y / 19.0 + 0.2*cos(0.6*camtime)) - 160.0;\n"
"float b = p.y + 30.0*cos(p.y / 18.0 + 0.4*sin(0.7*camtime)) + 20.0*sin(p.x / 16.0 + 0.5*cos(0.7*camtime)) - 97.0;\n"
"float e = floor((length(vec2(a, b))\n"
"	+ 4.0*mod(floor((p.x + p.y + p.y) / 2.0), 2.0)) / 13.0);\n"
"float c;\n"
"if (e == 0.0)\n"
"{\n"
"	c = 9.0;\n"
"}\n"
"else if (e == 1.0)\n"
"{\n"
"	c = 2.0;\n"
"}\n"
"else if (e == 2.0)\n"
"{\n"
"	c = 8.0;\n"
"}\n"
"else if (e == 3.0)\n"
"{\n"
"	c = 10.0;\n"
"}\n"
"else if (e == 4.0)\n"
"{\n"
"	c = 15.0;\n"
"}\n"
"else if (e == 5.0)\n"
"{\n"
"	c = 7.0;\n"
"}\n"
"else if (e == 6.0)\n"
"{\n"
"	c = 1.0;\n"
"}\n"
"else if (e == 7.0)\n"
"{\n"
"	c = 13.0;\n"
"}\n"
"else if (e == 8.0)\n"
"{\n"
"	c = 3.0;\n"
"}\n"
"else if (e == 9.0)\n"
"{\n"
"	c = 14.0;\n"
"}\n"
"else if (e == 10.0)\n"
"{\n"
"	c = 4.0;\n"
"}\n"
"else if (e == 11.0)\n"
"{\n"
"	c = 6.0;\n"
"}\n"
"else if (e == 12.0)\n"
"{\n"
"	c = 0.0;\n"
"}\n"
"else if (e == 13.0)\n"
"{\n"
"	c = 11.0;\n"
"}\n"
"else if (e == 14.0)\n"
"{\n"
"	c = 5.0;\n"
"}\n"
"else\n"
"{\n"
"	c = 12.0;\n"
"}\n"
"vec3 col;\n"
"if (c == 0.0)\n"
"{\n"
"	col = vec3(0.0, 0.0, 0.0);\n"
"}\n"
"else if (c == 1.0)\n"
"{\n"
"	col = vec3(1.0,1.0,1.0);\n"
"}\n"
"else if (c == 2.0)\n"
"{\n"
"	col = vec3(137.0, 64.0, 54.0) / 256.0;\n"
"}\n"
"else if (c == 3.0)\n"
"{\n"
"	col = vec3(122.0, 191.0, 199.0) / 256.0;\n"
"}\n"
"else if (c == 4.0)\n"
"{\n"
"	col = vec3(138.0, 70.0, 174.0) / 256.0;\n"
"}\n"
"else if (c == 5.0)\n"
"{\n"
"	col = vec3(104.0, 169.0, 65.0) / 256.0;\n"
"}\n"
"else if (c == 6.0)\n"
"{\n"
"	col = vec3(62.0, 49.0, 162.0) / 256.0;\n"
"}\n"
"else if (c == 7.0)\n"
"{\n"
"	col = vec3(208.0, 220.0, 113.0) / 256.0;\n"
"}\n"
"else if (c == 8.0)\n"
"{\n"
"	col = vec3(144.0, 95.0, 37.0) / 256.0;\n"
"}\n"
"else if (c == 9.0)\n"
"{\n"
"	col = vec3(92.0, 71.0, 0.0) / 256.0;\n"
"}\n"
"else if (c == 10.0)\n"
"{\n"
"	col = vec3(187.0, 119.0, 109.0) / 256.0;\n"
"}\n"
"else if (c == 11.0)\n"
"{\n"
"	col = vec3(85.0, 85.0, 85.0) / 256.0;\n"
"}\n"
"else if (c == 12.0)\n"
"{\n"
"	col = vec3(128.0, 128.0, 128.0) / 256.0;\n"
"}\n"
"else if (c == 13.0)\n"
"{\n"
"	col = vec3(172.0, 234.0, 136.0) / 256.0;\n"
"}\n"
"else if (c == 14.0)\n"
"{\n"
"	col = vec3(124.0, 112.0, 218.0) / 256.0;\n"
"}\n"
"else\n"
"{\n"
"	col = vec3(171.0, 171.0, 171.0) / 256.0;\n"
"}\n"

"   return vec4(col, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_TOON_CLOUD
/**
* Pixel Shader from shadertoy.com
* ----------------------------------------------------------------------------------------
*	"Toon Cloud" by Antoine Clappier - March 2015
*
*	Licensed under:
*  A Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	http://creativecommons.org/licenses/by-nc-sa/4.0/
* ----------------------------------------------------------------------------------------
***/
static const char* PS_TOON_CLOUD =
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

// "Toon Cloud" by Antoine Clappier - March 2015

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define TAU 6.28318530718\n"

"float Func(float pX)\n"
"{\n"
"	return 0.6*(0.5*sin(0.1*pX) + 0.5*sin(0.553*pX) + 0.7*sin(1.2*pX));\n"
"}\n"

"float FuncR(float pX)\n"
"{\n"
"	return 0.5 + 0.25*(1.0 + sin(mod(40.0*pX, TAU)));\n"
"}\n"

"float Layer(vec2 pQ, float pT)\n"
"{\n"
"	vec2 Qt = 3.5*pQ;\n"
"	pT *= 0.5;\n"
"	Qt.x += pT;\n"

"	float Xi = floor(Qt.x);\n"
"	float Xf = Qt.x - Xi - 0.5;\n"

"	vec2 C;\n"
"	float Yi;\n"
"	float D = 1.0 - step(Qt.y, Func(Qt.x));\n"

// Disk:
"	Yi = Func(Xi + 0.5);\n"
"	C = vec2(Xf, Qt.y - Yi);\n"
"	D = min(D, length(C) - FuncR(Xi + pT / 80.0));\n"

// Previous disk:
"	Yi = Func(Xi + 1.0 + 0.5);\n"
"	C = vec2(Xf - 1.0, Qt.y - Yi);\n"
"	D = min(D, length(C) - FuncR(Xi + 1.0 + pT / 80.0));\n"

// Next Disk:
"	Yi = Func(Xi - 1.0 + 0.5);\n"
"	C = vec2(Xf + 1.0, Qt.y - Yi);\n"
"	D = min(D, length(C) - FuncR(Xi - 1.0 + pT / 80.0));\n"

"	return min(1.0, D);\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"
// set constant colors
"	vec3 BackColor = vec3(0.0, 0.4, 0.58);\n"
"	vec3 CloudColor = vec3(0.18, 0.70, 0.87);\n"

//// Setup:
//"	vec2 UV = 2.0*(fragCoord.xy - fResolution.xy / 2.0) / min(fResolution.x, fResolution.y);\n" // original code
"	vec2 UV = -1.0 + 2.0 * Input.vTexcoord;\n"
"   UV.x *= sResolution.x / sResolution.y;\n"

//// Render:
"	vec3 Color = BackColor;\n"

"	for (float J = 0.0; J <= 1.0; J += 0.2)\n"
"	{\n"
// Cloud Layer: 
"		float Lt = fGlobalTime*(0.5 + 2.0*J)*(1.0 + 0.1*sin(226.0*J)) + 17.0*J;\n"
"		vec2 Lp = vec2(0.0, 0.3 + 1.5*(J - 0.5));\n"
"		float L = Layer(UV + Lp, Lt);\n"

// Blur and color:
"		float Blur = 4.0*(0.5*abs(2.0 - 5.0*J)) / (11.0 - 5.0*J);\n"

"		float V = mix(0.0, 1.0, 1.0 - smoothstep(0.0, 0.01 + 0.2*Blur, L));\n"
"		vec3 Lc = mix(CloudColor, vec3(1.0, 1.0, 1.0), J);\n"

"		Color = mix(Color, Lc, V);\n"
"	}\n"

"   return vec4(Color, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_WORLEY_01
/**
* Pixel Shader from shadertoy.com
* "Worley Algorithm (Cell Noise )" by  Yeis
* This work is licenced under :
* https://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US
***/
static const char* PS_WORLEY_01 =
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

// "Worley Algorithm (Cell Noise )" by  Yeis 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

"float length2(vec2 p){\n"
//producto punto entre los 2 vectore vx*vx + vY*vY
"	return dot(p, p);\n"
"}\n"
"float noise(vec2 p)\n"
"{\n"
//funcion de ruido pseudo-aleatoria basada en la funcio seno 
"	return fract(sin(fract(sin(p.x)*(41.13311)) + p.y)*31.0011);\n"
"}\n"

"float worley(vec2 p) {\n"
//ponemos un numero grandote 
"	float d = 1e30;\n"
//checamos todos los puntos vecinos en 9 direcciones 
"	for (int xo = -1; xo <= 1; ++xo)\n"
"	{\n"
"		for (int yo = -1; yo <= 1; ++yo)\n"
"		{\n"
"			vec2 tp = floor(p) + vec2(xo, yo);\n"
"			d = min(d, length2(p - tp - noise(tp)));\n"
"		}\n"
"	}\n"
//funcion exponencial mamona 3.0*exp(-4.0*abs(2.0*d - 1.0)).
"	return 3.0*exp(-4.0*abs((2.5*d) - 1.0));\n"
"}\n"

"float fworley(vec2 p)\n"
"{\n"
"	return sqrt(sqrt(sqrt(worley(p * 5.0 + 0.05 * fGlobalTime) *\n"
"		sqrt(worley(p*50.0 + 0.12 + -0.1*fGlobalTime)) *\n"
"		sqrt(sqrt(worley(p*-10.0 + 00.03*fGlobalTime))))));\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

//"	vec2 uv = fragCoord.xy / fResolution.xy;\n" // original code
"	vec2 uv = -1.0 + 2.0 * Input.vTexcoord;\n"
"   uv.x *= sResolution.x / sResolution.y;\n"

"	float wolo = fworley(uv*sResolution.xy / 1500.0);\n"
// Repeat the grid pattern 5 times by multiplying by uv.
"	wolo *= exp(-length2(abs(0.7*uv - 1.0)));\n"

//link http://ibreakdownshaders.blogspot.mx/
"   return vec4(wolo * vec3(1.0, 0.1*wolo, pow(wolo, 0.90 - wolo)), 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_WATER_CAUSTIC
/**
* Pixel Shader from shadertoy.com
// Tileable Water Caustic
// Found this on GLSL sandbox. I really liked it, changed a few things and made it tileable.
// :)
// by David Hoskins.


// Water turbulence effect by joltz0r 2013-07-04, improved 2013-07-07
***/
static const char* PS_WATER_CAUSTIC =
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

// 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

"#define TAU 6.28318530718\n"
"#define MAX_ITER 5\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"
"	float time = fGlobalTime * .5 + 23.0;\n"

// uv should be the 0-1 uv of texture...
// "	vec2 uv = fragCoord.xy / fResolution.xy;\n" // original code
"	vec2 uv = Input.vTexcoord;\n"

"	vec2 p = mod(uv*TAU, TAU) - 250.0;\n"
"	vec2 i = vec2(p);\n"
"	float c = 1.0;\n"
"	float inten = .005;\n"

"	for (int n = 0; n < MAX_ITER; n++)\n"
"	{\n"
"		float t = time * (1.0 - (3.5 / float(n + 1)));\n"
"		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));\n"
"		c += 1.0 / length(vec2(p.x / (sin(i.x + t) / inten), p.y / (cos(i.y + t) / inten)));\n"
"	}\n"
"	c /= float(MAX_ITER);\n"
"	c = 1.17 - pow(c, 1.4);\n"

"   float fTemp = pow(abs(c), 8.0);\n"
"	vec3 colour = vec3(fTemp, fTemp, fTemp);\n"
"	colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);\n"

"   return vec4(colour, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_HYPNOTIC_DISCO
/**
* Pixel Shader from shadertoy.com
* "Hypnotic disco" by
***/
static const char* PS_HYPNOTIC_DISCO =
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

// "Hypnotic disco" by 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

// Noise from IQ
"vec2 hash(vec2 p)\n"
"{\n"
"	p = vec2(dot(p, vec2(127.1, 311.7)),\n"
"		dot(p, vec2(269.5, 183.3)));\n"
"	return -1.0 + 2.0*fract(sin(p)*43758.5453123);\n"
"}\n"

"float noise(in vec2 p)\n"
"{\n"
"	const float K1 = 0.366025404;\n"
"	const float K2 = 0.211324865;\n"

"	vec2 i = floor(p + (p.x + p.y)*K1);\n"

"	vec2 a = p - i + (i.x + i.y)*K2;\n"
"	vec2 o = (a.x>a.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n"
"	vec2 b = a - o + K2;\n"
"	vec2 c = a - 1.0 + 2.0*K2;\n"

"	vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);\n"

"	vec3 n = h*h*h*h*vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));\n"

"	return dot(n, vec3(70.0, 70.0, 70.0));\n"
"}\n"
//
//// http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
"vec3 hsv2rgb(vec3 c)\n"
"{\n"
"	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
"	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
"	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
"}\n"

// Adapted from lensflare by Musk 
"vec3 spotLight(vec2 uv, vec2 pos)\n"
"{\n"
"	vec2 main = uv - pos;\n"
"	vec2 uvd = uv*(length(uv));\n"

"	float ang = atan(main.x / main.y);\n"
"	float dist = length(main); dist = pow(dist, .1);\n"

"	float f0 = 1.0 / (length(main)*16.0 + 1.0);\n"
"	f0 = f0 + f0 * (sin(noise(vec2(0., (pos.x + pos.y)*2.2 + ang*4.0 + 5.954)*2.0))*.1 + dist*.1 + .8);\n"

"	float fLength = length(uvd)*.05;\n"
"	vec3 c = -vec3(fLength, fLength, fLength);\n"
"	c += vec3(f0, f0, f0);\n"
"	return c;\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"	float T = fGlobalTime;\n"
"	float T2 = 2.*fGlobalTime;\n"
"	float T3 = 3.*fGlobalTime;\n"
"	float T_20 = fGlobalTime / 20.;\n"
//
// "	vec2 uv = fragCoord.xy / fResolution.xy;\n" <- original code
"	vec2 uv = Input.vTexcoord;\n"
"	vec2 q = 2.*uv - 1.;\n"
"	q.y *= sResolution.y / sResolution.x;\n"
//
"	float a = 360. * noise(vec2(0., T_20));\n"
"	mat2 rotmat = mat2(cos(a), -sin(a), sin(a), cos(a));\n"
"	q = mul( q, rotmat );\n"
//
"	vec3 tint = vec3(fract(a), 1., 1.);\n"

"	const float nbc = 128.;\n"
"	float st = sin(T_20);\n"
"	vec2 c = vec2(0., 0.);\n"
"	vec3 col = vec3(0., 0., 0.);\n"
"	float inc = 3.14159*cos(0.1*T);\n"
"	float d = 0.;\n"
"	for (float i = 0.; i<nbc; i += 1.)\n"
"	{\n"
"		c.x = cos(i*inc);\n"
"		c.y = sin(i*inc);\n"
"		c *= i / nbc;\n"
"		d = length(q - c);\n"
"		tint.y = fract(T2);\n"
"		tint.z = fract(i / nbc*st + T);\n"
"		col += clamp(exp(-64.*d), 0., 1.) * hsv2rgb(tint);\n"
"	}\n"
// Spots
"	vec2 pos;\n"
"	pos.x = sin(T2)*.5;\n"
"	pos.y = sin(T2*.913)*.5;\n"
"	vec3 colf = tint * (spotLight(q, pos) + spotLight(q, -pos));\n"
//
"	col = clamp(col + colf, 0., 1.);\n"
"   return vec4(col, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_PLANETS
/**
* Pixel Shader from shadertoy.com

***/
static const char* PS_PLANETS =
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

// 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

"#define MAX_DEPTH 10.0;\n"
"#define START_PLANET 18.0;\n"

"vec2 Rotate(vec2 pos, float angle) {\n"
"	return vec2(\n"
"		pos.x * cos(angle) - pos.y * sin(angle),\n"
"		pos.x * sin(angle) + pos.y * cos(angle)\n"
"		);\n"
"}\n"

"float Cubic(float a, float b, float t) {\n"
"	float blendFactor = t * t * (3.0 - 2.0 * t);\n"
"	return mix(a, b, blendFactor);\n"
"}\n"

"vec3 HueToRgb(float h) {\n"
"	vec3 rgb = 2.0 - abs(6.0 * h - vec3(3, 2, 4));\n"
"	rgb.r = 1.0 - rgb.r;\n"
"	return clamp(rgb, 0.0, 1.0);\n"
"}\n"

"vec3 HsvToRgb(vec3 hsv) {\n"
"	vec3 rgb = HueToRgb(hsv.x);\n"
"	return ((rgb - 1.0) * hsv.y + 1.0) * hsv.z;\n"
"}\n"

// Gradient noise functions courtesy Inigo Q
"vec3 hash(vec3 p)\n"
"{\n"
"	p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),\n"
"		dot(p, vec3(269.5, 183.3, 246.1)),\n"
"		dot(p, vec3(113.5, 271.9, 124.6)));\n"

"	return -1.0 + 2.0*fract(sin(p)*43758.5453123);\n"
"}\n"

"float noise(in vec3 p)\n"
"{\n"
"	vec3 i = floor(p);\n"
"	vec3 f = fract(p);\n"

"	vec3 u = f*f*(3.0 - 2.0*f);\n"

"	return mix(mix(mix(dot(hash(i + vec3(0.0, 0.0, 0.0)), f - vec3(0.0, 0.0, 0.0)),\n"
"		dot(hash(i + vec3(1.0, 0.0, 0.0)), f - vec3(1.0, 0.0, 0.0)), u.x),\n"
"		mix(dot(hash(i + vec3(0.0, 1.0, 0.0)), f - vec3(0.0, 1.0, 0.0)),\n"
"		dot(hash(i + vec3(1.0, 1.0, 0.0)), f - vec3(1.0, 1.0, 0.0)), u.x), u.y),\n"
"		mix(mix(dot(hash(i + vec3(0.0, 0.0, 1.0)), f - vec3(0.0, 0.0, 1.0)),\n"
"		dot(hash(i + vec3(1.0, 0.0, 1.0)), f - vec3(1.0, 0.0, 1.0)), u.x),\n"
"		mix(dot(hash(i + vec3(0.0, 1.0, 1.0)), f - vec3(0.0, 1.0, 1.0)),\n"
"		dot(hash(i + vec3(1.0, 1.0, 1.0)), f - vec3(1.0, 1.0, 1.0)), u.x), u.y), u.z);\n"
"}\n"

"float Rand(vec2 pos) {\n"
"	return fract(sin(dot(pos.xy, vec2(12.9898, 78.233))) * 43758.5453);\n"
"}\n"

"float Rand(float pos) {\n"
"	return Rand(vec2(pos, pos));\n"
"}\n"

"float SphereDistance(vec3 localPos, float radius) {\n"
"	return length(localPos) - radius;\n"
"}\n"

"float SceneDistance(vec3 pos, out float layer) {\n"
"	if (pos.x < 0.0)\n"
"	{\n"
"		return 1.0;\n"
"	}\n"

"	float planetNumber = floor((pos.x) / 2.0) + START_PLANET;\n"

"	vec3 planetPos = pos;\n"
"	planetPos.x = mod(planetPos.x, 2.0) - 1.0;\n"
"	float rotationSpeed = mix(-1.0, 1.0, fract(planetNumber / 2.3));\n"
"	planetPos.xz = Rotate(planetPos.xz, rotationSpeed * fGlobalTime);\n"

"	float terrainDetail = mix(0.1, 5.5, Rand(planetNumber + 0.16));\n"
"	float layerHeight = mix(0.005, 0.05, Rand(planetNumber + 0.55));\n"
"	float layerCount = floor(mix(3.5, 20.5, Rand(planetNumber + 0.36)));\n"
"	float noiseValue = 0.5 * noise((normalize(planetPos) + planetNumber) * terrainDetail) + 0.5;\n"
"	layer = floor(noiseValue * layerCount);\n"

"	float baseSize = mix(0.2, 0.4, Rand(planetNumber + 0.28));\n"
"	float d1 = SphereDistance(planetPos, baseSize + (layer - 1.0) * layerHeight);\n"
"	float d2 = SphereDistance(planetPos, baseSize + layer * layerHeight);\n"

"	float layerTransition = clamp(fract(noiseValue * layerCount) * 15.0, 0.0, 1.0);\n"
"	return mix(d1, d2, layerTransition);\n"
"}\n"

"float SceneDistance(vec3 pos) {\n"
"	float dummy;\n"
"	return SceneDistance(pos, dummy);\n"
"}\n"

"float RayMarch(vec3 startPos, vec3 dir) {\n"
"	float max_depth = 10.0;\n"
"	float depth = 0.0;\n"
"	for (int i = 0; i < 255; i++)\n"
"	{\n"
"		vec3 pos = startPos + dir * depth;\n"
"		float dist = SceneDistance(pos);\n"
"		if (dist < 0.0001)\n"
"		{\n"
"			return depth;\n"
"		}\n"
"		depth += 0.6 * dist;\n"
"		if (depth >= max_depth)\n"
"		{\n"
"			return max_depth;\n"
"		}\n"
"	}\n"
"	return max_depth;\n"
"}\n"

"vec3 SceneNormal(vec3 pos) {\n"
"	const float DX = 0.001;\n"
"	const vec3 dx = vec3(DX, 0.0, 0.0);\n"
"	const vec3 dy = vec3(0.0, DX, 0.0);\n"
"	const vec3 dz = vec3(0.0, 0.0, DX);\n"
"	return normalize(vec3(\n"
"		SceneDistance(pos + dx) - SceneDistance(pos - dx),\n"
"		SceneDistance(pos + dy) - SceneDistance(pos - dy),\n"
"		SceneDistance(pos + dz) - SceneDistance(pos - dz)\n"
"		));\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"vec4 fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"vec2 fragCoord = (Input.vTexcoord - 0.4f) * sResolution * 3.0f; \n"

"const float FOV = radians(45.0);\n"
"vec3 eyePos = vec3(0.5 * fGlobalTime - 1.0, 0.0, -2.0);\n"
"vec2 xy = (2.0 * fragCoord - sResolution.xy) * 0.5;\n"

"vec3 rayDir = normalize(vec3(xy, 1.0 / tan(0.5 * FOV) * 0.5 * sResolution.y));\n"
"vec3 lightDir = normalize(vec3(0.5, 0.8, -1.0));\n"

"float depth = RayMarch(eyePos, rayDir);\n"
"if (depth < 10.0)\n"
"{\n"

"	vec3 pos = eyePos + rayDir * depth;\n"
"	float layer;\n"
"	vec3 normal = SceneNormal(pos);\n"
"	SceneDistance(pos, layer);\n"

"	float planetNumber = floor((pos.x) / 2.0) + START_PLANET;\n"

"	float baseHue = Rand(planetNumber + 1.72);\n"
"	float hueStep = mix(0.02, 0.15, pow(Rand(planetNumber + 0.492), 2.0));\n"

"	float baseSat = pow(Rand(planetNumber + 0.195), 0.2);\n"
"	float satStep = mix(-0.2, 0.2, Rand(planetNumber + 0.777));\n"

"	float baseVal = mix(0.5, 1.0, pow(Rand(planetNumber + 0.888), 0.3));\n"
"	float valStep = mix(0.0, 0.2, Rand(planetNumber + 0.992));\n"

"	vec3 color = HsvToRgb(\n"
"		vec3(fract(baseHue + layer * hueStep),\n"
"		clamp(baseSat + layer * satStep, 0.0, 1.0),\n"
"		0.3 + 0.7 * fract(baseVal + layer * valStep)));\n"
"	float diffuse = 2.0 * clamp(dot(lightDir, normal), 0.0, 1.0);\n"
"	fragColor = vec4(diffuse * color, 1.0) * (1.0 - depth / 10.0);\n"
"}\n"
"else\n"
"{\n"
"	float gradient = abs(2.0 * (fragCoord.y / sResolution.y) - 1.0);\n"
"	vec3 skyColor = HsvToRgb(vec3(fract(0.015 * fGlobalTime) + 0.5, 1.0, 0.1 * gradient));\n"
"	fragColor = vec4(skyColor, 1.0);\n"
"}\n"

"   return fragColor;\n"
"}\n";
#pragma endregion
#pragma region PS_VORONOI_SMOOTH
/**
* Pixel Shader from shadertoy.com
* Created by inigo quilez - iq/2014
* License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

* Smooth Voronoi - avoiding aliasing, by replacing the usual min() function, which is
* discontinuous, with a smooth version. That can help preventing some aliasing, and also
* provides with more artistic control of the final procedural textures/models.

* The parameter w controls the smoothness
***/
static const char* PS_VORONOI_SMOOTH =
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

// 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

"float hash1(float n) { return fract(sin(n)*43758.5453); }\n"
"vec2  hash2(vec2  p) { p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3))); return fract(sin(p)*43758.5453); }\n"

"vec4 voronoi(in vec2 x, float w)\n"
"{\n"
"	vec2 n = floor(x);\n"
"	vec2 f = fract(x);\n"

"	vec4 m = vec4(8.0, 0.0, 0.0, 0.0);\n"
"	for (int j = -2; j <= 2; j++)\n"
"		for (int i = -2; i <= 2; i++)\n"
"		{\n"
"			vec2 g = vec2(float(i), float(j));\n"
"			vec2 o = hash2(n + g);\n"

// animate
"			o = 0.5 + 0.5*sin(fGlobalTime + 6.2831*o);\n"

// distance to cell		
"			float d = length(g - f + o);\n"

// do the smoth min for colors and distances		
"			vec3 col = 0.5 + 0.5*sin(hash1(dot(n + g, vec2(7.0, 113.0)))*2.5 + 3.5 + vec3(2.0, 3.0, 0.0));\n"
"			float h = smoothstep(0.0, 1.0, 0.5 + 0.5*(m.x - d) / w);\n"

"			m.x = mix(m.x, d, h) - h*(1.0 - h)*w / (1.0 + 3.0*w);\n" // distance
"			m.yzw = mix(m.yzw, col, h) - h*(1.0 - h)*w / (1.0 + 3.0*w);\n" // color
"		}\n"

"	return m;\n"
"}\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

// "	vec2 p = fragCoord.xy / sResolution.yy;\n" <-- original code
"	vec2 p = -8.0 + 16.0 * Input.vTexcoord;\n"

"	float k = 2.0 + 70.0 * pow(0.5 + 0.5*sin(0.25*6.2831*fGlobalTime), 4.0);\n"
"	k = 0.5 - 0.5*cos(0.25*6.2831*fGlobalTime);\n"
"	vec4 c = voronoi(6.0*p, k);\n"

"	vec3 col = c.yzw;\n"

"	col *= 1.0 - 0.8*c.x*step(p.y, 0.33);\n"
"	col *= mix(c.x, 1.0, step(p.y, 0.66));\n"

"	col *= smoothstep(0.005, 0.007, abs(p.y - 0.33));\n"
"	col *= smoothstep(0.005, 0.007, abs(p.y - 0.66));\n"

"   return vec4(col, 1.0);\n"
"}\n";
#pragma endregion
#pragma region PS_TEXT_FX_001
/**
* Text FX 001
* FX shader for font output.
***/
static const char* PS_TEXT_FX_001 =
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

// 

"#define vec2 float2\n"
"#define vec3 float3\n"
"#define vec4 float4\n"
"#define mat2 float2x2\n"
"#define mix lerp\n"
"#define fract frac\n"
"#define mod fmod\n"
"#define fract frac\n"

// pixel shader
"float4 PS(PS_INPUT Input) : SV_TARGET\n"
"{\n"

"	const float PI = 3.14159265;\n"

// get texel, discard if no color (in case red)
"	float4 afFragment4 = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);\n"
"	afFragment4.a = afFragment4.r;\n"
"	if (afFragment4.r == 0.0f) discard;\n"

// moire
"	vec4 sPos4 = mul(vec4(1.0f, 1.0f, 1.0f, 1.0f), sWorld);\n"
"	vec2 sPos = sPos4.xy;// Input.vTexcoord * 7.0f;\n"
"	sPos.x += Input.vTexcoord.y * 7;\n"
"	sPos.y += Input.vTexcoord.x * 7;\n"
"	vec3 sColor = sin(vec3(sin(fGlobalTime / 2.3f), cos(fGlobalTime / 2.3f), fract(fGlobalTime))*(dot(sPos, sPos)));\n"
"	if (afFragment4.a < 1.0f) sColor = vec3(0.0f, 0.0f, 0.0f);\n"

"	afFragment4.rgb = smoothstep(sColor, afFragment4.rgb, 0.9f) * 0.5f;\n"
"	afFragment4.g *= sin(fGlobalTime);\n"
"	afFragment4.b *= cos(fGlobalTime);\n"
"	if (afFragment4.a < 1.0f) afFragment4.r = 0.0f;\n"

"	return afFragment4;\n"

"}\n";
#pragma endregion
#pragma endregion

/**
* Simple enumeration of available vertex shaders.
***/
enum VertexShaderTechnique
{
	PosUV2D,        /**< Position, UV, for 2D projection **/
	PosNormUV,      /**< Position, Normal, UV. **/
	PosNormUV_Text, /**< Position, Normal, UV -> Converts UV to text glyph UV. ***/
};

/**
* Simple enumeration of available pixel shaders.
***/
enum PixelShaderTechnique
{
	FullscreenSimple,              /**< TexturedVertex **/
	WarpSimple,                    /**< TexturedVertex **/
	DistortSimple,                 /**< TexturedVertex **/
	FullscreenGammaCorrection,     /**< TexturedVertex **/
	GeometryDiffuseTextured,       /**< TexturedNormalVertex : simple lighting **/
	GeometryDiffuseTexturedMouse,  /**< TexturedNormalVertex : simple lighting, gamma correction, draws mouse laser pointer **/
	Fabric,                        /**< TexturedNormalVertex : fabric effect **/
	BumpComputeNormal,             /**< TexturedNormalVertex : bump mapping, computes per pixel normals **/
	StringTheory,                  /**< TexturedNormalVertex : "String Theory" effect from shadertoy.com **/
	Bubbles,                       /**< TexturedNormalVertex : "Bubbles!" effect from shadertoy.com **/
	C64Plasma,                     /**< TexturedNormalVertex : "C64 plasma" effect from shadertoy.com **/
	ToonCloud,                     /**< TexturedNormalVertex : "Toon Cloud" effect from shadertoy.com **/
	Worley01,                      /**< TexturedNormalVertex : "Worley Algorithm (Cell Noise )" effect from shadertoy.com **/
	WaterCaustic,                  /**< TexturedNormalVertex : "Tileable Water Caustic" effect from shadertoy.com **/
	HypnoticDisco,                 /**< TexturedNormalVertex : "Hypnotic Disco" effect from shadertoy.com **/
	Planets,                       /**< TexturedNormalVertex : "Planets" effect from shadertoy.com **/
	VoronoiSmooth,                 /**< TexturedNormalVertex : "Voronoi smooth" effect from shadertoy.com **/
	TextFX_001,                    /**< TextFX 001 : by Denis **/
};

/**
* Glyph constant buffer structure.
***/
struct Glyph_Constants
{
	uint32_t unCharacter;
	float fULeft;
	float fVTop;
	float fURight;
	float fVBottom;
	float fXOffset;
	float fYOffset;
	float fXAdvance;
};

/**
* Main vertex structure.
***/
struct VertexPosUV
{
	float3 afPos3;
	float3 afNormal3;
	float2 afUV2;
};

// method definitions
HRESULT CreateVertexShaderTechnique(ID3D11Device* pcDevice, ID3D11VertexShader** ppcVertexShader, ID3D11InputLayout** ppcInputLayout, VertexShaderTechnique eTechnique);
HRESULT CreatePixelShaderEffect(ID3D11Device* pcDevice, ID3D11PixelShader** ppcPixelShader, PixelShaderTechnique eTechnique);

/**
* Vireio font class.
* Does load .spritefont files and renders the chosen font glyphes.
***/
class VireioFont
{
public:
	/**
	* Constructor.
	* Loads the specified .spritefont file.
	* @param pcDevice The D3D 11 device.
	* @param pcDeviceContext The D3D 11 device context.
	* @param szPath File path to the .spritefont file.
	***/
	VireioFont(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, LPCSTR szPath, float fFontSize, float fAspect, HRESULT& nHr)
		: m_pcTexFont2D(nullptr)
		, m_pcTexFontSRV(nullptr)
		, m_asGlyphConstants(NULL)
		, m_pcConstantsGlyph(nullptr)
		, m_pcVBGlyphes(nullptr)
		, m_pcBlendState(nullptr)
	{
		static const char s_szSpriteFontMagic[] = "DXTKfont";
		/**
		* Microsoft description for a single character glyph.
		***/
		struct Glyph_MS
		{
			uint32_t Character;
			RECT Subrect;
			float XOffset;
			float YOffset;
			float XAdvance;
		};

		// open file
		std::ifstream cInFileStream;
		cInFileStream.open(szPath, std::ios::in | std::ios::binary);
		if (cInFileStream.is_open())
		{
			// Validate the header.
			char ch = 0;
			for (char const* magic = s_szSpriteFontMagic; *magic; magic++)
			{
				cInFileStream.read(&ch, sizeof(char));
				if (ch != *magic)
				{
					OutputDebugStringA("[VRO] SpriteFont provided with an invalid .spritefont file.\n");
					throw std::exception("Not a MakeSpriteFont output binary");
				}
			}

			// Read the glyph count.
			uint32_t unGlyphCount = 0;
			std::vector<Glyph_MS> asGlyphes = std::vector<Glyph_MS>();
			cInFileStream.read((char*)&unGlyphCount, sizeof(uint32_t));

			// Loop through glyphes, read and assign
			for (uint32_t unGlyphIx = 0; unGlyphIx < unGlyphCount; unGlyphIx++)
			{
				Glyph_MS sGlyph = {};
				cInFileStream.read((char*)&sGlyph, sizeof(Glyph_MS));
				asGlyphes.push_back(sGlyph);
			}

			// Read font properties.
			float fLineSpacing = 0;
			cInFileStream.read((char*)&fLineSpacing, sizeof(float));
			uint32_t unDefaultChar = 0;
			cInFileStream.read((char*)&unDefaultChar, sizeof(uint32_t));

			// Read the texture data.
			uint32_t unTextureWidth = 0;
			cInFileStream.read((char*)&unTextureWidth, sizeof(uint32_t));
			uint32_t unTextureHeight = 0;
			cInFileStream.read((char*)&unTextureHeight, sizeof(uint32_t));
			DXGI_FORMAT eTextureFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			cInFileStream.read((char*)&eTextureFormat, sizeof(DXGI_FORMAT));
			uint32_t unTextureStride = 0;
			cInFileStream.read((char*)&unTextureStride, sizeof(uint32_t));
			uint32_t unTextureRows = 0;
			cInFileStream.read((char*)&unTextureRows, sizeof(uint32_t));
			uint8_t* pchTextureData = new uint8_t[unTextureStride * unTextureRows];
			cInFileStream.read((char*)pchTextureData, sizeof(uint8_t)* unTextureStride * unTextureRows);

			// Create the D3D texture.
			CD3D11_TEXTURE2D_DESC sDescTex(eTextureFormat, unTextureWidth, unTextureHeight, 1, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
			CD3D11_SHADER_RESOURCE_VIEW_DESC sDescView(D3D11_SRV_DIMENSION_TEXTURE2D, eTextureFormat);
			D3D11_SUBRESOURCE_DATA sInitData = { pchTextureData, unTextureStride };

			nHr = pcDevice->CreateTexture2D(&sDescTex, &sInitData, &m_pcTexFont2D);
			if (FAILED(nHr)) return;
			nHr = pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTexFont2D, &sDescView, &m_pcTexFontSRV);
			if (FAILED(nHr)) return;

			// convert to glyph constants, first create constant array
			Glyph_Constants sGC = {};
			m_asGlyphConstants = std::vector<Glyph_Constants>(256, sGC);

			// and create a vertex buffer for all glyphes, first create vertex array
			VertexPosUV asVerticesGlyph[256 * 4] = {};

			// loop through the available glyphes
			float fFontSize = 128.0f;
			float fSpace = 0.02f;
			for (size_t unI = 0; unI < asGlyphes.size(); unI++)
			{
				// set glyph data
				uint32_t unAscii = asGlyphes[unI].Character;
				if (unAscii > 255)
				{
					OutputDebugStringA("[VRO] Invalid ascii code.");
					continue;
				}
				m_asGlyphConstants[unAscii].unCharacter = unAscii;
				m_asGlyphConstants[unAscii].fULeft = (float)asGlyphes[unI].Subrect.left / (float)unTextureWidth;
				m_asGlyphConstants[unAscii].fURight = (float)asGlyphes[unI].Subrect.right / (float)unTextureWidth;
				m_asGlyphConstants[unAscii].fVTop = (float)asGlyphes[unI].Subrect.top / (float)unTextureHeight;
				m_asGlyphConstants[unAscii].fVBottom = (float)asGlyphes[unI].Subrect.bottom / (float)unTextureHeight;
				m_asGlyphConstants[unAscii].fXOffset = asGlyphes[unI].XOffset;
				m_asGlyphConstants[unAscii].fYOffset = asGlyphes[unI].YOffset;

				// set x advance by glyph width
				float fGlyphWidth = ((float)asGlyphes[unI].Subrect.right - (float)asGlyphes[unI].Subrect.left) / fFontSize;
				float fGlyphHeight = ((float)asGlyphes[unI].Subrect.bottom - (float)asGlyphes[unI].Subrect.top) / fFontSize;
				float fGlyphOffset = (fFontSize - asGlyphes[unI].YOffset) / fFontSize;
				m_asGlyphConstants[unAscii].fXAdvance = -(fGlyphWidth + fSpace);

				// set index data
				uint32_t unIx = unAscii * 4;

				// 0
				asVerticesGlyph[unIx].afPos3 = XMFLOAT3(0.0f, 0.0f, -fGlyphOffset + fGlyphHeight);
				asVerticesGlyph[unIx].afUV2 = XMFLOAT2(1.0f, 1.0f);

				// 1
				asVerticesGlyph[unIx + 1].afPos3 = XMFLOAT3(fGlyphWidth, 0.0f, -fGlyphOffset + fGlyphHeight);
				asVerticesGlyph[unIx + 1].afUV2 = XMFLOAT2(0.0f, 1.0f);

				// 2
				asVerticesGlyph[unIx + 2].afPos3 = XMFLOAT3(0.0f, 0.0f, -fGlyphOffset);
				asVerticesGlyph[unIx + 2].afUV2 = XMFLOAT2(1.0f, 0.0f);

				// 3
				asVerticesGlyph[unIx + 3].afPos3 = XMFLOAT3(fGlyphWidth, 0.0f, -fGlyphOffset);
				asVerticesGlyph[unIx + 3].afUV2 = XMFLOAT2(0.0f, 0.0f);

			}

			// set space x advance manually
			m_asGlyphConstants[' '].fXAdvance = -fSpace * 10.0f;

			// create the glyph constant buffer
			D3D11_BUFFER_DESC sDescConst;
			ZeroMemory(&sDescConst, sizeof(D3D11_BUFFER_DESC));
			sDescConst.ByteWidth = sizeof(Glyph_Constants);
			sDescConst.Usage = D3D11_USAGE_DEFAULT;
			sDescConst.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			sDescConst.MiscFlags = 0;
			sDescConst.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA sInitDataConst;
			ZeroMemory(&sInitDataConst, sizeof(D3D11_SUBRESOURCE_DATA));
			sInitDataConst.pSysMem = &m_asGlyphConstants['A'];
			sInitDataConst.SysMemPitch = 0;
			sInitDataConst.SysMemSlicePitch = 0;

			// Create the buffer.
			HRESULT nHr = pcDevice->CreateBuffer(&sDescConst, &sInitDataConst, &m_pcConstantsGlyph);

			// create the glyph vertex buffer
			D3D11_BUFFER_DESC sDescVtx;
			ZeroMemory(&sDescVtx, sizeof(D3D11_BUFFER_DESC));
			sDescVtx.Usage = D3D11_USAGE_IMMUTABLE;
			sDescVtx.ByteWidth = sizeof(VertexPosUV)* 256 * 4;
			sDescVtx.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			sDescVtx.CPUAccessFlags = 0;
			sDescVtx.MiscFlags = 0;
			sDescVtx.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA sInitDataVtx;
			ZeroMemory(&sInitDataVtx, sizeof(D3D11_SUBRESOURCE_DATA));
			sInitDataVtx.pSysMem = asVerticesGlyph;
			if (FAILED(nHr = pcDevice->CreateBuffer(&sDescVtx, &sInitDataVtx, &m_pcVBGlyphes)))
			{
				OutputDebugStringA("[VRO] Failed to create buffer !");
				return;
			}
		}

		// set first matrices
		D3DXMatrixIdentity(&m_sWorld);
		D3DXMatrixIdentity(&m_sView);

		// we use a simple left handed projection matrix for text...
		const float fPi = 3.1415926535f;
		D3DXMatrixPerspectiveFovLH(&m_sProj, 0.25f*fPi, fAspect, 1.0f, 1000.0f);

		// Set constants
		ZeroMemory(&m_sConstantBuffer0, sizeof(GeometryConstantBuffer));
		m_sConstantBuffer0.sWorldViewProjection = m_sWorld*m_sView*m_sProj;

		// create the constant buffer
		D3D11_BUFFER_DESC sDescConst;
		ZeroMemory(&sDescConst, sizeof(D3D11_BUFFER_DESC));
		sDescConst.ByteWidth = sizeof(GeometryConstantBuffer);
		sDescConst.Usage = D3D11_USAGE_DEFAULT;
		sDescConst.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		sDescConst.MiscFlags = 0;
		sDescConst.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA sInitDataConst;
		ZeroMemory(&sInitDataConst, sizeof(D3D11_SUBRESOURCE_DATA));
		sInitDataConst.pSysMem = &m_sConstantBuffer0;
		sInitDataConst.SysMemPitch = 0;
		sInitDataConst.SysMemSlicePitch = 0;

		// Create the buffer.
		nHr = pcDevice->CreateBuffer(&sDescConst, &sInitDataConst, &m_pcConstantBuffer0);

		// create a blend state for alpha blending
		D3D11_BLEND_DESC sBlendDesc;
		ZeroMemory(&sBlendDesc, sizeof(D3D11_BLEND_DESC));

		sBlendDesc.RenderTarget[0].BlendEnable = TRUE;
		sBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		sBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		sBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		pcDevice->CreateBlendState(&sBlendDesc, &m_pcBlendState);

		// create constant shader constants..
		m_sConstantBuffer0.sLightDir = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		m_sConstantBuffer0.sLightAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_sConstantBuffer0.sLightDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_sConstantBuffer0.fGamma = 1.0f;

		// for aspect ratio based fx we set a 1.0 ratio here
		m_sConstantBuffer0.sResolution.x = 1024.0f;
		m_sConstantBuffer0.sResolution.y = 1024.0f;

		// create vertex shader
		if (FAILED(nHr = CreateVertexShaderTechnique(pcDevice, &m_pcVertexShader, &m_pcInputLayout, VertexShaderTechnique::PosNormUV_Text)))
		{
			OutputDebugString(L"[OVR] Failed to create vertex shader. ");
			return;
		}

		// create pixel shader... 
		if (FAILED(nHr = CreatePixelShaderEffect(pcDevice, &m_pcPixelShader, PixelShaderTechnique::TextFX_001)))
		{
			OutputDebugString(L"[OVR] Failed to create pixel shader. ");
			return;
		}

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(pcDevice->CreateSamplerState(&sampDesc, &m_pcSampler)))
			OutputDebugString(L"[VRO] Failed to create sampler.");
	}

	/**
	* Destructor.
	***/
	~VireioFont()
	{
		SAFE_RELEASE(m_pcSampler);
		SAFE_RELEASE(m_pcVertexShader);
		SAFE_RELEASE(m_pcPixelShader);
		SAFE_RELEASE(m_pcInputLayout);
		SAFE_RELEASE(m_pcTexFont2D);
		SAFE_RELEASE(m_pcTexFontSRV);
		SAFE_RELEASE(m_pcConstantsGlyph);
		SAFE_RELEASE(m_pcVBGlyphes);
		SAFE_RELEASE(m_pcConstantBuffer0);
		SAFE_RELEASE(m_pcBlendState);
	}

	/**
	* Set attributes for the text.
	***/
	void SetTextAttributes(float fX, float fY, float fZ, float fTime)
	{
		// Build the view matrix.
		D3DXVECTOR3 sPos = D3DXVECTOR3(fX, fY, fZ);
		D3DXVECTOR3 sTarget = D3DXVECTOR3();
		D3DXVECTOR3 sUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXMatrixLookAtLH(&m_sView, &sPos, &sTarget, &sUp);

		// update constants
		m_sConstantBuffer0.fGlobalTime = fTime;
	}

	/**
	* Sets all fields before the render calls.
	***/
	void ToRender(ID3D11DeviceContext* pcContext, float fTime)
	{
		UINT stride = sizeof(VertexPosUV);
		UINT offset = 0;

		// set glyph vertex buffer, constant buffers, font texture
		pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		pcContext->IASetVertexBuffers(0, 1, &m_pcVBGlyphes, &stride, &offset);
		pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBuffer0);
		pcContext->VSSetConstantBuffers(1, 1, &m_pcConstantsGlyph);
		pcContext->PSSetConstantBuffers(0, 1, &m_pcConstantBuffer0);
		pcContext->PSSetShaderResources(0, 1, &m_pcTexFontSRV);

		// set shaders
		pcContext->VSSetShader(m_pcVertexShader, nullptr, NULL);
		pcContext->IASetInputLayout((ID3D11InputLayout*)m_pcInputLayout);
		pcContext->PSSetShader(m_pcPixelShader, nullptr, NULL);

		// Set the sampler
		pcContext->PSSetSamplers(0, 1, &m_pcSampler);

		// set buffer fields
		m_sConstantBuffer0.fGlobalTime = fTime;
	}

	/**
	* Draw text on active render target.
	***/
	void RenderText(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, LPCSTR szText, float fX, float fY, float fZ)
	{
		// get old blend state
		ID3D11BlendState* pcBlendStateOld = nullptr;
		FLOAT afBlendFactor[4] = {};
		UINT unMask = 0;
		pcContext->OMGetBlendState(&pcBlendStateOld, afBlendFactor, &unMask);
		pcContext->OMSetBlendState(m_pcBlendState, 0, 0xffffffff);

		UINT unIx = 0;
		float fXTranslate = fX;
		while (szText[unIx])
		{
			char ch = szText[unIx];

			// update matrices
			fXTranslate += m_asGlyphConstants[ch].fXAdvance;
			D3DXMATRIX world, view, proj;
			D3DXMatrixTranslation(&world, fXTranslate, fY, fZ);
			D3DXMatrixTranspose(&m_sConstantBuffer0.sWorld, &world);
			D3DXMATRIX wvp;
			wvp = world*m_sView*m_sProj;
			D3DXMatrixTranspose(&m_sConstantBuffer0.sWorldViewProjection, &wvp);

			// update constant buffers
			pcContext->UpdateSubresource(m_pcConstantBuffer0, 0, 0, &m_sConstantBuffer0, 0, 0);
			pcContext->UpdateSubresource(m_pcConstantsGlyph, 0, 0, &m_asGlyphConstants[ch], 0, 0);

			pcContext->Draw(4, (UINT)ch * 4);

			unIx++;
		}

		// set old blend state, release
		pcContext->OMSetBlendState(pcBlendStateOld, afBlendFactor, unMask);
		SAFE_RELEASE(pcBlendStateOld);
	}

private:
	/**
	* Font texture.
	***/
	ID3D11Texture2D* m_pcTexFont2D;
	/**
	* Font texture shader resource view.
	***/
	ID3D11ShaderResourceView* m_pcTexFontSRV;
	/**
	* Glyph constants array.
	* Array size 256 for every ascii code.
	***/
	std::vector<Glyph_Constants> m_asGlyphConstants;
	/**
	* Glyph constant buffer.
	***/
	ID3D11Buffer* m_pcConstantsGlyph;
	/**
	* Vertex buffer containing all glyph vertices.
	***/
	ID3D11Buffer* m_pcVBGlyphes;
	/**
	* Main constant structure.
	***/
	GeometryConstantBuffer m_sConstantBuffer0;
	/**
	* Main constant buffer.
	***/
	ID3D11Buffer* m_pcConstantBuffer0;
	/**
	* Basic world, view and projection matrices.
	***/
	D3DXMATRIX m_sWorld, m_sView, m_sProj;
	/**
	* Alpha blending state.
	***/
	ID3D11BlendState* m_pcBlendState;
	/**
	* Font vertex shader.
	***/
	ID3D11VertexShader* m_pcVertexShader;
	/**
	* Font vertex layout.
	***/
	ID3D11InputLayout* m_pcInputLayout;
	/**
	* Font pixel shader.
	***/
	ID3D11PixelShader* m_pcPixelShader;
	/**
	* The d3d11 sampler.
	***/
	ID3D11SamplerState* m_pcSampler;
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
* Creates a simple 3D vertex shader with an input layout.
***/
HRESULT CreateVertexShaderTechnique(ID3D11Device* pcDevice, ID3D11VertexShader** ppcVertexShader, ID3D11InputLayout** ppcInputLayout, VertexShaderTechnique eTechnique)
{
	if ((!ppcVertexShader) || (!ppcInputLayout) || (!pcDevice)) return E_INVALIDARG;

	ID3D10Blob* pcShader;
	HRESULT hr;

	// compile shader
	switch (eTechnique)
	{
		case PosUV2D:
			hr = D3DX10CompileFromMemory(VS2D, strlen(VS2D), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case PosNormUV:
			hr = D3DX10CompileFromMemory(VS3D, strlen(VS3D), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case PosNormUV_Text:
			hr = D3DX10CompileFromMemory(VS3D_TEXT, strlen(VS3D_TEXT), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		default:
			break;
	}


	// create shader
	if (SUCCEEDED(hr))
	{
		//#ifdef _DEBUG
		OutputDebugString(L"[VRO] : Vertex Shader compiled !");
		//#endif
		hr = pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, ppcVertexShader);

		if (FAILED(hr))
		{
			OutputDebugString(L"[VRO] : Failed to create Vertex Shader !");
			return hr;
		}
		else
		{
			// Define the input layouts
			D3D11_INPUT_ELEMENT_DESC aLayout01[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			D3D11_INPUT_ELEMENT_DESC aLayout02[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			if (eTechnique == VertexShaderTechnique::PosUV2D)
			{
				UINT unNumElements = sizeof(aLayout01) / sizeof(aLayout01[0]);
				hr = pcDevice->CreateInputLayout(aLayout01, unNumElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), ppcInputLayout);
			}
			else
			{
				UINT unNumElements = sizeof(aLayout02) / sizeof(aLayout02[0]);
				hr = pcDevice->CreateInputLayout(aLayout02, unNumElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), ppcInputLayout);
			}

			if (FAILED(hr))
			{
				OutputDebugString(L"[VRO] Failed to create input layout !");
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
		case GeometryDiffuseTexturedMouse:
			hr = D3DX10CompileFromMemory(PS3D_MOUSE, strlen(PS3D_MOUSE), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
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
		case Bubbles:
			hr = D3DX10CompileFromMemory(PS_BUBBLES, strlen(PS_BUBBLES), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case C64Plasma:
			hr = D3DX10CompileFromMemory(PS_C64_PLASMA, strlen(PS_C64_PLASMA), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case ToonCloud:
			hr = D3DX10CompileFromMemory(PS_TOON_CLOUD, strlen(PS_TOON_CLOUD), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case Worley01:
			hr = D3DX10CompileFromMemory(PS_WORLEY_01, strlen(PS_WORLEY_01), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case WaterCaustic:
			hr = D3DX10CompileFromMemory(PS_WATER_CAUSTIC, strlen(PS_WATER_CAUSTIC), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case HypnoticDisco:
			hr = D3DX10CompileFromMemory(PS_HYPNOTIC_DISCO, strlen(PS_HYPNOTIC_DISCO), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case Planets:
			hr = D3DX10CompileFromMemory(PS_PLANETS, strlen(PS_PLANETS), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case VoronoiSmooth:
			hr = D3DX10CompileFromMemory(PS_VORONOI_SMOOTH, strlen(PS_VORONOI_SMOOTH), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		case TextFX_001:
			hr = D3DX10CompileFromMemory(PS_TEXT_FX_001, strlen(PS_TEXT_FX_001), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL);
			break;
		default:
			return E_INVALIDARG;
			break;
	}

	// succeded ?
	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"Vireio Perception : Pixel shader compiled !");
		pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, ppcPixelShader);
		pcShader->Release();

		return S_OK;
	}
	else
	{
		OutputDebugString(L"Vireio Perception : Failed to create pixel shader !");
		return hr;
	}
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




