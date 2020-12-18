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

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }
#define DEBUG_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#define IS_RENDER_TARGET(d3dusage) ((d3dusage & D3DUSAGE_RENDERTARGET) > 0 ? true : false)


#include"VireioStereoSplitter.h"

/**
* Constructor.
***/
StereoSplitter::StereoSplitter() :AQU_Nodus(),
m_apcActiveRenderTargets(D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT * 2, nullptr),
m_apcActiveTextures(D3D9_SIMULTANEAOUS_TEXTURE_COUNT * 2, nullptr),
m_apcActiveTexturesDisplacement(D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT * 2, nullptr),
m_eTechnique(StereoTechnique::VERSION_4),
m_hFont(nullptr),
m_unTextureNumber(0),
m_unRenderTargetNumber(0),
m_bPresent(false),
m_bApply(true),
m_pcDeviceCurrent(nullptr),
m_psModifierData(nullptr)
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

	// default... monitor stereo output on
	m_bMonitorStereo = true;

	// init d3d9ex specific vectors
	m_aapcActiveProxyBackBufferSurfaces = std::vector<std::vector<IDirect3DStereoSurface9*>>();
	m_apcActiveSwapChains = std::vector<IDirect3DSwapChain9*>();

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	DWORD bMonitorStereo = m_bMonitorStereo;
	bMonitorStereo = GetIniFileSetting(bMonitorStereo, "Stereo Splitter", "bMonitorStereo", szFilePathINI, bFileExists);
	if (bMonitorStereo) m_bMonitorStereo = true; else m_bMonitorStereo = false;

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Stereo Splitter";
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Monitor Stereo";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_bMonitorStereo;
		sEntry.bValue = m_bMonitorStereo;
		m_sMenu.asEntries.push_back(sEntry);
	}
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

	for (UINT unI = 0; unI < (UINT)m_apcActiveSwapChains.size(); unI++)
	{
		if (m_apcActiveSwapChains[unI]) m_apcActiveSwapChains[unI]->Release();
	}

	if (m_pcActiveDepthStencilSurface[0])
	{
		m_pcActiveDepthStencilSurface[0]->Release();
	}

	for (UINT unSampler = 0; unSampler < D3D9_SIMULTANEAOUS_TEXTURE_COUNT; unSampler++)
		if (m_apcActiveTexturesDisplacement[unSampler])
		{
			m_apcActiveTexturesDisplacement[unSampler]->Release();
			m_apcActiveTexturesDisplacement[unSampler] = nullptr;
		}

	for (UINT unSampler = D3DDMAPSAMPLER; unSampler <= D3DVERTEXTEXTURESAMPLER3; unSampler++)
		if (m_apcActiveTextures[unSampler])
		{
			m_apcActiveTextures[unSampler]->Release();
			m_apcActiveTextures[unSampler] = nullptr;
		}

	for (UINT unIndex = 0; unIndex < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; unIndex++)
		if (m_apcActiveRenderTargets[unIndex] != NULL)
		{
			m_apcActiveRenderTargets[unIndex]->Release();
			m_apcActiveRenderTargets[unIndex] = NULL;
		}

	auto itVB = m_apcActiveVertexBuffers.begin();
	while (itVB != m_apcActiveVertexBuffers.end())
	{
		if (itVB->second)
			itVB->second->Release();

		itVB = m_apcActiveVertexBuffers.erase(itVB);
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
	case STS_Commanders::StereoData_Splitter:
		return VLink::Name(VLink::_L::StereoData);
	default:
		break;
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
	case STS_Decommanders::Modifier:
		return VLink::Name(VLink::_L::ModifierData);
	case STS_Decommanders::CreateAdditionalSwapchain:
		return L"CreateAdditionalSwapchain";
	case STS_Decommanders::GetSwapChain:
		return L"GetSwapChain";
	case STS_Decommanders::Reset:
		return L"Reset";
	case STS_Decommanders::Present:
		return L"Present";
	case STS_Decommanders::GetBackBuffer:
		return L"GetBackBuffer";
	case STS_Decommanders::CreateTexture:
		return L"CreateTexture";
	case STS_Decommanders::CreateVolumeTexture:
		return L"CreateVolumeTexture";
	case STS_Decommanders::CreateCubeTexture:
		return L"CreateCubeTexture";
	case STS_Decommanders::CreateVertexBuffer:
		return L"CreateVertexBuffer";
	case STS_Decommanders::CreateIndexBuffer:
		return L"CreateIndexBuffer";
	case STS_Decommanders::CreateRenderTarget:
		return L"CreateRenderTarget";
	case STS_Decommanders::CreateDepthStencilSurface:
		return L"CreateDepthStencilSurface";
	case STS_Decommanders::UpdateSurface:
		return L"UpdateSurface";
	case STS_Decommanders::UpdateTexture:
		return L"UpdateTexture";
	case STS_Decommanders::GetRenderTargetData:
		return L"GetRenderTargetData";
	case STS_Decommanders::GetFrontBufferData:
		return L"GetFrontBufferData";
	case STS_Decommanders::StretchRect:
		return L"StretchRect";
	case STS_Decommanders::ColorFill:
		return L"ColorFill";
	case STS_Decommanders::CreateOffscreenPlainSurface:
		return L"CreateOffscreenPlainSurface";
	case STS_Decommanders::SetRenderTarget:
		return L"SetRenderTarget";
	case STS_Decommanders::GetRenderTarget:
		return L"GetRenderTarget";
	case STS_Decommanders::SetDepthStencilSurface:
		return L"SetDepthStencilSurface";
	case STS_Decommanders::GetDepthStencilSurface:
		return L"GetDepthStencilSurface";
	case STS_Decommanders::BeginScene:
		return L"BeginScene";
	case STS_Decommanders::EndScene:
		return L"EndScene";
	case STS_Decommanders::Clear:
		return L"Clear";
	case STS_Decommanders::SetViewport:
		return L"SetViewport";
	case STS_Decommanders::SetRenderState:
		return L"SetRenderState";
	case STS_Decommanders::GetTexture:
		return L"GetTexture";
	case STS_Decommanders::SetTexture:
		return L"SetTexture";
	case STS_Decommanders::DrawPrimitive:
		return L"DrawPrimitive";
	case STS_Decommanders::DrawIndexedPrimitive:
		return L"DrawIndexedPrimitive";
	case STS_Decommanders::DrawPrimitiveUP:
		return L"DrawPrimitiveUP";
	case STS_Decommanders::DrawIndexedPrimitiveUP:
		return L"DrawIndexedPrimitiveUP";
	case STS_Decommanders::ProcessVertices:
		return L"ProcessVertices";
	case STS_Decommanders::SetStreamSource:
		return L"SetStreamSource";
	case STS_Decommanders::GetStreamSource:
		return L"GetStreamSource";
	case STS_Decommanders::SetIndices:
		return L"SetIndices";
	case STS_Decommanders::GetIndices:
		return L"GetIndices";
	case STS_Decommanders::DrawRectPatch:
		return L"DrawRectPatch";
	case STS_Decommanders::DrawTriPatch:
		return L"DrawTriPatch";
	case STS_Decommanders::SC_Present:
		return L"SC_Present";
	case STS_Decommanders::SC_GetFrontBufferData:
		return L"SC_GetFrontBufferData";
	case STS_Decommanders::SC_GetBackBuffer:
		return L"SC_GetBackBuffer";
	case STS_Decommanders::SB_Apply:
		return L"SB_Apply";
	}

	return L"x";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD StereoSplitter::GetCommanderType(DWORD unCommanderIndex)
{
	switch ((STS_Commanders)unCommanderIndex)
	{
	case STS_Commanders::StereoData_Splitter:
		return VLink::Link(VLink::_L::StereoData);
	default:
		break;
	}

	return NOD_Plugtype::AQU_VOID;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoSplitter::GetDecommanderType(DWORD unDecommanderIndex)
{
	switch ((STS_Decommanders)unDecommanderIndex)
	{
	case STS_Decommanders::Modifier:
		return VLink::Link(VLink::_L::ModifierData);
	case STS_Decommanders::CreateAdditionalSwapchain:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::Present);
	case STS_Decommanders::GetSwapChain:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetSwapChain);
	case STS_Decommanders::Reset:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::Reset);
	case STS_Decommanders::Present:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::Present);
	case STS_Decommanders::GetBackBuffer:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetBackBuffer);
	case STS_Decommanders::CreateTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateTexture);
	case STS_Decommanders::CreateVolumeTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateVolumeTexture);
	case STS_Decommanders::CreateCubeTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateCubeTexture);
	case STS_Decommanders::CreateVertexBuffer:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateVertexBuffer);
	case STS_Decommanders::CreateIndexBuffer:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateIndexBuffer);
	case STS_Decommanders::CreateRenderTarget:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateRenderTarget);
	case STS_Decommanders::CreateDepthStencilSurface:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface);
	case STS_Decommanders::UpdateSurface:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::UpdateSurface);
	case STS_Decommanders::UpdateTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::UpdateTexture);
	case STS_Decommanders::GetRenderTargetData:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetRenderTargetData);
	case STS_Decommanders::GetFrontBufferData:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetFrontBufferData);
	case STS_Decommanders::StretchRect:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::StretchRect);
	case STS_Decommanders::ColorFill:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::ColorFill);
	case STS_Decommanders::CreateOffscreenPlainSurface:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface);
	case STS_Decommanders::SetRenderTarget:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetRenderTarget);
	case STS_Decommanders::GetRenderTarget:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetRenderTarget);
	case STS_Decommanders::SetDepthStencilSurface:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface);
	case STS_Decommanders::GetDepthStencilSurface:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface);
	case STS_Decommanders::BeginScene:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::BeginScene);
	case STS_Decommanders::EndScene:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::EndScene);
	case STS_Decommanders::Clear:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::Clear);
	case STS_Decommanders::SetViewport:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetViewport);
	case STS_Decommanders::SetRenderState:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetRenderState);
	case STS_Decommanders::GetTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetTexture);
	case STS_Decommanders::SetTexture:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetTexture);
	case STS_Decommanders::DrawPrimitive:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawPrimitive);
	case STS_Decommanders::DrawIndexedPrimitive:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive);
	case STS_Decommanders::DrawPrimitiveUP:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP);
	case STS_Decommanders::DrawIndexedPrimitiveUP:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP);
	case STS_Decommanders::ProcessVertices:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::ProcessVertices);
	case STS_Decommanders::SetStreamSource:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetStreamSource);
	case STS_Decommanders::GetStreamSource:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetStreamSource);
	case STS_Decommanders::SetIndices:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetIndices);
	case STS_Decommanders::GetIndices:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetIndices);
	case STS_Decommanders::DrawRectPatch:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawRectPatch);
	case STS_Decommanders::DrawTriPatch:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::DrawTriPatch);
	case STS_Decommanders::SC_Present:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9, (int)VMT_IDIRECT3DSWAPCHAIN9::Present);
	case STS_Decommanders::SC_GetFrontBufferData:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9, (int)VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData);
	case STS_Decommanders::SC_GetBackBuffer:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9, (int)VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer);
	case STS_Decommanders::SB_Apply:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, (int)VMT_IDIRECT3DSTATEBLOCK9::Apply);

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
	case STS_Commanders::StereoData_Splitter:
		return (void*)&m_sStereoData;
	default:
		break;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoSplitter::SetInputPointer(DWORD unDecommanderIndex, void* pData)
{
	if (unDecommanderIndex < NUMBER_OF_DECOMMANDERS)
		m_ppInput[unDecommanderIndex] = (pData);
}

/**
* Stereo Splitter supports various D3D9 calls.
***/
bool StereoSplitter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9)
		{
			if ((nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::Present) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetRenderTarget) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::Clear) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawPrimitive) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetBackBuffer) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::UpdateSurface) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::UpdateTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::StretchRect) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::ColorFill) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetRenderTarget) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::Reset) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::BeginScene) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::EndScene) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawRectPatch) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::DrawTriPatch) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetRenderState) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetViewport) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateVolumeTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateCubeTexture) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateVertexBuffer) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateIndexBuffer) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateRenderTarget) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetRenderTargetData) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetFrontBufferData) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::ProcessVertices) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetStreamSource) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetStreamSource) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetIndices) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetIndices) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetSwapChain))
				return true;
		}
		else if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9)
		{
			if ((nD3DMethod == (int)VMT_IDIRECT3DSWAPCHAIN9::Present) ||
				(nD3DMethod == (int)VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer) ||
				(nD3DMethod == (int)VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData)) return true;
		}
		else if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9)
		{
			if (nD3DMethod == (int)VMT_IDIRECT3DSTATEBLOCK9::Apply) return true;
		}
		else if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::D3DX9) return true;
	}
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
***/
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD unNumberConnected, int& nProvokerIndex)
{
	// return if any method call is done by proxy classes
	if (s_bDeviceInUseByProxy) return nullptr;

	// #define _DEBUG_STEREO_SPLITTER
#ifdef _DEBUG_STEREO_SPLITTER
	{ wchar_t buf[128]; wsprintf(buf, L"[STS] if %u mt %u", eD3DInterface, eD3DMethod); OutputDebugString(buf); }
#endif

	// save ini file ?
	if (m_nIniFrameCount)
	{
		if (m_nIniFrameCount == 1)
		{
			// save ini settings.. get file path
			char szFilePathINI[1024];
			GetCurrentDirectoryA(1024, szFilePathINI);
			strcat_s(szFilePathINI, "\\VireioPerception.ini");
			bool bFileExists = false;
			DWORD bMonitorStereo = m_bMonitorStereo;
			bMonitorStereo = GetIniFileSetting(bMonitorStereo, "Stereo Splitter", "bMonitorStereo", szFilePathINI, bFileExists);
			if (bMonitorStereo) m_bMonitorStereo = true; else m_bMonitorStereo = false;
		}
		m_nIniFrameCount--;
	}

	// main menu update ?
	if (m_sMenu.bOnChanged)
	{
		// set back event bool, set ini file frame count
		m_sMenu.bOnChanged = false;
		m_nIniFrameCount = 300;

		// loop through entries
		for (size_t nIx = 0; nIx < m_sMenu.asEntries.size(); nIx++)
		{
			// entry index changed ?
			if (m_sMenu.asEntries[nIx].bOnChanged)
			{
				m_sMenu.asEntries[nIx].bOnChanged = false;
			}
		}
	}

	// set a static HRESULT to be returned by any method and the current device
	static HRESULT nHr = S_OK;
	m_pcDeviceCurrent = (IDirect3DDevice9*)pThis;

	// the double call flag is only used within 
	// drawing methods (for stereo drawing), so
	// set it here anyway
	if (m_bPresent)
		nProvokerIndex |= AQU_PluginFlags::DoubleCallFlag;

	switch (eD3DInterface)
	{
#pragma region IDirect3DDevice9
	case (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9:
	{
		switch (eD3DMethod)
		{
#pragma region Present
		case (int)VMT_IDIRECT3DDEVICE9::Present:
		{
			SHOW_CALL("Present");

			// TODO !! make this an option !!
			static int s_nPresentStartCount = 3;
			int nFlags = 0;
			if (s_nPresentStartCount <= 0)
				switch (m_eTechnique)
				{
				case StereoSplitter::StereoTechnique::VERSION_3:
					nHr = Present_v3(nFlags);
					break;
				case StereoSplitter::StereoTechnique::VERSION_4:
					nHr = Present_v4(nFlags);
					break;
				}
			else
				s_nPresentStartCount--;
		}
		return nullptr;
#pragma endregion
#pragma region BeginScene
		case (int)VMT_IDIRECT3DDEVICE9::BeginScene:
			// ensure left drawing side here
			SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion 
#pragma region EndScene
		case (int)VMT_IDIRECT3DDEVICE9::EndScene:
			// ensure left drawing side here
			SetDrawingSide((LPDIRECT3DDEVICE9)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion 
#pragma region SetRenderTarget
		case (int)VMT_IDIRECT3DDEVICE9::SetRenderTarget:
		{
			SHOW_CALL("SetRenderTarget");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = SetRenderTarget_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = SetRenderTarget_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetDepthStencilSurface
		case (int)VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface:
		{
			SHOW_CALL("SetDepthStencilSurface");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = SetDepthStencilSurface_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = SetDepthStencilSurface_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetTexture
		case (int)VMT_IDIRECT3DDEVICE9::SetTexture:
		{
			SHOW_CALL("SetTexture");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = SetTexture_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = SetTexture_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region Clear
		case (int)VMT_IDIRECT3DDEVICE9::Clear:
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
		case (int)VMT_IDIRECT3DDEVICE9::DrawPrimitive:
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
		case (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive:
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
		case (int)VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP:
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
		case (int)VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP:
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
		case (int)VMT_IDIRECT3DDEVICE9::UpdateSurface:
		{
			SHOW_CALL("UpdateSurface");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = UpdateSurface_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = UpdateSurface_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion  
#pragma region UpdateTexture
		case (int)VMT_IDIRECT3DDEVICE9::UpdateTexture:
		{
			SHOW_CALL("UpdateTexture");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = UpdateTexture_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = UpdateTexture_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion   
#pragma region StretchRect 
		case (int)VMT_IDIRECT3DDEVICE9::StretchRect:
		{
			SHOW_CALL("StretchRect");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = StretchRect_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = StretchRect_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion  
#pragma region ColorFill
		case (int)VMT_IDIRECT3DDEVICE9::ColorFill:
		{
			SHOW_CALL("ColorFill");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = ColorFill_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				nHr = ColorFill_v4(nFlags);
				break;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion 
#pragma region GetBackBuffer
		case (int)VMT_IDIRECT3DDEVICE9::GetBackBuffer:
		{
			SHOW_CALL("GetBackBuffer");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = GetBackBuffer_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion 
#pragma region GetRenderTarget
		case (int)VMT_IDIRECT3DDEVICE9::GetRenderTarget:
		{
			SHOW_CALL("GetRenderTarget");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = GetRenderTarget_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion 
#pragma region GetDepthStencilSurface
		case (int)VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface:
		{
			SHOW_CALL("GetDepthStencilSurface");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = GetDepthStencilSurface_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion  
#pragma region GetTexture 
		case (int)VMT_IDIRECT3DDEVICE9::GetTexture:
		{
			SHOW_CALL("GetTexture");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = GetTexture_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion  
#pragma region Reset
		case (int)VMT_IDIRECT3DDEVICE9::Reset:
		{
			SHOW_CALL("Reset");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = Reset_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion 
#pragma region DrawRectPatch
		case (int)VMT_IDIRECT3DDEVICE9::DrawRectPatch:
			// TODO !!
			return nullptr;
#pragma endregion  
#pragma region DrawTriPatch
		case (int)VMT_IDIRECT3DDEVICE9::DrawTriPatch:
			// TODO !!
			return nullptr;
#pragma endregion 
#pragma region SetRenderState
		case (int)VMT_IDIRECT3DDEVICE9::SetRenderState:
			if (!m_ppInput[(int)STS_Decommanders::SetRenderState]) return nullptr;
			else
			{
				void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetRenderState]);
				D3DRENDERSTATETYPE* peState;
				DWORD* punValue;
				if (ppIn[0]) peState = *(D3DRENDERSTATETYPE**)ppIn[0]; else return nullptr;
				if (ppIn[1]) punValue = *(DWORD**)ppIn[1]; else return nullptr;

				// catch D3DRS_POINT for "Resolve Z-Buffer" operation... read "Advanced-DX9-Capabilities-for-ATI-Radeon-Cards_v2.pdf" for more info
				if (*peState == D3DRENDERSTATETYPE::D3DRS_POINTSIZE)
				{
					// the value 0x3f800000 seems to be also a resolve z buffer request
					// - it is send by Skyrim on AMD cards
					// however, the following code works to do it
					DWORD unValue = *punValue;
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
		case (int)VMT_IDIRECT3DDEVICE9::SetViewport:
			if (!m_ppInput[(int)STS_Decommanders::SetViewport]) return nullptr;
			else
			{
				void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetViewport]);
				D3DVIEWPORT9** ppsViewport;
				if (ppIn[0]) ppsViewport = *(D3DVIEWPORT9***)ppIn[0]; else return nullptr;

				// TODO !! move method(s) here
				nHr = SetViewport(m_pcDeviceCurrent, *ppsViewport);

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return (void*)&nHr;
			}
			return nullptr;
#pragma endregion
#pragma region CreateTexture
		case (int)VMT_IDIRECT3DDEVICE9::CreateTexture:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateVolumeTexture
		case  (int)VMT_IDIRECT3DDEVICE9::CreateVolumeTexture:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateCubeTexture
		case  (int)VMT_IDIRECT3DDEVICE9::CreateCubeTexture:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateVertexBuffer
		case  (int)VMT_IDIRECT3DDEVICE9::CreateVertexBuffer:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateIndexBuffer
		case  (int)VMT_IDIRECT3DDEVICE9::CreateIndexBuffer:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateRenderTarget
		case  (int)VMT_IDIRECT3DDEVICE9::CreateRenderTarget:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateDepthStencilSurface
		case  (int)VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region CreateOffscreenPlainSurface
		case (int)VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region GetRendertargetData
		case  (int)VMT_IDIRECT3DDEVICE9::GetRenderTargetData:
		{
			SHOW_CALL("GetRenderTargetData");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = GetRenderTargetData_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetFrontBufferData
		case (int)VMT_IDIRECT3DDEVICE9::GetFrontBufferData:
			// TODO !! v3 call !! v4 method needed ?
			return nullptr;
#pragma endregion
#pragma region ProcessVertices
		case (int)VMT_IDIRECT3DDEVICE9::ProcessVertices:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region SetStreamSource
		case (int)VMT_IDIRECT3DDEVICE9::SetStreamSource:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region GetStreamSource
		case (int)VMT_IDIRECT3DDEVICE9::GetStreamSource:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region SetIndices
		case (int)VMT_IDIRECT3DDEVICE9::SetIndices:
			// TODO !!
			return nullptr;
#pragma endregion
#pragma region GetIndices
		case (int)VMT_IDIRECT3DDEVICE9::GetIndices:
			// TODO !!
			return nullptr;
#pragma endregion
#pragma region CreateAdditionalSwapChain
		case (int)VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
#pragma region GetSwapChain
		case (int)VMT_IDIRECT3DDEVICE9::GetSwapChain:
			// TODO !! v3 call
			return nullptr;
#pragma endregion
		}
		return nullptr;
	}
#pragma endregion
#pragma region IDirect3DSwapChain9
	case (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9:
		switch (eD3DMethod)
		{
		case (int)VMT_IDIRECT3DSWAPCHAIN9::Present:
		{
			// get the device and call present
			((LPDIRECT3DSWAPCHAIN9)pThis)->GetDevice(&m_pcDeviceCurrent);
			if (m_pcDeviceCurrent)
			{
				int nFlags = 0;
				switch (m_eTechnique)
				{
				case StereoSplitter::StereoTechnique::VERSION_3:
				{
					// TODO !!
					// nHr = Present_v3(nFlags);
					/*if (!m_apcActiveSwapChains.size()) Present(pcDevice, true);

					// only call internal present if called by active swap chain !!
					if (((IDirect3DStereoSwapChain9*)m_apcActiveSwapChains[0])->GetActual() == pThis)
						Present(pcDevice, false);*/
				}
				break;
				case StereoSplitter::StereoTechnique::VERSION_4:
					nHr = Present_v4(nFlags);
					break;
				}
				m_pcDeviceCurrent->Release();
			}
		}
		return nullptr;
		case (int)VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer:
		{
			SHOW_CALL("GetBackBuffer");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = SC_GetBackBuffer_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
		case (int)VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData:
		{
			SHOW_CALL("GetFrontBufferData");

			int nFlags = 0;
			switch (m_eTechnique)
			{
			case StereoSplitter::StereoTechnique::VERSION_3:
				nHr = SC_GetFrontBufferData_v3(nFlags);
				break;
			case StereoSplitter::StereoTechnique::VERSION_4:
				// ensure left drawing side here
				SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);
				return nullptr;
			}

			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
		}
		return nullptr;
#pragma endregion
#pragma region IDirect3DStateBlock9
	case (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9:
		switch (eD3DMethod)
		{
		case (int)VMT_IDIRECT3DSTATEBLOCK9::Apply:
			// TODO !! v3 special handling here ?
			Apply();
			return nullptr;
		}
#pragma endregion
#pragma region D3DX9
		/*case INTERFACE_D3DX9:
			switch (eD3DMethod)
			{
	#pragma region D3D9_D3DXLoadSurfaceFromSurface
			case MT_D3DX9::D3D9_D3DXLoadSurfaceFromSurface:
				SHOW_CALL("INTERFACE_D3DX9::D3D9_D3DXLoadSurfaceFromSurface");
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
						// get actual surfaces and do the method call for the left surfaces
						IDirect3DSurface9* pSourceSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcSrcSurface))->GetActualLeft();
						IDirect3DSurface9* pSourceSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcSrcSurface))->GetActualRight();
						IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualLeft();
						IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualRight();
						nHr = D3DXLoadSurfaceFromSurface(pDestSurfaceLeft, *m_ppsDestPalette, *m_ppsDestRect, pSourceSurfaceLeft, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey);

						// no proxy surface ?
						if ((!pSourceSurfaceLeft) || (!pDestSurfaceLeft))
						{
							OutputDebugString(L"[STS] : Fatal Error - No Proxy Surface provided.");
							DEBUG_HEX(*m_ppcSrcSurface);
							DEBUG_HEX(*m_ppcDestSurface);
							exit(99);
						}

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
	#pragma endregion
	#pragma region D3D9_D3DXLoadSurfaceFromMemory
			case MT_D3DX9::D3D9_D3DXLoadSurfaceFromMemory:
				SHOW_CALL("INTERFACE_D3DX9::D3D9_D3DXLoadSurfaceFromMemory");
				if (!m_ppcDestSurface) return nullptr;
				if (!m_ppsDestPalette) return nullptr;
				if (!m_ppsDestRect) return nullptr;
				if (!m_ppSrcMemory) return nullptr;
				if (!m_peSrcFormat) return nullptr;
				if (!m_punSrcPitch) return nullptr;
				if (!m_ppsSrcPalette) return nullptr;
				if (!m_ppsSrcRect) return nullptr;
				if (!m_punFilter) return nullptr;
				if (!m_punColorKey) return nullptr;
				{
					// use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
					if (m_bUseD3D9Ex)
					{
						// get actual surfaces and do the method call for the left surface
						IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualLeft();
						IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualRight();
						nHr = D3DXLoadSurfaceFromMemory(pDestSurfaceLeft, *m_ppsDestPalette, *m_ppsDestRect, *m_ppSrcMemory, *m_peSrcFormat, *m_punSrcPitch, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey);

						// no proxy surface ?
						if (!pDestSurfaceLeft)
						{
							OutputDebugString(L"[STS] : Fatal Error - No Proxy Surface provided.");
							DEBUG_HEX(*m_ppcDestSurface);
							exit(99);
						}

						if (SUCCEEDED(nHr))
						{
							if (pDestSurfaceRight)
							{
								// do the call for the right side
								if (FAILED(D3DXLoadSurfaceFromMemory(pDestSurfaceRight, *m_ppsDestPalette, *m_ppsDestRect, *m_ppSrcMemory, *m_peSrcFormat, *m_punSrcPitch, *m_ppsSrcPalette, *m_ppsSrcRect, *m_punFilter, *m_punColorKey)))
								{
									OutputDebugString(L"[STS] ERROR: D3DXLoadSurfaceFromMemory - Failed to load to destination right.\n");
								}
							}
						}

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						return (void*)&nHr;
					}
				}
				return nullptr;
	#pragma endregion
	#pragma region D3D9_D3DXCreateTexture
			case MT_D3DX9::D3D9_D3DXCreateTexture:
				if (m_bUseD3D9Ex)
				{
					if (!m_punWidth) return nullptr;
					if (!m_punHeight) return nullptr;
					if (!m_punLevels) return nullptr;
					if (!m_punUsage) return nullptr;
					if (!m_peFormat) return nullptr;
					if (!m_pePool) return nullptr;
					if (!m_pppcTextureCreate) return nullptr;
	#ifdef _DEBUGTHIS
					wchar_t buf[32];
					wsprintf(buf, L"m_punWidth %u", *m_punWidth); OutputDebugString(buf);
					wsprintf(buf, L"m_punHeight %u", *m_punHeight); OutputDebugString(buf);
					wsprintf(buf, L"m_punLevels %u", *m_punLevels); OutputDebugString(buf);
					wsprintf(buf, L"m_punUsage %u", *m_punUsage); OutputDebugString(buf);
					wsprintf(buf, L"m_peFormat %u", *m_peFormat); OutputDebugString(buf);
					wsprintf(buf, L"m_pePool %u", *m_pePool); OutputDebugString(buf);
	#endif
					SHOW_CALL("D3DXCreateTexture");

					static IDirect3DTexture9* s_pcLeftTexture = NULL;
					static IDirect3DTexture9* s_pcRightTexture = NULL;
					s_pcLeftTexture = NULL;
					s_pcRightTexture = NULL;

					D3DPOOL ePool = *m_pePool;
					D3DFORMAT eFormat = *m_peFormat;

					static IDirect3DDevice9Ex* s_pcDirect3DDevice9Ex = NULL;
					s_pcDirect3DDevice9Ex = NULL;
					if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
						(*m_pePool) == D3DPOOL_MANAGED)
					{
						ePool = D3DPOOL_DEFAULT;
						s_pcDirect3DDevice9Ex->Release();
					}

	#ifdef _DEBUGTHIS
					OutputDebugString(L"[STS] Create texture format :");
					DEBUG_UINT(eFormat);
					DEBUG_HEX(eFormat);
	#endif

					// try and create left
					if (SUCCEEDED(nHr = D3DXCreateTexture((IDirect3DDevice9*)pThis, *m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcLeftTexture)))
					{
						// Does this Texture need duplicating?
						if (ShouldDuplicateTexture(*m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, *m_peFormat, *m_pePool))
						{
							if (FAILED(D3DXCreateTexture((IDirect3DDevice9*)pThis, *m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, &s_pcRightTexture)))
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
	#pragma region D3D9_D3DXCreateTextureFromFileInMemory
			case MT_D3DX9::D3D9_D3DXCreateTextureFromFileInMemory:
				if (m_bUseD3D9Ex)
				{
					if (!m_ppSrcData) return nullptr;
					if (!m_punSrcDataSize) return nullptr;
					if (!m_pppcTextureCreate) return nullptr;
	#ifdef _DEBUGTHIS
					wchar_t buf[32];
					wsprintf(buf, L"m_punWidth %u", *m_punWidth); OutputDebugString(buf);
					wsprintf(buf, L"m_punHeight %u", *m_punHeight); OutputDebugString(buf);
					wsprintf(buf, L"m_punLevels %u", *m_punLevels); OutputDebugString(buf);
					wsprintf(buf, L"m_punUsage %u", *m_punUsage); OutputDebugString(buf);
					wsprintf(buf, L"m_peFormat %u", *m_peFormat); OutputDebugString(buf);
					wsprintf(buf, L"m_pePool %u", *m_pePool); OutputDebugString(buf);
	#endif
					SHOW_CALL("D3DXCreateTextureFromFileInMemory");

					static IDirect3DTexture9* s_pcLeftTexture = NULL;
					static IDirect3DTexture9* s_pcRightTexture = NULL;
					s_pcLeftTexture = NULL;
					s_pcRightTexture = NULL;

	#ifdef _DEBUGTHIS
					OutputDebugString(L"[STS] Create texture format :");
					DEBUG_UINT(eFormat);
					DEBUG_HEX(eFormat);
	#endif

					// try and create left
					if (SUCCEEDED(nHr = D3DXCreateTextureFromFileInMemory((IDirect3DDevice9*)pThis, *m_ppSrcData, *m_punSrcDataSize, &s_pcLeftTexture)))
					{
						if (s_pcLeftTexture)
						{
							// get description
							UINT unLevels = s_pcLeftTexture->GetLevelCount();
							D3DSURFACE_DESC sDesc = {};
							IDirect3DSurface9* pcSurface = nullptr;
							s_pcLeftTexture->GetSurfaceLevel(0, &pcSurface);
							if (pcSurface) pcSurface->GetDesc(&sDesc);

							// Does this Texture need duplicating?
							if (ShouldDuplicateTexture(sDesc.Width, sDesc.Height, unLevels, sDesc.Usage, sDesc.Format, sDesc.Pool))
							{
								if (FAILED(D3DXCreateTextureFromFileInMemory((IDirect3DDevice9*)pThis, *m_ppSrcData, *m_punSrcDataSize, &s_pcRightTexture)))
								{
									OutputDebugString(L"[STS] Failed to create right eye texture while attempting to create stereo pair, falling back to mono\n");
									s_pcRightTexture = NULL;
								}
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
	#pragma region D3D9_D3DXCreateTextureFromFileInMemoryEx
			case MT_D3DX9::D3D9_D3DXCreateTextureFromFileInMemoryEx:
				if (m_bUseD3D9Ex)
				{
					if (!m_ppSrcData) return nullptr;
					if (!m_punSrcDataSize) return nullptr;
					if (!m_punWidth) return nullptr;
					if (!m_punHeight) return nullptr;
					if (!m_punLevels) return nullptr;
					if (!m_punUsage) return nullptr;
					if (!m_peFormat) return nullptr;
					if (!m_pePool) return nullptr;
					if (!m_punFilter) return nullptr;
					if (!m_punMipFilter) return nullptr;
					if (!m_punColorKey) return nullptr;
					if (!m_ppsSrcInfo) return nullptr;
					if (!m_ppsPalette) return nullptr;
					if (!m_pppcTextureCreate) return nullptr;
	#ifdef _DEBUGTHIS
					wchar_t buf[32];
					wsprintf(buf, L"m_punWidth %u", *m_punWidth); OutputDebugString(buf);
					wsprintf(buf, L"m_punHeight %u", *m_punHeight); OutputDebugString(buf);
					wsprintf(buf, L"m_punLevels %u", *m_punLevels); OutputDebugString(buf);
					wsprintf(buf, L"m_punUsage %u", *m_punUsage); OutputDebugString(buf);
					wsprintf(buf, L"m_peFormat %u", *m_peFormat); OutputDebugString(buf);
					wsprintf(buf, L"m_pePool %u", *m_pePool); OutputDebugString(buf);
	#endif
					SHOW_CALL("D3DXCreateTextureFromFileInMemoryEx");

					static IDirect3DTexture9* s_pcLeftTexture = NULL;
					static IDirect3DTexture9* s_pcRightTexture = NULL;
					s_pcLeftTexture = NULL;
					s_pcRightTexture = NULL;

					D3DPOOL ePool = *m_pePool;
					D3DFORMAT eFormat = *m_peFormat;

					HRESULT hr = S_OK;
					static IDirect3DDevice9Ex* s_pcDirect3DDevice9Ex = NULL;
					s_pcDirect3DDevice9Ex = NULL;
					if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))) &&
						(*m_pePool) == D3DPOOL_MANAGED)
					{
						ePool = D3DPOOL_DEFAULT;
						s_pcDirect3DDevice9Ex->Release();
					}

	#ifdef _DEBUGTHIS
					OutputDebugString(L"[STS] Create texture format :");
					DEBUG_UINT(eFormat);
					DEBUG_HEX(eFormat);
	#endif

					// try and create left
					if (SUCCEEDED(nHr = D3DXCreateTextureFromFileInMemoryEx((IDirect3DDevice9*)pThis, *m_ppSrcData, *m_punSrcDataSize, *m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, *m_punFilter, *m_punMipFilter, *m_punColorKey, *m_ppsSrcInfo, *m_ppsPalette, &s_pcLeftTexture)))
					{
						// Does this Texture need duplicating?
						if (ShouldDuplicateTexture(*m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, *m_peFormat, *m_pePool))
						{
							if (FAILED(D3DXCreateTextureFromFileInMemoryEx((IDirect3DDevice9*)pThis, *m_ppSrcData, *m_punSrcDataSize, *m_punWidth, *m_punHeight, *m_punLevels, *m_punUsage, eFormat, ePool, *m_punFilter, *m_punMipFilter, *m_punColorKey, *m_ppsSrcInfo, *m_ppsPalette, &s_pcRightTexture)))
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
	#pragma region D3D9_D3DXFillTexture
			case MT_D3DX9::D3D9_D3DXFillTexture:
				SHOW_CALL("INTERFACE_D3DX9::D3D9_D3DXFillTexture");
				if (!m_ppcTexture_D3DX) return nullptr;
				if (!m_ppFunction) return nullptr;
				if (!m_ppData) return nullptr;
				{
					// use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
					if (m_bUseD3D9Ex)
					{
						// set return flag and verify the texture
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						if (!(*m_ppcTexture_D3DX)) { nHr = D3DERR_INVALIDCALL; return (void*)&nHr; }
						if ((*m_ppcTexture_D3DX)->GetType() != D3DRESOURCETYPE::D3DRTYPE_TEXTURE) { nHr = D3DERR_INVALIDCALL; return (void*)&nHr; }

						// get actual texture
						IDirect3DTexture9* pcActualLeft = (static_cast<IDirect3DStereoTexture9*>(*m_ppcTexture_D3DX))->GetActualLeft();
						IDirect3DTexture9* pcActualRight = (static_cast<IDirect3DStereoTexture9*>(*m_ppcTexture_D3DX))->GetActualRight();

						// no proxy tex ?
						if (!pcActualLeft)
						{
							OutputDebugString(L"[STS] : Fatal Error - No Proxy Texture provided.");
							DEBUG_HEX(*m_ppcTexture_D3DX);
							exit(99);
						}

						// call the d3d9ex compatible method for the left side
						nHr = D3D9Ex_D3DXFillTexture(pcActualLeft, (LPD3DXFILL2D)*m_ppFunction, *m_ppData);

						// copy left to right if right side present
						if (pcActualRight) ((IDirect3DDevice9*)pThis)->UpdateTexture(pcActualLeft, pcActualRight);

						// method replaced, immediately return
						return (void*)&nHr;
					}
				}
				return nullptr;
	#pragma endregion
	#pragma region D3D9_D3DXFillVolumeTexture
			case MT_D3DX9::D3D9_D3DXFillVolumeTexture:
				SHOW_CALL("INTERFACE_D3DX9::D3D9_D3DXFillVolumeTexture");
				if (!m_ppcVolumeTexture) return nullptr;
				if (!m_ppFunction) return nullptr;
				if (!m_ppData) return nullptr;
				{
					// use D3D9Ex device ? handle proxy surfaces instead of private interfaces.. code from driver <v3
					if (m_bUseD3D9Ex)
					{
						// set return flag and verify the texture
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						if (!(*m_ppcVolumeTexture)) { nHr = D3DERR_INVALIDCALL; return (void*)&nHr; }
						if ((*m_ppcVolumeTexture)->GetType() != D3DRESOURCETYPE::D3DRTYPE_VOLUMETEXTURE) { nHr = D3DERR_INVALIDCALL; return (void*)&nHr; }

						// get actual volume
						IDirect3DVolumeTexture9* pcVolumeActual = (static_cast<IDirect3DStereoVolumeTexture9*>(*m_ppcVolumeTexture))->GetActual();

						// no proxy volume tex ?
						if (!pcVolumeActual)
						{
							OutputDebugString(L"[STS] : Fatal Error - No Proxy Volume Texture provided.");
							DEBUG_HEX(*m_ppcVolumeTexture);
							exit(99);
						}

						// call the d3d9ex compatible method
						nHr = D3D9Ex_D3DXFillVolumeTexture(pcVolumeActual, (LPD3DXFILL3D)*m_ppFunction, *m_ppData);

						// method replaced, immediately return
						return (void*)&nHr;
					}
				}
				return nullptr;
	#pragma endregion
	#pragma region D3D9_D3DX_NOT_IMPLEMENTED
			default:
			{
				// exit if method not implemented !
				wchar_t buf[128]; wsprintf(buf, L"[STS] Fatal Error : D3DX method not implemented ! Index : %u", eD3DMethod); OutputDebugString(buf); exit(99);
			}
			break;
	#pragma endregion
			}
			return nullptr;*/
#pragma endregion
	}
	return nullptr;
}

/**
* There's some windows event on our node.
***/
void StereoSplitter::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	// if (VERSION_3) return true;
	
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
	
	#define DRAW_INDICATORS
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
	if (m_psModifierData)
	{
		std::vector<Vireio_Constant_Rule_Index_DX9>* pasVSIndices = m_psModifierData->m_pasVSConstantRuleIndices;
		if (eSide == RenderPosition::Left)
		{
			for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasVSIndices->size(); nI++)
				pcDevice->SetVertexShaderConstantF((*pasVSIndices)[nI].m_dwConstantRuleRegister, (*pasVSIndices)[nI].m_afConstantDataLeft, (*pasVSIndices)[nI].m_dwConstantRuleRegisterCount);
		}
		else
		{
			for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasVSIndices->size(); nI++)
				pcDevice->SetVertexShaderConstantF((*pasVSIndices)[nI].m_dwConstantRuleRegister, (*pasVSIndices)[nI].m_afConstantDataRight, (*pasVSIndices)[nI].m_dwConstantRuleRegisterCount);
		}
		std::vector<Vireio_Constant_Rule_Index_DX9>* pasPSIndices = m_psModifierData->m_pasPSConstantRuleIndices;
		if (eSide == RenderPosition::Left)
		{
			for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasPSIndices->size(); nI++)
				pcDevice->SetPixelShaderConstantF((*pasPSIndices)[nI].m_dwConstantRuleRegister, (*pasPSIndices)[nI].m_afConstantDataLeft, (*pasPSIndices)[nI].m_dwConstantRuleRegisterCount);
		}
		else
		{
			for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < pasPSIndices->size(); nI++)
				pcDevice->SetPixelShaderConstantF((*pasPSIndices)[nI].m_dwConstantRuleRegister, (*pasPSIndices)[nI].m_afConstantDataRight, (*pasPSIndices)[nI].m_dwConstantRuleRegisterCount);
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
				* ppcStereoTwinTexture = nullptr;
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
				* ppcStereoTwinTexture = nullptr;
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

#pragma region /// => Version 3
/// <summary> 
/// Init v3 internal fields.
/// </summary>
void StereoSplitter::Init_v3()
{
	/*
		{
			// Wrap the swap chain
			IDirect3DSwapChain9* pActualPrimarySwapChain;
			if (FAILED(pcDevice->GetSwapChain(0, &pActualPrimarySwapChain)))
			{
				OutputDebugString(L"[STS] Critical Error : No D3D9 swapchain present !");
				exit(1);
			}

			if (m_apcActiveSwapChains.size()) OutputDebugString(L"[STS] Fatal ! Active swapchain already set !");
			assert(m_apcActiveSwapChains.size() == 0);
			m_apcActiveSwapChains.push_back(new IDirect3DStereoSwapChain9(pActualPrimarySwapChain, pcDevice, false));
			assert(m_apcActiveSwapChains.size() == 1);

			// Set the primary rendertarget to the first stereo backbuffer
			IDirect3DSurface9* pWrappedBackBuffer;
			m_apcActiveSwapChains[0]->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pWrappedBackBuffer);

			// cast proxy surface
			IDirect3DStereoSurface9* newRenderTarget = static_cast<IDirect3DStereoSurface9*>(pWrappedBackBuffer);

			// set first render target
			if (newRenderTarget == NULL)
				pcDevice->SetRenderTarget(0, NULL);
			else
				pcDevice->SetRenderTarget(0, newRenderTarget->GetActualLeft());

			// release old render target
			if (m_apcActiveRenderTargets[0] != NULL)
				m_apcActiveRenderTargets[0]->Release();

			// changing rendertarget resets viewport to fullsurface
			m_bActiveViewportIsDefault = true;

			// replace with new render target (may be NULL)
			m_apcActiveRenderTargets[0] = newRenderTarget;
			if (m_apcActiveRenderTargets[0] != NULL)
				m_apcActiveRenderTargets[0]->AddRef();

			pWrappedBackBuffer->Release();
			pWrappedBackBuffer = NULL;

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
					hr = pcDevice->CreateDepthStencilSurface(sDesc.Width, sDesc.Height, sDesc.Format, sDesc.MultiSampleType, sDesc.MultiSampleQuality, false, &pcDepthStencilRight, nullptr);
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
		}*/


		// get viewport
		//pcDevice->GetViewport(&m_sLastViewportSet);

}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateAdditionalSwapchain_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_ppsPresentationParams) return nullptr;
				if (!m_pppcSwapChain) return nullptr;

				if (!m_bPresent)
					Present((IDirect3DDevice9*)pThis, true);

				IDirect3DSwapChain9* pcActualSwapChain = nullptr;
				nHr = ((IDirect3DDevice9*)pThis)->CreateAdditionalSwapChain(*m_ppsPresentationParams, &pcActualSwapChain);

				if (SUCCEEDED(nHr))
				{
					IDirect3DStereoSwapChain9* pcWrappedSwapChain = new IDirect3DStereoSwapChain9(pcActualSwapChain, (IDirect3DDevice9*)pThis, true);
					*(*m_pppcSwapChain) = pcWrappedSwapChain;
				}

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return (void*)&nHr;
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetSwapChain_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_punISwapChain) return nullptr;
				if (!m_pppcSwapChain) return nullptr;

				nHr = S_OK;
				try
				{
					*(*m_pppcSwapChain) = m_apcActiveSwapChains.at(*m_punISwapChain);
					// pcDevice->GetSwapChain increases ref count on the chain (docs don't say this)
					(*(*m_pppcSwapChain))->AddRef();
				}
				catch (std::out_of_range)
				{
					OutputDebugStringA("[STS] GetSwapChain: out of range fetching swap chain");
					nHr = D3DERR_INVALIDCALL;
				}

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return (void*)&nHr;
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::Reset_v3(int& nFlags)
{
	/*
	{
		for (UINT unI = 0; unI < (UINT)m_apcActiveSwapChains.size(); unI++)
		{
			if (m_apcActiveSwapChains[unI]) m_apcActiveSwapChains[unI]->Release();
		}

		if (m_pcActiveDepthStencilSurface[0])
		{
			m_pcActiveDepthStencilSurface[0]->Release();
		}

		for (UINT unSampler = 0; unSampler < D3D9_SIMULTANEAOUS_TEXTURE_COUNT; unSampler++)
			if (m_apcActiveTexturesDisplacement[unSampler])
			{
				m_apcActiveTexturesDisplacement[unSampler]->Release();
				m_apcActiveTexturesDisplacement[unSampler] = nullptr;
			}

		for (UINT unSampler = D3DDMAPSAMPLER; unSampler <= D3DVERTEXTEXTURESAMPLER3; unSampler++)
			if (m_apcActiveTextures[unSampler])
			{
				m_apcActiveTextures[unSampler]->Release();
				m_apcActiveTextures[unSampler] = nullptr;
			}

		for (UINT unIndex = 0; unIndex < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; unIndex++)
			if (m_apcActiveRenderTargets[unIndex] != NULL)
			{
				m_apcActiveRenderTargets[unIndex]->Release();
				m_apcActiveRenderTargets[unIndex] = NULL;
			}

		auto itVB = m_apcActiveVertexBuffers.begin();
		while (itVB != m_apcActiveVertexBuffers.end())
		{
			if (itVB->second)
				itVB->second->Release();

			itVB = m_apcActiveVertexBuffers.erase(itVB);
		}

		m_bPresent = false;
	}
	*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::Present_v3(int& nFlags)
{
	/*
	// ensure to be on left drawing side
	if (m_eCurrentRenderingSide == RenderPosition::Right)
		SetDrawingSide(pcDevice, RenderPosition::Left);

	// initialize ?
	if (!m_bPresent)
	{

	}

	// set present() bool to true
	m_bPresent = true;

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

					{
						// query for the IDirect3DDevice9Ex interface
						static IDirect3DDevice9Ex* s_pcDirect3DDevice9Ex = NULL;
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

						if ((!m_pcStereoBuffer[0]) || (!m_pcStereoBuffer[1])) OutputDebugString(L"[STS] Failed to create texture default/dynamic.");
					}

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

					// monitor stereo output ?
					if (m_bMonitorStereo)
					{
						D3DSURFACE_DESC sDesc = {};
						if (m_pcActiveBackBufferSurface[0]) m_pcActiveBackBufferSurface[0]->GetDesc(&sDesc);
						RECT sRect = { 0, 0, (LONG)sDesc.Width >> 1, (LONG)sDesc.Height };
						pcDevice->StretchRect(m_pcStereoBufferSurface[0], NULL, m_pcActiveBackBufferSurface[0], &sRect, D3DTEXF_NONE);
					}

					m_pcStereoBufferSurface[0]->Release();
				}
				if (m_pcStereoBufferSurface[1])
				{
					pcDevice->StretchRect(m_pcActiveBackBufferSurface[1], NULL, m_pcStereoBufferSurface[1], NULL, D3DTEXF_NONE);

					// monitor stereo output ?
					if (m_bMonitorStereo)
					{
						D3DSURFACE_DESC sDesc = {};
						if (m_pcActiveBackBufferSurface[0]) m_pcActiveBackBufferSurface[0]->GetDesc(&sDesc);
						RECT sRect = { (LONG)sDesc.Width >> 1, 0, (LONG)sDesc.Width, (LONG)sDesc.Height };
						pcDevice->StretchRect(m_pcStereoBufferSurface[1], NULL, m_pcActiveBackBufferSurface[0], &sRect, D3DTEXF_NONE);
					}

					m_pcStereoBufferSurface[1]->Release();
				}
			}
		}
	}
	else
		OutputDebugString(L"Vireio Stereo Splitter : No primary swap chain found.");
		*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetBackBuffer_v3(int& nFlags)
{
	/*{
		if (!m_punISwapChain) return nullptr;
		if (!m_punIBackBuffer) return nullptr;
		if (!m_peType) return nullptr;
		if (!m_pppcBackBuffer) return nullptr;

		if (!m_bPresent)
			Present((IDirect3DDevice9*)pThis, true);

		try
		{
			// ref count increase happens in the swapchain GetBackBuffer so we don't add another ref here as we are just passing the value through
			nHr = m_apcActiveSwapChains.at(*m_punISwapChain)->GetBackBuffer(*m_punIBackBuffer, *m_peType, *m_pppcBackBuffer);
		}
		catch (std::out_of_range)
		{
			OutputDebugString(L"[STS]GetBackBuffer: out of range getting swap chain");
			nHr = D3DERR_INVALIDCALL;
		}

		// method replaced, immediately return
		nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
		return (void*)&nHr;
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateTexture_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
												   }*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateVolumeTexture_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
												   }*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateCubeTexture_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
												   }*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateVertexBuffer_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateIndexBuffer_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateRenderTarget_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
		//eMultiSample = D3DMULTISAMPLE_NONE;
		//eMultiSampleQuality = 0;
	}

#ifdef _DEBUGTHIS
													   OutputDebugString(L"[STS] Create render target format :");
													   DEBUG_UINT(eFormat);
													   DEBUG_HEX(eFormat);
#endif

													   if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->CreateRenderTarget(*m_punWidth, *m_punHeight, eFormat, eMultiSample, eMultiSampleQuality, *m_pnLockable, &s_pcLeftRenderTarget, *m_ppvSharedHandle)))
													   {
														   // "If Needed" heuristic is the complicated part here.
														   // Fixed heuristics (based on type, format, size, etc) + game specific overrides + isForcedMono + magic?

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
												   }*/

	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateDepthStencilSurface_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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

				// Override multisampling if DX9Ex... need that ??
				D3DMULTISAMPLE_TYPE eMultiSample = *m_peMultiSample;
				DWORD eMultiSampleQuality = *m_punMultisampleQuality;
				D3DFORMAT eFormat = *m_peFormat;

				static IDirect3DDevice9Ex *s_pcDirect3DDevice9Ex = NULL;
				s_pcDirect3DDevice9Ex = NULL;
				if (SUCCEEDED(((IDirect3DDevice9*)pThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&s_pcDirect3DDevice9Ex))))
				{
					//eMultiSample = D3DMULTISAMPLE_NONE;
					//eMultiSampleQuality = 0;

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
												   }*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::UpdateSurface_v3(int& nFlags)
{
	/*{
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
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::UpdateTexture_v3(int& nFlags)
{
	/*
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
	*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetRenderTargetData_v3(int& nFlags)
{
	/*{
		if (!m_ppcRenderTargetGetData) return nullptr;
		if (!m_ppcDestSurface) return nullptr;
		{
			// get actual surfaces and do the method call for the left surfaces
			IDirect3DSurface9* pSourceSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcRenderTargetGetData))->GetActualLeft();
			IDirect3DSurface9* pSourceSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcRenderTargetGetData))->GetActualRight();
			IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualLeft();
			IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualRight();
			nHr = ((IDirect3DDevice9*)pThis)->GetRenderTargetData(pSourceSurfaceLeft, pDestSurfaceLeft);

			// no proxy surface ?
			if ((!pSourceSurfaceLeft) || (!pDestSurfaceLeft))
			{
				OutputDebugString(L"[STS] : Fatal Error - No Proxy Surface provided.");
				DEBUG_HEX(*m_ppcDestSurface);
				exit(99);
			}

			if (SUCCEEDED(nHr))
			{
				if (!pSourceSurfaceRight && pDestSurfaceRight)
				{
					// Source is not stereo, destination is stereo. Copying source to both sides of destination.
					if (FAILED(((IDirect3DDevice9*)pThis)->GetRenderTargetData(pSourceSurfaceLeft, pDestSurfaceRight)))
					{
						OutputDebugString(L"[STS] ERROR: GetRenderTargetData - Failed to copy source left to destination right.\n");
					}
				}
				else if (pSourceSurfaceRight && !pDestSurfaceRight)
				{
					// Source is stereo, destination is not stereo. Copied Left side only.
				}
				else if (pSourceSurfaceRight && pDestSurfaceRight)
				{
					if (FAILED(((IDirect3DDevice9*)pThis)->GetRenderTargetData(pSourceSurfaceRight, pDestSurfaceRight)))
					{
						OutputDebugString(L"[STS] ERROR: GetRenderTargetData - Failed to copy source right to destination right.\n");
					}
				}
			}

			// method replaced, immediately return
			nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
			return (void*)&nHr;
		}
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetFrontBufferData_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_punISwapChain) return nullptr;
				if (!m_ppcDestSurface) return nullptr;
				{
					// get actual surfaces and do the method call
					IDirect3DSurface9* pDestSurfaceLeft = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualLeft();
					IDirect3DSurface9* pDestSurfaceRight = (static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface))->GetActualRight();
					nHr = ((IDirect3DDevice9*)pThis)->GetFrontBufferData(*m_punISwapChain, pDestSurfaceLeft);

					// no proxy surface ?
					if (!pDestSurfaceLeft)
					{
						OutputDebugString(L"[STS] : Fatal Error - No Proxy Surface provided.");
						DEBUG_HEX(*m_ppcDestSurface);
						exit(99);
					}

					if (SUCCEEDED(nHr))
					{
						if (pDestSurfaceRight)
						{
							// do the call for the right side
							((IDirect3DDevice9*)pThis)->GetFrontBufferData(*m_punISwapChain, pDestSurfaceRight);
						}
					}

					// method replaced, immediately return
					nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
					return (void*)&nHr;
				}
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::StretchRect_v3(int& nFlags)
{
	/*

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


	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::ColorFill_v3(int& nFlags)
{
	/*{
		IDirect3DStereoSurface9* pDerivedSurface = static_cast<IDirect3DStereoSurface9*>(*m_ppcSurface);
		if (SUCCEEDED(nHr = ((IDirect3DDevice9*)pThis)->ColorFill(pDerivedSurface->GetActualLeft(), *m_ppsRect, *m_punColor)))
		{
			if (pDerivedSurface->IsStereo())
				((IDirect3DDevice9*)pThis)->ColorFill(pDerivedSurface->GetActualRight(), *m_ppsRect, *m_punColor);
		}

		// method replaced, immediately return
		nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
		return (void*)&nHr;
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateOffscreenPlainSurface_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
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
												   }*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::SetRenderTarget_v3(int& nFlags)
{
	/*{
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
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::GetRenderTarget_v3(int& nFlags)
{
	/*if (!m_punRenderTargetIndex) return nullptr;
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
	exit(99);*/

	return E_FAIL;
}

/// <summary>Version 3 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::SetDepthStencilSurface_v3(int& nFlags)
{
	/*{
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
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetDepthStencilSurface_v3(int& nFlags)
{
	/*
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
	}*/

	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetViewport_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetRenderState_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetTexture_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 3 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::SetTexture_v3(int& nFlags)
{
	/*{
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
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::ProcessVertices_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_punSrcStartIndex) return nullptr; // ->ProcessVertices()
				if (!m_punDestIndex) return nullptr; // ->ProcessVertices()
				if (!m_punVertexCount) return nullptr; // ->ProcessVertices()
				if (!m_ppcDestBuffer) return nullptr; // ->ProcessVertices()
				if (!m_ppcVertexDecl) return nullptr; // ->ProcessVertices()
				if (!m_punFlags) return nullptr; // ->ProcessVertices()

				IDirect3DProxyVertexBuffer9* pCastDestBuffer = static_cast<IDirect3DProxyVertexBuffer9*>(*m_ppcDestBuffer);
				nHr = ((IDirect3DDevice9*)pThis)->ProcessVertices(*m_punSrcStartIndex, *m_punDestIndex, *m_punVertexCount, pCastDestBuffer->GetActual(), *m_ppcVertexDecl, *m_punFlags);

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return (void*)&nHr;
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetStreamSource_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_punStreamNumber) return nullptr; // ->SetStreamSource(), ->GetStreamSource()
				if (!m_ppcStreamData) return nullptr; // ->SetStreamSource(),
				if (!m_punOffsetInBytes) return nullptr; // ->SetStreamSource()
				if (!m_punStride) return nullptr; // ->SetStreamSource()
				if (!(*m_ppcStreamData)) return nullptr;

				// cast proxy
				IDirect3DProxyVertexBuffer9* pNewBuffer = static_cast<IDirect3DProxyVertexBuffer9*>(*m_ppcStreamData);

				// set actual buffer
				nHr = ((IDirect3DDevice9*)pThis)->SetStreamSource(*m_punStreamNumber, pNewBuffer->GetActual(), *m_punOffsetInBytes, *m_punStride);

				// remove existing vertex buffer that was active at StreamNumber if there is one
				if (m_apcActiveVertexBuffers.count(*m_punStreamNumber) == 1)
				{

					IDirect3DVertexBuffer9* pOldBuffer = m_apcActiveVertexBuffers.at(*m_punStreamNumber);
					if (pOldBuffer == *m_ppcStreamData)
					{
						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						return (void*)&nHr;
					}

					if (pOldBuffer)
						pOldBuffer->Release();

					m_apcActiveVertexBuffers.erase(*m_punStreamNumber);
				}

				// insert new vertex buffer
				if (m_apcActiveVertexBuffers.insert(std::pair<UINT, IDirect3DProxyVertexBuffer9*>(*m_punStreamNumber, pNewBuffer)).second)
				{
					// success
					if (*m_ppcStreamData)
						(*m_ppcStreamData)->AddRef();
				}
				else
				{
					OutputDebugStringA("[STS] Unable to store active Texture Stage.\n");
					assert(false);

					// If we get here the state of the texture tracking is fubared and an implosion is imminent.
					nHr = D3DERR_INVALIDCALL;
				}

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
				return (void*)&nHr;
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetStreamSource_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
	{
		if (!m_punStreamNumber) return nullptr;
		if (!m_pppcStreamData) return nullptr;
		if (!m_ppunOffsetInBytes) return nullptr;
		if (!m_ppunStride) return nullptr;

		// This whole methods implementation is highly questionable. Not sure exactly how GetStreamSource works
		nHr = D3DERR_INVALIDCALL;

		if (m_apcActiveVertexBuffers.count(*m_punStreamNumber) == 1)
		{
			*(*m_pppcStreamData) = m_apcActiveVertexBuffers[StreamNumber];
			if ((*(*m_pppcStreamData)))
				(*(*m_pppcStreamData))->AddRef();

			nHr = D3D_OK;
		}

		// method replaced, immediately return
		nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
		return (void*)&nHr;
	}*/

	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetIndices_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetIndices_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::DrawRectPatch_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::DrawTriPatch_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_Present_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_GetFrontBufferData_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
	{
		SHOW_CALL("IDirect3DStereoSwapChain9::GetFrontBufferData");

		if (!m_ppcDestSurface) return nullptr;

		IDirect3DStereoSurface9* pcWrappedDestSurface = static_cast<IDirect3DStereoSurface9*>(*m_ppcDestSurface);

		if (!pcWrappedDestSurface)
		{
			nHr = ((LPDIRECT3DSWAPCHAIN9)pThis)->GetFrontBufferData(NULL);
		}
		else
		{
			nHr = ((LPDIRECT3DSWAPCHAIN9)pThis)->GetFrontBufferData(pcWrappedDestSurface->GetActualLeft());

			if (SUCCEEDED(nHr) && pcWrappedDestSurface->GetActualRight())
			{

				if (FAILED(((LPDIRECT3DSWAPCHAIN9)pThis)->GetFrontBufferData(pcWrappedDestSurface->GetActualRight())))
				{
					OutputDebugString(L"[STS] IDirect3DStereoSwapChain9::GetFrontBufferData; right problem - left ok\n");
				}
			}
		}

		// TODO Might be able to use a frame delayed backbuffer (copy last back buffer?) to get proper left/right images. Much pondering required, and some testing
		OutputDebugString(L"[STS] IDirect3DStereoSwapChain9::GetFrontBufferData; Caution Will Robinson. The result of this method at the moment is wrapped surfaces containing what the user would see on a monitor. Example: A side-by-side warped image for the rift in the left and right surfaces of the output surface.\n");

		// method replaced, immediately return
		nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
		return (void*)&nHr;
	}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_GetBackBuffer_v3(int& nFlags)
{
	/*if (m_bUseD3D9Ex)
			{
				if (!m_punIBackBuffer) return nullptr;
				if (!m_peType) return nullptr;
				if (!m_pppcBackBuffer) return nullptr;

				OutputDebugString(L"[STS] FATAL : IDirect3DSwapChain->GetBackBuffer() should be handled by proxy class !!");
				if (m_apcActiveSwapChains.size()) DEBUG_HEX(((IDirect3DStereoSwapChain9*)m_apcActiveSwapChains[0])->GetActual());
				DEBUG_HEX(pThis);
				exit(99);
			}*/
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SB_Apply_v3(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
#pragma endregion

#pragma region /// => Version 4
/// <summary> 
/// Init v4 internal fields.
/// </summary>
void StereoSplitter::Init_v4()
{
	// get active render targets
	for (DWORD unI = 0; unI < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; unI++)
	{
		// get render target index
		IDirect3DSurface9* pcTarget = nullptr;
		m_pcDeviceCurrent->GetRenderTarget(unI, &pcTarget);

		// set active and release
		if (pcTarget)
		{
			m_apcActiveRenderTargets[unI] = pcTarget;
			m_apcActiveRenderTargets[D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT + unI] = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, pcTarget);
			pcTarget->Release();
		}
	}

	// set number of render targets
	m_unRenderTargetNumber = 0;
	for (std::vector<IDirect3DSurface9*>::size_type i = 0; i < D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		if (m_apcActiveRenderTargets[i] != NULL) m_unRenderTargetNumber = (DWORD)i + 1;

	// get depth stencil
	IDirect3DSurface9* pcDepthStencil = nullptr;
	m_pcDeviceCurrent->GetDepthStencilSurface(&pcDepthStencil);

	// set active and release
	if (pcDepthStencil)
	{
		m_pcActiveDepthStencilSurface[0] = pcDepthStencil;
		m_pcActiveDepthStencilSurface[1] = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, pcDepthStencil);
		pcDepthStencil->Release();
	}

	// get viewport
	m_pcDeviceCurrent->GetViewport(&m_sLastViewportSet);
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateAdditionalSwapchain_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetSwapChain_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::Reset_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary> 
/// => Version 4 Present() 
/// </summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::Present_v4(int& nFlags)
{
	// ensure to be on left drawing side
	if (m_eCurrentRenderingSide == RenderPosition::Right)
		SetDrawingSide(m_pcDeviceCurrent, RenderPosition::Left);

	// initialize ?
	if (!m_bPresent)
	{
		// init and set present() bool to true
		Init_v4();
		m_bPresent = true;
	}

	// finally, provide pointers to the left and right render target - get back buffer... 
	IDirect3DSurface9* pcBuffer = nullptr;
	m_pcDeviceCurrent->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pcBuffer);
	m_pcActiveBackBufferSurface[0] = pcBuffer;
	SAFE_RELEASE(pcBuffer);

	if (m_pcActiveBackBufferSurface[0])
	{
		// set twin by verifying
		if (!m_pcActiveBackBufferSurface[1])
			m_pcActiveBackBufferSurface[1] = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, m_pcActiveBackBufferSurface[0]);

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
					// create output textures
					if (!m_pcStereoBuffer[0])
						m_pcDeviceCurrent->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[0], NULL);
					if (!m_pcStereoBuffer[1])
						m_pcDeviceCurrent->CreateTexture(sDesc.Width, sDesc.Height, 0, D3DUSAGE_RENDERTARGET, sDesc.Format, D3DPOOL_DEFAULT, &m_pcStereoBuffer[1], NULL);

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
					m_pcDeviceCurrent->StretchRect(m_pcActiveBackBufferSurface[0], NULL, m_pcStereoBufferSurface[0], NULL, D3DTEXF_NONE);

					// monitor stereo output ?
					if (m_bMonitorStereo)
					{
						D3DSURFACE_DESC sDesc = {};
						if (m_pcActiveBackBufferSurface[0]) m_pcActiveBackBufferSurface[0]->GetDesc(&sDesc);
						RECT sRect = { 0, 0, (LONG)sDesc.Width >> 1, (LONG)sDesc.Height };
						m_pcDeviceCurrent->StretchRect(m_pcStereoBufferSurface[0], NULL, m_pcActiveBackBufferSurface[0], &sRect, D3DTEXF_NONE);
					}

					m_pcStereoBufferSurface[0]->Release();
				}
				if (m_pcStereoBufferSurface[1])
				{
					m_pcDeviceCurrent->StretchRect(m_pcActiveBackBufferSurface[1], NULL, m_pcStereoBufferSurface[1], NULL, D3DTEXF_NONE);

					// monitor stereo output ?
					if (m_bMonitorStereo)
					{
						D3DSURFACE_DESC sDesc = {};
						if (m_pcActiveBackBufferSurface[0]) m_pcActiveBackBufferSurface[0]->GetDesc(&sDesc);
						RECT sRect = { (LONG)sDesc.Width >> 1, 0, (LONG)sDesc.Width, (LONG)sDesc.Height };
						m_pcDeviceCurrent->StretchRect(m_pcStereoBufferSurface[1], NULL, m_pcActiveBackBufferSurface[0], &sRect, D3DTEXF_NONE);
					}

					m_pcStereoBufferSurface[1]->Release();
				}
			}
		}
	}
	else
		OutputDebugString(L"Vireio Stereo Splitter : No primary swap chain found.");

	return S_OK;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetBackBuffer_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateTexture_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateVolumeTexture_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateCubeTexture_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateVertexBuffer_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateIndexBuffer_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateRenderTarget_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateDepthStencilSurface_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::UpdateSurface_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::UpdateSurface]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::UpdateSurface]);

	IDirect3DSurface9** ppcSourceSurface;
	RECT** ppsSourceRect;
	IDirect3DSurface9** ppcDestinationSurface;
	POINT** ppsDestPoint;
	if (ppIn[0]) ppcSourceSurface = *(IDirect3DSurface9***)ppIn[0]; else return E_FAIL;
	if (ppIn[1]) ppsSourceRect = *(RECT***)ppIn[1]; else return E_FAIL;
	if (ppIn[2]) ppcDestinationSurface = *(IDirect3DSurface9***)ppIn[2]; else return E_FAIL;
	if (ppIn[3]) ppsDestPoint = *(POINT***)ppIn[3]; else return E_FAIL;

	// get twins
	IDirect3DSurface9* pcSurfaceDst = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcDestinationSurface);
	IDirect3DSurface9* pcSurfaceSrc = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcSourceSurface);

	if (pcSurfaceDst)
	{
		if (pcSurfaceSrc)
		{
			// Source is stereo, destination is stereo. Copying both.
			m_pcDeviceCurrent->UpdateSurface(pcSurfaceSrc, *ppsSourceRect, pcSurfaceDst, *ppsDestPoint);
		}
		else
		{
			// Source is not stereo, destination is stereo. Copying source to both sides of destination
			m_pcDeviceCurrent->UpdateSurface(*ppcSourceSurface, *ppsSourceRect, pcSurfaceDst, *ppsDestPoint);
		}
	}
	// else Source is maybe stereo, destination is not stereo. Copied left side only.

	return S_OK;
}

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::UpdateTexture_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::UpdateTexture]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::UpdateTexture]);

	IDirect3DBaseTexture9** ppcSourceTexture;
	IDirect3DBaseTexture9** ppcDestinationTexture;
	if (ppIn[0]) ppcSourceTexture = *(IDirect3DBaseTexture9***)ppIn[0]; else return E_FAIL;
	if (ppIn[1]) ppcDestinationTexture = *(IDirect3DBaseTexture9***)ppIn[1]; else return E_FAIL;

	// get twins
	IDirect3DBaseTexture9* pcTextureSrc = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcSourceTexture, false);
	IDirect3DBaseTexture9* pcTextureDst = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcDestinationTexture, (pcTextureSrc != nullptr));

	if (pcTextureDst)
	{
		if (pcTextureSrc)
		{
			// Source is stereo, destination is stereo. Copying both.
			m_pcDeviceCurrent->UpdateTexture(pcTextureSrc, pcTextureDst);
		}
		else
		{
			// Source is not stereo, destination is stereo. Copying source to both sides of destination
			m_pcDeviceCurrent->UpdateTexture(*ppcSourceTexture, pcTextureDst);
		}
	}
	// else Source is maybe stereo, destination is not stereo. Copied left side only.

	return S_OK;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetRenderTargetData_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetFrontBufferData_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::StretchRect_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::StretchRect]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::StretchRect]);

	IDirect3DSurface9** ppcSourceSurface;
	RECT** ppsSourceRect;
	IDirect3DSurface9** ppcDestSurface;
	RECT** ppsDestRect;
	D3DTEXTUREFILTERTYPE* peFilter;

	if (ppIn[0]) ppcSourceSurface = *(IDirect3DSurface9***)ppIn[0]; else return E_FAIL;
	if (ppIn[1]) ppsSourceRect = *(RECT***)ppIn[1]; else return E_FAIL;
	if (ppIn[2]) ppcDestSurface = *(IDirect3DSurface9***)ppIn[2]; else return E_FAIL;
	if (ppIn[3]) ppsDestRect = *(RECT***)ppIn[3]; else return E_FAIL;
	if (ppIn[4]) peFilter = *(D3DTEXTUREFILTERTYPE**)ppIn[4]; else return E_FAIL;

	// get twins
	IDirect3DSurface9* pcSurfaceDst = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcDestSurface);
	IDirect3DSurface9* pcSurfaceSrc = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcSourceSurface);

	if (pcSurfaceDst)
	{
		if (pcSurfaceSrc)
		{
			// Source is stereo, destination is stereo. Copying both.
			m_pcDeviceCurrent->StretchRect(pcSurfaceSrc, *ppsSourceRect, pcSurfaceDst, *ppsDestRect, *peFilter);
		}
		else
		{
			// Source is not stereo, destination is stereo. Copying source to both sides of destination
			m_pcDeviceCurrent->StretchRect(*ppcSourceSurface, *ppsSourceRect, pcSurfaceDst, *ppsDestRect, *peFilter);
		}
	}
	// else Source is maybe stereo, destination is not stereo. Copied left side only.

	return S_OK;
}

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::ColorFill_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::ColorFill]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::ColorFill]);

	IDirect3DSurface9** ppcSurface;
	RECT** ppsRect;
	D3DCOLOR* puColor;

	if (ppIn[0]) ppcSurface = *(IDirect3DSurface9***)ppIn[0]; else return E_FAIL;
	if (ppIn[1]) ppsRect = *(RECT***)ppIn[1]; else return E_FAIL;
	if (ppIn[2]) puColor = *(D3DCOLOR**)ppIn[2]; else return E_FAIL;

	// get twin
	IDirect3DSurface9* pcSurfaceTwin = VerifyPrivateDataInterfaces(m_pcDeviceCurrent, *ppcSurface);

	if (pcSurfaceTwin)
	{
		// call method for twin surface
		m_pcDeviceCurrent->ColorFill(pcSurfaceTwin, *ppsRect, *puColor);
	}

	return S_OK;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::CreateOffscreenPlainSurface_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::SetRenderTarget_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetRenderTarget]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetRenderTarget]);

	DWORD* punRenderTargetIndex;
	IDirect3DSurface9** ppcRenderTarget;
	if (ppIn[0])
		punRenderTargetIndex = *(DWORD**)ppIn[0];
	else return E_FAIL; // throw ??
	if (ppIn[1])
		ppcRenderTarget = *(IDirect3DSurface9***)ppIn[1];
	else return E_FAIL;

	HRESULT nHr = S_OK;
	if (m_bPresent)
	{
		// call method
		SetRenderTarget(m_pcDeviceCurrent, *punRenderTargetIndex, *ppcRenderTarget);

		// index too high ?
		if ((*punRenderTargetIndex) >= D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT) return E_INVALIDARG;

		// replace the call
		if (m_eCurrentRenderingSide == RenderPosition::Left)
		{
			nHr = (m_pcDeviceCurrent)->SetRenderTarget(*punRenderTargetIndex, m_apcActiveRenderTargets[*punRenderTargetIndex]);
		}
		else
		{
			nHr = (m_pcDeviceCurrent)->SetRenderTarget(*punRenderTargetIndex, m_apcActiveRenderTargets[*punRenderTargetIndex + D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT]);
		}

		// changing rendertarget resets viewport to fullsurface
		if (nHr == D3D_OK) m_bActiveViewportIsDefault = true;

		// method replaced, immediately return
		nFlags = AQU_PluginFlags::ImmediateReturnFlag;
	}
	return nHr;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetRenderTarget_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 4 method call</summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT StereoSplitter::SetDepthStencilSurface_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetDepthStencilSurface]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetDepthStencilSurface]);

	IDirect3DSurface9** ppcNewZStencil;
	if (ppIn[0])
		ppcNewZStencil = *(IDirect3DSurface9***)ppIn[0];
	else return E_FAIL;

	HRESULT nHr = S_OK;
	if (m_bPresent)
	{
		// call method
		SetDepthStencilSurface(m_pcDeviceCurrent, *ppcNewZStencil);

		// replace the call
		if (m_eCurrentRenderingSide == RenderPosition::Left)
		{
			nHr = m_pcDeviceCurrent->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[0]);
		}
		else
		{
			nHr = m_pcDeviceCurrent->SetDepthStencilSurface(m_pcActiveDepthStencilSurface[1]);
		}

		// method replaced, immediately return
		nFlags = AQU_PluginFlags::ImmediateReturnFlag;
	}
	return nHr;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetDepthStencilSurface_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetViewport_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetRenderState_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetTexture_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetTexture_v4(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetTexture]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetTexture]);

	DWORD* punSampler;
	IDirect3DBaseTexture9** ppcTexture;
	if (ppIn[0])
		punSampler = *(DWORD**)ppIn[0];
	else return E_FAIL; // throw ??
	if (ppIn[1])
		ppcTexture = *(IDirect3DBaseTexture9***)ppIn[1];
	else return E_FAIL;

	HRESULT nHr = S_OK;
	if (m_bPresent)
	{
		// TODO !! DISPLACEMENT MAPPING !!

		// call method
		SetTexture(m_pcDeviceCurrent, *punSampler, *ppcTexture);

		// index too high ?
		if ((*punSampler) >= D3D9_SIMULTANEAOUS_TEXTURE_COUNT) return E_INVALIDARG;

		// replace the call
		if ((m_eCurrentRenderingSide == RenderPosition::Left) || (!(m_apcActiveTextures[*punSampler + D3D9_SIMULTANEAOUS_TEXTURE_COUNT])))
		{
			nHr = m_pcDeviceCurrent->SetTexture(*punSampler, m_apcActiveTextures[*punSampler]);
		}
		else
		{
			nHr = m_pcDeviceCurrent->SetTexture(*punSampler, m_apcActiveTextures[*punSampler + D3D9_SIMULTANEAOUS_TEXTURE_COUNT]);
		}

		// method replaced, immediately return
		nFlags = AQU_PluginFlags::ImmediateReturnFlag;
	}
	return nHr;
}

/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::ProcessVertices_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetStreamSource_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetStreamSource_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SetIndices_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::GetIndices_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::DrawRectPatch_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::DrawTriPatch_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_Present_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_GetFrontBufferData_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SC_GetBackBuffer_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT StereoSplitter::SB_Apply_v4(int& nFlags) { return ERROR_CALL_NOT_IMPLEMENTED; }
#pragma endregion
