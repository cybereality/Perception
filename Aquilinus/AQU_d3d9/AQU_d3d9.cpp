/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d9.cpp>
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

#include "AQU_d3d9.h"
#include "AQU_D3DPERF.h"

/**
* Main dll entry point.
***/
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: 
		g_hD3D9 = NULL;
		g_hInstance = NULL;
		g_pPCL_IDirect3D9 = NULL;
		g_pPCL_IDirect3DDevice9 = NULL;	
		g_hInstance = (HINSTANCE)hInstance;
		break;
	case DLL_PROCESS_DETACH:
		// release original d3d9.dll
		if (g_hD3D9)
		{
			FreeLibrary(g_hD3D9);
			g_hD3D9 = NULL;  
		}
		break;

	}
	return TRUE;
}

/**
* Creaetes proxy d3d9 object.
***/
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	// load original d3d9.dll
	if (!g_hD3D9)
	{
		char szSysPath[MAX_PATH];

		// get the dll path in system directory
		GetSystemDirectoryA(szSysPath,MAX_PATH);
		strcat_s(szSysPath,"\\d3d9.dll");

		// try to load original d3d9.dll
		g_hD3D9 = LoadLibraryA(szSysPath);

		if (!g_hD3D9)
		{
			OutputDebugString(L"Aquilinus : d3d9.dll could not be loaded !!");
			// TODO !! Handle this exception
			ExitProcess(0); 
		}
	}

	// set super methods
	typedef IDirect3D9 *(WINAPI* AQU_Direct3DCreate9)(UINT SDKVersion);
	AQU_Direct3DCreate9 AQU_Direct3DCreate9_Super = (AQU_Direct3DCreate9)GetProcAddress(g_hD3D9, "Direct3DCreate9");

	g_pD3DPERF_BeginEvent_Super = (LPD3DPERF_BeginEvent)GetProcAddress(g_hD3D9, "D3DPERF_BeginEvent");
	g_pD3DPERF_EndEvent_Super = (LPD3DPERF_EndEvent)GetProcAddress(g_hD3D9, "D3DPERF_EndEvent");
	g_pD3DPERF_SetMarker_Super = (LPD3DPERF_SetMarker)GetProcAddress(g_hD3D9, "D3DPERF_SetMarker");
	g_pD3DPERF_SetRegion_Super = (LPD3DPERF_SetRegion)GetProcAddress(g_hD3D9, "D3DPERF_SetRegion");
	g_pD3DPERF_QueryRepeatFrame_Super = (LPD3DPERF_QueryRepeatFrame)GetProcAddress(g_hD3D9, "D3DPERF_QueryRepeatFrame");
	g_pD3DPERF_SetOptions_Super = (LPD3DPERF_SetOptions)GetProcAddress(g_hD3D9, "D3DPERF_SetOptions");
	g_pD3DPERF_GetStatus_Super = (LPD3DPERF_GetStatus)GetProcAddress(g_hD3D9, "D3DPERF_GetStatus");

	if (!AQU_Direct3DCreate9_Super) 
	{
		OutputDebugString(L"Aquilinus : Could not find >Direct3DCreate9< address.");
		// TODO !! Handle this exception
		ExitProcess(0); 
	}

	// create actual device
	IDirect3D9 *pIDirect3D9 = AQU_Direct3DCreate9_Super(SDKVersion);

	g_hConfigMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		g_szMemoryPageName);   // name of the Aquilinus config

	// return if failed
	if (g_hConfigMapFile == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
	}

	// create map view
	g_pAquilinusConfig = (AquilinusCfg*) 
		MapViewOfFile(g_hConfigMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS,              // read/write permission
		0,
		0,
		sizeof(AquilinusCfg));

	// return if failed
	if (g_pAquilinusConfig == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not map view of file.\n"));
		CloseHandle(g_hConfigMapFile);
	}

	OutputDebugString(g_pAquilinusConfig->szAquilinusPath);

	// create the global transfer site class
	g_pAQU_TransferSite = new AQU_TransferSite(g_pAquilinusConfig);
	g_pAQU_TransferSite->InitD3DNodes();

	OutputDebugString(L"Aquilinus : initialized.... :");
	
	// create and return proxy device
	g_pPCL_IDirect3D9 = new PCL_IDirect3D9(pIDirect3D9, g_pAQU_TransferSite);

	return (g_pPCL_IDirect3D9);
}
