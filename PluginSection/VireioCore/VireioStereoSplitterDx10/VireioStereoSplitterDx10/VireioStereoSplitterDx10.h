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

#include<stdio.h>
#include<vector>

#include"..//..//..//..//Aquilinus//Aquilinus//AQU_Nodus.h"
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

#include "directxmath.h"

#define VIREIO_D3D11
#include"..//..//..//..//Aquilinus/Aquilinus/ITA_D3D11Interfaces.h"
#include"..//..//..//..//Aquilinus/Aquilinus/ITA_DXGIInterfaces.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_ID3D11DeviceContext.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDXGISwapChain.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"..\..\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierClasses.h"

#define NUMBER_OF_COMMANDERS    1
#define NUMBER_OF_DECOMMANDERS 28

#define STEREO_L 0
#define STEREO_R 1
#define MONO     1
#define STEREO   2
#define TARGET_REGISTER_L_11 0
#define TARGET_REGISTER_R_11 D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
#define RESOURCE_REGISTER_L_11 0
#define RESOURCE_REGISTER_R_11 D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum struct STS_Commanders : int
{
	StereoData_Splitter
};

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum struct STS_Decommanders : int
{
	Modifier,
	CSSetConstantBuffers,
	CSSetSamplers,
	CSSetShader,
	CSSetShaderResources,
	CSSetUnorderedAccessViews,
	ClearDepthStencilView,
	ClearRenderTargetView,
	ClearState,
	Dispatch,
	DispatchIndirect,
	Draw,
	DrawAuto,
	DrawIndexed,
	DrawIndexedInstanced,
	DrawIndexedInstancedIndirect,
	DrawInstanced,
	DrawInstancedIndirect,
	Map,
	OMGetRenderTargets,
	OMGetRenderTargetsAndUnorderedAccessViews,
	OMSetRenderTargets,
	OMSetRenderTargetsAndUnorderedAccessViews,
	PSGetShaderResources,
	PSSetShaderResources,
	Present,
	Unmap,
	VSSetShader
};

/// <summary>
/// Simple D3D10/11 enumeration
/// </summary>
enum struct D3DVersion : unsigned
{
	NotDefined = 0,
	Direct3D10 = 10,
	Direct3D11 = 11,
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
	virtual       DWORD GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual       DWORD GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual      LPWSTR GetCommanderName(DWORD uCommanderIndex);
	virtual      LPWSTR GetDecommanderName(DWORD uDecommanderIndex);
	virtual       DWORD GetCommanderType(DWORD uCommanderIndex);
	virtual       DWORD GetDecommanderType(DWORD uDecommanderIndex);
	virtual       void* GetOutputPointer(DWORD uCommanderIndex);
	virtual        void SetInputPointer(DWORD uDecommanderIndex, void* pData);
	virtual        bool SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual       void* Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);
	virtual        void UpdateImGuiControl(float fZoom);

private:
	/*** StereoSplitter private D3D10+ methods ***/
	void                    XSSetShaderResourceViews(std::array<ID3D11ShaderResourceView*, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT* STEREO>& apcActiveShaderResourceViews, UINT& unNumViewsTotal, UINT unStartSlot, UINT unNumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);

	/*** StereoSplitter private methods ***/
	void                    CreateStereoView(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11View* pcView);
	ID3D11RenderTargetView* VerifyPrivateDataInterfaces(ID3D11RenderTargetView* pcRenderTargetView);
	ID3D11DepthStencilView* VerifyPrivateDataInterfaces(ID3D11DepthStencilView* pcDepthStencilView);
	ID3D11UnorderedAccessView* VerifyPrivateDataInterfaces(ID3D11UnorderedAccessView* pcUnorderedAccessView);
	bool                    SetDrawingSide(ID3D10Device* pcDevice, RenderPosition side);
	bool                    SetDrawingSide(ID3D11DeviceContext* pcContext, RenderPosition side);
	void                    SetDrawingSideField(RenderPosition eSide) { m_eCurrentRenderingSide = eSide; if (m_psModifierData) m_psModifierData->eCurrentRenderingSide = eSide; }

	/// <summary>
	/// [OUT] All data this node outputs.
	/// </summary>
	StereoData m_sStereoData;
	/// <summary>
	/// [IN] Input pointers array.
	/// </summary>
	void* m_ppInput[NUMBER_OF_DECOMMANDERS];

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
	/// 0---------------------------------------------> D3D1X_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	/// D3D1X_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D1X_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	/// </summary>
	union
	{
		std::array<ID3D10RenderTargetView*, D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT* STEREO> m_apcActiveRenderTargetViews10;
		std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT* STEREO> m_apcActiveRenderTargetViews11;
	};
	/// <summary>
	/// Active stored texture views.
	/// The textures that are currently in use.
	/// DX11 :
	/// 0----------------------------------------------> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ----- Left shader resource views
	/// D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT---> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2 - Right shader resource views
	/// </summary>
	std::array<ID3D11ShaderResourceView*, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT* STEREO> m_apcActivePSShaderResourceViews;
	/// <summary>
	/// Active stored compute shader resource views.
	/// The textures (INPUT!!) and buffers that are currently in use.
	/// DX11 :
	/// 0----------------------------------------------> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ----- Left shader resource views
	/// D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT---> D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT * 2 - Right shader resource views
	/// </summary>
	std::array<ID3D11ShaderResourceView*, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT* STEREO> m_apcActiveCSShaderResourceViews;
	/// <summary>
	/// The d3d11 active constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcActiveCSConstantBuffers;
	/// <summary>
	/// The d3d11 active unordered access views, for left and right side.
	/// 0 ------------------------------> D3D11_PS_CS_UAV_REGISTER_COUNT ----- Left views   (DX 11.0)
	/// D3D11_PS_CS_UAV_REGISTER_COUNT--> D3D11_PS_CS_UAV_REGISTER_COUNT * 2 - Right views. (DX 11.0)
	/// 0 ----------------------> D3D11_1_UAV_SLOT_COUNT ----- Left views   (DX 11.1)
	/// D3D11_1_UAV_SLOT_COUNT--> D3D11_1_UAV_SLOT_COUNT * 2 - Right views. (DX 11.1)
	/// </summary>
	std::vector<ID3D11UnorderedAccessView*> m_apcActiveUnorderedAccessViews;
	/// <summary>
	/// Active stored depth stencil view (Stereo).
	/// The depth stencil surface that is currently in use.
	/// </summary>
	union
	{
		ID3D10DepthStencilView* m_apcActiveDepthStencilView10[STEREO];
		ID3D11DepthStencilView* m_apcActiveDepthStencilView11[STEREO];
	};
	/// <summary>
	/// Active back buffer.
	/// The back buffer surface that is currently in use.
	/// </summary>
	union
	{
		ID3D10Texture2D* m_pcActiveBackBuffer10;
		ID3D11Texture2D* m_pcActiveBackBuffer11;
	};
	/// <summary>
	/// Stereo twin for active back buffer.
	/// The back buffer surface that is currently in use.
	/// </summary>
	union
	{
		ID3D10Texture2D* m_pcActiveStereoTwinBackBuffer10;
		ID3D11Texture2D* m_pcActiveStereoTwinBackBuffer11;
	};
	/// <summary>
	/// Stereo twin view for active back buffer.
	/// The back buffer surface view that is currently in use.
	/// </summary>
	union
	{
		ID3D10RenderTargetView* m_pcActiveStereoTwinBackBufferView10;
		ID3D11RenderTargetView* m_pcActiveStereoTwinBackBufferView11;
	};
	/// <summary>
	/// New depth stencils.
	/// Processed in next Present() call.
	/// </summary>
	std::vector<ID3D11DepthStencilView*> m_apcNewDepthStencilViews11;
	/// <summary>
	/// New render targets.
	/// Processed in next Present() call.
	/// </summary>
	std::vector<ID3D11RenderTargetView*> m_apcNewRenderTargetViews11;
	/// <summary>
	/// New resource views (Texture).
	/// Processed in next Present() call.
	/// </summary>
	std::vector<ID3D11ShaderResourceView*> m_apcNewShaderResourceViews11;
	/// <summary>
	/// New unordered access views (Texture).
	/// Processed in next Present() call.
	/// </summary>
	std::vector<ID3D11UnorderedAccessView*> m_apcNewUnorderedAccessViews11;
	/// <summary>
	/// True if Present() was called at least once.
	/// Game can crash if Present() is not connected,
	/// so this is added for security.
	/// </summary>
	bool m_bPresent;
	/// <summary>
	/// State of the back buffer verification process.
	/// Eventually this will be optional, only relevant
	/// if back buffer gets discarded for each frame.
	/// </summary>
	enum struct BackBufferVerificationState : unsigned
	{
		NotVerified = 0, /// <summary>< The back buffer is not verified for the current frame. /// </summary>
		NewBuffer = 1,   /// <summary>< The back buffer is new for this frame, stereo buffer interfaces to be assigned. /// </summary>
		Verified = 2,    /// <summary>< The current back buffer is stereo. /// </summary>
	} m_eBackBufferVerified;
	/// <summary>
	/// Number of set PS Shader Resource Views.
	/// Number of textures not set to NULL.
	/// </summary>
	UINT m_dwPSShaderResourceViewsNumber;
	/// <summary>
	/// Number of set CS Shader Resource Views.
	/// Number of textures not set to NULL.
	/// </summary>
	UINT m_dwCSShaderResourceViewsNumber;
	/// <summary>
	/// Number of render targets.
	/// Number of render targets not set to NULL.
	/// </summary>
	UINT m_uRenderTargetNumber;
	/// <summary>
	/// Union, pointer is either context or swapchain.
	/// </summary>
	union
	{
		ID3D11DeviceContext* m_pcContextCurrent;
		IDXGISwapChain* m_pcSwapChainCurrent;
	};
	/// <summary>
	/// The number of frames the constant buffers are to be verified.
	/// </summary>
	UINT m_dwVerifyConstantBuffers;
	/// <summary>
	/// A needless field of "-1" UINTs needed for CSSetUnorderedAccessViews.
	/// </summary>
	UINT m_aunMinusOne[D3D11_PS_CS_UAV_REGISTER_COUNT];
	/// <summary>
	/// True if render target was switched.
	/// </summary>
	bool m_bRenderTargetWasSwitched;
	/// <summary>
	/// Vireio menu.
	/// </summary>
	VireioSubMenu m_sMenu;
	/// <summary>
	/// Frames to save the ini file.
	/// </summary>
	INT m_nIniFrameCount;

	void    CSSetConstantBuffers(int& nFlags);
	HRESULT CSSetSamplers(int& nFlags);
	HRESULT CSSetShader(int& nFlags);
	void    CSSetShaderResources(int& nFlags);
	void    CSSetUnorderedAccessViews(UINT uStartSlot, UINT uNumUAVs, ID3D11UnorderedAccessView** ppcUnorderedAccessViews, UINT* puUAVInitialCounts);
	void    CSSetUnorderedAccessViews(int& nFlags);
	void    ClearDepthStencilView(int& nFlags);
	void    ClearRenderTargetView(int& nFlags);
	HRESULT ClearState(int& nFlags);
	void    Dispatch(int& nFlags);
	void    DispatchIndirect(int& nFlags);
	HRESULT Draw(int& nFlags);
	HRESULT DrawAuto(int& nFlags);
	HRESULT DrawIndexed(int& nFlags);
	HRESULT DrawIndexedInstanced(int& nFlags);
	HRESULT DrawIndexedInstancedIndirect(int& nFlags);
	HRESULT DrawInstanced(int& nFlags);
	HRESULT DrawInstancedIndirect(int& nFlags);
	HRESULT Map(int& nFlags);
	HRESULT OMGetRenderTargets(int& nFlags);
	HRESULT OMGetRenderTargetsAndUnorderedAccessViews(int& nFlags);
	void    OMSetRenderTargets(UINT uNumViews, ID3D11RenderTargetView** ppcRenderTargetViews, ID3D11DepthStencilView* pcDepthStencilView);
	void    OMSetRenderTargets(int& nFlags);
	void    OMSetRenderTargetsAndUnorderedAccessViews(int& nFlags);
	HRESULT PSGetShaderResources(int& nFlags);
	void    PSSetShaderResources(int& nFlags);
	void    Present(int& nFlags);
	void    Unmap(int& nFlags);
	HRESULT VSSetShader(int& nFlags);
};

/// <summary>
/// Exported Constructor Method.
/// </summary>
extern "C" __declspec(dllexport) AQU_Nodus * AQU_Nodus_Create(ImGuiContext * sCtx)
{
	StereoSplitter* pStereoSplitter = new StereoSplitter(sCtx);
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}

