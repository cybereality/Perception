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
#include"AQU_DirectDraw.h"
#include"AQU_2DData.h"
#include"AQU_DecodePNG.h"

/**
* Constructor.
***/
AQU_DirectDraw::AQU_DirectDraw()
{
	m_hWnd = NULL;
	m_pDD = NULL;
	m_pClipper = NULL;
	m_pDDS = NULL;
	m_pDDSBack = NULL;
	m_iBpp = 0;
	m_pFont_Aquilinus = NULL;
	m_pFont_Color1_0 = NULL;
	m_pFont_Color2_0 = NULL;
	m_pFont_Color3_0 = NULL;
	m_pFont_Color4_0 = NULL;
	m_pFont_Color1_1 = NULL;
	m_pFont_Color2_1 = NULL;
	m_pFont_Color3_1 = NULL;
	m_pFont_Color4_1 = NULL;
	m_hbFont_Aquilinus = NULL;
	m_pBackground = NULL;
	m_pBitmap = NULL;
	m_lMaxXTextOutput = 0;
	m_bSurfaceLocked = false;
	m_bBitmap = false;
	m_bBackground = false;
	m_bNoFontSurfaceRelease = false;
}

/**
* Destructor.
* Calls Release().
***/
AQU_DirectDraw::~AQU_DirectDraw()
{
	Release();
}

/***
* Direct Draw RGB helper.
***/
DWORD AQU_DirectDraw::RGB_DD(BYTE r, BYTE g, BYTE b) { if (m_iBpp < 24) return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8); else return (r << 16) | (g << 8) | (b); }

/**
* Init all Direct Draw stuff.
***/
HRESULT AQU_DirectDraw::Init(HWND hwnd, LPCWSTR szModule, DWORD dwWindowWidth, DWORD dwWindowHeight, bool bSetColorScheme, DWORD dwColorSchemeIndex, bool bBackground, LPWSTR szBackgroundPNG, BYTE* pPicture, DWORD dwPictureSize)
{
	// set main window
	m_hWnd = hwnd;

	// Initialize DirectDraw
	if (FAILED(DirectDrawCreate(NULL, &m_pDD, NULL)))
	{
		OutputDebugString(L"Aquilinus : Failed to initialize Direct Draw!");
		return E_FAIL;
	}

	// set coop level
	if (FAILED(m_pDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL)))
	{
		OutputDebugString(L"Aquilinus : Failed to set Direct Draw cooperative level !");
		return E_FAIL;
	}

	// create ddraw surface
	memset(&m_ddsd, 0, sizeof(m_ddsd));
	m_ddsd.dwSize = sizeof(m_ddsd);
	m_ddsd.dwFlags = DDSD_CAPS;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (FAILED(m_pDD->CreateSurface(&m_ddsd, &m_pDDS, NULL)))
	{
		OutputDebugString(L"Aquilinus : Failed to create primary Direct Draw surface!");
		return E_FAIL;
	}

	// create ddraw back buffer
	m_ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	m_ddsd.dwWidth = dwWindowWidth;
	m_ddsd.dwHeight = dwWindowHeight;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	if (FAILED(m_pDD->CreateSurface(&m_ddsd, &m_pDDSBack, NULL)))
	{
		OutputDebugString(L"Aquilinus : Failed to create Direct Draw back buffer surface!");
		return E_FAIL;
	}

	// create the clipper
	if (FAILED(m_pDD->CreateClipper(0, &m_pClipper, NULL)))
	{
		OutputDebugString(L"Aquilinus : Failed to create Direct Draw clipper !");
		return E_FAIL;
	}

	// set the window
	if (FAILED(m_pClipper->SetHWnd(0, hwnd)))
	{
		OutputDebugString(L"Aquilinus : Failed to set Direct Draw window !");
		return E_FAIL;
	}

	// Attach the clipper to the primary surface
	if (FAILED(m_pDDS->SetClipper(m_pClipper)))
	{
		OutputDebugString(L"Aquilinus : Failed to set Direct Draw clipper !");
		return E_FAIL;
	}

	// get surface info
	if (m_pDDSBack)
	{
		if (SUCCEEDED(m_pDDSBack->Lock(NULL, &m_ddsd, DDLOCK_WAIT, NULL)))
		{

			// Store bit depth of surface
			m_iBpp = m_ddsd.ddpfPixelFormat.dwRGBBitCount;

			// Unlock surface
			m_pDDSBack->Unlock(NULL);
		}
		else
		{
			OutputDebugString(L"Aquilinus : Failed to lock Direct Draw back buffer !");
			return E_FAIL;
		}
	}

#ifndef AQUILINUS_RUNTIME_ENVIRONMENT

	// load the font(s)
	m_hModule = GetModuleHandle(szModule);
	m_hbFont_Aquilinus = LoadBitmap(m_hModule, MAKEINTRESOURCE(IMG_FONT01));
	if (m_hbFont_Aquilinus == NULL)
	{
		OutputDebugString(L"Could not load font!");
		return E_FAIL;
	}

	// get size of bitmap
	BITMAP bm;
	GetObject(m_hbFont_Aquilinus, sizeof(bm), &bm);

	// create all DirectDrawSurface pointers for this bitmap font
	m_pFont_Aquilinus = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color1_0 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color2_0 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color3_0 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color4_0 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color1_1 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color2_1 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color3_1 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	m_pFont_Color4_1 = CreateOffScreenPlainSurface(bm.bmWidth, bm.bmHeight);
	HDC hdcImage = CreateCompatibleDC(NULL);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, m_hbFont_Aquilinus);

	HDC hdc;
	if ((m_pFont_Aquilinus->GetDC(&hdc)) == DD_OK)
	{
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
		m_pFont_Aquilinus->ReleaseDC(hdc);
	}

	SelectObject(hdcImage, hbmOld);
	DeleteDC(hdcImage);
	DeleteObject(m_hbFont_Aquilinus);

	// set the basic color scheme to init
	if (bSetColorScheme)
		SetColorScheme(dwColorSchemeIndex);

#endif

	// set background bool
	m_bBackground = bBackground;

	// create a background image ?
	if (m_bBackground)
	{
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
			// copy to direct draw structure
			memcpy((void*)&m_sBackgroundImageData, (void*)&sImageData, sizeof(PNG_ImageData));

			// create background surface
			m_pBackground = CreateOffScreenPlainSurface(m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
			DDSURFACEDESC desc;
			ZeroMemory(&desc, sizeof(DDSURFACEDESC));
			desc.dwSize = sizeof(DDSURFACEDESC);
			if (SUCCEEDED(m_pBackground->Lock(NULL, &desc, DDLOCK_WAIT, NULL)))
			{
				BYTE* pdwRow;

				for (DWORD y = 0; y < m_sBackgroundImageData.dwHeight; y++)
				{
					// get a pointer to the next row
					pdwRow = ((BYTE*)desc.lpSurface) + y*desc.lPitch;

					// copy the scanline
					memcpy((void*)pdwRow, (void*)&m_paBackgroundImage.data()[y*m_sBackgroundImageData.dwWidth * 4], m_sBackgroundImageData.dwWidth * 4);
				}

				// Unlock surface
				m_pBackground->Unlock(NULL);

				// resize back buffer
				ResizeBackbuffer(m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
			}
			else
			{
				m_paBackgroundImage.clear();
				return E_FAIL;
			}
		}
		else m_bBackground = false;
	}

	return S_OK;
}

/**
* Direct Draw surfaces check.
***/
void AQU_DirectDraw::CheckSurfaces()
{
	// Check the primary surface
	if (m_pDDS)
	{
		if (m_pDDS->IsLost() == DDERR_SURFACELOST)
			m_pDDS->Restore();
	}
	// Check the back buffer
	if (m_pDDSBack)
	{
		if (m_pDDSBack->IsLost() == DDERR_SURFACELOST)
			m_pDDSBack->Restore();
	}
	// ..and additional bitmaps
	if (m_pBackground)
	{
		if (m_pBackground->IsLost() == DDERR_SURFACELOST)
		{
			m_pBackground->Restore();
			if (m_bBackground)
			{
				if (m_paBackgroundImage.size() >= (m_sBackgroundImageData.dwHeight*m_sBackgroundImageData.dwWidth * 4))
				{
					DDSURFACEDESC desc;
					ZeroMemory(&desc, sizeof(DDSURFACEDESC));
					desc.dwSize = sizeof(DDSURFACEDESC);
					if (SUCCEEDED(m_pBackground->Lock(NULL, &desc, DDLOCK_WAIT, NULL)))
					{
						BYTE* pdwRow;

						for (DWORD y = 0; y < m_sBackgroundImageData.dwHeight; y++)
						{
							// get a pointer to the next row
							pdwRow = ((BYTE*)desc.lpSurface) + y*desc.lPitch;

							// copy the scanline
							memcpy((void*)pdwRow, (void*)&m_paBackgroundImage.data()[y*m_sBackgroundImageData.dwWidth * 4], m_sBackgroundImageData.dwWidth * 4);
						}

						// Unlock surface
						m_pBackground->Unlock(NULL);

						// resize back buffer
						ResizeBackbuffer(m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
					}
				}
			}
		}
	}
	if (m_pBitmap)
	{
		if (m_pBitmap->IsLost() == DDERR_SURFACELOST)
		{
			m_pBitmap->Restore();
			if (m_bBitmap)
			{
				if (m_paBitmapImage.size() >= (m_sBitmapImageData.dwHeight*m_sBitmapImageData.dwWidth * 4))
				{
					DDSURFACEDESC desc;
					ZeroMemory(&desc, sizeof(DDSURFACEDESC));
					desc.dwSize = sizeof(DDSURFACEDESC);
					if (SUCCEEDED(m_pBitmap->Lock(NULL, &desc, DDLOCK_WAIT, NULL)))
					{
						BYTE* pdwRow;

						for (DWORD y = 0; y < m_sBitmapImageData.dwHeight; y++)
						{
							// get a pointer to the next row
							pdwRow = ((BYTE*)desc.lpSurface) + y*desc.lPitch;

							// copy the scanline
							memcpy((void*)pdwRow, (void*)&m_paBitmapImage.data()[y*m_sBitmapImageData.dwWidth * 4], m_sBitmapImageData.dwWidth * 4);
						}

						// Unlock surface
						m_pBitmap->Unlock(NULL);
					}
				}
			}
		}
	}
	bool bFontLost = false;
	// ...and the font surfaces
	if (m_pFont_Aquilinus)
	{
		if (m_pFont_Aquilinus->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Aquilinus->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color1_0)
	{
		if (m_pFont_Color1_0->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color1_0->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color2_0)
	{
		if (m_pFont_Color2_0->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color2_0->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color3_0)
	{
		if (m_pFont_Color3_0->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color3_0->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color4_0)
	{
		if (m_pFont_Color4_0->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color4_0->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color1_1)
	{
		if (m_pFont_Color1_1->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color1_1->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color2_1)
	{
		if (m_pFont_Color2_1->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color2_1->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color3_1)
	{
		if (m_pFont_Color3_1->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color3_1->Restore();
			bFontLost = true;
		}
	}
	if (m_pFont_Color4_1)
	{
		if (m_pFont_Color4_1->IsLost() == DDERR_SURFACELOST)
		{
			m_pFont_Color4_1->Restore();
			bFontLost = true;
		}
	}

	// font lost ? reset color scheme
	if (bFontLost)
	{
		// load the font(s)
		m_hbFont_Aquilinus = LoadBitmap(m_hModule, MAKEINTRESOURCE(IMG_FONT01));
		if (m_hbFont_Aquilinus != NULL)
		{
			// get size of bitmap
			BITMAP bm;
			GetObject(m_hbFont_Aquilinus, sizeof(bm), &bm);

			HDC hdcImage = CreateCompatibleDC(NULL);
			HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, m_hbFont_Aquilinus);

			HDC hdc;
			if ((m_pFont_Aquilinus->GetDC(&hdc)) == DD_OK)
			{
				BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
				m_pFont_Aquilinus->ReleaseDC(hdc);
			}

			SelectObject(hdcImage, hbmOld);
			DeleteDC(hdcImage);
			DeleteObject(m_hbFont_Aquilinus);

			SetColorScheme(m_currentColorScheme);
		}
	}
}

/**
* Direct Draw clear.
***/
void AQU_DirectDraw::Clear(RECT rcDest, DWORD dwColor)
{
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);
	ddbfx.dwFillColor = dwColor;

	// set and blit the rectangle
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);
}

/**
* Direct Draw clear.
***/
void AQU_DirectDraw::Clear(RECT rcDest, AquilinusColor eColor)
{
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);

	// render the glyph
	switch (eColor)
	{
		case AquilinusColor::Color0:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor0;
			break;
		case AquilinusColor::Color1:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor1;
			break;
		case AquilinusColor::Color2:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor2;
			break;
		case AquilinusColor::Color3:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor3;
			break;
		case AquilinusColor::Color4:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor4;
			break;
	}

	// set and blit the rectangle
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);
}

/**
* Direct Draw clear.
***/
void AQU_DirectDraw::Clear(DWORD dwWindowWidth, DWORD dwWindowHeight)
{
	RECT    rcDest;
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);
	ddbfx.dwFillColor = m_currentColorScheme.dwColor0;

	// set and blit the rectangle
	SetRect(&rcDest, 0, 0, dwWindowWidth, dwWindowHeight);
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);

	// background to be drawn ?
	if (m_bBackground)
	{
		RECT rcSrc;
		SetRect(&rcSrc, 0, 0, m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
		if (rcSrc.right > (LONG)dwWindowWidth) rcSrc.right = (LONG)dwWindowWidth;
		if (rcSrc.bottom > (LONG)dwWindowHeight) rcSrc.bottom = (LONG)dwWindowHeight;
		SetRect(&rcDest, 0, 0, m_sBackgroundImageData.dwWidth, m_sBackgroundImageData.dwHeight);
		if (rcDest.right > (LONG)dwWindowWidth) rcDest.right = (LONG)dwWindowWidth;
		if (rcDest.bottom > (LONG)dwWindowHeight) rcDest.bottom = (LONG)dwWindowHeight;
		m_pDDSBack->Blt(&rcDest, m_pBackground, &rcSrc, DDBLT_WAIT, NULL);
	}
}

/**
* Direct Draw clear.
***/
void AQU_DirectDraw::Clear(DWORD dwWindowWidth, DWORD dwWindowHeight, AquilinusColor eColor)
{
	RECT    rcDest;
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);

	// render the glyph
	switch (eColor)
	{
		case AquilinusColor::Color0:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor0;
			break;
		case AquilinusColor::Color1:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor1;
			break;
		case AquilinusColor::Color2:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor2;
			break;
		case AquilinusColor::Color3:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor3;
			break;
		case AquilinusColor::Color4:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor4;
			break;
	}

	// set and blit the rectangle
	SetRect(&rcDest, 0, 0, dwWindowWidth, dwWindowHeight);
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);
}

/**
* Direct Draw present.
* @param hwnd Handle to the main window.
***/
HRESULT AQU_DirectDraw::Present(HWND hwnd, DWORD dwWindowWidth, DWORD dwWindowHeight)
{
	RECT    rcSrc;
	RECT    rcDest;
	POINT   p;

	// check back buffer
	if (m_pDDS == NULL)
		return E_FAIL;

	// get primary surface window position
	p.x = 0; p.y = 0;
	::ClientToScreen(hwnd, &p);
	::GetClientRect(hwnd, &rcDest);

	// set source, destination rectangles, copy backbuffer to viewport
	OffsetRect(&rcDest, p.x, p.y);
	SetRect(&rcSrc, 0, 0, dwWindowWidth, dwWindowHeight);
	return m_pDDS->Blt(&rcDest, m_pDDSBack, &rcSrc, DDBLT_WAIT, NULL);
}

/***
* Release all DirectDraw stuff.
***/
void AQU_DirectDraw::Release()
{
	if (m_pBackground != NULL)
	{
		m_pBackground->Release();
		m_pBackground = NULL;
	}
	if (m_pBitmap != NULL)
	{
		m_pBitmap->Release();
		m_pBitmap = NULL;
	}
	if (m_pFont_Aquilinus != NULL)
	{
		m_pFont_Aquilinus->Release();
		m_pFont_Aquilinus = NULL;
	}
	if (!m_bNoFontSurfaceRelease)
	{
		if (m_pFont_Color1_0 != NULL)
		{
			m_pFont_Color1_0->Release();
			m_pFont_Color1_0 = NULL;
		}
		if (m_pFont_Color2_0 != NULL)
		{
			m_pFont_Color2_0->Release();
			m_pFont_Color2_0 = NULL;
		}
		if (m_pFont_Color3_0 != NULL)
		{
			m_pFont_Color3_0->Release();
			m_pFont_Color3_0 = NULL;
		}
		if (m_pFont_Color4_0 != NULL)
		{
			m_pFont_Color4_0->Release();
			m_pFont_Color4_0 = NULL;
		}
		if (m_pFont_Color1_1 != NULL)
		{
			m_pFont_Color1_1->Release();
			m_pFont_Color1_1 = NULL;
		}
		if (m_pFont_Color2_1 != NULL)
		{
			m_pFont_Color2_1->Release();
			m_pFont_Color2_1 = NULL;
		}
		if (m_pFont_Color3_1 != NULL)
		{
			m_pFont_Color3_1->Release();
			m_pFont_Color3_1 = NULL;
		}
		if (m_pFont_Color4_1 != NULL)
		{
			m_pFont_Color4_1->Release();
			m_pFont_Color4_1 = NULL;
		}
	}
	if (m_pDDSBack != NULL)
	{
		m_pDDSBack->Release();
		m_pDDSBack = NULL;
	}
	if (m_pDDS != NULL)
	{
		m_pDDS->Release();
		m_pDDS = NULL;
	}
	if (m_pClipper != NULL)
	{
		m_pClipper->Release();
		m_pClipper = NULL;
	}
	if (m_pDD != NULL)
	{
		m_pDD->Release();
		m_pDD = NULL;
	}
}

/**
* Creates an offscreen plain surface.
* @param dwX Width, in Pixels.
* @param dwY Height, in Pixels.
***/
LPDIRECTDRAWSURFACE AQU_DirectDraw::CreateOffScreenPlainSurface(DWORD dwX, DWORD dwY)
{
	DDSURFACEDESC ddsd;
	IDirectDrawSurface *pdds;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = dwX;
	ddsd.dwHeight = dwY;

	if (m_pDD->CreateSurface(&ddsd, &pdds, NULL) != S_OK)
	{
		return NULL;
	}
	else
	{
		return pdds;
	}
}

/**
* Get the number of currently installed color schemes.
* @returns The number of currently installed color schemes.
***/
DWORD AQU_DirectDraw::GetColorSchemesNumber()
{
	return AQUILINUS_NUMBER_OF_COLOR_SCHEMES;
}

/**
* Set a color scheme by index.
* @param dwIndex The index of the color scheme.
***/
void AQU_DirectDraw::SetColorScheme(DWORD dwIndex)
{
	if (dwIndex >= AQUILINUS_NUMBER_OF_COLOR_SCHEMES)
		return SetColorScheme(g_colorSchemes[0]);

	return SetColorScheme(g_colorSchemes[dwIndex]);
}

/**
* Set a color scheme from another AQU_DirectDraw object.
* Speeds up color selection.
***/
void AQU_DirectDraw::SetColorScheme(AQU_DrawingAPI *pDirectDraw)
{
	// set the current color scheme
	memcpy(&m_currentColorScheme, &((AQU_DirectDraw*)pDirectDraw)->m_currentColorScheme, sizeof(ColorScheme));

	m_pFont_Color1_0 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color1_0;
	m_pFont_Color2_0 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color2_0;
	m_pFont_Color3_0 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color3_0;
	m_pFont_Color4_0 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color4_0;
	m_pFont_Color1_1 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color1_1;
	m_pFont_Color2_1 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color2_1;
	m_pFont_Color3_1 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color3_1;
	m_pFont_Color4_1 = ((AQU_DirectDraw*)pDirectDraw)->m_pFont_Color4_1;

	// for some reason ->AddRef() does not work here,
	// so we set this bool to true to NOT RELEASE
	// the surfaces at shutdown
	m_bNoFontSurfaceRelease = true;
}

/**
* Set a color scheme.
* @param sColorScheme The color scheme to be instantly used by all Direct Draw methods.
***/
void AQU_DirectDraw::SetColorScheme(ColorScheme &sColorScheme)
{
	// set the current color scheme
	memcpy(&m_currentColorScheme, &sColorScheme, sizeof(ColorScheme));

	// copy the base font surface to the color font surfaces
	m_pFont_Color1_0->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color2_0->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color3_0->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color4_0->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color1_1->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color2_1->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color3_1->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);
	m_pFont_Color4_1->Blt(NULL, m_pFont_Aquilinus, NULL, DDBLT_WAIT, NULL);

	// color the font surfaces
	ColorSurface(sColorScheme.dwColor1, sColorScheme.dwColor0, m_pFont_Color1_0);
	ColorSurface(sColorScheme.dwColor2, sColorScheme.dwColor0, m_pFont_Color2_0);
	ColorSurface(sColorScheme.dwColor3, sColorScheme.dwColor0, m_pFont_Color3_0);
	ColorSurface(sColorScheme.dwColor4, sColorScheme.dwColor0, m_pFont_Color4_0);
	ColorSurface(sColorScheme.dwColor0, sColorScheme.dwColor1, m_pFont_Color1_1); /**< exception : dwColor0 !! **/
	ColorSurface(sColorScheme.dwColor2, sColorScheme.dwColor1, m_pFont_Color2_1);
	ColorSurface(sColorScheme.dwColor3, sColorScheme.dwColor1, m_pFont_Color3_1);
	ColorSurface(sColorScheme.dwColor4, sColorScheme.dwColor1, m_pFont_Color4_1);
}

/**
* Get a color scheme.
***/
void AQU_DirectDraw::GetColorScheme(ColorScheme &sColorScheme, DWORD dwIndex)
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

/**
* Colors a surface to the desired colors (fore-and background).
* Surface front color must be white, background black.
***/
HRESULT AQU_DirectDraw::ColorSurface(DWORD dwColorFore, DWORD dwColorBack, LPDIRECTDRAWSURFACE pSurface)
{
	// get foreground color
	DWORD dwSrcRed = (dwColorFore >> 16);
	DWORD dwSrcGreen = (dwColorFore & 0xff00) >> 8;
	DWORD dwSrcBlue = (dwColorFore & 0xff);
	// get background color
	DWORD dwBckRed = (dwColorBack >> 16);
	DWORD dwBckGreen = (dwColorBack & 0xff00) >> 8;
	DWORD dwBckBlue = (dwColorBack & 0xff);

	DWORD dwAlpha, dwRed, dwGreen, dwBlue, dwPixel, dwGrey, dwGreyCont;
	DWORD *pdwRow = nullptr;

	DDSURFACEDESC desc;
	ZeroMemory(&desc, sizeof(DDSURFACEDESC));
	desc.dwSize = sizeof(DDSURFACEDESC);
	if (SUCCEEDED(pSurface->Lock(NULL, &desc, DDLOCK_WAIT, NULL)))
	{
		// set lock boolean to true
		m_bSurfaceLocked = true;

		if (desc.ddpfPixelFormat.dwRGBBitCount >= 24)
		{
			for (DWORD y = 0; y < desc.dwHeight; y++)
			{
				// get a pointer to the next row
				pdwRow = ((DWORD*)desc.lpSurface) + ((y*desc.lPitch) / sizeof(DWORD));

				for (DWORD x = 0; x < desc.dwWidth; x++)
				{
					// get the pixel
					dwPixel = *pdwRow;

					// color it by the source color, set full alpha
					dwAlpha = 0xff000000;

					// back color for black
					if ((dwPixel & 0xffffff) == 0x000000)
					{
						dwRed = dwBckRed;
						dwGreen = dwBckGreen;
						dwBlue = dwBckBlue;
					}
					// interpolated color for non-white and non-black
					else if ((dwPixel & 0xffffff) < 0xffffff)
					{
						dwGrey = (((dwPixel & 0xff0000) >> 16) + ((dwPixel & 0xff00) >> 8) + (dwPixel & 0xff)) / 3;
						dwGrey &= 0xff;
						dwGreyCont = 0xff - dwGrey;

						dwRed = ((dwSrcRed*dwGrey) + (dwBckRed*dwGreyCont)) >> 8;
						dwGreen = ((dwSrcGreen*dwGrey) + (+dwBckGreen*dwGreyCont)) >> 8;
						dwBlue = ((dwSrcBlue*dwGrey) + (+dwBckBlue*dwGreyCont)) >> 8;
					}
					else
					{
						// source color for white
						dwRed = dwSrcRed;
						dwGreen = dwSrcGreen;
						dwBlue = dwSrcBlue;
					}

					// and set it
					*pdwRow = dwAlpha + RGB_DD((BYTE)dwRed, (BYTE)dwGreen, (BYTE)dwBlue);

					pdwRow++;
				}
			}
		}
		pSurface->Unlock(NULL);
		m_bSurfaceLocked = false;
	}
	return S_OK;
}

/**
* Resizes the back buffer.
***/
HRESULT AQU_DirectDraw::ResizeBackbuffer(DWORD dwWindowWidth, DWORD dwWindowHeight)
{
	if (m_pDDSBack) m_pDDSBack->Release();

	// create ddraw back buffer
	memset(&m_ddsd, 0, sizeof(m_ddsd));
	m_ddsd.dwSize = sizeof(m_ddsd);
	m_ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	m_ddsd.dwWidth = dwWindowWidth;
	m_ddsd.dwHeight = dwWindowHeight;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	if (FAILED(m_pDD->CreateSurface(&m_ddsd, &m_pDDSBack, NULL)))
	{
		OutputDebugString(L"Aquilinus : Failed to create Direct Draw back buffer surface!");
		return E_FAIL;
	}

	return S_OK;
}

/**
* Loads an additional bitmap with screen coordinates.
***/
HRESULT AQU_DirectDraw::LoadAdditionalBitmap(LPWSTR szPNG, DWORD dwForceX, DWORD dwForceY)
{
	// load a test background
	PNG_ImageData sImageData;

	// if the path is empty use the picture data instead
	HRESULT hr = DecodePNGFile(szPNG, nullptr, 0, sImageData, m_paBitmapImage);

	// delete old bitmap if there
	if (m_pBitmap) m_pBitmap->Release();
	m_bBitmap = false;
	m_pBitmap = NULL;

	// test the force parameters
	if ((dwForceX > 0) && (dwForceY > 0))
	{
		if ((dwForceX != sImageData.dwWidth) || (dwForceY != sImageData.dwHeight))
		{
			m_paBitmapImage.clear();
			return E_FAIL;
		}
	}

	if (SUCCEEDED(hr))
	{
		// copy to direct draw structure
		memcpy((void*)&m_sBitmapImageData, (void*)&sImageData, sizeof(PNG_ImageData));

		// create Bitmap surface
		m_pBitmap = CreateOffScreenPlainSurface(m_sBitmapImageData.dwWidth, m_sBitmapImageData.dwHeight);
		DDSURFACEDESC desc;
		ZeroMemory(&desc, sizeof(DDSURFACEDESC));
		desc.dwSize = sizeof(DDSURFACEDESC);
		if (SUCCEEDED(m_pBitmap->Lock(NULL, &desc, DDLOCK_WAIT, NULL)))
		{
			BYTE* pdwRow;

			for (DWORD y = 0; y < m_sBitmapImageData.dwHeight; y++)
			{
				// get a pointer to the next row
				pdwRow = ((BYTE*)desc.lpSurface) + y*desc.lPitch;

				// copy the scanline
				memcpy((void*)pdwRow, (void*)&m_paBitmapImage.data()[y*m_sBitmapImageData.dwWidth * 4], m_sBitmapImageData.dwWidth * 4);
			}

			// Unlock surface
			m_pBitmap->Unlock(NULL);

			// set bitmap bool
			m_bBitmap = true;
		}
		else
		{
			m_paBitmapImage.clear();
			return E_FAIL;
		}
	}
	else return hr;

	return S_OK;
}

/**
* Release the additional bitmap.
***/
void AQU_DirectDraw::ReleaseAdditionalBitmap()
{
	// delete old bitmap
	if (m_bBitmap)
	{
		if (m_pBitmap)
		{
			m_pBitmap->Release();
			m_pBitmap = nullptr;
		}
	}
	if (m_bBackground)
	{
		if (m_pBackground)
		{
			m_pBackground->Release();
			m_pBackground = nullptr;
		}
	}
	m_bBitmap = false;
	m_bBackground = false;
	m_pBitmap = NULL;
	m_pBackground = NULL;
	m_paBitmapImage = std::vector<BYTE>();
	m_paBackgroundImage = std::vector<BYTE>();
}

/**
* Set the maximum X position for text.
* The last space before this position will be filled with >...<.
* @param lMax Maximum X position of the text, set -1 to take the window width and 0 for no limit.
***/
void AQU_DirectDraw::SetMaxXTextOutput(LONG lMax)
{
	if (lMax == -1)
		m_lMaxXTextOutput = (LONG)m_ddsd.dwWidth;
	else if (lMax <= 0)
		m_lMaxXTextOutput = 0;
	else
		m_lMaxXTextOutput = lMax;
}

/**
* Get the index of the character in the font table.
***/
DWORD AQU_DirectDraw::GetGlyphIndex(char character)
{

	DWORD d;
	for (d = 0; d < g_dwFontGlyphesNum; d++)
	{
		if (character == g_fontGlyphes[d].character)
			return d;
	}

	return 0;
}

/**
* Get the width of the text.
* @param [in] szText The text to be measured.
* @param [in] fSize The text zoom.
* @returns The width of the text in pixel space.
***/
DWORD AQU_DirectDraw::GetTextWidth(LPCWSTR szText, float fSize, float fZoom)
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
* Renders a glyph using the main font table by the specified index.
* @param [in] index The index of the glyph.
* @param [out] rcDest The destination rectangle created by the method.
* @param [in] nX The destination X origin.
* @param [in] nY The destination Y origin.
* @param [in] eColor The Text color.
* @param [in] eBackground The Background color, currently only Color0 and Color1 are supported.
* @param [in] fSize The size of the text (1.0f = original size).
***/
HRESULT AQU_DirectDraw::RenderGlyph(DWORD dwIndex, RECT &rcDest, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom)
{
	RECT rcSrc;

	// check back buffer
	if (m_pDDSBack == NULL)
		return E_FAIL;

	// set source, destination rectangles
	// right/bottom currently is width/height from default
	// to make calculation of the destination rectangle 
	// easier
	memcpy(&rcSrc, &g_fontGlyphes[dwIndex].rcSrc, sizeof(RECT));

	// set size (only if < 1.0f to avoid zoom in)
	if (fSize < 1.0f)
	{
		// zoom out the glyph
		DWORD top = static_cast<DWORD>(float(rcSrc.top)*fSize);
		DWORD right = static_cast<DWORD>(float(rcSrc.right)*fSize);
		DWORD height = static_cast<DWORD>(float(rcSrc.top + rcSrc.bottom)*fSize);

		// avoid fail
		if (right < 1) right = 1;
		if (height <= top) height = top + 1;

		SetRect(&rcDest, nX, nY + top, nX + right, nY + height);
	}
	else
		SetRect(&rcDest, nX, nY + rcSrc.top, nX + rcSrc.right, nY + rcSrc.top + rcSrc.bottom);


	// add source top/left to width and height of the glyph
	rcSrc.right += rcSrc.left;
	rcSrc.bottom += rcSrc.top;

	// glyph out of range ? 
	if (rcDest.left<0)
	{
		rcSrc.left -= rcDest.left;
		rcDest.left = 0;
		if (rcSrc.left>rcSrc.right) return S_OK;
	}
	if (rcDest.top<0)
	{
		rcSrc.top -= rcDest.top;
		rcDest.top = 0;
		if (rcSrc.top>rcSrc.bottom) return S_OK;
	}

	// set zoom, add origin
	RECT rcD;
	SetRect(&rcD, (LONG)(((float)rcDest.left) * fZoom) + vecOrigin.x,
		(LONG)(((float)rcDest.top) * fZoom) + vecOrigin.y,
		(LONG)(((float)rcDest.right) * fZoom) + vecOrigin.x,
		(LONG)(((float)rcDest.bottom) * fZoom) + vecOrigin.y);

	// avoid fail
	if (rcDest.right < (rcDest.left + 2)) rcDest.right = rcDest.left + 2;
	if (rcDest.bottom < (rcDest.top + 2)) rcDest.bottom = rcDest.top + 2;

	HRESULT hr = E_FAIL;

	// render the glyph
	switch (eBackground)
	{
		case AquilinusColor::Color0:
			switch (eColor)
			{
				case AquilinusColor::Color0:
					hr = E_FAIL;
					break;
				case AquilinusColor::Color1:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color1_0, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color2:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color2_0, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color3:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color3_0, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color4:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color4_0, &rcSrc, DDBLT_WAIT, NULL);
					break;
			}
			break;
		case AquilinusColor::Color1:
			switch (eColor)
			{
				case AquilinusColor::Color0:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color1_1, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color1:
					hr = E_FAIL;
					break;
				case AquilinusColor::Color2:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color2_1, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color3:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color3_1, &rcSrc, DDBLT_WAIT, NULL);
					break;
				case AquilinusColor::Color4:
					hr = m_pDDSBack->Blt(&rcD, m_pFont_Color4_1, &rcSrc, DDBLT_WAIT, NULL);
					break;
			}
			break;
	}

	// set new spacing
	if (fSize < 1.0f)
		rcDest.right += (LONG)(float(g_fontGlyphes[dwIndex].nAddSpacing)*fSize);
	else
		rcDest.right += g_fontGlyphes[dwIndex].nAddSpacing;

	return hr;
}

/**
* Outputs text to the backbuffer.
* @param [in] text The text to render.
* @param [in] nX The destination X origin.
* @param [in] nY The destination Y origin.
***/
void AQU_DirectDraw::RenderText(LPCWSTR szText, LONG nX, LONG nY, POINT vecOrigin, AquilinusColor eColor, AquilinusColor eBackground, float fSize, float fZoom)
{
	RECT rcDst;
	DWORD dwIndex = 0;

	if (eColor == AquilinusColor::Color0) return;

	// get three points width
	DWORD dwThreePointsWidth = GetTextWidth(L"...", fSize, fZoom);
	DWORD dwPointWidth = dwThreePointsWidth / 3;
	LONG lMax = (LONG)((float)m_lMaxXTextOutput * fZoom);
	if (m_lMaxXTextOutput <= 0) lMax = 0;

	ZeroMemory(&rcDst, sizeof(RECT));
	rcDst.right = nX;
	for (dwIndex = 0; dwIndex < (DWORD)wcslen(szText); dwIndex++)
	{
		// near the right border ? draw three points, exit
		if ((rcDst.right >= lMax) && (lMax > 0)) dwIndex = (DWORD)wcslen(szText);
		else if ((rcDst.right > lMax - (LONG)(dwThreePointsWidth * 2)) && (lMax > 0))
		{
			if (rcDst.right < m_lMaxXTextOutput - (LONG)dwPointWidth) RenderGlyph(GetGlyphIndex((char)'.'), rcDst, rcDst.right, nY, vecOrigin, eColor, eBackground, fSize, fZoom); rcDst.right += 2;
			if (rcDst.right < m_lMaxXTextOutput - (LONG)dwPointWidth) RenderGlyph(GetGlyphIndex((char)'.'), rcDst, rcDst.right, nY, vecOrigin, eColor, eBackground, fSize, fZoom); rcDst.right += 2;
			if (rcDst.right < m_lMaxXTextOutput - (LONG)dwPointWidth) RenderGlyph(GetGlyphIndex((char)'.'), rcDst, rcDst.right, nY, vecOrigin, eColor, eBackground, fSize, fZoom); rcDst.right += 2;
			dwIndex = (DWORD)wcslen(szText);
		}
		else
		{
			RenderGlyph(GetGlyphIndex((char)szText[dwIndex]), rcDst, rcDst.right, nY, vecOrigin, eColor, eBackground, fSize, fZoom);

			// space between characters == 2
			rcDst.right += 2;
		}
	}
}

/**
* Draws a line.
* @param [in] vecStart The starting point in pixel space.
* @param [in] vecEnd The end point in pixel space.
* @param [in] nSize The size of the line, in pixel space.
* @param [in] eColor The color of the edge line, taken from the current color scheme.
* @param [in] fZoom The current zoom factor.
***/
void AQU_DirectDraw::DrawLine(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
{
	// get the distances
	LONG lDistanceY, lDistanceX;
	lDistanceX = vecEnd.x - vecStart.x;
	lDistanceY = vecEnd.y - vecStart.y;

	// get floating fields
	float fX = (float)vecStart.x;
	float fY = (float)vecStart.y;

	// x distance smaller than y distance ?
	if (abs(lDistanceX) < abs(lDistanceY))
	{
		for (int i = 0; i < abs(lDistanceX); i++)
		{
			// add breaking distance
			fX += (vecStart.x < vecEnd.x) ? 1.0f : -1.0f;
			fY += float(lDistanceY) / float(abs(lDistanceX));

			if (lDistanceY < 0)
			{
				vecStart.y = (LONG)fY;
				DrawVLine(vecStart, vecOrigin, abs(lDistanceY) / abs(lDistanceX) + 1, 4, eColor, fZoom);
			}
			else
			{
				DrawVLine(vecStart, vecOrigin, lDistanceY / abs(lDistanceX) + 1, 4, eColor, fZoom);
				vecStart.y = (LONG)fY;
			}

			vecStart.x = (LONG)fX;
		}
	}
	else
	{
		for (int i = 0; i < abs(lDistanceY); i++)
		{
			// add breaking distance
			fX += float(lDistanceX) / float(abs(lDistanceY));
			fY += (vecStart.y < vecEnd.y) ? 1.0f : -1.0f;

			if (lDistanceX < 0)
			{
				vecStart.x = (LONG)fX;
				DrawHLine(vecStart, vecOrigin, abs(lDistanceX) / abs(lDistanceY) + 1, 4, eColor, fZoom);
			}
			else
			{
				DrawHLine(vecStart, vecOrigin, lDistanceX / abs(lDistanceY) + 1, 4, eColor, fZoom);
				vecStart.x = (LONG)fX;
			}

			vecStart.y = (LONG)fY;
		}
	}
}

/**
* Draws a horizontal line.
* @param [in] vecStart The starting vector (point) in pixel coords.
* @param [in] nLength The length in pixel.
* @param [in] nSize The size of the line, in pixel.
* @param [in] eColor The color from the aquilinus color scheme.
***/
void AQU_DirectDraw::DrawHLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
{
	RECT    rcDest;
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);
	switch (eColor)
	{
		case AquilinusColor::Color0:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor0;
			break;
		case AquilinusColor::Color1:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor1;
			break;
		case AquilinusColor::Color2:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor2;
			break;
		case AquilinusColor::Color3:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor3;
			break;
		case AquilinusColor::Color4:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor4;
			break;
	}

	// set and blit the rectangle
	SetRect(&rcDest, vecStart.x, vecStart.y, vecStart.x + nLength, vecStart.y + nSize);

	// set zoom, add origin clamp
	rcDest.left = (LONG)((float)rcDest.left * fZoom) + vecOrigin.x;
	rcDest.right = (LONG)((float)rcDest.right * fZoom) + vecOrigin.x;
	rcDest.top = (LONG)((float)rcDest.top * fZoom) + vecOrigin.y;
	rcDest.bottom = (LONG)((float)rcDest.bottom * fZoom) + vecOrigin.y;
	if (rcDest.left < 0) rcDest.left = 0;
	if (rcDest.top < 0) rcDest.top = 0;
	if (rcDest.right >(LONG)m_ddsd.dwWidth) rcDest.right = (LONG)m_ddsd.dwWidth;
	if (rcDest.bottom >(LONG)m_ddsd.dwHeight) rcDest.bottom = (LONG)m_ddsd.dwHeight;
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);
}

/**
* Draws a vertical line.
* @param [in] vecStart The starting vector (point) in pixel coords.
* @param [in] nLength The length in pixel.
* @param [in] nSize The size of the line, in pixel.
* @param [in] eColor The color from the aquilinus color scheme.
***/
void AQU_DirectDraw::DrawVLine(POINT vecStart, POINT vecOrigin, LONG nLength, LONG nSize, AquilinusColor eColor, float fZoom)
{
	RECT    rcDest;
	DDBLTFX ddbfx;

	// check back buffer
	if (m_pDDSBack == NULL)
		return;

	// set the pixel color
	ddbfx.dwSize = sizeof(ddbfx);
	switch (eColor)
	{
		case AquilinusColor::Color0:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor0;
			break;
		case AquilinusColor::Color1:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor1;
			break;
		case AquilinusColor::Color2:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor2;
			break;
		case AquilinusColor::Color3:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor3;
			break;
		case AquilinusColor::Color4:
			ddbfx.dwFillColor = m_currentColorScheme.dwColor4;
			break;
	}

	// set and blit the rectangle
	SetRect(&rcDest, vecStart.x, vecStart.y, vecStart.x + nSize, vecStart.y + nLength);

	// set zoom, add origin clamp
	rcDest.left = (LONG)((float)rcDest.left * fZoom) + vecOrigin.x;
	rcDest.right = (LONG)((float)rcDest.right * fZoom) + vecOrigin.x;
	rcDest.top = (LONG)((float)rcDest.top * fZoom) + vecOrigin.y;
	rcDest.bottom = (LONG)((float)rcDest.bottom * fZoom) + vecOrigin.y;
	if (rcDest.left < 0) rcDest.left = 0;
	if (rcDest.top < 0) rcDest.top = 0;
	if (rcDest.right >(LONG)m_ddsd.dwWidth) rcDest.right = (LONG)m_ddsd.dwWidth;
	if (rcDest.bottom >(LONG)m_ddsd.dwHeight) rcDest.bottom = (LONG)m_ddsd.dwHeight;
	m_pDDSBack->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx);
}

/**
* Draws a pipeline, starting at a right side, ending at a left side (of a node).
* @param [in] vecStart The starting point in pixel space.
* @param [in] vecEnd The end point in pixel space.
* @param [in] nSize The size of the line, in pixel space.
* @param [in] eColor The color of the edge line, taken from the current color scheme.
* @param [in] fZoom The current zoom factor.
***/
void AQU_DirectDraw::DrawPipeline(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
{
	// draw first left source connector
	DrawHLine(vecStart, vecOrigin, 10, 4, eColor, fZoom);
	vecStart.x += 10;

	// draw to the half height difference
	LONG lDistanceY;
	if (vecStart.y > vecEnd.y)
	{
		lDistanceY = (vecStart.y - vecEnd.y) >> 1;
		vecStart.y -= lDistanceY - 4;
		DrawVLine(vecStart, vecOrigin, lDistanceY, 4, eColor, fZoom);
		vecStart.y -= 4;
	}
	else if (vecStart.y < vecEnd.y)
	{
		lDistanceY = (vecEnd.y - vecStart.y) >> 1;
		DrawVLine(vecStart, vecOrigin, lDistanceY, 4, eColor, fZoom);
		vecStart.y += lDistanceY;
	}

	// draw to the X axis destination connector
	LONG lDistanceX = vecEnd.x - 10 - vecStart.x;
	if (lDistanceX < -4)
	{
		vecStart.x += lDistanceX + 4;
		DrawHLine(vecStart, vecOrigin, -lDistanceX, 4, eColor, fZoom);
		vecStart.x -= 4;
	}
	else if (lDistanceX > 4)
	{
		DrawHLine(vecStart, vecOrigin, lDistanceX, 4, eColor, fZoom);
		vecStart.x += lDistanceX;
	}

	// draw to the full height difference
	lDistanceY += 4;
	if (vecStart.y > vecEnd.y)
	{
		vecStart.y -= lDistanceY - 4;
		DrawVLine(vecStart, vecOrigin, lDistanceY, 4, eColor, fZoom);
		vecStart.y -= 4;
	}
	else if (vecStart.y < vecEnd.y)
	{
		DrawVLine(vecStart, vecOrigin, lDistanceY, 4, eColor, fZoom);
		vecStart.y += lDistanceY;
	}

	// draw first left source connector
	vecStart.y = vecEnd.y;
	DrawHLine(vecStart, vecOrigin, vecEnd.x - vecStart.x, 4, eColor, fZoom);
}

/**
* Draws an edge, left/right to up/down or up/down to left/right.
* @param [in] vecStart The starting point in pixel space.
* @param [in] vecEnd The end point in pixel space.
* @param [in] nSize The size of the line, in pixel space.
* @param [in] eColor The color of the edge line, taken from the current color scheme.
* @param [in] bUpOrLeft True if plug is from left/right to up/down plug, false if viceaversa.
* Depending on the plug up or down the entry point is the
* left point seen from inside.
* For a down plug this means the upper left pixel,
* for an up plug this means the lower right pixel.
* For left or right plugs it is always the top pixel.
***/
void AQU_DirectDraw::DrawEdge(POINT vecStart, POINT vecEnd, POINT vecOrigin, LONG nSize, AquilinusColor eColor, bool bUpOrLeft, float fZoom)
{
	// get float boundaries
	float fLeft = (float)vecStart.x;
	float fTop = (float)vecStart.y;
	float fRight = (float)vecEnd.x;
	float fBottom = (float)vecEnd.y;

	// get x length
	float fLengthX = fLeft - fRight;
	if (fRight > fLeft) fLengthX = fRight - fLeft;

	// get y length
	float fLengthY = fTop - fBottom;
	if (fBottom > fTop) fLengthY = fBottom - fTop;

	// get x y runners and divider
	float fStepX = 0.0f;
	float fStepY = 0.0f;
	float fDiv = 3.0f;

	// up or left plug ?
	if (bUpOrLeft)
	{
		// first, set the x runner to full length
		fStepX = fRight - fLeft;
		bool bForward = true;
		if (fStepX < 0.0f) bForward = false;

		// loop through, adding 1.0f (= 1 pixel) per line
		for (fStepY = 0.0f; fStepY < (fLengthY - (float)(nSize - 1)); fStepY += 1.0f)
		{
			POINT pt;
			pt.x = (LONG)fLeft;
			pt.y = (LONG)fTop;

			// the length is a divided part of the remaining length
			LONG n = (LONG)(fStepX / fDiv);
			if (!bForward) n *= -1;

			fLeft += fStepX / fDiv;
			fStepX -= fStepX / fDiv;

			// if smaller than double line width, compute a round falling line
			if (n < (nSize * 2))
			{
				if (n < nSize)
					n = nSize;
				fDiv *= 0.95f;
				if (fDiv < 1.0f) fDiv = 1.0f;
			}

			// line moves up or down ?
			if (fBottom < fTop) fTop -= 1.0f; else fTop += 1.0f;

			if (!bForward)
			{
				pt.x -= n + 1;
				DrawHLine(pt, vecOrigin, n + 1, nSize, eColor, fZoom);
			}
			else
				DrawHLine(pt, vecOrigin, n + 1, nSize, eColor, fZoom);
		}
	}
	else
	{
		// first, set the Y runner to full length
		fStepY = fBottom - fTop;
		bool bForward = true;
		if (fStepY < 0.0f) bForward = false;

		// loop through, adding 1.0f (= 1 pixel) per line
		for (fStepX = 0.0f; fStepX < (fLengthX - (float)(nSize - 1)); fStepX += 1.0f)
		{
			POINT pt;
			pt.x = (LONG)fLeft;
			pt.y = (LONG)fTop;

			// the length is a divided part of the remaining length
			LONG n = (LONG)(fStepY / fDiv);
			if (!bForward) n *= -1;

			fTop += fStepY / fDiv;
			fStepY -= fStepY / fDiv;

			// if smaller than double line width, compute a round falling line
			if (n < (nSize * 2))
			{
				if (n < nSize)
					n = nSize;
				fDiv *= 0.95f;
				if (fDiv < 1.0f) fDiv = 1.0f;
			}

			// line moves left or right ?
			if (fRight < fLeft) fLeft -= 1.0f; else fLeft += 1.0f;

			// and draw
			if (!bForward)
			{
				pt.y -= n + 1;
				DrawHLine(pt, vecOrigin, n + 1, nSize, eColor, fZoom);
			}
			else
				DrawVLine(pt, vecOrigin, n + 1, nSize, eColor, fZoom);
		}
	}
}

/**
* Draws a Box.
* @param [in] rcBox The pixel coordinates of the box.
* @param [in] nSize The size of the line, in pixel.
* @param [in] eColor The color from the aquilinus color scheme.
***/
void AQU_DirectDraw::DrawBox(RECT rcBox, POINT vecOrigin, LONG nSize, AquilinusColor eColor, float fZoom)
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
* Draws an external surface.
***/
void AQU_DirectDraw::DrawSurface(RECT rcDst, RECT rcSrc, POINT vecOrigin, LPDIRECTDRAWSURFACE pSurface, float fZoom)
{
	// create destination rectangle
	RECT rcDest;
	rcDest.left = (LONG)((float)rcDst.left * fZoom) + vecOrigin.x;
	rcDest.right = (LONG)((float)rcDst.right * fZoom) + vecOrigin.x;
	rcDest.top = (LONG)((float)rcDst.top * fZoom) + vecOrigin.y;
	rcDest.bottom = (LONG)((float)rcDst.bottom * fZoom) + vecOrigin.y;

	// create new source rectangle in case we need to clamp
	RECT rcSource;
	CopyMemory(&rcSource, &rcSrc, sizeof(RECT));

	// and clamp.. first left
	if (rcDest.left < 0)
	{
		float fClampFactor = (float)(rcDest.left*-1) / ((float)rcDest.right - (float)rcDest.left);
		float fSourceClamp = fClampFactor *  ((float)rcSource.right - (float)rcSource.left);
		rcSource.left += (int)fSourceClamp;
		rcDest.left = 0;
	}
	// now top 
	if (rcDest.top < 0)
	{
		float fClampFactor = (float)(rcDest.top*-1) / ((float)rcDest.bottom - (float)rcDest.top);
		float fSourceClamp = fClampFactor *  ((float)rcSource.bottom - (float)rcSource.top);
		rcSource.top += (int)fSourceClamp;
		rcDest.top = 0;
	}
	// now right
	if (rcDest.right >(LONG)m_ddsd.dwWidth)
	{
		float fClampFactor = (float)(rcDest.right - (LONG)m_ddsd.dwWidth) / ((float)rcDest.right - (float)rcDest.left);
		float fSourceClamp = fClampFactor *  ((float)rcSource.right - (float)rcSource.left);
		rcSource.right -= (int)fSourceClamp;
		rcDest.right = (LONG)m_ddsd.dwWidth;
	}
	// and bottom
	if (rcDest.bottom > (LONG)m_ddsd.dwHeight)
	{
		float fClampFactor = (float)(rcDest.bottom - (LONG)m_ddsd.dwHeight) / ((float)rcDest.bottom - (float)rcDest.top);
		float fSourceClamp = fClampFactor *  ((float)rcSource.bottom - (float)rcSource.top);
		rcSource.bottom -= (int)fSourceClamp;
		rcDest.bottom = (LONG)m_ddsd.dwHeight;
	}

	m_pDDSBack->Blt(&rcDest, pSurface, &rcSource, DDBLT_WAIT, NULL);
}

/**
* Draws the additional bitmap stored in a surface.
***/
void AQU_DirectDraw::DrawAdditionalBitmap(DWORD dwX, DWORD dwY)
{
	if (m_bBitmap)
	{
		RECT rc;
		SetRect(&rc, (int)dwX, (int)dwY, (int)dwX + m_sBitmapImageData.dwWidth, (int)dwY + m_sBitmapImageData.dwHeight);
		m_pDDSBack->Blt(&rc, m_pBitmap, NULL, DDBLT_WAIT, NULL);
	}
}