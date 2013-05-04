/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

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
#include "D3DProxyStereoSurface.h"
#include "StereoViewFactory.h"
#include "MotionTrackerFactory.h"
#include <typeinfo>

#ifdef _DEBUG
#include "DxErr.h"
#define D3D_DEBUG_INFO
#endif

#pragma comment(lib, "d3dx9.lib")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define IS_RENDER_TARGET(d3dusage) ((d3dusage & D3DUSAGE_RENDERTARGET) > 0 ? true : false)
#define IS_POSSIBLE_RENDER_TARGET(d3dpool) ((d3dpool & D3DPOOL_DEFAULT) > 0 ? true : false) // render targets have to be in D3DPOOL_DEFAULT



#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

D3DProxyDevice::D3DProxyDevice(IDirect3DDevice9* pDevice):BaseDirect3DDevice9(pDevice),
	m_activeRenderTargets (1, NULL)		
{
	OutputDebugString("D3D ProxyDev Created\n");
	
	// Check the maximum number of supported render targets
	D3DCAPS9 capabilities;
	BaseDirect3DDevice9::GetDeviceCaps(&capabilities);
	DWORD maxRenderTargets = capabilities.NumSimultaneousRTs;

	m_activeRenderTargets.resize(maxRenderTargets, NULL);
	m_currentRenderingSide = Left;
	
	pStereoBackBuffer = NULL;
	hudFont = NULL;

	centerlineR = 0.0f;
	centerlineL = 0.0f;
	yaw_mode = 0;
	pitch_mode = 0;
	roll_mode = 0;
	translation_mode = 0;
	trackingOn = true;
	SHOCT_mode = 0;
}




D3DProxyDevice::~D3DProxyDevice()
{
	if(hudFont) {
		hudFont->Release();
		hudFont = NULL;
	}


	for(std::vector<D3DProxyStereoSurface*>::size_type i = 0; i != m_activeRenderTargets.size(); i++) 
	{
		if (m_activeRenderTargets[i] != NULL) {
			m_activeRenderTargets[i]->Release();
			m_activeRenderTargets[i] = NULL;
		}
	}

	if(pStereoBackBuffer) {
		pStereoBackBuffer->Release();
		pStereoBackBuffer = NULL;
	}
}


/* 
	Subclasses which override this method must call through to super method.
	Anything that needs to be done before the device is used by the actual application should happen here.
 */
void D3DProxyDevice::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Init\n");

	stereoView = StereoViewFactory::Get(cfg);
	SetupOptions(cfg);
	OnCreateOrRestore();
	SetupMatrices();
}


/*
  Subclasses which override this method must call through to super method.
  Do not directly call this method in subclasses.
  This method should be used to re/create any resources that are held by the device proxy and deleted by Reset.

  The only resources used like this are going to be extra resources that are used by the proxy and are not
  part of the actual calling application. 
  
  Examples in D3DProxyDevice: The Font used in the SHOCT overlay and the stereo buffer.

  Example of something you wouldn't create here:
  Render targets in the m_activeRenderTargets collection. They need to be released to successfully Reset
  the device, but they just wrap IDirect3DSurface9 objects from the underlying application and will be
  re/created by the underlying application.

  This method will be called when the proxy device is initialised with Init (happens before device is
  passed back to actual application by CreateDevice) and after a successful device Reset.
*/
void D3DProxyDevice::OnCreateOrRestore()
{
	OutputDebugString(__FUNCTION__);
	OutputDebugString("\n");

	// Create a stereo render target with the same properties as the backbuffer and set it as the current render target
	IDirect3DSurface9* pBackBuffer;
	if (BaseDirect3DDevice9::GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer) != D3D_OK) {
		OutputDebugString("Failed to fetch backbuffer.\n");
		exit(1); 
	}

	D3DSURFACE_DESC backDesc;
	pBackBuffer->GetDesc(&backDesc);
	pBackBuffer->Release();

	IDirect3DSurface9* pTemp;
	CreateRenderTarget(backDesc.Width, backDesc.Height, backDesc.Format, backDesc.MultiSampleType, backDesc.MultiSampleQuality, false, &pTemp, NULL);
	pStereoBackBuffer = static_cast<D3DProxyStereoSurface*>(pTemp);
	SetRenderTarget(0, pTemp);

	SetupText();

	stereoView->Init(m_pDevice);
}


/*
	Subclasses which override this method must call through to super method at the end of the subclasses
	implementation.
 */
HRESULT WINAPI D3DProxyDevice::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	OutputDebugString(__FUNCTION__);
	OutputDebugString("\n");

	if(stereoView)
		stereoView->Reset();

	int newRefCount = 0;

	if(hudFont) {
		newRefCount = hudFont->Release();

		if (newRefCount > 0) {
			char buf[256];
			sprintf_s(buf, "hudFont count = %d\n", newRefCount);
			OutputDebugString(buf);
		}

		hudFont = NULL;
	}

	

	for(std::vector<D3DProxyStereoSurface*>::size_type i = 0; i != m_activeRenderTargets.size(); i++) 
	{
		if (m_activeRenderTargets[i] != NULL) {
			newRefCount = m_activeRenderTargets[i]->Release();

			if (newRefCount > 0) {
				char buf[256];
				sprintf_s(buf, "m_activeRenderTargets[%d] count = %d\n", i, newRefCount); // count for one of these will usually be one here because it is also referenced by pStereoBackBuffer
				OutputDebugString(buf);
			}

			m_activeRenderTargets[i] = NULL;
		}
	}

	if(pStereoBackBuffer) {
		newRefCount = pStereoBackBuffer->Release();

		if (newRefCount > 0) {
			char buf[256];
			sprintf_s(buf, "pStereoBackBuffer count = %d\n", newRefCount);
			OutputDebugString(buf);
		}

		pStereoBackBuffer = NULL;
	}
	

	HRESULT hr = BaseDirect3DDevice9::Reset(pPresentationParameters);


	
	
#ifdef _DEBUG
	if (FAILED(hr)) {
		char buf[256];
		sprintf_s(buf, "Error: %s error description: %s\n",
				DXGetErrorString(hr), DXGetErrorDescription(hr));

		OutputDebugString(buf);
	}
#endif

	// if the device has been successfully reset we need to recreate any resources we created
	if (hr == D3D_OK) 
		OnCreateOrRestore();

	return hr;
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
	swap_eyes = cfg.swap_eyes;
	stereoView->SwapEyes(swap_eyes);// why are there two swap_eyes
	centerlineL = cfg.centerlineL;
	centerlineR = cfg.centerlineR;

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

void D3DProxyDevice::SetupMatrices()
{	// call this function after StereoView::Init()

	aspectRatio = (float)stereoView->viewport.Width/(float)stereoView->viewport.Height;

	n = 0.1f;					
	f = 10.0f;
	l = -0.5f;
	r = 0.5f;
	t = 0.5f / aspectRatio;
	b = -0.5f / aspectRatio;

	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);
}

void D3DProxyDevice::ComputeViewTranslation()
{
	D3DXMATRIX transform;
	D3DXMatrixTranslation(&transform, separation*eyeShutter*10.0f+offset*10.0f, 0, 0);

	float adjustedFrustumOffset = convergence*eyeShutter*0.1f*separation;		
	D3DXMATRIX reProject;
	D3DXMatrixPerspectiveOffCenterLH(&reProject, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &reProject);
	D3DXMatrixPerspectiveOffCenterLH(&reProject, l+adjustedFrustumOffset, r+adjustedFrustumOffset, b, t, n, f);

	if(trackerInitialized && tracker->isAvailable() && roll_mode != 0)
	{
		D3DXMATRIX rollMatrix;
		D3DXMatrixRotationZ(&rollMatrix, tracker->currentRoll);
		D3DXMatrixMultiply(&transform, &rollMatrix, &transform);
	}

	matViewTranslation = matProjectionInv * transform * reProject;
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

	if(keyWaitCount<0)
		keyWaitCount=0;

	if(KEY_DOWN(VK_NUMPAD0))		// turn on/off stereo3D
	{
		if(keyWaitCount <= 0)
		{
			if(stereoView->stereoEnabled)
				stereoView->stereoEnabled = false;
			else
				stereoView->stereoEnabled = true;
			keyWaitCount = 50;
		}
	}

//////////  SHOCT non numpad
	if(KEY_DOWN(0x4F))// VK_KEY_O
	{
		centerlineL  -= keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(0x50))// VK_KEY_P
	{
		centerlineL  += keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(0x4B))//VK_KEY_K
	{
		centerlineR  -= keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(0x4C))//VK_KEY_L
	{
		centerlineR  += keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(0x49) && KEY_DOWN(VK_CONTROL))//VK_KEY_I		// Schneider-Hicks VR Calibration Tool
	{
		if(keyWaitCount <= 0)
		{
			SHOCT_mode++;
			SHOCT_mode %= 3;
			if(SHOCT_mode == 0){//off
				trackingOn = true;
			}
			if(SHOCT_mode == 1){// seperation
				trackingOn = false;
			}
			if(SHOCT_mode == 2){// convergence
				trackingOn = false;
			}
			keyWaitCount = 50;
		}
	}
//////////

//////////  SHOCT numpad
	if(KEY_DOWN(VK_NUMPAD1))
	{
		centerlineL  -= keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(VK_NUMPAD2))
	{
		centerlineL  += keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_NUMPAD4))
	{
		centerlineR  -= keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}
	if(KEY_DOWN(VK_NUMPAD5))
	{
		centerlineR  += keySpeed/2.0f;
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_MULTIPLY) && KEY_DOWN(VK_SHIFT))		// Schneider-Hicks VR Calibration Tool
	{
		if(keyWaitCount <= 0)
		{
			SHOCT_mode++;
			SHOCT_mode %= 3;
			if(SHOCT_mode == 0){//off
				trackingOn = true;
			}
			if(SHOCT_mode == 1){// seperation
				trackingOn = false;
			}
			if(SHOCT_mode == 2){// convergence
				trackingOn = false;
			}
			keyWaitCount = 50;
		}
	}
//////////

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
			separation -= keySpeed * 0.2f;
			if(separation < 0)		// no negative seperation
				separation = 0;
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
			separation += keySpeed * 0.2f;
		}
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_F4))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			this->stereoView->DistortionScale  -= keySpeed*10;
		} 
		else 
		{
			convergence -= keySpeed2*10;
		}
		saveWaitCount = 500;
		doSaveNext = true;
	}

	if(KEY_DOWN(VK_F5))
	{
		if(KEY_DOWN(VK_SHIFT))
		{
			this->stereoView->DistortionScale  += keySpeed*10;
		} 
		else 
		{
			convergence += keySpeed2*10;
		}
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
			saveWaitCount = 500;
			doSaveNext = true;
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
		helper->SaveProfile(separation, convergence, swap_eyes, yaw_multiplier, pitch_multiplier, roll_multiplier);
		helper->SaveUserConfig(centerlineL, centerlineR);
	}

}

void D3DProxyDevice::HandleTracking()
{
	if(!trackingOn){
		tracker->currentRoll = 0;
		return;
	}
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

void ClearVLine(LPDIRECT3DDEVICE9 Device_Interface,int x1,int y1,int x2,int y2,int bw,D3DCOLOR Color)
{
	D3DRECT rec;
	rec.x1 = x1-bw;
	rec.y1 = y1;
	rec.x2 = x2+bw;
	rec.y2 = y2;

	Device_Interface->Clear(1,&rec,D3DCLEAR_TARGET,Color,0,0);
}

void ClearHLine(LPDIRECT3DDEVICE9 Device_Interface,int x1,int y1,int x2,int y2,int bw,D3DCOLOR Color)
{
	D3DRECT rec;
	rec.x1 = x1;
	rec.y1 = y1-bw;
	rec.x2 = x2;
	rec.y2 = y2+bw;

	Device_Interface->Clear(1,&rec,D3DCLEAR_TARGET,Color,0,0);
}

HRESULT WINAPI D3DProxyDevice::EndScene()
{
///// hud text

	if(hudFont && SHOCT_mode !=0) {
		char vcString[512];
		int width = stereoView->viewport.Width;
		int height = stereoView->viewport.Height;

		float horWidth = 0.15f;
		int beg = (int)(width*(1.0f-horWidth)/2.0);
		int end = (int)(width*(0.5f+(horWidth/2.0f)));

		int hashTop = (int)(height * 0.48f);
		int hashBottom = (int)(height * 0.52f);

		RECT rec2 = {(int)(width*0.27f), (int)(height*0.3f),width,height};
		sprintf_s(vcString, 512, "Schneider-Hicks Optical Calibration Tool (S.H.O.C.T.).\n");
		hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));


		if(SHOCT_mode == 1){//Separation mode
			if((eyeShutter > 0 && stereoView->swap_eyes == false) || (eyeShutter < 0 && stereoView->swap_eyes == true)) {// left eye
				//eye center line
				ClearVLine(this, (int)(width/2 + (centerlineL * width)), 0, (int)(width/2 + (centerlineL * width)), height, 1, D3DCOLOR_ARGB(255,255,0,0));
			}else{// right eye
				ClearVLine(this, (int)(width/2 + (centerlineR * width)), 0, (int)(width/2 + (centerlineR * width)), height, 1, D3DCOLOR_ARGB(255,255,0,0));
			}
		}
		if(SHOCT_mode == 2){//Convergence mode
			//screen center line
			ClearVLine(this,width/2,0,width/2, height,1,D3DCOLOR_ARGB(255,0,0,255));
			if((eyeShutter > 0 && stereoView->swap_eyes == false) || (eyeShutter < 0 && stereoView->swap_eyes == true)) {// left eye
				// horizontal line
				ClearHLine(this,beg,(height/2),end, (height/2),1,D3DCOLOR_ARGB(255,0,0,255));

				// hash lines
				int hashNum = 10;
				float hashSpace = horWidth*width / (float)hashNum;
				for(int i=0; i<=hashNum; i++) {
					ClearVLine(this,beg+(int)(i*hashSpace),hashTop,beg+(int)(i*hashSpace),hashBottom,1,D3DCOLOR_ARGB(255,255,255,0));
				}

				RECT rec2 = {(int)(width*0.37f), (int)(height*0.525f), width, height};
				sprintf_s(vcString, 512, "Positive Parallax");
				hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

				rec2.left = (int)(width *0.52f);
				sprintf_s(vcString, 512, "Negative Parallax");
				hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

			}else{// right eye
				RECT rec2 = {(int)(width*0.37f), (int)(height*0.44f), width, height};
				sprintf_s(vcString, 512, "Walk up as close as possible to a 90 degree\n vertical object, and align this line with its edge.\n Good examples include a wall, corner, a table corner,\n a squared post, etc.");
				hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));
			}
		}

		rec2.left = (int)(width*0.35f);
		rec2.top = (int)(height*0.33f);
		if(SHOCT_mode == 1)
			sprintf_s(vcString, 512, "Separation");
		if(SHOCT_mode == 2)
			sprintf_s(vcString, 512, "Convergence");
		hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

	}
/////
	return BaseDirect3DDevice9::EndScene();
}



/*
	The IDirect3DSurface9** ppSurface returned should always be a D3DProxyStereoSurface. Any class overloading
	this method should ensure that this remains true.
 */
HRESULT WINAPI D3DProxyDevice::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample,
													DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 
	// Duplicate render target if needed.
	/* 
		"If Needed" heuristic is the complicated part here.
		  Fixed heuristics (based on type, format, size, etc) + game specific overrides + isForcedMono + magic?

		Always create a D3DProxyStereoSurface using the render target and return that
	 */

	// TODO Experiment: Try modifying fullscreen render targets to shrink them to side by side sizes.

	IDirect3DSurface9* pLeftRenderTarget = NULL;
	IDirect3DSurface9* pRightRenderTarget = NULL;
	HRESULT creationResult;

	if (true) // TODO Should we duplicate this Render Target? Replace "true" with heuristic
	{
		if ((creationResult = BaseDirect3DDevice9::CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &pLeftRenderTarget, pSharedHandle)) == D3D_OK) {
			if (BaseDirect3DDevice9::CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &pRightRenderTarget, pSharedHandle) != D3D_OK) {
				OutputDebugString("Failed to create right eye render target while attempting to create stereo pair, falling back to mono\n");
				pRightRenderTarget = NULL;
			}
		}
		else {
			OutputDebugString("Failed to create left eye render target while attempting to create stereo pair\n"); 
		}
	}
	else {
		creationResult = BaseDirect3DDevice9::CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &pLeftRenderTarget, pSharedHandle);
	}


	if (creationResult == D3D_OK)
		*ppSurface = new D3DProxyStereoSurface(pLeftRenderTarget, pRightRenderTarget);

	return creationResult;
}





HRESULT WINAPI D3DProxyDevice::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	// don't bother wrapping surfaces that aren't in the defalut pool. They can't be used as render targets so don't need to be stereo capable
	if (!IS_POSSIBLE_RENDER_TARGET(Pool)) 
		return BaseDirect3DDevice9::CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
	

	// Surfaces created in the default pool can be used as render targets so we have to assume this surface will be used as a target and be prepared for that.
	IDirect3DSurface9* pLeftSurface = NULL;
	IDirect3DSurface9* pRightSurface = NULL;
	HRESULT creationResult;
	if (true) // TODO Should we duplicate this possible Render Target? Replace "true" with heuristic
	{
		if ((creationResult = BaseDirect3DDevice9::CreateOffscreenPlainSurface(Width, Height, Format, Pool, &pLeftSurface, pSharedHandle)) == D3D_OK) {
			if (BaseDirect3DDevice9::CreateOffscreenPlainSurface(Width, Height, Format, Pool, &pRightSurface, pSharedHandle) != D3D_OK) {
				OutputDebugString("Failed to create right eye OffscreenPlainSurface while attempting to create stereo pair, falling back to mono\n");
				pRightSurface = NULL;
			}
		}
		else {
			OutputDebugString("Failed to create left eye while attempting to create stereo pair of OffscreenPlainSurfaces\n"); 
		}
	}
	else {
		// even though we haven't duplicated this surface we still wrap it as it could be a stereo surface and when past back to
		// any other method by the underlying application later we will be assuming it is wrapped by a stereo capable surface
		creationResult = BaseDirect3DDevice9::CreateOffscreenPlainSurface(Width, Height, Format, Pool, &pLeftSurface, pSharedHandle);
	}


	if (creationResult == D3D_OK)
		*ppSurface = new D3DProxyStereoSurface(pLeftSurface, pRightSurface);

	return creationResult;
}





HRESULT WINAPI D3DProxyDevice::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::Clear(Count, pRects, Flags, Color, Z, Stencil) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::Clear(Count, pRects, Flags, Color, Z, Stencil);
	}

	return result;
}



HRESULT WINAPI D3DProxyDevice::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::ColorFill(pSurface, pRect, color) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::ColorFill(pSurface, pRect, color);
	}

	return result;
}


HRESULT WINAPI D3DProxyDevice::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
	}

	return result;
}


HRESULT WINAPI D3DProxyDevice::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	return result;
}


HRESULT WINAPI D3DProxyDevice::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	return result;
}

HRESULT WINAPI D3DProxyDevice::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	return result;
}

HRESULT WINAPI D3DProxyDevice::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawRectPatch(Handle, pNumSegs, pRectPatchInfo) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
	}

	return result;
}

HRESULT WINAPI D3DProxyDevice::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	setDrawingSide(Left);
	
	HRESULT result;
	if (result = BaseDirect3DDevice9::DrawTriPatch(Handle, pNumSegs, pTriPatchInfo) == D3D_OK) {
		if (setDrawingSide(Right))
			BaseDirect3DDevice9::DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
	}

	return result;
}



HRESULT WINAPI D3DProxyDevice::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	if (!pRenderTarget)
		OutputDebugString("SetRenderTarget called with null surface\n"); 

	OutputDebugString(typeid(pRenderTarget).name());
	OutputDebugString("\n");

	D3DProxyStereoSurface* newRenderTarget = static_cast<D3DProxyStereoSurface*>(pRenderTarget);

	if (!newRenderTarget)
		OutputDebugString("SetRenderTarget cast D3DProxyStereoSurface resulted in null surface\n"); 
	
	// Update actual render target
	HRESULT result;
	if (newRenderTarget == NULL) {
		if (RenderTargetIndex == 0) {
			result = D3DERR_INVALIDCALL;
			OutputDebugString("newRenderTarget == null and RenderTargetIndex == 0\n"); 
		}
		else {
			result = BaseDirect3DDevice9::SetRenderTarget(RenderTargetIndex, newRenderTarget);
		}
	}
	else if (!newRenderTarget->IsStereo() && (m_currentRenderingSide == Left)) {
		OutputDebugString("(!newRenderTarget->IsStereo() && (m_currentRenderingSide == Left))\n"); 
		if (!newRenderTarget->getMonoSurface())
			OutputDebugString("Mono is null\n"); 
		if (!newRenderTarget->getLeftSurface())
			OutputDebugString("Left is null\n"); 
		if (!newRenderTarget->getRightSurface())
			OutputDebugString("right is null\n"); 
		result = BaseDirect3DDevice9::SetRenderTarget(RenderTargetIndex, newRenderTarget->getMonoSurface());
	}
	else if (m_currentRenderingSide == Left) {
		OutputDebugString("(m_currentRenderingSide == Left)\n"); 
		result = BaseDirect3DDevice9::SetRenderTarget(RenderTargetIndex, newRenderTarget->getLeftSurface());
	}
	else {
		OutputDebugString("else\n"); 
		result = BaseDirect3DDevice9::SetRenderTarget(RenderTargetIndex, newRenderTarget->getRightSurface());
	}

	
	
	if (result == D3D_OK) {
		// update proxy collection of stereo render targets to reflect new actual render target

		// release old render target
		if (m_activeRenderTargets[RenderTargetIndex] != NULL)
			m_activeRenderTargets[RenderTargetIndex]->Release();

		// replace with new render target (may be NULL)
		m_activeRenderTargets[RenderTargetIndex] = newRenderTarget;
		if (m_activeRenderTargets[RenderTargetIndex] != NULL)
			m_activeRenderTargets[RenderTargetIndex]->AddRef();
	}

	return result;
}




HRESULT WINAPI D3DProxyDevice::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	if ((RenderTargetIndex >= m_activeRenderTargets.capacity()) || (RenderTargetIndex < 0)) {
		return D3DERR_INVALIDCALL;
	}

	D3DProxyStereoSurface* targetToReturn = m_activeRenderTargets[RenderTargetIndex];
	if (!targetToReturn)
		return D3DERR_NOTFOUND;
	else {
		*ppRenderTarget = targetToReturn;
		targetToReturn->AddRef();
		return D3D_OK;
	}
}




/*
	Switches to rendering to which ever side is specified by side.

	Returns true if change succeeded, false if it fails. The switch will fail if you attempt to setDrawingSide(Right)
	when the current primary active render target (target 0  in m_activeRenderTargets) is not stereo.
	Attempting to switch to a side when that side is already the active side will return true without making any changes.
 */
bool D3DProxyDevice::setDrawingSide(EyeSide side)
{
	OutputDebugString(__FUNCTION__); 
	OutputDebugString("\n"); 

	if (side == m_currentRenderingSide)
		return true;

	// should never set render target 0 to null.
	if (!m_activeRenderTargets[0]->IsStereo() && (side == Right)) 
		return false;


	HRESULT result;
	D3DProxyStereoSurface* pCurrentRT;
	for(std::vector<D3DProxyStereoSurface*>::size_type i = 0; i != m_activeRenderTargets.size(); i++) 
	{
		if ((pCurrentRT = m_activeRenderTargets[i]) != NULL) {

			if (!pCurrentRT->IsStereo() && (side == Left)) {
				// draw mono surfaces on the left eye pass
				result = BaseDirect3DDevice9::SetRenderTarget(i, pCurrentRT->getMonoSurface()); 
			}
			else {
				if (side == Left) {
					result = BaseDirect3DDevice9::SetRenderTarget(i, pCurrentRT->getLeftSurface()); 
				}
				else {
					result = BaseDirect3DDevice9::SetRenderTarget(i, pCurrentRT->getRightSurface());
				}																			
			}
				
			if (result != D3D_OK)
				OutputDebugString("Error trying to set one of the Render Targets while switching between active eyes for drawing.\n");
		}
	}

	m_currentRenderingSide = side;

	return true;
}



HRESULT WINAPI D3DProxyDevice::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if (stereoView->initialized)
		stereoView->Draw(pStereoBackBuffer);

	return BaseDirect3DDevice9::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


/*
	Multiple swap chains not handled. It is assumed that there is only one swap chain. This is guaranteed for full screen applications? (might not be for multi monitor setups?)
	Currently only supports one back buffer

	This is quick and dirty. Proper swapchain proxying is probably required
 */
HRESULT WINAPI D3DProxyDevice::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	if (iSwapChain > 0)
		OutputDebugString("GetBackBuffer: Swap chain other than swapchain 0 requested. Support for this has not yet been implemented. Bad things may be about to happen.");

	if (iBackBuffer > 0)
		OutputDebugString("Swap chain has more than one back buffer. Support for this has not yet been implemented. Bad things may be about to happen.");


	*ppBackBuffer = pStereoBackBuffer;

	return D3D_OK;


	//return BaseDirect3DDevice9::GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}



/*
	Currently only a single swap chain with a single back buffer is supported. Calls to this method probably indicate a need to create a full swapchain proxy
	(as the backbuffer could be retrieved from the proxy and multiple swap chains mean multiple backbuffers)
 */
HRESULT WINAPI D3DProxyDevice::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	if (iSwapChain > 0)
		OutputDebugString("GetSwapChain: Swap chain other than swapchain 0 requested. Support for this has not yet been implemented. Bad things may be about to happen.");

	OutputDebugString("GetSwapChain: If this chain is used to get anything other than buffer 0 bad things are going to happen");

	return BaseDirect3DDevice9::GetSwapChain(iSwapChain, pSwapChain);
}

/* see above */
HRESULT WINAPI D3DProxyDevice::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	OutputDebugString("CreateAdditionalSwapChain: Doom, doom, doom... go home now.");

	return BaseDirect3DDevice9::CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

