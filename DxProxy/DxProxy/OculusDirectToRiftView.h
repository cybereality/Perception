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


//An extension to the Texture Class provided in the OVR Samples
struct VireioTexture : public Texture
{
    VireioTexture(ID3D11Texture2D *pTexture, bool rendertarget, Sizei size) :
		Texture(rendertarget, size)
    {
		SHOW_CALL("VireioTexture()");

		cleanup(rendertarget);		
		Tex = pTexture;

        DIRECTX.Device->CreateShaderResourceView(pTexture, NULL, &TexSv);
		if (rendertarget) DIRECTX.Device->CreateRenderTargetView(Tex, NULL, &TexRtv);
   }

	//sort of destructor
	void cleanup(bool rendertarget)
	{
		//Release the texture created by the base class
		if(Tex)
		{
			Tex->Release();
			Tex = NULL;
		}

		if(rendertarget)
		{
			TexRtv->Release();
			TexRtv = NULL;
		}

		if(TexSv)
		{
			TexSv->Release();
			TexSv = NULL;
		}
	}
};

//------------------------------------------------------------------------- 
struct VoidScene  
{
    Model * screen;
	VireioTexture *texture;

 	VoidScene(ID3D11Texture2D *pTexture, HANDLE sharedHandle, float aspect) :
		m_pTexture(pTexture),
		m_sharedHandle(sharedHandle),
		m_pSharedTexture(NULL),
		m_aspect(aspect)
    {
		SHOW_CALL("VoidScene::VoidScene()");
    }

	~VoidScene()
	{
		SHOW_CALL("VoidScene::~VoidScene()");
		m_pSharedTexture->Release();
		texture->cleanup(false);
		delete texture;
		delete screen;
	}

   void Render(Matrix4f projView, float R, float G, float B, float A, bool standardUniforms)
    {   
		SHOW_CALL("VoidScene::Render()");

		//Before we render, we need to get the texture from the shader resource if
		//we haven't already done this
		if (m_pSharedTexture == NULL)
		{
			ID3D11Resource *tempResource11 = NULL;
			IID iid = __uuidof(ID3D11Resource);
			if (FAILED(DIRECTX.Device->OpenSharedResource(m_sharedHandle, iid, (void**)(&tempResource11))))
			{
				vireio::debugf("Failed to open shared DX11 resource, HANDLE=%0.8x", m_sharedHandle);
				return;
			}

			//QI for the texture
			tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_pSharedTexture)); 
			tempResource11->Release();

			//Now we can realise the scene
			D3D11_TEXTURE2D_DESC desc;
			m_pSharedTexture->GetDesc(&desc);
#if _DEBUG
			vireio::debugf("Texture Format: %i", desc.Format);
#endif

			float aspect = (m_aspect == 0.0f) ? (float)(desc.Width) / (float)(desc.Height) : m_aspect;
			texture = new VireioTexture(m_pSharedTexture, false, Sizei(desc.Width, desc.Height));
			screen = new Model(texture, -2.0f, -2.0f / aspect, 2.0f, 2.0f / aspect);
		}

		screen->Render(projView,R,G,B,A,standardUniforms);    
	}


	ID3D11Texture2D *m_pTexture;
	ID3D11Texture2D *m_pSharedTexture;
	HANDLE m_sharedHandle;
	float m_aspect;
};

//This object will contain the scene for both eyes (with each texture) and the tracking state taken at the time the scene was generated
struct VireioVRScene
{
	VireioVRScene(UINT id)
	{
		SHOW_CALL("VireioVRScene()");
		m_pScene[ovrEye_Left] = NULL;
		m_pScene[ovrEye_Right] = NULL;
		sceneID = id;
	}

	~VireioVRScene()
	{
		SHOW_CALL("~VireioVRScene()");
		delete m_pScene[ovrEye_Left];
		delete m_pScene[ovrEye_Right];
	}

	VoidScene*	m_pScene[2];
	ovrTrackingState m_trackingState;
	ovrFrameTiming m_frameTiming;
	UINT frameIndex;
	UINT sceneID;
	ovrPosef m_eyePoses[2];
};

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
	virtual void PostPresent(D3D9ProxySurface* stereoCapableSurface, D3DProxyDevice* pProxyDevice);
	virtual std::string GetAdditionalFPSInfo();

	//This has to be public as it is called from the new thread method
	void DX11RenderThread_Main();

private:

	bool DX11RenderThread_Init();
	bool DX11RenderThread_InitDevices(Sizei sz);
	void DX11RenderThread_TimewarpLastFrame();
	void DX11RenderThread_ReleaseEverything();
	void DX11RenderThread_RenderNextFrame();

	enum ThreadEvents
	{
		NONE,
		TERMINATE_THREAD,
		RELEASE_EVERYTHING
	} m_eventFlag;

	std::mutex m_mtx;

	void SetEventFlag(ThreadEvents evt);
	ThreadEvents GetEventFlag();

	//Flags for when the event flag has been reset
	HANDLE m_EventFlagProcessed;
	HANDLE m_EventFlagRaised;

	void CalcFPS();
	float asyncFPS;

	struct ThreadSafeSceneStore
	{
		ThreadSafeSceneStore() {m_VRScene = NULL;}

		VireioVRScene* push(VireioVRScene* &eyeScenes);
		VireioVRScene* retrieve(bool remove);

		void ReleaseEverything();

	private:
		std::mutex m_mtx;
		VireioVRScene* m_VRScene;
	} m_safeSceneStore;

	struct ThreadSafePool : private std::deque<VireioVRScene*>
	{
		ThreadSafePool() {}

		void push(VireioVRScene* &vrScene)
		{
			std::lock_guard<std::mutex> lck(m_mtx);
			push_back(vrScene);
		}
		VireioVRScene* pop()
		{
			std::lock_guard<std::mutex> lck(m_mtx);
			if (size() == 0)
			{
				static UINT sceneID = 1;
				return new VireioVRScene(sceneID++);
			}

			VireioVRScene *vrScene = front();
			pop_front();
			return vrScene;
		}
		
		static void deleter(VireioVRScene* p) {delete p;}
		void ReleaseEverything()
		{
			std::for_each(begin(), end(), deleter);
			clear();
		}

	private:
		std::mutex m_mtx;
	} m_unusedVireioVRScenePool;

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

	UINT appFrameIndex;

	//The last scene we showed (just kept for eye poses)
	VireioVRScene *m_pCurrentScene;

	//Second DX11 device for copying textures
	DirectX11 m_copyDX11;

	// Make the eye render buffers (caution if actual size < requested due to HW limits). 
	OculusTextureSet  * m_pEyeRenderTexture[2];
	DepthBuffer    * m_pEyeDepthBuffer[2];
	ovrEyeRenderDesc m_eyeRenderDesc[2];
	ovrRecti         eyeRenderViewport[2];
};

#endif //OCULUSDIRECTTORIFTVIEW_H_INCLUDED