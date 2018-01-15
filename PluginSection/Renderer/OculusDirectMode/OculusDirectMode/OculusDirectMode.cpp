/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift DirectMode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.cpp> and
Class <OculusDirectMode> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from the Oculus Rift sample
>OculusRoomTiny< (Author Tom Heath - Copyright :
Copyright 2015 Oculus, Inc. All Rights reserved.
http://www.apache.org/licenses/LICENSE-2.0)
and from the Vireio source code originally authored by Simon Brown.
(class OculusDirectToRiftView v3.0.0 2015).

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include"OculusDirectMode.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

#define INTERFACE_IDIRECT3DDEVICE9           8
#define INTERFACE_IDIRECT3DSWAPCHAIN9       15
#define INTERFACE_IDXGISWAPCHAIN            29

#define	METHOD_IDIRECT3DDEVICE9_PRESENT     17
#define	METHOD_IDIRECT3DDEVICE9_ENDSCENE    42
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT   3
#define METHOD_IDXGISWAPCHAIN_PRESENT        8

#define AVATAR_MESH_LEFT_TOUCH 0x557a26331850dbf
#define AVATAR_MESH_RIGHT_HAND 0x6a4ae11446026286
#define AVATAR_MESH_RIGHT_TOUCH 0x6feb9283b780b5a3
#define AVATAR_MESH_CHEST 0x7f1ca835aeb1b69e
#define AVATAR_MESH_RING 0x8e78d539875b1886
#define AVATAR_MESH_HEAD 0xaf8f894a6872ef92
#define AVATAR_MESH_HMD 0xc9d9e6ab35efc3a0
#define AVATAR_MESH_THROAT 0xe986b9ff5f964dad
#define AVATAR_MESH_LEFT_HAND 0xf82847a6b3ddf1a6

/**
* Little Oculus matrix helper.
***/
inline void D3DMatrixFromOvrAvatarTransform(const ovrAvatarTransform& sTransform, D3DXMATRIX* psTarget) {
	D3DXMATRIX sTranslate, sOrientation, sScale;
	D3DXMatrixTranslation(&sTranslate, sTransform.position.x, sTransform.position.y, sTransform.position.z);
	D3DXMatrixScaling(&sScale, sTransform.scale.x, sTransform.scale.y, sTransform.scale.z);

	D3DXQUATERNION sOrientationQ = D3DXVECTOR4(sTransform.orientation.x, sTransform.orientation.y, sTransform.orientation.z, sTransform.orientation.w);
	D3DXMatrixRotationQuaternion(&sOrientation, &sOrientationQ);

	*psTarget = sScale * sOrientation * sTranslate;
}
/**
* Little Oculus matrix helper.
***/
inline void D3DMatrixFromOvrAvatarTransform(const ovrAvatarTransform& sTransform, XMMATRIX* psTarget) {
	XMMATRIX sTranslate, sOrientation, sScale;
	XMVECTOR sOrientationQ = XMVectorSet(sTransform.orientation.x, sTransform.orientation.y, sTransform.orientation.z, sTransform.orientation.w);
	sTranslate = XMMatrixTranslation(sTransform.position.x, sTransform.position.y, sTransform.position.z);
	sOrientation = XMMatrixRotationQuaternion(sOrientationQ);
	sScale = XMMatrixScaling(sTransform.scale.x, sTransform.scale.y, sTransform.scale.z);
	*psTarget = sScale * sOrientation * sTranslate;
}
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
/**
* Method from mirror sample.
***/
static void AvatarHandInputStateFromOvr(const ovrAvatarTransform& transform, const ovrInputState& inputState, ovrHandType hand, ovrAvatarHandInputState* state)
{
	state->transform = transform;
	state->buttonMask = 0;
	state->touchMask = 0;
	state->joystickX = inputState.Thumbstick[hand].x;
	state->joystickY = inputState.Thumbstick[hand].y;
	state->indexTrigger = inputState.IndexTrigger[hand];
	state->handTrigger = inputState.HandTrigger[hand];
	state->isActive = false;
	if (hand == ovrHand_Left)
	{
		if (inputState.Buttons & ovrButton_X) state->buttonMask |= ovrAvatarButton_One;
		if (inputState.Buttons & ovrButton_Y) state->buttonMask |= ovrAvatarButton_Two;
		if (inputState.Buttons & ovrButton_Enter) state->buttonMask |= ovrAvatarButton_Three;
		if (inputState.Buttons & ovrButton_LThumb) state->buttonMask |= ovrAvatarButton_Joystick;
		if (inputState.Touches & ovrTouch_X) state->touchMask |= ovrAvatarTouch_One;
		if (inputState.Touches & ovrTouch_Y) state->touchMask |= ovrAvatarTouch_Two;
		if (inputState.Touches & ovrTouch_LThumb) state->touchMask |= ovrAvatarTouch_Joystick;
		if (inputState.Touches & ovrTouch_LThumbRest) state->touchMask |= ovrAvatarTouch_ThumbRest;
		if (inputState.Touches & ovrTouch_LIndexTrigger) state->touchMask |= ovrAvatarTouch_Index;
		if (inputState.Touches & ovrTouch_LIndexPointing) state->touchMask |= ovrAvatarTouch_Pointing;
		if (inputState.Touches & ovrTouch_LThumbUp) state->touchMask |= ovrAvatarTouch_ThumbUp;
		state->isActive = (inputState.ControllerType & ovrControllerType_LTouch) != 0;
	}
	else if (hand == ovrHand_Right)
	{
		if (inputState.Buttons & ovrButton_A) state->buttonMask |= ovrAvatarButton_One;
		if (inputState.Buttons & ovrButton_B) state->buttonMask |= ovrAvatarButton_Two;
		if (inputState.Buttons & ovrButton_Home) state->buttonMask |= ovrAvatarButton_Three;
		if (inputState.Buttons & ovrButton_RThumb) state->buttonMask |= ovrAvatarButton_Joystick;
		if (inputState.Touches & ovrTouch_A) state->touchMask |= ovrAvatarTouch_One;
		if (inputState.Touches & ovrTouch_B) state->touchMask |= ovrAvatarTouch_Two;
		if (inputState.Touches & ovrTouch_RThumb) state->touchMask |= ovrAvatarTouch_Joystick;
		if (inputState.Touches & ovrTouch_RThumbRest) state->touchMask |= ovrAvatarTouch_ThumbRest;
		if (inputState.Touches & ovrTouch_RIndexTrigger) state->touchMask |= ovrAvatarTouch_Index;
		if (inputState.Touches & ovrTouch_RIndexPointing) state->touchMask |= ovrAvatarTouch_Pointing;
		if (inputState.Touches & ovrTouch_RThumbUp) state->touchMask |= ovrAvatarTouch_ThumbUp;
		state->isActive = (inputState.ControllerType & ovrControllerType_RTouch) != 0;
	}
}
#endif

/**
* Constructor.
***/
OculusDirectMode::OculusDirectMode() : AQU_Nodus(),
m_pcDeviceTemporary(nullptr),
m_pcContextTemporary(nullptr),
m_pcSwapChainTemporary(nullptr),
m_pcBackBufferTemporary(nullptr),
m_bInit(false),
m_pcMirrorTexture(nullptr),
m_pcMirrorTextureD3D11(nullptr),
m_pcMirrorTextureD3D11HMD(nullptr),
m_hHMD(nullptr),
m_phHMD(nullptr),
m_phHMD_Tracker(nullptr),
m_bHotkeySwitch(false),
m_pbZoomOut(nullptr),
m_ppcTexViewHud11(nullptr),
m_pcTex11CopyHUD(nullptr),
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
m_psAvatar(nullptr),
#endif
m_nLoadingAssets(0),
m_pcVSAvatar(nullptr),
m_pcPSAvatar(nullptr),
m_pcILAvatar(nullptr),
m_pcCVSAvatar(nullptr),
m_pcCPSAvatar(nullptr),
m_pcDSStateLess(nullptr),
m_pcDSStateEqual(nullptr),
m_pcRS(nullptr),
m_pcRS_RightHand(nullptr)
{
#ifdef _WIN64
	// add a zero asset as zero index.. only for 64bit
	m_asAssetMap.push_back(AvatarData());
#endif

	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;
	m_pcTex11Copy[0] = nullptr;
	m_pcTex11Copy[1] = nullptr;

	m_pcFrameTexture[0] = nullptr;
	m_pcFrameTexture[1] = nullptr;
	m_pcFrameTextureSRView[0] = nullptr;
	m_pcFrameTextureSRView[1] = nullptr;

	m_pcVertexShader11 = nullptr;
	m_pcPixelShader11 = nullptr;
	m_pcVertexLayout11 = nullptr;
	m_pcVertexBuffer11 = nullptr;
	m_pcConstantBufferDirect11 = nullptr;
	m_pcSamplerState = nullptr;

	m_pcDSGeometry11[0] = nullptr;
	m_pcDSGeometry11[1] = nullptr;
	m_pcDSVGeometry11[0] = nullptr;
	m_pcDSVGeometry11[1] = nullptr;

	m_bInit = false;
	m_phHMD = nullptr;
	sensorSampleTime = 0.0;
	m_bShowMirror = false;

	m_psEyeRenderTexture[0] = nullptr;
	m_psEyeRenderTexture[1] = nullptr;
	m_psEyeRenderTextureHUD = nullptr;

	m_pbZoomOut = nullptr;

	m_ePerfHudMode = ovrPerfHudMode::ovrPerfHud_Off;

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	DWORD bShowMirror = 0;
	bShowMirror = GetIniFileSetting(bShowMirror, "LibOVR", "bShowMirror", szFilePathINI, bFileExists);
	if (bShowMirror) m_bShowMirror = true; else m_bShowMirror = false;
	m_ePerfHudMode = (ovrPerfHudMode)GetIniFileSetting((DWORD)m_ePerfHudMode, "LibOVR", "ePerfHudMode", szFilePathINI, bFileExists);

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Oculus Direct Mode";

	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Show Mirror (D3D11)";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_bShowMirror;
		sEntry.bValue = m_bShowMirror;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		static unsigned s_uDummy = 0;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "OVR PerfHud";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = 6;
		sEntry.unChangeSize = 1;
		sEntry.bValueEnumeration = true;
		{ std::string strEnum = "Off"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "PerfSummary"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "LatencyTiming"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "AppRenderTiming"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "CompRenderTiming"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "VersionInfo"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "AswStats"; sEntry.astrValueEnumeration.push_back(strEnum); }
		sEntry.punValue = &s_uDummy;
		sEntry.unValue = m_ePerfHudMode;
		m_sMenu.asEntries.push_back(sEntry);
	}
}

/**
* Destructor.
***/
OculusDirectMode::~OculusDirectMode()
{
	// release all avatar stuff... first two members of the asset union are IUnknown interfaces
	for (size_t uIx = 0; uIx < m_asAssetMap.size(); uIx++)
	{
		SAFE_RELEASE(m_asAssetMap[uIx].sMesh.pcElementBuffer);
		SAFE_RELEASE(m_asAssetMap[uIx].sMesh.pcVertexBuffer);
	}

	SAFE_RELEASE(m_pcRS_RightHand);
	SAFE_RELEASE(m_pcRS);
	SAFE_RELEASE(m_pcVSAvatar);
	SAFE_RELEASE(m_pcPSAvatar);
	SAFE_RELEASE(m_pcILAvatar);
	SAFE_RELEASE(m_pcCVSAvatar);
	SAFE_RELEASE(m_pcCPSAvatar);

	SAFE_RELEASE(m_pcDSStateLess);
	SAFE_RELEASE(m_pcDSStateEqual);

	SAFE_RELEASE(m_pcDSVGeometry11[0]);
	SAFE_RELEASE(m_pcDSVGeometry11[1]);
	SAFE_RELEASE(m_pcDSGeometry11[0]);
	SAFE_RELEASE(m_pcDSGeometry11[1]);

	SAFE_RELEASE(m_pcMirrorTextureD3D11);
	SAFE_RELEASE(m_pcMirrorTextureD3D11HMD);
	if (m_pcMirrorTexture) ovr_DestroyMirrorTexture(m_hHMD, m_pcMirrorTexture);
	SAFE_RELEASE(m_pcBackBufferRTVTemporary);
	SAFE_RELEASE(m_pcBackBufferTemporary);
	SAFE_RELEASE(m_pcSwapChainTemporary);
	SAFE_RELEASE(m_pcContextTemporary);
	SAFE_RELEASE(m_pcDeviceTemporary);

	SAFE_RELEASE(m_pcVertexShader11);
	SAFE_RELEASE(m_pcPixelShader11);
	ovr_Destroy(m_hHMD);
}

/**
* Return the name of the  Oculus Renderer node.
***/
const char* OculusDirectMode::GetNodeType()
{
	return "Oculus Direct Mode";
}

/**
* Returns a global unique identifier for the Oculus Renderer node.
***/
UINT OculusDirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 259
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Oculus Renderer node.
***/
LPWSTR OculusDirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the Oculus Renderer node.
***/
HBITMAP OculusDirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OculusDirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Oculus Renderer node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OculusDirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR OculusDirectMode::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((ODM_Commanders)dwCommanderIndex)
	{
		case VireioMenu:
			return L"Vireio Menu";
	}

	return L"XXX";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OculusDirectMode::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture11:
			return L"Left Texture";
		case ODM_Decommanders::RightTexture11:
			return L"Right Texture";
		case ODM_Decommanders::HMD_Handle:
			return L"HMD Handle";
		case ODM_Decommanders::ZoomOut:
			return L"Zoom Out";
		case HUDTexture11:
			return L"HUD Texture DX11";
		case HUDTexture10:
			break;
		case HUDTexture9:
			break;
	}

	return L"x";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD OculusDirectMode::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((ODM_Commanders)dwCommanderIndex)
	{
		case VireioMenu:
			return NOD_Plugtype::AQU_VOID;
	}

	return 0;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OculusDirectMode::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case ODM_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case ODM_Decommanders::HMD_Handle:
			return NOD_Plugtype::AQU_HANDLE;
		case ODM_Decommanders::ZoomOut:
			return NOD_Plugtype::AQU_BOOL;
		case HUDTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case HUDTexture10:
			break;
		case HUDTexture9:
			break;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* OculusDirectMode::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((ODM_Commanders)dwCommanderIndex)
	{
		case ODM_Commanders::VireioMenu:
			return (void*)&m_sMenu;
		default:
			break;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OculusDirectMode::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((ODM_Decommanders)dwDecommanderIndex)
	{
		case ODM_Decommanders::LeftTexture11:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case ODM_Decommanders::RightTexture11:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case ODM_Decommanders::HMD_Handle:
			m_phHMD_Tracker = (ovrSession*)pData;
			break;
		case ODM_Decommanders::ZoomOut:
			m_pbZoomOut = (BOOL*)pData;
			break;
		case HUDTexture11:
			m_ppcTexViewHud11 = (ID3D11ShaderResourceView**)pData;
			break;
		case HUDTexture10:
			break;
		case HUDTexture9:
			break;
	}
}

/**
* DirectMode supports IDXGISwapChain->Present().
***/
bool OculusDirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	// TODO !! ->Present() only
	return true;
}

/**
* Render the Oculus Rift View.
***/
void* OculusDirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT)) return nullptr;
	if ((eD3DInterface == INTERFACE_IDIRECT3DDEVICE9) && (eD3DMethod != METHOD_IDIRECT3DDEVICE9_PRESENT)) return nullptr;
	if ((eD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9) && (eD3DMethod != METHOD_IDIRECT3DSWAPCHAIN9_PRESENT)) return nullptr;

	// always skip more frames than the oculus tracker to ensure initialization is done
	static UINT unFrameSkip = 200 + 100;
	if (unFrameSkip > 0)
	{
		unFrameSkip--;
		return nullptr;
	}

	// #define _DEBUG_ODM
#ifdef _DEBUG_ODM
	{ wchar_t buf[128]; wsprintf(buf, L"[ODM] ifc %u mtd %u", eD3DInterface, eD3DMethod); OutputDebugString(buf); }
#endif

	// save ini file ?
	if (m_nIniFrameCount)
	{
		if (m_nIniFrameCount == 1)
		{
			// save ini settings.. get file path
			char szFilePathINI[1024];
			GetCurrentDirectoryA(1024, szFilePathINI);
			strcat_s(szFilePathINI, "\\VireioPerception.ini");
			bool bFileExists = false;
			DWORD bShowMirror = m_bShowMirror;
			bShowMirror = GetIniFileSetting(bShowMirror, "LibOVR", "bShowMirror", szFilePathINI, bFileExists);
			m_ePerfHudMode = (ovrPerfHudMode)GetIniFileSetting((DWORD)m_ePerfHudMode, "LibOVR", "ePerfHudMode", szFilePathINI, bFileExists);
		}
		m_nIniFrameCount--;
	}

	// main menu update ?
	if (m_sMenu.bOnChanged)
	{
		// set back event bool, set ini file frame count
		m_sMenu.bOnChanged = false;
		m_nIniFrameCount = 300;

		// loop through entries
		for (size_t nIx = 0; nIx < m_sMenu.asEntries.size(); nIx++)
		{
			// entry index changed ?
			if (m_sMenu.asEntries[nIx].bOnChanged)
			{
				m_sMenu.asEntries[nIx].bOnChanged = false;

				// performance hud ?
				if (nIx = 1)
				{
					// set chosen performance hud option
					m_ePerfHudMode = (ovrPerfHudMode)m_sMenu.asEntries[nIx].unValue;
					ovr_SetInt(*m_phHMD, OVR_PERF_HUD_MODE, (int)m_ePerfHudMode);
				}
			}
		}
	}

	// get device
	ID3D11Device* pcDevice = nullptr;
	IDXGISwapChain* pcSwapChain = nullptr;
	switch (eD3DInterface)
	{
		case INTERFACE_IDXGISWAPCHAIN:
			if (pThis)
			{
				// cast swapchain
				pcSwapChain = (IDXGISwapChain*)pThis;

				// and get the device from the swapchain
				pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
			}
			else
			{
				OutputDebugString(L"[OVR] : No swapchain !");
				return nullptr;
			}
			break;
		case INTERFACE_IDIRECT3DDEVICE9:
		case INTERFACE_IDIRECT3DSWAPCHAIN9:
			// get the d3d11 device from the connected tex view
			if (m_ppcTexView11[0])
			{
				if (*(m_ppcTexView11[0]))
					(*(m_ppcTexView11[0]))->GetDevice(&pcDevice);
				// else return nullptr;
			}
			else OutputDebugString(L"[OVR] : Connect d3d 11 texture views to obtain d3d 11 device.");
			break;
	}

	if (!pcDevice)
	{
		OutputDebugString(L"[OVR] : No d3d 11 device !");
		return nullptr;
	}


	// get context
	ID3D11DeviceContext* pcContext = nullptr;
	pcDevice->GetImmediateContext(&pcContext);
	if (!pcContext)
	{
		OutputDebugString(L"[OVR] No device context !");
		return nullptr;
	}

	if (!m_bInit)
	{
#pragma region Init
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
		// Initialize the platform module
		if (ovr_PlatformInitializeWindows(OVR_SAMPLE_APP_ID) != ovrPlatformInitialize_Success)
		{
			MessageBox(NULL, L"[OVR] Startup error", L"Failed to initialize the Oculus platform", NULL);
			exit(99);
		}
#endif
		if (!m_phHMD)
		{
			if (m_phHMD_Tracker)
			{
				if (!(*m_phHMD_Tracker))
				{
					pcDevice->Release();
					pcContext->Release();
					return nullptr;
				}

				// for some reason we have to initialize OVR here again ?!
				ovrResult result = ovr_Initialize(nullptr);
				if (!OVR_SUCCESS(result))
				{
					OutputDebugString(L"[OVR] Failed to initialize libOVR.");
					pcDevice->Release();
					pcContext->Release();
					return nullptr;
				}

				// set same handle as created by Oculus tracker node
				OutputDebugString(L"[OVR] Receive session handle from tracker node.");
				m_phHMD = m_phHMD_Tracker;
				unFrameSkip = 100;
				return nullptr;
			}
			else
			{
				// Initialize LibOVR
				ovrResult result = ovr_Initialize(nullptr);
				if (!OVR_SUCCESS(result))
				{
					OutputDebugString(L"[OVR] Failed to initialize libOVR.");
					pcDevice->Release();
					pcContext->Release();
					return nullptr;
				}

				// create hmd (or session) handle
				result = ovr_Create(&m_hHMD, &m_sLuid);
				if (!OVR_SUCCESS(result))
				{
					pcDevice->Release();
					pcContext->Release();
					return nullptr;
				}

				// set pointer to own created handle
				m_phHMD = &m_hHMD;
			}
		}

#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
		// Initialize the avatar module
		ovrAvatar_Initialize(OVR_SAMPLE_APP_ID);

		// Start retrieving the avatar specification
		OutputDebugString(L"[OVR] Requesting avatar specification...\r\n");
		ovrID unUserID = ovr_GetLoggedInUserID();
		ovrAvatar_RequestAvatarSpecification(unUserID);
#endif
		// get HMD description
		m_sHMDDesc = ovr_GetHmdDesc(*m_phHMD);

		if (!m_pcDeviceTemporary)
		{
			// create temporary (OVR) device
			IDXGIFactory * DXGIFactory;
			IDXGIAdapter * Adapter;
			if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
				return(false);
			if (FAILED(DXGIFactory->EnumAdapters(0, &Adapter)))
				return(false);
			if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
				NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pcDeviceTemporary, NULL, &m_pcContextTemporary)))
				return(false);
			DXGIFactory->Release();

			// Set max frame latency to 1
			IDXGIDevice1* DXGIDevice1 = NULL;
			HRESULT hr = m_pcDeviceTemporary->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
			if (FAILED(hr) | (DXGIDevice1 == NULL))
			{
				pcDevice->Release();
				pcContext->Release();
				return nullptr;
			}
			DXGIDevice1->SetMaximumFrameLatency(1);
			DXGIDevice1->Release();
		}

		// Set a standard blend state, ie transparency from alpha
		D3D11_BLEND_DESC bm;
		memset(&bm, 0, sizeof(bm));
		bm.RenderTarget[0].BlendEnable = TRUE;
		bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState * BlendState;
		m_pcDeviceTemporary->CreateBlendState(&bm, &BlendState);
		m_pcContextTemporary->OMSetBlendState(BlendState, NULL, 0xffffffff);

		// Make the eye render buffers (caution if actual size < requested due to HW limits). 
		for (int eye = 0; eye < 2; ++eye)
		{
			ovrSizei sIdealSize = ovr_GetFovTextureSize(*m_phHMD, (ovrEyeType)eye, m_sHMDDesc.DefaultEyeFov[eye], 1.0f);
			if (sIdealSize.w == 0)
			{
				// strange bug here... in D3D9 we get no size here
				sIdealSize.w = 1200;
				sIdealSize.h = 2160;
			}
			m_psEyeRenderTexture[eye] = new OculusTexture();

			if (!m_psEyeRenderTexture[eye]->Init(m_pcDeviceTemporary, *m_phHMD, sIdealSize.w, sIdealSize.h))
			{
				OutputDebugString(L"[OVR] Failed to create eye texture.");
				m_bInit = true;

				pcDevice->Release();
				pcContext->Release();
				return nullptr;
			}
			m_psEyeRenderViewport[eye].Pos.x = 0;
			m_psEyeRenderViewport[eye].Pos.y = 0;
			m_psEyeRenderViewport[eye].Size = sIdealSize;
			if (!m_psEyeRenderTexture[eye]->TextureChain)
			{
				OutputDebugString(L"[OVR]: Failed to create texture chain.");
				m_bInit = true;

				pcDevice->Release();
				pcContext->Release();
				return nullptr;

			}
		}

		// mirror texture only available if D3D11
		if (pcSwapChain)
		{
			// get viewport size
			ID3D11Texture2D* pcBackBuffer = nullptr;
			D3D11_TEXTURE2D_DESC sDescBackBuffer = {};
			pcSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pcBackBuffer);
			if (pcBackBuffer)
			{
				pcBackBuffer->GetDesc(&sDescBackBuffer);
				pcBackBuffer->Release();
			}

			// create HUD oculus texture swapchain
			m_psEyeRenderTextureHUD = new OculusTexture();
			if (!m_psEyeRenderTextureHUD->Init(m_pcDeviceTemporary, *m_phHMD, sDescBackBuffer.Width, sDescBackBuffer.Height))
			{
				OutputDebugString(L"[OVR] Failed to create HUD texture.");
				m_bInit = true;


				pcDevice->Release();
				pcContext->Release();
				return nullptr;
			}

			// Create a mirror to see on the monitor.
			D3D11_TEXTURE2D_DESC sTd = {};
			sTd.ArraySize = 1;
			sTd.Format = sDescBackBuffer.Format;
			sTd.Width = (UINT)sDescBackBuffer.Width;
			sTd.Height = (UINT)sDescBackBuffer.Height;
			sTd.Usage = D3D11_USAGE_DEFAULT;
			sTd.SampleDesc.Count = 1;
			sTd.MipLevels = 1;
			sTd.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
			m_pcDeviceTemporary->CreateTexture2D(&sTd, nullptr, &m_pcMirrorTextureD3D11HMD);
			ovrMirrorTextureDesc sMirrorDesc = {};
			sMirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM;
			sMirrorDesc.Width = (UINT)sDescBackBuffer.Width;
			sMirrorDesc.Height = (UINT)sDescBackBuffer.Height;
			ovrResult result = ovr_CreateMirrorTextureDX(*m_phHMD, m_pcDeviceTemporary, &sMirrorDesc, &m_pcMirrorTexture);
			if (!OVR_SUCCESS(result))
			{
				OutputDebugString(L"[OVR]: Failed to create mirror texture.");
			}

			// get shared handle
			IDXGIResource* pcDXGIResource(NULL);
			m_pcMirrorTextureD3D11HMD->QueryInterface(__uuidof(IDXGIResource), (void**)&pcDXGIResource);
			HANDLE sharedHandle;
			if (pcDXGIResource)
			{
				pcDXGIResource->GetSharedHandle(&sharedHandle);
				pcDXGIResource->Release();
			}
			else OutputDebugString(L"[OVR] Failed to query IDXGIResource.");

			// open the shared handle with the temporary device
			ID3D11Resource* pcResourceShared;
			pcDevice->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&pcResourceShared));
			if (pcResourceShared)
			{
				pcResourceShared->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_pcMirrorTextureD3D11));
				pcResourceShared->Release();
			}
			else OutputDebugString(L"[OVR] Could not open shared resource.");
		}

		// Setup VR components, filling out description
		m_psEyeRenderDesc[0] = ovr_GetRenderDesc(*m_phHMD, ovrEye_Left, m_sHMDDesc.DefaultEyeFov[0]);
		m_psEyeRenderDesc[1] = ovr_GetRenderDesc(*m_phHMD, ovrEye_Right, m_sHMDDesc.DefaultEyeFov[1]);

		// set chosen performance hud option
		ovr_SetInt(*m_phHMD, OVR_PERF_HUD_MODE, (int)m_ePerfHudMode);

		m_bInit = true;
#pragma endregion
	}
	else
	{
#pragma region Avatar
#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
		// get avatar messages
		if (ovrAvatarMessage* psMessage = ovrAvatarMessage_Pop())
		{
			{ wchar_t buf[128]; wsprintf(buf, L"[OVR] New Avatar Message... %x", ovrAvatarMessage_GetType(psMessage)); OutputDebugString(buf); }
			switch (ovrAvatarMessage_GetType(psMessage))
			{
				case ovrAvatarMessageType_AvatarSpecification:
					if (true)
					{
						// get the specification
						const ovrAvatarMessage_AvatarSpecification* psASMessage = ovrAvatarMessage_GetAvatarSpecification(psMessage);

						// Create the avatar instance
						m_psAvatar = ovrAvatar_Create(psASMessage->avatarSpec, ovrAvatarCapability_All);

						// Trigger load operations for all of the assets referenced by the avatar
						uint32_t refCount = ovrAvatar_GetReferencedAssetCount(m_psAvatar);
						for (uint32_t i = 0; i < refCount; ++i)
						{
							ovrAvatarAssetID id = ovrAvatar_GetReferencedAsset(m_psAvatar, i);
							ovrAvatarAsset_BeginLoading(id);
							++m_nLoadingAssets;
						}
					}
					break;
				case ovrAvatarMessageType_AssetLoaded:
					if (true)
					{
						// get the loaded asset message
						const ovrAvatarMessage_AssetLoaded* psALMessage = ovrAvatarMessage_GetAssetLoaded(psMessage);

						// Determine the type of the asset that got loaded
						ovrAvatarAssetType assetType = ovrAvatarAsset_GetType(psALMessage->asset);
						AvatarData sData;

						// Call the appropriate loader function
						switch (assetType)
						{
							case ovrAvatarAssetType_Mesh:
								LoadMesh(m_pcDeviceTemporary, ovrAvatarAsset_GetMeshData(psALMessage->asset), &sData.sMesh);
								//{ char buf[128]; sprintf(buf, "[OVR] Asset loaded (mesh) %" PRIx64, psALMessage->assetID); OutputDebugStringA(buf); }
								break;
							case ovrAvatarAssetType_Texture:
								LoadTexture(m_pcDeviceTemporary, ovrAvatarAsset_GetTextureData(psALMessage->asset), &sData.sTexture);
								break;
						}

						// Store the data that we loaded for the asset in the asset map
						uint32_t uIndex = (uint32_t)m_asAssetMap.size();
						m_asAssetMap.push_back(sData);
						m_auAssetIndexMap[psALMessage->assetID] = uIndex;
						--m_nLoadingAssets;
					}
					break;
			}
			OutputDebugString(L"[OVR] Avatar Message processed...");
			ovrAvatarMessage_Free(psMessage);
		}
#else
		if (!m_asAssetMap.size())
		{
			// create the meshes manually for 32bit
			AvatarData sData = {};

			// create the vertex buffer
			D3D11_BUFFER_DESC sDesc = {};
			sDesc.Usage = D3D11_USAGE_DEFAULT;
			sDesc.ByteWidth = sizeof(ovrAvatarMeshVertex)* unAvatarVertexCount_4070;
			sDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			sDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA sInitData;
			ZeroMemory(&sInitData, sizeof(sInitData));
			sInitData.pSysMem = asAvatarVertices_4070;
			if (FAILED(m_pcDeviceTemporary->CreateBuffer(&sDesc, &sInitData, &sData.sMesh.pcVertexBuffer)))
				OutputDebugString(L"[OVR] Failed to create vertex buffer.");

			// create index buffer
			D3D11_BUFFER_DESC sIBDesc = {};
			sIBDesc.Usage = D3D11_USAGE_DEFAULT;
			sIBDesc.ByteWidth = sizeof(WORD)*unAvatarIndexCount_4070;
			sIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			sIBDesc.CPUAccessFlags = 0;
			ZeroMemory(&sInitData, sizeof(sInitData));
			sInitData.pSysMem = aunAvatarIndices_4070;
			if (FAILED(m_pcDeviceTemporary->CreateBuffer(&sIBDesc, &sInitData, &sData.sMesh.pcElementBuffer)))
				OutputDebugString(L"[OVR] Failed to create index buffer.");
			sData.sMesh.unElementCount = unAvatarIndexCount_4070;

			// Translate the bind pose
			for (uint32_t i = 0; i < unJointCount_4070; ++i)
			{
				sData.sMesh.asBindPose[i] = asBindPose_4070[i];
				D3DXMatrixInverse((D3DXMATRIX*)&sData.sMesh.asInverseBindPose[i], NULL, (D3DXMATRIX*)&sData.sMesh.asBindPose[i]);
			}

			m_asAssetMap.push_back(sData);
		}
#endif
#pragma endregion
#pragma region Render
		// secondary hud active ? copy in case
		if ((m_pbZoomOut) && (m_ppcTexViewHud11))
		{
			if ((*m_pbZoomOut) && (*m_ppcTexViewHud11))
			{
				// get the resource
				ID3D11Resource* pcResource = nullptr;
				(*m_ppcTexViewHud11)->GetResource(&pcResource);
				if (pcResource)
				{
					// copy the hud
					pcContext->CopyResource(m_pcTex11CopyHUD, pcResource);
					pcResource->Release();
				}
			}
		}

		// get eye render pose and other fields
		ovrPosef asHmdToEyePose[2] = { m_psEyeRenderDesc[0].HmdToEyePose, m_psEyeRenderDesc[1].HmdToEyePose };
		ovrPosef         asEyeRenderPose[2];
		ovrPosef         ZeroPose; ZeroMemory(&ZeroPose, sizeof(ovrPosef));
		ovrTrackingState sHmdState = ovr_GetTrackingState(*m_phHMD, ovr_GetTimeInSeconds(), false);
		static long long s_frameIndex = 0;
		static double s_sensorSampleTime = 0.0;    // sensorSampleTime is fed into the layer later
		ovr_GetEyePoses(*m_phHMD, s_frameIndex, ovrTrue, asHmdToEyePose, asEyeRenderPose, &s_sensorSampleTime);
		FLOAT colorBlack[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		double sensorSampleTime = ovr_GetTimeInSeconds();

		// render
		for (int eye = 0; eye < 2; eye++)
		{
#pragma region create and render
			// set viewport
			D3D11_VIEWPORT sD3Dvp;
			sD3Dvp.Width = (float)m_psEyeRenderViewport[eye].Size.w;
			sD3Dvp.Height = (float)m_psEyeRenderViewport[eye].Size.h;
			sD3Dvp.MinDepth = 0;
			sD3Dvp.MaxDepth = 1;
			sD3Dvp.TopLeftX = (float)m_psEyeRenderViewport[eye].Pos.x;
			sD3Dvp.TopLeftY = (float)m_psEyeRenderViewport[eye].Pos.y;
			m_pcContextTemporary->RSSetViewports(1, &sD3Dvp);

			// clear and set render target
			ID3D11RenderTargetView* pcRTV = m_psEyeRenderTexture[eye]->GetRTV();
			m_pcContextTemporary->OMSetRenderTargets(1, &pcRTV, nullptr);
			m_pcContextTemporary->ClearRenderTargetView(pcRTV, colorBlack);

			// texture connected ?
			if ((m_ppcTexView11[eye]) && (*m_ppcTexView11[eye]))
			{
				ID3D11Resource* pcResource = nullptr;
				(*m_ppcTexView11[eye])->GetResource(&pcResource);

				if (!m_pcTex11Copy[eye])
				{
					// get the description and create the copy texture
					D3D11_TEXTURE2D_DESC sDesc;
					((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);
					sDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
					sDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
					if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&m_pcTex11Copy[eye])))
					{
						OutputDebugString(L"[OVR] Failed to create twin texture !");
						break;
					}

					// create the HUD texture, shared + view
					if (!m_pcTex11CopyHUD)
					{
						if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&m_pcTex11CopyHUD)))
						{
							OutputDebugString(L"[OVR] Failed to create HUD copy texture !");
							break;
						}

						// get shared handle
						IDXGIResource* pcDXGIResource(NULL);
						m_pcTex11CopyHUD->QueryInterface(__uuidof(IDXGIResource), (void**)&pcDXGIResource);
						HANDLE sharedHandle;
						if (pcDXGIResource)
						{
							pcDXGIResource->GetSharedHandle(&sharedHandle);
							pcDXGIResource->Release();
						}
						else OutputDebugString(L"[OVR] Failed to query IDXGIResource.");

						// open the shared handle with the temporary device
						ID3D11Resource* pcResourceShared = nullptr;
						m_pcDeviceTemporary->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&pcResourceShared));
						if (pcResourceShared)
						{
							pcResourceShared->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_pcTex11SharedHUD));
							pcResourceShared->Release();
						}
						else OutputDebugString(L"[OVR] Could not open shared resource.");

						// create shader resource view
						if (m_pcTex11SharedHUD)
						{
							D3D11_SHADER_RESOURCE_VIEW_DESC sDescSRV;
							ZeroMemory(&sDescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
							sDescSRV.Format = sDesc.Format;
							sDescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
							sDescSRV.Texture2D.MostDetailedMip = 0;
							sDescSRV.Texture2D.MipLevels = 1;
							if (FAILED(m_pcDeviceTemporary->CreateShaderResourceView(m_pcTex11SharedHUD, &sDescSRV, &m_pcTex11SharedHudSRV)))
								OutputDebugString(L"[OVR] Failed to create shader resource view.");
						}
						else OutputDebugString(L"[OVR] No Texture available.");
					}

					// get shared handle
					IDXGIResource* pcDXGIResource(NULL);
					m_pcTex11Copy[eye]->QueryInterface(__uuidof(IDXGIResource), (void**)&pcDXGIResource);
					HANDLE sharedHandle;
					if (pcDXGIResource)
					{
						pcDXGIResource->GetSharedHandle(&sharedHandle);
						pcDXGIResource->Release();
					}
					else OutputDebugString(L"[OVR] Failed to query IDXGIResource.");

					// open the shared handle with the temporary device
					ID3D11Resource* pcResourceShared;
					m_pcDeviceTemporary->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&pcResourceShared));
					if (pcResourceShared)
					{
						pcResourceShared->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&m_pcFrameTexture[eye]));
						pcResourceShared->Release();
					}
					else OutputDebugString(L"[OVR] Could not open shared resource.");

					// create shader resource view
					if (m_pcFrameTexture[eye])
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC sDescSRV;
						ZeroMemory(&sDescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
						sDescSRV.Format = sDesc.Format;
						sDescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						sDescSRV.Texture2D.MostDetailedMip = 0;
						sDescSRV.Texture2D.MipLevels = 1;
						if (FAILED(m_pcDeviceTemporary->CreateShaderResourceView(m_pcFrameTexture[eye], &sDescSRV, &m_pcFrameTextureSRView[eye])))
							OutputDebugString(L"[OVR] Failed to create shader resource view.");
					}
					else OutputDebugString(L"[OVR] No Texture available.");
				}
				else
				{
					// copy the frame tex to shared texture
					pcContext->CopyResource(m_pcTex11Copy[eye], pcResource);

					// create all bool
					bool bAllCreated = true;

					// create vertex shader
					if (!m_pcVertexShader11)
					{
						if (FAILED(CreateVertexShaderTechnique(m_pcDeviceTemporary, &m_pcVertexShader11, &m_pcVertexLayout11, VertexShaderTechnique::PosUV2D)))
						{
							OutputDebugString(L"[OVR] Failed to create vertex shader. ");
							bAllCreated = false;
						}
					}

					// create pixel shader... 
					if (!m_pcPixelShader11)
					{
						if (FAILED(CreatePixelShaderEffect(m_pcDeviceTemporary, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
							bAllCreated = false;
					}

					// Create vertex buffer
					if (!m_pcVertexBuffer11)
					{
						if (FAILED(CreateFullScreenVertexBuffer(m_pcDeviceTemporary, &m_pcVertexBuffer11)))
							bAllCreated = false;
					}

					// create constant buffer
					if (!m_pcConstantBufferDirect11)
					{
						if (FAILED(CreateMatrixConstantBuffer(m_pcDeviceTemporary, &m_pcConstantBufferDirect11)))
							bAllCreated = false;
					}

					// sampler ?
					if (!m_pcSamplerState)
					{
						// Create the sampler state
						D3D11_SAMPLER_DESC sSampDesc;
						ZeroMemory(&sSampDesc, sizeof(sSampDesc));
						sSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
						sSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
						sSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
						sSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
						sSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
						sSampDesc.MinLOD = 0;
						sSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
						if (FAILED(m_pcDeviceTemporary->CreateSamplerState(&sSampDesc, &m_pcSamplerState)))
						{
							OutputDebugString(L"[OVR] Failed to create Sampler State.");
							bAllCreated = false;
						}
					}

					// avatar vertex shader
					if (!m_pcVSAvatar)
					{
						ID3D10Blob* pcShader;
						HRESULT hr;

						// compile shader
						hr = D3DX10CompileFromMemory(VS_OCULUS_AVATAR, strlen(VS_OCULUS_AVATAR), NULL, NULL, NULL, "main", "vs_5_0", NULL, NULL, NULL, &pcShader, NULL, NULL);


						// create shader
						if (SUCCEEDED(hr))
						{
							//#ifdef _DEBUG
							OutputDebugString(L"[OVR] : Avatar Vertex Shader compiled !");
							//#endif
							hr = m_pcDeviceTemporary->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcVSAvatar);

							if (FAILED(hr))
							{
								OutputDebugString(L"[OVR] : Failed to create Avatar Vertex Shader !");
								bAllCreated = false;
							}
							else
							{
								// create input layout
								D3D11_INPUT_ELEMENT_DESC aLayout01[] =
								{
									{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
									{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
									{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
									{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
									{ "TEXCOORD", 3, DXGI_FORMAT_R8G8B8A8_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
									{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
								};

								UINT unNumElements = sizeof(aLayout01) / sizeof(aLayout01[0]);
								hr = pcDevice->CreateInputLayout(aLayout01, unNumElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcILAvatar);

								if (FAILED(hr))
								{
									OutputDebugString(L"[OVR] Failed to create avatar input layout !");
									bAllCreated = false;
								}
							}
							pcShader->Release();
						}
					}

					// create avatar pixel shader
					if (!m_pcPSAvatar)
					{
						ID3D10Blob* pcShader;
						HRESULT hr;

						hr = D3DX10CompileFromMemory(PS_OCULUS_AVATAR, strlen(PS_OCULUS_AVATAR), NULL, NULL, NULL, "main", "ps_5_0", NULL, NULL, NULL, &pcShader, NULL, NULL);

						// succeded ?
						if (SUCCEEDED(hr))
						{
							OutputDebugString(L"[OVR] : Avatar Pixel shader compiled !");
							m_pcDeviceTemporary->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcPSAvatar);
							pcShader->Release();
						}
						else
						{
							OutputDebugString(L"[OVR] : Failed to create Avatar pixel shader !");
							bAllCreated = false;
						}
					}

					// create constant buffer
					if (!m_pcCVSAvatar)
					{
						if (FAILED(CreateGeometryConstantBuffer(m_pcDeviceTemporary, &m_pcCVSAvatar, (UINT)sizeof(ConstantsVS))))
						{
							OutputDebugString(L"[OVR] Failed to create avatar vertex constant buffer.");
							bAllCreated = false;
						}
					}

					// create constant buffer
					if (!m_pcCPSAvatar)
					{
						if (FAILED(CreateGeometryConstantBuffer(m_pcDeviceTemporary, &m_pcCPSAvatar, (UINT)sizeof(FragmentVars))))
						{
							OutputDebugString(L"[OVR] Failed to create avatar fragment constant buffer.");
							bAllCreated = false;
						}
					}

					// create the depth stencil
					if (!m_pcDSGeometry11[0])
					{
						ID3D11Texture2D* pcResource = nullptr;
						if (m_ppcTexView11[0])
							(*m_ppcTexView11)[0]->GetResource((ID3D11Resource**)&pcResource);

						if (pcResource)
						{
							D3D11_TEXTURE2D_DESC sDesc;
							pcResource->GetDesc(&sDesc);
							pcResource->Release();

							// Create depth stencil textures
							D3D11_TEXTURE2D_DESC descDepth;
							ZeroMemory(&descDepth, sizeof(descDepth));
							descDepth.Width = sDesc.Width;
							descDepth.Height = sDesc.Height;
							descDepth.MipLevels = 1;
							descDepth.ArraySize = 1;
							descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
							descDepth.SampleDesc.Count = 1;
							descDepth.SampleDesc.Quality = 0;
							descDepth.Usage = D3D11_USAGE_DEFAULT;
							descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
							descDepth.CPUAccessFlags = 0;
							descDepth.MiscFlags = 0;
							if (FAILED(m_pcDeviceTemporary->CreateTexture2D(&descDepth, NULL, &m_pcDSGeometry11[0])))
								OutputDebugString(L"[OVR] Failed to create depth stencil.");
							if (FAILED(m_pcDeviceTemporary->CreateTexture2D(&descDepth, NULL, &m_pcDSGeometry11[1])))
								OutputDebugString(L"[OVR] Failed to create depth stencil.");

							// Create the depth stencil views
							D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
							ZeroMemory(&descDSV, sizeof(descDSV));
							descDSV.Format = descDepth.Format;
							descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
							descDSV.Texture2D.MipSlice = 0;
							if (FAILED(m_pcDeviceTemporary->CreateDepthStencilView(m_pcDSGeometry11[0], &descDSV, &m_pcDSVGeometry11[0])))
								OutputDebugString(L"[OVR] Failed to create depth stencil view.");
							if (FAILED(m_pcDeviceTemporary->CreateDepthStencilView(m_pcDSGeometry11[1], &descDSV, &m_pcDSVGeometry11[1])))
								OutputDebugString(L"[OVR] Failed to create depth stencil view.");
						}
					}

					// depth stencil state less
					if (!m_pcDSStateLess)
					{
						D3D11_DEPTH_STENCIL_DESC sDesc = {};
						sDesc.DepthEnable = TRUE;
						sDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
						sDesc.DepthFunc = D3D11_COMPARISON_LESS;
						sDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
						sDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
						sDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
						sDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
						sDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
						sDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
						if (FAILED(m_pcDeviceTemporary->CreateDepthStencilState(&sDesc, &m_pcDSStateLess)))
							OutputDebugString(L"[OVR] Failed to create depth stencil state.");
					}
					// depth stencil state equal
					if (!m_pcDSStateEqual)
					{
						D3D11_DEPTH_STENCIL_DESC sDesc = {};
						sDesc.DepthEnable = TRUE;
						sDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
						sDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
						sDesc.StencilEnable = FALSE;
						sDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
						sDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
						sDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
						sDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
						sDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
						sDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
						sDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
						if (FAILED(m_pcDeviceTemporary->CreateDepthStencilState(&sDesc, &m_pcDSStateEqual)))
							OutputDebugString(L"[OVR] Failed to create depth stencil state.");
					}
					// rasterizer state
					if (!m_pcRS)
					{
						D3D11_RASTERIZER_DESC sDesc = {};
						sDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
						sDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
						sDesc.FrontCounterClockwise = FALSE;
						sDesc.DepthBias = 0;
						sDesc.SlopeScaledDepthBias = 0.0f;
						sDesc.DepthBiasClamp = 0.0f;
						sDesc.DepthClipEnable = TRUE;
						sDesc.ScissorEnable = FALSE;
						sDesc.MultisampleEnable = FALSE;
						sDesc.AntialiasedLineEnable = FALSE;
						if (FAILED(m_pcDeviceTemporary->CreateRasterizerState(&sDesc, &m_pcRS)))
							OutputDebugString(L"[OVR] Failed to create rasterizer state.");
					}

					// own state for right hand, for 32bit we render the right hand as a copy of the left
					if (!m_pcRS_RightHand)
					{
						D3D11_RASTERIZER_DESC sDesc = {};
						sDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
						sDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
						sDesc.FrontCounterClockwise = FALSE;
						sDesc.DepthBias = 0;
						sDesc.SlopeScaledDepthBias = 0.0f;
						sDesc.DepthBiasClamp = 0.0f;
						sDesc.DepthClipEnable = TRUE;
						sDesc.ScissorEnable = FALSE;
						sDesc.MultisampleEnable = FALSE;
						sDesc.AntialiasedLineEnable = FALSE;
						if (FAILED(m_pcDeviceTemporary->CreateRasterizerState(&sDesc, &m_pcRS_RightHand)))
							OutputDebugString(L"[OVR] Failed to create rasterizer state.");
					}

					if (bAllCreated)
					{
						// Set the input layout
						m_pcContextTemporary->IASetInputLayout(m_pcVertexLayout11);

						// Set vertex buffer
						UINT stride = sizeof(TexturedVertex);
						UINT offset = 0;
						m_pcContextTemporary->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

						//// Measured DK2 Field of View :
						////
						//// LENS - CAMERA DISTANCE   0MM  5MM  10MM 15MM 20MM 25MM
						//// LEFT HORIZONTAL FOV      49°  48°  47°  45°  38°  34°
						//// RIGHT HORIZONTAL FOV     47°	 47°  46°  44°	37°  33°
						//// TOTAL HORIZONTAL FOV     96°	 95°  93°  89°	75°  67°
						//// VERTICAL FOV             107° 106° 104° 99°	76°  66°
						//// The Rift DK2’s FoV is maximal at 0mm lens - camera distance, 
						//// i.e., when the user’s eyeball touches the lens.This is generally 
						//// not achievable, and the minimum eye relief configurable in the 
						//// SDK is 8mm, with an effective binocular FoV of 94° x 105°.

						//// Measured CV1 Field of View :
						////
						//// LENS - CAMERA DISTANCE	0MM   5MM   10MM  15MM  20MM  25MM
						//// LEFT HORIZONTAL FOV      35°   36°   37°   37°   35°   30°
						//// RIGHT HORIZONTAL FOV     43°   45°   47°   47°   44°   39°
						//// TOTAL HORIZONTAL FOV     78°   81°   84°   84°   79°   69°
						//// VERTICAL FOV             84°   89°   93°   86°   78°   69°
						//// As in Vive DK1 / Pre, the maximal FoV is achieved at some distance 
						//// from the lens, in this case 12mm. Taking the left / right FoV value 
						//// differences as accurate, the total binocular FoV at that lens - camera 
						//// distance is 94° x 93°.

						//// Measurements posted on <Doc-Ok.org>.

						//// so we assume a FoV of 90° x 100° (DK2) and 84° x 93° (CV1), that is ~12 mm lens-eye 
						//// distance
						//// due to this aspect ratio of the DK2/CV1 we adjust the screen by the height
						//// in this case we need to set a higher FOV by following formular:
						//// V = 2 * arctan( tan(H / 2) * aspectratio ) 
						//// so we get V 90° and H 121° (DK2) and V 84° and H 116° (CV1)

						// get identity matrix, this is a fullscreen copy
						OVR::Matrix4f sProj = OVR::Matrix4f::Identity();

						// Set constant buffer, first update it... scale and translate the left and right image
						m_pcContextTemporary->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
						m_pcContextTemporary->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

						// Set primitive topology
						m_pcContextTemporary->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						// set texture, sampler state
						m_pcContextTemporary->PSSetShaderResources(0, 1, &m_pcFrameTextureSRView[eye]);
						m_pcContextTemporary->PSSetSamplers(0, 1, &m_pcSamplerState);

						// set shaders
						m_pcContextTemporary->VSSetShader(m_pcVertexShader11, 0, 0);
						m_pcContextTemporary->PSSetShader(m_pcPixelShader11, 0, 0);

						// Render a triangle
						m_pcContextTemporary->Draw(6, 0);

#pragma endregion
#pragma region render avatar
						/*
						* Current hand pose enumeration.
						**/
						enum OculusHandPose
						{
							default,//: finger 0 + 1 on touch but not pressed, finger 3 - 5 released
							pinch,//: finger 0 + 1 pressed, finger 3 - 5 released
							fist,// : all fingers pressed
							point_relaxed,//: finger 0 on touch, finger 1 fully up, finger 3 - 5 released
							thumb_relaxed,//: finger 0 fully up, finger 1 on touch, finger 3 - 5 released
							thumb_point_relaxed,//: finger 0 + 1 fully up, finger 3 - 5 released
							point,// : finger 0 on touch or pressed, finger 1 fully up, finger 3 - 5 pressed
							thumb,// : finger 0 fully up, finger 1 on touch or pressed, finger 3 - 5 pressed
							thumb_point,// : finger 0 + 1 fully up, finger 3 - 5 pressed
						} eHandPose[2];

						// transforms
						ovrAvatarTransform hmd, left, right;

						// true if touch controller [left/right] is available
						bool bTouchAvailable[2];

#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
						if (m_psAvatar)
#else
						if (true)
#endif
						{
							// Convert the OVR inputs into Avatar SDK inputs
							ovrInputState touchState;
							ovr_GetInputState(*m_phHMD, ovrControllerType_Active, &touchState);
							ovrTrackingState trackingState = ovr_GetTrackingState(*m_phHMD, 0.0, false);

							hmd.position.x = trackingState.HeadPose.ThePose.Position.x;
							hmd.position.y = trackingState.HeadPose.ThePose.Position.y;
							hmd.position.z = trackingState.HeadPose.ThePose.Position.z;
							hmd.orientation.x = trackingState.HeadPose.ThePose.Orientation.x;
							hmd.orientation.y = trackingState.HeadPose.ThePose.Orientation.y;
							hmd.orientation.z = trackingState.HeadPose.ThePose.Orientation.z;
							hmd.orientation.w = trackingState.HeadPose.ThePose.Orientation.w;
							hmd.scale.x = 1.0f;
							hmd.scale.y = 1.0f;
							hmd.scale.z = 1.0f;

							if (trackingState.HandStatusFlags[ovrHand_Left])
							{
								left.position.x = trackingState.HandPoses[ovrHand_Left].ThePose.Position.x;
								left.position.y = trackingState.HandPoses[ovrHand_Left].ThePose.Position.y;
								left.position.z = trackingState.HandPoses[ovrHand_Left].ThePose.Position.z;
								left.orientation.x = trackingState.HandPoses[ovrHand_Left].ThePose.Orientation.x;
								left.orientation.y = trackingState.HandPoses[ovrHand_Left].ThePose.Orientation.y;
								left.orientation.z = trackingState.HandPoses[ovrHand_Left].ThePose.Orientation.z;
								left.orientation.w = trackingState.HandPoses[ovrHand_Left].ThePose.Orientation.w;
								left.scale.x = 1.0f;
								left.scale.y = 1.0f;
								left.scale.z = 1.0f;
								bTouchAvailable[ovrHand_Left] = true;
							}
							else
								bTouchAvailable[ovrHand_Left] = false;

							if (trackingState.HandStatusFlags[ovrHand_Left])
							{
								right.position.x = trackingState.HandPoses[ovrHand_Right].ThePose.Position.x;
								right.position.y = trackingState.HandPoses[ovrHand_Right].ThePose.Position.y;
								right.position.z = trackingState.HandPoses[ovrHand_Right].ThePose.Position.z;
								right.orientation.x = trackingState.HandPoses[ovrHand_Right].ThePose.Orientation.x;
								right.orientation.y = trackingState.HandPoses[ovrHand_Right].ThePose.Orientation.y;
								right.orientation.z = trackingState.HandPoses[ovrHand_Right].ThePose.Orientation.z;
								right.orientation.w = trackingState.HandPoses[ovrHand_Right].ThePose.Orientation.w;
								right.scale.x = 1.0f;
								right.scale.y = 1.0f;
								right.scale.z = 1.0f;
								bTouchAvailable[ovrHand_Right] = true;
							}
							else
								bTouchAvailable[ovrHand_Right] = false;

#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
							// create input states
							ovrAvatarHandInputState inputStateLeft;
							AvatarHandInputStateFromOvr(left, touchState, ovrHand_Left, &inputStateLeft);

							ovrAvatarHandInputState inputStateRight;
							AvatarHandInputStateFromOvr(right, touchState, ovrHand_Right, &inputStateRight);

							// Update the avatar pose from the inputs
							ovrAvatarPose_UpdateBody(m_psAvatar, hmd);
							ovrAvatarPose_UpdateHands(m_psAvatar, inputStateLeft, inputStateRight);
							ovrAvatarPose_Finalize(m_psAvatar, m_sConstantsFS.elapsedSeconds);
#endif
							for (unsigned uHand = 0; uHand < 2; uHand++)
							{
								// set the hand poses manually... 
								bool bPoint = false;
								if (uHand)
								{
									if (touchState.Touches & ovrTouch_RIndexPointing) bPoint = true;
								}
								else
								{
									if (touchState.Touches & ovrTouch_LIndexPointing) bPoint = true;
								}
								bool bFist = (touchState.HandTrigger[uHand] > 0.5f);
								bool bPinch = (touchState.IndexTrigger[uHand] > 0.5f);

								// thumb down ?
								unsigned uTouchStates = (ovrTouch_X | ovrTouch_Y | ovrTouch_LThumb | ovrTouch_LThumbRest);
								if (uHand) uTouchStates = (ovrTouch_A | ovrTouch_B | ovrTouch_RThumb | ovrTouch_RThumbRest);
								if (touchState.Touches & uTouchStates)
								{
									// pointer (index finger) up ?
									if (bPoint)
									{
										if (bFist)
											eHandPose[uHand] = OculusHandPose::point;
										else
											eHandPose[uHand] = OculusHandPose::point_relaxed;
									}
									else
									{
										if (bFist)
											eHandPose[uHand] = OculusHandPose::fist;
										else
										{
											if (bPinch)
												eHandPose[uHand] = OculusHandPose::pinch;
											else
												eHandPose[uHand] = OculusHandPose::default;
										}
									}

								}
								else
									// thumb up ?
								{
									// pointer (index finger) up ?
									if (bPoint)
									{
										if (bFist)
											eHandPose[uHand] = OculusHandPose::thumb_point;
										else
											eHandPose[uHand] = OculusHandPose::thumb_point_relaxed;
									}
									else
									{
										if (bFist)
											eHandPose[uHand] = OculusHandPose::thumb;
										else
											eHandPose[uHand] = OculusHandPose::thumb_relaxed;
									}
								}

								if (bTouchAvailable[uHand])
								{
									// set cinema matrix/position ... get pose matrix
									D3DXQUATERNION sOrientation;
									sOrientation.x = trackingState.HandPoses[uHand].ThePose.Orientation.x;
									sOrientation.y = trackingState.HandPoses[uHand].ThePose.Orientation.y;
									sOrientation.z = trackingState.HandPoses[uHand].ThePose.Orientation.z;
									sOrientation.w = trackingState.HandPoses[uHand].ThePose.Orientation.w;
									D3DXMatrixRotationQuaternion(&m_sMenu.sPoseMatrix[uHand], &sOrientation);

									// get position
									m_sMenu.sPosition[uHand].x = trackingState.HandPoses[uHand].ThePose.Position.x;
									m_sMenu.sPosition[uHand].y = trackingState.HandPoses[uHand].ThePose.Position.y;
									m_sMenu.sPosition[uHand].z = trackingState.HandPoses[uHand].ThePose.Position.z;

									// pose available
									m_sMenu.bHandPosesPresent = true;
								}
							}
						}

						// build the matrices
						XMFLOAT4X4 sWorld, sView, sProjection;

						// world
						XMMATRIX W = XMMatrixScaling(1.0f, 1.0f, 1.0f);
						XMStoreFloat4x4(&sWorld, W);

						// view
						ovrVector3f eyePosition = asEyeRenderPose[eye].Position;
						__ovrQuatf eyeOrientation;
						eyeOrientation.x = asEyeRenderPose[eye].Orientation.x;
						eyeOrientation.y = asEyeRenderPose[eye].Orientation.y;
						eyeOrientation.z = asEyeRenderPose[eye].Orientation.z;
						eyeOrientation.w = asEyeRenderPose[eye].Orientation.w;
						float fEuler[3];
						eyeOrientation.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_R >(&fEuler[1], &fEuler[0], &fEuler[2]);
						XMMATRIX sRotation;
						XMMATRIX sPitch = XMMatrixRotationX(fEuler[0]);
						XMMATRIX sYaw = XMMatrixRotationY(fEuler[1]);
						XMMATRIX sRoll = XMMatrixRotationZ(fEuler[2]);
						sRotation = sYaw * sPitch * sRoll;
						XMMATRIX sTranslation = XMMatrixTranslation(-eyePosition.x, -eyePosition.y, -eyePosition.z);
						XMStoreFloat4x4(&sView, sTranslation * sRotation);

						// projection...
						ovrMatrix4f ovrProjection = ovrMatrix4f_Projection(m_sHMDDesc.DefaultEyeFov[eye], 0.01f, 1000.0f, ovrProjection_None);
						XMMATRIX sPro(
							ovrProjection.M[0][0], ovrProjection.M[1][0], ovrProjection.M[2][0], ovrProjection.M[3][0],
							ovrProjection.M[0][1], ovrProjection.M[1][1], ovrProjection.M[2][1], ovrProjection.M[3][1],
							ovrProjection.M[0][2], ovrProjection.M[1][2], ovrProjection.M[2][2], ovrProjection.M[3][2],
							ovrProjection.M[0][3], ovrProjection.M[1][3], ovrProjection.M[2][3], ovrProjection.M[3][3]
							);
						XMStoreFloat4x4(&sProjection, sPro);

						// update and set constant buffers
						m_pcContextTemporary->UpdateSubresource(m_pcCPSAvatar, 0, NULL, &m_sConstantsFS, 0, 0);
						m_pcContextTemporary->VSSetConstantBuffers(0, 1, &m_pcCVSAvatar);
						m_pcContextTemporary->PSSetConstantBuffers(0, 1, &m_pcCPSAvatar);

						// set input layout
						m_pcContextTemporary->IASetInputLayout(m_pcILAvatar);

						// set shaders
						m_pcContextTemporary->VSSetShader(m_pcVSAvatar, 0, 0);
						m_pcContextTemporary->PSSetShader(m_pcPSAvatar, 0, 0);

						// clear and set the depth stencil
						m_pcContextTemporary->ClearDepthStencilView(m_pcDSVGeometry11[eye], D3D11_CLEAR_DEPTH, 1.0f, 0);
						m_pcContextTemporary->OMSetRenderTargets(1, &pcRTV, m_pcDSVGeometry11[eye]);

#ifdef _WIN64
						// set rasterizer state
						m_pcContextTemporary->RSSetState(m_pcRS);

						// Traverse over all components on the avatar
						uint32_t componentCount = 0;
						if (m_psAvatar)
							componentCount = ovrAvatarComponent_Count(m_psAvatar);
						else
							OutputDebugString(L"[OVR] Avatar pending !");
						for (uint32_t i = 0; i < componentCount; ++i)
						{
							const ovrAvatarComponent* component = ovrAvatarComponent_Get(m_psAvatar, i);

							// Render each rebder part attached to the component
							for (uint32_t j = 0; j < component->renderPartCount; ++j)
							{
								const ovrAvatarRenderPart* renderPart = component->renderParts[j];
								ovrAvatarRenderPartType type = ovrAvatarRenderPart_GetType(renderPart);
								switch (type)
								{
									case ovrAvatarRenderPartType_SkinnedMeshRenderPBS:
									case ovrAvatarRenderPartType_SkinnedMeshRender:
										//OutputDebugString(L"ovrAvatarRenderPartType_SkinnedMeshRender");
										if (true)
										{
											MeshData* data = nullptr;
											bool bSelfOccluding = false;
											if (type == ovrAvatarRenderPartType_SkinnedMeshRender)
											{
												// get mesh data
												auto mesh = ovrAvatarRenderPart_GetSkinnedMeshRender(renderPart);

												// Get the d3d mesh data for this mesh's asset
												uint32_t uIndex = m_auAssetIndexMap[mesh->meshAssetID];
												if (uIndex == 0) continue;
												data = &m_asAssetMap[uIndex].sMesh;
												if (!data) continue;
												if ((mesh->meshAssetID != AVATAR_MESH_LEFT_HAND) &&
													(mesh->meshAssetID != AVATAR_MESH_RIGHT_HAND))
													continue;

												// Apply the material state
												SetMaterialState(&mesh->materialState, nullptr);
												m_pcContextTemporary->UpdateSubresource(m_pcCPSAvatar, 0, NULL, &m_sConstantsFS, 0, 0);
												// world
												XMMATRIX sScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
												XMStoreFloat4x4(&sWorld, sScale);

												// Set constants
												XMMATRIX sW = XMLoadFloat4x4(&sWorld);
												XMMATRIX sV = XMLoadFloat4x4(&sView);
												XMMATRIX sP = XMLoadFloat4x4(&sProjection);
												XMVECTOR sVP = XMVectorSet(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f);
												if (mesh->meshAssetID == AVATAR_MESH_LEFT_HAND)
													SetMeshState(left, data, mesh->skinnedPose, sW, sV, sP, sVP);
												else
													SetMeshState(right, data, mesh->skinnedPose, sW, sV, sP, sVP);

												// set visibility options
												if (mesh->visibilityMask & ovrAvatarVisibilityFlag_SelfOccluding) bSelfOccluding = true;
											}
											else
											{
												// get mesh data
												auto mesh = ovrAvatarRenderPart_GetSkinnedMeshRenderPBS(renderPart);

												// Get the d3d mesh data for this mesh's asset
												uint32_t uIndex = m_auAssetIndexMap[mesh->meshAssetID];
												if (uIndex == 0) continue;
												data = &m_asAssetMap[uIndex].sMesh;
												if (!data) continue;
												if ((mesh->meshAssetID != AVATAR_MESH_LEFT_HAND) &&
													(mesh->meshAssetID != AVATAR_MESH_RIGHT_HAND))
													continue;

												// world
												XMMATRIX sScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
												XMStoreFloat4x4(&sWorld, sScale);

												// Set constants
												XMMATRIX sW = XMLoadFloat4x4(&sWorld);
												XMMATRIX sV = XMLoadFloat4x4(&sView);
												XMMATRIX sP = XMLoadFloat4x4(&sProjection);
												XMVECTOR sVP = XMVectorSet(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f);
												if (mesh->meshAssetID == AVATAR_MESH_LEFT_HAND)
													SetMeshState(left, data, mesh->skinnedPose, sW, sV, sP, sVP);
												else
													SetMeshState(right, data, mesh->skinnedPose, sW, sV, sP, sVP);

												// set visibility options
												if (mesh->visibilityMask & ovrAvatarVisibilityFlag_SelfOccluding) bSelfOccluding = true;
											}
											m_pcContextTemporary->UpdateSubresource(m_pcCVSAvatar, 0, NULL, &m_sConstantsVS, 0, 0);

											// set vertex buffer, index buffer
											stride = sizeof(ovrAvatarMeshVertex);
											offset = 0;
											m_pcContextTemporary->IASetVertexBuffers(0, 1, &data->pcVertexBuffer, &stride, &offset);
											m_pcContextTemporary->IASetIndexBuffer(data->pcElementBuffer, DXGI_FORMAT_R16_UINT, 0);

											// Write to depth first for self-occlusion
											m_pcContextTemporary->OMSetDepthStencilState(m_pcDSStateLess, 1);
											if (bSelfOccluding)
											{
												m_pcContextTemporary->OMSetRenderTargets(0, nullptr, m_pcDSVGeometry11[eye]);
												m_pcContextTemporary->DrawIndexed(data->unElementCount, 0, 0);
												m_pcContextTemporary->OMSetRenderTargets(1, &pcRTV, m_pcDSVGeometry11[eye]);
												m_pcContextTemporary->OMSetDepthStencilState(m_pcDSStateEqual, 1);
											}
											// draw
											m_pcContextTemporary->DrawIndexed(data->unElementCount, 0, 0);
										}
										break;
									case ovrAvatarRenderPartType_ProjectorRender:
										//OutputDebugString(L"ovrAvatarRenderPartType_ProjectorRender");
										break;
								}
							}
						}

						// set rasterizer to null
						m_pcContextTemporary->RSSetState(nullptr);
#else
						// render by hand for 32bit
						for (uint32_t i = 0; i < (uint32_t)m_asAssetMap.size(); ++i)
						{
							// get mesh... in the 32bit asset map we only have meshes
							MeshData* data = &m_asAssetMap[i].sMesh;

							// Apply the material state
							ovrAvatarMaterialState sState = {};
							sState.baseColor.x = 0.676471f;
							sState.baseColor.y = 0.852941f;
							sState.baseColor.z = 1.0f;
							sState.baseColor.w = 1.0f;

							sState.baseMaskType = ovrAvatarMaterialMaskType_Fresnel;

							sState.baseMaskParameters.x = 0.5f;
							sState.baseMaskParameters.y = 0.0f;
							sState.baseMaskParameters.z = 1.0f;
							sState.baseMaskParameters.w = 1.0f;

							sState.layerCount = 1;
							SetMaterialState(&sState, nullptr);
							m_pcContextTemporary->UpdateSubresource(m_pcCPSAvatar, 0, NULL, &m_sConstantsFS, 0, 0);

							// set vertex buffer, index buffer
							stride = sizeof(ovrAvatarMeshVertex_);
							offset = 0;
							m_pcContextTemporary->IASetVertexBuffers(0, 1, &data->pcVertexBuffer, &stride, &offset);
							m_pcContextTemporary->IASetIndexBuffer(data->pcElementBuffer, DXGI_FORMAT_R16_UINT, 0);

							// render both hands
							for (unsigned uHand = 0; uHand < 2; uHand++)
							{
								// touch controller present ?
								if (!bTouchAvailable[uHand]) continue;

								// world... x -1 for right hand
								XMMATRIX sScale, sLocal;
								if (uHand == 0)
								{
									sScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
									D3DMatrixFromOvrAvatarTransform(left, &sLocal);
								}
								else
								{
									sScale = XMMatrixScaling(-1.0f, 1.0f, 1.0f);
									D3DMatrixFromOvrAvatarTransform(right, &sLocal);
								}

								// Set constants
								XMMATRIX sV = XMLoadFloat4x4(&sView);
								XMMATRIX sP = XMLoadFloat4x4(&sProjection);
								XMVECTOR sVP = XMVectorSet(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f);

								// Compute the final world and viewProjection matrices for this part
								XMMATRIX worldMat = sScale * sLocal;
								XMMATRIX viewProjMat = sV * sP;

								// Compute the skinned pose
								for (uint32_t i = 0; i < unJointCount_4070; ++i)
								{
									D3DXMATRIX sSkinned = asPoseDefault[i];
									switch (eHandPose[uHand])
									{
										case default:
											sSkinned = asPoseDefault[i];
											break;
										case pinch:
											sSkinned = asPosePinch[i];
											break;
										case fist:
											sSkinned = asPoseFist[i];
											break;
										case point_relaxed:
											sSkinned = asPosePointRelaxed[i];
											break;
										case thumb_relaxed:
											sSkinned = asPoseThumbRelaxed1[i]; // < we have two thumb_relaxed poses here, did choose asPoseThumbRelaxed1
											break;
										case thumb_point_relaxed:
											sSkinned = asPoseThumbPointRelaxed[i];
											break;
										case point:
											sSkinned = asPosePoint[i];
											break;
										case thumb:
											sSkinned = asPoseThumb[i];
											break;
										case thumb_point:
											sSkinned = asPoseThumbPoint[i];
											break;
										default:
											break;
									}

									// FOR FUTURE USE ! matrix animation blending basics, to be done
									if (false)
									{
										// get quaternions of default and fist pose
										D3DXMATRIX sBind = data->asBindPose[i];
										D3DXQUATERNION sQ, sQ1;
										D3DXQuaternionRotationMatrix(&sQ, &asPoseDefault[i]);
										D3DXQuaternionRotationMatrix(&sQ1, &asPoseFist[i]);

										// get euler angles from those quaternions
										__ovrQuatf sQ2, sQ3;
										sQ2.x = sQ.x;
										sQ2.y = sQ.y;
										sQ2.z = sQ.z;
										sQ2.w = sQ.w;

										sQ3.x = sQ1.x;
										sQ3.y = sQ1.y;
										sQ3.z = sQ1.z;
										sQ3.w = sQ1.w;
										float fEuler[3], fEuler1[3];
										sQ2.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_L >(&fEuler[1], &fEuler[0], &fEuler[2]);
										sQ3.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_L >(&fEuler1[1], &fEuler1[0], &fEuler1[2]);
										D3DXVECTOR3 sV, sV1, sV3;

										// static const bools (for each joint [0..24]) determining wether counter-clockwise-rotation(true) or clockwise-rotation (false) of the euler angles [0..2]
										static const bool s_abCCW[25][3] = {
											{ false, false, false },
											{ false, false, false },
											{ true, false, true },
											{ false, false, false },
											{ true, false, true },
											{ true, false, true },
											{ true, false, true },
											{ true, false, true },
											{ false, false, true },
											{ false, false, true },
											{ false, false, true },
											{ false, false, true },
											{ true, false, true },
											{ false, true, true },
											{ false, true, true },
											{ false, true, false },
											{ false, true, false },
											{ false, true, true },
											{ false, true, true },
											{ false, true, false },
											{ false, true, false },
											{ false, true, true },
											{ false, true, true },
											{ false, true, true },
											{ false, true, true }
										};

										// do basic blend
										sV.x = (fEuler[0] + fEuler1[0]) / 2.0f;
										sV.y = (fEuler[1] + fEuler1[1]) / 2.0f;
										sV.z = (fEuler[2] + fEuler1[2]) / 2.0f;

										// counter-clockwise ?
										if (s_abCCW[i][0])
										{
											// fEuler < x < fEuler1
											if (fEuler[0] < fEuler1[0])
											{
												if (sV.x > 0) sV.x -= MATH_FLOAT_PI / 2.0f; else sV.x += MATH_FLOAT_PI / 2.0f;
											}
										}
										else
										{
											// fEuler > x > fEuler1
											if (fEuler[0] > fEuler1[0])
											{
												if (sV.x > 0) sV.x -= MATH_FLOAT_PI / 2.0f; else sV.x += MATH_FLOAT_PI / 2.0f;
											}
										}
										if (s_abCCW[i][1])
										{
											// fEuler < x < fEuler1
											if (fEuler[1] < fEuler1[1])
											{
												if (sV.y > 0) sV.y -= MATH_FLOAT_PI / 2.0f; else sV.y += MATH_FLOAT_PI / 2.0f;
											}
										}
										else
										{
											// fEuler > x > fEuler1
											if (fEuler[1] > fEuler1[1])
											{
												if (sV.y > 0) sV.y -= MATH_FLOAT_PI / 2.0f; else sV.y += MATH_FLOAT_PI / 2.0f;
											}
										}
										if (s_abCCW[i][2])
										{
											// fEuler < x < fEuler1
											if (fEuler[2] < fEuler1[2])
											{
												if (sV.z > 0) sV.z -= MATH_FLOAT_PI / 2.0f; else sV.z += MATH_FLOAT_PI / 2.0f;
											}
										}
										else
										{
											// fEuler > x > fEuler1
											if (fEuler[2] > fEuler1[2])
											{
												if (sV.z > 0) sV.z -= MATH_FLOAT_PI / 2.0f; else sV.z += MATH_FLOAT_PI / 2.0f;
											}
										}

										// set rotation matrix
										D3DXMatrixRotationYawPitchRoll(&sSkinned, sV.y, sV.x, sV.z);

										// add translation
										sSkinned(3, 0) = (asPoseDefault[i](3, 0) + asPoseFist[i](3, 0)) / 2.0f;
										sSkinned(3, 1) = (asPoseDefault[i](3, 1) + asPoseFist[i](3, 1)) / 2.0f;
										sSkinned(3, 2) = (asPoseDefault[i](3, 2) + asPoseFist[i](3, 2)) / 2.0f;
									}


									D3DXMATRIX sInvBind = data->asInverseBindPose[i];
									D3DXMATRIX sMeshPose = sInvBind * sSkinned;

									D3DXMatrixTranspose(&sMeshPose, &sMeshPose);
									m_sConstantsVS.meshPose[i] = sMeshPose;
								}

								// set rasterizer state... right hand has own state due to reversed triangles
								if (uHand)
									m_pcContextTemporary->RSSetState(m_pcRS_RightHand);
								else
									m_pcContextTemporary->RSSetState(m_pcRS);

								// Pass the world view position to the shader for view-dependent rendering
								m_sConstantsVS.viewPos = sVP;
								// Assign the vertex uniforms
								m_sConstantsVS.world = XMMatrixTranspose(worldMat);
								m_sConstantsVS.viewProj = XMMatrixTranspose(viewProjMat);
								m_pcContextTemporary->UpdateSubresource(m_pcCVSAvatar, 0, NULL, &m_sConstantsVS, 0, 0);
								// Write to depth first for self-occlusion
								m_pcContextTemporary->OMSetDepthStencilState(m_pcDSStateLess, 1);
								if (true) // self occluding
								{
									m_pcContextTemporary->OMSetRenderTargets(0, nullptr, m_pcDSVGeometry11[eye]);
									m_pcContextTemporary->DrawIndexed(data->unElementCount, 0, 0);
									m_pcContextTemporary->OMSetRenderTargets(1, &pcRTV, m_pcDSVGeometry11[eye]);
									m_pcContextTemporary->OMSetDepthStencilState(m_pcDSStateEqual, 1);
								}
								// draw
								m_pcContextTemporary->DrawIndexed(data->unElementCount, 0, 0);
							}
						}

						// set rasterizer to null
						m_pcContextTemporary->RSSetState(nullptr);
#endif
#pragma endregion
#pragma region hud
						// render hud ? only once
						if ((m_pbZoomOut) && (eye) && (pcSwapChain))
						{
							if (*m_pbZoomOut)
							{
								// render by copy recource
								ID3D11RenderTargetView* pcRTV = m_psEyeRenderTextureHUD->GetRTV();
								ID3D11Texture2D* pcTex = nullptr;
								pcRTV->GetResource((ID3D11Resource**)&pcTex);
								if (pcTex)
								{
									m_pcContextTemporary->CopyResource(pcTex, m_pcTex11SharedHUD);
									pcTex->Release();
								}
							}
						}
					}
				}

				if (pcResource) pcResource->Release();
			}
#pragma endregion

			// Commit rendering to the swap chain
			m_psEyeRenderTexture[eye]->Commit();
		}


		// init our layers, first our full screen Fov layer.
		ZeroMemory(&m_sLayerPrimal, sizeof(ovrLayerQuad));
		m_sLayerPrimal.Header.Type = ovrLayerType_EyeFov;
		m_sLayerPrimal.Header.Flags = 0;

		for (int eye = 0; eye < 2; ++eye)
		{
			m_sLayerPrimal.ColorTexture[eye] = m_psEyeRenderTexture[eye]->TextureChain;
			m_sLayerPrimal.Viewport[eye] = m_psEyeRenderViewport[eye];
			m_sLayerPrimal.Fov[eye] = m_sHMDDesc.DefaultEyeFov[eye];
			m_sLayerPrimal.RenderPose[eye] = asEyeRenderPose[eye];
			m_sLayerPrimal.SensorSampleTime = sensorSampleTime;
		}

		m_pasLayerList[OculusLayers::OculusLayer_MainEye] = &m_sLayerPrimal.Header;

		// add HUD layer only if swapchain (=d3d11) available
		UINT unLayerNumber = 1;
		if (pcSwapChain)
		{
			// next, our HUD layer
			float fMenuHudDistance = 500.0f;
			OVR::Recti sHudRenderedSize;
			sHudRenderedSize.w = 1920;
			sHudRenderedSize.h = 1080;
			sHudRenderedSize.x = 0;
			sHudRenderedSize.y = 0;
			ovrPosef sMenuPose;

			sMenuPose.Orientation = OVR::Quatf();
			sMenuPose.Position = OVR::Vector3f(0.0f, 0.0f, -fMenuHudDistance);

			// Assign HudLayer data.
			ZeroMemory(&m_sLayerHud, sizeof(ovrLayerQuad));
			m_sLayerHud.Header.Type = ovrLayerType_Quad;
			m_sLayerHud.Header.Flags = 0;
			m_sLayerHud.QuadPoseCenter = sMenuPose;
			m_sLayerHud.QuadSize = OVR::Vector2f((float)sHudRenderedSize.w, (float)sHudRenderedSize.h);
			m_sLayerHud.ColorTexture = m_psEyeRenderTextureHUD->TextureChain;
			m_sLayerHud.Viewport = (ovrRecti)sHudRenderedSize;

			// Grow the cliprect slightly to get a nicely-filtered edge.
			m_sLayerHud.Viewport.Pos.x -= 1;
			m_sLayerHud.Viewport.Pos.y -= 1;
			m_sLayerHud.Viewport.Size.w += 2;
			m_sLayerHud.Viewport.Size.h += 2;

			// IncrementSwapTextureSetIndex ( HudLayer.ColorTexture ) was done above when it was rendered.
			m_pasLayerList[OculusLayers::OculusLayer_Hud] = &m_sLayerHud.Header;

			if (m_pbZoomOut)
			{
				if (*m_pbZoomOut)
				{
					unLayerNumber = 2;
				}
			}
		}
		// and submit
		ovrResult result = ovr_SubmitFrame(*m_phHMD, 0, nullptr, m_pasLayerList, unLayerNumber);

		// Render mirror.. only available if D3D11
		if ((m_bShowMirror) && (pcSwapChain))
		{
			// copy the mirror texture to the shared texture
			ID3D11Texture2D* tex = nullptr;
			ovr_GetMirrorTextureBufferDX(*m_phHMD, m_pcMirrorTexture, IID_PPV_ARGS(&tex));
			m_pcContextTemporary->CopyResource(m_pcMirrorTextureD3D11HMD, tex);
			tex->Release();

			ID3D11Texture2D* pcBackBuffer = nullptr;
			pcSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pcBackBuffer);
			if (pcBackBuffer)
			{
				pcContext->CopyResource(pcBackBuffer, m_pcMirrorTextureD3D11);
				pcBackBuffer->Release();
			}
			else
				OutputDebugString(L"[OVR] No Back Buffer");

			if (!m_pcMirrorTextureD3D11) OutputDebugString(L"[OVR] No mirror texture!");
		}

		// is a swapchain connected to the device ? call present in case
		IDXGISwapChain* pcSwapchain = nullptr;
		UINT unSize = sizeof(pcSwapchain);
		pcDevice->GetPrivateData(PDIID_ID3D11Device_IDXGISwapChain, &unSize, &pcSwapchain);
		if (pcSwapchain) pcSwapchain->Present(0, 0);

#pragma endregion
	}

	// release d3d11 device + context... 
	pcContext->Release();
	pcDevice->Release();
	return nullptr;
}

#ifdef _WIN64 // TODO !! NO 32BIT SUPPORT FOR AVATAR SDK RIGHT NOW
/**
* Compute all world matrices.
***/
void OculusDirectMode::ComputeWorldPose(const ovrAvatarSkinnedMeshPose& sLocalPose, D3DXMATRIX* asWorldPose)
{
	for (uint32_t i = 0; i < sLocalPose.jointCount; ++i)
	{
		D3DXMATRIX sLocal;
		D3DMatrixFromOvrAvatarTransform(sLocalPose.jointTransform[i], &sLocal);

		int parentIndex = sLocalPose.jointParents[i];
		if (parentIndex < 0)
		{
			asWorldPose[i] = sLocal;
		}
		else
		{
			asWorldPose[i] = sLocal * asWorldPose[parentIndex];
		}
	}
}

/**
* Oculus mesh loader (D3D11).
**/
void OculusDirectMode::LoadMesh(ID3D11Device* pcDevice, const ovrAvatarMeshAssetData* data, MeshData* mesh)
{
	if (!mesh) return;

	// create the vertex buffer
	D3D11_BUFFER_DESC sDesc = {};
	sDesc.Usage = D3D11_USAGE_DEFAULT;
	sDesc.ByteWidth = sizeof(ovrAvatarMeshVertex)* data->vertexCount;
	sDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	sDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sInitData;
	ZeroMemory(&sInitData, sizeof(sInitData));
	sInitData.pSysMem = data->vertexBuffer;
	if (FAILED(pcDevice->CreateBuffer(&sDesc, &sInitData, &mesh->pcVertexBuffer)))
		OutputDebugString(L"[OVR] Failed to create vertex buffer.");

	// create index buffer
	D3D11_BUFFER_DESC sIBDesc = {};
	sIBDesc.Usage = D3D11_USAGE_DEFAULT;
	sIBDesc.ByteWidth = sizeof(WORD)* data->indexCount; // TODO !! UINT ??????
	sIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	sIBDesc.CPUAccessFlags = 0;
	ZeroMemory(&sInitData, sizeof(sInitData));
	sInitData.pSysMem = data->indexBuffer;
	if (FAILED(pcDevice->CreateBuffer(&sIBDesc, &sInitData, &mesh->pcElementBuffer)))
		OutputDebugString(L"[OVR] Failed to create index buffer.");
	mesh->unElementCount = data->indexCount;

	// Translate the bind pose
	ComputeWorldPose(data->skinnedBindPose, (D3DXMATRIX*)mesh->asBindPose);
	for (uint32_t i = 0; i < data->skinnedBindPose.jointCount; ++i)
		D3DXMatrixInverse((D3DXMATRIX*)&mesh->asInverseBindPose[i], NULL, (D3DXMATRIX*)&mesh->asBindPose[i]);

#ifdef _CREATE_MESH_FILES // if defined, outputs all mesh data to c++ ".h" files
	// get the file name
	std::ofstream ofMeshfile;
	std::stringstream strFileName;
	strFileName << "OculusMesh_" << data->vertexCount << ".h";

	// open file and set manipulators
	ofMeshfile.open(strFileName.str().c_str());
	ofMeshfile << std::fixed << std::setw(11) << std::setprecision(6);

	// write header
	ofMeshfile << "/********************************************************************\n";
	ofMeshfile << "Vireio Perception: Open-Source Stereoscopic 3D Driver\n";
	ofMeshfile << "Copyright (C) 2012 Andres Hernandez\n";

	ofMeshfile << "This file was exported from Vireio using Oculus Avatar SDK.\n";
	ofMeshfile << "Data Copyright : Copyright 2014-2016 Oculus VR, LLC All Rights reserved.\n\n";

	ofMeshfile << "Vireio Perception Version History :.\n";
	ofMeshfile << "v1.0.0 2012 by Andres Hernandez.\n";
	ofMeshfile << "v1.0.X 2013 by John Hicks, Neil Schneider.\n";
	ofMeshfile << "v1.1.x 2013 by Primary Coding Author : Chris Drain.\n";
	ofMeshfile << "Team Support : John Hicks, Phil Larkson, Neil Schneider.\n";
	ofMeshfile << "v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown.\n";
	ofMeshfile << "v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin.\n";
	ofMeshfile << "and Neil Schneider.\n\n";

	ofMeshfile << "This program is free software : you can redistribute it and / or modify.\n";
	ofMeshfile << "it under the terms of the GNU Lesser General Public License as published by.\n";
	ofMeshfile << "the Free Software Foundation, either version 3 of the License, or.\n";
	ofMeshfile << "(at your option) any later version..\n\n";

	ofMeshfile << "This program is distributed in the hope that it will be useful,.\n";
	ofMeshfile << "but WITHOUT ANY WARRANTY; without even the implied warranty of.\n";
	ofMeshfile << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the.\n";
	ofMeshfile << "GNU Lesser General Public License for more details..\n\n";

	ofMeshfile << "You should have received a copy of the GNU Lesser General Public License.\n";
	ofMeshfile << "along with this program.If not, see < http://www.gnu.org/licenses/>..\n";
	ofMeshfile << "********************************************************************/\n\n";

	// loop through vertices
	ofMeshfile << "const uint32_t unAvatarVertexCount_" << data->vertexCount << " = " << data->vertexCount << ";\n";
	ofMeshfile << "const ovrAvatarMeshVertex asAvatarVertices_" << data->vertexCount << "[] = {\n";
	for (uint16_t unIx = 0; unIx < data->vertexCount; unIx++)
	{
		ofMeshfile << "	{ " << data->vertexBuffer[unIx].x << "," << data->vertexBuffer[unIx].y << "," << data->vertexBuffer[unIx].z << ","
			<< data->vertexBuffer[unIx].nx << "," << data->vertexBuffer[unIx].ny << "," << data->vertexBuffer[unIx].nz << ","
			<< data->vertexBuffer[unIx].tx << "," << data->vertexBuffer[unIx].ty << "," << data->vertexBuffer[unIx].tz << "," << data->vertexBuffer[unIx].tw << ","
			<< data->vertexBuffer[unIx].u << "," << data->vertexBuffer[unIx].v << ","
			<< (UINT)data->vertexBuffer[unIx].blendIndices[0] << "," << (UINT)data->vertexBuffer[unIx].blendIndices[1] << "," << (UINT)data->vertexBuffer[unIx].blendIndices[2] << "," << (UINT)data->vertexBuffer[unIx].blendIndices[3] << ","
			<< data->vertexBuffer[unIx].blendWeights[0] << "," << data->vertexBuffer[unIx].blendWeights[1] << "," << data->vertexBuffer[unIx].blendWeights[2] << "," << data->vertexBuffer[unIx].blendWeights[3] << "}";
		if (unIx < (data->vertexCount - 1))
			ofMeshfile << ",\n";
		else
			ofMeshfile << "};\n\n";
	}
	// loop through indices
	ofMeshfile << "const uint32_t unAvatarIndexCount_" << data->vertexCount << " = " << data->indexCount << ";\n";
	ofMeshfile << "const uint32_t aunAvatarVertices_" << data->vertexCount << "[] = {\n";
	for (uint16_t unIx = 0; unIx < data->indexCount; unIx++)
	{
		ofMeshfile << data->indexBuffer[unIx];
		if (unIx < (data->indexCount - 1))
		{
			ofMeshfile << ",";
			if (unIx % 16 == 15)
				ofMeshfile << "\n";
		}
		else
			ofMeshfile << "};\n\n";
	}
	ofMeshfile.close();

	// get the file name
	strFileName = std::stringstream();
	strFileName << "OculusBindPose_" << data->vertexCount << ".h";

	// open file and set manipulators
	ofMeshfile.open(strFileName.str().c_str());
	ofMeshfile << std::fixed << std::setw(11) << std::setprecision(6);

	// write header
	ofMeshfile << "/********************************************************************\n";
	ofMeshfile << "Vireio Perception: Open-Source Stereoscopic 3D Driver\n";
	ofMeshfile << "Copyright (C) 2012 Andres Hernandez\n";

	ofMeshfile << "This file was exported from Vireio using Oculus Avatar SDK.\n";
	ofMeshfile << "Data Copyright : Copyright 2014-2016 Oculus VR, LLC All Rights reserved.\n\n";

	ofMeshfile << "Vireio Perception Version History :.\n";
	ofMeshfile << "v1.0.0 2012 by Andres Hernandez.\n";
	ofMeshfile << "v1.0.X 2013 by John Hicks, Neil Schneider.\n";
	ofMeshfile << "v1.1.x 2013 by Primary Coding Author : Chris Drain.\n";
	ofMeshfile << "Team Support : John Hicks, Phil Larkson, Neil Schneider.\n";
	ofMeshfile << "v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown.\n";
	ofMeshfile << "v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin.\n";
	ofMeshfile << "and Neil Schneider.\n\n";

	ofMeshfile << "This program is free software : you can redistribute it and / or modify.\n";
	ofMeshfile << "it under the terms of the GNU Lesser General Public License as published by.\n";
	ofMeshfile << "the Free Software Foundation, either version 3 of the License, or.\n";
	ofMeshfile << "(at your option) any later version..\n\n";

	ofMeshfile << "This program is distributed in the hope that it will be useful,.\n";
	ofMeshfile << "but WITHOUT ANY WARRANTY; without even the implied warranty of.\n";
	ofMeshfile << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the.\n";
	ofMeshfile << "GNU Lesser General Public License for more details..\n\n";

	ofMeshfile << "You should have received a copy of the GNU Lesser General Public License.\n";
	ofMeshfile << "along with this program.If not, see < http://www.gnu.org/licenses/>..\n";
	ofMeshfile << "********************************************************************/\n\n";

	// loop through vertices
	ofMeshfile << "const uint32_t unJointCount_" << data->vertexCount << " = " << data->skinnedBindPose.jointCount << ";\n";
	ofMeshfile << "const D3DXMATRIX asBindPose_" << data->vertexCount << "[] = {\n";
	for (uint16_t unIx = 0; unIx < data->skinnedBindPose.jointCount; unIx++)
	{
		ofMeshfile << "	{ " << mesh->asBindPose[unIx].m[0][0] << "f," << mesh->asBindPose[unIx].m[0][1] << "f," << mesh->asBindPose[unIx].m[0][2] << "f," << mesh->asBindPose[unIx].m[0][3] << "f,"
			<< mesh->asBindPose[unIx].m[1][0] << "f," << mesh->asBindPose[unIx].m[1][1] << "f," << mesh->asBindPose[unIx].m[1][2] << "f," << mesh->asBindPose[unIx].m[1][3] << "f,"
			<< mesh->asBindPose[unIx].m[2][0] << "f," << mesh->asBindPose[unIx].m[2][1] << "f," << mesh->asBindPose[unIx].m[2][2] << "f," << mesh->asBindPose[unIx].m[2][3] << "f,"
			<< mesh->asBindPose[unIx].m[3][0] << "f," << mesh->asBindPose[unIx].m[3][1] << "f," << mesh->asBindPose[unIx].m[3][2] << "f," << mesh->asBindPose[unIx].m[3][3] << "f"
			<< "}";
		if (unIx < (data->skinnedBindPose.jointCount - 1))
			ofMeshfile << ",\n";
		else
			ofMeshfile << "};\n\n";
	}
	ofMeshfile.close();
#endif
}

/**
* Loads a D3D11 texture from Oculus Avatar texture asset data.
***/
void OculusDirectMode::LoadTexture(ID3D11Device* pcDevice, const ovrAvatarTextureAssetData* data, TextureData* texture)
{
	if (!texture) return;
	DXGI_FORMAT eFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Load the image data
	switch (data->format)
	{
		// Handle uncompressed image data
		case ovrAvatarTextureFormat_RGB24:
			if (true)
			{
				// convert RGB->RGBA
				BYTE* acData = new BYTE[data->sizeX * data->sizeY * sizeof(BYTE)* 4];
				for (UINT unOffset = 0; unOffset < data->sizeX * data->sizeY; unOffset++)
				{
					acData[unOffset * 4] = data->textureData[unOffset * 3];
					acData[unOffset * 4 + 1] = data->textureData[unOffset * 3 + 1];
					acData[unOffset * 4 + 2] = data->textureData[unOffset * 3 + 2];
					acData[unOffset * 4 + 3] = 255;
				}

				// create geometry texture
				D3D11_TEXTURE2D_DESC sDesc = {};
				ZeroMemory(&sDesc, sizeof(sDesc));
				sDesc.Width = data->sizeX;
				sDesc.Height = data->sizeY;
				sDesc.MipLevels = 1; //data->mipCount; // TODO !! HANDLE MIPS
				sDesc.ArraySize = 1;
				sDesc.Format = eFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				sDesc.SampleDesc.Count = 1;
				sDesc.Usage = D3D11_USAGE_DEFAULT;
				sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				D3D11_SUBRESOURCE_DATA sData;
				ZeroMemory(&sData, sizeof(sData));
				sData.pSysMem = acData;
				sData.SysMemPitch = data->sizeX * 4;
				if (FAILED(pcDevice->CreateTexture2D(&sDesc, &sData, &texture->pcTexture)))
					OutputDebugString(L"[OVR] Failed to create model texture DXGI_FORMAT_R8G8B8A8_UNORM.");
				else
					OutputDebugString(L"[OVR] Created Avatar Texture DXGI_FORMAT_R8G8B8A8_UNORM");

				delete[] acData;
			}
			break;

			// Handle compressed image data
		case ovrAvatarTextureFormat_DXT1:
		case ovrAvatarTextureFormat_DXT5:
			if (true)
			{
				// create geometry texture
				if (data->format == ovrAvatarTextureFormat_DXT1)
					eFormat = DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM;
				else
					eFormat = DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM;

				// create a dds file in memory... code from https://github.com/ClemensX/ShadedPath12/blob/master/ShadedPath12/ShadedPath12/vr.cpp
				DDS_HEADER sHeader = {};
				int nBlockSize;
				if (data->format == ovrAvatarTextureFormat_DXT1)
				{
					nBlockSize = 8;
					sHeader.ddspf.dwFourCC = 0x31545844;
				}
				else
				{
					nBlockSize = 16;
					sHeader.ddspf.dwFourCC = 0x35545844;
				}
				sHeader.dwSize = 0x7c;
				sHeader.dwFlags = 0xa1007;
				sHeader.dwHeight = data->sizeY;
				sHeader.dwWidth = data->sizeX;
				sHeader.dwPitchOrLinearSize = nBlockSize * max(1, ((sHeader.dwWidth + 3) / 4)) * max(1, ((sHeader.dwHeight + 3) / 4));
				sHeader.dwDepth = 1;
				sHeader.dwMipMapCount = data->mipCount;
				sHeader.dwCaps = 0x401008;

				sHeader.ddspf.dwSize = 0x20;
				sHeader.ddspf.dwFlags = 0x04;

				// create memory file stream
				std::stringstream oMemoryFileStream;
				/*std::ofstream oMemoryFileStream;
				oMemoryFileStream.open("TEXTEST.dds");*/
				uint32_t dwMagicNumber = DDS_MAGIC;
				oMemoryFileStream.write((const char*)&dwMagicNumber, 4);
				oMemoryFileStream.write((const char*)&sHeader, sizeof(sHeader));

				int total = 0;
				for (uint32_t level = 0, offset = 0, width = data->sizeX, height = data->sizeY; level < data->mipCount; ++level)
				{
					// int levelSize = blockSize * (width / 4) * (height / 4);
					int levelSize = nBlockSize * max(1, ((width + 3) / 4)) * max(1, ((height + 3) / 4));

					total += levelSize;
					const char *mem = ((const char*)data->textureData) + offset;
					oMemoryFileStream.write(mem, levelSize);
					offset += levelSize;
					width /= 2;
					height /= 2;
				}
				//oMemoryFileStream.close();
				HRESULT nHr = S_OK;
				D3DX11CreateTextureFromMemory(pcDevice, oMemoryFileStream.str().c_str(), oMemoryFileStream.str().length(), NULL, NULL, (ID3D11Resource**)&texture->pcTexture, &nHr);
				if (FAILED(nHr))
				{
					if (data->format == ovrAvatarTextureFormat_DXT1)
						OutputDebugString(L"[OVR] Failed to create model texture. DXGI_FORMAT_BC1_UNORM");
					else
						OutputDebugString(L"[OVR] Failed to create model texture. DXGI_FORMAT_BC3_UNORM");
				}
				else
				{
					if (data->format == ovrAvatarTextureFormat_DXT1)
						OutputDebugString(L"[OVR] Created Avatar Texture DXGI_FORMAT_BC1_UNORM");
					else
						OutputDebugString(L"[OVR] Created Avatar Texture DXGI_FORMAT_BC3_UNORM");
				}
			}
			break;
	}

	if (texture->pcTexture)
	{
		// create texture shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc = {};
		sDesc.Format = eFormat;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.Texture2D.MipLevels = 1; // data->mipCount; // TODO !! MIP COUNT

		if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)texture->pcTexture, &sDesc, &texture->pcSRV))))
			OutputDebugString(L"[OVR] Failed to create model texture shader resource view!");
	}
}

/**
* Sets vertex shader constants.
***/
void OculusDirectMode::SetMeshState(const ovrAvatarTransform& localTransform,
	const MeshData* data,
	const ovrAvatarSkinnedMeshPose& skinnedPose,
	const XMMATRIX world,
	const XMMATRIX view,
	const XMMATRIX proj,
	const XMVECTOR viewPos
	)
{
	// Compute the final world and viewProjection matrices for this part
	XMMATRIX local;
	D3DMatrixFromOvrAvatarTransform(localTransform, &local);
	XMMATRIX worldMat = local;//world * local;
	XMMATRIX viewProjMat = view * proj;

	// Compute the skinned pose
	static D3DXMATRIX skinnedPoses[OVR_AVATAR_MAXIMUM_JOINT_COUNT];
	ComputeWorldPose(skinnedPose, skinnedPoses);
	for (uint32_t i = 0; i < skinnedPose.jointCount; ++i)
	{
		D3DXMATRIX sSkinned = skinnedPoses[i];
		D3DXMATRIX sInvBind = data->asInverseBindPose[i];
		D3DXMATRIX sMeshPose = sInvBind * sSkinned;

		D3DXMatrixTranspose(&sMeshPose, &sMeshPose);
		m_sConstantsVS.meshPose[i] = sMeshPose;
	}

	// Pass the world view position to the shader for view-dependent rendering
	m_sConstantsVS.viewPos = viewPos;
	// Assign the vertex uniforms
	m_sConstantsVS.world = XMMatrixTranspose(worldMat);
	m_sConstantsVS.viewProj = XMMatrixTranspose(viewProjMat);
}
#endif
/**
* Sets fragment shader constants.
**/
void OculusDirectMode::SetMaterialState(const ovrAvatarMaterialState* state, XMMATRIX* projectorInv)
{
	// Assign the fragment uniforms
	m_sConstantsFS.useAlpha = state->alphaMaskTextureID != 0;
	m_sConstantsFS.useNormalMap = state->normalMapTextureID != 0;
	m_sConstantsFS.useRoughnessMap = state->roughnessMapTextureID != 0;

	// Update and set time
	static float fTime = 0.0f;
	static DWORD unTimeStart = 0;
	DWORD unTimeCurrent = GetTickCount();
	if (unTimeStart == 0)
		unTimeStart = unTimeCurrent;
	fTime = (unTimeCurrent - unTimeStart) / 1000.0f;
	m_sConstantsFS.elapsedSeconds = fTime;

	if (projectorInv)
	{
		m_sConstantsFS.useProjector = 1;
		m_sConstantsFS.projectorInv = *projectorInv;
	}
	else
	{
		m_sConstantsFS.useProjector = 0;
	}

	// set base params
	m_sConstantsFS.baseColor = XMFLOAT4(&state->baseColor.x);
	m_sConstantsFS.baseMaskType = state->baseMaskType;
	m_sConstantsFS.baseMaskParameters = XMFLOAT4(&state->baseMaskParameters.x);
	m_sConstantsFS.baseMaskAxis = XMFLOAT4(&state->baseMaskAxis.x);

	// set texture SRVs
	UINT textureSlot = 0;
	if (state->alphaMaskTextureID)
	{
		uint32_t uIndex = m_auAssetIndexMap[state->alphaMaskTextureID];
		if (uIndex)
		{
			AvatarData* data = &m_asAssetMap[uIndex];
			if (data)
			{
				TextureData* psTexData = &data->sTexture;
				if (psTexData->pcSRV)
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, &psTexData->pcSRV);
				else
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, nullptr);
			}
		}
	}
	textureSlot++;
	m_sConstantsFS.alphaMaskScaleOffset = XMFLOAT4(&state->alphaMaskScaleOffset.x);
	if (state->normalMapTextureID)
	{
		uint32_t uIndex = m_auAssetIndexMap[state->alphaMaskTextureID];
		if (uIndex)
		{
			AvatarData* data = &m_asAssetMap[uIndex];
			if (data)
			{
				TextureData* psTexData = &data->sTexture;
				if (psTexData->pcSRV)
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, &psTexData->pcSRV);
				else
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, nullptr);
			}
		}
	}
	textureSlot++;
	m_sConstantsFS.normalMapScaleOffset = XMFLOAT4(&state->normalMapScaleOffset.x);
	if (state->parallaxMapTextureID)
	{
		uint32_t uIndex = m_auAssetIndexMap[state->alphaMaskTextureID];
		if (uIndex)
		{
			AvatarData* data = &m_asAssetMap[uIndex];
			if (data)
			{
				TextureData* psTexData = &data->sTexture;
				if (psTexData->pcSRV)
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, &psTexData->pcSRV);
				else
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, nullptr);
			}
		}
	}
	textureSlot++;
	m_sConstantsFS.parallaxMapScaleOffset = XMFLOAT4(&state->parallaxMapScaleOffset.x);
	if (state->roughnessMapTextureID)
	{
		uint32_t uIndex = m_auAssetIndexMap[state->alphaMaskTextureID];
		if (uIndex)
		{
			AvatarData* data = &m_asAssetMap[uIndex];
			if (data)
			{
				TextureData* psTexData = &data->sTexture;
				if (psTexData->pcSRV)
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, &psTexData->pcSRV);
				else
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, nullptr);
			}
		}
	}
	textureSlot++;
	m_sConstantsFS.roughnessMapScaleOffset = XMFLOAT4(&state->roughnessMapScaleOffset.x);

	struct LayerUniforms
	{
		int layerSamplerModes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		int layerBlendModes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		int layerMaskTypes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarVector4f layerColors[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		int layerSurfaces[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarAssetID layerSurfaceIDs[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarVector4f layerSurfaceScaleOffsets[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarVector4f layerSampleParameters[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarVector4f layerMaskParameters[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
		ovrAvatarVector4f layerMaskAxes[OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT];
	} layerUniforms;
	memset(&layerUniforms, 0, sizeof(layerUniforms));
	for (uint32_t i = 0; i < state->layerCount; ++i)
	{
		const ovrAvatarMaterialLayerState& layerState = state->layers[i];
		layerUniforms.layerSamplerModes[i] = layerState.sampleMode;
		layerUniforms.layerBlendModes[i] = layerState.blendMode;
		layerUniforms.layerMaskTypes[i] = layerState.maskType;
		layerUniforms.layerColors[i] = layerState.layerColor;
		uint32_t uIndex = m_auAssetIndexMap[layerState.sampleTexture];
		if (uIndex)
		{
			AvatarData* data = &m_asAssetMap[uIndex];
			if (data)
			{
				TextureData* psTexData = &data->sTexture;
				if (psTexData->pcSRV)
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, &psTexData->pcSRV);
				else
					m_pcContextTemporary->PSSetShaderResources(textureSlot, 1, nullptr);
			}
		}
		textureSlot++;
		layerUniforms.layerSurfaceScaleOffsets[i] = layerState.sampleScaleOffset;
		layerUniforms.layerSampleParameters[i] = layerState.sampleParameters;
		layerUniforms.layerMaskParameters[i] = layerState.maskParameters;
		layerUniforms.layerMaskAxes[i] = layerState.maskAxis;
	}

	m_sConstantsFS.layerCount = state->layerCount;
	memcpy(m_sConstantsFS.layerSamplerModes, layerUniforms.layerSamplerModes, OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT*sizeof(int));
	memcpy(m_sConstantsFS.layerBlendModes, layerUniforms.layerBlendModes, OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT*sizeof(int));
	memcpy(m_sConstantsFS.layerMaskTypes, layerUniforms.layerMaskTypes, OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT*sizeof(int));
	for (UINT unIx = 0; unIx < OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT; unIx++)
		m_sConstantsFS.layerColors[unIx] = XMFLOAT4(&layerUniforms.layerColors[unIx].x);
	for (UINT unIx = 0; unIx < OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT; unIx++)
		m_sConstantsFS.layerSurfaceScaleOffsets[unIx] = XMFLOAT4(&layerUniforms.layerSurfaceScaleOffsets[unIx].x);
	for (UINT unIx = 0; unIx < OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT; unIx++)
		m_sConstantsFS.layerSampleParameters[unIx] = XMFLOAT4(&layerUniforms.layerSampleParameters[unIx].x);
	for (UINT unIx = 0; unIx < OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT; unIx++)
		m_sConstantsFS.layerMaskParameters[unIx] = XMFLOAT4(&layerUniforms.layerMaskParameters[unIx].x);
	for (UINT unIx = 0; unIx < OVR_AVATAR_MAX_MATERIAL_LAYER_COUNT; unIx++)
		m_sConstantsFS.layerMaskAxes[unIx] = XMFLOAT4(&layerUniforms.layerMaskAxes[unIx].x);
}

