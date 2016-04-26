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
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 1700);
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
		SetRect(&rc, 0, 0, 1024, 1700);
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
			szBuffer << m_fPitch;
			TextOut(hdcImage, 880, nY, L"Pitch", 5);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fYaw;
			TextOut(hdcImage, 880, nY, L"Yaw", 3);
			TextOut(hdcImage, 392, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fRoll;
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

			// camera frustum
			wchar_t szBufferW[128];
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_sHMDDesc.CameraFrustumHFovInRadians);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY, L"CameraFrustumHFovInRadians", 25);
			int nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_sHMDDesc.CameraFrustumVFovInRadians);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY, L"CameraFrustumVFovInRadians", 25);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_sHMDDesc.CameraFrustumNearZInMeters);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 300, nY, L"CameraFrustumNearZInMeters", 26);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_sHMDDesc.CameraFrustumFarZInMeters);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 320, nY, L"CameraFrustumFarZInMeters", 25);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;

			// caps
			TextOut(hdcImage, 770, nY, L"HmdCaps", 7); nY += 64;
			TextOut(hdcImage, 690, nY, L"TrackingCaps", 12); nY += 64;

			// resolution
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_nRenderTextureWidth);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 510, nY, L"TexResolution Width", 19);
			nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hHMD)
				wsprintf(szBufferW, L"%u", m_nRenderTextureHeight);
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

			// handle
			TextOut(hdcImage, 150, nY, L"HMD Handle", 10); nY += 64;

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
		case OTR_Commanders::CameraFrustumHFovInRadians:
		case OTR_Commanders::CameraFrustumVFovInRadians:
		case OTR_Commanders::CameraFrustumNearZInMeters:
		case OTR_Commanders::CameraFrustumFarZInMeters:
			return PNT_FLOAT_PLUG_TYPE;
		case OTR_Commanders::HmdCaps:
		case OTR_Commanders::TrackingCaps:
			return PNT_UINT_PLUG_TYPE;
		case OTR_Commanders::ResolutionW:
		case OTR_Commanders::ResolutionH:
			return PNT_INT_PLUG_TYPE;
		case OTR_Commanders::DefaultEyeFovLeft:
		case OTR_Commanders::DefaultEyeFovRight:
		case OTR_Commanders::MaxEyeFovLeft:
		case OTR_Commanders::MaxEyeFovRight:
			return PNT_VECTOR4F_PLUG_TYPE;
		case OTR_Commanders::DefaultProjectionMatrixLeft:
		case OTR_Commanders::DefaultProjectionMatrixRight:
			return PNT_D3DMATRIX_PLUG_TYPE;
		case OTR_Commanders::HMD_Handle:
			return NOD_Plugtype::AQU_HANDLE;
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
	if (dwCommanderIndex == (DWORD)OTR_Commanders::Pitch)
		return (void*)&m_fPitch;
	if (dwCommanderIndex == (DWORD)OTR_Commanders::Yaw)
		return (void*)&m_fYaw;
	if (dwCommanderIndex == (DWORD)OTR_Commanders::Roll)
		return (void*)&m_fRoll;
	if (dwCommanderIndex == (DWORD)OTR_Commanders::HMD_Handle)
	{
		return (void*)&m_hHMD;
	}
	if (dwCommanderIndex < NUMBER_OF_COMMANDERS)
		return (void*)m_paOutput[dwCommanderIndex];

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
	if (m_hHMD)
	{
		// Start the sensor which informs of the Rift's pose and motion
		ovr_ConfigureTracking(m_hHMD, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);

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

			// get angles
			m_sOrientation.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_R >(&m_fYaw, &m_fPitch, &m_fRoll);
			
			// quick fix here...
			m_fYaw *= -1.0f;
			m_fPitch *= -1.0f;
			m_fRoll *= -1.0f;

			// set the drawing update to true
			m_bControlUpdate = true;
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
		OutputDebugString(L"CREATED HMD HANDLE");
		DEBUG_HEX(m_hHMD);
		DEBUG_HEX(&m_hHMD);
		if (m_hHMD)
		{
			// get the description and set pointers
			m_sHMDDesc = ovr_GetHmdDesc(m_hHMD);

			for (DWORD dwCommanderIndex = 0; dwCommanderIndex < NUMBER_OF_COMMANDERS; dwCommanderIndex++)
			{
				switch ((OTR_Commanders)dwCommanderIndex)
				{
					case OTR_Commanders::Pitch:
						m_paOutput[dwCommanderIndex] = (void*)&m_fPitch;
						break;
					case OTR_Commanders::Yaw:
						m_paOutput[dwCommanderIndex] = (void*)&m_fYaw;
						break;
					case OTR_Commanders::Roll:
						m_paOutput[dwCommanderIndex] = (void*)&m_fRoll;
						break;
					case OTR_Commanders::OrientationW:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Orientation.w;
						break;
					case OTR_Commanders::OrientationX:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Orientation.x;
						break;
					case OTR_Commanders::OrientationY:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Orientation.y;
						break;
					case OTR_Commanders::OrientationZ:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Orientation.z;
						break;
					case OTR_Commanders::PositionX:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Position.x;
						break;
					case OTR_Commanders::PositionY:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Position.y;
						break;
					case OTR_Commanders::PositionZ:
						m_paOutput[dwCommanderIndex] = (void*)&m_sPose.Position.z;
						break;
					case OTR_Commanders::CameraFrustumHFovInRadians:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.CameraFrustumHFovInRadians;
						break;
					case OTR_Commanders::CameraFrustumVFovInRadians:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.CameraFrustumVFovInRadians;
						break;
					case OTR_Commanders::CameraFrustumNearZInMeters:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.CameraFrustumNearZInMeters;
						break;
					case OTR_Commanders::CameraFrustumFarZInMeters:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.CameraFrustumFarZInMeters;
						break;
					case OTR_Commanders::HmdCaps:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.DefaultHmdCaps;
						break;
					case OTR_Commanders::TrackingCaps:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.DefaultTrackingCaps;
						break;
					case OTR_Commanders::ResolutionW:
						m_paOutput[dwCommanderIndex] = (void*)&m_nRenderTextureWidth;
						break;
					case OTR_Commanders::ResolutionH:
						m_paOutput[dwCommanderIndex] = (void*)&m_nRenderTextureHeight;
						break;
					case OTR_Commanders::DefaultEyeFovLeft:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.MaxEyeFov[0];
						break;
					case OTR_Commanders::DefaultEyeFovRight:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.MaxEyeFov[1];
						break;
					case OTR_Commanders::MaxEyeFovLeft:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.MaxEyeFov[0];
						break;
					case OTR_Commanders::MaxEyeFovRight:
						m_paOutput[dwCommanderIndex] = (void*)&m_sHMDDesc.MaxEyeFov[1];
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
