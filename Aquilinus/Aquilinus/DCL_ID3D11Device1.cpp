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
#ifdef _DEBUG
#include <windows.h>
#include <tchar.h>
#include <sstream>
typedef std::basic_stringstream<TCHAR> tstringstream;
template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }
#define OUTPUT_DEBUG_STRING_ID3D11Device1(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_ID3D11Device1(...)
#endif
#define AQU_ID3D11DEVICE1_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_ID3D11Device1[a]) && (m_pcTransferSite->m_ppNOD_ID3D11Device1[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_ID3D11DEVICE1_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_ID3D11Device1[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_ID3D11DEVICE1_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_ID3D11Device1[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_ID3D11DEVICE1_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11Device1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_ID3D11DEVICE1_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11Device1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }
#include"DCL_ID3D11Device1.h"
#include"DCL_ID3D11Device1_Super.h"

#pragma region DCL_ID3D11Device1 constructor/destructor
/**
* Constructor
***/
DCL_ID3D11Device1::DCL_ID3D11Device1(AQU_TransferSite* pcTransferSite) : 
m_pcTransferSite(pcTransferSite)
{ }
/**
* Destructor
***/
DCL_ID3D11Device1::~DCL_ID3D11Device1(){ }
#pragma endregion

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface
***/
HRESULT WINAPI DCL_ID3D11Device1::QueryInterface(ID3D11Device1* pcThis, REFIID riid, void** ppvObject)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::QueryInterface");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_IUNKNOWN::QueryInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_IUNKNOWN::QueryInterface,0,riid);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_IUNKNOWN::QueryInterface,1,ppvObject);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_IUNKNOWN::QueryInterface);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::QueryInterface, HRESULT);

 return D3D11_ID3D11Device1_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* ULONG WINAPI AddRef
***/
ULONG WINAPI DCL_ID3D11Device1::AddRef(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::AddRef");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_IUNKNOWN::AddRef);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_IUNKNOWN::AddRef);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::AddRef, ULONG);

 return D3D11_ID3D11Device1_AddRef_Super(pcThis);
}

/**
* ULONG WINAPI Release
***/
ULONG WINAPI DCL_ID3D11Device1::Release(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::Release");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_IUNKNOWN::Release);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_IUNKNOWN::Release);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::Release, ULONG);

 return D3D11_ID3D11Device1_Release_Super(pcThis);
}

#pragma endregion

#pragma region ID3D11Device methods
/**
* HRESULT WINAPI CreateBuffer
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateBuffer(ID3D11Device1* pcThis, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateBuffer");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateBuffer);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer,0,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer,1,pInitialData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateBuffer,2,ppBuffer);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateBuffer);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateBuffer, HRESULT);

 return D3D11_ID3D11Device1_CreateBuffer_Super(pcThis, pDesc, pInitialData, ppBuffer);
}

/**
* HRESULT WINAPI CreateTexture1D
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateTexture1D(ID3D11Device1* pcThis, D3D11_TEXTURE1D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture1D** ppTexture1D)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateTexture1D");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateTexture1D);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D,0,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D,1,pInitialData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture1D,2,ppTexture1D);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture1D);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture1D, HRESULT);

 return D3D11_ID3D11Device1_CreateTexture1D_Super(pcThis, pDesc, pInitialData, ppTexture1D);
}

/**
* HRESULT WINAPI CreateTexture2D
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateTexture2D(ID3D11Device1* pcThis, D3D11_TEXTURE2D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateTexture2D");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateTexture2D);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D,0,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D,1,pInitialData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture2D,2,ppTexture2D);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture2D);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture2D, HRESULT);

 return D3D11_ID3D11Device1_CreateTexture2D_Super(pcThis, pDesc, pInitialData, ppTexture2D);
}

/**
* HRESULT WINAPI CreateTexture3D
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateTexture3D(ID3D11Device1* pcThis, D3D11_TEXTURE3D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture3D** ppTexture3D)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateTexture3D");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateTexture3D);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D,0,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D,1,pInitialData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateTexture3D,2,ppTexture3D);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateTexture3D);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateTexture3D, HRESULT);

 return D3D11_ID3D11Device1_CreateTexture3D_Super(pcThis, pDesc, pInitialData, ppTexture3D);
}

/**
* HRESULT WINAPI CreateShaderResourceView
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateShaderResourceView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateShaderResourceView");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateShaderResourceView);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView,0,pResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView,1,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateShaderResourceView,2,ppSRView);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateShaderResourceView);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateShaderResourceView, HRESULT);

 return D3D11_ID3D11Device1_CreateShaderResourceView_Super(pcThis, pResource, pDesc, ppSRView);
}

/**
* HRESULT WINAPI CreateUnorderedAccessView
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateUnorderedAccessView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateUnorderedAccessView");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateUnorderedAccessView);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView,0,pResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView,1,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateUnorderedAccessView,2,ppUAView);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateUnorderedAccessView);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateUnorderedAccessView, HRESULT);

 return D3D11_ID3D11Device1_CreateUnorderedAccessView_Super(pcThis, pResource, pDesc, ppUAView);
}

/**
* HRESULT WINAPI CreateRenderTargetView
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateRenderTargetView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateRenderTargetView");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateRenderTargetView);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView,0,pResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView,1,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateRenderTargetView,2,ppRTView);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateRenderTargetView);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateRenderTargetView, HRESULT);

 return D3D11_ID3D11Device1_CreateRenderTargetView_Super(pcThis, pResource, pDesc, ppRTView);
}

/**
* HRESULT WINAPI CreateDepthStencilView
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDepthStencilView(ID3D11Device1* pcThis, ID3D11Resource* pResource, D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDepthStencilView");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateDepthStencilView);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView,0,pResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView,1,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilView,2,ppDepthStencilView);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateDepthStencilView);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDepthStencilView, HRESULT);

 return D3D11_ID3D11Device1_CreateDepthStencilView_Super(pcThis, pResource, pDesc, ppDepthStencilView);
}

/**
* HRESULT WINAPI CreateInputLayout
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateInputLayout(ID3D11Device1* pcThis, D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateInputLayout");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateInputLayout);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout,0,pInputElementDescs);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout,1,NumElements);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout,2,pShaderBytecodeWithInputSignature);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout,3,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateInputLayout,4,ppInputLayout);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateInputLayout);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateInputLayout, HRESULT);

 return D3D11_ID3D11Device1_CreateInputLayout_Super(pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

/**
* HRESULT WINAPI CreateVertexShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateVertexShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateVertexShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateVertexShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateVertexShader,3,ppVertexShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateVertexShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateVertexShader, HRESULT);

 return D3D11_ID3D11Device1_CreateVertexShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

/**
* HRESULT WINAPI CreateGeometryShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateGeometryShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateGeometryShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateGeometryShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShader,3,ppGeometryShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateGeometryShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateGeometryShader, HRESULT);

 return D3D11_ID3D11Device1_CreateGeometryShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
}

/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateGeometryShaderWithStreamOutput(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, UINT* pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateGeometryShaderWithStreamOutput");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,2,pSODeclaration);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,3,NumEntries);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,4,pBufferStrides);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,5,NumStrides);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,6,RasterizedStream);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,7,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput,8,ppGeometryShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput, HRESULT);

 return D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput_Super(pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
}

/**
* HRESULT WINAPI CreatePixelShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreatePixelShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreatePixelShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreatePixelShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePixelShader,3,ppPixelShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreatePixelShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreatePixelShader, HRESULT);

 return D3D11_ID3D11Device1_CreatePixelShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

/**
* HRESULT WINAPI CreateHullShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateHullShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateHullShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateHullShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateHullShader,3,ppHullShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateHullShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateHullShader, HRESULT);

 return D3D11_ID3D11Device1_CreateHullShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
}

/**
* HRESULT WINAPI CreateDomainShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDomainShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDomainShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateDomainShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDomainShader,3,ppDomainShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateDomainShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDomainShader, HRESULT);

 return D3D11_ID3D11Device1_CreateDomainShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
}

/**
* HRESULT WINAPI CreateComputeShader
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateComputeShader(ID3D11Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateComputeShader");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateComputeShader);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader,0,pShaderBytecode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader,1,BytecodeLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader,2,pClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateComputeShader,3,ppComputeShader);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateComputeShader);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateComputeShader, HRESULT);

 return D3D11_ID3D11Device1_CreateComputeShader_Super(pcThis, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
}

/**
* HRESULT WINAPI CreateClassLinkage
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateClassLinkage(ID3D11Device1* pcThis, ID3D11ClassLinkage** ppLinkage)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateClassLinkage");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateClassLinkage);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateClassLinkage,0,ppLinkage);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateClassLinkage);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateClassLinkage, HRESULT);

 return D3D11_ID3D11Device1_CreateClassLinkage_Super(pcThis, ppLinkage);
}

/**
* HRESULT WINAPI CreateBlendState
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateBlendState(ID3D11Device1* pcThis, D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateBlendState");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateBlendState);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateBlendState,0,pBlendStateDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateBlendState,1,ppBlendState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateBlendState);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateBlendState, HRESULT);

 return D3D11_ID3D11Device1_CreateBlendState_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
* HRESULT WINAPI CreateDepthStencilState
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDepthStencilState(ID3D11Device1* pcThis, D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDepthStencilState");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateDepthStencilState);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilState,0,pDepthStencilDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDepthStencilState,1,ppDepthStencilState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateDepthStencilState);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDepthStencilState, HRESULT);

 return D3D11_ID3D11Device1_CreateDepthStencilState_Super(pcThis, pDepthStencilDesc, ppDepthStencilState);
}

/**
* HRESULT WINAPI CreateRasterizerState
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateRasterizerState(ID3D11Device1* pcThis, D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateRasterizerState");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateRasterizerState);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateRasterizerState,0,pRasterizerDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateRasterizerState,1,ppRasterizerState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateRasterizerState);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateRasterizerState, HRESULT);

 return D3D11_ID3D11Device1_CreateRasterizerState_Super(pcThis, pRasterizerDesc, ppRasterizerState);
}

/**
* HRESULT WINAPI CreateSamplerState
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateSamplerState(ID3D11Device1* pcThis, D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateSamplerState");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateSamplerState);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateSamplerState,0,pSamplerDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateSamplerState,1,ppSamplerState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateSamplerState);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateSamplerState, HRESULT);

 return D3D11_ID3D11Device1_CreateSamplerState_Super(pcThis, pSamplerDesc, ppSamplerState);
}

/**
* HRESULT WINAPI CreateQuery
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateQuery(ID3D11Device1* pcThis, D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateQuery");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateQuery);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateQuery,0,pQueryDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateQuery,1,ppQuery);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateQuery);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateQuery, HRESULT);

 return D3D11_ID3D11Device1_CreateQuery_Super(pcThis, pQueryDesc, ppQuery);
}

/**
* HRESULT WINAPI CreatePredicate
***/
HRESULT WINAPI DCL_ID3D11Device1::CreatePredicate(ID3D11Device1* pcThis, D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreatePredicate");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreatePredicate);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePredicate,0,pPredicateDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreatePredicate,1,ppPredicate);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreatePredicate);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreatePredicate, HRESULT);

 return D3D11_ID3D11Device1_CreatePredicate_Super(pcThis, pPredicateDesc, ppPredicate);
}

/**
* HRESULT WINAPI CreateCounter
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateCounter(ID3D11Device1* pcThis, D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateCounter");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateCounter);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateCounter,0,pCounterDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateCounter,1,ppCounter);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateCounter);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateCounter, HRESULT);

 return D3D11_ID3D11Device1_CreateCounter_Super(pcThis, pCounterDesc, ppCounter);
}

/**
* HRESULT WINAPI CreateDeferredContext
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDeferredContext(ID3D11Device1* pcThis, UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDeferredContext");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CreateDeferredContext);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDeferredContext,0,ContextFlags);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CreateDeferredContext,1,ppDeferredContext);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CreateDeferredContext);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CreateDeferredContext, HRESULT);

 return D3D11_ID3D11Device1_CreateDeferredContext_Super(pcThis, ContextFlags, ppDeferredContext);
}

/**
* HRESULT WINAPI OpenSharedResource
***/
HRESULT WINAPI DCL_ID3D11Device1::OpenSharedResource(ID3D11Device1* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::OpenSharedResource");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::OpenSharedResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource,0,hResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource,1,ReturnedInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::OpenSharedResource,2,ppResource);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::OpenSharedResource);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::OpenSharedResource, HRESULT);

 return D3D11_ID3D11Device1_OpenSharedResource_Super(pcThis, hResource, ReturnedInterface, ppResource);
}

/**
* HRESULT WINAPI CheckFormatSupport
***/
HRESULT WINAPI DCL_ID3D11Device1::CheckFormatSupport(ID3D11Device1* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CheckFormatSupport");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CheckFormatSupport);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckFormatSupport,0,Format);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckFormatSupport,1,pFormatSupport);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CheckFormatSupport);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckFormatSupport, HRESULT);

 return D3D11_ID3D11Device1_CheckFormatSupport_Super(pcThis, Format, pFormatSupport);
}

/**
* HRESULT WINAPI CheckMultisampleQualityLevels
***/
HRESULT WINAPI DCL_ID3D11Device1::CheckMultisampleQualityLevels(ID3D11Device1* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CheckMultisampleQualityLevels");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels,0,Format);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels,1,SampleCount);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels,2,pNumQualityLevels);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckMultisampleQualityLevels, HRESULT);

 return D3D11_ID3D11Device1_CheckMultisampleQualityLevels_Super(pcThis, Format, SampleCount, pNumQualityLevels);
}

/**
* void WINAPI CheckCounterInfo
***/
void WINAPI DCL_ID3D11Device1::CheckCounterInfo(ID3D11Device1* pcThis, D3D11_COUNTER_INFO* pCounterInfo)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CheckCounterInfo");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CheckCounterInfo);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounterInfo,0,pCounterInfo);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CheckCounterInfo);
AQU_ID3D11DEVICE1_REPLACE_VOID__(VMT_ID3D11DEVICE::CheckCounterInfo);

D3D11_ID3D11Device1_CheckCounterInfo_Super(pcThis, pCounterInfo);
}

/**
* HRESULT WINAPI CheckCounter
***/
HRESULT WINAPI DCL_ID3D11Device1::CheckCounter(ID3D11Device1* pcThis, D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CheckCounter");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CheckCounter);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,0,pDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,1,pType);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,2,pActiveCounters);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,3,szName);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,4,pNameLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,5,szUnits);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,6,pUnitsLength);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,7,szDescription);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckCounter,8,pDescriptionLength);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CheckCounter);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckCounter, HRESULT);

 return D3D11_ID3D11Device1_CheckCounter_Super(pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

/**
* HRESULT WINAPI CheckFeatureSupport
***/
HRESULT WINAPI DCL_ID3D11Device1::CheckFeatureSupport(ID3D11Device1* pcThis, D3D11_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CheckFeatureSupport");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::CheckFeatureSupport);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport,0,Feature);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport,1,pFeatureSupportData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::CheckFeatureSupport,2,FeatureSupportDataSize);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::CheckFeatureSupport);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::CheckFeatureSupport, HRESULT);

 return D3D11_ID3D11Device1_CheckFeatureSupport_Super(pcThis, Feature, pFeatureSupportData, FeatureSupportDataSize);
}

/**
* HRESULT WINAPI GetPrivateData
***/
HRESULT WINAPI DCL_ID3D11Device1::GetPrivateData(ID3D11Device1* pcThis, REFGUID guid, UINT* pDataSize, void* pData)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetPrivateData");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetPrivateData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData,0,guid);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData,1,pDataSize);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::GetPrivateData,2,pData);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetPrivateData);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::GetPrivateData, HRESULT);

 return D3D11_ID3D11Device1_GetPrivateData_Super(pcThis, guid, pDataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateData
***/
HRESULT WINAPI DCL_ID3D11Device1::SetPrivateData(ID3D11Device1* pcThis, REFGUID guid, UINT DataSize, void* pData)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::SetPrivateData");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::SetPrivateData);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData,0,guid);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData,1,DataSize);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateData,2,pData);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::SetPrivateData);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::SetPrivateData, HRESULT);

 return D3D11_ID3D11Device1_SetPrivateData_Super(pcThis, guid, DataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateDataInterface
***/
HRESULT WINAPI DCL_ID3D11Device1::SetPrivateDataInterface(ID3D11Device1* pcThis, REFGUID guid, IUnknown* pData)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::SetPrivateDataInterface");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::SetPrivateDataInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateDataInterface,0,guid);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetPrivateDataInterface,1,pData);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::SetPrivateDataInterface);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::SetPrivateDataInterface, HRESULT);

 return D3D11_ID3D11Device1_SetPrivateDataInterface_Super(pcThis, guid, pData);
}

/**
* D3D_FEATURE_LEVEL WINAPI GetFeatureLevel
***/
D3D_FEATURE_LEVEL WINAPI DCL_ID3D11Device1::GetFeatureLevel(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetFeatureLevel");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetFeatureLevel);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetFeatureLevel);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::GetFeatureLevel, D3D_FEATURE_LEVEL);

 return D3D11_ID3D11Device1_GetFeatureLevel_Super(pcThis);
}

/**
* UINT WINAPI GetCreationFlags
***/
UINT WINAPI DCL_ID3D11Device1::GetCreationFlags(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetCreationFlags");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetCreationFlags);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetCreationFlags);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::GetCreationFlags, UINT);

 return D3D11_ID3D11Device1_GetCreationFlags_Super(pcThis);
}

/**
* HRESULT WINAPI GetDeviceRemovedReason
***/
HRESULT WINAPI DCL_ID3D11Device1::GetDeviceRemovedReason(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetDeviceRemovedReason");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetDeviceRemovedReason);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetDeviceRemovedReason);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::GetDeviceRemovedReason, HRESULT);

 return D3D11_ID3D11Device1_GetDeviceRemovedReason_Super(pcThis);
}

/**
* void WINAPI GetImmediateContext
***/
void WINAPI DCL_ID3D11Device1::GetImmediateContext(ID3D11Device1* pcThis, ID3D11DeviceContext** ppImmediateContext)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetImmediateContext");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetImmediateContext);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::GetImmediateContext,0,ppImmediateContext);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetImmediateContext);
AQU_ID3D11DEVICE1_REPLACE_VOID__(VMT_ID3D11DEVICE::GetImmediateContext);

D3D11_ID3D11Device1_GetImmediateContext_Super(pcThis, ppImmediateContext);
}

/**
* HRESULT WINAPI SetExceptionMode
***/
HRESULT WINAPI DCL_ID3D11Device1::SetExceptionMode(ID3D11Device1* pcThis, UINT RaiseFlags)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::SetExceptionMode");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::SetExceptionMode);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE::SetExceptionMode,0,RaiseFlags);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::SetExceptionMode);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::SetExceptionMode, HRESULT);

 return D3D11_ID3D11Device1_SetExceptionMode_Super(pcThis, RaiseFlags);
}

/**
* UINT WINAPI GetExceptionMode
***/
UINT WINAPI DCL_ID3D11Device1::GetExceptionMode(ID3D11Device1* pcThis )
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetExceptionMode");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE::GetExceptionMode);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE::GetExceptionMode);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE::GetExceptionMode, UINT);

 return D3D11_ID3D11Device1_GetExceptionMode_Super(pcThis);
}

#pragma endregion

#pragma region ID3D11Device1 methods
/**
* void WINAPI GetImmediateContext1
***/
void WINAPI DCL_ID3D11Device1::GetImmediateContext1(ID3D11Device1* pcThis, ID3D11DeviceContext1** ppImmediateContext)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::GetImmediateContext1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::GetImmediateContext1);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::GetImmediateContext1,0,ppImmediateContext);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::GetImmediateContext1);
AQU_ID3D11DEVICE1_REPLACE_VOID__(VMT_ID3D11DEVICE1::GetImmediateContext1);

D3D11_ID3D11Device1_GetImmediateContext1_Super(pcThis, ppImmediateContext);
}

/**
* HRESULT WINAPI CreateDeferredContext1
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDeferredContext1(ID3D11Device1* pcThis, UINT ContextFlags, ID3D11DeviceContext1** ppDeferredContext)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDeferredContext1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::CreateDeferredContext1);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeferredContext1,0,ContextFlags);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeferredContext1,1,ppDeferredContext);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::CreateDeferredContext1);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::CreateDeferredContext1, HRESULT);

 return D3D11_ID3D11Device1_CreateDeferredContext1_Super(pcThis, ContextFlags, ppDeferredContext);
}

/**
* HRESULT WINAPI CreateBlendState1
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateBlendState1(ID3D11Device1* pcThis, D3D11_BLEND_DESC1* pBlendStateDesc, ID3D11BlendState1** ppBlendState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateBlendState1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::CreateBlendState1);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateBlendState1,0,pBlendStateDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateBlendState1,1,ppBlendState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::CreateBlendState1);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::CreateBlendState1, HRESULT);

 return D3D11_ID3D11Device1_CreateBlendState1_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
* HRESULT WINAPI CreateRasterizerState1
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateRasterizerState1(ID3D11Device1* pcThis, D3D11_RASTERIZER_DESC1* pRasterizerDesc, ID3D11RasterizerState1** ppRasterizerState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateRasterizerState1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::CreateRasterizerState1);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateRasterizerState1,0,pRasterizerDesc);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateRasterizerState1,1,ppRasterizerState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::CreateRasterizerState1);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::CreateRasterizerState1, HRESULT);

 return D3D11_ID3D11Device1_CreateRasterizerState1_Super(pcThis, pRasterizerDesc, ppRasterizerState);
}

/**
* HRESULT WINAPI CreateDeviceContextState
***/
HRESULT WINAPI DCL_ID3D11Device1::CreateDeviceContextState(ID3D11Device1* pcThis, UINT Flags, D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface, D3D_FEATURE_LEVEL* pChosenFeatureLevel, ID3DDeviceContextState** ppContextState)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::CreateDeviceContextState");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::CreateDeviceContextState);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,0,Flags);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,1,pFeatureLevels);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,2,FeatureLevels);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,3,SDKVersion);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,4,EmulatedInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,5,pChosenFeatureLevel);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::CreateDeviceContextState,6,ppContextState);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::CreateDeviceContextState);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::CreateDeviceContextState, HRESULT);

 return D3D11_ID3D11Device1_CreateDeviceContextState_Super(pcThis, Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);
}

/**
* HRESULT WINAPI OpenSharedResource1
***/
HRESULT WINAPI DCL_ID3D11Device1::OpenSharedResource1(ID3D11Device1* pcThis, HANDLE hResource, REFIID returnedInterface, void** ppResource)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::OpenSharedResource1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::OpenSharedResource1);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResource1,0,hResource);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResource1,1,returnedInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResource1,2,ppResource);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::OpenSharedResource1);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::OpenSharedResource1, HRESULT);

 return D3D11_ID3D11Device1_OpenSharedResource1_Super(pcThis, hResource, returnedInterface, ppResource);
}

/**
* HRESULT WINAPI OpenSharedResourceByName
***/
HRESULT WINAPI DCL_ID3D11Device1::OpenSharedResourceByName(ID3D11Device1* pcThis, LPCWSTR lpName, DWORD dwDesiredAccess, REFIID returnedInterface, void** ppResource)
{
OUTPUT_DEBUG_STRING_ID3D11Device1(L"ID3D11Device1::OpenSharedResourceByName");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_ID3D11DEVICE1_HEADER________(VMT_ID3D11DEVICE1::OpenSharedResourceByName);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResourceByName,0,lpName);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResourceByName,1,dwDesiredAccess);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResourceByName,2,returnedInterface);
AQU_ID3D11DEVICE1_SET_DATA______(VMT_ID3D11DEVICE1::OpenSharedResourceByName,3,ppResource);
AQU_ID3D11DEVICE1_PROVOKE_______(VMT_ID3D11DEVICE1::OpenSharedResourceByName);
AQU_ID3D11DEVICE1_REPLACE_METHOD(VMT_ID3D11DEVICE1::OpenSharedResourceByName, HRESULT);

 return D3D11_ID3D11Device1_OpenSharedResourceByName_Super(pcThis, lpName, dwDesiredAccess, returnedInterface, ppResource);
}

#pragma endregion

#pragma region DCL_ID3D11Device1 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11Device1::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IUnknown super methods ***/
case VMT_IUNKNOWN::QueryInterface:*(PUINT_PTR)&D3D11_ID3D11Device1_QueryInterface_Super = (UINT_PTR)dwFunc; break;
case VMT_IUNKNOWN::AddRef:*(PUINT_PTR)&D3D11_ID3D11Device1_AddRef_Super = (UINT_PTR)dwFunc; break;
case VMT_IUNKNOWN::Release:*(PUINT_PTR)&D3D11_ID3D11Device1_Release_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11Device1::SetSuperFunctionPointer(VMT_ID3D11DEVICE::VMT_ID3D11Device eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** ID3D11Device super methods ***/
case VMT_ID3D11DEVICE::CreateBuffer:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBuffer_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateTexture1D:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture1D_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateTexture2D:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture2D_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateTexture3D:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture3D_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateShaderResourceView:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateShaderResourceView_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateUnorderedAccessView:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateUnorderedAccessView_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateRenderTargetView:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRenderTargetView_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateDepthStencilView:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDepthStencilView_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateInputLayout:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateInputLayout_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateVertexShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateVertexShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateGeometryShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateGeometryShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreatePixelShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreatePixelShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateHullShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateHullShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateDomainShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDomainShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateComputeShader:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateComputeShader_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateClassLinkage:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateClassLinkage_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateBlendState:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBlendState_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateDepthStencilState:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDepthStencilState_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateRasterizerState:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRasterizerState_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateSamplerState:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateSamplerState_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateQuery:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateQuery_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreatePredicate:*(PUINT_PTR)&D3D11_ID3D11Device1_CreatePredicate_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateCounter:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateCounter_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CreateDeferredContext:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeferredContext_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::OpenSharedResource:*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResource_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CheckFormatSupport:*(PUINT_PTR)&D3D11_ID3D11Device1_CheckFormatSupport_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CheckMultisampleQualityLevels:*(PUINT_PTR)&D3D11_ID3D11Device1_CheckMultisampleQualityLevels_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CheckCounterInfo:*(PUINT_PTR)&D3D11_ID3D11Device1_CheckCounterInfo_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CheckCounter:*(PUINT_PTR)&D3D11_ID3D11Device1_CheckCounter_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::CheckFeatureSupport:*(PUINT_PTR)&D3D11_ID3D11Device1_CheckFeatureSupport_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetPrivateData:*(PUINT_PTR)&D3D11_ID3D11Device1_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::SetPrivateData:*(PUINT_PTR)&D3D11_ID3D11Device1_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::SetPrivateDataInterface:*(PUINT_PTR)&D3D11_ID3D11Device1_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetFeatureLevel:*(PUINT_PTR)&D3D11_ID3D11Device1_GetFeatureLevel_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetCreationFlags:*(PUINT_PTR)&D3D11_ID3D11Device1_GetCreationFlags_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetDeviceRemovedReason:*(PUINT_PTR)&D3D11_ID3D11Device1_GetDeviceRemovedReason_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetImmediateContext:*(PUINT_PTR)&D3D11_ID3D11Device1_GetImmediateContext_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::SetExceptionMode:*(PUINT_PTR)&D3D11_ID3D11Device1_SetExceptionMode_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE::GetExceptionMode:*(PUINT_PTR)&D3D11_ID3D11Device1_GetExceptionMode_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11Device1::SetSuperFunctionPointer(VMT_ID3D11DEVICE1::VMT_ID3D11Device1 eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** ID3D11Device1 super methods ***/
case VMT_ID3D11DEVICE1::GetImmediateContext1:*(PUINT_PTR)&D3D11_ID3D11Device1_GetImmediateContext1_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::CreateDeferredContext1:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeferredContext1_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::CreateBlendState1:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBlendState1_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::CreateRasterizerState1:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRasterizerState1_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::CreateDeviceContextState:*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeviceContextState_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::OpenSharedResource1:*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResource1_Super = (UINT_PTR)dwFunc; break;
case VMT_ID3D11DEVICE1::OpenSharedResourceByName:*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResourceByName_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_ID3D11Device1::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
if (!pVMTable) return E_FAIL;

/*** IUnknown super methods ***/
*(PUINT_PTR)&D3D11_ID3D11Device1_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
*(PUINT_PTR)&D3D11_ID3D11Device1_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
*(PUINT_PTR)&D3D11_ID3D11Device1_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];
/*** ID3D11Device super methods ***/
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateBuffer];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture1D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture1D];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture2D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture2D];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateTexture3D_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateTexture3D];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateShaderResourceView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateShaderResourceView];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateUnorderedAccessView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateUnorderedAccessView];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRenderTargetView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateRenderTargetView];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDepthStencilView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDepthStencilView];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateInputLayout];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateVertexShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateVertexShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateGeometryShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateGeometryShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateGeometryShaderWithStreamOutput];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreatePixelShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreatePixelShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateHullShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateHullShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDomainShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDomainShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateComputeShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateComputeShader];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateClassLinkage_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateClassLinkage];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateBlendState];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDepthStencilState];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRasterizerState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateRasterizerState];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateSamplerState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateSamplerState];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateQuery_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateQuery];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreatePredicate_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreatePredicate];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateCounter];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeferredContext_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CreateDeferredContext];
*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResource_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::OpenSharedResource];
*(PUINT_PTR)&D3D11_ID3D11Device1_CheckFormatSupport_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckFormatSupport];
*(PUINT_PTR)&D3D11_ID3D11Device1_CheckMultisampleQualityLevels_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckMultisampleQualityLevels];
*(PUINT_PTR)&D3D11_ID3D11Device1_CheckCounterInfo_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckCounterInfo];
*(PUINT_PTR)&D3D11_ID3D11Device1_CheckCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckCounter];
*(PUINT_PTR)&D3D11_ID3D11Device1_CheckFeatureSupport_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::CheckFeatureSupport];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetPrivateData];
*(PUINT_PTR)&D3D11_ID3D11Device1_SetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetPrivateData];
*(PUINT_PTR)&D3D11_ID3D11Device1_SetPrivateDataInterface_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetPrivateDataInterface];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetFeatureLevel_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetFeatureLevel];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetCreationFlags_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetCreationFlags];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetDeviceRemovedReason_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetDeviceRemovedReason];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetImmediateContext_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetImmediateContext];
*(PUINT_PTR)&D3D11_ID3D11Device1_SetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::SetExceptionMode];
*(PUINT_PTR)&D3D11_ID3D11Device1_GetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE::GetExceptionMode];
/*** ID3D11Device1 super methods ***/
*(PUINT_PTR)&D3D11_ID3D11Device1_GetImmediateContext1_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::GetImmediateContext1];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeferredContext1_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::CreateDeferredContext1];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateBlendState1_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::CreateBlendState1];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateRasterizerState1_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::CreateRasterizerState1];
*(PUINT_PTR)&D3D11_ID3D11Device1_CreateDeviceContextState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::CreateDeviceContextState];
*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResource1_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::OpenSharedResource1];
*(PUINT_PTR)&D3D11_ID3D11Device1_OpenSharedResourceByName_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICE1::OpenSharedResourceByName];


return S_OK;
}

#pragma endregion