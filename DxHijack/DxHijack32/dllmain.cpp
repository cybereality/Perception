/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <dllmain.cpp> :
Copyright (C) 2012 by Andres Hernandez

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "hijackdll.h"
#include "apihijack.h"
#include "Direct3D9.h"
#include "Direct3D9Ex.h"
#include <string>

// Text buffer for sprintf
char targetExe[256];
char targetPath[256];
char lastExe[256];
std::string targetPathString;

LPCSTR realDll = "D3D9.DLL";
LPCWSTR realDllW = L"D3D9.DLL";
LPCSTR proxyDll = NULL;
LPCWSTR proxyDllW = NULL;
LPCSTR dllDir = NULL;

HINSTANCE hDLL;
void ParsePaths();

#ifndef x64

typedef HMODULE (WINAPI *LoadLibraryExW_Type)(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);   

HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);   

typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT sdk_version);
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version);

typedef HRESULT (WINAPI *Direct3DCreate9Ex_t)(UINT sdk_version, IDirect3D9Ex**);
HRESULT WINAPI MyDirect3DCreate9Ex(UINT sdk_version, IDirect3D9Ex**);



// Hook structure.
enum
{
	KERNEL32_LoadLibraryExW = 3
};

enum
{
	D3DFN_Direct3DCreate9 = 0,
	D3DFN_Direct3DCreate9Ex
};

SDLLHook KernelHook = 
{
	"KERNEL32.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "LoadLibraryExW", MyLoadLibraryExW },   
		{ NULL, NULL }
	}
};

SDLLHook D3DHook = 
{
	"D3D9.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "Direct3DCreate9", MyDirect3DCreate9},
		{ "Direct3DCreate9Ex", MyDirect3DCreate9Ex},
		{ NULL, NULL }
	}
};

HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)   
{   
	OutputDebugString("LoadLibraryExW ENTERED!\n");

	// Get the old function    
	LoadLibraryExW_Type OldFn = (LoadLibraryExW_Type)KernelHook.Functions[KERNEL32_LoadLibraryExW].OrigFn;    

	// A Place to store the module, that is returned    
	HMODULE retval;    

	OutputDebugString("LoadLibraryExW( lpLibFileName ");    
	OutputDebugStringW(lpFileName);    
	OutputDebugString(" )\n");    

	if(lstrcmpiW(lpFileName, realDllW) == 0)
	{
		OutputDebugString("Inject Proxy DLL ");    
		lpFileName = proxyDllW;
		OutputDebugStringW(lpFileName); 
		OutputDebugString("\n");
	} 

	// Time to call the original function    
	retval = OldFn(lpFileName, hFile, dwFlags);    

	//HookAPICalls(&KernelHook, retval);    

	return retval;   
}

IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version)
{
	OutputDebugString( "Direct3D-Hook: MyDirect3DCreate9 called.\n" );

	Direct3DCreate9_t old_func = (Direct3DCreate9_t) D3DHook.Functions[D3DFN_Direct3DCreate9].OrigFn;
	IDirect3D9* d3d = old_func(sdk_version);

	return d3d ? new BaseDirect3D9(d3d) : 0;
}

HRESULT WINAPI MyDirect3DCreate9Ex(UINT sdk_version, IDirect3D9Ex**p)
{
	OutputDebugString( "Direct3D-Hook: MyDirect3DCreate9Ex called.\n" );

	//Ex is not currently supported
	return ERROR_NOT_SUPPORTED;
}
#endif //!x64

bool fileExists(std::string file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file.c_str(), &FindFileData) ;
   bool found = handle != INVALID_HANDLE_VALUE;
   if(found) 
       FindClose(handle);
   return found;
}

// CBT Hook-style injection.
BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved )
{
	if (fdwReason == DLL_PROCESS_ATTACH)  // When initializing....
	{
		hDLL = hModule;

		// We don't need thread notifications for what we're doing.  Thus, get
		// rid of them, thereby eliminating some of the overhead of this DLL
		DisableThreadLibraryCalls(hModule);

		// Only hook the APIs if this is the right process.
		GetModuleFileName(GetModuleHandle(NULL), targetExe, sizeof(targetExe));
		PathStripPath(targetExe);
		/*if(std::string(targetExe) == "GitHub.exe")
		{
			OutputDebugString("Ignoring process: ");
			OutputDebugString(targetExe);
			OutputDebugString("\n");
			return TRUE;
		}*/
		GetModuleFileName(GetModuleHandle(NULL), targetPath, sizeof(targetPath));
		targetPathString = std::string(targetPath);
		targetPathString = targetPathString.substr(0, targetPathString.find_last_of("\\/") + 1);

		OutputDebugString("HIJACKDLL checking process: ");
		OutputDebugString(targetExe);
		OutputDebugString("\n");


#ifndef x64
		ParsePaths();
#endif
		ProxyHelper helper = ProxyHelper();

		if (helper.HasProfile(targetExe, targetPathString.c_str()))
		{
#ifndef x64
			//Need to check that the d3d9.dll is actually in the game folder - If it is, then we don't need to hook API calls
			//using the methods below as d3d9 will just be loaded by the game's executable
			if (!fileExists(targetPathString + "D3D9.dll"))
			{
				if (HookAPICalls(&D3DHook))
				{
					OutputDebugString("HookAPICalls(D3D): TRUE\n");
				} 
				else if(HookAPICalls(&KernelHook))
				{	
					OutputDebugString("HookAPICalls(Kernel): TRUE\n");
				} 
				else 
				{
					OutputDebugString("HookAPICalls(Both): FALSE\n");
				}

				SetDllDirectory(dllDir);
			}
			else
			{
				OutputDebugString(std::string("D3D9.dll found in game directory (" + targetPathString + ") - Bypassing API injection").c_str());
			}
#endif 
		}
	}

	return TRUE;
}

//Implemented for 32-bit only 
#ifndef x64

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hHook = NULL;
#pragma data_seg ()

HIJACKDLL_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	return CallNextHookEx(hHook, nCode, wParam, lParam); 
}

HIJACKDLL_API void InstallHook()
{
	OutputDebugString( "HIJACKDLL hook installed.\n" );
	hHook = SetWindowsHookEx( WH_CBT, HookProc, hDLL, 0 );
}

HIJACKDLL_API void RemoveHook()
{
	OutputDebugString( "HIJACKDLL hook removed.\n" );
	UnhookWindowsHookEx( hHook );
}

void ParsePaths()
{
	dllDir = (LPCSTR)malloc(512*sizeof(char));
	proxyDll = (LPCSTR)malloc(512*sizeof(char));
	proxyDllW = (LPCWSTR)malloc(512*sizeof(wchar_t));

	ProxyHelper helper = ProxyHelper();
	helper.GetPath((char*)dllDir, "bin\\");
	helper.GetPath((char*)proxyDll, "bin\\d3d9.dll");
	mbstowcs_s(NULL, (wchar_t*)proxyDllW, 512, proxyDll, 512);
}
#endif
