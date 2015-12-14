/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.cpp> and
Class <VireioMatrixModifier> :
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

#include"VireioMatrixModifier.h"

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_ID3D11DEVICECONTEXT                                        11
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8
#define METHOD_ID3D11DEVICE_CREATEBUFFER                                     3
#define METHOD_ID3D11DEVICE_CREATEVERTEXSHADER                               12
#define METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS                      7
#define METHOD_ID3D11DEVICECONTEXT_PSSETSHADER                               9
#define METHOD_ID3D11DEVICECONTEXT_VSSETSHADER                               11
#define METHOD_ID3D11DEVICECONTEXT_MAP                                       14
#define METHOD_ID3D11DEVICECONTEXT_UNMAP                                     15
#define METHOD_ID3D11DEVICECONTEXT_PSSETCONSTANTBUFFERS                      16
#define METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION                     46
#define METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE                              47
#define METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE                         48
#define METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS                      72
#define METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS                      77
#define METHOD_ID3D10DEVICE_COPYSUBRESOURCEREGION                            32
#define METHOD_ID3D10DEVICE_COPYRESOURCE                                     33
#define METHOD_ID3D10DEVICE_UPDATESUBRESOURCE                                34

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

#define TO_DO_ADD_BOOL_HERE_TRUE                                           true
#define TO_DO_ADD_BOOL_HERE_FALSE                                         false
#define IPD_DEFAULT                                                      0.064f

/**
* Constructor.
***/
MatrixModifier::MatrixModifier() : AQU_Nodus(),
m_aszShaderConstantsA(),
m_aszShaderConstants(),
m_eDebugOption(Debug_Grab_Options::Debug_ConstantFloat4),
m_bGrabDebug(false),
m_adwPageIDs(0, 0),
m_asConstantRules(),
m_adwGlobalConstantRuleIndices(),
m_asShaderSpecificRuleIndices(),
m_aasConstantBufferRuleIndices()
{
	// create a new HRESULT pointer
	m_pvReturn = (void*)new HRESULT();

	// Vireio GUI is always null at begin... since in a compiled profile it is never used
	m_pcVireioGUI = nullptr;

	// manually fill the HMD info here.... TODO !!
	m_psHmdInfo = new HMDisplayInfo_OculusRift();

	// at start, set the view adjustment class to basic config
	ZeroMemory(&m_sGameConfiguration, sizeof(Vireio_GameConfiguration));
	m_sGameConfiguration.convergence = 3.0f;
	m_sGameConfiguration.ipd = IPD_DEFAULT;
	m_sGameConfiguration.worldScaleFactor = -32.0f;
	m_sGameConfiguration.PFOV = 110.0f;
	m_pcShaderViewAdjustment = new ViewAdjustment(m_psHmdInfo, &m_sGameConfiguration);

	// init
	m_pcShaderViewAdjustment->UpdateProjectionMatrices((float)1920.0f / (float)1080.0f, m_sGameConfiguration.PFOV);
	m_pcShaderViewAdjustment->ComputeViewTransforms();

	// TEST !! ADD A TEST RULE
	Vireio_Constant_Modification_Rule sRule = Vireio_Constant_Modification_Rule("NoName", 12, 0, 8, false, false, true, false, true, 4, 0, 0, true);
	m_asConstantRules.push_back(sRule);
	m_adwGlobalConstantRuleIndices.push_back(0);

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// create buffer vectors
	m_apcActiveConstantBuffers11 = std::vector<ID3D11Buffer*>(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, nullptr);

	// create output pointers
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_VertexShader] = (void*)&m_apcActiveConstantBuffers11[0];

	// set constant buffer verification at startup (first 30 frames)
	m_dwVerifyConstantBuffers = CONSTANT_BUFFER_VERIFICATION_FRAME_NUMBER;

	// init shader vector
	m_asShaders = std::vector<Vireio_D3D11_Shader>();
	m_dwShaders = 0;

	// mapped resource data
	m_asMappedBuffers = std::vector<Vireio_Map_Data>();
	m_dwMappedBuffers = 0;

	// nullptr for all dx10/11 input fields
	m_ppvShaderBytecode_VertexShader = nullptr;
	m_pnBytecodeLength_VertexShader = nullptr;
	m_ppcClassLinkage_VertexShader = nullptr;
	m_pppcVertexShader_DX10 = nullptr;
	m_ppvShaderBytecode_PixelShader = nullptr;
	m_pnBytecodeLength_PixelShader = nullptr;
	m_ppcClassLinkage_PixelShader = nullptr;
	m_pppcPixelShader_DX10 = nullptr;
	m_ppcVertexShader_10 = nullptr;
	m_ppcVertexShader_11 = nullptr;
	m_ppcPixelShader_10 = nullptr;
	m_ppcPixelShader_11 = nullptr;
	m_ppsDesc_DX10 = nullptr;
	m_ppsInitialData_DX10 = nullptr;
	m_pppcBuffer_DX10 = nullptr;
	m_pdwStartSlot_VertexShader = nullptr;
	m_pdwNumBuffers_VertexShader = nullptr;
	m_pppcConstantBuffers_DX10_VertexShader = nullptr;
	m_pppcConstantBuffers_DX11_VertexShader = nullptr;
	m_pdwStartSlot_PixelShader = nullptr;
	m_pdwNumBuffers_PixelShader = nullptr;
	m_pppcConstantBuffers_DX10_PixelShader = nullptr;
	m_pppcConstantBuffers_DX11_PixelShader = nullptr;
	m_ppcDstResource_DX10 = nullptr;
	m_ppcDstResource_DX11 = nullptr;
	m_pdwDstSubresource = nullptr;
	m_ppsDstBox_DX10 = nullptr;
	m_ppsDstBox_DX11 = nullptr;
	m_ppvSrcData = nullptr;
	m_pdwSrcRowPitch = nullptr;
	m_pdwSrcDepthPitch = nullptr;
	m_ppcDstResource_DX10_Copy = nullptr;
	m_ppcSrcResource_DX10_Copy = nullptr;
	m_ppcDstResource_DX11_Copy = nullptr;
	m_ppcSrcResource_DX11_Copy = nullptr;
	m_ppcDstResource_DX10_CopySub = nullptr;
	m_ppcDstResource_DX11_CopySub = nullptr;
	m_pdwDstSubresource_CopySub = nullptr;
	m_pdwDstX = nullptr;
	m_pdwDstY = nullptr;
	m_pdwDstZ = nullptr;
	m_ppcSrcResource_DX10_CopySub = nullptr;
	m_ppcSrcResource_DX11_CopySub = nullptr;
	m_pdwSrcSubresource = nullptr;
	m_ppsSrcBox_DX10 = nullptr;
	m_ppsSrcBox_DX11 = nullptr;
	m_pdwStartSlot_VertexShader = nullptr;
	m_pdwNumBuffers_VertexShader = nullptr;
	m_pppcConstantBuffers_VertexShader = nullptr;
	m_pdwStartSlot_PixelShader = nullptr;
	m_pdwNumBuffers_PixelShader = nullptr;
	m_pppcConstantBuffers_PixelShader = nullptr;
	m_ppcResource_Map = nullptr;
	m_pdwSubresource_Map = nullptr;
	m_psMapType = nullptr;
	m_pdwMapFlags = nullptr;
	m_ppsMappedResource = nullptr;
	m_ppcResource_Unmap = nullptr;
	m_pdwSubresource_Unmap = nullptr;
#endif
}

/**
* Destructor.
***/
MatrixModifier::~MatrixModifier()
{
}

/**
* Return the name of the Matrix Modifier node.
***/
const char* MatrixModifier::GetNodeType()
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	return "Matrix ModifierDx10";
#elif defined(VIREIO_D3D9)
	return "Matrix Modifier";
#endif
}

/**
* Returns a global unique identifier for the Matrix Modifier node.
***/
UINT MatrixModifier::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#define MY_PLUGIN_IDENTIFIER 76
#elif defined(VIREIO_D3D9)
#define MY_PLUGIN_IDENTIFIER 75
#endif
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Matrix Modifier node.
***/
LPWSTR MatrixModifier::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Matrix Modifier node.
***/
HBITMAP MatrixModifier::GetLogo()
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifierDx10.dll");
#elif defined(VIREIO_D3D9)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifier.dll");
#endif
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Matrix Modifier node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP MatrixModifier::GetControl()
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// test the shader constant list for updates 
	if (m_dwShaders < (UINT)m_asShaders.size())
	{
		// loop through new shaders
		for (UINT dwI = m_dwShaders; dwI < (UINT)m_asShaders.size(); dwI++)
		{
			// loop through shader buffers
			for (UINT dwJ = 0; dwJ < (UINT)m_asShaders[dwI].asBuffers.size(); dwJ++)
			{
				for (UINT dwK = 0; dwK < (UINT)m_asShaders[dwI].asBuffers[dwJ].asVariables.size(); dwK++)
				{
					// convert to wstring
					std::string szNameA = std::string(m_asShaders[dwI].asBuffers[dwJ].asVariables[dwK].szName);
					std::wstring szName(szNameA.begin(), szNameA.end());

					// constant available in list ?
					if (std::find(m_aszShaderConstants.begin(), m_aszShaderConstants.end(), szName) == m_aszShaderConstants.end())
					{
						m_aszShaderConstants.push_back(szName);
						m_aszShaderConstantsA.push_back(szNameA);
					}
				}
			}
		}
		m_dwShaders = (UINT)m_asShaders.size();
	}
#endif

	// here we create the Vireio GUI.... if this runs under a compiled profile this method is never called
	if (!m_pcVireioGUI)
	{
		SIZE sSizeOfThis;
		sSizeOfThis.cx = GUI_WIDTH; sSizeOfThis.cy = GUI_HEIGHT;
		m_pcVireioGUI = new Vireio_GUI(sSizeOfThis, L"Cambria", TRUE, GUI_CONTROL_FONTSIZE, RGB(110, 105, 95), RGB(254, 249, 240));

		// first, add all pages
		for (int i = 0; i < (int)GUI_Pages::NumberOfPages; i++)
		{
			UINT dwPage = m_pcVireioGUI->AddPage();
			m_adwPageIDs.push_back(dwPage);
		}

		// control structure
		Vireio_Control sControl;
		static std::vector<std::wstring> sEntriesCommanders;
		static std::vector<std::wstring> sEntriesDecommanders;
		ZeroMemory(&sControl, sizeof(Vireio_Control));

		// create the decommanders list
		sControl.m_eControlType = Vireio_Control_Type::StaticListBox;
		sControl.m_sPosition.x = GUI_CONTROL_FONTBORDER;
		sControl.m_sPosition.y = 0;
		sControl.m_sSize.cx = GUI_WIDTH - GUI_CONTROL_BORDER;
		sControl.m_sSize.cy = NUMBER_OF_DECOMMANDERS * 64;
		sControl.m_sStaticListBox.m_bSelectable = false;
		sControl.m_sStaticListBox.m_paszEntries = &sEntriesDecommanders;
		UINT dwDecommandersList = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::MainPage], sControl);

		// create the commanders list out of the decommanders list
		sControl.m_sPosition.y += NUMBER_OF_DECOMMANDERS * 64;
		sControl.m_sPosition.x = GUI_WIDTH >> 2;
		sControl.m_sSize.cy = NUMBER_OF_COMMANDERS * 64;
		sControl.m_sStaticListBox.m_paszEntries = &sEntriesCommanders;
		UINT dwCommandersList = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::MainPage], sControl);

		// and add all entries
		for (int i = 0; i < NUMBER_OF_DECOMMANDERS; i++)
			m_pcVireioGUI->AddEntry(dwDecommandersList, this->GetDecommanderName(i));
		for (int i = 0; i < NUMBER_OF_COMMANDERS; i++)
			m_pcVireioGUI->AddEntry(dwCommandersList, this->GetCommanderName(i));

		// add debug options, first the debug trace (bottom list ALWAYS first for each page)
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		sControl.m_eControlType = Vireio_Control_Type::ListBox;
		sControl.m_sPosition.x = GUI_CONTROL_BORDER;
		sControl.m_sPosition.y = GUI_CONTROL_BORDER + GUI_CONTROL_LINE * 4 + (GUI_HEIGHT >> 1);
		sControl.m_sSize.cx = GUI_WIDTH - GUI_CONTROL_BORDER - GUI_CONTROL_LINE;
		sControl.m_sSize.cy = GUI_HEIGHT - sControl.m_sPosition.y - GUI_CONTROL_FONTSIZE * 4; // (GUI_CONTROL_FONTSIZE * 4) = size of the bottom arrows of the page
		sControl.m_sListBox.m_paszEntries = &m_aszDebugTrace;
		sControl.m_sListBox.m_bSelectable = false;
		m_dwDebugTrace = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::DebugPage], sControl);

		// shader constant debug list (lists always first from bottom to top!)
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		sControl.m_eControlType = Vireio_Control_Type::ListBox;
		sControl.m_sPosition.x = GUI_CONTROL_BORDER;
		sControl.m_sPosition.y = GUI_CONTROL_BORDER + GUI_CONTROL_LINE * 3;
		sControl.m_sSize.cx = GUI_WIDTH - GUI_CONTROL_BORDER - GUI_CONTROL_LINE;
		sControl.m_sSize.cy = GUI_HEIGHT >> 1;
		sControl.m_sListBox.m_paszEntries = &m_aszShaderConstants;
		sControl.m_sListBox.m_bSelectable = true;
		m_dwShaderConstantsDebug = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::DebugPage], sControl);

		// debug type spin control
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		m_aszDebugOptions.push_back(L"Constant Float 4");
		m_aszDebugOptions.push_back(L"Constant Float 8");
		m_aszDebugOptions.push_back(L"Constant Float 16");
		m_aszDebugOptions.push_back(L"Constant Float 32");
		m_aszDebugOptions.push_back(L"Constant Float 64");
		sControl.m_eControlType = Vireio_Control_Type::SpinControl;
		sControl.m_sPosition.x = GUI_CONTROL_BORDER;
		sControl.m_sPosition.y = GUI_CONTROL_BORDER;
		sControl.m_sSize.cx = GUI_CONTROL_BUTTONSIZE;
		sControl.m_sSize.cy = GUI_CONTROL_FONTSIZE + GUI_CONTROL_FONTBORDER;
		sControl.m_sSpinControl.m_dwCurrentSelection = (DWORD)m_eDebugOption;
		sControl.m_sSpinControl.m_paszEntries = &m_aszDebugOptions;
		m_dwDebugSpin = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::DebugPage], sControl);

		// debug "grab" button
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		sControl.m_eControlType = Vireio_Control_Type::Button;
		sControl.m_sPosition.x = GUI_CONTROL_BORDER;
		sControl.m_sPosition.y = GUI_CONTROL_BORDER + GUI_CONTROL_LINE;
		sControl.m_sSize.cx = GUI_CONTROL_BUTTONSIZE;
		sControl.m_sSize.cy = GUI_CONTROL_FONTSIZE + GUI_CONTROL_FONTBORDER;
		static std::wstring szButtonText = std::wstring(L"Grab Debug Data");
		sControl.m_sButton.m_pszText = &szButtonText;
		m_dwDebugGrab = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::DebugPage], sControl);

		// debug "clear" button
		ZeroMemory(&sControl, sizeof(Vireio_Control));
		sControl.m_eControlType = Vireio_Control_Type::Button;
		sControl.m_sPosition.x = GUI_CONTROL_BORDER;
		sControl.m_sPosition.y = GUI_CONTROL_BORDER + GUI_CONTROL_LINE * 2;
		sControl.m_sSize.cx = GUI_CONTROL_BUTTONSIZE;
		sControl.m_sSize.cy = GUI_CONTROL_FONTSIZE + GUI_CONTROL_FONTBORDER;
		static std::wstring szButtonClear = std::wstring(L"Clear Debug Trace");
		sControl.m_sButton.m_pszText = &szButtonClear;
		m_dwClearDebug = m_pcVireioGUI->AddControl(m_adwPageIDs[GUI_Pages::DebugPage], sControl);
	}
	else
		return m_pcVireioGUI->GetGUI(false, true, false, false);

	return nullptr;
}

/**
* Get node data from the profile file.
***/
void MatrixModifier::InitNodeData(char* pData, UINT dwSizeOfData)
{
	if (dwSizeOfData == sizeof(Vireio_GameConfiguration))
	{
		// copy the game configuration data
		memcpy(&m_sGameConfiguration, pData, dwSizeOfData);

		// set to ipd using vireio presenter.... // TODO !! currently set ipd to default
		m_sGameConfiguration.convergence = 3.0f;
		m_sGameConfiguration.ipd = IPD_DEFAULT;
		m_sGameConfiguration.worldScaleFactor = -32.0f;
		m_sGameConfiguration.PFOV = 110.0f;
		m_pcShaderViewAdjustment->Load(m_sGameConfiguration);
		m_pcShaderViewAdjustment->UpdateProjectionMatrices((float)1920.0f / (float)1080.0f, m_sGameConfiguration.PFOV);
		m_pcShaderViewAdjustment->ComputeViewTransforms();
	}
}

/**
* Provides the name of the requested commander.
***/
LPWSTR MatrixModifier::GetCommanderName(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
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
		case asShaderData:
			return L"Shader Data Array";
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return L"UNTITLED";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR MatrixModifier::GetDecommanderName(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return L"pShaderBytecode_VS";
		case BytecodeLength_VertexShader:
			return L"BytecodeLength_VS";
		case pClassLinkage_VertexShader:
			return L"pClassLinkage_VertexShader";
		case ppVertexShader_DX10:
			return L"ppVertexShader_DX10";
		case pShaderBytecode_PixelShader:
			return L"pShaderBytecode_PS";
		case BytecodeLength_PixelShader:
			return L"BytecodeLength_PS";
		case pClassLinkage_PixelShader:
			return L"pClassLinkage_PixelShader";
		case ppPixelShader_DX10:
			return L"ppPixelShader_DX10";
		case pVertexShader_10:
			return L"pVertexShader_10";
		case pVertexShader_11:
			return L"pVertexShader_11";
		case pPixelShader_10:
			return L"pPixelShader_10";
		case pPixelShader_11:
			return L"pPixelShader_11";
		case pDesc_DX10:
			return L"pDesc_DX10";
		case pInitialData_DX10:
			return L"pInitialData_DX10";
		case ppBuffer_DX10:
			return L"ppBuffer_DX10";
		case StartSlot_VertexShader:
			return L"StartSlot_VS";
		case NumBuffers_VertexShader:
			return L"NumBuffers_VS";
		case ppConstantBuffers_DX10_VertexShader:
			return L"ppConstantBuffers_DX10_VS";
		case ppConstantBuffers_DX11_VertexShader:
			return L"ppConstantBuffers_DX11_VS";
		case StartSlot_PixelShader:
			return L"StartSlot_PS";
		case NumBuffers_PixelShader:
			return L"NumBuffers_PS";
		case ppConstantBuffers_DX10_PixelShader:
			return L"ppConstantBuffers_DX10_PS";
		case ppConstantBuffers_DX11_PixelShader:
			return L"ppConstantBuffers_DX11_PS";
		case pDstResource_DX10:
			return L"pDstResource_DX10";
		case pDstResource_DX11:
			return L"pDstResource_DX11";
		case DstSubresource:
			return L"DstSubresource";
		case pDstBox_DX10:
			return L"pDstBox_DX10";
		case pDstBox_DX11:
			return L"pDstBox_DX11";
		case pSrcData:
			return L"pSrcData";
		case SrcRowPitch:
			return L"SrcRowPitch";
		case SrcDepthPitch:
			return L"SrcDepthPitch";
		case pDstResource_DX10_Copy:
			return L"pDstResource_DX10_Copy";
		case pSrcResource_DX10_Copy:
			return L"pSrcResource_DX10_Copy";
		case pDstResource_DX11_Copy:
			return L"pDstResource_DX11_Copy";
		case pSrcResource_DX11_Copy:
			return L"pSrcResource_DX11_Copy";
		case pDstResource_DX10_CopySub:
			return L"pDstResource_DX10_CopySub";
		case pDstResource_DX11_CopySub:
			return L"pDstResource_DX11_CopySub";
		case DstSubresource_CopySub:
			return L"DstSubresource_CopySub";
		case DstX:
			return L"DstX";
		case DstY:
			return L"DstY";
		case DstZ:
			return L"DstZ";
		case pSrcResource_DX10_CopySub:
			return L"pSrcResource_DX10_CopySub";
		case pSrcResource_DX11_CopySub:
			return L"pSrcResource_DX11_CopySub";
		case SrcSubresource:
			return L"SrcSubresource";
		case pSrcBox_DX10:
			return L"pSrcBox_DX10";
		case pSrcBox_DX11:
			return L"pSrcBox_DX11";
		case StartSlot_Get_VertexShader:
			return L"StartSlot_Get_VertexShader";
		case NumBuffers_Get_VertexShader:
			return L"NumBuffers_Get_VertexShader";
		case ppConstantBuffers_DX10_Get_VertexShader:
			return L"ppConstantBuffers_DX10_Get_VS";
		case ppConstantBuffers_DX11_Get_VertexShader:
			return L"ppConstantBuffers_DX11_Get_VS";
		case StartSlot_Get_PixelShader:
			return L"StartSlot_Get_PixelShader";
		case NumBuffers_Get_PixelShader:
			return L"NumBuffers_Get_PixelShader";
		case ppConstantBuffers_DX10_Get_PixelShader:
			return L"ppConstantBuffers_DX10_Get_PS";
		case ppConstantBuffers_DX11_Get_PixelShader:
			return L"ppConstantBuffers_DX11_Get_PS";
		case pResource:
			return L"pResource";
		case Subresource:
			return L"Subresource";
		case MapType:
			return L"MapType";
		case MapFlags:
			return L"MapFlags";
		case pMappedResource:
			return L"pMappedResource";
		case pResource_Unmap:
			return L"pResource_Unmap";
		case Subresource_Unmap:
			return L"Subresource_Unmap";
		default:
			return L"UNTITLED";
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			return L"pShader_Vertex";
		case pShader_Pixel:
			return L"pShader_Pixel";
		case State:
			return L"State";
		case pMatrix:
			return L"pMatrix";
		case State_Multiply:
			return L"State_Multiply";
		case pMatrix_Multiply:
			return L"pMatrix_Multiply";
		case StartRegister_VertexShader:
			return L"StartRegister_VS";
		case pConstantData_VertexShader:
			return L"pConstantData_VS";
		case Vector4fCount_VertexShader:
			return L"Vector4fCount_VS";
		case StartRegister_PixelShader:
			return L"StartRegister_PS";
		case pConstantData_PixelShader:
			return L"pConstantData_PS";
		case Vector4fCount_PixelShader:
			return L"Vector4fCount_PS";
	}
#endif
	return L"UNTITLED";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD MatrixModifier::GetCommanderType(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
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
		case asShaderData:
			return NOD_Plugtype::AQU_VOID;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return NULL;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD MatrixModifier::GetDecommanderType(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_VertexShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_VertexShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppVertexShader_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10VERTEXSHADER;
		case pShaderBytecode_PixelShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_PixelShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_PixelShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppPixelShader_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10PIXELSHADER;
		case pVertexShader_10:
			return NOD_Plugtype::AQU_PNT_ID3D10VERTEXSHADER;
		case pVertexShader_11:
			return NOD_Plugtype::AQU_PNT_ID3D11VERTEXSHADER;
		case pPixelShader_10:
			return NOD_Plugtype::AQU_PNT_ID3D10PIXELSHADER;
		case pPixelShader_11:
			return NOD_Plugtype::AQU_PNT_ID3D11VERTEXSHADER;
		case pDesc_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BUFFER_DESC;
		case pInitialData_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_SUBRESOURCE_DATA;
		case ppBuffer_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10BUFFER;
		case StartSlot_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case StartSlot_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case pDstResource_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case DstSubresource:
			return NOD_Plugtype::AQU_UINT;
		case pDstBox_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BOX;
		case pDstBox_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BOX;
		case pSrcData:
			return NOD_Plugtype::AQU_PNT_VOID;
		case SrcRowPitch:
			return NOD_Plugtype::AQU_UINT;
		case SrcDepthPitch:
			return NOD_Plugtype::AQU_UINT;
		case pDstResource_DX10_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pSrcResource_DX10_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case pSrcResource_DX11_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case pDstResource_DX10_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case DstSubresource_CopySub:
			return NOD_Plugtype::AQU_UINT;
		case DstX:
			return NOD_Plugtype::AQU_UINT;
		case DstY:
			return NOD_Plugtype::AQU_UINT;
		case DstZ:
			return NOD_Plugtype::AQU_UINT;
		case pSrcResource_DX10_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pSrcResource_DX11_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case SrcSubresource:
			return NOD_Plugtype::AQU_UINT;
		case pSrcBox_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BOX;
		case pSrcBox_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BOX;
		case StartSlot_Get_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_Get_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_Get_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_Get_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case StartSlot_Get_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_Get_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_Get_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_Get_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case pResource:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case Subresource:
			return NOD_Plugtype::AQU_UINT;
		case MapType:
			return NOD_Plugtype::AQU_D3D11_MAP;
		case MapFlags:
			return NOD_Plugtype::AQU_UINT;
		case pMappedResource:
			return NOD_Plugtype::AQU_PNT_D3D11_MAPPED_SUBRESOURCE;
		case pResource_Unmap:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case Subresource_Unmap:
			return NOD_Plugtype::AQU_UINT;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DVERTEXSHADER9;
		case pShader_Pixel:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DPIXELSHADER9;
		case State:
			return NOD_Plugtype::AQU_D3DTRANSFORMSTATETYPE;
		case pMatrix:
			return NOD_Plugtype::AQU_PNT_D3DMATRIX;
		case State_Multiply:
			return NOD_Plugtype::AQU_D3DTRANSFORMSTATETYPE;
		case pMatrix_Multiply:
			return NOD_Plugtype::AQU_PNT_D3DMATRIX;
		case StartRegister_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case pConstantData_VertexShader:
			return NOD_Plugtype::AQU_PNT_FLOAT;
		case Vector4fCount_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case StartRegister_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case pConstantData_PixelShader:
			return NOD_Plugtype::AQU_PNT_FLOAT;
		case Vector4fCount_PixelShader:
			return NOD_Plugtype::AQU_UINT;
	}
#endif
	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* MatrixModifier::GetOutputPointer(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
		case eDrawingSide:
			return (void*)&m_eCurrentRenderingSide;
			break;
		case ppActiveConstantBuffers_DX10_VertexShader:
			break;
		case ppActiveConstantBuffers_DX11_VertexShader:
			return (void*)&m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_VertexShader];
		case ppActiveConstantBuffers_DX10_PixelShader:
			break;
		case ppActiveConstantBuffers_DX11_PixelShader:
			break;
		case dwVerifyConstantBuffers:
			return (void*)&m_dwVerifyConstantBuffers;
		case asShaderData:
			return (void*)&m_asShaders;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void MatrixModifier::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			m_ppvShaderBytecode_VertexShader = (void**)pData;
			break;
		case BytecodeLength_VertexShader:
			m_pnBytecodeLength_VertexShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_VertexShader:
			m_ppcClassLinkage_VertexShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppVertexShader_DX10:
			m_pppcVertexShader_DX10 = (ID3D10VertexShader***)pData;
			break;
		case pShaderBytecode_PixelShader:
			m_ppvShaderBytecode_PixelShader = (void**)pData;
			break;
		case BytecodeLength_PixelShader:
			m_pnBytecodeLength_PixelShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_PixelShader:
			m_ppcClassLinkage_PixelShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppPixelShader_DX10:
			m_pppcPixelShader_DX10 = (ID3D10PixelShader***)pData;
			break;
		case pVertexShader_10:
			m_ppcVertexShader_10 = (ID3D10VertexShader**)pData;
			break;
		case pVertexShader_11:
			m_ppcVertexShader_11 = (ID3D11VertexShader**)pData;
			break;
		case pPixelShader_10:
			m_ppcPixelShader_10 = (ID3D10PixelShader**)pData;
			break;
		case pPixelShader_11:
			m_ppcPixelShader_11 = (ID3D11VertexShader**)pData;
			break;
		case pDesc_DX10:
			m_ppsDesc_DX10 = (D3D10_BUFFER_DESC**)pData;
			break;
		case pInitialData_DX10:
			m_ppsInitialData_DX10 = (D3D10_SUBRESOURCE_DATA**)pData;
			break;
		case ppBuffer_DX10:
			m_pppcBuffer_DX10 = (ID3D10Buffer***)pData;
			break;
		case StartSlot_VertexShader:
			m_pdwStartSlot_VertexShader = (UINT*)pData;
			break;
		case NumBuffers_VertexShader:
			m_pdwNumBuffers_VertexShader = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_VertexShader:
			m_pppcConstantBuffers_DX10_VertexShader = (ID3D10Buffer***)pData;
			break;
		case ppConstantBuffers_DX11_VertexShader:
			m_pppcConstantBuffers_DX11_VertexShader = (ID3D11Buffer***)pData;
			break;
		case StartSlot_PixelShader:
			m_pdwStartSlot_PixelShader = (UINT*)pData;
			break;
		case NumBuffers_PixelShader:
			m_pdwNumBuffers_PixelShader = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_PixelShader:
			m_pppcConstantBuffers_DX10_PixelShader = (ID3D10Buffer***)pData;
			break;
		case ppConstantBuffers_DX11_PixelShader:
			m_pppcConstantBuffers_DX11_PixelShader = (ID3D11Buffer***)pData;
			break;
		case pDstResource_DX10:
			m_ppcDstResource_DX10 = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11:
			m_ppcDstResource_DX11 = (ID3D11Resource**)pData;
			break;
		case DstSubresource:
			m_pdwDstSubresource = (UINT*)pData;
			break;
		case pDstBox_DX10:
			m_ppsDstBox_DX10 = (D3D10_BOX**)pData;
			break;
		case pDstBox_DX11:
			m_ppsDstBox_DX11 = (D3D11_BOX**)pData;
			break;
		case pSrcData:
			m_ppvSrcData = (void**)pData;
			break;
		case SrcRowPitch:
			m_pdwSrcRowPitch = (UINT*)pData;
			break;
		case SrcDepthPitch:
			m_pdwSrcDepthPitch = (UINT*)pData;
			break;
		case pDstResource_DX10_Copy:
			m_ppcDstResource_DX10_Copy = (ID3D10Resource**)pData;
			break;
		case pSrcResource_DX10_Copy:
			m_ppcSrcResource_DX10_Copy = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11_Copy:
			m_ppcDstResource_DX11_Copy = (ID3D11Resource**)pData;
			break;
		case pSrcResource_DX11_Copy:
			m_ppcSrcResource_DX11_Copy = (ID3D11Resource**)pData;
			break;
		case pDstResource_DX10_CopySub:
			m_ppcDstResource_DX10_CopySub = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11_CopySub:
			m_ppcDstResource_DX11_CopySub = (ID3D11Resource**)pData;
			break;
		case DstSubresource_CopySub:
			m_pdwDstSubresource_CopySub = (UINT*)pData;
			break;
		case DstX:
			m_pdwDstX = (UINT*)pData;
			break;
		case DstY:
			m_pdwDstY = (UINT*)pData;
			break;
		case DstZ:
			m_pdwDstZ = (UINT*)pData;
			break;
		case pSrcResource_DX10_CopySub:
			m_ppcSrcResource_DX10_CopySub = (ID3D10Resource**)pData;
			break;
		case pSrcResource_DX11_CopySub:
			m_ppcSrcResource_DX11_CopySub = (ID3D11Resource**)pData;
			break;
		case SrcSubresource:
			m_pdwSrcSubresource = (UINT*)pData;
			break;
		case pSrcBox_DX10:
			m_ppsSrcBox_DX10 = (D3D10_BOX**)pData;
			break;
		case pSrcBox_DX11:
			m_ppsSrcBox_DX11 = (D3D11_BOX**)pData;
			break;
		case StartSlot_Get_VertexShader:
			m_pdwStartSlot_VertexShader_Get = (UINT*)pData;
			break;
		case NumBuffers_Get_VertexShader:
			m_pdwNumBuffers_VertexShader_Get = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_Get_VertexShader:
			break;
		case ppConstantBuffers_DX11_Get_VertexShader:
			m_pppcConstantBuffers_VertexShader = (ID3D11Buffer***)pData;
			break;
		case StartSlot_Get_PixelShader:
			m_pdwStartSlot_PixelShader_Get = (UINT*)pData;
			break;
		case NumBuffers_Get_PixelShader:
			m_pdwNumBuffers_PixelShader_Get = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_Get_PixelShader:
			break;
		case ppConstantBuffers_DX11_Get_PixelShader:
			m_pppcConstantBuffers_PixelShader = (ID3D11Buffer***)pData;
			break;
		case pResource:
			m_ppcResource_Map = (ID3D11Resource**)pData;
			break;
		case Subresource:
			m_pdwSubresource_Map = (UINT*)pData;
			break;
		case MapType:
			m_psMapType = (D3D11_MAP*)pData;
			break;
		case MapFlags:
			m_pdwMapFlags = (UINT*)pData;
			break;
		case pMappedResource:
			m_ppsMappedResource = (D3D11_MAPPED_SUBRESOURCE**)pData;
			break;
		case pResource_Unmap:
			m_ppcResource_Unmap = (ID3D11Resource**)pData;
			break;
		case Subresource_Unmap:
			m_pdwSubresource_Unmap = (UINT*)pData;
			break;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			m_ppcShader_Vertex = (IDirect3DVertexShader9**)pData;
			break;
		case pShader_Pixel:
			m_ppcShader_Pixel = (IDirect3DPixelShader9**)pData;
			break;
		case State:
			m_psState = (D3DTRANSFORMSTATETYPE*)pData;
			break;
		case pMatrix:
			m_ppsMatrix = (D3DMATRIX**)pData;
			break;
		case State_Multiply:
			m_psState_Multiply = (D3DTRANSFORMSTATETYPE*)pData;
			break;
		case pMatrix_Multiply:
			m_ppsMatrix_Multiply = (D3DMATRIX**)pData;
			break;
		case StartRegister_VertexShader:
			m_pdwStartRegister_VertexShader = (UINT*)pData;
			break;
		case pConstantData_VertexShader:
			m_ppfConstantData_VertexShader = (float**)pData;
			break;
		case Vector4fCount_VertexShader:
			m_pdwVector4fCount_VertexShader = (UINT*)pData;
			break;
		case StartRegister_PixelShader:
			m_pdwStartRegister_PixelShader = (UINT*)pData;
			break;
		case pConstantData_PixelShader:
			m_ppfConstantData_PixelShader = (float**)pData;
			break;
		case Vector4fCount_PixelShader:
			m_pdwVector4fCount_PixelShader = (UINT*)pData;
			break;
	}
#endif
}

/**
* Matrix Modifier supports various D3D10 + D3D11 calls.
***/
bool MatrixModifier::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_10_1))
	{
		if (nD3DInterface == INTERFACE_ID3D10DEVICE)
		{

		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{

		}
	}
	else if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_11) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_11_2))
	{
		if (nD3DInterface == INTERFACE_ID3D11DEVICECONTEXT)
		{
			if ((nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSSETSHADER) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_MAP) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_UNMAP) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS))
				return true;
		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{

		}
	}
#elif defined(VIREIO_D3D9)
#endif
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
* Main entry point.
***/
void* MatrixModifier::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch (eD3DInterface)
	{
		case INTERFACE_ID3D11DEVICECONTEXT:
			switch (eD3DMethod)
			{
#pragma region ID3D11DeviceContext::UpdateSubresource
				case METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE:
					// verify pointers
					if (!m_ppcDstResource_DX11) return nullptr;
					if (!m_pdwDstSubresource) return nullptr;
					if (!m_ppsDstBox_DX11) return nullptr;
					if (!m_ppvSrcData) return nullptr;
					if (!m_pdwSrcRowPitch) return nullptr;
					if (!m_pdwSrcDepthPitch) return nullptr;
					if (!*m_ppcDstResource_DX11) return nullptr;
					if (!*m_ppvSrcData) return nullptr;

					// is this a buffer ?
					D3D11_RESOURCE_DIMENSION eDimension;
					(*m_ppcDstResource_DX11)->GetType(&eDimension);
					if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
					{
						// is this a constant buffer ?
						D3D11_BUFFER_DESC sDesc;
						((ID3D11Buffer*)*m_ppcDstResource_DX11)->GetDesc(&sDesc);
						if ((sDesc.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
						{
							// set constants as private data here
							((ID3D11Buffer*)*m_ppcDstResource_DX11)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDesc.ByteWidth, *m_ppvSrcData);

							// loop through active buffers, if this one is active update stereo buffers
							for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
							if (m_apcActiveConstantBuffers11[dwIndex])
							{
								if (*m_ppcDstResource_DX11 == m_apcActiveConstantBuffers11[dwIndex])
								{
									//  and update the stereo buffers
									if ((sDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
										UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, *m_ppvSrcData, 0, 0, dwIndex, sDesc.ByteWidth, true);
									else
										UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, *m_ppvSrcData, 0, 0, dwIndex, sDesc.ByteWidth, false);
								}
							}
						}
					}
					else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
					{
						// get the stereo twin of the resource (texture)
						ID3D11Resource* pcResourceTwin = nullptr;
						UINT dwSize = sizeof(pcResourceTwin);
						((ID3D11Resource*)*m_ppcDstResource_DX11)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
						if (pcResourceTwin)
						{
							// update stereo twin
							((ID3D11DeviceContext*)pThis)->UpdateSubresource(pcResourceTwin, *m_pdwDstSubresource, *m_ppsDstBox_DX11, *m_ppvSrcData, *m_pdwSrcRowPitch, *m_pdwSrcDepthPitch);
							pcResourceTwin->Release();
						}

					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopySubresourceRegion
				case METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION:
					if (!m_ppcDstResource_DX11_CopySub) return nullptr;
					if (!m_pdwDstSubresource_CopySub) return nullptr;
					if (!m_pdwDstX) return nullptr;
					if (!m_pdwDstY) return nullptr;
					if (!m_pdwDstZ) return nullptr;
					if (!m_ppcSrcResource_DX11_CopySub) return nullptr;
					if (!m_pdwSrcSubresource) return nullptr;
					if (!m_ppsSrcBox_DX11) return nullptr;
					if (!*m_ppcDstResource_DX11_CopySub) return nullptr;
					if (!*m_ppcSrcResource_DX11_CopySub) return nullptr;
					{
						// get destination resource type
						D3D11_RESOURCE_DIMENSION eDimension;
						(*m_ppcDstResource_DX11_CopySub)->GetType(&eDimension);

						// if buffer, get desc
						if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
						{
							if (!*m_ppsSrcBox_DX11) return nullptr;

							D3D11_BUFFER_DESC sDescDst;
							((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->GetDesc(&sDescDst);

							// if constant buffer, continue
							if ((sDescDst.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
							{
								// get description from source buffer
								D3D11_BUFFER_DESC sDescSrc;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_CopySub)->GetDesc(&sDescSrc);

								// get private data from source buffer
								UINT dwSize = sDescSrc.ByteWidth;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_CopySub)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								if (dwSize)
								{
									// get (byte) offsets and length
									UINT dwOffsetSrc = (*m_ppsSrcBox_DX11)->left;
									UINT dwOffsetDst = (*m_pdwDstX);
									UINT dwLength = (*m_ppsSrcBox_DX11)->right;

									// return if out of bounds
									if ((dwOffsetSrc + dwLength) > sDescSrc.ByteWidth) return nullptr;
									if ((dwOffsetDst + dwLength) > sDescDst.ByteWidth) return nullptr;

									// get private data from destination buffer
									dwSize = sDescDst.ByteWidth;
									((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11Temp);

									// clear buffer if no private data present
									if (!dwSize) ZeroMemory(m_pchBuffer11Temp, sDescDst.ByteWidth);

									// perform memory copy
									memcpy(&m_pchBuffer11Temp[dwOffsetDst], &m_pchBuffer11[dwOffsetSrc], dwLength);

									// set back private data for the destination buffer
									((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDescDst.ByteWidth, m_pchBuffer11Temp);

									// loop through active buffers, if this one is active update stereo buffers
									for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
									if (m_apcActiveConstantBuffers11[dwIndex])
									{
										if (*m_ppcDstResource_DX11_CopySub == m_apcActiveConstantBuffers11[dwIndex])
										{
											//  and update the stereo buffers
											if ((sDescDst.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11Temp, 0, 0, dwIndex, sDescDst.ByteWidth, true);
											else
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11Temp, 0, 0, dwIndex, sDescDst.ByteWidth, false);
										}
									}
								}
							}
						}
						// is this a texture ?
						else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						{
							// get the stereo twin of the resource (texture)
							ID3D11Resource* pcResourceTwinSrc = nullptr;
							UINT dwSize = sizeof(pcResourceTwinSrc);
							((ID3D11Resource*)*m_ppcSrcResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
							if (pcResourceTwinSrc)
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopySubresourceRegion(pcResourceTwinDst,
										*m_pdwDstSubresource_CopySub,
										*m_pdwDstX,
										*m_pdwDstY,
										*m_pdwDstZ,
										pcResourceTwinSrc,
										*m_pdwSrcSubresource,
										*m_ppsSrcBox_DX11);

									pcResourceTwinDst->Release();
								}
								else
								{
									// set private data "create new" BOOL
									BOOL bTrue = TRUE;
									((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
								}
								pcResourceTwinSrc->Release();
							}
							else
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopySubresourceRegion(pcResourceTwinDst,
										*m_pdwDstSubresource_CopySub,
										*m_pdwDstX,
										*m_pdwDstY,
										*m_pdwDstZ,
										*m_ppcSrcResource_DX11_CopySub,
										*m_pdwSrcSubresource,
										*m_ppsSrcBox_DX11);

									pcResourceTwinDst->Release();
								}
							}
						}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopyResource
				case METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE:
					if (!m_ppcDstResource_DX11_Copy) return nullptr;
					if (!m_ppcSrcResource_DX11_Copy) return nullptr;
					if (!*m_ppcDstResource_DX11_Copy) return nullptr;
					if (!*m_ppcSrcResource_DX11_Copy) return nullptr;
					{
						// get destination resource type
						D3D11_RESOURCE_DIMENSION eDimension;
						(*m_ppcDstResource_DX11_Copy)->GetType(&eDimension);

						// if buffer, get desc
						if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
						{
							D3D11_BUFFER_DESC sDescDst;
							((ID3D11Buffer*)*m_ppcDstResource_DX11_Copy)->GetDesc(&sDescDst);

							// if constant buffer, continue
							if ((sDescDst.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
							{
								// get description from source buffer
								D3D11_BUFFER_DESC sDescSrc;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_Copy)->GetDesc(&sDescSrc);

								// if source size not equal to destination size, return
								if (sDescSrc.ByteWidth != sDescDst.ByteWidth) return nullptr;

								// get private data from source buffer
								UINT dwSize = sDescSrc.ByteWidth;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_Copy)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								if (dwSize)
								{
									// set private data for the destination buffer
									((ID3D11Buffer*)*m_ppcDstResource_DX11_Copy)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDescDst.ByteWidth, m_pchBuffer11);

									// loop through active buffers, if this one is active update stereo buffers
									for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
									if (m_apcActiveConstantBuffers11[dwIndex])
									{
										if (*m_ppcDstResource_DX11_Copy == m_apcActiveConstantBuffers11[dwIndex])
										{
											//  and update the stereo buffers
											if ((sDescDst.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11, 0, 0, dwIndex, sDescDst.ByteWidth, true);
											else
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11, 0, 0, dwIndex, sDescDst.ByteWidth, false);
										}
									}
								}
							}
						}
						// is this a texture ?
						else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						{
							// get the stereo twin of the resource (texture)
							ID3D11Resource* pcResourceTwinSrc = nullptr;
							UINT dwSize = sizeof(pcResourceTwinSrc);
							((ID3D11Resource*)*m_ppcSrcResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
							if (pcResourceTwinSrc)
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopyResource(pcResourceTwinDst, pcResourceTwinSrc);
									pcResourceTwinDst->Release();
								}
								else
								{
									// set private data "create new" BOOL
									BOOL bTrue = TRUE;
									((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
								}
								pcResourceTwinSrc->Release();
							}
							else
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopyResource(pcResourceTwinDst, *m_ppcSrcResource_DX11_Copy);
									pcResourceTwinDst->Release();
								}
							}
						}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetShader
				case METHOD_ID3D11DEVICECONTEXT_VSSETSHADER:
					if (!m_ppcVertexShader_11)
					{
						m_pcActiveVertexShader11 = nullptr;
						return nullptr;
					}
					if (!*m_ppcVertexShader_11)
					{
						m_pcActiveVertexShader11 = nullptr;
						return nullptr;
					}
					else
					{
						// set the active vertex shader
						m_pcActiveVertexShader11 = *m_ppcVertexShader_11;

						// loop through active constant buffers, get private data and update them accordingly to the new shader
						for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
						if (m_apcActiveConstantBuffers11[dwIndex])
						{
							// get description from buffer
							D3D11_BUFFER_DESC sDesc;
							m_apcActiveConstantBuffers11[dwIndex]->GetDesc(&sDesc);

							// get private data from buffer
							UINT dwSize = sDesc.ByteWidth;
							m_apcActiveConstantBuffers11[dwIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

							//  and update the stereo buffers
							if (dwSize)
							{
								if ((sDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
									UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwIndex, sDesc.ByteWidth, true);
								else
									UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwIndex, sDesc.ByteWidth, false);
							}
						}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS:
					if (!m_pdwStartSlot_VertexShader) return nullptr;
					if (!m_pdwNumBuffers_VertexShader) return nullptr;
					if (!m_pppcConstantBuffers_DX11_VertexShader) return nullptr;
					if (!*m_pppcConstantBuffers_DX11_VertexShader) return nullptr;
					if (!**m_pppcConstantBuffers_DX11_VertexShader) return nullptr;

					// loop through the new buffers
					for (UINT dwIndex = 0; dwIndex < *m_pdwNumBuffers_VertexShader; dwIndex++)
					{
						// get internal index
						UINT dwInternalIndex = dwIndex + *m_pdwStartSlot_VertexShader;

						// in range ? 
						if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
						{
							// set buffer internally 
							m_apcActiveConstantBuffers11[dwInternalIndex] = ((*m_pppcConstantBuffers_DX11_VertexShader)[dwIndex]);

							if (m_apcActiveConstantBuffers11[dwInternalIndex])
							{
								// get private data from buffer
								D3D11_BUFFER_DESC sDesc;
								m_apcActiveConstantBuffers11[dwInternalIndex]->GetDesc(&sDesc);
								UINT dwSize = sDesc.ByteWidth;
								m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								//  and update the stereo buffers
								if (dwSize)
								{
									if ((sDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
										UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwInternalIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwInternalIndex, sDesc.ByteWidth, true);
									else
										UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwInternalIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwInternalIndex, sDesc.ByteWidth, false);
								}
							}
						}
					}

					{
						// get a copy of the active buffers
						std::vector<ID3D11Buffer*> apcActiveConstantBuffers11 = std::vector<ID3D11Buffer*>(m_apcActiveConstantBuffers11);

						// loop through the new buffers
						for (UINT dwIndex = 0; dwIndex < *m_pdwNumBuffers_VertexShader; dwIndex++)
						{
							// get internal index
							UINT dwInternalIndex = dwIndex + *m_pdwStartSlot_VertexShader;

							// in range ? 
							if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
							{
								if ((m_apcActiveConstantBuffers11)[dwInternalIndex])
								{
									// get the private data interfaces and set the current drawing side
									ID3D11Buffer* pcBuffer = nullptr;
									UINT dwSize = sizeof(pcBuffer);
									if (m_eCurrentRenderingSide == RenderPosition::Left)
										m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSize, (void*)&pcBuffer);
									else
										m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

									if (pcBuffer)
									{
										apcActiveConstantBuffers11[dwInternalIndex] = pcBuffer;

										// "If the data returned is a pointer to an IUnknown, or one of its derivative classes,
										// previously set by IDXGIObject::SetPrivateDataInterface, you must call::Release() on 
										// the pointer before the pointer is freed to decrement the reference count." (Microsoft)
										pcBuffer->Release();
									}
								}
							}
						}

						// call super method
						((ID3D11DeviceContext*)pThis)->VSSetConstantBuffers(*m_pdwStartSlot_VertexShader,
							*m_pdwNumBuffers_VertexShader,
							(ID3D11Buffer**)&apcActiveConstantBuffers11[*m_pdwStartSlot_VertexShader]);

						// method replaced, immediately return
						nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSGetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS:
					// currently, we set the main buffers to avoid that the game gets the 
					// stereo buffers assioziated with the main buffers as private data interfaces.
					// if there is a game that flickers (should not be) we need to replace the whole
					// method (using AQU_PluginFlags::ImmediateReturnFlag)
					((ID3D11DeviceContext*)pThis)->VSSetConstantBuffers(0,
						D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,
						(ID3D11Buffer**)&m_apcActiveConstantBuffers11[0]);
					break;
#pragma endregion
#pragma region ID3D11DeviceContext::PSGetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS:
					// ID3D11DeviceContext::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
					OutputDebugString(L"METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS");
					// return a poiner to the active constant buffers
					break;
#pragma endregion
#pragma region ID3D11DeviceContext::Map
				case METHOD_ID3D11DEVICECONTEXT_MAP:
					if (!m_ppcResource_Map) return nullptr;
					if (!m_pdwSubresource_Map) return nullptr;
					if (!m_psMapType) return nullptr;
					if (!m_pdwMapFlags) return nullptr;
					if (!m_ppsMappedResource) return nullptr;
					if (!*m_ppcResource_Map) return nullptr;
					if (!*m_ppsMappedResource) return nullptr;
					{
						// get destination resource type
						D3D11_RESOURCE_DIMENSION eDimension;
						(*m_ppcResource_Map)->GetType(&eDimension);

						// if buffer, get desc
						if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
						{
							D3D11_BUFFER_DESC sDescDst;
							((ID3D11Buffer*)*m_ppcResource_Map)->GetDesc(&sDescDst);

							// if constant buffer, continue
							if ((sDescDst.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
							{
								// get an index which is zero 
								UINT dwIndex = m_dwMappedBuffers;
								bool bFound = false;
								for (UINT dwI = 0; dwI < (UINT)m_asMappedBuffers.size(); dwI++)
								{
									// empty resource index ?
									if ((!m_asMappedBuffers[dwI].m_pcMappedResource) && (!bFound))
									{
										dwIndex = dwI;
										bFound = true;
									}
								}

								// do the map call..
								*(HRESULT*)m_pvReturn = ((ID3D11DeviceContext*)pThis)->Map(*m_ppcResource_Map, *m_pdwSubresource_Map, *m_psMapType, *m_pdwMapFlags, *m_ppsMappedResource);

								// succeeded ?
								if (*(HRESULT*)m_pvReturn == S_OK)
								{
									// resize the vector if necessary
									if (m_dwMappedBuffers >= (UINT)m_asMappedBuffers.size())
										m_asMappedBuffers.resize(m_dwMappedBuffers + 1);

									// set the mapped fields
									m_asMappedBuffers[dwIndex].m_pcMappedResource = *m_ppcResource_Map;
									m_asMappedBuffers[dwIndex].m_psMappedResource = *m_ppsMappedResource;
									m_asMappedBuffers[dwIndex].m_pMappedResourceData = (**m_ppsMappedResource).pData;
									m_asMappedBuffers[dwIndex].m_dwMappedResourceDataSize = sDescDst.ByteWidth;
									m_asMappedBuffers[dwIndex].m_eMapType = *m_psMapType;
									m_asMappedBuffers[dwIndex].m_dwMapFlags = *m_pdwMapFlags;

									// make buffer address homogenous
									UINT_PTR dwAddress = (UINT_PTR)m_asMappedBuffers[dwIndex].m_pchBuffer11;
									dwAddress |= 0xff; dwAddress++;
									(**m_ppsMappedResource).pData = (LPVOID)dwAddress;

									// update number of mapped Buffers
									m_dwMappedBuffers++;
								}

								// method replaced, immediately return
								nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
								return m_pvReturn;
							}
						}
					}
					break;
#pragma endregion
#pragma region ID3D11DeviceContext::Unmap
				case METHOD_ID3D11DEVICECONTEXT_UNMAP:
					if (!m_ppcResource_Unmap) return nullptr;
					if (!m_pdwSubresource_Unmap) return nullptr;
					if (!*m_ppcResource_Unmap) return nullptr;
					{
						// buffers mapped actually ?
						if (m_dwMappedBuffers)
						{
							// loop through all possibly mapped constant buffers
							for (UINT dwI = 0; dwI < (UINT)m_asMappedBuffers.size(); dwI++)
							{
								// continue only if mapped resource data present and the resource pointer matches the stored pointer
								if ((m_asMappedBuffers[dwI].m_pcMappedResource) && (*m_ppcResource_Unmap == m_asMappedBuffers[dwI].m_pcMappedResource))
								{
									// get homogenous address
									UINT_PTR dwAddress = (UINT_PTR)m_asMappedBuffers[dwI].m_pchBuffer11;
									dwAddress |= 0xff; dwAddress++;

									// copy the stored data... 
									if (TO_DO_ADD_BOOL_HERE_TRUE)
										memcpy(m_asMappedBuffers[dwI].m_pMappedResourceData, (LPVOID)dwAddress, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize);

									// do the unmap call..
									((ID3D11DeviceContext*)pThis)->Unmap(*m_ppcResource_Unmap, *m_pdwSubresource_Unmap);

									// set new data as private data
									(*m_ppcResource_Unmap)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize, (LPVOID)dwAddress);

									// loop through active buffers, if this one is active update stereo buffers
									for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
									if (m_apcActiveConstantBuffers11[dwIndex])
									{
										if (*m_ppcResource_Unmap == m_apcActiveConstantBuffers11[dwIndex])
										{
											//  and update the stereo buffers
											UpdateConstantBuffer((ID3D11DeviceContext*)pThis, *m_ppcResource_Unmap, 0, NULL, (LPVOID)dwAddress, 0, 0, dwIndex, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize, true);
										}
									}

									// set mapped resource data to zero
									m_asMappedBuffers[dwI].m_pcMappedResource = nullptr;
									m_asMappedBuffers[dwI].m_pMappedResourceData = nullptr;
									m_asMappedBuffers[dwI].m_dwMappedResourceDataSize = 0;
									m_asMappedBuffers[dwI].m_psMappedResource = nullptr;

									// update number of mapped Buffers
									m_dwMappedBuffers--;

									// method replaced, immediately return
									nProvokerIndex |= AQU_PluginFlags::ImmediateReturnFlag;
									return nullptr;
								}
							}
						}
					}
					break;
#pragma endregion
			}
			return nullptr;
#pragma region ID3D10Device::VSGetConstantBuffers
			// ID3D10Device::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
#pragma endregion
#pragma region ID3D10Device::PSGetConstantBuffers
			// ID3D10Device::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
#pragma endregion
	}
#elif defined(VIREIO_D3D9)
#endif

	return nullptr;
}

/**
* There's some windows event on our node.
***/
void MatrixModifier::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// multiply mouse coords by 4 due to Aquilinus workspace architecture
	Vireio_GUI_Event sEvent = m_pcVireioGUI->WindowsEvent(msg, wParam, lParam, 4);

	switch (sEvent.eType)
	{
		case NoEvent:
			break;
		case ChangedToNext:
			break;
		case ChangedToPrevious:
			break;
		case ChangedToValue:
			break;
		case ChangedToText:
			break;
		case Pressed:
			if (sEvent.dwIndexOfPage == m_adwPageIDs[GUI_Pages::DebugPage])
			{
				if (sEvent.dwIndexOfControl == m_dwDebugGrab)
				{
					// set the grab debug bool to true
					m_bGrabDebug = true;
				}
				else if (sEvent.dwIndexOfControl == m_dwClearDebug)
				{
					// clear debug string list
					m_aszDebugTrace = std::vector<std::wstring>();
				}
				else if (sEvent.dwIndexOfControl == m_dwDebugSpin)
				{
					// new debug option chosen
					m_eDebugOption = (Debug_Grab_Options)sEvent.dwNewValue;
				}
			}
			break;
		default:
			break;
	}
}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
/**
* Modifies all constants and updates the stereo constant buffer.
***/
void MatrixModifier::UpdateConstantBuffer(ID3D11DeviceContext* pcContext, ID3D11Resource *pcDstResource, UINT dwDstSubresource, const D3D11_BOX *psDstBox, const void *pvSrcData, UINT dwSrcRowPitch, UINT dwSrcDepthPitch, UINT dwBufferIndex, UINT dwBufferSize, bool bMapBuffer)
{
	// first get the current shader data
	Vireio_Shader_Private_Data sPrivateData;
	UINT dwDataSize = sizeof(sPrivateData);
	if (m_pcActiveVertexShader11)
		m_pcActiveVertexShader11->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
	else return;

	// no private data ? in this case the active shader was created
	// before the Vireio profile injected... get shader data from
	// the buffer (actually set in StereoSplitterDx10->SetDrawingSide() )
	if ((!dwDataSize) && (TO_DO_ADD_BOOL_HERE_TRUE))
	{
		dwDataSize = sizeof(sPrivateData);
		pcDstResource->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

		if (dwDataSize)
		{
			// has this shader relevant information about this buffer ?
			if (dwBufferIndex >= m_asShaders[sPrivateData.dwIndex].asBuffers.size())
			{
				// this shader has no constant buffer data ! clear shader data for this constant buffer
				pcDstResource->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, 0, 0);

				// should i automatically assigne the shader hash from the buffer ?
				if (TO_DO_ADD_BOOL_HERE_TRUE)
				{
					m_pcActiveVertexShader11->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, 0, nullptr);
				}
			}
			else
				// should i automatically assigne the shader hash from the buffer ?
			if (TO_DO_ADD_BOOL_HERE_TRUE)
			{
				m_pcActiveVertexShader11->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);
			}
		}
	}

	// get private data interfaces (stereo constant buffers)
	ID3D11Buffer* pcBufferLeft = nullptr;
	ID3D11Buffer* pcBufferRight = nullptr;

	// get the stereo private data interfaces
	UINT dwSizeLeft = sizeof(pcBufferLeft);
	UINT dwSizeRight = sizeof(pcBufferRight);
	pcDstResource->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSizeLeft, &pcBufferLeft);
	pcDstResource->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSizeRight, &pcBufferRight);

	// copy the data to left and right constant buffer data buffers
	memcpy(m_pchBuffer11Left, pvSrcData, dwBufferSize);
	memcpy(m_pchBuffer11Right, pvSrcData, dwBufferSize);

	// private data present ?
	if (dwDataSize)
	{
		// get the register size of the buffer
		UINT dwBufferRegisterSize = dwBufferSize >> 5;
		
		// constant rules addressed ? 
		INT nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

		// shader has this buffer index ?
		if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
			// get the new rule index
			nRulesIndex = m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].nConstantRulesIndex;
		// shader has this unaccounted buffer index ?
		if ((nRulesIndex == VIREIO_CONSTANT_RULES_NOT_ADDRESSED) && (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted.size()))
			// get the new rule index
			nRulesIndex = m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwBufferIndex].nConstantRulesIndex;
			
		// still not addressed ? address
		if (nRulesIndex == VIREIO_CONSTANT_RULES_NOT_ADDRESSED)
		{
			// create a vector for this constant buffer
			std::vector<Vireio_Constant_Rule_Index> asConstantBufferRules = std::vector<Vireio_Constant_Rule_Index>();

			// loop through all global rules
			for (UINT dwI = 0; dwI < (UINT)m_adwGlobalConstantRuleIndices.size(); dwI++)
			{
				// get a bool for each register index, set to false
				std::vector<bool> abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);

				// use name ?
				if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseName)
				{
					// shader has this buffer index ?
					if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
					{
						// loop through the shader constants
						for (UINT nConstant = 0; nConstant < (UINT)m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
						{
							// test full name matching
							if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_szConstantName.compare(m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName) == 0)
							{
								// set this register to 'false' if not matching
								UINT dwRegister = m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5;
								if (dwRegister <= dwBufferRegisterSize)
									abRegistersMatching[m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5] = true;
							}
						}
					}
				}

				// use partial name ?
				if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseName)
				{
					// shader has this buffer index ?
					if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
					{
						// loop through the shader constants
						for (UINT nConstant = 0; nConstant < (UINT)m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
						{
							// test partial name matching
							if (std::strstr(m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_szConstantName.c_str(), m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName))
							{
								// set this register to 'false' if not matching
								UINT dwRegister = m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5;
								if (dwRegister <= dwBufferRegisterSize)
									abRegistersMatching[dwRegister] = true;
							}
						}
					}
				}

				// use start reg index ?
				if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseStartRegIndex)
				{
					UINT dwRegister = m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_dwStartRegIndex;
					if (dwRegister <= dwBufferRegisterSize)
					{
						bool bOld = abRegistersMatching[dwRegister];
						abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
						abRegistersMatching[dwRegister] = bOld;

						// set to true if no naming convention 
						if ((!m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseName) && (!m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUsePartialNameMatch))
							abRegistersMatching[dwRegister] = true;
					}
					else
						abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
				}

				// use buffer index
				if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseBufferIndex)
				{
					if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_dwBufferIndex != dwBufferIndex)
						abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
				}

				// use buffer size
				if (m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_bUseBufferSize)
				{
					if ((m_asConstantRules[m_adwGlobalConstantRuleIndices[dwI]].m_dwBufferSize << 5) != dwBufferSize)
						abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
				}

				// loop through registers and create the rules
				for (UINT dwJ = 0; dwJ < dwBufferRegisterSize; dwJ++)
				{
					// register matches the rule ?
					if (abRegistersMatching[dwJ])
					{
						Vireio_Constant_Rule_Index sIndex;
						sIndex.m_dwIndex = m_adwGlobalConstantRuleIndices[dwI];
						sIndex.m_dwConstantRuleRegister = dwJ;
						asConstantBufferRules.push_back(sIndex);
					}
				}
			}

			// look wether these rules are already present, otherwise add
			bool bPresent = false;
			for (UINT dwI = 0; dwI < (UINT)m_aasConstantBufferRuleIndices.size(); dwI++)
			{
				// size equals ?
				if (m_aasConstantBufferRuleIndices[dwI].size() == asConstantBufferRules.size())
				{
					// test all constants
					UINT dwCount = 0;
					for (UINT dwJ = 0; dwJ < (UINT)m_aasConstantBufferRuleIndices[dwI].size(); dwJ++)
					{
						if ((m_aasConstantBufferRuleIndices[dwI][dwJ].m_dwConstantRuleRegister == asConstantBufferRules[dwJ].m_dwConstantRuleRegister) &&
							(m_aasConstantBufferRuleIndices[dwI][dwJ].m_dwIndex == asConstantBufferRules[dwJ].m_dwIndex))
							dwCount++;
					}
					if ((dwCount) && (dwCount == (UINT)asConstantBufferRules.size()))
					{
						// set existing constant rule indices
						nRulesIndex = (INT)dwI;
						if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
							m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].nConstantRulesIndex = (INT)dwI;
						if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted.size())
							m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwBufferIndex].nConstantRulesIndex = (INT)dwI;
						bPresent = true;
					}
				}
			}

			// no rules found ? set to unavailable
			if (!asConstantBufferRules.size())
			{
				nRulesIndex = VIREIO_CONSTANT_RULES_NOT_AVAILABLE;
				if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
					m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].nConstantRulesIndex = nRulesIndex;
				if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted.size())
					m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwBufferIndex].nConstantRulesIndex = nRulesIndex;
			}
			// rules not present ?
			else if (!bPresent)
			{
				// set index... current vector size
				nRulesIndex = (INT)m_aasConstantBufferRuleIndices.size();
				if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
					m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].nConstantRulesIndex = nRulesIndex;
				if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted.size())
					m_asShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwBufferIndex].nConstantRulesIndex = nRulesIndex;
					
				// and add
				m_aasConstantBufferRuleIndices.push_back(asConstantBufferRules);
			}
		}
		
		// do modifications
		if (nRulesIndex >= 0)
		{
			// loop through rules for that constant buffer
			for (UINT dwI = 0; dwI < (UINT)m_aasConstantBufferRuleIndices[nRulesIndex].size(); dwI++)
			{
				UINT dwIndex = m_aasConstantBufferRuleIndices[nRulesIndex][dwI].m_dwIndex;
				UINT dwRegister = m_aasConstantBufferRuleIndices[nRulesIndex][dwI].m_dwConstantRuleRegister;

				// TODO !! CHOOSE MODIFICATION BASED ON RULE TYPE

				// is this modification in range ?
				if ((dwSizeLeft) && (dwSizeRight) && (pcBufferLeft) && (pcBufferRight) && (dwBufferSize >= dwRegister * 4 * sizeof(float)+sizeof(D3DMATRIX)))
				{
					// get pointers to the matrix (left+right)
					UINT_PTR pv = (UINT_PTR)pvSrcData + dwRegister * 4 * sizeof(float);
					UINT_PTR pvLeft = (UINT_PTR)m_pchBuffer11Left + dwRegister * 4 * sizeof(float);
					UINT_PTR pvRight = (UINT_PTR)m_pchBuffer11Right + dwRegister * 4 * sizeof(float);

					// get the matrix
					D3DXMATRIX sMatrix = D3DXMATRIX((CONST FLOAT*)pv);
					D3DXMATRIX sMatrixModified;

					if (m_asConstantRules[dwIndex].m_bTranspose) D3DXMatrixTranspose(&sMatrix, &sMatrix);

					// apply left 
					sMatrixModified = sMatrix *  m_pcShaderViewAdjustment->LeftAdjustmentMatrix()*
						m_pcShaderViewAdjustment->ProjectionInverse() * m_pcShaderViewAdjustment->PositionMatrix() * m_pcShaderViewAdjustment->Projection();
					if (m_asConstantRules[dwIndex].m_bTranspose) D3DXMatrixTranspose(&sMatrixModified, &sMatrixModified);
					memcpy((void*)pvLeft, &sMatrixModified, sizeof(D3DXMATRIX));

					// apply right
					sMatrixModified = sMatrix *  m_pcShaderViewAdjustment->RightAdjustmentMatrix()*
						m_pcShaderViewAdjustment->ProjectionInverse() * m_pcShaderViewAdjustment->PositionMatrix() * m_pcShaderViewAdjustment->Projection();
					if (m_asConstantRules[dwIndex].m_bTranspose) D3DXMatrixTranspose(&sMatrixModified, &sMatrixModified);
					memcpy((void*)pvRight, &sMatrixModified, sizeof(D3DXMATRIX));
				}
			}
		}
	}
	else
	{
		// Both Vertex Shader and Constant buffer have no private shader data !
		// set the number of frames the constant buffers have to be verified
		m_dwVerifyConstantBuffers = CONSTANT_BUFFER_VERIFICATION_FRAME_NUMBER;
#ifdef _DEBUG
		OutputDebugString(L"MatrixModifier: Both Vertex Shader and Constant buffer have no private shader data ! ");
#endif
	}

	if ((!dwSizeLeft) || (!dwSizeRight))
	{
		// create stereo buffer, first get description
		D3D11_BUFFER_DESC sDesc;
		((ID3D11Buffer*)pcDstResource)->GetDesc(&sDesc);

		// is constant buffer ? actually must be
		if ((sDesc.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
		{
			// get device
			ID3D11Device* pcDevice = nullptr;
			pcDstResource->GetDevice(&pcDevice);
			if (pcDevice)
			{
				CreateStereoConstantBuffer(pcDevice, pcContext, (ID3D11Buffer*)pcDstResource, &sDesc, NULL, true);
				pcDevice->Release();
			}
		}
		else
			OutputDebugString(L"MatrixModifier: Code error, not a constant buffer !");
	}

	// directly write to mappable buffers, otherwise call UpdateResource()
	if (bMapBuffer)
	{
		if (pcBufferLeft)
		{
			D3D11_MAPPED_SUBRESOURCE sMapped;
			if (SUCCEEDED(pcContext->Map((ID3D11Resource*)pcBufferLeft, dwDstSubresource, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &sMapped)))
			{
				memcpy(sMapped.pData, m_pchBuffer11Left, dwBufferSize);
				pcContext->Unmap((ID3D11Resource*)pcBufferLeft, dwDstSubresource);
			}
		}
		if (pcBufferRight)
		{
			D3D11_MAPPED_SUBRESOURCE sMapped;
			if (SUCCEEDED(pcContext->Map((ID3D11Resource*)pcBufferRight, dwDstSubresource, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &sMapped)))
			{
				memcpy(sMapped.pData, m_pchBuffer11Right, dwBufferSize);
				pcContext->Unmap((ID3D11Resource*)pcBufferRight, dwDstSubresource);
			}
		}
	}
	else
	{
		// finally, update the stereo constant buffers
		if (pcBufferLeft)
			pcContext->UpdateSubresource((ID3D11Resource*)pcBufferLeft, dwDstSubresource, psDstBox, m_pchBuffer11Left, dwSrcRowPitch, dwSrcDepthPitch);
		if (pcBufferRight)
			pcContext->UpdateSubresource((ID3D11Resource*)pcBufferRight, dwDstSubresource, psDstBox, m_pchBuffer11Right, dwSrcRowPitch, dwSrcDepthPitch);
	}

	// and release them
	SAFE_RELEASE(pcBufferLeft);
	SAFE_RELEASE(pcBufferRight);
}

/**
* Creates a stereo buffer out of a buffer.
* Assigns both left and right buffer to the main buffer
* as private data.
* @param pcDevice The d3d11 device.
* @param pcContext The d3d11 device context.
* @param pcBuffer The mono constant buffer to assign stereo constant buffers.
* @param pDesc Pointer to the buffer description.
* @param pInitialData Pointer to the initial data, NULL if bCopyData is true.
* @param bCopyData True if data from main buffer is to be copied to stereo buffers.
***/
void MatrixModifier::CreateStereoConstantBuffer(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11Buffer* pcBuffer, D3D11_BUFFER_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, bool bCopyData)
{
	// create left buffer
	ID3D11Buffer* pcBufferLeft = nullptr;
	if (FAILED(pcDevice->CreateBuffer(pDesc,
		pInitialData,
		&pcBufferLeft)))
		OutputDebugString(L"MatrixModifier: Failed to create left buffer!");

	// create right buffer
	ID3D11Buffer* pcBufferRight = nullptr;
	if (FAILED(pcDevice->CreateBuffer(pDesc,
		pInitialData,
		&pcBufferRight)))
		OutputDebugString(L"MatrixModifier: Failed to create right buffer!");

	// copy resource ?
	if (bCopyData)
	{
		pcContext->CopyResource(pcBufferLeft, pcBuffer);
		pcContext->CopyResource(pcBufferRight, pcBuffer);
	}

	// set as private data interface to the main buffer
	pcBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_Constant_Buffer_Left, pcBufferLeft);
	pcBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_Constant_Buffer_Right, pcBufferRight);

	// reference counter must be 1 now (reference held by the main buffer)
	ULONG nRef = pcBufferLeft->Release();
	if (nRef != 1) OutputDebugString(L"MatrixModifier: Reference counter invalid !");
	nRef = pcBufferRight->Release();
	if (nRef != 1) OutputDebugString(L"MatrixModifier: Reference counter invalid !");
}

#elif defined(VIREIO_D3D9)
#endif

/**
* Handles the debug trace.
***/
void MatrixModifier::DebugOutput(const void *pvSrcData, UINT dwShaderIndex, UINT dwBufferIndex, UINT dwBufferSize)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	INT nSelection = 0;

	// get current selection of the shader constant list
	nSelection = m_pcVireioGUI->GetCurrentSelection(m_dwShaderConstantsDebug);
	if ((nSelection < 0) || (nSelection >= (INT)m_aszShaderConstantsA.size()))
	{
		m_bGrabDebug = false;
		return;
	}

	// loop through the shader constants
	for (size_t nConstant = 0; nConstant < m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
	{
		// add the debug data by type
		switch (m_eDebugOption)
		{
			case Debug_ConstantFloat4:
				// test the name of the constant
				if (std::strstr(m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
				{
					UINT dwSize = sizeof(float)* 4;

					// is this  in range ?
					if (dwBufferSize >= (m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
					{
						// get pointers to the data
						UINT_PTR pv = (UINT_PTR)pvSrcData + m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
						D3DXVECTOR4 sVector4 = D3DXVECTOR4((CONST FLOAT*)pv);

						std::wstringstream strStream;
						strStream << L"X:" << sVector4.x << L"::Y:" << sVector4.y << L"::Z:" << sVector4.z << L"::W:" << sVector4.w;
						m_aszDebugTrace.push_back(strStream.str().c_str());
					}

					m_bGrabDebug = false;
					return;
				}
				break;
			case Debug_ConstantFloat8:
				// test the name of the constant
				if (std::strstr(m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
				{
					UINT dwSize = sizeof(float)* 4 * 2;

					// is this  in range ?
					if (dwBufferSize >= (m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
					{
						// get pointers to the data
						UINT_PTR pv = (UINT_PTR)pvSrcData + m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
						D3DXVECTOR4 sVector4 = D3DXVECTOR4((CONST FLOAT*)pv);
						pv = (UINT_PTR)pvSrcData + m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + sizeof(D3DVECTOR);
						D3DXVECTOR4 sVector4_1 = D3DXVECTOR4((CONST FLOAT*)pv);

						std::wstringstream strStream;
						strStream << L"11:" << sVector4.x << L"::12:" << sVector4.y << L"::13:" << sVector4.z << L"::14:" << sVector4.w;
						m_aszDebugTrace.push_back(strStream.str().c_str()); strStream = std::wstringstream();
						strStream << L"21:" << sVector4_1.x << L"::22:" << sVector4_1.y << L"::23:" << sVector4_1.z << L"::24:" << sVector4_1.w;
						m_aszDebugTrace.push_back(strStream.str().c_str());
					}

					m_bGrabDebug = false;
					return;
				}
				break;
			case Debug_ConstantFloat16:
				// test the name of the constant
				if (std::strstr(m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
				{
					UINT dwSize = sizeof(float)* 4 * 4;

					// is this  in range ?
					if (dwBufferSize >= (m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
					{
						// get pointers to the data
						UINT_PTR pv = (UINT_PTR)pvSrcData + m_asShaders[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
						D3DXMATRIX sMatrix = D3DXMATRIX((CONST FLOAT*)pv);

						std::wstringstream strStream;
						strStream << L"11:" << sMatrix._11 << L"::12:" << sMatrix._12 << L"::13:" << sMatrix._13 << L"::14:" << sMatrix._14;
						m_aszDebugTrace.push_back(strStream.str().c_str()); strStream = std::wstringstream();
						strStream << L"21:" << sMatrix._21 << L"::22:" << sMatrix._22 << L"::23:" << sMatrix._23 << L"::24:" << sMatrix._24;
						m_aszDebugTrace.push_back(strStream.str().c_str()); strStream = std::wstringstream();
						strStream << L"31:" << sMatrix._31 << L"::32:" << sMatrix._32 << L"::33:" << sMatrix._33 << L"::34:" << sMatrix._34;
						m_aszDebugTrace.push_back(strStream.str().c_str()); strStream = std::wstringstream();
						strStream << L"41:" << sMatrix._41 << L"::42:" << sMatrix._42 << L"::43:" << sMatrix._43 << L"::44:" << sMatrix._44;
						m_aszDebugTrace.push_back(strStream.str().c_str());
					}

					m_bGrabDebug = false;
					return;
				}
				break;
			case Debug_ConstantFloat32:
				break;
			case Debug_ConstantFloat64:
				break;
			default:
				break;
		}
	}
#elif defined(VIREIO_D3D9)
#endif
}