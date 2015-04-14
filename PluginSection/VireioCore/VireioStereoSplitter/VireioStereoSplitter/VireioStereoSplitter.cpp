/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter.cpp> and
Class <VireioStereoSplitter> :
Copyright (C) 2015 Denis Reischl

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
#define INTERFACE_IDIRECT3DSWAPCHAIN9                       15
#define	METHOD_IDIRECT3DDEVICE9_PRESENT                     17
#define METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET             37
#define METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE      39 
#define METHOD_IDIRECT3DDEVICE9_SETTEXTURE                  65
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE               81
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE        82 
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP             83 
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP      84 
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT                   3

/**
* Constructor.
***/
StereoSplitter::StereoSplitter():AQU_Nodus(),
	m_pdwRenderTargetIndex(nullptr),
	m_ppcRenderTarget(nullptr),
	m_hBitmapControl(nullptr),
	m_bControlUpdate(false),
	m_hFont(nullptr),
	m_apcActiveRenderTargets(0, nullptr),
	m_apcMonitoredRenderTargets(0, nullptr),
	m_anMonitoredRenderTargetsCheckTimeCounter(0, 0),
	m_apcStereoTwinRenderTargets(0, nullptr),
	m_apcStereoTwinRenderTextures(0, nullptr),
	m_dwNewStereoTwinRenderTargets(0),
	m_apcStereoTwinRenderTargetClipboard(0, nullptr),
	m_apcStereoTwinRenderTextureClipboard(0, nullptr),
	m_dwMaxRenderTargets(0),
	m_bMaxRenderTargets(false),
	m_nChecktimeFrameConstant(30)                           /**< Set this constant to 30 frames, later we should be able to change this value on the node. ***/
{
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
	for (int i = 0; i < (int)m_apcStereoTwinRenderTargets.size(); i++)
	{
		if (m_apcStereoTwinRenderTargets[i])
			m_apcStereoTwinRenderTargets[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinRenderTextures.size(); i++)
	{
		if (m_apcStereoTwinRenderTextures[i])
			m_apcStereoTwinRenderTextures[i]->Release();
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
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 2600);
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
		SetRect(&rc, 0, 0, 1024, 2600);
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
			TextOut(hdcImage, 50, nY, L"VertexStreamZeroStrideIndexed", 29); nY+=128;

			// output maximum simultanous render targets
			TextOut(hdcImage, 50, nY, L"Max Render Targets : ", 21);
			wsprintf(szBuffer, L"%u", (UINT)m_dwMaxRenderTargets);
			int nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of currently monitored render targets
			TextOut(hdcImage, 50, nY, L"Monitored Render Targets : ", 27);
			wsprintf(szBuffer, L"%u", (UINT)m_apcMonitoredRenderTargets.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Check Time Counters : ", 22);
			wsprintf(szBuffer, L"%u", (UINT)m_anMonitoredRenderTargetsCheckTimeCounter.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Stereo Twin Targets : ", 22);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTargets.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 650, nY, szBuffer, nLen); nY+=64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Stereo Twin Textures : ", 23);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTextures.size());
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

			// Restore the original font.        
			SelectObject(hdcImage, hOldFont); 
		}

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
	}

	return L"";
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
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		return 0;
		//return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
	}

	return 0;
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
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
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
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETTEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP))
				return true;
		}
		if (nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
		{
			if (nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT) return true;
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
				{
					// get data
					DWORD dwRenderTargetIndex = 0;
					if (m_pdwRenderTargetIndex) 
						dwRenderTargetIndex = *m_pdwRenderTargetIndex; 
					else 
						return nullptr;
					IDirect3DSurface9* pcRenderTarget = nullptr;
					if (m_ppcRenderTarget) 
						pcRenderTarget = (IDirect3DSurface9*)*m_ppcRenderTarget; 
					else
						return nullptr;

					// call method
					SetRenderTarget((LPDIRECT3DDEVICE9)pThis, dwRenderTargetIndex, pcRenderTarget);
					return nullptr;
				}
			case METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE:
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_SETTEXTURE:
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE:
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE:
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP:
				return nullptr;
			case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP:
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
		m_bControlUpdate = true;
		if (m_dwMaxRenderTargets == m_apcActiveRenderTargets.size())
			m_bMaxRenderTargets = true;
	}

	// create new stereo twin render targets or assign from clipboard
	for (DWORD i = 0; i < m_dwNewStereoTwinRenderTargets; i++)
	{
		// get monitored render target a twin needs to be found for
		DWORD dwIndex = (DWORD)m_apcMonitoredRenderTargets.size() - (m_dwNewStereoTwinRenderTargets - i);
		LPDIRECT3DSURFACE9 pcRenderTarget = m_apcMonitoredRenderTargets[dwIndex];

		// get description
		D3DSURFACE_DESC desc;
		if (pcRenderTarget)
		{
			// get render target desc
			pcRenderTarget->GetDesc(&desc);

			// get render target pointers, both tex + surface
			LPDIRECT3DTEXTURE9 pcStereoTwinRenderTexture = nullptr;
			LPDIRECT3DSURFACE9 pcStereoTwinRenderTarget = nullptr;

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
					pcStereoTwinRenderTarget = m_apcStereoTwinRenderTargetClipboard[nIndex];
					pcStereoTwinRenderTexture = m_apcStereoTwinRenderTextureClipboard[nIndex];

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
			if (pcStereoTwinRenderTarget == nullptr)
			{
				if (FAILED(pcDevice->CreateTexture((UINT)desc.Width, (UINT)desc.Height, 1, desc.Usage, desc.Format, D3DPOOL_DEFAULT, &pcStereoTwinRenderTexture, NULL)))
				{
					OutputDebugString(L"VireioStereoSplitter : Failed to create render target texture.");
					pcStereoTwinRenderTexture = nullptr;
				}
				else
					pcStereoTwinRenderTexture->GetSurfaceLevel(0, &pcStereoTwinRenderTarget);
			}

			// add to stereo twin render targets
			m_apcStereoTwinRenderTargets.push_back(pcStereoTwinRenderTarget);
			m_apcStereoTwinRenderTextures.push_back(pcStereoTwinRenderTexture);			

			// update control
			m_bControlUpdate = true;
		}
		else
		{
			// code failure, null pointer render target
			OutputDebugString(L"VireioStereoSplitter code failure ! Null pointer monitored render target !");
			m_apcStereoTwinRenderTargets.push_back(nullptr);
			m_apcStereoTwinRenderTextures.push_back(nullptr);

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
				m_apcStereoTwinRenderTargetClipboard.push_back(m_apcStereoTwinRenderTargets[nIndex]);
				m_apcStereoTwinRenderTextureClipboard.push_back(m_apcStereoTwinRenderTextures[nIndex]);

				// erase render target iterator for all 3 vectors (original, stereo twin, stereo twin texture)
				m_apcMonitoredRenderTargets.erase(m_apcMonitoredRenderTargets.begin() + nIndex);
				m_apcStereoTwinRenderTargets.erase(m_apcStereoTwinRenderTargets.begin() + nIndex);
				m_apcStereoTwinRenderTextures.erase(m_apcStereoTwinRenderTextures.begin() + nIndex);

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
			m_apcActiveRenderTargets[dwRenderTargetIndex] = NULL;
		else			
			m_apcActiveRenderTargets[dwRenderTargetIndex] = pcRenderTarget;
	}

	// return if NULL
	if (!pcRenderTarget) return;

	// check wether this render target is actually monitored
	{
		auto it = std::find(m_apcMonitoredRenderTargets.begin(), m_apcMonitoredRenderTargets.end(), pcRenderTarget);
		if(it != m_apcMonitoredRenderTargets.end()) 
		{
			// set check time counter if this render target is actually monitored
			auto index = it - m_apcMonitoredRenderTargets.begin();
			if (m_anMonitoredRenderTargetsCheckTimeCounter.size() > (size_t)index)
			{
				m_anMonitoredRenderTargetsCheckTimeCounter[index] = m_nChecktimeFrameConstant;
			}
		} 
		else 
		{
			// add new render target + check time frame constant
			m_apcMonitoredRenderTargets.push_back(pcRenderTarget);
			m_anMonitoredRenderTargetsCheckTimeCounter.push_back(m_nChecktimeFrameConstant);

			// increase new render targets number, stereo twin will be created in Present() call
			m_dwNewStereoTwinRenderTargets++;

			// update control
			m_bControlUpdate = true;
		}
	}
}

/**
* Incoming SetDepthStencilSurface() call.
***/
void StereoSplitter::SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil)
{
}

/**
* Incoming SetTexture() call.
***/
void StereoSplitter::SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
}

/**
* Incoming DrawPrimitive() call.
***/
void StereoSplitter::DrawPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwStartVertex, UINT dwPrimitiveCount)
{
}

/**
* Incoming DrawIndexedPrimitive() call.
***/
void StereoSplitter::DrawIndexedPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, INT nBaseVertexIndex, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwStartIndex, UINT dwPrimCount)
{
}

/**
* Incoming DrawPrimitiveUP() call.
***/
void StereoSplitter::DrawPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwPrimitiveCount, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
{
}

/**
* Incoming DrawIndexedPrimitiveUP() call.
***/
void StereoSplitter::DrawIndexedPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwPrimitiveCount, CONST void* pIndexData, D3DFORMAT eIndexDataFormat, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
{
}

