/***************************************************************
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
****************************************************************/

#ifndef AQU_D3D9_HEAD
#define AQU_D3D9_HEAD

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#pragma region include
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "PCL_IDirect3D9.h"
#include "PCL_IDirect3DDevice9.h"

#include <stdio.h>
#include "..\Aquilinus\Resources.h"
#include "..\Aquilinus\AquilinusCfg.h"
#include "..\Aquilinus\AQU_DirectDraw.h"
#include "..\Aquilinus\AQU_TransferSite.h"
#include "..\Aquilinus\AQU_FileManager.h"
//#include "..\Aquilinus\AQU_WorkingArea.h"
/*#include "..\Aquilinus\VMT_ID3D10Device.h"
#include "..\Aquilinus\DCL_ID3D11DeviceContext.h"
#include "..\Aquilinus\DCL_ID3D11Device.h"
#include "..\Aquilinus\DCL_IDXGISwapChain.h"
#include "..\Aquilinus\DCL_ID3D10Device.h"*/

#pragma region DirectX 9.0 includes
// detour classes
#include "..\Aquilinus\DCL_IDirect3DBaseTexture9.h"
#include "..\Aquilinus\DCL_IDirect3DCubeTexture9.h"
#include "..\Aquilinus\DCL_IDirect3DDevice9.h"
#include "..\Aquilinus\DCL_IDirect3DIndexBuffer9.h"
#include "..\Aquilinus\DCL_IDirect3DPixelShader9.h"
#include "..\Aquilinus\DCL_IDirect3DQuery9.h"
#include "..\Aquilinus\DCL_IDirect3DResource9.h"
#include "..\Aquilinus\DCL_IDirect3DStateBlock9.h"
#include "..\Aquilinus\DCL_IDirect3DSurface9.h"
#include "..\Aquilinus\DCL_IDirect3DSwapChain9.h"
#include "..\Aquilinus\DCL_IDirect3DTexture9.h"
#include "..\Aquilinus\DCL_IDirect3DVertexBuffer9.h"
#include "..\Aquilinus\DCL_IDirect3DVertexDeclaration9.h"
#include "..\Aquilinus\DCL_IDirect3DVertexShader9.h"
#include "..\Aquilinus\DCL_IDirect3DVolume9.h"
#include "..\Aquilinus\DCL_IDirect3DVolumeTexture9.h"
#include "..\Aquilinus\DCL_IDirect3DDevice9ex.h"
// vmtables
#include "..\Aquilinus\VMT_IDirect3DBaseTexture9.h"
#include "..\Aquilinus\VMT_IDirect3DCubeTexture9.h"
#include "..\Aquilinus\VMT_IDirect3DDevice9.h"
#include "..\Aquilinus\VMT_IDirect3DIndexBuffer9.h"
#include "..\Aquilinus\VMT_IDirect3DPixelShader9.h"
#include "..\Aquilinus\VMT_IDirect3DQuery9.h"
#include "..\Aquilinus\VMT_IDirect3DResource9.h"
#include "..\Aquilinus\VMT_IDirect3DStateBlock9.h"
#include "..\Aquilinus\VMT_IDirect3DSurface9.h"
#include "..\Aquilinus\VMT_IDirect3DSwapChain9.h"
#include "..\Aquilinus\VMT_IDirect3DTexture9.h"
#include "..\Aquilinus\VMT_IDirect3DVertexBuffer9.h"
#include "..\Aquilinus\VMT_IDirect3DVertexDeclaration9.h"
#include "..\Aquilinus\VMT_IDirect3DVertexShader9.h"
#include "..\Aquilinus\VMT_IDirect3DVolume9.h"
#include "..\Aquilinus\VMT_IDirect3DVolumeTexture9.h"
#include "..\Aquilinus\VMT_IDirect3DDevice9Ex.h"
#pragma endregion

#pragma endregion

#pragma region AQU_d3d9 exported function
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion);
#pragma endregion

#pragma region AQU_d3d9 global fields
PCL_IDirect3DDevice9* g_pPCL_IDirect3DDevice9;
PCL_IDirect3D9*       g_pPCL_IDirect3D9;
HINSTANCE             g_hD3D9;
HINSTANCE             g_hInstance;
#pragma endregion

#pragma region Aquilinus global fields
AQU_TransferSite*       g_pAQU_TransferSite;    /**< The transfer site. **/
//AQU_WorkingArea*        g_pAQU_WorkingArea;     /**< The working area. **/
std::vector<NOD_Basic*> g_paNodes;              /**< The nodes vector used for complemented profiles. **/
int                     g_area_width  = 600;    /**< The default width of the working area window. **/
int                     g_area_height = 200;    /**< The default height of the working area window. **/
int                     g_profile_width  = 600; /**< The default width of the profile window. **/
int                     g_profile_height = 200; /**< The default height of the profile window. **/
#pragma endregion

#endif