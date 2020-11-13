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
#include "Inicio.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }

//#define INICIO_DRAWING_API AQU_DrawingAPIs::API_DirectDraw
#define INICIO_DRAWING_API AQU_DrawingAPIs::API_OpenGL

#define INICIO_FULL_TEXT_SIZE 1.0f
#define INICIO_MEDIUM_TEXT_SIZE 0.4f
#define INICIO_SMALL_TEXT_SIZE 0.25f
#define INICIO_SMALLER_TEXT_SIZE 0.215f
#define INICIO_TINY_TEXT_SIZE 0.15f

#pragma region Inicio controls methods

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

#pragma region Helpers

/**
* Hard coded control init.
***/
void InitControls()
{
	// init the controls, first set a zero origin
	g_vcZeroOrigin.x = 0;
	g_vcZeroOrigin.y = 0;

	// clear letter button bools
	for (int i = 0; i < 27; i++) g_bLetterButtonPressed[i] = false;

	// Exit : 
	SetRect(&g_sInicioControls[InicioControls::InicioExit].rcButtonRect, 658, 8, 668, 19);

	// New Project : 	
	SetRect(&g_sInicioControls[InicioControls::InicioNewProject].rcButtonRect, 400, 16, 460, 48);
	g_sInicioControls[InicioControls::InicioNewProject].szText = L"New";

	// Load Project : 	
	SetRect(&g_sInicioControls[InicioControls::InicioLoadProject].rcButtonRect, 470, 16, 530, 48);
	g_sInicioControls[InicioControls::InicioLoadProject].szText = L"Load";

	// Load Profile
	SetRect(&g_sInicioControls[InicioControls::InicioLoadProfile].rcButtonRect, 400, 104, 659, 171);
	g_sInicioControls[InicioControls::InicioLoadProfile].szText = L"";

	// Options : 
	SetRect(&g_sInicioControls[InicioControls::InicioOptions].rcButtonRect, 400, 58, 490, 90);
	g_sInicioControls[InicioControls::InicioOptions].szText = L"Options";

	// info : 
	SetRect(&g_sInicioControls[InicioControls::Info].rcButtonRect, 540, 16, 590, 48);
	g_sInicioControls[InicioControls::Info].szText = L"Info";

	// add profile window : 
	SetRect(&g_sInicioControls[InicioControls::AddProfileWindow].rcButtonRect, 500, 58, 590, 90);
	g_sInicioControls[InicioControls::AddProfileWindow].szText = L"Picture";

	// options Exit 
	SetRect(&g_sInicioControls[InicioControls::OptionsExit].rcButtonRect, 400, 8, 410, 19);

	// detour time delay slider
	SetRect(&g_sInicioControls[InicioControls::TimeDelaySlider].rcButtonRect, 590, 40, 790, 120);
	g_sInicioControls[InicioControls::TimeDelaySlider].nMinValue = 0;
	g_sInicioControls[InicioControls::TimeDelaySlider].nMaxValue = 20000;
	g_sInicioControls[InicioControls::TimeDelaySlider].nValue = 0;

	// injection repetition slider
	SetRect(&g_sInicioControls[InicioControls::InjectionRepetition].rcButtonRect, 590, 140, 790, 220);
	g_sInicioControls[InicioControls::InjectionRepetition].nMinValue = 0;
	g_sInicioControls[InicioControls::InjectionRepetition].nMaxValue = 15;
	g_sInicioControls[InicioControls::InjectionRepetition].nValue = 0;

	// keep process name for loading profile
	SetRect(&g_sInicioControls[InicioControls::KeepProcessName].rcButtonRect, 590, 220, 740, 260);
	g_sInicioControls[InicioControls::KeepProcessName].szText = L"Load Template";
	g_sInicioControls[InicioControls::KeepProcessName].bIsPressed = 0;

	// D3D selection
	SetRect(&g_sInicioControls[InicioControls::D3DSelection].rcButtonRect, 590, 280, 740, 320);
	g_sInicioControls[InicioControls::D3DSelection].dwEntries = 3;
	g_sInicioControls[InicioControls::D3DSelection].pszEntries = new LPWSTR[2];
	g_sInicioControls[InicioControls::D3DSelection].pszEntries[0] = L"D3D9";
	g_sInicioControls[InicioControls::D3DSelection].pszEntries[1] = L"D3D10";
	g_sInicioControls[InicioControls::D3DSelection].pszEntries[2] = L"D3D11";
	g_sInicioControls[InicioControls::D3DSelection].dwSelection = 0;
	g_sInicioControls[InicioControls::D3DSelection].bIsOpen = false;

	// select process Exit 
	SetRect(&g_sInicioControls[InicioControls::SelectProcessExit].rcButtonRect, 780, 8, 790, 19);

	// New Project Exit 
	SetRect(&g_sInicioControls[InicioControls::NewProjectExit].rcButtonRect, 450, 8, 460, 19);

	// hook selection
	SetRect(&g_sInicioControls[InicioControls::HookSelection].rcButtonRect, 8, 120, 136, 160);
	g_sInicioControls[InicioControls::HookSelection].dwEntries = 4;
	g_sInicioControls[InicioControls::HookSelection].pszEntries = new LPWSTR[3];
	g_sInicioControls[InicioControls::HookSelection].pszEntries[0] = L"No Injection";
	g_sInicioControls[InicioControls::HookSelection].pszEntries[1] = L"VMTable";
	g_sInicioControls[InicioControls::HookSelection].pszEntries[2] = L"Detour";
	g_sInicioControls[InicioControls::HookSelection].pszEntries[3] = L"Proxy";
	g_sInicioControls[InicioControls::HookSelection].dwSelection = 0;
	g_sInicioControls[InicioControls::HookSelection].bIsOpen = false;

	// Proceed to injection
	SetRect(&g_sInicioControls[InicioControls::ProceedToInjection].rcButtonRect, 8, 512, 128, 552);
	g_sInicioControls[InicioControls::ProceedToInjection].szText = L"Proceed";

	// next
	SetRect(&g_sInicioControls[InicioControls::NextPage].rcButtonRect, 8, 8, 88, 48);
	g_sInicioControls[InicioControls::NextPage].szText = L"Next";

	// Previous
	SetRect(&g_sInicioControls[InicioControls::PreviousPage].rcButtonRect, 8, 64, 88, 104);
	g_sInicioControls[InicioControls::PreviousPage].szText = L"Prev";

	// Ok
	SetRect(&g_sInicioControls[InicioControls::OK].rcButtonRect, 8, 120, 88, 160);
	g_sInicioControls[InicioControls::OK].szText = L"Ok";

	// Edit Mode
	SetRect(&g_sInicioControls[InicioControls::EditMode].rcButtonRect, 8, 120, 88, 160);
	g_sInicioControls[InicioControls::EditMode].szText = L"Editor";

	// colors list menu
	SetRect(&g_sInicioControls[InicioControls::ColorsList].rcButtonRect, 100, 16, 384, 552);
	g_sInicioControls[InicioControls::ColorsList].dwEntries = 0;
	g_sInicioControls[InicioControls::ColorsList].pszEntries = nullptr;
	g_sInicioControls[InicioControls::ColorsList].dwSelection = 0;
	g_sInicioControls[InicioControls::ColorsList].dwCurrentPage = 0;

	// game list menu
	SetRect(&g_sInicioControls[InicioControls::GameList].rcButtonRect, 150, 16, 564, 552);
	g_sInicioControls[InicioControls::GameList].dwEntries = 0;
	g_sInicioControls[InicioControls::GameList].pszEntries = nullptr;
	g_sInicioControls[InicioControls::GameList].dwSelection = 0;
	g_sInicioControls[InicioControls::GameList].dwCurrentPage = 0;

	// supported interfaces list menu
	SetRect(&g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect, 150, 16, 434, 552);
	g_sInicioControls[InicioControls::SupportedInterfacesList].dwEntries = 0;
	g_sInicioControls[InicioControls::SupportedInterfacesList].pszEntries = nullptr;
	g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection = 0;
	g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage = 0;

#ifdef DEVELOPER_BUILD
	// select processe Exit 
	SetRect(&g_sInicioControls[InicioControls::AddProcessExit].rcButtonRect, 400, 8, 410, 19);

	// process list menu
	SetRect(&g_sInicioControls[InicioControls::ProcessesList].rcButtonRect, 100, 16, 384, 552);
	g_sInicioControls[InicioControls::ProcessesList].dwEntries = 0;
	g_sInicioControls[InicioControls::ProcessesList].pszEntries = nullptr;
	g_sInicioControls[InicioControls::ProcessesList].dwSelection = 0;
	g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage = 0;

	// New Process
	SetRect(&g_sInicioControls[InicioControls::NewProcess].rcButtonRect, 8, 226, 88, 266);
	g_sInicioControls[InicioControls::NewProcess].szText = L"Add";
	// Save Processes
	SetRect(&g_sInicioControls[InicioControls::SaveProcessList].rcButtonRect, 8, 276, 88, 316);
	g_sInicioControls[InicioControls::SaveProcessList].szText = L"Save";
	// Load CSV
	SetRect(&g_sInicioControls[InicioControls::LoadCSV].rcButtonRect, 8, 326, 88, 366);
	g_sInicioControls[InicioControls::LoadCSV].szText = L"CSV>>";
	// Save CSV 
	SetRect(&g_sInicioControls[InicioControls::SaveCSV].rcButtonRect, 8, 376, 88, 416);
	g_sInicioControls[InicioControls::SaveCSV].szText = L">>CSV";
	// Save TXT 
	SetRect(&g_sInicioControls[InicioControls::SaveTXT].rcButtonRect, 8, 426, 88, 466);
	g_sInicioControls[InicioControls::SaveTXT].szText = L">>TXT";
#endif
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

/**
* Small control helper.
***/
bool InRect(RECT rc, POINT pt)
{
	return (pt.x >= rc.left) && (pt.y >= rc.top) && (pt.x <= rc.right) && (pt.y <= rc.bottom);
}

/**
* Set selection box entry.
* @param sInicioControl The selection box control.
***/
void SetSelectionBoxEntry(InicioControl &sInicioControl)
{
	// set the selection
	sInicioControl.dwSelection = (g_ptMouseCursor.y - sInicioControl.rcButtonRect.top)
		/ ((sInicioControl.rcButtonRect.bottom -
		sInicioControl.rcButtonRect.top) /
		sInicioControl.dwEntries);
	if (sInicioControl.dwSelection >= sInicioControl.dwEntries)
		sInicioControl.dwSelection = sInicioControl.dwEntries - 1;
}

/**
* Draws a selection box.
***/
void RenderSelectionBox(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize)
{
	// draw selection background
	pDirectDraw->Clear(sInicioControl.rcButtonRect, AquilinusColor::Color1);
	if (sInicioControl.bIsPressed)
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 4, AquilinusColor::Color2, 1.0f);
	else
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 4, AquilinusColor::Color0, 1.0f);

	if (sInicioControl.pszEntries)
		pDirectDraw->RenderText(
		sInicioControl.pszEntries[sInicioControl.dwSelection],
		sInicioControl.rcButtonRect.left + 16,
		sInicioControl.rcButtonRect.top + 16,
		g_vcZeroOrigin,
		AquilinusColor::Color4,
		AquilinusColor::Color1,
		fTextSize, 1.0f);
}

/**
* Draws the opened selection box entries.
***/
void RenderSelectionBoxEntries(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize)
{
	LONG nEntryHeigth = (sInicioControl.rcButtonRect.bottom -
		sInicioControl.rcButtonRect.top) /
		sInicioControl.dwEntries;

	// draw selection background
	pDirectDraw->Clear(sInicioControl.rcButtonRect, AquilinusColor::Color1);
	RECT rcSelection;
	SetRect(&rcSelection, sInicioControl.rcButtonRect.left,
		sInicioControl.rcButtonRect.top + nEntryHeigth * sInicioControl.dwSelection,
		sInicioControl.rcButtonRect.right,
		sInicioControl.rcButtonRect.top + nEntryHeigth * (sInicioControl.dwSelection + 1));
	pDirectDraw->Clear(rcSelection, AquilinusColor::Color0);

	if (sInicioControl.bIsPressed)
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 4, AquilinusColor::Color2, 1.0f);
	else
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 4, AquilinusColor::Color0, 1.0f);

	if (sInicioControl.pszEntries)
	{
		// loop throug entries and draw
		int nTop = sInicioControl.rcButtonRect.top;
		for (int i = 0; i < (int)sInicioControl.dwEntries; i++)
		{
			if (i == sInicioControl.dwSelection)
				pDirectDraw->RenderText(
				sInicioControl.pszEntries[i],
				sInicioControl.rcButtonRect.left + 16,
				nTop + 16,
				g_vcZeroOrigin,
				AquilinusColor::Color4,
				AquilinusColor::Color0,
				fTextSize, 1.0f);
			else
				pDirectDraw->RenderText(
				sInicioControl.pszEntries[i],
				sInicioControl.rcButtonRect.left + 16,
				nTop + 16,
				g_vcZeroOrigin,
				AquilinusColor::Color4,
				AquilinusColor::Color1,
				fTextSize, 1.0f);
			nTop += nEntryHeigth;
		}
	}
}

/**
* Draws a text selection list.
***/
void RenderSelectionTextList(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl)
{
	// loop throug entries and draw
	int nTop = sInicioControl.rcButtonRect.top;
	for (int i = 0; i < (int)16; i++)
	{
		DWORD dwIndex = i + sInicioControl.dwCurrentPage * 16;

		// is this index the current selection ? draw selection background...
		if (dwIndex == sInicioControl.dwSelection)
		{
			RECT rcSelection;
			SetRect(&rcSelection, sInicioControl.rcButtonRect.left,
				nTop + 10,
				sInicioControl.rcButtonRect.right,
				nTop + 42);
			pDirectDraw->Clear(rcSelection, AquilinusColor::Color0);
		}

		// render name
		if (dwIndex < sInicioControl.dwEntries)
		{
			if (dwIndex == sInicioControl.dwSelection)
				pDirectDraw->RenderText(
				sInicioControl.pszEntries[dwIndex],
				sInicioControl.rcButtonRect.left + 16,
				nTop + 16,
				g_vcZeroOrigin,
				AquilinusColor::Color2,
				AquilinusColor::Color0,
				INICIO_SMALL_TEXT_SIZE, 1.0f);
			else
				pDirectDraw->RenderText(
				sInicioControl.pszEntries[dwIndex],
				sInicioControl.rcButtonRect.left + 16,
				nTop + 16,
				g_vcZeroOrigin,
				AquilinusColor::Color2,
				AquilinusColor::Color1,
				INICIO_SMALL_TEXT_SIZE, 1.0f);

			nTop += 32;
		}
	}

	// draw a box for the list
	pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);

	// draw scroll bar
	RECT rcPage;
	LONG nTabTop = 24;
	LONG nBottom = g_nSubWindowHeight - 16;
	LONG nDistance = nBottom - nTabTop;
	LONG nTab = nDistance / (((sInicioControl.dwEntries - 1) / 16) + 1);

	nTabTop += sInicioControl.dwCurrentPage*nTab;
	SetRect(&rcPage, sInicioControl.rcButtonRect.right + 8, nTabTop, sInicioControl.rcButtonRect.right + 16, nTabTop + nTab);
	pDirectDraw->Clear(rcPage, AquilinusColor::Color4);

	// draw buttons
	if (sInicioControl.dwCurrentPage < ((sInicioControl.dwEntries - 1) / 16))
		RenderButton(pDirectDraw, g_sInicioControls[InicioControls::NextPage], INICIO_SMALL_TEXT_SIZE);
	if (sInicioControl.dwCurrentPage > 0)
		RenderButton(pDirectDraw, g_sInicioControls[InicioControls::PreviousPage], INICIO_SMALL_TEXT_SIZE);
}

/**
* Draws a button.
***/
void RenderButton(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize)
{
	// draw selection background
	pDirectDraw->Clear(sInicioControl.rcButtonRect, AquilinusColor::Color1);
	if (sInicioControl.bIsPressed)
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);
	else
		pDirectDraw->DrawBox(sInicioControl.rcButtonRect, g_vcZeroOrigin, 3, AquilinusColor::Color2, 1.0f);

	// and the text
	int nTab = (int)(fTextSize*48.0f);
	if (sInicioControl.szText)
		pDirectDraw->RenderText(
		sInicioControl.szText,
		sInicioControl.rcButtonRect.left + nTab,
		sInicioControl.rcButtonRect.top + nTab,
		g_vcZeroOrigin,
		AquilinusColor::Color4,
		AquilinusColor::Color1,
		fTextSize, 1.0f);
}

/**
* Draws a slider.
***/
void RenderSlider(AQU_Drawer* pDirectDraw, InicioControl &sInicioControl, float fTextSize)
{
	// draw selection background
	pDirectDraw->Clear(sInicioControl.rcButtonRect, AquilinusColor::Color1);

	// render slider belt
	RECT rc;
	SetRect(&rc, sInicioControl.rcButtonRect.left + 4, sInicioControl.rcButtonRect.top + 16, sInicioControl.rcButtonRect.right, sInicioControl.rcButtonRect.top + 20);
	pDirectDraw->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color2, 1.0f);

	// render slider
	float fSliderX = (float)(sInicioControl.nValue - sInicioControl.nMinValue) / (float)((sInicioControl.nMaxValue) - sInicioControl.nMinValue);
	int nSliderX = (int)(((float)sInicioControl.rcButtonRect.right - sInicioControl.rcButtonRect.left)*fSliderX) + sInicioControl.rcButtonRect.left;
	SetRect(&rc, nSliderX, sInicioControl.rcButtonRect.top + 4, nSliderX + 4, sInicioControl.rcButtonRect.top + 32);
	if (sInicioControl.bIsPressed)
		pDirectDraw->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);
	else
		pDirectDraw->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color2, 1.0f);

	// render text
	wchar_t sz[128];
	wsprintf(sz, L"%d", sInicioControl.nValue);
	pDirectDraw->RenderText(
		sz,
		sInicioControl.rcButtonRect.left + 4,
		sInicioControl.rcButtonRect.top + 42,
		g_vcZeroOrigin,
		AquilinusColor::Color4,
		AquilinusColor::Color1,
		fTextSize, 1.0f);

}

/**
* Sets new value for a slider control by mouse cursor.
* @param &sInicioControl The slider control.
* @param pt The mouse cursor.
***/
void SetSliderValue(InicioControl &sInicioControl, POINT pt)
{
	sInicioControl.nValue =
		(int)((float(g_ptMouseCursor.x - sInicioControl.rcButtonRect.left) /
		float(sInicioControl.rcButtonRect.right - sInicioControl.rcButtonRect.left))*
		(float(sInicioControl.nMaxValue - sInicioControl.nMinValue)));
}

/**
* Called for left mouse button down-event for a selection box.
**/
void HandleSelectionButtonDown(InicioControl &sInicioControl)
{
	// return if another sub menu is open
	if ((g_bSubMenuOpen) && (!sInicioControl.bIsOpen))
		return;

	// is the selection box open ?
	if (sInicioControl.bIsOpen)
	{
		SetSelectionBoxEntry(sInicioControl);
	}

	g_bControlActivated = true;
	sInicioControl.bIsPressed = true;
}

/**
* Called for left mouse button up-event for a selection box.
***/
void HandleSelectionButtonUp(InicioControl &sInicioControl)
{
	// return if another sub menu is open
	if ((g_bSubMenuOpen) && (!sInicioControl.bIsOpen))
		return;

	// get the current selection box height
	DWORD dwSelectionHeight = sInicioControl.rcButtonRect.bottom -
		sInicioControl.rcButtonRect.top;

	// open/close selection box
	sInicioControl.bIsPressed = false;
	sInicioControl.bIsOpen = !sInicioControl.bIsOpen;
	g_bSubMenuOpen = sInicioControl.bIsOpen;
	if (sInicioControl.bIsOpen)
	{
		SetRect(&sInicioControl.rcButtonRect, sInicioControl.rcButtonRect.left, sInicioControl.rcButtonRect.top, sInicioControl.rcButtonRect.right, sInicioControl.rcButtonRect.top + dwSelectionHeight*sInicioControl.dwEntries);
	}
	else
	{
		SetRect(&sInicioControl.rcButtonRect, sInicioControl.rcButtonRect.left, sInicioControl.rcButtonRect.top, sInicioControl.rcButtonRect.right, sInicioControl.rcButtonRect.top + dwSelectionHeight / sInicioControl.dwEntries);
	}
}

#pragma endregion

#pragma region SelectProcess window

/**
* Called for >SelectProcess< sub menu, whenever left mouse button is down.
***/
void HandleSelectProcessButtonDown(HWND hwnd)
{
	// set a letter rectangle
	RECT rc;
	for (int i = 0; i < 27; i++)
	{
		SetRect(&rc, 8 + ((i % 3) * 44), 108 + ((i / 3) * 44), 48 + ((i % 3) * 44), 148 + ((i / 3) * 44));
		if ((InRect(rc, g_ptMouseCursor)) && (g_nLetterEntriesNumber[i] > 0))
		{
			// set letter button bools
			g_bLetterButtonPressed[i] = true;
			g_bControlActivated = true;
			return;
		}
	}

	// go through active controls
	if (InRect(g_sInicioControls[InicioControls::SelectProcessExit].rcButtonRect, g_ptMouseCursor))
	{
		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		g_eInicioStatus = InicioStatus::Idle;
	}
	else if (InRect(g_sInicioControls[InicioControls::NextPage].rcButtonRect, g_ptMouseCursor))
	{
		// next page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::NextPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::PreviousPage].rcButtonRect, g_ptMouseCursor))
	{
		// previous page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::GameList].rcButtonRect, g_ptMouseCursor))
	{
		// Processes list... set new selection
		g_bControlActivated = true;
		LONG nBoxSelection = ((g_ptMouseCursor.y - g_sInicioControls[InicioControls::GameList].rcButtonRect.top - 10) / 32);
		if (nBoxSelection > 15) nBoxSelection = 15;
		LONG nNewSelection = g_sInicioControls[InicioControls::GameList].dwCurrentPage * 16 + nBoxSelection;

		if ((nNewSelection >= 0) && (nNewSelection < (LONG)g_sInicioControls[InicioControls::GameList].dwEntries))
			g_sInicioControls[InicioControls::GameList].dwSelection = nNewSelection;
	}
	else if (InRect(g_sInicioControls[InicioControls::KeepProcessName].rcButtonRect, g_ptMouseCursor))
	{
		if (!g_bControlActivated)
		{
			// keep process name ?
			g_bKeepProcessName = !g_bKeepProcessName;
			g_sInicioControls[InicioControls::KeepProcessName].bIsPressed = g_bKeepProcessName;

			g_bControlActivated = true;

			// instantly switch to load screen
			g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed = true;
		}
	}
	else if (InRect(g_sInicioControls[InicioControls::ProceedToInjection].rcButtonRect, g_ptMouseCursor))
	{
		// OK
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::TimeDelaySlider].rcButtonRect, g_ptMouseCursor))
	{
		// OK
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::TimeDelaySlider].bIsPressed = true;

		// set new value
		SetSliderValue(g_sInicioControls[InicioControls::TimeDelaySlider], g_ptMouseCursor);
		if (g_sInicioControls[InicioControls::TimeDelaySlider].nValue >= 0)
		{
			// the low 4 bits (0..15) value param is the injection repetition
			g_pAquilinusConfig->dwDetourTimeDelay = (DWORD)g_sInicioControls[InicioControls::TimeDelaySlider].nValue;
			g_pAquilinusConfig->dwDetourTimeDelay -= (DWORD)(g_sInicioControls[InicioControls::TimeDelaySlider].nValue & 15);
		}
	}
	else if (InRect(g_sInicioControls[InicioControls::InjectionRepetition].rcButtonRect, g_ptMouseCursor))
	{
		// OK
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::InjectionRepetition].bIsPressed = true;

		// set new value
		SetSliderValue(g_sInicioControls[InicioControls::InjectionRepetition], g_ptMouseCursor);
		if (g_sInicioControls[InicioControls::InjectionRepetition].nValue >= 0)
		{
			// the low 4 bits (0..15) value param is the injection repetition
			g_pAquilinusConfig->dwDetourTimeDelay -= (DWORD)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
			g_pAquilinusConfig->dwDetourTimeDelay = (DWORD)(g_sInicioControls[InicioControls::InjectionRepetition].nValue & 15);
		}
	}
	else if (InRect(g_sInicioControls[InicioControls::D3DSelection].rcButtonRect, g_ptMouseCursor))
	{
		// d3d selectino
		HandleSelectionButtonDown(g_sInicioControls[InicioControls::D3DSelection]);
	}
}

/**
* Called for >SelectProcess< sub menu, whenever left mouse button is up.
***/
void HandleSelectProcessButtonUp(HWND hwnd)
{
	// handle letter button bools
	for (int i = 0; i < 27; i++)
	{
		if (g_bLetterButtonPressed[i])
		{
			// set letter selection
			g_nCurrentLetterSelection = (DWORD)i;

			// delete old list
			if (g_sInicioControls[InicioControls::GameList].dwEntries > 0)
			{
				for (int j = 0; j < (int)g_sInicioControls[InicioControls::GameList].dwEntries; j++)
				{
					delete g_sInicioControls[InicioControls::GameList].pszEntries[j];
				}
			}

			// set game list control 
			g_sInicioControls[InicioControls::GameList].pszEntries = new LPWSTR[g_nLetterEntriesNumber[i]];
			g_sInicioControls[InicioControls::GameList].dwEntries = (DWORD)g_nLetterEntriesNumber[i];
			g_sInicioControls[InicioControls::GameList].dwCurrentPage = 0;
			g_sInicioControls[InicioControls::GameList].dwSelection = 0;

			// create strings
			for (int j = 0; j < (int)g_nLetterEntriesNumber[i]; j++)
			{
				g_sInicioControls[InicioControls::GameList].pszEntries[j] = new wchar_t[g_aszGameNames[j + g_nLetterStartIndex[i]].length() + 1];
				CopyMemory((void*)g_sInicioControls[InicioControls::GameList].pszEntries[j], (void*)g_aszGameNames[j + g_nLetterStartIndex[i]].c_str(), (g_aszGameNames[j + g_nLetterStartIndex[i]].length() + 1)*sizeof(wchar_t));
			}

			g_bLetterButtonPressed[i] = false;
		}
	}

	if (g_sInicioControls[InicioControls::NextPage].bIsPressed)
	{
		// next page
		g_sInicioControls[InicioControls::NextPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::GameList].dwCurrentPage < ((g_sInicioControls[InicioControls::GameList].dwEntries - 1) / 16))
			g_sInicioControls[InicioControls::GameList].dwCurrentPage++;
	}
	else if (g_sInicioControls[InicioControls::PreviousPage].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::GameList].dwCurrentPage > 0)
			g_sInicioControls[InicioControls::GameList].dwCurrentPage--;
	}
	else if (g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed)
	{
		// ok
		g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed = false;

		// get process id
		DWORD unOldProcessIndex = g_pAquilinusConfig->dwProcessIndex;
		for (int i = 0; i < (int)g_aszGameNamesUnsorted.size(); i++)
		{
			// compare strings from the sorted and unsorted game list
			if (g_aszGameNames[g_sInicioControls[InicioControls::GameList].dwSelection + g_nLetterStartIndex[g_nCurrentLetterSelection]].compare(g_aszGameNamesUnsorted[i]) == 0)
			{
				g_pAquilinusConfig->dwProcessIndex = i;
			}
		}

#ifdef DEVELOPER_BUILD
		// empty process chosen ?
		if (g_aszGameNames[g_sInicioControls[InicioControls::GameList].dwSelection + g_nLetterStartIndex[g_nCurrentLetterSelection]].compare(L"XYZ") == 0)
		{
			g_pAquilinusConfig->bEmptyProcess = true;
			g_pAquilinusConfig->dwProcessIndex = unOldProcessIndex;
		}
		else
			g_pAquilinusConfig->bEmptyProcess = false;
#else
		g_pAquilinusConfig->bEmptyProcess = false;
#endif

		// proceed...to new project window
		g_eCurrentSubWindow = InicioSubWindows::NewProject;
		g_nSubWindowWidth = NEW_PROJECT_WINDOW_WIDTH;
		g_nSubWindowHeight = NEW_PROJECT_WINDOW_HEIGHT;

		// set ALL classes to "NoInjection"
		for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
			g_pAquilinusConfig->eInjectionTechnique[i] = AQU_InjectionTechniques::NoInjection;

		// set injection technique based on d3d selection
		switch (g_sInicioControls[InicioControls::D3DSelection].dwSelection)
		{
			case 0: // D3D9
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] = AQU_InjectionTechniques::VMTable;
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9] = AQU_InjectionTechniques::VMTable;
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] = AQU_InjectionTechniques::VMTable;
				break;
			case 1: // D3D10
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] = AQU_InjectionTechniques::VMTable;
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = AQU_InjectionTechniques::VMTable;
				break;
			case 2: // D3D11
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] = AQU_InjectionTechniques::VMTable;
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] = AQU_InjectionTechniques::VMTable;
				g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = AQU_InjectionTechniques::VMTable;
				break;
			default:
				break;
		}

		// set the hook selection
		SetHookSelection(g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection);

		// enumerate supported interfaces.... set selection and page to zero first
		g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection = 0;
		g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage = 0;
		EnumerateSupportedInterfaces(g_sInicioControls[InicioControls::SupportedInterfacesList].pszEntries, g_sInicioControls[InicioControls::SupportedInterfacesList].dwEntries);

		// resize the back buffer
		g_pDirectDrawSub->Resize(g_nSubWindowWidth, g_nSubWindowHeight);

		// set the new window size
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, g_nSubWindowWidth, g_nSubWindowHeight, SWP_NOMOVE);

		// load template ? end window, call load workspace dialog
		if (g_bKeepProcessName)
		{
			// exit this sub window
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);

			// set to idle
			g_eInicioStatus = InicioStatus::Idle;
			SendMessage(g_hwnd, WM_MOUSEMOVE, NULL, NULL);

			// activate Load Project Button 
			g_bControlActivated = true;
			g_sInicioControls[InicioControls::InicioLoadProject].bIsPressed = true;
			SendMessage(g_hwnd, WM_LBUTTONUP, NULL, NULL);
		}

	}
	else if (g_sInicioControls[InicioControls::TimeDelaySlider].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::TimeDelaySlider].bIsPressed = false;
	}
	else if (g_sInicioControls[InicioControls::InjectionRepetition].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::InjectionRepetition].bIsPressed = false;
	}
	else if (g_sInicioControls[InicioControls::D3DSelection].bIsPressed)
	{
		// hook selection
		HandleSelectionButtonUp(g_sInicioControls[InicioControls::D3DSelection]);
	}
}

/**
* Render new project window.
***/
void RenderSelectProcess()
{
	// set a letter rectangle
	RECT rc;
	SetRect(&rc, 8, 108, 48, 148);

	// draw selection background
	g_pDirectDrawSub->Clear(rc, AquilinusColor::Color1);

	// draw border (only if entries present)
	if (g_nLetterEntriesNumber[0] > 0)
	{
		if (g_bLetterButtonPressed[0])
			g_pDirectDrawSub->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);
		else
			g_pDirectDrawSub->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color2, 1.0f);
	}

	// first, draw the zero
	RECT rc1;
	SetRect(&rc1, rc.left + 12, rc.top + 8, rc.right, rc.bottom);
	//g_pDirectDrawSub->RenderGlyph(g_pDirectDrawSub->GetGlyphIndex((char)'0'), rc1, rc1.left, rc1.top, g_vcZeroOrigin, AquilinusColor::Color4, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);

	// set new postions
	rc.left += 44;
	rc.right += 44;

	// set bool index
	int nIndex = 1;

	// loop through letters, render
	for (wchar_t c = 'A'; c <= 'Z'; c++)
	{
		// draw selection background
		g_pDirectDrawSub->Clear(rc, AquilinusColor::Color1);

		// draw border (only if entries present)
		if (g_nLetterEntriesNumber[nIndex] > 0)
		{
			if (g_bLetterButtonPressed[nIndex])
				g_pDirectDrawSub->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);
			else
				g_pDirectDrawSub->DrawBox(rc, g_vcZeroOrigin, 3, AquilinusColor::Color2, 1.0f);
		}

		RECT rc1;
		SetRect(&rc1, rc.left + 10, rc.top + 12, rc.right, rc.bottom);

		std::wstringstream sz; sz << c;
		g_pDirectDrawSub->RenderText(sz.str().c_str(), rc1.left, rc1.top, g_vcZeroOrigin, AquilinusColor::Color4, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);

		// set new postions
		rc.left += 44;
		rc.right += 44;
		if (rc.left >= 140)
		{
			rc.left = 8;
			rc.right = 48;
			rc.top += 44;
			rc.bottom =
				rc.top + 40;
		}

		nIndex++;
	}

	// render game list
	RenderSelectionTextList(g_pDirectDrawSub, g_sInicioControls[InicioControls::GameList]);

	// draw buttons
	if (g_sInicioControls[InicioControls::GameList].dwCurrentPage < ((g_sInicioControls[InicioControls::GameList].dwEntries - 1) / 16))
		RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::NextPage], INICIO_SMALL_TEXT_SIZE);
	if (g_sInicioControls[InicioControls::GameList].dwCurrentPage > 0)
		RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::PreviousPage], INICIO_SMALL_TEXT_SIZE);

	// render button >Proceed<
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::ProceedToInjection], INICIO_SMALL_TEXT_SIZE);

	// render slider >Detour time delay<
	RenderSlider(g_pDirectDrawSub, g_sInicioControls[InicioControls::TimeDelaySlider], INICIO_SMALL_TEXT_SIZE);
	g_pDirectDrawSub->RenderText(L"Detour time delay", 590, 20, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALLER_TEXT_SIZE, 1.0f);

	// render slider >Injection repetition<
	RenderSlider(g_pDirectDrawSub, g_sInicioControls[InicioControls::InjectionRepetition], INICIO_SMALL_TEXT_SIZE);
	g_pDirectDrawSub->RenderText(L"Injection repetition", 590, 120, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALLER_TEXT_SIZE, 1.0f);

	// render button >Keep Process Name<
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::KeepProcessName], INICIO_SMALL_TEXT_SIZE);

	// render selection box >D3D<
	RenderSelectionBox(g_pDirectDrawSub, g_sInicioControls[InicioControls::D3DSelection], INICIO_SMALLER_TEXT_SIZE);
	if (g_sInicioControls[InicioControls::D3DSelection].bIsOpen)
		RenderSelectionBoxEntries(g_pDirectDrawSub, g_sInicioControls[InicioControls::D3DSelection], INICIO_SMALLER_TEXT_SIZE);

	// and exit
	g_pDirectDrawSub->RenderText(L"x", g_sInicioControls[InicioControls::SelectProcessExit].rcButtonRect.left, 0, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
}

#pragma endregion

#pragma region NewProject window

/**
* Registers the window class for the new project window.
***/
HRESULT RegisterNewProjectWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	// register the new project window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProcSubWindow;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Aquilinus-Inicio new project class";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// exit if failed
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Aquilinus Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

/**
* Called for >NewProject< sub menu, whenever left mouse button is down.
***/
void HandleNewProjectButtonDown(HWND hwnd)
{
	// go through active controls
	if (InRect(g_sInicioControls[InicioControls::NewProjectExit].rcButtonRect, g_ptMouseCursor))
	{
		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		g_eInicioStatus = InicioStatus::Idle;
	}
	else if (InRect(g_sInicioControls[InicioControls::HookSelection].rcButtonRect, g_ptMouseCursor))
	{
		// hook selection
		HandleSelectionButtonDown(g_sInicioControls[InicioControls::HookSelection]);
	}
	else if (InRect(g_sInicioControls[InicioControls::NextPage].rcButtonRect, g_ptMouseCursor))
	{
		// next page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::NextPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::PreviousPage].rcButtonRect, g_ptMouseCursor))
	{
		// previous page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = true;
	}
	else if ((InRect(g_sInicioControls[InicioControls::ProceedToInjection].rcButtonRect, g_ptMouseCursor)) && (!g_bSubMenuOpen))
	{
		// proceed to injection
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect, g_ptMouseCursor))
	{
		// interface list... set new selection
		g_bControlActivated = true;
		LONG nBoxSelection = ((g_ptMouseCursor.y - g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect.top - 10) / 32);
		if (nBoxSelection > 15) nBoxSelection = 15;
		LONG nNewSelection = g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage * 16 + nBoxSelection;

		if ((nNewSelection >= 0) && (nNewSelection < (LONG)g_sInicioControls[InicioControls::SupportedInterfacesList].dwEntries) && (!g_sInicioControls[InicioControls::HookSelection].bIsOpen))
		{
			// set the new selection
			g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection = nNewSelection;

			// set the hook selection
			SetHookSelection(g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection);
		}
	}
}

/**
* Called for >NewProject< sub menu, whenever left mouse button is up.
***/
void HandleNewProjectButtonUp(HWND hwnd)
{
	if (g_sInicioControls[InicioControls::HookSelection].bIsPressed)
	{
		// hook selection
		HandleSelectionButtonUp(g_sInicioControls[InicioControls::HookSelection]);

		// set the config setting
		DWORD dwSelection = g_sInicioControls[InicioControls::HookSelection].dwSelection;
		if (!AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection].bInjectionTechnique[0])
			dwSelection++;
		if ((!AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection].bInjectionTechnique[1])
			&& (dwSelection > 0))
			dwSelection++;
		if ((!AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection].bInjectionTechnique[2])
			&& (dwSelection > 1))
			dwSelection++;
		g_pAquilinusConfig->eInjectionTechnique[g_sInicioControls[InicioControls::SupportedInterfacesList].dwSelection] = (AQU_InjectionTechniques)dwSelection;
	}
	else if (g_sInicioControls[InicioControls::NextPage].bIsPressed)
	{
		// next page
		g_sInicioControls[InicioControls::NextPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage < ((g_sInicioControls[InicioControls::SupportedInterfacesList].dwEntries - 1) / 16))
			g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage++;
	}
	else if (g_sInicioControls[InicioControls::PreviousPage].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage > 0)
			g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage--;
	}
	else if (g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed)
	{
		// proceed
		g_sInicioControls[InicioControls::ProceedToInjection].bIsPressed = false;

		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);

		// start to inject.... send wm_mousemove to inject
		g_eInicioStatus = InicioStatus::ToInject;
		SendMessage(g_hwnd, WM_MOUSEMOVE, NULL, NULL);
	}

}

/**
* Updates the hook selection for the currently chosen D3D interface.
***/
void SetHookSelection(DWORD dwSelection)
{
	// enumerate the supported hooking techniques for the chosen interface
	g_sInicioControls[InicioControls::HookSelection].dwEntries = 0;
	DWORD counter = 0;
	if (AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[dwSelection].bInjectionTechnique[0])
	{
		// set text
		g_sInicioControls[InicioControls::HookSelection].pszEntries[counter] = L"No Injection";

		// set the selection based on the configuration setting
		if (g_pAquilinusConfig->eInjectionTechnique[dwSelection] == AQU_InjectionTechniques::NoInjection)
			g_sInicioControls[InicioControls::HookSelection].dwSelection = counter;

		// increment entry counter
		counter++;
		g_sInicioControls[InicioControls::HookSelection].dwEntries = counter;
	}
	if (AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[dwSelection].bInjectionTechnique[1])
	{
		// set text
		g_sInicioControls[InicioControls::HookSelection].pszEntries[counter] = L"VMTable";

		// set the selection based on the configuration setting
		if (g_pAquilinusConfig->eInjectionTechnique[dwSelection] == AQU_InjectionTechniques::VMTable)
			g_sInicioControls[InicioControls::HookSelection].dwSelection = counter;

		// increment entry counter
		counter++;
		g_sInicioControls[InicioControls::HookSelection].dwEntries = counter;
	}
	if (AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[dwSelection].bInjectionTechnique[2])
	{
		// set text
		g_sInicioControls[InicioControls::HookSelection].pszEntries[counter] = L"Detour";

		// set the selection based on the configuration setting
		if (g_pAquilinusConfig->eInjectionTechnique[dwSelection] == AQU_InjectionTechniques::Detour)
			g_sInicioControls[InicioControls::HookSelection].dwSelection = counter;

		// increment entry counter
		counter++;
		g_sInicioControls[InicioControls::HookSelection].dwEntries = counter;
	}
	if (AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[dwSelection].bInjectionTechnique[3])
	{
		// set text
		g_sInicioControls[InicioControls::HookSelection].pszEntries[counter] = L"Proxy";

		// set the selection based on the configuration setting
		if (g_pAquilinusConfig->eInjectionTechnique[dwSelection] == AQU_InjectionTechniques::Proxy)
			g_sInicioControls[InicioControls::HookSelection].dwSelection = counter;

		// increment entry counter
		counter++;
		g_sInicioControls[InicioControls::HookSelection].dwEntries = counter;
	}
}

/**
* Render new project window.
***/
void RenderNewProject()
{
	// render the text list
	RenderSelectionTextList(g_pDirectDrawSub, g_sInicioControls[InicioControls::SupportedInterfacesList]);

	// render injection indicators
	int nStart = (int)g_sInicioControls[InicioControls::SupportedInterfacesList].dwCurrentPage * 16;
	for (int i = nStart; (i < nStart + 16) && (i < SUPPORTED_INTERFACES_NUMBER); i++)
	if (g_pAquilinusConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
	{
		// set the indicator rectangle
		int nTop = g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect.top + ((i - nStart) * 32);
		RECT rcIndicator;
		SetRect(&rcIndicator, g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect.left,
			nTop + 10,
			g_sInicioControls[InicioControls::SupportedInterfacesList].rcButtonRect.left + 12,
			nTop + 42);

		// draw different colors depending on injection technique
		switch (g_pAquilinusConfig->eInjectionTechnique[i])
		{
			case AQU_InjectionTechniques::VMTable:
				g_pDirectDrawSub->Clear(rcIndicator, AquilinusColor::Color2);
				break;
			case AQU_InjectionTechniques::Detour:
				g_pDirectDrawSub->Clear(rcIndicator, AquilinusColor::Color3);
				break;
			case AQU_InjectionTechniques::Proxy:
				g_pDirectDrawSub->Clear(rcIndicator, AquilinusColor::Color4);
				break;
			default:
				break;
		}
	}

	// render button >Proceed<
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::ProceedToInjection], INICIO_SMALL_TEXT_SIZE);

	// render selection box
	RenderSelectionBox(g_pDirectDrawSub, g_sInicioControls[InicioControls::HookSelection], INICIO_SMALLER_TEXT_SIZE);
	if (g_sInicioControls[InicioControls::HookSelection].bIsOpen)
		RenderSelectionBoxEntries(g_pDirectDrawSub, g_sInicioControls[InicioControls::HookSelection], INICIO_SMALLER_TEXT_SIZE);

	// and exit
	g_pDirectDrawSub->RenderText(L"x", g_sInicioControls[InicioControls::NewProjectExit].rcButtonRect.left, 0, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);

}

#pragma endregion

#pragma region Options window

/**
* Called for >Options< sub menu, whenever left mouse button is down.
***/
void HandleOptionsButtonDown(HWND hwnd)
{
	// go through active controls
	if (InRect(g_sInicioControls[InicioControls::OptionsExit].rcButtonRect, g_ptMouseCursor))
	{
		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		g_eInicioStatus = InicioStatus::Idle;
	}
	else if (InRect(g_sInicioControls[InicioControls::NextPage].rcButtonRect, g_ptMouseCursor))
	{
		// next page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::NextPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::PreviousPage].rcButtonRect, g_ptMouseCursor))
	{
		// previous page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::ColorsList].rcButtonRect, g_ptMouseCursor))
	{
		// Processes list... set new selection
		int nTabY = (g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom - g_sInicioControls[InicioControls::ColorsList].rcButtonRect.top) / 8;
		g_bControlActivated = true;
		LONG nBoxSelection = ((g_ptMouseCursor.y - g_sInicioControls[InicioControls::ColorsList].rcButtonRect.top) / nTabY);
		if (nBoxSelection > 8) nBoxSelection = 8;
		LONG nNewSelection = g_sInicioControls[InicioControls::ColorsList].dwCurrentPage * 8 + nBoxSelection;

		if ((nNewSelection >= 0) && (nNewSelection < (LONG)g_pDirectDrawSub->GetColorSchemesNumber()))
			g_sInicioControls[InicioControls::ColorsList].dwSelection = nNewSelection;
		g_pAquilinusConfig->dwColorSchemeIndex = nNewSelection;

		// save global options
		g_dwColorSchemeIndex = nNewSelection;
		SaveConfig();

		// set new color !
		g_pDirectDrawSub->SetColorScheme((DWORD)g_sInicioControls[InicioControls::ColorsList].dwSelection);
		g_pDirectDraw->SetColorScheme(g_pDirectDrawSub);
		SendMessage(hwnd, WM_PAINT, NULL, NULL);
		SendMessage(g_hwnd, WM_PAINT, NULL, NULL);

	}
	else if (g_ptMouseCursor.y > (g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 16))
	{
		// redirect to colourlovers.com
		ColorScheme cs;
		g_pDirectDrawSub->GetColorScheme(cs, g_pAquilinusConfig->dwColorSchemeIndex);
		ShellExecute(NULL, L"open", cs.szLink, NULL, NULL, SW_SHOWNORMAL);
	}
#ifdef DEVELOPER_BUILD
	else if (InRect(g_sInicioControls[InicioControls::NewProcess].rcButtonRect, g_ptMouseCursor))
	{
		// SaveProcessList
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::NewProcess].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::SaveProcessList].rcButtonRect, g_ptMouseCursor))
	{
		// SaveProcessList
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::SaveProcessList].bIsPressed = true;

		// save process list
		g_pFileManager->SaveProcessList();
	}
	else if (InRect(g_sInicioControls[InicioControls::SaveCSV].rcButtonRect, g_ptMouseCursor))
	{
		// SaveCSV
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::SaveCSV].bIsPressed = true;

		// save process list
		g_pFileManager->SaveProcessListCSV();
	}
	else if (InRect(g_sInicioControls[InicioControls::LoadCSV].rcButtonRect, g_ptMouseCursor))
	{
		// SaveProcessList
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::LoadCSV].bIsPressed = true;

		// save process list
		g_pFileManager->LoadProcessListCSV();
	}
	else if (InRect(g_sInicioControls[InicioControls::SaveTXT].rcButtonRect, g_ptMouseCursor))
	{
		// SaveProcessList
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::SaveTXT].bIsPressed = true;

		// save process list
		g_pFileManager->SaveGameListTXT();
	}
#endif
	else if (InRect(g_sInicioControls[InicioControls::EditMode].rcButtonRect, g_ptMouseCursor))
	{
		if (!g_bControlActivated)
		{
			// keep process name ?
			g_pAquilinusConfig->bAlwaysForceD3D = !g_pAquilinusConfig->bAlwaysForceD3D;
			g_sInicioControls[InicioControls::EditMode].bIsPressed = (g_pAquilinusConfig->bAlwaysForceD3D == TRUE);

			g_bControlActivated = true;
		}
	}
}

/**
* Called for >Options< sub menu, whenever left mouse button is up.
***/
void HandleOptionsButtonUp(HWND hwnd)
{
	if (g_sInicioControls[InicioControls::NextPage].bIsPressed)
	{
		// next page
		g_sInicioControls[InicioControls::NextPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::ColorsList].dwCurrentPage < ((g_pDirectDrawSub->GetColorSchemesNumber() - 1) / 8))
			g_sInicioControls[InicioControls::ColorsList].dwCurrentPage++;
	}
	else if (g_sInicioControls[InicioControls::PreviousPage].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::ColorsList].dwCurrentPage > 0)
			g_sInicioControls[InicioControls::ColorsList].dwCurrentPage--;
	}
#ifdef DEVELOPER_BUILD
	else if (g_sInicioControls[InicioControls::NewProcess].bIsPressed)
	{
		// add process
		g_sInicioControls[InicioControls::NewProcess].bIsPressed = false;

		// enumerate current processes.... set selection and page to zero first
		g_sInicioControls[InicioControls::ProcessesList].dwSelection = 0;
		g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage = 0;
		EnumerateProcesses(g_sInicioControls[InicioControls::ProcessesList].pszEntries, g_sInicioControls[InicioControls::ProcessesList].dwEntries);

		// proceed...to new project window
		g_eCurrentSubWindow = InicioSubWindows::AddProcess;
		g_nSubWindowWidth = ADD_PROCESS_WINDOW_WIDTH;
		g_nSubWindowHeight = ADD_PROCESS_WINDOW_HEIGHT;

		// resize the back buffer
		g_pDirectDrawSub->Resize(g_nSubWindowWidth, g_nSubWindowHeight);

		// set the new window size
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, g_nSubWindowWidth, g_nSubWindowHeight, SWP_NOMOVE);
	}
	else if (g_sInicioControls[InicioControls::SaveProcessList].bIsPressed)
	{
		// save process list
		g_sInicioControls[InicioControls::SaveProcessList].bIsPressed = false;
	}
	else if (g_sInicioControls[InicioControls::SaveCSV].bIsPressed)
	{
		// SaveCSV
		g_sInicioControls[InicioControls::SaveCSV].bIsPressed = false;
	}
	else if (g_sInicioControls[InicioControls::LoadCSV].bIsPressed)
	{
		// LoadCSV
		g_sInicioControls[InicioControls::LoadCSV].bIsPressed = false;
	}
	else if (g_sInicioControls[InicioControls::SaveTXT].bIsPressed)
	{
		// SaveTXT
		g_sInicioControls[InicioControls::SaveTXT].bIsPressed = false;
	}
#endif
}

/**
* Render new project window.
***/
void RenderOptions()
{
	ColorScheme cs;

	// loop throug entries and draw
	DWORD dwSchemesNumber = g_pDirectDrawSub->GetColorSchemesNumber();
	int nTabY = (g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom - g_sInicioControls[InicioControls::ColorsList].rcButtonRect.top) / 8;
	int nTabX = (g_sInicioControls[InicioControls::ColorsList].rcButtonRect.right - g_sInicioControls[InicioControls::ColorsList].rcButtonRect.left) / 5;
	RECT rcColor;
	SetRect(&rcColor, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.left + 6,
		g_sInicioControls[InicioControls::ColorsList].rcButtonRect.top + 6,
		g_sInicioControls[InicioControls::ColorsList].rcButtonRect.left + nTabX - 2,
		g_sInicioControls[InicioControls::ColorsList].rcButtonRect.top + (nTabY / 2) - 2);

	// clear a black box
	g_pDirectDrawSub->Clear(g_sInicioControls[InicioControls::ColorsList].rcButtonRect, AquilinusColor::Color0);
	for (int i = 0; i < (int)8; i++)
	{
		DWORD dwIndex = i + g_sInicioControls[InicioControls::ColorsList].dwCurrentPage * 8;

		// is this index the current selection ? draw selection background...
		if (dwIndex == g_sInicioControls[InicioControls::ColorsList].dwSelection)
		{
			RECT rcSelection;
			SetRect(&rcSelection, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.left + 2,
				rcColor.top - 2,
				g_sInicioControls[InicioControls::ColorsList].rcButtonRect.right - 2,
				rcColor.bottom + (nTabY / 2));
			g_pDirectDrawSub->DrawBox(rcSelection, g_vcZeroOrigin, 2, AquilinusColor::Color2, 1.0f);
		}

		// draw the colors
		if (dwIndex < (int)dwSchemesNumber)
		{
			g_pDirectDrawSub->GetColorScheme(cs, dwIndex);
			g_pDirectDrawSub->Clear(rcColor, cs.dwColor0);
			rcColor.left += nTabX;
			rcColor.right += nTabX;
			g_pDirectDrawSub->Clear(rcColor, cs.dwColor1);
			rcColor.left += nTabX;
			rcColor.right += nTabX;
			g_pDirectDrawSub->Clear(rcColor, cs.dwColor2);
			rcColor.left += nTabX;
			rcColor.right += nTabX;
			g_pDirectDrawSub->Clear(rcColor, cs.dwColor3);
			rcColor.left += nTabX;
			rcColor.right += nTabX;
			g_pDirectDrawSub->Clear(rcColor, cs.dwColor4);
			rcColor.left -= 4 * nTabX;
			rcColor.top += nTabY / 2;
			g_pDirectDrawSub->RenderText(cs.szName, rcColor.left, rcColor.top + 2, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
			rcColor.right -= 4 * nTabX;
			rcColor.top += nTabY / 2;
			rcColor.bottom += nTabY;
		}

	}

	// draw a box for the color list
	g_pDirectDrawSub->DrawBox(g_sInicioControls[InicioControls::ColorsList].rcButtonRect, g_vcZeroOrigin, 3, AquilinusColor::Color4, 1.0f);

	// draw a box for the page
	RECT rcPage;
	LONG nTabTop = g_sInicioControls[InicioControls::OptionsExit].rcButtonRect.bottom + 8;
	LONG nBottom = g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom;
	LONG nDistance = nBottom - nTabTop;
	LONG nTab = nDistance / (((dwSchemesNumber - 1) / 8) + 1);

	nTabTop += g_sInicioControls[InicioControls::ColorsList].dwCurrentPage*nTab;
	SetRect(&rcPage, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.right + 8, nTabTop, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.right + 16, nTabTop + nTab);
	g_pDirectDrawSub->Clear(rcPage, AquilinusColor::Color4);

	// draw buttons
	if (g_sInicioControls[InicioControls::ColorsList].dwCurrentPage < ((g_pDirectDrawSub->GetColorSchemesNumber() - 1) / 8))
		RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::NextPage], INICIO_SMALL_TEXT_SIZE);
	if (g_sInicioControls[InicioControls::ColorsList].dwCurrentPage > 0)
		RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::PreviousPage], INICIO_SMALL_TEXT_SIZE);
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::EditMode], INICIO_SMALL_TEXT_SIZE);

#ifdef DEVELOPER_BUILD
	g_pDirectDrawSub->RenderText(L"Dev :", g_sInicioControls[InicioControls::NewProcess].rcButtonRect.left, 185, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);

	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::NewProcess], INICIO_SMALL_TEXT_SIZE);
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::SaveProcessList], INICIO_SMALL_TEXT_SIZE);
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::LoadCSV], INICIO_SMALL_TEXT_SIZE);
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::SaveCSV], INICIO_SMALL_TEXT_SIZE);
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::SaveTXT], INICIO_SMALL_TEXT_SIZE);
#endif

	// render current color profile name
	g_pDirectDrawSub->GetColorScheme(cs, g_pAquilinusConfig->dwColorSchemeIndex);
	g_pDirectDrawSub->RenderText(L"Current COLOURlovers Palette :", 16, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 16, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
	g_pDirectDrawSub->RenderText(L"COLOURlovers", 107, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 16, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
	g_pDirectDrawSub->RenderText(cs.szName, 16, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 48, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
	wchar_t buf[64];
	wsprintf(buf, L"by Lover : %s", cs.szLover);
	g_pDirectDrawSub->RenderText(buf, 16, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 70, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
	g_pDirectDrawSub->RenderText(L"..click to redirect to colorlovers.com", 16, g_sInicioControls[InicioControls::ColorsList].rcButtonRect.bottom + 92, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);

	// and exit
	g_pDirectDrawSub->RenderText(L"x", g_sInicioControls[InicioControls::OptionsExit].rcButtonRect.left, 0, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
}

#pragma endregion

#endif

#ifdef DEVELOPER_BUILD

#pragma region AddProcess window

/**
* Game name text input dialog procedure.
***/
BOOL CALLBACK TextInputProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			// set window text as game name assumption
			SetDlgItemText(hwndDlg, IDC_INPUTBOX_01, g_szWindow);

			// output process and window text
			SetDlgItemText(hwndDlg, IDC_WINDOW_NAME, g_szWindow);
			SetDlgItemText(hwndDlg, IDC_PROC_NAME, g_szProc);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					// set game text
					if (!GetDlgItemText(hwndDlg, IDC_INPUTBOX_01, g_szGame, 80))
						*g_szGame = 0;
				case IDCANCEL:
					EndDialog(hwndDlg, wParam);
					return TRUE;
			}
			return TRUE;
	}
	return FALSE;
}

/**
* Called for >AddProcess< sub menu, whenever left mouse button is down.
***/
void HandleAddProcessButtonDown(HWND hwnd)
{
	// go through active controls
	if (InRect(g_sInicioControls[InicioControls::AddProcessExit].rcButtonRect, g_ptMouseCursor))
	{
		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		g_eInicioStatus = InicioStatus::Idle;
	}
	else if (InRect(g_sInicioControls[InicioControls::NextPage].rcButtonRect, g_ptMouseCursor))
	{
		// next page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::NextPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::PreviousPage].rcButtonRect, g_ptMouseCursor))
	{
		// previous page
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::OK].rcButtonRect, g_ptMouseCursor))
	{
		// OK
		g_bControlActivated = true;
		g_sInicioControls[InicioControls::OK].bIsPressed = true;
	}
	else if (InRect(g_sInicioControls[InicioControls::ProcessesList].rcButtonRect, g_ptMouseCursor))
	{
		// Processes list... set new selection
		g_bControlActivated = true;
		LONG nBoxSelection = ((g_ptMouseCursor.y - g_sInicioControls[InicioControls::ProcessesList].rcButtonRect.top - 10) / 32);
		if (nBoxSelection > 15) nBoxSelection = 15;
		LONG nNewSelection = g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage * 16 + nBoxSelection;

		if ((nNewSelection >= 0) && (nNewSelection < (LONG)g_sInicioControls[InicioControls::ProcessesList].dwEntries))
			g_sInicioControls[InicioControls::ProcessesList].dwSelection = nNewSelection;
	}
}

/**
* Called for >AddProcess< sub menu, whenever left mouse button is up.
***/
void HandleAddProcessButtonUp(HWND hwnd)
{
	if (g_sInicioControls[InicioControls::NextPage].bIsPressed)
	{
		// next page
		g_sInicioControls[InicioControls::NextPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage < ((g_sInicioControls[InicioControls::ProcessesList].dwEntries - 1) / 16))
			g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage++;
	}
	else if (g_sInicioControls[InicioControls::PreviousPage].bIsPressed)
	{
		// previous page
		g_sInicioControls[InicioControls::PreviousPage].bIsPressed = false;

		if (g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage > 0)
			g_sInicioControls[InicioControls::ProcessesList].dwCurrentPage--;
	}
	else if (g_sInicioControls[InicioControls::OK].bIsPressed)
	{
		// ok
		g_sInicioControls[InicioControls::OK].bIsPressed = false;

		// process name
		CopyMemory((PVOID)g_szProc,
			(PVOID)g_sInicioControls[InicioControls::ProcessesList].pszEntries[g_sInicioControls[InicioControls::ProcessesList].dwSelection],
			(wcslen(g_sInicioControls[InicioControls::ProcessesList].pszEntries[g_sInicioControls[InicioControls::ProcessesList].dwSelection]) + 1) * sizeof(wchar_t));

		// get proc id
		DWORD pidThis = GetTargetThreadID(g_szProc);

		// catch window 
		HWND h = ::GetTopWindow(0);
		bool bFound = false;
		while (h)
		{
			DWORD pid;
			DWORD dwTheardId = ::GetWindowThreadProcessId(h, &pid);

			// right pid and window visible?
			if ((pid == pidThis) && (IsWindowVisible(h)))
			{
				bFound = true;
				break;
			}

			h = ::GetNextWindow(h, GW_HWNDNEXT);
		}

		// get window name
		if (bFound)
			GetWindowText(h, g_szWindow, sizeof(g_szWindow));
		else
		{
			LPWSTR szNo = L"No Window name !";
			ZeroMemory(&g_szWindow, MAX_PATH*sizeof(wchar_t));
			CopyMemory((PVOID)&g_szWindow, (PVOID)&szNo, (wcslen(szNo) + 1)*sizeof(wchar_t));
		}

		// call text input dialog
		if (DialogBox(g_hInstance,
			MAKEINTRESOURCE(IDD_INPUTBOX),
			hwnd,
			(DLGPROC)TextInputProc) == IDOK)
		{
			// add process
			g_pFileManager->AddProcess(g_szGame, g_szWindow, g_szProc);

			// output debug string here
			OutputDebugString(g_pFileManager->GetName(g_pFileManager->GetProcessNumber() - 1));
			OutputDebugString(g_pFileManager->GetWindowName(g_pFileManager->GetProcessNumber() - 1));
			OutputDebugString(g_pFileManager->GetProcessName(g_pFileManager->GetProcessNumber() - 1));
		}

		// exit this sub window
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		g_eInicioStatus = InicioStatus::Idle;
	}
}

/**
* Render add process window.
***/
void RenderAddProcess()
{
	// render the text list
	RenderSelectionTextList(g_pDirectDrawSub, g_sInicioControls[InicioControls::ProcessesList]);

	// render button >OK<
	RenderButton(g_pDirectDrawSub, g_sInicioControls[InicioControls::OK], INICIO_SMALL_TEXT_SIZE);

	// and exit
	g_pDirectDrawSub->RenderText(L"x", g_sInicioControls[InicioControls::AddProcessExit].rcButtonRect.left, 0, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color1, INICIO_SMALL_TEXT_SIZE, 1.0f);
}

/**
* Enumerate currently running exe process names.
***/
DWORD EnumerateProcesses(LPWSTR *&pszEntries, DWORD &dwEntries)
{
	PROCESSENTRY32W pe32;
	HANDLE hSnapShot;
	BOOL bFoundProc = false;

	// get snapshot handle
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Aquilinus : Unable create toolhelp snapshot!");
		return 0;
	}

	// loop through processes, count them
	pe32.dwSize = sizeof(PROCESSENTRY32W);
	bFoundProc = Process32FirstW(hSnapShot, &pe32);
	dwEntries = 0;
	while (bFoundProc)
	{
		// is an exe process ?
		int len = (int)wcslen(pe32.szExeFile);
		if (len > 0)
		{
			if ((pe32.szExeFile[len - 1] == 'e') &&
				(pe32.szExeFile[len - 2] == 'x') &&
				(pe32.szExeFile[len - 3] == 'e'))
				dwEntries++;
		}
		bFoundProc = Process32NextW(hSnapShot, &pe32);
	}

	// loop again, enumerate
	pszEntries = new LPWSTR[dwEntries];
	DWORD nIndex = 0;
	bFoundProc = Process32FirstW(hSnapShot, &pe32);
	while (bFoundProc)
	{
		// is an exe process ?
		int len = (int)wcslen(pe32.szExeFile);
		if (len > 0)
		{
			if ((pe32.szExeFile[len - 1] == 'e') &&
				(pe32.szExeFile[len - 2] == 'x') &&
				(pe32.szExeFile[len - 3] == 'e'))
			{
				pszEntries[nIndex] = new wchar_t[wcslen(pe32.szExeFile) + sizeof(wchar_t)];
				CopyMemory((PVOID)pszEntries[nIndex], pe32.szExeFile, (wcslen(pe32.szExeFile) * sizeof(wchar_t)+sizeof(wchar_t)));
				nIndex++;
			}
		}
		bFoundProc = Process32NextW(hSnapShot, &pe32);
	}

	return 0;
}

#pragma endregion

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
	TerminateThread(g_hInjectionThread, S_OK);
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
		TerminateThread(g_hInjectionThread, S_OK);

	// kill profile window
	if (g_hProfileWindow)
	{
		if (IsWindow(g_hProfileWindow)) SendMessage(g_hProfileWindow, WM_CLOSE, NULL, NULL);
		g_hProfileWindow = NULL;
	}

	// kill sub window
	if (g_hSubWindow)
	{
		if (IsWindow(g_hSubWindow)) SendMessage(g_hSubWindow, WM_CLOSE, NULL, NULL);
		g_hSubWindow = NULL;
	}

	// release the additional bitmap
	if (g_pDirectDraw)
		g_pDirectDraw->ReleaseBackground();

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

#ifdef DEVELOPER_BUILD
	// add an empty process
	g_aszGameNames.push_back(L"XYZ");
	g_aszGameNamesUnsorted.push_back(L"XYZ");
#endif

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

	// clear game list
	g_nCurrentLetterSelection = 0;

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT
	if (g_sInicioControls[InicioControls::GameList].pszEntries) delete[] g_sInicioControls[InicioControls::GameList].pszEntries;
	g_sInicioControls[InicioControls::GameList].dwEntries = 0;
	g_sInicioControls[InicioControls::GameList].dwSelection = 0;
#endif

	return S_OK;
}

/**
* Enumerate supported interface names.
***/
HRESULT EnumerateSupportedInterfaces(LPWSTR *&pszEntries, DWORD &dwEntries)
{
	// loop and enumerate
	dwEntries = SUPPORTED_INTERFACES_NUMBER;
	pszEntries = new LPWSTR[dwEntries];
	for (int nIndex = 0; nIndex < SUPPORTED_INTERFACES_NUMBER; nIndex++)
	{
		pszEntries[nIndex] = new wchar_t[wcslen(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName) + sizeof(wchar_t)];
		CopyMemory((PVOID)pszEntries[nIndex], AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName, (wcslen(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[nIndex].szName) * sizeof(wchar_t)+sizeof(wchar_t)));
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
DWORD GetTargetThreadID(wchar_t * szProcName)
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
HRESULT Inject(DWORD dwID, const wchar_t * szDllName)
{
	HANDLE hProc;
	HANDLE hToken;
	wchar_t szBuffer[128] = { 0 };
	LPVOID pRemoteString, pLoadLibraryA;

	if (!dwID)
		return false;

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
	pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
	if (pLoadLibraryA == NULL)
	{
		wsprintf(szBuffer, L"Aquilinus : Failed to get the address of >LoadLibraryW< : %d", GetLastError());
		OutputDebugString(szBuffer);
		return E_FAIL;
	}

	// convert to LPCSTR
	int size = (int)wcslen(szDllName);
	size += 2;
	char *szDll = (char *)malloc(size);
	wcstombs_s(NULL, szDll, size,
		szDllName, size);

	// Allocate space in the process for our DLL 
	pRemoteString = (LPVOID)VirtualAllocEx(hProc, NULL, wcslen(szDllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(hProc, (LPVOID)pRemoteString, szDll, wcslen(szDllName), NULL);

	// free memory
	free(szDll);

	// Load our DLL
	if (CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibraryA, (LPVOID)pRemoteString, NULL, NULL) == NULL)
	{
		wsprintf(szBuffer, L"Aquilinus : Failed to create >LoadLibraryA< remote thread : %d", GetLastError());
		OutputDebugString(szBuffer);
		return E_FAIL;
	}

	CloseHandle(hToken);
	CloseHandle(hProc);
	return true;
}

/**
* Thread to inject.
***/
DWORD WINAPI InjectionThread(LPVOID Param)
{
	// injection repeat...
	int nRepeat = *(int*)Param;

	// Retrieve process ID
	DWORD dwID = NULL;
	while (dwID == NULL)
	{
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
		CopyMemory((void*)szProc, (void*)szP.c_str(), (szP.length() + 1)*sizeof(wchar_t));

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

/**
* Profile window thread.
***/
DWORD WINAPI ProfileWindowThread(LPVOID Param)
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	// get filename using the OPENFILENAME structure and GetOpenFileName()
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPCWSTR)L"PNG Image File (*.png)\0*.png\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = (LPCWSTR)L"png";
	ofn.lpstrTitle = L"Load PNG profile window file";
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		// kill old profile window
		if (IsWindow(g_hProfileWindow)) SendMessage(g_hProfileWindow, WM_CLOSE, NULL, NULL);

		// create borderless new project window
		g_hProfileWindow = CreateWindowEx(WS_EX_COMPOSITED, // dwExStyle
			L"Aquilinus-Inicio profile class",          // lpClassName
			L"Aquilinus-Inicio profile project",        // lpWindowName
			WS_POPUP,                                   // dwStyle
			(g_nScreenWidth / 8),                         // x
			(g_nScreenHeight / 8),                        // y
			64,                                         // nWidth
			64,                                         // nHeight
			NULL,                                       // hWndParent
			NULL,                                       // hMenu
			g_wc.hInstance,                             // hInstance
			NULL);                                      // lpParam

		// exit if failed
		if (g_hProfileWindow == NULL)
		{
			MessageBox(NULL, L"Aquilinus Window Creation Failed!", L"Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}

		// save file path to config and set config bool to true
		CopyMemory((PVOID)g_pAquilinusConfig->szPictureFilePath, ofn.lpstrFile, (wcslen(ofn.lpstrFile) * sizeof(wchar_t)));
		g_pAquilinusConfig->bProfileWindow = TRUE;

		// create and init drawer... TODO !! choose drawing api
		g_pDirectDrawProfile = new AQU_Drawer(INICIO_DRAWING_API);
#ifdef _WIN64
		if (FAILED(g_pDirectDrawProfile->Init(g_hProfileWindow, L"Inicio_x64.exe", 64, 64, false, g_pAquilinusConfig->dwColorSchemeIndex, true, ofn.lpstrFile, nullptr, 0))) return 0;
#else
		if (FAILED(g_pDirectDrawProfile->Init(g_hProfileWindow, L"Inicio_Win32.exe", 64, 64, false, g_pAquilinusConfig->dwColorSchemeIndex, true, ofn.lpstrFile, nullptr, 0))) return 0;
#endif
		g_pDirectDrawProfile->SetColorScheme(g_pDirectDraw);

		ShowWindow(g_hProfileWindow, SW_SHOWNORMAL);
		UpdateWindow(g_hProfileWindow);

		// set the new window size
		SetWindowPos(g_hProfileWindow, HWND_TOPMOST, 0, 0, g_pDirectDrawProfile->GetWidth(), g_pDirectDrawProfile->GetHeight(), SWP_NOMOVE);

		// the message loop
		MSG Msg;
		while (GetMessage(&Msg, g_hProfileWindow, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return (DWORD)Msg.wParam;
	}
	else return 0;
}

/**
* Profile window thread without load png dialog, creates picture window by configuration.
***/
DWORD WINAPI ProfileWindowMainThread(LPVOID Param)
{
	// kill old profile window
	if (IsWindow(g_hProfileWindow)) SendMessage(g_hProfileWindow, WM_CLOSE, NULL, NULL);

	// create borderless new project window
	g_hProfileWindow = CreateWindowEx(WS_EX_COMPOSITED, // dwExStyle
		L"Aquilinus-Inicio profile class",          // lpClassName
		L"Aquilinus-Inicio profile project",        // lpWindowName
		WS_POPUP,                                   // dwStyle
		(g_nScreenWidth / 8),                         // x
		(g_nScreenHeight / 8),                        // y
		64,                                         // nWidth
		64,                                         // nHeight
		NULL,                                       // hWndParent
		NULL,                                       // hMenu
		g_wc.hInstance,                             // hInstance
		NULL);                                      // lpParam

	// exit if failed
	if (g_hProfileWindow == NULL)
	{
		MessageBox(NULL, L"Aquilinus Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// create and init direct draw
	g_pDirectDrawProfile = new AQU_Drawer(INICIO_DRAWING_API);
#ifdef _WIN64
	if (FAILED(g_pDirectDrawProfile->Init(g_hProfileWindow, L"Inicio_x64.exe", 64, 64, false, g_pAquilinusConfig->dwColorSchemeIndex, true, g_pAquilinusConfig->szPictureFilePath, g_pPictureData, g_dwPictureSize))) return 0;
#else
	if (FAILED(g_pDirectDrawProfile->Init(g_hProfileWindow, L"Inicio_Win32.exe", 64, 64, false, g_pAquilinusConfig->dwColorSchemeIndex, true, g_pAquilinusConfig->szPictureFilePath, g_pPictureData, g_dwPictureSize))) return 0;
#endif
	g_pDirectDrawProfile->SetColorScheme(g_pDirectDraw);

	ShowWindow(g_hProfileWindow, SW_SHOWNORMAL);
	UpdateWindow(g_hProfileWindow);

	// set the new window size
	DWORD unWidth = g_pDirectDrawProfile->GetWidth();
	if (unWidth < 64) unWidth = 64;
	DWORD unHeight = g_pDirectDrawProfile->GetHeight();
	if (unHeight < 64) unHeight = 64;
	SetWindowPos(g_hProfileWindow, HWND_TOPMOST, 0, 0, unWidth, unHeight, SWP_NOMOVE);

	// the message loop
	MSG Msg;
	while (GetMessage(&Msg, g_hProfileWindow, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return (DWORD)Msg.wParam;
}

/**
* Inicio game profile window procedure.
***/
LRESULT CALLBACK WndProcProfileWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			// at start : no control activated + sub menu open
			g_bControlActivated = false;
			break;
		case WM_LBUTTONDOWN:
			// get the mouse cursor
			g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// get the old window position
			RECT rcWnd;
			GetWindowRect(hwnd, &rcWnd);

			// close window ?
			if ((g_ptMouseCursor.x > (rcWnd.right - rcWnd.left - 32)) && (g_ptMouseCursor.y < 32))
			{
				DestroyWindow(hwnd);
				return 0;
			}

			g_bControlActivated = false;
			break;
		case WM_MOUSEMOVE:
			// move the window ?
			if ((wParam&MK_LBUTTON) && (!g_bControlActivated))
			{
				// get new mouse cursor point
				LONG nX = (LONG)GET_X_LPARAM(lParam);
				LONG nY = (LONG)GET_Y_LPARAM(lParam);

				// get the difference to the old position set only in WM_LBUTTONDOWN
				nX -= g_ptMouseCursor.x;
				nY -= g_ptMouseCursor.y;

				// get the old window position
				RECT rcWnd;
				GetWindowRect(hwnd, &rcWnd);

				// set the new window position
				SetWindowPos(hwnd, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
			}
			else
			{
				// get the mouse cursor
				g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
				g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);
			}
			break;
		case WM_CLOSE:
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = NULL;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case WM_PAINT:
			if (!g_pDirectDrawProfile) break;

			// check if surface locked
			if (!g_pDirectDrawProfile->VerifyAPI()) break;

			// clear the back buffer
			g_pDirectDrawProfile->Clear(g_pDirectDrawProfile->GetWidth(), g_pDirectDrawProfile->GetHeight());

			// present
			g_pDirectDrawProfile->Present();
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

/**
* New Project thread.
***/
DWORD WINAPI SubWindowThread(LPVOID Param)
{
	// which sub window ?
	switch (g_eInicioStatus)
	{
		case InicioStatus::NewProjectWindow:
			// create a select process window
			g_eCurrentSubWindow = InicioSubWindows::SelectProcess;
			g_nSubWindowWidth = SELECT_PROCESS_WINDOW_WIDTH;
			g_nSubWindowHeight = SELECT_PROCESS_WINDOW_HEIGHT;
			break;
		case InicioStatus::OptionsWindow:
			// create an options window
			g_eCurrentSubWindow = InicioSubWindows::Options;
			g_nSubWindowWidth = OPTIONS_WINDOW_WIDTH;
			g_nSubWindowHeight = OPTIONS_WINDOW_HEIGHT;
			g_sInicioControls[InicioControls::ColorsList].dwSelection = g_pAquilinusConfig->dwColorSchemeIndex;
			break;
	}

	// create borderless new project window
	g_hSubWindow = CreateWindowEx(WS_EX_COMPOSITED, // dwExStyle
		L"Aquilinus-Inicio new project class",      // lpClassName
		L"Aquilinus-Inicio new project",            // lpWindowName
		WS_POPUP,                                   // dwStyle
		(g_nScreenWidth / 2) + (g_nSubWindowWidth / 8),   // x
		(g_nScreenHeight / 8),                        // y
		g_nSubWindowWidth,                          // nWidth
		g_nSubWindowHeight,                         // nHeight
		NULL,                                       // hWndParent
		NULL,                                       // hMenu
		g_wc.hInstance,                             // hInstance
		NULL);                                      // lpParam

	// exit if failed
	if (g_hSubWindow == NULL)
	{
		MessageBox(NULL, L"Aquilinus Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// create and init direct draw
	g_pDirectDrawSub = new AQU_Drawer(INICIO_DRAWING_API);
#ifdef _WIN64
	if (FAILED(g_pDirectDrawSub->Init(g_hSubWindow, L"Inicio_x64.exe", g_nSubWindowWidth, g_nSubWindowHeight, false, g_pAquilinusConfig->dwColorSchemeIndex, false, nullptr, nullptr, 0))) return 0;
#else
	if (FAILED(g_pDirectDrawSub->Init(g_hSubWindow, L"Inicio_Win32.exe", g_nSubWindowWidth, g_nSubWindowHeight, false, g_pAquilinusConfig->dwColorSchemeIndex, false, nullptr, nullptr, 0))) return 0;
#endif
	g_pDirectDrawSub->SetColorScheme(g_pDirectDraw);

	ShowWindow(g_hSubWindow, SW_SHOWNORMAL);
	UpdateWindow(g_hSubWindow);

	// the message loop
	MSG Msg;
	while (GetMessage(&Msg, g_hSubWindow, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (DWORD)Msg.wParam;
}

/**
* Inicio main window procedure.
***/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			if (g_eInicioStatus == InicioStatus::Initial)
			{
				// init inicio
				InicioInit();

				// start idle
				g_eInicioStatus = InicioStatus::Idle;
				g_bControlActivated = false;
				g_bSubMenuOpen = false;
			}
			break;
		case WM_LBUTTONDOWN:
			// get the mouse cursor
			g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			// go through active controls
			if (InRect(g_sInicioControls[InicioControls::InicioExit].rcButtonRect, g_ptMouseCursor))
				SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			else if (InRect(g_sInicioControls[InicioControls::InicioNewProject].rcButtonRect, g_ptMouseCursor))
			{
				// New Project Button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::InicioNewProject].bIsPressed = true;
			}
			else if (InRect(g_sInicioControls[InicioControls::InicioLoadProject].rcButtonRect, g_ptMouseCursor))
			{
				// Load Project Button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::InicioLoadProject].bIsPressed = true;
			}
			else if (InRect(g_sInicioControls[InicioControls::InicioOptions].rcButtonRect, g_ptMouseCursor))
			{
				// Options Button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::InicioOptions].bIsPressed = true;
			}
			else if (InRect(g_sInicioControls[InicioControls::Info].rcButtonRect, g_ptMouseCursor))
			{
				// Options Button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::Info].bIsPressed = true;
			}
			else if (InRect(g_sInicioControls[InicioControls::AddProfileWindow].rcButtonRect, g_ptMouseCursor) &&
				(!g_pAquilinusConfig->bProfileWindow) && (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::WorkingArea))
			{
				// add profile button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::AddProfileWindow].bIsPressed = true;
			}
			else if (InRect(g_sInicioControls[InicioControls::InicioLoadProfile].rcButtonRect, g_ptMouseCursor))
			{
				// Options Button activated
				g_bControlActivated = true;
				g_sInicioControls[InicioControls::InicioLoadProfile].bIsPressed = true;
			}
			break;
		case WM_LBUTTONUP:
			// new project
			if (g_sInicioControls[InicioControls::InicioNewProject].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::InicioNewProject].bIsPressed = false;
				g_bControlActivated = false;

				// force idle
				ForceIdle();

				// enumerate game names
				EnumerateGameNames();

				// create the thread for the sub window
				if (CreateThread(NULL, 0, SubWindowThread, NULL, 0, NULL) == NULL)
					OutputDebugString(L"Aquilinus : Failed to create new project thread !");
				else
					g_eInicioStatus = InicioStatus::NewProjectWindow;
			}
			// load profile
			if (g_sInicioControls[InicioControls::InicioLoadProfile].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::InicioLoadProfile].bIsPressed = false;
				g_bControlActivated = false;

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

					// thumbnail present ?
					wchar_t szPNGPath[MAX_PATH];
					size_t nLen = wcslen(g_pAquilinusConfig->szProfileFilePath);
					CopyMemory(szPNGPath, g_pAquilinusConfig->szProfileFilePath, nLen*sizeof(wchar_t));
					szPNGPath[nLen - 4] = 'p';
					szPNGPath[nLen - 3] = 'n';
					szPNGPath[nLen - 2] = 'g';
					szPNGPath[nLen - 1] = 0;
					wchar_t buf[128];
					wsprintf(buf, L"LEN %u", nLen);
					OutputDebugString(buf);
					OutputDebugString(g_pAquilinusConfig->szProfileFilePath);
					OutputDebugString(szPNGPath);
					g_pDirectDraw->LoadBackground(szPNGPath, 256, 64);

				}
				else
				{
					OutputDebugString(L"Aquilinus : Load profile failed!");
					g_eInicioStatus = InicioStatus::Idle;
				}

			}
			// load project
			if (g_sInicioControls[InicioControls::InicioLoadProject].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::InicioLoadProject].bIsPressed = false;
				g_bControlActivated = false;

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

					// create the thread
					g_eInicioStatus = InicioStatus::Injecting;
					g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingAreaLoad;
					g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
					g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
					if (g_hInjectionThread == NULL)
						OutputDebugString(L"Aquilinus : Failed to create injection thread !");

					// picture present ?
					if (g_pAquilinusConfig->bProfileWindow)
					{
						// create the thread for the profile window
						if (CreateThread(NULL, 0, ProfileWindowMainThread, NULL, 0, NULL) == NULL)
							OutputDebugString(L"Aquilinus : Failed to create profile window thread !");
					}
				}
				else
				{
					OutputDebugString(L"Aquilinus : Load working area failed!");
					g_eInicioStatus = InicioStatus::Idle;
				}
			}
			// options
			if (g_sInicioControls[InicioControls::InicioOptions].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::InicioOptions].bIsPressed = false;
				g_bControlActivated = false;

				// force idle
				ForceIdle();

				// create the thread to override the virtual methods table
				if (CreateThread(NULL, 0, SubWindowThread, NULL, 0, NULL) == NULL)
					OutputDebugString(L"Aquilinus : Failed to create new project thread !");
				else
					g_eInicioStatus = InicioStatus::OptionsWindow;
			}
			// info ... 
			if (g_sInicioControls[InicioControls::Info].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::Info].bIsPressed = false;
				g_bControlActivated = false;

				// output version
				wchar_t buf[2048];
				wsprintf(buf,
					L"Aquilinus - 3D Modification Studio\r\n"
					L"(c) 2014-2015 by Denis Reischl\r\n"
					L"Version : %d.%d.%d"
					L"\r\n"
					L"\r\n"
					L"The Aquilinus source code is intended to be used, changed,\r\n"
					L"compiled and its build published only by Denis Reischl.\r\n"
					L"\r\n"
					L"The Aquilinus source code is intended to be used by its author,\r\n"
					L"Denis Reischl, for any commercial and non-commercial purpose.\r\n"
					L"\r\n"
					L"Except where otherwise noted, this work is licensed under :\r\n"
					L"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\r\n"
					L"\r\n"
					L"The colourLOVERS palettes and the resulting artwork used in\r\n"
					L"this software are licensed under:\r\n"
					L"<http://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US>.\r\n"
					L"\r\n"
					L"The Aquilinus PNG/Deflate Decoder partially uses altered code\r\n"
					L"from following sources, with permission by licence :\r\n"
					L"picoPNG version 20101224\r\n"
					L"(c) 2005-2010 Lode Vandevenne\r\n"
					L"Deflate Decompressor\r\n"
					L"(c) 2011 Stephan Brumme\r\n"
					L"\r\n"
					L"Font 'TRAJANUS' used in this software with permission by\r\n"
					L"licence : \r\n"
					L"(c) Roger White - all rights reserved\r\n"
					L"\r\n"
					L"(read the software documentation for further information)\r\n"
					, g_eVersion.dwPrima, g_eVersion.dwSecondo, g_eVersion.dwTerzo);

				// create message box
				MessageBox(g_hwnd, buf, L"Info", MB_OK);
			}
			// profile window
			if (g_sInicioControls[InicioControls::AddProfileWindow].bIsPressed)
			{
				// first, set controls to false
				g_sInicioControls[InicioControls::AddProfileWindow].bIsPressed = false;
				g_bControlActivated = false;

				// create the thread to override the virtual methods table
				if (CreateThread(NULL, 0, ProfileWindowThread, NULL, 0, NULL) == NULL)
					OutputDebugString(L"Aquilinus : Failed to create profile window thread !");
			}
			// ensure that no control is activated whenever left mouse button is up
			g_bControlActivated = false;
			break;
		case WM_MOUSEMOVE:
			// take a look if project is closed...
			if (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::Closed)
			{
				OutputDebugString(L"Inicio : Project closed...");

				// force idle
				ForceIdle();

				// set project to >noProject<
				g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::NoProject;
			}

			// move the window ?
			if ((wParam&MK_LBUTTON) && (!g_bControlActivated))
			{
				// get new mouse cursor point
				LONG nX = (LONG)GET_X_LPARAM(lParam);
				LONG nY = (LONG)GET_Y_LPARAM(lParam);

				// get the difference to the old position set only in WM_LBUTTONDOWN
				nX -= g_ptMouseCursor.x;
				nY -= g_ptMouseCursor.y;

				// get the old window position
				RECT rcWnd;
				GetWindowRect(hwnd, &rcWnd);

				// set the new window position
				SetWindowPos(hwnd, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
			}
			else
			{
				// get the mouse cursor
				g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
				g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);
			}
			// start injection thread if >toInject<
			if (g_eInicioStatus == InicioStatus::ToInject)
			{
				// create the thread to override the virtual methods table
				g_eInicioStatus = InicioStatus::Injecting;
				g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingAreaNew;
				g_nRepeat = (int)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
				g_hInjectionThread = CreateThread(NULL, 0, InjectionThread, &g_nRepeat, 0, NULL);
				if (g_hInjectionThread == NULL)
					OutputDebugString(L"Aquilinus : Failed to create injection thread !");
			}
			break;
		case WM_CLOSE:
			InicioClose();
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = NULL;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:

			// check if external file is to be saved (we do that here since WM_PAINT messages are quite often)
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

			// direct draw object present ?
			if (!g_pDirectDraw) break;

			// check if surface locked
			if (!g_pDirectDraw->VerifyAPI()) break;

			// clear the back buffer
			g_pDirectDraw->Clear(g_nMainWindowWidth, g_nMainWindowHeight);

			// render static text
			g_pDirectDraw->RenderText(L"Aquilinus", 24, 12, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, INICIO_FULL_TEXT_SIZE, 1.0f);
			g_pDirectDraw->RenderText(L"by Eisern Schild", 80, 120, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_MEDIUM_TEXT_SIZE, 1.0f);
			g_pDirectDraw->RenderText(L"x", g_sInicioControls[InicioControls::InicioExit].rcButtonRect.left, 0, g_vcZeroOrigin, AquilinusColor::Color4, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);

#ifdef DEVELOPER_BUILD
			g_pDirectDraw->RenderText(L"DEVBUILD", 600, 180, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, INICIO_TINY_TEXT_SIZE, 1.0f);
#else
			// output version
			wchar_t buf[64];
			wsprintf(buf, L"v%d.%d.%d", g_eVersion.dwPrima, g_eVersion.dwSecondo, g_eVersion.dwTerzo);
			g_pDirectDraw->RenderText(buf, 600, 180, g_vcZeroOrigin, AquilinusColor::Color2, AquilinusColor::Color0, INICIO_TINY_TEXT_SIZE, 1.0f);
#endif

			// status text
			switch (g_eInicioStatus)
			{
				case InicioStatus::Idle:
					g_pDirectDraw->RenderText(L"Idle", 12, 170, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
					break;
				case InicioStatus::Injecting:
					g_pDirectDraw->RenderText(L"Injecting..", 12, 170, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
					break;
				case InicioStatus::ToInject:
					g_pDirectDraw->RenderText(L"Start to inject", 12, 170, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
					break;
				case InicioStatus::NewProjectWindow:
					g_pDirectDraw->RenderText(L"New Project", 12, 170, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
					break;
				case InicioStatus::OptionsWindow:
					g_pDirectDraw->RenderText(L"Options", 12, 170, g_vcZeroOrigin, AquilinusColor::Color3, AquilinusColor::Color0, INICIO_SMALL_TEXT_SIZE, 1.0f);
					break;
			}

			// draw controls
			RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::InicioNewProject], INICIO_SMALLER_TEXT_SIZE);
			RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::InicioLoadProject], INICIO_SMALLER_TEXT_SIZE);
			RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::InicioOptions], INICIO_SMALLER_TEXT_SIZE);
			RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::Info], INICIO_SMALLER_TEXT_SIZE);
			if ((!g_pAquilinusConfig->bProfileWindow) && (g_pAquilinusConfig->eProjectStage == AQU_ProjectStage::WorkingArea))
				RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::AddProfileWindow], INICIO_SMALLER_TEXT_SIZE);

			// draw the game profile box
			RenderButton(g_pDirectDraw, g_sInicioControls[InicioControls::InicioLoadProfile], INICIO_SMALLER_TEXT_SIZE);

			// draw the additional bitmap, if present
			g_pDirectDraw->DrawBackground((DWORD)g_sInicioControls[InicioControls::InicioLoadProfile].rcButtonRect.left + 3,
				(DWORD)g_sInicioControls[InicioControls::InicioLoadProfile].rcButtonRect.top + 3);

			// present
			g_pDirectDraw->Present();

			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/**
* Inicio new project window procedure.
***/
LRESULT CALLBACK WndProcSubWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			// at start : no control activated + sub menu open
			g_bControlActivated = false;
			g_bSubMenuOpen = false;
			break;
		case WM_LBUTTONDOWN:
			// get the mouse cursor
			g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
			g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

			switch (g_eCurrentSubWindow)
			{
				case InicioSubWindows::SelectProcess:
					HandleSelectProcessButtonDown(hwnd);
					break;
				case InicioSubWindows::NewProject:
					HandleNewProjectButtonDown(hwnd);
					break;
				case InicioSubWindows::Options:
					HandleOptionsButtonDown(hwnd);
					break;
#ifdef DEVELOPER_BUILD
				case InicioSubWindows::AddProcess:
					HandleAddProcessButtonDown(hwnd);
					break;
#endif
			}

			break;
		case WM_LBUTTONUP:
			switch (g_eCurrentSubWindow)
			{
				case InicioSubWindows::SelectProcess:
					HandleSelectProcessButtonUp(hwnd);
					break;
				case InicioSubWindows::NewProject:
					HandleNewProjectButtonUp(hwnd);
					break;
				case InicioSubWindows::Options:
					HandleOptionsButtonUp(hwnd);
					break;
#ifdef DEVELOPER_BUILD
				case InicioSubWindows::AddProcess:
					HandleAddProcessButtonUp(hwnd);
					break;
#endif
			}
			g_bControlActivated = false;
			break;
		case WM_MOUSEMOVE:
			// move the window ?
			if ((wParam&MK_LBUTTON) && (!g_bControlActivated))
			{
				// get new mouse cursor point
				LONG nX = (LONG)GET_X_LPARAM(lParam);
				LONG nY = (LONG)GET_Y_LPARAM(lParam);

				// get the difference to the old position set only in WM_LBUTTONDOWN
				nX -= g_ptMouseCursor.x;
				nY -= g_ptMouseCursor.y;

				// get the old window position
				RECT rcWnd;
				GetWindowRect(hwnd, &rcWnd);

				// set the new window position
				SetWindowPos(hwnd, HWND_TOPMOST, rcWnd.left + nX, rcWnd.top + nY, 0, 0, SWP_NOSIZE);
			}
			else
			{
				// get the mouse cursor
				g_ptMouseCursor.x = (LONG)GET_X_LPARAM(lParam);
				g_ptMouseCursor.y = (LONG)GET_Y_LPARAM(lParam);

				// any slider in sub window ? if we get more controls create a >HandleMouseMove< method...
				if ((g_sInicioControls[InicioControls::TimeDelaySlider].bIsPressed) &&
					(InRect(g_sInicioControls[InicioControls::TimeDelaySlider].rcButtonRect, g_ptMouseCursor)) &&
					(g_eCurrentSubWindow == InicioSubWindows::SelectProcess))
				{
					// set new value
					SetSliderValue(g_sInicioControls[InicioControls::TimeDelaySlider], g_ptMouseCursor);
					if (g_sInicioControls[InicioControls::TimeDelaySlider].nValue >= 0)
					{
						// the low 4 bits (0..15) value param is the injection repetition
						g_pAquilinusConfig->dwDetourTimeDelay = (DWORD)g_sInicioControls[InicioControls::TimeDelaySlider].nValue;
						g_pAquilinusConfig->dwDetourTimeDelay -= (DWORD)(g_sInicioControls[InicioControls::TimeDelaySlider].nValue & 15);
					}
				}
				else if ((g_sInicioControls[InicioControls::InjectionRepetition].bIsPressed) &&
					(InRect(g_sInicioControls[InicioControls::InjectionRepetition].rcButtonRect, g_ptMouseCursor)) &&
					(g_eCurrentSubWindow == InicioSubWindows::SelectProcess))
				{
					// OK
					g_bControlActivated = true;
					g_sInicioControls[InicioControls::InjectionRepetition].bIsPressed = true;

					// set new value
					SetSliderValue(g_sInicioControls[InicioControls::InjectionRepetition], g_ptMouseCursor);
					if (g_sInicioControls[InicioControls::InjectionRepetition].nValue >= 0)
					{
						// the low 4 bits (0..15) value param is the injection repetition
						g_pAquilinusConfig->dwDetourTimeDelay -= (DWORD)(g_pAquilinusConfig->dwDetourTimeDelay & 15);
						g_pAquilinusConfig->dwDetourTimeDelay = (DWORD)(g_sInicioControls[InicioControls::InjectionRepetition].nValue & 15);
					}
				}
			}
			break;
		case WM_CLOSE:
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = NULL;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case WM_PAINT:
			if (!g_pDirectDrawSub) break;

			// check if surface locked
			if (!g_pDirectDrawSub->VerifyAPI()) break;

			// clear the back buffer, draw a tiny border
			g_pDirectDrawSub->Clear(g_nSubWindowWidth, g_nSubWindowHeight, AquilinusColor::Color1);
			RECT rcBorder;
			SetRect(&rcBorder, 2, 2, g_nSubWindowWidth - 2, g_nSubWindowHeight - 2);
			g_pDirectDrawSub->DrawBox(rcBorder, g_vcZeroOrigin, 1, AquilinusColor::Color0, 1.0f);


			switch (g_eCurrentSubWindow)
			{
				case InicioSubWindows::SelectProcess:
					RenderSelectProcess();
					break;
				case InicioSubWindows::NewProject:
					RenderNewProject();
					break;
				case InicioSubWindows::Options:
					RenderOptions();
					break;
#ifdef DEVELOPER_BUILD
				case InicioSubWindows::AddProcess:
					RenderAddProcess();
					break;
#endif
			}

			// present
			g_pDirectDrawSub->Present();

			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/**
* Inicio main windows entry point.
***/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	OutputDebugString(L"Inicio : Started...");

	MSG Msg;

	// first, set the instance handle
	g_hInstance = hInstance;

	// set the version
	g_eVersion.dwPrima = AQUILINUS_VERSION_PRIMA;
	g_eVersion.dwSecondo = AQUILINUS_VERSION_SECONDO;
	g_eVersion.dwTerzo = AQUILINUS_VERSION_TERZO;

	// set initial status, init controls
	g_eInicioStatus = InicioStatus::Initial;
	g_eCurrentSubWindow = InicioSubWindows::NoWindow;
	InitControls();

	// register sub window classes
	if (FAILED(RegisterNewProjectWindowClass(hInstance)))
		return E_FAIL;

	// register the profile window class
	WNDCLASSEX wc;

	// register the new project window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProcProfileWindow;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
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
		return E_FAIL;
	}

	// register the global window class
	g_wc.cbSize = sizeof(WNDCLASSEX);
	g_wc.style = 0;
	g_wc.lpfnWndProc = WndProc;
	g_wc.cbClsExtra = 0;
	g_wc.cbWndExtra = 0;
	g_wc.hInstance = hInstance;
	g_wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	g_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	g_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	g_wc.lpszMenuName = NULL;
	g_wc.lpszClassName = L"Aquilinus-Inicio class";
	g_wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// exit if failed
	if (!RegisterClassEx(&g_wc))
	{
		MessageBox(NULL, L"Aquilinus Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// create borderless main window
	g_hwnd = CreateWindowEx(WS_EX_COMPOSITED,      // dwExStyle
		L"Aquilinus-Inicio class",                 // lpClassName
		L"Aquilinus-Inicio",                       // lpWindowName
		WS_POPUP | WS_BORDER,                      // dwStyle
		(g_nScreenWidth / 2) - (g_nMainWindowWidth),   // x
		(g_nScreenHeight / 8),                       // y
		g_nMainWindowWidth,                        // nWidth
		g_nMainWindowHeight,                       // nHeight
		NULL,                                      // hWndParent
		NULL,                                      // hMenu
		hInstance,                                 // hInstance
		NULL);                                     // lpParam

	// exit if failed
	if (g_hwnd == NULL)
	{
		MessageBox(NULL, L"Aquilinus window creation failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

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
	DEBUG_UINT(g_pAquilinusConfig->bCreateD3D9Ex);
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
	g_pAquilinusConfig->dwColorSchemeIndex = g_dwColorSchemeIndex;

	// always set the detour time delay to zero at startup !
	g_pAquilinusConfig->dwDetourTimeDelay = 0;

	// create and init direct draw 
	g_pDirectDraw = new AQU_Drawer(INICIO_DRAWING_API);
#ifdef _WIN64
	if (FAILED(g_pDirectDraw->Init(g_hwnd, L"Inicio_x64.exe", g_nMainWindowWidth, g_nMainWindowHeight, true, g_dwColorSchemeIndex, false, nullptr, nullptr, 0))) return 0;
#else
	if (FAILED(g_pDirectDraw->Init(g_hwnd, L"Inicio_Win32.exe", g_nMainWindowWidth, g_nMainWindowHeight, true, g_dwColorSchemeIndex, false, nullptr, nullptr, 0))) return 0;
#endif
	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	// the message loop
	while (GetMessage(&Msg, g_hwnd, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	if (g_pDirectDraw) delete g_pDirectDraw;
	if (g_pDirectDrawSub) delete g_pDirectDrawSub;
	if (g_pDirectDrawProfile) delete g_pDirectDrawProfile;

	return (DWORD)Msg.wParam;
}

#endif

#pragma endregion