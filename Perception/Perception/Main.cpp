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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
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

#define APP_SIZE_WIDTH 800
#define APP_SIZE_HEIGHT 360
#define APP_SIZE_FONT 32
#define APP_COLOR_FONT RGB(180, 120, 60)
#define APP_COLOR_BACK RGB(40, 40, 100)
#define APP_FONT L"Candara"
#define APP_FONT_ITALIC TRUE

#define DEBUG_UINT_A(a) { char buf[128]; wsprintf(buf, "%u", a); OutputDebugString(buf); }

using namespace std;

bool InitConfig(void);
bool InitModes(void);
std::string getCurrentPath(void);
bool LoadAquilinusRTE();
void LoadDLL(void);
HINSTANCE hD3D9;
ProxyHelper::OculusProfile oculusProfile;

/*** Aquilinus Runtime Environment type definitions ***/
typedef void (WINAPI *AQUILINUS_Init)();
typedef void (WINAPI *AQUILINUS_Close)();
typedef void (WINAPI *AQUILINUS_ForceIdle)();
typedef void (WINAPI *AQUILINUS_LoadProfile)();

/*** Aquilinus Runtime Environment methods ***/
AQUILINUS_Init g_pAquilinus_Init;
AQUILINUS_Close g_pAquilinus_Close;
AQUILINUS_ForceIdle g_pAquilinus_ForceIdle;
AQUILINUS_LoadProfile g_pAquilinus_LoadProfile;

/**
* Handle to the Aquilinus Runtime Environment.
***/
HMODULE g_hmAquilinusRTE = NULL;
/**
* True if Aquilinus profile to be loaded.
***/
bool g_bLoadAquilinusProfile = false;
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
	static POINT       m_ptMouseCursor;            /**< The current mouse cursor. **/
	static Vireio_GUI* m_pcVireioGUI;              /**< Vireio Graphical User Interface class. **/
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

		// load proxy config
		ProxyHelper helper = ProxyHelper();
		ProxyConfig cfg;
		helper.LoadUserConfig(cfg, oculusProfile);

		// create the vireio gui
		SIZE sSizeOfThis;
		sSizeOfThis.cx = APP_SIZE_WIDTH; sSizeOfThis.cy = APP_SIZE_HEIGHT;
		m_pcVireioGUI = new Vireio_GUI(sSizeOfThis, APP_FONT, APP_FONT_ITALIC, APP_SIZE_FONT, APP_COLOR_FONT, APP_COLOR_BACK);
		UINT dwPage = m_pcVireioGUI->AddPage();

		// control structure
		Vireio_Control sControl;

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

		// create the main entries
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		static std::vector<std::wstring> sEntriesVersion;
		sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
		sControl.m_sPosition.x = 450;
		sControl.m_sPosition.y = 300;
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
		m_pcVireioGUI->AddEntry(dwVersion, szStream.str().c_str());

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
		szStream << "Oculus Profile : " << oculusProfile.Name.c_str();
		m_pcVireioGUI->AddEntry(dwOculus, szStream.str().c_str());

		// create the 'Load Aquilinus Profile' button
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		static std::wstring szLoadProfile = std::wstring(L"Vireio Profile");
		sControl.m_eControlType = Vireio_Control_Type::Button;
		sControl.m_sPosition.x = 452;
		sControl.m_sPosition.y = 128;
		sControl.m_sSize.cx = 184 + 8; /**< +8 == border **/
		sControl.m_sSize.cy = 69 + 8; /**< +8 == border **/
		sControl.m_sButton.m_pszText = &szLoadProfile;
		m_dwLoadAquilinusProfile = m_pcVireioGUI->AddControl(dwPage, sControl);

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

		//Refresh on timer for FPS readings
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
Vireio_GUI* Vireio_Perception_Main_Window::m_pcVireioGUI;
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
				break;
			case Pressed:
				if (sEvent.dwIndexOfControl == m_dwExit)
					SendMessage(window_handle, WM_CLOSE, 0, 0);
				else if (sEvent.dwIndexOfControl == m_dwLoadAquilinusProfile)
					g_bLoadAquilinusProfile = true;
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

				// get the difference to the old position set only in WM_LBUTTONDOWN
				nX -= m_ptMouseCursor.x;
				nY -= m_ptMouseCursor.y;

				// get the old window position
				RECT rcWnd;
				GetWindowRect(window_handle, &rcWnd);

				// set the new window position
				SetWindowPos(window_handle, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
			}
			else
			{
				// get the mouse cursor
				m_ptMouseCursor.x = (LONG)GET_X_LPARAM(lparam);
				m_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lparam);
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
							   break;
		}
		case WM_LBUTTONUP:
		{
							 if (g_hmAquilinusRTE)
							 {
								 if (g_bLoadAquilinusProfile)
								 {
									 OutputDebugString("Vireio Perception: Load Aquilinus Profile!");

									 // load aquilinus profile
									 g_pAquilinus_LoadProfile();
									 g_bLoadAquilinusProfile = false;
								 }
							 }
							 break;
		}
		case WM_PAINT:
		{
						 PAINTSTRUCT 	ps;
						 HDC 			hdc;
						 BITMAP 		bitmap;
						 HDC 			hdcMem;

						 hdc = BeginPaint(window_handle, &ps);

						 hdcMem = CreateCompatibleDC(hdc);
						 HBITMAP hGUI = m_pcVireioGUI->GetGUI(true, false, true, true);
						 SelectObject(hdcMem, hGUI);

						 GetObject(hGUI, sizeof(bitmap), &bitmap);
						 BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

						 SelectObject(hdcMem, logo_bitmap);

						 GetObject(logo_bitmap, sizeof(bitmap), &bitmap);
						 BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
						 DeleteDC(hdcMem);

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
						 if (g_hmAquilinusRTE)
						 {
							 g_pAquilinus_Close();
							 FreeLibrary(g_hmAquilinusRTE);
						 }
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

	// load Aquilinus Runtime Environment, init driver
	if (LoadAquilinusRTE()) OutputDebugString("Vireio Perception : Using Aquilinus Runtime Environment.");
	InitConfig();
	InitModes();
#ifndef _WIN64
	InstallHook();
#endif

	// create window entries
	Vireio_Perception_Main_Window main_window("perception");

	// load the user configuration
	ProxyHelper helper = ProxyHelper();
	ProxyHelper::UserConfig userConfig;
	helper.LoadUserConfig(userConfig);

	// run main window
	main_window.run();

#ifndef _WIN64
	// remove hook, exit
	RemoveHook();
#endif

	return 0;
}

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
	g_hmAquilinusRTE = LoadLibrary("AquilinusRTE_Win32.dll");
#endif

	// get Aquilinus Runtime Environment methods
	if (g_hmAquilinusRTE != NULL)
	{
		// get methods explicit
		g_pAquilinus_Init = (AQUILINUS_Init)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_Init");
		g_pAquilinus_Close = (AQUILINUS_Close)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_Close");
		g_pAquilinus_ForceIdle = (AQUILINUS_ForceIdle)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_ForceIdle");
		g_pAquilinus_LoadProfile = (AQUILINUS_LoadProfile)GetProcAddress(g_hmAquilinusRTE, "Aquilinus_LoadProfile");

		if ((!g_pAquilinus_Init) || (!g_pAquilinus_Close) || (!g_pAquilinus_ForceIdle) || (!g_pAquilinus_LoadProfile))
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