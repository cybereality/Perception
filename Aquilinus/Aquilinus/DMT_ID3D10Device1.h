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
#ifndef DMT_ID3D10DEVICE1_TABLE
#define DMT_ID3D10DEVICE1_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D10_ID3D10Device1_QueryInterface                       (ID3D10Device1* pcThis, REFIID riid, void** ppvObject) { return pDCL_ID3D10Device1->QueryInterface                       (pcThis, riid, ppvObject);}
ULONG WINAPI D3D10_ID3D10Device1_AddRef                               (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->AddRef                               (pcThis);}
ULONG WINAPI D3D10_ID3D10Device1_Release                              (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->Release                              (pcThis);}

/*** ID3D10Device methods ***/
void WINAPI D3D10_ID3D10Device1_VSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->VSSetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_PSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->PSSetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_PSSetShader                          (ID3D10Device1* pcThis, ID3D10PixelShader* pPixelShader) { return pDCL_ID3D10Device1->PSSetShader                          (pcThis, pPixelShader);}
void WINAPI D3D10_ID3D10Device1_PSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->PSSetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_VSSetShader                          (ID3D10Device1* pcThis, ID3D10VertexShader* pVertexShader) { return pDCL_ID3D10Device1->VSSetShader                          (pcThis, pVertexShader);}
void WINAPI D3D10_ID3D10Device1_DrawIndexed                          (ID3D10Device1* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) { return pDCL_ID3D10Device1->DrawIndexed                          (pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);}
void WINAPI D3D10_ID3D10Device1_Draw                                 (ID3D10Device1* pcThis, UINT VertexCount, UINT StartVertexLocation) { return pDCL_ID3D10Device1->Draw                                 (pcThis, VertexCount, StartVertexLocation);}
void WINAPI D3D10_ID3D10Device1_PSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->PSSetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_IASetInputLayout                     (ID3D10Device1* pcThis, ID3D10InputLayout* pInputLayout) { return pDCL_ID3D10Device1->IASetInputLayout                     (pcThis, pInputLayout);}
void WINAPI D3D10_ID3D10Device1_IASetVertexBuffers                   (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets) { return pDCL_ID3D10Device1->IASetVertexBuffers                   (pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);}
void WINAPI D3D10_ID3D10Device1_IASetIndexBuffer                     (ID3D10Device1* pcThis, ID3D10Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset) { return pDCL_ID3D10Device1->IASetIndexBuffer                     (pcThis, pIndexBuffer, Format, Offset);}
void WINAPI D3D10_ID3D10Device1_DrawIndexedInstanced                 (ID3D10Device1* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) { return pDCL_ID3D10Device1->DrawIndexedInstanced                 (pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);}
void WINAPI D3D10_ID3D10Device1_DrawInstanced                        (ID3D10Device1* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) { return pDCL_ID3D10Device1->DrawInstanced                        (pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);}
void WINAPI D3D10_ID3D10Device1_GSSetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->GSSetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_GSSetShader                          (ID3D10Device1* pcThis, ID3D10GeometryShader* pShader) { return pDCL_ID3D10Device1->GSSetShader                          (pcThis, pShader);}
void WINAPI D3D10_ID3D10Device1_IASetPrimitiveTopology               (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY Topology) { return pDCL_ID3D10Device1->IASetPrimitiveTopology               (pcThis, Topology);}
void WINAPI D3D10_ID3D10Device1_VSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->VSSetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_VSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->VSSetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_SetPredication                       (ID3D10Device1* pcThis, ID3D10Predicate* pPredicate, BOOL PredicateValue) { return pDCL_ID3D10Device1->SetPredication                       (pcThis, pPredicate, PredicateValue);}
void WINAPI D3D10_ID3D10Device1_GSSetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->GSSetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_GSSetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->GSSetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_OMSetRenderTargets                   (ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView* pDepthStencilView) { return pDCL_ID3D10Device1->OMSetRenderTargets                   (pcThis, NumViews, ppRenderTargetViews, pDepthStencilView);}
void WINAPI D3D10_ID3D10Device1_OMSetBlendState                      (ID3D10Device1* pcThis, ID3D10BlendState* pBlendState, FLOAT* BlendFactor, UINT SampleMask) { return pDCL_ID3D10Device1->OMSetBlendState                      (pcThis, pBlendState, BlendFactor, SampleMask);}
void WINAPI D3D10_ID3D10Device1_OMSetDepthStencilState               (ID3D10Device1* pcThis, ID3D10DepthStencilState* pDepthStencilState, UINT StencilRef) { return pDCL_ID3D10Device1->OMSetDepthStencilState               (pcThis, pDepthStencilState, StencilRef);}
void WINAPI D3D10_ID3D10Device1_SOSetTargets                         (ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets) { return pDCL_ID3D10Device1->SOSetTargets                         (pcThis, NumBuffers, ppSOTargets, pOffsets);}
void WINAPI D3D10_ID3D10Device1_DrawAuto                             (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->DrawAuto                             (pcThis);}
void WINAPI D3D10_ID3D10Device1_RSSetState                           (ID3D10Device1* pcThis, ID3D10RasterizerState* pRasterizerState) { return pDCL_ID3D10Device1->RSSetState                           (pcThis, pRasterizerState);}
void WINAPI D3D10_ID3D10Device1_RSSetViewports                       (ID3D10Device1* pcThis, UINT NumViewports, D3D10_VIEWPORT* pViewports) { return pDCL_ID3D10Device1->RSSetViewports                       (pcThis, NumViewports, pViewports);}
void WINAPI D3D10_ID3D10Device1_RSSetScissorRects                    (ID3D10Device1* pcThis, UINT NumRects, D3D10_RECT* pRects) { return pDCL_ID3D10Device1->RSSetScissorRects                    (pcThis, NumRects, pRects);}
void WINAPI D3D10_ID3D10Device1_CopySubresourceRegion                (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource* pSrcResource, UINT SrcSubresource, D3D10_BOX* pSrcBox) { return pDCL_ID3D10Device1->CopySubresourceRegion                (pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);}
void WINAPI D3D10_ID3D10Device1_CopyResource                         (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, ID3D10Resource* pSrcResource) { return pDCL_ID3D10Device1->CopyResource                         (pcThis, pDstResource, pSrcResource);}
void WINAPI D3D10_ID3D10Device1_UpdateSubresource                    (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, D3D10_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch) { return pDCL_ID3D10Device1->UpdateSubresource                    (pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);}
void WINAPI D3D10_ID3D10Device1_ClearRenderTargetView                (ID3D10Device1* pcThis, ID3D10RenderTargetView* pRenderTargetView, FLOAT* ColorRGBA) { return pDCL_ID3D10Device1->ClearRenderTargetView                (pcThis, pRenderTargetView, ColorRGBA);}
void WINAPI D3D10_ID3D10Device1_ClearDepthStencilView                (ID3D10Device1* pcThis, ID3D10DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil) { return pDCL_ID3D10Device1->ClearDepthStencilView                (pcThis, pDepthStencilView, ClearFlags, Depth, Stencil);}
void WINAPI D3D10_ID3D10Device1_GenerateMips                         (ID3D10Device1* pcThis, ID3D10ShaderResourceView* pShaderResourceView) { return pDCL_ID3D10Device1->GenerateMips                         (pcThis, pShaderResourceView);}
void WINAPI D3D10_ID3D10Device1_ResolveSubresource                   (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, ID3D10Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format) { return pDCL_ID3D10Device1->ResolveSubresource                   (pcThis, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);}
void WINAPI D3D10_ID3D10Device1_VSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->VSGetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_PSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->PSGetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_PSGetShader                          (ID3D10Device1* pcThis, ID3D10PixelShader** ppPixelShader) { return pDCL_ID3D10Device1->PSGetShader                          (pcThis, ppPixelShader);}
void WINAPI D3D10_ID3D10Device1_PSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->PSGetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_VSGetShader                          (ID3D10Device1* pcThis, ID3D10VertexShader** ppVertexShader) { return pDCL_ID3D10Device1->VSGetShader                          (pcThis, ppVertexShader);}
void WINAPI D3D10_ID3D10Device1_PSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->PSGetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_IAGetInputLayout                     (ID3D10Device1* pcThis, ID3D10InputLayout** ppInputLayout) { return pDCL_ID3D10Device1->IAGetInputLayout                     (pcThis, ppInputLayout);}
void WINAPI D3D10_ID3D10Device1_IAGetVertexBuffers                   (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets) { return pDCL_ID3D10Device1->IAGetVertexBuffers                   (pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);}
void WINAPI D3D10_ID3D10Device1_IAGetIndexBuffer                     (ID3D10Device1* pcThis, ID3D10Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset) { return pDCL_ID3D10Device1->IAGetIndexBuffer                     (pcThis, pIndexBuffer, Format, Offset);}
void WINAPI D3D10_ID3D10Device1_GSGetConstantBuffers                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers) { return pDCL_ID3D10Device1->GSGetConstantBuffers                 (pcThis, StartSlot, NumBuffers, ppConstantBuffers);}
void WINAPI D3D10_ID3D10Device1_GSGetShader                          (ID3D10Device1* pcThis, ID3D10GeometryShader** ppGeometryShader) { return pDCL_ID3D10Device1->GSGetShader                          (pcThis, ppGeometryShader);}
void WINAPI D3D10_ID3D10Device1_IAGetPrimitiveTopology               (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY* pTopology) { return pDCL_ID3D10Device1->IAGetPrimitiveTopology               (pcThis, pTopology);}
void WINAPI D3D10_ID3D10Device1_VSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->VSGetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_VSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->VSGetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_GetPredication                       (ID3D10Device1* pcThis, ID3D10Predicate** ppPredicate, BOOL* pPredicateValue) { return pDCL_ID3D10Device1->GetPredication                       (pcThis, ppPredicate, pPredicateValue);}
void WINAPI D3D10_ID3D10Device1_GSGetShaderResources                 (ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews) { return pDCL_ID3D10Device1->GSGetShaderResources                 (pcThis, StartSlot, NumViews, ppShaderResourceViews);}
void WINAPI D3D10_ID3D10Device1_GSGetSamplers                        (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers) { return pDCL_ID3D10Device1->GSGetSamplers                        (pcThis, StartSlot, NumSamplers, ppSamplers);}
void WINAPI D3D10_ID3D10Device1_OMGetRenderTargets                   (ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView** ppDepthStencilView) { return pDCL_ID3D10Device1->OMGetRenderTargets                   (pcThis, NumViews, ppRenderTargetViews, ppDepthStencilView);}
void WINAPI D3D10_ID3D10Device1_OMGetBlendState                      (ID3D10Device1* pcThis, ID3D10BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask) { return pDCL_ID3D10Device1->OMGetBlendState                      (pcThis, ppBlendState, BlendFactor, pSampleMask);}
void WINAPI D3D10_ID3D10Device1_OMGetDepthStencilState               (ID3D10Device1* pcThis, ID3D10DepthStencilState** ppDepthStencilState, UINT* pStencilRef) { return pDCL_ID3D10Device1->OMGetDepthStencilState               (pcThis, ppDepthStencilState, pStencilRef);}
void WINAPI D3D10_ID3D10Device1_SOGetTargets                         (ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets) { return pDCL_ID3D10Device1->SOGetTargets                         (pcThis, NumBuffers, ppSOTargets, pOffsets);}
void WINAPI D3D10_ID3D10Device1_RSGetState                           (ID3D10Device1* pcThis, ID3D10RasterizerState** ppRasterizerState) { return pDCL_ID3D10Device1->RSGetState                           (pcThis, ppRasterizerState);}
void WINAPI D3D10_ID3D10Device1_RSGetViewports                       (ID3D10Device1* pcThis, UINT* NumViewports, D3D10_VIEWPORT* pViewports) { return pDCL_ID3D10Device1->RSGetViewports                       (pcThis, NumViewports, pViewports);}
void WINAPI D3D10_ID3D10Device1_RSGetScissorRects                    (ID3D10Device1* pcThis, UINT* NumRects, D3D10_RECT* pRects) { return pDCL_ID3D10Device1->RSGetScissorRects                    (pcThis, NumRects, pRects);}
HRESULT WINAPI D3D10_ID3D10Device1_GetDeviceRemovedReason               (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->GetDeviceRemovedReason               (pcThis);}
HRESULT WINAPI D3D10_ID3D10Device1_SetExceptionMode                     (ID3D10Device1* pcThis, UINT RaiseFlags) { return pDCL_ID3D10Device1->SetExceptionMode                     (pcThis, RaiseFlags);}
UINT WINAPI D3D10_ID3D10Device1_GetExceptionMode                     (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->GetExceptionMode                     (pcThis);}
HRESULT WINAPI D3D10_ID3D10Device1_GetPrivateData                       (ID3D10Device1* pcThis, REFGUID guid, UINT* pDataSize, void* pData) { return pDCL_ID3D10Device1->GetPrivateData                       (pcThis, guid, pDataSize, pData);}
HRESULT WINAPI D3D10_ID3D10Device1_SetPrivateData                       (ID3D10Device1* pcThis, REFGUID guid, UINT DataSize, void* pData) { return pDCL_ID3D10Device1->SetPrivateData                       (pcThis, guid, DataSize, pData);}
HRESULT WINAPI D3D10_ID3D10Device1_SetPrivateDataInterface              (ID3D10Device1* pcThis, REFGUID guid, IUnknown* pData) { return pDCL_ID3D10Device1->SetPrivateDataInterface              (pcThis, guid, pData);}
void WINAPI D3D10_ID3D10Device1_ClearState                           (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->ClearState                           (pcThis);}
void WINAPI D3D10_ID3D10Device1_Flush                                (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->Flush                                (pcThis);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateBuffer                         (ID3D10Device1* pcThis, D3D10_BUFFER_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Buffer** ppBuffer) { return pDCL_ID3D10Device1->CreateBuffer                         (pcThis, pDesc, pInitialData, ppBuffer);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateTexture1D                      (ID3D10Device1* pcThis, D3D10_TEXTURE1D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture1D** ppTexture1D) { return pDCL_ID3D10Device1->CreateTexture1D                      (pcThis, pDesc, pInitialData, ppTexture1D);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateTexture2D                      (ID3D10Device1* pcThis, D3D10_TEXTURE2D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture2D** ppTexture2D) { return pDCL_ID3D10Device1->CreateTexture2D                      (pcThis, pDesc, pInitialData, ppTexture2D);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateTexture3D                      (ID3D10Device1* pcThis, D3D10_TEXTURE3D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture3D** ppTexture3D) { return pDCL_ID3D10Device1->CreateTexture3D                      (pcThis, pDesc, pInitialData, ppTexture3D);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateShaderResourceView             (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D10ShaderResourceView** ppSRView) { return pDCL_ID3D10Device1->CreateShaderResourceView             (pcThis, pResource, pDesc, ppSRView);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateRenderTargetView               (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_RENDER_TARGET_VIEW_DESC* pDesc, ID3D10RenderTargetView** ppRTView) { return pDCL_ID3D10Device1->CreateRenderTargetView               (pcThis, pResource, pDesc, ppRTView);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateDepthStencilView               (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D10DepthStencilView** ppDepthStencilView) { return pDCL_ID3D10Device1->CreateDepthStencilView               (pcThis, pResource, pDesc, ppDepthStencilView);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateInputLayout                    (ID3D10Device1* pcThis, D3D10_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout** ppInputLayout) { return pDCL_ID3D10Device1->CreateInputLayout                    (pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateVertexShader                   (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader** ppVertexShader) { return pDCL_ID3D10Device1->CreateVertexShader                   (pcThis, pShaderBytecode, BytecodeLength, ppVertexShader);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateGeometryShader                 (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader** ppGeometryShader) { return pDCL_ID3D10Device1->CreateGeometryShader                 (pcThis, pShaderBytecode, BytecodeLength, ppGeometryShader);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, D3D10_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader** ppGeometryShader) { return pDCL_ID3D10Device1->CreateGeometryShaderWithStreamOutput (pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader);}
HRESULT WINAPI D3D10_ID3D10Device1_CreatePixelShader                    (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader** ppPixelShader) { return pDCL_ID3D10Device1->CreatePixelShader                    (pcThis, pShaderBytecode, BytecodeLength, ppPixelShader);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateBlendState                     (ID3D10Device1* pcThis, D3D10_BLEND_DESC* pBlendStateDesc, ID3D10BlendState** ppBlendState) { return pDCL_ID3D10Device1->CreateBlendState                     (pcThis, pBlendStateDesc, ppBlendState);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateDepthStencilState              (ID3D10Device1* pcThis, D3D10_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D10DepthStencilState** ppDepthStencilState) { return pDCL_ID3D10Device1->CreateDepthStencilState              (pcThis, pDepthStencilDesc, ppDepthStencilState);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateRasterizerState                (ID3D10Device1* pcThis, D3D10_RASTERIZER_DESC* pRasterizerDesc, ID3D10RasterizerState** ppRasterizerState) { return pDCL_ID3D10Device1->CreateRasterizerState                (pcThis, pRasterizerDesc, ppRasterizerState);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateSamplerState                   (ID3D10Device1* pcThis, D3D10_SAMPLER_DESC* pSamplerDesc, ID3D10SamplerState** ppSamplerState) { return pDCL_ID3D10Device1->CreateSamplerState                   (pcThis, pSamplerDesc, ppSamplerState);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateQuery                          (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pQueryDesc, ID3D10Query** ppQuery) { return pDCL_ID3D10Device1->CreateQuery                          (pcThis, pQueryDesc, ppQuery);}
HRESULT WINAPI D3D10_ID3D10Device1_CreatePredicate                      (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pPredicateDesc, ID3D10Predicate** ppPredicate) { return pDCL_ID3D10Device1->CreatePredicate                      (pcThis, pPredicateDesc, ppPredicate);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateCounter                        (ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pCounterDesc, ID3D10Counter** ppCounter) { return pDCL_ID3D10Device1->CreateCounter                        (pcThis, pCounterDesc, ppCounter);}
HRESULT WINAPI D3D10_ID3D10Device1_CheckFormatSupport                   (ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport) { return pDCL_ID3D10Device1->CheckFormatSupport                   (pcThis, Format, pFormatSupport);}
HRESULT WINAPI D3D10_ID3D10Device1_CheckMultisampleQualityLevels        (ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels) { return pDCL_ID3D10Device1->CheckMultisampleQualityLevels        (pcThis, Format, SampleCount, pNumQualityLevels);}
void WINAPI D3D10_ID3D10Device1_CheckCounterInfo                     (ID3D10Device1* pcThis, D3D10_COUNTER_INFO* pCounterInfo) { return pDCL_ID3D10Device1->CheckCounterInfo                     (pcThis, pCounterInfo);}
HRESULT WINAPI D3D10_ID3D10Device1_CheckCounter                         (ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pDesc, D3D10_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength) { return pDCL_ID3D10Device1->CheckCounter                         (pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);}
UINT WINAPI D3D10_ID3D10Device1_GetCreationFlags                     (ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->GetCreationFlags                     (pcThis);}
HRESULT WINAPI D3D10_ID3D10Device1_OpenSharedResource                   (ID3D10Device1* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource) { return pDCL_ID3D10Device1->OpenSharedResource                   (pcThis, hResource, ReturnedInterface, ppResource);}
void WINAPI D3D10_ID3D10Device1_SetTextFilterSize                    (ID3D10Device1* pcThis, UINT Width, UINT Height) { return pDCL_ID3D10Device1->SetTextFilterSize                    (pcThis, Width, Height);}
void WINAPI D3D10_ID3D10Device1_GetTextFilterSize                    (ID3D10Device1* pcThis, UINT* pWidth, UINT* pHeight) { return pDCL_ID3D10Device1->GetTextFilterSize                    (pcThis, pWidth, pHeight);}

/*** ID3D10Device1 methods ***/
HRESULT WINAPI D3D10_ID3D10Device1_CreateShaderResourceView1(ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC1* pDesc, ID3D10ShaderResourceView1** ppSRView) { return pDCL_ID3D10Device1->CreateShaderResourceView1(pcThis, pResource, pDesc, ppSRView);}
HRESULT WINAPI D3D10_ID3D10Device1_CreateBlendState1(ID3D10Device1* pcThis, D3D10_BLEND_DESC1* pBlendStateDesc, ID3D10BlendState1** ppBlendState) { return pDCL_ID3D10Device1->CreateBlendState1(pcThis, pBlendStateDesc, ppBlendState);}
D3D10_FEATURE_LEVEL1 WINAPI D3D10_ID3D10Device1_GetFeatureLevel(ID3D10Device1* pcThis ) { return pDCL_ID3D10Device1->GetFeatureLevel(pcThis);}


/**
* Overrides the Virtual Methods Table of ID3D10Device1
***/
void Override_D3D10_ID3D10Device1_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::QueryInterface                       ], (UINT_PTR)D3D10_ID3D10Device1_QueryInterface                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::AddRef                               ], (UINT_PTR)D3D10_ID3D10Device1_AddRef                               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::Release                              ], (UINT_PTR)D3D10_ID3D10Device1_Release                              );

	/*** ID3D10Device super methods ***/
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_VSSetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_PSSetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_PSSetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_PSSetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_VSSetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexed                          ], (UINT_PTR)D3D10_ID3D10Device1_DrawIndexed                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Draw                                 ], (UINT_PTR)D3D10_ID3D10Device1_Draw                                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_PSSetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetInputLayout                     ], (UINT_PTR)D3D10_ID3D10Device1_IASetInputLayout                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers                   ], (UINT_PTR)D3D10_ID3D10Device1_IASetVertexBuffers                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer                     ], (UINT_PTR)D3D10_ID3D10Device1_IASetIndexBuffer                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced                 ], (UINT_PTR)D3D10_ID3D10Device1_DrawIndexedInstanced                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawInstanced                        ], (UINT_PTR)D3D10_ID3D10Device1_DrawInstanced                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_GSSetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_GSSetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology               ], (UINT_PTR)D3D10_ID3D10Device1_IASetPrimitiveTopology               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_VSSetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_VSSetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPredication                       ], (UINT_PTR)D3D10_ID3D10Device1_SetPredication                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_GSSetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_GSSetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets                   ], (UINT_PTR)D3D10_ID3D10Device1_OMSetRenderTargets                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetBlendState                      ], (UINT_PTR)D3D10_ID3D10Device1_OMSetBlendState                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState               ], (UINT_PTR)D3D10_ID3D10Device1_OMSetDepthStencilState               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOSetTargets                         ], (UINT_PTR)D3D10_ID3D10Device1_SOSetTargets                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawAuto                             ], (UINT_PTR)D3D10_ID3D10Device1_DrawAuto                             );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetState                           ], (UINT_PTR)D3D10_ID3D10Device1_RSSetState                           );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetViewports                       ], (UINT_PTR)D3D10_ID3D10Device1_RSSetViewports                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects                    ], (UINT_PTR)D3D10_ID3D10Device1_RSSetScissorRects                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion                ], (UINT_PTR)D3D10_ID3D10Device1_CopySubresourceRegion                );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopyResource                         ], (UINT_PTR)D3D10_ID3D10Device1_CopyResource                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::UpdateSubresource                    ], (UINT_PTR)D3D10_ID3D10Device1_UpdateSubresource                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView                ], (UINT_PTR)D3D10_ID3D10Device1_ClearRenderTargetView                );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView                ], (UINT_PTR)D3D10_ID3D10Device1_ClearDepthStencilView                );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GenerateMips                         ], (UINT_PTR)D3D10_ID3D10Device1_GenerateMips                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ResolveSubresource                   ], (UINT_PTR)D3D10_ID3D10Device1_ResolveSubresource                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_VSGetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_PSGetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_PSGetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_PSGetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_VSGetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_PSGetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout                     ], (UINT_PTR)D3D10_ID3D10Device1_IAGetInputLayout                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers                   ], (UINT_PTR)D3D10_ID3D10Device1_IAGetVertexBuffers                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer                     ], (UINT_PTR)D3D10_ID3D10Device1_IAGetIndexBuffer                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers                 ], (UINT_PTR)D3D10_ID3D10Device1_GSGetConstantBuffers                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShader                          ], (UINT_PTR)D3D10_ID3D10Device1_GSGetShader                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology               ], (UINT_PTR)D3D10_ID3D10Device1_IAGetPrimitiveTopology               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_VSGetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_VSGetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPredication                       ], (UINT_PTR)D3D10_ID3D10Device1_GetPredication                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources                 ], (UINT_PTR)D3D10_ID3D10Device1_GSGetShaderResources                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetSamplers                        ], (UINT_PTR)D3D10_ID3D10Device1_GSGetSamplers                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets                   ], (UINT_PTR)D3D10_ID3D10Device1_OMGetRenderTargets                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetBlendState                      ], (UINT_PTR)D3D10_ID3D10Device1_OMGetBlendState                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState               ], (UINT_PTR)D3D10_ID3D10Device1_OMGetDepthStencilState               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOGetTargets                         ], (UINT_PTR)D3D10_ID3D10Device1_SOGetTargets                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetState                           ], (UINT_PTR)D3D10_ID3D10Device1_RSGetState                           );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetViewports                       ], (UINT_PTR)D3D10_ID3D10Device1_RSGetViewports                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects                    ], (UINT_PTR)D3D10_ID3D10Device1_RSGetScissorRects                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason               ], (UINT_PTR)D3D10_ID3D10Device1_GetDeviceRemovedReason               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetExceptionMode                     ], (UINT_PTR)D3D10_ID3D10Device1_SetExceptionMode                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetExceptionMode                     ], (UINT_PTR)D3D10_ID3D10Device1_GetExceptionMode                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPrivateData                       ], (UINT_PTR)D3D10_ID3D10Device1_GetPrivateData                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateData                       ], (UINT_PTR)D3D10_ID3D10Device1_SetPrivateData                       );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface              ], (UINT_PTR)D3D10_ID3D10Device1_SetPrivateDataInterface              );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearState                           ], (UINT_PTR)D3D10_ID3D10Device1_ClearState                           );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Flush                                ], (UINT_PTR)D3D10_ID3D10Device1_Flush                                );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBuffer                         ], (UINT_PTR)D3D10_ID3D10Device1_CreateBuffer                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture1D                      ], (UINT_PTR)D3D10_ID3D10Device1_CreateTexture1D                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture2D                      ], (UINT_PTR)D3D10_ID3D10Device1_CreateTexture2D                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture3D                      ], (UINT_PTR)D3D10_ID3D10Device1_CreateTexture3D                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView             ], (UINT_PTR)D3D10_ID3D10Device1_CreateShaderResourceView             );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView               ], (UINT_PTR)D3D10_ID3D10Device1_CreateRenderTargetView               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView               ], (UINT_PTR)D3D10_ID3D10Device1_CreateDepthStencilView               );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateInputLayout                    ], (UINT_PTR)D3D10_ID3D10Device1_CreateInputLayout                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateVertexShader                   ], (UINT_PTR)D3D10_ID3D10Device1_CreateVertexShader                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader                 ], (UINT_PTR)D3D10_ID3D10Device1_CreateGeometryShader                 );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ], (UINT_PTR)D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePixelShader                    ], (UINT_PTR)D3D10_ID3D10Device1_CreatePixelShader                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBlendState                     ], (UINT_PTR)D3D10_ID3D10Device1_CreateBlendState                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState              ], (UINT_PTR)D3D10_ID3D10Device1_CreateDepthStencilState              );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState                ], (UINT_PTR)D3D10_ID3D10Device1_CreateRasterizerState                );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateSamplerState                   ], (UINT_PTR)D3D10_ID3D10Device1_CreateSamplerState                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateQuery                          ], (UINT_PTR)D3D10_ID3D10Device1_CreateQuery                          );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePredicate                      ], (UINT_PTR)D3D10_ID3D10Device1_CreatePredicate                      );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateCounter                        ], (UINT_PTR)D3D10_ID3D10Device1_CreateCounter                        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport                   ], (UINT_PTR)D3D10_ID3D10Device1_CheckFormatSupport                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels        ], (UINT_PTR)D3D10_ID3D10Device1_CheckMultisampleQualityLevels        );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo                     ], (UINT_PTR)D3D10_ID3D10Device1_CheckCounterInfo                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounter                         ], (UINT_PTR)D3D10_ID3D10Device1_CheckCounter                         );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetCreationFlags                     ], (UINT_PTR)D3D10_ID3D10Device1_GetCreationFlags                     );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OpenSharedResource                   ], (UINT_PTR)D3D10_ID3D10Device1_OpenSharedResource                   );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize                    ], (UINT_PTR)D3D10_ID3D10Device1_SetTextFilterSize                    );
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize                    ], (UINT_PTR)D3D10_ID3D10Device1_GetTextFilterSize                    );

	/*** ID3D10Device1 super methods ***/
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateShaderResourceView1], (UINT_PTR)D3D10_ID3D10Device1_CreateShaderResourceView1);
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateBlendState1], (UINT_PTR)D3D10_ID3D10Device1_CreateBlendState1);
	OverrideVTable(&D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::GetFeatureLevel], (UINT_PTR)D3D10_ID3D10Device1_GetFeatureLevel);
}

/**
* Creates a Virtual Methods Table array of ID3D10Device1
***/
void Generate_D3D10_ID3D10Device1_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::QueryInterface                       ] =  (UINT_PTR)D3D10_ID3D10Device1_QueryInterface                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::AddRef                               ] =  (UINT_PTR)D3D10_ID3D10Device1_AddRef                               ;
	anD3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::Release                              ] =  (UINT_PTR)D3D10_ID3D10Device1_Release                              ;

	/*** ID3D10Device super methods ***/
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_VSSetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_PSSetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_PSSetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_PSSetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_VSSetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexed                          ] =  (UINT_PTR)D3D10_ID3D10Device1_DrawIndexed                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Draw                                 ] =  (UINT_PTR)D3D10_ID3D10Device1_Draw                                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_PSSetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetInputLayout                     ] =  (UINT_PTR)D3D10_ID3D10Device1_IASetInputLayout                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers                   ] =  (UINT_PTR)D3D10_ID3D10Device1_IASetVertexBuffers                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer                     ] =  (UINT_PTR)D3D10_ID3D10Device1_IASetIndexBuffer                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced                 ] =  (UINT_PTR)D3D10_ID3D10Device1_DrawIndexedInstanced                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawInstanced                        ] =  (UINT_PTR)D3D10_ID3D10Device1_DrawInstanced                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_GSSetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_GSSetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology               ] =  (UINT_PTR)D3D10_ID3D10Device1_IASetPrimitiveTopology               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_VSSetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_VSSetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPredication                       ] =  (UINT_PTR)D3D10_ID3D10Device1_SetPredication                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_GSSetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_GSSetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets                   ] =  (UINT_PTR)D3D10_ID3D10Device1_OMSetRenderTargets                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetBlendState                      ] =  (UINT_PTR)D3D10_ID3D10Device1_OMSetBlendState                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState               ] =  (UINT_PTR)D3D10_ID3D10Device1_OMSetDepthStencilState               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOSetTargets                         ] =  (UINT_PTR)D3D10_ID3D10Device1_SOSetTargets                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawAuto                             ] =  (UINT_PTR)D3D10_ID3D10Device1_DrawAuto                             ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetState                           ] =  (UINT_PTR)D3D10_ID3D10Device1_RSSetState                           ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetViewports                       ] =  (UINT_PTR)D3D10_ID3D10Device1_RSSetViewports                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects                    ] =  (UINT_PTR)D3D10_ID3D10Device1_RSSetScissorRects                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion                ] =  (UINT_PTR)D3D10_ID3D10Device1_CopySubresourceRegion                ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopyResource                         ] =  (UINT_PTR)D3D10_ID3D10Device1_CopyResource                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::UpdateSubresource                    ] =  (UINT_PTR)D3D10_ID3D10Device1_UpdateSubresource                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView                ] =  (UINT_PTR)D3D10_ID3D10Device1_ClearRenderTargetView                ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView                ] =  (UINT_PTR)D3D10_ID3D10Device1_ClearDepthStencilView                ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GenerateMips                         ] =  (UINT_PTR)D3D10_ID3D10Device1_GenerateMips                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ResolveSubresource                   ] =  (UINT_PTR)D3D10_ID3D10Device1_ResolveSubresource                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_VSGetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_PSGetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_PSGetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_PSGetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_VSGetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_PSGetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout                     ] =  (UINT_PTR)D3D10_ID3D10Device1_IAGetInputLayout                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers                   ] =  (UINT_PTR)D3D10_ID3D10Device1_IAGetVertexBuffers                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer                     ] =  (UINT_PTR)D3D10_ID3D10Device1_IAGetIndexBuffer                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers                 ] =  (UINT_PTR)D3D10_ID3D10Device1_GSGetConstantBuffers                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShader                          ] =  (UINT_PTR)D3D10_ID3D10Device1_GSGetShader                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology               ] =  (UINT_PTR)D3D10_ID3D10Device1_IAGetPrimitiveTopology               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_VSGetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_VSGetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPredication                       ] =  (UINT_PTR)D3D10_ID3D10Device1_GetPredication                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources                 ] =  (UINT_PTR)D3D10_ID3D10Device1_GSGetShaderResources                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetSamplers                        ] =  (UINT_PTR)D3D10_ID3D10Device1_GSGetSamplers                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets                   ] =  (UINT_PTR)D3D10_ID3D10Device1_OMGetRenderTargets                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetBlendState                      ] =  (UINT_PTR)D3D10_ID3D10Device1_OMGetBlendState                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState               ] =  (UINT_PTR)D3D10_ID3D10Device1_OMGetDepthStencilState               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOGetTargets                         ] =  (UINT_PTR)D3D10_ID3D10Device1_SOGetTargets                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetState                           ] =  (UINT_PTR)D3D10_ID3D10Device1_RSGetState                           ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetViewports                       ] =  (UINT_PTR)D3D10_ID3D10Device1_RSGetViewports                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects                    ] =  (UINT_PTR)D3D10_ID3D10Device1_RSGetScissorRects                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason               ] =  (UINT_PTR)D3D10_ID3D10Device1_GetDeviceRemovedReason               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetExceptionMode                     ] =  (UINT_PTR)D3D10_ID3D10Device1_SetExceptionMode                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetExceptionMode                     ] =  (UINT_PTR)D3D10_ID3D10Device1_GetExceptionMode                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPrivateData                       ] =  (UINT_PTR)D3D10_ID3D10Device1_GetPrivateData                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateData                       ] =  (UINT_PTR)D3D10_ID3D10Device1_SetPrivateData                       ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface              ] =  (UINT_PTR)D3D10_ID3D10Device1_SetPrivateDataInterface              ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearState                           ] =  (UINT_PTR)D3D10_ID3D10Device1_ClearState                           ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Flush                                ] =  (UINT_PTR)D3D10_ID3D10Device1_Flush                                ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBuffer                         ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateBuffer                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture1D                      ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateTexture1D                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture2D                      ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateTexture2D                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture3D                      ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateTexture3D                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView             ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateShaderResourceView             ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView               ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateRenderTargetView               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView               ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateDepthStencilView               ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateInputLayout                    ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateInputLayout                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateVertexShader                   ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateVertexShader                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader                 ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateGeometryShader                 ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePixelShader                    ] =  (UINT_PTR)D3D10_ID3D10Device1_CreatePixelShader                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBlendState                     ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateBlendState                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState              ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateDepthStencilState              ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState                ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateRasterizerState                ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateSamplerState                   ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateSamplerState                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateQuery                          ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateQuery                          ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePredicate                      ] =  (UINT_PTR)D3D10_ID3D10Device1_CreatePredicate                      ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateCounter                        ] =  (UINT_PTR)D3D10_ID3D10Device1_CreateCounter                        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport                   ] =  (UINT_PTR)D3D10_ID3D10Device1_CheckFormatSupport                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels        ] =  (UINT_PTR)D3D10_ID3D10Device1_CheckMultisampleQualityLevels        ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo                     ] =  (UINT_PTR)D3D10_ID3D10Device1_CheckCounterInfo                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounter                         ] =  (UINT_PTR)D3D10_ID3D10Device1_CheckCounter                         ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetCreationFlags                     ] =  (UINT_PTR)D3D10_ID3D10Device1_GetCreationFlags                     ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OpenSharedResource                   ] =  (UINT_PTR)D3D10_ID3D10Device1_OpenSharedResource                   ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize                    ] =  (UINT_PTR)D3D10_ID3D10Device1_SetTextFilterSize                    ;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize                    ] =  (UINT_PTR)D3D10_ID3D10Device1_GetTextFilterSize                    ;

	/*** ID3D10Device1 super methods ***/
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateShaderResourceView1] =  (UINT_PTR)D3D10_ID3D10Device1_CreateShaderResourceView1;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateBlendState1] =  (UINT_PTR)D3D10_ID3D10Device1_CreateBlendState1;
	anD3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::GetFeatureLevel] =  (UINT_PTR)D3D10_ID3D10Device1_GetFeatureLevel;
}

/**
* Overrides the Virtual Methods Table of ID3D10Device1 using a generated array.
***/
void Override_D3D10_ID3D10Device1_VMTable_by_Array()
{
	OverrideFullVTable(&D3D10_ID3D10Device1_VMTable[0],
		(void*)&anD3D10_ID3D10Device1_VMTable[0],
		(D3D10_ID3D10DEVICE1_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of ID3D10Device1
***/
void Detour_D3D10_ID3D10Device1VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::QueryInterface                       ],  (PBYTE)D3D10_ID3D10Device1_QueryInterface                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef                               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::AddRef                               ],  (PBYTE)D3D10_ID3D10Device1_AddRef                               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::Release                              ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_IUNKNOWN::Release                              ],  (PBYTE)D3D10_ID3D10Device1_Release                              , JMP32_SZ));

	/*** ID3D10Device super methods ***/
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_VSSetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_PSSetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetShader                          ],  (PBYTE)D3D10_ID3D10Device1_PSSetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_PSSetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShader                          ],  (PBYTE)D3D10_ID3D10Device1_VSSetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawIndexed                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexed                          ],  (PBYTE)D3D10_ID3D10Device1_DrawIndexed                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::Draw                                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Draw                                 ],  (PBYTE)D3D10_ID3D10Device1_Draw                                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_PSSetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetInputLayout                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetInputLayout                     ],  (PBYTE)D3D10_ID3D10Device1_IASetInputLayout                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetVertexBuffers                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers                   ],  (PBYTE)D3D10_ID3D10Device1_IASetVertexBuffers                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetIndexBuffer                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer                     ],  (PBYTE)D3D10_ID3D10Device1_IASetIndexBuffer                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawIndexedInstanced                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced                 ],  (PBYTE)D3D10_ID3D10Device1_DrawIndexedInstanced                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawInstanced                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawInstanced                        ],  (PBYTE)D3D10_ID3D10Device1_DrawInstanced                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_GSSetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShader                          ],  (PBYTE)D3D10_ID3D10Device1_GSSetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetPrimitiveTopology               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology               ],  (PBYTE)D3D10_ID3D10Device1_IASetPrimitiveTopology               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_VSSetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSSetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_VSSetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPredication                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPredication                       ],  (PBYTE)D3D10_ID3D10Device1_SetPredication                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_GSSetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSSetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_GSSetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetRenderTargets                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets                   ],  (PBYTE)D3D10_ID3D10Device1_OMSetRenderTargets                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetBlendState                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetBlendState                      ],  (PBYTE)D3D10_ID3D10Device1_OMSetBlendState                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetDepthStencilState               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState               ],  (PBYTE)D3D10_ID3D10Device1_OMSetDepthStencilState               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SOSetTargets                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOSetTargets                         ],  (PBYTE)D3D10_ID3D10Device1_SOSetTargets                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawAuto                             ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::DrawAuto                             ],  (PBYTE)D3D10_ID3D10Device1_DrawAuto                             , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetState                           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetState                           ],  (PBYTE)D3D10_ID3D10Device1_RSSetState                           , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetViewports                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetViewports                       ],  (PBYTE)D3D10_ID3D10Device1_RSSetViewports                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetScissorRects                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects                    ],  (PBYTE)D3D10_ID3D10Device1_RSSetScissorRects                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CopySubresourceRegion                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion                ],  (PBYTE)D3D10_ID3D10Device1_CopySubresourceRegion                , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CopyResource                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CopyResource                         ],  (PBYTE)D3D10_ID3D10Device1_CopyResource                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::UpdateSubresource                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::UpdateSubresource                    ],  (PBYTE)D3D10_ID3D10Device1_UpdateSubresource                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearRenderTargetView                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView                ],  (PBYTE)D3D10_ID3D10Device1_ClearRenderTargetView                , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearDepthStencilView                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView                ],  (PBYTE)D3D10_ID3D10Device1_ClearDepthStencilView                , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GenerateMips                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GenerateMips                         ],  (PBYTE)D3D10_ID3D10Device1_GenerateMips                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ResolveSubresource                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ResolveSubresource                   ],  (PBYTE)D3D10_ID3D10Device1_ResolveSubresource                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_VSGetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_PSGetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetShader                          ],  (PBYTE)D3D10_ID3D10Device1_PSGetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_PSGetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShader                          ],  (PBYTE)D3D10_ID3D10Device1_VSGetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_PSGetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetInputLayout                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout                     ],  (PBYTE)D3D10_ID3D10Device1_IAGetInputLayout                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetVertexBuffers                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers                   ],  (PBYTE)D3D10_ID3D10Device1_IAGetVertexBuffers                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetIndexBuffer                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer                     ],  (PBYTE)D3D10_ID3D10Device1_IAGetIndexBuffer                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetConstantBuffers                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers                 ],  (PBYTE)D3D10_ID3D10Device1_GSGetConstantBuffers                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetShader                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShader                          ],  (PBYTE)D3D10_ID3D10Device1_GSGetShader                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetPrimitiveTopology               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology               ],  (PBYTE)D3D10_ID3D10Device1_IAGetPrimitiveTopology               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_VSGetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::VSGetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_VSGetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetPredication                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPredication                       ],  (PBYTE)D3D10_ID3D10Device1_GetPredication                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetShaderResources                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources                 ],  (PBYTE)D3D10_ID3D10Device1_GSGetShaderResources                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetSamplers                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GSGetSamplers                        ],  (PBYTE)D3D10_ID3D10Device1_GSGetSamplers                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetRenderTargets                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets                   ],  (PBYTE)D3D10_ID3D10Device1_OMGetRenderTargets                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetBlendState                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetBlendState                      ],  (PBYTE)D3D10_ID3D10Device1_OMGetBlendState                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetDepthStencilState               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState               ],  (PBYTE)D3D10_ID3D10Device1_OMGetDepthStencilState               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SOGetTargets                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SOGetTargets                         ],  (PBYTE)D3D10_ID3D10Device1_SOGetTargets                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetState                           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetState                           ],  (PBYTE)D3D10_ID3D10Device1_RSGetState                           , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetViewports                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetViewports                       ],  (PBYTE)D3D10_ID3D10Device1_RSGetViewports                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetScissorRects                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects                    ],  (PBYTE)D3D10_ID3D10Device1_RSGetScissorRects                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetDeviceRemovedReason               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason               ],  (PBYTE)D3D10_ID3D10Device1_GetDeviceRemovedReason               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetExceptionMode                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetExceptionMode                     ],  (PBYTE)D3D10_ID3D10Device1_SetExceptionMode                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetExceptionMode                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetExceptionMode                     ],  (PBYTE)D3D10_ID3D10Device1_GetExceptionMode                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetPrivateData                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetPrivateData                       ],  (PBYTE)D3D10_ID3D10Device1_GetPrivateData                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPrivateData                       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateData                       ],  (PBYTE)D3D10_ID3D10Device1_SetPrivateData                       , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPrivateDataInterface              ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface              ],  (PBYTE)D3D10_ID3D10Device1_SetPrivateDataInterface              , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearState                           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::ClearState                           ],  (PBYTE)D3D10_ID3D10Device1_ClearState                           , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::Flush                                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::Flush                                ],  (PBYTE)D3D10_ID3D10Device1_Flush                                , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateBuffer                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBuffer                         ],  (PBYTE)D3D10_ID3D10Device1_CreateBuffer                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture1D                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture1D                      ],  (PBYTE)D3D10_ID3D10Device1_CreateTexture1D                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture2D                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture2D                      ],  (PBYTE)D3D10_ID3D10Device1_CreateTexture2D                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture3D                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateTexture3D                      ],  (PBYTE)D3D10_ID3D10Device1_CreateTexture3D                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateShaderResourceView             ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView             ],  (PBYTE)D3D10_ID3D10Device1_CreateShaderResourceView             , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateRenderTargetView               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView               ],  (PBYTE)D3D10_ID3D10Device1_CreateRenderTargetView               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateDepthStencilView               ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView               ],  (PBYTE)D3D10_ID3D10Device1_CreateDepthStencilView               , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateInputLayout                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateInputLayout                    ],  (PBYTE)D3D10_ID3D10Device1_CreateInputLayout                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateVertexShader                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateVertexShader                   ],  (PBYTE)D3D10_ID3D10Device1_CreateVertexShader                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateGeometryShader                 ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader                 ],  (PBYTE)D3D10_ID3D10Device1_CreateGeometryShader                 , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ],  (PBYTE)D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreatePixelShader                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePixelShader                    ],  (PBYTE)D3D10_ID3D10Device1_CreatePixelShader                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateBlendState                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateBlendState                     ],  (PBYTE)D3D10_ID3D10Device1_CreateBlendState                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateDepthStencilState              ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState              ],  (PBYTE)D3D10_ID3D10Device1_CreateDepthStencilState              , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateRasterizerState                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState                ],  (PBYTE)D3D10_ID3D10Device1_CreateRasterizerState                , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateSamplerState                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateSamplerState                   ],  (PBYTE)D3D10_ID3D10Device1_CreateSamplerState                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateQuery                          ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateQuery                          ],  (PBYTE)D3D10_ID3D10Device1_CreateQuery                          , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreatePredicate                      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreatePredicate                      ],  (PBYTE)D3D10_ID3D10Device1_CreatePredicate                      , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateCounter                        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CreateCounter                        ],  (PBYTE)D3D10_ID3D10Device1_CreateCounter                        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckFormatSupport                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport                   ],  (PBYTE)D3D10_ID3D10Device1_CheckFormatSupport                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels        ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels        ],  (PBYTE)D3D10_ID3D10Device1_CheckMultisampleQualityLevels        , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckCounterInfo                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo                     ],  (PBYTE)D3D10_ID3D10Device1_CheckCounterInfo                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckCounter                         ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::CheckCounter                         ],  (PBYTE)D3D10_ID3D10Device1_CheckCounter                         , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetCreationFlags                     ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetCreationFlags                     ],  (PBYTE)D3D10_ID3D10Device1_GetCreationFlags                     , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OpenSharedResource                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::OpenSharedResource                   ],  (PBYTE)D3D10_ID3D10Device1_OpenSharedResource                   , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetTextFilterSize                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize                    ],  (PBYTE)D3D10_ID3D10Device1_SetTextFilterSize                    , JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetTextFilterSize                    ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize                    ],  (PBYTE)D3D10_ID3D10Device1_GetTextFilterSize                    , JMP32_SZ));

	/*** ID3D10Device1 super methods ***/
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE1::CreateShaderResourceView1,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateShaderResourceView1],  (PBYTE)D3D10_ID3D10Device1_CreateShaderResourceView1, JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE1::CreateBlendState1,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::CreateBlendState1],  (PBYTE)D3D10_ID3D10Device1_CreateBlendState1, JMP32_SZ));
	pDCL_ID3D10Device1->SetSuperFunctionPointer(VMT_ID3D10DEVICE1::GetFeatureLevel,  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device1_VMTable[VMT_ID3D10DEVICE1::GetFeatureLevel],  (PBYTE)D3D10_ID3D10Device1_GetFeatureLevel, JMP32_SZ));
}

#endif

