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
#ifndef AQU_DIRECTDRAW_METHODS
#define AQU_DIRECTDRAW_METHODS

#include"AQU_DrawerFields.h"
#include <ddraw.h>
#include <stdio.h>
#include <vector>


/**
* Contains all Direct Draw stuff.
***/
class AQU_DirectDraw : public AQU_DrawingAPI
{
public:
	AQU_DirectDraw();
	~AQU_DirectDraw();

	/*** AQU_Drawer super methods ***/
	virtual HRESULT          LoadBackground(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY){ return LoadAdditionalBitmap(szPNG, dwForceX, dwForceY); }
	virtual void             ReleaseBackground(){ ReleaseAdditionalBitmap(); }
	virtual void             Resize(DWORD dwWidth, DWORD dwHeight) { ResizeBackbuffer(dwWidth, dwHeight); }
	virtual void             SetTextRange(DWORD dwMax){ SetMaxXTextOutput((LONG)dwMax); }
	virtual bool             VerifyAPI() { CheckSurfaces(); return !(IsSurfaceLocked()); }
	virtual DWORD            GetWidth() { return m_ddsd.dwWidth; }
	virtual DWORD            GetHeight() { return m_ddsd.dwHeight; }
	virtual void             Clear(){ Clear(m_ddsd.dwWidth, m_ddsd.dwHeight, AquilinusColor::Color0); }
	virtual void             Clear(DWORD dwColor){}
	virtual void             Clear(AquilinusColor eColor){ Clear(m_ddsd.dwWidth, m_ddsd.dwHeight, eColor); }
	virtual void             DrawBackground(DWORD dwX, DWORD dwY){ DrawAdditionalBitmap(dwX, dwY); }
	virtual void             Present() { Present(m_hWnd, m_ddsd.dwWidth, m_ddsd.dwHeight); }

	/*** AQU_DirectDraw Basic methods ***/
	DWORD               RGB_DD(BYTE r, BYTE g, BYTE b);
	HRESULT             Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize);
	void                CheckSurfaces();
	void                Clear(RECT rcDest, DWORD dwColor);
	void                Clear(RECT rcDest, AquilinusColor eColor);
	void                Clear(DWORD dwWindowWidth, DWORD dwWindowHeight);
	void                Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor);
	HRESULT             Present(HWND hwnd, DWORD dwWindowWidth, DWORD dwWindowHeight);
	void                Release();
	LPDIRECTDRAWSURFACE CreateOffScreenPlainSurface(DWORD dwX, DWORD dwY);
	DWORD               GetColorSchemesNumber();
	void                SetColorScheme(DWORD dwIndex);
	void                SetColorScheme(AQU_DrawingAPI *pDirectDraw);
	void                SetColorScheme(ColorScheme &sColorScheme);
	void                GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex);
	HRESULT             ColorSurface(DWORD dwColorFore, DWORD dwColorBack, LPDIRECTDRAWSURFACE pSurface);
	HRESULT             ResizeBackbuffer(DWORD dwWindowWidth, DWORD dwWindowHeight);
	HRESULT             LoadAdditionalBitmap(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY);
	void                ReleaseAdditionalBitmap();
	bool                IsSurfaceLocked() { return m_bSurfaceLocked; }

	/*** AQU_DirectDraw Font methods ***/
	void                SetMaxXTextOutput(LONG lMax);
	DWORD               GetGlyphIndex(char character);
	DWORD               GetTextWidth(LPCWSTR szText, float fSize, float fZoom);
	HRESULT             RenderGlyph(DWORD dwIndex, RECT &rcDest, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom);
	void                RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom);

	/*** AQU_DirectDraw Drawing methods ***/
	void                DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	void                DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom);
	void                DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom);
	void                DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	void                DrawEdge(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, bool bUpOrLeft, float fZoom);
	void                DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	void                DrawSurface(RECT rcDst, RECT rcSrc, POINT vecOrigin, LPDIRECTDRAWSURFACE pSurface, float fZoom);
	void                DrawAdditionalBitmap(DWORD dwX, DWORD dwY);

private:
	/**
	* Window handle.
	***/
	HWND m_hWnd;
	/**
	* Direct Draw object pointer.
	***/
	LPDIRECTDRAW        m_pDD;
	/**
	* Direct Draw clipper.
	***/
	LPDIRECTDRAWCLIPPER m_pClipper;
	/**
	* Direct Draw front surface.
	***/
	LPDIRECTDRAWSURFACE m_pDDS;
	/**
	* Direct Draw back buffer surface.
	***/
	LPDIRECTDRAWSURFACE m_pDDSBack;
	/**
	* Direct Draw surface description.
	***/
	DDSURFACEDESC m_ddsd;
	/**
	* Bits per pixel for the current device.
	***/
	int                 m_iBpp;
	/**
	* The Aquilinus color scheme.
	***/
	ColorScheme         m_currentColorScheme;
	/**
	* Font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Aquilinus;
	/**
	* First color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color1_0;
	/**
	* Second color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color2_0;
	/**
	* Third color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color3_0;
	/**
	* Fourth color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color4_0;
	/**
	* First color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color1_1;
	/**
	* Second color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color2_1;
	/**
	* Third color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color3_1;
	/**
	* Fourth color font surface.
	***/
	LPDIRECTDRAWSURFACE m_pFont_Color4_1;
	/**
	* Additional background surface.
	***/
	LPDIRECTDRAWSURFACE m_pBackground;
	/**
	* Additional bitmap.
	***/
	LPDIRECTDRAWSURFACE m_pBitmap;
	/**
	* Background image data.
	***/
	ImageData           m_sBackgroundImageData;
	/**
	* Bitmap image data.
	***/
	ImageData           m_sBitmapImageData;
	/**
	* The raw background image data.
	***/
	std::vector<BYTE>   m_paBackgroundImage;
	/**
	* The raw additional bitmap image data.
	***/
	std::vector<BYTE>   m_paBitmapImage;
	/**
	* True if background should be drawn.
	***/
	bool                m_bBackground;
	/**
	* True if bitmap should be drawn.
	***/
	bool                m_bBitmap;
	/**
	* Font bitmap.
	***/
	HBITMAP             m_hbFont_Aquilinus;
	/**
	* The module handle, used to load font.
	***/
	HMODULE             m_hModule;
	/**
	* The maximum x text output.
	* No limit if zero.
	***/
	LONG                m_lMaxXTextOutput;
	/**
	* True if font surfaces are held by other class.
	***/
	bool                m_bNoFontSurfaceRelease;
	/**
	* True if surface is locked.
	***/
	bool                m_bSurfaceLocked;
};
#endif