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

/**
* Constructor.
***/
OpenVR_Tracker::OpenVR_Tracker() :AQU_Nodus(),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr)
{
	ZeroMemory(&m_afTranslation[0], sizeof(float)* 3);
}

/**
* Destructor.
***/
OpenVR_Tracker::~OpenVR_Tracker()
{

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
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (false))
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
			/*szBuffer << m_sState.rotation.data[0];
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
			nY += 64; szBuffer = std::wstringstream();*/

			// position tracking
			/*szBuffer << m_sState.translation.data[0];
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
			nY += 64; szBuffer = std::wstringstream();*/

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
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::Yaw:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::Roll:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::OrientationW:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::OrientationX:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::OrientationY:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::OrientationZ:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::PositionX:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::PositionY:
			return PNT_FLOAT_PLUG_TYPE;
		case OpenVR_Commanders::PositionZ:
			return PNT_FLOAT_PLUG_TYPE;
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
			return (void*)&m_fPitch;
		case OpenVR_Commanders::Yaw:
			return (void*)&m_fYaw;
		case OpenVR_Commanders::Roll:
			return (void*)&m_fRoll;
		case OpenVR_Commanders::OrientationW:
			return nullptr;
		case OpenVR_Commanders::OrientationX:
			return nullptr;
		case OpenVR_Commanders::OrientationY:
			return nullptr;
		case OpenVR_Commanders::OrientationZ:
			return nullptr;
		case OpenVR_Commanders::PositionX:
			return (void*)&m_afTranslation[0];
		case OpenVR_Commanders::PositionY:
			return (void*)&m_afTranslation[1];
		case OpenVR_Commanders::PositionZ:
			return (void*)&m_afTranslation[2];
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

	return nullptr;
}