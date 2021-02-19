/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter.h> and
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

#define VIREIO_D3D9
#pragma region include
#include<stdio.h>
#include<vector>
#include<time.h> 

#include"..//..//..//..//Aquilinus//Aquilinus//AQU_Nodus.h"
#include"Resources.h"

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <D3DX11tex.h>
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

#include"..//..//..//..//Aquilinus/Aquilinus/ITA_D3D9Interfaces.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DDevice9.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DSwapchain9.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DStateBlock9.h"
#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"VireioStereoSplitter_Proxy.h"
#include"D3D9Ex_D3DX9_Methods.h"

#pragma endregion

#pragma region defines
#define SMALL_FLOAT 0.001f
#define	SLIGHTLY_LESS_THAN_ONE 0.999f

#define PI 3.141592654
#define RADIANS_TO_DEGREES(fRad) ((float) fRad * (float) (180.0 / PI))

#define STEREO_L 0
#define STEREO_R 1

#define D3DFMT_ATI1N ((D3DFORMAT)MAKEFOURCC('A','T','I','1'))
#define D3DFMT_ATI2N ((D3DFORMAT)MAKEFOURCC('A','T','I','2'))
#define D3DFMT_AYUV ((D3DFORMAT)MAKEFOURCC('A','Y','U','V'))
#define D3DFMT_DF16 ((D3DFORMAT)MAKEFOURCC('D','F','1','6'))
#define D3DFMT_DF24 ((D3DFORMAT)MAKEFOURCC('D','F','2','4'))
#define D3DFMT_INTZ ((D3DFORMAT)MAKEFOURCC('I','N','T','Z'))
#define D3DFMT_NULL ((D3DFORMAT)MAKEFOURCC('N','U','L','L'))
#define D3DFMT_NV12 ((D3DFORMAT)MAKEFOURCC('N','V','1','2'))
#define D3DFMT_YV12 ((D3DFORMAT)MAKEFOURCC('Y','V','1','2'))
#define D3DFMT_RAWZ ((D3DFORMAT)MAKEFOURCC('R','A','W','Z'))
#define D3DFMT_RESZ ((D3DFORMAT)MAKEFOURCC('R','E','S','Z'))
#define FOURCC_NULL ((D3DFORMAT)(MAKEFOURCC('N','U','L','L')))
#define FOURCC_INTZ ((D3DFORMAT)(MAKEFOURCC('I','N','T','Z')))
#define FOURCC_ATI1N ((D3DFORMAT)MAKEFOURCC('A', 'T', 'I', '1'))
#define FOURCC_ATI2N ((D3DFORMAT)MAKEFOURCC('A', 'T', 'I', '2'))
#define RESZ_CODE 0x7fa05000

#define NUMBER_OF_COMMANDERS                           1
#define NUMBER_OF_DECOMMANDERS                        46

#define MAX_DATA_SIZE                              65535                     /**< Arbitrary... TODO !! set a maximum node data size **/

#define NO_PRIVATE_INTERFACE             (IUnknown*)0xf1                     /**< Arbitrary value set instead of an IUnknown interface as "verified" indicator. ***/
#pragma endregion

/// <summary>
/// Maximum simultaneous textures : 16 {shader sampling stage registers: s0 to s15}
/// </summary>
#define D3D9_SIMULTANEAOUS_TEXTURE_COUNT                16
/// <summary>
/// Maximum simultaneous displacement textures : 5 {D3DDMAPSAMPLER+D3DVERTEXTEXTURESAMPLER0..3}
/// </summary>
#define D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT   5
/// <summary>
/// Maximum simultaneous render targets ?
/// </summary>
#define D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT            D3D_FL9_3_SIMULTANEOUS_RENDER_TARGET_COUNT 

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum class STS_Commanders : int
{
	StereoData_Splitter
};

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum class STS_Decommanders : int
{
	Modifier,
	CreateAdditionalSwapchain,
	GetSwapChain,
	Reset,
	Present,
	GetBackBuffer,
	CreateTexture,
	CreateVolumeTexture,
	CreateCubeTexture,
	CreateVertexBuffer,
	CreateIndexBuffer,
	CreateRenderTarget,
	CreateDepthStencilSurface,
	UpdateSurface,
	UpdateTexture,
	GetRenderTargetData,
	GetFrontBufferData,
	StretchRect,
	ColorFill,
	CreateOffscreenPlainSurface,
	SetRenderTarget,
	GetRenderTarget,
	SetDepthStencilSurface,
	GetDepthStencilSurface,
	BeginScene,
	EndScene,
	Clear,
	SetViewport,
	SetRenderState,
	GetTexture,
	SetTexture,
	DrawPrimitive,
	DrawIndexedPrimitive,
	DrawPrimitiveUP,
	DrawIndexedPrimitiveUP,
	ProcessVertices,
	SetStreamSource,
	GetStreamSource,
	SetIndices,
	GetIndices,
	DrawRectPatch,
	DrawTriPatch,
	SC_Present,
	SC_GetFrontBufferData,
	SC_GetBackBuffer,
	SB_Apply
};

/// <summary>
/// Methods table of D3DX9 methods.
/// Provided method ids by Aquilinus.
/// </summary>
enum MT_D3DX9
{
	D3D9_D3DXLoadSurfaceFromFileA,
	D3D9_D3DXLoadSurfaceFromFileW,
	D3D9_D3DXLoadSurfaceFromResourceA,
	D3D9_D3DXLoadSurfaceFromResourceW,
	D3D9_D3DXLoadSurfaceFromFileInMemory,
	D3D9_D3DXLoadSurfaceFromSurface,
	D3D9_D3DXLoadSurfaceFromMemory,
	D3D9_D3DXSaveSurfaceToFileA,
	D3D9_D3DXSaveSurfaceToFileW,
	D3D9_D3DXSaveSurfaceToFileInMemory,
	D3D9_D3DXLoadVolumeFromFileA,
	D3D9_D3DXLoadVolumeFromFileW,
	D3D9_D3DXLoadVolumeFromResourceA,
	D3D9_D3DXLoadVolumeFromResourceW,
	D3D9_D3DXLoadVolumeFromFileInMemory,
	D3D9_D3DXLoadVolumeFromVolume,
	D3D9_D3DXLoadVolumeFromMemory,
	D3D9_D3DXSaveVolumeToFileA,
	D3D9_D3DXSaveVolumeToFileW,
	D3D9_D3DXSaveVolumeToFileInMemory,
	D3D9_D3DXCreateTexture,
	D3D9_D3DXCreateCubeTexture,
	D3D9_D3DXCreateVolumeTexture,
	D3D9_D3DXCreateTextureFromFileA,
	D3D9_D3DXCreateTextureFromFileW,
	D3D9_D3DXCreateCubeTextureFromFileA,
	D3D9_D3DXCreateCubeTextureFromFileW,
	D3D9_D3DXCreateVolumeTextureFromFileA,
	D3D9_D3DXCreateVolumeTextureFromFileW,
	D3D9_D3DXCreateTextureFromResourceA,
	D3D9_D3DXCreateTextureFromResourceW,
	D3D9_D3DXCreateCubeTextureFromResourceA,
	D3D9_D3DXCreateCubeTextureFromResourceW,
	D3D9_D3DXCreateVolumeTextureFromResourceA,
	D3D9_D3DXCreateVolumeTextureFromResourceW,
	D3D9_D3DXCreateTextureFromFileExA,
	D3D9_D3DXCreateTextureFromFileExW,
	D3D9_D3DXCreateCubeTextureFromFileExA,
	D3D9_D3DXCreateCubeTextureFromFileExW,
	D3D9_D3DXCreateVolumeTextureFromFileExA,
	D3D9_D3DXCreateVolumeTextureFromFileExW,
	D3D9_D3DXCreateTextureFromResourceExA,
	D3D9_D3DXCreateTextureFromResourceExW,
	D3D9_D3DXCreateCubeTextureFromResourceExA,
	D3D9_D3DXCreateCubeTextureFromResourceExW,
	D3D9_D3DXCreateVolumeTextureFromResourceExA,
	D3D9_D3DXCreateVolumeTextureFromResourceExW,
	D3D9_D3DXCreateTextureFromFileInMemory,
	D3D9_D3DXCreateCubeTextureFromFileInMemory,
	D3D9_D3DXCreateVolumeTextureFromFileInMemory,
	D3D9_D3DXCreateTextureFromFileInMemoryEx,
	D3D9_D3DXCreateCubeTextureFromFileInMemoryEx,
	D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx,
	D3D9_D3DXSaveTextureToFileA,
	D3D9_D3DXSaveTextureToFileW,
	D3D9_D3DXSaveTextureToFileInMemory,
	D3D9_D3DXFilterTexture,
	D3D9_D3DXFillTexture,
	D3D9_D3DXFillCubeTexture,
	D3D9_D3DXFillVolumeTexture,
	D3D9_D3DXFillTextureTX,
	D3D9_D3DXFillCubeTextureTX,
	D3D9_D3DXFillVolumeTextureTX,
	D3D9_D3DXComputeNormalMap,
};

/// <summary>
/// Simple false/true enumeration prototype.
/// </summary>
typedef struct _DetermineFalseTrue
{
	enum
	{
		DT_FALSE = 0,
		DT_TRUE = 1,
		DETERMINEFALSETRUE_LAST
	};
} DetermineFalseTrue;

/// <summary>
/// Determines how to save render states for stereo view output.
/// </summary>
enum DetermineSaveRenderStates
{
	DT_STATE_BLOCK = 0,
	DT_SELECTED_STATES_MANUALLY = 1,
	DT_ALL_STATES_MANUALLY = 2,
	DT_DO_NOT_SAVE_AND_RESTORE = 3,
	DETERMINESAVERENDERSTATES_LAST
};

/// <summary>
/// Determines when to duplicate a render target or depth stencil.
/// </summary>
typedef struct _DetermineDuplicateRToDS : public DetermineFalseTrue
{
	enum
	{
		DT_WIDTH_UNEQUAL_HEIGHT = DetermineFalseTrue::DETERMINEFALSETRUE_LAST,
		DETERMINEDUPLICATERTODS_LAST
	};
} DetermineDuplicateRToDS;

/// <summary>
/// Determines when to duplicate a base texture.
/// </summary>
typedef struct _DetermineDuplicateBaseTexture : public DetermineFalseTrue
{
	enum : int
	{
		DT_IF_RENDER_TARGET = DetermineFalseTrue::DETERMINEFALSETRUE_LAST,
		DETERMINEDUPLICATEBASETEXTURE_LAST
	};
} DetermineDuplicateBaseTexture;

/// <summary>
/// Determines when to duplicate a texture.
/// </summary>
typedef struct _DetermineDuplicateTexture : public DetermineDuplicateBaseTexture
{
	enum
	{
		DT_IF_RENDER_TARGET_AND_WIDTH_UNEQUAL_HEIGHT = DetermineDuplicateBaseTexture::DETERMINEDUPLICATEBASETEXTURE_LAST,
		DETERMINEDUPLICATETEXTURE_LAST
	};
} DetermineDuplicateTexture;

/// <summary>
/// Determines when to duplicate a cube texture.
/// </summary>
typedef struct _DetermineDuplicateCubeTexture : public DetermineDuplicateBaseTexture
{
	enum
	{
		DETERMINEDUPLICATECUBETEXTURE_LAST = DetermineDuplicateBaseTexture::DETERMINEDUPLICATEBASETEXTURE_LAST,
	};
} DetermineDuplicateCubeTexture;

/// <summary>
/// Game type settings enumeration.
/// Contains all game specific stereo rendering settings.
/// </summary>
enum GameTypeEntry
{
	StateBlockSaveRestoreType,					// State Block Save/Restore type (DetermineSaveRenderStates enum)
	ShouldDuplicateRenderTarget,                // Should duplicate render target surface mode (0 - false, 1 - true, 2 - Width != height)
	ShouldDuplicateDepthStencil,                // Should duplicate depth stencil surface mode (0 - false, 1 - true, 2 - Width != height)
	ShouldDuplicateTexture,						// Should duplicate texture mode (0 - false, 1 - true, 2 - Is Render Target?, 3 - Is Render Target and Width != Height)
	ShouldDuplicateCubeTexture,					// Should duplicate cube texture mode (0 - false, 1 - true, 2 - Is Render Target?)
	// DeviceStateFlags  TODO !!!               // Bit 1 - Sampler State flags (0 D3DSAMP_SRGBTEXTURE = 0, 1 - D3DSAMP_SRGBTEXTURE = ssSrgb)
	// Bit 0 - Disable Scissor Test state before draw
	GAMETYPEENTRY_LAST
};

/// <summary>
/// => Vireio Stereo Splitter Node Plugin (Direct3D 9).
/// Vireio Perception Stereo Render Target Handler.
/// </summary>
class StereoSplitter : public AQU_Nodus
{
public:
	StereoSplitter(ImGuiContext* sCtx);
	virtual ~StereoSplitter();

	/*** AQU_Nodus public methods ***/
	virtual const char* GetNodeType();
	virtual        UINT GetNodeTypeId();
	virtual      LPWSTR GetCategory();
	virtual     HBITMAP GetLogo();
	virtual      ImVec2 GetNodeSize() { return ImVec2((float)g_uGlobalNodeWidth, (float)128); }
	virtual       DWORD GetSaveDataSize();
	virtual       char* GetSaveData(UINT* pdwSizeOfData);
	virtual        void InitNodeData(char* pData, UINT dwSizeOfData);
	virtual       DWORD GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual       DWORD GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual      LPWSTR GetCommanderName(DWORD unCommanderIndex);
	virtual      LPWSTR GetDecommanderName(DWORD unDecommanderIndex);
	virtual       DWORD GetCommanderType(DWORD unCommanderIndex);
	virtual       DWORD GetDecommanderType(DWORD unDecommanderIndex);
	virtual       void* GetOutputPointer(DWORD unCommanderIndex);
	virtual        void SetInputPointer(DWORD unDecommanderIndex, void* pData);
	virtual        bool SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual       void* Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD unNumberConnected, int& nProvokerIndex);
	virtual        void UpdateImGuiControl(float fZoom);

private:
	/*** StereoSplitter private D3D9 methods ***/
	void                    SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD unRenderTargetIndex, IDirect3DSurface9* pcRenderTarget);
	void                    SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil);
	void                    SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage, IDirect3DBaseTexture9* pcTexture);
	HRESULT                 SetViewport(IDirect3DDevice9* pcDevice, CONST D3DVIEWPORT9* psViewport);
	void                    Apply();

	/*** StereoSplitter private methods ***/
	IDirect3DSurface9* VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pcSurface);
	IDirect3DBaseTexture9* VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, bool bDuplicate);
	bool                    SetDrawingSide(IDirect3DDevice9* pcDevice, RenderPosition side);
	void                    SetDrawingSideField(RenderPosition eSide) { m_eCurrentRenderingSide = eSide; if (m_psModifierData) m_psModifierData->eCurrentRenderingSide = eSide; }
	void                    CreateStereoTexture(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, IDirect3DBaseTexture9** ppcStereoTwinTexture);
	bool                    ShouldDuplicateRenderTarget(UINT unWidth, UINT unHeight, D3DFORMAT Format, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bLockable, bool bIsSwapChainBackBuffer);
	bool                    ShouldDuplicateDepthStencilSurface(UINT unWidth, UINT unHeight, D3DFORMAT eFormat, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bDiscard);
	bool                    ShouldDuplicateTexture(UINT unWidth, UINT unHeight, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool);
	bool                    ShouldDuplicateCubeTexture(UINT unEdgeLength, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool);
	bool                    IsViewportDefaultForMainRT(CONST D3DVIEWPORT9* psViewport);
	void                    UnWrapProxyTexture(IDirect3DBaseTexture9* pWrappedTexture, IDirect3DBaseTexture9** ppActualLeftTexture, IDirect3DBaseTexture9** ppActualRightTexture);
	D3DFORMAT               GetD3D9ExFormat(D3DFORMAT eFormat);

	/// <summary>
	/// [OUT] All data this node outputs.
	/// </summary>
	StereoData m_sStereoData;
	/// <summary>
	/// [IN] Input pointers array.
	/// </summary>
	void* m_ppInput[NUMBER_OF_DECOMMANDERS];

#pragma region /// => Internal fields
	/// <summary>
	/// Matrix Modifier data.
	/// Used both in Matrix Modifier and in Stereo Splitter.
	/// </summary>
	ModifierData* m_psModifierData;
	/// <summary>
	/// Current drawing side, only changed in StereoSplitter->SetDrawingSide().
	/// </summary>
	RenderPosition m_eCurrentRenderingSide;
	/// <summary>
	/// Active stored render target views.
	/// The render targets that are currently in use.
	/// DX9 :
	/// 0--------------------------------------------> D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	/// D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	/// </summary>
	std::vector<IDirect3DSurface9*> m_apcActiveRenderTargets;
	/// <summary>
	/// Active stored textures.
	/// The textures that are currently in use.
	/// DX9 :
	/// 0--------------------------------------> D3D9_SIMULTANEOUS_TEXTURE_COUNT ----- Left render target views
	/// D3D9_SIMULTANEOUS_TEXTURE_COUNT -------> D3D9_SIMULTANEOUS_TEXTURE_COUNT * 2 - Right render target views
	/// </summary>
	std::vector<IDirect3DBaseTexture9*> m_apcActiveTextures;
	/// <summary>
	/// Active stored displacement textures.
	/// The displacement textures that are currently in use.
	/// DX9 :
	/// 0----------------------------------------------> D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT     ----- Left render target views
	/// D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT -> D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT * 2 ----- Right render target views
	/// </summary>
	std::vector<IDirect3DBaseTexture9*> m_apcActiveTexturesDisplacement;
	/// <summary>
	/// Active stored depth stencil.
	/// The depth stencil surface that are currently in use.
	/// Left (0) / Right (1)
	/// </summary>
	IDirect3DSurface9* m_pcActiveDepthStencilSurface[2];
	/// <summary>
	/// Active back buffer.
	/// The back buffer surface that is currently in use.
	/// Left (0) / Right (1)
	/// </summary>
	IDirect3DSurface9* m_pcActiveBackBufferSurface[2];
	/// <summary>
	/// Active proxy back buffers. Only valid if D3D9Ex device is used.
	/// The back buffer surfaces stored for any swapchain. First index swapchain, second backbuffer.
	/// </summary>
	std::vector<std::vector<IDirect3DStereoSurface9*>> m_aapcActiveProxyBackBufferSurfaces;
	/// <summary>
	/// Active stored swapchains. Only valid if D3D9Ex device is used.
	/// Note that any given device can support only one full-screen swap chain !!
	/// For a single adapter only ONE swapchain can be active !! (=index 0)
	/// </summary>
	std::vector<IDirect3DSwapChain9*> m_apcActiveSwapChains;
	/// <summary>
	/// Active stored vertex buffers.
	/// </summary>
	std::unordered_map<UINT, IDirect3DProxyVertexBuffer9*> m_apcActiveVertexBuffers;
	/// <summary>
	/// Number of set textures.
	/// Number of textures not set to NULL.
	/// </summary>
	DWORD m_unTextureNumber;
	/// <summary>
	/// Number of render targets.
	/// Number of render targets not set to NULL.
	/// </summary>
	DWORD m_unRenderTargetNumber;
	/// <summary>
	/// True if active viewport is the default one.
	/// @see isViewportDefaultForMainRT()
	/// </summary>
	bool m_bActiveViewportIsDefault;
	/// <summary>
	/// Last viewport backup.
	/// </summary>
	D3DVIEWPORT9 m_sLastViewportSet;
	/// <summary>
	/// The currently provided D3D9 device.
	/// </summary>
	IDirect3DDevice9* m_pcDeviceCurrent;
#pragma endregion
#pragma region /// => Node fields
	enum class StereoTechnique
	{
		VERSION_3,
		VERSION_4
	} m_eTechnique;
	/// <summary>
	/// True if Present() was called at least once.
	/// Game can crash if Present() is not connected,
	/// so this is added for security.
	/// </summary>
	bool m_bPresent;
	/// <summary>
	/// True if Apply() was called (IDirect3DStateBlock9).
	/// </summary>
	bool m_bApply;
	/// <summary>
	/// The font used.
	/// </summary>
	HFONT m_hFont;
	/// <summary>
	/// The output textures.
	/// Left (0) / Right (1)
	/// </summary>
	LPDIRECT3DTEXTURE9 m_pcStereoBuffer[2];
	/// <summary>
	/// The output surface´s.
	/// Left (0) / Right (1)
	/// </summary>
	LPDIRECT3DSURFACE9 m_pcStereoBufferSurface[2];
	/// <summary>
	/// All GUI pages IDs.
	/// </summary>
	std::vector<DWORD> m_aunPageIDs;
	/// <summary>
	/// Game Settings page control IDs.
	/// Structure contains all control IDs for the game settings page.
	/// </summary>
	struct PageGameSettings
	{
		/*** PageGameSettings game configuration ***/
		UINT unDuplicateRenderTargetID;  /**< [Switch] Setting which render targets are to be duplicated. **/
		UINT unDuplicateDepthStencilID;  /**< [Switch] Setting which depth stencils are to be duplicated. **/
		UINT unDuplicateTextureID;       /**< [Switch] Setting which textures are to be duplicated. **/
		UINT unDuplicateCubeTextureID;   /**< [Switch] Setting which cube textures are to be duplicated. **/
		UINT unSaveRenderStatesID;       /**< [Switch] Setting how to save render states for stereo rendering. **/
		UINT unTextlist;                 /**< [Text] Text for all controls of this page. **/
	} m_sPageGameSettings;
	/// <summary>
	/// Game settings page.
	/// Structure contains all stereo game settings.
	/// </summary>
	struct GameSettings
	{
		/*** GameSettings game configuration ***/
		int nDuplicateRenderTarget;  /**< [Switch] Setting which render targets are to be duplicated. **/
		int nDuplicateDepthStencil;  /**< [Switch] Setting which depth stencils are to be duplicated. **/
		int nDuplicateTexture;       /**< [Switch] Setting which textures are to be duplicated. **/
		int nDuplicateCubeTexture;   /**< [Switch] Setting which cube textures are to be duplicated. **/
		int nSaveRenderStatesID;     /**< [Switch] Setting how to save render states for stereo rendering. **/
	} m_sGameSettings;
	/// <summary>
	/// Data buffer to save this node.
	/// </summary>
	char m_acData[MAX_DATA_SIZE];
	/// <summary>
	/// True if stereo images should be streched to monitor output.
	/// </summary>
	bool m_bMonitorStereo;
	/// <summary>
	/// True if simple boxes as indicators for stereo should be drawn.
	/// </summary>
	bool m_bDrawStereoIndicators;
	/// <summary>
	/// Vireio menu.
	/// </summary>
	VireioSubMenu m_sMenu;
	/// <summary>
	/// Frames to save the ini file.
	/// </summary>
	INT m_nIniFrameCount;
	/// <summary>
	/// Trace button. Only used if TRACE_SPLITTER is defined.
	/// </summary>
	bool m_bTrace;
#pragma endregion

	/*** StereoSplitter private D3D9 methods ***/

#pragma region /// => Version 3 methods
	/// <summary> Init v3 internal fields. /// </summary>
	void Init_v3();
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateAdditionalSwapchain_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetSwapChain_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT Reset_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT Present_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetBackBuffer_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateVolumeTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateCubeTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateVertexBuffer_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateIndexBuffer_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateRenderTarget_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateDepthStencilSurface_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT UpdateSurface_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT UpdateTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetRenderTargetData_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetFrontBufferData_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT StretchRect_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT ColorFill_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateOffscreenPlainSurface_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetRenderTarget_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetRenderTarget_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetDepthStencilSurface_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetDepthStencilSurface_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetViewport_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetRenderState_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetTexture_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT ProcessVertices_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetStreamSource_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetStreamSource_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetIndices_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetIndices_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT DrawRectPatch_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT DrawTriPatch_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_Present_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_GetFrontBufferData_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_GetBackBuffer_v3(int& nFlags);
	/// <summary>Version 3 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SB_Apply_v3(int& nFlags);
#pragma endregion

#pragma region /// => Version 4 methods
	/// <summary> Init v4 internal fields. /// </summary>
	void Init_v4();
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateAdditionalSwapchain_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetSwapChain_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT Reset_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT Present_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetBackBuffer_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateVolumeTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateCubeTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateVertexBuffer_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateIndexBuffer_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateRenderTarget_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateDepthStencilSurface_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT UpdateSurface_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT UpdateTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetRenderTargetData_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetFrontBufferData_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT StretchRect_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT ColorFill_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT CreateOffscreenPlainSurface_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetRenderTarget_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetRenderTarget_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetDepthStencilSurface_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetDepthStencilSurface_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetViewport_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetRenderState_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetTexture_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT ProcessVertices_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetStreamSource_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetStreamSource_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SetIndices_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT GetIndices_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT DrawRectPatch_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT DrawTriPatch_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_Present_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_GetFrontBufferData_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SC_GetBackBuffer_v4(int& nFlags);
	/// <summary>Version 4 method call</summary><param name="nFlags">[in,out]Method call flags</param><returns>D3D result</returns>
	HRESULT SB_Apply_v4(int& nFlags);
#pragma endregion
};

/// <summary>
/// Exported Constructor Method.
/// </summary>
extern "C" __declspec(dllexport) AQU_Nodus * AQU_Nodus_Create(ImGuiContext * sCtx)
{
	StereoSplitter* pStereoSplitter = new StereoSplitter(sCtx);
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

