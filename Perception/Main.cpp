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
#include <algorithm>
#include "Resource.h"
#include "dependecies/json/json.hpp"
#include ".\dependecies\imgui\imgui.h"
#include ".\dependecies\imgui\imgui_impl_glfw.h"
#include ".\dependecies\imgui\imgui_impl_opengl3.h"
#include ".\dependecies\imgui\imgui_internal.h"
#include ".\dependecies\imgui\imgui_helpers.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "loadjpg.h"

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

/// <summary>bind raw texture data as texture in OpenGl (RGB => RGBA)</summary>
void bindTexture(GLuint& uTxId, GLsizei uW, GLsizei uH, unsigned char* rgbpix)
{
	if (rgbpix == NULL)
		return;

	glGenTextures(1, &uTxId);
	glBindTexture(GL_TEXTURE_2D, uTxId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uW, uH, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbpix);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, uW, uH, GL_RGB, GL_UNSIGNED_BYTE, rgbpix);
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
	GLFWwindow* m_psWindow;
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
	static std::vector<std::string> m_aatGames;   /**< Game profiles strings ***/
	static string      m_atAppVersion;            /**< Version string for the main app ***/
	static int         m_nCpX, m_nCpY;
	static int         m_nOffsetCpX, m_nOffsetCpY;
	static int         m_nButtonEvent;
	static GLuint      m_uLogoTexId;
	static unsigned    m_uLogoW, m_uLogoH;
	static int         m_nProfile_index, m_nHMD_index;

public:
	/// <summary>Window Constructor.</summary>
	Vireio_Perception_Main_Window(HINSTANCE pvHinst)
	{
		// zero static members
		m_nCpX = 0;
		m_nCpY = 0;
		m_nOffsetCpX = 0;
		m_nOffsetCpY = 0;
		m_nButtonEvent = 0;

		// get screen and module handle
		int nScreen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		int nScreen_height = GetSystemMetrics(SM_CYFULLSCREEN);
		m_pvInstance_handle = pvHinst;

		// glfw: initialize and configure
		if (!glfwInit()) { OutputDebugString(L"[Perception] GLFW Init fail !"); return; }
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		glfwWindowHint(GLFW_DECORATED, 0);
		m_psWindow = glfwCreateWindow(APP_SIZE_WIDTH, APP_SIZE_HEIGHT, "Vireio Perception", NULL, NULL);
		if (m_psWindow == NULL)
		{
			OutputDebugString(L"[Perception] GLFW window creation failed !");
			glfwTerminate();
			return;
		}

		// set size, position, callback methods, enable vsync
		glfwSetWindowSizeLimits(m_psWindow, APP_SIZE_WIDTH, APP_SIZE_HEIGHT, APP_SIZE_WIDTH, APP_SIZE_HEIGHT);
		glfwSetWindowPos(m_psWindow, (nScreen_width / 2) - (APP_SIZE_WIDTH), (nScreen_height / 8));
		glfwMakeContextCurrent(m_psWindow);
		glfwSetErrorCallback(Error_callback);

		// and init glew
		bool err = glewInit() != GLEW_OK;
		if (err)
		{
			OutputDebugString(L"[Perception] Failed to initialize OpenGL loader!\n");
			return;
		}

		// setup ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::ColorSchemeHex sScheme = { 0x4A4059,
			0xA18093,
			0xEC979A,
			0xFDCAC9,
			0xE8E8DE };
		ImGui::StyleColorsByScheme(sScheme);
		ImGui_ImplGlfw_InitForOpenGL(m_psWindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// set callbacks
		glfwSetFramebufferSizeCallback(m_psWindow, Viewport_callback);
		glfwSetCursorPosCallback(m_psWindow, CursorPosition_callback);
		glfwSetMouseButtonCallback(m_psWindow, MouseButton_callback);

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
		// user settings
		m_nProfile_index = cConfig_json.value("profile_index", 0);
		m_nHMD_index = cConfig_json.value("hmd_index", 0);

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
				m_aatGames.push_back(sProfile.atGameName);
			}
		}

		// set version string
		std::string atDate(__DATE__);
		std::string atBuildDate = atDate.substr(4, 2) + "-" + atDate.substr(0, 3) + "-" + atDate.substr(7, 4);
		std::stringstream atStream = std::stringstream();
		atStream << "v" << APP_VERSION << " " << atBuildDate.c_str();
#ifdef _WIN64
		atStream << " 64bit";
#else
		szStream << " 32bit";
#endif
		m_atAppVersion = atStream.str();
		OutputDebugStringA(m_atAppVersion.c_str());

		// load logo file (jpg)
		FILE* fp;
		unsigned int lengthOfFile = 0;
		unsigned char* buf;
		unsigned char* rgbpix = NULL;
		fopen_s(&fp, "..//..//img//logo.jpg", "rb");
		if (fp == NULL)
		{
			OutputDebugStringA("[Perception] Failed to open img//logo.jpg");
			return;
		}
		lengthOfFile = FileSize(fp);
		buf = new unsigned char[lengthOfFile + 4];
		if (buf == NULL)
		{
			OutputDebugStringA("[Perception] Not enough memory for loading file\n");
			return;
		}
		fread(buf, lengthOfFile, 1, fp);
		fclose(fp);
		DecodeJpgFileData(buf, lengthOfFile, &rgbpix, &m_uLogoW, &m_uLogoH);

		// bind raw bitmap data to texture id
		bindTexture(m_uLogoTexId, (GLsizei)m_uLogoW, (GLsizei)m_uLogoH, rgbpix);
	}
	~Vireio_Perception_Main_Window() { UnregisterClass(m_atWindow_class_name, m_pvInstance_handle); }

	/// <summary>
	/// Viewport callback.
	/// </summary>
	static void Viewport_callback(GLFWwindow * psWindow, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	/// <summary>
	/// Cursor position callback.
	/// </summary>
	static void CursorPosition_callback(GLFWwindow * psWindow, double x, double y)
	{
		if ((m_nButtonEvent == 1) && ((y < (double)m_uLogoH) || (m_nButtonEvent == 1)))
		{
			m_nOffsetCpX = (int)x - m_nCpX;
			m_nOffsetCpY = (int)y - m_nCpY;
		}
	}

	/// <summary>
	/// Mouse button callback.
	/// </summary>
	static void MouseButton_callback(GLFWwindow * psWindow, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(psWindow, &x, &y);
			if (y < (double)m_uLogoH)
			{
				m_nCpX = (int)floor(x);
				m_nCpY = (int)floor(y);
				m_nButtonEvent = 1;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			m_nButtonEvent = 0;
			m_nCpX = 0;
			m_nCpY = 0;
		}
	}

	/// <summary>
	/// GLFW error callback
	/// </summary>
	static void Error_callback(int nError, const char* atDescription)
	{
		TRACE_UINT(nError);
		OutputDebugStringA(atDescription);
	}

	/// <summary>handle aquilinus profile loading and injecting</summary>
	void hook_handler()
	{
		// constant number of HMD profiles present
		const UINT uHMD_N = 3;

		if (g_hmAquilinusRTE)
		{
			InjectionState eIS = (InjectionState)g_pAquilinus_GetInjectionState();
			if ((g_bLoadAquilinusProfile) && (eIS))
			{
				if ((eIS == InjectionState::Idle) || (eIS == InjectionState::Injecting))
				{
					OutputDebugStringA("[Perception] Load Aquilinus Profile !");

					// get current selections
					UINT unSelectionGame = (UINT)m_nProfile_index;
					if (unSelectionGame >= (UINT)m_asVireioGameProfiles.size()) unSelectionGame = 0;
					UINT unSelectionHMD = (UINT)m_nHMD_index;
					if (unSelectionHMD >= uHMD_N) unSelectionHMD = 0;
					OutputDebugStringA(m_asVireioGameProfiles[unSelectionGame].atGameName.c_str());

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
	}

	/// <summary>Vireio Perception glfw main loop</summary>
	void run()
	{
		ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
		static std::vector<std::string> s_aatHMDs = { "No HMD (Monitor)", "LibOVR", "OpenVR" };

		// => <main> render loop
		while (!glfwWindowShouldClose(m_psWindow))
		{
			glfwPollEvents();
			int nWPosX, nWPosY;
			glfwGetWindowPos(m_psWindow, &nWPosX, &nWPosY);
			glfwSetWindowPos(m_psWindow, nWPosX + m_nOffsetCpX, nWPosY + m_nOffsetCpY);
			m_nOffsetCpX = 0;
			m_nOffsetCpY = 0;

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// main window set flags and zero pos
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoBackground
				| ImGuiWindowFlags_NoBringToFrontOnFocus
				| ImGuiWindowFlags_NoTitleBar;
			ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2((float)APP_SIZE_WIDTH, (float)APP_SIZE_HEIGHT), ImGuiCond_FirstUseEver);

			static bool s_bOpen = true;
			if (ImGui::Begin("Vireio Perception", &s_bOpen, window_flags))
			{
				// logo image
				ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
				ImGui::Image((void*)(intptr_t)m_uLogoTexId, ImVec2((float)m_uLogoW, (float)m_uLogoH),
					ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

				// combos
				int nCIx = 0; int nCIxSelected = -1;
				ImGui::SetCursorPosY((float)m_uLogoH * 1.4f);
				for (std::string atCombo : { "HMD", "Game" })
				{
					ImGui::SetCursorPosX(32.0f);
					std::vector<std::string>* paatItems = nullptr;
					int* pnIx = nullptr;
					switch (nCIx)
					{
					case 0:paatItems = &s_aatHMDs; pnIx = &m_nHMD_index; break;
					case 1:paatItems = &m_aatGames; pnIx = &m_nProfile_index; break;
					default:paatItems = &m_aatGames; pnIx = &m_nProfile_index; break;
					}
					if (ImGui::BeginCombo(atCombo.c_str(), (*pnIx < (int)paatItems->size()) ? (*paatItems)[*pnIx].c_str() : "", 0))
					{
						int n = 0;
						for (std::string& at : (*paatItems))
						{
							const bool is_selected = (*pnIx == n);
							if (ImGui::Selectable(at.c_str(), is_selected)) *pnIx = n;
							if (is_selected) {
								ImGui::SetItemDefaultFocus(); nCIxSelected = nCIx;
							}
							n++;
						}
						ImGui::EndCombo();
					}
					nCIx++;
				}

				// combo box selected ?
				if (nCIxSelected == 0)
				{

				}
				else if (nCIxSelected == 1)
				{
					// new game selected, load profile new
					g_bLoadAquilinusProfile = true;
				}

				// Exit button
				ImVec2 sTextSize = ImGui::CalcTextSize("Exit");
				ImGui::SetCursorPos(ImVec2((float)APP_SIZE_WIDTH - (sTextSize.x * 2.0f), (float)APP_SIZE_HEIGHT - (sTextSize.y * 4.0f)));
				if (ImGui::Button("Exit")) glfwSetWindowShouldClose(m_psWindow, GL_TRUE);

				// version string + injection state (one line)
				sTextSize = ImGui::CalcTextSize(m_atAppVersion.c_str());
				ImGui::SetCursorPosY((float)APP_SIZE_HEIGHT - (sTextSize.y * 1.2f));
				g_eInjectionState = (InjectionState)g_pAquilinus_GetInjectionState();
				switch (g_eInjectionState)
				{
				case Initial:
					ImGui::TextUnformatted("Initial"); break;
				case Idle:
					ImGui::TextUnformatted("Idle"); break;
				case Injecting:
					ImGui::TextUnformatted("Injecting"); break;
				case ToInject:
					ImGui::TextUnformatted("ToInject"); break;
				case Injected:
					ImGui::TextUnformatted("Injected"); break;
				case Closed:
					ImGui::TextUnformatted("Closed"); break;
				default: break;
				}
				ImGui::SetCursorPos(ImVec2((float)APP_SIZE_WIDTH - sTextSize.x - (sTextSize.y * 0.2f), (float)APP_SIZE_HEIGHT - (sTextSize.y * 1.2f)));
				ImGui::TextUnformatted(m_atAppVersion.c_str());

				ImGui::End();
			}

			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(m_psWindow, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_psWindow);

			// handle injection
			hook_handler();
		}

		// Cleanup ImGui + GL
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_psWindow);
		glfwTerminate();
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
std::vector<std::string> Vireio_Perception_Main_Window::m_aatGames;
string      Vireio_Perception_Main_Window::m_atAppVersion;
int         Vireio_Perception_Main_Window::m_nCpX, Vireio_Perception_Main_Window::m_nCpY;
int         Vireio_Perception_Main_Window::m_nOffsetCpX, Vireio_Perception_Main_Window::m_nOffsetCpY;
int         Vireio_Perception_Main_Window::m_nButtonEvent;
GLuint      Vireio_Perception_Main_Window::m_uLogoTexId;
unsigned    Vireio_Perception_Main_Window::m_uLogoW, Vireio_Perception_Main_Window::m_uLogoH;
int         Vireio_Perception_Main_Window::m_nProfile_index, Vireio_Perception_Main_Window::m_nHMD_index;

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
