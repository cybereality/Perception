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



Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
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

#include <D3DX11.h>
#pragma comment(lib, "d3dx11.lib")

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
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
#include <OVR_Avatar.h>
#endif
#include <OVR_CAPI_D3D.h>
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
#include <OVR_Platform.h>
#include "DDS.h"
#else
typedef uint64_t ovrAvatarAssetID;
/// The maximum joint count that can be in a skinned mesh renderer.
///
#define OVR_AVATAR_MAXIMUM_JOINT_COUNT 64
/// The maximum number of material layers.
///
#define OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT 8
/// Mesh Vertex
/// custom mesh vertex declaration
typedef struct ovrAvatarMeshVertex_
{
	float   x;  ///< X component of the position
	float   y;  ///< Y component of the position
	float   z;  ///< Z component of the position
	float   nx; ///< X component of the normal
	float   ny; ///< Y component of the normal
	float   nz; ///< Z component of the normal
	float	tx; ///< X component of the tangent
	float	ty; ///< Y component of the tangent
	float	tz; ///< Z component of the tangent
	float	tw; ///< W component of the tangent
	float   u;  ///< U component of texture coordinates
	float   v;  ///< V component of texture coordinates
	uint8_t blendIndices[4]; ///< Joint indices for skin weighting
	float   blendWeights[4]; ///< Blend weights for skin weighting
} ovrAvatarMeshVertex;
/// 3D Vector Type
///
typedef struct ovrAvatarVector3f_
{
	float x, y, z;
} ovrAvatarVector3f;

/// 4D Vector Type
///
typedef struct ovrAvatarVector4f_
{
	float x, y, z, w;
} ovrAvatarVector4f;

/// Quaternion Type
///
typedef struct ovrAvatarQuatf_
{
	float x, y, z, w;
} ovrAvatarQuatf;
/// Transform Type
///
typedef struct ovrAvatarTransform_
{
	ovrAvatarVector3f position;
	ovrAvatarQuatf orientation;
	ovrAvatarVector3f scale;
} ovrAvatarTransform;
/// Material Mask Types
/// Masks programmatically control which portions of the surface this layer is applied to.
/// See reference shaders for implementation details.
///
typedef enum ovrAvatarMaterialMaskType_
{
	ovrAvatarMaterialMaskType_None,           ///< No mask is applied to this layer.
	ovrAvatarMaterialMaskType_Positional,     ///< A mask is applied to this based on vertex position.
	ovrAvatarMaterialMaskType_ViewReflection, ///< A mask is applied to this based on reflection of view vector.
	ovrAvatarMaterialMaskType_Fresnel,        ///< A mask is applied based on view direction's angle of incidence.
	ovrAvatarMaterialMaskType_Pulse,          ///< Like positional, but varying over time.
	ovrAvatarMaterialMaskType_Count
} ovrAvatarMaterialMaskType;
/// Material Layer Blend Modes
///
typedef enum ovrAvatarMaterialLayerBlendMode_
{
	ovrAvatarMaterialLayerBlendMode_Add,      ///< The color value of this layer is added to the layers before it.
	ovrAvatarMaterialLayerBlendMode_Multiply, ///< The color value of this layer is multiplied with the layers before it.
	ovrAvatarMaterialLayerBlendMode_Count
} ovrAvatarMaterialLayerBlendMode;
/// Material Layer Sample Modes
///
typedef enum ovrAvatarMaterialLayerSampleMode_
{
	ovrAvatarMaterialLayerSampleMode_Color,                ///< This layer's color value comes from the color constant.
	ovrAvatarMaterialLayerSampleMode_Texture,              ///< This layer's color value comes from the layer's texture sampler.
	ovrAvatarMaterialLayerSampleMode_TextureSingleChannel, ///< This layer's color value comes from a channel in the layer's texture sampler specified by the sample parameters.
	ovrAvatarMaterialLayerSampleMode_Parallax,             ///< This layer's color value comes from the layer's texture sampler with coordinates modified by the material's parallax texture.
	ovrAvatarMaterialLayerSampleMode_RDSM,                 ///< This layer's color value comes from the layer's texture sampler with coordinates modified by the material's roughness texture.
	ovrAvatarMaterialLayerSampleMode_Count
} ovrAvatarMaterialLayerSampleMode;
/// Material Layer State
///
typedef struct ovrAvatarMaterialLayerState_
{
	ovrAvatarMaterialLayerBlendMode     blendMode;         ///< Blend mode of the material layer
	ovrAvatarMaterialLayerSampleMode    sampleMode;        ///< Sample mode of the material layer
	ovrAvatarMaterialMaskType			maskType;          ///< Mask type of the material layer
	ovrAvatarVector4f					layerColor;        ///< Layer color
	ovrAvatarVector4f                   sampleParameters;  ///< Parameters driving sample mode
	ovrAvatarAssetID                    sampleTexture;     ///< Sample texture id (0 if unused)
	ovrAvatarVector4f					sampleScaleOffset; ///< UV scale and offset parameters for the sample texture
	ovrAvatarVector4f                   maskParameters;    ///< Parameters driving the layer mask
	ovrAvatarVector4f                   maskAxis;          ///< Axis used by some mask types
} ovrAvatarMaterialLayerState;
/// Material state
///
typedef struct ovrAvatarMaterialState_
{
	ovrAvatarVector4f			baseColor;               ///< Underlying base color for the material
	ovrAvatarMaterialMaskType	baseMaskType;            ///< Mask type of the base color
	ovrAvatarVector4f			baseMaskParameters;      ///< Parameters for the base mask type
	ovrAvatarVector4f			baseMaskAxis;            ///< Axis used by some mask types
	ovrAvatarAssetID            alphaMaskTextureID;      ///< Texture id for the base alpha mask (0 if unused)
	ovrAvatarVector4f			alphaMaskScaleOffset;    ///< UV scale and offset parameters for the alpha mask
	ovrAvatarAssetID			normalMapTextureID;      ///< Texture id for the normal map (0 if unused)
	ovrAvatarVector4f			normalMapScaleOffset;    ///< UV scale and offset parameters for the normal map
	ovrAvatarAssetID            parallaxMapTextureID;    ///< Texture id for the parallax map (0 if unused)
	ovrAvatarVector4f			parallaxMapScaleOffset;  ///< UV scale and offset parameters for the parallax map
	ovrAvatarAssetID			roughnessMapTextureID;   ///< Texture id for the roughness map (0 if unused)
	ovrAvatarVector4f			roughnessMapScaleOffset; ///< UV scale and offset parameters for the roughness map
	uint32_t                    layerCount;              ///< Number of layers
	ovrAvatarMaterialLayerState layers[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT]; ///< State for each material layer
} ovrAvatarMaterialState;
#include"OculusMesh_4070.h" // include mesh for 32 bit
#endif
#include <inttypes.h>

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\..\Include\VireioMenu.h"

// #define _CREATE_MESH_FILES // define this to output mesh data to c++ ".h" files
#ifdef _CREATE_MESH_FILES
#include <iostream>
#include <fstream>
#include <iomanip>
#endif

#define NUMBER_OF_COMMANDERS                            1
#define NUMBER_OF_DECOMMANDERS                         11

/**
* Vertex shader input constants.
***/
struct ConstantsVS
{
	XMVECTOR viewPos;
	XMMATRIX world;
	XMMATRIX viewProj;
	D3DXMATRIX meshPose[64];
};

/**
* Fragment shader input constants.
***/
#define MAX_LAYER_COUNT 8
struct FragmentVars
{
	float4 baseColor;
	float4 baseMaskParameters;
	float4 baseMaskAxis;
	float4 alphaMaskScaleOffset;
	float4 normalMapScaleOffset;
	float4 parallaxMapScaleOffset;
	float4 roughnessMapScaleOffset;
	float4x4 projectorInv;
	int baseMaskType;
	BOOL useAlpha;
	BOOL useNormalMap;
	BOOL useRoughnessMap;
	BOOL useProjector;
	float elapsedSeconds;
	int layerCount;
	int layerSamplerModes[MAX_LAYER_COUNT];
	int layerBlendModes[MAX_LAYER_COUNT];
	int layerMaskTypes[MAX_LAYER_COUNT];
	float4 layerColors[MAX_LAYER_COUNT];
	float4 layerSurfaceScaleOffsets[MAX_LAYER_COUNT];
	float4 layerSampleParameters[MAX_LAYER_COUNT];
	float4 layerMaskParameters[MAX_LAYER_COUNT];
	float4 layerMaskAxes[MAX_LAYER_COUNT];
};


/***
* Oculus Avatar Vertex Shader.
* (c) by Oculus VR
* Shader Model 5.0 (/5_0)
* HLSL conversion by Denis Reischl
***/
static const char* VS_OCULUS_AVATAR =
"cbuffer ConstantsVS : register (b0)\n"
"{\n"
"	float4 viewPos;\n"
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
"	uint4 poseIndices : TEXCOORD3;\n"
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
"	sOutput.position = mul(float4(sOutput.vertexWorldPos, 1.0f), viewProj);\n"
"	sOutput.vertexUV = sInput.texCoord;\n"

"	sOutput.vertexViewDir = normalize(viewPos - sOutput.vertexWorldPos).xyz;\n"
"	sOutput.vertexObjPos = sOutput.position.xyz;\n"

"	sOutput.vertexNormal = mul(normalPose, world).xyz;\n"
"	sOutput.vertexTangent = mul(tangentPose, world).xyz;\n"
"	sOutput.vertexBitangent = normalize(cross(sOutput.vertexNormal, sOutput.vertexTangent) * sInput.tangent.w);\n"

"	return sOutput;\n"
"}\n";

/***
* Oculus Avatar Fragment Shader.
* (c) by Oculus VR
* Shader Model 5.0 (/5_0)
* HLSL conversion by Denis Reischl
***/
static const char* PS_OCULUS_AVATAR =
//"Texture2D textureAvatar : register(t0);\n"
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

"Texture2D alphaMask : register(t0);\n"
"Texture2D normalMap : register(t1);\n"
"Texture2D parallaxMap : register(t2);\n"
"Texture2D roughnessMap : register(t3);\n"
"Texture2D layerSurfaces[MAX_LAYER_COUNT] : register(t4);\n"

"cbuffer FragmentVars : register (b0)\n"
"{\n"
"	float4 baseColor;\n"
"	float4 baseMaskParameters;\n"
"	float4 baseMaskAxis;\n"
"	float4 alphaMaskScaleOffset;\n"
"	float4 normalMapScaleOffset;\n"
"	float4 parallaxMapScaleOffset;\n"
"	float4 roughnessMapScaleOffset;\n"

"	float4x4 projectorInv;\n"

"	int baseMaskType;\n"
"	int useAlpha;\n"
"	int useNormalMap;\n"
"	int useRoughnessMap;\n"
"	int useProjector;\n"
"	float elapsedSeconds;\n"

"	int layerCount;\n"

"	int layerSamplerModes[MAX_LAYER_COUNT];\n"
"	int layerBlendModes[MAX_LAYER_COUNT];\n"
"	int layerMaskTypes[MAX_LAYER_COUNT];\n"
"	float4 layerColors[MAX_LAYER_COUNT];\n"
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
"		return worldNormal;\n"
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
"		float d = 1.0 - max(0.0, abs(dot(vertexViewDir, ComputeNormal(tangentTransform, worldNormal, surfaceNormal, normalMapStrength))));\n"
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

//"	color.a += 0.4f;"

"	if (useAlpha)\n"
"	{\n"
"		color.a *= alphaMask.Sample(samplerAvatar, uv * alphaMaskScaleOffset.xy + alphaMaskScaleOffset.zw).r;\n"
"	}\n"
"	color.a *= ComputeMask(sInput.vertexViewDir, sInput.vertexObjPos, baseMaskType, baseMaskParameters, baseMaskAxis, tangentTransform, worldNormal, surfaceNormal);\n"

//"	color.a += 0.2f;"

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
		{
			{ wchar_t buf[128]; wsprintf(buf, L"[OVR] Failed ovr_CreateTextureSwapChainDX : %x", result); OutputDebugString(buf); }
			{
			wchar_t buf[128]; wsprintf(buf, L"[OVR] sizeW : %u", sizeW); OutputDebugString(buf);
		}
			{ wchar_t buf[128]; wsprintf(buf, L"[OVR] sizeH : %u", sizeH); OutputDebugString(buf); }

			return false;
		}

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
* Definitions of axes for coordinate and rotation conversions.
* LibOVR 0.4.x enumeration.
***/
enum Axis
{
	Axis_X = 0, Axis_Y = 1, Axis_Z = 2
};

/**
* RotateDirection describes the rotation direction around an axis, interpreted as follows:
*  CW  - Clockwise while looking "down" from positive axis towards the origin.
*  CCW - Counter-clockwise while looking from the positive axis towards the origin,
*        which is in the negative axis direction.
*  CCW is the default for the RHS coordinate system. Oculus standard RHS coordinate
*  system defines Y up, X right, and Z back (pointing out from the screen). In this
*  system Rotate_CCW around Z will specifies counter-clockwise rotation in XY plane.
* LibOVR 0.4.x enumeration.
***/
enum RotateDirection
{
	Rotate_CCW = 1,
	Rotate_CW = -1
};

/**
* Constants for right handed and left handed coordinate systems
* LibOVR 0.4.x enumeration.
***/
enum HandedSystem
{
	Handed_R = 1, Handed_L = -1
};

/**
* AxisDirection describes which way the coordinate axis points. Used by WorldAxes.
* LibOVR 0.4.x enumeration.
***/
enum AxisDirection
{
	Axis_Up = 2,
	Axis_Down = -2,
	Axis_Right = 1,
	Axis_Left = -1,
	Axis_In = 3,
	Axis_Out = -3
};

/**
* GetEulerAngles extracts Euler angles from the quaternion, in the specified order of
* axis rotations and the specified coordinate system. Right-handed coordinate system
* is the default, with CCW rotations while looking in the negative axis direction.
* Here a,b,c, are the Yaw/Pitch/Roll angles to be returned.
* rotation a around axis A1
* is followed by rotation b around axis A2
* is followed by rotation c around axis A3
* rotations are CCW or CW (D) in LH or RH coordinate system (S)
* (LibOVR 0.4.x method)
***/
struct __ovrQuatf : public ovrQuatf
{
	//float x, y, z, w;
	template <Axis A1, Axis A2, Axis A3, RotateDirection D, HandedSystem S>
	void GetEulerAngles(float *a, float *b, float *c) const
	{
		static_assert((A1 != A2) && (A2 != A3) && (A1 != A3), "(A1 != A2) && (A2 != A3) && (A1 != A3)");

		float Q[3] = { x, y, z };  //Quaternion components x,y,z

		float ww = w*w;
		float Q11 = Q[A1] * Q[A1];
		float Q22 = Q[A2] * Q[A2];
		float Q33 = Q[A3] * Q[A3];

		float psign = float(-1);
		// Determine whether even permutation
		if (((A1 + 1) % 3 == A2) && ((A2 + 1) % 3 == A3))
			psign = float(1);

		float s2 = psign * float(2) * (psign*w*Q[A2] + Q[A1] * Q[A3]);

		if (s2 < float(-1) + ((float)MATH_DOUBLE_SINGULARITYRADIUS))
		{ // South pole singularity
			*a = float(0);
			*b = -S*D*((float)MATH_DOUBLE_PIOVER2);
			*c = S*D*atan2(float(2)*(psign*Q[A1] * Q[A2] + w*Q[A3]),
				ww + Q22 - Q11 - Q33);
		}
		else if (s2 > float(1) - ((float)MATH_DOUBLE_SINGULARITYRADIUS))
		{  // North pole singularity
			*a = float(0);
			*b = S*D*((float)MATH_DOUBLE_PIOVER2);
			*c = S*D*atan2(float(2)*(psign*Q[A1] * Q[A2] + w*Q[A3]),
				ww + Q22 - Q11 - Q33);
		}
		else
		{
			*a = -S*D*atan2(float(-2)*(w*Q[A1] - psign*Q[A2] * Q[A3]),
				ww + Q33 - Q11 - Q22);
			*b = S*D*asin(s2);
			*c = S*D*atan2(float(2)*(w*Q[A3] - psign*Q[A1] * Q[A2]),
				ww + Q11 - Q22 - Q33);
		}
		return;
	}
};

//#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
/**
* Oculus Mesh data structure (D3D11).
***/
struct MeshData
{
	// UINT unVertexArray;
	ID3D11Buffer* pcVertexBuffer;
	ID3D11Buffer* pcElementBuffer;
	UINT unElementCount;
	_D3DMATRIX asBindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
	_D3DMATRIX asInverseBindPose[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
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
* Data structure union.
***/
struct AvatarData
{
	union
	{
		TextureData sTexture;
		MeshData sMesh;
	};
};
//#endif

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
	virtual ImVec2          GetNodeSize() { return ImVec2((float)g_uGlobalNodeWidth, (float)GUI_HEIGHT); }
	//virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	//virtual DWORD           GetNodeHeight() { return 128; }
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
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
	/*** OculusDirectMode private methods ***/
	void ComputeWorldPose(const ovrAvatarSkinnedMeshPose& sLocalPose, D3DXMATRIX* asWorldPose);
	void LoadMesh(ID3D11Device* pcDevice, const ovrAvatarMeshAssetData* data, MeshData* mesh);
	void LoadTexture(ID3D11Device* pcDevice, const ovrAvatarTextureAssetData* data, TextureData* texture);
	void SetMeshState(const ovrAvatarTransform& localTransform, const MeshData* data, const ovrAvatarSkinnedMeshPose& skinnedPose, const XMMATRIX world, const XMMATRIX view, const XMMATRIX proj, const XMVECTOR viewPos);
#endif
	void SetMaterialState(const ovrAvatarMaterialState* state, XMMATRIX* projectorInv);

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

#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
	/**
	* The Oculus Avatar.
	***/
	ovrAvatar* m_psAvatar;
#endif
	/**
	* Asset loading counter.
	***/
	int m_nLoadingAssets;
	/**
	* All Oculus Asset indices.
	***/
	std::map<ovrAvatarAssetID, uint32_t> m_auAssetIndexMap;
	/**
	* All Oculus Assets.
	***/
	std::vector<AvatarData> m_asAssetMap;
	/**
	* Vertex shader constants.
	***/
	ConstantsVS m_sConstantsVS;
	/**
	* Fragment shader constants.
	***/
	FragmentVars m_sConstantsFS;
	/**
	* The avatar vertex shader.
	***/
	ID3D11VertexShader* m_pcVSAvatar;
	/**
	* The avatar pixel shader.
	***/
	ID3D11PixelShader* m_pcPSAvatar;
	/**
	* The avatar vertex layout.
	***/
	ID3D11InputLayout* m_pcILAvatar;
	/**
	* The constant buffer for the avatar vertex shader.
	***/
	ID3D11Buffer* m_pcCVSAvatar;
	/**
	* The constant buffer for the avatar pixel shader.
	***/
	ID3D11Buffer* m_pcCPSAvatar;
	/**
	* The depth stencil DX11 left/right.
	***/
	ID3D11Texture2D* m_pcDSGeometry11[2];
	/**
	* Depth stencil state less.
	***/
	ID3D11DepthStencilState* m_pcDSStateLess;
	/**
	* Depth stencil state equal.
	***/
	ID3D11DepthStencilState* m_pcDSStateEqual;
	/**
	* The depth stencil view DX11.
	***/
	ID3D11DepthStencilView* m_pcDSVGeometry11[2];
	/**
	* The rasterizer state (for self occlusion culling is turned off)
	***/
	ID3D11RasterizerState* m_pcRS;
	/**
	* Own state for the right hand due to reversed mesh triangles.
	***/
	ID3D11RasterizerState* m_pcRS_RightHand;
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