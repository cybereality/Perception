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

#ifndef _TRACE
#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#endif
#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define CASE_ENUM_2_WSTRING(ec, en) case ec::en: return L#en;
#ifdef TRACE_MODIFIER
#define SHOW_CALL_MODIFIER(b, name) { if (b) OutputDebugStringA(name); }
#else
#define SHOW_CALL_MODIFIER(b, name)
#endif

#include"VireioMatrixModifier.h"

/// <summary> 
/// Constructor.
/// </summary>
MatrixModifier::MatrixModifier(ImGuiContext* sCtx) : AQU_Nodus(sCtx),
m_aszShaderConstantsA(),
m_aszShaderConstants(),
m_aszVShaderHashCodes(),
m_adwVShaderHashCodes(),
m_aszPShaderHashCodes(),
m_adwPShaderHashCodes(),
m_eDebugOption(Debug_Grab_Options::Debug_ConstantFloat4),
m_bGrabDebug(false),
m_adwPageIDs(0, 0),
m_aunGlobalConstantRuleIndices(),
#if defined(VIREIO_D3D11)
m_pcContextCurrent(nullptr),
#elif defined(VIREIO_D3D9)
m_pcDeviceCurrent(nullptr),
m_asShaderSpecificRuleIndices(),
m_aszShaderRuleShaderIndices(),
m_pcActiveVertexShader(nullptr),
m_pcActivePixelShader(nullptr),
#endif
m_aasConstantBufferRuleIndices(),
m_dwCurrentChosenShaderHashCode(0),
m_sModifierData{},
m_acData(),
m_aszShaderRuleIndices(),
m_aszShaderRuleData(),
m_aszShaderRuleGeneralIndices(),
m_aszShaderConstantsCurrent(),
m_auShaderConstantsCurrentStartReg(),
m_aszShaderBuffersizes(),
m_bTrace(false)
{
	// create a new HRESULT pointer
	m_pvReturn = (void*)new HRESULT();

	// init constant rule vector, otherwise pointer to vector may not work
	m_asConstantRules = std::vector<Vireio_Constant_Modification_Rule>();

	// at start, set the view adjustment class to basic config
	ZeroMemory(&m_sGameConfiguration, sizeof(Vireio_GameConfiguration));
	m_sGameConfiguration.fWorldScaleFactor = 0.0f;
	m_sGameConfiguration.fConvergence = DEFAULT_CONVERGENCE;
	m_sGameConfiguration.fIPD = IPD_DEFAULT;
	m_sGameConfiguration.fPFOV = DEFAULT_PFOV;
	m_sGameConfiguration.fAspectMultiplier = DEFAULT_ASPECT_MULTIPLIER;
	m_sGameConfiguration.dwVRboostMinShaderCount = 0;
	m_sGameConfiguration.dwVRboostMaxShaderCount = 99999999;
#ifdef _WIN64
	m_sGameConfiguration.bIs64bit = true;
#else
	m_sGameConfiguration.bIs64bit = false;
#endif
	m_sGameConfiguration.nRollImpl = 0;
	m_sGameConfiguration.bConvergenceEnabled = false;
	m_sGameConfiguration.fYawMultiplier = DEFAULT_YAW_MULTIPLIER;
	m_sGameConfiguration.fPitchMultiplier = DEFAULT_PITCH_MULTIPLIER;
	m_sGameConfiguration.fRollMultiplier = DEFAULT_ROLL_MULTIPLIER;
	m_sGameConfiguration.fPositionMultiplier = 1.0f;
	m_sGameConfiguration.fPositionXMultiplier = DEFAULT_POS_TRACKING_X_MULT;
	m_sGameConfiguration.fPositionYMultiplier = DEFAULT_POS_TRACKING_Y_MULT;
	m_sGameConfiguration.fPositionZMultiplier = DEFAULT_POS_TRACKING_Z_MULT;
	m_sGameConfiguration.bPFOVToggle = false;
	m_pcShaderModificationCalculation = std::make_shared<ModificationCalculation>(&m_sGameConfiguration);

	// clear GUI page structures
	ZeroMemory(&m_sPageDebug, sizeof(PageDebug));
	ZeroMemory(&m_sPageGameSettings, sizeof(PageGameSettings));
	ZeroMemory(&m_sPageShader, sizeof(PageShader));
	ZeroMemory(&m_sPageGameShaderRules, sizeof(PageGameShaderRules));

	// set string entries to avoid string copy issues
	m_sPageGameShaderRules.m_szConstantName = std::string("WorldViewProj");

	// reg count to 4 on shader page by default (=Matrix), operation by default to 1 (=Simple Translate)
	m_sPageGameShaderRules.m_dwRegisterCount = 4;
	m_sPageGameShaderRules.m_dwOperationToApply = 1;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// create buffer vectors ( * 2 for left/right side 
	m_apcActiveRenderTargetViews11 = std::vector<ID3D11RenderTargetView*>(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2, nullptr);
	m_apcActiveDepthStencilView11[0] = nullptr;
	m_apcActiveDepthStencilView11[1] = nullptr;

	// set constant buffer verification at startup (first 30 frames)
	m_dwVerifyConstantBuffers = CONSTANT_BUFFER_VERIFICATION_FRAME_NUMBER;
	m_bConstantBuffersInitialized = false;

	// buffer index sizes debug to false
	m_bBufferIndexDebug = false;

	// init shader vector
	m_asVShaders = std::vector<Vireio_D3D11_Shader>();
	m_asPShaders = std::vector<Vireio_D3D11_Shader>();

	// mapped resource data
	m_asMappedBuffers = std::vector<Vireio_Map_Data>();
	m_dwMappedBuffers = 0;

	// constant rule buffer counter starts with 1 to init a first update
	m_dwConstantRulesUpdateCounter = 1;

#elif defined(VIREIO_D3D9)
	// init shader vector
	m_sModifierData.asVShaders = std::vector<Vireio_D3D9_Shader>();
	m_sModifierData.asPShaders = std::vector<Vireio_D3D9_Shader>();
	m_sModifierData.bVSActive = false;
	m_sModifierData.bPSActive = false;

	D3DXMatrixIdentity(&m_sModifierData.sMatView[0]);
	D3DXMatrixIdentity(&m_sModifierData.sMatView[1]);
	D3DXMatrixIdentity(&m_sModifierData.sMatProj[0]);
	D3DXMatrixIdentity(&m_sModifierData.sMatProj[1]);
#endif
}

/// <summary> 
/// Destructor.
/// </summary>
MatrixModifier::~MatrixModifier()
{
	m_pcShaderModificationCalculation.reset();
}

/// <summary> 
/// Return the name of the Matrix Modifier node.
/// </summary>
const char* MatrixModifier::GetNodeType()
{
#if defined(VIREIO_D3D11)
	return "Matrix Modifier Dx11";
#elif defined(VIREIO_D3D10)
	return "Matrix Modifier Dx10";
#elif defined(VIREIO_D3D9)
	return "Matrix Modifier";
#endif
}

/// <summary> 
/// Returns a global unique identifier for the Matrix Modifier node.
/// </summary>
UINT MatrixModifier::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#if defined(VIREIO_D3D11)
#define MY_PLUGIN_IDENTIFIER 77
#elif defined(VIREIO_D3D10)
#define MY_PLUGIN_IDENTIFIER 76
#elif defined(VIREIO_D3D9)
#define MY_PLUGIN_IDENTIFIER 75
#endif
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/// <summary> 
/// Returns the name of the category for the Matrix Modifier node.
/// </summary>
LPWSTR MatrixModifier::GetCategory()
{
	return L"Vireio Core";
}

/// <summary> 
/// Returns a logo to be used for the Matrix Modifier node.
/// </summary>
HBITMAP MatrixModifier::GetLogo()
{
#if defined(VIREIO_D3D11)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifierDx11.dll");
#elif  defined(VIREIO_D3D10)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifierDx10.dll");
#elif defined(VIREIO_D3D9)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifier.dll");
#endif
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/// <summary> 
/// Node data size.
/// 1) sizeof(Vireio_GameConfiguration)
/// 2) sizeof(UINT) = Number of Rules
/// 3) sizeof(Vireio_Constant_Modification_Rule_Normalized) * Number of Rules
/// 4) sizeof(UINT) = Number of General Indices
/// 5) sizeof(UINT) * Number of General Indices
/// </summary>
DWORD MatrixModifier::GetSaveDataSize()
{
	DWORD dwSizeofData = sizeof(Vireio_GameConfiguration);
	dwSizeofData += sizeof(UINT);
	dwSizeofData += (DWORD)m_asConstantRules.size() * sizeof(Vireio_Constant_Modification_Rule_Normalized);
	dwSizeofData += sizeof(UINT);
	dwSizeofData += (DWORD)m_aunGlobalConstantRuleIndices.size() * sizeof(UINT);
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	dwSizeofData += sizeof(UINT);
	dwSizeofData += (DWORD)m_aunFetchedHashCodes.size() * sizeof(UINT);
#elif defined(VIREIO_D3D9)
	dwSizeofData += sizeof(UINT);
	dwSizeofData += (DWORD)m_asShaderSpecificRuleIndices.size() * sizeof(Vireio_Hash_Rule_Index);
#endif
	return dwSizeofData;
}

/// <summary> 
/// Save the data.
/// </summary>
char* MatrixModifier::GetSaveData(UINT* pdwSizeOfData)
{
	static std::stringstream acStream;
	acStream = std::stringstream();

	// write game config
	acStream.write((char*)&m_sGameConfiguration, sizeof(Vireio_GameConfiguration));

	// number of rules
	UINT dwNumberOfRules = (UINT)m_asConstantRules.size();
	acStream.write((char*)&dwNumberOfRules, sizeof(UINT));

	// loop through rules, normalize each
	for (UINT dwI = 0; dwI < dwNumberOfRules; dwI++)
	{
		Vireio_Constant_Modification_Rule_Normalized sRule = { 0 };

		// normalize the constant string
		UINT dwStringSize = (UINT)m_asConstantRules[dwI].m_szConstantName.size();
		if (dwStringSize > 63) dwStringSize = 63;
		memcpy(sRule.m_szConstantName, &m_asConstantRules[dwI].m_szConstantName[0], dwStringSize);

		// parse the rest of the data
		sRule.m_dwBufferIndex = m_asConstantRules[dwI].m_dwBufferIndex;
		sRule.m_dwBufferSize = m_asConstantRules[dwI].m_dwBufferSize;
		sRule.m_dwStartRegIndex = m_asConstantRules[dwI].m_dwStartRegIndex;
		sRule.m_bUseName = m_asConstantRules[dwI].m_bUseName;
		sRule.m_bUsePartialNameMatch = m_asConstantRules[dwI].m_bUsePartialNameMatch;
		sRule.m_bUseBufferIndex = m_asConstantRules[dwI].m_bUseBufferIndex;
		sRule.m_bUseBufferSize = m_asConstantRules[dwI].m_bUseBufferSize;
		sRule.m_bUseStartRegIndex = m_asConstantRules[dwI].m_bUseStartRegIndex;
		sRule.m_dwRegisterCount = m_asConstantRules[dwI].m_dwRegisterCount;
		sRule.m_dwOperationToApply = m_asConstantRules[dwI].m_dwOperationToApply;
		sRule.m_bTranspose = m_asConstantRules[dwI].m_bTranspose;

		// write down the normalized rule
		acStream.write((char*)&sRule, sizeof(Vireio_Constant_Modification_Rule_Normalized));
	}

	// general indices
	UINT dwNumberOfIndices = (UINT)m_aunGlobalConstantRuleIndices.size();
	acStream.write((char*)&dwNumberOfIndices, sizeof(UINT));
	if (m_aunGlobalConstantRuleIndices.size() > 0)
		acStream.write((char*)&m_aunGlobalConstantRuleIndices[0], sizeof(UINT) * dwNumberOfIndices);

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// fetched shader hash codes
	UINT unNumberOfFetched = (UINT)m_aunFetchedHashCodes.size();
	acStream.write((char*)&unNumberOfFetched, sizeof(UINT));
	acStream.write((char*)&m_aunFetchedHashCodes[0], sizeof(UINT) * unNumberOfFetched);
#elif defined(VIREIO_D3D9)
	UINT unNumberOfShaderSpecific = (UINT)m_asShaderSpecificRuleIndices.size();
	acStream.write((char*)&unNumberOfShaderSpecific, sizeof(UINT));
	if (m_asShaderSpecificRuleIndices.size() > 0)
		acStream.write((char*)&m_asShaderSpecificRuleIndices[0], sizeof(Vireio_Hash_Rule_Index) * unNumberOfShaderSpecific);
#endif

	// set data size
	UINT unDataSize = (UINT)acStream.str().size();
	*pdwSizeOfData = unDataSize;

	// copy data
	m_acData.resize(unDataSize);
	memcpy(&m_acData.data()[0], (void*)&acStream.str()[0], (size_t)unDataSize);

	return (char*)&m_acData.data()[0];
}

/// <summary> 
/// Get node data from the profile file.
/// </summary>
void MatrixModifier::InitNodeData(char* pData, UINT dwSizeOfData)
{
	UINT dwDataOffset = 0;
	if (dwSizeOfData >= sizeof(Vireio_GameConfiguration))
	{
		// copy the game configuration data
		memcpy(&m_sGameConfiguration, pData, sizeof(Vireio_GameConfiguration));
		dwDataOffset += sizeof(Vireio_GameConfiguration);

		// get the size of the rules data block
		UINT dwSizeOfRules = dwSizeOfData - sizeof(Vireio_GameConfiguration);

		if (dwSizeOfRules >= sizeof(UINT))
		{
			// get the number of rules
			UINT dwNumRules;
			memcpy(&dwNumRules, pData + dwDataOffset, sizeof(UINT));
			dwDataOffset += sizeof(UINT);

			// data size correct ?
			dwSizeOfRules -= sizeof(UINT);
			if (dwSizeOfRules >= (sizeof(Vireio_Constant_Modification_Rule_Normalized) * dwNumRules))
			{
				// create the rules
				for (UINT dwI = 0; dwI < dwNumRules; dwI++)
				{
					// read normalized data block
					Vireio_Constant_Modification_Rule_Normalized sRuleNormalized;
					memcpy(&sRuleNormalized, pData + dwDataOffset, sizeof(Vireio_Constant_Modification_Rule_Normalized));
					dwDataOffset += sizeof(Vireio_Constant_Modification_Rule_Normalized);

					// create rule, create modification and push back
					Vireio_Constant_Modification_Rule sRule = Vireio_Constant_Modification_Rule(&sRuleNormalized);
					{ wchar_t buf[128]; wsprintf(buf, L"[Vireio] Mod : %u Reg : %u RegCount : %u", sRule.m_dwOperationToApply, sRule.m_dwStartRegIndex, sRule.m_dwRegisterCount); OutputDebugString(buf); }
					if (sRule.m_dwRegisterCount == 1)
						sRule.m_pcModification = CreateVector4Modification(sRule.m_dwOperationToApply, m_pcShaderModificationCalculation);
					else if (sRule.m_dwRegisterCount == 4)
						sRule.m_pcModification = CreateMatrixModification(sRule.m_dwOperationToApply, m_pcShaderModificationCalculation);
					else sRule.m_pcModification = nullptr;
					m_asConstantRules.push_back(sRule);

					dwSizeOfRules -= sizeof(Vireio_Constant_Modification_Rule_Normalized);
				}
			}
		}

		// indices
		if (dwSizeOfRules >= sizeof(UINT))
		{
			// get the number of rules
			UINT dwNumIndices;
			memcpy(&dwNumIndices, pData + dwDataOffset, sizeof(UINT));
			dwDataOffset += sizeof(UINT);

			// data size correct ?
			dwSizeOfRules -= sizeof(UINT);
			if (dwSizeOfRules >= (sizeof(UINT) * dwNumIndices))
			{
				// create the indices
				for (UINT dwI = 0; dwI < dwNumIndices; dwI++)
				{
					// read normalized data block
					UINT dwIndex;
					memcpy(&dwIndex, pData + dwDataOffset, sizeof(UINT));
					dwDataOffset += sizeof(UINT);

					// add index
					m_aunGlobalConstantRuleIndices.push_back(dwIndex);

					dwSizeOfRules -= sizeof(UINT);
				}
			}
		}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		// fetched hash codes
		if (dwSizeOfRules >= sizeof(UINT))
		{
			// get the number of rules
			UINT unNumberOfFetched;
			memcpy(&unNumberOfFetched, pData + dwDataOffset, sizeof(UINT));
			dwDataOffset += sizeof(UINT);

			// data size correct ?
			dwSizeOfRules -= sizeof(UINT);
			if (dwSizeOfRules >= (sizeof(UINT) * unNumberOfFetched))
			{
				// create the indices
				for (UINT dwI = 0; dwI < unNumberOfFetched; dwI++)
				{
					// read normalized data block
					UINT unHash;
					memcpy(&unHash, pData + dwDataOffset, sizeof(UINT));
					dwDataOffset += sizeof(UINT);

					// add hash
					m_aunFetchedHashCodes.push_back(unHash);

					dwSizeOfRules -= sizeof(UINT);
				}
			}
		}
#elif defined(VIREIO_D3D9)
		// shader specific rules
		if (dwSizeOfRules >= sizeof(UINT))
		{
			// get the number of rules
			UINT unNumberOfShaderSpecific;
			memcpy(&unNumberOfShaderSpecific, pData + dwDataOffset, sizeof(UINT));
			dwDataOffset += sizeof(UINT);

			// data size correct ?
			dwSizeOfRules -= sizeof(UINT);
			if (dwSizeOfRules >= (sizeof(Vireio_Hash_Rule_Index) * unNumberOfShaderSpecific))
			{
				// create the indices
				for (UINT dwI = 0; dwI < unNumberOfShaderSpecific; dwI++)
				{
					// read normalized data block
					Vireio_Hash_Rule_Index unHashRuleIndex;
					memcpy(&unHashRuleIndex, pData + dwDataOffset, sizeof(Vireio_Hash_Rule_Index));
					dwDataOffset += sizeof(Vireio_Hash_Rule_Index);

					// add rule index
					m_asShaderSpecificRuleIndices.push_back(unHashRuleIndex);

					dwSizeOfRules -= sizeof(Vireio_Hash_Rule_Index);
				}
			}
		}
#endif

		// fill the string list
		FillShaderRuleIndices();
		FillShaderRuleGeneralIndices();
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		FillFetchedHashCodeList();
#elif defined(VIREIO_D3D9)
		FillShaderRuleShaderIndices();
#endif
	}
	else
	{
		// set to ipd using vireio presenter.... // TODO !! currently set ipd to default
		m_sGameConfiguration.fConvergence = 3.0f;
		m_sGameConfiguration.fIPD = IPD_DEFAULT;
		m_sGameConfiguration.fWorldScaleFactor = 0.0f;
		m_sGameConfiguration.fPFOV = 110.0f;
	}

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// get ini file settings
	m_sGameConfiguration.fWorldScaleFactor = GetIniFileSetting(m_sGameConfiguration.fWorldScaleFactor, "MatrixModifier", "fWorldScaleFactor", szFilePathINI, bFileExists);
	m_sGameConfiguration.fConvergence = GetIniFileSetting(m_sGameConfiguration.fConvergence, "MatrixModifier", "fConvergence", szFilePathINI, bFileExists);
	m_sGameConfiguration.fIPD = GetIniFileSetting(m_sGameConfiguration.fIPD, "MatrixModifier", "fIPD", szFilePathINI, bFileExists);
	m_sGameConfiguration.fPFOV = GetIniFileSetting(m_sGameConfiguration.fPFOV, "MatrixModifier", "fPFOV", szFilePathINI, bFileExists);
	DWORD uDummy = 0;
	uDummy = GetIniFileSetting((DWORD)m_sGameConfiguration.bConvergenceEnabled, "MatrixModifier", "bConvergenceEnabled", szFilePathINI, bFileExists);
	if (uDummy) m_sGameConfiguration.bConvergenceEnabled = true; else m_sGameConfiguration.bConvergenceEnabled = false;
	uDummy = GetIniFileSetting((DWORD)m_sGameConfiguration.bPFOVToggle, "MatrixModifier", "bPFOVToggle", szFilePathINI, bFileExists);
	if (uDummy) m_sGameConfiguration.bPFOVToggle = true; else m_sGameConfiguration.bPFOVToggle = false;

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Matrix Modifier";

	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "World Scale Factor";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.pfValue = &m_sGameConfiguration.fWorldScaleFactor;
		sEntry.fValue = m_sGameConfiguration.fWorldScaleFactor;
		sEntry.fMinimum = -100000.0f;
		sEntry.fMaximum = 100000.0f;
		sEntry.fChangeSize = 0.01f;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Convergence";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.pfValue = &m_sGameConfiguration.fConvergence;
		sEntry.fValue = m_sGameConfiguration.fConvergence;
		sEntry.fMinimum = 1.0f;
		sEntry.fMaximum = 10.0f;
		sEntry.fChangeSize = 0.01f;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Inter-Pupillary Distance";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.pfValue = &m_sGameConfiguration.fIPD;
		sEntry.fValue = m_sGameConfiguration.fIPD;
		sEntry.fMinimum = 0.0510f;
		sEntry.fMaximum = 0.0770f;
		sEntry.fChangeSize = 0.001f;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Aspect Multiplier";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.pfValue = &m_sGameConfiguration.fAspectMultiplier;
		sEntry.fValue = m_sGameConfiguration.fAspectMultiplier;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 5.0f;
		sEntry.fChangeSize = 0.01f;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Convergence Toggle";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_sGameConfiguration.bConvergenceEnabled;
		sEntry.bValue = m_sGameConfiguration.bConvergenceEnabled;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Projection FOV";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.pfValue = &m_sGameConfiguration.fPFOV;
		sEntry.fValue = m_sGameConfiguration.fPFOV;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Projection FOV Toggle";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_sGameConfiguration.bPFOVToggle;
		sEntry.bValue = m_sGameConfiguration.bPFOVToggle;
		m_sMenu.asEntries.push_back(sEntry);
	}

	// update Shader element calculation class !
	m_pcShaderModificationCalculation->Load(&m_sGameConfiguration);
	m_pcShaderModificationCalculation->SetFloat(MathFloatFields::AspectMultiplier, (float)1920.0f / (float)1080.0f);
	m_pcShaderModificationCalculation->ComputeViewTransforms();
}

/// <summary> 
/// Provides the name of the requested commander.
/// </summary>
LPWSTR MatrixModifier::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((STS_Commanders)dwCommanderIndex)
	{
	case Modifier:
		return VLink::Name(VLink::_L::ModifierData);
	default:
		break;
	}

	return L"UNTITLED";
}

/// <summary> 
/// Provides the name of the requested decommander.
/// </summary>
LPWSTR MatrixModifier::GetDecommanderName(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		CASE_ENUM_2_WSTRING(STS_Decommanders, CopyResource);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CopySubresourceRegion);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CreateBuffer);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CreatePixelShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CreateVertexShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, DSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, HSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Map);
		CASE_ENUM_2_WSTRING(STS_Decommanders, PSGetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, PSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, PSSetShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, Unmap);
		CASE_ENUM_2_WSTRING(STS_Decommanders, UpdateSubresource);
		CASE_ENUM_2_WSTRING(STS_Decommanders, VSGetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, VSSetConstantBuffers);
		CASE_ENUM_2_WSTRING(STS_Decommanders, VSSetShader);
	default:
		break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetVertexShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetPixelShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetVertexShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetPixelShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetTransform);
		CASE_ENUM_2_WSTRING(STS_Decommanders, MultiplyTransform);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetVertexShaderConstantF);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetVertexShaderConstantF);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetVertexShaderConstantI);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetVertexShaderConstantI);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetVertexShaderConstantB);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetVertexShaderConstantB);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetPixelShaderConstantF);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetPixelShaderConstantF);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetPixelShaderConstantI);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetPixelShaderConstantI);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetPixelShaderConstantB);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetPixelShaderConstantB);
		CASE_ENUM_2_WSTRING(STS_Decommanders, SetStreamSource);
		CASE_ENUM_2_WSTRING(STS_Decommanders, GetStreamSource);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CreateVertexShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, CreatePixelShader);
		CASE_ENUM_2_WSTRING(STS_Decommanders, VB_Apply);
	default:
		break;
	}
#endif
	return L"UNTITLED";
}

/// <summary> 
/// Returns the plug type for the requested commander.
/// </summary>
DWORD MatrixModifier::GetCommanderType(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
	case Modifier:
		return VLink::Link(VLink::_L::ModifierData);
	default:
		break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Commanders)dwCommanderIndex)
	{
	case Modifier:
		return VLink::Link(VLink::_L::ModifierData);
	default:
		break;
	}
#endif

	return NULL;
}

/// <summary> 
/// Returns the plug type for the requested decommander.
/// </summary>
DWORD MatrixModifier::GetDecommanderType(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::CopyResource:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CopyResource);
	case STS_Decommanders::CopySubresourceRegion:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion);
	case STS_Decommanders::CreateBuffer:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, (int)VMT_ID3D11DEVICE::CreateBuffer);
	case STS_Decommanders::CreatePixelShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, (int)VMT_ID3D11DEVICE::CreatePixelShader);
	case STS_Decommanders::CreateVertexShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, (int)VMT_ID3D11DEVICE::CreateVertexShader);
	case STS_Decommanders::DSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers);
	case STS_Decommanders::GSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers);
	case STS_Decommanders::HSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers);
	case STS_Decommanders::Map:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Map);
	case STS_Decommanders::PSGetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers);
	case STS_Decommanders::PSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers);
	case STS_Decommanders::PSSetShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::PSSetShader);
	case STS_Decommanders::Unmap:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::Unmap);
	case STS_Decommanders::UpdateSubresource:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::UpdateSubresource);
	case STS_Decommanders::VSGetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers);
	case STS_Decommanders::VSSetConstantBuffers:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers);
	case STS_Decommanders::VSSetShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext, (int)VMT_ID3D11DEVICECONTEXT::VSSetShader);
	default:
		break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::SetVertexShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetVertexShader);
	case STS_Decommanders::SetPixelShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetPixelShader);
	case STS_Decommanders::GetVertexShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetVertexShader);
	case STS_Decommanders::GetPixelShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetPixelShader);
	case STS_Decommanders::SetTransform:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetTransform);
	case STS_Decommanders::MultiplyTransform:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::MultiplyTransform);
	case STS_Decommanders::SetVertexShaderConstantF:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF);
	case STS_Decommanders::GetVertexShaderConstantF:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF);
	case STS_Decommanders::SetVertexShaderConstantI:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI);
	case STS_Decommanders::GetVertexShaderConstantI:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI);
	case STS_Decommanders::SetVertexShaderConstantB:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB);
	case STS_Decommanders::GetVertexShaderConstantB:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB);
	case STS_Decommanders::SetPixelShaderConstantF:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF);
	case STS_Decommanders::GetPixelShaderConstantF:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF);
	case STS_Decommanders::SetPixelShaderConstantI:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI);
	case STS_Decommanders::GetPixelShaderConstantI:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI);
	case STS_Decommanders::SetPixelShaderConstantB:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB);
	case STS_Decommanders::GetPixelShaderConstantB:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB);
	case STS_Decommanders::SetStreamSource:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::SetStreamSource);
	case STS_Decommanders::GetStreamSource:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::GetStreamSource);
	case STS_Decommanders::CreateVertexShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreateVertexShader);
	case STS_Decommanders::CreatePixelShader:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::CreatePixelShader);
	case STS_Decommanders::VB_Apply:
		return NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9, (int)VMT_IDIRECT3DSTATEBLOCK9::Apply);
	default:
		break;
	}
#endif
	return 0;
}

/// <summary> 
/// Provides the output pointer for the requested commander.
/// </summary>
void* MatrixModifier::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((STS_Commanders)dwCommanderIndex)
	{
	case Modifier:
		return (void*)&m_sModifierData;
	default:
		break;
	}

	return nullptr;
}

/// <summary> 
/// Sets the input pointer for the requested decommander.
/// </summary>
void MatrixModifier::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	if (dwDecommanderIndex < NUMBER_OF_DECOMMANDERS)
		m_ppInput[dwDecommanderIndex] = (pData);
}

/// <summary> 
/// Matrix Modifier supports various D3D10 + D3D11 calls.
/// </summary>
bool MatrixModifier::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
#if defined(VIREIO_D3D10)
	if ((nD3DVersion >= (int)AQU_Direct3DVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_Direct3DVersion::DirectX_10_1))
	{
		if (nD3DInterface == ITA_D3D10INTERFACES::ID3D10Device)
		{

		}
		else if (nD3DInterface == ITA_DXGIINTERFACES::IDXGISwapChain)
		{

		}
	}
#elif defined VIREIO_D3D11
	if ((nD3DVersion >= (int)AQU_Direct3DVersion::DirectX_11) &&
		(nD3DVersion <= (int)AQU_Direct3DVersion::DirectX_11_2))
	{
		if (nD3DInterface == ITA_D3D11INTERFACES::ID3D11Device)
		{
			if ((nD3DMethod == (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateBuffer) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreatePixelShader) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateVertexShader))
				return true;
		}
		else if (nD3DInterface == ITA_D3D11INTERFACES::ID3D11DeviceContext)
		{
			if ((nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSSetShader) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSSetShader) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSSetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::Map) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::Unmap) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::CopySubresourceRegion) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::CopyResource) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::UpdateSubresource) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSGetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSGetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::HSSetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::DSSetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::GSSetConstantBuffers) ||
				(nD3DMethod == (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSSetConstantBuffers))
				return true;
		}
		else if (nD3DInterface == ITA_DXGIINTERFACES::IDXGISwapChain)
		{

		}
	}
#elif defined(VIREIO_D3D9)
	if ((nD3DVersion >= (int)AQU_Direct3DVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_Direct3DVersion::DirectX_9_29))
	{
		if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9)
		{
			if ((nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetVertexShader) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetPixelShader) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetVertexShader) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetPixelShader) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetTransform) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::MultiplyTransform) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::SetStreamSource) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::GetStreamSource) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreateVertexShader) ||
				(nD3DMethod == (int)VMT_IDIRECT3DDEVICE9::CreatePixelShader))
				return true;
		}
		else if (nD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9)
		{
			if (nD3DMethod == (int)VMT_IDIRECT3DSTATEBLOCK9::Apply) return true;
		}
	}

#endif
	return false;
}

/// <summary>
/// => Provoke
/// Handle Stereo Render Targets (+Depth Buffers).
/// Main entry point.
/// </summary>
void* MatrixModifier::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	static HRESULT nHr = S_OK;

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

			// get ini file settings
			m_sGameConfiguration.fWorldScaleFactor = GetIniFileSetting(m_sGameConfiguration.fWorldScaleFactor, "MatrixModifier", "fWorldScaleFactor", szFilePathINI, bFileExists);
			m_sGameConfiguration.fConvergence = GetIniFileSetting(m_sGameConfiguration.fConvergence, "MatrixModifier", "fConvergence", szFilePathINI, bFileExists);
			m_sGameConfiguration.fIPD = GetIniFileSetting(m_sGameConfiguration.fIPD, "MatrixModifier", "fIPD", szFilePathINI, bFileExists);
			m_sGameConfiguration.fPFOV = GetIniFileSetting(m_sGameConfiguration.fPFOV, "MatrixModifier", "fPFOV", szFilePathINI, bFileExists);
			DWORD uDummy = 0;
			uDummy = GetIniFileSetting((DWORD)m_sGameConfiguration.bConvergenceEnabled, "MatrixModifier", "bConvergenceEnabled", szFilePathINI, bFileExists);
			if (uDummy) m_sGameConfiguration.bConvergenceEnabled = true; else m_sGameConfiguration.bConvergenceEnabled = false;
			uDummy = GetIniFileSetting((DWORD)m_sGameConfiguration.bPFOVToggle, "MatrixModifier", "bPFOVToggle", szFilePathINI, bFileExists);
			if (uDummy) m_sGameConfiguration.bPFOVToggle = true; else m_sGameConfiguration.bPFOVToggle = false;
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
		// for (size_t nIx = 0; nIx < m_sMenu.asEntries.size(); nIx++)
		{
			// update view transform ..... TODO 
			/*	m_pcShaderModificationCalculation->Load(&m_sGameConfiguration);
				m_pcShaderModificationCalculation->SetFloat(MathFloatFields::AspectMultiplier, (float)1920.0f / (float)1080.0f);
				m_pcShaderModificationCalculation->ComputeViewTransforms();*/
		}
	}

	// head roll ? TODO !! Tracker input !!
	static float s_fRoll = 0.0f;
	/*if (m_apfTrackerInput[2])
	if ((*m_apfTrackerInput[2]) != s_fRoll)
	{
		s_fRoll = *m_apfTrackerInput[2];
		// TODO !!
		// m_pcShaderModificationCalculation->UpdateRoll(-s_fRoll);
		// m_pcShaderModificationCalculation->ComputeViewTransforms();
	}*/

#if defined(VIREIO_D3D11)
	
	// set current device context
	m_pcContextCurrent = (ID3D11DeviceContext*)pThis;
	int nFlags = 0;

	switch (eD3DInterface)
	{
	case ITA_D3D11INTERFACES::ID3D11DeviceContext:
		switch (eD3DMethod)
		{
#pragma region ID3D11DeviceContext::UpdateSubresource
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::UpdateSubresource:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "UpdateSubresource");

			int nFlags = 0;
			UpdateSubresource(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopySubresourceRegion
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::CopySubresourceRegion:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CopySubresourceRegion");

			int nFlags = 0;
			CopySubresourceRegion(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopyResource
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::CopyResource:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CopyResource");

			int nFlags = 0;
			CopyResource(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetShader
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSSetShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "VSSetShader");

			int nFlags = 0;
			VSSetShader(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::PSSetShader
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSSetShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "PSSetShader");

			int nFlags = 0;
			PSSetShader(nFlags);
			nProvokerIndex |= nFlags;
		}
		break;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSSetConstantBuffers:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "VSSetConstantBuffers");

			int nFlags = 0;
			VSSetConstantBuffers(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::HSSetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::HSSetConstantBuffers:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "HSSetConstantBuffers");

			int nFlags = 0;
			HSSetConstantBuffers(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::DSSetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::DSSetConstantBuffers:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "DSSetConstantBuffers");

			int nFlags = 0;
			DSSetConstantBuffers(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::GSSetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::GSSetConstantBuffers:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GSSetConstantBuffers");

			int nFlags = 0;
			GSSetConstantBuffers(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::PSSetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSSetConstantBuffers:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "PSSetConstantBuffers");

			int nFlags = 0;
			PSSetConstantBuffers(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSGetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::VSGetConstantBuffers:
			// currently, we set the main buffers to avoid that the game gets the 
			// stereo buffers assioziated with the main buffers as private data interfaces.
			// if there is a game that flickers (should not be) we need to replace the whole
			// method (using AQU_PluginFlags::ImmediateReturnFlag)
			m_pcContextCurrent->VSSetConstantBuffers(0,
				D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,
				(ID3D11Buffer**)&m_apcVSActiveConstantBuffers11[0]);
			break;
#pragma endregion
#pragma region ID3D11DeviceContext::PSGetConstantBuffers
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::PSGetConstantBuffers:
			// currently, we set the main buffers to avoid that the game gets the 
			// stereo buffers assioziated with the main buffers as private data interfaces.
			// if there is a game that flickers (should not be) we need to replace the whole
			// method (using AQU_PluginFlags::ImmediateReturnFlag)
			m_pcContextCurrent->PSSetConstantBuffers(0,
				D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,
				(ID3D11Buffer**)&m_apcPSActiveConstantBuffers11[0]);
			break;
#pragma endregion
#pragma region ID3D11DeviceContext::Map
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::Map:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "Map");

			int nFlags = 0;
			nHr = Map(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::Unmap
		case (int)VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext::Unmap:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "Unmap");

			int nFlags = 0;
			Unmap(nFlags);
			nProvokerIndex |= nFlags;
		}
		return nullptr;
#pragma endregion
		}
		return nullptr;
	case ITA_D3D11INTERFACES::ID3D11Device:
		switch (eD3DMethod)
		{
		case (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateBuffer:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CreateBuffer");

			int nFlags = 0;
			nHr = CreateBuffer(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
		case (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreatePixelShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CreatePixelShader");

			int nFlags = 0;
			nHr = CreatePixelShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
		case (int)VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateVertexShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CreateVertexShader");

			int nFlags = 0;
			nHr = CreateVertexShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
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
	// set current device... (union field with IDirect3DStateBlock9 for state block methods)
	m_pcDeviceCurrent = (IDirect3DDevice9*)pThis;

	switch (eD3DInterface)
	{
	case (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9:
		switch (eD3DMethod)
		{
#pragma region SetVertexShader
		case (int)VMT_IDIRECT3DDEVICE9::SetVertexShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetVertexShader");

			int nFlags = 0;
			nHr = SetVertexShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetPixelShader
		case (int)VMT_IDIRECT3DDEVICE9::SetPixelShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetPixelShader");

			int nFlags = 0;
			nHr = SetPixelShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetVertexShader
		case (int)VMT_IDIRECT3DDEVICE9::GetVertexShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetVertexShader");

			int nFlags = 0;
			nHr = GetVertexShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetPixelShader
		case (int)VMT_IDIRECT3DDEVICE9::GetPixelShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetPixelShader");

			int nFlags = 0;
			nHr = GetPixelShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetTransform
		case (int)VMT_IDIRECT3DDEVICE9::SetTransform:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetTransform");

			int nFlags = 0;
			nHr = SetTransform(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region MultiplyTransform
		case (int)VMT_IDIRECT3DDEVICE9::MultiplyTransform:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "MultiplyTransform");

			int nFlags = 0;
			nHr = MultiplyTransform(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetVertexShaderConstantF
		case (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetVertexShaderConstantF");

			int nFlags = 0;
			nHr = SetVertexShaderConstantF(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetVertexShaderConstantF
		case (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetVertexShaderConstantF");

			int nFlags = 0;
			nHr = GetVertexShaderConstantF(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetVertexShaderConstantI
		case (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetVertexShaderConstantI");

			int nFlags = 0;
			nHr = SetVertexShaderConstantI(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetVertexShaderConstantI
		case (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetVertexShaderConstantI");

			int nFlags = 0;
			nHr = GetVertexShaderConstantI(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetVertexShaderConstantB
		case (int)VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetVertexShaderConstantB");

			int nFlags = 0;
			nHr = SetVertexShaderConstantB(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetVertexShaderConstantB
		case (int)VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetVertexShaderConstantB");

			int nFlags = 0;
			nHr = GetVertexShaderConstantB(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetPixelShaderConstantF
		case (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetPixelShaderConstantF");

			int nFlags = 0;
			nHr = SetPixelShaderConstantF(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		break;
#pragma endregion
#pragma region GetPixelShaderConstantF
		case (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetPixelShaderConstantF");

			int nFlags = 0;
			nHr = GetPixelShaderConstantF(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetPixelShaderConstantI
		case (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetPixelShaderConstantI");

			int nFlags = 0;
			nHr = SetPixelShaderConstantI(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetPixelShaderConstantI
		case (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetPixelShaderConstantI");

			int nFlags = 0;
			nHr = GetPixelShaderConstantI(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetPixelShaderConstantB
		case (int)VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetPixelShaderConstantB");

			int nFlags = 0;
			nHr = SetPixelShaderConstantB(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetPixelShaderConstantB
		case (int)VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetPixelShaderConstantB");

			int nFlags = 0;
			nHr = GetPixelShaderConstantB(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region SetStreamSource
		case (int)VMT_IDIRECT3DDEVICE9::SetStreamSource:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "SetStreamSource");

			int nFlags = 0;
			nHr = SetStreamSource(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region GetStreamSource
		case (int)VMT_IDIRECT3DDEVICE9::GetStreamSource:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "GetStreamSource");

			int nFlags = 0;
			nHr = GetStreamSource(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region CreateVertexShader
		case (int)VMT_IDIRECT3DDEVICE9::CreateVertexShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CreateVertexShader");

			int nFlags = 0;
			nHr = CreateVertexShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region CreatePixelShader
		case (int)VMT_IDIRECT3DDEVICE9::CreatePixelShader:
		{
			SHOW_CALL_MODIFIER(m_bTrace, "CreatePixelShader");

			int nFlags = 0;
			nHr = CreatePixelShader(nFlags);
			if (nFlags)
			{
				nProvokerIndex |= nFlags;
				return (void*)&nHr;
			}
		}
		return nullptr;
#pragma endregion
#pragma region Apply
#pragma endregion
	}
}
#endif

	return nullptr;
}

/// <summary>
/// => Update ImGui Control
/// Update node controls using ImGui. 
/// </summary>
/// <param name="fZoom">Current zoom factor on ImNodes canvas.</param>
void MatrixModifier::UpdateImGuiControl(float fZoom)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	std::vector<Vireio_D3D11_Shader>* pasShaders;
#elif defined(VIREIO_D3D9)
	std::vector<Vireio_D3D9_Shader>* pasShaders;
#endif
	std::vector<std::string>* pasShaderHashCodes;
	std::vector<UINT>* padwShaderHashCodes;

	ImGui::TextUnformatted("Vireio Perception Matrix Modifier Controls\n");

#pragma region /// => GUI Static Text
	// game configuration
	static std::string szGameSeparationText = std::string("World Scale Factor");
	static std::string szConvergence = std::string("Convergence");
	static std::string szAspectMultiplier = std::string("Aspect Multiplier");
	static std::string szYawMultiplier = std::string("Yaw Multiplier");
	static std::string szPitchMultiplier = std::string("Pitch Multiplier");
	static std::string szRollMultiplier = std::string("Roll Multiplier");
	static std::string szPositionMultiplier = std::string("Position Multiplier");
	static std::string szPositionXMultiplier = std::string("Position X Multiplier");
	static std::string szPositionYMultiplier = std::string("Position Y Multiplier");
	static std::string szPositionZMultiplier = std::string("Position Z Multiplier");
	static std::string szPFOV = std::string("Projection FOV");
	static std::string szConvergenceToggle = std::string("Use Convergence");
	static std::string szPFOVToggle = std::string("Use ProjectionFOV");
	std::string sz64bit;
	if (m_sGameConfiguration.bIs64bit)
		sz64bit = std::string("64 Bit");
	else
		sz64bit = std::string("32 Bit");
	const char* aacHearoll_names[3] = { "No Roll", "Matrix Roll", "PShader Roll" };

	// shader page
	static std::string szUpdate = std::string("Update");
	static std::string szSort = std::string("Sort");
	static std::string szToName = std::string("-> Name");
	static std::string szToRegister = std::string("-> Register");
	static std::string szToBufferSize = std::string("-> Buffer Size");
	static std::string szToBufferIndex = std::string("-> Buffer Index");
	static std::string szToFetchedHash = std::string("-> Fetched Hash");

	// shader rules page
	static std::string szConstantName = std::string("Constant Name");
	static std::string szPartialNameMatch = std::string("Partial Name");
	static std::string szStartRegIndex = std::string("Start Register");
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	static std::string szBufferSize = std::string("Buffer Size");
	static std::string szBufferIndex = std::string("Buffer Index");
#endif
	static std::string szTranspose = std::string("Transpose");
	static std::string szCreate = std::string("Create");
	static std::string szToGeneral = std::string("To General");
	static std::string szDelete = std::string("Delete");

	// get modification names for the selection
	static std::vector<std::string> aszOperations;
	if (!aszOperations.size())
	{
		for (unsigned uMod = 0; uMod < uMatrixModificationNumber; uMod++)
			aszOperations.push_back(GetMatrixModificationName(uMod));
	}

	std::string sz1 = std::string("1 - Vector 4f");
	std::string sz2 = std::string("2 - Matrix 2x4f");
	std::string sz4 = std::string("4 - Matrix 4x4f");
	static std::vector<std::string> aszRegCounts;
	if (!aszRegCounts.size())
	{
		aszRegCounts.push_back(sz1);
		aszRegCounts.push_back(sz2);
		aszRegCounts.push_back(sz4);
	}
#pragma endregion

#pragma region /// => GUI Game settings

	/// => Game settings
	bool bGameSettings = false;
#if (defined(VIREIO_D3D11) || defined(VIREIO_D3D10))
	ImGui::TextUnformatted("Game Settings D3D10/11 ("); ImGui::SameLine();
#else
	ImGui::TextUnformatted("Game Settings D3D9 ("); ImGui::SameLine();
#endif
	ImGui::TextUnformatted(sz64bit.c_str()); ImGui::SameLine(); ImGui::TextUnformatted(")");
#ifdef TRACE_MODIFIER
	ImGui::SameLine(); ImGui::ToggleButton("Trace MAM", &m_bTrace);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Debug Trace output.\n(DebugView)");
#endif
	bGameSettings |= ImGui::DragFloat(szGameSeparationText.c_str(), &m_sGameConfiguration.fWorldScaleFactor, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "3D horizontal stereo separation.\n(in Game Units)");
	bGameSettings |= ImGui::DragFloat(szConvergence.c_str(), &m_sGameConfiguration.fConvergence, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Convergence l/r image shift.\n(NOT used for HMDs)");
	bGameSettings |= ImGui::DragFloat(szAspectMultiplier.c_str(), &m_sGameConfiguration.fAspectMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Aspect Multiplier.\n(Not used currently)");
	bGameSettings |= ImGui::DragFloat(szYawMultiplier.c_str(), &m_sGameConfiguration.fYawMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Yaw/Pitch/Roll tracker multiplier.\n(Not used currently)");
	bGameSettings |= ImGui::DragFloat(szPitchMultiplier.c_str(), &m_sGameConfiguration.fPitchMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	bGameSettings |= ImGui::DragFloat(szRollMultiplier.c_str(), &m_sGameConfiguration.fRollMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	bGameSettings |= ImGui::DragFloat(szPositionMultiplier.c_str(), &m_sGameConfiguration.fPositionMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Positional Tracking multiplier.\n(for all 3 axes)");
	bGameSettings |= ImGui::DragFloat(szPositionXMultiplier.c_str(), &m_sGameConfiguration.fPositionXMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Positional Tracking multiplier.\n(X/Y/Z separately)");
	bGameSettings |= ImGui::DragFloat(szPositionYMultiplier.c_str(), &m_sGameConfiguration.fPositionYMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	bGameSettings |= ImGui::DragFloat(szPositionZMultiplier.c_str(), &m_sGameConfiguration.fPositionZMultiplier, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	bGameSettings |= ImGui::DragFloat(szPFOV.c_str(), &m_sGameConfiguration.fPFOV, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Projection Matrix\nField-of-View.\n(if FOV applied via Matrix)");
	bGameSettings |= ImGui::Checkbox(szPFOVToggle.c_str(), &m_sGameConfiguration.bPFOVToggle); ImGui::SameLine();
	bGameSettings |= ImGui::Checkbox(szConvergenceToggle.c_str(), &m_sGameConfiguration.bConvergenceEnabled);
	const char* acElem_name = (m_sGameConfiguration.nRollImpl >= 0 && m_sGameConfiguration.nRollImpl < 3) ? aacHearoll_names[m_sGameConfiguration.nRollImpl] : "Unknown";
	bGameSettings |= ImGui::SliderInt("Head Roll", &m_sGameConfiguration.nRollImpl, 0, 2, acElem_name);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Head-Roll aplliance\nvia Matrix.\n(via Vertex or Pixel Shader)");
	if (bGameSettings)
	{
		// update Shader element calculation class !
		m_pcShaderModificationCalculation->Load(&m_sGameConfiguration);
		m_pcShaderModificationCalculation->SetFloat(MathFloatFields::AspectMultiplier, (float)1920.0f / (float)1080.0f);
		m_pcShaderModificationCalculation->ComputeViewTransforms();
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "Convergence l/r image shift.\n(NOT used for HMDs)");
	}
	ImGui::Separator();
#pragma endregion

#pragma region /// => GUI Shader

	ImGui::TextUnformatted("Shader controls");
	// TODO !! (D3D11) m_sPageShader.m_dwCurrentBuffersizes -> m_aszShaderBuffersizes

	// Shader controls..
	static int nItem_ShaderConstants = -1;
	static int nItem_VShader = 1;
	static int nItem_PShader = -1;
	static int nItem_ShaderRuleIndices = -1;
	static int nItem_ShaderRuleGeneralIndices = -1;
	static int nItem_ShaderRuleShaderIndices = -1;

	// current shader constants list box
	if (m_aszShaderConstantsCurrent.size())
	{
		if (ImGui::ListBox("Shader constants\n(current shader)", &nItem_ShaderConstants, m_aszShaderConstantsCurrent, 6))
		{
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "Shader constants\nof the currently\nchosen Vx/Px Shader.\nChoose to use in\nShader rule.");
	}
	// vertex shader hash code list
	bool bHashCodeChosen = false;
	if (m_aszVShaderHashCodes.size())
	{
		if (ImGui::ListBox("Vertex Shaders\n(Hash codes)", &nItem_VShader, m_aszVShaderHashCodes, 12))
		{
			// set pixel shader list to -1
			nItem_PShader = -1; nItem_ShaderConstants = -1;
			bHashCodeChosen = true;
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "Hash Codes of\nall Vertex\nShaders.\nChoose to show\nShader constants.");
	}
	// pixel shader hash code list
	if (m_aszPShaderHashCodes.size())
	{
		if (ImGui::ListBox("Pixel Shaders\n(Hash codes)", &nItem_PShader, m_aszPShaderHashCodes, 12))
		{
			// set vertex shader list to -1
			nItem_VShader = -1; nItem_ShaderConstants = -1;
			bHashCodeChosen = true;
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "Hash Codes of\nall Pixel (or\nFragment) Shaders.\nChoose to show\nShader constants.");
	}
	if (bHashCodeChosen)
	{
		// which shader ?
		if (nItem_VShader >= 0)
		{
			// set vertex shader lists
			pasShaders = &m_sModifierData.asVShaders;
			pasShaderHashCodes = &m_aszVShaderHashCodes;
			padwShaderHashCodes = &m_adwVShaderHashCodes;

			// get hash code
			if (nItem_VShader < (int)m_adwVShaderHashCodes.size())
				m_dwCurrentChosenShaderHashCode = (UINT)m_adwVShaderHashCodes[nItem_VShader];
			else
			{
				OutputDebugString(L"[MAM] Serious node logic error !! Wrong index !!");
				m_dwCurrentChosenShaderHashCode = 0;
		}
	}
		else if (nItem_PShader >= 0)
		{
			// set pixel shader lists
			pasShaders = &m_sModifierData.asPShaders;
			pasShaderHashCodes = &m_aszPShaderHashCodes;
			padwShaderHashCodes = &m_adwPShaderHashCodes;

			// get hash code
			if (nItem_PShader < (int)m_adwPShaderHashCodes.size())
				m_dwCurrentChosenShaderHashCode = (UINT)m_adwPShaderHashCodes[nItem_VShader];
			else
			{
				OutputDebugString(L"[MAM] Serious node logic error !! Wrong index !!");
				m_dwCurrentChosenShaderHashCode = 0;
			}
		}
		else m_dwCurrentChosenShaderHashCode = 0;

		// allocate vectors
		m_aszShaderConstantsCurrent = std::vector<std::string>();
		m_auShaderConstantsCurrentStartReg = std::vector<uint32_t>();
		m_aszShaderBuffersizes = std::vector<std::string>();

		// find the hash code in the shader list
		UINT dwIndex = 0;
		for (UINT dwI = 0; dwI < (UINT)(*pasShaders).size(); dwI++)
		{
			if (m_dwCurrentChosenShaderHashCode == (*pasShaders)[dwI].uHash)
			{
				dwIndex = dwI;
				dwI = (UINT)(*pasShaders).size();
			}
		}

#if defined(VIREIO_D3D9)
		// add all constant names to the current shader constant list
		for (UINT dwK = 0; dwK < (UINT)(*pasShaders)[dwIndex].asConstantDescriptions.size(); dwK++)
		{
			// get std::string and add to vector
			std::string szNameA = std::string((*pasShaders)[dwIndex].asConstantDescriptions[dwK].acName);
			m_aszShaderConstantsCurrent.push_back(szNameA);
			m_auShaderConstantsCurrentStartReg.push_back((*pasShaders)[dwIndex].asConstantDescriptions[dwK].uRegisterIndex);
		}
#endif
}
	// update all shaders info
	if (ImGui::Button(szUpdate.c_str()))
	{
		for (const unsigned uI : { 0, 1 })
		{
			// update both.. vertex and pixel shader
			switch (uI)
			{
			case 0:
				pasShaders = &m_sModifierData.asVShaders;
				pasShaderHashCodes = &m_aszVShaderHashCodes;
				padwShaderHashCodes = &m_adwVShaderHashCodes;
				break;
			case 1:
			default:
				pasShaders = &m_sModifierData.asPShaders;
				pasShaderHashCodes = &m_aszPShaderHashCodes;
				padwShaderHashCodes = &m_adwPShaderHashCodes;
				break;
			}

			// test the shader constant list for updates...
			if ((*pasShaderHashCodes).size() < (*pasShaders).size())
			{
				// loop through new shaders
				for (UINT dwI = (UINT)(*pasShaderHashCodes).size(); dwI < (UINT)(*pasShaders).size(); dwI++)
				{

#ifdef VIREIO_D3D9
					// loop through constant descriptions for that shader
					for (UINT dwK = 0; dwK < (UINT)(*pasShaders)[dwI].asConstantDescriptions.size(); dwK++)
					{
						// convert to wstring
						std::string szNameA = std::string((*pasShaders)[dwI].asConstantDescriptions[dwK].acName);
						std::wstring szName(szNameA.begin(), szNameA.end());

						// constant available in list ?
						if (std::find(m_aszShaderConstants.begin(), m_aszShaderConstants.end(), szName) == m_aszShaderConstants.end())
						{
							m_aszShaderConstants.push_back(szName);
							m_aszShaderConstantsA.push_back(szNameA);
						}
					}
#endif

					// add shader hex hash to lists
					char acHash[9] = {};
					HexString(acHash, (*pasShaders)[dwI].uHash, 8);
					std::string szHash = std::string(acHash);
					(*pasShaderHashCodes).push_back(szHash);
					(*padwShaderHashCodes).push_back((*pasShaders)[dwI].uHash);
				}
			}
		}
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Update all info\nfor all Shaders."); ImGui::SameLine();
	// Sort shader lists
	ImGui::ToggleButton(szSort.c_str(), &m_bSortShaderList);
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Sort the Hash\nCodes for all\nShaders by use\nfrequency."); ImGui::SameLine();
	// Copy constant name string
	if (ImGui::Button(szToName.c_str()))
	{
		if ((nItem_ShaderConstants >= 0) && (nItem_ShaderConstants < (INT)m_aszShaderConstantsCurrent.size()))
		{
			// set the name to the name control of the shader rules page
			m_sPageGameShaderRules.m_szConstantName = m_aszShaderConstantsCurrent[nItem_ShaderConstants];
		}
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Copy the Shader\nConstant name to the\nShader rule creation\ntext input box."); ImGui::SameLine();
	// Copy constant register index
	if (ImGui::Button(szToRegister.c_str()))
	{
		if ((nItem_ShaderConstants >= 0) && (nItem_ShaderConstants < (INT)m_auShaderConstantsCurrentStartReg.size()))
		{
			// set the name to the name control of the shader rules page
			m_sPageGameShaderRules.m_dwStartRegIndex = m_auShaderConstantsCurrentStartReg[nItem_ShaderConstants];
		}
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Copy the Shader\nConstant register\nindex to the\nShader rule creation\ntext input box.");

	// TODO !! D3D10/11 buffer size, buffer index, to fetched list ??

	ImGui::Separator();

#pragma endregion

#pragma region /// => GUI Shader Rules

	ImGui::TextUnformatted("Shader Rules Creation");

	ImGui::Columns(2, NULL, false);

	// constant name
	ImGui::InputText(szConstantName.c_str(), &m_sPageGameShaderRules.m_szConstantName, ImGuiInputTextFlags_None);
	ImGui::NextColumn(); ImGui::ToggleButton("Use_F", &m_sPageGameShaderRules.m_bConstantName);
	ImGui::SameLine(120.f); ImGui::HelpMarker("|?|", "Full shader\nConstant name to\nbe used in the\nrule to be created.\nSwitch to use/not use."); ImGui::NextColumn();
	// Partial name
	ImGui::InputText(szPartialNameMatch.c_str(), &m_sPageGameShaderRules.m_szPartialName, ImGuiInputTextFlags_None);
	ImGui::NextColumn(); ImGui::ToggleButton("Use_P", &m_sPageGameShaderRules.m_bPartialName);
	ImGui::SameLine(120.f); ImGui::HelpMarker("|?|", "Full shader\nConstant name to\nbe used in the\nrule to be created.\nSwitch to use/not use."); ImGui::NextColumn();
	// Start reg index
	ImGui::DragInt(szStartRegIndex.c_str(), (int*)&m_sPageGameShaderRules.m_dwStartRegIndex, 1.f, 0, MAX_DX9_CONSTANT_REGISTERS, "reg:%d", ImGuiSliderFlags_AlwaysClamp);
	ImGui::NextColumn(); ImGui::ToggleButton("Use_R", &m_sPageGameShaderRules.m_bStartRegIndex);
	ImGui::SameLine(120.f); ImGui::HelpMarker("|?|", "Shader Constant\nstart registere to\nbe used in the\nrule to be created.\nSwitch to use/not use."); ImGui::NextColumn();
	// operation to apply
	ImGui::Combo("Modification", (int*)&m_sPageGameShaderRules.m_dwOperationToApply, aszOperations);
	ImGui::HelpTooltip(GetMatrixModificationDesc(m_sPageGameShaderRules.m_dwOperationToApply).c_str());
	ImGui::NextColumn(); ImGui::Checkbox(szTranspose.c_str(), &m_sPageGameShaderRules.m_bTranspose);
	ImGui::SameLine(120.f); ImGui::HelpMarker("|?|", "Modification to\nbe applied\nto the rule\n to be created.\nSwitch checkbox to\ntranspose matrices."); ImGui::NextColumn();
	// register count
	ImGui::Combo("Register count", (int*)&m_sPageGameShaderRules.m_dwRegisterCount, aszRegCounts);
	ImGui::NextColumn(); ImGui::HelpMarker("|?|", "Choose wether\nVectices or\nMatrices to be\nmodified in the\nrule.");

	ImGui::Columns(1);

	// Create shader rule
	if (ImGui::Button(szCreate.c_str()))
	{
		// create rule, first parse input data
		Vireio_Constant_Modification_Rule sRule = Vireio_Constant_Modification_Rule();

		// full name
		if (m_sPageGameShaderRules.m_bConstantName)
		{
			sRule.m_szConstantName = m_sPageGameShaderRules.m_szConstantName;
			sRule.m_bUseName = true;
		}

		// partial name
		if (m_sPageGameShaderRules.m_bPartialName)
		{
			sRule.m_szConstantName = m_sPageGameShaderRules.m_szPartialName;
			sRule.m_bUsePartialNameMatch = true;
		}

		// start register
		if (m_sPageGameShaderRules.m_bStartRegIndex)
		{
			sRule.m_dwStartRegIndex = m_sPageGameShaderRules.m_dwStartRegIndex;
			sRule.m_bUseStartRegIndex = true;
		}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		// buffer size
		if (m_sPageGameShaderRules.m_bBufferSize)
		{
			sRule.m_dwBufferSize = m_sPageGameShaderRules.m_bBufferSize;
			sRule.m_bUseBufferSize = true;
		}

		// buffer index
		if (m_sPageGameShaderRules.m_bBufferIndex)
		{
			sRule.m_dwBufferIndex = m_sPageGameShaderRules.m_dwBufferIndex;
			sRule.m_bUseBufferIndex = true;
		}
#endif

		// operation
		sRule.m_dwOperationToApply = m_sPageGameShaderRules.m_dwOperationToApply;

		// register count
		sRule.m_dwRegisterCount = m_sPageGameShaderRules.m_dwRegisterCount;

		// transpose
		sRule.m_bTranspose = m_sPageGameShaderRules.m_bTranspose;

		// create modification
		if (sRule.m_dwRegisterCount == 1)
			sRule.m_pcModification = CreateVector4Modification(sRule.m_dwOperationToApply, m_pcShaderModificationCalculation);
		else if (sRule.m_dwRegisterCount == 4)
			sRule.m_pcModification = CreateMatrixModification(sRule.m_dwOperationToApply, m_pcShaderModificationCalculation);
		else sRule.m_pcModification = nullptr;

		// add the rule
		m_asConstantRules.push_back(sRule);

		// update the control string list
		FillShaderRuleIndices();
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Create the\neventual shader\nrule."); ImGui::SameLine();
	// Delete shader rule
	if (ImGui::Button(szDelete.c_str()))
	{
		if ((nItem_ShaderRuleGeneralIndices >= 0) && (nItem_ShaderRuleGeneralIndices < (INT)m_aszShaderRuleIndices.size()))
		{
			// erase index
			m_aunGlobalConstantRuleIndices.erase(m_aunGlobalConstantRuleIndices.begin() + nItem_ShaderRuleGeneralIndices);

#ifdef VIREIO_D3D9
			// init all shaders new
			for (Vireio_D3D9_Shader& sShader : m_sModifierData.asVShaders)
				InitShaderRules(&sShader);
			for (Vireio_D3D9_Shader& sShader : m_sModifierData.asPShaders)
				InitShaderRules(&sShader);
#endif

			// update list
			FillShaderRuleGeneralIndices();
		}
		if (nItem_ShaderRuleIndices >= 0)
		{
			// erase full general indices list
			m_aunGlobalConstantRuleIndices = std::vector<UINT>();

			// erase index
			m_asConstantRules.erase(m_asConstantRules.begin() + nItem_ShaderRuleIndices);

#ifdef VIREIO_D3D9
			// init all shaders new
			for (Vireio_D3D9_Shader& sShader : m_sModifierData.asVShaders)
				InitShaderRules(&sShader);
			for (Vireio_D3D9_Shader& sShader : m_sModifierData.asPShaders)
				InitShaderRules(&sShader);
#endif

			// update lists
			FillShaderRuleIndices();
			FillShaderRuleGeneralIndices();
		}
		else if (nItem_ShaderRuleShaderIndices >= 0)
		{
			// TODO !!!
		}
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Delete the\nchosen shader\nrule.\n(chose from any\nlist)"); ImGui::SameLine();
	// Delete shader rule
	if (ImGui::Button(szToGeneral.c_str()))
	{
		// increase the update counter to reverify the constant buffers for rules
		m_dwConstantRulesUpdateCounter++;

		// in range ? add to general rules
		if ((nItem_ShaderRuleIndices >= 0) && (nItem_ShaderRuleIndices < (INT)m_aszShaderRuleIndices.size()))
			m_aunGlobalConstantRuleIndices.push_back((UINT)nItem_ShaderRuleIndices);

#ifdef VIREIO_D3D9
		// init all shaders new
		for (Vireio_D3D9_Shader& sShader : m_sModifierData.asVShaders)
			InitShaderRules(&sShader);
		for (Vireio_D3D9_Shader& sShader : m_sModifierData.asPShaders)
			InitShaderRules(&sShader);
#endif

		// update the list
		FillShaderRuleGeneralIndices();
	}
	ImGui::SameLine(); ImGui::HelpMarker("|?|", "Add to general\n(all shaders)\nindices."); /*ImGui::SameLine();*/
	// TODO !! SHADER SPECIFIC RULES !!

	// all shader rule indices/identifiers
	if (m_aszShaderRuleIndices.size())
	{
		if (ImGui::ListBox("Shader rule indices\n(index = id)", &nItem_ShaderRuleIndices, m_aszShaderRuleIndices, 4))
		{
			// in range ? call the method to fill the rule data
			if ((nItem_ShaderRuleIndices >= 0) && (nItem_ShaderRuleIndices < (INT)m_aszShaderRuleIndices.size()))
				FillShaderRuleData((UINT)nItem_ShaderRuleIndices);

			// unselect general + shader-specific
			nItem_ShaderRuleGeneralIndices = -1;
			nItem_ShaderRuleShaderIndices = -1;
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "All created shader\nrules indices. Each\nindex is the rule\nidentifier as well.");
	}
	// shader rule data text output as list...
	if (m_aszShaderRuleData.size())
	{
		for (std::string acText : m_aszShaderRuleData)
			ImGui::TextUnformatted(acText.c_str());
		ImGui::SameLine();
		if (m_szShaderRuleDesc.size())
			ImGui::HelpMarker("|?|", m_szShaderRuleDesc.c_str());
		else
			ImGui::HelpMarker("|?|", "The data of the\nchosen shader rule.");
	}
	// shader rule indices general
	if (m_aszShaderRuleGeneralIndices.size())
	{
		if (ImGui::ListBox("Shader rule indices\n(general)", &nItem_ShaderRuleGeneralIndices, m_aszShaderRuleGeneralIndices, 4))
		{
			// unselect shader-rule + shader-specific
			nItem_ShaderRuleIndices = -1;
			nItem_ShaderRuleShaderIndices = -1;
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "All shader rule\nindices applied to\nall shaders.");
	}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#elif defined(VIREIO_D3D9)
	// shader rule indices shader specific
	if (m_aszShaderRuleShaderIndices.size())
	{
		if (ImGui::ListBox("Shader rule indices\n(general)", &nItem_ShaderRuleShaderIndices, m_aszShaderRuleShaderIndices, 4))
		{
			// set general to -1
			nItem_ShaderRuleGeneralIndices = -1;
		}
		ImGui::SameLine(); ImGui::HelpMarker("|?|", "Shader rule\nindices applied to\nspecific shaders.");
	}
#endif
#pragma endregion

	return;
}

/// <summary> 
/// Handles the debug trace.
/// </summary>
void MatrixModifier::DebugOutput(const void* pvSrcData, UINT dwShaderIndex, UINT dwBufferIndex, UINT dwBufferSize)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// which shader is chosen ?
	/*std::vector<Vireio_D3D11_Shader>* pasShaders;
	std::vector<std::wstring>* pasShaderHashCodes;
	std::vector<UINT>* padwShaderHashCodes;
	if (m_eChosenShaderType == Vireio_Supported_Shaders::VertexShader)
	{
		pasShaders = &m_asVShaders;
		pasShaderHashCodes = &m_aszVShaderHashCodes;
		padwShaderHashCodes = &m_adwVShaderHashCodes;
	}
	else if (m_eChosenShaderType == Vireio_Supported_Shaders::PixelShader)
	{
		pasShaders = &m_asPShaders;
		pasShaderHashCodes = &m_aszPShaderHashCodes;
		padwShaderHashCodes = &m_adwPShaderHashCodes;
	}

	// get current selection of the shader constant list
	INT nSelection = 0;
	nSelection = m_pcVireioGUI->GetCurrentSelection(m_sPageDebug.m_dwShaderConstants);
	if ((nSelection < 0) || (nSelection >= (INT)m_aszShaderConstantsA.size()))
	{
		m_bGrabDebug = false;
		return;
	}

	// loop through the shader constants
	for (size_t nConstant = 0; nConstant < (*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
	{
		// add the debug data by type
		switch (m_eDebugOption)
		{
		case Debug_ConstantFloat4:
			// test the name of the constant
			if (std::strstr((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
			{
				UINT dwSize = sizeof(float) * 4;

				// is this  in range ?
				if (dwBufferSize >= ((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
				{
					// get pointers to the data
					UINT_PTR pv = (UINT_PTR)pvSrcData + (*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
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
			if (std::strstr((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
			{
				UINT dwSize = sizeof(float) * 4 * 2;

				// is this  in range ?
				if (dwBufferSize >= ((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
				{
					// get pointers to the data
					UINT_PTR pv = (UINT_PTR)pvSrcData + (*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
					D3DXVECTOR4 sVector4 = D3DXVECTOR4((CONST FLOAT*)pv);
					pv = (UINT_PTR)pvSrcData + (*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + sizeof(D3DVECTOR);
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
			if (std::strstr((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_aszShaderConstantsA[nSelection].c_str()))
			{
				UINT dwSize = sizeof(float) * 4 * 4;

				// is this  in range ?
				if (dwBufferSize >= ((*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + dwSize))
				{
					// get pointers to the data
					UINT_PTR pv = (UINT_PTR)pvSrcData + (*pasShaders)[dwShaderIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
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
	}*/
#endif
}

/// <summary>
/// Fills list box for (global) shader rule indices
/// </summary>
void MatrixModifier::FillShaderRuleIndices()
{
	m_aszShaderRuleIndices = std::vector<std::string>();
	for (UINT dwI = 0; dwI < (UINT)m_asConstantRules.size(); dwI++)
	{
		// we use just the index as identifier
		std::stringstream szIndex; szIndex << "Rule : " << dwI;
		m_aszShaderRuleIndices.push_back(szIndex.str());
	}
}

/// <summary>
/// Fills the info list for a specified shader rule
/// </summary>
void MatrixModifier::FillShaderRuleData(UINT dwRuleIndex)
{
	if (dwRuleIndex >= (UINT)m_asConstantRules.size())
		return;

	m_aszShaderRuleData = std::vector<std::string>();
	m_szShaderRuleDesc = std::string();

	// add the data strings
	std::stringstream szIndex;
	szIndex << "Rule : " << dwRuleIndex;
	m_aszShaderRuleData.push_back(szIndex.str());

	// name ?
	if (m_asConstantRules[dwRuleIndex].m_bUseName)
	{
		std::stringstream szName;
		szName << "Constant Name : " << m_asConstantRules[dwRuleIndex].m_szConstantName.c_str();
		m_aszShaderRuleData.push_back(szName.str());
	}

	// partial name ?
	if (m_asConstantRules[dwRuleIndex].m_bUsePartialNameMatch)
	{
		std::stringstream szName;
		szName << "Partial Name : " << m_asConstantRules[dwRuleIndex].m_szConstantName.c_str();
		m_aszShaderRuleData.push_back(szName.str());
	}

	// start reg index ?
	if (m_asConstantRules[dwRuleIndex].m_bUseStartRegIndex)
	{
		std::stringstream szRegIndex;
		szRegIndex << "Register : " << m_asConstantRules[dwRuleIndex].m_dwStartRegIndex;
		m_aszShaderRuleData.push_back(szRegIndex.str());
	}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// buffer index ?
	if (m_asConstantRules[dwRuleIndex].m_bUseBufferIndex)
	{
		std::stringstream szBufferIndex;
		szBufferIndex << L"Buffer Index : " << m_asConstantRules[dwRuleIndex].m_dwBufferIndex;
		m_aszShaderRuleData.push_back(szBufferIndex.str());
	}

	// buffer size ?
	if (m_asConstantRules[dwRuleIndex].m_bUseBufferSize)
	{
		std::stringstream szBufferSize;
		szBufferSize << L"Buffer Size : " << m_asConstantRules[dwRuleIndex].m_dwBufferSize;
		m_aszShaderRuleData.push_back(szBufferSize.str());
	}
#endif

	// reg count ?
	{
		std::stringstream szRegCount;
		szRegCount << "Reg Count : " << m_asConstantRules[dwRuleIndex].m_dwRegisterCount;

		if (m_asConstantRules[dwRuleIndex].m_dwRegisterCount == 1)
			szRegCount << " (Vector4f)";
		else if (m_asConstantRules[dwRuleIndex].m_dwRegisterCount == 4)
			szRegCount << " (Matrix4x4f)";

		m_aszShaderRuleData.push_back(szRegCount.str());
	}

	// transpose ?
	{
		std::stringstream szTranspose;
		if (m_asConstantRules[dwRuleIndex].m_bTranspose)
			szTranspose << "Transpose : TRUE";
		else
			szTranspose << "Transpose : FALSE";
		m_aszShaderRuleData.push_back(szTranspose.str());
	}

	// operation
	{
		if (m_asConstantRules[dwRuleIndex].m_dwRegisterCount == 1)
			m_aszShaderRuleData.push_back("X");// ShaderConstantModificationFactory::GetVector4ModificationName(m_asConstantRules[dwRuleIndex].m_dwOperationToApply)); /// TODO !!
		else if (m_asConstantRules[dwRuleIndex].m_dwRegisterCount == 4)
		{
			m_aszShaderRuleData.push_back(GetMatrixModificationName(m_asConstantRules[dwRuleIndex].m_dwOperationToApply));
			m_szShaderRuleDesc = GetMatrixModificationDesc(m_asConstantRules[dwRuleIndex].m_dwOperationToApply);
		}
	}
}

/// <summary>
/// Fills the general shader rule indices info list
/// </summary>
void MatrixModifier::FillShaderRuleGeneralIndices()
{
	m_aszShaderRuleGeneralIndices = std::vector<std::string>();
	for (UINT dwI = 0; dwI < (UINT)m_aunGlobalConstantRuleIndices.size(); dwI++)
	{
		// add text
		std::stringstream szIndex; szIndex << "Rule : " << m_aunGlobalConstantRuleIndices[dwI];
		m_aszShaderRuleGeneralIndices.push_back(szIndex.str());
	}
}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
/// <summary>
///
/// </summary>
void MatrixModifier::FillFetchedHashCodeList()
{
	m_aszFetchedHashCodes = std::vector<std::string>();
	for (UINT dwI = 0; dwI < (UINT)m_aunFetchedHashCodes.size(); dwI++)
	{
		// add text
		std::stringstream szIndex; szIndex << L"Hash : " << m_aunFetchedHashCodes[dwI];
		m_aszFetchedHashCodes.push_back(szIndex.str());
	}
}
#endif
#if defined(VIREIO_D3D11)
/// <summary> 
/// Handles all constant buffer setting methods for all DX11 shaders.
/// Called by VSSetConstantBuffers, GSSetConstantBuffers, HSSetConstantBuffers, DSSetConstantBuffers and PSSetConstantBuffers.
/// </summary>
void MatrixModifier::XSSetConstantBuffers(ID3D11DeviceContext* pcContext, std::array<ID3D11Buffer*, BUFFER_REGISTER_R << 1>& apcActiveConstantBuffers, UINT dwStartSlot, UINT dwNumBuffers, ID3D11Buffer* const* ppcConstantBuffers, Vireio_Supported_Shaders eShaderType)
{
	// loop through the new buffers
	for (UINT dwIndex = 0; dwIndex < dwNumBuffers; dwIndex++)
	{
		// get internal index
		UINT dwInternalIndex = dwIndex + dwStartSlot;

		// in range ? 
		if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
		{
			// set buffer internally 
			apcActiveConstantBuffers[dwInternalIndex] = ppcConstantBuffers[dwIndex];

			if (apcActiveConstantBuffers[dwInternalIndex])
			{
				// get private rule index from buffer
				Vireio_Buffer_Rules_Index sRulesIndex;
				sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
				sRulesIndex.m_dwUpdateCounter = 0;
				UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
				apcActiveConstantBuffers[dwInternalIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);

				// set twin for right side, first get the private data interface
				ID3D11Buffer* pcBuffer = nullptr;
				UINT dwSize = sizeof(pcBuffer);
				apcActiveConstantBuffers[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

				// stereo buffer and rules index present ?
				if ((pcBuffer) && (dwDataSizeRulesIndex) && (sRulesIndex.m_nRulesIndex >= 0))
				{
					// set right buffer as active buffer
					apcActiveConstantBuffers[dwInternalIndex + BUFFER_REGISTER_R] = pcBuffer;
				}
				else
				{
					// no buffer or no shader rules assigned ? left = right side -> right = left side
					apcActiveConstantBuffers[dwInternalIndex + BUFFER_REGISTER_R] = apcActiveConstantBuffers[dwInternalIndex];

					// verify buffer
					if ((pcBuffer) && (sRulesIndex.m_nRulesIndex == VIREIO_CONSTANT_RULES_NOT_ADDRESSED))
						VerifyConstantBuffer(apcActiveConstantBuffers[dwInternalIndex], dwInternalIndex, eShaderType);
				}

				// no stereo buffer present ?
				if (!pcBuffer)
				{
					D3D11_BUFFER_DESC sDesc;
					apcActiveConstantBuffers[dwInternalIndex]->GetDesc(&sDesc);

					// ignore immutable buffers
					if (sDesc.Usage != D3D11_USAGE_IMMUTABLE)
					{
						// create stereo constant buffer, first get device
						ID3D11Device* pcDevice = nullptr;
						apcActiveConstantBuffers[dwInternalIndex]->GetDevice(&pcDevice);
						if (pcDevice)
						{
							CreateStereoBuffer(pcDevice, pcContext, (ID3D11Buffer*)apcActiveConstantBuffers[dwInternalIndex], &sDesc, NULL, true);
							pcDevice->Release();
						}
					}
				}
				else
					pcBuffer->Release();
			}
			else
				apcActiveConstantBuffers[dwInternalIndex + BUFFER_REGISTER_R] = nullptr;
		}
	}
}

/// <summary> 
/// Verifies a stereo constant buffer for shader rules and assigns them in case.
/// @param pcBuffer The constant buffer to be verified.
/// </summary>
void MatrixModifier::VerifyConstantBuffer(ID3D11Buffer* pcBuffer, UINT dwBufferIndex, Vireio_Supported_Shaders eShaderType)
{
	// buffer already verified ?
	Vireio_Buffer_Rules_Index sRulesIndex;
	sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
	sRulesIndex.m_dwUpdateCounter = 0;
	UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
	pcBuffer->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);
	if (dwDataSizeRulesIndex)
	{
		// if the update counter has increased set to "NOT ADDRESSED"
		if (sRulesIndex.m_dwUpdateCounter < m_dwConstantRulesUpdateCounter)
			sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

		// continue only if constant rules not addressed
		if (sRulesIndex.m_nRulesIndex != VIREIO_CONSTANT_RULES_NOT_ADDRESSED)
			return;
	}

	// get buffer size by description
	D3D11_BUFFER_DESC sDesc;
	pcBuffer->GetDesc(&sDesc);
	UINT dwBufferSize = sDesc.ByteWidth;

	// get the register size of the buffer
	UINT dwBufferRegisterSize = dwBufferSize >> 5;

	// not addressed ? address
	if (sRulesIndex.m_nRulesIndex == VIREIO_CONSTANT_RULES_NOT_ADDRESSED)
	{
		// create a vector for this constant buffer
		std::vector<Vireio_Constant_Rule_Index> asConstantBufferRules = std::vector<Vireio_Constant_Rule_Index>();

		// loop through all global rules
		for (UINT dwI = 0; dwI < (UINT)m_aunGlobalConstantRuleIndices.size(); dwI++)
		{
			// get a bool for each register index, set to false
			std::vector<bool> abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);

			if ((m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseName) || (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUsePartialNameMatch))
			{
				Vireio_Shader_Private_Data sPrivateData;
				UINT dwDataSize = sizeof(sPrivateData);
				pcBuffer->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
				if (dwDataSize)
				{
					// use name ?
					if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseName)
					{
						switch (eShaderType)
						{
						case VertexShader:
							// shader has this buffer index ?
							if (dwBufferIndex < m_asVShaders[sPrivateData.dwIndex].asBuffers.size())
							{
								// loop through the shader constants
								for (UINT nConstant = 0; nConstant < (UINT)m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
								{
									// test full name matching
									if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_szConstantName.compare(m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName) == 0)
									{
										// set this register to 'false' if not matching
										UINT dwRegister = m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5;
										if (dwRegister <= dwBufferRegisterSize)
											abRegistersMatching[m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5] = true;
									}
								}
							}
							break;
						case PixelShader:
							break;
						case GeometryShader:
							break;
						case HullShader:
							break;
						case DomainShader:
							break;
						default:
							break;
						}
					}

					// use partial name ?
					if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUsePartialNameMatch)
					{
						switch (eShaderType)
						{
						case VertexShader:
							// shader has this buffer index ?
							if (dwBufferIndex < m_asVShaders[sPrivateData.dwIndex].asBuffers.size())
							{
								// loop through the shader constants
								for (UINT nConstant = 0; nConstant < (UINT)m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
								{
									// test partial name matching
									if (std::strstr(m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_szConstantName.c_str()))
									{
										// set this register to 'false' if not matching
										UINT dwRegister = m_asVShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset >> 5;
										if (dwRegister <= dwBufferRegisterSize)
											abRegistersMatching[dwRegister] = true;
									}
								}
							}
							break;
						case PixelShader:
							break;
						case GeometryShader:
							break;
						case HullShader:
							break;
						case DomainShader:
							break;
						default:
							break;
						}
					}
				}
			}

			// use start reg index ?
			if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseStartRegIndex)
			{
				UINT dwRegister = m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_dwStartRegIndex;
				if ((dwBufferRegisterSize) && (dwRegister <= dwBufferRegisterSize))
				{
					bool bOld = abRegistersMatching[dwRegister];
					abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
					abRegistersMatching[dwRegister] = bOld;

					// set to true if no naming convention 
					if ((!m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseName) && (!m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUsePartialNameMatch))
						abRegistersMatching[dwRegister] = true;
				}
				else
					abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
			}

			// use buffer index
			if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseBufferIndex)
			{
				switch (eShaderType)
				{
				case VertexShader:
					if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_dwBufferIndex != dwBufferIndex)
						abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
					break;
				case PixelShader:
					break;
				case GeometryShader:
					break;
				case HullShader:
					break;
				case DomainShader:
					break;
				default:
					break;
				}
			}

			// use buffer size
			if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_bUseBufferSize)
			{
				if (m_asConstantRules[m_aunGlobalConstantRuleIndices[dwI]].m_dwBufferSize != dwBufferSize)
					abRegistersMatching = std::vector<bool>(dwBufferRegisterSize, false);
			}

			// loop through registers and create the rules
			for (UINT dwJ = 0; dwJ < dwBufferRegisterSize; dwJ++)
			{
				// register matches the rule ?
				if (abRegistersMatching[dwJ])
				{
					Vireio_Constant_Rule_Index sIndex;
					sIndex.m_dwIndex = m_aunGlobalConstantRuleIndices[dwI];
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
					sRulesIndex.m_nRulesIndex = (INT)dwI;
					bPresent = true;
				}
			}
		}

		// no rules found ? set to unavailable
		if (!asConstantBufferRules.size())
		{
			sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_AVAILABLE;
		}
		// rules not present ?
		else if (!bPresent)
		{
			// set index... current vector size
			sRulesIndex.m_nRulesIndex = (INT)m_aasConstantBufferRuleIndices.size();

			// and add
			m_aasConstantBufferRuleIndices.push_back(asConstantBufferRules);
		}
	}

	// set the rules index as private data to the constant buffer, first update the update counter
	sRulesIndex.m_dwUpdateCounter = m_dwConstantRulesUpdateCounter;
	pcBuffer->SetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, sizeof(Vireio_Buffer_Rules_Index), &sRulesIndex);
}

/// <summary> 
/// Modifies left and right constant data using specified shader rules.
/// @param nRulesIndex The index of the shader rules this constant buffer is modified with.
/// @param pdwLeft The buffer data for the left constant buffer. Data must match right buffer data.
/// @param pdwRight The buffer data for the right constant buffer. Data must match left buffer data.
/// @param dwBufferSize The size of this buffer, in bytes.
/// </summary>
void MatrixModifier::DoBufferModification(INT nRulesIndex, UINT_PTR pdwLeft, UINT_PTR pdwRight, UINT dwBufferSize)
{
	// do modifications
	if (nRulesIndex >= 0)
	{
		// loop through rules for that constant buffer
		for (UINT dwI = 0; dwI < (UINT)m_aasConstantBufferRuleIndices[nRulesIndex].size(); dwI++)
		{
			UINT dwIndex = m_aasConstantBufferRuleIndices[nRulesIndex][dwI].m_dwIndex;
			UINT dwRegister = m_aasConstantBufferRuleIndices[nRulesIndex][dwI].m_dwConstantRuleRegister;

			// TODO !! VECTOR MODIFICATION, MATRIX2x4 MODIFICATION

			// is this modification in range ?
			if ((dwBufferSize >= dwRegister * 4 * sizeof(float) + sizeof(D3DMATRIX)))
			{
				// get pointers to the matrix (left+right)
				UINT_PTR pvLeft = (UINT_PTR)pdwLeft + dwRegister * 4 * sizeof(float);
				UINT_PTR pvRight = (UINT_PTR)pdwRight + dwRegister * 4 * sizeof(float);

				// get the matrix
				D3DXMATRIX sMatrix = D3DXMATRIX((CONST FLOAT*)pvLeft);

				// do matrix modification
				if (m_asConstantRules[dwIndex].m_pcModification)
				{
					// matrix to be transposed ?
					if (m_asConstantRules[dwIndex].m_bTranspose)
					{
						D3DXMatrixTranspose(&sMatrix, &sMatrix);
					}

					// do modification
					std::array<float, 16> afMatrixLeft, afMatrixRight;
					((ShaderMatrixModification*)m_asConstantRules[dwIndex].m_pcModification.get())->ApplyModification(sMatrix, &afMatrixLeft, &afMatrixRight);
					D3DXMATRIX sMatrixLeft(afMatrixLeft.data());
					D3DXMATRIX sMatrixRight(afMatrixRight.data());

					// transpose back
					if (m_asConstantRules[dwIndex].m_bTranspose)
					{
						D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
						D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
					}

					memcpy((void*)pvLeft, &sMatrixLeft, sizeof(D3DXMATRIX));
					memcpy((void*)pvRight, &sMatrixRight, sizeof(D3DXMATRIX));
				}
			}
		}
	}
}

/// <summary> 
/// Handles CreateVertexShader() and CreatePixelShader() calls.
/// </summary>
void MatrixModifier::CreateShader(std::vector<Vireio_D3D11_Shader>* pasShaders, const void* pcShaderBytecode, SIZE_T unBytecodeLength, ID3D11ClassLinkage* pcClassLinkage, ID3D11DeviceChild** ppcShader, bool bOutputCode, char cPrefix)
{
	// get the shader pointer
	ID3D11DeviceChild* pcShader = nullptr;
	if (ppcShader)
		if (*ppcShader)
			pcShader = *ppcShader;
	if (pcShader)
	{
		// get the hash code
		DWORD dwHashCode = GetHashCode((BYTE*)pcShaderBytecode, (DWORD)unBytecodeLength, VIREIO_SEED);

		// is this shader already enumerated ?
		for (size_t nShaderDescIndex = 0; nShaderDescIndex < (*pasShaders).size(); nShaderDescIndex++)
		{
			if (dwHashCode == (*pasShaders)[nShaderDescIndex].uHash)
			{
				// create and set private shader data
				Vireio_Shader_Private_Data sPrivateData;
				sPrivateData.dwHash = dwHashCode;
				sPrivateData.dwIndex = (UINT)nShaderDescIndex;

				pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);
				return;
			}
		}

		// shader data for each shader
		Vireio_D3D11_Shader sShaderData = {};

		// first, disassemble shader
		ID3DBlob* pcIDisassembly = nullptr;
		HRESULT hr = D3DDisassemble(pcShaderBytecode, (DWORD)unBytecodeLength, D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS, 0, &pcIDisassembly);

		// get the shader code to a stringstream
		std::stringstream szStream = std::stringstream();
		szStream << static_cast<char*>(pcIDisassembly->GetBufferPointer());
		std::string szLine;

		// get all constant buffers from the stream
		while (std::getline(szStream, szLine))
		{
			if (szLine.find("dcl_constantbuffer") != std::string::npos)
			{
				Vireio_D3D11_Constant_Buffer_Unaccounted sBufferUnaccounted;

				// dcl_constantbuffer cb12[37], immediateIndexed
				std::stringstream szLineStream(szLine);
				std::string szTemp;
				std::getline(szLineStream, szTemp, ' ');
				std::getline(szLineStream, szTemp, '[');
				std::stringstream szIndex(szTemp);
				szIndex.ignore(2);
				szIndex >> sBufferUnaccounted.dwRegister;

				// get size between '[' and ']'
				std::getline(szLineStream, szTemp, ']');
				std::stringstream szSize(szTemp);
				szSize >> sBufferUnaccounted.dwSize;

				// no constant rules addressed at shader creation
				sBufferUnaccounted.nConstantRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

				// store the unaccounted buffer
				sShaderData.asBuffersUnaccounted.push_back(sBufferUnaccounted);
			}
		}

		// create reflection class
		ID3D11ShaderReflection* pcReflector = NULL;
		if (SUCCEEDED(D3DReflect(pcShaderBytecode,
			unBytecodeLength,
			IID_ID3D11ShaderReflection,
			(void**)&pcReflector)))
		{
			// get desc
			D3D11_SHADER_DESC sDesc;
			pcReflector->GetDesc(&sDesc);

			// fill shader data
			sShaderData.uConstantBuffers = sDesc.ConstantBuffers;
			sShaderData.uVersion = sDesc.Version;
			sShaderData.uBoundResources = sDesc.BoundResources;
			sShaderData.uInputParameters = sDesc.InputParameters;
			sShaderData.uOutputParameters = sDesc.OutputParameters;
			sShaderData.uHash = dwHashCode;

			// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
			UINT dwLen = (UINT)strnlen_s(sDesc.Creator, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
			sShaderData.atCreator = std::string(dwLen + 1, 0);
			CopyMemory((char*)sShaderData.atCreator.data(), sDesc.Creator, sizeof(CHAR) * dwLen);

			for (UINT dwIndex = 0; dwIndex < sDesc.ConstantBuffers; dwIndex++)
			{
				// get next constant buffer
				ID3D11ShaderReflectionConstantBuffer* pcConstantBuffer = pcReflector->GetConstantBufferByIndex(dwIndex);
				if (pcConstantBuffer)
				{
					// get desc
					D3D11_SHADER_BUFFER_DESC sDescBuffer;
					pcConstantBuffer->GetDesc(&sDescBuffer);

					// fill buffer data
					Vireio_D3D11_Constant_Buffer sBufferData;
					sBufferData.eType = sDescBuffer.Type;
					sBufferData.dwVariables = sDescBuffer.Variables;
					sBufferData.dwSize = sDescBuffer.Size;
					sBufferData.dwFlags = sDescBuffer.uFlags;

					// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
					dwLen = (UINT)strnlen_s(sDescBuffer.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
					CopyMemory(sBufferData.szName, sDescBuffer.Name, sizeof(CHAR) * dwLen);
					sBufferData.szName[dwLen] = 0;

					// enumerate variables
					for (UINT dwIndexVariable = 0; dwIndexVariable < sDescBuffer.Variables; dwIndexVariable++)
					{
						ID3D11ShaderReflectionVariable* pcVariable = pcConstantBuffer->GetVariableByIndex(dwIndexVariable);
						if (pcVariable)
						{
							// get desc
							D3D11_SHADER_VARIABLE_DESC sDescVariable;
							pcVariable->GetDesc(&sDescVariable);

							// fill variable data
							Vireio_D3D11_Shader_Variable sVariableData;
							sVariableData.dwSize = sDescVariable.Size;
							sVariableData.dwStartOffset = sDescVariable.StartOffset;

							// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
							dwLen = (UINT)strnlen_s(sDescVariable.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
							CopyMemory(sVariableData.szName, sDescVariable.Name, sizeof(CHAR) * dwLen);
							sVariableData.szName[dwLen] = 0;

							// TODO !! FILL DEFAULT VALUE sVariableData.pcDefaultValue (maybe we need this later)

#ifdef _GET_PROJECTION_MATRICES
							// quickly search for projection matrices here
							if (std::strstr(sDescVariable.Name, "roj"))
								OutputDebugStringA(sDescVariable.Name);
#endif
							// and add to buffer desc
							sBufferData.asVariables.push_back(sVariableData);
						}
					}

					// no constant rules addressed at shader creation
					sBufferData.nConstantRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

					// and add to shader desc
					sShaderData.asBuffers.push_back(sBufferData);
				}
			}

			pcReflector->Release();
		}

		// and add to shader vector
		(*pasShaders).push_back(sShaderData);

		// create and set private shader data
		Vireio_Shader_Private_Data sPrivateData;
		sPrivateData.dwHash = dwHashCode;
		sPrivateData.dwIndex = (UINT)(*pasShaders).size() - 1;

		pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);

		// output shader code ?
		if (bOutputCode)
		{
			// optionally, output shader code to "VS(hash).txt"
			char buf[32]; ZeroMemory(&buf[0], 32);
			sprintf_s(buf, "VS%u.txt", dwHashCode);
			buf[0] = cPrefix;
			std::ofstream oLogFile(buf, std::ios::ate);

			if (oLogFile.is_open())
			{
				if (SUCCEEDED(hr))
				{
					oLogFile << static_cast<char*>(pcIDisassembly->GetBufferPointer()) << std::endl;
					oLogFile << std::endl << std::endl;
					oLogFile << "// Shader Hash   : " << dwHashCode << std::endl;
				}
				oLogFile << std::endl;
				for (int i = 0; i < (int)sShaderData.asBuffers.size(); i++)
				{
					for (int j = 0; j < (int)sShaderData.asBuffers[i].asVariables.size(); j++)
						oLogFile << "// Shader Constant : Buffer - " << i << " - Variable - " << j << " - " << sShaderData.asBuffers[i].asVariables[j].szName << " " << sShaderData.asBuffers[i].asVariables[j].dwStartOffset << " " << sShaderData.asBuffers[i].asVariables[j].dwSize << std::endl;
				}
				oLogFile.close();
			}
		}
	}
	else OutputDebugString(L"MatrixModifier: Failed to reflect vertex shader !");
}
#endif
#if defined(VIREIO_D3D9)
/// <summary>
/// Fills the list of shader specific rule indices.
/// </summary>
void MatrixModifier::FillShaderRuleShaderIndices()
{
	m_aszShaderRuleShaderIndices = std::vector<std::string>();
	for (UINT dwI = 0; dwI < (UINT)m_asShaderSpecificRuleIndices.size(); dwI++)
	{
		// add text
		std::stringstream szIndex; szIndex << "Hash : " << m_asShaderSpecificRuleIndices[dwI].unHash << " Rule Ix: " << m_asShaderSpecificRuleIndices[dwI].unRuleIndex;
		m_aszShaderRuleShaderIndices.push_back(szIndex.str());
	}
}

/// <summary>
/// => Init Shader Rules
/// Inits or recreates all shader rule indices for a specified V/P-Shader 
/// </summary>
/// <param name="psShader">Shader pointer</param>
void MatrixModifier::InitShaderRules(Vireio_D3D9_Shader* psShader)
{
	// @see ShaderModificationRepository::GetModifiedConstantsF from Vireio < v3

	// clear constant rule vector
	psShader->asConstantRuleIndices = std::vector<Vireio_Constant_Rule_Index_DX9>();

	// clear register indices to max uint
	FillMemory(psShader->aunRegisterModificationIndex.data(), MAX_DX9_CONSTANT_REGISTERS * sizeof(UINT), 0xFF);

	// loop throught constants
	for (UINT unJ = 0; unJ < psShader->asConstantDescriptions.size(); unJ++)
	{
		// register count 1 (= vector) and 4 (= matrix) supported
		if (((psShader->asConstantDescriptions[unJ].eClass == D3DXPC_VECTOR) && (psShader->asConstantDescriptions[unJ].uRegisterCount == 1))
			|| (((psShader->asConstantDescriptions[unJ].eClass == D3DXPC_MATRIX_ROWS) || (psShader->asConstantDescriptions[unJ].eClass == D3DXPC_MATRIX_COLUMNS)) && (psShader->asConstantDescriptions[unJ].uRegisterCount == 4)))
		{
			// Check if any shader specific rules match this constant, first check shader specific indices
			bool bShaderSpecificRulePresent = false;
			for (UINT unK = 0; unK < (UINT)m_asShaderSpecificRuleIndices.size(); unK++)
			{
				// same hash code ?
				if (m_asShaderSpecificRuleIndices[unK].unHash == psShader->uHash)
				{
					// compare rule->description, break in case of success
					if (SUCCEEDED(VerifyConstantDescriptionForRule(&(m_asConstantRules[m_asShaderSpecificRuleIndices[unK].unRuleIndex]), &(psShader->asConstantDescriptions[unJ]), m_asShaderSpecificRuleIndices[unK].unRuleIndex, psShader)))
					{
						bShaderSpecificRulePresent = true;
						break;
					}
				}
			}

			// now check general indices if no shader specific rule got applied
			if (!bShaderSpecificRulePresent)
			{
				for (UINT unK = 0; unK < (UINT)m_aunGlobalConstantRuleIndices.size(); unK++)
				{
					// compare rule->description, break in case of success
					if (SUCCEEDED(VerifyConstantDescriptionForRule(&(m_asConstantRules[m_aunGlobalConstantRuleIndices[unK]]), &(psShader->asConstantDescriptions[unJ]), m_aunGlobalConstantRuleIndices[unK], psShader)))
						break;
				}
			}
		}
	}
}

/// <summary>
/// => Verify Constant Description
/// Verifies wether a specific modification rule
/// applies to a specific shader constant provided
/// by a (D3DX) constant description.
/// </summary>
/// <param name="psRule">modification rule</param>
/// <param name="psDescription">constant description</param>
/// <param name="unRuleIndex">the index of the modification rule</param>
/// <param name="psShader">Vireio D3D9 shader</param>
/// <returns>D3D_OK if match, E_NO_MATCH otherwise</returns>
HRESULT MatrixModifier::VerifyConstantDescriptionForRule(Vireio_Constant_Modification_Rule* psRule, SAFE_D3DXCONSTANT_DESC* psDescription, UINT unRuleIndex, Vireio_D3D9_Shader* psShader)
{
	// Type match
	if (psRule->m_dwRegisterCount == psDescription->uRegisterCount)
	{
		// name match required
		if (psRule->m_szConstantName.size() > 0)
		{
			bool bNameMatch = false;
			if (psRule->m_bUsePartialNameMatch)
				bNameMatch = std::strstr(psDescription->acName.c_str(), psRule->m_szConstantName.c_str()) != NULL;
			else
				bNameMatch = psRule->m_szConstantName.compare(psDescription->acName) == 0;

			// no match ?
			if (!bNameMatch)
				return E_NO_MATCH;
		}

		// register match required...
		if (psRule->m_bUseStartRegIndex)
		{
			if (psRule->m_dwStartRegIndex != psDescription->uRegisterIndex)
				return E_NO_MATCH;
		}

#ifdef TRACE_MODIFIER
		// output shader constant + index
		OutputDebugStringA(psDescription->acName.c_str());
		switch (psDescription->eClass)
		{
		case D3DXPC_VECTOR:
			OutputDebugString(L"VS: D3DXPC_VECTOR");
			break;
		case D3DXPC_MATRIX_ROWS:
			OutputDebugString(L"VS: D3DXPC_MATRIX_ROWS");
			break;
		case D3DXPC_MATRIX_COLUMNS:
			OutputDebugString(L"VS: D3DXPC_MATRIX_COLUMNS");
			break;
		default:
			OutputDebugString(L"VS: UNKNOWN_CONSTANT");
			break;
		}
		debugf("Register Index: %d", psDescription->uRegisterIndex);
#endif 
		// set register index
		psShader->aunRegisterModificationIndex[psDescription->uRegisterIndex] = (UINT)psShader->asConstantRuleIndices.size();

		// set constant rule index
		Vireio_Constant_Rule_Index_DX9 sConstantRuleIndex;
		sConstantRuleIndex.dwIndex = unRuleIndex;
		sConstantRuleIndex.dwConstantRuleRegister = psDescription->uRegisterIndex;
		sConstantRuleIndex.dwConstantRuleRegisterCount = psDescription->uRegisterCount;

		// init data if default value present
		size_t uSize = (size_t)psDescription->uRegisterCount * sizeof(float) * 4;
		if (psDescription->afDefaultValue.size() >= uSize)
		{
			memcpy(&sConstantRuleIndex.afConstantDataLeft[0], psDescription->afDefaultValue.data(), uSize);
			memcpy(&sConstantRuleIndex.afConstantDataRight[0], psDescription->afDefaultValue.data(), uSize);
		};

		psShader->asConstantRuleIndices.push_back(sConstantRuleIndex);

		// only the first matching rule is applied to a constant
		return S_OK;
	}
	else return E_NO_MATCH;
}

/// <summary>
/// => Set V/P Shader Constants Float
/// Handle VS/PS constant input method.
/// If the vector count is 4 (=Matrix size) it 
/// will look for matrix modification, otherwise
/// loops through the set registers.
/// </summary>
/// <param name="unStartRegister">D3D9 Method input</param>
/// <param name="pfConstantData">D3D9 Method input</param>
/// <param name="unVector4fCount">D3D9 Method input</param>
/// <param name="bModified">Set true if registers are dirty</param>
/// <param name="eRenderSide">Current render side</param>
/// <param name="afRegisters">Pointer to currently set register data</param>
/// <param name="psShader">Vireio D3D9 shader</param>
/// <returns>D3D_OK</returns>
HRESULT MatrixModifier::SetXShaderConstantF(UINT unStartRegister, const float* pfConstantData, UINT unVector4fCount, bool& bModified, RenderPosition eRenderSide, float* afRegisters, Vireio_D3D9_Shader* psShader)
{
	// no rules present ? return
	if (!psShader->asConstantRuleIndices.size()) return S_OK;

	// modification present for this index ? index for non-modified registers are set to 0xffffffff to allow zero index
	uint32_t unIndex = psShader->aunRegisterModificationIndex[unStartRegister];
	if ((unIndex < (UINT)psShader->asConstantRuleIndices.size()) && (unVector4fCount == 4))
	{
		// apply to left and right data
		bModified = true;

		// get the matrix
		D3DXMATRIX sMatrix(&afRegisters[RegisterIndex(unStartRegister)]);
		{
			// matrix to be transposed ?
			bool bTranspose = m_asConstantRules[psShader->asConstantRuleIndices[unIndex].dwIndex].m_bTranspose;
			if (bTranspose)
			{
				D3DXMatrixTranspose(&sMatrix, &sMatrix);
			}

			// do modification
			std::array<float, 16> afMatrixLeft, afMatrixRight;
			((ShaderMatrixModification*)m_asConstantRules[psShader->asConstantRuleIndices[unIndex].dwIndex].m_pcModification.get())->ApplyModification(sMatrix, &afMatrixLeft, &afMatrixRight);
			D3DXMATRIX sMatrixLeft(afMatrixLeft.data());
			D3DXMATRIX sMatrixRight(afMatrixRight.data());

			// transpose back
			if (bTranspose)
			{
				D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
				D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
			}

			psShader->asConstantRuleIndices[unIndex].asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
			psShader->asConstantRuleIndices[unIndex].asConstantDataRight = (D3DMATRIX)sMatrixRight;

			// copy modified data to buffer
			if (eRenderSide == RenderPosition::Left)
				memcpy(&psShader->afRegisterBuffer[0], &sMatrixLeft, sizeof(D3DMATRIX));
			else
				memcpy(&psShader->afRegisterBuffer[0], &sMatrixRight, sizeof(D3DMATRIX));
		}
	}
	else
	{
		// here we end up if more than 4 registers (=matrix) are set at once, or partially changed matrices (unlikely)
		auto it = psShader->asConstantRuleIndices.begin();
		while (it != psShader->asConstantRuleIndices.end())
		{
			// register in range ?
			if ((unStartRegister < ((*it).dwConstantRuleRegister + (*it).dwConstantRuleRegisterCount)) && ((unStartRegister + unVector4fCount) > (*it).dwConstantRuleRegister))
			{
				// apply to left and right data
				if (!bModified)
				{
					// set buffer...
					memcpy(&psShader->afRegisterBuffer[0], pfConstantData, unVector4fCount * 4 * sizeof(float));
					bModified = true;
				}
				UINT unStartRegisterConstant = (*it).dwConstantRuleRegister;

				// get the matrix
				D3DXMATRIX sMatrix(&afRegisters[RegisterIndex(unStartRegisterConstant)]);
				{
					// matrix to be transposed ?
					bool bTranspose = m_asConstantRules[it->dwIndex].m_bTranspose;
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrix, &sMatrix);
					}

					// do modification
					std::array<float, 16> afMatrixLeft, afMatrixRight;
					((ShaderMatrixModification*)m_asConstantRules[it->dwIndex].m_pcModification.get())->ApplyModification(sMatrix, &afMatrixLeft, &afMatrixRight);
					D3DXMATRIX sMatrixLeft(afMatrixLeft.data());
					D3DXMATRIX sMatrixRight(afMatrixRight.data());

					// transpose back
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
						D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
					}

					it->asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
					it->asConstantDataRight = (D3DMATRIX)sMatrixRight;

					// copy modified data to buffer
					if (eRenderSide == RenderPosition::Left)
					{
						if (unStartRegister <= unStartRegisterConstant)
							memcpy(&psShader->afRegisterBuffer[unStartRegisterConstant - unStartRegister], &sMatrixLeft, sizeof(D3DMATRIX));
						else
							OutputDebugString(L"[MAM] Unlikely case: partially changed matrices");
					}
					else
					{
						if (unStartRegister <= unStartRegisterConstant)
							memcpy(&psShader->afRegisterBuffer[unStartRegisterConstant - unStartRegister], &sMatrixRight, sizeof(D3DMATRIX));
						else
							OutputDebugString(L"[MAM] Unlikely case: partially changed matrices");
					}
				}
		}
			it++;
	}
}

	return D3D_OK;
}
#endif

#if defined(VIREIO_D3D11)
#pragma region /// => D3D11 methods

/*** MatrixModifier private D3D methods ***/
#define D3D11_MTH_PARAM(a, b, ix, pn) a pn b; if (ppIn[ix]) b = *(a*pn)ppIn[ix]; else return;
#define D3D11_MTH_PARAM_HR(a, b, ix, pn) a pn b; if (ppIn[ix]) b = *(a*pn)ppIn[ix]; else return E_FAIL;

/// <summary>D3D11 method call</summary>
void MatrixModifier::CopyResource(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CopyResource]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CopyResource]);
	D3D11_MTH_PARAM(ID3D11Resource, ppcDstResource, 0, **);
	D3D11_MTH_PARAM(ID3D11Resource, ppcSrcResource, 1, **);

	if (!*ppcDstResource) return;
	if (!*ppcSrcResource) return;
	{
		// get destination resource type
		D3D11_RESOURCE_DIMENSION eDimension;
		(*ppcDstResource)->GetType(&eDimension);

		// if buffer, get desc
		if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
		{
			D3D11_BUFFER_DESC sDescDst;
			((ID3D11Buffer*)*ppcDstResource)->GetDesc(&sDescDst);

			// set the same shader rules index (if present) for the destination as for the source
			Vireio_Buffer_Rules_Index sRulesIndex;
			sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
			sRulesIndex.m_dwUpdateCounter = 0;
			UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);
			if ((dwDataSizeRulesIndex) && (sRulesIndex.m_nRulesIndex >= 0))
			{
				((ID3D11Resource*)*ppcDstResource)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, sizeof(Vireio_Buffer_Rules_Index), &sRulesIndex);
			}

			// copy to both sides, if source is a mono buffer set source to stereo buffer
			ID3D11Resource* pcResourceTwinSrc = nullptr;
			UINT dwSize = sizeof(pcResourceTwinSrc);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinSrc);
			if (pcResourceTwinSrc)
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopyResource(pcResourceTwinDst, pcResourceTwinSrc);
					pcResourceTwinDst->Release();
				}
				pcResourceTwinSrc->Release();
			}
			else
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopyResource(pcResourceTwinDst, *ppcSrcResource);
					pcResourceTwinDst->Release();
				}
			}
		}
		// is this a texture ?
		else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
		{
			// get the stereo twin of the resource (texture)
			ID3D11Resource* pcResourceTwinSrc = nullptr;
			UINT dwSize = sizeof(pcResourceTwinSrc);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
			if (pcResourceTwinSrc)
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopyResource(pcResourceTwinDst, pcResourceTwinSrc);
					pcResourceTwinDst->Release();
				}
				else
				{
					// set private data "create new" BOOL
					BOOL bTrue = TRUE;
					((ID3D11Resource*)*ppcDstResource)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
				}
				pcResourceTwinSrc->Release();
			}
			else
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopyResource(pcResourceTwinDst, *ppcSrcResource);
					pcResourceTwinDst->Release();
				}
			}
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::CopySubresourceRegion(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CopySubresourceRegion]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CopySubresourceRegion]);
	D3D11_MTH_PARAM(ID3D11Resource, ppcDstResource, 0, **);
	D3D11_MTH_PARAM(UINT, puDstSubresource, 1, *);
	D3D11_MTH_PARAM(UINT, puDstX, 2, *);
	D3D11_MTH_PARAM(UINT, puDstY, 3, *);
	D3D11_MTH_PARAM(UINT, puDstZ, 4, *);
	D3D11_MTH_PARAM(ID3D11Resource, ppcSrcResource, 5, **);
	D3D11_MTH_PARAM(UINT, puSrcSubresource, 6, *);
	D3D11_MTH_PARAM(D3D11_BOX, ppsSrcBox, 7, **);
	if (!*ppcDstResource) return;
	if (!*ppcSrcResource) return;
	{
		// get destination resource type
		D3D11_RESOURCE_DIMENSION eDimension;
		(*ppcDstResource)->GetType(&eDimension);

		// if buffer, get desc
		if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
		{
			if (!*ppsSrcBox) return;

			D3D11_BUFFER_DESC sDescDst;
			((ID3D11Buffer*)*ppcDstResource)->GetDesc(&sDescDst);

			// set the same shader rules index (if present) for the destination as for the source
			Vireio_Buffer_Rules_Index sRulesIndex;
			sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
			sRulesIndex.m_dwUpdateCounter = 0;
			UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);
			if ((dwDataSizeRulesIndex) && (sRulesIndex.m_nRulesIndex >= 0))
			{
				((ID3D11Resource*)*ppcDstResource)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, sizeof(Vireio_Buffer_Rules_Index), &sRulesIndex);
			}

			// copy to both sides, if source is a mono buffer set source to stereo buffer
			ID3D11Resource* pcResourceTwinSrc = nullptr;
			UINT dwSize = sizeof(pcResourceTwinSrc);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinSrc);
			if (pcResourceTwinSrc)
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopySubresourceRegion(pcResourceTwinDst,
						*puDstSubresource,
						*puDstX,
						*puDstY,
						*puDstZ,
						pcResourceTwinSrc,
						*puSrcSubresource,
						*ppsSrcBox);

					pcResourceTwinDst->Release();
				}
				pcResourceTwinSrc->Release();
			}
			else
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopySubresourceRegion(pcResourceTwinDst,
						*puDstSubresource,
						*puDstX,
						*puDstY,
						*puDstZ,
						*ppcSrcResource,
						*puSrcSubresource,
						*ppsSrcBox);

					pcResourceTwinDst->Release();
				}
			}
		}
		// is this a texture ?
		else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
		{
			// get the stereo twin of the resource (texture)
			ID3D11Resource* pcResourceTwinSrc = nullptr;
			UINT dwSize = sizeof(pcResourceTwinSrc);
			((ID3D11Resource*)*ppcSrcResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
			if (pcResourceTwinSrc)
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopySubresourceRegion(pcResourceTwinDst,
						*puDstSubresource,
						*puDstX,
						*puDstY,
						*puDstZ,
						pcResourceTwinSrc,
						*puSrcSubresource,
						*ppsSrcBox);

					pcResourceTwinDst->Release();
				}
				else
				{
					// set private data "create new" BOOL
					BOOL bTrue = TRUE;
					((ID3D11Resource*)*ppcDstResource)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
				}
				pcResourceTwinSrc->Release();
			}
			else
			{
				// get the stereo twin of the destination
				ID3D11Resource* pcResourceTwinDst = nullptr;
				dwSize = sizeof(pcResourceTwinDst);
				((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
				if (pcResourceTwinDst)
				{
					// do the copy call on the stereo twins of these textures
					m_pcContextCurrent->CopySubresourceRegion(pcResourceTwinDst,
						*puDstSubresource,
						*puDstX,
						*puDstY,
						*puDstZ,
						*ppcSrcResource,
						*puSrcSubresource,
						*ppsSrcBox);

					pcResourceTwinDst->Release();
				}
			}
		}
	}
}

/// <summary>D3D11 method call</summary>
HRESULT MatrixModifier::CreateBuffer(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CreateBuffer]) return E_FAIL;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CreateBuffer]);
	D3D11_MTH_PARAM_HR(D3D11_BUFFER_DESC, ppsDesc, 0, **);
	D3D11_MTH_PARAM_HR(D3D11_SUBRESOURCE_DATA, ppsInitialData, 1, **);
	D3D11_MTH_PARAM_HR(ID3D11Buffer, pppcBuffer, 2, ***);

	// is this a constant buffer ?
	if (!*ppsDesc) return E_FAIL;
	if (((*ppsDesc)->BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
	{
		// create the buffer and create stereo buffers
		HRESULT nHr = m_pcDeviceCurrent->CreateBuffer(*ppsDesc, *ppsInitialData, *pppcBuffer);
		if (SUCCEEDED(nHr))
			CreateStereoBuffer(m_pcDeviceCurrent, nullptr, (**pppcBuffer), *ppsDesc, *ppsInitialData, false);

		// method replaced, immediately return
		nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		return nHr;
	}
	return S_OK;
}

/// <summary>D3D11 method call</summary>
HRESULT MatrixModifier::CreatePixelShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CreatePixelShader]) return E_FAIL;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CreatePixelShader]);
	D3D11_MTH_PARAM_HR(void, ppvShaderBytecode, 0, **);
	D3D11_MTH_PARAM_HR(SIZE_T, pvBytecodeLength, 1, *);
	D3D11_MTH_PARAM_HR(ID3D11ClassLinkage, ppcClassLinkage, 2, **);
	D3D11_MTH_PARAM_HR(ID3D11PixelShader, pppcPixelShader, 3, ***);

	// create the shader
	HRESULT nHr = m_pcDeviceCurrent->CreatePixelShader(*ppvShaderBytecode, *pvBytecodeLength, *ppcClassLinkage, *pppcPixelShader);

	// call the handling method
	CreateShader(&m_asPShaders, *ppvShaderBytecode, *pvBytecodeLength, *ppcClassLinkage,
		(ID3D11DeviceChild**)*pppcPixelShader, false, 'P');

	// method replaced, immediately return
	nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
	return nHr;
}

/// <summary>D3D11 method call</summary>
HRESULT MatrixModifier::CreateVertexShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CreateVertexShader]) return E_FAIL;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CreateVertexShader]);
	D3D11_MTH_PARAM_HR(void, ppvShaderBytecode, 0, **);
	D3D11_MTH_PARAM_HR(SIZE_T, pvBytecodeLength, 1, *);
	D3D11_MTH_PARAM_HR(ID3D11ClassLinkage, ppcClassLinkage, 2, **);
	D3D11_MTH_PARAM_HR(ID3D11VertexShader, pppcVertexShader, 3, ***);

	// create the shader
	HRESULT nHr = m_pcDeviceCurrent->CreateVertexShader(*ppvShaderBytecode, *pvBytecodeLength, *ppcClassLinkage, *pppcVertexShader);

	// call the handling method
	CreateShader(&m_asVShaders, *ppvShaderBytecode, *pvBytecodeLength, *ppcClassLinkage,
		(ID3D11DeviceChild**)*pppcVertexShader, false, 'V');

	// method replaced, immediately return
	nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
	return nHr;
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::DSSetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::DSSetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::DSSetConstantBuffers]);
	D3D11_MTH_PARAM(UINT, puStartSlot, 0, *);
	D3D11_MTH_PARAM(UINT, puNumBuffers, 1, *);
	D3D11_MTH_PARAM(ID3D11Buffer, pppcConstantBuffers, 2, ***);

	if (!puStartSlot) return;
	if (!puNumBuffers) return;
	if (!pppcConstantBuffers) return;
	if (!*pppcConstantBuffers) return;
	if (!**pppcConstantBuffers) return;
	{
		// call base method
		XSSetConstantBuffers(m_pcContextCurrent, m_apcDSActiveConstantBuffers11, *puStartSlot, *puNumBuffers, *pppcConstantBuffers, Vireio_Supported_Shaders::DomainShader);

		// call super method
		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			m_pcContextCurrent->DSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcDSActiveConstantBuffers11[*puStartSlot] + BUFFER_REGISTER_L);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
		else
		{
			m_pcContextCurrent->DSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcDSActiveConstantBuffers11[(*puStartSlot) + BUFFER_REGISTER_R]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::GSSetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GSSetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GSSetConstantBuffers]);
	D3D11_MTH_PARAM(UINT, puStartSlot, 0, *);
	D3D11_MTH_PARAM(UINT, puNumBuffers, 1, *);
	D3D11_MTH_PARAM(ID3D11Buffer, pppcConstantBuffers, 2, ***);

	if (!puStartSlot) return;
	if (!puNumBuffers) return;
	if (!pppcConstantBuffers) return;
	if (!*pppcConstantBuffers) return;
	if (!**pppcConstantBuffers) return;
	{
		// call base method
		XSSetConstantBuffers(m_pcContextCurrent, m_apcGSActiveConstantBuffers11, *puStartSlot, *puNumBuffers, *pppcConstantBuffers, Vireio_Supported_Shaders::GeometryShader);

		// call super method
		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			m_pcContextCurrent->GSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcGSActiveConstantBuffers11[*puStartSlot] + BUFFER_REGISTER_L);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
		else
		{
			m_pcContextCurrent->GSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcGSActiveConstantBuffers11[(*puStartSlot) + BUFFER_REGISTER_R]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::HSSetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::HSSetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::HSSetConstantBuffers]);
	D3D11_MTH_PARAM(UINT, puStartSlot, 0, *);
	D3D11_MTH_PARAM(UINT, puNumBuffers, 1, *);
	D3D11_MTH_PARAM(ID3D11Buffer, pppcConstantBuffers, 2, ***);

	if (!puStartSlot) return;
	if (!puNumBuffers) return;
	if (!pppcConstantBuffers) return;
	if (!*pppcConstantBuffers) return;
	if (!**pppcConstantBuffers) return;
	{
		// call base method
		XSSetConstantBuffers(m_pcContextCurrent, m_apcHSActiveConstantBuffers11, *puStartSlot, *puNumBuffers, *pppcConstantBuffers, Vireio_Supported_Shaders::HullShader);

		// call super method
		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			m_pcContextCurrent->HSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcHSActiveConstantBuffers11[*puStartSlot] + BUFFER_REGISTER_L);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
		else
		{
			m_pcContextCurrent->HSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcHSActiveConstantBuffers11[(*puStartSlot) + BUFFER_REGISTER_R]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
	}
}

/// <summary>D3D11 method call</summary>
HRESULT MatrixModifier::Map(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::Map]) return E_FAIL;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::Map]);
	D3D11_MTH_PARAM_HR(ID3D11Resource, ppcResource, 0, **);
	D3D11_MTH_PARAM_HR(UINT, puSubresource, 1, *);
	D3D11_MTH_PARAM_HR(D3D11_MAP, psMapType, 2, *);
	D3D11_MTH_PARAM_HR(UINT, puMapFlags, 3, *);
	D3D11_MTH_PARAM_HR(D3D11_MAPPED_SUBRESOURCE, ppsMappedResource, 4, **);

	HRESULT nHr = S_OK;
	if (!*ppcResource) return E_FAIL;
	if (!*ppsMappedResource) return E_FAIL;
	{
		// get destination resource type
		D3D11_RESOURCE_DIMENSION eDimension;
		(*ppcResource)->GetType(&eDimension);

		// if buffer, get desc
		if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
		{
			D3D11_BUFFER_DESC sDescDst;
			((ID3D11Buffer*)*ppcResource)->GetDesc(&sDescDst);

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

				// get private data rule index from buffer
				Vireio_Buffer_Rules_Index sRulesIndex;
				sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
				sRulesIndex.m_dwUpdateCounter = 0;
				UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
				((ID3D11Buffer*)*ppcResource)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);

				// no private data ? rules not addressed
				if (!dwDataSizeRulesIndex) sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;

				// do the map call..
				nHr = m_pcContextCurrent->Map(*ppcResource, *puSubresource, *psMapType, *puMapFlags, *ppsMappedResource);

				// succeeded ?
				if (nHr == S_OK)
				{
					// resize the vector if necessary
					if (m_dwMappedBuffers >= (UINT)m_asMappedBuffers.size())
						m_asMappedBuffers.resize(m_dwMappedBuffers + 1);

					// set the mapped fields
					m_asMappedBuffers[dwIndex].m_pcMappedResource = *ppcResource;
					m_asMappedBuffers[dwIndex].m_psMappedResource = *ppsMappedResource;
					m_asMappedBuffers[dwIndex].m_pMappedResourceData = (**ppsMappedResource).pData;
					m_asMappedBuffers[dwIndex].m_dwMappedResourceDataSize = sDescDst.ByteWidth;
					m_asMappedBuffers[dwIndex].m_eMapType = *psMapType;
					m_asMappedBuffers[dwIndex].m_dwMapFlags = *puMapFlags;
					m_asMappedBuffers[dwIndex].m_nMapRulesIndex = sRulesIndex.m_nRulesIndex;

					// make buffer address homogenous
					UINT_PTR dwAddress = (UINT_PTR)m_asMappedBuffers[dwIndex].m_pchBuffer11;
					dwAddress |= 0xff; dwAddress++;
					(**ppsMappedResource).pData = (LPVOID)dwAddress;

					// update number of mapped Buffers
					m_dwMappedBuffers++;
				}

				// method replaced, immediately return
				nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
			}
		}
	}
	return nHr;
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::PSGetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::PSGetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::PSGetConstantBuffers]);
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::PSSetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::PSSetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::PSSetConstantBuffers]);
	D3D11_MTH_PARAM(UINT, puStartSlot, 0, *);
	D3D11_MTH_PARAM(UINT, puNumBuffers, 1, *);
	D3D11_MTH_PARAM(ID3D11Buffer, pppcConstantBuffers, 2, ***);

	if (!puStartSlot) return;
	if (!puNumBuffers) return;
	if (!pppcConstantBuffers) return;
	if (!*pppcConstantBuffers) return;
	if (!**pppcConstantBuffers) return;
	{
		// call base method
		XSSetConstantBuffers(m_pcContextCurrent, m_apcPSActiveConstantBuffers11, *puStartSlot, *puNumBuffers, *pppcConstantBuffers, Vireio_Supported_Shaders::PixelShader);

		// call super method
		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			m_pcContextCurrent->PSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcPSActiveConstantBuffers11[*puStartSlot] + BUFFER_REGISTER_L);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
		else
		{
			m_pcContextCurrent->PSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcPSActiveConstantBuffers11[(*puStartSlot) + BUFFER_REGISTER_R]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::PSSetShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::PSSetShader]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::PSSetShader]);
	D3D11_MTH_PARAM(ID3D11PixelShader, ppcPixelShader, 0, **);
	D3D11_MTH_PARAM(ID3D11ClassInstance, pppcClassInstances, 1, ***);
	D3D11_MTH_PARAM(UINT, puNumClassInstances, 2, *);

	if (!ppcPixelShader)
	{
		//m_pcActivePixelShader11 = nullptr;
		return;
	}
	if (!*ppcPixelShader)
	{
		//m_pcActivePixelShader11 = nullptr;
		return;
	}
	else
	{
		// loop through active constant buffers, get private data and update them accordingly to the new shader... TODO !! VERIFY ALL SHADERS
		//for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
		//if (m_apcPSActiveConstantBuffers11[dwIndex])
		//{
		//// verify buffer
		//VerifyConstantBuffer(m_apcPSActiveConstantBuffers11[dwIndex], dwIndex);
		//}

		// currently chosen ?
		if (m_dwCurrentChosenShaderHashCode)
		{
			// get the current shader hash
			Vireio_Shader_Private_Data sPrivateData;
			UINT dwDataSize = sizeof(sPrivateData);
			(*ppcPixelShader)->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

			// set null shader if hash matches
			if (m_dwCurrentChosenShaderHashCode == sPrivateData.dwHash)
			{
				// call super method
				m_pcContextCurrent->PSSetShader(nullptr, nullptr, NULL);

				// method replaced, immediately return
				nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
			}
		}

		// sort shader list ?
		if (m_bSortShaderList)
		{
			// get the current shader hash
			Vireio_Shader_Private_Data sPrivateData;
			UINT dwDataSize = sizeof(sPrivateData);
			(*ppcPixelShader)->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

			// get shader index
			for (UINT dwI = 1; dwI < (UINT)m_adwPShaderHashCodes.size(); dwI++)
			{
				if (sPrivateData.dwHash == m_adwPShaderHashCodes[dwI])
				{
					// move one forward
					std::string szBuf = m_aszPShaderHashCodes[dwI - 1];
					m_aszPShaderHashCodes[dwI - 1] = m_aszPShaderHashCodes[dwI];
					m_aszPShaderHashCodes[dwI] = szBuf;

					UINT dwBuf = m_adwPShaderHashCodes[dwI - 1];
					m_adwPShaderHashCodes[dwI - 1] = m_adwPShaderHashCodes[dwI];
					m_adwPShaderHashCodes[dwI] = dwBuf;

					// end search;
					dwI = (UINT)m_adwPShaderHashCodes.size();
				}
			}
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::Unmap(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::Unmap]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::Unmap]);
	D3D11_MTH_PARAM(ID3D11Resource, ppcResource, 0, **);
	D3D11_MTH_PARAM(UINT, puSubresource, 1, *);

	if (!ppcResource) return;
	if (!puSubresource) return;
	if (!*ppcResource) return;
	{
		// buffers mapped actually ?
		if (m_dwMappedBuffers)
		{
			// loop through all possibly mapped constant buffers
			for (UINT dwI = 0; dwI < (UINT)m_asMappedBuffers.size(); dwI++)
			{
				// continue only if mapped resource data present and the resource pointer matches the stored pointer
				if ((m_asMappedBuffers[dwI].m_pcMappedResource) && (*ppcResource == m_asMappedBuffers[dwI].m_pcMappedResource))
				{
					// get homogenous address
					UINT_PTR dwAddress = (UINT_PTR)m_asMappedBuffers[dwI].m_pchBuffer11;
					dwAddress |= 0xff; dwAddress++;

					// do modification only if shader rule assigned !!
					if ((m_asMappedBuffers[dwI].m_nMapRulesIndex >= 0))
					{
						// do modification, first copy to right buffer
						memcpy(m_pchBuffer11Right, (LPVOID)dwAddress, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize);
						DoBufferModification(m_asMappedBuffers[dwI].m_nMapRulesIndex, dwAddress, (UINT_PTR)m_pchBuffer11Right, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize);
					}

					// copy the stored data...
					memcpy(m_asMappedBuffers[dwI].m_pMappedResourceData, (LPVOID)dwAddress, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize);

					// do the unmap call..
					m_pcContextCurrent->Unmap(*ppcResource, *puSubresource);

					// update right buffer only if shader rule assigned !!
					if ((m_asMappedBuffers[dwI].m_nMapRulesIndex >= 0))
					{
						// get the private data interface
						ID3D11Buffer* pcBuffer = nullptr;
						UINT dwSize = sizeof(pcBuffer);
						(*ppcResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

						if (pcBuffer)
						{
							// update right buffer
							D3D11_MAPPED_SUBRESOURCE sMapped;
							if (SUCCEEDED(m_pcContextCurrent->Map((ID3D11Resource*)pcBuffer, *puSubresource, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &sMapped)))
							{
								memcpy(sMapped.pData, (LPVOID)m_pchBuffer11Right, m_asMappedBuffers[dwI].m_dwMappedResourceDataSize);
								m_pcContextCurrent->Unmap((ID3D11Resource*)pcBuffer, *puSubresource);
							}

							pcBuffer->Release();
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
					nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
					return;
				}
			}
		}

		// no mapped buffer ? handle textures... first, do the ->Unmap() call
		m_pcContextCurrent->Unmap(*ppcResource, *puSubresource);

		// get the resource type
		D3D11_RESOURCE_DIMENSION eResourceDimension;
		(*ppcResource)->GetType(&eResourceDimension);

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
			(*ppcResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);

			// if stereo twin is present, copy the whole texture... TODO ! HANDLE BY SUBRESOURCE INDEX
			if (dwSize)
			{
				// copy the whole texture
				m_pcContextCurrent->CopyResource(pcResourceTwin, *ppcResource);

				// and release
				pcResourceTwin->Release();
			}
			break;
		default:
			break;
		}

		// method replaced, immediately return
		nFlags = AQU_PluginFlags::ImmediateReturnFlag;
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::UpdateSubresource(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::UpdateSubresource]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::UpdateSubresource]);
	D3D11_MTH_PARAM(ID3D11Resource, ppcDstResource, 0, **);
	D3D11_MTH_PARAM(UINT, puDstSubresource, 1, *);
	D3D11_MTH_PARAM(D3D11_BOX, ppsDstBox, 2, **);
	D3D11_MTH_PARAM(void, ppvSrcData, 3, **);
	D3D11_MTH_PARAM(UINT, puSrcRowPitch, 4, *);
	D3D11_MTH_PARAM(UINT, puSrcDepthPitch, 5, *);
	if (!*ppcDstResource) return;
	if (!*ppvSrcData) return;

	// is this a buffer ?
	D3D11_RESOURCE_DIMENSION eDimension;
	(*ppcDstResource)->GetType(&eDimension);
	if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
	{
		// is this a constant buffer ?
		D3D11_BUFFER_DESC sDesc;
		((ID3D11Buffer*)*ppcDstResource)->GetDesc(&sDesc);
		if ((sDesc.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
		{
			// get shader rules index
			Vireio_Buffer_Rules_Index sRulesIndex;
			sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
			sRulesIndex.m_dwUpdateCounter = 0;
			UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
			((ID3D11Buffer*)*ppcDstResource)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);

			// do modification and update right buffer only if shader rule assigned !!
			if ((dwDataSizeRulesIndex) && (sRulesIndex.m_nRulesIndex >= 0))
			{
				// get the private data interface
				ID3D11Buffer* pcBuffer = nullptr;
				UINT dwSize = sizeof(pcBuffer);
				((ID3D11Buffer*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

				if (pcBuffer)
				{
					// do the modification, first copy to buffers
					memcpy(m_pchBuffer11Left, *ppvSrcData, sDesc.ByteWidth);
					memcpy(m_pchBuffer11Right, *ppvSrcData, sDesc.ByteWidth);
					DoBufferModification(sRulesIndex.m_nRulesIndex, (UINT_PTR)m_pchBuffer11Left, (UINT_PTR)m_pchBuffer11Right, sDesc.ByteWidth);

					// update left + right buffer
					m_pcContextCurrent->UpdateSubresource(*ppcDstResource, *puDstSubresource, *ppsDstBox, m_pchBuffer11Left, *puSrcRowPitch, *puSrcDepthPitch);
					m_pcContextCurrent->UpdateSubresource((ID3D11Resource*)pcBuffer, *puDstSubresource, *ppsDstBox, m_pchBuffer11Right, *puSrcRowPitch, *puSrcDepthPitch);

					pcBuffer->Release();

					// method replaced, immediately return
					nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
				}
			}
		}
		else
		{
			// get the private data interface
			ID3D11Buffer* pcBuffer = nullptr;
			UINT dwSize = sizeof(pcBuffer);
			((ID3D11Buffer*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

			if (pcBuffer)
			{
				// update right buffer
				m_pcContextCurrent->UpdateSubresource((ID3D11Resource*)pcBuffer, *puDstSubresource, *ppsDstBox, *ppvSrcData, *puSrcRowPitch, *puSrcDepthPitch);
				pcBuffer->Release();
			}

		}
	}
	else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
	{
		// get the stereo twin of the resource (texture)
		ID3D11Resource* pcResourceTwin = nullptr;
		UINT dwSize = sizeof(pcResourceTwin);
		((ID3D11Resource*)*ppcDstResource)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
		if (pcResourceTwin)
		{
			// update stereo twin
			m_pcContextCurrent->UpdateSubresource(pcResourceTwin, *puDstSubresource, *ppsDstBox, *ppvSrcData, *puSrcRowPitch, *puSrcDepthPitch);
			pcResourceTwin->Release();
		}

	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::VSGetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::VSGetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::VSGetConstantBuffers]);
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::VSSetConstantBuffers(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::VSSetConstantBuffers]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::VSSetConstantBuffers]);
	D3D11_MTH_PARAM(UINT, puStartSlot, 0, *);
	D3D11_MTH_PARAM(UINT, puNumBuffers, 1, *);
	D3D11_MTH_PARAM(ID3D11Buffer, pppcConstantBuffers, 2, ***);

	if (!puStartSlot) return;
	if (!puNumBuffers) return;
	if (!pppcConstantBuffers) return;
	if (!*pppcConstantBuffers) return;
	if (!**pppcConstantBuffers) return;
	{
		// get current shader hash code if there is a shader chosen
		static Vireio_Shader_Private_Data sPrivateData = { 0 };
		static UINT dwChosenOld = 0;
		if ((m_dwCurrentChosenShaderHashCode) || (m_bBufferIndexDebug))
		{
			if (m_dwCurrentChosenShaderHashCode)
			{
				// get the current shader hash
				UINT dwDataSize = sizeof(sPrivateData);
				if (m_pcActiveVertexShader11)
					m_pcActiveVertexShader11->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
				else ZeroMemory(&sPrivateData, sizeof(Vireio_Shader_Private_Data));

				// output shader debug data as long as the chosen shader is set
				if (dwChosenOld == 0) dwChosenOld = m_dwCurrentChosenShaderHashCode;
				if ((dwChosenOld == m_dwCurrentChosenShaderHashCode) && (m_dwCurrentChosenShaderHashCode == sPrivateData.dwHash))
				{
					std::stringstream strStream = std::stringstream();
					strStream << L"VSSetConstantBuffers : " << sPrivateData.dwHash;
					m_aszDebugTrace.push_back(strStream.str().c_str());
				}
			}
			if (m_bBufferIndexDebug)
			{
				// get buffer index
				UINT dwBufferIndex = 0;
				std::stringstream sz = std::stringstream(m_sPageGameShaderRules.m_szBufferIndex);
				sz >> dwBufferIndex;

				// is this index set ?
				if ((dwBufferIndex >= *puStartSlot) && (dwBufferIndex < (*puStartSlot + *puNumBuffers)))
				{
					UINT dwIndex = dwBufferIndex - *puStartSlot;
					if ((*pppcConstantBuffers)[dwIndex])
					{
						// get the buffer description
						D3D11_BUFFER_DESC sDesc;
						((*pppcConstantBuffers)[dwIndex])->GetDesc(&sDesc);

						// already enlisted ? else enlist and output on debug trace
						auto it = std::find(m_aunBufferIndexSizesDebug.begin(), m_aunBufferIndexSizesDebug.end(), sDesc.ByteWidth);
						if (it == m_aunBufferIndexSizesDebug.end())
						{
							// output to debug trace
							std::stringstream strStream = std::stringstream();
							strStream << L"Buffer Index: " << dwBufferIndex << " Size:" << sDesc.ByteWidth;
							m_aszDebugTrace.push_back(strStream.str().c_str());

							// add to vector
							m_aunBufferIndexSizesDebug.push_back(sDesc.ByteWidth);
						}
					}
				}
			}
		}
		else dwChosenOld = 0;

		// call base method
		XSSetConstantBuffers(m_pcContextCurrent, m_apcVSActiveConstantBuffers11, *puStartSlot, *puNumBuffers, *pppcConstantBuffers, Vireio_Supported_Shaders::VertexShader);

		// call super method
		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			m_pcContextCurrent->VSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcVSActiveConstantBuffers11[*puStartSlot + BUFFER_REGISTER_L]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
		else
		{
			m_pcContextCurrent->VSSetConstantBuffers(*puStartSlot,
				*puNumBuffers,
				(ID3D11Buffer**)&m_apcVSActiveConstantBuffers11[(*puStartSlot) + BUFFER_REGISTER_R]);

			// method replaced, immediately return
			nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		}
	}
}

/// <summary>D3D11 method call</summary>
void MatrixModifier::VSSetShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::VSSetShader]) return;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::VSSetShader]);
	D3D11_MTH_PARAM(ID3D11VertexShader, ppcVertexShader, 0, **);
	D3D11_MTH_PARAM(ID3D11ClassInstance, pppcClassInstances, 1, ***);
	D3D11_MTH_PARAM(UINT, puNumClassInstances, 2, *);

	if (!ppcVertexShader)
	{
		m_pcActiveVertexShader11 = nullptr;
		return;
	}
	if (!*ppcVertexShader)
	{
		m_pcActiveVertexShader11 = nullptr;
		return;
	}
	else
	{
		// set the active vertex shader
		m_pcActiveVertexShader11 = *ppcVertexShader;

		// loop through active constant buffers, get private data and update them accordingly to the new shader
		for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
			if (m_apcVSActiveConstantBuffers11[dwIndex])
			{
				// verify buffer
				VerifyConstantBuffer(m_apcVSActiveConstantBuffers11[dwIndex], dwIndex, Vireio_Supported_Shaders::VertexShader);
			}

		// get the current shader hash
		Vireio_Shader_Private_Data sPrivateData;
		UINT dwDataSize = sizeof(sPrivateData);
		if (m_pcActiveVertexShader11)
			m_pcActiveVertexShader11->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

		// set secondary render target if hash matches .. to be verified !!
		/*if (m_dwCurrentChosenShaderHashCode == sPrivateData.dwHash)
		{
			// output shader debug data
			static UINT dwChosenOld = 0;
			if (dwChosenOld != m_dwCurrentChosenShaderHashCode)
			{
				std::stringstream strStream;
				strStream << "Hash:" << sPrivateData.dwHash;
				m_aszDebugTrace.push_back(strStream.str().c_str());
				for (UINT dwI = 0; dwI < (UINT)m_asVShaders[sPrivateData.dwIndex].asBuffers.size(); dwI++)
				{
					strStream = std::stringstream();
					strStream << "Name:" << m_asVShaders[sPrivateData.dwIndex].asBuffers[dwI].szName << "::Size:" << m_asVShaders[sPrivateData.dwIndex].asBuffers[dwI].dwSize;
					m_aszDebugTrace.push_back(strStream.str().c_str());
				}
				for (UINT dwI = 0; dwI < (UINT)m_asVShaders[sPrivateData.dwIndex].asBuffersUnaccounted.size(); dwI++)
				{
					strStream = std::stringstream();
					strStream <<L"Reg:" << m_asVShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwI].dwRegister << "::Size:" << m_asVShaders[sPrivateData.dwIndex].asBuffersUnaccounted[dwI].dwSize;
					m_aszDebugTrace.push_back(strStream.str().c_str());
				}

				dwChosenOld = m_dwCurrentChosenShaderHashCode;

				for (UINT dwI = 0; dwI < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwI++)
					if (m_apcVSActiveConstantBuffers11[dwI])
					{
						// get shader rules index
						Vireio_Buffer_Rules_Index sRulesIndex;
						sRulesIndex.m_nRulesIndex = VIREIO_CONSTANT_RULES_NOT_ADDRESSED;
						sRulesIndex.m_dwUpdateCounter = 0;
						UINT dwDataSizeRulesIndex = sizeof(Vireio_Buffer_Rules_Index);
						m_apcVSActiveConstantBuffers11[dwI]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Rules_Data, &dwDataSizeRulesIndex, &sRulesIndex);

						D3D11_BUFFER_DESC sDesc;
						m_apcVSActiveConstantBuffers11[dwI]->GetDesc(&sDesc);
						strStream = std::stringstream();
						strStream << "Buffer Size [" << dwI << "]:" << sDesc.ByteWidth << " RuleInd:" << sRulesIndex.m_nRulesIndex;
						m_aszDebugTrace.push_back(strStream.str().c_str());
					}
			}

			// secondary render target present ?
			if (!m_pcSecondaryRenderTarget11)
			{
				// get device
				ID3D11Device* pcDevice = nullptr;
				m_pcContextCurrent->GetDevice(&pcDevice);
				if (pcDevice)
				{
					// get the viewport
					UINT unNumViewports = 1;
					D3D11_VIEWPORT sViewport;
					m_pcContextCurrent->RSGetViewports(&unNumViewports, &sViewport);

					if (unNumViewports)
					{
						// fill the description
						D3D11_TEXTURE2D_DESC sDescTex;
						sDescTex.Width = (UINT)sViewport.Width;
						sDescTex.Height = (UINT)sViewport.Height;
						sDescTex.MipLevels = 1;
						sDescTex.ArraySize = 1;
						sDescTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						sDescTex.SampleDesc.Count = 1;
						sDescTex.SampleDesc.Quality = 0;
						sDescTex.Usage = D3D11_USAGE_DEFAULT;
						sDescTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
						sDescTex.CPUAccessFlags = 0;
						sDescTex.MiscFlags = 0;

						// create secondary render target
						pcDevice->CreateTexture2D(&sDescTex, NULL, &m_pcSecondaryRenderTarget11);
						if (m_pcSecondaryRenderTarget11)
						{
							// create render target view
							if (FAILED(pcDevice->CreateRenderTargetView(m_pcSecondaryRenderTarget11, NULL, &m_pcSecondaryRenderTargetView11)))
								OutputDebugString(L"[MAM] Failed to create secondary render target view.");

							// create shader resource view
							if (FAILED(pcDevice->CreateShaderResourceView(m_pcSecondaryRenderTarget11, NULL, &m_pcSecondaryRenderTargetSRView11)))
								OutputDebugString(L"[MAM] Failed to create secondary render target shader resource view.");
						}
						else OutputDebugString(L"[MAM] Failed to create secondary render target !");
					}
					else OutputDebugString(L"[MAM] No Viewport present !");

					pcDevice->Release();
				}
			}

			// set secondary render target
			m_pcContextCurrent->OMSetRenderTargets(1, &m_pcSecondaryRenderTargetView11, nullptr);
		}*/

		// loop through fetched hash codes .. to be verified !!
		/*for (UINT unI = 0; unI < (UINT)m_aunFetchedHashCodes.size(); unI++)
		{
			// switch render targets
			if ((sPrivateData.dwHash == m_aunFetchedHashCodes[unI]))
			{
				// secondary render target present ?
				if (!m_pcSecondaryRenderTarget11)
				{
					// get device
					ID3D11Device* pcDevice = nullptr;
					m_pcContextCurrent->GetDevice(&pcDevice);
					if (pcDevice)
					{
						// get the viewport
						UINT unNumViewports = 1;
						D3D11_VIEWPORT sViewport;
						m_pcContextCurrent->RSGetViewports(&unNumViewports, &sViewport);

						if (unNumViewports)
						{
							// fill the description
							D3D11_TEXTURE2D_DESC sDescTex;
							sDescTex.Width = (UINT)sViewport.Width;
							sDescTex.Height = (UINT)sViewport.Height;
							sDescTex.MipLevels = 1;
							sDescTex.ArraySize = 1;
							sDescTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
							sDescTex.SampleDesc.Count = 1;
							sDescTex.SampleDesc.Quality = 0;
							sDescTex.Usage = D3D11_USAGE_DEFAULT;
							sDescTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
							sDescTex.CPUAccessFlags = 0;
							sDescTex.MiscFlags = 0;

							// create secondary render target
							pcDevice->CreateTexture2D(&sDescTex, NULL, &m_pcSecondaryRenderTarget11);
							if (m_pcSecondaryRenderTarget11)
							{
								// create render target view
								if (FAILED(pcDevice->CreateRenderTargetView(m_pcSecondaryRenderTarget11, NULL, &m_pcSecondaryRenderTargetView11)))
									OutputDebugString(L"[MAM] Failed to create secondary render target view.");

								// create shader resource view
								if (FAILED(pcDevice->CreateShaderResourceView(m_pcSecondaryRenderTarget11, NULL, &m_pcSecondaryRenderTargetSRView11)))
									OutputDebugString(L"[MAM] Failed to create secondary render target shader resource view.");
							}
							else OutputDebugString(L"[MAM] Failed to create secondary render target !");
						}
						else OutputDebugString(L"[MAM] No Viewport present !");

						pcDevice->Release();
					}
				}

				// set secondary render target
				if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
					m_pcContextCurrent->OMSetRenderTargets(1, &m_pcSecondaryRenderTargetView11, (ID3D11DepthStencilView*)m_apcActiveDepthStencilView11[0]);
				else
					m_pcContextCurrent->OMSetRenderTargets(1, &m_pcSecondaryRenderTargetView11, (ID3D11DepthStencilView*)m_apcActiveDepthStencilView11[1]);
			}
		}*/

		// sort shader list ?
		if (m_bSortShaderList)
		{
			// get the current shader hash
			Vireio_Shader_Private_Data sPrivateData;
			UINT dwDataSize = sizeof(sPrivateData);
			if (m_pcActiveVertexShader11)
				m_pcActiveVertexShader11->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);

			// get shader index
			for (UINT dwI = 1; dwI < (UINT)m_adwVShaderHashCodes.size(); dwI++)
			{
				if (sPrivateData.dwHash == m_adwVShaderHashCodes[dwI])
				{
					// move one forward
					std::string szBuf = m_aszVShaderHashCodes[dwI - 1];
					m_aszVShaderHashCodes[dwI - 1] = m_aszVShaderHashCodes[dwI];
					m_aszVShaderHashCodes[dwI] = szBuf;

					UINT dwBuf = m_adwVShaderHashCodes[dwI - 1];
					m_adwVShaderHashCodes[dwI - 1] = m_adwVShaderHashCodes[dwI];
					m_adwVShaderHashCodes[dwI] = dwBuf;

					// end search;
					dwI = (UINT)m_adwVShaderHashCodes.size();
				}
			}
		}
	}
}

#pragma endregion
#elif defined(VIREIO_D3D10)
#pragma region /// => D3D10 methods
#pragma endregion
#else
#pragma region /// => D3D9 methods
/// <summary> 
/// => Set Vertex Shader
/// </summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT MatrixModifier::SetVertexShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetVertexShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetVertexShader]);

	IDirect3DVertexShader9** ppcShader;
	if (ppIn[0]) ppcShader = *(IDirect3DVertexShader9***)ppIn[0]; else return E_FAIL;
	if (!*ppcShader)
	{
		m_pcActiveVertexShader = nullptr;
		m_sModifierData.uActiveVSIx = 0;
		m_sModifierData.bVSActive = false;
	}
	else
	{
		// get shader function size
		uint32_t uSizeOfData = 0;
		uint32_t uIndex = 0;
		(*ppcShader)->GetFunction(NULL, &uSizeOfData);
		if (uSizeOfData)
		{
			// .. and the function itself
			std::vector<BYTE> acFunction(uSizeOfData);
			(*ppcShader)->GetFunction(acFunction.data(), &uSizeOfData);

			// get the creator index
			uint32_t uCreatorIx = GetCreatorIndex((uint32_t*)acFunction.data());
			if (uCreatorIx)
			{
				// provide the shader data by modified creator string and its stored shader index
				CreatorMod* psModData = (CreatorMod*)&acFunction[uCreatorIx];
				std::string acIndex = std::string("0x") + std::string(psModData->acIndex, 4);
				uIndex = (uint32_t)std::stoul(acIndex, nullptr, 16);
			}
		}

		// set new active shader
		m_pcActiveVertexShader = *ppcShader;
		m_sModifierData.bVSActive = true;

		// and update shader constants
		if (uIndex < (uint32_t)m_sModifierData.asVShaders.size())
		{
			// set index
			m_sModifierData.uActiveVSIx = uIndex;

			// update shader constants for active side
			auto it = m_sModifierData.asVShaders[uIndex].asConstantRuleIndices.begin();
			while (it != m_sModifierData.asVShaders[uIndex].asConstantRuleIndices.end())
			{
				// apply to left and right data
				UINT unStartRegisterConstant = (*it).dwConstantRuleRegister;

				// get the matrix
				D3DXMATRIX sMatrix(&m_afRegistersVertex[RegisterIndex(unStartRegisterConstant)]);
				{
					// matrix to be transposed ?
					bool bTranspose = m_asConstantRules[it->dwIndex].m_bTranspose;
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrix, &sMatrix);
					}

					// do modification
					std::array<float, 16> afMatrixLeft, afMatrixRight;
					((ShaderMatrixModification*)m_asConstantRules[it->dwIndex].m_pcModification.get())->ApplyModification(sMatrix, &afMatrixLeft, &afMatrixRight);
					D3DXMATRIX sMatrixLeft(afMatrixLeft.data());
					D3DXMATRIX sMatrixRight(afMatrixRight.data());

					// transpose back
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
						D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
					}

					it->asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
					it->asConstantDataRight = (D3DMATRIX)sMatrixRight;
				}
				it++;
			}

			// set modified constants
			if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < m_sModifierData.asVShaders[uIndex].asConstantRuleIndices.size(); nI++)
					m_pcDeviceCurrent->SetVertexShaderConstantF(m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegister, m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].afConstantDataLeft, m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegisterCount);
			}
			else
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < m_sModifierData.asVShaders[uIndex].asConstantRuleIndices.size(); nI++)
					m_pcDeviceCurrent->SetVertexShaderConstantF(m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegister, m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].afConstantDataRight, m_sModifierData.asVShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegisterCount);
			}
		}
		else m_sModifierData.uActiveVSIx = 0;
	}

	return S_OK;
}

/// <summary> 
/// => Set Pixel Shader
/// </summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result</returns>
HRESULT MatrixModifier::SetPixelShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetPixelShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetPixelShader]);

	IDirect3DPixelShader9** ppcShader;
	if (ppIn[0]) ppcShader = *(IDirect3DPixelShader9***)ppIn[0]; else return E_FAIL;
	if (!*ppcShader)
	{
		m_pcActivePixelShader = nullptr;
		m_sModifierData.uActivePSIx = 0;
		m_sModifierData.bPSActive = false;
	}
	else
	{
		// get shader function size
		uint32_t uSizeOfData = 0;
		uint32_t uIndex = 0;
		(*ppcShader)->GetFunction(NULL, &uSizeOfData);
		if (uSizeOfData)
		{
			// .. and the function itself
			std::vector<BYTE> acFunction(uSizeOfData);
			(*ppcShader)->GetFunction(acFunction.data(), &uSizeOfData);

			// get the creator index
			uint32_t uCreatorIx = GetCreatorIndex((uint32_t*)acFunction.data());
			if (uCreatorIx)
			{
				// provide the shader data by modified creator string and its stored shader index
				CreatorMod* psModData = (CreatorMod*)&acFunction[uCreatorIx];
				std::string acIndex = std::string("0x") + std::string(psModData->acIndex, 4);
				uIndex = (uint32_t)std::stoul(acIndex, nullptr, 16);
			}
		}

		// set new active shader
		m_pcActivePixelShader = *ppcShader;
		m_sModifierData.bPSActive = true;

		// and update shader constants
		if (uIndex < (uint32_t)m_sModifierData.asPShaders.size())
		{
			// set index
			m_sModifierData.uActivePSIx = uIndex;

			// update shader constants for active side
			auto it = m_sModifierData.asPShaders[uIndex].asConstantRuleIndices.begin();
			while (it != m_sModifierData.asPShaders[uIndex].asConstantRuleIndices.end())
			{
				// apply to left and right data
				UINT unStartRegisterConstant = (*it).dwConstantRuleRegister;

				// get the matrix
				D3DXMATRIX sMatrix(&m_afRegistersPixel[RegisterIndex(unStartRegisterConstant)]);
				{
					// matrix to be transposed ?
					bool bTranspose = m_asConstantRules[it->dwIndex].m_bTranspose;
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrix, &sMatrix);
					}

					// do modification
					std::array<float, 16> afMatrixLeft, afMatrixRight;
					((ShaderMatrixModification*)m_asConstantRules[it->dwIndex].m_pcModification.get())->ApplyModification(sMatrix, &afMatrixLeft, &afMatrixRight);
					D3DXMATRIX sMatrixLeft(afMatrixLeft.data());
					D3DXMATRIX sMatrixRight(afMatrixRight.data());

					// transpose back
					if (bTranspose)
					{
						D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
						D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
					}

					it->asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
					it->asConstantDataRight = (D3DMATRIX)sMatrixRight;
				}
				it++;
			}

			// set modified constants
			if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < m_sModifierData.asPShaders[uIndex].asConstantRuleIndices.size(); nI++)
					m_pcDeviceCurrent->SetPixelShaderConstantF(m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegister, m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].afConstantDataLeft, m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegisterCount);
			}
			else
			{
				for (std::vector<Vireio_Constant_Rule_Index_DX9>::size_type nI = 0; nI < m_sModifierData.asPShaders[uIndex].asConstantRuleIndices.size(); nI++)
					m_pcDeviceCurrent->SetPixelShaderConstantF(m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegister, m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].afConstantDataRight, m_sModifierData.asPShaders[uIndex].asConstantRuleIndices[nI].dwConstantRuleRegisterCount);
			}
		}
		else m_sModifierData.uActivePSIx = 0;
	}

	return S_OK;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetVertexShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetVertexShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetVertexShader]);

	IDirect3DVertexShader9*** pppcShader;
	if (ppIn[0]) pppcShader = *(IDirect3DVertexShader9****)ppIn[0]; else return E_FAIL;

	//if ((!m_pcActiveVertexShader) || (!*pppcShader))
		//return D3DERR_INVALIDCALL;


	return D3D_OK;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetPixelShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetPixelShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetPixelShader]);

	IDirect3DPixelShader9*** pppcShader;
	if (ppIn[0]) pppcShader = *(IDirect3DPixelShader9****)ppIn[0]; else return E_FAIL;

	//if ((!m_pcActivePixelShader) || (!*pppcShader))
	//	return D3DERR_INVALIDCALL;


	return D3D_OK;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetTransform(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetTransform]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetTransform]);

	D3DTRANSFORMSTATETYPE* peState;
	if (ppIn[0]) peState = *(D3DTRANSFORMSTATETYPE**)ppIn[0]; else return E_FAIL;
	D3DMATRIX** ppsMatrix;
	if (ppIn[1]) ppsMatrix = *(D3DMATRIX***)ppIn[1]; else return E_FAIL;

	HRESULT nHr = S_OK;
	if ((*peState) == D3DTS_VIEW)
	{
		D3DXMATRIX sMatTempLeft;
		D3DXMATRIX sMatTempRight;

		if (!(*ppsMatrix))
		{
			D3DXMatrixIdentity(&sMatTempLeft);
			D3DXMatrixIdentity(&sMatTempRight);
			m_sModifierData.bViewSet = false;
		}
		else
		{
			D3DXMATRIX sMatSource(*(*ppsMatrix));

			// If the view is set to the identity then we don't need to perform any adjustments
			if (D3DXMatrixIsIdentity(&sMatSource))
			{
				D3DXMatrixIdentity(&sMatTempLeft);
				D3DXMatrixIdentity(&sMatTempRight);
				m_sModifierData.bViewSet = false;
			}
			else
			{
				// If the view matrix is modified we need to apply left/right adjustments (for stereo rendering)
				sMatTempLeft = sMatSource * m_pcShaderModificationCalculation->Get(MathRegisters::MAT_TransformL, 4);
				sMatTempRight = sMatSource * m_pcShaderModificationCalculation->Get(MathRegisters::MAT_TransformR, 4);
				m_sModifierData.bViewSet = true;
			}
		}

		// update proxy device
		m_sModifierData.sMatView[0] = sMatTempLeft;
		m_sModifierData.sMatView[1] = sMatTempRight;

		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			nHr = m_pcDeviceCurrent->SetTransform(*peState, &m_sModifierData.sMatView[0]);
		}
		else
		{
			nHr = m_pcDeviceCurrent->SetTransform(*peState, &m_sModifierData.sMatView[1]);
		}
	}
	else if ((*peState) == D3DTS_PROJECTION)
	{
		D3DXMATRIX sMatTempLeft;
		D3DXMATRIX sMatTempRight;

		if (!(*ppsMatrix))
		{
			D3DXMatrixIdentity(&sMatTempLeft);
			D3DXMatrixIdentity(&sMatTempRight);
			m_sModifierData.bProjSet = false;
		}
		else
		{
			D3DXMATRIX sMatSource(*(*ppsMatrix));

			// If the view is set to the identity then we don't need to perform any adjustments
			if (D3DXMatrixIsIdentity(&sMatSource))
			{
				D3DXMatrixIdentity(&sMatTempLeft);
				D3DXMatrixIdentity(&sMatTempRight);
				m_sModifierData.bProjSet = false;
			}
			else
			{
				// did we choose the right l/r transform matrices here ??
				sMatTempLeft = sMatSource * m_pcShaderModificationCalculation->Get(MathRegisters::MAT_ViewProjectionL, 4);
				sMatTempRight = sMatSource * m_pcShaderModificationCalculation->Get(MathRegisters::MAT_ViewProjectionR, 4);
				m_sModifierData.bProjSet = true;
			}
		}

		// update proxy device
		m_sModifierData.sMatProj[0] = sMatTempLeft;
		m_sModifierData.sMatProj[1] = sMatTempRight;

		if (m_sModifierData.eCurrentRenderingSide == RenderPosition::Left)
		{
			nHr = m_pcDeviceCurrent->SetTransform(*peState, &m_sModifierData.sMatProj[0]);
		}
		else
		{
			nHr = m_pcDeviceCurrent->SetTransform(*peState, &m_sModifierData.sMatProj[1]);
		}
	}
	// if D3DTS_WORLD matrix is set do not replace the call
	else return S_OK;

	// method replaced, immediately return
	nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
	return nHr;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::MultiplyTransform(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::MultiplyTransform]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::MultiplyTransform]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetVertexShaderConstantF(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantF]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantF]);
	UINT* puStartRegister;
	if (ppIn[0]) puStartRegister = *(UINT**)ppIn[0]; else return E_FAIL;
	float** ppfConstantData;
	if (ppIn[1]) ppfConstantData = *(float***)ppIn[1]; else return E_FAIL;
	UINT* puVector4fCount;
	if (ppIn[2]) puVector4fCount = *(UINT**)ppIn[2]; else return E_FAIL;

	// ->D3DERR_INVALIDCALL, will actually be returned by base d3d method
	if (((*puStartRegister) >= MAX_DX9_CONSTANT_REGISTERS) || (((*puStartRegister) + (*puVector4fCount)) >= MAX_DX9_CONSTANT_REGISTERS))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	memcpy(&m_afRegistersVertex[RegisterIndex(*puStartRegister)], *ppfConstantData, (*puVector4fCount) * 4 * sizeof(float));

	if (m_pcActiveVertexShader)
	{
		// check wether the data gets modified or not
		bool bModified = false;
		SetXShaderConstantF(*puStartRegister, *ppfConstantData, *puVector4fCount, bModified, m_sModifierData.eCurrentRenderingSide, &m_afRegistersVertex[0], &m_sModifierData.asVShaders[m_sModifierData.uActiveVSIx]);

		// was the data modified  ?
		if (bModified)
		{
			// set modified data, immediate return
			nFlags = AQU_PluginFlags::ImmediateReturnFlag;
			return m_pcDeviceCurrent->SetVertexShaderConstantF(*puStartRegister, &m_sModifierData.asVShaders[m_sModifierData.uActiveVSIx].afRegisterBuffer[0], *puVector4fCount);
		}
	}
	return S_OK;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetVertexShaderConstantF(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetVertexShaderConstantF]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetVertexShaderConstantF]);
	UINT* puStartRegister;
	if (ppIn[0]) puStartRegister = *(UINT**)ppIn[0]; else return E_FAIL;
	float** ppfConstantData;
	if (ppIn[1]) ppfConstantData = *(float***)ppIn[1]; else return E_FAIL;
	UINT* puVector4fCount;
	if (ppIn[2]) puVector4fCount = *(UINT**)ppIn[2]; else return E_FAIL;

	HRESULT nHr = S_OK;


	return nHr;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetVertexShaderConstantI(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantI]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantI]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetVertexShaderConstantI(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetVertexShaderConstantI]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetVertexShaderConstantI]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetVertexShaderConstantB(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantB]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetVertexShaderConstantB]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetVertexShaderConstantB(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetPixelShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetPixelShader]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetPixelShaderConstantF(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantF]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantF]);
	UINT* puStartRegister;
	if (ppIn[0]) puStartRegister = *(UINT**)ppIn[0]; else return E_FAIL;
	float** ppfConstantData;
	if (ppIn[1]) ppfConstantData = *(float***)ppIn[1]; else return E_FAIL;
	UINT* puVector4fCount;
	if (ppIn[2]) puVector4fCount = *(UINT**)ppIn[2]; else return E_FAIL;

	// ->D3DERR_INVALIDCALL, will actually be returned by base d3d method
	if (((*puStartRegister) >= MAX_DX9_CONSTANT_REGISTERS) || (((*puStartRegister) + (*puVector4fCount)) >= MAX_DX9_CONSTANT_REGISTERS))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	memcpy(&m_afRegistersPixel[RegisterIndex(*puStartRegister)], *ppfConstantData, (*puVector4fCount) * 4 * sizeof(float));

	if (m_pcActivePixelShader)
	{
		// check wether the data gets modified or not
		bool bModified = false;
		SetXShaderConstantF(*puStartRegister, *ppfConstantData, *puVector4fCount, bModified, m_sModifierData.eCurrentRenderingSide, &m_afRegistersPixel[0], &m_sModifierData.asPShaders[m_sModifierData.uActivePSIx]);

		// was the data modified  ?
		if (bModified)
		{
			// set modified data, immediate return
			nFlags = AQU_PluginFlags::ImmediateReturnFlag;
			return m_pcDeviceCurrent->SetPixelShaderConstantF(*puStartRegister, &m_sModifierData.asPShaders[m_sModifierData.uActivePSIx].afRegisterBuffer[0], *puVector4fCount);
		}
	}
	return S_OK;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetPixelShaderConstantF(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantF]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantF]);
	UINT* puStartRegister;
	if (ppIn[0]) puStartRegister = *(UINT**)ppIn[0]; else return E_FAIL;
	float** ppfConstantData;
	if (ppIn[1]) ppfConstantData = *(float***)ppIn[1]; else return E_FAIL;
	UINT* puVector4fCount;
	if (ppIn[2]) puVector4fCount = *(UINT**)ppIn[2]; else return E_FAIL;

	HRESULT nHr = S_OK;


	return nHr;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetPixelShaderConstantI(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantI]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantI]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetPixelShaderConstantI(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantI]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantI]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetPixelShaderConstantB(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantB]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetPixelShaderConstantB]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetPixelShaderConstantB(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantB]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetPixelShaderConstantB]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::SetStreamSource(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::SetStreamSource]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::SetStreamSource]);
	return E_NOTIMPL;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::GetStreamSource(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::GetStreamSource]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::GetStreamSource]);
	return E_NOTIMPL;
}

/// <summary>
/// => Create Vertex Shader
/// </summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result, S_OK if not replaced</returns>
HRESULT MatrixModifier::CreateVertexShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CreateVertexShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CreateVertexShader]);
	DWORD** ppuFunction;
	if (ppIn[0]) ppuFunction = *(DWORD***)ppIn[0]; else return E_FAIL;
	IDirect3DVertexShader9*** pppcShader;
	if (ppIn[1]) pppcShader = *(IDirect3DVertexShader9****)ppIn[1]; else return E_FAIL;
	if (!(*pppcShader)) return S_OK;
	if (!(*ppuFunction)) return S_OK;

	// parse the shader
	uint32_t* auFunc = (uint32_t*)(*ppuFunction);
	uint32_t uSizeOfData = 0, uCreatorIx = 0, uHash = 0;
	std::vector<VIREIO_D3D9_CONSTANT_DESC> asConstantDesc;
	if (FAILED(ParseShaderFunction(auFunc, uSizeOfData, uCreatorIx, asConstantDesc, uHash, VIREIO_SEED)))
	{
		OutputDebugString(L"[MAM] Failed to parse shader function !!");

		// create shader
		HRESULT nHr = m_pcDeviceCurrent->CreateVertexShader(*ppuFunction, *pppcShader);

		if (**pppcShader)
		{
			// get function and output to file
			UINT uSizeOfData;
			(**pppcShader)->GetFunction(NULL, &uSizeOfData);
			std::vector<BYTE> acData(uSizeOfData);
			(**pppcShader)->GetFunction(acData.data(), &uSizeOfData);

			// output code to file
			static unsigned uIx = 0;
			char buf[32]; ZeroMemory(&buf[0], 32);
			sprintf_s(buf, "VS%u.cso", uIx++);
			std::ofstream oLogFile(buf, std::ios::ate);
			if (oLogFile.is_open())
			{
				oLogFile.write((char*)acData.data(), uSizeOfData);
				oLogFile.close();
			}
		}
		nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		return nHr;
	}

	// create array by function
	if (!uSizeOfData) return S_OK;
	std::vector<BYTE> acFunction(uSizeOfData);
	CopyMemory(acFunction.data(), (BYTE*)auFunc, uSizeOfData);

	// no creator string text provided within function ?
	if (!uCreatorIx)
	{
		// TODO !! NO CONSTANT TABLE PRESENT - CREATE CONSTANT TABLE BY HAND
		OutputDebugString(L"[MAM] No Creator CTAB text present within shader byte code !!");
		return S_OK;
	}

	// loop through shader list wether hash is present
	bool bPresent = false;
	uint32_t uShaderIx = 0;
	for (UINT unI = 0; unI < (UINT)m_sModifierData.asVShaders.size(); unI++)
	{
		if (m_sModifierData.asVShaders[unI].uHash == (UINT)uHash)
		{
			bPresent = true;
			uShaderIx = (uint32_t)unI;
		}
	}

	// add to shader list
	if (!bPresent)
	{
		Vireio_D3D9_Shader sShaderDesc = {};
		sShaderDesc.uHash = (UINT)uHash;
		sShaderDesc.atCreator = std::string((char*)&acFunction[uCreatorIx]);

		// get the constant descriptions from that shader
		sShaderDesc.asConstantDescriptions = std::vector<SAFE_D3DXCONSTANT_DESC>(asConstantDesc.begin(), asConstantDesc.end());

		// copy the name (LPCSTR) to a new string
		for (UINT unI = 0; unI < (UINT)asConstantDesc.size(); unI++)
		{
			// copy name string
			sShaderDesc.asConstantDescriptions[unI].acName = asConstantDesc[unI].acName;
		}

		// init the shader rules
		InitShaderRules(&sShaderDesc);

		// set index, add to vector
		uShaderIx = (uint32_t)m_sModifierData.asVShaders.size();
		m_sModifierData.asVShaders.push_back(sShaderDesc);
	}

	// write down data to shader creator string hex strings
	static const char* acDigits = "0123456789ABCDEF";
	const char acModHeader[4] = { 'V', 'M', 'O', 'D' };
	CreatorMod* psModData = (CreatorMod*)&acFunction[uCreatorIx];
	CopyMemory(psModData->acModHeader, acModHeader, 4);
	HexString(psModData->acHash, uHash, 8);
	HexString(psModData->acIndex, uShaderIx, 4);

#ifdef TRACE_MODIFIER
	// output constants...
	for (VIREIO_D3D9_CONSTANT_DESC sDesc : asConstantDesc)
	{
		OutputDebugStringA(sDesc.acName.c_str());
	}
	// and shader hash, size, creator string index
	{ wchar_t buf[128]; wsprintf(buf, L"Hash %x:Size %u:CrIx %u", uHash, uSizeOfData, uCreatorIx); OutputDebugString(buf); }
	if (uCreatorIx)
	{
		std::string atCreator = std::string((char*)&acFunction[uCreatorIx]);
		OutputDebugStringA(atCreator.c_str());
	}
#endif		

	// create shader using modified function byte code and return
	HRESULT nHr = S_OK;
	nHr = m_pcDeviceCurrent->CreateVertexShader((const DWORD*)acFunction.data(), *pppcShader);
	nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
	return nHr;
}

/// <summary>
/// => Create Pixel Shader
/// </summary>
/// <param name="nFlags">[in,out]Method call flags</param>
/// <returns>D3D result, S_OK if not replaced</returns>
HRESULT MatrixModifier::CreatePixelShader(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::CreatePixelShader]) return S_OK;
	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::CreatePixelShader]);
	DWORD** ppuFunction;
	if (ppIn[0]) ppuFunction = *(DWORD***)ppIn[0]; else return E_FAIL;
	IDirect3DPixelShader9*** pppcShader;
	if (ppIn[1]) pppcShader = *(IDirect3DPixelShader9****)ppIn[1]; else return E_FAIL;
	if (!(*pppcShader)) return S_OK;
	if (!(*ppuFunction)) return S_OK;

	// parse the shader
	uint32_t* auFunc = (uint32_t*)(*ppuFunction);
	uint32_t uSizeOfData = 0, uCreatorIx = 0, uHash = 0;
	std::vector<VIREIO_D3D9_CONSTANT_DESC> asConstantDesc;
	if (FAILED(ParseShaderFunction(auFunc, uSizeOfData, uCreatorIx, asConstantDesc, uHash, VIREIO_SEED)))
	{
		OutputDebugString(L"[MAM] Failed to parse shader function !!");

		// create shader
		HRESULT nHr = m_pcDeviceCurrent->CreatePixelShader(*ppuFunction, *pppcShader);

		if (**pppcShader)
		{
			// get function and output to file
			UINT uSizeOfData;
			(**pppcShader)->GetFunction(NULL, &uSizeOfData);
			std::vector<BYTE> acData(uSizeOfData);
			(**pppcShader)->GetFunction(acData.data(), &uSizeOfData);

			// output code to file
			static unsigned uIx = 0;
			char buf[32]; ZeroMemory(&buf[0], 32);
			sprintf_s(buf, "PS%u.cso", uIx++);
			std::ofstream oLogFile(buf, std::ios::ate);
			if (oLogFile.is_open())
			{
				oLogFile.write((char*)acData.data(), uSizeOfData);
				oLogFile.close();
			}
		}
		nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
		return nHr;
	}

	// create array by function
	if (!uSizeOfData) return S_OK;
	std::vector<BYTE> acFunction(uSizeOfData);
	CopyMemory(acFunction.data(), (BYTE*)auFunc, uSizeOfData);

	// no creator string text provided within function ?
	if (!uCreatorIx)
	{
		// TODO !! NO CONSTANT TABLE PRESENT - CREATE CONSTANT TABLE BY HAND
		OutputDebugString(L"[MAM] No Creator CTAB text present within shader byte code !!");
		return S_OK;
	}

	// loop through shader list wether hash is present
	bool bPresent = false;
	uint32_t uShaderIx = 0;
	for (UINT unI = 0; unI < (UINT)m_sModifierData.asPShaders.size(); unI++)
	{
		if (m_sModifierData.asPShaders[unI].uHash == (UINT)uHash)
		{
			bPresent = true;
			uShaderIx = (uint32_t)unI;
		}
	}

	// add to shader list
	if (!bPresent)
	{
		Vireio_D3D9_Shader sShaderDesc = {};
		sShaderDesc.uHash = (UINT)uHash;
		sShaderDesc.atCreator = std::string((char*)&acFunction[uCreatorIx]);

		// get the constant descriptions from that shader
		sShaderDesc.asConstantDescriptions = std::vector<SAFE_D3DXCONSTANT_DESC>(asConstantDesc.begin(), asConstantDesc.end());

		// copy the name (LPCSTR) to a new string
		for (UINT unI = 0; unI < (UINT)asConstantDesc.size(); unI++)
		{
			// copy name string
			sShaderDesc.asConstantDescriptions[unI].acName = asConstantDesc[unI].acName;
		}

		// init the shader rules
		InitShaderRules(&sShaderDesc);

		// set index, add to vector
		uShaderIx = (uint32_t)m_sModifierData.asPShaders.size();
		m_sModifierData.asPShaders.push_back(sShaderDesc);
	}

	// write down data to shader creator string hex strings
	static const char* acDigits = "0123456789ABCDEF";
	const char acModHeader[4] = { 'V', 'M', 'O', 'D' };
	CreatorMod* psModData = (CreatorMod*)&acFunction[uCreatorIx];
	CopyMemory(psModData->acModHeader, acModHeader, 4);
	HexString(psModData->acHash, uHash, 8);
	HexString(psModData->acIndex, uShaderIx, 4);

#ifdef TRACE_MODIFIER
	// output constants...
	for (VIREIO_D3D9_CONSTANT_DESC sDesc : asConstantDesc)
	{
		OutputDebugStringA(sDesc.acName.c_str());
	}
	// and shader hash, size, creator string index
	{ wchar_t buf[128]; wsprintf(buf, L"Hash %x:Size %u:CrIx %u", uHash, uSizeOfData, uCreatorIx); OutputDebugString(buf); }
	if (uCreatorIx)
	{
		std::string atCreator = std::string((char*)&acFunction[uCreatorIx]);
		OutputDebugStringA(atCreator.c_str());
	}
#endif		

	// create shader using modified function byte code and return
	HRESULT nHr = S_OK;
	nHr = m_pcDeviceCurrent->CreatePixelShader((const DWORD*)acFunction.data(), *pppcShader);
	nFlags = (int)AQU_PluginFlags::ImmediateReturnFlag;
	return nHr;
}

/// <summary>D3D9 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
HRESULT MatrixModifier::VB_Apply(int& nFlags)
{
	if (!m_ppInput[(int)STS_Decommanders::VB_Apply]) return S_OK;	void** ppIn = (void**)(m_ppInput[(int)STS_Decommanders::VB_Apply]);
	return E_NOTIMPL;
}

#pragma endregion
#endif