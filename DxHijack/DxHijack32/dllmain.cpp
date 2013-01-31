/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
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

typedef HMODULE (WINAPI *LoadLibrary_Type)(LPCSTR lpLibFileName);    
typedef HMODULE (WINAPI *LoadLibraryW_Type)(LPCWSTR lpLibFileName);  
typedef HMODULE (WINAPI *LoadLibraryEx_Type)(LPCTSTR lpFileName, HANDLE hFile, DWORD dwFlags);     
typedef HMODULE (WINAPI *LoadLibraryExW_Type)(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);   

HMODULE WINAPI MyLoadLibraryA(LPCSTR lpLibFileName);    
HMODULE WINAPI MyLoadLibraryW(LPCWSTR lpLibFileName);    
HMODULE WINAPI MyLoadLibraryExA(LPCTSTR lpFileName, HANDLE hFile, DWORD dwFlags);   
HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);   

typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT sdk_version);
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version);

void ParsePaths();
void SaveExeName(char*);

// Hook structure.
enum
{
    KERNEL32_LoadLibraryA = 0,   
    KERNEL32_LoadLibraryW = 1,   
    KERNEL32_LoadLibraryExA = 2,   
    KERNEL32_LoadLibraryExW = 3
};

enum
{
    D3DFN_Direct3DCreate9 = 0
};

SDLLHook KernelHook = 
{
    "KERNEL32.DLL",
    false, NULL,		// Default hook disabled, NULL function pointer.
    {
		{ "LoadLibraryA", MyLoadLibraryA },    
        { "LoadLibraryW", MyLoadLibraryW },   
        { "LoadLibraryExA", MyLoadLibraryExA },    
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
        { NULL, NULL }
    }
};
  
HMODULE WINAPI MyLoadLibraryA(LPCSTR lpLibFileName)    
{
	OutputDebugString("LoadLibraryA ENTERED!");

    // Get the old function   
    LoadLibrary_Type OldFn = (LoadLibrary_Type)KernelHook.Functions[KERNEL32_LoadLibraryA].OrigFn;    
   
    // A Place to store the module, that is returned    
    HMODULE retval;    
   
    OutputDebugString("LoadLibraryA( lpLibFileName ");    
    OutputDebugString(lpLibFileName);    
    OutputDebugString(" )\n");    

	if(lstrcmpi(lpLibFileName, realDll) == 0)
	{
		OutputDebugString("Inject Proxy DLL");    
		lpLibFileName = proxyDll;
		OutputDebugString(lpLibFileName); 
	} 
   
    // Time to call the original function    
    retval = OldFn(lpLibFileName);    
   
    //HookAPICalls(&KernelHook, retval);    
       
    return retval;   
}    
   
HMODULE WINAPI MyLoadLibraryW(LPCWSTR lpLibFileName)    
{    
	OutputDebugString("LoadLibraryW ENTERED!");

    // Get the old function    
    LoadLibraryW_Type OldFn = (LoadLibraryW_Type)KernelHook.Functions[KERNEL32_LoadLibraryW].OrigFn;    
   
    // A Place to store the module, that is returned    
    HMODULE retval;    
   
    OutputDebugString("LoadLibraryW( lpLibFileName ");    
    OutputDebugStringW(lpLibFileName);    
    OutputDebugString(" )\n");    

	if(lstrcmpiW(lpLibFileName, realDllW) == 0)
	{
		OutputDebugString("Inject Proxy DLL");    
		lpLibFileName = proxyDllW;
		OutputDebugStringW(lpLibFileName); 
	} 
   
    // Time to call the original function    
    retval = OldFn(lpLibFileName);    
   
    //HookAPICalls(&KernelHook, retval);    
       
    return retval;   
}    
   
HMODULE WINAPI MyLoadLibraryExA(LPCTSTR lpFileName, HANDLE hFile, DWORD dwFlags)   
{   
	OutputDebugString("LoadLibraryExA ENTERED!");

    // Get the old function    
    LoadLibraryEx_Type OldFn = (LoadLibraryEx_Type)KernelHook.Functions[KERNEL32_LoadLibraryExA].OrigFn;    
   
    // A Place to store the module, that is returned    
    HMODULE retval;    
   
    OutputDebugString("LoadLibraryExA( lpLibFileName ");    
    OutputDebugString(lpFileName);    
    OutputDebugString(" )\n");    

	if(lstrcmpi(lpFileName, realDll) == 0)
	{
		OutputDebugString("Inject Proxy DLL");    
		lpFileName = proxyDll;
		OutputDebugString(lpFileName); 
	} 
   
    // Time to call the original function    
    retval = OldFn(lpFileName, hFile, dwFlags);    
   
    //HookAPICalls(&KernelHook, retval);    
       
    return retval;   
}   
   
HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)   
{   
	OutputDebugString("LoadLibraryExW ENTERED!");

    // Get the old function    
    LoadLibraryExW_Type OldFn = (LoadLibraryExW_Type)KernelHook.Functions[KERNEL32_LoadLibraryExW].OrigFn;    
   
    // A Place to store the module, that is returned    
    HMODULE retval;    
   
    OutputDebugString("LoadLibraryExW( lpLibFileName ");    
    OutputDebugStringW(lpFileName);    
    OutputDebugString(" )\n");    

	if(lstrcmpiW(lpFileName, realDllW) == 0)
	{
		OutputDebugString("Inject Proxy DLL");    
		lpFileName = proxyDllW;
		OutputDebugStringW(lpFileName); 
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

		GetModuleFileName(GetModuleHandle(NULL), targetPath, sizeof(targetPath));
		targetPathString = std::string(targetPath);
		targetPathString = targetPathString.substr(0, targetPathString.find_last_of("\\/"));

        OutputDebugString("HIJACKDLL checking process: ");
        OutputDebugString(targetExe);
        OutputDebugString("\n");

		ParsePaths();
		ProxyHelper helper = ProxyHelper();

        if (helper.HasProfile(targetExe))
		{
			if (HookAPICalls(&D3DHook))
			{
				OutputDebugString("HookAPICalls(D3D): TRUE");
			} 
			else if(HookAPICalls(&KernelHook))
			{	
				OutputDebugString("HookAPICalls(Kernel): TRUE");
			} 
			else 
			{
				OutputDebugString("HookAPICalls(Both): FALSE");
			}

			SetDllDirectory(dllDir);
			SaveExeName(targetExe);
		}
    }

    return TRUE;
}

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

void SaveExeName(char* data)
{
	HKEY hKey;
    LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

    LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS , &hKey);

    if (openRes==ERROR_SUCCESS) {
        OutputDebugString("Hx // Success opening key.");
    } else {
        OutputDebugString("Hx // Error opening key.");
    }

	LPCTSTR value = TEXT("TargetExe");

	LONG setRes = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)data, strlen(data)+1);

	if (setRes == ERROR_SUCCESS) {
		OutputDebugString("Hx // Success writing to Registry.");
	} else {
		OutputDebugString("Hx // Error writing to Registry.");
	}

    LONG closeOut = RegCloseKey(hKey);

    if (closeOut == ERROR_SUCCESS) {
        OutputDebugString("Hx // Success closing key.");
    } else {
        OutputDebugString("Hx // Error closing key.");
    }
}