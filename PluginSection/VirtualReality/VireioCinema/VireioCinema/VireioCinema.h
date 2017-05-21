/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin
Copyright (C) 2014 Denis Reischl

File <VireioCinema.h> and
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
#include"..\..\..\Include\VireioMenu.h"

#define INT_PLUG_TYPE                                  7 
#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_VECTOR3F_PLUG_TYPE                      2061
#define PNT_IDIRECT3DTEXTURE9_PLUG_TYPE             2048

#define NUMBER_OF_COMMANDERS                           4
#define NUMBER_OF_DECOMMANDERS                        22

/**
* Node Commander Enumeration.
***/
enum VRC_Commanders
{
	LeftTexture11,
	RightTexture11,
	MenuTexture,
	VireioMenu
};

/**
* Node Decommander Enumeration.
***/
enum VRC_Decommanders
{
	LeftTexture11_In,
	RightTexture11_In,
	LeftTexture10_In,
	RightTexture10_In,
	LeftTexture9_In,
	RightTexture9_In,
	Pitch,
	Yaw,
	Roll,
	OrientationW,
	OrientationX,
	OrientationY,
	OrientationZ,
	PositionX,
	PositionY,
	PositionZ,
	View,
	World,
	ResolutionWidth,
	ResolutionHeight,
	ProjectionLeft,
	ProjectionRight,
};


/**
* Small helper to convert D3D9 format to DXGI.
**/
inline DXGI_FORMAT GetDXGI_Format(D3DFORMAT eInputFormat)
{
	switch (eInputFormat)
	{
		case D3DFMT_UNKNOWN:
			return	DXGI_FORMAT_UNKNOWN;
		case D3DFMT_R8G8B8:
			return DXGI_FORMAT_UNKNOWN; //	Not available
		case D3DFMT_A8R8G8B8:
			return DXGI_FORMAT_B8G8R8A8_UNORM; // or DXGI_FORMAT_B8G8R8A8_UNORM_SRGB¹
		case D3DFMT_X8R8G8B8:
			return DXGI_FORMAT_B8G8R8X8_UNORM; // or DXGI_FORMAT_B8G8R8X8_UNORM_SRGB¹
		case D3DFMT_R5G6B5:
			return DXGI_FORMAT_B5G6R5_UNORM;
		case D3DFMT_X1R5G5B5:
			return DXGI_FORMAT_UNKNOWN; //Not available
		case D3DFMT_A1R5G5B5:
			return DXGI_FORMAT_B5G5R5A1_UNORM;
		case D3DFMT_A4R4G4B4:
			return DXGI_FORMAT_B4G4R4A4_UNORM;
		case D3DFMT_R3G3B2:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_A8:
			return DXGI_FORMAT_A8_UNORM;
		case D3DFMT_A8R3G3B2:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_X4R4G4B4:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_A2B10G10R10:
			// DXGI_FORMAT_R10G10B10A2 -> dropped since DX10 ??
			return DXGI_FORMAT_UNKNOWN;
		case D3DFMT_A8B8G8R8:
			return	DXGI_FORMAT_R8G8B8A8_UNORM; // or DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
		case D3DFMT_X8B8G8R8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_G16R16:
			return DXGI_FORMAT_R16G16_UNORM;
		case D3DFMT_A2R10G10B10:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_A16B16G16R16:
			return	DXGI_FORMAT_R16G16B16A16_UNORM;
		case D3DFMT_A8P8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_P8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_L8:
			// Note : Use.r swizzle in shader to duplicate red to other components to get D3D9 behavior.
			return	DXGI_FORMAT_R8_UNORM;
		case D3DFMT_A8L8:
			// Note : Use swizzle.rrrg in shader to duplicate red and move green to the alpha components to get D3D9 behavior.
			return	DXGI_FORMAT_R8G8_UNORM;
		case D3DFMT_A4L4:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_V8U8:
			return	DXGI_FORMAT_R8G8_SNORM;
		case D3DFMT_L6V5U5:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_X8L8V8U8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_Q8W8V8U8:
			return	DXGI_FORMAT_R8G8B8A8_SNORM;
		case D3DFMT_V16U16:
			return	DXGI_FORMAT_R16G16_SNORM;
		case D3DFMT_A2W10V10U10:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_UYVY:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_R8G8_B8G8:
			// (in DX9 the data was scaled up by 255.0f, but this can be handled in shader code).
			return	DXGI_FORMAT_G8R8_G8B8_UNORM;
		case D3DFMT_YUY2:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_G8R8_G8B8:
			// (in DX9 the data was scaled up by 255.0f, but this can be handled in shader code).
			return	DXGI_FORMAT_R8G8_B8G8_UNORM;
		case D3DFMT_DXT1:
			return	DXGI_FORMAT_BC1_UNORM; // or DXGI_FORMAT_BC1_UNORM_SRGB
		case (D3DFORMAT)76: // = D3DFMT_X8D24:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case (D3DFORMAT)78: // = D3DFMT_X4S4D24:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_DXT2:
			//  Note : DXT3 and DXT4 are the same from an API / hardware perspective... 
			//         only difference was 'premultiplied alpha', which can be tracked by 
			//         an application and doesn't need a separate format.
			return	DXGI_FORMAT_BC1_UNORM; // or DXGI_FORMAT_BC1_UNORM_SRGB; 
		case D3DFMT_DXT3:
			return	DXGI_FORMAT_BC2_UNORM; // or DXGI_FORMAT_BC2_UNORM_SRGB;
		case D3DFMT_DXT4:
			//  Note : DXT3 and DXT4 are the same from an API / hardware perspective... 
			//         only difference was 'premultiplied alpha', which can be tracked by 
			//         an application and doesn't need a separate format.
			return	DXGI_FORMAT_BC2_UNORM; // or DXGI_FORMAT_BC2_UNORM_SRGB; 
		case D3DFMT_DXT5:
			return	DXGI_FORMAT_BC3_UNORM; // or DXGI_FORMAT_BC3_UNORM_SRGB;
		case D3DFMT_D16_LOCKABLE:
			return DXGI_FORMAT_D16_UNORM;
		case D3DFMT_D32:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D15S1:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D24S8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D24X8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D24X4S4:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D16:
			return	DXGI_FORMAT_D16_UNORM;
		case D3DFMT_D32F_LOCKABLE:
			return	DXGI_FORMAT_D32_FLOAT;
		case D3DFMT_D24FS8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_D32_LOCKABLE:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_S8_LOCKABLE:
			return	DXGI_FORMAT_D24_UNORM_S8_UINT;
		case D3DFMT_L16:
			//  Note : Use.r swizzle in shader to duplicate red to other components to get D3D9 behavior.
			return	DXGI_FORMAT_R16_UNORM;
		case D3DFMT_VERTEXDATA:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_INDEX16:
			return	DXGI_FORMAT_R16_UINT;
		case D3DFMT_INDEX32:
			return	DXGI_FORMAT_R32_UINT;
		case D3DFMT_Q16W16V16U16:
			return	DXGI_FORMAT_R16G16B16A16_SNORM;
		case D3DFMT_MULTI2_ARGB8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_R16F:
			return	DXGI_FORMAT_R16_FLOAT;
		case D3DFMT_G16R16F:
			return	DXGI_FORMAT_R16G16_FLOAT;
		case D3DFMT_A16B16G16R16F:
			return	DXGI_FORMAT_R16G16B16A16_FLOAT;
		case D3DFMT_R32F:
			return	DXGI_FORMAT_R32_FLOAT;
		case D3DFMT_G32R32F:
			return	DXGI_FORMAT_R32G32_FLOAT;
		case D3DFMT_A32B32G32R32F:
			return	DXGI_FORMAT_R32G32B32A32_FLOAT;
		case D3DFMT_CxV8U8:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_A1:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_A2B10G10R10_XR_BIAS:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_BINARYBUFFER:
			return DXGI_FORMAT_UNKNOWN; // Not available
		case D3DFMT_FORCE_DWORD:
			return DXGI_FORMAT_UNKNOWN; // Not available
		default:
			return DXGI_FORMAT_UNKNOWN; // Not available
			// D3DFMT_W11V11U10	Not available
			// D3DDECLTYPE_FLOAT1	DXGI_FORMAT_R32_FLOAT
			/*D3DDECLTYPE_FLOAT2	DXGI_FORMAT_R32G32_FLOAT
			D3DDECLTYPE_FLOAT3	DXGI_FORMAT_R32G32B32_FLOAT
			D3DDECLTYPE_FLOAT4	DXGI_FORMAT_R32G32B32A32_FLOAT
			D3DDECLTYPED3DCOLOR	Not available
			D3DDECLTYPE_UBYTE4	DXGI_FORMAT_R8G8B8A8_UINT Note : Shader gets UINT values, but if Direct3D 9 style integral floats are needed(0.0f, 1.0f... 255.f), UINT can just be converted to float32 in shader.
			D3DDECLTYPE_SHORT2	DXGI_FORMAT_R16G16_SINT Note : Shader gets SINT values, but if Direct3D 9 style integral floats are needed, SINT can just be converted to float32 in shader.
			D3DDECLTYPE_SHORT4	DXGI_FORMAT_R16G16B16A16_SINT Note : Shader gets SINT values, but if Direct3D 9 style integral floats are needed, SINT can just be converted to float32 in shader.
			D3DDECLTYPE_UBYTE4N	DXGI_FORMAT_R8G8B8A8_UNORM
			D3DDECLTYPE_SHORT2N	DXGI_FORMAT_R16G16_SNORM
			D3DDECLTYPE_SHORT4N	DXGI_FORMAT_R16G16B16A16_SNORM
			D3DDECLTYPE_USHORT2N	DXGI_FORMAT_R16G16_UNORM
			D3DDECLTYPE_USHORT4N	DXGI_FORMAT_R16G16B16A16_UNORM
			D3DDECLTYPE_UDEC3	Not available
			D3DDECLTYPE_DEC3N	Not available
			D3DDECLTYPE_FLOAT16_2	DXGI_FORMAT_R16G16_FLOAT
			D3DDECLTYPE_FLOAT16_4	DXGI_FORMAT_R16G16B16A16_FLOAT
			FourCC 'ATI1'	DXGI_FORMAT_BC4_UNORM
			FourCC 'ATI2'	DXGI_FORMAT_BC5_UNORM*/
			break;
	}
}

/**
* Virtual Reality Cinema Node Plugin (Direct3D 9).
***/
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
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
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

#pragma region VireioCinema D3D9/D3D10 private fields
	/**
	* D3D11 device to be used in D3D9/D3D10 games.
	***/
	ID3D11Device* m_pcD3D11Device;
	/**
	* D3D11 device context to be used in D3D9/D3D10 games.
	***/
	ID3D11DeviceContext* m_pcD3D11Context;
	/**
	* Dummy window for the d3d11 device.
	***/
	HWND m_hDummy;
	/**
	* The input textures left/right (D3D9).
	***/
	IDirect3DTexture9** m_ppcTex9Input[2];
	/**
	* The lockable copy textures left/right (D3D9).
	***/
	IDirect3DTexture9* m_pcTex9Copy[2];
	/**
	* The shared D3D11 textures.
	***/
	ID3D11Texture2D* m_pcSharedTexture[2];
	/**
	* Input texture SR views left/right.
	***/
	ID3D11Texture2D* m_pcTexCopy11[2];
	/**
	* Input texture SR views left/right.
	***/
	ID3D11ShaderResourceView* m_pcTexCopy11SRV[2];
#pragma endregion
#pragma region VireioCinema D3D11 private fields
	/**
	* Drawing textures left/right.
	***/
	ID3D11Texture2D* m_pcTex11Draw[2];
	/**
	* Drawing texture RT views left/right.
	***/
	ID3D11RenderTargetView* m_pcTex11DrawRTV[2];
	/**
	* Drawing texture SR views left/right.
	***/
	ID3D11ShaderResourceView* m_pcTex11DrawSRV[2];
	/**
	* Input texture SR views left/right.
	***/
	ID3D11ShaderResourceView** m_ppcTex11InputSRV[2];
	/**
	* Back buffer copy texture for mono cinema screen.
	***/
	ID3D11Texture2D* m_pcBackBufferCopy;
	/**
	* Back buffer copy texture SRV for mono cinema screen.
	***/
	ID3D11ShaderResourceView* m_pcBackBufferCopySR;
	/**
	* Basic sampler state.
	***/
	ID3D11SamplerState* m_pcSamplerState;
	/**
	* The 3D vertex shader for the openVR models.
	***/
	ID3D11VertexShader* m_pcVSGeometry11;
	/**
	* The 3D pixel shader for the screen.
	***/
	ID3D11PixelShader* m_pcPSGeometry11;
	/**
	* The 3D pixel shader for the menu.
	***/
	ID3D11PixelShader* m_pcPSMenuScreen11;
	/**
	* The 3D vertex layout for the openVR models.
	***/
	ID3D11InputLayout* m_pcVLGeometry11;
	/**
	* The depth stencil DX11 left/right.
	***/
	ID3D11Texture2D* m_pcDSGeometry11[2];
	/**
	* The depth stencil view DX11.
	***/
	ID3D11DepthStencilView* m_pcDSVGeometry11[2];
	/**
	* The d3d11 sampler.
	***/
	ID3D11SamplerState* m_pcSampler11;
	/**
	* Blend state for alpha blending.
	***/
	ID3D11BlendState* m_pcBlendState;
	/**
	* Constant buffer data structure.
	***/
	GeometryConstantBuffer m_sGeometryConstants;
	/**
	* The constant buffer for geometry shaders.
	***/
	ID3D11Buffer* m_pcConstantBufferGeometry;
	/**
	* Current view matrix.
	***/
	D3DXMATRIX m_sView;
	/**
	* Current eye pose matrix left/right.
	***/
	D3DXMATRIX m_sToEye[2];
	/**
	* Current projection matrix left/right.
	***/
	D3DXMATRIX m_sProj[2];
	/**
	* Vector of all models to render.
	***/
	std::vector<RenderModel_D3D11> m_asRenderModels;
	/**
	* Indices of render models.
	***/
	UINT m_unScreenModelIndex, m_unFloorModelIndex, m_unCeilingModelIndex, m_unWallFModelIndex, m_unWallBModelIndex, m_unWallLModelIndex, m_unWallRModelIndex, m_unMenuModelIndex;
	/**
	* Default rasterizer state.
	***/
	ID3D11RasterizerState* m_pcRS;
	/**
	* Shader resource view placeholders left/right.
	***/
	ID3D11ShaderResourceView* m_apcTex11InputSRV[2];
	/**
	* Menu texture.
	***/
	ID3D11Texture2D* m_pcTexMenu;
	/**
	* Menu texture srv.
	***/
	ID3D11ShaderResourceView* m_pcTexMenuSRV;
	/**
	* Menu texture rtv.
	***/
	ID3D11RenderTargetView* m_pcTexMenuRTV;
#pragma endregion
	/**
	* D3D version used in game
	**/
	enum D3DVersion
	{
		D3D_Undefined,
		D3D_9,
		D3D_10,
		D3D_11,
	} m_eD3DVersion;
	/**
	* Yaw angle pointer.
	***/
	float* m_pfYaw;
	/**
	* Pitch angle pointer.
	***/
	float* m_pfPitch;
	/**
	* Roll angle pointer.
	***/
	float* m_pfRoll;
	/**
	* Yaw angle.
	***/
	float m_fYaw;
	/**
	* Pitch angle.
	***/
	float m_fPitch;
	/**
	* Roll angle.
	***/
	float m_fRoll;
	/**
	* Position X pointer.
	***/
	float* m_pfPositionX;
	/**
	* Position Y pointer.
	***/
	float* m_pfPositionY;
	/**
	* Position X pointer.
	***/
	float* m_pfPositionZ;
	/**
	* Projection pointers left/right.
	***/
	D3DMATRIX* m_psProjection[2];
	/**
	* View pointers left/right.
	***/
	D3DMATRIX* m_psView;
	/**
	* Position in cinema.
	***/
	D3DVECTOR m_sPositionVector;
	/**
	* Texture resolution width.
	* Each stereo output texture will have this width.
	***/
	UINT32* m_punTexResolutionWidth;
	/**
	* Texture resolution Height.
	* Each stereo output texture will have this height.
	***/
	UINT32* m_punTexResolutionHeight;
	/**
	* Render FOV aspect ratio.
	***/
	float m_fAspectRatio;
	/**
	* Max tick count to show mouse when not moved.
	***/
	UINT m_unMouseTickCount;
	/**
	* Cinema room setup structure.
	***/
	struct CinemaRoomSetup
	{
		D3DXCOLOR sColorAmbient;
		D3DXCOLOR sColorDiffuse;
		D3DXVECTOR4 sLightDirection;
		float fGamma;

		enum PixelShaderFX_Screen
		{
			Screen_GeometryDiffuseTexturedMouse,  /**< TexturedNormalVertex : simple lighting, draws mouse laser pointer **/
			Screen_NumberOfFX
		} ePixelShaderFX_Screen;

		std::string FX_AsString(PixelShaderFX_Screen eIx)
		{
			switch (eIx)
			{
				case VireioCinema::CinemaRoomSetup::Screen_GeometryDiffuseTexturedMouse:
					return std::string("Default Screen");
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
	/**
	* User settings for immersive fullscreen rendering.
	***/
	struct ImmersiveFullscreenSettings
	{
		float fIPD;                    /**< Interpupillary distance, to be read from Stereo Presenter node **/
		float fVSD;                    /**< Virtual screen distance, to be read from Stereo Presenter node **/
	} m_sImmersiveFullscreenSettings;
	/**
	* Cinema menu.
	***/
	VireioSubMenu m_sMenu;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	VireioCinema* pVireioCinema = new VireioCinema();
	return static_cast<AQU_Nodus*>(pVireioCinema);
}
