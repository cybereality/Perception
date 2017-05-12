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

#define ENTRY_FONT 0

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
m_pcDSGeometry11(nullptr),
m_pcDSVGeometry11(nullptr),
m_pcSampler11(nullptr),
m_bHotkeySwitch(false),
m_eStereoMode(VireioMonitorStereoModes::Vireio_Mono),
m_bMenu(false),
m_bMenuHotkeySwitch(false),
m_pcFontSegeo128(nullptr),
m_ppcTexViewMenu(nullptr),
m_pbCinemaMode(nullptr)
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;
	m_strFontName = std::string("PassionOne");
	m_unFontSelection = 0;

	// read or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// read settings
	m_strFontName = GetIniFileSetting(m_strFontName, "Stereo Presenter", "strFontName", szFilePathINI, bFileExists);

	// TODO !! LOOP THROUGH AVAILABLE MENUES, SET SECONDARY VALUE ! (IN FIRST PROVOKING CALL)
	ZeroMemory(&m_apsSubMenues[0], sizeof(VireioSubMenu*));
	ZeroMemory(&m_sMenuControl, sizeof(MenuControl));
	ZeroMemory(&m_abMenuEvents[0], sizeof(VireioMenuEvent)* (int)VireioMenuEvent::NumberOfEvents);

	// create the presenter sub menu
	ZeroMemory(&m_sSubMenu, sizeof(VireioSubMenu));
	m_sSubMenu.strSubMenu = "Stereo Presenter";
#pragma region entry font
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Font";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = 6;
		sEntry.unChangeSize = 1;
		sEntry.bValueEnumeration = true;
		{ std::string strEnum = "PassionOne"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "ComicSansMS"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "JacintoSans"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "bitwise"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "SegoeUI128"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "UltimateGameplayer"; sEntry.astrValueEnumeration.push_back(strEnum); }
		{ std::string strEnum = "Videophreak"; sEntry.astrValueEnumeration.push_back(strEnum); }
		// TODO !! LOOP THROUGH SELECTIONS
		sEntry.punValue = &m_unFontSelection;
		sEntry.unValue = *sEntry.punValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}
#pragma endregion
	{
		static float s_fDummy = 1.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Test Entry";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 100.0f;
		sEntry.fChangeSize = 0.5f;
		sEntry.pfValue = &s_fDummy;
		sEntry.fValue = *sEntry.pfValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}
	{
		static float s_fDummy = 1.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Another dummy";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 100.0f;
		sEntry.fChangeSize = 0.5f;
		sEntry.pfValue = &s_fDummy;
		sEntry.fValue = *sEntry.pfValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}
	{
		static float s_fDummy = 1.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Yet another dummy";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 100.0f;
		sEntry.fChangeSize = 0.5f;
		sEntry.pfValue = &s_fDummy;
		sEntry.fValue = *sEntry.pfValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}
	{
		static float s_fDummy = 1.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Dummy Value";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 100.0f;
		sEntry.fChangeSize = 0.5f;
		sEntry.pfValue = &s_fDummy;
		sEntry.fValue = *sEntry.pfValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}
	{
		static float s_fDummy = 1.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Next dummy Value";
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 100.0f;
		sEntry.fChangeSize = 0.5f;
		sEntry.pfValue = &s_fDummy;
		sEntry.fValue = *sEntry.pfValue;
		m_sSubMenu.asEntries.push_back(sEntry);
	}

}

/**
* Destructor.
***/
StereoPresenter::~StereoPresenter()
{
	if (m_pcFontSegeo128) delete m_pcFontSegeo128;

	SAFE_RELEASE(m_pcDSVGeometry11);
	SAFE_RELEASE(m_pcDSGeometry11);
	SAFE_RELEASE(m_pcSampler11);
	SAFE_RELEASE(m_pcVertexShader10);
	SAFE_RELEASE(m_pcPixelShader10);
	SAFE_RELEASE(m_pcVertexLayout10);
	SAFE_RELEASE(m_pcVertexBuffer10);
	SAFE_RELEASE(m_pcBackBufferView);
	SAFE_RELEASE(m_pcConstantBufferDirect10);
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
		case STP_Decommanders::MenuTexture:
			return L"Menu Texture";
		case VireioSubMenu00:
			return L"Sub Menu 0";
		case VireioSubMenu01:
			return L"Sub Menu 1";
		case VireioSubMenu02:
			return L"Sub Menu 2";
		case VireioSubMenu03:
			return L"Sub Menu 3";
		case VireioSubMenu04:
			return L"Sub Menu 4";
		case VireioSubMenu05:
			return L"Sub Menu 5";
		case VireioSubMenu06:
			return L"Sub Menu 6";
		case VireioSubMenu07:
			return L"Sub Menu 7";
		case VireioSubMenu08:
			return L"Sub Menu 8";
		case VireioSubMenu09:
			return L"Sub Menu 9";
		case VireioSubMenu10:
			return L"Sub Menu 10";
		case VireioSubMenu11:
			return L"Sub Menu 11";
		case VireioSubMenu12:
			return L"Sub Menu 12";
		case VireioSubMenu13:
			return L"Sub Menu 13";
		case VireioSubMenu14:
			return L"Sub Menu 14";
		case VireioSubMenu15:
			return L"Sub Menu 15";
		case VireioSubMenu16:
			return L"Sub Menu 16";
		case VireioSubMenu17:
			return L"Sub Menu 17";
		case VireioSubMenu18:
			return L"Sub Menu 18";
		case VireioSubMenu19:
			return L"Sub Menu 19";
		case VireioSubMenu20:
			return L"Sub Menu 20";
		case VireioSubMenu21:
			return L"Sub Menu 21";
		case VireioSubMenu22:
			return L"Sub Menu 22";
		case VireioSubMenu23:
			return L"Sub Menu 23";
		case VireioSubMenu24:
			return L"Sub Menu 24";
		case VireioSubMenu25:
			return L"Sub Menu 25";
		case VireioSubMenu26:
			return L"Sub Menu 26";
		case VireioSubMenu27:
			return L"Sub Menu 27";
		case VireioSubMenu28:
			return L"Sub Menu 28";
		case VireioSubMenu29:
			return L"Sub Menu 29";
		case VireioSubMenu30:
			return L"Sub Menu 30";
		case VireioSubMenu31:
			return L"Sub Menu 31";
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD StereoPresenter::GetCommanderType(DWORD dwCommanderIndex)
{
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
		case STP_Decommanders::MenuTexture:
			return NOD_Plugtype::AQU_PNT_ID3D11RENDERTARGETVIEW;
		case VireioSubMenu00:
		case VireioSubMenu01:
		case VireioSubMenu02:
		case VireioSubMenu03:
		case VireioSubMenu04:
		case VireioSubMenu05:
		case VireioSubMenu06:
		case VireioSubMenu07:
		case VireioSubMenu08:
		case VireioSubMenu09:
		case VireioSubMenu10:
		case VireioSubMenu11:
		case VireioSubMenu12:
		case VireioSubMenu13:
		case VireioSubMenu14:
		case VireioSubMenu15:
		case VireioSubMenu16:
		case VireioSubMenu17:
		case VireioSubMenu18:
		case VireioSubMenu19:
		case VireioSubMenu20:
		case VireioSubMenu21:
		case VireioSubMenu22:
		case VireioSubMenu23:
		case VireioSubMenu24:
		case VireioSubMenu25:
		case VireioSubMenu26:
		case VireioSubMenu27:
		case VireioSubMenu28:
		case VireioSubMenu29:
		case VireioSubMenu30:
		case VireioSubMenu31:
			return NOD_Plugtype::AQU_VOID;
			break;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* StereoPresenter::GetOutputPointer(DWORD dwCommanderIndex)
{
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
		case STP_Decommanders::MenuTexture:
			m_ppcTexViewMenu = (ID3D11RenderTargetView**)pData;
			break;
		case VireioSubMenu00:
		case VireioSubMenu01:
		case VireioSubMenu02:
		case VireioSubMenu03:
		case VireioSubMenu04:
		case VireioSubMenu05:
		case VireioSubMenu06:
		case VireioSubMenu07:
		case VireioSubMenu08:
		case VireioSubMenu09:
		case VireioSubMenu10:
		case VireioSubMenu11:
		case VireioSubMenu12:
		case VireioSubMenu13:
		case VireioSubMenu14:
		case VireioSubMenu15:
		case VireioSubMenu16:
		case VireioSubMenu17:
		case VireioSubMenu18:
		case VireioSubMenu19:
		case VireioSubMenu20:
		case VireioSubMenu21:
		case VireioSubMenu22:
		case VireioSubMenu23:
		case VireioSubMenu24:
		case VireioSubMenu25:
		case VireioSubMenu26:
		case VireioSubMenu27:
		case VireioSubMenu28:
		case VireioSubMenu29:
		case VireioSubMenu30:
		case VireioSubMenu31:
			m_apsSubMenues[dwDecommanderIndex - 3] = (VireioSubMenu*)pData;
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

	// update our global time
	static float fGlobalTime = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	fGlobalTime = (dwTimeCur - dwTimeStart) / 1000.0f;

	// only present accepted
	bool bValid = false;
	if (((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)) ||
		((eD3DInterface == INTERFACE_IDIRECT3DDEVICE9) && (eD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT))) bValid = true;
	if (!bValid) return nullptr;

	// clear all previous menu events
	ZeroMemory(&m_abMenuEvents[0], sizeof(VireioMenuEvent)* (int)VireioMenuEvent::NumberOfEvents);

	// sub menu update ?
	if (m_sSubMenu.bOnChanged)
	{
		m_sSubMenu.bOnChanged = false;

		// loop through entries
		for (size_t nIx = 0; nIx < m_sSubMenu.asEntries.size(); nIx++)
		{
			// entry index changed ?
			if (m_sSubMenu.asEntries[nIx].bOnChanged)
			{
				m_sSubMenu.asEntries[nIx].bOnChanged = false;

				// font ?
				if (nIx == ENTRY_FONT)
				{
					// get device and context
					ID3D11Device* pcDevice = nullptr;
					ID3D11DeviceContext* pcContext = nullptr;
					if (SUCCEEDED(GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext)))
					{
						HRESULT nHr;
						// get base directory
						std::string strVireioPath = GetBaseDir();

						// add file path
						strVireioPath += "font//";
						strVireioPath += m_sSubMenu.asEntries[nIx].astrValueEnumeration[m_sSubMenu.asEntries[nIx].unValue];
						strVireioPath += ".spritefont";
						OutputDebugStringA(strVireioPath.c_str());

						// create font, make backup
						VireioFont* pcOldFont = m_pcFontSegeo128;
						m_pcFontSegeo128 = new VireioFont(pcDevice, pcContext, strVireioPath.c_str(), 128.0f, 1.0f, nHr, 1);
						if (FAILED(nHr))
						{
							delete m_pcFontSegeo128; m_pcFontSegeo128 = pcOldFont;
						}
						else
						{
							// set new font name
							m_strFontName = m_sSubMenu.asEntries[nIx].astrValueEnumeration[m_sSubMenu.asEntries[nIx].unValue];

							// write to ini file
							char szFilePathINI[1024];
							GetCurrentDirectoryA(1024, szFilePathINI);
							strcat_s(szFilePathINI, "\\VireioPerception.ini");
							WritePrivateProfileStringA("Stereo Presenter", "strFontName", m_strFontName.c_str(), szFilePathINI);
						}
					}
					SAFE_RELEASE(pcDevice);
					SAFE_RELEASE(pcContext);
				}

			}
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
#pragma region menu hotkeys
		static bool bReleased = true;

		// static hotkeys :  LCTRL+Q - toggle vireio menu
		//                   F12 - toggle stereo output
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
#pragma endregion
#pragma region menu events
		// handle controller
		if (bControllerAttached)
		{
			if (sControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			{
				m_abMenuEvents[VireioMenuEvent::OnExit] = TRUE;
			}
			if (sControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				m_abMenuEvents[VireioMenuEvent::OnAccept] = TRUE;
			}
			if (sControllerState.Gamepad.sThumbLY > 28000)
				m_abMenuEvents[VireioMenuEvent::OnUp] = TRUE;
			if (sControllerState.Gamepad.sThumbLY < -28000)
				m_abMenuEvents[VireioMenuEvent::OnDown] = TRUE;
			if (sControllerState.Gamepad.sThumbLX < -28000)
				m_abMenuEvents[VireioMenuEvent::OnLeft] = TRUE;
			if (sControllerState.Gamepad.sThumbLX > 28000)
				m_abMenuEvents[VireioMenuEvent::OnRight] = TRUE;
		}
#pragma endregion
#pragma region menu update

		// TODO !! OWN METHOD
		VireioSubMenu* psSubMenu = &m_sSubMenu;

		// set the menu y origin
		m_sMenuControl.fYOrigin = -2.8f + (float)m_sMenuControl.unSelection * -1.4f;

		if (m_sMenuControl.eSelectionMovement)
		{
			// how much action time elapsed for the movement ?
			float fActionTimeElapsed = (fGlobalTime - m_sMenuControl.fActionStartTime) / m_sMenuControl.fActionTime;

			// is the movement done ?
			if (fActionTimeElapsed > 1.0f)
			{
				fActionTimeElapsed = 1.0f;
				m_sMenuControl.eSelectionMovement = MenuControl::SelectionMovement::Standing;
			}

			// compute the movement origin
			float fOldOrigin = -2.8f + (float)m_sMenuControl.unSelectionFormer * -1.4f;
			float fYOriginMovement = (fOldOrigin - m_sMenuControl.fYOrigin) * (1.0f - sin(fActionTimeElapsed*PI_F*0.5f));
			m_sMenuControl.fYOrigin += fYOriginMovement;
		}
		else
		{
			// no selection movement, events possible...up
			if ((m_abMenuEvents[VireioMenuEvent::OnUp]) && (m_sMenuControl.unSelection > 0))
			{
				// set event
				m_sMenuControl.eSelectionMovement = MenuControl::SelectionMovement::MovesUp;
				m_sMenuControl.fActionTime = 0.3f;
				m_sMenuControl.fActionStartTime = fGlobalTime;

				// set selection
				m_sMenuControl.unSelectionFormer = m_sMenuControl.unSelection;
				m_sMenuControl.unSelection--;
			}
			// down
			if ((m_abMenuEvents[VireioMenuEvent::OnDown]) && (m_sMenuControl.unSelection < (UINT)(psSubMenu->asEntries.size() - 1)))
			{
				// set event
				m_sMenuControl.eSelectionMovement = MenuControl::SelectionMovement::MovesDown;
				m_sMenuControl.fActionTime = 0.3f;
				m_sMenuControl.fActionStartTime = fGlobalTime;

				// set selection
				m_sMenuControl.unSelectionFormer = m_sMenuControl.unSelection;
				m_sMenuControl.unSelection++;
			}
			// left

			// right

			// accept
			if (m_abMenuEvents[VireioMenuEvent::OnAccept])
			{
				// set event
				m_sMenuControl.eSelectionMovement = MenuControl::SelectionMovement::Accepted;
				m_sMenuControl.fActionTime = 0.3f;
				m_sMenuControl.fActionStartTime = fGlobalTime;

				// update sub menu, first get index and set events
				UINT unIx = m_sMenuControl.unSelection;
				psSubMenu->bOnAccept = true;
				psSubMenu->bOnChanged = true;
				psSubMenu->asEntries[unIx].bOnChanged = true;

				// is this a string selection enumeration entry ?
				if (psSubMenu->asEntries[unIx].bValueEnumeration)
				{
					// before last entry in enumeration list ?
					if (psSubMenu->asEntries[unIx].unValue < (psSubMenu->asEntries[unIx].astrValueEnumeration.size() - 1))
						psSubMenu->asEntries[unIx].unValue++;
					else
						psSubMenu->asEntries[unIx].unValue = 0;
				}
				else
				{
				}
			}
		}

#pragma endregion
#pragma region render menu (if opened)

		// menu is shown ?
		if (m_bMenu)
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

			// get the viewport
			UINT dwNumViewports = 1;
			D3D11_VIEWPORT psViewport[16];
			pcContext->RSGetViewports(&dwNumViewports, psViewport);

			// backup all states
			D3DX11_STATE_BLOCK sStateBlock;
			CreateStateblock(pcContext, &sStateBlock);

			// clear all states, set targets
			ClearContextState(pcContext);

			// set the menu texture (if present)
			if (m_ppcTexViewMenu)
			{
				if (*m_ppcTexViewMenu)
				{
					// set render target
					ID3D11RenderTargetView* pcRTView = *m_ppcTexViewMenu;
					pcContext->OMSetRenderTargets(1, &pcRTView, NULL);

					// set viewport
					D3D11_VIEWPORT sViewport = {};
					sViewport.TopLeftX = 0;
					sViewport.TopLeftY = 0;
					sViewport.Width = 1024;
					sViewport.Height = 1024;
					sViewport.MinDepth = 0.0f;
					sViewport.MaxDepth = 1.0f;
					pcContext->RSSetViewports(1, &sViewport);

					// clear render target...zero alpha
					FLOAT afColorRgba[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					pcContext->ClearRenderTargetView(*m_ppcTexViewMenu, afColorRgba);
				}
			}
			else
			{
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
			}

			// create the font class if not present 
			HRESULT nHr = S_OK;
			if (!m_pcFontSegeo128)
			{
				// get base directory
				std::string strVireioPath = GetBaseDir();

				// add file path
				strVireioPath += "font//";
				strVireioPath += m_strFontName;
				strVireioPath += ".spritefont";
				OutputDebugStringA(strVireioPath.c_str());

				// create font
				m_pcFontSegeo128 = new VireioFont(pcDevice, pcContext, strVireioPath.c_str(), 128.0f, 1.0f, nHr, 1);
			}
			if (FAILED(nHr)) { delete m_pcFontSegeo128; m_pcFontSegeo128 = nullptr; }

			// test draw... TODO !!

			// render text (if font present)
			if (m_pcFontSegeo128)
			{
				m_pcFontSegeo128->SetTextAttributes(0.0f, 0.2f, 0.0001f);
				m_pcFontSegeo128->ToRender(pcContext, fGlobalTime, /*(sin(fGlobalTime * 0.05f)*5.0f) - 6.0f*/m_sMenuControl.fYOrigin, 30.0f);
				RenderMenu(pcDevice, pcContext);
			}
			else OutputDebugString(L"Failed to create font!");

			// set back device
			ApplyStateblock(pcContext, &sStateBlock);

			if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
			if (pcContext) { pcContext->Release(); pcContext = nullptr; }
		}
#pragma endregion
#pragma region draw stereo (optionally)

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

/**
* Menu render method.
***/
void StereoPresenter::RenderMenu(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext)
{
	// TODO !! SWITCH MAIN->SUB MENU

	// TODO !! LOOP THROUGH DIFFERENT SUB MENUES

	RenderSubMenu(pcDevice, pcContext, &m_sSubMenu);
}

/**
* Renders a sub menu.
***/
void StereoPresenter::RenderSubMenu(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, VireioSubMenu* psSubMenu)
{
	// render title
	m_pcFontSegeo128->RenderTextLine(pcDevice, pcContext, psSubMenu->strSubMenu.c_str());
	m_pcFontSegeo128->Enter();

	// loop through entries
	for (size_t nEntryIx = 0; nEntryIx < psSubMenu->asEntries.size(); nEntryIx++)
	{
		// create an output stream based on entry string
		std::stringstream strOutput = std::stringstream();
		strOutput << psSubMenu->asEntries[nEntryIx].strEntry;

		// switch by entry type
		switch (psSubMenu->asEntries[nEntryIx].eType)
		{
			case VireioMenuEntry::Entry_Bool:
				if (psSubMenu->asEntries[nEntryIx].bValue)
					strOutput << " - True ";
				else
					strOutput << " - False ";
				break;
			case VireioMenuEntry::Entry_Int:
				strOutput << " : " << psSubMenu->asEntries[nEntryIx].nValue;
				break;
			case VireioMenuEntry::Entry_UInt:
				if (psSubMenu->asEntries[nEntryIx].bValueEnumeration)
				{
					// render the enumeration string
					UINT unIx = psSubMenu->asEntries[nEntryIx].unValue;
					if (unIx < (UINT)psSubMenu->asEntries[nEntryIx].astrValueEnumeration.size())
						strOutput << " : " << psSubMenu->asEntries[nEntryIx].astrValueEnumeration[unIx];
				}
				else
				{
					strOutput << " : " << psSubMenu->asEntries[nEntryIx].unValue;
				}
				break;
			case VireioMenuEntry::Entry_Float:
				strOutput << " : " << psSubMenu->asEntries[nEntryIx].fValue;
				break;
			default:
				strOutput << "ERROR";
				break;
		}

		// render the text line
		m_pcFontSegeo128->RenderTextLine(pcDevice, pcContext, strOutput.str().c_str());
	}
}
