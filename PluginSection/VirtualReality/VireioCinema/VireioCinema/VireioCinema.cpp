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
m_vsCinema(nullptr),
m_vsCinemaTheatre(nullptr),
m_psCinema(nullptr),
m_psCinemaTheatre(nullptr),
m_ctPCinema(nullptr),
m_ctPCinemaTheatre(nullptr),
m_ctVCinema(nullptr),
m_ctVCinemaTheatre(nullptr),
m_pVBCinemaScreen(nullptr),
m_pVBCinemaTheatre(nullptr),
m_pIBCinemaTheatre(nullptr),
m_pTextureCinema(nullptr),
m_pTextureCinemaTheatre(nullptr),
m_pcVSGeometry11(nullptr),
m_pcVLGeometry11(nullptr),
m_pcPSGeometry11(nullptr),
m_pfYaw(nullptr),
m_pfPitch(nullptr),
m_pfRoll(nullptr),
m_fYaw(0.0f),
m_fPitch(0.0f),
m_fRoll(0.0f),
m_pfPositionX(nullptr),
m_pfPositionY(nullptr),
m_pfPositionZ(nullptr),
m_ppcFrameTexture(nullptr),
m_pStereoOutputLeft(nullptr),
m_pStereoOutputRight(nullptr),
m_pStereoOutputSurfaceLeft(nullptr),
m_pStereoOutputSurfaceRight(nullptr),
m_pcDepthStencilLeft(nullptr),
m_pcDepthStencilRight(nullptr),
m_asRenderModels(),
m_punTexResolutionHeight(nullptr),
m_punTexResolutionWidth(nullptr),
m_pcBackBufferCopy(nullptr),
m_pcBackBufferCopySR(nullptr),
m_pcRS(nullptr),
m_pbImmersiveMode(nullptr)
{
	ZeroMemory(&m_sPositionVector, sizeof(D3DVECTOR));
	ZeroMemory(&m_sGeometryConstants, sizeof(GeometryConstantBuffer));

	// set default aspect ratio
	m_fAspectRatio = 1920.0f / 1080.0f;

	m_pcConstantBufferGeometry = nullptr;
	m_pcSampler11 = nullptr;
	m_pcDSGeometry11[0] = nullptr;
	m_pcDSGeometry11[1] = nullptr;
	m_pcDSVGeometry11[0] = nullptr;
	m_pcDSVGeometry11[1] = nullptr;
	m_pcTex11Draw[0] = nullptr;
	m_pcTex11Draw[1] = nullptr;
	m_pcTex11DrawRTV[0] = nullptr;
	m_pcTex11DrawRTV[1] = nullptr;
	m_pcTex11DrawSRV[0] = nullptr;
	m_pcTex11DrawSRV[1] = nullptr;
	m_ppcTex11InputSRV[0] = nullptr;
	m_ppcTex11InputSRV[1] = nullptr;
	m_apcTex11InputSRV[0] = nullptr;
	m_apcTex11InputSRV[1] = nullptr;
	m_psView = nullptr;
	m_psProjection[0] = nullptr;
	m_psProjection[1] = nullptr;

	m_pcVertexShader11 = nullptr;
	m_pcPixelShader11 = nullptr;
	m_pcVertexLayout11 = nullptr;
	m_pcVertexBuffer11 = nullptr;
	m_pcConstantBufferDirect11 = nullptr;
	m_pcSamplerState = nullptr;

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
	m_sCinemaRoomSetup.fScreenLevel = 2.0f; /**< default : 2 meters height level */
	m_sCinemaRoomSetup.fScreenDepth = 3.0f; /**< default : 3 meters depth level */
	m_sCinemaRoomSetup.bPerformanceMode = FALSE;
	m_sCinemaRoomSetup.bImmersiveMode = FALSE;
	m_unMouseTickCount = 2000;
	m_sImmersiveFullscreenSettings.fIPD = 0.064f;
	m_sImmersiveFullscreenSettings.fVSD = 0.05f;

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
	m_sCinemaRoomSetup.bPerformanceMode = (BOOL)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bPerformanceMode, "Stereo Cinema", "sCinemaRoomSetup.bPerformanceMode", szFilePathINI, bFileExists);
	m_sCinemaRoomSetup.bImmersiveMode = (BOOL)GetIniFileSetting((DWORD)m_sCinemaRoomSetup.bImmersiveMode, "Stereo Cinema", "sCinemaRoomSetup.bImmersiveMode", szFilePathINI, bFileExists);
	m_unMouseTickCount = GetIniFileSetting((DWORD)m_unMouseTickCount, "Stereo Cinema", "unMouseTickCount", szFilePathINI, bFileExists);
	m_sImmersiveFullscreenSettings.fIPD = GetIniFileSetting(m_sImmersiveFullscreenSettings.fIPD, "Stereo Presenter", "fIPD", szFilePathINI, bFileExists);
	m_sImmersiveFullscreenSettings.fVSD = GetIniFileSetting(m_sImmersiveFullscreenSettings.fVSD, "Stereo Presenter", "fVSD", szFilePathINI, bFileExists);

}

/**
* Destructor.
***/
VireioCinema::~VireioCinema()
{
	if (m_pStereoOutputSurfaceLeft)
	{
		m_pStereoOutputSurfaceLeft->Release();
		m_pStereoOutputSurfaceLeft = nullptr;
	}
	if (m_pStereoOutputSurfaceRight)
	{
		m_pStereoOutputSurfaceRight->Release();
		m_pStereoOutputSurfaceRight = nullptr;
	}
	if (m_pStereoOutputLeft)
	{
		m_pStereoOutputLeft->Release();
		m_pStereoOutputLeft = nullptr;
	}
	if (m_pStereoOutputRight)
	{
		m_pStereoOutputRight->Release();
		m_pStereoOutputRight = nullptr;
	}

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
	SAFE_RELEASE(m_pcTex11Draw[0]);
	SAFE_RELEASE(m_pcTex11Draw[1]);
	SAFE_RELEASE(m_pcTex11DrawRTV[0]);
	SAFE_RELEASE(m_pcTex11DrawRTV[1]);
	SAFE_RELEASE(m_pcTex11DrawSRV[0]);
	SAFE_RELEASE(m_pcTex11DrawSRV[1]);
	SAFE_RELEASE(m_pcConstantBufferGeometry);
	SAFE_RELEASE(m_pcDSVGeometry11[0]);
	SAFE_RELEASE(m_pcDSVGeometry11[1]);
	SAFE_RELEASE(m_pcDSGeometry11[0]);
	SAFE_RELEASE(m_pcDSGeometry11[1]);
	SAFE_RELEASE(m_pcVLGeometry11);
	SAFE_RELEASE(m_pcVSGeometry11);
	SAFE_RELEASE(m_pcPSGeometry11);
	SAFE_RELEASE(m_pcVertexShader11);
	SAFE_RELEASE(m_pcPixelShader11);
	SAFE_RELEASE(m_pcVertexLayout11);
	SAFE_RELEASE(m_pcVertexBuffer11);
	SAFE_RELEASE(m_pcConstantBufferDirect11);
	SAFE_RELEASE(m_pcSamplerState);
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
		case LeftTexture11:
			return L"Left Texture DX11";
		case RightTexture11:
			return L"Right Texture DX11";
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
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
		case LeftTexture11:
			return L"Left Texture DX11";
		case RightTexture11:
			return L"Right Texture DX11";
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
		case Pitch:
			return L"Pitch";
		case Yaw:
			return L"Yaw";
		case Roll:
			return L"Roll";
		case OrientationW:
			break;
		case OrientationX:
			break;
		case OrientationY:
			break;
		case OrientationZ:
			break;
		case PositionX:
			return L"Position X";
		case PositionY:
			return L"Position Y";
		case PositionZ:
			return L"Position Z";
		case View:
			return L"View";
		case World:
			return L"World";
		case ResolutionWidth:
			return L"Resolution Width";
		case ResolutionHeight:
			return L"Resolution Height";
		case ProjectionLeft:
			return L"Projection Left";
		case ProjectionRight:
			return L"Projection Right";
		case ImmersiveMode:
			return L"Immersive Mode";
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
		case LeftTexture11:
		case RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
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
		case LeftTexture11:
		case RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
		case Pitch:
		case Yaw:
		case Roll:
		case OrientationW:
		case OrientationX:
		case OrientationY:
		case OrientationZ:
		case PositionX:
		case PositionY:
		case PositionZ:
			return NOD_Plugtype::AQU_FLOAT;
		case View:
		case World:
			return NOD_Plugtype::AQU_D3DMATRIX;
			break;
		case ResolutionWidth:
		case ResolutionHeight:
			return NOD_Plugtype::AQU_UINT;
		case ProjectionLeft:
		case ProjectionRight:
			return NOD_Plugtype::AQU_D3DMATRIX;
		case ImmersiveMode:
			return NOD_Plugtype::AQU_BOOL;
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
		case LeftTexture11:
			return (void*)&m_pcTex11DrawSRV[0];
		case RightTexture11:
			return (void*)&m_pcTex11DrawSRV[1];
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
		default:
			break;
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
		case LeftTexture11:
			m_ppcTex11InputSRV[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case RightTexture11:
			m_ppcTex11InputSRV[1] = (ID3D11ShaderResourceView**)pData;
			break;
		case LeftTexture10:
			break;
		case RightTexture10:
			break;
		case LeftTexture9:
			break;
		case RightTexture9:
			break;
		case Pitch:
			m_pfPitch = (float*)pData;
			break;
		case Yaw:
			m_pfYaw = (float*)pData;
			break;
		case Roll:
			m_pfRoll = (float*)pData;
			break;
		case OrientationW:
			break;
		case OrientationX:
			break;
		case OrientationY:
			break;
		case OrientationZ:
			break;
		case PositionX:
			m_pfPositionX = (float*)pData;
			break;
		case PositionY:
			m_pfPositionY = (float*)pData;
			break;
		case PositionZ:
			m_pfPositionZ = (float*)pData;
			break;
		case View:
			m_psView = (D3DMATRIX*)pData;
			break;
		case World:
			break;
		case ResolutionWidth:
			m_punTexResolutionWidth = (UINT32*)pData;
			break;
		case ResolutionHeight:
			m_punTexResolutionHeight = (UINT32*)pData;
			break;
		case ProjectionLeft:
			m_psProjection[0] = (D3DMATRIX*)pData;
			break;
		case ProjectionRight:
			m_psProjection[1] = (D3DMATRIX*)pData;
			break;
		case ImmersiveMode:
			m_pbImmersiveMode = (BOOL*)pData;
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
	// TODO !! DX9 classes
	if (eD3DInterface != INTERFACE_IDXGISWAPCHAIN) return nullptr;
	if (eD3DMethod != METHOD_IDXGISWAPCHAIN_PRESENT) return nullptr;

	// D3D version ?
	static enum D3DVersion
	{
		D3D_Undefined,
		D3D_9,
		D3D_10,
		D3D_11,
	} eD3DVersion = D3D_Undefined;

	// get data
	if (m_pfPitch) m_fPitch = *m_pfPitch;
	if (m_pfYaw) m_fYaw = *m_pfYaw;
	if (m_pfRoll) m_fRoll = *m_pfRoll;

	if (m_pfPositionX) m_sPositionVector.x = *m_pfPositionX;
	if (m_pfPositionY) m_sPositionVector.y = *m_pfPositionY;
	if (m_pfPositionZ) m_sPositionVector.z = *m_pfPositionZ;

	if (m_pbImmersiveMode) m_sCinemaRoomSetup.bImmersiveMode = *m_pbImmersiveMode;

	// render cinema for specified D3D version
	switch (eD3DVersion)
	{
		case D3D_Undefined:
			// D3D version not defined ?
			switch (eD3DInterface)
			{
				case INTERFACE_IDIRECT3DDEVICE9:
				case INTERFACE_IDIRECT3DSWAPCHAIN9:
					eD3DVersion = D3D_9;
					break;
				case INTERFACE_IDXGISWAPCHAIN:
					if (eD3DVersion == D3D_Undefined)
					{
						// get device
						IUnknown* pcDevice = nullptr;
						((IDXGISwapChain*)pThis)->GetDevice(__uuidof(ID3D10Device), (void**)&pcDevice);
						if (pcDevice)
						{
							eD3DVersion = D3D_10;
							pcDevice->Release();
							return nullptr;
						}
						// get device
						((IDXGISwapChain*)pThis)->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
						if (pcDevice)
						{
							eD3DVersion = D3D_11;
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

					  if ((m_pVBCinemaTheatre) && (m_pVBCinemaScreen))
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
								   InitD3D11(pcDevice, pcContext, (IDXGISwapChain*)pThis);

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
	// output textures...left
	if ((!m_pStereoOutputLeft) && (m_punTexResolutionWidth) && (m_punTexResolutionHeight))
	{
		if ((*m_punTexResolutionWidth) && (*m_punTexResolutionHeight))
		{
			INT32 nWidth = (INT32)*m_punTexResolutionWidth;
			INT32 nHeight = (INT32)*m_punTexResolutionHeight;

			// set same aspect ratio for left and right
			m_fAspectRatio = float(nWidth) / float(nHeight);

			// create left render target
			if (FAILED(pcDevice->CreateTexture((UINT)nWidth, (UINT)nHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pStereoOutputLeft, NULL)))
			{
				OutputDebugString(L"[CIN] Failed to create left stereo output texture.");
				m_pStereoOutputLeft = nullptr;
			}
			else
			{
				// get surface
				m_pStereoOutputLeft->GetSurfaceLevel(0, &m_pStereoOutputSurfaceLeft);

				// create depth stencil left
				if (m_pcDepthStencilLeft)
				{
					m_pcDepthStencilLeft->Release();
					m_pcDepthStencilLeft = nullptr;
				}
				if (FAILED(pcDevice->CreateDepthStencilSurface((UINT)nWidth, (UINT)nHeight, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &m_pcDepthStencilLeft, NULL)))
				{
					OutputDebugString(L"[CIN] Failed to create left depth stencil !");
				}
			}
		}
	}
	else
	{
		if (!(*m_punTexResolutionWidth) || !(*m_punTexResolutionHeight))
		{
			SAFE_RELEASE(m_pStereoOutputSurfaceLeft);
			SAFE_RELEASE(m_pStereoOutputLeft);
			SAFE_RELEASE(m_pcDepthStencilLeft);
		}
	}

	// output textures...right
	if ((!m_pStereoOutputRight) && (m_punTexResolutionWidth) && (m_punTexResolutionHeight))
	{
		if ((*m_punTexResolutionWidth) && (*m_punTexResolutionHeight))
		{
			INT32 nWidth = *m_punTexResolutionWidth;
			INT32 nHeight = *m_punTexResolutionHeight;

			// set same aspect ratio for left and right
			m_fAspectRatio = float(nWidth) / float(nHeight);

			// create left render target
			if (FAILED(pcDevice->CreateTexture((UINT)nWidth, (UINT)nHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pStereoOutputRight, NULL)))
			{
				OutputDebugString(L"[CIN] Failed to create right stereo output texture.");
				m_pStereoOutputRight = nullptr;
			}
			else
			{
				// get surface
				m_pStereoOutputRight->GetSurfaceLevel(0, &m_pStereoOutputSurfaceRight);

				// create depth stencil right
				if (m_pcDepthStencilRight)
				{
					m_pcDepthStencilRight->Release();
					m_pcDepthStencilRight = nullptr;
				}
				if (FAILED(pcDevice->CreateDepthStencilSurface((UINT)nWidth, (UINT)nHeight, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &m_pcDepthStencilRight, NULL)))
				{
					OutputDebugString(L"[CIN] Failed to create right depth stencil !");
				}
			}
		}
	}
	else
	{
		if (!(*m_punTexResolutionWidth) || !(*m_punTexResolutionHeight))
		{
			SAFE_RELEASE(m_pStereoOutputSurfaceRight);
			SAFE_RELEASE(m_pStereoOutputRight);
			SAFE_RELEASE(m_pcDepthStencilRight);
		}
	}

	// pixel shader created ?
	if (!m_psCinema)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(PSSimplest, (UINT)strlen(PSSimplest), NULL, NULL, "OurFirstPixelShader", "ps_3_0", NULL, &pShader, NULL, &m_ctPCinema)))
		{
			OutputDebugString(L"Pixel shader >Textured< compiled!");
			pcDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &m_psCinema);
		}
	}
	if (!m_psCinemaTheatre)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(PSTexColor, (UINT)strlen(PSTexColor), NULL, NULL, "TexColorPixelShader", "ps_3_0", NULL, &pShader, NULL, &m_ctPCinemaTheatre)))
		{
			OutputDebugString(L"Pixel shader >Textured+Colored< compiled!");
			pcDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &m_psCinemaTheatre);
		}
	}

	// vertex shader created ?
	if (!m_vsCinema)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(VSSimplest, (UINT)strlen(VSSimplest), NULL, NULL, "SimplestVertexShader", "vs_1_1", NULL, &pShader, NULL, &m_ctVCinema)))
		{
			OutputDebugString(L"Vertex shader >Textured< compiled!");
			pcDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_vsCinema);
		}
	}
	if (!m_vsCinemaTheatre)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(VSTexColor, (UINT)strlen(VSTexColor), NULL, NULL, "TexColorVertexShader", "vs_1_1", NULL, &pShader, NULL, &m_ctVCinemaTheatre)))
		{
			OutputDebugString(L"Vertex shader >Textured+Colored< compiled!");
			pcDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_vsCinemaTheatre);
		}
	}

	// texture created ?
	if (!m_pTextureCinema)
	{
		HMODULE hModule = GetModuleHandle(L"VireioCinema.dll");
		if (SUCCEEDED(D3DXCreateTextureFromResource(pcDevice, hModule, MAKEINTRESOURCE(IMG_LOGO01), &m_pTextureCinema)))
			OutputDebugString(L"Texture created !");
	}
	if (!m_pTextureCinemaTheatre)
	{
		if (SUCCEEDED(D3DXCreateTexture(pcDevice, 256, 256, 1, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureCinemaTheatre)))
		{
			D3DLOCKED_RECT rect;
			m_pTextureCinemaTheatre->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
			CopyMemory(rect.pBits, tex_pixels[0], 256 * 256 * sizeof(D3DCOLOR));
			m_pTextureCinemaTheatre->UnlockRect(0);
		}
	}

	// vertex buffer created ?
	if (!m_pVBCinemaScreen)
	{
		TexturedVertex asVertices[] =
		{
			{ D3DXVECTOR4(-1.92f, -1.08f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f) },
			{ D3DXVECTOR4(1.92f, -1.08f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f) },
			{ D3DXVECTOR4(1.92f, 1.08f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
			{ D3DXVECTOR4(-1.92f, 1.08f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f) }
		};

		float fScale = 0.8f;
		D3DXVECTOR3 sTranslate = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
		for (int i = 0; i < 4; i++)
		{
			// scale and translate
			asVertices[i].sPos.x *= fScale;
			asVertices[i].sPos.y *= fScale;
			asVertices[i].sPos.z *= fScale;

			asVertices[i].sPos.x += sTranslate.x;
			asVertices[i].sPos.y += sTranslate.y;
			asVertices[i].sPos.z += sTranslate.z;
		}

		// create
		if (FAILED(pcDevice->CreateVertexBuffer(4 * sizeof(TexturedVertex), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_pVBCinemaScreen, NULL)))
		{
			OutputDebugString(L"Error while creating VertexBuffer!");
		}

		// lock and copy vertices
		PVOID pVertices;
		if (FAILED(m_pVBCinemaScreen->Lock(0, 4 * sizeof(TexturedVertex), (void**)&pVertices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pVertices, asVertices, 4 * sizeof(TexturedVertex));
			m_pVBCinemaScreen->Unlock();
		}
	}

	if (!m_pVBCinemaTheatre)
	{
		// create vertex buffer
		if (FAILED(pcDevice->CreateVertexBuffer(numVertices*sizeof(TexColoredVertex), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_pVBCinemaTheatre, NULL)))
		{
			OutputDebugString(L"Error while creating VertexBuffer!");
		}

		// lock and copy vertices
		PVOID pVertices;
		if (FAILED(m_pVBCinemaTheatre->Lock(0, numVertices*sizeof(TexColoredVertex), (void**)&pVertices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pVertices, Vertices, numVertices*sizeof(TexColoredVertex));
			m_pVBCinemaTheatre->Unlock();
		}

		// create index buffer
		if (FAILED(pcDevice->CreateIndexBuffer(numIndices*sizeof(UINT16), 0, D3DFORMAT::D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIBCinemaTheatre, NULL)))
		{
			OutputDebugString(L"Error while creating IndexBuffer!");
		}

		// lock and copy indices
		PVOID pIndices;
		if (FAILED(m_pIBCinemaTheatre->Lock(0, numIndices*sizeof(UINT16), (void**)&pIndices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pIndices, Indices, numIndices*sizeof(UINT16));
			m_pIBCinemaTheatre->Unlock();
		}
	}
}

/**
* Renders the virtual cinema (DX9).
***/
void VireioCinema::RenderD3D9(LPDIRECT3DDEVICE9 pcDevice)
{
	// TODO !! WHOLE FUNCTIONALITY BROKEN FOR NOW !!
	(E_NOTIMPL);
	return;

	// render... first create matrices
	D3DXMATRIX matProjection;
	//D3DXMatrixPerspectiveFovLH(&matProjection, (float)(70.0f * 3.14f / 180.0f), m_fAspectRatio, 0.01f, 50.0f);
	D3DXMatrixPerspectiveFovLH(&matProjection, (float)D3DXToRadian(116.0), m_fAspectRatio, 0.1f, 30.0f);
	D3DXMATRIX matView;
	D3DXMatrixLookAtRH(&matView, /*CameraPos*/&D3DXVECTOR3(0, 2.1f, -0.5f), &D3DXVECTOR3(0, 2.0, 0), &D3DXVECTOR3(0, 1, 0));

	// is that right to add the offset at this point ?
	D3DXMATRIX matViewLeft, matViewRight, matTransLeft, matTransRight;
	// D3DXMatrixTranslation(&matTransLeft, m_sViewOffset[0].x, m_sViewOffset[0].y, m_sViewOffset[0].z);
	// D3DXMatrixTranslation(&matTransRight, m_sViewOffset[1].x, m_sViewOffset[1].y, m_sViewOffset[1].z);
	matViewLeft = matTransLeft * matView;
	matViewRight = matTransRight * matView;

	// yaw pitch roll
	D3DXMATRIX matYaw;
	D3DXMatrixIdentity(&matYaw);
	D3DXMatrixRotationY(&matYaw, -m_fYaw);
	D3DXMATRIX matPitch;
	D3DXMatrixIdentity(&matPitch);
	D3DXMatrixRotationX(&matPitch, -m_fPitch);
	D3DXMATRIX matRoll;
	D3DXMatrixIdentity(&matRoll);
	D3DXMatrixRotationZ(&matRoll, -m_fRoll);

	// first, use standard IPD (0.064 meters)
	D3DXMATRIX matSeparationLeft, matSeparationRight;
	D3DXMatrixTranslation(&matSeparationLeft, 0.032f, 0, 0);
	D3DXMatrixTranslation(&matSeparationRight, -0.032f, 0, 0);

	pcDevice->BeginScene();

	// save states
	IDirect3DStateBlock9* pStateBlock;
	pcDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

	// set ALL render states to default
	SetAllRenderStatesDefault(pcDevice);

	// set states
	pcDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pcDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pcDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pcDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
	pcDevice->SetTextureStageState(0, D3DTSS_CONSTANT, 0xffffffff);
	pcDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pcDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);// D3DZB_TRUE);
	pcDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);// TRUE);
	pcDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	pcDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	pcDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pcDevice->SetVertexShader(m_vsCinema);
	pcDevice->SetPixelShader(m_psCinema);
	pcDevice->SetVertexDeclaration(NULL);

	// set stream source, FVF, texture
	pcDevice->SetStreamSource(0, m_pVBCinemaScreen, 0, sizeof(TexturedVertex));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	if (m_ppcFrameTexture)
	{
		if (*m_ppcFrameTexture)
			pcDevice->SetTexture(0, *m_ppcFrameTexture);
		else
			pcDevice->SetTexture(0, m_pTextureCinema);
	}
	else
		pcDevice->SetTexture(0, m_pTextureCinema);

	// save render targets.. TODO !! get max render targets
	int nTargets = 4;
	LPDIRECT3DSURFACE9 pSurface[4];
	for (int i = 0; i < nTargets; i++)
		pcDevice->GetRenderTarget(i, &pSurface[i]);

	D3DCOLOR clearColor = D3DCOLOR_RGBA(0, 255, 0, 0);

	// set matrix
	if (m_psProjection[0])
		CopyMemory(&matProjection, (void*)(m_psProjection[0]), sizeof(D3DMATRIX));
	m_ctVCinema->SetMatrix(pcDevice, "xViewProjection", &(matViewLeft * matYaw * matPitch * matRoll * matSeparationLeft * matProjection));
	// pcDevice->SetVertexShaderConstantF(0, &matProjection.m[0][0], 4);

	// set render target and clear
	pcDevice->SetRenderTarget(0, m_pStereoOutputSurfaceLeft);
	pcDevice->SetDepthStencilSurface(m_pcDepthStencilLeft);
	pcDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);

	// draw left
	pcDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// set render data and draw left theatre
	pcDevice->SetVertexShader(m_vsCinemaTheatre);
	pcDevice->SetPixelShader(m_psCinemaTheatre);
	pcDevice->SetStreamSource(0, m_pVBCinemaTheatre, 0, sizeof(TexColoredVertex));
	pcDevice->SetIndices(m_pIBCinemaTheatre);
	pcDevice->SetTexture(0, m_pTextureCinemaTheatre);
	m_ctVCinemaTheatre->SetMatrix(pcDevice, "xViewProjection", &(matViewLeft * matYaw * matPitch * matRoll * matSeparationLeft * matProjection));
	// pcDevice->SetVertexShaderConstantF(0, &matProjection.m[0][0], 4);
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pcDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numIndices / 3);

	D3DRECT crec = { 200, 200, 400, 600 }; //position and dimensions of our rectangle
	pcDevice->Clear(1, &crec, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 128, 64), 0, 0);

	// set matrix (right)
	if (m_psProjection[1])
		CopyMemory(&matProjection, (void*)(m_psProjection[1]), sizeof(D3DMATRIX));
	m_ctVCinema->SetMatrix(pcDevice, "xViewProjection", &(matViewRight * matYaw * matPitch * matRoll * matSeparationRight * matProjection));

	// set all back and render right cinema screen
	pcDevice->SetVertexShader(m_vsCinema);
	pcDevice->SetPixelShader(m_psCinema);
	pcDevice->SetStreamSource(0, m_pVBCinemaScreen, 0, sizeof(TexturedVertex));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	if (m_ppcFrameTexture)
	{
		if (*m_ppcFrameTexture)
			pcDevice->SetTexture(0, *m_ppcFrameTexture);
		else
			pcDevice->SetTexture(0, m_pTextureCinema);
	}
	else
		pcDevice->SetTexture(0, m_pTextureCinema);
	pcDevice->SetRenderTarget(0, m_pStereoOutputSurfaceRight);
	pcDevice->SetDepthStencilSurface(m_pcDepthStencilRight);
	pcDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);

	// draw right
	pcDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// set render data and draw
	pcDevice->SetVertexShader(m_vsCinemaTheatre);
	pcDevice->SetPixelShader(m_psCinemaTheatre);
	pcDevice->SetStreamSource(0, m_pVBCinemaTheatre, 0, sizeof(TexColoredVertex));
	pcDevice->SetIndices(m_pIBCinemaTheatre);
	pcDevice->SetTexture(0, m_pTextureCinemaTheatre);
	m_ctVCinemaTheatre->SetMatrix(pcDevice, "xViewProjection", &(matViewRight * matYaw * matPitch * matRoll * matSeparationRight * matProjection));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pcDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numIndices / 3);

	// set back render target
	for (int i = 0; i < nTargets; i++)
		pcDevice->SetRenderTarget(i, pSurface[i]);

	pStateBlock->Apply();
	pStateBlock->Release();

	pcDevice->EndScene();
}

/**
* Inits all stuff (DX11).
***/
void VireioCinema::InitD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain)
{
	// create vertex shader
	if (!m_pcVSGeometry11)
	{
		if (FAILED(Create3DVertexShader(pcDevice, &m_pcVSGeometry11, &m_pcVLGeometry11)))
			OutputDebugString(L"[CIN] Failed to create vertex shader. !");
	}

	// create pixel shader
	if (!m_pcPSGeometry11)
	{
		// create screen pixel shader technique by cinema room setup
		PixelShaderTechnique eTechnique = PixelShaderTechnique::GeometryDiffuseTexturedMouse;
		switch (m_sCinemaRoomSetup.ePixelShaderFX_Screen)
		{
			case CinemaRoomSetup::PixelShaderFX_Screen::Screen_GeometryDiffuseTexturedMouse:
				eTechnique = PixelShaderTechnique::GeometryDiffuseTexturedMouse;
				break;
		}
		if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPSGeometry11, eTechnique)))
			OutputDebugString(L"[CIN] Failed to create pixel shader. !");
	}

	// create render targets
	for (UINT unEye = 0; unEye < 2; unEye++)
	{
		if ((!m_pcTex11Draw[unEye]) && (m_punTexResolutionHeight) && (m_punTexResolutionWidth))
		{
			if (!(*m_punTexResolutionWidth) || !(*m_punTexResolutionHeight)) continue;

			// fill the description
			D3D11_TEXTURE2D_DESC sDescTex;
			sDescTex.Width = (UINT)(*m_punTexResolutionWidth);
			sDescTex.Height = (UINT)(*m_punTexResolutionHeight);
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
			pcDevice->CreateTexture2D(&sDescTex, NULL, &m_pcTex11Draw[unEye]);
			if (m_pcTex11Draw[unEye])
			{
				// create render target view
				if (FAILED(pcDevice->CreateRenderTargetView(m_pcTex11Draw[unEye], NULL, &m_pcTex11DrawRTV[unEye])))
					OutputDebugString(L"[CIN] Failed to create render target view.");

				// create shader resource view
				if (FAILED(pcDevice->CreateShaderResourceView(m_pcTex11Draw[unEye], NULL, &m_pcTex11DrawSRV[unEye])))
					OutputDebugString(L"[CIN] Failed to create render target shader resource view.");

				// set this as private data interface to the shader resource views instead of texture here !!!!
				if ((m_pcTex11DrawRTV[unEye]) && (m_pcTex11DrawSRV[unEye]))
				{
					m_pcTex11DrawSRV[unEye]->SetPrivateDataInterface(PDIID_ID3D11TextureXD_RenderTargetView, m_pcTex11DrawRTV[unEye]);
					m_pcTex11DrawRTV[unEye]->Release();
				}
			}
			else OutputDebugString(L"[CIN] Failed to create render target !");
		}
	}

	// create the depth stencil
	if ((!m_pcDSGeometry11[0]) && (m_punTexResolutionHeight) && (m_punTexResolutionWidth))
	{
		if ((*m_punTexResolutionHeight) && (*m_punTexResolutionWidth))
		{
			// Create depth stencil textures
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = *m_punTexResolutionWidth;
			descDepth.Height = *m_punTexResolutionHeight;
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

				// create eye pose matrix... TODO !! first, use standard IPD (0.064 meters)
				if (nEye)
					D3DXMatrixTranslation(&m_sToEye[nEye], -0.032f, 0, 0);
				else
					D3DXMatrixTranslation(&m_sToEye[nEye], 0.032f, 0, 0);
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

	// create constant buffer
	if (!m_pcConstantBufferGeometry)
	{
		if (FAILED(CreateGeometryConstantBuffer(pcDevice, &m_pcConstantBufferGeometry, (UINT)sizeof(GeometryConstantBuffer))))
			OutputDebugString(L"[CIN] Failed to create constant buffer.");
	}

	// is all created ? only create render models if all things are created
	if ((!m_pcConstantBufferGeometry) || (!m_pcSampler11) || (!m_pcDSGeometry11[0]) || (!m_pcTex11Draw[0]) || (!m_pcPSGeometry11) || (!m_pcVSGeometry11)) return;

	// create render models...
	if (!m_asRenderModels.size())
	{
		// set room size by screen depth (+10cm)
		float fSizeScale = m_sCinemaRoomSetup.fScreenDepth + 0.1f;
#pragma region cinema screen
		if (true/**TODO_ADD_BOOL_HERE**/)
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

			// and create the model...
			float fScale = m_sCinemaRoomSetup.fScreenWidth / 3.84f;
			D3DXVECTOR3 sScale = D3DXVECTOR3(fScale, fScale, fScale);
			AddRenderModelD3D11(pcDevice, nullptr, nullptr, asVertices, aunIndices, 4, 2, sScale, D3DXVECTOR3(0.0f, m_sCinemaRoomSetup.fScreenLevel, m_sCinemaRoomSetup.fScreenDepth), 1920, 1080);
		}
#pragma endregion
#pragma region floor top/bottom
		if (true/**TODO_ADD_BOOL_HERE**/)
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
			PixelShaderTechnique eTechnique = PixelShaderTechnique::GeometryDiffuseTexturedMouse;

			switch (m_sCinemaRoomSetup.ePixelShaderFX_Floor[1])
			{
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Bubbles:
					eTechnique = PixelShaderTechnique::Bubbles;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_C64Plasma:
					eTechnique = PixelShaderTechnique::C64Plasma;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_StringTheory:
					eTechnique = PixelShaderTechnique::StringTheory;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_WaterCaustic:
					eTechnique = PixelShaderTechnique::WaterCaustic;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Worley01:
					eTechnique = PixelShaderTechnique::Worley01;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Planets:
					eTechnique = PixelShaderTechnique::Planets;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_HypnoticDisco:
					eTechnique = PixelShaderTechnique::HypnoticDisco;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_VoronoiSmooth:
					eTechnique = PixelShaderTechnique::VoronoiSmooth;
					break;
			}
			ID3D11PixelShader* pcEffect = nullptr;
			CreatePixelShaderEffect(pcDevice, &pcEffect, eTechnique);

			// and create the model... scale here by the screen depth to ensure the screen is always seen
			float fScale = fSizeScale / 2.0f;
			D3DXVECTOR3 sScale = D3DXVECTOR3(fScale, fScale, fScale);
			AddRenderModelD3D11(pcDevice, nullptr, pcEffect, asVertices, aunIndices, 4, 2, sScale, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// set indices top
			WORD aunIndices_top[] = { 0, 1, 3, 1, 2, 3 };

			// negate normals
			for (UINT unI = 0; unI < 4; unI++) asVertices[unI].sNormal *= -1.0f;

			// create top floor effect by cinema room setup
			PixelShaderTechnique eTechnique_top = PixelShaderTechnique::GeometryDiffuseTexturedMouse;
			switch (m_sCinemaRoomSetup.ePixelShaderFX_Floor[0])
			{
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Bubbles:
					eTechnique_top = PixelShaderTechnique::Bubbles;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_C64Plasma:
					eTechnique_top = PixelShaderTechnique::C64Plasma;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_StringTheory:
					eTechnique_top = PixelShaderTechnique::StringTheory;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_WaterCaustic:
					eTechnique_top = PixelShaderTechnique::WaterCaustic;
					break;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Worley01:
					eTechnique_top = PixelShaderTechnique::Worley01;
					break;
			}
			ID3D11PixelShader* pcEffect_top = nullptr;
			CreatePixelShaderEffect(pcDevice, &pcEffect_top, eTechnique_top);

			// and create the model... no more scale here since already scaled by first call
			AddRenderModelD3D11(pcDevice, nullptr, pcEffect_top, asVertices, aunIndices_top, 4, 2, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 4.8f, 0.0f));

		}
#pragma endregion
#pragma region walls
		if (true/**TODO_ADD_BOOL_HERE**/)
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
						break;
					case 1:
						eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_LR[1];
						break;
					case 2:
						eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[0];
						break;
					case 3:
						eEffect = m_sCinemaRoomSetup.ePixelShaderFX_Wall_FB[1];
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

				// and create the model... scale here by the screen depth to ensure the screen is always seen
				float fHeight = 600.0f;  // = (4.8f * 1200.0f) / 4.8f;
				float fWidth = (fSizeScale * 1000.0f) / 4.0f;

				AddRenderModelD3D11(pcDevice, nullptr, pcEffect, &asVertices[unI * 4], &aunIndices[unI * 6], 4, 2,
					D3DXVECTOR3(fSizeScale, 4.0f, fSizeScale),
					D3DXVECTOR3(0.0f, 2.4f, 0.0f), (UINT)fWidth, (UINT)fHeight);
			}
		}
#pragma endregion
	}

	// create constant shader constants..
	D3DXVECTOR4 sLightDir = m_sCinemaRoomSetup.sLightDirection;
	m_sGeometryConstants.sLightDir = sLightDir;
	m_sGeometryConstants.sLightAmbient = m_sCinemaRoomSetup.sColorAmbient;
	m_sGeometryConstants.sLightDiffuse = m_sCinemaRoomSetup.sColorDiffuse;

	// for aspect ratio based fx we set a 1.0 ratio here
	m_sGeometryConstants.sResolution.x = 1024.0f;
	m_sGeometryConstants.sResolution.y = 1024.0f;
}

/**
* Renders the virtual cinema (DX11).
***/
void VireioCinema::RenderD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain)
{
	if ((!m_ppcTex11InputSRV[0]) || (!m_ppcTex11InputSRV[1]))
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
		m_apcTex11InputSRV[0] = *m_ppcTex11InputSRV[0];
		m_apcTex11InputSRV[1] = *m_ppcTex11InputSRV[1];
	};

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
	sViewport.Width = (FLOAT)(*m_punTexResolutionWidth);
	sViewport.Height = (FLOAT)(*m_punTexResolutionHeight);
	sViewport.MaxDepth = 1.0f;
	pcContext->RSSetViewports(1, &sViewport);

	// immersive mode ? render fullscreen
	if (m_sCinemaRoomSetup.bImmersiveMode)
	{
		RenderFullscreenD3D11(pcDevice, pcContext, pcSwapchain);

		// set back device
		ApplyStateblock(pcContext, &sStateBlock);

		return;
	}

	// Set the input layout, buffers, sampler
	pcContext->IASetInputLayout(m_pcVLGeometry11);
	UINT stride = sizeof(TexturedNormalVertex);
	UINT offset = 0;

	pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferGeometry);
	pcContext->PSSetConstantBuffers(0, 1, &m_pcConstantBufferGeometry);
	pcContext->PSSetSamplers(0, 1, &m_pcSampler11);

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
		FLOAT afColorRgba[4] = { 0.2f, 0.0f, 0.2f, 1.0f };
		pcContext->ClearRenderTargetView(m_pcTex11DrawRTV[nEye], afColorRgba);
	}

	// Initialize the view matrix
	if (m_psView)
	{
		// get connected
		CopyMemory(&m_sView, m_psView, sizeof(D3DMATRIX));
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
	if (s_unTick > m_unMouseTickCount)
		m_sGeometryConstants.sMouse = D3DXVECTOR4(1.1f, 1.1f, 0.0f, 0.0f);
	else
		m_sGeometryConstants.sMouse = D3DXVECTOR4((float)sPoint.x / (float)sDesktop.right, (float)sPoint.y / (float)sDesktop.bottom, 0.0f, 0.0f);

	// set world identity
	D3DXMATRIX sWorld;
	D3DXMatrixIdentity(&sWorld);

	// performance mode ?
	UINT unRenderModelsNo = (UINT)m_asRenderModels.size();
	if (m_sCinemaRoomSetup.bPerformanceMode) unRenderModelsNo = 1;

	// loop through available render models, render
	for (UINT unI = 0; unI < (UINT)unRenderModelsNo; unI++)
	{
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
			pcContext->PSSetShaderResources(0, 1, &m_asRenderModels[unI].pcTextureSRV);
			pcContext->PSSetShader(m_asRenderModels[unI].pcEffect, NULL, 0);
		}
		else // set main shader
			pcContext->PSSetShader(m_pcPSGeometry11, NULL, 0);

		// left + right
		for (int nEye = 0; nEye < 2; nEye++)
		{
			// set frame texture left/right if main effect
			if (!m_asRenderModels[unI].pcEffect)
				pcContext->PSSetShaderResources(0, 1, &m_apcTex11InputSRV[nEye]);

			// set WVP matrix, update constant buffer
			D3DXMATRIX sWorldViewProjection;
			if (m_psProjection[nEye])
			{
				sWorldViewProjection = sWorld * m_sView * (*(m_psProjection[nEye]));
			}
			else sWorldViewProjection = sWorld * m_sView * m_sToEye[nEye] * m_sProj[nEye];

			D3DXMatrixTranspose(&m_sGeometryConstants.sWorldViewProjection, &sWorldViewProjection);
			pcContext->UpdateSubresource(m_pcConstantBufferGeometry, 0, NULL, &m_sGeometryConstants, 0, 0);

			// set render target
			pcContext->OMSetRenderTargets(1, &m_pcTex11DrawRTV[nEye], m_pcDSVGeometry11[nEye]);

			// draw
			pcContext->DrawIndexed(m_asRenderModels[unI].unTriangleCount * 3, 0, 0);
		}
	}

	// set back device
	ApplyStateblock(pcContext, &sStateBlock);
}

/**
* Renders the
***/
void VireioCinema::RenderFullscreenD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain)
{
	// create all bool
	bool bAllCreated = true;

	// create vertex shader fullscreen
	if (!m_pcVertexShader11)
	{
		if (FAILED(Create2DVertexShader(pcDevice, &m_pcVertexShader11, &m_pcVertexLayout11)))
		{
			bAllCreated = false;
		}
	}
	// create pixel shader fullscreen
	if (!m_pcPixelShader11)
	{
		if (FAILED(CreatePixelShaderEffect(pcDevice, &m_pcPixelShader11, PixelShaderTechnique::FullscreenSimple)))
			bAllCreated = false;
	}
	// Create vertex buffer fullscreen
	if (!m_pcVertexBuffer11)
	{
		if (FAILED(CreateFullScreenVertexBuffer(pcDevice, &m_pcVertexBuffer11)))
			bAllCreated = false;
	}
	// create constant buffer fullscreen
	if (!m_pcConstantBufferDirect11)
	{
		if (FAILED(CreateMatrixConstantBuffer(pcDevice, &m_pcConstantBufferDirect11)))
			bAllCreated = false;
	}
	// sampler ?
	if (!m_pcSamplerState)
	{
		// Create the sampler state
		D3D11_SAMPLER_DESC sSampDesc;
		ZeroMemory(&sSampDesc, sizeof(sSampDesc));
		sSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sSampDesc.MinLOD = 0;
		sSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(pcDevice->CreateSamplerState(&sSampDesc, &m_pcSamplerState)))
		{
			OutputDebugString(L"[OSVR] Failed to create Sampler State.");
			bAllCreated = false;
		}
	}

	// Set the input layout, buffers, sampler
	pcContext->IASetInputLayout(m_pcVertexLayout11);
	UINT stride = sizeof(TexturedVertex);
	UINT offset = 0;
	pcContext->IASetVertexBuffers(0, 1, &m_pcVertexBuffer11, &stride, &offset);
	pcContext->VSSetConstantBuffers(0, 1, &m_pcConstantBufferDirect11);
	pcContext->PSSetSamplers(0, 1, &m_pcSampler11);

	// Set primitive topology
	pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set shaders
	pcContext->VSSetShader(m_pcVertexShader11, 0, 0);
	pcContext->PSSetShader(m_pcPixelShader11, 0, 0);

	// aspect correction
	static D3DXVECTOR2 sAspectNormalized = D3DXVECTOR2();
	if (sAspectNormalized.x == 0.0f)
	{
		// get target aspect ratio
		D3DXVECTOR2 sTarget;
		if (m_punTexResolutionWidth)
			sTarget.x = (FLOAT)(*m_punTexResolutionWidth);
		else
			sTarget.x = 1.0f;
		if (m_punTexResolutionHeight)
			sTarget.y = (FLOAT)(*m_punTexResolutionHeight);
		else
			sTarget.y = 1.0f;
		float fAspectTarget = sTarget.y / sTarget.x;

		// get desktop aspect ratio
		RECT sDesktop;
		HWND pDesktop = GetDesktopWindow();
		GetWindowRect(pDesktop, &sDesktop);
		float fAspectScreen = (float)sDesktop.right / (float)sDesktop.bottom;

		// get normalized aspect
		sAspectNormalized.x = fAspectScreen * fAspectTarget;
		sAspectNormalized.y = 1.0f;
		D3DXVec2Normalize(&sAspectNormalized, &sAspectNormalized);

		// get a 1:1 ratio to the target
		D3DXVECTOR2 sOneNormalized = D3DXVECTOR2(1.0f, 1.0f);
		D3DXVec2Normalize(&sOneNormalized, &sOneNormalized);
		sAspectNormalized.x /= sOneNormalized.x;
		sAspectNormalized.y /= sOneNormalized.y;
	}

	// left + right
	for (int nEye = 0; nEye < 2; nEye++)
	{
		// get a matrix here
		D3DXMATRIX sProj;

		// get a vector with ipd and virtual screen distance
		D3DXVECTOR4 sVector;
		if (nEye) sVector = D3DXVECTOR4(-(m_sImmersiveFullscreenSettings.fIPD / 2.0f), 0, m_sImmersiveFullscreenSettings.fVSD, 0); else sVector = D3DXVECTOR4(m_sImmersiveFullscreenSettings.fIPD / 2.0f, 0, m_sImmersiveFullscreenSettings.fVSD, 0);

		// projection matrices connected ?
		if (m_psProjection[nEye])
		{
			// get projection for this eye
			sProj = (*(m_psProjection[nEye]));

			// translate the vector by the eye matrix
			D3DXVec4Transform(&sVector, &sVector, &sProj);
		}

		// scale both x+y by normalized aspect translate on x axis for each eye
		D3DXMatrixIdentity(&sProj);
		sProj.m[0][0] = sAspectNormalized.x;
		sProj.m[1][1] = sAspectNormalized.y;
		sProj.m[0][3] = sVector.x;

		// update constant buffer
		pcContext->UpdateSubresource((ID3D11Resource*)m_pcConstantBufferDirect11, 0, NULL, &sProj, 0, 0);

		// set frame texture left/right
		pcContext->PSSetShaderResources(0, 1, &m_apcTex11InputSRV[nEye]);

		// set render target
		pcContext->OMSetRenderTargets(1, &m_pcTex11DrawRTV[nEye], nullptr);

		// Render a triangle
		pcContext->Draw(6, 0);
	}
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
	sVertexBufferDesc.ByteWidth = sizeof(TexturedNormalVertex)* unVertexCount;
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
	sIndexBufferDesc.ByteWidth = sizeof(WORD)* unTriangleCount * 3;
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
* Sets all Direct3D 9 render states to their default values.
* Use this function only if a game does not want to render.
***/
void VireioCinema::SetAllRenderStatesDefault(LPDIRECT3DDEVICE9 pcDevice)
{
	// set all Direct3D 9 RenderStates to default values
	float fData = 0.0f;
	double dData = 0.0f;

	pcDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pcDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pcDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pcDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pcDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_LASTPIXEL, TRUE);
	pcDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pcDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	pcDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pcDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pcDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pcDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_FOGCOLOR, 0);
	pcDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_STENCILREF, 0);
	pcDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_WRAP0, 0);
	pcDevice->SetRenderState(D3DRS_WRAP1, 0);
	pcDevice->SetRenderState(D3DRS_WRAP2, 0);
	pcDevice->SetRenderState(D3DRS_WRAP3, 0);
	pcDevice->SetRenderState(D3DRS_WRAP4, 0);
	pcDevice->SetRenderState(D3DRS_WRAP5, 0);
	pcDevice->SetRenderState(D3DRS_WRAP6, 0);
	pcDevice->SetRenderState(D3DRS_WRAP7, 0);
	pcDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	pcDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pcDevice->SetRenderState(D3DRS_AMBIENT, 0);
	pcDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	pcDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	pcDevice->SetRenderState(D3DRS_LOCALVIEWER, TRUE);
	pcDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pcDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	pcDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
	pcDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	pcDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	pcDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pcDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	pcDevice->SetRenderState(D3DRS_POINTSIZE, 64);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	pcDevice->SetRenderState(D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
	pcDevice->SetRenderState(D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE);
	dData = 64.0;
	pcDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&dData));
	pcDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_TWEENFACTOR, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pcDevice->SetRenderState(D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC);
	pcDevice->SetRenderState(D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR);
	pcDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	pcDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_MINTESSELLATIONLEVEL, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_MAXTESSELLATIONLEVEL, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_X, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_Y, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_Z, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_W, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_ENABLEADAPTIVETESSELLATION, FALSE);
	pcDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE1, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE2, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE3, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_BLENDFACTOR, 0xffffffff);
	pcDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);
	pcDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
	pcDevice->SetRenderState(D3DRS_WRAP8, 0);
	pcDevice->SetRenderState(D3DRS_WRAP9, 0);
	pcDevice->SetRenderState(D3DRS_WRAP10, 0);
	pcDevice->SetRenderState(D3DRS_WRAP11, 0);
	pcDevice->SetRenderState(D3DRS_WRAP12, 0);
	pcDevice->SetRenderState(D3DRS_WRAP13, 0);
	pcDevice->SetRenderState(D3DRS_WRAP14, 0);
	pcDevice->SetRenderState(D3DRS_WRAP15, 0);
	pcDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	pcDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	pcDevice->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
}
