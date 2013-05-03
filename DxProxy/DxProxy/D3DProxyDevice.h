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

#ifndef D3DPROXYDEVICE_H_INCLUDED
#define D3DPROXYDEVICE_H_INCLUDED

#include "Direct3DDevice9.h"
#include "Direct3DSurface9Vireio.h"
#include "ProxyHelper.h"
#include "StereoView.h"
#include "MotionTracker.h"
#include <d3dx9.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

class StereoView;

class D3DProxyDevice : public BaseDirect3DDevice9
{
public:
	D3DProxyDevice(IDirect3DDevice9* pDevice);
	virtual ~D3DProxyDevice();

	virtual void Init(ProxyHelper::ProxyConfig& cfg);
	
	virtual HRESULT WINAPI Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
	void SetupOptions(ProxyHelper::ProxyConfig& cfg);
	void SetupMatrices();
	void ComputeViewTranslation();
	void SetupText();
	void HandleControls(void);
	void HandleTracking(void);
	bool validRegister(UINT reg);
	
	
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	

	D3DXMATRIX matProjection;
	D3DXMATRIX matProjectionInv;
	D3DXMATRIX matViewTranslation;

	float* currentMatrix;

	// view translation settings
	int yaw_mode;			// 0 disabled, 1 enabled
	int pitch_mode;			// 0 disabled, 1 enabled
	int roll_mode;			// 0 disabled, 1 enabled
	int translation_mode;	// for head translation

	// Projection Matrix variables
	float n;	//Minimum z-value of the view volume
	float f;	//Maximum z-value of the view volume
	float l;	//Minimum x-value of the view volume
	float r;	//Maximum x-value of the view volume
	float t;	//Minimum y-value of the view volume
	float b;	//Maximum y-value of the view volume

	bool trackingOn;

	int eyeShutter;
	int game_type;
	float separation;
	float convergence;
	float aspect_multiplier;
	float yaw_multiplier;
	float pitch_multiplier;
	float roll_multiplier;
	float offset;
	float aspectRatio;
	bool swap_eyes;
	ProxyHelper::ProxyConfig config;
	int matrixIndex;
	bool saveDebugFile;
	std::ofstream debugFile;
	StereoView* stereoView;
	ID3DXFont *hudFont;

	time_t lastInputTime;

	MotionTracker* tracker;
	bool trackerInitialized;
	bool *m_keys;
	int SHOCT_mode;
	float centerlineR;
	float centerlineL;


	static enum ProxyTypes
	{
		MONO = 0,
		FIXED = 10,
		SOURCE = 100,
		SOURCE_L4D = 101,
		UNREAL = 200,
		UNREAL_MIRROR = 201,
		UNREAL_UT3 = 202,
		UNREAL_BIOSHOCK = 203,
		EGO = 300,
		EGO_DIRT = 301,
		REALV = 400,
		REALV_ARMA = 401,
		UNITY = 500,
		UNITY_SLENDER = 501,
		ADVANCED = 600,
		ADVANCED_SKYRIM = 601
	};


protected:
	
	

	enum EyeSide
	{
		Left = 1,
		Right = 2
	};


	
	virtual void OnCreateOrRestore();

	// Use to specify the side that you want to draw to
	void setDrawingSide(enum EyeSide side);

	Direct3DSurface9Vireio* pStereoBuffer;


private:

	

	// The render targets that are currently in use.
	std::vector<Direct3DSurface9Vireio*> m_activeRenderTargets;
	

	enum EyeSide m_currentRenderingSide;



	
	
};

#endif