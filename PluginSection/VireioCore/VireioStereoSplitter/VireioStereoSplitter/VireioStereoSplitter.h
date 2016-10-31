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

#include<stdio.h>
#include<vector>

#include"AQU_Nodus.h"
#include"Resources.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierDataStructures.h"

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
#define RESZ_CODE 0x7fa05000

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                        24

/**
* Maximum simultaneous textures : 16 {shader sampling stage registers: s0 to s15}
***/
#define D3D9_SIMULTANEAOUS_TEXTURE_COUNT                16
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
	RenderTargetIndex,            /**< ->SetRenderTarget() render target index ***/
	pRenderTarget,                /**< ->SetRenderTarget() render target ***/
	pNewZStencil,                 /**< ->SetDepthStencilSurface() stencil surface ***/
	Sampler,                      /**< ->SetTexture() sampler index **/
	pTexture,                     /**< ->SetTexture() texture pointer ***/
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
	peDrawingSide,                /**< Pointer to the extern drawing side bool. The extern bool will be updated depending on m_eCurrentRenderingSide ***/
	pasVShaderConstantIndices,    /**< The constant rule indices for the actual vertex shader. ***/
	pasPShaderConstantIndices,    /**< The constant rule indices for the actual pixel shader. ***/
	State,                        /**< ->SetRenderState() State ***/
	Value,                        /**< ->SetRenderState() Value ***/
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

private:
	/*** StereoSplitter private D3D9 stub methods ***/
	void                    Present(IDirect3DDevice9* pcDevice);

	/*** StereoSplitter private D3D9 methods ***/
	void                    SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD unRenderTargetIndex, IDirect3DSurface9* pcRenderTarget);
	void                    SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil);
	void                    SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage, IDirect3DBaseTexture9* pcTexture);
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

	
	/**
	* Input pointers.
	***/
	DWORD*                  m_punRenderTargetIndex;                            /**< ->SetRenderTarget() render target index ***/
	IDirect3DSurface9**     m_ppcRenderTarget;                                 /**< ->SetRenderTarget() render target ***/
	IDirect3DSurface9**     m_ppcNewZStencil;                                  /**< ->SetDepthStencilSurface() stencil surface ***/
	DWORD*                  m_punSampler;                                      /**< ->SetTexture() sampler index **/
	IDirect3DTexture9**     m_ppcTexture;                                      /**< ->SetTexture() texture pointer ***/
	IDirect3DSurface9**     m_ppSourceSurface;                                 /**< ->UpdateSurface() source surface ***/
	RECT**                  m_ppcSourceRect;                                   /**< ->UpdateSurface() source rectangle ***/
	IDirect3DSurface9**     m_ppcDestinationSurface;                           /**< ->UpdateSurface() destination surface ***/
	POINT**                 m_ppsDestPoint;                                    /**< ->UpdateSurface() destination point ***/
	IDirect3DBaseTexture9** m_ppcSourceTexture;                                /**< ->UpdateTexture() source texture ***/
	IDirect3DBaseTexture9** m_ppcDestinationTexture;                           /**< ->UpdateTexture() destination texture ***/
	IDirect3DSurface9**     m_ppcSurface;                                      /**< ->ColorFill() surface pointer ***/
	RECT**                  m_ppsRect;                                         /**< ->ColorFill() destination rectangle ***/
	D3DCOLOR*               m_punColor;                                        /**< ->ColorFill() destination color ***/
	IDirect3DSurface9**     m_ppcSourceSurface_StretchRect;                    /**< ->StretchRect() source surface ***/
	RECT**                  m_ppcSourceRect_StretchRect;                       /**< ->StretchRect() source rectangle ***/
	IDirect3DSurface9**     m_ppcDestSurface_StretchRect;                      /**< ->StretchRect() destination surface ***/
	RECT**                  m_ppcDestRect_StretchRect;                         /**< ->StretchRect() destination rectangle ***/
	D3DTEXTUREFILTERTYPE*   m_peFilter_StretchRect;                            /**< ->StretchRect() filter ***/
	RenderPosition*         m_peDrawingSide;                                   /**< Pointer to the extern drawing side bool. The extern bool will be updated depending on m_eCurrentRenderingSide ***/
	std::vector<Vireio_Constant_Rule_Index_DX9>** m_ppasVSConstantRuleIndices; /**< Pointer to the constant rule indices for the current vertex shader ***/
	std::vector<Vireio_Constant_Rule_Index_DX9>** m_ppasPSConstantRuleIndices; /**< Pointer to the constant rule indices for the current pixel shader ***/
	D3DRENDERSTATETYPE*     m_peState;                                         /**< ->SetRenderState() State ***/
	DWORD*                  m_punValue;                                        /**< ->SetRenderState() Value ***/

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
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

