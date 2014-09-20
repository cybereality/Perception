/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDevice.cpp> and
Class <D3DProxyDevice> :
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

#include "D3DProxyDevice.h"
#include "D3D9ProxySurface.h"
#include "D3D9ProxySwapChain.h"
#include "D3D9ProxyDirect3D.h"
#include "D3D9ProxyTexture.h"
#include "D3D9ProxyVolumeTexture.h"
#include "D3D9ProxyCubeTexture.h"
#include "D3D9ProxyVertexBuffer.h"
#include "D3D9ProxyIndexBuffer.h"
#include "D3D9ProxyStateBlock.h" 
#include "D3D9ProxyQuery.h"
#include "MotionTrackerFactory.h"
#include "cStereoMode.h"
#include "VRBoostEnums.h"
#include "StereoShaderConstant.h"
#include "StereoBackBuffer.h"
#include "GameHandler.h"
#include "ShaderRegisters.h"
#include "ViewAdjustment.h"
#include "StereoView.h"
#include "MotionTracker.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <typeinfo>
#include <assert.h>
#include <comdef.h>
#include <tchar.h>
#ifdef _DEBUG
#include "DxErr.h"
#endif

#include "Version.h"

#pragma comment(lib, "d3dx9.lib")

#define SMALL_FLOAT 0.001f
#define	SLIGHTLY_LESS_THAN_ONE 0.999f

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#define OUTPUT_HRESULT(hr) { _com_error err(hr); LPCTSTR errMsg = err.ErrorMessage(); OutputDebugStringA(errMsg); }

#define MAX_PIXEL_SHADER_CONST_2_0 32
#define MAX_PIXEL_SHADER_CONST_2_X 32
#define MAX_PIXEL_SHADER_CONST_3_0 224

#define MENU_ITEM_SEPARATION  40

using namespace VRBoost;

/**
* Returns the mouse wheel scroll lines.
***/
UINT GetMouseScrollLines()
{
	#ifdef SHOW_CALLS
		OutputDebugStringA("called GetMouseScrollLines");
	#endif
	int nScrollLines = 3;
	HKEY hKey;

	if (RegOpenKeyExA(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
		0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		char szData[128];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = sizeof(szData);

		if (RegQueryValueExA(hKey, "WheelScrollLines", NULL, &dwKeyDataType,
			(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			nScrollLines = strtoul(szData, NULL, 10);
		}

		RegCloseKey(hKey);
	}

	return nScrollLines;
}

/**
* Constructor : creates game handler and sets various states.
***/
D3DProxyDevice::D3DProxyDevice(IDirect3DDevice9* pDevice,IDirect3DDevice9Ex* pDeviceEx,  D3D9ProxyDirect3D* pCreatedBy , cConfig& cfg ) :
	actual(pDevice),
	actualEx(pDeviceEx),
	m_pCreatedBy(pCreatedBy),
	m_nRefCount(1),
	m_activeRenderTargets (1, NULL),
	m_activeTextureStages(),
	m_activeVertexBuffers(),
	m_activeSwapChains(),
	m_gameXScaleUnits(),
	controls(),
	dinput(),
	activePopup(VPT_NONE),
	show_fps(FPS_NONE),
	calibrate_tracker(false)
{

	InitVRBoost();

	m_spShaderViewAdjustment = std::make_shared<ViewAdjustment>( 1.0f, false , cfg );
	m_pGameHandler = new GameHandler();

	// Check the maximum number of supported render targets
	D3DCAPS9 capabilities;
	actual->GetDeviceCaps(&capabilities);
	DWORD maxRenderTargets = capabilities.NumSimultaneousRTs;
	m_activeRenderTargets.resize(maxRenderTargets, NULL);

	D3DXMatrixIdentity(&m_leftView);
	D3DXMatrixIdentity(&m_rightView);
	D3DXMatrixIdentity(&m_leftProjection);
	D3DXMatrixIdentity(&m_rightProjection);	

	m_currentRenderingSide = vireio::Left;
	m_pCurrentMatViewTransform = &m_spShaderViewAdjustment->LeftAdjustmentMatrix(); 
	m_pCurrentView = &m_leftView;
	m_pCurrentProjection = &m_leftProjection;

	// get pixel shader max constants
	auto major_ps=D3DSHADER_VERSION_MAJOR(capabilities.PixelShaderVersion);
	auto minor_ps=D3DSHADER_VERSION_MINOR(capabilities.PixelShaderVersion);
	DWORD MaxPixelShaderConst = MAX_PIXEL_SHADER_CONST_2_0;
	if ((major_ps>=2) && (minor_ps>0)) MaxPixelShaderConst = MAX_PIXEL_SHADER_CONST_2_X;
	if ((major_ps>=3) && (minor_ps>=0)) MaxPixelShaderConst = MAX_PIXEL_SHADER_CONST_3_0;

	m_spManagedShaderRegisters = std::make_shared<ShaderRegisters>(MaxPixelShaderConst, capabilities.MaxVertexShaderConst, pDevice);	
	m_pActiveStereoDepthStencil = NULL;
	m_pActiveIndicies = NULL;
	m_pActivePixelShader = NULL;
	m_pActiveVertexShader = NULL;
	m_pActiveVertexDeclaration = NULL;
	m_bActiveViewportIsDefault = true;
	m_bViewportIsSquished = false;
	m_bViewTransformSet = false;
	m_bProjectionTransformSet = false;
	m_bInBeginEndStateBlock = false;
	m_pCapturingStateTo = NULL;
	m_isFirstBeginSceneOfFrame = true;
	yaw_mode = 0;
	pitch_mode = 0;
	translation_mode = 0;
	trackingOn = true;
	InitBrassa();
	//Create Direct Input Mouse Device
	bool directInputActivated = dinput.Init(GetModuleHandle(NULL), ::GetActiveWindow());
	if(directInputActivated)
	{		
		dinput.Activate();		
	}	

	std::string date(__DATE__);
	std::string buildDate = date.substr(4, 2) + "-" + date.substr(0, 3) + "-" + date.substr(7, 4);

	//Show a splash screen on startup
	VireioPopup splashPopup(VPT_SPLASH, VPS_TOAST, 10000);
	strcpy_s(splashPopup.line1, "Vireio Perception: Stereoscopic 3D Driver");
	strcpy_s(splashPopup.line2, (std::string("Version: ") + APP_VERSION + "   Build Date: " + buildDate).c_str());
	strcpy_s(splashPopup.line3, "This program is distributed in the hope that it will be useful,"); 
	strcpy_s(splashPopup.line4, "but WITHOUT ANY WARRANTY; without even the implied warranty of "); 
	strcpy_s(splashPopup.line5, "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
	strcpy_s(splashPopup.line6, "See the GNU LGPL: http://www.gnu.org/licenses/ for more details. ");
	ShowPopup(splashPopup);



	config         = cfg;
	m_configBackup = cfg;

	eyeShutter = 1;
	m_bfloatingMenu = false;
	m_bfloatingScreen = false;
	m_bSurpressHeadtracking = false;

	// first time configuration
	m_spShaderViewAdjustment->Load(config);
	m_pGameHandler->Load(config, m_spShaderViewAdjustment);
	stereoView                 = new StereoView( config );
	stereoView->YOffset        = config.YOffset;
	stereoView->HeadYOffset    = 0;
	stereoView->IPDOffset      = config.IPDOffset;
	stereoView->DistortionScale= config.DistortionScale;
	m_maxDistortionScale       = config.DistortionScale;

	BRASSA_UpdateDeviceSettings();
	OnCreateOrRestore();
}

/**
* Destructor : calls ReleaseEverything() and releases swap chains.
* @see ReleaseEverything()
***/
D3DProxyDevice::~D3DProxyDevice()
{

	#ifdef SHOW_CALLS
		OutputDebugStringA("called ~D3DProxyDevice");
	#endif
	ReleaseEverything();

	m_spShaderViewAdjustment.reset();

	delete m_pGameHandler;
	m_spManagedShaderRegisters.reset();

	FreeLibrary(hmVRboost);

	// always do this last
	auto it = m_activeSwapChains.begin();
	while (it != m_activeSwapChains.end()) {

		if ((*it) != NULL) {
			(*it)->Release();
			delete (*it);
		}

		it = m_activeSwapChains.erase(it);
	}

#ifdef _EXPORT_LOGFILE
	m_logFile.close();
#endif

	if(actual)
		actual->Release();
}

#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))
/**
* Catch QueryInterface calls and increment the reference counter if necesarry. 
***/

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , QueryInterface , REFIID , riid , LPVOID* , ppv )

	//DEFINE_GUID(IID_IDirect3DDevice9Ex, 0xb18b10ce, 0x2649, 0x405a, 0x87, 0xf, 0x95, 0xf7, 0x77, 0xd4, 0x31, 0x3a);
	IF_GUID(riid,0xb18b10ce,0x2649,0x405a,0x87,0xf,0x95,0xf7,0x77,0xd4,0x31,0x3a)
	{
		if (ppv==NULL)
			return E_POINTER;

		this->AddRef();
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return actual->QueryInterface(riid,ppv);
}



METHOD_IMPL( ULONG , WINAPI , D3DProxyDevice , AddRef )	 
	return ++m_nRefCount;
}


METHOD_IMPL( ULONG , WINAPI , D3DProxyDevice , Release )
	if(--m_nRefCount == 0){
		delete this;
		return 0;
	}	
	return m_nRefCount;
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetDirect3D , IDirect3D9** , ppD3D9 )
	if (!m_pCreatedBy){
		return D3DERR_INVALIDCALL;
	}

	*ppD3D9 = m_pCreatedBy;
	m_pCreatedBy->AddRef();
	return D3D_OK;
}


// Calls SetCursorProperties() using the actual left surface from the proxy of pCursorBitmap.
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetCursorProperties , UINT , XHotSpot , UINT , YHotSpot , IDirect3DSurface9* , pCursorBitmap )
	if (!pCursorBitmap){
		return actual->SetCursorProperties(XHotSpot, YHotSpot, NULL);
	}
	return actual->SetCursorProperties(XHotSpot, YHotSpot, static_cast<D3D9ProxySurface*>(pCursorBitmap)->actual );
}


/**
* Creates a proxy (or wrapped) swap chain.
* @param pSwapChain [in, out] Proxy (wrapped) swap chain to be returned.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateAdditionalSwapChain , D3DPRESENT_PARAMETERS* , pPresentationParameters , IDirect3DSwapChain9** , pSwapChain )
	IDirect3DSwapChain9* pActualSwapChain;
	HRESULT result = actual->CreateAdditionalSwapChain(pPresentationParameters, &pActualSwapChain);

	if (SUCCEEDED(result)) {
		D3D9ProxySwapChain* wrappedSwapChain = new D3D9ProxySwapChain(pActualSwapChain, this, true);
		*pSwapChain = wrappedSwapChain;
		m_activeSwapChains.push_back(wrappedSwapChain);
	}

	return result;
}

/**
* Provides the swap chain from the intern vector of active proxy (wrapped) swap chains.
* @param pSwapChain [in, out] The proxy (wrapped) swap chain to be returned.
* @see D3D9ProxySwapChain
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetSwapChain , UINT , iSwapChain , IDirect3DSwapChain9** , pSwapChain )

	try {
		*pSwapChain = m_activeSwapChains.at(iSwapChain); 
		//Device->GetSwapChain increases ref count on the chain (docs don't say this)
		(*pSwapChain)->AddRef();
	}
	catch (std::out_of_range) {
		OutputDebugStringA("GetSwapChain: out of range fetching swap chain");
		return D3DERR_INVALIDCALL;
	}

	return D3D_OK;
}






/**
* Calls the backbuffer using the stored active proxy (wrapped) swap chain.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetBackBuffer , UINT , iSwapChain, UINT , iBackBuffer , D3DBACKBUFFER_TYPE , Type , IDirect3DSurface9** , ppBackBuffer )
	HRESULT result;
	try {
		result = m_activeSwapChains.at(iSwapChain)->GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
		// ref count increase happens in the swapchain GetBackBuffer so we don't add another ref here as we are just passing the value through
	}
	catch (std::out_of_range) {
		OutputDebugStringA("GetBackBuffer: out of range getting swap chain");
		result = D3DERR_INVALIDCALL;
	}

	return result;
}


/**
* Creates a proxy (or wrapped) texture (D3DProxyTexture).
* Texture to be created only gets both stereo textures if game handler agrees.
* @see D3DProxyTexture
* @see GameHandler::ShouldDuplicateTexture()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateTexture , UINT , Width , UINT , Height , UINT , Levels , DWORD , Usage , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DTexture9** , ppTexture , HANDLE* , pSharedHandle )
	HRESULT creationResult;
	IDirect3DTexture9* pLeftTexture = NULL;
	IDirect3DTexture9* pRightTexture = NULL;	

	// try and create left
	if (SUCCEEDED(creationResult = actual->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &pLeftTexture, pSharedHandle))) {

		// Does this Texture need duplicating?
		if (m_pGameHandler->ShouldDuplicateTexture(Width, Height, Levels, Usage, Format, Pool)) {

			if (FAILED(actual->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &pRightTexture, pSharedHandle))) {
				OutputDebugStringA("Failed to create right eye texture while attempting to create stereo pair, falling back to mono\n");
				pRightTexture = NULL;
			}
		}
	}
	else {
		OutputDebugStringA("Failed to create texture\n"); 
	}

	if (SUCCEEDED(creationResult))
		*ppTexture = new D3D9ProxyTexture(pLeftTexture, pRightTexture, this);

	return creationResult;
}

/**
* Creates a a proxy (or wrapped) volume texture (D3D9ProxyVolumeTexture).
* Volumes can't be used as render targets and therefore don't need to be stereo (in DX9).
* @see D3D9ProxyVolumeTexture
***/	
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateVolumeTexture , UINT , Width , UINT , Height , UINT , Depth , UINT , Levels , DWORD , Usage , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DVolumeTexture9** , ppVolumeTexture , HANDLE* , pSharedHandle )
	IDirect3DVolumeTexture9* pActualTexture = NULL;
	HRESULT creationResult = actual->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, &pActualTexture, pSharedHandle);

	if (SUCCEEDED(creationResult))
		*ppVolumeTexture = new D3D9ProxyVolumeTexture(pActualTexture, this);

	return creationResult;
}

/**
* Creates a proxy (or wrapped) cube texture (D3D9ProxyCubeTexture).
* Texture to be created only gets both stereo textures if game handler agrees.
* @see D3D9ProxyCubeTexture
* @see GameHandler::ShouldDuplicateCubeTexture() 
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateCubeTexture , UINT , EdgeLength , UINT , Levels , DWORD , Usage , D3DFORMAT , Format, D3DPOOL , Pool , IDirect3DCubeTexture9** , ppCubeTexture , HANDLE* , pSharedHandle )
	HRESULT creationResult;
	IDirect3DCubeTexture9* pLeftCubeTexture = NULL;
	IDirect3DCubeTexture9* pRightCubeTexture = NULL;	

	// try and create left
	if (SUCCEEDED(creationResult = actual->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, &pLeftCubeTexture, pSharedHandle))) {

		// Does this Texture need duplicating?
		if (m_pGameHandler->ShouldDuplicateCubeTexture(EdgeLength, Levels, Usage, Format, Pool)) {

			if (FAILED(actual->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, &pRightCubeTexture, pSharedHandle))) {
				OutputDebugStringA("Failed to create right eye texture while attempting to create stereo pair, falling back to mono\n");
				pRightCubeTexture = NULL;
			}
		}
	}
	else {
		OutputDebugStringA("Failed to create texture\n"); 
	}

	if (SUCCEEDED(creationResult))
		*ppCubeTexture = new D3D9ProxyCubeTexture(pLeftCubeTexture, pRightCubeTexture, this);

	return creationResult;
}

/**
* Creates base vertex buffer pointer (D3D9ProxyVertexBuffer).
* @see D3D9ProxyVertexBuffer
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateVertexBuffer , UINT , Length , DWORD , Usage , DWORD , FVF , D3DPOOL , Pool, IDirect3DVertexBuffer9** , ppVertexBuffer , HANDLE* , pSharedHandle )
	IDirect3DVertexBuffer9* pActualBuffer = NULL;
	HRESULT creationResult = actual->CreateVertexBuffer(Length, Usage, FVF, Pool, &pActualBuffer, pSharedHandle);

	if (SUCCEEDED(creationResult))
		*ppVertexBuffer = new D3D9ProxyVertexBuffer(pActualBuffer, this);

	return creationResult;
}

/**
* * Creates base index buffer pointer (D3D9ProxyIndexBuffer).
* @see D3D9ProxyIndexBuffer
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateIndexBuffer , UINT , Length , DWORD , Usage , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DIndexBuffer9** , ppIndexBuffer , HANDLE* , pSharedHandle )
	IDirect3DIndexBuffer9* pActualBuffer = NULL;
	HRESULT creationResult = actual->CreateIndexBuffer(Length, Usage, Format, Pool, &pActualBuffer, pSharedHandle);

	if (SUCCEEDED(creationResult))
		*ppIndexBuffer = new D3D9ProxyIndexBuffer(pActualBuffer, this);

	return creationResult;
}


/**
* Copies rectangular subsets of pixels from one proxy (wrapped) surface to another.
* @see D3D9ProxySurface
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , UpdateSurface , IDirect3DSurface9* , pSourceSurface , CONST RECT* , pSourceRect , IDirect3DSurface9* , pDestinationSurface , CONST POINT* , pDestPoint )
	if (!pSourceSurface || !pDestinationSurface)
		return D3DERR_INVALIDCALL;

	IDirect3DSurface9* pSourceSurfaceLeft = static_cast<D3D9ProxySurface*>(pSourceSurface)->actual;
	IDirect3DSurface9* pSourceSurfaceRight = static_cast<D3D9ProxySurface*>(pSourceSurface)->right;
	IDirect3DSurface9* pDestSurfaceLeft = static_cast<D3D9ProxySurface*>(pDestinationSurface)->actual;
	IDirect3DSurface9* pDestSurfaceRight = static_cast<D3D9ProxySurface*>(pDestinationSurface)->right;

	HRESULT result = actual->UpdateSurface(pSourceSurfaceLeft, pSourceRect, pDestSurfaceLeft, pDestPoint);

	if (SUCCEEDED(result)) {
		if (!pSourceSurfaceRight && pDestSurfaceRight) {
			//OutputDebugStringA("INFO: UpdateSurface - Source is not stereo, destination is stereo. Copying source to both sides of destination.\n");

			if (FAILED(actual->UpdateSurface(pSourceSurfaceLeft, pSourceRect, pDestSurfaceRight, pDestPoint))) {
				OutputDebugStringA("ERROR: UpdateSurface - Failed to copy source left to destination right.\n");
			}
		} 
		else if (pSourceSurfaceRight && !pDestSurfaceRight) {
			//OutputDebugStringA("INFO: UpdateSurface - Source is stereo, destination is not stereo. Copied Left side only.\n");
		}
		else if (pSourceSurfaceRight && pDestSurfaceRight)	{
			if (FAILED(actual->UpdateSurface(pSourceSurfaceRight, pSourceRect, pDestSurfaceRight, pDestPoint))) {
				OutputDebugStringA("ERROR: UpdateSurface - Failed to copy source right to destination right.\n");
			}
		}
	}

	return result;
}

/**
* Calls a helper function to unwrap the textures and calls the super method for both sides.
* The super method updates the dirty portions of a texture.
* @see vireio::UnWrapTexture()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , UpdateTexture , IDirect3DBaseTexture9* , pSourceTexture , IDirect3DBaseTexture9* , pDestinationTexture )
	if (!pSourceTexture || !pDestinationTexture)
		return D3DERR_INVALIDCALL;


	IDirect3DBaseTexture9* pSourceTextureLeft = NULL;
	IDirect3DBaseTexture9* pSourceTextureRight = NULL;
	IDirect3DBaseTexture9* pDestTextureLeft = NULL;
	IDirect3DBaseTexture9* pDestTextureRight = NULL;

	vireio::UnWrapTexture(pSourceTexture, &pSourceTextureLeft, &pSourceTextureRight);
	vireio::UnWrapTexture(pDestinationTexture, &pDestTextureLeft, &pDestTextureRight);

	HRESULT result = actual->UpdateTexture(pSourceTextureLeft, pDestTextureLeft);

	if (SUCCEEDED(result)) {
		if (!pSourceTextureRight && pDestTextureRight) {
			//OutputDebugStringA("INFO: UpdateTexture - Source is not stereo, destination is stereo. Copying source to both sides of destination.\n");

			if (FAILED(actual->UpdateTexture(pSourceTextureLeft, pDestTextureRight))) {
				OutputDebugStringA("ERROR: UpdateTexture - Failed to copy source left to destination right.\n");
			}
		} 
		else if (pSourceTextureRight && !pDestTextureRight) {
			//OutputDebugStringA("INFO: UpdateTexture - Source is stereo, destination is not stereo. Copied Left side only.\n");
		}
		else if (pSourceTextureRight && pDestTextureRight)	{
			if (FAILED(actual->UpdateTexture(pSourceTextureRight, pDestTextureRight))) {
				OutputDebugStringA("ERROR: UpdateTexture - Failed to copy source right to destination right.\n");
			}
		}
	}

	return result;
}

/**
* Copies the render-target data from proxy (wrapped) source surface to proxy (wrapped) destination surface.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetRenderTargetData , IDirect3DSurface9* , pRenderTarget , IDirect3DSurface9* , pDestSurface )
	if ((pDestSurface == NULL) || (pRenderTarget == NULL))
		return D3DERR_INVALIDCALL;

	D3D9ProxySurface* pWrappedRenderTarget = static_cast<D3D9ProxySurface*>(pRenderTarget);
	D3D9ProxySurface* pWrappedDest = static_cast<D3D9ProxySurface*>(pDestSurface);

	IDirect3DSurface9* pRenderTargetLeft = pWrappedRenderTarget->actual;
	IDirect3DSurface9* pRenderTargetRight = pWrappedRenderTarget->right;
	IDirect3DSurface9* pDestSurfaceLeft = pWrappedDest->actual;
	IDirect3DSurface9* pDestSurfaceRight = pWrappedDest->right;

	HRESULT result = actual->GetRenderTargetData(pRenderTargetLeft, pDestSurfaceLeft);

	if (SUCCEEDED(result)) {
		if (!pRenderTargetRight && pDestSurfaceRight) {
			//OutputDebugStringA("INFO: GetRenderTargetData - Source is not stereo, destination is stereo. Copying source to both sides of destination.\n");

			if (FAILED(actual->GetRenderTargetData(pRenderTargetLeft, pDestSurfaceRight))) {
				OutputDebugStringA("ERROR: GetRenderTargetData - Failed to copy source left to destination right.\n");
			}
		} 
		else if (pRenderTargetRight && !pDestSurfaceRight) {
			//OutputDebugStringA("INFO: GetRenderTargetData - Source is stereo, destination is not stereo. Copied Left side only.\n");
		}
		else if (pRenderTargetRight && pDestSurfaceRight)	{
			if (FAILED(actual->GetRenderTargetData(pRenderTargetRight, pDestSurfaceRight))) {
				OutputDebugStringA("ERROR: GetRenderTargetData - Failed to copy source right to destination right.\n");
			}
		}
	}

	return result;
}

/**
* Gets the front buffer data from the internal stored active proxy (or wrapped) swap chain.
* @see D3D9ProxySwapChain
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetFrontBufferData , UINT , iSwapChain , IDirect3DSurface9* , pDestSurface )
	HRESULT result;
	try {
		result = m_activeSwapChains.at(iSwapChain)->GetFrontBufferData(pDestSurface);
	}
	catch (std::out_of_range) {
		OutputDebugStringA("GetFrontBufferData: out of range fetching swap chain");
		result = D3DERR_INVALIDCALL;
	}

	return result;
}

/**
* Copy the contents of the source proxy (wrapped) surface rectangles to the destination proxy (wrapped) surface rectangles.
* @see D3D9ProxySurface
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , StretchRect , IDirect3DSurface9* , pSourceSurface , CONST RECT* , pSourceRect , IDirect3DSurface9* , pDestSurface , CONST RECT* , pDestRect , D3DTEXTUREFILTERTYPE , Filter )
	if (!pSourceSurface || !pDestSurface)
		return D3DERR_INVALIDCALL;

	D3D9ProxySurface* pWrappedSource = static_cast<D3D9ProxySurface*>(pSourceSurface);
	D3D9ProxySurface* pWrappedDest = static_cast<D3D9ProxySurface*>(pDestSurface);

	IDirect3DSurface9* pSourceSurfaceLeft = pWrappedSource->actual;
	IDirect3DSurface9* pSourceSurfaceRight = pWrappedSource->right;
	IDirect3DSurface9* pDestSurfaceLeft = pWrappedDest->actual;
	IDirect3DSurface9* pDestSurfaceRight = pWrappedDest->right;

	HRESULT result = actual->StretchRect(pSourceSurfaceLeft, pSourceRect, pDestSurfaceLeft, pDestRect, Filter);

	if (SUCCEEDED(result)) {
		if (!pSourceSurfaceRight && pDestSurfaceRight) {
			//OutputDebugStringA("INFO: StretchRect - Source is not stereo, destination is stereo. Copying source to both sides of destination.\n");

			if (FAILED(actual->StretchRect(pSourceSurfaceLeft, pSourceRect, pDestSurfaceRight, pDestRect, Filter))) {
				OutputDebugStringA("ERROR: StretchRect - Failed to copy source left to destination right.\n");
			}
		} 
		else if (pSourceSurfaceRight && !pDestSurfaceRight) {
			//OutputDebugStringA("INFO: StretchRect - Source is stereo, destination is not stereo. Copied Left side only.\n");
		}
		else if (pSourceSurfaceRight && pDestSurfaceRight)	{
			if (FAILED(actual->StretchRect(pSourceSurfaceRight, pSourceRect, pDestSurfaceRight, pDestRect, Filter))) {
				OutputDebugStringA("ERROR: StretchRect - Failed to copy source right to destination right.\n");
			}
		}
	}

	return result;
}

/**
* Fills the rectangle for both stereo sides if switchDrawingSide() agrees and sets the render target accordingly.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , ColorFill, IDirect3DSurface9* , pSurface , CONST RECT* , pRect , D3DCOLOR , color )
	HRESULT result;

	D3D9ProxySurface* pDerivedSurface = static_cast<D3D9ProxySurface*> (pSurface);
	if (SUCCEEDED(result = actual->ColorFill(pDerivedSurface->actual, pRect, color)))
	{
		if (pDerivedSurface->right){
			actual->ColorFill(pDerivedSurface->right, pRect, color);
		}
	}

	return result;
}

/**
* Updates render target accordingly to current render side.
* Updates proxy collection of stereo render targets to reflect new actual render target.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetRenderTarget , DWORD , RenderTargetIndex , IDirect3DSurface9* , pRenderTarget )
	D3D9ProxySurface* newRenderTarget = static_cast<D3D9ProxySurface*>(pRenderTarget);

#ifdef _DEBUG
	if (newRenderTarget && !newRenderTarget->actual && !newRenderTarget->right) {
		OutputDebugStringA("RenderTarget is not a valid (D3D9ProxySurface) stereo capable surface\n"); 
	}
#endif

	//// Update actual render target ////
	HRESULT result;

	// Removing a render target
	if (newRenderTarget == NULL) {
		if (RenderTargetIndex == 0) {
			// main render target should never be set to NULL
			result = D3DERR_INVALIDCALL; 
		}		
		else {
			result = actual->SetRenderTarget(RenderTargetIndex, NULL);
		}
	}
	// Setting a render target
	else {
		if (m_currentRenderingSide == vireio::Left) {
			result = actual->SetRenderTarget(RenderTargetIndex, newRenderTarget->actual);
		}
		else {
			result = actual->SetRenderTarget(RenderTargetIndex, newRenderTarget->right);
		}
	}

	//// update proxy collection of stereo render targets to reflect new actual render target ////
	if (result == D3D_OK) {		
		// changing rendertarget resets viewport to fullsurface
		m_bActiveViewportIsDefault = true;

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

/**
* Provides render target from the internally stored active proxy (wrapped) render targets.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetRenderTarget , DWORD , RenderTargetIndex , IDirect3DSurface9** , ppRenderTarget )
	if ((RenderTargetIndex >= m_activeRenderTargets.capacity()) || (RenderTargetIndex < 0)) {
		return D3DERR_INVALIDCALL;
	}

	IDirect3DSurface9* targetToReturn = m_activeRenderTargets[RenderTargetIndex];
	if (!targetToReturn)
		return D3DERR_NOTFOUND;
	else {
		*ppRenderTarget = targetToReturn;
		targetToReturn->AddRef();
		return D3D_OK;
	}
}

/**
* Updates depth stencil accordingly to current render side.
* Updates stored proxy (or wrapped) depth stencil.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetDepthStencilSurface , IDirect3DSurface9* , pNewZStencil )
	D3D9ProxySurface* pNewDepthStencil = static_cast<D3D9ProxySurface*>(pNewZStencil);

	IDirect3DSurface9* pActualStencilForCurrentSide = NULL;
	if (pNewDepthStencil) {
		if (m_currentRenderingSide == vireio::Left)
			pActualStencilForCurrentSide = pNewDepthStencil->actual;
		else
			pActualStencilForCurrentSide = pNewDepthStencil->right;
	}

	// Update actual depth stencil
	HRESULT result = actual->SetDepthStencilSurface(pActualStencilForCurrentSide);

	// Update stored proxy depth stencil
	if (SUCCEEDED(result)) {
		if (m_pActiveStereoDepthStencil) {
			m_pActiveStereoDepthStencil->Release();
		}

		m_pActiveStereoDepthStencil = pNewDepthStencil;
		if (m_pActiveStereoDepthStencil) {
			m_pActiveStereoDepthStencil->AddRef();
		}
	}

	return result;
}

/**
* Provides the active proxy (wrapped) depth stencil.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetDepthStencilSurface , IDirect3DSurface9** , ppZStencilSurface )
	if (!m_pActiveStereoDepthStencil)
		return D3DERR_NOTFOUND;

	*ppZStencilSurface = m_pActiveStereoDepthStencil;
	(*ppZStencilSurface)->AddRef();

	return D3D_OK;
}

/**
* Updates tracker if device says it should.  Handles controls if this is the first scene of the frame.
* Because input for this frame would already have been handled here so injection of any mouse 
* manipulation ?
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , BeginScene )
	if (tracker)
		tracker->BeginFrame();

	if (m_isFirstBeginSceneOfFrame) {

		// save screenshot before first clear() is called
		if (screenshot>0)
		{
			if (screenshot==1)
				stereoView->SaveScreen();
			screenshot--;
		}

		// set last frame vertex shader count
		m_VertexShaderCountLastFrame = m_VertexShaderCount;

		// avoid squished viewport in case of brassa menu being drawn
		if ((m_bViewportIsSquished) && (BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
		{
			if (m_bViewportIsSquished)
				actual->SetViewport(&m_LastViewportSet);
			m_bViewportIsSquished = false;
		}

		// handle controls 
		if ((m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::WhenToDo::BEGIN_SCENE) || (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::WhenToDo::FIRST_BEGIN_SCENE))
			HandleTracking();

		// draw BRASSA
		if ((m_deviceBehavior.whenToRenderBRASSA == DeviceBehavior::WhenToDo::BEGIN_SCENE) || (m_deviceBehavior.whenToRenderBRASSA == DeviceBehavior::WhenToDo::FIRST_BEGIN_SCENE))
		{
			if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
				BRASSA();
			else
				BRASSA_AdditionalOutput();
		}

		// handle controls
		HandleControls();

		// set vertex shader call count to zero
		m_VertexShaderCount = 0;
	}
	else
	{
		// handle controls 
		if (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::WhenToDo::BEGIN_SCENE)
			HandleTracking();

		// draw BRASSA
		if (m_deviceBehavior.whenToRenderBRASSA == DeviceBehavior::WhenToDo::BEGIN_SCENE)
		{
			if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
				BRASSA();
			else
				BRASSA_AdditionalOutput();
		}
	}

	

	return actual->BeginScene();
}

/**
* BRASSA called here for source engine games.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , EndScene )
	// handle controls 
	if (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::WhenToDo::END_SCENE) 
		HandleTracking();

	// draw BRASSA
	if (m_deviceBehavior.whenToRenderBRASSA == DeviceBehavior::WhenToDo::END_SCENE) 
	{
		if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
			BRASSA();
		else
			BRASSA_AdditionalOutput();
	}

	if (tracker)
		tracker->EndFrame();

	return actual->EndScene();
}

/**
* Clears both stereo sides if switchDrawingSide() agrees.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , Clear , DWORD , Count , CONST D3DRECT* , pRects , DWORD , Flags , D3DCOLOR , Color , float , Z , DWORD , Stencil )
	HRESULT result;
	if (SUCCEEDED(result = actual->Clear(Count, pRects, Flags, Color, Z, Stencil))) {
		if (switchDrawingSide()) {

			HRESULT hr;
			if (FAILED(hr = actual->Clear(Count, pRects, Flags, Color, Z, Stencil))) {

#ifdef _DEBUG
				char buf[256];
				sprintf_s(buf, "Error: %s error description: %s\n",
				DXGetErrorString(hr), DXGetErrorDescription(hr));

				OutputDebugStringA(buf);
				OutputDebugStringA("Clear failed\n");

#endif

			}
		}
	}

	return result;
}

/**
* Catches transform for stored proxy state block accordingly or updates proxy device.
* @see D3D9ProxyStateBlock
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetTransform , D3DTRANSFORMSTATETYPE , State , CONST D3DMATRIX* , pMatrix )
	if(State == D3DTS_VIEW)
	{
		D3DXMATRIX tempLeft;
		D3DXMATRIX tempRight;
		D3DXMATRIX* pViewToSet = NULL;
		bool tempIsTransformSet = false;

		if (!pMatrix) {
			D3DXMatrixIdentity(&tempLeft);
			D3DXMatrixIdentity(&tempRight);
		}
		else {

			D3DXMATRIX sourceMatrix(*pMatrix);

			// If the view is set to the identity then we don't need to perform any adjustments
			if (D3DXMatrixIsIdentity(&sourceMatrix)) {

				D3DXMatrixIdentity(&tempLeft);
				D3DXMatrixIdentity(&tempRight);
			}
			else {
				// If the view matrix is modified we need to apply left/right adjustments (for stereo rendering)
				tempLeft = sourceMatrix * m_spShaderViewAdjustment->LeftViewTransform();
				tempRight = sourceMatrix * m_spShaderViewAdjustment->RightViewTransform();

				tempIsTransformSet = true;
			}
		}


		// If capturing state block capture without updating proxy device
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureViewTransform(tempLeft, tempRight);
			if (m_currentRenderingSide == vireio::Left) {
				pViewToSet = &tempLeft;
			}
			else {
				pViewToSet = &tempRight;
			}
		}
		else { // otherwise update proxy device

			m_bViewTransformSet = tempIsTransformSet;
			m_leftView = tempLeft;
			m_rightView = tempRight;

			if (m_currentRenderingSide == vireio::Left) {
				m_pCurrentView = &m_leftView;
			}
			else {
				m_pCurrentView = &m_rightView;
			}

			pViewToSet = m_pCurrentView;
		}

		return actual->SetTransform(State, pViewToSet);

	}
	else if(State == D3DTS_PROJECTION)
	{

		D3DXMATRIX tempLeft;
		D3DXMATRIX tempRight;
		D3DXMATRIX* pProjectionToSet = NULL;
		bool tempIsTransformSet = false;

		if (!pMatrix) {

			D3DXMatrixIdentity(&tempLeft);
			D3DXMatrixIdentity(&tempRight);
		}
		else {
			D3DXMATRIX sourceMatrix(*pMatrix);

			// world scale mode ? in case, add all possible actual game x scale units
			if (BRASSA_mode == BRASSA_Modes::WORLD_SCALE_CALIBRATION)
			{
				// store the actual projection matrix for game unit calculation
				D3DXMATRIX m_actualProjection = D3DXMATRIX(*pMatrix);

				// get the scale the 
				float gameXScale = m_actualProjection._11;

				// add if not present
				if (std::find(m_gameXScaleUnits.begin(), m_gameXScaleUnits.end(), gameXScale) == m_gameXScaleUnits.end()) {
					m_gameXScaleUnits.push_back(gameXScale);
				}
			}

			// If the view is set to the identity then we don't need to perform any adjustments
			if (D3DXMatrixIsIdentity(&sourceMatrix)) {

				D3DXMatrixIdentity(&tempLeft);
				D3DXMatrixIdentity(&tempRight);
			}
			else {


				tempLeft = sourceMatrix;
				tempRight = sourceMatrix;

				tempIsTransformSet = true;
			}			
		}

		// If capturing state block capture without updating proxy device
		if (m_pCapturingStateTo) {

			m_pCapturingStateTo->SelectAndCaptureProjectionTransform(tempLeft, tempRight);
			if (m_currentRenderingSide == vireio::Left) {
				pProjectionToSet = &tempLeft;
			}
			else {
				pProjectionToSet = &tempRight;
			}
		}
		else { // otherwise update proxy device

			m_bProjectionTransformSet = tempIsTransformSet;
			m_leftProjection = tempLeft;
			m_rightProjection = tempRight;

			if (m_currentRenderingSide == vireio::Left) {
				m_pCurrentProjection = &m_leftProjection;
			}
			else {
				m_pCurrentProjection = &m_rightProjection;
			}

			pProjectionToSet = m_pCurrentProjection;
		}

		return actual->SetTransform(State, pProjectionToSet);
	}

	return actual->SetTransform(State, pMatrix);
}





/**
* Try and set, if success save viewport.
* Also, it captures the viewport state in stored proxy state block.
* If viewport width and height match primary render target size and zmin is 0 and zmax 1 set 
* m_bActiveViewportIsDefault flag true.
* @see D3D9ProxyStateBlock::SelectAndCaptureState()
* @see m_bActiveViewportIsDefault
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetViewport , CONST D3DVIEWPORT9* , pViewport )	
	HRESULT result = actual->SetViewport(pViewport);

	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(*pViewport);
		}
		else {
			m_bActiveViewportIsDefault = isViewportDefaultForMainRT(pViewport);
			m_LastViewportSet = *pViewport;
		}
	}

	
	if (m_bViewportIsSquished)
		SetGUIViewport();
	
	return result;
}





/**
* Creates proxy state block.
* Also, selects capture type option according to state block type.
* @param ppSB [in, out] The proxy (or wrapped) state block returned.
* @see D3DProxyStateBlock
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateStateBlock , D3DSTATEBLOCKTYPE , Type , IDirect3DStateBlock9** , ppSB )
	IDirect3DStateBlock9* pActualStateBlock = NULL;
	HRESULT creationResult = actual->CreateStateBlock(Type, &pActualStateBlock);

	if (SUCCEEDED(creationResult)) {

		D3D9ProxyStateBlock::CaptureType capType;

		switch (Type) {
		case D3DSBT_ALL: 
			{
				capType = D3D9ProxyStateBlock::Cap_Type_Full;
				break;
			}

		case D3DSBT_PIXELSTATE: 
			{
				capType = D3D9ProxyStateBlock::Cap_Type_Pixel;
				break;
			}

		case D3DSBT_VERTEXSTATE: 
			{
				capType = D3D9ProxyStateBlock::Cap_Type_Vertex;
				break;
			}

		default:
			{
				capType = D3D9ProxyStateBlock::Cap_Type_Full;
				break;
			}    
		}

		*ppSB = new D3D9ProxyStateBlock(pActualStateBlock, this, capType, m_currentRenderingSide == vireio::Left);
	}

	return creationResult;
}

/**
* Creates and stores proxy state block.
* @see D3DProxyStateBlock
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , BeginStateBlock )
	HRESULT result;
	if (SUCCEEDED(result = actual->BeginStateBlock())) {
		m_bInBeginEndStateBlock = true;
		m_pCapturingStateTo = new D3D9ProxyStateBlock(NULL, this, D3D9ProxyStateBlock::Cap_Type_Selected, m_currentRenderingSide == vireio::Left);
	}

	return result;
}

/**
* Calls both super method and method from stored proxy state block.
* @param [in, out] The returned proxy (or wrapped) state block.
* @see D3D9ProxyStateBlock::EndStateBlock()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , EndStateBlock , IDirect3DStateBlock9** , ppSB )
	IDirect3DStateBlock9* pActualStateBlock = NULL;
	HRESULT creationResult = actual->EndStateBlock(&pActualStateBlock);

	if (SUCCEEDED(creationResult)) {
		m_pCapturingStateTo->EndStateBlock(pActualStateBlock);
		*ppSB = m_pCapturingStateTo;
	}
	else {
		m_pCapturingStateTo->Release();
		if (m_pCapturingStateTo) delete m_pCapturingStateTo;
	}

	m_pCapturingStateTo = NULL;
	m_bInBeginEndStateBlock = false;

	return creationResult;
}



/**
* Provides texture from stored active (mono) texture stages.
* @see D3D9ProxyTexture
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetTexture , DWORD , Stage , IDirect3DBaseTexture9** , ppTexture )
	if (m_activeTextureStages.count(Stage) != 1)
		return D3DERR_INVALIDCALL;
	else {
		*ppTexture = m_activeTextureStages[Stage];
		if ((*ppTexture))
			(*ppTexture)->AddRef();
		return D3D_OK;
	}
}

/**
* Calls a helper function to unwrap the textures and calls the super method for both sides.
* Update stored active (mono) texture stages if new texture was successfully set.
*
* @see vireio::UnWrapTexture() 
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetTexture , DWORD , Stage , IDirect3DBaseTexture9* , pTexture )
	HRESULT result;
	if (pTexture) {

		IDirect3DBaseTexture9* pActualLeftTexture = NULL;
		IDirect3DBaseTexture9* pActualRightTexture = NULL;

		vireio::UnWrapTexture(pTexture, &pActualLeftTexture, &pActualRightTexture);

		// Try and Update the actual devices textures
		if ((pActualRightTexture == NULL) || (m_currentRenderingSide == vireio::Left)) // use left (mono) if not stereo or one left side
			result = actual->SetTexture(Stage, pActualLeftTexture);
		else
			result = actual->SetTexture(Stage, pActualRightTexture);

	}
	else {
		result = actual->SetTexture(Stage, NULL);
	}

	// Update m_activeTextureStages if new texture was successfully set
	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(Stage, pTexture);
		}
		else {

			// remove existing texture that was active at Stage if there is one
			if (m_activeTextureStages.count(Stage) == 1) { 

				IDirect3DBaseTexture9* pOldTexture = m_activeTextureStages.at(Stage);
				if (pOldTexture)
					pOldTexture->Release();

				m_activeTextureStages.erase(Stage);
			}

			// insert new texture (can be a NULL pointer, this is important for StateBlock tracking)
			if(m_activeTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(Stage, pTexture)).second) {
				//success
				if (pTexture)
					pTexture->AddRef();
			}
			else {
				OutputDebugStringA(__FUNCTION__);
				OutputDebugStringA("\n");
				OutputDebugStringA("Unable to store active Texture Stage.\n");
				assert(false);

				//If we get here the state of the texture tracking is fubared and an implosion is imminent.

				result = D3DERR_INVALIDCALL;
			}
		}
	}

	return result;
}

/**
* Base GetTextureStageState functionality.
***/



/**
* Applies all dirty shader registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawPrimitive , D3DPRIMITIVETYPE , PrimitiveType , UINT , StartVertex , UINT , PrimitiveCount )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount))) {
		if (switchDrawingSide())
			actual->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
	}

	return result;
}

/**
* Applies all dirty shader registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawIndexedPrimitive , D3DPRIMITIVETYPE , PrimitiveType , INT , BaseVertexIndex , UINT , MinVertexIndex , UINT , NumVertices , UINT , startIndex , UINT , primCount )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount))) {
		if (switchDrawingSide()) {
			HRESULT result2 = actual->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			if (result != result2)
				OutputDebugStringA("moop\n");
		}
	}

	return result;
}

/**
* Applies all dirty shader registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawPrimitiveUP , D3DPRIMITIVETYPE , PrimitiveType , UINT , PrimitiveCount , CONST void* , pVertexStreamZeroData , UINT , VertexStreamZeroStride )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride))) {
		if (switchDrawingSide())
			actual->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	return result;
}

/**
* Applies all dirty shader registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawIndexedPrimitiveUP , D3DPRIMITIVETYPE , PrimitiveType , UINT , MinVertexIndex , UINT , NumVertices , UINT , PrimitiveCount , CONST void* , pIndexData , D3DFORMAT , IndexDataFormat , CONST void* , pVertexStreamZeroData , UINT , VertexStreamZeroStride )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride))) {
		if (switchDrawingSide())
			actual->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	return result;
}

/**
* Applies all dirty shader registers, processes vertices.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , ProcessVertices , UINT , SrcStartIndex , UINT , DestIndex , UINT , VertexCount , IDirect3DVertexBuffer9* , pDestBuffer , IDirect3DVertexDeclaration9* , pVertexDecl , DWORD , Flags )
	if (!pDestBuffer)
		return D3DERR_INVALIDCALL;

	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	D3D9ProxyVertexBuffer* pCastDestBuffer = static_cast<D3D9ProxyVertexBuffer*>(pDestBuffer);
	D3D9ProxyVertexDeclaration* pCastVertexDeclaration = NULL;

	HRESULT result;
	if (pVertexDecl) {
		pCastVertexDeclaration = static_cast<D3D9ProxyVertexDeclaration*>(pVertexDecl);
		result = actual->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pCastDestBuffer->actual, pCastVertexDeclaration->actual, Flags);
	}
	else {
		result = actual->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pCastDestBuffer->actual, NULL, Flags);
	}

	return result;
}

/**
* Creates base vertex declaration (D3D9ProxyVertexDeclaration).
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateVertexDeclaration , CONST D3DVERTEXELEMENT9* , pVertexElements , IDirect3DVertexDeclaration9** , ppDecl )
	IDirect3DVertexDeclaration9* pActualVertexDeclaration = NULL;
	HRESULT creationResult = actual->CreateVertexDeclaration(pVertexElements, &pActualVertexDeclaration );

	if (SUCCEEDED(creationResult))
		*ppDecl = new D3D9ProxyVertexDeclaration(pActualVertexDeclaration, this);

	return creationResult;
}

/**
* Catches vertex declaration in stored proxy state block.
* First, set vertex declaration by base function.
* @see D3D9ProxyStateBlock
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetVertexDeclaration , IDirect3DVertexDeclaration9* , pDecl )
	D3D9ProxyVertexDeclaration* pWrappedVDeclarationData = static_cast<D3D9ProxyVertexDeclaration*>(pDecl);

	// Update actual Vertex Declaration
	HRESULT result;
	if (pWrappedVDeclarationData)
		result = actual->SetVertexDeclaration(pWrappedVDeclarationData->actual);
	else
		result = actual->SetVertexDeclaration(NULL);

	// Update stored proxy Vertex Declaration
	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(pWrappedVDeclarationData);
		}
		else {

			if (m_pActiveVertexDeclaration) {
				m_pActiveVertexDeclaration->Release();
			}

			m_pActiveVertexDeclaration = pWrappedVDeclarationData;
			if (m_pActiveVertexDeclaration) {
				m_pActiveVertexDeclaration->AddRef();
			}
		}
	}

	return result;
}

/**
* Provides currently stored vertex declaration.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetVertexDeclaration , IDirect3DVertexDeclaration9** , ppDecl )
	if (!m_pActiveVertexDeclaration) 
		// TODO check this is the response if no declaration set
		//In Response to TODO:  JB, Jan 12. I believe it crashes most times this happens, tested by simply nulling out the ppDecl pointer and passing it into the base d3d method
		return D3DERR_INVALIDCALL; 

	*ppDecl = m_pActiveVertexDeclaration;

	return D3D_OK;
}

/**
* Creates proxy (wrapped) vertex shader.
* @param ppShader [in, out] The created proxy vertex shader.
* @see D3D9ProxyVertexShader
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateVertexShader , CONST DWORD* , pFunction , IDirect3DVertexShader9** , ppShader )
	IDirect3DVertexShader9* pActualVShader = NULL;
	HRESULT creationResult = actual->CreateVertexShader(pFunction, &pActualVShader);

	if (SUCCEEDED(creationResult)) {
		*ppShader = new D3D9ProxyVertexShader(pActualVShader, this, m_pGameHandler->GetShaderModificationRepository());
	}

	return creationResult;
}

/**
* Sets and updates stored proxy vertex shader.
* @see D3D9ProxyVertexShader
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetVertexShader , IDirect3DVertexShader9* , pShader )
	D3D9ProxyVertexShader* pWrappedVShaderData = static_cast<D3D9ProxyVertexShader*>(pShader);

	// Update actual Vertex shader
	HRESULT result;
	if (pWrappedVShaderData)
		result = actual->SetVertexShader(pWrappedVShaderData->actual);
	else
		result = actual->SetVertexShader(NULL);

	// Update stored proxy Vertex shader
	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(pWrappedVShaderData);
		}
		else {
			if (m_pActiveVertexShader) {
				m_pActiveVertexShader->Release();
			}

			m_pActiveVertexShader = pWrappedVShaderData;
			if (m_pActiveVertexShader) {
				m_pActiveVertexShader->AddRef();
			}

			m_spManagedShaderRegisters->ActiveVertexShaderChanged(m_pActiveVertexShader);
		}
	}

	if (pWrappedVShaderData)
	{
		if (pWrappedVShaderData->m_bSquishViewport)
			SetGUIViewport();
		else
		{
			if (m_bViewportIsSquished)
				actual->SetViewport(&m_LastViewportSet);
			m_bViewportIsSquished = false;
		}
	}

	// increase vertex shader call count
	++m_VertexShaderCount;
	return result;
}

/**
* Returns the stored and active proxy vertex shader.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetVertexShader , IDirect3DVertexShader9** , ppShader )
	if (!m_pActiveVertexShader)
		return D3DERR_INVALIDCALL;

	*ppShader = m_pActiveVertexShader;

	return D3D_OK;
}

/**
* Sets shader constants either at stored proxy state block or in managed shader register class.
* @see D3D9ProxyStateBlock
* @see ShaderRegisters
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetVertexShaderConstantF , UINT , StartRegister , CONST float* , pConstantData , UINT , Vector4fCount )
	HRESULT result = D3DERR_INVALIDCALL;

	if (m_pCapturingStateTo) {
		result = m_pCapturingStateTo->SelectAndCaptureStateVSConst(StartRegister, pConstantData, Vector4fCount);
	}
	else { 
		result = m_spManagedShaderRegisters->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	return result;
}

/**
* Provides constant registers from managed shader register class.
* @see ShaderRegisters
* @see ShaderRegisters::GetVertexShaderConstantF()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetVertexShaderConstantF , UINT , StartRegister , float* , pData , UINT , Vector4fCount )
	return m_spManagedShaderRegisters->GetVertexShaderConstantF(StartRegister, pData, Vector4fCount);
}






/**
* Sets stream source and updates stored vertex buffers.
* Also, it calls proxy state block to capture states.
* @see D3D9ProxyStateBlock::SelectAndCaptureState()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetStreamSource , UINT , StreamNumber , IDirect3DVertexBuffer9* , pStreamData , UINT , OffsetInBytes , UINT , Stride )	
	D3D9ProxyVertexBuffer* pCastStreamData = static_cast<D3D9ProxyVertexBuffer*>(pStreamData);
	HRESULT result;
	if (pStreamData) {		
		result = actual->SetStreamSource(StreamNumber, pCastStreamData->actual, OffsetInBytes, Stride);
	}
	else {
		result = actual->SetStreamSource(StreamNumber, NULL, OffsetInBytes, Stride);
	}


	// Update m_activeVertexBuffers if new vertex buffer was successfully set
	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(StreamNumber, pCastStreamData);
		}
		else {
			// remove existing vertex buffer that was active at StreamNumber if there is one
			if (m_activeVertexBuffers.count(StreamNumber) == 1) { 

				IDirect3DVertexBuffer9* pOldBuffer = m_activeVertexBuffers.at(StreamNumber);
				if (pOldBuffer == pStreamData)
					return result;

				if (pOldBuffer)
					pOldBuffer->Release();

				m_activeVertexBuffers.erase(StreamNumber);
			}

			// insert new vertex buffer
			if(m_activeVertexBuffers.insert(std::pair<UINT, D3D9ProxyVertexBuffer*>(StreamNumber, pCastStreamData)).second) {
				//success
				if (pStreamData)
					pStreamData->AddRef();
			}
			else {
				OutputDebugStringA(__FUNCTION__);
				OutputDebugStringA("\n");
				OutputDebugStringA("Unable to store active Texture Stage.\n");
				assert(false);

				//If we get here the state of the texture tracking is fubared and an implosion is imminent.

				result = D3DERR_INVALIDCALL;
			}
		}
	}

	return result;
}

/**
* Provides stream data from stored vertex buffers.
* TODO ppStreamData is marked in and out in docs. Potentially it can be a get when the stream hasn't been set before???
* Category of probleme: Worry about it if it breaks.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetStreamSource , UINT , StreamNumber , IDirect3DVertexBuffer9** , ppStreamData , UINT* , pOffsetInBytes , UINT* , pStride )
	// This whole methods implementation is highly questionable. Not sure exactly how GetStreamSource works
	HRESULT result = D3DERR_INVALIDCALL;

	if (m_activeVertexBuffers.count(StreamNumber) == 1) {

		//IDirect3DVertexBuffer9* pCurrentActual = m_activeVertexBuffers[StreamNumber]->actual;

		//IDirect3DVertexBuffer9* pActualResultBuffer = NULL;
		//HRESULT result = actual->GetStreamSource(StreamNumber, &pCurrentActual, pOffsetInBytes, pStride);

		*ppStreamData = m_activeVertexBuffers[StreamNumber];
		if ((*ppStreamData))
			(*ppStreamData)->AddRef();

		result = D3D_OK;
	}
	return result;
}




/**
* Sets indices and calls proxy state block to capture states.
* @see D3D9ProxyStateBlock::SelectAndCaptureState()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetIndices , IDirect3DIndexBuffer9* , pIndexData )
	D3D9ProxyIndexBuffer* pWrappedNewIndexData = static_cast<D3D9ProxyIndexBuffer*>(pIndexData);

	// Update actual index buffer
	HRESULT result;
	if (pWrappedNewIndexData)
		result = actual->SetIndices(pWrappedNewIndexData->actual);
	else
		result = actual->SetIndices(NULL);

	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(pWrappedNewIndexData);
		}
		else {
			// Update stored proxy index buffer
			if (m_pActiveIndicies) {
				m_pActiveIndicies->Release();
			}

			m_pActiveIndicies = pWrappedNewIndexData;
			if (m_pActiveIndicies) {
				m_pActiveIndicies->AddRef();
			}
		}
	}

	return result;
}

/**
* Provides stored indices.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetIndices , IDirect3DIndexBuffer9** , ppIndexData )
	if (!m_pActiveIndicies)
		return D3DERR_INVALIDCALL;

	*ppIndexData = m_pActiveIndicies;
	m_pActiveIndicies->AddRef();

	return D3D_OK;
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreatePixelShader , CONST DWORD* , pFunction , IDirect3DPixelShader9** , ppShader )
	IDirect3DPixelShader9* pActualPShader = NULL;
	HRESULT creationResult = actual->CreatePixelShader(pFunction, &pActualPShader);

	if (SUCCEEDED(creationResult)) {
		*ppShader = new D3D9ProxyPixelShader(pActualPShader, this, m_pGameHandler->GetShaderModificationRepository());
	}

	return creationResult;
}

/**
* Sets pixel shader and calls proxy state block to capture states.
* @see D3D9ProxyStateBlock::SelectAndCaptureState()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetPixelShader , IDirect3DPixelShader9* , pShader )
	D3D9ProxyPixelShader* pWrappedPShaderData = static_cast<D3D9ProxyPixelShader*>(pShader);

	// Update actual pixel shader
	HRESULT result;
	if (pWrappedPShaderData)
		result = actual->SetPixelShader(pWrappedPShaderData->actual);
	else
	{
		result = actual->SetPixelShader(NULL);
		return result;
	}

	// Update stored proxy pixel shader
	if (SUCCEEDED(result)) {

		// If in a Begin-End StateBlock pair update the block state rather than the current proxy device state
		if (m_pCapturingStateTo) {
			m_pCapturingStateTo->SelectAndCaptureState(pWrappedPShaderData);
		}
		else {

			if (m_pActivePixelShader) {
				m_pActivePixelShader->Release();
			}

			m_pActivePixelShader = pWrappedPShaderData;
			if (m_pActivePixelShader) {
				m_pActivePixelShader->AddRef();
			}

			m_spManagedShaderRegisters->ActivePixelShaderChanged(m_pActivePixelShader);
		}
	}

	return result;
}

/**
* Provides stored pixel shader.
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetPixelShader , IDirect3DPixelShader9** , ppShader )
	if (!m_pActivePixelShader)
		return D3DERR_INVALIDCALL;

	*ppShader = m_pActivePixelShader;

	return D3D_OK;
}

/**
* Sets shader constants either at stored proxy state block or in managed shader register class.
* @see D3D9ProxyStateBlock
* @see ShaderRegisters
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , SetPixelShaderConstantF , UINT , StartRegister , CONST float* , pConstantData , UINT , Vector4fCount )
	HRESULT result = D3DERR_INVALIDCALL;

	if (m_pCapturingStateTo) {
		result = m_pCapturingStateTo->SelectAndCaptureStatePSConst(StartRegister, pConstantData, Vector4fCount);
	}
	else { 
		result = m_spManagedShaderRegisters->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	return result;
}

/**
* Provides constant registers from managed shader register class.
* @see ShaderRegisters
* @see ShaderRegisters::GetPixelShaderConstantF()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , GetPixelShaderConstantF , UINT , StartRegister , float* , pData , UINT , Vector4fCount )
	return m_spManagedShaderRegisters->GetPixelShaderConstantF(StartRegister, pData, Vector4fCount);
}




/**
* Applies all dirty registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide()
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawRectPatch , UINT , Handle , CONST float* , pNumSegs , CONST D3DRECTPATCH_INFO* , pRectPatchInfo )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo))) {
		if (switchDrawingSide())
			actual->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
	}

	return result;
}

/**
* Applies all dirty registers, draws both stereo sides if switchDrawingSide() agrees.
* @see switchDrawingSide() 
***/
METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , DrawTriPatch , UINT , Handle , CONST float* , pNumSegs , CONST D3DTRIPATCH_INFO* , pTriPatchInfo )
	m_spManagedShaderRegisters->ApplyAllDirty(m_currentRenderingSide);

	HRESULT result;
	if (SUCCEEDED(result = actual->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo))) {
		if (switchDrawingSide())
			actual->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
	}

	return result;
}




METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateQuery , D3DQUERYTYPE , Type , IDirect3DQuery9** , ppQuery )
	// this seems a weird response to me but it's what the actual device does.
	if (!ppQuery)
		return D3D_OK;

	IDirect3DQuery9* pActualQuery = NULL;
	HRESULT creationResult = actual->CreateQuery(Type, &pActualQuery);

	if (SUCCEEDED(creationResult)) {
		*ppQuery = new D3D9ProxyQuery(pActualQuery, this);
	}

	return creationResult;
}


/**
* Returns the actual embedded device pointer.
***/
IDirect3DDevice9* D3DProxyDevice::getActual()
{
	return actual;
}


/**
* Creates HUD according to viewport height.
***/
METHOD_IMPL( void , , D3DProxyDevice , SetupHUD )
	D3DXCreateFontA( this, 32, 0, FW_BOLD, 4, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &hudFont );
	D3DXCreateFontA( this, 24, 0, FW_BOLD, 4, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &popupFont );
	D3DXCreateFontA( this, 26, 0, FW_BOLD, 4, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New", &errorFont );
	D3DXCreateSprite(this, &hudMainMenu);
	D3DXCreateSprite(this, &hudTextBox);
}

/**
* Keyboard input handling, BRASSA called here.
***/
METHOD_IMPL( void , , D3DProxyDevice, HandleControls )
	controls.UpdateXInputs();

	// loop through hotkeys
	bool hotkeyPressed = false;
	for (int i = 0; i < 5; i++)
	{
		if ((controls.Key_Down(hudHotkeys[i])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (i==0)
			{
				HUD_3D_Depth_Modes newMode=(HUD_3D_Depth_Modes)(hud3DDepthMode+1);
				if (newMode>=HUD_3D_Depth_Modes::HUD_ENUM_RANGE)
					newMode=HUD_3D_Depth_Modes::HUD_DEFAULT;
				{
					oldHudMode = hud3DDepthMode;
					ChangeHUD3DDepthMode(newMode);

				}
			}
			else
			{
				if (hud3DDepthMode==(HUD_3D_Depth_Modes)(i-1))
				{
					if (controls.Key_Down(VK_RCONTROL))
					{
						oldHudMode = hud3DDepthMode;
						ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(i-1));
					}
					else
					{
						ChangeHUD3DDepthMode(oldHudMode);
					}

				}
				else
				{
					oldHudMode = hud3DDepthMode;
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(i-1));
				}
			}
			hotkeyPressed = true;
		}
		if ((controls.Key_Down(guiHotkeys[i])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (i==0)
			{
				GUI_3D_Depth_Modes newMode=(GUI_3D_Depth_Modes)(gui3DDepthMode+1);
				if (newMode>=GUI_3D_Depth_Modes::GUI_ENUM_RANGE)
					newMode=GUI_3D_Depth_Modes::GUI_DEFAULT;
				{
					oldGuiMode = gui3DDepthMode;
					ChangeGUI3DDepthMode(newMode);
				}
			}
			else
			{
				if (gui3DDepthMode==(GUI_3D_Depth_Modes)(i-1))
				{
					if (controls.Key_Down(VK_RCONTROL))
					{
						oldGuiMode = gui3DDepthMode;
						ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(i-1));
					}
					else
					{
						ChangeGUI3DDepthMode(oldGuiMode);
					}

				}
				else
				{
					oldGuiMode = gui3DDepthMode;
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(i-1));
				}
			}
			hotkeyPressed=true;
		}
	}
	// test VRBoost reset hotkey
	if (controls.Key_Down(toggleVRBoostHotkey) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (hmVRboost!=NULL)
		{
			m_pVRboost_ReleaseAllMemoryRules();
			m_bVRBoostToggle = !m_bVRBoostToggle;
			if (tracker->getStatus() > MTS_OK)
				tracker->resetOrientationAndPosition();

			// set the indicator to be drawn
			m_fVRBoostIndicator = 1.0f;

			menuVelocity.x += 4.0f;
		}
	}

	//Show FPS Counter / Frame Time counter
	if ((controls.Key_Down(VK_F9) || ((controls.Key_Down(VK_LSHIFT) || controls.Key_Down(VK_LCONTROL)) && controls.Key_Down(0x46))) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		show_fps = (FPS_TYPE)((show_fps+1) % 3);
		menuVelocity.x+=2.0f;
	}

	//Show HMD Stats Counter
	if ((controls.Key_Down(VK_F10) || ((controls.Key_Down(VK_LSHIFT) || controls.Key_Down(VK_LCONTROL)) && controls.Key_Down(0x53))) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (activePopup.popupType == VPT_STATS)
		{
			DismissPopup(VPT_STATS);
		}
		else
		{
			VireioPopup popup(VPT_STATS);
			ShowPopup(popup);
		}
		menuVelocity.x+=2.0f;
	}

	//Rset HMD Orientation+Position
	if ((controls.Key_Down(VK_F12) || ((controls.Key_Down(VK_LSHIFT) || controls.Key_Down(VK_LCONTROL)) && controls.Key_Down(0x52))) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (calibrate_tracker)
		{
			calibrate_tracker = false;
			//Replace popup
			VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 3000);
			strcpy_s(popup.line3, "HMD Orientation and Position Calibrated");
			strcpy_s(popup.line3, "Please repeat if required...");
			ShowPopup(popup);
		}
		else
		{
			VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 1000);
			strcpy_s(popup.line3, "HMD Orientation and Position Reset");
			ShowPopup(popup);
		}
		tracker->resetOrientationAndPosition();
		menuVelocity.x+=2.0f;
	}

	//Toggle positional tracking
	if ((controls.Key_Down(VK_F11) || ((controls.Key_Down(VK_LSHIFT) || controls.Key_Down(VK_LCONTROL)) && controls.Key_Down(0x50))) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		m_bPosTrackingToggle = !m_bPosTrackingToggle;

		VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 1000);
		if (m_bPosTrackingToggle)
			strcpy_s(popup.line3, "HMD Positional Tracking Enabled");
		else
			strcpy_s(popup.line3, "HMD Positional Tracking Disabled");
		ShowPopup(popup);

		if (!m_bPosTrackingToggle)
			m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);

		menuVelocity.x += 4.0f;
	}

	// toggle VR Mouse
	if (controls.Key_Down(VK_LCONTROL) && controls.Key_Down(VK_NUMPAD0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (m_bShowVRMouse)
			m_bShowVRMouse = false;
		else
			m_bShowVRMouse = true;

		VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 1000);
		if (m_bShowVRMouse)
			strcpy_s(popup.line3, "VR Mouse Enabled");
		else
			strcpy_s(popup.line3, "VR Mouse Disabled");
		ShowPopup(popup);

		menuVelocity.x += 4.0f;		
	}

	// floaty menus
	if (controls.Key_Down(VK_LCONTROL) && controls.Key_Down(VK_NUMPAD1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (m_bfloatingMenu)
			m_bfloatingMenu = false;
		else
		{
			m_bfloatingMenu = true;
			if (tracker->getStatus() >= MTS_OK)
			{
				m_fFloatingPitch = tracker->primaryPitch;
				m_fFloatingYaw = tracker->primaryYaw;			
			}
		}

		VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 1000);
		if (m_bfloatingMenu)
			strcpy_s(popup.line3, "Floating Menus Enabled");
		else
			strcpy_s(popup.line3, "Floating Menus Disabled");
		ShowPopup(popup);

		menuVelocity.x += 4.0f;		
	}

	//Floaty Screen
	if ((controls.Key_Down(VK_MBUTTON) || (controls.Key_Down(VK_LCONTROL) && controls.Key_Down(VK_NUMPAD2))) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (m_bfloatingScreen)
		{
			m_bfloatingScreen = false;
			m_bSurpressHeadtracking = false;
			//TODO Change this back to initial
			this->stereoView->HeadYOffset = 0;
			this->stereoView->XOffset = 0;
			this->stereoView->PostReset();	
		}
		else
		{
			m_bfloatingScreen = true;
			m_bSurpressHeadtracking = true;
			if (tracker->getStatus() >= MTS_OK)
			{
				m_fFloatingScreenPitch = tracker->primaryPitch;
				m_fFloatingScreenYaw = tracker->primaryYaw;			
			}
		}

		VireioPopup popup(VPT_NOTIFICATION, VPS_TOAST, 1000);
		if (m_bfloatingScreen)
			strcpy_s(popup.line3, "Disconnected Screen View Enabled");
		else
			strcpy_s(popup.line3, "Disconnected Screen View Disabled");
		ShowPopup(popup);

		menuVelocity.x += 4.0f;		
	}
	if(m_bfloatingScreen)
	{
		float screenFloatMultiplierY = 0.75;
		float screenFloatMultiplierX = 0.5;
		if (tracker->getStatus() >= MTS_OK)
		{
			this->stereoView->HeadYOffset = (m_fFloatingScreenPitch - tracker->primaryPitch) * screenFloatMultiplierY;
			this->stereoView->XOffset = (m_fFloatingScreenYaw - tracker->primaryYaw) * screenFloatMultiplierX;
			this->stereoView->PostReset();
		}
		//m_ViewportIfSquished.X = (int)(vOut.x+centerX-(((m_fFloatingYaw - tracker->primaryYaw) * floatMultiplier) * (180 / PI)));
		//m_ViewportIfSquished.Y = (int)(vOut.y+centerY-(((m_fFloatingPitch - tracker->primaryPitch) * floatMultiplier) * (180 / PI)));
	}

	// avoid double input by using the menu velocity
	if (hotkeyPressed)
		menuVelocity.x+=2.0f;

	// open BRASSA - <CTRL>+<T>
	if(controls.Key_Down(0x51) && controls.Key_Down(VK_LCONTROL) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (BRASSA_mode == BRASSA_Modes::INACTIVE)
		{
			borderTopHeight = 0.0f;
			BRASSA_mode = BRASSA_Modes::MAINMENU;
		}
		else
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}

		menuVelocity.x+=2.0f;
	}

	// open BRASSA - <LSHIFT>+<*>
	if(controls.Key_Down(VK_MULTIPLY) && controls.Key_Down(VK_LSHIFT) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))		
	{
		if (BRASSA_mode == BRASSA_Modes::INACTIVE)
		{
			borderTopHeight = 0.0f;
			BRASSA_mode = BRASSA_Modes::MAINMENU;
		}
		else
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}

		menuVelocity.x+=2.0f;
	}

	//Mouse Wheel Scroll
	if(controls.Key_Down(VK_LCONTROL))
	{
		int _wheel = dinput.GetWheel();
		if(controls.Key_Down(VK_TAB))
		{
			if(_wheel < 0)
			{
				if(this->stereoView->YOffset > -0.1f)
				{
					this->stereoView->YOffset -= 0.005f;
					this->stereoView->PostReset();				
				}
			}
			else if(_wheel > 0)
			{
				if(this->stereoView->YOffset < 0.1f)
				{
					this->stereoView->YOffset += 0.005f;
					this->stereoView->PostReset();										
				}
			}
		}
		else if(controls.Key_Down(VK_LSHIFT))
 		{			
			if(_wheel < 0)
			{
				if(this->stereoView->IPDOffset > -0.1f)
				{
					this->stereoView->IPDOffset -= 0.005f;
					this->stereoView->PostReset();				
				}
			}
			else if(_wheel > 0)
			{
				if(this->stereoView->IPDOffset < 0.1f)
				{
					this->stereoView->IPDOffset += 0.005f;
					this->stereoView->PostReset();										
				}
 			}
 		}
		else
		{
			if(_wheel < 0)
			{
				if(this->stereoView->DistortionScale > config.minDistortionScale)
				{
					this->stereoView->DistortionScale -= 0.05f;
					this->stereoView->PostReset();				
				}
			}
			else if(_wheel > 0)
			{
				if(this->stereoView->DistortionScale < m_maxDistortionScale)
					{
						this->stereoView->DistortionScale += 0.05f;
						this->stereoView->PostReset();										
					}
			}
		}
	}
	
	//Change Distortion Scale CTRL + + / -
	if(controls.Key_Down(VK_LCONTROL) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if(config.DistortionScale > 0.00f)
		{
			m_maxDistortionScale = config.DistortionScale;
		}
		if(controls.Key_Down(VK_ADD))
		{
			if(this->stereoView->DistortionScale < m_maxDistortionScale)
			{
				this->stereoView->DistortionScale = m_maxDistortionScale;
				this->stereoView->PostReset();										
			}
		}
		else if(controls.Key_Down(VK_SUBTRACT))
		{
			if(this->stereoView->DistortionScale != config.minDistortionScale)
			{
				this->stereoView->DistortionScale = config.minDistortionScale;
				this->stereoView->PostReset();							
			}
		}		
	}	


	// screenshot - <RCONTROL>+<*>
	if(controls.Key_Down(VK_MULTIPLY) && controls.Key_Down(VK_RCONTROL) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))		
	{
		// render 3 frames to get screenshots without BRASSA
		screenshot = 3;
		menuVelocity.x+=8.0f;
	}
}

/**
* Updates selected motion tracker orientation.
***/
METHOD_IMPL( void , , D3DProxyDevice , HandleTracking )
	if(!tracker)
	{
		InitTracker();
	}

	if(!tracker || tracker->getStatus() < MTS_OK)
	{
		if (tracker)
		{
			//Populate with popups about the following
			switch (tracker->getStatus())
			{
				case MTS_NOTINIT:
					{
						VireioPopup popup(VPT_NO_HMD_DETECTED, VPS_ERROR, 10000);
						strcpy_s(popup.line3, "HMD NOT INITIALISED");
						ShowPopup(popup);
					}
					break;
				case MTS_INITIALISING:
					{
						VireioPopup popup(VPT_NO_HMD_DETECTED);
						strcpy_s(popup.line3, "HMD INITIALISING");
						ShowPopup(popup);
					}
					break;
				case MTS_NOHMDDETECTED:
					{
						VireioPopup popup(VPT_NO_HMD_DETECTED, VPS_ERROR, 10000);
						strcpy_s(popup.line3, (std::string(10, ' ') + "HMD NOT DETECTED").c_str());
						ShowPopup(popup);
					}
					break;
				case MTS_INITFAIL:
					{
						VireioPopup popup(VPT_NO_HMD_DETECTED, VPS_ERROR, 10000);
						strcpy_s(popup.line3, "HMD INITIALISATION FAILED");
						ShowPopup(popup);
					}
					break;
				case MTS_DRIVERFAIL:
					{
						VireioPopup popup(VPT_NO_HMD_DETECTED, VPS_ERROR, 10000);
						strcpy_s(popup.line3, "TRACKER DRIVER FAILED TO INITIALISE");
						ShowPopup(popup);
					}
					break;
				default:
					break;
			}
		}

		tracker->currentRoll = 0;
		return;
	}

	float xPos=0, yPos=0, zPos=0;
	float yaw=0, pitch=0, roll=0;

	if(tracker->getStatus() >= MTS_OK)
	{
		tracker->updateOrientationAndPosition();

		switch (tracker->getStatus())
		{
		case MTS_OK:
			{
				//Dismiss popups related to issues
				DismissPopup(VPT_POSITION_TRACKING_LOST);
				DismissPopup(VPT_NO_HMD_DETECTED);
				DismissPopup(VPT_NO_ORIENTATION);
				if (calibrate_tracker)
				{
					VireioPopup popup(VPT_CALIBRATE_TRACKER, VPS_INFO, 30000);
					strcpy_s(popup.line2, "Please Calibrate HMD/Tracker:");
					strcpy_s(popup.line3, "     -  Sit comfortably with your head facing forwards");
					strcpy_s(popup.line4, "     -  Press any of the following:");
					strcpy_s(popup.line5, "        F12 / CTRL+R / L-SHIFT + R");
					ShowPopup(popup);
				}
			}
			break;
		case MTS_NOORIENTATION:
			{
				VireioPopup popup(VPT_NO_ORIENTATION, VPS_ERROR);
				strcpy_s(popup.line3, "HMD ORIENTATION NOT BEING REPORTED");
				ShowPopup(popup);
			}
			break;
		case MTS_CAMERAMALFUNCTION:
			{
				VireioPopup popup(VPT_NO_HMD_DETECTED, VPS_ERROR);
				strcpy_s(popup.line3, "CAMERA MALFUNCTION - PLEASE WAIT WHILST CAMERA INITIALISES");
				ShowPopup(popup);
			}
			break;
		case MTS_LOSTPOSITIONAL:
			{
				//Show popup regarding lost positional tracking
				VireioPopup popup(VPT_POSITION_TRACKING_LOST);
				strcpy_s(popup.line5, "HMD POSITIONAL TRACKING LOST");
				ShowPopup(popup);
			}
			break;
		}

		// update view adjustment class
		if (tracker->getStatus() >= MTS_OK)
		{
			if (m_spShaderViewAdjustment->RollEnabled()) {
				m_spShaderViewAdjustment->UpdateRoll(tracker->currentRoll);
			}

			m_spShaderViewAdjustment->UpdatePitchYaw(tracker->primaryPitch, tracker->primaryYaw);

			if (m_bPosTrackingToggle && tracker->getStatus() != MTS_LOSTPOSITIONAL)
			{
				m_spShaderViewAdjustment->UpdatePosition(tracker->primaryYaw, tracker->primaryPitch, tracker->primaryRoll,
					(VRBoostValue[VRboostAxis::CameraTranslateX] / 20.0f) + tracker->primaryX, 
					(VRBoostValue[VRboostAxis::CameraTranslateY] / 20.0f) + tracker->primaryY,
					(VRBoostValue[VRboostAxis::CameraTranslateZ] / 20.0f) + tracker->primaryZ,
					config.position_multiplier);
			}
		}
	}
		
	m_spShaderViewAdjustment->ComputeViewTransforms();

	m_isFirstBeginSceneOfFrame = false;

	// update vrboost, if present, tracker available and shader count higher than the minimum
	if ((!m_bSurpressHeadtracking) && (!m_bForceMouseEmulation) && (hmVRboost) && (m_VRboostRulesPresent) 
		&& (tracker->getStatus() >= MTS_OK) && (m_bVRBoostToggle)
		&& (m_VertexShaderCountLastFrame>(UINT)config.VRboostMinShaderCount)
		&& (m_VertexShaderCountLastFrame<(UINT)config.VRboostMaxShaderCount) )
	{
		VRBoostStatus.VRBoost_Active = true;
		// development bool
		bool createNSave = false;

		// apply VRboost memory rules if present
		VRBoostValue[VRboostAxis::TrackerYaw] = tracker->primaryYaw;
		VRBoostValue[VRboostAxis::TrackerPitch] = tracker->primaryPitch;
		VRBoostValue[VRboostAxis::TrackerRoll] = tracker->primaryRoll;
		if (m_pVRboost_ApplyMemoryRules(MAX_VRBOOST_VALUES, (float**)&VRBoostValue) != S_OK)
		{
			VRBoostStatus.VRBoost_ApplyRules = false;
			if (!createNSave)
			{
				// load VRboost rules
				if (config.VRboostPath != "")
				{
					//OutputDebugStringA(std::string("config.VRboostPath: " + config.VRboostPath).c_str());
					if (m_pVRboost_LoadMemoryRules(config.exeName.toStdString() , config.VRboostPath.toStdString() ) != S_OK)
						VRBoostStatus.VRBoost_LoadRules = false;
					else
						VRBoostStatus.VRBoost_LoadRules = true;
				}
			}
		}
		else
		{
			VRBoostStatus.VRBoost_ApplyRules = true;
		}
	}
	else
	{
		VRBoostStatus.VRBoost_Active = false;
	}

	if (!m_bForceMouseEmulation && VRBoostStatus.VRBoost_Active)
	{
		if (!VRBoostStatus.VRBoost_LoadRules)
		{
			VireioPopup popup(VPT_VRBOOST_FAILURE, VPS_ERROR);
			strcpy_s(popup.line3, "VRBoost LoadRules Failed");
			strcpy_s(popup.line4, "To Enable head tracking, turn on Force Mouse Emulation");
			strcpy_s(popup.line5, "in BRASSA Settings");
			ShowPopup(popup);
		}
		else if (!VRBoostStatus.VRBoost_ApplyRules)
		{
			VireioPopup popup(VPT_VRBOOST_FAILURE, VPS_ERROR);
			strcpy_s(popup.line2, "VRBoost rules loaded but could not be applied");
			strcpy_s(popup.line3, "Mouse Emulation is not Enabled,");
			strcpy_s(popup.line4, "To Enable head tracking, turn on Force Mouse Emulation");
			strcpy_s(popup.line5, "in BRASSA Settings");
			ShowPopup(popup);
		}
		else
		{
			//All is well
			DismissPopup(VPT_VRBOOST_FAILURE);
		}
	}
}

/**
* Handles all updates if Present() is called in an extern swap chain.
***/
METHOD_IMPL( void , , D3DProxyDevice , HandleUpdateExtern )
	#ifdef SHOW_CALLS
		OutputDebugStringA("called HandleUpdateExtern");
	#endif
	m_isFirstBeginSceneOfFrame = true;

	BRASSA_UpdateBorder();
}

/**
* Creates or restores class setup.
* Subclasses which override this method must call through to super method.
* Do not directly call this method in subclasses.
* This method should be used to re/create any resources that are held by the device proxy and deleted by Reset.
* 
* The only resources used like this are going to be extra resources that are used by the proxy and are not
* part of the actual calling application. 
* 
* Examples in D3DProxyDevice: The Font used in the BRASSA overlay and the stereo buffer.
* 
* Example of something you wouldn't create here:
* Render targets in the m_activeRenderTargets collection. They need to be released to successfully Reset
* the device, but they just wrap IDirect3DSurface9 objects from the underlying application and will be
* re/created by the underlying application.
* 
* This method will be called when the proxy device is initialised with Init (happens before device is
* passed back to actual application by CreateDevice) and after a successful device Reset.
***/
METHOD_IMPL( void , , D3DProxyDevice , OnCreateOrRestore )	
	m_currentRenderingSide = vireio::Left;
	m_pCurrentMatViewTransform = &m_spShaderViewAdjustment->LeftAdjustmentMatrix();
	m_pCurrentView = &m_leftView;
	m_pCurrentProjection = &m_leftProjection;

	// Wrap the swap chain
	IDirect3DSwapChain9* pActualPrimarySwapChain;
	if (FAILED(actual->GetSwapChain(0, &pActualPrimarySwapChain))) {
		OutputDebugStringA("Failed to fetch swapchain.\n");
		exit(1); 
	}

	assert (m_activeSwapChains.size() == 0);
	m_activeSwapChains.push_back(new D3D9ProxySwapChain(pActualPrimarySwapChain, this, false));
	assert (m_activeSwapChains.size() == 1);

	// Set the primary rendertarget to the first stereo backbuffer
	IDirect3DSurface9* pWrappedBackBuffer;
	m_activeSwapChains[0]->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pWrappedBackBuffer);
	SetRenderTarget(0, pWrappedBackBuffer);
	pWrappedBackBuffer->Release();
	pWrappedBackBuffer = NULL;


	actual->GetViewport(&m_LastViewportSet);

	// If there is an initial depth stencil
	IDirect3DSurface9* pDepthStencil;
	if (SUCCEEDED(actual->GetDepthStencilSurface(&pDepthStencil))) { 

		D3DSURFACE_DESC stencilDesc;
		pDepthStencil->GetDesc(&stencilDesc);
		pDepthStencil->Release();

		IDirect3DSurface9* pTemp = NULL;
		CreateDepthStencilSurface(stencilDesc.Width, stencilDesc.Height, stencilDesc.Format, stencilDesc.MultiSampleType, stencilDesc.MultiSampleQuality, false, &pTemp, NULL);
		SetDepthStencilSurface(pTemp);
		pTemp->Release();	
	}

	SetupHUD();

	stereoView->Init(getActual());

	m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);
	m_spShaderViewAdjustment->ComputeViewTransforms();

	// set BRASSA main values
	viewportWidth = stereoView->viewport.Width;
	viewportHeight = stereoView->viewport.Height;

	menuTop = viewportHeight*0.32f;
	menuEntryHeight = viewportHeight*0.037f;

	fScaleX = ((float)viewportWidth / (float)BRASSA_PIXEL_WIDTH);
	fScaleY = ((float)viewportHeight / (float)BRASSA_PIXEL_HEIGHT);

	menuHelperRect.left = 0;
	menuHelperRect.right = BRASSA_PIXEL_WIDTH;
	menuHelperRect.top = 0;
	menuHelperRect.bottom = BRASSA_PIXEL_HEIGHT;
}

/**
* Switches rendering to which ever side is specified by side.
* Use to specify the side that you want to draw to.
* Overriding classes should call the base implementation first and then makes any extra needed changes
* based on the result of the base implementation (if the base class doesn't change side then derived shouldn't 
* change either)
* 
* @return True if change succeeded, false if it fails. The switch will fail if you attempt to setDrawingSide(Right)
* when the current primary active render target (target 0  in m_activeRenderTargets) is not stereo.
* Attempting to switch to a side when that side is already the active side will return true without making any changes.
***/
METHOD_IMPL( bool , , D3DProxyDevice , setDrawingSide , vireio::RenderPosition , side )
	// Already on the correct eye
	if (side == m_currentRenderingSide) {
		return true;
	}

	// should never try and render for the right eye if there is no render target for the main render targets right side
	if (!m_activeRenderTargets[0]->right && (side == vireio::Right)) {
		return false;
	}

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	m_currentRenderingSide = side;

	// switch render targets to new side
	bool renderTargetChanged = false;
	HRESULT result;
	D3D9ProxySurface* pCurrentRT;
	for(std::vector<D3D9ProxySurface*>::size_type i = 0; i != m_activeRenderTargets.size(); i++) 
	{
		if ((pCurrentRT = m_activeRenderTargets[i]) != NULL) {

			if (side == vireio::Left) 
				result = actual->SetRenderTarget(i, pCurrentRT->actual); 
			else 
				result = actual->SetRenderTarget(i, pCurrentRT->right);

			if (result != D3D_OK) {
				OutputDebugStringA("Error trying to set one of the Render Targets while switching between active eyes for drawing.\n");
			}
			else {
				renderTargetChanged = true;
			}
		}
	}

	// if a non-fullsurface viewport is active and a rendertarget changed we need to reapply the viewport
	if (renderTargetChanged && !m_bActiveViewportIsDefault) {
		actual->SetViewport(&m_LastViewportSet);
	}

	if (m_bViewportIsSquished)
		SetGUIViewport();

	// switch depth stencil to new side
	if (m_pActiveStereoDepthStencil != NULL) { 
		if (side == vireio::Left) 
			result = actual->SetDepthStencilSurface(m_pActiveStereoDepthStencil->actual); 
		else 
			result = actual->SetDepthStencilSurface(m_pActiveStereoDepthStencil->right);
	}

	// switch textures to new side
	IDirect3DBaseTexture9* pActualLeftTexture = NULL;
	IDirect3DBaseTexture9* pActualRightTexture = NULL;

	for(auto it = m_activeTextureStages.begin(); it != m_activeTextureStages.end(); ++it )
	{
		if (it->second) {
			pActualLeftTexture = NULL;
			pActualRightTexture = NULL;
			vireio::UnWrapTexture(it->second, &pActualLeftTexture, &pActualRightTexture);

			// if stereo texture
			if (pActualRightTexture != NULL) { 
				if (side == vireio::Left) 
					result = actual->SetTexture(it->first, pActualLeftTexture); 
				else 
					result = actual->SetTexture(it->first, pActualRightTexture);
			}
			// else the texture is mono and doesn't need changing. It will always be set initially and then won't need changing

			if (result != D3D_OK)
				OutputDebugStringA("Error trying to set one of the textures while switching between active eyes for drawing.\n");
		}
	}

	// update view transform for new side 
	if (m_bViewTransformSet) {

		if (side == vireio::Left) {
			m_pCurrentView = &m_leftView;
		}
		else {
			m_pCurrentView = &m_rightView;
		}

		actual->SetTransform(D3DTS_VIEW, m_pCurrentView);
	}

	// update projection transform for new side 
	if (m_bProjectionTransformSet) {

		if (side == vireio::Left) {
			m_pCurrentProjection = &m_leftProjection;
		}
		else {
			m_pCurrentProjection = &m_rightProjection;
		}

		actual->SetTransform(D3DTS_PROJECTION, m_pCurrentProjection);
	}

	// Updated computed view translation (used by several derived proxies - see: ComputeViewTranslation)
	if (side == vireio::Left) {
		m_pCurrentMatViewTransform = &m_spShaderViewAdjustment->LeftAdjustmentMatrix();
	}
	else {
		m_pCurrentMatViewTransform = &m_spShaderViewAdjustment->RightAdjustmentMatrix();
	}

	// Apply active stereo shader constants
	m_spManagedShaderRegisters->ApplyAllStereoConstants(side);

	return true;
}

/**
* Try and toggle to other drawing side. 
* @return False if changes fails due to the current render target being mono.
***/
METHOD_IMPL( bool , , D3DProxyDevice , switchDrawingSide )
	bool switched = false;

	if (m_currentRenderingSide == vireio::Left) {
		switched = setDrawingSide(vireio::Right);
	}
	else if (m_currentRenderingSide == vireio::Right) {
		switched = setDrawingSide(vireio::Left);
	}
	else {
		DebugBreak();
	}

	return switched;
}

/**
* Adds a default shader rule to the game configuration.
* @return True if rule was added, false if rule already present.
***/
METHOD_IMPL( bool , , D3DProxyDevice , addRule , std::string , constantName , bool , allowPartialNameMatch , UINT , startRegIndex , D3DXPARAMETER_CLASS , constantType , UINT , operationToApply , bool , transpose )
	return m_pGameHandler->AddRule(m_spShaderViewAdjustment, constantName, allowPartialNameMatch, startRegIndex, constantType, operationToApply, transpose);
}

/**
* Adds a default shader rule to the game configuration.
* @return True if rule was added, false if rule already present.
***/
METHOD_IMPL( bool , , D3DProxyDevice , modifyRule , std::string , constantName , UINT , operationToApply , bool , transpose )
	return m_pGameHandler->ModifyRule(m_spShaderViewAdjustment, constantName, operationToApply, transpose);
}

/**
* Delete rule.
* @return True if rule was deleted, false if rule not present.
***/
METHOD_IMPL( bool , , D3DProxyDevice , deleteRule , std::string , constantName )
	return m_pGameHandler->DeleteRule(m_spShaderViewAdjustment, constantName);
}

/**
* Saves current game shader rules (and game configuration).
***/
METHOD_IMPL( void , , D3DProxyDevice , saveShaderRules ) 
	m_pGameHandler->Save(config, m_spShaderViewAdjustment);

	config.cGameProfile::save();
}

/**
* Simple helper to clear a rectangle using the specified color.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
***/
METHOD_IMPL( void , , D3DProxyDevice , ClearRect , vireio::RenderPosition , renderPosition , D3DRECT , rect , D3DCOLOR , color )
	setDrawingSide(renderPosition);
	actual->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

/**
* Simple helper to clear an empty rectangle or border using the specified color.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param bw The border width.
***/
METHOD_IMPL( void , , D3DProxyDevice , ClearEmptyRect , vireio::RenderPosition , renderPosition , D3DRECT , rect , D3DCOLOR , color, int , bw )
	// helper rectangle
	D3DRECT rect0 = D3DRECT(rect);

	setDrawingSide(renderPosition);

	rect0.y2 = rect.y1 + bw;
	actual->Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.y1 = rect.y2 - bw;
	rect0.y2 = rect.y2;
	actual->Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.y1 = rect.y1;
	rect0.x2 = rect.x1 + bw;
	actual->Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.x1 = rect.x2 - bw;
	rect0.x2 = rect.x2;
	actual->Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);
}

/**
* Draws a simple selection control.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param selectionIndex The index of the currently chosen selection.
* @param selectionRange The range of the selection.
***/
METHOD_IMPL( void , , D3DProxyDevice , DrawSelection , vireio::RenderPosition , renderPosition , D3DRECT , rect , D3DCOLOR , color , int , selectionIndex , int , selectionRange )	
	// get width of each selection
	float selectionWidth = (rect.x2-rect.x1) / (float)selectionRange;

	// get secondary color
	D3DXCOLOR color2 = D3DXCOLOR(color);
	FLOAT red = color2.r;
	color2.r = color2.g * 0.7f;
	color2.g = red;

	for (int i = 0; i < selectionRange; i++)
	{
		rect.x2 = rect.x1+(int)selectionWidth;
		if (i==selectionIndex)
			ClearRect(renderPosition, rect, color);
		else
			ClearRect(renderPosition, rect, color2);
		rect.x1+=(int)selectionWidth;
	}
}

/**
* Draws a simple selection control.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param selectionIndex The index of the currently chosen selection.
* @param selectionRange The range of the selection.
***/
METHOD_IMPL( void , , D3DProxyDevice , DrawScrollbar , vireio::RenderPosition , renderPosition , D3DRECT , rect , D3DCOLOR , color , float , scroll , int , scrollbarSize )	
	if (scroll<0.0f) scroll=0.0f;
	if (scroll>1.0f) scroll=1.0f;

	// get width of each selection
	int scrollHeight = rect.y2-rect.y1-scrollbarSize;
	scrollHeight = (int)(scrollHeight*scroll);

	// get secondary color
	D3DXCOLOR color2 = D3DXCOLOR(color);
	FLOAT red = color2.r;
	color2.r = color2.g * 0.7f;
	color2.g = red;

	ClearRect(renderPosition, rect, color2);
	rect.y1 += scrollHeight;
	rect.y2 = rect.y1+scrollbarSize;
	ClearRect(renderPosition, rect, color);
}

/**
* Draws a text with a dark shadow.
* @see DrawText()
***/
METHOD_IMPL( void , , D3DProxyDevice , DrawTextShadowed , ID3DXFont* , font , LPD3DXSPRITE , sprite , LPCSTR , lpchText , int , cchText , LPRECT , lprc , UINT , format, D3DCOLOR , color )
	lprc->left+=2; lprc->right+=2; lprc->top+=2; lprc->bottom+=2;
	font->DrawTextA(sprite, lpchText, -1, lprc, format, D3DCOLOR_ARGB(255, 64, 64, 64));
	lprc->left-=2; lprc->right-=2; lprc->top-=2; lprc->bottom-=2;
	font->DrawTextA(sprite, lpchText, -1, lprc, format, color);
}

/**
* Changes the HUD scale mode - also changes new scale in view adjustment class.
***/
METHOD_IMPL( void , , D3DProxyDevice , ChangeHUD3DDepthMode , HUD_3D_Depth_Modes , newMode )
	if (newMode >= HUD_3D_Depth_Modes::HUD_ENUM_RANGE)
		return;

	hud3DDepthMode = newMode;

	m_spShaderViewAdjustment->ChangeHUDDistance(hudDistancePresets[(int)newMode]);
	m_spShaderViewAdjustment->ChangeHUD3DDepth(hud3DDepthPresets[(int)newMode]);
}

/**
* Changes the GUI scale mode - also changes new scale in view adjustment class.
***/
METHOD_IMPL( void , , D3DProxyDevice , ChangeGUI3DDepthMode , GUI_3D_Depth_Modes , newMode )
	if (newMode >= GUI_3D_Depth_Modes::GUI_ENUM_RANGE)
		return;

	gui3DDepthMode = newMode;

	m_spShaderViewAdjustment->ChangeGUISquash(guiSquishPresets[(int)newMode]);
	m_spShaderViewAdjustment->ChangeGUI3DDepth(gui3DDepthPresets[(int)newMode]);


	if (newMode == GUI_3D_Depth_Modes::GUI_FULL)
		m_spShaderViewAdjustment->SetBulletLabyrinthMode(true);
	else
		m_spShaderViewAdjustment->SetBulletLabyrinthMode(false);
}

/**
* BRASSA menu helper to setup new frame.
* @param entryID [in, out] Provides the identifier by count of the menu entry.
* @param menuEntryCount [in] The number of menu entries.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_NewFrame , UINT& , entryID , UINT , menuEntryCount )
	// set menu entry attraction
	menuAttraction.y = ((borderTopHeight-menuTop)/menuEntryHeight);
	menuAttraction.y -= (float)((UINT)menuAttraction.y);
	menuAttraction.y -= 0.5f;
	menuAttraction.y *= 2.0f;
	if ((menuVelocity.y>0.0f) && (menuAttraction.y<0.0f)) menuAttraction.y = 0.0f;
	if ((menuVelocity.y<0.0f) && (menuAttraction.y>0.0f)) menuAttraction.y = 0.0f;

	// handle border height
	if (borderTopHeight<menuTop)
	{
		borderTopHeight = menuTop;
		menuVelocity.y=0.0f;
		menuAttraction.y=0.0f;

	}
	if (borderTopHeight>(menuTop+(menuEntryHeight*(float)(menuEntryCount-1))))
	{
		borderTopHeight = menuTop+menuEntryHeight*(float)(menuEntryCount-1);
		menuVelocity.y=0.0f;
		menuAttraction.y=0.0f;
	}

	// get menu entry id
	float entry = (borderTopHeight-menuTop+(menuEntryHeight/3.0f))/menuEntryHeight;
	entryID = (UINT)entry;
	if (entryID >= menuEntryCount)
		OutputDebugStringA("Error in BRASSA menu programming !");
}

/**
* BRASSA menu main method.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA )
	switch (BRASSA_mode)
	{
	case D3DProxyDevice::MAINMENU:
		BRASSA_MainMenu();
		break;
	case D3DProxyDevice::WORLD_SCALE_CALIBRATION:
		BRASSA_WorldScale();
		break;
	case D3DProxyDevice::CONVERGENCE_ADJUSTMENT:
		BRASSA_Convergence();
		break;
	case D3DProxyDevice::HUD_CALIBRATION:
		BRASSA_HUD();
		break;
	case D3DProxyDevice::GUI_CALIBRATION:
		BRASSA_GUI();
		break;
	case D3DProxyDevice::OVERALL_SETTINGS:
		BRASSA_Settings();
		break;
	case D3DProxyDevice::VRBOOST_VALUES:
		BRASSA_VRBoostValues();
		break;
	case D3DProxyDevice::POS_TRACKING_SETTINGS:
		BRASSA_PosTracking();
		break;
	case D3DProxyDevice::BRASSA_SHADER_ANALYZER_SUBMENU:
		BRASSA_ShaderSubMenu();
		break;
	case D3DProxyDevice::CHANGE_RULES_SCREEN:
		BRASSA_ChangeRules();
		break;
	case D3DProxyDevice::PICK_RULES_SCREEN:
		BRASSA_PickRules();
		break;
	case D3DProxyDevice::SHOW_SHADERS_SCREEN:
		BRASSA_ShowActiveShaders();
		break;
	}
}

/**
* BRASSA Main Menu method.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_MainMenu )
	UINT menuEntryCount = 11;
	if (config.game_type > 10000) menuEntryCount++;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;
	if (config.game_type <= 10000)
		entryID++;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
		BRASSA_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// brassa shader analyzer sub menu
		if (entryID == 0)
		{
			BRASSA_mode = BRASSA_Modes::BRASSA_SHADER_ANALYZER_SUBMENU;
			menuVelocity.x+=2.0f;
		}
		// world scale
		if (entryID == 1)
		{
			BRASSA_mode = BRASSA_Modes::WORLD_SCALE_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// hud calibration
		if (entryID == 2)
		{
			BRASSA_mode = BRASSA_Modes::CONVERGENCE_ADJUSTMENT;
			menuVelocity.x+=2.0f;
		}
		// hud calibration
		if (entryID == 3)
		{
			BRASSA_mode = BRASSA_Modes::HUD_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// gui calibration
		if (entryID == 4)
		{
			BRASSA_mode = BRASSA_Modes::GUI_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// overall settings
		if (entryID == 7)
		{
			BRASSA_mode = BRASSA_Modes::OVERALL_SETTINGS;
			menuVelocity.x+=2.0f;
		}	
		// vrboost settings
		if (entryID == 8)
		{
			BRASSA_mode = BRASSA_Modes::VRBOOST_VALUES;
			menuVelocity.x+=2.0f;
		}
		// position tracking settings
		if (entryID == 9)
		{
			BRASSA_mode = BRASSA_Modes::POS_TRACKING_SETTINGS;
			menuVelocity.x+=2.0f;
		}
		// restore configuration
		if (entryID == 10)
		{
			// first, backup all strings
			QString game_exe      = config.exeName;
			QString shaderRulePath= config.shaderRulePath;
			QString VRboostPath   = config.VRboostPath;

			config = m_configBackup;

			config.exeName          = game_exe;
			config.shaderRulePath   = shaderRulePath;
			config.VRboostPath      = VRboostPath;

			BRASSA_UpdateDeviceSettings();
			BRASSA_UpdateConfigSettings();
			menuVelocity.x+=10.0f;
		}	
		// back to game
		if (entryID == 11)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}
	}

	if (controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192))
	{
		// change hud scale 
		if ((entryID == 5) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
			menuVelocity.x-=2.0f;
		}

		// change gui scale
		if ((entryID == 6) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
			menuVelocity.x-=2.0f;
		}
	}

	if (controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192))
	{
		// change hud scale 
		if ((entryID == 5) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
			menuVelocity.x+=2.0f;
		}

		// change gui scale
		if ((entryID == 6) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
			menuVelocity.x+=2.0f;
		}

	}

	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250;
		if (config.game_type > 10000)
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Activate BRASSA (Shader Analyzer)\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			menuHelperRect.top += MENU_ITEM_SEPARATION;
		}
		DrawTextShadowed(hudFont, hudMainMenu, "World-Scale Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Convergence Adjustment\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "HUD Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "GUI Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION; float hudQSHeight = (float)menuHelperRect.top * fScaleY;
		DrawTextShadowed(hudFont, hudMainMenu, "HUD Quick Setting : \n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		DrawTextShadowed(hudFont, hudMainMenu, "GUI Quick Setting : \n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Overall Settings\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "VRBoost Values\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Position Tracking Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Restore Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw HUD quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)hudQSHeight; rect.y2 = (int)(hudQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

		// draw GUI quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)guiQSHeight; rect.y2 = (int)(guiQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* BRASSA World Scale Calibration.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_WorldScale )
	// base values
	float separationChange = 0.005f;
	static UINT gameXScaleUnitIndex = 0;

	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	// sort the game unit vector
	std::sort (m_gameXScaleUnits.begin(), m_gameXScaleUnits.end());

	// enter ? rshift ? increase gameXScaleUnitIndex
	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (controls.Key_Down(VK_LSHIFT))
		{
			if (gameXScaleUnitIndex>0) --gameXScaleUnitIndex;
		}
		else
			gameXScaleUnitIndex++;
		menuVelocity.x+=2.0f;
	}

	// game unit index out of range ?
	if ((gameXScaleUnitIndex != 0) && (gameXScaleUnitIndex >= m_gameXScaleUnits.size()))
		gameXScaleUnitIndex = m_gameXScaleUnits.size()-1;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
		BRASSA_UpdateConfigSettings();
	}

	/**
	* LEFT : Decrease world scale (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			separationChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT)) {
			separationChange *= 10.0f;
		} 
		else if(controls.Key_Down(VK_MENU))
		{
			separationChange /= 500.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeWorldScale(-separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	/**
	* RIGHT : Increase world scale (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			separationChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT))
		{
			separationChange *= 10.0f;
		}
		else if(controls.Key_Down(VK_MENU))
		{
			separationChange /= 500.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	if(hudFont){

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		// standard hud size, will be scaled later to actual viewport
		char vcString[1024];
		int width = BRASSA_PIXEL_WIDTH;
		int height = BRASSA_PIXEL_HEIGHT;

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		// arbitrary formular... TODO !! find a more nifty solution
		float BlueLineCenterAsPercentage = config.lensXCenterOffset * 0.2f;

		float horWidth = 0.15f;
		int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
		int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

		int hashTop = (int)(viewportHeight  * 0.48f);
		int hashBottom = (int)(viewportHeight  * 0.52f);

		RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
		sprintf_s(vcString, 1024, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Right, rec3, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Left, rec3, D3DCOLOR_ARGB(255,0,0,255));

		// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec4, D3DCOLOR_ARGB(255,255,0,0));
		else
			ClearRect(vireio::RenderPosition::Right, rec4, D3DCOLOR_ARGB(255,255,0,0));

		// horizontal line
		D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,0,0,255));

		// hash lines
		int hashNum = 10;
		float hashSpace = horWidth*viewportWidth / (float)hashNum;
		for(int i=0; i<=hashNum; i++) {
			D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
			if (!config.swap_eyes)
				ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,255,255,0));
			else
				ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,255,255,0));
		}

		rec2.left = (int)(width*0.35f);
		rec2.top = (int)(height*0.83f);
		sprintf_s(vcString, 1024, "World-Scale Calibration");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
		DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		float gameUnit = m_spShaderViewAdjustment->WorldScale();

		// actual game unit chosen ... in case game has called SetTransform(>projection<);
		if (m_bProjectionTransformSet)
		{
			// get the scale the 
			float gameXScale = m_gameXScaleUnits[gameXScaleUnitIndex];

			// get the scale the driver projects
			D3DXMATRIX driverProjection = m_spShaderViewAdjustment->Projection();
			float driverXScale = driverProjection._11;

			// gameUnit = (driverWorldScale * driverXScale) /  gameXScale
			gameUnit = ((m_spShaderViewAdjustment->WorldScale()) * driverXScale ) / gameXScale;

			rec10.top = (int)(height*0.77f); rec10.left = (int)(width*0.45f);
			sprintf_s(vcString,"Actual Units %u/%u", gameXScaleUnitIndex, m_gameXScaleUnits.size());
			DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		}

		//Column 1:
		//1 Game Unit = X Meters
		//1 Game Unit = X Centimeters
		//1 Game Unit = X Feet
		//1 Game Unit = X Inches
		//Column 2:
		//1 Meter = X Game Units
		//1 Centimeter = X Game Units
		//1 Foot = X Game Units
		//1 Inch = X Game Units
		rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.28f);
		float meters = 1 / gameUnit;
		sprintf_s(vcString,"1 Game Unit = %g Meters", meters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float centimeters = meters * 100.0f;
		sprintf_s(vcString,"1 Game Unit = %g CM", centimeters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float feet = meters * 3.2808399f;
		sprintf_s(vcString,"1 Game Unit = %g Feet", feet);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float inches = feet * 12.0f;
		sprintf_s(vcString,"1 Game Unit = %g In.", inches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		RECT rec11 = {(int)(width*0.52f), (int)(height*0.6f),width,height};
		sprintf_s(vcString,"1 Meter      = %g Game Units", gameUnit);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToCentimeter =  gameUnit / 100.0f;
		sprintf_s(vcString,"1 CM         = %g Game Units", gameUnitsToCentimeter);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToFoot = gameUnit / 3.2808399f;
		sprintf_s(vcString,"1 Foot       = %g Game Units", gameUnitsToFoot);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToInches = gameUnit / 39.3700787f;
		sprintf_s(vcString,"1 Inch       = %g Game Units", gameUnitsToInches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));  
		hudMainMenu->End();

		// draw description text box
		hudTextBox->Begin(D3DXSPRITE_ALPHABLEND);
		hudTextBox->SetTransform(&matScale);
		RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
		sprintf_s(vcString, 1024,
			"In the right eye view, walk up as close as\n"
			"possible to a 90 degree vertical object and\n"
			"align the BLUE vertical line with its edge.\n"
			"Good examples include a wall corner, a table\n"
			"corner, a square post, etc.  While looking at\n"
			"the left image, adjust the World View setting\n"
			"until the same object's edge is on the fourth\n"
			"notch in the >Negative Parallax< section (to\n"
			"the right of the RED line).  If objects go \n"
			"beyond this point, reduce the World Scale \n"
			"further.  Try to keep the BLUE line aligned\n"
			"while changing the World Scale.  Adjust \n"
			"further for comfort and game unit accuracy.\n"
			);
		DrawTextShadowed(hudFont, hudTextBox, vcString, -1, &rec8, 0, D3DCOLOR_ARGB(255,255,255,255));
		hudTextBox->Draw(NULL, &rec8, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw description box scroll bar
		float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
		D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
		DrawScrollbar(vireio::RenderPosition::Left, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));
		DrawScrollbar(vireio::RenderPosition::Right, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));

		hudTextBox->End();
	}
}

/**
* BRASSA Convergence Adjustment.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_Convergence )
	// base values
	float convergenceChange = 0.05f;

	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
		BRASSA_UpdateConfigSettings();
	}

	/**
	* LEFT : Decrease convergence (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			convergenceChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT)) {
			convergenceChange *= 10.0f;
		} 

		if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeConvergence(-convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	/**
	* RIGHT : Increase convergence (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			convergenceChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT))
		{
			convergenceChange *= 10.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4C))
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	if(hudFont){

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		// standard hud size, will be scaled later to actual viewport
		char vcString[1024];
		int width = BRASSA_PIXEL_WIDTH;
		int height = BRASSA_PIXEL_HEIGHT;

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		// arbitrary formular... TODO !! find a more nifty solution
		float BlueLineCenterAsPercentage = config.lensXCenterOffset * 0.2f;

		float horWidth = 0.15f;
		int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
		int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

		int hashTop = (int)(viewportHeight  * 0.48f);
		int hashBottom = (int)(viewportHeight  * 0.52f);

		RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
		sprintf_s(vcString, 1024, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Right, rec3, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Left, rec3, D3DCOLOR_ARGB(255,0,0,255));

		// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec4, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec4, D3DCOLOR_ARGB(255,0,0,255));

		// horizontal line
		D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,0,0,255));

		// hash lines
		int hashNum = 10;
		float hashSpace = horWidth*viewportWidth / (float)hashNum;
		for(int i=0; i<=hashNum; i++) {
			D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
			if (!config.swap_eyes)
				ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,255,255,0));
			else
				ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,255,255,0));
		}

		rec2.left = (int)(width*0.35f);
		rec2.top = (int)(height*0.83f);
		sprintf_s(vcString, 1024, "Convergence Adjustment");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// output convergence
		RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
		DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		// Convergence Screen = X Meters = X Feet
		rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.385f);
		float meters = m_spShaderViewAdjustment->Convergence();
		sprintf_s(vcString,"Convergence Screen = %g Meters", meters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float centimeters = meters * 100.0f;
		sprintf_s(vcString,"Convergence Screen = %g CM", centimeters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float feet = meters * 3.2808399f;
		sprintf_s(vcString,"Convergence Screen = %g Feet", feet);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float inches = feet * 12.0f;
		sprintf_s(vcString,"Convergence Screen = %g Inches", inches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));  
		hudMainMenu->End();

		// draw description text box
		hudTextBox->Begin(D3DXSPRITE_ALPHABLEND);
		hudTextBox->SetTransform(&matScale);
		RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
		sprintf_s(vcString, 1024,
			"Note that the Convergence Screens distance\n"
			"is measured in physical meters and should\n"
			"only be adjusted to match Your personal\n"
			"depth cognition after You calibrated the\n"
			"World Scale accordingly.\n"
			"In the right eye view, walk up as close as\n"
			"possible to a 90 degree vertical object and\n"
			"align the BLUE vertical line with its edge.\n"
			"Good examples include a wall corner, a table\n"
			"corner, a square post, etc.\n"
			);
		DrawTextShadowed(hudFont, hudTextBox, vcString, -1, &rec8, 0, D3DCOLOR_ARGB(255,255,255,255));
		hudTextBox->Draw(NULL, &rec8, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw description box scroll bar
		float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
		D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
		DrawScrollbar(vireio::RenderPosition::Left, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));
		DrawScrollbar(vireio::RenderPosition::Right, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));

		hudTextBox->End();
	}
}

/**
* BRASSA HUD Calibration.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_HUD )
	UINT menuEntryCount = 10;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					hudHotkeys[index] = (byte)i;
			}
	}
	else
	{
		if (controls.Key_Down(VK_ESCAPE))
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == 8)
			{
				BRASSA_mode = BRASSA_Modes::MAINMENU;
				BRASSA_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == 9)
			{
				BRASSA_mode = BRASSA_Modes::INACTIVE;
				BRASSA_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					hudHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192))
		{
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
				menuVelocity.x-=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hudDistancePresets[(int)hud3DDepthMode]-=0.01f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x-=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hud3DDepthPresets[(int)hud3DDepthMode]-=0.002f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x-=0.7f;
			}
		}

		if (controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192))
		{
			// change hud scale
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
				menuVelocity.x+=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x+=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x+=0.7f;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float hudQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (hud3DDepthMode)
		{
		case D3DProxyDevice::HUD_DEFAULT:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Default", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_SMALL:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Small", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_LARGE:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Large", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_FULL:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Full", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		default:
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"HUD Distance : %g", RoundBrassaValue(hudDistancePresets[(int)hud3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"HUD's 3D Depth : %g", RoundBrassaValue(hud3DDepthPresets[(int)hud3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Switch< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[0]));
		if ((hotkeyCatch) && (entryID==3))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Default< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[1]));
		if ((hotkeyCatch) && (entryID==4))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Small< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[2]));
		if ((hotkeyCatch) && (entryID==5))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Large< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[3]));
		if ((hotkeyCatch) && (entryID==6))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Full< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[4]));
		if ((hotkeyCatch) && (entryID==7))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw HUD quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)hudQSHeight; rect.y2 = (int)(hudQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* BRASSA GUI Calibration.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_GUI )
	UINT menuEntryCount = 10;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = (byte)i;
			}
	}
	else
	{
		if (controls.Key_Down(VK_ESCAPE))
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == 8)
			{
				BRASSA_mode = BRASSA_Modes::MAINMENU;
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == 9)
			{
				BRASSA_mode = BRASSA_Modes::INACTIVE;
				BRASSA_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192))
		{
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
				menuVelocity.x-=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					guiSquishPresets[(int)gui3DDepthMode]-=0.01f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x-=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					gui3DDepthPresets[(int)gui3DDepthMode]-=0.002f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x-=0.7f;
			}
		}

		if (controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192))
		{
			// change gui scale
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
				menuVelocity.x+=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x+=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x+=0.5;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (gui3DDepthMode)
		{
		case D3DProxyDevice::GUI_DEFAULT:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Default", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_SMALL:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Small", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_LARGE:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Large", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_FULL:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Full", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		default:
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"GUI Size : %g", RoundBrassaValue(guiSquishPresets[(int)gui3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"GUI's 3D Depth : %g", RoundBrassaValue(gui3DDepthPresets[(int)gui3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Switch< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[0]));
		if ((hotkeyCatch) && (entryID==3))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Default< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[1]));
		if ((hotkeyCatch) && (entryID==4))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Small< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[2]));
		if ((hotkeyCatch) && (entryID==5))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Large< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[3]));
		if ((hotkeyCatch) && (entryID==6))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Full< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[4]));
		if ((hotkeyCatch) && (entryID==7))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw GUI quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)guiQSHeight; rect.y2 = (int)(guiQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* BRASSA Settings.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_Settings )
	//Use enumeration for menu items to avoid confusion
	enum 
	{
		SWAP_EYES,
		IPD_OFFSET,
		Y_OFFSET,
		DISTORTION_SCALE,
		STEREO_SCREENSHOTS,
		YAW_MULT,
		PITCH_MULT,
		ROLL_MULT,
		RESET_MULT,
		ROLL_ENABLED,
		FORCE_MOUSE_EMU,
		TOGGLE_VRBOOST,
		HOTKEY_VRBOOST,
		BACK_BRASSA,
		BACK_GAME,
		NUM_MENU_ITEMS
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				toggleVRBoostHotkey = (byte)i;
			}
	}
	else
	{
		/**
		* ESCAPE : Set BRASSA inactive and save the configuration.
		***/
		if (controls.Key_Down(VK_ESCAPE))
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x += 4.0f;
			}
			// screenshot
			if (entryID == STEREO_SCREENSHOTS)
			{
				// render 3 frames to get screenshots without BRASSA
				screenshot = 3;
				BRASSA_mode = BRASSA_Modes::INACTIVE;
			}
			// reset multipliers
			if (entryID == RESET_MULT)
			{
				tracker->multiplierYaw = 25.0f;
				tracker->multiplierPitch = 25.0f;
				tracker->multiplierRoll = 1.0f;
				menuVelocity.x += 4.0f;
			}

			// force mouse emulation
			if (entryID == ROLL_ENABLED)
			{
				config.rollEnabled = !config.rollEnabled;
				m_spShaderViewAdjustment->SetRollEnabled(config.rollEnabled);
				menuVelocity.x += 4.0f;
			}

			// force mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				m_bForceMouseEmulation = !m_bForceMouseEmulation;

				if ((m_bForceMouseEmulation) && (tracker->getStatus() >= MTS_OK) && (!m_bSurpressHeadtracking))
					tracker->setMouseEmulation(true);

				if ((!m_bForceMouseEmulation) && (hmVRboost) && (m_VRboostRulesPresent)  && (tracker->getStatus() >= MTS_OK))
					tracker->setMouseEmulation(false);

				menuVelocity.x += 4.0f;
			}
			// Toggle VRBoost
			if (entryID == TOGGLE_VRBOOST)
			{
				if (hmVRboost!=NULL)
				{
					m_pVRboost_ReleaseAllMemoryRules();
					m_bVRBoostToggle = !m_bVRBoostToggle;
					if (tracker->getStatus() >= MTS_OK)
						tracker->resetOrientationAndPosition();
					menuVelocity.x+=2.0f;
				}
			}
			// VRBoost hotkey
			if (entryID == HOTKEY_VRBOOST)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == BACK_BRASSA)
			{
				BRASSA_mode = BRASSA_Modes::MAINMENU;
				BRASSA_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == BACK_GAME)
			{
				BRASSA_mode = BRASSA_Modes::INACTIVE;
				BRASSA_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= DISTORTION_SCALE) && (entryID <= ROLL_MULT) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_BACK) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
				this->stereoView->IPDOffset = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			//y offset
			if (entryID == Y_OFFSET)
 			{
				this->stereoView->YOffset = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// distortion
			if (entryID == DISTORTION_SCALE)
			{
				this->stereoView->DistortionScale = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// reset hotkey
			if (entryID == HOTKEY_VRBOOST)
			{
				toggleVRBoostHotkey = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if ((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x-=2.0f;
			}
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
				{
					if (this->stereoView->IPDOffset > 0.1f)
						this->stereoView->IPDOffset -= 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->IPDOffset > -0.1f)
						this->stereoView->IPDOffset -= 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// y-offset
			if (entryID == Y_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
				{
					if (this->stereoView->YOffset > 0.1f)
						this->stereoView->YOffset -= 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->YOffset > -0.1f)
						this->stereoView->YOffset -= 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// distortion
			if (entryID == DISTORTION_SCALE)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					this->stereoView->DistortionScale -= 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					this->stereoView->DistortionScale -= 0.01f;
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// yaw multiplier
			if (entryID == YAW_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					tracker->multiplierYaw += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierYaw -= 0.5f;
				menuVelocity.x -= 0.7f;
			}
			// pitch multiplier
			if (entryID == PITCH_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					tracker->multiplierPitch += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierPitch -= 0.5f;
				menuVelocity.x -= 0.7f;
			}
			// roll multiplier
			if (entryID == ROLL_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
					tracker->multiplierRoll += 0.05f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierRoll -= 0.05f;
				menuVelocity.x -= 0.7f;
			}

			// mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				m_bForceMouseEmulation = false;

				if ((hmVRboost) && (m_VRboostRulesPresent) && (tracker->getStatus() >= MTS_OK))
					tracker->setMouseEmulation(false);

				menuVelocity.x-=2.0f;
			}
		}

		if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x-=2.0f;
			}
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
				{
					if (this->stereoView->IPDOffset < 0.1f)
						this->stereoView->IPDOffset += 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->IPDOffset < 0.1f)
						this->stereoView->IPDOffset += 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// y-offset
			if (entryID == Y_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
				{
					if (this->stereoView->YOffset < 0.1f)
						this->stereoView->YOffset += 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->YOffset < 0.1f)
						this->stereoView->YOffset += 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			if (entryID == DISTORTION_SCALE)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					this->stereoView->DistortionScale += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					this->stereoView->DistortionScale += 0.01f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// yaw multiplier
			if (entryID == YAW_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					tracker->multiplierYaw += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierYaw += 0.5f;
				menuVelocity.x += 0.7f;
			}
			// pitch multiplier
			if (entryID == PITCH_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					tracker->multiplierPitch += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierPitch += 0.5f;
				menuVelocity.x += 0.7f;
			}
			// roll multiplier
			if (entryID == ROLL_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
					tracker->multiplierRoll += 0.05f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierRoll += 0.05f;
				menuVelocity.x += 0.7f;
			}
			// mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				if(tracker->getStatus() >= MTS_OK)
				{
					tracker->setMouseEmulation(true);
					m_bForceMouseEmulation = true;
				}

				menuVelocity.x-=2.0f;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (stereoView->swapEyes)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Swap Eyes : True", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Swap Eyes : False", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"IPD-Offset : %1.3f", RoundBrassaValue(this->stereoView->IPDOffset));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Y-Offset : %1.3f", RoundBrassaValue(this->stereoView->YOffset));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Distortion Scale : %g", RoundBrassaValue(this->stereoView->DistortionScale));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Stereo Screenshots", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Yaw multiplier : %g", RoundBrassaValue(tracker->multiplierYaw));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Pitch multiplier : %g", RoundBrassaValue(tracker->multiplierPitch));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Roll multiplier : %g", RoundBrassaValue(tracker->multiplierRoll));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Reset Multipliers", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_spShaderViewAdjustment->RollEnabled())
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : True", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : False", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_bForceMouseEmulation)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : true", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : false", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_bVRBoostToggle)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Toggle VRBoost : On", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Toggle VRBoost : Off", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 128, 128));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Toggle VRBoost< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(toggleVRBoostHotkey));
		if ((hotkeyCatch) && (entryID==11))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 70;
		sprintf_s(vcString,"(current VShader Count : %u)", m_VertexShaderCountLastFrame);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 128, 255, 128));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}


/**
* BRASSA Positional Tracking Settings.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_PosTracking )
	enum
	{
		TOGGLE_TRACKING,
		TRACKING_MULT,
		RESET_HMD,
		BACK_BRASSA,
		BACK_GAME,
		NUM_MENU_ITEMS
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
		BRASSA_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// toggle position tracking
		if (entryID == TOGGLE_TRACKING)
		{
			m_bPosTrackingToggle = !m_bPosTrackingToggle;

			if (!m_bPosTrackingToggle)
				m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);

			menuVelocity.x += 4.0f;
		}

		// reset orientation
		if (entryID == RESET_HMD)
		{
			tracker->resetOrientationAndPosition();
			menuVelocity.x += 4.0f;
		}

		// back to main menu
		if (entryID == BACK_BRASSA
			)
		{
			BRASSA_mode = BRASSA_Modes::MAINMENU;
			BRASSA_UpdateConfigSettings();
			menuVelocity.x+=2.0f;
		}

		// back to game
		if (entryID == BACK_GAME)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}
	}

	if (controls.Key_Down(VK_BACK))
	{
		if ((entryID == TRACKING_MULT) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			config.position_multiplier = 1.0f;
			menuVelocity.x+=2.0f;
		}
	}

	if ((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// position multiplier
		if (entryID == TRACKING_MULT)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
				config.position_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_multiplier -= 0.01f;
			menuVelocity.x -= 1.0f;
		}
	}

	if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// position multiplier
		if (entryID == TRACKING_MULT)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
				config.position_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_multiplier += 0.01f;
			menuVelocity.x += 1.0f;
		}
	}


	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Position Tracking Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];
		switch (m_bPosTrackingToggle)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Positional Tracking (CTRL + P) : On", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Positional Tracking (CTRL + P) : Off", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 128, 128));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Position Tracking multiplier : %g", RoundBrassaValue(config.position_multiplier));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Reset HMD Orientation (CTRL + R)", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* VRBoost constant value sub-menu.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_VRBoostValues )
	UINT menuEntryCount = 14;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
		BRASSA_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// back to main menu
		if (entryID == 12)
		{
			BRASSA_mode = BRASSA_Modes::MAINMENU;
			menuVelocity.x+=2.0f;
		}
		// back to game
		if (entryID == 13)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			BRASSA_UpdateConfigSettings();
		}
	}

	if ((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 0) && (entryID <=11))
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_LEFT) && !controls.Key_Down(0x4A))
				VRBoostValue[24+entryID] += 0.1f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				VRBoostValue[24+entryID] -= 0.1f;
			menuVelocity.x-=0.1f;
		}
	}

	if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 0) && (entryID <=11))
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down(0x4C))
				VRBoostValue[24+entryID] += 0.1f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				VRBoostValue[24+entryID] += 0.1f;
			menuVelocity.x+=0.1f;
		}
	}

	if (controls.Key_Down(VK_BACK) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 3) && (entryID <=11))
		{
			VRBoostValue[24+entryID] = 0.0f;
		}
	}
	
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];
		sprintf_s(vcString,"World FOV : %g", RoundBrassaValue(VRBoostValue[24]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Player FOV : %g", RoundBrassaValue(VRBoostValue[25]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Far Plane FOV : %g", RoundBrassaValue(VRBoostValue[26]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate X : %g", RoundBrassaValue(VRBoostValue[27]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate Y : %g", RoundBrassaValue(VRBoostValue[28]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate Z : %g", RoundBrassaValue(VRBoostValue[29]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Distance : %g", RoundBrassaValue(VRBoostValue[30]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Zoom : %g", RoundBrassaValue(VRBoostValue[31]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Horizon Adjustment : %g", RoundBrassaValue(VRBoostValue[32]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 1 : %g", RoundBrassaValue(VRBoostValue[33]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 2 : %g", RoundBrassaValue(VRBoostValue[34]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 2 : %g", RoundBrassaValue(VRBoostValue[35]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* BRASSA menu border velocity updated here
* Arrow up/down need to be done via call from Present().
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_UpdateBorder )
	// handle controls 
	if (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::PRESENT)
		HandleTracking();

	// draw BRASSA
	if (m_deviceBehavior.whenToRenderBRASSA == DeviceBehavior::PRESENT)
	{
		if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
			BRASSA();
		else
			BRASSA_AdditionalOutput();
	}

	// first, calculate a time scale to adjust the menu speed for the frame speed of the game
	float timeStamp;
	timeStamp = (float)GetTickCount()/1000.0f;
	menuSeconds = timeStamp-menuTime;
	menuTime = timeStamp;
	// tested having about 50 fps, so menu velocity is based on that
	float timeScale = (float)menuSeconds*50;

	// menu velocity present ? in case calculate diminution of the velocity
	if (menuVelocity != D3DXVECTOR2(0.0f, 0.0f))
	{
		float diminution = 0.05f;
		diminution *= timeScale;
		if (diminution > 1.0f) diminution = 1.0f;
		menuVelocity*=1.0f-diminution;

		// set velocity to zero in case of low velocity
		if ((menuVelocity.y<0.9f) && (menuVelocity.y>-0.9f) &&
			(menuVelocity.x<0.7f) && (menuVelocity.x>-0.7f))
			menuVelocity = D3DXVECTOR2(0.0f, 0.0f);
	}

	// brassa menu active ? handle up/down controls
	if (BRASSA_mode != BRASSA_Modes::INACTIVE)
	{
		int viewportHeight = stereoView->viewport.Height;

		float fScaleY = ((float)viewportHeight / (float)1080.0f);
		if ((controls.Key_Down(VK_UP) || controls.Key_Down(0x49) || (controls.xInputState.Gamepad.sThumbLY>8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=-2.7f;
		if ((controls.Key_Down(VK_DOWN) || controls.Key_Down(0x4B) || (controls.xInputState.Gamepad.sThumbLY<-8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=2.7f;
		if ((controls.Key_Down(VK_PRIOR) || controls.Key_Down(0x55)) && (menuVelocity.y==0.0f))
			menuVelocity.y=-15.0f;
		if ((controls.Key_Down(VK_NEXT) ||controls.Key_Down(0x4F)) && (menuVelocity.y==0.0f))
			menuVelocity.y=15.0f;
		borderTopHeight += (menuVelocity.y+menuAttraction.y)*fScaleY*timeScale;
	}
}

/**
* Updates the current config based on the current device settings.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_UpdateConfigSettings )
	config.roll_multiplier = tracker->multiplierRoll;
	config.yaw_multiplier = tracker->multiplierYaw;
	config.pitch_multiplier = tracker->multiplierPitch;
	config.YOffset = stereoView->YOffset;
	config.IPDOffset = stereoView->IPDOffset;
	config.swap_eyes = stereoView->swapEyes;
	config.DistortionScale = stereoView->DistortionScale;
	config.hud3DDepthMode = (int)hud3DDepthMode;
	for (int i = 0; i < 4; i++)
	{
		config.hud3DDepthPresets[i] = hud3DDepthPresets[i];
		config.hudDistancePresets[i] = hudDistancePresets[i];
		config.hudHotkeys[i] = hudHotkeys[i];
	}
	config.hudHotkeys[4] = hudHotkeys[4];

	config.gui3DDepthMode = (int)gui3DDepthMode;
	for (int i = 0; i < 4; i++)
	{
		config.gui3DDepthPresets[i] = gui3DDepthPresets[i];
		config.guiSquishPresets[i] = guiSquishPresets[i];
		config.guiHotkeys[i] = guiHotkeys[i];
	}
	config.guiHotkeys[4] = guiHotkeys[4];

	config.VRBoostResetHotkey = toggleVRBoostHotkey;
	config.WorldFOV = VRBoostValue[VRboostAxis::WorldFOV];
	config.PlayerFOV = VRBoostValue[VRboostAxis::PlayerFOV];
	config.FarPlaneFOV = VRBoostValue[VRboostAxis::FarPlaneFOV];
	config.CameraTranslateX = VRBoostValue[VRboostAxis::CameraTranslateX];
	config.CameraTranslateY = VRBoostValue[VRboostAxis::CameraTranslateY];
	config.CameraTranslateZ = VRBoostValue[VRboostAxis::CameraTranslateZ];
	config.CameraDistance = VRBoostValue[VRboostAxis::CameraDistance];
	config.CameraZoom = VRBoostValue[VRboostAxis::CameraZoom];
	config.CameraHorizonAdjustment = VRBoostValue[VRboostAxis::CameraHorizonAdjustment];
	config.ConstantValue1 = VRBoostValue[VRboostAxis::ConstantValue1];
	config.ConstantValue2 = VRBoostValue[VRboostAxis::ConstantValue2];
	config.ConstantValue3 = VRBoostValue[VRboostAxis::ConstantValue3];

	m_spShaderViewAdjustment->Save( config );
}

/**
* Updates all device settings read from the current config.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_UpdateDeviceSettings )
	m_spShaderViewAdjustment->Load( config );
	stereoView->DistortionScale = config.DistortionScale;

	// HUD
	for (int i = 0; i < 4; i++)
	{
		hud3DDepthPresets[i] = config.hud3DDepthPresets[i];
		hudDistancePresets[i] = config.hudDistancePresets[i];
		hudHotkeys[i] = config.hudHotkeys[i];
	}
	hudHotkeys[4] = config.hudHotkeys[4];
	ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)config.hud3DDepthMode);

	// GUI
	for (int i = 0; i < 4; i++)
	{
		gui3DDepthPresets[i] = config.gui3DDepthPresets[i];
		guiSquishPresets[i] = config.guiSquishPresets[i];
		guiHotkeys[i] = config.guiHotkeys[i];
	}
	guiHotkeys[4] = config.guiHotkeys[4];
	ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)config.gui3DDepthMode);

	// VRBoost
	toggleVRBoostHotkey = config.VRBoostResetHotkey;
	VRBoostValue[VRboostAxis::WorldFOV] = config.WorldFOV;
	VRBoostValue[VRboostAxis::PlayerFOV] = config.PlayerFOV;
	VRBoostValue[VRboostAxis::FarPlaneFOV] = config.FarPlaneFOV;
	VRBoostValue[VRboostAxis::CameraTranslateX] = config.CameraTranslateX;
	VRBoostValue[VRboostAxis::CameraTranslateY] = config.CameraTranslateY;
	VRBoostValue[VRboostAxis::CameraTranslateZ] = config.CameraTranslateZ;
	VRBoostValue[VRboostAxis::CameraDistance] = config.CameraDistance;
	VRBoostValue[VRboostAxis::CameraZoom] = config.CameraZoom;
	VRBoostValue[VRboostAxis::CameraHorizonAdjustment] = config.CameraHorizonAdjustment;
	VRBoostValue[VRboostAxis::ConstantValue1] = config.ConstantValue1;
	VRBoostValue[VRboostAxis::ConstantValue2] = config.ConstantValue2;
	VRBoostValue[VRboostAxis::ConstantValue3] = config.ConstantValue3;

	// set behavior accordingly to game type
	int gameType = config.game_type;
	if (gameType>10000) gameType-=10000;
	switch(gameType)
	{
	case D3DProxyDevice::FIXED:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::PRESENT;
		break;
	case D3DProxyDevice::SOURCE:
	case D3DProxyDevice::SOURCE_L4D:
	case D3DProxyDevice::SOURCE_ESTER:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::UNREAL:
	case D3DProxyDevice::UNREAL_MIRROR:
	case D3DProxyDevice::UNREAL_UT3:
	case D3DProxyDevice::UNREAL_BIOSHOCK:
	case D3DProxyDevice::UNREAL_BORDERLANDS:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::EGO:
	case D3DProxyDevice::EGO_DIRT:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::REALV:
	case D3DProxyDevice::REALV_ARMA:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::UNITY:
	case D3DProxyDevice::UNITY_SLENDER:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::GAMEBRYO:
	case D3DProxyDevice::GAMEBRYO_SKYRIM:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::LFS:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::CDC:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	default:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderBRASSA = DeviceBehavior::WhenToDo::PRESENT;
		break;
	}
}

/**
* Additional output when menu is not drawn.
***/
METHOD_IMPL( void , , D3DProxyDevice , BRASSA_AdditionalOutput )
	// draw vrboost toggle indicator
	if (m_fVRBoostIndicator>0.0f)
	{
		D3DRECT rec;
		rec.x1 = (int)(viewportWidth*(0.5f-(m_fVRBoostIndicator*0.05f))); rec.x2 = (int)(viewportWidth*(0.5f+(m_fVRBoostIndicator*0.05f))); 
		rec.y1 = (int)(viewportHeight*(0.4f-(m_fVRBoostIndicator*0.05f))); rec.y2 = (int)(viewportHeight*(0.4f+(m_fVRBoostIndicator*0.05f)));
		if (m_bVRBoostToggle)
			ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,64,255,64));
		else
			ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,255,128,128));

		// update the indicator float
		m_fVRBoostIndicator-=menuSeconds;
	}

	//GBCODE - Test VR Mouse Positioning
	if (m_bShowVRMouse)
	{
		POINT pt;   
		GetCursorPos(&pt); 
		D3DRECT rec2;	
		//D3DRECT rec2hud;	
		rec2.x1 = (int)-5 + ((pt.x * guiSquishPresets[(int)gui3DDepthMode]) + (((1 - guiSquishPresets[(int)gui3DDepthMode]) / 2) * viewportWidth)); 
		rec2.x2 = rec2.x1 + 10; 
		rec2.y1 = (int)-5 + ((pt.y * guiSquishPresets[(int)gui3DDepthMode]) + (((1 - guiSquishPresets[(int)gui3DDepthMode]) / 2) * viewportHeight)); 
		rec2.y2 = rec2.y1 + 10; 	
		
		ClearRect(vireio::RenderPosition::Left, rec2, D3DCOLOR_ARGB(255,255,255,255));
		ClearRect(vireio::RenderPosition::Right, rec2, D3DCOLOR_ARGB(255,255,255,255));
		rec2.x1 += 2;
		rec2.x2 -= 2;
		rec2.y1 += 2;
		rec2.y2 -= 2;
		ClearRect(vireio::RenderPosition::Left, rec2, D3DCOLOR_ARGB(0,0,0,0));
		ClearRect(vireio::RenderPosition::Right, rec2, D3DCOLOR_ARGB(0,0,0,0));	
		/*
		//Hud Depth = 0 = Full Size 0.5 = Half Size
		/*rec2hud.x1 = (int)-5 + (hudDistancePresets[(int)hud3DDepthMode]) * viewportWidth)) + (pt.x * (1 - hudDistancePresets[(int)hud3DDepthMode]));
		rec2hud.x2 = rec2hud.x1 + 10;		
		rec2hud.y1 = (int)-5 + (hudDistancePresets[(int)hud3DDepthMode]) * viewportHeight)) + (pt.y * (1 - hudDistancePresets[(int)hud3DDepthMode]));
		rec2hud.y2 = rec2hud.y1 + 10;
		
		rec2hud.x1 = (int)-5 + ((pt.x * (1 - hudDistancePresets[(int)hud3DDepthMode])) + (((hudDistancePresets[(int)hud3DDepthMode]) / 2) * viewportWidth)); 
		rec2hud.x2 = rec2hud.x1 + 10; 
		rec2hud.y1 = (int)-5 + ((pt.y * (1 - hudDistancePresets[(int)hud3DDepthMode])) + (((hudDistancePresets[(int)hud3DDepthMode]) / 2) * viewportHeight)); 
		rec2hud.y2 = rec2hud.y1 + 10; 	
		ClearRect(vireio::RenderPosition::Left, rec2hud, D3DCOLOR_ARGB(255,255,255,255));
		ClearRect(vireio::RenderPosition::Right, rec2hud, D3DCOLOR_ARGB(255,255,255,255));

		rec2hud.x1 = 0;
		rec2hud.x2 = (viewportWidth / 2) * (hudDistancePresets[(int)hud3DDepthMode]) + 200;		
		rec2hud.y1 = 0;
		rec2hud.y2 = (viewportHeight / 2) * (hudDistancePresets[(int)hud3DDepthMode]) + 200;
		ClearRect(vireio::RenderPosition::Left, rec2hud, D3DCOLOR_ARGB(255,255,255,255));
		//ClearRect(vireio::RenderPosition::Right, rec2hud, D3DCOLOR_ARGB(255,255,255,255));

		rec2hud.x1 = viewportWidth - ((hudDistancePresets[(int)hud3DDepthMode]) * viewportWidth);
		rec2hud.x2 = rec2hud.x1 + 10;		
		rec2hud.y1 = viewportHeight - ((hudDistancePresets[(int)hud3DDepthMode]) * viewportHeight);
		rec2hud.y2 = rec2hud.y1 + 10;
		ClearRect(vireio::RenderPosition::Left, rec2hud, D3DCOLOR_ARGB(255,255,255,255));
		ClearRect(vireio::RenderPosition::Right, rec2hud, D3DCOLOR_ARGB(255,255,255,255));*/
	}
	
	// do not squish the viewport in case brassa menu is open - GBCODE - Why? Test on supported games. 
	//if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
	//return;

	//Finally, draw any popups if required
	DisplayCurrentPopup();
}

METHOD_IMPL( void , , D3DProxyDevice , DisplayCurrentPopup )
	if ((activePopup.popupType == VPT_NONE && show_fps == FPS_NONE) || 
		BRASSA_mode != BRASSA_Modes::INACTIVE ||
		!showNotifications)
		return;
	
	// output menu
	if (hudFont)
	{
		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		if (activePopup.popupType == VPT_STATS && config.isHmd )
		{
			sprintf_s(activePopup.line1, "HMD Description: %s", tracker->GetTrackerDescription()); 
			sprintf_s(activePopup.line2, "Yaw: %.3f Pitch: %.3f Roll: %.3f", tracker->primaryYaw, tracker->primaryPitch, tracker->primaryRoll); 
			sprintf_s(activePopup.line3, "X: %.3f Y: %.3f Z: %.3f", tracker->primaryX, tracker->primaryY, tracker->primaryZ); 
			sprintf_s(activePopup.line4, "VRBoost Active: %s     Rules Loaded: %s   Applied: %s", 
				(VRBoostStatus.VRBoost_Active ? "TRUE" : "FALSE"), 
				(VRBoostStatus.VRBoost_LoadRules ? "TRUE" : "FALSE"), 
				(VRBoostStatus.VRBoost_ApplyRules ? "TRUE" : "FALSE"));
			if (m_bPosTrackingToggle)
				strcpy_s(activePopup.line5, "HMD Positional Tracking Enabled");
			else
				strcpy_s(activePopup.line5, "HMD Positional Tracking Disabled");
		}

		if (activePopup.expired())
		{
			//Ensure we stop showing this popup
			activePopup.popupType = VPT_NONE;
			activePopup.reset();
		}

		D3DCOLOR popupColour;
		ID3DXFont *pFont;
		switch (activePopup.severity)
		{
			case VPS_TOAST:
				{
					popupColour = D3DCOLOR_ARGB(255, 255, 255, 255);
					pFont = popupFont;
				}
				break;
			case VPS_INFO:
				{
					popupColour = D3DCOLOR_ARGB(255, 128, 255, 128);
					pFont = popupFont;
				}
				break;
			case VPS_ERROR:
				{
					popupColour = D3DCOLOR_ARGB(255, 255, 0, 0);
					pFont = errorFont;
				}
				break;
		}

		menuHelperRect.left = 640;
		menuHelperRect.top = 480;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line1, -1, &menuHelperRect, 0, popupColour);
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line2, -1, &menuHelperRect, 0, popupColour);
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line3, -1, &menuHelperRect, 0, popupColour);
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line4, -1, &menuHelperRect, 0, popupColour);
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line5, -1, &menuHelperRect, 0, popupColour);
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(pFont, hudMainMenu, activePopup.line6, -1, &menuHelperRect, 0, popupColour);
 

		if (show_fps != FPS_NONE)
		{
			char buffer[256];
			if (show_fps == FPS_COUNT)
				sprintf_s(buffer, "       FPS: %.1f", fps);
			else if (show_fps == FPS_TIME)
				sprintf_s(buffer, "Frame Time: %.2f ms", 1000.0f / fps);

			D3DCOLOR colour = (fps <= 40) ? D3DCOLOR_ARGB(255, 255, 0, 0) : D3DCOLOR_ARGB(255, 255, 255, 255);;
			menuHelperRect.left = 850;
			menuHelperRect.top = 800;
			DrawTextShadowed(hudFont, hudMainMenu, buffer, -1, &menuHelperRect, 0, colour);
		}

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}

}


//FPS Calculator

#define MAXSAMPLES 100

float D3DProxyDevice::CalcFPS(){
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

	float FPS = (float)((double)MAXSAMPLES / ((double)ticksum / (double)perffreq.QuadPart));

	//char buffer[256];
	//sprintf(buffer, "FPS: %.1f\n", FPS);
	//OutputDebugStringA(buffer);

    return FPS;
}

//Logic for popup, need some priority logic here
METHOD_IMPL( void , , D3DProxyDevice , ShowPopup , VireioPopup& , popup )
	//Nothing to do if we are already showing this popup, or splash screen is currently displayed
	if (activePopup.popupType == popup.popupType ||
		activePopup.popupType == VPT_SPLASH)
		return;

//	if (popup.popupType == VPT_FPS || popup.popupType == VPT_STATS)
	{
		activePopup = popup;
		return;
	}

	if ( !config.isHmd )   //stereo type > 100 reserved specifically for HMDs
	{
	}
}

//DIsmiss popup if the popup type matches current displayed popup
METHOD_IMPL( void , , D3DProxyDevice , DismissPopup , VireioPopupType , popupType )
	if (activePopup.popupType == popupType)
		activePopup.reset();
}

/**
* Releases HUD font, shader registers, render targets, texture stages, vertex buffers, depth stencils, indices, shaders, declarations.
***/
METHOD_IMPL( void , , D3DProxyDevice , ReleaseEverything )
	// Fonts and any other D3DX interfaces should be released first.
	// They frequently hold stateblocks which are holding further references to other resources.
	if(hudFont) {
		hudFont->Release();
		hudFont = NULL;
	}


	m_spManagedShaderRegisters->ReleaseResources();

	if (m_pCapturingStateTo) {
		m_pCapturingStateTo->Release();
		m_pCapturingStateTo = NULL;
	}

	// one of these will still have a count of 1 until the backbuffer is released
	for(std::vector<D3D9ProxySurface*>::size_type i = 0; i != m_activeRenderTargets.size(); i++) 
	{
		if (m_activeRenderTargets[i] != NULL) {
			m_activeRenderTargets[i]->Release();
			m_activeRenderTargets[i] = NULL;
		}
	} 


	auto it = m_activeTextureStages.begin();
	while (it != m_activeTextureStages.end()) {
		if (it->second)
			it->second->Release();

		it = m_activeTextureStages.erase(it);
	}


	auto itVB = m_activeVertexBuffers.begin();
	while (itVB != m_activeVertexBuffers.end()) {
		if (itVB->second)
			itVB->second->Release();

		itVB = m_activeVertexBuffers.erase(itVB);
	}




	if (m_pActiveStereoDepthStencil) {
		m_pActiveStereoDepthStencil->Release();
		m_pActiveStereoDepthStencil = NULL;
	}

	if (m_pActiveIndicies) {
		m_pActiveIndicies->Release();
		m_pActiveIndicies = NULL;
	}

	if (m_pActivePixelShader) {
		m_pActivePixelShader->Release();
		m_pActivePixelShader = NULL;
	}

	if (m_pActiveVertexShader) {
		m_pActiveVertexShader->Release();
		m_pActiveVertexShader = NULL;
	}

	if (m_pActiveVertexDeclaration) {
		m_pActiveVertexDeclaration->Release();
		m_pActiveVertexDeclaration = NULL;
	}
}
/**
* Comparison made against active primary render target.
*
***/
METHOD_IMPL( bool , , D3DProxyDevice , isViewportDefaultForMainRT , CONST D3DVIEWPORT9* , pViewport )
	D3D9ProxySurface* pPrimaryRenderTarget = m_activeRenderTargets[0];
	D3DSURFACE_DESC pRTDesc;
	pPrimaryRenderTarget->GetDesc(&pRTDesc);

	return  ((pViewport->Height == pRTDesc.Height) && (pViewport->Width == pRTDesc.Width) &&
		(pViewport->MinZ <= SMALL_FLOAT) && (pViewport->MaxZ >= SLIGHTLY_LESS_THAN_ONE));
}

/**
* Stores and sets view transform calling SetTransform() accordingly to current render side.
* @param pLeftMatrix The left view matrix.
* @param pRightMatrix The right view matrix.
* @param apply True to apply calling SetTransform()
* @see actual->SetTransform()
***/
METHOD_IMPL( HRESULT , , D3DProxyDevice , SetStereoViewTransform , D3DXMATRIX , pLeftMatrix , D3DXMATRIX , pRightMatrix , bool , apply )
	if (D3DXMatrixIsIdentity(&pLeftMatrix) && D3DXMatrixIsIdentity(&pRightMatrix)) {
		m_bViewTransformSet = false;
	}
	else {
		m_bViewTransformSet = true;
	}

	m_leftView = pLeftMatrix;
	m_rightView = pRightMatrix;

	if (m_currentRenderingSide == vireio::Left) {
		m_pCurrentView = &m_leftView;
	}
	else {
		m_pCurrentView = &m_rightView;
	}

	if (apply)
		return actual->SetTransform(D3DTS_VIEW, m_pCurrentView);
	else
		return D3D_OK;
}

/**
* Stores and sets projection transform calling SetTransform() accordingly to current render side.
* @param pLeftMatrix The left view matrix.
* @param pRightMatrix The right view matrix.
* @param apply True to apply calling SetTransform()
* @see actual->SetTransform()
***/
METHOD_IMPL( HRESULT , , D3DProxyDevice , SetStereoProjectionTransform , D3DXMATRIX , pLeftMatrix , D3DXMATRIX , pRightMatrix , bool , apply )
	if (D3DXMatrixIsIdentity(&pLeftMatrix) && D3DXMatrixIsIdentity(&pRightMatrix)) {
		m_bProjectionTransformSet = false;
	}
	else {
		m_bProjectionTransformSet = true;
	}

	m_leftProjection = pLeftMatrix;
	m_rightProjection = pRightMatrix;

	if (m_currentRenderingSide == vireio::Left) {
		m_pCurrentProjection = &m_leftProjection;
	}
	else {
		m_pCurrentProjection = &m_rightProjection;
	}

	if (apply)
		return actual->SetTransform(D3DTS_PROJECTION, m_pCurrentProjection);
	else
		return D3D_OK;
}

/**
* Sets the viewport to squish the GUI accordingly.
***/
METHOD_IMPL( void , , D3DProxyDevice , SetGUIViewport )
	// do not squish the viewport in case brassa menu is open - GBCODE Why?
	//if ((BRASSA_mode>=BRASSA_Modes::MAINMENU) && (BRASSA_mode<BRASSA_Modes::BRASSA_ENUM_RANGE))
	//	return;

	D3DXMATRIX mLeftShift;
	D3DXMATRIX mRightShift;

	// set shift by current gui 3d depth
	float shiftInPixels = gui3DDepthPresets[gui3DDepthMode];
	D3DXMatrixTranslation(&mLeftShift, -shiftInPixels, 0, 0);
	D3DXMatrixTranslation(&mRightShift, shiftInPixels, 0, 0);

	// get matrix
	D3DXMATRIX mVPSquash = mLeftShift * m_spShaderViewAdjustment->Squash();
	if (m_currentRenderingSide != vireio::Left)
		mVPSquash = mRightShift * m_spShaderViewAdjustment->Squash();

	// get viewport
	actual->GetViewport(&m_ViewportIfSquished);

	// get screen center and translate it
	float centerX = (((FLOAT)stereoView->viewport.Width-(FLOAT)stereoView->viewport.X)/2.0f);
	float centerY = (((FLOAT)stereoView->viewport.Height-(FLOAT)stereoView->viewport.Y)/2.0f);

	// get left/top viewport sides
	D3DXVECTOR3 vIn = D3DXVECTOR3((FLOAT)stereoView->viewport.X-centerX, (FLOAT)stereoView->viewport.Y-centerY,1);
	D3DXVECTOR4 vOut = D3DXVECTOR4();
	D3DXVec3Transform(&vOut,&vIn, &mVPSquash);
	float floatMultiplier = 4;
	int originalX = (int)(vOut.x+centerX);
	int originalY = (int)(vOut.y+centerY);
	if(m_bfloatingMenu && (tracker->getStatus() >= MTS_OK))
	{
		/*char buf[64];
		LPCSTR psz = NULL;
		sprintf_s(buf, "yaw: %f, pitch: %f\n", tracker->primaryYaw, tracker->primaryPitch);
		psz = buf;*/		
		m_ViewportIfSquished.X = (int)(vOut.x+centerX-(((m_fFloatingYaw - tracker->primaryYaw) * floatMultiplier) * (180 / PI)));
		m_ViewportIfSquished.Y = (int)(vOut.y+centerY-(((m_fFloatingPitch - tracker->primaryPitch) * floatMultiplier) * (180 / PI)));
	}
	else
	{
		m_ViewportIfSquished.X = (int)(vOut.x+centerX);
		m_ViewportIfSquished.Y = (int)(vOut.y+centerY);
	}

	// get right/bottom viewport sides
	vIn = D3DXVECTOR3((FLOAT)(stereoView->viewport.Width+stereoView->viewport.X)-centerX, (FLOAT)(stereoView->viewport.Height+stereoView->viewport.Y)-centerY,1);
	vOut = D3DXVECTOR4();
	D3DXVec3Transform(&vOut,&vIn, &mVPSquash);
	m_ViewportIfSquished.Width = (int)(vOut.x+centerX) - originalX;
	m_ViewportIfSquished.Height = (int)(vOut.y+centerY) - originalY;

	// set viewport
	m_bViewportIsSquished = true;
	actual->SetViewport(&m_ViewportIfSquished);
}

/**
* Rounds the floats to make them more display friendly
**/
METHOD_IMPL( float , , D3DProxyDevice , RoundBrassaValue , float , val )
	return (float)floor(val * 1000.0f + 0.5f) / 1000.0f;
}


METHOD_IMPL( bool , , D3DProxyDevice , InitVRBoost )
	bool initSuccess = false;

	// explicit VRboost dll import
	hmVRboost = LoadLibraryA("VRboost.dll");

	VRBoostStatus.VRBoost_Active = false;
	VRBoostStatus.VRBoost_LoadRules = false;
	VRBoostStatus.VRBoost_ApplyRules = false;

	// get VRboost methods
	if (hmVRboost != NULL)
	{
		OutputDebugStringA("VR Boost Loaded\n");
		// get methods explicit
		m_pVRboost_LoadMemoryRules = (LPVRBOOST_LoadMemoryRules)GetProcAddress(hmVRboost, "VRboost_LoadMemoryRules");
		m_pVRboost_SaveMemoryRules = (LPVRBOOST_SaveMemoryRules)GetProcAddress(hmVRboost, "VRboost_SaveMemoryRules");
		m_pVRboost_CreateFloatMemoryRule = (LPVRBOOST_CreateFloatMemoryRule)GetProcAddress(hmVRboost, "VRboost_CreateFloatMemoryRule");
		m_pVRboost_SetProcess = (LPVRBOOST_SetProcess)GetProcAddress(hmVRboost, "VRboost_SetProcess");
		m_pVRboost_ReleaseAllMemoryRules = (LPVRBOOST_ReleaseAllMemoryRules)GetProcAddress(hmVRboost, "VRboost_ReleaseAllMemoryRules");
		m_pVRboost_ApplyMemoryRules = (LPVRBOOST_ApplyMemoryRules)GetProcAddress(hmVRboost, "VRboost_ApplyMemoryRules");
		if ((!m_pVRboost_LoadMemoryRules) || 
			(!m_pVRboost_SaveMemoryRules) || 
			(!m_pVRboost_CreateFloatMemoryRule) || 
			(!m_pVRboost_SetProcess) || 
			(!m_pVRboost_ReleaseAllMemoryRules) || 
			(!m_pVRboost_ApplyMemoryRules))
		{
			hmVRboost = NULL;
			m_bForceMouseEmulation = false;
			FreeLibrary(hmVRboost);
		}
		else
		{
			initSuccess = true;
			m_bForceMouseEmulation = true;
			VRBoostStatus.VRBoost_Active = true;
			OutputDebugStringA("Success loading VRboost methods.");
		}

		m_VRboostRulesPresent = false;
		m_VertexShaderCount = 0;
		m_VertexShaderCountLastFrame = 0;

		// set common default VRBoost values
		ZeroMemory(&VRBoostValue[0], MAX_VRBOOST_VALUES*sizeof(float));
		VRBoostValue[VRboostAxis::Zero] = 0.0f;
		VRBoostValue[VRboostAxis::One] = 1.0f;
		VRBoostValue[VRboostAxis::WorldFOV] = 95.0f;
		VRBoostValue[VRboostAxis::PlayerFOV] = 125.0f;
		VRBoostValue[VRboostAxis::FarPlaneFOV] = 95.0f;
	}
	else
	{
		initSuccess = false;
	}
	return initSuccess;
}

METHOD_IMPL( bool , , D3DProxyDevice , InitBrassa )
	hudFont = NULL;
	menuTime = (float)GetTickCount()/1000.0f;
	screenshot = (int)false;
	m_bForceMouseEmulation = false;
	m_bVRBoostToggle = true;
	m_bPosTrackingToggle = true;
	m_bShowVRMouse = false;
	m_fVRBoostIndicator = 0.0f;
	BRASSA_mode = BRASSA_Modes::INACTIVE;
	borderTopHeight = 0.0f;
	menuTopHeight = 0.0f;
	menuVelocity = D3DXVECTOR2(0.0f, 0.0f);
	menuAttraction = D3DXVECTOR2(0.0f, 0.0f);
	hud3DDepthMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	gui3DDepthMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	oldHudMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	oldGuiMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	hud3DDepthPresets[0] = 0.0f;
	hud3DDepthPresets[1] = 0.0f;
	hud3DDepthPresets[2] = 0.0f;
	hud3DDepthPresets[3] = 0.0f;
	hudDistancePresets[0] = 0.5f;
	hudDistancePresets[1] = 0.9f;
	hudDistancePresets[2] = 0.3f;
	hudDistancePresets[3] = 0.0f;
	gui3DDepthPresets[0] = 0.0f;
	gui3DDepthPresets[1] = 0.0f;
	gui3DDepthPresets[2] = 0.0f;
	gui3DDepthPresets[3] = 0.0f;
	guiSquishPresets[0] = 0.6f;
	guiSquishPresets[1] = 0.5f;
	guiSquishPresets[2] = 0.9f;
	guiSquishPresets[3] = 1.0f;
	ChangeHUD3DDepthMode(HUD_3D_Depth_Modes::HUD_DEFAULT);
	ChangeGUI3DDepthMode(GUI_3D_Depth_Modes::GUI_DEFAULT);

	hotkeyCatch = false;
	toggleVRBoostHotkey = 0;
	for (int i = 0; i < 5; i++)
	{
		guiHotkeys[i] = 0;
		hudHotkeys[i] = 0;
	}
	for (int i = 0; i < 16; i++)
		controls.xButtonsStatus[i] = false;
	
	
	return true;
}

/*
  * Initializes the tracker, setting the tracker initialized status.
  * @return true if tracker was initialized, false otherwise
  */
METHOD_IMPL( bool , , D3DProxyDevice , InitTracker )
 	// VRboost rules present ?
 	if (config.VRboostPath != "") m_VRboostRulesPresent = true; else m_VRboostRulesPresent = false;
 
 	OutputDebugStringA("GB - Try to init Tracker\n");
	tracker.reset(MotionTrackerFactory::Get(config));
	if (tracker && tracker->getStatus() >= MTS_OK)
 	{
		OutputDebugStringA("Tracker Got\n");
 		OutputDebugStringA("Setting Multipliers\n");
		tracker->setMultipliers(config.yaw_multiplier, config.pitch_multiplier, config.roll_multiplier);
 		OutputDebugStringA("Setting Mouse EMu\n");
		tracker->setMouseEmulation((!m_VRboostRulesPresent) || (hmVRboost==NULL));

		//Only advise calibration for positional tracking on DK2
		if( config.stereoMode.contains("DK2") ){
			calibrate_tracker = true;
			//TODO: add new tracker interface
		}

		return true;
 	}

 	return false;
 }










  /**
* Here the chosen stereoviews draw function is called to render to wrapped back buffer.
* All other final screen output is also done here.
***/
void D3DProxyDevice::ProxyPresent(){
 	IDirect3DSurface9* pWrappedBackBuffer;

	try {
		m_activeSwapChains.at(0)->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pWrappedBackBuffer);

		if (stereoView->initialized)
			stereoView->Draw(static_cast<D3D9ProxySurface*>(pWrappedBackBuffer));

		pWrappedBackBuffer->Release();
	}
	catch (std::out_of_range) {
		OutputDebugStringA("Present: No primary swap chain found. (Present probably called before device has been reset)");
	}

	// did set this now also in proxy swap chain ? solved ?
	// (this can break if device present is followed by present on another swap chain... or not work well anyway)
	m_isFirstBeginSceneOfFrame = true; 

	BRASSA_UpdateBorder();

	//Now calculate frames per second
	fps = CalcFPS();
 }


 






/**
* Creates a proxy (or wrapped) depth stencil surface (D3D9ProxySurface).
* Surface to be created only gets both stereo surfaces if game handler agrees.
* @see D3D9ProxySurface
* @see GameHandler::ShouldDuplicateDepthStencilSurface() 
***/
METHOD_IMPL( HRESULT , , D3DProxyDevice , ProxyCreateDepthStencilSurface , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample , DWORD , MultisampleQuality , BOOL , Discard , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage , bool , useEx )
	IDirect3DSurface9* left  = 0;
	IDirect3DSurface9* right = 0;
	HRESULT            resultLeft;
	HRESULT            resultRight;

	if( useEx ){
		resultLeft = actualEx->CreateDepthStencilSurfaceEx( Width, Height, Format, MultiSample, MultisampleQuality, Discard, &left, pSharedHandle , Usage );
	}else{
		resultLeft = actual  ->CreateDepthStencilSurface  ( Width, Height, Format, MultiSample, MultisampleQuality, Discard, &left, pSharedHandle);
	}
	
	if( SUCCEEDED(resultLeft) ){

		// TODO Should we always duplicated Depth stencils? I think yes, but there may be exceptions
		if (m_pGameHandler->ShouldDuplicateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard)) 
		{
			if( useEx ){
				resultRight = actualEx->CreateDepthStencilSurfaceEx( Width, Height, Format, MultiSample, MultisampleQuality, Discard, &right, pSharedHandle , Usage );
			}else{
				resultRight = actual  ->CreateDepthStencilSurface  ( Width, Height, Format, MultiSample, MultisampleQuality, Discard, &right, pSharedHandle);
			}

			if( FAILED(resultRight) ){
				OutputDebugStringA("Failed to create right eye Depth Stencil Surface while attempting to create stereo pair, falling back to mono\n");
				right = 0;
			}
		}

		*ppSurface = new D3D9ProxySurface(left, right, this, NULL);
	} else {
		OutputDebugStringA("Failed to create Depth Stencil Surface\n"); 
	}

	return resultLeft;
}



/**
* Creates proxy (wrapped) render target, if swapchain buffer returns StereoBackBuffer, otherwise D3D9ProxySurface.
* Duplicates render target if game handler agrees.
* @see GameHandler::ShouldDuplicateRenderTarget()
* @see StereoBackBuffer
* @see D3D9ProxySurface
***/
METHOD_IMPL( HRESULT , , D3DProxyDevice , ProxyCreateRenderTarget , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample ,
												  DWORD , MultisampleQuality , BOOL , Lockable , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage , bool , isSwapChainBackBuffer , bool , useEx )
	IDirect3DSurface9* left  = 0;
	IDirect3DSurface9* right = 0;
	HRESULT resultLeft;
	HRESULT resultRight;

	// create left/mono

	if( useEx ){
		resultLeft = actualEx->CreateRenderTargetEx(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &left, pSharedHandle , Usage );
	}else{
		resultLeft = actual->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &left, pSharedHandle);
	}

	if( SUCCEEDED(resultLeft) ){

		/* "If Needed" heuristic is the complicated part here.
		Fixed heuristics (based on type, format, size, etc) + game specific overrides + isForcedMono + magic? */
		// TODO Should we duplicate this Render Target? Replace "true" with heuristic
		if (m_pGameHandler->ShouldDuplicateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, isSwapChainBackBuffer))
		{
			if( useEx ){
				resultRight = actualEx->CreateRenderTargetEx(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &right, pSharedHandle , Usage );
			}else{
				resultRight = actual->CreateRenderTarget  (Width, Height, Format, MultiSample, MultisampleQuality, Lockable, &right, pSharedHandle);
			}

			if( FAILED(resultRight) ){
				OutputDebugStringA("Failed to create right eye render target while attempting to create stereo pair, falling back to mono\n");
				right = NULL;
			}
		}

		if (!isSwapChainBackBuffer){
			*ppSurface = new D3D9ProxySurface(left, right, this, NULL);
		}else{
			*ppSurface = new StereoBackBuffer(left, right, this);
		}
	}
	else {
		OutputDebugStringA("Failed to create render target\n"); 
	}


	return resultLeft;
}



//Creates a wrapped mono surface with only one (left) side.
//OffscreenPlainSurfaces doesn't need to be Stereo. 
//They can't be used as render targets and they can't have rendertargets copied to them with stretch
//rect, so don't need to be stereo capable.
//See table at bottom of 
//<http://msdn.microsoft.com/en-us/library/windows/desktop/bb174471%28v=vs.85%29.aspx> 
//for stretch rect restrictions.
METHOD_IMPL( HRESULT , , D3DProxyDevice , ProxyCreateOffscreenPlainSurface , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage , bool , useEx )
	IDirect3DSurface9* surface = 0;
	HRESULT result;
	
	if( useEx ){
		result = actualEx->CreateOffscreenPlainSurfaceEx(Width, Height, Format, Pool, &surface, pSharedHandle , Usage );
	}else{
		result = actual->CreateOffscreenPlainSurface(Width, Height, Format, Pool, &surface, pSharedHandle );
	}

	if (SUCCEEDED(result)){
		*ppSurface = new D3D9ProxySurface(surface, NULL, this, NULL);
	}

	return result;
}


/**
* Calls release functions here and in stereo view class, releases swap chains and restores everything.
* Subclasses which override this method must call through to super method at the end of the subclasses
* implementation.
* @see ReleaseEverything()
* @see StereoView::ReleaseEverything()
* @see OnCreateOrRestore()
***/
METHOD_IMPL( HRESULT , , D3DProxyDevice , ProxyReset , D3DPRESENT_PARAMETERS* , pPresentationParameters , D3DDISPLAYMODEEX* , pFullscreenDisplayMode , bool , useEx )
	if(stereoView)
		stereoView->ReleaseEverything();

	ReleaseEverything();

	m_bInBeginEndStateBlock = false;

	auto it = m_activeSwapChains.begin();
	while (it != m_activeSwapChains.end()) {

		if ((*it) != NULL)
			(*it)->Release();

		delete (*it);

		it = m_activeSwapChains.erase(it);
	}

	HRESULT hr;
	if( useEx ){
		hr = actualEx->ResetEx(pPresentationParameters,pFullscreenDisplayMode);
	}else{
		hr = actual->Reset(pPresentationParameters);
	}

	// if the device has been successfully reset we need to recreate any resources we created
	if (hr == D3D_OK)  {
		OnCreateOrRestore();
		stereoView->PostReset();
	}
	else {
#ifdef _DEBUG
		char buf[256];
		sprintf_s(buf, "Error: %s error description: %s\n",
			DXGetErrorString(hr), DXGetErrorDescription(hr));

		OutputDebugStringA(buf);				
#endif
		OutputDebugStringA("Device reset failed");
	}

	return hr;
}







// IDirect3DDevice9 / IDirect3DDevice9Ex similar methods

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , Present , CONST RECT* , pSourceRect , CONST RECT* , pDestRect , HWND , hDestWindowOverride , CONST RGNDATA* , pDirtyRegion )
	ProxyPresent();
	return actual->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , PresentEx , CONST RECT* , pSourceRect , CONST RECT* , pDestRect , HWND , hDestWindowOverride , CONST RGNDATA* , pDirtyRegion , DWORD , dwFlags )
	ProxyPresent();
	return actualEx->PresentEx(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion , dwFlags );
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateOffscreenPlainSurface , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle )	
	return ProxyCreateOffscreenPlainSurface( Width, Height, Format, Pool, ppSurface , pSharedHandle , 0 , false );
}

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateOffscreenPlainSurfaceEx , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DPOOL , Pool , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage )	
	return ProxyCreateOffscreenPlainSurface( Width, Height, Format, Pool, ppSurface , pSharedHandle , Usage , true );
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateRenderTarget , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample , DWORD , MultisampleQuality , BOOL , Lockable , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle )
	//The IDirect3DSurface9** ppSurface returned should always be a D3D9ProxySurface. Any class overloading
	//this method should ensure that this remains true.
	return ProxyCreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, 0 , false , false );
}

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateRenderTargetEx , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample , DWORD , MultisampleQuality , BOOL , Lockable , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage )
	return ProxyCreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, 0 , false , true );
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateDepthStencilSurface , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample , DWORD , MultisampleQuality , BOOL , Discard , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle )
	return ProxyCreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface , pSharedHandle , 0  , false );
}

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , CreateDepthStencilSurfaceEx , UINT , Width , UINT , Height , D3DFORMAT , Format , D3DMULTISAMPLE_TYPE , MultiSample , DWORD , MultisampleQuality , BOOL , Discard , IDirect3DSurface9** , ppSurface , HANDLE* , pSharedHandle , DWORD , Usage )
	return ProxyCreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface , pSharedHandle , Usage , true );
}


METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , Reset , D3DPRESENT_PARAMETERS* , pPresentationParameters )
	return ProxyReset( pPresentationParameters , 0 , false );
}

METHOD_IMPL( HRESULT , WINAPI , D3DProxyDevice , ResetEx , D3DPRESENT_PARAMETERS* , pPresentationParameters , D3DDISPLAYMODEEX* , pFullscreenDisplayMode )
	return ProxyReset( pPresentationParameters , pFullscreenDisplayMode , true );
}




// IDirect3DDevice9Ex base methods. are there any proxy required for those?
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , GetDisplayModeEx , UINT , iSwapChain , D3DDISPLAYMODEEX* , pMode , D3DDISPLAYROTATION* , pRotation )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , SetConvolutionMonoKernel , UINT , width , UINT , height , float* , rows , float* , columns )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , ComposeRects , IDirect3DSurface9* , pSrc , IDirect3DSurface9* , pDst , IDirect3DVertexBuffer9* , pSrcRectDescs , UINT , NumRects , IDirect3DVertexBuffer9* , pDstRectDescs , D3DCOMPOSERECTSOP , Operation , int , Xoffset , int , Yoffset )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , GetGPUThreadPriority , INT* , pPriority )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , SetGPUThreadPriority , INT , Priority )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , WaitForVBlank , UINT , iSwapChain )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , CheckResourceResidency , IDirect3DResource9** , pResourceArray , UINT32 , NumResources )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , SetMaximumFrameLatency , UINT , MaxLatency )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , GetMaximumFrameLatency , UINT* , pMaxLatency )
METHOD_THRU_EX( HRESULT , WINAPI , D3DProxyDevice , CheckDeviceState , HWND , hDestinationWindow )



// IDirect3DDevice9 base methods
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , TestCooperativeLevel ) // The calling application will start releasing resources after TestCooperativeLevel returns D3DERR_DEVICENOTRESET.
METHOD_THRU( UINT    , WINAPI , D3DProxyDevice , GetAvailableTextureMem )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , EvictManagedResources )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetDeviceCaps , D3DCAPS9* , pCaps )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetDisplayMode , UINT , iSwapChain , D3DDISPLAYMODE* , pMode )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetCreationParameters , D3DDEVICE_CREATION_PARAMETERS* , pParameters )
METHOD_THRU( void    , WINAPI , D3DProxyDevice , SetCursorPosition , int , X , int , Y , DWORD , Flags )
METHOD_THRU( BOOL    , WINAPI , D3DProxyDevice , ShowCursor , BOOL , bShow )
METHOD_THRU( UINT    , WINAPI , D3DProxyDevice , GetNumberOfSwapChains )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetRasterStatus , UINT , iSwapChain , D3DRASTER_STATUS* , pRasterStatus )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetDialogBoxMode , BOOL , bEnableDialogs )
METHOD_THRU( void    , WINAPI , D3DProxyDevice , SetGammaRamp     , UINT , iSwapChain , DWORD , Flags , CONST D3DGAMMARAMP* , pRamp )
METHOD_THRU( void    , WINAPI , D3DProxyDevice , GetGammaRamp     , UINT , iSwapChain , D3DGAMMARAMP* , pRamp )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetTransform , D3DTRANSFORMSTATETYPE , State , D3DMATRIX* , pMatrix )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , MultiplyTransform , D3DTRANSFORMSTATETYPE , State , CONST D3DMATRIX* , pMatrix );
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetViewport , D3DVIEWPORT9* , pViewport )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetMaterial , CONST D3DMATERIAL9* , pMaterial )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetMaterial , D3DMATERIAL9* , pMaterial )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetLight , DWORD , Index , CONST D3DLIGHT9* , pLight )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetLight , DWORD , Index , D3DLIGHT9* , pLight )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , LightEnable , DWORD , Index , BOOL , Enable )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetLightEnable , DWORD , Index , BOOL* , pEnable )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetClipPlane , DWORD , Index , CONST float* , pPlane )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetClipPlane , DWORD , Index , float* , pPlane )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetRenderState , D3DRENDERSTATETYPE , State , DWORD , Value )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetRenderState , D3DRENDERSTATETYPE , State , DWORD* , pValue )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetClipStatus , CONST D3DCLIPSTATUS9* , pClipStatus )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetClipStatus , D3DCLIPSTATUS9* , pClipStatus )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetTextureStageState , DWORD , Stage , D3DTEXTURESTAGESTATETYPE , Type , DWORD* , pValue )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetTextureStageState , DWORD , Stage , D3DTEXTURESTAGESTATETYPE , Type , DWORD  , Value )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetSamplerState , DWORD , Sampler , D3DSAMPLERSTATETYPE , Type , DWORD* , pValue)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetSamplerState,DWORD ,Sampler,D3DSAMPLERSTATETYPE ,Type,DWORD ,Value)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , ValidateDevice,DWORD* ,pNumPasses)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetPaletteEntries,UINT ,PaletteNumber,CONST PALETTEENTRY*, pEntries)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetPaletteEntries,UINT ,PaletteNumber,PALETTEENTRY* ,pEntries)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetCurrentTexturePalette,UINT, PaletteNumber)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetCurrentTexturePalette,UINT*,PaletteNumber)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetScissorRect,CONST RECT*, pRect)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetScissorRect,RECT*, pRect)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetSoftwareVertexProcessing,BOOL, bSoftware)
METHOD_THRU( BOOL    , WINAPI , D3DProxyDevice , GetSoftwareVertexProcessing)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetNPatchMode,float, nSegments)
METHOD_THRU( float   , WINAPI , D3DProxyDevice , GetNPatchMode)
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetFVF , DWORD , FVF )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetFVF , DWORD* , pFVF )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetVertexShaderConstantI , UINT , StartRegister , CONST int*  , pConstantData , UINT , Vector4iCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetVertexShaderConstantI , UINT , StartRegister , int*        , pConstantData , UINT , Vector4iCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetVertexShaderConstantB , UINT , StartRegister , CONST BOOL* , pConstantData , UINT , BoolCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetVertexShaderConstantB , UINT , StartRegister , BOOL*       , pConstantData , UINT , BoolCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetStreamSourceFreq , UINT , StreamNumber , UINT  , Setting )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetStreamSourceFreq , UINT , StreamNumber , UINT* , pSetting )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetPixelShaderConstantI , UINT , StartRegister , CONST int* , pConstantData , UINT , Vector4iCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetPixelShaderConstantI , UINT , StartRegister , int*       , pConstantData , UINT , Vector4iCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , SetPixelShaderConstantB , UINT , StartRegister , CONST BOOL*, pConstantData , UINT , BoolCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , GetPixelShaderConstantB , UINT , StartRegister , BOOL*      , pConstantData , UINT , BoolCount )
METHOD_THRU( HRESULT , WINAPI , D3DProxyDevice , DeletePatch , UINT , Handle )

