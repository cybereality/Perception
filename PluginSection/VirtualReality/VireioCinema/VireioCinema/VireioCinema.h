/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin
Copyright (C) 2014 Denis Reischl

File <VireioCinema.h> and
Class <VireioCinema> :
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

#include"AQU_Nodus.h"
#include<vector>


#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <D3DX11.h>
#pragma comment(lib, "D3DX11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_COMMANDERS                           1
#define NUMBER_OF_DECOMMANDERS                         2

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum class VRC_Commanders
{
	Stereo_Cinema
};

/// <summary>
/// Node Decommander Enumeration.
/// </summary>
enum class VRC_Decommanders
{
	Stereo_Splitter,
	Tracker
};

/// <summary>
/// Virtual Reality Cinema Node Plugin
/// </summary>
class VireioCinema : public AQU_Nodus
{
public:
	VireioCinema();
	virtual ~VireioCinema();

	/*** VireioCinema public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual DWORD           GetNodeWidth() { return 512; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** VireioCinema private methods ***/
	void InitD3D9(LPDIRECT3DDEVICE9 pcDevice);
	void RenderD3D9(LPDIRECT3DDEVICE9 pcDevice);
	void InitD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext);
	void RenderD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain);
	void RenderFullscreenD3D11(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, IDXGISwapChain* pcSwapchain);
	void AddRenderModelD3D11(ID3D11Device* pcDevice, ID3D11Texture2D* pcTexture, ID3D11PixelShader* pcEffect, TexturedNormalVertex* asVertices, WORD* aunIndices, UINT32 unVertexCount, UINT32 unTriangleCount, D3DXVECTOR3 sScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3 sTranslate = D3DXVECTOR3(), UINT32 unWidth = 1024, UINT32 unHeight = 1024);
	HRESULT CreateD3D11Device(bool bCreateSwapChain);
	void SaveIniSettings();

#pragma region Commanders / Decommanders
	/// <summary>
	/// [OUT] All data this node outputs
	/// </summary>
	StereoData m_sStereoData;
	/// <summary>
	/// [IN] HMD Tracker Data
	/// All data incoming from Tracker (Oculus, OpenVR,...)
	/// </summary>
	HMDTrackerData* m_psTrackerData;
	/// <summary>
	/// [IN] Stereo Splitter Data
	/// All data incoming from Stereo Splitter
	/// </summary>
	StereoData* m_psStereoDataIn;
#pragma endregion
#pragma region VireioCinema D3D9/D3D10 private fields
	/// <summary>D3D11 device to be used in D3D9/D3D10 games</summary>
	ID3D11Device* m_pcD3D11Device;
	/// <summary>D3D11 device context to be used in D3D9/D3D10 games.</summary>
	ID3D11DeviceContext* m_pcD3D11Context;
	/// <summary>Dummy window for the d3d11 device.</summary>
	HWND m_hDummy;
	/// <summary>The lockable copy textures left/right (D3D9).	</summary>
	IDirect3DTexture9* m_pcTex9Copy[2];
	/// <summary>The shared D3D11 textures.</summary>
	ID3D11Texture2D* m_pcSharedTexture[2];
	/// <summary>Input texture SR views left/right.</summary>
	ID3D11Texture2D* m_pcTexCopy11[2];
	/// <summary>Input texture SR views left/right.</summary>
	ID3D11ShaderResourceView* m_pcTexCopy11SRV[2];
#pragma endregion
#pragma region VireioCinema D3D11 private fields
	/// <summary>Back buffer copy texture for mono cinema screen.</summary>
	ID3D11Texture2D* m_pcBackBufferCopy;
	/// <summary>Back buffer copy texture SRV for mono cinema screen.</summary>
	ID3D11ShaderResourceView* m_pcBackBufferCopySR;
	/// <summary>Basic sampler state</summary>
	ID3D11SamplerState* m_pcSamplerState;
	/// <summary>The 3D vertex shader for the openVR models.</summary>
	ID3D11VertexShader* m_pcVSGeometry11;
	/// <summary>The 3D pixel shader for the screen.</summary>
	ID3D11PixelShader* m_pcPSGeometry11;
	/// <summary>The 3D pixel shader for the menu.</summary>
	ID3D11PixelShader* m_pcPSMenuScreen11;
	/// <summary>The 3D vertex layout for the openVR models.</summary>
	ID3D11InputLayout* m_pcVLGeometry11;
	/// <summary>The depth stencil DX11 left/right.</summary>
	ID3D11Texture2D* m_pcDSGeometry11[2];
	/// <summary>The depth stencil view DX11.</summary>
	ID3D11DepthStencilView* m_pcDSVGeometry11[2];
	/// <summary>The d3d11 sampler.</summary>
	ID3D11SamplerState* m_pcSampler11;
	/// <summary>Blend state for alpha blending.</summary>
	ID3D11BlendState* m_pcBlendState;
	/// <summary>Constant buffer data structure.</summary>
	GeometryConstantBuffer m_sGeometryConstants;
	/// <summary>The constant buffer for geometry shaders.</summary>
	ID3D11Buffer* m_pcConstantBufferGeometry;
	/// <summary>Current view matrix.</summary>
	D3DXMATRIX m_sView;
	/// <summary>Current eye pose matrix left/right.</summary>
	D3DXMATRIX m_sToEye[2];
	/// <summary>Current projection matrix left/right.</summary>
	D3DXMATRIX m_sProj[2];
	/// <summary>Vector of all models to render.</summary>
	std::vector<RenderModel_D3D11> m_asRenderModels;
	/// <summary>Indices of render models.</summary>
	UINT m_unScreenModelIndex, m_unFloorModelIndex, m_unCeilingModelIndex, m_unWallFModelIndex, m_unWallBModelIndex, m_unWallLModelIndex, m_unWallRModelIndex, m_unMenuModelIndex;
	/// <summary>Default rasterizer state.</summary>
	ID3D11RasterizerState* m_pcRS;
	/// <summary>Shader resource view placeholders left/right.</summary>
	ID3D11ShaderResourceView* m_apcTex11InputSRV[2];	
#pragma endregion
	/// <summary>D3D version used in game</summary>
	enum D3DVersion
	{
		D3D_Undefined,
		D3D_9,
		D3D_10,
		D3D_11,
	} m_eD3DVersion;
	/// <summary>Render FOV aspect ratio.</summary>
	float m_fAspectRatio;
	/// <summary>Max tick count to show mouse when not moved.</summary>
	UINT m_unMouseTickCount;
	/// <summary>
	/// Cinema room setup structure.
	/// </summary>
	struct CinemaRoomSetup
	{
		D3DXCOLOR sColorAmbient;
		D3DXCOLOR sColorDiffuse;
		D3DXVECTOR4 sLightDirection;
		float fGamma;

		enum PixelShaderFX_Screen
		{
			Screen_GeometryDiffuseTexturedMouse,  /**< TexturedNormalVertex : simple lighting, draws mouse laser pointer **/
			Screen_Filter,                        /**< TexturedVertex : "Filter" by kbjwes77 **/
			Screen_Sharpen,                       /**< TexturedVertex : "Sharpen" by KimWest **/
			Screen_LevelFilter,                   /**< TexturedVertex : "Level Filter" by sepehr **/
			Screen_NumberOfFX
		} ePixelShaderFX_Screen;

		std::string FX_AsString(PixelShaderFX_Screen eIx)
		{
			switch (eIx)
			{
				case VireioCinema::CinemaRoomSetup::Screen_GeometryDiffuseTexturedMouse:
					return std::string("Default Screen");
				case VireioCinema::CinemaRoomSetup::Screen_Filter:
					return std::string("Filter by kbjwes77");
				case VireioCinema::CinemaRoomSetup::Screen_Sharpen:
					return std::string("Sharpen by KimWest");
				case VireioCinema::CinemaRoomSetup::Screen_LevelFilter:
					return std::string("Level Filter by sepehr");
				default:
					return std::string("User FX");
			}
		}

		PixelShaderTechnique GetTechnique(PixelShaderFX_Screen eIx)
		{
			switch (eIx)
			{
				case VireioCinema::CinemaRoomSetup::Screen_GeometryDiffuseTexturedMouse:
					return PixelShaderTechnique::GeometryDiffuseTexturedMouse;
				case VireioCinema::CinemaRoomSetup::Screen_Filter:
					return PixelShaderTechnique::ScreenFilter;
				case VireioCinema::CinemaRoomSetup::Screen_Sharpen:
					return PixelShaderTechnique::ScreenSharpen;
				case VireioCinema::CinemaRoomSetup::Screen_LevelFilter:
					return PixelShaderTechnique::ScreenLevelFilter;
			}
			return PixelShaderTechnique::GeometryDiffuseTexturedMouse;
		}

		enum PixelShaderFX_Wall
		{
			Wall_StringTheory,              /**< TexturedNormalVertex : "String Theory" effect from shadertoy.com **/
			Wall_Bubbles,                   /**< TexturedNormalVertex : "Bubbles!" effect from shadertoy.com **/
			Wall_ToonCloud,                 /**< TexturedNormalVertex : "Toon Cloud" effect from shadertoy.com **/
			Wall_Worley01,                  /**< TexturedNormalVertex : "Worley Algorithm (Cell Noise )" effect from shadertoy.com **/
			Wall_WaterCaustic,              /**< TexturedNormalVertex : "Tileable Water Caustic" effect from shadertoy.com **/
			Wall_Planets,                   /**< TexturedNormalVertex : "Planets" effect from shadertoy.com **/
			Wall_VoronoiSmooth,             /**< TexturedNormalVertex : "Voronoi smooth" effect from shadertoy.com **/
			Wall_NumberOfFX
		} ePixelShaderFX_Wall_FB[2], ePixelShaderFX_Wall_LR[2];

		std::string FX_AsString(PixelShaderFX_Wall eIx)
		{
			switch (eIx)
			{
				case VireioCinema::CinemaRoomSetup::Wall_StringTheory:
					return std::string("String Theory by nimitz");
				case VireioCinema::CinemaRoomSetup::Wall_Bubbles:
					return std::string("Bubbles! by iq & weyland");
				case VireioCinema::CinemaRoomSetup::Wall_ToonCloud:
					return std::string("Toon Cloud by AntoineC");
				case VireioCinema::CinemaRoomSetup::Wall_Worley01:
					return std::string("Worley Algorithm by Yeis");
				case VireioCinema::CinemaRoomSetup::Wall_WaterCaustic:
					return std::string("Tileable Water Caustic by Dave_Hoskins");
				case VireioCinema::CinemaRoomSetup::Wall_Planets:
					return std::string("Museum of random Planets by LukeRissacher");
				case VireioCinema::CinemaRoomSetup::Wall_VoronoiSmooth:
					return std::string("Voronoi Smooth by iq");
				default:
					return std::string("User FX");
					break;
			}
		}

		PixelShaderTechnique GetTechnique(PixelShaderFX_Wall eIx)
		{
			switch (eIx)
			{
				case CinemaRoomSetup::Wall_Bubbles:
					return PixelShaderTechnique::Bubbles;
				case VireioCinema::CinemaRoomSetup::Wall_Planets:
					return PixelShaderTechnique::Planets;
				case CinemaRoomSetup::Wall_StringTheory:
					return PixelShaderTechnique::StringTheory;
				case CinemaRoomSetup::Wall_ToonCloud:
					return PixelShaderTechnique::ToonCloud;
				case CinemaRoomSetup::Wall_WaterCaustic:
					return PixelShaderTechnique::WaterCaustic;
				case CinemaRoomSetup::Wall_Worley01:
					return PixelShaderTechnique::Worley01;
				case VireioCinema::CinemaRoomSetup::Wall_VoronoiSmooth:
					return PixelShaderTechnique::VoronoiSmooth;
			}
			return PixelShaderTechnique::Bubbles;
		}

		enum PixelShaderFX_Floor
		{
			Floor_StringTheory,              /**< TexturedNormalVertex : "String Theory" effect from shadertoy.com **/
			Floor_Bubbles,                   /**< TexturedNormalVertex : "Bubbles!" effect from shadertoy.com **/
			Floor_C64Plasma,                 /**< TexturedNormalVertex : "C64 plasma" effect from shadertoy.com **/
			Floor_Worley01,                  /**< TexturedNormalVertex : "Worley Algorithm (Cell Noise )" effect from shadertoy.com **/
			Floor_WaterCaustic,              /**< TexturedNormalVertex : "Tileable Water Caustic" effect from shadertoy.com **/
			Floor_Planets,                   /**< TexturedNormalVertex : "Planets" effect from shadertoy.com **/
			Floor_HypnoticDisco,             /**< TexturedNormalVertex : "Hypnotic Disco" effect from shadertoy.com **/
			Floor_VoronoiSmooth,             /**< TexturedNormalVertex : "Voronoi smooth" effect from shadertoy.com **/
			Floor_NumberOfFX
		} ePixelShaderFX_Floor[2];

		std::string FX_AsString(PixelShaderFX_Floor eIx)
		{
			switch (eIx)
			{
				case VireioCinema::CinemaRoomSetup::Floor_StringTheory:
					return std::string("String Theory by nimitz");
				case VireioCinema::CinemaRoomSetup::Floor_Bubbles:
					return std::string("Bubbles! by iq & weyland");
				case VireioCinema::CinemaRoomSetup::Floor_C64Plasma:
					return std::string("C64 Plasma by ssdsa");
				case VireioCinema::CinemaRoomSetup::Floor_Worley01:
					return std::string("Worley Algorithm by Yeis");
				case VireioCinema::CinemaRoomSetup::Floor_WaterCaustic:
					return std::string("Tileable Water Caustic by Dave_Hoskins");
				case VireioCinema::CinemaRoomSetup::Floor_Planets:
					return std::string("Museum of random Planets by LukeRissacher");
				case VireioCinema::CinemaRoomSetup::Floor_HypnoticDisco:
					return std::string("Hypnotic Disco by xbe");
				case VireioCinema::CinemaRoomSetup::Floor_VoronoiSmooth:
					return std::string("Voronoi Smooth by iq");
				default:
					return std::string("User FX");
					break;
			}

		}

		PixelShaderTechnique GetTechnique(PixelShaderFX_Floor eIx)
		{
			switch (eIx)
			{
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Bubbles:
					return PixelShaderTechnique::Bubbles;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_C64Plasma:
					return PixelShaderTechnique::C64Plasma;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_StringTheory:
					return PixelShaderTechnique::StringTheory;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_WaterCaustic:
					return PixelShaderTechnique::WaterCaustic;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Worley01:
					return PixelShaderTechnique::Worley01;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_Planets:
					return PixelShaderTechnique::Planets;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_HypnoticDisco:
					return PixelShaderTechnique::HypnoticDisco;
				case CinemaRoomSetup::PixelShaderFX_Floor::Floor_VoronoiSmooth:
					return PixelShaderTechnique::VoronoiSmooth;
			}
			return PixelShaderTechnique::Bubbles;
		}

		struct PixelShaderFX_Colors
		{
			D3DXCOLOR sColorFX[8];               /**< Colors to be used by a shader effect **/
		} sColors_Screen;                        /**< Colors to be used by the screen effect **/
		PixelShaderFX_Colors sColors_Wall_FB[2]; /**< Colors to be used by the wall (front+back) effects **/
		PixelShaderFX_Colors sColors_Wall_LR[2]; /**< Colors to be used by the wall (left+right) effects **/
		PixelShaderFX_Colors sColors_Floor[2];   /**< Colors to be used by the floor (top+bottom) effects **/

		float fScreenWidth;        /**< The width of the cinema screen, in physical meters. */
		float fScreenLevel;        /**< The vertical level of the cinema center, in physical meters. */
		float fScreenDepth;        /**< The depth of the cinema screen, in physical meters. */
		float fScreenRotateX;      /**< The rotation angle (x) of the screen, in degree. */
		float fScreenRotateY;      /**< The rotation angle (y) of the screen, in degree. */
		float fMenuScreenDepth;    /**< The depth of the menu screen, in physical meters. */
		float fMenuScreenScale;    /**< The scale of the menu screen. */
		float fMenuScreenRotateY;  /**< The rotation angle (y) of the menu screen, in degree. */
		float fRoomScale;          /**< The scale of the gaming room. */

		bool bMenuIsHUD;           /**< True if the menu is stick to the HMD yaw angle. */
		bool bPerformanceMode;     /**< True if performance mode is on ***/
		bool bImmersiveMode;       /**< True if full immersive mode is on ***/
	} m_sCinemaRoomSetup;
	/// <summary>User settings for immersive fullscreen rendering.</summary>
	struct ImmersiveFullscreenSettings
	{
		float fIPD;                    /**< Interpupillary distance, to be read from Stereo Presenter node **/
		float fVSD;                    /**< Virtual screen distance, to be read from Stereo Presenter node **/
	} m_sImmersiveFullscreenSettings;
	/// <summary>Cinema in-game menu.</summary>
	VireioSubMenu m_sMenu;
	/// <summary>Frames to save the ini file.</summary>
	INT m_nIniFrameCount;
};

/// <summary>
/// Exported Constructor Method.
/// </summary>
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	VireioCinema* pVireioCinema = new VireioCinema();
	return static_cast<AQU_Nodus*>(pVireioCinema);
}
