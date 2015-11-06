/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter.cpp> and
Class <VireioStereoSplitter> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from Vireio source code originally 
authored by Chris Drain (v1.1.x 2013).

The stub class <AQU_Nodus> is the only public class from the Aquilinus 
repository and permitted to be used for open source plugins of any kind. 
Read the Aquilinus documentation for further information.

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

#include"VireioStereoSplitter.h"

#define INTERFACE_IDIRECT3DDEVICE9                           8
#define INTERFACE_IDIRECT3DSTATEBLOCK9                      13
#define INTERFACE_IDIRECT3DSWAPCHAIN9                       15
#define	METHOD_IDIRECT3DDEVICE9_PRESENT                     17
#define METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET             37
#define METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE      39 
#define METHOD_IDIRECT3DDEVICE9_CLEAR                       43
#define METHOD_IDIRECT3DDEVICE9_SETTEXTURE                  65
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE               81
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE        82 
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP             83 
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP      84 
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT                   3
#define METHOD_IDIRECT3DSTATEBLOCK9_APPLY                    5

/**
* Constructor.
***/
StereoSplitter::StereoSplitter():AQU_Nodus(),
	m_apcActiveRenderTargets(0, nullptr),
	m_apcActiveTextures(MAX_SIMULTANEOUS_TEXTURES_D3D9, nullptr),
	m_pcActiveDepthStencilSurface(nullptr),
	m_pcActiveBackBufferSurface(nullptr),
	m_apcMonitoredSurfaces(0, nullptr),
	m_apcStereoTwinSurfaces(0, nullptr),
	m_apcStereoTwinTextures(0, nullptr),
	m_apcActiveStereoTwinRenderTarget(0, nullptr),
	m_apcActiveStereoTwinTextures(MAX_SIMULTANEOUS_TEXTURES_D3D9, nullptr),
	m_pcActiveStereoTwinDepthStencilSurface(nullptr),
	m_pcActiveStereoTwinBackBufferSurface(nullptr),
	m_anMonitoredRenderTargetsCheckTimeCounter(0, 0),
	m_dwNewStereoTwinRenderTargets(0),
	m_pdwRenderTargetIndex(nullptr),
	m_ppcRenderTarget(nullptr),
	m_ppcNewZStencil(nullptr),
	m_pdwSampler(nullptr),
	m_ppcTexture(nullptr),
	m_hBitmapControl(nullptr),
	m_bControlUpdate(false),
	m_hFont(nullptr),
	m_apcStereoTwinRenderTargetClipboard(0, nullptr),
	m_apcStereoTwinRenderTextureClipboard(0, nullptr),
	m_dwTextureNumber(0),
	m_dwRenderTargetNumber(0),
	m_dwMaxRenderTargets(0),
	m_bMaxRenderTargets(false),
	m_bPresent(false),
	m_bApply(false),
	m_nChecktimeFrameConstant(30),                          /**< Set this constant to 30 frames, later we should be able to change this value on the node. ***/
	m_pcStereoOutputLeft(nullptr),
	m_pcStereoOutputRight(nullptr),
	m_pcStereoOutputSurfaceLeft(nullptr),
	m_pcStereoOutputSurfaceRight(nullptr)
{
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
	for (int i = 0; i < (int)m_apcStereoTwinSurfaces.size(); i++)
	{
		if (m_apcStereoTwinSurfaces[i])
			m_apcStereoTwinSurfaces[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinTextures.size(); i++)
	{
		if (m_apcStereoTwinTextures[i])
			m_apcStereoTwinTextures[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinRenderTargetClipboard.size(); i++)
	{
		if (m_apcStereoTwinRenderTargetClipboard[i])
			m_apcStereoTwinRenderTargetClipboard[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinRenderTextureClipboard.size(); i++)
	{
		if (m_apcStereoTwinRenderTextureClipboard[i])
			m_apcStereoTwinRenderTextureClipboard[i]->Release();
	}

	if (m_pcStereoOutputSurfaceLeft)
	{
		m_pcStereoOutputSurfaceLeft->Release();
		m_pcStereoOutputSurfaceLeft = nullptr;
	}
	if (m_pcStereoOutputSurfaceRight)
	{
		m_pcStereoOutputSurfaceRight->Release();
		m_pcStereoOutputSurfaceRight = nullptr;
	}
	if (m_pcStereoOutputLeft)
	{
		m_pcStereoOutputLeft->Release();
		m_pcStereoOutputLeft = nullptr;
	}
	if (m_pcStereoOutputRight)
	{
		m_pcStereoOutputRight->Release();
		m_pcStereoOutputRight = nullptr;
	}
}

/**
* Return the name of the  Stereo Splitter node.
***/
const char* StereoSplitter::GetNodeType() 
{
	return "Stereo Splitter"; 
}

/**
* Returns a global unique identifier for the Stereo Splitter node.
***/
UINT StereoSplitter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 64
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Stereo Splitter node.
***/
LPWSTR StereoSplitter::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Stereo Splitter node.
***/
HBITMAP StereoSplitter::GetLogo() 
{ 
	HMODULE hModule = GetModuleHandle(L"VireioStereoSplitter.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/** 
* Returns the updated control for the Stereo Splitter node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP StereoSplitter::GetControl()
{
	if (!m_hBitmapControl)
	{
		// create bitmap, set control update to true
		HWND hwnd = GetActiveWindow();
		HDC hdc = GetDC(hwnd);
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 2800);
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
		SetRect(&rc, 0, 0, 1024, 2800);
		FillRect(hdcImage, &rc, (HBRUSH)CreateSolidBrush(RGB(160, 160, 200)));

		// create font
		if (!m_hFont)
			m_hFont = CreateFont(64, 0, 0, 0, 0, FALSE,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			L"Segoe UI");

		// Select the variable stock font into the specified device context. 
		if (hOldFont = (HFONT)SelectObject(hdcImage, m_hFont))
		{
			int nY = 16;
			wchar_t szBuffer[256];

			SetTextColor(hdcImage, RGB(240,240,240));
			SetBkColor(hdcImage, RGB(160, 160, 200));

			// display all data
			TextOut(hdcImage, 50, nY, L"RenderTargetIndex", 17); nY+=64;
			TextOut(hdcImage, 50, nY, L"pRenderTarget", 13); nY+=64;
			TextOut(hdcImage, 50, nY, L"pNewZStencil", 12); nY+=64;
			TextOut(hdcImage, 50, nY, L"Sampler", 7); nY+=64;
			TextOut(hdcImage, 50, nY, L"pTexture", 8); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveType", 13); nY+=64;
			TextOut(hdcImage, 50, nY, L"StartVertex", 11); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveCount", 14); nY+=64;
			TextOut(hdcImage, 50, nY, L"Type", 4); nY+=64;
			TextOut(hdcImage, 50, nY, L"BaseVertexIndex", 15); nY+=64;
			TextOut(hdcImage, 50, nY, L"MinIndex", 8); nY+=64;
			TextOut(hdcImage, 50, nY, L"NumVertices", 11); nY+=64;
			TextOut(hdcImage, 50, nY, L"StartIndex", 10); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveCountIndexed", 21); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveTypeUP", 15); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveCountUP", 16); nY+=64;
			TextOut(hdcImage, 50, nY, L"pVertexStreamZeroData", 21); nY+=64;
			TextOut(hdcImage, 50, nY, L"VertexStreamZeroStride", 22); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveTypeUPIndexed", 22); nY+=64;
			TextOut(hdcImage, 50, nY, L"MinVertexIndex", 14); nY+=64;
			TextOut(hdcImage, 50, nY, L"NumVerticesUPIndexed", 20); nY+=64;
			TextOut(hdcImage, 50, nY, L"PrimitiveCountUPIndexed", 23); nY+=64;
			TextOut(hdcImage, 50, nY, L"pIndexData", 10); nY+=64;
			TextOut(hdcImage, 50, nY, L"IndexDataFormat", 15); nY+=64;
			TextOut(hdcImage, 50, nY, L"pVertexStreamZeroDataIndexed", 28); nY+=64;
			TextOut(hdcImage, 50, nY, L"VertexStreamZeroStrideIndexed", 29); nY+=64;
			TextOut(hdcImage, 50, nY, L"Count", 5); nY+=64;
			TextOut(hdcImage, 50, nY, L"pRects", 6); nY+=64;
			TextOut(hdcImage, 50, nY, L"Flags", 5); nY+=64;
			TextOut(hdcImage, 50, nY, L"Color", 5); nY+=64;
			TextOut(hdcImage, 50, nY, L"Z", 1); nY+=64;
			TextOut(hdcImage, 50, nY, L"Stencil", 7); nY+=64;
			TextOut(hdcImage, 600, nY, L"Left Texture", 12); nY+=64;
			TextOut(hdcImage, 600, nY, L"Right Texture", 13); nY+=128;

			// output maximum simultanous render targets
			TextOut(hdcImage, 50, nY, L"Max Render Targets : ", 21);
			wsprintf(szBuffer, L"%u", (UINT)m_dwMaxRenderTargets);
			int nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of currently monitored render targets
			TextOut(hdcImage, 50, nY, L"Monitored Render Targets : ", 27);
			wsprintf(szBuffer, L"%u", (UINT)m_apcMonitoredSurfaces.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Check Time Counters : ", 22);
			wsprintf(szBuffer, L"%u", (UINT)m_anMonitoredRenderTargetsCheckTimeCounter.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Stereo Twin Targets : ", 22);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinSurfaces.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Stereo Twin Textures : ", 23);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinTextures.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Twin Target Clipboard : ", 24);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTargetClipboard.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Twin Textures Clipboard : ", 26);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTextureClipboard.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// debug output for back buffer
			if (m_pcActiveBackBufferSurface) 
			{
				if (!m_pcActiveStereoTwinBackBufferSurface)
					TextOut(hdcImage, 50, nY, L"Back Buffer set.", 16);
				else
					TextOut(hdcImage, 50, nY, L"Back Buffer + Twin set.", 23);
			}
			nY+=64;

			// Restore the original font.        
			SelectObject(hdcImage, hOldFont); 
		}

		// draw boundaries
		int nY = 16 + 128;
		HBRUSH hb = CreateSolidBrush(RGB(64, 64, 192));
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=64;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=128;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=192;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=64*6;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=64*4;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=64*8;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); nY+=64*6;
		SetRect(&rc, 0, nY, 600, nY+4); FillRect(hdcImage, &rc, hb); 

		SelectObject(hdcImage, hbmOld);
		DeleteDC(hdcImage);

		// next update only by request, return updated bitmap
		m_bControlUpdate = false;
		return m_hBitmapControl;
	}
	else
		return nullptr;	
}

/**
* Provides the name of the requested commander.
***/
LPWSTR StereoSplitter::GetCommanderName(DWORD dwCommanderIndex)
{
	switch((STS_Commanders)dwCommanderIndex)
	{
	case STS_Commanders::StereoTextureLeft:
		return L"Stereo Output Texture Left";
	case STS_Commanders::StereoTextureRight:
		return L"Stereo Output Texture Right";
	}

	return L"";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR StereoSplitter::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
		return L"RenderTargetIndex";
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
		return L"pRenderTarget";
	case STS_Decommanders::pNewZStencil:                  /**< ->SetDepthStencilSurface() stencil surface ***/
		return L"pNewZStencil";
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
		return L"Sampler";
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
		return L"pTexture";
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
		return L"PrimitiveType";
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
		return L"StartVertex";
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
		return L"PrimitiveCount";
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
		return L"Type";
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
		return L"BaseVertexIndex";
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
		return L"MinIndex";
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
		return L"NumVertices";
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
		return L"StartIndex";
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
		return L"PrimitiveCountIndexed";
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
		return L"PrimitiveTypeUP";
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
		return L"PrimitiveCountUP";
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
		return L"pVertexStreamZeroData";
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
		return L"VertexStreamZeroStride";
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
		return L"PrimitiveTypeUPIndexed";
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
		return L"MinVertexIndex";
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
		return L"NumVerticesUPIndexed";
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
		return L"PrimitiveCountUPIndexed";
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
		return L"pIndexData";
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
		return L"IndexDataFormat";
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
		return L"pVertexStreamZeroDataIndexed";
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		return L"VertexStreamZeroStrideIndexed";
	case STS_Decommanders::Count:                         /**< ->Clear() Number of rectangles in the array at pRects. ***/
		return L"Count";
	case STS_Decommanders::pRects:                        /**< ->Clear() Pointer to an array of D3DRECT structures. ***/
		return L"pRects";
	case STS_Decommanders::Flags:                         /**< ->Clear() D3DCLEAR flags that specify the surface(s) that will be cleared. ***/
		return L"Flags";
	case STS_Decommanders::Color:                         /**< ->Clear() Clear a render target to this ARGB color. ***/
		return L"Color";
	case STS_Decommanders::Z:                             /**< ->Clear() Clear the depth buffer to this new z value which ranges from 0 to 1. ***/
		return L"Z";
	case STS_Decommanders::Stencil:                       /**< ->Clear() Clear the stencil buffer to this new value ***/
		return L"Stencil";
	}

	return L"";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD StereoSplitter::GetCommanderType(DWORD dwCommanderIndex) 
{
	return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoSplitter::GetDecommanderType(DWORD dwDecommanderIndex) 
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
		return PNT_IDIRECT3DSURFACE9_PLUG_TYPE;
	case STS_Decommanders::pNewZStencil:                  /**< ->SetDepthStencilSurface() stencil surface ***/
		return PNT_IDIRECT3DSURFACE9_PLUG_TYPE;
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
		return PNT_IDIRECT3DBASETEXTURE9_PLUG_TYPE;
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
		return D3DPRIMITIVETYPE_PLUG_TYPE;
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
		return D3DPRIMITIVETYPE_PLUG_TYPE;
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
		return INT_PLUG_TYPE;
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
		return D3DPRIMITIVETYPE_PLUG_TYPE;
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
		return PNT_VOID_PLUG_TYPE;
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
		return D3DPRIMITIVETYPE_PLUG_TYPE;
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
		return PNT_VOID_PLUG_TYPE;
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
		return D3DFORMAT_PLUG_TYPE;
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
		return PNT_VOID_PLUG_TYPE;
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::Count:                         /**< ->Clear() Number of rectangles in the array at pRects. ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::pRects:                        /**< ->Clear() Pointer to an array of D3DRECT structures. ***/
		return PNT_D3DRECT_PLUG_TYPE;
	case STS_Decommanders::Flags:                         /**< ->Clear() D3DCLEAR flags that specify the surface(s) that will be cleared. ***/
		return UINT_PLUG_TYPE;
	case STS_Decommanders::Color:                         /**< ->Clear() Clear a render target to this ARGB color. ***/
		return D3DCOLOR_PLUG_TYPE;
	case STS_Decommanders::Z:                             /**< ->Clear() Clear the depth buffer to this new z value which ranges from 0 to 1. ***/
		return FLOAT_PLUG_TYPE;
	case STS_Decommanders::Stencil:                       /**< ->Clear() Clear the stencil buffer to this new value ***/
		return UINT_PLUG_TYPE;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* StereoSplitter::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch((STS_Commanders)dwCommanderIndex)
	{
	case STS_Commanders::StereoTextureLeft:
		return (void*)&m_pcStereoOutputLeft;
	case STS_Commanders::StereoTextureRight:
		return (void*)&m_pcStereoOutputRight;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoSplitter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
		m_pdwRenderTargetIndex = (DWORD*)pData;
		break;
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
		m_ppcRenderTarget = (IDirect3DSurface9**)pData;
		break;
	case STS_Decommanders::pNewZStencil:                  /**< ->SetDepthStencilSurface() stencil surface ***/
		m_ppcNewZStencil = (IDirect3DSurface9**)pData;
		break;
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
		m_pdwSampler = (DWORD*)pData;
		break;
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
		m_ppcTexture = (IDirect3DTexture9**)pData;
		break;
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
		m_pePrimitiveType = (D3DPRIMITIVETYPE*)pData;
		break;
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
		m_pdwStartVertex = (UINT*)pData;
		break;
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
		m_pdwPrimitiveCount = (UINT*)pData;
		break;
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
		m_peType = (D3DPRIMITIVETYPE*)pData;
		break;
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
		m_pnBaseVertexIndex = (INT*)pData;
		break;
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
		m_pdwMinIndex = (UINT*)pData;
		break;
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
		m_pdwNumVertices = (UINT*)pData;
		break;
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
		m_pdwStartIndex = (UINT*)pData;
		break;
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
		m_pdwPrimitiveCountIndexed = (UINT*)pData;
		break;
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
		m_pePrimitiveTypeUP = (D3DPRIMITIVETYPE*)pData;
		break;
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
		m_pdwPrimitiveCountUP = (UINT*)pData;
		break;
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
		m_ppVertexStreamZeroData = (void**)pData;
		break;
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
		m_pdwVertexStreamZeroStride = (UINT*)pData;
		break;
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
		m_pePrimitiveTypeUPIndexed = (D3DPRIMITIVETYPE*)pData;
		break;
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
		m_pdwMinVertexIndex = (UINT*)pData;
		break;
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
		m_pdwNumVerticesUPIndexed = (UINT*)pData;
		break;
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
		m_pdwPrimitiveCountUPIndexed = (UINT*)pData;
		break;
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
		m_ppIndexData = (void**)pData;
		break;
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
		m_peIndexDataFormat = (D3DFORMAT*)pData;
		break;
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
		m_ppVertexStreamZeroDataIndexed = (void**)pData;
		break;
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		m_pdwVertexStreamZeroStrideIndexed = (UINT*)pData;
		break;
	case STS_Decommanders::Count:                         /**< ->Clear() Number of rectangles in the array at pRects. ***/
		m_pdwCount = (DWORD*)pData;
		break;
	case STS_Decommanders::pRects:                        /**< ->Clear() Pointer to an array of D3DRECT structures. ***/
		m_ppsRects = (D3DRECT**)pData;
		break;
	case STS_Decommanders::Flags:                         /**< ->Clear() D3DCLEAR flags that specify the surface(s) that will be cleared. ***/
		m_pdwFlags = (DWORD*)pData;
		break;
	case STS_Decommanders::Color:                         /**< ->Clear() Clear a render target to this ARGB color. ***/
		m_psColor = (D3DCOLOR*)pData;
		break;
	case STS_Decommanders::Z:                             /**< ->Clear() Clear the depth buffer to this new z value which ranges from 0 to 1. ***/
		m_pfZ = (float*)pData;
		break;
	case STS_Decommanders::Stencil:                       /**< ->Clear() Clear the stencil buffer to this new value ***/
		m_pdwStencil = (DWORD*)pData;
		break;
	}
}

/**
* Stereo Splitter supports various D3D9 calls.
***/
bool StereoSplitter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  
{ 
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if (nD3DInterface == INTERFACE_IDIRECT3DDEVICE9)
		{
			if ((nD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CLEAR) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETTEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP))
				return true;
		}
		else if (nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
		{
			if (nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT) return true;
		}
		else if (nD3DInterface == INTERFACE_IDIRECT3DSTATEBLOCK9)
		{
			if (nD3DMethod == METHOD_IDIRECT3DSTATEBLOCK9_APPLY) return true;
		}
	}
	return false; 
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
***/
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	
{
	switch(eD3DInterface)
	{
	case INTERFACE_IDIRECT3DDEVICE9:
		{
			switch(eD3DMethod)
			{
			case METHOD_IDIRECT3DDEVICE9_PRESENT:
				Present((LPDIRECT3DDEVICE9)pThis);
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET:
				if (m_bPresent)
				{
					// get data
					DWORD dwRenderTargetIndex = 0;
					if (m_pdwRenderTargetIndex) dwRenderTargetIndex = *m_pdwRenderTargetIndex; else return nullptr;
					IDirect3DSurface9* pcRenderTarget = nullptr;
					if (m_ppcRenderTarget) pcRenderTarget = (IDirect3DSurface9*)*m_ppcRenderTarget; else return nullptr;

					// call method
					SetRenderTarget((LPDIRECT3DDEVICE9)pThis, dwRenderTargetIndex, pcRenderTarget);
				}
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE:
				if (m_bPresent)
				{
					// get data
					IDirect3DSurface9* pcNewZStencil = nullptr;
					if (m_ppcNewZStencil) pcNewZStencil = (IDirect3DSurface9*)*m_ppcNewZStencil; else return nullptr;

					// call method
					SetDepthStencilSurface((LPDIRECT3DDEVICE9)pThis, pcNewZStencil);
				}	
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_CLEAR:
				if (m_bPresent)
				{
					// get data
					DWORD dwCount = 0;
					if (m_pdwCount) dwCount = *m_pdwCount; else return nullptr;
					D3DRECT* pRects = nullptr;
					if (m_ppsRects) pRects = *m_ppsRects; else return nullptr;
					DWORD dwFlags = 0;
					if (m_pdwFlags) dwFlags = *m_pdwFlags; else return nullptr;
					D3DCOLOR sColor = 0;
					if (m_psColor) sColor = *m_psColor; else return nullptr;
					float fZ = 0.0f;
					if (m_pfZ) fZ = *m_pfZ; else return nullptr;
					DWORD dwStencil = 0;
					if (m_pdwStencil) dwStencil = *m_pdwStencil; else return nullptr;

					// call method
					Clear((LPDIRECT3DDEVICE9)pThis, dwCount, pRects, dwFlags, sColor, fZ, dwStencil);
				}	
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_SETTEXTURE:
				if (m_bPresent)
				{
					// get data
					DWORD dwSampler = 0;
					if (m_pdwSampler) dwSampler = *m_pdwSampler; else return nullptr;
					IDirect3DTexture9* pcTexture = nullptr;
					if (m_ppcTexture) pcTexture = (IDirect3DTexture9*)*m_ppcTexture; else return nullptr;

					// call method
					SetTexture((LPDIRECT3DDEVICE9)pThis, dwSampler, pcTexture);
				}
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE:
				if (m_bPresent)
				{
					// get data
					D3DPRIMITIVETYPE ePrimitiveType;
					if (m_pePrimitiveType) ePrimitiveType = *m_pePrimitiveType; else return nullptr;
					UINT dwStartVertex;
					if (m_pdwStartVertex) dwStartVertex = *m_pdwStartVertex; else return nullptr;
					UINT dwPrimitiveCount;
					if (m_pdwPrimitiveCount) dwPrimitiveCount = *m_pdwPrimitiveCount; else return nullptr;

					// call method
					DrawPrimitive((LPDIRECT3DDEVICE9)pThis, ePrimitiveType, dwStartVertex, dwPrimitiveCount);
				}
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE:
				if (m_bPresent)
				{
					// get data
					D3DPRIMITIVETYPE eType;
					if (m_peType) eType = *m_peType; else return nullptr;
					INT nBaseVertexIndex;
					if (m_pnBaseVertexIndex) nBaseVertexIndex = *m_pnBaseVertexIndex; else return nullptr;
					UINT dwMinIndex;
					if (m_pdwMinIndex) dwMinIndex = *m_pdwMinIndex; else return nullptr;
					UINT dwNumVertices;
					if (m_pdwNumVertices) dwNumVertices = *m_pdwNumVertices; else return nullptr;
					UINT dwStartIndex;
					if (m_pdwStartIndex) dwStartIndex = *m_pdwStartIndex; else return nullptr;
					UINT dwPrimitiveCountIndexed;
					if (m_pdwPrimitiveCountIndexed) dwPrimitiveCountIndexed = *m_pdwPrimitiveCountIndexed; else return nullptr;

					// call method
					DrawIndexedPrimitive((LPDIRECT3DDEVICE9)pThis, eType, nBaseVertexIndex, dwMinIndex, dwNumVertices, dwStartIndex, dwPrimitiveCountIndexed);
				}
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP:
				if (m_bPresent)
				{
					// get data
					D3DPRIMITIVETYPE ePrimitiveType;
					if (m_pePrimitiveTypeUP) ePrimitiveType = *m_pePrimitiveTypeUP; else return nullptr;
					UINT dwPrimitiveCount;
					if (m_pdwPrimitiveCountUP) dwPrimitiveCount = *m_pdwPrimitiveCountUP; else return nullptr;
					void* pVertexStreamZeroData;
					if (m_ppVertexStreamZeroData) pVertexStreamZeroData = *m_ppVertexStreamZeroData; else return nullptr;
					UINT dwVertexStreamZeroStride;
					if (m_pdwVertexStreamZeroStride) dwVertexStreamZeroStride = *m_pdwVertexStreamZeroStride; else return nullptr;

					// call method
					DrawPrimitiveUP((LPDIRECT3DDEVICE9)pThis, ePrimitiveType, dwPrimitiveCount, pVertexStreamZeroData, dwVertexStreamZeroStride);
				}
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP:
				if (m_bPresent)
				{
					// get data
					D3DPRIMITIVETYPE ePrimitiveType;
					if (m_pePrimitiveTypeUPIndexed) ePrimitiveType = *m_pePrimitiveTypeUPIndexed; else return nullptr;
					UINT dwMinVertexIndex;
					if (m_pdwMinVertexIndex) dwMinVertexIndex = *m_pdwMinVertexIndex; else return nullptr;
					UINT dwNumVerticesUPIndexed;
					if (m_pdwNumVerticesUPIndexed) dwNumVerticesUPIndexed = *m_pdwNumVerticesUPIndexed; else return nullptr;
					UINT dwPrimitiveCountUPIndexed;
					if (m_pdwPrimitiveCountUPIndexed) dwPrimitiveCountUPIndexed = *m_pdwPrimitiveCountUPIndexed; else return nullptr;
					void* pIndexData;
					if (m_ppIndexData) pIndexData = *m_ppIndexData; else return nullptr;
					D3DFORMAT eIndexDataFormat;
					if (m_peIndexDataFormat) eIndexDataFormat = *m_peIndexDataFormat; else return nullptr;
					void* pVertexStreamZeroDataIndexed;
					if (m_ppVertexStreamZeroDataIndexed) pVertexStreamZeroDataIndexed = *m_ppVertexStreamZeroDataIndexed; else return nullptr;
					UINT dwVertexStreamZeroStrideIndexed;
					if (m_pdwVertexStreamZeroStrideIndexed) dwVertexStreamZeroStrideIndexed = *m_pdwVertexStreamZeroStrideIndexed; else return nullptr;

					// call method
					DrawIndexedPrimitiveUP((LPDIRECT3DDEVICE9)pThis, ePrimitiveType, dwMinVertexIndex, dwNumVerticesUPIndexed, dwPrimitiveCountUPIndexed, pIndexData, eIndexDataFormat, pVertexStreamZeroDataIndexed, dwVertexStreamZeroStrideIndexed);
				}
				return nullptr;
			}
			return nullptr;
		}
	case INTERFACE_IDIRECT3DSWAPCHAIN9:
		switch(eD3DMethod)
		{
		case METHOD_IDIRECT3DSWAPCHAIN9_PRESENT:
			return nullptr;
		}
		return nullptr;
	case INTERFACE_IDIRECT3DSTATEBLOCK9:
		switch (eD3DMethod)
		{
		case METHOD_IDIRECT3DSTATEBLOCK9_APPLY:
			Apply();
			return nullptr;
		}
	}
	return nullptr;
}

/**
* Incoming Present() call.
* Handle the check time counter.
***/
void StereoSplitter::Present(IDirect3DDevice9* pcDevice)
{
	// Check the maximum number of supported render targets
	if (m_dwMaxRenderTargets == 0)
	{
		D3DCAPS9 sCapabilities;
		pcDevice->GetDeviceCaps(&sCapabilities);
		m_dwMaxRenderTargets = sCapabilities.NumSimultaneousRTs;
	}

	// validate active render targets vector here
	if (m_dwMaxRenderTargets != m_apcActiveRenderTargets.size())
	{
		m_apcActiveRenderTargets.resize(m_dwMaxRenderTargets, NULL);
		m_apcActiveStereoTwinRenderTarget.resize(m_dwMaxRenderTargets, NULL);
		m_bControlUpdate = true;
		if (m_dwMaxRenderTargets == m_apcActiveRenderTargets.size())
			m_bMaxRenderTargets = true;
	}

	// create new stereo twin render targets or assign from clipboard
	for (DWORD i = 0; i < m_dwNewStereoTwinRenderTargets; i++)
	{
		// get monitored render target a twin needs to be found for
		DWORD dwIndex = (DWORD)m_apcMonitoredSurfaces.size() - (m_dwNewStereoTwinRenderTargets - i);
		LPDIRECT3DSURFACE9 pcRenderTarget = m_apcMonitoredSurfaces[dwIndex];

		// get description
		D3DSURFACE_DESC desc;
		if (pcRenderTarget)
		{
			// get render target desc
			pcRenderTarget->GetDesc(&desc);

			// get target pointers, both tex + surface
			LPDIRECT3DTEXTURE9 pcStereoTwinTexture = nullptr;
			LPDIRECT3DSURFACE9 pcStereoTwinSurface = nullptr;

			// loop through the clipboard, try to find useable stereo twin render target
			int nIndex = 0;
			auto it = m_apcStereoTwinRenderTargetClipboard.begin();
			while (it < m_apcStereoTwinRenderTargetClipboard.end())
			{
				// get surface desc
				D3DSURFACE_DESC descClipboard;
				(*it)->GetDesc(&descClipboard);

				// compare descriptions, take this target if all matches
				if ((desc.Width == descClipboard.Width) ||
					(desc.Height == descClipboard.Height) ||
					(desc.Usage == descClipboard.Usage) ||
					(desc.Format == descClipboard.Format))
				{
					// first, set the new stereo twins from the clipboard
					pcStereoTwinSurface = m_apcStereoTwinRenderTargetClipboard[nIndex];
					pcStereoTwinTexture = m_apcStereoTwinRenderTextureClipboard[nIndex];

					// erase render target iterator for both clipboard vectors
					m_apcStereoTwinRenderTargetClipboard.erase(m_apcStereoTwinRenderTargetClipboard.begin() + nIndex);
					m_apcStereoTwinRenderTextureClipboard.erase(m_apcStereoTwinRenderTextureClipboard.begin() + nIndex);

					// end loop
					it = m_apcStereoTwinRenderTargetClipboard.end();
				}
				else
				{
					// increase index
					nIndex++;

					// increase iterator
					it++;
				}
			}

			// if no render target (texture) is found on the clipboard, create a new
			if (pcStereoTwinSurface == nullptr)
			{
				switch(desc.Type)
				{
				case D3DRESOURCETYPE::D3DRTYPE_SURFACE:
					// depth stencil ??
					if ((desc.Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
					{
						// discard must be false since we switch the stencil surfaces for every draw call
						if (FAILED(pcDevice->CreateDepthStencilSurface(desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, false, &pcStereoTwinSurface, NULL)))
						{
							OutputDebugString(L"VireioStereoSplitter : Failed to create depth stencil surface.");
#ifdef _DEBUG
							wchar_t buf[32];
							wsprintf(buf, L"desc.Width %u", desc.Width); OutputDebugString(buf);
							wsprintf(buf, L"desc.Height %u", desc.Height); OutputDebugString(buf);
							wsprintf(buf, L"desc.Format %u", desc.Format); OutputDebugString(buf);
							wsprintf(buf, L"desc.MultiSampleType %u", desc.MultiSampleType); OutputDebugString(buf);
							wsprintf(buf, L"desc.MultiSampleQuality %u", desc.MultiSampleQuality); OutputDebugString(buf);
#endif
						}
					}
					else
						if (FAILED(pcDevice->CreateRenderTarget(desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, false, &pcStereoTwinSurface, NULL)))
						{
							if (FAILED(pcDevice->CreateRenderTarget(desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, true, &pcStereoTwinSurface, NULL)))
							{							
								OutputDebugString(L"VireioStereoSplitter : Failed to create render target.");
#ifdef _DEBUG
								wchar_t buf[32];
								wsprintf(buf, L"desc.Width %u", desc.Width); OutputDebugString(buf);
								wsprintf(buf, L"desc.Height %u", desc.Height); OutputDebugString(buf);
								wsprintf(buf, L"desc.Format %u", desc.Format); OutputDebugString(buf);
								wsprintf(buf, L"desc.MultiSampleType %u", desc.MultiSampleType); OutputDebugString(buf);
								wsprintf(buf, L"desc.MultiSampleQuality %u", desc.MultiSampleQuality); OutputDebugString(buf);
#endif
								pcStereoTwinSurface = nullptr;
							}
						}
						break;
				case D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
					{
						if (FAILED(pcDevice->CreateTexture((UINT)desc.Width, (UINT)desc.Height, 1, desc.Usage, desc.Format, D3DPOOL_DEFAULT, &pcStereoTwinTexture, NULL)))
						{
							OutputDebugString(L"VireioStereoSplitter : Failed to create render target texture.");
#ifdef _DEBUG							
							wchar_t buf[32];
							wsprintf(buf, L"desc.Width %u", desc.Width); OutputDebugString(buf);
							wsprintf(buf, L"desc.Height %u", desc.Height); OutputDebugString(buf);
							wsprintf(buf, L"desc.Usage %u", desc.Usage); OutputDebugString(buf);
							wsprintf(buf, L"desc.Format %u", desc.Format); OutputDebugString(buf);
#endif
							pcStereoTwinTexture = nullptr;
						}
						else
							pcStereoTwinTexture->GetSurfaceLevel(0, &pcStereoTwinSurface);
					}
					break;
				default:
					// TODO !! HANDLE CUBE + VOLUME TEX
					break;
				}
			}

			// add to stereo twin render targets
			m_apcStereoTwinSurfaces.push_back(pcStereoTwinSurface);
			m_apcStereoTwinTextures.push_back(pcStereoTwinTexture);			

			// update control
			m_bControlUpdate = true;
		}
		else
		{
			// code failure, null pointer render target
			OutputDebugString(L"VireioStereoSplitter code failure ! Null pointer monitored render target !");
			m_apcStereoTwinSurfaces.push_back(nullptr);
			m_apcStereoTwinTextures.push_back(nullptr);

			// update control
			m_bControlUpdate = true;
		}
	}

	// finally, clear new render targets number
	m_dwNewStereoTwinRenderTargets = 0;

	// loop through check time vector, decrease check time counter for each render target.. 
	{
		int nIndex = 0;
		auto it = m_anMonitoredRenderTargetsCheckTimeCounter.begin();
		while (it < m_anMonitoredRenderTargetsCheckTimeCounter.end())
		{
			// decrease counter
			auto i = *it; i--; *it = i;

			// remove render target (+counter) from list if counter <= zero
			if (*it <= 0)
			{
				// first, move the stereo twin of this render target to the clipboard
				m_apcStereoTwinRenderTargetClipboard.push_back(m_apcStereoTwinSurfaces[nIndex]);
				m_apcStereoTwinRenderTextureClipboard.push_back(m_apcStereoTwinTextures[nIndex]);

				// erase render target iterator for all 3 vectors (original, stereo twin, stereo twin texture)
				m_apcMonitoredSurfaces.erase(m_apcMonitoredSurfaces.begin() + nIndex);
				m_apcStereoTwinSurfaces.erase(m_apcStereoTwinSurfaces.begin() + nIndex);
				m_apcStereoTwinTextures.erase(m_apcStereoTwinTextures.begin() + nIndex);

				// erase check time counter
				m_anMonitoredRenderTargetsCheckTimeCounter.erase(it);

				// end loop
				it = m_anMonitoredRenderTargetsCheckTimeCounter.end();

				// update control
				m_bControlUpdate = true;
			}
			else
			{
				// increase index
				nIndex++;

				// increase iterator
				it++;
			}
		}
	}

	// finally, provide pointers to the left and right render target - get back buffer
	m_pcActiveBackBufferSurface = nullptr;
	pcDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pcActiveBackBufferSurface);

	if (m_pcActiveBackBufferSurface)
	{
		// check wether back buffer is actually monitored
		int nIndex = CheckIfMonitored(m_pcActiveBackBufferSurface);
		if (nIndex == -1)
		{
			// not monitored, so start
			MonitorSurface(m_pcActiveBackBufferSurface);
		}
		else
		{
			// set twin surface if twin created
			if (nIndex < (int)m_apcStereoTwinSurfaces.size())
				m_pcActiveStereoTwinBackBufferSurface = m_apcStereoTwinSurfaces[nIndex];
			else
				// set twin surface to null meanwhile
				m_pcActiveStereoTwinBackBufferSurface = NULL;
		}

		if (nIndex > -1)
		{
			// create textures... (if not created) - THIS WILL BE MOVED TO THE STEREO PRESENTER NODE !!
			if ((!m_pcStereoOutputLeft) || (!m_pcStereoOutputRight))
			{
				D3DVIEWPORT9 sViewport;
				pcDevice->GetViewport(&sViewport);
				D3DSURFACE_DESC sDesc = D3DSURFACE_DESC();
				m_pcActiveBackBufferSurface->GetDesc(&sDesc);

				// got the backbuffer ?
				if (sDesc.Width > 0)
				{
					if (!m_pcStereoOutputLeft)
						pcDevice->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoOutputLeft, NULL);

					if (!m_pcStereoOutputRight)
						pcDevice->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoOutputRight, NULL);
				}
			}

			// ...and surfaces
			if (!m_pcStereoOutputSurfaceLeft)
				if (m_pcStereoOutputLeft)
					m_pcStereoOutputLeft->GetSurfaceLevel(0, &m_pcStereoOutputSurfaceLeft);
			if (!m_pcStereoOutputSurfaceRight)
				if (m_pcStereoOutputRight)
					m_pcStereoOutputRight->GetSurfaceLevel(0, &m_pcStereoOutputSurfaceRight);


			// first stereo output test
			if ((m_pcStereoOutputLeft) && (m_pcStereoOutputRight) && (m_pcStereoOutputSurfaceLeft) && (m_pcStereoOutputSurfaceRight))
			{
				if (m_pcActiveBackBufferSurface)
					pcDevice->StretchRect(m_pcActiveBackBufferSurface, NULL, m_pcStereoOutputSurfaceLeft, NULL, D3DTEXF_NONE);
				if (m_pcActiveStereoTwinBackBufferSurface)
					pcDevice->StretchRect(m_pcActiveStereoTwinBackBufferSurface, NULL, m_pcStereoOutputSurfaceRight, NULL, D3DTEXF_NONE);
			}
		}

		// release back buffer
		m_pcActiveBackBufferSurface->Release();
	}
	else
		OutputDebugString(L"Vireio Stereo Splitter : No primary swap chain found.");

	// set present() bool to true
	m_bPresent = true;


}

/**
* Incoming SetRenderTarget() call.
* Set render targets internally, handle render target monitoring.
***/ 
void StereoSplitter::SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD dwRenderTargetIndex,IDirect3DSurface9* pcRenderTarget)
{	
	// set the render target internally
	if ((dwRenderTargetIndex < m_dwMaxRenderTargets) && (m_bMaxRenderTargets))
	{
		// set NULL manually, otherwise just set the render target :
		if (!pcRenderTarget)
		{
			m_apcActiveRenderTargets[dwRenderTargetIndex] = NULL;
			m_apcActiveStereoTwinRenderTarget[dwRenderTargetIndex] = NULL;

			// set number of render targets
			m_dwRenderTargetNumber = 0;
			for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_apcActiveRenderTargets.size(); i++) 
				if (m_apcActiveRenderTargets[i] != NULL) m_dwRenderTargetNumber = (DWORD)i + 1;
		}
		else			
		{
			m_apcActiveRenderTargets[dwRenderTargetIndex] = pcRenderTarget;

			// set number of render targets
			if (dwRenderTargetIndex >= m_dwRenderTargetNumber) m_dwRenderTargetNumber = dwRenderTargetIndex + 1;
		}
	}
	else return;

	// return if NULL
	if (!pcRenderTarget) return;

	// check wether this render target is actually monitored
	int nIndex = CheckIfMonitored(pcRenderTarget);
	if (nIndex == -1)
	{
		// not monitored, so start
		MonitorSurface(pcRenderTarget);

		// set twin surface to null meanwhile
		m_apcActiveStereoTwinRenderTarget[dwRenderTargetIndex] = NULL;
	}
	else
	{
		// set twin surface if twin created
		if (nIndex < (int)m_apcStereoTwinSurfaces.size())
			m_apcActiveStereoTwinRenderTarget[dwRenderTargetIndex] = m_apcStereoTwinSurfaces[nIndex];
		else
			// set twin surface to null meanwhile
			m_apcActiveStereoTwinRenderTarget[dwRenderTargetIndex] = NULL;
	}
}

/**
* Incoming SetDepthStencilSurface() call.
***/
void StereoSplitter::SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil)
{
	// set NULL manually, otherwise just set the render target :
	if (!pNewZStencil) 
		m_pcActiveDepthStencilSurface = NULL;
	else			
		m_pcActiveDepthStencilSurface = pNewZStencil;

	// set NULL twin if NULL
	if (!pNewZStencil) 
		m_pcActiveStereoTwinDepthStencilSurface = NULL;
	else
	{
		// check wether this depth stencil is actually monitored
		int nIndex = CheckIfMonitored(pNewZStencil);
		if (nIndex == -1)
		{
			// not monitored, so start
			MonitorSurface(pNewZStencil);

			// set twin surface to null meanwhile
			m_pcActiveStereoTwinDepthStencilSurface = NULL;
		}
		else
		{
			// set twin surface if twin created
			if (nIndex < (int)m_apcStereoTwinSurfaces.size())
				m_pcActiveStereoTwinDepthStencilSurface = m_apcStereoTwinSurfaces[nIndex];
			else
				// set twin surface to null meanwhile
				m_pcActiveStereoTwinDepthStencilSurface = NULL;
		}
	}

	m_bControlUpdate = true;
}

/**
* Incoming SetTexture() call.
***/
void StereoSplitter::SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage,IDirect3DBaseTexture9* pcTexture)
{
	// set the texture internally
	if (Stage < MAX_SIMULTANEOUS_TEXTURES_D3D9)
	{
		// set NULL manually, otherwise just set the render target :
		if (!pcTexture) 
		{
			m_apcActiveTextures[Stage] = NULL;

			// set number of textures
			m_dwTextureNumber = 0;
			for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_apcActiveTextures.size(); i++) 
				if (m_apcActiveTextures[i] != NULL) m_dwTextureNumber = (DWORD)i + 1;
		}
		else
		{
			m_apcActiveTextures[Stage] = pcTexture;

			// set number of textures
			if (Stage >= m_dwTextureNumber) m_dwTextureNumber = Stage + 1;
		}
	}
	else return;

	// set NULL twin if NULL
	if (!pcTexture) 
		m_apcActiveStereoTwinTextures[Stage] = NULL;
	else
	{
		// get the surface
		IDirect3DSurface9* pcSurface = nullptr;
		D3DRESOURCETYPE type = pcTexture->GetType();
		switch (type)
		{
		case D3DRTYPE_TEXTURE:
			{
				IDirect3DTexture9* pcDerivedTexture = static_cast<IDirect3DTexture9*> (pcTexture);
				pcDerivedTexture->GetSurfaceLevel(0, &pcSurface);
				break;
			}
		case D3DRTYPE_VOLUMETEXTURE:
			{
				IDirect3DVolumeTexture9* pDerivedTexture = static_cast<IDirect3DVolumeTexture9*> (pcTexture);
				// TODO !! handle volume textures
				break;
			}
		case D3DRTYPE_CUBETEXTURE:
			{
				IDirect3DCubeTexture9* pDerivedTexture = static_cast<IDirect3DCubeTexture9*> (pcTexture);
				// TODO !! handle cube textures
				break;
			}
		}

		// set twin texture, if monitored
		int nIndex = CheckIfMonitored(pcSurface);
		if (nIndex == -1)
		{
			// set stereo texture to null meanwhile
			m_apcActiveStereoTwinTextures[Stage] = NULL;
		}
		else
		{
			// set twin surface if twin in created
			if (nIndex < (int)m_apcStereoTwinTextures.size())
				// set twin surface
					m_apcActiveStereoTwinTextures[Stage] = m_apcStereoTwinTextures[nIndex];
			else
				// set stereo texture to null meanwhile
				m_apcActiveStereoTwinTextures[Stage] = NULL;
		}
	}

	m_bControlUpdate = true;
}

/**
* Incoming Clear() call.
***/
void StereoSplitter::Clear(IDirect3DDevice9* pcDevice, DWORD dwCount, D3DRECT *pRects, DWORD dwFlags, D3DCOLOR sColor, float fZ, DWORD dwStencil)
{
	// Always draw first on the right side, the left (original) side
	// will be drawn at the end
	SetDrawingSide(pcDevice, RenderPosition::Right);

	// draw
	pcDevice->Clear(dwCount, pRects, dwFlags, sColor, fZ, dwStencil);

	// Allways switch back to the left (original) side here !
	SetDrawingSide(pcDevice, RenderPosition::Left);
}

/**
* Incoming DrawPrimitive() call.
***/
void StereoSplitter::DrawPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwStartVertex, UINT dwPrimitiveCount)
{
	// Always draw first on the right side, the left (original) side
	// will be drawn at the end
	SetDrawingSide(pcDevice, RenderPosition::Right);

	// draw
	pcDevice->DrawPrimitive(ePrimitiveType, dwStartVertex, dwPrimitiveCount);

	// Allways switch back to the left (original) side here !
	SetDrawingSide(pcDevice, RenderPosition::Left);
}

/**
* Incoming DrawIndexedPrimitive() call.
***/
void StereoSplitter::DrawIndexedPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, INT nBaseVertexIndex, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwStartIndex, UINT dwPrimCount)
{
	// Always draw first on the right side, the left (original) side
	// will be drawn at the end
	SetDrawingSide(pcDevice, RenderPosition::Right);

	// draw
	pcDevice->DrawIndexedPrimitive(ePrimitiveType, nBaseVertexIndex, dwMinVertexIndex, dwNumVertices, dwStartIndex, dwPrimCount);

	// Allways switch back to the left (original) side here !
	SetDrawingSide(pcDevice, RenderPosition::Left);
}

/**
* Incoming DrawPrimitiveUP() call.
***/
void StereoSplitter::DrawPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwPrimitiveCount, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
{
	// Always draw first on the right side, the left (original) side
	// will be drawn at the end
	SetDrawingSide(pcDevice, RenderPosition::Right);

	// draw
	pcDevice->DrawPrimitiveUP(ePrimitiveType, dwPrimitiveCount, pVertexStreamZeroData, dwVertexStreamZeroStride);

	// Allways switch back to the left (original) side here !
	SetDrawingSide(pcDevice, RenderPosition::Left);
}

/**
* Incoming DrawIndexedPrimitiveUP() call.
***/
void StereoSplitter::DrawIndexedPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwPrimitiveCount, CONST void* pIndexData, D3DFORMAT eIndexDataFormat, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
{
	// Always draw first on the right side, the left (original) side
	// will be drawn at the end
	SetDrawingSide(pcDevice, RenderPosition::Right);

	// draw
	pcDevice->DrawIndexedPrimitiveUP(ePrimitiveType, dwMinVertexIndex, dwNumVertices, dwPrimitiveCount, pIndexData, eIndexDataFormat, pVertexStreamZeroData, dwVertexStreamZeroStride);

	// Allways switch back to the left (original) side here !
	SetDrawingSide(pcDevice, RenderPosition::Left);
}

/**
* IDirect3DStateBlock9->Apply()
***/
void StereoSplitter::Apply()
{
	// state block to be applied, will be handled in SetDrawingSide()
	m_bApply = true;
}

/**
* Index of the monitored surface in m_apcMonitoredSurfaces, -1 if not monitored.
***/
int StereoSplitter::CheckIfMonitored(IDirect3DSurface9* pcSurface)
{
	auto it = std::find(m_apcMonitoredSurfaces.begin(), m_apcMonitoredSurfaces.end(), pcSurface);
	if(it != m_apcMonitoredSurfaces.end()) 
	{
		// set check time counter if this render target is actually monitored
		auto index = it - m_apcMonitoredSurfaces.begin();
		if (m_anMonitoredRenderTargetsCheckTimeCounter.size() > (size_t)index)
		{
			m_anMonitoredRenderTargetsCheckTimeCounter[index] = m_nChecktimeFrameConstant;
			return (int)index;
		}
	} 
	return -1;
}

/**
* Start to monitor surface.
***/
void StereoSplitter::MonitorSurface(IDirect3DSurface9* pcSurface)
{
	// add new render target + check time frame constant
	m_apcMonitoredSurfaces.push_back(pcSurface);
	m_anMonitoredRenderTargetsCheckTimeCounter.push_back(m_nChecktimeFrameConstant);

	// increase new render targets number, stereo twin will be created in Present() call
	m_dwNewStereoTwinRenderTargets++;

	// update control
	m_bControlUpdate = true;
}

/**
* Switches rendering to which ever side is specified by side.
* Use to specify the side that you want to draw to.
* 
* Derives D3DProxyDevice::setDrawingSide() from the Vireio 2.x.x driver code.
* 
* @return True if change succeeded, false if it fails.
* Attempting to switch to a side when that side is already the active side will return true without making any changes.
***/
bool StereoSplitter::SetDrawingSide(IDirect3DDevice9* pcDevice, RenderPosition eSide)
{
	// Already on the correct eye
	if (eSide == m_eCurrentRenderingSide)
		return true;

	// state block was applied ?
	if (m_bApply)
	{
		// verify the render targets
		for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_apcActiveRenderTargets.size(); i++) 
		{
			// get the render target for this index
			LPDIRECT3DSURFACE9 pcRenderTarget;
			pcDevice->GetRenderTarget((DWORD)i, &pcRenderTarget);

			// is this render target stored ?
			if (pcRenderTarget != m_apcActiveRenderTargets[i])
			{
				SetRenderTarget(pcDevice, (DWORD)i, pcRenderTarget);
			}

			// ..and release
			pcRenderTarget->Release();
		}

		// TODO !! TEXTURES

		// state block is handled
		m_bApply = false;
	}

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	m_eCurrentRenderingSide = eSide;

	// switch render targets to new eSide
	bool renderTargetChanged = false;
	HRESULT hr = D3D_OK;
	for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_dwRenderTargetNumber; i++) 
	{
		if (eSide == RenderPosition::Left) 
		{
			if (m_apcActiveRenderTargets[i])
				hr = pcDevice->SetRenderTarget(i, m_apcActiveRenderTargets[i]); 
			else
				hr = pcDevice->SetRenderTarget(i, NULL); 
		}
		else 
		{
			if (i < m_apcActiveStereoTwinRenderTarget.size())
			{
				if (m_apcActiveStereoTwinRenderTarget[i])
					hr = pcDevice->SetRenderTarget(i, m_apcActiveStereoTwinRenderTarget[i]);
				else
					// never set first render target to NULL
					if (i > 0)
						hr = pcDevice->SetRenderTarget(i, NULL);
			}
		}

		if (hr != D3D_OK) 
		{
			OutputDebugString(L"Error trying to set one of the Render Targets while switching between active eyes for drawing.\n");
		}
		else 
		{
			renderTargetChanged = true;
		}
	}

	// if a non-fullsurface viewport is active and a rendertarget changed we need to reapply the viewport - TODO !!
	/*if (renderTargetChanged && !m_bActiveViewportIsDefault) {
	BaseDirect3DDevice9::SetViewport(&m_LastViewportSet);
	}*/

	// switch depth stencil to new side
	if (m_pcActiveDepthStencilSurface != NULL) { 
		if (eSide == RenderPosition::Left) 
			hr = pcDevice->SetDepthStencilSurface(m_pcActiveDepthStencilSurface);
		else 
		{
			if (m_pcActiveStereoTwinDepthStencilSurface)
				hr = pcDevice->SetDepthStencilSurface(m_pcActiveStereoTwinDepthStencilSurface);
			else
				hr = pcDevice->SetDepthStencilSurface(NULL);
		}
	}

	// switch textures to new side
	for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_dwTextureNumber; i++)
	{
		// if stereo texture
		if (m_apcActiveStereoTwinTextures[i] != NULL) 
		{ 
			if (eSide == RenderPosition::Left) 
			{
				if (m_apcActiveTextures[i])
					hr = pcDevice->SetTexture(i, m_apcActiveTextures[i]);
				else
					hr = pcDevice->SetTexture(i, NULL);
			}
			else 
				hr = pcDevice->SetTexture(i, m_apcActiveStereoTwinTextures[i]);
		}
		// else the texture is mono and doesn't need changing. It will always be set initially and then won't need changing

		if (hr != D3D_OK)
			OutputDebugString(L"Error trying to set one of the textures while switching between active eyes for drawing.\n");
	}

	// TODO !!
	// the rest of the code from the original method from the Vireio 2.x.x D3DProxyDevice 

	return true;
}

