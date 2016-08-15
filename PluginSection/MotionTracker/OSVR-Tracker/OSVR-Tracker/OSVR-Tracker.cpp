/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Tracker - Open Source Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OSVR-Tracker.cpp> and
Class <OSVR-Tracker> :
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

#include"OSVR-Tracker.h"

#define INTERFACE_IDIRECT3DDEVICE9           8

/**
* Constructor.
***/
OSVR_Tracker::OSVR_Tracker() :AQU_Nodus(),
m_psOSVR_ClientContext(nullptr),
m_psOSVR_ClientInterface(nullptr),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr)
{
	ZeroMemory(&m_sState, sizeof(OSVR_PoseState));
	ZeroMemory(&m_sTimestamp, sizeof(OSVR_TimeValue));
	ZeroMemory(&m_afPosition[0], sizeof(float) * 3);
	ZeroMemory(&m_sTargetSize, sizeof(OSVR_Size));

	// set default ini file settings
	m_afPositionOrigin[0] = 0.0f;
	m_afPositionOrigin[1] = 1.7f; /**< Default y tracking origin : 1.7 meters **/
	m_afPositionOrigin[2] = 0.0f;
	m_nHotkeySync = VK_BACK;      /**< Actually we don't need that here ! **/

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	m_afPositionOrigin[0] = GetIniFileSetting(m_afPositionOrigin[0], "OSVR", "afPositionOrigin[0]", szFilePathINI, bFileExists);
	m_afPositionOrigin[1] = GetIniFileSetting(m_afPositionOrigin[1], "OSVR", "afPositionOrigin[1]", szFilePathINI, bFileExists);
	m_afPositionOrigin[2] = GetIniFileSetting(m_afPositionOrigin[2], "OSVR", "afPositionOrigin[2]", szFilePathINI, bFileExists);
	m_nHotkeySync = GetIniFileSettingKeyCode("VK_BACK", "OSVR", "nHotkeySync", szFilePathINI, bFileExists);

	m_cGameTimer.Reset();
}

/**
* Destructor.
***/
OSVR_Tracker::~OSVR_Tracker()
{
	if (m_psOSVR_ClientInterface)
		osvrClientFreeInterface(m_psOSVR_ClientContext, m_psOSVR_ClientInterface);
	if (m_psOSVR_ClientContext)
		osvrClientShutdown(m_psOSVR_ClientContext);
}

/**
* Return the name of the  OSVR Tracker node.
***/
const char* OSVR_Tracker::GetNodeType()
{
	return "OSVR Tracker";
}

/**
* Returns a global unique identifier for the OSVR Tracker node.
***/
UINT OSVR_Tracker::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 288
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OSVR Tracker node.
***/
LPWSTR OSVR_Tracker::GetCategory()
{
	return L"Motion Tracker";
}

/**
* Returns a logo to be used for the OSVR Tracker node.
***/
HBITMAP OSVR_Tracker::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OSVR-Tracker.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OSVR Tracker node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OSVR_Tracker::GetControl()
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
			L"Consolas");

		// Select the variable stock font into the specified device context. 
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (m_psOSVR_ClientContext))
		{
			int nY = 16;
			std::wstringstream szBuffer;

			SetTextColor(hdcImage, RGB(253, 128, 0));
			SetBkColor(hdcImage, RGB(238, 238, 238));

			// display the values suiteable to the data commanders... first yaw pitch roll
			szBuffer << m_afEuler[0];
			TextOut(hdcImage, 730, nY, L"Pitch X", 7);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_afEuler[1];
			TextOut(hdcImage, 780, nY, L"Yaw Y", 5);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_afEuler[2];
			TextOut(hdcImage, 765, nY, L"Roll Z", 6);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// orientation
			szBuffer << m_sState.rotation.data[0];
			TextOut(hdcImage, 570, nY, L"Orientation W", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sState.rotation.data[1];
			TextOut(hdcImage, 570, nY, L"Orientation X", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sState.rotation.data[2];
			TextOut(hdcImage, 570, nY, L"Orientation Y", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sState.rotation.data[3];
			TextOut(hdcImage, 570, nY, L"Orientation Z", 13);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

			// position tracking
			szBuffer << m_sState.translation.data[0];
			TextOut(hdcImage, 650, nY, L"Position X", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sState.translation.data[1];
			TextOut(hdcImage, 650, nY, L"Position Y", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_sState.translation.data[2];
			TextOut(hdcImage, 650, nY, L"Position Z", 10);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();

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
LPWSTR OSVR_Tracker::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case OSVR_Commanders::Pitch:
			return L"Pitch";
		case OSVR_Commanders::Yaw:
			return L"Yaw";
		case OSVR_Commanders::Roll:
			return L"Roll";
		case OSVR_Commanders::OrientationW:
			return L"Orientation W";
		case OSVR_Commanders::OrientationX:
			return L"Orientation X";
		case OSVR_Commanders::OrientationY:
			return L"Orientation Y";
		case OSVR_Commanders::OrientationZ:
			return L"Orientation Z";
		case OSVR_Commanders::PositionX:
			return L"Position X";
		case OSVR_Commanders::PositionY:
			return L"Position Y";
		case OSVR_Commanders::PositionZ:
			return L"Position Z";
		case OSVR_Commanders::View:
			return L"View";
		case OSVR_Commanders::ProjectionLeft:
			return L"ProjectionLeft";
		case OSVR_Commanders::ProjectionRight:
			return L"ProjectionRight";
		case OSVR_Commanders::TargetWidth:
			return L"TargetWidth";
		case OSVR_Commanders::TargetHeight:
			return L"TargetHeight";
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OSVR_Tracker::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case OSVR_Commanders::Pitch:
		case OSVR_Commanders::Yaw:
		case OSVR_Commanders::Roll:
		case OSVR_Commanders::OrientationW:
		case OSVR_Commanders::OrientationX:
		case OSVR_Commanders::OrientationY:
		case OSVR_Commanders::OrientationZ:
		case OSVR_Commanders::PositionX:
		case OSVR_Commanders::PositionY:
		case OSVR_Commanders::PositionZ:
			return NOD_Plugtype::AQU_FLOAT;
		case OSVR_Commanders::View:
		case OSVR_Commanders::ProjectionLeft:
		case OSVR_Commanders::ProjectionRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
		case OSVR_Commanders::TargetWidth:
		case OSVR_Commanders::TargetHeight:
			return NOD_Plugtype::AQU_UINT;
	}

	return 0;
}

/**
* Provides the pointer of the requested commander.
***/
void* OSVR_Tracker::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((OSVR_Commanders)dwCommanderIndex)
	{
		case OSVR_Commanders::Pitch:
			return (void*)&m_afEulerPredicted[0];
		case OSVR_Commanders::Yaw:
			return (void*)&m_afEulerPredicted[1];
		case OSVR_Commanders::Roll:
			return (void*)&m_afEulerPredicted[2];
		case OSVR_Commanders::OrientationW:
			return (void*)&m_sState.rotation.data[0];
		case OSVR_Commanders::OrientationX:
			return (void*)&m_sState.rotation.data[1];
		case OSVR_Commanders::OrientationY:
			return (void*)&m_sState.rotation.data[2];
		case OSVR_Commanders::OrientationZ:
			return (void*)&m_sState.rotation.data[3];
		case OSVR_Commanders::PositionX:
			return (void*)&m_afPosition[0];
		case OSVR_Commanders::PositionY:
			return (void*)&m_afPosition[1];
		case OSVR_Commanders::PositionZ:
			return (void*)&m_afPosition[2];
		case OSVR_Commanders::View:
			return (void*)&m_sView;
		case OSVR_Commanders::ProjectionLeft:
			return (void*)&m_asProjection[0];
		case OSVR_Commanders::ProjectionRight:
			return (void*)&m_asProjection[1];
		case OSVR_Commanders::TargetWidth:
			return (void*)&m_sTargetSize.unWidth;
		case OSVR_Commanders::TargetHeight:
			return (void*)&m_sTargetSize.unHeight;
	}

	return nullptr;
}

/**
* Tracker supports any calls.
***/
bool OSVR_Tracker::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OSVR tracking.
***/
void* OSVR_Tracker::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	// update game timer
	m_cGameTimer.Tick();

	static UINT unFrameSkip = 200;
	if (unFrameSkip > 0)
	{
		unFrameSkip--;
		return nullptr;
	}

	if ((!m_psOSVR_ClientContext) || (!m_psOSVR_ClientInterface))
	{
		// create client context handle
		m_psOSVR_ClientContext =
			osvrClientInit("com.mtbs3d.vireio.osvr.tracker", 0);

		// get client interface
		osvrClientGetInterface(m_psOSVR_ClientContext, "/me/head", &m_psOSVR_ClientInterface);

		// get the projection matrix for each eye, OSVR direct mode node will override this setting eventually
		for (UINT unEye = 0; unEye < 2; unEye++)
		{
			D3DXMATRIX sProj;
			D3DXMatrixPerspectiveFovLH(&sProj, (float)D3DXToRadian(116.0), 1.0f, 0.1f, 30.0f);

			// create eye pose matrix... first, use standard IPD (0.064 meters)
			D3DXMATRIX sToEye;
			if (unEye)
				D3DXMatrixTranslation(&sToEye, -0.032f, 0, 0);
			else
				D3DXMatrixTranslation(&sToEye, 0.032f, 0, 0);

			m_asProjection[unEye] = sToEye * sProj;
		}
	}
	else
	{
		// update the client context
		osvrClientUpdate(m_psOSVR_ClientContext);

		// let's read the tracker state.
		OSVR_ReturnCode cRet =
			osvrGetPoseState(m_psOSVR_ClientInterface, &m_sTimestamp, &m_sState);
		if (cRet != OSVR_RETURN_SUCCESS)
		{
			OutputDebugStringA("No pose state!\n");
		}
		else
		{
			m_bControlUpdate = true;

			// backup old euler angles and velocity
			float afEulerOld[3];
			float afEulerVelocityOld[3];
			memcpy(&afEulerOld[0], &m_afEuler[0], sizeof(float) * 3);
			memcpy(&afEulerVelocityOld[0], &m_afEulerVelocity[0], sizeof(float) * 3);

			// quaternion -> euler angles
			const float w = (float)m_sState.rotation.data[0];
			const float x = (float)m_sState.rotation.data[1];
			const float y = (float)m_sState.rotation.data[2];
			const float z = (float)m_sState.rotation.data[3];

			float sqw = w*w;
			float sqx = x*x;
			float sqy = y*y;
			float sqz = z*z;

			float unit = sqx + sqy + sqz + sqw;
			float test = x*y + z*w;

			if (test > 0.499*unit)
			{
				// singularity at north pole
				m_afEuler[1] = 2 * atan2(x, w);
				m_afEuler[2] = FLOAT_PI / 2;
				m_afEuler[0] = 0;
			}
			else if (test < -0.499*unit)
			{
				// singularity at south pole
				m_afEuler[1] = -2 * atan2(x, w);
				m_afEuler[2] = -FLOAT_PI / 2;
				m_afEuler[0] = 0;
			}
			else
			{
				m_afEuler[1] = atan2(2 * y*w - 2 * x*z, sqx - sqy - sqz + sqw);
				m_afEuler[2] = asin(2 * test / unit);
				m_afEuler[0] = atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
			}

			// PITCH = atan2(2.0 * (x * y + w * z), w * w + x * x - y * y - z * z);
			// ROLL = atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z);

			// prevent from yaw prediction overturn
			float fYawChange = abs(afEulerOld[1] - m_afEuler[1]);
			if (fYawChange > 3.14f) afEulerOld[1] = m_afEuler[1];

			// get euler velocity + acceleration
			float afEulerAcceleration[3];
			for (UINT unI = 0; unI < 3; unI++)
			{
				// get the velocity
				m_afEulerVelocity[unI] = (m_afEuler[unI] - afEulerOld[unI]) / (float)m_cGameTimer.DeltaTime();

				// get the acceleration
				afEulerAcceleration[unI] = (m_afEulerVelocity[unI] - afEulerVelocityOld[unI]) / (float)m_cGameTimer.DeltaTime();
			}

			// get predicted euler
			for (UINT unI = 0; unI < 3; unI++)
			{
				// compute predicted euler
				m_afEulerPredicted[unI] = (0.5f * afEulerAcceleration[unI] * ((float)m_cGameTimer.DeltaTime() * (float)m_cGameTimer.DeltaTime())) + (m_afEulerVelocity[unI] * (float)m_cGameTimer.DeltaTime()) + m_afEuler[unI];
			}

			// resync yaw ?
			static float s_fYawOrigin = 0.0f;
			if (GetAsyncKeyState(m_nHotkeySync))
			{
				s_fYawOrigin = -m_afEulerPredicted[1];
			}

			// sync yaw to origin
			m_afEulerPredicted[1] += s_fYawOrigin;

			// set position TODO !! TEST ORIGIN SETTINGS USING POSITIONAL TRACKING
			m_afPosition[0] = (float)m_sState.translation.data[0] - m_afPositionOrigin[0];
			m_afPosition[1] = (float)m_sState.translation.data[1] - m_afPositionOrigin[1];
			m_afPosition[2] = (float)m_sState.translation.data[2] + m_afPositionOrigin[2];

			// create view matrix from rotation and position
			D3DXMATRIX sRotation;
			D3DXMATRIX sPitch, sYaw, sRoll;
			D3DXMatrixRotationX(&sPitch, m_afEulerPredicted[0]);
			D3DXMatrixRotationY(&sYaw, m_afEulerPredicted[1]);
			D3DXMatrixRotationZ(&sRoll, -m_afEulerPredicted[2]);
			sRotation = sYaw * sPitch * sRoll;
			D3DXMATRIX sTranslation;
			D3DXMatrixTranslation(&sTranslation, (float)m_afPosition[0], (float)m_afPosition[1], (float)m_afPosition[2]);
			m_sView = sTranslation * sRotation;

#ifdef _DEBUG
			// output debug data
			std::wstringstream szPose;
			/*szPose << L"Got POSE state: Position = ("
				<< m_sState.translation.data[0] << L", "
				<< m_sState.translation.data[1] << L", "
				<< m_sState.translation.data[2] << L"), orientation = ("
				<< osvrQuatGetW(&(m_sState.rotation)) << L", "
				<< osvrQuatGetX(&(m_sState.rotation)) << L", "
				<< osvrQuatGetY(&(m_sState.rotation)) << L", "
				<< osvrQuatGetZ(&(m_sState.rotation)) << L")";*/
			szPose << L"Got POSE state: Rotation = ("
				<< m_afEulerPredicted[0] << L", "
				<< m_afEulerPredicted[1] << L", "
				<< -m_afEulerPredicted[2] << L")";
			OutputDebugString(szPose.str().c_str());
#endif
		}
	}

	return nullptr;
}