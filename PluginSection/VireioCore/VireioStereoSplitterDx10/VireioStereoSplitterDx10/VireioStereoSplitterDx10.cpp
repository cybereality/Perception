/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitterDx10.cpp> and
Class <VireioStereoSplitterDx10> :
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

#include"VireioStereoSplitterDx10.h"

#define INTERFACE_ID3D10DEVICE                                   7
#define INTERFACE_ID3D11DEVICECONTEXT                            11
#define INTERFACE_IDXGISWAPCHAIN                                 29
#define METHOD_ID3D10DEVICE_OMSETRENDERTARGETS                   24
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED                   12
#define METHOD_ID3D11DEVICECONTEXT_DRAW                          13
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED          20
#define METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED                 21
#define METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS            33
#define METHOD_ID3D11DEVICECONTEXT_DRAWAUTO                      38
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT  39
#define METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT         40
#define METHOD_IDXGISWAPCHAIN_PRESENT                            8

/**
* Constructor.
***/
StereoSplitter::StereoSplitter() : AQU_Nodus(),
m_apcActiveRenderTargetViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveStereoTwinViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_pcActiveDepthStencilView(nullptr),
m_pcActiveBackBufferSurface(nullptr),
m_apcMonitoredViews(0, nullptr),
m_apcStereoTwinViews(0, nullptr),
m_apcStereoTwinTextures(0, nullptr),
m_apcActiveStereoTwinTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_pcActiveStereoTwinDepthStencilView(nullptr),
//m_pcActiveStereoTwinBackBuffers(nullptr),
m_anMonitoredRenderTargetsCheckTimeCounter(0, 0),
m_dwNewStereoTwinRenderTargets(0),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_bPresent(false),
apcStereoTwinRenderTargetViewClipboard(0, nullptr),
m_apcStereoTwinRenderTextureClipboard(0, nullptr),
m_dwTextureNumber(0),
m_dwRenderTargetNumber(0),
m_eD3DVersion(D3DVersion::NotDefined),
m_nChecktimeFrameConstant(30)                          /**< Set this constant to 30 frames, later we should be able to change this value on the node. ***/
/*m_pcStereoOutputLeft(nullptr),
m_pcStereoOutputRight(nullptr),
m_pcStereoOutputSurfaceLeft(nullptr),
m_pcStereoOutputSurfaceRight(nullptr)*/
{
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
	for (int i = 0; i < (int)m_apcStereoTwinViews.size(); i++)
	{
		if (m_apcStereoTwinViews[i])
			m_apcStereoTwinViews[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinTextures.size(); i++)
	{
		if (m_apcStereoTwinTextures[i])
			m_apcStereoTwinTextures[i]->Release();
	}
	for (int i = 0; i < (int)apcStereoTwinRenderTargetViewClipboard.size(); i++)
	{
		if (apcStereoTwinRenderTargetViewClipboard[i])
			apcStereoTwinRenderTargetViewClipboard[i]->Release();
	}
	for (int i = 0; i < (int)m_apcStereoTwinRenderTextureClipboard.size(); i++)
	{
		if (m_apcStereoTwinRenderTextureClipboard[i])
			m_apcStereoTwinRenderTextureClipboard[i]->Release();
	}

	/*if (m_pcStereoOutputSurfaceLeft)
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
	}*/
}

/**
* Return the name of the  Stereo Splitter node.
***/
const char* StereoSplitter::GetNodeType()
{
	return "Stereo Splitter Dx10+";
}

/**
* Returns a global unique identifier for the Stereo Splitter node.
***/
UINT StereoSplitter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 65
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
	HMODULE hModule = GetModuleHandle(L"VireioStereoSplitterDx10.dll");
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
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 1920);
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
		SetRect(&rc, 0, 0, 1024, 1920);
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

			SetTextColor(hdcImage, RGB(240, 240, 240));
			SetBkColor(hdcImage, RGB(160, 160, 200));

			// display all data
			TextOut(hdcImage, 50, nY, L"NumViews", 8); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppRenderTargetViews DX10", 24); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView DX10", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppRenderTargetViews DX11", 24); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView DX11", 22); nY += 64;
			TextOut(hdcImage, 600, nY, L"Left Texture", 12); nY += 64;
			TextOut(hdcImage, 600, nY, L"Right Texture", 13); nY += 128;

			// output maximum simultanous render targets
			TextOut(hdcImage, 50, nY, L"Max Render Targets : ", 21);
			wsprintf(szBuffer, L"%u", (UINT)D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
			int nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of currently monitored render targets
			TextOut(hdcImage, 50, nY, L"Monitored Render Target Views : ", 32);
			wsprintf(szBuffer, L"%u", (UINT)m_apcMonitoredViews.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Check Time Counters : ", 22);
			wsprintf(szBuffer, L"%u", (UINT)m_anMonitoredRenderTargetsCheckTimeCounter.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of Stereo Twin Targets
			TextOut(hdcImage, 50, nY, L"Stereo Twin Target Views : ", 27);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinViews.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of Stereo Twin Textures
			TextOut(hdcImage, 50, nY, L"Stereo Twin Textures : ", 23);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinTextures.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Twin Target View Clipboard : ", 29);
			wsprintf(szBuffer, L"%u", (UINT)apcStereoTwinRenderTargetViewClipboard.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			// output the number of check time counters - this number should match monitored render targets number
			TextOut(hdcImage, 50, nY, L"Twin Textures Clipboard : ", 26);
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTextureClipboard.size());
			nLen = (int)wcslen(szBuffer); if (nLen > 11) nLen = 11;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

			//// debug output for back buffer
			//if (m_pcActiveBackBufferSurface)
			//{
			//	if (!m_pcActiveStereoTwinBackBufferSurface)
			//		TextOut(hdcImage, 50, nY, L"Back Buffer set.", 16);
			//	else
			//		TextOut(hdcImage, 50, nY, L"Back Buffer + Twin set.", 23);
			//}
			//nY += 64;

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
* Provides the name of the requested commander.
***/
LPWSTR StereoSplitter::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((STS_Commanders)dwCommanderIndex)
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
		case NumViews:
			return L"NumViews";
			break;
		case ppRenderTargetViews_DX10:
			return L"ppRenderTargetViews_DX10";
			break;
		case pDepthStencilView_DX10:
			return L"pDepthStencilView_DX10";
			break;
		case ppRenderTargetViews_DX11:
			return L"ppRenderTargetViews_DX11";
			break;
		case pDepthStencilView_DX11:
			return L"pDepthStencilView_DX11";
			break;
		default:
			return L"";
			break;
	}

	return L"";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD StereoSplitter::GetCommanderType(DWORD dwCommanderIndex)
{
	return NULL;//PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoSplitter::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case NumViews:
			return UINT_PLUG_TYPE;
			break;
		case ppRenderTargetViews_DX10:
			return PPNT_ID3D10RENDERTARGETVIEW;
			break;
		case pDepthStencilView_DX10:
			return PNT_ID3D10DEPTHSTENCILVIEW;
			break;
		case ppRenderTargetViews_DX11:
			return PPNT_ID3D11RENDERTARGETVIEW;
			break;
		case pDepthStencilView_DX11:
			return PNT_ID3D11DEPTHSTENCILVIEW;
			break;
		default:
			break;

	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* StereoSplitter::GetOutputPointer(DWORD dwCommanderIndex)
{

	//switch((STS_Commanders)dwCommanderIndex)
	//{
	///*case STS_Commanders::StereoTextureLeft:
	//	return (void*)&m_pcStereoOutputLeft;
	//case STS_Commanders::StereoTextureRight:
	//	return (void*)&m_pcStereoOutputRight;*/
	//}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoSplitter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case NumViews:
			m_pdwNumViews = (DWORD*)pData;                                 /** Number of render targets to bind. **/
			break;
		case ppRenderTargetViews_DX10:
			m_pppcRenderTargetViews_DX10 = (IUnknown***)pData;             /** Pointer to an array of render targets (see ID3D10RenderTargetView) to bind to the device. **/
			break;
		case pDepthStencilView_DX10:
			m_ppcDepthStencilView_DX10 = (IUnknown**)pData;                /** Pointer to a depth-stencil view (see ID3D10DepthStencilView) to bind to the device.**/
			break;
		case ppRenderTargetViews_DX11:
			m_pppcRenderTargetViews_DX11 = (IUnknown**)pData;              /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
			break;
		case pDepthStencilView_DX11:
			m_ppcDepthStencilView_DX11 = (IUnknown**)pData;                /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
			break;
		default:
			break;
	}

}

/**
* Stereo Splitter supports various D3D10 + D3D11 calls.
***/
bool StereoSplitter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_11_2))
	{
		if (nD3DInterface == INTERFACE_ID3D10DEVICE)
		{
			if (nD3DMethod == METHOD_ID3D10DEVICE_OMSETRENDERTARGETS)
				return true;
		}
		else if (nD3DInterface == INTERFACE_ID3D11DEVICECONTEXT)
		{
			if ((nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWAUTO) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT))
				return true;
		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{
			if (nD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)
				return true;
		}
	}
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
***/
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	switch (eD3DInterface)
	{
		case INTERFACE_ID3D10DEVICE:
			switch (eD3DMethod)
			{
				case METHOD_ID3D10DEVICE_OMSETRENDERTARGETS:
					if (m_bPresent)
					{
						// get data
						DWORD dwNumViews = 0;
						if (m_pdwNumViews) dwNumViews = *m_pdwNumViews; else return nullptr;
						ID3D10RenderTargetView** ppcRenderTargetViews = nullptr;
						if (m_pppcRenderTargetViews_DX10) ppcRenderTargetViews = (ID3D10RenderTargetView**)*m_pppcRenderTargetViews_DX10; else ppcRenderTargetViews = nullptr;
						ID3D10DepthStencilView* pcDepthStencilView = nullptr;
						if (m_ppcDepthStencilView_DX10) pcDepthStencilView = (ID3D10DepthStencilView*)*m_ppcDepthStencilView_DX10; else pcDepthStencilView = nullptr;

						// call method
						OMSetRenderTargets(dwNumViews, (IUnknown**)ppcRenderTargetViews, (IUnknown*)pcDepthStencilView);
					}
					// ensure D3D10 is set
					m_eD3DVersion = D3DVersion::Direct3D10;
					return nullptr;
			}
			return nullptr;

		case INTERFACE_ID3D11DEVICECONTEXT:
			switch (eD3DMethod)
			{
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAW:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWAUTO:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT:
					// set node behavior to "double call" for this method
					// node that this is only supported by drawing methods
					nProvokerIndex = -1;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS:
					if (m_bPresent)
					{
						// get data
						DWORD dwNumViews = 0;
						if (m_pdwNumViews) dwNumViews = *m_pdwNumViews; else return nullptr;
						ID3D11RenderTargetView** ppcRenderTargetViews = nullptr;
						if (m_pppcRenderTargetViews_DX11) ppcRenderTargetViews = (ID3D11RenderTargetView**)*m_pppcRenderTargetViews_DX11; else ppcRenderTargetViews = nullptr;
						ID3D11DepthStencilView* pcDepthStencilView = nullptr;
						if (m_ppcDepthStencilView_DX11) pcDepthStencilView = (ID3D11DepthStencilView*)*m_ppcDepthStencilView_DX11; else pcDepthStencilView = nullptr;

						// call method
						OMSetRenderTargets(dwNumViews, (IUnknown**)ppcRenderTargetViews, (IUnknown*)pcDepthStencilView);
					}
					// ensure D3D11 is set
					m_eD3DVersion = D3DVersion::Direct3D11;
					return nullptr;
			}
			return nullptr;
		case INTERFACE_IDXGISWAPCHAIN:
			if (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)
				Present((IDXGISwapChain*)pThis);
			return nullptr;
	}
	return nullptr;
}

/**
* Incoming Present() call.
* Handle the check time counter.
***/
void StereoSplitter::Present(IDXGISwapChain* pcSwapChain)
{
	if (!m_eD3DVersion) return;

	// create new stereo twin render targets or assign from clipboard
	for (DWORD i = 0; i < m_dwNewStereoTwinRenderTargets; i++)
	{
		// get monitored render target a twin needs to be found for
		DWORD dwIndex = (DWORD)m_apcMonitoredViews.size() - (m_dwNewStereoTwinRenderTargets - i);

		// m_apcMonitoredViews[dwIndex] = ID3D10/ID3D11RenderTargetView or ID3D10/ID3D11DepthStencilView
		IUnknown* pcView = m_apcMonitoredViews[dwIndex];

		// all possible view types enumeration
		enum D3DViewType
		{
			D3D10_RENDER_TARGET_VIEW,
			D3D10_DEPTH_STENCIL_VIEW,
			D3D10_SHADER_RESOURCE_VIEW,
			D3D11_RENDER_TARGET_VIEW,
			D3D11_DEPTH_STENCIL_VIEW,
			D3D11_SHADER_RESOURCE_VIEW,
		} eD3DViewType = D3DViewType::D3D10_RENDER_TARGET_VIEW;

		// determine the type of the view
		LPVOID pvObject = nullptr;
		switch (m_eD3DVersion)
		{
			case Direct3D10:
				if (FAILED(pcView->QueryInterface(__uuidof(ID3D10RenderTargetView), &pvObject)))
				{
					if (FAILED(pcView->QueryInterface(__uuidof(ID3D10DepthStencilView), &pvObject)))
					{
						if (FAILED(pcView->QueryInterface(__uuidof(ID3D10ShaderResourceView), &pvObject)))
						{
							OutputDebugString(L"StereoSplitterDX10: Failed to determine d3d view type !");
						}
						else
						{
							eD3DViewType = D3DViewType::D3D10_SHADER_RESOURCE_VIEW;
							((IUnknown*)pvObject)->Release();
						}
					}
					else
					{
						eD3DViewType = D3DViewType::D3D10_DEPTH_STENCIL_VIEW;
						((IUnknown*)pvObject)->Release();
					}
				}
				else
				{
					eD3DViewType = D3DViewType::D3D10_RENDER_TARGET_VIEW;
					((IUnknown*)pvObject)->Release();
				}
				break;
			case Direct3D11:
				if (FAILED(pcView->QueryInterface(__uuidof(ID3D11RenderTargetView), &pvObject)))
				{
					if (FAILED(pcView->QueryInterface(__uuidof(ID3D11DepthStencilView), &pvObject)))
					{
						if (FAILED(pcView->QueryInterface(__uuidof(ID3D11ShaderResourceView), &pvObject)))
						{
							OutputDebugString(L"StereoSplitterDX10: Failed to determine d3d view type !");
						}
						else
						{
							eD3DViewType = D3DViewType::D3D11_SHADER_RESOURCE_VIEW;
							((IUnknown*)pvObject)->Release();
						}
					}
					else
					{
						eD3DViewType = D3DViewType::D3D11_DEPTH_STENCIL_VIEW;
						((IUnknown*)pvObject)->Release();
					}
				}
				else
				{
					eD3DViewType = D3DViewType::D3D11_RENDER_TARGET_VIEW;
					((IUnknown*)pvObject)->Release();
				}
				break;
		}

		// all possible description union
		union
		{
			D3D10_RENDER_TARGET_VIEW_DESC sDescRT10;
			D3D10_DEPTH_STENCIL_VIEW_DESC sDescDS10;
			D3D10_SHADER_RESOURCE_VIEW_DESC sDescSR10;
			D3D11_RENDER_TARGET_VIEW_DESC sDescRT11;
			D3D11_DEPTH_STENCIL_VIEW_DESC sDescDS11;
			D3D11_SHADER_RESOURCE_VIEW_DESC sDescSR11;
		};
		if (pcView)
		{
			// get description
			switch (eD3DViewType)
			{
				case D3D10_RENDER_TARGET_VIEW:
					((ID3D10RenderTargetView*)pcView)->GetDesc(&sDescRT10);
					break;
				case D3D10_DEPTH_STENCIL_VIEW:
					((ID3D10DepthStencilView*)pcView)->GetDesc(&sDescDS10);
					break;
				case D3D10_SHADER_RESOURCE_VIEW:
					break;
				case D3D11_RENDER_TARGET_VIEW:
					((ID3D11RenderTargetView*)pcView)->GetDesc(&sDescRT11);
					break;
				case D3D11_DEPTH_STENCIL_VIEW:
					((ID3D11DepthStencilView*)pcView)->GetDesc(&sDescDS11);
					break;
				case D3D11_SHADER_RESOURCE_VIEW:
					break;
				default:
					break;
			}

			// target pointers, both tex + view
			union
			{
				ID3D10Resource* pcStereoTwinTexture10;
				ID3D11Resource* pcStereoTwinTexture11;
			};
			pcStereoTwinTexture10 = nullptr;
			union
			{
				ID3D10View* pcStereoTwinView10;
				ID3D11View* pcStereoTwinView11;
			};
			pcStereoTwinView10 = nullptr;

			// loop through the clipboard, try to find useable stereo twin view
			int nIndex = 0;
			auto it = apcStereoTwinRenderTargetViewClipboard.begin();
			while (it < apcStereoTwinRenderTargetViewClipboard.end())
			{
				// all possible description union
				union
				{
					D3D10_RENDER_TARGET_VIEW_DESC sDescClipboardRT10;
					D3D10_DEPTH_STENCIL_VIEW_DESC sDescClipboardDS10;
					D3D10_SHADER_RESOURCE_VIEW_DESC sDescClipboardSR10;
					D3D11_RENDER_TARGET_VIEW_DESC sDescClipboardRT11;
					D3D11_DEPTH_STENCIL_VIEW_DESC sDescClipboardDS11;
					D3D11_SHADER_RESOURCE_VIEW_DESC sDescClipboardSR11;
				};

				// get description, compare to monitored view
				bool bViewCompare = false;
				if (*it)
				{
					switch (eD3DViewType)
					{
						case D3D10_RENDER_TARGET_VIEW:
							((ID3D10RenderTargetView*)(*it))->GetDesc(&sDescClipboardRT10);
							if (memcmp(&sDescClipboardRT10, &sDescRT10, sizeof(D3D10_RENDER_TARGET_VIEW_DESC)) == NULL)
								bViewCompare = true;
							break;
						case D3D10_DEPTH_STENCIL_VIEW:
							((ID3D10DepthStencilView*)(*it))->GetDesc(&sDescClipboardDS10);
							if (memcmp(&sDescClipboardDS10, &sDescDS10, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC)) == NULL)
								bViewCompare = true;
							break;
						case D3D10_SHADER_RESOURCE_VIEW:
							break;
						case D3D11_RENDER_TARGET_VIEW:
							if (SUCCEEDED((*it)->QueryInterface(__uuidof(ID3D11RenderTargetView), &pvObject)))
							{
								((IUnknown*)pvObject)->Release();
								((ID3D11RenderTargetView*)(*it))->GetDesc(&sDescClipboardRT11);
								if (memcmp(&sDescClipboardRT11, &sDescRT11, sizeof(D3D11_RENDER_TARGET_VIEW_DESC)) == NULL)
									bViewCompare = true;
							}
							break;
						case D3D11_DEPTH_STENCIL_VIEW:
							if (SUCCEEDED((*it)->QueryInterface(__uuidof(ID3D11DepthStencilView), &pvObject)))
							{
								((IUnknown*)pvObject)->Release();
								((ID3D11DepthStencilView*)(*it))->GetDesc(&sDescClipboardDS11);
								if (memcmp(&sDescClipboardDS11, &sDescDS11, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC)) == NULL)
									bViewCompare = true;
							}
							break;
						case D3D11_SHADER_RESOURCE_VIEW:
							break;
						default:
							break;
					}
				}

				// take this target if all matches
				if (bViewCompare)
				{
					// first, set the new stereo twins from the clipboard
					// note that ID3D10View can be ID3D11View (same for ID3D10/ID3D11Resource)
					pcStereoTwinView10 = (ID3D10View*)apcStereoTwinRenderTargetViewClipboard[nIndex];
					pcStereoTwinTexture10 = (ID3D10Resource*)m_apcStereoTwinRenderTextureClipboard[nIndex];

					// erase render target iterator for both clipboard vectors
					apcStereoTwinRenderTargetViewClipboard.erase(apcStereoTwinRenderTargetViewClipboard.begin() + nIndex);
					m_apcStereoTwinRenderTextureClipboard.erase(m_apcStereoTwinRenderTextureClipboard.begin() + nIndex);

					// end loop
					it = apcStereoTwinRenderTargetViewClipboard.end();
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
			if (pcStereoTwinView10 == nullptr)
			{
				// note that D3D10_RTV_DIMENSION == D3D11_RTV_DIMENSION
				// and D3D10_DSV_DIMENSION == D3D11_DSV_DIMENSION but has
				// no 3D texture type as D3D10_RTV_DIMENSION (sDesc.ViewDimension)
				switch (sDescRT10.ViewDimension)
				{
					case D3D10_RTV_DIMENSION_UNKNOWN:
						break;
					case D3D10_RTV_DIMENSION_BUFFER:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE1D:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE1DARRAY:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2D:
						switch (eD3DViewType)
						{
							case D3D10_RENDER_TARGET_VIEW:
								if (true)
								{
									// get device
									ID3D10Device* pcDevice = nullptr;
									pcSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&pcDevice);
									if (pcDevice)
									{
										// get the texture
										ID3D10Texture2D* pcResource = nullptr;
										((ID3D10RenderTargetView*)pcView)->GetResource((ID3D10Resource**)&pcResource);
										if (pcResource)
										{
											// get the description and create the twin texture
											D3D10_TEXTURE2D_DESC sDesc;
											pcResource->GetDesc(&sDesc);
											pcResource->Release();
											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D10Texture2D**)&pcStereoTwinTexture10)))
												OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
											else
											{
												// create twin render target view
												if (FAILED(pcDevice->CreateRenderTargetView((ID3D10Resource*)pcStereoTwinTexture10, &sDescRT10, (ID3D10RenderTargetView**)&pcStereoTwinView10)))
													OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view !");
											}
										}
										pcDevice->Release();
									}
								}
								break;
							case D3D10_DEPTH_STENCIL_VIEW:
								if (true)
								{
									// get device
									ID3D10Device* pcDevice = nullptr;
									pcSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&pcDevice);
									if (pcDevice)
									{
										// get the texture
										ID3D10Texture2D* pcResource = nullptr;
										((ID3D10DepthStencilView*)pcView)->GetResource((ID3D10Resource**)&pcResource);
										if (pcResource)
										{
											// get the description and create the twin texture
											D3D10_TEXTURE2D_DESC sDesc;
											pcResource->GetDesc(&sDesc);
											pcResource->Release();
											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D10Texture2D**)&pcStereoTwinTexture10)))
												OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
											else
											{
												// create twin render target view
												if (FAILED(pcDevice->CreateDepthStencilView((ID3D10Resource*)pcStereoTwinTexture10, &sDescDS10, (ID3D10DepthStencilView**)&pcStereoTwinView10)))
													OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view !");
											}
										}
										pcDevice->Release();
									}
								}
								break;
							case D3D10_SHADER_RESOURCE_VIEW:
								break;
							case D3D11_RENDER_TARGET_VIEW:
								if (true)
								{
									// get device
									ID3D11Device* pcDevice = nullptr;
									pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
									if (pcDevice)
									{
										// get the texture
										ID3D11Texture2D* pcResource = nullptr;
										((ID3D11RenderTargetView*)pcView)->GetResource((ID3D11Resource**)&pcResource);
										if (pcResource)
										{
											// get the description and create the twin texture
											D3D11_TEXTURE2D_DESC sDesc;
											pcResource->GetDesc(&sDesc);
											pcResource->Release();

											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture10)))
												OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
											else
											{
												// create twin render target view
												if (FAILED(pcDevice->CreateRenderTargetView((ID3D11Resource*)pcStereoTwinTexture11, &sDescRT11, (ID3D11RenderTargetView**)&pcStereoTwinView11)))
													OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view !");
											}
										}
										pcDevice->Release();
									}
								}
								break;
							case D3D11_DEPTH_STENCIL_VIEW:
								if (true)
								{
									// get device
									ID3D11Device* pcDevice = nullptr;
									pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
									if (pcDevice)
									{
										// get the texture
										ID3D11Texture2D* pcResource = nullptr;
										((ID3D11DepthStencilView*)pcView)->GetResource((ID3D11Resource**)&pcResource);
										if (pcResource)
										{
											// get the description and create the twin texture
											D3D11_TEXTURE2D_DESC sDesc;
											pcResource->GetDesc(&sDesc);
											pcResource->Release();

											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture10)))
												OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
											else
											{
												// create twin render target view
												if (FAILED(pcDevice->CreateDepthStencilView((ID3D11Resource*)pcStereoTwinTexture11, &sDescDS11, (ID3D11DepthStencilView**)&pcStereoTwinView11)))
													OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view !");
											}
										}
										pcDevice->Release();
									}
								}
								break;
							case D3D11_SHADER_RESOURCE_VIEW:
								break;
							default:
								break;
						}
					case D3D10_RTV_DIMENSION_TEXTURE2DARRAY:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2DMS:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY:
						break;
					case D3D10_RTV_DIMENSION_TEXTURE3D:
						break;
					default:
						break;
				}
			}

			// add to stereo twin render targets
			m_apcStereoTwinViews.push_back(pcStereoTwinView10);
			m_apcStereoTwinTextures.push_back(pcStereoTwinTexture10);

			// update control
			m_bControlUpdate = true;
		}
		else
		{
			// code failure, null pointer render target
			OutputDebugString(L"VireioStereoSplitterDx10 code failure ! Null pointer monitored view !");
			m_apcStereoTwinViews.push_back(nullptr);
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
				apcStereoTwinRenderTargetViewClipboard.push_back(m_apcStereoTwinViews[nIndex]);
				m_apcStereoTwinRenderTextureClipboard.push_back(m_apcStereoTwinTextures[nIndex]);

				// erase render target iterator for all 3 vectors (original, stereo twin, stereo twin texture)
				m_apcMonitoredViews.erase(m_apcMonitoredViews.begin() + nIndex);
				m_apcStereoTwinViews.erase(m_apcStereoTwinViews.begin() + nIndex);
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
	/*m_pcActiveBackBufferSurface = nullptr;
	pcDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pcActiveBackBufferSurface);

	if (m_pcActiveBackBufferSurface)
	{
	// check wether back buffer is actually monitored
	int nIndex = CheckIfMonitored(m_pcActiveBackBufferSurface);
	if (nIndex == -1)
	{
	// not monitored, so start
	MonitorView(m_pcActiveBackBufferSurface);
	}
	else
	{
	// set twin surface if twin created
	if (nIndex < (int)m_apcStereoTwinViews.size())
	m_pcActiveStereoTwinBackBuffers = m_apcStereoTwinViews[nIndex];
	else
	// set twin surface to null meanwhile
	m_pcActiveStereoTwinBackBuffers = NULL;
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
	if (m_pcActiveStereoTwinBackBuffers)
	pcDevice->StretchRect(m_pcActiveStereoTwinBackBuffers, NULL, m_pcStereoOutputSurfaceRight, NULL, D3DTEXF_NONE);
	}
	}

	// release back buffer
	m_pcActiveBackBufferSurface->Release();
	}
	else
	OutputDebugString(L"Vireio Stereo Splitter : No primary swap chain found.");
	*/

	// set present() bool to true
	m_bPresent = true;
}

/**
* Incoming OMSetRenderTargets() call (D3D10+).
* Set render targets internally, handle render target monitoring.
***/
void StereoSplitter::OMSetRenderTargets(UINT NumViews, IUnknown *const *ppRenderTargetViews, IUnknown *pDepthStencilView)
{
	// set the number of render targets set
	// for DX10 and DX11 all render targets above this number are set to NULL
	m_dwRenderTargetNumber = (DWORD)NumViews;

	// set the render target internally
	if (NumViews < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
	{
		// set NULL manually, otherwise just set the render target :
		if (!ppRenderTargetViews)
		{
			// set render targets and the twins			
			for (std::vector<IUnknown*>::size_type i = 0; i < m_apcActiveRenderTargetViews.size(); i++)
			{
				m_apcActiveRenderTargetViews[i] = NULL;
				m_apcActiveStereoTwinViews[i] = NULL;
			}
		}
		else
		{
			// set render targets and the twins			
			for (std::vector<IUnknown*>::size_type i = 0; i < m_apcActiveRenderTargetViews.size(); i++)
			{
				if (i < (size_t)NumViews)
				{
					// set zero if zero
					if (!ppRenderTargetViews[i])
					{
						m_apcActiveRenderTargetViews[i] = NULL;
						m_apcActiveStereoTwinViews[i] = NULL;
					}
					else
					{
						// check wether this render target is actually monitored
						int nIndex = CheckIfMonitored(ppRenderTargetViews[i]);
						if (nIndex == -1)
						{
							// not monitored, so start
							MonitorView((IUnknown*)ppRenderTargetViews[i]);

							// set twin surface to null meanwhile
							m_apcActiveStereoTwinViews[i] = NULL;
						}
						else
						{
							// set twin surface if twin created
							if (nIndex < (int)m_apcStereoTwinViews.size())
								m_apcActiveStereoTwinViews[i] = m_apcStereoTwinViews[nIndex];
							else
								// set twin surface to null meanwhile
								m_apcActiveStereoTwinViews[i] = NULL;
						}
					}
				}
				else
				{
					m_apcActiveRenderTargetViews[i] = NULL;
					m_apcActiveStereoTwinViews[i] = NULL;
				}
			}
		}
	}

	// now, the back buffer... set NULL manually, otherwise just set the render target :
	if (!pDepthStencilView)
		m_pcActiveDepthStencilView = NULL;
	else
		m_pcActiveDepthStencilView = pDepthStencilView;

	// set NULL twin if NULL
	if (!pDepthStencilView)
		m_pcActiveStereoTwinDepthStencilView = NULL;
	else
	{
		// check wether this depth stencil is actually monitored.. 
		int nIndex = CheckIfMonitored(pDepthStencilView);
		if (nIndex == -1)
		{
			// not monitored, so start
			MonitorView(pDepthStencilView);

			// set twin surface to null meanwhile
			m_pcActiveStereoTwinDepthStencilView = NULL;
		}
		else
		{
			// set twin surface if twin created
			if (nIndex < (int)m_apcStereoTwinViews.size())
				m_pcActiveStereoTwinDepthStencilView = m_apcStereoTwinViews[nIndex];
			else
				// set twin surface to null meanwhile
				m_pcActiveStereoTwinDepthStencilView = NULL;
		}
	}

	m_bControlUpdate = true;
}


/**
* Incoming SetTexture() call.
***/
/*void StereoSplitter::SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage,IDirect3DBaseTexture9* pcTexture)
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
/*void StereoSplitter::Clear(IDirect3DDevice9* pcDevice, DWORD dwCount, D3DRECT *pRects, DWORD dwFlags, D3DCOLOR sColor, float fZ, DWORD dwStencil)
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
/*void StereoSplitter::DrawPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwStartVertex, UINT dwPrimitiveCount)
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
/*void StereoSplitter::DrawIndexedPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, INT nBaseVertexIndex, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwStartIndex, UINT dwPrimCount)
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
/*void StereoSplitter::DrawPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwPrimitiveCount, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
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
/*void StereoSplitter::DrawIndexedPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwPrimitiveCount, CONST void* pIndexData, D3DFORMAT eIndexDataFormat, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride)
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


/**
* Index of the monitored surface in m_apcMonitoredViews, -1 if not monitored.
***/
int StereoSplitter::CheckIfMonitored(IUnknown* pcView)
{
	auto it = std::find(m_apcMonitoredViews.begin(), m_apcMonitoredViews.end(), (IUnknown*)pcView);
	if (it != m_apcMonitoredViews.end())
	{
		// set check time counter if this render target is actually monitored
		auto index = it - m_apcMonitoredViews.begin();
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
void StereoSplitter::MonitorView(IUnknown* pcView)
{
	// add new render target + check time frame constant
	m_apcMonitoredViews.push_back(pcView);
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
bool StereoSplitter::SetDrawingSide(RenderPosition eSide)
{
	// Already on the correct eye
	/*if (eSide == m_eCurrentRenderingSide)
	return true;

	// state block was applied ?
	if (m_bApply)
	{
	// verify the render targets
	for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_apcActiveRenderTargetViews.size(); i++)
	{
	// get the render target for this index
	LPDIRECT3DSURFACE9 pcRenderTarget;
	pcDevice->GetRenderTarget((DWORD)i, &pcRenderTarget);

	// is this render target stored ?
	if (pcRenderTarget != m_apcActiveRenderTargetViews[i])
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
	if (m_apcActiveRenderTargetViews[i])
	hr = pcDevice->SetRenderTarget(i, m_apcActiveRenderTargetViews[i]);
	else
	hr = pcDevice->SetRenderTarget(i, NULL);
	}
	else
	{
	if (i < m_apcActiveStereoTwinViews.size())
	{
	if (m_apcActiveStereoTwinViews[i])
	hr = pcDevice->SetRenderTarget(i, m_apcActiveStereoTwinViews[i]);
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

	/*// switch depth stencil to new side
	if (m_pcActiveDepthStencilView != NULL) {
	if (eSide == RenderPosition::Left)
	hr = pcDevice->SetDepthStencilSurface(m_pcActiveDepthStencilView);
	else
	{
	if (m_pcActiveStereoTwinDepthStencil)
	hr = pcDevice->SetDepthStencilSurface(m_pcActiveStereoTwinDepthStencil);
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
	*/
	return true;
}

