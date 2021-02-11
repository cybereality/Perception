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
#define OUTPUT_DEBUG_STRING_IDXGISwapChain(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDXGISwapChain(...)
#endif
#define AQU_IDXGISWAPCHAIN_HEADER________(a) m_pcTransferSite->m_dwMainThreadId = GetCurrentThreadId(); if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]) && (m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_IDXGISWAPCHAIN_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_IDXGISWAPCHAIN_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_IDXGISWAPCHAIN_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_IDXGISWAPCHAIN_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDXGISwapChain[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }

#include"DCL_IDXGISwapChain.h"
#include"DCL_IDXGISwapChain_Super.h"

#pragma region DCL_IDXGISwapChain constructor/destructor
/**
*
***/
DCL_IDXGISwapChain::DCL_IDXGISwapChain(AQU_TransferSite* pcTransferSite) :
	m_pcTransferSite(pcTransferSite),
	m_pID3D10Device(NULL),
	m_pID3D11Device(NULL)
{
}

/**
*
***/
DCL_IDXGISwapChain::~DCL_IDXGISwapChain() {}
#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::QueryInterface(IDXGISwapChain* pcThis, REFIID riid, void** ppvObject)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::QueryInterface");
	return D3D10_IDXGISwapChain_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
*
***/
ULONG WINAPI DCL_IDXGISwapChain::AddRef(IDXGISwapChain* pcThis)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::AddRef");

	// set reference counter in the transfer site class
	int iCount = D3D10_IDXGISwapChain_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDXGISwapChain::Release(IDXGISwapChain* pcThis)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::Release");

	// set reference counter in the transfer site class
	int iCount = D3D10_IDXGISwapChain_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDXGIObject methods

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::SetPrivateData(IDXGISwapChain* pcThis, REFGUID Name, UINT DataSize, const void* pData)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGIOBJECT::SetPrivateData);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData, 0, Name);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData, 1, DataSize);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData, 2, pData);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGIOBJECT::SetPrivateData);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGIOBJECT::SetPrivateData, HRESULT);

	return D3D10_IDXGISwapChain_SetPrivateData_Super(pcThis, Name, DataSize, pData);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::SetPrivateDataInterface(IDXGISwapChain* pcThis, REFGUID Name, const IUnknown* pUnknown)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::SetPrivateDataInterface");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGIOBJECT::SetPrivateDataInterface);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateDataInterface, 0, Name);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateDataInterface, 1, pUnknown);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGIOBJECT::SetPrivateDataInterface);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGIOBJECT::SetPrivateDataInterface, HRESULT);

	return D3D10_IDXGISwapChain_SetPrivateDataInterface_Super(pcThis, Name, pUnknown);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetPrivateData(IDXGISwapChain* pcThis, REFGUID Name, UINT* pDataSize, void* pData)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGIOBJECT::GetPrivateData);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData, 0, Name);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData, 1, pDataSize);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData, 2, pData);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGIOBJECT::GetPrivateData);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGIOBJECT::GetPrivateData, HRESULT);

	return D3D10_IDXGISwapChain_GetPrivateData_Super(pcThis, Name, pDataSize, pData);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetParent(IDXGISwapChain* pcThis, REFIID riid, void** ppParent)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetParent");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGIOBJECT::GetParent);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::GetParent, 0, riid);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIOBJECT::GetParent, 1, ppParent);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGIOBJECT::GetParent);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGIOBJECT::GetParent, HRESULT);

	return D3D10_IDXGISwapChain_GetParent_Super(pcThis, riid, ppParent);
}

#pragma endregion

#pragma region IDXGIDeviceSubObject methods

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetDevice(IDXGISwapChain* pcThis, REFIID riid, void** ppDevice)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetDevice");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGIDEVICESUBOBJECT::GetDevice);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIDEVICESUBOBJECT::GetDevice, 0, riid);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGIDEVICESUBOBJECT::GetDevice, 1, ppDevice);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGIDEVICESUBOBJECT::GetDevice);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGIDEVICESUBOBJECT::GetDevice, HRESULT);

	return D3D10_IDXGISwapChain_GetDevice_Super(pcThis, riid, ppDevice);
}

#pragma endregion

#pragma region IDXGISwapChain methods 
/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::Present(IDXGISwapChain* pcThis, UINT SyncInterval, UINT Flags)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::Present");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::Present);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::Present, 0, SyncInterval);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::Present, 1, Flags);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::Present);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::Present, HRESULT);

	return D3D10_IDXGISwapChain_Present_Super(pcThis, SyncInterval, Flags);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetBuffer(IDXGISwapChain* pcThis, UINT Buffer, REFIID riid, void** ppSurface)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetBuffer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetBuffer);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer, 0, Buffer);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer, 1, riid);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer, 2, ppSurface);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetBuffer);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetBuffer, HRESULT);

	return D3D10_IDXGISwapChain_GetBuffer_Super(pcThis, Buffer, riid, ppSurface);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::SetFullscreenState(IDXGISwapChain* pcThis, BOOL Fullscreen, IDXGIOutput* pTarget)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::SetFullscreenState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::SetFullscreenState);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::SetFullscreenState, 0, Fullscreen);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::SetFullscreenState, 1, pTarget);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::SetFullscreenState);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::SetFullscreenState, HRESULT);

	return D3D10_IDXGISwapChain_SetFullscreenState_Super(pcThis, Fullscreen, pTarget);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetFullscreenState(IDXGISwapChain* pcThis, BOOL* pFullscreen, IDXGIOutput** ppTarget)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetFullscreenState");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetFullscreenState);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFullscreenState, 0, pFullscreen);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFullscreenState, 1, ppTarget);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetFullscreenState);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetFullscreenState, HRESULT);

	return D3D10_IDXGISwapChain_GetFullscreenState_Super(pcThis, pFullscreen, ppTarget);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetDesc(IDXGISwapChain* pcThis, DXGI_SWAP_CHAIN_DESC* pDesc)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetDesc");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetDesc);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetDesc, 0, pDesc);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetDesc);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetDesc, HRESULT);

	return D3D10_IDXGISwapChain_GetDesc_Super(pcThis, pDesc);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::ResizeBuffers(IDXGISwapChain* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::ResizeBuffers");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::ResizeBuffers);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers, 0, BufferCount);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers, 1, Width);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers, 2, Height);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers, 3, NewFormat);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers, 4, SwapChainFlags);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::ResizeBuffers);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::ResizeBuffers, HRESULT);

	return D3D10_IDXGISwapChain_ResizeBuffers_Super(pcThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::ResizeTarget(IDXGISwapChain* pcThis, const DXGI_MODE_DESC* pNewTargetParameters)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::ResizeTarget");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::ResizeTarget);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeTarget, 0, pNewTargetParameters);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::ResizeTarget);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::ResizeTarget, HRESULT);

	return D3D10_IDXGISwapChain_ResizeTarget_Super(pcThis, pNewTargetParameters);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetContainingOutput(IDXGISwapChain* pcThis, IDXGIOutput** ppOutput)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetContainingOutput");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetContainingOutput);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetContainingOutput, 0, ppOutput);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetContainingOutput);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetContainingOutput, HRESULT);

	return D3D10_IDXGISwapChain_GetContainingOutput_Super(pcThis, ppOutput);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetFrameStatistics(IDXGISwapChain* pcThis, DXGI_FRAME_STATISTICS* pStats)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetFrameStatistics");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetFrameStatistics);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFrameStatistics, 0, pStats);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetFrameStatistics);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetFrameStatistics, HRESULT);

	return D3D10_IDXGISwapChain_GetFrameStatistics_Super(pcThis, pStats);
}

/**
*
***/
HRESULT WINAPI DCL_IDXGISwapChain::GetLastPresentCount(IDXGISwapChain* pcThis, UINT* pLastPresentCount)
{
	OUTPUT_DEBUG_STRING_IDXGISwapChain(L"DCL_IDXGISwapChain::GetLastPresentCount");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDXGISWAPCHAIN_HEADER________(VMT_IDXGISWAPCHAIN::GetLastPresentCount);
	AQU_IDXGISWAPCHAIN_SET_DATA______(VMT_IDXGISWAPCHAIN::GetLastPresentCount, 0, pLastPresentCount);
	AQU_IDXGISWAPCHAIN_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetLastPresentCount);
	AQU_IDXGISWAPCHAIN_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetLastPresentCount, HRESULT);

	return D3D10_IDXGISwapChain_GetLastPresentCount_Super(pcThis, pLastPresentCount);
}

#pragma endregion

#pragma region DCL_IDXGISwapChain public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface:*(PUINT_PTR)&D3D10_IDXGISwapChain_QueryInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:*(PUINT_PTR)&D3D10_IDXGISwapChain_AddRef_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:*(PUINT_PTR)&D3D10_IDXGISwapChain_Release_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain::SetSuperFunctionPointer(VMT_IDXGIOBJECT::VMT_IDXGIObject eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IDXGIObject super methods ***/
	case VMT_IDXGIOBJECT::SetPrivateData:*(PUINT_PTR)&D3D10_IDXGISwapChain_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGIOBJECT::SetPrivateDataInterface:*(PUINT_PTR)&D3D10_IDXGISwapChain_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGIOBJECT::GetPrivateData:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGIOBJECT::GetParent:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetParent_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain::SetSuperFunctionPointer(VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IDXGIDeviceSubObject super methods ***/
	case VMT_IDXGIDEVICESUBOBJECT::GetDevice:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetDevice_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain::SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IDXGISwapChain super methods ***/
	case VMT_IDXGISWAPCHAIN::Present:*(PUINT_PTR)&D3D10_IDXGISwapChain_Present_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetBuffer:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetBuffer_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::SetFullscreenState:*(PUINT_PTR)&D3D10_IDXGISwapChain_SetFullscreenState_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetFullscreenState:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetFullscreenState_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetDesc:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetDesc_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::ResizeBuffers:*(PUINT_PTR)&D3D10_IDXGISwapChain_ResizeBuffers_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::ResizeTarget:*(PUINT_PTR)&D3D10_IDXGISwapChain_ResizeTarget_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetContainingOutput:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetContainingOutput_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetFrameStatistics:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetFrameStatistics_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDXGISWAPCHAIN::GetLastPresentCount:*(PUINT_PTR)&D3D10_IDXGISwapChain_GetLastPresentCount_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDXGISwapChain::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D10_IDXGISwapChain_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDXGIObject super methods ***/
	*(PUINT_PTR)&D3D10_IDXGISwapChain_SetPrivateData_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::SetPrivateData];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_SetPrivateDataInterface_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetPrivateData_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::GetPrivateData];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetParent_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::GetParent];

	/*** IDXGIDeviceSubObject super methods ***/
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetDevice_Super = (UINT_PTR)pVMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice];

	/*** IDXGISwapChain super methods ***/
	*(PUINT_PTR)&D3D10_IDXGISwapChain_Present_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::Present];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetBuffer_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetBuffer];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_SetFullscreenState_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetFullscreenState_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetDesc_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetDesc];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_ResizeBuffers_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_ResizeTarget_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::ResizeTarget];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetContainingOutput_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetFrameStatistics_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics];
	*(PUINT_PTR)&D3D10_IDXGISwapChain_GetLastPresentCount_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount];

	return S_OK;
}

/**
* Provides the stored DX 10 device pointer.
***/
HRESULT DCL_IDXGISwapChain::GetD3D10Device(ID3D10Device** ppDevice)
{
	if (m_pID3D10Device == NULL)
	{
		*ppDevice = NULL;
		return E_FAIL;
	}

	*ppDevice = m_pID3D10Device;

	return S_OK;
}

/**
* Provides the stored DX 11 device pointer.
***/
HRESULT DCL_IDXGISwapChain::GetD3D11Device(ID3D11Device** ppDevice)
{
	if (m_pID3D11Device == NULL)
	{
		*ppDevice = NULL;
		return E_FAIL;
	}

	*ppDevice = m_pID3D11Device;

	return S_OK;
}
#pragma endregion