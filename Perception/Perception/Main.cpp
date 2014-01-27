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
		DrawText(item->hDC, string, length, &item->rcItem, DT_LEFT|DT_SINGLELINE);
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
		window_handle = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,    
			window_class_name,    
			"Vireio Perception", 
			WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), 
			//WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,   
			(screen_width-585)/2, (screen_height-195)/2, 
			585, 195,
			NULL, NULL, instance_handle, NULL);
		SetWindowLongPtr(window_handle, GWL_USERDATA, (LONG)this);  
		GetClientRect(window_handle, &client_rectangle);
		int width = client_rectangle.right - client_rectangle.left;
		int height = client_rectangle.bottom - client_rectangle.top;
		text = new static_control(instance_handle, window_handle);
		combobox = new combobox_control(instance_handle, window_handle, text, 20, 81, 1001);
		combobox2 = new combobox_control(instance_handle, window_handle, text, 20, 115, 1002);

		char viewPath[512];
		ProxyHelper helper = ProxyHelper();
		helper.GetPath(viewPath, "img\\logo.bmp");

		logo_bitmap = (HBITMAP)LoadImage(NULL,viewPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		OutputDebugString("Load the bitmap\n");

		ProxyHelper::ProxyConfig cfg;
		helper.LoadUserConfig(cfg, oculusProfile);

		SetCursor(LoadCursor(NULL, IDC_ARROW)); 
		ShowWindow(window_handle, SW_SHOW);   
		UpdateWindow(window_handle); 
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
			case WM_PAINT:   
				{   
					PAINTSTRUCT paint_structure;   
					RECT client_rect;   
					HDC paint_device_context, paint_dc;   
					HBITMAP bitmap;   

					paint_device_context = BeginPaint(window_handle, &paint_structure);   
					paint_dc = CreateCompatibleDC(paint_device_context);   
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
					BitBlt(paint_device_context,0,0,600,140,paint_dc,0,0,SRCCOPY);

					// Output user profile data
					SetBkMode(paint_device_context, TRANSPARENT);
					TextOut (paint_device_context,290,81,"Oculus Profile:",15);
					TextOut (paint_device_context,290,101,oculusProfile.Name.c_str(),oculusProfile.Name.size());

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
					return TRUE;
				}
			case WM_COMMAND:
				{
					if ( HIWORD(wparam) == CBN_SELCHANGE ) {
						This->combobox->new_selection();
						This->combobox2->new_selection2();
					} 
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
	main_window.add_item("DIY Rift\t25");
	main_window.add_item("Oculus Rift\t26");
	main_window.add_item("Oculus Rift Cropped\t27");
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

	main_window.add_item2("No Tracking\t0");
	main_window.add_item2("Hillcrest Labs\t10");
	main_window.add_item2("FreeTrack\t20");
	main_window.add_item2("Shared Memory Tracker\t30");
	main_window.add_item2("OculusTrack\t40");

	int mode;
	int mode2;
	ProxyHelper helper = ProxyHelper();
	helper.LoadUserConfig(mode, mode2);

	SendMessage(main_window.combobox->combobox_handle, CB_SETCURSEL, stereoModes[mode], 0);
	SendMessage(main_window.combobox2->combobox_handle, CB_SETCURSEL, trackerModes[mode2], 0);

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
	stereoModes[0] = 0;
	stereoModes[1] = 9;
	stereoModes[2] = 10;
	stereoModes[5] = 11;
	stereoModes[6] = 12;
	stereoModes[10] = 13;
	stereoModes[11] = 14;
	stereoModes[20] = 4;
	stereoModes[25] = 1;
	stereoModes[26] = 2;
	stereoModes[27] = 3;
	stereoModes[30] = 5;
	stereoModes[40] = 6;
	stereoModes[50] = 7;
	stereoModes[60] = 8;

	trackerModes[0] = 0;
	trackerModes[10] = 1;
	trackerModes[20] = 2;
	trackerModes[30] = 3;
	trackerModes[40] = 4;

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