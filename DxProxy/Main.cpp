/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Main.cpp> :
Copyright (C) 2012 Andres Hernandez

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

#define _CRT_SECURE_NO_WARNINGS
#include "Main.h"
#include "Direct3D9.h"
#include <windows.h>
#include <d3d9.h>
#include <stdio.h>

// Function pointer trypedefs
typedef IDirect3D9* (WINAPI *LPDirect3DCreate9)(UINT nSDKVersion);

typedef int (WINAPI *LPD3DPERF_BeginEvent)( D3DCOLOR col, LPCWSTR wszName );
typedef int (WINAPI *LPD3DPERF_EndEvent)( void );
typedef void (WINAPI *LPD3DPERF_SetMarker)( D3DCOLOR col, LPCWSTR wszName );
typedef void (WINAPI *LPD3DPERF_SetRegion)( D3DCOLOR col, LPCWSTR wszName );
typedef BOOL (WINAPI *LPD3DPERF_QueryRepeatFrame)( void );
typedef void (WINAPI *LPD3DPERF_SetOptions)( DWORD dwOptions );
typedef DWORD (WINAPI *LPD3DPERF_GetStatus)( void );

// Globals from d3d9.dll
HMODULE g_hDll = NULL;
LPDirect3DCreate9 g_pfnDirect3DCreate9 = NULL;

LPD3DPERF_BeginEvent g_pfnD3DPERF_BeginEvent = NULL;
LPD3DPERF_EndEvent g_pfnD3DPERF_EndEvent = NULL;
LPD3DPERF_SetMarker g_pfnD3DPERF_SetMarker = NULL;
LPD3DPERF_SetRegion g_pfnD3DPERF_SetRegion = NULL;
LPD3DPERF_QueryRepeatFrame g_pfnD3DPERF_QueryRepeatFrame = NULL;
LPD3DPERF_SetOptions g_pfnD3DPERF_SetOptions = NULL;
LPD3DPERF_GetStatus g_pfnD3DPERF_GetStatus = NULL;

static bool LoadDll()
{
	if(g_hDll)
		return true;

	// Load DLL
	char szBuff[MAX_PATH+64];
	GetSystemDirectory(szBuff, sizeof(szBuff));
	szBuff[MAX_PATH] = 0;
	strcat(szBuff, "\\d3d9.dll");
	g_hDll = LoadLibrary(szBuff);
	if(!g_hDll)
		return false;

	// Get function addresses
	g_pfnDirect3DCreate9 = (LPDirect3DCreate9)GetProcAddress(g_hDll, "Direct3DCreate9");
	if(!g_pfnDirect3DCreate9)
	{
		FreeLibrary(g_hDll);
		return false;
	}

	g_pfnD3DPERF_BeginEvent = (LPD3DPERF_BeginEvent)GetProcAddress(g_hDll, "D3DPERF_BeginEvent");
	g_pfnD3DPERF_EndEvent = (LPD3DPERF_EndEvent)GetProcAddress(g_hDll, "D3DPERF_EndEvent");
	g_pfnD3DPERF_SetMarker = (LPD3DPERF_SetMarker)GetProcAddress(g_hDll, "D3DPERF_SetMarker");
	g_pfnD3DPERF_SetRegion = (LPD3DPERF_SetRegion)GetProcAddress(g_hDll, "D3DPERF_SetRegion");
	g_pfnD3DPERF_QueryRepeatFrame = (LPD3DPERF_QueryRepeatFrame)GetProcAddress(g_hDll, "D3DPERF_QueryRepeatFrame");
	g_pfnD3DPERF_SetOptions = (LPD3DPERF_SetOptions)GetProcAddress(g_hDll, "D3DPERF_SetOptions");
	g_pfnD3DPERF_GetStatus = (LPD3DPERF_GetStatus)GetProcAddress(g_hDll, "D3DPERF_GetStatus");

	// Done
	return true;
}

IDirect3D9* WINAPI Direct3DCreate9(UINT nSDKVersion)
{
	// Log
	Log("Direct3DCreate9(%d)\n", nSDKVersion);

	// Load DLL
	if(!LoadDll())
		return NULL;

	// Create real interface
	IDirect3D9* pD3D = g_pfnDirect3DCreate9(nSDKVersion);
	if(!pD3D)
		return NULL;

	// Create and return proxy interface
	BaseDirect3D9* pWrapper = new BaseDirect3D9(pD3D);

	return pWrapper;
}

int WINAPI D3DPERF_BeginEvent( D3DCOLOR col, LPCWSTR wszName )
{
	return g_pfnD3DPERF_BeginEvent(col, wszName);
}

int WINAPI D3DPERF_EndEvent( void )
{
	return g_pfnD3DPERF_EndEvent();
}

void WINAPI D3DPERF_SetMarker( D3DCOLOR col, LPCWSTR wszName )
{
	g_pfnD3DPERF_SetMarker(col, wszName);
}

void WINAPI D3DPERF_SetRegion( D3DCOLOR col, LPCWSTR wszName )
{
	g_pfnD3DPERF_SetRegion(col, wszName);
}

BOOL WINAPI D3DPERF_QueryRepeatFrame( void )
{
	return g_pfnD3DPERF_QueryRepeatFrame();
}

void WINAPI D3DPERF_SetOptions( DWORD dwOptions )
{
	g_pfnD3DPERF_SetOptions(dwOptions);
}

DWORD WINAPI D3DPERF_GetStatus( void )
{
	return g_pfnD3DPERF_GetStatus();
}

void Log(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	static FILE* pFile = NULL;
	if(!pFile)
		pFile = fopen("C:/D3D9Proxy.log", "w");

	OutputDebugString(szBuff);
	OutputDebugString("\n");
	if(pFile) {
		fwrite(szBuff, 1, strlen(szBuff), pFile);
		fflush(pFile);
	} else {
		OutputDebugString("Couldn't open log file for writing.\n");
	}
}