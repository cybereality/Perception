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
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

#ifdef _DEBUG
#include <windows.h>
#include <tchar.h>
#include <sstream>
typedef std::basic_stringstream<TCHAR> tstringstream;
template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }
#define OUTPUT_DEBUG_STRING_ID3D11DeviceContext(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_ID3D11DeviceContext(...)
#endif
#define AQU_ID3D11DEVICECONTEXT_PRECALL_______(a) if (m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
#define AQU_ID3D11DEVICECONTEXT_HEADER________(a) m_pcTransferSite->m_dwMainThreadId = GetCurrentThreadId(); if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]) && (m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_ID3D11DEVICECONTEXT_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_ID3D11DEVICECONTEXT_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false; 
#define AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D11DeviceContext[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }
#include"DCL_ID3D11DeviceContext.h"
#include"DCL_ID3D11DeviceContext_Super.h"

#pragma region DCL_ID3D11DeviceContext constructor/destructor
/**
* Constructor
***/
DCL_ID3D11DeviceContext::DCL_ID3D11DeviceContext(AQU_TransferSite* pcTransferSite) :
m_pcSwapChain(nullptr),
m_pcTransferSite(pcTransferSite)
{ }
/**
* Destructor
***/
DCL_ID3D11DeviceContext::~DCL_ID3D11DeviceContext(){ if (m_pcSwapChain) { m_pcSwapChain->Release(); m_pcSwapChain = nullptr; } }
#pragma endregion

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::QueryInterface(ID3D11DeviceContext* pcThis, REFIID riid, void** ppvObject)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::QueryInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	/*AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_IUNKNOWN::QueryInterface,0,riid);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_IUNKNOWN::QueryInterface,1,ppvObject);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_IUNKNOWN::QueryInterface, HRESULT);*/

	return D3D11_ID3D11DeviceContext_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* ULONG WINAPI AddRef
***/
ULONG WINAPI DCL_ID3D11DeviceContext::AddRef(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::AddRef");

	/*// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_IUNKNOWN::AddRef);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_IUNKNOWN::AddRef);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_IUNKNOWN::AddRef, ULONG);*/

	// set reference counter in the transfer site class
	int iCount = D3D11_ID3D11DeviceContext_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] = iCount;
	return iCount;
}

/**
* ULONG WINAPI Release
***/
ULONG WINAPI DCL_ID3D11DeviceContext::Release(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Release");

	/*// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_IUNKNOWN::Release);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_IUNKNOWN::Release);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_IUNKNOWN::Release, ULONG);*/

	// set reference counter in the transfer site class
	int iCount = D3D11_ID3D11DeviceContext_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext] = iCount;
	return iCount;
}

#pragma endregion

#pragma region ID3D11DeviceChild methods
/**
* void WINAPI GetDevice
***/
void WINAPI DCL_ID3D11DeviceContext::GetDevice(ID3D11DeviceContext* pcThis, ID3D11Device** ppDevice)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetDevice");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECHILD::GetDevice);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::GetDevice, 0, ppDevice);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECHILD::GetDevice);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECHILD::GetDevice);

	D3D11_ID3D11DeviceContext_GetDevice_Super(pcThis, ppDevice);
}

/**
* HRESULT WINAPI GetPrivateData
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::GetPrivateData(ID3D11DeviceContext* pcThis, REFGUID guid, UINT* pDataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECHILD::GetPrivateData);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::GetPrivateData, 0, guid);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::GetPrivateData, 1, pDataSize);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::GetPrivateData, 2, pData);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECHILD::GetPrivateData);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECHILD::GetPrivateData, HRESULT);

	return D3D11_ID3D11DeviceContext_GetPrivateData_Super(pcThis, guid, pDataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateData
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::SetPrivateData(ID3D11DeviceContext* pcThis, REFGUID guid, UINT DataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECHILD::SetPrivateData);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::SetPrivateData, 0, guid);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::SetPrivateData, 1, DataSize);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::SetPrivateData, 2, pData);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECHILD::SetPrivateData);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECHILD::SetPrivateData, HRESULT);

	return D3D11_ID3D11DeviceContext_SetPrivateData_Super(pcThis, guid, DataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateDataInterface
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::SetPrivateDataInterface(ID3D11DeviceContext* pcThis, REFGUID guid, IUnknown* pData)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SetPrivateDataInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECHILD::SetPrivateDataInterface);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::SetPrivateDataInterface, 0, guid);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECHILD::SetPrivateDataInterface, 1, pData);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECHILD::SetPrivateDataInterface);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECHILD::SetPrivateDataInterface, HRESULT);

	return D3D11_ID3D11DeviceContext_SetPrivateDataInterface_Super(pcThis, guid, pData);
}

#pragma endregion

#pragma region ID3D11DeviceContext methods
/**
* void WINAPI VSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::VSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_VSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI PSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::PSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSSetShaderResources);

	D3D11_ID3D11DeviceContext_PSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI PSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::PSSetShader(ID3D11DeviceContext* pcThis, ID3D11PixelShader* pPixelShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShader, 0, pPixelShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSSetShader);

	D3D11_ID3D11DeviceContext_PSSetShader_Super(pcThis, pPixelShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI PSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::PSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSSetSamplers);

	D3D11_ID3D11DeviceContext_PSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI VSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::VSSetShader(ID3D11DeviceContext* pcThis, ID3D11VertexShader* pVertexShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShader, 0, pVertexShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSSetShader);

	D3D11_ID3D11DeviceContext_VSSetShader_Super(pcThis, pVertexShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI DrawIndexed
***/
void WINAPI DCL_ID3D11DeviceContext::DrawIndexed(ID3D11DeviceContext* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawIndexed");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawIndexed);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexed, 0, IndexCount);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexed, 1, StartIndexLocation);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexed, 2, BaseVertexLocation);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawIndexed) D3D11_ID3D11DeviceContext_DrawIndexed_Super(pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawIndexed);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawIndexed);

	D3D11_ID3D11DeviceContext_DrawIndexed_Super(pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);
}

/**
* void WINAPI Draw
***/
void WINAPI DCL_ID3D11DeviceContext::Draw(ID3D11DeviceContext* pcThis, UINT VertexCount, UINT StartVertexLocation)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Draw");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Draw);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Draw, 0, VertexCount);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Draw, 1, StartVertexLocation);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::Draw) D3D11_ID3D11DeviceContext_Draw_Super(pcThis, VertexCount, StartVertexLocation);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Draw);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::Draw);

	D3D11_ID3D11DeviceContext_Draw_Super(pcThis, VertexCount, StartVertexLocation);
}

/**
* HRESULT WINAPI Map
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::Map(ID3D11DeviceContext* pcThis, ID3D11Resource* pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE* pMappedResource)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Map");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Map);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Map, 0, pResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Map, 1, Subresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Map, 2, MapType);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Map, 3, MapFlags);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Map, 4, pMappedResource);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Map);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::Map, HRESULT);

	return D3D11_ID3D11DeviceContext_Map_Super(pcThis, pResource, Subresource, MapType, MapFlags, pMappedResource);
}

/**
* void WINAPI Unmap
***/
void WINAPI DCL_ID3D11DeviceContext::Unmap(ID3D11DeviceContext* pcThis, ID3D11Resource* pResource, UINT Subresource)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Unmap");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Unmap);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Unmap, 0, pResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Unmap, 1, Subresource);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Unmap);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::Unmap);

	D3D11_ID3D11DeviceContext_Unmap_Super(pcThis, pResource, Subresource);
}

/**
* void WINAPI PSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::PSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_PSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI IASetInputLayout
***/
void WINAPI DCL_ID3D11DeviceContext::IASetInputLayout(ID3D11DeviceContext* pcThis, ID3D11InputLayout* pInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IASetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IASetInputLayout);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetInputLayout, 0, pInputLayout);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IASetInputLayout);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IASetInputLayout);

	D3D11_ID3D11DeviceContext_IASetInputLayout_Super(pcThis, pInputLayout);
}

/**
* void WINAPI IASetVertexBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::IASetVertexBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IASetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers, 2, ppVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers, 3, pStrides);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers, 4, pOffsets);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers);

	D3D11_ID3D11DeviceContext_IASetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* void WINAPI IASetIndexBuffer
***/
void WINAPI DCL_ID3D11DeviceContext::IASetIndexBuffer(ID3D11DeviceContext* pcThis, ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IASetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer, 0, pIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer, 1, Format);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer, 2, Offset);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer);

	D3D11_ID3D11DeviceContext_IASetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
* void WINAPI DrawIndexedInstanced
***/
void WINAPI DCL_ID3D11DeviceContext::DrawIndexedInstanced(ID3D11DeviceContext* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawIndexedInstanced");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced, 0, IndexCountPerInstance);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced, 1, InstanceCount);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced, 2, StartIndexLocation);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced, 3, BaseVertexLocation);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced, 4, StartInstanceLocation);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced) D3D11_ID3D11DeviceContext_DrawIndexedInstanced_Super(pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced);

	D3D11_ID3D11DeviceContext_DrawIndexedInstanced_Super(pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

/**
* void WINAPI DrawInstanced
***/
void WINAPI DCL_ID3D11DeviceContext::DrawInstanced(ID3D11DeviceContext* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawInstanced");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawInstanced);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstanced, 0, VertexCountPerInstance);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstanced, 1, InstanceCount);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstanced, 2, StartVertexLocation);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstanced, 3, StartInstanceLocation);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawInstanced) D3D11_ID3D11DeviceContext_DrawInstanced_Super(pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawInstanced);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawInstanced);

	D3D11_ID3D11DeviceContext_DrawInstanced_Super(pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/**
* void WINAPI GSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::GSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_GSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI GSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::GSSetShader(ID3D11DeviceContext* pcThis, ID3D11GeometryShader* pShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShader, 0, pShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSSetShader);

	D3D11_ID3D11DeviceContext_GSSetShader_Super(pcThis, pShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI IASetPrimitiveTopology
***/
void WINAPI DCL_ID3D11DeviceContext::IASetPrimitiveTopology(ID3D11DeviceContext* pcThis, D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IASetPrimitiveTopology");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology, 0, Topology);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology);

	D3D11_ID3D11DeviceContext_IASetPrimitiveTopology_Super(pcThis, Topology);
}

/**
* void WINAPI VSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::VSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSSetShaderResources);

	D3D11_ID3D11DeviceContext_VSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI VSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::VSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSSetSamplers);

	D3D11_ID3D11DeviceContext_VSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI Begin
***/
void WINAPI DCL_ID3D11DeviceContext::Begin(ID3D11DeviceContext* pcThis, ID3D11Asynchronous* pAsync)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Begin");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Begin);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Begin, 0, pAsync);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Begin);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::Begin);

	D3D11_ID3D11DeviceContext_Begin_Super(pcThis, pAsync);
}

/**
* void WINAPI End
***/
void WINAPI DCL_ID3D11DeviceContext::End(ID3D11DeviceContext* pcThis, ID3D11Asynchronous* pAsync)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::End");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::End);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::End, 0, pAsync);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::End);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::End);

	D3D11_ID3D11DeviceContext_End_Super(pcThis, pAsync);
}

/**
* HRESULT WINAPI GetData
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::GetData(ID3D11DeviceContext* pcThis, ID3D11Asynchronous* pAsync, void* pData, UINT DataSize, UINT GetDataFlags)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GetData);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetData, 0, pAsync);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetData, 1, pData);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetData, 2, DataSize);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetData, 3, GetDataFlags);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GetData);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::GetData, HRESULT);

	return D3D11_ID3D11DeviceContext_GetData_Super(pcThis, pAsync, pData, DataSize, GetDataFlags);
}

/**
* void WINAPI SetPredication
***/
void WINAPI DCL_ID3D11DeviceContext::SetPredication(ID3D11DeviceContext* pcThis, ID3D11Predicate* pPredicate, BOOL PredicateValue)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::SetPredication);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SetPredication, 0, pPredicate);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SetPredication, 1, PredicateValue);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::SetPredication);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::SetPredication);

	D3D11_ID3D11DeviceContext_SetPredication_Super(pcThis, pPredicate, PredicateValue);
}

/**
* void WINAPI GSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::GSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSSetShaderResources);

	D3D11_ID3D11DeviceContext_GSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI GSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::GSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSSetSamplers);

	D3D11_ID3D11DeviceContext_GSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI OMSetRenderTargets
***/
void WINAPI DCL_ID3D11DeviceContext::OMSetRenderTargets(ID3D11DeviceContext* pcThis, UINT NumViews, ID3D11RenderTargetView*const * ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMSetRenderTargets");

	if (!m_pcSwapChain)
	{
		if (NumViews)
		{
			if (ppRenderTargetViews)
			{
				if (ppRenderTargetViews[0])
				{
					// get the buffer texture
					ID3D11Texture2D* pBuffer = nullptr;
					ppRenderTargetViews[0]->GetResource((ID3D11Resource**)(&pBuffer));

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
							else OutputDebugString(L"Failed to get swapchain!");

							if (pSurface) { pSurface->Release(); pSurface = nullptr; }
						}
						else OutputDebugString(L"No Surface!");
						if (pBuffer) { pBuffer->Release(); pBuffer = nullptr; }
					}
					else OutputDebugString(L"No Buffer!");
				}
			}
		}
		else
		{
			//ID3D11RenderTargetView* pcRTV = nullptr;
			//ID3D11DepthStencilView* pcDSV = nullptr;
			//D3D11_ID3D11DeviceContext_OMGetRenderTargets_Super(pcThis, 1, &pcRTV, &pcDSV);
			////if (!pcRTV) OutputDebugString(L"Failed to get render target !"); else pcRTV->Release();
			//if (pcRTV)
			//{
			//	OutputDebugString(L"Render Target");
			//	pcRTV->Release();
			//}
			//if (pcDSV)
			//{
			//	OutputDebugString(L"Depth Stencil");
			//	pcDSV->Release();
			//}
		}
	}

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets, 0, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets, 1, ppRenderTargetViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets, 2, pDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets);

	D3D11_ID3D11DeviceContext_OMSetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, pDepthStencilView);
}

/**
* void WINAPI OMSetRenderTargetsAndUnorderedAccessViews
***/
void WINAPI DCL_ID3D11DeviceContext::OMSetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext* pcThis, UINT NumRTVs, ID3D11RenderTargetView*const * ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView*const * ppUnorderedAccessViews, const UINT* pUAVInitialCounts)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMSetRenderTargetsAndUnorderedAccessViews");

	if (!m_pcSwapChain)
	{
		if (NumRTVs)
		{
			if (ppRenderTargetViews)
			{
				if (ppRenderTargetViews[0])
				{
					// get the buffer texture
					ID3D11Texture2D* pBuffer = nullptr;
					ppRenderTargetViews[0]->GetResource((ID3D11Resource**)(&pBuffer));

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
							// else OutputDebugString(L"Failed to get swapchain!");

							if (pSurface) { pSurface->Release(); pSurface = nullptr; }
						}
						// else OutputDebugString(L"No Surface!");
						if (pBuffer) { pBuffer->Release(); pBuffer = nullptr; }
					}
				}
			}
		}
	}

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 0, NumRTVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 1, ppRenderTargetViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 2, pDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 3, UAVStartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 4, NumUAVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 5, ppUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews, 6, pUAVInitialCounts);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews);

	D3D11_ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews_Super(pcThis, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

/**
* void WINAPI OMSetBlendState
***/
void WINAPI DCL_ID3D11DeviceContext::OMSetBlendState(ID3D11DeviceContext* pcThis, ID3D11BlendState* pBlendState, const FLOAT BlendFactor[4], UINT SampleMask)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMSetBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMSetBlendState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetBlendState, 0, pBlendState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetBlendState, 1, BlendFactor);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetBlendState, 2, SampleMask);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMSetBlendState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMSetBlendState);

	D3D11_ID3D11DeviceContext_OMSetBlendState_Super(pcThis, pBlendState, BlendFactor, SampleMask);
}

/**
* void WINAPI OMSetDepthStencilState
***/
void WINAPI DCL_ID3D11DeviceContext::OMSetDepthStencilState(ID3D11DeviceContext* pcThis, ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMSetDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState, 0, pDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState, 1, StencilRef);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState);

	D3D11_ID3D11DeviceContext_OMSetDepthStencilState_Super(pcThis, pDepthStencilState, StencilRef);
}

/**
* void WINAPI SOSetTargets
***/
void WINAPI DCL_ID3D11DeviceContext::SOSetTargets(ID3D11DeviceContext* pcThis, UINT NumBuffers, ID3D11Buffer*const * ppSOTargets, const UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SOSetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::SOSetTargets);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SOSetTargets, 0, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SOSetTargets, 1, ppSOTargets);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SOSetTargets, 2, pOffsets);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::SOSetTargets);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::SOSetTargets);

	D3D11_ID3D11DeviceContext_SOSetTargets_Super(pcThis, NumBuffers, ppSOTargets, pOffsets);
}

/**
* void WINAPI DrawAuto
***/
void WINAPI DCL_ID3D11DeviceContext::DrawAuto(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawAuto");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawAuto);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawAuto) D3D11_ID3D11DeviceContext_DrawAuto_Super(pcThis);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawAuto);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawAuto);

	D3D11_ID3D11DeviceContext_DrawAuto_Super(pcThis);
}

/**
* void WINAPI DrawIndexedInstancedIndirect
***/
void WINAPI DCL_ID3D11DeviceContext::DrawIndexedInstancedIndirect(ID3D11DeviceContext* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawIndexedInstancedIndirect");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect, 0, pBufferForArgs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect, 1, AlignedByteOffsetForArgs);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect) D3D11_ID3D11DeviceContext_DrawIndexedInstancedIndirect_Super(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect);

	D3D11_ID3D11DeviceContext_DrawIndexedInstancedIndirect_Super(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);
}

/**
* void WINAPI DrawInstancedIndirect
***/
void WINAPI DCL_ID3D11DeviceContext::DrawInstancedIndirect(ID3D11DeviceContext* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DrawInstancedIndirect");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect, 0, pBufferForArgs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect, 1, AlignedByteOffsetForArgs);
	AQU_ID3D11DEVICECONTEXT_PRECALL_______(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect) D3D11_ID3D11DeviceContext_DrawInstancedIndirect_Super(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect);

	D3D11_ID3D11DeviceContext_DrawInstancedIndirect_Super(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);
}

/**
* void WINAPI Dispatch
***/
void WINAPI DCL_ID3D11DeviceContext::Dispatch(ID3D11DeviceContext* pcThis, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Dispatch");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Dispatch);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Dispatch, 0, ThreadGroupCountX);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Dispatch, 1, ThreadGroupCountY);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::Dispatch, 2, ThreadGroupCountZ);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Dispatch);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::Dispatch);

	D3D11_ID3D11DeviceContext_Dispatch_Super(pcThis, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

/**
* void WINAPI DispatchIndirect
***/
void WINAPI DCL_ID3D11DeviceContext::DispatchIndirect(ID3D11DeviceContext* pcThis, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DispatchIndirect");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DispatchIndirect);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DispatchIndirect, 0, pBufferForArgs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DispatchIndirect, 1, AlignedByteOffsetForArgs);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DispatchIndirect);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DispatchIndirect);

	D3D11_ID3D11DeviceContext_DispatchIndirect_Super(pcThis, pBufferForArgs, AlignedByteOffsetForArgs);
}

/**
* void WINAPI RSSetState
***/
void WINAPI DCL_ID3D11DeviceContext::RSSetState(ID3D11DeviceContext* pcThis, ID3D11RasterizerState* pRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSSetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSSetState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSSetState, 0, pRasterizerState);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSSetState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSSetState);

	D3D11_ID3D11DeviceContext_RSSetState_Super(pcThis, pRasterizerState);
}

/**
* void WINAPI RSSetViewports
***/
void WINAPI DCL_ID3D11DeviceContext::RSSetViewports(ID3D11DeviceContext* pcThis, UINT NumViewports, const D3D11_VIEWPORT* pViewports)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSSetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSSetViewports);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSSetViewports, 0, NumViewports);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSSetViewports, 1, pViewports);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSSetViewports);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSSetViewports);

	D3D11_ID3D11DeviceContext_RSSetViewports_Super(pcThis, NumViewports, pViewports);
}

/**
* void WINAPI RSSetScissorRects
***/
void WINAPI DCL_ID3D11DeviceContext::RSSetScissorRects(ID3D11DeviceContext* pcThis, UINT NumRects, const D3D11_RECT* pRects)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSSetScissorRects");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects, 0, NumRects);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects, 1, pRects);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSSetScissorRects);

	D3D11_ID3D11DeviceContext_RSSetScissorRects_Super(pcThis, NumRects, pRects);
}

/**
* void WINAPI CopySubresourceRegion
***/
void WINAPI DCL_ID3D11DeviceContext::CopySubresourceRegion(ID3D11DeviceContext* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CopySubresourceRegion");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 0, pDstResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 1, DstSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 2, DstX);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 3, DstY);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 4, DstZ);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 5, pSrcResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 6, SrcSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion, 7, pSrcBox);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion);

	D3D11_ID3D11DeviceContext_CopySubresourceRegion_Super(pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
}

/**
* void WINAPI CopyResource
***/
void WINAPI DCL_ID3D11DeviceContext::CopyResource(ID3D11DeviceContext* pcThis, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CopyResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CopyResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopyResource, 0, pDstResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopyResource, 1, pSrcResource);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CopyResource);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CopyResource);

	D3D11_ID3D11DeviceContext_CopyResource_Super(pcThis, pDstResource, pSrcResource);
}

/**
* void WINAPI UpdateSubresource
***/
void WINAPI DCL_ID3D11DeviceContext::UpdateSubresource(ID3D11DeviceContext* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::UpdateSubresource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::UpdateSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 0, pDstResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 1, DstSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 2, pDstBox);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 3, pSrcData);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 4, SrcRowPitch);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource, 5, SrcDepthPitch);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::UpdateSubresource);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::UpdateSubresource);

	D3D11_ID3D11DeviceContext_UpdateSubresource_Super(pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

/**
* void WINAPI CopyStructureCount
***/
void WINAPI DCL_ID3D11DeviceContext::CopyStructureCount(ID3D11DeviceContext* pcThis, ID3D11Buffer* pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView* pSrcView)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CopyStructureCount");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CopyStructureCount);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopyStructureCount, 0, pDstBuffer);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopyStructureCount, 1, DstAlignedByteOffset);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CopyStructureCount, 2, pSrcView);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CopyStructureCount);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CopyStructureCount);

	D3D11_ID3D11DeviceContext_CopyStructureCount_Super(pcThis, pDstBuffer, DstAlignedByteOffset, pSrcView);
}

/**
* void WINAPI ClearRenderTargetView
***/
void WINAPI DCL_ID3D11DeviceContext::ClearRenderTargetView(ID3D11DeviceContext* pcThis, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ClearRenderTargetView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView, 0, pRenderTargetView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView, 1, ColorRGBA);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView);

	D3D11_ID3D11DeviceContext_ClearRenderTargetView_Super(pcThis, pRenderTargetView, ColorRGBA);
}

/**
* void WINAPI ClearUnorderedAccessViewUint
***/
void WINAPI DCL_ID3D11DeviceContext::ClearUnorderedAccessViewUint(ID3D11DeviceContext* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, const UINT Values[4])
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ClearUnorderedAccessViewUint");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint, 0, pUnorderedAccessView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint, 1, Values);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint);

	D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewUint_Super(pcThis, pUnorderedAccessView, Values);
}

/**
* void WINAPI ClearUnorderedAccessViewFloat
***/
void WINAPI DCL_ID3D11DeviceContext::ClearUnorderedAccessViewFloat(ID3D11DeviceContext* pcThis, ID3D11UnorderedAccessView* pUnorderedAccessView, const FLOAT Values[4])
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ClearUnorderedAccessViewFloat");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat, 0, pUnorderedAccessView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat, 1, Values);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat);

	D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewFloat_Super(pcThis, pUnorderedAccessView, Values);
}

/**
* void WINAPI ClearDepthStencilView
***/
void WINAPI DCL_ID3D11DeviceContext::ClearDepthStencilView(ID3D11DeviceContext* pcThis, ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ClearDepthStencilView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView, 0, pDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView, 1, ClearFlags);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView, 2, Depth);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView, 3, Stencil);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView);

	D3D11_ID3D11DeviceContext_ClearDepthStencilView_Super(pcThis, pDepthStencilView, ClearFlags, Depth, Stencil);
}

/**
* void WINAPI GenerateMips
***/
void WINAPI DCL_ID3D11DeviceContext::GenerateMips(ID3D11DeviceContext* pcThis, ID3D11ShaderResourceView* pShaderResourceView)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GenerateMips");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GenerateMips);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GenerateMips, 0, pShaderResourceView);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GenerateMips);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GenerateMips);

	D3D11_ID3D11DeviceContext_GenerateMips_Super(pcThis, pShaderResourceView);
}

/**
* void WINAPI SetResourceMinLOD
***/
void WINAPI DCL_ID3D11DeviceContext::SetResourceMinLOD(ID3D11DeviceContext* pcThis, ID3D11Resource* pResource, FLOAT MinLOD)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SetResourceMinLOD");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD, 0, pResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD, 1, MinLOD);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD);

	D3D11_ID3D11DeviceContext_SetResourceMinLOD_Super(pcThis, pResource, MinLOD);
}

/**
* FLOAT WINAPI GetResourceMinLOD
***/
FLOAT WINAPI DCL_ID3D11DeviceContext::GetResourceMinLOD(ID3D11DeviceContext* pcThis, ID3D11Resource* pResource)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetResourceMinLOD");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD, 0, pResource);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD, FLOAT);

	return D3D11_ID3D11DeviceContext_GetResourceMinLOD_Super(pcThis, pResource);
}

/**
* void WINAPI ResolveSubresource
***/
void WINAPI DCL_ID3D11DeviceContext::ResolveSubresource(ID3D11DeviceContext* pcThis, ID3D11Resource* pDstResource, UINT DstSubresource, ID3D11Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ResolveSubresource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ResolveSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource, 0, pDstResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource, 1, DstSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource, 2, pSrcResource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource, 3, SrcSubresource);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource, 4, Format);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ResolveSubresource);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ResolveSubresource);

	D3D11_ID3D11DeviceContext_ResolveSubresource_Super(pcThis, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

/**
* void WINAPI ExecuteCommandList
***/
void WINAPI DCL_ID3D11DeviceContext::ExecuteCommandList(ID3D11DeviceContext* pcThis, ID3D11CommandList* pCommandList, BOOL RestoreContextState)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ExecuteCommandList");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList, 0, pCommandList);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList, 1, RestoreContextState);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ExecuteCommandList);

	D3D11_ID3D11DeviceContext_ExecuteCommandList_Super(pcThis, pCommandList, RestoreContextState);
}

/**
* void WINAPI HSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::HSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSSetShaderResources);

	D3D11_ID3D11DeviceContext_HSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI HSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::HSSetShader(ID3D11DeviceContext* pcThis, ID3D11HullShader* pHullShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShader, 0, pHullShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSSetShader);

	D3D11_ID3D11DeviceContext_HSSetShader_Super(pcThis, pHullShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI HSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::HSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSSetSamplers);

	D3D11_ID3D11DeviceContext_HSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI HSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::HSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_HSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI DSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::DSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSSetShaderResources);

	D3D11_ID3D11DeviceContext_DSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI DSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::DSSetShader(ID3D11DeviceContext* pcThis, ID3D11DomainShader* pDomainShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShader, 0, pDomainShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSSetShader);

	D3D11_ID3D11DeviceContext_DSSetShader_Super(pcThis, pDomainShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI DSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::DSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSSetSamplers);

	D3D11_ID3D11DeviceContext_DSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI DSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::DSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_DSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI CSSetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::CSSetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView*const * ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSSetShaderResources);

	D3D11_ID3D11DeviceContext_CSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI CSSetUnorderedAccessViews
***/
void WINAPI DCL_ID3D11DeviceContext::CSSetUnorderedAccessViews(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView*const * ppUnorderedAccessViews, const UINT* pUAVInitialCounts)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSSetUnorderedAccessViews");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews, 1, NumUAVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews, 2, ppUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews, 3, pUAVInitialCounts);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews);

	D3D11_ID3D11DeviceContext_CSSetUnorderedAccessViews_Super(pcThis, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

/**
* void WINAPI CSSetShader
***/
void WINAPI DCL_ID3D11DeviceContext::CSSetShader(ID3D11DeviceContext* pcThis, ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance*const * ppClassInstances, UINT NumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSSetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShader, 0, pComputeShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetShader, 2, NumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSSetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSSetShader);

	D3D11_ID3D11DeviceContext_CSSetShader_Super(pcThis, pComputeShader, ppClassInstances, NumClassInstances);
}

/**
* void WINAPI CSSetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::CSSetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState*const * ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSSetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSSetSamplers);

	D3D11_ID3D11DeviceContext_CSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI CSSetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::CSSetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer*const * ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers);

	D3D11_ID3D11DeviceContext_CSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI VSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::VSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_VSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI PSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::PSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSGetShaderResources);

	D3D11_ID3D11DeviceContext_PSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI PSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::PSGetShader(ID3D11DeviceContext* pcThis, ID3D11PixelShader** ppPixelShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShader, 0, ppPixelShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSGetShader);

	D3D11_ID3D11DeviceContext_PSGetShader_Super(pcThis, ppPixelShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI PSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::PSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSGetSamplers);

	D3D11_ID3D11DeviceContext_PSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI VSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::VSGetShader(ID3D11DeviceContext* pcThis, ID3D11VertexShader** ppVertexShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShader, 0, ppVertexShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSGetShader);

	D3D11_ID3D11DeviceContext_VSGetShader_Super(pcThis, ppVertexShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI PSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::PSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::PSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_PSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI IAGetInputLayout
***/
void WINAPI DCL_ID3D11DeviceContext::IAGetInputLayout(ID3D11DeviceContext* pcThis, ID3D11InputLayout** ppInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IAGetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IAGetInputLayout);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetInputLayout, 0, ppInputLayout);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IAGetInputLayout);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IAGetInputLayout);

	D3D11_ID3D11DeviceContext_IAGetInputLayout_Super(pcThis, ppInputLayout);
}

/**
* void WINAPI IAGetVertexBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::IAGetVertexBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IAGetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers, 2, ppVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers, 3, pStrides);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers, 4, pOffsets);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers);

	D3D11_ID3D11DeviceContext_IAGetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* void WINAPI IAGetIndexBuffer
***/
void WINAPI DCL_ID3D11DeviceContext::IAGetIndexBuffer(ID3D11DeviceContext* pcThis, ID3D11Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IAGetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer, 0, pIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer, 1, Format);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer, 2, Offset);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer);

	D3D11_ID3D11DeviceContext_IAGetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
* void WINAPI GSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::GSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_GSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI GSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::GSGetShader(ID3D11DeviceContext* pcThis, ID3D11GeometryShader** ppGeometryShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShader, 0, ppGeometryShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSGetShader);

	D3D11_ID3D11DeviceContext_GSGetShader_Super(pcThis, ppGeometryShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI IAGetPrimitiveTopology
***/
void WINAPI DCL_ID3D11DeviceContext::IAGetPrimitiveTopology(ID3D11DeviceContext* pcThis, D3D11_PRIMITIVE_TOPOLOGY* pTopology)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::IAGetPrimitiveTopology");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology, 0, pTopology);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology);

	D3D11_ID3D11DeviceContext_IAGetPrimitiveTopology_Super(pcThis, pTopology);
}

/**
* void WINAPI VSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::VSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSGetShaderResources);

	D3D11_ID3D11DeviceContext_VSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI VSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::VSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::VSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::VSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::VSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::VSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::VSGetSamplers);

	D3D11_ID3D11DeviceContext_VSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI GetPredication
***/
void WINAPI DCL_ID3D11DeviceContext::GetPredication(ID3D11DeviceContext* pcThis, ID3D11Predicate** ppPredicate, BOOL* pPredicateValue)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GetPredication);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetPredication, 0, ppPredicate);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GetPredication, 1, pPredicateValue);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GetPredication);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GetPredication);

	D3D11_ID3D11DeviceContext_GetPredication_Super(pcThis, ppPredicate, pPredicateValue);
}

/**
* void WINAPI GSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::GSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSGetShaderResources);

	D3D11_ID3D11DeviceContext_GSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI GSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::GSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::GSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::GSGetSamplers);

	D3D11_ID3D11DeviceContext_GSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI OMGetRenderTargets
***/
void WINAPI DCL_ID3D11DeviceContext::OMGetRenderTargets(ID3D11DeviceContext* pcThis, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMGetRenderTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets, 0, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets, 1, ppRenderTargetViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets, 2, ppDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets);

	D3D11_ID3D11DeviceContext_OMGetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, ppDepthStencilView);
}

/**
* void WINAPI OMGetRenderTargetsAndUnorderedAccessViews
***/
void WINAPI DCL_ID3D11DeviceContext::OMGetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext* pcThis, UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMGetRenderTargetsAndUnorderedAccessViews");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 0, NumRTVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 1, ppRenderTargetViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 2, ppDepthStencilView);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 3, UAVStartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 4, NumUAVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews, 5, ppUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews);

	D3D11_ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews_Super(pcThis, NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
}

/**
* void WINAPI OMGetBlendState
***/
void WINAPI DCL_ID3D11DeviceContext::OMGetBlendState(ID3D11DeviceContext* pcThis, ID3D11BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMGetBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMGetBlendState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetBlendState, 0, ppBlendState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetBlendState, 1, BlendFactor);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetBlendState, 2, pSampleMask);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMGetBlendState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMGetBlendState);

	D3D11_ID3D11DeviceContext_OMGetBlendState_Super(pcThis, ppBlendState, BlendFactor, pSampleMask);
}

/**
* void WINAPI OMGetDepthStencilState
***/
void WINAPI DCL_ID3D11DeviceContext::OMGetDepthStencilState(ID3D11DeviceContext* pcThis, ID3D11DepthStencilState** ppDepthStencilState, UINT* pStencilRef)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::OMGetDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState, 0, ppDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState, 1, pStencilRef);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState);

	D3D11_ID3D11DeviceContext_OMGetDepthStencilState_Super(pcThis, ppDepthStencilState, pStencilRef);
}

/**
* void WINAPI SOGetTargets
***/
void WINAPI DCL_ID3D11DeviceContext::SOGetTargets(ID3D11DeviceContext* pcThis, UINT NumBuffers, ID3D11Buffer** ppSOTargets)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::SOGetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::SOGetTargets);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SOGetTargets, 0, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::SOGetTargets, 1, ppSOTargets);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::SOGetTargets);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::SOGetTargets);

	D3D11_ID3D11DeviceContext_SOGetTargets_Super(pcThis, NumBuffers, ppSOTargets);
}

/**
* void WINAPI RSGetState
***/
void WINAPI DCL_ID3D11DeviceContext::RSGetState(ID3D11DeviceContext* pcThis, ID3D11RasterizerState** ppRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSGetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSGetState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSGetState, 0, ppRasterizerState);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSGetState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSGetState);

	D3D11_ID3D11DeviceContext_RSGetState_Super(pcThis, ppRasterizerState);
}

/**
* void WINAPI RSGetViewports
***/
void WINAPI DCL_ID3D11DeviceContext::RSGetViewports(ID3D11DeviceContext* pcThis, UINT* pNumViewports, D3D11_VIEWPORT* pViewports)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSGetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSGetViewports);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSGetViewports, 0, pNumViewports);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSGetViewports, 1, pViewports);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSGetViewports);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSGetViewports);

	D3D11_ID3D11DeviceContext_RSGetViewports_Super(pcThis, pNumViewports, pViewports);
}

/**
* void WINAPI RSGetScissorRects
***/
void WINAPI DCL_ID3D11DeviceContext::RSGetScissorRects(ID3D11DeviceContext* pcThis, UINT* pNumRects, D3D11_RECT* pRects)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::RSGetScissorRects");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects, 0, pNumRects);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects, 1, pRects);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::RSGetScissorRects);

	D3D11_ID3D11DeviceContext_RSGetScissorRects_Super(pcThis, pNumRects, pRects);
}

/**
* void WINAPI HSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::HSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSGetShaderResources);

	D3D11_ID3D11DeviceContext_HSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI HSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::HSGetShader(ID3D11DeviceContext* pcThis, ID3D11HullShader** ppHullShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShader, 0, ppHullShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSGetShader);

	D3D11_ID3D11DeviceContext_HSGetShader_Super(pcThis, ppHullShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI HSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::HSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSGetSamplers);

	D3D11_ID3D11DeviceContext_HSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI HSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::HSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::HSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_HSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI DSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::DSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSGetShaderResources);

	D3D11_ID3D11DeviceContext_DSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI DSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::DSGetShader(ID3D11DeviceContext* pcThis, ID3D11DomainShader** ppDomainShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShader, 0, ppDomainShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSGetShader);

	D3D11_ID3D11DeviceContext_DSGetShader_Super(pcThis, ppDomainShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI DSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::DSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSGetSamplers);

	D3D11_ID3D11DeviceContext_DSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI DSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::DSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::DSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_DSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI CSGetShaderResources
***/
void WINAPI DCL_ID3D11DeviceContext::CSGetShaderResources(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources, 1, NumViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources, 2, ppShaderResourceViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSGetShaderResources);

	D3D11_ID3D11DeviceContext_CSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI CSGetUnorderedAccessViews
***/
void WINAPI DCL_ID3D11DeviceContext::CSGetUnorderedAccessViews(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSGetUnorderedAccessViews");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews, 1, NumUAVs);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews, 2, ppUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews);

	D3D11_ID3D11DeviceContext_CSGetUnorderedAccessViews_Super(pcThis, StartSlot, NumUAVs, ppUnorderedAccessViews);
}

/**
* void WINAPI CSGetShader
***/
void WINAPI DCL_ID3D11DeviceContext::CSGetShader(ID3D11DeviceContext* pcThis, ID3D11ComputeShader** ppComputeShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSGetShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShader, 0, ppComputeShader);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShader, 1, ppClassInstances);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetShader, 2, pNumClassInstances);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSGetShader);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSGetShader);

	D3D11_ID3D11DeviceContext_CSGetShader_Super(pcThis, ppComputeShader, ppClassInstances, pNumClassInstances);
}

/**
* void WINAPI CSGetSamplers
***/
void WINAPI DCL_ID3D11DeviceContext::CSGetSamplers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetSamplers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetSamplers, 1, NumSamplers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetSamplers, 2, ppSamplers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSGetSamplers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSGetSamplers);

	D3D11_ID3D11DeviceContext_CSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI CSGetConstantBuffers
***/
void WINAPI DCL_ID3D11DeviceContext::CSGetConstantBuffers(ID3D11DeviceContext* pcThis, UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::CSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers, 0, StartSlot);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers, 1, NumBuffers);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers, 2, ppConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers);

	D3D11_ID3D11DeviceContext_CSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI ClearState
***/
void WINAPI DCL_ID3D11DeviceContext::ClearState(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::ClearState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::ClearState);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::ClearState);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::ClearState);

	D3D11_ID3D11DeviceContext_ClearState_Super(pcThis);
}

/**
* void WINAPI Flush
***/
void WINAPI DCL_ID3D11DeviceContext::Flush(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::Flush");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::Flush);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::Flush);
	AQU_ID3D11DEVICECONTEXT_REPLACE_VOID__(VMT_ID3D11DEVICECONTEXT::Flush);

	D3D11_ID3D11DeviceContext_Flush_Super(pcThis);
}

/**
* D3D11_DEVICE_CONTEXT_TYPE WINAPI GetType
***/
D3D11_DEVICE_CONTEXT_TYPE WINAPI DCL_ID3D11DeviceContext::GetType(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetType");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GetType);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GetType);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::GetType, D3D11_DEVICE_CONTEXT_TYPE);

	return D3D11_ID3D11DeviceContext_GetType_Super(pcThis);
}

/**
* UINT WINAPI GetContextFlags
***/
UINT WINAPI DCL_ID3D11DeviceContext::GetContextFlags(ID3D11DeviceContext* pcThis)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::GetContextFlags");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::GetContextFlags);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::GetContextFlags);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::GetContextFlags, UINT);

	return D3D11_ID3D11DeviceContext_GetContextFlags_Super(pcThis);
}

/**
* HRESULT WINAPI FinishCommandList
***/
HRESULT WINAPI DCL_ID3D11DeviceContext::FinishCommandList(ID3D11DeviceContext* pcThis, BOOL RestoreDeferredContextState, ID3D11CommandList** ppCommandList)
{
	OUTPUT_DEBUG_STRING_ID3D11DeviceContext(L"ID3D11DeviceContext::FinishCommandList");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D11DEVICECONTEXT_HEADER________(VMT_ID3D11DEVICECONTEXT::FinishCommandList);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::FinishCommandList, 0, RestoreDeferredContextState);
	AQU_ID3D11DEVICECONTEXT_SET_DATA______(VMT_ID3D11DEVICECONTEXT::FinishCommandList, 1, ppCommandList);
	AQU_ID3D11DEVICECONTEXT_PROVOKE_______(VMT_ID3D11DEVICECONTEXT::FinishCommandList);
	AQU_ID3D11DEVICECONTEXT_REPLACE_METHOD(VMT_ID3D11DEVICECONTEXT::FinishCommandList, HRESULT);

	return D3D11_ID3D11DeviceContext_FinishCommandList_Super(pcThis, RestoreDeferredContextState, ppCommandList);
}

#pragma endregion

#pragma region DCL_ID3D11DeviceContext public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11DeviceContext.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11DeviceContext::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IUnknown super methods ***/
		case VMT_IUNKNOWN::QueryInterface:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_QueryInterface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::AddRef:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_AddRef_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::Release:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Release_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11DeviceContext.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11DeviceContext::SetSuperFunctionPointer(VMT_ID3D11DEVICECHILD::VMT_ID3D11DeviceChild eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** ID3D11DeviceChild super methods ***/
		case VMT_ID3D11DEVICECHILD::GetDevice:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetDevice_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECHILD::GetPrivateData:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECHILD::SetPrivateData:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECHILD::SetPrivateDataInterface:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D11DeviceContext.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D11DeviceContext::SetSuperFunctionPointer(VMT_ID3D11DEVICECONTEXT::VMT_ID3D11DeviceContext eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** ID3D11DeviceContext super methods ***/
		case VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawIndexed:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexed_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Draw:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Draw_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Map:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Map_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Unmap:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Unmap_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IASetInputLayout:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetInputLayout_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetVertexBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetIndexBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexedInstanced_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawInstanced:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawInstanced_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetPrimitiveTopology_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Begin:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Begin_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::End:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_End_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GetData:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetData_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::SetPredication:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetPredication_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetRenderTargets_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMSetBlendState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetBlendState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetDepthStencilState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::SOSetTargets:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SOSetTargets_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawAuto:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawAuto_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexedInstancedIndirect_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawInstancedIndirect_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Dispatch:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Dispatch_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DispatchIndirect:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DispatchIndirect_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSSetState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSSetViewports:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetViewports_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSSetScissorRects:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetScissorRects_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopySubresourceRegion_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CopyResource:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopyResource_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::UpdateSubresource:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_UpdateSubresource_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CopyStructureCount:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopyStructureCount_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearRenderTargetView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewUint_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewFloat_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearDepthStencilView_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GenerateMips:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GenerateMips_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetResourceMinLOD_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetResourceMinLOD_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ResolveSubresource:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ResolveSubresource_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ExecuteCommandList:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ExecuteCommandList_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSSetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetUnorderedAccessViews_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSSetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSSetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IAGetInputLayout:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetInputLayout_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetVertexBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetIndexBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetPrimitiveTopology_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::VSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GetPredication:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetPredication_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetRenderTargets_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMGetBlendState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetBlendState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetDepthStencilState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::SOGetTargets:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SOGetTargets_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSGetState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSGetViewports:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetViewports_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::RSGetScissorRects:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetScissorRects_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSGetShaderResources:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetUnorderedAccessViews_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSGetShader:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSGetSamplers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetSamplers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::ClearState:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearState_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::Flush:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Flush_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GetType:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetType_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::GetContextFlags:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetContextFlags_Super = (UINT_PTR)dwFunc; break;
		case VMT_ID3D11DEVICECONTEXT::FinishCommandList:*(PUINT_PTR)&D3D11_ID3D11DeviceContext_FinishCommandList_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_ID3D11DeviceContext::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];
	/*** ID3D11DeviceContext super methods ***/
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexed_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexed];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Draw_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Draw];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Map_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Map];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Unmap_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Unmap];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IASetInputLayout];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetVertexBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IASetVertexBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetIndexBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IASetIndexBuffer];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexedInstanced_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstanced];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawInstanced_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawInstanced];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IASetPrimitiveTopology_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IASetPrimitiveTopology];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Begin_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Begin];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_End_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::End];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetData_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GetData];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetPredication_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::SetPredication];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetRenderTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargets];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMSetRenderTargetsAndUnorderedAccessViews];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMSetBlendState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMSetDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMSetDepthStencilState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SOSetTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::SOSetTargets];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawAuto_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawAuto];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawIndexedInstancedIndirect_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawIndexedInstancedIndirect];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DrawInstancedIndirect_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DrawInstancedIndirect];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Dispatch_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Dispatch];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DispatchIndirect_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DispatchIndirect];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSSetState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetViewports_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSSetViewports];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSSetScissorRects_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSSetScissorRects];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopySubresourceRegion_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CopySubresourceRegion];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopyResource_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CopyResource];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_UpdateSubresource_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::UpdateSubresource];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CopyStructureCount_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CopyStructureCount];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearRenderTargetView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ClearRenderTargetView];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewUint_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewUint];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearUnorderedAccessViewFloat_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ClearUnorderedAccessViewFloat];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearDepthStencilView_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ClearDepthStencilView];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GenerateMips_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GenerateMips];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SetResourceMinLOD_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::SetResourceMinLOD];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetResourceMinLOD_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GetResourceMinLOD];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ResolveSubresource_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ResolveSubresource];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ExecuteCommandList_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ExecuteCommandList];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSSetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetUnorderedAccessViews_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSSetUnorderedAccessViews];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSSetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSSetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSSetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_PSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::PSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IAGetInputLayout];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetVertexBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IAGetVertexBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetIndexBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IAGetIndexBuffer];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_IAGetPrimitiveTopology_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::IAGetPrimitiveTopology];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_VSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::VSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetPredication_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GetPredication];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetRenderTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargets];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMGetRenderTargetsAndUnorderedAccessViews];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMGetBlendState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_OMGetDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::OMGetDepthStencilState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_SOGetTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::SOGetTargets];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSGetState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetViewports_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSGetViewports];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_RSGetScissorRects_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::RSGetScissorRects];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_HSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::HSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_DSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::DSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSGetShaderResources];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetUnorderedAccessViews_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSGetUnorderedAccessViews];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSGetShader];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSGetSamplers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_CSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::CSGetConstantBuffers];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_ClearState_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::ClearState];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_Flush_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::Flush];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetType_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GetType];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_GetContextFlags_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::GetContextFlags];
	*(PUINT_PTR)&D3D11_ID3D11DeviceContext_FinishCommandList_Super = (UINT_PTR)pVMTable[VMT_ID3D11DEVICECONTEXT::FinishCommandList];


	return S_OK;
}

/**
* Provides the swapchain pointer.
***/
HRESULT DCL_ID3D11DeviceContext::GetDXGISwapChain(IDXGISwapChain** ppcSwapChain)
{
	if (m_pcSwapChain == NULL)
	{
		*ppcSwapChain = NULL;
		return E_FAIL;
	}

	*ppcSwapChain = m_pcSwapChain;

	return S_OK;
}

#pragma endregion