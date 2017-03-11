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

#pragma region include
#include<stdio.h>
#include<vector>

#include"AQU_Nodus.h"
#include"Resources.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_GUI.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierDataStructures.h"
#include"VireioStereoSplitter_Proxy.h"
#include"D3D9Ex_D3DX9_Methods.h"
#pragma endregion

#pragma region defines
#define SMALL_FLOAT 0.001f
#define	SLIGHTLY_LESS_THAN_ONE 0.999f

#define PI 3.141592654
#define RADIANS_TO_DEGREES(fRad) ((float) fRad * (float) (180.0 / PI))

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

#define GUI_WIDTH                                   1024                      
#define GUI_HEIGHT                                  7700
#define GUI_CONTROL_BORDER                            64
#define GUI_CONTROL_FONTSIZE                          64
#define GUI_CONTROL_FONTBORDER                        16
#define GUI_CONTROL_LINE                              92
#define GUI_CONTROL_BUTTONSIZE                       488
#define GUI_CONTROL_SPINSIZE                         980

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                       115

#define DUPLICATE_RENDERTARGET_POS_X                  16
#define DUPLICATE_RENDERTARGET_POS_Y                  64
#define DUPLICATE_DEPTHSTENCIL_POS_X                  16
#define DUPLICATE_DEPTHSTENCIL_POS_Y                 256
#define DUPLICATE_TEXTURE_POS_X                       16
#define DUPLICATE_TEXTURE_POS_Y                      448
#define DUPLICATE_CUBETEXTURE_POS_X                   16
#define DUPLICATE_CUBETEXTURE_POS_Y                  640
#define SAVE_RENDERSTATES_POS_X                       16
#define SAVE_RENDERSTATES_POS_Y                      832

#define MAX_DATA_SIZE                              65535                     /**< Arbitrary... TODO !! set a maximum node data size **/

#define NO_PRIVATE_INTERFACE             (IUnknown*)0xf1                     /**< Arbitrary value set instead of an IUnknown interface as "verified" indicator. ***/
#pragma endregion

/**
* Maximum simultaneous textures : 16 {shader sampling stage registers: s0 to s15}
***/
#define D3D9_SIMULTANEAOUS_TEXTURE_COUNT                16
/**
* Maximum simultaneous displacement textures : 5 {D3DDMAPSAMPLER+D3DVERTEXTEXTURESAMPLER0..3}
***/
#define D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT   5
/**
* Maximum simultaneous render targets ?
***/
#define D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT            D3D_FL9_3_SIMULTANEOUS_RENDER_TARGET_COUNT 

/**
* Node Commander Enumeration.
***/
enum STS_Commanders
{
	StereoTextureLeft,
	StereoTextureRight
};

/**
* Node Commander Enumeration.
***/
enum STS_Decommanders
{
	RenderTargetIndex,            /**< ->SetRenderTarget(), GetRenderTarget() render target index ***/
	pRenderTarget,                /**< ->SetRenderTarget() render target ***/
	ppRenderTarget,               /**< ->GetRenderTarget() render target **/
	pNewZStencil,                 /**< ->SetDepthStencilSurface() stencil surface ***/
	ppZStencilSurface,            /**< ->GetDepthStencilSurface() **/
	Sampler,                      /**< ->SetTexture(), GetTexture() sampler index **/
	pTexture,                     /**< ->SetTexture() texture pointer ***/
	ppTextureGet,                 /**< ->GetTexture() ***/
	pSourceSurface,               /**< ->UpdateSurface() source surface ***/
	pSourceRect,                  /**< ->UpdateSurface() source rectangle ***/
	pDestinationSurface,          /**< ->UpdateSurface() destination surface ***/
	pDestPoint,                   /**< ->UpdateSurface() destination point ***/
	pSourceTexture,               /**< ->UpdateTexture() source texture ***/
	pDestinationTexture,          /**< ->UpdateTexture() destination texture ***/
	pSurface,                     /**< ->ColorFill() surface pointer ***/
	pRect,                        /**< ->ColorFill() destination rectangle ***/
	color,                        /**< ->ColorFill() destination color ***/
	pSourceSurface_StretchRect,   /**< ->StretchRect() source surface ***/
	pSourceRect_StretchRect,      /**< ->StretchRect() source rectangle ***/
	pDestSurface_StretchRect,     /**< ->StretchRect() destination surface ***/
	pDestRect_StretchRect,        /**< ->StretchRect() destination rectangle ***/
	Filter_StretchRect,           /**< ->StretchRect() filter ***/
	State,                        /**< ->SetRenderState() State ***/
	Value,                        /**< ->SetRenderState() Value ***/
	pViewport,                    /**< ->SetViewport() Viewport ***/
	pRenderTargetGetData,         /**< ->GetRenderTargetData() */
	iSwapChain,                   /**< ->GetFrontBufferData(), GetBackBuffer(), GetSwapChain() */
	pDestSurface,                 /**< ->GetRenderTargetData() + GetFrontBufferData() */
	iBackBuffer,                  /**< ->GetBackBuffer() **/
	Type,                         /**< ->GetBackBuffer() **/
	ppBackBuffer,                 /**< ->GetBackBuffer() **/
	Width,                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	Height,                       /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	Length,
	Depth,                        /**< ->CreateVolumeTexture() **/
	EdgeLength,                   /**< ->CreateCubeTexture() **/
	Levels,                       /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture() **/
	Usage,                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture() **/
	Format,                       /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	FVF,
	MultiSample,                  /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
	MultisampleQuality,           /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
	Discard,                      /**< ->CreateDepthStencilSurface() **/
	Lockable,                     /**< ->CreateRenderTarget() **/
	Pool,                         /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateOffscreenPlainSurface() **/
	ppTexture,                    /**< ->CreateTexture() **/
	ppVolumeTexture,              /**< ->CreateVolumeTexture() **/
	ppCubeTexture,                /**< ->CreateCubeTexture() **/
	ppVertexBuffer,
	ppIndexBuffer,
	ppSurface,                    /**< ->CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	pSharedHandle,                /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	SrcStartIndex,                /**< ->ProcessVertices() **/
	DestIndex,                    /**< ->ProcessVertices() **/
	VertexCount,                  /**< ->ProcessVertices() **/
	pDestBuffer,                  /**< ->ProcessVertices() **/
	pVertexDecl,                  /**< ->ProcessVertices() **/
	Flags,                        /**< ->ProcessVertices() **/
	StreamNumber,                 /**< ->SetStreamSource(), ->GetStreamSource() **/
	pStreamData,                  /**< ->SetStreamSource(), **/
	OffsetInBytes,                /**< ->SetStreamSource() **/
	Stride,                       /**< ->SetStreamSource() **/
	ppStreamData,                 /**< ->GetStreamSource() **/
	pOffsetInBytes,               /**< ->GetStreamSource() **/
	pStride,                      /**< ->GetStreamSource() **/
	pIndexData,                   /**< ->SetIndices() **/
	ppIndexData,                  /**< ->SetIndices() **/
	peDrawingSide,                /**< Pointer to the extern drawing side bool. The extern bool will be updated depending on m_eCurrentRenderingSide ***/
	pasVShaderConstantIndices,    /**< The constant rule indices for the actual vertex shader. ***/
	pasPShaderConstantIndices,    /**< The constant rule indices for the actual pixel shader. ***/
	pDestBox,                     /**< D3DX9 method types ***/
	pSrcBox,
	ColorKey,
	SrcFormat,
	DestFormat,
	pSrcInfo,
	Channel,
	Filter,
	MipFilter,
	Amplitude,
	hSrcModule,
	pDestFile,
	pSrcFile,
	pSrcResource,
	pSrcData,
	pSrcMemory,
	pDestFileW,
	pSrcFileW,
	pSrcResourceW,
	ppDestBuf,
	pFunction,
	pTextureShader,
	pBaseTexture,
	pSrcTexture,
	pCubeTexture,
	pSrcSurface,
	pTexture_D3DX,
	pDestVolume,
	pSrcVolume,
	pVolumeTexture,
	pData_Enum,
	pDestPalette,
	pPalette,
	pSrcPalette,
	pDestRect,
	pSrcRect,
	MipLevels,
	Size,
	SrcDataSize,
	SrcLevel,
	SrcPitch,
	SrcRowPitch,
	SrcSlicePitch,
	pPresentationParameters,
	pSwapChain
};

/**
* Methods table of D3DX9 methods.
* Provided method ids by Aquilinus.
***/
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

/**
* Simple left, right enumeration.
***/
enum RenderPosition
{
	// probably need an 'Original' here
	Left = 1,
	Right = 2
};

/**
* All GUI pages for the Stereo Splitter.
* Must be added in following order.
***/
enum GUI_Pages
{
	MainPage = 0,
	DescriptionPage = 1,
	GameSettingsPage = 2,
	NumberOfPages = 3,
};

/**
* Simple false/true enumeration prototype.
***/
typedef struct
{
	enum
	{
		DT_FALSE = 0,
		DT_TRUE = 1,
		DETERMINEFALSETRUE_LAST
	};
} DetermineFalseTrue;

/**
* Determines how to save render states for stereo view output.
***/
enum DetermineSaveRenderStates
{
	DT_STATE_BLOCK = 0,
	DT_SELECTED_STATES_MANUALLY = 1,
	DT_ALL_STATES_MANUALLY = 2,
	DT_DO_NOT_SAVE_AND_RESTORE = 3,
	DETERMINESAVERENDERSTATES_LAST
};

/**
* Determines when to duplicate a render target or depth stencil.
***/
typedef struct : public DetermineFalseTrue
{
	enum
	{
		DT_WIDTH_UNEQUAL_HEIGHT = DetermineFalseTrue::DETERMINEFALSETRUE_LAST,
		DETERMINEDUPLICATERTODS_LAST
	};
} DetermineDuplicateRToDS;

/**
* Determines when to duplicate a base texture.
***/
typedef struct : public DetermineFalseTrue
{
	enum : int
	{
		DT_IF_RENDER_TARGET = DetermineFalseTrue::DETERMINEFALSETRUE_LAST,
		DETERMINEDUPLICATEBASETEXTURE_LAST
	};
} DetermineDuplicateBaseTexture;

/**
* Determines when to duplicate a texture.
***/
typedef struct : public DetermineDuplicateBaseTexture
{
	enum
	{
		DT_IF_RENDER_TARGET_AND_WIDTH_UNEQUAL_HEIGHT = DetermineDuplicateBaseTexture::DETERMINEDUPLICATEBASETEXTURE_LAST,
		DETERMINEDUPLICATETEXTURE_LAST
	};
} DetermineDuplicateTexture;

/**
* Determines when to duplicate a cube texture.
***/
typedef struct : public DetermineDuplicateBaseTexture
{
	enum
	{
		DETERMINEDUPLICATECUBETEXTURE_LAST = DetermineDuplicateBaseTexture::DETERMINEDUPLICATEBASETEXTURE_LAST,
	};
} DetermineDuplicateCubeTexture;

/**
* Game type settings enumeration.
* Contains all game specific stereo rendering settings.
***/
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

/**
* Vireio Stereo Splitter Node Plugin (Direct3D 9).
* Vireio Perception Stereo Render Target Handler.
***/
class StereoSplitter : public AQU_Nodus
{
public:
	StereoSplitter();
	virtual ~StereoSplitter();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetSaveDataSize();
	virtual char*           GetSaveData(UINT* pdwSizeOfData);
	virtual void            InitNodeData(char* pData, UINT dwSizeOfData);
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD unCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD unDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD unCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD unDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD unCommanderIndex);
	virtual void            SetInputPointer(DWORD unDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD unNumberConnected, int& nProvokerIndex);
	virtual void            WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	/*** StereoSplitter private D3D9 stub methods ***/
	void                    Present(IDirect3DDevice9* pcDevice, bool bInit);

	/*** StereoSplitter private D3D9 methods ***/
	void                    SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD unRenderTargetIndex, IDirect3DSurface9* pcRenderTarget);
	void                    SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil);
	void                    SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage, IDirect3DBaseTexture9* pcTexture);
	HRESULT                 SetViewport(IDirect3DDevice9* pcDevice, CONST D3DVIEWPORT9* psViewport);
	void                    Apply();

	/*** StereoSplitter private methods ***/
	IDirect3DSurface9*      VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pcSurface);
	IDirect3DBaseTexture9*  VerifyPrivateDataInterfaces(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, bool bDuplicate);
	bool                    SetDrawingSide(IDirect3DDevice9* pcDevice, RenderPosition side);
	void                    SetDrawingSideField(RenderPosition eSide) { m_eCurrentRenderingSide = eSide; if (m_peDrawingSide) *m_peDrawingSide = eSide; }
	void                    CreateStereoTexture(IDirect3DDevice9* pcDevice, IDirect3DBaseTexture9* pcTexture, IDirect3DBaseTexture9** ppcStereoTwinTexture);
	bool                    ShouldDuplicateRenderTarget(UINT unWidth, UINT unHeight, D3DFORMAT Format, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bLockable, bool bIsSwapChainBackBuffer);
	bool                    ShouldDuplicateDepthStencilSurface(UINT unWidth, UINT unHeight, D3DFORMAT eFormat, D3DMULTISAMPLE_TYPE eMultiSample, DWORD unMultisampleQuality, BOOL bDiscard);
	bool                    ShouldDuplicateTexture(UINT unWidth, UINT unHeight, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool);
	bool                    ShouldDuplicateCubeTexture(UINT unEdgeLength, UINT unLevels, DWORD unUsage, D3DFORMAT eFormat, D3DPOOL ePool);
	bool                    IsViewportDefaultForMainRT(CONST D3DVIEWPORT9* psViewport);
	void                    CreateGUI();
	void                    UnWrapProxyTexture(IDirect3DBaseTexture9* pWrappedTexture, IDirect3DBaseTexture9** ppActualLeftTexture, IDirect3DBaseTexture9** ppActualRightTexture);
	D3DFORMAT               GetD3D9ExFormat(D3DFORMAT eFormat);

	/**
	* Input pointers.
	***/
	DWORD*                                        m_punRenderTargetIndex;                            /**< ->SetRenderTarget(), GetRenderTarget() render target index ***/
	IDirect3DSurface9**                           m_ppcRenderTarget;                                 /**< ->SetRenderTarget() render target ***/
	IDirect3DSurface9***                          m_pppcRenderTarget;                                /**< ->GetRenderTarget() render target **/
	IDirect3DSurface9**                           m_ppcNewZStencil;                                  /**< ->SetDepthStencilSurface() stencil surface ***/
	IDirect3DSurface9***                          m_pppcZStencilSurface;                             /**< ->GetDepthStencilSurface() **/
	DWORD*                                        m_punSampler;                                      /**< ->SetTexture(), GetTexture() sampler index **/
	IDirect3DBaseTexture9**                       m_ppcTexture;                                      /**< ->SetTexture() texture pointer ***/
	IDirect3DBaseTexture9***                      m_pppcTexture;                                     /**< ->GetTexture() texture pointer ***/
	IDirect3DSurface9**                           m_ppSourceSurface;                                 /**< ->UpdateSurface() source surface ***/
	RECT**                                        m_ppcSourceRect;                                   /**< ->UpdateSurface() source rectangle ***/
	IDirect3DSurface9**                           m_ppcDestinationSurface;                           /**< ->UpdateSurface() destination surface ***/
	POINT**                                       m_ppsDestPoint;                                    /**< ->UpdateSurface() destination point ***/
	IDirect3DBaseTexture9**                       m_ppcSourceTexture;                                /**< ->UpdateTexture() source texture ***/
	IDirect3DBaseTexture9**                       m_ppcDestinationTexture;                           /**< ->UpdateTexture() destination texture ***/
	IDirect3DSurface9**                           m_ppcSurface;                                      /**< ->ColorFill() surface pointer ***/
	RECT**                                        m_ppsRect;                                         /**< ->ColorFill() destination rectangle ***/
	D3DCOLOR*                                     m_punColor;                                        /**< ->ColorFill() destination color ***/
	IDirect3DSurface9**                           m_ppcSourceSurface_StretchRect;                    /**< ->StretchRect() source surface ***/
	RECT**                                        m_ppcSourceRect_StretchRect;                       /**< ->StretchRect() source rectangle ***/
	IDirect3DSurface9**                           m_ppcDestSurface_StretchRect;                      /**< ->StretchRect() destination surface ***/
	RECT**                                        m_ppcDestRect_StretchRect;                         /**< ->StretchRect() destination rectangle ***/
	D3DTEXTUREFILTERTYPE*                         m_peFilter_StretchRect;                            /**< ->StretchRect() filter ***/
	D3DRENDERSTATETYPE*                           m_peState;                                         /**< ->SetRenderState() State ***/
	DWORD*                                        m_punValue;                                        /**< ->SetRenderState() Value ***/
	D3DVIEWPORT9**                                m_ppsViewport;                                     /**< ->SetViewport() Viewport ***/
	IDirect3DSurface9**                           m_ppcRenderTargetGetData;                          /**< ->GetRenderTargetData() */
	UINT*                                         m_punISwapChain;                                   /**< ->GetFrontBufferData(), GetBackBuffer()  */
	IDirect3DSurface9**                           m_ppcDestSurface;                                  /**< ->GetRenderTargetData() + GetFrontBufferData() */
	UINT*                                         m_punIBackBuffer;                                  /**< ->GetBackBuffer() **/
	D3DBACKBUFFER_TYPE*                           m_peType;                                          /**< ->GetBackBuffer() **/
	IDirect3DSurface9***                          m_pppcBackBuffer;                                  /**< ->GetBackBuffer() **/
	UINT*                                         m_punWidth;                                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	UINT*                                         m_punHeight;                                       /**< ->CreateTexture(), CreateVolumeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	UINT*                                         m_punEdgeLength;                                   /**< ->CreateCubeTexture() **/
	UINT*                                         m_punLevels;                                       /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture() **/
	UINT*                                         m_punDepth;                                        /**< ->CreateVolumeTexture() **/
	DWORD*                                        m_punUsage;                                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateVertexBuffer(), CreateIndexBuffer() **/
	D3DFORMAT*                                    m_peFormat;                                        /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface(), CreateIndexBuffer() **/
	D3DMULTISAMPLE_TYPE*                          m_peMultiSample;                                   /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
	DWORD*                                        m_punMultisampleQuality;                           /**< ->CreateRenderTarget(), CreateDepthStencilSurface() **/
	BOOL*                                         m_pnDiscard;                                       /**< ->CreateDepthStencilSurface() **/
	BOOL*                                         m_pnLockable;                                      /**< ->CreateRenderTarget() **/
	D3DPOOL*                                      m_pePool;                                          /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateOffscreenPlainSurface(), CreateVertexBuffer(), CreateIndexBuffer() **/
	IDirect3DTexture9***                          m_pppcTextureCreate;                               /**< ->CreateTexture() **/
	IDirect3DVolumeTexture9***                    m_pppcVolumeTexture;                               /**< ->CreateVolumeTexture() **/
	IDirect3DCubeTexture9***                      m_pppcCubeTexture;                                 /**< ->CreateCubeTexture() **/
	IDirect3DSurface9***                          m_pppcSurfaceCreate;                               /**< ->CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface() **/
	UINT*                                         m_punLength;                                       /**< ->CreateVertexBuffer(), CreateIndexBuffer() **/
	DWORD*                                        m_punFVF;                                          /**< ->CreateVertexBuffer() **/
	IDirect3DVertexBuffer9***                     m_pppcVertexBuffer;                                /**< ->CreateVertexBuffer() **/
	IDirect3DIndexBuffer9***                      m_pppcIndexBuffer;                                 /**< ->CreateIndexBuffer() **/
	HANDLE**                                      m_ppvSharedHandle;                                 /**< ->CreateTexture(), CreateVolumeTexture(), CreateCubeTexture(), CreateRenderTarget(), CreateDepthStencilSurface(), CreateOffscreenPlainSurface(), CreateVertexBuffer(), CreateIndexBuffer() **/
	UINT*                                         m_punSrcStartIndex;                                /**< ->ProcessVertices() **/
	UINT*                                         m_punDestIndex;                                    /**< ->ProcessVertices() **/
	UINT*                                         m_punVertexCount;                                  /**< ->ProcessVertices() **/
	IDirect3DVertexBuffer9**                      m_ppcDestBuffer;                                   /**< ->ProcessVertices() **/
	IDirect3DVertexDeclaration9**                 m_ppcVertexDecl;                                   /**< ->ProcessVertices() **/
	DWORD*                                        m_punFlags;                                        /**< ->ProcessVertices() **/
	UINT*                                         m_punStreamNumber;                                 /**< ->SetStreamSource(), ->GetStreamSource() **/
	IDirect3DVertexBuffer9**                      m_ppcStreamData;                                   /**< ->SetStreamSource(), **/
	UINT*                                         m_punOffsetInBytes;                                /**< ->SetStreamSource() **/
	UINT*                                         m_punStride;                                       /**< ->SetStreamSource() **/
	IDirect3DVertexBuffer9***                     m_pppcStreamData;                                  /**< ->GetStreamSource() **/
	UINT**                                        m_ppunOffsetInBytes;                               /**< ->GetStreamSource() **/
	UINT**                                        m_ppunStride;                                      /**< ->GetStreamSource() **/
	IDirect3DIndexBuffer9**                       m_ppcIndexData;                                    /**< ->SetIndices() **/
	IDirect3DIndexBuffer9***                      m_pppcIndexData;                                   /**< ->GetIndices() **/
	RenderPosition*                               m_peDrawingSide;                                   /**< Pointer to the extern drawing side bool. The extern bool will be updated depending on m_eCurrentRenderingSide ***/
	std::vector<Vireio_Constant_Rule_Index_DX9>** m_ppasVSConstantRuleIndices;                       /**< Pointer to the constant rule indices for the current vertex shader ***/
	std::vector<Vireio_Constant_Rule_Index_DX9>** m_ppasPSConstantRuleIndices;                       /**< Pointer to the constant rule indices for the current pixel shader ***/
	D3DBOX** m_ppsDestBox;
	D3DBOX** m_ppsSrcBox;
	D3DCOLOR* m_punColorKey;
	D3DFORMAT* m_peSrcFormat;
	D3DXIMAGE_FILEFORMAT* m_peDestFormat;
	D3DXIMAGE_INFO** m_ppsSrcInfo;
	DWORD* m_punChannel;
	DWORD* m_punFilter;
	DWORD* m_punMipFilter;
	FLOAT* m_pfAmplitude;
	HMODULE* m_ppsSrcModule;
	LPCSTR* m_pszSrcFile;
	LPCSTR* m_pszSrcResource;
	LPCVOID* m_ppSrcData;
	LPCVOID* m_ppSrcMemory;
	LPCWSTR* m_pszDestFile;
	LPCWSTR* m_pszSrcFileW;
	LPCWSTR* m_pszSrcResourceW;
	LPD3DXBUFFER** m_pppcDestBuf;
	LPD3DXFILL2D* m_ppFunction;
	LPD3DXTEXTURESHADER* m_ppcTextureShader;
	LPDIRECT3DBASETEXTURE9* m_ppcBaseTexture;
	LPDIRECT3DBASETEXTURE9* m_ppcSrcBaseTexture;
	LPDIRECT3DCUBETEXTURE9* m_ppcCubeTexture;
	LPDIRECT3DSURFACE9* m_ppcSrcSurface;
	LPDIRECT3DTEXTURE9* m_ppcTexture_D3DX;
	LPDIRECT3DVOLUME9* m_ppcDestVolume;
	LPDIRECT3DVOLUME9* m_ppcSrcVolume;
	LPDIRECT3DVOLUMETEXTURE9* m_ppcVolumeTexture;
	LPVOID* m_ppData;
	PALETTEENTRY** m_ppsDestPalette;
	PALETTEENTRY** m_ppsPalette;
	PALETTEENTRY** m_ppsSrcPalette;
	RECT** m_ppsDestRect;
	RECT** m_ppsSrcRect;
	UINT* m_punSize;
	UINT* m_punSrcDataSize;
	UINT* m_punSrcLevel;
	UINT* m_punSrcPitch;
	UINT* m_punSrcRowPitch;
	UINT* m_punSrcSlicePitch;
	D3DPRESENT_PARAMETERS** m_ppsPresentationParams;
	IDirect3DSwapChain9*** m_pppcSwapChain;

	/**
	* Active stored render target views.
	* The render targets that are currently in use.
	* DX9 :
	* 0--------------------------------------------> D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	* D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D9_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	***/
	std::vector<IDirect3DSurface9*> m_apcActiveRenderTargets;
	/**
	* Active stored textures.
	* The textures that are currently in use.
	* DX9 :
	* 0--------------------------------------> D3D9_SIMULTANEOUS_TEXTURE_COUNT ----- Left render target views
	* D3D9_SIMULTANEOUS_TEXTURE_COUNT -------> D3D9_SIMULTANEOUS_TEXTURE_COUNT * 2 - Right render target views
	***/
	std::vector<IDirect3DBaseTexture9*> m_apcActiveTextures;
	/**
	* Active stored displacement textures.
	* The displacement textures that are currently in use.
	* DX9 :
	* 0----------------------------------------------> D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT     ----- Left render target views
	* D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT -> D3D9_SIMULTANEAOUS_DISPLACEMENT_TEXTURE_COUNT * 2 ----- Right render target views
	***/
	std::vector<IDirect3DBaseTexture9*> m_apcActiveTexturesDisplacement; 
	/**
	* Active stored depth stencil.
	* The depth stencil surface that are currently in use.
	* Left (0) / Right (1)
	***/
	IDirect3DSurface9* m_pcActiveDepthStencilSurface[2];
	/**
	* Active back buffer.
	* The back buffer surface that is currently in use.
	* Left (0) / Right (1)
	***/
	IDirect3DSurface9* m_pcActiveBackBufferSurface[2];
	/**
	* Active proxy back buffers. Only valid if D3D9Ex device is used.
	* The back buffer surfaces stored for any swapchain. First index swapchain, second backbuffer.
	***/
	std::vector<std::vector<IDirect3DStereoSurface9*>> m_aapcActiveProxyBackBufferSurfaces;
	/**
	* Active stored swapchains. Only valid if D3D9Ex device is used.
	**/
	std::vector<IDirect3DSwapChain9*> m_apcActiveSwapChains;
	/**
	* Active stored vertex buffers.
	**/
	std::unordered_map<UINT, IDirect3DProxyVertexBuffer9*> m_apcActiveVertexBuffers;
	/**
	* Number of set textures.
	* Number of textures not set to NULL.
	***/
	DWORD m_unTextureNumber;
	/**
	* Number of render targets.
	* Number of render targets not set to NULL.
	***/
	DWORD m_unRenderTargetNumber;
	/**
	* Current drawing side, only changed in SetDrawingSide().
	**/
	RenderPosition m_eCurrentRenderingSide;
	/**
	* True if active viewport is the default one.
	* @see isViewportDefaultForMainRT()
	**/
	bool m_bActiveViewportIsDefault;
	/**
	* Last viewport backup.
	**/
	D3DVIEWPORT9 m_sLastViewportSet;
	/**
	* The control bitmap.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* The control update bool.
	***/
	bool m_bControlUpdate;
	/**
	* True if Present() was called at least once.
	* Game can crash if Present() is not connected,
	* so this is added for security.
	***/
	bool m_bPresent;
	/**
	* True if Apply() was called (IDirect3DStateBlock9).
	***/
	bool m_bApply;
	/**
	* The font used.
	***/
	HFONT m_hFont;
	/**
	* The output textures.
	* Left (0) / Right (1)
	***/
	LPDIRECT3DTEXTURE9 m_pcStereoBuffer[2];
	/**
	* The output surface´s.
	* Left (0) / Right (1)
	***/
	LPDIRECT3DSURFACE9 m_pcStereoBufferSurface[2];
	/**
	* Vireio Graphical User Interface class.
	***/
	Vireio_GUI* m_pcVireioGUI;
	/**
	* All GUI pages IDs.
	***/
	std::vector<DWORD> m_aunPageIDs;
	/**
	* Game Settings page control IDs.
	* Structure contains all control IDs for the game settings page.
	***/
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
	/**
	* Game settings page.
	* Structure contains all stereo game settings.
	***/
	struct GameSettings
	{
		/*** GameSettings game configuration ***/
		int nDuplicateRenderTarget;  /**< [Switch] Setting which render targets are to be duplicated. **/
		int nDuplicateDepthStencil;  /**< [Switch] Setting which depth stencils are to be duplicated. **/
		int nDuplicateTexture;       /**< [Switch] Setting which textures are to be duplicated. **/
		int nDuplicateCubeTexture;   /**< [Switch] Setting which cube textures are to be duplicated. **/
		int nSaveRenderStatesID;   /**< [Switch] Setting how to save render states for stereo rendering. **/
	} m_sGameSettings;
	/**
	* Data buffer to save this node.
	***/
	char m_acData[MAX_DATA_SIZE];
	/**
	* True if the Splitter used a D3D9Ex device.
	***/
	bool m_bUseD3D9Ex;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

