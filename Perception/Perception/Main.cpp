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
#include <string>
#include "hijackdll.h"
#include <CommCtrl.h> 
#include <ctime>  
#include <cstdlib>  
#include "ProxyHelper.h"
#include <map>
#include "Resource.h"
#include "Version.h"


using namespace std;

bool InitConfig(void);
bool InitModes(void);
std::string getCurrentPath(void);
void LoadDLL(void);
HINSTANCE hD3D9;
ProxyHelper::OculusProfile oculusProfile;

class static_control;

class combobox_control {
private:
	HINSTANCE instance_handle;
	char * window_class_name;
	HWND parent_handle;
	static_control * text;
public:
	HWND combobox_handle;
	combobox_control(HINSTANCE instance, HWND parent, static_control * text_area, int cx, int cy, int menuId) 
		: instance_handle(instance), parent_handle(parent), text(text_area) {
			combobox_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "Combobox", NULL,
				WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL |
				CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | 
				CBS_HASSTRINGS | CBS_SORT, cx, cy, 250, 200, // 20, 81, 250, 200, 
				parent_handle, (HMENU)menuId, instance_handle, NULL);
			SetWindowLongPtr(combobox_handle, GWL_USERDATA, (LONG)this);
	}
	static LRESULT combobox_window_proc(HWND hwnd, UINT message,    
		WPARAM wparam, LPARAM lparam) {
			combobox_control * This = (combobox_control *)GetWindowLongPtr(hwnd, GWL_USERDATA);
			return DefWindowProc(hwnd, message, wparam, lparam);
	}
	void add_item(char * string) {
		int index = (int)SendMessage(combobox_handle, CB_INSERTSTRING, -1, (LPARAM)string);
		if ( index == 0 ) {
			SendMessage(combobox_handle, CB_SETCURSEL, index, 0);
		}
	}
	void measure_item(LPMEASUREITEMSTRUCT item) {
		item->itemHeight = 20;
		item->itemWidth = 200;
	}
	void draw_item(LPDRAWITEMSTRUCT item) {
		char string[120];
		if ( item->itemID == -1 ) return;
		SendMessage(combobox_handle, CB_GETLBTEXT, item->itemID, (LPARAM)string);
		int length = 0;
		char * s = string;
		while ( *s++ != '\t' ) length++;
		COLORREF back_colour;
		if ( item->itemState & ODS_SELECTED ) {
			back_colour = RGB(25, 25, 25);
			HBRUSH back_brush = CreateSolidBrush(back_colour);
			FillRect(item->hDC, &item->rcItem, back_brush);
			DeleteObject(back_brush);
			SetBkColor(item->hDC, back_colour);
			SetTextColor(item->hDC, RGB(255, 255, 255));
		}
		else {
			back_colour = RGB(255, 255, 255);
			HBRUSH back_brush = CreateSolidBrush(back_colour);
			FillRect(item->hDC, &item->rcItem, back_brush);
			DeleteObject(back_brush);
			SetBkColor(item->hDC, back_colour);
			SetTextColor(item->hDC, RGB(0, 0, 0));
			}      
		
		HFONT font = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "Calibri");
		HFONT oldFont = (HFONT)SelectObject(item->hDC, font);
		DrawText(item->hDC, string, length, &item->rcItem, DT_LEFT|DT_SINGLELINE);
		DeleteObject(SelectObject(item->hDC, oldFont));

	}

	int get_selection()
	{
		char string[120];
		int selection = (int)SendMessage(combobox_handle, CB_GETCURSEL, 0, 0);
		if ( selection != CB_ERR ) {
			SendMessage(combobox_handle, CB_GETLBTEXT, selection, (LPARAM)string);
			int length = 0;
			char * s = string;
			while ( *s++ != '\t' ) length++;
			return atoi(s);
		}
	}

	void new_selection() {
		char string[120];
		int selection = (int)SendMessage(combobox_handle, CB_GETCURSEL, 0, 0);
		if ( selection != CB_ERR ) {
			SendMessage(combobox_handle, CB_GETLBTEXT, selection, (LPARAM)string);
			int length = 0;
			char * s = string;
			while ( *s++ != '\t' ) length++;
			//text->set_text(s);
			// save the stereo mode to xml file
			ProxyHelper helper = ProxyHelper();
			int mode = atoi(s);
			float aspect = mode == 25 ? 0.5f : 1.0f;
			helper.SaveUserConfig(mode, aspect);
		}
	}
	void new_selection2() {
		char string[120];
		int selection = (int)SendMessage(combobox_handle, CB_GETCURSEL, 0, 0);
		if ( selection != CB_ERR ) {
			SendMessage(combobox_handle, CB_GETLBTEXT, selection, (LPARAM)string);
			int length = 0;
			char * s = string;
			while ( *s++ != '\t' ) length++;
			// save the tracker mode to xml file
			ProxyHelper helper = ProxyHelper();
			int mode = atoi(s);
			helper.SaveTrackerMode(mode);
		}
	}

	void new_selection3() {
		int selection = (int)SendMessage(combobox_handle, CB_GETCURSEL, 0, 0);
		if ( selection != CB_ERR ) {
			// save the adapter to xml file
			ProxyHelper helper = ProxyHelper();
			helper.SaveDisplayAdapter(selection);
		}
	}
};

class static_control {
private:
	HINSTANCE instance_handle;
	wchar_t * window_class_name;
	HWND parent_handle;
	HWND static_handle;
public:
	static_control(HINSTANCE instance, HWND parent) 
		: instance_handle(instance), parent_handle(parent) {
			static_handle = CreateWindowEx(0, "Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
				280, 164, 200, 22, 
				parent_handle, NULL, instance_handle, NULL);
			SetWindowLongPtr(static_handle, GWL_USERDATA, (LONG)this);
	}
	static LRESULT combobox_window_proc(HWND hwnd, UINT message,    
		WPARAM wparam, LPARAM lparam) {
			static_control * This = (static_control *)GetWindowLongPtr(hwnd, GWL_USERDATA);
			return DefWindowProc(hwnd, message, wparam, lparam);
	}
	void set_text(char * string) {
		SetWindowText(static_handle, string);
	}
};

class frame_window {
private:
	LPCSTR window_class_name;  
	HINSTANCE instance_handle;  
	HCURSOR cursor_arrow;  
	HWND window_handle;
	HWND header_handle;
	RECT client_rectangle;
	static_control * text;
	HBITMAP logo_bitmap;
public:
	combobox_control* combobox;
	combobox_control* combobox2;
	combobox_control* combobox3;
	frame_window(LPCSTR window_class_identity) : window_class_name(window_class_identity) {         
		int screen_width = GetSystemMetrics(SM_CXFULLSCREEN);  
		int screen_height = GetSystemMetrics(SM_CYFULLSCREEN);  
		instance_handle = GetModuleHandle(NULL);  

		WNDCLASS window_class = { CS_OWNDC, main_window_proc, 0, 0,    
			instance_handle, NULL,    
			NULL, NULL, NULL,    
			window_class_name };   

		window_class.hIcon = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_ICON_BIG));

		RegisterClass(&window_class);   
		window_handle = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_TOPMOST,    
			window_class_name,    
			"Vireio Perception", 
			WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), 
			//WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,   
			(screen_width-585)/2, (screen_height-240)/2, 
			585, 270,
			NULL, NULL, instance_handle, NULL);
		SetWindowLongPtr(window_handle, GWL_USERDATA, (LONG)this);  
		GetClientRect(window_handle, &client_rectangle);
		int width = client_rectangle.right - client_rectangle.left;
		int height = client_rectangle.bottom - client_rectangle.top;
		text = new static_control(instance_handle, window_handle);
		combobox = new combobox_control(instance_handle, window_handle, text, 20, 81, 1001);
		combobox2 = new combobox_control(instance_handle, window_handle, text, 20, 115, 1002);
		combobox3 = new combobox_control(instance_handle, window_handle, text, 20, 149, 1003);

		ProxyHelper helper = ProxyHelper();
		std::string viewPath = helper.GetPath("img\\logo.bmp");

		logo_bitmap = (HBITMAP)LoadImage(NULL,viewPath.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		OutputDebugString("Load the bitmap\n");

		ProxyConfig cfg;
		helper.LoadUserConfig(cfg, oculusProfile);

		SetCursor(LoadCursor(NULL, IDC_ARROW)); 
		ShowWindow(window_handle, SW_SHOW);   
		UpdateWindow(window_handle); 

		//Refresh on timer for FPS readings
		SetTimer(window_handle, 1, 500, NULL);
	}  
	~frame_window() {  
		UnregisterClass(window_class_name, instance_handle);   
	}  

	static LRESULT WINAPI main_window_proc(HWND window_handle, UINT message,    
		WPARAM wparam, LPARAM lparam) {  
			frame_window *This = (frame_window *)GetWindowLongPtr(window_handle, GWL_USERDATA); 

			switch ( message ) {  
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
					break;
				}
			case WM_PAINT:   
				{   
					PAINTSTRUCT paint_structure;   
					RECT client_rect;   
					HDC paint_device_context, paint_dc;   
					HBITMAP bitmap;   

					paint_device_context = BeginPaint(window_handle, &paint_structure);   
					paint_dc = CreateCompatibleDC(paint_device_context);   

					HFONT font = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH, "Calibri");
					HFONT oldFont = (HFONT)SelectObject(paint_device_context, font);

					GetClientRect(window_handle, &client_rect);   
					int window_width = client_rect.right - client_rect.left;  
					int window_height = client_rect.bottom - client_rect.top;  
					bitmap = CreateBitmap(window_width, window_height, 1, 32, NULL);  
					HGDIOBJ old_bitmap = SelectObject(paint_dc, bitmap);  

					// Fill the client aread with the user selected face colour  
					HBRUSH light_brush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));  
					FillRect(paint_dc, &paint_structure.rcPaint, light_brush);  
					DeleteObject(light_brush);
					BitBlt(paint_device_context, 0, 0,    
						client_rect.right-client_rect.left,   
						client_rect.bottom-client_rect.top,   
						paint_dc, 0, 0, SRCCOPY);   
					SelectObject(paint_dc, old_bitmap);   
					DeleteObject(bitmap);   

					SelectObject(paint_dc, This->logo_bitmap);   
					//BitBlt(paint_device_context,0,0,600,140,paint_dc,0,0,SRCCOPY);
					StretchBlt(paint_device_context,0,0,565,68,paint_dc,0,0,470, 68, SRCCOPY);

					std::string date(__DATE__);
					std::string buildDate = date.substr(4, 2) + "-" + date.substr(0, 3) + "-" + date.substr(7, 4);

					// Output user profile data
					SetBkMode(paint_device_context, TRANSPARENT);
					TextOut (paint_device_context,290,81,"Version:",8);
					TextOut (paint_device_context,420,81,std::string(APP_VERSION).c_str(),std::string(APP_VERSION).size());
					TextOut (paint_device_context,290,115,"Build Date:",11);
					TextOut (paint_device_context,420,115,buildDate.c_str(),buildDate.size());
					TextOut (paint_device_context,290,149,"Oculus Profile:",15);
					TextOut (paint_device_context,420,149,oculusProfile.Name.c_str(),oculusProfile.Name.size());
					TextOut (paint_device_context,290,183,"Current FPS:",12);
					//Now get FPS from the registry
					HKEY hKey;
					LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Vireio\\Perception", 0, KEY_ALL_ACCESS, &hKey);
					if (openRes==ERROR_SUCCESS)
					{
						char fpsBuffer[10];
						memset(fpsBuffer, 0, 10);
						DWORD dwDataSize = 10;
						LONG lResult = RegGetValue(hKey, NULL, "FPS", RRF_RT_REG_SZ, NULL, (LPVOID)&fpsBuffer, &dwDataSize);
						if (lResult == ERROR_SUCCESS)
						{
							TextOut (paint_device_context,420,183,fpsBuffer, strlen(fpsBuffer));
							//Now delete, if this isn't refreshed, then we'll report blank next time round
							RegDeleteValue(hKey, "FPS");
							RegCloseKey(hKey);
						}
						else
							TextOut (paint_device_context,420,183,"--", 2);
					}


					DeleteObject(SelectObject(paint_device_context, oldFont));
					DeleteDC(paint_dc);   
					EndPaint(window_handle, &paint_structure);   
					return 0;   
				}   
			case WM_ERASEBKGND:   
				{   
					return TRUE;   
				}
			case WM_SIZE:   
				{
					InvalidateRect(window_handle, NULL, TRUE);   
					return 0;   
				}
			case WM_MEASUREITEM:
				{
					This->combobox->measure_item((LPMEASUREITEMSTRUCT)lparam);
					This->combobox2->measure_item((LPMEASUREITEMSTRUCT)lparam);
					This->combobox3->measure_item((LPMEASUREITEMSTRUCT)lparam);
					return TRUE;
				}
			case WM_DRAWITEM:
				{
					LPDRAWITEMSTRUCT Item = (LPDRAWITEMSTRUCT)lparam;
					if (Item->CtlID == 1001)
					{
						This->combobox->draw_item((LPDRAWITEMSTRUCT)lparam);
					} 
					else if (Item->CtlID == 1002)
					{
						This->combobox2->draw_item((LPDRAWITEMSTRUCT)lparam);
					}
					else if (Item->CtlID == 1003)
					{
						This->combobox3->draw_item((LPDRAWITEMSTRUCT)lparam);
					}
					return TRUE;
				}
			case WM_COMMAND:
				{
					if ( HIWORD(wparam) == CBN_SELCHANGE ) {
						This->combobox->new_selection();
						This->combobox2->new_selection2();
						This->combobox3->new_selection3();
					}

					//Disable other controls if selection is direct-to-rift
/*					if (This->combobox->get_selection() == 111)
					{
						ShowWindow(This->combobox2->combobox_handle, SW_HIDE);
						ShowWindow(This->combobox3->combobox_handle, SW_HIDE);
					}
					else
					{
						ShowWindow(This->combobox2->combobox_handle, SW_SHOW);
						ShowWindow(This->combobox3->combobox_handle, SW_SHOW);
					}*/

					return 0;
				}
			case WM_CLOSE:   
				{   
					PostQuitMessage(0);   
					return 0;   
				}   
			}   
			return DefWindowProc(window_handle, message, wparam, lparam);   
	}

	void run() {  
		MSG window_message;   
		while ( GetMessage(&window_message, NULL, 0, 0) ) {   
			TranslateMessage(&window_message);   
			DispatchMessage(&window_message);   
		}   
	} 

	void add_item(char * string) {
		combobox->add_item(string);
	}

	void add_item2(char * string) {
		combobox2->add_item(string);
	}

	void add_item3(const char * string) {
		combobox3->add_item(const_cast<char*>(string));
	}
};  

map<int, int> stereoModes;
map<int, int> trackerModes;

int WINAPI wWinMain(HINSTANCE instance_handle, HINSTANCE, LPWSTR, INT) {  

	// avoid double driver window
	HWND window = FindWindow( "perception", "Vireio Perception");
    if( window != 0 )
    {
		OutputDebugString("Vireio Perception is already present !");
		return 0;
    }

	InitConfig();
	InitModes();
	InstallHook();


	frame_window main_window("perception");
	main_window.add_item("Disabled\t0");
	main_window.add_item("Side by Side\t20");
	main_window.add_item("Over Under\t30");
	main_window.add_item("Horizontal Interleave\t40");
	main_window.add_item("Vertical Interleave\t50");
	main_window.add_item("Checkerboard\t60");
	main_window.add_item("Anaglyph (Red/Cyan)\t1");
	main_window.add_item("Anaglyph (Red/Cyan) B+W\t2");
	main_window.add_item("Anaglyph (Yellow/Blue)\t5");
	main_window.add_item("Anaglyph (Yellow/Blue) B+W\t6");
	main_window.add_item("Anaglyph (Green/Magenta)\t10");
	main_window.add_item("Anaglyph (Green/Magenta) B+W\t11");
	main_window.add_item("DIY Rift\t100");
	main_window.add_item("Oculus Rift: Extended Mode\t110");
	main_window.add_item("Oculus Rift: Direct-to-HMD\t111");

	main_window.add_item2("No Tracking\t0");
	main_window.add_item2("Hillcrest Labs\t10");
	main_window.add_item2("FreeTrack\t20");
	main_window.add_item2("Shared Memory Tracker\t30");
	main_window.add_item2("OculusTrack\t40");


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
			DisplayConfigGetDeviceInfo( (DISPLAYCONFIG_DEVICE_INFO_HEADER*) &deviceName );

			char monitorFriendlyDeviceName[256];
			ZeroMemory(monitorFriendlyDeviceName, 256);

			size_t countConverted = 0;
			wcstombs_s(&countConverted, monitorFriendlyDeviceName, 256, deviceName.monitorFriendlyDeviceName, 256);

			std::string adapterStr(monitorFriendlyDeviceName);
			if (adapterNum == 0) adapterStr = "Primary Monitor: " + adapterStr + "\t0";
			if (adapterNum == 1) adapterStr = "Secondary Monitor: " + adapterStr + "\t1";
			if (adapterNum == 2) adapterStr = "Tertiary Monitor: " + adapterStr + "\t2";
			main_window.add_item3(adapterStr.c_str());
			adapterNum++;
		}

		index++;
	}

	ProxyHelper helper = ProxyHelper();
	ProxyHelper::UserConfig userConfig;
	helper.LoadUserConfig(userConfig);

	SendMessage(main_window.combobox->combobox_handle, CB_SETCURSEL, stereoModes[userConfig.mode], 0);
	SendMessage(main_window.combobox2->combobox_handle, CB_SETCURSEL, trackerModes[userConfig.mode2], 0);

	//If an HMD is unplugged we may not actually be able to select it
	if (userConfig.adapter >= adapterNum)
	{
		userConfig.adapter = 0;
		helper.SaveDisplayAdapter(0);
	}
	SendMessage(main_window.combobox3->combobox_handle, CB_SETCURSEL, userConfig.adapter, 0);

	main_window.run();

	RemoveHook();

	return 0;   
}

bool InitConfig(void)
{
	std::string basePath = getCurrentPath();
	OutputDebugString("Current Path: ");
	OutputDebugString(basePath.c_str());
	OutputDebugString("\n");

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG createRes = RegCreateKeyEx(HKEY_CURRENT_USER, sk, 0, NULL, 0, 0, NULL, &hKey, NULL);

	if (createRes == ERROR_SUCCESS) {
		OutputDebugString("Success creating Registry.\n");
	} else {
		OutputDebugString("Error creating Registry.\n");
	}

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS , &hKey);

	if (openRes==ERROR_SUCCESS) {
		OutputDebugString("Success opening key.\n");
	} else {
		OutputDebugString("Error opening key.\n");
	}

	LPCTSTR value = TEXT("BasePath");
	LPCTSTR data = basePath.c_str();

	LONG setRes = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)data, strlen(data)+1);

	if (setRes == ERROR_SUCCESS) {
		OutputDebugString("Success writing to Registry.\n");
	} else {
		OutputDebugString("Error writing to Registry.\n");
	}

	LONG closeOut = RegCloseKey(hKey);

	if (closeOut == ERROR_SUCCESS) {
		OutputDebugString("Success closing key.\n");
	} else {
		OutputDebugString("Error closing key.\n");
	}

	return true;
}

bool InitModes()
{
	int comboPos = 0;
	stereoModes[0] = comboPos++;
	stereoModes[20] = comboPos++;
	stereoModes[30] = comboPos++;
	stereoModes[40] = comboPos++;
	stereoModes[50] = comboPos++;
	stereoModes[60] = comboPos++;
	stereoModes[1] = comboPos++;
	stereoModes[2] = comboPos++;
	stereoModes[5] = comboPos++;
	stereoModes[6] = comboPos++;
	stereoModes[10] = comboPos++;
	stereoModes[11] = comboPos++;
	stereoModes[100] = comboPos++;
	stereoModes[110] = comboPos++;

	comboPos = 0;
	trackerModes[0] = comboPos++;
	trackerModes[10] = comboPos++;
	trackerModes[20] = comboPos++;
	trackerModes[30] = comboPos++;
	trackerModes[40] = comboPos++;

	return true;
}

std::string getCurrentPath(void)
{
	TCHAR fullPath[1024];
	int len = 1024;
	int bytes = GetModuleFileName(NULL, fullPath, len);
	if(bytes == 0)
		return false;

	std::string pathName = std::string(fullPath);
	std::string exeName = "bin\\Perception.exe";
	std::string basePath = "";
	size_t found = pathName.find(exeName);

	if (found != std::string::npos)
	{
		basePath = pathName.substr(0, int(found));
	}

	return basePath;
}