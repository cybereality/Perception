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

#ifndef STEREOVIEW_H_INCLUDED
#define STEREOVIEW_H_INCLUDED

#include "ProxyHelper.h"
#include "D3DProxyDevice.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string.h>

class StereoView
{
public:
	static enum StereoTypes
	{
		DISABLED = 0,
		ANAGLYPH_RED_CYAN = 1,
		ANAGLYPH_RED_CYAN_GRAY = 2,
		ANAGLYPH_YELLOW_BLUE = 5,
		ANAGLYPH_YELLOW_BLUE_GRAY = 6,
		ANAGLYPH_GREEN_MAGENTA = 10,
		ANAGLYPH_GREEN_MAGENTA_GRAY = 11,
		SIDE_BY_SIDE = 20,
		DIY_RIFT = 25,
		OCULUS_RIFT = 26,
		OCULUS_RIFT_CROPPED = 27,
		OVER_UNDER = 30,
		INTERLEAVE_HORZ = 40,
		INTERLEAVE_VERT = 50,
		CHECKERBOARD = 60
	};

	static enum Eyes
	{
		LEFT_EYE,
		RIGHT_EYE
	};

	StereoView(ProxyHelper::ProxyConfig& config);
	virtual ~StereoView();
	virtual void Init(IDirect3DDevice9* dev);
	virtual void InitTextureBuffers();
	virtual void InitVertexBuffers();
	virtual void InitShaderEffects();
	virtual void Draw();
	virtual void SaveState();
	virtual void SetState();
	virtual void RestoreState();
	virtual void UpdateEye(int eye);
	virtual void SaveScreen();
	virtual void SwapEyes(bool doSwap);
	virtual void Reset();

	IDirect3DDevice9* device;

	D3DVIEWPORT9 viewport;

	IDirect3DSurface9* backBuffer;
	IDirect3DTexture9* leftTexture;
	IDirect3DTexture9* rightTexture;
	IDirect3DTexture9* screenTexture;
	IDirect3DSurface9* leftSurface;
	IDirect3DSurface9* rightSurface;
	IDirect3DSurface9* screenSurface;

	IDirect3DVertexBuffer9* screenVertexBuffer;

	DWORD tssColorOp;
	DWORD tssColorArg1;
	DWORD tssAlphaOp;
	DWORD tssAlphaArg1;
	DWORD tssConstant;

	DWORD rsAlphaEnable;
	DWORD rsZEnable;
	DWORD rsZWriteEnable;
	DWORD rsDepthBias;
	DWORD rsSlopeScaleDepthBias;
	DWORD rsSrgbEnable;

	DWORD ssSrgb;
	DWORD ssSrgb1;
	DWORD ssAddressU;
	DWORD ssAddressV;
	DWORD ssAddressW;
	DWORD ssMag0;
	DWORD ssMag1;
	DWORD ssMin0;
	DWORD ssMin1;
	DWORD ssMip0;
	DWORD ssMip1;

	IDirect3DVertexShader9* lastVertexShader;
	IDirect3DPixelShader9* lastPixelShader;
	IDirect3DBaseTexture9* lastTexture;
	IDirect3DBaseTexture9* lastTexture1;
	IDirect3DVertexDeclaration9* lastVertexDeclaration;

	IDirect3DSurface9* lastRenderTarget0;
	IDirect3DSurface9* lastRenderTarget1;

	ID3DXEffect* viewEffect;

	bool initialized;
	int game_type;
	int stereo_mode;
	bool swap_eyes;

	std::map<int, std::string> shaderEffect;

	float DistortionScale;	// used by OculusRiftView and D3DProxyDevice
};

const DWORD D3DFVF_TEXVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

struct TEXVERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	float u;
	float v;
};

#endif