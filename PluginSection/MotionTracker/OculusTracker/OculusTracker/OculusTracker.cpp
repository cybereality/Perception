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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

/**
* Constructor.
***/
OculusTracker::OculusTracker() :AQU_Nodus(),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_unRenderTextureWidth(0),
m_unRenderTextureHeight(0),
m_hHMD(nullptr)
{
	ZeroMemory(&m_sDefaultFOVMatrixProjLeft, sizeof(D3DMATRIX));
	ZeroMemory(&m_sDefaultFOVMatrixProjRight, sizeof(D3DMATRIX));

	ZeroMemory(&m_fEulerPredicted[0], sizeof(float)* 3);
	ZeroMemory(&m_fPositionPredicted[0], sizeof(float)* 3);
	ZeroMemory(&m_fEulerVelocity[0], sizeof(float)* 3);
	ZeroMemory(&m_fPositionVelocity[0], sizeof(float)* 3);

	m_cGameTimer.Reset();

	// set default ini file settings
	m_afPositionOrigin[0] = 0.0f;
	m_afPositionOrigin[1] = 1.7f; /**< Default y tracking origin : 1.7 meters **/
	m_afPositionOrigin[2] = 0.0f;

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	m_afPositionOrigin[0] = GetIniFileSetting(m_afPositionOrigin[0], "LibOVR", "afPositionOrigin[0]", szFilePathINI, bFileExists);
	m_afPositionOrigin[1] = GetIniFileSetting(m_afPositionOrigin[1], "LibOVR", "afPositionOrigin[1]", szFilePathINI, bFileExists);
	m_afPositionOrigin[2] = GetIniFileSetting(m_afPositionOrigin[2], "LibOVR", "afPositionOrigin[2]", szFilePathINI, bFileExists);
}

/**
* Destructor.
***/
OculusTracker::~OculusTracker()
{
	if (m_hHMD) ovr_Destroy(m_hHMD);
	ovr_Shutdown();
	if (m_hBitmapControl) CloseHandle(m_hBitmapControl);
	if (m_hFont) CloseHandle(m_hFont);
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
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 2000);
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
		SetRect(&rc, 0, 0, 1024, 2000);
		FillRect(hdcImage, &rc, (HBRUSH)CreateSolidBrush(RGB(240, 160, 192)));

		// create font
		if (!m_hFont)
			m_hFont = CreateFont(64, 0, 0, 0, 0, FALSE,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			L"Segoe UI");

		// Select the variable stock font into the specified device context. 
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (m_hHMD))
		{
			int nY = 16;
			std::wstringstream szBuffer;
			//char szBuffer[256];

			SetTextColor(hdcImage, RGB(64, 0, 48));
			SetBkColor(hdcImage, RGB(240, 160, 192));

			// display the values suiteable to the data commanders... first yaw pitch roll
			szBuffer << m_fEuler[0];
			TextOut(hdcImage, 880, nY, L"Pitch", 5);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fEuler[1];
			TextOut(hdcImage, 880, nY, L"Yaw", 3);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fEuler[2];
			TextOut(hdcImage, 880, nY, L"Roll", 4);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// orientation
			szBuffer << m_sPose.Orientation.w;
			TextOut(hdcImage, 670, nY, L"Orientation W", 13);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPose.Orientation.x;
			TextOut(hdcImage, 670, nY, L"Orientation X", 13);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPose.Orientation.y;
			TextOut(hdcImage, 670, nY, L"Orientation Y", 13);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPose.Orientation.z;
			TextOut(hdcImage, 670, nY, L"Orientation Z", 13);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// position tracking
			szBuffer << m_sPose.Position.x;
			TextOut(hdcImage, 750, nY, L"Position X", 10);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPose.Position.y;
			TextOut(hdcImage, 750, nY, L"Position Y", 10);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPose.Position.z;
			TextOut(hdcImage, 750, nY, L"Position Z", 10);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// empty... former camera frustum
			nY += 256;

			// caps
			TextOut(hdcImage, 770, nY, L"HmdCaps", 7); nY += 64;
			TextOut(hdcImage, 690, nY, L"TrackingCaps", 12); nY += 64;

			// resolution
			wchar_t szBufferW[128];
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_unRenderTextureWidth);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 510, nY, L"TexResolution Width", 19);
			int nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_unRenderTextureHeight);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 500, nY, L"TexResolution Height", 20);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;

			// FOV
			TextOut(hdcImage, 520, nY, L"Default EyeFov Left", 19); nY += 64;
			TextOut(hdcImage, 520, nY, L"Default EyeFov Right", 20); nY += 64;
			TextOut(hdcImage, 570, nY, L"MaxEye Fov Left", 15); nY += 64;
			TextOut(hdcImage, 570, nY, L"MaxEye Fov Right", 16); nY += 64;

			// Default FOV projection matrix
			TextOut(hdcImage, 150, nY, L"Default FOV Projection Matrix Left", 34); nY += 64;
			TextOut(hdcImage, 150, nY, L"Default FOV Projection Matrix Right", 35); nY += 64;

			// handle, matrices
			TextOut(hdcImage, 150, nY, L"HMD Handle", 10); nY += 64;
			TextOut(hdcImage, 150, nY, L"View", 4); nY += 64;
			TextOut(hdcImage, 150, nY, L"Projection Left", 15); nY += 64;
			TextOut(hdcImage, 150, nY, L"Projection Right", 16); nY += 64;

			// Display the text string for the provoker
			szBuffer << m_sHMDDesc.ProductName << " " << m_sHMDDesc.Resolution.w << "x" << m_sHMDDesc.Resolution.h;
			TextOut(hdcImage, 50, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());

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
		case OTR_Commanders::Pitch:
			return L"Pitch";
		case OTR_Commanders::Yaw:
			return L"Yaw";
		case OTR_Commanders::Roll:
			return L"Roll";
		case OTR_Commanders::OrientationW:
			return L"Orientation W";
		case OTR_Commanders::OrientationX:
			return L"Orientation X";
		case OTR_Commanders::OrientationY:
			return L"Orientation Y";
		case OTR_Commanders::OrientationZ:
			return L"Orientation Z";
		case OTR_Commanders::PositionX:
			return L"Position X";
		case OTR_Commanders::PositionY:
			return L"Position Y";
		case OTR_Commanders::PositionZ:
			return L"Position Z";
		case OTR_Commanders::HmdCaps:
			return L"HmdCaps";
		case OTR_Commanders::TrackingCaps:
			return L"TrackingCaps";
		case OTR_Commanders::ResolutionW:
			return L"Texture Resolution Width";
		case OTR_Commanders::ResolutionH:
			return L"Texture Resolution Height";
		case OTR_Commanders::DefaultEyeFovLeft:
			return L"Default Eye Fov Left";
		case OTR_Commanders::DefaultEyeFovRight:
			return L"Default Eye Fov Right";
		case OTR_Commanders::MaxEyeFovLeft:
			return L"Max Eye Fov Left";
		case OTR_Commanders::MaxEyeFovRight:
			return L"Max Eye Fov Right";
		case OTR_Commanders::DefaultProjectionMatrixLeft:
			return L"Default FOV Projection Matrix Left";
		case OTR_Commanders::DefaultProjectionMatrixRight:
			return L"Default FOV Projection Matrix Right";
		case OTR_Commanders::HMD_Handle:
			return L"HMD Handle";
		case OTR_Commanders::View:
			return L"View";
		case OTR_Commanders::ProjectionLeft:
			return L"ProjectionLeft";
		case OTR_Commanders::ProjectionRight:
			return L"ProjectionRight";
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
		case OTR_Commanders::Pitch:
		case OTR_Commanders::Yaw:
		case OTR_Commanders::Roll:
		case OTR_Commanders::OrientationW:
		case OTR_Commanders::OrientationX:
		case OTR_Commanders::OrientationY:
		case OTR_Commanders::OrientationZ:
		case OTR_Commanders::PositionX:
		case OTR_Commanders::PositionY:
		case OTR_Commanders::PositionZ:
			return NOD_Plugtype::AQU_FLOAT;
		case OTR_Commanders::HmdCaps:
		case OTR_Commanders::TrackingCaps:
		case OTR_Commanders::ResolutionW:
		case OTR_Commanders::ResolutionH:
			return NOD_Plugtype::AQU_UINT;
		case OTR_Commanders::DefaultEyeFovLeft:
		case OTR_Commanders::DefaultEyeFovRight:
		case OTR_Commanders::MaxEyeFovLeft:
		case OTR_Commanders::MaxEyeFovRight:
			return NOD_Plugtype::AQU_VECTOR4F;
		case OTR_Commanders::DefaultProjectionMatrixLeft:
		case OTR_Commanders::DefaultProjectionMatrixRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
		case OTR_Commanders::HMD_Handle:
			return NOD_Plugtype::AQU_HANDLE;
		case OTR_Commanders::View:
		case OTR_Commanders::ProjectionLeft:
		case OTR_Commanders::ProjectionRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
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
	switch (dwCommanderIndex)
	{
		case OTR_Commanders::Pitch:
			return (void*)&m_fEulerPredicted[0];
		case OTR_Commanders::Yaw:
			return (void*)&m_fEulerPredicted[1];
		case OTR_Commanders::Roll:
			return (void*)&m_fEulerPredicted[2];
		case OTR_Commanders::OrientationW:
			return (void*)&m_sPose.Orientation.w;
		case OTR_Commanders::OrientationX:
			return (void*)&m_sPose.Orientation.x;
		case OTR_Commanders::OrientationY:
			return (void*)&m_sPose.Orientation.y;
		case OTR_Commanders::OrientationZ:
			return (void*)&m_sPose.Orientation.z;
		case OTR_Commanders::PositionX:
			return (void*)&m_afPosition[0];
		case OTR_Commanders::PositionY:
			return (void*)&m_afPosition[1];
		case OTR_Commanders::PositionZ:
			return (void*)&m_afPosition[2];
		case OTR_Commanders::HmdCaps:
			return (void*)&m_sHMDDesc.DefaultHmdCaps;
		case OTR_Commanders::TrackingCaps:
			return (void*)&m_sHMDDesc.DefaultTrackingCaps;
		case OTR_Commanders::ResolutionW:
			return (void*)&m_unRenderTextureWidth;
		case OTR_Commanders::ResolutionH:
			return (void*)&m_unRenderTextureHeight;
		case OTR_Commanders::DefaultEyeFovLeft:
			return (void*)&m_sHMDDesc.MaxEyeFov[0];
		case OTR_Commanders::DefaultEyeFovRight:
			return (void*)&m_sHMDDesc.MaxEyeFov[1];
		case OTR_Commanders::MaxEyeFovLeft:
			return (void*)&m_sHMDDesc.MaxEyeFov[0];
		case OTR_Commanders::MaxEyeFovRight:
			return (void*)&m_sHMDDesc.MaxEyeFov[1];
		case OTR_Commanders::DefaultProjectionMatrixLeft:
			return (void*)&m_sDefaultFOVMatrixProjLeft;
		case OTR_Commanders::DefaultProjectionMatrixRight:
			return (void*)&m_sDefaultFOVMatrixProjRight;
		case OTR_Commanders::HMD_Handle:
			return (void*)&m_hHMD;
		case OTR_Commanders::View:
			return (void*)&m_sView;
		case OTR_Commanders::ProjectionLeft:
			return (void*)&m_asProjection[0];
		case OTR_Commanders::ProjectionRight:
			return (void*)&m_asProjection[1];
		default:
			break;
	}

	return nullptr;
}

/**
* Supports any method.
***/
bool OculusTracker::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Render the Virtual Cinema Theatre.
***/
void* OculusTracker::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	// update game timer
	m_cGameTimer.Tick();

	static UINT unFrameSkip = 200;
	if (unFrameSkip > 0)
	{
		unFrameSkip--;
		return nullptr;
	}

	if (m_hHMD)
	{
		/*// Start the sensor which informs of the Rift's pose and motion   .... obsolete for SDK 1.3.x ??
		ovr_ConfigureTracking(m_hHMD, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);*/

		// get the current tracking state
		ovrTrackingState sTrackingState = ovr_GetTrackingState(m_hHMD, ovr_GetTimeInSeconds(), false);

		if (sTrackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{
			// get pose
			ovrPoseStatef sPoseState = sTrackingState.HeadPose;
			m_sPose = sPoseState.ThePose;
			m_sOrientation.x = m_sPose.Orientation.x;
			m_sOrientation.y = m_sPose.Orientation.y;
			m_sOrientation.z = m_sPose.Orientation.z;
			m_sOrientation.w = m_sPose.Orientation.w;

			// backup old euler angles and velocity
			float fEulerOld[3];
			float fEulerVelocityOld[3];
			memcpy(&fEulerOld[0], &m_fEuler[0], sizeof(float)* 3);
			memcpy(&fEulerVelocityOld[0], &m_fEulerVelocity[0], sizeof(float)* 3);

			// get angles
			m_sOrientation.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_R >(&m_fEuler[1], &m_fEuler[0], &m_fEuler[2]);
			
			// quick fix here...
			m_fEuler[1] *= -1.0f;
			m_fEuler[0] *= -1.0f;
			m_fEuler[2] *= -1.0f;

			// get euler velocity + acceleration
			float fEulerAcceleration[3];
			for (UINT unI = 0; unI < 3; unI++)
			{
				// get the velocity
				m_fEulerVelocity[unI] = (m_fEuler[unI] - fEulerOld[unI]) / (float)m_cGameTimer.DeltaTime();

				// get the acceleration
				fEulerAcceleration[unI] = (m_fEulerVelocity[unI] - fEulerVelocityOld[unI]) / (float)m_cGameTimer.DeltaTime();
			}

			// get predicted euler
			for (UINT unI = 0; unI < 3; unI++)
			{
				// compute predicted euler
				m_fEulerPredicted[unI] = (0.5f * fEulerAcceleration[unI] * ((float)m_cGameTimer.DeltaTime() * (float)m_cGameTimer.DeltaTime())) + (m_fEulerVelocity[unI] * (float)m_cGameTimer.DeltaTime()) + m_fEuler[unI];
			}

			// set the drawing update to true
			m_bControlUpdate = true;

			// set position
			m_afPosition[0] = (float)-m_sPose.Position.x - m_afPositionOrigin[0];
			m_afPosition[1] = (float)-m_sPose.Position.y - m_afPositionOrigin[1];
			m_afPosition[2] = (float)m_sPose.Position.z + m_afPositionOrigin[2];

			// create view matrix from rotation and position
			D3DXMATRIX sRotation;
			D3DXMATRIX sPitch, sYaw, sRoll;
			D3DXMatrixRotationX(&sPitch, m_fEulerPredicted[0]);
			D3DXMatrixRotationY(&sYaw, m_fEulerPredicted[1]);
			D3DXMatrixRotationZ(&sRoll, -m_fEulerPredicted[2]);
			sRotation = sYaw * sPitch * sRoll;
			D3DXMATRIX sTranslation;
			D3DXMatrixTranslation(&sTranslation, (float)m_afPosition[0], (float)m_afPosition[1], (float)m_afPosition[2]);
			m_sView = sTranslation * sRotation;
		}
	}
	else
	{
		// Initialize LibOVR, and the Rift... then create hmd handle
		ovrResult result = ovr_Initialize(nullptr);
		if (!OVR_SUCCESS(result))
		{
			OutputDebugString(L"Failed to initialize libOVR.");
			return nullptr;
		}

		result = ovr_Create(&m_hHMD, &m_sLuid);
		if (!OVR_SUCCESS(result))
			return nullptr;

		if (m_hHMD)
		{
			// get the description and set pointers
			m_sHMDDesc = ovr_GetHmdDesc(m_hHMD);

			// Configure Stereo settings.
			ovrSizei sRecommenedTex0Size = ovr_GetFovTextureSize(m_hHMD, ovrEye_Left,
				m_sHMDDesc.DefaultEyeFov[0], 1.0f);
			ovrSizei sRecommenedTex1Size = ovr_GetFovTextureSize(m_hHMD, ovrEye_Right,
				m_sHMDDesc.DefaultEyeFov[1], 1.0f);

			ovrSizei sTextureSize;
			sTextureSize.w = max(sRecommenedTex0Size.w, sRecommenedTex1Size.w);
			sTextureSize.h = max(sRecommenedTex0Size.h, sRecommenedTex1Size.h);
			m_unRenderTextureWidth = (UINT)sTextureSize.w;
			m_unRenderTextureHeight = (UINT)sTextureSize.h;

			// get view offset
			ovrEyeRenderDesc asEyeRenderDesc[2];
			asEyeRenderDesc[0] = ovr_GetRenderDesc(m_hHMD, ovrEye_Left, m_sHMDDesc.DefaultEyeFov[0]);
			asEyeRenderDesc[1] = ovr_GetRenderDesc(m_hHMD, ovrEye_Right, m_sHMDDesc.DefaultEyeFov[1]);
			ovrVector3f asViewOffset[2] = { asEyeRenderDesc[0].HmdToEyeOffset, asEyeRenderDesc[1].HmdToEyeOffset };

			// get projection matrices left/right
			D3DXMATRIX asToEye[2];
			D3DXMATRIX asProjection[2];
			for (UINT unEye = 0; unEye < 2; unEye++)
			{
				// get ovr projection
				ovrMatrix4f sProj = ovrMatrix4f_Projection(m_sHMDDesc.DefaultEyeFov[unEye], 0.01f, 30.0f, ovrProjection_None);

				// create dx projection
				asProjection[unEye] = D3DXMATRIX(&sProj.M[0][0]);
				D3DXMatrixTranspose(&asProjection[unEye], &asProjection[unEye]);

				// create view offset translation matrix
				D3DXMatrixTranslation(&asToEye[unEye], -asViewOffset[unEye].x, -asViewOffset[unEye].y, asViewOffset[unEye].z);

				// create eventual projection
				m_asProjection[unEye] = asToEye[unEye] * asProjection[unEye];
			}
		}
	}

	return nullptr;
}
