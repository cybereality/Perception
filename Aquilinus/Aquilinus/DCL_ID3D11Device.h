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
#ifndef DCL_ID3D11DEVICE_CLASS
#define DCL_ID3D11DEVICE_CLASS

#include <d3d11.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include "VMT_ID3D11Device.h"
#include "AQU_TransferSite.h"

/**
* ID3D11Device detour class.
***/
class DCL_ID3D11Device
{
public:
	DCL_ID3D11Device(AQU_TransferSite* pcTransferSite);
	~DCL_ID3D11Device(void);

	/*** IUnknown methods ***/
	HRESULT           WINAPI QueryInterface(ID3D11Device *pcThis, REFIID riid, void **ppvObject);
	ULONG             WINAPI AddRef(ID3D11Device *pcThis);
	ULONG             WINAPI Release(ID3D11Device *pcThis);

	/*** ID3D11Device methods ***/
	HRESULT           WINAPI CreateBuffer(ID3D11Device *pcThis, const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer);
	HRESULT           WINAPI CreateTexture1D(ID3D11Device *pcThis, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D);
	HRESULT           WINAPI CreateTexture2D(ID3D11Device *pcThis, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D);
	HRESULT           WINAPI CreateTexture3D(ID3D11Device *pcThis, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D);
	HRESULT           WINAPI CreateShaderResourceView(ID3D11Device *pcThis, ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView);
	HRESULT           WINAPI CreateUnorderedAccessView(ID3D11Device *pcThis, ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView);
	HRESULT           WINAPI CreateRenderTargetView(ID3D11Device *pcThis, ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView);
	HRESULT           WINAPI CreateDepthStencilView(ID3D11Device *pcThis, ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView);
	HRESULT           WINAPI CreateInputLayout(ID3D11Device *pcThis, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout);
	HRESULT           WINAPI CreateVertexShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
	HRESULT           WINAPI CreateGeometryShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader);
	HRESULT           WINAPI CreateGeometryShaderWithStreamOutput(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader);
	HRESULT           WINAPI CreatePixelShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader);
	HRESULT           WINAPI CreateHullShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader);
	HRESULT           WINAPI CreateDomainShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader);
	HRESULT           WINAPI CreateComputeShader(ID3D11Device *pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader);
	HRESULT           WINAPI CreateClassLinkage(ID3D11Device *pcThis, ID3D11ClassLinkage **ppLinkage);
	HRESULT           WINAPI CreateBlendState(ID3D11Device *pcThis, const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState);
	HRESULT           WINAPI CreateDepthStencilState(ID3D11Device *pcThis, const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState);
	HRESULT           WINAPI CreateRasterizerState(ID3D11Device *pcThis, const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState);
	HRESULT           WINAPI CreateSamplerState(ID3D11Device *pcThis, const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState);
	HRESULT           WINAPI CreateQuery(ID3D11Device *pcThis, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
	HRESULT           WINAPI CreatePredicate(ID3D11Device *pcThis, const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate);
	HRESULT           WINAPI CreateCounter(ID3D11Device *pcThis, const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter);
	HRESULT           WINAPI CreateDeferredContext(ID3D11Device *pcThis, UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext);
	HRESULT           WINAPI OpenSharedResource(ID3D11Device *pcThis, HANDLE hResource, REFIID ReturnedInterface, void **ppResource);
	HRESULT           WINAPI CheckFormatSupport(ID3D11Device *pcThis, DXGI_FORMAT Format, UINT *pFormatSupport);
	HRESULT           WINAPI CheckMultisampleQualityLevels(ID3D11Device *pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels);
	void              WINAPI CheckCounterInfo(ID3D11Device *pcThis, D3D11_COUNTER_INFO *pCounterInfo);
	HRESULT           WINAPI CheckCounter(ID3D11Device *pcThis, const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength);
	HRESULT           WINAPI CheckFeatureSupport(ID3D11Device *pcThis, D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize);
	HRESULT           WINAPI GetPrivateData(ID3D11Device *pcThis, REFGUID guid, UINT *pDataSize, void *pData);
	HRESULT           WINAPI SetPrivateData(ID3D11Device *pcThis, REFGUID guid, UINT DataSize, const void *pData);
	HRESULT           WINAPI SetPrivateDataInterface(ID3D11Device *pcThis, REFGUID guid, const IUnknown *pData);
	D3D_FEATURE_LEVEL WINAPI GetFeatureLevel(ID3D11Device *pcThis);
	UINT              WINAPI GetCreationFlags(ID3D11Device *pcThis);
	HRESULT           WINAPI GetDeviceRemovedReason(ID3D11Device *pcThis);
	void              WINAPI GetImmediateContext(ID3D11Device *pcThis, ID3D11DeviceContext **ppImmediateContext);
	HRESULT           WINAPI SetExceptionMode(ID3D11Device *pcThis, UINT RaiseFlags);
	UINT              WINAPI GetExceptionMode(ID3D11Device *pcThis);

	/*** DCL_ID3D11Device public methods ***/
	HRESULT           SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT           SetSuperFunctionPointer(VMT_ID3D11DEVICE::VMT_ID3D11Device eFunc, UINT_PTR dwFunc);
	HRESULT           SetSuperFunctionPointers(PUINT_PTR pVMTable);
	void              CatchContextAndSwapChain(ID3D11Device* pcThis);
	HRESULT           GetDevice(ID3D11Device** ppcDevice);
	HRESULT           GetDeviceContext(ID3D11DeviceContext** ppcContext);
	HRESULT           GetDXGISwapChain(IDXGISwapChain** ppcSwapChain);
	void              ReleaseAllInterfaces();
	void              CreateSwapChainIfNecessary() { m_bCreateSwapChain = true; }

	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
	/**
	* The swapchain.
	***/
	IDXGISwapChain* m_pcSwapChain;
	/**
	* The device context.
	***/
	ID3D11DeviceContext* m_pcContext;
	/**
	* The device.
	***/
	ID3D11Device* m_pcDevice;
	/**
	* True if both context and swapchain pointer is catched.
	***/
	bool m_bPointersCatched;
	/**
	* True if swapchain should be created if necessary.
	***/
	bool m_bCreateSwapChain;
};

#endif

