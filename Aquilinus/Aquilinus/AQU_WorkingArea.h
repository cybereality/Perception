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

#pragma warning( disable : 26812 )
#pragma warning( disable : 26495 )

#ifndef AQU_WORKINGAREA_CLASS
#define AQU_WORKINGAREA_CLASS

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <d3d10_1.h>
#include <d3d10.h>
#include <windowsx.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include "AQU_TransferSite.h"
#include "AQU_FileManager.h"
#include "AQU_Nodes.h"
#include "AQU_GlobalTypes.h"
#include "..\dependecies\imgui\imgui.h"
#include "..\dependecies\imgui\imgui_impl_glfw.h"
#include "..\dependecies\imgui\imgui_impl_opengl3.h"
#include "..\dependecies\imgui\imgui_internal.h"
#include "..\dependecies\imgui\ImNodes.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "advapi32.lib")

/**
* Working area menu control status.
***/
enum AQU_WorkingAreaStatus
{
	Idle,
	ToIdle,
};

/**
* Direct X Working Area.
* Includes the window proc.
***/
class AQU_WorkingArea
{
public :
	AQU_WorkingArea(HINSTANCE hInstance, AQU_TransferSite* pcTransferSite);
	virtual ~AQU_WorkingArea();

	/*** AQU_WorkingArea public methods ***/
	static DWORD   WINAPI   s_WorkingAreaMsgThread (void* param);
	static HRESULT          s_LoadWorkSpace        ();
	static void             s_Viewport_callback(GLFWwindow* window, int width, int height)	{ glViewport(0, 0, width, height);	}
	static void             s_Cursor_position_callback(GLFWwindow* window, double x, double y) 
	{
		m_sMouseCursor.x = (float)x; m_sMouseCursor.y = (float)y;
		if ((m_sWindowControl.nButtonEvent == 1) && ((y < (double)ImGui::GetFontSize()) || (m_sWindowControl.nButtonEvent == 1)))
		{
			m_sWindowControl.nOffset_cpx = (int)x - m_sWindowControl.nCp_x;
			m_sWindowControl.nOffset_cpy = (int)y - m_sWindowControl.nCp_y;
		}
	}
	static void             s_Mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			if (y < (double)22) // TODO !! SET BY TOP BAR FONT SIZE
			{
				m_sWindowControl.nCp_x = (int)floor(x);
				m_sWindowControl.nCp_y = (int)floor(y);
				m_sWindowControl.nButtonEvent = 1;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			m_sWindowControl.nButtonEvent = 0;
			m_sWindowControl.nCp_x = 0;
			m_sWindowControl.nCp_y = 0;
		}
	}

private:
	/*** AQU_WorkingArea private methods ***/
	static HRESULT s_EnumeratePlugins (LPCWSTR szDllPath);
	
	/**
	* The window class.
	***/
	static WNDCLASSEX m_wc;
	/**
	* The window handle.
	***/
	static HWND m_hwnd;
	/**
	* The module handle.
	***/
	static HINSTANCE m_hInstance;
	/**
	* ImGui window control fields.
	***/
	static struct WindowControl
	{
		int nCp_x;
		int nCp_y;
		int nOffset_cpx;
		int nOffset_cpy;
		int nW_posx;
		int nW_posy;
		int nButtonEvent, nControlEvent;
	} m_sWindowControl;
	/**
	* Working area width.
	***/
	static int m_nWindowWidth;
	/**
	* Working area height.
	***/
	static int m_nWindowHeight;
	/**
	* The current mouse cursor.
	***/
	static ImVec2 m_sMouseCursor;
	/**
	* All nodes vector.
	***/
	static std::vector<NOD_Basic*> m_paNodes;
	/**
	* Working Area status.
	***/
	static AQU_WorkingAreaStatus m_eWorkingAreaStatus;
	/**
	* The Aquilinus transfer site.
	***/
	static AQU_TransferSite* m_pcTransferSite;
	/**
	* The plugin enumeration name vector.
	***/
	static std::vector<std::wstring> m_vcPluginNames;
	/**
	* The plugin enumeration file path vector.
	***/
	static std::vector<std::wstring> m_vcPluginFilePathes;
	/**
	* The plugin enumeration type id vector.
	***/
	static std::vector<UINT> m_vcPluginIDs;
	/**
	* The plugin enumeration category name.
	***/
	static std::vector<std::wstring> m_vcPluginCategories;
	/**
	* The enlisted plugin prototypes.
	***/
	static std::vector<AQU_Nodus*> m_vcPlugins;
	/**
	* The enlisted plugin handles.
	***/
	static std::vector<HMODULE> m_vcPluginHandles;
	/**
	* The currently selected data sheet category.
	***/
	static int m_nDataSheetCategorySelection;
	/**
	* The currently selected data sheet entry (for the selected category).
	***/
	static int m_nDataSheetEntrySelection;
};

#endif