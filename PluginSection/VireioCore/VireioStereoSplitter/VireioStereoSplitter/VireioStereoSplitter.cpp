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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }
#define IS_RENDER_TARGET(d3dusage) ((d3dusage & D3DUSAGE_RENDERTARGET) > 0 ? true : false)

#include"VireioStereoSplitter.h"

#define INTERFACE_IDIRECT3DDEVICE9                           8
#define INTERFACE_IDIRECT3DSTATEBLOCK9                      13
#define INTERFACE_IDIRECT3DSWAPCHAIN9                       15
#define INTERFACE_D3DX9                                     29
#define METHOD_IDIRECT3DDEVICE9_RESET                       16
#define	METHOD_IDIRECT3DDEVICE9_PRESENT                     17
#define METHOD_IDIRECT3DDEVICE9_GETBACKBUFFER               18 
#define METHOD_IDIRECT3DDEVICE9_CREATETEXTURE               23
#define METHOD_IDIRECT3DDEVICE9_CREATEVOLUMETEXTURE         24
#define METHOD_IDIRECT3DDEVICE9_CREATECUBETEXTURE           25
#define METHOD_IDIRECT3DDEVICE9_CREATEVERTEXBUFFER          26
#define METHOD_IDIRECT3DDEVICE9_CREATEINDEXBUFFER           27
#define METHOD_IDIRECT3DDEVICE9_CREATERENDERTARGET          28
#define METHOD_IDIRECT3DDEVICE9_CREATEDEPTHSTENCILSURFACE   29
#define METHOD_IDIRECT3DDEVICE9_UPDATESURFACE               30 
#define METHOD_IDIRECT3DDEVICE9_UPDATETEXTURE               31 
#define METHOD_IDIRECT3DDEVICE9_GETRENDERTARGETDATA         32
#define METHOD_IDIRECT3DDEVICE9_GETFRONTBUFFERDATA          33
#define METHOD_IDIRECT3DDEVICE9_STRETCHRECT                 34 
#define METHOD_IDIRECT3DDEVICE9_COLORFILL                   35 
#define METHOD_IDIRECT3DDEVICE9_CREATEOFFSCREENPLAINSURFACE 36
#define METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET             37
#define METHOD_IDIRECT3DDEVICE9_GETRENDERTARGET             38 
#define METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE      39
#define METHOD_IDIRECT3DDEVICE9_GETDEPTHSTENCILSURFACE      40 
#define METHOD_IDIRECT3DDEVICE9_BEGINSCENE                  41
#define METHOD_IDIRECT3DDEVICE9_ENDSCENE                    42
#define METHOD_IDIRECT3DDEVICE9_CLEAR                       43
#define METHOD_IDIRECT3DDEVICE9_SETVIEWPORT                 47
#define METHOD_IDIRECT3DDEVICE9_SETRENDERSTATE              57
#define METHOD_IDIRECT3DDEVICE9_GETTEXTURE                  64 
#define METHOD_IDIRECT3DDEVICE9_SETTEXTURE                  65
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE               81
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE        82 
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP             83 
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP      84
#define METHOD_IDIRECT3DDEVICE9_PROCESSVERTICES             85
#define METHOD_IDIRECT3DDEVICE9_SETSTREAMSOURCE            100
#define METHOD_IDIRECT3DDEVICE9_GETSTREAMSOURCE            101
#define METHOD_IDIRECT3DDEVICE9_SETINDICES                 104
#define METHOD_IDIRECT3DDEVICE9_GETINDICES                 105
#define METHOD_IDIRECT3DDEVICE9_DRAWRECTPATCH              115 
#define METHOD_IDIRECT3DDEVICE9_DRAWTRIPATCH               116
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT                   3
#define	METHOD_IDIRECT3DSWAPCHAIN9_GETFRONTBUFFERDATA        4
#define	METHOD_IDIRECT3DSWAPCHAIN9_GETBACKBUFFER             5
#define METHOD_IDIRECT3DSTATEBLOCK9_APPLY                    5

/**
* Constructor.
***/
StereoSplitter::StereoSplitter() :AQU_Nodus(),
m_apcActiveRenderTargets(D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT * 2, nullptr),
m_apcActiveTextures(D3D9_SIMULTANEAOUS_TEXTURE_COUNT * 2, nullptr),
m_apcActiveTexturesDisplacement(D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT * 2, nullptr),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_unTextureNumber(0),
m_unRenderTargetNumber(0),
m_bPresent(false),
m_bApply(true),
m_punRenderTargetIndex(nullptr),
m_ppcRenderTarget(nullptr),
m_ppcNewZStencil(nullptr),
m_punSampler(nullptr),
m_ppcTexture(nullptr),
m_ppSourceSurface(nullptr),
m_ppcSourceRect(nullptr),
m_ppcDestinationSurface(nullptr),
m_ppsDestPoint(nullptr),
m_ppcSourceTexture(nullptr),
m_ppcDestinationTexture(nullptr),
m_ppcSurface(nullptr),
m_ppsRect(nullptr),
m_punColor(nullptr),
m_ppcSourceSurface_StretchRect(nullptr),
m_ppcSourceRect_StretchRect(nullptr),
m_ppcDestSurface_StretchRect(nullptr),
m_ppcDestRect_StretchRect(nullptr),
m_peFilter_StretchRect(nullptr),
m_peDrawingSide(nullptr),
m_ppasVSConstantRuleIndices(nullptr),
m_ppasPSConstantRuleIndices(nullptr),
m_peState(nullptr),
m_punValue(nullptr),
m_ppsViewport(nullptr),
m_ppcRenderTargetGetData(nullptr),
m_punISwapChain(nullptr),
m_ppcDestSurface(nullptr),
m_punWidth(nullptr),
m_punHeight(nullptr),
m_punEdgeLength(nullptr),
m_punLevels(nullptr),
m_punDepth(nullptr),
m_punUsage(nullptr),
m_peFormat(nullptr),
m_peMultiSample(nullptr),
m_punMultisampleQuality(nullptr),
m_pnDiscard(nullptr),
m_pnLockable(nullptr),
m_pePool(nullptr),
m_pppcTextureCreate(nullptr),
m_pppcVolumeTexture(nullptr),
m_pppcCubeTexture(nullptr),
m_pppcSurfaceCreate(nullptr),
m_ppvSharedHandle(nullptr),
m_ppsDestBox(nullptr),
m_ppsSrcBox(nullptr),
m_punColorKey(nullptr),
m_peSrcFormat(nullptr),
m_peDestFormat(nullptr),
m_ppsSrcInfo(nullptr),
m_punChannel(nullptr),
m_punFilter(nullptr),
m_punMipFilter(nullptr),
m_pfAmplitude(nullptr),
m_ppsSrcModule(nullptr),
m_pszhSrcFile(nullptr),
m_pszhSrcResource(nullptr),
m_ppSrcData(nullptr),
m_ppSrcMemory(nullptr),
m_pszDestFile(nullptr),
m_pszSrcFile(nullptr),
m_pszSrcResource(nullptr),
m_pppcDestBuf(nullptr),
m_ppFunction(nullptr),
m_ppcTextureShader(nullptr),
m_ppcBaseTexture(nullptr),
m_ppcSrcTexture(nullptr),
m_ppcCubeTexture(nullptr),
m_ppcSrcSurface(nullptr),
m_ppcDestVolume(nullptr),
m_ppcSrcVolume(nullptr),
m_ppcVolumeTexture(nullptr),
m_ppData(nullptr),
m_ppsDestPalette(nullptr),
m_ppsPalette(nullptr),
m_ppsSrcPalette(nullptr),
m_ppsDestRect(nullptr),
m_ppsSrcRect(nullptr),
m_punMipLevels(nullptr),
m_punSize(nullptr),
m_punSrcDataSize(nullptr),
m_punSrcLevel(nullptr),
m_punSrcPitch(nullptr),
m_punSrcRowPitch(nullptr),
m_punSrcSlicePitch(nullptr)
{
	m_pcActiveDepthStencilSurface[0] = nullptr;
	m_pcActiveDepthStencilSurface[1] = nullptr;
	m_pcActiveBackBufferSurface[0] = nullptr;
	m_pcActiveBackBufferSurface[1] = nullptr;
	m_pcStereoBuffer[0] = nullptr;
	m_pcStereoBuffer[1] = nullptr;

	// set to "LEFT" !!
	m_eCurrentRenderingSide = RenderPosition::Left;

	// first viewport set is default
	m_bActiveViewportIsDefault = true;

	// set default behaviour
	m_sGameSettings.nDuplicateRenderTarget = DetermineDuplicateRToDS::DT_TRUE;
	m_sGameSettings.nDuplicateDepthStencil = DetermineDuplicateRToDS::DT_WIDTH_UNEQUAL_HEIGHT;
	m_sGameSettings.nDuplicateTexture = DetermineDuplicateTexture::DT_IF_RENDER_TARGET;
	m_sGameSettings.nDuplicateCubeTexture = DetermineDuplicateCubeTexture::DT_IF_RENDER_TARGET;
	m_sGameSettings.nSaveRenderStatesID = DetermineSaveRenderStates::DT_STATE_BLOCK;

	// Vireio GUI is always null at begin... since in a compiled profile it is never used
	m_pcVireioGUI = nullptr;

	// use a D3D9Ex device ? only available by setting, false by constructor
	m_bUseD3D9Ex = false;

	// init d3d9ex specific vectors
	m_aapcActiveProxyBackBufferSurfaces = std::vector<std::vector<IDirect3DStereoSurface9*>>();
	m_apcActiveSwapChains = std::vector<IDirect3DSwapChain9*>();
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
	if (m_pcStereoBufferSurface[0])
	{
		m_pcStereoBufferSurface[0]->Release();
		m_pcStereoBufferSurface[0] = nullptr;
	}
	if (m_pcStereoBufferSurface[1])
	{
		m_pcStereoBufferSurface[1]->Release();
		m_pcStereoBufferSurface[1] = nullptr;
	}
	if (m_pcStereoBuffer[0])
	{
		m_pcStereoBuffer[0]->Release();
		m_pcStereoBuffer[0] = nullptr;
	}
	if (m_pcStereoBuffer[1])
	{
		m_pcStereoBuffer[1]->Release();
		m_pcStereoBuffer[1] = nullptr;
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
	// here we create the Vireio GUI.... if this runs under a compiled profile this method is never called
	if (!m_pcVireioGUI)
		CreateGUI();
	else
		return m_pcVireioGUI->GetGUI(false, true, false, false);

	return nullptr;
}

/**
* Node data size.
* 1) sizeof(GameSettings)
***/
DWORD StereoSplitter::GetSaveDataSize()
{
	DWORD dwSizeofData = sizeof(GameSettings);

	return dwSizeofData;
}

/**
* Save the data.
***/
char* StereoSplitter::GetSaveData(UINT* pdwSizeOfData)
{
	static std::stringstream acStream;
	acStream = std::stringstream();

	// write game config
	acStream.write((char*)&m_sGameSettings, sizeof(GameSettings));

	// set data size
	UINT unDataSize = (UINT)acStream.str().size();
	if (unDataSize > MAX_DATA_SIZE) unDataSize = 0;
	*pdwSizeOfData = unDataSize;

	// copy data
	memcpy(&m_acData[0], (void*)&acStream.str()[0], (size_t)unDataSize);

	return (char*)&m_acData[0];
}

/**
* Get node data from the profile file.
***/
void StereoSplitter::InitNodeData(char* pData, UINT dwSizeOfData)
{
	UINT dwDataOffset = 0;
	if (dwSizeOfData >= sizeof(GameSettings))
	{
		// copy the game settings data
		memcpy(&m_sGameSettings, pData, sizeof(GameSettings));
		dwDataOffset += sizeof(GameSettings);
	}
}

/**
* Provides the name of the requested commander.
***/
LPWSTR StereoSplitter::GetCommanderName(DWORD unCommanderIndex)
{
	switch ((STS_Commanders)unCommanderIndex)
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
LPWSTR StereoSplitter::GetDecommanderName(DWORD unDecommanderIndex)
{
	switch ((STS_Decommanders)unDecommanderIndex)
	{
		case RenderTargetIndex:
			return L"RenderTargetIndex";
		case pRenderTarget:
			return L"pRenderTarget";
		case ppRenderTarget:
			return L"ppRenderTarget";
		case pNewZStencil:
			return L"pNewZStencil";
		case ppZStencilSurface:
			return L"ppZStencilSurface";
		case Sampler:
			return L"Sampler";
		case pTexture:
			return L"pTexture";
		case ppTextureGet:
			return L"ppTexture_Get";
		case pSourceSurface:
			return L"pSourceSurface";
		case pSourceRect:
			return L"pSourceRect";
		case pDestinationSurface:
			return L"pDestinationSurface";
		case pDestPoint:
			return L"pDestPoint";
		case pSourceTexture:
			return L"pSourceTexture";
		case pDestinationTexture:
			return L"pDestinationTexture";
		case pSurface:
			return L"pSurface";
		case pRect:
			return L"pRect";
		case color:
			return L"color";
		case pSourceSurface_StretchRect:
			return L"pSourceSurface_StretchRect";
		case pSourceRect_StretchRect:
			return L"pSourceRect_StretchRect";
		case pDestSurface_StretchRect:
			return L"pDestSurface_StretchRect";
		case pDestRect_StretchRect:
			return L"pDestRect_StretchRect";
		case Filter_StretchRect:
			return L"Filter_StretchRect";
		case State:
			return L"State";
		case Value:
			return L"Value";
		case pViewport:
			return L"pViewport";
		case pRenderTargetGetData:
			return L"pRenderTargetGetData";
		case iSwapChain:
			return L"iSwapChain";
		case pDestSurface:
			return L"pDestSurface";
		case iBackBuffer:
			return L"iBackBuffer";
		case Type:
			return L"Type";
		case ppBackBuffer:
			return L"ppBackBuffer";
		case Width:
			return L"Width";
		case Height:
			return L"Height";
		case Length:
			return L"Length";
		case EdgeLength:
			return L"EdgeLength";
		case Levels:
			return L"Levels";
		case Depth:
			return L"Depth";
		case Usage:
			return L"Usage";
		case Format:
			return L"Format";
		case FVF:
			return L"FVF";
		case MultiSample:
			return L"MultiSample";
		case MultisampleQuality:
			return L"MultisampleQuality";
		case Discard:
			return L"Discard";
		case Lockable:
			return L"Lockable";
		case Pool:
			return L"Pool";
		case ppTexture:
			return L"ppTexture";
		case ppVolumeTexture:
			return L"ppVolumeTexture";
		case ppCubeTexture:
			return L"ppCubeTexture";
		case ppVertexBuffer:
			return L"ppVertexBuffer";
		case ppIndexBuffer:
			return L"ppIndexBuffer";
		case ppSurface:
			return L"ppSurface";
		case pSharedHandle:
			return L"pSharedHandle";
		case SrcStartIndex:
			return L"SrcStartIndex";
		case DestIndex:
			return L"DestIndex";
		case VertexCount:
			return L"VertexCount";
		case pDestBuffer:
			return L"pDestBuffer";
		case pVertexDecl:
			return L"pVertexDecl";
		case Flags:
			return L"Flags";
		case StreamNumber:
			return L"StreamNumber";
		case pStreamData:
			return L"pStreamData";
		case OffsetInBytes:
			return L"OffsetInBytes";
		case Stride:
			return L"Stride";
		case ppStreamData:
			return L"ppStreamData";
		case pOffsetInBytes:
			return L"pOffsetInBytes";
		case pStride:
			return L"pStride";
		case pIndexData:
			return L"pIndexData";
		case ppIndexData:
			return L"ppIndexData";
		case peDrawingSide:
			return L"peDrawingSide";
		case pasVShaderConstantIndices:
			return L"pasVShaderConstantIndices";
		case pasPShaderConstantIndices:
			return L"pasPShaderConstantIndices";
		case pDestBox:
			break;
		case pSrcBox:
			break;
		case ColorKey:
			return L"ColorKey";
		case SrcFormat:
			break;
		case DestFormat:
			break;
		case pSrcInfo:
			break;
		case Channel:
			break;
		case Filter:
			return L"Filter";
		case MipFilter:
			break;
		case Amplitude:
			break;
		case hSrcModule:
			break;
		case pDestFile:
			break;
		case pSrcFile:
			break;
		case pSrcResource:
			break;
		case pSrcData:
			break;
		case pSrcMemory:
			break;
		case pDestFileW:
			break;
		case pSrcFileW:
			break;
		case pSrcResourceW:
			break;
		case ppDestBuf:
			break;
		case pFunction:
			break;
		case pTextureShader:
			break;
		case pBaseTexture:
			break;
		case pSrcBaseTexture:
			break;
		case pCubeTexture:
			break;
		case pSrcSurface:
			return L"pSrcSurface";
		case pSrcTexture:
			break;
		case pDestVolume:
			break;
		case pSrcVolume:
			break;
		case pVolumeTexture:
			break;
		case pData_Enum:
			break;
		case pDestPalette:
			return L"pDestPalette";
		case pPalette:
			break;
		case pSrcPalette:
			return L"pSrcPalette";
		case pDestRect:
			return L"pDestRect";
		case pSrcRect:
			return L"pSrcRect";
		case MipLevels:
			break;
		case Size:
			break;
		case SrcDataSize:
			break;
		case SrcLevel:
			break;
		case SrcPitch:
			break;
		case SrcRowPitch:
			break;
		case SrcSlicePitch:
			break;
	}

	return L"x";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD StereoSplitter::GetCommanderType(DWORD unCommanderIndex)
{
	return NOD_Plugtype::AQU_PNT_IDIRECT3DTEXTURE9;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoSplitter::GetDecommanderType(DWORD unDecommanderIndex)
{
	switch ((STS_Decommanders)unDecommanderIndex)
	{
		case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
			return NOD_Plugtype::AQU_UINT;
		case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case ppRenderTarget:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DSURFACE9;
		case STS_Decommanders::pNewZStencil:                  /**< ->SetDepthStencilSurface() stencil surface ***/
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case ppZStencilSurface:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DSURFACE9;
		case STS_Decommanders::Sampler:                       /**< ->SetTexture(), GetTexture() sampler index **/
			return NOD_Plugtype::AQU_UINT;
		case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
			return NOD_Plugtype::AQU_PNT_IDIRECT3DBASETEXTURE9;
		case STS_Decommanders::ppTextureGet:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DBASETEXTURE9;
		case pSourceSurface:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case pSourceRect:
			return NOD_Plugtype::AQU_PNT_RECT;
		case pDestinationSurface:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case pDestPoint:
			return NOD_Plugtype::AQU_PNT_POINT;
		case pSourceTexture:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DBASETEXTURE9;
		case pDestinationTexture:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DBASETEXTURE9;
		case pSurface:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case pRect:
			return NOD_Plugtype::AQU_PNT_RECT;
		case color:
			return NOD_Plugtype::AQU_D3DCOLOR;
		case pSourceSurface_StretchRect:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case pSourceRect_StretchRect:
			return NOD_Plugtype::AQU_PNT_RECT;
		case pDestSurface_StretchRect:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case pDestRect_StretchRect:
			return NOD_Plugtype::AQU_PNT_RECT;
		case Filter_StretchRect:
			return NOD_Plugtype::AQU_D3DTEXTUREFILTERTYPE;
		case State:
			return NOD_Plugtype::AQU_D3DRENDERSTATETYPE;
		case Value:
			return NOD_Plugtype::AQU_UINT;
		case pViewport:
			return NOD_Plugtype::AQU_PNT_D3DVIEWPORT9;
		case pRenderTargetGetData:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case iSwapChain:
			return NOD_Plugtype::AQU_UINT;
		case pDestSurface:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9;
		case iBackBuffer:
			return NOD_Plugtype::AQU_UINT;
		case Type:
			return NOD_Plugtype::AQU_D3DBACKBUFFER_TYPE;
		case ppBackBuffer:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DSURFACE9;
		case Width:
			return NOD_Plugtype::AQU_UINT;
		case Height:
			return NOD_Plugtype::AQU_UINT;
		case Length:
			return NOD_Plugtype::AQU_UINT;
		case EdgeLength:
			return NOD_Plugtype::AQU_UINT;
		case Levels:
			return NOD_Plugtype::AQU_UINT;
		case Depth:
			return NOD_Plugtype::AQU_UINT;
		case Usage:
			return NOD_Plugtype::AQU_UINT;
		case Format:
			return NOD_Plugtype::AQU_D3DFORMAT;
		case FVF:
			return NOD_Plugtype::AQU_UINT;
		case MultiSample:
			return NOD_Plugtype::AQU_D3DMULTISAMPLE_TYPE;
		case MultisampleQuality:
			return NOD_Plugtype::AQU_UINT;
		case Discard:
			return NOD_Plugtype::AQU_BOOL;
		case Lockable:
			return NOD_Plugtype::AQU_BOOL;
		case Pool:
			return NOD_Plugtype::AQU_D3DPOOL;
		case ppTexture:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DTEXTURE9;
		case ppVolumeTexture:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DVOLUMETEXTURE9;
		case ppCubeTexture:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DCUBETEXTURE9;
		case ppVertexBuffer:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DVERTEXBUFFER9;
		case ppIndexBuffer:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DINDEXBUFFER9;
		case ppSurface:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DSURFACE9;
		case pSharedHandle:
			return NOD_Plugtype::AQU_PNT_HANDLE;
		case SrcStartIndex:
			return NOD_Plugtype::AQU_UINT;                             /**< ->ProcessVertices() **/
		case DestIndex:
			return NOD_Plugtype::AQU_UINT;                             /**< ->ProcessVertices() **/
		case VertexCount:
			return NOD_Plugtype::AQU_UINT;                             /**< ->ProcessVertices() **/
		case pDestBuffer:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DVERTEXBUFFER9;       /**< ->ProcessVertices() **/
		case pVertexDecl:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DVERTEXDECLARATION9;  /**< ->ProcessVertices() **/
		case Flags:
			return NOD_Plugtype::AQU_UINT;                             /**< ->ProcessVertices() **/
		case StreamNumber:
			return NOD_Plugtype::AQU_UINT;                             /**< ->SetStreamSource(), ->GetStreamSource() **/
		case pStreamData:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DVERTEXBUFFER9;       /**< ->SetStreamSource(), **/
		case OffsetInBytes:
			return NOD_Plugtype::AQU_UINT;                             /**< ->SetStreamSource() **/
		case Stride:
			return NOD_Plugtype::AQU_UINT;                             /**< ->SetStreamSource() **/
		case ppStreamData:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DVERTEXBUFFER9;      /**< ->GetStreamSource() **/
		case pOffsetInBytes:
			return NOD_Plugtype::AQU_PNT_UINT;                         /**< ->GetStreamSource() **/
		case pStride:
			return NOD_Plugtype::AQU_PNT_UINT;                         /**< ->GetStreamSource() **/
		case pIndexData:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DINDEXBUFFER9;        /**< ->SetIndices() **/
		case ppIndexData:
			return NOD_Plugtype::AQU_PPNT_IDIRECT3DINDEXBUFFER9;       /**< ->GetIndices() **/
		case peDrawingSide:
			return NOD_Plugtype::AQU_INT;
		case pasVShaderConstantIndices:
			return NOD_Plugtype::AQU_VOID;
		case pasPShaderConstantIndices:
			return NOD_Plugtype::AQU_VOID;
		case pDestBox:
			break;
		case pSrcBox:
			break;
		case ColorKey:
			return AQU_D3DCOLOR;
		case SrcFormat:
			break;
		case DestFormat:
			break;
		case pSrcInfo:
			break;
		case Channel:
			break;
		case Filter:
			return AQU_UINT;
		case MipFilter:
			break;
		case Amplitude:
			break;
		case hSrcModule:
			break;
		case pDestFile:
			break;
		case pSrcFile:
			break;
		case pSrcResource:
			break;
		case pSrcData:
			break;
		case pSrcMemory:
			break;
		case pDestFileW:
			break;
		case pSrcFileW:
			break;
		case pSrcResourceW:
			break;
		case ppDestBuf:
			break;
		case pFunction:
			break;
		case pTextureShader:
			break;
		case pBaseTexture:
			break;
		case pSrcBaseTexture:
			break;
		case pCubeTexture:
			break;
		case pSrcSurface:
			return AQU_PNT_IDIRECT3DSURFACE9;
		case pSrcTexture:
			break;
		case pDestVolume:
			break;
		case pSrcVolume:
			break;
		case pVolumeTexture:
			break;
		case pData_Enum:
			break;
		case pDestPalette:
			return AQU_PNT_PALETTEENTRY;
		case pPalette:
			break;
		case pSrcPalette:
			return AQU_PNT_PALETTEENTRY;
		case pDestRect:
			return AQU_PNT_RECT;
		case pSrcRect:
			return AQU_PNT_RECT;
		case MipLevels:
			break;
		case Size:
			break;
		case SrcDataSize:
			break;
		case SrcLevel:
			break;
		case SrcPitch:
			break;
		case SrcRowPitch:
			break;
		case SrcSlicePitch:
			break;
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* StereoSplitter::GetOutputPointer(DWORD unCommanderIndex)
{
	switch ((STS_Commanders)unCommanderIndex)
	{
		case STS_Commanders::StereoTextureLeft:
			return (void*)&m_pcStereoBuffer[0];
		case STS_Commanders::StereoTextureRight:
			return (void*)&m_pcStereoBuffer[1];
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoSplitter::SetInputPointer(DWORD unDecommanderIndex, void* pData)
{
	switch ((STS_Decommanders)unDecommanderIndex)
	{
		case STS_Decommanders::RenderTargetIndex:                        /**< ->SetRenderTarget(), GetRenderTarget() render target index ***/
			m_punRenderTargetIndex = (DWORD*)pData;
			break;
		case STS_Decommanders::pRenderTarget:                            /**< ->SetRenderTarget() render target ***/
			m_ppcRenderTarget = (IDirect3DSurface9**)pData;
			break;
		case ppRenderTarget:                                             /**< ->GetRenderTarget() render target ***/
			m_pppcRenderTarget = (IDirect3DSurface9***)pData;
			break;
		case STS_Decommanders::pNewZStencil:                             /**< ->SetDepthStencilSurface() stencil surface ***/
			m_ppcNewZStencil = (IDirect3DSurface9**)pData;
			break;
		case ppZStencilSurface:
			m_pppcZStencilSurface = (IDirect3DSurface9***)pData;
			break;
		case STS_Decommanders::Sampler:                                  /**< ->SetTexture() sampler index **/
			m_punSampler = (DWORD*)pData;
			break;
		case STS_Decommanders::pTexture:                                 /**< ->SetTexture() texture pointer ***/
			m_ppcTexture = (IDirect3DBaseTexture9**)pData;
			break;
		case STS_Decommanders::ppTextureGet:
			m_pppcTexture = (IDirect3DBaseTexture9***)pData;
			break;
		case pSourceSurface:
			m_ppSourceSurface = (IDirect3DSurface9**)pData;              /**< ->UpdateSurface() source surface ***/
			break;
		case pSourceRect:
			m_ppcSourceRect = (RECT**)pData;                             /**< ->UpdateSurface() source rectangle ***/
			break;
		case pDestinationSurface:
			m_ppcDestinationSurface = (IDirect3DSurface9**)pData;        /**< ->UpdateSurface() destination surface ***/
			break;
		case pDestPoint:
			m_ppsDestPoint = (POINT**)pData;                             /**< ->UpdateSurface() destination point ***/
			break;
		case pSourceTexture:
			m_ppcSourceTexture = (IDirect3DBaseTexture9**)pData;         /**< ->UpdateTexture() source texture ***/
			break;
		case pDestinationTexture:
			m_ppcDestinationTexture = (IDirect3DBaseTexture9**)pData;    /**< ->UpdateTexture() destination texture ***/
			break;
		case pSurface:
			m_ppcSurface = (IDirect3DSurface9**)pData;                   /**< ->ColorFill() surface pointer ***/
			break;
		case pRect:
			m_ppsRect = (RECT**)pData;                                   /**< ->ColorFill() destination rectangle ***/
			break;
		case color:
			m_punColor = (D3DCOLOR*)pData;                               /**< ->ColorFill() destination color ***/
			break;
		case pSourceSurface_StretchRect:
			m_ppcSourceSurface_StretchRect = (IDirect3DSurface9**)pData; /**< ->StretchRect() source surface ***/
			break;
		case pSourceRect_StretchRect:
			m_ppcSourceRect_StretchRect = (RECT**)pData;                 /**< ->StretchRect() source rectangle ***/
			break;
		case pDestSurface_StretchRect:
			m_ppcDestSurface_StretchRect = (IDirect3DSurface9**)pData;   /**< ->StretchRect() destination surface ***/
			break;
		case pDestRect_StretchRect:
			m_ppcDestRect_StretchRect = (RECT**)pData;                   /**< ->StretchRect() destination rectangle ***/
			break;
		case Filter_StretchRect:
			m_peFilter_StretchRect = (D3DTEXTUREFILTERTYPE*)pData;       /**< ->StretchRect() filter ***/
			break;
		case State:
			m_peState = (D3DRENDERSTATETYPE*)pData;
			break;
		case Value:
			m_punValue = (DWORD*)pData;
			break;
		case pViewport:
			m_ppsViewport = (D3DVIEWPORT9**)pData;
			break;
		case pRenderTargetGetData:
			m_ppcRenderTargetGetData = (IDirect3DSurface9**)pData; /**< ->GetRenderTargetData() */
			break;
		case iSwapChain:
			m_punISwapChain = (UINT*)pData;                        /**< ->GetFrontBufferData() */
			break;
		case pDestSurface:
			m_ppcDestSurface = (IDirect3DSurface9**)pData;         /**< ->GetRenderTargetData() + GetFrontBufferData() */
			break;
		case iBackBuffer:
			m_punIBackBuffer = (UINT*)pData;
			break;
		case Type:
			m_peType = (D3DBACKBUFFER_TYPE*)pData;
			break;
		case ppBackBuffer:
			m_pppcBackBuffer = (IDirect3DSurface9***)pData;
			break;
		case Width:
			m_punWidth = (UINT*)pData;                             /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
			break;
		case Height:
			m_punHeight = (UINT*)pData;                            /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
			break;
		case Length:
			m_punLength = (UINT*)pData;
			break;
		case EdgeLength:
			m_punEdgeLength = (UINT*)pData;                        /**< ->CreateCubeTexture() **/
			break;
		case Levels:
			m_punLevels = (UINT*)pData;                            /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture() **/
			break;
		case Depth:
			m_punDepth = (UINT*)pData;                             /**< ->CreateVolumeTexture() **/
			break;
		case Usage:
			m_punUsage = (DWORD*)pData;                            /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture() **/
			break;
		case Format:
			m_peFormat = (D3DFORMAT*)pData;                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
			break;
		case FVF:
			m_punFVF = (DWORD*)pData;
			break;
		case MultiSample:
			m_peMultiSample = (D3DMULTISAMPLE_TYPE*)pData;         /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
			break;
		case MultisampleQuality:
			m_punMultisampleQuality = (DWORD*)pData;               /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
			break;
		case Discard:
			m_pnDiscard = (BOOL*)pData;                            /**< ->CreateDepthStencilSurface() **/
			break;
		case Lockable:
			m_pnLockable = (BOOL*)pData;                           /**< ->CreateRenderTarget() **/
			break;
		case Pool:
			m_pePool = (D3DPOOL*)pData;                            /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateOffscreenPlainSurface() **/
			break;
		case ppTexture:
			m_pppcTextureCreate = (IDirect3DTexture9***)pData;       /**< ->CreateTexture() **/
			break;
		case ppVolumeTexture:
			m_pppcVolumeTexture = (IDirect3DVolumeTexture9***)pData; /**< ->CreateVolumeTexture() **/
			break;
		case ppCubeTexture:
			m_pppcCubeTexture = (IDirect3DCubeTexture9***)pData;     /**< ->CreateCubeTexture() **/
			break;
		case ppVertexBuffer:
			m_pppcVertexBuffer = (IDirect3DVertexBuffer9***)pData;
			break;
		case ppIndexBuffer:
			m_pppcIndexBuffer = (IDirect3DIndexBuffer9***)pData;
			break;
		case ppSurface:
			m_pppcSurfaceCreate = (IDirect3DSurface9***)pData;       /**< ->CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
			break;
		case pSharedHandle:
			m_ppvSharedHandle = (HANDLE**)pData;
			break;
		case SrcStartIndex:
			m_punSrcStartIndex = (UINT*)pData;                       /**< ->ProcessVertices() **/
			break;
		case DestIndex:
			m_punDestIndex = (UINT*)pData;                           /**< ->ProcessVertices() **/
			break;
		case VertexCount:
			m_punVertexCount = (UINT*)pData;                         /**< ->ProcessVertices() **/
			break;
		case pDestBuffer:
			m_ppcDestBuffer = (IDirect3DVertexBuffer9**)pData;       /**< ->ProcessVertices() **/
			break;
		case pVertexDecl:
			m_ppcVertexDecl = (IDirect3DVertexDeclaration9**)pData;  /**< ->ProcessVertices() **/
			break;
		case Flags:
			m_punFlags = (DWORD*)pData;                              /**< ->ProcessVertices() **/
			break;
		case StreamNumber:
			m_punStreamNumber = (UINT*)pData;                        /**< ->SetStreamSource(), ->GetStreamSource() **/
			break;
		case pStreamData:
			m_ppcStreamData = (IDirect3DVertexBuffer9**)pData;       /**< ->SetStreamSource(), **/
			break;
		case OffsetInBytes:
			m_punOffsetInBytes = (UINT*)pData;                       /**< ->SetStreamSource() **/
			break;
		case Stride:
			m_punStride = (UINT*)pData;                              /**< ->SetStreamSource() **/
			break;
		case ppStreamData:
			m_pppcStreamData = (IDirect3DVertexBuffer9***)pData;     /**< ->GetStreamSource() **/
			break;
		case pOffsetInBytes:
			m_ppunOffsetInBytes = (UINT**)pData;                     /**< ->GetStreamSource() **/
			break;
		case pStride:
			m_ppunStride = (UINT**)pData;                            /**< ->GetStreamSource() **/
			break;
		case pIndexData:
			m_ppcIndexData = (IDirect3DIndexBuffer9**)pData;         /**< ->SetIndices() **/
			break;
		case ppIndexData:
			m_pppcIndexData = (IDirect3DIndexBuffer9***)pData;       /**< ->GetIndices() **/
			break;
		case peDrawingSide:
			m_peDrawingSide = (RenderPosition*)pData;
			break;
		case pasVShaderConstantIndices:
			m_ppasVSConstantRuleIndices = (std::vector<Vireio_Constant_Rule_Index_DX9>**)pData;
			break;
		case pasPShaderConstantIndices:
			m_ppasPSConstantRuleIndices = (std::vector<Vireio_Constant_Rule_Index_DX9>**)pData;
			break;
		case pDestBox:
			break;
		case pSrcBox:
			break;
		case ColorKey:
			m_punColorKey = (D3DCOLOR*)pData;
			break;
		case SrcFormat:
			break;
		case DestFormat:
			break;
		case pSrcInfo:
			break;
		case Channel:
			break;
		case Filter:
			m_punFilter = (DWORD*)pData;
			break;
		case MipFilter:
			break;
		case Amplitude:
			break;
		case hSrcModule:
			break;
		case pDestFile:
			break;
		case pSrcFile:
			break;
		case pSrcResource:
			break;
		case pSrcData:
			break;
		case pSrcMemory:
			break;
		case pDestFileW:
			break;
		case pSrcFileW:
			break;
		case pSrcResourceW:
			break;
		case ppDestBuf:
			break;
		case pFunction:
			break;
		case pTextureShader:
			break;
		case pBaseTexture:
			break;
		case pSrcBaseTexture:
			break;
		case pCubeTexture:
			break;
		case pSrcSurface:
			m_ppcSrcSurface = (IDirect3DSurface9**)pData;
			break;
		case pSrcTexture:
			break;
		case pDestVolume:
			break;
		case pSrcVolume:
			break;
		case pVolumeTexture:
			break;
		case pData_Enum:
			break;
		case pDestPalette:
			m_ppsDestPalette = (PALETTEENTRY**)pData;
			break;
		case pPalette:
			break;
		case pSrcPalette:
			m_ppsSrcPalette = (PALETTEENTRY**)pData;
			break;
		case pDestRect:
			m_ppsDestRect = (RECT**)pData;
			break;
		case pSrcRect:
			m_ppsSrcRect = (RECT**)pData;
			break;
		case MipLevels:
			break;
		case Size:
			break;
		case SrcDataSize:
			break;
		case SrcLevel:
			break;
		case SrcPitch:
			break;
		case SrcRowPitch:
			break;
		case SrcSlicePitch:
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
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETBACKBUFFER) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_UPDATESURFACE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_UPDATETEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_STRETCHRECT) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_COLORFILL) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETRENDERTARGET) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETDEPTHSTENCILSURFACE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETTEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_RESET) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_BEGINSCENE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_ENDSCENE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWRECTPATCH) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWTRIPATCH) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETRENDERSTATE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETVIEWPORT) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATETEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATEVOLUMETEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATECUBETEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATEVERTEXBUFFER) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATEINDEXBUFFER) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATERENDERTARGET) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATEDEPTHSTENCILSURFACE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETRENDERTARGETDATA) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETFRONTBUFFERDATA) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_CREATEOFFSCREENPLAINSURFACE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_PROCESSVERTICES) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETSTREAMSOURCE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETSTREAMSOURCE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETINDICES) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_GETINDICES))
				return true;
		}
		else if (nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
		{
			if ((nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT) ||
				(nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_GETBACKBUFFER) ||
				(nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_GETFRONTBUFFERDATA)) return true;
		}
		else if (nD3DInterface == INTERFACE_IDIRECT3DSTATEBLOCK9)
		{
			if (nD3DMethod == METHOD_IDIRECT3DSTATEBLOCK9_APPLY) return true;
		}
		else if (nD3DInterface == INTERFACE_D3DX9) return true;
	}
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
***/
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD unNumberConnected, int& nProvokerIndex)
{
	m_bUseD3D9Ex = true; // TODO !! DELETE !!

	// instantly return if the device is in use by a proxy class;
	if ((m_bUseD3D9Ex) && (s_bDeviceInUseByProxy)) return nullptr;

	//#define _DEBUG_STEREO_SPLITTER
	//#define _DEBUGTHIS
#ifdef _DEBUG_STEREO_SPLITTER
	{ wchar_t buf[128]; wsprintf(buf, L"[STS] if %u mt %u", eD3DInterface, eD3DMethod); OutputDebugString(buf); }
#endif

	static HRESULT nHr = S_OK;

	//if (m_bPresent)
	//nProvokerIndex |= AQU_PluginFlags::DoubleCallFlag;

	switch (eD3DInterface)
	{
#pragma region IDirect3DDevice9
		case INTERFACE_IDIRECT3DDEVICE9:
		{
										   switch (eD3DMethod)
										   {
#pragma region Present
											   case METHOD_IDIRECT3DDEVICE9_PRESENT:
												   Present((LPDIRECT3DDEVICE9)pThis, false);
												   return nullptr;
#pragma endregion
#pragma region BeginScene
											   case METHOD_IDIRECT3DDEVICE9_BEGINSCENE:
												   // ensure left drawing side here
												   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   return nullptr;
#pragma endregion 
#pragma region EndScene
											   case METHOD_IDIRECT3DDEVICE9_ENDSCENE:
												   // ensure left drawing side here
												   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
												   nHr = ((IDirect3DDevice9*)pThis)->EndScene();
												   return (void*)&nHr;
#pragma endregion 
#pragma region SetRenderTarget
											   case METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET:
												   if (!m_punRenderTargetIndex) return nullptr;
												   if (!m_ppcRenderTarget) return nullptr;

												   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   // cast proxy surface
													   IDirect3DStereoSurface9* newRenderTarget = static_cast<IDirect3DStereoSurface9*>(*m_ppcRenderTarget);

#ifdef _DEBUG
													   if (newRenderTarget && !newRenderTarget->GetActualLeft() && !newRenderTarget->GetActualRight())
													   {
														   OutputDebugString(L"[STS] RenderTarget is not a valid (IDirect3DStereoSurface) stereo capable surface\n");
													   }
#endif

													   //// Update actual render target ////

													   // Removing a render target
													   if (newRenderTarget == NULL)
													   {
														   if ((*m_punRenderTargetIndex) == 0)
														   {
															   // main render target should never be set to NULL
															   nHr = D3DERR_INVALIDCALL;
														   }
														   else
														   {
															   nHr = ((IDirect3DDevice9*)pThis)->SetRenderTarget(*m_punRenderTargetIndex, NULL);
														   }
													   }
													   // Setting a render target
													   else
													   {
														   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   {
															   nHr = ((IDirect3DDevice9*)pThis)->SetRenderTarget(*m_punRenderTargetIndex, newRenderTarget->GetActualLeft());
														   }
														   else
														   {
															   nHr = ((IDirect3DDevice9*)pThis)->SetRenderTarget(*m_punRenderTargetIndex, newRenderTarget->GetActualRight());
														   }
													   }

													   // release old render target
													   if (m_apcActiveRenderTargets[*m_punRenderTargetIndex] != NULL)
														   m_apcActiveRenderTargets[*m_punRenderTargetIndex]->Release();

													   //// update proxy collection of stereo render targets to reflect new actual render target ////
													   if (nHr == D3D_OK)
													   {
														   // changing rendertarget resets viewport to fullsurface
														   m_bActiveViewportIsDefault = true;

														   // replace with new render target (may be NULL)
														   m_apcActiveRenderTargets[*m_punRenderTargetIndex] = newRenderTarget;
														   if (m_apcActiveRenderTargets[*m_punRenderTargetIndex] != NULL)
															   m_apcActiveRenderTargets[*m_punRenderTargetIndex]->AddRef();
													   }
													   else
													   {
														   m_apcActiveRenderTargets[*m_punRenderTargetIndex] = NULL;
														   OutputDebugString(L"[STS] Failed to set render target !");
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else if (m_bPresent)
												   {

													   // call method
													   SetRenderTarget((LPDIRECT3DDEVICE9)pThis, *m_punRenderTargetIndex, *m_ppcRenderTarget);

													   // index too high ?
													   if ((*m_punRenderTargetIndex) >= D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT) return nullptr;

													   // replace the call
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetRenderTarget(*m_punRenderTargetIndex, m_apcActiveRenderTargets[*m_punRenderTargetIndex]);
													   }
													   else
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetRenderTarget(*m_punRenderTargetIndex, m_apcActiveRenderTargets[*m_punRenderTargetIndex + D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT]);
													   }

													   // changing rendertarget resets viewport to fullsurface
													   if (nHr == D3D_OK) m_bActiveViewportIsDefault = true;

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region SetDepthStencilSurface
											   case METHOD_IDIRECT3DDEVICE9_SETDEPTHSTENCILSURFACE:

												   SHOW_CALL("SetDepthStencilSurface");

												   if (!m_ppcNewZStencil) return nullptr;

												   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   // null parameter ?
													   if (!(*m_ppcNewZStencil))
													   {
														   if (m_pcActiveDepthStencilSurface[0])
														   {
															   m_pcActiveDepthStencilSurface[0]->Release();
														   }
														   m_pcActiveDepthStencilSurface[0] = nullptr;
														   nHr = S_OK;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // cast proxy surface
													   IDirect3DStereoSurface9* pNewDepthStencil = static_cast<IDirect3DStereoSurface9*>(*m_ppcNewZStencil);

													   IDirect3DSurface9* pActualStencilForCurrentSide = NULL;
													   if (pNewDepthStencil)
													   {
														   if (m_eCurrentRenderingSide == RenderPosition::Left)
															   pActualStencilForCurrentSide = pNewDepthStencil->GetActualLeft();
														   else
															   pActualStencilForCurrentSide = pNewDepthStencil->GetActualRight();
													   }

#ifdef _DEBUGTHIS
													   D3DSURFACE_DESC sDesc = {};
													   if (pActualStencilForCurrentSide) pActualStencilForCurrentSide->GetDesc(&sDesc);
													   DEBUG_UINT(sDesc.Format);
													   DEBUG_HEX(sDesc.Format);
#endif

													   // Update actual depth stencil
													   nHr = ((IDirect3DDevice9*)pThis)->SetDepthStencilSurface(pActualStencilForCurrentSide);

													   // Update stored proxy depth stencil
													   if (SUCCEEDED(nHr))
													   {
														   if (m_pcActiveDepthStencilSurface[0])
														   {
															   m_pcActiveDepthStencilSurface[0]->Release();
														   }

														   m_pcActiveDepthStencilSurface[0] = pNewDepthStencil;
														   if (m_pcActiveDepthStencilSurface[0])
														   {
															   m_pcActiveDepthStencilSurface[0]->AddRef();
														   }
													   }
													   else
													   {
														   { wchar_t buf[128]; wsprintf(buf, L"[STS] Failed to set depth stencil surface - Error %x - Ifc %x", nHr, pActualStencilForCurrentSide); OutputDebugString(buf); }
#ifdef _DEBUGTHIS
														   if (pActualStencilForCurrentSide)
														   {
															   D3DSURFACE_DESC sDesc = {};
															   pActualStencilForCurrentSide->GetDesc(&sDesc);
															   wchar_t buf[32];
															   wsprintf(buf, L"Width %u", sDesc.Width); OutputDebugString(buf);
															   wsprintf(buf, L"Height %u", sDesc.Height); OutputDebugString(buf);
															   wsprintf(buf, L"Format %u", sDesc.Format); OutputDebugString(buf);
															   wsprintf(buf, L"MultiSampleType %u", sDesc.MultiSampleType); OutputDebugString(buf);
															   wsprintf(buf, L"MultiSampleQuality %u", sDesc.MultiSampleQuality); OutputDebugString(buf);
															   wsprintf(buf, L"Pool %u", sDesc.Pool); OutputDebugString(buf);
															   wsprintf(buf, L"Type %u", sDesc.Type); OutputDebugString(buf);
															   wsprintf(buf, L"Usage %u", sDesc.Usage); OutputDebugString(buf);
														   }
#endif
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else if (m_bPresent)
												   {

													   // call method
													   SetDepthStencilSurface((LPDIRECT3DDEVICE9)pThis, *m_ppcNewZStencil);

													   // replace the call
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[0]);
													   }
													   else
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[1]);
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region SetTexture
											   case METHOD_IDIRECT3DDEVICE9_SETTEXTURE:

												   SHOW_CALL("SetTexture");

												   // get data
												   if (!m_punSampler) return nullptr;
												   if (!m_ppcTexture) return nullptr;

												   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   bool bDisplacement = false;
													   UINT unSampler = *m_punSampler;

													   // index too high ?
													   if ((unSampler) >= D3D9_SIMULTANEAOUS_TEXTURE_COUNT)
													   {
														   if (((unSampler) == D3DDMAPSAMPLER) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER0) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER1) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER2) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER3))
														   {
															   // displacement map
															   bDisplacement = true;
														   }
														   else
														   {
															   nHr = D3DERR_INVALIDCALL;

															   // method replaced, immediately return
															   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
															   return (void*)&nHr;
														   }
													   }

													   // texture provided ?
													   if (*m_ppcTexture)
													   {
														   IDirect3DBaseTexture9* pActualLeftTexture = NULL;
														   IDirect3DBaseTexture9* pActualRightTexture = NULL;

														   UnWrapProxyTexture(*m_ppcTexture, &pActualLeftTexture, &pActualRightTexture);

														   // Try and Update the actual devices textures -> use left (mono) if not stereo or one left side
														   if ((pActualRightTexture == NULL) || (m_eCurrentRenderingSide == RenderPosition::Left))
															   nHr = ((IDirect3DDevice9*)pThis)->SetTexture(unSampler, pActualLeftTexture);
														   else
															   nHr = ((IDirect3DDevice9*)pThis)->SetTexture(unSampler, pActualRightTexture);

													   }
													   else
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetTexture(unSampler, NULL);
													   }

													   // Update m_activeTextureStages if new texture was successfully set
													   if (SUCCEEDED(nHr))
													   {
														   if (bDisplacement)
														   {
															   // set sampler index zero based
															   unSampler -= D3DDMAPSAMPLER;

															   // remove existing texture that was active at Stage if there is one
															   if (m_apcActiveTexturesDisplacement[unSampler])
															   {
																   m_apcActiveTexturesDisplacement[unSampler]->Release();
																   m_apcActiveTexturesDisplacement[unSampler] = nullptr;
															   }

															   // insert new texture (can be a NULL pointer, this is important for StateBlock tracking)
															   m_apcActiveTexturesDisplacement[unSampler] = *m_ppcTexture;
															   if (m_apcActiveTexturesDisplacement[unSampler]) m_apcActiveTexturesDisplacement[unSampler]->AddRef();
														   }
														   else
														   {
															   // remove existing texture that was active at Stage if there is one
															   if (m_apcActiveTextures[unSampler])
															   {
																   m_apcActiveTextures[unSampler]->Release();
																   m_apcActiveTextures[unSampler] = nullptr;
															   }

															   // insert new texture (can be a NULL pointer, this is important for StateBlock tracking)
															   m_apcActiveTextures[unSampler] = *m_ppcTexture;
															   if (m_apcActiveTextures[unSampler]) m_apcActiveTextures[unSampler]->AddRef();
														   }
													   }
													   else
														   OutputDebugString(L"[STS] Failed to set texture !!");

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else if (m_bPresent)
												   {
													   // TODO !! DISPLACEMENT MAPPING !!

													   // call method
													   SetTexture((LPDIRECT3DDEVICE9)pThis, *m_punSampler, *m_ppcTexture);

													   // index too high ?
													   if ((*m_punSampler) >= D3D9_SIMULTANEAOUS_TEXTURE_COUNT) return nullptr;

													   // replace the call
													   if ((m_eCurrentRenderingSide == RenderPosition::Left) || (!(m_apcActiveTextures[*m_punSampler + D3D9_SIMULTANEAOUS_TEXTURE_COUNT])))
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetTexture(*m_punSampler, m_apcActiveTextures[*m_punSampler]);
													   }
													   else
													   {
														   nHr = ((IDirect3DDevice9*)pThis)->SetTexture(*m_punSampler, m_apcActiveTextures[*m_punSampler + D3D9_SIMULTANEAOUS_TEXTURE_COUNT]);
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region Clear
											   case METHOD_IDIRECT3DDEVICE9_CLEAR:
												   if (m_bPresent)
												   {
													   bool bSwitched = true;

													   // switch the drawing side before the second draw call is done
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
													   else if (m_eCurrentRenderingSide == RenderPosition::Right)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

													   // no second call if not switched -> immediate return
													   if (!bSwitched)
													   {
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   nHr = S_OK;
														   return (void*)&nHr;
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region DrawPrimitive
											   case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE:
												   if (m_bPresent)
												   {
													   bool bSwitched = true;

													   // switch the drawing side before the second draw call is done
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
													   else if (m_eCurrentRenderingSide == RenderPosition::Right)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

													   // no second call if not switched -> immediate return
													   if (!bSwitched)
													   {
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   nHr = S_OK;
														   return (void*)&nHr;
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region DrawIndexedPrimitive
											   case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE:
												   if (m_bPresent)
												   {
													   bool bSwitched = true;

													   // switch the drawing side before the second draw call is done
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
													   else if (m_eCurrentRenderingSide == RenderPosition::Right)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

													   // no second call if not switched -> immediate return
													   if (!bSwitched)
													   {
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   nHr = S_OK;
														   return (void*)&nHr;
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region DrawPrimitiveUP
											   case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP:
												   if (m_bPresent)
												   {
													   bool bSwitched = true;

													   // switch the drawing side before the second draw call is done
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
													   else if (m_eCurrentRenderingSide == RenderPosition::Right)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

													   // no second call if not switched -> immediate return
													   if (!bSwitched)
													   {
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   nHr = S_OK;
														   return (void*)&nHr;
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region DrawIndexedPrimitiveUP
											   case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP:
												   if (m_bPresent)
												   {
													   bool bSwitched = true;

													   // switch the drawing side before the second draw call is done
													   if (m_eCurrentRenderingSide == RenderPosition::Left)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
													   else if (m_eCurrentRenderingSide == RenderPosition::Right)
														   bSwitched = SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

													   // no second call if not switched -> immediate return
													   if (!bSwitched)
													   {
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   nHr = S_OK;
														   return (void*)&nHr;
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region UpdateSurface
											   case METHOD_IDIRECT3DDEVICE9_UPDATESURFACE:
												   if (!m_ppSourceSurface) return nullptr;
												   if (!m_ppcSourceRect) return nullptr;
												   if (!m_ppcDestinationSurface) return nullptr;
												   if (!m_ppsDestPoint) return nullptr;
												   if (!(*m_ppSourceSurface)) return nullptr;
												   if (!(*m_ppcDestinationSurface)) return nullptr;
												   {
													   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
													   if (m_bUseD3D9Ex)
													   {
														   IDirect3DSurface9* pSourceSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppSourceSurface))->GetActualLeft();
														   IDirect3DSurface9* pSourceSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppSourceSurface))->GetActualRight();
														   IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestinationSurface))->GetActualLeft();
														   IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestinationSurface))->GetActualRight();

														   nHr = ((IDirect3DDevice9*)pThis)->UpdateSurface(pSourceSurfaceLeft, *m_ppcSourceRect, pDestSurfaceLeft, *m_ppsDestPoint);

														   if (SUCCEEDED(nHr))
														   {
															   if (!pSourceSurfaceRight && pDestSurfaceRight)
															   {
																   // Source is not stereo, destination is stereo. Copying source to both sides of destination.
																   if (FAILED(((IDirect3DDevice9*)pThis)->UpdateSurface(pSourceSurfaceLeft, *m_ppcSourceRect, pDestSurfaceRight, *m_ppsDestPoint)))
																   {
																	   OutputDebugString(L"[STS] ERROR: UpdateSurface - Failed to copy source left to destination right.\n");
																   }
															   }
															   else if (pSourceSurfaceRight && !pDestSurfaceRight)
															   {
																   // Source is stereo, destination is not stereo. Copied Left side only.
															   }
															   else if (pSourceSurfaceRight && pDestSurfaceRight)
															   {
																   if (FAILED(((IDirect3DDevice9*)pThis)->UpdateSurface(pSourceSurfaceRight, *m_ppcSourceRect, pDestSurfaceRight, *m_ppsDestPoint)))
																   {
																	   OutputDebugString(L"[STS] ERROR: UpdateSurface - Failed to copy source right to destination right.\n");
																   }
															   }
														   }

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // get twins
													   IDirect3DSurface9* pcSurfaceDst = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcDestinationSurface);
													   IDirect3DSurface9* pcSurfaceSrc = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppSourceSurface);

													   if (pcSurfaceDst)
													   {
														   if (pcSurfaceSrc)
														   {
															   // Source is stereo, destination is stereo. Copying both.
															   ((IDirect3DDevice9*)pThis)->UpdateSurface(pcSurfaceSrc, *m_ppcSourceRect, pcSurfaceDst, *m_ppsDestPoint);
														   }
														   else
														   {
															   // Source is not stereo, destination is stereo. Copying source to both sides of destination
															   ((IDirect3DDevice9*)pThis)->UpdateSurface(*m_ppSourceSurface, *m_ppcSourceRect, pcSurfaceDst, *m_ppsDestPoint);
														   }
													   }
													   // else Source is maybe stereo, destination is not stereo. Copied left side only.
												   }
												   return nullptr;
#pragma endregion  
#pragma region UpdateTexture
											   case METHOD_IDIRECT3DDEVICE9_UPDATETEXTURE:
												   if (!m_ppcSourceTexture) return nullptr;
												   if (!m_ppcDestinationTexture) return nullptr;
												   if (!(*m_ppcSourceTexture)) return nullptr;
												   if (!(*m_ppcDestinationTexture)) return nullptr;
												   {
													   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
													   if (m_bUseD3D9Ex)
													   {
														   IDirect3DBaseTexture9* pSourceTextureLeft = NULL;
														   IDirect3DBaseTexture9* pSourceTextureRight = NULL;
														   IDirect3DBaseTexture9* pDestTextureLeft = NULL;
														   IDirect3DBaseTexture9* pDestTextureRight = NULL;

														   UnWrapProxyTexture(*m_ppcSourceTexture, &pSourceTextureLeft, &pSourceTextureRight);
														   UnWrapProxyTexture(*m_ppcDestinationTexture, &pDestTextureLeft, &pDestTextureRight);

														   nHr = ((IDirect3DDevice9*)pThis)->UpdateTexture(pSourceTextureLeft, pDestTextureLeft);

														   if (SUCCEEDED(nHr))
														   {
															   if (!pSourceTextureRight && pDestTextureRight)
															   {
																   // Source is not stereo, destination is stereo. Copying source to both sides of destination.
																   if (FAILED(((IDirect3DDevice9*)pThis)->UpdateTexture(pSourceTextureLeft, pDestTextureRight)))
																   {
																	   OutputDebugString(L"[STS] ERROR: UpdateTexture - Failed to copy source left to destination right.\n");
																   }
															   }
															   else if (pSourceTextureRight && !pDestTextureRight)
															   {
																   // Source is stereo, destination is not stereo. Copied Left side only.
															   }
															   else if (pSourceTextureRight && pDestTextureRight)
															   {
																   if (FAILED(((IDirect3DDevice9*)pThis)->UpdateTexture(pSourceTextureRight, pDestTextureRight)))
																   {
																	   OutputDebugString(L"[STS] ERROR: UpdateTexture - Failed to copy source right to destination right.\n");
																   }
															   }
														   }

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // get twins
													   IDirect3DBaseTexture9* pcTextureSrc = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcSourceTexture, false);
													   IDirect3DBaseTexture9* pcTextureDst = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcDestinationTexture, (pcTextureSrc != nullptr));

													   if (pcTextureDst)
													   {
														   if (pcTextureSrc)
														   {
															   // Source is stereo, destination is stereo. Copying both.
															   ((IDirect3DDevice9*)pThis)->UpdateTexture(pcTextureSrc, pcTextureDst);
														   }
														   else
														   {
															   // Source is not stereo, destination is stereo. Copying source to both sides of destination
															   ((IDirect3DDevice9*)pThis)->UpdateTexture(*m_ppcSourceTexture, pcTextureDst);
														   }
													   }
													   // else Source is maybe stereo, destination is not stereo. Copied left side only.
												   }
												   return nullptr;
#pragma endregion   
#pragma region StretchRect 
											   case METHOD_IDIRECT3DDEVICE9_STRETCHRECT:
												   if (!m_ppcSourceSurface_StretchRect) return nullptr;
												   if (!m_ppcSourceRect_StretchRect) return nullptr;
												   if (!m_ppcDestSurface_StretchRect) return nullptr;
												   if (!m_ppcDestRect_StretchRect) return nullptr;
												   if (!m_peFilter_StretchRect) return nullptr;
												   if (!(*m_ppcSourceSurface_StretchRect)) return nullptr;
												   if (!(*m_ppcDestSurface_StretchRect)) return nullptr;
												   {
													   SHOW_CALL("StrechRect");

													   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
													   if (m_bUseD3D9Ex)
													   {
														   IDirect3DStereoSurface9* pWrappedSource = static_cast<IDirect3DStereoSurface9*>(*m_ppcSourceSurface_StretchRect);
														   IDirect3DStereoSurface9* pWrappedDest = static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface_StretchRect);

														   IDirect3DSurface9* pSourceSurfaceLeft = pWrappedSource->GetActualLeft();
														   IDirect3DSurface9* pSourceSurfaceRight = pWrappedSource->GetActualRight();
														   IDirect3DSurface9* pDestSurfaceLeft = pWrappedDest->GetActualLeft();
														   IDirect3DSurface9* pDestSurfaceRight = pWrappedDest->GetActualRight();

														   nHr = ((IDirect3DDevice9*)pThis)->StretchRect(pSourceSurfaceLeft, *m_ppcSourceRect_StretchRect, pDestSurfaceLeft, *m_ppcDestRect_StretchRect, *m_peFilter_StretchRect);

														   if (SUCCEEDED(nHr))
														   {
															   if (!pSourceSurfaceRight && pDestSurfaceRight)
															   {
																   // Source is not stereo, destination is stereo. Copying source to both sides of destination.
																   if (FAILED(((IDirect3DDevice9*)pThis)->StretchRect(pSourceSurfaceLeft, *m_ppcSourceRect_StretchRect, pDestSurfaceRight, *m_ppcDestRect_StretchRect, *m_peFilter_StretchRect)))
																   {
																	   OutputDebugString(L"[STS] ERROR: StretchRect - Failed to copy source left to destination right.\n");
																   }
															   }
															   else if (pSourceSurfaceRight && !pDestSurfaceRight)
															   {
																   // Source is stereo, destination is not stereo. Copied Left side only.
															   }
															   else if (pSourceSurfaceRight && pDestSurfaceRight)
															   {
																   if (FAILED(((IDirect3DDevice9*)pThis)->StretchRect(pSourceSurfaceRight, *m_ppcSourceRect_StretchRect, pDestSurfaceRight, *m_ppcDestRect_StretchRect, *m_peFilter_StretchRect)))
																   {
																	   OutputDebugString(L"[STS] ERROR: StretchRect - Failed to copy source right to destination right.\n");
																   }
															   }
														   }

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // get twins
													   IDirect3DSurface9* pcSurfaceDst = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcDestSurface_StretchRect);
													   IDirect3DSurface9* pcSurfaceSrc = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcSourceSurface_StretchRect);

													   if (pcSurfaceDst)
													   {
														   if (pcSurfaceSrc)
														   {
															   // Source is stereo, destination is stereo. Copying both.
															   ((IDirect3DDevice9*)pThis)->StretchRect(pcSurfaceSrc, *m_ppcSourceRect_StretchRect, pcSurfaceDst, *m_ppcDestRect_StretchRect, *m_peFilter_StretchRect);
														   }
														   else
														   {
															   // Source is not stereo, destination is stereo. Copying source to both sides of destination
															   ((IDirect3DDevice9*)pThis)->StretchRect(*m_ppcSourceSurface_StretchRect, *m_ppcSourceRect_StretchRect, pcSurfaceDst, *m_ppcDestRect_StretchRect, *m_peFilter_StretchRect);
														   }
													   }
													   // else Source is maybe stereo, destination is not stereo. Copied left side only.
												   }
												   return nullptr;
#pragma endregion  
#pragma region ColorFill
											   case METHOD_IDIRECT3DDEVICE9_COLORFILL:
												   if (!m_ppcSurface) return nullptr;
												   if (!m_ppsRect) return nullptr;
												   if (!m_punColor) return nullptr;
												   if (!(*m_ppcSurface)) return nullptr;
												   {
													   // use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
													   if (m_bUseD3D9Ex)
													   {
														   IDirect3DStereoSurface9* pDerivedSurface = static_cast<IDirect3DStereoSurface9*>(*m_ppcSurface);
														   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->ColorFill(pDerivedSurface->GetActualLeft(), *m_ppsRect, *m_punColor)))
														   {
															   if (pDerivedSurface->IsStereo())
																   ((IDirect3DDevice9*)pThis)->ColorFill(pDerivedSurface->GetActualRight(), *m_ppsRect, *m_punColor);
														   }

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // get twin
													   IDirect3DSurface9* pcSurfaceTwin = VerifyPrivateDataInterfaces((IDirect3DDevice9*)pThis, *m_ppcSurface);

													   if (pcSurfaceTwin)
													   {
														   // call method for twin surface
														   ((IDirect3DDevice9*)pThis)->ColorFill(pcSurfaceTwin, *m_ppsRect, *m_punColor);
													   }
												   }
												   return nullptr;
#pragma endregion 
#pragma region GetBackBuffer
											   case METHOD_IDIRECT3DDEVICE9_GETBACKBUFFER:
												   // D3D9Ex device used ? in case enumerate swapchains and create proxy back buffers on request
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punISwapChain) return nullptr;
													   if (!m_punIBackBuffer) return nullptr;
													   if (!m_peType) return nullptr;
													   if (!m_pppcBackBuffer) return nullptr;

													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   // swapchain index not present ?
													   if ((*m_punISwapChain) >= (UINT)m_apcActiveSwapChains.size())
													   {
														   // enumerate swapchains in m_apcActiveSwapChains, backbuffers in m_aapcActiveProxyBackBufferSurfaces
														   for (UINT unI = (UINT)m_apcActiveSwapChains.size(); unI <= (*m_punISwapChain); unI++)
														   {
															   // get swapchain for the specified index
															   static IDirect3DSwapChain9* s_pcSwapChain = nullptr;
															   s_pcSwapChain = nullptr;
															   ((IDirect3DDevice9*)pThis)->GetSwapChain(unI, &s_pcSwapChain);

															   if (s_pcSwapChain)
															   {
																   // enumerate this swapchain
																   EnumerateSwapchain((IDirect3DDevice9*)pThis, s_pcSwapChain, unI);
																   s_pcSwapChain->Release();
															   }
															   else
															   {
																   // invalid call ?
																   OutputDebugString(L"[STS] Invalid call to ->GetBackBuffer().");
																   nHr = D3DERR_INVALIDCALL;

																   // method replaced, immediately return
																   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
																   return (void*)&nHr;
															   }
														   }
													   }

													   // swapchain index is active ?
													   if ((*m_punISwapChain) < (UINT)m_apcActiveSwapChains.size())
													   {
														   if ((*m_punIBackBuffer) < (UINT)m_aapcActiveProxyBackBufferSurfaces[*m_punISwapChain].size())
															   *(*m_pppcBackBuffer) = m_aapcActiveProxyBackBufferSurfaces[*m_punISwapChain][*m_punIBackBuffer];
														   if (*(*m_pppcBackBuffer)) (*(*m_pppcBackBuffer))->AddRef();

														   nHr = D3D_OK;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   OutputDebugString(L"[STS] Fatal : GetBackBuffer no swapchain !");
													   exit(99);
												   }
												   else
												   {
													   // ensure left drawing side here
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion 
#pragma region GetRenderTarget
											   case METHOD_IDIRECT3DDEVICE9_GETRENDERTARGET:
												   if (m_bUseD3D9Ex)
												   {
													   SHOW_CALL("GetRenderTarget");

													   if (!m_punRenderTargetIndex) return nullptr;
													   if (!m_pppcRenderTarget) return nullptr;

													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   if (*m_punRenderTargetIndex >= D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT)
													   {
														   nHr = D3DERR_INVALIDCALL;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   IDirect3DSurface9* pcTargetToReturn = m_apcActiveRenderTargets[*m_punRenderTargetIndex];
													   if (!pcTargetToReturn)
													   {
														   nHr = D3DERR_NOTFOUND;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }
													   else
													   {
														   // set return target and add a reference
														   *(*m_pppcRenderTarget) = pcTargetToReturn;
														   pcTargetToReturn->AddRef();

														   nHr = D3D_OK;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   OutputDebugString(L"[STS] Critical : Failed to return render target !");
													   exit(99);
												   }
												   else
												   {
													   // ensure left drawing side here
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion 
#pragma region GetDepthStencilSurface
											   case METHOD_IDIRECT3DDEVICE9_GETDEPTHSTENCILSURFACE:
												   if (m_bUseD3D9Ex)
												   {
													   SHOW_CALL("GetDepthStencilSurface");

													   if (!m_pppcZStencilSurface) return nullptr;
													   if (!(*m_pppcZStencilSurface)) return nullptr;

													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   if (!m_pcActiveDepthStencilSurface[0])
													   {
														   *m_pppcZStencilSurface = nullptr;
														   nHr = D3DERR_NOTFOUND;

														   // method replaced, immediately return
														   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
														   return (void*)&nHr;
													   }

													   // return stored depth stencil
													   *(*m_pppcZStencilSurface) = m_pcActiveDepthStencilSurface[0];
													   (*(*m_pppcZStencilSurface))->AddRef();
													   nHr = D3D_OK;

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else
												   {
													   // ensure left drawing side here
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion  
#pragma region GetTexture 
											   case METHOD_IDIRECT3DDEVICE9_GETTEXTURE:
												   if (m_bUseD3D9Ex)
												   {
													   SHOW_CALL("GetTexture");

													   if (!m_punSampler) return nullptr;
													   if (!m_pppcTexture) return nullptr;

													   if (!m_bPresent)
														   Present((IDirect3DDevice9*)pThis, true);

													   bool bDisplacement = false;
													   UINT unSampler = *m_punSampler;

													   // index too high ?
													   if ((unSampler) >= D3D9_SIMULTANEAOUS_TEXTURE_COUNT)
													   {
														   if (((unSampler) == D3DDMAPSAMPLER) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER0) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER1) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER2) ||
															   ((unSampler) == D3DVERTEXTEXTURESAMPLER3))
														   {
															   // displacement map
															   bDisplacement = true;
														   }
														   else
														   {
															   nHr = D3DERR_INVALIDCALL;

															   // method replaced, immediately return
															   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
															   return (void*)&nHr;
														   }
													   }

													   if (bDisplacement)
													   {
														   // set sampler index zero based
														   unSampler -= D3DDMAPSAMPLER;

														   // displacement texture
														   *(*m_pppcTexture) = m_apcActiveTexturesDisplacement[unSampler];
													   }
													   else
													   {
														   // casual texture
														   *(*m_pppcTexture) = m_apcActiveTextures[unSampler];
													   }

													   if (*(*m_pppcTexture))
														   (*(*m_pppcTexture))->AddRef();

													   nHr = D3D_OK;

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else
												   {
													   // ensure left drawing side here
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion  
#pragma region Reset
											   case METHOD_IDIRECT3DDEVICE9_RESET:
												   if (m_bUseD3D9Ex)
												   {
													   OutputDebugString(L"[STS] ->Reset() NOT IMPLEMENTED !!");
													   exit(99);
												   }
												   else
												   {
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion 
#pragma region DrawRectPatch
											   case METHOD_IDIRECT3DDEVICE9_DRAWRECTPATCH:
												   // TODO !!
												   return nullptr;
#pragma endregion  
#pragma region DrawTriPatch
											   case METHOD_IDIRECT3DDEVICE9_DRAWTRIPATCH:
												   // TODO !!
												   return nullptr;
#pragma endregion 
#pragma region SetRenderState
											   case METHOD_IDIRECT3DDEVICE9_SETRENDERSTATE:
												   if (!m_peState) return nullptr;
												   if (!m_punValue) return nullptr;
												   else
												   {
													   // catch D3DRS_POINT for "Resolve Z-Buffer" operation... read "Advanced-DX9-Capabilities-for-ATI-Radeon-Cards_v2.pdf" for more info
													   if (*m_peState == D3DRENDERSTATETYPE::D3DRS_POINTSIZE)
													   {
														   // the value 0x3f800000 seems to be also a resolve z buffer request
														   // - it is send by Skyrim on AMD cards
														   // however, the following code works to do it
														   DWORD unValue = *m_punValue;
														   if ((unValue == 0x3f800000) || (unValue == RESZ_CODE))
														   {
															   // Perform a dummy draw call to ensure texture sampler 0 is set before the
															   // resolve is triggered
															   // Vertex declaration and shaders may need to me adjusted to ensure no debug
															   // error message is produced
															   D3DXVECTOR3 vDummyPoint(0.0f, 0.0f, 0.0f);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZENABLE, FALSE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
															   ((IDirect3DDevice9*)pThis)->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vDummyPoint, sizeof(D3DXVECTOR3));
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZENABLE, TRUE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);

															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_POINTSIZE, unValue);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_POINTSIZE, 0);

															   // switch the drawing side before the second resolve is done
															   if (m_eCurrentRenderingSide == RenderPosition::Left)
																   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Right);
															   else if (m_eCurrentRenderingSide == RenderPosition::Right)
																   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);

															   // do the same procedure for the second target
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZENABLE, FALSE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
															   ((IDirect3DDevice9*)pThis)->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vDummyPoint, sizeof(D3DXVECTOR3));
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_ZENABLE, TRUE);
															   ((IDirect3DDevice9*)pThis)->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
														   }
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region SetViewport
											   case METHOD_IDIRECT3DDEVICE9_SETVIEWPORT:
												   if (m_ppsViewport)
												   {
													   nHr = SetViewport((IDirect3DDevice9*)pThis, *m_ppsViewport);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateTexture
											   case METHOD_IDIRECT3DDEVICE9_CREATETEXTURE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punWidth) return nullptr;
													   if (!m_punHeight) return nullptr;
													   if (!m_punLevels) return nullptr;
													   if (!m_punUsage) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_pePool) return nullptr;
													   if (!m_pppcTextureCreate) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

#ifdef _DEBUGTHIS
													   wchar_t buf[32];
													   wsprintf(buf, L"m_punWidth %u", *m_punWidth); OutputDebugString(buf);
													   wsprintf(buf, L"m_punHeight %u", *m_punHeight); OutputDebugString(buf);
													   wsprintf(buf, L"m_punLevels %u", *m_punLevels); OutputDebugString(buf);
													   wsprintf(buf, L"m_punUsage %u", *m_punUsage); OutputDebugString(buf);
													   wsprintf(buf, L"m_peFormat %u", *m_peFormat); OutputDebugString(buf);
													   wsprintf(buf, L"m_pePool %u", *m_pePool); OutputDebugString(buf);
#endif
													   SHOW_CALL("CreateTexture");

													   static IDirect3DTexture9* s_pcLeftTexture = NULL;
													   static IDirect3DTexture9* s_pcRightTexture = NULL;
													   s_pcLeftTexture = NULL;
													   s_pcRightTexture = NULL;

													   D3DPOOL ePool = *m_pePool;
													   D3DFORMAT eFormat = *m_peFormat;

													   HRESULT hr = S_OK;
													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   ePool = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }

													   if ((*m_punUsage) & D3DUSAGE_RENDERTARGET)
													   {
														   // compressed formats not working in d3d9ex ?? verify this !!
														   eFormat = GetD3D9ExFormat(*m_peFormat);
													   }

													   // INTZ not supported in IDirect3DDevice9Ex ??
													   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create texture format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   // try and create left
													   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->CreateTexture(*m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcLeftTexture, *m_ppvSharedHandle)))
													   {
														   // Does this Texture need duplicating?
														   if (ShouldDuplicateTexture(*m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, *m_peFormat, *m_pePool))
														   {
															   if (FAILED(((IDirect3DDevice9*)pThis)->CreateTexture(*m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcRightTexture, *m_ppvSharedHandle)))
															   {
																   OutputDebugString(L"[STS] Failed to create right eye texture while attempting to create stereo pair, falling back to mono\n");
																   s_pcRightTexture = NULL;
															   }
														   }
													   }
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create texture IDirect3DTexture9\n");
													   }

													   if (SUCCEEDED(nHr))
													   {
														   *(*m_pppcTextureCreate) = new IDirect3DStereoTexture9(s_pcLeftTexture, s_pcRightTexture, ((IDirect3DDevice9*)pThis));
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateVolumeTexture
											   case  METHOD_IDIRECT3DDEVICE9_CREATEVOLUMETEXTURE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punWidth) return nullptr;
													   if (!m_punHeight) return nullptr;
													   if (!m_punDepth) return nullptr;
													   if (!m_punLevels) return nullptr;
													   if (!m_punUsage) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_pePool) return nullptr;
													   if (!m_pppcVolumeTexture) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   D3DPOOL ePool = D3DPOOL_DEFAULT;
													   D3DFORMAT eFormat = *m_peFormat;

													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   ePool = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }

													   if ((*m_punUsage) & D3DUSAGE_RENDERTARGET)
													   {
														   // compressed formats not working in d3d9ex ?? verify this !!
														   eFormat = GetD3D9ExFormat(*m_peFormat);
													   }

													   // INTZ not supported in IDirect3DDevice9Ex ??
													   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create volume texture format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   static IDirect3DVolumeTexture9* s_pcActualTexture = NULL;
													   s_pcActualTexture = NULL;
													   nHr = ((IDirect3DDevice9*)pThis)->CreateVolumeTexture(*m_punWidth, *m_punHeight, *m_punDepth, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcActualTexture, *m_ppvSharedHandle);

													   if (SUCCEEDED(nHr))
														   *(*m_pppcVolumeTexture) = new IDirect3DStereoVolumeTexture9(s_pcActualTexture, ((IDirect3DDevice9*)pThis), (*m_pePool));
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create texture IDirect3DVolumeTexture9\n");
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateCubeTexture
											   case  METHOD_IDIRECT3DDEVICE9_CREATECUBETEXTURE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punEdgeLength) return nullptr;
													   if (!m_punLevels) return nullptr;
													   if (!m_punUsage) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_pePool) return nullptr;
													   if (!m_pppcCubeTexture) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   D3DPOOL ePool = *m_pePool;
													   D3DFORMAT eFormat = *m_peFormat;

													   // query d3d9ex device
													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   ePool = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }

													   if ((*m_punUsage) & D3DUSAGE_RENDERTARGET)
													   {
														   // compressed formats not working in d3d9ex ?? verify this !!
														   eFormat = GetD3D9ExFormat(*m_peFormat);
													   }

													   // INTZ not supported in IDirect3DDevice9Ex ??
													   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;

													   static IDirect3DCubeTexture9* s_pcLeftCubeTexture = NULL;
													   s_pcLeftCubeTexture = NULL;
													   static IDirect3DCubeTexture9* s_pcRightCubeTexture = NULL;
													   s_pcRightCubeTexture = NULL;

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create cube texture format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   // try and create left
													   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->CreateCubeTexture(*m_punEdgeLength, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcLeftCubeTexture, *m_ppvSharedHandle)))
													   {

														   // Does this Texture need duplicating?
														   if (ShouldDuplicateCubeTexture(*m_punEdgeLength, *m_punLevels, *m_punUsage, *m_peFormat, *m_pePool))
														   {

															   if (FAILED(((IDirect3DDevice9*)pThis)->CreateCubeTexture(*m_punEdgeLength, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcRightCubeTexture, *m_ppvSharedHandle)))
															   {
																   OutputDebugString(L"[STS] Failed to create right eye texture while attempting to create stereo pair, falling back to mono\n");
																   s_pcRightCubeTexture = NULL;
															   }
														   }
													   }
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create texture : IDirect3DCubeTexture9 \n");
													   }

													   if (SUCCEEDED(nHr))
														   *(*m_pppcCubeTexture) = new IDirect3DStereoCubeTexture9(s_pcLeftCubeTexture, s_pcRightCubeTexture, (IDirect3DDevice9*)pThis);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateVertexBuffer
											   case  METHOD_IDIRECT3DDEVICE9_CREATEVERTEXBUFFER:
												   if (m_bUseD3D9Ex)
												   {
													   SHOW_CALL("CreateVertexBuffer");

													   if (!m_punLength) return nullptr;
													   if (!m_punUsage) return nullptr;
													   if (!m_pePool) return nullptr;
													   if (!m_punFVF) return nullptr;
													   if (!m_pppcVertexBuffer) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   HRESULT hr = S_OK;
													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   (*m_pePool) = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }

													   static IDirect3DVertexBuffer9* s_pcActualBuffer = NULL;
													   s_pcActualBuffer = NULL;
													   nHr = ((IDirect3DDevice9*)pThis)->CreateVertexBuffer(*m_punLength, *m_punUsage, *m_punFVF, *m_pePool, &s_pcActualBuffer, *m_ppvSharedHandle);

													   if (SUCCEEDED(nHr))
														   *(*m_pppcVertexBuffer) = new IDirect3DProxyVertexBuffer9(s_pcActualBuffer, ((IDirect3DDevice9*)pThis));

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateIndexBuffer
											   case  METHOD_IDIRECT3DDEVICE9_CREATEINDEXBUFFER:
												   if (m_bUseD3D9Ex)
												   {
													   SHOW_CALL("CreateIndexBuffer");
													   if (!m_pePool) return nullptr;

													   HRESULT hr = S_OK;
													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   (*m_pePool) = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateRenderTarget
											   case  METHOD_IDIRECT3DDEVICE9_CREATERENDERTARGET:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punWidth) return nullptr;
													   if (!m_punHeight) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_peMultiSample) return nullptr;
													   if (!m_punMultisampleQuality) return nullptr;
													   if (!m_pnLockable) return nullptr;
													   if (!m_pppcSurfaceCreate) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   SHOW_CALL("CreateRenderTarget");

													   static IDirect3DSurface9* s_pcLeftRenderTarget = NULL;
													   s_pcLeftRenderTarget = NULL;
													   static IDirect3DSurface9* s_pcRightRenderTarget = NULL;
													   s_pcRightRenderTarget = NULL;

													   // create left/mono
													   HANDLE sharedHandleLeft = NULL;
													   HANDLE sharedHandleRight = NULL;

													   // Override multisampling and compressed formats if DX9Ex
													   D3DFORMAT eFormat = *m_peFormat;
													   D3DMULTISAMPLE_TYPE eMultiSample = *m_peMultiSample;
													   DWORD eMultiSampleQuality = *m_punMultisampleQuality;

													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))))
													   {
														   *m_ppvSharedHandle = &sharedHandleLeft;
														   eMultiSample = D3DMULTISAMPLE_NONE;
														   eMultiSampleQuality = 0;

														   // compressed formats not working in d3d9ex ?? verify this !!
														   eFormat = GetD3D9ExFormat(*m_peFormat);

														   // INTZ not supported in IDirect3DDevice9Ex ??
														   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;
													   }

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create render target format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->CreateRenderTarget(*m_punWidth, *m_punHeight, eFormat, eMultiSample, eMultiSampleQuality, *m_pnLockable, &s_pcLeftRenderTarget, *m_ppvSharedHandle)))
													   {
														   /* "If Needed" heuristic is the complicated part here.
														   Fixed heuristics (based on type, format, size, etc) + game specific overrides + isForcedMono + magic? */

														   // TODO Should we duplicate this Render Target? Replace "true" with heuristic
														   if (ShouldDuplicateRenderTarget(*m_punWidth, *m_punHeight, *m_peFormat, eMultiSample, eMultiSampleQuality, *m_pnLockable, false))
														   {
															   if (s_pcDirect3DDevice9Ex)
																   *m_ppvSharedHandle = &sharedHandleRight;

															   if (FAILED(((IDirect3DDevice9*)pThis)->CreateRenderTarget(*m_punWidth, *m_punHeight, eFormat, eMultiSample, eMultiSampleQuality, *m_pnLockable, &s_pcRightRenderTarget, *m_ppvSharedHandle)))
															   {
																   OutputDebugString(L"[STS] Failed to create right eye render target while attempting to create stereo pair, falling back to mono\n");
																   s_pcRightRenderTarget = NULL;
															   }
														   }
													   }
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create render target\n");
#ifdef _DEBUGTHIS
														   wchar_t buf[32];
														   wsprintf(buf, L"Width %u", *m_punWidth); OutputDebugString(buf);
														   wsprintf(buf, L"Height %u", *m_punHeight); OutputDebugString(buf);
														   wsprintf(buf, L"Format %x", eFormat); OutputDebugString(buf);
														   wsprintf(buf, L"MultiSampleType %u", eMultiSample); OutputDebugString(buf);
														   wsprintf(buf, L"MultiSampleQuality %u", eMultiSampleQuality); OutputDebugString(buf);
														   wsprintf(buf, L"Lockable %u", *m_pnLockable); OutputDebugString(buf);
														   wsprintf(buf, L"SharedHandle %x", *m_ppvSharedHandle); OutputDebugString(buf);
														   wsprintf(buf, L"HRESULT %x", nHr); OutputDebugString(buf);
#endif
													   }

													   if (SUCCEEDED(nHr))
													   {
														   *(*m_pppcSurfaceCreate) = new IDirect3DStereoSurface9(s_pcLeftRenderTarget, s_pcRightRenderTarget, (IDirect3DDevice9*)pThis, NULL, sharedHandleLeft, sharedHandleRight);
													   }

													   if (s_pcDirect3DDevice9Ex)
														   s_pcDirect3DDevice9Ex->Release();

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateDepthStencilSurface
											   case  METHOD_IDIRECT3DDEVICE9_CREATEDEPTHSTENCILSURFACE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punWidth) return nullptr;
													   if (!m_punHeight) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_peMultiSample) return nullptr;
													   if (!m_punMultisampleQuality) return nullptr;
													   if (!m_pnDiscard) return nullptr;
													   if (!m_pppcSurfaceCreate) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   SHOW_CALL("CreateDepthStencilSurface");

													   static IDirect3DSurface9* s_pcDepthStencilSurfaceLeft = NULL;
													   s_pcDepthStencilSurfaceLeft = NULL;
													   static IDirect3DSurface9* s_pcDepthStencilSurfaceRight = NULL;
													   s_pcDepthStencilSurfaceRight = NULL;

													   // Override multisampling if DX9Ex
													   D3DMULTISAMPLE_TYPE eMultiSample = *m_peMultiSample;
													   DWORD eMultiSampleQuality = *m_punMultisampleQuality;
													   D3DFORMAT eFormat = *m_peFormat;

													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))))
													   {
														   eMultiSample = D3DMULTISAMPLE_NONE;
														   eMultiSampleQuality = 0;

														   // INTZ not supported in IDirect3DDevice9Ex ??
														   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;

														   s_pcDirect3DDevice9Ex->Release();
													   }

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create depth stencil format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   // create left/mono
													   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->CreateDepthStencilSurface(*m_punWidth, *m_punHeight, eFormat, eMultiSample, eMultiSampleQuality, *m_pnDiscard, &s_pcDepthStencilSurfaceLeft, *m_ppvSharedHandle)))
													   {
														   // TODO Should we always duplicated Depth stencils? I think yes, but there may be exceptions
														   if (ShouldDuplicateDepthStencilSurface(*m_punWidth, *m_punHeight, *m_peFormat, eMultiSample, eMultiSampleQuality, *m_pnDiscard))
														   {
															   if (FAILED(((IDirect3DDevice9*)pThis)->CreateDepthStencilSurface(*m_punWidth, *m_punHeight, eFormat, eMultiSample, eMultiSampleQuality, *m_pnDiscard, &s_pcDepthStencilSurfaceRight, *m_ppvSharedHandle)))
															   {
																   OutputDebugString(L"[STS] Failed to create right eye Depth Stencil Surface while attempting to create stereo pair, falling back to mono\n");
																   s_pcDepthStencilSurfaceRight = NULL;
															   }
														   }
													   }
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create Depth Stencil Surface\n");
													   }

													   if (SUCCEEDED(nHr))
														   *(*m_pppcSurfaceCreate) = new IDirect3DStereoSurface9(s_pcDepthStencilSurfaceLeft, s_pcDepthStencilSurfaceRight, (IDirect3DDevice9*)pThis, NULL, NULL, NULL);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region CreateOffscreenPlainSurface
											   case METHOD_IDIRECT3DDEVICE9_CREATEOFFSCREENPLAINSURFACE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punWidth) return nullptr;
													   if (!m_punHeight) return nullptr;
													   if (!m_peFormat) return nullptr;
													   if (!m_pePool) return nullptr;
													   if (!m_pppcSurfaceCreate) return nullptr;
													   if (!m_ppvSharedHandle) return nullptr;

													   SHOW_CALL("CreateOffscreenPlainSurface");

													   D3DPOOL ePool = *m_pePool;
													   D3DFORMAT eFormat = *m_peFormat;

													   HRESULT hr = S_OK;
													   static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
													   s_pcDirect3DDevice9Ex = NULL;
													   if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
														   (*m_pePool) == D3DPOOL_MANAGED)
													   {
														   ePool = D3DPOOL_DEFAULT;
														   s_pcDirect3DDevice9Ex->Release();
													   }

													   // INTZ not supported in IDirect3DDevice9Ex ??
													   if (eFormat == D3DFMT_INTZ) eFormat = D3DFMT_D24S8;

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create offscreen plain format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   static IDirect3DSurface9* s_pcActualSurface = NULL;
													   s_pcActualSurface = NULL;
													   HRESULT creationResult = ((IDirect3DDevice9*)pThis)->CreateOffscreenPlainSurface(*m_punWidth, *m_punHeight, eFormat, ePool, &s_pcActualSurface, *m_ppvSharedHandle);
													   if (SUCCEEDED(creationResult))
													   {
														   *(*m_pppcSurfaceCreate) = new IDirect3DStereoSurface9(s_pcActualSurface, NULL, (IDirect3DDevice9*)pThis, NULL, NULL, NULL);
													   }
													   else
													   {
														   OutputDebugString(L"[STS] Failed to create surface IDirect3DSurface9\n");
													   }

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region GetRendertargetData
											   case  METHOD_IDIRECT3DDEVICE9_GETRENDERTARGETDATA:
												   if (m_bUseD3D9Ex)
												   {
													   OutputDebugString(L"[STS] NOT IMPLEMENTED : GetRenderTargetData()");
													   exit(99);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   else
												   {
													   // ensure left drawing side here
													   SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
												   }
												   return nullptr;
#pragma endregion
#pragma region GetFrontBufferData
											   case METHOD_IDIRECT3DDEVICE9_GETFRONTBUFFERDATA:
												   if (m_bUseD3D9Ex)
												   {
													   OutputDebugString(L"[STS] NOT IMPLEMENTED : GetFrontBufferData()");
													   exit(99);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region ProcessVertices
											   case METHOD_IDIRECT3DDEVICE9_PROCESSVERTICES:
												   if (m_bUseD3D9Ex)
												   {
													   OutputDebugString(L"[STS] NOT IMPLEMENTED : ProcessVertices()");
													   exit(99);

													   if (!m_punSrcStartIndex) return nullptr; /**< ->ProcessVertices() **/
													   if (!m_punDestIndex) return nullptr; /**< ->ProcessVertices() **/
													   if (!m_punVertexCount) return nullptr; /**< ->ProcessVertices() **/
													   if (!m_ppcDestBuffer) return nullptr; /**< ->ProcessVertices() **/
													   if (!m_ppcVertexDecl) return nullptr; /**< ->ProcessVertices() **/
													   if (!m_punFlags) return nullptr; /**< ->ProcessVertices() **/

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region SetStreamSource
											   case METHOD_IDIRECT3DDEVICE9_SETSTREAMSOURCE:
												   if (m_bUseD3D9Ex)
												   {
													   if (!m_punStreamNumber) return nullptr; /**< ->SetStreamSource(), ->GetStreamSource() **/
													   if (!m_ppcStreamData) return nullptr; /**< ->SetStreamSource(), **/
													   if (!m_punOffsetInBytes) return nullptr; /**< ->SetStreamSource() **/
													   if (!m_punStride) return nullptr; /**< ->SetStreamSource() **/
													   if (!(*m_ppcStreamData)) return nullptr;

													   // cast proxy
													   IDirect3DProxyVertexBuffer9* pNewBuffer = static_cast<IDirect3DProxyVertexBuffer9*>(*m_ppcStreamData);

													   // set actual buffer
													   nHr = ((IDirect3DDevice9*)pThis)->SetStreamSource(*m_punStreamNumber, pNewBuffer->GetActual(), *m_punOffsetInBytes, *m_punStride);

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region GetStreamSource
											   case METHOD_IDIRECT3DDEVICE9_GETSTREAMSOURCE:
												   if (m_bUseD3D9Ex)
												   {
													   OutputDebugString(L"[STS] NOT IMPLEMENTED : GetStreamSource()");
													   exit(99);

													   if (!m_punStreamNumber) return nullptr; /**< ->SetStreamSource(), ->GetStreamSource() **/
													   if (!m_pppcStreamData) return nullptr; /**< ->GetStreamSource() **/
													   if (!m_ppunOffsetInBytes) return nullptr; /**< ->GetStreamSource() **/
													   if (!m_ppunStride) return nullptr; /**< ->GetStreamSource() **/

													   // method replaced, immediately return
													   nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
													   return (void*)&nHr;
												   }
												   return nullptr;
#pragma endregion
#pragma region SetIndices
											   case METHOD_IDIRECT3DDEVICE9_SETINDICES:
												   if (m_bUseD3D9Ex)
												   {
												   }
												   return nullptr;
#pragma endregion
#pragma region GetIndices
											   case METHOD_IDIRECT3DDEVICE9_GETINDICES:
												   if (m_bUseD3D9Ex)
												   {
												   }
												   return nullptr;
#pragma endregion
										   }
										   return nullptr;
		}
#pragma endregion
#pragma region IDirect3DSwapChain9
		case INTERFACE_IDIRECT3DSWAPCHAIN9:
			switch (eD3DMethod)
			{
				case METHOD_IDIRECT3DSWAPCHAIN9_PRESENT:
				{
														   // get the device and call present
														   IDirect3DDevice9* pcDevice = nullptr;
														   if (pcDevice)
														   {
															   ((LPDIRECT3DSWAPCHAIN9)pThis)->GetDevice(&pcDevice);
															   Present(pcDevice, false);
															   pcDevice->Release();
														   }
				}
					return nullptr;
				case METHOD_IDIRECT3DSWAPCHAIN9_GETBACKBUFFER:
					if (m_bUseD3D9Ex)
					{
						OutputDebugString(L"[STS] Fatal : IDirect3DSwapChain->GetBackBuffer() not implemented !");
						exit(99);
					}
					else
					{
						// ensure left drawing side here
						SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
					}
					return nullptr;
				case METHOD_IDIRECT3DSWAPCHAIN9_GETFRONTBUFFERDATA:
					if (m_bUseD3D9Ex)
					{
						OutputDebugString(L"[STS] IDirect3DSwapChain->GetFrontBufferData() not implemented !");
					}
					else
					{
						// ensure left drawing side here
						SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
					}
					return nullptr;
			}
			return nullptr;
#pragma endregion
#pragma region IDirect3DStateBlock9
		case INTERFACE_IDIRECT3DSTATEBLOCK9:
			switch (eD3DMethod)
			{
				case METHOD_IDIRECT3DSTATEBLOCK9_APPLY:
					Apply();
					return nullptr;
			}
#pragma endregion
#pragma region D3DX9
		case INTERFACE_D3DX9:
			switch (eD3DMethod)
			{
				case MT_D3DX9::D3D9_D3DXLoadSurfaceFromSurface:
					SHOW_CALL(L"INTERFACE_D3DX9::D3D9_D3DXLoadSurfaceFromSurface");
					if (!m_ppcDestSurface) return nullptr;
					if (!m_ppsDestPalette) return nullptr;
					if (!m_ppsDestRect) return nullptr;
					if (!m_ppcSrcSurface) return nullptr;
					if (!m_ppsSrcPalette) return nullptr;
					if (!m_ppsSrcRect) return nullptr;
					if (!m_punFilter) return nullptr;
					if (!m_punColorKey) return nullptr;
					{
						// use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
						if (m_bUseD3D9Ex)
						{
							IDirect3DSurface9* pSourceSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcSrcSurface))->GetActualLeft();
							IDirect3DSurface9* pSourceSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcSrcSurface))->GetActualRight();
							IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualLeft();
							IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualRight();
							nHr = D3DXLoadSurfaceFromSurface(pDestSurfaceLeft, *m_ppsDestPalette, *m_ppsDestRect, pSourceSurfaceLeft, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey);
							
							/*OutputDebugString(L"CAll-----");
							DEBUG_HEX(*m_ppcSrcSurface);
							DEBUG_HEX(*m_ppcDestSurface);
							DEBUG_HEX(*m_ppsDestPalette);
							DEBUG_HEX(*m_ppsDestRect);
							DEBUG_HEX(*m_ppsSrcPalette);
							DEBUG_HEX(*m_ppsSrcRect);
							DEBUG_HEX(*m_punFilter);
							DEBUG_HEX(*m_punColorKey);
							OutputDebugString(L"----");
							DEBUG_HEX(pDestSurfaceLeft);
							DEBUG_HEX(pSourceSurfaceLeft);*/


							if (SUCCEEDED(nHr))
							{
								if (!pSourceSurfaceRight && pDestSurfaceRight)
								{
									// Source is not stereo, destination is stereo. Copying source to both sides of destination.
									if (FAILED(D3DXLoadSurfaceFromSurface(pDestSurfaceLeft, *m_ppsDestPalette, *m_ppsDestRect, pSourceSurfaceLeft, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey)))
									{
										OutputDebugString(L"[STS] ERROR: D3DXLoadSurfaceFromSurface - Failed to copy source left to destination right.\n");
									}
								}
								else if (pSourceSurfaceRight && !pDestSurfaceRight)
								{
									// Source is stereo, destination is not stereo. Copied Left side only.
								}
								else if (pSourceSurfaceRight && pDestSurfaceRight)
								{
									if (FAILED(D3DXLoadSurfaceFromSurface(pDestSurfaceRight, *m_ppsDestPalette, *m_ppsDestRect, pSourceSurfaceRight, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey)))
									{
										OutputDebugString(L"[STS] ERROR: D3DXLoadSurfaceFromSurface - Failed to copy source right to destination right.\n");
									}
								}
							}

							// method replaced, immediately return
							nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
							return (void*)&nHr;
						}
					}
					return nullptr;
			}
			return nullptr;
#pragma endregion
	}
	return nullptr;
}

/**
* There's some windows event on our node.
***/
void StereoSplitter::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// multiply mouse coords by 4 due to Aquilinus workspace architecture
	Vireio_GUI_Event sEvent = m_pcVireioGUI->WindowsEvent(msg, wParam, lParam, 4);
	switch (sEvent.eType)
	{
		case NoEvent:
			break;
		case ChangedToNext:
		case ChangedToPrevious:
			// handle game settings page
			if (sEvent.dwIndexOfPage == m_aunPageIDs[GUI_Pages::GameSettingsPage])
			{
				if (sEvent.dwIndexOfControl == m_sPageGameSettings.unDuplicateRenderTargetID)
					m_sGameSettings.nDuplicateRenderTarget = sEvent.nNewValue;
				if (sEvent.dwIndexOfControl == m_sPageGameSettings.unDuplicateDepthStencilID)
					m_sGameSettings.nDuplicateDepthStencil = sEvent.nNewValue;
				if (sEvent.dwIndexOfControl == m_sPageGameSettings.unDuplicateTextureID)
					m_sGameSettings.nDuplicateTexture = sEvent.nNewValue;
				if (sEvent.dwIndexOfControl == m_sPageGameSettings.unDuplicateCubeTextureID)
					m_sGameSettings.nDuplicateCubeTexture = sEvent.nNewValue;
				if (sEvent.dwIndexOfControl == m_sPageGameSettings.unSaveRenderStatesID)
					m_sGameSettings.nSaveRenderStatesID = sEvent.nNewValue;
			}
			break;
	}
}

/**
* Incoming Present() call.
* Handle the check time counter.
* @param bInit : True if not called by Present() to init otherwise.
***/
void StereoSplitter::Present(IDirect3DDevice9* pcDevice, bool bInit)
{
	// ensure to be on left drawing side
	if (m_eCurrentRenderingSide == RenderPosition::Right)
		SetDrawingSide(pcDevice, RenderPosition::Left);

	// initialize ?
	if (!m_bPresent)
	{
		if (m_bUseD3D9Ex)
		{
			// get the first swapchain
			IDirect3DSwapChain9* pcSwapChain = nullptr;
			pcDevice->GetSwapChain(0, &pcSwapChain);

			if (pcSwapChain)
			{
				// ...and enumerate it internally
				EnumerateSwapchain(pcDevice, pcSwapChain, 0);
				pcSwapChain->Release();

				// set the first proxy render target internally
				m_apcActiveRenderTargets[0] = m_aapcActiveProxyBackBufferSurfaces[0][0];
				if (m_apcActiveRenderTargets[0]) m_apcActiveRenderTargets[0]->AddRef();
			}
			else
			{
				// actually we shouldnt come here...
				OutputDebugString(L"[STS] Critical Error : No D3D9 swapchain present !");
				exit(99);
			}

			// is there a depth stencil set from startup ?
			IDirect3DSurface9* pcDepthStencil = nullptr;
			pcDevice->GetDepthStencilSurface(&pcDepthStencil);

			if (pcDepthStencil)
			{
				// create stereo proxy back buffer
				D3DSURFACE_DESC sDesc = {};
				pcDepthStencil->GetDesc(&sDesc);
				IDirect3DSurface9* pcDepthStencilRight = nullptr;
				HRESULT hr = pcDevice->CreateDepthStencilSurface(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false, &pcDepthStencilRight, nullptr);
				if (FAILED(hr))
				{
					{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: pcDevice->CreateDepthStencilSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
					hr = pcDevice->CreateDepthStencilSurface(sDesc.Width, sDesc.Height, sDesc.Format, D3DMULTISAMPLE_NONE, 0, false, &pcDepthStencilRight, nullptr);
					if (FAILED(hr))
					{
						{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: pcDevice->CreateDepthStencilSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
						exit(99);
					}
				}
				IDirect3DStereoSurface9* pcBackBufferStereo = new IDirect3DStereoSurface9(pcDepthStencil, pcDepthStencilRight, pcDevice, nullptr, nullptr, nullptr);

				// ...and set active, do not release the actual buffer since the stereo buffer keeps a reference
				m_pcActiveDepthStencilSurface[0] = pcBackBufferStereo;
				if (m_pcActiveDepthStencilSurface[0]) m_pcActiveDepthStencilSurface[0]->AddRef();

#ifdef _DEBUGTHIS
				OutputDebugString(L"[STS] Default back buffer format :");
				DEBUG_UINT(sDesc.Format);
				DEBUG_HEX(sDesc.Format);
#endif
			}

#ifdef _DEBUGTHIS
			// check device formats debug option
			IDirect3D9* pD3D = nullptr;
			pcDevice->GetDirect3D(&pD3D);
			if (pD3D)
			{
				DWORD unUsage = D3DUSAGE_RENDERTARGET;

				wchar_t buf[128]; wsprintf(buf, L"[STS] Check device formats usage %u", unUsage); OutputDebugString(buf);

				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_UNKNOWN) == S_OK) OutputDebugString(L"D3DFMT_UNKNOWN");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R8G8B8) == S_OK) OutputDebugString(L"D3DFMT_R8G8B8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8R8G8B8) == S_OK) OutputDebugString(L"D3DFMT_A8R8G8B8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_X8R8G8B8) == S_OK) OutputDebugString(L"D3DFMT_X8R8G8B8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R5G6B5) == S_OK) OutputDebugString(L"D3DFMT_R5G6B5");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_X1R5G5B5) == S_OK) OutputDebugString(L"D3DFMT_X1R5G5B5");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A1R5G5B5) == S_OK) OutputDebugString(L"D3DFMT_A1R5G5B5");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A4R4G4B4) == S_OK) OutputDebugString(L"D3DFMT_A4R4G4B4");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R3G3B2) == S_OK) OutputDebugString(L"D3DFMT_R3G3B2");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8) == S_OK) OutputDebugString(L"D3DFMT_A8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8R3G3B2) == S_OK) OutputDebugString(L"D3DFMT_A8R3G3B2");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_X4R4G4B4) == S_OK) OutputDebugString(L"D3DFMT_X4R4G4B4");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A2B10G10R10) == S_OK) OutputDebugString(L"D3DFMT_A2B10G10R10");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8B8G8R8) == S_OK) OutputDebugString(L"D3DFMT_A8B8G8R8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_X8B8G8R8) == S_OK) OutputDebugString(L"D3DFMT_X8B8G8R8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_G16R16) == S_OK) OutputDebugString(L"D3DFMT_G16R16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A2R10G10B10) == S_OK) OutputDebugString(L"D3DFMT_A2R10G10B10");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A16B16G16R16) == S_OK) OutputDebugString(L"D3DFMT_A16B16G16R16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8P8) == S_OK) OutputDebugString(L"D3DFMT_A8P8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_P8) == S_OK) OutputDebugString(L"D3DFMT_P8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_L8) == S_OK) OutputDebugString(L"D3DFMT_L8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A8L8) == S_OK) OutputDebugString(L"D3DFMT_A8L8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A4L4) == S_OK) OutputDebugString(L"D3DFMT_A4L4");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_V8U8) == S_OK) OutputDebugString(L"D3DFMT_V8U8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_L6V5U5) == S_OK) OutputDebugString(L"D3DFMT_L6V5U5");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_X8L8V8U8) == S_OK) OutputDebugString(L"D3DFMT_X8L8V8U8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_Q8W8V8U8) == S_OK) OutputDebugString(L"D3DFMT_Q8W8V8U8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_V16U16) == S_OK) OutputDebugString(L"D3DFMT_V16U16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A2W10V10U10) == S_OK) OutputDebugString(L"D3DFMT_V16U16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_UYVY) == S_OK) OutputDebugString(L"D3DFMT_UYVY");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R8G8_B8G8) == S_OK) OutputDebugString(L"D3DFMT_R8G8_B8G8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_YUY2) == S_OK) OutputDebugString(L"D3DFMT_YUY2");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_G8R8_G8B8) == S_OK) OutputDebugString(L"D3DFMT_G8R8_G8B8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_DXT1) == S_OK) OutputDebugString(L"D3DFMT_DXT1");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_DXT2) == S_OK) OutputDebugString(L"D3DFMT_DXT2");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_DXT3) == S_OK) OutputDebugString(L"D3DFMT_DXT3");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_DXT4) == S_OK) OutputDebugString(L"D3DFMT_DXT4");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_DXT5) == S_OK) OutputDebugString(L"D3DFMT_DXT5");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D16_LOCKABLE) == S_OK) OutputDebugString(L"D3DFMT_D16_LOCKABLE");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D32) == S_OK) OutputDebugString(L"D3DFMT_D32");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D15S1) == S_OK) OutputDebugString(L"D3DFMT_D15S1");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D24S8) == S_OK) OutputDebugString(L"D3DFMT_D24S8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D24X8) == S_OK) OutputDebugString(L"D3DFMT_D24X8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D24X4S4) == S_OK) OutputDebugString(L"D3DFMT_D24X4S4");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D16) == S_OK) OutputDebugString(L"D3DFMT_D16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D32F_LOCKABLE) == S_OK) OutputDebugString(L"D3DFMT_D32F_LOCKABLE");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D24FS8) == S_OK) OutputDebugString(L"D3DFMT_D24FS8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_D32_LOCKABLE) == S_OK) OutputDebugString(L"D3DFMT_D32_LOCKABLE");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_S8_LOCKABLE) == S_OK) OutputDebugString(L"D3DFMT_S8_LOCKABLE");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_L16) == S_OK) OutputDebugString(L"D3DFMT_L16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_VERTEXDATA) == S_OK) OutputDebugString(L"D3DFMT_VERTEXDATA");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_INDEX16) == S_OK) OutputDebugString(L"D3DFMT_INDEX16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_INDEX32) == S_OK) OutputDebugString(L"D3DFMT_INDEX32");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_Q16W16V16U16) == S_OK) OutputDebugString(L"D3DFMT_Q16W16V16U16");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_MULTI2_ARGB8) == S_OK) OutputDebugString(L"D3DFMT_MULTI2_ARGB8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R16F) == S_OK) OutputDebugString(L"D3DFMT_R16F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_G16R16F) == S_OK) OutputDebugString(L"D3DFMT_G16R16F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A16B16G16R16F) == S_OK) OutputDebugString(L"D3DFMT_A16B16G16R16F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_R32F) == S_OK) OutputDebugString(L"D3DFMT_R32F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_G32R32F) == S_OK) OutputDebugString(L"D3DFMT_G32R32F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A32B32G32R32F) == S_OK) OutputDebugString(L"D3DFMT_A32B32G32R32F");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_CxV8U8) == S_OK) OutputDebugString(L"D3DFMT_CxV8U8");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A1) == S_OK) OutputDebugString(L"D3DFMT_A1");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_A2B10G10R10_XR_BIAS) == S_OK) OutputDebugString(L"D3DFMT_A2B10G10R10_XR_BIAS");
				if (pD3D->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFORMAT::D3DFMT_X8R8G8B8, unUsage, D3DRESOURCETYPE::D3DRTYPE_SURFACE, D3DFORMAT::D3DFMT_BINARYBUFFER) == S_OK) OutputDebugString(L"D3DFMT_BINARYBUFFER");

				pD3D->Release();
			}
			else
				OutputDebugString(L"[STS] Failed to get D3D Device");
#endif
		}
		else
		{
			// get active render targets
			for (DWORD unI = 0; unI < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; unI++)
			{
				// get render target index
				IDirect3DSurface9* pcTarget = nullptr;
				pcDevice->GetRenderTarget(unI, &pcTarget);

				// set active and release
				if (pcTarget)
				{
					m_apcActiveRenderTargets[unI] = pcTarget;
					m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT + unI] = VerifyPrivateDataInterfaces(pcDevice, pcTarget);
					pcTarget->Release();
				}
			}

			// set number of render targets
			m_unRenderTargetNumber = 0;
			for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			if (m_apcActiveRenderTargets[i] != NULL) m_unRenderTargetNumber = (DWORD)i + 1;

			// get depth stencil
			IDirect3DSurface9* pcDepthStencil = nullptr;
			pcDevice->GetDepthStencilSurface(&pcDepthStencil);

			// set active and release
			if (pcDepthStencil)
			{
				m_pcActiveDepthStencilSurface[0] = pcDepthStencil;
				m_pcActiveDepthStencilSurface[1] = VerifyPrivateDataInterfaces(pcDevice, pcDepthStencil);
				pcDepthStencil->Release();
			}
		}

		// get viewport
		pcDevice->GetViewport(&m_sLastViewportSet);

		// TODO !! INIT ACTIVE TEXTURES
	}

	// set present() bool to true
	m_bPresent = true;

	// return if only called for initialization
	if (bInit) return;

	// finally, provide pointers to the left and right render target - get back buffer... TODO !! HANDLE CHANGED
	if (!m_pcActiveBackBufferSurface[0])
		pcDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pcActiveBackBufferSurface[0]);

	if (m_pcActiveBackBufferSurface[0])
	{
		// set twin by verifying
		if (!m_pcActiveBackBufferSurface[1])
			m_pcActiveBackBufferSurface[1] = VerifyPrivateDataInterfaces(pcDevice, m_pcActiveBackBufferSurface[0]);

		if (m_pcActiveBackBufferSurface[1])
		{
			// create textures... (if not created)
			if ((!m_pcStereoBuffer[0]) || (!m_pcStereoBuffer[1]))
			{
				D3DSURFACE_DESC sDesc = D3DSURFACE_DESC();
				m_pcActiveBackBufferSurface[0]->GetDesc(&sDesc);

				// got the backbuffer ?
				if (sDesc.Width > 0)
				{
					// query for the IDirect3DDevice9Ex interface
					static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
					s_pcDirect3DDevice9Ex = NULL;
					if (SUCCEEDED(pcDevice->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))))
					{
						// create the textures with a shared handle
						HANDLE pHandleLeft = nullptr;
						if (!m_pcStereoBuffer[0])
							s_pcDirect3DDevice9Ex->CreateTexture(sDesc.Width, sDesc.Height, 1, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[0], &pHandleLeft);

						HANDLE pHandleRight = nullptr;
						if (!m_pcStereoBuffer[1])
							s_pcDirect3DDevice9Ex->CreateTexture(sDesc.Width, sDesc.Height, 1, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[1], &pHandleRight);

						// set shared handles as private interfaces
						if (m_pcStereoBuffer[0])
							m_pcStereoBuffer[0]->SetPrivateData(PDIID_Shared_Handle, (void*)&pHandleLeft, sizeof(HANDLE), NULL);
						if (m_pcStereoBuffer[1])
							m_pcStereoBuffer[1]->SetPrivateData(PDIID_Shared_Handle, (void*)&pHandleRight, sizeof(HANDLE), NULL);

						s_pcDirect3DDevice9Ex->Release();
					}
					else OutputDebugString(L"[STS] Failed to query IDirect3DDevice9Ex interface.");

					// should we create standard textures if failed ?
					if (false)
					{
						if (!m_pcStereoBuffer[0])
							pcDevice->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[0], NULL);
						if (!m_pcStereoBuffer[1])
							pcDevice->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[1], NULL);
					}

					if ((!m_pcStereoBuffer[0]) || (!m_pcStereoBuffer[1])) OutputDebugString(L"[STS] Failed to create texture default/dynamic.");
				}
			}

			// first stereo output test
			if ((m_pcStereoBuffer[0]) && (m_pcStereoBuffer[1]))
			{
				m_pcStereoBuffer[0]->GetSurfaceLevel(0, &m_pcStereoBufferSurface[0]);
				m_pcStereoBuffer[1]->GetSurfaceLevel(0, &m_pcStereoBufferSurface[1]);
				if (m_pcStereoBufferSurface[0])
				{
					pcDevice->StretchRect(m_pcActiveBackBufferSurface[0], NULL, m_pcStereoBufferSurface[0], NULL, D3DTEXF_NONE);
					m_pcStereoBufferSurface[0]->Release();
				}
				if (m_pcStereoBufferSurface[1])
				{
					pcDevice->StretchRect(m_pcActiveBackBufferSurface[1], NULL, m_pcStereoBufferSurface[1], NULL, D3DTEXF_NONE);
					m_pcStereoBufferSurface[1]->Release();
				}
			}
		}
	}
	else
		OutputDebugString(L"Vireio Stereo Splitter : No primary swap chain found.");
}

/**
* Incoming SetRenderTarget() call.
* Set render targets internally, handle render target monitoring.
***/
void StereoSplitter::SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD unRenderTargetIndex, IDirect3DSurface9* pcRenderTarget)
{
	// set the render target internally
	if (unRenderTargetIndex < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT)
	{
		// set NULL manually, otherwise just set the render target :
		if (!pcRenderTarget)
		{
			m_apcActiveRenderTargets[unRenderTargetIndex] = NULL;
			m_apcActiveRenderTargets[unRenderTargetIndex + D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT] = NULL;

			// set number of render targets
			m_unRenderTargetNumber = 0;
			for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			if (m_apcActiveRenderTargets[i] != NULL) m_unRenderTargetNumber = (DWORD)i + 1;

			// and return
			return;
		}
		else
		{
			m_apcActiveRenderTargets[unRenderTargetIndex] = pcRenderTarget;

			// set number of render targets
			if (unRenderTargetIndex >= m_unRenderTargetNumber) m_unRenderTargetNumber = unRenderTargetIndex + 1;
		}
	}
	else return;

	// set second render target
	m_apcActiveRenderTargets[unRenderTargetIndex + D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT] = VerifyPrivateDataInterfaces(pcDevice, pcRenderTarget);
	if ((unRenderTargetIndex == 0) && (m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT] == nullptr))
		m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT] = m_apcActiveRenderTargets[0];
}

/**
* Incoming SetDepthStencilSurface() call.
***/
void StereoSplitter::SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil)
{
	// set NULL manually, otherwise just set the render target :
	if (!pNewZStencil)
		m_pcActiveDepthStencilSurface[0] = NULL;
	else
		m_pcActiveDepthStencilSurface[0] = pNewZStencil;

	// set NULL twin if NULL
	if (!pNewZStencil)
		m_pcActiveDepthStencilSurface[1] = NULL;
	else
	{
		// get surface twin
		m_pcActiveDepthStencilSurface[1] = VerifyPrivateDataInterfaces(pcDevice, pNewZStencil);
	}

	m_bControlUpdate = true;
}

/**
* Incoming SetTexture() call.
***/
void StereoSplitter::SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage, IDirect3DBaseTexture9* pcTexture)
{
	// set the texture internally
	if (Stage < D3D9_SIMULTANEAOUS_TEXTURE_COUNT)
	{
		// set NULL manually, otherwise just set the render target :
		if (!pcTexture)
		{
			m_apcActiveTextures[Stage] = NULL;

			// set number of textures
			m_unTextureNumber = 0;
			for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < D3D9_SIMULTANEAOUS_TEXTURE_COUNT; i++)
			if (m_apcActiveTextures[i] != NULL) m_unTextureNumber = (DWORD)i + 1;
		}
		else
		{
			m_apcActiveTextures[Stage] = pcTexture;

			// set number of textures
			if (Stage >= m_unTextureNumber) m_unTextureNumber = Stage + 1;
		}
	}
	else return;

	// set NULL twin if NULL
	if (!pcTexture)
		m_apcActiveTextures[Stage + D3D9_SIMULTANEAOUS_TEXTURE_COUNT] = NULL;
	else
	{
		// get texture twin
		m_apcActiveTextures[Stage + D3D9_SIMULTANEAOUS_TEXTURE_COUNT] = VerifyPrivateDataInterfaces(pcDevice, pcTexture, false);
	}

	m_bControlUpdate = true;
}

/**
* Try and set, if success save viewport.
* If viewport width and height match primary render target size and zmin is 0 and zmax 1 set
* m_bActiveViewportIsDefault flag true.
* @see m_bActiveViewportIsDefault
***/
HRESULT StereoSplitter::SetViewport(IDirect3DDevice9* pcDevice, CONST D3DVIEWPORT9* psViewport)
{
	HRESULT nHR = pcDevice->SetViewport(psViewport);

	if (SUCCEEDED(nHR))
	{
		m_bActiveViewportIsDefault = IsViewportDefaultForMainRT(psViewport);
		m_sLastViewportSet = *psViewport;
	}

	return nHR;
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
* Verify private data interfaces for the surface.
***/
IDirect3DSurface9* StereoSplitter::VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pcSurface)
{
	if (!pcSurface) return nullptr;

	// does this render target view have a stereo twin view ?
	IDirect3DSurface9* pcSurfaceTwin = nullptr;
	DWORD unSize = sizeof(pcSurfaceTwin);
	pcSurface->GetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcSurfaceTwin, &unSize);

	if (pcSurfaceTwin)
	{
		// interface already verified ? return nullptr
		if (pcSurfaceTwin == NO_PRIVATE_INTERFACE)
			return nullptr;

		// no release here, DX9 does not increment the counter
		return pcSurfaceTwin;
	}
	else
	{
		// try to get the container
		IDirect3DBaseTexture9* pcTexture = nullptr;
		pcSurface->GetContainer(__uuidof(IDirect3DTexture9), (void**)&pcTexture);
		if (!pcTexture)
			pcSurface->GetContainer(__uuidof(IDirect3DCubeTexture9), (void**)&pcTexture);

		// is this a texture level surface ?
		if (pcTexture)
		{
			// create the stereo texture
			IDirect3DBaseTexture9* pcTextureTwin = nullptr;
			CreateStereoTexture(pcDevice, pcTexture, &pcTextureTwin);

			// no twin ?
			if (!pcTextureTwin)
				return nullptr;

			// try to get the twin after creating the stereo texture
			DWORD unSize = sizeof(pcSurfaceTwin);
			pcSurface->GetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcSurfaceTwin, &unSize);

			// interface already verified ? return nullptr
			if (pcSurfaceTwin == NO_PRIVATE_INTERFACE)
				return nullptr;

			// and return the twin
			if (pcSurfaceTwin) return pcSurfaceTwin; else return nullptr;
		}
		else
		{
			// get description
			D3DSURFACE_DESC sDesc;
			pcSurface->GetDesc(&sDesc);

			// should we duplicate ?
			if ((sDesc.Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			{
				// no duplicate ? set "verified" indicator
				if (!ShouldDuplicateDepthStencilSurface(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false /*TODO!!*/))
				{
					IUnknown* pcIndicator = NO_PRIVATE_INTERFACE;
					pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);
					return nullptr;
				}
			}
			else
			{
				// no duplicate ? set "verified" indicator
				if (!ShouldDuplicateRenderTarget(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false /*TODO!!*/, (pcSurface == m_apcActiveRenderTargets[0])))
				{
					IUnknown* pcIndicator = NO_PRIVATE_INTERFACE;
					pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);
					return nullptr;
				}
			}

			// get target pointers, both tex + surface
			LPDIRECT3DTEXTURE9 pcStereoTwinTexture = nullptr;
			LPDIRECT3DSURFACE9 pcStereoTwinSurface = nullptr;

			// depth stencil ??
			if ((sDesc.Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			{
				// discard must be false since we switch the stencil surfaces for every draw call
				if (FAILED(pcDevice->CreateDepthStencilSurface(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false, &pcStereoTwinSurface, NULL)))
				{
					OutputDebugString(L"[STS] Failed to create depth stencil surface.");
#ifdef _DEBUGTHIS
					wchar_t buf[32];
					wsprintf(buf, L"desc.Width %u", sDesc.Width); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.Height %u", sDesc.Height); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.Format %u", sDesc.Format); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.MultiSampleType %u", sDesc.MultiSampleType); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.MultiSampleQuality %u", sDesc.MultiSampleQuality); OutputDebugString(buf);
#endif
				}
			}
			else
			if (FAILED(pcDevice->CreateRenderTarget(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false, &pcStereoTwinSurface, NULL)))
			{
				if (FAILED(pcDevice->CreateRenderTarget(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, true, &pcStereoTwinSurface, NULL)))
				{
					OutputDebugString(L"[STS] Failed to create render target.");
#ifdef _DEBUGTHIS
					wchar_t buf[32];
					wsprintf(buf, L"sDesc.Width %u", sDesc.Width); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.Height %u", sDesc.Height); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.Format %u", sDesc.Format); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.MultiSampleType %u", sDesc.MultiSampleType); OutputDebugString(buf);
					wsprintf(buf, L"sDesc.MultiSampleQuality %u", sDesc.MultiSampleQuality); OutputDebugString(buf);
#endif
					pcStereoTwinSurface = nullptr;
				}
			}

			// update resource and set private data
			if (pcStereoTwinSurface)
			{
				// update the surface
				pcDevice->UpdateSurface(pcSurface, nullptr, pcStereoTwinSurface, nullptr);

				// set twin as private data
				pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)pcStereoTwinSurface, sizeof(IUnknown*), D3DSPD_IUNKNOWN);

				// and return the twin
				return pcStereoTwinSurface;
			}
			else OutputDebugString(L"[STS] No surface !!");
		}
	}

	return nullptr;
}

/**
* Verify private data interfaces for the surface.
***/
IDirect3DBaseTexture9* StereoSplitter::VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, bool bDuplicate)
{
	if (!pcTexture) return nullptr;

	// get private data texture twin
	IDirect3DBaseTexture9* pcTextureTwin = nullptr;
	DWORD unSize = sizeof(pcTextureTwin);
	pcTexture->GetPrivateData(PDIID_IDirect3DBaseTexture9_Stereo_Twin, (void*)&pcTextureTwin, &unSize);

	// has a twin already ?
	if (pcTextureTwin)
	{
		// interface already verified ? return nullptr
		if (pcTextureTwin == NO_PRIVATE_INTERFACE)
			return nullptr;

		// no release here, DX9 does not increment the counter
		return pcTextureTwin;
	}
	else
	{
		if (bDuplicate)
		{
			// create the twin and return
			CreateStereoTexture(pcDevice, pcTexture, &pcTextureTwin);
			if (pcTextureTwin) return pcTextureTwin; else return nullptr;
		}
		else
		{
			// duplicate if depth stencil or render target
			if (pcTexture->GetType() == D3DRESOURCETYPE::D3DRTYPE_TEXTURE)
			{
				// get surface level 0
				IDirect3DSurface9* pcSurface = nullptr;
				((IDirect3DTexture9*)pcTexture)->GetSurfaceLevel(0, &pcSurface);
				if (pcSurface)
				{
					// get description
					D3DSURFACE_DESC sDesc = {};
					pcSurface->GetDesc(&sDesc);
					pcSurface->Release();

					// is depth buffer or render target ?
					if ((sDesc.Usage & D3DUSAGE_DEPTHSTENCIL) || (sDesc.Usage & D3DUSAGE_RENDERTARGET))
					{
						// create the twin and return
						CreateStereoTexture(pcDevice, pcTexture, &pcTextureTwin);
						if (pcTextureTwin) return pcTextureTwin; else return nullptr;
					}
				}
			}

		}
	}

	return nullptr;
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

	// TODO !! IMPLEMENT !!
	if (m_bUseD3D9Ex) return true;

	// should never try and render for the right eye if there is no render target for the main render targets right side
	if ((m_apcActiveRenderTargets[0] == m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT]) && (eSide == RenderPosition::Left))
	{
		return false;
	}

	// Everything hasn't changed yet but we set this first so we don't accidentally use the member instead of the local and break
	// things, as I have already managed twice.
	SetDrawingSideField(eSide);

	// state block was applied ?
	if (m_bApply)
	{
		// verify the render targets
		for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			// get the render target for this index
			LPDIRECT3DSURFACE9 pcRenderTarget = nullptr;
			pcDevice->GetRenderTarget((DWORD)i, &pcRenderTarget);

			// is this render target stored ?
			if (pcRenderTarget != m_apcActiveRenderTargets[i])
			{
				if (pcRenderTarget)
					SetRenderTarget(pcDevice, (DWORD)i, pcRenderTarget);
				else
					SetRenderTarget(pcDevice, (DWORD)i, nullptr);
			}

			// ..and release
			if (pcRenderTarget) pcRenderTarget->Release();
		}

		// TODO !! TEXTURES

		// state block is handled
		m_bApply = false;
	}

	// switch render targets to new eSide
	bool bRenderTargetChanged = false;
	HRESULT nHr = D3D_OK;
	for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_unRenderTargetNumber; i++)
	{
		if (eSide == RenderPosition::Left)
		{
			nHr = pcDevice->SetRenderTarget((DWORD)i, m_apcActiveRenderTargets[i]);
		}
		else
		{
			nHr = pcDevice->SetRenderTarget((DWORD)i, m_apcActiveRenderTargets[i + D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT]);
		}

		if (nHr)
		{
			OutputDebugString(L"[STS] Error trying to set one of the Render Targets while switching between active eyes for drawing.\n");
		}
		else
		{
			bRenderTargetChanged = true;
		}
	}

	// if a non-fullsurface viewport is active and a rendertarget changed we need to reapply the viewport
	if (bRenderTargetChanged && !m_bActiveViewportIsDefault)
	{
		pcDevice->SetViewport(&m_sLastViewportSet);
	}

	//#define DRAW_INDICATORS
#ifdef DRAW_INDICATORS
	if (eSide == RenderPosition::Left)
	{
		RECT rc;
		rc.top = 10;
		rc.bottom = 60;
		rc.left = 10;
		rc.right = 60;
		pcDevice->ColorFill(m_apcActiveRenderTargets[0], &rc, D3DCOLOR_ARGB(255, 100, 10, 200));
	}
	else
	{
		RECT rc;
		rc.top = 400;
		rc.bottom = 450;
		rc.left = 10;
		rc.right = 60;
		pcDevice->ColorFill(m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT], &rc, D3DCOLOR_ARGB(255, 100, 200, 10));
	}
#endif

	// switch depth stencil to new side
	if (m_pcActiveDepthStencilSurface[0] != NULL)
	{
		if (eSide == RenderPosition::Left)
			nHr = pcDevice->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[0]);
		else
		{
			nHr = pcDevice->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[1]);
		}
	}

	// switch textures to new side
	for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < m_unTextureNumber; i++)
	{
		if ((eSide == RenderPosition::Left) || (!(m_apcActiveTextures[i + D3D9_SIMULTANEAOUS_TEXTURE_COUNT])))
		{
			nHr = pcDevice->SetTexture((DWORD)i, m_apcActiveTextures[i]);
		}
		else
			nHr = pcDevice->SetTexture((DWORD)i, m_apcActiveTextures[i + D3D9_SIMULTANEAOUS_TEXTURE_COUNT]);

		if (nHr)
			OutputDebugString(L"[STS] Error trying to set one of the textures while switching between active eyes for drawing.\n");
	}

	// set shader constants to new side, first vertex shader
	if (m_ppasVSConstantRuleIndices)
	{
		if (*m_ppasVSConstantRuleIndices)
		{
			std::vector<Vireio_Constant_Rule_Index_DX9>* pasIndices = *m_ppasVSConstantRuleIndices;
			if (eSide == RenderPosition::Left)
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasIndices->size(); nI++)
					pcDevice->SetVertexShaderConstantF((*pasIndices)[nI].m_dwConstantRuleRegister, (*pasIndices)[nI].m_afConstantDataLeft, (*pasIndices)[nI].m_dwConstantRuleRegisterCount);
			}
			else
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasIndices->size(); nI++)
					pcDevice->SetVertexShaderConstantF((*pasIndices)[nI].m_dwConstantRuleRegister, (*pasIndices)[nI].m_afConstantDataRight, (*pasIndices)[nI].m_dwConstantRuleRegisterCount);
			}
		}
	}
	// now pixel shader
	if (m_ppasPSConstantRuleIndices)
	{
		if (*m_ppasPSConstantRuleIndices)
		{
			std::vector<Vireio_Constant_Rule_Index_DX9>* pasIndices = *m_ppasPSConstantRuleIndices;
			if (eSide == RenderPosition::Left)
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasIndices->size(); nI++)
					pcDevice->SetPixelShaderConstantF((*pasIndices)[nI].m_dwConstantRuleRegister, (*pasIndices)[nI].m_afConstantDataLeft, (*pasIndices)[nI].m_dwConstantRuleRegisterCount);
			}
			else
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasIndices->size(); nI++)
					pcDevice->SetPixelShaderConstantF((*pasIndices)[nI].m_dwConstantRuleRegister, (*pasIndices)[nI].m_afConstantDataRight, (*pasIndices)[nI].m_dwConstantRuleRegisterCount);
			}
		}
	}

	return true;
}

/**
* Creates a texture by surface description and returns both texture and surface level zero.
***/
void StereoSplitter::CreateStereoTexture(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, IDirect3DBaseTexture9** ppcStereoTwinTexture)
{
	// get the resource type
	D3DRESOURCETYPE eType = pcTexture->GetType();
	switch (eType)
	{
		case D3DRTYPE_TEXTURE:
		{
								 // get first level
								 IDirect3DSurface9* pcSurface = nullptr;
								 ((IDirect3DTexture9*)pcTexture)->GetSurfaceLevel(0, &pcSurface);
								 if (pcSurface)
								 {
									 // get description
									 D3DSURFACE_DESC sDesc = {};
									 pcSurface->GetDesc(&sDesc);
									 pcSurface->Release();

									 // should we duplicate ?
									 if (!ShouldDuplicateTexture(sDesc.Width, sDesc.Height, pcTexture->GetLevelCount(), sDesc.Usage, sDesc.Format, sDesc.Pool))
									 {
										 // set no interface indicator
										 IUnknown* pcIndicator = NO_PRIVATE_INTERFACE;
										 pcTexture->SetPrivateData(PDIID_IDirect3DBaseTexture9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);

										 // loop throug all levels, set no interface indicator
										 for (DWORD unI = 0; unI < pcTexture->GetLevelCount(); unI++)
										 {
											 // get current level
											 pcSurface = nullptr;
											 ((IDirect3DTexture9*)pcTexture)->GetSurfaceLevel(unI, &pcSurface);
											 if (pcSurface)
											 {
												 // set no interface indicator
												 pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);
												 pcSurface->Release();
											 }

										 }

										 return;
									 }

									 // create the texture
									 if (FAILED(pcDevice->CreateTexture((UINT)sDesc.Width, (UINT)sDesc.Height, pcTexture->GetLevelCount(), sDesc.Usage, sDesc.Format, sDesc.Pool, (IDirect3DTexture9**)ppcStereoTwinTexture, NULL)))
									 {
										 OutputDebugString(L"[STS] Failed to create render target texture.");
#ifdef _DEBUGTHIS							
										 wchar_t buf[32];
										 wsprintf(buf, L"sDesc.Width %u", sDesc.Width); OutputDebugString(buf);
										 wsprintf(buf, L"sDesc.Height %u", sDesc.Height); OutputDebugString(buf);
										 wsprintf(buf, L"sDesc.Usage %u", sDesc.Usage); OutputDebugString(buf);
										 wsprintf(buf, L"sDesc.Format %u", sDesc.Format); OutputDebugString(buf);
#endif
										 *ppcStereoTwinTexture = nullptr;
									 }
									 else
									 {
										 // update the texture
										 pcDevice->UpdateTexture(pcTexture, *ppcStereoTwinTexture);

										 // set twin as private data interface
										 IDirect3DTexture9* pcTextureTwin = (IDirect3DTexture9*)*ppcStereoTwinTexture;
										 pcTexture->SetPrivateData(PDIID_IDirect3DBaseTexture9_Stereo_Twin, (void*)pcTextureTwin, sizeof(IUnknown*), D3DSPD_IUNKNOWN);

										 // loop throug all levels, set stereo twin
										 for (DWORD unI = 0; unI < pcTexture->GetLevelCount(); unI++)
										 {
											 // get current level
											 pcSurface = nullptr;
											 ((IDirect3DTexture9*)pcTexture)->GetSurfaceLevel(unI, &pcSurface);
											 if (pcSurface)
											 {
												 // get level from twin
												 IDirect3DSurface9* pcSurfaceTwin = nullptr;
												 pcTextureTwin->GetSurfaceLevel(unI, &pcSurfaceTwin);
												 if (pcSurfaceTwin)
												 {
													 // set as private interface
													 pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)pcSurfaceTwin, sizeof(IUnknown*), D3DSPD_IUNKNOWN);
													 pcSurfaceTwin->Release();
												 }
												 pcSurface->Release();
											 }

										 }
									 }
								 }
		}
			break;
		case D3DRTYPE_VOLUMETEXTURE:
			// TODO !! VOLUME TEXTURE !!
			OutputDebugString(L"VOLUMETEXTURE... TODO !!");
			break;
		case D3DRTYPE_CUBETEXTURE:
		{
									 // get first level
									 IDirect3DSurface9* pcSurface = nullptr;
									 ((IDirect3DCubeTexture9*)pcTexture)->GetCubeMapSurface(D3DCUBEMAP_FACES::D3DCUBEMAP_FACE_POSITIVE_X, 0, &pcSurface);
									 if (pcSurface)
									 {
										 // get description
										 D3DSURFACE_DESC sDesc = {};
										 pcSurface->GetDesc(&sDesc);
										 pcSurface->Release();

										 // should we duplicate ?
										 if (!ShouldDuplicateCubeTexture(sDesc.Width, pcTexture->GetLevelCount(), sDesc.Usage, sDesc.Format, sDesc.Pool))
										 {
											 // set no interface indicator
											 IUnknown* pcIndicator = NO_PRIVATE_INTERFACE;
											 pcTexture->SetPrivateData(PDIID_IDirect3DBaseTexture9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);

											 // loop throug all levels, set no interface indicator
											 for (DWORD unI = 0; unI < pcTexture->GetLevelCount(); unI++)
											 {
												 // loop throug all facetypes
												 for (UINT unFaceType = 0; unFaceType < 6; unFaceType++)
												 {
													 // get current level
													 pcSurface = nullptr;
													 ((IDirect3DCubeTexture9*)pcTexture)->GetCubeMapSurface((D3DCUBEMAP_FACES)unFaceType, unI, &pcSurface);
													 if (pcSurface)
													 {
														 // set no interface indicator
														 pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)&pcIndicator, sizeof(IUnknown*), 0);
														 pcSurface->Release();
													 }
												 }
											 }

											 return;
										 }

										 // create the texture
										 if (FAILED(pcDevice->CreateCubeTexture((UINT)sDesc.Width, pcTexture->GetLevelCount(), sDesc.Usage, sDesc.Format, sDesc.Pool, (IDirect3DCubeTexture9**)ppcStereoTwinTexture, NULL)))
										 {
											 OutputDebugString(L"[STS] Failed to create render target texture.");
#ifdef _DEBUGTHIS							
											 wchar_t buf[32];
											 wsprintf(buf, L"sDesc.Width %u", sDesc.Width); OutputDebugString(buf);
											 wsprintf(buf, L"sDesc.Height %u", sDesc.Height); OutputDebugString(buf);
											 wsprintf(buf, L"sDesc.Usage %u", sDesc.Usage); OutputDebugString(buf);
											 wsprintf(buf, L"sDesc.Format %u", sDesc.Format); OutputDebugString(buf);
#endif
											 *ppcStereoTwinTexture = nullptr;
										 }
										 else
										 {
											 // update the texture
											 pcDevice->UpdateTexture(pcTexture, *ppcStereoTwinTexture);

											 // set twin as private data interface
											 IDirect3DCubeTexture9* pcTextureTwin = (IDirect3DCubeTexture9*)*ppcStereoTwinTexture;
											 pcTexture->SetPrivateData(PDIID_IDirect3DBaseTexture9_Stereo_Twin, (void*)pcTextureTwin, sizeof(IUnknown*), D3DSPD_IUNKNOWN);

											 // loop throug all levels, set stereo twin
											 for (DWORD unI = 0; unI < pcTexture->GetLevelCount(); unI++)
											 {
												 // loop throug all facetypes
												 for (UINT unFaceType = 0; unFaceType < 6; unFaceType++)
												 {
													 // get current level
													 pcSurface = nullptr;
													 ((IDirect3DCubeTexture9*)pcTexture)->GetCubeMapSurface((D3DCUBEMAP_FACES)unFaceType, unI, &pcSurface);
													 if (pcSurface)
													 {
														 // get level from twin
														 IDirect3DSurface9* pcSurfaceTwin = nullptr;
														 pcTextureTwin->GetCubeMapSurface((D3DCUBEMAP_FACES)unFaceType, unI, &pcSurfaceTwin);
														 if (pcSurfaceTwin)
														 {
															 // set as private interface
															 pcSurface->SetPrivateData(PDIID_IDirect3DSurface9_Stereo_Twin, (void*)pcSurfaceTwin, sizeof(IUnknown*), D3DSPD_IUNKNOWN);
															 pcSurfaceTwin->Release();
														 }
														 pcSurface->Release();
													 }
												 }
											 }
										 }
									 }
		}
		default:
			break;
	}
}

/**
* True if the by parameters described render target is to be duplicated for the handled game.
* @see StereoSplitter::VerifyPrivateDataInterfaces()
***/
bool StereoSplitter::ShouldDuplicateRenderTarget(UINT unWidth, UINT unHeight, D3DFORMAT Format, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bLockable, bool bIsSwapChainBackBuffer)
{
	// switch by defined behaviour
	switch (m_sGameSettings.nDuplicateRenderTarget)
	{
		case DetermineDuplicateRToDS::DT_FALSE:
			return false;
		case DetermineDuplicateRToDS::DT_TRUE:
			return true;
		case DetermineDuplicateRToDS::DT_WIDTH_UNEQUAL_HEIGHT:
			if (bIsSwapChainBackBuffer)
			{
				return true;
			}
			return unWidth != unHeight;
	}

	return true;
}

/**
* True if the by parameters described depth stencil surface is to be duplicated for the handled game.
* @see StereoSplitter::VerifyPrivateDataInterfaces()
***/
bool StereoSplitter::ShouldDuplicateDepthStencilSurface(UINT unWidth, UINT unHeight, D3DFORMAT eFormat, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bDiscard)
{
	// switch by defined behaviour
	switch (m_sGameSettings.nDuplicateDepthStencil)
	{
		case DetermineDuplicateRToDS::DT_FALSE:
			return false;
		case DetermineDuplicateRToDS::DT_TRUE:
			return true;
		case DetermineDuplicateRToDS::DT_WIDTH_UNEQUAL_HEIGHT:
			return unWidth != unHeight;
	}

	return true;
}

/**
* True if the by parameters described texture is to be duplicated for the handled game.
* @see StereoSplitter::VerifyPrivateDataInterfaces()
***/
bool StereoSplitter::ShouldDuplicateTexture(UINT unWidth, UINT unHeight, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool)
{
	// switch by defined behaviour
	switch (m_sGameSettings.nDuplicateTexture)
	{
		case DetermineDuplicateTexture::DT_FALSE:
			return false;
		case DetermineDuplicateTexture::DT_TRUE:
			return true;
		case DetermineDuplicateTexture::DT_IF_RENDER_TARGET:
			if ((unUsage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
				return true;
			return IS_RENDER_TARGET(unUsage);
		case DetermineDuplicateTexture::DT_IF_RENDER_TARGET_AND_WIDTH_UNEQUAL_HEIGHT:
			if ((unUsage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
				return true;
			return IS_RENDER_TARGET(unUsage) && (unWidth != unHeight);
	}

	// default - won't ever get here
	if ((unUsage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
		return true;

	return IS_RENDER_TARGET(unUsage);
}

/**
* True if the by parameters described cube texture is to be duplicated for the handled game.
* Currently, returns true if cube texture is a render target.
* @see D3DProxyDevice::CreateCubeTexture()
***/
bool StereoSplitter::ShouldDuplicateCubeTexture(UINT unEdgeLength, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool)
{
	// switch by defined behaviour
	switch (m_sGameSettings.nDuplicateCubeTexture)
	{
		case DetermineDuplicateCubeTexture::DT_FALSE:
			return false;
		case DetermineDuplicateCubeTexture::DT_TRUE:
			return true;
		case DetermineDuplicateCubeTexture::DT_IF_RENDER_TARGET:
			return IS_RENDER_TARGET(unUsage);
	}

	return IS_RENDER_TARGET(unUsage);
}

/**
* Comparison made against active primary render target.
***/
bool StereoSplitter::IsViewportDefaultForMainRT(CONST D3DVIEWPORT9* psViewport)
{
	if (!m_apcActiveRenderTargets[0]) return false;

	// get description from first active render target
	D3DSURFACE_DESC sRTDesc;
	m_apcActiveRenderTargets[0]->GetDesc(&sRTDesc);

	return  ((psViewport->Height == sRTDesc.Height) && (psViewport->Width == sRTDesc.Width) &&
		(psViewport->MinZ <= SMALL_FLOAT) && (psViewport->MaxZ >= SLIGHTLY_LESS_THAN_ONE));
}

/**
* Creates the Graphical User Interface for this node.
***/
void StereoSplitter::CreateGUI()
{
	SIZE sSizeOfThis;
	sSizeOfThis.cx = GUI_WIDTH; sSizeOfThis.cy = GUI_HEIGHT;
	m_pcVireioGUI = new Vireio_GUI(sSizeOfThis, L"Candara", TRUE, GUI_CONTROL_FONTSIZE, RGB(120, 125, 90), RGB(250, 250, 200));

	// first, add all pages
	for (int i = 0; i < (int)GUI_Pages::NumberOfPages; i++)
	{
		UINT dwPage = m_pcVireioGUI->AddPage();
		m_aunPageIDs.push_back(dwPage);
	}

	// control structure
	Vireio_Control sControl;

#pragma region Static Text
	static std::vector<std::wstring> aszGameSettingsStrings;
	aszGameSettingsStrings.push_back(L"Duplicate Render Target"); aszGameSettingsStrings.push_back(std::wstring()); aszGameSettingsStrings.push_back(std::wstring());
	aszGameSettingsStrings.push_back(L"Duplicate Depth Stencil"); aszGameSettingsStrings.push_back(std::wstring()); aszGameSettingsStrings.push_back(std::wstring());
	aszGameSettingsStrings.push_back(L"Duplicate Texture"); aszGameSettingsStrings.push_back(std::wstring()); aszGameSettingsStrings.push_back(std::wstring());
	aszGameSettingsStrings.push_back(L"Duplicate Cube Texture"); aszGameSettingsStrings.push_back(std::wstring()); aszGameSettingsStrings.push_back(std::wstring());
	aszGameSettingsStrings.push_back(L"Save Render States"); aszGameSettingsStrings.push_back(std::wstring()); aszGameSettingsStrings.push_back(std::wstring());

	static std::vector<std::wstring> aszDetermineRenderTargetStrings;
	aszDetermineRenderTargetStrings.push_back(L"DT_FALSE");
	aszDetermineRenderTargetStrings.push_back(L"DT_TRUE");
	aszDetermineRenderTargetStrings.push_back(L"DT_WIDTH_UNEQUAL_HEIGHT");

	static std::vector<std::wstring> aszDetermineDepthStencilStrings;
	aszDetermineDepthStencilStrings.push_back(L"DT_FALSE");
	aszDetermineDepthStencilStrings.push_back(L"DT_TRUE");
	aszDetermineDepthStencilStrings.push_back(L"DT_WIDTH_UNEQUAL_HEIGHT");

	static std::vector<std::wstring> aszDetermineTextureStrings;
	aszDetermineTextureStrings.push_back(L"DT_FALSE");
	aszDetermineTextureStrings.push_back(L"DT_TRUE");
	aszDetermineTextureStrings.push_back(L"DT_IF_RENDER_TARGET");
	aszDetermineTextureStrings.push_back(L"DT_IF_RENDER_TARGET_AND_WIDTH_UNEQUAL_HEIGHT");

	static std::vector<std::wstring> aszDetermineCubeTextureStrings;
	aszDetermineCubeTextureStrings.push_back(L"DT_FALSE");
	aszDetermineCubeTextureStrings.push_back(L"DT_TRUE");
	aszDetermineCubeTextureStrings.push_back(L"DT_IF_RENDER_TARGET");

	static std::vector<std::wstring> aszDetermineSaveRenderStates;
	aszDetermineSaveRenderStates.push_back(L"DT_STATE_BLOCK");
	aszDetermineSaveRenderStates.push_back(L"DT_SELECTED_STATES_MANUALLY");
	aszDetermineSaveRenderStates.push_back(L"DT_ALL_STATES_MANUALLY");
	aszDetermineSaveRenderStates.push_back(L"DT_DO_NOT_SAVE_AND_RESTORE");
#pragma endregion

#pragma region Main Page
	ZeroMemory(&sControl, sizeof(Vireio_Control));
	static std::vector<std::wstring> sEntriesCommanders;
	static std::vector<std::wstring> sEntriesDecommanders;

	// create the decommanders list
	sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
	sControl.m_sPosition.x = GUI_CONTROL_FONTBORDER;
	sControl.m_sPosition.y = 0;
	sControl.m_sSize.cx = GUI_WIDTH - GUI_CONTROL_BORDER;
	sControl.m_sSize.cy = NUMBER_OF_DECOMMANDERS * 64;
	sControl.m_sStaticListBox.m_bSelectable = false;
	sControl.m_sStaticListBox.m_paszEntries = &sEntriesDecommanders;
	UINT dwDecommandersList = m_pcVireioGUI->AddControl(m_aunPageIDs[GUI_Pages::MainPage], sControl);

	// create the commanders list out of the decommanders list
	sControl.m_sPosition.y += NUMBER_OF_DECOMMANDERS * 64;
	sControl.m_sPosition.x = GUI_WIDTH >> 2;
	sControl.m_sSize.cy = NUMBER_OF_COMMANDERS * 64;
	sControl.m_sStaticListBox.m_paszEntries = &sEntriesCommanders;
	UINT dwCommandersList = m_pcVireioGUI->AddControl(m_aunPageIDs[GUI_Pages::MainPage], sControl);

	// and add all entries
	for (int i = 0; i < NUMBER_OF_DECOMMANDERS; i++)
		m_pcVireioGUI->AddEntry(dwDecommandersList, this->GetDecommanderName(i));
	for (int i = 0; i < NUMBER_OF_COMMANDERS; i++)
		m_pcVireioGUI->AddEntry(dwCommandersList, this->GetCommanderName(i));
#pragma endregion

#pragma region Game setting page
	m_sPageGameSettings.unTextlist = CreateStaticListControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszGameSettingsStrings, DUPLICATE_RENDERTARGET_POS_X, DUPLICATE_RENDERTARGET_POS_Y - GUI_CONTROL_FONTSIZE, GUI_CONTROL_SPINSIZE);

	m_sPageGameSettings.unDuplicateRenderTargetID = CreateSpinControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszDetermineRenderTargetStrings, (UINT)m_sGameSettings.nDuplicateRenderTarget, DUPLICATE_RENDERTARGET_POS_X, DUPLICATE_RENDERTARGET_POS_Y, GUI_CONTROL_SPINSIZE);
	m_sPageGameSettings.unDuplicateDepthStencilID = CreateSpinControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszDetermineDepthStencilStrings, (UINT)m_sGameSettings.nDuplicateDepthStencil, DUPLICATE_DEPTHSTENCIL_POS_X, DUPLICATE_DEPTHSTENCIL_POS_Y, GUI_CONTROL_SPINSIZE);
	m_sPageGameSettings.unDuplicateTextureID = CreateSpinControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszDetermineTextureStrings, (UINT)m_sGameSettings.nDuplicateTexture, DUPLICATE_TEXTURE_POS_X, DUPLICATE_TEXTURE_POS_Y, GUI_CONTROL_SPINSIZE);
	m_sPageGameSettings.unDuplicateCubeTextureID = CreateSpinControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszDetermineCubeTextureStrings, (UINT)m_sGameSettings.nDuplicateCubeTexture, DUPLICATE_CUBETEXTURE_POS_X, DUPLICATE_CUBETEXTURE_POS_Y, GUI_CONTROL_SPINSIZE);
	m_sPageGameSettings.unSaveRenderStatesID = CreateSpinControl(m_pcVireioGUI, m_aunPageIDs[GUI_Pages::GameSettingsPage], &aszDetermineSaveRenderStates, (UINT)m_sGameSettings.nSaveRenderStatesID, SAVE_RENDERSTATES_POS_X, SAVE_RENDERSTATES_POS_Y, GUI_CONTROL_SPINSIZE);
#pragma endregion

#pragma region Description Page
	ZeroMemory(&sControl, sizeof(Vireio_Control));
	static std::vector<std::wstring> sEntriesDescription;

	// create the description list
	sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
	sControl.m_sPosition.x = GUI_CONTROL_FONTBORDER;
	sControl.m_sPosition.y = 0;
	sControl.m_sSize.cx = GUI_WIDTH - GUI_CONTROL_BORDER;
	sControl.m_sSize.cy = (NUMBER_OF_DECOMMANDERS + NUMBER_OF_COMMANDERS) * 64;
	sControl.m_sStaticListBox.m_bSelectable = false;
	sControl.m_sStaticListBox.m_paszEntries = &sEntriesDescription;
	UINT dwDescriptionList = m_pcVireioGUI->AddControl(m_aunPageIDs[GUI_Pages::DescriptionPage], sControl);

	// and add all entries
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetRenderTarget()  target index");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetRenderTarget() render target");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetDepthStencilSurface() surface");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetTexture() sampler index");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetTexture() texture pointer");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateSurface() source surface");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateSurface() source rectangle");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateSurface() destination surface");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateSurface() destination point");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateTexture() source texture");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->UpdateTexture() destination texture");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->ColorFill() surface pointer");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->ColorFill() destination rectangle");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->ColorFill() destination color");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->StretchRect() source surface");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->StretchRect() source rectangle");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->StretchRect() destination surface");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->StretchRect() destination rectangle");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->StretchRect() filter");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Pointer to the extern drawing side bool.");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"The constant rule indices for the vshader.");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"The constant rule indices for the pshader.");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetRenderState() State");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetRenderState() Value");
	m_pcVireioGUI->AddEntry(dwDescriptionList, L"Device9::->SetViewport() Viewport");
#pragma endregion
}

/**
* Returns actual textures from wrapped (=proxy) texture.
* @param pWrappedTexture [in] Wrapped texture.
* @param ppActualLeftTexture [in, out] Will be set to the actual textures from pWrappedTexture. Left should never be NULL.
* @param ppActualRightTexture [in, out] Will be set to the actual textures from pWrappedTexture. Right maybe NULL if texture isn't stereo.
***/
void StereoSplitter::UnWrapProxyTexture(IDirect3DBaseTexture9* pWrappedTexture, IDirect3DBaseTexture9** ppActualLeftTexture, IDirect3DBaseTexture9** ppActualRightTexture)
{
	if (!pWrappedTexture)
		assert(false);

	D3DRESOURCETYPE type = pWrappedTexture->GetType();

	*ppActualLeftTexture = NULL;
	*ppActualRightTexture = NULL;

	switch (type)
	{
		case D3DRTYPE_TEXTURE:
		{
								 IDirect3DStereoTexture9* pDerivedTexture = static_cast<IDirect3DStereoTexture9*>((IDirect3DTexture9*)pWrappedTexture);
								 *ppActualLeftTexture = pDerivedTexture->GetActualLeft();
								 *ppActualRightTexture = pDerivedTexture->GetActualRight();

								 break;
		}
		case D3DRTYPE_VOLUMETEXTURE:
		{
									   IDirect3DStereoVolumeTexture9* pDerivedTexture = static_cast<IDirect3DStereoVolumeTexture9*>((IDirect3DVolumeTexture9*)pWrappedTexture);
									   *ppActualLeftTexture = pDerivedTexture->GetActual();
									   break;
		}
		case D3DRTYPE_CUBETEXTURE:
		{
									 IDirect3DStereoCubeTexture9* pDerivedTexture = static_cast<IDirect3DStereoCubeTexture9*>((IDirect3DCubeTexture9*)pWrappedTexture);
									 *ppActualLeftTexture = pDerivedTexture->GetActualLeft();
									 *ppActualRightTexture = pDerivedTexture->GetActualRight();
									 break;
		}

		default:
			OutputDebugString(L"[STS] Unhandled texture type in SetTexture\n");
			break;
	}

	if ((*ppActualLeftTexture) == NULL)
	{
		OutputDebugString(L"[STS] No left texture? Unpossible!\n");
		assert(false);
	}
}

/**
* Enumerates a swapchain to the internal swapchain array.
**/
void StereoSplitter::EnumerateSwapchain(IDirect3DDevice9* pcDevice, IDirect3DSwapChain9* pcSwapChain, UINT unIndex)
{
	// get number of backbuffers in that swapchain
	D3DPRESENT_PARAMETERS sParams = {};
	pcSwapChain->GetPresentParameters(&sParams);

	// create back buffer vector
	static std::vector<IDirect3DStereoSurface9*> s_apcProxyBackBuffers = std::vector<IDirect3DStereoSurface9*>();

	// loop through backbuffers, add them
	for (UINT unJ = 0; unJ < sParams.BackBufferCount; unJ++)
	{
		// get back buffer for the specified index
		IDirect3DSurface9* pcBackBuffer = nullptr;
		pcSwapChain->GetBackBuffer(unJ, D3DBACKBUFFER_TYPE::D3DBACKBUFFER_TYPE_MONO, &pcBackBuffer);

		if (pcBackBuffer)
		{
			// create stereo proxy back buffer
			D3DSURFACE_DESC sDesc = {};
			pcBackBuffer->GetDesc(&sDesc);
			static IDirect3DSurface9* s_pcBackBufferRight = nullptr;
			s_pcBackBufferRight = nullptr;
			HRESULT hr = pcDevice->CreateRenderTarget(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false, &s_pcBackBufferRight, nullptr);
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: pWrappedOwningDevice->CreateRenderTarget hr = 0x%0.8x", hr); OutputDebugString(buf); }
				HRESULT hr = pcDevice->CreateRenderTarget(sDesc.Width, sDesc.Height, sDesc.Format, D3DMULTISAMPLE_NONE, 0, false, &s_pcBackBufferRight, nullptr);
				if (FAILED(hr))
				{
					{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: pWrappedOwningDevice->CreateRenderTarget hr = 0x%0.8x", hr); OutputDebugString(buf); }
					exit(99);
				}
			}

			// add a reference to the swapchain since we set it as container here
			pcSwapChain->AddRef();
			IDirect3DStereoSurface9* pcBackBufferStereo = new IDirect3DStereoSurface9(pcBackBuffer, s_pcBackBufferRight, pcDevice, (IUnknown*)pcSwapChain, nullptr, nullptr);

			// add backbuffer to vector and add a reference, to not release the backbuffer since the stereo buffer keeps a reference
			pcBackBufferStereo->AddRef();
			s_apcProxyBackBuffers.push_back(pcBackBufferStereo);
		}
		else
		{
			// actually, we shouldn't come here
			OutputDebugString(L"[STS] D3DPRESTENT_PARAMETERS struct is invalid !");
			s_apcProxyBackBuffers.push_back(nullptr);
		}
	}

	// add backbuffer vector and swapchain
	m_apcActiveSwapChains.push_back(pcSwapChain);
	m_aapcActiveProxyBackBufferSurfaces.push_back(s_apcProxyBackBuffers);
}

/**
* Converts D3D9 render target formats to D3D9Ex formats.
* In case compressed formats and bump mapping formats get converted.
***/
D3DFORMAT StereoSplitter::GetD3D9ExFormat(D3DFORMAT eFormat)
{
	switch (eFormat)
	{
		case D3DFMT_UNKNOWN:
			return D3DFORMAT::D3DFMT_UNKNOWN;
		case D3DFMT_R8G8B8:
			return D3DFORMAT::D3DFMT_R8G8B8;
		case D3DFMT_A8R8G8B8:
			return D3DFORMAT::D3DFMT_A8R8G8B8;
		case D3DFMT_X8R8G8B8:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_R5G6B5:
			return D3DFORMAT::D3DFMT_R5G6B5;
		case D3DFMT_X1R5G5B5:
			return D3DFORMAT::D3DFMT_X1R5G5B5;
		case D3DFMT_A1R5G5B5:
			return D3DFORMAT::D3DFMT_A1R5G5B5;
		case D3DFMT_A4R4G4B4:
			return D3DFORMAT::D3DFMT_A4R4G4B4;
		case D3DFMT_R3G3B2:
			return D3DFORMAT::D3DFMT_R3G3B2;
		case D3DFMT_A8:
			return D3DFORMAT::D3DFMT_A8;
		case D3DFMT_A8R3G3B2:
			return D3DFORMAT::D3DFMT_A8R3G3B2;
		case D3DFMT_X4R4G4B4:
			return D3DFORMAT::D3DFMT_X4R4G4B4;
		case D3DFMT_A2B10G10R10:
			return D3DFORMAT::D3DFMT_A2B10G10R10;
		case D3DFMT_A8B8G8R8:
			return D3DFORMAT::D3DFMT_A8B8G8R8;
		case D3DFMT_X8B8G8R8:
			return D3DFORMAT::D3DFMT_X8B8G8R8;
		case D3DFMT_G16R16:
			return D3DFORMAT::D3DFMT_G16R16;
		case D3DFMT_A2R10G10B10:
			return D3DFORMAT::D3DFMT_A2R10G10B10;
		case D3DFMT_A16B16G16R16:
			return D3DFORMAT::D3DFMT_A16B16G16R16;
		case D3DFMT_A8P8:
			return D3DFORMAT::D3DFMT_A8P8;
		case D3DFMT_P8:
			return D3DFORMAT::D3DFMT_P8;
		case D3DFMT_L8:
			return D3DFORMAT::D3DFMT_L8;
		case D3DFMT_A8L8:
			return D3DFORMAT::D3DFMT_A8L8;
		case D3DFMT_A4L4:
			return D3DFORMAT::D3DFMT_A4L4;
		case D3DFMT_V8U8:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_L6V5U5:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_X8L8V8U8:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_Q8W8V8U8:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_V16U16:
			return D3DFORMAT::D3DFMT_X8R8G8B8;
		case D3DFMT_A2W10V10U10:
			return D3DFORMAT::D3DFMT_A2W10V10U10;
		case D3DFMT_UYVY:
			return D3DFORMAT::D3DFMT_UYVY;
		case D3DFMT_R8G8_B8G8:
			return D3DFORMAT::D3DFMT_R8G8_B8G8;
		case D3DFMT_YUY2:
			return D3DFORMAT::D3DFMT_YUY2;
		case D3DFMT_G8R8_G8B8:
			return D3DFORMAT::D3DFMT_G8R8_G8B8;
		case D3DFMT_DXT1:
			// Texture format DXT1 is for textures that are opaque or have a single transparent color. D3DFMT_X4R4G4B4 ??
			return D3DFORMAT::D3DFMT_X1R5G5B5;
		case D3DFMT_DXT2:
			return D3DFMT_X8R8G8B8;
		case D3DFMT_DXT3:
			return D3DFMT_X8R8G8B8;
		case D3DFMT_DXT4:
			return D3DFMT_X8R8G8B8;
		case D3DFMT_DXT5:
			return D3DFMT_X8R8G8B8;
		case D3DFMT_D16_LOCKABLE:
			return D3DFORMAT::D3DFMT_D16_LOCKABLE;
		case D3DFMT_D32:
			return D3DFORMAT::D3DFMT_D32;
		case D3DFMT_D15S1:
			return D3DFORMAT::D3DFMT_D15S1;
		case D3DFMT_D24S8:
			return D3DFORMAT::D3DFMT_D24S8;
		case D3DFMT_D24X8:
			return D3DFORMAT::D3DFMT_D24X8;
		case D3DFMT_D24X4S4:
			return D3DFORMAT::D3DFMT_D24X4S4;
		case D3DFMT_D16:
			return D3DFORMAT::D3DFMT_D16;
		case D3DFMT_D32F_LOCKABLE:
			return D3DFORMAT::D3DFMT_D32F_LOCKABLE;
		case D3DFMT_D24FS8:
			return D3DFORMAT::D3DFMT_D24FS8;
		case D3DFMT_D32_LOCKABLE:
			return D3DFORMAT::D3DFMT_D32_LOCKABLE;
		case D3DFMT_S8_LOCKABLE:
			return D3DFORMAT::D3DFMT_S8_LOCKABLE;
		case D3DFMT_L16:
			return D3DFORMAT::D3DFMT_L16;
		case D3DFMT_VERTEXDATA:
			return D3DFORMAT::D3DFMT_VERTEXDATA;
		case D3DFMT_INDEX16:
			return D3DFORMAT::D3DFMT_INDEX16;
		case D3DFMT_INDEX32:
			return D3DFORMAT::D3DFMT_INDEX32;
		case D3DFMT_Q16W16V16U16:
			return D3DFORMAT::D3DFMT_Q16W16V16U16;
		case D3DFMT_MULTI2_ARGB8:
			return D3DFORMAT::D3DFMT_MULTI2_ARGB8;
		case D3DFMT_R16F:
			return D3DFORMAT::D3DFMT_R16F;
		case D3DFMT_G16R16F:
			return D3DFORMAT::D3DFMT_G16R16F;
		case D3DFMT_A16B16G16R16F:
			return D3DFORMAT::D3DFMT_A16B16G16R16F;
		case D3DFMT_R32F:
			return D3DFORMAT::D3DFMT_R32F;
		case D3DFMT_G32R32F:
			return D3DFORMAT::D3DFMT_G32R32F;
		case D3DFMT_A32B32G32R32F:
			return D3DFORMAT::D3DFMT_A32B32G32R32F;
		case D3DFMT_CxV8U8:
			return D3DFORMAT::D3DFMT_CxV8U8;
		case D3DFMT_A1:
			return D3DFORMAT::D3DFMT_A1;
		case D3DFMT_A2B10G10R10_XR_BIAS:
			return D3DFORMAT::D3DFMT_A2B10G10R10_XR_BIAS;
		case D3DFMT_BINARYBUFFER:
			return D3DFORMAT::D3DFMT_BINARYBUFFER;
		case D3DFMT_FORCE_DWORD:
			return D3DFORMAT::D3DFMT_FORCE_DWORD;
	}
	return eFormat;
}
