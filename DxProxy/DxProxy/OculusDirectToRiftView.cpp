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

//FPS Calculator

#define MAXSAMPLES 10

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

	hmdFPS = (float)((double)MAXSAMPLES / ((double)ticksum / (double)perffreq.QuadPart));
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
	m_pLastScene(NULL)
{
	SHOW_CALL("OculusDirectToRiftView::OculusDirectToRiftView()");

	m_pOculusTracker = static_cast<OculusTracker*>(motionTracker);
	rift = m_pOculusTracker->GetOVRHmd();

	m_EventFlagProcessed = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagProcessed");
	m_EventFlagRaised = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagRaised");

	HANDLE ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&DX11RenderThread, (void*)this, 0, NULL);
	//Bump the priority of the DX11 rendering thread
	SetThreadPriority(ThreadHandle, THREAD_PRIORITY_HIGHEST);
}

OculusDirectToRiftView::~OculusDirectToRiftView()
{
	SHOW_CALL("OculusDirectToRiftView::~OculusDirectToRiftView()");


	//Terminate the rendering thread and wait for it to go away
	SetEventFlag(TERMINATE_THREAD);
}

bool OculusDirectToRiftView::DX11RenderThread_Init()
{
	bool initialized = DX11RenderThread_InitDevices(Sizei(hmdInfo->GetResolution().first, hmdInfo->GetResolution().second));
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
	ovrHmd_SetBool(rift, "QueueAheadEnabled", true);

	//Turn on performance HUD if set in config.xml
	ovrHmd_SetInt(rift, "PerfHudMode", (int)config->PerfHudMode);
		
	return initialized;
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

	//CReate a second DX11 device solely for copying textures
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
	m_safeSceneStore.ReleaseEverything();
}

std::string OculusDirectToRiftView::GetAdditionalFPSInfo()
{
	return vireio::retprintf("Async-Timewarp FPS: %.1f", hmdFPS);
}

void OculusDirectToRiftView::ThreadSafeSceneStore::push(VireioVRScene* &vrScene)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	if (m_VRScene != NULL)
	{
		delete m_VRScene;
	}
	m_used = true;
	m_VRScene = vrScene;
}

bool OculusDirectToRiftView::ThreadSafeSceneStore::hasScene()
{
	std::lock_guard<std::mutex> lock(m_mtx);
	return (m_VRScene != NULL);
}

VireioVRScene* OculusDirectToRiftView::ThreadSafeSceneStore::retrieve()
{
	std::lock_guard<std::mutex> lock(m_mtx);

	//Doesn't matter if m_VRScene is NULL
	VireioVRScene *vrScene = m_VRScene;
	m_VRScene = NULL;
	return vrScene;
}

bool OculusDirectToRiftView::ThreadSafeSceneStore::getUsed()
{
	bool used = false;
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		used = m_used;
	}
	return used;
}

void OculusDirectToRiftView::ThreadSafeSceneStore::ReleaseEverything()
{
	std::lock_guard<std::mutex> lock(m_mtx);
	delete m_VRScene;
	m_VRScene = NULL;
	m_used = false;
}


void OculusDirectToRiftView::DX11RenderThread_RenderNextFrame()
{
	SHOW_CALL("OculusDirectToRiftView::DX11RenderThread_RenderNextFrame()");

	//If we've never put anything in the store, then we don't have a next frame to render or timewarp
	if (!m_safeSceneStore.getUsed())
		return;

	VireioVRScene *vrScene = m_safeSceneStore.retrieve();
	if (vrScene != NULL)
	{
		//Save this as the last scene (for future timewarping)
		m_pLastScene = vrScene;

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
			Camera mainCam(Vector3f(0.0f, 0.1f - config->YOffset, (2.25f - ZoomOutScale) + (m_screenViewGlideFactor > 0.5f ? 1.2f : 0.0f)), Matrix4f::Identity());

			// View and projection matrices for the camera using HMD orientation
			Camera finalCam(mainCam.Pos + mainCam.Rot.Transform(vrScene->m_eyePoses[eye].Position),
				mainCam.Rot * Matrix4f(vrScene->m_eyePoses[eye].Orientation));

			Matrix4f view = (m_screenViewGlideFactor >= 0.5f) ? finalCam.GetViewMatrix() : mainCam.GetViewMatrix();
			Matrix4f proj = ovrMatrix4f_Projection(m_eyeRenderDesc[eye].Fov, 0.2f, 1000.0f, ovrProjection_RightHanded);

			float fade = fabs(m_screenViewGlideFactor - 0.5f) * 2.0f;
		
			vrScene->m_pScene[eye]->Render(proj*view, fade, fade, fade, 1, true);
		}

		//Clean up
		m_unusedVireioVRScenePool.push(vrScene);
	}
	else
	{
#ifdef _DEBUG
		vireio::debugf("No available DX9 scene - Timewarping Previous Frame");
#endif
	}

    // Initialize our single full screen Fov layer.
    ovrLayerEyeFov ld;
    ld.Header.Type  = ovrLayerType_EyeFov;
    //ld.Header.Type  = (vrScene != NULL) ? ovrLayerType_EyeFov : ovrLayerType_Disabled;
    ld.Header.Flags = 0;

	for (int eye = 0; eye < 2; eye++)
	{
		ld.ColorTexture[eye] = m_pEyeRenderTexture[eye]->TextureSet;
		ld.Viewport[eye]     = eyeRenderViewport[eye];
		ld.Fov[eye]          = rift->DefaultEyeFov[eye];
		ld.RenderPose[eye]   = m_pLastScene->m_eyePoses[eye];
	}

    ovrLayerHeader* layers = &ld.Header;
    ovrResult result = ovrHmd_SubmitFrame(rift, m_pLastScene->frameIndex, nullptr, &layers, 1);

	CalcFPS();
}

void OculusDirectToRiftView::PostPresent(D3D9ProxySurface* stereoCapableSurface, D3DProxyDevice* pProxyDevice)
{
	SHOW_CALL("OculusDirectToRiftView::PostPresent()");

	//If we already have a scene for this frame, then we don't need to do anything
	if (m_safeSceneStore.hasScene())
		return;

	// Initialise a new VRScene
	VireioVRScene *pVRScene = m_unusedVireioVRScenePool.pop();

	if (pVRScene)
	{
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
						vireio::debugf("pIDXGIResource->GetSharedHandle");
						hr = pIDXGIResource->GetSharedHandle(&sharedHandle);
						pIDXGIResource->Release();

						if (SUCCEEDED(hr) && sharedHandle != NULL)
						{
							//Create the void scene for this eye, setting the DX9 shared texture as the screen source
							std::pair<float, float> size = hmdInfo->GetPhysicalScreenSize();
							pVRScene->m_pScene[eye] = new VoidScene(pDX11Texture, sharedHandle, size.first / size.second);
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

				//Copy and flush
				m_copyDX11.Context->CopyResource(pVRScene->m_pScene[eye]->m_pTexture, pDX9Texture);
				m_copyDX11.Context->Flush();
			}

			//We don't need to keep the ref here now
			pDX9Texture->Release();
		}
	}

	//Get the frame timing for the very next frame - we'll use the information therein to estimate the
	//actual frame we are going to hit
	ovrFrameTiming ftiming = ovrHmd_GetFrameTiming(rift, 0);
	
	//Write out timing info - might reveal something!
	vireio::debugf("**********  START *****************"); 
	vireio::debugf("ftiming.DisplayMidpointSeconds = %0.8f", ftiming.DisplayMidpointSeconds); 
	vireio::debugf("ftiming.AppFrameIndex = %u", ftiming.AppFrameIndex); 
	vireio::debugf("ftiming.DisplayFrameIndex = %u", ftiming.DisplayFrameIndex); 
	vireio::debugf("ftiming.FrameIntervalSeconds = %0.8f", ftiming.FrameIntervalSeconds); 
	vireio::debugf("ovr_GetTimeInSeconds() = %0.8f", ovr_GetTimeInSeconds()); 
	vireio::debugf("lastFrameTime (ms) = %0.8f", pProxyDevice->getLastFrameTime() * 1000.0f); 

	//Calculate the frame index of the frame we are most likely to be ready for, use last frame time
	//as our guide, can't think of anything better right now
	int frameIndexOffset = (((pProxyDevice->getLastFrameTime() - (ftiming.DisplayMidpointSeconds - ovr_GetTimeInSeconds())) 
		/ ftiming.FrameIntervalSeconds) + 1.0);

	//Get the tracking state for the frame we are going to (hopefully!) hit
	pVRScene->frameIndex = ftiming.DisplayFrameIndex + frameIndexOffset;
	ftiming = ovrHmd_GetFrameTiming(rift, pVRScene->frameIndex);
	pVRScene->m_trackingState = ovrHmd_GetTrackingState(rift, ftiming.DisplayMidpointSeconds);

	vireio::debugf("Target frameIndex = %u", pVRScene->frameIndex); 
	vireio::debugf("Target DisplayMidpointSeconds = %0.8f", ftiming.DisplayMidpointSeconds); 
	vireio::debugf("Target AppFrameIndex = %u", ftiming.AppFrameIndex); 
	vireio::debugf("Target DisplayFrameIndex = %u", ftiming.DisplayFrameIndex); 
	vireio::debugf("Target FrameIntervalSeconds = %0.8f", ftiming.FrameIntervalSeconds); 

	// Get both eye poses simultaneously, with IPD offset already included. 
	ovrVector3f      HmdToEyeViewOffset[2] = { m_eyeRenderDesc[0].HmdToEyeViewOffset,
												m_eyeRenderDesc[1].HmdToEyeViewOffset };

	//If DSV is not enabled, then we don't want to use stereo separation, since this is already done
	//in the DX9 rendering, simply glide the IPD separation in and out as we change from DSV rather than a sudden switch
	HmdToEyeViewOffset[ovrEye_Left].x *= m_screenViewGlideFactor;
	HmdToEyeViewOffset[ovrEye_Right].x *= m_screenViewGlideFactor;

	ovr_CalcEyePoses(pVRScene->m_trackingState.HeadPose.ThePose, HmdToEyeViewOffset, pVRScene->m_eyePoses);
	m_pOculusTracker->SetFrameHMDData(pVRScene->m_trackingState);

	//Now push the new scene into our safe store - This will indicate to the DX11 thread a new scene is available
	m_safeSceneStore.push(pVRScene);
}

void OculusDirectToRiftView::PrePresent(D3D9ProxySurface* stereoCapableSurface)
{
	SHOW_CALL("OculusDirectToRiftView::PrePresent()");

	//Screen output
	IDirect3DSurface9* leftImage = stereoCapableSurface->getActualLeft();
	m_pActualDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	m_pActualDevice->StretchRect(leftImage, NULL, backBuffer, NULL, D3DTEXF_NONE);

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


