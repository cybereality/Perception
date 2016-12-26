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
* Number of HTC Vive controller buttons.
***/
static const UINT unButtonNo = 13;
/**
* Ids of HTC Vive controller buttons.
***/
static UINT aunButtonIds[] =
{
	vr::k_EButton_System,
	vr::k_EButton_ApplicationMenu,
	vr::k_EButton_Grip,
	vr::k_EButton_DPad_Left,
	vr::k_EButton_DPad_Up,
	vr::k_EButton_DPad_Right,
	vr::k_EButton_DPad_Down,
	vr::k_EButton_A,
	vr::k_EButton_Axis0,
	vr::k_EButton_Axis1,
	vr::k_EButton_Axis2,
	vr::k_EButton_Axis3,
	vr::k_EButton_Axis4
};
/**
* Vireio Enumeration of HTC Vive buttons.
**/
enum ViveControllerButtons
{
	Index_EButton_System = 0,
	Index_EButton_ApplicationMenu = 1,
	Index_EButton_Grip = 2,
	Index_EButton_DPad_Left = 3,
	Index_EButton_DPad_Up = 4,
	Index_EButton_DPad_Right = 5,
	Index_EButton_DPad_Down = 6,
	Index_EButton_A = 7,
	Index_EButton_Axis0 = 8,
	Index_EButton_Axis1 = 9,
	Index_EButton_Axis2 = 10,
	Index_EButton_Axis3 = 11,
	Index_EButton_Axis4 = 12,
	Index_EButton_Axis0_Below_X = 13,
	Index_EButton_Axis0_Above_X,
	Index_EButton_Axis0_Below_Y,
	Index_EButton_Axis0_Above_Y,
	Index_EButton_Axis1_Below_X,
	Index_EButton_Axis1_Above_X,
	Index_EButton_Axis1_Below_Y,
	Index_EButton_Axis1_Above_Y,
	Index_EButton_Axis2_Below_X,
	Index_EButton_Axis2_Above_X,
	Index_EButton_Axis2_Below_Y,
	Index_EButton_Axis2_Above_Y,
	Index_EButton_Axis3_Below_X,
	Index_EButton_Axis3_Above_X,
	Index_EButton_Axis3_Below_Y,
	Index_EButton_Axis3_Above_Y,
	Index_EButton_Axis4_Below_X,
	Index_EButton_Axis4_Above_X,
	Index_EButton_Axis4_Below_Y,
	Index_EButton_Axis4_Above_Y,
	Index_EButton_Axis0_Below_Pressed_X,
	Index_EButton_Axis0_Above_Pressed_X,
	Index_EButton_Axis0_Below_Pressed_Y,
	Index_EButton_Axis0_Above_Pressed_Y,
	Index_EButton_Axis1_Below_Pressed_X,
	Index_EButton_Axis1_Above_Pressed_X,
	Index_EButton_Axis1_Below_Pressed_Y,
	Index_EButton_Axis1_Above_Pressed_Y,
	Index_EButton_Axis2_Below_Pressed_X,
	Index_EButton_Axis2_Above_Pressed_X,
	Index_EButton_Axis2_Below_Pressed_Y,
	Index_EButton_Axis2_Above_Pressed_Y,
	Index_EButton_Axis3_Below_Pressed_X,
	Index_EButton_Axis3_Above_Pressed_X,
	Index_EButton_Axis3_Below_Pressed_Y,
	Index_EButton_Axis3_Above_Pressed_Y,
	Index_EButton_Axis4_Below_Pressed_X,
	Index_EButton_Axis4_Above_Pressed_X,
	Index_EButton_Axis4_Below_Pressed_Y,
	Index_EButton_Axis4_Above_Pressed_Y = 52,
};

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
	ZeroMemory(&m_sTargetSize, sizeof(OpenVR_Size));

	m_cGameTimer.Reset();

	std::string astrVKCodes[] = { "VK_ESCAPE", "VK_CONTROL", "VK_ESCAPE", "X", "X", "X", "X", "VK_TAB", "VK_RBUTTON", "VK_LBUTTON", "VK_F4", "VK_F5", "VK_F6", // << Keys controller 0
		"WM_MOUSEMOVE", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X",                                         // << Keys controller 0 axis 
		"X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X",                                                    // << Keys controller 0 axis pressed
		"VK_ESCAPE", "VK_R", "VK_E", "X", "X", "X", "X", "VK_C", "VK_RETURN", "VK_SPACE", "VK_F1", "VK_F2", "VK_F3",                                           // << Keys controller 1
		"VK_A", "VK_D", "VK_S", "VK_W", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X",                                        // << Keys controller 1 axis
		"X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X" };                                                  // << Keys controller 1 axis pressed

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

	// set default key codes
	m_aaunKeys[0][Index_EButton_System] = GetIniFileSettingKeyCode(astrVKCodes[0], "OpenVR", "aaunKeys[0][Index_EButton_System]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_ApplicationMenu] = GetIniFileSettingKeyCode(astrVKCodes[1], "OpenVR", "aaunKeys[0][Index_EButton_ApplicationMenu]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Grip] = GetIniFileSettingKeyCode(astrVKCodes[2], "OpenVR", "aaunKeys[0][Index_EButton_Grip]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_DPad_Left] = GetIniFileSettingKeyCode(astrVKCodes[3], "OpenVR", "aaunKeys[0][Index_EButton_DPad_Left]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_DPad_Up] = GetIniFileSettingKeyCode(astrVKCodes[4], "OpenVR", "aaunKeys[0][Index_EButton_DPad_Up]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_DPad_Right] = GetIniFileSettingKeyCode(astrVKCodes[5], "OpenVR", "aaunKeys[0][Index_EButton_DPad_Right]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_DPad_Down] = GetIniFileSettingKeyCode(astrVKCodes[6], "OpenVR", "aaunKeys[0][Index_EButton_DPad_Down]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_A] = GetIniFileSettingKeyCode(astrVKCodes[7], "OpenVR", "aaunKeys[0][Index_EButton_A]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0] = GetIniFileSettingKeyCode(astrVKCodes[8], "OpenVR", "aaunKeys[0][Index_EButton_Axis0]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1] = GetIniFileSettingKeyCode(astrVKCodes[9], "OpenVR", "aaunKeys[0][Index_EButton_Axis1]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2] = GetIniFileSettingKeyCode(astrVKCodes[10], "OpenVR", "aaunKeys[0][Index_EButton_Axis2]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3] = GetIniFileSettingKeyCode(astrVKCodes[11], "OpenVR", "aaunKeys[0][Index_EButton_Axis3]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4] = GetIniFileSettingKeyCode(astrVKCodes[12], "OpenVR", "aaunKeys[0][Index_EButton_Axis4]", szFilePathINI, bFileExists);

	m_aaunKeys[0][Index_EButton_Axis0_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[13], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[14], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[15], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[16], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[17], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[18], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[19], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[20], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[21], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[22], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[23], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[24], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[25], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[26], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[27], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[28], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[29], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[30], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[31], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[32], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Above_Y]", szFilePathINI, bFileExists);

	m_aaunKeys[0][Index_EButton_Axis0_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[33], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[34], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[35], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis0_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[36], "OpenVR", "aaunKeys[0][Index_EButton_Axis0_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[37], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[38], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[39], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis1_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[40], "OpenVR", "aaunKeys[0][Index_EButton_Axis1_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[41], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[42], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[43], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis2_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[44], "OpenVR", "aaunKeys[0][Index_EButton_Axis2_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[45], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[46], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[47], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis3_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[48], "OpenVR", "aaunKeys[0][Index_EButton_Axis3_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[49], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[50], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[51], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[0][Index_EButton_Axis4_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[52], "OpenVR", "aaunKeys[0][Index_EButton_Axis4_Above_Pressed_Y]", szFilePathINI, bFileExists);

	m_aaunKeys[1][Index_EButton_System] = GetIniFileSettingKeyCode(astrVKCodes[53], "OpenVR", "aaunKeys[1][Index_EButton_System]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_ApplicationMenu] = GetIniFileSettingKeyCode(astrVKCodes[54], "OpenVR", "aaunKeys[1][Index_EButton_ApplicationMenu]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Grip] = GetIniFileSettingKeyCode(astrVKCodes[55], "OpenVR", "aaunKeys[1][Index_EButton_Grip]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_DPad_Left] = GetIniFileSettingKeyCode(astrVKCodes[56], "OpenVR", "aaunKeys[1][Index_EButton_DPad_Left]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_DPad_Up] = GetIniFileSettingKeyCode(astrVKCodes[57], "OpenVR", "aaunKeys[1][Index_EButton_DPad_Up]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_DPad_Right] = GetIniFileSettingKeyCode(astrVKCodes[58], "OpenVR", "aaunKeys[1][Index_EButton_DPad_Right]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_DPad_Down] = GetIniFileSettingKeyCode(astrVKCodes[59], "OpenVR", "aaunKeys[1][Index_EButton_DPad_Down]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_A] = GetIniFileSettingKeyCode(astrVKCodes[60], "OpenVR", "aaunKeys[1][Index_EButton_A]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0] = GetIniFileSettingKeyCode(astrVKCodes[61], "OpenVR", "aaunKeys[1][Index_EButton_Axis0]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1] = GetIniFileSettingKeyCode(astrVKCodes[62], "OpenVR", "aaunKeys[1][Index_EButton_Axis1]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2] = GetIniFileSettingKeyCode(astrVKCodes[63], "OpenVR", "aaunKeys[1][Index_EButton_Axis2]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3] = GetIniFileSettingKeyCode(astrVKCodes[64], "OpenVR", "aaunKeys[1][Index_EButton_Axis3]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4] = GetIniFileSettingKeyCode(astrVKCodes[65], "OpenVR", "aaunKeys[1][Index_EButton_Axis4]", szFilePathINI, bFileExists);

	m_aaunKeys[1][Index_EButton_Axis0_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[66], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[67], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[68], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[69], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[70], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[71], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[72], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[73], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[74], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[75], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[76], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[77], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[78], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[79], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[80], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[81], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Above_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Below_X] = GetIniFileSettingKeyCode(astrVKCodes[82], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Below_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Above_X] = GetIniFileSettingKeyCode(astrVKCodes[83], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Above_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Below_Y] = GetIniFileSettingKeyCode(astrVKCodes[84], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Below_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Above_Y] = GetIniFileSettingKeyCode(astrVKCodes[85], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Above_Y]", szFilePathINI, bFileExists);

	m_aaunKeys[1][Index_EButton_Axis0_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[86], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[87], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[88], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis0_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[89], "OpenVR", "aaunKeys[1][Index_EButton_Axis0_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[90], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[91], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[92], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis1_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[93], "OpenVR", "aaunKeys[1][Index_EButton_Axis1_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[94], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[95], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[96], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis2_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[97], "OpenVR", "aaunKeys[1][Index_EButton_Axis2_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[98], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[99], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[100], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis3_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[101], "OpenVR", "aaunKeys[1][Index_EButton_Axis3_Above_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Below_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[102], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Below_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Above_Pressed_X] = GetIniFileSettingKeyCode(astrVKCodes[103], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Above_Pressed_X]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Below_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[104], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Below_Pressed_Y]", szFilePathINI, bFileExists);
	m_aaunKeys[1][Index_EButton_Axis4_Above_Pressed_Y] = GetIniFileSettingKeyCode(astrVKCodes[105], "OpenVR", "aaunKeys[1][Index_EButton_Axis4_Above_Pressed_Y]", szFilePathINI, bFileExists);

	m_aafAxisScopeOrFactor[0][0] = GetIniFileSetting(m_aafAxisScopeOrFactor[0][0], "OpenVR", "aafAxisScopeOrFactor[0][0]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[0][1] = GetIniFileSetting(m_aafAxisScopeOrFactor[0][1], "OpenVR", "aafAxisScopeOrFactor[0][1]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[0][2] = GetIniFileSetting(m_aafAxisScopeOrFactor[0][2], "OpenVR", "aafAxisScopeOrFactor[0][2]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[0][3] = GetIniFileSetting(m_aafAxisScopeOrFactor[0][3], "OpenVR", "aafAxisScopeOrFactor[0][3]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[0][4] = GetIniFileSetting(m_aafAxisScopeOrFactor[0][4], "OpenVR", "aafAxisScopeOrFactor[0][4]", szFilePathINI, bFileExists);

	m_aafAxisScopeOrFactor[1][0] = GetIniFileSetting(m_aafAxisScopeOrFactor[1][0], "OpenVR", "aafAxisScopeOrFactor[1][0]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[1][1] = GetIniFileSetting(m_aafAxisScopeOrFactor[1][1], "OpenVR", "aafAxisScopeOrFactor[1][1]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[1][2] = GetIniFileSetting(m_aafAxisScopeOrFactor[1][2], "OpenVR", "aafAxisScopeOrFactor[1][2]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[1][3] = GetIniFileSetting(m_aafAxisScopeOrFactor[1][3], "OpenVR", "aafAxisScopeOrFactor[1][3]", szFilePathINI, bFileExists);
	m_aafAxisScopeOrFactor[1][4] = GetIniFileSetting(m_aafAxisScopeOrFactor[1][4], "OpenVR", "aafAxisScopeOrFactor[1][4]", szFilePathINI, bFileExists);

	// erase key bool field
	ZeroMemory(&m_aabKeys[0][0], sizeof(BOOL)* 2 * 13);

	// extended keys set ?
	for (UINT unI = 0; unI < 2; unI++)
	for (UINT unJ = 0; unJ < 53; unJ++)
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
			TextOut(hdcImage, 650, nY, L"IVRSystem", 9); nY += 64;
			TextOut(hdcImage, 650, nY, L"View", 4); nY += 64;
			TextOut(hdcImage, 650, nY, L"ProjectionLeft", 14); nY += 64;
			TextOut(hdcImage, 650, nY, L"ProjectionRight", 15); nY += 64;
			TextOut(hdcImage, 650, nY, L"TargetWidth", 11); nY += 64;
			TextOut(hdcImage, 650, nY, L"TargetHeight", 12); nY += 64;
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
		case OpenVR_Commanders::ProjectionLeft:
			return L"ProjectionLeft";
		case OpenVR_Commanders::ProjectionRight:
			return L"ProjectionRight";
		case OpenVR_Commanders::TargetWidth:
			return L"TargetWidth";
		case OpenVR_Commanders::TargetHeight:
			return L"TargetHeight";
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
		case OpenVR_Commanders::View:
		case OpenVR_Commanders::ProjectionLeft:
		case OpenVR_Commanders::ProjectionRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
		case OpenVR_Commanders::TargetWidth:
		case OpenVR_Commanders::TargetHeight:
			return NOD_Plugtype::AQU_UINT;
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
		case OpenVR_Commanders::View:
			return (void*)&m_sView;
		case OpenVR_Commanders::ProjectionLeft:
			return (void*)&m_asProjection[0];
		case OpenVR_Commanders::ProjectionRight:
			return (void*)&m_asProjection[1];
		case OpenVR_Commanders::TargetWidth:
			return (void*)&m_sTargetSize.unWidth;
		case OpenVR_Commanders::TargetHeight:
			return (void*)&m_sTargetSize.unHeight;
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
	// update game timer
	m_cGameTimer.Tick();

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

		// create projection matrices
		D3DXMATRIX asProj[2];
		D3DXMATRIX asToEye[2];
		for (UINT unEye = 0; unEye < 2; unEye++)
		{
			D3DXMATRIX asProj[2];
			asProj[unEye] = GetHMDMatrixProjectionEyeLH(m_pHMD, (vr::Hmd_Eye)unEye, 0.1f, 30.0f);
			asToEye[unEye] = GetHMDMatrixPoseEyeLH(m_pHMD, (vr::Hmd_Eye)unEye);
			m_asProjection[unEye] = asToEye[unEye] * asProj[unEye];
		}

		// get recommended size
		m_pHMD->GetRecommendedRenderTargetSize(&m_sTargetSize.unWidth, &m_sTargetSize.unHeight);
	}
	else
	{
		// Process SteamVR events .... TODO !!
		/*vr::VREvent_t event;
		while (m_pHMD->PollNextEvent(&event, sizeof(event)))
		{
		ProcessVREvent(event);
		}*/

		// Process SteamVR controller state
		UINT unControllerIndex = 0;
		for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
		{
			vr::TrackedDeviceClass eClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
			if (eClass == vr::TrackedDeviceClass::TrackedDeviceClass_Controller)
			{
				vr::VRControllerState_t state;
				if (m_pHMD->GetControllerState(unDevice, &state, sizeof(vr::VRControllerState_t)))
				{
					// loop through controller buttons
					for (UINT unButtonIx = 0; unButtonIx < unButtonNo; unButtonIx++)
					{
						// cast keyboard event
						if (vr::ButtonMaskFromId((vr::EVRButtonId)aunButtonIds[unButtonIx]) & state.ulButtonPressed)
						{
							if (!m_aabKeys[unControllerIndex][unButtonIx])
								MapButtonDown(unControllerIndex, unButtonIx);
						}
						else
						if (m_aabKeys[unControllerIndex][unButtonIx])
							MapButtonUp(unControllerIndex, unButtonIx);
					}

					// loop throug axis
					for (UINT unAxisIx = 0; unAxisIx < 5; unAxisIx++)
					{
#pragma region mouse emulation
						// mouse emulation set ? 
						UINT unButtonIx0 = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_X;
						if (m_aaunKeys[unControllerIndex][unButtonIx0] == WM_MOUSEMOVE)
						{
							float fXMove = state.rAxis[unAxisIx].x * m_aafAxisScopeOrFactor[unControllerIndex][unAxisIx];
							float fYMove = -state.rAxis[unAxisIx].y * m_aafAxisScopeOrFactor[unControllerIndex][unAxisIx];

							POINT sPoint;
							GetCursorPos(&sPoint);
							mouse_event(MOUSEEVENTF_MOVE, (DWORD)fXMove, (DWORD)fYMove, 0, 0);
						}
#pragma endregion
#pragma region axis below/above pressed
						// is the axis button pressed ?
						if (vr::ButtonMaskFromId((vr::EVRButtonId)(unAxisIx + (UINT)vr::EVRButtonId::k_EButton_Axis0)) & state.ulButtonPressed)
						{
							// take half of the full scope here
							float fAxisScope = m_aafAxisScopeOrFactor[unControllerIndex][unAxisIx] / 2.0f;

							// x ?
							if (state.rAxis[unAxisIx].x < -fAxisScope)
							{
								// below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Pressed_X;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Pressed_X;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							if (state.rAxis[unAxisIx].x > fAxisScope)
							{
								// above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Pressed_X;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Pressed_X;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							// y ?
							if (state.rAxis[unAxisIx].y < -fAxisScope)
							{
								// below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Pressed_Y;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Pressed_Y;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							if (state.rAxis[unAxisIx].y > fAxisScope)
							{
								// above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Pressed_Y;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Pressed_Y;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}
						}
#pragma endregion
#pragma region axis below/above touched
						// or is it touched ?
						else if (vr::ButtonMaskFromId((vr::EVRButtonId)(unAxisIx + (UINT)vr::EVRButtonId::k_EButton_Axis0)) & state.ulButtonTouched)
						{
							// take half of the full scope here
							float fAxisScope = m_aafAxisScopeOrFactor[unControllerIndex][unAxisIx] / 2.0f;

							// x ?
							if (state.rAxis[unAxisIx].x < -fAxisScope)
							{
								// below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_X;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_X;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							if (state.rAxis[unAxisIx].x > fAxisScope)
							{
								// above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_X;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_X;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							// y ?
							if (state.rAxis[unAxisIx].y < -fAxisScope)
							{
								// below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Y;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no below event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Y;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							if (state.rAxis[unAxisIx].y > fAxisScope)
							{
								// above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Y;
								if (!m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonDown(unControllerIndex, unButtonIx);
							}
							else
							{
								// no above event
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Above_Y;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}
						}
#pragma endregion
#pragma region release buttons
						else
						{
							for (UINT unXY_BA = 0; unXY_BA < 4; unXY_BA++)
							{
								// not touched or pressed, release all buttons if pressed
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_X + unXY_BA;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

							for (UINT unXY_BA = 0; unXY_BA < 4; unXY_BA++)
							{
								// not touched or pressed, release all buttons if pressed
								UINT unButtonIx = unAxisIx * 4 + (UINT)Index_EButton_Axis0_Below_Pressed_X + unXY_BA;
								if (m_aabKeys[unControllerIndex][unButtonIx])
									MapButtonUp(unControllerIndex, unButtonIx);
							}

						}
#pragma endregion
					}
				}

				// set index to next controller (max 2)
				unControllerIndex = 1;
			}
		}

		// call WaitGetPoses() with "1" count, then get all poses with time delta provided for pose prediction
		vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, 1, NULL, 0);
		m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, (float)m_cGameTimer.DeltaTime(), m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount);
		
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

		// get view matrix convert HMD matrix to left handed matrix
		D3DXMATRIX sDeviceToAbsoluteTracking = GetLH(m_rTrackedDevicePose[0].mDeviceToAbsoluteTracking);

		// create view matrix by hmd matrix
		D3DXMatrixInverse(&m_sView, 0, &sDeviceToAbsoluteTracking);

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
