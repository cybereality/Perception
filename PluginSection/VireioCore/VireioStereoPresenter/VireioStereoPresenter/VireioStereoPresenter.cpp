/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.cpp> and
Class <VireioStereoPresenter> :
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

#include"VireioStereoPresenter.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_IDIRECT3DDEVICE9                                           8
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8
#define	METHOD_IDIRECT3DDEVICE9_PRESENT                                      17

/**
* Constructor.
***/
StereoPresenter::StereoPresenter() :AQU_Nodus(),
m_pcBackBufferView(nullptr),
m_pcVertexShader10(nullptr),
m_pcPixelShader10(nullptr),
m_pcVertexLayout10(nullptr),
m_pcVertexBuffer10(nullptr),
m_pcConstantBufferDirect10(nullptr),
m_pcTextureMenu10(nullptr),
m_pcVSGeometry10(nullptr),
m_pcVLGeometry10(nullptr),
m_pcPSGeometry10(nullptr),
m_pcVBGeometry10(nullptr),
m_pcIBGeometry11(nullptr),
m_pcDSGeometry11(nullptr),
m_pcDSVGeometry11(nullptr),
m_pcSampler11(nullptr),
m_bHotkeySwitch(false),
m_eStereoMode(VireioMonitorStereoModes::Vireio_Mono),
m_bZoomOut(FALSE),
m_bMenu(false),
m_bMenuHotkeySwitch(false)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;

	m_pfEuler[0] = nullptr;
	m_pfEuler[1] = nullptr;
	m_pfEuler[2] = nullptr;
	m_pfPosition[0] = nullptr;
	m_pfPosition[1] = nullptr;
	m_pfPosition[2] = nullptr;

	// set defaults
	m_sUserSettings.fFoV = 121.0f;
	m_sUserSettings.fFoVADS = 121.0f;
	m_sUserSettings.fIPD = 0.064f;
	m_sUserSettings.fWorldScale = -1.44f;
	m_sUserSettings.fConvergence = 3.0f;
	m_sUserSettings.bConvergence = 1;

	// read or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// read settings
	m_sUserSettings.fFoV = GetIniFileSetting(m_sUserSettings.fFoV, "Stereo Presenter", "fFoV", szFilePathINI, bFileExists);
	m_sUserSettings.fFoVADS = GetIniFileSetting(m_sUserSettings.fFoVADS, "Stereo Presenter", "fFoVADS", szFilePathINI, bFileExists);
	m_sUserSettings.fIPD = GetIniFileSetting(m_sUserSettings.fIPD, "Stereo Presenter", "fIPD", szFilePathINI, bFileExists);
	m_sUserSettings.fWorldScale = GetIniFileSetting(m_sUserSettings.fWorldScale, "Stereo Presenter", "fWorldScale", szFilePathINI, bFileExists);
	m_sUserSettings.fConvergence = GetIniFileSetting(m_sUserSettings.fConvergence, "Stereo Presenter", "fConvergence", szFilePathINI, bFileExists);
	BOOL bConvergence = GetIniFileSetting((DWORD)m_sUserSettings.bConvergence, "Stereo Presenter", "bConvergence", szFilePathINI, bFileExists);
	if (bConvergence) m_sUserSettings.bConvergence = 1; else m_sUserSettings.bConvergence = 0;

	// meanwhile we set them both to 99 (=uninitialized)
	m_unFoV = 99;
	m_unFoVADS = 99;

	ZeroMemory(&m_apfFloatInput[0], sizeof(float*)* 16);
	ZeroMemory(&m_apnIntInput[0], sizeof(int*)* 16);

}

/**
* Destructor.
***/
StereoPresenter::~StereoPresenter()
{
	SAFE_RELEASE(m_pcDSVGeometry11);
	SAFE_RELEASE(m_pcDSGeometry11);
	SAFE_RELEASE(m_pcSampler11);
	SAFE_RELEASE(m_pcVertexShader10);
	SAFE_RELEASE(m_pcPixelShader10);
	SAFE_RELEASE(m_pcVertexLayout10);
	SAFE_RELEASE(m_pcVertexBuffer10);
	SAFE_RELEASE(m_pcBackBufferView);
	SAFE_RELEASE(m_pcConstantBufferDirect10);
	SAFE_RELEASE(m_pcTextureMenu10);
	SAFE_RELEASE(m_pcVLGeometry10);
	SAFE_RELEASE(m_pcVSGeometry10);
	SAFE_RELEASE(m_pcPSGeometry10);
	SAFE_RELEASE(m_pcVBGeometry10);
	SAFE_RELEASE(m_pcIBGeometry10);
}

/**
* Return the name of the  Stereo Presenter node.
***/
const char* StereoPresenter::GetNodeType()
{
	return "Stereo Presenter";
}

/**
* Returns a global unique identifier for the Stereo Presenter node.
***/
UINT StereoPresenter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 70
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Stereo Presenter node.
***/
LPWSTR StereoPresenter::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Stereo Presenter node.
***/
HBITMAP StereoPresenter::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioStereoPresenter.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Stereo Presenter node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP StereoPresenter::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR StereoPresenter::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((STP_Commanders)dwCommanderIndex)
	{
		case ZoomOut:
			return L"ZoomOut";
	}
	return L"UNTITLED";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR StereoPresenter::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			return L"Left Texture DX11";
		case STP_Decommanders::RightTexture11:
			return L"Right Texture DX11";
		case STP_Decommanders::LeftTexture10:
			return L"Left Texture DX10";
		case STP_Decommanders::RightTexture10:
			return L"Right Texture DX10";
		case STP_Decommanders::LeftTexture9:
			return L"Left Texture DX9";
		case STP_Decommanders::RightTexture9:
			return L"Right Texture DX9";
		case STP_Decommanders::ViewAdjustments:
			return L"ViewAdjustments";
		case STP_Decommanders::Yaw:
			return L"Yaw";
		case STP_Decommanders::Pitch:
			return L"Pitch";
		case STP_Decommanders::Roll:
			return L"Roll";
		case STP_Decommanders::XPosition:
			return L"X-Position";
		case STP_Decommanders::YPosition:
			return L"Y-Position";
		case STP_Decommanders::ZPosition:
			return L"Z-Position";
		case FloatInput00:
			return L"FloatInput00";
		case FloatInput01:
			return L"FloatInput01";
		case FloatInput02:
			return L"FloatInput02";
		case FloatInput03:
			return L"FloatInput03";
		case FloatInput04:
			return L"FloatInput04";
		case FloatInput05:
			return L"FloatInput05";
		case FloatInput06:
			return L"FloatInput06";
		case FloatInput07:
			return L"FloatInput07";
		case FloatInput08:
			return L"FloatInput08";
		case FloatInput09:
			return L"FloatInput09";
		case FloatInput10:
			return L"FloatInput10";
		case FloatInput11:
			return L"FloatInput11";
		case FloatInput12:
			return L"FloatInput12";
		case FloatInput13:
			return L"FloatInput13";
		case FloatInput14:
			return L"FloatInput14";
		case FloatInput15:
			return L"FloatInput15";
		case IntInput00:
			return L"IntInput00";
		case IntInput01:
			return L"IntInput01";
		case IntInput02:
			return L"IntInput02";
		case IntInput03:
			return L"IntInput03";
		case IntInput04:
			return L"IntInput04";
		case IntInput05:
			return L"IntInput05";
		case IntInput06:
			return L"IntInput06";
		case IntInput07:
			return L"IntInput07";
		case IntInput08:
			return L"IntInput08";
		case IntInput09:
			return L"IntInput09";
		case IntInput10:
			return L"IntInput10";
		case IntInput11:
			return L"IntInput11";
		case IntInput12:
			return L"IntInput12";
		case IntInput13:
			return L"IntInput13";
		case IntInput14:
			return L"IntInput14";
		case IntInput15:
			return L"IntInput15";
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD StereoPresenter::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((STP_Commanders)dwCommanderIndex)
	{
		case ZoomOut:
			return NOD_Plugtype::AQU_BOOL;
	}
	return 0;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoPresenter::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
		case STP_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case STP_Decommanders::LeftTexture10:
		case STP_Decommanders::RightTexture10:
			return NOD_Plugtype::AQU_PNT_ID3D10SHADERRESOURCEVIEW;
		case STP_Decommanders::LeftTexture9:
		case STP_Decommanders::RightTexture9:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DTEXTURE9;
		case STP_Decommanders::ViewAdjustments:
			return NOD_Plugtype::AQU_SHAREDPOINTER;
		case STP_Decommanders::Yaw:
		case STP_Decommanders::Pitch:
		case STP_Decommanders::Roll:
		case STP_Decommanders::XPosition:
		case STP_Decommanders::YPosition:
		case STP_Decommanders::ZPosition:
			return NOD_Plugtype::AQU_FLOAT;
		case FloatInput00:
		case FloatInput01:
		case FloatInput02:
		case FloatInput03:
		case FloatInput04:
		case FloatInput05:
		case FloatInput06:
		case FloatInput07:
		case FloatInput08:
		case FloatInput09:
		case FloatInput10:
		case FloatInput11:
		case FloatInput12:
		case FloatInput13:
		case FloatInput14:
		case FloatInput15:
			return NOD_Plugtype::AQU_FLOAT;
		case IntInput00:
		case IntInput01:
		case IntInput02:
		case IntInput03:
		case IntInput04:
		case IntInput05:
		case IntInput06:
		case IntInput07:
		case IntInput08:
		case IntInput09:
		case IntInput10:
		case IntInput11:
		case IntInput12:
		case IntInput13:
		case IntInput14:
		case IntInput15:
			return NOD_Plugtype::AQU_INT;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* StereoPresenter::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((STP_Commanders)dwCommanderIndex)
	{
		case ZoomOut:
			return (void*)&m_bZoomOut;
	}
	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoPresenter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case STP_Decommanders::RightTexture11:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case STP_Decommanders::LeftTexture10:
		case STP_Decommanders::RightTexture10:
		case STP_Decommanders::LeftTexture9:
		case STP_Decommanders::RightTexture9:
			break;
		case STP_Decommanders::ViewAdjustments:
			m_ppcShaderViewAdjustment = (std::shared_ptr<ViewAdjustment>*)pData;
			break;
		case STP_Decommanders::Yaw:
			m_pfEuler[0] = (float*)pData;
			break;
		case STP_Decommanders::Pitch:
			m_pfEuler[1] = (float*)pData;
			break;
		case STP_Decommanders::Roll:
			m_pfEuler[2] = (float*)pData;
			break;
		case STP_Decommanders::XPosition:
			m_pfPosition[0] = (float*)pData;
			break;
		case STP_Decommanders::YPosition:
			m_pfPosition[1] = (float*)pData;
			break;
		case STP_Decommanders::ZPosition:
			m_pfPosition[2] = (float*)pData;
			break;
		case FloatInput00:
		case FloatInput01:
		case FloatInput02:
		case FloatInput03:
		case FloatInput04:
		case FloatInput05:
		case FloatInput06:
		case FloatInput07:
		case FloatInput08:
		case FloatInput09:
		case FloatInput10:
		case FloatInput11:
		case FloatInput12:
		case FloatInput13:
		case FloatInput14:
		case FloatInput15:
			m_apfFloatInput[dwDecommanderIndex - 13] = (float*)pData;
			break;
		case IntInput00:
		case IntInput01:
		case IntInput02:
		case IntInput03:
		case IntInput04:
		case IntInput05:
		case IntInput06:
		case IntInput07:
		case IntInput08:
		case IntInput09:
		case IntInput10:
		case IntInput11:
		case IntInput12:
		case IntInput13:
		case IntInput14:
		case IntInput15:
			m_apnIntInput[dwDecommanderIndex - 29] = (int*)pData;
			break;
	}
}

/**
* Stereo Presenter supports any call since it is on the end of the line.
* (verifies for supported methods in provoke call)
***/
bool StereoPresenter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	//if ((nD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (nD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
	return true;

	//return false;
}

/**
* Handle Stereo Drawing.
***/
void* StereoPresenter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
#ifdef _DEBUG_STP
	{ wchar_t buf[128]; wsprintf(buf, L"ifc %u mtd %u", eD3DInterface, eD3DMethod); OutputDebugString(buf); }
#endif

	// only present accepted
	bool bValid = false;
	if (((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)) ||
		((eD3DInterface == INTERFACE_IDIRECT3DDEVICE9) && (eD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT))) bValid = true;
	if (!bValid) return nullptr;

	static UINT unWidthRT = 0, unHeightRT = 0;
	static const UINT unFoVSettings = 3;
	static float afFoV[] = { 90.0f, 116.0f, 121.0f };
	static const UINT unFoVADSSettings = 10;
	static float afFoVADS[] = { 141.0f, 146.0f, 101.0f, 106.0f, 111.0f, 116.0f, 121.0f, 126.0f, 131.0f, 136.0f };
	if (m_unFoV >= unFoVSettings)
	{
		m_unFoV = 0;
		for (UINT un = 0; un < unFoVSettings; un++)
		{
			if (m_sUserSettings.fFoV == afFoV[un]) m_unFoV = un;
		}
	}
	if (m_unFoVADS >= unFoVADSSettings)
	{
		m_unFoVADS = 0;
		for (UINT un = 0; un < unFoVADSSettings; un++)
		{
			if (m_sUserSettings.fFoVADS == afFoVADS[un]) m_unFoVADS = un;
		}
	}

	// immersive mode ?
	bool bImmersiveMode = true;
	if (m_apnIntInput[1])
	{
		if (*m_apnIntInput[1] == 0)
			bImmersiveMode = false;
	}
	else bImmersiveMode = false;

	// update view adjustment class for user settings and tracking
	static bool s_bConfigLoaded = false;
	if (m_ppcShaderViewAdjustment)
	{
		if (*m_ppcShaderViewAdjustment)
		{
			// did we initialize the game configurations for immersive and cinema mode ?
			if (!s_bConfigLoaded)
			{
				// note that we only override IPD here !!
				(*m_ppcShaderViewAdjustment).get()->Save(m_sGameConfigurationDefault);
				m_sGameConfigurationDefault.fIPD = m_sUserSettings.fIPD;

				// copy default to user configuration and set all fields
				memcpy(&m_sGameConfigurationUser, &m_sGameConfigurationDefault, sizeof(Vireio_GameConfiguration));
				m_sGameConfigurationUser.fPFOV = m_sUserSettings.fFoV;
				m_sGameConfigurationUser.fWorldScaleFactor = m_sUserSettings.fWorldScale;
				m_sGameConfigurationUser.fConvergence = m_sUserSettings.fConvergence;
				m_sGameConfigurationUser.bConvergenceEnabled = m_sUserSettings.bConvergence;

				// set default for immersive mode, user for cinema mode
				if (bImmersiveMode)
					(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationDefault);
				else
					(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationUser);

				s_bConfigLoaded = true;
			}

			// update roll, update view transforms
			if (m_pfEuler[2])
			{
				// update roll only for immersive mode
				if (bImmersiveMode)
					(*m_ppcShaderViewAdjustment).get()->UpdateRoll(-(*m_pfEuler[2]));
				else
					(*m_ppcShaderViewAdjustment).get()->UpdateRoll(0.0f);
			}

			// update position
			if (bImmersiveMode &&
				/*m_pfEuler[0] &&
					m_pfEuler[1] &&*/
					m_pfEuler[2] &&
					/*m_pfPosition[0] &&*/
					m_pfPosition[1]
					/*m_pfPosition[2]*/)
			{
				// optimized for 1.75 meters eye height
				float fY = *m_pfPosition[1] - 1.75f;
				(*m_ppcShaderViewAdjustment).get()->UpdatePosition(
					0.0f,//*m_pfEuler[0],
					0.0f,//*m_pfEuler[1],
					*m_pfEuler[2],
					0.0f,//*m_pfPosition[0], <- Currently only y positional tracking !!
					-fY,
					0.0f);//*m_pfPosition[2]);
			}
			else
				(*m_ppcShaderViewAdjustment).get()->UpdatePosition(0.0f, 0.0f, 0.0f);

			(*m_ppcShaderViewAdjustment).get()->ComputeViewTransforms();
		}
	}

	// get xbox controller input
	XINPUT_STATE sControllerState;
	bool bControllerAttached = false;
	ZeroMemory(&sControllerState, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &sControllerState) == ERROR_SUCCESS)
	{
		bControllerAttached = true;
	}

	if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
	{
#pragma region menu hotkeys/ controller
		static bool bReleased = true;
		// handle switch hotkeys
		if (GetAsyncKeyState(VK_F1))
		{
			if (bReleased)
			{
				// fov
				m_unFoV++;
				if (m_unFoV >= unFoVSettings)
					m_unFoV = 0;
				m_sUserSettings.fFoV = afFoV[m_unFoV];
				bReleased = false;

				// read or create the INI file
				char szFilePathINI[1024];
				GetCurrentDirectoryA(1024, szFilePathINI);
				strcat_s(szFilePathINI, "\\VireioPerception.ini");

				// fov
				std::stringstream sz;
				sz << m_sUserSettings.fFoV;
				WritePrivateProfileStringA("Stereo Presenter", "fFoV", sz.str().c_str(), szFilePathINI);
			}
		}
		else
		if (GetAsyncKeyState(VK_F2))
		{
			if (bReleased)
			{
				// fov ads
				m_unFoVADS++;
				if (m_unFoVADS >= unFoVADSSettings)
					m_unFoVADS = 0;
				m_sUserSettings.fFoVADS = afFoVADS[m_unFoVADS];
				bReleased = false;

				// read or create the INI file
				char szFilePathINI[1024];
				GetCurrentDirectoryA(1024, szFilePathINI);
				strcat_s(szFilePathINI, "\\VireioPerception.ini");

				// fov aiming down sights
				std::stringstream sz;
				sz << m_sUserSettings.fFoVADS;
				WritePrivateProfileStringA("Stereo Presenter", "fFoVADS", sz.str().c_str(), szFilePathINI);
			}
		}
		else
		if (GetAsyncKeyState(VK_F12))
		{
			m_bHotkeySwitch = true;
		}
		else
		if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(0x51))
		{
			m_bMenuHotkeySwitch = true;
		}
		else
		if (m_bMenuHotkeySwitch)
		{
			m_bMenu = !m_bMenu;
			m_bMenuHotkeySwitch = false;
		}
		else
		if (m_bHotkeySwitch)
		{
			if (m_eStereoMode) m_eStereoMode = VireioMonitorStereoModes::Vireio_Mono; else m_eStereoMode = VireioMonitorStereoModes::Vireio_SideBySide;
			m_bHotkeySwitch = false;
		}
		else
			bReleased = true;

		// handle controller
		if (bControllerAttached)
		{
			if (m_apnIntInput[0])
			{
				if (*m_apnIntInput[0] > 0) (*m_apnIntInput[0])++;

				static bool bWasPressed = false;
				if (GetAsyncKeyState(VK_BACK))
				{
					if (!bWasPressed)
					{
						if ((*m_apnIntInput[0] == 0))
						{
							(*m_apnIntInput[0]) = TRUE;
							m_bZoomOut = TRUE;
						}
						else
						{
							*m_apnIntInput[0] = FALSE;
							m_bZoomOut = FALSE;
						}

						bWasPressed = true;
					}
				}
				else
				{
					bWasPressed = false;
				}
			}

			static bool bWasPressed = false;
			if (sControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				if ((m_apnIntInput[1]) && (!bWasPressed))
				{
					if (*m_apnIntInput[1] == 0)
					{
						*m_apnIntInput[1] = TRUE;

						// set default for immersive mode
						if (m_ppcShaderViewAdjustment)
							(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationDefault);
					}
					else
					{
						*m_apnIntInput[1] = FALSE;

						// set user for cinema mode
						if (m_ppcShaderViewAdjustment)
							(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationUser);
					}

					bWasPressed = true;
				}
			}
			else
			{
				bWasPressed = false;
			}

			if (m_apfFloatInput[0])
			{
				*m_apfFloatInput[0] = m_sUserSettings.fFoV;
			}

			if (m_apfFloatInput[1])
			{
				if (sControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
				{
					*m_apfFloatInput[1] = afFoV[0];
				}
				else
				{
					*m_apfFloatInput[1] = m_sUserSettings.fFoVADS;
				}
			}
		}
		else
		{
			// handle keyboard
			if (m_apnIntInput[0])
			{
				if (*m_apnIntInput[0] > 0) (*m_apnIntInput[0])++;

				static bool bWasPressed = false;
				if (GetAsyncKeyState(VK_BACK))
				{
					if (!bWasPressed)
					{
						if ((*m_apnIntInput[0] == 0))
						{
							(*m_apnIntInput[0]) = TRUE;
							m_bZoomOut = TRUE;
						}
						else
						{
							*m_apnIntInput[0] = FALSE;
							m_bZoomOut = FALSE;
						}

						bWasPressed = true;
					}
				}
				else
				{
					bWasPressed = false;
				}
			}

			if (m_apnIntInput[1])
			{
				static bool bWasPressed = false;
				if (GetAsyncKeyState(VK_BACK))
				{
					if (!bWasPressed)
					{
						if (*m_apnIntInput[1] == 0)
						{
							*m_apnIntInput[1] = TRUE;

							// set default for immersive mode
							if (m_ppcShaderViewAdjustment)
								(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationDefault);
						}
						else
						{
							*m_apnIntInput[1] = FALSE;

							// set user for cinema mode
							if (m_ppcShaderViewAdjustment)
								(*m_ppcShaderViewAdjustment).get()->Load(m_sGameConfigurationUser);
						}

						bWasPressed = true;
					}
				}
				else
				{
					bWasPressed = false;
				}
			}

			if (m_apfFloatInput[0])
			{
				*m_apfFloatInput[0] = m_sUserSettings.fFoV;
			}

			if (m_apfFloatInput[1])
			{
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					*m_apfFloatInput[1] = afFoV[0];
				}
				else
				{
					*m_apfFloatInput[1] = m_sUserSettings.fFoVADS;
				}
			}
		}
#pragma endregion
#pragma region menu
		if (m_bMenu)
		{
			if ((m_ppcTexView11[0]) && (m_ppcTexView11[1]))
			{
				// get device and context
				ID3D11Device* pcDevice = nullptr;
				ID3D11DeviceContext* pcContext = nullptr;
				if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
				{
					// release frame texture+view
					if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
					if (pcContext) { pcContext->Release(); pcContext = nullptr; }
					return nullptr;
				}

				// create the menu texture
				if (!m_pcTextureMenu11)
				{
					// load the bitmap resource
					HMODULE hModule = GetModuleHandle(L"VireioStereoPresenter.dll");
					HBITMAP hbTiles = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_TILES01));
					if (hbTiles)
					{
						// get size of bitmap
						BITMAP bm;
						GetObject(hbTiles, sizeof(bm), &bm);

						// create byte buffer
						char *pchBytes = (char*)malloc(bm.bmWidth*bm.bmHeight * 4);

						// get source hdc
						HDC hdcImage = CreateCompatibleDC(NULL);
						HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, hbTiles);
						int x = 0; int y = 0;

						BITMAPINFO MyBMInfo = { 0 };
						MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

						// Get the BITMAPINFO structure from the bitmap
						if (0 == GetDIBits(hdcImage, hbTiles, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS))
						{
							OutputDebugString(L"VSTP : Error GetDIBits");
						}

						MyBMInfo.bmiHeader.biBitCount = 32;
						MyBMInfo.bmiHeader.biCompression = BI_RGB;
						MyBMInfo.bmiHeader.biHeight = abs(MyBMInfo.bmiHeader.biHeight);

						// Call GetDIBits a second time, this time to store the actual bitmap data
						if (0 == GetDIBits(hdcImage, hbTiles, 0, MyBMInfo.bmiHeader.biHeight,
							pchBytes, &MyBMInfo, DIB_RGB_COLORS))
						{
							OutputDebugString(L"VSTP : Error GetDIBits");
						}

						// release bitmap
						DeleteDC(hdcImage);
						DeleteObject(hbTiles);

						// create font texture
						D3D11_TEXTURE2D_DESC sDesc;
						ZeroMemory(&sDesc, sizeof(sDesc));
						sDesc.Width = bm.bmWidth;
						sDesc.Height = bm.bmHeight;
						sDesc.MipLevels = sDesc.ArraySize = 1;
						sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						sDesc.SampleDesc.Count = 1;
						sDesc.Usage = D3D11_USAGE_DEFAULT;
						sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
						D3D11_SUBRESOURCE_DATA sData;
						ZeroMemory(&sData, sizeof(sData));
						sData.pSysMem = pchBytes;
						sData.SysMemPitch = bm.bmWidth * 4;
						if (FAILED(pcDevice->CreateTexture2D(&sDesc, &sData, &m_pcTextureMenu11)))
							OutputDebugString(L"Failed to create Texture DEFAULT.");

						free((void*)pchBytes);

						if (m_pcTextureMenu11)
						{
							OutputDebugString(L"VSTP : Created font texture !");
						}
						else
						{
							OutputDebugString(L"VSTP : Can't create font texture.");
						}

						DeleteObject(hbTiles);
					}
				}

				// get the viewport
				UINT dwNumViewports = 1;
				D3D11_VIEWPORT psViewport[16];
				pcContext->RSGetViewports(&dwNumViewports, psViewport);

				// draw to the textures
				ID3D11Texture2D* pcTexture[2];
				for (UINT unI = 0; unI < 2; unI++)
				{
					(*m_ppcTexView11[unI])->GetResource((ID3D11Resource**)&pcTexture[unI]);

					if (pcTexture[unI])
					{
						// draw logo
						D3D11_BOX sBox;
						sBox.left = 128;
						sBox.right = 384;
						sBox.top = 0;
						sBox.bottom = 32;
						sBox.front = 0;
						sBox.back = 1;
						pcContext->CopySubresourceRegion(pcTexture[unI], 0, (UINT(psViewport[0].Width) >> 1) - 128, (UINT(psViewport[0].Height) >> 1) - 48, 0, m_pcTextureMenu11, 0, &sBox);

						// draw tiles
						sBox.left = 0;
						sBox.right = 128;
						sBox.top = m_unFoV * 32;
						sBox.bottom = m_unFoV * 32 + 32;
						pcContext->CopySubresourceRegion(pcTexture[unI], 0, (UINT(psViewport[0].Width) >> 1) - 64, (UINT(psViewport[0].Height) >> 1) - 16, 0, m_pcTextureMenu11, 0, &sBox);
						sBox.top = (m_unFoVADS + 3) * 32;
						sBox.bottom = (m_unFoVADS + 3) * 32 + 32;
						pcContext->CopySubresourceRegion(pcTexture[unI], 0, (UINT(psViewport[0].Width) >> 1) - 64, (UINT(psViewport[0].Height) >> 1) + 16, 0, m_pcTextureMenu11, 0, &sBox);

						pcTexture[unI]->Release();
					}
				}

				if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
				if (pcContext) { pcContext->Release(); pcContext = nullptr; }
			}
		}
#pragma endregion
#pragma region draw to stereo textures

		// draw to stereo targets
		if (true/*TODO*/)
		{
			// get device and context
			ID3D11Device* pcDevice = nullptr;
			ID3D11DeviceContext* pcContext = nullptr;
			if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
			{
				// release frame texture+view
				if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
				if (pcContext) { pcContext->Release(); pcContext = nullptr; }
				return nullptr;
			}

			// create vertex shader
			if (!m_pcVSGeometry11)
			{
				if (FAILED(CreateVertexShaderTechnique(pcDevice, &m_pcVSGeometry11, &m_pcVLGeometry11, VertexShaderTechnique::PosNormUV)))
					OutputDebugString(L"[STP] Failed to create vertex shader. !");
			}

			// create pixel shader
			if (!m_pcPSGeometry11)
			{
				if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPSGeometry11, PixelShaderTechnique::GeometryDiffuseTextured)))
					OutputDebugString(L"[STP] Failed to create pixel shader. !");
			}

			// create the depth stencil
			if (!m_pcDSGeometry11)
			{
				ID3D11Texture2D* pcBackBuffer = nullptr;
				((IDXGISwapChain*)pThis)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);

				if (pcBackBuffer)
				{
					D3D11_TEXTURE2D_DESC sDesc;
					pcBackBuffer->GetDesc(&sDesc);
					pcBackBuffer->Release();

					// set static height, width
					unWidthRT = sDesc.Width;
					unHeightRT = sDesc.Height;

					// Create depth stencil texture
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
					if (FAILED(pcDevice->CreateTexture2D(&descDepth, NULL, &m_pcDSGeometry11)))
						OutputDebugString(L"[STP] Failed to create depth stencil.");

					// Create the depth stencil view
					D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
					ZeroMemory(&descDSV, sizeof(descDSV));
					descDSV.Format = descDepth.Format;
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					descDSV.Texture2D.MipSlice = 0;
					if (FAILED(pcDevice->CreateDepthStencilView(m_pcDSGeometry11, &descDSV, &m_pcDSVGeometry11)))
						OutputDebugString(L"[STP] Failed to create depth stencil view.");
				}
			}

			// Create vertex buffer
			if (!m_pcVBGeometry11)
			{
				TexturedNormalVertex asVertices[] =
				{
					{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

					{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

					{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

					{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

					{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },

					{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },
					{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
				};

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(TexturedNormalVertex)* 24;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = asVertices;
				if (FAILED(pcDevice->CreateBuffer(&bd, &InitData, &m_pcVBGeometry11)))
					OutputDebugString(L"[STS] Failed to create vertex buffer.");
			}

			// Create index buffer
			if (!m_pcIBGeometry11)
			{
				WORD aunIndices[] =
				{
					3, 1, 0,
					2, 1, 3,

					6, 4, 5,
					7, 4, 6,

					11, 9, 8,
					10, 9, 11,

					14, 12, 13,
					15, 12, 14,

					19, 17, 16,
					18, 17, 19,

					22, 20, 21,
					23, 20, 22
				};

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(WORD)* 36;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = aunIndices;
				if (FAILED(pcDevice->CreateBuffer(&bd, &InitData, &m_pcIBGeometry11)))
					OutputDebugString(L"[STS] Failed to create index buffer.");
			}

			if (!m_pcSampler11)
			{
				// Create the sample state
				D3D11_SAMPLER_DESC sampDesc;
				ZeroMemory(&sampDesc, sizeof(sampDesc));
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				sampDesc.MinLOD = 0;
				sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
				if (FAILED(pcDevice->CreateSamplerState(&sampDesc, &m_pcSampler11)))
					OutputDebugString(L"[STS] Failed to create sampler.");
			}

			if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
			if (pcContext) { pcContext->Release(); pcContext = nullptr; }
		}
#pragma endregion
#pragma region draw (optionally)

		// draw stereo target to screen (optionally)
		if (m_eStereoMode)
		{
			// DX 11
			if ((m_ppcTexView11[0]) && (m_ppcTexView11[1]))
			{
				// get device and context
				ID3D11Device* pcDevice = nullptr;
				ID3D11DeviceContext* pcContext = nullptr;
				if (FAILED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
				{
					// release frame texture+view
					if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
					if (pcContext) { pcContext->Release(); pcContext = nullptr; }
					return nullptr;
				}

				// get the viewport
				UINT dwNumViewports = 1;
				D3D11_VIEWPORT psViewport[16];
				pcContext->RSGetViewports(&dwNumViewports, psViewport);

				// backup all states
				D3DX11_STATE_BLOCK sStateBlock;
				CreateStateblock(pcContext, &sStateBlock);

				// clear all states, set targets
				ClearContextState(pcContext);

				// set first active render target - the stored back buffer - get the stored private data view
				ID3D11Texture2D* pcBackBuffer = nullptr;
				((IDXGISwapChain*)pThis)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
				ID3D11RenderTargetView* pcView = nullptr;
				UINT dwSize = sizeof(pcView);
				pcBackBuffer->GetPrivateData(PDIID_ID3D11TextureXD_RenderTargetView, &dwSize, (void*)&pcView);
				if (dwSize)
				{
					pcContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&pcView, m_pcDSVGeometry11);
					pcView->Release();
				}
				else
				{
					// create render target view for the back buffer
					ID3D11RenderTargetView* pcRTV = nullptr;
					pcDevice->CreateRenderTargetView(pcBackBuffer, NULL, &pcRTV);
					if (pcRTV)
					{
						pcBackBuffer->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcRTV);
						pcRTV->Release();
					}
				}
				pcContext->RSSetViewports(dwNumViewports, psViewport);
				pcBackBuffer->Release();

				// clear the depth stencil
				pcContext->ClearDepthStencilView(m_pcDSVGeometry11, D3D11_CLEAR_DEPTH, 1.0f, 0);

				// create all bool
				bool bAllCreated = true;

				// create vertex shader
				if (!m_pcVertexShader11)
				{
					if (FAILED(CreateVertexShaderTechnique(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11, VertexShaderTechnique::PosUV2D)))
						bAllCreated = false;
				}
				// create pixel shader... TODO !! add option to switch output
				if (!m_pcPixelShader11)
				{
					if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
						bAllCreated = false;
				}
				// Create vertex buffer
				if (!m_pcVertexBuffer11)
				{
					if (FAILED(CreateFullScreenVertexBuffer(pcDevice, &m_pcVertexBuffer11)))
						bAllCreated = false;
				}
				// create constant buffer
				if (!m_pcConstantBufferDirect11)
				{
					if (FAILED(CreateGeometryConstantBuffer(pcDevice, &m_pcConstantBufferDirect11, (UINT)sizeof(GeometryConstantBuffer))))
						bAllCreated = false;
				}

				float fTestFont = false;

				if (bAllCreated)
				{
					// left/right eye
					for (int nEye = 0; nEye < 2; nEye++)
					if (!fTestFont)
					{
						// Set the input layout
						pcContext->IASetInputLayout(m_pcVertexLayout11);

						// Set vertex buffer
						UINT stride = sizeof(TexturedVertex);
						UINT offset = 0;
						pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

						// Set constant buffer, first update it... scale and translate the left and right image
						D3DXMATRIX sScale;
						D3DXMatrixScaling(&sScale, 0.5f, 1.0f, 1.0f);
						D3DXMATRIX sTrans;
						if (nEye == 0)
							D3DXMatrixTranslation(&sTrans, -0.5f, 0.0f, 0.0f);
						else
							D3DXMatrixTranslation(&sTrans, 0.5f, 0.0f, 0.0f);
						D3DXMatrixTranspose(&sTrans, &sTrans);
						D3DXMATRIX sProj;
						D3DXMatrixMultiply(&sProj, &sTrans, &sScale);
						pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
						pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

						// Set primitive topology
						pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						// set texture
						pcContext->PSSetShaderResources(0, 1, m_ppcTexView11[nEye]);

						// set shaders
						pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
						pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

						// Render a triangle
						pcContext->Draw(6, 0);
					}

					if (fTestFont)
					{
						static VireioFont* m_pcFontSegeo128 = nullptr;
						HRESULT nHr = S_OK;
						if (!m_pcFontSegeo128)
							m_pcFontSegeo128 = new VireioFont(pcDevice, pcContext, "SegoeUI128.spritefont", 128.0f, 1920.0f / 1080.0f, nHr);
						if (FAILED(nHr)) { delete m_pcFontSegeo128; m_pcFontSegeo128 = nullptr; }

						// test draw... TODO !!

						// Update our time
						static float t = 0.0f;
						static DWORD dwTimeStart = 0;
						DWORD dwTimeCur = GetTickCount();
						if (dwTimeStart == 0)
							dwTimeStart = dwTimeCur;
						t = (dwTimeCur - dwTimeStart) / 1000.0f;

						if (m_pcFontSegeo128)
						{
							m_pcFontSegeo128->SetTextAttributes(0.0f, 5.0f, 0.0001f, 0.0f);
							m_pcFontSegeo128->ToRender(pcContext);
							m_pcFontSegeo128->RenderText(pcDevice, pcContext, "Vireio Perception: Open-Source Stereoscopic 3D Driver", 0.0f, 0.0f, 0.0f);
						}
						else OutputDebugString(L"Failed to create font!");
					}
				}

				// set back device
				ApplyStateblock(pcContext, &sStateBlock);

				if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
				if (pcContext) { pcContext->Release(); pcContext = nullptr; }
			}
		}
#pragma endregion
	}

	return nullptr;
}