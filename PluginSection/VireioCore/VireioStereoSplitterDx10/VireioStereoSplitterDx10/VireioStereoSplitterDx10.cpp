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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

#include"VireioStereoSplitterDx10.h"

#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_ID3D11DEVICECONTEXT                                        11
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_ID3D10DEVICE_DRAWINDEXED                                      8
#define METHOD_ID3D10DEVICE_DRAW                                             9
#define METHOD_ID3D10DEVICE_DRAWINDEXEDINSTANCED                             14
#define METHOD_ID3D10DEVICE_DRAWINSTANCED                                    15
#define METHOD_ID3D10DEVICE_OMSETRENDERTARGETS                               24
#define METHOD_ID3D10DEVICE_DRAWAUTO                                         28
#define METHOD_ID3D10DEVICE_COPYSUBRESOURCEREGION                            32
#define METHOD_ID3D10DEVICE_COPYRESOURCE                                     33
#define METHOD_ID3D10DEVICE_UPDATESUBRESOURCE                                34
#define METHOD_ID3D10DEVICE_CLEARRENDERTARGETVIEW                            35
#define METHOD_ID3D10DEVICE_CLEARDEPTHSTENCILVIEW                            36
#define METHOD_ID3D10DEVICE_OMGETRENDERTARGETS                               56
#define METHOD_ID3D10DEVICE_CLEARSTATE                                       69
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED                               12
#define METHOD_ID3D11DEVICECONTEXT_DRAW                                      13
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED                      20
#define METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED                             21
#define METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS                        33
#define METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS 34
#define METHOD_ID3D11DEVICECONTEXT_DRAWAUTO                                  38
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT              39
#define METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT                     40
#define METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION                     46
#define METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE                              47
#define METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE                         48
#define METHOD_ID3D11DEVICECONTEXT_CLEARRENDERTARGETVIEW                     50
#define METHOD_ID3D11DEVICECONTEXT_CLEARDEPTHSTENCILVIEW                     53
#define METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS                        89
#define METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS 90
#define METHOD_ID3D11DEVICECONTEXT_CLEARSTATE                                110
#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

/**
* Constructor.
***/
StereoSplitter::StereoSplitter() : AQU_Nodus(),
m_apcActiveRenderTargetViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveStereoTwinViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_apcActiveStereoTwinTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_pcActiveStereoTwinDepthStencilView(nullptr),
m_pcActiveDepthStencilView(nullptr),
m_pcActiveBackBuffer10(nullptr),
m_pcActiveStereoTwinBackBuffer10(nullptr),
m_nBackBufferIndex(-1),
m_apcMonitoredViews(0, nullptr),
m_apcStereoTwinViews(0, nullptr),
m_apcStereoTwinTextures(0, nullptr),
m_anMonitoredRenderTargetsCheckTimeCounter(0, 0),
m_apcStereoTwinRenderTargetViewClipboard(0, nullptr),
m_apcStereoTwinRenderTextureClipboard(0, nullptr),
m_dwNewStereoTwinRenderTargets(0),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_bPresent(false),
m_dwTextureNumber(0),
m_dwRenderTargetNumber(0),
m_eD3DVersion(D3DVersion::NotDefined),
m_nChecktimeFrameConstant(30),                          /**< Set this constant to 30 frames, later we should be able to change this value on the node. ***/
m_pcBackBufferView(nullptr),
m_pcVertexShader10(nullptr),
m_pcPixelShader10(nullptr),
m_pcVertexLayout10(nullptr),
m_pcVertexBuffer10(nullptr),
m_pcConstantBufferDirect10(nullptr),
m_eCurrentRenderingSide(RenderPosition::Left),
m_appcActiveConstantBuffers11(nullptr)
{
	m_pcTexView10[0] = nullptr;
	m_pcTexView10[1] = nullptr;
	m_pcTex10[0] = nullptr;
	m_pcTex10[1] = nullptr;
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
	for (int i = 0; i < (int)m_apcStereoTwinViews.size(); i++)
		SAFE_RELEASE(m_apcStereoTwinViews[i]);
	for (int i = 0; i < (int)m_apcStereoTwinTextures.size(); i++)
		SAFE_RELEASE(m_apcStereoTwinTextures[i]);
	for (int i = 0; i < (int)m_apcStereoTwinRenderTargetViewClipboard.size(); i++)
		SAFE_RELEASE(m_apcStereoTwinRenderTargetViewClipboard[i]);
	for (int i = 0; i < (int)m_apcStereoTwinRenderTextureClipboard.size(); i++)
		SAFE_RELEASE(m_apcStereoTwinRenderTextureClipboard[i]);

	SAFE_RELEASE(m_pcTexView10[0]);
	SAFE_RELEASE(m_pcTexView10[1]);
	SAFE_RELEASE(m_pcTex10[0]);
	SAFE_RELEASE(m_pcTex10[1]);

	SAFE_RELEASE(m_pcVertexShader10);
	SAFE_RELEASE(m_pcPixelShader10);
	SAFE_RELEASE(m_pcVertexLayout10);
	SAFE_RELEASE(m_pcVertexBuffer10);
	SAFE_RELEASE(m_pcBackBufferView);
	SAFE_RELEASE(m_pcConstantBufferDirect10);
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
			TextOut(hdcImage, 50, nY, L"NumRTVs", 7); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppRenderTargetViews_DX11", 24); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView_DX11", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"pRenderTargetView_DX10", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"pRenderTargetView_DX11", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"ColorRGBA", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView__DX10", 23); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView__DX11", 23); nY += 64;
			TextOut(hdcImage, 50, nY, L"ClearFlags", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"Depth", 5); nY += 64;
			TextOut(hdcImage, 50, nY, L"Stencil", 7); nY += 64;
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
			wsprintf(szBuffer, L"%u", (UINT)m_apcStereoTwinRenderTargetViewClipboard.size());
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
		case ppRenderTargetViews_DX10:
			return L"ppRenderTargetViews_DX10";
		case pDepthStencilView_DX10:
			return L"pDepthStencilView_DX10";
		case ppRenderTargetViews_DX11:
			return L"ppRenderTargetViews_DX11";
		case pDepthStencilView_DX11:
			return L"pDepthStencilView_DX11";
		case NumRTVs:
			return L"NumRTVs";
		case ppRenderTargetViewsUAV_DX11:
			return L"ppRenderTargetViews_DX11";
		case pDepthStencilViewUAV_DX11:
			return L"pDepthStencilView_DX11";
		case pRenderTargetView_DX10:
			return L"pRenderTargetView_DX10";
		case pRenderTargetView_DX11:
			return L"pRenderTargetView_DX11";
		case ColorRGBA:
			return L"ColorRGBA";
		case pDepthStencilViewCDS_DX10:
			return L"pDepthStencilView__DX10";
		case pDepthStencilViewCDS_DX11:
			return L"pDepthStencilView__DX11";
		case ClearFlags:
			return L"ClearFlags";
		case Depth:
			return L"Depth";
		case Stencil:
			return L"Stencil";
		case ppActiveConstantBuffers_DX10_VertexShader:
			return L"ppConstantBuffers_DX10_VS";
		case ppActiveConstantBuffers_DX11_VertexShader:
			return L"ppConstantBuffers_DX11_VS";
		case ppActiveConstantBuffers_DX10_PixelShader:
			return L"ppConstantBuffers_DX10_PS";
		case ppActiveConstantBuffers_DX11_PixelShader:
			return L"ppConstantBuffers_DX11_PS";
		default:
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
		case ppRenderTargetViews_DX10:
			return PPNT_ID3D10RENDERTARGETVIEW;
		case pDepthStencilView_DX10:
			return PNT_ID3D10DEPTHSTENCILVIEW;
		case ppRenderTargetViews_DX11:
			return PPNT_ID3D11RENDERTARGETVIEW;
		case pDepthStencilView_DX11:
			return PNT_ID3D11DEPTHSTENCILVIEW;
		case NumRTVs:
			return UINT_PLUG_TYPE;
		case ppRenderTargetViewsUAV_DX11:
			return PPNT_ID3D11RENDERTARGETVIEW;
		case pDepthStencilViewUAV_DX11:
			return PNT_ID3D11DEPTHSTENCILVIEW;
		case pRenderTargetView_DX10:
			return PNT_ID3D10RENDERTARGETVIEW_TYPE;
		case pRenderTargetView_DX11:
			return PNT_ID3D11RENDERTARGETVIEW_TYPE;
		case ColorRGBA:
			return VECTOR4F_PLUG_TYPE;
		case pDepthStencilViewCDS_DX10:
			return PNT_ID3D10DEPTHSTENCILVIEW;
		case pDepthStencilViewCDS_DX11:
			return PNT_ID3D11DEPTHSTENCILVIEW;
		case ClearFlags:
			return UINT_PLUG_TYPE;
		case Depth:
			return FLOAT_PLUG_TYPE;
		case Stencil:
			return BYTE_PLUG_TYPE;
		case ppActiveConstantBuffers_DX10_VertexShader:
			return PPNT_ID3D10BUFFER_PLUG_TYPE;
		case ppActiveConstantBuffers_DX11_VertexShader:
			return PPNT_ID3D11BUFFER_PLUG_TYPE;
		case ppActiveConstantBuffers_DX10_PixelShader:
			return PPNT_ID3D10BUFFER_PLUG_TYPE;
		case ppActiveConstantBuffers_DX11_PixelShader:
			return PPNT_ID3D11BUFFER_PLUG_TYPE;
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
			m_pdwNumViews = (DWORD*)pData;                                             /** Number of render targets to bind. **/
			break;
		case ppRenderTargetViews_DX10:
			m_pppcRenderTargetViews_DX10 = (IUnknown***)pData;                         /** Pointer to an array of render targets (see ID3D10RenderTargetView) to bind to the device. **/
			break;
		case pDepthStencilView_DX10:
			m_ppcDepthStencilView_DX10 = (IUnknown**)pData;                            /** Pointer to a depth-stencil view (see ID3D10DepthStencilView) to bind to the device.**/
			break;
		case ppRenderTargetViews_DX11:
			m_pppcRenderTargetViews_DX11 = (IUnknown***)pData;                         /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
			break;
		case pDepthStencilView_DX11:
			m_ppcDepthStencilView_DX11 = (IUnknown**)pData;                            /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
			break;
		case NumRTVs:
			m_pdwNumRTVs = (DWORD*)pData;                                              /** Number of render targets to bind. **/
			break;
		case ppRenderTargetViewsUAV_DX11:
			m_pppcRenderTargetViewsUAV_DX11 = (IUnknown***)pData;                      /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
			break;
		case pDepthStencilViewUAV_DX11:
			m_ppcDepthStencilViewUAV_DX11 = (IUnknown**)pData;                         /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
			break;
		case pRenderTargetView_DX10:
			m_ppcRenderTargetView_DX10 = (ID3D10RenderTargetView**)pData;              /** Pointer to the render target. */
			break;
		case pRenderTargetView_DX11:
			m_ppcRenderTargetView_DX11 = (ID3D11RenderTargetView**)pData;              /** Pointer to the render target. */
			break;
		case ColorRGBA:
			m_ppfColorRGBA = (float**)pData;                                           /** A 4-component array that represents the color to fill the render target with. */
			break;
		case pDepthStencilViewCDS_DX10:
			m_ppcDepthStencilViewClear_DX10 = (ID3D10DepthStencilView**)pData;         /** Pointer to the depth stencil to be cleared. */
			break;
		case pDepthStencilViewCDS_DX11:
			m_ppcDepthStencilViewClear_DX11 = (ID3D11DepthStencilView**)pData;         /** Pointer to the depth stencil to be cleared. */
			break;
		case ClearFlags:
			m_pdwClearFlags = (UINT*)pData;                                            /** Identify the type of data to clear */
			break;
		case Depth:
			m_pfDepth = (FLOAT*)pData;                                                 /** Clear the depth buffer with this value. This value will be clamped between 0 and 1. */
			break;
		case Stencil:
			m_pchStencil = (UINT8*)pData;                                               /** Clear the stencil buffer with this value. */
			break;
		case ppActiveConstantBuffers_DX10_VertexShader:
			break;
		case ppActiveConstantBuffers_DX11_VertexShader:
			m_appcActiveConstantBuffers11 = (ID3D11Buffer***)pData;
			break;
		case ppActiveConstantBuffers_DX10_PixelShader:
			break;
		case ppActiveConstantBuffers_DX11_PixelShader:
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
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWAUTO) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARRENDERTARGETVIEW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARDEPTHSTENCILVIEW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARSTATE))
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
	// set node behavior to "double call" for this method
	// node that this is only supported by drawing methods
	nProvokerIndex = -1;

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
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAW:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWAUTO:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION:
					// TODO !! check if render target / monitored texture, in case update twin at next present call
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE:
					// TODO !! check if render target / monitored texture, in case update twin at next present call
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE:
					// TODO !! check if render target / monitored texture, in case update twin at next present call
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_CLEARRENDERTARGETVIEW:
					if (m_bPresent)
					{
						ID3D11RenderTargetView* pcRenderTargetView = nullptr;
						if (m_ppcRenderTargetView_DX11) pcRenderTargetView = (ID3D11RenderTargetView*)*m_ppcRenderTargetView_DX11; else return nullptr;
						float* pfColor;
						if (m_ppfColorRGBA) pfColor = (float*)*m_ppfColorRGBA; else return nullptr;

						if (pcRenderTargetView)
						{
							// check wether this depth stencil is actually monitored.. 
							int nIndex = CheckIfMonitored((IUnknown*)pcRenderTargetView);
							if (nIndex == -1)
								// not monitored, so start
								MonitorView(pcRenderTargetView);
							else
							{
								if (nIndex < (int)m_apcStereoTwinViews.size())
								{
									if (m_apcStereoTwinViews[nIndex])
										// clear twin render target
										((ID3D11DeviceContext*)pThis)->ClearRenderTargetView((ID3D11RenderTargetView*)m_apcStereoTwinViews[nIndex], (const FLOAT*)pfColor);
								}
							}
						}
					}
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_CLEARDEPTHSTENCILVIEW:
					if (m_bPresent)
					{
						ID3D11DepthStencilView* pcDepthStencilView = nullptr;
						if (m_ppcDepthStencilViewClear_DX11) pcDepthStencilView = (ID3D11DepthStencilView*)*m_ppcDepthStencilViewClear_DX11; else return nullptr;
						UINT dwClearFlags;
						if (m_pdwClearFlags) dwClearFlags = (UINT)*m_pdwClearFlags; else return nullptr;
						FLOAT fDepth;
						if (m_pfDepth) fDepth = (FLOAT)*m_pfDepth; else return nullptr;
						UINT8 chStencil;
						if (m_pchStencil) chStencil = (UINT8)*m_pchStencil; else return nullptr;

						if (pcDepthStencilView)
						{
							// check wether this depth stencil is actually monitored.. 
							int nIndex = CheckIfMonitored((IUnknown*)pcDepthStencilView);
							if (nIndex == -1)
								// not monitored, so start
								MonitorView(pcDepthStencilView);
							else
							{
								if (nIndex < (int)m_apcStereoTwinViews.size())
								{
									if (m_apcStereoTwinViews[nIndex])
										// clear twin render target
										((ID3D11DeviceContext*)pThis)->ClearDepthStencilView((ID3D11DepthStencilView*)m_apcStereoTwinViews[nIndex], dwClearFlags, fDepth, chStencil);
								}
							}
						}
					}
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_CLEARSTATE:
					OMSetRenderTargets(0, NULL, NULL);
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					// TODO !! TEXTURES
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
				case METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS:
					if (m_bPresent)
					{
						// get data
						DWORD dwNumViews = 0;
						if (m_pdwNumRTVs) dwNumViews = *m_pdwNumRTVs; else return nullptr;
						ID3D11RenderTargetView** ppcRenderTargetViews = nullptr;
						if (m_pppcRenderTargetViewsUAV_DX11) ppcRenderTargetViews = (ID3D11RenderTargetView**)*m_pppcRenderTargetViewsUAV_DX11; else ppcRenderTargetViews = nullptr;
						ID3D11DepthStencilView* pcDepthStencilView = nullptr;
						if (m_ppcDepthStencilViewUAV_DX11) pcDepthStencilView = (ID3D11DepthStencilView*)*m_ppcDepthStencilViewUAV_DX11; else pcDepthStencilView = nullptr;

						// call method
						OMSetRenderTargets(dwNumViews, (IUnknown**)ppcRenderTargetViews, (IUnknown*)pcDepthStencilView);
					}
					// ensure D3D11 is set
					m_eD3DVersion = D3DVersion::Direct3D11;
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS:
					// if the app tries to get the render targets ensure the left render side is set
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					return nullptr;
				case METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS:
					// if the app tries to get the render targets ensure the left render side is set
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
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

#pragma region create or assign
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
					OutputDebugString(L"NotImplemented: D3D10_SHADER_RESOURCE_VIEW");
					break;
				case D3D11_RENDER_TARGET_VIEW:
					((ID3D11RenderTargetView*)pcView)->GetDesc(&sDescRT11);
					break;
				case D3D11_DEPTH_STENCIL_VIEW:
					((ID3D11DepthStencilView*)pcView)->GetDesc(&sDescDS11);
					break;
				case D3D11_SHADER_RESOURCE_VIEW:
					OutputDebugString(L"NotImplemented: D3D11_SHADER_RESOURCE_VIEW");
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
			auto it = m_apcStereoTwinRenderTargetViewClipboard.begin();
			while (it < m_apcStereoTwinRenderTargetViewClipboard.end())
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
					pcStereoTwinView10 = (ID3D10View*)m_apcStereoTwinRenderTargetViewClipboard[nIndex];
					pcStereoTwinTexture10 = (ID3D10Resource*)m_apcStereoTwinRenderTextureClipboard[nIndex];

					// erase render target iterator for both clipboard vectors
					m_apcStereoTwinRenderTargetViewClipboard.erase(m_apcStereoTwinRenderTargetViewClipboard.begin() + nIndex);
					m_apcStereoTwinRenderTextureClipboard.erase(m_apcStereoTwinRenderTextureClipboard.begin() + nIndex);

					// end loop
					it = m_apcStereoTwinRenderTargetViewClipboard.end();
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
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_UNKNOWN");
						break;
					case D3D10_RTV_DIMENSION_BUFFER:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_BUFFER");
						break;
					case D3D10_RTV_DIMENSION_TEXTURE1D:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE1D");
						break;
					case D3D10_RTV_DIMENSION_TEXTURE1DARRAY:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE1DARRAY");
						switch (eD3DViewType)
						{
							case D3D10_RENDER_TARGET_VIEW:
								OutputDebugString(L"NotImplemented: D3D10_RENDER_TARGET_VIEW");
								break;
							case D3D10_DEPTH_STENCIL_VIEW:
								OutputDebugString(L"NotImplemented: D3D10_DEPTH_STENCIL_VIEW");
								break;
							case D3D10_SHADER_RESOURCE_VIEW:
								OutputDebugString(L"NotImplemented: D3D10_SHADER_RESOURCE_VIEW");
								break;
							case D3D11_RENDER_TARGET_VIEW:
								OutputDebugString(L"NotImplemented: D3D11_RENDER_TARGET_VIEW");
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
										ID3D11Texture1D* pcResource = nullptr;
										((ID3D11DepthStencilView*)pcView)->GetResource((ID3D11Resource**)&pcResource);
										if (pcResource)
										{
											// get the description and create the twin texture
											D3D11_TEXTURE1D_DESC sDesc;
											pcResource->GetDesc(&sDesc);
											pcResource->Release();

											if (FAILED(pcDevice->CreateTexture1D(&sDesc, NULL, (ID3D11Texture1D**)&pcStereoTwinTexture11)))
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
								OutputDebugString(L"NotImplemented: D3D11_SHADER_RESOURCE_VIEW");
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}
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
								OutputDebugString(L"NotImplemented: D3D10_SHADER_RESOURCE_VIEW");
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

											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture11)))
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

											if (FAILED(pcDevice->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture11)))
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
								OutputDebugString(L"NotImplemented: D3D11_SHADER_RESOURCE_VIEW");
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2DARRAY:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE2DARRAY");
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2DMS:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE2DMS");
						break;
					case D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY");
						break;
					case D3D10_RTV_DIMENSION_TEXTURE3D:
						OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE3D");
						break;
					default:
						OutputDebugString(L"NotImplemented: UNKNOWN DIMENSION");
						break;
				}
			}

			if (!pcStereoTwinView10) OutputDebugString(L"StereoSplitter: Could not create twin view !");
			if (!pcStereoTwinTexture10) OutputDebugString(L"StereoSplitter: Could not create twin texture !");

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
#pragma endregion

#pragma region check time counter
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
				m_apcStereoTwinRenderTargetViewClipboard.push_back(m_apcStereoTwinViews[nIndex]);
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
#pragma endregion

#pragma region get back buffer
	// finally, provide pointers to the left and right render target
	switch (m_eD3DVersion)
	{
		case Direct3D10:
			break;
		case Direct3D11:
		{
			// get back buffer
			ID3D11Texture2D* pcBackBuffer = NULL;
			pcSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
			if (pcBackBuffer)
			{
				if (pcBackBuffer != m_pcActiveBackBuffer11)
				{
					SAFE_RELEASE(m_pcActiveBackBuffer11);
					SAFE_RELEASE(m_pcTexView11[0]);
					SAFE_RELEASE(m_pcTexView11[1]);
					SAFE_RELEASE(m_pcTex11[0]);
					SAFE_RELEASE(m_pcTex11[1]);

					// get view from monitored views, actually the current back buffer should be monitored
					m_nBackBufferIndex = -1;
					for (size_t nI = 0; nI < m_apcMonitoredViews.size(); nI++)
					{
						ID3D11Resource* pcResource;
						((ID3D11View*)m_apcMonitoredViews[nI])->GetResource(&pcResource);
						if (pcResource == pcBackBuffer)
							m_nBackBufferIndex = (int)nI;
					}

					// set stereo twin back buffer and create output textures
					if (m_nBackBufferIndex > -1)
					{
						m_pcActiveBackBuffer11 = pcBackBuffer;
						m_pcActiveStereoTwinBackBuffer11 = (ID3D11Texture2D*)m_apcStereoTwinTextures[m_nBackBufferIndex];

						// get device
						ID3D11Device* pcDevice = nullptr;
						pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
						if (pcDevice)
						{
							// create target textures by description
							D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
							ZeroMemory(&sDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
							D3D11_TEXTURE2D_DESC sDescRT;
							pcBackBuffer->GetDesc(&sDescRT);
							sDescRT.BindFlags = D3D11_BIND_SHADER_RESOURCE;

							if (FAILED(pcDevice->CreateTexture2D(&sDescRT, NULL, &m_pcTex11[0])))
								OutputDebugString(L"VireioStereoSplitter: Failed to create Texture.");
							if (FAILED(pcDevice->CreateTexture2D(&sDescRT, NULL, &m_pcTex11[1])))
								OutputDebugString(L"VireioStereoSplitter: Failed to create Texture.");

							// ...and the views
							sDesc.Format = sDescRT.Format;
							sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
							sDesc.Texture2D.MostDetailedMip = 0;
							sDesc.Texture2D.MipLevels = 1;

							if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[0], &sDesc, &m_pcTexView11[0]))))
								OutputDebugString(L"VireioStereoSplitter: Failed to create texture view!");
							if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[1], &sDesc, &m_pcTexView11[1]))))
								OutputDebugString(L"VireioStereoSplitter: Failed to create texture view!");
							pcDevice->Release();
						}
					}
					else
					{
						OutputDebugString(L"VireioStereoSplitter: Back buffer not monitored !");
						pcBackBuffer->Release();
					}
				}
				else
					pcBackBuffer->Release();
			}
			else
			{
				// no backbuffer present ? should not be...
				OutputDebugString(L"VireioStereoSplitter: No back buffer present !");
				if (m_pcActiveBackBuffer11) m_pcActiveBackBuffer11->Release();
				m_nBackBufferIndex = -1;
				m_pcActiveBackBuffer11 = NULL;
				m_pcActiveStereoTwinBackBuffer11 = NULL;
			}
		}
		break;
		default:
			break;
	}
#pragma endregion

#pragma region draw (optionally)
	// draw stereo target to screen (optionally)
	if (true) // TODO !! OPTION TO SET DRAWING ON/OFF
	{
		switch (m_eD3DVersion)
		{
			case Direct3D10:
				break;
			case Direct3D11:
			{
				if ((m_nBackBufferIndex == -1) || (!m_pcActiveBackBuffer11)) break;

				// get device and context
				ID3D11Device* pcDevice = nullptr;
				ID3D11DeviceContext* pcContext = nullptr;
				if (FAILED(GetDeviceAndContext(pcSwapChain, &pcDevice, &pcContext)))
				{
					// release frame texture+view
					if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
					if (pcContext) { pcContext->Release(); pcContext = nullptr; }
					return;
				}

				// update the output textures
				if ((m_pcTex11[0]) && (m_pcActiveBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[0], (ID3D11Resource*)m_pcActiveBackBuffer11);
				if ((m_pcTex11[1]) && (m_pcActiveStereoTwinBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[1], (ID3D11Resource*)m_pcActiveStereoTwinBackBuffer11);

				// get the viewport
				UINT dwNumViewports = 1;
				D3D11_VIEWPORT psViewport[16];
				pcContext->RSGetViewports(&dwNumViewports, psViewport);

				// backup all states
				D3DX11_STATE_BLOCK sStateBlock;
				CreateStateblock(pcContext, &sStateBlock);

				// clear all states, set targets
				pcContext->ClearState();

				// set first active render target - the stored back buffer
				pcContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&m_apcMonitoredViews[m_nBackBufferIndex], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
				pcContext->RSSetViewports(dwNumViewports, psViewport);

				// create all bool
				bool bAllCreated = true;

				// create vertex shader
				if (!m_pcVertexShader11)
				{
					if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
						bAllCreated = false;
				}
				// create pixel shader
				if (!m_pcPixelShader11)
				{
					if (FAILED(CreateSimplePixelShader(pcDevice, &m_pcPixelShader11)))
						bAllCreated = false;
				}
				// Create vertex buffer
				if (!m_pcVertexBuffer11)
				{
					if (FAILED(CreateFullScreenVertexBuffer(pcDevice, &m_pcVertexBuffer11)))
						bAllCreated = false;
				}
				// create constant buffer
				if (!m_pcConstantBufferDirect11)
				{
					if (FAILED(CreateMatrixConstantBuffer(pcDevice, &m_pcConstantBufferDirect11)))
						bAllCreated = false;
				}


				if (bAllCreated)
				{
					// Clear ? TODO !! DELETE... NO CLEAR FOR FULLSCREEN RENDERING
					if (false)
					{
						float ClearColor[4] = { 0.6f, 0.125f, 0.3f, 0.0f }; // red,green,blue,alpha
						if (m_apcMonitoredViews[m_nBackBufferIndex])
							pcContext->ClearRenderTargetView((ID3D11RenderTargetView*)m_apcMonitoredViews[m_nBackBufferIndex], ClearColor);
					}
					//pcContext->ClearDepthStencilView((ID3D11DepthStencilView*)m_pcActiveDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

					for (int nEye = 0; nEye < 2; nEye++)
					{
						// Set the input layout
						pcContext->IASetInputLayout(m_pcVertexLayout11);

						// Set vertex buffer
						UINT stride = sizeof(TexturedVertex);
						UINT offset = 0;
						pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);

						// Set constant buffer, first update it... scale and translate the left and right image
						D3DXMATRIX sScale;
						D3DXMatrixScaling(&sScale, 0.5f, 1.0f, 1.0f);
						D3DXMATRIX sTrans;
						if (nEye == 0)
							D3DXMatrixTranslation(&sTrans, -0.5f, 0.0f, 0.0f);
						else
							D3DXMatrixTranslation(&sTrans, 0.5f, 0.0f, 0.0f);
						D3DXMatrixTranspose(&sTrans, &sTrans);
						D3DXMATRIX sProj;
						D3DXMatrixMultiply(&sProj, &sTrans, &sScale);
						pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);
						pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);

						// Set primitive topology
						pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						// set texture
						pcContext->PSSetShaderResources(0, 1, &m_pcTexView11[nEye]);

						// set shaders
						pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
						pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

						// Render a triangle
						pcContext->Draw(6, 0);
					}
				}

				// set back device
				ApplyStateblock(pcContext, &sStateBlock);

				if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
				if (pcContext) { pcContext->Release(); pcContext = nullptr; }
			}
			break;
			default:
				break;
		}
	}
	else
	{
		// get device and context
		ID3D11Device* pcDevice = nullptr;
		ID3D11DeviceContext* pcContext = nullptr;
		if (FAILED(GetDeviceAndContext(pcSwapChain, &pcDevice, &pcContext)))
		{
			// release frame texture+view
			if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
			if (pcContext) { pcContext->Release(); pcContext = nullptr; }
			return;
		}

		// update the output textures
		if ((m_pcTex11[0]) && (m_pcActiveBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[0], (ID3D11Resource*)m_pcActiveBackBuffer11);
		if ((m_pcTex11[1]) && (m_pcActiveBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[1], (ID3D11Resource*)m_pcActiveStereoTwinBackBuffer11);

		if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
		if (pcContext) { pcContext->Release(); pcContext = nullptr; }
	}
#pragma endregion

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

	// drawing side is automatically set to "left"
	m_eCurrentRenderingSide = RenderPosition::Left;

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
						// set the target
						m_apcActiveRenderTargetViews[i] = ppRenderTargetViews[i];

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

	// now, the back buffer... set NULL manually, otherwise just set 
	if (!pDepthStencilView)
	{
		m_pcActiveDepthStencilView = NULL;
		m_pcActiveStereoTwinDepthStencilView = NULL;
	}
	else
	{
		m_pcActiveDepthStencilView = pDepthStencilView;

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
			{
				// set twin surface to null meanwhile
				m_pcActiveStereoTwinDepthStencilView = NULL;
			}
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
* DX10 method !!
*
* @param pcDevice The D3D 10 device.
* @param eSide The destination side to set.
* @return True if change succeeded, false if it fails.
* Attempting to switch to a side when that side is already the active side will return true without making any changes.
***/
bool StereoSplitter::SetDrawingSide(ID3D10Device* pcDevice, RenderPosition eSide)
{
	// Already on the correct eye
	if (eSide == m_eCurrentRenderingSide)
		return true;

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	m_eCurrentRenderingSide = eSide;

	// switch render targets to new eSide
	bool renderTargetChanged = false;
	HRESULT hr = S_OK;
	if (eSide == RenderPosition::Left)
		pcDevice->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D10RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D10DepthStencilView*)m_pcActiveDepthStencilView);
	else
		pcDevice->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D10RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D10DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

	// if a non-fullsurface viewport is active and a rendertarget changed we need to reapply the viewport - TODO !!
	/*if (renderTargetChanged && !m_bActiveViewportIsDefault) {
	BaseDirect3DDevice9::SetViewport(&m_LastViewportSet);
	}*/

	// switch textures to new side
	/*for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_dwTextureNumber; i++)
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
	// the rest of the code from the original method from the Vireio 2.x.x D3DProxyDevice*/

	return true;
}

/**
* Switches rendering to which ever side is specified by side.
* Use to specify the side that you want to draw to.
*
* Derives D3DProxyDevice::setDrawingSide() from the Vireio 2.x.x driver code.
*
* DX11 method !!
*
* @param pcContext The D3D 11 device context.
* @param eSide The destination side to set.
* @return True if change succeeded, false if it fails.
* Attempting to switch to a side when that side is already the active side will return true without making any changes.
***/
bool StereoSplitter::SetDrawingSide(ID3D11DeviceContext* pcContext, RenderPosition eSide)
{
	// Already on the correct eye
	if (eSide == m_eCurrentRenderingSide)
		return true;

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	m_eCurrentRenderingSide = eSide;

	// switch render targets to new eSide
	HRESULT hr = S_OK;
	if (eSide == RenderPosition::Left)
		pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
	else
		pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

	// switch textures to new side
	/*for(std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_dwTextureNumber; i++)
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
	}*/

	// switch constant buffers
	if (m_appcActiveConstantBuffers11)
		if (*m_appcActiveConstantBuffers11)
		{
			for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
				// has the buffer private data set ?
				if ((*m_appcActiveConstantBuffers11)[dwIndex])
				{
					// test for private data
					Vireio_Constant_Buffer_Private_Data sPrivateData;
					UINT dwDataSize = sizeof(sPrivateData);
					((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
					if (!dwDataSize)
					{
						// no data, so set new.. first get active vertex shader
						ID3D11VertexShader* pcShader = nullptr;
						pcContext->VSGetShader(&pcShader, NULL, NULL);
						if (pcShader)
						{
							Vireio_Shader_Private_Data sPrivateDataShader;
							dwDataSize = sizeof(sPrivateDataShader);
							pcShader->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateDataShader);

							if (dwDataSize)
							{
								// set the private constant buffer data
								sPrivateData.dwHash = sPrivateDataShader.dwHash;
								sPrivateData.dwIndex = sPrivateDataShader.dwIndex;
								sPrivateData.dwIndexBuffer = dwIndex;
								((*m_appcActiveConstantBuffers11)[dwIndex])->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);
							}
							pcShader->Release();
						}
					}

					// get the private data interfaces and set the current drawing side
					ID3D11Buffer* pcBuffer = nullptr;
					UINT dwSize = sizeof(pcBuffer);
					if (eSide == RenderPosition::Left)
						((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSize, (void*)&pcBuffer);
					else
						((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

					// TODO !! eventually set all buffers in one call
					if (pcBuffer)
						pcContext->VSSetConstantBuffers(dwIndex, 1, &pcBuffer);
				}
		}

	return true;
}

