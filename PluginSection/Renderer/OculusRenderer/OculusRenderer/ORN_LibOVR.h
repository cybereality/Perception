/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Renderer - Oculus Rift Renderer Node Plugin
Copyright (C) 2015 Denis Reischl

--------

File <ORN_LibOVR.h> explicitly derives from 
Oculus Rift SDK Library <LibOVR> :
Copyright (C) 2012 Oculus VR, LLC All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.2 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.2

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

---------

File <ORN_LibOVR.h> :
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

/**
* A 2D size with integer components.
* (LibOVR code)
***/
typedef struct ovrSizei_
{
	int w, h;
} ovrSizei;

/**
* A 2D vector with float components.
* (LibOVR code)
***/
typedef struct ovrVector2f_
{
	float x, y;
} ovrVector2f;

/**
* A 2D vector with integer components.
* (LibOVR code)
***/
typedef struct ovrVector2i_
{
	int x, y;
} ovrVector2i;

/**
* A 2D rectangle with a position and size.
* All components are integers.
* (LibOVR code)
***/
typedef struct ovrRecti_
{
	ovrVector2i Pos;
	ovrSizei    Size;
} ovrRecti;

/**
* Describes a vertex used by the distortion mesh. This is intended to be converted into
* the engine-specific format. Some fields may be unused based on the ovrDistortionCaps
* flags selected. TexG and TexB, for example, are not used if chromatic correction is
* not requested.
* (LibOVR code)
***/
typedef struct ovrDistortionVertex_
{
	ovrVector2f ScreenPosNDC;    ///< [-1,+1],[-1,+1] over the entire framebuffer.
	float       TimeWarpFactor;  ///< Lerp factor between time-warp matrices. Can be encoded in Pos.z.
	float       VignetteFactor;  ///< Vignette fade factor. Can be encoded in Pos.w.
	ovrVector2f TanEyeAnglesR;   ///< The tangents of the horizontal and vertical eye angles for the red channel.
	ovrVector2f TanEyeAnglesG;   ///< The tangents of the horizontal and vertical eye angles for the green channel.
	ovrVector2f TanEyeAnglesB;   ///< The tangents of the horizontal and vertical eye angles for the blue channel.
} ovrDistortionVertex;

/**
* ScaleAndOffset
* (LibOVR code)
***/
struct ScaleAndOffset2D
{
	ovrVector2f Scale;
	ovrVector2f Offset;

	ScaleAndOffset2D(float sx = 0.0f, float sy = 0.0f, float ox = 0.0f, float oy = 0.0f)
	{ 
		Scale.x = sx;
		Scale.y = sy;

		Offset.x = ox;
		Offset.y = oy;
	}
};

/**
* Field Of View (FOV) in tangent of the angle units.
* As an example, for a standard 90 degree vertical FOV, we would
* have: { UpTan = tan(90 degrees / 2), DownTan = tan(90 degrees / 2) }.
* (LibOVR code)
***/
typedef struct ovrFovPort_
{
	/**
	* The tangent of the angle between the viewing vector and the top edge of the field of view.
	***/
	float UpTan;
	/**
	* The tangent of the angle between the viewing vector and the bottom edge of the field of view.
	***/
	float DownTan;
	/**
	* The tangent of the angle between the viewing vector and the left edge of the field of view.
	***/
	float LeftTan;
	/**
	* The tangent of the angle between the viewing vector and the right edge of the field of view.
	***/
	float RightTan;
} ovrFovPort;

/**
* Oculus vertex shader (no timewarp).
***/
const char* VertexShaderSrc = 

	"float2 EyeToSourceUVScale  : register(c0);                                        \n"
	"float2 EyeToSourceUVOffset : register(c2);                                        \n"

	"void main(in float2 Position    : POSITION,    in float  TimeWarp    : POSITION1, \n"
	"          in float  Vignette    : POSITION2,   in float2 TexCoord0   : TEXCOORD0, \n"
	"          in float2 TexCoord1   : TEXCOORD1,   in float2 TexCoord2   : TEXCOORD2, \n"
	"          out float4 oPosition  : SV_Position, out float2 oTexCoord0 : TEXCOORD0, \n"
	"          out float2 oTexCoord1 : TEXCOORD1,   out float2 oTexCoord2 : TEXCOORD2, \n"
	"          out float oVignette   : TEXCOORD3)                                      \n"
	"{                                                                                 \n"
	"    oTexCoord0 = EyeToSourceUVScale * TexCoord0 + EyeToSourceUVOffset;            \n"
	"    oTexCoord1 = EyeToSourceUVScale * TexCoord1 + EyeToSourceUVOffset;            \n"
	"    oTexCoord2 = EyeToSourceUVScale * TexCoord2 + EyeToSourceUVOffset;            \n"
	"    oVignette  = Vignette;                                                        \n"
	"    oPosition  = float4(Position.xy, 0.5, 1.0);                                   \n"
	"}";

/**
* Oculus vertex shader with timewarp.
***/
const char* VertexShaderTimewarpSrc = 

	"float2 EyeToSourceUVScale    : register(c0);                                      \n"
	"float2 EyeToSourceUVOffset   : register(c2);                                      \n"
	"float4x4 EyeRotationStart : register(c4);                                         \n"
	"float4x4 EyeRotationEnd   : register(c20);                                        \n"

	"float2 TimewarpTexCoord(float2 TexCoord, float4x4 rotMat)                         \n"
	"{                                                                                 \n"
	"    float3 transformed = float3( mul ( rotMat, float4(TexCoord.xy, 1, 1) ).xyz);  \n"
	"    float2 flattened = (transformed.xy / transformed.z);                          \n"
	"    return(EyeToSourceUVScale * flattened + EyeToSourceUVOffset);                 \n"
	"}                                                                                 \n"
	"void main(in float2 Position    : POSITION,    in float  TimeWarp    : POSITION1, \n"
	"          in float  Vignette    : POSITION2,   in float2 TexCoord0   : TEXCOORD0, \n"
	"          in float2 TexCoord1   : TEXCOORD1,   in float2 TexCoord2   : TEXCOORD2, \n"
	"          out float4 oPosition  : SV_Position, out float2 oTexCoord0 : TEXCOORD0, \n"
	"          out float2 oTexCoord1 : TEXCOORD1,   out float2 oTexCoord2 : TEXCOORD2, \n"
	"          out float oVignette   : TEXCOORD3)                                      \n"
	"{                                                                                 \n"
	"    float4x4 lerpedEyeRot = lerp(EyeRotationStart, EyeRotationEnd, TimeWarp);     \n"
	"    oTexCoord0  = TimewarpTexCoord(TexCoord0,lerpedEyeRot);                       \n"
	"    oTexCoord1  = TimewarpTexCoord(TexCoord1,lerpedEyeRot);                       \n"
	"    oTexCoord2  = TimewarpTexCoord(TexCoord2,lerpedEyeRot);                       \n"
	"    oVignette  = Vignette;                                                        \n"
	"    oPosition  = float4(Position.xy, 0.5, 1.0);                                   \n"
	"}";

/**
* Oculus pixel shader with vignette.
***/
const char* PixelShaderSrc =

	" sampler2D Texture : register(s0);		                                           \n"

	"float4 main(in float4 oPosition  : SV_Position, in float2 oTexCoord0 : TEXCOORD0, \n"
	"            in float2 oTexCoord1 : TEXCOORD1,   in float2 oTexCoord2 : TEXCOORD2, \n"
	"            in float  oVignette  : TEXCOORD3) \n"
	"          : SV_Target                                                             \n" 
	"{                                                                                 \n"
	"	 float R = tex2D(Texture,oTexCoord0).r;		                                   \n"
	"	 float G = tex2D(Texture,oTexCoord1).g;		                                   \n"
	"	 float B = tex2D(Texture,oTexCoord2).b;		                                   \n"
	"    return (oVignette*float4(R,G,B,1));                                           \n"
	"}";

/**
* Simple side by side pixel shader.
***/
const char* PixelShaderSrcSideBySide =
	"// Combines two images into one Side-by-Side image		                           \n"

	"sampler2D TexMap0;		                                                           \n"
	"sampler2D TexMap1;		                                                           \n"

	"float4 SBS(float2 Tex : TEXCOORD0) : COLOR		                                   \n"
	"{		                                                                           \n"
	"	float4 tColor;		                                                           \n"
	"	float2 newPos = Tex;		                                                   \n"
	"	if(newPos.x < 0.5)		                                                       \n"
	"	{		                                                                       \n"
	"		newPos.x = newPos.x * 2.0f;		                                           \n"
	"		tColor = tex2D(TexMap0, newPos);			                               \n"
	"	}		                                                                       \n"
	"	else 		                                                                   \n"
	"	{		                                                                       \n"
	"		newPos.x = (newPos.x - 0.5f) * 2.0f;		                               \n"
	"		tColor = tex2D(TexMap1, newPos);		                                   \n"
	"	}		                                                                       \n"
	"	return tColor;		                                                           \n"
	"}		                                                                           \n";

/**
* LibOVR method to create scale and offset from FOV.
***/
ScaleAndOffset2D CreateNDCScaleAndOffsetFromFov(ovrFovPort tanHalfFov)
{
	// avoid compiler warning c4723
	if (tanHalfFov.LeftTan == 0.0f) tanHalfFov.LeftTan = 0.0001f;
	if (tanHalfFov.UpTan == 0.0f) tanHalfFov.UpTan = 0.0001f;

	float projXScale = 2.0f / ( tanHalfFov.LeftTan + tanHalfFov.RightTan );
	float projXOffset = ( tanHalfFov.LeftTan - tanHalfFov.RightTan ) * projXScale * 0.5f;
	float projYScale = 2.0f / ( tanHalfFov.UpTan + tanHalfFov.DownTan );
	float projYOffset = ( tanHalfFov.UpTan - tanHalfFov.DownTan ) * projYScale * 0.5f;

	ScaleAndOffset2D result;
	result.Scale.x  = projXScale;
	result.Scale.y  = projYScale;
	result.Offset.x = projXOffset;
	result.Offset.y	= projYOffset;
	// Hey - why is that Y.Offset negated?
	// It's because a projection matrix transforms from world coords with Y=up,
	// whereas this is from NDC which is Y=down.

	return result;
}

/**
* LibOVR method to create scale and offset from NDC spcae scale and offset.
***/
ScaleAndOffset2D CreateUVScaleAndOffsetfromNDCScaleandOffset(ScaleAndOffset2D scaleAndOffsetNDC, ovrRecti renderedViewport, ovrSizei renderTargetSize)
{
	// scaleAndOffsetNDC takes you to NDC space [-1,+1] within the given viewport on the rendertarget.
	// We want a scale to instead go to actual UV coordinates you can sample with,
	// which need [0,1] and ignore the viewport.
	ScaleAndOffset2D result;
	// Scale [-1,1] to [0,1]
	result.Scale.x = scaleAndOffsetNDC.Scale.x * 0.5f;
	result.Scale.y = scaleAndOffsetNDC.Scale.y * 0.5f;
	result.Offset.x = scaleAndOffsetNDC.Offset.x * 0.5f + 0.5f;
	result.Offset.y = scaleAndOffsetNDC.Offset.y * 0.5f + 0.5f;

	// ...but we will have rendered to a subsection of the RT, so scale for that.
	ovrVector2f scale;
	scale.x = (float)renderedViewport.Size.w / (float)renderTargetSize.w;
	scale.y = (float)renderedViewport.Size.h / (float)renderTargetSize.h;
	ovrVector2f offset;
	offset.x = (float)renderedViewport.Pos.x / (float)renderTargetSize.w;
	offset.y = (float)renderedViewport.Pos.y / (float)renderTargetSize.h;

	// Vector2	EntrywiseMultiply(const Vector2& b) const	{ return Vector2(x * b.x, y * b.y);}
	// result.Scale  = result.Scale.EntrywiseMultiply(scale);
	// result.Offset  = result.Offset.EntrywiseMultiply(scale) + offset;
	result.Scale.x = result.Scale.x * scale.x;
	result.Scale.y = result.Scale.y * scale.y;
	result.Offset.x = result.Offset.x * scale.x + offset.x;
	result.Offset.y = result.Offset.y * scale.y + offset.y;
	return result;
}

/**
* Computes updated 'uvScaleOffsetOut' to be used with a distortion if render target size or
* viewport changes after the fact. This can be used to adjust render size every frame, if desired.
* (LibOVR method)
***/
void ovrHmd_GetRenderScaleAndOffset( ovrFovPort fov, ovrSizei textureSize, ovrRecti renderViewport, ovrVector2f uvScaleOffsetOut[2] )
{        
	// Find the mapping from TanAngle space to target NDC space.
	ScaleAndOffset2D  eyeToSourceNDC = CreateNDCScaleAndOffsetFromFov(fov);
	// Find the mapping from TanAngle space to textureUV space.
	ScaleAndOffset2D  eyeToSourceUV  = CreateUVScaleAndOffsetfromNDCScaleandOffset(
		eyeToSourceNDC,
		renderViewport, textureSize );

	uvScaleOffsetOut[0].x = eyeToSourceUV.Scale.x;
	uvScaleOffsetOut[0].y = eyeToSourceUV.Scale.y;
	uvScaleOffsetOut[1].x = eyeToSourceUV.Offset.x;
	uvScaleOffsetOut[1].y = eyeToSourceUV.Offset.y;
}
