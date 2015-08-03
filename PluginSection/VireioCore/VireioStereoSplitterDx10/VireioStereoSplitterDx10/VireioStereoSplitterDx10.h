/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitterDx10.h> and
Class <VireioStereoSplitterDx10> :
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

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#define	FLOAT_PLUG_TYPE                                4
#define INT_PLUG_TYPE                                  7 
#define UINT_PLUG_TYPE                                12
#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_VOID_PLUG_TYPE                           114
#define	D3DCOLOR_PLUG_TYPE                          1004
#define D3DFORMAT_PLUG_TYPE                         1011
#define D3DPRIMITIVETYPE_PLUG_TYPE                  1021
#define PNT_D3DRECT_PLUG_TYPE                       2024
#define PNT_ID3D10DEPTHSTENCILVIEW                  8021
#define PNT_ID3D10RENDERTARGETVIEW_TYPE             8022
#define PPNT_ID3D10DEPTHSTENCILVIEW                 9021
#define PPNT_ID3D10RENDERTARGETVIEW                 9022
#define PNT_ID3D11DEPTHSTENCILVIEW                 11025
#define PNT_ID3D11RENDERTARGETVIEW_TYPE            11026
#define PPNT_ID3D11DEPTHSTENCILVIEW                12025
#define PPNT_ID3D11RENDERTARGETVIEW                12026

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                         5

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
	/*** OMSetRenderTargets ***/
	NumViews,                                              /** Number of render targets to bind. **/
	ppRenderTargetViews_DX10,                              /** Pointer to an array of render targets (see ID3D10RenderTargetView) to bind to the device. **/
	pDepthStencilView_DX10,                                /** Pointer to a depth-stencil view (see ID3D10DepthStencilView) to bind to the device.**/
	ppRenderTargetViews_DX11,                              /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
	pDepthStencilView_DX11,                                /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
	/*** All Draw methods ***/
	DrawIndexed_IndexCount,                                /** Number of indices to draw. */
	DrawIndexed_StartIndexLocation,                        /** The location of the first index */
	DrawIndexed_BaseVertexLocation,                        /** A value added to each index. */
	Draw_VertexCount,                                      /** Number of vertices to draw. */
	Draw_StartVertexLocation,                              /** Index of the first vertex */
	DrawIndexedInstanced_IndexCountPerInstance,            /** Number of indices read */
	DrawIndexedInstanced_InstanceCount,                    /** Number of instances to draw. */
	DrawIndexedInstanced_StartIndexLocation,               /** The location of the first index read. */
	DrawIndexedInstanced_BaseVertexLocation,               /** A value added to each index. */
	DrawIndexedInstanced_StartInstanceLocation,            /** A value added to each index */
	DrawInstanced_VertexCountPerInstance,                  /** Number of vertices to draw */
	DrawInstanced_InstanceCount,                           /** Number of instances to draw */
	DrawInstanced_StartVertexLocation,                     /** Index of the first vertex */
	DrawInstanced_StartInstanceLocation,                   /** A value added to each index */
	DrawIndexedInstancedIndirect_pBufferForArgs,           /** A pointer to an ID3D11Buffer, which is a buffer containing the GPU generated primitives. */
	DrawIndexedInstancedIndirect_AlignedByteOffsetForArgs, /** Offset in pBufferForArgs to the start of the GPU generated primitives. */
	DrawInstancedIndirect_pBufferForArgs,                  /** A pointer to an ID3D11Buffer, which is a buffer containing the GPU generated primitives. */
	DrawInstancedIndirect_AlignedByteOffsetForArgs,        /** Offset in pBufferForArgs to the start of the GPU generated primitives. */
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
* Simple D3D10/11 enumeration
***/
enum D3DVersion
{
	NotDefined,
	Direct3D10,
	Direct3D11,
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
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** StereoSplitter private D3D10+ stub methods ***/
	void                    Present(IDXGISwapChain* pcSwapChain);

	/*** StereoSplitter private D3D10+ methods ***/
	void                    OMSetRenderTargets(UINT NumViews, IUnknown *const *ppRenderTargetViews, IUnknown *pDepthStencilView);
	void                    DrawIndexed(ID3D11DeviceContext *pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void                    Draw(ID3D11DeviceContext *pcThis, UINT VertexCount, UINT StartVertexLocation);
	void                    DrawIndexedInstanced(ID3D11DeviceContext *pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	void                    DrawInstanced(ID3D11DeviceContext *pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void                    DrawAuto(ID3D11DeviceContext *pcThis);
	void                    DrawIndexedInstancedIndirect(ID3D11DeviceContext *pcThis, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void                    DrawInstancedIndirect(ID3D11DeviceContext *pcThis, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);

	/*** StereoSplitter private methods ***/
	int                     CheckIfMonitored(IUnknown* pcView);
	void                    MonitorView(IUnknown* pcView);
	bool                    SetDrawingSide(RenderPosition side);

	/**
	* Input pointers.
	***/
	DWORD* m_pdwNumViews;                                             /** Number of render targets to bind. **/
	IUnknown*** m_pppcRenderTargetViews_DX10;                         /** Pointer to an array of render targets (see ID3D10RenderTargetView) to bind to the device. **/
	IUnknown** m_ppcDepthStencilView_DX10;                            /** Pointer to a depth-stencil view (see ID3D10DepthStencilView) to bind to the device.**/
	IUnknown** m_pppcRenderTargetViews_DX11;                          /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
	IUnknown** m_ppcDepthStencilView_DX11;                            /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
	UINT* m_pdwDrawIndexed_IndexCount;                                /** Number of indices to draw. */
	UINT* m_pdwDrawIndexed_StartIndexLocation;                        /** The location of the first index */
	INT* m_pnDrawIndexed_BaseVertexLocation;                          /** A value added to each index. */
	UINT* m_pdwDraw_VertexCount;                                      /** Number of vertices to draw. */
	UINT* m_pdwDraw_StartVertexLocation;                              /** Index of the first vertex */
	UINT* m_pdwDrawIndexedInstanced_IndexCountPerInstance;            /** Number of indices read */
	UINT* m_pdwDrawIndexedInstanced_InstanceCount;                    /** Number of instances to draw. */
	UINT* m_pdwDrawIndexedInstanced_StartIndexLocation;               /** The location of the first index read. */
	INT* m_pnDrawIndexedInstanced_BaseVertexLocation;                 /** A value added to each index. */
	UINT* m_pdwDrawIndexedInstanced_StartInstanceLocation;            /** A value added to each index */
	UINT* m_pdwDrawInstanced_VertexCountPerInstance;                  /** Number of vertices to draw */
	UINT* m_pdwDrawInstanced_InstanceCount;                           /** Number of instances to draw */
	UINT* m_pdwDrawInstanced_StartVertexLocation;                     /** Index of the first vertex */
	UINT* m_pdwDrawInstanced_StartInstanceLocation;                   /** A value added to each index */
	ID3D11Buffer** m_ppcDrawIndexedInstancedIndirect_pBufferForArgs;  /** A pointer to an ID3D11Buffer, which is a buffer containing the GPU generated primitives. */
	UINT* m_pdwDrawIndexedInstancedIndirect_AlignedByteOffsetForArgs; /** Offset in pBufferForArgs to the start of the GPU generated primitives. */
	ID3D11Buffer** m_ppcDrawInstancedIndirect_pBufferForArgs;         /** A pointer to an ID3D11Buffer, which is a buffer containing the GPU generated primitives. */
	UINT* m_pdwDrawInstancedIndirect_AlignedByteOffsetForArgs;        /** Offset in pBufferForArgs to the start of the GPU generated primitives. */

	/**
	* Active stored render target views.
	* The render targets that are currently in use.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown *> m_apcActiveRenderTargetViews;
	/**
	* Active stored textures.
	* The textures that are currently in use.
	* (IUnknown*) for campatibility to DX10+DX11.
	* TODO !! TEXTURE VIEW ???
	***/
	std::vector<IUnknown*> m_apcActiveTextures;
	/**
	* Active stored depth stencil view.
	* The depth stencil surface that is currently in use.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	IUnknown* m_pcActiveDepthStencilView;
	/**
	* Active back buffer.
	* The back buffer surface that is currently in use.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	IUnknown* m_pcActiveBackBufferSurface;
	/**
	* Monitored stored views (both render target and depth stencil).
	* The render targets that are currently watched or monitored.
	* These render targets have been used for the last period of
	* frames.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcMonitoredViews;
	/**
	* Stereo twin render targets.
	* Each entry in this vector corresponds as the stereo twin
	* of the render target stored in m_apcMonitoredViews
	* with the same index.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcStereoTwinViews;
	/**
	* Stereo twin render textures.
	* Each entry in this vector corresponds as the the texture of
	* the stereo twin surface with the same index (stored in
	* m_apcStereoTwinViews).
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcStereoTwinTextures;
	/**
	* Twin for active render target.
	* Entries ALLWAYS also exist in m_apcStereoTwinViews.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcActiveStereoTwinViews;
	/**
	* Twin for active texture.
	* Entries ALLWAYS also exist in m_apcStereoTwinTextures.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcActiveStereoTwinTextures;
	/**
	* Twin for active depth stencil.
	* Entry ALLWAYS also exist in m_apcStereoTwinViews.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	IUnknown* m_pcActiveStereoTwinDepthStencilView;
	/**
	* Twin for active back buffer.
	* Entry ALLWAYS also exist in m_apcStereoTwinViews.
	***/
	//IUnknown* m_pcActiveStereoTwinBackBuffers;
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
	* Currently unused render target views (=clipboard).
	* To be taken as new render targets if needed.
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> apcStereoTwinRenderTargetViewClipboard;
	/**
	* Currently unused render textures (=clipboard).
	* Each entry in this vector corresponds as the the texture of
	* the stereo twin view with the same clipboard index.
	* (apcStereoTwinRenderTargetViewClipboard)
	* (IUnknown*) for campatibility to DX10+DX11.
	***/
	std::vector<IUnknown*> m_apcStereoTwinRenderTextureClipboard;
	/**
	* True if Present() was called at least once.
	* Game can crash if Present() is not connected,
	* so this is added for security.
	***/
	bool m_bPresent;
	/**
	* Number of set textures.
	* Number of textures not set to NULL.
	***/
	DWORD m_dwTextureNumber;
	/**
	* Number of render targets.
	* Number of render targets not set to NULL.
	***/
	DWORD m_dwRenderTargetNumber;
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
	* The font used.
	***/
	HFONT m_hFont;
	/**
	* The used Direct3D version.
	***/
	D3DVersion m_eD3DVersion;
	/**
	* The output texture (left). TO BE MOVED TO PRESENTER
	***/
	//LPDIRECT3DTEXTURE9 m_pcStereoOutputLeft;
	/**
	* The output texture (right).
	***/
	//LPDIRECT3DTEXTURE9 m_pcStereoOutputRight;
	/**
	* The output surface (left).
	***/
	//LPDIRECT3DSURFACE9 m_pcStereoOutputSurfaceLeft;
	/**
	* The output surface (right).
	***/
	//LPDIRECT3DSURFACE9 m_pcStereoOutputSurfaceRight;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

