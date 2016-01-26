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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

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
#define METHOD_ID3D11DEVICECONTEXT_PSSETSHADERRESOURCES                       8
#define METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED                               12
#define METHOD_ID3D11DEVICECONTEXT_DRAW                                      13
#define METHOD_ID3D11DEVICECONTEXT_MAP                                       14
#define METHOD_ID3D11DEVICECONTEXT_UNMAP                                     15
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
#define METHOD_ID3D11DEVICECONTEXT_PSGETSHADERRESOURCES                      73
#define METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS                        89
#define METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS 90
#define METHOD_ID3D11DEVICECONTEXT_CLEARSTATE                                110
#define METHOD_ID3D11DEVICECONTEXT_DISPATCH  41
#define METHOD_ID3D11DEVICECONTEXT_DISPATCHINDIRECT  42
#define METHOD_ID3D11DEVICECONTEXT_CSSETSHADERRESOURCES 67
#define METHOD_ID3D11DEVICECONTEXT_CSSETUNORDEREDACCESSVIEWS  68
#define METHOD_ID3D11DEVICECONTEXT_CSSETSHADER  69
#define METHOD_ID3D11DEVICECONTEXT_CSSETSAMPLERS  70
#define METHOD_ID3D11DEVICECONTEXT_CSSETCONSTANTBUFFERS  71
#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

#define TODO_ADD_BOOL_HERE                                                   true

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

/**
* Constructor.
***/
StereoSplitter::StereoSplitter() : AQU_Nodus(),
m_apcActiveRenderTargetViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveStereoTwinViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullptr),
m_apcActiveTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_apcActiveStereoTwinTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_apcActiveCSTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_apcActiveStereoTwinCSTextures(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr),
m_apcActiveCSConstantBuffers(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT*2, nullptr),
m_pcActiveStereoTwinDepthStencilView(nullptr),
m_pcActiveDepthStencilView(nullptr),
m_pcActiveBackBuffer10(nullptr),
m_pcActiveStereoTwinBackBuffer10(nullptr),
m_pcActiveStereoTwinBackBufferView10(nullptr),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_bPresent(false),
m_eBackBufferVerified(NotVerified),
m_dwTextureNumber(0),
m_dwRenderTargetNumber(0),
m_eD3DVersion(D3DVersion::NotDefined),
m_pcBackBufferView(nullptr),
m_pcVertexShader10(nullptr),
m_pcPixelShader10(nullptr),
m_pcVertexLayout10(nullptr),
m_pcVertexBuffer10(nullptr),
m_pcConstantBufferDirect10(nullptr),
m_eCurrentRenderingSide(RenderPosition::Left),
m_appcActiveConstantBuffers11(nullptr),
m_peDrawingSide(nullptr),
m_dwVerifyConstantBuffers(0),
m_ppcResource(nullptr),
m_pdwSubresource(nullptr),
m_ppcResource_Unmap(nullptr),
m_pdwSubresource_Unmap(nullptr)
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
		m_hBitmapControl = CreateCompatibleBitmap(hdc, 1024, 4000);
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
		SetRect(&rc, 0, 0, 1024, 4000);
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
			TextOut(hdcImage, 50, nY, L"UAVStartSlot", 12); nY += 64;
			TextOut(hdcImage, 50, nY, L"NumUAVs", 7); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppUnorderedAccessViews", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"pUAVInitialCounts", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"pRenderTargetView_DX10", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"pRenderTargetView_DX11", 22); nY += 64;
			TextOut(hdcImage, 50, nY, L"ColorRGBA", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView__DX10", 23); nY += 64;
			TextOut(hdcImage, 50, nY, L"pDepthStencilView__DX11", 23); nY += 64;
			TextOut(hdcImage, 50, nY, L"ClearFlags", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"Depth", 5); nY += 64;
			TextOut(hdcImage, 50, nY, L"Stencil", 7); nY += 64;
			TextOut(hdcImage, 50, nY, L"StartSlot", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"NumSRVs", 7); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppShaderResourceViews_DX10", 26); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppShaderResourceViews_DX11", 26); nY += 64;
			TextOut(hdcImage, 50, nY, L"pResource", 9); nY += 64;
			TextOut(hdcImage, 50, nY, L"Subresource", 11); nY += 64;
			TextOut(hdcImage, 50, nY, L"pResource_Unmap", 15); nY += 64;
			TextOut(hdcImage, 50, nY, L"Subresource_Unmap", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"ThreadGroupCountX", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"ThreadGroupCountY", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"ThreadGroupCountZ", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"pBufferForArgs", 14); nY += 64;
			TextOut(hdcImage, 50, nY, L"AlignedByteOffsetForArgs", 24); nY += 64;
			TextOut(hdcImage, 50, nY, L"StartSlot_CS", 12); nY += 64;
			TextOut(hdcImage, 50, nY, L"NumViews_CS", 11); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppShaderResourceViews", 21); nY += 64;
			TextOut(hdcImage, 50, nY, L"StartSlot_CSUAV", 15); nY += 64;
			TextOut(hdcImage, 50, nY, L"NumUAVs_CS", 10); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppUnorderedAccessViews_CS", 25); nY += 64;
			TextOut(hdcImage, 50, nY, L"pUAVInitialCounts_CS", 20); nY += 64;
			TextOut(hdcImage, 50, nY, L"StartSlot_CSCB", 14); nY += 64;
			TextOut(hdcImage, 50, nY, L"NumBuffers", 10); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppConstantBuffers", 17); nY += 64;
			TextOut(hdcImage, 50, nY, L"Stereo Drawing Side", 19); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppActiveConstantBuffers_DX10_VS", 31); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppActiveConstantBuffers_DX11_VS", 31); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppActiveConstantBuffers_DX10_PS", 31); nY += 64;
			TextOut(hdcImage, 50, nY, L"ppActiveConstantBuffers_DX11_PS", 31); nY += 64;
			TextOut(hdcImage, 50, nY, L"Verify Constant Buffers", 23); nY += 64;
			TextOut(hdcImage, 600, nY, L"Left Texture", 12); nY += 64;
			TextOut(hdcImage, 600, nY, L"Right Texture", 13); nY += 128;

			// output constant buffer verification counter
			TextOut(hdcImage, 50, nY, L"Verify Constant Buffers : ", 26);
			wsprintf(szBuffer, L"%u", (UINT)m_dwVerifyConstantBuffers);
			szBuffer[11] = 0;
			int nLen = (int)wcslen(szBuffer); if (nLen > 10) nLen = 10;
			TextOut(hdcImage, 720, nY, szBuffer, nLen); nY += 64;

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
		case StartSlot:
			return L"StartSlot";
		case NumSRVs:
			return L"NumSRVs";
		case ppShaderResourceViews_DX10:
			return L"ppShaderResourceViews_DX10";
		case ppShaderResourceViews_DX11:
			return L"ppShaderResourceViews_DX11";
		case pResource:
			return L"pResource";
		case Subresource:
			return L"Subresource";
		case pResource_Unmap:
			return L"pResource_Unmap";
		case Subresource_Unmap:
			return L"Subresource_Unmap";
		case eDrawingSide:
			return L"Stereo Drawing Side";
		case ppActiveConstantBuffers_DX10_VertexShader:
			return L"ppConstantBuffers_DX10_VS";
		case ppActiveConstantBuffers_DX11_VertexShader:
			return L"ppConstantBuffers_DX11_VS";
		case ppActiveConstantBuffers_DX10_PixelShader:
			return L"ppConstantBuffers_DX10_PS";
		case ppActiveConstantBuffers_DX11_PixelShader:
			return L"ppConstantBuffers_DX11_PS";
		case dwVerifyConstantBuffers:
			return L"Verify Constant Buffers";
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
			return NOD_Plugtype::AQU_UINT;
		case ppRenderTargetViews_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10RENDERTARGETVIEW;
		case pDepthStencilView_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10DEPTHSTENCILVIEW;
		case ppRenderTargetViews_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11RENDERTARGETVIEW;
		case pDepthStencilView_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11DEPTHSTENCILVIEW;
		case NumRTVs:
			return NOD_Plugtype::AQU_UINT;
		case ppRenderTargetViewsUAV_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11RENDERTARGETVIEW;
		case pDepthStencilViewUAV_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11DEPTHSTENCILVIEW;
		case UAVStartSlot:
			return NOD_Plugtype::AQU_UINT;
		case NumUAVs:
			return NOD_Plugtype::AQU_UINT;
		case ppUnorderedAccessViews:
			return NOD_Plugtype::AQU_PPNT_ID3D11UNORDEREDACCESSVIEW;
		case pUAVInitialCounts:
			return NOD_Plugtype::AQU_PNT_UINT;
		case pRenderTargetView_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10RENDERTARGETVIEW;
		case pRenderTargetView_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11RENDERTARGETVIEW;
		case ColorRGBA:
			return NOD_Plugtype::AQU_VECTOR4F;
		case pDepthStencilViewCDS_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10DEPTHSTENCILVIEW;
		case pDepthStencilViewCDS_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11DEPTHSTENCILVIEW;
		case ClearFlags:
			return NOD_Plugtype::AQU_UINT;
		case Depth:
			return NOD_Plugtype::AQU_FLOAT;
		case Stencil:
			return NOD_Plugtype::AQU_BYTE;
		case StartSlot:
			return NOD_Plugtype::AQU_UINT;
		case NumSRVs:
			return NOD_Plugtype::AQU_UINT;
		case ppShaderResourceViews_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10SHADERRESOURCEVIEW;
		case ppShaderResourceViews_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11SHADERRESOURCEVIEW;
		case pResource:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case Subresource:
			return NOD_Plugtype::AQU_UINT;
		case pResource_Unmap:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case Subresource_Unmap:
			return NOD_Plugtype::AQU_UINT;
		case ThreadGroupCountX:
			return NOD_Plugtype::AQU_UINT;
		case ThreadGroupCountY:
			return NOD_Plugtype::AQU_UINT;
		case ThreadGroupCountZ:
			return NOD_Plugtype::AQU_UINT;
		case pBufferForArgs:
			return NOD_Plugtype::AQU_PNT_ID3D11BUFFER;
		case AlignedByteOffsetForArgs:
			return NOD_Plugtype::AQU_UINT;
		case StartSlot_CS:
			return NOD_Plugtype::AQU_UINT;
		case NumViews_CS:
			return NOD_Plugtype::AQU_UINT;
		case ppShaderResourceViews:
			return NOD_Plugtype::AQU_PPNT_ID3D11SHADERRESOURCEVIEW;
		case StartSlot_CSUAV:
			return NOD_Plugtype::AQU_UINT;
		case NumUAVs_CS:
			return NOD_Plugtype::AQU_UINT;
		case ppUnorderedAccessViews_CS:
			return NOD_Plugtype::AQU_PPNT_ID3D11UNORDEREDACCESSVIEW;
		case pUAVInitialCounts_CS:
			return NOD_Plugtype::AQU_PNT_UINT;
		case StartSlot_CSCB:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case eDrawingSide:
			return NOD_Plugtype::AQU_INT;
		case ppActiveConstantBuffers_DX10_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppActiveConstantBuffers_DX11_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case ppActiveConstantBuffers_DX10_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppActiveConstantBuffers_DX11_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case dwVerifyConstantBuffers:
			return NOD_Plugtype::AQU_UINT;
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
		case UAVStartSlot:
			m_pdwUAVStartSlot = (UINT*)pData;
			break;
		case NumUAVs:
			m_pdwNumUAVs = (UINT*)pData;
			break;
		case ppUnorderedAccessViews:
			m_pppcUnorderedAccessViews = (ID3D11UnorderedAccessView***)pData;
			break;
		case pUAVInitialCounts:
			m_ppdwUAVInitialCounts = (UINT**)pData;
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
		case StartSlot:
			m_pdwStartSlot = (UINT*)pData;
			break;
		case NumSRVs:
			m_pdwNumViewsSRVs = (UINT*)pData;
			break;
		case ppShaderResourceViews_DX10:
			m_pppcShaderResourceViews10 = (ID3D10ShaderResourceView***)pData;
			break;
		case ppShaderResourceViews_DX11:
			m_pppcShaderResourceViews11 = (ID3D11ShaderResourceView***)pData;
			break;
		case pResource:
			m_ppcResource = (ID3D11Resource**)pData;
			break;
		case Subresource:
			m_pdwSubresource = (UINT*)pData;
			break;
		case pResource_Unmap:
			m_ppcResource_Unmap = (ID3D11Resource**)pData;
			break;
		case Subresource_Unmap:
			m_pdwSubresource_Unmap = (UINT*)pData;
			break;
		case ThreadGroupCountX:
			m_pdwThreadGroupCountX = (UINT*)pData;
			break;
		case ThreadGroupCountY:
			m_pdwThreadGroupCountY = (UINT*)pData;
			break;
		case ThreadGroupCountZ:
			m_pdwThreadGroupCountZ = (UINT*)pData;
			break;
		case pBufferForArgs:
			m_ppcBufferForArgs = (ID3D11Buffer**)pData;
			break;
		case AlignedByteOffsetForArgs:
			m_pdwpAlignedByteOffsetForArgs = (UINT*)pData;
			break;
		case StartSlot_CS:
			m_pdwStartSlot_CS = (UINT*)pData;
			break;
		case NumViews_CS:
			m_pdwNumViews_CS = (UINT*)pData;
			break;
		case ppShaderResourceViews:
			m_pppcShaderResourceViews = (ID3D11ShaderResourceView***)pData;
			break;
		case StartSlot_CSUAV:
			m_pdwStartSlot_CSUAV = (UINT*)pData;
			break;
		case NumUAVs_CS:
			m_pdwNumUAVs_CS = (UINT*)pData;
			break;
		case ppUnorderedAccessViews_CS:
			m_pppcUnorderedAccessViews_CS = (ID3D11UnorderedAccessView***)pData;
			break;
		case pUAVInitialCounts_CS:
			m_ppdwUAVInitialCounts_CS = (UINT**)pData;
			break;
		case StartSlot_CSCB:
			m_pdwStartSlot_CSCB = (UINT*)pData;
			break;
		case NumBuffers:
			m_pdwNumBuffers = (UINT*)pData;
			break;
		case ppConstantBuffers:
			m_pppcConstantBuffers = (ID3D11Buffer***)pData;
			break;
		case eDrawingSide:
			m_peDrawingSide = (RenderPosition*)pData;
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
		case dwVerifyConstantBuffers:
			m_pdwVerifyConstantBuffers = (UINT*)pData;
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
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_PSSETSHADERRESOURCES) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWAUTO) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARRENDERTARGETVIEW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARDEPTHSTENCILVIEW) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_PSGETSHADERRESOURCES) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_MAP) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_UNMAP) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CLEARSTATE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DISPATCH) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_DISPATCHINDIRECT) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CSSETSHADERRESOURCES) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CSSETUNORDEREDACCESSVIEWS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CSSETSHADER) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CSSETSAMPLERS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_CSSETCONSTANTBUFFERS))
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
	if (m_bPresent)
		nProvokerIndex |= AQU_PluginFlags::DoubleCallFlag;

	switch (eD3DInterface)
	{
#pragma region ID3D10DEVICE
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
						OMSetRenderTargets((IUnknown*)pThis, dwNumViews, (IUnknown**)ppcRenderTargetViews, (IUnknown*)pcDepthStencilView);
					}
					// ensure D3D10 is set
					m_eD3DVersion = D3DVersion::Direct3D10;
					return nullptr;
			}
			return nullptr;
#pragma endregion
#pragma region ID3D11DEVICECONTEXT
		case INTERFACE_ID3D11DEVICECONTEXT:
			switch (eD3DMethod)
			{
#pragma region PSSETSHADERRESOURCES
				case METHOD_ID3D11DEVICECONTEXT_PSSETSHADERRESOURCES:
					if (m_bPresent)
					{
						// ensure D3D11 is set
						m_eD3DVersion = D3DVersion::Direct3D11;

						// verify node pointers
						if (!m_pdwStartSlot) return nullptr;
						if (!m_pdwNumViewsSRVs) return nullptr;
						if (!m_pppcShaderResourceViews11) return nullptr;

						// set to NULL ?
						bool bSetNull = false;
						if (!*m_pppcShaderResourceViews11)
							bSetNull = true;
						else if (!**m_pppcShaderResourceViews11)
							bSetNull = true;
						if (bSetNull)
						{
							// loop through new resource views
							for (UINT dwIndex = 0; dwIndex < *m_pdwNumViewsSRVs; dwIndex++)
							{
								// get and verify index
								DWORD dwIndexActive = *m_pdwStartSlot + dwIndex;
								if (dwIndexActive < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
								{
									// set shader resource view for both sides
									m_apcActiveTextures[dwIndexActive] = nullptr;
									m_apcActiveStereoTwinTextures[dwIndexActive] = nullptr;
								}
								// replace the call
								if (m_eCurrentRenderingSide == RenderPosition::Left)
									((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActiveTextures[*m_pdwStartSlot]);
								else
									((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActiveStereoTwinTextures[*m_pdwStartSlot]);

								// method replaced, immediately return
								nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
							}
							return nullptr;
						}

						// invalid call ? validate !
						if (*m_pdwStartSlot + *m_pdwNumViewsSRVs > D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) return nullptr;

						// set texture number
						if (*m_pdwStartSlot + *m_pdwNumViewsSRVs > m_dwTextureNumber)
							m_dwTextureNumber = (DWORD)(*m_pdwStartSlot + *m_pdwNumViewsSRVs);

						// delete all if zero ? should have been done only in ClearState().. maybe we can delete this
						if (!m_dwTextureNumber)
						{
							// clear texture vectors and number ?
							m_apcActiveTextures = std::vector<IUnknown*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
							m_apcActiveStereoTwinTextures = std::vector<IUnknown*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
						}

						// loop through new resource views
						for (UINT dwIndex = 0; dwIndex < *m_pdwNumViewsSRVs; dwIndex++)
						{
							// get and verify index
							DWORD dwIndexActive = *m_pdwStartSlot + dwIndex;
							if (!((*m_pppcShaderResourceViews11)[dwIndex]))
							{
								// set shader resource view for both sides
								m_apcActiveTextures[dwIndexActive] = nullptr;
								m_apcActiveStereoTwinTextures[dwIndexActive] = nullptr;

								continue;
							}

							// get description
							D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
							((*m_pppcShaderResourceViews11)[dwIndex])->GetDesc(&sDesc);

							// is texture ? 
							if ((sDesc.ViewDimension >= D3D11_SRV_DIMENSION_TEXTURE1D) &&
								(sDesc.ViewDimension <= D3D11_SRV_DIMENSION_TEXTURECUBEARRAY))
							{
								// set active texture
								m_apcActiveTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);

								// get resource
								ID3D11Resource* pcResource;
								((*m_pppcShaderResourceViews11)[dwIndex])->GetResource(&pcResource);
								if (pcResource)
								{
									// get the stereo twin of the resource (texture)
									ID3D11Resource* pcResourceTwin = nullptr;
									UINT dwSize = sizeof(pcResourceTwin);
									pcResource->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
									if (pcResourceTwin)
									{
										// has a shader resource view ?
										ID3D11ShaderResourceView* pcShaderResourceView = nullptr;
										dwSize = sizeof(pcShaderResourceView);
										pcResourceTwin->GetPrivateData(PDIID_ID3D11TextureXD_ShaderResourceView, &dwSize, (void*)&pcShaderResourceView);
										if (pcShaderResourceView)
										{
											// set twin shader resouce view
											m_apcActiveStereoTwinTextures[dwIndexActive] = pcShaderResourceView;

											pcShaderResourceView->Release();
										}
										else
										{
											// set mono
											m_apcActiveStereoTwinTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);

											// add to vector for new views, just if not present
											auto it = std::find(m_apcNewShaderResourceViews11.begin(), m_apcNewShaderResourceViews11.end(), ((*m_pppcShaderResourceViews11)[dwIndex]));
											if (it == m_apcNewShaderResourceViews11.end())
												m_apcNewShaderResourceViews11.push_back(((*m_pppcShaderResourceViews11)[dwIndex]));
										}

										pcResourceTwin->Release();
									}
									else
									{
										// no resource twin ? no render target texture, set mono
										m_apcActiveStereoTwinTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);

										// is this texture to be set to stereo ?
										BOOL bNew = FALSE;
										dwSize = sizeof(BOOL);
										pcResource->GetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, &dwSize, (void*)&bNew);
										if ((dwSize) && (bNew))
										{
											// bool to create a new view here is set to "TRUE", so add to new shader resource views list
											// (for these - currently mono views - ALL stereo fields will be created new)

											// add to vector for new views, just if not present
											auto it = std::find(m_apcNewShaderResourceViews11.begin(), m_apcNewShaderResourceViews11.end(), ((*m_pppcShaderResourceViews11)[dwIndex]));
											if (it == m_apcNewShaderResourceViews11.end())
												m_apcNewShaderResourceViews11.push_back(((*m_pppcShaderResourceViews11)[dwIndex]));

											// set private data to "FALSE"
											bNew = FALSE;
											pcResource->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bNew);
										}
									}

									pcResource->Release();
								}
								else
									// no resource ? shouldn't be.. set shader resource view for both sides
									m_apcActiveStereoTwinTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);
							}
							else
							{
								// set shader resource view for both sides
								m_apcActiveTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);
								m_apcActiveStereoTwinTextures[dwIndexActive] = ((*m_pppcShaderResourceViews11)[dwIndex]);
							}
						}
					}
					else
					{
						// TODO !! catch active resources before first present call
						return nullptr;
					}

					// replace the call
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActiveTextures[*m_pdwStartSlot]);
					else
						((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActiveStereoTwinTextures[*m_pdwStartSlot]);

					// method replaced, immediately return
					nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;

					return nullptr;
#pragma endregion
#pragma region DRAWINDEXED
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXED:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAW
				case METHOD_ID3D11DEVICECONTEXT_DRAW:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAWINDEXEDINSTANCED
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCED:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAWINSTANCED
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCED:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAWAUTO
				case METHOD_ID3D11DEVICECONTEXT_DRAWAUTO:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAWINDEXEDINSTANCEDINDIRECT
				case METHOD_ID3D11DEVICECONTEXT_DRAWINDEXEDINSTANCEDINDIRECT:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region DRAWINSTANCEDINDIRECT
				case METHOD_ID3D11DEVICECONTEXT_DRAWINSTANCEDINDIRECT:
					// switch the drawing side before the second draw call is done
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
					else if (m_eCurrentRenderingSide == RenderPosition::Right)
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);

					return nullptr;
#pragma endregion
#pragma region CLEARRENDERTARGETVIEW
				case METHOD_ID3D11DEVICECONTEXT_CLEARRENDERTARGETVIEW:
					if (m_bPresent)
					{
						// verify node pointers
						if (!m_ppcRenderTargetView_DX11) return nullptr;
						if (!m_ppfColorRGBA) return nullptr;

						if (*m_ppcRenderTargetView_DX11)
						{
							// get the stored private data twin view
							ID3D11RenderTargetView* pcViewStereoTwin = nullptr;
							UINT dwSize = sizeof(pcViewStereoTwin);
							(*m_ppcRenderTargetView_DX11)->GetPrivateData(PDIID_ID3D11RenderTargetView_Stereo_Twin, &dwSize, (void*)&pcViewStereoTwin);

							if (dwSize)
							{
								// clear and release the second view
								((ID3D11DeviceContext*)pThis)->ClearRenderTargetView((ID3D11RenderTargetView*)pcViewStereoTwin, *m_ppfColorRGBA);
								pcViewStereoTwin->Release();
							}
							else
							{
								// TODO !! NO STEREO TWIN PRESENT !! CREATE ONE !!
							}

						}
					}
					return nullptr;
#pragma endregion
#pragma region CLEARDEPTHSTENCILVIEW
				case METHOD_ID3D11DEVICECONTEXT_CLEARDEPTHSTENCILVIEW:
					if (m_bPresent)
					{
						if (!m_ppcDepthStencilViewClear_DX11) return nullptr;
						if (!m_pdwClearFlags) return nullptr;
						if (!m_pfDepth) return nullptr;
						if (!m_pchStencil) return nullptr;

						if (*m_ppcDepthStencilViewClear_DX11)
						{
							// get the stored private data twin view
							ID3D11DepthStencilView* pcViewStereoTwin = nullptr;
							UINT dwSize = sizeof(pcViewStereoTwin);
							(*m_ppcDepthStencilViewClear_DX11)->GetPrivateData(PDIID_ID3D11DepthStencilView_Stereo_Twin, &dwSize, (void*)&pcViewStereoTwin);

							if (dwSize)
							{
								// clear and release the second view
								((ID3D11DeviceContext*)pThis)->ClearDepthStencilView((ID3D11DepthStencilView*)pcViewStereoTwin, *m_pdwClearFlags, *m_pfDepth, *m_pchStencil);
								pcViewStereoTwin->Release();
							}
							else
							{
								// TODO !! NO STEREO TWIN PRESENT !! CREATE ONE !!
							}

						}
					}
					return nullptr;
#pragma endregion
#pragma region CLEARSTATE
				case METHOD_ID3D11DEVICECONTEXT_CLEARSTATE:
					if (m_bPresent)
					{
						// clear texture vectors and number
						m_apcActiveTextures = std::vector<IUnknown*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
						m_apcActiveStereoTwinTextures = std::vector<IUnknown*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
						m_dwTextureNumber = 0;

						// clear render targets
						OMSetRenderTargets((IUnknown*)pThis, 0, NULL, NULL);

						// set render target side to >left<
						SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					}
					return nullptr;
#pragma endregion
#pragma region OMSETRENDERTARGETS
				case METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETS:
					// ensure D3D11 is set
					m_eD3DVersion = D3DVersion::Direct3D11;

					if (m_bPresent)
					{
						// verify node pointers
						if (!m_pdwNumViews) return nullptr;
						if (!m_pppcRenderTargetViews_DX11) return nullptr;
						if (!m_ppcDepthStencilView_DX11) return nullptr;

						// call intern method
						OMSetRenderTargets((IUnknown*)pThis, *m_pdwNumViews, *m_pppcRenderTargetViews_DX11, *m_ppcDepthStencilView_DX11);

						// switch render targets to new side
						if (m_eCurrentRenderingSide == RenderPosition::Left)
							((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
						else
							((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
					}
					return nullptr;
#pragma endregion
#pragma region OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS
				case METHOD_ID3D11DEVICECONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS:
					// ensure D3D11 is set
					m_eD3DVersion = D3DVersion::Direct3D11;

					if (m_bPresent)
					{
						// verify node pointers
						if (!m_pdwNumRTVs) return nullptr;
						if (!m_pppcRenderTargetViewsUAV_DX11) return nullptr;
						if (!m_ppcDepthStencilViewUAV_DX11) return nullptr;
						if (!m_pdwUAVStartSlot) return nullptr;
						if (!m_pdwNumUAVs) return nullptr;
						if (!m_pppcUnorderedAccessViews) return nullptr;
						if (!m_ppdwUAVInitialCounts) return nullptr;

						// call internal methods
						OMSetRenderTargets((IUnknown*)pThis, *m_pdwNumRTVs, *m_pppcRenderTargetViewsUAV_DX11, *m_ppcDepthStencilViewUAV_DX11);
						CSSetUnorderedAccessViews((ID3D11DeviceContext*)pThis, *m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);

						// switch render targets to new side
						if (m_eCurrentRenderingSide == RenderPosition::Left)
							((ID3D11DeviceContext*)pThis)->OMSetRenderTargetsAndUnorderedAccessViews(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView, *m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);
						else
							((ID3D11DeviceContext*)pThis)->OMSetRenderTargetsAndUnorderedAccessViews(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView, *m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
					}
					return nullptr;
#pragma endregion
#pragma region PSGETSHADERRESOURCES
				case METHOD_ID3D11DEVICECONTEXT_PSGETSHADERRESOURCES:
					// if the app tries to get the shader resources ensure the left render side is set
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					return nullptr;
#pragma endregion
#pragma region OMGETRENDERTARGETS
				case METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETS:
					// if the app tries to get the render targets ensure the left render side is set
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					return nullptr;
#pragma endregion
#pragma region OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS
				case METHOD_ID3D11DEVICECONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS:
					// if the app tries to get the render targets ensure the left render side is set
					SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
					return nullptr;
#pragma endregion
#pragma region MAP
				case METHOD_ID3D11DEVICECONTEXT_MAP:
					return nullptr;
#pragma endregion
#pragma region UNMAP
				case METHOD_ID3D11DEVICECONTEXT_UNMAP:
					if ((m_ppcResource_Unmap) && (m_pdwSubresource_Unmap))
					{
						// first, do the ->Unmap() call
						((ID3D11DeviceContext*)pThis)->Unmap(*m_ppcResource_Unmap, *m_pdwSubresource_Unmap);

						// get the resource type
						D3D11_RESOURCE_DIMENSION eResourceDimension;
						(*m_ppcResource_Unmap)->GetType(&eResourceDimension);

						// handle textures and buffers
						ID3D11Resource* pcResourceTwin = nullptr;
						UINT dwSize = sizeof(pcResourceTwin);
						switch (eResourceDimension)
						{
							case D3D11_RESOURCE_DIMENSION_UNKNOWN:
								break;
							case D3D11_RESOURCE_DIMENSION_BUFFER:
								break;
							case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
							case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
							case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
								// get the stereo twin of the resource (texture)
								(*m_ppcResource_Unmap)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);

								// if stereo twin is present, copy the whole texture... TODO ! HANDLE BY SUBRESOURCE INDEX
								if (dwSize)
								{
									// copy the whole texture
									((ID3D11DeviceContext*)pThis)->CopyResource(pcResourceTwin, (*m_ppcResource_Unmap));

									// and release
									pcResourceTwin->Release();
								}
								break;
							default:
								break;
						}

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
					}
					return nullptr;
#pragma endregion
#pragma region DISPATCH
				case METHOD_ID3D11DEVICECONTEXT_DISPATCH:
					//UINT* m_pdwThreadGroupCountX;
					//UINT* m_pdwThreadGroupCountY;
					//UINT* m_pdwThreadGroupCountZ;
					return nullptr;
#pragma endregion
#pragma region DISPATCHINDIRECT
				case METHOD_ID3D11DEVICECONTEXT_DISPATCHINDIRECT:
					//ID3D11Buffer** m_ppcBufferForArgs;
					//UINT* m_pdwpAlignedByteOffsetForArgs;
					return nullptr;
#pragma endregion
#pragma region CSSETSHADERRESOURCES
				case METHOD_ID3D11DEVICECONTEXT_CSSETSHADERRESOURCES:
					if ((m_pdwStartSlot_CS) && (m_pdwNumViews_CS) && (m_pppcShaderResourceViews) && (m_bPresent))
					{
						// TODO !! use same method PSSetShaderResourceViews !!
					}
					return nullptr;
#pragma endregion
#pragma region CSSETUNORDEREDACCESSVIEWS
				case METHOD_ID3D11DEVICECONTEXT_CSSETUNORDEREDACCESSVIEWS:
					if (!m_pdwStartSlot_CSUAV) return nullptr;
					if (!m_pdwNumUAVs_CS) return nullptr;
					if (!m_pppcUnorderedAccessViews_CS) return nullptr;
					if (!m_ppdwUAVInitialCounts_CS) return nullptr;

					// call internal method
					CSSetUnorderedAccessViews((ID3D11DeviceContext*)pThis, *m_pdwStartSlot_CSUAV, *m_pdwNumUAVs_CS, *m_pppcUnorderedAccessViews_CS, *m_ppdwUAVInitialCounts_CS);
					return nullptr;
#pragma endregion
#pragma region CSSETCONSTANTBUFFERS
				case METHOD_ID3D11DEVICECONTEXT_CSSETCONSTANTBUFFERS:
					if ((m_pdwStartSlot_CSCB) && (m_pdwNumBuffers) && (m_pppcConstantBuffers))
					{
						if (!*m_pppcConstantBuffers) return nullptr;
						if (!**m_pppcConstantBuffers) return nullptr;

						// loop through the new buffers
						for (UINT dwIndex = 0; dwIndex < *m_pdwNumBuffers; dwIndex++)
						{
							// get internal index
							UINT dwInternalIndex = dwIndex + *m_pdwStartSlot_CSCB;

							// in range ? 
							if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
							{
								// set buffer internally 
								m_apcActiveCSConstantBuffers[dwInternalIndex] = ((*m_pppcConstantBuffers)[dwIndex]);

								if (m_apcActiveCSConstantBuffers[dwInternalIndex])
								{
									// get private rule index from buffer
									INT nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
									UINT dwDataSizeRulesIndex = sizeof(INT);
									m_apcActiveCSConstantBuffers[dwInternalIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Index, &dwDataSizeRulesIndex, &nRulesIndex);

									// set twin for right side, first get the private data interface
									ID3D11Buffer* pcBuffer = nullptr;
									UINT dwSize = sizeof(pcBuffer);
									m_apcActiveCSConstantBuffers[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

									// stereo buffer and rules index present ?
									if ((pcBuffer) && (dwDataSizeRulesIndex) && (nRulesIndex >= 0))
									{
										// set right buffer as active buffer
										m_apcActiveCSConstantBuffers[dwInternalIndex + D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = pcBuffer;
									}
									else
									{
										// no buffer or no shader rules assigned ? left = right side -> right = left side
										m_apcActiveCSConstantBuffers[dwInternalIndex + D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = m_apcActiveCSConstantBuffers[dwInternalIndex];

										// verify buffer... TODO !! STILL POSSIBLE THAT A CONSTANT BUFFER NEVER ADDRESSED TO THE VERTEX SHADER BUT TO THE COMPUTE SHADER !!!
										/*if ((pcBuffer) && (nRulesIndex == VIREIO_CONSTANT_RULES_NOT_ADDRESSED))
											VerifyConstantBuffer(m_apcActiveConstantBuffers11[dwInternalIndex], dwInternalIndex);*/
									}

									// no stereo buffer present ?
									if (!pcBuffer)
									{
										// create stereo constant buffer, first get device
										ID3D11Device* pcDevice = nullptr;
										m_apcActiveCSConstantBuffers[dwInternalIndex]->GetDevice(&pcDevice);
										if (pcDevice)
										{
											D3D11_BUFFER_DESC sDesc;
											m_apcActiveCSConstantBuffers[dwInternalIndex]->GetDesc(&sDesc);
											CreateStereoConstantBuffer(pcDevice, (ID3D11DeviceContext*)pThis, (ID3D11Buffer*)m_apcActiveCSConstantBuffers[dwInternalIndex], &sDesc, NULL, true);
											pcDevice->Release();
										}
									}
									else
										pcBuffer->Release();
								}
								else
									m_apcActiveCSConstantBuffers[dwInternalIndex + D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = nullptr;
							}
						}
					}
					return nullptr;
#pragma endregion
			}
			return nullptr;
#pragma endregion
#pragma region IDXGISWAPCHAIN
		case INTERFACE_IDXGISWAPCHAIN:
			if (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)
				Present((IDXGISwapChain*)pThis);
			return nullptr;
#pragma endregion
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

#pragma region verify constant buffer counter
	// if not connected the constant buffers will not be verified
	if (m_pdwVerifyConstantBuffers)
	{
		// > Zero ?
		if (*m_pdwVerifyConstantBuffers)
		{
			// update constant buffer frame count
			(*m_pdwVerifyConstantBuffers)--;
			m_dwVerifyConstantBuffers = *m_pdwVerifyConstantBuffers;

			m_bControlUpdate = true;
		}
	}
#pragma endregion

#pragma region create new
	if ((m_apcNewRenderTargetViews11.size()) || (m_apcNewDepthStencilViews11.size()) || (m_apcNewShaderResourceViews11.size()))
	{
		// get device
		ID3D11Device* pcDevice = nullptr;
		pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
		if (pcDevice)
		{
			// create new render target view
			while (m_apcNewRenderTargetViews11.size())
			{
				// create and delete the first in the list
				CreateStereoView((IUnknown*)pcDevice, m_apcNewRenderTargetViews11[0]);
				m_apcNewRenderTargetViews11.erase(m_apcNewRenderTargetViews11.begin());
			}

			// create new depth stencil view
			if (m_apcNewDepthStencilViews11.size())
			{
				// create and delete the first in the list
				CreateStereoView((IUnknown*)pcDevice, m_apcNewDepthStencilViews11[0]);
				m_apcNewDepthStencilViews11.erase(m_apcNewDepthStencilViews11.begin());
			}

			// create new shader resource view
			if (m_apcNewShaderResourceViews11.size())
			{
				// get the description
				D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
				m_apcNewShaderResourceViews11[0]->GetDesc(&sDesc);

				// get resource
				ID3D11Resource* pcResource;
				m_apcNewShaderResourceViews11[0]->GetResource(&pcResource);
				if (pcResource)
				{
					// get the stereo twin of the resource (texture)
					ID3D11Resource* pcResourceTwin = nullptr;
					UINT dwSize = sizeof(pcResourceTwin);
					pcResource->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
					if (pcResourceTwin)
					{
						ID3D11ShaderResourceView* pcShaderResourceView = nullptr;
						if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)pcResourceTwin, &sDesc, &pcShaderResourceView))))
							OutputDebugString(L"VireioStereoSplitter: Failed to create texture view!");
						else
						{
							// set private data interface
							pcResourceTwin->SetPrivateDataInterface(PDIID_ID3D11TextureXD_ShaderResourceView, pcShaderResourceView);

							// release resource view, reference now only in the resource present
							pcShaderResourceView->Release();
						}

						pcResourceTwin->Release();
					}
					else
					{
						// create new stereo view
						CreateStereoView((IUnknown*)pcDevice, m_apcNewShaderResourceViews11[0]);
					}

					pcResource->Release();
				}

				// and update the new vector
				m_apcNewShaderResourceViews11.erase(m_apcNewShaderResourceViews11.begin());
			}

			pcDevice->Release();
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
			if (true)
			{
				// get back buffer
				ID3D11Texture2D* pcBackBuffer = NULL;
				pcSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
				if (pcBackBuffer)
				{
					if (pcBackBuffer != m_pcActiveBackBuffer11)
					{
						SAFE_RELEASE(m_pcTexView11[0]);
						SAFE_RELEASE(m_pcTexView11[1]);
						SAFE_RELEASE(m_pcTex11[0]);
						SAFE_RELEASE(m_pcTex11[1]);

						// get the stored private data view
						ID3D11RenderTargetView* pcView = nullptr;
						UINT dwSize = sizeof(pcView);
						pcBackBuffer->GetPrivateData(PDIID_ID3D11TextureXD_RenderTargetView, &dwSize, (void*)&pcView);

						if (dwSize)
						{
							// set the active buffer
							m_pcActiveBackBuffer11 = pcBackBuffer;

							// get the stereo twin buffer, release since private data interface
							m_pcActiveStereoTwinBackBuffer11 = nullptr;
							dwSize = sizeof(m_pcActiveStereoTwinBackBuffer11);
							pcBackBuffer->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&m_pcActiveStereoTwinBackBuffer11);
							if (m_pcActiveStereoTwinBackBuffer11)
							{
								// get render target twin
								ID3D11RenderTargetView* pcViewTwin = nullptr;
								dwSize = sizeof(pcViewTwin);
								pcView->GetPrivateData(PDIID_ID3D11RenderTargetView_Stereo_Twin, &dwSize, (void*)&pcViewTwin);
								if (dwSize)
								{
									// set as private data interfaces to the swapchain
									pcSwapChain->SetPrivateDataInterface(PDIID_IDXGISwapChain_TextureXD_Stereo_Twin, m_pcActiveStereoTwinBackBuffer11);
									pcSwapChain->SetPrivateDataInterface(PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin, pcViewTwin);

									pcViewTwin->Release();
								}
								m_pcActiveStereoTwinBackBuffer11->Release();
							}
							else
								OutputDebugString(L"StereoSplitter: No back buffer stereo texture present !");

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

							pcView->Release();
						}
						else
						{
							// no render target view present ? return
							OutputDebugString(L"StereoSplitter: No back buffer render target view present!");

							pcBackBuffer->Release();
							m_pcActiveBackBuffer11 = nullptr;
							m_pcActiveStereoTwinBackBuffer11 = nullptr;
							m_bPresent = true; /**< do not forget to set present bool here !! ***/
							return;
						}
					}
					else
					{
						// verify back buffer ??
						UINT dwSize = sizeof(m_pcActiveStereoTwinBackBuffer11);
						pcBackBuffer->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&m_pcActiveStereoTwinBackBuffer11);

						if (m_pcActiveStereoTwinBackBuffer11)
						{
							m_pcActiveStereoTwinBackBuffer11->Release();
						}
						else
							OutputDebugString(L"StereoSplitter: No back buffer stereo texture present !");
					}
					pcBackBuffer->Release();
				}
				else
				{
					// no backbuffer present ? should not be...
					OutputDebugString(L"VireioStereoSplitter: No back buffer present !");
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
	// draw stereo target to screen (optionally) // TODO !! OPTION TO SET DRAWING ON/OFF, TOBEDONE WHEN DOING THE GUI OF THIS NODE
	if (true)
	{
		switch (m_eD3DVersion)
		{
			case Direct3D10:
				break;
			case Direct3D11:
				if (true)
				{
					if (!m_pcActiveBackBuffer11) break;

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

					// update the output textures - get stored private data
					if ((m_pcTex11[0]) && (m_pcActiveBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[0], (ID3D11Resource*)m_pcActiveBackBuffer11);
					UINT dwSize = sizeof(m_pcActiveBackBuffer11);
					m_pcActiveBackBuffer11->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&m_pcActiveStereoTwinBackBuffer11);
					if ((dwSize) && (m_pcActiveStereoTwinBackBuffer11))
					{
						if (m_pcTex11[1]) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[1], (ID3D11Resource*)m_pcActiveStereoTwinBackBuffer11);
						m_pcActiveStereoTwinBackBuffer11->Release();
					}

					// get the viewport
					UINT dwNumViewports = 1;
					D3D11_VIEWPORT psViewport[16];
					pcContext->RSGetViewports(&dwNumViewports, psViewport);

					// backup all states
					D3DX11_STATE_BLOCK sStateBlock;
					CreateStateblock(pcContext, &sStateBlock);

					// clear all states, set targets
					pcContext->ClearState();

					// set first active render target - the stored back buffer - get the stored private data view
					ID3D11RenderTargetView* pcView = nullptr;
					dwSize = sizeof(pcView);
					m_pcActiveBackBuffer11->GetPrivateData(PDIID_ID3D11TextureXD_RenderTargetView, &dwSize, (void*)&pcView);
					if (dwSize)
					{
						pcContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&pcView, (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
						pcView->Release();
					}
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
						// eventually set option to clear the depth stencil view
						if (true)
							pcContext->ClearDepthStencilView((ID3D11DepthStencilView*)m_pcActiveDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

						// left/right eye
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

		// update the output textures - get stored private data
		if ((m_pcTex11[0]) && (m_pcActiveBackBuffer11)) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[0], (ID3D11Resource*)m_pcActiveBackBuffer11);
		UINT dwSize = sizeof(m_pcActiveBackBuffer11);
		m_pcActiveBackBuffer11->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&m_pcActiveStereoTwinBackBuffer11);
		if ((dwSize) && (m_pcActiveStereoTwinBackBuffer11))
		{
			if (m_pcTex11[1]) pcContext->CopyResource((ID3D11Resource*)m_pcTex11[1], (ID3D11Resource*)m_pcActiveStereoTwinBackBuffer11);
			m_pcActiveStereoTwinBackBuffer11->Release();
		}

		if (pcDevice) { pcDevice->Release(); pcDevice = nullptr; }
		if (pcContext) { pcContext->Release(); pcContext = nullptr; }
	}
#pragma endregion

	// set present() bool to true, back buffer not picked for the next frame
	m_bPresent = true;
	m_eBackBufferVerified = NotVerified;
}

/**
* Incoming OMSetRenderTargets() call (D3D10+).
* Set render targets internally, handle render target monitoring.
* @param pcDeviceOrContext For DX10 this is the device, for DX11 this is the context.
***/
void StereoSplitter::OMSetRenderTargets(IUnknown* pcDeviceOrContext, UINT NumViews, IUnknown *const *ppRenderTargetViews, IUnknown *pDepthStencilView)
{
	// is the back buffer verified for the current frame ?
	if ((TODO_ADD_BOOL_HERE) && (!m_eBackBufferVerified))
	{
		// seems a bit odd here but for some scenarious (DXGI_USAGE_DISCARD_ON_PRESENT)
		// we need to verify the stereo back buffers
		switch (m_eD3DVersion)
		{
			case NotDefined:
				break;
			case Direct3D10:
				break;
			case Direct3D11:
				if (true)
				{
					// get the render target
					ID3D11RenderTargetView* pRenderTargetView = nullptr;
					ID3D11DepthStencilView* pDepthStencilView = nullptr;
					((ID3D11DeviceContext*)pcDeviceOrContext)->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);

					if (pRenderTargetView)
					{
						// get the buffer texture
						ID3D11Texture2D* pBuffer = nullptr;
						pRenderTargetView->GetResource((ID3D11Resource**)(&pBuffer));
						if (pBuffer)
						{
							// get the buffer surface
							IDXGISurface* pSurface = nullptr;
							if (SUCCEEDED(pBuffer->QueryInterface(__uuidof(IDXGISurface), (void**)(&pSurface))))
							{
								// and finally get the swapchain
								IDXGISwapChain* pSwapChain = nullptr;
								if (SUCCEEDED(pSurface->GetParent(__uuidof(IDXGISwapChain), (void**)(&pSwapChain))))
								{
									// get back buffer
									ID3D11Texture2D* pcBackBuffer = nullptr;
									pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
									if (pcBackBuffer)
									{
										// get the stereo twins from the swapchain
										ID3D11Texture2D* pcBackBufferTwin = nullptr;
										UINT dwSize = sizeof(pcBackBufferTwin);
										pSwapChain->GetPrivateData(PDIID_IDXGISwapChain_TextureXD_Stereo_Twin, &dwSize, &pcBackBufferTwin);
										if (dwSize)
										{
											// now, the view
											ID3D11RenderTargetView* pcBackBufferTwinView;
											dwSize = sizeof(pcBackBufferTwinView);
											pSwapChain->GetPrivateData(PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin, &dwSize, &pcBackBufferTwinView);
											if (dwSize)
											{
												// set the active back buffer
												m_pcActiveBackBuffer11 = pcBackBuffer;
												m_pcActiveStereoTwinBackBuffer11 = pcBackBufferTwin;
												m_pcActiveStereoTwinBackBufferView11 = pcBackBufferTwinView;

												m_eBackBufferVerified = BackBufferVerificationState::NewBuffer;
											}
											pcBackBufferTwin->Release();
										}
										pcBackBuffer->Release();
									}
									pSwapChain->Release();
								}
								pSurface->Release();
							}
							pBuffer->Release();
						}
						pRenderTargetView->Release();
					}
					if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }
				}
				break;
			default:
				break;
		}
	}

	// set the number of render targets set
	// for DX10 and DX11 all render targets above this number are set to NULL
	m_dwRenderTargetNumber = (DWORD)NumViews;
	if (m_dwRenderTargetNumber > D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		m_dwRenderTargetNumber = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

	// set the render target internally
	if (NumViews <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
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

						// verify the stereo components of this interface
						m_apcActiveStereoTwinViews[i] =
							VerifyPrivateDataInterfaces((ID3D11RenderTargetView*)ppRenderTargetViews[i]);
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

		// verify the stereo components of this interface
		m_pcActiveStereoTwinDepthStencilView =
			VerifyPrivateDataInterfaces((ID3D11DepthStencilView*)pDepthStencilView);
	}

	m_bControlUpdate = true;
}

/**
* Incoming CSSetUnorderedAccessViews() call (D3D11).
* Handle UAVs.
* @param pcThis D3D11 Device Context.
***/
void StereoSplitter::CSSetUnorderedAccessViews(ID3D11DeviceContext *pcThis, UINT dwStartSlot, UINT dwNumUAVs, ID3D11UnorderedAccessView *const *ppcUnorderedAccessViews, const UINT *pdwUAVInitialCounts)
{
	// loop through the new buffers
	for (UINT dwIndex = 0; dwIndex < dwNumUAVs; dwIndex++)
	{
		// get internal index
		UINT dwInternalIndex = dwIndex + dwStartSlot;

		// in range ? 
		if (dwInternalIndex < D3D11_PS_CS_UAV_REGISTER_COUNT)
		{
			if (ppcUnorderedAccessViews[dwInternalIndex])
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC sDesc;
				ppcUnorderedAccessViews[dwInternalIndex]->GetDesc(&sDesc);

				// is a buffer ?
				if (sDesc.ViewDimension == D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_BUFFER)
				{
					OutputDebugString(L"D3D11_UAV_DIMENSION_BUFFER");
				}
			}
		}
	}
}

/**
* Creates a stereo view out of a mono view.
* Creates the stereo texture and its view, then assignes all as private data interfaces.
***/
void StereoSplitter::CreateStereoView(IUnknown* pcDevice, ID3D11View* pcView)
{
	if ((!pcView) || (!pcDevice)) return;

#pragma region determine view type
	// all possible view types enumeration
	enum D3DViewType
	{
		D3D10_RENDER_TARGET_VIEW,
		D3D10_DEPTH_STENCIL_VIEW,
		D3D10_SHADER_RESOURCE_VIEW,
		D3D11_RENDER_TARGET_VIEW,
		D3D11_DEPTH_STENCIL_VIEW,
		D3D11_SHADER_RESOURCE_VIEW,
		UNSUPPORTED,
	} eD3DViewType = D3DViewType::UNSUPPORTED;

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
						if (FAILED(pcView->QueryInterface(__uuidof(ID3D11UnorderedAccessView), &pvObject)))
						{
							OutputDebugString(L"StereoSplitterDX10: Failed to determine d3d view type !");
						}
						else
						{
							OutputDebugString(L"StereoSplitterDX10: Unsupported view type: ID3D11UnorderedAccessView !");
							eD3DViewType = D3DViewType::UNSUPPORTED;
						}
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
#pragma endregion

#pragma region get description
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
			((ID3D11ShaderResourceView*)pcView)->GetDesc(&sDescSR11);
			break;
		default:
			break;
	}
#pragma endregion

#pragma region create new
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
	union
	{
		ID3D10Texture1D* pcResource10_1D;
		ID3D11Texture1D* pcResource11_1D;
		ID3D10Texture2D* pcResource10;
		ID3D11Texture2D* pcResource11;
	};
	pcResource10 = nullptr;


	// create a new render target texture
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
				if ((int)eD3DViewType < 3)
				{
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
						default:
							OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
							break;
					}
				}
				else
				{
					// get the texture
					((ID3D11View*)pcView)->GetResource((ID3D11Resource**)&pcResource11);
					if ((pcResource11) && (pcDevice))
					{
						// has this texture already a stereo twin ?
						UINT dwSize = sizeof(pcStereoTwinTexture11);
						pcResource11_1D->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcStereoTwinTexture11);
						if (!dwSize)
						{
							// get the description and create the twin texture
							D3D11_TEXTURE1D_DESC sDesc;
							pcResource11_1D->GetDesc(&sDesc);

							if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture1D(&sDesc, NULL, (ID3D11Texture1D**)&pcStereoTwinTexture11)))
							{
								OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
								break;
							}
						}

						// create view and apply stereo twins
						switch (eD3DViewType)
						{
							case D3D11_RENDER_TARGET_VIEW:
								// create twin render target view
								if (FAILED(((ID3D11Device*)pcDevice)->CreateRenderTargetView((ID3D11Resource*)pcStereoTwinTexture11, &sDescRT11, (ID3D11RenderTargetView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_RENDER_TARGET_VIEW !");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the render target view to the texture
									pcResource11_1D->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcView);
									// now assign the stereo texture twin interface
									pcResource11_1D->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin render target view
									pcView->SetPrivateDataInterface(PDIID_ID3D11RenderTargetView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							case D3D11_DEPTH_STENCIL_VIEW:
								// create twin render target view
								if (FAILED(((ID3D11Device*)pcDevice)->CreateDepthStencilView((ID3D11Resource*)pcStereoTwinTexture11, &sDescDS11, (ID3D11DepthStencilView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_DEPTH_STENCIL_VIEW !");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the depth stencil view to the texture
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_DepthStencilView, pcView);
									// now assign the stereo texture twin interface
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin depth stencil
									pcView->SetPrivateDataInterface(PDIID_ID3D11DepthStencilView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							case D3D11_SHADER_RESOURCE_VIEW:
								OutputDebugString(L"NotImplemented: D3D11_SHADER_RESOURCE_VIEW");
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}

						SAFE_RELEASE(pcResource11_1D);
					}
					else
						SAFE_RELEASE(pcResource11_1D);
				}
				break;
			case D3D10_RTV_DIMENSION_TEXTURE1DARRAY:
				if ((int)eD3DViewType < 3)
				{
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
						default:
							OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
							break;
					}
				}
				else
				{
					// get the texture
					((ID3D11View*)pcView)->GetResource((ID3D11Resource**)&pcResource11);
					if ((pcResource11) && (pcDevice))
					{
						// has this texture already a stereo twin ?
						UINT dwSize = sizeof(pcStereoTwinTexture11);
						pcResource11->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcStereoTwinTexture11);
						if (!dwSize)
						{
							// get the description and create the twin texture
							D3D11_TEXTURE2D_DESC sDesc;
							pcResource11->GetDesc(&sDesc);

							if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture11)))
							{
								OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
								break;
							}
						}

						// create view and apply stereo twins
						switch (eD3DViewType)
						{
							case D3D11_RENDER_TARGET_VIEW:
								OutputDebugString(L"NotImplemented: D3D11_RENDER_TARGET_VIEW");
								break;
							case D3D11_DEPTH_STENCIL_VIEW:
								// create twin render target view
								if (FAILED(((ID3D11Device*)pcDevice)->CreateDepthStencilView((ID3D11Resource*)pcStereoTwinTexture11, &sDescDS11, (ID3D11DepthStencilView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_DEPTH_STENCIL_VIEW !");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the depth stencil view to the texture
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_DepthStencilView, pcView);
									// now assign the stereo texture twin interface
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin depth stencil
									pcView->SetPrivateDataInterface(PDIID_ID3D11DepthStencilView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							case D3D11_SHADER_RESOURCE_VIEW:
								OutputDebugString(L"NotImplemented: D3D11_SHADER_RESOURCE_VIEW");
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}

						SAFE_RELEASE(pcResource11);
					}
					else
						SAFE_RELEASE(pcResource11);
				}
				break;
			case D3D10_RTV_DIMENSION_TEXTURE2D:
			case D3D10_RTV_DIMENSION_TEXTURE2DARRAY:
			case D3D10_RTV_DIMENSION_TEXTURE2DMS:
			case D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY:
				if ((int)eD3DViewType < 3)
				{
					// get the texture
					((ID3D10View*)pcView)->GetResource((ID3D10Resource**)&pcResource10);
					if ((pcResource10) && (pcDevice))
					{
						// has this texture already a stereo twin ?
						UINT dwSize = sizeof(pcStereoTwinTexture11);
						pcResource10->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcStereoTwinTexture10);
						if (!dwSize)
						{
							// get the description and create the twin texture
							D3D10_TEXTURE2D_DESC sDesc;
							pcResource10->GetDesc(&sDesc);

							if (FAILED(((ID3D10Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D10Texture2D**)&pcStereoTwinTexture10)))
							{
								OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
								break;
							}
						}

						// create view and apply stereo twins
						switch (eD3DViewType)
						{
							case D3D10_RENDER_TARGET_VIEW:
								// create twin render target view
								if (FAILED(((ID3D10Device*)pcDevice)->CreateRenderTargetView((ID3D10Resource*)pcStereoTwinTexture10, &sDescRT10, (ID3D10RenderTargetView**)&pcStereoTwinView10)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D10_RENDER_TARGET_VIEW !");

								if ((pcStereoTwinTexture10) && (pcStereoTwinView10))
								{
									// assign stereo private data interfaces, first the render target view to the texture
									pcResource10->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcView);
									// now assign the stereo texture twin interface
									pcResource10->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture10);
									// last, the stereo twin render target view
									pcView->SetPrivateDataInterface(PDIID_ID3D11RenderTargetView_Stereo_Twin, pcStereoTwinView10);
								}

								break;
							case D3D10_DEPTH_STENCIL_VIEW:
								// create twin render target view
								if (FAILED(((ID3D10Device*)pcDevice)->CreateDepthStencilView((ID3D10Resource*)pcStereoTwinTexture10, &sDescDS10, (ID3D10DepthStencilView**)&pcStereoTwinView10)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D10_DEPTH_STENCIL_VIEW !");
								// TODO !! ASSIGN PRIVATE DATA INTERFACES
								break;
							case D3D10_SHADER_RESOURCE_VIEW:
								OutputDebugString(L"NotImplemented: D3D10_SHADER_RESOURCE_VIEW");
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}

						SAFE_RELEASE(pcResource10);
					}
					else
						SAFE_RELEASE(pcResource10);
				}
				else
				{
					// get the texture
					((ID3D11View*)pcView)->GetResource((ID3D11Resource**)&pcResource11);
					if ((pcResource11) && (pcDevice))
					{
						// get the description and create the twin texture
						D3D11_TEXTURE2D_DESC sDesc;
						pcResource11->GetDesc(&sDesc);

						// handle sRGB formats
						// there was an error creating textures with one game... can't remember which one... hmm... this fixed it
						if (sDesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
							sDescRT11.Format = sDesc.Format;

						// has this texture already a stereo twin ?
						UINT dwSize = sizeof(pcStereoTwinTexture11);
						pcResource11->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcStereoTwinTexture11);
						if (!dwSize)
						{
							if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcStereoTwinTexture11)))
							{
								OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
								break;
							}
						}

						// create view and apply stereo twins
						switch (eD3DViewType)
						{
							case D3D11_RENDER_TARGET_VIEW:
								// create twin render target view
								if (FAILED(((ID3D11Device*)pcDevice)->CreateRenderTargetView((ID3D11Resource*)pcStereoTwinTexture11, &sDescRT11, (ID3D11RenderTargetView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_RENDER_TARGET_VIEW !");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the render target view to the texture
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcView);
									// now assign the stereo texture twin interface
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin render target view
									pcView->SetPrivateDataInterface(PDIID_ID3D11RenderTargetView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							case D3D11_DEPTH_STENCIL_VIEW:
								// create depth stencil view
								if (FAILED(((ID3D11Device*)pcDevice)->CreateDepthStencilView((ID3D11Resource*)pcStereoTwinTexture11, &sDescDS11, (ID3D11DepthStencilView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_DEPTH_STENCIL_VIEW !");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the depth stencil view to the texture
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_DepthStencilView, pcView);
									// now assign the stereo texture twin interface
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin depth stencil
									pcView->SetPrivateDataInterface(PDIID_ID3D11DepthStencilView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							case D3D11_SHADER_RESOURCE_VIEW:
								if (FAILED(((ID3D11Device*)pcDevice)->CreateShaderResourceView((ID3D11Resource*)pcStereoTwinTexture11, &sDescSR11, (ID3D11ShaderResourceView**)&pcStereoTwinView11)))
									OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_SHADER_RESOURCE_VIEW!");

								if ((pcStereoTwinTexture11) && (pcStereoTwinView11))
								{
									// assign stereo private data interfaces, first the depth stencil view to the texture
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_ShaderResourceView, pcView);
									// now assign the stereo texture twin interface
									pcResource11->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcStereoTwinTexture11);
									// last, the stereo twin depth stencil TODO !! create GUID for this ? need this ?
									// pcView->SetPrivateDataInterface(PDIID_ID3D11ShaderResourceView_Stereo_Twin, pcStereoTwinView11);
								}
								break;
							default:
								OutputDebugString(L"NotImplemented: UNKNOWN TYPE");
								break;
						}

						SAFE_RELEASE(pcResource11);
					}
					else
						SAFE_RELEASE(pcResource11);
				}
				break;
			case D3D10_RTV_DIMENSION_TEXTURE3D:
				OutputDebugString(L"NotImplemented: D3D10_RTV_DIMENSION_TEXTURE3D");
				break;
			default:
				OutputDebugString(L"NotImplemented: UNKNOWN DIMENSION");
				break;
		}
	}

	// release safe
	SAFE_RELEASE(pcStereoTwinTexture11);
	SAFE_RELEASE(pcStereoTwinView11);
#pragma endregion

	// update control... maybe we need this later
	m_bControlUpdate = true;
}

/**
* Verifies all (stereo) private data interfaces for this render target view.
* Creates new stereo interfaces if not present.
***/
ID3D11RenderTargetView* StereoSplitter::VerifyPrivateDataInterfaces(ID3D11RenderTargetView* pcRenderTargetView)
{
	if (!pcRenderTargetView) return nullptr;

	// does this render target view have a stereo twin view ?
	ID3D11RenderTargetView* pcView = nullptr;
	UINT dwSize = sizeof(pcView);
	pcRenderTargetView->GetPrivateData(PDIID_ID3D11RenderTargetView_Stereo_Twin, &dwSize, (void*)&pcView);

	if (dwSize)
	{
		// one release on a private data interface must not give back a nullptr
		pcView->Release();
		return pcView;
	}
	else
	{
		// is the back buffer verified for this frame ?
		if (TODO_ADD_BOOL_HERE)
		{
			if (m_eBackBufferVerified == BackBufferVerificationState::NewBuffer)
			{
				// get the resource for this view and compare to active back buffer
				ID3D11Resource* pcResource = nullptr;
				pcRenderTargetView->GetResource(&pcResource);
				if (pcResource)
				{
					if (pcResource == m_pcActiveBackBuffer11)
					{
						// assign stereo private data interfaces, first the render target view to the texture
						pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcRenderTargetView);
						// now assign the stereo texture twin interface
						pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, m_pcActiveStereoTwinBackBuffer11);
						// last, the stereo twin render target view
						pcRenderTargetView->SetPrivateDataInterface(PDIID_ID3D11RenderTargetView_Stereo_Twin, m_pcActiveStereoTwinBackBufferView11);

						// set private data interfaces to this view
						m_eBackBufferVerified = BackBufferVerificationState::Verified;

						pcResource->Release();
						return m_pcActiveStereoTwinBackBufferView11;
					}
					pcResource->Release();
				}
			}
		}

		// is this render target already present in "new" list ?
		bool bInList = false;
		auto it = m_apcNewRenderTargetViews11.begin();
		while (it != m_apcNewRenderTargetViews11.end())
		{
			if (pcRenderTargetView == *it)
			{
				bInList = true;
				break;
			}
			it++;
		}

		// add to new render targets list vector
		if (!bInList)
			m_apcNewRenderTargetViews11.push_back(pcRenderTargetView);

		return nullptr;
	}
}

/**
* Verifies all (stereo) private data interfaces for this depth view.
* Creates new stereo interfaces if not present.
***/
ID3D11DepthStencilView* StereoSplitter::VerifyPrivateDataInterfaces(ID3D11DepthStencilView* pcDepthStencilView)
{
	if (!pcDepthStencilView) return nullptr;

	// does this render target view have a stereo twin view ?
	ID3D11DepthStencilView* pcView = nullptr;
	UINT dwSize = sizeof(pcView);
	pcDepthStencilView->GetPrivateData(PDIID_ID3D11DepthStencilView_Stereo_Twin, &dwSize, (void*)&pcView);

	if (dwSize)
	{
		// one release on a private data interface must not give back a nullptr
		pcView->Release();
		return pcView;
	}
	else
	{
		// is this depth stencil already present in "new" list ?
		bool bInList = false;
		auto it = m_apcNewDepthStencilViews11.begin();
		while (it != m_apcNewDepthStencilViews11.end())
		{
			if (pcDepthStencilView == *it)
			{
				bInList = true;
				break;
			}
			it++;
		}

		// add to new depth stencil list vector
		if (!bInList)
			m_apcNewDepthStencilViews11.push_back(pcDepthStencilView);

		return nullptr;
	}
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
	SetDrawingSideField(eSide);

	// switch render targets to new eSide
	bool renderTargetChanged = false;
	HRESULT hr = S_OK;
	if (eSide == RenderPosition::Left)
		pcDevice->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D10RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D10DepthStencilView*)m_pcActiveDepthStencilView);
	else
		pcDevice->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D10RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D10DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

	// TODO !! port DX11 code back to DX10

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
	// static constant buffer buffer
	static std::vector<ID3D11Buffer*> acConstantBuffers = std::vector<ID3D11Buffer*>(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, nullptr);

	// Already on the correct eye
	if (eSide == m_eCurrentRenderingSide)
		return true;

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	SetDrawingSideField(eSide);

	// switch render targets to new side
	if (eSide == RenderPosition::Left)
		pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
	else
		pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveStereoTwinViews[0], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

	// switch textures to new side
	if (eSide == RenderPosition::Left)
		pcContext->PSSetShaderResources(0, m_dwTextureNumber, (ID3D11ShaderResourceView**)&m_apcActiveTextures[0]);
	else
		pcContext->PSSetShaderResources(0, m_dwTextureNumber, (ID3D11ShaderResourceView**)&m_apcActiveStereoTwinTextures[0]);

#ifdef _DEBUG_VIREIO
	// constant buffers are to be verified ?
	if (m_dwVerifyConstantBuffers)
	{
		// get the current vertex shader private data
		ID3D11VertexShader* pcVertexShader = nullptr;
		pcContext->VSGetShader(&pcVertexShader, nullptr, nullptr);
		Vireio_Shader_Private_Data sPrivateData;
		UINT dwDataSize = sizeof(sPrivateData);
		if (pcVertexShader)
			pcVertexShader->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

		// switch constant buffers
		if (m_appcActiveConstantBuffers11)
		if (*m_appcActiveConstantBuffers11)
		{
			// loop through the buffers
			for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
			if ((*m_appcActiveConstantBuffers11)[dwIndex])
			{
				// set the current shader data as private data to the buffer
				if ((pcVertexShader) && (dwDataSize))
					((*m_appcActiveConstantBuffers11)[dwIndex])->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);

				// get the private data interfaces and set the current drawing side
				UINT dwSize = sizeof(ID3D11Buffer*);
				if (eSide == RenderPosition::Left)
					((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSize, (void*)&acConstantBuffers[dwIndex]);
				else
					((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&acConstantBuffers[dwIndex]);

				// "If the data returned is a pointer to an IUnknown, or one of its derivative classes,
				// previously set by IDXGIObject::SetPrivateDataInterface, you must call::Release() on 
				// the pointer before the pointer is freed to decrement the reference count." (Microsoft)
				if (acConstantBuffers[dwIndex])
					acConstantBuffers[dwIndex]->Release();
			}

			// finally set all constant buffers for the left or right side
			pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &acConstantBuffers[0]);
		}

		if (pcVertexShader)
			pcVertexShader->Release();
	}
	else
	{
		// switch constant buffers
		if (m_appcActiveConstantBuffers11)
		if (*m_appcActiveConstantBuffers11)
		{
			// loop through the buffers
			for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
			if ((*m_appcActiveConstantBuffers11)[dwIndex])
			{
				// get the private data interfaces and set the current drawing side
				UINT dwSize = sizeof(ID3D11Buffer*);
				if (eSide == RenderPosition::Left)
					((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSize, (void*)&acConstantBuffers[dwIndex]);
				else
					((*m_appcActiveConstantBuffers11)[dwIndex])->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&acConstantBuffers[dwIndex]);

				// "If the data returned is a pointer to an IUnknown, or one of its derivative classes,
				// previously set by IDXGIObject::SetPrivateDataInterface, you must call::Release() on 
				// the pointer before the pointer is freed to decrement the reference count." (Microsoft)
				if (acConstantBuffers[dwIndex])
					acConstantBuffers[dwIndex]->Release();
			}

			// finally set all constant buffers for the left or right side
			pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &acConstantBuffers[0]);
		}
	}
#else
	// switch constant buffers
	if (m_appcActiveConstantBuffers11)
	if (*m_appcActiveConstantBuffers11)
	{
		// set all constant buffers for the left or right side
		if (eSide == RenderPosition::Left)
			pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcActiveConstantBuffers11)[0]);
		else
			pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
	}
#endif

	return true;
}