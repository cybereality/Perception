/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_GUI.cpp>
and Class <Vireio_GUI>:
Copyright (C) 2015 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include "Vireio_GUI.h"

/**
* Constructor.
***/
Vireio_GUI::Vireio_GUI(SIZE sSize, LPCWSTR szFont, DWORD dwFontSize, COLORREF dwColorFront, COLORREF dwColorBack)
{
	// set the size, colors, font size and the font name
	CopyMemory(&m_sGUISize, &sSize, sizeof(SIZE));
	m_dwColorFront = dwColorFront;
	m_dwColorBack = dwColorBack;
	m_dwFontSize = dwFontSize;
	m_szFontName = std::wstring(szFont);

	// create bitmap, set control update to true
	HWND hwnd = GetActiveWindow();
	HDC hdc = GetDC(hwnd);
	m_hBitmapControl = CreateCompatibleBitmap(hdc, (int)sSize.cx, (int)sSize.cy);
	if (!m_hBitmapControl)
		OutputDebugString(L"Failed to create bitmap!");
	m_bControlUpdate = true;

	// create the font
	m_hFont = CreateFont(dwFontSize, 0, 0, 0, 0, FALSE,
		FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		m_szFontName.c_str());
}

/**
* Destructor.
***/
Vireio_GUI::~Vireio_GUI() {}

/**
* Provides the rendered bitmap for the current frame.
* @returns The current GUI bitmap, nullptr if no changes.
***/
HBITMAP Vireio_GUI::GetControl()
{
	if (!m_hBitmapControl)
	{
		// create bitmap, set control update to true
		HWND hwnd = GetActiveWindow();
		HDC hdc = GetDC(hwnd);
		m_hBitmapControl = CreateCompatibleBitmap(hdc, (int)m_sGUISize.cx, (int)m_sGUISize.cy);
		if (!m_hBitmapControl)
			OutputDebugString(L"Failed to create bitmap!");
		m_bControlUpdate = true;
	}

	if (m_bControlUpdate)
	{
		// get control bitmap dc
		HDC hdcImage = CreateCompatibleDC(NULL);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, m_hBitmapControl);
		HFONT hOldFont;

		// clear the background
		RECT rc;
		SetRect(&rc, 0, 0, (int)m_sGUISize.cx, (int)m_sGUISize.cy);
		FillRect(hdcImage, &rc, (HBRUSH)CreateSolidBrush(m_dwColorBack));

		// create font
		if (!m_hFont)
			m_hFont = CreateFont(m_dwFontSize, 0, 0, 0, 0, FALSE,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			m_szFontName.c_str());

		// Select the variable stock font into the specified device context. 
		if (hOldFont = (HFONT)SelectObject(hdcImage, m_hFont))
		{
			SetTextColor(hdcImage, m_dwColorFront);
			SetBkColor(hdcImage, m_dwColorBack);

			// TODO !! loop through controls for this page, draw them

			// Restore the original font.        
			SelectObject(hdcImage, hOldFont);
		}

		SelectObject(hdcImage, hbmOld);
		DeleteDC(hdcImage);

		// next update only by request, return updated bitmap
		m_bControlUpdate = false;
		return m_hBitmapControl;
	}

	return nullptr;
}