/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d9.h>
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
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