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

#ifndef DMT_ID3D11DEVICE_TABLE
#define DMT_ID3D11DEVICE_TABLE

/*** IUnknown methods ***/
HRESULT           WINAPI D3D11_ID3D11Device_QueryInterface(ID3D11Device *pDevice, REFIID riid, void **ppvObject){ return pDCL_ID3D11Device->QueryInterface(pDevice, riid, ppvObject); }
ULONG             WINAPI D3D11_ID3D11Device_AddRef(ID3D11Device *pDevice){ return pDCL_ID3D11Device->AddRef(pDevice); }
ULONG             WINAPI D3D11_ID3D11Device_Release(ID3D11Device *pDevice){ return pDCL_ID3D11Device->Release(pDevice); }

/*** ID3D11Device methods ***/
HRESULT           WINAPI D3D11_ID3D11Device_CreateBuffer(ID3D11Device *pDevice, const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer){ return pDCL_ID3D11Device->CreateBuffer(pDevice, pDesc, pInitialData, ppBuffer); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateTexture1D(ID3D11Device *pDevice, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D){ return pDCL_ID3D11Device->CreateTexture1D(pDevice, pDesc, pInitialData, ppTexture1D); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateTexture2D(ID3D11Device *pDevice, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D){ return pDCL_ID3D11Device->CreateTexture2D(pDevice, pDesc, pInitialData, ppTexture2D); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateTexture3D(ID3D11Device *pDevice, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D){ return pDCL_ID3D11Device->CreateTexture3D(pDevice, pDesc, pInitialData, ppTexture3D); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateShaderResourceView(ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView){ return pDCL_ID3D11Device->CreateShaderResourceView(pDevice, pResource, pDesc, ppSRView); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateUnorderedAccessView(ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView){ return pDCL_ID3D11Device->CreateUnorderedAccessView(pDevice, pResource, pDesc, ppUAView); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateRenderTargetView(ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView){ return pDCL_ID3D11Device->CreateRenderTargetView(pDevice, pResource, pDesc, ppRTView); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateDepthStencilView(ID3D11Device *pDevice, ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView){ return pDCL_ID3D11Device->CreateDepthStencilView(pDevice, pResource, pDesc, ppDepthStencilView); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateInputLayout(ID3D11Device *pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout){ return pDCL_ID3D11Device->CreateInputLayout(pDevice, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateVertexShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader){ return pDCL_ID3D11Device->CreateVertexShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateGeometryShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader){ return pDCL_ID3D11Device->CreateGeometryShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader){ return pDCL_ID3D11Device->CreateGeometryShaderWithStreamOutput(pDevice, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreatePixelShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader){ return pDCL_ID3D11Device->CreatePixelShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateHullShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader){ return pDCL_ID3D11Device->CreateHullShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateDomainShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader){ return pDCL_ID3D11Device->CreateDomainShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateComputeShader(ID3D11Device *pDevice, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader){ return pDCL_ID3D11Device->CreateComputeShader(pDevice, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateClassLinkage(ID3D11Device *pDevice, ID3D11ClassLinkage **ppLinkage){ return pDCL_ID3D11Device->CreateClassLinkage(pDevice, ppLinkage); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateBlendState(ID3D11Device *pDevice, const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState){ return pDCL_ID3D11Device->CreateBlendState(pDevice, pBlendStateDesc, ppBlendState); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateDepthStencilState(ID3D11Device *pDevice, const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState){ return pDCL_ID3D11Device->CreateDepthStencilState(pDevice, pDepthStencilDesc, ppDepthStencilState); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateRasterizerState(ID3D11Device *pDevice, const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState){ return pDCL_ID3D11Device->CreateRasterizerState(pDevice, pRasterizerDesc, ppRasterizerState); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateSamplerState(ID3D11Device *pDevice, const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState){ return pDCL_ID3D11Device->CreateSamplerState(pDevice, pSamplerDesc, ppSamplerState); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateQuery(ID3D11Device *pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery){ return pDCL_ID3D11Device->CreateQuery(pDevice, pQueryDesc, ppQuery); }
HRESULT           WINAPI D3D11_ID3D11Device_CreatePredicate(ID3D11Device *pDevice, const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate){ return pDCL_ID3D11Device->CreatePredicate(pDevice, pPredicateDesc, ppPredicate); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateCounter(ID3D11Device *pDevice, const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter){ return pDCL_ID3D11Device->CreateCounter(pDevice, pCounterDesc, ppCounter); }
HRESULT           WINAPI D3D11_ID3D11Device_CreateDeferredContext(ID3D11Device *pDevice, UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext){ return pDCL_ID3D11Device->CreateDeferredContext(pDevice, ContextFlags, ppDeferredContext); }
HRESULT           WINAPI D3D11_ID3D11Device_OpenSharedResource(ID3D11Device *pDevice, HANDLE hResource, REFIID ReturnedInterface, void **ppResource){ return pDCL_ID3D11Device->OpenSharedResource(pDevice, hResource, ReturnedInterface, ppResource); }
HRESULT           WINAPI D3D11_ID3D11Device_CheckFormatSupport(ID3D11Device *pDevice, DXGI_FORMAT Format, UINT *pFormatSupport){ return pDCL_ID3D11Device->CheckFormatSupport(pDevice, Format, pFormatSupport); }
HRESULT           WINAPI D3D11_ID3D11Device_CheckMultisampleQualityLevels(ID3D11Device *pDevice, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels){ return pDCL_ID3D11Device->CheckMultisampleQualityLevels(pDevice, Format, SampleCount, pNumQualityLevels); }void WINAPI D3D11_ID3D11Device_CheckCounterInfo(ID3D11Device *pDevice, D3D11_COUNTER_INFO *pCounterInfo){ return pDCL_ID3D11Device->CheckCounterInfo(pDevice, pCounterInfo); }
HRESULT           WINAPI D3D11_ID3D11Device_CheckCounter(ID3D11Device *pDevice, const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength){ return pDCL_ID3D11Device->CheckCounter(pDevice, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
HRESULT           WINAPI D3D11_ID3D11Device_CheckFeatureSupport(ID3D11Device *pDevice, D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize){ return pDCL_ID3D11Device->CheckFeatureSupport(pDevice, Feature, pFeatureSupportData, FeatureSupportDataSize); }
HRESULT           WINAPI D3D11_ID3D11Device_GetPrivateData(ID3D11Device *pDevice, REFGUID guid, UINT *pDataSize, void *pData){ return pDCL_ID3D11Device->GetPrivateData(pDevice, guid, pDataSize, pData); }
HRESULT           WINAPI D3D11_ID3D11Device_SetPrivateData(ID3D11Device *pDevice, REFGUID guid, UINT DataSize, const void *pData){ return pDCL_ID3D11Device->SetPrivateData(pDevice, guid, DataSize, pData); }
HRESULT           WINAPI D3D11_ID3D11Device_SetPrivateDataInterface(ID3D11Device *pDevice, REFGUID guid, const IUnknown *pData){ return pDCL_ID3D11Device->SetPrivateDataInterface(pDevice, guid, pData); }
D3D_FEATURE_LEVEL WINAPI D3D11_ID3D11Device_GetFeatureLevel(ID3D11Device *pDevice){ return pDCL_ID3D11Device->GetFeatureLevel(pDevice); }
UINT              WINAPI D3D11_ID3D11Device_GetCreationFlags(ID3D11Device *pDevice){ return pDCL_ID3D11Device->GetCreationFlags(pDevice); }
HRESULT           WINAPI D3D11_ID3D11Device_GetDeviceRemovedReason(ID3D11Device *pDevice){ return pDCL_ID3D11Device->GetDeviceRemovedReason(pDevice); }
void              WINAPI D3D11_ID3D11Device_GetImmediateContext(ID3D11Device *pDevice, ID3D11DeviceContext **ppImmediateContext){ return pDCL_ID3D11Device->GetImmediateContext(pDevice, ppImmediateContext); }
HRESULT           WINAPI D3D11_ID3D11Device_SetExceptionMode(ID3D11Device *pDevice, UINT RaiseFlags){ return pDCL_ID3D11Device->SetExceptionMode(pDevice, RaiseFlags); }
UINT              WINAPI D3D11_ID3D11Device_GetExceptionMode(ID3D11Device *pDevice){ return pDCL_ID3D11Device->GetExceptionMode(pDevice); }

/**
* Overrides the Virtual Methods Table of the DirectX 11 Device
* I assume that most methods here are unstable when overridden so i commented them out.
***/
void Override_D3D11_ID3D11Device_VMTable()
{
	/*** IUnknown super methods ***/
	/*OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::QueryInterface], (UINT_PTR)D3D11_ID3D11Device_QueryInterface);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::AddRef], (UINT_PTR)D3D11_ID3D11Device_AddRef);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::Release], (UINT_PTR)D3D11_ID3D11Device_Release);*/

	/*** ID3D11Device super methods ***/
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBuffer], (UINT_PTR)D3D11_ID3D11Device_CreateBuffer);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture1D], (UINT_PTR)D3D11_ID3D11Device_CreateTexture1D);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture2D], (UINT_PTR)D3D11_ID3D11Device_CreateTexture2D);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture3D], (UINT_PTR)D3D11_ID3D11Device_CreateTexture3D);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView], (UINT_PTR)D3D11_ID3D11Device_CreateShaderResourceView);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView], (UINT_PTR)D3D11_ID3D11Device_CreateUnorderedAccessView);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView], (UINT_PTR)D3D11_ID3D11Device_CreateRenderTargetView);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView], (UINT_PTR)D3D11_ID3D11Device_CreateDepthStencilView);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateInputLayout], (UINT_PTR)D3D11_ID3D11Device_CreateInputLayout);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateVertexShader], (UINT_PTR)D3D11_ID3D11Device_CreateVertexShader);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader], (UINT_PTR)D3D11_ID3D11Device_CreateGeometryShader);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput], (UINT_PTR)D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePixelShader], (UINT_PTR)D3D11_ID3D11Device_CreatePixelShader);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateHullShader], (UINT_PTR)D3D11_ID3D11Device_CreateHullShader);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDomainShader], (UINT_PTR)D3D11_ID3D11Device_CreateDomainShader);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateComputeShader], (UINT_PTR)D3D11_ID3D11Device_CreateComputeShader);
	/*OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage],                   (UINT_PTR)D3D11_ID3D11Device_CreateClassLinkage);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBlendState],                     (UINT_PTR)D3D11_ID3D11Device_CreateBlendState);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState],              (UINT_PTR)D3D11_ID3D11Device_CreateDepthStencilState);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState],                (UINT_PTR)D3D11_ID3D11Device_CreateRasterizerState);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateSamplerState],                   (UINT_PTR)D3D11_ID3D11Device_CreateSamplerState);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateQuery],                          (UINT_PTR)D3D11_ID3D11Device_CreateQuery);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePredicate],                      (UINT_PTR)D3D11_ID3D11Device_CreatePredicate);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateCounter],                        (UINT_PTR)D3D11_ID3D11Device_CreateCounter);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext],                (UINT_PTR)D3D11_ID3D11Device_CreateDeferredContext);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::OpenSharedResource],                   (UINT_PTR)D3D11_ID3D11Device_OpenSharedResource);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport],                   (UINT_PTR)D3D11_ID3D11Device_CheckFormatSupport);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels],        (UINT_PTR)D3D11_ID3D11Device_CheckMultisampleQualityLevels);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo],                     (UINT_PTR)D3D11_ID3D11Device_CheckCounterInfo);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounter],                         (UINT_PTR)D3D11_ID3D11Device_CheckCounter);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport],                  (UINT_PTR)D3D11_ID3D11Device_CheckFeatureSupport);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetPrivateData],                       (UINT_PTR)D3D11_ID3D11Device_GetPrivateData);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateData],                       (UINT_PTR)D3D11_ID3D11Device_SetPrivateData);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface],              (UINT_PTR)D3D11_ID3D11Device_SetPrivateDataInterface);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel],                      (UINT_PTR)D3D11_ID3D11Device_GetFeatureLevel);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetCreationFlags],                     (UINT_PTR)D3D11_ID3D11Device_GetCreationFlags);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason],               (UINT_PTR)D3D11_ID3D11Device_GetDeviceRemovedReason);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetImmediateContext],                  (UINT_PTR)D3D11_ID3D11Device_GetImmediateContext);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetExceptionMode],                     (UINT_PTR)D3D11_ID3D11Device_SetExceptionMode);
	OverrideVTable(&D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetExceptionMode],                     (UINT_PTR)D3D11_ID3D11Device_GetExceptionMode);*/
}

/**
* Creates a Virtual Methods Table array of the DirectX 11 DXGISwapChain
***/
void Generate_D3D11_ID3D11Device_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::QueryInterface] = (UINT_PTR)D3D11_ID3D11Device_QueryInterface;
	anD3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::AddRef] = (UINT_PTR)D3D11_ID3D11Device_AddRef;
	anD3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::Release] = (UINT_PTR)D3D11_ID3D11Device_Release;

	/*** ID3D11Device super methods ***/
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBuffer] = (UINT_PTR)D3D11_ID3D11Device_CreateBuffer;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture1D] = (UINT_PTR)D3D11_ID3D11Device_CreateTexture1D;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture2D] = (UINT_PTR)D3D11_ID3D11Device_CreateTexture2D;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture3D] = (UINT_PTR)D3D11_ID3D11Device_CreateTexture3D;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView] = (UINT_PTR)D3D11_ID3D11Device_CreateShaderResourceView;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView] = (UINT_PTR)D3D11_ID3D11Device_CreateUnorderedAccessView;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView] = (UINT_PTR)D3D11_ID3D11Device_CreateRenderTargetView;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView] = (UINT_PTR)D3D11_ID3D11Device_CreateDepthStencilView;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateInputLayout] = (UINT_PTR)D3D11_ID3D11Device_CreateInputLayout;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateVertexShader] = (UINT_PTR)D3D11_ID3D11Device_CreateVertexShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader] = (UINT_PTR)D3D11_ID3D11Device_CreateGeometryShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput] = (UINT_PTR)D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePixelShader] = (UINT_PTR)D3D11_ID3D11Device_CreatePixelShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateHullShader] = (UINT_PTR)D3D11_ID3D11Device_CreateHullShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDomainShader] = (UINT_PTR)D3D11_ID3D11Device_CreateDomainShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateComputeShader] = (UINT_PTR)D3D11_ID3D11Device_CreateComputeShader;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage] = (UINT_PTR)D3D11_ID3D11Device_CreateClassLinkage;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBlendState] = (UINT_PTR)D3D11_ID3D11Device_CreateBlendState;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState] = (UINT_PTR)D3D11_ID3D11Device_CreateDepthStencilState;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState] = (UINT_PTR)D3D11_ID3D11Device_CreateRasterizerState;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateSamplerState] = (UINT_PTR)D3D11_ID3D11Device_CreateSamplerState;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateQuery] = (UINT_PTR)D3D11_ID3D11Device_CreateQuery;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePredicate] = (UINT_PTR)D3D11_ID3D11Device_CreatePredicate;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateCounter] = (UINT_PTR)D3D11_ID3D11Device_CreateCounter;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext] = (UINT_PTR)D3D11_ID3D11Device_CreateDeferredContext;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::OpenSharedResource] = (UINT_PTR)D3D11_ID3D11Device_OpenSharedResource;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport] = (UINT_PTR)D3D11_ID3D11Device_CheckFormatSupport;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels] = (UINT_PTR)D3D11_ID3D11Device_CheckMultisampleQualityLevels;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo] = (UINT_PTR)D3D11_ID3D11Device_CheckCounterInfo;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounter] = (UINT_PTR)D3D11_ID3D11Device_CheckCounter;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport] = (UINT_PTR)D3D11_ID3D11Device_CheckFeatureSupport;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetPrivateData] = (UINT_PTR)D3D11_ID3D11Device_GetPrivateData;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateData] = (UINT_PTR)D3D11_ID3D11Device_SetPrivateData;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface] = (UINT_PTR)D3D11_ID3D11Device_SetPrivateDataInterface;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel] = (UINT_PTR)D3D11_ID3D11Device_GetFeatureLevel;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetCreationFlags] = (UINT_PTR)D3D11_ID3D11Device_GetCreationFlags;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason] = (UINT_PTR)D3D11_ID3D11Device_GetDeviceRemovedReason;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetImmediateContext] = (UINT_PTR)D3D11_ID3D11Device_GetImmediateContext;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetExceptionMode] = (UINT_PTR)D3D11_ID3D11Device_SetExceptionMode;
	anD3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetExceptionMode] = (UINT_PTR)D3D11_ID3D11Device_GetExceptionMode;
}

/**
* Overrides the Virtual Methods Table of the DirectX 11 Device using a generated array.
***/
void Override_D3D11_ID3D11Device_VMTable_by_Array()
{
	OverrideFullVTable(&D3D11_ID3D11Device_VMTable[0],
		(void*)&anD3D11_ID3D11Device_VMTable[0],
		(D3D11_DEVICE_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/// <summary>
/// Backup (old method)
/// </summary>
void __Detour_D3D11_ID3D11Device_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::QueryInterface], (PBYTE)D3D11_ID3D11Device_QueryInterface, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::AddRef], (PBYTE)D3D11_ID3D11Device_AddRef, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::Release, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::Release], (PBYTE)D3D11_ID3D11Device_Release, JMP32_SZ));

	/*** ID3D11Device super methods ***/
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBuffer, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBuffer], (PBYTE)D3D11_ID3D11Device_CreateBuffer, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture1D, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture1D], (PBYTE)D3D11_ID3D11Device_CreateTexture1D, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture2D, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture2D], (PBYTE)D3D11_ID3D11Device_CreateTexture2D, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture3D, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture3D], (PBYTE)D3D11_ID3D11Device_CreateTexture3D, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateShaderResourceView, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView], (PBYTE)D3D11_ID3D11Device_CreateShaderResourceView, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateUnorderedAccessView, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView], (PBYTE)D3D11_ID3D11Device_CreateUnorderedAccessView, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRenderTargetView, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView], (PBYTE)D3D11_ID3D11Device_CreateRenderTargetView, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilView, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView], (PBYTE)D3D11_ID3D11Device_CreateDepthStencilView, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateInputLayout, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateInputLayout], (PBYTE)D3D11_ID3D11Device_CreateInputLayout, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateVertexShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateVertexShader], (PBYTE)D3D11_ID3D11Device_CreateVertexShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader], (PBYTE)D3D11_ID3D11Device_CreateGeometryShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput], (PBYTE)D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePixelShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePixelShader], (PBYTE)D3D11_ID3D11Device_CreatePixelShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateHullShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateHullShader], (PBYTE)D3D11_ID3D11Device_CreateHullShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDomainShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDomainShader], (PBYTE)D3D11_ID3D11Device_CreateDomainShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateComputeShader, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateComputeShader], (PBYTE)D3D11_ID3D11Device_CreateComputeShader, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateClassLinkage, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage], (PBYTE)D3D11_ID3D11Device_CreateClassLinkage, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBlendState, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBlendState], (PBYTE)D3D11_ID3D11Device_CreateBlendState, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilState, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState], (PBYTE)D3D11_ID3D11Device_CreateDepthStencilState, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRasterizerState, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState], (PBYTE)D3D11_ID3D11Device_CreateRasterizerState, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateSamplerState, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateSamplerState], (PBYTE)D3D11_ID3D11Device_CreateSamplerState, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateQuery, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateQuery], (PBYTE)D3D11_ID3D11Device_CreateQuery, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePredicate, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePredicate], (PBYTE)D3D11_ID3D11Device_CreatePredicate, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateCounter, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateCounter], (PBYTE)D3D11_ID3D11Device_CreateCounter, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDeferredContext, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext], (PBYTE)D3D11_ID3D11Device_CreateDeferredContext, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::OpenSharedResource, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::OpenSharedResource], (PBYTE)D3D11_ID3D11Device_OpenSharedResource, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFormatSupport, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport], (PBYTE)D3D11_ID3D11Device_CheckFormatSupport, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels], (PBYTE)D3D11_ID3D11Device_CheckMultisampleQualityLevels, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounterInfo, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo], (PBYTE)D3D11_ID3D11Device_CheckCounterInfo, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounter, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounter], (PBYTE)D3D11_ID3D11Device_CheckCounter, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFeatureSupport, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport], (PBYTE)D3D11_ID3D11Device_CheckFeatureSupport, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetPrivateData, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetPrivateData], (PBYTE)D3D11_ID3D11Device_GetPrivateData, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateData, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateData], (PBYTE)D3D11_ID3D11Device_SetPrivateData, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateDataInterface, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface], (PBYTE)D3D11_ID3D11Device_SetPrivateDataInterface, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetFeatureLevel, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel], (PBYTE)D3D11_ID3D11Device_GetFeatureLevel, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetCreationFlags, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetCreationFlags], (PBYTE)D3D11_ID3D11Device_GetCreationFlags, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetDeviceRemovedReason, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason], (PBYTE)D3D11_ID3D11Device_GetDeviceRemovedReason, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetImmediateContext, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetImmediateContext], (PBYTE)D3D11_ID3D11Device_GetImmediateContext, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetExceptionMode, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetExceptionMode], (PBYTE)D3D11_ID3D11Device_SetExceptionMode, JMP32_SZ));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetExceptionMode, (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetExceptionMode], (PBYTE)D3D11_ID3D11Device_GetExceptionMode, JMP32_SZ));
}

/// <summary>
/// Overrides the Virtual Methods Table of the DirectX 11 Device
/// </summary>
void Detour_D3D11_ID3D11Device_VMTable()
{
	OutputDebugString(L"[AQU] Detour_D3D11_ID3D11Device_VMTable");

	/*** IUnknown super methods ***/
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::QueryInterface], (void*)D3D11_ID3D11Device_QueryInterface));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::AddRef], (void*)D3D11_ID3D11Device_AddRef));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_IUNKNOWN::Release, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_IUNKNOWN::Release], (void*)D3D11_ID3D11Device_Release));

	/*** ID3D11Device super methods ***/
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBuffer, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBuffer], (void*)D3D11_ID3D11Device_CreateBuffer));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture1D, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture1D], (void*)D3D11_ID3D11Device_CreateTexture1D));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture2D, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture2D], (void*)D3D11_ID3D11Device_CreateTexture2D));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture3D, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateTexture3D], (void*)D3D11_ID3D11Device_CreateTexture3D));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateShaderResourceView, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView], (void*)D3D11_ID3D11Device_CreateShaderResourceView));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateUnorderedAccessView, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView], (void*)D3D11_ID3D11Device_CreateUnorderedAccessView));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRenderTargetView, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView], (void*)D3D11_ID3D11Device_CreateRenderTargetView));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilView, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView], (void*)D3D11_ID3D11Device_CreateDepthStencilView));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateInputLayout, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateInputLayout], (void*)D3D11_ID3D11Device_CreateInputLayout));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateVertexShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateVertexShader], (void*)D3D11_ID3D11Device_CreateVertexShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader], (void*)D3D11_ID3D11Device_CreateGeometryShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput], (void*)D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePixelShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePixelShader], (void*)D3D11_ID3D11Device_CreatePixelShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateHullShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateHullShader], (void*)D3D11_ID3D11Device_CreateHullShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDomainShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDomainShader], (void*)D3D11_ID3D11Device_CreateDomainShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateComputeShader, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateComputeShader], (void*)D3D11_ID3D11Device_CreateComputeShader));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateClassLinkage, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage], (void*)D3D11_ID3D11Device_CreateClassLinkage));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBlendState, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateBlendState], (void*)D3D11_ID3D11Device_CreateBlendState));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilState, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState], (void*)D3D11_ID3D11Device_CreateDepthStencilState));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRasterizerState, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState], (void*)D3D11_ID3D11Device_CreateRasterizerState));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateSamplerState, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateSamplerState], (void*)D3D11_ID3D11Device_CreateSamplerState));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateQuery, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateQuery], (void*)D3D11_ID3D11Device_CreateQuery));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePredicate, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreatePredicate], (void*)D3D11_ID3D11Device_CreatePredicate));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateCounter, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateCounter], (void*)D3D11_ID3D11Device_CreateCounter));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDeferredContext, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext], (void*)D3D11_ID3D11Device_CreateDeferredContext));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::OpenSharedResource, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::OpenSharedResource], (void*)D3D11_ID3D11Device_OpenSharedResource));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFormatSupport, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport], (void*)D3D11_ID3D11Device_CheckFormatSupport));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels], (void*)D3D11_ID3D11Device_CheckMultisampleQualityLevels));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounterInfo, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo], (void*)D3D11_ID3D11Device_CheckCounterInfo));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounter, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckCounter], (void*)D3D11_ID3D11Device_CheckCounter));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFeatureSupport, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport], (void*)D3D11_ID3D11Device_CheckFeatureSupport));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetPrivateData, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetPrivateData], (void*)D3D11_ID3D11Device_GetPrivateData));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateData, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateData], (void*)D3D11_ID3D11Device_SetPrivateData));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateDataInterface, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface], (void*)D3D11_ID3D11Device_SetPrivateDataInterface));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetFeatureLevel, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel], (void*)D3D11_ID3D11Device_GetFeatureLevel));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetCreationFlags, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetCreationFlags], (void*)D3D11_ID3D11Device_GetCreationFlags));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetDeviceRemovedReason, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason], (void*)D3D11_ID3D11Device_GetDeviceRemovedReason));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetImmediateContext, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetImmediateContext], (void*)D3D11_ID3D11Device_GetImmediateContext));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetExceptionMode, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::SetExceptionMode], (void*)D3D11_ID3D11Device_SetExceptionMode));
	pDCL_ID3D11Device->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetExceptionMode, (UINT_PTR)DetourFuncMinHook((void*)D3D11_ID3D11Device_VMTable[VMT_ID3D11DEVICE::GetExceptionMode], (void*)D3D11_ID3D11Device_GetExceptionMode));
}

#endif