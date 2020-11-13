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
#ifndef DCL_ID3D11DEVICECONTEXT_CLASS
#define DCL_ID3D11DEVICECONTEXT_CLASS

#include <d3d11.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include "VMT_ID3D11DeviceContext.h"
#include "AQU_TransferSite.h"

/**
* ID3D11Device detour class.
***/
class DCL_ID3D11DeviceContext
{
public:
	DCL_ID3D11DeviceContext(AQU_TransferSite* pcTransferSite);
	~DCL_ID3D11DeviceContext(void);

	/*** IUnknown methods ***/	
	HRESULT                   WINAPI QueryInterface(ID3D11DeviceContext *pcThis, REFIID riid, void **ppvObject);
	ULONG                     WINAPI AddRef(ID3D11DeviceContext *pcThis);
	ULONG                     WINAPI Release(ID3D11DeviceContext *pcThis);

	/*** ID3D11DeviceChild public methods ***/
	void                      WINAPI GetDevice(ID3D11DeviceContext* pcThis, ID3D11Device** ppDevice);
	HRESULT                   WINAPI GetPrivateData(ID3D11DeviceContext* pcThis, REFGUID guid, UINT* pDataSize, void* pData);
	HRESULT                   WINAPI SetPrivateData(ID3D11DeviceContext* pcThis, REFGUID guid, UINT DataSize, void* pData);
	HRESULT                   WINAPI SetPrivateDataInterface(ID3D11DeviceContext* pcThis, REFGUID guid, IUnknown* pData);

	/*** ID3D11DeviceContext methods ***/  
	void                      WINAPI VSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI PSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI PSSetShader(ID3D11DeviceContext *pcThis, ID3D11PixelShader *pPixelShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI PSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI VSSetShader(ID3D11DeviceContext *pcThis, ID3D11VertexShader *pVertexShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI DrawIndexed(ID3D11DeviceContext *pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void                      WINAPI Draw(ID3D11DeviceContext *pcThis, UINT VertexCount, UINT StartVertexLocation);
	HRESULT                   WINAPI Map(ID3D11DeviceContext *pcThis, ID3D11Resource *pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE *pMappedResource);
	void                      WINAPI Unmap(ID3D11DeviceContext *pcThis, ID3D11Resource *pResource, UINT Subresource);
	void                      WINAPI PSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI IASetInputLayout(ID3D11DeviceContext *pcThis, ID3D11InputLayout *pInputLayout);
	void                      WINAPI IASetVertexBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets);
	void                      WINAPI IASetIndexBuffer(ID3D11DeviceContext *pcThis, ID3D11Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
	void                      WINAPI DrawIndexedInstanced(ID3D11DeviceContext *pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	void                      WINAPI DrawInstanced(ID3D11DeviceContext *pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void                      WINAPI GSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI GSSetShader(ID3D11DeviceContext *pcThis, ID3D11GeometryShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI IASetPrimitiveTopology(ID3D11DeviceContext *pcThis, D3D11_PRIMITIVE_TOPOLOGY Topology);
	void                      WINAPI VSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot,  UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI VSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI Begin(ID3D11DeviceContext *pcThis, ID3D11Asynchronous *pAsync);
	void                      WINAPI End(ID3D11DeviceContext *pcThis, ID3D11Asynchronous *pAsync);
	HRESULT                   WINAPI GetData(ID3D11DeviceContext *pcThis, ID3D11Asynchronous *pAsync, void *pData, UINT DataSize, UINT GetDataFlags);
	void                      WINAPI SetPredication(ID3D11DeviceContext *pcThis, ID3D11Predicate *pPredicate, BOOL PredicateValue);
	void                      WINAPI GSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI GSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI OMSetRenderTargets(ID3D11DeviceContext *pcThis, UINT NumViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView);
	void                      WINAPI OMSetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext *pcThis, UINT NumRTVs, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts);
	void                      WINAPI OMSetBlendState(ID3D11DeviceContext *pcThis, ID3D11BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask);
	void                      WINAPI OMSetDepthStencilState(ID3D11DeviceContext *pcThis, ID3D11DepthStencilState *pDepthStencilState, UINT StencilRef);
	void                      WINAPI SOSetTargets(ID3D11DeviceContext *pcThis, UINT NumBuffers, ID3D11Buffer *const *ppSOTargets, const UINT *pOffsets);
	void                      WINAPI DrawAuto(ID3D11DeviceContext *pcThis);
	void                      WINAPI DrawIndexedInstancedIndirect(ID3D11DeviceContext *pcThis, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void                      WINAPI DrawInstancedIndirect(ID3D11DeviceContext *pcThis, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void                      WINAPI Dispatch(ID3D11DeviceContext *pcThis, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
	void                      WINAPI DispatchIndirect(ID3D11DeviceContext *pcThis, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void                      WINAPI RSSetState(ID3D11DeviceContext *pcThis, ID3D11RasterizerState *pRasterizerState);
	void                      WINAPI RSSetViewports(ID3D11DeviceContext *pcThis, UINT NumViewports, const D3D11_VIEWPORT *pViewports);
	void                      WINAPI RSSetScissorRects(ID3D11DeviceContext *pcThis, UINT NumRects, const D3D11_RECT *pRects);
	void                      WINAPI CopySubresourceRegion(ID3D11DeviceContext *pcThis, ID3D11Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox);
	void                      WINAPI CopyResource(ID3D11DeviceContext *pcThis, ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource);
	void                      WINAPI UpdateSubresource(ID3D11DeviceContext *pcThis, ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
	void                      WINAPI CopyStructureCount(ID3D11DeviceContext *pcThis, ID3D11Buffer *pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView *pSrcView);
	void                      WINAPI ClearRenderTargetView(ID3D11DeviceContext *pcThis, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ]);
	void                      WINAPI ClearUnorderedAccessViewUint(ID3D11DeviceContext *pcThis, ID3D11UnorderedAccessView *pUnorderedAccessView, const UINT Values[ 4 ]);
	void                      WINAPI ClearUnorderedAccessViewFloat(ID3D11DeviceContext *pcThis, ID3D11UnorderedAccessView *pUnorderedAccessView, const FLOAT Values[ 4 ]);
	void                      WINAPI ClearDepthStencilView(ID3D11DeviceContext *pcThis, ID3D11DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);
	void                      WINAPI GenerateMips(ID3D11DeviceContext *pcThis, ID3D11ShaderResourceView *pShaderResourceView);
	void                      WINAPI SetResourceMinLOD(ID3D11DeviceContext *pcThis, ID3D11Resource *pResource, FLOAT MinLOD);
	FLOAT                     WINAPI GetResourceMinLOD(ID3D11DeviceContext *pcThis, ID3D11Resource *pResource);
	void                      WINAPI ResolveSubresource(ID3D11DeviceContext *pcThis, ID3D11Resource *pDstResource, UINT DstSubresource, ID3D11Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format);
	void                      WINAPI ExecuteCommandList(ID3D11DeviceContext *pcThis, ID3D11CommandList *pCommandList, BOOL RestoreContextState);
	void                      WINAPI HSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI HSSetShader(ID3D11DeviceContext *pcThis, ID3D11HullShader *pHullShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI HSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI HSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI DSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI DSSetShader(ID3D11DeviceContext *pcThis, ID3D11DomainShader *pDomainShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI DSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI DSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI CSSetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void                      WINAPI CSSetUnorderedAccessViews(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts);
	void                      WINAPI CSSetShader(ID3D11DeviceContext *pcThis, ID3D11ComputeShader *pComputeShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	void                      WINAPI CSSetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void                      WINAPI CSSetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void                      WINAPI VSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI PSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI PSGetShader(ID3D11DeviceContext *pcThis, ID3D11PixelShader **ppPixelShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI PSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI VSGetShader(ID3D11DeviceContext *pcThis, ID3D11VertexShader **ppVertexShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI PSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI IAGetInputLayout(ID3D11DeviceContext *pcThis, ID3D11InputLayout **ppInputLayout);
	void                      WINAPI IAGetVertexBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets);
	void                      WINAPI IAGetIndexBuffer(ID3D11DeviceContext *pcThis, ID3D11Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset);
	void                      WINAPI GSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI GSGetShader(ID3D11DeviceContext *pcThis, ID3D11GeometryShader **ppGeometryShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI IAGetPrimitiveTopology(ID3D11DeviceContext *pcThis, D3D11_PRIMITIVE_TOPOLOGY *pTopology);
	void                      WINAPI VSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI VSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI GetPredication(ID3D11DeviceContext *pcThis, ID3D11Predicate **ppPredicate, BOOL *pPredicateValue);
	void                      WINAPI GSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI GSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI OMGetRenderTargets(ID3D11DeviceContext *pcThis, UINT NumViews, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView);
	void                      WINAPI OMGetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext *pcThis, UINT NumRTVs, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews);
	void                      WINAPI OMGetBlendState(ID3D11DeviceContext *pcThis, ID3D11BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask);
	void                      WINAPI OMGetDepthStencilState(ID3D11DeviceContext *pcThis, ID3D11DepthStencilState **ppDepthStencilState, UINT *pStencilRef);
	void                      WINAPI SOGetTargets(ID3D11DeviceContext *pcThis, UINT NumBuffers, ID3D11Buffer **ppSOTargets);
	void                      WINAPI RSGetState(ID3D11DeviceContext *pcThis, ID3D11RasterizerState **ppRasterizerState);
	void                      WINAPI RSGetViewports(ID3D11DeviceContext *pcThis, UINT *pNumViewports, D3D11_VIEWPORT *pViewports);
	void                      WINAPI RSGetScissorRects(ID3D11DeviceContext *pcThis, UINT *pNumRects, D3D11_RECT *pRects);
	void                      WINAPI HSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI HSGetShader(ID3D11DeviceContext *pcThis, ID3D11HullShader **ppHullShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI HSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI HSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI DSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI DSGetShader(ID3D11DeviceContext *pcThis, ID3D11DomainShader **ppDomainShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI DSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI DSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI CSGetShaderResources(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews);
	void                      WINAPI CSGetUnorderedAccessViews(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews);
	void                      WINAPI CSGetShader(ID3D11DeviceContext *pcThis, ID3D11ComputeShader **ppComputeShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances);
	void                      WINAPI CSGetSamplers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers);
	void                      WINAPI CSGetConstantBuffers(ID3D11DeviceContext *pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	void                      WINAPI ClearState(ID3D11DeviceContext *pcThis);
	void                      WINAPI Flush(ID3D11DeviceContext *pcThis);
	D3D11_DEVICE_CONTEXT_TYPE WINAPI GetType(ID3D11DeviceContext *pcThis);
	UINT                      WINAPI GetContextFlags(ID3D11DeviceContext *pcThis);
	HRESULT                   WINAPI FinishCommandList(ID3D11DeviceContext *pcThis, BOOL RestoreDeferredContextState, ID3D11CommandList **ppCommandList);

	/*** DCL_ID3D11DeviceContext public methods ***/
	HRESULT           SetSuperFunctionPointer              (VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT           SetSuperFunctionPointer              (VMT_ID3D11DEVICECHILD::VMT_ID3D11DeviceChild eFunc, UINT_PTR dwFunc);
	HRESULT           SetSuperFunctionPointer              (VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext eFunc, UINT_PTR dwFunc);
	HRESULT           SetSuperFunctionPointers             (PUINT_PTR pVMTable);
	HRESULT           SetDXGISwapChain                     (IDXGISwapChain* pcSwapChain) { m_pcSwapChain = pcSwapChain; if (m_pcSwapChain) m_pcSwapChain->AddRef(); return S_OK; }
	HRESULT           GetDXGISwapChain                     (IDXGISwapChain** ppcSwapChain);

	/**
	* The swapchain.
	***/
	IDXGISwapChain* m_pcSwapChain;
	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
};

#endif
