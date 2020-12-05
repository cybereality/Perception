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
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(...)
#endif

#include"DCL_IDirect3DDevice9Ex.h"
#include"DCL_IDirect3DDevice9Ex_Super.h"

#pragma region helper
/**
* Get hash code helper, same as in file manager class.
***/ 
inline DWORD GetHashCode(BYTE* pcData, DWORD dwSize)
{
	DWORD h = 0;

	// create hash
	for (DWORD i = 0; i < dwSize; i++) 
	{
		h = 31*h + pcData[i];
	}

	return h;
}
#pragma endregion

#pragma region DCL_IDirect3DDevice9Ex constructor/destructor

/**
* Constructor.
***/
DCL_IDirect3DDevice9Ex::DCL_IDirect3DDevice9Ex(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{}

/**
*
***/
DCL_IDirect3DDevice9Ex::~DCL_IDirect3DDevice9Ex(){}

#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::QueryInterface(IDirect3DDevice9Ex* pcThis, REFIID riid, void** ppvObj)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::QueryInterface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&riid;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppvObj;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_QueryInterface_Super(pcThis, riid, ppvObj);
	}
	else
		return D3D929_IDirect3DDevice9Ex_QueryInterface_Super(pcThis, riid, ppvObj);
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DDevice9Ex::AddRef(IDirect3DDevice9Ex* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::AddRef");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::AddRef]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::AddRef]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::AddRef]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::AddRef]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static ULONG nCount = (ULONG)*(ULONG*)pvRet;
	//		return nCount;
	//	}
	//	else
	//		return ;
	//}
	//else
	//	return ;


	// set reference counter in the transfer site class
	int iCount = D3D929_IDirect3DDevice9Ex_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DDevice9Ex::Release(IDirect3DDevice9Ex* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::Release");


	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::Release]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::Release]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::Release]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IUNKNOWN::Release]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static ULONG nCount = (ULONG)*(ULONG*)pvRet;
	//		return nCount;
	//	}
	//	else
	//		return ;
	//}
	//else
	//	return ;

	// set reference counter in the transfer site class
	int iCount = D3D929_IDirect3DDevice9Ex_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDirect3DDevice9 methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::TestCooperativeLevel(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::TestCooperativeLevel");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_TestCooperativeLevel_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_TestCooperativeLevel_Super(pcThis);
}

/** 
* 
***/
UINT WINAPI DCL_IDirect3DDevice9Ex::GetAvailableTextureMem(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetAvailableTextureMem");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static UINT ui = (UINT)*(UINT*)pvRet;
			return ui;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetAvailableTextureMem_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetAvailableTextureMem_Super(pcThis);
}

/**
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::EvictManagedResources(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::EvictManagedResources"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EvictManagedResources]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_EvictManagedResources_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_EvictManagedResources_Super(pcThis);
}

/**
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetDirect3D(IDirect3DDevice9Ex* pcThis, IDirect3D9** ppD3D9)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetDirect3D"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDirect3D]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppD3D9;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDirect3D]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetDirect3D_Super(pcThis, ppD3D9);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetDirect3D_Super(pcThis, ppD3D9);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetDeviceCaps(IDirect3DDevice9Ex* pcThis, D3DCAPS9* pCaps)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetDeviceCaps"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pCaps;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetDeviceCaps_Super(pcThis, pCaps);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetDeviceCaps_Super(pcThis, pCaps);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetDisplayMode(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,D3DDISPLAYMODE* pMode)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetDisplayMode"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMode;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetDisplayMode_Super(pcThis, iSwapChain, pMode);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetDisplayMode_Super(pcThis, iSwapChain, pMode);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetCreationParameters(IDirect3DDevice9Ex* pcThis, D3DDEVICE_CREATION_PARAMETERS *pParameters)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetCreationParameters"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCreationParameters]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pParameters;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetCreationParameters_Super(pcThis, pParameters);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetCreationParameters_Super(pcThis, pParameters);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetCursorProperties(IDirect3DDevice9Ex* pcThis, UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetCursorProperties"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[0]->m_pOutput = (void*)&XHotSpot;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[1]->m_pOutput = (void*)&YHotSpot;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pCursorBitmap;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetCursorProperties_Super(pcThis, XHotSpot, YHotSpot, pCursorBitmap);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetCursorProperties_Super(pcThis, XHotSpot, YHotSpot, pCursorBitmap);
}

/** 
* 
***/
void WINAPI DCL_IDirect3DDevice9Ex::SetCursorPosition(IDirect3DDevice9Ex* pcThis, int X,int Y,DWORD Flags)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetCursorPosition"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[0]->m_pOutput = (void*)&X;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Y;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Flags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else
			return D3D929_IDirect3DDevice9Ex_SetCursorPosition_Super(pcThis, X, Y, Flags);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetCursorPosition_Super(pcThis, X, Y, Flags);
}

/** 
* 
***/
BOOL    WINAPI DCL_IDirect3DDevice9Ex::ShowCursor(IDirect3DDevice9Ex* pcThis, BOOL bShow)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ShowCursor"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ShowCursor]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bShow;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ShowCursor]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static BOOL b = (BOOL)*(BOOL*)pvRet;
			return b;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ShowCursor_Super(pcThis, bShow);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ShowCursor_Super(pcThis, bShow);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateAdditionalSwapChain(IDirect3DDevice9Ex* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateAdditionalSwapChain"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPresentationParameters;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSwapChain;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateAdditionalSwapChain_Super(pcThis, pPresentationParameters, pSwapChain);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateAdditionalSwapChain_Super(pcThis, pPresentationParameters, pSwapChain);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetSwapChain(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetSwapChain"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSwapChain;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetSwapChain_Super(pcThis, iSwapChain, pSwapChain);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetSwapChain_Super(pcThis, iSwapChain, pSwapChain);
}

/** 
* 
***/
UINT WINAPI DCL_IDirect3DDevice9Ex::GetNumberOfSwapChains(IDirect3DDevice9Ex* pcThis)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetNumberOfSwapChains"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static UINT ui = (UINT)*(UINT*)pvRet;
			return ui;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetNumberOfSwapChains_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetNumberOfSwapChains_Super(pcThis);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::Reset(IDirect3DDevice9Ex* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::Reset"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Reset]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Reset]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Reset]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPresentationParameters;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Reset]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Reset]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_Reset_Super(pcThis, pPresentationParameters);
	}
	else
		return D3D929_IDirect3DDevice9Ex_Reset_Super(pcThis, pPresentationParameters);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::Present(IDirect3DDevice9Ex* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::Present");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[2]->m_pOutput = (void*)&hDestWindowOverride;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDirtyRegion;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Present]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
	else
		return D3D929_IDirect3DDevice9Ex_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetBackBuffer(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetBackBuffer"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&iBackBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&ppBackBuffer;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetBackBuffer_Super(pcThis, iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetBackBuffer_Super(pcThis, iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetRasterStatus(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetRasterStatus"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRasterStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetRasterStatus_Super(pcThis, iSwapChain, pRasterStatus);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetRasterStatus_Super(pcThis, iSwapChain, pRasterStatus);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetDialogBoxMode(IDirect3DDevice9Ex* pcThis, BOOL bEnableDialogs)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetDialogBoxMode"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bEnableDialogs;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetDialogBoxMode_Super(pcThis, bEnableDialogs);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetDialogBoxMode_Super(pcThis, bEnableDialogs);
}

/** 
* 
***/
void WINAPI DCL_IDirect3DDevice9Ex::SetGammaRamp(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetGammaRamp"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Flags;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pRamp;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else
			return D3D929_IDirect3DDevice9Ex_SetGammaRamp_Super(pcThis, iSwapChain, Flags, pRamp);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetGammaRamp_Super(pcThis, iSwapChain, Flags, pRamp);
}

/** 
* 
***/
void WINAPI DCL_IDirect3DDevice9Ex::GetGammaRamp(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,D3DGAMMARAMP* pRamp)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetGammaRamp"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRamp;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else
			return D3D929_IDirect3DDevice9Ex_GetGammaRamp_Super(pcThis, iSwapChain, pRamp);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetGammaRamp_Super(pcThis, iSwapChain, pRamp);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateTexture(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateTexture"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateTexture_Super(pcThis, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateTexture_Super(pcThis, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateVolumeTexture(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateVolumeTexture"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Depth;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[7]->m_pOutput = (void*)&ppVolumeTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[8]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateVolumeTexture_Super(pcThis, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateVolumeTexture_Super(pcThis, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateCubeTexture(IDirect3DDevice9Ex* pcThis, UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateCubeTexture"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&EdgeLength;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&ppCubeTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateCubeTexture_Super(pcThis, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateCubeTexture_Super(pcThis, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateVertexBuffer(IDirect3DDevice9Ex* pcThis, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateVertexBuffer"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Length;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&FVF;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppVertexBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateVertexBuffer_Super(pcThis, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateVertexBuffer_Super(pcThis, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateIndexBuffer(IDirect3DDevice9Ex* pcThis, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateIndexBuffer"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Length;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppIndexBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateIndexBuffer_Super(pcThis, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateIndexBuffer_Super(pcThis, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateRenderTarget(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateRenderTarget"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Lockable;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateRenderTarget_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateRenderTarget_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateDepthStencilSurface(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateDepthStencilSurface"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Discard;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurface_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurface_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::UpdateSurface(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::UpdateSurface"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pDestinationSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestPoint;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_UpdateSurface_Super(pcThis, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
	}
	else
		return D3D929_IDirect3DDevice9Ex_UpdateSurface_Super(pcThis, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::UpdateTexture(IDirect3DDevice9Ex* pcThis, IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::UpdateTexture"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestinationTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_UpdateTexture_Super(pcThis, pSourceTexture, pDestinationTexture);
	}
	else
		return D3D929_IDirect3DDevice9Ex_UpdateTexture_Super(pcThis, pSourceTexture, pDestinationTexture);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetRenderTargetData(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetRenderTargetData"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRenderTarget;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetRenderTargetData_Super(pcThis, pRenderTarget, pDestSurface);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetRenderTargetData_Super(pcThis, pRenderTarget, pDestSurface);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetFrontBufferData(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetFrontBufferData"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetFrontBufferData_Super(pcThis, iSwapChain, pDestSurface);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetFrontBufferData_Super(pcThis, iSwapChain, pDestSurface);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::StretchRect(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::StretchRect"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pDestSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Filter;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::StretchRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_StretchRect_Super(pcThis, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
	}
	else
		return D3D929_IDirect3DDevice9Ex_StretchRect_Super(pcThis, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

/**
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::ColorFill(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ColorFill"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[2]->m_pOutput = (void*)&color;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ColorFill]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ColorFill_Super(pcThis, pSurface, pRect, color);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ColorFill_Super(pcThis, pSurface, pRect, color);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateOffscreenPlainSurface(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateOffscreenPlainSurface"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Super(pcThis, Width, Height, Format, Pool, ppSurface, pSharedHandle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Super(pcThis, Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetRenderTarget(IDirect3DDevice9Ex* pcThis, DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetRenderTarget"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&RenderTargetIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRenderTarget;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetRenderTarget_Super(pcThis, RenderTargetIndex, pRenderTarget);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetRenderTarget_Super(pcThis, RenderTargetIndex, pRenderTarget);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetRenderTarget(IDirect3DDevice9Ex* pcThis, DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{  
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetRenderTarget"); 

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&RenderTargetIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppRenderTarget;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetRenderTarget_Super(pcThis, RenderTargetIndex, ppRenderTarget);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetRenderTarget_Super(pcThis, RenderTargetIndex, ppRenderTarget);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetDepthStencilSurface(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pNewZStencil)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetDepthStencilSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pNewZStencil;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetDepthStencilSurface_Super(pcThis, pNewZStencil);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetDepthStencilSurface_Super(pcThis, pNewZStencil);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetDepthStencilSurface(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9** ppZStencilSurface)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetDepthStencilSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppZStencilSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetDepthStencilSurface_Super(pcThis, ppZStencilSurface);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetDepthStencilSurface_Super(pcThis, ppZStencilSurface);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::BeginScene(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::BeginScene");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginScene]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginScene]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginScene]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginScene]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_BeginScene_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_BeginScene_Super(pcThis);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::EndScene(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::EndScene");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndScene]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndScene]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndScene]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndScene]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_EndScene_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_EndScene_Super(pcThis);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::Clear(IDirect3DDevice9Ex* pcThis, DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::Clear");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Count;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRects;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Flags;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Color;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Z;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Stencil;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::Clear]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_Clear_Super(pcThis, Count, pRects, Flags, Color, Z, Stencil);
	}
	else
		return D3D929_IDirect3DDevice9Ex_Clear_Super(pcThis, Count, pRects, Flags, Color, Z, Stencil);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetTransform(IDirect3DDevice9Ex* pcThis, D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetTransform_Super(pcThis, State, pMatrix);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetTransform_Super(pcThis, State, pMatrix);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetTransform(IDirect3DDevice9Ex* pcThis, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetTransform_Super(pcThis, State, pMatrix);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetTransform_Super(pcThis, State, pMatrix);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::MultiplyTransform(IDirect3DDevice9Ex* pcThis, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::MultiplyTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_MultiplyTransform_Super(pcThis, State, pMatrix);
	}
	else
		return D3D929_IDirect3DDevice9Ex_MultiplyTransform_Super(pcThis, State, pMatrix);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetViewport(IDirect3DDevice9Ex* pcThis, CONST D3DVIEWPORT9* pViewport)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetViewport");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetViewport]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetViewport]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetViewport]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pViewport;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetViewport]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetViewport]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetViewport_Super(pcThis, pViewport);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetViewport_Super(pcThis, pViewport);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetViewport(IDirect3DDevice9Ex* pcThis, D3DVIEWPORT9* pViewport)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetViewport");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetViewport]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetViewport]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetViewport]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pViewport;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetViewport]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetViewport]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetViewport_Super(pcThis, pViewport);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetViewport_Super(pcThis, pViewport);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetMaterial(IDirect3DDevice9Ex* pcThis, CONST D3DMATERIAL9* pMaterial)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetMaterial");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetMaterial]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pMaterial;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetMaterial]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetMaterial_Super(pcThis, pMaterial);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetMaterial_Super(pcThis, pMaterial);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetMaterial(IDirect3DDevice9Ex* pcThis, D3DMATERIAL9* pMaterial)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetMaterial");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetMaterial]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pMaterial;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetMaterial]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetMaterial_Super(pcThis, pMaterial);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetMaterial_Super(pcThis, pMaterial);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetLight(IDirect3DDevice9Ex* pcThis, DWORD Index, CONST D3DLIGHT9* pLight)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetLight");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pLight;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetLight]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetLight_Super(pcThis, Index, pLight);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetLight_Super(pcThis, Index, pLight);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetLight(IDirect3DDevice9Ex* pcThis, DWORD Index, D3DLIGHT9* pLight)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetLight");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pLight;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLight]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetLight_Super(pcThis, Index, pLight);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetLight_Super(pcThis, Index, pLight);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::LightEnable(IDirect3DDevice9Ex* pcThis, DWORD Index, BOOL Enable)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::LightEnable");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Enable;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::LightEnable]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_LightEnable_Super(pcThis, Index, Enable);
	}
	else
		return D3D929_IDirect3DDevice9Ex_LightEnable_Super(pcThis, Index, Enable);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetLightEnable(IDirect3DDevice9Ex* pcThis, DWORD Index, BOOL* pEnable)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetLightEnable");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEnable;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetLightEnable_Super(pcThis, Index, pEnable);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetLightEnable_Super(pcThis, Index, pEnable);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetClipPlane(IDirect3DDevice9Ex* pcThis, DWORD Index, CONST float* pPlane)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetClipPlane");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pPlane;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetClipPlane_Super(pcThis, Index, pPlane);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetClipPlane_Super(pcThis, Index, pPlane);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetClipPlane(IDirect3DDevice9Ex* pcThis, DWORD Index, float* pPlane)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetClipPlane");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pPlane;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetClipPlane_Super(pcThis, Index, pPlane);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetClipPlane_Super(pcThis, Index, pPlane);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetRenderState(IDirect3DDevice9Ex* pcThis, D3DRENDERSTATETYPE State, DWORD Value)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetRenderState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetRenderState_Super(pcThis, State, Value);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetRenderState_Super(pcThis, State, Value);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetRenderState(IDirect3DDevice9Ex* pcThis, D3DRENDERSTATETYPE State, DWORD* pValue)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetRenderState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetRenderState_Super(pcThis, State, pValue);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetRenderState_Super(pcThis, State, pValue);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateStateBlock(IDirect3DDevice9Ex* pcThis, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppSB;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateStateBlock_Super(pcThis, Type, ppSB);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateStateBlock_Super(pcThis, Type, ppSB);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::BeginStateBlock(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::BeginStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_BeginStateBlock_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_BeginStateBlock_Super(pcThis);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::EndStateBlock(IDirect3DDevice9Ex* pcThis, IDirect3DStateBlock9** ppSB)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::EndStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppSB;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_EndStateBlock_Super(pcThis, ppSB);
	}
	else
		return D3D929_IDirect3DDevice9Ex_EndStateBlock_Super(pcThis, ppSB);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetClipStatus(IDirect3DDevice9Ex* pcThis, CONST D3DCLIPSTATUS9* pClipStatus)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetClipStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pClipStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetClipStatus_Super(pcThis, pClipStatus);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetClipStatus_Super(pcThis, pClipStatus);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetClipStatus(IDirect3DDevice9Ex* pcThis, D3DCLIPSTATUS9* pClipStatus)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetClipStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pClipStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetClipStatus_Super(pcThis, pClipStatus);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetClipStatus_Super(pcThis, pClipStatus);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetTexture(IDirect3DDevice9Ex* pcThis, DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetTexture_Super(pcThis, Stage, ppTexture);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetTexture_Super(pcThis, Stage, ppTexture);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetTexture(IDirect3DDevice9Ex* pcThis, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetTexture_Super(pcThis, Stage, pTexture);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetTexture_Super(pcThis, Stage, pTexture);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetTextureStageState(IDirect3DDevice9Ex* pcThis, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetTextureStageState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetTextureStageState_Super(pcThis, Stage, Type, pValue);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetTextureStageState_Super(pcThis, Stage, Type, pValue);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetTextureStageState(IDirect3DDevice9Ex* pcThis, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetTextureStageState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetTextureStageState_Super(pcThis, Stage, Type, Value);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetTextureStageState_Super(pcThis, Stage, Type, Value);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetSamplerState(IDirect3DDevice9Ex* pcThis, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetSamplerState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Sampler;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetSamplerState_Super(pcThis, Sampler, Type, pValue);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetSamplerState_Super(pcThis, Sampler, Type, pValue);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetSamplerState(IDirect3DDevice9Ex* pcThis, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetSamplerState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Sampler;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetSamplerState_Super(pcThis, Sampler, Type, Value);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetSamplerState_Super(pcThis, Sampler, Type, Value);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::ValidateDevice(IDirect3DDevice9Ex* pcThis, DWORD* pNumPasses)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ValidateDevice");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ValidateDevice]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pNumPasses;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ValidateDevice]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ValidateDevice_Super(pcThis, pNumPasses);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ValidateDevice_Super(pcThis, pNumPasses);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetPaletteEntries(IDirect3DDevice9Ex* pcThis, UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetPaletteEntries");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEntries;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetPaletteEntries(IDirect3DDevice9Ex* pcThis, UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetPaletteEntries");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEntries;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetCurrentTexturePalette(IDirect3DDevice9Ex* pcThis, UINT PaletteNumber)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetCurrentTexturePalette");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetCurrentTexturePalette_Super(pcThis, PaletteNumber);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetCurrentTexturePalette_Super(pcThis, PaletteNumber);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetCurrentTexturePalette(IDirect3DDevice9Ex* pcThis, UINT *PaletteNumber)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetCurrentTexturePalette");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetCurrentTexturePalette_Super(pcThis, PaletteNumber);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetCurrentTexturePalette_Super(pcThis, PaletteNumber);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetScissorRect(IDirect3DDevice9Ex* pcThis, CONST RECT* pRect)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetScissorRect");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetScissorRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRect;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetScissorRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetScissorRect_Super(pcThis, pRect);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetScissorRect_Super(pcThis, pRect);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetScissorRect(IDirect3DDevice9Ex* pcThis, RECT* pRect)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetScissorRect");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetScissorRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRect;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetScissorRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetScissorRect_Super(pcThis, pRect);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetScissorRect_Super(pcThis, pRect);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetSoftwareVertexProcessing(IDirect3DDevice9Ex* pcThis, BOOL bSoftware)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetSoftwareVertexProcessing");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bSoftware;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Super(pcThis, bSoftware);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Super(pcThis, bSoftware);
} 

/** 
* 
***/
BOOL    WINAPI DCL_IDirect3DDevice9Ex::GetSoftwareVertexProcessing(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetSoftwareVertexProcessing");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static BOOL b = (BOOL)*(BOOL*)pvRet;
			return b;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Super(pcThis);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetNPatchMode(IDirect3DDevice9Ex* pcThis, float nSegments)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetNPatchMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetNPatchMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&nSegments;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetNPatchMode_Super(pcThis, nSegments);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetNPatchMode_Super(pcThis, nSegments);
} 

/** 
* 
***/
float   WINAPI DCL_IDirect3DDevice9Ex::GetNPatchMode(IDirect3DDevice9Ex* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetNPatchMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNPatchMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static float hr = (float)*(float*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetNPatchMode_Super(pcThis);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetNPatchMode_Super(pcThis);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawPrimitive(IDirect3DDevice9Ex* pcThis, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawPrimitive");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[1]->m_pOutput = (void*)&StartVertex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[2]->m_pOutput = (void*)&PrimitiveCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawPrimitive_Super(pcThis, PrimitiveType, StartVertex, PrimitiveCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawPrimitive_Super(pcThis, PrimitiveType, StartVertex, PrimitiveCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawIndexedPrimitive(IDirect3DDevice9Ex* pcThis, D3DPRIMITIVETYPE PrimitiveType ,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawIndexedPrimitive");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[1]->m_pOutput = (void*)&BaseVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[2]->m_pOutput = (void*)&MinVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[3]->m_pOutput = (void*)&NumVertices;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[4]->m_pOutput = (void*)&startIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[5]->m_pOutput = (void*)&primCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitive_Super(pcThis, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitive_Super(pcThis, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawPrimitiveUP(IDirect3DDevice9Ex* pcThis, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawPrimitiveUP");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[1]->m_pOutput = (void*)&PrimitiveCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pVertexStreamZeroData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[3]->m_pOutput = (void*)&VertexStreamZeroStride;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawPrimitiveUP_Super(pcThis, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawPrimitiveUP_Super(pcThis, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawIndexedPrimitiveUP(IDirect3DDevice9Ex* pcThis, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawIndexedPrimitiveUP");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[1]->m_pOutput = (void*)&MinVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[2]->m_pOutput = (void*)&NumVertices;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[3]->m_pOutput = (void*)&PrimitiveCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[4]->m_pOutput = (void*)&pIndexData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[5]->m_pOutput = (void*)&IndexDataFormat;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[6]->m_pOutput = (void*)&pVertexStreamZeroData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[7]->m_pOutput = (void*)&VertexStreamZeroStride;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Super(pcThis, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Super(pcThis, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::ProcessVertices(IDirect3DDevice9Ex* pcThis, UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ProcessVertices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&SrcStartIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[1]->m_pOutput = (void*)&DestIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[2]->m_pOutput = (void*)&VertexCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[4]->m_pOutput = (void*)&pVertexDecl;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Flags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ProcessVertices_Super(pcThis, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ProcessVertices_Super(pcThis, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateVertexDeclaration(IDirect3DDevice9Ex* pcThis, CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pVertexElements;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateVertexDeclaration_Super(pcThis, pVertexElements, ppDecl);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateVertexDeclaration_Super(pcThis, pVertexElements, ppDecl);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetVertexDeclaration(IDirect3DDevice9Ex* pcThis, IDirect3DVertexDeclaration9* pDecl)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetVertexDeclaration_Super(pcThis, pDecl);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetVertexDeclaration_Super(pcThis, pDecl);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetVertexDeclaration(IDirect3DDevice9Ex* pcThis, IDirect3DVertexDeclaration9** ppDecl)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetVertexDeclaration_Super(pcThis, ppDecl);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetVertexDeclaration_Super(pcThis, ppDecl);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetFVF(IDirect3DDevice9Ex* pcThis, DWORD FVF)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetFVF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetFVF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetFVF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetFVF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&FVF;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetFVF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetFVF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetFVF_Super(pcThis, FVF);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetFVF_Super(pcThis, FVF);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetFVF(IDirect3DDevice9Ex* pcThis, DWORD* pFVF)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetFVF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFVF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFVF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFVF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFVF;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFVF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetFVF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetFVF_Super(pcThis, pFVF);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetFVF_Super(pcThis, pFVF);
} 

/** 
* Vertex Shader detour method, creates data sheet entry for new shader if workspace is present.
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateVertexShader(IDirect3DDevice9Ex* pcThis, CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	static HRESULT hr;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateVertexShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFunction;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			hr = (HRESULT)*(HRESULT*)pvRet;
		else
			hr = D3D929_IDirect3DDevice9Ex_CreateVertexShader_Super(pcThis, pFunction, ppShader);
	}
	else
		hr = D3D929_IDirect3DDevice9Ex_CreateVertexShader_Super(pcThis, pFunction, ppShader);

	if (!m_pcTransferSite->m_bIsWorkingArea) return hr;

	if (SUCCEEDED(hr) && ((*ppShader))) 
	{
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		BYTE* pData = NULL;
		UINT dwSizeOfData;
		UINT dwHash;

		// get shader function
		(*ppShader)->GetFunction(NULL, &dwSizeOfData);
		pData = new BYTE[dwSizeOfData];
		(*ppShader)->GetFunction(pData, &dwSizeOfData);

		if (m_pcTransferSite->m_bCreateShaderHash)
		{
			// get hash code
			dwHash = GetHashCode(pData, dwSizeOfData);

			// return if hash code already present
			if (m_pcTransferSite->VertexShaderPresent(dwHash)) return hr;
		}
		else
			// return if shader length already present
			if (m_pcTransferSite->VertexShaderPresent(dwSizeOfData)) return hr;

		// get constants
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
		if(pConstantTable == NULL) 
			return hr;

		// get constant table description
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		// create constant description array, count possible entries
		D3DXCONSTANT_DESC* aConstDesc = new D3DXCONSTANT_DESC[pDesc.Constants];
		UINT dwEntryNumber = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);
			dwEntryNumber += dwCount;
		}

		// loop again, create entry
		wchar_t* szTitle = new wchar_t[64];
		if (m_pcTransferSite->m_bCreateShaderHash)
			wsprintf(szTitle, L"%I32u", dwHash);
		else
			wsprintf(szTitle, L"%I32u", dwSizeOfData);

		LPCWSTR *paSubEntries = new LPCWSTR[dwEntryNumber];
		UINT dwIndex = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);

			// add all constants to shader entry vector
			for(UINT j = 0; j < dwCount; j++)
			{
				// add constant name to sub entry array
				std::string szName(aConstDesc[j].Name);
				std::wstring szNameW(szName.begin(), szName.end());
				paSubEntries[dwIndex] = new wchar_t[szNameW.size()+1];
				CopyMemory((void*)&paSubEntries[dwIndex][0], szNameW.c_str(), (szNameW.size()+1)*sizeof(wchar_t));
				dwIndex++;
			}
			// if (aConstDesc) delete [] aConstDesc; - how to release ??
		}

		// register the shader entry to the data sheet via the transfer site class
		if (m_pcTransferSite->m_bCreateShaderHash)
			m_pcTransferSite->RegisterDataSheetVertexShader(szTitle, paSubEntries, dwEntryNumber, dwHash);
		else
			m_pcTransferSite->RegisterDataSheetVertexShader(szTitle, paSubEntries, dwEntryNumber, dwSizeOfData);

		if (pConstantTable) { pConstantTable->Release(); pConstantTable = NULL; } 
		if (pData) delete[] pData;
	}

	return hr;
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetVertexShader(IDirect3DDevice9Ex* pcThis, IDirect3DVertexShader9* pShader)
{
	static HRESULT hr;

	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetVertexShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			// get return value.. MUST be STATIC !
				hr = (HRESULT)*(HRESULT*)pvRet;
		else
			hr = D3D929_IDirect3DDevice9Ex_SetVertexShader_Super(pcThis, pShader);
	}
	else
		hr = D3D929_IDirect3DDevice9Ex_SetVertexShader_Super(pcThis, pShader);

	if ((!m_pcTransferSite->m_bIsWorkingArea) || (!m_pcTransferSite->m_bGatherVShaderBySet)) return hr;

	if (SUCCEEDED(hr) && (pShader)) 
	{
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		BYTE* pData = NULL;
		UINT dwSizeOfData;
		UINT dwHash;

		// get shader function
		pShader->GetFunction(NULL, &dwSizeOfData);
		pData = new BYTE[dwSizeOfData];
		pShader->GetFunction(pData, &dwSizeOfData);

		if (m_pcTransferSite->m_bCreateShaderHash)
		{
			// get hash code
			dwHash = GetHashCode(pData, dwSizeOfData);

			// return if hash code already present
			if (m_pcTransferSite->VertexShaderPresent(dwHash)) return hr;
		}
		else
			// return if shader length already present
			if (m_pcTransferSite->VertexShaderPresent(dwSizeOfData)) return hr;

		// get constants
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
		if(pConstantTable == NULL) 
			return hr;

		// get constant table description
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		// create constant description array, count possible entries
		D3DXCONSTANT_DESC* aConstDesc = new D3DXCONSTANT_DESC[pDesc.Constants];
		UINT dwEntryNumber = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);
			dwEntryNumber += dwCount;
		}

		// loop again, create entry
		wchar_t* szTitle = new wchar_t[64];
		if (m_pcTransferSite->m_bCreateShaderHash)
			wsprintf(szTitle, L"%I32u", dwHash);
		else
			wsprintf(szTitle, L"%I32u", dwSizeOfData);

		LPCWSTR *paSubEntries = new LPCWSTR[dwEntryNumber];
		UINT dwIndex = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);

			// add all constants to shader entry vector
			for(UINT j = 0; j < dwCount; j++)
			{
				// add constant name to sub entry array
				std::string szName(aConstDesc[j].Name);
				std::wstring szNameW(szName.begin(), szName.end());
				paSubEntries[dwIndex] = new wchar_t[szNameW.size()+1];
				CopyMemory((void*)&paSubEntries[dwIndex][0], szNameW.c_str(), (szNameW.size()+1)*sizeof(wchar_t));
				dwIndex++;
			}
			// if (aConstDesc) delete [] aConstDesc; - how to release ??
		}

		// register the shader entry to the data sheet via the transfer site class
		if (m_pcTransferSite->m_bCreateShaderHash)
			m_pcTransferSite->RegisterDataSheetVertexShader(szTitle, paSubEntries, dwEntryNumber, dwHash);
		else
			m_pcTransferSite->RegisterDataSheetVertexShader(szTitle, paSubEntries, dwEntryNumber, dwSizeOfData);

		if (pConstantTable) { pConstantTable->Release(); pConstantTable = NULL; } 
		if (pData) delete[] pData;
	}

	return hr;
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetVertexShader(IDirect3DDevice9Ex* pcThis, IDirect3DVertexShader9** ppShader)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetVertexShader");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppShader;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_GetVertexShader_Super(pcThis, ppShader);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_GetVertexShader_Super(pcThis, ppShader);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetVertexShaderConstantF(IDirect3DDevice9Ex* pcThis, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetVertexShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetVertexShaderConstantF(IDirect3DDevice9Ex* pcThis, UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetVertexShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetVertexShaderConstantI(IDirect3DDevice9Ex* pcThis, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetVertexShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetVertexShaderConstantI(IDirect3DDevice9Ex* pcThis, UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetVertexShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetVertexShaderConstantB(IDirect3DDevice9Ex* pcThis, UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetVertexShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetVertexShaderConstantB(IDirect3DDevice9Ex* pcThis, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetVertexShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetStreamSource(IDirect3DDevice9Ex* pcThis, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	//OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetStreamSource");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pStreamData;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[2]->m_pOutput = (void*)&OffsetInBytes;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Stride;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_SetStreamSource_Super(pcThis, StreamNumber, pStreamData, OffsetInBytes, Stride);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_SetStreamSource_Super(pcThis, StreamNumber, pStreamData, OffsetInBytes, Stride);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetStreamSource(IDirect3DDevice9Ex* pcThis, UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
	// OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetStreamSource");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppStreamData;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pOffsetInBytes;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pStride;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_GetStreamSource_Super(pcThis, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_GetStreamSource_Super(pcThis, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetStreamSourceFreq(IDirect3DDevice9Ex* pcThis, UINT StreamNumber,UINT Setting)
{
	//OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetStreamSourceFreq");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Setting;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_SetStreamSourceFreq_Super(pcThis, StreamNumber, Setting);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_SetStreamSourceFreq_Super(pcThis, StreamNumber, Setting);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetStreamSourceFreq(IDirect3DDevice9Ex* pcThis, UINT StreamNumber, UINT* pSetting)
{
	//OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetStreamSourceFreq");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSetting;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_GetStreamSourceFreq_Super(pcThis, StreamNumber, pSetting);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_GetStreamSourceFreq_Super(pcThis, StreamNumber, pSetting);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetIndices(IDirect3DDevice9Ex* pcThis, IDirect3DIndexBuffer9* pIndexData)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetIndices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetIndices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetIndices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetIndices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pIndexData;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetIndices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetIndices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetIndices_Super(pcThis, pIndexData);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetIndices_Super(pcThis, pIndexData);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetIndices(IDirect3DDevice9Ex* pcThis, IDirect3DIndexBuffer9** ppIndexData)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetIndices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetIndices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetIndices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetIndices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppIndexData;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetIndices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetIndices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetIndices_Super(pcThis, ppIndexData);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetIndices_Super(pcThis, ppIndexData);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreatePixelShader(IDirect3DDevice9Ex* pcThis, CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	static HRESULT hr;

	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreatePixelShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFunction;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			// get return value.. MUST be STATIC !
				hr = (HRESULT)*(HRESULT*)pvRet;
		else
			hr = D3D929_IDirect3DDevice9Ex_CreatePixelShader_Super(pcThis, pFunction, ppShader);
	}
	else
		hr = D3D929_IDirect3DDevice9Ex_CreatePixelShader_Super(pcThis, pFunction, ppShader);

	if (!m_pcTransferSite->m_bIsWorkingArea) return hr;

	if (SUCCEEDED(hr) && ((*ppShader))) 
	{
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		BYTE* pData = NULL;
		UINT dwSizeOfData;
		UINT dwHash;

		// get shader function
		(*ppShader)->GetFunction(NULL, &dwSizeOfData);
		pData = new BYTE[dwSizeOfData];
		(*ppShader)->GetFunction(pData, &dwSizeOfData);

		if (m_pcTransferSite->m_bCreateShaderHash)
		{
			// get hash code
			dwHash = GetHashCode(pData, dwSizeOfData);

			// return if hash code already present
			if (m_pcTransferSite->PixelShaderPresent(dwHash)) return hr;
		}
		else
			// return if shader length already present
			if (m_pcTransferSite->PixelShaderPresent(dwSizeOfData)) return hr;

		// get constants
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
		if(pConstantTable == NULL) 
			return hr;

		// get constant table description
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		// create constant description array, count possible entries
		D3DXCONSTANT_DESC* aConstDesc = new D3DXCONSTANT_DESC[pDesc.Constants];
		UINT dwEntryNumber = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);
			dwEntryNumber += dwCount;
		}

		// loop again, create entry
		wchar_t* szTitle = new wchar_t[64];
		if (m_pcTransferSite->m_bCreateShaderHash)
			wsprintf(szTitle, L"%I32u", dwHash);
		else
			wsprintf(szTitle, L"%I32u", dwSizeOfData);

		LPCWSTR *paSubEntries = new LPCWSTR[dwEntryNumber];
		UINT dwIndex = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);

			// add all constants to shader entry vector
			for(UINT j = 0; j < dwCount; j++)
			{
				// add constant name to sub entry array
				std::string szName(aConstDesc[j].Name);
				std::wstring szNameW(szName.begin(), szName.end());
				paSubEntries[dwIndex] = new wchar_t[szNameW.size()+1];
				CopyMemory((void*)&paSubEntries[dwIndex][0], szNameW.c_str(), (szNameW.size()+1)*sizeof(wchar_t));
				dwIndex++;
			}
			// if (aConstDesc) delete [] aConstDesc; - how to release ??
		}

		// register the shader entry to the data sheet via the transfer site class
		if (m_pcTransferSite->m_bCreateShaderHash)
			m_pcTransferSite->RegisterDataSheetPixelShader(szTitle, paSubEntries, dwEntryNumber, dwHash);
		else
			m_pcTransferSite->RegisterDataSheetPixelShader(szTitle, paSubEntries, dwEntryNumber, dwSizeOfData);

		if (pConstantTable) { pConstantTable->Release(); pConstantTable = NULL; } 
		if (pData) delete[] pData;
	}

	return hr;
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetPixelShader(IDirect3DDevice9Ex* pcThis, IDirect3DPixelShader9* pShader)
{
	static HRESULT hr;

	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetPixelShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			// get return value.. MUST be STATIC !
				hr = (HRESULT)*(HRESULT*)pvRet;
		else
			hr = D3D929_IDirect3DDevice9Ex_SetPixelShader_Super(pcThis, pShader);
	}
	else
		hr = D3D929_IDirect3DDevice9Ex_SetPixelShader_Super(pcThis, pShader);

	if ((!m_pcTransferSite->m_bIsWorkingArea) || (!m_pcTransferSite->m_bGatherVShaderBySet)) return hr;

	if (SUCCEEDED(hr) && (pShader)) 
	{
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		BYTE* pData = NULL;
		UINT dwSizeOfData;
		UINT dwHash;

		// get shader function
		pShader->GetFunction(NULL, &dwSizeOfData);
		pData = new BYTE[dwSizeOfData];
		pShader->GetFunction(pData, &dwSizeOfData);

		if (m_pcTransferSite->m_bCreateShaderHash)
		{
			// get hash code
			dwHash = GetHashCode(pData, dwSizeOfData);

			// return if hash code already present
			if (m_pcTransferSite->PixelShaderPresent(dwHash)) return hr;
		}
		else
			// return if shader length already present
			if (m_pcTransferSite->PixelShaderPresent(dwSizeOfData)) return hr;

		// get constants
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
		if(pConstantTable == NULL) 
			return hr;

		// get constant table description
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		// create constant description array, count possible entries
		D3DXCONSTANT_DESC* aConstDesc = new D3DXCONSTANT_DESC[pDesc.Constants];
		UINT dwEntryNumber = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);
			dwEntryNumber += dwCount;
		}

		// loop again, create entry
		wchar_t* szTitle = new wchar_t[64];
		if (m_pcTransferSite->m_bCreateShaderHash)
			wsprintf(szTitle, L"%I32u", dwHash);
		else
			wsprintf(szTitle, L"%I32u", dwSizeOfData);

		LPCWSTR *paSubEntries = new LPCWSTR[dwEntryNumber];
		UINT dwIndex = 0;
		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT dwCount;
			pConstantTable->GetConstantDesc(handle, aConstDesc, &dwCount);

			// add all constants to shader entry vector
			for(UINT j = 0; j < dwCount; j++)
			{
				// add constant name to sub entry array
				std::string szName(aConstDesc[j].Name);
				std::wstring szNameW(szName.begin(), szName.end());
				paSubEntries[dwIndex] = new wchar_t[szNameW.size()+1];
				CopyMemory((void*)&paSubEntries[dwIndex][0], szNameW.c_str(), (szNameW.size()+1)*sizeof(wchar_t));
				dwIndex++;
			}
			// if (aConstDesc) delete [] aConstDesc; - how to release ??
		}

		// register the shader entry to the data sheet via the transfer site class
		if (m_pcTransferSite->m_bCreateShaderHash)
			m_pcTransferSite->RegisterDataSheetPixelShader(szTitle, paSubEntries, dwEntryNumber, dwHash);
		else
			m_pcTransferSite->RegisterDataSheetPixelShader(szTitle, paSubEntries, dwEntryNumber, dwSizeOfData);

		if (pConstantTable) { pConstantTable->Release(); pConstantTable = NULL; } 
		if (pData) delete[] pData;
	}

	return hr;
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetPixelShader(IDirect3DDevice9Ex* pcThis, IDirect3DPixelShader9** ppShader)
{
	//OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetPixelShader");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppShader;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return D3D929_IDirect3DDevice9Ex_GetPixelShader_Super(pcThis, ppShader);
	//}
	//else
	return D3D929_IDirect3DDevice9Ex_GetPixelShader_Super(pcThis, ppShader);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetPixelShaderConstantF(IDirect3DDevice9Ex* pcThis, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetPixelShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetPixelShaderConstantF(IDirect3DDevice9Ex* pcThis, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetPixelShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetPixelShaderConstantI(IDirect3DDevice9Ex* pcThis, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetPixelShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetPixelShaderConstantI(IDirect3DDevice9Ex* pcThis, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetPixelShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetPixelShaderConstantB(IDirect3DDevice9Ex* pcThis, UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetPixelShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetPixelShaderConstantB(IDirect3DDevice9Ex* pcThis, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetPixelShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawRectPatch(IDirect3DDevice9Ex* pcThis, UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawRectPatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pNumSegs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pRectPatchInfo;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawRectPatch_Super(pcThis, Handle, pNumSegs, pRectPatchInfo);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawRectPatch_Super(pcThis, Handle, pNumSegs, pRectPatchInfo);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DrawTriPatch(IDirect3DDevice9Ex* pcThis, UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DrawTriPatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pNumSegs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pTriPatchInfo;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DrawTriPatch_Super(pcThis, Handle, pNumSegs, pTriPatchInfo);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DrawTriPatch_Super(pcThis, Handle, pNumSegs, pTriPatchInfo);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::DeletePatch(IDirect3DDevice9Ex* pcThis, UINT Handle)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::DeletePatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DeletePatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DeletePatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_DeletePatch_Super(pcThis, Handle);
	}
	else
		return D3D929_IDirect3DDevice9Ex_DeletePatch_Super(pcThis, Handle);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateQuery(IDirect3DDevice9Ex* pcThis, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateQuery");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppQuery;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateQuery_Super(pcThis, Type, ppQuery);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateQuery_Super(pcThis, Type, ppQuery);
} 

#pragma endregion

#pragma region IDirect3DDevice9Ex methods

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetConvolutionMonoKernel(IDirect3DDevice9Ex* pcThis, UINT width,UINT height,float* rows,float* columns)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetConvolutionMonoKernel");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_paCommandersTemporary[0]->m_pOutput = (void*)&width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_paCommandersTemporary[1]->m_pOutput = (void*)&height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_paCommandersTemporary[2]->m_pOutput = (void*)&rows;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_paCommandersTemporary[3]->m_pOutput = (void*)&columns;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetConvolutionMonoKernel]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetConvolutionMonoKernel_Super(pcThis, width, height, rows, columns);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetConvolutionMonoKernel_Super(pcThis, width, height, rows, columns);
} 

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::ComposeRects(IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pSrc,IDirect3DSurface9* pDst,IDirect3DVertexBuffer9* pSrcRectDescs,UINT NumRects,IDirect3DVertexBuffer9* pDstRectDescs,D3DCOMPOSERECTSOP Operation,int Xoffset,int Yoffset)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ComposeRects");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSrc;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDst;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pSrcRectDescs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[3]->m_pOutput = (void*)&NumRects;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[4]->m_pOutput = (void*)&pDstRectDescs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Operation;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[6]->m_pOutput = (void*)&Xoffset;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_paCommandersTemporary[7]->m_pOutput = (void*)&Yoffset;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ComposeRects]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ComposeRects_Super(pcThis, pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ComposeRects_Super(pcThis, pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::PresentEx(IDirect3DDevice9Ex* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::PresentEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_paCommandersTemporary[2]->m_pOutput = (void*)&hDestWindowOverride;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDirtyRegion;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_paCommandersTemporary[4]->m_pOutput = (void*)&dwFlags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::PresentEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_PresentEx_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	}
	else
		return D3D929_IDirect3DDevice9Ex_PresentEx_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetGPUThreadPriority(IDirect3DDevice9Ex* pcThis, INT* pPriority)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetGPUThreadPriority");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetGPUThreadPriority]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetGPUThreadPriority]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetGPUThreadPriority]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPriority;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetGPUThreadPriority]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetGPUThreadPriority]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetGPUThreadPriority_Super(pcThis, pPriority);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetGPUThreadPriority_Super(pcThis, pPriority);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetGPUThreadPriority(IDirect3DDevice9Ex* pcThis, INT Priority)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetGPUThreadPriority");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetGPUThreadPriority]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetGPUThreadPriority]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetGPUThreadPriority]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Priority;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetGPUThreadPriority]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetGPUThreadPriority]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetGPUThreadPriority_Super(pcThis, Priority);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetGPUThreadPriority_Super(pcThis, Priority);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::WaitForVBlank(IDirect3DDevice9Ex* pcThis, UINT iSwapChain)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::WaitForVBlank");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::WaitForVBlank]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::WaitForVBlank]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::WaitForVBlank]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::WaitForVBlank]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::WaitForVBlank]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_WaitForVBlank_Super(pcThis, iSwapChain);
	}
	else
		return D3D929_IDirect3DDevice9Ex_WaitForVBlank_Super(pcThis, iSwapChain);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CheckResourceResidency(IDirect3DDevice9Ex* pcThis, IDirect3DResource9** pResourceArray,UINT32 NumResources)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CheckResourceResidency");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pResourceArray;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]->m_paCommandersTemporary[1]->m_pOutput = (void*)&NumResources;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckResourceResidency]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CheckResourceResidency_Super(pcThis, pResourceArray, NumResources);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CheckResourceResidency_Super(pcThis, pResourceArray, NumResources);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::SetMaximumFrameLatency(IDirect3DDevice9Ex* pcThis, UINT MaxLatency)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::SetMaximumFrameLatency");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetMaximumFrameLatency]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetMaximumFrameLatency]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetMaximumFrameLatency]->m_paCommandersTemporary[0]->m_pOutput = (void*)&MaxLatency;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetMaximumFrameLatency]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::SetMaximumFrameLatency]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_SetMaximumFrameLatency_Super(pcThis, MaxLatency);
	}
	else
		return D3D929_IDirect3DDevice9Ex_SetMaximumFrameLatency_Super(pcThis, MaxLatency);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetMaximumFrameLatency(IDirect3DDevice9Ex* pcThis, UINT* pMaxLatency)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetMaximumFrameLatency");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetMaximumFrameLatency]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetMaximumFrameLatency]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetMaximumFrameLatency]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pMaxLatency;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetMaximumFrameLatency]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetMaximumFrameLatency]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetMaximumFrameLatency_Super(pcThis, pMaxLatency);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetMaximumFrameLatency_Super(pcThis, pMaxLatency);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CheckDeviceState(IDirect3DDevice9Ex* pcThis, HWND hDestinationWindow)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CheckDeviceState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckDeviceState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckDeviceState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckDeviceState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&hDestinationWindow;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckDeviceState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CheckDeviceState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CheckDeviceState_Super(pcThis, hDestinationWindow);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CheckDeviceState_Super(pcThis, hDestinationWindow);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateRenderTargetEx(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateRenderTargetEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Lockable;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_paCommandersTemporary[8]->m_pOutput = (void*)&Usage;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateRenderTargetEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateRenderTargetEx_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateRenderTargetEx_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_paCommandersTemporary[6]->m_pOutput = (void*)&Usage;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateOffscreenPlainSurfaceEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Super(pcThis, Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Super(pcThis, Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::CreateDepthStencilSurfaceEx(IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::CreateDepthStencilSurfaceEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Discard;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_paCommandersTemporary[8]->m_pOutput = (void*)&Usage;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::CreateDepthStencilSurfaceEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
	}
	else
		return D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::ResetEx(IDirect3DDevice9Ex* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX *pFullscreenDisplayMode)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::ResetEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPresentationParameters;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pFullscreenDisplayMode;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::ResetEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_ResetEx_Super(pcThis, pPresentationParameters, pFullscreenDisplayMode);
	}
	else
		return D3D929_IDirect3DDevice9Ex_ResetEx_Super(pcThis, pPresentationParameters, pFullscreenDisplayMode);
}

/** 
* 
***/
HRESULT WINAPI DCL_IDirect3DDevice9Ex::GetDisplayModeEx(IDirect3DDevice9Ex* pcThis, UINT iSwapChain,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9Ex(L"IDirect3DDevice9Ex::GetDisplayModeEx");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMode;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pRotation;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9Ex[VMT_IDIRECT3DDEVICE9EX::GetDisplayModeEx]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
			return hr;
		}
		else
			return D3D929_IDirect3DDevice9Ex_GetDisplayModeEx_Super(pcThis, iSwapChain, pMode, pRotation);
	}
	else
		return D3D929_IDirect3DDevice9Ex_GetDisplayModeEx_Super(pcThis, iSwapChain, pMode, pRotation);
}

#pragma endregion

#pragma region DCL_IDirect3DDevice9Ex public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DDevice9Ex.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DDevice9Ex::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_QueryInterface_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:                      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_AddRef_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:                     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Release_Super                     = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
};

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DDevice9Ex.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DDevice9Ex::SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::VMT_IDirect3DDevice9 eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IDirect3DDevice9Ex super methods ***/
	case VMT_IDIRECT3DDEVICE9::TestCooperativeLevel:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_TestCooperativeLevel_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem:      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetAvailableTextureMem_Super      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::EvictManagedResources:       *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EvictManagedResources_Super       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetDirect3D:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDirect3D_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetDeviceCaps:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDeviceCaps_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetDisplayMode:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDisplayMode_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetCreationParameters:       *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetCreationParameters_Super       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetCursorProperties:         *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCursorProperties_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetCursorPosition:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCursorPosition_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::ShowCursor:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ShowCursor_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain:   *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateAdditionalSwapChain_Super   = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetSwapChain:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSwapChain_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains:       *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetNumberOfSwapChains_Super       = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::Reset:                       *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Reset_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::Present:                     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Present_Super                     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetBackBuffer:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetBackBuffer_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetRasterStatus:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRasterStatus_Super             = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::SetDialogBoxMode:            *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetDialogBoxMode_Super            = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetGammaRamp:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetGammaRamp_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetGammaRamp:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetGammaRamp_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateTexture:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateTexture_Super               = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::CreateVolumeTexture:         *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVolumeTexture_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateCubeTexture:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateCubeTexture_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateVertexBuffer:          *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexBuffer_Super          = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateIndexBuffer:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateIndexBuffer_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateRenderTarget:          *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateRenderTarget_Super          = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface:   *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurface_Super   = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::UpdateSurface:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_UpdateSurface_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::UpdateTexture:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_UpdateTexture_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetRenderTargetData:         *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderTargetData_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetFrontBufferData:          *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetFrontBufferData_Super          = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::StretchRect:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_StretchRect_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::ColorFill:                   *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ColorFill_Super                   = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface: *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetRenderTarget:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetRenderTarget_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetRenderTarget:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderTarget_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface:      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetDepthStencilSurface_Super      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface:      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDepthStencilSurface_Super      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::BeginScene:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_BeginScene_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::EndScene:                    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EndScene_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::Clear:                       *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Clear_Super                       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetTransform:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTransform_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetTransform:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTransform_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::MultiplyTransform:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_MultiplyTransform_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetViewport:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetViewport_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetViewport:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetViewport_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetMaterial:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetMaterial_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetMaterial:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetMaterial_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetLight:                    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetLight_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetLight:                    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetLight_Super                    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::LightEnable:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_LightEnable_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetLightEnable:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetLightEnable_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetClipPlane:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetClipPlane_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetClipPlane:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetClipPlane_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetRenderState:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetRenderState_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetRenderState:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderState_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateStateBlock:            *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateStateBlock_Super            = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::BeginStateBlock:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_BeginStateBlock_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::EndStateBlock:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EndStateBlock_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetClipStatus:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetClipStatus_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetClipStatus:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetClipStatus_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetTexture:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTexture_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetTexture:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTexture_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetTextureStageState:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTextureStageState_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetTextureStageState:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTextureStageState_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetSamplerState:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSamplerState_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetSamplerState:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetSamplerState_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::ValidateDevice:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ValidateDevice_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetPaletteEntries:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPaletteEntries_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetPaletteEntries:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPaletteEntries_Super           = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCurrentTexturePalette_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetCurrentTexturePalette_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetScissorRect:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetScissorRect_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetScissorRect:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetScissorRect_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing: *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing: *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetNPatchMode:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetNPatchMode_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetNPatchMode:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetNPatchMode_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawPrimitive:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawPrimitive_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitive_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawPrimitiveUP_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP:      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Super      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::ProcessVertices:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ProcessVertices_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexDeclaration_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetVertexDeclaration:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexDeclaration_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetVertexDeclaration:        *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexDeclaration_Super        = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetFVF:                      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetFVF_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetFVF:                      *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetFVF_Super                      = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateVertexShader:          *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexShader_Super          = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetVertexShader:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShader_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetVertexShader:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShader_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantF_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantF_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantI_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantI_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantB_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB:    *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantB_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetStreamSource:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetStreamSource_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetStreamSource:             *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetStreamSource_Super             = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq:         *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetStreamSourceFreq_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq:         *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetStreamSourceFreq_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetIndices:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetIndices_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetIndices:                  *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetIndices_Super                  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreatePixelShader:           *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreatePixelShader_Super           = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::SetPixelShader:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShader_Super              = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::GetPixelShader:              *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShader_Super              = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantF_Super     = (UINT_PTR)dwFunc; break; 
	case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantF_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantI_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantI_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantB_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB:     *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantB_Super     = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawRectPatch:               *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawRectPatch_Super               = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DrawTriPatch:                *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawTriPatch_Super                = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::DeletePatch:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DeletePatch_Super                 = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DDEVICE9::CreateQuery:                 *(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateQuery_Super                 = (UINT_PTR)dwFunc; break;
	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDirect3DDevice9Ex::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_QueryInterface_Super              = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_AddRef_Super                      = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Release_Super                     = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDirect3DDevice9Ex super methods ***/
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_TestCooperativeLevel_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetAvailableTextureMem_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EvictManagedResources_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDirect3D_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDeviceCaps_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDisplayMode_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetCreationParameters_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCursorProperties_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCursorPosition_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ShowCursor_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ShowCursor];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateAdditionalSwapChain_Super   = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSwapChain_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetNumberOfSwapChains_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Reset_Super                       = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Reset];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Present_Super                     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Present];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetBackBuffer_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRasterStatus_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetDialogBoxMode_Super            = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetGammaRamp_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetGammaRamp_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateTexture_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateTexture]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVolumeTexture_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateCubeTexture_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexBuffer_Super          = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateIndexBuffer_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateRenderTarget_Super          = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurface_Super   = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_UpdateSurface_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_UpdateTexture_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderTargetData_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetFrontBufferData_Super          = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_StretchRect_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::StretchRect];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ColorFill_Super                   = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ColorFill];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetRenderTarget_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderTarget_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetDepthStencilSurface_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetDepthStencilSurface_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_BeginScene_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::BeginScene];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EndScene_Super                    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EndScene];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_Clear_Super                       = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Clear];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTransform_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTransform];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTransform_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTransform];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_MultiplyTransform_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetViewport_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetViewport];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetViewport_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetViewport];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetMaterial_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetMaterial];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetMaterial_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetMaterial];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetLight_Super                    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetLight];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetLight_Super                    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetLight];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_LightEnable_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::LightEnable];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetLightEnable_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetClipPlane_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetClipPlane_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetRenderState_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetRenderState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetRenderState_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateStateBlock_Super            = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_BeginStateBlock_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_EndStateBlock_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetClipStatus_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetClipStatus_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTexture_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTexture];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTexture_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTexture];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetTextureStageState_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetTextureStageState_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSamplerState_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetSamplerState_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ValidateDevice_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPaletteEntries_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPaletteEntries_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetCurrentTexturePalette_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetCurrentTexturePalette_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetScissorRect_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetScissorRect_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetNPatchMode_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetNPatchMode_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawPrimitive_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitive_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawPrimitiveUP_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_ProcessVertices_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexDeclaration_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexDeclaration_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexDeclaration_Super        = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetFVF_Super                      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetFVF];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetFVF_Super                      = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetFVF];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateVertexShader_Super          = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShader_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShader_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantF_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantF_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantI_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantI_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantB_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantB_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetStreamSource_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetStreamSource_Super             = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetStreamSourceFreq_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetStreamSourceFreq_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetIndices_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetIndices];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetIndices_Super                  = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetIndices];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreatePixelShader_Super           = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShader_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShader_Super              = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantF_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]; 
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantF_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantI_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantI_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantB_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantB_Super     = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawRectPatch_Super               = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DrawTriPatch_Super                = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_DeletePatch_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DeletePatch];
	*(PUINT_PTR)&D3D929_IDirect3DDevice9Ex_CreateQuery_Super                 = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateQuery];

	return S_OK;
}
#pragma endregion