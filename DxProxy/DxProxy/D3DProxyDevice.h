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
#include "ProxyHelper.h"
#include "StereoView.h"
#include "MotionTracker.h"
#include <d3dx9.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

class StereoView;

class D3DProxyDevice : public BaseDirect3DDevice9
{
public:
	D3DProxyDevice(IDirect3DDevice9* pDevice);
	virtual ~D3DProxyDevice();

	virtual void Init(ProxyHelper::ProxyConfig& cfg);
	void SetupOptions(ProxyHelper::ProxyConfig& cfg);
	void SetupMatrices(ProxyHelper::ProxyConfig& cfg);
	void SetupText();
	void HandleControls(void);
	void HandleTracking(void);
	bool validRegister(UINT reg);

	D3DXMATRIX matProjection;
	D3DXMATRIX matProjectionInv;
	float* currentMatrix;

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

	MotionTracker* tracker;
	bool trackerInitialized;

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
};

#endif