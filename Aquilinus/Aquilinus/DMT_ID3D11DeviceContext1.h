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
#include"AQU_Detour.h"
#ifndef DMT_ID3D11DEVICECONTEXT1_TABLE
#define DMT_ID3D11DEVICECONTEXT1_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D11_ID3D11DeviceContext1_QueryInterface(ID3D11DeviceContext1* pcThis, REFIID riid, void** ppvObject) { return pDCL_ID3D11DeviceContext1->QueryInterface(pcThis, riid, ppvObject);}
ULONG WINAPI D3D11_ID3D11DeviceContext1_AddRef(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->AddRef(pcThis);}
ULONG WINAPI D3D11_ID3D11DeviceContext1_Release(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->Release(pcThis);}

/*** ID3D11DeviceContext methods ***/
void WINAPI D3D11_ID3D11DeviceContext1_VSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->VSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_PSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->PSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_PSSetShader(ID3D11DeviceContext1* pcThis, ID3D11PixelShader* pPixelShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->PSSetShader(pcThis, pPixelShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_PSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->PSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_VSSetShader(ID3D11DeviceContext1* pcThis, ID3D11VertexShader* pVertexShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->VSSetShader(pcThis, pVertexShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawIndexed(ID3D11DeviceContext1* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) { return pDCL_ID3D11DeviceContext1->DrawIndexed(pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);}
void WINAPI D3D11_ID3D11DeviceContext1_Draw(ID3D11DeviceContext1* pcThis, UINT VertexCount, UINT StartVertexLocation) { return pDCL_ID3D11DeviceContext1->Draw(pcThis, VertexCount, StartVertexLocation);}
HRESULT WINAPI D3D11_ID3D11DeviceContext1_Map(ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE* pMappedResource) { return pDCL_ID3D11DeviceContext1->Map(pcThis, pResource, Subresource, MapType, MapFlags, pMappedResource);}
void WINAPI D3D11_ID3D11DeviceContext1_Unmap(ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, UINT Subresource) { return pDCL_ID3D11DeviceContext1->Unmap(pcThis, pResource, Subresource);}
void WINAPI D3D11_ID3D11DeviceContext1_PSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->PSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_IASetInputLayout(ID3D11DeviceContext1* pcThis, ID3D11InputLayout* pInputLayout) { return pDCL_ID3D11DeviceContext1->IASetInputLayout(pcThis, pInputLayout);}
void WINAPI D3D11_ID3D11DeviceContext1_IASetVertexBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets) { return pDCL_ID3D11DeviceContext1->IASetVertexBuffers(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);}
void WINAPI D3D11_ID3D11DeviceContext1_IASetIndexBuffer(ID3D11DeviceContext1* pcThis, ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset) { return pDCL_ID3D11DeviceContext1->IASetIndexBuffer(pcThis, pIndexBuffer, Format, Offset);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawIndexedInstanced(ID3D11DeviceContext1* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) { return pDCL_ID3D11DeviceContext1->DrawIndexedInstanced(pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawInstanced(ID3D11DeviceContext1* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) { return pDCL_ID3D11DeviceContext1->DrawInstanced(pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);}
void WINAPI D3D11_ID3D11DeviceContext1_GSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->GSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_GSSetShader(ID3D11DeviceContext1* pcThis, ID3D11GeometryShader* pShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->GSSetShader(pcThis, pShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_IASetPrimitiveTopology(ID3D11DeviceContext1* pcThis, D3D11_PRIMITIVE_TOPOLOGY Topology) { return pDCL_ID3D11DeviceContext1->IASetPrimitiveTopology(pcThis, Topology);}
void WINAPI D3D11_ID3D11DeviceContext1_VSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->VSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_VSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->VSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_Begin(ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync) { return pDCL_ID3D11DeviceContext1->Begin(pcThis, pAsync);}
void WINAPI D3D11_ID3D11DeviceContext1_End(ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync) { return pDCL_ID3D11DeviceContext1->End(pcThis, pAsync);}
HRESULT WINAPI D3D11_ID3D11DeviceContext1_GetData(ID3D11DeviceContext1* pcThis, ID3D11Asynchronous* pAsync, void* pData, UINT DataSize, UINT GetDataFlags) { return pDCL_ID3D11DeviceContext1->GetData(pcThis, pAsync, pData, DataSize, GetDataFlags);}
void WINAPI D3D11_ID3D11DeviceContext1_SetPredication(ID3D11DeviceContext1* pcThis, ID3D11Predicate* pPredicate, BOOL PredicateValue) { return pDCL_ID3D11DeviceContext1->SetPredication(pcThis, pPredicate, PredicateValue);}
void WINAPI D3D11_ID3D11DeviceContext1_GSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->GSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_GSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->GSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_OMSetRenderTargets(ID3D11DeviceContext1* pcThis, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView) { return pDCL_ID3D11DeviceContext1->OMSetRenderTargets(pcThis, NumViews, ppRenderTargetViews, pDepthStencilView);}
void WINAPI D3D11_ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext1* pcThis, UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews, UINT* pUAVInitialCounts) { return pDCL_ID3D11DeviceContext1->OMSetRenderTargetsAndUnorderedAccessViews(pcThis, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);}
void WINAPI D3D11_ID3D11DeviceContext1_OMSetBlendState(ID3D11DeviceContext1* pcThis, ID3D11BlendState* pBlendState, FLOAT* BlendFactor, UINT SampleMask) { return pDCL_ID3D11DeviceContext1->OMSetBlendState(pcThis, pBlendState, BlendFactor, SampleMask);}
void WINAPI D3D11_ID3D11DeviceContext1_OMSetDepthStencilState(ID3D11DeviceContext1* pcThis, ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef) { return pDCL_ID3D11DeviceContext1->OMSetDepthStencilState(pcThis, pDepthStencilState, StencilRef);}
void WINAPI D3D11_ID3D11DeviceContext1_SOSetTargets(ID3D11DeviceContext1* pcThis, UINT NumBuffers, ID3D11Buffer** ppSOTargets, UINT* pOffsets) { return pDCL_ID3D11DeviceContext1->SOSetTargets(pcThis, NumBuffers, ppSOTargets, pOffsets);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawAuto(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->DrawAuto(pcThis);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawIndexedInstancedIndirect(ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) { return pDCL_ID3D11DeviceContext1->DrawIndexedInstancedIndirect(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);}
void WINAPI D3D11_ID3D11DeviceContext1_DrawInstancedIndirect(ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) { return pDCL_ID3D11DeviceContext1->DrawInstancedIndirect(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);}
void WINAPI D3D11_ID3D11DeviceContext1_Dispatch(ID3D11DeviceContext1* pcThis, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ) { return pDCL_ID3D11DeviceContext1->Dispatch(pcThis, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);}
void WINAPI D3D11_ID3D11DeviceContext1_DispatchIndirect(ID3D11DeviceContext1* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) { return pDCL_ID3D11DeviceContext1->DispatchIndirect(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);}
void WINAPI D3D11_ID3D11DeviceContext1_RSSetState(ID3D11DeviceContext1* pcThis, ID3D11RasterizerState* pRasterizerState) { return pDCL_ID3D11DeviceContext1->RSSetState(pcThis, pRasterizerState);}
void WINAPI D3D11_ID3D11DeviceContext1_RSSetViewports(ID3D11DeviceContext1* pcThis, UINT NumViewports, D3D11_VIEWPORT* pViewports) { return pDCL_ID3D11DeviceContext1->RSSetViewports(pcThis, NumViewports, pViewports);}
void WINAPI D3D11_ID3D11DeviceContext1_RSSetScissorRects(ID3D11DeviceContext1* pcThis, UINT NumRects, D3D11_RECT* pRects) { return pDCL_ID3D11DeviceContext1->RSSetScissorRects(pcThis, NumRects, pRects);}
void WINAPI D3D11_ID3D11DeviceContext1_CopySubresourceRegion(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, D3D11_BOX* pSrcBox) { return pDCL_ID3D11DeviceContext1->CopySubresourceRegion(pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);}
void WINAPI D3D11_ID3D11DeviceContext1_CopyResource(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource) { return pDCL_ID3D11DeviceContext1->CopyResource(pcThis, pDstResource, pSrcResource);}
void WINAPI D3D11_ID3D11DeviceContext1_UpdateSubresource(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, D3D11_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch) { return pDCL_ID3D11DeviceContext1->UpdateSubresource(pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);}
void WINAPI D3D11_ID3D11DeviceContext1_CopyStructureCount(ID3D11DeviceContext1* pcThis, ID3D11Buffer* pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView* pSrcView) { return pDCL_ID3D11DeviceContext1->CopyStructureCount(pcThis, pDstBuffer, DstAlignedByteOffset, pSrcView);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearRenderTargetView(ID3D11DeviceContext1* pcThis, ID3D11RenderTargetView* pRenderTargetView, FLOAT* ColorRGBA) { return pDCL_ID3D11DeviceContext1->ClearRenderTargetView(pcThis, pRenderTargetView, ColorRGBA);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewUint(ID3D11DeviceContext1* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, UINT* Values) { return pDCL_ID3D11DeviceContext1->ClearUnorderedAccessViewUint(pcThis, pUnorderedAccessView, Values);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewFloat(ID3D11DeviceContext1* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, FLOAT* Values) { return pDCL_ID3D11DeviceContext1->ClearUnorderedAccessViewFloat(pcThis, pUnorderedAccessView, Values);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearDepthStencilView(ID3D11DeviceContext1* pcThis, ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil) { return pDCL_ID3D11DeviceContext1->ClearDepthStencilView(pcThis, pDepthStencilView, ClearFlags, Depth, Stencil);}
void WINAPI D3D11_ID3D11DeviceContext1_GenerateMips(ID3D11DeviceContext1* pcThis, ID3D11ShaderResourceView* pShaderResourceView) { return pDCL_ID3D11DeviceContext1->GenerateMips(pcThis, pShaderResourceView);}
void WINAPI D3D11_ID3D11DeviceContext1_SetResourceMinLOD(ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource, FLOAT MinLOD) { return pDCL_ID3D11DeviceContext1->SetResourceMinLOD(pcThis, pResource, MinLOD);}
FLOAT WINAPI D3D11_ID3D11DeviceContext1_GetResourceMinLOD(ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource) { return pDCL_ID3D11DeviceContext1->GetResourceMinLOD(pcThis, pResource);}
void WINAPI D3D11_ID3D11DeviceContext1_ResolveSubresource(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, ID3D11Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format) { return pDCL_ID3D11DeviceContext1->ResolveSubresource(pcThis, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);}
void WINAPI D3D11_ID3D11DeviceContext1_ExecuteCommandList(ID3D11DeviceContext1* pcThis, ID3D11CommandList* pCommandList, BOOL RestoreContextState) { return pDCL_ID3D11DeviceContext1->ExecuteCommandList(pcThis, pCommandList, RestoreContextState);}
void WINAPI D3D11_ID3D11DeviceContext1_HSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->HSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_HSSetShader(ID3D11DeviceContext1* pcThis, ID3D11HullShader* pHullShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->HSSetShader(pcThis, pHullShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_HSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->HSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_HSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->HSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_DSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->DSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_DSSetShader(ID3D11DeviceContext1* pcThis, ID3D11DomainShader* pDomainShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->DSSetShader(pcThis, pDomainShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_DSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->DSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_DSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->DSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->CSSetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetUnorderedAccessViews(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews, UINT* pUAVInitialCounts) { return pDCL_ID3D11DeviceContext1->CSSetUnorderedAccessViews(pcThis, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetShader(ID3D11DeviceContext1* pcThis, ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance** ppClassInstances, UINT NumClassInstances) { return pDCL_ID3D11DeviceContext1->CSSetShader(pcThis, pComputeShader, ppClassInstances, NumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->CSSetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->CSSetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_VSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->VSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_PSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->PSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_PSGetShader(ID3D11DeviceContext1* pcThis, ID3D11PixelShader** ppPixelShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->PSGetShader(pcThis, ppPixelShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_PSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->PSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_VSGetShader(ID3D11DeviceContext1* pcThis, ID3D11VertexShader** ppVertexShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->VSGetShader(pcThis, ppVertexShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_PSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->PSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_IAGetInputLayout(ID3D11DeviceContext1* pcThis, ID3D11InputLayout** ppInputLayout) { return pDCL_ID3D11DeviceContext1->IAGetInputLayout(pcThis, ppInputLayout);}
void WINAPI D3D11_ID3D11DeviceContext1_IAGetVertexBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets) { return pDCL_ID3D11DeviceContext1->IAGetVertexBuffers(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);}
void WINAPI D3D11_ID3D11DeviceContext1_IAGetIndexBuffer(ID3D11DeviceContext1* pcThis, ID3D11Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset) { return pDCL_ID3D11DeviceContext1->IAGetIndexBuffer(pcThis, pIndexBuffer, Format, Offset);}
void WINAPI D3D11_ID3D11DeviceContext1_GSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->GSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_GSGetShader(ID3D11DeviceContext1* pcThis, ID3D11GeometryShader** ppGeometryShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->GSGetShader(pcThis, ppGeometryShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_IAGetPrimitiveTopology(ID3D11DeviceContext1* pcThis, D3D11_PRIMITIVE_TOPOLOGY* pTopology) { return pDCL_ID3D11DeviceContext1->IAGetPrimitiveTopology(pcThis, pTopology);}
void WINAPI D3D11_ID3D11DeviceContext1_VSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->VSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_VSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->VSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_GetPredication(ID3D11DeviceContext1* pcThis, ID3D11Predicate** ppPredicate, BOOL* pPredicateValue) { return pDCL_ID3D11DeviceContext1->GetPredication(pcThis, ppPredicate, pPredicateValue);}
void WINAPI D3D11_ID3D11DeviceContext1_GSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->GSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_GSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->GSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_OMGetRenderTargets(ID3D11DeviceContext1* pcThis, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView) { return pDCL_ID3D11DeviceContext1->OMGetRenderTargets(pcThis, NumViews, ppRenderTargetViews, ppDepthStencilView);}
void WINAPI D3D11_ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext1* pcThis, UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews) { return pDCL_ID3D11DeviceContext1->OMGetRenderTargetsAndUnorderedAccessViews(pcThis, NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);}
void WINAPI D3D11_ID3D11DeviceContext1_OMGetBlendState(ID3D11DeviceContext1* pcThis, ID3D11BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask) { return pDCL_ID3D11DeviceContext1->OMGetBlendState(pcThis, ppBlendState, BlendFactor, pSampleMask);}
void WINAPI D3D11_ID3D11DeviceContext1_OMGetDepthStencilState(ID3D11DeviceContext1* pcThis, ID3D11DepthStencilState** ppDepthStencilState, UINT* pStencilRef) { return pDCL_ID3D11DeviceContext1->OMGetDepthStencilState(pcThis, ppDepthStencilState, pStencilRef);}
void WINAPI D3D11_ID3D11DeviceContext1_SOGetTargets(ID3D11DeviceContext1* pcThis, UINT NumBuffers, ID3D11Buffer** ppSOTargets) { return pDCL_ID3D11DeviceContext1->SOGetTargets(pcThis, NumBuffers, ppSOTargets);}
void WINAPI D3D11_ID3D11DeviceContext1_RSGetState(ID3D11DeviceContext1* pcThis, ID3D11RasterizerState** ppRasterizerState) { return pDCL_ID3D11DeviceContext1->RSGetState(pcThis, ppRasterizerState);}
void WINAPI D3D11_ID3D11DeviceContext1_RSGetViewports(ID3D11DeviceContext1* pcThis, UINT* pNumViewports, D3D11_VIEWPORT* pViewports) { return pDCL_ID3D11DeviceContext1->RSGetViewports(pcThis, pNumViewports, pViewports);}
void WINAPI D3D11_ID3D11DeviceContext1_RSGetScissorRects(ID3D11DeviceContext1* pcThis, UINT* pNumRects, D3D11_RECT* pRects) { return pDCL_ID3D11DeviceContext1->RSGetScissorRects(pcThis, pNumRects, pRects);}
void WINAPI D3D11_ID3D11DeviceContext1_HSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->HSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_HSGetShader(ID3D11DeviceContext1* pcThis, ID3D11HullShader** ppHullShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->HSGetShader(pcThis, ppHullShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_HSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->HSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_HSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->HSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_DSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->DSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_DSGetShader(ID3D11DeviceContext1* pcThis, ID3D11DomainShader** ppDomainShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->DSGetShader(pcThis, ppDomainShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_DSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->DSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_DSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->DSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetShaderResources(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D11DeviceContext1->CSGetShaderResources(pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetUnorderedAccessViews(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews) { return pDCL_ID3D11DeviceContext1->CSGetUnorderedAccessViews(pcThis, StartSlot, NumUAVs, ppUnorderedAccessViews);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetShader(ID3D11DeviceContext1* pcThis, ID3D11ComputeShader** ppComputeShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) { return pDCL_ID3D11DeviceContext1->CSGetShader(pcThis, ppComputeShader, ppClassInstances, pNumClassInstances);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetSamplers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) { return pDCL_ID3D11DeviceContext1->CSGetSamplers(pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetConstantBuffers(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) { return pDCL_ID3D11DeviceContext1->CSGetConstantBuffers(pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearState(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->ClearState(pcThis);}
void WINAPI D3D11_ID3D11DeviceContext1_Flush(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->Flush(pcThis);}
D3D11_DEVICE_CONTEXT_TYPE WINAPI D3D11_ID3D11DeviceContext1_GetType(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->GetType(pcThis);}
UINT WINAPI D3D11_ID3D11DeviceContext1_GetContextFlags(ID3D11DeviceContext1* pcThis ) { return pDCL_ID3D11DeviceContext1->GetContextFlags(pcThis);}
HRESULT WINAPI D3D11_ID3D11DeviceContext1_FinishCommandList(ID3D11DeviceContext1* pcThis, BOOL RestoreDeferredContextState, ID3D11CommandList** ppCommandList) { return pDCL_ID3D11DeviceContext1->FinishCommandList(pcThis, RestoreDeferredContextState, ppCommandList);}

/*** ID3D11DeviceContext1 methods ***/
void WINAPI D3D11_ID3D11DeviceContext1_CopySubresourceRegion1(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, D3D11_BOX* pSrcBox, UINT CopyFlags) { return pDCL_ID3D11DeviceContext1->CopySubresourceRegion1(pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox, CopyFlags);}
void WINAPI D3D11_ID3D11DeviceContext1_UpdateSubresource1(ID3D11DeviceContext1* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, D3D11_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch, UINT CopyFlags) { return pDCL_ID3D11DeviceContext1->UpdateSubresource1(pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch, CopyFlags);}
void WINAPI D3D11_ID3D11DeviceContext1_DiscardResource(ID3D11DeviceContext1* pcThis, ID3D11Resource* pResource) { return pDCL_ID3D11DeviceContext1->DiscardResource(pcThis, pResource);}
void WINAPI D3D11_ID3D11DeviceContext1_DiscardView(ID3D11DeviceContext1* pcThis, ID3D11View* pResourceView) { return pDCL_ID3D11DeviceContext1->DiscardView(pcThis, pResourceView);}
void WINAPI D3D11_ID3D11DeviceContext1_VSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->VSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_HSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->HSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_DSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->DSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_GSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->GSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_PSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->PSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_CSSetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->CSSetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_VSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->VSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_HSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->HSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_DSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->DSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_GSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->GSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_PSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->PSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_CSGetConstantBuffers1(ID3D11DeviceContext1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers, UINT* pFirstConstant, UINT* pNumConstants) { return pDCL_ID3D11DeviceContext1->CSGetConstantBuffers1(pcThis, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);}
void WINAPI D3D11_ID3D11DeviceContext1_SwapDeviceContextState(ID3D11DeviceContext1* pcThis, ID3DDeviceContextState* pState, ID3DDeviceContextState** ppPreviousState) { return pDCL_ID3D11DeviceContext1->SwapDeviceContextState(pcThis, pState, ppPreviousState);}
void WINAPI D3D11_ID3D11DeviceContext1_ClearView(ID3D11DeviceContext1* pcThis, ID3D11View* pView, FLOAT* Color, D3D11_RECT* pRect, UINT NumRects) { return pDCL_ID3D11DeviceContext1->ClearView(pcThis, pView, Color, pRect, NumRects);}
void WINAPI D3D11_ID3D11DeviceContext1_DiscardView1(ID3D11DeviceContext1* pcThis, ID3D11View* pResourceView, D3D11_RECT* pRects, UINT NumRects) { return pDCL_ID3D11DeviceContext1->DiscardView1(pcThis, pResourceView, pRects, NumRects);}


/**
* Overrides the Virtual Methods Table of ID3D11DeviceContext1
***/
void Override_D3D11_ID3D11DeviceContext1_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::QueryInterface], (UINT_PTR)D3D11_ID3D11DeviceContext1_QueryInterface);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::AddRef], (UINT_PTR)D3D11_ID3D11DeviceContext1_AddRef);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::Release], (UINT_PTR)D3D11_ID3D11DeviceContext1_Release);

	/*** ID3D11DeviceContext super methods ***/
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexed], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexed);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Draw], (UINT_PTR)D3D11_ID3D11DeviceContext1_Draw);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Map], (UINT_PTR)D3D11_ID3D11DeviceContext1_Map);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Unmap], (UINT_PTR)D3D11_ID3D11DeviceContext1_Unmap);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetInputLayout], (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetInputLayout);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetVertexBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer], (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetIndexBuffer);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexedInstanced);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstanced], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawInstanced);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology], (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetPrimitiveTopology);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Begin], (UINT_PTR)D3D11_ID3D11DeviceContext1_Begin);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::End], (UINT_PTR)D3D11_ID3D11DeviceContext1_End);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetData], (UINT_PTR)D3D11_ID3D11DeviceContext1_GetData);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetPredication], (UINT_PTR)D3D11_ID3D11DeviceContext1_SetPredication);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetRenderTargets);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetBlendState], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetBlendState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetDepthStencilState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOSetTargets], (UINT_PTR)D3D11_ID3D11DeviceContext1_SOSetTargets);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawAuto], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawAuto);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexedInstancedIndirect);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect], (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawInstancedIndirect);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Dispatch], (UINT_PTR)D3D11_ID3D11DeviceContext1_Dispatch);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DispatchIndirect], (UINT_PTR)D3D11_ID3D11DeviceContext1_DispatchIndirect);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetState], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetViewports], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetViewports);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetScissorRects], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetScissorRects);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion], (UINT_PTR)D3D11_ID3D11DeviceContext1_CopySubresourceRegion);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyResource], (UINT_PTR)D3D11_ID3D11DeviceContext1_CopyResource);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::UpdateSubresource], (UINT_PTR)D3D11_ID3D11DeviceContext1_UpdateSubresource);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyStructureCount], (UINT_PTR)D3D11_ID3D11DeviceContext1_CopyStructureCount);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearRenderTargetView);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewUint);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewFloat);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearDepthStencilView);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GenerateMips], (UINT_PTR)D3D11_ID3D11DeviceContext1_GenerateMips);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD], (UINT_PTR)D3D11_ID3D11DeviceContext1_SetResourceMinLOD);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD], (UINT_PTR)D3D11_ID3D11DeviceContext1_GetResourceMinLOD);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ResolveSubresource], (UINT_PTR)D3D11_ID3D11DeviceContext1_ResolveSubresource);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ExecuteCommandList], (UINT_PTR)D3D11_ID3D11DeviceContext1_ExecuteCommandList);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetUnorderedAccessViews);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetInputLayout], (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetInputLayout);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetVertexBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer], (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetIndexBuffer);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology], (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetPrimitiveTopology);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetPredication], (UINT_PTR)D3D11_ID3D11DeviceContext1_GetPredication);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetRenderTargets);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetBlendState], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetBlendState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState], (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetDepthStencilState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOGetTargets], (UINT_PTR)D3D11_ID3D11DeviceContext1_SOGetTargets);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetState], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetViewports], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetViewports);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetScissorRects], (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetScissorRects);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShaderResources], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetShaderResources);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetUnorderedAccessViews);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShader], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetShader);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetSamplers], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetSamplers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearState], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Flush], (UINT_PTR)D3D11_ID3D11DeviceContext1_Flush);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetType], (UINT_PTR)D3D11_ID3D11DeviceContext1_GetType);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetContextFlags], (UINT_PTR)D3D11_ID3D11DeviceContext1_GetContextFlags);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::FinishCommandList], (UINT_PTR)D3D11_ID3D11DeviceContext1_FinishCommandList);

	/*** ID3D11DeviceContext1 super methods ***/
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CopySubresourceRegion1], (UINT_PTR)D3D11_ID3D11DeviceContext1_CopySubresourceRegion1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::UpdateSubresource1], (UINT_PTR)D3D11_ID3D11DeviceContext1_UpdateSubresource1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardResource], (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardResource);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView], (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardView);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSSetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSGetConstantBuffers1], (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers1);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::SwapDeviceContextState], (UINT_PTR)D3D11_ID3D11DeviceContext1_SwapDeviceContextState);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::ClearView], (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearView);
	OverrideVTable(&D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView1], (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardView1);
}

/**
* Creates a Virtual Methods Table array of ID3D11DeviceContext1
***/
void Generate_D3D11_ID3D11DeviceContext1_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::QueryInterface] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_QueryInterface;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::AddRef] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_AddRef;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::Release] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Release;

	/*** ID3D11DeviceContext super methods ***/
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexed] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexed;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Draw] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Draw;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Map] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Map;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Unmap] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Unmap;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetInputLayout] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetInputLayout;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetVertexBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetIndexBuffer;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexedInstanced;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstanced] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawInstanced;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IASetPrimitiveTopology;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Begin] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Begin;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::End] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_End;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetData] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GetData;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetPredication] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_SetPredication;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetRenderTargets;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetBlendState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetBlendState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMSetDepthStencilState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOSetTargets] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_SOSetTargets;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawAuto] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawAuto;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawIndexedInstancedIndirect;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DrawInstancedIndirect;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Dispatch] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Dispatch;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DispatchIndirect] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DispatchIndirect;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetViewports] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetViewports;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetScissorRects] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSSetScissorRects;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CopySubresourceRegion;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyResource] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CopyResource;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::UpdateSubresource] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_UpdateSubresource;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyStructureCount] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CopyStructureCount;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearRenderTargetView;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewUint;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewFloat;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearDepthStencilView;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GenerateMips] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GenerateMips;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_SetResourceMinLOD;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GetResourceMinLOD;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ResolveSubresource] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ResolveSubresource;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ExecuteCommandList] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ExecuteCommandList;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetUnorderedAccessViews;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetInputLayout] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetInputLayout;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetVertexBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetIndexBuffer;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_IAGetPrimitiveTopology;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetPredication] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GetPredication;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetRenderTargets;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetBlendState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetBlendState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_OMGetDepthStencilState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOGetTargets] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_SOGetTargets;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetViewports] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetViewports;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetScissorRects] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_RSGetScissorRects;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShaderResources] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetShaderResources;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetUnorderedAccessViews;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShader] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetShader;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetSamplers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetSamplers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Flush] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_Flush;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetType] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GetType;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetContextFlags] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GetContextFlags;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::FinishCommandList] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_FinishCommandList;

	/*** ID3D11DeviceContext1 super methods ***/
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CopySubresourceRegion1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CopySubresourceRegion1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::UpdateSubresource1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_UpdateSubresource1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardResource] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardResource;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardView;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSSetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSGetConstantBuffers1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers1;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::SwapDeviceContextState] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_SwapDeviceContextState;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::ClearView] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_ClearView;
	anD3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView1] =  (UINT_PTR)D3D11_ID3D11DeviceContext1_DiscardView1;
}

/**
* Overrides the Virtual Methods Table of ID3D11DeviceContext1 using a generated array.
***/
void Override_D3D11_ID3D11DeviceContext1_VMTable_by_Array()
{
	OverrideFullVTable(&D3D11_ID3D11DeviceContext1_VMTable[0],
		(void*)&anD3D11_ID3D11DeviceContext1_VMTable[0],
		(D3D11_ID3D11DEVICECONTEXT1_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of ID3D11DeviceContext1
***/
void Detour_D3D11_ID3D11DeviceContext1VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::QueryInterface],  (PBYTE)D3D11_ID3D11DeviceContext1_QueryInterface, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::AddRef],  (PBYTE)D3D11_ID3D11DeviceContext1_AddRef, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_IUNKNOWN::Release],  (PBYTE)D3D11_ID3D11DeviceContext1_Release, JMP32_SZ));

	/*** ID3D11DeviceContext super methods ***/
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_PSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_PSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_PSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_VSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawIndexed,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexed],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawIndexed, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Draw,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Draw],  (PBYTE)D3D11_ID3D11DeviceContext1_Draw, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Map,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Map],  (PBYTE)D3D11_ID3D11DeviceContext1_Map, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Unmap,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Unmap],  (PBYTE)D3D11_ID3D11DeviceContext1_Unmap, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IASetInputLayout,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetInputLayout],  (PBYTE)D3D11_ID3D11DeviceContext1_IASetInputLayout, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_IASetVertexBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer],  (PBYTE)D3D11_ID3D11DeviceContext1_IASetIndexBuffer, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawIndexedInstanced, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawInstanced,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstanced],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawInstanced, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_GSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology],  (PBYTE)D3D11_ID3D11DeviceContext1_IASetPrimitiveTopology, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_VSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_VSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Begin,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Begin],  (PBYTE)D3D11_ID3D11DeviceContext1_Begin, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::End,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::End],  (PBYTE)D3D11_ID3D11DeviceContext1_End, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GetData,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetData],  (PBYTE)D3D11_ID3D11DeviceContext1_GetData, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::SetPredication,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetPredication],  (PBYTE)D3D11_ID3D11DeviceContext1_SetPredication, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_GSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_GSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets],  (PBYTE)D3D11_ID3D11DeviceContext1_OMSetRenderTargets, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews],  (PBYTE)D3D11_ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMSetBlendState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetBlendState],  (PBYTE)D3D11_ID3D11DeviceContext1_OMSetBlendState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState],  (PBYTE)D3D11_ID3D11DeviceContext1_OMSetDepthStencilState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::SOSetTargets,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOSetTargets],  (PBYTE)D3D11_ID3D11DeviceContext1_SOSetTargets, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawAuto,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawAuto],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawAuto, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawIndexedInstancedIndirect, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect],  (PBYTE)D3D11_ID3D11DeviceContext1_DrawInstancedIndirect, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Dispatch,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Dispatch],  (PBYTE)D3D11_ID3D11DeviceContext1_Dispatch, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DispatchIndirect,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DispatchIndirect],  (PBYTE)D3D11_ID3D11DeviceContext1_DispatchIndirect, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSSetState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetState],  (PBYTE)D3D11_ID3D11DeviceContext1_RSSetState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSSetViewports,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetViewports],  (PBYTE)D3D11_ID3D11DeviceContext1_RSSetViewports, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSSetScissorRects],  (PBYTE)D3D11_ID3D11DeviceContext1_RSSetScissorRects, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion],  (PBYTE)D3D11_ID3D11DeviceContext1_CopySubresourceRegion, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CopyResource,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyResource],  (PBYTE)D3D11_ID3D11DeviceContext1_CopyResource, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::UpdateSubresource,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::UpdateSubresource],  (PBYTE)D3D11_ID3D11DeviceContext1_UpdateSubresource, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CopyStructureCount,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CopyStructureCount],  (PBYTE)D3D11_ID3D11DeviceContext1_CopyStructureCount, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearRenderTargetView, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewUint, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearUnorderedAccessViewFloat, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearDepthStencilView, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GenerateMips,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GenerateMips],  (PBYTE)D3D11_ID3D11DeviceContext1_GenerateMips, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD],  (PBYTE)D3D11_ID3D11DeviceContext1_SetResourceMinLOD, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD],  (PBYTE)D3D11_ID3D11DeviceContext1_GetResourceMinLOD, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ResolveSubresource,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ResolveSubresource],  (PBYTE)D3D11_ID3D11DeviceContext1_ResolveSubresource, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ExecuteCommandList],  (PBYTE)D3D11_ID3D11DeviceContext1_ExecuteCommandList, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_HSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_HSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_HSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_DSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_DSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_DSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetUnorderedAccessViews, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSSetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSSetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_PSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_PSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_PSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_VSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IAGetInputLayout,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetInputLayout],  (PBYTE)D3D11_ID3D11DeviceContext1_IAGetInputLayout, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_IAGetVertexBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer],  (PBYTE)D3D11_ID3D11DeviceContext1_IAGetIndexBuffer, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_GSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology],  (PBYTE)D3D11_ID3D11DeviceContext1_IAGetPrimitiveTopology, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_VSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::VSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_VSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GetPredication,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetPredication],  (PBYTE)D3D11_ID3D11DeviceContext1_GetPredication, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_GSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_GSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets],  (PBYTE)D3D11_ID3D11DeviceContext1_OMGetRenderTargets, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews],  (PBYTE)D3D11_ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMGetBlendState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetBlendState],  (PBYTE)D3D11_ID3D11DeviceContext1_OMGetBlendState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState],  (PBYTE)D3D11_ID3D11DeviceContext1_OMGetDepthStencilState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::SOGetTargets,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::SOGetTargets],  (PBYTE)D3D11_ID3D11DeviceContext1_SOGetTargets, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSGetState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetState],  (PBYTE)D3D11_ID3D11DeviceContext1_RSGetState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSGetViewports,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetViewports],  (PBYTE)D3D11_ID3D11DeviceContext1_RSGetViewports, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::RSGetScissorRects],  (PBYTE)D3D11_ID3D11DeviceContext1_RSGetScissorRects, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_HSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_HSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_HSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_DSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_DSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_DSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShaderResources],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetShaderResources, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetUnorderedAccessViews, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSGetShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetShader],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetShader, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSGetSamplers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetSamplers],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetSamplers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::ClearState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::ClearState],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::Flush,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::Flush],  (PBYTE)D3D11_ID3D11DeviceContext1_Flush, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GetType,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetType],  (PBYTE)D3D11_ID3D11DeviceContext1_GetType, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::GetContextFlags,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::GetContextFlags],  (PBYTE)D3D11_ID3D11DeviceContext1_GetContextFlags, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::FinishCommandList,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT::FinishCommandList],  (PBYTE)D3D11_ID3D11DeviceContext1_FinishCommandList, JMP32_SZ));

	/*** ID3D11DeviceContext1 super methods ***/
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::CopySubresourceRegion1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CopySubresourceRegion1],  (PBYTE)D3D11_ID3D11DeviceContext1_CopySubresourceRegion1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::UpdateSubresource1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::UpdateSubresource1],  (PBYTE)D3D11_ID3D11DeviceContext1_UpdateSubresource1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::DiscardResource,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardResource],  (PBYTE)D3D11_ID3D11DeviceContext1_DiscardResource, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::DiscardView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView],  (PBYTE)D3D11_ID3D11DeviceContext1_DiscardView, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::VSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_VSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::HSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_HSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::DSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_DSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::GSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_GSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::PSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_PSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::CSSetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSSetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_CSSetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::VSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::VSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_VSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::HSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::HSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_HSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::DSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_DSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::GSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::GSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_GSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::PSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::PSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_PSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::CSGetConstantBuffers1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::CSGetConstantBuffers1],  (PBYTE)D3D11_ID3D11DeviceContext1_CSGetConstantBuffers1, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::SwapDeviceContextState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::SwapDeviceContextState],  (PBYTE)D3D11_ID3D11DeviceContext1_SwapDeviceContextState, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::ClearView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::ClearView],  (PBYTE)D3D11_ID3D11DeviceContext1_ClearView, JMP32_SZ));
	pDCL_ID3D11DeviceContext1->SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT1::DiscardView1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11DeviceContext1_VMTable[VMT_ID3D11DEVICECONTEXT1::DiscardView1],  (PBYTE)D3D11_ID3D11DeviceContext1_DiscardView1, JMP32_SZ));
}

#endif

