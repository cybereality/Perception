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
#define PNG_DECODER_APP_NAME L"Aquilinus"

#include"Resources.h"
#include"AQU_OpenGL.h"
#include"AQU_DecodePNG.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }

// inherit from AQU_2DData :
#define AQUILINUS_NUMBER_OF_COLOR_SCHEMES 37
#define AQUILINUS_NUMBER_OF_FONT_GLYPHES 94
extern ColorScheme g_colorSchemes[];
extern Glyph g_fontGlyphes[];

#pragma region helper
/**
* Simple COLORREF->GLcolor converter.
* Ignores Alpha channel.
***/
inline GLCol4f FromCOLORREF(COLORREF dwColor)
{
	GLCol4f sRet;

	// background color
	sRet.fR = (GLclampf)GetRValue(dwColor) / (GLclampf)256.0f;
	sRet.fG = (GLclampf)GetGValue(dwColor) / (GLclampf)256.0f;
	sRet.fB = (GLclampf)GetBValue(dwColor) / (GLclampf)256.0f;
	sRet.fA = 0.0f;

	return sRet;
}

/**
* Simple COLORREF(BGR)->GLcolor converter.
* Ignores Alpha channel.
***/
inline GLCol4f FromCOLORREF_BGR(COLORREF dwColor)
{
	GLCol4f sRet;

	// background color
	sRet.fR = (GLclampf)GetBValue(dwColor) / (GLclampf)256.0f;
	sRet.fG = (GLclampf)GetGValue(dwColor) / (GLclampf)256.0f;
	sRet.fB = (GLclampf)GetRValue(dwColor) / (GLclampf)256.0f;
	sRet.fA = 0.0f;

	return sRet;
}

#pragma endregion

#pragma region constructor/destructor
/**
* Constructor.
***/
AQU_OpenGL::AQU_OpenGL() :
m_hGLRC(nullptr),
m_hDC(nullptr),
m_nFontTexture(0),
m_fFontTextureHeight(0),
m_fFontTextureWidth(0),
m_nBackgroundTexture(0)
{
	m_bBackground = false;
}

/**
* Destructor
***/
AQU_OpenGL::~AQU_OpenGL()
{
}
#pragma endregion

#pragma region AQU_Drawer Basic methods

/**
* Initialization
***/
HRESULT AQU_OpenGL::Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize)
{
	// get dc
	m_hWnd = hwnd;
	m_hDC = GetDC(hwnd);

	// set pixel format
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR sPFD = {
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		32,                                     //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		0,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		16,                                     //16 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored

	// Choose best matching format
	nPixelFormat = ChoosePixelFormat(m_hDC, &sPFD);

	// Set the pixel format to the device context
	SetPixelFormat(m_hDC, nPixelFormat, &sPFD);

	// create a rendering context  
	m_hGLRC = wglCreateContext(m_hDC);

	// make it the calling thread's current rendering context 
	wglMakeCurrent(m_hDC, m_hGLRC);

	// set the viewport
	glViewport(0, 0, dwWindowWidth, dwWindowHeight);
	m_sViewport.cx = (LONG)dwWindowWidth;
	m_sViewport.cy = (LONG)dwWindowHeight;

	// set the basic color scheme to init
	if (bSetColorScheme)
		SetColorScheme(dwColorSchemeIndex);

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

	// load the font texture
	m_hModule = GetModuleHandle(szModule);
	m_hbFont_Aquilinus = LoadBitmap(m_hModule, MAKEINTRESOURCE(IMG_FONT02));

	// get size of bitmap
	BITMAP bm;
	GetObject(m_hbFont_Aquilinus, sizeof(bm), &bm);
	m_fFontTextureWidth = (float)bm.bmWidth;
	m_fFontTextureHeight = (float)bm.bmHeight;

	BITMAPINFO sBitmapInfo = { 0 };
	sBitmapInfo.bmiHeader.biSize = sizeof(sBitmapInfo.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if (0 == GetDIBits(m_hDC, m_hbFont_Aquilinus, 0, 0, NULL, &sBitmapInfo, DIB_RGB_COLORS))
	{
		OutputDebugString(L"Error GetDIBits");
	}

	// create the pixel buffer
	BYTE* data = new BYTE[sBitmapInfo.bmiHeader.biSizeImage];
	if (data)
	{
		sBitmapInfo.bmiHeader.biBitCount = 32;
		sBitmapInfo.bmiHeader.biCompression = BI_RGB;
		sBitmapInfo.bmiHeader.biHeight = abs(sBitmapInfo.bmiHeader.biHeight);

		if (0 == GetDIBits(m_hDC, m_hbFont_Aquilinus, 0, sBitmapInfo.bmiHeader.biHeight,
			data, &sBitmapInfo, DIB_RGB_COLORS))
		{
			OutputDebugString(L"Error GetDIBits");
		}

		// ALPHA = B ; R = G
		for (int i = 0; i < bm.bmWidth * bm.bmHeight; i++)
		{
			data[i * 4] = data[i * 4 + 1];
			data[i * 4 + 3] = data[i * 4 + 2];
		}

		// create one texture
		glGenTextures(1, &m_nFontTexture);

		// select texture
		glBindTexture(GL_TEXTURE_2D, m_nFontTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bm.bmWidth, bm.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bm.bmWidth, bm.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
		glDisable(GL_TEXTURE_2D);
	}
#endif

	// set background bool
	m_bBackground = bBackground;

	// create a background image ?
	if (m_bBackground)
	{
		OutputDebugString(L"Background image available.");

		// load a test background
		PNG_ImageData sImageData;

		// if the path is empty use the picture data instead
		HRESULT hr;
		if (szBackgroundPNG[0] == 0)
		{
			hr = DecodePNGFile(nullptr, pPicture, dwPictureSize, sImageData, m_paBackgroundImage);
		}
		else
		{
			hr = DecodePNGFile(szBackgroundPNG, nullptr, 0, sImageData, m_paBackgroundImage);
		}

		if (SUCCEEDED(hr))
		{
			OutputDebugString(L"Succeeded to decode PNG file !");
			// copy to direct draw structure
			memcpy((void*)&m_sBackgroundImageData, (void*)&sImageData, sizeof(PNG_ImageData));

			// create one texture
			glGenTextures(1, &m_nBackgroundTexture);

			// select texture
			glBindTexture(GL_TEXTURE_2D, m_nBackgroundTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			// generate texture

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_sBackgroundImageData.dwWidth, (GLsizei)m_sBackgroundImageData.dwHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_paBackgroundImage.data()[0]);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight, GL_RGBA, GL_UNSIGNED_BYTE, &m_paBackgroundImage.data()[0]);

			glDisable(GL_TEXTURE_2D);

			// adjust viewport
			glViewport(0, 0, m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
			m_sViewport.cx = (LONG)m_sBackgroundImageData.dwWidth;
			m_sViewport.cy = (LONG)m_sBackgroundImageData.dwHeight;
		}
		else m_bBackground = false;
	}

	return S_OK;
}

/**
* Release rendering context and device context.
***/
void AQU_OpenGL::Release()
{
	// make the rendering context not current  
	wglMakeCurrent(NULL, NULL);

	// delete the rendering context
	if (m_hGLRC)
		wglDeleteContext(m_hGLRC);

	ReleaseDC(m_hWnd, m_hDC);
}

/**
*
***/
HRESULT AQU_OpenGL::LoadBackground(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY)
{
	return (E_NOTIMPL);
}

/**
*
***/
void AQU_OpenGL::ReleaseBackground()
{
	(E_NOTIMPL);
}

/**
*
***/
void AQU_OpenGL::Resize(DWORD dwWidth, DWORD dwHeight)
{
	// adjust viewport
	glViewport(0, 0, dwWidth, dwHeight);
	m_sViewport.cx = (LONG)dwWidth;
	m_sViewport.cy = (LONG)dwHeight;
}

/**
*
***/
void AQU_OpenGL::SetTextRange(DWORD dwMax)
{
	(E_NOTIMPL);
}

/**
*
***/
DWORD AQU_OpenGL::GetTextWidth(LPCWSTR szText, float fSize, float fZoom)
{
	// TODO !! DELETE ZOOM MEMBER ?? NEED IT ??

	DWORD dwRet = 0;

	for (DWORD dwIndex = 0; dwIndex < wcslen(szText); dwIndex++)
	{
		DWORD dwInd = GetGlyphIndex((char)szText[dwIndex]);

		// set size (only if < 1.0f to avoid zoom in)
		if (fSize < 1.0f)
		{
			// zoom out the glyph
			dwRet += static_cast<DWORD>(float(g_fontGlyphes[dwInd].rcSrc.right)*fSize);
		}
		else
			dwRet += g_fontGlyphes[dwInd].rcSrc.right;

		// space between characters == 2
		dwRet += 2;
	}

	return dwRet;
}

/**
*
***/
bool AQU_OpenGL::VerifyAPI()
{
	return true; (E_NOTIMPL);
}
#pragma endregion

#pragma region AQU_Drawer Color methods
/**
* Returns the number of current color schemes
***/
DWORD AQU_OpenGL::GetColorSchemesNumber()
{
	return AQUILINUS_NUMBER_OF_COLOR_SCHEMES;
}

/**
*
***/
void AQU_OpenGL::SetColorScheme(DWORD dwIndex)
{
	if (dwIndex >= AQUILINUS_NUMBER_OF_COLOR_SCHEMES)
		return SetColorScheme(g_colorSchemes[0]);

	return SetColorScheme(g_colorSchemes[dwIndex]);
}

/**
*
***/
void AQU_OpenGL::SetColorScheme(AQU_DrawingAPI *pcDrawer)
{
	// set the current color scheme
	memcpy(&m_sCurrentColorScheme, &((AQU_OpenGL*)pcDrawer)->m_sCurrentColorScheme, sizeof(ColorScheme));

	// set GL colors
	m_asAquilinusColor[0] = FromCOLORREF_BGR(m_sCurrentColorScheme.dwColor0);
	m_asAquilinusColor[1] = FromCOLORREF_BGR(m_sCurrentColorScheme.dwColor1);
	m_asAquilinusColor[2] = FromCOLORREF_BGR(m_sCurrentColorScheme.dwColor2);
	m_asAquilinusColor[3] = FromCOLORREF_BGR(m_sCurrentColorScheme.dwColor3);
	m_asAquilinusColor[4] = FromCOLORREF_BGR(m_sCurrentColorScheme.dwColor4);
}

/**
* Set a color scheme.
* @param sColorScheme The color scheme to be instantly used by all Direct Draw methods.
***/
void AQU_OpenGL::SetColorScheme(ColorScheme &sColorScheme)
{
	// set the current color scheme
	memcpy(&m_sCurrentColorScheme, &sColorScheme, sizeof(ColorScheme));

	// set GL colors
	m_asAquilinusColor[0] = FromCOLORREF_BGR(sColorScheme.dwColor0);
	m_asAquilinusColor[1] = FromCOLORREF_BGR(sColorScheme.dwColor1);
	m_asAquilinusColor[2] = FromCOLORREF_BGR(sColorScheme.dwColor2);
	m_asAquilinusColor[3] = FromCOLORREF_BGR(sColorScheme.dwColor3);
	m_asAquilinusColor[4] = FromCOLORREF_BGR(sColorScheme.dwColor4);
}

/**
* Get a color scheme by index.
***/
void AQU_OpenGL::GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex)
{
	if (dwIndex >= (DWORD)AQUILINUS_NUMBER_OF_COLOR_SCHEMES) return;

	sColorScheme.dwColor0 = g_colorSchemes[dwIndex].dwColor0;
	sColorScheme.dwColor1 = g_colorSchemes[dwIndex].dwColor1;
	sColorScheme.dwColor2 = g_colorSchemes[dwIndex].dwColor2;
	sColorScheme.dwColor3 = g_colorSchemes[dwIndex].dwColor3;
	sColorScheme.dwColor4 = g_colorSchemes[dwIndex].dwColor4;
	CopyMemory(&sColorScheme.szName[0], &g_colorSchemes[dwIndex].szName[0], 32 * sizeof(wchar_t));
	CopyMemory(&sColorScheme.szLover[0], &g_colorSchemes[dwIndex].szLover[0], 32 * sizeof(wchar_t));
	CopyMemory(&sColorScheme.szLink[0], &g_colorSchemes[dwIndex].szLink[0], 128 * sizeof(wchar_t));
}
#pragma endregion

#pragma region AQU_Drawer Drawing methods
/**
* Clear background using background color.
***/
void AQU_OpenGL::Clear()
{
	Clear((DWORD)m_sViewport.cx, (DWORD)m_sViewport.cy);
}

/**
* Clear background using COLORREF (BGR) color.
***/
void AQU_OpenGL::Clear(DWORD dwColor)
{
	// clear the background by specified COLORREF background color
	GLCol4f sColor = FromCOLORREF_BGR((COLORREF)dwColor);
	glClearColor(sColor.fR,
		sColor.fG,
		sColor.fB,
		sColor.fA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
* Clear background using Aquilinus color.
***/
void AQU_OpenGL::Clear(AquilinusColor eColor)
{
	// clear the background by Aquilinus background color
	glClearColor(m_asAquilinusColor[eColor].fR,
		m_asAquilinusColor[eColor].fG,
		m_asAquilinusColor[eColor].fB,
		m_asAquilinusColor[eColor].fA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
* Partially clear background using COLORREF (BGR) color.
***/
void AQU_OpenGL::Clear(RECT rcDest, DWORD dwColor)
{
	// get a float rectangle
	GLRec4f sDest = { 0 };
	sDest.fLeft = ((GLfloat)rcDest.left * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fRight = ((GLfloat)rcDest.right * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fTop = (((GLfloat)rcDest.top * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;
	sDest.fBottom = (((GLfloat)rcDest.bottom * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;

	// Draw a square
	GLCol4f sColor = FromCOLORREF_BGR((COLORREF)dwColor);
	glBegin(GL_QUADS);
	glColor3f(sColor.fR, sColor.fG, sColor.fB);
	glVertex2f(sDest.fLeft, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fBottom);
	glVertex2f(sDest.fLeft, sDest.fBottom);
	glEnd();
}

/**
* Partially clear background using Aquilinus color.
***/
void AQU_OpenGL::Clear(RECT rcDest, AquilinusColor eColor)
{
	// get a float rectangle
	GLRec4f sDest = { 0 };
	sDest.fLeft = ((GLfloat)rcDest.left * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fRight = ((GLfloat)rcDest.right * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fTop = (((GLfloat)rcDest.top * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;
	sDest.fBottom = (((GLfloat)rcDest.bottom * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;

	// Draw a square
	glBegin(GL_QUADS);
	glColor3f(m_asAquilinusColor[eColor].fR, m_asAquilinusColor[eColor].fG, m_asAquilinusColor[eColor].fB);
	glVertex2f(sDest.fLeft, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fBottom);
	glVertex2f(sDest.fLeft, sDest.fBottom);
	glEnd();
}

/**
* Clear background with specified size using Aquilinus background color.
***/
void AQU_OpenGL::Clear(DWORD dwWindowWidth, DWORD dwWindowHeight)
{
	// clear the background by Aquilinus background color
	glClearColor(m_asAquilinusColor[AquilinusColor::Color0].fR,
		m_asAquilinusColor[AquilinusColor::Color0].fG,
		m_asAquilinusColor[AquilinusColor::Color0].fB,
		m_asAquilinusColor[AquilinusColor::Color0].fA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// background ?
	if (m_nBackgroundTexture)
	{
		// set texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_nBackgroundTexture);

		// Draw a square
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

}

/**
* Clear background with specified size using spezified Aquilinus color.
* TODO !! currently clears the whole window
***/
void AQU_OpenGL::Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor)
{
	// clear the background by Aquilinus background color
	glClearColor(m_asAquilinusColor[eColor].fR,
		m_asAquilinusColor[eColor].fG,
		m_asAquilinusColor[eColor].fB,
		m_asAquilinusColor[eColor].fA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
*
***/
void AQU_OpenGL::DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
{
	// zoom + screen space converrt
	GLfloat fX1 = ((GLfloat)vecStart.x * 2.0f * fZoom) / (GLfloat)m_sViewport.cx - 1.0f;
	GLfloat fY1 = (((GLfloat)vecStart.y * 2.0f * fZoom) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;
	GLfloat fX2 = ((GLfloat)vecEnd.x * 2.0f * fZoom) / (GLfloat)m_sViewport.cx - 1.0f;
	GLfloat fY2 = (((GLfloat)vecEnd.y * 2.0f * fZoom) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;

	// calc origin
	GLfloat fOriginX = ((GLfloat)vecOrigin.x * 2.0f) / (GLfloat)m_sViewport.cx;
	GLfloat fOriginY = (((GLfloat)vecOrigin.y * 2.0f) / (GLfloat)m_sViewport.cy) * -1.0f;

	// draw the line, first zoom line width
	glLineWidth((GLfloat)nSize * fZoom);
	glColor3f(m_asAquilinusColor[eColor].fR, m_asAquilinusColor[eColor].fG, m_asAquilinusColor[eColor].fB);
	glBegin(GL_LINES);
	glVertex3f(fX1 + fOriginX, fY1 + fOriginY, 0.0f);
	glVertex3f(fX2 + fOriginX, fY2 + fOriginY, 0.0f);
	glEnd();
}

/**
* Draw a horizontal line.
***/
void AQU_OpenGL::DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
{
	// set the rectangle
	RECT rcDest;
	SetRect(&rcDest, vecStart.x, vecStart.y, vecStart.x + nLength, vecStart.y + nSize);

	// set zoom, add origin clamp
	rcDest.left = (LONG)((float)rcDest.left * fZoom) + vecOrigin.x;
	rcDest.right = (LONG)((float)rcDest.right * fZoom) + vecOrigin.x;
	rcDest.top = (LONG)((float)rcDest.top * fZoom) + vecOrigin.y;
	rcDest.bottom = (LONG)((float)rcDest.bottom * fZoom) + vecOrigin.y;
	if (rcDest.left < 0) rcDest.left = 0;
	if (rcDest.top < 0) rcDest.top = 0;
	if (rcDest.right > m_sViewport.cx) rcDest.right = m_sViewport.cx;
	if (rcDest.bottom > m_sViewport.cy) rcDest.bottom = m_sViewport.cy;

	// get a float rectangle
	GLRec4f sDest = { 0 };
	sDest.fLeft = ((GLfloat)rcDest.left * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fRight = ((GLfloat)rcDest.right * 2.0f) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fTop = (((GLfloat)rcDest.top * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;
	sDest.fBottom = (((GLfloat)rcDest.bottom * 2.0f) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;

	// Draw a square
	glBegin(GL_QUADS);
	glColor3f(m_asAquilinusColor[eColor].fR, m_asAquilinusColor[eColor].fG, m_asAquilinusColor[eColor].fB);
	glVertex2f(sDest.fLeft, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fTop);
	glVertex2f(sDest.fRight, sDest.fBottom);
	glVertex2f(sDest.fLeft, sDest.fBottom);
	glEnd();
}

/**
* Draw a vertical line.
***/
void AQU_OpenGL::DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
{
	// simply change size and length, call horizontal line method
	DrawHLine(vecStart, vecOrigin, nSize, nLength, eColor, fZoom);
}

/**
*
***/
void AQU_OpenGL::DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
{
	(E_NOTIMPL);
}

/**
* Draw a box.
***/
void AQU_OpenGL::DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
{
	POINT pt;
	pt.x = rcBox.left; pt.y = rcBox.top;
	DrawVLine(pt, vecOrigin, rcBox.bottom - rcBox.top + nSize, nSize, eColor, fZoom);
	pt.x = rcBox.right;
	DrawVLine(pt, vecOrigin, rcBox.bottom - rcBox.top + nSize, nSize, eColor, fZoom);
	pt.x = rcBox.left + nSize;
	DrawHLine(pt, vecOrigin, rcBox.right - rcBox.left - nSize, nSize, eColor, fZoom);
	pt.y = rcBox.bottom;
	DrawHLine(pt, vecOrigin, rcBox.right - rcBox.left - nSize, nSize, eColor, fZoom);
}

/**
*
***/
void AQU_OpenGL::DrawBitmap(UINT dwID, RECT rcDest, POINT vecOrigin, float fZoom)
{
	// get a float rectangle
	GLRec4f sDest = { 0 };
	sDest.fLeft = ((GLfloat)rcDest.left * 2.0f * fZoom) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fRight = ((GLfloat)rcDest.right * 2.0f * fZoom) / (GLfloat)m_sViewport.cx - 1.0f;
	sDest.fTop = (((GLfloat)rcDest.top * 2.0f * fZoom) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;
	sDest.fBottom = (((GLfloat)rcDest.bottom * 2.0f * fZoom) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f;

	// calc origin
	GLfloat fOriginX = ((GLfloat)vecOrigin.x * 2.0f) / (GLfloat)m_sViewport.cx;
	GLfloat fOriginY = (((GLfloat)vecOrigin.y * 2.0f) / (GLfloat)m_sViewport.cy) * -1.0f;

	// set texture
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint)dwID);

	// set mipmap parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Draw a square
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(sDest.fLeft + fOriginX, sDest.fTop + fOriginY);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(sDest.fLeft + fOriginX, sDest.fBottom + fOriginY);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(sDest.fRight + fOriginX, sDest.fBottom + fOriginY);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(sDest.fRight + fOriginX, sDest.fTop + fOriginY);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

/**
*
***/
void AQU_OpenGL::DrawBackground(DWORD dwX, DWORD dwY)
{
	(E_NOTIMPL);
}

/**
*
***/
void AQU_OpenGL::RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom)
{
	// set color
	glColor3f(m_asAquilinusColor[eColor].fR,
		m_asAquilinusColor[eColor].fG,
		m_asAquilinusColor[eColor].fB);

	// set texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nFontTexture);

	// set alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set mipmap parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	RenderText(szText, vecOrigin, ((GLfloat)nX * 2.0f * fZoom) / (GLfloat)m_sViewport.cx - 1.0f, (((GLfloat)nY * 2.0f * fZoom) / (GLfloat)m_sViewport.cy - 1.0f) * -1.0f, fSize, fZoom);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

/**
*
***/
void AQU_OpenGL::Present()
{
	// flush, swap buffers
	glFlush();
	SwapBuffers(m_hDC);
}
#pragma endregion

#pragma region AQU_OpenGL Bitmap methods
/**
* Registers a bitmap to be used by OpenGL.
* @returns The ID of the bitmap.
***/
UINT AQU_OpenGL::RegisterBitmap(HBITMAP hBitmap)
{
	// get size of bitmap
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);

	BITMAPINFO sBitmapInfo = { 0 };
	sBitmapInfo.bmiHeader.biSize = sizeof(sBitmapInfo.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if (0 == GetDIBits(m_hDC, hBitmap, 0, 0, NULL, &sBitmapInfo, DIB_RGB_COLORS))
	{
		OutputDebugString(L"Error GetDIBits");
	}

	// create the pixel buffer
	BYTE* data = new BYTE[sBitmapInfo.bmiHeader.biSizeImage];
	GLuint nTexture = 0;
	if (data)
	{
		sBitmapInfo.bmiHeader.biBitCount = 32;
		sBitmapInfo.bmiHeader.biCompression = BI_RGB;
		sBitmapInfo.bmiHeader.biHeight = abs(sBitmapInfo.bmiHeader.biHeight);

		if (0 == GetDIBits(m_hDC, hBitmap, 0, sBitmapInfo.bmiHeader.biHeight,
			data, &sBitmapInfo, DIB_RGB_COLORS))
		{
			OutputDebugString(L"Error GetDIBits");
		}

		// BGRA -> RGBA
		BYTE chB;
		for (int i = 0; i < bm.bmWidth * bm.bmHeight; i++)
		{
			chB = data[i * 4];
			data[i * 4] = data[i * 4 + 2];
			data[i * 4 + 2] = chB;
		}

		// create one texture
		glGenTextures(1, &nTexture);

		// select texture
		glBindTexture(GL_TEXTURE_2D, nTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bm.bmWidth, bm.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bm.bmWidth, bm.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
		glDisable(GL_TEXTURE_2D);
	}

	return (UINT)nTexture;
}

/**
*
***/
void AQU_OpenGL::UpdateBitmap(HBITMAP hBitmap, UINT dwID)
{
	// get size of bitmap
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);

	BITMAPINFO sBitmapInfo = { 0 };
	sBitmapInfo.bmiHeader.biSize = sizeof(sBitmapInfo.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if (0 == GetDIBits(m_hDC, hBitmap, 0, 0, NULL, &sBitmapInfo, DIB_RGB_COLORS))
	{
		OutputDebugString(L"Error GetDIBits");
	}

	// create the pixel buffer
	BYTE* data = new BYTE[sBitmapInfo.bmiHeader.biSizeImage];
	if (data)
	{
		sBitmapInfo.bmiHeader.biBitCount = 32;
		sBitmapInfo.bmiHeader.biCompression = BI_RGB;
		sBitmapInfo.bmiHeader.biHeight = abs(sBitmapInfo.bmiHeader.biHeight);

		if (0 == GetDIBits(m_hDC, hBitmap, 0, sBitmapInfo.bmiHeader.biHeight,
			data, &sBitmapInfo, DIB_RGB_COLORS))
		{
			OutputDebugString(L"Error GetDIBits");
		}

		// BGRA -> RGBA
		BYTE chB;
		for (int i = 0; i < bm.bmWidth * bm.bmHeight; i++)
		{
			chB = data[i * 4];
			data[i * 4] = data[i * 4 + 2];
			data[i * 4 + 2] = chB;
		}

		// select texture
		glBindTexture(GL_TEXTURE_2D, (GLuint)dwID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// update texture
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bm.bmWidth, bm.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bm.bmWidth, bm.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bm.bmWidth, bm.bmHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
		glDisable(GL_TEXTURE_2D);
	}
}

/**
*
***/
void AQU_OpenGL::ReleaseBitmap(UINT dwID)
{
	glDeleteTextures(1, (const GLuint*)&dwID);
}
#pragma endregion

#pragma region AQU_OpenGL private methods
/**
* Get the index of the character in the font table.
***/
DWORD AQU_OpenGL::GetGlyphIndex(char cChar)
{
	DWORD d;
	for (d = 0; d < AQUILINUS_NUMBER_OF_FONT_GLYPHES; d++)
	{
		if (cChar == g_fontGlyphes[d].character)
			return d;
	}

	return 0;
}

/**
* Renders a glyph using the main font table by the specified index.
* @param [in] dwIndex The index of the glyph.
* @param [in] fX The destination X.
* @param [in] fY The destination Y.
* @param [in] fScale The size of the text (1.0f = original size).
***/
void AQU_OpenGL::RenderGlyph(DWORD dwIndex, POINT sOrigin, float fX, float fY, float fScale, float fZoom)
{
	// set glyph position, first calculate top
	float fYY = fY - (((GLfloat)g_fontGlyphes[dwIndex].rcSrc.top * fZoom) / (GLfloat)m_sViewport.cy) * fScale;
	float Pos[] = { fX, fYY };

	// calc origin
	GLfloat fOriginX = ((GLfloat)sOrigin.x * 2.0f) / (GLfloat)m_sViewport.cx;
	GLfloat fOriginY = (((GLfloat)sOrigin.y * 2.0f) / (GLfloat)m_sViewport.cy) * -1.0f;

	// TODO !! USE GLRec4f HERE !! (INSTEAD OF THIS CHAOS HERE)

	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(float(g_fontGlyphes[dwIndex].rcSrc.left) / m_fFontTextureWidth, 1.0f - (float(g_fontGlyphes[dwIndex].rcSrc.top) / m_fFontTextureHeight));
	glVertex3f(fX + fOriginX,
		fYY + fOriginY, 0.0f);
	glTexCoord2f(float(g_fontGlyphes[dwIndex].rcSrc.left) / m_fFontTextureWidth, 1.0f - (float(g_fontGlyphes[dwIndex].rcSrc.top + g_fontGlyphes[dwIndex].rcSrc.bottom) / m_fFontTextureHeight));
	glVertex3f(fX + fOriginX,
		fYY - (((GLfloat)g_fontGlyphes[dwIndex].rcSrc.bottom * fZoom) / (GLfloat)m_sViewport.cy) * fScale + fOriginY, 0.0f);
	glTexCoord2f(float(g_fontGlyphes[dwIndex].rcSrc.left + g_fontGlyphes[dwIndex].rcSrc.right) / m_fFontTextureWidth, 1.0f - (float(g_fontGlyphes[dwIndex].rcSrc.top + g_fontGlyphes[dwIndex].rcSrc.bottom) / m_fFontTextureHeight));
	glVertex3f(fX + (((GLfloat)g_fontGlyphes[dwIndex].rcSrc.right * fZoom) / (GLfloat)m_sViewport.cx) * fScale + fOriginX,
		fYY - (((GLfloat)g_fontGlyphes[dwIndex].rcSrc.bottom * fZoom) / (GLfloat)m_sViewport.cy) * fScale + fOriginY, 0.0f);
	glTexCoord2f(float(g_fontGlyphes[dwIndex].rcSrc.left + g_fontGlyphes[dwIndex].rcSrc.right) / m_fFontTextureWidth, 1.0f - (float(g_fontGlyphes[dwIndex].rcSrc.top) / m_fFontTextureHeight));
	glVertex3f(fX + (((GLfloat)g_fontGlyphes[dwIndex].rcSrc.right * fZoom) / (GLfloat)m_sViewport.cx) * fScale + fOriginX,
		fYY + fOriginY, 0.0f);
	glEnd();
}

/**
* Outputs text to the backbuffer.
* @param [in] szText The text to render.
* @param [in] fX The destination X.
* @param [in] fY The destination Y.
* @param [in] fScale The size of the text (1.0f = original size).
***/
void AQU_OpenGL::RenderText(LPCWSTR szText, POINT sOrigin, float fX, float fY, float fScale, float fZoom)
{
	DWORD dwIndex = 0;
	float fXX = fX;
	float fZZoom = fZoom * 2.0f;

	// walk through the text, render glyphes
	for (dwIndex = 0; dwIndex < wcslen(szText); dwIndex++)
	{
		DWORD dwI = GetGlyphIndex((char)szText[dwIndex]);
		RenderGlyph(dwI, sOrigin, fXX, fY, fScale, fZZoom);
		fXX += ((((GLfloat)g_fontGlyphes[dwI].rcSrc.right + (GLfloat)g_fontGlyphes[dwI].nAddSpacing) * fZZoom) / (GLfloat)m_sViewport.cx) * fScale;
	}
}
#pragma endregion