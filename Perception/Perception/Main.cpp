/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Main.cpp> :
Copyright (C) 2012 Andres Hernandez

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

// define this for Version 3 compile ----->>>>>> #define _VIREIO_3

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <Shlwapi.h>
#include <urlmon.h>
#include <string>
#include "hijackdll.h"
#include <CommCtrl.h> 
#include <ctime>  
#include <cstdlib>  
#include "ProxyHelper.h"
#include <vector>
#include "Resource.h"
#include "Version.h"
#include "..\PluginSection\Include\Vireio_GUI.h"
#include "loadjpg.h"
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shlwapi.lib")
#ifndef _VIREIO_3
#include "pugixml.hpp"
using namespace pugi;
#endif

#define APP_SIZE_WIDTH 800
#define APP_SIZE_HEIGHT 360
#ifdef _VIREIO_3
#define APP_COLOR_FONT RGB(10, 140, 100)
#define APP_COLOR_BACK RGB(10, 30, 40)
#define APP_SIZE_FONT 32
#else
#define APP_COLOR_FONT RGB(100, 40, 40)
#define APP_COLOR_BACK RGB(30, 5, 5)
#define APP_SIZE_FONT 22
#endif
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

#ifndef _VIREIO_3
/*** Aquilinus Initialization method ***/
bool LoadAquilinusRTE();

/*** Aquilinus Runtime Environment type definitions ***/
typedef void(*AQUILINUS_Init)();
typedef void(*AQUILINUS_Close)();
typedef void(*AQUILINUS_ForceIdle)();
typedef void(*AQUILINUS_LoadProfile)(LPCWSTR szPath, LPCWSTR szProcess, LPCWSTR* szProcReturn, DWORD unRepetition, DWORD unTimedelay);
typedef void(*AQUILINUS_Reinject)();
typedef int(*AQUILINUS_GetInjectionState)();

/*** Aquilinus Runtime Environment methods ***/
AQUILINUS_Init g_pAquilinus_Init;
AQUILINUS_Close g_pAquilinus_Close;
AQUILINUS_ForceIdle g_pAquilinus_ForceIdle;
AQUILINUS_LoadProfile g_pAquilinus_LoadProfile;
AQUILINUS_Reinject g_pAquilinus_Reinject;
AQUILINUS_GetInjectionState g_pAquilinus_GetInjectionState;

/**
* Handle to the Aquilinus Runtime Environment.
***/
HMODULE g_hmAquilinusRTE = NULL;
/**
* True if Aquilinus profile to be loaded.
***/
bool g_bLoadAquilinusProfile = false;
/**
* Aquilinus injection state.
***/
enum InjectionState
{
	Initial = 0,
	Idle = 1,
	Injecting = 2,
	ToInject = 3,
	Injected = 4,
	Closed = 5
} g_eInjectionState;
/**
* Game profiles structure.
* Contains any information about a game process.
***/
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
	std::string szGameName;
	std::string szGameProcess;
	std::string szProfileFilePath;
	std::string szGameImageUrl;
	DWORD unTimedelay;
	DWORD unRepetition;
};
/**
* Vector containing all mono cinema profiles.
***/
std::vector<VireioGameProfile> m_asVireioGameProfiles;

#else
/*** Vireio v3 methods and fields ***/
bool InitConfig(void);
bool InitModes(void);
void LoadDLL(void);
HINSTANCE hD3D9;
ProxyHelper::OculusProfile oculusProfile;

/**
* Vector of Vireio Perception Stereo View Render Modes.
***/
vector<int> anStereoModes;
/**
* Vector of Vireio Perception Head Tracker Modes.
***/
vector<int> anTrackerModes;
/**
* Vector of installed Monitors.
***/
vector<int> anMonitors;
#endif
/*** Vireio methods ***/
std::string getCurrentPath(void);


/**
* Simple helper
***/
string StringToLower(const string& str)
{
	string result = str;
	for (unsigned ii = 0; ii < result.size(); ii++)
		result[ii] = ::tolower(result[ii]);
	return result;
}

/**
* Vireio Perception Window Class
***/
class Vireio_Perception_Main_Window
{
private:
	LPCSTR    window_class_name;
	HINSTANCE instance_handle;
	HCURSOR   cursor_arrow;
	HWND      window_handle;
	HWND      header_handle;
	RECT      client_rectangle;
	RECT      extended_profile_rectangle;
	static HBITMAP     logo_bitmap;
	static HBITMAP     game_bitmap;
	static POINT       m_ptMouseCursor;            /**< The current mouse cursor. **/
	static Vireio_GUI* m_pcVireioGUI;              /**< Vireio Graphical User Interface class. **/
	static UINT        m_dwListGameProfiles;       /**< Game profiles selection. ***/
	static UINT        m_dwSpinHMD;                /**< HMD selection. ***/
	static UINT        m_dwSpinStereoView;         /**< Main driver stereo view selection. ***/
	static UINT        m_dwSpinTracker;            /**< Main driver tracker selection. ***/
	static UINT        m_dwSpinMonitor;            /**< Main driver monitor selection. ***/
	static UINT        m_dwLoadAquilinusProfile;   /**< The Load Profile Button. (184x69 - same as 'Steam' thumbnails)  ***/
	static UINT        m_dwExit;                   /**< Exit button. ***/
public:
	/**
	* Window Constructor.
	***/
	Vireio_Perception_Main_Window(LPCSTR window_class_identity) :
		window_class_name(window_class_identity)
	{
		// get screen and module handle
		int screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		int screen_height = GetSystemMetrics(SM_CYFULLSCREEN);
		instance_handle = GetModuleHandle(NULL);

		// register window class
		WNDCLASS window_class = { CS_OWNDC, main_window_proc, 0, 0,
			instance_handle, NULL,
			NULL, NULL, NULL,
			window_class_name };

		window_class.hIcon = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_ICON_BIG));
		RegisterClass(&window_class);

		// create the window
		window_handle = CreateWindowEx(WS_EX_COMPOSITED,
			window_class_name,
			"Vireio Perception",
			WS_POPUP | WS_BORDER,
			(screen_width - 585) / 2, (screen_height - 240) / 2,
			APP_SIZE_WIDTH, APP_SIZE_HEIGHT,
			NULL, NULL, instance_handle, NULL);

		GetClientRect(window_handle, &client_rectangle);
		int width = client_rectangle.right - client_rectangle.left;
		int height = client_rectangle.bottom - client_rectangle.top;

#ifdef _VIREIO_3
		// load proxy config
		ProxyHelper helper = ProxyHelper();
		ProxyConfig cfg;
		helper.LoadUserConfig(cfg, oculusProfile);
#else
		// get global config
#ifdef _WIN64
		string configPath = string("..\\cfg_v4\\config_64bit.xml");
#else
		string configPath = string("..\\cfg_v4\\config_32bit.xml");
#endif
		xml_document docConfig;
		xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());
		UINT unProfileIndex = 0;
		UINT unHMDIndex = 0;

		if (resultConfig.status == status_ok)
		{
			xml_node xml_config = docConfig.child("config");

			unProfileIndex = xml_config.attribute("profile_index").as_uint(0);
			unHMDIndex = xml_config.attribute("hmd_index").as_uint(0);
		}
#endif

		// create the vireio gui
		SIZE sSizeOfThis;
		sSizeOfThis.cx = APP_SIZE_WIDTH; sSizeOfThis.cy = APP_SIZE_HEIGHT;
		m_pcVireioGUI = new Vireio_GUI(sSizeOfThis, APP_FONT, APP_FONT_ITALIC, APP_SIZE_FONT, APP_COLOR_FONT, APP_COLOR_BACK);
		UINT dwPage = m_pcVireioGUI->AddPage();

		// control structure
		Vireio_Control sControl;

#ifdef _VIREIO_3
		// spin control - stereo view
		static std::vector<std::wstring> m_aszStereoViewOptions;
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		m_aszStereoViewOptions.push_back(L"Disabled - 0");
		m_aszStereoViewOptions.push_back(L"Side by Side - 20");
		m_aszStereoViewOptions.push_back(L"Over Under - 30");
		m_aszStereoViewOptions.push_back(L"Horizontal Interleave - 40");
		m_aszStereoViewOptions.push_back(L"Vertical Interleave - 50");
		m_aszStereoViewOptions.push_back(L"Checkerboard - 60");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Red/Cyan) - 1");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Red/Cyan) B+W - 2");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Yellow/Blue) - 5");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Yellow/Blue) B+W - 6");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Green/Magenta) - 10");
		m_aszStereoViewOptions.push_back(L"Anaglyph (Green/Magenta) B+W - 11");
		m_aszStereoViewOptions.push_back(L"DIY Rift - 100");
		m_aszStereoViewOptions.push_back(L"Oculus Rift: Direct Mode - 111");
		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 128;
		sControl.m_sSize.cx = 420;
		sControl.m_sSize.cy = APP_SIZE_FONT + 16;
		sControl.m_sSpinControl.m_dwCurrentSelection = 0;
		sControl.m_sSpinControl.m_paszEntries = &m_aszStereoViewOptions;
		m_dwSpinStereoView = m_pcVireioGUI->AddControl(dwPage, sControl);

		// spin control - tracker
		static std::vector<std::wstring> m_aszTrackerOptions;
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		m_aszTrackerOptions.push_back(L"No Tracking - 0");
		m_aszTrackerOptions.push_back(L"Hillcrest Labs - 10");
		m_aszTrackerOptions.push_back(L"FreeTrack - 20");
		m_aszTrackerOptions.push_back(L"Shared Memory Tracker - 30");
		m_aszTrackerOptions.push_back(L"OculusTrack - 40");
		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 188;
		sControl.m_sSize.cx = 420;
		sControl.m_sSize.cy = APP_SIZE_FONT + 16;
		sControl.m_sSpinControl.m_dwCurrentSelection = 0;
		sControl.m_sSpinControl.m_paszEntries = &m_aszTrackerOptions;
		m_dwSpinTracker = m_pcVireioGUI->AddControl(dwPage, sControl);

		// spin control - monitors
		static std::vector<std::wstring> m_aszMonitorOptions;
		ZeroMemory(&sControl, sizeof(Vireio_Control));

		// start to query for monitors
		UINT32 num_of_paths = 0;
		UINT32 num_of_modes = 0;
		DISPLAYCONFIG_PATH_INFO* displayPaths = NULL;
		DISPLAYCONFIG_MODE_INFO* displayModes = NULL;
		GetDisplayConfigBufferSizes(QDC_ALL_PATHS, &num_of_paths, &num_of_modes);

		// Allocate paths and modes dynamically
		displayPaths = (DISPLAYCONFIG_PATH_INFO*)calloc((int)num_of_paths, sizeof(DISPLAYCONFIG_PATH_INFO));
		displayModes = (DISPLAYCONFIG_MODE_INFO*)calloc((int)num_of_modes, sizeof(DISPLAYCONFIG_MODE_INFO));

		// Query for the information 
		QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &num_of_paths, displayPaths, &num_of_modes, displayModes, NULL);

		UINT32 index = 0;
		int adapterNum = 0;
		while (index < num_of_modes)
		{
			if (displayModes[index].infoType == DISPLAYCONFIG_MODE_INFO_TYPE_TARGET)
			{
				DISPLAYCONFIG_TARGET_DEVICE_NAME deviceName;
				DISPLAYCONFIG_DEVICE_INFO_HEADER header;
				header.size = sizeof(DISPLAYCONFIG_TARGET_DEVICE_NAME);
				header.adapterId = displayModes[index].adapterId;
				header.id = displayModes[index].id;
				header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
				deviceName.header = header;
				DisplayConfigGetDeviceInfo((DISPLAYCONFIG_DEVICE_INFO_HEADER*)&deviceName);

				char monitorFriendlyDeviceName[256];
				ZeroMemory(monitorFriendlyDeviceName, 256);

				size_t countConverted = 0;
				wcstombs_s(&countConverted, monitorFriendlyDeviceName, 256, deviceName.monitorFriendlyDeviceName, 256);

				std::string szAdapter(monitorFriendlyDeviceName);
				std::wstring szAdapterW(szAdapter.begin(), szAdapter.end());
				if (adapterNum == 0) szAdapterW = L"Primary Monitor: " + szAdapterW + L" - 0";
				if (adapterNum == 1) szAdapterW = L"Secondary Monitor: " + szAdapterW + L" - 1";
				if (adapterNum == 2) szAdapterW = L"Tertiary Monitor: " + szAdapterW + L" - 2";
				m_aszMonitorOptions.push_back(szAdapterW);
				anMonitors.push_back(adapterNum);
				adapterNum++;
			}

			index++;
		}

		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 248;
		sControl.m_sSize.cx = 420;
		sControl.m_sSize.cy = APP_SIZE_FONT + 16;
		sControl.m_sSpinControl.m_dwCurrentSelection = 0;
		sControl.m_sSpinControl.m_paszEntries = &m_aszMonitorOptions;
		m_dwSpinMonitor = m_pcVireioGUI->AddControl(dwPage, sControl);
#else

		// spin control - game selection
		m_asVireioGameProfiles = std::vector<VireioGameProfile>();
		static std::vector<std::wstring> m_aszGames;
		ZeroMemory(&sControl, sizeof(Vireio_Control));

		// get the profiles
		bool profileFound = false;
		string profilePath = string("..\\cfg_v4\\profiles.xml");
		xml_document docProfiles;
		xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
		xml_node profile;
		xml_node gameProfile;

		if (resultProfiles.status == status_ok)
		{
			xml_node xml_profiles = docProfiles.child("profiles");

			// loop through profiles
			for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
			{
				VireioGameProfile sProfile;

				// get name
				sProfile.szGameName = profile.attribute("game_name").as_string();

				// get process
				sProfile.szGameProcess = profile.attribute("game_exe").as_string();
				sProfile.szGameProcess = StringToLower(sProfile.szGameProcess);

				// get architecture
				string szCPU = profile.attribute("cpu_architecture").as_string("32bit");

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
					// get dx version
					UINT unDX = profile.attribute("dx_version").as_uint(9);
					switch (unDX)
					{
#ifdef _WIN64
						case 9:
							sProfile.szProfileFilePath = profile.attribute("profile_path").as_string("..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX9.aqup");
							sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
							break;
						case 10:
							sProfile.eDX_Version = VireioGameProfile::DX_Version_10;
							break;
						case 11:
							// get v4 profile path
							sProfile.szProfileFilePath = profile.attribute("profile_path").as_string("..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX11.aqup");
							sProfile.eDX_Version = VireioGameProfile::DX_Version_11;
							break;
						default:
							sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
							break;
#else
						case 9:
							sProfile.szProfileFilePath = profile.attribute("profile_path").as_string("..\\game_profiles\\HMD\\x64\\Mono\\Mono_Cinema_DX9.aqup");
							sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
							break;
						case 10:
							sProfile.eDX_Version = VireioGameProfile::DX_Version_10;
							break;
						case 11:
							// get v4 profile path
							sProfile.szProfileFilePath = profile.attribute("profile_path").as_string("..\\game_profiles\\HMD\\Win32\\Mono\\Mono_Cinema_DX11.aqup");
							sProfile.eDX_Version = VireioGameProfile::DX_Version_11;
							break;
						default:
							sProfile.eDX_Version = VireioGameProfile::DX_Version_9;
							break;
#endif
					};

					// get image url
					sProfile.szGameImageUrl = profile.attribute("image_url").as_string();

					// get time delay, repetition
					sProfile.unTimedelay = profile.attribute("time_delay").as_uint(0);
					sProfile.unRepetition = profile.attribute("repetition").as_uint(0);

					// and add to list
					m_asVireioGameProfiles.push_back(sProfile);
					std::wstring szName = std::wstring(sProfile.szGameName.begin(), sProfile.szGameName.end());
					m_aszGames.push_back(szName);
				}
			}
		}

		// Game selection
		sControl.m_eControlType = Vireio_Control_Type::ListBox;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 168;
		sControl.m_sSize.cx = 400;
		sControl.m_sSize.cy = APP_SIZE_FONT * 7;
		sControl.m_sListBox.m_bSelectable = true;
		sControl.m_sListBox.m_nCurrentSelection = unProfileIndex;
		sControl.m_sListBox.m_paszEntries = &m_aszGames;
		m_dwListGameProfiles = m_pcVireioGUI->AddControl(dwPage, sControl);

		// spin control - HMD selection
		static std::vector<std::wstring> m_aszHMD_Options;
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		m_aszHMD_Options.push_back(L"No HMD");
		m_aszHMD_Options.push_back(L"LibOVR");
		m_aszHMD_Options.push_back(L"OpenVR");
		m_aszHMD_Options.push_back(L"OSVR");
		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = 16;
		sControl.m_sPosition.y = 128;
		sControl.m_sSize.cx = 100;
		sControl.m_sSize.cy = APP_SIZE_FONT + 12;
		sControl.m_sSpinControl.m_dwCurrentSelection = unHMDIndex;
		sControl.m_sSpinControl.m_paszEntries = &m_aszHMD_Options;
		m_dwSpinHMD = m_pcVireioGUI->AddControl(dwPage, sControl);
#endif

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
		UINT dwVersion = m_pcVireioGUI->AddControl(dwPage, sControl);

		std::string date(__DATE__);
		std::string buildDate = date.substr(4, 2) + "-" + date.substr(0, 3) + "-" + date.substr(7, 4);

		// and add all entries
		std::wstringstream szStream = std::wstringstream();
		szStream << L"v" << APP_VERSION << " " << buildDate.c_str();
#ifdef _WIN64
		szStream << L" 64bit";
#else
		szStream << L" 32bit";
#endif
		m_pcVireioGUI->AddEntry(dwVersion, szStream.str().c_str());

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
		sControl.m_sSize.cx = 184 + 12; /**< +12 == border **/
		sControl.m_sSize.cy = 69 + 12; /**< +12 == border **/
		sControl.m_sButton.m_pszText = &szLoadProfile;
		m_dwLoadAquilinusProfile = m_pcVireioGUI->AddControl(dwPage, sControl);
#endif

		// create the 'x'(=exit) button
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		static std::wstring szX = std::wstring(L"x");
		sControl.m_eControlType = Vireio_Control_Type::Button;
		sControl.m_sPosition.x = APP_SIZE_WIDTH - 18;
		sControl.m_sPosition.y = 85;
		sControl.m_sSize.cx = 30;
		sControl.m_sSize.cy = 30;
		sControl.m_sButton.m_pszText = &szX;
		m_dwExit = m_pcVireioGUI->AddControl(dwPage, sControl);

		// load the logo
		logo_bitmap = (HBITMAP)LoadImage(NULL, "..//img//logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		OutputDebugString("Load the bitmap\n");

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowWindow(window_handle, SW_SHOW);
		UpdateWindow(window_handle);

#ifndef _VIREIO_3
		// download game jpg files from steam if not present.. only for v4
		std::vector<std::string> aszImageURLs;
		std::vector<std::string> aszImageFilePaths;
		aszImageURLs.push_back("https://steamcdn-a.akamaihd.net/steamcommunity/public/images/apps/377160/8977a8e98acbbdd3c0ff905afb7e0a6e2eb555ea.jpg");
		aszImageFilePaths.push_back("..//img//Fallout4.jpg");
		game_bitmap = nullptr;

		for (size_t nI = 0; nI < aszImageURLs.size(); nI++)
		{
			if (!PathFileExists(aszImageFilePaths[nI].c_str()))
			{
				HRESULT hr = URLDownloadToFile(NULL, aszImageURLs[nI].c_str(), aszImageFilePaths[nI].c_str(), 0, NULL);
				if (FAILED(hr))
				{
					OutputDebugString("Failed to download STEAM game image file !");
				}
			}
		}
#endif

		// Refresh on timer for FPS readings
		SetTimer(window_handle, 1, 500, NULL);
	}
	~Vireio_Perception_Main_Window() {
		UnregisterClass(window_class_name, instance_handle);
	}

	/*** Vireio_Perception_Main_Window public members ***/
	static LRESULT WINAPI main_window_proc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
	void                  run() { MSG window_message; while (GetMessage(&window_message, NULL, 0, 0)) { TranslateMessage(&window_message); DispatchMessage(&window_message); } }
};

/*** Vireio_Perception_Main_Window static fields ***/
POINT       Vireio_Perception_Main_Window::m_ptMouseCursor;
HBITMAP     Vireio_Perception_Main_Window::logo_bitmap;
HBITMAP     Vireio_Perception_Main_Window::game_bitmap;
Vireio_GUI* Vireio_Perception_Main_Window::m_pcVireioGUI;
UINT        Vireio_Perception_Main_Window::m_dwListGameProfiles;
UINT        Vireio_Perception_Main_Window::m_dwSpinHMD;
UINT        Vireio_Perception_Main_Window::m_dwSpinStereoView;
UINT        Vireio_Perception_Main_Window::m_dwSpinTracker;
UINT        Vireio_Perception_Main_Window::m_dwSpinMonitor;
UINT        Vireio_Perception_Main_Window::m_dwLoadAquilinusProfile;
UINT        Vireio_Perception_Main_Window::m_dwExit;

/**
* Main window procedure.
***/
LRESULT WINAPI Vireio_Perception_Main_Window::main_window_proc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	// call vireio gui class for all mouse actions
	if ((message == WM_LBUTTONDOWN) || (message == WM_LBUTTONUP) || (message == WM_MOUSEMOVE))
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
				if (sEvent.dwIndexOfControl == m_dwListGameProfiles)
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
				if (sEvent.dwIndexOfControl == m_dwSpinHMD)
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
				if (sEvent.dwIndexOfControl == m_dwExit)
					SendMessage(window_handle, WM_CLOSE, 0, 0);
#ifndef _VIREIO_3
				else if (sEvent.dwIndexOfControl == m_dwLoadAquilinusProfile)
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

				if (m_ptMouseCursor.y < 94)
				{
					// get the difference to the old position set only in WM_LBUTTONDOWN
					nX -= m_ptMouseCursor.x;
					nY -= m_ptMouseCursor.y;

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
							   m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lparam);
							   m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lparam);
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
										 UINT unSelectionGame = (UINT)m_pcVireioGUI->GetCurrentSelection(m_dwListGameProfiles);
										 if (unSelectionGame >= (UINT)m_asVireioGameProfiles.size()) unSelectionGame = 0;
										 UINT unSelectionHMD = (UINT)m_pcVireioGUI->GetCurrentSelection(m_dwSpinHMD);
										 if (unSelectionHMD >= 4) unSelectionHMD = 0;
										 OutputDebugString(m_asVireioGameProfiles[unSelectionGame].szGameName.c_str());

										 // get profile path and set HMD string
										 std::wstring szProfilePath = std::wstring(m_asVireioGameProfiles[unSelectionGame].szProfileFilePath.begin(), m_asVireioGameProfiles[unSelectionGame].szProfileFilePath.end());
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
											 case 3:
												 szProfilePath = szProfilePath.substr(0, szProfilePath.find(L"HMD")) + L"OSVR" + szProfilePath.substr(szProfilePath.find(L"HMD") + 3);
												 break;
										 }
										 OutputDebugStringW(szProfilePath.c_str());

										 // get process
										 std::wstring szProcess = std::wstring(m_asVireioGameProfiles[unSelectionGame].szGameProcess.begin(), m_asVireioGameProfiles[unSelectionGame].szGameProcess.end());

										 // load aquilinus profile
										 LPCWSTR szP;
										 g_pAquilinus_LoadProfile(szProfilePath.c_str(), szProcess.c_str(), &szP, m_asVireioGameProfiles[unSelectionGame].unRepetition, m_asVireioGameProfiles[unSelectionGame].unTimedelay);
										 std::wstring szPath = std::wstring(szP);

										 // load game logo... TODO !! LOAD BY PROFILE !!
										 std::wstring szFilename = szPath.substr(szPath.find_last_of('\\') + 1);
										 if (szFilename.find(L"Fallout4") == 0)
										 {
											 ConvertJpgFile("..//img//Fallout4.jpg", "..//img//game_logo.bmp");
											 game_bitmap = (HBITMAP)LoadImage(NULL, "..//img//game_logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
										 }
									 }
									 else
										 g_pAquilinus_Reinject();

									 g_bLoadAquilinusProfile = false;
								 }
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
						 SelectObject(hdcMem, logo_bitmap);
						 GetObject(logo_bitmap, sizeof(bitmap), &bitmap);
						 BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
						 DeleteDC(hdcMem);

#ifndef _VIREIO_3
						 // draw profile button + close icon
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
						 SetDCBrushColor(hdc, APP_BUTTON_PENCOLOR);
						 Rectangle(hdc, APP_SIZE_WIDTH - 12, bitmap.bmHeight + 7, APP_SIZE_WIDTH - 7, bitmap.bmHeight + 12);

						 // draw game logo
						 if (game_bitmap)
						 {
							 hdcMem = CreateCompatibleDC(hdc);
							 SelectObject(hdcMem, game_bitmap);
							 GetObject(game_bitmap, sizeof(bitmap), &bitmap);
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

						 */
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
	return DefWindowProc(window_handle, message, wparam, lparam);
};

/**
* Main windows entry point
***/
int WINAPI wWinMain(HINSTANCE instance_handle, HINSTANCE, LPWSTR, INT) {

	// avoid double driver window
	HWND window = FindWindow("perception", "Vireio Perception");
	if (window != 0)
	{
		OutputDebugString("Vireio Perception is already present !");
		return 0;
	}

#ifdef _VIREIO_3
	InitConfig();
	InitModes();
	InstallHook();
#else
	// load Aquilinus Runtime Environment, init driver
	if (LoadAquilinusRTE()) OutputDebugString("Vireio Perception : Using Aquilinus Runtime Environment.");
#endif

	// create window entries
	Vireio_Perception_Main_Window main_window("perception");

#ifdef _VIREIO_3
	// load the user configuration
	ProxyHelper helper = ProxyHelper();
	ProxyHelper::UserConfig userConfig;
	helper.LoadUserConfig(userConfig);
#endif

	// run main window
	main_window.run();

#ifdef _VIREIO_3
	// remove hook, exit
	RemoveHook();
#endif

	return 0;
}

#ifdef _VIREIO_3
/**
* Init registry configuration.
***/
bool InitConfig(void)
{
	std::string basePath = getCurrentPath();
	OutputDebugString("Current Path: ");
	OutputDebugString(basePath.c_str());
	OutputDebugString("\n");

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG createRes = RegCreateKeyEx(HKEY_CURRENT_USER, sk, 0, NULL, 0, 0, NULL, &hKey, NULL);

	if (createRes == ERROR_SUCCESS)
	{
		OutputDebugString("Success creating Registry.\n");
	}
	else
	{
		OutputDebugString("Error creating Registry.\n");
	}

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS, &hKey);

	if (openRes == ERROR_SUCCESS)
	{
		OutputDebugString("Success opening key.\n");
	}
	else
	{
		OutputDebugString("Error opening key.\n");
	}

	LPCTSTR value = TEXT("BasePath");
	LPCTSTR data = basePath.c_str();

	LONG setRes = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)data, (int)strlen(data) + 1);

	if (setRes == ERROR_SUCCESS)
	{
		OutputDebugString("Success writing to Registry.\n");
	}
	else
	{
		OutputDebugString("Error writing to Registry.\n");
	}

	LONG closeOut = RegCloseKey(hKey);

	if (closeOut == ERROR_SUCCESS)
	{
		OutputDebugString("Success closing key.\n");
	}
	else
	{
		OutputDebugString("Error closing key.\n");
	}

	return true;
}

/**
* Creates simple vectors to easily set the Vireio proxy config.
***/
bool InitModes()
{
	anStereoModes.push_back(0);
	anStereoModes.push_back(20);
	anStereoModes.push_back(30);
	anStereoModes.push_back(40);
	anStereoModes.push_back(50);
	anStereoModes.push_back(60);
	anStereoModes.push_back(1);
	anStereoModes.push_back(2);
	anStereoModes.push_back(5);
	anStereoModes.push_back(6);
	anStereoModes.push_back(10);
	anStereoModes.push_back(11);
	anStereoModes.push_back(100);
	anStereoModes.push_back(111);

	anTrackerModes.push_back(0);
	anTrackerModes.push_back(10);
	anTrackerModes.push_back(20);
	anTrackerModes.push_back(30);
	anTrackerModes.push_back(40);

	return true;
}

#else
/**
* Loads the Aquilinus runtime environment, if present.
* @returns True if the Aquilinus runtime environment is present and loaded.
***/
bool LoadAquilinusRTE()
{
	// explicit Aquilinus Runtime Environment dll import
#ifdef _WIN64
	g_hmAquilinusRTE = LoadLibrary("AquilinusRTE_x64.dll");
#else
	g_hmAquilinusRTE = LoadLibraryW(L"AquilinusRTE_Win32.dll");
#endif

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
			OutputDebugString("Aquilinus Runtime Environment method(s) not found !");
			FreeLibrary(g_hmAquilinusRTE);
			g_hmAquilinusRTE = NULL;
			return false;
		}
		else
			g_pAquilinus_Init();
	}
	else
	{
		OutputDebugString("Aquilinus Runtime not found !");
		return false;
	}

	return true;
}
#endif

/**
* Returns the path of Vireio Perception.
***/
std::string getCurrentPath(void)
{
	TCHAR fullPath[1024];
	int len = 1024;
	int bytes = GetModuleFileName(NULL, fullPath, len);
	if (bytes == 0)
	{
		OutputDebugString("GetModuleFileName FAILS !");
		return false;
	}

	std::string pathName = std::string(fullPath);
#ifdef _WIN64
	std::string exeName = "bin\\Perception_x64.exe";
#else
	std::string exeName = "bin\\Perception_Win32.exe";
#endif
	std::string basePath = "";
	size_t found = pathName.find(exeName);

	if (found != std::string::npos)
	{
		basePath = pathName.substr(0, int(found));
	}

	return basePath;
}