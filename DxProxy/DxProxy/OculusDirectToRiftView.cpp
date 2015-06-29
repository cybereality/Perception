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

//FPS Calculator

#define MAXSAMPLES 100

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
//		vireio::debugf("SetEventFlag: %i", evt);
	}

	//Signal that we have raised a flag, and then wait for indication it has been processed
	//Wait for at most 200ms, if we have waited that long, then something is clearly wrong
	SignalObjectAndWait(m_EventFlagRaised, m_EventFlagProcessed, 200, FALSE);
}

void OculusDirectToRiftView::GetEventFlag(ThreadEvents &evt)
{
	{
		std::lock_guard<std::mutex> lock(m_mtx);
//		vireio::debugf("GetEventFlag: %i", m_eventFlag);
		evt = (ThreadEvents)m_eventFlag;
		//reset
		m_eventFlag = NONE;
	}
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

//An extension to the Texture Class provided in the OVR Samples
struct VireioTexture : public Texture
{
    VireioTexture(ID3D11Texture2D *pTexture, bool rendertarget, Sizei size) :
		Texture(rendertarget, size)
    {
		SHOW_CALL("VireioTexture()");
		
		//Release the texture created by the base class
		if(Tex)
		{
			Tex->Release();
			Tex = pTexture;
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

        DIRECTX.Device->CreateShaderResourceView(pTexture, NULL, &TexSv);
		if (rendertarget) DIRECTX.Device->CreateRenderTargetView(Tex, NULL, &TexRtv);
   }
};

//------------------------------------------------------------------------- 
struct VoidScene  
{
    Model * screen;
	VireioTexture *texture;

 	VoidScene(ID3D11Texture2D *pTexture, float aspect) :
		m_pTexture(pTexture)
    {
		SHOW_CALL("VoidScene::VoidScene()");

		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);
#if _DEBUG
		vireio::debugf("Texture Format: %i", desc.Format);
#endif

		//float aspect = //(float)(desc.Width) / (float)(desc.Height);
		texture = new VireioTexture(pTexture, false, Sizei(desc.Width, desc.Height));
		screen = new Model(texture, -2.0f, -2.0f / aspect, 2.0f, 2.0f / aspect);
    }

	~VoidScene()
	{
		SHOW_CALL("VoidScene::~VoidScene()");
		m_pTexture->Release();
		delete texture;
		delete screen;
	}

   void Render(Matrix4f projView, float R, float G, float B, float A, bool standardUniforms)
    {   
		SHOW_CALL("VoidScene::Render()");
		screen->Render(projView,R,G,B,A,standardUniforms);    
	}


	ID3D11Texture2D *m_pTexture;
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
	m_eventFlag(0)
{
	SHOW_CALL("OculusDirectToRiftView::OculusDirectToRiftView()");

	m_pOculusTracker = static_cast<OculusTracker*>(motionTracker);
	rift = m_pOculusTracker->GetOVRHmd();

	m_EventFlagProcessed = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagProcessed");
	m_EventFlagRaised = CreateEvent(NULL, FALSE, FALSE, "m_EventFlagRaised");

	HANDLE dx11Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&DX11RenderThread, (void*)this, 0, NULL);
	SetThreadPriority(dx11Thread, THREAD_PRIORITY_HIGHEST);
}

OculusDirectToRiftView::~OculusDirectToRiftView()
{
	SHOW_CALL("OculusDirectToRiftView::~OculusDirectToRiftView()");


	//Terminate the rendering thread and wait for it to go away
	SetEventFlag(TERMINATE_THREAD);
}

bool OculusDirectToRiftView::DX11RenderThread_Init()
{
	bool initialized = DX11RenderThread_InitDevice(Sizei(hmdInfo->GetResolution().first, hmdInfo->GetResolution().second));
	VALIDATE(initialized, "Unable to initialize D3D11 device.");
	ovrHmd_SetBool(rift, "QueueAheadEnabled", true);
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

	return initialized;
}

bool OculusDirectToRiftView::DX11RenderThread_InitDevice(Sizei sz)
{
    IDXGIFactory * DXGIFactory;
    IDXGIAdapter * Adapter;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
        return(false);
    if (FAILED(DXGIFactory->EnumAdapters(0, &Adapter)))
        return(false);
    if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
        NULL, 0, NULL, 0, D3D11_SDK_VERSION, &DIRECTX.Device, NULL, &DIRECTX.Context)))
        return(false);

    // Main depth buffer
    DIRECTX.MainDepthBuffer = new DepthBuffer(DIRECTX.Device, sz);
    DIRECTX.Context->OMSetRenderTargets(1, &DIRECTX.BackBufferRT, DIRECTX.MainDepthBuffer->TexDsv);

    // Buffer for shader constants
    DIRECTX.UniformBufferGen = new DataBuffer(DIRECTX.Device, D3D11_BIND_CONSTANT_BUFFER, NULL, UNIFORM_DATA_SIZE);
    DIRECTX.Context->VSSetConstantBuffers(0, 1, &DIRECTX.UniformBufferGen->D3DBuffer);

    // Set a standard blend state, ie transparency from alpha
    /*D3D11_BLEND_DESC bm;
    memset(&bm, 0, sizeof(bm));
    bm.RenderTarget[0].BlendEnable = TRUE;
    bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ID3D11BlendState * BlendState;
    DIRECTX.Device->CreateBlendState(&bm, &BlendState);
    DIRECTX.Context->OMSetBlendState(BlendState, NULL, 0xffffffff);*/

    // Set max frame latency to 1
    IDXGIDevice1* DXGIDevice1 = NULL;
    HRESULT hr = DIRECTX.Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
    if (FAILED(hr) | (DXGIDevice1 == NULL)) return(false);
    DXGIDevice1->SetMaximumFrameLatency(1);
    DXGIDevice1->Release();

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

	for (int eye = 0; eye < 2; ++eye)
	{
		if (m_pScene[eye])
		{
			delete m_pScene[eye];
			m_pScene[eye] = NULL;
		}
	}

	//Signal the DX9 thread can now continue		
	SetEvent(m_EventFlagProcessed);
}

void OculusDirectToRiftView::DX11RenderThread_TimewarpLastFrame()
{
	SHOW_CALL("OculusDirectToRiftView::DX11RenderThread_TimewarpLastFrame()");

	//Don't do anything if we haven't got the scene ready yet
	if (!m_pScene[0])
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
		ld.RenderPose[eye]   = m_EyeRenderPose[eye];
    }

	ovrLayerHeader* layers = &ld.Header;
    ovrResult result = ovrHmd_SubmitFrame(rift, 0, nullptr, &layers, 1);

	CalcFPS();
}

std::string OculusDirectToRiftView::GetAdditionalFPSInfo()
{
	return vireio::retprintf("Async Timewarp FPS: %.1f", hmdFPS);
}


void OculusDirectToRiftView::DX11RenderThread_RenderNewFrame()
{
	SHOW_CALL("OculusDirectToRiftView::DX11RenderThread_RenderNewFrame()");

	// Initialise Eye Buffers
	for (int eye = 0; eye < 2; eye++)
	{
		ID3D11Resource *tempResource11 = NULL;
		HANDLE textHandle = (eye == (!config->swap_eyes ? ovrEye_Left : ovrEye_Right)) ? m_stereoCapableSurface->getHandleLeft() : m_stereoCapableSurface->getHandleRight();

		IID iid = __uuidof(ID3D11Resource);
		if (FAILED(DIRECTX.Device->OpenSharedResource(textHandle, iid, (void**)(&tempResource11))))
		{
			vireio::debugf("Failed to open shared resource");
			return;
		}

		//QI for the texture
		ID3D11Texture2D *pDX9Texture = NULL;
		tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&pDX9Texture)); 
		tempResource11->Release();

		//Have to create scene if we don't already have one
		if (!m_pScene[eye])
		{
			vireio::debugf("Creating scene for %s eye", eye == 0 ? "Left" : "Right");
			//Create local instances of the textures and then copy the data into them
			D3D11_TEXTURE2D_DESC dsDesc;
			pDX9Texture->GetDesc(&dsDesc);
			ID3D11Texture2D *pDX11Texture = NULL;
			if (SUCCEEDED(DIRECTX.Device->CreateTexture2D(&dsDesc, NULL, &pDX11Texture)))
			{
				//Create the void scene for this eye, setting the DX9 shared texture as the screen source
				std::pair<float, float> size = hmdInfo->GetPhysicalScreenSize();
				m_pScene[eye] = new VoidScene(pDX11Texture, size.first / size.second);
			}
			else
			{
				vireio::debugf("Failed to create Dx11 texture");
			}
		}

		//Copy and flush immediately
		DIRECTX.Context->CopyResource(m_pScene[eye]->m_pTexture, pDX9Texture);
		DIRECTX.Context->Flush();

		//We don't need to keep the ref here now
		pDX9Texture->Release();
	}

	// Get both eye poses simultaneously, with IPD offset already included. 
    ovrVector3f      HmdToEyeViewOffset[2] = { m_eyeRenderDesc[0].HmdToEyeViewOffset,
                                                m_eyeRenderDesc[1].HmdToEyeViewOffset };

	//Get the tracking state for the frame we are about to show
	ovrTrackingState trackingState = m_pOculusTracker->GetTrackingState();
	ovr_CalcEyePoses(trackingState.HeadPose.ThePose, HmdToEyeViewOffset, m_EyeRenderPose);

	//If we get here, then the DX9 thread can be released (since we have local copies of the textures now)
	SetEvent(m_EventFlagProcessed);

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
		Camera mainCam(Vector3f(0.0f, 0.2f - config->YOffset, (2.4f - ZoomOutScale) + m_screenViewGlideFactor), Matrix4f::RotationY(0.0f));

        // View and projection matrices for the stereo camera
        Camera finalCam(mainCam.Pos + mainCam.Rot.Transform(m_EyeRenderPose[eye].Position),
            mainCam.Rot * Matrix4f(m_EyeRenderPose[eye].Orientation));

		//View matrix is a monoscopic straight ahead camera is not in disconnected screen view mode
		Matrix4f view = (m_disconnectedScreenView || m_screenViewGlideFactor > 0.0f) ? finalCam.GetViewMatrix() : mainCam.GetViewMatrix();
        Matrix4f proj = ovrMatrix4f_Projection(m_eyeRenderDesc[eye].Fov, 0.2f, 1000.0f, ovrProjection_RightHanded);
		
		m_pScene[eye]->Render(proj*view, 1, 1, 1, 1, true);
    }

    // Initialize our single full screen Fov layer.
    ovrLayerEyeFov ld;
    ld.Header.Type  = ovrLayerType_EyeFov;
    ld.Header.Flags = 0;

    for (int eye = 0; eye < 2; eye++)
    {
        ld.ColorTexture[eye] = m_pEyeRenderTexture[eye]->TextureSet;
        ld.Viewport[eye]     = eyeRenderViewport[eye];
        ld.Fov[eye]          = rift->DefaultEyeFov[eye];
		ld.RenderPose[eye]   = m_EyeRenderPose[eye];
    }

    ovrLayerHeader* layers = &ld.Header;
    ovrResult result = ovrHmd_SubmitFrame(rift, 0, nullptr, &layers, 1);

	CalcFPS();
}

void OculusDirectToRiftView::PostPresent(D3D9ProxySurface* stereoCapableSurface)
{
	SHOW_CALL("OculusDirectToRiftView::PostPresent()");

	//Flag the arrival of the new frame and wait
	m_stereoCapableSurface = stereoCapableSurface;
	SetEventFlag(NEW_FRAME);
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
		//Can't do antyhing immediately, sleep for a very small timeout period (hopefullly wake before next vsync)
		DWORD result = WaitForSingleObject(m_EventFlagRaised, 0);
		if (result == WAIT_TIMEOUT)
		{
			//If we've not had a frame update from DX9, then just timewarp the last frame
			DX11RenderThread_TimewarpLastFrame();
		}
		else
		{
			GetEventFlag(eventFlag);
			switch (eventFlag)
			{
			case NONE:
				{
					vireio::debugf("eventFlag = NONE, WaitForSingleObject = %0.8x", result);
				}
				break;
			case NEW_FRAME:
				{
					DX11RenderThread_RenderNewFrame();
				}
				break;
			case RELEASE_EVERYTHING:
				{
					DX11RenderThread_ReleaseEverything();
				}
				break;
			default:
				//Do nothing here
				break;
			}
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

	if (!m_disconnectedScreenView)
	{
		if (m_screenViewGlideFactor > 0.0f)
			m_screenViewGlideFactor -= 0.05f;
	}
	else
	{
		//Disconnected screen view not active
		if (m_screenViewGlideFactor < 1.0f)
			m_screenViewGlideFactor += 0.05f;
	}

}

/**
* Loads Oculus Rift shader effect files.
***/ 
void OculusDirectToRiftView::InitShaderEffects()
{
	SHOW_CALL("OculusDirectToRiftView::InitShaderEffects");
}


