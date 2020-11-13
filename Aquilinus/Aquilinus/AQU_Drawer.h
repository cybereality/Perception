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
#ifndef AQU_DRAWER_CLASS
#define AQU_DRAWER_CLASS

#include"AQU_DrawingAPI.h"

/**
* All possible drawing APIs enumeration.
****/
enum AQU_DrawingAPIs
{
	API_DirectDraw,
	API_OpenGL,
};

/**
* Aquilinus drawing class.
* Inherits all possible drawing APIs.
***/
class AQU_Drawer
{
public:
	/**
	* Constructor - choose drawing API.
	***/
	AQU_Drawer(AQU_DrawingAPIs eDrawingAPI)
	{
		switch (eDrawingAPI)
		{
			case AQU_DrawingAPIs::API_DirectDraw: m_pcDrawingAPI = new AQU_DirectDraw(); break;
			case AQU_DrawingAPIs::API_OpenGL: m_pcDrawingAPI = new AQU_OpenGL(); break;
		}
	}
	~AQU_Drawer(){ delete m_pcDrawingAPI; }

	/*** AQU_Drawer Basic methods ***/
	HRESULT          Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize)
	{
		return m_pcDrawingAPI->Init(hwnd, szModule, dwWindowWidth, dwWindowHeight, bSetColorScheme, dwColorSchemeIndex, bBackground, szBackgroundPNG, pPicture, dwPictureSize);
	}
	void             Release()
	{
		m_pcDrawingAPI->Release();
	}
	HRESULT          LoadBackground(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY)
	{
		return m_pcDrawingAPI->LoadBackground(szPNG, dwForceX, dwForceY);
	}
	void             ReleaseBackground()
	{
		m_pcDrawingAPI->ReleaseBackground();
	}
	void             Resize(DWORD dwWidth, DWORD dwHeight)
	{
		m_pcDrawingAPI->Resize(dwWidth, dwHeight);
	}
	void             SetTextRange(DWORD dwMax)
	{
		m_pcDrawingAPI->SetTextRange(dwMax);
	}
	DWORD            GetTextWidth(LPCWSTR szText, float fSize, float fZoom)
	{
		return m_pcDrawingAPI->GetTextWidth(szText, fSize, fZoom);
	}
	DWORD            GetWidth() { return m_pcDrawingAPI->GetWidth(); }
	DWORD            GetHeight() { return m_pcDrawingAPI->GetHeight(); }
	bool             VerifyAPI()
	{
		return m_pcDrawingAPI->VerifyAPI();
	}

	/*** AQU_Drawer Color methods ***/
	DWORD            GetColorSchemesNumber()
	{
		return m_pcDrawingAPI->GetColorSchemesNumber();
	}
	void             SetColorScheme(DWORD dwIndex)
	{
		m_pcDrawingAPI->SetColorScheme(dwIndex);
	}
	void             SetColorScheme(AQU_Drawer *pcDrawer)
	{
		m_pcDrawingAPI->SetColorScheme(pcDrawer->m_pcDrawingAPI);
	}
	void             SetColorScheme(ColorScheme &sColorScheme)
	{
		m_pcDrawingAPI->SetColorScheme(sColorScheme);
	}
	void             GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex)
	{
		m_pcDrawingAPI->GetColorScheme(sColorScheme, dwIndex);
	}

	/*** AQU_Drawer Drawing methods ***/
	void             Clear()
	{
		m_pcDrawingAPI->Clear();
	}
	void             Clear(DWORD dwColor)
	{
		m_pcDrawingAPI->Clear(dwColor);
	}
	void             Clear(AquilinusColor eColor)
	{
		m_pcDrawingAPI->Clear(eColor);
	}
	void             Clear(RECT rcDest, DWORD dwColor)
	{
		m_pcDrawingAPI->Clear(rcDest, dwColor);
	}
	void             Clear(RECT rcDest, AquilinusColor eColor)
	{
		m_pcDrawingAPI->Clear(rcDest, eColor);
	}
	void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight)
	{
		m_pcDrawingAPI->Clear(dwWindowWidth, dwWindowHeight);
	}
	void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor)
	{
		m_pcDrawingAPI->Clear(dwWindowWidth, dwWindowHeight, eColor);
	}
	void             DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
	{
		m_pcDrawingAPI->DrawLine(vecStart, vecEnd, vecOrigin, nSize, eColor, fZoom);
	}
	void             DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
	{
		m_pcDrawingAPI->DrawHLine(vecStart, vecOrigin, nLength, nSize, eColor, fZoom);
	}
	void             DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
	{
		m_pcDrawingAPI->DrawVLine(vecStart, vecOrigin, nLength, nSize, eColor, fZoom);
	}
	void             DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
	{
		m_pcDrawingAPI->DrawPipeline(vecStart, vecEnd, vecOrigin, nSize, eColor, fZoom);
	}
	void             DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
	{
		m_pcDrawingAPI->DrawBox(rcBox, vecOrigin, nSize, eColor, fZoom);
	}
	void             DrawBitmap(UINT dwID, RECT rcDest, POINT vecOrigin, float fZoom)
	{
		m_pcDrawingAPI->DrawBitmap(dwID, rcDest, vecOrigin, fZoom);
	}
	void             DrawBackground(DWORD dwX, DWORD dwY)
	{
		m_pcDrawingAPI->DrawBackground(dwX, dwY);
	}
	void             RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom)
	{
		m_pcDrawingAPI->RenderText(szText, nX, nY, vecOrigin, eColor, eBackground, fSize, fZoom);
	}
	void             Present()
	{
		m_pcDrawingAPI->Present();
	}

	/*** AQU_Drawer Bitmap methods ***/
	UINT             RegisterBitmap(HBITMAP hBitmap)
	{
		return m_pcDrawingAPI->RegisterBitmap(hBitmap);
	}
	void             UpdateBitmap(HBITMAP hBitmap, UINT dwID)
	{
		m_pcDrawingAPI->UpdateBitmap(hBitmap, dwID);
	}
	void             ReleaseBitmap(UINT dwID)
	{
		m_pcDrawingAPI->ReleaseBitmap(dwID);
	}

private:
	/**
	* The chosen drawing API.
	***/
	AQU_DrawingAPI* m_pcDrawingAPI;
};

#endif