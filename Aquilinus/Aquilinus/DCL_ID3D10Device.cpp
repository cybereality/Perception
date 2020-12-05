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
#define OUTPUT_DEBUG_STRING_ID3D10Device(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_ID3D10Device(...)
#endif
#define AQU_ID3D10DEVICE_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_ID3D10Device[a]) && (m_pcTransferSite->m_ppNOD_ID3D10Device[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_ID3D10DEVICE_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_ID3D10Device[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_ID3D10DEVICE_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_ID3D10Device[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_ID3D10DEVICE_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D10Device[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_ID3D10DEVICE_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_ID3D10Device[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }

#include"DCL_ID3D10Device.h"
#include"DCL_ID3D10Device_Super.h"

#pragma region DCL_ID3D10Device constructor/destructor
/**
*
***/
DCL_ID3D10Device::DCL_ID3D10Device(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{ m_pcSwapChain = nullptr; }

/**
*
***/
DCL_ID3D10Device::~DCL_ID3D10Device(){ if (m_pcSwapChain) { m_pcSwapChain->Release(); m_pcSwapChain = nullptr; } }
#pragma endregion

#pragma region IUnknown methods
/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::QueryInterface(ID3D10Device* pcThis, REFIID riid, void **ppvObject)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::QueryInterface");
	return D3D10_ID3D10Device_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* Catch the DXGI SwapChain pointer.
***/
ULONG WINAPI DCL_ID3D10Device::AddRef(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::AddRef");

	// set reference counter in the transfer site class
	int iCount = D3D10_ID3D10Device_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] = iCount;

	// catch the swapchain pointer
	if (!m_pcSwapChain)
	{
		// get the render target
		ID3D10RenderTargetView* pRenderTargetView = nullptr;
		ID3D10DepthStencilView* pDepthStencilView = nullptr;
		pcThis->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);

		if (pRenderTargetView)
		{
			// get the buffer texture
			ID3D10Texture2D* pBuffer = nullptr;
			pRenderTargetView->GetResource((ID3D10Resource**)(&pBuffer));
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
						//if (pSwapChain) { pSwapChain->Release(); pSwapChain = nullptr; }
					}

					if (pSurface) { pSurface->Release(); pSurface = nullptr; }
				}
				if (pBuffer) { pBuffer->Release(); pBuffer = nullptr; }
			}

			if (pRenderTargetView) { pRenderTargetView->Release(); pRenderTargetView = nullptr; }
		}

		if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }
	}
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_ID3D10Device::Release(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::Release");

	// set reference counter in the transfer site class
	int iCount = D3D10_ID3D10Device_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device] = iCount;
	return iCount;
}
#pragma endregion

#pragma region ID3D10Device methods
/**
*
***/
void WINAPI DCL_ID3D10Device::VSSetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSSetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSSetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetConstantBuffers);

	D3D10_ID3D10Device_VSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSSetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView*const* ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSSetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSSetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetShaderResources);

	D3D10_ID3D10Device_PSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSSetShader(ID3D10Device* pcThis, ID3D10PixelShader *pPixelShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSSetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetShader,0,pPixelShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSSetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetShader);

	D3D10_ID3D10Device_PSSetShader_Super(pcThis, pPixelShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSSetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState*const* ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSSetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSSetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetSamplers);

	D3D10_ID3D10Device_PSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSSetShader(ID3D10Device* pcThis, ID3D10VertexShader *pVertexShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSSetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetShader,0,pVertexShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSSetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetShader);

	D3D10_ID3D10Device_VSSetShader_Super(pcThis, pVertexShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::DrawIndexed(ID3D10Device* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::DrawIndexed");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::DrawIndexed);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed,0,IndexCount);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed,1,StartIndexLocation);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexed,2,BaseVertexLocation);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::DrawIndexed);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawIndexed);

	D3D10_ID3D10Device_DrawIndexed_Super(pcThis, IndexCount, StartIndexLocation, BaseVertexLocation);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::Draw(ID3D10Device* pcThis, UINT VertexCount, UINT StartVertexLocation)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::Draw");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::Draw);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::Draw,0,VertexCount);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::Draw,1,StartVertexLocation);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::Draw);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::Draw);

	D3D10_ID3D10Device_Draw_Super(pcThis, VertexCount, StartVertexLocation);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSSetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSSetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSSetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSSetConstantBuffers);

	D3D10_ID3D10Device_PSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IASetInputLayout(ID3D10Device* pcThis, ID3D10InputLayout *pInputLayout)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IASetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IASetInputLayout);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetInputLayout,0,pInputLayout);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IASetInputLayout);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetInputLayout);

	D3D10_ID3D10Device_IASetInputLayout_Super(pcThis, pInputLayout);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IASetVertexBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IASetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IASetVertexBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,2,ppVertexBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,3,pStrides);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetVertexBuffers,4,pOffsets);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IASetVertexBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetVertexBuffers);

	D3D10_ID3D10Device_IASetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IASetIndexBuffer(ID3D10Device* pcThis, ID3D10Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IASetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IASetIndexBuffer);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,0,pIndexBuffer);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,1,Format);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetIndexBuffer,2,Offset);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IASetIndexBuffer);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetIndexBuffer);

	D3D10_ID3D10Device_IASetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::DrawIndexedInstanced(ID3D10Device* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::DrawIndexedInstanced");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::DrawIndexedInstanced);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,0,IndexCountPerInstance);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,1,InstanceCount);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,2,StartIndexLocation);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,3,BaseVertexLocation);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawIndexedInstanced,4,StartInstanceLocation);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::DrawIndexedInstanced);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawIndexedInstanced);

	D3D10_ID3D10Device_DrawIndexedInstanced_Super(pcThis, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::DrawInstanced(ID3D10Device* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::DrawInstanced");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::DrawInstanced);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced,0,VertexCountPerInstance);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced,1,InstanceCount);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced,2,StartVertexLocation);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::DrawInstanced,3,StartInstanceLocation);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::DrawInstanced);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawInstanced);

	D3D10_ID3D10Device_DrawInstanced_Super(pcThis, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSSetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSSetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSSetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSSetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetConstantBuffers);

	D3D10_ID3D10Device_GSSetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSSetShader(ID3D10Device* pcThis, ID3D10GeometryShader *pShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSSetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSSetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetShader,0,pShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSSetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetShader);

	D3D10_ID3D10Device_GSSetShader_Super(pcThis, pShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IASetPrimitiveTopology(ID3D10Device* pcThis, D3D10_PRIMITIVE_TOPOLOGY Topology)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IASetPrimitiveTopology");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IASetPrimitiveTopology);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IASetPrimitiveTopology,0,Topology);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IASetPrimitiveTopology);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IASetPrimitiveTopology);

	D3D10_ID3D10Device_IASetPrimitiveTopology_Super(pcThis, Topology);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSSetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSSetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSSetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetShaderResources);

	D3D10_ID3D10Device_VSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSSetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSSetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSSetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSSetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSSetSamplers);

	D3D10_ID3D10Device_VSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::SetPredication(ID3D10Device* pcThis, ID3D10Predicate *pPredicate, BOOL PredicateValue)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SetPredication);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPredication,0,pPredicate);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPredication,1,PredicateValue);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SetPredication);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::SetPredication);

	D3D10_ID3D10Device_SetPredication_Super(pcThis, pPredicate, PredicateValue);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSSetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSSetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSSetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSSetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetShaderResources);

	D3D10_ID3D10Device_GSSetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSSetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSSetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSSetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSSetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSSetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSSetSamplers);

	D3D10_ID3D10Device_GSSetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMSetRenderTargets(ID3D10Device* pcThis, UINT NumViews, ID3D10RenderTargetView *const *ppRenderTargetViews, ID3D10DepthStencilView *pDepthStencilView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMSetRenderTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMSetRenderTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,0,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,1,ppRenderTargetViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetRenderTargets,2,pDepthStencilView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMSetRenderTargets);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetRenderTargets);

	D3D10_ID3D10Device_OMSetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, pDepthStencilView);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMSetBlendState(ID3D10Device* pcThis, ID3D10BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMSetBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMSetBlendState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState,0,pBlendState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState,1,BlendFactor);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetBlendState,2,SampleMask);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMSetBlendState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetBlendState);

	D3D10_ID3D10Device_OMSetBlendState_Super(pcThis, pBlendState, BlendFactor, SampleMask);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMSetDepthStencilState(ID3D10Device* pcThis, ID3D10DepthStencilState *pDepthStencilState, UINT StencilRef)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMSetDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMSetDepthStencilState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetDepthStencilState,0,pDepthStencilState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMSetDepthStencilState,1,StencilRef);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMSetDepthStencilState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMSetDepthStencilState);

	D3D10_ID3D10Device_OMSetDepthStencilState_Super(pcThis, pDepthStencilState, StencilRef);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::SOSetTargets(ID3D10Device* pcThis, UINT NumBuffers, ID3D10Buffer *const *ppSOTargets, const UINT *pOffsets)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SOSetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SOSetTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,0,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,1,ppSOTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOSetTargets,2,pOffsets);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SOSetTargets);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::SOSetTargets);

	D3D10_ID3D10Device_SOSetTargets_Super(pcThis, NumBuffers, ppSOTargets, pOffsets);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::DrawAuto(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::DrawAuto");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::DrawAuto);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::DrawAuto);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::DrawAuto);

	D3D10_ID3D10Device_DrawAuto_Super(pcThis);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSSetState(ID3D10Device* pcThis, ID3D10RasterizerState *pRasterizerState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSSetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSSetState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSSetState,0,pRasterizerState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSSetState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetState);

	D3D10_ID3D10Device_RSSetState_Super(pcThis, pRasterizerState);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSSetViewports(ID3D10Device* pcThis, UINT NumViewports, const D3D10_VIEWPORT *pViewports)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSSetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSSetViewports);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSSetViewports,0,NumViewports);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSSetViewports,1,pViewports);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSSetViewports);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetViewports);

	D3D10_ID3D10Device_RSSetViewports_Super(pcThis, NumViewports, pViewports);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSSetScissorRects(ID3D10Device* pcThis, UINT NumRects, const D3D10_RECT *pRects)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSSetScissorRects");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSSetScissorRects);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSSetScissorRects,0,NumRects);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSSetScissorRects,1,pRects);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSSetScissorRects);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSSetScissorRects);

	D3D10_ID3D10Device_RSSetScissorRects_Super(pcThis, NumRects, pRects);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::CopySubresourceRegion(ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CopySubresourceRegion");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CopySubresourceRegion);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,0,pDstResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,1,DstSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,2,DstX);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,3,DstY);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,4,DstZ);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,5,pSrcResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,6,SrcSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopySubresourceRegion,7,pSrcBox);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CopySubresourceRegion);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::CopySubresourceRegion);

	D3D10_ID3D10Device_CopySubresourceRegion_Super(pcThis, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::CopyResource(ID3D10Device* pcThis, ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource)
{
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CopyResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CopyResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopyResource,0,pDstResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CopyResource,1,pSrcResource);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CopyResource);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::CopyResource);

	D3D10_ID3D10Device_CopyResource_Super(pcThis, pDstResource, pSrcResource);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::UpdateSubresource(ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::UpdateSubresource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::UpdateSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,0,pDstResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,1,DstSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,2,pDstBox);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,3,pSrcData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,4,SrcRowPitch);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::UpdateSubresource,5,SrcDepthPitch);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::UpdateSubresource);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::UpdateSubresource);

	D3D10_ID3D10Device_UpdateSubresource_Super(pcThis, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::ClearRenderTargetView(ID3D10Device* pcThis, ID3D10RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ])
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::ClearRenderTargetView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::ClearRenderTargetView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearRenderTargetView,0,pRenderTargetView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearRenderTargetView,1,ColorRGBA);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::ClearRenderTargetView);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearRenderTargetView);

	D3D10_ID3D10Device_ClearRenderTargetView_Super(pcThis, pRenderTargetView, ColorRGBA);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::ClearDepthStencilView(ID3D10Device* pcThis, ID3D10DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::ClearDepthStencilView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::ClearDepthStencilView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,0,pDepthStencilView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,1,ClearFlags);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,2,Depth);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ClearDepthStencilView,3,Stencil);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::ClearDepthStencilView);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearDepthStencilView);

	D3D10_ID3D10Device_ClearDepthStencilView_Super(pcThis, pDepthStencilView, ClearFlags, Depth, Stencil);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GenerateMips(ID3D10Device* pcThis, ID3D10ShaderResourceView *pShaderResourceView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GenerateMips");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GenerateMips);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GenerateMips,0,pShaderResourceView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GenerateMips);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GenerateMips);

	D3D10_ID3D10Device_GenerateMips_Super(pcThis, pShaderResourceView);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::ResolveSubresource(ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, ID3D10Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::ResolveSubresource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::ResolveSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,0,pDstResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,1,DstSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,2,pSrcResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,3,SrcSubresource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::ResolveSubresource,4,Format);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::ResolveSubresource);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::ResolveSubresource);

	D3D10_ID3D10Device_ResolveSubresource_Super(pcThis, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSGetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSGetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSGetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetConstantBuffers);

	D3D10_ID3D10Device_VSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSGetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSGetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSGetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetShaderResources);

	D3D10_ID3D10Device_PSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSGetShader(ID3D10Device* pcThis, ID3D10PixelShader **ppPixelShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSGetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetShader,0,ppPixelShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSGetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetShader);

	D3D10_ID3D10Device_PSGetShader_Super(pcThis, ppPixelShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSGetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSGetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSGetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetSamplers);

	D3D10_ID3D10Device_PSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSGetShader(ID3D10Device* pcThis, ID3D10VertexShader **ppVertexShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSGetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetShader,0,ppVertexShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSGetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetShader);

	D3D10_ID3D10Device_VSGetShader_Super(pcThis, ppVertexShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::PSGetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::PSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::PSGetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::PSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::PSGetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::PSGetConstantBuffers);

	D3D10_ID3D10Device_PSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IAGetInputLayout(ID3D10Device* pcThis, ID3D10InputLayout **ppInputLayout)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IAGetInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IAGetInputLayout);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetInputLayout,0,ppInputLayout);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IAGetInputLayout);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetInputLayout);

	D3D10_ID3D10Device_IAGetInputLayout_Super(pcThis, ppInputLayout);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IAGetVertexBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IAGetVertexBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IAGetVertexBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,2,ppVertexBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,3,pStrides);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetVertexBuffers,4,pOffsets);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IAGetVertexBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetVertexBuffers);

	D3D10_ID3D10Device_IAGetVertexBuffers_Super(pcThis, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IAGetIndexBuffer(ID3D10Device* pcThis, ID3D10Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IAGetIndexBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IAGetIndexBuffer);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,0,pIndexBuffer);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,1,Format);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetIndexBuffer,2,Offset);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IAGetIndexBuffer);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetIndexBuffer);

	D3D10_ID3D10Device_IAGetIndexBuffer_Super(pcThis, pIndexBuffer, Format, Offset);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSGetConstantBuffers(ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSGetConstantBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSGetConstantBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,1,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetConstantBuffers,2,ppConstantBuffers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSGetConstantBuffers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetConstantBuffers);

	D3D10_ID3D10Device_GSGetConstantBuffers_Super(pcThis, StartSlot, NumBuffers, ppConstantBuffers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSGetShader(ID3D10Device* pcThis, ID3D10GeometryShader **ppGeometryShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSGetShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSGetShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetShader,0,ppGeometryShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSGetShader);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetShader);

	D3D10_ID3D10Device_GSGetShader_Super(pcThis, ppGeometryShader);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::IAGetPrimitiveTopology(ID3D10Device* pcThis, D3D10_PRIMITIVE_TOPOLOGY *pTopology)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::IAGetPrimitiveTopology");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::IAGetPrimitiveTopology);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::IAGetPrimitiveTopology,0,pTopology);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::IAGetPrimitiveTopology);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::IAGetPrimitiveTopology);

	D3D10_ID3D10Device_IAGetPrimitiveTopology_Super(pcThis, pTopology);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSGetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSGetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSGetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetShaderResources);

	D3D10_ID3D10Device_VSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::VSGetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::VSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::VSGetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::VSGetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::VSGetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::VSGetSamplers);

	D3D10_ID3D10Device_VSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GetPredication(ID3D10Device* pcThis, ID3D10Predicate **ppPredicate, BOOL *pPredicateValue)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetPredication");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetPredication);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetPredication,0,ppPredicate);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetPredication,1,pPredicateValue);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetPredication);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GetPredication);

	D3D10_ID3D10Device_GetPredication_Super(pcThis, ppPredicate, pPredicateValue);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSGetShaderResources(ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSGetShaderResources");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSGetShaderResources);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,1,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetShaderResources,2,ppShaderResourceViews);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSGetShaderResources);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetShaderResources);

	D3D10_ID3D10Device_GSGetShaderResources_Super(pcThis, StartSlot, NumViews, ppShaderResourceViews);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GSGetSamplers(ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GSGetSamplers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GSGetSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers,0,StartSlot);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers,1,NumSamplers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GSGetSamplers,2,ppSamplers);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GSGetSamplers);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GSGetSamplers);

	D3D10_ID3D10Device_GSGetSamplers_Super(pcThis, StartSlot, NumSamplers, ppSamplers);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMGetRenderTargets(ID3D10Device* pcThis, UINT NumViews, ID3D10RenderTargetView **ppRenderTargetViews, ID3D10DepthStencilView **ppDepthStencilView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMGetRenderTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMGetRenderTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,0,NumViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,1,ppRenderTargetViews);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetRenderTargets,2,ppDepthStencilView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMGetRenderTargets);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetRenderTargets);

	D3D10_ID3D10Device_OMGetRenderTargets_Super(pcThis, NumViews, ppRenderTargetViews, ppDepthStencilView);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMGetBlendState(ID3D10Device* pcThis, ID3D10BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMGetBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMGetBlendState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState,0,ppBlendState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState,1,BlendFactor);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetBlendState,2,pSampleMask);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMGetBlendState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetBlendState);

	D3D10_ID3D10Device_OMGetBlendState_Super(pcThis, ppBlendState, BlendFactor, pSampleMask);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::OMGetDepthStencilState(ID3D10Device* pcThis, ID3D10DepthStencilState **ppDepthStencilState, UINT *pStencilRef)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OMGetDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OMGetDepthStencilState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetDepthStencilState,0,ppDepthStencilState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OMGetDepthStencilState,1,pStencilRef);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OMGetDepthStencilState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::OMGetDepthStencilState);

	D3D10_ID3D10Device_OMGetDepthStencilState_Super(pcThis, ppDepthStencilState, pStencilRef);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::SOGetTargets(ID3D10Device* pcThis, UINT NumBuffers, ID3D10Buffer **ppSOTargets, UINT *pOffsets)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SOGetTargets");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SOGetTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,0,NumBuffers);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,1,ppSOTargets);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SOGetTargets,2,pOffsets);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SOGetTargets);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::SOGetTargets);

	D3D10_ID3D10Device_SOGetTargets_Super(pcThis, NumBuffers, ppSOTargets, pOffsets);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSGetState(ID3D10Device* pcThis, ID3D10RasterizerState **ppRasterizerState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSGetState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSGetState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSGetState,0,ppRasterizerState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSGetState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetState);

	D3D10_ID3D10Device_RSGetState_Super(pcThis, ppRasterizerState);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSGetViewports(ID3D10Device* pcThis, UINT *NumViewports, D3D10_VIEWPORT *pViewports)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSGetViewports");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSGetViewports);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSGetViewports,0,NumViewports);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSGetViewports,1,pViewports);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSGetViewports);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetViewports);

	D3D10_ID3D10Device_RSGetViewports_Super(pcThis, NumViewports, pViewports);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::RSGetScissorRects(ID3D10Device* pcThis, UINT *NumRects, D3D10_RECT *pRects)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::RSGetScissorRects");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::RSGetScissorRects);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSGetScissorRects,0,NumRects);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::RSGetScissorRects,1,pRects);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::RSGetScissorRects);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::RSGetScissorRects);

	D3D10_ID3D10Device_RSGetScissorRects_Super(pcThis, NumRects, pRects);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::GetDeviceRemovedReason(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetDeviceRemovedReason");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetDeviceRemovedReason);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetDeviceRemovedReason);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::GetDeviceRemovedReason, HRESULT);

	return D3D10_ID3D10Device_GetDeviceRemovedReason_Super(pcThis);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::SetExceptionMode(ID3D10Device* pcThis, UINT RaiseFlags)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SetExceptionMode);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetExceptionMode,0,RaiseFlags);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SetExceptionMode);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::SetExceptionMode, HRESULT);

	return D3D10_ID3D10Device_SetExceptionMode_Super(pcThis, RaiseFlags);
}

/**
*
***/
UINT WINAPI DCL_ID3D10Device::GetExceptionMode(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetExceptionMode");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetExceptionMode);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetExceptionMode);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::GetExceptionMode, UINT);

	return D3D10_ID3D10Device_GetExceptionMode_Super(pcThis);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::GetPrivateData(ID3D10Device* pcThis, REFGUID guid, UINT *pDataSize, void *pData)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetPrivateData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,0,guid);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,1,pDataSize);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetPrivateData,2,pData);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetPrivateData);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::GetPrivateData, HRESULT);

	return D3D10_ID3D10Device_GetPrivateData_Super(pcThis, guid, pDataSize, pData);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::SetPrivateData(ID3D10Device* pcThis, REFGUID guid, UINT DataSize, const void *pData)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SetPrivateData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,0,guid);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,1,DataSize);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateData,2,pData);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SetPrivateData);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::SetPrivateData, HRESULT);

	return D3D10_ID3D10Device_SetPrivateData_Super(pcThis, guid, DataSize, pData);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::SetPrivateDataInterface(ID3D10Device* pcThis, REFGUID guid, const IUnknown *pData)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SetPrivateDataInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SetPrivateDataInterface);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateDataInterface,0,guid);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetPrivateDataInterface,1,pData);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SetPrivateDataInterface);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::SetPrivateDataInterface, HRESULT);

	return D3D10_ID3D10Device_SetPrivateDataInterface_Super(pcThis, guid, pData);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::ClearState(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::ClearState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::ClearState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::ClearState);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::ClearState);

	D3D10_ID3D10Device_ClearState_Super(pcThis);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::Flush(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::Flush");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::Flush);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::Flush);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::Flush);

	D3D10_ID3D10Device_Flush_Super(pcThis);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateBuffer(ID3D10Device* pcThis, const D3D10_BUFFER_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateBuffer);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,0,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,1,pInitialData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateBuffer,2,ppBuffer);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateBuffer);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateBuffer, HRESULT);

	return D3D10_ID3D10Device_CreateBuffer_Super(pcThis, pDesc, pInitialData, ppBuffer);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateTexture1D(ID3D10Device* pcThis, const D3D10_TEXTURE1D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture1D **ppTexture1D)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateTexture1D");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateTexture1D);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D,0,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D,1,pInitialData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture1D,2,ppTexture1D);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture1D);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture1D, HRESULT);

	return D3D10_ID3D10Device_CreateTexture1D_Super(pcThis, pDesc, pInitialData, ppTexture1D);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateTexture2D(ID3D10Device* pcThis, const D3D10_TEXTURE2D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture2D **ppTexture2D)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateTexture2D");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateTexture2D);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D,0,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D,1,pInitialData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture2D,2,ppTexture2D);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture2D);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture2D, HRESULT);

	return D3D10_ID3D10Device_CreateTexture2D_Super(pcThis, pDesc, pInitialData, ppTexture2D);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateTexture3D(ID3D10Device* pcThis, const D3D10_TEXTURE3D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture3D **ppTexture3D)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateTexture3D");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateTexture3D);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D,0,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D,1,pInitialData);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateTexture3D,2,ppTexture3D);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateTexture3D);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateTexture3D, HRESULT);

	return D3D10_ID3D10Device_CreateTexture3D_Super(pcThis, pDesc, pInitialData, ppTexture3D);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateShaderResourceView(ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D10ShaderResourceView **ppSRView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateShaderResourceView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateShaderResourceView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView,0,pResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView,1,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateShaderResourceView,2,ppSRView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateShaderResourceView);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateShaderResourceView, HRESULT);

	return D3D10_ID3D10Device_CreateShaderResourceView_Super(pcThis, pResource, pDesc, ppSRView);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateRenderTargetView(ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_RENDER_TARGET_VIEW_DESC *pDesc, ID3D10RenderTargetView **ppRTView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateRenderTargetView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateRenderTargetView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView,0,pResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView,1,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateRenderTargetView,2,ppRTView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateRenderTargetView);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateRenderTargetView, HRESULT);

	return D3D10_ID3D10Device_CreateRenderTargetView_Super(pcThis, pResource, pDesc, ppRTView);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateDepthStencilView(ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D10DepthStencilView **ppDepthStencilView)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateDepthStencilView");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateDepthStencilView);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView,0,pResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView,1,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilView,2,ppDepthStencilView);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateDepthStencilView);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateDepthStencilView, HRESULT);

	return D3D10_ID3D10Device_CreateDepthStencilView_Super(pcThis, pResource, pDesc, ppDepthStencilView);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateInputLayout(ID3D10Device* pcThis, const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout **ppInputLayout)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateInputLayout");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateInputLayout);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout,0,pInputElementDescs);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout,1,NumElements);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout,2,pShaderBytecodeWithInputSignature);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout,3,BytecodeLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateInputLayout,4,ppInputLayout);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateInputLayout);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateInputLayout, HRESULT);

	return D3D10_ID3D10Device_CreateInputLayout_Super(pcThis, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

/**
* 
***/
HRESULT WINAPI DCL_ID3D10Device::CreateVertexShader(ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateVertexShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateVertexShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,0,pShaderBytecode);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,1,BytecodeLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateVertexShader,2,ppVertexShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateVertexShader);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateVertexShader, HRESULT);

	//OUTPUT_DEBUG_STRING_ID3D10Device(L"Aquilinus : Create, disassemble and enumerate Vertex Shader !");

	HRESULT hr = D3D10_ID3D10Device_CreateVertexShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppVertexShader);

	// disassemble shader
	/*LPCSTR commentString = NULL;
	ID3D10Blob* pIDisassembly = NULL;
	char* pDisassembly = NULL;
	D3D10DisassembleShader( (UINT*) pShaderBytecode, BytecodeLength, TRUE, commentString, &pIDisassembly );

	//
	char* pData = (char*)pIDisassembly->GetBufferPointer();
	OUTPUT_DEBUG_STRING_ID3D10DeviceA(pData);*/


	return hr;
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateGeometryShader(ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader **ppGeometryShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateGeometryShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateGeometryShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,0,pShaderBytecode);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,1,BytecodeLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShader,2,ppGeometryShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateGeometryShader);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateGeometryShader, HRESULT);

	return D3D10_ID3D10Device_CreateGeometryShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppGeometryShader);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateGeometryShaderWithStreamOutput(ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D10_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader **ppGeometryShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateGeometryShaderWithStreamOutput");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,0,pShaderBytecode);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,1,BytecodeLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,2,pSODeclaration);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,3,NumEntries);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,4,OutputStreamStride);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput,5,ppGeometryShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput, HRESULT);

	return D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput_Super(pcThis, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreatePixelShader(ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreatePixelShader");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreatePixelShader);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader,0,pShaderBytecode);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader,1,BytecodeLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreatePixelShader,2,ppPixelShader);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreatePixelShader);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreatePixelShader, HRESULT);

	return D3D10_ID3D10Device_CreatePixelShader_Super(pcThis, pShaderBytecode, BytecodeLength, ppPixelShader);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateBlendState(ID3D10Device* pcThis, const D3D10_BLEND_DESC *pBlendStateDesc, ID3D10BlendState **ppBlendState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateBlendState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateBlendState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateBlendState,0,pBlendStateDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateBlendState,1,ppBlendState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateBlendState);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateBlendState, HRESULT);

	return D3D10_ID3D10Device_CreateBlendState_Super(pcThis, pBlendStateDesc, ppBlendState);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateDepthStencilState(ID3D10Device* pcThis, const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D10DepthStencilState **ppDepthStencilState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateDepthStencilState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateDepthStencilState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilState,0,pDepthStencilDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateDepthStencilState,1,ppDepthStencilState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateDepthStencilState);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateDepthStencilState, HRESULT);

	return D3D10_ID3D10Device_CreateDepthStencilState_Super(pcThis, pDepthStencilDesc, ppDepthStencilState);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateRasterizerState(ID3D10Device* pcThis, const D3D10_RASTERIZER_DESC *pRasterizerDesc, ID3D10RasterizerState **ppRasterizerState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateRasterizerState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateRasterizerState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateRasterizerState,0,pRasterizerDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateRasterizerState,1,ppRasterizerState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateRasterizerState);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateRasterizerState, HRESULT);

	return D3D10_ID3D10Device_CreateRasterizerState_Super(pcThis, pRasterizerDesc, ppRasterizerState);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateSamplerState(ID3D10Device* pcThis, const D3D10_SAMPLER_DESC *pSamplerDesc, ID3D10SamplerState **ppSamplerState)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateSamplerState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateSamplerState);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateSamplerState,0,pSamplerDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateSamplerState,1,ppSamplerState);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateSamplerState);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateSamplerState, HRESULT);

	return D3D10_ID3D10Device_CreateSamplerState_Super(pcThis, pSamplerDesc, ppSamplerState);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateQuery(ID3D10Device* pcThis, const D3D10_QUERY_DESC *pQueryDesc, ID3D10Query **ppQuery)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateQuery");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateQuery);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateQuery,0,pQueryDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateQuery,1,ppQuery);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateQuery);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateQuery, HRESULT);

	return D3D10_ID3D10Device_CreateQuery_Super(pcThis, pQueryDesc, ppQuery);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreatePredicate(ID3D10Device* pcThis, const D3D10_QUERY_DESC *pPredicateDesc, ID3D10Predicate **ppPredicate)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreatePredicate");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreatePredicate);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreatePredicate,0,pPredicateDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreatePredicate,1,ppPredicate);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreatePredicate);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreatePredicate, HRESULT);

	return D3D10_ID3D10Device_CreatePredicate_Super(pcThis, pPredicateDesc, ppPredicate);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CreateCounter(ID3D10Device* pcThis, const D3D10_COUNTER_DESC *pCounterDesc, ID3D10Counter **ppCounter)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CreateCounter");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CreateCounter);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateCounter,0,pCounterDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CreateCounter,1,ppCounter);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CreateCounter);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CreateCounter, HRESULT);

	return D3D10_ID3D10Device_CreateCounter_Super(pcThis, pCounterDesc, ppCounter);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CheckFormatSupport(ID3D10Device* pcThis, DXGI_FORMAT Format, UINT *pFormatSupport)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CheckFormatSupport");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CheckFormatSupport);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckFormatSupport,0,Format);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckFormatSupport,1,pFormatSupport);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CheckFormatSupport);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckFormatSupport, HRESULT);

	return D3D10_ID3D10Device_CheckFormatSupport_Super(pcThis, Format, pFormatSupport);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CheckMultisampleQualityLevels(ID3D10Device* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CheckMultisampleQualityLevels");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,0,Format);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,1,SampleCount);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels,2,pNumQualityLevels);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckMultisampleQualityLevels, HRESULT);

	return D3D10_ID3D10Device_CheckMultisampleQualityLevels_Super(pcThis, Format, SampleCount, pNumQualityLevels);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::CheckCounterInfo(ID3D10Device* pcThis, D3D10_COUNTER_INFO *pCounterInfo)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CheckCounterInfo");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CheckCounterInfo);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounterInfo,0,pCounterInfo);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CheckCounterInfo);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::CheckCounterInfo);

	D3D10_ID3D10Device_CheckCounterInfo_Super(pcThis, pCounterInfo);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::CheckCounter(ID3D10Device* pcThis, const D3D10_COUNTER_DESC *pDesc, D3D10_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::CheckCounter");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::CheckCounter);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,0,pDesc);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,1,pType);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,2,pActiveCounters);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,3,szName);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,4,pNameLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,5,szUnits);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,6,pUnitsLength);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,7,szDescription);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::CheckCounter,8,pDescriptionLength);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::CheckCounter);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::CheckCounter, HRESULT);

	return D3D10_ID3D10Device_CheckCounter_Super(pcThis, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

/**
*
***/
UINT WINAPI DCL_ID3D10Device::GetCreationFlags(ID3D10Device* pcThis)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetCreationFlags");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetCreationFlags);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetCreationFlags);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::GetCreationFlags, UINT);

	return D3D10_ID3D10Device_GetCreationFlags_Super(pcThis);
}

/**
*
***/
HRESULT WINAPI DCL_ID3D10Device::OpenSharedResource(ID3D10Device* pcThis, HANDLE hResource, REFIID ReturnedInterface, void **ppResource)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::OpenSharedResource");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::OpenSharedResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,0,hResource);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,1,ReturnedInterface);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::OpenSharedResource,2,ppResource);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::OpenSharedResource);
	AQU_ID3D10DEVICE_REPLACE_METHOD(VMT_ID3D10DEVICE::OpenSharedResource, HRESULT);

	return D3D10_ID3D10Device_OpenSharedResource_Super(pcThis, hResource, ReturnedInterface, ppResource);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::SetTextFilterSize(ID3D10Device* pcThis, UINT Width, UINT Height)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::SetTextFilterSize");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::SetTextFilterSize);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetTextFilterSize,0,Width);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::SetTextFilterSize,1,Height);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::SetTextFilterSize);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::SetTextFilterSize);

	D3D10_ID3D10Device_SetTextFilterSize_Super(pcThis, Width, Height);
}

/**
*
***/
void WINAPI DCL_ID3D10Device::GetTextFilterSize(ID3D10Device* pcThis, UINT *pWidth, UINT *pHeight)
{ 
	OUTPUT_DEBUG_STRING_ID3D10Device(L"ID3DDevice10::GetTextFilterSize");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_ID3D10DEVICE_HEADER________(VMT_ID3D10DEVICE::GetTextFilterSize);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetTextFilterSize,0,pWidth);
	AQU_ID3D10DEVICE_SET_DATA______(VMT_ID3D10DEVICE::GetTextFilterSize,1,pHeight);
	AQU_ID3D10DEVICE_PROVOKE_______(VMT_ID3D10DEVICE::GetTextFilterSize);
	AQU_ID3D10DEVICE_REPLACE_VOID__(VMT_ID3D10DEVICE::GetTextFilterSize);

	D3D10_ID3D10Device_GetTextFilterSize_Super(pcThis, pWidth, pHeight);
}

#pragma endregion

#pragma region DCL_IDXGISwapChain public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D10Device.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D10Device::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface:                      *(PUINT_PTR)&D3D10_ID3D10Device_QueryInterface_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:                              *(PUINT_PTR)&D3D10_ID3D10Device_AddRef_Super                               = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:                             *(PUINT_PTR)&D3D10_ID3D10Device_Release_Super                              = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_ID3D10Device.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_ID3D10Device::SetSuperFunctionPointer(VMT_ID3D10DEVICE::VMT_ID3D10Device eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** ID3D10Device super methods ***/
	case VMT_ID3D10DEVICE::VSSetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_VSSetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_PSSetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_PSSetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_PSSetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_VSSetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawIndexed:                         *(PUINT_PTR)&D3D10_ID3D10Device_DrawIndexed_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::Draw:                                *(PUINT_PTR)&D3D10_ID3D10Device_Draw_Super                                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSSetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_PSSetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetInputLayout:                    *(PUINT_PTR)&D3D10_ID3D10Device_IASetInputLayout_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetVertexBuffers:                  *(PUINT_PTR)&D3D10_ID3D10Device_IASetVertexBuffers_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetIndexBuffer:                    *(PUINT_PTR)&D3D10_ID3D10Device_IASetIndexBuffer_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawIndexedInstanced:                *(PUINT_PTR)&D3D10_ID3D10Device_DrawIndexedInstanced_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawInstanced:                       *(PUINT_PTR)&D3D10_ID3D10Device_DrawInstanced_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_GSSetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_GSSetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IASetPrimitiveTopology:              *(PUINT_PTR)&D3D10_ID3D10Device_IASetPrimitiveTopology_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_VSSetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSSetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_VSSetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPredication:                      *(PUINT_PTR)&D3D10_ID3D10Device_SetPredication_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_GSSetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSSetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_GSSetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetRenderTargets:                  *(PUINT_PTR)&D3D10_ID3D10Device_OMSetRenderTargets_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetBlendState:                     *(PUINT_PTR)&D3D10_ID3D10Device_OMSetBlendState_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMSetDepthStencilState:              *(PUINT_PTR)&D3D10_ID3D10Device_OMSetDepthStencilState_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SOSetTargets:                        *(PUINT_PTR)&D3D10_ID3D10Device_SOSetTargets_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::DrawAuto:                            *(PUINT_PTR)&D3D10_ID3D10Device_DrawAuto_Super                             = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetState:                          *(PUINT_PTR)&D3D10_ID3D10Device_RSSetState_Super                           = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetViewports:                      *(PUINT_PTR)&D3D10_ID3D10Device_RSSetViewports_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSSetScissorRects:                   *(PUINT_PTR)&D3D10_ID3D10Device_RSSetScissorRects_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CopySubresourceRegion:               *(PUINT_PTR)&D3D10_ID3D10Device_CopySubresourceRegion_Super                = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CopyResource:                        *(PUINT_PTR)&D3D10_ID3D10Device_CopyResource_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::UpdateSubresource:                   *(PUINT_PTR)&D3D10_ID3D10Device_UpdateSubresource_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearRenderTargetView:               *(PUINT_PTR)&D3D10_ID3D10Device_ClearRenderTargetView_Super                = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearDepthStencilView:               *(PUINT_PTR)&D3D10_ID3D10Device_ClearDepthStencilView_Super                = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GenerateMips:                        *(PUINT_PTR)&D3D10_ID3D10Device_GenerateMips_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ResolveSubresource:                  *(PUINT_PTR)&D3D10_ID3D10Device_ResolveSubresource_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_VSGetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_PSGetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_PSGetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_PSGetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_VSGetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::PSGetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_PSGetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetInputLayout:                    *(PUINT_PTR)&D3D10_ID3D10Device_IAGetInputLayout_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetVertexBuffers:                  *(PUINT_PTR)&D3D10_ID3D10Device_IAGetVertexBuffers_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetIndexBuffer:                    *(PUINT_PTR)&D3D10_ID3D10Device_IAGetIndexBuffer_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetConstantBuffers:                *(PUINT_PTR)&D3D10_ID3D10Device_GSGetConstantBuffers_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetShader:                         *(PUINT_PTR)&D3D10_ID3D10Device_GSGetShader_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::IAGetPrimitiveTopology:              *(PUINT_PTR)&D3D10_ID3D10Device_IAGetPrimitiveTopology_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_VSGetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::VSGetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_VSGetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetPredication:                      *(PUINT_PTR)&D3D10_ID3D10Device_GetPredication_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetShaderResources:                *(PUINT_PTR)&D3D10_ID3D10Device_GSGetShaderResources_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GSGetSamplers:                       *(PUINT_PTR)&D3D10_ID3D10Device_GSGetSamplers_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetRenderTargets:                  *(PUINT_PTR)&D3D10_ID3D10Device_OMGetRenderTargets_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetBlendState:                     *(PUINT_PTR)&D3D10_ID3D10Device_OMGetBlendState_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OMGetDepthStencilState:              *(PUINT_PTR)&D3D10_ID3D10Device_OMGetDepthStencilState_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SOGetTargets:                        *(PUINT_PTR)&D3D10_ID3D10Device_SOGetTargets_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetState:                          *(PUINT_PTR)&D3D10_ID3D10Device_RSGetState_Super                           = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetViewports:                      *(PUINT_PTR)&D3D10_ID3D10Device_RSGetViewports_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::RSGetScissorRects:                   *(PUINT_PTR)&D3D10_ID3D10Device_RSGetScissorRects_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetDeviceRemovedReason:              *(PUINT_PTR)&D3D10_ID3D10Device_GetDeviceRemovedReason_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetExceptionMode:                    *(PUINT_PTR)&D3D10_ID3D10Device_SetExceptionMode_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetExceptionMode:                    *(PUINT_PTR)&D3D10_ID3D10Device_GetExceptionMode_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetPrivateData:                      *(PUINT_PTR)&D3D10_ID3D10Device_GetPrivateData_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPrivateData:                      *(PUINT_PTR)&D3D10_ID3D10Device_SetPrivateData_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetPrivateDataInterface:             *(PUINT_PTR)&D3D10_ID3D10Device_SetPrivateDataInterface_Super              = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::ClearState:                          *(PUINT_PTR)&D3D10_ID3D10Device_ClearState_Super                           = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::Flush:                               *(PUINT_PTR)&D3D10_ID3D10Device_Flush_Super                                = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateBuffer:                        *(PUINT_PTR)&D3D10_ID3D10Device_CreateBuffer_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture1D:                     *(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture1D_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture2D:                     *(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture2D_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateTexture3D:                     *(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture3D_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateShaderResourceView:            *(PUINT_PTR)&D3D10_ID3D10Device_CreateShaderResourceView_Super             = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateRenderTargetView:              *(PUINT_PTR)&D3D10_ID3D10Device_CreateRenderTargetView_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateDepthStencilView:              *(PUINT_PTR)&D3D10_ID3D10Device_CreateDepthStencilView_Super               = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateInputLayout:                   *(PUINT_PTR)&D3D10_ID3D10Device_CreateInputLayout_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateVertexShader:                  *(PUINT_PTR)&D3D10_ID3D10Device_CreateVertexShader_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateGeometryShader:                *(PUINT_PTR)&D3D10_ID3D10Device_CreateGeometryShader_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput:*(PUINT_PTR)&D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreatePixelShader:                   *(PUINT_PTR)&D3D10_ID3D10Device_CreatePixelShader_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateBlendState:                    *(PUINT_PTR)&D3D10_ID3D10Device_CreateBlendState_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateDepthStencilState:             *(PUINT_PTR)&D3D10_ID3D10Device_CreateDepthStencilState_Super              = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateRasterizerState:               *(PUINT_PTR)&D3D10_ID3D10Device_CreateRasterizerState_Super                = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateSamplerState:                  *(PUINT_PTR)&D3D10_ID3D10Device_CreateSamplerState_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateQuery:                         *(PUINT_PTR)&D3D10_ID3D10Device_CreateQuery_Super                          = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreatePredicate:                     *(PUINT_PTR)&D3D10_ID3D10Device_CreatePredicate_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CreateCounter:                       *(PUINT_PTR)&D3D10_ID3D10Device_CreateCounter_Super                        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckFormatSupport:                  *(PUINT_PTR)&D3D10_ID3D10Device_CheckFormatSupport_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckMultisampleQualityLevels:       *(PUINT_PTR)&D3D10_ID3D10Device_CheckMultisampleQualityLevels_Super        = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckCounterInfo:                    *(PUINT_PTR)&D3D10_ID3D10Device_CheckCounterInfo_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::CheckCounter:                        *(PUINT_PTR)&D3D10_ID3D10Device_CheckCounter_Super                         = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetCreationFlags:                    *(PUINT_PTR)&D3D10_ID3D10Device_GetCreationFlags_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::OpenSharedResource:                  *(PUINT_PTR)&D3D10_ID3D10Device_OpenSharedResource_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::SetTextFilterSize:                   *(PUINT_PTR)&D3D10_ID3D10Device_SetTextFilterSize_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_ID3D10DEVICE::GetTextFilterSize:                   *(PUINT_PTR)&D3D10_ID3D10Device_GetTextFilterSize_Super                    = (UINT_PTR)dwFunc; break;
	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_ID3D10Device::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D10_ID3D10Device_QueryInterface_Super                       = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D10_ID3D10Device_AddRef_Super                               = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D10_ID3D10Device_Release_Super                              = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** ID3D10Device super methods ***/
	*(PUINT_PTR)&D3D10_ID3D10Device_VSSetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSSetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSSetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSSetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSSetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_DrawIndexed_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawIndexed];
	*(PUINT_PTR)&D3D10_ID3D10Device_Draw_Super                                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::Draw];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSSetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_IASetInputLayout_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetInputLayout];
	*(PUINT_PTR)&D3D10_ID3D10Device_IASetVertexBuffers_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetVertexBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_IASetIndexBuffer_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetIndexBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device_DrawIndexedInstanced_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawIndexedInstanced];
	*(PUINT_PTR)&D3D10_ID3D10Device_DrawInstanced_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawInstanced];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSSetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSSetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_IASetPrimitiveTopology_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IASetPrimitiveTopology];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSSetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSSetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSSetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_SetPredication_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPredication];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSSetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSSetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSSetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMSetRenderTargets_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetRenderTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMSetBlendState_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetBlendState];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMSetDepthStencilState_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMSetDepthStencilState];
	*(PUINT_PTR)&D3D10_ID3D10Device_SOSetTargets_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SOSetTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device_DrawAuto_Super                             = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::DrawAuto];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSSetState_Super                           = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetState];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSSetViewports_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetViewports];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSSetScissorRects_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSSetScissorRects];
	*(PUINT_PTR)&D3D10_ID3D10Device_CopySubresourceRegion_Super                = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CopySubresourceRegion];
	*(PUINT_PTR)&D3D10_ID3D10Device_CopyResource_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CopyResource];
	*(PUINT_PTR)&D3D10_ID3D10Device_UpdateSubresource_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::UpdateSubresource];
	*(PUINT_PTR)&D3D10_ID3D10Device_ClearRenderTargetView_Super                = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearRenderTargetView];
	*(PUINT_PTR)&D3D10_ID3D10Device_ClearDepthStencilView_Super                = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearDepthStencilView];
	*(PUINT_PTR)&D3D10_ID3D10Device_GenerateMips_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GenerateMips];
	*(PUINT_PTR)&D3D10_ID3D10Device_ResolveSubresource_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ResolveSubresource];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSGetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSGetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSGetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSGetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSGetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_PSGetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::PSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_IAGetInputLayout_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetInputLayout];
	*(PUINT_PTR)&D3D10_ID3D10Device_IAGetVertexBuffers_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetVertexBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_IAGetIndexBuffer_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetIndexBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSGetConstantBuffers_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetConstantBuffers];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSGetShader_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_IAGetPrimitiveTopology_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::IAGetPrimitiveTopology];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSGetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_VSGetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::VSGetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetPredication_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetPredication];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSGetShaderResources_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetShaderResources];
	*(PUINT_PTR)&D3D10_ID3D10Device_GSGetSamplers_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GSGetSamplers];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMGetRenderTargets_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetRenderTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMGetBlendState_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetBlendState];
	*(PUINT_PTR)&D3D10_ID3D10Device_OMGetDepthStencilState_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OMGetDepthStencilState];
	*(PUINT_PTR)&D3D10_ID3D10Device_SOGetTargets_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SOGetTargets];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSGetState_Super                           = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetState];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSGetViewports_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetViewports];
	*(PUINT_PTR)&D3D10_ID3D10Device_RSGetScissorRects_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::RSGetScissorRects];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetDeviceRemovedReason_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetDeviceRemovedReason];
	*(PUINT_PTR)&D3D10_ID3D10Device_SetExceptionMode_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetExceptionMode];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetExceptionMode_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetExceptionMode];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetPrivateData_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetPrivateData];
	*(PUINT_PTR)&D3D10_ID3D10Device_SetPrivateData_Super                       = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPrivateData];
	*(PUINT_PTR)&D3D10_ID3D10Device_SetPrivateDataInterface_Super              = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetPrivateDataInterface];
	*(PUINT_PTR)&D3D10_ID3D10Device_ClearState_Super                           = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::ClearState];
	*(PUINT_PTR)&D3D10_ID3D10Device_Flush_Super                                = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::Flush];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateBuffer_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateBuffer];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture1D_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture1D];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture2D_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture2D];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateTexture3D_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateTexture3D];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateShaderResourceView_Super             = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateShaderResourceView];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateRenderTargetView_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateRenderTargetView];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateDepthStencilView_Super               = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateDepthStencilView];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateInputLayout_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateInputLayout];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateVertexShader_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateVertexShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateGeometryShader_Super                 = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateGeometryShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateGeometryShaderWithStreamOutput_Super = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreatePixelShader_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreatePixelShader];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateBlendState_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateBlendState];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateDepthStencilState_Super              = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateDepthStencilState];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateRasterizerState_Super                = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateRasterizerState];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateSamplerState_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateSamplerState];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateQuery_Super                          = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateQuery];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreatePredicate_Super                      = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreatePredicate];
	*(PUINT_PTR)&D3D10_ID3D10Device_CreateCounter_Super                        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CreateCounter];
	*(PUINT_PTR)&D3D10_ID3D10Device_CheckFormatSupport_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckFormatSupport];
	*(PUINT_PTR)&D3D10_ID3D10Device_CheckMultisampleQualityLevels_Super        = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckMultisampleQualityLevels];
	*(PUINT_PTR)&D3D10_ID3D10Device_CheckCounterInfo_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckCounterInfo];
	*(PUINT_PTR)&D3D10_ID3D10Device_CheckCounter_Super                         = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::CheckCounter];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetCreationFlags_Super                     = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetCreationFlags];
	*(PUINT_PTR)&D3D10_ID3D10Device_OpenSharedResource_Super                   = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::OpenSharedResource];
	*(PUINT_PTR)&D3D10_ID3D10Device_SetTextFilterSize_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::SetTextFilterSize];
	*(PUINT_PTR)&D3D10_ID3D10Device_GetTextFilterSize_Super                    = (UINT_PTR)pVMTable[VMT_ID3D10DEVICE::GetTextFilterSize];

	return S_OK;
}

/**
* Provides the swapchain pointer.
***/
HRESULT DCL_ID3D10Device::GetDXGISwapChain(IDXGISwapChain** ppcSwapChain)
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