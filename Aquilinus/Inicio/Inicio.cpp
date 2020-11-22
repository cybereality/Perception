/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio
Copyright � 2014 Denis Reischl

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
#include "Inicio.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }

#pragma region Inicio GUI methods

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

/**
* Main window control
***/
void ImGui_Main()
{
	ImVec2 sPos = { float(g_nMainWindowWidth) * .33f, float(g_nMainWindowHeight) * .45f };
	ImGui::SetCursorScreenPos(sPos);
	ImVec2 sSize = { float(g_nMainWindowWidth) * .2f,  float(g_nMainWindowHeight) * .18f };
	if (ImGui::Button("New", sSize))
	{
		// force idle
		ForceIdle();

		// enumerate game names
		EnumerateGameNames();

		// set new project window
		g_eCurrentWindow = InicioWindows::NewProject;
		g_bWindowResize = true;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Create new Vireio Perception Game Profile.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load", sSize))
	{
		// force idle
		ForceIdle();

		// get the current directory
		wchar_t szCurrentPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szCurrentPath);

		// keep the "old" process name ?
		DWORD dwProcessIndexOld = 0;
		DWORD dwDetourTimeDelay = 0;
		if (g_bKeepProcessName)
		{
			dwProcessIndexOld = g_pAquilinusConfig->dwProcessIndex;
			dwDetourTimeDelay = g_pAquilinusConfig->dwDetourTimeDelay;
		}

		// load the working area basics to inject
		DWORD dwSupportedInterfacesNumber;
		if (SUCCEEDED(g_pFileManager->LoadWorkingAreaBasics(g_pAquilinusConfig->szWorkspaceFilePath,
			g_pAquilinusConfig->dwProcessIndex,
			dwSupportedInterfacesNumber,
			(int*)g_pAquilinusConfig->eInjectionTechnique,
			g_pAquilinusConfig->szPictureFilePath,
			g_pAquilinusConfig->bProfileWindow,
			g_pAquilinusConfig->dwDetourTimeDelay,
			g_bKeepProcessName)))
		{
			// keep the "old" process name ?
			if (g_bKeepProcessName)
			{
				g_pAquilinusConfig->dwProcessIndex = dwProcessIndexOld;
				g_pAquilinusConfig->dwDetourTimeDelay = dwDetourTimeDelay;
			}

			// output debug data...
			OutputDebugString(L"Aquilinus : Start to inject to....");
			LPWSTR szName = g_pFileManager->GetName(g_pAquilinusConfig->dwProcessIndex);
			OutputDebugString(szName);
			OutputDebugString(g_pAquilinusConfig->szWorkspaceFilePath);

			// ensure to be back in old path before starting the injection thread
			SetCurrentDirectory(szCurrentPath);

			// create the abort event
			if (g_hInjectionAbortEvent) CloseHandle(g_hInjectionAbortEvent);
			g_hInjectionAbortEvent = CreateEvent(
				NULL,               // default security attributes
				TRUE,               // manual-reset event
				FALSE,              // initial state is nonsignaled
				TEXT("Abort")  // object name
			);

			// create the thread
			g_eInicioStatus = InicioStatus::Injecting;
			g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingAreaLoad;
			g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
			g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
			if (g_hInjectionThread == NULL)
				OutputDebugString(L"Aquilinus : Failed to create injection thread !");
		}
		else
		{
			OutputDebugString(L"Aquilinus : Load working area failed!");
			g_eInicioStatus = InicioStatus::Idle;
		}
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Load Game Profile Workspace.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::Button("Compiled", sSize))
	{
		// force idle
		ForceIdle();

		// get the current directory
		wchar_t szCurrentPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szCurrentPath);

		// load the profile basics to inject
		DWORD dwSupportedInterfacesNumber;
		if (SUCCEEDED(g_pFileManager->LoadProfileBasics(nullptr, g_pAquilinusConfig, dwSupportedInterfacesNumber, g_pPictureData, g_dwPictureSize)))
		{
			if (g_pAquilinusConfig->bEmptyProcess)
			{
				std::wstring szP = g_pFileManager->GetProcessName(g_pAquilinusConfig->dwProcessIndex);
				std::wstring szN = g_pFileManager->GetName(g_pAquilinusConfig->dwProcessIndex);
				CopyMemory(g_pAquilinusConfig->szEntryName, szN.c_str(), szN.length() + 1);
				CopyMemory(g_pAquilinusConfig->szProcessName, szP.c_str(), szP.length() + 1);
			}

			// output debug data...
			OutputDebugString(L"Aquilinus : Start to inject to....");
			OutputDebugString(g_pAquilinusConfig->szEntryName);
			OutputDebugString(g_pAquilinusConfig->szProfileFilePath);

			// ensure to be back in old path before starting the injection thread
			SetCurrentDirectory(szCurrentPath);

			// create the abort event
			if (g_hInjectionAbortEvent) CloseHandle(g_hInjectionAbortEvent);
			g_hInjectionAbortEvent = CreateEvent(
				NULL,               // default security attributes
				TRUE,               // manual-reset event
				FALSE,              // initial state is nonsignaled
				TEXT("Abort")  // object name
			);

			// create the thread for injection
			g_eInicioStatus = InicioStatus::Injecting;
			g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::Complemented;
			g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
			g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
			if (g_hInjectionThread == NULL)
				OutputDebugString(L"Aquilinus : Failed to create injection thread !");

			// thumbnail present ?
			wchar_t szPNGPath[MAX_PATH];
			size_t nLen = wcslen(g_pAquilinusConfig->szProfileFilePath);
			CopyMemory(szPNGPath, g_pAquilinusConfig->szProfileFilePath, nLen * sizeof(wchar_t));
			szPNGPath[nLen - 4] = 'p';
			szPNGPath[nLen - 3] = 'n';
			szPNGPath[nLen - 2] = 'g';
			szPNGPath[nLen - 1] = 0;
			wchar_t buf[128];
			wsprintf(buf, L"LEN %u", nLen);
			OutputDebugString(buf);
			OutputDebugString(g_pAquilinusConfig->szProfileFilePath);
			OutputDebugString(szPNGPath);
			//g_pDirectDraw->LoadBackground(szPNGPath, 256, 64);

		}
		else
		{
			OutputDebugString(L"Aquilinus : Load profile failed!");
			g_eInicioStatus = InicioStatus::Idle;
		}
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Load Compiled Vireio Perception Game Profile.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	sPos = { float(g_nMainWindowWidth) * .33f, float(g_nMainWindowHeight) * .65f };
	ImGui::SetCursorScreenPos(sPos);
	if (ImGui::Button("Options", sSize))
	{
		// force idle
		ForceIdle();

		// set options window
		g_eCurrentWindow = InicioWindows::Options;
		g_bWindowResize = true;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Aquilinus Application Options.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::Button("Info", sSize))
	{
		// force idle
		ForceIdle();

		// set options window
		g_eCurrentWindow = InicioWindows::Info;
		g_bWindowResize = true;
	}
	// status text
	sPos = { float(g_nMainWindowWidth) * .01f, float(g_nMainWindowHeight) * .9f };
	ImGui::SetCursorScreenPos(sPos);
	switch (g_eInicioStatus)
	{
	case InicioStatus::Idle:
		ImGui::Text("Idle");
		break;
	case InicioStatus::Injecting:
		ImGui::Text("Injecting..");
		break;
	case InicioStatus::ToInject:
		ImGui::Text("Start to inject");
		break;
	}
	sPos = { float(g_nMainWindowWidth) * .91f, float(g_nMainWindowHeight) * .9f };
	ImGui::SetCursorScreenPos(sPos);
	ImGui::Text("v%d.%d.%d", g_eVersion.dwPrima, g_eVersion.dwSecondo, g_eVersion.dwTerzo);
}

/**
* New project window control.
***/
void ImGui_New()
{
	if (ImGui::CollapsingHeader("Select Game"))
	{
		// 0AB CDE FGH IJK LMN OPQ RST UVW XYZ
		INT nSelect = -1;
		ImGui::BeginGroup();
		if (ImGui::Button("0", ImVec2(40, 40))) nSelect = 0;
		if (ImGui::Button("C", ImVec2(40, 40))) nSelect = 3;
		if (ImGui::Button("F", ImVec2(40, 40))) nSelect = 6;
		if (ImGui::Button("I", ImVec2(40, 40))) nSelect = 9;
		if (ImGui::Button("L", ImVec2(40, 40))) nSelect = 12;
		if (ImGui::Button("O", ImVec2(40, 40))) nSelect = 15;
		if (ImGui::Button("R", ImVec2(40, 40))) nSelect = 18;
		if (ImGui::Button("U", ImVec2(40, 40))) nSelect = 21;
		if (ImGui::Button("X", ImVec2(40, 40))) nSelect = 24;
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		if (ImGui::Button("A", ImVec2(40, 40))) nSelect = 1;
		if (ImGui::Button("D", ImVec2(40, 40))) nSelect = 4;
		if (ImGui::Button("G", ImVec2(40, 40))) nSelect = 7;
		if (ImGui::Button("J", ImVec2(40, 40))) nSelect = 10;
		if (ImGui::Button("M", ImVec2(40, 40))) nSelect = 13;
		if (ImGui::Button("P", ImVec2(40, 40))) nSelect = 16;
		if (ImGui::Button("S", ImVec2(40, 40))) nSelect = 19;
		if (ImGui::Button("V", ImVec2(40, 40))) nSelect = 22;
		if (ImGui::Button("Y", ImVec2(40, 40))) nSelect = 25;
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		if (ImGui::Button("B", ImVec2(40, 40))) nSelect = 2;
		if (ImGui::Button("E", ImVec2(40, 40))) nSelect = 5;
		if (ImGui::Button("H", ImVec2(40, 40))) nSelect = 8;
		if (ImGui::Button("K", ImVec2(40, 40))) nSelect = 11;
		if (ImGui::Button("N", ImVec2(40, 40))) nSelect = 14;
		if (ImGui::Button("Q", ImVec2(40, 40))) nSelect = 17;
		if (ImGui::Button("T", ImVec2(40, 40))) nSelect = 20;
		if (ImGui::Button("W", ImVec2(40, 40))) nSelect = 23;
		if (ImGui::Button("Z", ImVec2(40, 40))) nSelect = 26;
		ImGui::EndGroup();
		ImGui::SameLine();

		// Left
		ImVec2 sSize = ImGui::GetItemRectSize();
		{
			ImGui::BeginChild("left pane", ImVec2(float(g_nMainWindowWidth) - 4.f * sSize.x, sSize.y), true);
			for (int i = 0; i < (int)g_aszGameNames.size(); i++)
			{
				// set labels
				std::string acGameName;
				for (wchar_t c : g_aszGameNames[i]) acGameName += (char)c;
				if (ImGui::Selectable(acGameName.c_str(), g_nGameSelected == i))
					g_nGameSelected = i;

				// letter button pressed ?
				if (nSelect >= 0)
				{
					// 0 selected ?
					if (nSelect == 0)
					{
						// set scroll here
						ImGui::SetScrollHereY(0.0f);
						nSelect = -1;
					}
					else
					{
						// set letter character
						char cLower = 'a' + (char)(nSelect - 1);
						char cUpper = 'A' + (char)(nSelect - 1);

						if ((acGameName.c_str()[0] == cLower) || (acGameName.c_str()[0] == cUpper))
						{
							// set scroll here
							ImGui::SetScrollHereY(0.0f);
							nSelect = -1;
						}
					}
				}
			}
			ImGui::EndChild();
		}

		// select graphics profile
		const char* aacElems_names[InjectionProfile::IP_Count] = { "Direct 3D 9.x", "Direct 3D 10.x", "Direct 3D 11.x" };
		const char* acElem_name = (g_eGameProfile >= 0 && g_eGameProfile < InjectionProfile::IP_Count) ? aacElems_names[g_eGameProfile] : "Unknown";
		static int s_nSelection = 0;
		ImGui::SliderInt("GPU API", &s_nSelection, 0, InjectionProfile::IP_Count - 1, acElem_name);
		switch (s_nSelection)
		{
		case 0:
			g_eGameProfile = InjectionProfile::IP_D3D9;
			break;
		case 1:
			g_eGameProfile = InjectionProfile::IP_D3D10;
			break;
		case 2:
			g_eGameProfile = InjectionProfile::IP_D3D11;
			break;
		default:
			break;
		}
	}

	// selected game
	ImGui::Separator();
	std::string acGameName;
	UINT uI = 0;
	if ((g_aszGameNames.size() > 0) && (g_nGameSelected >= 0) && (g_nGameSelected < (int)g_aszGameNames.size()))
	{
		for (wchar_t c : g_aszGameNames[g_nGameSelected]) acGameName += (char)c;
	}
	ImGui::Text("Game : %s", acGameName.c_str());
	ImGui::Separator();
}

/**
* Set ImGui style by Colorlovers color scheme.
**/
void ImGui_StyleColorsByScheme()
{
	ImGui::ColorSchemeHex sScheme = {};
	CopyMemory(&sScheme.uColor[0], &g_colorSchemes[g_pAquilinusConfig->dwColorSchemeIndex].uColor[0], sizeof(UINT[5]));
	ImGui::StyleColorsByScheme(sScheme);
}

/**
* ImGui Options.
***/
void ImGui_Options(AquilinusCfg& sConfig)
{
	// set selection
	static int s_nStyle_idx = -1;
	s_nStyle_idx = (int)sConfig.dwColorSchemeIndex;

	// create selection string by color scheme names
	static std::string acItems(AQUILINUS_NUMBER_OF_COLOR_SCHEMES * 32, '\0');
	static bool s_bInit = false;
	if (!s_bInit)
	{
		// create a zero-separated string for ImGui
		UINT uCount = 0;
		for (unsigned uI = 0; uI < AQUILINUS_NUMBER_OF_COLOR_SCHEMES; uI++)
		{
			unsigned uSize;
			for (uSize = 0; uSize < 31; uSize++)
				if (g_colorSchemes[uI].szName[uSize] == 0) break;
			uSize++;
			CopyMemory(&acItems[uCount], &g_colorSchemes[uI].szName[0], (size_t)uSize);
			uCount += uSize;
		}

		s_bInit = true;
	}

	// set combo box
	if (ImGui::Combo("Color Scheme", &s_nStyle_idx, acItems.c_str()))
	{
		if ((s_nStyle_idx >= 0) && (s_nStyle_idx < AQUILINUS_NUMBER_OF_COLOR_SCHEMES))
			sConfig.dwColorSchemeIndex = (DWORD)s_nStyle_idx;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(g_colorSchemes[sConfig.dwColorSchemeIndex].szName);
		std::string acL = std::string("by ") + std::string(g_colorSchemes[sConfig.dwColorSchemeIndex].szLover);
		ImGui::TextUnformatted(acL.c_str());
		ImGui::TextUnformatted(g_colorSchemes[sConfig.dwColorSchemeIndex].szLink);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("All Color Schemes are User-Generated Content from and licenced at ColourLovers.com ! https://www.colourlovers.com/terms ");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	// checkbox D3D9Ex
	ImGui::Checkbox("Create D3D9Ex", (bool*)&sConfig.bCreateD3D9Ex);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Creates a D3D9Ex device instead of a D3D9 device.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	}

/**
* Save the global config file.
***/
void SaveConfig()
{
	// write file new
	std::ofstream configWrite;
#ifdef _WIN64
	configWrite.open("Aquilinus_x64.cfg");
#else
	configWrite.open("Aquilinus_Win32.cfg");
#endif
	configWrite << "dwColorSchemeIndex = " << g_dwColorSchemeIndex << "\n";
	configWrite << "bCreateD3DEx = " << g_pAquilinusConfig->bCreateD3D9Ex << "\n";
	configWrite.close();
}

#endif

#pragma endregion

#pragma region Inicio methods

/**
* Inicio init.
***/
HRESULT InicioInit()
{
	// create config memory page
	g_hConfigMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		sizeof(AquilinusCfg),    // maximum object size (low-order DWORD)
		g_szMemoryPageName);     // name of the Aquilinus config

	// return if failed
	if (g_hConfigMapFile == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
		return E_FAIL;
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
		return E_FAIL;
	}

	// set base config... 
	g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::NoProject;
	g_pAquilinusConfig->dwColorSchemeIndex = g_dwColorSchemeIndex;
	g_pAquilinusConfig->bUseDllPath = FALSE;
	g_pAquilinusConfig->bProfileWindow = FALSE;
	g_pAquilinusConfig->bExternalSave = FALSE;
	g_pAquilinusConfig->dwSizeOfExternalSaveFile = 0;

	wchar_t buf[MAX_PATH] = { 0 };

	// Get the dll's full path name  
#ifdef _WIN64
	GetFullPathName(L"Aquilinus_x64.dll", MAX_PATH, buf, NULL);
	// set Aquilinus path in the configuration
	ZeroMemory((PVOID)g_pAquilinusConfig->szAquilinusPath, MAX_PATH * sizeof(wchar_t));
	CopyMemory((PVOID)g_pAquilinusConfig->szAquilinusPath, buf, ((wcslen(buf) - 17) * sizeof(wchar_t)));
#else
	GetFullPathName(L"Aquilinus_Win32.dll", MAX_PATH, buf, NULL);
	// set Aquilinus path in the configuration
	ZeroMemory((PVOID)g_pAquilinusConfig->szAquilinusPath, MAX_PATH * sizeof(wchar_t));
	CopyMemory((PVOID)g_pAquilinusConfig->szAquilinusPath, buf, ((wcslen(buf) - 19) * sizeof(wchar_t)));
#endif
	OutputDebugString(L"Aquilinus path:");
	OutputDebugString(g_pAquilinusConfig->szAquilinusPath);
	OutputDebugString(buf);

	return S_OK;
}

/**
* Inicio close.
***/
void InicioClose()
{
	ForceIdle();
	SetEvent(g_hInjectionAbortEvent);
	UnmapViewOfFile((LPCVOID)g_pAquilinusConfig);
	CloseHandle(g_hConfigMapFile);
}

/**
* Forces >Idle< state for Inicio closing any profile settings.
***/
void ForceIdle()
{
	// stop injection thread if running
	if (g_eInicioStatus == InicioStatus::Injecting)
		SetEvent(g_hInjectionAbortEvent);

	g_eInicioStatus = InicioStatus::Idle;
}

/**
* Enumerates the game names in alphabetical order using the filemanager class.
***/
HRESULT EnumerateGameNames()
{
	// clear vector
	g_aszGameNames.clear();
	g_aszGameNamesUnsorted.clear();

	// add names
	for (DWORD i = 0; i < g_pFileManager->GetProcessNumber(); i++)
	{
		g_aszGameNames.push_back(g_pFileManager->GetName(i));
		g_aszGameNamesUnsorted.push_back(g_pFileManager->GetName(i));
	}

	// and sort alphabetical
	std::sort(g_aszGameNames.begin(), g_aszGameNames.end());

	// set letter indices and entry number
	DWORD nCurrentLetter = 0;
	bool bFirstIndexSet = false;
	DWORD nFirstIndex = 0;
	ZeroMemory(&g_nLetterEntriesNumber[0], 27 * sizeof(DWORD));
	ZeroMemory(&g_nLetterStartIndex[0], 27 * sizeof(DWORD));
	for (int i = 0; i < (int)g_aszGameNames.size(); i++)
	{
		// handle zero separately
		if (nCurrentLetter == 0)
		{
			// does this game name start with '0' to '9' ?
			if ((g_aszGameNames[i].c_str()[0] >= '0') && (g_aszGameNames[i].c_str()[0] <= '9'))
			{
				// set letter start index
				if ((g_nLetterStartIndex[nCurrentLetter] == 0) && (bFirstIndexSet) && (nFirstIndex != nCurrentLetter))
					g_nLetterStartIndex[nCurrentLetter] = (DWORD)i;

				if (!bFirstIndexSet)
				{
					nFirstIndex = nCurrentLetter;
					bFirstIndexSet = true;
				}

				// increase letter entry number
				g_nLetterEntriesNumber[nCurrentLetter]++;
			}
			else
			{
				// increase current letter, push back game list index
				i--;
				nCurrentLetter++;
				if (nCurrentLetter >= 27) i = (int)g_aszGameNames.size();
			}
		}
		else
		{
			// does this game name start with the current letter ?
			if (((g_aszGameNames[i].c_str()[0] - 'A') == (char)nCurrentLetter - 1) || ((g_aszGameNames[i].c_str()[0] - 'A') == (char)nCurrentLetter - 1))
			{
				// set letter start index
				if ((g_nLetterStartIndex[nCurrentLetter] == 0) && (bFirstIndexSet) && (nFirstIndex != nCurrentLetter))
					g_nLetterStartIndex[nCurrentLetter] = (DWORD)i;

				if (!bFirstIndexSet)
				{
					nFirstIndex = nCurrentLetter;
					bFirstIndexSet = true;
				}

				// increase letter entry number
				g_nLetterEntriesNumber[nCurrentLetter]++;
			}
			else
			{
				// increase current letter, push back game list index
				i--;
				nCurrentLetter++;
				if (nCurrentLetter >= 27) i = (int)g_aszGameNames.size();
			}
			}
		}

#ifdef _DEBUG
	// output debug list
	for (int i = 0; i < 27; i++)
	{
		wchar_t buf[64];
		wsprintf(buf, L"%d", i);
		OutputDebugString(buf);
		for (int j = 0; j < (int)g_nLetterEntriesNumber[i]; j++)
		{
			OutputDebugString(g_aszGameNames[j + (int)g_nLetterStartIndex[i]].c_str());
		}
	}
#endif

	return S_OK;
	}

/**
* Enumerate supported interface names.
***/
HRESULT EnumerateSupportedInterfaces(LPWSTR*& pszEntries, DWORD& dwEntries)
{
	// loop and enumerate
	dwEntries = SUPPORTED_INTERFACES_NUMBER;
	pszEntries = new LPWSTR[dwEntries];
	for (int nIndex = 0; nIndex < SUPPORTED_INTERFACES_NUMBER; nIndex++)
	{
		pszEntries[nIndex] = new wchar_t[wcslen(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName) + 1];
		CopyMemory((PVOID)pszEntries[nIndex], AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName, (wcslen(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName) * sizeof(wchar_t) + sizeof(wchar_t)));
	}

	return S_OK;
}

/**
* @param hToken access token handle
* @param lpszPrivilege name of privilege to enable or disable
* @param bEnablePrivilege to enable or disable privilege
***/
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            /**< lookup privilege on local system */
		lpszPrivilege,   /**< privilege to lookup */
		&luid))        /**< receives LUID of privilege */
	{
		wchar_t buf[256];
		wsprintf(buf, L"LookupPrivilegeValue error: %u\n", GetLastError());
		OutputDebugString(buf);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		wchar_t buf[256];
		wsprintf(buf, L"AdjustTokenPrivileges error: %u\n", GetLastError());
		OutputDebugString(buf);
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		wchar_t buf[256];
		wsprintf(buf, L"The token does not have the specified privilege. \n");
		OutputDebugString(buf);
		return FALSE;
	}

	return TRUE;
}

/**
* Get the thread ID from the process name.
* @param szProcName The name of the process.
***/
DWORD GetTargetThreadID(wchar_t* szProcName)
{
	PROCESSENTRY32W pe32;
	HANDLE hSnapShot;
	BOOL bFoundProc = false;

	if (!szProcName) return E_FAIL;

	// set szProcName to lower characters
	int i = 0;
	while (szProcName[i])
	{
		szProcName[i] = towlower(szProcName[i]);
		i++;
	}

	// get snapshot handle
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Aquilinus : Unable create toolhelp snapshot!");
		return 0;
	}

	// loop through processes
	pe32.dwSize = sizeof(PROCESSENTRY32W);
	bFoundProc = Process32FirstW(hSnapShot, &pe32);
	while (bFoundProc)
	{
		// set process entry to lower characters
		int i = 0;
		while (pe32.szExeFile[i])
		{
			pe32.szExeFile[i] = towlower(pe32.szExeFile[i]);
			i++;
		}

		if (wcscmp(pe32.szExeFile, szProcName) == NULL)
		{
			return pe32.th32ProcessID;
		}
		bFoundProc = Process32NextW(hSnapShot, &pe32);
	}
	return 0;
}

/**
* Inject a dll to a process.
* @param dwID The thread ID of the process.
* @param szDllName The full path name of the dll.
***/
HRESULT Inject(DWORD dwID, const wchar_t* szDllName)
{
	HANDLE hProc = nullptr;
	HANDLE hToken = nullptr;
	wchar_t szBuffer[128] = { 0 };
	LPVOID pRemoteString = nullptr, pLoadLibraryA = nullptr;

	if (!dwID)
		return E_FAIL;

	// open the desired process
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
	if (!hProc)
	{
		// failed, set SeDebugPrivilege
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
		{
			if (GetLastError() == ERROR_NO_TOKEN)
			{
				if (!ImpersonateSelf(SecurityImpersonation))
					return E_FAIL;

				if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
				{
					wsprintf(szBuffer, L"OpenThreadToken() failed: %d", GetLastError());
					OutputDebugString(szBuffer);
					return E_FAIL;
				}
			}
			else
				return E_FAIL;
		}

		// enable SeDebugPrivilege
		if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
		{
			// method can have success, even if it fails
			wsprintf(szBuffer, L"SetPrivilege() failed: %d", GetLastError());
			OutputDebugString(szBuffer);
		}

		// try to open the process again...
		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
		if (!hProc)
		{
			wsprintf(szBuffer, L"OpenProcess() failed: %d", GetLastError());
			OutputDebugString(szBuffer);

			CloseHandle(hToken);
			return E_FAIL;
		}
	}

	// get LoadLibraryA method address
	HMODULE hKernel = GetModuleHandle(L"kernel32.dll");
	if (hKernel)
		pLoadLibraryA = (LPVOID)GetProcAddress(hKernel, "LoadLibraryA");
	if (pLoadLibraryA == nullptr)
	{
		wsprintf(szBuffer, L"Aquilinus : Failed to get the address of >LoadLibraryW< : %d", GetLastError());
		OutputDebugString(szBuffer);
		return E_FAIL;
	}

	// convert to LPCSTR
	int size = (int)wcslen(szDllName);
	size += 2;
	char* szDll = (char*)malloc(size);
	wcstombs_s(NULL, szDll, size,
		szDllName, size);

	// Allocate space in the process for our DLL 
	pRemoteString = (LPVOID)VirtualAllocEx(hProc, NULL, wcslen(szDllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated
	if ((pRemoteString) && (szDll))
		WriteProcessMemory(hProc, (LPVOID)pRemoteString, szDll, wcslen(szDllName), NULL);
	else
		return E_FAIL;

	// free memory
	free(szDll);

	// Load our DLL
	if (CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibraryA, (LPVOID)pRemoteString, NULL, NULL) == NULL)
	{
		wsprintf(szBuffer, L"Aquilinus : Failed to create >LoadLibraryA< remote thread : %d", GetLastError());
		OutputDebugString(szBuffer);
		return E_FAIL;
	}

	if (hToken) CloseHandle(hToken);
	if (hProc) CloseHandle(hProc);
	return S_OK;
}

/**
* Thread to inject.
***/
DWORD WINAPI InjectionThread(LPVOID Param)
{
	// injection repeat...
	int nRepeat = *(int*)Param;

	// Retrieve process ID
	DWORD dwID = NULL, dwRes = 1;
	while (dwID == NULL)
	{
		// abort ??
		dwRes = WaitForSingleObject(g_hInjectionAbortEvent, 10);
		if (dwRes == WAIT_OBJECT_0) return 0;

		// get the process name, only for complemented profiles get it directly from the cfg
		std::wstring szP;
		if (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::Complemented)
			szP = g_pAquilinusConfig->szProcessName;
		else
			szP = g_pFileManager->GetProcessName(g_pAquilinusConfig->dwProcessIndex);

		// erase '\r' and '\n'
		szP.erase(std::remove(szP.begin(), szP.end(), '\r'), szP.end());
		szP.erase(std::remove(szP.begin(), szP.end(), '\n'), szP.end());
		LPWSTR szProc = new wchar_t[szP.length() + 1];
		CopyMemory((void*)szProc, (void*)szP.c_str(), (szP.length() + 1) * sizeof(wchar_t));

		// get id
		dwID = GetTargetThreadID(szProc);
		Sleep(5);

		if (dwID)
		{
			// reinject ?
			if (nRepeat > 0)
			{
				DWORD dwOld = dwID;
				while ((dwID) && (dwID == dwOld))
				{
					Sleep(5);
					dwID = GetTargetThreadID(szProc);
				}
				nRepeat--;
		}
	}
}

	// suspend the process
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// call external suspend tool
	std::wstringstream szAppName = std::wstringstream();
	std::wstringstream szCmd = std::wstringstream();
#ifdef _WIN64
	szAppName << g_pAquilinusConfig->szAquilinusPath << L"//pssuspend64.exe";
	szCmd << "pssuspend64 " << dwID;
#else
	szAppName << g_pAquilinusConfig->szAquilinusPath << L"pssuspend.exe";
	szCmd << "pssuspend " << dwID;
#endif
	CreateProcessW(szAppName.str().c_str(), (LPWSTR)&szCmd.str().c_str()[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// set configuration ID
	g_pAquilinusConfig->dwID = dwID;

	// Inject our main dll
	wchar_t buf[MAX_PATH] = { 0 };

	// Get the dll's full path name
#ifdef _WIN64
	GetFullPathName(L"Aquilinus_x64.dll", MAX_PATH, buf, NULL);
#else
	GetFullPathName(L"Aquilinus_Win32.dll", MAX_PATH, buf, NULL);
#endif
	OutputDebugString(buf);

	if (dwID != NULL)
	{
		if (FAILED(Inject(dwID, buf)))
		{
			OutputDebugString(L"Aquilinus : DLL Not Loaded!");
		}

		else
		{
			OutputDebugString(L"Aquilinus : DLL Loaded!");
		}
		}
	else
		OutputDebugString(L"Aquilinus : No target process found !");

	// resume suspended thread
	if (true)
	{
		Sleep(1000);
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// call external suspend tool
		szAppName = std::wstringstream();
		szCmd = std::wstringstream();
#ifdef _WIN64
		szAppName << g_pAquilinusConfig->szAquilinusPath << L"//pssuspend64.exe";
		szCmd << "pssuspend64 -r " << dwID;
#else
		szAppName << g_pAquilinusConfig->szAquilinusPath << L"pssuspend.exe";
		szCmd << "pssuspend -r " << dwID;
#endif
		CreateProcessW(szAppName.str().c_str(), (LPWSTR)&szCmd.str().c_str()[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return S_OK;
	}

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

/**
* Viewport callback.
***/
void viewport_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/**
* Cursor position callback.
***/
void cursor_position_callback(GLFWwindow* window, double x, double y) {
	if ((g_buttonEvent == 1) && ((y < (double)22) || (g_buttonEvent == 1))) // TODO !! SET BY TOP BAR FONT SIZE
	{
		g_offset_cpx = (int)x - g_cp_x;
		g_offset_cpy = (int)y - g_cp_y;
	}
}

/**
* Mouse button callback.
***/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (y < (double)22) // TODO !! SET BY TOP BAR FONT SIZE
		{
			g_cp_x = (int)floor(x);
			g_cp_y = (int)floor(y);
			g_buttonEvent = 1;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		g_buttonEvent = 0;
		g_cp_x = 0;
		g_cp_y = 0;
	}
}

/**
* Inicio main windows entry point.
***/
int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	OutputDebugString(L"Inicio : Started...");

	// init
	InicioInit();

	// start idle
	g_eInicioStatus = InicioStatus::Idle;
	g_eCurrentWindow = InicioWindows::Main;

	// set the version
	g_eVersion.dwPrima = AQUILINUS_VERSION_PRIMA;
	g_eVersion.dwSecondo = AQUILINUS_VERSION_SECONDO;
	g_eVersion.dwTerzo = AQUILINUS_VERSION_TERZO;

	// glfw: initialize and configure
	if (!glfwInit()) { OutputDebugString(L"Inicio: GLFW Init fail !"); return 1; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	glfwWindowHint(GLFW_DECORATED, 0);
	GLFWwindow* window = glfwCreateWindow(g_nMainWindowWidth, g_nMainWindowHeight, "Inicio", NULL, NULL);
	if (window == NULL)
	{
		OutputDebugString(L"Inicio: GLFW window creation failed !");
		glfwTerminate();
		return -1;
	}

	// set size, position, callback methods, enable vsync
	glfwSetWindowSizeLimits(window, g_nMainWindowWidth, g_nMainWindowHeight, g_nMainWindowWidth, g_nMainWindowHeight);
	glfwSetWindowPos(window, (g_nScreenWidth / 2) - (g_nMainWindowWidth), (g_nScreenHeight / 8));
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, viewport_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// and init glew
	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		OutputDebugString(L"Inicio: Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context, font
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig sConfig = {};
	sConfig.SizePixels = 16;
	ImFont* psFontSmall = io.Fonts->AddFontDefault(&sConfig);
	sConfig.SizePixels = 22;
	ImFont* psFontMedium = io.Fonts->AddFontDefault(&sConfig);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(nullptr);

	OutputDebugString(L"Inicio : Aquilinus window creation succeeded !");

	// create file manager
	g_pFileManager = new AQU_FileManager(true);

	// load or create options file
#ifdef _WIN64
	std::ifstream configRead("Aquilinus_x64.cfg");
#else
	std::ifstream configRead("Aquilinus_Win32.cfg");
#endif
	bool bFileGood = false;
	if (configRead.good())
	{
		bFileGood = true;
		std::string str;
		bool bGood = false;

		// read first line
		if (std::getline(configRead, str)) bGood = true;

		// color
		std::size_t found = str.find("dwColorSchemeIndex");
		if ((found != std::string::npos) && (found == 0))
		{
			// parse color
			str.erase(0, 20);
			g_dwColorSchemeIndex = atoi(str.c_str());
		}
		else
			bFileGood = false;

		str = std::string();
		if (std::getline(configRead, str)) bGood = true; else bGood = false;

		// color
		found = str.find("bCreateD3DEx");
		if ((found != std::string::npos) && (found == 0))
		{
			// parse color
			str.erase(0, 14);
			g_pAquilinusConfig->bCreateD3D9Ex = atoi(str.c_str());
		}
		else
			bFileGood = false;
	}

	configRead.close();

	// no or corrupt config file ?
	if (!bFileGood)
	{
		// init base config
		g_dwColorSchemeIndex = 0;
		g_pAquilinusConfig->bCreateD3D9Ex = 0;

		// save config file
		SaveConfig();
	}

	// set configuration
	if (g_dwColorSchemeIndex >= AQUILINUS_NUMBER_OF_COLOR_SCHEMES) g_dwColorSchemeIndex = 0;
	g_pAquilinusConfig->dwColorSchemeIndex = g_dwColorSchemeIndex;

	// always set the detour time delay to zero at startup !
	g_pAquilinusConfig->dwDetourTimeDelay = 0;

	// set color scheme
	ImGui_StyleColorsByScheme();
	ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// create the abort event
		if (g_hInjectionAbortEvent) CloseHandle(g_hInjectionAbortEvent);
		g_hInjectionAbortEvent = CreateEvent(
			NULL,               // default security attributes
			TRUE,               // manual-reset event
			FALSE,              // initial state is nonsignaled
			TEXT("Abort")  // object name
		);

		// proceed to injection
		if (g_eInicioStatus == InicioStatus::ToInject)
		{
			// create the thread to override the virtual methods table
			g_eInicioStatus = InicioStatus::Injecting;
			g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingAreaNew;
			g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
			g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
			if (g_hInjectionThread == NULL)
			{
				g_eInicioStatus = InicioStatus::ToInject;
				OutputDebugString(L"Aquilinus : Failed to create injection thread !");
				Sleep(100);
			}
		}

		// check if external file is to be saved
		if ((g_pAquilinusConfig->bExternalSave) && (g_pAquilinusConfig->dwSizeOfExternalSaveFile))
		{
			// set bool to false
			g_pAquilinusConfig->bExternalSave = false;

			// get the config map handle
			HANDLE hSaveFile = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,   // read/write access
				FALSE,                 // do not inherit the name
				L"AquilinusSaveMap");   // name of the Aquilinus config

			// failed ?
			if (hSaveFile == NULL)
			{
				OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
			}
			else
			{
				// create map view
				LPVOID pData =
					MapViewOfFile(hSaveFile,   // handle to map object
						FILE_MAP_ALL_ACCESS,              // read/write permission
						0,
						0,
						g_pAquilinusConfig->dwSizeOfExternalSaveFile);

				// failed ?
				if (pData == NULL)
				{
					OutputDebugString(TEXT("Aquilinus : Could not map view of file.\n"));
					CloseHandle(hSaveFile);
				}
				else
				{
					// save working area file
					std::ofstream outFile;
					outFile.open(g_pAquilinusConfig->szExternalSaveFilePath, std::ios::out | std::ios::binary);
					if (outFile.is_open())
					{
						// write the file data
						outFile.write((const char*)pData, g_pAquilinusConfig->dwSizeOfExternalSaveFile);
						outFile.close();
					}
					else
						OutputDebugString(L"Aquilinus : Could not open file to write !");

					// close handle
					UnmapViewOfFile((LPCVOID)hSaveFile);
					CloseHandle(hSaveFile);
				}
			}
		}

		// Poll and handle events, update window fields
		glfwPollEvents();
		glfwGetWindowPos(window, &g_w_posx, &g_w_posy);
		glfwSetWindowPos(window, g_w_posx + g_offset_cpx, g_w_posy + g_offset_cpy);
		g_controlEvent = 0;
		g_offset_cpx = 0;
		g_offset_cpy = 0;
		g_cp_x += g_offset_cpx;
		g_cp_y += g_offset_cpy;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// main window set flags and zero pos
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2((float)g_nMainWindowWidth, (float)g_nMainWindowHeight), ImGuiCond_FirstUseEver);

		// select window stage
		static bool s_bOpen = true;
		if (g_eCurrentWindow == InicioWindows::Main)
		{
			// window resize ??
			if (g_bWindowResize)
			{
				// set size and constant size
				g_nMainWindowWidth = MAIN_WINDOW_WIDTH;
				g_nMainWindowHeight = MAIN_WINDOW_HEIGHT;
				glfwSetWindowSize(window, g_nMainWindowWidth, g_nMainWindowHeight);
				glfwSetWindowSizeLimits(window, g_nMainWindowWidth, g_nMainWindowHeight, g_nMainWindowWidth, g_nMainWindowHeight);
				ImGui::SetNextWindowSize(ImVec2((float)g_nMainWindowWidth, (float)g_nMainWindowHeight), ImGuiCond_None);
			}

			// main window
			ImGui::PushFont(psFontMedium);
			if (ImGui::Begin("Aquilinus", &s_bOpen, window_flags))
			{
				ImGui::PopFont();
				ImGui::PushFont(psFontSmall);
				ImGui::Text("Vireio Perception 3D Modification Studio");
				ImGui_Main();
				ImGui::PopFont();
			}
			else
			{
				OutputDebugString(L"Inicio: ImGui failed to create window !");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::End();

			if (!s_bOpen) glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		else if (g_eCurrentWindow == InicioWindows::NewProject)
		{
			// window resize ??
			if (g_bWindowResize)
			{
				// set size and constant size
				g_nMainWindowWidth = MAIN_WINDOW_WIDTH;
				g_nMainWindowHeight = MAIN_WINDOW_HEIGHT_NEW;
				glfwSetWindowSize(window, g_nMainWindowWidth, g_nMainWindowHeight);
				glfwSetWindowSizeLimits(window, g_nMainWindowWidth, g_nMainWindowHeight, g_nMainWindowWidth, g_nMainWindowHeight);
				ImGui::SetNextWindowSize(ImVec2((float)g_nMainWindowWidth, (float)g_nMainWindowHeight), ImGuiCond_None);
			}

			// info window
			ImGui::PushFont(psFontMedium);
			if (ImGui::Begin("Aquilinus New", &s_bOpen, window_flags))
			{
				ImGui::PopFont();
				ImGui::PushFont(psFontSmall);
				ImGui_New();
				if (ImGui::Button("Inject", ImVec2(100, 40)))
				{
					// get process id
					DWORD unOldProcessIndex = g_pAquilinusConfig->dwProcessIndex;
					for (int i = 0; i < (int)g_aszGameNamesUnsorted.size(); i++)
					{
						// compare strings from the sorted and unsorted game list
						if (g_aszGameNames[g_nGameSelected].compare(g_aszGameNamesUnsorted[i]) == 0)
						{
							g_pAquilinusConfig->dwProcessIndex = i;
						}
					}

					// set ALL classes to "NoInjection"
					for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
						g_pAquilinusConfig->eInjectionTechnique[i] = AQU_InjectionTechniques::NoInjection;

					// set injection technique based on d3d selection
					switch (g_eGameProfile)
					{
					case InjectionProfile::IP_D3D9:
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] = AQU_InjectionTechniques::VMTable;
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9] = AQU_InjectionTechniques::VMTable;
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] = AQU_InjectionTechniques::VMTable;
						break;
					case InjectionProfile::IP_D3D10:
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] = AQU_InjectionTechniques::VMTable;
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = AQU_InjectionTechniques::VMTable;
						break;
					case InjectionProfile::IP_D3D11:
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] = AQU_InjectionTechniques::VMTable;
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] = AQU_InjectionTechniques::VMTable;
						g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = AQU_InjectionTechniques::VMTable;
						break;
					default:
						break;
					}

					// back to main
					g_eCurrentWindow = InicioWindows::Main;
					g_eInicioStatus = InicioStatus::ToInject;
					g_bWindowResize = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(100, 40)))
				{
					// back to main
					g_eCurrentWindow = InicioWindows::Main;
					g_bWindowResize = true;
				}
				ImGui::PopFont();

				if (!s_bOpen) glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else
			{
				OutputDebugString(L"Inicio: ImGui failed to create window !");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::End();

		}
		else if (g_eCurrentWindow == InicioWindows::Options)
		{
			// window resize ??
			if (g_bWindowResize)
			{
				// set size and constant size
				g_nMainWindowWidth = MAIN_WINDOW_WIDTH;
				g_nMainWindowHeight = MAIN_WINDOW_HEIGHT_OPTIONS;
				glfwSetWindowSize(window, g_nMainWindowWidth, g_nMainWindowHeight);
				glfwSetWindowSizeLimits(window, g_nMainWindowWidth, g_nMainWindowHeight, g_nMainWindowWidth, g_nMainWindowHeight);
				ImGui::SetNextWindowSize(ImVec2((float)g_nMainWindowWidth, (float)g_nMainWindowHeight), ImGuiCond_None);
			}

			// info window
			ImGui::PushFont(psFontMedium);
			if (ImGui::Begin("Aquilinus Options", &s_bOpen, window_flags))
			{
				ImGui::PopFont();

				ImGui::PushFont(psFontSmall);
				static AquilinusCfg s_sConfigTmp = {};
				static bool s_bTmpConfigInit = false;
				if (!s_bTmpConfigInit)
				{
					// set all relevant config fields
					s_sConfigTmp.dwColorSchemeIndex = g_pAquilinusConfig->dwColorSchemeIndex;
					s_sConfigTmp.bCreateD3D9Ex = g_pAquilinusConfig->bCreateD3D9Ex;
					s_bTmpConfigInit = true;
				}
				ImGui_Options(s_sConfigTmp);
				if (ImGui::Button("Apply", ImVec2(100, 40)))
				{
					// set all relevant config fields
					g_pAquilinusConfig->dwColorSchemeIndex = s_sConfigTmp.dwColorSchemeIndex;
					g_pAquilinusConfig->bCreateD3D9Ex = s_sConfigTmp.bCreateD3D9Ex;
					g_dwColorSchemeIndex = s_sConfigTmp.dwColorSchemeIndex;

					// set color scheme
					ImGui_StyleColorsByScheme();
					clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

					// and save config
					SaveConfig();
				}
				ImGui::SameLine();
				if (ImGui::Button("Back", ImVec2(100, 40)))
				{
					// set all relevant config fields
					s_sConfigTmp.dwColorSchemeIndex = g_pAquilinusConfig->dwColorSchemeIndex;
					s_sConfigTmp.bCreateD3D9Ex = g_pAquilinusConfig->bCreateD3D9Ex;

					// back to main
					g_eCurrentWindow = InicioWindows::Main;
					g_bWindowResize = true;
				}
				ImGui::PopFont();



				if (!s_bOpen) glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else
			{
				OutputDebugString(L"Inicio: ImGui failed to create window !");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::End();

		}
		else if (g_eCurrentWindow == InicioWindows::Info)
		{
			// window resize ??
			if (g_bWindowResize)
			{
				// set size and constant size
				g_nMainWindowWidth = MAIN_WINDOW_WIDTH;
				g_nMainWindowHeight = MAIN_WINDOW_HEIGHT_INFO;
				glfwSetWindowSize(window, g_nMainWindowWidth, g_nMainWindowHeight);
				glfwSetWindowSizeLimits(window, g_nMainWindowWidth, g_nMainWindowHeight, g_nMainWindowWidth, g_nMainWindowHeight);
				ImGui::SetNextWindowSize(ImVec2((float)g_nMainWindowWidth, (float)g_nMainWindowHeight), ImGuiCond_None);
			}

			// info window
			ImGui::PushFont(psFontMedium);
			if (ImGui::Begin("Aquilinus Info", &s_bOpen, window_flags))
			{
				ImGui::PopFont();
				ImGui::PushFont(psFontSmall);

				ImGui::TextWrapped("Vireio Perception : Open - Source Stereoscopic 3D Driver");
				ImGui::TextWrapped("Copyright(C) 2012 Andres Hernandez");
				ImGui::NewLine();
				ImGui::TextWrapped("Aquilinus : Vireio Perception 3D Modification Studio");
				ImGui::TextWrapped("Copyright(C) 2014 Denis Reischl");
				ImGui::Separator();

				ImGui::TextWrapped("Vireio Perception Version History :");
				ImGui::TextWrapped("v1.0.0 2012 by Andres Hernandez");
				ImGui::TextWrapped("v1.0.X 2013 by John Hicks, Neil Schneider");
				ImGui::TextWrapped("v1.1.x 2013 by Primary Coding Author : Chris Drain");
				ImGui::TextWrapped("Team Support : John Hicks, Phil Larkson, Neil Schneider");
				ImGui::TextWrapped("v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown");
				ImGui::TextWrapped("v2.0.4 to v3.0.x 2014 - 2015 by Grant Bagwell, Simon Brown and Neil Schneider");
				ImGui::TextWrapped("v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider");
				ImGui::NewLine();

				ImGui::TextWrapped("This program is free software : you can redistribute itand /or modify");
				ImGui::TextWrapped("it under the terms of the GNU Lesser General Public License as published by");
				ImGui::TextWrapped("the Free Software Foundation, either version 3 of the License, or");
				ImGui::TextWrapped("(at your option) any later version.");
				ImGui::NewLine();

				ImGui::TextWrapped("This program is distributed in the hope that it will be useful,");
				ImGui::TextWrapped("but WITHOUT ANY WARRANTY; without even the implied warranty of");
				ImGui::TextWrapped("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the");
				ImGui::TextWrapped("GNU Lesser General Public License for more details.");
				ImGui::NewLine();

				ImGui::TextWrapped("You should have received a copy of the GNU Lesser General Public License");
				ImGui::TextWrapped("along with this program.If not, see < http://www.gnu.org/licenses/>.");
				ImGui::NewLine();	ImGui::Separator();

				ImGui::TextWrapped("The Aquilinus PNG/Deflate Decoder partially uses altered code from following sources, with permission by licence :");
				ImGui::TextWrapped("picoPNG version 20101224 (c) 2005-2010 Lode Vandevenne");
				ImGui::TextWrapped("Deflate Decompressor (c) 2011 Stephan Brumme");
				ImGui::Separator(); ImGui::NewLine();
				ImGui::TextWrapped("Dear ImGui %s", ImGui::GetVersion());
				ImGui::Separator();
				ImGui::TextWrapped("By Omar Cornut and all Dear ImGui contributors.");
				ImGui::TextWrapped("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

				if (ImGui::Button("Ok"))
				{
					// back to main
					g_eCurrentWindow = InicioWindows::Main;
					g_bWindowResize = true;
				}
				ImGui::PopFont();

				if (!s_bOpen) glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else
			{
				OutputDebugString(L"Inicio: ImGui failed to create window !");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::End();

		}
		else
		{
			// any wrong window enumeration ?
			OutputDebugString(L"Inicio: Code failure ! Wrong window enumeration!");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup ImGui + GL
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	if (g_hInjectionAbortEvent) CloseHandle(g_hInjectionAbortEvent);

	return (DWORD)0;
	}

#endif

#pragma endregion