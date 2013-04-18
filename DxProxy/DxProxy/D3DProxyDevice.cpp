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

#include "D3DProxyDevice.h"
#include "StereoViewFactory.h"
#include "MotionTrackerFactory.h"

#pragma comment(lib, "d3dx9.lib")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

D3DProxyDevice::D3DProxyDevice(IDirect3DDevice9* pDevice):BaseDirect3DDevice9(pDevice)
{
	OutputDebugString("D3D ProxyDev Created\n");
}

D3DProxyDevice::~D3DProxyDevice()
{
}

void D3DProxyDevice::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Init\n");
	stereoView = StereoViewFactory::Get(cfg);
	SetupMatrices(cfg);
	SetupOptions(cfg);
	SetupText();
}

void D3DProxyDevice::SetupOptions(ProxyHelper::ProxyConfig& cfg)
{
	separation = cfg.separation;
	convergence = cfg.convergence;
	game_type = cfg.game_type;
	aspect_multiplier = cfg.aspect_multiplier;
	yaw_multiplier = cfg.yaw_multiplier;
	pitch_multiplier = cfg.pitch_multiplier;
	roll_multiplier = cfg.roll_multiplier;
	config = cfg;
	eyeShutter = 1;
	matrixIndex = 0;
	offset = 0.0f;
	swap_eyes = false;
	saveDebugFile = false;
	trackerInitialized = false;

	if(yaw_multiplier == 0.0f) yaw_multiplier = 25.0f;
	if(pitch_multiplier == 0.0f) pitch_multiplier = 25.0f;
	if(roll_multiplier == 0.0f) roll_multiplier = 1.0f;

	char buf[64];
	LPCSTR psz = NULL;

	sprintf_s(buf, "type: %d, aspect: %f", cfg.game_type, cfg.aspect_multiplier);
	psz = buf;
	OutputDebugString(psz);
	OutputDebugString("\n");
}

void D3DProxyDevice::SetupMatrices(ProxyHelper::ProxyConfig& cfg)
{
	aspectRatio = (float)stereoView->viewport.Width/(float)stereoView->viewport.Height;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(90), aspectRatio, 1.0f, 100.0f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(90), aspectRatio*cfg.aspect_multiplier, 1.0f, 100.0f);
}

void D3DProxyDevice::SetupText()
{
	D3DXCreateFont( m_pDevice, 22, 0, FW_BOLD, 4, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &hudFont );
}

void D3DProxyDevice::HandleControls()
{
	float keySpeed = 0.00002f;
	float keySpeed2 = 0.0005f;
	float mouseSpeed = 0.25f;
	float rollSpeed = 0.01f;
	static int keyWaitCount = 0; 
	keyWaitCount--;
	static int saveWaitCount = 0; 
	saveWaitCount--;
	static bool doSaveNext = false;

	if(KEY_DOWN(VK_F1))
	{
		if(stereoView->initialized)
		{
			stereoView->SaveScreen();
		}
	}

	if(KEY_DOWN(VK_F2))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			offset -= keySpeed;
		} else 
		{
			separation -= keySpeed;
		}
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(VK_F3))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			offset += keySpeed;
		} 
		else 
		{
			separation += keySpeed;
		}
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_F4))
	{
		convergence -= keySpeed2;
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(VK_F5))
	{
		convergence += keySpeed2;
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_F6))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			separation = 0.0f;
			convergence = 0.0f;
			offset = 0.0f;
			yaw_multiplier = 25.0f;
			pitch_multiplier = 25.0f;
			roll_multiplier = 1.0f;
			//matrixIndex = 0;
			saveWaitCount = 500;
			doSaveNext = true;
		}
		else if(keyWaitCount <= 0)
		{
			swap_eyes = !swap_eyes;
			stereoView->SwapEyes(swap_eyes);
			keyWaitCount = 200;
		}
	}
	
	if(KEY_DOWN(VK_F7) && keyWaitCount <= 0)
	{
		matrixIndex++;
		if(matrixIndex > 15) 
		{
			matrixIndex = 0;
		}
		keyWaitCount = 200;
	}

	if(KEY_DOWN(VK_F8))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			pitch_multiplier -= mouseSpeed;
		}  
		else if(KEY_DOWN(VK_CONTROL))
		{
			roll_multiplier -= rollSpeed;
		}  
		else 
		{
			yaw_multiplier -= mouseSpeed;
		}

		if(trackerInitialized && tracker->isAvailable())
		{
			tracker->setMultipliers(yaw_multiplier, pitch_multiplier, roll_multiplier);
		}

		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(VK_F9))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			pitch_multiplier += mouseSpeed;
		}  
		else if(KEY_DOWN(VK_CONTROL))
		{
			roll_multiplier += rollSpeed;
		}  
		else 
		{
			yaw_multiplier += mouseSpeed;
		}

		if(trackerInitialized && tracker->isAvailable())
		{
			tracker->setMultipliers(yaw_multiplier, pitch_multiplier, roll_multiplier);
		}
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(saveDebugFile)
	{
		debugFile.close();
	}
	saveDebugFile = false;

	if(KEY_DOWN(VK_F12) && keyWaitCount <= 0)
	{
		// uncomment to save text debug file
		//saveDebugFile = true;
		keyWaitCount = 200;
	}

	if(doSaveNext && saveWaitCount < 0)
	{
		doSaveNext = false;
		ProxyHelper* helper = new ProxyHelper();
		helper->SaveProfile(separation, convergence, yaw_multiplier, pitch_multiplier, roll_multiplier);
	}
}

void D3DProxyDevice::HandleTracking()
{
	if(!trackerInitialized)
	{
		OutputDebugString("Try to init Tracker\n");
		tracker = MotionTrackerFactory::Get(config);
		trackerInitialized = true;
	}

	if(trackerInitialized && tracker->isAvailable())
	{
		tracker->updateOrientation();
	}
}

bool D3DProxyDevice::validRegister(UINT reg)
{
	return true;
}

