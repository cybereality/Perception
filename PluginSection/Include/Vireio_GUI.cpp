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
Vireio_GUI::Vireio_GUI(SIZE sSize, LPCWSTR szFont, BOOL bItalic, DWORD dwFontSize, COLORREF dwColorFront, COLORREF dwColorBack) :
m_bMouseBoundToControl(false),
m_dwCurrentPage(0)
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

			// verify page index
			if (m_dwCurrentPage >= (UINT)m_asPages.size()) m_dwCurrentPage = (UINT)m_asPages.size() - 1;

			// loop through controls for this page, draw them
			if (m_asPages.size())
				for (UINT dwI = 0; dwI < (UINT)m_asPages[m_dwCurrentPage].m_asControls.size(); dwI++)
				{
					// render control depending on type
					switch (m_asPages[m_dwCurrentPage].m_asControls[dwI].m_eControlType)
					{
						case StaticListBox:
						{
							POINT* psPos = &m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sPosition;
							SIZE* psSize = &m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sSize;
							int nY = (int)psPos->y;

							// loop through entries
							if (m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sStaticListBox.m_paszEntries)
								for (UINT dwJ = 0; dwJ < (UINT)m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sStaticListBox.m_paszEntries->size(); dwJ++)
								{
									// output the list entry text
									TextOut(hdcImage,
										psPos->x,
										nY,
										((*(m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sStaticListBox.m_paszEntries))[dwJ]).c_str(),
										((*(m_asPages[m_dwCurrentPage].m_asControls[dwI].m_sStaticListBox.m_paszEntries))[dwJ]).length());

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

		// draw the arrows... first, the arrow background
		RECT sRect;
		sRect.top = m_sGUISize.cy - m_dwFontSize * 4;
		sRect.bottom = m_sGUISize.cy;
		sRect.left = 0;
		sRect.right = m_sGUISize.cx;
		FillRect(hdcImage, &sRect, (HBRUSH)(COLOR_SCROLLBAR + 1));

		// left arrow.. arrows are adjusted by the font size and GUI size
		POINT asPoints[3];
		asPoints[0].x = m_sGUISize.cx >> 4;
		asPoints[0].y = sRect.top + m_dwFontSize * 2;
		asPoints[1].x = (m_sGUISize.cx >> 1) - (m_sGUISize.cx >> 3);
		asPoints[1].y = sRect.top + (m_dwFontSize >> 2);
		asPoints[2].x = asPoints[1].x;
		asPoints[2].y = sRect.bottom - (m_dwFontSize >> 2);
		SelectObject(hdcImage, GetStockObject(DC_PEN));
		SelectObject(hdcImage, GetStockObject(DC_BRUSH));
		if ((m_bMouseBoundToControl) && (sMouseCoords.y > (LONG)(m_sGUISize.cy - m_dwFontSize * 4)) && (sMouseCoords.x < (LONG)(m_sGUISize.cx >> 1)))
		{
			SetDCPenColor(hdcImage, m_dwColorFront ^ m_dwColorFront);
			SetDCBrushColor(hdcImage, m_dwColorFront ^ m_dwColorFront);
		}
		else
		{
			SetDCPenColor(hdcImage, m_dwColorFront);
			SetDCBrushColor(hdcImage, m_dwColorFront);
		}
		if (m_dwCurrentPage > 0)
			Polygon(hdcImage, asPoints, 3);

		// right arrow
		asPoints[0].x = m_sGUISize.cx - (m_sGUISize.cx >> 4);
		asPoints[0].y = sRect.top + m_dwFontSize * 2;
		asPoints[1].x = (m_sGUISize.cx >> 1) + (m_sGUISize.cx >> 3);
		asPoints[1].y = sRect.top + (m_dwFontSize >> 2);
		asPoints[2].x = asPoints[1].x;
		asPoints[2].y = sRect.bottom - (m_dwFontSize >> 2);
		if ((m_bMouseBoundToControl) && (sMouseCoords.y > (LONG)(m_sGUISize.cy - m_dwFontSize * 4)) && (sMouseCoords.x >= (LONG)(m_sGUISize.cx >> 1)))
		{
			SetDCPenColor(hdcImage, m_dwColorFront ^ m_dwColorFront);
			SetDCBrushColor(hdcImage, m_dwColorFront ^ m_dwColorFront);
		}
		else
		{
			SetDCPenColor(hdcImage, m_dwColorFront);
			SetDCBrushColor(hdcImage, m_dwColorFront);
		}
		if ((m_asPages.size() > 1) && (m_dwCurrentPage < (m_asPages.size() - 1)))
			Polygon(hdcImage, asPoints, 3);

		// line between
		sRect.left = (m_sGUISize.cx >> 1) - (m_sGUISize.cx >> 7);
		sRect.right = (m_sGUISize.cx >> 1) + (m_sGUISize.cx >> 7);
		FillRect(hdcImage, &sRect, (HBRUSH)(COLOR_BACKGROUND + 1));

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

/**
* Windows event for the GUI.
***/
Vireio_GUI_Event Vireio_GUI::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// create empty return value
	Vireio_GUI_Event sRet;
	ZeroMemory(&sRet, sizeof(Vireio_GUI_Event));
	sRet.eType = Vireio_GUI_Event_Type::NoEvent;

	// get local mouse cursor
	sMouseCoords.x = GET_X_LPARAM(lParam) * 4;
	sMouseCoords.y = GET_Y_LPARAM(lParam) * 4;

	// update control
	m_bControlUpdate = true;

	switch (msg)
	{
		// left mouse button down ?
		case WM_LBUTTONDOWN:
			// mouse currently bound to any control ?
			if (!m_bMouseBoundToControl)
			{
				// next/previous page ?
				if (sMouseCoords.y > (LONG)(m_sGUISize.cy - m_dwFontSize * 4))
				{
					// left/right ?
					if (sMouseCoords.x < (LONG)(m_sGUISize.cx >> 1))
					{
						if (m_dwCurrentPage > 0) m_dwCurrentPage--;
					}
					else
					{
						if (m_dwCurrentPage < (UINT)(m_asPages.size() - 1)) m_dwCurrentPage++;
					}
					m_bMouseBoundToControl = true;
				}
				// loop through active controls for this page
				else
					for (UINT dwI = 0; dwI < (UINT)m_asPages[m_dwCurrentPage].m_asControls.size(); dwI++)
					{

					}
			}
			break;
			// left mouse button up ?
		case WM_LBUTTONUP:
			m_bMouseBoundToControl = false;
			break;
			// mouse move ?
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON) m_bMouseBoundToControl = false;
			break;
		case WM_RBUTTONDOWN:
			break;
	}

	return sRet;
}