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
#ifndef DMT_ID3D11DEVICE1_TABLE
#define DMT_ID3D11DEVICE1_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D11_ID3D11Device1_QueryInterface(ID3D11Device1* pcThis, REFIID riid, void** ppvObject) { return pDCL_ID3D11Device1->QueryInterface(pcThis, riid, ppvObject);}
ULONG WINAPI D3D11_ID3D11Device1_AddRef(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->AddRef(pcThis);}
ULONG WINAPI D3D11_ID3D11Device1_Release(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->Release(pcThis);}

/*** ID3D11Device methods ***/
HRESULT WINAPI D3D11_ID3D11Device1_CreateBuffer(ID3D11Device1* pcThis, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer) { return pDCL_ID3D11Device1->CreateBuffer(pcThis, pDesc, pInitialData, ppBuffer);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateTexture1D(ID3D11Device1* pcThis, D3D11_TEXTURE1D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture1D** ppTexture1D) { return pDCL_ID3D11Device1->CreateTexture1D(pcThis, pDesc, pInitialData, ppTexture1D);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateTexture2D(ID3D11Device1* pcThis, D3D11_TEXTURE2D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D) { return pDCL_ID3D11Device1->CreateTexture2D(pcThis, pDesc, pInitialData, ppTexture2D);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateTexture3D(ID3D11Device1* pcThis, D3D11_TEXTURE3D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture3D** ppTexture3D) { return pDCL_ID3D11Device1->CreateTexture3D(pcThis, pDesc, pInitialData, ppTexture3D);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateShaderResourceView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView) { return pDCL_ID3D11Device1->CreateShaderResourceView(pcThis, pResource, pDesc, ppSRView);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateUnorderedAccessView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView) { return pDCL_ID3D11Device1->CreateUnorderedAccessView(pcThis, pResource, pDesc, ppUAView);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateRenderTargetView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView) { return pDCL_ID3D11Device1->CreateRenderTargetView(pcThis, pResource, pDesc, ppRTView);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDepthStencilView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView) { return pDCL_ID3D11Device1->CreateDepthStencilView(pcThis, pResource, pDesc, ppDepthStencilView);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateInputLayout(ID3D11Device1* pcThis, D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout) { return pDCL_ID3D11Device1->CreateInputLayout(pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateVertexShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader) { return pDCL_ID3D11Device1->CreateVertexShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateGeometryShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader) { return pDCL_ID3D11Device1->CreateGeometryShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, UINT* pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader) { return pDCL_ID3D11Device1->CreateGeometryShaderWithStreamOutput(pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreatePixelShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader) { return pDCL_ID3D11Device1->CreatePixelShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateHullShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader) { return pDCL_ID3D11Device1->CreateHullShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDomainShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader) { return pDCL_ID3D11Device1->CreateDomainShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateComputeShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader) { return pDCL_ID3D11Device1->CreateComputeShader(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateClassLinkage(ID3D11Device1* pcThis, ID3D11ClassLinkage** ppLinkage) { return pDCL_ID3D11Device1->CreateClassLinkage(pcThis, ppLinkage);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateBlendState(ID3D11Device1* pcThis, D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState) { return pDCL_ID3D11Device1->CreateBlendState(pcThis, pBlendStateDesc, ppBlendState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDepthStencilState(ID3D11Device1* pcThis, D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState) { return pDCL_ID3D11Device1->CreateDepthStencilState(pcThis, pDepthStencilDesc, ppDepthStencilState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateRasterizerState(ID3D11Device1* pcThis, D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState) { return pDCL_ID3D11Device1->CreateRasterizerState(pcThis, pRasterizerDesc, ppRasterizerState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateSamplerState(ID3D11Device1* pcThis, D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState) { return pDCL_ID3D11Device1->CreateSamplerState(pcThis, pSamplerDesc, ppSamplerState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateQuery(ID3D11Device1* pcThis, D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery) { return pDCL_ID3D11Device1->CreateQuery(pcThis, pQueryDesc, ppQuery);}
HRESULT WINAPI D3D11_ID3D11Device1_CreatePredicate(ID3D11Device1* pcThis, D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate) { return pDCL_ID3D11Device1->CreatePredicate(pcThis, pPredicateDesc, ppPredicate);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateCounter(ID3D11Device1* pcThis, D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter) { return pDCL_ID3D11Device1->CreateCounter(pcThis, pCounterDesc, ppCounter);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDeferredContext(ID3D11Device1* pcThis, UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext) { return pDCL_ID3D11Device1->CreateDeferredContext(pcThis, ContextFlags, ppDeferredContext);}
HRESULT WINAPI D3D11_ID3D11Device1_OpenSharedResource(ID3D11Device1* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource) { return pDCL_ID3D11Device1->OpenSharedResource(pcThis, hResource, ReturnedInterface, ppResource);}
HRESULT WINAPI D3D11_ID3D11Device1_CheckFormatSupport(ID3D11Device1* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport) { return pDCL_ID3D11Device1->CheckFormatSupport(pcThis, Format, pFormatSupport);}
HRESULT WINAPI D3D11_ID3D11Device1_CheckMultisampleQualityLevels(ID3D11Device1* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels) { return pDCL_ID3D11Device1->CheckMultisampleQualityLevels(pcThis, Format, SampleCount, pNumQualityLevels);}
void WINAPI D3D11_ID3D11Device1_CheckCounterInfo(ID3D11Device1* pcThis, D3D11_COUNTER_INFO* pCounterInfo) { return pDCL_ID3D11Device1->CheckCounterInfo(pcThis, pCounterInfo);}
HRESULT WINAPI D3D11_ID3D11Device1_CheckCounter(ID3D11Device1* pcThis, D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength) { return pDCL_ID3D11Device1->CheckCounter(pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);}
HRESULT WINAPI D3D11_ID3D11Device1_CheckFeatureSupport(ID3D11Device1* pcThis, D3D11_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) { return pDCL_ID3D11Device1->CheckFeatureSupport(pcThis, Feature, pFeatureSupportData, FeatureSupportDataSize);}
HRESULT WINAPI D3D11_ID3D11Device1_GetPrivateData(ID3D11Device1* pcThis, REFGUID guid, UINT* pDataSize, void* pData) { return pDCL_ID3D11Device1->GetPrivateData(pcThis, guid, pDataSize, pData);}
HRESULT WINAPI D3D11_ID3D11Device1_SetPrivateData(ID3D11Device1* pcThis, REFGUID guid, UINT DataSize, void* pData) { return pDCL_ID3D11Device1->SetPrivateData(pcThis, guid, DataSize, pData);}
HRESULT WINAPI D3D11_ID3D11Device1_SetPrivateDataInterface(ID3D11Device1* pcThis, REFGUID guid, IUnknown* pData) { return pDCL_ID3D11Device1->SetPrivateDataInterface(pcThis, guid, pData);}
D3D_FEATURE_LEVEL WINAPI D3D11_ID3D11Device1_GetFeatureLevel(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->GetFeatureLevel(pcThis);}
UINT WINAPI D3D11_ID3D11Device1_GetCreationFlags(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->GetCreationFlags(pcThis);}
HRESULT WINAPI D3D11_ID3D11Device1_GetDeviceRemovedReason(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->GetDeviceRemovedReason(pcThis);}
void WINAPI D3D11_ID3D11Device1_GetImmediateContext(ID3D11Device1* pcThis, ID3D11DeviceContext** ppImmediateContext) { return pDCL_ID3D11Device1->GetImmediateContext(pcThis, ppImmediateContext);}
HRESULT WINAPI D3D11_ID3D11Device1_SetExceptionMode(ID3D11Device1* pcThis, UINT RaiseFlags) { return pDCL_ID3D11Device1->SetExceptionMode(pcThis, RaiseFlags);}
UINT WINAPI D3D11_ID3D11Device1_GetExceptionMode(ID3D11Device1* pcThis ) { return pDCL_ID3D11Device1->GetExceptionMode(pcThis);}

/*** ID3D11Device1 methods ***/
void WINAPI D3D11_ID3D11Device1_GetImmediateContext1(ID3D11Device1* pcThis, ID3D11DeviceContext1** ppImmediateContext) { return pDCL_ID3D11Device1->GetImmediateContext1(pcThis, ppImmediateContext);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDeferredContext1(ID3D11Device1* pcThis, UINT ContextFlags, ID3D11DeviceContext1** ppDeferredContext) { return pDCL_ID3D11Device1->CreateDeferredContext1(pcThis, ContextFlags, ppDeferredContext);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateBlendState1(ID3D11Device1* pcThis, D3D11_BLEND_DESC1* pBlendStateDesc, ID3D11BlendState1** ppBlendState) { return pDCL_ID3D11Device1->CreateBlendState1(pcThis, pBlendStateDesc, ppBlendState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateRasterizerState1(ID3D11Device1* pcThis, D3D11_RASTERIZER_DESC1* pRasterizerDesc, ID3D11RasterizerState1** ppRasterizerState) { return pDCL_ID3D11Device1->CreateRasterizerState1(pcThis, pRasterizerDesc, ppRasterizerState);}
HRESULT WINAPI D3D11_ID3D11Device1_CreateDeviceContextState(ID3D11Device1* pcThis, UINT Flags, D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface, D3D_FEATURE_LEVEL* pChosenFeatureLevel, ID3DDeviceContextState** ppContextState) { return pDCL_ID3D11Device1->CreateDeviceContextState(pcThis, Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);}
HRESULT WINAPI D3D11_ID3D11Device1_OpenSharedResource1(ID3D11Device1* pcThis, HANDLE hResource, REFIID returnedInterface, void** ppResource) { return pDCL_ID3D11Device1->OpenSharedResource1(pcThis, hResource, returnedInterface, ppResource);}
HRESULT WINAPI D3D11_ID3D11Device1_OpenSharedResourceByName(ID3D11Device1* pcThis, LPCWSTR lpName, DWORD dwDesiredAccess, REFIID returnedInterface, void** ppResource) { return pDCL_ID3D11Device1->OpenSharedResourceByName(pcThis, lpName, dwDesiredAccess, returnedInterface, ppResource);}


/**
* Overrides the Virtual Methods Table of ID3D11Device1
***/
void Override_D3D11_ID3D11Device1_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::QueryInterface], (UINT_PTR)D3D11_ID3D11Device1_QueryInterface);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::AddRef], (UINT_PTR)D3D11_ID3D11Device1_AddRef);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::Release], (UINT_PTR)D3D11_ID3D11Device1_Release);

	/*** ID3D11Device super methods ***/
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBuffer], (UINT_PTR)D3D11_ID3D11Device1_CreateBuffer);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture1D], (UINT_PTR)D3D11_ID3D11Device1_CreateTexture1D);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture2D], (UINT_PTR)D3D11_ID3D11Device1_CreateTexture2D);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture3D], (UINT_PTR)D3D11_ID3D11Device1_CreateTexture3D);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView], (UINT_PTR)D3D11_ID3D11Device1_CreateShaderResourceView);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView], (UINT_PTR)D3D11_ID3D11Device1_CreateUnorderedAccessView);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView], (UINT_PTR)D3D11_ID3D11Device1_CreateRenderTargetView);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView], (UINT_PTR)D3D11_ID3D11Device1_CreateDepthStencilView);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateInputLayout], (UINT_PTR)D3D11_ID3D11Device1_CreateInputLayout);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateVertexShader], (UINT_PTR)D3D11_ID3D11Device1_CreateVertexShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader], (UINT_PTR)D3D11_ID3D11Device1_CreateGeometryShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput], (UINT_PTR)D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePixelShader], (UINT_PTR)D3D11_ID3D11Device1_CreatePixelShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateHullShader], (UINT_PTR)D3D11_ID3D11Device1_CreateHullShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDomainShader], (UINT_PTR)D3D11_ID3D11Device1_CreateDomainShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateComputeShader], (UINT_PTR)D3D11_ID3D11Device1_CreateComputeShader);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage], (UINT_PTR)D3D11_ID3D11Device1_CreateClassLinkage);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBlendState], (UINT_PTR)D3D11_ID3D11Device1_CreateBlendState);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState], (UINT_PTR)D3D11_ID3D11Device1_CreateDepthStencilState);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState], (UINT_PTR)D3D11_ID3D11Device1_CreateRasterizerState);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateSamplerState], (UINT_PTR)D3D11_ID3D11Device1_CreateSamplerState);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateQuery], (UINT_PTR)D3D11_ID3D11Device1_CreateQuery);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePredicate], (UINT_PTR)D3D11_ID3D11Device1_CreatePredicate);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateCounter], (UINT_PTR)D3D11_ID3D11Device1_CreateCounter);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext], (UINT_PTR)D3D11_ID3D11Device1_CreateDeferredContext);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::OpenSharedResource], (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResource);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport], (UINT_PTR)D3D11_ID3D11Device1_CheckFormatSupport);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels], (UINT_PTR)D3D11_ID3D11Device1_CheckMultisampleQualityLevels);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo], (UINT_PTR)D3D11_ID3D11Device1_CheckCounterInfo);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounter], (UINT_PTR)D3D11_ID3D11Device1_CheckCounter);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport], (UINT_PTR)D3D11_ID3D11Device1_CheckFeatureSupport);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetPrivateData], (UINT_PTR)D3D11_ID3D11Device1_GetPrivateData);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateData], (UINT_PTR)D3D11_ID3D11Device1_SetPrivateData);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface], (UINT_PTR)D3D11_ID3D11Device1_SetPrivateDataInterface);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel], (UINT_PTR)D3D11_ID3D11Device1_GetFeatureLevel);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetCreationFlags], (UINT_PTR)D3D11_ID3D11Device1_GetCreationFlags);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason], (UINT_PTR)D3D11_ID3D11Device1_GetDeviceRemovedReason);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetImmediateContext], (UINT_PTR)D3D11_ID3D11Device1_GetImmediateContext);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetExceptionMode], (UINT_PTR)D3D11_ID3D11Device1_SetExceptionMode);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetExceptionMode], (UINT_PTR)D3D11_ID3D11Device1_GetExceptionMode);

	/*** ID3D11Device1 super methods ***/
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::GetImmediateContext1], (UINT_PTR)D3D11_ID3D11Device1_GetImmediateContext1);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeferredContext1], (UINT_PTR)D3D11_ID3D11Device1_CreateDeferredContext1);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateBlendState1], (UINT_PTR)D3D11_ID3D11Device1_CreateBlendState1);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateRasterizerState1], (UINT_PTR)D3D11_ID3D11Device1_CreateRasterizerState1);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeviceContextState], (UINT_PTR)D3D11_ID3D11Device1_CreateDeviceContextState);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResource1], (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResource1);
	OverrideVTable(&D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResourceByName], (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResourceByName);
}

/**
* Creates a Virtual Methods Table array of ID3D11Device1
***/
void Generate_D3D11_ID3D11Device1_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::QueryInterface] =  (UINT_PTR)D3D11_ID3D11Device1_QueryInterface;
	anD3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::AddRef] =  (UINT_PTR)D3D11_ID3D11Device1_AddRef;
	anD3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::Release] =  (UINT_PTR)D3D11_ID3D11Device1_Release;

	/*** ID3D11Device super methods ***/
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBuffer] =  (UINT_PTR)D3D11_ID3D11Device1_CreateBuffer;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture1D] =  (UINT_PTR)D3D11_ID3D11Device1_CreateTexture1D;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture2D] =  (UINT_PTR)D3D11_ID3D11Device1_CreateTexture2D;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture3D] =  (UINT_PTR)D3D11_ID3D11Device1_CreateTexture3D;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView] =  (UINT_PTR)D3D11_ID3D11Device1_CreateShaderResourceView;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView] =  (UINT_PTR)D3D11_ID3D11Device1_CreateUnorderedAccessView;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView] =  (UINT_PTR)D3D11_ID3D11Device1_CreateRenderTargetView;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDepthStencilView;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateInputLayout] =  (UINT_PTR)D3D11_ID3D11Device1_CreateInputLayout;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateVertexShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreateVertexShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreateGeometryShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput] =  (UINT_PTR)D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePixelShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreatePixelShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateHullShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreateHullShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDomainShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDomainShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateComputeShader] =  (UINT_PTR)D3D11_ID3D11Device1_CreateComputeShader;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage] =  (UINT_PTR)D3D11_ID3D11Device1_CreateClassLinkage;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBlendState] =  (UINT_PTR)D3D11_ID3D11Device1_CreateBlendState;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDepthStencilState;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState] =  (UINT_PTR)D3D11_ID3D11Device1_CreateRasterizerState;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateSamplerState] =  (UINT_PTR)D3D11_ID3D11Device1_CreateSamplerState;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateQuery] =  (UINT_PTR)D3D11_ID3D11Device1_CreateQuery;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePredicate] =  (UINT_PTR)D3D11_ID3D11Device1_CreatePredicate;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateCounter] =  (UINT_PTR)D3D11_ID3D11Device1_CreateCounter;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDeferredContext;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::OpenSharedResource] =  (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResource;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport] =  (UINT_PTR)D3D11_ID3D11Device1_CheckFormatSupport;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels] =  (UINT_PTR)D3D11_ID3D11Device1_CheckMultisampleQualityLevels;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo] =  (UINT_PTR)D3D11_ID3D11Device1_CheckCounterInfo;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounter] =  (UINT_PTR)D3D11_ID3D11Device1_CheckCounter;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport] =  (UINT_PTR)D3D11_ID3D11Device1_CheckFeatureSupport;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetPrivateData] =  (UINT_PTR)D3D11_ID3D11Device1_GetPrivateData;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateData] =  (UINT_PTR)D3D11_ID3D11Device1_SetPrivateData;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface] =  (UINT_PTR)D3D11_ID3D11Device1_SetPrivateDataInterface;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel] =  (UINT_PTR)D3D11_ID3D11Device1_GetFeatureLevel;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetCreationFlags] =  (UINT_PTR)D3D11_ID3D11Device1_GetCreationFlags;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason] =  (UINT_PTR)D3D11_ID3D11Device1_GetDeviceRemovedReason;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetImmediateContext] =  (UINT_PTR)D3D11_ID3D11Device1_GetImmediateContext;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetExceptionMode] =  (UINT_PTR)D3D11_ID3D11Device1_SetExceptionMode;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetExceptionMode] =  (UINT_PTR)D3D11_ID3D11Device1_GetExceptionMode;

	/*** ID3D11Device1 super methods ***/
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::GetImmediateContext1] =  (UINT_PTR)D3D11_ID3D11Device1_GetImmediateContext1;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeferredContext1] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDeferredContext1;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateBlendState1] =  (UINT_PTR)D3D11_ID3D11Device1_CreateBlendState1;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateRasterizerState1] =  (UINT_PTR)D3D11_ID3D11Device1_CreateRasterizerState1;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeviceContextState] =  (UINT_PTR)D3D11_ID3D11Device1_CreateDeviceContextState;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResource1] =  (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResource1;
	anD3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResourceByName] =  (UINT_PTR)D3D11_ID3D11Device1_OpenSharedResourceByName;
}

/**
* Overrides the Virtual Methods Table of ID3D11Device1 using a generated array.
***/
void Override_D3D11_ID3D11Device1_VMTable_by_Array()
{
	OverrideFullVTable(&D3D11_ID3D11Device1_VMTable[0],
		(void*)&anD3D11_ID3D11Device1_VMTable[0],
		(D3D11_ID3D11DEVICE1_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of ID3D11Device1
***/
void Detour_D3D11_ID3D11Device1VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::QueryInterface],  (PBYTE)D3D11_ID3D11Device1_QueryInterface, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::AddRef],  (PBYTE)D3D11_ID3D11Device1_AddRef, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_IUNKNOWN::Release],  (PBYTE)D3D11_ID3D11Device1_Release, JMP32_SZ));

	/*** ID3D11Device super methods ***/
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBuffer,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBuffer],  (PBYTE)D3D11_ID3D11Device1_CreateBuffer, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture1D,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture1D],  (PBYTE)D3D11_ID3D11Device1_CreateTexture1D, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture2D,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture2D],  (PBYTE)D3D11_ID3D11Device1_CreateTexture2D, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateTexture3D,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateTexture3D],  (PBYTE)D3D11_ID3D11Device1_CreateTexture3D, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateShaderResourceView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateShaderResourceView],  (PBYTE)D3D11_ID3D11Device1_CreateShaderResourceView, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateUnorderedAccessView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView],  (PBYTE)D3D11_ID3D11Device1_CreateUnorderedAccessView, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRenderTargetView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRenderTargetView],  (PBYTE)D3D11_ID3D11Device1_CreateRenderTargetView, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilView,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilView],  (PBYTE)D3D11_ID3D11Device1_CreateDepthStencilView, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateInputLayout,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateInputLayout],  (PBYTE)D3D11_ID3D11Device1_CreateInputLayout, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateVertexShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateVertexShader],  (PBYTE)D3D11_ID3D11Device1_CreateVertexShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShader],  (PBYTE)D3D11_ID3D11Device1_CreateGeometryShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput],  (PBYTE)D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePixelShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePixelShader],  (PBYTE)D3D11_ID3D11Device1_CreatePixelShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateHullShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateHullShader],  (PBYTE)D3D11_ID3D11Device1_CreateHullShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDomainShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDomainShader],  (PBYTE)D3D11_ID3D11Device1_CreateDomainShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateComputeShader,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateComputeShader],  (PBYTE)D3D11_ID3D11Device1_CreateComputeShader, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateClassLinkage,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateClassLinkage],  (PBYTE)D3D11_ID3D11Device1_CreateClassLinkage, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateBlendState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateBlendState],  (PBYTE)D3D11_ID3D11Device1_CreateBlendState, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDepthStencilState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDepthStencilState],  (PBYTE)D3D11_ID3D11Device1_CreateDepthStencilState, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateRasterizerState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateRasterizerState],  (PBYTE)D3D11_ID3D11Device1_CreateRasterizerState, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateSamplerState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateSamplerState],  (PBYTE)D3D11_ID3D11Device1_CreateSamplerState, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateQuery,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateQuery],  (PBYTE)D3D11_ID3D11Device1_CreateQuery, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreatePredicate,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreatePredicate],  (PBYTE)D3D11_ID3D11Device1_CreatePredicate, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateCounter,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateCounter],  (PBYTE)D3D11_ID3D11Device1_CreateCounter, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CreateDeferredContext,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CreateDeferredContext],  (PBYTE)D3D11_ID3D11Device1_CreateDeferredContext, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::OpenSharedResource,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::OpenSharedResource],  (PBYTE)D3D11_ID3D11Device1_OpenSharedResource, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFormatSupport,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFormatSupport],  (PBYTE)D3D11_ID3D11Device1_CheckFormatSupport, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels],  (PBYTE)D3D11_ID3D11Device1_CheckMultisampleQualityLevels, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounterInfo,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounterInfo],  (PBYTE)D3D11_ID3D11Device1_CheckCounterInfo, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckCounter,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckCounter],  (PBYTE)D3D11_ID3D11Device1_CheckCounter, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::CheckFeatureSupport,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::CheckFeatureSupport],  (PBYTE)D3D11_ID3D11Device1_CheckFeatureSupport, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetPrivateData],  (PBYTE)D3D11_ID3D11Device1_GetPrivateData, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateData,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateData],  (PBYTE)D3D11_ID3D11Device1_SetPrivateData, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetPrivateDataInterface,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface],  (PBYTE)D3D11_ID3D11Device1_SetPrivateDataInterface, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetFeatureLevel,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetFeatureLevel],  (PBYTE)D3D11_ID3D11Device1_GetFeatureLevel, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetCreationFlags,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetCreationFlags],  (PBYTE)D3D11_ID3D11Device1_GetCreationFlags, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetDeviceRemovedReason,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason],  (PBYTE)D3D11_ID3D11Device1_GetDeviceRemovedReason, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetImmediateContext,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetImmediateContext],  (PBYTE)D3D11_ID3D11Device1_GetImmediateContext, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::SetExceptionMode,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::SetExceptionMode],  (PBYTE)D3D11_ID3D11Device1_SetExceptionMode, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE::GetExceptionMode,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE::GetExceptionMode],  (PBYTE)D3D11_ID3D11Device1_GetExceptionMode, JMP32_SZ));

	/*** ID3D11Device1 super methods ***/
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::GetImmediateContext1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::GetImmediateContext1],  (PBYTE)D3D11_ID3D11Device1_GetImmediateContext1, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::CreateDeferredContext1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeferredContext1],  (PBYTE)D3D11_ID3D11Device1_CreateDeferredContext1, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::CreateBlendState1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateBlendState1],  (PBYTE)D3D11_ID3D11Device1_CreateBlendState1, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::CreateRasterizerState1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateRasterizerState1],  (PBYTE)D3D11_ID3D11Device1_CreateRasterizerState1, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::CreateDeviceContextState,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::CreateDeviceContextState],  (PBYTE)D3D11_ID3D11Device1_CreateDeviceContextState, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::OpenSharedResource1,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResource1],  (PBYTE)D3D11_ID3D11Device1_OpenSharedResource1, JMP32_SZ));
	pDCL_ID3D11Device1->SetSuperFunctionPointer(VMT_ID3D11DEVICE1::OpenSharedResourceByName,  (UINT_PTR)DetourFunc((PBYTE)D3D11_ID3D11Device1_VMTable[VMT_ID3D11DEVICE1::OpenSharedResourceByName],  (PBYTE)D3D11_ID3D11Device1_OpenSharedResourceByName, JMP32_SZ));
}

#endif

