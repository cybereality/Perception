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

#define INT_PLUG_TYPE                                  7 
#define UINT_PLUG_TYPE                                12
#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_VOID_PLUG_TYPE                           114
#define D3DFORMAT_PLUG_TYPE                         1011
#define D3DPRIMITIVETYPE_PLUG_TYPE                  1021
#define PNT_IDIRECT3DSURFACE9_PLUG_TYPE             2046
#define PNT_IDIRECT3DBASETEXTURE9_PLUG_TYPE         2038
#define PNT_IDIRECT3DTEXTURE9_PLUG_TYPE             2048

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                        26

/**
* Maximum simultaneous textures : 16 {shader sampling stage registers: s0 to s15} 
***/
#define MAX_SIMULTANEOUS_TEXTURES_D3D9                16

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
	PrimitiveType,                /**< ->DrawPrimitive() primitive type ***/
	StartVertex,                  /**< ->DrawPrimitive() start vertex ***/
	PrimitiveCount,               /**< ->DrawPrimitive() primitive count ***/
	Type,                         /**< ->DrawIndexedPrimitive() primitive type ***/
	BaseVertexIndex,              /**< ->DrawIndexedPrimitive() base vertex index ***/
	MinIndex,                     /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	NumVertices,                  /**< ->DrawIndexedPrimitive() number of vertices ***/
	StartIndex,                   /**< ->DrawIndexedPrimitive() start index  ***/
	PrimitiveCountIndexed,        /**< ->DrawIndexedPrimitive() primitive count ***/
	PrimitiveTypeUP,              /**< ->DrawPrimitiveUP() primitive type ***/
	PrimitiveCountUP,             /**< ->DrawPrimitiveUP() primitive count ***/
	pVertexStreamZeroData,        /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	VertexStreamZeroStride,       /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	PrimitiveTypeUPIndexed,       /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	MinVertexIndex,               /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	NumVerticesUPIndexed,         /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	PrimitiveCountUPIndexed,      /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	pIndexData,                   /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	IndexDataFormat,              /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	pVertexStreamZeroDataIndexed, /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	VertexStreamZeroStrideIndexed /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
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
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
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
	/*** StereoSplitter private D3D9 stub methods ***/
	void                    Present(IDirect3DDevice9* pcDevice);

	/*** StereoSplitter private D3D9 methods ***/
	void                    SetRenderTarget(IDirect3DDevice9* pcDevice, DWORD dwRenderTargetIndex, IDirect3DSurface9* pcRenderTarget);
	void                    SetDepthStencilSurface(IDirect3DDevice9* pcDevice, IDirect3DSurface9* pNewZStencil);
	void                    SetTexture(IDirect3DDevice9* pcDevice, DWORD Stage,IDirect3DBaseTexture9* pcTexture);
	void                    DrawPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwStartVertex, UINT dwPrimitiveCount);
	void                    DrawIndexedPrimitive(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, INT nBaseVertexIndex, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwStartIndex, UINT dwPrimCount);
	void                    DrawPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwPrimitiveCount, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride);
	void                    DrawIndexedPrimitiveUP(IDirect3DDevice9* pcDevice, D3DPRIMITIVETYPE ePrimitiveType, UINT dwMinVertexIndex, UINT dwNumVertices, UINT dwPrimitiveCount, CONST void* pIndexData, D3DFORMAT eIndexDataFormat, CONST void* pVertexStreamZeroData, UINT dwVertexStreamZeroStride);
	void                    Apply();

	/*** StereoSplitter private methods ***/
	int                     CheckIfMonitored(IDirect3DSurface9* pcSurface);
	void                    MonitorSurface(IDirect3DSurface9* pcSurface);
	bool                    SetDrawingSide(IDirect3DDevice9* pcDevice, RenderPosition side);

	/**
	* Input pointers.
	***/
	DWORD*              m_pdwRenderTargetIndex;              /**< ->SetRenderTarget() render target index ***/
	IDirect3DSurface9** m_ppcRenderTarget;                   /**< ->SetRenderTarget() render target ***/
	IDirect3DSurface9** m_ppcNewZStencil;                    /**< ->SetDepthStencilSurface() stencil surface ***/
	DWORD*              m_pdwSampler;                        /**< ->SetTexture() sampler index **/
	IDirect3DTexture9** m_ppcTexture;                        /**< ->SetTexture() texture pointer ***/
	D3DPRIMITIVETYPE*   m_pePrimitiveType;                   /**< ->DrawPrimitive() primitive type ***/
	UINT*               m_pdwStartVertex;                    /**< ->DrawPrimitive() start vertex ***/
	UINT*               m_pdwPrimitiveCount;                 /**< ->DrawPrimitive() primitive count ***/
	D3DPRIMITIVETYPE*   m_peType;                            /**< ->DrawIndexedPrimitive() primitive type ***/
	INT*                m_pnBaseVertexIndex;                 /**< ->DrawIndexedPrimitive() base vertex index ***/
	UINT*               m_pdwMinIndex;                       /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	UINT*               m_pdwNumVertices;                    /**< ->DrawIndexedPrimitive() number of vertices ***/
	UINT*               m_pdwStartIndex;                     /**< ->DrawIndexedPrimitive() start index  ***/
	UINT*               m_pdwPrimitiveCountIndexed;          /**< ->DrawIndexedPrimitive() primitive count ***/
	D3DPRIMITIVETYPE*   m_pePrimitiveTypeUP;                 /**< ->DrawPrimitiveUP() primitive type ***/
	UINT*               m_pdwPrimitiveCountUP;               /**< ->DrawPrimitiveUP() primitive count ***/
	void**              m_ppVertexStreamZeroData;            /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	UINT*               m_pdwVertexStreamZeroStride;         /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	D3DPRIMITIVETYPE*   m_pePrimitiveTypeUPIndexed;          /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	UINT*               m_pdwMinVertexIndex;                 /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	UINT*               m_pdwNumVerticesUPIndexed;           /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	UINT*               m_pdwPrimitiveCountUPIndexed;        /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	void**              m_ppIndexData;                       /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	D3DFORMAT*          m_peIndexDataFormat;                 /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	void**              m_ppVertexStreamZeroDataIndexed;     /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	UINT*               m_pdwVertexStreamZeroStrideIndexed;  /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/

	/**
	* Active stored render targets.
	* The render targets that are currently in use.
	***/
	std::vector<IDirect3DSurface9*> m_apcActiveRenderTargets;
	/**
	* Active stored textures.
	* The textures that are currently in use.
	***/
	std::vector<IDirect3DBaseTexture9*> m_apcActiveTextures;
	/**
	* Active stored render targets.
	* The depth stencil surface that are currently in use.
	***/
	IDirect3DSurface9* m_pcActiveDepthStencilSurface;
	/**
	* Active back buffer.
	* The back buffer surface that is currently in use.
	***/
	IDirect3DSurface9* m_pcActiveBackBufferSurface;
	/**
	* Monitored stored render targets.
	* The render targets that are currently watched or monitored.
	* These render targets have been used for the last period of 
	* frames.
	***/
	std::vector<IDirect3DSurface9*> m_apcMonitoredSurfaces;
	/**
	* Stereo twin render targets.
	* Each entry in this vector corresponds as the stereo twin
	* of the render target stored in m_apcMonitoredSurfaces
	* with the same index.
	***/
	std::vector<IDirect3DSurface9*> m_apcStereoTwinSurfaces;
	/**
	* Stereo twin render textures.
	* Each entry in this vector corresponds as the the texture of 
	* the stereo twin surface with the same index (stored in 
	* m_apcStereoTwinSurfaces).
	***/
	std::vector<IDirect3DTexture9*> m_apcStereoTwinTextures;
	/**
	* Twin for active render target. 
	* Entries ALLWAYS also exist in m_apcStereoTwinSurfaces.
	***/
	std::vector<IDirect3DSurface9*> m_apcActiveStereoTwinRenderTarget;	
	/**
	* Twin for active texture. 
	* Entries ALLWAYS also exist in m_apcStereoTwinSurfaces.
	***/
	std::vector<IDirect3DTexture9*> m_apcActiveStereoTwinTextures;
	/**
	* Twin for active depth stencil. 
	* Entry ALLWAYS also exist in m_apcStereoTwinSurfaces.
	***/
	IDirect3DSurface9* m_pcActiveStereoTwinDepthStencilSurface;
	/**
	* Twin for active back buffer.
	* Entry ALLWAYS also exist in m_apcStereoTwinSurfaces.
	***/
	IDirect3DSurface9* m_pcActiveStereoTwinBackBufferSurface;
	/**
	* Monitored render targets check time counter.
	* Each index of the vector array represents the frame counter
	* the render target stored in m_pcMonitoredRenderTargets will
	* be watched. If this render target is in use again this counter 
	* will be set to the check time counter constant set in 
	* m_nChecktimeFrameConstant.
	***/
	std::vector<int> m_anMonitoredRenderTargetsCheckTimeCounter;
	/**
	* Monitored render target check time constant (in frames).
	* Time (in frames) any render target will be (at least) monitored.
	* If any render target is set again, its check time counter
	* will be resetted to this value.
	***/
	int m_nChecktimeFrameConstant;	
	/**
	* The number of stereo twin render targets to be verified this frame.
	***/
	DWORD m_dwNewStereoTwinRenderTargets;
	/**
	* Currently unused render targets.
	* To be taken as new render targets if needed.
	***/
	std::vector<IDirect3DSurface9*> m_apcStereoTwinRenderTargetClipboard;
	/**
	* Currently unused render textures.
	* Each entry in this vector corresponds as the the texture of 
	* the stereo twin surface with the same clipboard index.
	* (m_apcStereoTwinRenderTargetClipboard)
	***/
	std::vector<IDirect3DTexture9*> m_apcStereoTwinRenderTextureClipboard;
	/**
	* Maximum render targets, as read from 
	***/
	DWORD m_dwMaxRenderTargets;
	/**
	* True if max render targets vector is initialized.
	***/
	bool m_bMaxRenderTargets;
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
	* The output texture (left). TO BE MOVED TO PRESENTER
	***/
	LPDIRECT3DTEXTURE9 m_pcStereoOutputLeft;
	/**
	* The output texture (right).
	***/
	LPDIRECT3DTEXTURE9 m_pcStereoOutputRight;
	/**
	* The output surface (left).
	***/
	LPDIRECT3DSURFACE9 m_pcStereoOutputSurfaceLeft;
	/**
	* The output surface (right).
	***/
	LPDIRECT3DSURFACE9 m_pcStereoOutputSurfaceRight;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

