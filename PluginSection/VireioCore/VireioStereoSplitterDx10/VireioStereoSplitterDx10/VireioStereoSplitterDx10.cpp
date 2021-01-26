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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }
#define CASE_ENUM_2_WSTRING(ec, en) case ec::en: return L#en;

#include"VireioStereoSplitterDx10.h"

#define TODO_ADD_BOOL_HERE true

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

/// <summary>
/// Constructor.
/// </summary>
StereoSplitter::StereoSplitter(ImGuiContext* sCtx) :AQU_Nodus(sCtx),
m_apcActiveRenderTargetViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2, nullptr),
m_apcActivePSShaderResourceViews(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2, nullptr),
m_apcActiveCSShaderResourceViews(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2, nullptr),
m_apcActiveCSConstantBuffers(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2, nullptr),
m_apcActiveUnorderedAccessViews(D3D11_1_UAV_SLOT_COUNT * 2, nullptr),
m_pcActiveStereoTwinDepthStencilView(nullptr),
m_pcActiveDepthStencilView(nullptr),
m_pcActiveBackBuffer10(nullptr),
m_pcActiveStereoTwinBackBuffer10(nullptr),
m_pcActiveStereoTwinBackBufferView10(nullptr),
m_hBitmapControl(nullptr),
m_bControlUpdate(false),
m_hFont(nullptr),
m_bPresent(false),
m_eBackBufferVerified(BackBufferVerificationState::NotVerified),
m_dwPSShaderResourceViewsNumber(0),
m_dwCSShaderResourceViewsNumber(0),
m_dwRenderTargetNumber(0),
m_eD3DVersion(D3DVersion::NotDefined),
m_eCurrentRenderingSide(RenderPosition::Left),
m_dwVerifyConstantBuffers(0),
m_bRenderTargetWasSwitched(false),
m_pcHUDRenderTargetView(nullptr),
m_bClearHUDRenderTarget(false)
{
	m_pcTexView10[0] = nullptr;
	m_pcTexView10[1] = nullptr;
	m_pcTex10[0] = nullptr;
	m_pcTex10[1] = nullptr;

	// fill the "minus one" array for setting the unordered access views
	memset(m_aunMinusOne, -1, sizeof(m_aunMinusOne));

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Stereo Splitter";
	{
		static float fDummy = 0.0f;
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "NOT IMPLEMENTED NOW !!";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 1.0f;
		sEntry.fMaximum = 30.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &fDummy;
		sEntry.fValue = fDummy;
		m_sMenu.asEntries.push_back(sEntry);
	}
}

/// <summary>
/// Destructor.
/// </summary>
StereoSplitter::~StereoSplitter()
{
	SAFE_RELEASE(m_pcHUDRenderTargetView);
	SAFE_RELEASE(m_pcTexView10[0]);
	SAFE_RELEASE(m_pcTexView10[1]);
	SAFE_RELEASE(m_pcTex10[0]);
	SAFE_RELEASE(m_pcTex10[1]);
}

/// <summary>
/// Return the name of the  Stereo Splitter node.
/// </summary>
const char* StereoSplitter::GetNodeType()
{
	return "Stereo Splitter Dx10+";
}

/// <summary>
/// Returns a global unique identifier for the Stereo Splitter node.
/// </summary>
UINT StereoSplitter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 65
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/// <summary>
/// Returns the name of the category for the Stereo Splitter node.
/// </summary>
LPWSTR StereoSplitter::GetCategory()
{
	return L"Vireio Core";
}

/// <summary>
/// Returns a logo to be used for the Stereo Splitter node.
/// </summary>
HBITMAP StereoSplitter::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioStereoSplitterDx10.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/// <summary>
/// Returns the updated control for the Stereo Splitter node.
/// Allways return >nullptr< if there is no update for the control !!
/// </summary>
HBITMAP StereoSplitter::GetControl()
{
	return nullptr;
}

/// <summary>
/// Provides the name of the requested commander.
/// </summary>
LPWSTR StereoSplitter::GetCommanderName(DWORD uCommanderIndex)
{
	switch ((STS_Commanders)uCommanderIndex)
	{
	case STS_Commanders::StereoData_Splitter:
		return VLink::Name(VLink::_L::StereoData);
	default:
		break;
	}

	return L"";
}

/// <summary>
/// Provides the name of the requested decommander.
/// </summary>
LPWSTR StereoSplitter::GetDecommanderName(DWORD uDecommanderIndex)
{
	switch ((STS_Decommanders)uDecommanderIndex)
	{
	case STS_Decommanders::Modifier:
		return VLink::Name(VLink::_L::ModifierData);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CSSetSamplers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CSSetShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CSSetShaderResources);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CSSetUnorderedAccessViews);
		CASE_ENUM_2_WSTRING(STS_Decommanders, ClearDepthStencilView);
		CASE_ENUM_2_WSTRING(STS_Decommanders, ClearRenderTargetView);
		CASE_ENUM_2_WSTRING(STS_Decommanders, ClearState);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Dispatch);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DispatchIndirect);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Draw);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawAuto);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawIndexed);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawIndexedInstanced);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawIndexedInstancedIndirect);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawInstanced);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DrawInstancedIndirect);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Map);
		CASE_ENUM_2_WSTRING(STS_Decommanders, OMGetRenderTargets);
		CASE_ENUM_2_WSTRING(STS_Decommanders, OMGetRenderTargetsAndUnorderedAccessViews);
		CASE_ENUM_2_WSTRING(STS_Decommanders, OMSetRenderTargets);
		CASE_ENUM_2_WSTRING(STS_Decommanders, OMSetRenderTargetsAndUnorderedAccessViews);
		CASE_ENUM_2_WSTRING(STS_Decommanders, PSGetShaderResources);
		CASE_ENUM_2_WSTRING(STS_Decommanders, PSSetShaderResources);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Present);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Unmap);
		CASE_ENUM_2_WSTRING(STS_Decommanders, VSSetShader);
	default:
		break;
	}

	return L"";
}

/// <summary>
/// Returns the plug type for the requested commander.
/// </summary>
DWORD StereoSplitter::GetCommanderType(DWORD uCommanderIndex)
{
	switch ((STS_Commanders)uCommanderIndex)
	{
	case STS_Commanders::StereoData_Splitter:
		return VLink::Link(VLink::_L::StereoData);
	default:
		break;
	}

	return NOD_Plugtype::AQU_VOID;
}

/// <summary>
/// Returns the plug type for the requested decommander.
/// </summary>
DWORD StereoSplitter::GetDecommanderType(DWORD uDecommanderIndex)
{
	switch ((STS_Decommanders)uDecommanderIndex)
	{
	case STS_Decommanders::Modifier:
		return VLink::Link(VLink::_L::ModifierData);
	case STS_Decommanders::CSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers);
	case STS_Decommanders::CSSetSamplers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CSSetSamplers);
	case STS_Decommanders::CSSetShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CSSetShader);
	case STS_Decommanders::CSSetShaderResources:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CSSetShaderResources);
	case STS_Decommanders::CSSetUnorderedAccessViews:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews);
	case STS_Decommanders::ClearDepthStencilView:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView);
	case STS_Decommanders::ClearRenderTargetView:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView);
	case STS_Decommanders::ClearState:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::ClearState);
	case STS_Decommanders::Dispatch:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Dispatch);
	case STS_Decommanders::DispatchIndirect:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DispatchIndirect);
	case STS_Decommanders::Draw:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Draw);
	case STS_Decommanders::DrawAuto:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawAuto);
	case STS_Decommanders::DrawIndexed:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawIndexed);
	case STS_Decommanders::DrawIndexedInstanced:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced);
	case STS_Decommanders::DrawIndexedInstancedIndirect:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect);
	case STS_Decommanders::DrawInstanced:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawInstanced);
	case STS_Decommanders::DrawInstancedIndirect:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect);
	case STS_Decommanders::Map:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Map);
	case STS_Decommanders::OMGetRenderTargets:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets);
	case STS_Decommanders::OMGetRenderTargetsAndUnorderedAccessViews:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews);
	case STS_Decommanders::OMSetRenderTargets:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets);
	case STS_Decommanders::OMSetRenderTargetsAndUnorderedAccessViews:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews);
	case STS_Decommanders::PSGetShaderResources:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::PSGetShaderResources);
	case STS_Decommanders::PSSetShaderResources:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::PSSetShaderResources);
	case STS_Decommanders::Present:
		return NOD_Plugtype::WireCable((int)(int)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, (int)VMT_IDXGISWAPCHAIN::Present);
	case STS_Decommanders::Unmap:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Unmap);
	case STS_Decommanders::VSSetShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::VSSetShader);
	default:
		break;
	}

	return 0;
}

/// <summary>
/// Provides the output pointer for the requested commander.
/// </summary>
void* StereoSplitter::GetOutputPointer(DWORD uCommanderIndex)
{
	switch ((STS_Commanders)uCommanderIndex)
	{
	case STS_Commanders::StereoData_Splitter:
		return (void*)&m_sStereoData;
	default:
		break;
	}

	return nullptr;
}

/// <summary>
/// Sets the input pointer for the requested decommander.
/// </summary>
void StereoSplitter::SetInputPointer(DWORD uDecommanderIndex, void* pData)
{
	if (uDecommanderIndex == (DWORD)STS_Decommanders::Modifier)
		m_psModifierData = (ModifierData*)pData;
	if (uDecommanderIndex < NUMBER_OF_DECOMMANDERS)
		m_ppInput[uDecommanderIndex] = (pData);
}

/// <summary>
/// Stereo Splitter supports various D3D10 + D3D11 calls.
/// </summary>
bool StereoSplitter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_Direct3DVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_Direct3DVersion::DirectX_11_2))
	{
		/*if (nD3DInterface == (int)ITA_D3D11INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9))
		{
			if (nD3DMethod == METHOD_ID3D10DEVICE_OMSETRENDERTARGETS)
				return true;
		}
		else */
		if (nD3DInterface == (int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext)
		{
			if ((nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::PSSetShaderResources) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VSSetShader) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawIndexed) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::Draw) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawInstanced) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawAuto) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::PSGetShaderResources) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::Map) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::Unmap) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::ClearState) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::Dispatch) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::DispatchIndirect) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::CSSetShaderResources) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::CSSetShader) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::CSSetSamplers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers))
				return true;
		}
		else if (nD3DInterface == (int)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain)
		{
			if (nD3DMethod == (int)VMT_IDXGISWAPCHAIN::Present)
				return true;
		}
	}
	return false;
}

/// <summary>
/// Handle Stereo Render Targets (+Depth Buffers).
/// </summary>
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	// set node behavior to "double call" for this method
	// node that this is only supported by drawing methods
	if (m_bPresent)
		nProvokerIndex |= AQU_PluginFlags::DoubleCallFlag;

	// set current device (or swapchain)
	m_pcContextCurrent = (ID3D11DeviceContext*)pThis;
	int nFlags = 0;

	switch (eD3DInterface)
	{
#pragma region ID3D10DEVICE
		/*case INTERFACE_ID3D10DEVICE:
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
			return nullptr;*/
#pragma endregion
#pragma region ID3D11DEVICECONTEXT
	case (int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext:
		switch (eD3DMethod)
		{
#pragma region PSSETSHADERRESOURCES
		case (int)VMT_ID3D11DEVICECONTEXT::PSSetShaderResources:
			/*if (m_bPresent)
			{
				// ensure D3D11 is set
				m_eD3DVersion = D3DVersion::Direct3D11;

				// verify node pointers
				if (!m_pdwStartSlot) return nullptr;
				if (!m_pdwNumViewsSRVs) return nullptr;
				if (!m_pppcShaderResourceViews11) return nullptr;

				// invalid call ? validate !
				if (*m_pdwStartSlot + *m_pdwNumViewsSRVs > D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) return nullptr;

				XSSetShaderResourceViews(m_apcActivePSShaderResourceViews, m_dwPSShaderResourceViewsNumber, *m_pdwStartSlot, *m_pdwNumViewsSRVs, *m_pppcShaderResourceViews11);
			}
			else
			{
				// TODO !! catch active resources before first present call
				return nullptr;
			}

			// replace the call
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActivePSShaderResourceViews[*m_pdwStartSlot]);
			else
				((ID3D11DeviceContext*)pThis)->PSSetShaderResources(*m_pdwStartSlot, *m_pdwNumViewsSRVs, (ID3D11ShaderResourceView**)&m_apcActivePSShaderResourceViews[*m_pdwStartSlot + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]);

			// method replaced, immediately return
			nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;*/
			return nullptr;
#pragma endregion
#pragma region VSSETSHADER
		case (int)VMT_ID3D11DEVICECONTEXT::VSSetShader:
			/*if ((m_bRenderTargetWasSwitched) && (m_pbSwitchRenderTarget))
			{
				if (!(*m_pbSwitchRenderTarget))
				{
					// restore render targets
					if (m_eCurrentRenderingSide == RenderPosition::Left)
						((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
					else
						((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);
				}
			}*/
			return nullptr;
#pragma endregion
#pragma region DRAWINDEXED
		case (int)VMT_ID3D11DEVICECONTEXT::DrawIndexed:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAW
		case (int)VMT_ID3D11DEVICECONTEXT::Draw:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAWINDEXEDINSTANCED
		case (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAWINSTANCED
		case (int)VMT_ID3D11DEVICECONTEXT::DrawInstanced:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAWAUTO
		case (int)VMT_ID3D11DEVICECONTEXT::DrawAuto:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAWINDEXEDINSTANCEDINDIRECT
		case (int)VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region DRAWINSTANCEDINDIRECT
		case (int)VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect:
			// switch the drawing side before the second draw call is done
			if (m_eCurrentRenderingSide == RenderPosition::Left)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Right);
			else if (m_eCurrentRenderingSide == RenderPosition::Right)
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region CLEARRENDERTARGETVIEW
		case (int)VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView:
			/*if (m_bPresent)
			{
				// clear HUD ?
				if ((m_bClearHUDRenderTarget) && (m_pcHUDRenderTargetView))
				{
					// clear HUD render target view
					float afColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
					((ID3D11DeviceContext*)pThis)->ClearRenderTargetView((ID3D11RenderTargetView*)m_pcHUDRenderTargetView, afColor);
					m_bClearHUDRenderTarget = false;
				}

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
			}*/
			return nullptr;
#pragma endregion
#pragma region CLEARDEPTHSTENCILVIEW
		case (int)VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView:
			/*if (m_bPresent)
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
			}*/
			return nullptr;
#pragma endregion
#pragma region CLEARSTATE
		case (int)VMT_ID3D11DEVICECONTEXT::ClearState:
			if (m_bPresent)
			{
				// clear texture vectors and number
				m_apcActivePSShaderResourceViews = std::vector<ID3D11ShaderResourceView*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2, nullptr);
				m_dwPSShaderResourceViewsNumber = 0;

				// clear render targets
				OMSetRenderTargets((IUnknown*)pThis, 0, NULL, NULL);

				// set render target side to >left<
				SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			}
			return nullptr;
#pragma endregion
#pragma region OMSETRENDERTARGETS
		case (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets:
			/*// ensure D3D11 is set
			m_eD3DVersion = D3DVersion::Direct3D11;

			if (m_bPresent)
			{
				// verify node pointers
				if (!m_pdwNumViews) return nullptr;
				if (!m_pppcRenderTargetViews_DX11) return nullptr;
				if (!m_ppcDepthStencilView_DX11) return nullptr;

				// call intern method
				OMSetRenderTargets((IUnknown*)pThis, *m_pdwNumViews, *m_pppcRenderTargetViews_DX11, *m_ppcDepthStencilView_DX11);

				// copy internal render targets to backup for matrix modifier
				if (m_appcRenderTargetViews11)
				{
					ID3D11RenderTargetView** ppcRTV = (*m_appcRenderTargetViews11);
					if (ppcRTV)
					{
						CopyMemory(&ppcRTV[0], &m_apcActiveRenderTargetViews[0], sizeof(IUnknown*) * D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2);
					}
				}
				if (m_appcDepthStencilViews11)
				{
					ID3D11DepthStencilView** ppcDSV = (*m_appcDepthStencilViews11);
					if (ppcDSV)
					{
						ppcDSV[0] = (ID3D11DepthStencilView*)m_pcActiveDepthStencilView;
						ppcDSV[1] = (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView;
					}
				}

				// switch render targets ?
				if (m_pbSwitchRenderTarget)
				{
					if (*m_pbSwitchRenderTarget)
					{
						// did we catch the HUD render target view ?
						if (!m_pcHUDRenderTargetView)
						{
							((ID3D11DeviceContext*)pThis)->OMGetRenderTargets(1, &m_pcHUDRenderTargetView, nullptr);
						}

						m_bRenderTargetWasSwitched = true;

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						return nullptr;
					}
				}
				m_bRenderTargetWasSwitched = false;

				// switch render targets to new side
				if (m_eCurrentRenderingSide == RenderPosition::Left)
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
				else
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
			}*/
			return nullptr;
#pragma endregion
#pragma region OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS
		case (int)VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews:
			/*// ensure D3D11 is set
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

				// copy internal render targets to backup for matrix modifier
				if (m_appcRenderTargetViews11)
				{
					ID3D11RenderTargetView** ppcRTV = (*m_appcRenderTargetViews11);
					if (ppcRTV)
					{
						CopyMemory(&ppcRTV[0], &m_apcActiveRenderTargetViews[0], sizeof(IUnknown*) * D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2);
					}
				}
				if (m_appcDepthStencilViews11)
				{
					ID3D11DepthStencilView** ppcDSV = (*m_appcDepthStencilViews11);
					if (ppcDSV)
					{
						ppcDSV[0] = (ID3D11DepthStencilView*)m_pcActiveDepthStencilView;
						ppcDSV[1] = (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView;
					}
				}

				// switch render targets ?
				if (m_pbSwitchRenderTarget)
				{
					if (*m_pbSwitchRenderTarget)
					{
						m_bRenderTargetWasSwitched = true;

						// only set UAVs here
						((ID3D11DeviceContext*)pThis)->CSSetUnorderedAccessViews(*m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
						return nullptr;
					}
				}
				m_bRenderTargetWasSwitched = false;

				// switch render targets to new side
				if (m_eCurrentRenderingSide == RenderPosition::Left)
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargetsAndUnorderedAccessViews(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView, *m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);
				else
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargetsAndUnorderedAccessViews(*m_pdwNumViews, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView, *m_pdwUAVStartSlot, *m_pdwNumUAVs, *m_pppcUnorderedAccessViews, *m_ppdwUAVInitialCounts);

				// method replaced, immediately return
				nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
			}*/
			return nullptr;
#pragma endregion
#pragma region PSGETSHADERRESOURCES
		case (int)VMT_ID3D11DEVICECONTEXT::PSGetShaderResources:
			// if the app tries to get the shader resources ensure the left render side is set
			SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);
			return nullptr;
#pragma endregion
#pragma region OMGETRENDERTARGETS
		case (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets:
			/*
			if ((m_bRenderTargetWasSwitched) && (m_pbSwitchRenderTarget))
			{
				// restore render targets if we are on left side
				if (m_eCurrentRenderingSide == RenderPosition::Left)
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
			}

			// if the app tries to get the render targets ensure the left render side is set
			SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);*/
			return nullptr;
#pragma endregion
#pragma region OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS
		case (int)VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews:
			/*
			if ((m_bRenderTargetWasSwitched) && (m_pbSwitchRenderTarget))
			{
				// restore render targets if we are on left side
				if (m_eCurrentRenderingSide == RenderPosition::Left)
					((ID3D11DeviceContext*)pThis)->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
			}

			// if the app tries to get the render targets ensure the left render side is set
			SetDrawingSide((ID3D11DeviceContext*)pThis, RenderPosition::Left);*/
			return nullptr;
#pragma endregion
#pragma region MAP
		case (int)VMT_ID3D11DEVICECONTEXT::Map:
			return nullptr;
#pragma endregion
#pragma region UNMAP
		case (int)VMT_ID3D11DEVICECONTEXT::Unmap:
			/*if ((m_ppcResource_Unmap) && (m_pdwSubresource_Unmap))
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
			}*/
			return nullptr;
#pragma endregion
#pragma region DISPATCH
		case (int)VMT_ID3D11DEVICECONTEXT::Dispatch:
			/*if ((m_pdwThreadGroupCountX) && (m_pdwThreadGroupCountY) && (m_pdwThreadGroupCountZ))
			{
				// set the right side
				((ID3D11DeviceContext*)pThis)->CSSetShaderResources(0, m_dwCSShaderResourceViewsNumber, &m_apcActiveCSShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]);
				((ID3D11DeviceContext*)pThis)->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &m_apcActiveCSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
				((ID3D11DeviceContext*)pThis)->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, &m_apcActiveUnorderedAccessViews[D3D11_PS_CS_UAV_REGISTER_COUNT], m_aunMinusOne);

				// dispatch for the right side
				((ID3D11DeviceContext*)pThis)->Dispatch(*m_pdwThreadGroupCountX, *m_pdwThreadGroupCountY, *m_pdwThreadGroupCountZ);

				// set back to the left side
				((ID3D11DeviceContext*)pThis)->CSSetShaderResources(0, m_dwCSShaderResourceViewsNumber, &m_apcActiveCSShaderResourceViews[0]);
				((ID3D11DeviceContext*)pThis)->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &m_apcActiveCSConstantBuffers[0]);
				((ID3D11DeviceContext*)pThis)->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, &m_apcActiveUnorderedAccessViews[0], m_aunMinusOne);

			}*/
			return nullptr;
#pragma endregion
#pragma region DISPATCHINDIRECT
		case (int)VMT_ID3D11DEVICECONTEXT::DispatchIndirect:
			/*if ((m_ppcBufferForArgs) && (m_pdwpAlignedByteOffsetForArgs))
			{
				// set the right side
				((ID3D11DeviceContext*)pThis)->CSSetShaderResources(0, m_dwCSShaderResourceViewsNumber, &m_apcActiveCSShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]);
				((ID3D11DeviceContext*)pThis)->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &m_apcActiveCSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
				((ID3D11DeviceContext*)pThis)->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, &m_apcActiveUnorderedAccessViews[D3D11_PS_CS_UAV_REGISTER_COUNT], m_aunMinusOne);

				// dispatch for the right side
				((ID3D11DeviceContext*)pThis)->DispatchIndirect(*m_ppcBufferForArgs, *m_pdwpAlignedByteOffsetForArgs);

				// set back to the left side
				((ID3D11DeviceContext*)pThis)->CSSetShaderResources(0, m_dwCSShaderResourceViewsNumber, &m_apcActiveCSShaderResourceViews[0]);
				((ID3D11DeviceContext*)pThis)->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &m_apcActiveCSConstantBuffers[0]);
				((ID3D11DeviceContext*)pThis)->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, &m_apcActiveUnorderedAccessViews[0], m_aunMinusOne);
			}*/
			return nullptr;
#pragma endregion
#pragma region CSSETSHADERRESOURCES
		case (int)VMT_ID3D11DEVICECONTEXT::CSSetShaderResources:
			/*if ((m_pdwStartSlot_CS) && (m_pdwNumViews_CS) && (m_pppcShaderResourceViews) && (m_bPresent))
			{
				if (m_bPresent)
				{
					// ensure D3D11 is set
					m_eD3DVersion = D3DVersion::Direct3D11;

					// verify node pointers
					if (!m_pdwStartSlot_CS) return nullptr;
					if (!m_pdwNumViews_CS) return nullptr;
					if (!m_pppcShaderResourceViews) return nullptr;

					// invalid call ? validate !
					if (*m_pdwStartSlot_CS + *m_pdwNumViews_CS > D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) return nullptr;

					XSSetShaderResourceViews(m_apcActiveCSShaderResourceViews, m_dwCSShaderResourceViewsNumber, *m_pdwStartSlot_CS, *m_pdwNumViews_CS, *m_pppcShaderResourceViews);
				}
			}*/
			return nullptr;
#pragma endregion
#pragma region CSSETUNORDEREDACCESSVIEWS
		case (int)VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews:
			/*if (!m_pdwStartSlot_CSUAV) return nullptr;
			if (!m_pdwNumUAVs_CS) return nullptr;
			if (!m_pppcUnorderedAccessViews_CS) return nullptr;
			if (!m_ppdwUAVInitialCounts_CS) return nullptr;

			// call internal method
			CSSetUnorderedAccessViews((ID3D11DeviceContext*)pThis, *m_pdwStartSlot_CSUAV, *m_pdwNumUAVs_CS, *m_pppcUnorderedAccessViews_CS, *m_ppdwUAVInitialCounts_CS);*/
			return nullptr;
#pragma endregion
#pragma region CSSETCONSTANTBUFFERS
		case (int)VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers:
			/*if ((m_pdwStartSlot_CSCB) && (m_pdwNumBuffers) && (m_pppcConstantBuffers))
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
							Vireio_Buffer_Rules_Index sRulesIndex;
							sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
							sRulesIndex.m_dwUpdateCounter = 0;
							UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
							m_apcActiveCSConstantBuffers[dwInternalIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);

							// set twin for right side, first get the private data interface
							ID3D11Buffer* pcBuffer = nullptr;
							UINT dwSize = sizeof(pcBuffer);
							m_apcActiveCSConstantBuffers[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

							// stereo buffer and rules index present ?
							if ((pcBuffer) && (dwDataSizeRulesIndex) && (sRulesIndex.m_nRulesIndex >= 0))
							{
								// set right buffer as active buffer
								m_apcActiveCSConstantBuffers[dwInternalIndex + D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = pcBuffer;
							}
							else
							{
								// no buffer or no shader rules assigned ? left = right side -> right = left side
								m_apcActiveCSConstantBuffers[dwInternalIndex + D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = m_apcActiveCSConstantBuffers[dwInternalIndex];

								// verify buffer... TODO !! STILL POSSIBLE THAT A CONSTANT BUFFER NEVER ADDRESSED TO THE VERTEX SHADER BUT TO THE COMPUTE SHADER !!!
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
									CreateStereoBuffer(pcDevice, (ID3D11DeviceContext*)pThis, (ID3D11Buffer*)m_apcActiveCSConstantBuffers[dwInternalIndex], &sDesc, NULL, true);
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
			}*/
			return nullptr;
#pragma endregion
		}
		return nullptr;
#pragma endregion
#pragma region IDXGISWAPCHAIN
	case (int)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain:
		if (eD3DMethod == (int)VMT_IDXGISWAPCHAIN::Present)
			Present(nFlags);
		return nullptr;
#pragma endregion
	}
	return nullptr;
}

/// <summary>
/// => Update ImGui Control
/// Update node controls using ImGui. 
/// </summary>
/// <param name="fZoom">Current zoom factor on ImNodes canvas.</param>
void StereoSplitter::UpdateImGuiControl(float fZoom)
{
	ImGui::TextUnformatted("Vireio Perception Stereo Splitter Dx10+ Controls\n");

}

/// <summary>
/// Incoming OMSetRenderTargets() call (D3D10+).
/// Set render targets internally, handle render target monitoring.
/// @param pcDeviceOrContext For DX10 this is the device, for DX11 this is the context.
/// </summary>
void StereoSplitter::OMSetRenderTargets(IUnknown* pcDeviceOrContext, UINT NumViews, IUnknown* const* ppRenderTargetViews, IUnknown* pDepthStencilView)
{
	// is the back buffer verified for the current frame ?
	if ((TODO_ADD_BOOL_HERE) && (!(unsigned)m_eBackBufferVerified))
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
			for (std::vector<IUnknown*>::size_type i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			{
				m_apcActiveRenderTargetViews[i] = NULL;
				m_apcActiveRenderTargetViews[i + D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = NULL;
			}
		}
		else
		{
			// set render targets and the twins			
			for (std::vector<IUnknown*>::size_type i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			{
				if (i < (size_t)NumViews)
				{
					// set zero if zero
					if (!ppRenderTargetViews[i])
					{
						m_apcActiveRenderTargetViews[i] = NULL;
						m_apcActiveRenderTargetViews[i + D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = NULL;
					}
					else
					{
						// set the target
						m_apcActiveRenderTargetViews[i] = ppRenderTargetViews[i];

						// verify the stereo components of this interface
						m_apcActiveRenderTargetViews[i + D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] =
							VerifyPrivateDataInterfaces((ID3D11RenderTargetView*)ppRenderTargetViews[i]);
					}
				}
				else
				{
					// set all render targets above the provided number to null
					m_apcActiveRenderTargetViews[i] = NULL;
					m_apcActiveRenderTargetViews[i + D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = NULL;
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

/// <summary>
/// Incoming CSSetUnorderedAccessViews() call (D3D11).
/// Handle UAVs and set them internally.
/// D3D11_PS_CS_UAV_REGISTER_COUNT -> DX 11.0
/// D3D11_1_UAV_SLOT_COUNT ---------> DX 11.1
/// @param pcThis D3D11 Device Context.
/// </summary>
void StereoSplitter::CSSetUnorderedAccessViews(ID3D11DeviceContext* pcContext, UINT dwStartSlot, UINT dwNumUAVs, ID3D11UnorderedAccessView* const* ppcUnorderedAccessViews, const UINT* pdwUAVInitialCounts)
{
	// loop through the new buffers
	for (UINT dwIndex = 0; dwIndex < dwNumUAVs; dwIndex++)
	{
		// get internal index
		UINT dwInternalIndex = dwIndex + dwStartSlot;

		// in range ? 
		if (dwInternalIndex < D3D11_PS_CS_UAV_REGISTER_COUNT)
		{
			// set view internal
			m_apcActiveUnorderedAccessViews[dwInternalIndex] = ppcUnorderedAccessViews[dwIndex];

			if (m_apcActiveUnorderedAccessViews[dwInternalIndex])
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC sDesc;
				m_apcActiveUnorderedAccessViews[dwInternalIndex]->GetDesc(&sDesc);

				// is a buffer ?
				if (sDesc.ViewDimension == D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_BUFFER)
				{
					// get the resource
					ID3D11Resource* pcBuffer = nullptr;
					m_apcActiveUnorderedAccessViews[dwInternalIndex]->GetResource(&pcBuffer);
					if (pcBuffer)
					{
						// set twin for right side, first get the private data interface
						ID3D11Buffer* pcStereoBuffer = nullptr;
						UINT dwSize = sizeof(pcStereoBuffer);
						pcBuffer->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcStereoBuffer);

						if (pcStereoBuffer)
						{
							// get the private view from the stereo buffer
							ID3D11UnorderedAccessView* pcUAV = nullptr;
							UINT dwSize = sizeof(pcUAV);
							pcStereoBuffer->GetPrivateData(PDIID_ID3D11Buffer_UAV_Right, &dwSize, (void*)&pcUAV);
							if (pcUAV)
							{
								// set stereo UAV internally
								m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] = pcUAV;

								pcUAV->Release();
							}
							else
							{
								// no UAV present, create one
								ID3D11Device* pcDevice = nullptr;
								pcStereoBuffer->GetDevice(&pcDevice);
								if (pcDevice)
								{
									pcDevice->CreateUnorderedAccessView(pcStereoBuffer, &sDesc, &pcUAV);
									pcDevice->Release();
								}

								if (pcUAV)
								{
									// set stereo UAV internally
									m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] = pcUAV;

									// set as private data interface to the stereo buffer
									dwSize = sizeof(pcUAV);
									pcStereoBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_UAV_Right, (const IUnknown*)pcUAV);

									pcUAV->Release();
								}
								else
									m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] = m_apcActiveUnorderedAccessViews[dwInternalIndex];
							}

							pcStereoBuffer->Release();
						}
						else
						{
							// create stereo constant buffer, first get device
							ID3D11Device* pcDevice = nullptr;
							pcBuffer->GetDevice(&pcDevice);
							if (pcDevice)
							{
								D3D11_BUFFER_DESC sDesc;
								((ID3D11Buffer*)pcBuffer)->GetDesc(&sDesc);
								CreateStereoBuffer(pcDevice, pcContext, (ID3D11Buffer*)pcBuffer, &sDesc, NULL, true);
								pcDevice->Release();
							}

							// set mono view meanwhile
							m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] = m_apcActiveUnorderedAccessViews[dwInternalIndex];
						}


						pcBuffer->Release();
					}
				}
				else
				{
					// verify the stereo components of this interface
					m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] =
						VerifyPrivateDataInterfaces(m_apcActiveUnorderedAccessViews[dwInternalIndex]);

					// UAVs must not be NULL for first register, set the left view in case
					if (!m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT])
						m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] =
						m_apcActiveUnorderedAccessViews[dwInternalIndex];
				}
			}
			else
				m_apcActiveUnorderedAccessViews[dwInternalIndex + D3D11_PS_CS_UAV_REGISTER_COUNT] = m_apcActiveUnorderedAccessViews[dwInternalIndex];


		}
		else OutputDebugString(L"[STS] Game uses DirectX 11.1 : D3D11_1_UAV_SLOT_COUNT");
	}
}

/// <summary>
/// Handles shader resource views internally.
/// Called by PSSetShaderResourceViews() and CSSetShaderResourceViews().
/// </summary>
void StereoSplitter::XSSetShaderResourceViews(std::vector<ID3D11ShaderResourceView*>& apcActiveShaderResourceViews, UINT& unNumViewsTotal, UINT unStartSlot, UINT unNumViews, ID3D11ShaderResourceView* const* ppcShaderResourceViews)
{
	// set to NULL ?
	bool bSetNull = false;
	if (!ppcShaderResourceViews)
		bSetNull = true;
	else if (!*ppcShaderResourceViews)
		bSetNull = true;
	if (bSetNull)
	{
		// loop through new resource views
		for (UINT dwIndex = 0; dwIndex < unNumViews; dwIndex++)
		{
			// get and verify index
			DWORD dwIndexActive = unStartSlot + dwIndex;
			if (dwIndexActive < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
			{
				// set shader resource view for both sides
				apcActiveShaderResourceViews[dwIndexActive] = nullptr;
				apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = nullptr;
			}

		}
		// return to replace the call
		return;
	}

	// set total view number
	if (unStartSlot + unNumViews > unNumViewsTotal)
		unNumViewsTotal = (DWORD)(unStartSlot + unNumViews);

	// delete all if zero ? should have been done only in ClearState().. maybe we can delete this
	if (!unNumViewsTotal)
	{
		// clear view vector and number ?
		apcActiveShaderResourceViews = std::vector<ID3D11ShaderResourceView*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2, nullptr);
	}

	// loop through new resource views
	for (UINT dwIndex = 0; dwIndex < unNumViews; dwIndex++)
	{
		// get and verify index
		DWORD dwIndexActive = unStartSlot + dwIndex;
		if (!ppcShaderResourceViews[dwIndex])
		{
			// set shader resource view for both sides
			apcActiveShaderResourceViews[dwIndexActive] = nullptr;
			apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = nullptr;

			continue;
		}

		// get description
		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
		ppcShaderResourceViews[dwIndex]->GetDesc(&sDesc);

		// is texture ? 
		if ((sDesc.ViewDimension >= D3D11_SRV_DIMENSION_TEXTURE1D) &&
			(sDesc.ViewDimension <= D3D11_SRV_DIMENSION_TEXTURECUBEARRAY))
		{
			// set active texture
			apcActiveShaderResourceViews[dwIndexActive] = ppcShaderResourceViews[dwIndex];

			// get resource
			ID3D11Resource* pcResource;
			ppcShaderResourceViews[dwIndex]->GetResource(&pcResource);
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
						apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = pcShaderResourceView;

						pcShaderResourceView->Release();
					}
					else
					{
						// set mono
						apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];

						// add to vector for new views, just if not present
						auto it = std::find(m_apcNewShaderResourceViews11.begin(), m_apcNewShaderResourceViews11.end(), ppcShaderResourceViews[dwIndex]);
						if (it == m_apcNewShaderResourceViews11.end())
							m_apcNewShaderResourceViews11.push_back(ppcShaderResourceViews[dwIndex]);
					}

					pcResourceTwin->Release();
				}
				else
				{
					// no resource twin ? no render target texture, set mono
					apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];

					// is this texture to be set to stereo ?
					BOOL bNew = FALSE;
					dwSize = sizeof(BOOL);
					pcResource->GetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, &dwSize, (void*)&bNew);
					if ((dwSize) && (bNew))
					{
						// bool to create a new view here is set to "TRUE", so add to new shader resource views list

						// add to vector for new views, just if not present
						auto it = std::find(m_apcNewShaderResourceViews11.begin(), m_apcNewShaderResourceViews11.end(), ppcShaderResourceViews[dwIndex]);
						if (it == m_apcNewShaderResourceViews11.end())
							m_apcNewShaderResourceViews11.push_back(ppcShaderResourceViews[dwIndex]);

						// set private data to "FALSE"
						bNew = FALSE;
						pcResource->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bNew);
					}
				}

				pcResource->Release();
			}
			else
				// no resource ? shouldn't be.. set shader resource view for both sides
				apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];
		}
		else
			// is a buffer ?
			if (sDesc.ViewDimension == D3D11_SRV_DIMENSION_BUFFER)
			{
				// set active buffer
				apcActiveShaderResourceViews[dwIndexActive] = ppcShaderResourceViews[dwIndex];

				// get the resource
				ID3D11Resource* pcBuffer = nullptr;
				apcActiveShaderResourceViews[dwIndexActive]->GetResource(&pcBuffer);
				if (pcBuffer)
				{
					// set twin for right side, first get the private data interface
					ID3D11Buffer* pcStereoBuffer = nullptr;
					UINT dwSize = sizeof(pcStereoBuffer);
					pcBuffer->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcStereoBuffer);

					if (pcStereoBuffer)
					{
						// get the private view from the stereo buffer
						ID3D11ShaderResourceView* pcSRV = nullptr;
						UINT dwSize = sizeof(pcSRV);
						pcStereoBuffer->GetPrivateData(PDIID_ID3D11Buffer_ShaderResourceView_Right, &dwSize, (void*)&pcSRV);
						if (pcSRV)
						{
							// set stereo view internally
							apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = pcSRV;

							pcSRV->Release();
						}
						else
						{
							// no UAV present, create one
							ID3D11Device* pcDevice = nullptr;
							pcStereoBuffer->GetDevice(&pcDevice);
							if (pcDevice)
							{
								pcDevice->CreateShaderResourceView(pcStereoBuffer, &sDesc, &pcSRV);
								pcDevice->Release();
							}

							if (pcSRV)
							{
								// set stereo view internally
								apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = pcSRV;

								// set as private data interface to the stereo buffer
								dwSize = sizeof(pcSRV);
								pcStereoBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_ShaderResourceView_Right, (const IUnknown*)pcSRV);

								pcSRV->Release();
							}
							else
							{
								OutputDebugString(L"Failed to create shader resource view!");
								apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];
							}
						}
						pcStereoBuffer->Release();
					}
					else
						apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];

					pcBuffer->Release();
				}
			}
			else
			{
				OutputDebugString(L"Unknown Shader Resource View !");

				// set shader resource view for both sides
				apcActiveShaderResourceViews[dwIndexActive] = ppcShaderResourceViews[dwIndex];
				apcActiveShaderResourceViews[dwIndexActive + D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = ppcShaderResourceViews[dwIndex];
			}
	}
}

/// <summary>
/// Creates a stereo view out of a mono view.
/// Creates the stereo texture and its view, then assignes all as private data interfaces.
/// </summary>
void StereoSplitter::CreateStereoView(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11View* pcView)
{
	if ((!pcView) || (!pcDevice) || (!pcContext)) return;

#pragma region determine view type
	// get bind flag
	D3D11_BIND_FLAG eBindFlag;
	LPVOID pvObject = nullptr;
	if (FAILED(pcView->QueryInterface(__uuidof(ID3D11RenderTargetView), &pvObject)))
	{
		if (FAILED(pcView->QueryInterface(__uuidof(ID3D11DepthStencilView), &pvObject)))
		{
			if (FAILED(pcView->QueryInterface(__uuidof(ID3D11ShaderResourceView), &pvObject)))
			{
				if (FAILED(pcView->QueryInterface(__uuidof(ID3D11UnorderedAccessView), &pvObject)))
				{
					// set to unsupported bind flag "video encoder"
					OutputDebugString(L"StereoSplitterDX10: Failed to determine d3d view type !");
					eBindFlag = D3D11_BIND_FLAG::D3D11_BIND_VIDEO_ENCODER;
				}
				else
				{
					eBindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
					((IUnknown*)pvObject)->Release();
				}
			}
			else
			{
				eBindFlag = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
				((IUnknown*)pvObject)->Release();
			}
		}
		else
		{
			eBindFlag = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
			((IUnknown*)pvObject)->Release();
		}
	}
	else
	{
		eBindFlag = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		((IUnknown*)pvObject)->Release();
	}
#pragma endregion

#pragma region get description
	// all possible description union
	union
	{
		D3D11_RENDER_TARGET_VIEW_DESC sDescRT11;
		D3D11_DEPTH_STENCIL_VIEW_DESC sDescDS11;
		D3D11_SHADER_RESOURCE_VIEW_DESC sDescSR11;
		D3D11_UNORDERED_ACCESS_VIEW_DESC sDescUAV11;
	};

	// get description
	switch (eBindFlag)
	{
	case D3D11_BIND_SHADER_RESOURCE:
		((ID3D11ShaderResourceView*)pcView)->GetDesc(&sDescSR11);
		break;
	case D3D11_BIND_RENDER_TARGET:
		((ID3D11RenderTargetView*)pcView)->GetDesc(&sDescRT11);
		break;
	case D3D11_BIND_DEPTH_STENCIL:
		((ID3D11DepthStencilView*)pcView)->GetDesc(&sDescDS11);
		break;
	case D3D11_BIND_UNORDERED_ACCESS:
		((ID3D11UnorderedAccessView*)pcView)->GetDesc(&sDescUAV11);
		break;
	default:
		break;
	}
#pragma endregion

#pragma region create new
	// get the resource
	ID3D11Resource* pcResource = nullptr;
	ID3D11Resource* pcResourceTwin = nullptr;
	pcView->GetResource(&pcResource);

	if (pcResource)
	{
		// get the view dimension
		D3D11_RESOURCE_DIMENSION sDimension;
		pcResource->GetType(&sDimension);

		// verify or create twin resource
		switch (sDimension)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			// has this texture already a stereo twin ?
			UINT dwSize = sizeof(pcResourceTwin);
			pcResource->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
			if (!dwSize)
			{
				// get the description and create the twin texture
				D3D11_TEXTURE1D_DESC sDesc;
				((ID3D11Texture1D*)pcResource)->GetDesc(&sDesc);

				if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture1D(&sDesc, NULL, (ID3D11Texture1D**)&pcResourceTwin)))
				{
					OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
					break;
				}
				else
				{
					pcContext->CopyResource(pcResourceTwin, pcResource);
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcResourceTwin);
				}
			}
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			// has this texture already a stereo twin ?
			UINT dwSize = sizeof(pcResourceTwin);
			pcResource->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
			if (!dwSize)
			{
				// get the description and create the twin texture
				D3D11_TEXTURE2D_DESC sDesc;
				((ID3D11Texture2D*)pcResource)->GetDesc(&sDesc);

				// handle sRGB formats : there was an error creating textures with one game... can't remember which one... hmm... this fixed it
				if (sDesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
					sDescRT11.Format = sDesc.Format;

				if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture2D(&sDesc, NULL, (ID3D11Texture2D**)&pcResourceTwin)))
				{
					OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
					break;
				}
				else
				{
					pcContext->CopyResource(pcResourceTwin, pcResource);
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcResourceTwin);
				}
			}
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			// has this texture already a stereo twin ?
			UINT dwSize = sizeof(pcResourceTwin);
			pcResource->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
			if (!dwSize)
			{
				// get the description and create the twin texture
				D3D11_TEXTURE3D_DESC sDesc;
				((ID3D11Texture3D*)pcResource)->GetDesc(&sDesc);

				if (FAILED(((ID3D11Device*)pcDevice)->CreateTexture3D(&sDesc, NULL, (ID3D11Texture3D**)&pcResourceTwin)))
				{
					OutputDebugString(L"StereoSplitterDX10 : Failed to create twin texture !");
					break;
				}
				else
				{
					pcContext->CopyResource(pcResourceTwin, pcResource);
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_Stereo_Twin, pcResourceTwin);
				}
			}
		}
		break;
		default:
			break;
		}

		// create view by bind flag
		switch (eBindFlag)
		{
		case D3D11_BIND_SHADER_RESOURCE:
		{
			// create the shader resource view
			ID3D11ShaderResourceView* pcViewTwin = nullptr;
			if (FAILED(((ID3D11Device*)pcDevice)->CreateShaderResourceView(pcResourceTwin, &sDescSR11, &pcViewTwin)))
				OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_BIND_SHADER_RESOURCE!");
			else
				if (pcResourceTwin)
				{
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_ShaderResourceView, pcView);
					// TODO !! create GUID for this ? need this ?
					// pcView->SetPrivateDataInterface(PDIID_ID3D11ShaderResourceView_Stereo_Twin, pcViewTwin);
					pcViewTwin->Release();
				}
			break;
		}
		break;
		case D3D11_BIND_RENDER_TARGET:
		{
			// create the render target view twin
			ID3D11RenderTargetView* pcViewTwin = nullptr;
			if (FAILED(((ID3D11Device*)pcDevice)->CreateRenderTargetView(pcResourceTwin, &sDescRT11, &pcViewTwin)))
				OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_BIND_RENDER_TARGET !");
			else
				if (pcResourceTwin)
				{
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcView);
					pcView->SetPrivateDataInterface(PDIID_ID3D11RenderTargetView_Stereo_Twin, pcViewTwin);
					pcViewTwin->Release();
				}
		}
		break;
		case D3D11_BIND_DEPTH_STENCIL:
		{
			// create the depth stencil view twin
			ID3D11DepthStencilView* pcViewTwin = nullptr;
			if (FAILED(((ID3D11Device*)pcDevice)->CreateDepthStencilView(pcResourceTwin, &sDescDS11, &pcViewTwin)))
				OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_BIND_DEPTH_STENCIL !");
			else
				if (pcResourceTwin)
				{
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_DepthStencilView, pcView);
					pcView->SetPrivateDataInterface(PDIID_ID3D11DepthStencilView_Stereo_Twin, pcViewTwin);
					pcViewTwin->Release();
				}
		}
		break;
		case D3D11_BIND_UNORDERED_ACCESS:
		{
			// create the unordered access view twin
			ID3D11UnorderedAccessView* pcViewTwin = nullptr;
			if (FAILED(((ID3D11Device*)pcDevice)->CreateUnorderedAccessView(pcResourceTwin, &sDescUAV11, &pcViewTwin)))
				OutputDebugString(L"StereoSplitterDX10 : Failed to create twin view D3D11_BIND_UNORDERED_ACCESS !");
			else
				if (pcResourceTwin)
				{
					pcResource->SetPrivateDataInterface(PDIID_ID3D11TextureXD_UnorderedAccessView, pcView);
					pcView->SetPrivateDataInterface(PDIID_ID3D11UnorderedAccessView_Stereo_Twin, pcViewTwin);
					pcViewTwin->Release();
				}
		}
		break;
		default:
			break;
		}

		if (pcResourceTwin) pcResourceTwin->Release();
		pcResource->Release();
	}
#pragma endregion

	// update control... maybe we need this later
	m_bControlUpdate = true;
}

/// <summary>
/// Verifies all (stereo) private data interfaces for this render target view.
/// Creates new stereo interfaces if not present.
/// </summary>
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

/// <summary>
/// Verifies all (stereo) private data interfaces for this depth view.
/// Creates new stereo interfaces if not present.
/// </summary>
ID3D11DepthStencilView* StereoSplitter::VerifyPrivateDataInterfaces(ID3D11DepthStencilView* pcDepthStencilView)
{
	if (!pcDepthStencilView) return nullptr;

	// does this depth stencil view have a stereo twin view ?
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

/// <summary>
/// Verifies all (stereo) private data interfaces for this unordered access view.
/// Creates new stereo interfaces if not present.
/// </summary>
ID3D11UnorderedAccessView* StereoSplitter::VerifyPrivateDataInterfaces(ID3D11UnorderedAccessView* pcUnorderedAccessView)
{
	if (!pcUnorderedAccessView) return nullptr;

	// does this unordered access view have a stereo twin view ?
	ID3D11UnorderedAccessView* pcView = nullptr;
	UINT dwSize = sizeof(pcView);
	pcUnorderedAccessView->GetPrivateData(PDIID_ID3D11UnorderedAccessView_Stereo_Twin, &dwSize, (void*)&pcView);

	if (dwSize)
	{
		// one release on a private data interface must not give back a nullptr
		pcView->Release();
		return pcView;
	}
	else
	{
		// is this unordered access view already present in "new" list ?
		bool bInList = false;
		auto it = m_apcNewUnorderedAccessViews11.begin();
		while (it != m_apcNewUnorderedAccessViews11.end())
		{
			if (pcUnorderedAccessView == *it)
			{
				bInList = true;
				break;
			}
			it++;
		}

		// add to new unordered access list vector
		if (!bInList)
			m_apcNewUnorderedAccessViews11.push_back(pcUnorderedAccessView);

		return nullptr;
	}
}

/// <summary>
/// Switches rendering to which ever side is specified by side.
/// Use to specify the side that you want to draw to.
///
/// Derives D3DProxyDevice::setDrawingSide() from the Vireio 2.x.x driver code.
///
/// DX10 method !!
///
/// @param pcDevice The D3D 10 device.
/// @param eSide The destination side to set.
/// @return True if change succeeded, false if it fails.
/// Attempting to switch to a side when that side is already the active side will return true without making any changes.
/// </summary>
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
		pcDevice->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D10RenderTargetView**)&m_apcActiveRenderTargetViews[D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D10DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);

	// TODO !! port DX11 code back to DX10

	return true;
}

/// <summary>
/// Switches rendering to which ever side is specified by side.
/// Use to specify the side that you want to draw to.
///
/// Derives D3DProxyDevice::setDrawingSide() from the Vireio 2.x.x driver code.
///
/// DX11 method !!
///
/// @param pcContext The D3D 11 device context.
/// @param eSide The destination side to set.
/// @return True if change succeeded, false if it fails.
/// Attempting to switch to a side when that side is already the active side will return true without making any changes.
/// </summary>
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

	// switch render targets ?
	/*if (m_pbSwitchRenderTarget)
	{
		if (*m_pbSwitchRenderTarget)
		{
			m_bRenderTargetWasSwitched = true;
		}
		else
		{
			m_bRenderTargetWasSwitched = false;

			// switch render targets to new side
			if (eSide == RenderPosition::Left)
				pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
			else
				pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);
		}
	}
	else
	{
		m_bRenderTargetWasSwitched = false;

		// switch render targets to new side
		if (eSide == RenderPosition::Left)
			pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[0], (ID3D11DepthStencilView*)m_pcActiveDepthStencilView);
		else
			pcContext->OMSetRenderTargets(m_dwRenderTargetNumber, (ID3D11RenderTargetView**)&m_apcActiveRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT], (ID3D11DepthStencilView*)m_pcActiveStereoTwinDepthStencilView);
	}

	// switch textures to new side
	if (eSide == RenderPosition::Left)
		pcContext->PSSetShaderResources(0, m_dwPSShaderResourceViewsNumber, (ID3D11ShaderResourceView**)&m_apcActivePSShaderResourceViews[0]);
	else
		pcContext->PSSetShaderResources(0, m_dwPSShaderResourceViewsNumber, (ID3D11ShaderResourceView**)&m_apcActivePSShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]);

	// switch vertex shader constant buffers
	if (m_appcVSActiveConstantBuffers11)
		if (*m_appcVSActiveConstantBuffers11)
		{
			// set all constant buffers for the left or right side
			if (eSide == RenderPosition::Left)
				pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcVSActiveConstantBuffers11)[0]);
			else
				pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcVSActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
		}

	// switch hull shader constant buffers
	if (m_appcHSActiveConstantBuffers11)
		if (*m_appcHSActiveConstantBuffers11)
		{
			// set all constant buffers for the left or right side
			if (eSide == RenderPosition::Left)
				pcContext->HSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcHSActiveConstantBuffers11)[0]);
			else
				pcContext->HSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcHSActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
		}

	// switch domain shader constant buffers
	if (m_appcDSActiveConstantBuffers11)
		if (*m_appcDSActiveConstantBuffers11)
		{
			// set all constant buffers for the left or right side
			if (eSide == RenderPosition::Left)
				pcContext->DSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcDSActiveConstantBuffers11)[0]);
			else
				pcContext->DSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcDSActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
		}

	// switch geometry shader constant buffers
	if (m_appcGSActiveConstantBuffers11)
		if (*m_appcGSActiveConstantBuffers11)
		{
			// set all constant buffers for the left or right side
			if (eSide == RenderPosition::Left)
				pcContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcGSActiveConstantBuffers11)[0]);
			else
				pcContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcGSActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
		}

	// switch pixel shader constant buffers
	if (m_appcPSActiveConstantBuffers11)
		if (*m_appcPSActiveConstantBuffers11)
		{
			// set all constant buffers for the left or right side
			if (eSide == RenderPosition::Left)
				pcContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcPSActiveConstantBuffers11)[0]);
			else
				pcContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &(*m_appcPSActiveConstantBuffers11)[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]);
		}

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

		if (m_appcVSActiveConstantBuffers11)
			if (*m_appcVSActiveConstantBuffers11)
			{
				// loop through the buffers
				for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
					if ((*m_appcVSActiveConstantBuffers11)[dwIndex])
					{
						// set the current shader data as private data to the buffer
						if ((pcVertexShader) && (dwDataSize))
							((*m_appcVSActiveConstantBuffers11)[dwIndex])->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);
					}
			}

		if (pcVertexShader)
			pcVertexShader->Release();
	}*/

	return true;
}

HRESULT StereoSplitter::CSSetConstantBuffers(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::CSSetSamplers(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::CSSetShader(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::CSSetShaderResources(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::CSSetUnorderedAccessViews(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::ClearDepthStencilView(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::ClearRenderTargetView(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::ClearState(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::Dispatch(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DispatchIndirect(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::Draw(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawAuto(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawIndexed(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawIndexedInstanced(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawIndexedInstancedIndirect(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawInstanced(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::DrawInstancedIndirect(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::Map(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::OMGetRenderTargets(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::OMGetRenderTargetsAndUnorderedAccessViews(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::OMSetRenderTargets(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::OMSetRenderTargetsAndUnorderedAccessViews(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::PSGetShaderResources(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::PSSetShaderResources(int& nFlags) { return E_NOTIMPL; }

/// <summary>
/// => Present
/// </summary>
void StereoSplitter::Present(int& nFlags)
{
	if (!m_eD3DVersion) return;

#pragma region /// => Present - verify constant buffer counter

	// TODO !! VERIFY WHICH CONSTANT BUFFER BELONGS TO WHICH SHADER !!

	// if not connected the constant buffers will not be verified
	/*if (m_pdwVerifyConstantBuffers)
	{
		// > Zero ?
		if (*m_pdwVerifyConstantBuffers)
		{
			// update constant buffer frame count
			(*m_pdwVerifyConstantBuffers)--;
			m_dwVerifyConstantBuffers = *m_pdwVerifyConstantBuffers;

			m_bControlUpdate = true;
		}
	}*/
#pragma endregion

#pragma region /// => Present - create new stereo views
	if ((m_apcNewRenderTargetViews11.size()) || (m_apcNewDepthStencilViews11.size()) || (m_apcNewShaderResourceViews11.size()) || (m_apcNewUnorderedAccessViews11.size()))
	{
		// get device and context
		ID3D11Device* pcDevice = nullptr;
		ID3D11DeviceContext* pcContext = nullptr;
		if (SUCCEEDED(GetDeviceAndContext(m_pcSwapChainCurrent, &pcDevice, &pcContext)))
		{
			// create new render target view
			while (m_apcNewRenderTargetViews11.size())
			{
				// create and delete the first in the list
				CreateStereoView(pcDevice, pcContext, m_apcNewRenderTargetViews11[0]);
				m_apcNewRenderTargetViews11.erase(m_apcNewRenderTargetViews11.begin());
			}

			// create new depth stencil view
			if (m_apcNewDepthStencilViews11.size())
			{
				// create and delete the first in the list
				CreateStereoView(pcDevice, pcContext, m_apcNewDepthStencilViews11[0]);
				m_apcNewDepthStencilViews11.erase(m_apcNewDepthStencilViews11.begin());
			}

			// create new unordered access views
			while (m_apcNewUnorderedAccessViews11.size())
			{
				// create and delete the first in the list
				CreateStereoView(pcDevice, pcContext, m_apcNewUnorderedAccessViews11[0]);
				m_apcNewUnorderedAccessViews11.erase(m_apcNewUnorderedAccessViews11.begin());
			}

			// create new shader resource view
			if (m_apcNewShaderResourceViews11.size())
			{
				// get the description
				D3D11_SHADER_RESOURCE_VIEW_DESC sDesc = {};
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
						{
							if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)pcResourceTwin, nullptr, &pcShaderResourceView))))
								OutputDebugString(L"[STS] Failed to create texture view!");
						}
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
						CreateStereoView(pcDevice, pcContext, m_apcNewShaderResourceViews11[0]);
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

#pragma region /// => Present - get back buffer
	// finally, provide pointers to the left and right render target
	switch (m_eD3DVersion)
	{
	case Direct3D10:
		break;
	case Direct3D11:
	{
		// get back buffer
		ID3D11Texture2D* pcBackBuffer = NULL;
		m_pcSwapChainCurrent->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
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
							m_pcSwapChainCurrent->SetPrivateDataInterface(PDIID_IDXGISwapChain_TextureXD_Stereo_Twin, m_pcActiveStereoTwinBackBuffer11);
							m_pcSwapChainCurrent->SetPrivateDataInterface(PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin, pcViewTwin);

							pcViewTwin->Release();
						}
						m_pcActiveStereoTwinBackBuffer11->Release();
					}
					else
						OutputDebugString(L"StereoSplitter: No back buffer stereo texture present !");

					// get device
					ID3D11Device* pcDevice = nullptr;
					m_pcSwapChainCurrent->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
					if (pcDevice)
					{
						// create target textures by description
						D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
						ZeroMemory(&sDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
						D3D11_TEXTURE2D_DESC sDescRT;
						pcBackBuffer->GetDesc(&sDescRT);
						sDescRT.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

						if (FAILED(pcDevice->CreateTexture2D(&sDescRT, NULL, &m_pcTex11[0])))
							OutputDebugString(L"[STS] : Failed to create Texture.");
						if (FAILED(pcDevice->CreateTexture2D(&sDescRT, NULL, &m_pcTex11[1])))
							OutputDebugString(L"[STS] : Failed to create Texture.");

						// ...and the views
						sDesc.Format = sDescRT.Format;
						sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						sDesc.Texture2D.MostDetailedMip = 0;
						sDesc.Texture2D.MipLevels = 1;

						if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[0], &sDesc, &m_pcTexView11[0]))))
						{
							if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[0], nullptr, &m_pcTexView11[0]))))
								OutputDebugString(L"[STS] : Failed to create texture shader resource view!");
						}
						if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[1], &sDesc, &m_pcTexView11[1]))))
						{
							if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)m_pcTex11[1], nullptr, &m_pcTexView11[1]))))
								OutputDebugString(L"[STS] : Failed to create texture shader resource view!");
						}

						ID3D11RenderTargetView* pcRTV[2];
						pcRTV[0] = nullptr; pcRTV[1] = nullptr;
						if ((FAILED(pcDevice->CreateRenderTargetView((ID3D11Resource*)m_pcTex11[0], NULL, &pcRTV[0]))))
						{
							OutputDebugString(L"[STS] : Failed to create texture render target view!");
						}
						if ((FAILED(pcDevice->CreateRenderTargetView((ID3D11Resource*)m_pcTex11[1], NULL, &pcRTV[1]))))
						{
							OutputDebugString(L"[STS] : Failed to create texture render target view!");
						}

						// set this as private data interface to the shader resource views instead of texture here !!!!
						if (pcRTV[0])
						{
							m_pcTexView11[0]->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcRTV[0]);
							pcRTV[0]->Release();
						}
						if (pcRTV[1])
						{
							m_pcTexView11[1]->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, pcRTV[1]);
							pcRTV[1]->Release();
						}

						pcDevice->Release();
					}

					pcView->Release();
				}
				else
				{
					// no render target view present ? return
					OutputDebugString(L"[STS] : No back buffer render target view present!");

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
					OutputDebugString(L"[STS] : No back buffer stereo texture present !");
			}
			pcBackBuffer->Release();
		}
		else
		{
			// no backbuffer present ? should not be...
			OutputDebugString(L"[STS] : No back buffer present !");
			m_pcActiveBackBuffer11 = NULL;
			m_pcActiveStereoTwinBackBuffer11 = NULL;
		}
	}
	break;
	default:
		break;
	}
#pragma endregion

#pragma region /// => Present - update stereo textures
	{
		// get device and context
		ID3D11Device* pcDevice = nullptr;
		ID3D11DeviceContext* pcContext = nullptr;
		if (FAILED(GetDeviceAndContext(m_pcSwapChainCurrent, &pcDevice, &pcContext)))
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

	// set present() bool to true, back buffer not picked for the next frame, HUD render target to be deleted
	m_bPresent = true;
	m_eBackBufferVerified = BackBufferVerificationState::NotVerified;
	m_bClearHUDRenderTarget = true;
}

HRESULT StereoSplitter::Unmap(int& nFlags) { return E_NOTIMPL; }
HRESULT StereoSplitter::VSSetShader(int& nFlags) { return E_NOTIMPL; }