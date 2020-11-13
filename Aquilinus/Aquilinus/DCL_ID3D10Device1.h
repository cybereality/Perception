/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
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
#ifndef DCL_ID3D10DEVICE1_CLASS
#define DCL_ID3D10DEVICE1_CLASS

#include <d3d10_1.h>
#include "VMT_ID3D10Device1.h"
#include "AQU_TransferSite.h"

/**
* ID3D10Device1 detour class.
***/
class DCL_ID3D10Device1
{
public:
	DCL_ID3D10Device1(AQU_TransferSite* pcTransferSite);
	~DCL_ID3D10Device1();

	/*** IUnknown public methods ***/
	HRESULT WINAPI QueryInterface                       (ID3D10Device1* pcThis, REFIID riid, void** ppvObject);
	ULONG WINAPI AddRef                               (ID3D10Device1* pcThis );
	ULONG WINAPI Release                              (ID3D10Device1* pcThis );
	/*** ID3D10Device public methods ***/
	void WINAPI VSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI PSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI PSSetShader                          (ID3D10Device1* pcThis, ID3D10PixelShader* pPixelShader);
	void WINAPI PSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI VSSetShader                          (ID3D10Device1* pcThis, ID3D10VertexShader* pVertexShader);
	void WINAPI DrawIndexed                          (ID3D10Device1* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void WINAPI Draw                                 (ID3D10Device1* pcThis, UINT VertexCount, UINT StartVertexLocation);
	void WINAPI PSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI IASetInputLayout                     (ID3D10Device1* pcThis, ID3D10InputLayout* pInputLayout);
	void WINAPI IASetVertexBuffers                   (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets);
	void WINAPI IASetIndexBuffer                     (ID3D10Device1* pcThis, ID3D10Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
	void WINAPI DrawIndexedInstanced                 (ID3D10Device1* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	void WINAPI DrawInstanced                        (ID3D10Device1* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void WINAPI GSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI GSSetShader                          (ID3D10Device1* pcThis, ID3D10GeometryShader* pShader);
	void WINAPI IASetPrimitiveTopology               (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY Topology);
	void WINAPI VSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI VSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI SetPredication                       (ID3D10Device1* pcThis, ID3D10Predicate* pPredicate, BOOL PredicateValue);
	void WINAPI GSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI GSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI OMSetRenderTargets                   (ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView* pDepthStencilView);
	void WINAPI OMSetBlendState                      (ID3D10Device1* pcThis, ID3D10BlendState* pBlendState, FLOAT* BlendFactor, UINT SampleMask);
	void WINAPI OMSetDepthStencilState               (ID3D10Device1* pcThis, ID3D10DepthStencilState* pDepthStencilState, UINT StencilRef);
	void WINAPI SOSetTargets                         (ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets);
	void WINAPI DrawAuto                             (ID3D10Device1* pcThis );
	void WINAPI RSSetState                           (ID3D10Device1* pcThis, ID3D10RasterizerState* pRasterizerState);
	void WINAPI RSSetViewports                       (ID3D10Device1* pcThis, UINT NumViewports, D3D10_VIEWPORT* pViewports);
	void WINAPI RSSetScissorRects                    (ID3D10Device1* pcThis, UINT NumRects, D3D10_RECT* pRects);
	void WINAPI CopySubresourceRegion                (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource* pSrcResource, UINT SrcSubresource, D3D10_BOX* pSrcBox);
	void WINAPI CopyResource                         (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, ID3D10Resource* pSrcResource);
	void WINAPI UpdateSubresource                    (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, D3D10_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
	void WINAPI ClearRenderTargetView                (ID3D10Device1* pcThis, ID3D10RenderTargetView* pRenderTargetView, FLOAT* ColorRGBA);
	void WINAPI ClearDepthStencilView                (ID3D10Device1* pcThis, ID3D10DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);
	void WINAPI GenerateMips                         (ID3D10Device1* pcThis, ID3D10ShaderResourceView* pShaderResourceView);
	void WINAPI ResolveSubresource                   (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, ID3D10Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format);
	void WINAPI VSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI PSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI PSGetShader                          (ID3D10Device1* pcThis, ID3D10PixelShader** ppPixelShader);
	void WINAPI PSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI VSGetShader                          (ID3D10Device1* pcThis, ID3D10VertexShader** ppVertexShader);
	void WINAPI PSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI IAGetInputLayout                     (ID3D10Device1* pcThis, ID3D10InputLayout** ppInputLayout);
	void WINAPI IAGetVertexBuffers                   (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets);
	void WINAPI IAGetIndexBuffer                     (ID3D10Device1* pcThis, ID3D10Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset);
	void WINAPI GSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers);
	void WINAPI GSGetShader                          (ID3D10Device1* pcThis, ID3D10GeometryShader** ppGeometryShader);
	void WINAPI IAGetPrimitiveTopology               (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY* pTopology);
	void WINAPI VSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI VSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI GetPredication                       (ID3D10Device1* pcThis, ID3D10Predicate** ppPredicate, BOOL* pPredicateValue);
	void WINAPI GSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews);
	void WINAPI GSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers);
	void WINAPI OMGetRenderTargets                   (ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView** ppDepthStencilView);
	void WINAPI OMGetBlendState                      (ID3D10Device1* pcThis, ID3D10BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask);
	void WINAPI OMGetDepthStencilState               (ID3D10Device1* pcThis, ID3D10DepthStencilState** ppDepthStencilState, UINT* pStencilRef);
	void WINAPI SOGetTargets                         (ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets);
	void WINAPI RSGetState                           (ID3D10Device1* pcThis, ID3D10RasterizerState** ppRasterizerState);
	void WINAPI RSGetViewports                       (ID3D10Device1* pcThis, UINT* NumViewports, D3D10_VIEWPORT* pViewports);
	void WINAPI RSGetScissorRects                    (ID3D10Device1* pcThis, UINT* NumRects, D3D10_RECT* pRects);
	HRESULT WINAPI GetDeviceRemovedReason               (ID3D10Device1* pcThis );
	HRESULT WINAPI SetExceptionMode                     (ID3D10Device1* pcThis, UINT RaiseFlags);
	UINT WINAPI GetExceptionMode                     (ID3D10Device1* pcThis );
	HRESULT WINAPI GetPrivateData                       (ID3D10Device1* pcThis, REFGUID guid, UINT* pDataSize, void* pData);
	HRESULT WINAPI SetPrivateData                       (ID3D10Device1* pcThis, REFGUID guid, UINT DataSize, void* pData);
	HRESULT WINAPI SetPrivateDataInterface              (ID3D10Device1* pcThis, REFGUID guid, IUnknown* pData);
	void WINAPI ClearState                           (ID3D10Device1* pcThis );
	void WINAPI Flush                                (ID3D10Device1* pcThis );
	HRESULT WINAPI CreateBuffer                         (ID3D10Device1* pcThis, D3D10_BUFFER_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Buffer** ppBuffer);
	HRESULT WINAPI CreateTexture1D                      (ID3D10Device1* pcThis, D3D10_TEXTURE1D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture1D** ppTexture1D);
	HRESULT WINAPI CreateTexture2D                      (ID3D10Device1* pcThis, D3D10_TEXTURE2D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture2D** ppTexture2D);
	HRESULT WINAPI CreateTexture3D                      (ID3D10Device1* pcThis, D3D10_TEXTURE3D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture3D** ppTexture3D);
	HRESULT WINAPI CreateShaderResourceView             (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D10ShaderResourceView** ppSRView);
	HRESULT WINAPI CreateRenderTargetView               (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_RENDER_TARGET_VIEW_DESC* pDesc, ID3D10RenderTargetView** ppRTView);
	HRESULT WINAPI CreateDepthStencilView               (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D10DepthStencilView** ppDepthStencilView);
	HRESULT WINAPI CreateInputLayout                    (ID3D10Device1* pcThis, D3D10_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout** ppInputLayout);
	HRESULT WINAPI CreateVertexShader                   (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader** ppVertexShader);
	HRESULT WINAPI CreateGeometryShader                 (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader** ppGeometryShader);
	HRESULT WINAPI CreateGeometryShaderWithStreamOutput (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, D3D10_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader** ppGeometryShader);
	HRESULT WINAPI CreatePixelShader                    (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader** ppPixelShader);
	HRESULT WINAPI CreateBlendState                     (ID3D10Device1* pcThis, D3D10_BLEND_DESC* pBlendStateDesc, ID3D10BlendState** ppBlendState);
	HRESULT WINAPI CreateDepthStencilState              (ID3D10Device1* pcThis, D3D10_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D10DepthStencilState** ppDepthStencilState);
	HRESULT WINAPI CreateRasterizerState                (ID3D10Device1* pcThis, D3D10_RASTERIZER_DESC* pRasterizerDesc, ID3D10RasterizerState** ppRasterizerState);
	HRESULT WINAPI CreateSamplerState                   (ID3D10Device1* pcThis, D3D10_SAMPLER_DESC* pSamplerDesc, ID3D10SamplerState** ppSamplerState);
	HRESULT WINAPI CreateQuery                          (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pQueryDesc, ID3D10Query** ppQuery);
	HRESULT WINAPI CreatePredicate                      (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pPredicateDesc, ID3D10Predicate** ppPredicate);
	HRESULT WINAPI CreateCounter                        (ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pCounterDesc, ID3D10Counter** ppCounter);
	HRESULT WINAPI CheckFormatSupport                   (ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport);
	HRESULT WINAPI CheckMultisampleQualityLevels        (ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels);
	void WINAPI CheckCounterInfo                     (ID3D10Device1* pcThis, D3D10_COUNTER_INFO* pCounterInfo);
	HRESULT WINAPI CheckCounter                         (ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pDesc, D3D10_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength);
	UINT WINAPI GetCreationFlags                     (ID3D10Device1* pcThis );
	HRESULT WINAPI OpenSharedResource                   (ID3D10Device1* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource);
	void WINAPI SetTextFilterSize                    (ID3D10Device1* pcThis, UINT Width, UINT Height);
	void WINAPI GetTextFilterSize                    (ID3D10Device1* pcThis, UINT* pWidth, UINT* pHeight);
	/*** ID3D10Device1 public methods ***/
	HRESULT WINAPI CreateShaderResourceView1(ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC1* pDesc, ID3D10ShaderResourceView1** ppSRView);
	HRESULT WINAPI CreateBlendState1(ID3D10Device1* pcThis, D3D10_BLEND_DESC1* pBlendStateDesc, ID3D10BlendState1** ppBlendState);
	D3D10_FEATURE_LEVEL1 WINAPI GetFeatureLevel(ID3D10Device1* pcThis );

	/*** DCL_ID3D10Device1 public methods ***/
	HRESULT SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_ID3D10DEVICE::VMT_ID3D10Device eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointer(VMT_ID3D10DEVICE1::VMT_ID3D10Device1 eFunc, UINT_PTR dwFunc);
	HRESULT SetSuperFunctionPointers(PUINT_PTR pVMTable);

	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
};

#endif
