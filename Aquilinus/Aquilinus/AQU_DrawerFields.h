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
#ifndef AQU_DRAWER_FIELDS
#define AQU_DRAWER_FIELDS

#include<Windows.h>

#pragma region structures
/**
* Simple color enumeration for the simple Aquilinus color scheme.
***/
enum AquilinusColor
{
	Color0,
	Color1,
	Color2,
	Color3,
	Color4
};

/**
* Color scheme structure for 5 colors + creator info.
***/
struct ColorScheme
{
	DWORD dwColor0;
	DWORD dwColor1;
	DWORD dwColor2;
	DWORD dwColor3;
	DWORD dwColor4;
	WCHAR szName[32];
	WCHAR szLover[32];
	WCHAR szLink[128];
};

/**
* Basic PNG image data as stored in the IHDR chunk.
***/
struct ImageData
{
	DWORD dwWidth;
	DWORD dwHeight;
	BYTE chBitDepth;
	BYTE chColorType;
	BYTE chCompressionMethod;
	BYTE chFilterMethod;
	BYTE chInterlaceMethod;
};

/**
* Aquilinus drawing API virtual class.
***/
class AQU_DrawingAPI
{
public:
	AQU_DrawingAPI(){}
	~AQU_DrawingAPI(){}

	/*** AQU_Drawer Basic methods ***/
	virtual HRESULT          Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize){ return E_NOTIMPL; }
	virtual void             Release(){}
	virtual HRESULT          LoadBackground(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY){ return E_NOTIMPL; }
	virtual void             ReleaseBackground(){}
	virtual void             Resize(DWORD dwWidth, DWORD dwHeight) {}
	virtual void             SetTextRange(DWORD dwMax){}
	virtual DWORD            GetTextWidth(LPCWSTR szText, float fSize, float fZoom){ return 0; }
	virtual bool             VerifyAPI() { return true; }
	virtual DWORD            GetWidth() { return 0; }
	virtual DWORD            GetHeight() { return 0; }


	/*** AQU_Drawer Color methods ***/
	virtual DWORD            GetColorSchemesNumber() { return 0; }
	virtual void             SetColorScheme(DWORD dwIndex){}
	virtual void             SetColorScheme(AQU_DrawingAPI *pcDrawer){}
	virtual void             SetColorScheme(ColorScheme &sColorScheme){}
	virtual void             GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex){}

	/*** AQU_Drawer Drawing methods ***/
	virtual void             Clear(){}
	virtual void             Clear(DWORD dwColor){}
	virtual void             Clear(AquilinusColor eColor){}
	virtual void             Clear(RECT rcDest, DWORD dwColor){}
	virtual void             Clear(RECT rcDest, AquilinusColor eColor){}
	virtual void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight){}
	virtual void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor){}
	virtual void             DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom){}
	virtual void             DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom){}
	virtual void             DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom){}
	virtual void             DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom){}
	virtual void             DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom){}
	virtual void             DrawBitmap(UINT dwID, RECT rcDest, POINT vecOrigin, float fZoom){}
	virtual void             DrawBackground(DWORD dwX, DWORD dwY){}
	virtual void             RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom){}
	virtual void             Present(){}

	/*** AQU_Drawer Bitmap methods ***/
	virtual UINT             RegisterBitmap(HBITMAP hBitmap){ return 0;}
	virtual void             UpdateBitmap(HBITMAP hBitmap, UINT dwID){}
	virtual void             ReleaseBitmap(UINT dwID){}
};

/**
* Contains all data necessary for a glyph.
***/
struct Glyph
{
	char character;   /**< The glyph character. **/
	RECT rcSrc;       /**< The source rect (in pixel space). **/
	SIZE size;        /**< The size (in pixel space). **/
	LONG nAddSpacing; /**< Additional spacing. **/
};

#pragma endregion

#endif