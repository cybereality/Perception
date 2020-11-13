/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d10.h>
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

#ifndef AQU_D3D10_HEAD
#define AQU_D3D10_HEAD

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <D3D10.h>
//#include <stdio.h>
#include "PCL_ID3D10Device.h"
#include "PCL_IDXGISwapChain.h"

#pragma region AQU_d3d10 exported function
/*D3D10CompileShader
D3D10DisassembleShader
D3D10GetGeometryShaderProfile
D3D10GetInputAndOutputSignatureBlob
D3D10GetInputSignatureBlob
D3D10GetOutputSignatureBlob
D3D10GetPixelShaderProfile
D3D10GetShaderDebugInfo
D3D10GetVertexShaderProfile
D3D10PreprocessShader
D3D10ReflectShader*/
HRESULT WINAPI D3D10CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D10Device** ppDevice);
HRESULT WINAPI D3D10CreateDevice(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, ID3D10Device** ppDevice);
HRESULT WINAPI D3D10CreateBlob(SIZE_T NumBytes, LPD3D10BLOB *ppBuffer);
#pragma endregion

#pragma region AQU_d3d9 global fields
PCL_ID3D10Device*   g_pPCL_ID3D10Device;
PCL_IDXGISwapChain* g_pPCL_IDXGISwapChain;
HMODULE             g_hD3D10;
#pragma endregion

#endif