/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Tracker - Oculus Rift Motion Tracker Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusTracker.cpp> and
Class <OculusTracker> :
Copyright (C) 2015 Denis Reischl

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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"OculusTracker.h"

#define INTERFACE_IDIRECT3DDEVICE9 8
#define INTERFACE_IDIRECT3DSWAPCHAIN9 15

/**
* Constructor.
***/
OculusTracker::OculusTracker():AQU_Nodus(),
	m_hBitmapControl(nullptr),
	m_bControlUpdate(false),
	m_hFont(nullptr),
	m_pcDistortionVertexBufferLeft(nullptr),
	m_pcDistortionVertexBufferRight(nullptr),
	m_pcDistortionIndexBufferLeft(nullptr),
	m_pcDistortionIndexBufferRight(nullptr),
	m_pcVertexDecl(nullptr),
	m_nRenderTextureWidth(0),
	m_nRenderTextureHeight(1),
	m_hHMD(nullptr)
{
	for (int i = 0; i < NUMBER_OF_COMMANDERS; i++)
		m_paOutput[i] = nullptr;

	ZeroMemory(&m_sDefaultFOVMatrixProjLeft, sizeof(D3DMATRIX));
	ZeroMemory(&m_sDefaultFOVMatrixProjRight, sizeof(D3DMATRIX));
}

/**
* Destructor.
***/
OculusTracker::~OculusTracker()
{
	if (m_hHMD) ovrHmd_Destroy(m_hHMD);
	if (m_hBitmapControl) CloseHandle(m_hBitmapControl);
	if (m_hFont) CloseHandle(m_hFont);

	if (m_pcDistortionVertexBufferLeft) m_pcDistortionVertexBufferLeft->Release();
	if (m_pcDistortionVertexBufferRight) m_pcDistortionVertexBufferRight->Release();
	if (m_pcDistortionIndexBufferLeft) m_pcDistortionIndexBufferLeft->Release();
	if (m_pcDistortionIndexBufferRight) m_pcDistortionIndexBufferRight->Release();
	if (m_pcVertexDecl) m_pcVertexDecl->Release();
}

/**
* Return the name of the  Oculus Tracker node.
***/
const char* OculusTracker::GetNodeType() 
{
	return "Oculus Tracker"; 
}

/**
* Returns a global unique identifier for the Oculus Tracker node.
***/
UINT OculusTracker::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 256
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Oculus Tracker node.
***/
LPWSTR OculusTracker::GetCategory()
{
	return L"Motion Tracker";
}

/**
* Returns a logo to be used for the Oculus Tracker node.
***/
HBITMAP OculusTracker::GetLogo() 
{ 
	HMODULE hModule = GetModuleHandle(L"OculusTracker.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/** 
* Returns the updated control for the Oculus Tracker node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OculusTracker::GetControl()
{
	if (!m_hBitmapControl)
	{
		// create bitmap, set control update to true
		HWND hwnd = GetActiveWindow();
		HDC hdc = GetDC(hwnd);
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 2200);
		if (!m_hBitmapControl)
			OutputDebugString(L"Failed to create bitmap!");
		m_bControlUpdate = true;
	}

	if (m_bControlUpdate)
	{
		// get control bitmap dc
		HDC hdcImage = CreateCompatibleDC(NULL);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, m_hBitmapControl);
		HFONT hOldFont; 

		// clear the background
		RECT rc;
		SetRect(&rc, 0, 0, 1024, 2200);
		FillRect(hdcImage, &rc, (HBRUSH)CreateSolidBrush(RGB(10,92,10)));

		// create font
		if (!m_hFont)
			m_hFont = CreateFont(64, 0, 0, 0, 0, FALSE,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			L"Comic Sans MS");

		// Select the variable stock font into the specified device context. 
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (m_hHMD))
		{
			int nY = 16;
			char szBuffer[256];

			SetTextColor(hdcImage, RGB(240,240,240));
			SetBkColor(hdcImage, RGB(10,92,10));

			// display the values suiteable to the data commanders... first yaw pitch roll
			sprintf(szBuffer, "%4.6f",m_fYaw);
			TextOut(hdcImage, 880, nY, L"Yaw", 3);
			int nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_fPitch);
			TextOut(hdcImage, 850, nY, L"Pitch", 5);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_fRoll);
			TextOut(hdcImage, 870, nY, L"Roll", 4);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;

			// orientation 
			sprintf(szBuffer, "%4.6f",m_sPose.Orientation.x);
			TextOut(hdcImage, 670, nY, L"Orientation X", 13);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_sPose.Orientation.y);
			TextOut(hdcImage, 670, nY, L"Orientation Y", 13);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_sPose.Orientation.z);
			TextOut(hdcImage, 670, nY, L"Orientation Z", 13);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_sPose.Orientation.w);
			TextOut(hdcImage, 670, nY, L"Orientation W", 13);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;

			// position tracking
			sprintf(szBuffer, "%4.6f",m_sPose.Position.x);
			TextOut(hdcImage, 750, nY, L"Position X", 10);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_sPose.Position.y);
			TextOut(hdcImage, 750, nY, L"Position Y", 10);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+=64;
			sprintf(szBuffer, "%4.6f",m_sPose.Position.z);
			TextOut(hdcImage, 750, nY, L"Position Z", 10);
			nLen = (int)strlen(szBuffer); if (nLen > 11) nLen = 11;
			TextOutA(hdcImage, 392, nY, szBuffer, nLen); nY+= 64;

			// camera frustum
			wchar_t szBufferW[128];
			if (m_hHMD)
				wsprintf(szBufferW, L"%u",m_hHMD->CameraFrustumHFovInRadians);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY,  L"CameraFrustumHFovInRadians", 25);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u",m_hHMD->CameraFrustumVFovInRadians);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY,  L"CameraFrustumVFovInRadians", 25);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u",m_hHMD->CameraFrustumNearZInMeters);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 300, nY,  L"CameraFrustumNearZInMeters", 26);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u",m_hHMD->CameraFrustumFarZInMeters);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY,  L"CameraFrustumFarZInMeters", 25);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;

			// caps
			TextOut(hdcImage, 770, nY,  L"HmdCaps", 7); nY+=64;
			TextOut(hdcImage, 690, nY,  L"TrackingCaps", 12); nY+=64;
			TextOut(hdcImage, 660, nY,  L"DistortionCaps", 14); nY+=64;

			// resolution
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_nRenderTextureWidth);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 510, nY,  L"TexResolution Width", 19);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_nRenderTextureHeight);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 500, nY,  L"TexResolution Height", 20);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY+=64;

			// distortion vertex and index buffers
			TextOut(hdcImage, 300, nY,  L"Distortion Vertex Buffer Left", 29); nY+=64;
			TextOut(hdcImage, 300, nY,  L"Distortion Vertex Buffer Right", 30); nY+=64;
			TextOut(hdcImage, 300, nY,  L"Distortion Index Buffer Left", 28); nY+=64;
			TextOut(hdcImage, 300, nY,  L"Distortion Index Buffer Right", 29); nY+=64;

			// oculus vertex declaration
			TextOut(hdcImage, 400, nY,  L"Oculus Vertex Declaration", 25); nY+=64;

			// FOV
			TextOut(hdcImage, 520, nY, L"Default EyeFov Left", 19); nY+=64;
			TextOut(hdcImage, 520, nY, L"Default EyeFov Right", 20); nY+=64;
			TextOut(hdcImage, 570, nY, L"MaxEye Fov Left", 15); nY+=64;
			TextOut(hdcImage, 570, nY, L"MaxEye Fov Right", 16); nY+=64;

			// HMD to Eye view offset
			TextOut(hdcImage, 300, nY, L"Hmd To Eye View Offset Left", 27); nY+=64;
			TextOut(hdcImage, 300, nY, L"Hmd To Eye View Offset Right", 28); nY+=64;

			// Default FOV projection matrix
			TextOut(hdcImage, 150, nY, L"Default FOV Projection Matrix Left", 34); nY+=64;
			TextOut(hdcImage, 150, nY, L"Default FOV Projection Matrix Right", 35); nY+=64;

			// Display the text string for the provoker
			/*TextOutA(hdcImage, 50, nY, m_hHMD->ProductName, strlen(m_hHMD->ProductName)); 
			wsprintf(szBufferW, L"w:%u h:%u", m_hHMD->Resolution.w, m_hHMD->Resolution.h);
			nLen = (int)wcslen(szBufferW); 
			TextOut(hdcImage, 500, nY, szBufferW, nLen);*/
			TextOut(hdcImage, 50, nY, L"Oculus Rift Tracker", 19); nY+=64;


			// Restore the original font.        
			SelectObject(hdcImage, hOldFont); 
		}

		SelectObject(hdcImage, hbmOld);
		DeleteDC(hdcImage);

		// next update only by request, return updated bitmap
		m_bControlUpdate = false;
		return m_hBitmapControl;
	}
	else
		return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR OculusTracker::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((OTR_Commanders)dwCommanderIndex)
	{
	case OTR_Commanders::Yaw:
		return L"Yaw";
	case OTR_Commanders::Pitch:
		return L"Pitch";
	case OTR_Commanders::Roll:
		return L"Roll";
	case OTR_Commanders::OrientationX:
		return L"Orientation X";
	case OTR_Commanders::OrientationY:
		return L"Orientation Y";
	case OTR_Commanders::OrientationZ:
		return L"Orientation Z";
	case OTR_Commanders::OrientationW:
		return L"Orientation W";
	case OTR_Commanders::PositionX:
		return L"Position X";
	case OTR_Commanders::PositionY:
		return L"Position Y";
	case OTR_Commanders::PositionZ:
		return L"Position Z";
	case OTR_Commanders::CameraFrustumHFovInRadians:
		return L"CameraFrustumHFovInRadians";
	case OTR_Commanders::CameraFrustumVFovInRadians:
		return L"CameraFrustumVFovInRadians";
	case OTR_Commanders::CameraFrustumNearZInMeters:
		return L"CameraFrustumNearZInMeters";
	case OTR_Commanders::CameraFrustumFarZInMeters:
		return L"CameraFrustumFarZInMeters";
	case OTR_Commanders::HmdCaps:
		return L"HmdCaps";
	case OTR_Commanders::TrackingCaps:
		return L"TrackingCaps";
	case OTR_Commanders::DistortionCaps:
		return L"DistortionCaps";
	case OTR_Commanders::ResolutionW:
		return L"Texture Resolution Width";
	case OTR_Commanders::ResolutionH:
		return L"Texture Resolution Height";
	case OTR_Commanders::DistortionVertexBufferLeft:
		return L"Distortion Vertex Buffer Left";
	case OTR_Commanders::DistortionVertexBufferRight:
		return L"Distortion Vertex Buffer Right";
	case OTR_Commanders::DistortionIndexBufferLeft:
		return L"Distortion Index Buffer Left";
	case OTR_Commanders::DistortionIndexBufferRight:
		return L"Distortion Index Buffer Right";
	case OTR_Commanders::OculusVertexDeclaration:
		return L"Oculus Vertex Declaration";
	case OTR_Commanders::DefaultEyeFovLeft:
		return L"Default Eye Fov Left";
	case OTR_Commanders::DefaultEyeFovRight:
		return L"Default Eye Fov Right";
	case OTR_Commanders::MaxEyeFovLeft:
		return L"Max Eye Fov Left";
	case OTR_Commanders::MaxEyeFovRight:
		return L"Max Eye Fov Right";
	case OTR_Commanders::HmdToEyeViewOffsetLeft:
		return L"HMD to Eye View Offset Left";
	case OTR_Commanders::HmdToEyeViewOffsetRight:
		return L"HMD to Eye View Offset Right";
	case OTR_Commanders::DefaultProjectionMatrixLeft:
		return L"Default FOV Projection Matrix Left";
	case OTR_Commanders::DefaultProjectionMatrixRight:
		return L"Default FOV Projection Matrix Right";
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OculusTracker::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((OTR_Commanders)dwCommanderIndex)
	{
	case OTR_Commanders::Yaw:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::Pitch:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::Roll:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::OrientationX:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::OrientationY:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::OrientationZ:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::OrientationW:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::PositionX:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::PositionY:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::PositionZ:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::CameraFrustumHFovInRadians:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::CameraFrustumVFovInRadians:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::CameraFrustumNearZInMeters:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::CameraFrustumFarZInMeters:
		return PNT_FLOAT_PLUG_TYPE;
	case OTR_Commanders::HmdCaps:
		return PNT_UINT_PLUG_TYPE;
	case OTR_Commanders::TrackingCaps:
		return PNT_UINT_PLUG_TYPE;
	case OTR_Commanders::DistortionCaps:
		return PNT_UINT_PLUG_TYPE;
	case OTR_Commanders::ResolutionW:
		return PNT_INT_PLUG_TYPE;
	case OTR_Commanders::ResolutionH:
		return PNT_INT_PLUG_TYPE;
	case OTR_Commanders::DistortionVertexBufferLeft:
		return PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE;
	case OTR_Commanders::DistortionVertexBufferRight:
		return PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE;
	case OTR_Commanders::DistortionIndexBufferLeft:
		return PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE;
	case OTR_Commanders::DistortionIndexBufferRight:
		return PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE;
	case OTR_Commanders::OculusVertexDeclaration:
		return PPNT_IDIRECT3DVERTEXDECLARATION9_PLUG_TYPE;
	case OTR_Commanders::DefaultEyeFovLeft:
		return PNT_VECTOR4F_PLUG_TYPE;
	case OTR_Commanders::DefaultEyeFovRight:
		return PNT_VECTOR4F_PLUG_TYPE;
	case OTR_Commanders::MaxEyeFovLeft:
		return PNT_VECTOR4F_PLUG_TYPE;
	case OTR_Commanders::MaxEyeFovRight:
		return PNT_VECTOR4F_PLUG_TYPE;
	case OTR_Commanders::HmdToEyeViewOffsetLeft:
		return PNT_VECTOR3F_PLUG_TYPE;
	case OTR_Commanders::HmdToEyeViewOffsetRight:
		return PNT_VECTOR3F_PLUG_TYPE;
	case OTR_Commanders::DefaultProjectionMatrixLeft:
		return PNT_D3DMATRIX_PLUG_TYPE;
	case OTR_Commanders::DefaultProjectionMatrixRight:
		return PNT_D3DMATRIX_PLUG_TYPE;
	}

	return 0;
}

/**
* Provides the pointer of the requested commander.
* Always return a pointer to the output pointer
* of a class field not created at startup !
***/
void* OculusTracker::GetOutputPointer(DWORD dwCommanderIndex)
{	
	if (dwCommanderIndex < NUMBER_OF_COMMANDERS)
		return (void*)&m_paOutput[dwCommanderIndex];

	return nullptr;	
}

/**
* Feel free to connect the Oculus Tracker to any Direct3D 9 device or swapchain node.
* Device needed to create the oculus distortion vertex buffer.
***/
bool OculusTracker::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  
{ 
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if ((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) || (nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9))
			return true;
	}
	return false; 
}

/**
* Render the Virtual Cinema Theatre.
***/
void* OculusTracker::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	
{
	if (m_hHMD)
	{
		// Start the sensor which informs of the Rift's pose and motion
		ovrHmd_ConfigureTracking(m_hHMD, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);

		// get the current tracking state
		ovrTrackingState sTrackingState = ovrHmd_GetTrackingState(m_hHMD, ovr_GetTimeInSeconds());

		if (sTrackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{
			// get pose
			ovrPoseStatef sPoseState = sTrackingState.HeadPose;
			m_sPose = sPoseState.ThePose;
			m_sOrientation = m_sPose.Orientation;

			// get angles
			m_sOrientation.GetEulerAngles<OVR::Axis::Axis_Y, OVR::Axis::Axis_X, OVR::Axis::Axis_Z>(&m_fYaw, &m_fPitch, &m_fRoll);

			// set the drawing update to true
			m_bControlUpdate = true;
		}

		// create the distortion vertex buffers for the left and right eye
		if (!m_pcDistortionVertexBufferLeft)
		{
			// get device 
			LPDIRECT3DDEVICE9 pDevice = nullptr;			
			bool bReleaseDevice = false;
			if (eD3DInterface == INTERFACE_IDIRECT3DDEVICE9)
			{
				pDevice = (LPDIRECT3DDEVICE9)pThis;
			}
			else if (eD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
			{
				LPDIRECT3DSWAPCHAIN9 pSwapChain = (LPDIRECT3DSWAPCHAIN9)pThis;
				if (!pSwapChain) 
				{
					OutputDebugString(L"VRCinemaNodus_D3D9 : No swapchain !");
					return nullptr;
				}
				pSwapChain->GetDevice(&pDevice);
				bReleaseDevice = true;
			}
			if (!pDevice)
			{
				OutputDebugString(L"VRCinemaNodus_D3D9 Node : No device !");
				return nullptr;
			}

			// create eye render descriptions
			m_sEyeRenderDescLeft = ovrHmd_GetRenderDesc(m_hHMD, ovrEyeType::ovrEye_Left, m_hHMD->DefaultEyeFov[ovrEyeType::ovrEye_Left]);
			m_sEyeRenderDescRight = ovrHmd_GetRenderDesc(m_hHMD, ovrEyeType::ovrEye_Right, m_hHMD->DefaultEyeFov[ovrEyeType::ovrEye_Right]);

			// Oculus Rift distortion mesh : 

			// typedef struct ovrDistortionVertex_
			// {
			//      ovrVector2f ScreenPosNDC;    ///< [-1,+1],[-1,+1] over the entire framebuffer.
			//      float       TimeWarpFactor;  ///< Lerp factor between time-warp matrices. Can be encoded in Pos.z.
			//      float       VignetteFactor;  ///< Vignette fade factor. Can be encoded in Pos.w.
			//      ovrVector2f TanEyeAnglesR;   ///< The tangents of the horizontal and vertical eye angles for the red channel.
			//      ovrVector2f TanEyeAnglesG;   ///< The tangents of the horizontal and vertical eye angles for the green channel.
			//      ovrVector2f TanEyeAnglesB;   ///< The tangents of the horizontal and vertical eye angles for the blue channel.
			// } ovrDistortionVertex;

			// create the distortion mesh
			ovrDistortionMesh meshDataLeft;
			ovrHmd_CreateDistortionMesh(m_hHMD,ovrEyeType::ovrEye_Left, m_sEyeRenderDescLeft.Fov, m_hHMD->DistortionCaps, &meshDataLeft);
			ovrDistortionMesh meshDataRight;
			ovrHmd_CreateDistortionMesh(m_hHMD,ovrEyeType::ovrEye_Right, m_sEyeRenderDescRight.Fov, m_hHMD->DistortionCaps, &meshDataRight);

			// create the vertex declaration
			pDevice->CreateVertexDeclaration( VertexElements, &m_pcVertexDecl );

			// create left eye vertex buffer
			if (SUCCEEDED(pDevice->CreateVertexBuffer(meshDataLeft.VertexCount*sizeof(ovrDistortionVertex), 0, 0, D3DPOOL_DEFAULT, &m_pcDistortionVertexBufferLeft, NULL ) ) )
			{
				ovrDistortionVertex * dxv; 	
				if (SUCCEEDED(m_pcDistortionVertexBufferLeft->Lock( 0, 0, (void**)&dxv, 0 )))
				{
					for (DWORD v = 0; v < meshDataLeft.VertexCount; v++) 
						dxv[v] = meshDataLeft.pVertexData[v];

					m_pcDistortionVertexBufferLeft->Unlock();
				}
				else 
					OutputDebugString(L"OculusTracker : Failed to lock vertex buffer !");
			}
			else 
				OutputDebugString(L"OculusTracker : Failed to create vertex buffer !");

			// create right eye vertex buffer
			if (SUCCEEDED(pDevice->CreateVertexBuffer(meshDataRight.VertexCount*sizeof(ovrDistortionVertex), 0, 0, D3DPOOL_DEFAULT, &m_pcDistortionVertexBufferRight, NULL ) ) )
			{
				ovrDistortionVertex * dxv; 	
				if (SUCCEEDED(m_pcDistortionVertexBufferRight->Lock( 0, 0, (void**)&dxv, 0 )))
				{
					for (DWORD v = 0; v < meshDataRight.VertexCount; v++) 
						dxv[v] = meshDataRight.pVertexData[v];

					m_pcDistortionVertexBufferRight->Unlock();
				}
				else 
					OutputDebugString(L"OculusTracker : Failed to lock vertex buffer !");
			}
			else 
				OutputDebugString(L"OculusTracker : Failed to create vertex buffer !");

			// create left eye index buffer
			if (SUCCEEDED(pDevice->CreateIndexBuffer( (meshDataLeft.IndexCount)*sizeof(u_short),0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pcDistortionIndexBufferLeft, NULL ) ) )
			{
				unsigned short* dxi; 
				if (SUCCEEDED(m_pcDistortionIndexBufferLeft->Lock( 0, 0, (void**)&dxi, 0 )))
				{
					for (UINT i = 0; i < meshDataLeft.IndexCount; i++) 
						dxi[i] = meshDataLeft.pIndexData[i];
					m_pcDistortionIndexBufferLeft->Unlock();
				}
				else 
					OutputDebugString(L"OculusTracker : Failed to lock index buffer !");
			} 
			else 
				OutputDebugString(L"OculusTracker : Failed to create index buffer !");

			// create right eye index buffer
			if (SUCCEEDED(pDevice->CreateIndexBuffer( (meshDataRight.IndexCount)*sizeof(u_short),0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pcDistortionIndexBufferRight, NULL ) ) )
			{
				unsigned short* dxi; 
				if (SUCCEEDED(m_pcDistortionIndexBufferRight->Lock( 0, 0, (void**)&dxi, 0 )))
				{
					for (UINT i = 0; i < meshDataRight.IndexCount; i++) 
						dxi[i] = meshDataRight.pIndexData[i];

					m_pcDistortionIndexBufferRight->Unlock();
				}
				else 
					OutputDebugString(L"OculusTracker : Failed to lock index buffer !");
			} 
			else 
				OutputDebugString(L"OculusTracker : Failed to create index buffer !");

			// release all meshes
			ovrHmd_DestroyDistortionMesh(&meshDataLeft);
			ovrHmd_DestroyDistortionMesh(&meshDataRight);

			// Configure Stereo settings.
			ovrSizei recommenedTex0Size = ovrHmd_GetFovTextureSize(m_hHMD, ovrEye_Left,	m_sEyeRenderDescLeft.Fov, 1.0f);
			ovrSizei recommenedTex1Size = ovrHmd_GetFovTextureSize(m_hHMD, ovrEye_Right, m_sEyeRenderDescRight.Fov, 1.0f);
			m_nRenderTextureWidth = max ( recommenedTex0Size.w, recommenedTex1Size.w );
			m_nRenderTextureHeight = max ( recommenedTex0Size.h, recommenedTex1Size.h );

			// create default FOV projection matrix
			ovrMatrix4f sProjLeft = ovrMatrix4f_Projection(m_sEyeRenderDescLeft.Fov, 0.01f, 10000.0f, true);
			ovrMatrix4f sProjRight = ovrMatrix4f_Projection(m_sEyeRenderDescRight.Fov, 0.01f, 10000.0f, true);

			D3DXMATRIX* psProjLeft = reinterpret_cast<D3DXMATRIX*>(&sProjLeft);
			D3DXMATRIX* psProjRight = reinterpret_cast<D3DXMATRIX*>(&sProjRight);
			D3DXMATRIX sProjLeftTransposed;
			D3DXMATRIX sProjRightTransposed;
			D3DXMatrixTranspose(&sProjLeftTransposed, psProjLeft);
			D3DXMatrixTranspose(&sProjRightTransposed, psProjRight);
			CopyMemory(&m_sDefaultFOVMatrixProjLeft, &sProjLeftTransposed, sizeof(D3DMATRIX));
			CopyMemory(&m_sDefaultFOVMatrixProjRight, &sProjRightTransposed, sizeof(D3DMATRIX));

			// release device if provided by swapchain
			if (bReleaseDevice) pDevice->Release();
		}
	}
	else
	{
		// Initialize LibOVR, and the Rift... then create hmd handle
		ovr_Initialize();
		m_hHMD = ovrHmd_Create(0);

		if (m_hHMD)
		{
			// recenter rift at startup, set output pointers
			ovrHmd_RecenterPose(m_hHMD);

			for (DWORD dwCommanderIndex = 0; dwCommanderIndex < NUMBER_OF_COMMANDERS; dwCommanderIndex++)
			{
				switch ((OTR_Commanders)dwCommanderIndex)
				{
				case OTR_Commanders::Yaw:
					m_paOutput[dwCommanderIndex] = (void*)&m_fYaw;
					break;
				case OTR_Commanders::Pitch:
					m_paOutput[dwCommanderIndex] =  (void*)&m_fPitch;
					break;
				case OTR_Commanders::Roll:
					m_paOutput[dwCommanderIndex] =  (void*)&m_fRoll;
					break;
				case OTR_Commanders::OrientationX:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Orientation.x;
					break;
				case OTR_Commanders::OrientationY:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Orientation.y;
					break;
				case OTR_Commanders::OrientationZ:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Orientation.z;
					break;
				case OTR_Commanders::OrientationW:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Orientation.w;
					break;
				case OTR_Commanders::PositionX:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Position.x;
					break;
				case OTR_Commanders::PositionY:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Position.y;
					break;
				case OTR_Commanders::PositionZ:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sPose.Position.z;
					break;
				case OTR_Commanders::CameraFrustumHFovInRadians:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->CameraFrustumHFovInRadians;
					break;
				case OTR_Commanders::CameraFrustumVFovInRadians:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->CameraFrustumVFovInRadians;
					break;
				case OTR_Commanders::CameraFrustumNearZInMeters:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->CameraFrustumNearZInMeters;
					break;
				case OTR_Commanders::CameraFrustumFarZInMeters:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->CameraFrustumFarZInMeters;
					break;
				case OTR_Commanders::HmdCaps:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->HmdCaps;
					break;
				case OTR_Commanders::TrackingCaps:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->TrackingCaps;
					break;
				case OTR_Commanders::DistortionCaps:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->DistortionCaps;
					break;
				case OTR_Commanders::ResolutionW:
					m_paOutput[dwCommanderIndex] =  (void*)&m_nRenderTextureWidth;
					break;
				case OTR_Commanders::ResolutionH:
					m_paOutput[dwCommanderIndex] =  (void*)&m_nRenderTextureHeight;
					break;
				case OTR_Commanders::DistortionVertexBufferLeft:
					m_paOutput[dwCommanderIndex] =  &m_pcDistortionVertexBufferLeft;
					break;
				case OTR_Commanders::DistortionVertexBufferRight:
					m_paOutput[dwCommanderIndex] =  &m_pcDistortionVertexBufferRight;
					break;
				case OTR_Commanders::DistortionIndexBufferLeft:
					m_paOutput[dwCommanderIndex] =  &m_pcDistortionIndexBufferLeft;
					break;
				case OTR_Commanders::DistortionIndexBufferRight:
					m_paOutput[dwCommanderIndex] =  &m_pcDistortionIndexBufferRight;
					break;
				case OTR_Commanders::OculusVertexDeclaration:
					m_paOutput[dwCommanderIndex] =  &m_pcVertexDecl;
					break;
				case OTR_Commanders::DefaultEyeFovLeft:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sEyeRenderDescLeft.Fov;
					break;
				case OTR_Commanders::DefaultEyeFovRight:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sEyeRenderDescRight.Fov;
					break;
				case OTR_Commanders::MaxEyeFovLeft:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->MaxEyeFov[0];
					break;
				case OTR_Commanders::MaxEyeFovRight:
					m_paOutput[dwCommanderIndex] =  (void*)&m_hHMD->MaxEyeFov[1];
					break;
				case OTR_Commanders::HmdToEyeViewOffsetLeft:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sEyeRenderDescLeft.HmdToEyeViewOffset;
					break;
				case OTR_Commanders::HmdToEyeViewOffsetRight:
					m_paOutput[dwCommanderIndex] =  (void*)&m_sEyeRenderDescRight.HmdToEyeViewOffset;
					break;
				case OTR_Commanders::DefaultProjectionMatrixLeft:
					m_paOutput[dwCommanderIndex] = (void*)&m_sDefaultFOVMatrixProjLeft;
					break;
				case OTR_Commanders::DefaultProjectionMatrixRight:
					m_paOutput[dwCommanderIndex] = (void*)&m_sDefaultFOVMatrixProjRight;
					break;
				}
			}
		}
	}

	return nullptr;
}
