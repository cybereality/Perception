/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift Direct Mode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.h> and
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

#include"AQU_Nodus.h"
#include"Resources.h"
#include<vector>
#include<map>
#include<sstream>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "Extras/OVR_Math.h"
#define OVR_D3D_VERSION 11
#define OVR_SAMPLE_APP_ID "958062084316416"
#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#include <OVR_Avatar.h>
#include <OVR_CAPI_D3D.h>
#include <OVR_Platform.h>

#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\..\Include\VireioMenu.h"

#define NUMBER_OF_COMMANDERS                            1
#define NUMBER_OF_DECOMMANDERS                         11


/***
* Oculus Avatar Vertex Shader.
* (c) by Oculus VR
* Shader Model 5.0 (/5_0)
* HLSL conversion by Denis Reischl
***/
static const char* VS_OCULUS_AVATAR =
"cbuffer ConstantsVS : register (b0)\n"
"{\n"
"	float3 viewPos;\n"
"	float4x4 world;\n"
"	float4x4 viewProj;\n"
"	float4x4 meshPose[64];\n"
"};\n"

"struct InputVS\n"
"{\n"
"	float3 position : POSITION;\n"
"	float3 normal : TEXCOORD0;\n"
"	float4 tangent : TEXCOORD1;\n"
"	float2 texCoord : TEXCOORD2;\n"
"	float4 poseIndices : TEXCOORD3;\n"
"	float4 poseWeights : TEXCOORD4;\n"
"};\n"

"struct OutputVS\n"
"{\n"
"	float4 position : SV_POSITION;\n"
"	float3 vertexWorldPos : POSITION0;\n"
"	float3 vertexViewDir : TEXCOORD0;\n"
"	float3 vertexObjPos : TEXCOORD1;\n"
"	float3 vertexNormal : TEXCOORD2;\n"
"	float3 vertexTangent : TEXCOORD3;\n"
"	float3 vertexBitangent : TEXCOORD4;\n"
"	float2 vertexUV : TEXCOORD5;\n"
"};\n"

"OutputVS main(InputVS sInput)\n"
"{\n"
"	float4 vertexPose;\n"
"	vertexPose = mul(float4(sInput.position, 1.0), meshPose[int(sInput.poseIndices[0])]) * sInput.poseWeights[0];\n"
"	vertexPose += mul(float4(sInput.position, 1.0), meshPose[int(sInput.poseIndices[1])]) * sInput.poseWeights[1];\n"
"	vertexPose += mul(float4(sInput.position, 1.0), meshPose[int(sInput.poseIndices[2])]) * sInput.poseWeights[2];\n"
"	vertexPose += mul(float4(sInput.position, 1.0), meshPose[int(sInput.poseIndices[3])]) * sInput.poseWeights[3];\n"

"	float4 normalPose;\n"
"	normalPose = mul(float4(sInput.normal, 1.0), meshPose[int(sInput.poseIndices[0])]) * sInput.poseWeights[0];\n"
"	normalPose += mul(float4(sInput.normal, 1.0), meshPose[int(sInput.poseIndices[1])]) * sInput.poseWeights[1];\n"
"	normalPose += mul(float4(sInput.normal, 1.0), meshPose[int(sInput.poseIndices[2])]) * sInput.poseWeights[2];\n"
"	normalPose += mul(float4(sInput.normal, 1.0), meshPose[int(sInput.poseIndices[3])]) * sInput.poseWeights[3];\n"
"	normalPose = normalize(normalPose);\n"

"	float4 tangentPose;\n"
"	tangentPose = mul(float4(sInput.tangent.xyz, 1.0), meshPose[int(sInput.poseIndices[0])]) * sInput.poseWeights[0];\n"
"	tangentPose += mul(float4(sInput.tangent.xyz, 1.0), meshPose[int(sInput.poseIndices[1])]) * sInput.poseWeights[1];\n"
"	tangentPose += mul(float4(sInput.tangent.xyz, 1.0), meshPose[int(sInput.poseIndices[2])]) * sInput.poseWeights[2];\n"
"	tangentPose += mul(float4(sInput.tangent.xyz, 1.0), meshPose[int(sInput.poseIndices[3])]) * sInput.poseWeights[3];\n"
"	tangentPose = normalize(tangentPose);\n"

"	OutputVS sOutput;\n"
"	sOutput.vertexWorldPos = mul(vertexPose, world).xyz;\n"
"	sOutput.position = mul(float4(sOutput.vertexWorldPos, 1.0), viewProj);\n"
"	sOutput.vertexViewDir = normalize(viewPos - sOutput.vertexWorldPos.xyz);\n"
"	sOutput.vertexObjPos = sOutput.position.xyz;\n"
"	sOutput.vertexNormal = mul(normalPose, world).xyz;\n"
"	sOutput.vertexTangent = mul(tangentPose, world).xyz;\n"
"	sOutput.vertexBitangent = normalize(cross(sOutput.vertexNormal, sOutput.vertexTangent) * sInput.tangent.w);\n"
"	sOutput.vertexUV = sInput.texCoord;\n"

"	return sOutput;\n"
"}\n";

/***
* Oculus Avatar Fragment Shader.
* (c) by Oculus VR
* Shader Model 5.0 (/5_0)
* HLSL conversion by Denis Reischl
***/
static const char* PS_OCULUS_AVATAR =
"Texture2D textureAvatar : register(t0);\n"
"SamplerState samplerAvatar : register(s0);\n"

"#define SAMPLE_MODE_COLOR 0\n"
"#define SAMPLE_MODE_TEXTURE 1\n"
"#define SAMPLE_MODE_TEXTURE_SINGLE_CHANNEL 2\n"
"#define SAMPLE_MODE_PARALLAX 3\n"
"#define SAMPLE_MODE_RSRM 4\n"

"#define MASK_TYPE_NONE 0\n"
"#define MASK_TYPE_POSITIONAL 1\n"
"#define MASK_TYPE_REFLECTION 2\n"
"#define MASK_TYPE_FRESNEL 3\n"
"#define MASK_TYPE_PULSE 4\n"

"#define BLEND_MODE_ADD 0\n"
"#define BLEND_MODE_MULTIPLY 1\n"

"#define MAX_LAYER_COUNT 8\n"

"struct OutputVS\n"
"{\n"
"	float4 position : SV_POSITION;\n"
"	float3 vertexWorldPos : POSITION0;\n"
"	float3 vertexViewDir : TEXCOORD0;\n"
"	float3 vertexObjPos : TEXCOORD1;\n"
"	float3 vertexNormal : TEXCOORD2;\n"
"	float3 vertexTangent : TEXCOORD3;\n"
"	float3 vertexBitangent : TEXCOORD4;\n"
"	float2 vertexUV : TEXCOORD5;\n"
"};\n"

"cbuffer FragmentVars : register (b0)\n"
"{\n"
"	float4 baseColor;\n"
"	int baseMaskType;\n"
"	float4 baseMaskParameters;\n"
"	float4 baseMaskAxis;\n"
"	Texture2D alphaMask;\n"
"	float4 alphaMaskScaleOffset;\n"
"	Texture2D normalMap;\n"
"	float4 normalMapScaleOffset;\n"
"	Texture2D parallaxMap;\n"
"	float4 parallaxMapScaleOffset;\n"
"	Texture2D roughnessMap;\n"
"	float4 roughnessMapScaleOffset;\n"

"	float4x4 projectorInv;\n"

"	bool useAlpha;\n"
"	bool useNormalMap;\n"
"	bool useRoughnessMap;\n"
"	bool useProjector;\n"
"	float elapsedSeconds;\n"

"	int layerCount;\n"

"	int layerSamplerModes[MAX_LAYER_COUNT];\n"
"	int layerBlendModes[MAX_LAYER_COUNT];\n"
"	int layerMaskTypes[MAX_LAYER_COUNT];\n"
"	float4 layerColors[MAX_LAYER_COUNT];\n"
"	Texture2D layerSurfaces[MAX_LAYER_COUNT];\n"
"	float4 layerSurfaceScaleOffsets[MAX_LAYER_COUNT];\n"
"	float4 layerSampleParameters[MAX_LAYER_COUNT];\n"
"	float4 layerMaskParameters[MAX_LAYER_COUNT];\n"
"	float4 layerMaskAxes[MAX_LAYER_COUNT];\n"
"};\n"

"float3 ComputeNormal(float3x3 tangentTransform, float3 worldNormal, float3 surfaceNormal, float surfaceStrength)\n"
"{\n"
"	if (useNormalMap)\n"
"	{\n"
"		float3 surface = lerp(float3(0.0, 0.0, 1.0), surfaceNormal, surfaceStrength);\n"
"		return normalize(mul(tangentTransform, surface));\n"
"	}\n"
"	else\n"
"	{\n"
"		return worldNormal;\n"
"	}\n"
"}\n"

"float3 ComputeColor(float3 vertexViewDir, int sampleMode, float2 uv, float4 color, Texture2D surface, float4 surfaceScaleOffset, float4 sampleParameters, float3x3 tangentTransform, float3 worldNormal, float3 surfaceNormal)\n"
"{\n"
"	if (sampleMode == SAMPLE_MODE_TEXTURE)\n"
"	{\n"
"		float2 panning = elapsedSeconds * sampleParameters.xy;\n"
"		return surface.Sample(samplerAvatar, (uv + panning) * surfaceScaleOffset.xy + surfaceScaleOffset.zw).rgb * color.rgb;\n"
"	}\n"
"	else if (sampleMode == SAMPLE_MODE_TEXTURE_SINGLE_CHANNEL)\n"
"	{\n"
"		float4 channelMask = sampleParameters;\n"
"		float4 channels = surface.Sample(samplerAvatar, uv * surfaceScaleOffset.xy + surfaceScaleOffset.zw);\n"
"		return dot(channelMask, channels) * color.rgb;\n"
"	}\n"
"	else if (sampleMode == SAMPLE_MODE_PARALLAX)\n"
"	{\n"
"		float parallaxMinHeight = sampleParameters.x;\n"
"		float parallaxMaxHeight = sampleParameters.y;\n"
"		float parallaxValue = parallaxMap.Sample(samplerAvatar, uv * parallaxMapScaleOffset.xy + parallaxMapScaleOffset.zw).r;\n"
"		float scaledHeight = lerp(parallaxMinHeight, parallaxMaxHeight, parallaxValue);\n"
"		float2 parallaxUV = mul(vertexViewDir, tangentTransform).xy * scaledHeight;\n"

"		return surface.Sample(samplerAvatar, (uv + parallaxUV) * surfaceScaleOffset.xy + surfaceScaleOffset.zw).rgb * color.rgb;\n"
"	}\n"
"	else if (sampleMode == SAMPLE_MODE_RSRM)\n"
"	{\n"
"		float roughnessMin = sampleParameters.x;\n"
"		float roughnessMax = sampleParameters.y;\n"

"		float scaledRoughness = roughnessMin;\n"
"		if (useRoughnessMap)\n"
"		{\n"
"			float roughnessValue = roughnessMap.Sample(samplerAvatar, uv * roughnessMapScaleOffset.xy + roughnessMapScaleOffset.zw).r;\n"
"			scaledRoughness = lerp(roughnessMin, roughnessMax, roughnessValue);\n"
"		}\n"

"		float normalMapStrength = sampleParameters.z;\n"
"		float3 viewReflect = reflect(-vertexViewDir, ComputeNormal(tangentTransform, worldNormal, surfaceNormal, normalMapStrength));\n"
"		float viewAngle = viewReflect.y * 0.5 + 0.5;\n"
"		return surface.Sample(samplerAvatar, float2(scaledRoughness, viewAngle)).rgb * color.rgb;\n"
"	}\n"
"	return color.rgb;\n"
"}\n"

"float ComputeMask(float3 vertexViewDir, float3 vertexObjPos, int maskType, float4 maskParameters, float4 maskAxis, float3x3 tangentTransform, float3 worldNormal, float3 surfaceNormal)\n"
"{\n"
"	if (maskType == MASK_TYPE_POSITIONAL)\n"
"	{\n"
"		float centerDistance = maskParameters.x;\n"
"		float fadeAbove = maskParameters.y;\n"
"		float fadeBelow = maskParameters.z;\n"
"		float d = dot(vertexObjPos, maskAxis.xyz);\n"
"		if (d > centerDistance)\n"
"		{\n"
"			return clamp(1.0 - (d - centerDistance) / fadeAbove, 0.0, 1.0);\n"
"		}\n"
"		else\n"
"		{\n"
"			return clamp(1.0 - (centerDistance - d) / fadeBelow, 0.0, 1.0);\n"
"		}\n"
"	}\n"
"	else if (maskType == MASK_TYPE_REFLECTION)\n"
"	{\n"
"		float fadeStart = maskParameters.x;\n"
"		float fadeEnd = maskParameters.y;\n"
"		float normalMapStrength = maskParameters.z;\n"
"		float3 viewReflect = reflect(-vertexViewDir, ComputeNormal(tangentTransform, worldNormal, surfaceNormal, normalMapStrength));\n"
"		float d = dot(viewReflect, maskAxis.xyz);\n"
"		return clamp(1.0 - (d - fadeStart) / (fadeEnd - fadeStart), 0.0, 1.0);\n"
"	}\n"
"	else if (maskType == MASK_TYPE_FRESNEL)\n"
"	{\n"
"		float power = maskParameters.x;\n"
"		float fadeStart = maskParameters.y;\n"
"		float fadeEnd = maskParameters.z;\n"
"		float normalMapStrength = maskParameters.w;\n"
"		float d = 1.0 - max(0.0, dot(vertexViewDir, ComputeNormal(tangentTransform, worldNormal, surfaceNormal, normalMapStrength)));\n"
"		float p = pow(abs(d), power);\n"
"		return clamp(lerp(fadeStart, fadeEnd, p), 0.0, 1.0);\n"
"	}\n"
"	else if (maskType == MASK_TYPE_PULSE)\n"
"	{\n"
"		float distance = maskParameters.x;\n"
"		float speed = maskParameters.y;\n"
"		float power = maskParameters.z;\n"
"		float d = dot(vertexObjPos, maskAxis.xyz);\n"
"		float theta = 6.2831 * frac((d - elapsedSeconds * speed) / distance);\n"
"		return clamp(pow((sin(theta) * 0.5 + 0.5), power), 0.0, 1.0);\n"
"	}\n"
"	else\n"
"	{\n"
"		return 1.0;\n"
"	}\n"
"	return 1.0;\n"
"}\n"

"float3 ComputeBlend(int blendMode, float3 dst, float3 src, float mask)\n"
"{\n"
"	if (blendMode == BLEND_MODE_MULTIPLY)\n"
"	{\n"
"		return dst * (src * mask);\n"
"	}\n"
"	else\n"
"	{\n"
"		return dst + src * mask;\n"
"	}\n"
"}\n"

"float4 main(OutputVS sInput) : SV_TARGET\n"
"{\n"
"	float3 worldNormal = normalize(sInput.vertexNormal);\n"
"	float3x3 tangentTransform = float3x3(sInput.vertexTangent, sInput.vertexBitangent, worldNormal);\n"

"	float2 uv = sInput.vertexUV;\n"
"	if (useProjector)\n"
"	{\n"
"		float4 projectorPos = mul(float4(sInput.vertexWorldPos, 1.0), projectorInv);\n"
"		if (abs(projectorPos.x) > 1.0 || abs(projectorPos.y) > 1.0 || abs(projectorPos.z) > 1.0)\n"
"		{\n"
"			discard;\n"
"		}\n"
"		uv = projectorPos.xy * 0.5 + 0.5;\n"
"	}\n"

"	float3 surfaceNormal = float3(0.0, 0.0, 1.0);\n"
"	if (useNormalMap)\n"
"	{\n"
"		surfaceNormal.xy = normalMap.Sample(samplerAvatar, uv * normalMapScaleOffset.xy + normalMapScaleOffset.zw).xy * 2.0 - 1.0;\n"
"		surfaceNormal.z = sqrt(1.0 - dot(surfaceNormal.xy, surfaceNormal.xy));\n"
"	}\n"

"	float4 color = baseColor;\n"
"	for (int i = 0; i < layerCount; ++i)\n"
"	{\n"
"		float3 layerColor = ComputeColor(sInput.vertexViewDir, layerSamplerModes[i], uv, layerColors[i], layerSurfaces[i], layerSurfaceScaleOffsets[i], layerSampleParameters[i], tangentTransform, worldNormal, surfaceNormal);\n"
"		float layerMask = ComputeMask(sInput.vertexViewDir, sInput.vertexObjPos, layerMaskTypes[i], layerMaskParameters[i], layerMaskAxes[i], tangentTransform, worldNormal, surfaceNormal);\n"
"		color.rgb = ComputeBlend(layerBlendModes[i], color.rgb, layerColor, layerMask);\n"
"	}\n"

"	if (useAlpha)\n"
"	{\n"
"		color.a *= alphaMask.Sample(samplerAvatar, uv * alphaMaskScaleOffset.xy + alphaMaskScaleOffset.zw).r;\n"
"	}\n"
"	color.a *= ComputeMask(sInput.vertexViewDir, sInput.vertexObjPos, baseMaskType, baseMaskParameters, baseMaskAxis, tangentTransform, worldNormal, surfaceNormal);\n"

"	return color;\n"
"}\n";

/**
* Node Commander enumeration
***/
enum ODM_Commanders
{
	VireioMenu,                  /**<  The Vireio Menu node connector. ***/
};

/**
* Node Commander Enumeration.
***/
enum ODM_Decommanders
{
	LeftTexture11,
	RightTexture11,
	LeftTexture10,
	RightTexture10,
	LeftTexture9,
	RightTexture9,
	HMD_Handle,
	ZoomOut,
	HUDTexture11,
	HUDTexture10,
	HUDTexture9,
};

/**
* Oculus layers enumeration.
***/
enum OculusLayers
{
	OculusLayer_MainEye = 0,
	OculusLayer_Hud = 1,
	OculusLayer_Total = 2,      /**< Total # of layers. **/
};

/**
* ovrSwapTextureSet wrapper class that also maintains the render target views
* needed for D3D11 rendering.
* Taken from the OculusRoomTiny demo for simplicity.
***/
struct OculusTexture
{
	ovrSession               Session;
	ovrTextureSwapChain      TextureChain;
	std::vector<ID3D11RenderTargetView*> TexRtv;

	OculusTexture() :
		Session(nullptr),
		TextureChain(nullptr)
	{
	}

	bool Init(ID3D11Device* pcDevice, ovrSession session, int sizeW, int sizeH)
	{
		Session = session;

		ovrTextureSwapChainDesc desc = {};
		desc.Type = ovrTexture_2D;
		desc.ArraySize = 1;
		desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.Width = sizeW;
		desc.Height = sizeH;
		desc.MipLevels = 1;
		desc.SampleCount = 1;
		desc.MiscFlags = ovrTextureMisc_DX_Typeless;
		desc.BindFlags = ovrTextureBind_DX_RenderTarget;
		desc.StaticImage = ovrFalse;

		ovrResult result = ovr_CreateTextureSwapChainDX(session, pcDevice, &desc, &TextureChain);
		if (!OVR_SUCCESS(result))
			return false;

		int textureCount = 0;
		ovr_GetTextureSwapChainLength(Session, TextureChain, &textureCount);
		for (int i = 0; i < textureCount; ++i)
		{
			ID3D11Texture2D* tex = nullptr;
			ovr_GetTextureSwapChainBufferDX(Session, TextureChain, i, IID_PPV_ARGS(&tex));
			D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			ID3D11RenderTargetView* rtv;
			pcDevice->CreateRenderTargetView(tex, &rtvd, &rtv);
			TexRtv.push_back(rtv);
			tex->Release();
		}

		return true;
	}

	~OculusTexture()
	{
		for (int i = 0; i < (int)TexRtv.size(); ++i)
		{
			SAFE_RELEASE(TexRtv[i]);
		}
		if (TextureChain)
		{
			ovr_DestroyTextureSwapChain(Session, TextureChain);
		}
	}

	ID3D11RenderTargetView* GetRTV()
	{
		int index = 0;
		ovr_GetTextureSwapChainCurrentIndex(Session, TextureChain, &index);
		return TexRtv[index];
	}

	// Commit changes
	void Commit()
	{
		ovr_CommitTextureSwapChain(Session, TextureChain);
	}
};

/**
* Oculus Mesh data structure (D3D11).
***/
struct MeshData
{
	// UINT unVertexArray;
	ID3D11Buffer* pcVertexBuffer;
	ID3D11Buffer* pcElementBuffer;
	UINT unElementCount;
	D3DXMATRIX asBindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
	D3DXMATRIX asInverseBindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
};

/**
* Simple texture data structure (D3D11).
***/
struct TextureData
{
	ID3D11Texture2D* pcTexture;
	ID3D11ShaderResourceView* pcSRV;
};

/**
* Oculus Direct Mode Node Plugin.
***/
class OculusDirectMode : public AQU_Nodus
{
public:
	OculusDirectMode();
	virtual ~OculusDirectMode();

	/*** OculusDirectMode public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** OculusDirectMode private methods ***/
	void ComputeWorldPose(const ovrAvatarSkinnedMeshPose& sLocalPose, D3DXMATRIX* asWorldPose);
	MeshData* LoadMesh(ID3D11Device* pcDevice, const ovrAvatarMeshAssetData* data);
	TextureData* LoadTexture(ID3D11Device* pcDevice, const ovrAvatarTextureAssetData* data);

	/**
	* True if OVR is initialized.
	***/
	bool m_bInit;
	/**
	* The handle of the headset.
	***/
	ovrSession m_hHMD;
	/**
	* The pointer to the HMD handle created either by this node or the oculus tracker node.
	***/
	ovrSession* m_phHMD;
	/**
	* The pointer to the handle created by the oculus tracker node.
	***/
	ovrSession* m_phHMD_Tracker;
	/**
	* The HMD description.
	***/
	ovrHmdDesc m_sHMDDesc;
	/**
	* Adapter Identifier.
	***/
	ovrGraphicsLuid m_sLuid;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	***/
	ovrMirrorTexture m_pcMirrorTexture;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	* Dx version for the HMD device.
	***/
	ID3D11Texture2D* m_pcMirrorTextureD3D11HMD;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	* Dx version for the game device.
	***/
	ID3D11Texture2D* m_pcMirrorTextureD3D11;
	/**
	* The Oculus swapchain. (for both eyes)
	***/
	OculusTexture* m_psEyeRenderTexture[2];
	/**
	* The Oculus HUD swapchain.
	***/
	OculusTexture* m_psEyeRenderTextureHUD;
	/**
	* The Oculus render viewport. (for both eyes)
	***/
	ovrRecti m_psEyeRenderViewport[2];
	/**
	* The Oculus render description. (for both eyes)
	***/
	ovrEyeRenderDesc m_psEyeRenderDesc[2];
	/**
	* Stereo Textures input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexView11[2];
	/**
	* HUD Texture input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexViewHud11;
	/**
	* Shared copy of textures.
	***/
	ID3D11Texture2D* m_pcTex11Copy[2];
	/**
	* Copy texture shared for HUD (created by game device).
	***/
	ID3D11Texture2D* m_pcTex11CopyHUD;
	/**
	* Shared texture for HUD (created by temporary device 1.1)
	***/
	ID3D11Texture2D* m_pcTex11SharedHUD;
	/**
	* Shared texture shader resource view for HUD (created by temporary device 1.1)
	**/
	ID3D11ShaderResourceView* m_pcTex11SharedHudSRV;
	/**
	* Temporary directx 11 device for the oculus sdk.
	***/
	ID3D11Device* m_pcDeviceTemporary;
	/**
	* Temporary directx 11 device context for the oculus sdk.
	***/
	ID3D11DeviceContext* m_pcContextTemporary;
	/**
	* Temporary directx 11 dxgi swapchain for the oculus sdk.
	* (keep that one for the mirror texture)
	***/
	IDXGISwapChain* m_pcSwapChainTemporary;
	/**
	* Temporary directx 11 back buffer for the oculus sdk.
	***/
	ID3D11Texture2D* m_pcBackBufferTemporary;
	/**
	* Temporary directx 11 back buffer render target view for the oculus sdk.
	***/
	ID3D11RenderTargetView * m_pcBackBufferRTVTemporary;
	/**
	* The 2D vertex shader.
	***/
	ID3D11VertexShader* m_pcVertexShader11;
	/**
	* The 2D pixel shader.
	***/
	ID3D11PixelShader* m_pcPixelShader11;
	/**
	* The 2D vertex layout.
	***/
	ID3D11InputLayout* m_pcVertexLayout11;
	/**
	* The 2D vertex buffer.
	***/
	ID3D11Buffer* m_pcVertexBuffer11;
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	ID3D11Buffer* m_pcConstantBufferDirect11;
	/**
	* Basic sampler state.
	***/
	ID3D11SamplerState* m_pcSamplerState;
	/**
	* The eventual frame textures to be rendered to the Oculus screens.
	***/
	ID3D11Texture2D* m_pcFrameTexture[2];
	/**
	* The eventual frame texture shader resource views to be rendered to the Oculus screens.
	***/
	ID3D11ShaderResourceView* m_pcFrameTextureSRView[2];
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
	/**
	* Zoom out switch.
	***/
	BOOL* m_pbZoomOut;
	/**
	* Eye render poses.
	***/
	ovrPosef asEyeRenderPose[2];
	/**
	* Timestamp for last drawn frame.
	***/
	double sensorSampleTime;
	/**
	* Oculus layers.
	***/
	ovrLayerHeader* m_pasLayerList[OculusLayer_Total];
	/**
	* Layer primal.
	***/
	ovrLayerEyeFov m_sLayerPrimal;
	/**
	* Layer HUD.
	***/
	ovrLayerQuad m_sLayerHud;
	/**
	* Performance HUD mode.
	***/
	ovrPerfHudMode m_ePerfHudMode;
	/**
	* True if mirror is to be shown.
	***/
	bool m_bShowMirror;
	/**
	* The Oculus Avatar.
	***/
	ovrAvatar* m_psAvatar;
	/**
	* Asset loading counter.
	***/
	int m_nLoadingAssets;
	/**
	* All Oulus Assets.
	***/
	std::map<ovrAvatarAssetID, void*> m_asAssetMap;
	/**
	* Vireio menu.
	***/
	VireioSubMenu m_sMenu;
	/**
	* Frames to save the ini file.
	***/
	INT m_nIniFrameCount;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OculusDirectMode* pOculusDirectMode = new OculusDirectMode();
	return static_cast<AQU_Nodus*>(pOculusDirectMode);
}