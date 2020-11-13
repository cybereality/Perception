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
#ifndef DCL_ID3D10DEVICE_SUPER
#define DCL_ID3D10DEVICE_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D10_QueryInterface)                       (ID3D10Device* pDevice, REFIID riid, void **ppvObject);
typedef ULONG   (WINAPI* D3D10_AddRef)                               (ID3D10Device* pDevice);
typedef ULONG   (WINAPI* D3D10_Release)                              (ID3D10Device* pDevice);

/*** ID3D10Device method prototypes ***/
typedef void    (WINAPI* D3D10_VSSetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppConstantBuffers);
typedef void    (WINAPI* D3D10_PSSetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView*const* ppShaderResourceViews);
typedef void    (WINAPI* D3D10_PSSetShader)                          (ID3D10Device* pDevice, ID3D10PixelShader *pPixelShader);
typedef void    (WINAPI* D3D10_PSSetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState*const* ppSamplers);
typedef void    (WINAPI* D3D10_VSSetShader)                          (ID3D10Device* pDevice, ID3D10VertexShader *pVertexShader);
typedef void    (WINAPI* D3D10_DrawIndexed)                          (ID3D10Device* pDevice, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void    (WINAPI* D3D10_Draw)                                 (ID3D10Device* pDevice, UINT VertexCount, UINT StartVertexLocation);
typedef void    (WINAPI* D3D10_PSSetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers);
typedef void    (WINAPI* D3D10_IASetInputLayout)                     (ID3D10Device* pDevice, ID3D10InputLayout *pInputLayout);
typedef void    (WINAPI* D3D10_IASetVertexBuffers)                   (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets);
typedef void    (WINAPI* D3D10_IASetIndexBuffer)                     (ID3D10Device* pDevice, ID3D10Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
typedef void    (WINAPI* D3D10_DrawIndexedInstanced)                 (ID3D10Device* pDevice, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void    (WINAPI* D3D10_DrawInstanced)                        (ID3D10Device* pDevice, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void    (WINAPI* D3D10_GSSetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers);
typedef void    (WINAPI* D3D10_GSSetShader)                          (ID3D10Device* pDevice, ID3D10GeometryShader *pShader);
typedef void    (WINAPI* D3D10_IASetPrimitiveTopology)               (ID3D10Device* pDevice, D3D10_PRIMITIVE_TOPOLOGY Topology);
typedef void    (WINAPI* D3D10_VSSetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews);
typedef void    (WINAPI* D3D10_VSSetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers);
typedef void    (WINAPI* D3D10_SetPredication)                       (ID3D10Device* pDevice, ID3D10Predicate *pPredicate, BOOL PredicateValue);
typedef void    (WINAPI* D3D10_GSSetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews);
typedef void    (WINAPI* D3D10_GSSetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers);
typedef void    (WINAPI* D3D10_OMSetRenderTargets)                   (ID3D10Device* pDevice, UINT NumViews, ID3D10RenderTargetView *const *ppRenderTargetViews, ID3D10DepthStencilView *pDepthStencilView);
typedef void    (WINAPI* D3D10_OMSetBlendState)                      (ID3D10Device* pDevice, ID3D10BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask);
typedef void    (WINAPI* D3D10_OMSetDepthStencilState)               (ID3D10Device* pDevice, ID3D10DepthStencilState *pDepthStencilState, UINT StencilRef);
typedef void    (WINAPI* D3D10_SOSetTargets)                         (ID3D10Device* pDevice, UINT NumBuffers, ID3D10Buffer *const *ppSOTargets, const UINT *pOffsets);
typedef void    (WINAPI* D3D10_DrawAuto)                             (ID3D10Device* pDevice);
typedef void    (WINAPI* D3D10_RSSetState)                           (ID3D10Device* pDevice, ID3D10RasterizerState *pRasterizerState);
typedef void    (WINAPI* D3D10_RSSetViewports)                       (ID3D10Device* pDevice, UINT NumViewports, const D3D10_VIEWPORT *pViewports);
typedef void    (WINAPI* D3D10_RSSetScissorRects)                    (ID3D10Device* pDevice, UINT NumRects, const D3D10_RECT *pRects);
typedef void    (WINAPI* D3D10_CopySubresourceRegion)                (ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox);
typedef void    (WINAPI* D3D10_CopyResource)                         (ID3D10Device* pDevice, ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource);
typedef void    (WINAPI* D3D10_UpdateSubresource)                    (ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
typedef void    (WINAPI* D3D10_ClearRenderTargetView)                (ID3D10Device* pDevice, ID3D10RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ]);
typedef void    (WINAPI* D3D10_ClearDepthStencilView)                (ID3D10Device* pDevice, ID3D10DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);
typedef void    (WINAPI* D3D10_GenerateMips)                         (ID3D10Device* pDevice, ID3D10ShaderResourceView *pShaderResourceView);
typedef void    (WINAPI* D3D10_ResolveSubresource)                   (ID3D10Device* pDevice, ID3D10Resource *pDstResource, UINT DstSubresource, ID3D10Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format);
typedef void    (WINAPI* D3D10_VSGetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
typedef void    (WINAPI* D3D10_PSGetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
typedef void    (WINAPI* D3D10_PSGetShader)                          (ID3D10Device* pDevice, ID3D10PixelShader **ppPixelShader);
typedef void    (WINAPI* D3D10_PSGetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
typedef void    (WINAPI* D3D10_VSGetShader)                          (ID3D10Device* pDevice, ID3D10VertexShader **ppVertexShader);
typedef void    (WINAPI* D3D10_PSGetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
typedef void    (WINAPI* D3D10_IAGetInputLayout)                     (ID3D10Device* pDevice, ID3D10InputLayout **ppInputLayout);
typedef void    (WINAPI* D3D10_IAGetVertexBuffers)                   (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets);
typedef void    (WINAPI* D3D10_IAGetIndexBuffer)                     (ID3D10Device* pDevice, ID3D10Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset);
typedef void    (WINAPI* D3D10_GSGetConstantBuffers)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
typedef void    (WINAPI* D3D10_GSGetShader)                          (ID3D10Device* pDevice, ID3D10GeometryShader **ppGeometryShader);
typedef void    (WINAPI* D3D10_IAGetPrimitiveTopology)               (ID3D10Device* pDevice, D3D10_PRIMITIVE_TOPOLOGY *pTopology);
typedef void    (WINAPI* D3D10_VSGetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
typedef void    (WINAPI* D3D10_VSGetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
typedef void    (WINAPI* D3D10_GetPredication)                       (ID3D10Device* pDevice, ID3D10Predicate **ppPredicate, BOOL *pPredicateValue);
typedef void    (WINAPI* D3D10_GSGetShaderResources)                 (ID3D10Device* pDevice, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
typedef void    (WINAPI* D3D10_GSGetSamplers)                        (ID3D10Device* pDevice, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
typedef void    (WINAPI* D3D10_OMGetRenderTargets)                   (ID3D10Device* pDevice, UINT NumViews, ID3D10RenderTargetView **ppRenderTargetViews, ID3D10DepthStencilView **ppDepthStencilView);
typedef void    (WINAPI* D3D10_OMGetBlendState)                      (ID3D10Device* pDevice, ID3D10BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask);
typedef void    (WINAPI* D3D10_OMGetDepthStencilState)               (ID3D10Device* pDevice, ID3D10DepthStencilState **ppDepthStencilState, UINT *pStencilRef);
typedef void    (WINAPI* D3D10_SOGetTargets)                         (ID3D10Device* pDevice, UINT NumBuffers, ID3D10Buffer **ppSOTargets, UINT *pOffsets);
typedef void    (WINAPI* D3D10_RSGetState)                           (ID3D10Device* pDevice, ID3D10RasterizerState **ppRasterizerState);
typedef void    (WINAPI* D3D10_RSGetViewports)                       (ID3D10Device* pDevice, UINT *NumViewports, D3D10_VIEWPORT *pViewports);
typedef void    (WINAPI* D3D10_RSGetScissorRects)                    (ID3D10Device* pDevice, UINT *NumRects, D3D10_RECT *pRects);
typedef HRESULT (WINAPI* D3D10_GetDeviceRemovedReason)               (ID3D10Device* pDevice);
typedef HRESULT (WINAPI* D3D10_SetExceptionMode)                     (ID3D10Device* pDevice, UINT RaiseFlags);
typedef UINT    (WINAPI* D3D10_GetExceptionMode)                     (ID3D10Device* pDevice);
typedef HRESULT (WINAPI* D3D10_GetPrivateData)                       (ID3D10Device* pDevice, REFGUID guid, UINT *pDataSize, void *pData);
typedef HRESULT (WINAPI* D3D10_SetPrivateData)                       (ID3D10Device* pDevice, REFGUID guid, UINT DataSize, const void *pData);
typedef HRESULT (WINAPI* D3D10_SetPrivateDataInterface)              (ID3D10Device* pDevice, REFGUID guid, const IUnknown *pData);
typedef void    (WINAPI* D3D10_ClearState)                           (ID3D10Device* pDevice);
typedef void    (WINAPI* D3D10_Flush)                                (ID3D10Device* pDevice);
typedef HRESULT (WINAPI* D3D10_CreateBuffer)                         (ID3D10Device* pDevice, const D3D10_BUFFER_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer);
typedef HRESULT (WINAPI* D3D10_CreateTexture1D)                      (ID3D10Device* pDevice, const D3D10_TEXTURE1D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture1D **ppTexture1D);
typedef HRESULT (WINAPI* D3D10_CreateTexture2D)                      (ID3D10Device* pDevice, const D3D10_TEXTURE2D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture2D **ppTexture2D);
typedef HRESULT (WINAPI* D3D10_CreateTexture3D)                      (ID3D10Device* pDevice, const D3D10_TEXTURE3D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture3D **ppTexture3D);
typedef HRESULT (WINAPI* D3D10_CreateShaderResourceView)             (ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D10ShaderResourceView **ppSRView);
typedef HRESULT (WINAPI* D3D10_CreateRenderTargetView)               (ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_RENDER_TARGET_VIEW_DESC *pDesc, ID3D10RenderTargetView **ppRTView);
typedef HRESULT (WINAPI* D3D10_CreateDepthStencilView)               (ID3D10Device* pDevice, ID3D10Resource *pResource, const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D10DepthStencilView **ppDepthStencilView);
typedef HRESULT (WINAPI* D3D10_CreateInputLayout)                    (ID3D10Device* pDevice, const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout **ppInputLayout);
typedef HRESULT (WINAPI* D3D10_CreateVertexShader)                   (ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader);
typedef HRESULT (WINAPI* D3D10_CreateGeometryShader)                 (ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader **ppGeometryShader);
typedef HRESULT (WINAPI* D3D10_CreateGeometryShaderWithStreamOutput) (ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D10_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader **ppGeometryShader);
typedef HRESULT (WINAPI* D3D10_CreatePixelShader)                    (ID3D10Device* pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader);
typedef HRESULT (WINAPI* D3D10_CreateBlendState)                     (ID3D10Device* pDevice, const D3D10_BLEND_DESC *pBlendStateDesc, ID3D10BlendState **ppBlendState);
typedef HRESULT (WINAPI* D3D10_CreateDepthStencilState)              (ID3D10Device* pDevice, const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D10DepthStencilState **ppDepthStencilState);
typedef HRESULT (WINAPI* D3D10_CreateRasterizerState)                (ID3D10Device* pDevice, const D3D10_RASTERIZER_DESC *pRasterizerDesc, ID3D10RasterizerState **ppRasterizerState);
typedef HRESULT (WINAPI* D3D10_CreateSamplerState)                   (ID3D10Device* pDevice, const D3D10_SAMPLER_DESC *pSamplerDesc, ID3D10SamplerState **ppSamplerState);
typedef HRESULT (WINAPI* D3D10_CreateQuery)                          (ID3D10Device* pDevice, const D3D10_QUERY_DESC *pQueryDesc, ID3D10Query **ppQuery);
typedef HRESULT (WINAPI* D3D10_CreatePredicate)                      (ID3D10Device* pDevice, const D3D10_QUERY_DESC *pPredicateDesc, ID3D10Predicate **ppPredicate);
typedef HRESULT (WINAPI* D3D10_CreateCounter)                        (ID3D10Device* pDevice, const D3D10_COUNTER_DESC *pCounterDesc, ID3D10Counter **ppCounter);
typedef HRESULT (WINAPI* D3D10_CheckFormatSupport)                   (ID3D10Device* pDevice, DXGI_FORMAT Format, UINT *pFormatSupport);
typedef HRESULT (WINAPI* D3D10_CheckMultisampleQualityLevels)        (ID3D10Device* pDevice, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels);
typedef void    (WINAPI* D3D10_CheckCounterInfo)                     (ID3D10Device* pDevice, D3D10_COUNTER_INFO *pCounterInfo);
typedef HRESULT (WINAPI* D3D10_CheckCounter)                         (ID3D10Device* pDevice, const D3D10_COUNTER_DESC *pDesc, D3D10_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength);
typedef UINT    (WINAPI* D3D10_GetCreationFlags)                     (ID3D10Device* pDevice);
typedef HRESULT (WINAPI* D3D10_OpenSharedResource)                   (ID3D10Device* pDevice, HANDLE hResource, REFIID ReturnedInterface, void **ppResource);
typedef void    (WINAPI* D3D10_SetTextFilterSize)                    (ID3D10Device* pDevice, UINT Width, UINT Height);
typedef void    (WINAPI* D3D10_GetTextFilterSize)                    (ID3D10Device* pDevice, UINT *pWidth, UINT *pHeight);

/*** IUnknown super methods ***/
D3D10_QueryInterface                        D3D10_ID3D10Device_QueryInterface_Super;
D3D10_AddRef                                D3D10_ID3D10Device_AddRef_Super;
D3D10_Release                               D3D10_ID3D10Device_Release_Super;

/*** ID3D10Device super methods ***/
D3D10_VSSetConstantBuffers                  D3D10_ID3D10Device_VSSetConstantBuffers_Super;
D3D10_PSSetShaderResources                  D3D10_ID3D10Device_PSSetShaderResources_Super;
D3D10_PSSetShader                           D3D10_ID3D10Device_PSSetShader_Super;
D3D10_PSSetSamplers                         D3D10_ID3D10Device_PSSetSamplers_Super;
D3D10_VSSetShader                           D3D10_ID3D10Device_VSSetShader_Super;
D3D10_DrawIndexed                           D3D10_ID3D10Device_DrawIndexed_Super;
D3D10_Draw                                  D3D10_ID3D10Device_Draw_Super;
D3D10_PSSetConstantBuffers                  D3D10_ID3D10Device_PSSetConstantBuffers_Super;
D3D10_IASetInputLayout                      D3D10_ID3D10Device_IASetInputLayout_Super;
D3D10_IASetVertexBuffers                    D3D10_ID3D10Device_IASetVertexBuffers_Super;
D3D10_IASetIndexBuffer                      D3D10_ID3D10Device_IASetIndexBuffer_Super;
D3D10_DrawIndexedInstanced                  D3D10_ID3D10Device_DrawIndexedInstanced_Super;
D3D10_DrawInstanced                         D3D10_ID3D10Device_DrawInstanced_Super;
D3D10_GSSetConstantBuffers                  D3D10_ID3D10Device_GSSetConstantBuffers_Super;
D3D10_GSSetShader                           D3D10_ID3D10Device_GSSetShader_Super;
D3D10_IASetPrimitiveTopology                D3D10_ID3D10Device_IASetPrimitiveTopology_Super;
D3D10_VSSetShaderResources                  D3D10_ID3D10Device_VSSetShaderResources_Super;
D3D10_VSSetSamplers                         D3D10_ID3D10Device_VSSetSamplers_Super;
D3D10_SetPredication                        D3D10_ID3D10Device_SetPredication_Super;
D3D10_GSSetShaderResources                  D3D10_ID3D10Device_GSSetShaderResources_Super;
D3D10_GSSetSamplers                         D3D10_ID3D10Device_GSSetSamplers_Super;
D3D10_OMSetRenderTargets                    D3D10_ID3D10Device_OMSetRenderTargets_Super;
D3D10_OMSetBlendState                       D3D10_ID3D10Device_OMSetBlendState_Super;
D3D10_OMSetDepthStencilState                D3D10_ID3D10Device_OMSetDepthStencilState_Super;
D3D10_SOSetTargets                          D3D10_ID3D10Device_SOSetTargets_Super;
D3D10_DrawAuto                              D3D10_ID3D10Device_DrawAuto_Super;
D3D10_RSSetState                            D3D10_ID3D10Device_RSSetState_Super;
D3D10_RSSetViewports                        D3D10_ID3D10Device_RSSetViewports_Super;
D3D10_RSSetScissorRects                     D3D10_ID3D10Device_RSSetScissorRects_Super;
D3D10_CopySubresourceRegion                 D3D10_ID3D10Device_CopySubresourceRegion_Super;
D3D10_CopyResource                          D3D10_ID3D10Device_CopyResource_Super;
D3D10_UpdateSubresource                     D3D10_ID3D10Device_UpdateSubresource_Super;
D3D10_ClearRenderTargetView                 D3D10_ID3D10Device_ClearRenderTargetView_Super;
D3D10_ClearDepthStencilView                 D3D10_ID3D10Device_ClearDepthStencilView_Super;
D3D10_GenerateMips                          D3D10_ID3D10Device_GenerateMips_Super;
D3D10_ResolveSubresource                    D3D10_ID3D10Device_ResolveSubresource_Super;
D3D10_VSGetConstantBuffers                  D3D10_ID3D10Device_VSGetConstantBuffers_Super;
D3D10_PSGetShaderResources                  D3D10_ID3D10Device_PSGetShaderResources_Super;
D3D10_PSGetShader                           D3D10_ID3D10Device_PSGetShader_Super;
D3D10_PSGetSamplers                         D3D10_ID3D10Device_PSGetSamplers_Super;
D3D10_VSGetShader                           D3D10_ID3D10Device_VSGetShader_Super;
D3D10_PSGetConstantBuffers                  D3D10_ID3D10Device_PSGetConstantBuffers_Super;
D3D10_IAGetInputLayout                      D3D10_ID3D10Device_IAGetInputLayout_Super;
D3D10_IAGetVertexBuffers                    D3D10_ID3D10Device_IAGetVertexBuffers_Super;
D3D10_IAGetIndexBuffer                      D3D10_ID3D10Device_IAGetIndexBuffer_Super;
D3D10_GSGetConstantBuffers                  D3D10_ID3D10Device_GSGetConstantBuffers_Super;
D3D10_GSGetShader                           D3D10_ID3D10Device_GSGetShader_Super;
D3D10_IAGetPrimitiveTopology                D3D10_ID3D10Device_IAGetPrimitiveTopology_Super;
D3D10_VSGetShaderResources                  D3D10_ID3D10Device_VSGetShaderResources_Super;
D3D10_VSGetSamplers                         D3D10_ID3D10Device_VSGetSamplers_Super;
D3D10_GetPredication                        D3D10_ID3D10Device_GetPredication_Super;
D3D10_GSGetShaderResources                  D3D10_ID3D10Device_GSGetShaderResources_Super;
D3D10_GSGetSamplers                         D3D10_ID3D10Device_GSGetSamplers_Super;
D3D10_OMGetRenderTargets                    D3D10_ID3D10Device_OMGetRenderTargets_Super;
D3D10_OMGetBlendState                       D3D10_ID3D10Device_OMGetBlendState_Super;
D3D10_OMGetDepthStencilState                D3D10_ID3D10Device_OMGetDepthStencilState_Super;
D3D10_SOGetTargets                          D3D10_ID3D10Device_SOGetTargets_Super;
D3D10_RSGetState                            D3D10_ID3D10Device_RSGetState_Super;
D3D10_RSGetViewports                        D3D10_ID3D10Device_RSGetViewports_Super;
D3D10_RSGetScissorRects                     D3D10_ID3D10Device_RSGetScissorRects_Super;
D3D10_GetDeviceRemovedReason                D3D10_ID3D10Device_GetDeviceRemovedReason_Super;
D3D10_SetExceptionMode                      D3D10_ID3D10Device_SetExceptionMode_Super;
D3D10_GetExceptionMode                      D3D10_ID3D10Device_GetExceptionMode_Super;
D3D10_GetPrivateData                        D3D10_ID3D10Device_GetPrivateData_Super;
D3D10_SetPrivateData                        D3D10_ID3D10Device_SetPrivateData_Super;
D3D10_SetPrivateDataInterface               D3D10_ID3D10Device_SetPrivateDataInterface_Super;
D3D10_ClearState                            D3D10_ID3D10Device_ClearState_Super;
D3D10_Flush                                 D3D10_ID3D10Device_Flush_Super;
D3D10_CreateBuffer                          D3D10_ID3D10Device_CreateBuffer_Super;
D3D10_CreateTexture1D                       D3D10_ID3D10Device_CreateTexture1D_Super;
D3D10_CreateTexture2D                       D3D10_ID3D10Device_CreateTexture2D_Super;
D3D10_CreateTexture3D                       D3D10_ID3D10Device_CreateTexture3D_Super;
D3D10_CreateShaderResourceView              D3D10_ID3D10Device_CreateShaderResourceView_Super;
D3D10_CreateRenderTargetView                D3D10_ID3D10Device_CreateRenderTargetView_Super;
D3D10_CreateDepthStencilView                D3D10_ID3D10Device_CreateDepthStencilView_Super;
D3D10_CreateInputLayout                     D3D10_ID3D10Device_CreateInputLayout_Super;
D3D10_CreateVertexShader                    D3D10_ID3D10Device_CreateVertexShader_Super;
D3D10_CreateGeometryShader                  D3D10_ID3D10Device_CreateGeometryShader_Super;
D3D10_CreateGeometryShaderWithStreamOutput  D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput_Super;
D3D10_CreatePixelShader                     D3D10_ID3D10Device_CreatePixelShader_Super;
D3D10_CreateBlendState                      D3D10_ID3D10Device_CreateBlendState_Super;
D3D10_CreateDepthStencilState               D3D10_ID3D10Device_CreateDepthStencilState_Super;
D3D10_CreateRasterizerState                 D3D10_ID3D10Device_CreateRasterizerState_Super;
D3D10_CreateSamplerState                    D3D10_ID3D10Device_CreateSamplerState_Super;
D3D10_CreateQuery                           D3D10_ID3D10Device_CreateQuery_Super;
D3D10_CreatePredicate                       D3D10_ID3D10Device_CreatePredicate_Super;
D3D10_CreateCounter                         D3D10_ID3D10Device_CreateCounter_Super;
D3D10_CheckFormatSupport                    D3D10_ID3D10Device_CheckFormatSupport_Super;
D3D10_CheckMultisampleQualityLevels         D3D10_ID3D10Device_CheckMultisampleQualityLevels_Super;
D3D10_CheckCounterInfo                      D3D10_ID3D10Device_CheckCounterInfo_Super;
D3D10_CheckCounter                          D3D10_ID3D10Device_CheckCounter_Super;
D3D10_GetCreationFlags                      D3D10_ID3D10Device_GetCreationFlags_Super;
D3D10_OpenSharedResource                    D3D10_ID3D10Device_OpenSharedResource_Super;
D3D10_SetTextFilterSize                     D3D10_ID3D10Device_SetTextFilterSize_Super;
D3D10_GetTextFilterSize                     D3D10_ID3D10Device_GetTextFilterSize_Super;

#endif