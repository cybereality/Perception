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
* Number of Oculus controller buttons.
***/
static const UINT unButtonNo = 13;

/**
* Ids of Oculus controller buttons.
* Remote, Touch + XBox controller.
***/
static UINT aunButtonIds[] =
{
	ovrButton::ovrButton_A,
	ovrButton::ovrButton_B,
	ovrButton::ovrButton_RThumb,
	ovrButton::ovrButton_RShoulder,
	ovrButton::ovrButton_X,
	ovrButton::ovrButton_Y,
	ovrButton::ovrButton_LThumb,
	ovrButton::ovrButton_LShoulder,
	ovrButton::ovrButton_Up,
	ovrButton::ovrButton_Down,
	ovrButton::ovrButton_Left,
	ovrButton::ovrButton_Right,
	ovrButton::ovrButton_Enter,
	ovrButton::ovrButton_Back,
	ovrButton::ovrButton_VolUp,
	ovrButton::ovrButton_VolDown,
	ovrButton::ovrButton_Home,

	// TODO !! axis as buttons
};

/**
* Simple indexation of all possible ovr buttons.
***/
enum OvrButtonIx
{
	Index_ovrButton_A,
	Index_ovrButton_B,
	Index_ovrButton_RThumb,
	Index_ovrButton_RShoulder,
	Index_ovrButton_X,
	Index_ovrButton_Y,
	Index_ovrButton_LThumb,
	Index_ovrButton_LShoulder,
	Index_ovrButton_Up,
	Index_ovrButton_Down,
	Index_ovrButton_Left,
	Index_ovrButton_Right,
	Index_ovrButton_Enter,
	Index_ovrButton_Back,
	Index_ovrButton_VolUp,
	Index_ovrButton_VolDown,
	Index_ovrButton_Home,
	Index_ovrButton_UpL,        /**< Left thumb stick. ***/
	Index_ovrButton_DownL,
	Index_ovrButton_LeftL,
	Index_ovrButton_RightL,
	Index_ovrButton_UpR,        /**< Right thumb stick. ***/
	Index_ovrButton_DownR,
	Index_ovrButton_LeftR,
	Index_ovrButton_RightR,
};

/**
* Names for the menu of the Touch controls.
***/
const std::string astrTouchControls[] =
{
	"Touch A",
	"Touch B",
	"Touch RThumb",
	"Touch RShoulder",
	"Touch X",
	"Touch Y",
	"Touch LThumb",
	"Touch LShoulder",
	"Touch Up",
	"Touch Down",
	"Touch Left",
	"Touch Right",
	"Touch Enter",
	"Touch Back",
	"Touch VolUp",
	"Touch VolDown",
	"Touch Home",
	"Touch UpL",        /**< Left thumb stick. ***/
	"Touch DownL",
	"Touch LeftL",
	"Touch RightL",
	"Touch UpR",        /**< Right thumb stick. ***/
	"Touch DownR",
	"Touch LeftR",
	"Touch RightR"
};

/**
* Constructor.
***/
OculusTracker::OculusTracker() :AQU_Nodus(),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_unRenderTextureWidth(0),
m_unRenderTextureHeight(0),
m_hSession(nullptr),
m_nIniFrameCount(0)
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

	// Touch predefined keyboard codes
	std::string astrVKCodes_Touch[] = { "VK_RETURN", // Index_ovrButton_A
		"VK_SPACE", // Index_ovrButton_B
		"X", // Index_ovrButton_RThumb
		"VK_BACK", // Index_ovrButton_RShoulder
		"VK_LBUTTON", // Index_ovrButton_X
		"VK_RBUTTON", // Index_ovrButton_Y
		"X", // Index_ovrButton_LThumb
		"VK_TAB", // Index_ovrButton_LShoulder
		"X", // Index_ovrButton_Up
		"X", // Index_ovrButton_Down
		"X", // Index_ovrButton_Left
		"X", // Index_ovrButton_Right
		"VK_ESCAPE", // Index_ovrButton_Enter
		"X", // Index_ovrButton_Back
		"X", // Index_ovrButton_VolUp
		"X", // Index_ovrButton_VolDown
		"VK_C", // Index_ovrButton_Home
		"VK_W", // Index_ovrButton_UpL
		"VK_S", // Index_ovrButton_DownL
		"VK_A", // Index_ovrButton_LeftL
		"VK_D", // Index_ovrButton_RightL
		"WM_MOUSEMOVE", // Index_ovrButton_UpR
		"WM_MOUSEMOVE", // Index_ovrButton_DownR
		"WM_MOUSEMOVE", // Index_ovrButton_LeftR
		"WM_MOUSEMOVE" }; // Index_ovrButton_RightR

	// set inner scope
	for (UINT unI = 0; unI < 5; unI++)
	{
		m_aafAxisScopeOrFactor[0][unI] = 0.8f;
		m_aafAxisScopeOrFactor[1][unI] = 0.8f;
	}

	// set movement factor for trackpad 0
	m_aafAxisScopeOrFactor[0][0] = 10.0f;

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

	// set default key codes > Touch
	m_aaunKeys[1][Index_ovrButton_A] = GetIniFileSettingKeyCode(astrVKCodes_Touch[0], "LibOVR", "aaunKeys[1][Index_ovrButton_A]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_B] = GetIniFileSettingKeyCode(astrVKCodes_Touch[1], "LibOVR", "aaunKeys[1][Index_ovrButton_B]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RThumb] = GetIniFileSettingKeyCode(astrVKCodes_Touch[2], "LibOVR", "aaunKeys[1][Index_ovrButton_RThumb]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RShoulder] = GetIniFileSettingKeyCode(astrVKCodes_Touch[3], "LibOVR", "aaunKeys[1][Index_ovrButton_RShoulder]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_X] = GetIniFileSettingKeyCode(astrVKCodes_Touch[4], "LibOVR", "aaunKeys[1][Index_ovrButton_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Y] = GetIniFileSettingKeyCode(astrVKCodes_Touch[5], "LibOVR", "aaunKeys[1][Index_ovrButton_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LThumb] = GetIniFileSettingKeyCode(astrVKCodes_Touch[6], "LibOVR", "aaunKeys[1][Index_ovrButton_LThumb]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LShoulder] = GetIniFileSettingKeyCode(astrVKCodes_Touch[7], "LibOVR", "aaunKeys[1][Index_ovrButton_LShoulder]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Up] = GetIniFileSettingKeyCode(astrVKCodes_Touch[8], "LibOVR", "aaunKeys[1][Index_ovrButton_Up]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Down] = GetIniFileSettingKeyCode(astrVKCodes_Touch[9], "LibOVR", "aaunKeys[1][Index_ovrButton_Down]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Left] = GetIniFileSettingKeyCode(astrVKCodes_Touch[10], "LibOVR", "aaunKeys[1][Index_ovrButton_Left]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Right] = GetIniFileSettingKeyCode(astrVKCodes_Touch[11], "LibOVR", "aaunKeys[1][Index_ovrButton_Right]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Enter] = GetIniFileSettingKeyCode(astrVKCodes_Touch[12], "LibOVR", "aaunKeys[1][Index_ovrButton_Enter]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Back] = GetIniFileSettingKeyCode(astrVKCodes_Touch[13], "LibOVR", "aaunKeys[1][Index_ovrButton_Back]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_VolUp] = GetIniFileSettingKeyCode(astrVKCodes_Touch[14], "LibOVR", "aaunKeys[1][Index_ovrButton_VolUp]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_VolDown] = GetIniFileSettingKeyCode(astrVKCodes_Touch[15], "LibOVR", "aaunKeys[1][Index_ovrButton_VolDown]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Home] = GetIniFileSettingKeyCode(astrVKCodes_Touch[16], "LibOVR", "aaunKeys[1][Index_ovrButton_Home]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_UpL] = GetIniFileSettingKeyCode(astrVKCodes_Touch[17], "LibOVR", "aaunKeys[1][Index_ovrButton_UpL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_DownL] = GetIniFileSettingKeyCode(astrVKCodes_Touch[18], "LibOVR", "aaunKeys[1][Index_ovrButton_DownL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LeftL] = GetIniFileSettingKeyCode(astrVKCodes_Touch[19], "LibOVR", "aaunKeys[1][Index_ovrButton_LeftL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RightL] = GetIniFileSettingKeyCode(astrVKCodes_Touch[20], "LibOVR", "aaunKeys[1][Index_ovrButton_RightL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_UpR] = GetIniFileSettingKeyCode(astrVKCodes_Touch[21], "LibOVR", "aaunKeys[1][Index_ovrButton_UpR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_DownR] = GetIniFileSettingKeyCode(astrVKCodes_Touch[22], "LibOVR", "aaunKeys[1][Index_ovrButton_DownR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LeftR] = GetIniFileSettingKeyCode(astrVKCodes_Touch[23], "LibOVR", "aaunKeys[1][Index_ovrButton_LeftR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RightR] = GetIniFileSettingKeyCode(astrVKCodes_Touch[24], "LibOVR", "aaunKeys[1][Index_ovrButton_RightR]", szFilePathINI, bFileExists);


	// erase key bool field
	ZeroMemory(&m_aabKeys[0][0], sizeof(BOOL)* 3 * 25);

	// extended keys set ?
	for (UINT unI = 0; unI < 3; unI++)
	for (UINT unJ = 0; unJ < 25; unJ++)
	{
		if ((m_aaunKeys[unI][unJ] == VK_UP) ||
			(m_aaunKeys[unI][unJ] == VK_DOWN) ||
			(m_aaunKeys[unI][unJ] == VK_LBUTTON) ||
			(m_aaunKeys[unI][unJ] == VK_UP) ||
			(m_aaunKeys[unI][unJ] == VK_SHIFT) ||
			(m_aaunKeys[unI][unJ] == VK_CONTROL) ||
			(m_aaunKeys[unI][unJ] == VK_BACK) ||
			(m_aaunKeys[unI][unJ] == VK_INSERT) ||
			(m_aaunKeys[unI][unJ] == VK_DELETE) ||
			(m_aaunKeys[unI][unJ] == VK_HOME) ||
			(m_aaunKeys[unI][unJ] == VK_END) ||
			(m_aaunKeys[unI][unJ] == VK_MBUTTON) ||
			(m_aaunKeys[unI][unJ] == VK_LBUTTON) ||
			(m_aaunKeys[unI][unJ] == VK_RBUTTON) ||
			(m_aaunKeys[unI][unJ] == VK_LCONTROL) ||
			(m_aaunKeys[unI][unJ] == VK_RCONTROL) ||
			(m_aaunKeys[unI][unJ] == VK_RMENU) ||
			(m_aaunKeys[unI][unJ] == VK_LMENU))
			m_aabKeyExtended[unI][unJ] = TRUE;
		else
			m_aabKeyExtended[unI][unJ] = FALSE;
	}

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Oculus Tracker";

	// add touch menu options
	static UINT s_unDummy[26];
	for (UINT unIx = 0; unIx < 25; unIx++)
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = astrTouchControls[unIx];
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = 1;
		sEntry.unChangeSize = 1;
		sEntry.punValue = &s_unDummy[unIx];
		sEntry.unValue = s_unDummy[unIx];
		sEntry.bValueEnumeration = true;

		// add "X"
		std::string strX = std::string("X");
		sEntry.astrValueEnumeration.push_back(strX);

		// loop through VK codes, add possible strings
		for (UINT unVK = 1; unVK < 256; unVK++)
		{
			// get string
			std::string strEnum = GetStringByVKCode(unVK);
			if (strEnum != "X")
			{
				// is current selection ?
				if (unVK == m_aaunKeys[1][unIx])
				{
					s_unDummy[unIx] = sEntry.unValue = (UINT)sEntry.astrValueEnumeration.size();
				}

				sEntry.astrValueEnumeration.push_back(strEnum);
				sEntry.unMaximum++;
			}
		}

		m_sMenu.asEntries.push_back(sEntry);
	}
	// position origin x
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Origin X";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -5.0f;
		sEntry.fMaximum = 5.0f;
		sEntry.fChangeSize = 0.01f;
		sEntry.pfValue = &m_afPositionOrigin[0];
		sEntry.fValue = m_afPositionOrigin[0];
		m_sMenu.asEntries.push_back(sEntry);
	}
	// position origin y
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Origin Y";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -5.0f;
		sEntry.fMaximum = 5.0f;
		sEntry.fChangeSize = 0.01f;
		sEntry.pfValue = &m_afPositionOrigin[1];
		sEntry.fValue = m_afPositionOrigin[1];
		m_sMenu.asEntries.push_back(sEntry);
	}
	// position origin z
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Origin Z";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -5.0f;
		sEntry.fMaximum = 5.0f;
		sEntry.fChangeSize = 0.01f;
		sEntry.pfValue = &m_afPositionOrigin[2];
		sEntry.fValue = m_afPositionOrigin[2];
		m_sMenu.asEntries.push_back(sEntry);
	}
}

/**
* Destructor.
***/
OculusTracker::~OculusTracker()
{
	if (m_hSession)
	{
		// set performance hud to zero
		ovr_SetInt(m_hSession, OVR_PERF_HUD_MODE, 0);
		ovr_Destroy(m_hSession);
	}
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
		if ((hOldFont = (HFONT)SelectObject(hdcImage, m_hFont)) && (m_hSession))
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
			if (m_hSession)
				wsprintf(szBufferW, L"%u", m_unRenderTextureWidth);
			else
				wsprintf(szBufferW, L"0");
			TextOut(hdcImage, 510, nY, L"TexResolution Width", 19);
			int nLen = (int)wcslen(szBufferW); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 50, nY, szBufferW, nLen); nY += 64;
			if (m_hSession)
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

			// handle, matrices, menu
			TextOut(hdcImage, 150, nY, L"HMD Handle", 10); nY += 64;
			TextOut(hdcImage, 150, nY, L"View", 4); nY += 64;
			TextOut(hdcImage, 150, nY, L"Projection Left", 15); nY += 64;
			TextOut(hdcImage, 150, nY, L"Projection Right", 16); nY += 64;
			TextOut(hdcImage, 150, nY, L"Vireio Menu", 11); nY += 64;

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
		case VireioMenu:
			return L"Vireio Menu";
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
		case VireioMenu:
			return NOD_Plugtype::AQU_VOID;
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
			return (void*)&m_hSession;
		case OTR_Commanders::View:
			return (void*)&m_sView;
		case OTR_Commanders::ProjectionLeft:
			return (void*)&m_asProjection[0];
		case OTR_Commanders::ProjectionRight:
			return (void*)&m_asProjection[1];
		case VireioMenu:
			return (void*)&m_sMenu;
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

	// save ini file ?
	if (m_nIniFrameCount)
	{
		if (m_nIniFrameCount == 1)
			SaveIniSettings();
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

				// touch entries ?
				if (nIx < 25)
				{
					// set new vk code by string
					m_aaunKeys[1][nIx] = GetVkCodeByString(m_sMenu.asEntries[nIx].astrValueEnumeration[m_sMenu.asEntries[nIx].unValue]);
				}
			}
		}
	}

	if (m_hSession)
	{
#pragma region controller
		// controller indices
		static const uint32_t s_unIndexRemote = 0;
		static const uint32_t s_unIndexTouch = 1;
		static const uint32_t s_unIndexXBox = 2;

		// get all connected input states
		ovrInputState sInputState[3] = {};
		unsigned int unControllersConnected = ovr_GetConnectedControllerTypes(m_hSession);
#pragma region Remote
		if (unControllersConnected & ovrControllerType_Remote)
		{
			ovr_GetInputState(m_hSession, ovrControllerType_Remote, &sInputState[s_unIndexRemote]);

			// handle all remote buttons except Oculus private ones
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Up)
				m_sMenu.bOnUp = true;
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Down)
				m_sMenu.bOnDown = true;
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Left)
				m_sMenu.bOnLeft = true;
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Right)
				m_sMenu.bOnRight = true;
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Enter)
				m_sMenu.bOnAccept = true;
			if (sInputState[s_unIndexRemote].Buttons & ovrButton_Back)
				m_sMenu.bOnBack = true;
		}
#pragma endregion
#pragma region touch
		if (unControllersConnected & ovrControllerType_Touch)
		{
			// get input state
			ovr_GetInputState(m_hSession, ovrControllerType_Touch, &sInputState[s_unIndexTouch]);

			// loop through controller buttons
			for (UINT unButtonIx = 0; unButtonIx < unButtonNo; unButtonIx++)
			{
				// cast keyboard event
				if (sInputState[s_unIndexTouch].Buttons & aunButtonIds[unButtonIx])
				{
					if (!m_aabKeys[s_unIndexTouch][unButtonIx])
						MapButtonDown(s_unIndexTouch, unButtonIx);
				}
				else
				if (m_aabKeys[s_unIndexTouch][unButtonIx])
					MapButtonUp(s_unIndexTouch, unButtonIx);
			}
		}
#pragma endregion
		if (unControllersConnected & ovrControllerType_XBox)
			ovr_GetInputState(m_hSession, ovrControllerType_XBox, &sInputState[s_unIndexXBox]);



#pragma endregion
#pragma region hmd
		/*// Start the sensor which informs of the Rift's pose and motion   .... obsolete for SDK 1.3.x ??
		ovr_ConfigureTracking(m_hSession, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection |
		ovrTrackingCap_Position, 0);*/

		// get the current tracking state
		ovrTrackingState sTrackingState = ovr_GetTrackingState(m_hSession, ovr_GetTimeInSeconds(), false);

		if (TRUE)//(sTrackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
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

			// predicted euler angles ? for Oculus, due to ATW, we do not predict the euler angles
			if (FALSE)
			{
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
			}
			else
			{
				// get angles
				m_sOrientation.GetEulerAngles<Axis::Axis_Y, Axis::Axis_X, Axis::Axis_Z, RotateDirection::Rotate_CW, HandedSystem::Handed_R >(&m_fEulerPredicted[1], &m_fEulerPredicted[0], &m_fEulerPredicted[2]);

				// quick fix here...
				m_fEulerPredicted[1] *= -1.0f;
				m_fEulerPredicted[0] *= -1.0f;
				m_fEulerPredicted[2] *= -1.0f;
			}

			// set the drawing update to true
			m_bControlUpdate = true;

			// set position
			m_afPosition[0] = (float)-m_sPose.Position.x - m_afPositionOrigin[0];
			m_afPosition[1] = (float)-m_sPose.Position.y - m_afPositionOrigin[1];
			m_afPosition[2] = (float)m_sPose.Position.z + m_afPositionOrigin[2];

			// get eye render pose and other fields
			ovrEyeRenderDesc asEyeRenderDesc[2];
			asEyeRenderDesc[0] = ovr_GetRenderDesc(m_hSession, ovrEye_Left, m_sHMDDesc.DefaultEyeFov[0]);
			asEyeRenderDesc[1] = ovr_GetRenderDesc(m_hSession, ovrEye_Right, m_sHMDDesc.DefaultEyeFov[1]);
			ovrVector3f      asHmdToEyeViewOffset[2] = { asEyeRenderDesc[0].HmdToEyeOffset, asEyeRenderDesc[1].HmdToEyeOffset };
			ovrPosef         asEyeRenderPose[2];
			ovr_CalcEyePoses(sTrackingState.HeadPose.ThePose, asHmdToEyeViewOffset, asEyeRenderPose);
			D3DXMATRIX sView[2];
			for (UINT unEye = 0; unEye < 2; unEye++)
			{
				// create view matrix from rotation and position
				D3DXMATRIX sRotation;
				D3DXMATRIX sPitch, sYaw, sRoll;
				D3DXMatrixRotationX(&sPitch, m_fEulerPredicted[0]);
				D3DXMatrixRotationY(&sYaw, m_fEulerPredicted[1]);
				D3DXMatrixRotationZ(&sRoll, -m_fEulerPredicted[2]);
				sRotation = sYaw * sPitch * sRoll;
				D3DXMATRIX sTranslation;
				D3DXMatrixTranslation(&sTranslation, (float)-asEyeRenderPose[unEye].Position.x - m_afPositionOrigin[0], (float)-asEyeRenderPose[unEye].Position.y - m_afPositionOrigin[1], (float)asEyeRenderPose[unEye].Position.z + m_afPositionOrigin[2]);
				sView[unEye] = sTranslation * sRotation;
			}
			D3DXMatrixIdentity(&m_sView);

			// get projection matrices left/right
			D3DXMATRIX asToEye[2];
			D3DXMATRIX asProjection[2];
			for (UINT unEye = 0; unEye < 2; unEye++)
			{
				// get ovr projection
				ovrMatrix4f sProj = ovrMatrix4f_Projection(m_sHMDDesc.DefaultEyeFov[unEye], 0.01f, 30.0f, ovrProjection_LeftHanded);

				// create dx projection
				asProjection[unEye] = D3DXMATRIX(&sProj.M[0][0]);
				D3DXMatrixTranspose(&asProjection[unEye], &asProjection[unEye]);

				// create eventual projection
				m_asProjection[unEye] = sView[unEye] * asProjection[unEye];
			}
		}
#pragma endregion
	}
	else
	{
		// Initialize LibOVR, and the Rift... then create hmd handle
		ovrResult result = ovr_Initialize(nullptr);
		if (!OVR_SUCCESS(result))
		{
			OutputDebugString(L"[OVR] Failed to initialize libOVR.");
			return nullptr;
		}

		result = ovr_Create(&m_hSession, &m_sLuid);
		if (!OVR_SUCCESS(result))
		{
			OutputDebugString(L"[OVR] Failed to retreive HMD handle.");
			return nullptr;
		}
		else
			OutputDebugString(L"[OVR] HMD handle initialized !");

		if (m_hSession)
		{
			// get the description and set pointers
			m_sHMDDesc = ovr_GetHmdDesc(m_hSession);

			// Configure Stereo settings.
			ovrSizei sRecommenedTex0Size = ovr_GetFovTextureSize(m_hSession, ovrEye_Left,
				m_sHMDDesc.DefaultEyeFov[0], 1.0f);
			ovrSizei sRecommenedTex1Size = ovr_GetFovTextureSize(m_hSession, ovrEye_Right,
				m_sHMDDesc.DefaultEyeFov[1], 1.0f);

			ovrSizei sTextureSize;
			sTextureSize.w = max(sRecommenedTex0Size.w, sRecommenedTex1Size.w);
			sTextureSize.h = max(sRecommenedTex0Size.h, sRecommenedTex1Size.h);
			m_unRenderTextureWidth = (UINT)sTextureSize.w;
			m_unRenderTextureHeight = (UINT)sTextureSize.h;

			// get view offset
			ovrEyeRenderDesc asEyeRenderDesc[2];
			asEyeRenderDesc[0] = ovr_GetRenderDesc(m_hSession, ovrEye_Left, m_sHMDDesc.DefaultEyeFov[0]);
			asEyeRenderDesc[1] = ovr_GetRenderDesc(m_hSession, ovrEye_Right, m_sHMDDesc.DefaultEyeFov[1]);
			ovrVector3f asViewOffset[2] = { asEyeRenderDesc[0].HmdToEyeOffset, asEyeRenderDesc[1].HmdToEyeOffset };

			// get projection matrices left/right
			D3DXMATRIX asToEye[2];
			D3DXMATRIX asProjection[2];
			for (UINT unEye = 0; unEye < 2; unEye++)
			{
				// get ovr projection
				ovrMatrix4f sProj = ovrMatrix4f_Projection(m_sHMDDesc.DefaultEyeFov[unEye], 0.01f, 30.0f, ovrProjection_LeftHanded);

				// create dx projection
				asProjection[unEye] = D3DXMATRIX(&sProj.M[0][0]);
				D3DXMatrixTranspose(&asProjection[unEye], &asProjection[unEye]);

				// create view offset translation matrix
				D3DXMatrixTranslation(&asToEye[unEye], -asViewOffset[unEye].x, -asViewOffset[unEye].y, -asViewOffset[unEye].z);

				// create eventual projection
				m_asProjection[unEye] = asToEye[unEye] * asProjection[unEye];
			}
		}
	}

	return nullptr;
}

/**
* Saves all node related ini file settings.
***/
void OculusTracker::SaveIniSettings()
{
	// get file path
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;

	// save ini file
	m_aaunKeys[1][Index_ovrButton_A] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][0]), "LibOVR", "aaunKeys[1][Index_ovrButton_A]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_B] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][1]), "LibOVR", "aaunKeys[1][Index_ovrButton_B]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RThumb] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][2]), "LibOVR", "aaunKeys[1][Index_ovrButton_RThumb]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RShoulder] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][3]), "LibOVR", "aaunKeys[1][Index_ovrButton_RShoulder]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_X] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][4]), "LibOVR", "aaunKeys[1][Index_ovrButton_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Y] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][5]), "LibOVR", "aaunKeys[1][Index_ovrButton_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LThumb] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][6]), "LibOVR", "aaunKeys[1][Index_ovrButton_LThumb]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LShoulder] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][7]), "LibOVR", "aaunKeys[1][Index_ovrButton_LShoulder]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Up] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][8]), "LibOVR", "aaunKeys[1][Index_ovrButton_Up]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Down] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][9]), "LibOVR", "aaunKeys[1][Index_ovrButton_Down]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Left] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][10]), "LibOVR", "aaunKeys[1][Index_ovrButton_Left]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Right] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][11]), "LibOVR", "aaunKeys[1][Index_ovrButton_Right]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Enter] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][12]), "LibOVR", "aaunKeys[1][Index_ovrButton_Enter]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Back] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][13]), "LibOVR", "aaunKeys[1][Index_ovrButton_Back]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_VolUp] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][14]), "LibOVR", "aaunKeys[1][Index_ovrButton_VolUp]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_VolDown] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][15]), "LibOVR", "aaunKeys[1][Index_ovrButton_VolDown]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_Home] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][16]), "LibOVR", "aaunKeys[1][Index_ovrButton_Home]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_UpL] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][17]), "LibOVR", "aaunKeys[1][Index_ovrButton_UpL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_DownL] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][18]), "LibOVR", "aaunKeys[1][Index_ovrButton_DownL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LeftL] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][19]), "LibOVR", "aaunKeys[1][Index_ovrButton_LeftL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RightL] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][20]), "LibOVR", "aaunKeys[1][Index_ovrButton_RightL]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_UpR] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][21]), "LibOVR", "aaunKeys[1][Index_ovrButton_UpR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_DownR] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][22]), "LibOVR", "aaunKeys[1][Index_ovrButton_DownR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_LeftR] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][23]), "LibOVR", "aaunKeys[1][Index_ovrButton_LeftR]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_ovrButton_RightR] = GetIniFileSettingKeyCode(GetStringByVKCode(m_aaunKeys[1][24]), "LibOVR", "aaunKeys[1][Index_ovrButton_RightR]", szFilePathINI, bFileExists);
	m_afPositionOrigin[0] = GetIniFileSetting(m_afPositionOrigin[0], "LibOVR", "afPositionOrigin[0]", szFilePathINI, bFileExists);
	m_afPositionOrigin[1] = GetIniFileSetting(m_afPositionOrigin[1], "LibOVR", "afPositionOrigin[1]", szFilePathINI, bFileExists);
	m_afPositionOrigin[2] = GetIniFileSetting(m_afPositionOrigin[2], "LibOVR", "afPositionOrigin[2]", szFilePathINI, bFileExists);
}
