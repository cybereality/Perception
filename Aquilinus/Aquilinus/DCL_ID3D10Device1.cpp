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
#define OUTPUT_DEBUG_STRING_ID3D10Device1(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_ID3D10Device1(...)
#endif
#define AQU_ID3D10DEVICE1_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_ID3D10Device1[a]) && (m_pcTransferSite->m_ppNOD_ID3D10Device1[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_ID3D10DEVICE1_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_ID3D10Device1[a]->m_paCommanders[b]->m_pOutput = (void*)&c;
#define AQU_ID3D10DEVICE1_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_ID3D10Device1[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_ID3D10DEVICE1_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D10Device1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_ID3D10DEVICE1_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D10Device1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }
#include"DCL_ID3D10Device1.h"
#include"DCL_ID3D10Device1_Super.h"

#pragma region DCL_ID3D10Device1 constructor/destructor
/**
* Constructor
***/
DCL_ID3D10Device1::DCL_ID3D10Device1(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{ }
/**
* Destructor
***/
DCL_ID3D10Device1::~DCL_ID3D10Device1(){ }
#pragma endregion

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface
***/
HRESULT WINAPI DCL_ID3D10Device1::QueryInterface(ID3D10Device1* pcThis, REFIID riid, void** ppvObject)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::QueryInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_IUNKNOWN::QueryInterface,0,riid);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_IUNKNOWN::QueryInterface,1,ppvObject);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_IUNKNOWN::QueryInterface);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::QueryInterface, HRESULT);

	return D3D10_ID3D10Device1_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* ULONG WINAPI AddRef
***/
ULONG WINAPI DCL_ID3D10Device1::AddRef(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::AddRef");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_IUNKNOWN::AddRef);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_IUNKNOWN::AddRef);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::AddRef, ULONG);

	return D3D10_ID3D10Device1_AddRef_Super(pcThis);
}

/**
* ULONG WINAPI Release 
***/
ULONG WINAPI DCL_ID3D10Device1::Release (ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::Release ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_IUNKNOWN::Release );
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_IUNKNOWN::Release );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_IUNKNOWN::Release , ULONG);

	return D3D10_ID3D10Device1_Release_Super(pcThis);
}

#pragma endregion

#pragma region ID3D10Device methods
/**
* void WINAPI VSSetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::VSSetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSSetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSSetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetConstantBuffers);

	D3D10_ID3D10Device1_VSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI PSSetShaderResources
***/
void WINAPI DCL_ID3D10Device1::PSSetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSSetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSSetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetShaderResources);

	D3D10_ID3D10Device1_PSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI PSSetShader 
***/
void WINAPI DCL_ID3D10Device1::PSSetShader (ID3D10Device1* pcThis, ID3D10PixelShader* pPixelShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSSetShader);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetShader,0,pPixelShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSSetShader);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetShader);

	D3D10_ID3D10Device1_PSSetShader_Super(pcThis, pPixelShader);
}

/**
* void WINAPI PSSetSamplers 
***/
void WINAPI DCL_ID3D10Device1::PSSetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSSetSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSSetSamplers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetSamplers);

	D3D10_ID3D10Device1_PSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI VSSetShader 
***/
void WINAPI DCL_ID3D10Device1::VSSetShader (ID3D10Device1* pcThis, ID3D10VertexShader* pVertexShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSSetShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSSetShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetShader ,0,pVertexShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSSetShader );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetShader );

	D3D10_ID3D10Device1_VSSetShader_Super(pcThis, pVertexShader);
}

/**
* void WINAPI DrawIndexed 
***/
void WINAPI DCL_ID3D10Device1::DrawIndexed (ID3D10Device1* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::DrawIndexed ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::DrawIndexed );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed ,0,IndexCount);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed ,1,StartIndexLocation);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed ,2,BaseVertexLocation);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::DrawIndexed );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawIndexed );

	D3D10_ID3D10Device1_DrawIndexed_Super(pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);
}

/**
* void WINAPI Draw
***/
void WINAPI DCL_ID3D10Device1::Draw(ID3D10Device1* pcThis, UINT VertexCount, UINT StartVertexLocation)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::Draw");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::Draw);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::Draw,0,VertexCount);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::Draw,1,StartVertexLocation);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::Draw);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::Draw);

	D3D10_ID3D10Device1_Draw_Super(pcThis, VertexCount, StartVertexLocation);
}

/**
* void WINAPI PSSetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::PSSetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSSetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSSetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetConstantBuffers);

	D3D10_ID3D10Device1_PSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI IASetInputLayout
***/
void WINAPI DCL_ID3D10Device1::IASetInputLayout(ID3D10Device1* pcThis, ID3D10InputLayout* pInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IASetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IASetInputLayout);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetInputLayout,0,pInputLayout);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IASetInputLayout);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetInputLayout);

	D3D10_ID3D10Device1_IASetInputLayout_Super(pcThis, pInputLayout);
}

/**
* void WINAPI IASetVertexBuffers
***/
void WINAPI DCL_ID3D10Device1::IASetVertexBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IASetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IASetVertexBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,2,ppVertexBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,3,pStrides);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,4,pOffsets);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IASetVertexBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetVertexBuffers);

	D3D10_ID3D10Device1_IASetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* void WINAPI IASetIndexBuffer
***/
void WINAPI DCL_ID3D10Device1::IASetIndexBuffer(ID3D10Device1* pcThis, ID3D10Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IASetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IASetIndexBuffer);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,0,pIndexBuffer);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,1,Format);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,2,Offset);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IASetIndexBuffer);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetIndexBuffer);

	D3D10_ID3D10Device1_IASetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
* void WINAPI DrawIndexedInstanced
***/
void WINAPI DCL_ID3D10Device1::DrawIndexedInstanced(ID3D10Device1* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::DrawIndexedInstanced");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::DrawIndexedInstanced);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,0,IndexCountPerInstance);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,1,InstanceCount);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,2,StartIndexLocation);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,3,BaseVertexLocation);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,4,StartInstanceLocation);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::DrawIndexedInstanced);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawIndexedInstanced);

	D3D10_ID3D10Device1_DrawIndexedInstanced_Super(pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

/**
* void WINAPI DrawInstanced 
***/
void WINAPI DCL_ID3D10Device1::DrawInstanced (ID3D10Device1* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::DrawInstanced ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::DrawInstanced );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced ,0,VertexCountPerInstance);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced ,1,InstanceCount);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced ,2,StartVertexLocation);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced ,3,StartInstanceLocation);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::DrawInstanced );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawInstanced );

	D3D10_ID3D10Device1_DrawInstanced_Super(pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/**
* void WINAPI GSSetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::GSSetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSSetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSSetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetConstantBuffers);

	D3D10_ID3D10Device1_GSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI GSSetShader 
***/
void WINAPI DCL_ID3D10Device1::GSSetShader (ID3D10Device1* pcThis, ID3D10GeometryShader* pShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSSetShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSSetShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetShader ,0,pShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSSetShader );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetShader );

	D3D10_ID3D10Device1_GSSetShader_Super(pcThis, pShader);
}

/**
* void WINAPI IASetPrimitiveTopology 
***/
void WINAPI DCL_ID3D10Device1::IASetPrimitiveTopology (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY Topology)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IASetPrimitiveTopology ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IASetPrimitiveTopology );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IASetPrimitiveTopology ,0,Topology);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IASetPrimitiveTopology );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetPrimitiveTopology );

	D3D10_ID3D10Device1_IASetPrimitiveTopology_Super(pcThis, Topology);
}

/**
* void WINAPI VSSetShaderResources
***/
void WINAPI DCL_ID3D10Device1::VSSetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSSetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSSetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetShaderResources);

	D3D10_ID3D10Device1_VSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI VSSetSamplers 
***/
void WINAPI DCL_ID3D10Device1::VSSetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSSetSamplers ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSSetSamplers );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers ,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers ,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers ,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSSetSamplers );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetSamplers );

	D3D10_ID3D10Device1_VSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI SetPredication
***/
void WINAPI DCL_ID3D10Device1::SetPredication(ID3D10Device1* pcThis, ID3D10Predicate* pPredicate, BOOL PredicateValue)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SetPredication);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPredication,0,pPredicate);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPredication,1,PredicateValue);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SetPredication);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::SetPredication);

	D3D10_ID3D10Device1_SetPredication_Super(pcThis, pPredicate, PredicateValue);
}

/**
* void WINAPI GSSetShaderResources
***/
void WINAPI DCL_ID3D10Device1::GSSetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSSetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSSetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetShaderResources);

	D3D10_ID3D10Device1_GSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI GSSetSamplers 
***/
void WINAPI DCL_ID3D10Device1::GSSetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSSetSamplers ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSSetSamplers );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers ,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers ,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers ,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSSetSamplers );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetSamplers );

	D3D10_ID3D10Device1_GSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI OMSetRenderTargets
***/
void WINAPI DCL_ID3D10Device1::OMSetRenderTargets(ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView* pDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMSetRenderTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMSetRenderTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,0,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,1,ppRenderTargetViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,2,pDepthStencilView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMSetRenderTargets);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetRenderTargets);

	D3D10_ID3D10Device1_OMSetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, pDepthStencilView);
}

/**
* void WINAPI OMSetBlendState 
***/
void WINAPI DCL_ID3D10Device1::OMSetBlendState (ID3D10Device1* pcThis, ID3D10BlendState* pBlendState, FLOAT* BlendFactor, UINT SampleMask)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMSetBlendState ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMSetBlendState );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState ,0,pBlendState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState ,1,BlendFactor);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState ,2,SampleMask);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMSetBlendState );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetBlendState );

	D3D10_ID3D10Device1_OMSetBlendState_Super(pcThis, pBlendState, BlendFactor, SampleMask);
}

/**
* void WINAPI OMSetDepthStencilState 
***/
void WINAPI DCL_ID3D10Device1::OMSetDepthStencilState (ID3D10Device1* pcThis, ID3D10DepthStencilState* pDepthStencilState, UINT StencilRef)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMSetDepthStencilState ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMSetDepthStencilState );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetDepthStencilState ,0,pDepthStencilState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMSetDepthStencilState ,1,StencilRef);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMSetDepthStencilState );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetDepthStencilState );

	D3D10_ID3D10Device1_OMSetDepthStencilState_Super(pcThis, pDepthStencilState, StencilRef);
}

/**
* void WINAPI SOSetTargets
***/
void WINAPI DCL_ID3D10Device1::SOSetTargets(ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SOSetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SOSetTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,0,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,1,ppSOTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,2,pOffsets);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SOSetTargets);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::SOSetTargets);

	D3D10_ID3D10Device1_SOSetTargets_Super(pcThis, NumBuffers, ppSOTargets, pOffsets);
}

/**
* void WINAPI DrawAuto
***/
void WINAPI DCL_ID3D10Device1::DrawAuto(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::DrawAuto");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::DrawAuto);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::DrawAuto);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawAuto);

	D3D10_ID3D10Device1_DrawAuto_Super(pcThis);
}

/**
* void WINAPI RSSetState
***/
void WINAPI DCL_ID3D10Device1::RSSetState(ID3D10Device1* pcThis, ID3D10RasterizerState* pRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSSetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSSetState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSSetState,0,pRasterizerState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSSetState);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetState);

	D3D10_ID3D10Device1_RSSetState_Super(pcThis, pRasterizerState);
}

/**
* void WINAPI RSSetViewports
***/
void WINAPI DCL_ID3D10Device1::RSSetViewports(ID3D10Device1* pcThis, UINT NumViewports, D3D10_VIEWPORT* pViewports)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSSetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSSetViewports);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSSetViewports,0,NumViewports);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSSetViewports,1,pViewports);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSSetViewports);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetViewports);

	D3D10_ID3D10Device1_RSSetViewports_Super(pcThis, NumViewports, pViewports);
}

/**
* void WINAPI RSSetScissorRects 
***/
void WINAPI DCL_ID3D10Device1::RSSetScissorRects (ID3D10Device1* pcThis, UINT NumRects, D3D10_RECT* pRects)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSSetScissorRects ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSSetScissorRects );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSSetScissorRects ,0,NumRects);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSSetScissorRects ,1,pRects);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSSetScissorRects );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetScissorRects );

	D3D10_ID3D10Device1_RSSetScissorRects_Super(pcThis, NumRects, pRects);
}

/**
* void WINAPI CopySubresourceRegion
***/
void WINAPI DCL_ID3D10Device1::CopySubresourceRegion(ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource* pSrcResource, UINT SrcSubresource, D3D10_BOX* pSrcBox)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CopySubresourceRegion");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CopySubresourceRegion);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,0,pDstResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,1,DstSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,2,DstX);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,3,DstY);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,4,DstZ);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,5,pSrcResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,6,SrcSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,7,pSrcBox);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CopySubresourceRegion);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::CopySubresourceRegion);

	D3D10_ID3D10Device1_CopySubresourceRegion_Super(pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
}

/**
* void WINAPI CopyResource
***/
void WINAPI DCL_ID3D10Device1::CopyResource(ID3D10Device1* pcThis, ID3D10Resource* pDstResource, ID3D10Resource* pSrcResource)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CopyResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CopyResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopyResource,0,pDstResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CopyResource,1,pSrcResource);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CopyResource);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::CopyResource);

	D3D10_ID3D10Device1_CopyResource_Super(pcThis, pDstResource, pSrcResource);
}

/**
* void WINAPI UpdateSubresource 
***/
void WINAPI DCL_ID3D10Device1::UpdateSubresource (ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, D3D10_BOX* pDstBox, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::UpdateSubresource ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::UpdateSubresource );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,0,pDstResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,1,DstSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,2,pDstBox);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,3,pSrcData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,4,SrcRowPitch);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource ,5,SrcDepthPitch);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::UpdateSubresource );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::UpdateSubresource );

	D3D10_ID3D10Device1_UpdateSubresource_Super(pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

/**
* void WINAPI ClearRenderTargetView
***/
void WINAPI DCL_ID3D10Device1::ClearRenderTargetView(ID3D10Device1* pcThis, ID3D10RenderTargetView* pRenderTargetView, FLOAT* ColorRGBA)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::ClearRenderTargetView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::ClearRenderTargetView);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearRenderTargetView,0,pRenderTargetView);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearRenderTargetView,1,ColorRGBA);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::ClearRenderTargetView);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearRenderTargetView);

	D3D10_ID3D10Device1_ClearRenderTargetView_Super(pcThis, pRenderTargetView, ColorRGBA);
}

/**
* void WINAPI ClearDepthStencilView
***/
void WINAPI DCL_ID3D10Device1::ClearDepthStencilView(ID3D10Device1* pcThis, ID3D10DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::ClearDepthStencilView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::ClearDepthStencilView);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,0,pDepthStencilView);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,1,ClearFlags);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,2,Depth);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,3,Stencil);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::ClearDepthStencilView);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearDepthStencilView);

	D3D10_ID3D10Device1_ClearDepthStencilView_Super(pcThis, pDepthStencilView, ClearFlags, Depth, Stencil);
}

/**
* void WINAPI GenerateMips
***/
void WINAPI DCL_ID3D10Device1::GenerateMips(ID3D10Device1* pcThis, ID3D10ShaderResourceView* pShaderResourceView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GenerateMips");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GenerateMips);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GenerateMips,0,pShaderResourceView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GenerateMips);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GenerateMips);

	D3D10_ID3D10Device1_GenerateMips_Super(pcThis, pShaderResourceView);
}

/**
* void WINAPI ResolveSubresource
***/
void WINAPI DCL_ID3D10Device1::ResolveSubresource(ID3D10Device1* pcThis, ID3D10Resource* pDstResource, UINT DstSubresource, ID3D10Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::ResolveSubresource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::ResolveSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,0,pDstResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,1,DstSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,2,pSrcResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,3,SrcSubresource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,4,Format);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::ResolveSubresource);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::ResolveSubresource);

	D3D10_ID3D10Device1_ResolveSubresource_Super(pcThis, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

/**
* void WINAPI VSGetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::VSGetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSGetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSGetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetConstantBuffers);

	D3D10_ID3D10Device1_VSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI PSGetShaderResources
***/
void WINAPI DCL_ID3D10Device1::PSGetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSGetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSGetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetShaderResources);

	D3D10_ID3D10Device1_PSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI PSGetShader 
***/
void WINAPI DCL_ID3D10Device1::PSGetShader (ID3D10Device1* pcThis, ID3D10PixelShader** ppPixelShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSGetShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSGetShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetShader ,0,ppPixelShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSGetShader );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetShader );

	D3D10_ID3D10Device1_PSGetShader_Super(pcThis, ppPixelShader);
}

/**
* void WINAPI PSGetSamplers 
***/
void WINAPI DCL_ID3D10Device1::PSGetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSGetSamplers ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSGetSamplers );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers ,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers ,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers ,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSGetSamplers );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetSamplers );

	D3D10_ID3D10Device1_PSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI VSGetShader 
***/
void WINAPI DCL_ID3D10Device1::VSGetShader (ID3D10Device1* pcThis, ID3D10VertexShader** ppVertexShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSGetShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSGetShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetShader ,0,ppVertexShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSGetShader );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetShader );

	D3D10_ID3D10Device1_VSGetShader_Super(pcThis, ppVertexShader);
}

/**
* void WINAPI PSGetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::PSGetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::PSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::PSGetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::PSGetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetConstantBuffers);

	D3D10_ID3D10Device1_PSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI IAGetInputLayout
***/
void WINAPI DCL_ID3D10Device1::IAGetInputLayout(ID3D10Device1* pcThis, ID3D10InputLayout** ppInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IAGetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IAGetInputLayout);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetInputLayout,0,ppInputLayout);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IAGetInputLayout);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetInputLayout);

	D3D10_ID3D10Device1_IAGetInputLayout_Super(pcThis, ppInputLayout);
}

/**
* void WINAPI IAGetVertexBuffers
***/
void WINAPI DCL_ID3D10Device1::IAGetVertexBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IAGetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IAGetVertexBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,2,ppVertexBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,3,pStrides);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,4,pOffsets);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IAGetVertexBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetVertexBuffers);

	D3D10_ID3D10Device1_IAGetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* void WINAPI IAGetIndexBuffer
***/
void WINAPI DCL_ID3D10Device1::IAGetIndexBuffer(ID3D10Device1* pcThis, ID3D10Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IAGetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IAGetIndexBuffer);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,0,pIndexBuffer);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,1,Format);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,2,Offset);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IAGetIndexBuffer);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetIndexBuffer);

	D3D10_ID3D10Device1_IAGetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
* void WINAPI GSGetConstantBuffers
***/
void WINAPI DCL_ID3D10Device1::GSGetConstantBuffers(ID3D10Device1* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer** ppConstantBuffers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSGetConstantBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSGetConstantBuffers);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetConstantBuffers);

	D3D10_ID3D10Device1_GSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* void WINAPI GSGetShader 
***/
void WINAPI DCL_ID3D10Device1::GSGetShader (ID3D10Device1* pcThis, ID3D10GeometryShader** ppGeometryShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSGetShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSGetShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetShader ,0,ppGeometryShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSGetShader );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetShader );

	D3D10_ID3D10Device1_GSGetShader_Super(pcThis, ppGeometryShader);
}

/**
* void WINAPI IAGetPrimitiveTopology 
***/
void WINAPI DCL_ID3D10Device1::IAGetPrimitiveTopology (ID3D10Device1* pcThis, D3D10_PRIMITIVE_TOPOLOGY* pTopology)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::IAGetPrimitiveTopology ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::IAGetPrimitiveTopology );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::IAGetPrimitiveTopology ,0,pTopology);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::IAGetPrimitiveTopology );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetPrimitiveTopology );

	D3D10_ID3D10Device1_IAGetPrimitiveTopology_Super(pcThis, pTopology);
}

/**
* void WINAPI VSGetShaderResources
***/
void WINAPI DCL_ID3D10Device1::VSGetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSGetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSGetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetShaderResources);

	D3D10_ID3D10Device1_VSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI VSGetSamplers 
***/
void WINAPI DCL_ID3D10Device1::VSGetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::VSGetSamplers ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::VSGetSamplers );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers ,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers ,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers ,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::VSGetSamplers );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetSamplers );

	D3D10_ID3D10Device1_VSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI GetPredication
***/
void WINAPI DCL_ID3D10Device1::GetPredication(ID3D10Device1* pcThis, ID3D10Predicate** ppPredicate, BOOL* pPredicateValue)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetPredication);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetPredication,0,ppPredicate);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetPredication,1,pPredicateValue);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetPredication);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GetPredication);

	D3D10_ID3D10Device1_GetPredication_Super(pcThis, ppPredicate, pPredicateValue);
}

/**
* void WINAPI GSGetShaderResources
***/
void WINAPI DCL_ID3D10Device1::GSGetShaderResources(ID3D10Device1* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView** ppShaderResourceViews)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSGetShaderResources);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSGetShaderResources);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetShaderResources);

	D3D10_ID3D10Device1_GSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
* void WINAPI GSGetSamplers 
***/
void WINAPI DCL_ID3D10Device1::GSGetSamplers (ID3D10Device1* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState** ppSamplers)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GSGetSamplers ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GSGetSamplers );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers ,0,StartSlot);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers ,1,NumSamplers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers ,2,ppSamplers);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GSGetSamplers );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetSamplers );

	D3D10_ID3D10Device1_GSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
* void WINAPI OMGetRenderTargets
***/
void WINAPI DCL_ID3D10Device1::OMGetRenderTargets(ID3D10Device1* pcThis, UINT NumViews, ID3D10RenderTargetView** ppRenderTargetViews, ID3D10DepthStencilView** ppDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMGetRenderTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMGetRenderTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,0,NumViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,1,ppRenderTargetViews);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,2,ppDepthStencilView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMGetRenderTargets);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetRenderTargets);

	D3D10_ID3D10Device1_OMGetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, ppDepthStencilView);
}

/**
* void WINAPI OMGetBlendState 
***/
void WINAPI DCL_ID3D10Device1::OMGetBlendState (ID3D10Device1* pcThis, ID3D10BlendState** ppBlendState, FLOAT* BlendFactor, UINT* pSampleMask)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMGetBlendState ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMGetBlendState );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState ,0,ppBlendState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState ,1,BlendFactor);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState ,2,pSampleMask);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMGetBlendState );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetBlendState );

	D3D10_ID3D10Device1_OMGetBlendState_Super(pcThis, ppBlendState, BlendFactor, pSampleMask);
}

/**
* void WINAPI OMGetDepthStencilState 
***/
void WINAPI DCL_ID3D10Device1::OMGetDepthStencilState (ID3D10Device1* pcThis, ID3D10DepthStencilState** ppDepthStencilState, UINT* pStencilRef)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OMGetDepthStencilState ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OMGetDepthStencilState );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetDepthStencilState ,0,ppDepthStencilState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OMGetDepthStencilState ,1,pStencilRef);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OMGetDepthStencilState );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetDepthStencilState );

	D3D10_ID3D10Device1_OMGetDepthStencilState_Super(pcThis, ppDepthStencilState, pStencilRef);
}

/**
* void WINAPI SOGetTargets
***/
void WINAPI DCL_ID3D10Device1::SOGetTargets(ID3D10Device1* pcThis, UINT NumBuffers, ID3D10Buffer** ppSOTargets, UINT* pOffsets)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SOGetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SOGetTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,0,NumBuffers);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,1,ppSOTargets);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,2,pOffsets);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SOGetTargets);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::SOGetTargets);

	D3D10_ID3D10Device1_SOGetTargets_Super(pcThis, NumBuffers, ppSOTargets, pOffsets);
}

/**
* void WINAPI RSGetState
***/
void WINAPI DCL_ID3D10Device1::RSGetState(ID3D10Device1* pcThis, ID3D10RasterizerState** ppRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSGetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSGetState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSGetState,0,ppRasterizerState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSGetState);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetState);

	D3D10_ID3D10Device1_RSGetState_Super(pcThis, ppRasterizerState);
}

/**
* void WINAPI RSGetViewports
***/
void WINAPI DCL_ID3D10Device1::RSGetViewports(ID3D10Device1* pcThis, UINT* NumViewports, D3D10_VIEWPORT* pViewports)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSGetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSGetViewports);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSGetViewports,0,NumViewports);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSGetViewports,1,pViewports);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSGetViewports);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetViewports);

	D3D10_ID3D10Device1_RSGetViewports_Super(pcThis, NumViewports, pViewports);
}

/**
* void WINAPI RSGetScissorRects 
***/
void WINAPI DCL_ID3D10Device1::RSGetScissorRects (ID3D10Device1* pcThis, UINT* NumRects, D3D10_RECT* pRects)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::RSGetScissorRects ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::RSGetScissorRects );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSGetScissorRects ,0,NumRects);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::RSGetScissorRects ,1,pRects);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::RSGetScissorRects );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetScissorRects );

	D3D10_ID3D10Device1_RSGetScissorRects_Super(pcThis, NumRects, pRects);
}

/**
* HRESULT WINAPI GetDeviceRemovedReason 
***/
HRESULT WINAPI DCL_ID3D10Device1::GetDeviceRemovedReason (ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetDeviceRemovedReason ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetDeviceRemovedReason );
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetDeviceRemovedReason );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::GetDeviceRemovedReason , HRESULT);

	return D3D10_ID3D10Device1_GetDeviceRemovedReason_Super(pcThis);
}

/**
* HRESULT WINAPI SetExceptionMode
***/
HRESULT WINAPI DCL_ID3D10Device1::SetExceptionMode(ID3D10Device1* pcThis, UINT RaiseFlags)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SetExceptionMode);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetExceptionMode,0,RaiseFlags);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SetExceptionMode);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::SetExceptionMode, HRESULT);

	return D3D10_ID3D10Device1_SetExceptionMode_Super(pcThis, RaiseFlags);
}

/**
* UINT WINAPI GetExceptionMode
***/
UINT WINAPI DCL_ID3D10Device1::GetExceptionMode(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetExceptionMode);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetExceptionMode);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::GetExceptionMode, UINT);

	return D3D10_ID3D10Device1_GetExceptionMode_Super(pcThis);
}

/**
* HRESULT WINAPI GetPrivateData
***/
HRESULT WINAPI DCL_ID3D10Device1::GetPrivateData(ID3D10Device1* pcThis, REFGUID guid, UINT* pDataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetPrivateData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,0,guid);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,1,pDataSize);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,2,pData);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetPrivateData);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::GetPrivateData, HRESULT);

	return D3D10_ID3D10Device1_GetPrivateData_Super(pcThis, guid, pDataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateData
***/
HRESULT WINAPI DCL_ID3D10Device1::SetPrivateData(ID3D10Device1* pcThis, REFGUID guid, UINT DataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SetPrivateData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,0,guid);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,1,DataSize);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,2,pData);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SetPrivateData);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::SetPrivateData, HRESULT);

	return D3D10_ID3D10Device1_SetPrivateData_Super(pcThis, guid, DataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateDataInterface
***/
HRESULT WINAPI DCL_ID3D10Device1::SetPrivateDataInterface(ID3D10Device1* pcThis, REFGUID guid, IUnknown* pData)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SetPrivateDataInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SetPrivateDataInterface);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateDataInterface,0,guid);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateDataInterface,1,pData);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SetPrivateDataInterface);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::SetPrivateDataInterface, HRESULT);

	return D3D10_ID3D10Device1_SetPrivateDataInterface_Super(pcThis, guid, pData);
}

/**
* void WINAPI ClearState
***/
void WINAPI DCL_ID3D10Device1::ClearState(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::ClearState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::ClearState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::ClearState);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearState);

	D3D10_ID3D10Device1_ClearState_Super(pcThis);
}

/**
* void WINAPI Flush 
***/
void WINAPI DCL_ID3D10Device1::Flush (ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::Flush ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::Flush );
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::Flush );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::Flush );

	D3D10_ID3D10Device1_Flush_Super(pcThis);
}

/**
* HRESULT WINAPI CreateBuffer
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateBuffer(ID3D10Device1* pcThis, D3D10_BUFFER_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Buffer** ppBuffer)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateBuffer);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,0,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,1,pInitialData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,2,ppBuffer);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateBuffer);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateBuffer, HRESULT);

	return D3D10_ID3D10Device1_CreateBuffer_Super(pcThis, pDesc, pInitialData, ppBuffer);
}

/**
* HRESULT WINAPI CreateTexture1D 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateTexture1D (ID3D10Device1* pcThis, D3D10_TEXTURE1D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture1D** ppTexture1D)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateTexture1D ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateTexture1D );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D ,0,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D ,1,pInitialData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D ,2,ppTexture1D);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture1D );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture1D , HRESULT);

	return D3D10_ID3D10Device1_CreateTexture1D_Super(pcThis, pDesc, pInitialData, ppTexture1D);
}

/**
* HRESULT WINAPI CreateTexture2D 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateTexture2D (ID3D10Device1* pcThis, D3D10_TEXTURE2D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture2D** ppTexture2D)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateTexture2D ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateTexture2D );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D ,0,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D ,1,pInitialData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D ,2,ppTexture2D);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture2D );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture2D , HRESULT);

	return D3D10_ID3D10Device1_CreateTexture2D_Super(pcThis, pDesc, pInitialData, ppTexture2D);
}

/**
* HRESULT WINAPI CreateTexture3D 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateTexture3D (ID3D10Device1* pcThis, D3D10_TEXTURE3D_DESC* pDesc, D3D10_SUBRESOURCE_DATA* pInitialData, ID3D10Texture3D** ppTexture3D)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateTexture3D ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateTexture3D );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D ,0,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D ,1,pInitialData);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D ,2,ppTexture3D);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture3D );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture3D , HRESULT);

	return D3D10_ID3D10Device1_CreateTexture3D_Super(pcThis, pDesc, pInitialData, ppTexture3D);
}

/**
* HRESULT WINAPI CreateShaderResourceView 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateShaderResourceView (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D10ShaderResourceView** ppSRView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateShaderResourceView ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateShaderResourceView );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView ,0,pResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView ,1,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView ,2,ppSRView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateShaderResourceView );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateShaderResourceView , HRESULT);

	return D3D10_ID3D10Device1_CreateShaderResourceView_Super(pcThis, pResource, pDesc, ppSRView);
}

/**
* HRESULT WINAPI CreateRenderTargetView 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateRenderTargetView (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_RENDER_TARGET_VIEW_DESC* pDesc, ID3D10RenderTargetView** ppRTView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateRenderTargetView ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateRenderTargetView );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView ,0,pResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView ,1,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView ,2,ppRTView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateRenderTargetView );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateRenderTargetView , HRESULT);

	return D3D10_ID3D10Device1_CreateRenderTargetView_Super(pcThis, pResource, pDesc, ppRTView);
}

/**
* HRESULT WINAPI CreateDepthStencilView 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateDepthStencilView (ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D10DepthStencilView** ppDepthStencilView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateDepthStencilView ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateDepthStencilView );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView ,0,pResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView ,1,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView ,2,ppDepthStencilView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateDepthStencilView );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateDepthStencilView , HRESULT);

	return D3D10_ID3D10Device1_CreateDepthStencilView_Super(pcThis, pResource, pDesc, ppDepthStencilView);
}

/**
* HRESULT WINAPI CreateInputLayout 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateInputLayout (ID3D10Device1* pcThis, D3D10_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout** ppInputLayout)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateInputLayout ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateInputLayout );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout ,0,pInputElementDescs);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout ,1,NumElements);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout ,2,pShaderBytecodeWithInputSignature);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout ,3,BytecodeLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout ,4,ppInputLayout);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateInputLayout );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateInputLayout , HRESULT);

	return D3D10_ID3D10Device1_CreateInputLayout_Super(pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

/**
* HRESULT WINAPI CreateVertexShader
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateVertexShader(ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader** ppVertexShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateVertexShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateVertexShader);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,0,pShaderBytecode);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,1,BytecodeLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,2,ppVertexShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateVertexShader);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateVertexShader, HRESULT);

	return D3D10_ID3D10Device1_CreateVertexShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppVertexShader);
}

/**
* HRESULT WINAPI CreateGeometryShader
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateGeometryShader(ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader** ppGeometryShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateGeometryShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateGeometryShader);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,0,pShaderBytecode);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,1,BytecodeLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,2,ppGeometryShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateGeometryShader);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateGeometryShader, HRESULT);

	return D3D10_ID3D10Device1_CreateGeometryShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppGeometryShader);
}

/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateGeometryShaderWithStreamOutput (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, D3D10_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader** ppGeometryShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateGeometryShaderWithStreamOutput ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,0,pShaderBytecode);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,1,BytecodeLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,2,pSODeclaration);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,3,NumEntries);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,4,OutputStreamStride);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ,5,ppGeometryShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput , HRESULT);

	return D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput_Super(pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader);
}

/**
* HRESULT WINAPI CreatePixelShader 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreatePixelShader (ID3D10Device1* pcThis, void* pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader** ppPixelShader)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreatePixelShader ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreatePixelShader );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader ,0,pShaderBytecode);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader ,1,BytecodeLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader ,2,ppPixelShader);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreatePixelShader );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreatePixelShader , HRESULT);

	return D3D10_ID3D10Device1_CreatePixelShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppPixelShader);
}

/**
* HRESULT WINAPI CreateBlendState
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateBlendState(ID3D10Device1* pcThis, D3D10_BLEND_DESC* pBlendStateDesc, ID3D10BlendState** ppBlendState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateBlendState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateBlendState,0,pBlendStateDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateBlendState,1,ppBlendState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateBlendState);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateBlendState, HRESULT);

	return D3D10_ID3D10Device1_CreateBlendState_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
* HRESULT WINAPI CreateDepthStencilState
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateDepthStencilState(ID3D10Device1* pcThis, D3D10_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D10DepthStencilState** ppDepthStencilState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateDepthStencilState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilState,0,pDepthStencilDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilState,1,ppDepthStencilState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateDepthStencilState);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateDepthStencilState, HRESULT);

	return D3D10_ID3D10Device1_CreateDepthStencilState_Super(pcThis, pDepthStencilDesc, ppDepthStencilState);
}

/**
* HRESULT WINAPI CreateRasterizerState
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateRasterizerState(ID3D10Device1* pcThis, D3D10_RASTERIZER_DESC* pRasterizerDesc, ID3D10RasterizerState** ppRasterizerState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateRasterizerState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateRasterizerState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateRasterizerState,0,pRasterizerDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateRasterizerState,1,ppRasterizerState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateRasterizerState);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateRasterizerState, HRESULT);

	return D3D10_ID3D10Device1_CreateRasterizerState_Super(pcThis, pRasterizerDesc, ppRasterizerState);
}

/**
* HRESULT WINAPI CreateSamplerState
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateSamplerState(ID3D10Device1* pcThis, D3D10_SAMPLER_DESC* pSamplerDesc, ID3D10SamplerState** ppSamplerState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateSamplerState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateSamplerState);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateSamplerState,0,pSamplerDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateSamplerState,1,ppSamplerState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateSamplerState);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateSamplerState, HRESULT);

	return D3D10_ID3D10Device1_CreateSamplerState_Super(pcThis, pSamplerDesc, ppSamplerState);
}

/**
* HRESULT WINAPI CreateQuery 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateQuery (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pQueryDesc, ID3D10Query** ppQuery)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateQuery ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateQuery );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateQuery ,0,pQueryDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateQuery ,1,ppQuery);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateQuery );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateQuery , HRESULT);

	return D3D10_ID3D10Device1_CreateQuery_Super(pcThis, pQueryDesc, ppQuery);
}

/**
* HRESULT WINAPI CreatePredicate 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreatePredicate (ID3D10Device1* pcThis, D3D10_QUERY_DESC* pPredicateDesc, ID3D10Predicate** ppPredicate)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreatePredicate ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreatePredicate );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreatePredicate ,0,pPredicateDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreatePredicate ,1,ppPredicate);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreatePredicate );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreatePredicate , HRESULT);

	return D3D10_ID3D10Device1_CreatePredicate_Super(pcThis, pPredicateDesc, ppPredicate);
}

/**
* HRESULT WINAPI CreateCounter 
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateCounter (ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pCounterDesc, ID3D10Counter** ppCounter)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateCounter ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CreateCounter );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateCounter ,0,pCounterDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CreateCounter ,1,ppCounter);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CreateCounter );
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateCounter , HRESULT);

	return D3D10_ID3D10Device1_CreateCounter_Super(pcThis, pCounterDesc, ppCounter);
}

/**
* HRESULT WINAPI CheckFormatSupport
***/
HRESULT WINAPI DCL_ID3D10Device1::CheckFormatSupport(ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT* pFormatSupport)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CheckFormatSupport");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CheckFormatSupport);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckFormatSupport,0,Format);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckFormatSupport,1,pFormatSupport);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CheckFormatSupport);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckFormatSupport, HRESULT);

	return D3D10_ID3D10Device1_CheckFormatSupport_Super(pcThis, Format, pFormatSupport);
}

/**
* HRESULT WINAPI CheckMultisampleQualityLevels
***/
HRESULT WINAPI DCL_ID3D10Device1::CheckMultisampleQualityLevels(ID3D10Device1* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CheckMultisampleQualityLevels");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,0,Format);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,1,SampleCount);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,2,pNumQualityLevels);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels, HRESULT);

	return D3D10_ID3D10Device1_CheckMultisampleQualityLevels_Super(pcThis, Format, SampleCount, pNumQualityLevels);
}

/**
* void WINAPI CheckCounterInfo
***/
void WINAPI DCL_ID3D10Device1::CheckCounterInfo(ID3D10Device1* pcThis, D3D10_COUNTER_INFO* pCounterInfo)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CheckCounterInfo");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CheckCounterInfo);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounterInfo,0,pCounterInfo);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CheckCounterInfo);
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::CheckCounterInfo);

	D3D10_ID3D10Device1_CheckCounterInfo_Super(pcThis, pCounterInfo);
}

/**
* HRESULT WINAPI CheckCounter
***/
HRESULT WINAPI DCL_ID3D10Device1::CheckCounter(ID3D10Device1* pcThis, D3D10_COUNTER_DESC* pDesc, D3D10_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CheckCounter");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::CheckCounter);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,0,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,1,pType);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,2,pActiveCounters);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,3,szName);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,4,pNameLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,5,szUnits);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,6,pUnitsLength);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,7,szDescription);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,8,pDescriptionLength);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::CheckCounter);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckCounter, HRESULT);

	return D3D10_ID3D10Device1_CheckCounter_Super(pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

/**
* UINT WINAPI GetCreationFlags
***/
UINT WINAPI DCL_ID3D10Device1::GetCreationFlags(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetCreationFlags");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetCreationFlags);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetCreationFlags);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::GetCreationFlags, UINT);

	return D3D10_ID3D10Device1_GetCreationFlags_Super(pcThis);
}

/**
* HRESULT WINAPI OpenSharedResource
***/
HRESULT WINAPI DCL_ID3D10Device1::OpenSharedResource(ID3D10Device1* pcThis, HANDLE hResource, REFIID ReturnedInterface, void** ppResource)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::OpenSharedResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::OpenSharedResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,0,hResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,1,ReturnedInterface);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,2,ppResource);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::OpenSharedResource);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE::OpenSharedResource, HRESULT);

	return D3D10_ID3D10Device1_OpenSharedResource_Super(pcThis, hResource, ReturnedInterface, ppResource);
}

/**
* void WINAPI SetTextFilterSize 
***/
void WINAPI DCL_ID3D10Device1::SetTextFilterSize (ID3D10Device1* pcThis, UINT Width, UINT Height)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::SetTextFilterSize ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::SetTextFilterSize );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetTextFilterSize ,0,Width);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::SetTextFilterSize ,1,Height);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::SetTextFilterSize );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::SetTextFilterSize );

	D3D10_ID3D10Device1_SetTextFilterSize_Super(pcThis, Width, Height);
}

/**
* void WINAPI GetTextFilterSize 
***/
void WINAPI DCL_ID3D10Device1::GetTextFilterSize (ID3D10Device1* pcThis, UINT* pWidth, UINT* pHeight)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetTextFilterSize ");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE::GetTextFilterSize );
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetTextFilterSize ,0,pWidth);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE::GetTextFilterSize ,1,pHeight);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE::GetTextFilterSize );
	AQU_ID3D10DEVICE1_REPLACE_VOID__(VMT_ID3D10DEVICE::GetTextFilterSize );

	D3D10_ID3D10Device1_GetTextFilterSize_Super(pcThis, pWidth, pHeight);
}

#pragma endregion

#pragma region ID3D10Device1 methods
/**
* HRESULT WINAPI CreateShaderResourceView1
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateShaderResourceView1(ID3D10Device1* pcThis, ID3D10Resource* pResource, D3D10_SHADER_RESOURCE_VIEW_DESC1* pDesc, ID3D10ShaderResourceView1** ppSRView)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateShaderResourceView1");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE1::CreateShaderResourceView1);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE1::CreateShaderResourceView1,0,pResource);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE1::CreateShaderResourceView1,1,pDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE1::CreateShaderResourceView1,2,ppSRView);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE1::CreateShaderResourceView1);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE1::CreateShaderResourceView1, HRESULT);

	return D3D10_ID3D10Device1_CreateShaderResourceView1_Super(pcThis, pResource, pDesc, ppSRView);
}

/**
* HRESULT WINAPI CreateBlendState1
***/
HRESULT WINAPI DCL_ID3D10Device1::CreateBlendState1(ID3D10Device1* pcThis, D3D10_BLEND_DESC1* pBlendStateDesc, ID3D10BlendState1** ppBlendState)
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::CreateBlendState1");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE1::CreateBlendState1);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE1::CreateBlendState1,0,pBlendStateDesc);
	AQU_ID3D10DEVICE1_SET_DATA______(VMT_ID3D10DEVICE1::CreateBlendState1,1,ppBlendState);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE1::CreateBlendState1);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE1::CreateBlendState1, HRESULT);

	return D3D10_ID3D10Device1_CreateBlendState1_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
* D3D10_FEATURE_LEVEL1 WINAPI GetFeatureLevel
***/
D3D10_FEATURE_LEVEL1 WINAPI DCL_ID3D10Device1::GetFeatureLevel(ID3D10Device1* pcThis )
{
	OUTPUT_DEBUG_STRING_ID3D10Device1(L"ID3D10Device1::GetFeatureLevel");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE1_HEADER________(VMT_ID3D10DEVICE1::GetFeatureLevel);
	AQU_ID3D10DEVICE1_PROVOKE_______(VMT_ID3D10DEVICE1::GetFeatureLevel);
	AQU_ID3D10DEVICE1_REPLACE_METHOD(VMT_ID3D10DEVICE1::GetFeatureLevel, D3D10_FEATURE_LEVEL1);

	return D3D10_ID3D10Device1_GetFeatureLevel_Super(pcThis);
}

#pragma endregion

#pragma region DCL_ID3D10Device1 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D10Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D10Device1::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface:*(PUINT_PTR)&D3D10_ID3D10Device1_QueryInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:*(PUINT_PTR)&D3D10_ID3D10Device1_AddRef_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release :*(PUINT_PTR)&D3D10_ID3D10Device1_Release_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D10Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D10Device1::SetSuperFunctionPointer(VMT_ID3D10DEVICE::VMT_ID3D10Device eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** ID3D10Device super methods ***/
	case VMT_ID3D10DEVICE::VSSetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawIndexed :*(PUINT_PTR)&D3D10_ID3D10Device1_DrawIndexed_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::Draw:*(PUINT_PTR)&D3D10_ID3D10Device1_Draw_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetInputLayout:*(PUINT_PTR)&D3D10_ID3D10Device1_IASetInputLayout_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetVertexBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_IASetVertexBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetIndexBuffer:*(PUINT_PTR)&D3D10_ID3D10Device1_IASetIndexBuffer_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawIndexedInstanced:*(PUINT_PTR)&D3D10_ID3D10Device1_DrawIndexedInstanced_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawInstanced :*(PUINT_PTR)&D3D10_ID3D10Device1_DrawInstanced_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetPrimitiveTopology :*(PUINT_PTR)&D3D10_ID3D10Device1_IASetPrimitiveTopology_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPredication:*(PUINT_PTR)&D3D10_ID3D10Device1_SetPredication_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetRenderTargets:*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetRenderTargets_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetBlendState :*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetBlendState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetDepthStencilState :*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetDepthStencilState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SOSetTargets:*(PUINT_PTR)&D3D10_ID3D10Device1_SOSetTargets_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawAuto:*(PUINT_PTR)&D3D10_ID3D10Device1_DrawAuto_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetState:*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetViewports:*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetViewports_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetScissorRects :*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetScissorRects_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CopySubresourceRegion:*(PUINT_PTR)&D3D10_ID3D10Device1_CopySubresourceRegion_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CopyResource:*(PUINT_PTR)&D3D10_ID3D10Device1_CopyResource_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::UpdateSubresource :*(PUINT_PTR)&D3D10_ID3D10Device1_UpdateSubresource_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearRenderTargetView:*(PUINT_PTR)&D3D10_ID3D10Device1_ClearRenderTargetView_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearDepthStencilView:*(PUINT_PTR)&D3D10_ID3D10Device1_ClearDepthStencilView_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GenerateMips:*(PUINT_PTR)&D3D10_ID3D10Device1_GenerateMips_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ResolveSubresource:*(PUINT_PTR)&D3D10_ID3D10Device1_ResolveSubresource_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetInputLayout:*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetInputLayout_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetVertexBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetVertexBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetIndexBuffer:*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetIndexBuffer_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetConstantBuffers:*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetConstantBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetShader :*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetPrimitiveTopology :*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetPrimitiveTopology_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetPredication:*(PUINT_PTR)&D3D10_ID3D10Device1_GetPredication_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetShaderResources:*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetShaderResources_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetSamplers :*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetSamplers_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetRenderTargets:*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetRenderTargets_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetBlendState :*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetBlendState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetDepthStencilState :*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetDepthStencilState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SOGetTargets:*(PUINT_PTR)&D3D10_ID3D10Device1_SOGetTargets_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetState:*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetViewports:*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetViewports_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetScissorRects :*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetScissorRects_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetDeviceRemovedReason :*(PUINT_PTR)&D3D10_ID3D10Device1_GetDeviceRemovedReason_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetExceptionMode:*(PUINT_PTR)&D3D10_ID3D10Device1_SetExceptionMode_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetExceptionMode:*(PUINT_PTR)&D3D10_ID3D10Device1_GetExceptionMode_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetPrivateData:*(PUINT_PTR)&D3D10_ID3D10Device1_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPrivateData:*(PUINT_PTR)&D3D10_ID3D10Device1_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPrivateDataInterface:*(PUINT_PTR)&D3D10_ID3D10Device1_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearState:*(PUINT_PTR)&D3D10_ID3D10Device1_ClearState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::Flush :*(PUINT_PTR)&D3D10_ID3D10Device1_Flush_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateBuffer:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBuffer_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture1D :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture1D_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture2D :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture2D_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture3D :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture3D_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateShaderResourceView :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateShaderResourceView_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateRenderTargetView :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateRenderTargetView_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateDepthStencilView :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateDepthStencilView_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateInputLayout :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateInputLayout_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateVertexShader:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateVertexShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateGeometryShader:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateGeometryShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreatePixelShader :*(PUINT_PTR)&D3D10_ID3D10Device1_CreatePixelShader_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateBlendState:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBlendState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateDepthStencilState:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateDepthStencilState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateRasterizerState:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateRasterizerState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateSamplerState:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateSamplerState_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateQuery :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateQuery_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreatePredicate :*(PUINT_PTR)&D3D10_ID3D10Device1_CreatePredicate_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateCounter :*(PUINT_PTR)&D3D10_ID3D10Device1_CreateCounter_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckFormatSupport:*(PUINT_PTR)&D3D10_ID3D10Device1_CheckFormatSupport_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckMultisampleQualityLevels:*(PUINT_PTR)&D3D10_ID3D10Device1_CheckMultisampleQualityLevels_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckCounterInfo:*(PUINT_PTR)&D3D10_ID3D10Device1_CheckCounterInfo_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckCounter:*(PUINT_PTR)&D3D10_ID3D10Device1_CheckCounter_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetCreationFlags:*(PUINT_PTR)&D3D10_ID3D10Device1_GetCreationFlags_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OpenSharedResource:*(PUINT_PTR)&D3D10_ID3D10Device1_OpenSharedResource_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetTextFilterSize :*(PUINT_PTR)&D3D10_ID3D10Device1_SetTextFilterSize_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetTextFilterSize :*(PUINT_PTR)&D3D10_ID3D10Device1_GetTextFilterSize_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D10Device1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D10Device1::SetSuperFunctionPointer(VMT_ID3D10DEVICE1::VMT_ID3D10Device1 eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** ID3D10Device1 super methods ***/
	case VMT_ID3D10DEVICE1::CreateShaderResourceView1:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateShaderResourceView1_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE1::CreateBlendState1:*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBlendState1_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE1::GetFeatureLevel:*(PUINT_PTR)&D3D10_ID3D10Device1_GetFeatureLevel_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_ID3D10Device1::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D10_ID3D10Device1_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D10_ID3D10Device1_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D10_ID3D10Device1_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release ];
	/*** ID3D10Device super methods ***/
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_DrawIndexed_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawIndexed ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_Draw_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::Draw];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IASetInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetInputLayout];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IASetVertexBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetVertexBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IASetIndexBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetIndexBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device1_DrawIndexedInstanced_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced];
	*(PUINT_PTR)&D3D10_ID3D10Device1_DrawInstanced_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawInstanced ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IASetPrimitiveTopology_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SetPredication_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPredication];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSSetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetRenderTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetRenderTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetBlendState ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMSetDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SOSetTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SOSetTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device1_DrawAuto_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawAuto];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetViewports_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetViewports];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSSetScissorRects_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetScissorRects ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CopySubresourceRegion_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CopySubresourceRegion];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CopyResource_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CopyResource];
	*(PUINT_PTR)&D3D10_ID3D10Device1_UpdateSubresource_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::UpdateSubresource ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_ClearRenderTargetView_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearRenderTargetView];
	*(PUINT_PTR)&D3D10_ID3D10Device1_ClearDepthStencilView_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearDepthStencilView];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GenerateMips_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GenerateMips];
	*(PUINT_PTR)&D3D10_ID3D10Device1_ResolveSubresource_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ResolveSubresource];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_PSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetInputLayout];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetVertexBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetIndexBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetConstantBuffers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_IAGetPrimitiveTopology_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_VSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetPredication_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetPredication];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetShaderResources_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GSGetSamplers_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetSamplers ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetRenderTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetRenderTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetBlendState ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OMGetDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SOGetTargets_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SOGetTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetViewports_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetViewports];
	*(PUINT_PTR)&D3D10_ID3D10Device1_RSGetScissorRects_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetScissorRects ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetDeviceRemovedReason_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetExceptionMode];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetExceptionMode_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetExceptionMode];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetPrivateData];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SetPrivateData_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPrivateData];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SetPrivateDataInterface_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface];
	*(PUINT_PTR)&D3D10_ID3D10Device1_ClearState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_Flush_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::Flush ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBuffer_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture1D_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture1D ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture2D_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture2D ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateTexture3D_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture3D ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateShaderResourceView_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateShaderResourceView ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateRenderTargetView_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateRenderTargetView ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateDepthStencilView_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateDepthStencilView ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateInputLayout_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateInputLayout ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateVertexShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateVertexShader];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateGeometryShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateGeometryShader];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreatePixelShader_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreatePixelShader ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBlendState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateBlendState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateDepthStencilState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateDepthStencilState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateRasterizerState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateRasterizerState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateSamplerState_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateSamplerState];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateQuery_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateQuery ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreatePredicate_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreatePredicate ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateCounter ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CheckFormatSupport_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckFormatSupport];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CheckMultisampleQualityLevels_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CheckCounterInfo_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckCounterInfo];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CheckCounter_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckCounter];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetCreationFlags_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetCreationFlags];
	*(PUINT_PTR)&D3D10_ID3D10Device1_OpenSharedResource_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OpenSharedResource];
	*(PUINT_PTR)&D3D10_ID3D10Device1_SetTextFilterSize_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetTextFilterSize ];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetTextFilterSize_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetTextFilterSize ];
	/*** ID3D10Device1 super methods ***/
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateShaderResourceView1_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE1::CreateShaderResourceView1];
	*(PUINT_PTR)&D3D10_ID3D10Device1_CreateBlendState1_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE1::CreateBlendState1];
	*(PUINT_PTR)&D3D10_ID3D10Device1_GetFeatureLevel_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE1::GetFeatureLevel];


	return S_OK;
}

#pragma endregion