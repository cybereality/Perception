/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin
Copyright (C) 2014 Denis Reischl

File <VireioCinema.cpp> and
Class <VireioCinema> :
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

#include"VireioCinema.h"
#include"Shader_D3D9.h"
#include"Resources.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }
#define DEBUG_HR(msg, hr) { wchar_t buf[128]; wsprintf(buf, L"%s : %x", msg, hr); OutputDebugString(buf); }

#define INTERFACE_IDIRECT3DDEVICE9 8
#define INTERFACE_IDIRECT3DSWAPCHAIN9 15
#define INTERFACE_IDXGISWAPCHAIN 29
#define	METHOD_IDIRECT3DDEVICE9_PRESENT 17
#define	METHOD_IDIRECT3DDEVICE9_ENDSCENE 42
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT 3
#define METHOD_IDXGISWAPCHAIN_PRESENT 8

/**
* Constructor.
***/
VireioCinema::VireioCinema() :AQU_Nodus(),
m_sCinemaData{},
m_psTrackerData(nullptr),
m_psSplitterData(nullptr),
m_sGeometryConstants{},
m_pcVSGeometry11(nullptr),
m_pcVLGeometry11(nullptr),
m_pcPSGeometry11(nullptr),
m_pcPSMenuScreen11(nullptr),
m_asRenderModels(),
m_pcBackBufferCopy(nullptr),
m_pcBackBufferCopySR(nullptr),
m_pcRS(nullptr),
m_hDummy(nullptr),
m_pcBlendState(nullptr),
m_unMenuModelIndex(0)
{
	// set default aspect ratio
	m_fAspectRatio = 1920.0f / 1080.0f;

	m_eD3DVersion = D3D_Undefined;

	m_pcD3D11Device = nullptr;
	m_pcD3D11Context = nullptr;

	m_pcConstantBufferGeometry = nullptr;
	m_pcSampler11 = nullptr;
	m_pcDSGeometry11[0] = nullptr;
	m_pcDSGeometry11[1] = nullptr;
	m_pcDSVGeometry11[0] = nullptr;
	m_pcDSVGeometry11[1] = nullptr;
	m_apcTex11InputSRV[0] = nullptr;
	m_apcTex11InputSRV[1] = nullptr;
	m_pcSamplerState = nullptr;
	m_pcTex9Copy[0] = nullptr;
	m_pcTex9Copy[1] = nullptr;
	m_pcSharedTexture[0] = nullptr;
	m_pcSharedTexture[1] = nullptr;
	m_pcTexCopy11[0] = nullptr;
	m_pcTexCopy11[1] = nullptr;
	m_pcTexCopy11SRV[0] = nullptr;
	m_pcTexCopy11SRV[1] = nullptr;

	D3DXMatrixIdentity(&m_sView);
	D3DXMatrixIdentity(&m_sToEye[0]);
	D3DXMatrixIdentity(&m_sToEye[1]);
	D3DXMatrixIdentity(&m_sProj[0]);
	D3DXMatrixIdentity(&m_sProj[1]);

	// locate or create the INI file
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;
	if (PathFileExistsA(szFilePathINI)) bFileExists = true;

	// set default room setup
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	m_sCinemaRoomSetup.sColorAmbient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	m_sCinemaRoomSetup.sColorDiffuse = D3DXCOLOR(1.0f, 0.2f, 0.7f, 1.0f);
	m_sCinemaRoomSetup.sLightDirection = D3DXVECTOR4(-0.7f, -0.6f, -0.02f, 1.0f);
	m_sCinemaRoomSetup.ePixelShaderFX_Screen = VireioCinema::CinemaRoomSetup::PixelShaderFX_Screen::Screen_GeometryDiffuseTexturedMouse;
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_ToonCloud;
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_WaterCaustic;
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_VoronoiSmooth;
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_Bubbles;
	m_sCinemaRoomSetup.ePixelShaderFX_Floor[0] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_StringTheory;
	m_sCinemaRoomSetup.ePixelShaderFX_Floor[1] = VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_Worley01;
	m_sCinemaRoomSetup.fScreenWidth = 6.0f; /**< default : 6 meters screen width **/
	m_sCinemaRoomSetup.fScreenLevel = 0.0f; /**< default : 0 meters height level */
	m_sCinemaRoomSetup.fScreenDepth = 3.0f; /**< default : 3 meters depth level */
	m_sCinemaRoomSetup.fScreenRotateX = 0.0f; /**< default : 0.0 degree x rotation */
	m_sCinemaRoomSetup.fScreenRotateY = 0.0f; /**< default : 0.0 degree y rotation */
	m_sCinemaRoomSetup.fMenuScreenDepth = 2.99f; /**< default : 2.99 meters depth level */
	m_sCinemaRoomSetup.fMenuScreenScale = 1.0f; /**< default : 1.0 scale */
	m_sCinemaRoomSetup.fMenuScreenRotateY = 0.0f; /**< default : 0.0 degree y rotation */
	m_sCinemaRoomSetup.fRoomScale = 1.0f; /**< default : 1.0 scale */
	m_sCinemaRoomSetup.bMenuIsHUD = FALSE;
	m_sCinemaRoomSetup.bPerformanceMode = FALSE;
	m_sCinemaRoomSetup.bImmersiveMode = FALSE;
	m_sCinemaRoomSetup.fGamma = 1.0f;
	m_unMouseTickCount = 2000;
	m_sImmersiveFullscreenSettings.fIPD = 0.064f;
	m_sImmersiveFullscreenSettings.fVSD = 1.25f; /**< default : the immersive mode screen is 1.25 meters away from eye ***/

	// TODO !! FX COLORS

	// read ini file
	m_sCinemaRoomSetup.sColorAmbient.a = GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.a, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.a", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorAmbient.r = GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.r, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.r", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorAmbient.g = GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.g, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.g", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorAmbient.b = GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.b, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.b", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorDiffuse.a = GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.a, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.a", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorDiffuse.r = GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.r, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.r", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorDiffuse.g = GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.g, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.g", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sColorDiffuse.b = GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.b, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.b", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sLightDirection.w = GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.w, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.w", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sLightDirection.x = GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.x, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.x", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sLightDirection.y = GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.y, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.y", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.sLightDirection.z = GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.z, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.z", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Screen = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Screen)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Screen, "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Screen.b", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Floor[0] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Floor[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Floor[0]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.ePixelShaderFX_Floor[1] = (VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Floor[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Floor[1]", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fScreenWidth = GetIniFileSetting(m_sCinemaRoomSetup.fScreenWidth, "Stereo Cinema", "sCinemaRoomSetup.fScreenWidth", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fScreenLevel = GetIniFileSetting(m_sCinemaRoomSetup.fScreenLevel, "Stereo Cinema", "sCinemaRoomSetup.fScreenLevel", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fScreenDepth = GetIniFileSetting(m_sCinemaRoomSetup.fScreenDepth, "Stereo Cinema", "sCinemaRoomSetup.fScreenDepth", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fScreenRotateX = GetIniFileSetting(m_sCinemaRoomSetup.fScreenRotateX, "Stereo Cinema", "sCinemaRoomSetup.fScreenRotateX", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fScreenRotateY = GetIniFileSetting(m_sCinemaRoomSetup.fScreenRotateY, "Stereo Cinema", "sCinemaRoomSetup.fScreenRotateY", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fMenuScreenDepth = GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenDepth, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenDepth", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fMenuScreenScale = GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenScale, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenScale", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fMenuScreenRotateY = GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenRotateY, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenRotateY", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.fRoomScale = GetIniFileSetting(m_sCinemaRoomSetup.fRoomScale, "Stereo Cinema", "sCinemaRoomSetup.fRoomScale", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.bMenuIsHUD = (GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bMenuIsHUD, "Stereo Cinema", "sCinemaRoomSetup.bMenuIsHUD", szFilePathINI, bFileExists) != 0);
	m_sCinemaRoomSetup.bPerformanceMode = (GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bPerformanceMode, "Stereo Cinema", "sCinemaRoomSetup.bPerformanceMode", szFilePathINI, bFileExists) != 0);
	m_sCinemaRoomSetup.bImmersiveMode = (GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bImmersiveMode, "Stereo Cinema", "sCinemaRoomSetup.bImmersiveMode", szFilePathINI, bFileExists) != 0);
	m_sCinemaRoomSetup.fGamma = GetIniFileSetting(m_sCinemaRoomSetup.fGamma, "Stereo Cinema", "sCinemaRoomSetup.fGamma", szFilePathINI, bFileExists);
	m_unMouseTickCount = GetIniFileSetting((DWORD)m_unMouseTickCount, "Stereo Cinema", "unMouseTickCount", szFilePathINI, bFileExists);
	m_sImmersiveFullscreenSettings.fIPD = GetIniFileSetting(m_sImmersiveFullscreenSettings.fIPD, "Stereo Presenter", "fIPD", szFilePathINI, bFileExists);
	m_sImmersiveFullscreenSettings.fVSD = GetIniFileSetting(m_sImmersiveFullscreenSettings.fVSD, "Stereo Presenter", "fVSD", szFilePathINI, bFileExists);

#pragma region /// => menu entries

	// create the menu
	ZeroMemory(&m_sMenu, sizeof(VireioSubMenu));
	m_sMenu.strSubMenu = "Cinema - All FX from ShaderToy.com";

	// this node requests hand pose data
	m_sMenu.bHandPosesRequest = true;
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Screen";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Screen::Screen_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Screen;
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Screen;
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Screen::Screen_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Screen)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Wall B";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Wall F";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Wall L";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Wall R";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall::Wall_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Wall)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Floor";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Floor[1];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Floor[1];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "FX Ceiling";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_NumberOfFX;
		sEntry.unChangeSize = 1;
		sEntry.punValue = (UINT*)&m_sCinemaRoomSetup.ePixelShaderFX_Floor[0];
		sEntry.unValue = (UINT)m_sCinemaRoomSetup.ePixelShaderFX_Floor[0];
		sEntry.bValueEnumeration = true;
		for (UINT unIx = 0; unIx < VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor::Floor_NumberOfFX; unIx++)
		{
			std::string strEnum = m_sCinemaRoomSetup.FX_AsString((VireioCinema::CinemaRoomSetup::PixelShaderFX_Floor)unIx); sEntry.astrValueEnumeration.push_back(strEnum);
		}
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Screen Width";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 1.0f;
		sEntry.fMaximum = 30.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fScreenWidth;
		sEntry.fValue = m_sCinemaRoomSetup.fScreenWidth;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Screen Level";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0f;
		sEntry.fMaximum = 15.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fScreenLevel;
		sEntry.fValue = m_sCinemaRoomSetup.fScreenLevel;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Screen Depth";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.1f;
		sEntry.fMaximum = 10.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fScreenDepth;
		sEntry.fValue = m_sCinemaRoomSetup.fScreenDepth;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Screen Rotation X";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -360.0f;
		sEntry.fMaximum = 360.0f;
		sEntry.fChangeSize = 3.0f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fScreenRotateX;
		sEntry.fValue = m_sCinemaRoomSetup.fScreenRotateX;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Screen Rotation Y";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -360.0f;
		sEntry.fMaximum = 360.0f;
		sEntry.fChangeSize = 3.0f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fScreenRotateY;
		sEntry.fValue = m_sCinemaRoomSetup.fScreenRotateY;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Room Scale";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.5f;
		sEntry.fMaximum = 4.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fRoomScale;
		sEntry.fValue = m_sCinemaRoomSetup.fRoomScale;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Menu Screen Scale";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 1.0f;
		sEntry.fMaximum = 10.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fMenuScreenScale;
		sEntry.fValue = m_sCinemaRoomSetup.fMenuScreenScale;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Menu Screen Depth";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.5f;
		sEntry.fMaximum = 10.0f;
		sEntry.fChangeSize = 0.1f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fMenuScreenDepth;
		sEntry.fValue = m_sCinemaRoomSetup.fMenuScreenDepth;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Menu Screen Rotate Y";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = -360.0f;
		sEntry.fMaximum = 360.0f;
		sEntry.fChangeSize = 3.0f;
		sEntry.pfValue = &m_sCinemaRoomSetup.fMenuScreenRotateY;
		sEntry.fValue = m_sCinemaRoomSetup.fMenuScreenRotateY;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Menu as HUD";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_sCinemaRoomSetup.bMenuIsHUD;
		sEntry.bValue = m_sCinemaRoomSetup.bMenuIsHUD;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Hide Cinema Room";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_sCinemaRoomSetup.bPerformanceMode;
		sEntry.bValue = m_sCinemaRoomSetup.bPerformanceMode;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Mouse Pointer Visible (ms)";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_UInt;
		sEntry.unMinimum = 0;
		sEntry.unMaximum = 10000;
		sEntry.unChangeSize = 100;
		sEntry.punValue = &m_unMouseTickCount;
		sEntry.unValue = m_unMouseTickCount;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Full Immersive Mode";
		sEntry.bIsActive = true;
		sEntry.bIsFullscreenswitch = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Bool;
		sEntry.pbValue = &m_sCinemaRoomSetup.bImmersiveMode;
		sEntry.bValue = m_sCinemaRoomSetup.bImmersiveMode;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Inter-Pupillary distance";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.0510f;
		sEntry.fMaximum = 0.0770f;
		sEntry.fChangeSize = 0.0001f;
		sEntry.pfValue = &m_sImmersiveFullscreenSettings.fIPD;
		sEntry.fValue = m_sImmersiveFullscreenSettings.fIPD;
		m_sMenu.asEntries.push_back(sEntry);
	}
	{
		VireioMenuEntry sEntry = {};
		sEntry.strEntry = "Virtual Screen Distance";
		sEntry.bIsActive = true;
		sEntry.eType = VireioMenuEntry::EntryType::Entry_Float;
		sEntry.fMinimum = 0.5f;
		sEntry.fMaximum = 2.5f;
		sEntry.fChangeSize = 0.01f;
		sEntry.pfValue = &m_sImmersiveFullscreenSettings.fVSD;
		sEntry.fValue = m_sImmersiveFullscreenSettings.fVSD;
		m_sMenu.asEntries.push_back(sEntry);
	}
#pragma endregion

}

/**
* Destructor.
***/
VireioCinema::~VireioCinema()
{
	SAFE_RELEASE(m_pcBlendState);
	for (UINT unI = 0; unI < (UINT)m_asRenderModels.size(); unI++)
	{
		SAFE_RELEASE(m_asRenderModels[unI].pcIndexBuffer);
		SAFE_RELEASE(m_asRenderModels[unI].pcVertexBuffer);
		SAFE_RELEASE(m_asRenderModels[unI].pcTextureSRV);
		SAFE_RELEASE(m_asRenderModels[unI].pcTexture);
		SAFE_RELEASE(m_asRenderModels[unI].pcEffect);
	}
	SAFE_RELEASE(m_pcRS);
	SAFE_RELEASE(m_pcBackBufferCopySR);
	SAFE_RELEASE(m_pcBackBufferCopy);
	SAFE_RELEASE(m_pcConstantBufferGeometry);
	SAFE_RELEASE(m_pcDSVGeometry11[0]);
	SAFE_RELEASE(m_pcDSVGeometry11[1]);
	SAFE_RELEASE(m_pcDSGeometry11[0]);
	SAFE_RELEASE(m_pcDSGeometry11[1]);
	SAFE_RELEASE(m_pcVLGeometry11);
	SAFE_RELEASE(m_pcVSGeometry11);
	SAFE_RELEASE(m_pcPSGeometry11);
	SAFE_RELEASE(m_pcPSMenuScreen11);
	SAFE_RELEASE(m_pcSamplerState);

	SendMessage(m_hDummy, WM_CLOSE, 0, 0);
}

/**
* Return the name of the Vireio Cinema node.
***/
const char* VireioCinema::GetNodeType()
{
	return "Vireio Cinema";
}

/**
* Returns a global unique identifier for the Vireio Cinema node.
***/
UINT VireioCinema::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 128
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Vireio Cinema node.
***/
LPWSTR VireioCinema::GetCategory()
{
	return L"Virtual Reality";
}

/**
* Returns a logo to be used for the Vireio Cinema node.
***/
HBITMAP VireioCinema::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioCinema.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR VireioCinema::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((VRC_Commanders)dwCommanderIndex)
	{
	case VRC_Commanders::Cinema:
		return VLink::Name(VLink::_L::CinemaData);
	}

	return L"x";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR VireioCinema::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((VRC_Decommanders)dwDecommanderIndex)
	{
	case VRC_Decommanders::Splitter:
		return VLink::Name(VLink::_L::SplitterData);
	case VRC_Decommanders::Tracker:
		return VLink::Name(VLink::_L::TrackerData);
	}

	return L"x";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD VireioCinema::GetCommanderType(DWORD dwCommanderIndex)
{
	switch ((VRC_Commanders)dwCommanderIndex)
	{
	case VRC_Commanders::Cinema:
		return VLink::Link(VLink::_L::CinemaData);
	}

	return 0;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD VireioCinema::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((VRC_Decommanders)dwDecommanderIndex)
	{
	case VRC_Decommanders::Splitter:
		return VLink::Link(VLink::_L::SplitterData);
	case VRC_Decommanders::Tracker:
		return VLink::Link(VLink::_L::TrackerData);
	}

	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* VireioCinema::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((VRC_Commanders)dwCommanderIndex)
	{
	case VRC_Commanders::Cinema:
		return (void*)&m_sCinemaData;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void VireioCinema::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((VRC_Decommanders)dwDecommanderIndex)
	{
	case VRC_Decommanders::Splitter:
		m_psSplitterData = (SplitterData*)pData;
	case VRC_Decommanders::Tracker:
		m_psTrackerData = (HMDTrackerData*)pData;
		break;
	}
}

/**
* Vireio Cinema supports D3D 9 Present() and EndScene() calls.
***/
bool VireioCinema::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if (((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
			(nD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT)) ||
			((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_ENDSCENE)) ||
			((nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9) &&
				(nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT)))
			return true;
	}
	else
		if ((nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9) &&
			(nD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
			return true;

	return false;
}

/**
* Virtual Cinema entry point.
***/
void* VireioCinema::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
#ifdef _DEBUG_CIN
	{ wchar_t buf[128]; wsprintf(buf, L"[CIN] ifc %u mtd %u", eD3DInterface, eD3DMethod); OutputDebugString(buf); }
#endif

	// only present accepted
	bool bValid = false;
	if (((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT)) ||
		((eD3DInterface == INTERFACE_IDIRECT3DDEVICE9) && (eD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT))) bValid = true;
	if (!bValid) return nullptr;

	// set this nodes sub menu pointer
	// TODO !! CREATE ALL NODES SUB MENU INDICES VECTOR
	if (m_sCinemaData.aasMenu)
	{
		if (!((*m_sCinemaData.aasMenu)[0]))
			(*m_sCinemaData.aasMenu)[0] = &m_sMenu;
	}


	// save ini file ?
	if (m_nIniFrameCount)
	{
		if (m_nIniFrameCount == 1)
			SaveIniSettings();
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
				if (nIx < 7)
				{
					// get device and context
					ID3D11Device* pcDevice = nullptr;
					ID3D11DeviceContext* pcContext = nullptr;
					HRESULT nHr = S_OK;
					if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
						nHr = GetDeviceAndContext((IDXGISwapChain*)pThis, &pcDevice, &pcContext);
					else
					{
						if (m_pcD3D11Device) pcDevice = m_pcD3D11Device; else nHr = E_FAIL;
						if (m_pcD3D11Context) pcContext = m_pcD3D11Context; else nHr = E_FAIL;
					}
					if (SUCCEEDED(nHr))
					{
						switch (nIx)
						{
						case 0: // screen
							SAFE_RELEASE(m_pcPSGeometry11);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPSGeometry11, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Screen))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 1: // wall front
							SAFE_RELEASE(m_asRenderModels[m_unWallFModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unWallFModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 2: // wall back
							SAFE_RELEASE(m_asRenderModels[m_unWallBModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unWallBModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 3: // wall left
							SAFE_RELEASE(m_asRenderModels[m_unWallLModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unWallLModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 4: // wall right
							SAFE_RELEASE(m_asRenderModels[m_unWallRModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unWallRModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 5: // floor
							SAFE_RELEASE(m_asRenderModels[m_unFloorModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unFloorModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Floor[1]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						case 6: // ceiling
							SAFE_RELEASE(m_asRenderModels[m_unCeilingModelIndex].pcEffect);
							if (FAILED(CreatePixelShaderEffect(pcDevice, &m_asRenderModels[m_unCeilingModelIndex].pcEffect, m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Floor[0]))))
								OutputDebugString(L"[CIN] Failed to create pixel shader. !");
							break;
						}
					}
					if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
					{
						SAFE_RELEASE(pcDevice);
						SAFE_RELEASE(pcContext);
					}
				}
			}
		}
	}

	// hand poses available ?
	for (UINT uHand = 0; uHand < 2; uHand++)
	{
		// pose available ?
		if (m_sMenu.sPosition[uHand].y == 0.0f) continue;

		// transform direction vector using this matrix and normalize it, negate
		D3DXVECTOR3 sDirectionZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVECTOR4 sDirection;
		D3DXVec3Transform(&sDirection, &sDirectionZ, &m_sMenu.sPoseMatrix[uHand]);
		sDirection.x *= -1.0f;
		sDirection.y *= -1.0f;
		D3DXVec4Normalize(&sDirection, &sDirection);
		float fPlaneDistanceZ = m_sMenu.sPosition[uHand].z + m_sCinemaRoomSetup.fScreenDepth;

		// get intersection point to screen...
		D3DXVECTOR3 sIntersect = D3DXVECTOR3(sDirection.x * (1.0f / sDirection.z), sDirection.y * (1.0f / sDirection.z), 0.0f);
		sIntersect *= fPlaneDistanceZ;

		// add position to intersection point
		sIntersect.x += m_sMenu.sPosition[uHand].x;
		sIntersect.y += m_sMenu.sPosition[uHand].y;

		// substract the height of the screen center and negate
		sIntersect.y -= m_sCinemaRoomSetup.fScreenLevel + 2.0f;
		sIntersect.y *= -1.0f;

		// set new mouse cursor position
		float fXClip = m_sCinemaRoomSetup.fScreenWidth / 2.0f;
		float fYClip = fXClip / 1.777777777777778f;
		if ((sIntersect.x >= -fXClip) && (sIntersect.y >= -fYClip) && (sIntersect.x <= fXClip) && (sIntersect.y <= fYClip) && (sDirection.z > 0.0f))
		{
			RECT sDesktop;
			HWND pDesktop = GetDesktopWindow();
			GetWindowRect(pDesktop, &sDesktop);
			float fXPos = ((sIntersect.x + fXClip) / (fXClip * 2.0f)) * (float)sDesktop.right;
			float fYPos = ((sIntersect.y + fYClip) / (fYClip * 2.0f)) * (float)sDesktop.bottom;

			SetCursorPos((int)fXPos, (int)fYPos);
			//bMouseCursorSet = true;

			// hardware emulation... if ever needed
			// POINT sPoint;
			// GetCursorPos(&sPoint);
			// mouse_event(MOUSEEVENTF_MOVE, (DWORD)fXPos - (DWORD)sPoint.x, (DWORD)fYPos - (DWORD)sPoint.y, 0, 0);
		}
	}

	// render cinema for specified D3D version
	switch (m_eD3DVersion)
	{
	case D3D_Undefined:
		// D3D version not defined ?
		switch (eD3DInterface)
		{
		case INTERFACE_IDIRECT3DDEVICE9:
		case INTERFACE_IDIRECT3DSWAPCHAIN9:
			m_eD3DVersion = D3D_9;
			break;
		case INTERFACE_IDXGISWAPCHAIN:
			if (m_eD3DVersion == D3D_Undefined)
			{
				// get device
				IUnknown* pcDevice = nullptr;
				((IDXGISwapChain*)pThis)->GetDevice(__uuidof(ID3D10Device), (void**)&pcDevice);
				if (pcDevice)
				{
					m_eD3DVersion = D3D_10;
					pcDevice->Release();
					return nullptr;
				}
				// get device
				((IDXGISwapChain*)pThis)->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
				if (pcDevice)
				{
					m_eD3DVersion = D3D_11;
					pcDevice->Release();
					return nullptr;
				}
				// no device ?
				OutputDebugString(L"[CIN] Could not resolve which D3D device is in use !");
			}
			break;
		}
		break;
	case D3D_9:
	{

		// get device 
		LPDIRECT3DDEVICE9 pcDevice = nullptr;
		bool bReleaseDevice = false;
		if (eD3DInterface == INTERFACE_IDIRECT3DDEVICE9)
		{
			pcDevice = (LPDIRECT3DDEVICE9)pThis;
		}
		else if (eD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
		{
			LPDIRECT3DSWAPCHAIN9 pSwapChain = (LPDIRECT3DSWAPCHAIN9)pThis;
			if (!pSwapChain)
			{
				OutputDebugString(L"[CIN] No swapchain !");
				return nullptr;
			}
			pSwapChain->GetDevice(&pcDevice);
			bReleaseDevice = true;
		}
		if (!pcDevice)
		{
			OutputDebugString(L"[CIN] No device !");
			return nullptr;
		}

		if (m_asRenderModels.size())
			// render
			RenderD3D9(pcDevice);
		else
			// ...or init all stuff
			InitD3D9(pcDevice);

		// release device if provided by swapchain
		if (bReleaseDevice) pcDevice->Release();
	}
	break;
	case D3D_10:
		break;
	case D3D_11:
	{
		// get device
		ID3D11Device* pcDevice = nullptr;
		((IDXGISwapChain*)pThis)->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
		if (pcDevice)
		{
			// get context
			ID3D11DeviceContext* pcContext = nullptr;
			pcDevice->GetImmediateContext(&pcContext);
			if (pcContext)
			{
				if (m_asRenderModels.size())
					// render
					RenderD3D11(pcDevice, pcContext, (IDXGISwapChain*)pThis);
				else
					// ...or init all stuff
					InitD3D11(pcDevice, pcContext);

				pcContext->Release();
			}
			pcDevice->Release();
			return nullptr;
		}
	}
	break;
	default:
		break;
	}
	return nullptr;
}

/**
* Inits all stuff (DX9).
***/
void VireioCinema::InitD3D9(LPDIRECT3DDEVICE9 pcDevice)
{
	OutputDebugString(L"[CIN] Init D3D9...");

	// create d3d11 device/context/swapchain
	if ((!m_pcD3D11Device) || (!m_pcD3D11Context))
	{
		CreateD3D11Device(true);
	}

	// init all stuff
	if ((m_pcD3D11Device) || (m_pcD3D11Context))
	{
		InitD3D11(m_pcD3D11Device, m_pcD3D11Context);
	}
}

/**
* Renders the virtual cinema (DX9).
***/
void VireioCinema::RenderD3D9(LPDIRECT3DDEVICE9 pcDevice)
{
	// no splitter data connected ?
	if (!m_psSplitterData)
	{
		// get back buffer
		IDirect3DSurface9* pcBackBuffer = nullptr;
		pcDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pcBackBuffer);

		if (!pcBackBuffer)
		{
			OutputDebugString(L"[CIN] No D3D9 backbuffer available !");
			return;
		}

		// get the description and create the copy texture
		D3DSURFACE_DESC sDescSurfaceD3D9 = {};
		pcBackBuffer->GetDesc(&sDescSurfaceD3D9);

		// copy texture created ?
		if (!m_pcTex9Copy[0])
		{
			HRESULT nHr = pcDevice->CreateTexture(sDescSurfaceD3D9.Width, sDescSurfaceD3D9.Height, 1, 0, sDescSurfaceD3D9.Format, D3DPOOL_SYSTEMMEM, &m_pcTex9Copy[0], NULL);
			if (!m_pcTex9Copy[0])
			{
				DEBUG_HR(L"[CIN] Failed to create D3D9 copy texture : ", nHr);
				return;
			}
		}

		// release old texture
		if (m_pcTexCopy11SRV[0]) m_pcTexCopy11SRV[0]->Release();
		if (m_pcTexCopy11[0]) m_pcTexCopy11[0]->Release();

		// copy d3d9 texture
		IDirect3DSurface9* pcSurfaceDst = nullptr;
		m_pcTex9Copy[0]->GetSurfaceLevel(0, &pcSurfaceDst);
		pcDevice->GetRenderTargetData(pcBackBuffer, pcSurfaceDst);
		//		pcDevice->StretchRect(pcBackBuffer, NULL, pcSurfaceDst, NULL, D3DTEXF_NONE); // USE THIS HERE ???
		if (pcBackBuffer) pcBackBuffer->Release();

		// lock and create d3d11 tex
		D3DLOCKED_RECT sRect = {};
		if (SUCCEEDED(pcSurfaceDst->LockRect(&sRect, NULL, NULL)))
		{
			D3D11_TEXTURE2D_DESC sDesc = {};
			sDesc.Width = sDescSurfaceD3D9.Width;
			sDesc.Height = sDescSurfaceD3D9.Height;
			sDesc.MipLevels = 1;
			sDesc.ArraySize = 1;
			sDesc.Format = GetDXGI_Format(sDescSurfaceD3D9.Format);
			sDesc.SampleDesc.Count = 1;
			sDesc.SampleDesc.Quality = 0;
			sDesc.Usage = D3D11_USAGE_DEFAULT;
			sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			sDesc.CPUAccessFlags = 0;
			sDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA sData = {};
			sData.pSysMem = sRect.pBits;
			sData.SysMemPitch = sRect.Pitch;
			if (FAILED(m_pcD3D11Device->CreateTexture2D(&sDesc, &sData, (ID3D11Texture2D**)&m_pcTexCopy11[0])))
			{
				OutputDebugString(L"[CIN] Failed to create copy texture !");
				return;
			}

			pcSurfaceDst->UnlockRect();

			// create shader resource view
			if (m_pcTexCopy11[0])
			{
				if (FAILED(m_pcD3D11Device->CreateShaderResourceView(m_pcTexCopy11[0], NULL, &m_pcTexCopy11SRV[0])))
					OutputDebugString(L"[CIN] Failed to create shader resource view.");
			}

			// set stereo to mono
			m_pcTexCopy11[1] = m_pcTexCopy11[0];
			m_pcTexCopy11SRV[1] = m_pcTexCopy11SRV[0];

		}

		if (pcSurfaceDst) pcSurfaceDst->Release();


		if ((m_pcD3D11Device) || (m_pcD3D11Context))
		{
			RenderD3D11(m_pcD3D11Device, m_pcD3D11Context, nullptr);
		}
		return;
	}
	else
	{
		// connected textures already initialized ?? return if not
		if ((!(m_psSplitterData->pcTex9Input[0])) || (!(m_psSplitterData->pcTex9Input[1])))
		{
			static int nDummyCounter = 5;
			if ((nDummyCounter--) <= 0)
				OutputDebugString(L"[CIN] No Input Textures !");
			return;
		}

		for (UINT unEye = 0; unEye < 2; unEye++)
		{
			// get the description and create the copy texture
			D3DSURFACE_DESC sDescSurfaceD3D9 = {};
			m_psSplitterData->pcTex9Input[unEye]->GetLevelDesc(0, &sDescSurfaceD3D9);

			// copy texture created ?
			if (!m_pcTex9Copy[unEye])
			{
				HRESULT nHr = pcDevice->CreateTexture(sDescSurfaceD3D9.Width, sDescSurfaceD3D9.Height, 1, 0, sDescSurfaceD3D9.Format, D3DPOOL_SYSTEMMEM, &m_pcTex9Copy[unEye], NULL);
				if (!m_pcTex9Copy[unEye])
				{
					DEBUG_HR(L"[STS] Failed to create D3D9 copy texture : ", nHr);
					return;
				}
			}

			// release old texture
			if (m_pcTexCopy11SRV[unEye]) m_pcTexCopy11SRV[unEye]->Release();
			if (m_pcTexCopy11[unEye]) m_pcTexCopy11[unEye]->Release();

			// copy d3d9 texture
			IDirect3DSurface9* pcSurfaceSrc = nullptr;
			IDirect3DSurface9* pcSurfaceDst = nullptr;
			m_psSplitterData->pcTex9Input[unEye]->GetSurfaceLevel(0, &pcSurfaceSrc);
			m_pcTex9Copy[unEye]->GetSurfaceLevel(0, &pcSurfaceDst);
			pcDevice->GetRenderTargetData(pcSurfaceSrc, pcSurfaceDst);
			if (pcSurfaceSrc) pcSurfaceSrc->Release();

			// lock and create d3d11 tex
			D3DLOCKED_RECT sRect = {};
			if (SUCCEEDED(pcSurfaceDst->LockRect(&sRect, NULL, NULL)))
			{
				D3D11_TEXTURE2D_DESC sDesc = {};
				sDesc.Width = sDescSurfaceD3D9.Width;
				sDesc.Height = sDescSurfaceD3D9.Height;
				sDesc.MipLevels = 1;
				sDesc.ArraySize = 1;
				sDesc.Format = GetDXGI_Format(sDescSurfaceD3D9.Format);
				sDesc.SampleDesc.Count = 1;
				sDesc.SampleDesc.Quality = 0;
				sDesc.Usage = D3D11_USAGE_DEFAULT;
				sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				sDesc.CPUAccessFlags = 0;
				sDesc.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA sData = {};
				sData.pSysMem = sRect.pBits;
				sData.SysMemPitch = sRect.Pitch;
				if (FAILED(m_pcD3D11Device->CreateTexture2D(&sDesc, &sData, (ID3D11Texture2D**)&m_pcTexCopy11[unEye])))
				{
					OutputDebugString(L"[CIN] Failed to create copy texture !");
					return;
				}

				pcSurfaceDst->UnlockRect();

				// create shader resource view
				if (m_pcTexCopy11[unEye])
				{
					if (FAILED(m_pcD3D11Device->CreateShaderResourceView(m_pcTexCopy11[unEye], NULL, &m_pcTexCopy11SRV[unEye])))
						OutputDebugString(L"[CIN] Failed to create shader resource view.");
				}
			}

			if (pcSurfaceDst) pcSurfaceDst->Release();
		}

		if ((m_pcD3D11Device) || (m_pcD3D11Context))
		{
			// TODO !! PROVIDE A D3D9 DEVICE HERE
			RenderD3D11(m_pcD3D11Device, m_pcD3D11Context, nullptr);
		}
	}
}

/**
* Inits all stuff (DX11).
***/
void VireioCinema::InitD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext)
{
	// create vertex shader
	if (!m_pcVSGeometry11)
	{
		if (FAILED(CreateVertexShaderTechnique(pcDevice, &m_pcVSGeometry11, &m_pcVLGeometry11, VertexShaderTechnique::PosNormUV)))
			OutputDebugString(L"[CIN] Failed to create vertex shader. !");
	}

	// create screen pixel shader
	if (!m_pcPSGeometry11)
	{
		// create screen pixel shader technique by cinema room setup
		PixelShaderTechnique eTechnique = m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Screen);
		if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPSGeometry11, eTechnique)))
			OutputDebugString(L"[CIN] Failed to create pixel shader. !");
	}

	// create menu screen pixel shader
	if (!m_pcPSMenuScreen11)
	{
		// create menu screen pixel shader technique by cinema room setup
		PixelShaderTechnique eTechnique = PixelShaderTechnique::MenuScreen;
		/*switch (m_sCinemaRoomSetup.ePixelShaderFX_Screen) TODO !!!
		{
		case CinemaRoomSetup::PixelShaderFX_Screen::Screen_GeometryDiffuseTexturedMouse:
		eTechnique = PixelShaderTechnique::GeometryDiffuseTexturedMouse;
		break;
		}*/
		if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPSMenuScreen11, eTechnique)))
			OutputDebugString(L"[CIN] Failed to create pixel shader. !");
	}

	// create render targets
	for (UINT unEye = 0; unEye < 2; unEye++)
	{
		if ((!m_sCinemaData.pcTex11Draw[unEye]) && (m_psTrackerData))
		{
			if (!(m_psTrackerData->sTx.fW) || !(m_psTrackerData->sTx.fH))
			{
				OutputDebugString(L"[CIN] Waiting for tracker to initialize render target size...");
				continue;
			}

			// fill the description
			D3D11_TEXTURE2D_DESC sDescTex;
			sDescTex.Width = (UINT)m_psTrackerData->sTx.fW;
			sDescTex.Height = (UINT)m_psTrackerData->sTx.fH;
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
			pcDevice->CreateTexture2D(&sDescTex, NULL, &m_sCinemaData.pcTex11Draw[unEye]);
			if (m_sCinemaData.pcTex11Draw[unEye])
			{
				// create render target view
				if (FAILED(pcDevice->CreateRenderTargetView(m_sCinemaData.pcTex11Draw[unEye], NULL, &m_sCinemaData.pcTex11DrawRTV[unEye])))
					OutputDebugString(L"[CIN] Failed to create render target view.");

				// create shader resource view
				if (FAILED(pcDevice->CreateShaderResourceView(m_sCinemaData.pcTex11Draw[unEye], NULL, &m_sCinemaData.pcTex11DrawSRV[unEye])))
					OutputDebugString(L"[CIN] Failed to create render target shader resource view.");

				// TODO !! DELETE THIS (BOTH HERE AND IN PRESENTER) set this as private data interface to the shader resource views instead of texture here !!!!
				if ((m_sCinemaData.pcTex11DrawRTV[unEye]) && (m_sCinemaData.pcTex11DrawSRV[unEye]))
				{
					m_sCinemaData.pcTex11DrawSRV[unEye]->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, m_sCinemaData.pcTex11DrawRTV[unEye]);
					m_sCinemaData.pcTex11DrawRTV[unEye]->Release();
				}
			}
			else OutputDebugString(L"[CIN] Failed to create render target !");
		}
	}

	// create the depth stencil
	if ((!m_pcDSGeometry11[0]) && (m_psTrackerData))
	{
		if ((m_psTrackerData->sTx.fW) && (m_psTrackerData->sTx.fH))
		{
			// Create depth stencil textures
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = (UINT)m_psTrackerData->sTx.fW;
			descDepth.Height = (UINT)m_psTrackerData->sTx.fH;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			if (FAILED(pcDevice->CreateTexture2D(&descDepth, NULL, &m_pcDSGeometry11[0])))
				OutputDebugString(L"[CIN] Failed to create depth stencil.");
			if (FAILED(pcDevice->CreateTexture2D(&descDepth, NULL, &m_pcDSGeometry11[1])))
				OutputDebugString(L"[CIN] Failed to create depth stencil.");

			// Create the depth stencil views
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			if (FAILED(pcDevice->CreateDepthStencilView(m_pcDSGeometry11[0], &descDSV, &m_pcDSVGeometry11[0])))
				OutputDebugString(L"[CIN] Failed to create depth stencil view.");
			if (FAILED(pcDevice->CreateDepthStencilView(m_pcDSGeometry11[1], &descDSV, &m_pcDSVGeometry11[1])))
				OutputDebugString(L"[CIN] Failed to create depth stencil view.");

			float fAspect = (float)descDepth.Width / (float)descDepth.Height;

			// create left/right matrices
			for (INT nEye = 0; nEye < 2; nEye++)
			{
				// get the projection matrix for each eye
				D3DXMatrixPerspectiveFovLH(&m_sProj[nEye], (float)D3DXToRadian(116.0), 1.0f, 0.1f, 30.0f);

				// create eye pose matrix... 
				if (nEye)
					D3DXMatrixTranslation(&m_sToEye[nEye], -m_sImmersiveFullscreenSettings.fIPD / 2.0f, 0, 0);
				else
					D3DXMatrixTranslation(&m_sToEye[nEye], m_sImmersiveFullscreenSettings.fIPD / 2.0f, 0, 0);
			}
		}
	}

	if (!m_pcSampler11)
	{
		// Create the sampler state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(pcDevice->CreateSamplerState(&sampDesc, &m_pcSampler11)))
			OutputDebugString(L"[CIN] Failed to create sampler.");
	}

	if (!m_pcSamplerState)
	{
		// Create the sampler state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(pcDevice->CreateSamplerState(&sampDesc, &m_pcSamplerState)))
			OutputDebugString(L"[CIN] Failed to create sampler.");
	}

	// create blend state
	if (!m_pcBlendState)
	{
		// create a blend state for alpha blending
		D3D11_BLEND_DESC sBlendDesc;
		ZeroMemory(&sBlendDesc, sizeof(D3D11_BLEND_DESC));

		sBlendDesc.RenderTarget[0].BlendEnable = TRUE;
		sBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		sBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		sBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		sBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		sBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		sBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		pcDevice->CreateBlendState(&sBlendDesc, &m_pcBlendState);
	}

	// create constant buffer
	if (!m_pcConstantBufferGeometry)
	{
		if (FAILED(CreateGeometryConstantBuffer(pcDevice, &m_pcConstantBufferGeometry, (UINT)sizeof(GeometryConstantBuffer))))
			OutputDebugString(L"[CIN] Failed to create constant buffer.");
	}

	// create menu texture
	if (!m_sCinemaData.pcTexMenu)
	{
		// Initialize the render target texture description
		D3D11_TEXTURE2D_DESC sDesc = {};
		sDesc.Width = 1024;
		sDesc.Height = 1024;
		sDesc.MipLevels = 1;
		sDesc.ArraySize = 1;
		sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sDesc.SampleDesc.Count = 1;
		sDesc.Usage = D3D11_USAGE_DEFAULT;
		sDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		sDesc.CPUAccessFlags = 0;
		sDesc.MiscFlags = 0;

		// Create the render target texture.
		HRESULT nHr = pcDevice->CreateTexture2D(&sDesc, NULL, &m_sCinemaData.pcTexMenu);
		if (FAILED(nHr))
		{
			OutputDebugString(L"[CIN] Failed to create menu texture !");
			return;
		}

		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC sDescRTV = {};
		sDescRTV.Format = sDesc.Format;
		sDescRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		sDescRTV.Texture2D.MipSlice = 0;

		// Create the render target view.
		nHr = pcDevice->CreateRenderTargetView(m_sCinemaData.pcTexMenu, &sDescRTV, &m_sCinemaData.pcTexMenuRTV);
		if (FAILED(nHr))
		{
			OutputDebugString(L"[CIN] Failed to create menu texture RTV !");
			return;
		}

		// Setup the description of the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC sDescSRV = {};
		sDescSRV.Format = sDesc.Format;
		sDescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sDescSRV.Texture2D.MostDetailedMip = 0;
		sDescSRV.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		nHr = pcDevice->CreateShaderResourceView(m_sCinemaData.pcTexMenu, &sDescSRV, &m_sCinemaData.pcTexMenuSRV);
		if (FAILED(nHr))
		{
			OutputDebugString(L"[CIN] Failed to create menu texture SRV !");
			return;
		}
	}

	// is all created ? only create render models if all things are created
	if ((!m_pcConstantBufferGeometry) || (!m_pcSampler11) || (!m_pcSamplerState) || (!m_pcDSGeometry11[0]) || (!m_sCinemaData.pcTex11Draw[0]) || (!m_pcPSGeometry11) || (!m_pcVSGeometry11) || (!m_sCinemaData.pcTexMenuRTV)) return;

	// create render models...
	if (!m_asRenderModels.size())
	{
		// set room size by default screen depth (3m) (+10cm)
		float fSizeScale = 3.1f;
#pragma region cinema screen
		if (true)
		{
			// set vertices
			TexturedNormalVertex asVertices[] =
			{
				{ D3DXVECTOR3(1.92f, -1.08f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-1.92f, -1.08f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
				{ D3DXVECTOR3(-1.92f, 1.08f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(1.92f, 1.08f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) }
			};

			// set indices
			WORD aunIndices[] = { 0, 1, 3, 1, 2, 3 };

			// create the model for immersive mode
			AddRenderModelD3D11(pcDevice, nullptr, nullptr, asVertices, aunIndices, 4, 2, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1920, 1080);

			// and create the model for the gaming room... default screen width 6.0 m
			float fScale = 6.0f / 3.84f;
			D3DXVECTOR3 sScale = D3DXVECTOR3(fScale, fScale, fScale);
			m_unScreenModelIndex = (UINT)m_asRenderModels.size();
			AddRenderModelD3D11(pcDevice, nullptr, nullptr, asVertices, aunIndices, 4, 2, sScale, D3DXVECTOR3(0.0f, 2.0f, 0.0f), 1920, 1080);
		}
#pragma endregion
#pragma region floor top/bottom
		if (true)
		{
			// set vertices
			TexturedNormalVertex asVertices[] =
			{
				{ D3DXVECTOR3(-2.0f, 0.0f, -2.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(2.0f, 0.0f, -2.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
				{ D3DXVECTOR3(2.0f, 0.0f, 2.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(-2.0f, 0.0f, 2.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) }
			};

			// set indices
			WORD aunIndices[] = { 3, 1, 0, 3, 2, 1 };

			// create bottom floor effect by cinema room setup
			PixelShaderTechnique eTechnique = m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Floor[1]);
			ID3D11PixelShader* pcEffect = nullptr;
			CreatePixelShaderEffect(pcDevice, &pcEffect, eTechnique);

			// and create the model... scale here by the screen depth to ensure the screen is always seen
			float fScale = fSizeScale / 2.0f;
			D3DXVECTOR3 sScale = D3DXVECTOR3(fScale, fScale, fScale);
			m_unFloorModelIndex = (UINT)m_asRenderModels.size();
			AddRenderModelD3D11(pcDevice, nullptr, pcEffect, asVertices, aunIndices, 4, 2, sScale, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// set indices top
			WORD aunIndices_top[] = { 0, 1, 3, 1, 2, 3 };

			// negate normals
			for (UINT unI = 0; unI < 4; unI++) asVertices[unI].sNormal *= -1.0f;

			// create top floor effect by cinema room setup
			PixelShaderTechnique eTechnique_top = m_sCinemaRoomSetup.GetTechnique(m_sCinemaRoomSetup.ePixelShaderFX_Floor[0]);
			ID3D11PixelShader* pcEffect_top = nullptr;
			CreatePixelShaderEffect(pcDevice, &pcEffect_top, eTechnique_top);

			// and create the model... no more scale here since already scaled by first call
			m_unCeilingModelIndex = (UINT)m_asRenderModels.size();
			AddRenderModelD3D11(pcDevice, nullptr, pcEffect_top, asVertices, aunIndices_top, 4, 2, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 4.8f, 0.0f));

		}
#pragma endregion
#pragma region walls
		if (true)
		{
			// set vertices
			TexturedNormalVertex asVertices[] =
			{
				{ D3DXVECTOR3(-1.0f, -0.6f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(-1.0f, -0.6f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
				{ D3DXVECTOR3(-1.0f, 0.6f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-1.0f, 0.6f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

				{ D3DXVECTOR3(1.0f, -0.6f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, -0.6f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, 0.6f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(1.0f, 0.6f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },

				{ D3DXVECTOR3(-1.0f, -0.6f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, -0.6f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, 0.6f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-1.0f, 0.6f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },

				{ D3DXVECTOR3(-1.0f, -0.6f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, -0.6f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, 0.6f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-1.0f, 0.6f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
			};

			// set indices
			WORD aunIndices[] =
			{
				0, 1, 3,
				3, 1, 2,

				1, 0, 2, // 5, 4, 6,
				2, 0, 3, // 6, 4, 7,

				0, 1, 3, // 8, 9, 11,
				3, 1, 2, // 11, 9, 10,

				1, 0, 2, // 13, 12, 14,
				2, 0, 3, // 14, 12, 15,
			};

			for (UINT unI = 0; unI < 4; unI++)
			{
				// get setting
				int eEffect = 0;
				switch (unI)
				{
				case 0:
					eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0];
					m_unWallLModelIndex = (UINT)m_asRenderModels.size();
					break;
				case 1:
					eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1];
					m_unWallRModelIndex = (UINT)m_asRenderModels.size();
					break;
				case 2:
					eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0];
					m_unWallFModelIndex = (UINT)m_asRenderModels.size();
					break;
				case 3:
					eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1];
					m_unWallBModelIndex = (UINT)m_asRenderModels.size();
					break;
				}

				// create wall effect by cinema room setup
				PixelShaderTechnique eTechnique = PixelShaderTechnique::Worley01;
				switch (eEffect)
				{
				case CinemaRoomSetup::Wall_Bubbles:
					eTechnique = PixelShaderTechnique::Bubbles;
					break;
				case VireioCinema::CinemaRoomSetup::Wall_Planets:
					eTechnique = PixelShaderTechnique::Planets;
					break;
				case CinemaRoomSetup::Wall_StringTheory:
					eTechnique = PixelShaderTechnique::StringTheory;
					break;
				case CinemaRoomSetup::Wall_ToonCloud:
					eTechnique = PixelShaderTechnique::ToonCloud;
					break;
				case CinemaRoomSetup::Wall_WaterCaustic:
					eTechnique = PixelShaderTechnique::WaterCaustic;
					break;
				case CinemaRoomSetup::Wall_Worley01:
					eTechnique = PixelShaderTechnique::Worley01;
				case VireioCinema::CinemaRoomSetup::Wall_VoronoiSmooth:
					eTechnique = PixelShaderTechnique::VoronoiSmooth;
					break;
				}
				ID3D11PixelShader* pcEffect = nullptr;
				CreatePixelShaderEffect(pcDevice, &pcEffect, eTechnique);

				// and create the model... scale here by the (default) screen depth to ensure the screen is seen
				float fHeight = 600.0f;  // = (4.8f * 1200.0f) / 4.8f;
				float fWidth = (fSizeScale * 1000.0f) / 4.0f;

				AddRenderModelD3D11(pcDevice, nullptr, pcEffect, &asVertices[unI * 4], &aunIndices[unI * 6], 4, 2,
					D3DXVECTOR3(fSizeScale, 4.0f, fSizeScale),
					D3DXVECTOR3(0.0f, 2.4f, 0.0f), (UINT)fWidth, (UINT)fHeight);
			}
		}
#pragma endregion
#pragma region left/right hand
		if (false) // code added for possible future use
		{
			const float X = 0.525731f;
			const float Z = 0.850651f;

			// create vertices, normals are vertex positions
			TexturedNormalVertex asVertices[12] =
			{
				{ D3DXVECTOR3(-X, 0.0f, Z), D3DXVECTOR3(-X, 0.0f, Z), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(X, 0.0f, Z), D3DXVECTOR3(X, 0.0f, Z), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(X, 0.0f, -Z), D3DXVECTOR3(X, 0.0f, -Z), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(0.0f, Z, X), D3DXVECTOR3(0.0f, Z, X), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(0.0f, Z, -X), D3DXVECTOR3(0.0f, Z, -X), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(0.0f, -Z, X), D3DXVECTOR3(0.0f, -Z, X), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(0.0f, -Z, -X), D3DXVECTOR3(0.0f, -Z, -X), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(Z, X, 0.0f), D3DXVECTOR3(Z, X, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-Z, X, 0.0f), D3DXVECTOR3(-Z, X, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(Z, -X, 0.0f), D3DXVECTOR3(Z, -X, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-Z, -X, 0.0f), D3DXVECTOR3(-Z, -X, 0.0f), D3DXVECTOR2(1.0f, 1.0f) }
			};

			// normalize normals
			for (UINT unIx = 0; unIx < 12; unIx++)
			{
				D3DXVECTOR3 sCopy = {};
				D3DXVec3Normalize(&sCopy, &asVertices[12].sNormal);
				asVertices[12].sNormal = sCopy;
			}

			// create indices
			WORD aunIndices[60] =
			{
				1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
				1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
				3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
				10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
			};

			// create effect
			PixelShaderTechnique eTechnique = PixelShaderTechnique::GeometryDiffuseTextured;
			ID3D11PixelShader* pcEffect = nullptr;
			CreatePixelShaderEffect(pcDevice, &pcEffect, eTechnique);

			// and create the model...
			// m_unIndex = (UINT)m_asRenderModels.size();
			AddRenderModelD3D11(pcDevice, nullptr, pcEffect, asVertices, aunIndices, 12, 20, D3DXVECTOR3(0.02f, 0.02f, 0.02f), D3DXVECTOR3(0.0f, 0.2f, 0.0f));
		}
#pragma endregion
#pragma region menu screen
		if (true)
		{
			// set vertices
			TexturedNormalVertex asVertices[] =
			{
				{ D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 1.0f) },
				{ D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f) },
				{ D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
				{ D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f) }
			};

			// set indices
			WORD aunIndices[] = { 0, 1, 3, 1, 2, 3 };

			m_unMenuModelIndex = (UINT)m_asRenderModels.size();
			AddRenderModelD3D11(pcDevice, nullptr, nullptr, asVertices, aunIndices, 4, 2, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		}
#pragma endregion

	}

	// create constant shader constants..
	D3DXVECTOR4 sLightDir = m_sCinemaRoomSetup.sLightDirection;
	m_sGeometryConstants.sLightDir = sLightDir;
	m_sGeometryConstants.sLightAmbient = m_sCinemaRoomSetup.sColorAmbient;
	m_sGeometryConstants.sLightDiffuse = m_sCinemaRoomSetup.sColorDiffuse;
	m_sGeometryConstants.fGamma = m_sCinemaRoomSetup.fGamma;

	// for aspect ratio based fx we set a 1.0 ratio here
	m_sGeometryConstants.sResolution.x = 1024.0f;
	m_sGeometryConstants.sResolution.y = 1024.0f;

	OutputDebugString(L"~InitD3D11");
}

/**
* Renders the virtual cinema (DX11).
***/
void VireioCinema::RenderD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain)
{
	switch (m_eD3DVersion)
	{
	case VireioCinema::D3D_Undefined:
		return;
	case VireioCinema::D3D_9:
		if ((m_pcTexCopy11SRV[0]) && (m_pcTexCopy11SRV[1]))
		{
			// set srv
			m_apcTex11InputSRV[0] = m_pcTexCopy11SRV[0];
			m_apcTex11InputSRV[1] = m_pcTexCopy11SRV[1];
		}
		else
		{
			DEBUG_HEX(m_pcTexCopy11SRV[0]);
			DEBUG_HEX(m_pcTexCopy11SRV[1]);
			return;
		}
	case VireioCinema::D3D_10:
		break;
	case VireioCinema::D3D_11:
		// no splitter data ? set to mono output
		if (!m_psSplitterData)
		{
			// get back buffer
			ID3D11Texture2D* pcBackBuffer = NULL;
			pcSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pcBackBuffer);
			if (pcBackBuffer)
			{
				if (m_pcBackBufferCopy)
				{
					pcContext->CopyResource(m_pcBackBufferCopy, pcBackBuffer);
				}
				else
				{
					// create back buffer copy texture
					D3D11_TEXTURE2D_DESC sDesc;
					pcBackBuffer->GetDesc(&sDesc);
					sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					if (SUCCEEDED(pcDevice->CreateTexture2D(&sDesc, NULL, &m_pcBackBufferCopy)))
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC sDescSR11 = {};
						sDescSR11.Format = sDesc.Format;
						sDescSR11.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						sDescSR11.Texture2D.MipLevels = 1;
						sDescSR11.Texture2D.MostDetailedMip = 0;
						if (FAILED(((ID3D11Device*)pcDevice)->CreateShaderResourceView(m_pcBackBufferCopy, &sDescSR11, &m_pcBackBufferCopySR)))
						{
							OutputDebugString(L"[CIN] Failed to create sr view !");
							m_pcBackBufferCopy->Release(); m_pcBackBufferCopy = nullptr;
							pcBackBuffer->Release();
							return;
						}
					}
				}

				// set left/right mono and instantly release
				m_apcTex11InputSRV[0] = m_pcBackBufferCopySR;
				m_apcTex11InputSRV[1] = m_pcBackBufferCopySR;

				pcBackBuffer->Release();
			}
			else
			{
				OutputDebugString(L"[CIN] No back buffer !!");
				return;
			}
		}
		else
		{
			// set stereo input textures
			m_apcTex11InputSRV[0] = m_psSplitterData->pcTex11InputSRV[0];
			m_apcTex11InputSRV[1] = m_psSplitterData->pcTex11InputSRV[1];
		};
		break;
	default:
		break;
	}


	// backup all states
	D3DX11_STATE_BLOCK sStateBlock;
	CreateStateblock(pcContext, &sStateBlock);

	// clear all states, set targets
	ClearContextState(pcContext);

	// set or create a default rasterizer state here
	if (!m_pcRS)
	{
		D3D11_RASTERIZER_DESC sDesc;
		sDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		sDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		sDesc.FrontCounterClockwise = FALSE;
		sDesc.DepthBias = 0;
		sDesc.SlopeScaledDepthBias = 0.0f;
		sDesc.DepthBiasClamp = 0.0f;
		sDesc.DepthClipEnable = TRUE;
		sDesc.ScissorEnable = FALSE;
		sDesc.MultisampleEnable = FALSE;
		sDesc.AntialiasedLineEnable = FALSE;

		pcDevice->CreateRasterizerState(&sDesc, &m_pcRS);
	}
	pcContext->RSSetState(m_pcRS);

	// set render target viewport
	D3D11_VIEWPORT sViewport = {};
	if (m_psTrackerData)
	{
		sViewport.Width = (FLOAT)m_psTrackerData->sTx.fW;
		sViewport.Height = (FLOAT)m_psTrackerData->sTx.fH;
	}
	else
	{
		// we have standard render target size if no tracker connected
		sViewport.Width = 1024.f;
		sViewport.Height = 1024.f;
	}
	sViewport.MaxDepth = 1.0f;
	pcContext->RSSetViewports(1, &sViewport);

	// Set the input layout, buffers
	pcContext->IASetInputLayout(m_pcVLGeometry11);
	pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferGeometry);
	pcContext->PSSetConstantBuffers(0, 1, &m_pcConstantBufferGeometry);

	// Set primitive topology
	pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set vertex shader... this is a constant shader for all geometry
	pcContext->VSSetShader(m_pcVSGeometry11, NULL, 0);

	// clear render target, depth stencil
	for (int nEye = 0; nEye < 2; nEye++)
	{
		// clear the depth stencil
		pcContext->ClearDepthStencilView(m_pcDSVGeometry11[nEye], D3D11_CLEAR_DEPTH, 1.0f, 0);

		// clear render target
		FLOAT afColorRgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		pcContext->ClearRenderTargetView(m_sCinemaData.pcTex11DrawRTV[nEye], afColorRgba);
	}

	// Initialize the view matrix
	if (m_psTrackerData)
	{
		// get connected
		CopyMemory(&m_sView, &m_psTrackerData->sView, sizeof(D3DMATRIX));
	}
	else
	{
		// TODO !! CREATE VIEW MATRIX HERE BASED ON CONNECTED EULER AND POSITION

		// no view matrix connected, create basic lookat matrix
		D3DXVECTOR3 sEye = D3DXVECTOR3(0.0f, 1.8f, 0.0f);
		D3DXVECTOR3 sAt = D3DXVECTOR3(0.0f, 1.5f, 1.0f);
		D3DXVECTOR3 sUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&m_sView, &sEye, &sAt, &sUp);
	}

	// Update and set our time
	static float fTime = 0.0f;
	static DWORD unTimeStart = 0;
	DWORD unTimeCurrent = GetTickCount();
	if (unTimeStart == 0)
		unTimeStart = unTimeCurrent;
	fTime = (unTimeCurrent - unTimeStart) / 1000.0f;
	m_sGeometryConstants.fGlobalTime = fTime;

	// set mouse position
	static POINT s_sPointOld;
	static DWORD s_unTick = 0;

	// get desktop
	RECT sDesktop;
	HWND pDesktop = GetDesktopWindow();
	GetWindowRect(pDesktop, &sDesktop);

	// get current pos
	POINT sPoint;
	GetCursorPos(&sPoint);

	// mouse not moved ?
	static DWORD unTimeLastFrame = 0;
	if ((sPoint.x == s_sPointOld.x) && (sPoint.y == s_sPointOld.y))
	{
		s_unTick += unTimeCurrent - unTimeLastFrame;
	}
	else s_unTick = 0;
	s_sPointOld = sPoint;
	unTimeLastFrame = unTimeCurrent;

	// set new pos, if not moved set out of view
	if ((s_unTick > m_unMouseTickCount) || (m_unMouseTickCount == 0))
		m_sGeometryConstants.sMouse = D3DXVECTOR4(1.1f, 1.1f, 0.0f, 0.0f);
	else
		m_sGeometryConstants.sMouse = D3DXVECTOR4((float)sPoint.x / (float)sDesktop.right, (float)sPoint.y / (float)sDesktop.bottom, 0.0f, 0.0f);

	// immersive mode toggle
	if (m_sMenu.bOnFullImmersive)
	{
		m_sMenu.bOnFullImmersive = false;
		m_sCinemaRoomSetup.bImmersiveMode = !m_sCinemaRoomSetup.bImmersiveMode;
	}

	// immersive mode ? render fullscreen
	if (m_sCinemaRoomSetup.bImmersiveMode)
	{
		pcContext->PSSetSamplers(0, 1, &m_pcSamplerState);
		RenderFullscreenD3D11(pcDevice, pcContext, pcSwapchain);

		// set back device
		ApplyStateblock(pcContext, &sStateBlock);

		return;
	}
	else
		pcContext->PSSetSamplers(0, 1, &m_pcSampler11);

	// set screen world matrix
	D3DXMATRIX sWorld, sRotateX, sRotateY, sTrans, sScale;
	float fScale = m_sCinemaRoomSetup.fScreenWidth / 6.0f; // default screen width 6.0 m
	D3DXMatrixScaling(&sScale, fScale, fScale, fScale);
	D3DXMatrixRotationX(&sRotateX, (float)D3DXToRadian((double)m_sCinemaRoomSetup.fScreenRotateX));
	D3DXMatrixRotationY(&sRotateY, (float)D3DXToRadian((double)m_sCinemaRoomSetup.fScreenRotateY));
	D3DXMatrixTranslation(&sTrans, 0.0f, m_sCinemaRoomSetup.fScreenLevel, m_sCinemaRoomSetup.fScreenDepth);
	sWorld = sScale * sRotateX * sTrans * sRotateY;

	// buffer data
	UINT stride = sizeof(TexturedNormalVertex);
	UINT offset = 0;

	// loop through available render models, render
	UINT unRenderModelsNo = (UINT)m_asRenderModels.size();
	for (UINT unI = 1; unI < unRenderModelsNo; unI++)
	{
		// performance mode ?
		if ((m_sCinemaRoomSetup.bPerformanceMode) && (unI >= 2) && (unI != m_unMenuModelIndex)) continue;

		// menu ?
		if ((m_unMenuModelIndex == unI) && (!m_sMenu.bIsActive)) continue;

		static D3DXMATRIX s_sViewBackup = {};

		// set model buffers
		pcContext->IASetVertexBuffers(0, 1, &m_asRenderModels[unI].pcVertexBuffer, &stride, &offset);
		pcContext->IASetIndexBuffer(m_asRenderModels[unI].pcIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// set world matrix and render target resolution
		D3DXMatrixTranspose(&m_sGeometryConstants.sWorld, &sWorld);
		m_sGeometryConstants.sResolution.x = (float)m_asRenderModels[unI].sResolution.unWidth;
		m_sGeometryConstants.sResolution.y = (float)m_asRenderModels[unI].sResolution.unHeight;

		// set texture and effect if pixel shader assigned
		if (m_asRenderModels[unI].pcEffect)
		{
			pcContext->OMSetBlendState(NULL, NULL, 0xffffffff);
			pcContext->PSSetShaderResources(0, 1, &m_asRenderModels[unI].pcTextureSRV);
			pcContext->PSSetShader(m_asRenderModels[unI].pcEffect, NULL, 0);
		}
		else
		{
			// set menu or main shader
			if (m_unMenuModelIndex == unI)
			{
				pcContext->OMSetBlendState(m_pcBlendState, NULL, 0xffffffff);
				pcContext->PSSetShader(m_pcPSMenuScreen11, NULL, 0);

				// backup view matrix, we use the view matrix for the menu 
				// screen since the world matrix is used for global scale
				s_sViewBackup = m_sView;

				// setup view matrices
				fScale = m_sCinemaRoomSetup.fMenuScreenScale / m_sCinemaRoomSetup.fRoomScale;
				D3DXMatrixScaling(&sScale, fScale, fScale, fScale);

				// stuck to HMD yaw (+roll) angle ?
				if (m_sCinemaRoomSetup.bMenuIsHUD)
				{
					// rotate x axis for better reading
					D3DXMATRIX sTemp;
					D3DXMatrixRotationX(&sTemp, s_sViewBackup(1, 2));
					m_sView *= sTemp;

					// translate back from default y 2.0f
					D3DXMatrixTranslation(&sTrans, 0.0f, -2.0f, m_sCinemaRoomSetup.fMenuScreenDepth);
					m_sView = sScale * sTrans * sTemp;
				}
				else
				{
					// set view matrix based on view backup
					D3DXMatrixTranslation(&sTrans, 0.0f, 0.0f, m_sCinemaRoomSetup.fMenuScreenDepth);
					D3DXMatrixRotationY(&sRotateY, (float)D3DXToRadian((double)m_sCinemaRoomSetup.fMenuScreenRotateY));
					m_sView = sScale * sTrans * sRotateY * s_sViewBackup;
				}
			}
			else
			{
				pcContext->OMSetBlendState(NULL, NULL, 0xffffffff);
				pcContext->PSSetShader(m_pcPSGeometry11, NULL, 0);
			}
		}

		// left + right
		for (int nEye = 0; nEye < 2; nEye++)
		{
			// set frame texture left/right if main effect
			if (!m_asRenderModels[unI].pcEffect)
			{
				if (m_unMenuModelIndex == unI)
					pcContext->PSSetShaderResources(0, 1, &m_sCinemaData.pcTexMenuSRV);
				else
					pcContext->PSSetShaderResources(0, 1, &m_apcTex11InputSRV[nEye]);
			}

			// set WVP matrix, update constant buffer
			D3DXMATRIX sWorldViewProjection;
			if (m_psTrackerData)
			{
				sWorldViewProjection = sWorld * m_sView * m_psTrackerData->sProj[nEye];
			}
			else sWorldViewProjection = sWorld * m_sView * m_sToEye[nEye] * m_sProj[nEye];

			D3DXMatrixTranspose(&m_sGeometryConstants.sWorldViewProjection, &sWorldViewProjection);
			pcContext->UpdateSubresource(m_pcConstantBufferGeometry, 0, NULL, &m_sGeometryConstants, 0, 0);

			// set render target
			pcContext->OMSetRenderTargets(1, &m_sCinemaData.pcTex11DrawRTV[nEye], m_pcDSVGeometry11[nEye]);

			// draw
			pcContext->DrawIndexed(m_asRenderModels[unI].unTriangleCount * 3, 0, 0);
		}

		// set back view matrix
		if (m_unMenuModelIndex == unI)
			m_sView = s_sViewBackup;
		// set back world matrix after screen got rendered
		else if (unI == 1)
			D3DXMatrixScaling(&sWorld, m_sCinemaRoomSetup.fRoomScale, m_sCinemaRoomSetup.fRoomScale, m_sCinemaRoomSetup.fRoomScale);
	}

	// set back device
	ApplyStateblock(pcContext, &sStateBlock);

#ifdef _DUMMY_RENDER_TEST

	// this is just a dummy render test... to show up all necessary render states

	// backup all states
	ZeroMemory(&sStateBlock, sizeof(D3DX11_STATE_BLOCK));
	CreateStateblock(pcContext, &sStateBlock);

	// clear all states, set targets
	ClearContextState(pcContext);

	// set viewport
	pcContext->RSSetViewports(1, &sViewport);

	{
		static ID3D11VertexShader* s_pcVS = nullptr;
		static ID3D11InputLayout* s_pcVL = nullptr;
		static ID3D11PixelShader* s_pcPS = nullptr;

		// create vertex shader
		if (!s_pcVS)
		{
			static const char* V_Shader =
				"float4 VS(float4 Pos : POSITION) : SV_POSITION\n"
				"{\n"
				"return Pos;\n"
				"}\n";
			ID3D10Blob* pcShader = nullptr;
			if (SUCCEEDED(D3DX10CompileFromMemory(V_Shader, strlen(V_Shader), NULL, NULL, NULL, "VS", "vs_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL)))
			{
				OutputDebugString(L"[TEST!!!!] : Vertex Shader compiled !");
				if (SUCCEEDED(pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &s_pcVS)))
				{
					OutputDebugString(L"[TEST!!!!] : Vertex Shader created !");
					// Define the input layout
					D3D11_INPUT_ELEMENT_DESC layout[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};
					UINT numElements = ARRAYSIZE(layout);

					// Create the input layout
					if (SUCCEEDED(pcDevice->CreateInputLayout(layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &s_pcVL)))
					{
						OutputDebugString(L"[TEST!!!!] : Input Layout created !");
					}
				}
				pcShader->Release();
			}
		}

		// create pixel shader
		if (!s_pcPS)
		{
			static const char* P_Shader =
				"float4 PS(float4 Pos : SV_POSITION) : SV_Target\n"
				"{\n"
				"return float4(1.0f, 1.0f, 0.0f, 1.0f);    // Yellow, with Alpha = 1\n"
				"}\n";
			ID3D10Blob* pcShader = nullptr;
			if (SUCCEEDED(D3DX10CompileFromMemory(P_Shader, strlen(P_Shader), NULL, NULL, NULL, "PS", "ps_4_0", NULL, NULL, NULL, &pcShader, NULL, NULL)))
			{
				OutputDebugString(L"[TEST!!!!] : Pixel Shader compiled !");
				if (SUCCEEDED(pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &s_pcPS)))
					OutputDebugString(L"[TEST!!!!] : Pixel Shader created !");
				pcShader->Release();
			}
		}

		// get private data interface (render target view) for both eyes
		ID3D11RenderTargetView* pcRTV[2] = {};
		UINT dwSize = sizeof(ID3D11RenderTargetView*);
		for (unsigned uEye = 0; uEye < 2; uEye++)
			m_pcTex11DrawSRV[uEye]->GetPrivateData(PDIID_ID3D11TextureXD_RenderTargetView, &dwSize, (void*)&pcRTV[uEye]);

		struct SimpleVertex
		{
			XMFLOAT3 Pos;
		};
		static ID3D11Buffer* s_pVertexBuffer = NULL;
		if (!s_pVertexBuffer)
		{
			// Create vertex buffer
			SimpleVertex vertices[] =
			{
				XMFLOAT3(0.0f, 0.5f, 0.5f),
				XMFLOAT3(0.5f, -0.5f, 0.5f),
				XMFLOAT3(-0.5f, -0.5f, 0.5f),
			};
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * 3;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = vertices;

			if (FAILED(pcDevice->CreateBuffer(&bd, &InitData, &s_pVertexBuffer)))
				OutputDebugString(L"Failed to create vertex buffer !!!!!!");

		}

		pcContext->OMSetRenderTargets(1, &pcRTV[0], NULL);
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pcContext->IASetVertexBuffers(0, 1, &s_pVertexBuffer, &stride, &offset);
		pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pcContext->IASetInputLayout(s_pcVL);

		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		pcContext->ClearRenderTargetView(pcRTV[0], ClearColor);
		pcContext->PSSetSamplers(0, 1, &m_pcSampler11);

		// Render a triangle
		pcContext->VSSetShader(s_pcVS, NULL, 0);
		pcContext->PSSetShader(s_pcPS, NULL, 0);
		pcContext->Draw(3, 0);

		SAFE_RELEASE(pcRTV[1]);
		SAFE_RELEASE(pcRTV[0]);
	}

	// set back device
	ApplyStateblock(pcContext, &sStateBlock);

#endif
}

/**
* Renders the
***/
void VireioCinema::RenderFullscreenD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain)
{
	// set model buffers
	UINT stride = sizeof(TexturedNormalVertex);
	UINT offset = 0;
	pcContext->IASetVertexBuffers(0, 1, &m_asRenderModels[0].pcVertexBuffer, &stride, &offset);
	pcContext->IASetIndexBuffer(m_asRenderModels[0].pcIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// set world matrix and render target resolution
	D3DXMatrixIdentity(&m_sGeometryConstants.sWorld);
	m_sGeometryConstants.sResolution.x = (float)m_asRenderModels[0].sResolution.unWidth;
	m_sGeometryConstants.sResolution.y = (float)m_asRenderModels[0].sResolution.unHeight;

	// set main shader
	pcContext->PSSetShader(m_pcPSGeometry11, NULL, 0);

	// left + right
	for (int nEye = 0; nEye < 2; nEye++)
	{
		// set frame texture left/right 
		pcContext->PSSetShaderResources(0, 1, &m_apcTex11InputSRV[nEye]);

		// create view matrix
		D3DXMATRIX sView;
		D3DXMatrixTranslation(&sView, -m_sToEye[nEye](3, 0), 0.0f, m_sImmersiveFullscreenSettings.fVSD);

		// set WVP matrix, update constant buffer
		D3DXMATRIX sWorldViewProjection;
		if (m_psTrackerData)
		{
			sWorldViewProjection = sView * m_psTrackerData->sProj[nEye];
		}
		else sWorldViewProjection = sView * m_sToEye[nEye] * m_sProj[nEye];

		D3DXMatrixTranspose(&m_sGeometryConstants.sWorldViewProjection, &sWorldViewProjection);
		pcContext->UpdateSubresource(m_pcConstantBufferGeometry, 0, NULL, &m_sGeometryConstants, 0, 0);

		// set render target
		pcContext->OMSetRenderTargets(1, &m_sCinemaData.pcTex11DrawRTV[nEye], m_pcDSVGeometry11[nEye]);

		// draw
		pcContext->DrawIndexed(m_asRenderModels[0].unTriangleCount * 3, 0, 0);
	}

	// return if menu is not active
	if (!m_sMenu.bIsActive) return;

	// set menu parameters
	pcContext->OMSetBlendState(m_pcBlendState, NULL, 0xffffffff);
	pcContext->PSSetShader(m_pcPSMenuScreen11, NULL, 0);
	pcContext->PSSetShaderResources(0, 1, &m_sCinemaData.pcTexMenuSRV);
	m_sGeometryConstants.sResolution.x = (float)m_asRenderModels[m_unMenuModelIndex].sResolution.unWidth;
	m_sGeometryConstants.sResolution.y = (float)m_asRenderModels[m_unMenuModelIndex].sResolution.unHeight;

	// set menu model buffers
	pcContext->IASetVertexBuffers(0, 1, &m_asRenderModels[m_unMenuModelIndex].pcVertexBuffer, &stride, &offset);
	pcContext->IASetIndexBuffer(m_asRenderModels[m_unMenuModelIndex].pcIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// draw menu
	for (int nEye = 0; nEye < 2; nEye++)
	{
		// set view matrix based on view backup..translate back from default y 2.0f...scale to 0.5f
		D3DXMATRIX sTrans;
		D3DXMatrixTranslation(&sTrans, 0.0f, -2.0f, 0.0f);
		D3DXMATRIX sScale;
		D3DXMatrixScaling(&sScale, 0.5f, 0.5f, 0.5f);
		D3DXMATRIX sView;
		D3DXMatrixTranslation(&sView, -m_sToEye[nEye](3, 0), 0.0f, m_sImmersiveFullscreenSettings.fVSD * 0.9f);
		D3DXMATRIX sWorldViewProjection;
		if (m_psTrackerData)
		{
			sWorldViewProjection = sTrans * sScale * sView * m_psTrackerData->sProj[nEye];
		}
		else sWorldViewProjection = sTrans * sScale * sView * m_sToEye[nEye] * m_sProj[nEye];

		D3DXMatrixTranspose(&m_sGeometryConstants.sWorldViewProjection, &sWorldViewProjection);
		pcContext->UpdateSubresource(m_pcConstantBufferGeometry, 0, NULL, &m_sGeometryConstants, 0, 0);

		// set render target
		pcContext->OMSetRenderTargets(1, &m_sCinemaData.pcTex11DrawRTV[nEye], m_pcDSVGeometry11[nEye]);

		// and draw
		pcContext->DrawIndexed(m_asRenderModels[m_unMenuModelIndex].unTriangleCount * 3, 0, 0);
	}

	// set back blend state
	pcContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	pcContext->PSSetShader(m_pcPSGeometry11, NULL, 0);
}

/**
* Creates a simple render model based on mesh data and adds it to the input vector.
***/
void VireioCinema::AddRenderModelD3D11(ID3D11Device* pcDevice, ID3D11Texture2D* pcTexture, ID3D11PixelShader* pcEffect, TexturedNormalVertex* asVertices, WORD* aunIndices, UINT32 unVertexCount, UINT32 unTriangleCount, D3DXVECTOR3 sScale, D3DXVECTOR3 sTranslate, UINT32 unWidth, UINT32 unHeight)
{
	// create a D3D render model structure
	RenderModel_D3D11 sRenderModel = {};

	for (int i = 0; i < (int)unVertexCount; i++)
	{
		// scale and translate
		asVertices[i].sPosition.x *= sScale.x;
		asVertices[i].sPosition.y *= sScale.y;
		asVertices[i].sPosition.z *= sScale.z;

		asVertices[i].sPosition.x += sTranslate.x;
		asVertices[i].sPosition.y += sTranslate.y;
		asVertices[i].sPosition.z += sTranslate.z;
	}

	// Create vertex buffer
	D3D11_BUFFER_DESC sVertexBufferDesc;
	ZeroMemory(&sVertexBufferDesc, sizeof(sVertexBufferDesc));
	sVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	sVertexBufferDesc.ByteWidth = sizeof(TexturedNormalVertex) * unVertexCount;
	sVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	sVertexBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sInitData;
	ZeroMemory(&sInitData, sizeof(sInitData));
	sInitData.pSysMem = asVertices;
	if (FAILED(pcDevice->CreateBuffer(&sVertexBufferDesc, &sInitData, &sRenderModel.pcVertexBuffer)))
		OutputDebugString(L"[CIN] Failed to create vertex buffer.");

	// create index buffer
	D3D11_BUFFER_DESC sIndexBufferDesc;
	ZeroMemory(&sIndexBufferDesc, sizeof(sIndexBufferDesc));
	sIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	sIndexBufferDesc.ByteWidth = sizeof(WORD) * unTriangleCount * 3;
	sIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	sIndexBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&sInitData, sizeof(sInitData));
	sInitData.pSysMem = aunIndices;
	if (FAILED(pcDevice->CreateBuffer(&sIndexBufferDesc, &sInitData, &sRenderModel.pcIndexBuffer)))
		OutputDebugString(L"[CIN] Failed to create index buffer.");

	// set vertices/triangle count
	sRenderModel.unTriangleCount = unTriangleCount;
	sRenderModel.unVertexCount = unVertexCount;

	// set effect, texture and create shader resource view
	sRenderModel.pcEffect = pcEffect;
	sRenderModel.pcTexture = pcTexture;
	if (sRenderModel.pcTexture)
	{
		// get texture description
		D3D11_TEXTURE2D_DESC sDescTex;
		pcTexture->GetDesc(&sDescTex);

		// create texture shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(sDesc));
		sDesc.Format = sDescTex.Format;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.Texture2D.MipLevels = 1;

		if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)sRenderModel.pcTexture, &sDesc, &sRenderModel.pcTextureSRV))))
			OutputDebugString(L"[CIN] Failed to create model texture shader resource view!");
	}

	// set resolution
	sRenderModel.sResolution.unWidth = unWidth;
	sRenderModel.sResolution.unHeight = unHeight;

	// and add to vector
	m_asRenderModels.push_back(sRenderModel);
}

/**
* Creates a D3D11 device to be used in D3D9/D3D10 games.
***/
HRESULT VireioCinema::CreateD3D11Device(bool bCreateSwapChain)
{
	// create a dummy window
	static const wchar_t* szClassname = L"dummy class";
	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = DefWindowProc;
	wx.hInstance = GetModuleHandle(NULL);
	wx.lpszClassName = szClassname;
	if (RegisterClassEx(&wx))
	{
		m_hDummy = CreateWindowEx(0, szClassname, L"dummy", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	}

	// get factory / adapter
	IDXGIFactory* pcDXGIFactory;
	IDXGIAdapter* pcAdapter;
	HRESULT unHR;
	if (FAILED(unHR = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&pcDXGIFactory))))
		return unHR;
	if (FAILED(unHR = pcDXGIFactory->EnumAdapters(0, &pcAdapter)))
		return unHR;

	// create the DX11 device
	{
		if (bCreateSwapChain)
		{
			// get render target dimension if hmd node connected
			UINT unWidth = 1024;
			UINT unHeight = 1024;
			if (m_psTrackerData)
			{
				if (m_psTrackerData->sTx.fW) unWidth = (UINT)m_psTrackerData->sTx.fW;
				if (m_psTrackerData->sTx.fH) unHeight = (UINT)m_psTrackerData->sTx.fH;
			}

			// fill swap chain description
			DXGI_SWAP_CHAIN_DESC sDesc = {};
			ZeroMemory(&sDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			sDesc.BufferCount = 1;
			sDesc.BufferDesc.Width = unWidth;
			sDesc.BufferDesc.Height = unHeight;
			sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sDesc.BufferDesc.RefreshRate.Numerator = 90;
			sDesc.BufferDesc.RefreshRate.Denominator = 1;
			sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sDesc.OutputWindow = m_hDummy;
			sDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			sDesc.Windowed = true;
			sDesc.SampleDesc.Count = 1;
			sDesc.SampleDesc.Quality = 0;

			sDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// create with swapchain
			IDXGISwapChain* pcSwapChain = nullptr;
			unHR = D3D11CreateDeviceAndSwapChain(pcAdapter, pcAdapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
				NULL, 0, D3D11_SDK_VERSION, &sDesc, &pcSwapChain, &m_pcD3D11Device,
				NULL, &m_pcD3D11Context);

			if ((m_pcD3D11Device) && (pcSwapChain))
			{
				// set the swapchain as private data interface to the device
				m_pcD3D11Device->SetPrivateDataInterface(PDIID_ID3D11Device_IDXGISwapChain, pcSwapChain);
				pcSwapChain->Release();
			}
			else OutputDebugString(L"[CIN] Failed to create d3d11 device or/and swapchain.");
		}
		else
		{
			// create without swapchain
			if (FAILED(unHR = D3D11CreateDevice(pcAdapter, pcAdapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
				NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pcD3D11Device, NULL, &m_pcD3D11Context)))
				return unHR;
		}

		// Set max frame latency to 1
		IDXGIDevice1* pcDXGIDevice1 = NULL;
		unHR = m_pcD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pcDXGIDevice1);
		if (FAILED(unHR)) return unHR;
		pcDXGIDevice1->SetMaximumFrameLatency(1);
		pcDXGIDevice1->Release();
		return unHR;
	}
}

/**
* Saves all settings to the VireioPerception.ini file.
***/
void VireioCinema::SaveIniSettings()
{
	// get file path
	char szFilePathINI[1024];
	GetCurrentDirectoryA(1024, szFilePathINI);
	strcat_s(szFilePathINI, "\\VireioPerception.ini");
	bool bFileExists = false;

	// write to ini file
	GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.a, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.a", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.r, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.r", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.g, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.g", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorAmbient.b, "Stereo Cinema", "sCinemaRoomSetup.sColorAmbient.b", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.a, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.a", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.r, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.r", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.g, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.g", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sColorDiffuse.b, "Stereo Cinema", "sCinemaRoomSetup.sColorDiffuse.b", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.w, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.w", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.x, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.x", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.y, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.y", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.sLightDirection.z, "Stereo Cinema", "sCinemaRoomSetup.sLightDirection.z", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Screen, "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Screen.b", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0]", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1]", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_LR[0]", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1]", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Floor[0], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Floor[0]", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.ePixelShaderFX_Floor[1], "Stereo Cinema", "sCinemaRoomSetup.ePixelShaderFX_Floor[1]", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fScreenWidth, "Stereo Cinema", "sCinemaRoomSetup.fScreenWidth", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fScreenLevel, "Stereo Cinema", "sCinemaRoomSetup.fScreenLevel", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fScreenDepth, "Stereo Cinema", "sCinemaRoomSetup.fScreenDepth", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fScreenRotateX, "Stereo Cinema", "sCinemaRoomSetup.fScreenRotateX", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fScreenRotateY, "Stereo Cinema", "sCinemaRoomSetup.fScreenRotateY", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenDepth, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenDepth", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenScale, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenScale", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fMenuScreenRotateY, "Stereo Cinema", "sCinemaRoomSetup.fMenuScreenRotateY", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fRoomScale, "Stereo Cinema", "sCinemaRoomSetup.fRoomScale", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bMenuIsHUD, "Stereo Cinema", "sCinemaRoomSetup.bMenuIsHUD", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bPerformanceMode, "Stereo Cinema", "sCinemaRoomSetup.bPerformanceMode", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bImmersiveMode, "Stereo Cinema", "sCinemaRoomSetup.bImmersiveMode", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sCinemaRoomSetup.fGamma, "Stereo Cinema", "sCinemaRoomSetup.fGamma", szFilePathINI, bFileExists);
	GetIniFileSetting((DWORD)m_unMouseTickCount, "Stereo Cinema", "unMouseTickCount", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sImmersiveFullscreenSettings.fIPD, "Stereo Presenter", "fIPD", szFilePathINI, bFileExists);
	GetIniFileSetting(m_sImmersiveFullscreenSettings.fVSD, "Stereo Presenter", "fVSD", szFilePathINI, bFileExists);
}

