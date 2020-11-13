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
#define OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(...)
#endif
#define AQU_IDIRECT3DSWAPCHAIN9_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[a]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_IDIRECT3DSWAPCHAIN9_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[a]->m_paCommanders[b]->m_pOutput = (void*)&c;
#define AQU_IDIRECT3DSWAPCHAIN9_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_IDIRECT3DSWAPCHAIN9_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }

#include"DCL_IDirect3DSwapChain9.h"
#include"DCL_IDirect3DSwapChain9_Super.h"

#pragma region DCL_IDirect3DSwapChain9 constructor/destructor

/**
*
***/
DCL_IDirect3DSwapChain9::DCL_IDirect3DSwapChain9(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{}

/**
*
***/
DCL_IDirect3DSwapChain9::~DCL_IDirect3DSwapChain9(){}

#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::QueryInterface(IDirect3DSwapChain9* pcThis, REFIID riid, void** ppvObj)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::QueryInterface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]->m_paCommanders[0]->m_pOutput = (void*)&riid;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]->m_paCommanders[1]->m_pOutput = (void*)&ppvObj;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_QueryInterface_Super(pcThis, riid, ppvObj);
	}
	else
		return D3D9_IDirect3DSwapChain9_QueryInterface_Super(pcThis, riid, ppvObj);
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DSwapChain9::AddRef(IDirect3DSwapChain9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::AddRef");

	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DSwapChain9_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DSwapChain9::Release(IDirect3DSwapChain9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::Release");

	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DSwapChain9_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDirect3DSwapChain9 methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::Present(IDirect3DSwapChain9* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::Present");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_paCommanders[0]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_paCommanders[1]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_paCommanders[2]->m_pOutput = (void*)&hDestWindowOverride;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_paCommanders[3]->m_pOutput = (void*)&pDirtyRegion;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_paCommanders[4]->m_pOutput = (void*)&dwFlags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::Present]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	else
		return D3D9_IDirect3DSwapChain9_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetFrontBufferData(IDirect3DSwapChain9* pcThis, IDirect3DSurface9* pDestSurface)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetFrontBufferData");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData]->m_paCommanders[0]->m_pOutput = (void*)&pDestSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetFrontBufferData_Super(pcThis, pDestSurface);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetFrontBufferData_Super(pcThis, pDestSurface);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetBackBuffer(IDirect3DSwapChain9* pcThis, UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetBackBuffer");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->m_paCommanders[0]->m_pOutput = (void*)&iBackBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->m_paCommanders[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->m_paCommanders[2]->m_pOutput = (void*)&ppBackBuffer;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetBackBuffer_Super(pcThis, iBackBuffer, Type, ppBackBuffer);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetBackBuffer_Super(pcThis, iBackBuffer, Type, ppBackBuffer);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetRasterStatus(IDirect3DSwapChain9* pcThis, D3DRASTER_STATUS* pRasterStatus)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetRasterStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus]->m_paCommanders[0]->m_pOutput = (void*)&pRasterStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetRasterStatus_Super(pcThis, pRasterStatus);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetRasterStatus_Super(pcThis, pRasterStatus);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetDisplayMode(IDirect3DSwapChain9* pcThis, D3DDISPLAYMODE* pMode)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetDisplayMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode]->m_paCommanders[0]->m_pOutput = (void*)&pMode;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetDisplayMode_Super(pcThis, pMode);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetDisplayMode_Super(pcThis, pMode);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetDevice(IDirect3DSwapChain9* pcThis, IDirect3DDevice9** ppDevice)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetDevice");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDevice]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDevice]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDevice]->m_paCommanders[0]->m_pOutput = (void*)&ppDevice;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDevice]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetDevice]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetDevice_Super(pcThis, ppDevice);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetDevice_Super(pcThis, ppDevice);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DSwapChain9::GetPresentParameters(IDirect3DSwapChain9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DSwapChain9(L"IDirect3DSwapChain9::GetPresentParameters");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters]) && (m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters]->m_paCommanders[0]->m_pOutput = (void*)&pPresentationParameters;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DSwapChain9[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D9_IDirect3DSwapChain9_GetPresentParameters_Super(pcThis, pPresentationParameters);
	}
	else
		return D3D9_IDirect3DSwapChain9_GetPresentParameters_Super(pcThis, pPresentationParameters);
}

#pragma endregion

#pragma region DCL_IDirect3DSwapChain9 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DSwapChain9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DSwapChain9::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface: *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_QueryInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:         *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_AddRef_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:        *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_Release_Super        = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
};

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DSwapChain9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DSwapChain9::SetSuperFunctionPointer(VMT_IDIRECT3DSWAPCHAIN9::VMT_IDirect3DSwapChain9 eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IDirect3DSwapChain9 super methods ***/
	case VMT_IDIRECT3DSWAPCHAIN9::Present:              *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_Present_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData:   *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetFrontBufferData_Super   = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer:        *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetBackBuffer_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus:      *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetRasterStatus_Super      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode:       *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetDisplayMode_Super       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetDevice:            *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetDevice_Super            = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters: *(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetPresentParameters_Super = (UINT_PTR)dwFunc; break;
	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDirect3DSwapChain9::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_QueryInterface_Super       = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_AddRef_Super               = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_Release_Super              = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDirect3DSwapChain9 super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_Present_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::Present];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetFrontBufferData_Super   = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetFrontBufferData];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetBackBuffer_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetBackBuffer];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetRasterStatus_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetRasterStatus];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetDisplayMode_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDisplayMode];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetDevice_Super            = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetDevice];
	*(PUINT_PTR)&D3D9_IDirect3DSwapChain9_GetPresentParameters_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DSWAPCHAIN9::GetPresentParameters];

	return S_OK;
}
#pragma endregion