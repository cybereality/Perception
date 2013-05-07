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

#include "StereoView.h"

StereoView::StereoView(ProxyHelper::ProxyConfig& config)
{
	OutputDebugString("Created SteroView\n");
	initialized = false;
	game_type = config.game_type;
	stereo_mode = config.stereo_mode;
	swap_eyes = false;
	stereoEnabled = true;

	// set all member pointers to NULL to prevent uninitialized objects being used
	device = NULL;
	backBuffer = NULL;
	leftTexture = NULL;
	rightTexture = NULL;
	
	leftSurface = NULL;
	rightSurface = NULL;
	
	screenVertexBuffer = NULL;
	lastVertexShader = NULL;
	lastPixelShader = NULL;
	lastTexture = NULL;
	lastTexture1 = NULL;
	lastVertexDeclaration = NULL;
	lastRenderTarget0 = NULL;
	lastRenderTarget1 = NULL;
	viewEffect = NULL;
}

StereoView::~StereoView()
{
	OutputDebugString("Destroyed SteroView\n");
}



void releaseCheck(char* object, int newRefCount)
{
	if (newRefCount > 0) {
		char buf[128];
		sprintf_s(buf, "Error: %s count = %d\n", object, newRefCount);
		OutputDebugString(buf);
	}
}


void StereoView::Init(IDirect3DDevice9* dev)
{
	OutputDebugString("SteroView Init\n");

	if (initialized) {
		OutputDebugString("SteroView already Init'd\n");
		return;
	}


	device = dev;

	InitShaderEffects();
	InitTextureBuffers();
	InitVertexBuffers();

	initialized = true;
}





void StereoView::InitShaderEffects()
{
	shaderEffect[ANAGLYPH_RED_CYAN] = "AnaglyphRedCyan.fx";
	shaderEffect[ANAGLYPH_RED_CYAN_GRAY] = "AnaglyphRedCyanGray.fx";
	shaderEffect[ANAGLYPH_YELLOW_BLUE] = "AnaglyphYellowBlue.fx";
	shaderEffect[ANAGLYPH_YELLOW_BLUE_GRAY] = "AnaglyphYellowBlueGray.fx";
	shaderEffect[ANAGLYPH_GREEN_MAGENTA] = "AnaglyphGreenMagenta.fx";
	shaderEffect[ANAGLYPH_GREEN_MAGENTA_GRAY] = "AnaglyphGreenMagentaGray.fx";
	shaderEffect[SIDE_BY_SIDE] = "SideBySide.fx";
	shaderEffect[DIY_RIFT] = "SideBySideRift.fx";
	shaderEffect[OVER_UNDER] = "OverUnder.fx";
	shaderEffect[INTERLEAVE_HORZ] = "InterleaveHorz.fx";
	shaderEffect[INTERLEAVE_VERT] = "InterleaveVert.fx";
	shaderEffect[CHECKERBOARD] = "Checkerboard.fx";

	char viewPath[512];
	ProxyHelper helper = ProxyHelper();
	helper.GetPath(viewPath, "fx\\");

	strcat_s(viewPath, 512, shaderEffect[stereo_mode].c_str());

	D3DXCreateEffectFromFile(device, viewPath, NULL, NULL, 0, NULL, &viewEffect, NULL);
}

void StereoView::InitTextureBuffers()
{
	device->GetViewport(&viewport);
	D3DSURFACE_DESC pDesc = D3DSURFACE_DESC();
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	backBuffer->GetDesc(&pDesc);
	/*
	char buf[32];
	LPCSTR psz = NULL;

	wsprintf(buf,"vp w: %d",viewport.Width);
	psz = buf;
	OutputDebugString(psz);
	OutputDebugString("\n");
	
	

	wsprintf(buf,"bb w: %d",pDesc.Width);
	psz = buf;
	OutputDebugString(psz);
	OutputDebugString("\n");

	device->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &leftSurface, NULL);*/
	device->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &leftTexture, NULL);
	leftTexture->GetSurfaceLevel(0, &leftSurface);

	//device->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &rightSurface, NULL);
	device->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &rightTexture, NULL);
	rightTexture->GetSurfaceLevel(0, &rightSurface);

	/*device->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &screenSurface, NULL);
	device->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &screenTexture, NULL);
	screenTexture->GetSurfaceLevel(0, &screenSurface);*/
}

void StereoView::InitVertexBuffers()
{
	OutputDebugString("SteroView initVertexBuffers\n");

	device->CreateVertexBuffer(sizeof(TEXVERTEX) * 4, NULL,
        D3DFVF_TEXVERTEX, D3DPOOL_MANAGED, &screenVertexBuffer, NULL);

	TEXVERTEX* vertices;

	screenVertexBuffer->Lock(0, 0, (void**)&vertices, NULL);

	float scale = 1.0f;

	RECT* rDest = new RECT();
	rDest->left = 0;
	rDest->right = int(viewport.Width*scale);
	rDest->top = 0;
	rDest->bottom = int(viewport.Height*scale);
	
	//Setup vertices
	vertices[0].x = (float) rDest->left - 0.5f;
	vertices[0].y = (float) rDest->top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	
	vertices[1].x = (float) rDest->right - 0.5f;
	vertices[1].y = (float) rDest->top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].x = (float) rDest->right - 0.5f;
	vertices[2].y = (float) rDest->bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;	
	vertices[2].v = 1.0f;

	vertices[3].x = (float) rDest->left - 0.5f;
	vertices[3].y = (float) rDest->bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

	screenVertexBuffer->Unlock();
}

void StereoView::SaveState()
{
	device->GetTextureStageState(0, D3DTSS_COLOROP, &tssColorOp);
	device->GetTextureStageState(0, D3DTSS_COLORARG1, &tssColorArg1);
	device->GetTextureStageState(0, D3DTSS_ALPHAOP, &tssAlphaOp);
	device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &tssAlphaArg1);
	device->GetTextureStageState(0, D3DTSS_CONSTANT, &tssConstant);

	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &rsAlphaEnable);
	device->GetRenderState(D3DRS_ZWRITEENABLE, &rsZWriteEnable);
	device->GetRenderState(D3DRS_ZENABLE, &rsZEnable);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &rsSrgbEnable);

	device->GetSamplerState(0, D3DSAMP_SRGBTEXTURE, &ssSrgb);
	device->GetSamplerState(1, D3DSAMP_SRGBTEXTURE, &ssSrgb1);
	device->GetSamplerState(0, D3DSAMP_ADDRESSU, &ssAddressU);
	device->GetSamplerState(0, D3DSAMP_ADDRESSV, &ssAddressV);
	device->GetSamplerState(0, D3DSAMP_ADDRESSW, &ssAddressW);

	device->GetSamplerState(0, D3DSAMP_MAGFILTER, &ssMag0);
	device->GetSamplerState(1, D3DSAMP_MAGFILTER, &ssMag1);
	device->GetSamplerState(0, D3DSAMP_MINFILTER, &ssMin0);
	device->GetSamplerState(1, D3DSAMP_MINFILTER, &ssMin1);
	device->GetSamplerState(0, D3DSAMP_MIPFILTER, &ssMip0);
	device->GetSamplerState(1, D3DSAMP_MIPFILTER, &ssMip1);

	device->GetTexture(0, &lastTexture);
	device->GetTexture(1, &lastTexture1);

	device->GetVertexShader(&lastVertexShader);
	device->GetPixelShader(&lastPixelShader);

	device->GetVertexDeclaration(&lastVertexDeclaration);

	device->GetRenderTarget(0, &lastRenderTarget0);
	device->GetRenderTarget(1, &lastRenderTarget1);
}

void StereoView::SetState()
{
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
	device->SetTextureStageState(0, D3DTSS_CONSTANT, 0xffffffff);
	
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//device->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);  // will cause visual errors in HL2
	
	if(game_type == D3DProxyDevice::SOURCE_L4D)
	{
		device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, ssSrgb);
		device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, ssSrgb);
	}
	else 
	{
		device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
		device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, 0);
	}

	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	device->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	// TODO Need to check device capabilities if we want a prefered order of fallback rather than 
	// whatever the default is being used when a mode isn't supported.
	// Example - GeForce 660 doesn't appear to support D3DTEXF_ANISOTROPIC on the MAGFILTER (at least
	// according to the spam of error messages when running with the directx debug runtime)
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	device->SetTexture(0, NULL);
	device->SetTexture(1, NULL);

	device->SetVertexShader(NULL);
	device->SetPixelShader(NULL);

	device->SetVertexDeclaration(NULL);

	//It's a Direct3D9 error when using the debug runtine to set RenderTarget 0 to NULL
	//device->SetRenderTarget(0, NULL);
	device->SetRenderTarget(1, NULL);
}

void StereoView::RestoreState()
{
	device->SetTextureStageState(0, D3DTSS_COLOROP, tssColorOp);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, tssColorArg1);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, tssAlphaOp);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, tssAlphaArg1);
	device->SetTextureStageState(0, D3DTSS_CONSTANT, tssConstant);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, rsAlphaEnable);
	device->SetRenderState(D3DRS_ZWRITEENABLE, rsZWriteEnable);
	device->SetRenderState(D3DRS_ZENABLE, rsZEnable);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, rsSrgbEnable);

	device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, ssSrgb);
	device->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, ssSrgb1);

	device->SetSamplerState(0, D3DSAMP_ADDRESSU, ssAddressU);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, ssAddressV);
	device->SetSamplerState(0, D3DSAMP_ADDRESSW, ssAddressW);

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, ssMag0);
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, ssMag1);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, ssMin0);
	device->SetSamplerState(1, D3DSAMP_MINFILTER, ssMin1);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, ssMip0);
	device->SetSamplerState(1, D3DSAMP_MIPFILTER, ssMip1);

	device->SetTexture(0, lastTexture);
	if(lastTexture != NULL)
		lastTexture->Release();
	lastTexture = NULL;

	device->SetTexture(1, lastTexture1);
	if(lastTexture1 != NULL)
		lastTexture1->Release();
	lastTexture1 = NULL;

	device->SetVertexShader(lastVertexShader);
	if(lastVertexShader != NULL)
		lastVertexShader->Release();
	lastVertexShader = NULL;

	device->SetPixelShader(lastPixelShader);
	if(lastPixelShader != NULL)
		lastPixelShader->Release();
	lastPixelShader = NULL;

	device->SetVertexDeclaration(lastVertexDeclaration);
	if(lastVertexDeclaration != NULL)
		lastVertexDeclaration->Release();
	lastVertexDeclaration = NULL;

	device->SetRenderTarget(0, lastRenderTarget0);
	if(lastRenderTarget0 != NULL)
		lastRenderTarget0->Release();
	lastRenderTarget0 = NULL;

	device->SetRenderTarget(1, lastRenderTarget1);
	if(lastRenderTarget1 != NULL)
		lastRenderTarget1->Release();
	lastRenderTarget1 = NULL;
}



void StereoView::SwapEyes(bool doSwap)
{
	swap_eyes = doSwap;
}

/// To be removed ///
void StereoView::UpdateEye(int eye){}
void StereoView::Draw() {}
/////////////////////

void StereoView::Draw(D3D9ProxyStereoSurface* stereoCapableSurface)
{
	// Copy left and right surfaces to textures to use as shader input
	// TODO match aspect ratio of source in target ? 
	IDirect3DSurface9* leftImage = stereoCapableSurface->getLeftSurface();
	IDirect3DSurface9* rightImage = stereoCapableSurface->getRightSurface();

	device->StretchRect(leftImage, NULL, leftSurface, NULL, D3DTEXF_NONE);

	if (stereoCapableSurface->IsStereo())
		device->StretchRect(rightImage, NULL, rightSurface, NULL, D3DTEXF_NONE);
	else
		device->StretchRect(leftImage, NULL, rightSurface, NULL, D3DTEXF_NONE);


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

	device->SetRenderTarget(0, backBuffer);
	device->SetStreamSource(0, screenVertexBuffer, 0, sizeof(TEXVERTEX));

	UINT iPass, cPasses;

	viewEffect->SetTechnique("ViewShader");
	viewEffect->Begin(&cPasses, 0);

	for(iPass = 0; iPass < cPasses; iPass++)
	{
		viewEffect->BeginPass(iPass);
		device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		viewEffect->EndPass();
	}

	viewEffect->End();
	

	RestoreState();
}

void StereoView::SaveScreen()
{
	static int screenCount = 0;
	++screenCount;

	char fileName[32];
	wsprintf(fileName, "screenshot_%d.bmp", screenCount);
	OutputDebugString(fileName);
	OutputDebugString("\n");

	D3DXSaveSurfaceToFile(fileName, D3DXIFF_BMP, backBuffer, NULL, NULL);
}





void StereoView::Reset()
{
	OutputDebugString("SteroView Reset\n");

	if(!initialized)
		OutputDebugString("SteroView is already reset\n");

	if(backBuffer)
		releaseCheck("backBuffer", backBuffer->Release());	
	backBuffer = NULL;

	

	if(leftTexture)
		releaseCheck("leftTexture", leftTexture->Release());
	leftTexture = NULL;

	if(rightTexture)
		releaseCheck("rightTexture", rightTexture->Release());
	rightTexture = NULL;



	if(leftSurface)
		releaseCheck("leftSurface", leftSurface->Release());
	leftSurface = NULL;

	if(rightSurface)
		releaseCheck("rightSurface", rightSurface->Release());
	rightSurface = NULL;


	
	

	if(lastVertexShader)
		releaseCheck("lastVertexShader", lastVertexShader->Release());
	lastVertexShader = NULL;

	if(lastPixelShader)
		releaseCheck("lastPixelShader", lastPixelShader->Release());
	lastPixelShader = NULL;

	if(lastTexture)
		releaseCheck("lastTexture", lastTexture->Release());
	lastTexture = NULL;

	if(lastTexture1)
		releaseCheck("lastTexture1", lastTexture1->Release());
	lastTexture1 = NULL;

	if(lastVertexDeclaration)
		releaseCheck("lastVertexDeclaration", lastVertexDeclaration->Release());
	lastVertexDeclaration = NULL;

	if(lastRenderTarget0)
		releaseCheck("lastRenderTarget0", lastRenderTarget0->Release());
	lastRenderTarget0 = NULL;

	if(lastRenderTarget1)
		releaseCheck("lastRenderTarget1", lastRenderTarget1->Release());
	lastRenderTarget1 = NULL;

	if(viewEffect)
		releaseCheck("viewEffect", viewEffect->Release());
	viewEffect = NULL;

	initialized = false;
}