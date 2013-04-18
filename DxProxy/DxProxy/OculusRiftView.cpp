/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "OculusRiftView.h"
#include "StereoView.h"
#include "D3DProxyDevice.h"


OculusRiftView::OculusRiftView(ProxyHelper::ProxyConfig& config):StereoView(config)
{
	OutputDebugString("Created OculusRiftView\n");
	DistortionScale = 1.70152f;
}

OculusRiftView::~OculusRiftView()
{
	OutputDebugString("Destroyed OculusRiftView\n");
	StereoView::~StereoView();
}

void OculusRiftView::Init(IDirect3DDevice9* dev)
{
	OutputDebugString("OculusRiftView Init\n");
	StereoView::Init(dev);
}

void OculusRiftView::InitShaderEffects()
{
//	MessageBox(NULL,"OculusRiftView","",MB_OK);
	shaderEffect[OCULUS_RIFT] = "OculusRift.fx";
	shaderEffect[OCULUS_RIFT_CROPPED] = "OculusRiftCropped.fx";

	char viewPath[512];
	ProxyHelper helper = ProxyHelper();
	helper.GetPath(viewPath, "fx\\");

	strcat_s(viewPath, 512, shaderEffect[stereo_mode].c_str());

	D3DXCreateEffectFromFile(device, viewPath, NULL, NULL, 0, NULL, &viewEffect, NULL);
}

void OculusRiftView::InitTextureBuffers()
{
	StereoView::InitTextureBuffers();
}

void OculusRiftView::InitVertexBuffers()
{
	StereoView::InitVertexBuffers();
}

void OculusRiftView::SaveState()
{
	StereoView::SaveState();
}

void OculusRiftView::SetState()
{
	StereoView::SetState();
}

void OculusRiftView::RestoreState()
{
	StereoView::RestoreState();
}

void OculusRiftView::UpdateEye(int eye)
{
	IDirect3DSurface9* currentSurface = NULL;

	if(eye == LEFT_EYE)
	{
		currentSurface = leftSurface;
	} 
	else
	{
		currentSurface = rightSurface;
	}
/////  difference from StereoView::
	CalculateShaderVariables(eye);
/////
	device->StretchRect(backBuffer, NULL, currentSurface, NULL, D3DTEXF_NONE);
}

void OculusRiftView::Draw()
{
	SaveState();
	SetState();

	device->SetFVF(D3DFVF_TEXVERTEX);

	if(!swap_eyes)
	{
		device->SetTexture(0, leftTexture);
		device->SetTexture(1, rightTexture);
	}
	else 
	{
		device->SetTexture(0, rightTexture);
		device->SetTexture(1, leftTexture);
	}

	device->SetRenderTarget(0, screenSurface);
	device->SetStreamSource(0, screenVertexBuffer, 0, sizeof(TEXVERTEX));

	UINT iPass, cPasses;

	viewEffect->SetTechnique("ViewShader");

/////  difference from StereoView::
	viewEffect->SetFloatArray("LensCenter", LensCenter,2);
	viewEffect->SetFloatArray("ScreenCenter", ScreenCenter,2);
	viewEffect->SetFloatArray("Scale", Scale,2);
	viewEffect->SetFloatArray("ScaleIn", ScaleIn,2);
	viewEffect->SetFloatArray("HmdWarpParam", HmdWarpParam,4);
/////

	viewEffect->Begin(&cPasses, 0);

	for(iPass = 0; iPass < cPasses; iPass++)
	{
		viewEffect->BeginPass(iPass);
		device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		viewEffect->EndPass();
	}

	viewEffect->End();
	
	device->StretchRect(screenSurface, NULL, backBuffer, NULL, D3DTEXF_NONE);

	RestoreState();
}

void OculusRiftView::CalculateShaderVariables(int eye)
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

