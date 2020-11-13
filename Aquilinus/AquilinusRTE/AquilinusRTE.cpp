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
#include"AquilinusRTE.h"
#include"..\Inicio\Inicio.cpp"

/**
* Inits the Aquilinus configuration.
***/
extern "C" __declspec(dllexport) void Aquilinus_Init()
{
	OutputDebugString(L"Aquilinus Runtime Environment : Initialization...");

	// init
	InicioInit();

	// use the aquilinus dll path for the runtime environment data (plugins)
	g_pAquilinusConfig->bUseDllPath = true;

	// create and init direct draw 
	g_pDirectDraw = new AQU_Drawer(AQUILINUS_RTE_DRAWING_API);

	// register the new project window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProcProfileWindow;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Aquilinus-Inicio profile class";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// exit if failed
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Aquilinus Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// start idle
	g_eInicioStatus = InicioStatus::Idle;
	g_bControlActivated = false;
	g_bSubMenuOpen = false;
}

/**
* Close Aquilinus configuration.
***/
extern "C" __declspec(dllexport) void Aquilinus_Close()
{
	OutputDebugString(L"Aquilinus Runtime Environment : Close...");

	// close
	InicioClose();

	if (g_pDirectDraw) delete g_pDirectDraw;
	if (g_pDirectDrawSub) delete g_pDirectDrawSub;
	if (g_pDirectDrawProfile) delete g_pDirectDrawProfile;
}

/**
* Forces >Idle< state for Inicio closing any profile settings.
***/
extern "C" __declspec(dllexport) void Aquilinus_ForceIdle()
{
	ForceIdle();
}

/**
* Load Aquilinus profile.
***/
extern "C" __declspec(dllexport) void Aquilinus_LoadProfile(LPCWSTR szPath, LPCWSTR szProcess, LPCWSTR* szPathReturn, DWORD unRepetition, DWORD unTimedelay)
{
	static std::wstring szPathRet;

	// force idle
	ForceIdle();

	// set to init until loading window finished
	g_eInicioStatus = InicioStatus::Initial;

	// set D3D9Ex device meanwhile to false
	g_pAquilinusConfig->bCreateD3D9Ex = FALSE;

	// get the current directory
	wchar_t szCurrentPath[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, szCurrentPath);

	// load the working area basics to inject
	DWORD dwSupportedInterfacesNumber;
	if (SUCCEEDED(g_pFileManager->LoadProfileBasics(szPath, g_pAquilinusConfig, dwSupportedInterfacesNumber, g_pPictureData, g_dwPictureSize)))
	{
		if (g_pAquilinusConfig->bEmptyProcess)
		{
			std::wstring szP = std::wstring(szProcess);
			CopyMemory(g_pAquilinusConfig->szEntryName, szP.c_str(), (szP.length() + 1) * sizeof(wchar_t));
			CopyMemory(g_pAquilinusConfig->szProcessName, szP.c_str(), (szP.length() + 1) * sizeof(wchar_t));
		}

		// output debug data...
		OutputDebugStringW(L"Aquilinus : Start to inject to....");
		OutputDebugStringW(g_pAquilinusConfig->szEntryName);
		OutputDebugStringW(g_pAquilinusConfig->szProfileFilePath);
		szPathRet = std::wstring(g_pAquilinusConfig->szProfileFilePath);

		// ensure to be back in old path before starting the injection thread
		SetCurrentDirectoryW(szCurrentPath);

		// override repetition and time delay
		g_nRepeat = (int)(unRepetition & 15);
		g_pAquilinusConfig->dwDetourTimeDelay = unTimedelay;

		// create the thread for injection
		g_eInicioStatus = InicioStatus::Injecting;
		g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::Complemented;
		g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
		if (g_hInjectionThread == NULL)
			OutputDebugString(L"Aquilinus : Failed to create injection thread !");

		// picture present ?
		if (g_pAquilinusConfig->bProfileWindow)
		{
			// ensure that the image path is zero, so it will use the picture data instead
			g_pAquilinusConfig->szPictureFilePath[0] = 0;

			// create the thread to override the virtual methods table
			if (CreateThread(NULL, 0, ProfileWindowMainThread, NULL, 0, NULL) == NULL)
				OutputDebugString(L"Aquilinus : Failed to create profile window thread !");
		}
	}
	else
	{
		OutputDebugString(L"Aquilinus : Load profile failed!");
		g_eInicioStatus = InicioStatus::Idle;
	}

	if (szPathReturn)
		*szPathReturn = &szPathRet.c_str()[0];
}

/**
* Inject into the loaded profile again.
***/
extern "C" __declspec(dllexport) void Aquilinus_Reinject()
{
	// force idle
	ForceIdle();

	// get the current directory
	wchar_t szCurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentPath);

	// output debug data...
	OutputDebugString(L"Aquilinus : Start to inject to....");
	//LPWSTR szName = g_pFileManager->GetName(g_pAquilinusConfig->dwProcessIndex);
	OutputDebugString(g_pAquilinusConfig->szEntryName);
	OutputDebugString(g_pAquilinusConfig->szProfileFilePath);

	// ensure to be back in old path before starting the injection thread
	SetCurrentDirectory(szCurrentPath);

	// create the thread for injection
	g_eInicioStatus = InicioStatus::Injecting;
	g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::Complemented;
	g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
	g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
	if (g_hInjectionThread == NULL)
		OutputDebugString(L"Aquilinus : Failed to create injection thread !");

	// picture present ?
	if (g_pAquilinusConfig->bProfileWindow)
	{
		// ensure that the image path is zero, so it will use the picture data instead
		g_pAquilinusConfig->szPictureFilePath[0] = 0;

		// create the thread to override the virtual methods table
		if (CreateThread(NULL, 0, ProfileWindowMainThread, NULL, 0, NULL) == NULL)
			OutputDebugString(L"Aquilinus : Failed to create profile window thread !");
	}

}

/**
* Get the current state of the injection.
***/
extern "C" __declspec(dllexport) int Aquilinus_GetInjectionState()
{
	if (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::ComplementedInjected) return 4;
	if (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::Closed) return 5;
	return (int)g_eInicioStatus;
}
