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
#ifndef NOD_ID3D11DEVICE
#define NOD_ID3D11DEVICE

#include"VMT_ID3D11Device.h"

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_QueryInterface, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObject");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI AddRef
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_AddRef, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI Release
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_Release, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::Release");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D11Device methods
/**
* HRESULT WINAPI CreateBuffer
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateBuffer, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_BUFFER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11BUFFER, L"ppBuffer");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateBuffer);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture1D
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateTexture1D, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateTexture1D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_TEXTURE1D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11TEXTURE1D, L"ppTexture1D");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateTexture1D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture2D
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateTexture2D, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateTexture2D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_TEXTURE2D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11TEXTURE2D, L"ppTexture2D");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateTexture2D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture3D
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateTexture3D, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateTexture3D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_TEXTURE3D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11TEXTURE3D, L"ppTexture3D");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateTexture3D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateShaderResourceView
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateShaderResourceView, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateShaderResourceView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SHADER_RESOURCE_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11SHADERRESOURCEVIEW, L"ppSRView");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateShaderResourceView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateUnorderedAccessView
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateUnorderedAccessView, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateUnorderedAccessView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_UNORDERED_ACCESS_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11UNORDEREDACCESSVIEW, L"ppUAView");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateUnorderedAccessView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateRenderTargetView
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateRenderTargetView, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateRenderTargetView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_RENDER_TARGET_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11RENDERTARGETVIEW, L"ppRTView");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateRenderTargetView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDepthStencilView
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateDepthStencilView, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateDepthStencilView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_DEPTH_STENCIL_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11DEPTHSTENCILVIEW, L"ppDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateDepthStencilView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateInputLayout
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateInputLayout, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateInputLayout");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_INPUT_ELEMENT_DESC, L"pInputElementDescs");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumElements");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecodeWithInputSignature");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11INPUTLAYOUT, L"ppInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateInputLayout);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateVertexShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateVertexShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateVertexShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11VERTEXSHADER, L"ppVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateVertexShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateGeometryShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateGeometryShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateGeometryShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateGeometryShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateGeometryShaderWithStreamOutput, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateGeometryShaderWithStreamOutput");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SO_DECLARATION_ENTRY, L"pSODeclaration");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumEntries");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pBufferStrides");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumStrides");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"RasterizedStream");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreatePixelShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreatePixelShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreatePixelShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11PIXELSHADER, L"ppPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreatePixelShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateHullShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateHullShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateHullShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11HULLSHADER, L"ppHullShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateHullShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDomainShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateDomainShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateDomainShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11DOMAINSHADER, L"ppDomainShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateDomainShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateComputeShader
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateComputeShader, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateComputeShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D11CLASSLINKAGE, L"pClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11COMPUTESHADER, L"ppComputeShader");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateComputeShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateClassLinkage
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateClassLinkage, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateClassLinkage");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11CLASSLINKAGE, L"ppLinkage");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateClassLinkage);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateBlendState
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateBlendState, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateBlendState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_BLEND_DESC, L"pBlendStateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11BLENDSTATE, L"ppBlendState");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateBlendState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDepthStencilState
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateDepthStencilState, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_DEPTH_STENCIL_DESC, L"pDepthStencilDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11DEPTHSTENCILSTATE, L"ppDepthStencilState");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateDepthStencilState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateRasterizerState
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateRasterizerState, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateRasterizerState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_RASTERIZER_DESC, L"pRasterizerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11RASTERIZERSTATE, L"ppRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateRasterizerState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateSamplerState
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateSamplerState, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateSamplerState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_SAMPLER_DESC, L"pSamplerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11SAMPLERSTATE, L"ppSamplerState");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateSamplerState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateQuery
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateQuery, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateQuery");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_QUERY_DESC, L"pQueryDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11QUERY, L"ppQuery");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateQuery);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreatePredicate
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreatePredicate, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreatePredicate");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_QUERY_DESC, L"pPredicateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11PREDICATE, L"ppPredicate");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreatePredicate);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateCounter
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateCounter, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateCounter");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_COUNTER_DESC, L"pCounterDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11COUNTER, L"ppCounter");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateCounter);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDeferredContext
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CreateDeferredContext, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CreateDeferredContext");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"ContextFlags");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11DEVICECONTEXT, L"ppDeferredContext");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CreateDeferredContext);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI OpenSharedResource
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_OpenSharedResource, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::OpenSharedResource");
AQU_D3D_NODE_COMMANDER(AQU_HANDLE, L"hResource");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"ReturnedInterface");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppResource");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::OpenSharedResource);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckFormatSupport
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CheckFormatSupport, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CheckFormatSupport");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pFormatSupport");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CheckFormatSupport);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckMultisampleQualityLevels
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CheckMultisampleQualityLevels, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CheckMultisampleQualityLevels");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SampleCount");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNumQualityLevels");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CheckMultisampleQualityLevels);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI CheckCounterInfo
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CheckCounterInfo, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CheckCounterInfo");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_COUNTER_INFO, L"pCounterInfo");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CheckCounterInfo);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckCounter
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CheckCounter, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CheckCounter");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_COUNTER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D11_COUNTER_TYPE, L"pType");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pActiveCounters");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szName");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNameLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szUnits");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pUnitsLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szDescription");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDescriptionLength");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CheckCounter);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckFeatureSupport
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_CheckFeatureSupport, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::CheckFeatureSupport");
AQU_D3D_NODE_COMMANDER(AQU_D3D11_FEATURE, L"Feature");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pFeatureSupportData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"FeatureSupportDataSize");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::CheckFeatureSupport);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetPrivateData
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetPrivateData, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetPrivateData
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_SetPrivateData, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::SetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::SetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetPrivateDataInterface
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_SetPrivateDataInterface, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::SetPrivateDataInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IUNKNOWN, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::SetPrivateDataInterface);
AQU_D3D_NODE_FOOTER___;

/**
* D3D_FEATURE_LEVEL WINAPI GetFeatureLevel
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetFeatureLevel, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetFeatureLevel");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetFeatureLevel);
AQU_D3D_NODE_FOOTER___;

/**
* UINT WINAPI GetCreationFlags
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetCreationFlags, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetCreationFlags");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetCreationFlags);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetDeviceRemovedReason
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetDeviceRemovedReason, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetDeviceRemovedReason");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetDeviceRemovedReason);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GetImmediateContext
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetImmediateContext, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetImmediateContext");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D11DEVICECONTEXT, L"ppImmediateContext");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetImmediateContext);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetExceptionMode
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_SetExceptionMode, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::SetExceptionMode");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"RaiseFlags");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::SetExceptionMode);
AQU_D3D_NODE_FOOTER___;

/**
* UINT WINAPI GetExceptionMode
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D11Device_GetExceptionMode, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D11Device::GetExceptionMode");
AQU_D3D_NODE_METHOD___(DirectX_11, ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device, VMT_ID3D11DEVICE::GetExceptionMode);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D11Device node provider method

/**
* Node provider class.
***/
class NOD_ID3D11Device
{
public:
	/**
	* Provides ID3D11Device node by index.
	***/
	NOD_Basic* Get_ID3D11Device_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_ID3D11DEVICE::VMT_ID3D11Device::GetExceptionMode) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_ID3D11Device_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_ID3D11Device_Node((VMT_ID3D11DEVICE::VMT_ID3D11Device)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_ID3D11Device_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::VMT_IUnknown::QueryInterface:
			return (NOD_Basic*)new NOD_ID3D11Device_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::AddRef:
			return (NOD_Basic*)new NOD_ID3D11Device_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::Release:
			return (NOD_Basic*)new NOD_ID3D11Device_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides ID3D11Device node by enumeration.
	***/
	NOD_Basic* Get_ID3D11Device_Node(VMT_ID3D11DEVICE::VMT_ID3D11Device eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateBuffer:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateBuffer(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateTexture1D:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateTexture1D(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateTexture2D:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateTexture2D(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateTexture3D:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateTexture3D(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateShaderResourceView:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateShaderResourceView(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateUnorderedAccessView:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateUnorderedAccessView(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateRenderTargetView:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateRenderTargetView(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateDepthStencilView:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateDepthStencilView(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateInputLayout:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateInputLayout(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateVertexShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateVertexShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateGeometryShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateGeometryShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateGeometryShaderWithStreamOutput:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateGeometryShaderWithStreamOutput(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreatePixelShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreatePixelShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateHullShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateHullShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateDomainShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateDomainShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateComputeShader:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateComputeShader(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateClassLinkage:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateClassLinkage(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateBlendState:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateBlendState(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateDepthStencilState:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateDepthStencilState(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateRasterizerState:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateRasterizerState(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateSamplerState:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateSamplerState(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateQuery:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateQuery(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreatePredicate:
			return (NOD_Basic*)new NOD_ID3D11Device_CreatePredicate(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateCounter:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateCounter(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CreateDeferredContext:
			return (NOD_Basic*)new NOD_ID3D11Device_CreateDeferredContext(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::OpenSharedResource:
			return (NOD_Basic*)new NOD_ID3D11Device_OpenSharedResource(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CheckFormatSupport:
			return (NOD_Basic*)new NOD_ID3D11Device_CheckFormatSupport(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CheckMultisampleQualityLevels:
			return (NOD_Basic*)new NOD_ID3D11Device_CheckMultisampleQualityLevels(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CheckCounterInfo:
			return (NOD_Basic*)new NOD_ID3D11Device_CheckCounterInfo(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CheckCounter:
			return (NOD_Basic*)new NOD_ID3D11Device_CheckCounter(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::CheckFeatureSupport:
			return (NOD_Basic*)new NOD_ID3D11Device_CheckFeatureSupport(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetPrivateData:
			return (NOD_Basic*)new NOD_ID3D11Device_GetPrivateData(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::SetPrivateData:
			return (NOD_Basic*)new NOD_ID3D11Device_SetPrivateData(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::SetPrivateDataInterface:
			return (NOD_Basic*)new NOD_ID3D11Device_SetPrivateDataInterface(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetFeatureLevel:
			return (NOD_Basic*)new NOD_ID3D11Device_GetFeatureLevel(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetCreationFlags:
			return (NOD_Basic*)new NOD_ID3D11Device_GetCreationFlags(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetDeviceRemovedReason:
			return (NOD_Basic*)new NOD_ID3D11Device_GetDeviceRemovedReason(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetImmediateContext:
			return (NOD_Basic*)new NOD_ID3D11Device_GetImmediateContext(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::SetExceptionMode:
			return (NOD_Basic*)new NOD_ID3D11Device_SetExceptionMode(nX, nY);
			break;
		case VMT_ID3D11DEVICE::VMT_ID3D11Device::GetExceptionMode:
			return (NOD_Basic*)new NOD_ID3D11Device_GetExceptionMode(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

};
#pragma endregion

#endif
