/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
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
#ifndef INICIO_MAIN_APP
#define INICIO_MAIN_APP

#define AQUILINUS_VERSION_PRIMA 1
#define AQUILINUS_VERSION_SECONDO 0
#define AQUILINUS_VERSION_TERZO 0

#define MAIN_WINDOW_WIDTH 680
#define MAIN_WINDOW_HEIGHT 200
#define MAIN_WINDOW_HEIGHT_INFO 800
#define SELECT_PROCESS_WINDOW_WIDTH 800
#define SELECT_PROCESS_WINDOW_HEIGHT 568
#define NEW_PROJECT_WINDOW_WIDTH 474
#define NEW_PROJECT_WINDOW_HEIGHT 568
#define OPTIONS_WINDOW_WIDTH 424
#define OPTIONS_WINDOW_HEIGHT 680
#ifdef DEVELOPER_BUILD
#define ADD_PROCESS_WINDOW_WIDTH 424
#define ADD_PROCESS_WINDOW_HEIGHT 568
#endif

#define GLFW_INCLUDE_NONE

#include <windows.h>
#include <windowsx.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <algorithm>
#include "Resources.h"
#include "..//Aquilinus/AquilinusCfg.h"
#include "..//Aquilinus/AQU_FileManager.h"
#include "..\dependecies\imgui\imgui.h"
#include "..\dependecies\imgui\imgui_impl_glfw.h"
#include "..\dependecies\imgui\imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "advapi32.lib")

/*** Inicio Types ***/

/**
* Inicio menu control status.
***/
enum InicioStatus
{
	Idle,
	Injecting,
	ToInject
//	Injected
	/*NewProjectWindow,
	OptionsWindow,
#ifdef DEVELOPER_BUILD
	AddProcessWindow,
#endif*/
};

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

/**
* Enumeration of all Inicio controls.
***/
enum InicioControls
{
	InicioExit = 0,
	InicioNewProject = 1,
	InicioLoadProject = 2,
	InicioOptions = 3,
	InicioInfo = 4,
	InicioLoadProfile = 5,
	NewProjectExit = 6,
	ProceedToInjection = 7,
	NextPage = 8,
	PreviousPage = 9,
	OK = 10,
	ColorsList = 11,
	SupportedInterfacesList = 12,
	HookSelection = 13,
	OptionsExit = 14,
	SelectProcessExit = 15,
	GameList = 16,
	TimeDelaySlider = 17,
	Info = 18,
	AddProfileWindow = 19,
	KeepProcessName = 20,
	InjectionRepetition = 21,
	EditMode = 22,
	D3DSelection = 23,
	Reserved9 = 24,
#ifdef DEVELOPER_BUILD
	ProcessesList = 25,
	AddProcessExit = 26,
	NewProcess = 27,
	SaveProcessList = 28,
	LoadCSV = 29,
	SaveCSV = 30,
	SaveTXT = 31,
	InicioTypesNumber = 32,   /**< The number of types **/
#else
	InicioTypesNumber = 25,   /**< The number of types **/
#endif
};

/**
* Main window control structure.
***/
struct InicioControl
{
	RECT rcButtonRect;               /**< The main window button rectangle. **/
	bool bIsPressed;                 /**< True if the mouse button 1 is pressed and the cursor is in the rectangles border. **/
	union
	{
		bool bIsOpen;                /**< True if this control is opened. **/
		DWORD dwCurrentPage;         /**< The current menu page of a book menu. **/
		int nMinValue;               /**< The minimum integer value of that control **/
		float fMinValue;             /**< The minimum float value of that control. **/
	};
	union
	{
		DWORD dwEntries;              /**< The number of entries in that control. **/
		int nMaxValue;               /**< The maximum integer value of that control **/
		float fMaxValue;             /**< The maximum float value of that control. **/
	};
	union
	{
		LPWSTR * pszEntries;         /**< The entry strings. **/
		LPCWSTR szText;              /**< The text of the control. **/
		int nValue;                  /**< The current integer value of that control **/
		float fValue;                /**< The current float value of that control. **/
	};
	DWORD dwSelection;               /**< The current selection. **/
};

/**
* The Aquilinus versioning structure.
***/
struct AQU_Version 
{ 
	DWORD dwPrima;
	DWORD dwSecondo; 
	DWORD dwTerzo; 
};

/**
* Inicio sub window types.
***/
enum class InicioWindows
{
	Main = 0,
	SelectProcess,
	SelectProfile,
	NewProject,
	Options,
	Info,
#ifdef DEVELOPER_BUILD
	AddProcess,
#endif
};

#endif

/*** Inicio Globals  ***/
AQU_Drawer*           g_pDirectDraw;                                             /**< Pointer to the direct draw class. **/
AQU_Drawer*           g_pDirectDrawSub;                                          /**< Pointer to the sub window direct draw class. **/
AQU_Drawer*           g_pDirectDrawProfile;                                      /**< Pointer to the profile window direct draw class. **/
AQU_FileManager*          g_pFileManager;                                            /**< Inicio File Manager class. ***/
InicioStatus              g_eInicioStatus;                                           /**< The control status for Inicio or the Aquilinus RTE. **/
int                       g_nScreenWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
int                       g_nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
int                       g_nMainWindowWidth  = MAIN_WINDOW_WIDTH;                   /**< Current window width. **/
int                       g_nMainWindowHeight = MAIN_WINDOW_HEIGHT;                  /**< Current window height. **/
DWORD                     g_dwColorSchemeIndex = 0;                                  /**< Global color scheme index. Only needed to load/save options. Actual color scheme index stored in config. **/
bool                      g_bControlActivated;                                       /**< True if any control is activated currently. **/
bool                      g_bSubMenuOpen;                                            /**< True if any sub menu is opened. **/
bool                      g_bKeepProcessName = false;                                /**< True if the game process should be kept for next workspace load **/
bool                      g_bLetterButtonPressed[27];                                /**< True if the button for that letter is pressed in the select process window. **/
DWORD                     g_nLetterStartIndex[27];                                   /**< The start index for that Letter (0, A, B,....., X, Y, Z) in the game list. **/
DWORD                     g_nLetterEntriesNumber[27];                                /**< The number of entries for that Letter (0, A, B,....., X, Y, Z) in the game list. **/
DWORD                     g_nCurrentLetterSelection;                                 /**< The Letter selection (0, A, B,....., X, Y, Z) in the game list. From 0 to 27. **/
POINT                     g_ptMouseCursor;                                           /**< The current mouse cursor. **/
WNDCLASSEX                g_wc;                                                      /**< The window class. **/
HINSTANCE                 g_hInstance;                                               /**< The main instance handle. **/
HWND                      g_hwnd;                                                    /**< The main window handle **/
HWND                      g_hSubWindow;                                              /**< The handle for the sub window. **/
HWND                      g_hProfileWindow;                                          /**< The handle for the profile window. **/
BYTE*                     g_pPictureData;                                            /**< The global picture data. **/
DWORD                     g_dwPictureSize;                                           /**< The size of the raw (PNG) data. **/
HANDLE                    g_hInjectionThread;
POINT                     g_vcZeroOrigin;                                            /**< A zero origin used for drawing methods. ***/
std::vector<std::wstring> g_aszGameNames;                                            /**< The sorted game list to be used in the game selection window. **/
std::vector<std::wstring> g_aszGameNamesUnsorted;                                    /**< The unsorted game list to be used to get the process index for the game **/
int                       g_nRepeat = 0;                                             /**< The number of process IDs to be skipped in injection process. (0..15) **/

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT
AQU_Version               g_eVersion;                                                /**< The current version of Aquilinus. Maybe we put that in the configuration... **/
InicioControl             g_sInicioControls[InicioControls::InicioTypesNumber];      /**< All inicio controls. Index equals InicioControl enumeration. **/
InicioWindows             g_eCurrentWindow;                                          /**< The current active window. ***/
bool                      g_bWindowResize;                                           /**< True if main window ought to be resized. **/
#endif

#ifdef DEVELOPER_BUILD
wchar_t             g_szGame[MAX_PATH];
wchar_t             g_szWindow[MAX_PATH];
wchar_t             g_szProc[MAX_PATH];
#endif

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

/*** Inicio controls methods ***/
void             InitControls();
void             SaveConfig();
bool             InRect(RECT rc, POINT pt);
void             SetSelectionBoxEntry(InicioControl &sInicioControl);
void             RenderSelectionBox(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize);
void             RenderSelectionBoxEntries(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize);
void             RenderSelectionTextList(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl);
void             RenderButton(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize);
void             RenderSlider(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize);
void             SetSliderValue(InicioControl &sInicioControl, POINT pt);
void             HandleSelectionButtonDown(InicioControl &sInicioControl);
void             HandleSelectionButtonUp(InicioControl &sInicioControl);
void             HandleSelectProcessButtonDown(HWND hwnd);
void             HandleSelectProcessButtonUp(HWND hwnd);
void             RenderSelectProcess();
void             HandleNewProjectButtonDown(HWND hwnd);
void             HandleNewProjectButtonUp(HWND hwnd);
void             SetHookSelection(DWORD dwSelection);
void             RenderNewProject();
void             HandleOptionsButtonDown(HWND hwnd);
void             HandleOptionsButtonUp(HWND hwnd);
void             RenderOptions();

#endif

#ifdef DEVELOPER_BUILD

BOOL    CALLBACK TextInputProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
void             HandleAddProcessButtonDown(HWND hwnd);
void             HandleAddProcessButtonUp(HWND hwnd);
void             RenderAddProcess();
DWORD            EnumerateProcesses(LPWSTR *&pszEntries, DWORD &dwEntries);

#endif

/*** Inicio methods ***/
HRESULT          InicioInit();
void             InicioClose();
void             ForceIdle();
HRESULT          EnumerateGameNames();
HRESULT          EnumerateSupportedInterfaces(LPWSTR *&pszEntries, DWORD &dwEntries);
BOOL             SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege); 
DWORD            GetTargetThreadID(wchar_t * szProcName);
HRESULT          Inject(DWORD dwID, const wchar_t * szDllName);
DWORD   WINAPI   InjectionThread(LPVOID Param);
DWORD   WINAPI   ProfileWindowThread(LPVOID Param);
DWORD   WINAPI   ProfileWindowMainThread(LPVOID Param);
LRESULT CALLBACK WndProcProfileWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

DWORD   WINAPI   SubWindowThread(LPVOID Param);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcSubWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int     WINAPI   WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif

#endif