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
#ifndef DCL_ID3D11DEVICECONTEXT1_SUPER
#define DCL_ID3D11DEVICECONTEXT1_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D11_QueryInterface) (ID3D11DeviceContext1* pcThis, REFIID riid, void** ppvObject);
typedef ULONG (WINAPI* D3D11_AddRef) (ID3D11DeviceContext1* pcThis );
typedef ULONG (WINAPI* D3D11_Release) (ID3D11DeviceContext1* pcThis );

/*** ID3D11DeviceContext method prototypes ***/
typedef void (WINAPI* D3D11_VSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_PSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_PSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11PixelShader* pPixelShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_PSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_VSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11VertexShader* pVertexShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_DrawIndexed) (ID3D11DeviceContext1* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void (WINAPI* D3D11_Draw) (ID3D11DeviceContext1* pcThis, UINT VertexCount, UINT StartVertexLocation);
typedef HRESULT (WINAPI* D3D11_Map) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE* pMappedResource);
typedef void (WINAPI* D3D11_Unmap) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, UINT Subresource);
typedef void (WINAPI* D3D11_PSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_IASetInputLayout) (ID3D11DeviceContext1* pcThis, ID3D11InputLayout* pInputLayout);
typedef void (WINAPI* D3D11_IASetVertexBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets);
typedef void (WINAPI* D3D11_IASetIndexBuffer) (ID3D11DeviceContext1* pcThis, ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
typedef void (WINAPI* D3D11_DrawIndexedInstanced) (ID3D11DeviceContext1* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void (WINAPI* D3D11_DrawInstanced) (ID3D11DeviceContext1* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void (WINAPI* D3D11_GSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_GSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11GeometryShader* pShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_IASetPrimitiveTopology) (ID3D11DeviceContext1* pcThis, D3D11_PRIMITIVE_TOPOLOGY Topology);
typedef void (WINAPI* D3D11_VSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_VSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_Begin) (ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync);
typedef void (WINAPI* D3D11_End) (ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync);
typedef HRESULT (WINAPI* D3D11_GetData) (ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync, void* pData, UINT DataSize, UINT GetDataFlags);
typedef void (WINAPI* D3D11_SetPredication) (ID3D11DeviceContext1* pcThis, ID3D11Predicate* pPredicate, BOOL PredicateValue);
typedef void (WINAPI* D3D11_GSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_GSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_OMSetRenderTargets) (ID3D11DeviceContext1* pcThis, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView);
typedef void (WINAPI* D3D11_OMSetRenderTargetsAndUnorderedAccessViews) (ID3D11DeviceContext1* pcThis, UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews, UINT* pUAVInitialCounts);
typedef void (WINAPI* D3D11_OMSetBlendState) (ID3D11DeviceContext1* pcThis, ID3D11BlendState* pBlendState, FLOAT* BlendFactor, UINT SampleMask);
typedef void (WINAPI* D3D11_OMSetDepthStencilState) (ID3D11DeviceContext1* pcThis, ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef);
typedef void (WINAPI* D3D11_SOSetTargets) (ID3D11DeviceContext1* pcThis, UINT NumBuffers, ID3D11Buffer** ppSOTargets, UINT* pOffsets);
typedef void (WINAPI* D3D11_DrawAuto) (ID3D11DeviceContext1* pcThis );
typedef void (WINAPI* D3D11_DrawIndexedInstancedIndirect) (ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void (WINAPI* D3D11_DrawInstancedIndirect) (ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void (WINAPI* D3D11_Dispatch) (ID3D11DeviceContext1* pcThis, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
typedef void (WINAPI* D3D11_DispatchIndirect) (ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void (WINAPI* D3D11_RSSetState) (ID3D11DeviceContext1* pcThis, ID3D11RasterizerState* pRasterizerState);
typedef void (WINAPI* D3D11_RSSetViewports) (ID3D11DeviceContext1* pcThis, UINT NumViewports, D3D11_VIEWPORT* pViewports);
typedef void (WINAPI* D3D11_RSSetScissorRects) (ID3D11DeviceContext1* pcThis, UINT NumRects, D3D11_RECT* pRects);
typedef void (WINAPI* D3D11_CopySubresourceRegion) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, D3D11_BOX* pSrcBox);
typedef void (WINAPI* D3D11_CopyResource) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource);
typedef void (WINAPI* D3D11_UpdateSubresource) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, D3D11_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
typedef void (WINAPI* D3D11_CopyStructureCount) (ID3D11DeviceContext1* pcThis, ID3D11Buffer* pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView* pSrcView);
typedef void (WINAPI* D3D11_ClearRenderTargetView) (ID3D11DeviceContext1* pcThis, ID3D11RenderTargetView* pRenderTargetView, FLOAT* ColorRGBA);
typedef void (WINAPI* D3D11_ClearUnorderedAccessViewUint) (ID3D11DeviceContext1* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, UINT* Values);
typedef void (WINAPI* D3D11_ClearUnorderedAccessViewFloat) (ID3D11DeviceContext1* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, FLOAT* Values);
typedef void (WINAPI* D3D11_ClearDepthStencilView) (ID3D11DeviceContext1* pcThis, ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);
typedef void (WINAPI* D3D11_GenerateMips) (ID3D11DeviceContext1* pcThis, ID3D11ShaderResourceView* pShaderResourceView);
typedef void (WINAPI* D3D11_SetResourceMinLOD) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, FLOAT MinLOD);
typedef FLOAT (WINAPI* D3D11_GetResourceMinLOD) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource);
typedef void (WINAPI* D3D11_ResolveSubresource) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, ID3D11Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format);
typedef void (WINAPI* D3D11_ExecuteCommandList) (ID3D11DeviceContext1* pcThis, ID3D11CommandList* pCommandList, BOOL RestoreContextState);
typedef void (WINAPI* D3D11_HSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_HSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11HullShader* pHullShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_HSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_HSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_DSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_DSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11DomainShader* pDomainShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_DSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_DSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_CSSetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_CSSetUnorderedAccessViews) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews, UINT* pUAVInitialCounts);
typedef void (WINAPI* D3D11_CSSetShader) (ID3D11DeviceContext1* pcThis, ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances);
typedef void (WINAPI* D3D11_CSSetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_CSSetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_VSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_PSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_PSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11PixelShader** ppPixelShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_PSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_VSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11VertexShader** ppVertexShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_PSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_IAGetInputLayout) (ID3D11DeviceContext1* pcThis, ID3D11InputLayout** ppInputLayout);
typedef void (WINAPI* D3D11_IAGetVertexBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets);
typedef void (WINAPI* D3D11_IAGetIndexBuffer) (ID3D11DeviceContext1* pcThis, ID3D11Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset);
typedef void (WINAPI* D3D11_GSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_GSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11GeometryShader** ppGeometryShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_IAGetPrimitiveTopology) (ID3D11DeviceContext1* pcThis, D3D11_PRIMITIVE_TOPOLOGY* pTopology);
typedef void (WINAPI* D3D11_VSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_VSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_GetPredication) (ID3D11DeviceContext1* pcThis, ID3D11Predicate** ppPredicate, BOOL* pPredicateValue);
typedef void (WINAPI* D3D11_GSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_GSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_OMGetRenderTargets) (ID3D11DeviceContext1* pcThis, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView);
typedef void (WINAPI* D3D11_OMGetRenderTargetsAndUnorderedAccessViews) (ID3D11DeviceContext1* pcThis, UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews);
typedef void (WINAPI* D3D11_OMGetBlendState) (ID3D11DeviceContext1* pcThis, ID3D11BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask);
typedef void (WINAPI* D3D11_OMGetDepthStencilState) (ID3D11DeviceContext1* pcThis, ID3D11DepthStencilState** ppDepthStencilState, UINT* pStencilRef);
typedef void (WINAPI* D3D11_SOGetTargets) (ID3D11DeviceContext1* pcThis, UINT NumBuffers, ID3D11Buffer** ppSOTargets);
typedef void (WINAPI* D3D11_RSGetState) (ID3D11DeviceContext1* pcThis, ID3D11RasterizerState** ppRasterizerState);
typedef void (WINAPI* D3D11_RSGetViewports) (ID3D11DeviceContext1* pcThis, UINT* pNumViewports, D3D11_VIEWPORT* pViewports);
typedef void (WINAPI* D3D11_RSGetScissorRects) (ID3D11DeviceContext1* pcThis, UINT* pNumRects, D3D11_RECT* pRects);
typedef void (WINAPI* D3D11_HSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_HSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11HullShader** ppHullShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_HSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_HSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_DSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_DSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11DomainShader** ppDomainShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_DSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_DSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_CSGetShaderResources) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews);
typedef void (WINAPI* D3D11_CSGetUnorderedAccessViews) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews);
typedef void (WINAPI* D3D11_CSGetShader) (ID3D11DeviceContext1* pcThis, ID3D11ComputeShader** ppComputeShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances);
typedef void (WINAPI* D3D11_CSGetSamplers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers);
typedef void (WINAPI* D3D11_CSGetConstantBuffers) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers);
typedef void (WINAPI* D3D11_ClearState) (ID3D11DeviceContext1* pcThis );
typedef void (WINAPI* D3D11_Flush) (ID3D11DeviceContext1* pcThis );
typedef D3D11_DEVICE_CONTEXT_TYPE (WINAPI* D3D11_GetType) (ID3D11DeviceContext1* pcThis );
typedef UINT (WINAPI* D3D11_GetContextFlags) (ID3D11DeviceContext1* pcThis );
typedef HRESULT (WINAPI* D3D11_FinishCommandList) (ID3D11DeviceContext1* pcThis, BOOL RestoreDeferredContextState, ID3D11CommandList** ppCommandList);

/*** ID3D11DeviceContext1 method prototypes ***/
typedef void (WINAPI* D3D11_CopySubresourceRegion1) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, D3D11_BOX* pSrcBox, UINT CopyFlags);
typedef void (WINAPI* D3D11_UpdateSubresource1) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, D3D11_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch, UINT CopyFlags);
typedef void (WINAPI* D3D11_DiscardResource) (ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource);
typedef void (WINAPI* D3D11_DiscardView) (ID3D11DeviceContext1* pcThis, ID3D11View* pResourceView);
typedef void (WINAPI* D3D11_VSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_HSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_DSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_GSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_PSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_CSSetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_VSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_HSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_DSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_GSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_PSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_CSGetConstantBuffers1) (ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants);
typedef void (WINAPI* D3D11_SwapDeviceContextState) (ID3D11DeviceContext1* pcThis, ID3DDeviceContextState* pState, ID3DDeviceContextState** ppPreviousState);
typedef void (WINAPI* D3D11_ClearView) (ID3D11DeviceContext1* pcThis, ID3D11View* pView, FLOAT* Color, D3D11_RECT* pRect, UINT NumRects);
typedef void (WINAPI* D3D11_DiscardView1) (ID3D11DeviceContext1* pcThis, ID3D11View* pResourceView, D3D11_RECT* pRects, UINT NumRects);

/*** IUnknown super methods ***/
D3D11_QueryInterface    D3D11_ID3D11DeviceContext1_QueryInterface_Super;
D3D11_AddRef    D3D11_ID3D11DeviceContext1_AddRef_Super;
D3D11_Release    D3D11_ID3D11DeviceContext1_Release_Super;

/*** ID3D11DeviceContext super methods ***/
D3D11_VSSetConstantBuffers    D3D11_ID3D11DeviceContext1_VSSetConstantBuffers_Super;
D3D11_PSSetShaderResources    D3D11_ID3D11DeviceContext1_PSSetShaderResources_Super;
D3D11_PSSetShader    D3D11_ID3D11DeviceContext1_PSSetShader_Super;
D3D11_PSSetSamplers    D3D11_ID3D11DeviceContext1_PSSetSamplers_Super;
D3D11_VSSetShader    D3D11_ID3D11DeviceContext1_VSSetShader_Super;
D3D11_DrawIndexed    D3D11_ID3D11DeviceContext1_DrawIndexed_Super;
D3D11_Draw    D3D11_ID3D11DeviceContext1_Draw_Super;
D3D11_Map    D3D11_ID3D11DeviceContext1_Map_Super;
D3D11_Unmap    D3D11_ID3D11DeviceContext1_Unmap_Super;
D3D11_PSSetConstantBuffers    D3D11_ID3D11DeviceContext1_PSSetConstantBuffers_Super;
D3D11_IASetInputLayout    D3D11_ID3D11DeviceContext1_IASetInputLayout_Super;
D3D11_IASetVertexBuffers    D3D11_ID3D11DeviceContext1_IASetVertexBuffers_Super;
D3D11_IASetIndexBuffer    D3D11_ID3D11DeviceContext1_IASetIndexBuffer_Super;
D3D11_DrawIndexedInstanced    D3D11_ID3D11DeviceContext1_DrawIndexedInstanced_Super;
D3D11_DrawInstanced    D3D11_ID3D11DeviceContext1_DrawInstanced_Super;
D3D11_GSSetConstantBuffers    D3D11_ID3D11DeviceContext1_GSSetConstantBuffers_Super;
D3D11_GSSetShader    D3D11_ID3D11DeviceContext1_GSSetShader_Super;
D3D11_IASetPrimitiveTopology    D3D11_ID3D11DeviceContext1_IASetPrimitiveTopology_Super;
D3D11_VSSetShaderResources    D3D11_ID3D11DeviceContext1_VSSetShaderResources_Super;
D3D11_VSSetSamplers    D3D11_ID3D11DeviceContext1_VSSetSamplers_Super;
D3D11_Begin    D3D11_ID3D11DeviceContext1_Begin_Super;
D3D11_End    D3D11_ID3D11DeviceContext1_End_Super;
D3D11_GetData    D3D11_ID3D11DeviceContext1_GetData_Super;
D3D11_SetPredication    D3D11_ID3D11DeviceContext1_SetPredication_Super;
D3D11_GSSetShaderResources    D3D11_ID3D11DeviceContext1_GSSetShaderResources_Super;
D3D11_GSSetSamplers    D3D11_ID3D11DeviceContext1_GSSetSamplers_Super;
D3D11_OMSetRenderTargets    D3D11_ID3D11DeviceContext1_OMSetRenderTargets_Super;
D3D11_OMSetRenderTargetsAndUnorderedAccessViews    D3D11_ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews_Super;
D3D11_OMSetBlendState    D3D11_ID3D11DeviceContext1_OMSetBlendState_Super;
D3D11_OMSetDepthStencilState    D3D11_ID3D11DeviceContext1_OMSetDepthStencilState_Super;
D3D11_SOSetTargets    D3D11_ID3D11DeviceContext1_SOSetTargets_Super;
D3D11_DrawAuto    D3D11_ID3D11DeviceContext1_DrawAuto_Super;
D3D11_DrawIndexedInstancedIndirect    D3D11_ID3D11DeviceContext1_DrawIndexedInstancedIndirect_Super;
D3D11_DrawInstancedIndirect    D3D11_ID3D11DeviceContext1_DrawInstancedIndirect_Super;
D3D11_Dispatch    D3D11_ID3D11DeviceContext1_Dispatch_Super;
D3D11_DispatchIndirect    D3D11_ID3D11DeviceContext1_DispatchIndirect_Super;
D3D11_RSSetState    D3D11_ID3D11DeviceContext1_RSSetState_Super;
D3D11_RSSetViewports    D3D11_ID3D11DeviceContext1_RSSetViewports_Super;
D3D11_RSSetScissorRects    D3D11_ID3D11DeviceContext1_RSSetScissorRects_Super;
D3D11_CopySubresourceRegion    D3D11_ID3D11DeviceContext1_CopySubresourceRegion_Super;
D3D11_CopyResource    D3D11_ID3D11DeviceContext1_CopyResource_Super;
D3D11_UpdateSubresource    D3D11_ID3D11DeviceContext1_UpdateSubresource_Super;
D3D11_CopyStructureCount    D3D11_ID3D11DeviceContext1_CopyStructureCount_Super;
D3D11_ClearRenderTargetView    D3D11_ID3D11DeviceContext1_ClearRenderTargetView_Super;
D3D11_ClearUnorderedAccessViewUint    D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewUint_Super;
D3D11_ClearUnorderedAccessViewFloat    D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewFloat_Super;
D3D11_ClearDepthStencilView    D3D11_ID3D11DeviceContext1_ClearDepthStencilView_Super;
D3D11_GenerateMips    D3D11_ID3D11DeviceContext1_GenerateMips_Super;
D3D11_SetResourceMinLOD    D3D11_ID3D11DeviceContext1_SetResourceMinLOD_Super;
D3D11_GetResourceMinLOD    D3D11_ID3D11DeviceContext1_GetResourceMinLOD_Super;
D3D11_ResolveSubresource    D3D11_ID3D11DeviceContext1_ResolveSubresource_Super;
D3D11_ExecuteCommandList    D3D11_ID3D11DeviceContext1_ExecuteCommandList_Super;
D3D11_HSSetShaderResources    D3D11_ID3D11DeviceContext1_HSSetShaderResources_Super;
D3D11_HSSetShader    D3D11_ID3D11DeviceContext1_HSSetShader_Super;
D3D11_HSSetSamplers    D3D11_ID3D11DeviceContext1_HSSetSamplers_Super;
D3D11_HSSetConstantBuffers    D3D11_ID3D11DeviceContext1_HSSetConstantBuffers_Super;
D3D11_DSSetShaderResources    D3D11_ID3D11DeviceContext1_DSSetShaderResources_Super;
D3D11_DSSetShader    D3D11_ID3D11DeviceContext1_DSSetShader_Super;
D3D11_DSSetSamplers    D3D11_ID3D11DeviceContext1_DSSetSamplers_Super;
D3D11_DSSetConstantBuffers    D3D11_ID3D11DeviceContext1_DSSetConstantBuffers_Super;
D3D11_CSSetShaderResources    D3D11_ID3D11DeviceContext1_CSSetShaderResources_Super;
D3D11_CSSetUnorderedAccessViews    D3D11_ID3D11DeviceContext1_CSSetUnorderedAccessViews_Super;
D3D11_CSSetShader    D3D11_ID3D11DeviceContext1_CSSetShader_Super;
D3D11_CSSetSamplers    D3D11_ID3D11DeviceContext1_CSSetSamplers_Super;
D3D11_CSSetConstantBuffers    D3D11_ID3D11DeviceContext1_CSSetConstantBuffers_Super;
D3D11_VSGetConstantBuffers    D3D11_ID3D11DeviceContext1_VSGetConstantBuffers_Super;
D3D11_PSGetShaderResources    D3D11_ID3D11DeviceContext1_PSGetShaderResources_Super;
D3D11_PSGetShader    D3D11_ID3D11DeviceContext1_PSGetShader_Super;
D3D11_PSGetSamplers    D3D11_ID3D11DeviceContext1_PSGetSamplers_Super;
D3D11_VSGetShader    D3D11_ID3D11DeviceContext1_VSGetShader_Super;
D3D11_PSGetConstantBuffers    D3D11_ID3D11DeviceContext1_PSGetConstantBuffers_Super;
D3D11_IAGetInputLayout    D3D11_ID3D11DeviceContext1_IAGetInputLayout_Super;
D3D11_IAGetVertexBuffers    D3D11_ID3D11DeviceContext1_IAGetVertexBuffers_Super;
D3D11_IAGetIndexBuffer    D3D11_ID3D11DeviceContext1_IAGetIndexBuffer_Super;
D3D11_GSGetConstantBuffers    D3D11_ID3D11DeviceContext1_GSGetConstantBuffers_Super;
D3D11_GSGetShader    D3D11_ID3D11DeviceContext1_GSGetShader_Super;
D3D11_IAGetPrimitiveTopology    D3D11_ID3D11DeviceContext1_IAGetPrimitiveTopology_Super;
D3D11_VSGetShaderResources    D3D11_ID3D11DeviceContext1_VSGetShaderResources_Super;
D3D11_VSGetSamplers    D3D11_ID3D11DeviceContext1_VSGetSamplers_Super;
D3D11_GetPredication    D3D11_ID3D11DeviceContext1_GetPredication_Super;
D3D11_GSGetShaderResources    D3D11_ID3D11DeviceContext1_GSGetShaderResources_Super;
D3D11_GSGetSamplers    D3D11_ID3D11DeviceContext1_GSGetSamplers_Super;
D3D11_OMGetRenderTargets    D3D11_ID3D11DeviceContext1_OMGetRenderTargets_Super;
D3D11_OMGetRenderTargetsAndUnorderedAccessViews    D3D11_ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews_Super;
D3D11_OMGetBlendState    D3D11_ID3D11DeviceContext1_OMGetBlendState_Super;
D3D11_OMGetDepthStencilState    D3D11_ID3D11DeviceContext1_OMGetDepthStencilState_Super;
D3D11_SOGetTargets    D3D11_ID3D11DeviceContext1_SOGetTargets_Super;
D3D11_RSGetState    D3D11_ID3D11DeviceContext1_RSGetState_Super;
D3D11_RSGetViewports    D3D11_ID3D11DeviceContext1_RSGetViewports_Super;
D3D11_RSGetScissorRects    D3D11_ID3D11DeviceContext1_RSGetScissorRects_Super;
D3D11_HSGetShaderResources    D3D11_ID3D11DeviceContext1_HSGetShaderResources_Super;
D3D11_HSGetShader    D3D11_ID3D11DeviceContext1_HSGetShader_Super;
D3D11_HSGetSamplers    D3D11_ID3D11DeviceContext1_HSGetSamplers_Super;
D3D11_HSGetConstantBuffers    D3D11_ID3D11DeviceContext1_HSGetConstantBuffers_Super;
D3D11_DSGetShaderResources    D3D11_ID3D11DeviceContext1_DSGetShaderResources_Super;
D3D11_DSGetShader    D3D11_ID3D11DeviceContext1_DSGetShader_Super;
D3D11_DSGetSamplers    D3D11_ID3D11DeviceContext1_DSGetSamplers_Super;
D3D11_DSGetConstantBuffers    D3D11_ID3D11DeviceContext1_DSGetConstantBuffers_Super;
D3D11_CSGetShaderResources    D3D11_ID3D11DeviceContext1_CSGetShaderResources_Super;
D3D11_CSGetUnorderedAccessViews    D3D11_ID3D11DeviceContext1_CSGetUnorderedAccessViews_Super;
D3D11_CSGetShader    D3D11_ID3D11DeviceContext1_CSGetShader_Super;
D3D11_CSGetSamplers    D3D11_ID3D11DeviceContext1_CSGetSamplers_Super;
D3D11_CSGetConstantBuffers    D3D11_ID3D11DeviceContext1_CSGetConstantBuffers_Super;
D3D11_ClearState    D3D11_ID3D11DeviceContext1_ClearState_Super;
D3D11_Flush    D3D11_ID3D11DeviceContext1_Flush_Super;
D3D11_GetType    D3D11_ID3D11DeviceContext1_GetType_Super;
D3D11_GetContextFlags    D3D11_ID3D11DeviceContext1_GetContextFlags_Super;
D3D11_FinishCommandList    D3D11_ID3D11DeviceContext1_FinishCommandList_Super;

/*** ID3D11DeviceContext1 super methods ***/
D3D11_CopySubresourceRegion1    D3D11_ID3D11DeviceContext1_CopySubresourceRegion1_Super;
D3D11_UpdateSubresource1    D3D11_ID3D11DeviceContext1_UpdateSubresource1_Super;
D3D11_DiscardResource    D3D11_ID3D11DeviceContext1_DiscardResource_Super;
D3D11_DiscardView    D3D11_ID3D11DeviceContext1_DiscardView_Super;
D3D11_VSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_VSSetConstantBuffers1_Super;
D3D11_HSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_HSSetConstantBuffers1_Super;
D3D11_DSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_DSSetConstantBuffers1_Super;
D3D11_GSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_GSSetConstantBuffers1_Super;
D3D11_PSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_PSSetConstantBuffers1_Super;
D3D11_CSSetConstantBuffers1    D3D11_ID3D11DeviceContext1_CSSetConstantBuffers1_Super;
D3D11_VSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_VSGetConstantBuffers1_Super;
D3D11_HSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_HSGetConstantBuffers1_Super;
D3D11_DSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_DSGetConstantBuffers1_Super;
D3D11_GSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_GSGetConstantBuffers1_Super;
D3D11_PSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_PSGetConstantBuffers1_Super;
D3D11_CSGetConstantBuffers1    D3D11_ID3D11DeviceContext1_CSGetConstantBuffers1_Super;
D3D11_SwapDeviceContextState    D3D11_ID3D11DeviceContext1_SwapDeviceContextState_Super;
D3D11_ClearView    D3D11_ID3D11DeviceContext1_ClearView_Super;
D3D11_DiscardView1    D3D11_ID3D11DeviceContext1_DiscardView1_Super;

#endif
