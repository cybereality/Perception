/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusDirectToRiftView.cpp> and
Class <OculusDirectToRiftView> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#include "OculusDirectToRiftView.h"
#include "StereoView.h"
#include "D3DProxyDevice.h"
#include "Resource.h"
#include "OculusTracker.h"

#define DLL_NAME "d3d9.dll"

/**
* Constructor.
* @param config Game configuration.
* @param hmd Oculus Rift Head Mounted Display info.
***/ 
OculusDirectToRiftView::OculusDirectToRiftView(ProxyConfig *config, HMDisplayInfo *hmd, MotionTracker *motionTracker) : 
	StereoView(config),
	hmdInfo(hmd),
	tracker(motionTracker),
	m_logoTexture(NULL),
	m_prevTexture(NULL)
{
	OutputDebugString("Created OculusDirectToRiftView\n");

	if (tracker &&
		config->tracker_mode == MotionTracker::OCULUSTRACK)
	{
		OculusTracker *pOculusTracker = static_cast<OculusTracker*>(tracker);
		ovrHmd hmd = pOculusTracker->GetOVRHmd();
	}
}

void OculusDirectToRiftView::ReleaseEverything()
{
	//Release the texture we loaded
	if (m_logoTexture)
	{
		m_logoTexture->Release();
		m_logoTexture = NULL;
	}

	//Call base class
	StereoView::ReleaseEverything();
}


/**
* Sets vertex shader constants.
***/ 
void OculusDirectToRiftView::SetViewEffectInitialValues() 
{
	SHOW_CALL("OculusDirectToRiftView::SetViewEffectInitialValues\n");

	viewEffect->SetFloatArray("LensCenter", LensCenter, 2);
	viewEffect->SetFloatArray("Scale", Scale, 2);
	viewEffect->SetFloatArray("ScaleIn", ScaleIn, 2);
	viewEffect->SetFloat("ZoomScale", m_zoom);
	viewEffect->SetFloat("ViewportXOffset", -ViewportXOffset);
	viewEffect->SetFloat("ViewportYOffset", -ViewportYOffset);
	if (chromaticAberrationCorrection)
		viewEffect->SetFloatArray("Chroma", hmdInfo->GetDistortionCoefficientsChroma(), 4);
	else
	{
		static float noChroma[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		viewEffect->SetFloatArray("Chroma", noChroma, 4);
	}

	viewEffect->SetFloatArray("Resolution", Resolution, 2);
	viewEffect->SetFloatArray("HmdWarpParam", hmdInfo->GetDistortionCoefficients(), 4);

	//Set rotation, this will only be non-zero if we have pixel shader roll enabled
	viewEffect->SetFloat("Rotation", m_rotation);

	//Set the black smear corection - 0.0f will do nothing
	viewEffect->SetFloat("SmearCorrection", m_blackSmearCorrection);

	//Set mouse position for VR Mouse
	viewEffect->SetFloatArray("MousePosition", m_mouseTexLocation, 2);

	if(m_3DReconstructionMode == 2)
		viewEffect->SetBool("ZBuffer", true);
	else
		viewEffect->SetBool("ZBuffer", false);
	
	viewEffect->SetFloat("ZBufferStrength", config->zbufferStrength);
	viewEffect->SetBool("ZBufferFilterMode", m_bZBufferFilterMode);
	viewEffect->SetFloat("ZBufferFilter", m_fZBufferFilter);	
	viewEffect->SetBool("ZBufferVisualisationMode", m_bZBufferVisualisationMode);
	viewEffect->SetFloat("ZBufferDepthLow", config->zbufferDepthLow);
	viewEffect->SetFloat("ZBufferDepthHigh", config->zbufferDepthHigh);
	viewEffect->SetBool("ZBufferSwitch", config->zbufferSwitch);

	//Local static for controlling vignette in telescopic sight mode
	static float vignette_val = 1.0f;

	switch (m_vignetteStyle)
	{
	case NONE:
		{
			//This also controls the zooming out of telescope mode
			if (vignette_val < 1.0f)
				vignette_val += 0.02f;
			float vignette[3] = {vignette_val, 0.05f, 0.0f};
			viewEffect->SetFloatArray("Vignette", vignette, 3);
		}
		break;
	case SOFT_EDGE:
		{
			//Soft edge implementation - Not used
			float vignette[3] = {0.9f, 0.1f, 0.275f};
			viewEffect->SetFloatArray("Vignette", vignette, 3);
		}
		break;
	case TELESCOPIC_SIGHT:
		{
			if (vignette_val > 0.44f)
				vignette_val -= 0.02f;
			float vignette[3] = {vignette_val, 0.05f, 0.0f};
			viewEffect->SetFloatArray("Vignette", vignette, 3);
		}
		break;
	}

	if (!m_logoTexture)
		D3DXCreateTextureFromResource(m_pActualDevice, GetModuleHandle(DLL_NAME), MAKEINTRESOURCE(IDB_IMAGE), &m_logoTexture);

	if (m_logoTexture)
	{
		m_pActualDevice->GetTexture(2, &m_prevTexture);
		m_pActualDevice->SetTexture(2, m_logoTexture);
	}

}

void OculusDirectToRiftView::PostViewEffectCleanup()
{
	if (m_prevTexture)
	{
		m_pActualDevice->SetTexture(2, m_prevTexture);
		//Does GetTexture increase ref count?!?
		m_prevTexture->Release();
		m_prevTexture = NULL;
	}	
}

void OculusDirectToRiftView::SetVRMouseSquish(float squish)
{
	m_VRMouseSquish = squish;
}

/**
* Calculate all vertex shader constants.
***/ 
void OculusDirectToRiftView::CalculateShaderVariables()
{
	SHOW_CALL("OculusDirectToRiftView::CalculateShaderVariables");

	// Center of half screen is 0.25 in x (halfscreen x input in 0 to 0.5 range)
	// Lens offset is in a -1 to 1 range. Using in shader with a 0 to 0.5 range so use 25% of the value.
	LensCenter[0] = 0.25f + (hmdInfo->GetLensXCenterOffset() * 0.25f) +config->IPDOffset;

	// Center of halfscreen range is 0.5 in y (halfscreen y input in 0 to 1 range)
	LensCenter[1] = hmdInfo->GetLensYCenterOffset() - config->YOffset; 
		
	ViewportXOffset = XOffset;
	ViewportYOffset = HeadYOffset;
	

	D3DSURFACE_DESC eyeTextureDescriptor;
	leftSurface->GetDesc(&eyeTextureDescriptor);

	float inputTextureAspectRatio = (float)eyeTextureDescriptor.Width / (float)eyeTextureDescriptor.Height;

	//Set the mouse position for VR Mouse
	if (m_mousePos.x != 0 && m_mousePos.y != 0)
	{
		//X mouse pos on the texture
		m_mousePos.x = abs((long)(m_mousePos.x % eyeTextureDescriptor.Width));
		m_mousePos.y = abs((long)(m_mousePos.y % eyeTextureDescriptor.Height));

		m_mouseTexLocation[0] = ((1.0f - m_VRMouseSquish) / 2.0f) + (m_VRMouseSquish * ((float)m_mousePos.x / (float)eyeTextureDescriptor.Width)) - (0.125f * m_VRMouseSquish);
		m_mouseTexLocation[1] = ((1.0f - m_VRMouseSquish) / 2.0f) + (m_VRMouseSquish * ((float)m_mousePos.y / (float)eyeTextureDescriptor.Height));
	}
	else
	{
		m_mouseTexLocation[0]=0.0f;
		m_mouseTexLocation[1]=0.0f;
	}
	
	// Note: The range is shifted using the LensCenter in the shader before the scale is applied so you actually end up with a -1 to 1 range
	// in the distortion function rather than the 0 to 2 I mention below.
	// Input texture scaling to sample the 0 to 0.5 x range of the half screen area in the correct aspect ratio in the distortion function
	// x is changed from 0 to 0.5 to 0 to 2.
	ScaleIn[0] = 4.0f;
	// y is changed from 0 to 1 to 0 to 2 and scaled to account for aspect ratio
	ScaleIn[1] = 2.0f / (inputTextureAspectRatio * 0.5f); // 1/2 aspect ratio for differing input ranges
	
	float scaleFactor = (1.0f / (hmdInfo->GetScaleToFillHorizontal() + config->DistortionScale));
	float glide = (sinf(1 + (-cosf(m_screenViewGlideFactor * 3.142f) / 2)) - 0.5f) * 2.0f;

	//GB This should change the zoom - not the scale factor
	//This change will gently glide the disconnected screen view in and out
	if (HeadZOffset != FLT_MAX)
	{
		m_zoom = (ZoomOutScale * ((glide * 0.333f) + 0.666f)) + HeadZOffset;
		m_zoom = (float)max(0.4, m_zoom);
		if (m_screenViewGlideFactor > 0.0f)
			m_screenViewGlideFactor -= 0.04f;
	}
	else
	{
		//Disconnected screen view not active
		m_zoom = (ZoomOutScale * ((glide * 0.333f) + 0.666f));
		if (m_screenViewGlideFactor < 1.0f)
			m_screenViewGlideFactor += 0.04f;
	}

	// Scale from 0 to 2 to 0 to 1  for x and y 
	// Then use scaleFactor to fill horizontal space in line with the lens and adjust for aspect ratio for y.
	Scale[0] = (1.0f / 4.0f) * scaleFactor;
	Scale[1] = (1.0f / 2.0f) * scaleFactor * inputTextureAspectRatio;

	//Set resolution  0 = Horizontal, 1 = Vertical
	Resolution[0] = (float)hmdInfo->GetResolution().first;
	Resolution[1] = (float)hmdInfo->GetResolution().second;

}

/**
* Loads Oculus Rift shader effect files.
***/ 
void OculusDirectToRiftView::InitShaderEffects()
{
	SHOW_CALL("OculusDirectToRiftView::InitShaderEffects");

	shaderEffect[OCULUS_DIRECT_TO_RIFT] = "OculusRift.fx";

	ProxyHelper helper = ProxyHelper();
	std::string viewPath = helper.GetPath("fx\\") + shaderEffect[config->stereo_mode];

	D3DXCreateEffectFromFile(m_pActualDevice, viewPath.c_str(), NULL, NULL, 0, NULL, &viewEffect, NULL);
}


