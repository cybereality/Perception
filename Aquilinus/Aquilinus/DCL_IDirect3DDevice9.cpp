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
#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))

// #define _DEBUGTHIS
#ifdef _DEBUGTHIS
#include <windows.h>
#include <tchar.h>
#include <sstream>
typedef std::basic_stringstream<TCHAR> tstringstream;
template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9(...)
#endif

#include"DCL_IDirect3DDevice9.h"
#include"DCL_IDirect3DDevice9_Super.h"

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
		h = 31 * h + pcData[i];
	}

	return h;
}
#pragma endregion

#pragma region DCL_IDirect3DDevice9 constructor/destructor

/**
* Constructor.
***/
DCL_IDirect3DDevice9::DCL_IDirect3DDevice9(AQU_TransferSite* pcTransferSite, IDirect3D9* pcParentObject) :
m_pcTransferSite(pcTransferSite),
m_pcParentObject(pcParentObject),
m_pcVShaderCurrent(nullptr),
m_unFVF(0),
m_bDrawingActive(false)
{}

/**
*
***/
DCL_IDirect3DDevice9::~DCL_IDirect3DDevice9(){}

#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::QueryInterface(IDirect3DDevice9* pcThis, REFIID riid, void** ppvObj)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::QueryInterface");

	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_pConfig->bCreateD3D9Ex))
	{
		//DEFINE_GUID(IID_IDirect3DDevice9Ex, 0xb18b10ce, 0x2649, 0x405a, 0x87, 0xf, 0x95, 0xf7, 0x77, 0xd4, 0x31, 0x3a);
		IF_GUID(riid, 0xb18b10ce, 0x2649, 0x405a, 0x87, 0xf, 0x95, 0xf7, 0x77, 0xd4, 0x31, 0x3a)
		{
			// no IID_IDirect3DDevice9Ex available
			if (ppvObj == NULL)
				return E_POINTER;

			D3D9_IDirect3DDevice9_AddRef_Super(pcThis);
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}

		//DEFINE_GUID(IID_IDirect3DDevice9, 0xd0223b96, 0xbf7a, 0x43fd, 0x92, 0xbd, 0xa4, 0x3b, 0xd, 0x82, 0xb9, 0xeb);
		IF_GUID(riid, 0xd0223b96, 0xbf7a, 0x43fd, 0x92, 0xbd, 0xa4, 0x3b, 0xd, 0x82, 0xb9, 0xeb)
		{
			*ppvObj = (LPVOID)pcThis;
			D3D9_IDirect3DDevice9_AddRef_Super(pcThis);
			return S_OK;
		}
	}

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&riid;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppvObj;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_QueryInterface_Super(pcThis, riid, ppvObj);
	}
	else nHr = D3D9_IDirect3DDevice9_QueryInterface_Super(pcThis, riid, ppvObj);

	return nHr;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DDevice9::AddRef(IDirect3DDevice9* pcThis)
{
	static int iCount = 0;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::AddRef");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::AddRef]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::AddRef]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::AddRef]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::AddRef]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
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
	iCount = D3D9_IDirect3DDevice9_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DDevice9::Release(IDirect3DDevice9* pcThis)
{
	static int iCount = 0;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::Release");


	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::Release]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::Release]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pcTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::Release]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
	//	m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::Release]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
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
	iCount = D3D9_IDirect3DDevice9_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDirect3DDevice9 methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::TestCooperativeLevel(IDirect3DDevice9* pcThis)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::TestCooperativeLevel");

	// automatically return S_OK for IDirect3DDeviceEx... TODO !!
	return S_OK;

	// return check device state in case of ex device
	IDirect3DDevice9Ex *pcDirect3DDevice9Ex = NULL;
	if (SUCCEEDED(((IDirect3DDevice9*)pcThis)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&pcDirect3DDevice9Ex))))
	{
		// TODO !! GETCREATIONPARAMETERS -> hDeviceWindow
		nHr = D3D_OK; //pcDirect3DDevice9Ex->CheckDeviceState(GetForegroundWindow());
		pcDirect3DDevice9Ex->Release();
		return nHr;
	}

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_TestCooperativeLevel_Super(pcThis);
	}
	else nHr = D3D9_IDirect3DDevice9_TestCooperativeLevel_Super(pcThis);
	return nHr;
}

/**
*
***/
UINT WINAPI DCL_IDirect3DDevice9::GetAvailableTextureMem(IDirect3DDevice9* pcThis)
{
	static UINT unI = 0;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetAvailableTextureMem");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			unI = (UINT)*(UINT*)pvRet;
			return unI;
		}
		else
			unI = D3D9_IDirect3DDevice9_GetAvailableTextureMem_Super(pcThis);
	}
	else
		unI = D3D9_IDirect3DDevice9_GetAvailableTextureMem_Super(pcThis);
	return unI;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::EvictManagedResources(IDirect3DDevice9* pcThis)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::EvictManagedResources");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EvictManagedResources]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EvictManagedResources]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_EvictManagedResources_Super(pcThis);
	}
	else nHr = D3D9_IDirect3DDevice9_EvictManagedResources_Super(pcThis);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetDirect3D(IDirect3DDevice9* pcThis, IDirect3D9** ppD3D9)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetDirect3D");

	if (!m_pcParentObject)
		return D3DERR_INVALIDCALL;
	else
	{
		*ppD3D9 = m_pcParentObject;
		m_pcParentObject->AddRef();
		return D3D_OK;
	}

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDirect3D]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppD3D9;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDirect3D]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDirect3D]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetDirect3D_Super(pcThis, ppD3D9);
	}
	else nHr = D3D9_IDirect3DDevice9_GetDirect3D_Super(pcThis, ppD3D9);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetDeviceCaps(IDirect3DDevice9* pcThis, D3DCAPS9* pCaps)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetDeviceCaps");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pCaps;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDeviceCaps]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetDeviceCaps_Super(pcThis, pCaps);
	}
	else nHr = D3D9_IDirect3DDevice9_GetDeviceCaps_Super(pcThis, pCaps);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetDisplayMode(IDirect3DDevice9* pcThis, UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetDisplayMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMode;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDisplayMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetDisplayMode_Super(pcThis, iSwapChain, pMode);
	}
	else nHr = D3D9_IDirect3DDevice9_GetDisplayMode_Super(pcThis, iSwapChain, pMode);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetCreationParameters(IDirect3DDevice9* pcThis, D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetCreationParameters");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCreationParameters]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pParameters;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCreationParameters]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetCreationParameters_Super(pcThis, pParameters);
	}
	else nHr = D3D9_IDirect3DDevice9_GetCreationParameters_Super(pcThis, pParameters);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetCursorProperties(IDirect3DDevice9* pcThis, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetCursorProperties");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[0]->m_pOutput = (void*)&XHotSpot;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[1]->m_pOutput = (void*)&YHotSpot;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pCursorBitmap;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorProperties]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetCursorProperties_Super(pcThis, XHotSpot, YHotSpot, pCursorBitmap);
	}
	else nHr = D3D9_IDirect3DDevice9_SetCursorProperties_Super(pcThis, XHotSpot, YHotSpot, pCursorBitmap);
	return nHr;
}

/**
*
***/
void WINAPI DCL_IDirect3DDevice9::SetCursorPosition(IDirect3DDevice9* pcThis, int X, int Y, DWORD Flags)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetCursorPosition");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[0]->m_pOutput = (void*)&X;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Y;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Flags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCursorPosition]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else return D3D9_IDirect3DDevice9_SetCursorPosition_Super(pcThis, X, Y, Flags);
	}
	else return D3D9_IDirect3DDevice9_SetCursorPosition_Super(pcThis, X, Y, Flags);
}

/**
*
***/
BOOL WINAPI DCL_IDirect3DDevice9::ShowCursor(IDirect3DDevice9* pcThis, BOOL bShow)
{
	static BOOL nB = FALSE;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::ShowCursor");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ShowCursor]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bShow;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ShowCursor]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ShowCursor]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nB = (BOOL)*(BOOL*)pvRet;
			return nB;
		}
		else
			nB = D3D9_IDirect3DDevice9_ShowCursor_Super(pcThis, bShow);
	}
	else
		nB = D3D9_IDirect3DDevice9_ShowCursor_Super(pcThis, bShow);

	return nB;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateAdditionalSwapChain(IDirect3DDevice9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateAdditionalSwapChain");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPresentationParameters;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSwapChain;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DSwapChain9 = *pSwapChain;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateAdditionalSwapChain_Super(pcThis, pPresentationParameters, pSwapChain);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetSwapChain(IDirect3DDevice9* pcThis, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	static HRESULT nHr = S_OK; OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetSwapChain");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSwapChain;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSwapChain]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetSwapChain_Super(pcThis, iSwapChain, pSwapChain);
	}
	else nHr = D3D9_IDirect3DDevice9_GetSwapChain_Super(pcThis, iSwapChain, pSwapChain);

	return nHr;
}

/**
*
***/
UINT WINAPI DCL_IDirect3DDevice9::GetNumberOfSwapChains(IDirect3DDevice9* pcThis)
{
	static UINT unI = 0;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetNumberOfSwapChains");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			unI = (UINT)*(UINT*)pvRet;
			return unI;
		}
		else
			unI = D3D9_IDirect3DDevice9_GetNumberOfSwapChains_Super(pcThis);
	}
	else
		unI = D3D9_IDirect3DDevice9_GetNumberOfSwapChains_Super(pcThis);

	return unI;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::Reset(IDirect3DDevice9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::Reset");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Reset]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Reset]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Reset]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pPresentationParameters;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Reset]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Reset]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}
	nHr = D3D9_IDirect3DDevice9_Reset_Super(pcThis, pPresentationParameters);

	// automatically reinstate interfaces for any reset
	CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::Present(IDirect3DDevice9* pcThis, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::Present");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[2]->m_pOutput = (void*)&hDestWindowOverride;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDirtyRegion;

		static bool s_bDebug = false;
		if (!s_bDebug)
		{
			OutputDebugString(L"------------------------PRESENT");
			DEBUG_HEX(m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[0]->m_pOutput);
			DEBUG_HEX(m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[1]->m_pOutput);
			DEBUG_HEX(m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[2]->m_pOutput);
			DEBUG_HEX(m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_paCommandersTemporary[3]->m_pOutput);
			s_bDebug = true;
		}

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Present]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
	else nHr = D3D9_IDirect3DDevice9_Present_Super(pcThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetBackBuffer(IDirect3DDevice9* pcThis, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetBackBuffer");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&iBackBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&ppBackBuffer;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetBackBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetBackBuffer_Super(pcThis, iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}
	else nHr = D3D9_IDirect3DDevice9_GetBackBuffer_Super(pcThis, iSwapChain, iBackBuffer, Type, ppBackBuffer);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetRasterStatus(IDirect3DDevice9* pcThis, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetRasterStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRasterStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRasterStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetRasterStatus_Super(pcThis, iSwapChain, pRasterStatus);
	}
	else nHr = D3D9_IDirect3DDevice9_GetRasterStatus_Super(pcThis, iSwapChain, pRasterStatus);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetDialogBoxMode(IDirect3DDevice9* pcThis, BOOL bEnableDialogs)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetDialogBoxMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bEnableDialogs;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetDialogBoxMode_Super(pcThis, bEnableDialogs);
	}
	else nHr = D3D9_IDirect3DDevice9_SetDialogBoxMode_Super(pcThis, bEnableDialogs);

	return nHr;
}

/**
*
***/
void WINAPI DCL_IDirect3DDevice9::SetGammaRamp(IDirect3DDevice9* pcThis, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetGammaRamp");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Flags;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pRamp;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetGammaRamp]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else
			return D3D9_IDirect3DDevice9_SetGammaRamp_Super(pcThis, iSwapChain, Flags, pRamp);
	}
	else return D3D9_IDirect3DDevice9_SetGammaRamp_Super(pcThis, iSwapChain, Flags, pRamp);
}

/**
*
***/
void WINAPI DCL_IDirect3DDevice9::GetGammaRamp(IDirect3DDevice9* pcThis, UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetGammaRamp");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRamp;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetGammaRamp]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			return;
		else
			return D3D9_IDirect3DDevice9_GetGammaRamp_Super(pcThis, iSwapChain, pRamp);
	}
	else return D3D9_IDirect3DDevice9_GetGammaRamp_Super(pcThis, iSwapChain, pRamp);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateTexture(IDirect3DDevice9* pcThis, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}

	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DTexture9 = *ppTexture;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateTexture_Super(pcThis, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateVolumeTexture(IDirect3DDevice9* pcThis, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateVolumeTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Depth;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[7]->m_pOutput = (void*)&ppVolumeTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_paCommandersTemporary[8]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DVolumeTexture9 = *ppVolumeTexture;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateVolumeTexture_Super(pcThis, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateCubeTexture(IDirect3DDevice9* pcThis, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateCubeTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&EdgeLength;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Levels;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[5]->m_pOutput = (void*)&ppCubeTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_paCommandersTemporary[6]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateCubeTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DCubeTexture9 = *ppCubeTexture;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateCubeTexture_Super(pcThis, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateVertexBuffer(IDirect3DDevice9* pcThis, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateVertexBuffer");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Length;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&FVF;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppVertexBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DVertexBuffer9 = *ppVertexBuffer;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateVertexBuffer_Super(pcThis, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateIndexBuffer(IDirect3DDevice9* pcThis, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateIndexBuffer");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Length;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Usage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppIndexBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DIndexBuffer9 = *ppIndexBuffer;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateIndexBuffer_Super(pcThis, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateRenderTarget(IDirect3DDevice9* pcThis, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateRenderTarget");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Lockable;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DSurface9 = *ppSurface;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateRenderTarget_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateDepthStencilSurface(IDirect3DDevice9* pcThis, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateDepthStencilSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&MultiSample;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[4]->m_pOutput = (void*)&MultisampleQuality;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Discard;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[6]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_paCommandersTemporary[7]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DSurface9 = *ppSurface;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateDepthStencilSurface_Super(pcThis, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::UpdateSurface(IDirect3DDevice9* pcThis, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::UpdateSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pDestinationSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestPoint;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_UpdateSurface_Super(pcThis, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
	}
	else nHr = D3D9_IDirect3DDevice9_UpdateSurface_Super(pcThis, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::UpdateTexture(IDirect3DDevice9* pcThis, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::UpdateTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceTexture;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestinationTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::UpdateTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_UpdateTexture_Super(pcThis, pSourceTexture, pDestinationTexture);
	}
	else nHr = D3D9_IDirect3DDevice9_UpdateTexture_Super(pcThis, pSourceTexture, pDestinationTexture);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetRenderTargetData(IDirect3DDevice9* pcThis, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetRenderTargetData");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRenderTarget;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTargetData]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetRenderTargetData_Super(pcThis, pRenderTarget, pDestSurface);
	}
	else nHr = D3D9_IDirect3DDevice9_GetRenderTargetData_Super(pcThis, pRenderTarget, pDestSurface);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetFrontBufferData(IDirect3DDevice9* pcThis, UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetFrontBufferData");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_paCommandersTemporary[0]->m_pOutput = (void*)&iSwapChain;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pDestSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFrontBufferData]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetFrontBufferData_Super(pcThis, iSwapChain, pDestSurface);
	}
	else nHr = D3D9_IDirect3DDevice9_GetFrontBufferData_Super(pcThis, iSwapChain, pDestSurface);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::StretchRect(IDirect3DDevice9* pcThis, IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::StretchRect");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSourceSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSourceRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pDestSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Filter;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::StretchRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_StretchRect_Super(pcThis, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
	}
	else nHr = D3D9_IDirect3DDevice9_StretchRect_Super(pcThis, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::ColorFill(IDirect3DDevice9* pcThis, IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::ColorFill");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRect;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->m_paCommandersTemporary[2]->m_pOutput = (void*)&color;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ColorFill]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_ColorFill_Super(pcThis, pSurface, pRect, color);
	}
	else nHr = D3D9_IDirect3DDevice9_ColorFill_Super(pcThis, pSurface, pRect, color);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateOffscreenPlainSurface(IDirect3DDevice9* pcThis, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateOffscreenPlainSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Width;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Height;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Format;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Pool;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[4]->m_pOutput = (void*)&ppSurface;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_paCommandersTemporary[5]->m_pOutput = (void*)&pSharedHandle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}
	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DSurface9 = *ppSurface;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}
	nHr = D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface_Super(pcThis, Width, Height, Format, Pool, ppSurface, pSharedHandle);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetRenderTarget(IDirect3DDevice9* pcThis, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetRenderTarget");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&RenderTargetIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRenderTarget;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetRenderTarget_Super(pcThis, RenderTargetIndex, pRenderTarget);
	}
	nHr = D3D9_IDirect3DDevice9_SetRenderTarget_Super(pcThis, RenderTargetIndex, pRenderTarget);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetRenderTarget(IDirect3DDevice9* pcThis, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetRenderTarget");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_paCommandersTemporary[0]->m_pOutput = (void*)&RenderTargetIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppRenderTarget;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderTarget]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetRenderTarget_Super(pcThis, RenderTargetIndex, ppRenderTarget);
	}
	else nHr = D3D9_IDirect3DDevice9_GetRenderTarget_Super(pcThis, RenderTargetIndex, ppRenderTarget);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetDepthStencilSurface(IDirect3DDevice9* pcThis, IDirect3DSurface9* pNewZStencil)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetDepthStencilSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pNewZStencil;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetDepthStencilSurface_Super(pcThis, pNewZStencil);
	}
	else
		nHr = D3D9_IDirect3DDevice9_SetDepthStencilSurface_Super(pcThis, pNewZStencil);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetDepthStencilSurface(IDirect3DDevice9* pcThis, IDirect3DSurface9** ppZStencilSurface)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetDepthStencilSurface");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppZStencilSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetDepthStencilSurface_Super(pcThis, ppZStencilSurface);
	}
	else nHr = D3D9_IDirect3DDevice9_GetDepthStencilSurface_Super(pcThis, ppZStencilSurface);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::BeginScene(IDirect3DDevice9* pcThis)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::BeginScene");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginScene]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginScene]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginScene]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginScene]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_BeginScene_Super(pcThis);
	}
	else nHr = D3D9_IDirect3DDevice9_BeginScene_Super(pcThis);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::EndScene(IDirect3DDevice9* pcThis)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::EndScene");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndScene]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndScene]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndScene]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndScene]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_EndScene_Super(pcThis);
	}
	else nHr = D3D9_IDirect3DDevice9_EndScene_Super(pcThis);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::Clear(IDirect3DDevice9* pcThis, DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::Clear");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Count;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pRects;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Flags;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Color;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[4]->m_pOutput = (void*)&Z;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Stencil;

		// precall
		if (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
			D3D9_IDirect3DDevice9_Clear_Super(pcThis, Count, pRects, Flags, Color, Z, Stencil);

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::Clear]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_Clear_Super(pcThis, Count, pRects, Flags, Color, Z, Stencil);
	}
	else nHr = D3D9_IDirect3DDevice9_Clear_Super(pcThis, Count, pRects, Flags, Color, Z, Stencil);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetTransform(IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetTransform_Super(pcThis, State, pMatrix);
	}
	else nHr = D3D9_IDirect3DDevice9_SetTransform_Super(pcThis, State, pMatrix);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetTransform(IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetTransform_Super(pcThis, State, pMatrix);
	}
	else nHr = D3D9_IDirect3DDevice9_GetTransform_Super(pcThis, State, pMatrix);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::MultiplyTransform(IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::MultiplyTransform");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pMatrix;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::MultiplyTransform]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_MultiplyTransform_Super(pcThis, State, pMatrix);
	}
	else nHr = D3D9_IDirect3DDevice9_MultiplyTransform_Super(pcThis, State, pMatrix);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetViewport(IDirect3DDevice9* pcThis, CONST D3DVIEWPORT9* pViewport)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetViewport");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetViewport]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetViewport]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetViewport]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pViewport;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetViewport]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetViewport]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetViewport_Super(pcThis, pViewport);
	}
	else nHr = D3D9_IDirect3DDevice9_SetViewport_Super(pcThis, pViewport);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetViewport(IDirect3DDevice9* pcThis, D3DVIEWPORT9* pViewport)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetViewport");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetViewport]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetViewport]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetViewport]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pViewport;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetViewport]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetViewport]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetViewport_Super(pcThis, pViewport);
	}
	else nHr = D3D9_IDirect3DDevice9_GetViewport_Super(pcThis, pViewport);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetMaterial(IDirect3DDevice9* pcThis, CONST D3DMATERIAL9* pMaterial)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetMaterial");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetMaterial]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pMaterial;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetMaterial]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetMaterial]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetMaterial_Super(pcThis, pMaterial);
	}
	else nHr = D3D9_IDirect3DDevice9_SetMaterial_Super(pcThis, pMaterial);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetMaterial(IDirect3DDevice9* pcThis, D3DMATERIAL9* pMaterial)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetMaterial");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetMaterial]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pMaterial;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetMaterial]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetMaterial]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetMaterial_Super(pcThis, pMaterial);
	}
	else nHr = D3D9_IDirect3DDevice9_GetMaterial_Super(pcThis, pMaterial);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetLight(IDirect3DDevice9* pcThis, DWORD Index, CONST D3DLIGHT9* pLight)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetLight");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pLight;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetLight]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetLight_Super(pcThis, Index, pLight);
	}
	else nHr = D3D9_IDirect3DDevice9_SetLight_Super(pcThis, Index, pLight);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetLight(IDirect3DDevice9* pcThis, DWORD Index, D3DLIGHT9* pLight)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetLight");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pLight;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLight]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetLight_Super(pcThis, Index, pLight);
	}
	else nHr = D3D9_IDirect3DDevice9_GetLight_Super(pcThis, Index, pLight);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::LightEnable(IDirect3DDevice9* pcThis, DWORD Index, BOOL Enable)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::LightEnable");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Enable;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::LightEnable]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_LightEnable_Super(pcThis, Index, Enable);
	}
	else nHr = D3D9_IDirect3DDevice9_LightEnable_Super(pcThis, Index, Enable);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetLightEnable(IDirect3DDevice9* pcThis, DWORD Index, BOOL* pEnable)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetLightEnable");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEnable;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetLightEnable]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetLightEnable_Super(pcThis, Index, pEnable);
	}
	else nHr = D3D9_IDirect3DDevice9_GetLightEnable_Super(pcThis, Index, pEnable);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetClipPlane(IDirect3DDevice9* pcThis, DWORD Index, CONST float* pPlane)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetClipPlane");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pPlane;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipPlane]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetClipPlane_Super(pcThis, Index, pPlane);
	}
	else nHr = D3D9_IDirect3DDevice9_SetClipPlane_Super(pcThis, Index, pPlane);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetClipPlane(IDirect3DDevice9* pcThis, DWORD Index, float* pPlane)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetClipPlane");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Index;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pPlane;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipPlane]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetClipPlane_Super(pcThis, Index, pPlane);
	}
	else nHr = D3D9_IDirect3DDevice9_GetClipPlane_Super(pcThis, Index, pPlane);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetRenderState(IDirect3DDevice9* pcThis, D3DRENDERSTATETYPE State, DWORD Value)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetRenderState");

	//#define DEBUG_DX9_RENDERSTATES
#ifdef DEBUG_DX9_RENDERSTATES
	if (!m_pcTransferSite->m_bForceD3D)
	{
		switch (State)
		{
			case D3DRS_ZENABLE:
				OutputDebugString(L"D3DRS_ZENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_FILLMODE:
				OutputDebugString(L"D3DRS_FILLMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SHADEMODE:
				OutputDebugString(L"D3DRS_SHADEMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_ZWRITEENABLE:
				OutputDebugString(L"D3DRS_ZWRITEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_ALPHATESTENABLE:
				OutputDebugString(L"D3DRS_ALPHATESTENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_LASTPIXEL:
				OutputDebugString(L"D3DRS_LASTPIXEL"); DEBUG_UINT(Value);
				break;
			case D3DRS_SRCBLEND:
				OutputDebugString(L"D3DRS_SRCBLEND"); DEBUG_UINT(Value);
				break;
			case D3DRS_DESTBLEND:
				OutputDebugString(L"D3DRS_DESTBLEND"); DEBUG_UINT(Value);
				break;
			case D3DRS_CULLMODE:
				OutputDebugString(L"D3DRS_CULLMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_ZFUNC:
				OutputDebugString(L"D3DRS_ZFUNC"); DEBUG_UINT(Value);
				break;
			case D3DRS_ALPHAREF:
				OutputDebugString(L"D3DRS_ALPHAREF"); DEBUG_UINT(Value);
				break;
			case D3DRS_ALPHAFUNC:
				OutputDebugString(L"D3DRS_ALPHAFUNC"); DEBUG_UINT(Value);
				break;
			case D3DRS_DITHERENABLE:
				OutputDebugString(L"D3DRS_DITHERENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_ALPHABLENDENABLE:
				OutputDebugString(L"D3DRS_ALPHABLENDENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGENABLE:
				OutputDebugString(L"D3DRS_FOGENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SPECULARENABLE:
				OutputDebugString(L"D3DRS_SPECULARENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGCOLOR:
				OutputDebugString(L"D3DRS_FOGCOLOR"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGTABLEMODE:
				OutputDebugString(L"D3DRS_FOGTABLEMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGSTART:
				OutputDebugString(L"D3DRS_FOGSTART"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGEND:
				OutputDebugString(L"D3DRS_FOGEND"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGDENSITY:
				OutputDebugString(L"D3DRS_FOGDENSITY"); DEBUG_UINT(Value);
				break;
			case D3DRS_RANGEFOGENABLE:
				OutputDebugString(L"D3DRS_RANGEFOGENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILENABLE:
				OutputDebugString(L"D3DRS_STENCILENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILFAIL:
				OutputDebugString(L"D3DRS_STENCILFAIL"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILZFAIL:
				OutputDebugString(L"D3DRS_STENCILZFAIL"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILPASS:
				OutputDebugString(L"D3DRS_STENCILPASS"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILFUNC:
				OutputDebugString(L"D3DRS_STENCILFUNC"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILREF:
				OutputDebugString(L"D3DRS_STENCILREF"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILMASK:
				OutputDebugString(L"D3DRS_STENCILMASK"); DEBUG_UINT(Value);
				break;
			case D3DRS_STENCILWRITEMASK:
				OutputDebugString(L"D3DRS_STENCILWRITEMASK"); DEBUG_UINT(Value);
				break;
			case D3DRS_TEXTUREFACTOR:
				OutputDebugString(L"D3DRS_TEXTUREFACTOR"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP0:
				OutputDebugString(L"D3DRS_WRAP0"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP1:
				OutputDebugString(L"D3DRS_WRAP1"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP2:
				OutputDebugString(L"D3DRS_WRAP2"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP3:
				OutputDebugString(L"D3DRS_WRAP3"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP4:
				OutputDebugString(L"D3DRS_WRAP4"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP5:
				OutputDebugString(L"D3DRS_WRAP5"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP6:
				OutputDebugString(L"D3DRS_WRAP6"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP7:
				OutputDebugString(L"D3DRS_WRAP7"); DEBUG_UINT(Value);
				break;
			case D3DRS_CLIPPING:
				OutputDebugString(L"D3DRS_CLIPPING"); DEBUG_UINT(Value);
				break;
			case D3DRS_LIGHTING:
				OutputDebugString(L"D3DRS_LIGHTING"); DEBUG_UINT(Value);
				break;
			case D3DRS_AMBIENT:
				OutputDebugString(L"D3DRS_AMBIENT"); DEBUG_UINT(Value);
				break;
			case D3DRS_FOGVERTEXMODE:
				OutputDebugString(L"D3DRS_FOGVERTEXMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_COLORVERTEX:
				OutputDebugString(L"D3DRS_COLORVERTEX"); DEBUG_UINT(Value);
				break;
			case D3DRS_LOCALVIEWER:
				OutputDebugString(L"D3DRS_LOCALVIEWER"); DEBUG_UINT(Value);
				break;
			case D3DRS_NORMALIZENORMALS:
				OutputDebugString(L"D3DRS_NORMALIZENORMALS"); DEBUG_UINT(Value);
				break;
			case D3DRS_DIFFUSEMATERIALSOURCE:
				OutputDebugString(L"D3DRS_DIFFUSEMATERIALSOURCE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SPECULARMATERIALSOURCE:
				OutputDebugString(L"D3DRS_SPECULARMATERIALSOURCE"); DEBUG_UINT(Value);
				break;
			case D3DRS_AMBIENTMATERIALSOURCE:
				OutputDebugString(L"D3DRS_AMBIENTMATERIALSOURCE"); DEBUG_UINT(Value);
				break;
			case D3DRS_EMISSIVEMATERIALSOURCE:
				OutputDebugString(L"D3DRS_EMISSIVEMATERIALSOURCE"); DEBUG_UINT(Value);
				break;
			case D3DRS_VERTEXBLEND:
				OutputDebugString(L"D3DRS_VERTEXBLEND"); DEBUG_UINT(Value);
				break;
			case D3DRS_CLIPPLANEENABLE:
				OutputDebugString(L"D3DRS_CLIPPLANEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSIZE:
				OutputDebugString(L"D3DRS_POINTSIZE"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSIZE_MIN:
				OutputDebugString(L"D3DRS_POINTSIZE_MIN"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSPRITEENABLE:
				OutputDebugString(L"D3DRS_POINTSPRITEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSCALEENABLE:
				OutputDebugString(L"D3DRS_POINTSCALEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSCALE_A:
				OutputDebugString(L"D3DRS_POINTSCALE_A"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSCALE_B:
				OutputDebugString(L"D3DRS_POINTSCALE_B"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSCALE_C:
				OutputDebugString(L"D3DRS_POINTSCALE_C"); DEBUG_UINT(Value);
				break;
			case D3DRS_MULTISAMPLEANTIALIAS:
				OutputDebugString(L"D3DRS_MULTISAMPLEANTIALIAS"); DEBUG_UINT(Value);
				break;
			case D3DRS_MULTISAMPLEMASK:
				OutputDebugString(L"D3DRS_MULTISAMPLEMASK"); DEBUG_UINT(Value);
				break;
			case D3DRS_PATCHEDGESTYLE:
				OutputDebugString(L"D3DRS_PATCHEDGESTYLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_DEBUGMONITORTOKEN:
				OutputDebugString(L"D3DRS_DEBUGMONITORTOKEN"); DEBUG_UINT(Value);
				break;
			case D3DRS_POINTSIZE_MAX:
				OutputDebugString(L"D3DRS_POINTSIZE_MAX"); DEBUG_UINT(Value);
				break;
			case D3DRS_INDEXEDVERTEXBLENDENABLE:
				OutputDebugString(L"D3DRS_INDEXEDVERTEXBLENDENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_COLORWRITEENABLE:
				OutputDebugString(L"D3DRS_COLORWRITEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_TWEENFACTOR:
				OutputDebugString(L"D3DRS_TWEENFACTOR"); DEBUG_UINT(Value);
				break;
			case D3DRS_BLENDOP:
				OutputDebugString(L"D3DRS_BLENDOP"); DEBUG_UINT(Value);
				break;
			case D3DRS_POSITIONDEGREE:
				OutputDebugString(L"D3DRS_POSITIONDEGREE"); DEBUG_UINT(Value);
				break;
			case D3DRS_NORMALDEGREE:
				OutputDebugString(L"D3DRS_NORMALDEGREE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SCISSORTESTENABLE:
				OutputDebugString(L"D3DRS_SCISSORTESTENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SLOPESCALEDEPTHBIAS:
				OutputDebugString(L"D3DRS_SLOPESCALEDEPTHBIAS"); DEBUG_UINT(Value);
				break;
			case D3DRS_ANTIALIASEDLINEENABLE:
				OutputDebugString(L"D3DRS_ANTIALIASEDLINEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_MINTESSELLATIONLEVEL:
				OutputDebugString(L"D3DRS_MINTESSELLATIONLEVEL"); DEBUG_UINT(Value);
				break;
			case D3DRS_MAXTESSELLATIONLEVEL:
				OutputDebugString(L"D3DRS_MAXTESSELLATIONLEVEL"); DEBUG_UINT(Value);
				break;
			case D3DRS_ADAPTIVETESS_X:
				OutputDebugString(L"D3DRS_ADAPTIVETESS_X"); DEBUG_UINT(Value);
				break;
			case D3DRS_ADAPTIVETESS_Y:
				OutputDebugString(L"D3DRS_ADAPTIVETESS_Y"); DEBUG_UINT(Value);
				break;
			case D3DRS_ADAPTIVETESS_Z:
				OutputDebugString(L"D3DRS_ADAPTIVETESS_Z"); DEBUG_UINT(Value);
				break;
			case D3DRS_ADAPTIVETESS_W:
				OutputDebugString(L"D3DRS_ADAPTIVETESS_W"); DEBUG_UINT(Value);
				break;
			case D3DRS_ENABLEADAPTIVETESSELLATION:
				OutputDebugString(L"D3DRS_ENABLEADAPTIVETESSELLATION"); DEBUG_UINT(Value);
				break;
			case D3DRS_TWOSIDEDSTENCILMODE:
				OutputDebugString(L"D3DRS_TWOSIDEDSTENCILMODE"); DEBUG_UINT(Value);
				break;
			case D3DRS_CCW_STENCILFAIL:
				OutputDebugString(L"D3DRS_CCW_STENCILFAIL"); DEBUG_UINT(Value);
				break;
			case D3DRS_CCW_STENCILZFAIL:
				OutputDebugString(L"D3DRS_CCW_STENCILZFAIL"); DEBUG_UINT(Value);
				break;
			case D3DRS_CCW_STENCILPASS:
				OutputDebugString(L"D3DRS_CCW_STENCILPASS"); DEBUG_UINT(Value);
				break;
			case D3DRS_CCW_STENCILFUNC:
				OutputDebugString(L"D3DRS_CCW_STENCILFUNC"); DEBUG_UINT(Value);
				break;
			case D3DRS_COLORWRITEENABLE1:
				OutputDebugString(L"D3DRS_COLORWRITEENABLE1"); DEBUG_UINT(Value);
				break;
			case D3DRS_COLORWRITEENABLE2:
				OutputDebugString(L"D3DRS_COLORWRITEENABLE2"); DEBUG_UINT(Value);
				break;
			case D3DRS_COLORWRITEENABLE3:
				OutputDebugString(L"D3DRS_COLORWRITEENABLE3"); DEBUG_UINT(Value);
				break;
			case D3DRS_BLENDFACTOR:
				OutputDebugString(L"D3DRS_BLENDFACTOR"); DEBUG_UINT(Value);
				break;
			case D3DRS_SRGBWRITEENABLE:
				OutputDebugString(L"D3DRS_SRGBWRITEENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_DEPTHBIAS:
				OutputDebugString(L"D3DRS_DEPTHBIAS"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP8:
				OutputDebugString(L"D3DRS_WRAP8"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP9:
				OutputDebugString(L"D3DRS_WRAP9"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP10:
				OutputDebugString(L"D3DRS_WRAP10"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP11:
				OutputDebugString(L"D3DRS_WRAP11"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP12:
				OutputDebugString(L"D3DRS_WRAP12"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP13:
				OutputDebugString(L"D3DRS_WRAP13"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP14:
				OutputDebugString(L"D3DRS_WRAP14"); DEBUG_UINT(Value);
				break;
			case D3DRS_WRAP15:
				OutputDebugString(L"D3DRS_WRAP15"); DEBUG_UINT(Value);
				break;
			case D3DRS_SEPARATEALPHABLENDENABLE:
				OutputDebugString(L"D3DRS_SEPARATEALPHABLENDENABLE"); DEBUG_UINT(Value);
				break;
			case D3DRS_SRCBLENDALPHA:
				OutputDebugString(L"D3DRS_SRCBLENDALPHA"); DEBUG_UINT(Value);
				break;
			case D3DRS_DESTBLENDALPHA:
				OutputDebugString(L"D3DRS_DESTBLENDALPHA"); DEBUG_UINT(Value);
				break;
			case D3DRS_BLENDOPALPHA:
				OutputDebugString(L"D3DRS_BLENDOPALPHA"); DEBUG_UINT(Value);
				break;
			case D3DRS_FORCE_DWORD:
				OutputDebugString(L"D3DRS_FORCE_DWORD"); DEBUG_UINT(Value);
				break;
			default:
				break;
		}
	}
#endif

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetRenderState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetRenderState_Super(pcThis, State, Value);
	}
	else nHr = D3D9_IDirect3DDevice9_SetRenderState_Super(pcThis, State, Value);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetRenderState(IDirect3DDevice9* pcThis, D3DRENDERSTATETYPE State, DWORD* pValue)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetRenderState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&State;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetRenderState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetRenderState_Super(pcThis, State, pValue);
	}
	else nHr = D3D9_IDirect3DDevice9_GetRenderState_Super(pcThis, State, pValue);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateStateBlock(IDirect3DDevice9* pcThis, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppSB;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DStateBlock9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DStateBlock9 = *ppSB;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateStateBlock_Super(pcThis, Type, ppSB);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::BeginStateBlock(IDirect3DDevice9* pcThis)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::BeginStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::BeginStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_BeginStateBlock_Super(pcThis);
	}
	else nHr = D3D9_IDirect3DDevice9_BeginStateBlock_Super(pcThis);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::EndStateBlock(IDirect3DDevice9* pcThis, IDirect3DStateBlock9** ppSB)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::EndStateBlock");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndStateBlock]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppSB;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndStateBlock]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::EndStateBlock]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DStateBlock9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DStateBlock9 = *ppSB;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_EndStateBlock_Super(pcThis, ppSB);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetClipStatus(IDirect3DDevice9* pcThis, CONST D3DCLIPSTATUS9* pClipStatus)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetClipStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pClipStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetClipStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetClipStatus_Super(pcThis, pClipStatus);
	}
	else nHr = D3D9_IDirect3DDevice9_SetClipStatus_Super(pcThis, pClipStatus);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetClipStatus(IDirect3DDevice9* pcThis, D3DCLIPSTATUS9* pClipStatus)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetClipStatus");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipStatus]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pClipStatus;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipStatus]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetClipStatus]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetClipStatus_Super(pcThis, pClipStatus);
	}
	else nHr = D3D9_IDirect3DDevice9_GetClipStatus_Super(pcThis, pClipStatus);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetTexture(IDirect3DDevice9* pcThis, DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetTexture_Super(pcThis, Stage, ppTexture);
	}
	else nHr = D3D9_IDirect3DDevice9_GetTexture_Super(pcThis, Stage, ppTexture);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetTexture(IDirect3DDevice9* pcThis, DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetTexture");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTexture]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetTexture_Super(pcThis, Stage, pTexture);
	}
	else
		nHr = D3D9_IDirect3DDevice9_SetTexture_Super(pcThis, Stage, pTexture);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetTextureStageState(IDirect3DDevice9* pcThis, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetTextureStageState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetTextureStageState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetTextureStageState_Super(pcThis, Stage, Type, pValue);
	}
	else nHr = D3D9_IDirect3DDevice9_GetTextureStageState_Super(pcThis, Stage, Type, pValue);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetTextureStageState(IDirect3DDevice9* pcThis, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetTextureStageState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Stage;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetTextureStageState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetTextureStageState_Super(pcThis, Stage, Type, Value);
	}
	else nHr = D3D9_IDirect3DDevice9_SetTextureStageState_Super(pcThis, Stage, Type, Value);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetSamplerState(IDirect3DDevice9* pcThis, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetSamplerState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Sampler;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pValue;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSamplerState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetSamplerState_Super(pcThis, Sampler, Type, pValue);
	}
	else nHr = D3D9_IDirect3DDevice9_GetSamplerState_Super(pcThis, Sampler, Type, pValue);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetSamplerState(IDirect3DDevice9* pcThis, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetSamplerState");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Sampler;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Value;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSamplerState]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetSamplerState_Super(pcThis, Sampler, Type, Value);
	}
	else nHr = D3D9_IDirect3DDevice9_SetSamplerState_Super(pcThis, Sampler, Type, Value);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::ValidateDevice(IDirect3DDevice9* pcThis, DWORD* pNumPasses)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::ValidateDevice");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ValidateDevice]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pNumPasses;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ValidateDevice]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ValidateDevice]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_ValidateDevice_Super(pcThis, pNumPasses);
	}
	else nHr = D3D9_IDirect3DDevice9_ValidateDevice_Super(pcThis, pNumPasses);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetPaletteEntries(IDirect3DDevice9* pcThis, UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetPaletteEntries");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEntries;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPaletteEntries]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
	}
	else nHr = D3D9_IDirect3DDevice9_SetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetPaletteEntries(IDirect3DDevice9* pcThis, UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetPaletteEntries");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pEntries;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPaletteEntries]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);
	}
	else nHr = D3D9_IDirect3DDevice9_GetPaletteEntries_Super(pcThis, PaletteNumber, pEntries);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetCurrentTexturePalette(IDirect3DDevice9* pcThis, UINT PaletteNumber)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetCurrentTexturePalette");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetCurrentTexturePalette_Super(pcThis, PaletteNumber);
	}
	else nHr = D3D9_IDirect3DDevice9_SetCurrentTexturePalette_Super(pcThis, PaletteNumber);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetCurrentTexturePalette(IDirect3DDevice9* pcThis, UINT *PaletteNumber)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetCurrentTexturePalette");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PaletteNumber;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetCurrentTexturePalette_Super(pcThis, PaletteNumber);
	}
	else nHr = D3D9_IDirect3DDevice9_GetCurrentTexturePalette_Super(pcThis, PaletteNumber);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetScissorRect(IDirect3DDevice9* pcThis, CONST RECT* pRect)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetScissorRect");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetScissorRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRect;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetScissorRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetScissorRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetScissorRect_Super(pcThis, pRect);
	}
	else nHr = D3D9_IDirect3DDevice9_SetScissorRect_Super(pcThis, pRect);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetScissorRect(IDirect3DDevice9* pcThis, RECT* pRect)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetScissorRect");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetScissorRect]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pRect;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetScissorRect]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetScissorRect]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetScissorRect_Super(pcThis, pRect);
	}
	else nHr = D3D9_IDirect3DDevice9_GetScissorRect_Super(pcThis, pRect);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetSoftwareVertexProcessing(IDirect3DDevice9* pcThis, BOOL bSoftware)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetSoftwareVertexProcessing");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_paCommandersTemporary[0]->m_pOutput = (void*)&bSoftware;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing_Super(pcThis, bSoftware);
	}
	else nHr = D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing_Super(pcThis, bSoftware);

	return nHr;
}

/**
*
***/
BOOL WINAPI DCL_IDirect3DDevice9::GetSoftwareVertexProcessing(IDirect3DDevice9* pcThis)
{
	static BOOL nB = FALSE;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetSoftwareVertexProcessing");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nB = (BOOL)*(BOOL*)pvRet;
			return nB;
		}
		else
			nB = D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing_Super(pcThis);
	}
	else
		nB = D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing_Super(pcThis);

	return nB;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetNPatchMode(IDirect3DDevice9* pcThis, float nSegments)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetNPatchMode");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetNPatchMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_paCommandersTemporary[0]->m_pOutput = (void*)&nSegments;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetNPatchMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetNPatchMode_Super(pcThis, nSegments);
	}
	else nHr = D3D9_IDirect3DDevice9_SetNPatchMode_Super(pcThis, nSegments);
	return nHr;
}

/**
*
***/
float WINAPI DCL_IDirect3DDevice9::GetNPatchMode(IDirect3DDevice9* pcThis)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetNPatchMode");

	static float fRet = 0.0f;

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNPatchMode]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetNPatchMode]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			fRet = (float)*(float*)pvRet;
			return fRet;
		}
		else
			fRet = D3D9_IDirect3DDevice9_GetNPatchMode_Super(pcThis);
	}
	else
		fRet = D3D9_IDirect3DDevice9_GetNPatchMode_Super(pcThis);

	return fRet;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawPrimitive(IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawPrimitive");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[1]->m_pOutput = (void*)&StartVertex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_paCommandersTemporary[2]->m_pOutput = (void*)&PrimitiveCount;

		// precall
		if (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
		{
			m_pcTransferSite->m_bForceD3D = true;
			D3D9_IDirect3DDevice9_DrawPrimitive_Super(pcThis, PrimitiveType, StartVertex, PrimitiveCount);
			m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;
		}

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;


		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitive]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawPrimitive_Super(pcThis, PrimitiveType, StartVertex, PrimitiveCount);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawPrimitive_Super(pcThis, PrimitiveType, StartVertex, PrimitiveCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawIndexedPrimitive(IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawIndexedPrimitive");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[1]->m_pOutput = (void*)&BaseVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[2]->m_pOutput = (void*)&MinVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[3]->m_pOutput = (void*)&NumVertices;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[4]->m_pOutput = (void*)&startIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_paCommandersTemporary[5]->m_pOutput = (void*)&primCount;

		// precall
		if (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
		{
			D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super(pcThis, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		}

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;


		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super(pcThis, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super(pcThis, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawPrimitiveUP(IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawPrimitiveUP");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[1]->m_pOutput = (void*)&PrimitiveCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pVertexStreamZeroData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_paCommandersTemporary[3]->m_pOutput = (void*)&VertexStreamZeroStride;

		// precall
		if (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
			D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super(pcThis, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;


		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super(pcThis, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super(pcThis, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawIndexedPrimitiveUP(IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawIndexedPrimitiveUP");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[0]->m_pOutput = (void*)&PrimitiveType;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[1]->m_pOutput = (void*)&MinVertexIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[2]->m_pOutput = (void*)&NumVertices;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[3]->m_pOutput = (void*)&PrimitiveCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[4]->m_pOutput = (void*)&pIndexData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[5]->m_pOutput = (void*)&IndexDataFormat;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[6]->m_pOutput = (void*)&pVertexStreamZeroData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_paCommandersTemporary[7]->m_pOutput = (void*)&VertexStreamZeroStride;

		// precall
		if (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->GetNextCycleBehavior() == AQU_NextNodeCall::DoubleCall)
			D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super(pcThis, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;


		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super(pcThis, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super(pcThis, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::ProcessVertices(IDirect3DDevice9* pcThis, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::ProcessVertices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&SrcStartIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[1]->m_pOutput = (void*)&DestIndex;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[2]->m_pOutput = (void*)&VertexCount;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pDestBuffer;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[4]->m_pOutput = (void*)&pVertexDecl;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_paCommandersTemporary[5]->m_pOutput = (void*)&Flags;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::ProcessVertices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_ProcessVertices_Super(pcThis, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
	}
	else nHr = D3D9_IDirect3DDevice9_ProcessVertices_Super(pcThis, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateVertexDeclaration(IDirect3DDevice9* pcThis, CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pVertexElements;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_CreateVertexDeclaration_Super(pcThis, pVertexElements, ppDecl);
	}
	else nHr = D3D9_IDirect3DDevice9_CreateVertexDeclaration_Super(pcThis, pVertexElements, ppDecl);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetVertexDeclaration(IDirect3DDevice9* pcThis, IDirect3DVertexDeclaration9* pDecl)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetVertexDeclaration_Super(pcThis, pDecl);
	}
	else nHr = D3D9_IDirect3DDevice9_SetVertexDeclaration_Super(pcThis, pDecl);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetVertexDeclaration(IDirect3DDevice9* pcThis, IDirect3DVertexDeclaration9** ppDecl)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetVertexDeclaration");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppDecl;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetVertexDeclaration_Super(pcThis, ppDecl);
	}
	else nHr = D3D9_IDirect3DDevice9_GetVertexDeclaration_Super(pcThis, ppDecl);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetFVF(IDirect3DDevice9* pcThis, DWORD FVF)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetFVF");

	// set internal fvf
	m_unFVF = FVF;

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetFVF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetFVF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetFVF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&FVF;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetFVF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetFVF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetFVF_Super(pcThis, FVF);
	}
	else
		nHr = D3D9_IDirect3DDevice9_SetFVF_Super(pcThis, FVF);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetFVF(IDirect3DDevice9* pcThis, DWORD* pFVF)
{
	static HRESULT nHr = S_OK; OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetFVF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFVF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFVF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFVF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFVF;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFVF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetFVF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetFVF_Super(pcThis, pFVF);
	}
	else nHr = D3D9_IDirect3DDevice9_GetFVF_Super(pcThis, pFVF);
	return nHr;
}

/**
* Vertex Shader detour method, creates data sheet entry for new shader if workspace is present.
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateVertexShader(IDirect3DDevice9* pcThis, CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	static HRESULT nHr;

	// return if the method was called internally by a drawing operation (in case if no shader assigned and FVF is set)
	if ((m_bDrawingActive) && (!m_pcVShaderCurrent) && (m_unFVF))
	{
		nHr = D3D9_IDirect3DDevice9_CreateVertexShader_Super(pcThis, pFunction, ppShader);
		return nHr;
	}

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFunction;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DVertexShader9 = *ppShader;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreateVertexShader_Super(pcThis, pFunction, ppShader);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetVertexShader(IDirect3DDevice9* pcThis, IDirect3DVertexShader9* pShader)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetVertexShader");

	// set shader internally
	m_pcVShaderCurrent = pShader;

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else
			nHr = D3D9_IDirect3DDevice9_SetVertexShader_Super(pcThis, pShader);
	}
	else
		nHr = D3D9_IDirect3DDevice9_SetVertexShader_Super(pcThis, pShader);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetVertexShader(IDirect3DDevice9* pcThis, IDirect3DVertexShader9** ppShader)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetVertexShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetVertexShader_Super(pcThis, ppShader);
	}
	else nHr = D3D9_IDirect3DDevice9_GetVertexShader_Super(pcThis, ppShader);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetVertexShaderConstantF(IDirect3DDevice9* pcThis, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetVertexShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetVertexShaderConstantF(IDirect3DDevice9* pcThis, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetVertexShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetVertexShaderConstantI(IDirect3DDevice9* pcThis, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetVertexShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetVertexShaderConstantI(IDirect3DDevice9* pcThis, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetVertexShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetVertexShaderConstantB(IDirect3DDevice9* pcThis, UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetVertexShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetVertexShaderConstantB(IDirect3DDevice9* pcThis, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetVertexShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetVertexShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetStreamSource(IDirect3DDevice9* pcThis, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetStreamSource");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pStreamData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[2]->m_pOutput = (void*)&OffsetInBytes;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_paCommandersTemporary[3]->m_pOutput = (void*)&Stride;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSource]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetStreamSource_Super(pcThis, StreamNumber, pStreamData, OffsetInBytes, Stride);
	}
	else nHr = D3D9_IDirect3DDevice9_SetStreamSource_Super(pcThis, StreamNumber, pStreamData, OffsetInBytes, Stride);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetStreamSource(IDirect3DDevice9* pcThis, UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetStreamSource");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppStreamData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pOffsetInBytes;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_paCommandersTemporary[3]->m_pOutput = (void*)&pStride;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSource]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetStreamSource_Super(pcThis, StreamNumber, ppStreamData, pOffsetInBytes, pStride);
	}
	else nHr = D3D9_IDirect3DDevice9_GetStreamSource_Super(pcThis, StreamNumber, ppStreamData, pOffsetInBytes, pStride);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetStreamSourceFreq(IDirect3DDevice9* pcThis, UINT StreamNumber, UINT Setting)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetStreamSourceFreq");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_paCommandersTemporary[1]->m_pOutput = (void*)&Setting;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetStreamSourceFreq_Super(pcThis, StreamNumber, Setting);
	}
	else nHr = D3D9_IDirect3DDevice9_SetStreamSourceFreq_Super(pcThis, StreamNumber, Setting);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetStreamSourceFreq(IDirect3DDevice9* pcThis, UINT StreamNumber, UINT* pSetting)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetStreamSourceFreq");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StreamNumber;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pSetting;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetStreamSourceFreq_Super(pcThis, StreamNumber, pSetting);
	}
	else nHr = D3D9_IDirect3DDevice9_GetStreamSourceFreq_Super(pcThis, StreamNumber, pSetting);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetIndices(IDirect3DDevice9* pcThis, IDirect3DIndexBuffer9* pIndexData)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetIndices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetIndices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetIndices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetIndices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pIndexData;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetIndices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetIndices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetIndices_Super(pcThis, pIndexData);
	}
	else nHr = D3D9_IDirect3DDevice9_SetIndices_Super(pcThis, pIndexData);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetIndices(IDirect3DDevice9* pcThis, IDirect3DIndexBuffer9** ppIndexData)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetIndices");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetIndices]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetIndices]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetIndices]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppIndexData;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetIndices]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetIndices]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetIndices_Super(pcThis, ppIndexData);
	}
	else nHr = D3D9_IDirect3DDevice9_GetIndices_Super(pcThis, ppIndexData);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreatePixelShader(IDirect3DDevice9* pcThis, CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreatePixelShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pFunction;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreatePixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			// get return value.. MUST be STATIC !
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	// IDirect3DSwapChain9 reinstate
	if (m_pcTransferSite->m_pConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9] == AQU_InjectionTechniques::VMTable)
	{
		m_pcTransferSite->m_pIDirect3DPixelShader9 = *ppShader;
		CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
	}

	nHr = D3D9_IDirect3DDevice9_CreatePixelShader_Super(pcThis, pFunction, ppShader);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetPixelShader(IDirect3DDevice9* pcThis, IDirect3DPixelShader9* pShader)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetPixelShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&pShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
			// get return value.. MUST be STATIC !
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else
			nHr = D3D9_IDirect3DDevice9_SetPixelShader_Super(pcThis, pShader);
	}
	else
		nHr = D3D9_IDirect3DDevice9_SetPixelShader_Super(pcThis, pShader);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetPixelShader(IDirect3DDevice9* pcThis, IDirect3DPixelShader9** ppShader)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetPixelShader");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShader]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_paCommandersTemporary[0]->m_pOutput = (void*)&ppShader;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShader]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShader]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetPixelShader_Super(pcThis, ppShader);
	}
	else nHr = D3D9_IDirect3DDevice9_GetPixelShader_Super(pcThis, ppShader);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetPixelShaderConstantF(IDirect3DDevice9* pcThis, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetPixelShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetPixelShaderConstantF(IDirect3DDevice9* pcThis, UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetPixelShaderConstantF");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4fCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantF_Super(pcThis, StartRegister, pConstantData, Vector4fCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetPixelShaderConstantI(IDirect3DDevice9* pcThis, UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetPixelShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetPixelShaderConstantI(IDirect3DDevice9* pcThis, UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetPixelShaderConstantI");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_paCommandersTemporary[2]->m_pOutput = (void*)&Vector4iCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantI_Super(pcThis, StartRegister, pConstantData, Vector4iCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::SetPixelShaderConstantB(IDirect3DDevice9* pcThis, UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::SetPixelShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else nHr = D3D9_IDirect3DDevice9_SetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::GetPixelShaderConstantB(IDirect3DDevice9* pcThis, UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::GetPixelShaderConstantB");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[0]->m_pOutput = (void*)&StartRegister;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pConstantData;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_paCommandersTemporary[2]->m_pOutput = (void*)&BoolCount;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);
	}
	else nHr = D3D9_IDirect3DDevice9_GetPixelShaderConstantB_Super(pcThis, StartRegister, pConstantData, BoolCount);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawRectPatch(IDirect3DDevice9* pcThis, UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawRectPatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pNumSegs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pRectPatchInfo;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;


		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawRectPatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawRectPatch_Super(pcThis, Handle, pNumSegs, pRectPatchInfo);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawRectPatch_Super(pcThis, Handle, pNumSegs, pRectPatchInfo);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DrawTriPatch(IDirect3DDevice9* pcThis, UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DrawTriPatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[1]->m_pOutput = (void*)&pNumSegs;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_paCommandersTemporary[2]->m_pOutput = (void*)&pTriPatchInfo;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = false;
		m_bDrawingActive = true;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DrawTriPatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DrawTriPatch_Super(pcThis, Handle, pNumSegs, pTriPatchInfo);
	}
	else nHr = D3D9_IDirect3DDevice9_DrawTriPatch_Super(pcThis, Handle, pNumSegs, pTriPatchInfo);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::DeletePatch(IDirect3DDevice9* pcThis, UINT Handle)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::DeletePatch");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DeletePatch]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Handle;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DeletePatch]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::DeletePatch]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_DeletePatch_Super(pcThis, Handle);
	}
	else nHr = D3D9_IDirect3DDevice9_DeletePatch_Super(pcThis, Handle);

	return nHr;
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DDevice9::CreateQuery(IDirect3DDevice9* pcThis, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	static HRESULT nHr = S_OK;
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::CreateQuery");

	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]) && (m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_paCommandersTemporary[0]->m_pOutput = (void*)&Type;
		m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_paCommandersTemporary[1]->m_pOutput = (void*)&ppQuery;

		// provoke, set bForceD3D to "true" for any provoking circle
		m_pcTransferSite->m_bForceD3D = true;
		void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes);
		m_pcTransferSite->m_bForceD3D = m_bDrawingActive = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IDIRECT3DDEVICE9::CreateQuery]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
		else nHr = D3D9_IDirect3DDevice9_CreateQuery_Super(pcThis, Type, ppQuery);
	}
	else nHr = D3D9_IDirect3DDevice9_CreateQuery_Super(pcThis, Type, ppQuery);

	return nHr;
}

#pragma endregion

#pragma region DCL_IDirect3DDevice9 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DDevice9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DDevice9::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IUnknown super methods ***/
		case VMT_IUNKNOWN::QueryInterface:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_QueryInterface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::AddRef:                      *(PUINT_PTR)&D3D9_IDirect3DDevice9_AddRef_Super = (UINT_PTR)dwFunc; break;
		case VMT_IUNKNOWN::Release:                     *(PUINT_PTR)&D3D9_IDirect3DDevice9_Release_Super = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
};

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DDevice9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DDevice9::SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::VMT_IDirect3DDevice9 eFunc, UINT_PTR dwFunc)
{
	switch (eFunc)
	{
		/*** IDirect3DDevice9 super methods ***/
		case VMT_IDIRECT3DDEVICE9::TestCooperativeLevel:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_TestCooperativeLevel_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem:      *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetAvailableTextureMem_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::EvictManagedResources:       *(PUINT_PTR)&D3D9_IDirect3DDevice9_EvictManagedResources_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetDirect3D:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDirect3D_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetDeviceCaps:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDeviceCaps_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetDisplayMode:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDisplayMode_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetCreationParameters:       *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetCreationParameters_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetCursorProperties:         *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCursorProperties_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetCursorPosition:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCursorPosition_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::ShowCursor:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_ShowCursor_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain:   *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateAdditionalSwapChain_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetSwapChain:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSwapChain_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains:       *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetNumberOfSwapChains_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::Reset:                       *(PUINT_PTR)&D3D9_IDirect3DDevice9_Reset_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::Present:                     *(PUINT_PTR)&D3D9_IDirect3DDevice9_Present_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetBackBuffer:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetBackBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetRasterStatus:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRasterStatus_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetDialogBoxMode:            *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetDialogBoxMode_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetGammaRamp:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetGammaRamp_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetGammaRamp:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetGammaRamp_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateTexture:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateVolumeTexture:         *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVolumeTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateCubeTexture:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateCubeTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateVertexBuffer:          *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateIndexBuffer:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateIndexBuffer_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateRenderTarget:          *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateRenderTarget_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface:   *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateDepthStencilSurface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::UpdateSurface:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_UpdateSurface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::UpdateTexture:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_UpdateTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetRenderTargetData:         *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderTargetData_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetFrontBufferData:          *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetFrontBufferData_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::StretchRect:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_StretchRect_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::ColorFill:                   *(PUINT_PTR)&D3D9_IDirect3DDevice9_ColorFill_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface: *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetRenderTarget:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetRenderTarget_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetRenderTarget:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderTarget_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface:      *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetDepthStencilSurface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface:      *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDepthStencilSurface_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::BeginScene:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_BeginScene_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::EndScene:                    *(PUINT_PTR)&D3D9_IDirect3DDevice9_EndScene_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::Clear:                       *(PUINT_PTR)&D3D9_IDirect3DDevice9_Clear_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetTransform:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTransform_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetTransform:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTransform_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::MultiplyTransform:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_MultiplyTransform_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetViewport:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetViewport_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetViewport:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetViewport_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetMaterial:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetMaterial_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetMaterial:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetMaterial_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetLight:                    *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetLight_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetLight:                    *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetLight_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::LightEnable:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_LightEnable_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetLightEnable:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetLightEnable_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetClipPlane:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetClipPlane_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetClipPlane:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetClipPlane_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetRenderState:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetRenderState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetRenderState:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateStateBlock:            *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateStateBlock_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::BeginStateBlock:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_BeginStateBlock_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::EndStateBlock:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_EndStateBlock_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetClipStatus:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetClipStatus_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetClipStatus:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetClipStatus_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetTexture:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetTexture:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTexture_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetTextureStageState:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTextureStageState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetTextureStageState:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTextureStageState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetSamplerState:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSamplerState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetSamplerState:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetSamplerState_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::ValidateDevice:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_ValidateDevice_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetPaletteEntries:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPaletteEntries_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetPaletteEntries:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPaletteEntries_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCurrentTexturePalette_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetCurrentTexturePalette_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetScissorRect:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetScissorRect_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetScissorRect:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetScissorRect_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing: *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing: *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetNPatchMode:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetNPatchMode_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetNPatchMode:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetNPatchMode_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawPrimitive:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawPrimitive_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP:      *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::ProcessVertices:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_ProcessVertices_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexDeclaration_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetVertexDeclaration:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexDeclaration_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetVertexDeclaration:        *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexDeclaration_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetFVF:                      *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetFVF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetFVF:                      *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetFVF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateVertexShader:          *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetVertexShader:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetVertexShader:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantI_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantI_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantB_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB:    *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantB_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetStreamSource:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetStreamSource_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetStreamSource:             *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetStreamSource_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq:         *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetStreamSourceFreq_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq:         *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetStreamSourceFreq_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetIndices:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetIndices_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetIndices:                  *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetIndices_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreatePixelShader:           *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreatePixelShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetPixelShader:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetPixelShader:              *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShader_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantF_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantI_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantI_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantB_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB:     *(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantB_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawRectPatch:               *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawRectPatch_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DrawTriPatch:                *(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawTriPatch_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::DeletePatch:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_DeletePatch_Super = (UINT_PTR)dwFunc; break;
		case VMT_IDIRECT3DDEVICE9::CreateQuery:                 *(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateQuery_Super = (UINT_PTR)dwFunc; break;
	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDirect3DDevice9::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDirect3DDevice9 super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_TestCooperativeLevel_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetAvailableTextureMem_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_EvictManagedResources_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDirect3D_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDeviceCaps_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDisplayMode_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetCreationParameters_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCursorProperties_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCursorPosition_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_ShowCursor_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ShowCursor];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateAdditionalSwapChain_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSwapChain_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetNumberOfSwapChains_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_Reset_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Reset];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_Present_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Present];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetBackBuffer_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRasterStatus_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetDialogBoxMode_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetGammaRamp_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetGammaRamp_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVolumeTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateCubeTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexBuffer_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateIndexBuffer_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateRenderTarget_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateDepthStencilSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_UpdateSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_UpdateTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderTargetData_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetFrontBufferData_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_StretchRect_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::StretchRect];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_ColorFill_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ColorFill];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetRenderTarget_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderTarget_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetDepthStencilSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetDepthStencilSurface_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_BeginScene_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::BeginScene];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_EndScene_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EndScene];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_Clear_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::Clear];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTransform_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTransform];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTransform_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTransform];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_MultiplyTransform_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetViewport_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetViewport];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetViewport_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetViewport];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetMaterial_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetMaterial];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetMaterial_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetMaterial];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetLight_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetLight];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetLight_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetLight];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_LightEnable_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::LightEnable];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetLightEnable_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetClipPlane_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetClipPlane_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetRenderState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetRenderState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetRenderState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetRenderState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateStateBlock_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_BeginStateBlock_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_EndStateBlock_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetClipStatus_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetClipStatus_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTexture_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTexture];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetTextureStageState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetTextureStageState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSamplerState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetSamplerState_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_ValidateDevice_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPaletteEntries_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPaletteEntries_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetCurrentTexturePalette_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetCurrentTexturePalette_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetScissorRect_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetScissorRect_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetNPatchMode_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetNPatchMode_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawPrimitive_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_ProcessVertices_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexDeclaration_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexDeclaration_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexDeclaration_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetFVF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetFVF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetFVF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetFVF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateVertexShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantI_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantI_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetVertexShaderConstantB_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetVertexShaderConstantB_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetStreamSource_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetStreamSource_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetStreamSourceFreq_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetStreamSourceFreq_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetIndices_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetIndices];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetIndices_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetIndices];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreatePixelShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShader_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantF_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantI_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantI_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_SetPixelShaderConstantB_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_GetPixelShaderConstantB_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawRectPatch_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DrawTriPatch_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_DeletePatch_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::DeletePatch];
	*(PUINT_PTR)&D3D9_IDirect3DDevice9_CreateQuery_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DDEVICE9::CreateQuery];

	return S_OK;
}
#pragma endregion