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

#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierDataStructures.h"

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                         53

// enable for debug -> #define _DEBUG_VIREIO

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
	/*** OMSetRenderTargetsAndUnorderedAccessViews ***/
	NumRTVs,                                               /** Number of render targets to bind. **/
	ppRenderTargetViewsUAV_DX11,                           /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
	pDepthStencilViewUAV_DX11,                             /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
	UAVStartSlot,
	NumUAVs,
	ppUnorderedAccessViews,
	pUAVInitialCounts,
	/*** ClearRenderTargetView ***/
	pRenderTargetView_DX10,                                /** Pointer to the render target. */
	pRenderTargetView_DX11,                                /** Pointer to the render target. */
	ColorRGBA,                                             /** A 4-component array that represents the color to fill the render target with. */
	/*** ClearDepthStencilView ***/
	pDepthStencilViewCDS_DX10,                             /** Pointer to the depth stencil to be cleared. */
	pDepthStencilViewCDS_DX11,                             /** Pointer to the depth stencil to be cleared. */
	ClearFlags,                                            /** Identify the type of data to clear */
	Depth,                                                 /** Clear the depth buffer with this value. This value will be clamped between 0 and 1. */
	Stencil,                                               /** Clear the stencil buffer with this value. */
	/*** PSSetShaderResources ***/
	StartSlot,                                             /** Index into the device's zero-based array to begin setting shader resources to (ranges from 0 to D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1). **/
	NumSRVs,                                               /** Number of shader resources to set. Up to a maximum of 128 slots are available for shader resources (ranges from 0 to D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot). **/
	ppShaderResourceViews_DX10,                            /** Array of shader resource view interfaces to set to the device. **/
	ppShaderResourceViews_DX11,                            /** Array of shader resource view interfaces to set to the device. **/
	/*** Map ***/
	pResource,
	Subresource,
	/*** Unmap ***/
	pResource_Unmap,
	Subresource_Unmap,
	/*** Dispatch ***/
	ThreadGroupCountX,
	ThreadGroupCountY,
	ThreadGroupCountZ,
	/*** DispatchIndirect ***/
	pBufferForArgs,
	AlignedByteOffsetForArgs,
	/*** CSSetShaderResources ***/
	StartSlot_CS,
	NumViews_CS,
	ppShaderResourceViews,
	/*** CSSetUnorderedAccessViews ***/
	StartSlot_CSUAV,
	NumUAVs_CS,
	ppUnorderedAccessViews_CS,
	pUAVInitialCounts_CS,
	/*** CSSetConstantBuffers ***/
	StartSlot_CSCB,
	NumBuffers,
	ppConstantBuffers,
	/*** Active constant buffers ***/
	eDrawingSide,                                          /**< Left/Right drawing side enumeration. Switches once per draw call ***/
	ppActiveConstantBuffers_DX10_VertexShader,             /**< Active D3D10 vertex shader constant buffers ***/
	ppActiveConstantBuffers_DX10_GeometryShader,           /**< Active D3D10 geometry shader constant buffers ***/
	ppActiveConstantBuffers_DX10_PixelShader,              /**< Active D3D10 pixel shader constant buffers ***/
	ppActiveConstantBuffers_DX11_VertexShader,             /**< Active D3D11 vertex shader constant buffers ***/
	ppActiveConstantBuffers_DX11_HullShader,               /**< Active D3D11 hull shader constant buffers ***/
	ppActiveConstantBuffers_DX11_DomainShader,             /**< Active D3D11 domain shader constant buffers ***/
	ppActiveConstantBuffers_DX11_GeometryShader,           /**< Active D3D11 geometry shader constant buffers ***/
	ppActiveConstantBuffers_DX11_PixelShader,              /**< Active D3D11 pixel shader constant buffers ***/
	/*** Additional parameters ***/
	dwVerifyConstantBuffers,                               /**< The number of frames the constant buffers are to be verified **/
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
* Available stereo output modes (only monitor modes here).
***/
enum VireioMonitorStereoModes
{
	Vireio_Mono = 0,
	Vireio_SideBySide = 1,
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
	void                    OMSetRenderTargets(IUnknown* pcDeviceOrContext, UINT NumViews, IUnknown *const *ppRenderTargetViews, IUnknown *pDepthStencilView);
	void                    CSSetUnorderedAccessViews(ID3D11DeviceContext* pcContext, UINT dwStartSlot, UINT dwNumUAVs, ID3D11UnorderedAccessView *const *ppcUnorderedAccessViews, const UINT *pdwUAVInitialCounts);
	void                    XSSetShaderResourceViews(std::vector<ID3D11ShaderResourceView*> &apcActiveShaderResourceViews, UINT& unNumViewsTotal, UINT unStartSlot, UINT unNumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);

	/*** StereoSplitter private methods ***/
	void                       CreateStereoView(ID3D11Device* pcDevice, ID3D11View* pcView);
	ID3D11RenderTargetView*    VerifyPrivateDataInterfaces(ID3D11RenderTargetView* pcRenderTargetView);
	ID3D11DepthStencilView*    VerifyPrivateDataInterfaces(ID3D11DepthStencilView* pcDepthStencilView);
	ID3D11UnorderedAccessView* VerifyPrivateDataInterfaces(ID3D11UnorderedAccessView* pcUnorderedAccessView);
	bool                       SetDrawingSide(ID3D10Device* pcDevice, RenderPosition side);
	bool                       SetDrawingSide(ID3D11DeviceContext* pcContext, RenderPosition side);
	void                       SetDrawingSideField(RenderPosition eSide) { m_eCurrentRenderingSide = eSide; if (m_peDrawingSide) *m_peDrawingSide = eSide; }

	/**
	* Input pointers.
	***/
	DWORD* m_pdwNumViews;                                             /** Number of render targets to bind. **/
	IUnknown*** m_pppcRenderTargetViews_DX10;                         /** Pointer to an array of render targets (see ID3D10RenderTargetView) to bind to the device. **/
	IUnknown** m_ppcDepthStencilView_DX10;                            /** Pointer to a depth-stencil view (see ID3D10DepthStencilView) to bind to the device.**/
	IUnknown*** m_pppcRenderTargetViews_DX11;                         /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
	IUnknown** m_ppcDepthStencilView_DX11;                            /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
	DWORD* m_pdwNumRTVs;                                              /** Number of render targets to bind. **/
	IUnknown*** m_pppcRenderTargetViewsUAV_DX11;                      /** Pointer to an array of ID3D11RenderTargetView that represent the render targets to bind to the device. **/
	IUnknown** m_ppcDepthStencilViewUAV_DX11;                         /** Pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. **/
	ID3D10RenderTargetView** m_ppcRenderTargetView_DX10;              /** Pointer to the render target. */
	ID3D11RenderTargetView** m_ppcRenderTargetView_DX11;              /** Pointer to the render target. */
	float** m_ppfColorRGBA;                                           /** A 4-component array that represents the color to fill the render target with. */
	ID3D10DepthStencilView** m_ppcDepthStencilViewClear_DX10;         /** Pointer to the depth stencil to be cleared. */
	ID3D11DepthStencilView** m_ppcDepthStencilViewClear_DX11;         /** Pointer to the depth stencil to be cleared. */
	UINT* m_pdwClearFlags;                                            /** Identify the type of data to clear */
	FLOAT* m_pfDepth;                                                 /** Clear the depth buffer with this value. This value will be clamped between 0 and 1. */
	UINT8* m_pchStencil;                                              /** Clear the stencil buffer with this value. */
	RenderPosition* m_peDrawingSide;                                  /** Pointer to the extern drawing side bool. The extern bool will be updated depending on m_eCurrentRenderingSide ***/
	ID3D11Buffer*** m_appcVSActiveConstantBuffers11;                  /** The d3d11 vertex shader constant buffer array. ***/
	ID3D11Buffer*** m_appcHSActiveConstantBuffers11;                  /** The d3d11 hull shader constant buffer array. ***/
	ID3D11Buffer*** m_appcDSActiveConstantBuffers11;                  /** The d3d11 domain shader constant buffer array. ***/
	ID3D11Buffer*** m_appcGSActiveConstantBuffers11;                  /** The d3d11 geometry shader constant buffer array. ***/
	ID3D11Buffer*** m_appcPSActiveConstantBuffers11;                  /** The d3d11 pixel shader constant buffer array. ***/
	UINT* m_pdwStartSlot;                                             /** Index into the device's zero-based array to begin setting shader resources to (ranges from 0 to D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1). **/
	UINT* m_pdwNumViewsSRVs;                                          /** Number of shader resources to set. Up to a maximum of 128 slots are available for shader resources (ranges from 0 to D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot). **/
	ID3D10ShaderResourceView*** m_pppcShaderResourceViews10;          /** Array of shader resource view interfaces to set to the device. **/
	ID3D11ShaderResourceView*** m_pppcShaderResourceViews11;          /** Array of shader resource view interfaces to set to the device. **/
	ID3D11Resource **m_ppcResource;
	UINT *m_pdwSubresource;
	ID3D11Resource **m_ppcResource_Unmap;
	UINT *m_pdwSubresource_Unmap;

	UINT* m_pdwUAVStartSlot;
	UINT* m_pdwNumUAVs;
	ID3D11UnorderedAccessView*** m_pppcUnorderedAccessViews;
	UINT** m_ppdwUAVInitialCounts;

	UINT* m_pdwThreadGroupCountX;
	UINT* m_pdwThreadGroupCountY;
	UINT* m_pdwThreadGroupCountZ;

	ID3D11Buffer** m_ppcBufferForArgs;
	UINT* m_pdwpAlignedByteOffsetForArgs;

	UINT* m_pdwStartSlot_CS;
	UINT* m_pdwNumViews_CS;
	ID3D11ShaderResourceView*** m_pppcShaderResourceViews;

	UINT* m_pdwStartSlot_CSUAV;
	UINT* m_pdwNumUAVs_CS;
	ID3D11UnorderedAccessView*** m_pppcUnorderedAccessViews_CS;
	UINT** m_ppdwUAVInitialCounts_CS;

	UINT* m_pdwStartSlot_CSCB;
	UINT* m_pdwNumBuffers;
	ID3D11Buffer*** m_pppcConstantBuffers;

	UINT* m_pdwVerifyConstantBuffers;                                 /** The number of frames the constant buffers are to be verified. ***/

	/**
	* Active stored render target views.
	* The render targets that are currently in use.
	* (IUnknown*) for compatibility to DX10+DX11.
	* DX11 :
	* 0---------------------------------------------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	* D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	***/
	std::vector<IUnknown *> m_apcActiveRenderTargetViews;
	/**
	* Active stored texture views.
	* The textures that are currently in use.
	* (IUnknown*) for compatibility to DX10+DX11.
	* DX11 :
	* 0----------------------------------------------> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ----- Left shader resource views
	* D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT---> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2 - Right shader resource views
	***/
	std::vector<ID3D11ShaderResourceView*> m_apcActivePSShaderResourceViews;
	/**
	* Active stored compute shader resource views.
	* The textures (INPUT!!) and buffers that are currently in use.
	* DX11 :
	* 0----------------------------------------------> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ----- Left shader resource views
	* D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT---> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2 - Right shader resource views
	***/
	std::vector<ID3D11ShaderResourceView*> m_apcActiveCSShaderResourceViews;
	/**
	* The d3d11 active constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcActiveCSConstantBuffers;
	/**
	* The d3d11 active unordered access views, for left and right side.
	* 0 ------------------------------> D3D11_PS_CS_UAV_REGISTER_COUNT ----- Left views   (DX 11.0)
	* D3D11_PS_CS_UAV_REGISTER_COUNT--> D3D11_PS_CS_UAV_REGISTER_COUNT * 2 - Right views. (DX 11.0)
	* 0 ----------------------> D3D11_1_UAV_SLOT_COUNT ----- Left views   (DX 11.1)
	* D3D11_1_UAV_SLOT_COUNT--> D3D11_1_UAV_SLOT_COUNT * 2 - Right views. (DX 11.1)
	***/
	std::vector<ID3D11UnorderedAccessView*> m_apcActiveUnorderedAccessViews;
	/**
	* Active stored depth stencil view.
	* The depth stencil surface that is currently in use.
	* (IUnknown*) for compatibility to DX10+DX11.
	***/
	IUnknown* m_pcActiveDepthStencilView;
	/**
	* Stereo twin for active depth stencil.
	* (IUnknown*) for compatibility to DX10+DX11.
	***/
	IUnknown* m_pcActiveStereoTwinDepthStencilView;
	/**
	* Active back buffer.
	* The back buffer surface that is currently in use.
	***/
	union
	{
		ID3D10Texture2D* m_pcActiveBackBuffer10;
		ID3D11Texture2D* m_pcActiveBackBuffer11;
	};
	/**
	* Stereo twin for active back buffer.
	* The back buffer surface that is currently in use.
	***/
	union
	{
		ID3D10Texture2D* m_pcActiveStereoTwinBackBuffer10;
		ID3D11Texture2D* m_pcActiveStereoTwinBackBuffer11;
	};
	/**
	* Stereo twin view for active back buffer.
	* The back buffer surface view that is currently in use.
	***/
	union
	{
		ID3D10RenderTargetView* m_pcActiveStereoTwinBackBufferView10;
		ID3D11RenderTargetView* m_pcActiveStereoTwinBackBufferView11;
	};
	/**
	* Active output textures (shader bind flag), for both eyes.
	* The back buffer surface copies.
	***/
	union
	{
		ID3D10Texture2D* m_pcTex10[2];
		ID3D11Texture2D* m_pcTex11[2];
	};
	/**
	* Active back buffer view, for both eyes.
	* The back buffer surface copy views.
	***/
	union
	{
		ID3D10ShaderResourceView* m_pcTexView10[2];
		ID3D11ShaderResourceView* m_pcTexView11[2];
	};
	/**
	* New depth stencils.
	* Processed in next Present() call.
	***/
	std::vector<ID3D11DepthStencilView*> m_apcNewDepthStencilViews11;
	/**
	* New render targets.
	* Processed in next Present() call.
	***/
	std::vector<ID3D11RenderTargetView*> m_apcNewRenderTargetViews11;
	/**
	* New resource views (Texture).
	* Processed in next Present() call.
	***/
	std::vector<ID3D11ShaderResourceView*> m_apcNewShaderResourceViews11;
	/**
	* New unordered access views (Texture).
	* Processed in next Present() call.
	***/
	std::vector<ID3D11UnorderedAccessView*> m_apcNewUnorderedAccessViews11;
	/**
	* True if Present() was called at least once.
	* Game can crash if Present() is not connected,
	* so this is added for security.
	***/
	bool m_bPresent;
	/**
	* State of the back buffer verification process.
	* Eventually this will be optional, only relevant
	* if back buffer gets discarded for each frame.
	***/
	enum BackBufferVerificationState
	{
		NotVerified = 0,                       /**< The back buffer is not verified for the current frame. ***/
		NewBuffer = 1,                         /**< The back buffer is new for this frame, stereo buffer interfaces to be assigned. ***/
		Verified = 2,                          /**< The current back buffer is stereo. ***/
	} m_eBackBufferVerified;
	/**
	* Number of set PS Shader Resource Views.
	* Number of textures not set to NULL.
	***/
	UINT m_dwPSShaderResourceViewsNumber;
	/**
	* Number of set CS Shader Resource Views.
	* Number of textures not set to NULL.
	***/
	UINT m_dwCSShaderResourceViewsNumber;
	/**
	* Number of render targets.
	* Number of render targets not set to NULL.
	***/
	UINT m_dwRenderTargetNumber;
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
	* The number of frames the constant buffers are to be verified.
	***/
	UINT m_dwVerifyConstantBuffers;
	/**
	* A needless field of "-1" UINTs needed for CSSetUnorderedAccessViews.
	***/
	UINT m_aunMinusOne[D3D11_PS_CS_UAV_REGISTER_COUNT];

	/*** Optional draw operation fields ***/

	/**
	* The back buffer render target view (DX11).
	***/
	ID3D11RenderTargetView* m_pcBackBufferView;
	/**
	* The 2D vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcVertexShader10;
		ID3D11VertexShader* m_pcVertexShader11;
	};
	/**
	* The 2D pixel shader.
	***/
	union
	{
		ID3D10PixelShader* m_pcPixelShader10;
		ID3D11PixelShader* m_pcPixelShader11;
	};
	/**
	* The 2D vertex layout.
	***/
	union
	{
		ID3D10InputLayout* m_pcVertexLayout10;
		ID3D11InputLayout* m_pcVertexLayout11;
	};
	/**
	* The 2D vertex buffer.
	***/
	union
	{
		ID3D10Buffer* m_pcVertexBuffer10;
		ID3D11Buffer* m_pcVertexBuffer11;
	};
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	union
	{
		ID3D10Buffer* m_pcConstantBufferDirect10;
		ID3D11Buffer* m_pcConstantBufferDirect11;
	};
	/**
	* True if a stereo mode is selected.
	***/
	VireioMonitorStereoModes m_eStereoMode;
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

