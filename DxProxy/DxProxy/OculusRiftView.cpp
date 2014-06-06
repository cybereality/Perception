/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusRiftView.cpp> and
Class <OculusRiftView> :
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

#include "OculusRiftView.h"
#include "StereoView.h"
#include "D3DProxyDevice.h"

/**
* Constructor.
* @param config Game configuration.
* @param hmd Oculus Rift Head Mounted Display info.
***/ 
OculusRiftView::OculusRiftView(ProxyHelper::ProxyConfig& config, HMDisplayInfo hmd) : StereoView(config),
	hmdInfo(hmd)
{
	OutputDebugString("Created OculusRiftView\n");
}

/**
* Sets vertex shader constants.
***/ 
void OculusRiftView::SetViewEffectInitialValues() 
{
	viewEffect->SetFloatArray("LensCenter", LensCenter, 2);
	viewEffect->SetFloatArray("Scale", Scale, 2);
	viewEffect->SetFloatArray("ScaleIn", ScaleIn, 2);
	viewEffect->SetFloatArray("HmdWarpParam", hmdInfo.distortionCoefficients, 4);
}

/**
* Calculate all vertex shader constants.
***/ 
void OculusRiftView::CalculateShaderVariables()
{
	// Center of half screen is 0.25 in x (halfscreen x input in 0 to 0.5 range)
	// Lens offset is in a -1 to 1 range. Using in shader with a 0 to 0.5 range so use 25% of the value.
	LensCenter[0] = 0.25f + (hmdInfo.lensXCenterOffset * 0.25f);
	// Center of halfscreen range is 0.5 in y (halfscreen y input in 0 to 1 range)
	LensCenter[1] = hmdInfo.lensYCenterOffset - YOffset; 
	
	D3DSURFACE_DESC eyeTextureDescriptor;
	leftSurface->GetDesc(&eyeTextureDescriptor);

	float inputTextureAspectRatio = (float)eyeTextureDescriptor.Width / (float)eyeTextureDescriptor.Height;
	
	// Note: The range is shifted using the LensCenter in the shader before the scale is applied so you actually end up with a -1 to 1 range
	// in the distortion function rather than the 0 to 2 I mention below.
	// Input texture scaling to sample the 0 to 0.5 x range of the half screen area in the correct aspect ratio in the distortion function
	// x is changed from 0 to 0.5 to 0 to 2.
	ScaleIn[0] = 4.0f;
	// y is changed from 0 to 1 to 0 to 2 and scaled to account for aspect ratio
	ScaleIn[1] = 2.0f / (inputTextureAspectRatio * 0.5f); // 1/2 aspect ratio for differing input ranges
	
	float scaleFactor = 1.0f / (hmdInfo.scaleToFillHorizontal + DistortionScale );

	// Scale from 0 to 2 to 0 to 1  for x and y 
	// Then use scaleFactor to fill horizontal space in line with the lens and adjust for aspect ratio for y.
	Scale[0] = (1.0f / 4.0f) * scaleFactor;
	Scale[1] = (1.0f / 2.0f) * scaleFactor * inputTextureAspectRatio;
}

/**
* Loads Oculus Rift shader effect files.
***/ 
void OculusRiftView::InitShaderEffects()
{
	shaderEffect[OCULUS_RIFT] = "OculusRift.fx";
	shaderEffect[OCULUS_RIFT_CROPPED] = "OculusRiftCropped.fx";

	char viewPath[512];
	ProxyHelper helper = ProxyHelper();
	helper.GetPath(viewPath, "fx\\");

	strcat_s(viewPath, 512, shaderEffect[stereo_mode].c_str());

	D3DXCreateEffectFromFile(m_pActualDevice, viewPath, NULL, NULL, 0, NULL, &viewEffect, NULL);
}


