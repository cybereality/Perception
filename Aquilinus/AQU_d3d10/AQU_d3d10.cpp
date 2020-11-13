/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d10.cpp>
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

#include "AQU_d3d10.h"
#include "AQU_d3d10shader.h"

/**
* Main dll entry point.
***/
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hD3D10 = NULL;
		g_pPCL_ID3D10Device = NULL;
		break;
	case DLL_PROCESS_DETACH:
		// release original d3d10.dll
		if (g_hD3D10)
		{
			FreeLibrary(g_hD3D10);
			g_hD3D10 = NULL;
		}
		break;
	}
	return TRUE;
}

/**
* Creates proxy d3d10 device.
***/
HRESULT WINAPI D3D10CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
											 UINT SDKVersion, DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D10Device** ppDevice)
{
	OutputDebugString("D3D10CreateDeviceAndSwapChain");
	// load original d3d10.dll
	if (!g_hD3D10)
	{
		char cBuf[MAX_PATH];

		// get the dll path in system directory
		GetSystemDirectory(cBuf, MAX_PATH);
		strcat_s(cBuf, MAX_PATH, "\\d3d10.dll");

		// try to load original d3d9.dll
		g_hD3D10 = LoadLibrary(cBuf);

		if (!g_hD3D10)
		{
			OutputDebugString("Aquilinus : d3d10.dll could not be loaded !!");
			OutputDebugString(cBuf);
			// TODO !! Handle this exception
			ExitProcess(0); 
		}
	}

	// declare proxy device and class extern
	extern PCL_ID3D10Device* g_pPCL_ID3D10Device;
	extern PCL_IDXGISwapChain* g_pPCL_IDXGISwapChain;

	// set super methods
	typedef HRESULT (WINAPI* AQU_D3D10CreateDeviceAndSwapChain)(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D10Device** ppDevice);
	AQU_D3D10CreateDeviceAndSwapChain AQU_D3D10CreateDeviceAndSwapChain_Super = (AQU_D3D10CreateDeviceAndSwapChain) GetProcAddress(g_hD3D10, "D3D10CreateDeviceAndSwapChain");

	g_pD3D10CompileShader_Super = (LPD3D10CompileShader)GetProcAddress(g_hD3D10, "D3D10CompileShader");
	g_pD3D10DisassembleShader_Super = (LPD3D10DisassembleShader)GetProcAddress(g_hD3D10, "D3D10DisassembleShader");
	g_pD3D10GetPixelShaderProfile_Super = (LPD3D10GetPixelShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetPixelShaderProfile");
	g_pD3D10GetVertexShaderProfile_Super = (LPD3D10GetVertexShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetVertexShaderProfile");
	g_pD3D10GetGeometryShaderProfile_Super = (LPD3D10GetGeometryShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetGeometryShaderProfile");
	g_pD3D10ReflectShader_Super = (LPD3D10ReflectShader)GetProcAddress(g_hD3D10, "D3D10ReflectShader");
	g_pD3D10PreprocessShader_Super = (LPD3D10PreprocessShader)GetProcAddress(g_hD3D10, "D3D10PreprocessShader");
	g_pD3D10GetInputSignatureBlob_Super = (LPD3D10GetInputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetInputSignatureBlob");
	g_pD3D10GetOutputSignatureBlob_Super = (LPD3D10GetOutputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetOutputSignatureBlob");
	g_pD3D10GetInputAndOutputSignatureBlob_Super = (LPD3D10GetInputAndOutputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetInputAndOutputSignatureBlob");
	g_pD3D10GetShaderDebugInfo_Super = (LPD3D10GetShaderDebugInfo)GetProcAddress(g_hD3D10, "D3D10GetShaderDebugInfo");

	if (!AQU_D3D10CreateDeviceAndSwapChain_Super) 
	{
		OutputDebugString("Aquilinus : Could not find >D3D10CreateDeviceAndSwapChain< address.");
		// TODO !! Handle this exception
		ExitProcess(0); 
	}

	// create actual device
	HRESULT hr = AQU_D3D10CreateDeviceAndSwapChain_Super(pAdapter, DriverType, Software, Flags, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice);

	g_pPCL_ID3D10Device = new PCL_ID3D10Device(reinterpret_cast<ID3D10Device*>(*ppDevice));
	g_pPCL_IDXGISwapChain = new PCL_IDXGISwapChain(reinterpret_cast<IDXGISwapChain*>(*ppSwapChain));
	*ppDevice = (ID3D10Device*)g_pPCL_ID3D10Device;
	*ppSwapChain = (IDXGISwapChain*)g_pPCL_IDXGISwapChain;

	return hr;
}

/**
* Creates proxy d3d10 device.
***/
HRESULT WINAPI D3D10CreateDevice(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, 
								 ID3D10Device** ppDevice)
{
	OutputDebugString("D3D10CreateDevice");
	// load original d3d10.dll
	if (!g_hD3D10)
	{
		char cBuf[MAX_PATH];

		// get the dll path in system directory
		GetSystemDirectory(cBuf, MAX_PATH);
		strcat_s(cBuf,"\\d3d10.dll");

		// try to load original d3d9.dll
		g_hD3D10 = LoadLibrary(cBuf);

		if (!g_hD3D10)
		{
			OutputDebugString("Aquilinus : d3d10.dll could not be loaded !!");
			OutputDebugString(cBuf);
			// TODO !! Handle this exception
			ExitProcess(0); 
		}
	}

	// declare proxy device extern
	extern PCL_ID3D10Device* g_pPCL_ID3D10Device;

	// set super methods
	typedef HRESULT (WINAPI* AQU_D3D10CreateDevice)(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, ID3D10Device** ppDevice);
	AQU_D3D10CreateDevice AQU_D3D10CreateDevice_Super = (AQU_D3D10CreateDevice) GetProcAddress(g_hD3D10, "D3D10CreateDevice");

	g_pD3D10CompileShader_Super = (LPD3D10CompileShader)GetProcAddress(g_hD3D10, "D3D10CompileShader");
	g_pD3D10DisassembleShader_Super = (LPD3D10DisassembleShader)GetProcAddress(g_hD3D10, "D3D10DisassembleShader");
	g_pD3D10GetPixelShaderProfile_Super = (LPD3D10GetPixelShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetPixelShaderProfile");
	g_pD3D10GetVertexShaderProfile_Super = (LPD3D10GetVertexShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetVertexShaderProfile");
	g_pD3D10GetGeometryShaderProfile_Super = (LPD3D10GetGeometryShaderProfile)GetProcAddress(g_hD3D10, "D3D10GetGeometryShaderProfile");
	g_pD3D10ReflectShader_Super = (LPD3D10ReflectShader)GetProcAddress(g_hD3D10, "D3D10ReflectShader");
	g_pD3D10PreprocessShader_Super = (LPD3D10PreprocessShader)GetProcAddress(g_hD3D10, "D3D10PreprocessShader");
	g_pD3D10GetInputSignatureBlob_Super = (LPD3D10GetInputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetInputSignatureBlob");
	g_pD3D10GetOutputSignatureBlob_Super = (LPD3D10GetOutputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetOutputSignatureBlob");
	g_pD3D10GetInputAndOutputSignatureBlob_Super = (LPD3D10GetInputAndOutputSignatureBlob)GetProcAddress(g_hD3D10, "D3D10GetInputAndOutputSignatureBlob");
	g_pD3D10GetShaderDebugInfo_Super = (LPD3D10GetShaderDebugInfo)GetProcAddress(g_hD3D10, "D3D10GetShaderDebugInfo");

	// TODO !! test super method pointers

	// create actual device
	HRESULT hr = AQU_D3D10CreateDevice_Super(pAdapter, DriverType, Software, Flags, SDKVersion, ppDevice);

	g_pPCL_ID3D10Device = new PCL_ID3D10Device(reinterpret_cast<ID3D10Device*>(*ppDevice)); 
	*ppDevice = (ID3D10Device*)g_pPCL_ID3D10Device;

	return hr;
}

/**
* Calls original D3D10CreateBlob method.
***/
HRESULT WINAPI D3D10CreateBlob(SIZE_T NumBytes, LPD3D10BLOB *ppBuffer)
{
	OutputDebugString("D3D10CreateBlob");
	// load original d3d10.dll
	if (!g_hD3D10)
	{
		char cBuf[MAX_PATH];

		// get the dll path in system directory
		GetSystemDirectory(cBuf, MAX_PATH);
		strcat_s(cBuf, MAX_PATH, "\\d3d10.dll");

		// try to load original d3d9.dll
		g_hD3D10 = LoadLibrary(cBuf);

		if (!g_hD3D10)
		{
			OutputDebugString("Aquilinus : d3d10.dll could not be loaded !!");
			OutputDebugString(cBuf);
			// TODO !! Handle this exception
			ExitProcess(0); 
		}
	}

	// set super methods
	typedef HRESULT (WINAPI* AQU_D3D10CreateBlob)(SIZE_T NumBytes, LPD3D10BLOB *ppBuffer);
	AQU_D3D10CreateBlob AQU_D3D10CreateBlob_Super = (AQU_D3D10CreateBlob) GetProcAddress(g_hD3D10, "D3D10CreateBlob");

	// call super method
	HRESULT hr = AQU_D3D10CreateBlob_Super(NumBytes, ppBuffer);

	return hr;
}