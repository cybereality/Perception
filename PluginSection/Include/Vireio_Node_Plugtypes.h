/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_Node_Plugtypes.h> :
Copyright (C) 2015 Denis Reischl

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

#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include<d3d11_1.h>
#include<d3d11.h>
#include<d3d10_1.h>
#include<d3d10.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"..//..//Aquilinus/Aquilinus//AQU_NodesStructures.h"
#include"..\..\..\Include\VireioMenu.h"
#include"..\VireioCore\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierDataStructures.h"

#pragma region global fields
/// <summary>
/// Global node width.
/// Do NOT change that ! Bound to node drawing functionality !
/// </summary>
constexpr DWORD g_uGlobalNodeWidth = 512;

/// <summary>
/// Plugin flags
/// </summary>
enum AQU_PluginFlags
{
	DoubleCallFlag = 512,       /** Double call : call D3D method -> provoke -> call D3D method **/
	ImmediateReturnFlag = 1024, /** Immediate return : D3D method call replaced by plugin **/
};
#pragma endregion

/// <summary>
/// All Vireio plugin connections identifiers
/// </summary>
namespace VLink
{
	/// <summary>
	/// enumeration
	/// </summary>
	enum class _L
	{
		TrackerData,
		StereoData,
		ModifierData
	};

	/// <summary>
	/// 
	/// </summary>
	/// <param name="nLink"></param>
	/// <returns>Identifier</returns>
	constexpr int Link(_L nLink)
	{
		return NOD_Plugtype::PluginRegister() + (int)nLink;
	}

	/// <summary>
	/// String identifier
	/// </summary>
	/// <param name="nLink">enumeration id</param>
	/// <returns>Name String</returns>
	constexpr LPWSTR Name(_L nLink)
	{
		switch (nLink)
		{
		case _L::TrackerData:
			return L"Tracked";
		case _L::StereoData:
			return L"Stereo";
		case _L::ModifierData:
			return L"Modified";

		default:
			break;
		}

		return L"unknown Link";
	}
};

#pragma region ini file helpers
/// <summary>
/// Provides the keyboard code by initialization string.
/// Also returns WM_MOUSEMOVE code by string.
/// </summary>
/// <param name="szCode">String name</param>
/// <returns>VK code</returns>
UINT GetVkCodeByString(std::string szCode)
{
	// 0...9 or A...Z ??
	if (szCode.length() == 4)
	{
		wchar_t uc = szCode[3];
		return (UINT)uc;
	}

	if (szCode == "X") return                 0x00;
	if (szCode == "VK_LBUTTON") return        0x01;
	if (szCode == "VK_RBUTTON") return        0x02;
	if (szCode == "VK_CANCEL") return         0x03;
	if (szCode == "VK_MBUTTON") return        0x04;
	if (szCode == "VK_XBUTTON1") return       0x05;
	if (szCode == "VK_XBUTTON2") return       0x06;
	if (szCode == "VK_BACK") return           0x08;
	if (szCode == "VK_TAB") return            0x09;
	if (szCode == "VK_CLEAR") return          0x0C;
	if (szCode == "VK_RETURN") return         0x0D;
	if (szCode == "VK_SHIFT") return          0x10;
	if (szCode == "VK_CONTROL") return        0x11;
	if (szCode == "VK_MENU") return           0x12;
	if (szCode == "VK_PAUSE") return          0x13;
	if (szCode == "VK_CAPITAL") return        0x14;
	if (szCode == "VK_KANA") return           0x15;
	if (szCode == "VK_HANGEUL") return        0x15;
	if (szCode == "VK_HANGUL") return         0x15;
	if (szCode == "VK_JUNJA") return          0x17;
	if (szCode == "VK_FINAL") return          0x18;
	if (szCode == "VK_HANJA") return          0x19;
	if (szCode == "VK_KANJI") return          0x19;
	if (szCode == "VK_ESCAPE") return         0x1B;
	if (szCode == "VK_CONVERT") return        0x1C;
	if (szCode == "VK_NONCONVERT") return     0x1D;
	if (szCode == "VK_ACCEPT") return         0x1E;
	if (szCode == "VK_MODECHANGE") return     0x1F;
	if (szCode == "VK_SPACE") return          0x20;
	if (szCode == "VK_PRIOR") return          0x21;
	if (szCode == "VK_NEXT") return           0x22;
	if (szCode == "VK_END") return            0x23;
	if (szCode == "VK_HOME") return           0x24;
	if (szCode == "VK_LEFT") return           0x25;
	if (szCode == "VK_UP") return             0x26;
	if (szCode == "VK_RIGHT") return          0x27;
	if (szCode == "VK_DOWN") return           0x28;
	if (szCode == "VK_SELECT") return         0x29;
	if (szCode == "VK_PRINT") return          0x2A;
	if (szCode == "VK_EXECUTE") return        0x2B;
	if (szCode == "VK_SNAPSHOT") return       0x2C;
	if (szCode == "VK_INSERT") return         0x2D;
	if (szCode == "VK_DELETE") return         0x2E;
	if (szCode == "VK_HELP") return           0x2F;
	if (szCode == "VK_LWIN") return           0x5B;
	if (szCode == "VK_RWIN") return           0x5C;
	if (szCode == "VK_APPS") return           0x5D;
	if (szCode == "VK_SLEEP") return          0x5F;
	if (szCode == "VK_NUMPAD0") return        0x60;
	if (szCode == "VK_NUMPAD1") return        0x61;
	if (szCode == "VK_NUMPAD2") return        0x62;
	if (szCode == "VK_NUMPAD3") return        0x63;
	if (szCode == "VK_NUMPAD4") return        0x64;
	if (szCode == "VK_NUMPAD5") return        0x65;
	if (szCode == "VK_NUMPAD6") return        0x66;
	if (szCode == "VK_NUMPAD7") return        0x67;
	if (szCode == "VK_NUMPAD8") return        0x68;
	if (szCode == "VK_NUMPAD9") return        0x69;
	if (szCode == "VK_MULTIPLY") return       0x6A;
	if (szCode == "VK_ADD") return            0x6B;
	if (szCode == "VK_SEPARATOR") return      0x6C;
	if (szCode == "VK_SUBTRACT") return       0x6D;
	if (szCode == "VK_DECIMAL") return        0x6E;
	if (szCode == "VK_DIVIDE") return         0x6F;
	if (szCode == "VK_F1") return             0x70;
	if (szCode == "VK_F2") return             0x71;
	if (szCode == "VK_F3") return             0x72;
	if (szCode == "VK_F4") return             0x73;
	if (szCode == "VK_F5") return             0x74;
	if (szCode == "VK_F6") return             0x75;
	if (szCode == "VK_F7") return             0x76;
	if (szCode == "VK_F8") return             0x77;
	if (szCode == "VK_F9") return             0x78;
	if (szCode == "VK_F10") return            0x79;
	if (szCode == "VK_F11") return            0x7A;
	if (szCode == "VK_F12") return            0x7B;
	if (szCode == "VK_F13") return            0x7C;
	if (szCode == "VK_F14") return            0x7D;
	if (szCode == "VK_F15") return            0x7E;
	if (szCode == "VK_F16") return            0x7F;
	if (szCode == "VK_F17") return            0x80;
	if (szCode == "VK_F18") return            0x81;
	if (szCode == "VK_F19") return            0x82;
	if (szCode == "VK_F20") return            0x83;
	if (szCode == "VK_F21") return            0x84;
	if (szCode == "VK_F22") return            0x85;
	if (szCode == "VK_F23") return            0x86;
	if (szCode == "VK_F24") return            0x87;
	if (szCode == "VK_NUMLOCK") return        0x90;
	if (szCode == "VK_SCROLL") return         0x91;
	if (szCode == "VK_LSHIFT") return         0xA0;
	if (szCode == "VK_RSHIFT") return         0xA1;
	if (szCode == "VK_LCONTROL") return       0xA2;
	if (szCode == "VK_RCONTROL") return       0xA3;
	if (szCode == "VK_LMENU") return          0xA4;
	if (szCode == "VK_RMENU") return          0xA5;
	if (szCode == "WM_MOUSEMOVE") return      WM_MOUSEMOVE;

	return 0x00;
}

/**
* Provides the initialization string by string.
* Also returns string >WM_MOUSEMOVE< by code.
***/
std::string GetStringByVKCode(UINT unCode)
{
	// 0...9 or A...Z ??
	if (((unCode >= (UINT)'0') && (unCode <= (UINT)'9')) ||
		((unCode >= (UINT)'A') && (unCode <= (UINT)'Z')))
	{
		std::string szRet = std::string("VK_X");
		szRet[3] = (char)unCode;
		return szRet;
	}

	if (unCode == 0x00) return "X";
	if (unCode == 0x01) return "VK_LBUTTON";
	if (unCode == 0x02) return "VK_RBUTTON";
	if (unCode == 0x03) return "VK_CANCEL";
	if (unCode == 0x04) return "VK_MBUTTON";
	if (unCode == 0x05) return "VK_XBUTTON1";
	if (unCode == 0x06) return "VK_XBUTTON2";
	if (unCode == 0x08) return "VK_BACK";
	if (unCode == 0x09) return "VK_TAB";
	if (unCode == 0x0C) return "VK_CLEAR";
	if (unCode == 0x0D) return "VK_RETURN";
	if (unCode == 0x10) return "VK_SHIFT";
	if (unCode == 0x11) return "VK_CONTROL";
	if (unCode == 0x12) return "VK_MENU";
	if (unCode == 0x13) return "VK_PAUSE";
	if (unCode == 0x14) return "VK_CAPITAL";
	if (unCode == 0x15) return "VK_KANA";
	if (unCode == 0x15) return "VK_HANGEUL";
	if (unCode == 0x15) return "VK_HANGUL";
	if (unCode == 0x17) return "VK_JUNJA";
	if (unCode == 0x18) return "VK_FINAL";
	if (unCode == 0x19) return "VK_HANJA";
	if (unCode == 0x19) return "VK_KANJI";
	if (unCode == 0x1B) return "VK_ESCAPE";
	if (unCode == 0x1C) return "VK_CONVERT";
	if (unCode == 0x1D) return "VK_NONCONVERT";
	if (unCode == 0x1E) return "VK_ACCEPT";
	if (unCode == 0x1F) return "VK_MODECHANGE";
	if (unCode == 0x20) return "VK_SPACE";
	if (unCode == 0x21) return "VK_PRIOR";
	if (unCode == 0x22) return "VK_NEXT";
	if (unCode == 0x23) return "VK_END";
	if (unCode == 0x24) return "VK_HOME";
	if (unCode == 0x25) return "VK_LEFT";
	if (unCode == 0x26) return "VK_UP";
	if (unCode == 0x27) return "VK_RIGHT";
	if (unCode == 0x28) return "VK_DOWN";
	if (unCode == 0x29) return "VK_SELECT";
	if (unCode == 0x2A) return "VK_PRINT";
	if (unCode == 0x2B) return "VK_EXECUTE";
	if (unCode == 0x2C) return "VK_SNAPSHOT";
	if (unCode == 0x2D) return "VK_INSERT";
	if (unCode == 0x2E) return "VK_DELETE";
	if (unCode == 0x2F) return "VK_HELP";
	if (unCode == 0x5B) return "VK_LWIN";
	if (unCode == 0x5C) return "VK_RWIN";
	if (unCode == 0x5D) return "VK_APPS";
	if (unCode == 0x5F) return "VK_SLEEP";
	if (unCode == 0x60) return "VK_NUMPAD0";
	if (unCode == 0x61) return "VK_NUMPAD1";
	if (unCode == 0x62) return "VK_NUMPAD2";
	if (unCode == 0x63) return "VK_NUMPAD3";
	if (unCode == 0x64) return "VK_NUMPAD4";
	if (unCode == 0x65) return "VK_NUMPAD5";
	if (unCode == 0x66) return "VK_NUMPAD6";
	if (unCode == 0x67) return "VK_NUMPAD7";
	if (unCode == 0x68) return "VK_NUMPAD8";
	if (unCode == 0x69) return "VK_NUMPAD9";
	if (unCode == 0x6A) return "VK_MULTIPLY";
	if (unCode == 0x6B) return "VK_ADD";
	if (unCode == 0x6C) return "VK_SEPARATOR";
	if (unCode == 0x6D) return "VK_SUBTRACT";
	if (unCode == 0x6E) return "VK_DECIMAL";
	if (unCode == 0x6F) return "VK_DIVIDE";
	if (unCode == 0x70) return "VK_F1";
	if (unCode == 0x71) return "VK_F2";
	if (unCode == 0x72) return "VK_F3";
	if (unCode == 0x73) return "VK_F4";
	if (unCode == 0x74) return "VK_F5";
	if (unCode == 0x75) return "VK_F6";
	if (unCode == 0x76) return "VK_F7";
	if (unCode == 0x77) return "VK_F8";
	if (unCode == 0x78) return "VK_F9";
	if (unCode == 0x79) return "VK_F10";
	if (unCode == 0x7A) return "VK_F11";
	if (unCode == 0x7B) return "VK_F12";
	if (unCode == 0x7C) return "VK_F13";
	if (unCode == 0x7D) return "VK_F14";
	if (unCode == 0x7E) return "VK_F15";
	if (unCode == 0x7F) return "VK_F16";
	if (unCode == 0x80) return "VK_F17";
	if (unCode == 0x81) return "VK_F18";
	if (unCode == 0x82) return "VK_F19";
	if (unCode == 0x83) return "VK_F20";
	if (unCode == 0x84) return "VK_F21";
	if (unCode == 0x85) return "VK_F22";
	if (unCode == 0x86) return "VK_F23";
	if (unCode == 0x87) return "VK_F24";
	if (unCode == 0x90) return "VK_NUMLOCK";
	if (unCode == 0x91) return "VK_SCROLL";
	if (unCode == 0xA0) return "VK_LSHIFT";
	if (unCode == 0xA1) return "VK_RSHIFT";
	if (unCode == 0xA2) return "VK_LCONTROL";
	if (unCode == 0xA3) return "VK_RCONTROL";
	if (unCode == 0xA4) return "VK_LMENU";
	if (unCode == 0xA5) return "VK_RMENU";
	if (unCode == WM_MOUSEMOVE) return "WM_MOUSEMOVE";

	return "X";
}

/**
* Ini file helper.
***/
DWORD GetIniFileSetting(DWORD unDefault, LPCSTR szAppName, LPCSTR szKeyName, LPCSTR szFileName, bool bFileExists)
{
	DWORD unRet = 0;
	char szBuffer[128];

	if (bFileExists)
	{
		// get value and write down if default (since maybe not present)
		std::stringstream sz;
		sz << unDefault;
		GetPrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szBuffer, 128, szFileName);
		sz = std::stringstream(szBuffer);
		sz >> unRet;
		if (unRet == unDefault) WritePrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szFileName);
	}
	else
	{
		// get value and write down if default (since maybe not present)
		std::stringstream sz;
		sz << unDefault;
		WritePrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szFileName);
		unRet = unDefault;
	}

	return unRet;
}

/**
* Ini file helper.
***/
float GetIniFileSetting(float fDefault, LPCSTR szAppName, LPCSTR szKeyName, LPCSTR szFileName, bool bFileExists)
{
	float fRet = 0;
	char szBuffer[128];

	if (bFileExists)
	{
		// get value and write down if default (since maybe not present)
		std::stringstream sz;
		sz << fDefault;
		GetPrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szBuffer, 128, szFileName);
		sz = std::stringstream(szBuffer);
		sz >> fRet;
		if (fRet == fDefault) WritePrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szFileName);
	}
	else
	{
		// write down, new file
		std::stringstream sz;
		sz << fDefault;
		WritePrivateProfileStringA(szAppName, szKeyName, sz.str().c_str(), szFileName);
		fRet = fDefault;
	}

	return fRet;
}

/**
* Ini file helper.
***/
std::string GetIniFileSetting(std::string strDefault, LPCSTR szAppName, LPCSTR szKeyName, LPCSTR szFileName, bool bFileExists)
{
	std::string strRet;
	char szBuffer[128];

	if (bFileExists)
	{
		// get value and write down if default (since maybe not present)
		GetPrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szBuffer, 128, szFileName);
		strRet = std::string(szBuffer);
		if (strRet == strDefault) WritePrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szFileName);
	}
	else
	{
		// write down, new file
		WritePrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szFileName);
	}

	return strRet;
}

/**
* Ini file helper. Converts string to virtual keyboard code.
***/
UINT GetIniFileSettingKeyCode(std::string strDefault, LPCSTR szAppName, LPCSTR szKeyName, LPCSTR szFileName, bool bFileExists)
{
	UINT unRet = 0;
	char szBuffer[128];

	if (bFileExists)
	{
		// get value and write down if default (since maybe not present)
		GetPrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szBuffer, 128, szFileName);
		std::string str = std::string(szBuffer);
		if (str == strDefault) WritePrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szFileName);
		unRet = GetVkCodeByString(str);
	}
	else
	{
		// write down, new file
		WritePrivateProfileStringA(szAppName, szKeyName, strDefault.c_str(), szFileName);
		unRet = GetVkCodeByString(strDefault);
	}

	return unRet;
}
#pragma endregion

#pragma region /// => Vireio plugin structures
/// <summary>
/// Simple left, right enumeration.
/// </summary>
enum RenderPosition
{
	// probably need an 'Original' here
	Left = 1,
	Right = 2
};

/// <summary>
/// Base structure for all connections
/// </summary>
struct VireioPluginData
{
	/// <summary>
	/// Array of all possible connected menues.
	/// To be created within cinema node.
	/// </summary>
	std::vector<VireioSubMenu*>* aasMenu = nullptr;

	/// <returns>Link identifier for this structure</returns>
	const virtual unsigned GetPlugtype() { return 0; }
};

/// <summary>
/// All data from the HMD tracker (Oculus, OpenVR, ... )
/// </summary>
struct HMDTrackerData : public VireioPluginData
{
	/// <summary>
	/// Euler angles
	/// </summary>
	struct _Vec3Eu
	{
		float fYaw, fPitch, fRoll;
	} sEu;

	/// <summary>
	/// Position
	/// </summary>
	struct _Vec3Po
	{
		float fX, fY, fZ;
	} sPo;

	/// <summary>
	/// Projection left/right
	/// </summary>
	D3DMATRIX sProj[2];

	/// <summary>
	/// View
	/// </summary>
	D3DMATRIX sView;

	/// <summary>
	/// Texture resolution.
	/// </summary>
	struct _Vec2Tx
	{
		UINT32 fW, fH;
	} sTx;

	/// <returns>Link identifier for this structure</returns>
	const virtual unsigned GetPlugtype() { return VLink::Link(VLink::_L::StereoData); }
};

/// <summary>
/// All data from the Matrix Modifier
/// </summary>
struct ModifierData : public VireioPluginData
{
#ifdef VIREIO_D3D11
	/// <summary>
	/// The d3d11 vertex shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D11_Shader> asVShaders;
	/// <summary>
	/// The d3d11 pixel shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D11_Shader> asPShaders;

	// create output pointers
	/*m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_VertexShader] = (void*)&m_apcVSActiveConstantBuffers11[0];
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_HullShader] = (void*)&m_apcHSActiveConstantBuffers11[0];
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_DomainShader] = (void*)&m_apcDSActiveConstantBuffers11[0];
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_GeometryShader] = (void*)&m_apcGSActiveConstantBuffers11[0];
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_PixelShader] = (void*)&m_apcPSActiveConstantBuffers11[0];
	m_pvOutput[STS_Commanders::ppActiveRenderTargets_DX11] = (void*)&m_apcActiveRenderTargetViews11[0];
	m_pvOutput[STS_Commanders::ppActiveDepthStencil_DX11] = (void*)&m_apcActiveDepthStencilView11[0];
	
	m_pvOutput[STS_Commanders::ppActiveRenderTargets_DX11];
	m_pvOutput[STS_Commanders::ppActiveDepthStencil_DX11];
	
	*/
	/*
	* m_asVShaders
	* m_asPShaders
	*/
#elif defined VIREIO_D3D9
	/// <summary>
	/// The d3d9 vertex shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D9_Shader> asVShaders;
	/// <summary>
	/// The d3d9 pixel shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D9_Shader> asPShaders;
#endif
	/// <summary>
	/// The active vertex shader index.
	/// Only used if codemod method is active.
	/// </summary>
	uint32_t uActiveVSIx;
	/// <summary>
	/// The active pixel shader index.
	/// Only used if codemod method is active.
	/// </summary>
	uint32_t uActivePSIx;
	/// <summary>
	/// True if a vertex shader is active.
	/// </summary>
	bool bVSActive;
	/// <summary>
	/// True if a pixel shader is active.
	/// </summary>
	bool bPSActive;
	/// <summary>
	/// The stored view transform set via SetTransform() l/r.
	/// </summary>
	D3DXMATRIX sMatView[2];
	/// <summary>
	/// The stored projection transform set via SetTransform() l/r.
	/// </summary>
	D3DXMATRIX sMatProj[2];
	/// <summary>
	/// True if View matrix set via SetTransform.
	/// </summary>
	bool bViewSet;
	/// <summary>
	/// True if Projection matrix set via SetTransform.
	/// </summary>
	bool bProjSet;
	/// <summary>
	/// Current drawing side, only changed in StereoSplitter->SetDrawingSide().
	/// </summary>
	RenderPosition eCurrentRenderingSide;

	/// <returns>Link identifier for this structure</returns>
	const virtual unsigned GetPlugtype() { return VLink::Link(VLink::_L::StereoData); }
};

/// <summary>
/// All data from the Vireio cinema.
/// </summary>
struct StereoData : public VireioPluginData
{
	/// TODO !! STORE DX11 DEVICE HERE FOR HMD !!

	/// <summary>
	/// Active output textures (shader bind flag), for both eyes.
	/// The back buffer surface copies.
	/// </summary>
	union
	{
		ID3D10Texture2D* pcTex10[2];
		ID3D11Texture2D* pcTex11[2];
	};

	/// <summary>
	/// Texture data based on D3D version l/r
	/// </summary>
	union
	{
		IDirect3DTexture9* pcTex9Input[2];
		ID3D10ShaderResourceView* pcTex10InputSRV[2];
		ID3D11ShaderResourceView* pcTex11InputSRV[2];
	};

	/// <summary>
	/// Draw tex l/r
	/// </summary>
	ID3D11Texture2D* pcTex11Draw[2];

	/// <summary>
	/// Draw tex RTV l/r
	/// </summary>
	ID3D11RenderTargetView* pcTex11DrawRTV[2];

	/// <summary>
	///  Draw tex SRV l/r
	/// </summary>
	ID3D11ShaderResourceView* pcTex11DrawSRV[2];

	/// <summary>
	/// Menu texture
	/// </summary>
	ID3D11Texture2D* pcTexMenu;

	/// <summary>
	/// Menu texture SRV
	/// </summary>
	ID3D11ShaderResourceView* pcTexMenuSRV;

	/// <summary>
	/// Menu texture RTV
	/// </summary>
	ID3D11RenderTargetView* pcTexMenuRTV;

	/// <returns>Link identifier for this structure</returns>
	const virtual unsigned GetPlugtype() { return VLink::Link(VLink::_L::StereoData); }
};

#pragma endregion

#pragma region /// => ImGui helpers
namespace ImGui
{
	/// <summary>
	/// Toggle button.
	/// Automatically choses colors based on button color settings.
	/// </summary>
	/// <param name="acLabel">Button label</param>
	/// <param name="pbToggle">Toggle boolean</param>
	/// <returns>True if clicked</returns>
	bool ToggleButton(const char* acLabel, bool* pbToggle)
	{
		bool bRet = false;
		if (!pbToggle) return bRet;
		if (*pbToggle == true)
		{
			ImGui::PushID(acLabel);
			ImVec4 sColB = ImGui::GetStyleColorVec4(ImGuiCol_Button); sColB.x = 1.f - sColB.x; sColB.y = 1.f - sColB.y; sColB.z = 1.f - sColB.z;
			ImVec4 sColBH = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered); sColBH.x = 1.f - sColBH.x; sColBH.y = 1.f - sColBH.y; sColBH.z = 1.f - sColBH.z;
			ImVec4 sColBA = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive); sColBA.x = 1.f - sColBA.x; sColBA.y = 1.f - sColBA.y; sColBA.z = 1.f - sColBA.z;
			ImGui::PushStyleColor(ImGuiCol_Button, sColB);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, sColBH);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, sColBA);
			ImGui::Button(acLabel);
			if (bRet = ImGui::IsItemClicked(0))
			{
				*pbToggle = !(*pbToggle);
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else
		{
			if (bRet = ImGui::Button(acLabel))
				*pbToggle = true;
		}
		return bRet;
	}

	/// <summary>
	/// Helper callback for ImGui methods.
	/// </summary>
	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			std::string* str = (std::string*)data->UserData;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		return 0;
	}

	/// <summary>
	/// ImGui InputText control using std::string wrapper
	/// </summary>
	/// <param name="acLabel">ImGui Label</param>
	/// <param name="acText">Current text, to be edited by user</param>
	/// <param name="nFlags">Additional flags, callback flag will be applied.</param>
	/// <returns>True if input text has changed</returns>
	bool InputText(const char* acLabel, std::string* acText, ImGuiInputTextFlags nFlags)
	{
		nFlags |= ImGuiInputTextFlags_CallbackResize;
		return InputText(acLabel, (char*)acText->c_str(), acText->capacity() + 1, nFlags, InputTextCallback, (void*)acText);
	}

	/// <summary>
	/// Helper callback for ImGui methods.
	/// </summary>
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	/// <summary>
	/// ImGui combo box using std::vector
	/// </summary>
	/// <param name="acLabel">Combo box label</param>
	/// <param name="nCurrIndex">Current combo box selection index</param>
	/// <param name="aacValues">Vector of values (string)</param>
	/// <returns>True if combo box selection is changed</returns>
	bool Combo(const char* acLabel, int* nCurrIndex, std::vector<std::string>& aacValues)
	{
		if (aacValues.empty()) { return false; }
		return Combo(acLabel, nCurrIndex, vector_getter,
			static_cast<void*>(&aacValues), (int)aacValues.size());
	}

	/// <summary>
	/// ImGui list box using std::vector
	/// </summary>
	/// <param name="acLabel">List box label</param>
	/// <param name="nCurrIndex">Current list box selection index</param>
	/// <param name="aacValues">Vector of values (string)</param>
	/// <param name="nHeight_in_items">Height of the list box (in items)</param>
	/// <returns>True if list box selection is changed</returns>
	bool ListBox(const char* acLabel, int* nCurrIndex, std::vector<std::string>& aacValues, int nHeight_in_items = -1)
	{
		if (aacValues.empty()) { return false; }
		return ListBox(acLabel, nCurrIndex, vector_getter,
			static_cast<void*>(&aacValues), (int)aacValues.size(), nHeight_in_items);
	}

	/// <summary>
	/// Shows a little help description if hovered.
	/// </summary>
	/// <param name="acLabel">Label, usually (?)</param>
	/// <param name="acDesc">Help description</param>
	static void HelpMarker(const char* acLabel, const char* acDesc)
	{
		ImGui::TextDisabled(acLabel);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(acDesc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	/// <summary>
	/// Shows a little help description if last item hovered.
	/// </summary>
	/// <param name="acDesc">Help description</param>
	static void HelpTooltip(const char* acDesc)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(acDesc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}
#pragma endregion

