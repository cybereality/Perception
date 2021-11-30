/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Main.cpp> :
Copyright (C) 2012 Andres Hernandez
Restructuring 2021 by Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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

#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Resource.h"
#include "dependecies/json/json.hpp"
#include ".\dependecies\imgui\imgui.h"
#include ".\dependecies\imgui\imgui_impl_glfw.h"
#include ".\dependecies\imgui\imgui_impl_opengl3.h"
#include ".\dependecies\imgui\imgui_internal.h"
#include ".\dependecies\imgui\imgui_helpers.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"%d", __LINE__); OutputDebugString(buf); }

#define APP_VERSION	  "4.0.0"

#define APP_SIZE_WIDTH 800
#define APP_SIZE_HEIGHT 360
#define APP_COLOR_FONT RGB(100, 40, 40)
#define APP_COLOR_BACK RGB(30, 5, 5)
#define APP_SIZE_FONT 22

#define APP_BUTTON_PENCOLOR RGB(0, 0, 0)
#define APP_BUTTON_PENCOLOR_INIT RGB(0, 0, 0)
#define APP_BUTTON_PENCOLOR_IDLE RGB(128, 128, 128)
#define APP_BUTTON_PENCOLOR_INJECTING RGB(255, 255, 0)
#define APP_BUTTON_PENCOLOR_TOINJECT RGB(128, 128, 0)
#define APP_BUTTON_PENCOLOR_INJECTED RGB(0, 255, 0)
#define APP_BUTTON_PENCOLOR_CLOSED RGB(255, 0, 0)
#define APP_BUTTON_BRUSHCOLOR RGB(80, 60, 60)
#define APP_FONT L"Constantia"
#define APP_FONT_ITALIC FALSE

#define DEBUG_UINT_A(a) { char buf[128]; wsprintf(buf, "%u", a); OutputDebugString(buf); }
#define DEBUG_HEX_W(a) { wchar_t buf[128]; wsprintfW(buf, L"%x", a); OutputDebugStringW(buf); }

using namespace std;
using namespace nlohmann;

#pragma region /// => fields

/// <summary>Aquilinus Runtime Environment type definitions</summary>
typedef void(*AQUILINUS_Init)();
typedef void(*AQUILINUS_Close)();
typedef void(*AQUILINUS_ForceIdle)();
typedef void(*AQUILINUS_LoadProfile)(LPCWSTR szPath, LPCWSTR szProcess, LPCWSTR* szProcReturn, DWORD unRepetition, DWORD unTimedelay);
typedef void(*AQUILINUS_Reinject)();
typedef int(*AQUILINUS_GetInjectionState)();

/// <summary>Aquilinus Runtime Environment methods</summary>
AQUILINUS_Init g_pAquilinus_Init;
AQUILINUS_Close g_pAquilinus_Close;
AQUILINUS_ForceIdle g_pAquilinus_ForceIdle;
AQUILINUS_LoadProfile g_pAquilinus_LoadProfile;
AQUILINUS_Reinject g_pAquilinus_Reinject;
AQUILINUS_GetInjectionState g_pAquilinus_GetInjectionState;

/// <summary>Handle to the Aquilinus Runtime Environment.</summary>
HMODULE g_hmAquilinusRTE = NULL;

/// <summary>True if Aquilinus profile to be loaded.</summary>
bool g_bLoadAquilinusProfile = false;

/// <summary>Aquilinus injection state.</summary>
enum InjectionState
{
	Initial = 0,
	Idle = 1,
	Injecting = 2,
	ToInject = 3,
	Injected = 4,
	Closed = 5
} g_eInjectionState;

/// <summary>
/// Game profiles structure.
/// Contains any information about a game process.
/// </summary>
struct VireioGameProfile
{
	enum DX_Version
	{
		DX_Version_9,
		DX_Version_10,
		DX_Version_11,
	} eDX_Version;
	enum CPU_Architecture
	{
		CPU_Architecture_32bit,
		CPU_Architecture_64bit
	} eCPU_Architecture;
	std::string atGameName;
	std::string atGameProcess;
	std::string atProfileFilePath;
	std::string atGameImageUrl;
	DWORD uTimedelay;
	DWORD uRepetition;
};

/// <summary>Vector containing all game profiles.</summary>
std::vector<VireioGameProfile> m_asVireioGameProfiles;

#pragma endregion

#pragma region /// => methods

/// <summary>Loads the Aquilinus runtime environment, if present.</summary>
/// <returns>True if the Aquilinus runtime environment is presentand loaded.</returns>
bool LoadAquilinusRTE()
{
	// explicit Aquilinus Runtime Environment dll import
	g_hmAquilinusRTE = LoadLibraryW(L"AquilinusRTE.dll");

	// get Aquilinus Runtime Environment methods
	if (g_hmAquilinusRTE != NULL)
	{
		// get methods explicit
		g_pAquilinus_Init = (AQUILINUS_Init)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_Init");
		g_pAquilinus_Close = (AQUILINUS_Close)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_Close");
		g_pAquilinus_ForceIdle = (AQUILINUS_ForceIdle)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_ForceIdle");
		g_pAquilinus_LoadProfile = (AQUILINUS_LoadProfile)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_LoadProfile");
		g_pAquilinus_Reinject = (AQUILINUS_Reinject)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_Reinject");
		g_pAquilinus_GetInjectionState = (AQUILINUS_GetInjectionState)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_GetInjectionState");

		if ((!g_pAquilinus_Init) || (!g_pAquilinus_Close) || (!g_pAquilinus_ForceIdle) || (!g_pAquilinus_LoadProfile) || (!g_pAquilinus_Reinject) || (!g_pAquilinus_GetInjectionState))
		{
			OutputDebugString(L"[Perception] Aquilinus Runtime Environment method(s) not found !");
			FreeLibrary(g_hmAquilinusRTE);
			g_hmAquilinusRTE = NULL;
			return false;
		}
		else
			g_pAquilinus_Init();
	}
	else
	{
		OutputDebugString(L"[Perception] Aquilinus Runtime not found !");
		return false;
	}

	return true;
}

/// <summary>simple path helper</summary>
std::string getCurrentPath(void)
{
	char atFullPath[1024];
	int nLen = 1024;
	int nBytes = GetModuleFileNameA(NULL, atFullPath, nLen);
	if (nBytes == 0)
	{
		OutputDebugString(L"[Perception] GetModuleFileName FAILS !");
		return "";
	}

	std::string atPathName = std::string(atFullPath);
#ifdef _WIN64
	std::string atExeName = "bin\\Perception_x64.exe";
#else
	std::string exeName = "bin\\Perception_Win32.exe";
#endif
	std::string atBasePath = "";
	size_t uPos = atPathName.find(atExeName);

	if (uPos != std::string::npos)
		atBasePath = atPathName.substr(0, int(uPos));

	return atBasePath;
}

/// <summary>Vireio v3 config registry path method</summary>
/// <summary>Init registry configuration.</summary>
bool InitConfig(void)
{
	std::string atBasePath = getCurrentPath();
	OutputDebugString(L"Current Path: ");
	OutputDebugStringA(atBasePath.c_str());
	OutputDebugString(L"\n");

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG createRes = RegCreateKeyEx(HKEY_CURRENT_USER, sk, 0, NULL, 0, 0, NULL, &hKey, NULL);

	if (createRes == ERROR_SUCCESS)
		OutputDebugString(L"[Perception] Success creating Registry.\n");
	else
		OutputDebugString(L"[Perception] Error creating Registry.\n");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS, &hKey);

	if (openRes == ERROR_SUCCESS)
		OutputDebugString(L"[Perception] Success opening key.\n");
	else
		OutputDebugString(L"[Perception] Error opening key.\n");

	LONG setRes = RegSetValueEx(hKey, TEXT("BasePath"), 0, REG_SZ, (LPBYTE)atBasePath.c_str(), (int)atBasePath.length() + 1);

	if (setRes == ERROR_SUCCESS)
		OutputDebugString(L"[Perception] Success writing to Registry.\n");
	else
		OutputDebugString(L"[Perception] Error writing to Registry.\n");

	LONG closeOut = RegCloseKey(hKey);

	if (closeOut == ERROR_SUCCESS)
		OutputDebugString(L"[Perception] Success closing key.\n");
	else
		OutputDebugString(L"[Perception] Error closing key.\n");

	return true;
}

/// <summary>lowercase helper</summary>
string StringToLower(const string& str)
{
	string result = str;
	for (unsigned ii = 0; ii < result.size(); ii++)
		result[ii] = ::tolower(result[ii]);
	return result;
}

#pragma endregion 

#pragma region /// => main class / windows entry

/// <summary>Vireio Perception Window Class</summary>
class Vireio_Perception_Main_Window
{
private:
	const LPCWSTR    m_atWindow_class_name{ L"perception" };
	HINSTANCE        m_pvInstance_handle;
	HCURSOR          m_pvCursor_arrow;
	HWND             m_pvWindow_handle;
	HWND             m_pvHeader_handle;
	RECT             m_sClient_rectangle;
	RECT             m_sExtended_profile_rectangle;
	static HBITMAP     m_pvLogo_bitmap;
	static HBITMAP     m_apvButton_bitmap[6];
	static POINT       m_psMouseCursor;           /**< The current mouse cursor. **/
	static UINT        m_uListGameProfiles;       /**< Game profiles selection. ***/
	static UINT        m_uSpinHMD;                /**< HMD selection. ***/
	static UINT        m_uSpinStereoView;         /**< Main driver stereo view selection. ***/
	static UINT        m_uSpinTracker;            /**< Main driver tracker selection. ***/
	static UINT        m_uSpinMonitor;            /**< Main driver monitor selection. ***/
	static UINT        m_uLoadAquilinusProfile;   /**< The Load Profile Button. (184x69 - same as 'Steam' thumbnails)  ***/
	static UINT        m_uMinimize;               /**< Minimize button. ***/
	static UINT        m_uExit;                   /**< Exit button. ***/
	static std::vector<std::wstring> m_aatGames;  /**< Game profiles strings ***/
	static string      m_atAppVersion;            /**< Version string for the main app ***/

public:
	/// <summary>Window Constructor.</summary>
	Vireio_Perception_Main_Window(HINSTANCE pvHinst)
	{
		// get screen and module handle
		int nScreen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		int nScreen_height = GetSystemMetrics(SM_CYFULLSCREEN);
		m_pvInstance_handle = pvHinst;

		// register window class
		WNDCLASS sWindow_class = { CS_OWNDC, main_window_proc, 0, 0,
			m_pvInstance_handle, NULL,
			NULL, NULL, NULL,
			m_atWindow_class_name };

		sWindow_class.hIcon = LoadIcon(m_pvInstance_handle, MAKEINTRESOURCE(IDI_ICON_BIG));
		if (!RegisterClass(&sWindow_class))
		{
			OutputDebugStringA("[Perception] Failed to create main window !");
			return;
		}

		// create the window
		m_pvWindow_handle = CreateWindowEx(WS_EX_COMPOSITED,
			m_atWindow_class_name,
			L"Vireio Perception",
			WS_POPUP | WS_BORDER,
			(nScreen_width - 585) / 2, (nScreen_height - 240) / 2,
			APP_SIZE_WIDTH, APP_SIZE_HEIGHT,
			NULL, NULL, m_pvInstance_handle, NULL);

		if (!m_pvWindow_handle)
		{
			OutputDebugStringA("[Perception] Failed to create main window !");
			return;
		}

		GetClientRect(m_pvWindow_handle, &m_sClient_rectangle);
		int width = m_sClient_rectangle.right - m_sClient_rectangle.left;
		int height = m_sClient_rectangle.bottom - m_sClient_rectangle.top;

		// read json config file
		json cConfig_json;
#ifdef _WIN64
		std::ifstream atConfig_file("..\\..\\cfg\\config_64bit.json");
		if (atConfig_file.is_open())
			atConfig_file >> cConfig_json;
		else
		{
			OutputDebugStringW(L"[Perception] Error opening config_64bit.json");
			return;
		}
#else
		std::ifstream atConfig_file("..\\..\\cfg\\config_32bit.json", std::ifstream::binary);
		if (atConfig_file.is_open())
			atConfig_file >> cConfig_json;
		else
		{
			OutputDebugStringW(L"[Perception] Error opening config_32bit.json");
			return;
		}
#endif
		UINT uProfile_index = 0;
		UINT uHMD_index = 0;

		// user settings
		uProfile_index = cConfig_json.value("profile_index", 0);
		uHMD_index = cConfig_json.value("hmd_index", 0);

		// game selection
		m_asVireioGameProfiles = std::vector<VireioGameProfile>();

		// get the profiles
		bool bProfile_found = false;
		json cProfiles_json;
		std::ifstream atProfiles_file("..\\..\\cfg\\profiles.json");
		if (atConfig_file.is_open())
			atProfiles_file >> cProfiles_json;
		else
		{
			OutputDebugStringW(L"[Perception] Error opening profiles.json");
			return;
		}

		// loop through profiles
		for (auto& cProfile : cProfiles_json["profile"])
		{
			VireioGameProfile sProfile = {};

			// get name
			if (cProfile["game_name"].empty()) continue;
			sProfile.atGameName = cProfile["game_name"];

			// get process
			if (cProfile["game_exe"].empty()) continue;
			sProfile.atGameProcess = cProfile["game_exe"];
			sProfile.atGameProcess = StringToLower(sProfile.atGameProcess);

			// get architecture
			string szCPU = cProfile.value("cpu_architecture", "32bit");

			// right cpu architecture ?
#ifdef _WIN64
			if (szCPU == string("64bit"))
			{
				sProfile.eCPU_Architecture = VireioGameProfile::CPU_Architecture_64bit;
#else
			if (szCPU == string("32bit"))
			{
				sProfile.eCPU_Architecture = VireioGameProfile::CPU_Architecture_32bit;
#endif
				// get dx version, set profile
				UINT uDX = cProfile.value("dx_version", 9);
				switch (uDX)
				{
				case 9:
#ifdef _WIN64
					sProfile.atProfileFilePath = cProfile.value("profile_path", "..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX9.aqup");
#else
					sProfile.atProfileFilePath = cProfile.value("profile_path", "..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX9.aqup");
#endif
					sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
					break;
				case 10:
					sProfile.eDX_Version = VireioGameProfile::DX_Version_10;
					break;
				case 11:
					// get v4 profile path
#ifdef _WIN64
					sProfile.atProfileFilePath = cProfile.value("profile_path", "..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX11.aqup");
#else
					sProfile.atProfileFilePath = cProfile.value("profile_path", "..\\game_profiles\\HMD\\Win32\\Mono\\Mono_Cinema_DX11.aqup");
#endif
					sProfile.eDX_Version = VireioGameProfile::DX_Version_11;
					break;
				default:
					sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
					break;
				};

				// get image url, time delay, repetition
				sProfile.atGameImageUrl = cProfile.value("image_url", "");
				sProfile.uTimedelay = cProfile.value("time_delay", 0);
				sProfile.uRepetition = cProfile.value("repetition", 0);

				// and add to list
				m_asVireioGameProfiles.push_back(sProfile);
				std::wstring szName = std::wstring(sProfile.atGameName.begin(), sProfile.atGameName.end());
				m_aatGames.push_back(szName);
			}
		}

		/*// Game selection
		sControl.m_eControlType = Vireio_Control_Type::ListBox;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 168;
		sControl.m_sSize.cx = 400;
		sControl.m_sSize.cy = APP_SIZE_FONT * 7;
		sControl.m_sListBox.m_bSelectable = true;
		sControl.m_sListBox.m_nCurrentSelection = uProfileIndex;
		sControl.m_sListBox.m_paszEntries = &m_aatGames;
		m_uListGameProfiles = m_pcVireioGUI->AddControl(dwPage, sControl);

		// spin control - HMD selection
		static std::vector<std::wstring> m_aszHMD_Options;
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		m_aszHMD_Options.push_back(L"No HMD");
		m_aszHMD_Options.push_back(L"LibOVR");
		m_aszHMD_Options.push_back(L"OpenVR");
		// OSVR deprecated -> m_aszHMD_Options.push_back(L"OSVR");
		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 128;
		sControl.m_sSize.cx = 100;
		sControl.m_sSize.cy = APP_SIZE_FONT + 12;
		sControl.m_sSpinControl.m_dwCurrentSelection = uHMDIndex;
		sControl.m_sSpinControl.m_paszEntries = &m_aszHMD_Options;
		m_uSpinHMD = m_pcVireioGUI->AddControl(dwPage, sControl);

		// create the main entries
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		static std::vector<std::wstring> sEntriesVersion;
		sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
		sControl.m_sPosition.x = APP_SIZE_WIDTH - 270;
		sControl.m_sPosition.y = APP_SIZE_HEIGHT - APP_SIZE_FONT - 6;
		sControl.m_sSize.cx = APP_SIZE_WIDTH;
		sControl.m_sSize.cy = APP_SIZE_HEIGHT - 100;
		sControl.m_sStaticListBox.m_bSelectable = false;
		sControl.m_sStaticListBox.m_paszEntries = &sEntriesVersion;
		UINT dwVersion = m_pcVireioGUI->AddControl(dwPage, sControl);*/

		std::string atDate(__DATE__);
		std::string atBuildDate = atDate.substr(4, 2) + "-" + atDate.substr(0, 3) + "-" + atDate.substr(7, 4);

		// and add all entries
		std::wstringstream szStream = std::wstringstream();
		szStream << L"v" << APP_VERSION << " " << atBuildDate.c_str();
#ifdef _WIN64
		szStream << L" 64bit";
#else
		szStream << L" 32bit";
#endif

		/*
		#ifdef _VIREIO_3
				// oculus entry... only for v3
				ZeroMemory(&sControl, sizeof(Vireio_Control));
				static std::vector<std::wstring> sEntriesOculus;
				sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
				sControl.m_sPosition.x = 16;
				sControl.m_sPosition.y = 300;
				sControl.m_sSize.cx = APP_SIZE_WIDTH;
				sControl.m_sSize.cy = APP_SIZE_HEIGHT - 100;
				sControl.m_sStaticListBox.m_bSelectable = false;
				sControl.m_sStaticListBox.m_paszEntries = &sEntriesOculus;
				UINT dwOculus = m_pcVireioGUI->AddControl(dwPage, sControl);
				szStream = std::wstringstream();
				if (oculusProfile.Name.size())
				{
					szStream << "Oculus Profile : " << oculusProfile.Name.c_str();
					m_pcVireioGUI->AddEntry(dwOculus, szStream.str().c_str());
				}
		#else
				// create the 'Load Aquilinus Profile' button
				ZeroMemory(&sControl, sizeof(Vireio_Control));
				static std::wstring szLoadProfile = std::wstring(L"   Vireio Profile");
				sControl.m_eControlType = Vireio_Control_Type::Button;
				sControl.m_sPosition.x = 452;
				sControl.m_sPosition.y = 128;
				sControl.m_sSize.cx = 184 + 12;
				sControl.m_sSize.cy = 69 + 12;
				sControl.m_sButton.m_pszText = &szLoadProfile;
				m_uLoadAquilinusProfile = m_pcVireioGUI->AddControl(dwPage, sControl);
		#endif
				// create the 'x'(=exit) button
				ZeroMemory(&sControl, sizeof(Vireio_Control));
				static std::wstring szX = std::wstring(L"x");
				sControl.m_eControlType = Vireio_Control_Type::Button;
				sControl.m_sPosition.x = APP_SIZE_WIDTH - 18;
				sControl.m_sPosition.y = 97;
				sControl.m_sSize.cx = 17;
				sControl.m_sSize.cy = 17;
				sControl.m_sButton.m_pszText = &szX;
				m_uExit = m_pcVireioGUI->AddControl(dwPage, sControl);

				// create the '_'(=minimize) button
				ZeroMemory(&sControl, sizeof(Vireio_Control));
				static std::wstring szMin = std::wstring(L"~");
				sControl.m_eControlType = Vireio_Control_Type::Button;
				sControl.m_sPosition.x = APP_SIZE_WIDTH - 36;
				sControl.m_sPosition.y = 97;
				sControl.m_sSize.cx = 17;
				sControl.m_sSize.cy = 17;
				sControl.m_sButton.m_pszText = &szMin;
				m_uMinimize = m_pcVireioGUI->AddControl(dwPage, sControl);
				*/

				// load the logo
		m_pvLogo_bitmap = (HBITMAP)LoadImage(NULL, L"..//img//logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		OutputDebugStringA("Load the bitmap\n");

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowWindow(m_pvWindow_handle, SW_SHOW);
		UpdateWindow(m_pvWindow_handle);
	}
	~Vireio_Perception_Main_Window() { UnregisterClass(m_atWindow_class_name, m_pvInstance_handle); }

	/*** Vireio_Perception_Main_Window public members ***/
	static LRESULT WINAPI main_window_proc(HWND pvWindow_handle, UINT uMessage, WPARAM uWparam, LPARAM uLparam)
	{
		switch (uMessage)
		{
		case WM_LBUTTONDOWN:
		{
			// get the mouse cursor position
			m_psMouseCursor.x = (LONG)GET_X_LPARAM(uLparam);
			m_psMouseCursor.y = (LONG)GET_Y_LPARAM(uLparam);
			break;
		}
		case WM_MOUSEMOVE:
			// move the window ?
			if (uWparam & MK_LBUTTON)
			{
				// get new mouse cursor point
				LONG nX = (LONG)GET_X_LPARAM(uLparam);
				LONG nY = (LONG)GET_Y_LPARAM(uLparam);

				if (m_psMouseCursor.y < 94)
				{
					// get the difference to the old position set only in WM_LBUTTONDOWN
					nX -= m_psMouseCursor.x;
					nY -= m_psMouseCursor.y;

					// get the old window position
					RECT rcWnd;
					GetWindowRect(pvWindow_handle, &rcWnd);

					// set the new window position
					SetWindowPos(pvWindow_handle, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
				}
			}
			UpdateWindow(pvWindow_handle);
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(pvWindow_handle, uMessage, uWparam, uLparam);
		}
		return 0;
	}
	void run()
	{
		MSG window_message;
		while (GetMessage(&window_message, NULL, 0, 0)) { TranslateMessage(&window_message); DispatchMessage(&window_message); }
	}
};

/*** Vireio_Perception_Main_Window static fields ***/
POINT       Vireio_Perception_Main_Window::m_psMouseCursor;
HBITMAP     Vireio_Perception_Main_Window::m_pvLogo_bitmap;
HBITMAP     Vireio_Perception_Main_Window::m_apvButton_bitmap[6];
UINT        Vireio_Perception_Main_Window::m_uListGameProfiles;
UINT        Vireio_Perception_Main_Window::m_uSpinHMD;
UINT        Vireio_Perception_Main_Window::m_uSpinStereoView;
UINT        Vireio_Perception_Main_Window::m_uSpinTracker;
UINT        Vireio_Perception_Main_Window::m_uSpinMonitor;
UINT        Vireio_Perception_Main_Window::m_uLoadAquilinusProfile;
UINT        Vireio_Perception_Main_Window::m_uExit;
UINT        Vireio_Perception_Main_Window::m_uMinimize;
std::vector<std::wstring> Vireio_Perception_Main_Window::m_aatGames;
string      Vireio_Perception_Main_Window::m_atAppVersion;

/**
* Main window procedure.
***/
LRESULT WINAPI ___main_window_proc(HWND pvWindow_handle, UINT uMessage, WPARAM uWparam, LPARAM uLparam)
{
	switch (uMessage)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(pvWindow_handle, uMessage, uWparam, uLparam);
	}
	return 0;

	/*switch (uMessage)
	{
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_PAINT: { return 0; }
	default: return 0;
	}
	return DefWindowProc(pvWindow_handle, uMessage, uWparam, uLparam);*/

	// call vireio gui class for all mouse actions
	/*if ((message == WM_KEYDOWN) || (message == WM_KEYUP) || (message == WM_LBUTTONDOWN) || (message == WM_LBUTTONUP) || (message == WM_MOUSEMOVE))
	{
		Vireio_GUI_Event sEvent = m_pcVireioGUI->WindowsEvent(message, wparam, lparam, 1);

		switch (sEvent.eType)
		{
		case ChangedToNext:
		case ChangedToPrevious:
		case ChangedToValue:
#ifdef _VIREIO_3
			if (sEvent.dwIndexOfControl == m_dwSpinStereoView)
			{
				ProxyHelper helper = ProxyHelper();
				helper.SaveUserConfig(anStereoModes[sEvent.dwNewValue]);
			}
			else if (sEvent.dwIndexOfControl == m_dwSpinTracker)
			{
				ProxyHelper helper = ProxyHelper();
				helper.SaveTrackerMode(anTrackerModes[sEvent.dwNewValue]);
			}
			else if (sEvent.dwIndexOfControl == m_dwSpinMonitor)
			{
				ProxyHelper helper = ProxyHelper();
				helper.SaveDisplayAdapter(anMonitors[sEvent.dwNewValue]);
			}
#else
			if (sEvent.dwIndexOfControl == m_uListGameProfiles)
			{
				// get global config
#ifdef _WIN64
				string configPath = string("..\\cfg_v4\\config_64bit.xml");
#else
				string configPath = string("..\\cfg_v4\\config_32bit.xml");
#endif
				xml_document docConfig;
				xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

				if (resultConfig.status == status_ok)
				{
					xml_node xml_config = docConfig.child("config");

					if (sEvent.dwNewValue < (UINT)m_asVireioGameProfiles.size())
						xml_config.attribute("profile_index") = sEvent.dwNewValue;

					docConfig.save_file(configPath.c_str());
				}
			}
			if (sEvent.dwIndexOfControl == m_uSpinHMD)
			{
				// get global config
#ifdef _WIN64
				string configPath = string("..\\cfg_v4\\config_64bit.xml");
#else
				string configPath = string("..\\cfg_v4\\config_32bit.xml");
#endif
				xml_document docConfig;
				xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

				if (resultConfig.status == status_ok)
				{
					xml_node xml_config = docConfig.child("config");

					if (sEvent.dwNewValue < 4)
						xml_config.attribute("hmd_index") = sEvent.dwNewValue;

					docConfig.save_file(configPath.c_str());
				}
				}
#endif
			break;
		case Pressed:
			if (sEvent.dwIndexOfControl == m_uExit)
				SendMessage(window_handle, WM_CLOSE, 0, 0);
			if (sEvent.dwIndexOfControl == m_uMinimize)
				ShowWindow(window_handle, SW_MINIMIZE);
#ifndef _VIREIO_3
			else if (sEvent.dwIndexOfControl == m_uLoadAquilinusProfile)
			{
				g_bLoadAquilinusProfile = true;
			}
#endif
			break;
		default:
			break;
		}

		// send WM_PAINT
		if (message != WM_MOUSEMOVE)
			UpdateWindow(window_handle);
			}

	switch (message)
	{
	case WM_MOUSEMOVE:
		// move the window ?
		if (wparam & MK_LBUTTON)
		{
			// get new mouse cursor point
			LONG nX = (LONG)GET_X_LPARAM(lparam);
			LONG nY = (LONG)GET_Y_LPARAM(lparam);

			if (m_psMouseCursor.y < 94)
			{
				// get the difference to the old position set only in WM_LBUTTONDOWN
				nX -= m_psMouseCursor.x;
				nY -= m_psMouseCursor.y;

				// get the old window position
				RECT rcWnd;
				GetWindowRect(window_handle, &rcWnd);

				// set the new window position
				SetWindowPos(window_handle, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
			}
		}
		UpdateWindow(window_handle);
		return 0;
	case WM_CREATE:
	{
		OutputDebugString("Create Window\n");
		break;
	}
	case WM_TIMER:
	{
		RECT client_rect;
		GetClientRect(window_handle, &client_rect);
		InvalidateRect(window_handle, &client_rect, FALSE);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		// get the mouse cursor position
		m_psMouseCursor.x = (LONG)GET_X_LPARAM(lparam);
		m_psMouseCursor.y = (LONG)GET_Y_LPARAM(lparam);
		break;
	}
	case WM_LBUTTONUP:
	{
#ifndef _VIREIO_3
		if (g_hmAquilinusRTE)
		{
			InjectionState eIS = (InjectionState)g_pAquilinus_GetInjectionState();
			if ((g_bLoadAquilinusProfile) && (eIS))
			{
				if ((eIS == InjectionState::Idle) || (eIS == InjectionState::Injecting))
				{
					OutputDebugString("Vireio Perception: Load Aquilinus Profile!");

					// get current selections
					UINT unSelectionGame = (UINT)m_pcVireioGUI->GetCurrentSelection(m_uListGameProfiles);
					if (unSelectionGame >= (UINT)m_asVireioGameProfiles.size()) unSelectionGame = 0;
					UINT unSelectionHMD = (UINT)m_pcVireioGUI->GetCurrentSelection(m_uSpinHMD);
					if (unSelectionHMD >= 4) unSelectionHMD = 0;
					OutputDebugString(m_asVireioGameProfiles[unSelectionGame].atGameName.c_str());

					// get profile path and set HMD string
					std::wstring szProfilePath = std::wstring(m_asVireioGameProfiles[unSelectionGame].atProfileFilePath.begin(), m_asVireioGameProfiles[unSelectionGame].atProfileFilePath.end());
					std::string szBasePath = getCurrentPath();
					szProfilePath = std::wstring(szBasePath.begin(), szBasePath.end()) + szProfilePath.substr(3);
					switch (unSelectionHMD)
					{
					case 0:
						szProfilePath = szProfilePath.substr(0, szProfilePath.find(L"HMD")) + L"Monitor" + szProfilePath.substr(szProfilePath.find(L"HMD") + 3);
						break;
					case 1:
						szProfilePath = szProfilePath.substr(0, szProfilePath.find(L"HMD")) + L"LibOVR" + szProfilePath.substr(szProfilePath.find(L"HMD") + 3);
						break;
					case 2:
						szProfilePath = szProfilePath.substr(0, szProfilePath.find(L"HMD")) + L"OpenVR" + szProfilePath.substr(szProfilePath.find(L"HMD") + 3);
						break;
						///OSVR deprecated -> case 3:
						///	szProfilePath = szProfilePath.substr(0, szProfilePath.find(L"HMD")) + L"OSVR" + szProfilePath.substr(szProfilePath.find(L"HMD") + 3);
						///	break;
					}
					OutputDebugStringW(szProfilePath.c_str());

					// get process
					std::wstring szProcess = std::wstring(m_asVireioGameProfiles[unSelectionGame].atGameProcess.begin(), m_asVireioGameProfiles[unSelectionGame].atGameProcess.end());

					// load aquilinus profile
					LPCWSTR szP;
					g_pAquilinus_LoadProfile(szProfilePath.c_str(), szProcess.c_str(), &szP, m_asVireioGameProfiles[unSelectionGame].uRepetition, m_asVireioGameProfiles[unSelectionGame].uTimedelay);
					std::wstring szPath = std::wstring(szP);

					//// load game logo... TODO !! LOAD BY PROFILE !!
					//std::wstring szFilename = szPath.substr(szPath.find_last_of('\\') + 1);
					//if (szFilename.find(L"Fallout4") == 0)
					//{
					//	ConvertJpgFile("..//img//Fallout4.jpg", "..//img//game_logo.bmp");
					//	game_bitmap = (HBITMAP)LoadImage(NULL, "..//img//game_logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					//}
				}
				else
					g_pAquilinus_Reinject();

				g_bLoadAquilinusProfile = false;
			}
		}
#endif
		break;
	}
	case WM_RBUTTONUP:
	{
#ifndef _VIREIO_3
		if (g_hmAquilinusRTE)
		{
			g_pAquilinus_ForceIdle();
			g_bLoadAquilinusProfile = false;
		}
#endif
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT 	ps;
		HDC 			hdc;
		BITMAP 		bitmap;
		HDC 			hdcMem;

		hdc = BeginPaint(window_handle, &ps);

		// draw GUI
		hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hGUI = m_pcVireioGUI->GetGUI(true, false, true, true);
		SelectObject(hdcMem, hGUI);
		GetObject(hGUI, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		// draw logo
		SelectObject(hdcMem, m_pvLogo_bitmap);
		GetObject(m_pvLogo_bitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);

#ifndef _VIREIO_3
		// draw profile button + close icon + minimize icon
		SelectObject(hdc, GetStockObject(DC_PEN));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		g_eInjectionState = (InjectionState)g_pAquilinus_GetInjectionState();
		switch (g_eInjectionState)
		{
		case Initial:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_INIT);
			break;
		case Idle:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_IDLE);
			break;
		case Injecting:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_INJECTING);
			break;
		case ToInject:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_TOINJECT);
			break;
		case Injected:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_INJECTED);
			break;
		case Closed:
			SetDCPenColor(hdc, APP_BUTTON_PENCOLOR_CLOSED);
			break;
		default:
			break;
		}
		SetDCBrushColor(hdc, APP_BUTTON_BRUSHCOLOR);
		Rectangle(hdc, 456, 132, 456 + 188, 132 + 73);
		SetDCPenColor(hdc, APP_BUTTON_PENCOLOR);
		Rectangle(hdc, APP_SIZE_WIDTH - 18, bitmap.bmHeight, APP_SIZE_WIDTH - 1, bitmap.bmHeight + 19);
		Rectangle(hdc, APP_SIZE_WIDTH - 36, bitmap.bmHeight, APP_SIZE_WIDTH - 19, bitmap.bmHeight + 19);
		SetDCBrushColor(hdc, APP_BUTTON_PENCOLOR);
		Rectangle(hdc, APP_SIZE_WIDTH - 12, bitmap.bmHeight + 7, APP_SIZE_WIDTH - 7, bitmap.bmHeight + 12);
		Rectangle(hdc, APP_SIZE_WIDTH - 32, bitmap.bmHeight + 14, APP_SIZE_WIDTH - 23, bitmap.bmHeight + 16);

		//// draw game logo
		//if (game_bitmap)
		//{
		//	hdcMem = CreateCompatibleDC(hdc);
		//	SelectObject(hdcMem, game_bitmap);
		//	GetObject(game_bitmap, sizeof(bitmap), &bitmap);
		//	BitBlt(hdc, 458, 134, 184, 69, hdcMem, 0, 0, SRCCOPY);
		//	DeleteDC(hdcMem);
		//}
		// draw game logo
		if (m_apvButton_bitmap[(int)g_eInjectionState])
		{
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, m_apvButton_bitmap[(int)g_eInjectionState]);
			GetObject(m_apvButton_bitmap[(int)g_eInjectionState], sizeof(bitmap), &bitmap);
			BitBlt(hdc, 458, 134, 184, 69, hdcMem, 0, 0, SRCCOPY);
			DeleteDC(hdcMem);
		}
#endif

		EndPaint(window_handle, &ps);

		/*
		// Now get FPS from the registry
		HKEY hKey;
		LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Vireio\\Perception", 0, KEY_ALL_ACCESS, &hKey);
		if (openRes == ERROR_SUCCESS)
		{
		char fpsBuffer[10];
		memset(fpsBuffer, 0, 10);
		DWORD dwDataSize = 10;
		LONG lResult = RegGetValue(hKey, NULL, "FPS", RRF_RT_REG_SZ, NULL, (LPVOID)&fpsBuffer, &dwDataSize);
		if (lResult == ERROR_SUCCESS)
		{
		TextOut(paint_device_context, 420, 183, fpsBuffer, (int)strlen(fpsBuffer));
		//Now delete, if this isn't refreshed, then we'll report blank next time round
		RegDeleteValue(hKey, "FPS");
		RegCloseKey(hKey);
		}
		else
		TextOut(paint_device_context, 420, 183, "--", 2);
		}

		// output extended profile data if aquilinus runtime environment present
		if (g_hmAquilinusRTE)
		{
		SetRect(&This->extended_profile_rectangle, 16, 190, 272, 254);
		}


		return 0;
	}
	case WM_SIZE:
	{
		InvalidateRect(window_handle, NULL, TRUE);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
#ifndef _VIREIO_3
		if (g_hmAquilinusRTE)
		{
			g_pAquilinus_Close();
			FreeLibrary(g_hmAquilinusRTE);
		}
#endif
		break;
	}
	}
	return DefWindowProc(window_handle, message, wparam, lparam);*/
};


/// <summary>Windows entry point</summary>
int WINAPI wWinMain(HINSTANCE instance_handle, HINSTANCE, LPWSTR, INT) {

	// avoid double driver window
	HWND pvWindow = FindWindow(L"perception", L"Vireio Perception");
	if (pvWindow != 0)
	{
		OutputDebugString(L"Vireio Perception is already present !");
		return 0;
	}

	// save perception path to registry (for all versions)
	InitConfig();

	// load Aquilinus Runtime Environment, init driver
	if (LoadAquilinusRTE()) OutputDebugString(L"[Perception] Init Aquilinus Runtime Environment.");

	// create and run window 
	Vireio_Perception_Main_Window main_window(instance_handle);
	main_window.run();

	return 0;
}

#pragma endregion
