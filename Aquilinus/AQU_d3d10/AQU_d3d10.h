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