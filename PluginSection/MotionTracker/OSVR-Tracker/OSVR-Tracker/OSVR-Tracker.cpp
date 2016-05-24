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
	ZeroMemory(&m_afTranslation[0], sizeof(float)*3);
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
			szBuffer << m_fPitch;
			TextOut(hdcImage, 730, nY, L"Pitch X", 7);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fYaw;
			TextOut(hdcImage, 780, nY, L"Yaw Y", 5);
			TextOut(hdcImage, 200, nY, szBuffer.str().c_str(), (int)szBuffer.str().length());
			nY += 64; szBuffer = std::wstringstream();
			szBuffer << m_fRoll;
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
			return (void*)&m_fPitch;
		case OSVR_Commanders::Yaw:
			return (void*)&m_fYaw;
		case OSVR_Commanders::Roll:
			return (void*)&m_fRoll;
		case OSVR_Commanders::OrientationW:
			return nullptr;
		case OSVR_Commanders::OrientationX:
			return nullptr;
		case OSVR_Commanders::OrientationY:
			return nullptr;
		case OSVR_Commanders::OrientationZ:
			return nullptr;
		case OSVR_Commanders::PositionX:
			return (void*)&m_afTranslation[0];
		case OSVR_Commanders::PositionY:
			return (void*)&m_afTranslation[1];
		case OSVR_Commanders::PositionZ:
			return (void*)&m_afTranslation[2];
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
	if ((!m_psOSVR_ClientContext) || (!m_psOSVR_ClientInterface))
	{
		// create client context handle
		m_psOSVR_ClientContext =
			osvrClientInit("com.mtbs3d.vireio.osvr.tracker", 0);

		// get client interface
		osvrClientGetInterface(m_psOSVR_ClientContext, "/me/head", &m_psOSVR_ClientInterface);
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
				m_fYaw = 2 * atan2(x, w);
				m_fRoll = FLOAT_PI / 2;
				m_fPitch = 0;
			}
			else if (test < -0.499*unit)
			{
				// singularity at south pole
				m_fYaw = -2 * atan2(x, w);
				m_fRoll = -FLOAT_PI / 2;
				m_fPitch = 0;
			}
			else
			{
				m_fYaw = atan2(2 * y*w - 2 * x*z, sqx - sqy - sqz + sqw);
				m_fRoll = asin(2 * test / unit);
				m_fPitch = atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
			}

			// PITCH = atan2(2.0 * (x * y + w * z), w * w + x * x - y * y - z * z);
			// ROLL = atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z);

			// set position
			m_afTranslation[0] = (float)m_sState.translation.data[0];
			m_afTranslation[1] = (float)m_sState.translation.data[1];
			m_afTranslation[2] = (float)m_sState.translation.data[2];

#ifdef _DEBUG
			// output debug data
			std::wstringstream szPose;
			szPose << L"Got POSE state: Position = ("
				<< m_sState.translation.data[0] << L", "
				<< m_sState.translation.data[1] << L", "
				<< m_sState.translation.data[2] << L"), orientation = ("
				<< osvrQuatGetW(&(m_sState.rotation)) << L", "
				<< osvrQuatGetX(&(m_sState.rotation)) << L", "
				<< osvrQuatGetY(&(m_sState.rotation)) << L", "
				<< osvrQuatGetZ(&(m_sState.rotation)) << L")";
			OutputDebugString(szPose.str().c_str());
#endif
		}
	}

	return nullptr;
}