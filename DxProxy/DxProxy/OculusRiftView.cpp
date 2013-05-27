/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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


OculusRiftView::OculusRiftView(ProxyHelper::ProxyConfig& config):StereoView(config)
{
	OutputDebugString("Created OculusRiftView\n");

	DistortionScale = 1.70152f;

	float DistortionXCenterOffset = 0.145299f;
	float
		VPw = 640,
		VPh = 800;
	float
		x = 0.0f,
		y = 0.0f,
		w = 0.5f,
		h = 1.0f;

	LensCenter[0] = x + (w + DistortionXCenterOffset * 0.5f)*0.5f;			//	0.28632475
	LensCenter[1] = y + h*0.5f;
//	LensShift[0]  = LensCenter[0];
//	LensShift[1]  = LensCenter[1];
	LensShift[0]  = 0;
	LensShift[1]  = 0;

	CalculateShaderVariables();
}

OculusRiftView::~OculusRiftView()
{
}


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


void OculusRiftView::Draw(D3D9ProxySurface* stereoCapableSurface)
{
	// Copy left and right surfaces to textures to use as shader input
	// TODO match aspect ratio of source in target ? 
	IDirect3DSurface9* leftImage = stereoCapableSurface->getActualLeft();
	IDirect3DSurface9* rightImage = stereoCapableSurface->getActualRight();

	m_pActualDevice->StretchRect(leftImage, NULL, leftSurface, NULL, D3DTEXF_NONE);

	if (stereoCapableSurface->IsStereo())
		m_pActualDevice->StretchRect(rightImage, NULL, rightSurface, NULL, D3DTEXF_NONE);
	else
		m_pActualDevice->StretchRect(leftImage, NULL, rightSurface, NULL, D3DTEXF_NONE);

	SaveState();
	SetState();

	m_pActualDevice->SetFVF(D3DFVF_TEXVERTEX);

	if(!swap_eyes)
	{
		m_pActualDevice->SetTexture(0, leftTexture);
		m_pActualDevice->SetTexture(1, rightTexture);
	}
	else 
	{
		m_pActualDevice->SetTexture(0, rightTexture);
		m_pActualDevice->SetTexture(1, leftTexture);
	}

	m_pActualDevice->SetRenderTarget(0, backBuffer);
	m_pActualDevice->SetStreamSource(0, screenVertexBuffer, 0, sizeof(TEXVERTEX));

	UINT iPass, cPasses;

	viewEffect->SetTechnique("ViewShader");

/////  difference from StereoView::
	viewEffect->SetFloatArray("LensCenter", LensCenter,2);
	viewEffect->SetFloatArray("LensShift", LensShift,2);
	viewEffect->SetFloatArray("ScreenCenter", ScreenCenter,2);
	viewEffect->SetFloatArray("Scale", Scale,2);
	viewEffect->SetFloatArray("ScaleIn", ScaleIn,2);
	viewEffect->SetFloatArray("HmdWarpParam", HmdWarpParam,4);
/////

	viewEffect->Begin(&cPasses, 0);

	for(iPass = 0; iPass < cPasses; iPass++)
	{
		viewEffect->BeginPass(iPass);
		m_pActualDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		viewEffect->EndPass();
	}

	viewEffect->End();
	
	m_pActualDevice->StretchRect(backBuffer, NULL, backBuffer, NULL, D3DTEXF_NONE);

	RestoreState();
}

void OculusRiftView::CalculateShaderVariables()
{
	float
//		VPw = 1280,
		VPw = 640,
		VPh = 800;
	float
		x = 0.0f,
		y = 0.0f,
//		w = 1.0f,
		w = 0.5f,
		h = 1.0f;

	float asIn = float(VPw) / float(VPh);	// resoultion of eye input texture
	float as = float(1280) / float(800);	// resolution of eye on screen texture
	if(stereo_mode == OCULUS_RIFT_CROPPED)
		as = float(640) / float(800);	// resolution of eye on screen texture

	float scaleFactor = 1.0f / DistortionScale;

	float DistortionXCenterOffset = 0.145299f;
//	float DistortionXCenterOffset = 0.25;

	LensCenter[0] = x + (w + DistortionXCenterOffset * 0.5f)*0.5f;			//	0.28632475
	LensCenter[1] = y + h*0.5f;
	ScreenCenter[0] = x + w*0.5f;
	ScreenCenter[1] = y + h*0.5f;
	Scale[0] = (w/2.0f) * scaleFactor;
	Scale[1] = (h/2.0f) * scaleFactor * as;
	ScaleIn[0] = 2.0f/w;
	ScaleIn[1] = (2.0f/h) / asIn;

	// y or z
	HmdWarpParam[0] = 1.0f;	HmdWarpParam[1] = 0.22f;	HmdWarpParam[2] = 0.24f;	HmdWarpParam[3] = 0.0f;
//	HmdWarpParam[0] = 1.0f;	HmdWarpParam[1] = 0.18f;	HmdWarpParam[2] = 0.115;	HmdWarpParam[3] = 0.0f;
}

