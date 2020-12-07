/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusDirectToRiftView.cpp> and
Class <OculusDirectToRiftView> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

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

#include "OculusDirectToRiftView.h"
#include "StereoView.h"
#include "D3DProxyDevice.h"
#include "Resource.h"
#include "OculusTracker.h"

#define DLL_NAME "d3d9.dll"

//#define BLEND
//#define TIMING_LOG

//FPS Calculator

#define MAXSAMPLES 20

void OculusDirectToRiftView::CalcFPS()
{
	static bool init=false;
	static int tickindex=0;
	static LONGLONG ticksum=0;
	static LONGLONG ticklist[MAXSAMPLES];
	static LONGLONG prevTick;
	static LARGE_INTEGER perffreq;
	if (!init)
	{
		//Initialise - should only ever happen once
		memset(ticklist, 0, sizeof(LONGLONG) * MAXSAMPLES);
		QueryPerformanceFrequency(&perffreq);
		init=true;
	}

	//Get the new tick
	LARGE_INTEGER newtick;
	QueryPerformanceCounter(&newtick);
	
	ticksum -= ticklist[tickindex];
    ticksum += newtick.QuadPart - prevTick;
    ticklist[tickindex] = newtick.QuadPart - prevTick;
    tickindex = ++tickindex % MAXSAMPLES;
	prevTick = newtick.QuadPart;

	asyncFPS = (float)((double)MAXSAMPLES / ((double)ticksum / (double)perffreq.QuadPart));
}


void OculusDirectToRiftView::SetEventFlag(ThreadEvents evt)
{
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_eventFlag = evt;
	}

	//Signal that we have raised a flag, and then wait for indication it has been processed
	//Wait for at most 200ms, if we have waited that long, then something is clearly wrong
	SignalObjectAndWait(m_EventFlagRaised, m_EventFlagProcessed, 200, FALSE);
}

OculusDirectToRiftView::ThreadEvents OculusDirectToRiftView::GetEventFlag()
{
	ThreadEvents evt = NONE;
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		evt = m_eventFlag;
		m_eventFlag = NONE;
	}
	return evt;
}

DWORD DX11RenderThread(void *pParam)
{
	OculusDirectToRiftView *pThis = (OculusDirectToRiftView*)pParam;

	//Now we enter the main dx11 thread loop
	pThis->DX11RenderThread_Main();

	return 0;
}

//------------------------------------------------------------
// ovrSwapTextureSet wrapper class that also maintains the render target views
// needed for D3D11 rendering.
struct OculusTextureSet
{
    ovrSwapTextureSet      * TextureSet;
    ID3D11RenderTargetView * TexRtv[3];

    OculusTextureSet(ovrHmd hmd, Sizei size)
    {
        D3D11_TEXTURE2D_DESC dsDesc;
        dsDesc.Width            = size.w;
        dsDesc.Height           = size.h;
        dsDesc.MipLevels        = 1;
        dsDesc.ArraySize        = 1;
        dsDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
        dsDesc.SampleDesc.Count = 1;   // No multi-sampling allowed
        dsDesc.SampleDesc.Quality = 0;
        dsDesc.Usage            = D3D11_USAGE_DEFAULT;
        dsDesc.CPUAccessFlags   = 0;
        dsDesc.MiscFlags        = 0;
        dsDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        ovrHmd_CreateSwapTextureSetD3D11(hmd, DIRECTX.Device, &dsDesc, &TextureSet);
        for (int i = 0; i < TextureSet->TextureCount; ++i)
        {
            ovrD3D11Texture* tex = (ovrD3D11Texture*)&TextureSet->Textures[i];
            DIRECTX.Device->CreateRenderTargetView(tex->D3D11.pTexture, NULL, &TexRtv[i]);
        }
    }

    void AdvanceToNextTexture()
    {
        TextureSet->CurrentIndex = (TextureSet->CurrentIndex + 1) % TextureSet->TextureCount;
    }
    void Release(ovrHmd hmd)
    {
        ovrHmd_DestroySwapTextureSet(hmd, TextureSet);
    }
};

/**
* Constructor.
* @param config Game configuration.
* @param hmd Oculus Rift Head Mounted Display info.
***/ 
OculusDirectToRiftView::OculusDirectToRiftView(ProxyConfig *config, HMDisplayInfo *hmd, MotionTracker *motionTracker) : 
	StereoView(config),
	m_logoTexture(NULL),
	hmdInfo(hmd),
	m_pCurrentScene(NULL),
	m_pNextScene(NULL)
{
	SHOW_CALL("OculusDirectToRiftView::OculusDirectToRiftView()");

	m_pOculusTracker = static_cast<OculusTracker*>(motionTracker);
	rift = m_pOculusTracker->GetOVRHmd();

	mirrorTexture = nullptr;


	appFrameIndex.index = 0;

	m_EventFlagProcessed = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagProcessed");
	m_EventFlagRaised = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagRaised");

	HANDLE ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&DX11RenderThread, (void*)this, 0, NULL);
	//Bump the priority of the DX11 rendering thread
	SetThreadPriority(ThreadHandle, THREAD_PRIORITY_HIGHEST);

	//Now wait for the DX11 initialisation to complete
	DWORD result = WaitForSingleObject(m_EventFlagProcessed, 60000);
	if (result == WAIT_TIMEOUT)
	{
		vireio::debugf("TIMEOUT EXPIRED IN DX11 INITIALISATION");
		exit(87);
	}
	else
	{
		vireio::debugf("DX11 Thread Initialised and running...");
	}
}

OculusDirectToRiftView::~OculusDirectToRiftView()
{
	SHOW_CALL("OculusDirectToRiftView::~OculusDirectToRiftView()");

	//Terminate the rendering thread and wait for it to go away
	SetEventFlag(TERMINATE_THREAD);

	ovrHmd_DestroyMirrorTexture(rift, mirrorTexture);

	for (int eye = 0; eye < 2; eye++)
		m_pEyeRenderTexture[eye]->Release(rift);
}

bool OculusDirectToRiftView::DX11RenderThread_Init()
{
	bool initialized = DX11RenderThread_InitDevices(Sizei(rift->Resolution.w, rift->Resolution.h));
	VALIDATE(initialized, "Unable to initialize D3D11 device.");

	for (int eye = 0; eye < 2; eye++)
	{
		Sizei idealSize = ovrHmd_GetFovTextureSize(rift, (ovrEyeType)eye, rift->DefaultEyeFov[eye], 1.0f);
		m_pEyeRenderTexture[eye]      = new OculusTextureSet(rift, idealSize);
		m_pEyeDepthBuffer[eye]        = new DepthBuffer(DIRECTX.Device, idealSize);
		eyeRenderViewport[eye].Pos  = Vector2i(0, 0);
		eyeRenderViewport[eye].Size = idealSize;

		vireio::debugf("Eye: %i, Size(%i, %i)", eye, idealSize.h, idealSize.w);
	}	

	// Setup VR components, filling out description
	m_eyeRenderDesc[0] = ovrHmd_GetRenderDesc(rift, ovrEye_Left, rift->DefaultEyeFov[0]);
	m_eyeRenderDesc[1] = ovrHmd_GetRenderDesc(rift, ovrEye_Right, rift->DefaultEyeFov[1]);

	//Turn on the "queue ahead"
	//ovrHmd_SetBool(rift, "QueueAheadEnabled", false);

	//Turn on performance HUD if set in config.xml
	ovrHmd_SetInt(rift, "PerfHudMode", (int)config->PerfHudMode);

	//Let main thread know we are done initialising
	SetEvent(m_EventFlagProcessed);
		
	return initialized;
}

DXGI_FORMAT getDX11Format(D3DFORMAT dx9Format)
{
	switch (dx9Format)
	{
	case D3DFMT_A8R8G8B8:
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case D3DFMT_X8R8G8B8:
		return DXGI_FORMAT_B8G8R8X8_UNORM;
	case D3DFMT_R5G6B5:
		return DXGI_FORMAT_B5G6R5_UNORM;
	case D3DFMT_A1R5G5B5:
		return DXGI_FORMAT_B5G5R5A1_UNORM;
	case D3DFMT_A8B8G8R8	:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case D3DFMT_A16B16G16R16: 
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	}

	return DXGI_FORMAT_UNKNOWN;
}

bool OculusDirectToRiftView::DX11RenderThread_InitDevices(Sizei sz)
{
    IDXGIFactory * DXGIFactory;
    IDXGIAdapter * Adapter;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
        return(false);
    if (FAILED(DXGIFactory->EnumAdapters(0, &Adapter)))
        return(false);

	//Create the Main DX11 device for rendering to the rift
	{
		if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &DIRECTX.Device, NULL, &DIRECTX.Context)))
			return(false);

		// Main depth buffer
		DIRECTX.MainDepthBuffer = new DepthBuffer(DIRECTX.Device, sz);
		DIRECTX.Context->OMSetRenderTargets(1, &DIRECTX.BackBufferRT, DIRECTX.MainDepthBuffer->TexDsv);

		// Buffer for shader constants
		DIRECTX.UniformBufferGen = new DataBuffer(DIRECTX.Device, D3D11_BIND_CONSTANT_BUFFER, NULL, UNIFORM_DATA_SIZE);
		DIRECTX.Context->VSSetConstantBuffers(0, 1, &DIRECTX.UniformBufferGen->D3DBuffer);

#ifdef BLEND
		// Set a standard blend state, ie transparency from alpha
	    D3D11_BLEND_DESC bm;
		memset(&bm, 0, sizeof(bm));
		bm.RenderTarget[0].BlendEnable = TRUE;
		bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState * BlendState;
		DIRECTX.Device->CreateBlendState(&bm, &BlendState);
		DIRECTX.Context->OMSetBlendState(BlendState, NULL, 0xffffffff);
#endif 

		// Set max frame latency to 1
		IDXGIDevice1* DXGIDevice1 = NULL;
		HRESULT hr = DIRECTX.Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
		if (FAILED(hr) | (DXGIDevice1 == NULL)) return(false);
		DXGIDevice1->SetMaximumFrameLatency(1);
		DXGIDevice1->Release();
	}

	//Create a second DX11 device solely for copying textures
	{
		if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_copyDX11.Device, NULL, &m_copyDX11.Context)))
			return(false);

		// Main depth buffer
		m_copyDX11.MainDepthBuffer = new DepthBuffer(m_copyDX11.Device, sz);
		m_copyDX11.Context->OMSetRenderTargets(1, &m_copyDX11.BackBufferRT, m_copyDX11.MainDepthBuffer->TexDsv);

		// Buffer for shader constants
		m_copyDX11.UniformBufferGen = new DataBuffer(m_copyDX11.Device, D3D11_BIND_CONSTANT_BUFFER, NULL, UNIFORM_DATA_SIZE);
		m_copyDX11.Context->VSSetConstantBuffers(0, 1, &m_copyDX11.UniformBufferGen->D3DBuffer);

#ifdef BLEND
		// Set a standard blend state, ie transparency from alpha
	    D3D11_BLEND_DESC bm;
		memset(&bm, 0, sizeof(bm));
		bm.RenderTarget[0].BlendEnable = TRUE;
		bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState * BlendState;
		m_copyDX11.Device->CreateBlendState(&bm, &BlendState);
		m_copyDX11.Context->OMSetBlendState(BlendState, NULL, 0xffffffff);
#endif
	
		// Set max frame latency to 1
		IDXGIDevice1* DXGIDevice1 = NULL;
		HRESULT hr = m_copyDX11.Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
		if (FAILED(hr) | (DXGIDevice1 == NULL)) return(false);
		DXGIDevice1->SetMaximumFrameLatency(1);
		DXGIDevice1->Release();
	}

    return(true);
}

void OculusDirectToRiftView::ReleaseEverything()
{
	SHOW_CALL("OculusDirectToRiftView::ReleaseEverything()");

	//Release the texture we loaded, do this on this thread as it is a DX9 texture
	if (m_logoTexture)
	{
		m_logoTexture->Release();
		m_logoTexture = NULL;
	}

	//Set flag and wait
	SetEventFlag(RELEASE_EVERYTHING);

	//Call base class
	StereoView::ReleaseEverything();
}

void OculusDirectToRiftView::DX11RenderThread_ReleaseEverything()
{
	SHOW_CALL("OculusDirectToRiftView::DX11RenderThread_ReleaseEverything()");

	//Reset scenes
	m_unusedVireioVRScenePool.ReleaseEverything();
	m_safeSceneStore.ReleaseEverything();
	delete m_pCurrentScene;
	m_pCurrentScene = NULL;
	delete m_pNextScene;
	m_pNextScene = NULL;
}

std::string OculusDirectToRiftView::GetAdditionalFPSInfo()
{
	return vireio::retprintf("Async-Timewarp FPS: %.1f", asyncFPS);
}

VireioVRScene* OculusDirectToRiftView::ThreadSafeSceneStore::push(VireioVRScene* &vrScene)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	VireioVRScene* pReturnScene = NULL;
	if (m_VRScene != NULL)
		pReturnScene = m_VRScene;
	m_VRScene = vrScene;
	return pReturnScene;
}

VireioVRScene* OculusDirectToRiftView::ThreadSafeSceneStore::retrieve(bool remove)
{
	std::lock_guard<std::mutex> lock(m_mtx);

	//Doesn't matter if m_VRScene is NULL
	VireioVRScene *vrScene = m_VRScene;
	if (remove)
		m_VRScene = NULL;
	return vrScene;
}

void OculusDirectToRiftView::ThreadSafeSceneStore::ReleaseEverything()
{
	std::lock_guard<std::mutex> lock(m_mtx);
	delete m_VRScene;
	m_VRScene = NULL;
}


void OculusDirectToRiftView::DX11RenderThread_RenderNextFrame()
{
	//Removed this one, as it was swamping the log!
	//SHOW_CALL("OculusDirectToRiftView::DX11RenderThread_RenderNextFrame()");

	//If not using SDK pose pred, then just take from the store, if we are then we'll just get 
	//a pointer to the scene to check if it is time to submit it yet
	VireioVRScene *vrScene = m_safeSceneStore.retrieve(true);
	if (vrScene)
	{
		//Get the frame HMD data for this scene from the tracker
		m_pOculusTracker->GetFrameHMDData(vrScene->frameIndex, vrScene->m_frameTiming, vrScene->m_trackingState);

		//We are now finished with the previous scene, so put it on the unused pool
		if (m_pCurrentScene)
			m_unusedVireioVRScenePool.push(m_pCurrentScene);

		//Save this as the current VR scene (for future timewarping)
		m_pCurrentScene = vrScene;

		//Can now cleanup the tracker, we have all we need for this scene
		m_pOculusTracker->CleanupFrameHMDData(vrScene->frameIndex);

		// Get both eye poses simultaneously, with IPD offset already included. 
		ovrVector3f      HmdToEyeViewOffset[2] = { m_eyeRenderDesc[0].HmdToEyeViewOffset,
													m_eyeRenderDesc[1].HmdToEyeViewOffset };

		//If DSV is not enabled, then we don't want to use stereo separation, since this is already done
		//in the DX9 rendering, simply glide the IPD separation in and out as we change from DSV rather than a sudden switch
		HmdToEyeViewOffset[ovrEye_Left].x *= m_screenViewGlideFactor;
		HmdToEyeViewOffset[ovrEye_Right].x *= m_screenViewGlideFactor;

		ovr_CalcEyePoses(vrScene->m_trackingState.HeadPose.ThePose, HmdToEyeViewOffset, vrScene->m_eyePoses);

		// Initialise Eye Buffers
		for (int eye = 0; eye < 2; eye++)
		{
			// Increment to use next texture, just before writing
			m_pEyeRenderTexture[eye]->AdvanceToNextTexture();

			// Clear and set up rendertarget
			int texIndex = m_pEyeRenderTexture[eye]->TextureSet->CurrentIndex;
			DIRECTX.SetAndClearRenderTarget(m_pEyeRenderTexture[eye]->TexRtv[texIndex], m_pEyeDepthBuffer[eye]);
			DIRECTX.SetViewport(Recti(eyeRenderViewport[eye]));

			// View and projection matrices for the main camera
			Camera mainCam(Vector3f(0.0f, 0.15f - config->YOffset, (2.25f - ZoomOutScale) + (m_screenViewGlideFactor > 0.5f ? 1.2f : 0.0f)), Matrix4f::Identity());

			// View and projection matrices for the camera using HMD orientation
			Camera finalCam(mainCam.Pos + mainCam.Rot.Transform(vrScene->m_eyePoses[eye].Position),
				mainCam.Rot * Matrix4f(vrScene->m_eyePoses[eye].Orientation));

			Matrix4f view = (m_screenViewGlideFactor >= 0.5f) ? finalCam.GetViewMatrix() : mainCam.GetViewMatrix();
			Matrix4f proj = ovrMatrix4f_Projection(m_eyeRenderDesc[eye].Fov, 0.2f, 1000.0f, ovrProjection_RightHanded);

			float fade = fabs(m_screenViewGlideFactor - 0.5f) * 2.0f;
		
			vrScene->m_pScene[eye]->Render(proj*view, fade, fade, fade, 1, true);
		}
	}

	//If we've never had a scene before, just return
	if (!m_pCurrentScene)
		return;

    // Initialize our single full screen Fov layer.
    ovrLayerEyeFov ld;
    ld.Header.Type  = ovrLayerType_EyeFov;
    ld.Header.Flags = 0;

	for (int eye = 0; eye < 2; eye++)
	{
		ld.ColorTexture[eye] = m_pEyeRenderTexture[eye]->TextureSet;
		ld.Viewport[eye]     = eyeRenderViewport[eye];
		ld.Fov[eye]          = rift->DefaultEyeFov[eye];
		ld.RenderPose[eye]   = m_pCurrentScene->m_eyePoses[eye];
	}

    ovrLayerHeader* layers = &ld.Header;
	if (m_pOculusTracker->useSDKPosePrediction)
	{
#ifdef TIMING_LOG
		vireio::debugf("Submitting Scene ID: %u, Frame Index: %u at time: %0.8f, Tracking State read time: %0.8f", m_pCurrentScene->sceneID, 
			m_pCurrentScene->frameIndex, ovr_GetTimeInSeconds(), m_pCurrentScene->m_trackingState.RawSensorData.TimeInSeconds);
#endif
		ovrHmd_SubmitFrame(rift, m_pCurrentScene->frameIndex, nullptr, &layers, 1);
	}
	else
	{
		ovrHmd_SubmitFrame(rift, 0, nullptr, &layers, 1);
	}
	

	CalcFPS();
}

void OculusDirectToRiftView::PostPresent(D3D9ProxySurface* stereoCapableSurface, D3DProxyDevice* pProxyDevice)
{
	//Removed this one as it was swamping the log
	//SHOW_CALL("OculusDirectToRiftView::PostPresent()");

	// Get the "next" scene
	VireioVRScene *pVRScene = m_pNextScene == NULL ? m_unusedVireioVRScenePool.pop() : m_pNextScene;

	if (pVRScene)
	{
		//artificially create a delay before we copy (configurable)
		if (config->sleep > 0)
			Sleep(config->sleep);

		//We'll use this to wait for GPU to finish the copy
		D3D11_QUERY_DESC QueryDesc;
		QueryDesc.Query = D3D11_QUERY_EVENT;
		QueryDesc.MiscFlags = 0;
		ID3D11Query *pEventQuery = NULL;
		m_copyDX11.Device->CreateQuery( &QueryDesc, &pEventQuery);

		for (int eye = 0; eye < 2; eye++)
		{
			ID3D11Resource *tempResource11 = NULL;
			HANDLE textHandle = (eye == (!config->swap_eyes ? ovrEye_Left : ovrEye_Right)) ? stereoCapableSurface->getHandleLeft() : stereoCapableSurface->getHandleRight();

			IID iid = __uuidof(ID3D11Resource);
			if (FAILED(m_copyDX11.Device->OpenSharedResource(textHandle, iid, (void**)(&tempResource11))))
			{
				vireio::debugf("Failed to open shared resource");
				return;
			}

			//QI for the texture
			ID3D11Texture2D *pDX9Texture = NULL;
			tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&pDX9Texture)); 
			tempResource11->Release();

			//Copy 
			{
				if (pVRScene->m_pScene[eye] == NULL)
				{
					//Create local instances of the textures and then copy the data into them
					D3D11_TEXTURE2D_DESC dsDesc;
					pDX9Texture->GetDesc(&dsDesc);

					ID3D11Texture2D *pDX11Texture = NULL;
					HRESULT hr = m_copyDX11.Device->CreateTexture2D(&dsDesc, NULL, &pDX11Texture);
					if (SUCCEEDED(hr))
					{
						//Need to get the shared handle
						IDXGIResource *pIDXGIResource = NULL;
						pDX11Texture->QueryInterface(__uuidof(IDXGIResource), (void**)(&pIDXGIResource)); 
						HANDLE sharedHandle = NULL;
						hr = pIDXGIResource->GetSharedHandle(&sharedHandle);
						pIDXGIResource->Release();

						if (SUCCEEDED(hr) && sharedHandle != NULL)
						{
							//Create the void scene for this eye, setting the DX9 shared texture as the screen source
							//Aspect ratio is a bit of a guess, but this figure seems to reduce the warping effect considerably
							pVRScene->m_pScene[eye] = new VoidScene(pDX11Texture, sharedHandle, config->fAspectMultiplier);
						}
						else
						{
							vireio::debugf("Failed to get shared handle for Dx11 texture: %0.8x", hr);
							//Just return, can't do anything else
							return;
						}
					}
					else
					{
						vireio::debugf("Failed to create Dx11 texture: %0.8x", hr);
						//Just return, can't do anything else
						return;
					}
				}

				//Copy
				m_copyDX11.Context->CopyResource(pVRScene->m_pScene[eye]->m_pTexture, pDX9Texture);
				m_copyDX11.Context->Flush();
			}

			//We don't need to keep the ref here now
			pDX9Texture->Release();
		}

		//FLsuh all commands, then wait for completion
		m_copyDX11.Context->End(pEventQuery);

		//Now wait for the flush to complete
		while( m_copyDX11.Context->GetData( pEventQuery, NULL, 0, 0 ) == S_FALSE )
		{
			//Waiting.. - just spin the CPU, not a lot else we can do
			//We have to wait for this copy to complete, or we get strange flicker
		}
		//All done
		pEventQuery->Release();
	}

	//Now push the new scene into our safe store - This will indicate to the DX11 thread a new scene is available
	VireioVRScene* pOldScene = m_safeSceneStore.push(pVRScene);
	if (pOldScene)
	{
		//If we had an older scene which we've superceded, just push this into the unused pool
		m_unusedVireioVRScenePool.push(pOldScene);
	}

	//Only enter this code if we aren't using pose prediction, or if it is the first scene of this frame
//	if (!m_pOculusTracker->useSDKPosePrediction || !pOldScene)
	{
		//get the frame timing for the new scene we are creating
		//and provide to the tracker
		m_pNextScene = m_unusedVireioVRScenePool.pop();
		//Set the correct frame index
		{
			//std::lock_guard<std::mutex> lock(appFrameIndex.m_mtx);
			m_pNextScene->frameIndex = ++appFrameIndex.index;

			/**********************************
			**  FRAME TIMING LOGIC
			**********************************/

			//Now we get the orientation prediction for the next scene
			ovrFrameTiming frameTiming = ovrHmd_GetFrameTiming(rift, appFrameIndex.index);
			ovrTrackingState trackingState;
			if (m_pOculusTracker->useSDKPosePrediction)
				trackingState = ovrHmd_GetTrackingState(rift, frameTiming.DisplayMidpointSeconds);
			else
				//This will just get the position for now
				trackingState = ovrHmd_GetTrackingState(rift, ovr_GetTimeInSeconds());

			//Write out timing info - might reveal something!
#ifdef TIMING_LOG
			vireio::debugf("**********  START *****************"); 
			vireio::debugf("appFrameIndex.index = %u", appFrameIndex.index); 
			vireio::debugf("ftiming.DisplayMidpointSeconds = %0.8f", frameTiming.DisplayMidpointSeconds); 
			vireio::debugf("ftiming.AppFrameIndex = %u", frameTiming.AppFrameIndex); 
			vireio::debugf("ftiming.DisplayFrameIndex = %u", frameTiming.DisplayFrameIndex); 
			vireio::debugf("ovr_GetTimeInSeconds() = %0.8f", ovr_GetTimeInSeconds()); 
			vireio::debugf("lastFrameTime (ms) = %0.8f", pProxyDevice->getLastFrameTime() * 1000.0f); 
#endif
			m_pOculusTracker->SetFrameHMDData(appFrameIndex.index, frameTiming, trackingState);
		}
	}
}

void OculusDirectToRiftView::PrePresent(D3D9ProxySurface* stereoCapableSurface)
{
	//Switch off for now
	//SHOW_CALL("OculusDirectToRiftView::PrePresent()");

	//Screen output
	IDirect3DSurface9* leftImage = stereoCapableSurface->getActualLeft();
	m_pActualDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	if ((int)config->mirror_mode == 1)
	{
		if (!mirrorTexture)
		{
			m_pActualDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
			D3DSURFACE_DESC desc;
			backBuffer->GetDesc(&desc);
			backBuffer->Release();

			// Create a mirror, to see Rift output on a monitor
			D3D11_TEXTURE2D_DESC td = {};
			td.ArraySize = 1;
			td.Format = getDX11Format(desc.Format);
			td.Width = desc.Width;
			td.Height = desc.Height;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
			td.SampleDesc.Count = 1;
			td.MipLevels = 1;
			ovrResult res = ovrHmd_CreateMirrorTextureD3D11(rift, DIRECTX.Device, &td, &mirrorTexture);
			if (OVR_FAILURE(res))
			{
				vireio::debugf("Failed to create mirror texture");
				config->mirror_mode = 0;
			}
		}

		//Switch on mirroring mode
		if (mirrorTexture)
		{
			D3DSURFACE_DESC desc;
			backBuffer->GetDesc(&desc);

			HANDLE sharedHandle = NULL;
			IDXGIResource* pDXGIResource = NULL;
			ovrD3D11Texture* tex = (ovrD3D11Texture*)mirrorTexture;
			tex->D3D11.pTexture->QueryInterface(__uuidof(IDXGIResource), (LPVOID*) &pDXGIResource);
			if (FAILED(pDXGIResource->GetSharedHandle(&sharedHandle)))
			{
				vireio::debugf("Failed to get shared handle");
				return;
			}
			pDXGIResource->Release();

			IDirect3DTexture9 *pD3D9_DX11Texture = NULL;
			IDirect3DDevice9Ex *pDirect3DDevice9Ex = NULL;
			if (SUCCEEDED(m_pActualDevice->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&pDirect3DDevice9Ex))))
			{
				HRESULT hr = pDirect3DDevice9Ex->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT, &pD3D9_DX11Texture, &sharedHandle);
				if (FAILED(hr))
				{
					vireio::debugf("Failed m_pActualDevice->CreateTexture = %0.8x", hr);
					//Just do the old copy
					m_pActualDevice->StretchRect(leftImage, NULL, backBuffer, NULL, D3DTEXF_NONE);
					return;
				}
			
				IDirect3DSurface9 *pSurface = NULL;
				pD3D9_DX11Texture->GetSurfaceLevel(0, &pSurface);
				m_pActualDevice->StretchRect(pSurface, NULL, backBuffer, NULL, D3DTEXF_NONE);
				pD3D9_DX11Texture->Release();
				pSurface->Release();
				pDirect3DDevice9Ex->Release();
			}
		}
	}
	else
	{
		//Simple, just copy the left image!
		m_pActualDevice->StretchRect(leftImage, NULL, backBuffer, NULL, D3DTEXF_NONE);
	}

	//Draw the Vireio logo to the top left of the screen
	if (!m_logoTexture)
		D3DXCreateTextureFromResource(m_pActualDevice, GetModuleHandle(DLL_NAME), MAKEINTRESOURCE(IDB_IMAGE), &m_logoTexture);

	if (m_logoTexture)
	{
		IDirect3DSurface9 *pSurface = NULL;
		if (SUCCEEDED(m_logoTexture->GetSurfaceLevel(0, &pSurface)))
		{
			RECT r;
			r.left = 0; r.top = 0; r.right = 350; r.bottom = 75;
			m_pActualDevice->StretchRect(pSurface, NULL, backBuffer, &r, D3DTEXF_NONE);
			pSurface->Release();
		}
	}

	backBuffer->Release();
}


void OculusDirectToRiftView::DX11RenderThread_Main()
{
	//First thing we do is initialise
	DX11RenderThread_Init();

	ThreadEvents eventFlag = NONE;
	while (eventFlag != TERMINATE_THREAD)
	{
		//Adjust screen view glide factor on this thread, then it will be consistent regardless of DX9 framerate
		if (!m_disconnectedScreenView)
		{
			if (m_screenViewGlideFactor > 0.0f)
				m_screenViewGlideFactor -= 0.05f;
		}
		else
		{
			if (m_screenViewGlideFactor < 1.0f)
				m_screenViewGlideFactor += 0.05f;
		}

		//Check for non-rendering flag (such as terminate or release), this will return immediately if
		//there isn't one reaised
		DWORD result = WaitForSingleObject(m_EventFlagRaised, 0);
		if (result == WAIT_TIMEOUT)
		{
			//This will just do the timewarp if there's no new frame
			DX11RenderThread_RenderNextFrame();
		}
		else
		{
			eventFlag = GetEventFlag();
			switch (eventFlag)
			{
			case NONE:
				{
					vireio::debugf("NONE event flag from a signalled state - SHOULD NOT BE POSSIBLE");
				}
				break;
			case RELEASE_EVERYTHING:
				{
					DX11RenderThread_ReleaseEverything();
				}
				break;
			case TERMINATE_THREAD:
				{
					vireio::debugf("TERMINATE_THREAD event flag");
				}
				break;
			default:
				//Do nothing here
				break;
			}

			//Signal the DX9 thread can now continue		
			SetEvent(m_EventFlagProcessed);
		}
	}
}

void OculusDirectToRiftView::SetViewEffectInitialValues() 
{
	SHOW_CALL("OculusDirectToRiftView::SetViewEffectInitialValues\n");
}

void OculusDirectToRiftView::PostViewEffectCleanup()
{
	SHOW_CALL("OculusDirectToRiftView::PostViewEffectCleanup()");
}

void OculusDirectToRiftView::SetVRMouseSquish(float squish)
{
	SHOW_CALL("OculusDirectToRiftView::SetVRMouseSquish()");
}

/**
* Calculate all vertex shader constants.
***/ 
void OculusDirectToRiftView::CalculateShaderVariables()
{
	SHOW_CALL("OculusDirectToRiftView::CalculateShaderVariables");
}

/**
* Loads Oculus Rift shader effect files.
***/ 
void OculusDirectToRiftView::InitShaderEffects()
{
	SHOW_CALL("OculusDirectToRiftView::InitShaderEffects");
}


