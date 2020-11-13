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
#ifndef DCL_ID3D11DEVICE_SUPER
#define DCL_ID3D11DEVICE_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT           (WINAPI* D3D11_QueryInterface)                       (ID3D11Device* pDevice, REFIID riid, void **ppvObject);
typedef ULONG             (WINAPI* D3D11_AddRef)                               (ID3D11Device *pDevice);
typedef ULONG             (WINAPI* D3D11_Release)                              (ID3D11Device *pDevice);

/*** ID3D11Device methods ***/     	
typedef HRESULT           (WINAPI* D3D11_CreateBuffer)                         (ID3D11Device *pDevice, const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer);
typedef HRESULT           (WINAPI* D3D11_CreateTexture1D)                      (ID3D11Device *pDevice, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D);
typedef HRESULT           (WINAPI* D3D11_CreateTexture2D)                      (ID3D11Device *pDevice, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D);
typedef HRESULT           (WINAPI* D3D11_CreateTexture3D)                      (ID3D11Device *pDevice, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D);
typedef HRESULT           (WINAPI* D3D11_CreateShaderResourceView)             (ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView);
typedef HRESULT           (WINAPI* D3D11_CreateUnorderedAccessView)            (ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView);
typedef HRESULT           (WINAPI* D3D11_CreateRenderTargetView)               (ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView);
typedef HRESULT           (WINAPI* D3D11_CreateDepthStencilView)               (ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView);
typedef HRESULT           (WINAPI* D3D11_CreateInputLayout)                    (ID3D11Device *pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout);
typedef HRESULT           (WINAPI* D3D11_CreateVertexShader)                   (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
typedef HRESULT           (WINAPI* D3D11_CreateGeometryShader)                 (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader);
typedef HRESULT           (WINAPI* D3D11_CreateGeometryShaderWithStreamOutput) (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader);
typedef HRESULT           (WINAPI* D3D11_CreatePixelShader)                    (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader);
typedef HRESULT           (WINAPI* D3D11_CreateHullShader)                     (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader);
typedef HRESULT           (WINAPI* D3D11_CreateDomainShader)                   (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader);
typedef HRESULT           (WINAPI* D3D11_CreateComputeShader)                  (ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader);
typedef HRESULT           (WINAPI* D3D11_CreateClassLinkage)                   (ID3D11Device *pDevice, ID3D11ClassLinkage **ppLinkage);
typedef HRESULT           (WINAPI* D3D11_CreateBlendState)                     (ID3D11Device *pDevice, const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState);
typedef HRESULT           (WINAPI* D3D11_CreateDepthStencilState)              (ID3D11Device *pDevice, const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState);
typedef HRESULT           (WINAPI* D3D11_CreateRasterizerState)                (ID3D11Device *pDevice, const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState);
typedef HRESULT           (WINAPI* D3D11_CreateSamplerState)                   (ID3D11Device *pDevice, const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState);
typedef HRESULT           (WINAPI* D3D11_CreateQuery)                          (ID3D11Device *pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef HRESULT           (WINAPI* D3D11_CreatePredicate)                      (ID3D11Device *pDevice, const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate);
typedef HRESULT           (WINAPI* D3D11_CreateCounter)                        (ID3D11Device *pDevice, const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter);
typedef HRESULT           (WINAPI* D3D11_CreateDeferredContext)                (ID3D11Device *pDevice, UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext);
typedef HRESULT           (WINAPI* D3D11_OpenSharedResource)                   (ID3D11Device *pDevice, HANDLE hResource, REFIID ReturnedInterface, void **ppResource);
typedef HRESULT           (WINAPI* D3D11_CheckFormatSupport)                   (ID3D11Device *pDevice, DXGI_FORMAT Format, UINT *pFormatSupport);
typedef HRESULT           (WINAPI* D3D11_CheckMultisampleQualityLevels)        (ID3D11Device *pDevice, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels);
typedef void              (WINAPI* D3D11_CheckCounterInfo)                     (ID3D11Device *pDevice, D3D11_COUNTER_INFO *pCounterInfo);
typedef HRESULT           (WINAPI* D3D11_CheckCounter)                         (ID3D11Device *pDevice, const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength);
typedef HRESULT           (WINAPI* D3D11_CheckFeatureSupport)                  (ID3D11Device *pDevice, D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize);
typedef HRESULT           (WINAPI* D3D11_GetPrivateData)                       (ID3D11Device *pDevice, REFGUID guid, UINT *pDataSize, void *pData);
typedef HRESULT           (WINAPI* D3D11_SetPrivateData)                       (ID3D11Device *pDevice, REFGUID guid, UINT DataSize, const void *pData);
typedef HRESULT           (WINAPI* D3D11_SetPrivateDataInterface)              (ID3D11Device *pDevice, REFGUID guid, const IUnknown *pData);
typedef	D3D_FEATURE_LEVEL (WINAPI* D3D11_GetFeatureLevel)                      (ID3D11Device *pDevice);
typedef UINT              (WINAPI* D3D11_GetCreationFlags)                     (ID3D11Device *pDevice);
typedef HRESULT           (WINAPI* D3D11_GetDeviceRemovedReason)               (ID3D11Device *pDevice);
typedef void              (WINAPI* D3D11_GetImmediateContext)                  (ID3D11Device *pDevice, ID3D11DeviceContext **ppImmediateContext);
typedef HRESULT           (WINAPI* D3D11_SetExceptionMode)                     (ID3D11Device *pDevice, UINT RaiseFlags);
typedef UINT              (WINAPI* D3D11_GetExceptionMode)                     (ID3D11Device *pDevice);

/*** IUnknown super methods ***/
D3D11_QueryInterface                       D3D11_ID3D11Device_QueryInterface_Super;
D3D11_AddRef                               D3D11_ID3D11Device_AddRef_Super;
D3D11_Release                              D3D11_ID3D11Device_Release_Super;

/*** ID3D11Device super methods ***/
D3D11_CreateBuffer                         D3D11_ID3D11Device_CreateBuffer_Super;
D3D11_CreateTexture1D                      D3D11_ID3D11Device_CreateTexture1D_Super;
D3D11_CreateTexture2D                      D3D11_ID3D11Device_CreateTexture2D_Super;
D3D11_CreateTexture3D                      D3D11_ID3D11Device_CreateTexture3D_Super;
D3D11_CreateShaderResourceView             D3D11_ID3D11Device_CreateShaderResourceView_Super;
D3D11_CreateUnorderedAccessView            D3D11_ID3D11Device_CreateUnorderedAccessView_Super;
D3D11_CreateRenderTargetView               D3D11_ID3D11Device_CreateRenderTargetView_Super;
D3D11_CreateDepthStencilView               D3D11_ID3D11Device_CreateDepthStencilView_Super;
D3D11_CreateInputLayout                    D3D11_ID3D11Device_CreateInputLayout_Super;
D3D11_CreateVertexShader                   D3D11_ID3D11Device_CreateVertexShader_Super;
D3D11_CreateGeometryShader                 D3D11_ID3D11Device_CreateGeometryShader_Super;
D3D11_CreateGeometryShaderWithStreamOutput D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput_Super;
D3D11_CreatePixelShader                    D3D11_ID3D11Device_CreatePixelShader_Super;
D3D11_CreateHullShader                     D3D11_ID3D11Device_CreateHullShader_Super;
D3D11_CreateDomainShader                   D3D11_ID3D11Device_CreateDomainShader_Super;
D3D11_CreateComputeShader                  D3D11_ID3D11Device_CreateComputeShader_Super;
D3D11_CreateClassLinkage                   D3D11_ID3D11Device_CreateClassLinkage_Super;
D3D11_CreateBlendState                     D3D11_ID3D11Device_CreateBlendState_Super;
D3D11_CreateDepthStencilState              D3D11_ID3D11Device_CreateDepthStencilState_Super;
D3D11_CreateRasterizerState                D3D11_ID3D11Device_CreateRasterizerState_Super;
D3D11_CreateSamplerState                   D3D11_ID3D11Device_CreateSamplerState_Super;
D3D11_CreateQuery                          D3D11_ID3D11Device_CreateQuery_Super;
D3D11_CreatePredicate                      D3D11_ID3D11Device_CreatePredicate_Super;
D3D11_CreateCounter                        D3D11_ID3D11Device_CreateCounter_Super;
D3D11_CreateDeferredContext                D3D11_ID3D11Device_CreateDeferredContext_Super;
D3D11_OpenSharedResource                   D3D11_ID3D11Device_OpenSharedResource_Super;
D3D11_CheckFormatSupport                   D3D11_ID3D11Device_CheckFormatSupport_Super;
D3D11_CheckMultisampleQualityLevels        D3D11_ID3D11Device_CheckMultisampleQualityLevels_Super;
D3D11_CheckCounterInfo                     D3D11_ID3D11Device_CheckCounterInfo_Super;
D3D11_CheckCounter                         D3D11_ID3D11Device_CheckCounter_Super;
D3D11_CheckFeatureSupport                  D3D11_ID3D11Device_CheckFeatureSupport_Super;
D3D11_GetPrivateData                       D3D11_ID3D11Device_GetPrivateData_Super;
D3D11_SetPrivateData                       D3D11_ID3D11Device_SetPrivateData_Super;
D3D11_SetPrivateDataInterface              D3D11_ID3D11Device_SetPrivateDataInterface_Super;
D3D11_GetFeatureLevel                      D3D11_ID3D11Device_GetFeatureLevel_Super;
D3D11_GetCreationFlags                     D3D11_ID3D11Device_GetCreationFlags_Super;
D3D11_GetDeviceRemovedReason               D3D11_ID3D11Device_GetDeviceRemovedReason_Super;
D3D11_GetImmediateContext                  D3D11_ID3D11Device_GetImmediateContext_Super;
D3D11_SetExceptionMode                     D3D11_ID3D11Device_SetExceptionMode_Super;
D3D11_GetExceptionMode                     D3D11_ID3D11Device_GetExceptionMode_Super;

#endif