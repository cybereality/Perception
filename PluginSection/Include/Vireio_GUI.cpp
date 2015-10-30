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
Vireio_GUI::Vireio_GUI(SIZE sSize, LPCWSTR szFont, BOOL bItalic, DWORD dwFontSize, COLORREF dwColorFront, COLORREF dwColorBack)
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
	m_hFont = CreateFont(dwFontSize, 0, 0, 0, 0, bItalic,
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
HBITMAP Vireio_GUI::GetGUI()
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

			// loop through controls for this page, draw them
			if (m_asPages.size())
				for (UINT dwI = 0; dwI < (UINT)m_asPages[0/*TODO:ADD_INDEX*/].m_asControls.size(); dwI++)
				{
					// render control depending on type
					switch (m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_eControlType)
					{
						case StaticListBox:
						{
							POINT* psPos = &m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sPosition;
							SIZE* psSize = &m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sSize;
							int nY = (int)psPos->y;

							// loop through entries
							if (m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sStaticListBox.m_paszEntries)
								for (UINT dwJ = 0; dwJ < (UINT)m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sStaticListBox.m_paszEntries->size(); dwJ++)
								{
									// output the list entry text
									TextOut(hdcImage,
										psPos->x,
										nY,
										((*(m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sStaticListBox.m_paszEntries))[dwJ]).c_str(),
										((*(m_asPages[0/*TODO:ADD_INDEX*/].m_asControls[dwI].m_sStaticListBox.m_paszEntries))[dwJ]).length());

									// next line
									nY += (int)m_dwFontSize;
								}
						}
						break;
						case ListBox:
							break;
						case SpinControl:
							break;
						case EditLine:
							break;
						case Slider:
							break;
						case CheckBox:
							break;
						default:
							break;
					}
				}

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

/**
* Adds a Vireio Control to the specified page.
* @param dwPage The index of the page the control will be added. (=id)
***/
UINT Vireio_GUI::AddControl(UINT dwPage, Vireio_Control& sControl)
{
	// page present ?
	if (dwPage < (UINT)m_asPages.size())
	{
		// add control
		if (m_asPages[dwPage].m_asControls.size() < MAX_CONTROLS_PER_PAGE)
			m_asPages[dwPage].m_asControls.push_back(sControl);

		// create id and return
		return (UINT)(dwPage << 16) + m_asPages[dwPage].m_asControls.size() - 1;
	}
	else return 0;
}

/**
* Adds an entry to the specified control.
* @param dwControl The id of the control.
***/
void Vireio_GUI::AddEntry(UINT dwControl, LPCWSTR szString)
{
	// decode id to page and index
	UINT dwPage = dwControl >> 16;
	UINT dwIndex = dwControl & 65535;

	// page present ?
	if (dwPage < (UINT)m_asPages.size())
	{
		// control present ?
		if (dwIndex < (UINT)m_asPages[dwPage].m_asControls.size())
		{
			// add the string depending on the control type
			std::wstring sz = std::wstring(szString);
			switch (m_asPages[dwPage].m_asControls[dwIndex].m_eControlType)
			{
				case StaticListBox:
					if (m_asPages[dwPage].m_asControls[dwIndex].m_sStaticListBox.m_paszEntries)
						m_asPages[dwPage].m_asControls[dwIndex].m_sStaticListBox.m_paszEntries->push_back(sz);
					else OutputDebugString(L"Faulty code: Entry vector nullptr !");
					break;
				case ListBox:
					if (m_asPages[dwPage].m_asControls[dwIndex].m_sListBox.m_paszEntries)
						m_asPages[dwPage].m_asControls[dwIndex].m_sListBox.m_paszEntries->push_back(sz);
					else OutputDebugString(L"Faulty code: Entry vector nullptr !");
					break;
				case SpinControl:
					break;
				case EditLine:
					break;
				case Slider:
					break;
				case CheckBox:
					break;
				default:
					break;
			}

		}
	}
}