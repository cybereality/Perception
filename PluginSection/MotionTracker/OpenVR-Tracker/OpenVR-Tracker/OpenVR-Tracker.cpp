/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OpenVR Tracker - Open Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OpenVR-Tracker.cpp> and
Class <OpenVR-Tracker> :
Copyright (C) 2016 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin
and Neil Schneider

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

#include"OpenVR-Tracker.h"

#define INTERFACE_IDIRECT3DDEVICE9           8

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

/**
* Constructor.
***/
OpenVR_Tracker::OpenVR_Tracker() :AQU_Nodus()
, m_pHMD(NULL)
, m_hBitmapControl(nullptr)
, m_bControlUpdate(false)
, m_hFont(nullptr)
{
	ZeroMemory(&m_sEuler[0], sizeof(D3DXVECTOR3)* vr::k_unMaxTrackedDeviceCount);
	ZeroMemory(&m_sOrientation[0], sizeof(D3DXQUATERNION)* vr::k_unMaxTrackedDeviceCount);
	ZeroMemory(&m_sPosition[0], sizeof(D3DXVECTOR3)* vr::k_unMaxTrackedDeviceCount);
}

/**
* Destructor.
***/
OpenVR_Tracker::~OpenVR_Tracker()
{
	if (m_pHMD) m_pHMD->ReleaseInputFocus();
	vr::VR_Shutdown();
}

/**
* Return the name of the  OpenVR Tracker node.
***/
const char* OpenVR_Tracker::GetNodeType()
{
	return "OpenVR Tracker";
}

/**
* Returns a global unique identifier for the OpenVR Tracker node.
***/
UINT OpenVR_Tracker::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 320
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OpenVR Tracker node.
***/
LPWSTR OpenVR_Tracker::GetCategory()
{
	return L"Motion Tracker";
}

/**
* Returns a logo to be used for the OpenVR Tracker node.
***/
HBITMAP OpenVR_Tracker::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OpenVR-Tracker.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OpenVR Tracker node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OpenVR_Tracker::GetControl()
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
		FillRect(hdcImage, &rc, (HBRUSH)CreateSolidBrush(RGB(238, 238, 238)));

		// create font
		if (!m_hFont)
			m_hFont = CreateFont(64, 0, 0, 0, 0, FALSE,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			L"Segoe UI");

		// Select the variable stock font into the specified device context. 
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (m_pHMD))
		{
			int nY = 16;
			std::wstringstream szBuffer;

			SetTextColor(hdcImage, RGB(253, 128, 0));
			SetBkColor(hdcImage, RGB(238, 238, 238));

			// display the values suiteable to the data commanders... first yaw pitch roll
			szBuffer << m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].x;
			TextOut(hdcImage, 730, nY, L"Pitch X", 7);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].y;
			TextOut(hdcImage, 780, nY, L"Yaw Y", 5);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].z;
			TextOut(hdcImage, 765, nY, L"Roll Z", 6);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// orientation
			szBuffer << m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].w;
			TextOut(hdcImage, 570, nY, L"Orientation W", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].x;
			TextOut(hdcImage, 570, nY, L"Orientation X", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].y;
			TextOut(hdcImage, 570, nY, L"Orientation Y", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].z;
			TextOut(hdcImage, 570, nY, L"Orientation Z", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// position tracking
			szBuffer << m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].x;
			TextOut(hdcImage, 650, nY, L"Position X", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].y;
			TextOut(hdcImage, 650, nY, L"Position Y", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].z;
			TextOut(hdcImage, 650, nY, L"Position Z", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// tracking system + serial
			TextOut(hdcImage, 100, nY, L"IVRSystem", 9); nY += 64;
			szBuffer << m_strDisplay.c_str() << "-" << m_strDriver.c_str();
			TextOut(hdcImage, 100, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());

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
LPWSTR OpenVR_Tracker::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((OpenVR_Commanders)dwCommanderIndex)
	{
		case OpenVR_Commanders::Pitch:
			return L"Pitch";
		case OpenVR_Commanders::Yaw:
			return L"Yaw";
		case OpenVR_Commanders::Roll:
			return L"Roll";
		case OpenVR_Commanders::OrientationW:
			return L"Orientation W";
		case OpenVR_Commanders::OrientationX:
			return L"Orientation X";
		case OpenVR_Commanders::OrientationY:
			return L"Orientation Y";
		case OpenVR_Commanders::OrientationZ:
			return L"Orientation Z";
		case OpenVR_Commanders::PositionX:
			return L"Position X";
		case OpenVR_Commanders::PositionY:
			return L"Position Y";
		case OpenVR_Commanders::PositionZ:
			return L"Position Z";
		case OpenVR_Commanders::IVRSystem:
			return L"IVRSystem";
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OpenVR_Tracker::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((OpenVR_Commanders)dwCommanderIndex)
	{
		case OpenVR_Commanders::Pitch:
		case OpenVR_Commanders::Yaw:
		case OpenVR_Commanders::Roll:
		case OpenVR_Commanders::OrientationW:
		case OpenVR_Commanders::OrientationX:
		case OpenVR_Commanders::OrientationY:
		case OpenVR_Commanders::OrientationZ:
		case OpenVR_Commanders::PositionX:
		case OpenVR_Commanders::PositionY:
		case OpenVR_Commanders::PositionZ:
			return NOD_Plugtype::AQU_FLOAT;
		case OpenVR_Commanders::IVRSystem:
			return NOD_Plugtype::AQU_HANDLE;
	}

	return 0;
}

/**
* Provides the pointer of the requested commander.
***/
void* OpenVR_Tracker::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((OpenVR_Commanders)dwCommanderIndex)
	{
		case OpenVR_Commanders::Pitch:
			return (void*)&m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].x;
		case OpenVR_Commanders::Yaw:
			return (void*)&m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].y;
		case OpenVR_Commanders::Roll:
			return (void*)&m_sEuler[vr::k_unTrackedDeviceIndex_Hmd].z;
		case OpenVR_Commanders::OrientationW:
			return (void*)&m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].w;
		case OpenVR_Commanders::OrientationX:
			return (void*)&m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].x;
		case OpenVR_Commanders::OrientationY:
			return (void*)&m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].y;
		case OpenVR_Commanders::OrientationZ:
			return (void*)&m_sOrientation[vr::k_unTrackedDeviceIndex_Hmd].z;
		case OpenVR_Commanders::PositionX:
			return (void*)&m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].x;
		case OpenVR_Commanders::PositionY:
			return (void*)&m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].y;
		case OpenVR_Commanders::PositionZ:
			return (void*)&m_sPosition[vr::k_unTrackedDeviceIndex_Hmd].z;
		case OpenVR_Commanders::IVRSystem:
			return (void*)&m_pHMD;
	}

	return nullptr;
}

/**
* Tracker supports any calls.
***/
bool OpenVR_Tracker::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OpenVR tracking.
***/
void* OpenVR_Tracker::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if (!m_pHMD)
	{
		// Loading the SteamVR Runtime
		vr::EVRInitError eError = vr::VRInitError_None;
		m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

		if (eError != vr::VRInitError_None)
		{
			m_pHMD = NULL;
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
			OutputDebugStringA(buf);
			return false;
		}

		// set driver+display string
		{
			uint32_t unRequiredBufferLen = m_pHMD->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, NULL, 0, NULL);
			if (unRequiredBufferLen)
			{
				char *pchBuffer = new char[unRequiredBufferLen];
				unRequiredBufferLen = m_pHMD->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, pchBuffer, unRequiredBufferLen, NULL);
				m_strDriver = pchBuffer;
				delete[] pchBuffer;
			}
		}
		{
			uint32_t unRequiredBufferLen = m_pHMD->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String, NULL, 0, NULL);
			if (unRequiredBufferLen)
			{
				char *pchBuffer = new char[unRequiredBufferLen];
				unRequiredBufferLen = m_pHMD->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String, pchBuffer, unRequiredBufferLen, NULL);
				m_strDisplay = pchBuffer;
				delete[] pchBuffer;
			}
		}
	}
	else
	{
		// Process SteamVR events .... TODO !!
		/*vr::VREvent_t event;
		while (m_pHMD->PollNextEvent(&event, sizeof(event)))
		{
		ProcessVREvent(event);
		}

		// Process SteamVR controller state
		for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
		{
		vr::VRControllerState_t state;
		if (m_pHMD->GetControllerState(unDevice, &state))
		{
		m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
		}
		}*/

		// get predicted pose for next frame
		if (!vr::VRCompositor()->GetLastFrameRenderer())
			vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
		else
		{
			m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, /*fPredictedSecondsToPhotonsFromNow*/0.0f, m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount);
		}

		// first, we only handle the HMD = 0 index
		uint32_t unI = vr::k_unTrackedDeviceIndex_Hmd;

		// get quaternion from that pose
		D3DXMATRIX sPoseMatrix(
			m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[0][0], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[1][0], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[2][0], 0.0,
			m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[0][1], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[1][1], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[2][1], 0.0,
			m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[0][2], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[1][2], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[2][2], 0.0,
			m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[0][3], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[1][3], m_rTrackedDevicePose[unI].mDeviceToAbsoluteTracking.m[2][3], 1.0f
			);
		D3DXQuaternionRotationMatrix(&m_sOrientation[unI], &sPoseMatrix);

		// quaternion -> euler angles
		const float w = (float)m_sOrientation[unI].w;
		const float x = (float)m_sOrientation[unI].x;
		const float y = (float)m_sOrientation[unI].y;
		const float z = (float)m_sOrientation[unI].z;

		float sqw = w*w;
		float sqx = x*x;
		float sqy = y*y;
		float sqz = z*z;

		float unit = sqx + sqy + sqz + sqw;
		float test = x*y + z*w;

		if (test > 0.499*unit)
		{
			// singularity at north pole
			m_sEuler[unI].y = 2 * atan2(x, w);
			m_sEuler[unI].z = FLOAT_PI / 2;
			m_sEuler[unI].x = 0;
		}
		else if (test < -0.499*unit)
		{
			// singularity at south pole
			m_sEuler[unI].y = -2 * atan2(x, w);
			m_sEuler[unI].z = -FLOAT_PI / 2;
			m_sEuler[unI].x = 0;
		}
		else
		{
			m_sEuler[unI].y = atan2(2 * y*w - 2 * x*z, sqx - sqy - sqz + sqw);
			m_sEuler[unI].z = asin(2 * test / unit);
			m_sEuler[unI].x = atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
		}

		// get position
		m_sPosition[unI].x = sPoseMatrix(3, 0);
		m_sPosition[unI].y = sPoseMatrix(3, 1);
		m_sPosition[unI].z = sPoseMatrix(3, 2);

		//// output debug string
		//std::wstringstream sz;
		//sz << sAxis.x << ":" << sAxis.y << ":" << sAxis.z << ":" << fAngle;
		//OutputDebugString(sz.str().c_str());

		// set the drawing update to true
		m_bControlUpdate = true;
	}

	return nullptr;
}

/**
* Processes any OpenVR event .
***/
void OpenVR_Tracker::ProcessVREvent(const vr::VREvent_t & event)
{
	switch (event.eventType)
	{
		case vr::VREvent_None:
			break;
		case vr::VREvent_TrackedDeviceActivated:
			break;
		case vr::VREvent_TrackedDeviceDeactivated:
			break;
		case vr::VREvent_TrackedDeviceUpdated:
			break;
		case vr::VREvent_TrackedDeviceUserInteractionStarted:
			break;
		case vr::VREvent_TrackedDeviceUserInteractionEnded:
			break;
		case vr::VREvent_IpdChanged:
			break;
		case vr::VREvent_EnterStandbyMode:
			break;
		case vr::VREvent_LeaveStandbyMode:
			break;
		case vr::VREvent_TrackedDeviceRoleChanged:
			break;
		case vr::VREvent_ButtonPress:
			break;
		case vr::VREvent_ButtonUnpress:
			break;
		case vr::VREvent_ButtonTouch:
			break;
		case vr::VREvent_ButtonUntouch:
			break;
		case vr::VREvent_MouseMove:
			break;
		case vr::VREvent_MouseButtonDown:
			break;
		case vr::VREvent_MouseButtonUp:
			break;
		case vr::VREvent_FocusEnter:
			break;
		case vr::VREvent_FocusLeave:
			break;
		case vr::VREvent_Scroll:
			break;
		case vr::VREvent_TouchPadMove:
			break;
		case vr::VREvent_InputFocusCaptured:
			break;
		case vr::VREvent_InputFocusReleased:
			break;
		case vr::VREvent_SceneFocusLost:
			break;
		case vr::VREvent_SceneFocusGained:
			break;
		case vr::VREvent_SceneApplicationChanged:
			break;
		case vr::VREvent_SceneFocusChanged:
			break;
		case vr::VREvent_InputFocusChanged:
			break;
		case vr::VREvent_HideRenderModels:
			break;
		case vr::VREvent_ShowRenderModels:
			break;
		case vr::VREvent_OverlayShown:
			break;
		case vr::VREvent_OverlayHidden:
			break;
		case vr::VREvent_DashboardActivated:
			break;
		case vr::VREvent_DashboardDeactivated:
			break;
		case vr::VREvent_DashboardThumbSelected:
			break;
		case vr::VREvent_DashboardRequested:
			break;
		case vr::VREvent_ResetDashboard:
			break;
		case vr::VREvent_RenderToast:
			break;
		case vr::VREvent_ImageLoaded:
			break;
		case vr::VREvent_ShowKeyboard:
			break;
		case vr::VREvent_HideKeyboard:
			break;
		case vr::VREvent_OverlayGamepadFocusGained:
			break;
		case vr::VREvent_OverlayGamepadFocusLost:
			break;
		case vr::VREvent_OverlaySharedTextureChanged:
			break;
		case vr::VREvent_DashboardGuideButtonDown:
			break;
		case vr::VREvent_DashboardGuideButtonUp:
			break;
		case vr::VREvent_Notification_Shown:
			break;
		case vr::VREvent_Notification_Hidden:
			break;
		case vr::VREvent_Notification_BeginInteraction:
			break;
		case vr::VREvent_Notification_Destroyed:
			break;
		case vr::VREvent_Quit:
			break;
		case vr::VREvent_ProcessQuit:
			break;
		case vr::VREvent_QuitAborted_UserPrompt:
			break;
		case vr::VREvent_QuitAcknowledged:
			break;
		case vr::VREvent_DriverRequestedQuit:
			break;
		case vr::VREvent_ChaperoneDataHasChanged:
			break;
		case vr::VREvent_ChaperoneUniverseHasChanged:
			break;
		case vr::VREvent_ChaperoneTempDataHasChanged:
			break;
		case vr::VREvent_ChaperoneSettingsHaveChanged:
			break;
		case vr::VREvent_SeatedZeroPoseReset:
			break;
		case vr::VREvent_AudioSettingsHaveChanged:
			break;
		case vr::VREvent_BackgroundSettingHasChanged:
			break;
		case vr::VREvent_CameraSettingsHaveChanged:
			break;
		case vr::VREvent_ReprojectionSettingHasChanged:
			break;
		case vr::VREvent_StatusUpdate:
			break;
		case vr::VREvent_MCImageUpdated:
			break;
		case vr::VREvent_FirmwareUpdateStarted:
			break;
		case vr::VREvent_FirmwareUpdateFinished:
			break;
		case vr::VREvent_KeyboardClosed:
			break;
		case vr::VREvent_KeyboardCharInput:
			break;
		case vr::VREvent_KeyboardDone:
			break;
		case vr::VREvent_ApplicationTransitionStarted:
			break;
		case vr::VREvent_ApplicationTransitionAborted:
			break;
		case vr::VREvent_ApplicationTransitionNewAppStarted:
			break;
		case vr::VREvent_Compositor_MirrorWindowShown:
			break;
		case vr::VREvent_Compositor_MirrorWindowHidden:
			break;
		case vr::VREvent_Compositor_ChaperoneBoundsShown:
			break;
		case vr::VREvent_Compositor_ChaperoneBoundsHidden:
			break;
		case vr::VREvent_TrackedCamera_StartVideoStream:
			break;
		case vr::VREvent_TrackedCamera_StopVideoStream:
			break;
		case vr::VREvent_TrackedCamera_PauseVideoStream:
			break;
		case vr::VREvent_TrackedCamera_ResumeVideoStream:
			break;
		case vr::VREvent_PerformanceTest_EnableCapture:
			break;
		case vr::VREvent_PerformanceTest_DisableCapture:
			break;
		case vr::VREvent_PerformanceTest_FidelityLevel:
			break;
		case vr::VREvent_VendorSpecific_Reserved_Start:
			break;
		case vr::VREvent_VendorSpecific_Reserved_End:
			break;
		default:
			break;
	}
}
