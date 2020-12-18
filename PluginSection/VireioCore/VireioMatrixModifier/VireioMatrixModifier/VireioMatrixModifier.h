﻿/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.h> and
Class <VireioMatrixModifier> :
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
#include<map>
#include<vector>
#include<sstream>
#include<ctime>
#include<fstream>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
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
#elif defined(VIREIO_D3D9)
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#endif

#define VIREIO_SEED	                               12345                     /**< Do not change this !! ***/
#define VECTOR_LENGTH 4                                                      /**< One shader register has 4 float values. ***/
#define MAX_DX9_CONSTANT_REGISTERS                   224                     /**< Maximum shader registers for DX9 ***/
#define RegisterIndex(x) (x * VECTOR_LENGTH)                                 /**< Simple helper to access shader register. ***/

#include"AQU_Nodus.h"
#include"Resources.h"
#include"..//..//..//..//Aquilinus/Aquilinus/ITA_D3D9Interfaces.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DDevice9.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DSwapchain9.h"
#include"..//..//..//..//Aquilinus/Aquilinus/VMT_IDirect3DStateBlock9.h"
#include"..\..\..\Include\Vireio_GameConfig.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"
#include"VireioMatrixModifierClasses.h"
#include"VireioMatrixModifierDataStructures.h"

#define	PROVOKING_TYPE                                 2                     /**< Provoking type is 2 - just invoker, no provoker **/
#define METHOD_REPLACEMENT                         false                     /**< This node does NOT replace the D3D call (default) **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#define NUMBER_OF_COMMANDERS                          22
#define NUMBER_OF_DECOMMANDERS                        53
#define GUI_WIDTH                                   1024                      
#define GUI_HEIGHT                                  5250               
#define CONSTANT_BUFFER_VERIFICATION_FRAME_NUMBER    100                     /**< If no shader data is present, the constant buffers are verified for 100 frames. ***/
#elif defined(VIREIO_D3D9)
#define NUMBER_OF_COMMANDERS                           1
#define NUMBER_OF_DECOMMANDERS                        21
#define GUI_HEIGHT                                   768
#endif

#define MAX_DATA_SIZE                              65535                     /**< Arbitrary... TODO !! set a maximum node data size **/

#define E_NO_MATCH         _HRESULT_TYPEDEF_(0x8A596AF85)

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }
void debugf(const char *fmt, ...) { va_list args; va_start(args, fmt); char buf[8192]; vsnprintf_s(buf, 8192, fmt, args); va_end(args); OutputDebugStringA(buf); }

/// <summary>
/// Simple hash code helper.
/// </summary>
inline uint32_t GetHashCode(BYTE* pcData, int32_t nLen, uint32_t uSeed)
{
	uint32_t uH = uSeed;

	// create hash
	for (int32_t i = 0; i < nLen; i++)
	{
		uH = 31 * uH + pcData[i];
	}

	return uH;
}

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum STS_Commanders
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	eDrawingSide,                                                           /**< Left/Right drawing side enumeration. Switches once per draw call ***/
	ppActiveConstantBuffers_DX10_VertexShader,                              /**< Active D3D10 vertex shader constant buffers ***/
	ppActiveConstantBuffers_DX10_GeometryShader,                            /**< Active D3D10 geometry shader constant buffers ***/
	ppActiveConstantBuffers_DX10_PixelShader,                               /**< Active D3D10 pixel shader constant buffers ***/
	ppActiveConstantBuffers_DX11_VertexShader,                              /**< Active D3D11 vertex shader constant buffers ***/
	ppActiveConstantBuffers_DX11_HullShader,                                /**< Active D3D11 hull shader constant buffers ***/
	ppActiveConstantBuffers_DX11_DomainShader,                              /**< Active D3D11 domain shader constant buffers ***/
	ppActiveConstantBuffers_DX11_GeometryShader,                            /**< Active D3D11 geometry shader constant buffers ***/
	ppActiveConstantBuffers_DX11_PixelShader,                               /**< Active D3D11 pixel shader constant buffers ***/
	dwVerifyConstantBuffers,                                                /**< Connect this commander to the stereo splitter to verify constant buffers ***/
	asVShaderData,                                                          /**< The shader data vector. ***/
	asPShaderData,                                                          /**< The shader data vector. ***/
	ViewAdjustments,                                                        /**< Shared pointer to the view adjustment class. ***/
	SwitchRenderTarget,                                                     /**< Option to switch the render target to exclude shaders beeing drawn. ***/
	RESERVED00,                                                             /**< RESERVED ***/
	SecondaryRenderTarget_DX10,                                             /**< Secondary (HUD) render target mono DX10. ***/
	SecondaryRenderTarget_DX11,                                             /**< Secondary (HUD) render target mono DX11. ***/
	ppActiveRenderTargets_DX10,                                             /**< Active render targets DX10. Backup for render target operations. ***/
	ppActiveRenderTargets_DX11,                                             /**< Active render targets DX11. Backup for render target operations. ***/
	ppActiveDepthStencil_DX10,                                              /**< Active depth stencil DX10. Backup for render target operations. ***/
	ppActiveDepthStencil_DX11,                                              /**< Active depth stencil DX11. Backup for render target operations. ***/
	VireioMenu,                                                             /**<  The Vireio Menu node connector. ***/
#elif defined(VIREIO_D3D9)
	Modifier
#endif
};

/// <summary>
/// Node Commander Enumeration.
/// </summary>
enum STS_Decommanders
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/*** D3D10 + D3D11 methods ***/
	pShaderBytecode_VertexShader,            /**< ID3D10Device::CreateVertexShader ***/
	BytecodeLength_VertexShader,             /**< ID3D10Device::CreateVertexShader ***/
	pClassLinkage_VertexShader,              /**< ID3D10Device::CreateVertexShader ***/
	ppVertexShader_DX10,                     /**< ID3D10Device::CreateVertexShader ***/
	pShaderBytecode_PixelShader,             /**< ID3D10Device::CreatePixelShader ***/
	BytecodeLength_PixelShader,              /**< ID3D10Device::CreatePixelShader ***/
	pClassLinkage_PixelShader,               /**< ID3D10Device::CreatePixelShader **/
	ppPixelShader_DX10,                      /**< ID3D10Device::CreatePixelShader ***/
	pVertexShader_10,                        /**< ID3D10Device/ID3D11DeviceContext::VSSetShader ***/
	pVertexShader_11,                        /**< ID3D10Device/ID3D11DeviceContext::VSSetShader ***/
	pPixelShader_10,                         /**< ID3D10Device/ID3D11DeviceContext::PSSetShader ***/
	pPixelShader_11,                         /**< ID3D10Device/ID3D11DeviceContext::PSSetShader ***/
	pDesc_DX10,                              /**< ID3D10Device::CreateBuffer ***/
	pInitialData_DX10,                       /**< ID3D10Device::CreateBuffer ***/
	ppBuffer_DX10,                           /**< ID3D10Device::CreateBuffer ***/
	StartSlot_VertexShader,                  /**< ID3D10Device/ID3D11DeviceContext::XSSetConstantBuffers ***/
	NumBuffers_VertexShader,                 /**< ID3D10Device/ID3D11DeviceContext::XSSetConstantBuffers ***/
	ppConstantBuffers_DX10_VertexShader,     /**< ID3D10Device/ID3D11DeviceContext::XSSetConstantBuffers ***/
	ppConstantBuffers_DX11_VertexShader,     /**< ID3D10Device/ID3D11DeviceContext::XSSetConstantBuffers ***/
	pDstResource_DX10,                       /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	pDstResource_DX11,                       /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	DstSubresource,                          /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	pDstBox_DX10,                            /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	pDstBox_DX11,                            /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	pSrcData,                                /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	SrcRowPitch,                             /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	SrcDepthPitch,                           /**< ID3D10Device/ID3D11DeviceContext::UpdateSubresource ***/
	pDstResource_DX10_Copy,                  /**< ID3D10Device/ID3D11DeviceContext::CopyResource ***/
	pSrcResource_DX10_Copy,                  /**< ID3D10Device/ID3D11DeviceContext::CopyResource ***/
	pDstResource_DX11_Copy,                  /**< ID3D10Device/ID3D11DeviceContext::CopyResource ***/
	pSrcResource_DX11_Copy,                  /**< ID3D10Device/ID3D11DeviceContext::CopyResource ***/
	pDstResource_DX10_CopySub,               /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	pDstResource_DX11_CopySub,               /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	DstSubresource_CopySub,                  /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	DstX,                                    /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	DstY,                                    /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	DstZ,                                    /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	pSrcResource_DX10_CopySub,               /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	pSrcResource_DX11_CopySub,               /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	SrcSubresource,                          /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	pSrcBox_DX10,                            /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	pSrcBox_DX11,                            /**< ID3D10Device/ID3D11DeviceContext::CopySubresourceRegion ***/
	StartSlot_Get_VertexShader,              /**< ID3D10Device/ID3D11DeviceContext::XSGetConstantBuffers ***/
	NumBuffers_Get_VertexShader,             /**< ID3D10Device/ID3D11DeviceContext::XSGetConstantBuffers ***/
	ppConstantBuffers_DX10_Get_VertexShader, /**< ID3D10Device/ID3D11DeviceContext::XSGetConstantBuffers ***/
	ppConstantBuffers_DX11_Get_VertexShader, /**< ID3D10Device/ID3D11DeviceContext::XSGetConstantBuffers ***/
	pResource,                               /**< ID3D11DeviceContext::Map ***/
	Subresource,                             /**< ID3D11DeviceContext::Map ***/
	MapType,                                 /**< ID3D11DeviceContext::Map ***/
	MapFlags,                                /**< ID3D11DeviceContext::Map ***/
	pMappedResource,                         /**< ID3D11DeviceContext::Map ***/
	pResource_Unmap,                         /**< ID3D11DeviceContext::Unmap ***/
	Subresource_Unmap,                       /**< ID3D11DeviceContext::Unmap ***/
#elif defined(VIREIO_D3D9)
	/*** D3D9 methods ***/
	SetVertexShader,
	SetPixelShader,
	SetTransform,
	MultiplyTransform,
	SetVertexShaderConstantF,
	GetVertexShaderConstantF,
	SetVertexShaderConstantI,
	GetVertexShaderConstantI,
	SetVertexShaderConstantB,
	GetVertexShaderConstantB,
	SetPixelShaderConstantF,
	GetPixelShaderConstantF,
	SetPixelShaderConstantI,
	GetPixelShaderConstantI,
	SetPixelShaderConstantB,
	GetPixelShaderConstantB,
	SetStreamSource,
	GetStreamSource,
	CreateVertexShader,
	CreatePixelShader,
	VB_Apply,
#endif
	// Tracker,                        /*<- Tracker input, not used now */
};

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
/// <summary>
* Vireio Map DX10/11 data structure.
* Contains all data for a mapped constant buffer.
/// </summary>
struct Vireio_Map_Data
{
	/// <summary>
	/// Stored mapped resource pointer. (DX11 only)
	/// </summary>
	ID3D11Resource* m_pcMappedResource;
	/// <summary>
	/// Stored mapped resource description.
	/// </summary>
	D3D11_MAPPED_SUBRESOURCE* m_psMappedResource;
	/// <summary>
	/// Stored mapped resource data pointer.
	/// </summary>
	void* m_pMappedResourceData;
	/// <summary>
	/// Stored mapped resource data size (in bytes).
	/// </summary>
	UINT m_dwMappedResourceDataSize;
	/// <summary>
	/// Stored map type.
	/// </summary>
	D3D11_MAP m_eMapType;
	/// <summary>
	/// Stored map flags.
	/// </summary>
	UINT m_dwMapFlags;
	/// <summary>
	/// The private data shader rules index for a mapped constant buffer.
	/// </summary>
	INT m_nMapRulesIndex;
	/// <summary>
	/// Constant Buffer private data buffer.
	/// Buffer data needed for Map(). (+0xff to provide homogenous address)
	/// </summary>
	union
	{
		BYTE m_pchBuffer10[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3) + 0xff];
		BYTE m_pchBuffer11[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3) + 0xff];
	};
};

/// <summary>
* Simple enumeration of supported Shaders.
/// </summary>
enum Vireio_Supported_Shaders
{
	VertexShader,
	PixelShader,
	GeometryShader,
	HullShader,
	DomainShader
};
#endif

/// <summary>
/// Vireio Matrix Modifier (DX9/10/11).
/// Vireio Perception Stereo Matrix Modification Handler.
/// </summary>
class MatrixModifier : public AQU_Nodus
{
public:
	MatrixModifier();
	virtual ~MatrixModifier();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return g_uGlobalNodeWidth; }
	virtual DWORD           GetNodeHeight() { return GUI_HEIGHT; }
	virtual int             GetProvokingType() { return PROVOKING_TYPE; }
	virtual bool            GetMethodReplacement() { return METHOD_REPLACEMENT; }
	virtual DWORD           GetSaveDataSize();
	virtual char*           GetSaveData(UINT* pdwSizeOfData);
	virtual void            InitNodeData(char* pData, UINT dwSizeOfData);
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
	virtual void            WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Return value pointer (HRESULT).
	/// </summary>
	void* m_pvReturn;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/// <summary>
	/// Constant Buffer Map data vector
	/// Contains all data for all currently mapped constant buffers.
	/// The size of this vector can be higher than m_dwMappedBuffers;
	/// </summary>
	std::vector<Vireio_Map_Data> m_asMappedBuffers;
#endif

private:

#if (defined(VIREIO_D3D11) || defined(VIREIO_D3D10))
	/*** MatrixModifier private methods ***/
	void XSSetConstantBuffers(ID3D11DeviceContext* pcContext, std::vector<ID3D11Buffer*> &apcActiveConstantBuffers, UINT dwStartSlot, UINT dwNumBuffers, ID3D11Buffer *const*ppcConstantBuffers, Vireio_Supported_Shaders eShaderType);
	void VerifyConstantBuffer(ID3D11Buffer *pcBuffer, UINT dwBufferIndex, Vireio_Supported_Shaders eShaderType);
	void DoBufferModification(INT nRulesIndex, UINT_PTR pdwLeft, UINT_PTR pdwRight, UINT dwBufferSize);
#endif
	void DebugOutput(const void *pvSrcData, UINT dwShaderIndex, UINT dwBufferIndex, UINT dwBufferSize);
	void FillShaderRuleIndices();
	void FillShaderRuleData(UINT dwRuleIndex);
	void FillShaderRuleGeneralIndices();
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	void FillFetchedHashCodeList();
#else
	void FillShaderRuleShaderIndices();
#endif
	bool MatrixModifier::ImportXMLRules(std::string rulesPath);


	/// <summary>
	/// [IN] Input pointers array.
	/// </summary>
	void* m_ppInput[NUMBER_OF_DECOMMANDERS];
	/// <summary>
	/// [OUT] All data this node provides to other nodes.
	/// </summary>
	ModifierData m_sModifierData;

#pragma region => Internal Data

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/// <summary>*
	/// MatrixModifier output pointers
	/// </summary>
	void* m_pvOutput[NUMBER_OF_COMMANDERS];
	/// <summary>
	/// The d3d11 vertex shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D11_Shader> m_asVShaders;
	/// <summary>
	/// The d3d11 pixel shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D11_Shader> m_asPShaders;
	/// <summary>
	/// The d3d11 active Vertex Shader constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcVSActiveConstantBuffers11;
	/// <summary>
	/// The d3d11 active Hull Shader constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcHSActiveConstantBuffers11;
	/// <summary>
	/// The d3d11 active Domain Shader constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcDSActiveConstantBuffers11;
	/// <summary>
	/// The d3d11 active Geometry Shader constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcGSActiveConstantBuffers11;
	/// <summary>
	/// The d3d11 active Pixel Shader constant buffer vector, for left and right side.
	/// 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	/// D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	/// </summary>
	std::vector<ID3D11Buffer*> m_apcPSActiveConstantBuffers11;
	/// <summary>
	/// True if constant buffers are initialized.
	/// </summary>
	bool m_bConstantBuffersInitialized;
	/// <summary>
	/// The active vertex shader.
	/// </summary>
	union
	{
		ID3D10VertexShader* m_pcActiveVertexShader10;
		ID3D11VertexShader* m_pcActiveVertexShader11;
	};
	/// <summary>
	/// Active stored render target views.
	/// The render targets that are currently in use.
	/// DX11 :
	/// 0---------------------------------------------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	/// D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	/// </summary>
	std::vector<ID3D11RenderTargetView *> m_apcActiveRenderTargetViews11;
	/// <summary>
	/// Active stored depth stencil view.
	/// The depth stencil surface that is currently in use.
	/// </summary>
	ID3D11DepthStencilView* m_apcActiveDepthStencilView11[2];
	/// <summary>
	/// The number of actively mapped constant buffers.
	/// </summary>
	UINT m_dwMappedBuffers;
	/// <summary>
	/// The number of frames the constant buffers are to be verified.
	/// Set to zero to optimize StereoSplitter->SetDrawingSide()
	/// </summary>
	UINT m_dwVerifyConstantBuffers;	
	/// <summary>
	/// Constant Buffer private data buffer left eye.
	/// </summary>
	union
	{
		BYTE m_pchBuffer10Left[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
		BYTE m_pchBuffer11Left[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
	};
	/// <summary>
	/// Constant Buffer private data buffer right eye.
	/// </summary>
	union
	{
		BYTE m_pchBuffer10Right[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
		BYTE m_pchBuffer11Right[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
	};
	/// <summary>
	/// Secondary render target.
	/// </summary>
	union
	{
		ID3D10Texture2D* m_pcSecondaryRenderTarget10;
		ID3D11Texture2D* m_pcSecondaryRenderTarget11;
	};
	/// <summary>
	/// Secondary render target shader resource view.
	/// </summary>
	union
	{
		ID3D10ShaderResourceView* m_pcSecondaryRenderTargetSRView10;
		ID3D11ShaderResourceView* m_pcSecondaryRenderTargetSRView11;
	};
	/// <summary>
	/// Secondary render target view.
	/// </summary>
	union
	{
		ID3D10RenderTargetView* m_pcSecondaryRenderTargetView10;
		ID3D11RenderTargetView* m_pcSecondaryRenderTargetView11;
	};

#elif defined(VIREIO_D3D9)
	/// <summary>
	/// The d3d9 vertex shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D9_Shader> m_asVShaders;
	/// <summary>
	/// The d3d9 pixel shader description vector.
	/// Contains all enumerated shader data structures.
	/// </summary>
	std::vector<Vireio_D3D9_Shader> m_asPShaders;
	/// <summary>
	/// The active vertex shader.
	/// </summary>
	IDirect3DManagedStereoShader9<IDirect3DVertexShader9>* m_pcActiveVertexShader;
	/// <summary>
	/// The active pixel shader.
	/// </summary>
	IDirect3DManagedStereoShader9<IDirect3DPixelShader9>* m_pcActivePixelShader;
	/// <summary>
	/// Shader-specific constant rule indices array.
	/// </summary>
	std::vector<Vireio_Hash_Rule_Index> m_asShaderSpecificRuleIndices;
	/// <summary>
	/// Shader register vector. Unmodified vertex shader constant data.
	/// 4 floats == 1 register (defined in VECTOR_LENGTH):
	/// [0][1][2][3] would be the first register.
	/// [4][5][6][7] the second, etc.
	/// use RegisterIndex(x) to access first float in register
	/// </summary>
	std::vector<float> m_afRegistersVertex;
	/// <summary>
	/// Shader register vector. Unmodified pixel shader constant data.
	/// 4 floats == 1 register (defined in VECTOR_LENGTH):
	/// [0][1][2][3] would be the first register.
	/// [4][5][6][7] the second, etc.
	/// use RegisterIndex(x) to access first float in register
	/// </summary>
	std::vector<float> m_afRegistersPixel;
	/// <summary>
	/// True if view transform is set via SetTransform().
	/// @see SetTransform()
	/// </summary>
	bool m_bViewTransformSet;
	/// <summary>
	/// True if projection transform is set via SetTransform().
	/// @see SetTransform()
	/// </summary>
	bool m_bProjectionTransformSet;
	/// <summary>
	/// The stored view transform set via SetTransform() l/r.
	/// </summary>
	D3DXMATRIX m_sMatView[2];
	/// <summary>
	/// The stored projection transform set via SetTransform() l/r.
	/// </summary>
	D3DXMATRIX m_sMatProj[2];
	/// <summary>
	/// Either the left or right view, depending on active render side.
	/// </summary>
	D3DXMATRIX* m_psMatViewCurrent;
	/// <summary>
	/// Either the left or right projection, depending on active render side.
	/// </summary>
	D3DXMATRIX* m_psMatProjCurrent;
#endif

	/// <summary>
	/// All constant rules loaded for that game.
	/// </summary>
	std::vector<Vireio_Constant_Modification_Rule> m_asConstantRules;
	/// <summary>
	/// Global constant rule indices array.
	/// </summary>
	std::vector<UINT> m_aunGlobalConstantRuleIndices;
	/// <summary>
	/// The constant rules update counter.
	/// Starts with "1" and increases for every newly added shader rule.
	/// </summary>
	UINT m_dwConstantRulesUpdateCounter;
	/// <summary>
	/// Indices for constant buffer addressed shader rules.
	/// </summary>
	std::vector<std::vector<Vireio_Constant_Rule_Index>> m_aasConstantBufferRuleIndices;
	/// <summary>
	/// View matrix adjustment class.
	/// @see ViewAdjustment
	/// </summary>
	std::shared_ptr<ModificationCalculation> m_pcShaderModificationCalculation;
	/// <summary>
	/// The game configuration for the current game.
	/// </summary>
	Vireio_GameConfiguration m_sGameConfiguration;
	/// <summary>
	/// All GUI pages IDs.
	/// </summary>
	std::vector<DWORD> m_adwPageIDs;
	/// <summary>
	/// Current shader chosen from list (hash code)
	/// </summary>
	UINT m_dwCurrentChosenShaderHashCode;
	/// <summary>
	/// List of available Debug Options (Entries).
	/// </summary>
	std::vector<std::wstring> m_aszDebugOptions;
#pragma endregion

#pragma region => Node Controls / In-Game Menu

	/// <summary>
	/// Shader page control IDs
	/// Structure contains all control IDs for the vertex shader page.
	/// </summary>
	struct PageShader
	{
		UINT m_dwUpdate;                           /**< [Button] : Activate to update all vertex shader data (ID) ***/
		UINT m_dwSort;                             /**< [Switch] : Sort the shader list ***/
		UINT m_dwHashCodes;                        /**< [List] : Contains ALL vertex shader hash codes (ID) ***/
		UINT m_dwCurrentConstants;                 /**< [List] : Contains all constants for the chosen vertex shader (ID) ***/
		UINT m_dwToName;                           /**< [Button] : Activate to fill the constant name control on the shader rule page (ID) ***/
		UINT m_dwToRegister;                       /**< [Button] : Activate to fill the register control on the shader rule page (ID) ***/
		UINT m_dwShaderType;                       /**< [Spin] Currently chosen shader type (ID) ***/
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		UINT m_dwCurrentBuffersizes;               /**< [List] : Contains all constant buffer sizes for the chosen vertex shader (ID) ***/
		UINT m_dwToBufferSize;                     /**< [Button] : Activate to fill the buffer size control on the shader rule page (ID) ***/
		UINT m_dwToBufferIndex;                    /**< [Button] : Activate to fill the buffer index control on the shader rule page (ID) ***/
		UINT m_dwToFetchedList;                    /**< [Button] : Activate to fill the fetched shader hash code list on the shader rule page (ID) ***/
#endif
	} m_sPageShader;
	/// <summary>
	/// Debug page control IDs
	/// Structure contains all control IDs for the debug page.
	/// </summary>
	struct PageDebug
	{
		UINT m_dwOptions;                          /**< [Spin] The debug options (ID) ***/
		UINT m_dwGrab;                             /**< [Button] Grabs debug data to trace (ID) **/
		UINT m_dwClear;                            /**< [Button] Clears the debug trace (ID) ***/
		UINT m_dwShaderConstants;                  /**< [List] Contains ALL possible shader constants (ID) ***/
		UINT m_dwTrace;                            /**< [List] Debug output trace (ID) ***/
	} m_sPageDebug;
	/// <summary>
	/// Game Settings page control IDs.
	/// Structure contains all control IDs for the game settings page.
	/// </summary>
	struct PageGameSettings
	{
		/*** PageGameSettings game configuration ***/
		UINT m_dwGameSeparation;                   /**< [Float] Game Stereo Separation (ID) ***/
		UINT m_dwConvergence;                      /**< [Float] Convergence or Neutral Point distance, in meters. **/
		UINT m_dwAspectMultiplier;                 /**< [Float] Aspect multiplier allows adjusting virtual screen aspect ratio. */
		UINT m_dwVRboostMinShaderCount;            /**< Minimum Vertex Shader Count to apply VRboost (security) */
		UINT m_dwVRboostMaxShaderCount;            /**< Maximum Vertex Shader Count to apply VRboost (security) */
		UINT m_dwIs64bit;                          /**< [StaticList] The game cpu-architecture, true for 64-bit games */
		UINT m_dwRollImpl;                         /**< [Spin] 0 - NONE, 1 - Matrix Roll, 2 - Pixel Shader Roll */
		UINT m_dwConvergenceEnabled;               /**< [Switch] Whether convergence is enabled. Typically on for 3D monitors, off for head-mounted displays. **/
		UINT m_dwYawMultiplier;                    /**< [Float] Game-specific tracking multiplier (yaw). */
		UINT m_dwPitchMultiplier;                  /**< [Float] Game-specific tracking multiplier (pitch). */
		UINT m_dwRollMultiplier;                   /**< [Float] Game-specific tracking multiplier (roll). */
		UINT m_dwPositionMultiplier;               /**< [Float] Game-specific position overall multiplier (for X, Y and Z). */
		UINT m_dwPositionXMultiplier;              /**< [Float] Game-specific position multiplier for X*/
		UINT m_dwPositionYMultiplier;              /**< [Float] Game-specific position multiplier for Y*/
		UINT m_dwPositionZMultiplier;              /**< [Float] Game-specific position multiplier for Z*/
		UINT m_dwPFOV;                             /**< [Float] Projection FOV, alternative to modifying game's FOV **/
		UINT m_dwPFOVToggle;                       /**< [Switch] Projection FOV, toggle for above **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		/*** PageGameSettings technical options ***/
#endif
	} m_sPageGameSettings;
	/// <summary>
	/// Game Shader Rules page control IDs + text.
	/// Structure contains all control IDs for the game shader rules page.
	/// </summary>
	struct PageGameShaderRules
	{
		UINT m_dwTextlist;                          /**< [StaticList] The text of the controls. **/
		UINT m_dwRuleIndices;                       /**< [List] List of all created shader rules **/
		UINT m_dwRuleData;                          /**< [List] Data output for the chosen rule **/
		UINT m_dwGeneralIndices;                    /**< [List] All general indices **/

		UINT m_dwConstantName;                      /**< [Switch] Shader constant name **/
		UINT m_dwPartialName;                       /**< [Switch] Shader constant partial name **/
		UINT m_dwStartRegIndex;                     /**< [Switch] Start register index **/

		UINT m_dwRegisterCount;                     /**< [Spin] Register count **/
		UINT m_dwOperationToApply;                  /**< [Spin] Operation to apply **/
		UINT m_dwTranspose;                         /**< [Switch] Transpose Yes/No **/

		UINT m_dwCreate;                            /**< [Button] Create a rule **/
		UINT m_dwDeleteLatest;                      /**< [Button] Delete latest rule **/
		UINT m_dwAddGeneral;                        /**< [Button] Add to general indices **/
		UINT m_dwDeleteGeneral;                     /**< [Button] Delete chosen general index **/
		UINT m_dwImportXML;                         /**< [Button] Import v3 XML file **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		UINT m_dwFetchedShaderHashcodes;            /**< [List] All shader hash codes using the temporary render target (HUD/GUI) ***/
		UINT m_dwBufferIndexDebug;                  /**< [Switch] : Activate to output all possible buffer sizes to the debug trace for the index in m_dwBufferIndex.***/

		UINT m_dwBufferIndex;                       /**< [Switch] Buffer index **/
		UINT m_dwBufferSize;                        /**< [Switch] Buffer size **/
#endif
		// string entries for the switches above
		std::wstring m_szConstantName;
		std::wstring m_szPartialName;
		std::wstring m_szBufferIndex;
		std::wstring m_szBufferSize;
		std::wstring m_szStartRegIndex;

		// values of the controls
		bool m_bConstantName;
		bool m_bPartialName;
		bool m_bStartRegIndex;
		bool m_bTranspose;
		UINT m_dwOperationValue;
		UINT m_dwRegCountValue;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		bool m_bBufferIndex;
		bool m_bBufferSize;
#elif defined(VIREIO_D3D9)
		UINT m_dwShaderIndices;                     /**< [List] All shader-specific indices **/
#endif
	} m_sPageGameShaderRules;
	/// <summary>
	/// This structure contains all DX version specific technical options
	/// you have to influence or optimize the way the MatrixModifier works.
	/// </summary>
	struct TechnicalOptions
	{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)

#endif
	} m_sTechnicalOptions;
	/// <summary>
	/// List of all available vertex shader hash codes (std::wstring).
	/// To be used on the shader page, the debug page
	/// and to create shader rules.
	/// Each entry MUST MATCH same entry in m_adwShaderHashCodes.
	/// </summary>
	std::vector<std::wstring> m_aszVShaderHashCodes;
	/// <summary>
	/// List of all available vertex shader hash codes (UINT).
	/// To be used on the shader page, the debug page
	/// and to create shader rules.
	/// Each entry MUST MATCH same entry in m_aszShaderHashCodes.
	/// </summary>
	std::vector<UINT> m_adwVShaderHashCodes;
	/// <summary>
	/// List of all available pixel shader hash codes (std::wstring).
	/// To be used on the shader page, the debug page
	/// and to create shader rules.
	/// Each entry MUST MATCH same entry in m_adwShaderHashCodes.
	/// </summary>
	std::vector<std::wstring> m_aszPShaderHashCodes;
	/// <summary>
	/// List of all available pixel shader hash codes (UINT).
	/// To be used on the shader page, the debug page
	/// and to create shader rules.
	/// Each entry MUST MATCH same entry in m_aszShaderHashCodes.
	/// </summary>
	std::vector<UINT> m_adwPShaderHashCodes;
	/// <summary>
	/// List of all available shader constant names (std::wstring).
	/// To be used on the shader modifaction page, the debug page
	/// and to create shader rules.
	/// </summary>
	std::vector<std::wstring> m_aszShaderConstants;
	/// <summary>
	/// List of all available shader constant names (std::string).
	/// To be used on the shader modifaction page, the debug page
	/// and to create shader rules.
	/// </summary>
	std::vector<std::string> m_aszShaderConstantsA;
	/// <summary>
	/// List of shader constant names for the currently chosen shader. (std::wstring).
	/// To be used on the shader modifaction page
	/// and to create shader rules.
	/// </summary>
	std::vector<std::wstring> m_aszShaderConstantsCurrent;
	/// <summary>
	/// List of shader buffer sizes for the currently chosen shader. (std::wstring).
	/// To be used on the shader modifaction page
	/// and to create shader rules.
	/// </summary>
	std::vector<std::wstring> m_aszShaderBuffersizes;
	/// <summary>
	/// List of all shader rule indices present. (std::wstring).
	/// To be used on the shader rules page.
	/// </summary>
	std::vector<std::wstring> m_aszShaderRuleIndices;
	/// <summary>
	/// List of shader rule data for the chosen shader rule index (std::wstring).
	/// To be used on the shader rules page.
	/// </summary>
	std::vector<std::wstring> m_aszShaderRuleData;
	/// <summary>
	/// List of all shader rule indices generally used. (std::wstring).
	/// To be used on the shader rules page.
	/// </summary>
	std::vector<std::wstring> m_aszShaderRuleGeneralIndices;
	/// <summary>
	/// Debug trace string list.
	/// Contains all strings for the debug trace.
	/// </summary>
	std::vector<std::wstring> m_aszDebugTrace;
	/// <summary>
	/// Currently chosen option to grab debug text.
	/// </summary>
	Debug_Grab_Options m_eDebugOption;
	/// <summary>
	/// If true debug text will be grabbed by the chosen debug option (m_eDebugOption)
	/// </summary>
	bool m_bGrabDebug;
	/// <summary>
	/// True if the shader list is to be sorted.
	/// </summary>
	bool m_bSortShaderList;
	/// <summary>
	/// Data buffer to save this node.
	/// </summary>
	char m_acData[MAX_DATA_SIZE];
	/// <summary>
	/// Vireio menu.
	/// </summary>
	VireioSubMenu m_sMenu;
	/// <summary>
	/// Frames to save the ini file.
	/// </summary>
	INT m_nIniFrameCount;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/// <summary>
	/// True if the buffer index sizes ar provided to the debug trace.
	/// </summary>
	bool m_bBufferIndexDebug;
	/// <summary>
	/// Vector helper for the buffer index debug output.
	/// </summary>
	std::vector<UINT> m_aunBufferIndexSizesDebug;
	/// <summary>
	/// List of all fetched shader hash codes. (std::wstring).
	/// To be used on the shader rules page.
	/// </summary>
	std::vector<std::wstring> m_aszFetchedHashCodes;
	/// <summary>
	/// List of all fetched shader hash codes. (UINT).
	/// To be used on the shader rules page.
	/// </summary>
	std::vector<UINT> m_aunFetchedHashCodes;
#elif defined(VIREIO_D3D9)
	/// <summary>
	/// List of all shaderrule indices for the currently chosen shader on the shaders page. (std::wstring).
	/// (DX9 only)
	/// </summary>
	std::vector<std::wstring> m_aszShaderRuleShaderIndices;
#endif

#pragma endregion

};

/// <summary>
/// Exported Constructor Method.
/// </summary>
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	MatrixModifier* pMatrixModifier = new MatrixModifier();
	return static_cast<AQU_Nodus*>(pMatrixModifier);
}

