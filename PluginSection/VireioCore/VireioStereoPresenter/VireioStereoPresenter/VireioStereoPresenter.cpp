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

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_IDIRECT3DDEVICE9                                           8
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

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
m_bHotkeySwitch(false),
m_eStereoMode(VireioMonitorStereoModes::Vireio_Mono),
m_bZoomOut(FALSE),
m_bMenu(false)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;

	m_pfEuler[0] = nullptr;
	m_pfEuler[1] = nullptr;
	m_pfEuler[2] = nullptr;
	m_pfPosition[0] = nullptr;
	m_pfPosition[1] = nullptr;
	m_pfPosition[2] = nullptr;

	// read or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	if (PathFileExistsA(szFilePathINI))
	{
		char szBuffer[128];

		// fov
		m_sUserSettings.fFoV = 121.0f;
		std::stringstream sz;
		sz << m_sUserSettings.fFoV;
		GetPrivateProfileStringA("Stereo Presenter", "fFoV", sz.str().c_str(), szBuffer, 128, szFilePathINI);
		sz = std::stringstream(szBuffer);
		sz >> m_sUserSettings.fFoV;

		// fov aiming down sights
		m_sUserSettings.fFoVADS = 121.0f;
		sz = std::stringstream();
		sz << m_sUserSettings.fFoVADS;
		GetPrivateProfileStringA("Stereo Presenter", "fFoVADS", sz.str().c_str(), szBuffer, 128, szFilePathINI);
		sz = std::stringstream(szBuffer);
		sz >> m_sUserSettings.fFoVADS;

		// ipd
		m_sUserSettings.fIPD = 0.064f;
		sz = std::stringstream();
		sz << m_sUserSettings.fIPD;
		GetPrivateProfileStringA("Stereo Presenter", "fIPD", sz.str().c_str(), szBuffer, 128, szFilePathINI);
		sz = std::stringstream(szBuffer);
		sz >> m_sUserSettings.fIPD;

		// separation
		m_sUserSettings.fWorldScale = -1.44f;
		sz = std::stringstream();
		sz << m_sUserSettings.fWorldScale;
		GetPrivateProfileStringA("Stereo Presenter", "fWorldScale", sz.str().c_str(), szBuffer, 128, szFilePathINI);
		sz = std::stringstream(szBuffer);
		sz >> m_sUserSettings.fWorldScale;

		// convergence
		m_sUserSettings.fConvergence = 3.0f;
		sz = std::stringstream();
		sz << m_sUserSettings.fConvergence;
		GetPrivateProfileStringA("Stereo Presenter", "fConvergence", sz.str().c_str(), szBuffer, 128, szFilePathINI);
		sz = std::stringstream(szBuffer);
		sz >> m_sUserSettings.fConvergence;
	}
	else
	{
		// fov
		m_sUserSettings.fFoV = 121.0f;
		std::stringstream sz;
		sz << m_sUserSettings.fFoV;
		WritePrivateProfileStringA("Stereo Presenter", "fFoV", sz.str().c_str(), szFilePathINI);

		// fov aiming down sights
		m_sUserSettings.fFoVADS = 136.0f;
		sz = std::stringstream();
		sz << m_sUserSettings.fFoVADS;
		WritePrivateProfileStringA("Stereo Presenter", "fFoVADS", sz.str().c_str(), szFilePathINI);

		// ipd
		m_sUserSettings.fIPD = 0.064f;
		sz = std::stringstream();
		sz << m_sUserSettings.fIPD;
		WritePrivateProfileStringA("Stereo Presenter", "fIPD", sz.str().c_str(), szFilePathINI);

		// separation
		m_sUserSettings.fWorldScale = -1.44f;
		sz = std::stringstream();
		sz << m_sUserSettings.fWorldScale;
		WritePrivateProfileStringA("Stereo Presenter", "fWorldScale", sz.str().c_str(), szFilePathINI);

		// convergence
		m_sUserSettings.fConvergence = 3.0f;
		sz = std::stringstream();
		sz << m_sUserSettings.fConvergence;
		WritePrivateProfileStringA("Stereo Presenter", "fConvergence", sz.str().c_str(), szFilePathINI);
	}

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
	SAFE_RELEASE(m_pcVertexShader10);
	SAFE_RELEASE(m_pcPixelShader10);
	SAFE_RELEASE(m_pcVertexLayout10);
	SAFE_RELEASE(m_pcVertexBuffer10);
	SAFE_RELEASE(m_pcBackBufferView);
	SAFE_RELEASE(m_pcConstantBufferDirect10);
	SAFE_RELEASE(m_pcTextureMenu10);
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
			return NOD_Plugtype::AQU_PNT_FLOAT;
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
	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;

	// update view adjustment class for user settings and tracking
	if (m_ppcShaderViewAdjustment)
	{
		if (*m_ppcShaderViewAdjustment)
		{
			// update roll, update view transforms
			if (m_pfEuler[2])
				(*m_ppcShaderViewAdjustment).get()->UpdateRoll(-(*m_pfEuler[2]));
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
			if (sControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				if (m_apnIntInput[0])
				{
					*m_apnIntInput[0] = TRUE;
				}
				m_bZoomOut = TRUE;
			}
			else
			{
				if (m_apnIntInput[0])
				{
					*m_apnIntInput[0] = FALSE;
				}
				m_bZoomOut = FALSE;
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
			if (GetAsyncKeyState(VK_MBUTTON))
			{
				if (m_apnIntInput[0])
				{
					*m_apnIntInput[0] = TRUE;
				}
				m_bZoomOut = TRUE;
			}
			else
			{
				if (m_apnIntInput[0])
				{
					*m_apnIntInput[0] = FALSE;
				}
				m_bZoomOut = FALSE;
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
					pcContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&pcView, NULL);
					pcView->Release();
				}
				pcContext->RSSetViewports(dwNumViewports, psViewport);
				pcBackBuffer->Release();

				// create all bool
				bool bAllCreated = true;

				// create vertex shader
				if (!m_pcVertexShader11)
				{
					if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
						bAllCreated = false;
				}
				// create pixel shader... TODO !! add option to switch output
				if (!m_pcPixelShader11)
				{
					if (FAILED(CreateSimplePixelShader(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
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
					if (FAILED(CreateMatrixConstantBuffer(pcDevice, &m_pcConstantBufferDirect11)))
						bAllCreated = false;
				}

				if (bAllCreated)
				{
					// left/right eye
					for (int nEye = 0; nEye < 2; nEye++)
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