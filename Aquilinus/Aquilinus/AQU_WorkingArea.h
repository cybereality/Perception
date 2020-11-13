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
#ifndef AQU_WORKINGAREA_CLASS
#define AQU_WORKINGAREA_CLASS

#define AQUILINUS_DRAWING_API AQU_DrawingAPIs::API_OpenGL
//#define AQUILINUS_DRAWING_API AQU_DrawingAPIs::API_DirectDraw

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

/**
* Working area menu control status.
***/
enum AQU_WorkingAreaStatus
{
	Idle,
	ToIdle,
	WindowResize,
	WorkspaceZoom,
	WorkspaceScrollX,
	WorkspaceScrollY,
	NodeActive,
	TopMenuBar_File,
	TopMenuBar_Options,
};

/**
* Top menu >File< entries enumeration.
***/
enum AQU_TopMenuBarFileEntries
{
	SAVE_WORKING_AREA,
	COMPILE_PROFILE,
	SAVE_EXTERNALLY,
	NUMBER_OF_FILE_ENTRIES = 3
};


/**
* Top menu >File< entries enumeration.
***/
enum AQU_TopMenuBarOptionsEntries
{
	REINSTATE_INTERFACES,
	DRAW_DATA_LINES,
	DRAW_DATA_SLINGS,
	DRAW_DATA_PIPELINES,
	DRAW_DATA_DOTTED,
	DRAW_STREAM_LINES,
	DRAW_STREAM_SLINGS,
	DRAW_STREAM_PIPELINES,
	DRAW_STREAM_DOTTED,
	DELETE_UNCONNECTED,
	NUMBER_OF_OPTIONS_ENTRIES = 10
};

/**
* Working area element sizes data field.
***/
struct AQU_WorkingAreaElementSizes
{
	/*** Workspace border sizes. ***/
	DWORD dwMenu;                 /**< The size, in pixels, of the upper menu tab. ***/
	DWORD dwDataSheet;            /**< The size, in pixels, of the left side data shed tab. ***/
	DWORD dwScrollBarRight;       /**< The size, in pixels, of the right side scroll bar tab. ***/
	DWORD dwScrollBarBottom;      /**< The size, in pixels, of the bottom scroll bar tab. ***/
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
	static LRESULT CALLBACK s_WorkingAreaWndProc   (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DWORD   WINAPI   s_WorkingAreaMsgThread (void* param);
	static HRESULT          s_LoadWorkSpace        ();

private:
	/*** AQU_WorkingArea private methods ***/
	static HRESULT s_EnumeratePlugins (LPCWSTR szDllPath);
	static int     s_GetNodeIndex     (POINT vecCursor, POINT vecOrigin, float fZoom);
	static void    s_UpdateActiveNode (UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	* Pointer to the direct draw class.
	***/
	static AQU_Drawer* m_pDirectDraw;
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
	* Desktop width.
	***/
	static int m_nScreenWidth;  
	/**
	* Desktop height.
	***/
	static int m_nScreenHeight;
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
	static POINT m_ptMouseCursor;
	/**
	* The current zoom factor.
	***/
	static float m_fZoom;
	/**
	* The y scollbar value.
	***/
	static float m_fXScrollPos;
	/**
	* The y scollbar value.
	***/
	static float m_fYScrollPos;
	/**
	* The data sheet y scrolling value.
	***/
	static float m_fYScrollDataSheet;
	/**
	* The size of the workspace.
	* (in pixel space with zoom factor 1.0)
	***/
	static SIZE m_vcWorkspaceSize;
	/**
	* The size of the DataSheet.
	***/
	static SIZE m_vcDataSheetSize;
	/**
	* All nodes vector.
	***/
	static std::vector<NOD_Basic*> m_paNodes;
	/**
	* Active node index.
	* -1 if no node active.
	***/
	static int m_nActiveNodeIndex;
	/**
	* The currently selected data sheet category.
	***/
	static int m_nDataSheetCategorySelection;
	/**
	* The currently selected data sheet entry (for the selected category).
	***/
	static int m_nDataSheetEntrySelection;
	/**
	* Active data shed entry.
	* -1 if no entry active.
	***/
	static int m_nActiveDataSheetEntry;
	/**
	* The currently selected top menu >File< entry selection.
	* -1 if no entry active.
	***/
	static int m_nActiveTopMenuFileEntry;
	/**
	* The currently selected top menu >Options< entry selection.
	* -1 if no entry active.
	***/
	static int m_nActiveTopMenuOptionsEntry;
	/**
	* Active node behavior.
	* Only in question if m_nActiveNodeIndex > -1.
	***/
	static AQU_NodeBehavior m_eActiveNodeBehavior;
	/**
	* Working Area status.
	***/
	static AQU_WorkingAreaStatus m_eWorkingAreaStatus;
	/**
	* The Aquilinus transfer site.
	***/
	static AQU_TransferSite* m_pcTransferSite;
	/**
	* The sizes of the workspace elements.
	***/
	static AQU_WorkingAreaElementSizes m_sWorkingAreaElementSizes;
	/**
	* The plugin enumeration name vector.
	***/
	static std::vector<LPSTR> m_vcPluginNames;
	/**
	* The plugin enumeration file path vector.
	***/
	static std::vector<LPWSTR> m_vcPluginFilePathes;
	/**
	* The plugin enumeration type id vector.
	***/
	static std::vector<UINT> m_vcPluginIDs;
	/**
	* The plugin enumeration category name.
	***/
	static std::vector<LPWSTR> m_vcPluginCategories;
	/**
	* The enlisted plugin prototypes.
	***/
	static std::vector<AQU_Nodus*> m_vcPlugins;
	/**
	* The enlisted plugin handles.
	***/
	static std::vector<HMODULE> m_vcPluginHandles;
	/**
	* Highlighted menu entries (File).
	* Pointer to the entry bool, one per menu entry. True to highlight.
	***/
	static BOOL* m_pbHighlightEntryFile[AQU_TopMenuBarFileEntries::NUMBER_OF_FILE_ENTRIES];
	/**
	* Highlighted menu entries (Options).
	* Pointer to the entry bool, one per menu entry. True to highlight.
	***/
	static BOOL* m_pbHighlightEntryOptions[AQU_TopMenuBarOptionsEntries::NUMBER_OF_OPTIONS_ENTRIES];
	/**
	* True if lines are drawn for commander connections.
	***/
	static BOOL m_bDrawDataLines;
	/**
	* True if slings are drawn for commander connections.
	***/
	static BOOL m_bDrawDataSlings;
	/**
	* True if lines are drawn for commander connections.
	***/
	static BOOL m_bDrawDataPipelines;
	/**
	* True if commander connections are drawn dotted.
	***/
	static BOOL m_bDrawDataDotted;
	/**
	* True if lines are drawn for provoker connections.
	***/
	static BOOL m_bDrawStreamLines;
	/**
	* True if slings are drawn for provoker connections.
	***/
	static BOOL m_bDrawStreamSlings;
	/**
	* True if lines are drawn for provoker connections.
	***/
	static BOOL m_bDrawStreamPipelines;
	/**
	* True if provoker connections are drawn dotted.
	***/
	static BOOL m_bDrawStreamDotted;
};

#endif