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
#ifndef AQU_OPENGL_CLASS
#define AQU_OPENGL_CLASS

#include"AQU_DrawerFields.h"
#include <stdio.h>
#include <vector>
#include <Wingdi.h>
#include <gl\GL.h>
#include <gl\GLU.h>

/**
* OpenGL vector 4f.
***/
typedef struct _GLVec4f
{
	GLfloat fX, fY, fZ, fW;
} GLVec4f;

/**
* OpenGL rectangle 4f.
***/
typedef struct _GLRec4f
{
	GLfloat fLeft, fTop, fRight, fBottom;
} GLRec4f;

/**
* OpenGL color 4f.
***/
typedef struct _GLCol4f
{
	GLclampf fR, fG, fB, fA;
} GLCol4f;


/**
* Contains all Direct Draw stuff.
***/
class AQU_OpenGL : public AQU_DrawingAPI
{
public:
	AQU_OpenGL();
	~AQU_OpenGL();

	/*** AQU_Drawer Basic methods ***/
	virtual HRESULT          Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize);
	virtual void             Release();
	virtual HRESULT          LoadBackground(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY);
	virtual void             ReleaseBackground();
	virtual void             Resize(DWORD dwWidth, DWORD dwHeight);
	virtual void             SetTextRange(DWORD dwMax);
	virtual DWORD            GetTextWidth(LPCWSTR szText, float fSize, float fZoom);
	virtual bool             VerifyAPI();
	virtual DWORD            GetWidth() { return (DWORD)m_sViewport.cx; }
	virtual DWORD            GetHeight() { return (DWORD)m_sViewport.cy; }

	/*** AQU_Drawer Color methods ***/
	virtual DWORD            GetColorSchemesNumber();
	virtual void             SetColorScheme(DWORD dwIndex);
	virtual void             SetColorScheme(AQU_DrawingAPI *pcDrawer);
	virtual void             SetColorScheme(ColorScheme &sColorScheme);
	virtual void             GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex);

	/*** AQU_Drawer Drawing methods ***/
	virtual void             Clear();
	virtual void             Clear(DWORD dwColor);
	virtual void             Clear(AquilinusColor eColor);
	virtual void             Clear(RECT rcDest, DWORD dwColor);
	virtual void             Clear(RECT rcDest, AquilinusColor eColor);
	virtual void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight);
	virtual void             Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor);
	virtual void             DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	virtual void             DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom);
	virtual void             DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom);
	virtual void             DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	virtual void             DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom);
	virtual void             DrawBitmap(UINT dwID, RECT rcDest, POINT vecOrigin, float fZoom);
	virtual void             DrawBackground(DWORD dwX, DWORD dwY);
	virtual void             RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom);
	virtual void             Present();

	/*** AQU_Drawer Bitmap methods ***/
	virtual UINT             RegisterBitmap(HBITMAP hBitmap);
	virtual void             UpdateBitmap(HBITMAP hBitmap, UINT dwID);
	virtual void             ReleaseBitmap(UINT dwID);

private:
	/*** AQU_OpenGL private methods ***/
	DWORD AQU_OpenGL::GetGlyphIndex(char cChar);
	void RenderGlyph(DWORD dwIndex, POINT sOrigin, float fX, float fY, float fScale, float fZoom);
	void RenderText(LPCWSTR szText, POINT sOrigin, float fX, float fY, float fScale, float fZoom);

	/**
	* Main window.
	***/
	HWND m_hWnd;
	/**
	* Device context handle.
	***/
	HDC m_hDC;
	/**
	* Open GL rendering context.
	***/
	HGLRC m_hGLRC;
	/**
	* The viewport size.
	***/
	SIZE m_sViewport;
	/**
	* The Aquilinus color scheme.
	***/
	ColorScheme m_sCurrentColorScheme;
	/**
	* The Aquilinus color scheme in GL format.
	***/
	GLCol4f m_asAquilinusColor[5];
	/**
	* Font bitmap.
	***/
	HBITMAP m_hbFont_Aquilinus;
	/**
	* The module handle, used to load font.
	***/
	HMODULE m_hModule;
	/**
	* The font texture identifier.
	***/
	GLuint m_nFontTexture;
	/**
	* The font texture width (float).
	***/
	float m_fFontTextureWidth;
	/**
	* The font texture height (float).
	***/
	float m_fFontTextureHeight;
	/**
	* The background texture identifier.
	***/
	GLuint m_nBackgroundTexture;
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

};
#endif