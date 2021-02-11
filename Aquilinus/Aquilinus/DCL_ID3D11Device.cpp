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
#ifndef _TRACE
#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#endif

#ifdef _DEBUG
#include <windows.h>
#include <tchar.h>
#include <sstream>
typedef std::basic_stringstream<TCHAR> tstringstream;
template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }
#define OUTPUT_DEBUG_STRING_ID3D11Device(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_ID3D11Device(...)
#endif
#define AQU_ID3D11DEVICE_HEADER________(a) DWORD dwId = GetCurrentThreadId(); auto it = std::find(m_pcTransferSite->m_adwCurrentThreadIds.begin(), m_pcTransferSite->m_adwCurrentThreadIds.end(), dwId); if ((it == m_pcTransferSite->m_adwCurrentThreadIds.end()) && ((dwId != m_pcTransferSite->m_dwMainThreadId) || (!m_pcTransferSite->m_bForceD3D)) && (m_pcTransferSite->m_ppNOD_ID3D11Device[a]) && (m_pcTransferSite->m_ppNOD_ID3D11Device[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_ID3D11DEVICE_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_ID3D11Device[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_ID3D11DEVICE_PROVOKE_______(a) it = std::find(m_pcTransferSite->m_adwCurrentThreadIds.begin(), m_pcTransferSite->m_adwCurrentThreadIds.end(), 0); *it = dwId; void* pvRet = m_pcTransferSite->m_ppNOD_ID3D11Device[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); *it = 0;
#define AQU_ID3D11DEVICE_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11Device[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_ID3D11DEVICE_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11Device[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }
#include"DCL_ID3D11Device.h"
#include"DCL_ID3D11Device_Super.h"

#pragma region DCL_ID3D11Device constructor/destructor
/**
* Constructor
***/
DCL_ID3D11Device::DCL_ID3D11Device(AQU_TransferSite* pcTransferSite) :
m_pcTransferSite(pcTransferSite),
m_pcContext(nullptr),
m_pcSwapChain(nullptr),
m_bPointersCatched(false),
m_bCreateSwapChain(false)
{
}
/**
* Destructor
***/
DCL_ID3D11Device::~DCL_ID3D11Device()
{
	if (m_pcSwapChain) { m_pcSwapChain->Release(); m_pcSwapChain = nullptr; }
	if (m_pcContext) { m_pcContext->Release(); m_pcContext = nullptr; }
}
#pragma endregion

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface
***/
HRESULT WINAPI DCL_ID3D11Device::QueryInterface(ID3D11Device* pcThis, REFIID riid, void** ppvObject)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::QueryInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	/*AQU_ID3D11DEVICE_HEADER________(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_IUNKNOWN::QueryInterface,0,riid);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_IUNKNOWN::QueryInterface,1,ppvObject);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_IUNKNOWN::QueryInterface, HRESULT);*/

	return D3D11_ID3D11Device_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* ULONG WINAPI AddRef
***/
ULONG WINAPI DCL_ID3D11Device::AddRef(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::AddRef");

	// set reference counter in the transfer site class
	int iCount = D3D11_ID3D11Device_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] = iCount;

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	/*AQU_ID3D11DEVICE_HEADER________(VMT_IUNKNOWN::AddRef);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_IUNKNOWN::AddRef);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_IUNKNOWN::AddRef, ULONG);*/

	return iCount;
}

/**
* ULONG WINAPI Release
***/
ULONG WINAPI DCL_ID3D11Device::Release(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::Release");

	/*// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_IUNKNOWN::Release);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_IUNKNOWN::Release);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_IUNKNOWN::Release, ULONG);*/

	// set reference counter in the transfer site class
	int iCount = D3D11_ID3D11Device_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device] = iCount;
	return iCount;
}

#pragma endregion

#pragma region ID3D11Device methods
/**
* HRESULT WINAPI CreateBuffer
***/
HRESULT WINAPI DCL_ID3D11Device::CreateBuffer(ID3D11Device *pcThis, const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateBuffer");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateBuffer);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer, 0, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer, 1, pInitialData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer, 2, ppBuffer);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateBuffer);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateBuffer, HRESULT);

	return D3D11_ID3D11Device_CreateBuffer_Super(pcThis, pDesc, pInitialData, ppBuffer);
}

/**
* HRESULT WINAPI CreateTexture1D
***/
HRESULT WINAPI DCL_ID3D11Device::CreateTexture1D(ID3D11Device *pcThis, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateTexture1D");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateTexture1D);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D, 0, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D, 1, pInitialData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D, 2, ppTexture1D);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture1D);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture1D, HRESULT);

	return D3D11_ID3D11Device_CreateTexture1D_Super(pcThis, pDesc, pInitialData, ppTexture1D);
}

/**
* HRESULT WINAPI CreateTexture2D
***/
HRESULT WINAPI DCL_ID3D11Device::CreateTexture2D(ID3D11Device *pcThis, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateTexture2D");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateTexture2D);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D, 0, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D, 1, pInitialData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D, 2, ppTexture2D);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture2D);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture2D, HRESULT);

	return D3D11_ID3D11Device_CreateTexture2D_Super(pcThis, pDesc, pInitialData, ppTexture2D);
}

/**
* HRESULT WINAPI CreateTexture3D
***/
HRESULT WINAPI DCL_ID3D11Device::CreateTexture3D(ID3D11Device *pcThis, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateTexture3D");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateTexture3D);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D, 0, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D, 1, pInitialData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D, 2, ppTexture3D);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture3D);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture3D, HRESULT);

	return D3D11_ID3D11Device_CreateTexture3D_Super(pcThis, pDesc, pInitialData, ppTexture3D);
}

/**
* HRESULT WINAPI CreateShaderResourceView
***/
HRESULT WINAPI DCL_ID3D11Device::CreateShaderResourceView(ID3D11Device* pcThis, ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateShaderResourceView");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateShaderResourceView);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView, 0, pResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView, 1, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView, 2, ppSRView);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateShaderResourceView);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateShaderResourceView, HRESULT);

	return D3D11_ID3D11Device_CreateShaderResourceView_Super(pcThis, pResource, pDesc, ppSRView);
}

/**
* HRESULT WINAPI CreateUnorderedAccessView
***/
HRESULT WINAPI DCL_ID3D11Device::CreateUnorderedAccessView(ID3D11Device* pcThis, ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateUnorderedAccessView");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateUnorderedAccessView);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView, 0, pResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView, 1, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView, 2, ppUAView);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateUnorderedAccessView);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateUnorderedAccessView, HRESULT);

	return D3D11_ID3D11Device_CreateUnorderedAccessView_Super(pcThis, pResource, pDesc, ppUAView);
}

/**
* HRESULT WINAPI CreateRenderTargetView
***/
HRESULT WINAPI DCL_ID3D11Device::CreateRenderTargetView(ID3D11Device* pcThis, ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateRenderTargetView");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateRenderTargetView);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView, 0, pResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView, 1, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView, 2, ppRTView);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateRenderTargetView);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateRenderTargetView, HRESULT);

	return D3D11_ID3D11Device_CreateRenderTargetView_Super(pcThis, pResource, pDesc, ppRTView);
}

/**
* HRESULT WINAPI CreateDepthStencilView
***/
HRESULT WINAPI DCL_ID3D11Device::CreateDepthStencilView(ID3D11Device* pcThis, ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateDepthStencilView");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateDepthStencilView);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView, 0, pResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView, 1, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView, 2, ppDepthStencilView);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateDepthStencilView);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDepthStencilView, HRESULT);

	return D3D11_ID3D11Device_CreateDepthStencilView_Super(pcThis, pResource, pDesc, ppDepthStencilView);
}

/**
* HRESULT WINAPI CreateInputLayout
***/
HRESULT WINAPI DCL_ID3D11Device::CreateInputLayout(ID3D11Device* pcThis, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateInputLayout");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateInputLayout);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout, 0, pInputElementDescs);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout, 1, NumElements);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout, 2, pShaderBytecodeWithInputSignature);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout, 3, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout, 4, ppInputLayout);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateInputLayout);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateInputLayout, HRESULT);

	return D3D11_ID3D11Device_CreateInputLayout_Super(pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

/**
* HRESULT WINAPI CreateVertexShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreateVertexShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateVertexShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateVertexShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader, 3, ppVertexShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateVertexShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateVertexShader, HRESULT);

	return D3D11_ID3D11Device_CreateVertexShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

/**
* HRESULT WINAPI CreateGeometryShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreateGeometryShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateGeometryShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateGeometryShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader, 3, ppGeometryShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateGeometryShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateGeometryShader, HRESULT);

	return D3D11_ID3D11Device_CreateGeometryShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
}

/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput
***/
HRESULT WINAPI DCL_ID3D11Device::CreateGeometryShaderWithStreamOutput(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateGeometryShaderWithStreamOutput");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 2, pSODeclaration);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 3, NumEntries);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 4, pBufferStrides);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 5, NumStrides);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 6, RasterizedStream);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 7, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, 8, ppGeometryShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, HRESULT);

	return D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput_Super(pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
}

/**
* HRESULT WINAPI CreatePixelShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreatePixelShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreatePixelShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreatePixelShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader, 3, ppPixelShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreatePixelShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreatePixelShader, HRESULT);

	return D3D11_ID3D11Device_CreatePixelShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

/**
* HRESULT WINAPI CreateHullShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreateHullShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateHullShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateHullShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader, 3, ppHullShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateHullShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateHullShader, HRESULT);

	return D3D11_ID3D11Device_CreateHullShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
}

/**
* HRESULT WINAPI CreateDomainShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreateDomainShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateDomainShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateDomainShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader, 3, ppDomainShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateDomainShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDomainShader, HRESULT);

	return D3D11_ID3D11Device_CreateDomainShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
}

/**
* HRESULT WINAPI CreateComputeShader
***/
HRESULT WINAPI DCL_ID3D11Device::CreateComputeShader(ID3D11Device* pcThis, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateComputeShader");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateComputeShader);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader, 0, pShaderBytecode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader, 1, BytecodeLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader, 2, pClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader, 3, ppComputeShader);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateComputeShader);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateComputeShader, HRESULT);

	return D3D11_ID3D11Device_CreateComputeShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
}

/**
* HRESULT WINAPI CreateClassLinkage
***/
HRESULT WINAPI DCL_ID3D11Device::CreateClassLinkage(ID3D11Device* pcThis, ID3D11ClassLinkage** ppLinkage)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateClassLinkage");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateClassLinkage);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateClassLinkage, 0, ppLinkage);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateClassLinkage);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateClassLinkage, HRESULT);

	return D3D11_ID3D11Device_CreateClassLinkage_Super(pcThis, ppLinkage);
}

/**
* HRESULT WINAPI CreateBlendState
***/
HRESULT WINAPI DCL_ID3D11Device::CreateBlendState(ID3D11Device* pcThis, const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateBlendState");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateBlendState);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateBlendState, 0, pBlendStateDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateBlendState, 1, ppBlendState);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateBlendState);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateBlendState, HRESULT);

	return D3D11_ID3D11Device_CreateBlendState_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
* HRESULT WINAPI CreateDepthStencilState
***/
HRESULT WINAPI DCL_ID3D11Device::CreateDepthStencilState(ID3D11Device* pcThis, const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateDepthStencilState");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateDepthStencilState);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilState, 0, pDepthStencilDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilState, 1, ppDepthStencilState);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateDepthStencilState);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDepthStencilState, HRESULT);

	return D3D11_ID3D11Device_CreateDepthStencilState_Super(pcThis, pDepthStencilDesc, ppDepthStencilState);
}

/**
* HRESULT WINAPI CreateRasterizerState
***/
HRESULT WINAPI DCL_ID3D11Device::CreateRasterizerState(ID3D11Device* pcThis, const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateRasterizerState");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateRasterizerState);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateRasterizerState, 0, pRasterizerDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateRasterizerState, 1, ppRasterizerState);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateRasterizerState);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateRasterizerState, HRESULT);

	return D3D11_ID3D11Device_CreateRasterizerState_Super(pcThis, pRasterizerDesc, ppRasterizerState);
}

/**
* HRESULT WINAPI CreateSamplerState
***/
HRESULT WINAPI DCL_ID3D11Device::CreateSamplerState(ID3D11Device* pcThis, const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateSamplerState");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateSamplerState);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateSamplerState, 0, pSamplerDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateSamplerState, 1, ppSamplerState);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateSamplerState);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateSamplerState, HRESULT);

	return D3D11_ID3D11Device_CreateSamplerState_Super(pcThis, pSamplerDesc, ppSamplerState);
}

/**
* HRESULT WINAPI CreateQuery
***/
HRESULT WINAPI DCL_ID3D11Device::CreateQuery(ID3D11Device* pcThis, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateQuery");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateQuery);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateQuery, 0, pQueryDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateQuery, 1, ppQuery);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateQuery);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateQuery, HRESULT);

	return D3D11_ID3D11Device_CreateQuery_Super(pcThis, pQueryDesc, ppQuery);
}

/**
* HRESULT WINAPI CreatePredicate
***/
HRESULT WINAPI DCL_ID3D11Device::CreatePredicate(ID3D11Device* pcThis, const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreatePredicate");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreatePredicate);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePredicate, 0, pPredicateDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreatePredicate, 1, ppPredicate);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreatePredicate);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreatePredicate, HRESULT);

	return D3D11_ID3D11Device_CreatePredicate_Super(pcThis, pPredicateDesc, ppPredicate);
}

/**
* HRESULT WINAPI CreateCounter
***/
HRESULT WINAPI DCL_ID3D11Device::CreateCounter(ID3D11Device* pcThis, const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateCounter");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateCounter);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateCounter, 0, pCounterDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateCounter, 1, ppCounter);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateCounter);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateCounter, HRESULT);

	return D3D11_ID3D11Device_CreateCounter_Super(pcThis, pCounterDesc, ppCounter);
}

/**
* HRESULT WINAPI CreateDeferredContext
***/
HRESULT WINAPI DCL_ID3D11Device::CreateDeferredContext(ID3D11Device* pcThis, UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CreateDeferredContext");

	if (!m_bPointersCatched)
		CatchContextAndSwapChain(pcThis);

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CreateDeferredContext);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDeferredContext, 0, ContextFlags);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CreateDeferredContext, 1, ppDeferredContext);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CreateDeferredContext);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDeferredContext, HRESULT);

	return D3D11_ID3D11Device_CreateDeferredContext_Super(pcThis, ContextFlags, ppDeferredContext);
}

/**
* HRESULT WINAPI OpenSharedResource
***/
HRESULT WINAPI DCL_ID3D11Device::OpenSharedResource(ID3D11Device* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::OpenSharedResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::OpenSharedResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource, 0, hResource);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource, 1, ReturnedInterface);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource, 2, ppResource);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::OpenSharedResource);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::OpenSharedResource, HRESULT);

	return D3D11_ID3D11Device_OpenSharedResource_Super(pcThis, hResource, ReturnedInterface, ppResource);
}

/**
* HRESULT WINAPI CheckFormatSupport
***/
HRESULT WINAPI DCL_ID3D11Device::CheckFormatSupport(ID3D11Device* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CheckFormatSupport");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CheckFormatSupport);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckFormatSupport, 0, Format);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckFormatSupport, 1, pFormatSupport);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CheckFormatSupport);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckFormatSupport, HRESULT);

	return D3D11_ID3D11Device_CheckFormatSupport_Super(pcThis, Format, pFormatSupport);
}

/**
* HRESULT WINAPI CheckMultisampleQualityLevels
***/
HRESULT WINAPI DCL_ID3D11Device::CheckMultisampleQualityLevels(ID3D11Device* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CheckMultisampleQualityLevels");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, 0, Format);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, 1, SampleCount);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, 2, pNumQualityLevels);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, HRESULT);

	return D3D11_ID3D11Device_CheckMultisampleQualityLevels_Super(pcThis, Format, SampleCount, pNumQualityLevels);
}

/**
* void WINAPI CheckCounterInfo
***/
void WINAPI DCL_ID3D11Device::CheckCounterInfo(ID3D11Device* pcThis, D3D11_COUNTER_INFO* pCounterInfo)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CheckCounterInfo");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CheckCounterInfo);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounterInfo, 0, pCounterInfo);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CheckCounterInfo);
	AQU_ID3D11DEVICE_REPLACE_VOID__(VMT_ID3D11DEVICE::CheckCounterInfo);

	D3D11_ID3D11Device_CheckCounterInfo_Super(pcThis, pCounterInfo);
}

/**
* HRESULT WINAPI CheckCounter
***/
HRESULT WINAPI DCL_ID3D11Device::CheckCounter(ID3D11Device* pcThis, const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CheckCounter");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CheckCounter);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 0, pDesc);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 1, pType);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 2, pActiveCounters);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 3, szName);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 4, pNameLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 5, szUnits);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 6, pUnitsLength);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 7, szDescription);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter, 8, pDescriptionLength);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CheckCounter);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckCounter, HRESULT);

	return D3D11_ID3D11Device_CheckCounter_Super(pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

/**
* HRESULT WINAPI CheckFeatureSupport
***/
HRESULT WINAPI DCL_ID3D11Device::CheckFeatureSupport(ID3D11Device* pcThis, D3D11_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::CheckFeatureSupport");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::CheckFeatureSupport);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport, 0, Feature);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport, 1, pFeatureSupportData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport, 2, FeatureSupportDataSize);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::CheckFeatureSupport);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckFeatureSupport, HRESULT);

	return D3D11_ID3D11Device_CheckFeatureSupport_Super(pcThis, Feature, pFeatureSupportData, FeatureSupportDataSize);
}

/**
* HRESULT WINAPI GetPrivateData
***/
HRESULT WINAPI DCL_ID3D11Device::GetPrivateData(ID3D11Device* pcThis, REFGUID guid, UINT* pDataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetPrivateData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData, 0, guid);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData, 1, pDataSize);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData, 2, pData);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetPrivateData);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::GetPrivateData, HRESULT);

	return D3D11_ID3D11Device_GetPrivateData_Super(pcThis, guid, pDataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateData
***/
HRESULT WINAPI DCL_ID3D11Device::SetPrivateData(ID3D11Device* pcThis, REFGUID guid, UINT DataSize, const void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::SetPrivateData);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData, 0, guid);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData, 1, DataSize);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData, 2, pData);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::SetPrivateData);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::SetPrivateData, HRESULT);

	return D3D11_ID3D11Device_SetPrivateData_Super(pcThis, guid, DataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateDataInterface
***/
HRESULT WINAPI DCL_ID3D11Device::SetPrivateDataInterface(ID3D11Device* pcThis, REFGUID guid, const IUnknown* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::SetPrivateDataInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::SetPrivateDataInterface);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateDataInterface, 0, guid);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateDataInterface, 1, pData);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::SetPrivateDataInterface);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::SetPrivateDataInterface, HRESULT);

	return D3D11_ID3D11Device_SetPrivateDataInterface_Super(pcThis, guid, pData);
}

/**
* D3D_FEATURE_LEVEL WINAPI GetFeatureLevel
***/
D3D_FEATURE_LEVEL WINAPI DCL_ID3D11Device::GetFeatureLevel(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetFeatureLevel");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetFeatureLevel);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetFeatureLevel);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::GetFeatureLevel, D3D_FEATURE_LEVEL);

	return D3D11_ID3D11Device_GetFeatureLevel_Super(pcThis);
}

/**
* UINT WINAPI GetCreationFlags
***/
UINT WINAPI DCL_ID3D11Device::GetCreationFlags(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetCreationFlags");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetCreationFlags);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetCreationFlags);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::GetCreationFlags, UINT);

	return D3D11_ID3D11Device_GetCreationFlags_Super(pcThis);
}

/**
* HRESULT WINAPI GetDeviceRemovedReason
***/
HRESULT WINAPI DCL_ID3D11Device::GetDeviceRemovedReason(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetDeviceRemovedReason");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetDeviceRemovedReason);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetDeviceRemovedReason);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::GetDeviceRemovedReason, HRESULT);

	return D3D11_ID3D11Device_GetDeviceRemovedReason_Super(pcThis);
}

/**
* void WINAPI GetImmediateContext
***/
void WINAPI DCL_ID3D11Device::GetImmediateContext(ID3D11Device* pcThis, ID3D11DeviceContext** ppImmediateContext)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetImmediateContext");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetImmediateContext);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::GetImmediateContext, 0, ppImmediateContext);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetImmediateContext);
	AQU_ID3D11DEVICE_REPLACE_VOID__(VMT_ID3D11DEVICE::GetImmediateContext);

	D3D11_ID3D11Device_GetImmediateContext_Super(pcThis, ppImmediateContext);
}

/**
* HRESULT WINAPI SetExceptionMode
***/
HRESULT WINAPI DCL_ID3D11Device::SetExceptionMode(ID3D11Device* pcThis, UINT RaiseFlags)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::SetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::SetExceptionMode);
	AQU_ID3D11DEVICE_SET_DATA______(VMT_ID3D11DEVICE::SetExceptionMode, 0, RaiseFlags);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::SetExceptionMode);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::SetExceptionMode, HRESULT);

	return D3D11_ID3D11Device_SetExceptionMode_Super(pcThis, RaiseFlags);
}

/**
* UINT WINAPI GetExceptionMode
***/
UINT WINAPI DCL_ID3D11Device::GetExceptionMode(ID3D11Device* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11Device(L"ID3D11Device::GetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICE_HEADER________(VMT_ID3D11DEVICE::GetExceptionMode);
	AQU_ID3D11DEVICE_PROVOKE_______(VMT_ID3D11DEVICE::GetExceptionMode);
	AQU_ID3D11DEVICE_REPLACE_METHOD(VMT_ID3D11DEVICE::GetExceptionMode, UINT);

	return D3D11_ID3D11Device_GetExceptionMode_Super(pcThis);
}

#pragma endregion

#pragma region DCL_ID3D11Device public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11Device.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11Device::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IUnknown super methods ***/
		case VMT_IUNKNOWN::QueryInterface:*(PUINT_PTR)&D3D11_ID3D11Device_QueryInterface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::AddRef:*(PUINT_PTR)&D3D11_ID3D11Device_AddRef_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::Release:*(PUINT_PTR)&D3D11_ID3D11Device_Release_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11Device.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11Device::SetSuperFunctionPointer(VMT_ID3D11DEVICE::VMT_ID3D11Device eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** ID3D11Device super methods ***/
		case VMT_ID3D11DEVICE::CreateBuffer:*(PUINT_PTR)&D3D11_ID3D11Device_CreateBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateTexture1D:*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture1D_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateTexture2D:*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture2D_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateTexture3D:*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture3D_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateShaderResourceView:*(PUINT_PTR)&D3D11_ID3D11Device_CreateShaderResourceView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateUnorderedAccessView:*(PUINT_PTR)&D3D11_ID3D11Device_CreateUnorderedAccessView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateRenderTargetView:*(PUINT_PTR)&D3D11_ID3D11Device_CreateRenderTargetView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateDepthStencilView:*(PUINT_PTR)&D3D11_ID3D11Device_CreateDepthStencilView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateInputLayout:*(PUINT_PTR)&D3D11_ID3D11Device_CreateInputLayout_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateVertexShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreateVertexShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateGeometryShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreateGeometryShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput:*(PUINT_PTR)&D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreatePixelShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreatePixelShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateHullShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreateHullShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateDomainShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreateDomainShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateComputeShader:*(PUINT_PTR)&D3D11_ID3D11Device_CreateComputeShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateClassLinkage:*(PUINT_PTR)&D3D11_ID3D11Device_CreateClassLinkage_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateBlendState:*(PUINT_PTR)&D3D11_ID3D11Device_CreateBlendState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateDepthStencilState:*(PUINT_PTR)&D3D11_ID3D11Device_CreateDepthStencilState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateRasterizerState:*(PUINT_PTR)&D3D11_ID3D11Device_CreateRasterizerState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateSamplerState:*(PUINT_PTR)&D3D11_ID3D11Device_CreateSamplerState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateQuery:*(PUINT_PTR)&D3D11_ID3D11Device_CreateQuery_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreatePredicate:*(PUINT_PTR)&D3D11_ID3D11Device_CreatePredicate_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateCounter:*(PUINT_PTR)&D3D11_ID3D11Device_CreateCounter_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CreateDeferredContext:*(PUINT_PTR)&D3D11_ID3D11Device_CreateDeferredContext_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::OpenSharedResource:*(PUINT_PTR)&D3D11_ID3D11Device_OpenSharedResource_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CheckFormatSupport:*(PUINT_PTR)&D3D11_ID3D11Device_CheckFormatSupport_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CheckMultisampleQualityLevels:*(PUINT_PTR)&D3D11_ID3D11Device_CheckMultisampleQualityLevels_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CheckCounterInfo:*(PUINT_PTR)&D3D11_ID3D11Device_CheckCounterInfo_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CheckCounter:*(PUINT_PTR)&D3D11_ID3D11Device_CheckCounter_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::CheckFeatureSupport:*(PUINT_PTR)&D3D11_ID3D11Device_CheckFeatureSupport_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetPrivateData:*(PUINT_PTR)&D3D11_ID3D11Device_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::SetPrivateData:*(PUINT_PTR)&D3D11_ID3D11Device_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::SetPrivateDataInterface:*(PUINT_PTR)&D3D11_ID3D11Device_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetFeatureLevel:*(PUINT_PTR)&D3D11_ID3D11Device_GetFeatureLevel_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetCreationFlags:*(PUINT_PTR)&D3D11_ID3D11Device_GetCreationFlags_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetDeviceRemovedReason:*(PUINT_PTR)&D3D11_ID3D11Device_GetDeviceRemovedReason_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetImmediateContext:*(PUINT_PTR)&D3D11_ID3D11Device_GetImmediateContext_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::SetExceptionMode:*(PUINT_PTR)&D3D11_ID3D11Device_SetExceptionMode_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICE::GetExceptionMode:*(PUINT_PTR)&D3D11_ID3D11Device_GetExceptionMode_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_ID3D11Device::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D11_ID3D11Device_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D11_ID3D11Device_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D11_ID3D11Device_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];
	/*** ID3D11Device super methods ***/
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateBuffer];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture1D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture1D];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture2D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture2D];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateTexture3D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture3D];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateShaderResourceView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateShaderResourceView];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateUnorderedAccessView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateRenderTargetView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateRenderTargetView];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateDepthStencilView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDepthStencilView];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateInputLayout];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateVertexShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateVertexShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateGeometryShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateGeometryShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreatePixelShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreatePixelShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateHullShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateHullShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateDomainShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDomainShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateComputeShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateComputeShader];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateClassLinkage_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateClassLinkage];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateBlendState];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDepthStencilState];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateRasterizerState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateRasterizerState];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateSamplerState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateSamplerState];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateQuery_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateQuery];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreatePredicate_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreatePredicate];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateCounter];
	*(PUINT_PTR)&D3D11_ID3D11Device_CreateDeferredContext_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDeferredContext];
	*(PUINT_PTR)&D3D11_ID3D11Device_OpenSharedResource_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::OpenSharedResource];
	*(PUINT_PTR)&D3D11_ID3D11Device_CheckFormatSupport_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckFormatSupport];
	*(PUINT_PTR)&D3D11_ID3D11Device_CheckMultisampleQualityLevels_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels];
	*(PUINT_PTR)&D3D11_ID3D11Device_CheckCounterInfo_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckCounterInfo];
	*(PUINT_PTR)&D3D11_ID3D11Device_CheckCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckCounter];
	*(PUINT_PTR)&D3D11_ID3D11Device_CheckFeatureSupport_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckFeatureSupport];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetPrivateData];
	*(PUINT_PTR)&D3D11_ID3D11Device_SetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetPrivateData];
	*(PUINT_PTR)&D3D11_ID3D11Device_SetPrivateDataInterface_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetFeatureLevel_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetFeatureLevel];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetCreationFlags_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetCreationFlags];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetDeviceRemovedReason_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetImmediateContext_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetImmediateContext];
	*(PUINT_PTR)&D3D11_ID3D11Device_SetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetExceptionMode];
	*(PUINT_PTR)&D3D11_ID3D11Device_GetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetExceptionMode];


	return S_OK;
}

/**
* Catches the pointers to the current context and swapchain.
***/
void DCL_ID3D11Device::CatchContextAndSwapChain(ID3D11Device* pcThis)
{
	if ((!m_pcContext) && (!m_pcTransferSite->m_bForceD3D))
	{
		m_pcTransferSite->m_bForceD3D = true;
		ID3D11DeviceContext* pcContext;
		pcThis->GetImmediateContext(&pcContext);
		if (pcContext)
		{
			m_pcContext = pcContext;
			pcContext->Release();
		}

		m_pcTransferSite->m_bForceD3D = false;
	}

	if ((!m_pcSwapChain) && (!m_pcTransferSite->m_bForceD3D))
	{
		TRACE_LINE;
		if (m_pcContext)
		{
			TRACE_LINE;
			m_pcTransferSite->m_bForceD3D = true;
			// get the render target
			ID3D11RenderTargetView* pRenderTargetView = nullptr;
			ID3D11DepthStencilView* pDepthStencilView = nullptr;
			m_pcContext->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);

			// get the buffer texture
			ID3D11Texture2D* pBuffer = nullptr;
			if (pRenderTargetView)
				pRenderTargetView->GetResource((ID3D11Resource**)(&pBuffer));
			else
			{
				// create texture
				D3D11_TEXTURE2D_DESC sDesc;
				ZeroMemory(&sDesc, sizeof(sDesc));
				sDesc.Width = 16;
				sDesc.Height = 16;
				sDesc.MipLevels = sDesc.ArraySize = 1;
				sDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sDesc.SampleDesc.Count = 1;
				sDesc.Usage = D3D11_USAGE_DEFAULT;
				sDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

				if (FAILED(pcThis->CreateTexture2D(&sDesc, NULL, &pBuffer)))
					OutputDebugString(L"Failed to create Texture.");
			}

			if (pBuffer)
			{
				// get the buffer surface
				IDXGISurface* pSurface = nullptr;
				if (SUCCEEDED(pBuffer->QueryInterface(__uuidof(IDXGISurface), (void**)(&pSurface))))
				{
					// and finally get the swapchain
					IDXGISwapChain* pSwapChain = nullptr;
					if (SUCCEEDED(pSurface->GetParent(__uuidof(IDXGISwapChain), (void**)(&pSwapChain))))
					{
						m_pcSwapChain = pSwapChain;
						wchar_t buf[128]; wsprintf(buf, L"m_pcSwapChain %x", m_pcSwapChain); OutputDebugString(buf);
					}
					else if (SUCCEEDED(pSurface->GetParent(__uuidof(IDXGISwapChain1), (void**)(&pSwapChain))))
					{
						m_pcSwapChain = pSwapChain;
						wchar_t buf[128]; wsprintf(buf, L"m_pcSwapChain1 %x", m_pcSwapChain); OutputDebugString(buf);
					}
					else if (SUCCEEDED(pSurface->GetParent(__uuidof(IDXGISwapChain2), (void**)(&pSwapChain))))
					{
						m_pcSwapChain = pSwapChain;
						wchar_t buf[128]; wsprintf(buf, L"m_pcSwapChain2 %x", m_pcSwapChain); OutputDebugString(buf);
					}
					else
					{
						if (m_bCreateSwapChain)
						{
							// create a swapchain if not accessible
							IDXGIFactory * DXGIFactory;
							if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
							{
								HWND pWnd = GetDesktopWindow();
								DXGI_SWAP_CHAIN_DESC sDesc = {};
								sDesc.BufferCount = 1;
								sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
								sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
								sDesc.SampleDesc.Count = 1;
								sDesc.OutputWindow = pWnd;
								sDesc.Windowed = true;//((GetWindowLong(pWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

								DXGIFactory->CreateSwapChain(pcThis, &sDesc, &m_pcSwapChain);
								wchar_t buf[128]; wsprintf(buf, L"m_pcSwapChain %x", m_pcSwapChain); OutputDebugString(buf);
								DXGIFactory->Release();
							}
						}
					}

					if (pSurface) { pSurface->Release(); pSurface = nullptr; }
				}
				// else OutputDebugString(L"No Surface!");
				if (pBuffer) { pBuffer->Release(); pBuffer = nullptr; }
			}
			// else OutputDebugString(L"No Buffer!");

			if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }

			m_pcTransferSite->m_bForceD3D = false;
			if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }
		}
	}
}

/**
* Provides the swapchain pointer.
***/
HRESULT DCL_ID3D11Device::GetDevice(ID3D11Device** ppcDevice)
{
	if (m_pcDevice == NULL)
	{
		*ppcDevice = NULL;
		return E_FAIL;
	}

	*ppcDevice = m_pcDevice;

	return S_OK;
}

/**
* Provides the swapchain pointer.
***/
HRESULT DCL_ID3D11Device::GetDeviceContext(ID3D11DeviceContext** ppcContext)
{
	if (m_pcContext == NULL)
	{
		*ppcContext = NULL;
		return E_FAIL;
	}

	*ppcContext = m_pcContext;

	return S_OK;
}

/**
* Provides the swapchain pointer.
***/
HRESULT DCL_ID3D11Device::GetDXGISwapChain(IDXGISwapChain** ppcSwapChain)
{
	if (m_pcSwapChain == NULL)
	{
		*ppcSwapChain = NULL;
		return E_FAIL;
	}

	*ppcSwapChain = m_pcSwapChain;

	return S_OK;
}

/**
* Releases all D3D interface pointers.
***/
void DCL_ID3D11Device::ReleaseAllInterfaces()
{
	if (m_pcSwapChain) { m_pcSwapChain->Release(); m_pcSwapChain = nullptr; }
	if (m_pcContext) { m_pcContext->Release(); m_pcContext = nullptr; }
}

#pragma endregion