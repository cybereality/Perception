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
#include <windows.h>
#include <sstream>

#ifndef AQU_GLOBALTYPES
#define AQU_GLOBALTYPES

/*** Aquilinus global defines ***/
#define DIRECTX_8_0 80
#define	DIRECTX_8_0A 81	
#define	DIRECTX_8_1 82
#define	DIRECTX_8_1A 83
#define	DIRECTX_8_1B 84
#define	DIRECTX_8_2 85
#define DIRECTX_9_0 90
#define	DIRECTX_9_0A 91
#define	DIRECTX_9_0B 92
#define	DIRECTX_9_0C 93
#define	DIRECTX_9_29 94
#define	DIRECTX_10 100
#define	DIRECTX_10_1 101
#define	DIRECTX_11 110
#define	DIRECTX_11_1 111
#define	DIRECTX_11_2 112
#define	DIRECTX_11_3 113
#define DIRECTX_12 120

#define D3D9_IDIRECT3DBASETEXTURE9_METHODS_NUMBER 17
#define D3D9_IDIRECT3DCUBETEXTURE9_METHODS_NUMBER 22
#define D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER 119
#define D3D9_IDIRECT3DINDEXBUFFER9_METHODS_NUMBER 14
#define D3D9_IDIRECT3DPIXELSHADER9_METHODS_NUMBER 5
#define D3D9_IDIRECT3DQUERY9_METHODS_NUMBER 8
#define D3D9_IDIRECT3DRESOURCE9_METHODS_NUMBER 11
#define D3D9_IDIRECT3DSTATEBLOCK9_METHODS_NUMBER 6
#define D3D9_IDIRECT3DSURFACE9_METHODS_NUMBER 15
#define D3D9_IDIRECT3DSWAPCHAIN9_METHODS_NUMBER 10
#define D3D9_IDIRECT3DTEXTURE9_METHODS_NUMBER 22
#define D3D9_IDIRECT3DVERTEXBUFFER9_METHODS_NUMBER 14
#define D3D9_IDIRECT3DVERTEXDECLARATION9_METHODS_NUMBER 5
#define D3D9_IDIRECT3DVERTEXSHADER9_METHODS_NUMBER 5
#define D3D9_IDIRECT3DVOLUME9_METHODS_NUMBER 11
#define D3D9_IDIRECT3DVOLUMETEXTURE9_METHODS_NUMBER 22
#define D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER 134
#define D3D10_ID3D10DEVICE_METHODS_NUMBER 98
#define D3D10_ID3D10DEVICE1_METHODS_NUMBER 101
#define D3D11_ID3D11DEVICE_METHODS_NUMBER 43
#define D3D11_ID3D11DEVICE1_METHODS_NUMBER 50
#define D3D11_ID3D11DEVICE2_METHODS_NUMBER 0
#define D3D11_ID3D11DEVICE3_METHODS_NUMBER 0
#define DXGI_IDXGISWAPCHAIN_METHODS_NUMBER 18
#define DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER 29
#define DXGI_IDXGISWAPCHAIN2_METHODS_NUMBER 0
#define DXGI_IDXGISWAPCHAIN3_METHODS_NUMBER 0
#define D3D11_ID3D11DEVICECONTEXT_METHODS_NUMBER 111
#define D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER 130
#define D3D11_ID3D11DEVICECONTEXT2_METHODS_NUMBER 0
#define D3D11_ID3D11DEVICECONTEXT3_METHODS_NUMBER 0

#define MAX_INTERFACES_NUMBER 256

#define ELEMENTARY_NODE_FALSE            0xFFFF0000
#define ELEMENTARY_NODE_TRUE             0xFFFF0001
#define ELEMENTARY_NODE_FIXED_FLOAT      0xFFFF0002
#define ELEMENTARY_NODE_FIXED_ASTRING    0xFFFF0003
#define ELEMENTARY_NODE_FIXED_WSTRING    0xFFFF0004
#define ELEMENTARY_NODE_FIXED_INT        0xFFFF0005
#define ELEMENTARY_NODE_FIXED_POINTER    0xFFFF0008
#define ELEMENTARY_NODE_CONFIGURATION    0xFFFF0009
#define ELEMENTARY_NODE_PLUGIN           0xFFFF000A
#define ELEMENTARY_NODE_HELLO_WORLD_DX9  0xFFFF0010
#define ELEMENTARY_NODE_HELLO_WORLD_DX10 0xFFFF0011
#define ELEMENTARY_NODE_MATH_LOOKAT      0xFFFF0800
#define ELEMENTARY_NODE_DEBUG_D3DVECTOR  0xFFFF0900
#define ELEMENTARY_NODE_DEBUG_D3DMATRIX  0xFFFF0901

#define D3DOPS_NODE_FRAMEDECAL           0xFFFF1000
#define D3DOPS_NODE_D3DX9                0xFFFF2000

/**
* The joliet specification only allows filenames to be up to 64 Unicode characters in length.
* We use that as limit for the process list.
***/
#define MAX_JOLIET_FILENAME 64

/*** Aquilinus global types ***/

/**
* The current stage of the project.
***/
enum class AQU_ProjectStage
{
	NoProject = 0,
	BrokenProject = 1,
	WorkingArea = 2,
	WorkingAreaNew = 3,
	WorkingAreaLoad = 4,
	Complemented = 5,
	ComplementedInjected = 6,
	Closed = 7
};

/**
* The supported Direct3D Versions.
***/
enum AQU_Direct3DVersion
{
	DirectX_8_0    = DIRECTX_8_0,
	DirectX_8_0a   = DIRECTX_8_0A,	
	DirectX_8_1    = DIRECTX_8_1,
	DirectX_8_1a   = DIRECTX_8_1A,
	DirectX_8_1b   = DIRECTX_8_1B,
	DirectX_8_2    = DIRECTX_8_2,
	DirectX_9_0    = DIRECTX_9_0,
	DirectX_9_0a   = DIRECTX_9_0A,
	DirectX_9_0b   = DIRECTX_9_0B,
	DirectX_9_0c   = DIRECTX_9_0C,
	DirectX_9_29   = DIRECTX_9_29,
	DirectX_10     = DIRECTX_10,
	DirectX_10_1   = DIRECTX_10_1,
	DirectX_11     = DIRECTX_11,
	DirectX_11_1   = DIRECTX_11_1,
	DirectX_11_2   = DIRECTX_11_2,
	DirectX_11_3   = DIRECTX_11_3,
	DirectX_12     = DIRECTX_12
};

/**
* The supported injection techniques.
***/
enum AQU_InjectionTechniques
{
	NoInjection,
	VMTable,
	Detour,
	Proxy
};

/**
* Base Aquilinus shared memory data structure.
* Inherits ALL data shared between the driver and the main app >Inicio<.
***/
struct AquilinusCfg
{
	wchar_t                 szEntryName[MAX_JOLIET_FILENAME];                 /**< The entry name (used only in compiled projects) ***/
	wchar_t                 szWindowName[MAX_JOLIET_FILENAME];                /**< The window name (used only in compiled projects) ***/
	wchar_t                 szProcessName[MAX_JOLIET_FILENAME];               /**< The process name (used only in compiled projects) ***/
	DWORD                   dwProcessIndex;                                   /**< The current game process index in the filemanager class. **/
	BOOL                    bEmptyProcess;                                    /**< True if the process is empty. (for empty process profiles) **/
	BOOL                    bAlwaysForceD3D;                                  /**< Always force D3D bool. This bool ensures that the detour classes ALWAYS call (super) D3D methods. **/
	DWORD                   dwID;                                             /**< The process ID of the last injected process ***/
	wchar_t                 szAquilinusPath[MAX_PATH];                        /**< The directory path of the driver dll. **/
	wchar_t                 szWorkspaceFilePath[MAX_PATH];                    /**< The exact file path of the last loaded workspace file. **/
	wchar_t                 szProfileFilePath[MAX_PATH];                      /**< The exact file path of the last loaded workspace file. **/
	wchar_t                 szPictureFilePath[MAX_PATH];                      /**< The exact file path of the game profile png image file. **/
	wchar_t                 szExternalSaveFilePath[MAX_PATH];                 /**< The exact file path of the external save file to be saved by Inicio. **/
	AQU_ProjectStage        eProjectStage;                                    /**< The project stage, defines wether the project has a working area or is complemented and the loading state. **/
	__int32                 eInjectionTechnique[MAX_INTERFACES_NUMBER];       /**< The chosen injection technique for each supported D3D interface. ***/
	DWORD                   dwColorSchemeIndex;                               /**< The current color scheme index. **/
	unsigned __int32        dwDetourTimeDelay;                                /**< The delay time for the detour, in milliseconds. **/
	__int32                 nInjectionRepetition;                            /**< Repeat the injectin process for various times. (0..15) **/
	BOOL                    bProfileWindow;                                   /**< True if a profile window is present for the current game profile. **/
	BOOL                    bExternalSave;                                    /**< True if Inicio should save a file path stored in a memory data page. ***/
	BOOL                    bUseDllPath;                                      /**< True if Aquilinus uses the dll path stored in this->szAquilinusPath, False if it uses the path in User//my games//Aquilnus **/
	BOOL                    bCreateD3D9Ex;                                    /**< True if Aquilinus creates a D3D9Ex device instead of a D3D9 device. ***/
	DWORD                   dwSizeOfExternalSaveFile;                         /**< The size of the file to be saved externally. ***/
};

#endif
