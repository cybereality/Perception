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

#ifndef DMT_ID3D10DEVICE_TABLE
#define DMT_ID3D10DEVICE_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D10_ID3D10Device_QueryInterface(ID3D10Device* pDevice, REFIID riid, void **ppvObject){ return pDCL_ID3D10Device->QueryInterface(pDevice, riid, ppvObject); }
ULONG   WINAPI D3D10_ID3D10Device_AddRef(ID3D10Device* pDevice){ return pDCL_ID3D10Device->AddRef(pDevice); }
ULONG   WINAPI D3D10_ID3D10Device_Release(ID3D10Device* pDevice){ return pDCL_ID3D10Device->Release(pDevice); }

/*** ID3D10Device methods ***/
void    WINAPI D3D10_ID3D10Device_VSSetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers){ pDCL_ID3D10Device->VSSetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_PSSetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews){ pDCL_ID3D10Device->PSSetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_PSSetShader(ID3D10Device* pDevice, ID3D10PixelShader *pPixelShader){ pDCL_ID3D10Device->PSSetShader(pDevice, pPixelShader); }
void    WINAPI D3D10_ID3D10Device_PSSetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers){ pDCL_ID3D10Device->PSSetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_VSSetShader(ID3D10Device* pDevice, ID3D10VertexShader *pVertexShader){ pDCL_ID3D10Device->VSSetShader(pDevice, pVertexShader); }
void    WINAPI D3D10_ID3D10Device_DrawIndexed(ID3D10Device* pDevice, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation){ pDCL_ID3D10Device->DrawIndexed(pDevice, IndexCount, StartIndexLocation, BaseVertexLocation); }
void    WINAPI D3D10_ID3D10Device_Draw(ID3D10Device* pDevice, UINT VertexCount, UINT StartVertexLocation){ pDCL_ID3D10Device->Draw(pDevice, VertexCount, StartVertexLocation); }
void    WINAPI D3D10_ID3D10Device_PSSetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers){ pDCL_ID3D10Device->PSSetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_IASetInputLayout(ID3D10Device* pDevice, ID3D10InputLayout *pInputLayout){ pDCL_ID3D10Device->IASetInputLayout(pDevice, pInputLayout); }
void    WINAPI D3D10_ID3D10Device_IASetVertexBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets){ pDCL_ID3D10Device->IASetVertexBuffers(pDevice, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
void    WINAPI D3D10_ID3D10Device_IASetIndexBuffer(ID3D10Device* pDevice, ID3D10Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset){ pDCL_ID3D10Device->IASetIndexBuffer(pDevice, pIndexBuffer, Format, Offset); }
void    WINAPI D3D10_ID3D10Device_DrawIndexedInstanced(ID3D10Device* pDevice, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation){ pDCL_ID3D10Device->DrawIndexedInstanced(pDevice, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
void    WINAPI D3D10_ID3D10Device_DrawInstanced(ID3D10Device* pDevice, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation){ pDCL_ID3D10Device->DrawInstanced(pDevice, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
void    WINAPI D3D10_ID3D10Device_GSSetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers){ pDCL_ID3D10Device->GSSetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_GSSetShader(ID3D10Device* pDevice, ID3D10GeometryShader *pShader){ pDCL_ID3D10Device->GSSetShader(pDevice, pShader); }
void    WINAPI D3D10_ID3D10Device_IASetPrimitiveTopology(ID3D10Device* pDevice, D3D10_PRIMITIVE_TOPOLOGY Topology){ pDCL_ID3D10Device->IASetPrimitiveTopology(pDevice, Topology); }
void    WINAPI D3D10_ID3D10Device_VSSetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews){ pDCL_ID3D10Device->VSSetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_VSSetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers){ pDCL_ID3D10Device->VSSetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_SetPredication(ID3D10Device* pDevice, ID3D10Predicate *pPredicate, BOOL PredicateValue){ pDCL_ID3D10Device->SetPredication(pDevice, pPredicate, PredicateValue); }
void    WINAPI D3D10_ID3D10Device_GSSetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews){ pDCL_ID3D10Device->GSSetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_GSSetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers){ pDCL_ID3D10Device->GSSetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_OMSetRenderTargets(ID3D10Device* pDevice, UINT NumViews, ID3D10RenderTargetView *const *ppRenderTargetViews, ID3D10DepthStencilView *pDepthStencilView){ pDCL_ID3D10Device->OMSetRenderTargets(pDevice, NumViews, ppRenderTargetViews, pDepthStencilView); }
void    WINAPI D3D10_ID3D10Device_OMSetBlendState(ID3D10Device* pDevice, ID3D10BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask){ pDCL_ID3D10Device->OMSetBlendState(pDevice, pBlendState, BlendFactor, SampleMask); }
void    WINAPI D3D10_ID3D10Device_OMSetDepthStencilState(ID3D10Device* pDevice, ID3D10DepthStencilState *pDepthStencilState, UINT StencilRef){ pDCL_ID3D10Device->OMSetDepthStencilState(pDevice, pDepthStencilState, StencilRef); }
void    WINAPI D3D10_ID3D10Device_SOSetTargets(ID3D10Device* pDevice, UINT NumBuffers, ID3D10Buffer *const *ppSOTargets, const UINT *pOffsets){ pDCL_ID3D10Device->SOSetTargets(pDevice, NumBuffers, ppSOTargets, pOffsets); }
void    WINAPI D3D10_ID3D10Device_DrawAuto(ID3D10Device* pDevice){ pDCL_ID3D10Device->DrawAuto(pDevice); }
void    WINAPI D3D10_ID3D10Device_RSSetState(ID3D10Device* pDevice, ID3D10RasterizerState *pRasterizerState){ pDCL_ID3D10Device->RSSetState(pDevice, pRasterizerState); }
void    WINAPI D3D10_ID3D10Device_RSSetViewports(ID3D10Device* pDevice, UINT NumViewports, const D3D10_VIEWPORT *pViewports){ pDCL_ID3D10Device->RSSetViewports(pDevice, NumViewports, pViewports); }
void    WINAPI D3D10_ID3D10Device_RSSetScissorRects(ID3D10Device* pDevice, UINT NumRects, const D3D10_RECT *pRects){ pDCL_ID3D10Device->RSSetScissorRects(pDevice, NumRects, pRects); }
void    WINAPI D3D10_ID3D10Device_CopySubresourceRegion(ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox){ pDCL_ID3D10Device->CopySubresourceRegion(pDevice, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
void    WINAPI D3D10_ID3D10Device_CopyResource(ID3D10Device* pDevice, ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource){ pDCL_ID3D10Device->CopyResource(pDevice, pDstResource, pSrcResource); }
void    WINAPI D3D10_ID3D10Device_UpdateSubresource(ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch){ pDCL_ID3D10Device->UpdateSubresource(pDevice, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
void    WINAPI D3D10_ID3D10Device_ClearRenderTargetView(ID3D10Device* pDevice, ID3D10RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ]){ pDCL_ID3D10Device->ClearRenderTargetView(pDevice, pRenderTargetView, ColorRGBA); }
void    WINAPI D3D10_ID3D10Device_ClearDepthStencilView(ID3D10Device* pDevice, ID3D10DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil){ pDCL_ID3D10Device->ClearDepthStencilView(pDevice, pDepthStencilView, ClearFlags, Depth, Stencil); }
void    WINAPI D3D10_ID3D10Device_GenerateMips(ID3D10Device* pDevice, ID3D10ShaderResourceView *pShaderResourceView){ pDCL_ID3D10Device->GenerateMips(pDevice, pShaderResourceView); }
void    WINAPI D3D10_ID3D10Device_ResolveSubresource(ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, ID3D10Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format){ pDCL_ID3D10Device->ResolveSubresource(pDevice, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
void    WINAPI D3D10_ID3D10Device_VSGetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers){ pDCL_ID3D10Device->VSGetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_PSGetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews){ pDCL_ID3D10Device->PSGetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_PSGetShader(ID3D10Device* pDevice, ID3D10PixelShader **ppPixelShader){ pDCL_ID3D10Device->PSGetShader(pDevice, ppPixelShader); }
void    WINAPI D3D10_ID3D10Device_PSGetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers){ pDCL_ID3D10Device->PSGetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_VSGetShader(ID3D10Device* pDevice, ID3D10VertexShader **ppVertexShader){ pDCL_ID3D10Device->VSGetShader(pDevice, ppVertexShader); }
void    WINAPI D3D10_ID3D10Device_PSGetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers){ pDCL_ID3D10Device->PSGetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_IAGetInputLayout(ID3D10Device* pDevice, ID3D10InputLayout **ppInputLayout){ pDCL_ID3D10Device->IAGetInputLayout(pDevice, ppInputLayout); }
void    WINAPI D3D10_ID3D10Device_IAGetVertexBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets){ pDCL_ID3D10Device->IAGetVertexBuffers(pDevice, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
void    WINAPI D3D10_ID3D10Device_IAGetIndexBuffer(ID3D10Device* pDevice, ID3D10Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset){ pDCL_ID3D10Device->IAGetIndexBuffer(pDevice, pIndexBuffer, Format, Offset); }
void    WINAPI D3D10_ID3D10Device_GSGetConstantBuffers(ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers){ pDCL_ID3D10Device->GSGetConstantBuffers(pDevice, StartSlot, NumBuffers, ppConstantBuffers); }
void    WINAPI D3D10_ID3D10Device_GSGetShader(ID3D10Device* pDevice, ID3D10GeometryShader **ppGeometryShader){ pDCL_ID3D10Device->GSGetShader(pDevice, ppGeometryShader); }
void    WINAPI D3D10_ID3D10Device_IAGetPrimitiveTopology(ID3D10Device* pDevice, D3D10_PRIMITIVE_TOPOLOGY *pTopology){ pDCL_ID3D10Device->IAGetPrimitiveTopology(pDevice, pTopology); }
void    WINAPI D3D10_ID3D10Device_VSGetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews){ pDCL_ID3D10Device->VSGetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_VSGetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers){ pDCL_ID3D10Device->VSGetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_GetPredication(ID3D10Device* pDevice, ID3D10Predicate **ppPredicate, BOOL *pPredicateValue){ pDCL_ID3D10Device->GetPredication(pDevice, ppPredicate, pPredicateValue); }
void    WINAPI D3D10_ID3D10Device_GSGetShaderResources(ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews){ pDCL_ID3D10Device->GSGetShaderResources(pDevice, StartSlot, NumViews, ppShaderResourceViews); }
void    WINAPI D3D10_ID3D10Device_GSGetSamplers(ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers){ pDCL_ID3D10Device->GSGetSamplers(pDevice, StartSlot, NumSamplers, ppSamplers); }
void    WINAPI D3D10_ID3D10Device_OMGetRenderTargets(ID3D10Device* pDevice, UINT NumViews, ID3D10RenderTargetView **ppRenderTargetViews, ID3D10DepthStencilView **ppDepthStencilView){ pDCL_ID3D10Device->OMGetRenderTargets(pDevice, NumViews, ppRenderTargetViews, ppDepthStencilView); }
void    WINAPI D3D10_ID3D10Device_OMGetBlendState(ID3D10Device* pDevice, ID3D10BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask){ pDCL_ID3D10Device->OMGetBlendState(pDevice, ppBlendState, BlendFactor, pSampleMask); }
void    WINAPI D3D10_ID3D10Device_OMGetDepthStencilState(ID3D10Device* pDevice, ID3D10DepthStencilState **ppDepthStencilState, UINT *pStencilRef){ pDCL_ID3D10Device->OMGetDepthStencilState(pDevice, ppDepthStencilState, pStencilRef); }
void    WINAPI D3D10_ID3D10Device_SOGetTargets(ID3D10Device* pDevice, UINT NumBuffers, ID3D10Buffer **ppSOTargets, UINT *pOffsets){ pDCL_ID3D10Device->SOGetTargets(pDevice, NumBuffers, ppSOTargets, pOffsets); }
void    WINAPI D3D10_ID3D10Device_RSGetState(ID3D10Device* pDevice, ID3D10RasterizerState **ppRasterizerState){ pDCL_ID3D10Device->RSGetState(pDevice, ppRasterizerState); }
void    WINAPI D3D10_ID3D10Device_RSGetViewports(ID3D10Device* pDevice, UINT *NumViewports, D3D10_VIEWPORT *pViewports){ pDCL_ID3D10Device->RSGetViewports(pDevice, NumViewports, pViewports); }
void    WINAPI D3D10_ID3D10Device_RSGetScissorRects(ID3D10Device* pDevice, UINT *NumRects, D3D10_RECT *pRects){ pDCL_ID3D10Device->RSGetScissorRects(pDevice, NumRects, pRects); }
HRESULT WINAPI D3D10_ID3D10Device_GetDeviceRemovedReason(ID3D10Device* pDevice){ return pDCL_ID3D10Device->GetDeviceRemovedReason(pDevice); }
HRESULT WINAPI D3D10_ID3D10Device_SetExceptionMode(ID3D10Device* pDevice, UINT RaiseFlags){ return pDCL_ID3D10Device->SetExceptionMode(pDevice, RaiseFlags); }
UINT    WINAPI D3D10_ID3D10Device_GetExceptionMode(ID3D10Device* pDevice){ return pDCL_ID3D10Device->GetExceptionMode(pDevice); }
HRESULT WINAPI D3D10_ID3D10Device_GetPrivateData(ID3D10Device* pDevice, REFGUID guid, UINT *pDataSize, void *pData){ return pDCL_ID3D10Device->GetPrivateData(pDevice, guid, pDataSize, pData); }
HRESULT WINAPI D3D10_ID3D10Device_SetPrivateData(ID3D10Device* pDevice, REFGUID guid, UINT DataSize, const void *pData){ return pDCL_ID3D10Device->SetPrivateData(pDevice, guid, DataSize, pData); }
HRESULT WINAPI D3D10_ID3D10Device_SetPrivateDataInterface(ID3D10Device* pDevice, REFGUID guid, const IUnknown *pData){	return pDCL_ID3D10Device->SetPrivateDataInterface(pDevice, guid, pData); }
void    WINAPI D3D10_ID3D10Device_ClearState(ID3D10Device* pDevice){ pDCL_ID3D10Device->ClearState(pDevice); }
void    WINAPI D3D10_ID3D10Device_Flush(ID3D10Device* pDevice){ pDCL_ID3D10Device->Flush(pDevice); }
HRESULT WINAPI D3D10_ID3D10Device_CreateBuffer(ID3D10Device* pDevice, const D3D10_BUFFER_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer){ return pDCL_ID3D10Device->CreateBuffer(pDevice, pDesc, pInitialData, ppBuffer); }
HRESULT WINAPI D3D10_ID3D10Device_CreateTexture1D(ID3D10Device* pDevice, const D3D10_TEXTURE1D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture1D **ppTexture1D){ return pDCL_ID3D10Device->CreateTexture1D(pDevice, pDesc, pInitialData, ppTexture1D); }
HRESULT WINAPI D3D10_ID3D10Device_CreateTexture2D(ID3D10Device* pDevice, const D3D10_TEXTURE2D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture2D **ppTexture2D){ return pDCL_ID3D10Device->CreateTexture2D(pDevice, pDesc, pInitialData, ppTexture2D); }
HRESULT WINAPI D3D10_ID3D10Device_CreateTexture3D(ID3D10Device* pDevice, const D3D10_TEXTURE3D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture3D **ppTexture3D){ return pDCL_ID3D10Device->CreateTexture3D(pDevice, pDesc, pInitialData, ppTexture3D); }
HRESULT WINAPI D3D10_ID3D10Device_CreateShaderResourceView(ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D10ShaderResourceView **ppSRView){ return pDCL_ID3D10Device->CreateShaderResourceView(pDevice, pResource, pDesc, ppSRView); }
HRESULT WINAPI D3D10_ID3D10Device_CreateRenderTargetView(ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_RENDER_TARGET_VIEW_DESC *pDesc, ID3D10RenderTargetView **ppRTView){	return pDCL_ID3D10Device->CreateRenderTargetView(pDevice, pResource, pDesc, ppRTView); }
HRESULT WINAPI D3D10_ID3D10Device_CreateDepthStencilView(ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D10DepthStencilView **ppDepthStencilView){ return pDCL_ID3D10Device->CreateDepthStencilView(pDevice, pResource, pDesc, ppDepthStencilView); }
HRESULT WINAPI D3D10_ID3D10Device_CreateInputLayout(ID3D10Device* pDevice, const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout **ppInputLayout){	return pDCL_ID3D10Device->CreateInputLayout(pDevice, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
HRESULT WINAPI D3D10_ID3D10Device_CreateVertexShader(ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader){ return pDCL_ID3D10Device->CreateVertexShader(pDevice, pShaderBytecode, BytecodeLength, ppVertexShader); }
HRESULT WINAPI D3D10_ID3D10Device_CreateGeometryShader(ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader **ppGeometryShader){ return pDCL_ID3D10Device->CreateGeometryShader(pDevice, pShaderBytecode, BytecodeLength, ppGeometryShader); }
HRESULT WINAPI D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput(ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D10_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader **ppGeometryShader){ return pDCL_ID3D10Device->CreateGeometryShaderWithStreamOutput(pDevice, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader); }
HRESULT WINAPI D3D10_ID3D10Device_CreatePixelShader(ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader){	return pDCL_ID3D10Device->CreatePixelShader(pDevice, pShaderBytecode, BytecodeLength, ppPixelShader); }
HRESULT WINAPI D3D10_ID3D10Device_CreateBlendState(ID3D10Device* pDevice, const D3D10_BLEND_DESC *pBlendStateDesc, ID3D10BlendState **ppBlendState){ return pDCL_ID3D10Device->CreateBlendState(pDevice, pBlendStateDesc, ppBlendState); }
HRESULT WINAPI D3D10_ID3D10Device_CreateDepthStencilState(ID3D10Device* pDevice, const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D10DepthStencilState **ppDepthStencilState){ return pDCL_ID3D10Device->CreateDepthStencilState(pDevice, pDepthStencilDesc, ppDepthStencilState); }
HRESULT WINAPI D3D10_ID3D10Device_CreateRasterizerState(ID3D10Device* pDevice, const D3D10_RASTERIZER_DESC *pRasterizerDesc, ID3D10RasterizerState **ppRasterizerState){ return pDCL_ID3D10Device->CreateRasterizerState(pDevice, pRasterizerDesc, ppRasterizerState); }
HRESULT WINAPI D3D10_ID3D10Device_CreateSamplerState(ID3D10Device* pDevice, const D3D10_SAMPLER_DESC *pSamplerDesc, ID3D10SamplerState **ppSamplerState){ return pDCL_ID3D10Device->CreateSamplerState(pDevice, pSamplerDesc, ppSamplerState); }
HRESULT WINAPI D3D10_ID3D10Device_CreateQuery(ID3D10Device* pDevice, const D3D10_QUERY_DESC *pQueryDesc, ID3D10Query **ppQuery){ return pDCL_ID3D10Device->CreateQuery(pDevice, pQueryDesc, ppQuery); }
HRESULT WINAPI D3D10_ID3D10Device_CreatePredicate(ID3D10Device* pDevice, const D3D10_QUERY_DESC *pPredicateDesc, ID3D10Predicate **ppPredicate){ return pDCL_ID3D10Device->CreatePredicate(pDevice, pPredicateDesc, ppPredicate); }
HRESULT WINAPI D3D10_ID3D10Device_CreateCounter(ID3D10Device* pDevice, const D3D10_COUNTER_DESC *pCounterDesc, ID3D10Counter **ppCounter){	return pDCL_ID3D10Device->CreateCounter(pDevice, pCounterDesc, ppCounter); }
HRESULT WINAPI D3D10_ID3D10Device_CheckFormatSupport(ID3D10Device* pDevice, DXGI_FORMAT Format, UINT *pFormatSupport){	return pDCL_ID3D10Device->CheckFormatSupport(pDevice, Format, pFormatSupport); }
HRESULT WINAPI D3D10_ID3D10Device_CheckMultisampleQualityLevels(ID3D10Device* pDevice, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels){	return pDCL_ID3D10Device->CheckMultisampleQualityLevels(pDevice, Format, SampleCount, pNumQualityLevels); }
void    WINAPI D3D10_ID3D10Device_CheckCounterInfo(ID3D10Device* pDevice, D3D10_COUNTER_INFO *pCounterInfo){ pDCL_ID3D10Device->CheckCounterInfo(pDevice, pCounterInfo); }
HRESULT WINAPI D3D10_ID3D10Device_CheckCounter(ID3D10Device* pDevice, const D3D10_COUNTER_DESC *pDesc, D3D10_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength){ return pDCL_ID3D10Device->CheckCounter(pDevice, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
UINT    WINAPI D3D10_ID3D10Device_GetCreationFlags(ID3D10Device* pDevice){ return pDCL_ID3D10Device->GetCreationFlags(pDevice); }
HRESULT WINAPI D3D10_ID3D10Device_OpenSharedResource(ID3D10Device* pDevice, HANDLE hResource, REFIID ReturnedInterface, void **ppResource){ return pDCL_ID3D10Device->OpenSharedResource(pDevice, hResource, ReturnedInterface, ppResource); }
void    WINAPI D3D10_ID3D10Device_SetTextFilterSize(ID3D10Device* pDevice, UINT Width, UINT Height){ pDCL_ID3D10Device->SetTextFilterSize(pDevice, Width, Height); }
void    WINAPI D3D10_ID3D10Device_GetTextFilterSize(ID3D10Device* pDevice, UINT *pWidth, UINT *pHeight){ pDCL_ID3D10Device->GetTextFilterSize(pDevice, pWidth, pHeight); }

/**
* Overrides the Virtual Methods Table of the DirectX 10 DXGISwapChain
***/
void Override_D3D10_ID3D10Device_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::QueryInterface],                          (UINT_PTR)D3D10_ID3D10Device_QueryInterface);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::AddRef],                                  (UINT_PTR)D3D10_ID3D10Device_AddRef);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::Release],                                 (UINT_PTR)D3D10_ID3D10Device_Release);

	/*** ID3D10Device super methods ***/
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_VSSetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_PSSetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShader],                         (UINT_PTR)D3D10_ID3D10Device_PSSetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_PSSetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShader],                         (UINT_PTR)D3D10_ID3D10Device_VSSetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexed],                         (UINT_PTR)D3D10_ID3D10Device_DrawIndexed);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Draw],                                (UINT_PTR)D3D10_ID3D10Device_Draw);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_PSSetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetInputLayout],                    (UINT_PTR)D3D10_ID3D10Device_IASetInputLayout);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers],                  (UINT_PTR)D3D10_ID3D10Device_IASetVertexBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer],                    (UINT_PTR)D3D10_ID3D10Device_IASetIndexBuffer);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced],                (UINT_PTR)D3D10_ID3D10Device_DrawIndexedInstanced);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawInstanced],                       (UINT_PTR)D3D10_ID3D10Device_DrawInstanced);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_GSSetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShader],                         (UINT_PTR)D3D10_ID3D10Device_GSSetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology],              (UINT_PTR)D3D10_ID3D10Device_IASetPrimitiveTopology);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_VSSetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_VSSetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPredication],                      (UINT_PTR)D3D10_ID3D10Device_SetPredication);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_GSSetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_GSSetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets],                  (UINT_PTR)D3D10_ID3D10Device_OMSetRenderTargets);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetBlendState],                     (UINT_PTR)D3D10_ID3D10Device_OMSetBlendState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState],              (UINT_PTR)D3D10_ID3D10Device_OMSetDepthStencilState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOSetTargets],                        (UINT_PTR)D3D10_ID3D10Device_SOSetTargets);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawAuto],                            (UINT_PTR)D3D10_ID3D10Device_DrawAuto);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetState],                          (UINT_PTR)D3D10_ID3D10Device_RSSetState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetViewports],                      (UINT_PTR)D3D10_ID3D10Device_RSSetViewports);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects],                   (UINT_PTR)D3D10_ID3D10Device_RSSetScissorRects);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion],               (UINT_PTR)D3D10_ID3D10Device_CopySubresourceRegion);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopyResource],                        (UINT_PTR)D3D10_ID3D10Device_CopyResource );
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::UpdateSubresource],                   (UINT_PTR)D3D10_ID3D10Device_UpdateSubresource);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView],               (UINT_PTR)D3D10_ID3D10Device_ClearRenderTargetView);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView],               (UINT_PTR)D3D10_ID3D10Device_ClearDepthStencilView);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GenerateMips],                        (UINT_PTR)D3D10_ID3D10Device_GenerateMips );
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ResolveSubresource],                  (UINT_PTR)D3D10_ID3D10Device_ResolveSubresource);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_VSGetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_PSGetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShader],                         (UINT_PTR)D3D10_ID3D10Device_PSGetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_PSGetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShader],                         (UINT_PTR)D3D10_ID3D10Device_VSGetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_PSGetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout],                    (UINT_PTR)D3D10_ID3D10Device_IAGetInputLayout );
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers],                  (UINT_PTR)D3D10_ID3D10Device_IAGetVertexBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer],                    (UINT_PTR)D3D10_ID3D10Device_IAGetIndexBuffer );
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers],                (UINT_PTR)D3D10_ID3D10Device_GSGetConstantBuffers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShader],                         (UINT_PTR)D3D10_ID3D10Device_GSGetShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology],              (UINT_PTR)D3D10_ID3D10Device_IAGetPrimitiveTopology);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_VSGetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_VSGetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPredication],                      (UINT_PTR)D3D10_ID3D10Device_GetPredication);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources],                (UINT_PTR)D3D10_ID3D10Device_GSGetShaderResources);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetSamplers],                       (UINT_PTR)D3D10_ID3D10Device_GSGetSamplers);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets],                  (UINT_PTR)D3D10_ID3D10Device_OMGetRenderTargets);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetBlendState],                     (UINT_PTR)D3D10_ID3D10Device_OMGetBlendState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState],              (UINT_PTR)D3D10_ID3D10Device_OMGetDepthStencilState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOGetTargets],                        (UINT_PTR)D3D10_ID3D10Device_SOGetTargets);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetState],                          (UINT_PTR)D3D10_ID3D10Device_RSGetState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetViewports],                      (UINT_PTR)D3D10_ID3D10Device_RSGetViewports);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects],                   (UINT_PTR)D3D10_ID3D10Device_RSGetScissorRects);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason],              (UINT_PTR)D3D10_ID3D10Device_GetDeviceRemovedReason);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetExceptionMode],                    (UINT_PTR)D3D10_ID3D10Device_SetExceptionMode);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetExceptionMode],                    (UINT_PTR)D3D10_ID3D10Device_GetExceptionMode);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPrivateData],                      (UINT_PTR)D3D10_ID3D10Device_GetPrivateData);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateData],                      (UINT_PTR)D3D10_ID3D10Device_SetPrivateData);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface],             (UINT_PTR)D3D10_ID3D10Device_SetPrivateDataInterface);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearState],                          (UINT_PTR)D3D10_ID3D10Device_ClearState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Flush],                               (UINT_PTR)D3D10_ID3D10Device_Flush);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBuffer],                        (UINT_PTR)D3D10_ID3D10Device_CreateBuffer);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture1D],                     (UINT_PTR)D3D10_ID3D10Device_CreateTexture1D);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture2D],                     (UINT_PTR)D3D10_ID3D10Device_CreateTexture2D);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture3D],                     (UINT_PTR)D3D10_ID3D10Device_CreateTexture3D);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView],            (UINT_PTR)D3D10_ID3D10Device_CreateShaderResourceView);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView],              (UINT_PTR)D3D10_ID3D10Device_CreateRenderTargetView);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView],              (UINT_PTR)D3D10_ID3D10Device_CreateDepthStencilView);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateInputLayout],                   (UINT_PTR)D3D10_ID3D10Device_CreateInputLayout);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateVertexShader],                  (UINT_PTR)D3D10_ID3D10Device_CreateVertexShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader],                (UINT_PTR)D3D10_ID3D10Device_CreateGeometryShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput],(UINT_PTR)D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput );
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePixelShader],                   (UINT_PTR)D3D10_ID3D10Device_CreatePixelShader);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBlendState],                    (UINT_PTR)D3D10_ID3D10Device_CreateBlendState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState],             (UINT_PTR)D3D10_ID3D10Device_CreateDepthStencilState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState],               (UINT_PTR)D3D10_ID3D10Device_CreateRasterizerState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateSamplerState],                  (UINT_PTR)D3D10_ID3D10Device_CreateSamplerState);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateQuery],                         (UINT_PTR)D3D10_ID3D10Device_CreateQuery);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePredicate],                     (UINT_PTR)D3D10_ID3D10Device_CreatePredicate);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateCounter],                       (UINT_PTR)D3D10_ID3D10Device_CreateCounter);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport],                  (UINT_PTR)D3D10_ID3D10Device_CheckFormatSupport);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels],       (UINT_PTR)D3D10_ID3D10Device_CheckMultisampleQualityLevels);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo],                    (UINT_PTR)D3D10_ID3D10Device_CheckCounterInfo);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounter],                        (UINT_PTR)D3D10_ID3D10Device_CheckCounter);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetCreationFlags],                    (UINT_PTR)D3D10_ID3D10Device_GetCreationFlags);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OpenSharedResource],                  (UINT_PTR)D3D10_ID3D10Device_OpenSharedResource);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize],                   (UINT_PTR)D3D10_ID3D10Device_SetTextFilterSize);
	OverrideVTable(&D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize],                   (UINT_PTR)D3D10_ID3D10Device_GetTextFilterSize);
}

/**
* Creates a Virtual Methods Table array of the DirectX 10 DXGISwapChain
***/
void Generate_D3D10_ID3D10Device_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::QueryInterface] =                           (UINT_PTR)D3D10_ID3D10Device_QueryInterface;
	anD3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::AddRef] =                                   (UINT_PTR)D3D10_ID3D10Device_AddRef;
	anD3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::Release] =                                  (UINT_PTR)D3D10_ID3D10Device_Release;

	/*** ID3D10Device super methods ***/
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_VSSetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_PSSetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShader] =                          (UINT_PTR)D3D10_ID3D10Device_PSSetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_PSSetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShader] =                          (UINT_PTR)D3D10_ID3D10Device_VSSetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexed] =                          (UINT_PTR)D3D10_ID3D10Device_DrawIndexed;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Draw] =                                 (UINT_PTR)D3D10_ID3D10Device_Draw;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_PSSetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetInputLayout] =                     (UINT_PTR)D3D10_ID3D10Device_IASetInputLayout;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers] =                   (UINT_PTR)D3D10_ID3D10Device_IASetVertexBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer] =                     (UINT_PTR)D3D10_ID3D10Device_IASetIndexBuffer;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced] =                 (UINT_PTR)D3D10_ID3D10Device_DrawIndexedInstanced;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawInstanced] =                        (UINT_PTR)D3D10_ID3D10Device_DrawInstanced;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_GSSetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShader] =                          (UINT_PTR)D3D10_ID3D10Device_GSSetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology] =               (UINT_PTR)D3D10_ID3D10Device_IASetPrimitiveTopology;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_VSSetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_VSSetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPredication] =                       (UINT_PTR)D3D10_ID3D10Device_SetPredication;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_GSSetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_GSSetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets] =                   (UINT_PTR)D3D10_ID3D10Device_OMSetRenderTargets;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetBlendState] =                      (UINT_PTR)D3D10_ID3D10Device_OMSetBlendState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState] =               (UINT_PTR)D3D10_ID3D10Device_OMSetDepthStencilState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOSetTargets] =                         (UINT_PTR)D3D10_ID3D10Device_SOSetTargets;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawAuto] =                             (UINT_PTR)D3D10_ID3D10Device_DrawAuto;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetState] =                           (UINT_PTR)D3D10_ID3D10Device_RSSetState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetViewports] =                       (UINT_PTR)D3D10_ID3D10Device_RSSetViewports;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects] =                    (UINT_PTR)D3D10_ID3D10Device_RSSetScissorRects;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion] =                (UINT_PTR)D3D10_ID3D10Device_CopySubresourceRegion;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopyResource] =                         (UINT_PTR)D3D10_ID3D10Device_CopyResource ;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::UpdateSubresource] =                    (UINT_PTR)D3D10_ID3D10Device_UpdateSubresource;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView] =                (UINT_PTR)D3D10_ID3D10Device_ClearRenderTargetView;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView] =                (UINT_PTR)D3D10_ID3D10Device_ClearDepthStencilView;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GenerateMips] =                         (UINT_PTR)D3D10_ID3D10Device_GenerateMips ;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ResolveSubresource] =                   (UINT_PTR)D3D10_ID3D10Device_ResolveSubresource;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_VSGetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_PSGetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShader] =                          (UINT_PTR)D3D10_ID3D10Device_PSGetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_PSGetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShader] =                          (UINT_PTR)D3D10_ID3D10Device_VSGetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_PSGetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout] =                     (UINT_PTR)D3D10_ID3D10Device_IAGetInputLayout ;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers] =                   (UINT_PTR)D3D10_ID3D10Device_IAGetVertexBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer] =                     (UINT_PTR)D3D10_ID3D10Device_IAGetIndexBuffer ;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers] =                 (UINT_PTR)D3D10_ID3D10Device_GSGetConstantBuffers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShader] =                          (UINT_PTR)D3D10_ID3D10Device_GSGetShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology] =               (UINT_PTR)D3D10_ID3D10Device_IAGetPrimitiveTopology;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_VSGetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_VSGetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPredication] =                       (UINT_PTR)D3D10_ID3D10Device_GetPredication;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources] =                 (UINT_PTR)D3D10_ID3D10Device_GSGetShaderResources;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetSamplers] =                        (UINT_PTR)D3D10_ID3D10Device_GSGetSamplers;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets] =                   (UINT_PTR)D3D10_ID3D10Device_OMGetRenderTargets;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetBlendState] =                      (UINT_PTR)D3D10_ID3D10Device_OMGetBlendState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState] =               (UINT_PTR)D3D10_ID3D10Device_OMGetDepthStencilState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOGetTargets] =                         (UINT_PTR)D3D10_ID3D10Device_SOGetTargets;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetState] =                           (UINT_PTR)D3D10_ID3D10Device_RSGetState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetViewports] =                       (UINT_PTR)D3D10_ID3D10Device_RSGetViewports;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects] =                    (UINT_PTR)D3D10_ID3D10Device_RSGetScissorRects;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason] =               (UINT_PTR)D3D10_ID3D10Device_GetDeviceRemovedReason;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetExceptionMode] =                     (UINT_PTR)D3D10_ID3D10Device_SetExceptionMode;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetExceptionMode] =                     (UINT_PTR)D3D10_ID3D10Device_GetExceptionMode;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPrivateData] =                       (UINT_PTR)D3D10_ID3D10Device_GetPrivateData;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateData] =                       (UINT_PTR)D3D10_ID3D10Device_SetPrivateData;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface] =              (UINT_PTR)D3D10_ID3D10Device_SetPrivateDataInterface;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearState] =                           (UINT_PTR)D3D10_ID3D10Device_ClearState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Flush] =                                (UINT_PTR)D3D10_ID3D10Device_Flush;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBuffer] =                         (UINT_PTR)D3D10_ID3D10Device_CreateBuffer;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture1D] =                      (UINT_PTR)D3D10_ID3D10Device_CreateTexture1D;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture2D] =                      (UINT_PTR)D3D10_ID3D10Device_CreateTexture2D;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture3D] =                      (UINT_PTR)D3D10_ID3D10Device_CreateTexture3D;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView] =             (UINT_PTR)D3D10_ID3D10Device_CreateShaderResourceView;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView] =               (UINT_PTR)D3D10_ID3D10Device_CreateRenderTargetView;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView] =               (UINT_PTR)D3D10_ID3D10Device_CreateDepthStencilView;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateInputLayout] =                    (UINT_PTR)D3D10_ID3D10Device_CreateInputLayout;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateVertexShader] =                   (UINT_PTR)D3D10_ID3D10Device_CreateVertexShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader] =                 (UINT_PTR)D3D10_ID3D10Device_CreateGeometryShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput] = (UINT_PTR)D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput ;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePixelShader] =                    (UINT_PTR)D3D10_ID3D10Device_CreatePixelShader;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBlendState] =                     (UINT_PTR)D3D10_ID3D10Device_CreateBlendState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState] =              (UINT_PTR)D3D10_ID3D10Device_CreateDepthStencilState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState] =                (UINT_PTR)D3D10_ID3D10Device_CreateRasterizerState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateSamplerState] =                   (UINT_PTR)D3D10_ID3D10Device_CreateSamplerState;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateQuery] =                          (UINT_PTR)D3D10_ID3D10Device_CreateQuery;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePredicate] =                      (UINT_PTR)D3D10_ID3D10Device_CreatePredicate;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateCounter] =                        (UINT_PTR)D3D10_ID3D10Device_CreateCounter;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport] =                   (UINT_PTR)D3D10_ID3D10Device_CheckFormatSupport;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels] =        (UINT_PTR)D3D10_ID3D10Device_CheckMultisampleQualityLevels;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo] =                     (UINT_PTR)D3D10_ID3D10Device_CheckCounterInfo;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounter] =                         (UINT_PTR)D3D10_ID3D10Device_CheckCounter;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetCreationFlags] =                     (UINT_PTR)D3D10_ID3D10Device_GetCreationFlags;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OpenSharedResource] =                   (UINT_PTR)D3D10_ID3D10Device_OpenSharedResource;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize] =                    (UINT_PTR)D3D10_ID3D10Device_SetTextFilterSize;
	anD3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize] =                    (UINT_PTR)D3D10_ID3D10Device_GetTextFilterSize;
}

/**
* Overrides the Virtual Methods Table of the DirectX 10 DXGISwapChain using a generated array.
***/
void Override_D3D10_ID3D10Device_VMTable_by_Array()
{
	OverrideFullVTable(&D3D10_ID3D10Device_VMTable[0],
		(void*)&anD3D10_ID3D10Device_VMTable[0],
		(D3D10_DEVICE_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 10 DXGISwapChain
***/
void Detour_D3D10_ID3D10Device_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                           (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::QueryInterface],                          (PBYTE)D3D10_ID3D10Device_QueryInterface,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::AddRef],                                  (PBYTE)D3D10_ID3D10Device_AddRef,                                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                                  (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_IUNKNOWN::Release],                                 (PBYTE)D3D10_ID3D10Device_Release,                               JMP32_SZ));

	/*** ID3D10Device super methods ***/
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_VSSetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShaderResources],                (PBYTE)D3D10_ID3D10Device_PSSetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetShader],                         (PBYTE)D3D10_ID3D10Device_PSSetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetSamplers],                       (PBYTE)D3D10_ID3D10Device_PSSetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShader],                         (PBYTE)D3D10_ID3D10Device_VSSetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawIndexed,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexed],                         (PBYTE)D3D10_ID3D10Device_DrawIndexed,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::Draw,                                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Draw],                                (PBYTE)D3D10_ID3D10Device_Draw,                                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSSetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_PSSetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetInputLayout,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetInputLayout],                    (PBYTE)D3D10_ID3D10Device_IASetInputLayout,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetVertexBuffers,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetVertexBuffers],                  (PBYTE)D3D10_ID3D10Device_IASetVertexBuffers,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetIndexBuffer,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetIndexBuffer],                    (PBYTE)D3D10_ID3D10Device_IASetIndexBuffer,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawIndexedInstanced,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced],                (PBYTE)D3D10_ID3D10Device_DrawIndexedInstanced,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawInstanced,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawInstanced],                       (PBYTE)D3D10_ID3D10Device_DrawInstanced,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_GSSetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShader],                         (PBYTE)D3D10_ID3D10Device_GSSetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IASetPrimitiveTopology,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology],              (PBYTE)D3D10_ID3D10Device_IASetPrimitiveTopology,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetShaderResources],                (PBYTE)D3D10_ID3D10Device_VSSetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSSetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSSetSamplers],                       (PBYTE)D3D10_ID3D10Device_VSSetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPredication,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPredication],                      (PBYTE)D3D10_ID3D10Device_SetPredication,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetShaderResources],                (PBYTE)D3D10_ID3D10Device_GSSetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSSetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSSetSamplers],                       (PBYTE)D3D10_ID3D10Device_GSSetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetRenderTargets,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetRenderTargets],                  (PBYTE)D3D10_ID3D10Device_OMSetRenderTargets,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetBlendState,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetBlendState],                     (PBYTE)D3D10_ID3D10Device_OMSetBlendState,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMSetDepthStencilState,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState],              (PBYTE)D3D10_ID3D10Device_OMSetDepthStencilState,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SOSetTargets,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOSetTargets],                        (PBYTE)D3D10_ID3D10Device_SOSetTargets,                          JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::DrawAuto,                             (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::DrawAuto],                            (PBYTE)D3D10_ID3D10Device_DrawAuto,                              JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetState,                           (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetState],                          (PBYTE)D3D10_ID3D10Device_RSSetState,                            JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetViewports,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetViewports],                      (PBYTE)D3D10_ID3D10Device_RSSetViewports,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSSetScissorRects,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSSetScissorRects],                   (PBYTE)D3D10_ID3D10Device_RSSetScissorRects,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CopySubresourceRegion,                (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopySubresourceRegion],               (PBYTE)D3D10_ID3D10Device_CopySubresourceRegion,                 JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CopyResource,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CopyResource],                        (PBYTE)D3D10_ID3D10Device_CopyResource ,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::UpdateSubresource,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::UpdateSubresource],                   (PBYTE)D3D10_ID3D10Device_UpdateSubresource,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearRenderTargetView,                (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearRenderTargetView],               (PBYTE)D3D10_ID3D10Device_ClearRenderTargetView,                 JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearDepthStencilView,                (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearDepthStencilView],               (PBYTE)D3D10_ID3D10Device_ClearDepthStencilView,                 JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GenerateMips,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GenerateMips],                        (PBYTE)D3D10_ID3D10Device_GenerateMips,                          JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ResolveSubresource,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ResolveSubresource],                  (PBYTE)D3D10_ID3D10Device_ResolveSubresource,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_VSGetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShaderResources],                (PBYTE)D3D10_ID3D10Device_PSGetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetShader],                         (PBYTE)D3D10_ID3D10Device_PSGetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetSamplers],                       (PBYTE)D3D10_ID3D10Device_PSGetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShader],                         (PBYTE)D3D10_ID3D10Device_VSGetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::PSGetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_PSGetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetInputLayout,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetInputLayout],                    (PBYTE)D3D10_ID3D10Device_IAGetInputLayout,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetVertexBuffers,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers],                  (PBYTE)D3D10_ID3D10Device_IAGetVertexBuffers,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetIndexBuffer,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer],                    (PBYTE)D3D10_ID3D10Device_IAGetIndexBuffer,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetConstantBuffers,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers],                (PBYTE)D3D10_ID3D10Device_GSGetConstantBuffers,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetShader,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShader],                         (PBYTE)D3D10_ID3D10Device_GSGetShader,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::IAGetPrimitiveTopology,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology],              (PBYTE)D3D10_ID3D10Device_IAGetPrimitiveTopology,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetShaderResources],                (PBYTE)D3D10_ID3D10Device_VSGetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::VSGetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::VSGetSamplers],                       (PBYTE)D3D10_ID3D10Device_VSGetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetPredication,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPredication],                      (PBYTE)D3D10_ID3D10Device_GetPredication,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetShaderResources,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetShaderResources],                (PBYTE)D3D10_ID3D10Device_GSGetShaderResources,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GSGetSamplers,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GSGetSamplers],                       (PBYTE)D3D10_ID3D10Device_GSGetSamplers,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetRenderTargets,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetRenderTargets],                  (PBYTE)D3D10_ID3D10Device_OMGetRenderTargets,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetBlendState,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetBlendState],                     (PBYTE)D3D10_ID3D10Device_OMGetBlendState,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OMGetDepthStencilState,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState],              (PBYTE)D3D10_ID3D10Device_OMGetDepthStencilState,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SOGetTargets,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SOGetTargets],                        (PBYTE)D3D10_ID3D10Device_SOGetTargets,                          JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetState,                           (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetState],                          (PBYTE)D3D10_ID3D10Device_RSGetState,                            JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetViewports,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetViewports],                      (PBYTE)D3D10_ID3D10Device_RSGetViewports,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::RSGetScissorRects,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::RSGetScissorRects],                   (PBYTE)D3D10_ID3D10Device_RSGetScissorRects,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetDeviceRemovedReason,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason],              (PBYTE)D3D10_ID3D10Device_GetDeviceRemovedReason,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetExceptionMode,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetExceptionMode],                    (PBYTE)D3D10_ID3D10Device_SetExceptionMode,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetExceptionMode,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetExceptionMode],                    (PBYTE)D3D10_ID3D10Device_GetExceptionMode,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetPrivateData,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetPrivateData],                      (PBYTE)D3D10_ID3D10Device_GetPrivateData,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPrivateData,                       (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateData],                      (PBYTE)D3D10_ID3D10Device_SetPrivateData,                        JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetPrivateDataInterface,              (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface],             (PBYTE)D3D10_ID3D10Device_SetPrivateDataInterface,               JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::ClearState,                           (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::ClearState],                          (PBYTE)D3D10_ID3D10Device_ClearState,                            JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::Flush,                                (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::Flush],                               (PBYTE)D3D10_ID3D10Device_Flush,                                 JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateBuffer,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBuffer],                        (PBYTE)D3D10_ID3D10Device_CreateBuffer,                          JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture1D,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture1D],                     (PBYTE)D3D10_ID3D10Device_CreateTexture1D,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture2D,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture2D],                     (PBYTE)D3D10_ID3D10Device_CreateTexture2D,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateTexture3D,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateTexture3D],                     (PBYTE)D3D10_ID3D10Device_CreateTexture3D,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateShaderResourceView,             (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateShaderResourceView],            (PBYTE)D3D10_ID3D10Device_CreateShaderResourceView,              JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateRenderTargetView,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRenderTargetView],              (PBYTE)D3D10_ID3D10Device_CreateRenderTargetView,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateDepthStencilView,               (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilView],              (PBYTE)D3D10_ID3D10Device_CreateDepthStencilView,                JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateInputLayout,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateInputLayout],                   (PBYTE)D3D10_ID3D10Device_CreateInputLayout,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateVertexShader,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateVertexShader],                  (PBYTE)D3D10_ID3D10Device_CreateVertexShader,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateGeometryShader,                 (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShader],                (PBYTE)D3D10_ID3D10Device_CreateGeometryShader,                  JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput, (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput],(PBYTE)D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput , JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreatePixelShader,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePixelShader],                   (PBYTE)D3D10_ID3D10Device_CreatePixelShader,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateBlendState,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateBlendState],                    (PBYTE)D3D10_ID3D10Device_CreateBlendState,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateDepthStencilState,              (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateDepthStencilState],             (PBYTE)D3D10_ID3D10Device_CreateDepthStencilState,               JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateRasterizerState,                (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateRasterizerState],               (PBYTE)D3D10_ID3D10Device_CreateRasterizerState,                 JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateSamplerState,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateSamplerState],                  (PBYTE)D3D10_ID3D10Device_CreateSamplerState,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateQuery,                          (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateQuery],                         (PBYTE)D3D10_ID3D10Device_CreateQuery,                           JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreatePredicate,                      (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreatePredicate],                     (PBYTE)D3D10_ID3D10Device_CreatePredicate,                       JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CreateCounter,                        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CreateCounter],                       (PBYTE)D3D10_ID3D10Device_CreateCounter,                         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckFormatSupport,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckFormatSupport],                  (PBYTE)D3D10_ID3D10Device_CheckFormatSupport,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,        (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels],       (PBYTE)D3D10_ID3D10Device_CheckMultisampleQualityLevels,         JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckCounterInfo,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounterInfo],                    (PBYTE)D3D10_ID3D10Device_CheckCounterInfo,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::CheckCounter,                         (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::CheckCounter],                        (PBYTE)D3D10_ID3D10Device_CheckCounter,                          JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetCreationFlags,                     (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetCreationFlags],                    (PBYTE)D3D10_ID3D10Device_GetCreationFlags,                      JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::OpenSharedResource,                   (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::OpenSharedResource],                  (PBYTE)D3D10_ID3D10Device_OpenSharedResource,                    JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::SetTextFilterSize,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::SetTextFilterSize],                   (PBYTE)D3D10_ID3D10Device_SetTextFilterSize,                     JMP32_SZ));
	pDCL_ID3D10Device->SetSuperFunctionPointer(VMT_ID3D10DEVICE::GetTextFilterSize,                    (UINT_PTR)DetourFunc((PBYTE)D3D10_ID3D10Device_VMTable[VMT_ID3D10DEVICE::GetTextFilterSize],                   (PBYTE)D3D10_ID3D10Device_GetTextFilterSize,                     JMP32_SZ));
}


#endif