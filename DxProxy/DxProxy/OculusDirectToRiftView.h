/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusRiftView.h> and
Class <OculusRiftView> :
Copyright (C) 2012 Andres Hernandez

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

#ifndef OCULUSDIRECTTORIFTVIEW_H_INCLUDED
#define OCULUSDIRECTTORIFTVIEW_H_INCLUDED

#include "StereoView.h"
#include "HMDisplayInfo.h"
#include "MotionTracker.h"

// Include DirectX
#include "Win32_DirectXAppUtil.h" 

// Include the Oculus SDK
#define   OVR_D3D_VERSION 11
#include "OVR_CAPI_D3D.h"
using namespace OVR;

struct OculusTextureSet;
class OculusTracker;
struct VoidScene;


/**
* Oculus direct-to-rift render class.
*/
class OculusDirectToRiftView : public StereoView
{
public:
	OculusDirectToRiftView(ProxyConfig *config, HMDisplayInfo *hmd, MotionTracker *tracker);
	virtual ~OculusDirectToRiftView();
	
	/*** OculusRiftView public methods ***/
	virtual void SetViewEffectInitialValues();
	virtual void PostViewEffectCleanup(); 
	virtual void CalculateShaderVariables( );
	virtual void InitShaderEffects();
	virtual void ReleaseEverything();
	virtual void SetVRMouseSquish(float squish);
	virtual void PrePresent(D3D9ProxySurface* stereoCapableSurface);
	virtual void PostPresent(D3D9ProxySurface* stereoCapableSurface);
	virtual std::string GetAdditionalFPSInfo();

	//This has to be public as it is called from the new thread method
	void DX11RenderThread_Main();

private:

	bool DX11RenderThread_Init();
	bool DX11RenderThread_InitDevice(Sizei sz);
	void DX11RenderThread_TimewarpLastFrame();
	void DX11RenderThread_ReleaseEverything();
	void DX11RenderThread_RenderNewFrame();

	enum ThreadEvents
	{
		NONE,
		TERMINATE_THREAD,
		RELEASE_EVERYTHING,
		NEW_FRAME
	};
	
	int m_eventFlag;

	std::mutex m_mtx;

	void SetEventFlag(ThreadEvents evt);
	void GetEventFlag(ThreadEvents &evt);

	//Flags for when the event flag has been reset
	HANDLE m_EventFlagProcessed;
	HANDLE m_EventFlagRaised;

	D3D9ProxySurface* m_stereoCapableSurface;

	void CalcFPS();
	float hmdFPS;

	/**
	* Predefined Oculus Rift Head Mounted Display info.
	* Contains distortionCoefficients, needed as vertex shader constants
	***/
	HMDisplayInfo *hmdInfo;

	/**
	***/
	OculusTracker *m_pOculusTracker;

	IDirect3DTexture9 *m_logoTexture;

	//The rift!
	ovrHmd rift;

	// Make the eye render buffers (caution if actual size < requested due to HW limits). 
	OculusTextureSet  * m_pEyeRenderTexture[2];
	DepthBuffer    * m_pEyeDepthBuffer[2];
	ovrEyeRenderDesc m_eyeRenderDesc[2];
    ovrPosef         m_EyeRenderPose[2];

	// Create the screen model, one per eye as they use different textures for the screen
	VoidScene *m_pScene[2];

	ovrRecti         eyeRenderViewport[2];
};

#endif //OCULUSDIRECTTORIFTVIEW_H_INCLUDED