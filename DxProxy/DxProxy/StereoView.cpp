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

DWORD g_color;   

StereoView::StereoView(ProxyHelper::ProxyConfig& config)
{
	OutputDebugString("Created SteroView\n");
	initialized = false;
	game_type = config.game_type;
	stereo_mode = config.stereo_mode;
	swap_eyes = false;
	stereoEnabled = true;

	// set all member pointers to NULL to prevent uninitialized objects being used
	m_pActualDevice = NULL;
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
	sb = NULL;

	g_color = D3DCOLOR_RGBA(255,255,0,255);
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


void StereoView::Init(IDirect3DDevice9* pActualDevice)
{
	OutputDebugString("SteroView Init\n");


	if (initialized) {
		OutputDebugString("SteroView already Init'd\n");
		return;
	}


	m_pActualDevice = pActualDevice;

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

	if (FAILED(D3DXCreateEffectFromFile(m_pActualDevice, viewPath, NULL, NULL, D3DXFX_DONOTSAVESTATE, NULL, &viewEffect, NULL))) {
		OutputDebugString("Effect creation failed\n");
	}
}

void StereoView::InitTextureBuffers()
{
	m_pActualDevice->GetViewport(&viewport);
	D3DSURFACE_DESC pDesc = D3DSURFACE_DESC();
	m_pActualDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
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

	m_pActualDevice->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &leftSurface, NULL);*/
	m_pActualDevice->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &leftTexture, NULL);
	leftTexture->GetSurfaceLevel(0, &leftSurface);

	//m_pActualDevice->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &rightSurface, NULL);
	m_pActualDevice->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &rightTexture, NULL);
	rightTexture->GetSurfaceLevel(0, &rightSurface);

	/*m_pActualDevice->CreateOffscreenPlainSurface(pDesc.Width, pDesc.Height, pDesc.Format, D3DPOOL_SYSTEMMEM, &screenSurface, NULL);
	m_pActualDevice->CreateTexture(pDesc.Width, pDesc.Height, 0, D3DUSAGE_RENDERTARGET, pDesc.Format, D3DPOOL_DEFAULT, &screenTexture, NULL);
	screenTexture->GetSurfaceLevel(0, &screenSurface);*/
}

void StereoView::InitVertexBuffers()
{
	OutputDebugString("SteroView initVertexBuffers\n");


	m_pActualDevice->CreateVertexBuffer(sizeof(TEXVERTEX) * 4, NULL,
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
	m_pActualDevice->GetTextureStageState(0, D3DTSS_COLOROP, &tssColorOp);
	m_pActualDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &tssColorArg1);
	m_pActualDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &tssAlphaOp);
	m_pActualDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &tssAlphaArg1);
	m_pActualDevice->GetTextureStageState(0, D3DTSS_CONSTANT, &tssConstant);

	m_pActualDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &rsAlphaEnable);
	m_pActualDevice->GetRenderState(D3DRS_ZWRITEENABLE, &rsZWriteEnable);
	m_pActualDevice->GetRenderState(D3DRS_ZENABLE, &rsZEnable);
	m_pActualDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &rsSrgbEnable);

	m_pActualDevice->GetSamplerState(0, D3DSAMP_SRGBTEXTURE, &ssSrgb);
	m_pActualDevice->GetSamplerState(1, D3DSAMP_SRGBTEXTURE, &ssSrgb1);
	m_pActualDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &ssAddressU);
	m_pActualDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &ssAddressV);
	m_pActualDevice->GetSamplerState(0, D3DSAMP_ADDRESSW, &ssAddressW);

	m_pActualDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &ssMag0);
	m_pActualDevice->GetSamplerState(1, D3DSAMP_MAGFILTER, &ssMag1);
	m_pActualDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &ssMin0);
	m_pActualDevice->GetSamplerState(1, D3DSAMP_MINFILTER, &ssMin1);
	m_pActualDevice->GetSamplerState(0, D3DSAMP_MIPFILTER, &ssMip0);
	m_pActualDevice->GetSamplerState(1, D3DSAMP_MIPFILTER, &ssMip1);

	m_pActualDevice->GetTexture(0, &lastTexture);
	m_pActualDevice->GetTexture(1, &lastTexture1);

	m_pActualDevice->GetVertexShader(&lastVertexShader);
	m_pActualDevice->GetPixelShader(&lastPixelShader);

	m_pActualDevice->GetVertexDeclaration(&lastVertexDeclaration);

	m_pActualDevice->GetRenderTarget(0, &lastRenderTarget0);
	m_pActualDevice->GetRenderTarget(1, &lastRenderTarget1);
}

void StereoView::SetState()
{
	D3DXMATRIX	identity;
	m_pActualDevice->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&identity));
	m_pActualDevice->SetTransform(D3DTS_VIEW, &identity);
	m_pActualDevice->SetTransform(D3DTS_PROJECTION, &identity);
	m_pActualDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pActualDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);// This fixed interior or car not being drawn in rFactor
	m_pActualDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE); 

	m_pActualDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_CONSTANT, 0xffffffff);
	
	m_pActualDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pActualDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  
	
	//m_pActualDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);  // will cause visual errors in HL2
	
	if(game_type == D3DProxyDevice::SOURCE_L4D)
	{
		m_pActualDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, ssSrgb);
		m_pActualDevice->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, ssSrgb);
	}
	else 
	{
		m_pActualDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
		m_pActualDevice->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, 0);
	}

	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	// TODO Need to check m_pActualDevice capabilities if we want a prefered order of fallback rather than 
	// whatever the default is being used when a mode isn't supported.
	// Example - GeForce 660 doesn't appear to support D3DTEXF_ANISOTROPIC on the MAGFILTER (at least
	// according to the spam of error messages when running with the directx debug runtime)
	m_pActualDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//m_pActualDevice->SetTexture(0, NULL);
	//m_pActualDevice->SetTexture(1, NULL);

	m_pActualDevice->SetVertexShader(NULL);
	m_pActualDevice->SetPixelShader(NULL);

	m_pActualDevice->SetVertexDeclaration(NULL);

	//It's a Direct3D9 error when using the debug runtine to set RenderTarget 0 to NULL
	//m_pActualDevice->SetRenderTarget(0, NULL);
	m_pActualDevice->SetRenderTarget(1, NULL);
	m_pActualDevice->SetRenderTarget(2, NULL);
	m_pActualDevice->SetRenderTarget(3, NULL);
}

void StereoView::RestoreState()
{
	m_pActualDevice->SetTextureStageState(0, D3DTSS_COLOROP, tssColorOp);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_COLORARG1, tssColorArg1);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, tssAlphaOp);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, tssAlphaArg1);
	m_pActualDevice->SetTextureStageState(0, D3DTSS_CONSTANT, tssConstant);

	m_pActualDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, rsAlphaEnable);
	m_pActualDevice->SetRenderState(D3DRS_ZWRITEENABLE, rsZWriteEnable);
	m_pActualDevice->SetRenderState(D3DRS_ZENABLE, rsZEnable);
	m_pActualDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, rsSrgbEnable);

	m_pActualDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, ssSrgb);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_SRGBTEXTURE, ssSrgb1);

	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, ssAddressU);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, ssAddressV);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, ssAddressW);

	m_pActualDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, ssMag0);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, ssMag1);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_MINFILTER, ssMin0);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MINFILTER, ssMin1);
	m_pActualDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, ssMip0);
	m_pActualDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, ssMip1);

	m_pActualDevice->SetTexture(0, lastTexture);
	if(lastTexture != NULL)
		lastTexture->Release();
	lastTexture = NULL;

	m_pActualDevice->SetTexture(1, lastTexture1);
	if(lastTexture1 != NULL)
		lastTexture1->Release();
	lastTexture1 = NULL;

	m_pActualDevice->SetVertexShader(lastVertexShader);
	if(lastVertexShader != NULL)
		lastVertexShader->Release();
	lastVertexShader = NULL;

	m_pActualDevice->SetPixelShader(lastPixelShader);
	if(lastPixelShader != NULL)
		lastPixelShader->Release();
	lastPixelShader = NULL;

	m_pActualDevice->SetVertexDeclaration(lastVertexDeclaration);
	if(lastVertexDeclaration != NULL)
		lastVertexDeclaration->Release();
	lastVertexDeclaration = NULL;

	m_pActualDevice->SetRenderTarget(0, lastRenderTarget0);
	if(lastRenderTarget0 != NULL)
		lastRenderTarget0->Release();
	lastRenderTarget0 = NULL;

	m_pActualDevice->SetRenderTarget(1, lastRenderTarget1);
	if(lastRenderTarget1 != NULL)
		lastRenderTarget1->Release();
	lastRenderTarget1 = NULL;
}



void StereoView::SwapEyes(bool doSwap)
{
	swap_eyes = doSwap;
}





void StereoView::SetViewEffectInitialValues() {}


void StereoView::Draw(D3D9ProxySurface* stereoCapableSurface)
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


	// TODO figure out HL2 problem. This is a workaround for now
	// Problem: Using StateBlock to save and restore causes the world in HL2 to scale up and down constantly
	// This only effects HL2 (but all source games are using the l4d profile).
	// Possbile fix: Use a more discriminant stateblock to save only what is being modified
	if(game_type == D3DProxyDevice::SOURCE_L4D)
	{
		SaveState();
	}
	else {
		m_pActualDevice->CreateStateBlock(D3DSBT_ALL, &sb);
	}



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




	if (FAILED(m_pActualDevice->SetRenderTarget(0, backBuffer))) {
		OutputDebugString("SetRenderTarget backbuffer failed\n");
	}


	/*if (FAILED(m_pActualDevice->Clear(0, NULL, D3DCLEAR_TARGET, g_color, 0, 0))) {
		OutputDebugString("clear failed\n");
	}*/

	if (FAILED(m_pActualDevice->SetStreamSource(0, screenVertexBuffer, 0, sizeof(TEXVERTEX)))) {
		OutputDebugString("SetStreamSource failed\n");
	}

	UINT iPass, cPasses;


	if (FAILED(viewEffect->SetTechnique("ViewShader"))) {
		OutputDebugString("SetTechnique failed\n");
	}

	SetViewEffectInitialValues();


	if (FAILED(viewEffect->Begin(&cPasses, 0))) {
		OutputDebugString("Begin failed\n");
	}

	for(iPass = 0; iPass < cPasses; iPass++)
	{
		if (FAILED(viewEffect->BeginPass(iPass))) {
			OutputDebugString("Beginpass failed\n");
		}

		if (FAILED(m_pActualDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2))) {
			OutputDebugString("Draw failed\n");
		}

		if (FAILED(viewEffect->EndPass())) {
			OutputDebugString("Beginpass failed\n");
		}
	}

	if (FAILED(viewEffect->End())) {
		OutputDebugString("End failed\n");
	}
	
	// TODO figure out HL2 problem. This is a workaround for now
	if(game_type == D3DProxyDevice::SOURCE_L4D)
	{
		RestoreState();
	}
	else {
		sb->Apply();
		sb->Release();
		sb = NULL;
	}
}

void StereoView::SaveScreen()
{
	g_color = D3DCOLOR_RGBA(255,255,255,255);

	static int screenCount = 0;
	++screenCount;

	char fileName[32];
	wsprintf(fileName, "%d_final.bmp", screenCount);

	char fileNameLeft[32];
	wsprintf(fileNameLeft, "%d_left.bmp", screenCount);
	char fileNameRight[32];
	wsprintf(fileNameRight, "%d_right.bmp", screenCount);
	/*OutputDebugString(fileName);
	OutputDebugString("\n");

	D3DXSaveSurfaceToFile(fileName, D3DXIFF_BMP, backBuffer, NULL, NULL);*/

	D3DXSaveSurfaceToFile(fileNameLeft, D3DXIFF_BMP, leftSurface, NULL, NULL);
	D3DXSaveSurfaceToFile(fileNameRight, D3DXIFF_BMP, rightSurface, NULL, NULL);
	D3DXSaveSurfaceToFile(fileName, D3DXIFF_BMP, backBuffer, NULL, NULL);
}






void StereoView::ReleaseEverything()
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

	viewEffect->OnLostDevice();
		

	initialized = false;
}


void StereoView::PostReset()
{
	viewEffect->OnResetDevice();
}