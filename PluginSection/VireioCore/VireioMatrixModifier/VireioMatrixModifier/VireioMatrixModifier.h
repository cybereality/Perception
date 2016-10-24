/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.h> and
Class <VireioMatrixModifier> :
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
#include<map>
#include<vector>
#include<sstream>
#include<ctime>

#include"AQU_Nodus.h"
#include"Resources.h"
#include"VireioMatrixModifierDataStructures.h"
#include"..\..\..\..\DxProxy\DxProxy\ViewAdjustment.h"
#include"..\..\..\..\DxProxy\DxProxy\HMDisplayInfo.h"
#include"..\..\..\..\DxProxy\DxProxy\HMDisplayInfo_Default.h"
#include"..\..\..\..\DxProxy\DxProxy\HMDisplayInfo_OculusRift.h"
#include"..\..\..\..\Shared\ConfigDefaults.h"

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

#include"..\..\..\..\DxProxy\DxProxy\MurmurHash3.h"
#include"..\..\..\..\DxProxy\DxProxy\StereoShaderConstant.h"
#endif

#include"..\..\..\Include\Vireio_GUI.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define	PROVOKING_TYPE                                 2                     /**< Provoking type is 2 - just invoker, no provoker **/
#define METHOD_REPLACEMENT                         false                     /**< This node does NOT replace the D3D call (default) **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#define NUMBER_OF_COMMANDERS                          21
#define NUMBER_OF_DECOMMANDERS                        53
#define GUI_WIDTH                                   1024                      
#define GUI_HEIGHT                                  5000               
#define CONSTANT_BUFFER_VERIFICATION_FRAME_NUMBER    100                     /**< If no shader data is present, the constant buffers are verified for 100 frames. ***/
#elif defined(VIREIO_D3D9)
#define NUMBER_OF_COMMANDERS                           3
#define NUMBER_OF_DECOMMANDERS                        20
#define GUI_WIDTH                                   1024                      
#define GUI_HEIGHT                                  2000     
#define VECTOR_LENGTH 4                                                      /**< One shader register has 4 float values. ***/
#define MAX_DX9_CONSTANT_REGISTERS                   224                     /**< Maximum shader registers for DX9 ***/
#define RegisterIndex(x) (x * VECTOR_LENGTH)                                 /**< Simple helper to access shader register. ***/
#endif
#define GUI_CONTROL_BORDER                            64
#define GUI_CONTROL_FONTSIZE                          64
#define GUI_CONTROL_FONTBORDER                        16
#define GUI_CONTROL_LINE                              92
#define GUI_CONTROL_BUTTONSIZE                       488

#define MAX_DATA_SIZE                              65535                     /**< Arbitrary... TODO !! set a maximum node data size **/

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }
void debugf(const char *fmt, ...) { va_list args; va_start(args, fmt); char buf[8192]; vsnprintf_s(buf, 8192, fmt, args); va_end(args); OutputDebugStringA(buf); }

/**
* Node Commander Enumeration.
***/
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
	SwitchRenderTarget,                                                     /**< Option to switch the render target for the game HUD and GUI. ***/
	HudOperation,                                                           /**< True if any HUD operation is executed. ***/
	SecondaryRenderTarget_DX10,                                             /**< Secondary (HUD) render target mono DX10. ***/
	SecondaryRenderTarget_DX11,                                             /**< Secondary (HUD) render target mono DX11. ***/
	ppActiveRenderTargets_DX10,                                             /**< Active render targets DX10. Backup for render target operations. ***/
	ppActiveRenderTargets_DX11,                                             /**< Active render targets DX11. Backup for render target operations. ***/
	ppActiveDepthStencil_DX10,                                              /**< Active depth stencil DX10. Backup for render target operations. ***/
	ppActiveDepthStencil_DX11,                                              /**< Active depth stencil DX11. Backup for render target operations. ***/
#elif defined(VIREIO_D3D9)
	eDrawingSide,                                                           /**< Left/Right drawing side enumeration. Switches once per draw call ***/
	pasVShaderConstantIndices,                                              /**< The constant rule indices for the actual vertex shader. ***/
	pasPShaderConstantIndices,                                              /**< The constant rule indices for the actual pixel shader. ***/
#endif
};

/**
* Node Commander Enumeration.
***/
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
	pShader_Vertex,              // SetVertexShader(IDirect3DVertexShader9 *pShader);
	pShader_Pixel,               // SetPixelShader(IDirect3DPixelShader9 *pShader);
	State,                       // SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	pMatrix,
	State_Multiply,              // MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	pMatrix_Multiply,
	StartRegister,               // Set/GetVertex/PixelShaderConstant(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	pConstantData,
	Vector4fCount,
	StreamNumber,                // SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
	pStreamData,
	OffsetInBytes,
	Stride,
	StreamNumber_Get,            // GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
	ppStreamData_Get,
	pOffsetInBytes_Get,
	pStride_Get,
	pFunction,                   // CreateVertex/PixelShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
	ppShader_Vertex,
	ppShader_Pixel,
	// IDirect3DStateBlock::Apply();
#endif
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
* All GUI pages for the Matrix Modifier.
* Must be added in following order.
***/
enum GUI_Pages
{
	MainPage = 0,
	DescriptionPage = 1,
	GameSettingsPage = 2,
	ShadersPage = 3,
	ShaderRulesPage = 4,
	DebugPage = 5,
	NumberOfPages = 6,
};

/**
* Debug grab options.
* Enumeration must match the strings added to spin control : m_dwDebugSpin (ID).
**/
enum Debug_Grab_Options
{
	Debug_ConstantFloat4,
	Debug_ConstantFloat8,
	Debug_ConstantFloat16,
	Debug_ConstantFloat32,
	Debug_ConstantFloat64,
};

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
/**
* Vireio Map DX10/11 data structure.
* Contains all data for a mapped constant buffer.
***/
struct Vireio_Map_Data
{
	/**
	* Stored mapped resource pointer. (DX11 only)
	***/
	ID3D11Resource* m_pcMappedResource;
	/**
	* Stored mapped resource description.
	***/
	D3D11_MAPPED_SUBRESOURCE* m_psMappedResource;
	/**
	* Stored mapped resource data pointer.
	***/
	void* m_pMappedResourceData;
	/**
	* Stored mapped resource data size (in bytes).
	***/
	UINT m_dwMappedResourceDataSize;
	/**
	* Stored map type.
	***/
	D3D11_MAP m_eMapType;
	/**
	* Stored map flags.
	***/
	UINT m_dwMapFlags;
	/**
	* The private data shader rules index for a mapped constant buffer.
	***/
	INT m_nMapRulesIndex;
	/**
	* Constant Buffer private data buffer.
	* Buffer data needed for Map(). (+0xff to provide homogenous address)
	***/
	union
	{
		BYTE m_pchBuffer10[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3) + 0xff];
		BYTE m_pchBuffer11[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3) + 0xff];
	};
};

/**
* Simple enumeration of supported Shaders.
***/
enum Vireio_Supported_Shaders
{
	VertexShader,
	PixelShader,
	GeometryShader,
	HullShader,
	DomainShader
};
#elif defined(VIREIO_D3D9)
/**
* Simple helper to get the hash of a shader.
* @param pShader The input vertex shader.
* @return The hash code of the shader.
***/
uint32_t ShaderHash(LPDIRECT3DVERTEXSHADER9 pcShader)
{
	if (!pcShader) return 0;

	BYTE* pnData = NULL;
	UINT punSizeOfData;
	pcShader->GetFunction(NULL, &punSizeOfData);

	pnData = new BYTE[punSizeOfData];
	pcShader->GetFunction(pnData, &punSizeOfData);

	uint32_t unHash = 0;
	MurmurHash3_x86_32(pnData, punSizeOfData, VIREIO_SEED, &unHash);
	delete[] pnData;
	return unHash;
}
/**
* Managed proxy shader class.
* Contains left and right shader constants.
*/
template <class T = float>
class IDirect3DManagedStereoShader9 : public T //T
{
public:
	IDirect3DManagedStereoShader9(T* pcActualVertexShader, IDirect3DDevice9* pcOwningDevice, std::vector<Vireio_Constant_Modification_Rule>* pasConstantRules) :
		m_pcVertexShader(pcActualVertexShader),
		m_pcOwningDevice(pcOwningDevice),
		m_pasConstantRules(pasConstantRules),
		m_unRefCount(1)
	{
		assert(pcActualVertexShader != NULL);
		assert(pcOwningDevice != NULL);
		assert(pasConstantRules != NULL);

		pcOwningDevice->AddRef();

		// get shader hash
		m_unShaderHash = ShaderHash(pcActualVertexShader);

		// create managed registers
		m_unMaxShaderConstantRegs = MAX_DX9_CONSTANT_REGISTERS; /** TODO !! COUNT MAX CONSTANT REG NUMBER FOR THIS SHADER ***/
		m_afRegisters = std::vector<float>(m_unMaxShaderConstantRegs * VECTOR_LENGTH);

		// init the shader rules
		InitShaderRules();
	}
	virtual ~IDirect3DManagedStereoShader9() {}

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		return m_pcVertexShader->QueryInterface(riid, ppv);
	}
	virtual ULONG   WINAPI AddRef()
	{
		return ++m_unRefCount;
	}
	virtual ULONG   WINAPI Release()
	{
		if (--m_unRefCount == 0)
		{
			delete this;
			return 0;
		}

		return m_unRefCount;
	}

	/*** IDirect3DXShader9 methods ***/

	/**
	* IDirect3DXShader9->GetDevice() call.
	***/
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9 **ppcDevice)
	{
		if (!m_pcOwningDevice)
			return D3DERR_INVALIDCALL;
		else
		{
			*ppcDevice = m_pcOwningDevice;
			//m_pOwningDevice->AddRef(); //TODO Test this. Docs don't have the notice that is usually there about a refcount increase
			return D3D_OK;
		}
	}
	/**
	* IDirect3DXShader9->GetFunction() call.
	***/
	virtual HRESULT WINAPI GetFunction(void *pDate, UINT *punSizeOfData)
	{
		return m_pcVertexShader->GetFunction(pDate, punSizeOfData);
	}

	/*** IDirect3DManagedStereoShader9 methods ***/

	/**
	* Inits the shader rules based on the constant table of the shader.
	***/
	void InitShaderRules()
	{
		// @see ShaderModificationRepository::GetModifiedConstantsF from Vireio < v3

		// clear constant rule vector
		m_asConstantRuleIndices = std::vector<Vireio_Constant_Rule_Index_DX9>();

		// clear register indices to max int
		FillMemory(m_aunRegisterModificationIndex, MAX_DX9_CONSTANT_REGISTERS * sizeof(UINT), 0xFF);

		/*if (m_shaderSpecificModificationRuleIDs.count(hash) == 1)
		{

		// There are specific modification rules to use with this shader
		auto itRules = m_shaderSpecificModificationRuleIDs[hash].begin();
		while (itRules != m_shaderSpecificModificationRuleIDs[hash].end())
		{
		rulesToApply.push_back(&(m_AllModificationRules[*itRules]));
		++itRules;
		}
		}*/

		// get shader function
		BYTE *pData = NULL;
		UINT pSizeOfData;

		m_pcVertexShader->GetFunction(NULL, &pSizeOfData);
		pData = new BYTE[pSizeOfData];
		m_pcVertexShader->GetFunction(pData, &pSizeOfData);

		// Load the constant descriptions for this shader and create StereoShaderConstants as the applicable rules require them.
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);

		if (pConstantTable)
		{
			// get constant table description
			D3DXCONSTANTTABLE_DESC pDesc;
			pConstantTable->GetDesc(&pDesc);

			D3DXCONSTANT_DESC pConstantDesc[64];

			// loop throught constants
			for (UINT i = 0; i < pDesc.Constants; i++)
			{
				D3DXHANDLE handle = pConstantTable->GetConstant(NULL, i);
				if (handle == NULL) continue;

				UINT unConstantNum = 64;
				pConstantTable->GetConstantDesc(handle, pConstantDesc, &unConstantNum);
				if (unConstantNum >= 64)
				{
					OutputDebugString(L"[MAM] Need larger constant description buffer");
					unConstantNum = 63;
				}

				for (UINT j = 0; j < unConstantNum; j++)
				{
					// register count 1 (= vector) and 4 (= matrix) supported
					if (((pConstantDesc[j].Class == D3DXPC_VECTOR) && (pConstantDesc[j].RegisterCount == 1))
						|| (((pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS)) && (pConstantDesc[j].RegisterCount == 4)))
					{
						// Check if any rules match this constant
						UINT unIndex = 0;
						auto itRules = (*m_pasConstantRules).begin();
						while (itRules != (*m_pasConstantRules).end())
						{
							// Type match
							if ((*itRules).m_dwRegisterCount == pConstantDesc[j].RegisterCount)
							{
								// name match required
								if ((*itRules).m_szConstantName.size() > 0)
								{
									bool nameMatch = false;
									if ((*itRules).m_bUsePartialNameMatch)
									{
										nameMatch = std::strstr(pConstantDesc[j].Name, (*itRules).m_szConstantName.c_str()) != NULL;
									}
									else
									{
										nameMatch = (*itRules).m_szConstantName.compare(pConstantDesc[j].Name) == 0;
									}

									if (!nameMatch)
									{
										// no match
										++itRules; ++unIndex;
										continue;
									}
								}

								// register match required
								if ((*itRules).m_dwStartRegIndex != UINT_MAX)
								{
									if ((*itRules).m_dwStartRegIndex != pConstantDesc[j].RegisterIndex)
									{
										// no match
										++itRules; ++unIndex;
										continue;
									}
								}

#ifdef _DEBUG
								// output shader constant + index 
								switch (pConstantDesc[j].Class)
								{
									case D3DXPC_VECTOR:
										OutputDebugString(L"VS: D3DXPC_VECTOR");
										break;
									case D3DXPC_MATRIX_ROWS:
										OutputDebugString(L"VS: D3DXPC_MATRIX_ROWS");
										break;
									case D3DXPC_MATRIX_COLUMNS:
										OutputDebugString(L"VS: D3DXPC_MATRIX_COLUMNS");
										break;
									default:
										OutputDebugString(L"VS: UNKNOWN_CONSTANT");
										break;
								}
								debugf("Register Index: %d", pConstantDesc[j].RegisterIndex);
#endif 
								// set register index
								m_aunRegisterModificationIndex[pConstantDesc[j].RegisterIndex] = (UINT)m_asConstantRuleIndices.size();

								// set constant rule index
								Vireio_Constant_Rule_Index_DX9 sConstantRuleIndex;
								sConstantRuleIndex.m_dwIndex = unIndex;
								sConstantRuleIndex.m_dwConstantRuleRegister = pConstantDesc[j].RegisterIndex;
								sConstantRuleIndex.m_dwConstantRuleRegisterCount = pConstantDesc[j].RegisterCount;

								// init data.. TODO !! INIT DATA MODIFIED
								IDirect3DDevice9* pcDevice = nullptr;
								m_pcVertexShader->GetDevice(&pcDevice);
								if (pcDevice)
								{
									// get current shader
									T* pcShaderOld = nullptr;
									pcDevice->GetVertexShader(&pcShaderOld);

									// set new shader and get data
									pcDevice->SetVertexShader(pcShaderOld);
									pcDevice->GetVertexShaderConstantF(pConstantDesc[j].RegisterIndex, sConstantRuleIndex.m_afConstantDataLeft, pConstantDesc[j].RegisterCount);
									pcDevice->GetVertexShaderConstantF(pConstantDesc[j].RegisterIndex, sConstantRuleIndex.m_afConstantDataRight, pConstantDesc[j].RegisterCount);
									pcDevice->GetVertexShaderConstantF(0, &m_afRegisters[0], m_unMaxShaderConstantRegs);
									pcDevice->Release();

									// set back vertex shader
									if (pcShaderOld) { pcDevice->SetVertexShader(pcShaderOld); pcShaderOld->Release(); }
									else pcDevice->SetVertexShader(nullptr);
								}

								m_asConstantRuleIndices.push_back(sConstantRuleIndex);

								// only the first matching rule is applied to a constant
								break;
							}

							++itRules; ++unIndex;
						}
					}
				}
			}
		}

		if (pConstantTable) pConstantTable->Release();
		if (pData) delete[] pData;
	}
	/**
	* Override IDirect3DDevice9->SetShaderConstantF() here.
	***/
	HRESULT SetShaderConstantF(UINT unStartRegister, const float* pfConstantData, UINT unVector4fCount, bool& bModified, UINT& unIndex)
	{
		if ((unStartRegister >= m_unMaxShaderConstantRegs) || ((unStartRegister + unVector4fCount) >= m_unMaxShaderConstantRegs))
			return D3DERR_INVALIDCALL;

		// Set proxy registers
		memcpy(&m_afRegisters[unStartRegister], pfConstantData, unVector4fCount * 4 * sizeof(float));

		// modification present for this index ?
		if ((m_aunRegisterModificationIndex[unStartRegister] < (UINT)m_asConstantRuleIndices.size()) && (unVector4fCount == 4))
		{
			// apply to left and right data
			unIndex = m_aunRegisterModificationIndex[unStartRegister];
			bModified = true;

			// get the matrix
			D3DXMATRIX sMatrix(&m_afRegisters[unStartRegister]);
			{
				// matrix to be transposed ?
				if (true)
				{
					D3DXMatrixTranspose(&sMatrix, &sMatrix);
				}

				// do modification
				D3DXMATRIX sMatrixLeft, sMatrixRight;
				((ShaderMatrixModification*)(*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_pcModification.get())->DoMatrixModification(sMatrix, sMatrixLeft, sMatrixRight);

				// transpose back
				if (true)
				{
					D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
					D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
				}

				m_asConstantRuleIndices[unIndex].m_asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
				m_asConstantRuleIndices[unIndex].m_asConstantDataRight = (D3DMATRIX)sMatrixRight;
			}
		}
		else
		{
			UINT unInd = 0;

			// more data, loop through modified constants for this shader
			auto it = m_asConstantRuleIndices.begin();
			while (it != m_asConstantRuleIndices.end())
			{
				// register in range ?
				if ((unStartRegister < ((*it).m_dwConstantRuleRegister + (*it).m_dwConstantRuleRegisterCount)) && ((unStartRegister + unVector4fCount) >(*it).m_dwConstantRuleRegister))
				{
					// apply to left and right data
					bModified = true;
					unIndex = unInd;

					// get the matrix
					D3DXMATRIX sMatrix(&m_afRegisters[unStartRegister]);
					{
						// matrix to be transposed ?
						if (true)
						{
							D3DXMatrixTranspose(&sMatrix, &sMatrix);
						}

						// do modification
						D3DXMATRIX sMatrixLeft, sMatrixRight;
						((ShaderMatrixModification*)(*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_pcModification.get())->DoMatrixModification(sMatrix, sMatrixLeft, sMatrixRight);

						// transpose back
						if (true)
						{
							D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
							D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
						}

						m_asConstantRuleIndices[unIndex].m_asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
						m_asConstantRuleIndices[unIndex].m_asConstantDataRight = (D3DMATRIX)sMatrixRight;
					}
				}
				it++; unInd++;
			}
		}

		return D3D_OK;
	}
	/**
	* Override IDirect3DDevice9->GetShaderConstantF() here.
	***/
	HRESULT GetShaderConstantF(UINT unStartRegister, float* pfConstantData, UINT unVector4fCount)
	{
		if ((StartRegister >= m_unMaxShaderConstantRegs) || ((StartRegister + Vector4fCount) >= m_unMaxShaderConstantRegs))
			return D3DERR_INVALIDCALL;

		pfConstantData = &m_afRegisters[RegisterIndex(StartRegister)];

		return D3D_OK;
	}
	/**
	* @returns: The hash code of the shader.
	***/
	uint32_t GetShaderHash() { return m_unShaderHash; }
	/**
	* @returns: The actual shader.
	***/
	T* GetActualShader() { return m_pcVertexShader; }

	/**
	* The indices of the shader rules assigned to that shader.
	* For DX9 these indices also contain the left/right modified constant data.
	***/
	std::vector<Vireio_Constant_Rule_Index_DX9> m_asConstantRuleIndices;

protected:
	/**
	* The actual vertex shader embedded.
	***/
	T* const m_pcVertexShader;
	/**
	* Pointer to the D3D device that owns the shader.
	***/
	IDirect3DDevice9* m_pcOwningDevice;
	/**
	* Internal reference counter.
	***/
	ULONG m_unRefCount;
	/**
	* Shader register vector. Unmodified shader constant data.
	* 4 floats == 1 register (defined in VECTOR_LENGTH):
	* [0][1][2][3] would be the first register.
	* [4][5][6][7] the second, etc.
	* use RegisterIndex(x) to access first float in register
	***/
	std::vector<float> m_afRegisters;
	/**
	* Maximum shader constant registers.
	***/
	UINT m_unMaxShaderConstantRegs;
	/**
	* The shader hash code.
	***/
	uint32_t m_unShaderHash;
	/**
	* Pointer to all available constant rules.
	***/
	std::vector<Vireio_Constant_Modification_Rule>* m_pasConstantRules;
	/**
	* Index of modification for the specified register.
	***/
	UINT m_aunRegisterModificationIndex[MAX_DX9_CONSTANT_REGISTERS];
};
#endif

/**
* Vireio Matrix Modifier (DX9/10/11).
* Vireio Perception Stereo Matrix Modification Handler.
***/
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
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
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

	/**
	* Return value pointer (HRESULT).
	***/
	void* m_pvReturn;
	/**
	* Current drawing side, only changed in VireioStereoSplitter::SetDrawingSide().
	**/
	RenderPosition m_eCurrentRenderingSide;
	/**
	* Vireio Graphical User Interface class.
	***/
	Vireio_GUI* m_pcVireioGUI;
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/**
	* Constant Buffer Map data vector
	* Contains all data for all currently mapped constant buffers.
	* The size of this vector can be higher than m_dwMappedBuffers;
	***/
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
	void CreateGUI();
	void FillShaderRuleIndices();
	void FillShaderRuleData(UINT dwRuleIndex);
	void FillShaderRuleGeneralIndices();
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	void FillFetchedHashCodeList();
#else
	void FillShaderRuleShaderIndices();
#endif


#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/*** MatrixModifier input pointers ***/
	void** m_ppvShaderBytecode_VertexShader;
	SIZE_T* m_pnBytecodeLength_VertexShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_VertexShader;
	ID3D10VertexShader*** m_pppcVertexShader_DX10;
	void** m_ppvShaderBytecode_PixelShader;
	SIZE_T* m_pnBytecodeLength_PixelShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_PixelShader;
	ID3D10PixelShader*** m_pppcPixelShader_DX10;
	ID3D10VertexShader** m_ppcVertexShader_10;
	ID3D11VertexShader** m_ppcVertexShader_11;
	ID3D10PixelShader** m_ppcPixelShader_10;
	ID3D11VertexShader** m_ppcPixelShader_11;
	D3D10_BUFFER_DESC** m_ppsDesc_DX10;
	D3D10_SUBRESOURCE_DATA** m_ppsInitialData_DX10;
	ID3D10Buffer*** m_pppcBuffer_DX10;
	UINT* m_pdwStartSlot;
	UINT* m_pdwNumBuffers;
	ID3D10Buffer*** m_pppcConstantBuffers_DX10;
	ID3D11Buffer*** m_pppcConstantBuffers_DX11;
	UINT* m_pdwStartSlot_PixelShader;
	UINT* m_pdwNumBuffers_PixelShader;
	ID3D10Buffer*** m_pppcConstantBuffers_DX10_PixelShader;
	ID3D11Buffer*** m_pppcConstantBuffers_DX11_PixelShader;
	ID3D10Resource** m_ppcDstResource_DX10;
	ID3D11Resource** m_ppcDstResource_DX11;
	UINT* m_pdwDstSubresource;
	D3D10_BOX** m_ppsDstBox_DX10;
	D3D11_BOX** m_ppsDstBox_DX11;
	void** m_ppvSrcData;
	UINT* m_pdwSrcRowPitch;
	UINT* m_pdwSrcDepthPitch;
	ID3D10Resource** m_ppcDstResource_DX10_Copy;
	ID3D10Resource** m_ppcSrcResource_DX10_Copy;
	ID3D11Resource** m_ppcDstResource_DX11_Copy;
	ID3D11Resource** m_ppcSrcResource_DX11_Copy;
	ID3D10Resource** m_ppcDstResource_DX10_CopySub;
	ID3D11Resource** m_ppcDstResource_DX11_CopySub;
	UINT* m_pdwDstSubresource_CopySub;
	UINT* m_pdwDstX;
	UINT* m_pdwDstY;
	UINT* m_pdwDstZ;
	ID3D10Resource** m_ppcSrcResource_DX10_CopySub;
	ID3D11Resource** m_ppcSrcResource_DX11_CopySub;
	UINT* m_pdwSrcSubresource;
	D3D10_BOX** m_ppsSrcBox_DX10;
	D3D11_BOX** m_ppsSrcBox_DX11;
	UINT* m_pdwStartSlot_VertexShader_Get;
	UINT* m_pdwNumBuffers_VertexShader_Get;
	ID3D11Buffer*** m_pppcConstantBuffers_VertexShader;
	UINT* m_pdwStartSlot_PixelShader_Get;
	UINT* m_pdwNumBuffers_PixelShader_Get;
	ID3D11Buffer*** m_pppcConstantBuffers_PixelShader;
	ID3D11Resource** m_ppcResource_Map;
	UINT* m_pdwSubresource_Map;
	D3D11_MAP* m_psMapType;
	UINT* m_pdwMapFlags;
	D3D11_MAPPED_SUBRESOURCE** m_ppsMappedResource;
	ID3D11Resource** m_ppcResource_Unmap;
	UINT* m_pdwSubresource_Unmap;

	/***
	* MatrixModifier output pointers
	***/
	void* m_pvOutput[NUMBER_OF_COMMANDERS];
	/**
	* The d3d11 vertex shader description vector.
	* Contains all enumerated shader data structures.
	***/
	std::vector<Vireio_D3D11_Shader> m_asVShaders;
	/**
	* The d3d11 pixel shader description vector.
	* Contains all enumerated shader data structures.
	***/
	std::vector<Vireio_D3D11_Shader> m_asPShaders;
	/**
	* The d3d11 active Vertex Shader constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcVSActiveConstantBuffers11;
	/**
	* The d3d11 active Hull Shader constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcHSActiveConstantBuffers11;
	/**
	* The d3d11 active Domain Shader constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcDSActiveConstantBuffers11;
	/**
	* The d3d11 active Geometry Shader constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcGSActiveConstantBuffers11;
	/**
	* The d3d11 active Pixel Shader constant buffer vector, for left and right side.
	* 0 -------------------------------------------------> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ----- Left buffers
	* D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT--> D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT * 2 - Right buffers.
	***/
	std::vector<ID3D11Buffer*> m_apcPSActiveConstantBuffers11;
	/**
	* True if constant buffers are initialized.
	***/
	bool m_bConstantBuffersInitialized;
	/**
	* The active vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcActiveVertexShader10;
		ID3D11VertexShader* m_pcActiveVertexShader11;
	};
	/**
	* Active stored render target views.
	* The render targets that are currently in use.
	* DX11 :
	* 0---------------------------------------------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ----- Left render target views
	* D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT -------> D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT * 2 - Right render target views
	***/
	std::vector<ID3D11RenderTargetView *> m_apcActiveRenderTargetViews11;
	/**
	* Active stored depth stencil view.
	* The depth stencil surface that is currently in use.
	***/
	ID3D11DepthStencilView* m_apcActiveDepthStencilView11[2];
	/**
	* The number of actively mapped constant buffers.
	***/
	UINT m_dwMappedBuffers;
	/**
	* The number of frames the constant buffers are to be verified.
	* Set to zero to optimize StereoSplitter->SetDrawingSide()
	***/
	UINT m_dwVerifyConstantBuffers;
	/**
	* The constant rules update counter.
	* Starts with "1" and increases for every newly added shader rule.
	***/
	UINT m_dwConstantRulesUpdateCounter;
	/**
	* Current chosen shader type.
	***/
	Vireio_Supported_Shaders m_eChosenShaderType;
	/**
	* Constant Buffer private data buffer left eye.
	***/
	union
	{
		BYTE m_pchBuffer10Left[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
		BYTE m_pchBuffer11Left[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
	};
	/**
	* Constant Buffer private data buffer right eye.
	***/
	union
	{
		BYTE m_pchBuffer10Right[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D10_VS_INPUT_REGISTER_COMPONENTS * (D3D10_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
		BYTE m_pchBuffer11Right[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * D3D11_VS_INPUT_REGISTER_COMPONENTS * (D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT >> 3)];
	};
	/**
	* Secondary render target.
	***/
	union
	{
		ID3D10Texture2D* m_pcSecondaryRenderTarget10;
		ID3D11Texture2D* m_pcSecondaryRenderTarget11;
	};
	/**
	* Secondary render target shader resource view.
	***/
	union
	{
		ID3D10ShaderResourceView* m_pcSecondaryRenderTargetSRView10;
		ID3D11ShaderResourceView* m_pcSecondaryRenderTargetSRView11;
	};
	/**
	* Secondary render target view.
	***/
	union
	{
		ID3D10RenderTargetView* m_pcSecondaryRenderTargetView10;
		ID3D11RenderTargetView* m_pcSecondaryRenderTargetView11;
	};

#elif defined(VIREIO_D3D9)
	/*** MatrixModifier input pointers ***/
	IDirect3DVertexShader9** m_ppcShader_Vertex;
	IDirect3DPixelShader9** m_ppcShader_Pixel;
	D3DTRANSFORMSTATETYPE* m_psState;
	D3DMATRIX** m_ppsMatrix;
	D3DTRANSFORMSTATETYPE* m_psState_Multiply;
	D3DMATRIX** m_ppsMatrix_Multiply;
	UINT* m_pdwStartRegister;
	float** m_ppfConstantData;
	UINT* m_pdwVector4fCount;
	UINT* m_punStreamNumber;
	IDirect3DVertexBuffer9** m_ppcStreamData;
	UINT* m_punOffsetInBytes;
	UINT* m_punStride;
	UINT* m_punStreamNumber_Get;
	IDirect3DVertexBuffer9*** m_pppcStreamData_Get;
	UINT** m_ppunOffsetInBytes_Get;
	UINT** m_ppunStride_Get;
	DWORD** m_ppunFunction;
	IDirect3DVertexShader9*** m_pppcShader_Vertex;
	IDirect3DPixelShader9*** m_pppcShader_Pixel;

	/**
	* The active vertex shader.
	***/
	IDirect3DManagedStereoShader9<IDirect3DVertexShader9>* m_pcActiveVertexShader;
	/**
	* The indices of the shader rules assigned to the active vertex shader.
	***/
	std::vector<Vireio_Constant_Rule_Index_DX9>* m_pasVSConstantRuleIndices;
	/**
	* The indices of the shader rules assigned to the active pixel shader.
	***/
	std::vector<Vireio_Constant_Rule_Index_DX9>* m_pasPSConstantRuleIndices;
	/**
	* True if view transform is set via SetTransform().
	* @see SetTransform()
	**/
	bool m_bViewTransformSet;
	/**
	* True if projection transform is set via SetTransform().
	* @see SetTransform()
	**/
	bool m_bProjectionTransformSet;
	/**
	* The stored left view transform set via SetTransform().
	**/
	D3DXMATRIX m_sMatViewLeft;
	/**
	* The stored right view transform set via SetTransform().
	**/
	D3DXMATRIX m_sMatViewRight;
	/**
	* The stored left projection transform set via SetTransform().
	**/
	D3DXMATRIX m_sMatProjLeft;
	/**
	* The stored right projection transform set via SetTransform().
	**/
	D3DXMATRIX m_sMatProjRight;
	/**
	* Either the left or right view, depending on active render side.
	**/
	D3DXMATRIX* m_psMatViewCurrent;
	/**
	* Either the left or right projection, depending on active render side.
	**/
	D3DXMATRIX* m_psMatProjCurrent;
#endif

	/**
	* All constant rules loaded for that game.
	***/
	std::vector<Vireio_Constant_Modification_Rule> m_asConstantRules;
	/**
	* Global constant rule indices array.
	***/
	std::vector<UINT> m_adwGlobalConstantRuleIndices;
	/**
	* Shader-specific constant rule indices array.
	***/
	std::vector<Vireio_Shader_Constant_Rule_Index> m_asShaderSpecificRuleIndices;
	/**
	* Indices for constant buffer addressed shader rules.
	***/
	std::vector<std::vector<Vireio_Constant_Rule_Index>> m_aasConstantBufferRuleIndices;
	/**
	* View matrix adjustment class.
	* @see ViewAdjustment
	**/
	std::shared_ptr<ViewAdjustment> m_pcShaderViewAdjustment;
	/**
	* The game configuration for the current game.
	***/
	Vireio_GameConfiguration m_sGameConfiguration;
	/**
	* Pointer to the hmd info
	*/
	HMDisplayInfo* m_psHmdInfo;
	/**
	* All GUI pages IDs.
	***/
	std::vector<DWORD> m_adwPageIDs;
	/**
	* Current shader chosen from list (hash code)
	***/
	UINT m_dwCurrentChosenShaderHashCode;
	/**
	* List of available Debug Options (Entries).
	***/
	std::vector<std::wstring> m_aszDebugOptions;
	/**
	* Shader page control IDs
	* Structure contains all control IDs for the vertex shader page.
	***/
	struct PageShader
	{
		UINT m_dwUpdate;                           /**< [Button] : Activate to update all vertex shader data (ID) ***/
		UINT m_dwSort;                             /**< [Switch] : Sort the shader list ***/
		UINT m_dwHashCodes;                        /**< [List] : Contains ALL vertex shader hash codes (ID) ***/
		UINT m_dwCurrentConstants;                 /**< [List] : Contains all constants for the chosen vertex shader (ID) ***/
		UINT m_dwToName;                           /**< [Button] : Activate to fill the constant name control on the shader rule page (ID) ***/
		UINT m_dwToRegister;                       /**< [Button] : Activate to fill the register control on the shader rule page (ID) ***/
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		UINT m_dwCurrentBuffersizes;               /**< [List] : Contains all constant buffer sizes for the chosen vertex shader (ID) ***/
		UINT m_dwToBufferSize;                     /**< [Button] : Activate to fill the buffer size control on the shader rule page (ID) ***/
		UINT m_dwToBufferIndex;                    /**< [Button] : Activate to fill the buffer index control on the shader rule page (ID) ***/
		UINT m_dwToFetchedList;                    /**< [Button] : Activate to fill the fetched shader hash code list on the shader rule page (ID) ***/
		UINT m_dwShaderType;                       /**< [Spin] Currently chosen shader type (ID) ***/
#endif
	} m_sPageShader;
	/**
	* Debug page control IDs
	* Structure contains all control IDs for the debug page.
	***/
	struct PageDebug
	{
		UINT m_dwOptions;                          /**< [Spin] The debug options (ID) ***/
		UINT m_dwGrab;                             /**< [Button] Grabs debug data to trace (ID) **/
		UINT m_dwClear;                            /**< [Button] Clears the debug trace (ID) ***/
		UINT m_dwShaderConstants;                  /**< [List] Contains ALL possible shader constants (ID) ***/
		UINT m_dwTrace;                            /**< [List] Debug output trace (ID) ***/
	} m_sPageDebug;
	/**
	* Game Settings page control IDs.
	* Structure contains all control IDs for the game settings page.
	***/
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
	/**
	* Game Shader Rules page control IDs + text.
	* Structure contains all control IDs for the game shader rules page.
	***/
	struct PageGameShaderRules
	{
		UINT m_dwTextlist;                          /**< [StaticList] The text of the controls. **/
		UINT m_dwRuleIndices;                       /**< [List] List of all created shader rules **/
		UINT m_dwRuleData;                          /**< [List] Data output for the chosen rule **/
		UINT m_dwGeneralIndices;                    /**< [List] All general indices **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
		UINT m_dwFetchedShaderHashcodes;            /**< [List] All shader hash codes using the temporary render target (HUD/GUI) ***/

		UINT m_dwConstantName;                      /**< [Switch] Shader constant name **/
		UINT m_dwPartialName;                       /**< [Switch] Shader constant partial name **/
		UINT m_dwBufferIndex;                       /**< [Switch] Buffer index **/
		UINT m_dwBufferSize;                        /**< [Switch] Buffer size **/
		UINT m_dwStartRegIndex;                     /**< [Switch] Start register index **/

		UINT m_dwRegisterCount;                     /**< [Spin] Register count **/
		UINT m_dwOperationToApply;                  /**< [Spin] Operation to apply **/
		UINT m_dwTranspose;                         /**< [Switch] Transpose Yes/No **/

		UINT m_dwCreate;                            /**< [Button] Create a rule **/
		UINT m_dwDeleteLatest;                      /**< [Button] Delete latest rule **/
		UINT m_dwAddGeneral;                        /**< [Button] Add to general indices **/
		UINT m_dwDeleteGeneral;                     /**< [Button] Delete chosen general index **/
		UINT m_dwBufferIndexDebug;                  /**< [Switch] : Activate to output all possible buffer sizes to the debug trace for the index in m_dwBufferIndex.***/

		// string entries for the switches above
		std::wstring m_szConstantName;
		std::wstring m_szPartialName;
		std::wstring m_szBufferIndex;
		std::wstring m_szBufferSize;
		std::wstring m_szStartRegIndex;

		// values of the controls
		bool m_bConstantName;
		bool m_bPartialName;
		bool m_bBufferIndex;
		bool m_bBufferSize;
		bool m_bStartRegIndex;
		bool m_bTranspose;
		UINT m_dwOperationValue;
		UINT m_dwRegCountValue;
#elif defined(VIREIO_D3D9)
		UINT m_dwShaderIndices;                     /**< [List] All shader-specific indices **/
#endif
	} m_sPageGameShaderRules;
	/**
	* This structure contains all DX version specific technical options
	* you have to influence or optimize the way the MatrixModifier works.
	***/
	struct TechnicalOptions
	{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)

#endif
	} m_sTechnicalOptions;
	/**
	* List of all available vertex shader hash codes (std::wstring).
	* To be used on the shader page, the debug page
	* and to create shader rules.
	* Each entry MUST MATCH same entry in m_adwShaderHashCodes.
	***/
	std::vector<std::wstring> m_aszVShaderHashCodes;
	/**
	* List of all available vertex shader hash codes (UINT).
	* To be used on the shader page, the debug page
	* and to create shader rules.
	* Each entry MUST MATCH same entry in m_aszShaderHashCodes.
	***/
	std::vector<UINT> m_adwVShaderHashCodes;
	/**
	* List of all available pixel shader hash codes (std::wstring).
	* To be used on the shader page, the debug page
	* and to create shader rules.
	* Each entry MUST MATCH same entry in m_adwShaderHashCodes.
	***/
	std::vector<std::wstring> m_aszPShaderHashCodes;
	/**
	* List of all available pixel shader hash codes (UINT).
	* To be used on the shader page, the debug page
	* and to create shader rules.
	* Each entry MUST MATCH same entry in m_aszShaderHashCodes.
	***/
	std::vector<UINT> m_adwPShaderHashCodes;
	/**
	* List of all available shader constant names (std::wstring).
	* To be used on the shader modifaction page, the debug page
	* and to create shader rules.
	***/
	std::vector<std::wstring> m_aszShaderConstants;
	/**
	* List of all available shader constant names (std::string).
	* To be used on the shader modifaction page, the debug page
	* and to create shader rules.
	***/
	std::vector<std::string> m_aszShaderConstantsA;
	/**
	* List of shader constant names for the currently chosen shader. (std::wstring).
	* To be used on the shader modifaction page
	* and to create shader rules.
	***/
	std::vector<std::wstring> m_aszShaderConstantsCurrent;
	/**
	* List of shader buffer sizes for the currently chosen shader. (std::wstring).
	* To be used on the shader modifaction page
	* and to create shader rules.
	***/
	std::vector<std::wstring> m_aszShaderBuffersizes;
	/**
	* List of all shader rule indices present. (std::wstring).
	* To be used on the shader rules page.
	***/
	std::vector<std::wstring> m_aszShaderRuleIndices;
	/**
	* List of shader rule data for the chosen shader rule index (std::wstring).
	* To be used on the shader rules page.
	***/
	std::vector<std::wstring> m_aszShaderRuleData;
	/**
	* List of all shader rule indices generally used. (std::wstring).
	* To be used on the shader rules page.
	***/
	std::vector<std::wstring> m_aszShaderRuleGeneralIndices;
	/**
	* Debug trace string list.
	* Contains all strings for the debug trace.
	***/
	std::vector<std::wstring> m_aszDebugTrace;
	/**
	* Currently chosen option to grab debug text.
	***/
	Debug_Grab_Options m_eDebugOption;
	/**
	* If true debug text will be grabbed by the chosen debug option (m_eDebugOption)
	***/
	bool m_bGrabDebug;
	/**
	* True if the shader list is to be sorted.
	***/
	bool m_bSortShaderList;
	/**
	* Data buffer to save this node.
	***/
	char m_acData[MAX_DATA_SIZE];
	/**
	* Option to switch the render target for game HUD and GUI.
	***/
	BOOL m_bSwitchRenderTarget;
	/**
	* True if HUD operation (Viewport squish or Render Target switch) executed.
	***/
	BOOL m_bHudOperation;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/**
	* True if the buffer index sizes ar provided to the debug trace.
	***/
	bool m_bBufferIndexDebug;
	/**
	* Vector helper for the buffer index debug output.
	***/
	std::vector<UINT> m_aunBufferIndexSizesDebug;
	/**
	* List of all fetched shader hash codes. (std::wstring).
	* To be used on the shader rules page.
	***/
	std::vector<std::wstring> m_aszFetchedHashCodes;
	/**
	* List of all fetched shader hash codes. (UINT).
	* To be used on the shader rules page.
	***/
	std::vector<UINT> m_aunFetchedHashCodes;
#elif defined(VIREIO_D3D9)
	/**
	* List of all shaderrule indices for the currently chosen shader on the shaders page. (std::wstring).
	* (DX9 only)
	***/
	std::vector<std::wstring> m_aszShaderRuleShaderIndices;
#endif
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	MatrixModifier* pMatrixModifier = new MatrixModifier();
	return static_cast<AQU_Nodus*>(pMatrixModifier);
}

