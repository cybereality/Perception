/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright � 2014 Denis Reischl

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
#define OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(...)
#endif
#define AQU_IDIRECT3DPIXELSHADER9_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DPixelShader9[a]) && (m_pcTransferSite->m_ppNOD_IDirect3DPixelShader9[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_IDIRECT3DPIXELSHADER9_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_IDirect3DPixelShader9[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_IDIRECT3DPIXELSHADER9_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DPixelShader9[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_IDIRECT3DPIXELSHADER9_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DPixelShader9[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }

#include"DCL_IDirect3DPixelShader9.h"
#include"DCL_IDirect3DPixelShader9_Super.h"

#pragma region DCL_IDirect3DPixelShader9 constructor/destructor

/**
*
***/
DCL_IDirect3DPixelShader9::DCL_IDirect3DPixelShader9(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{}

/**
*
***/
DCL_IDirect3DPixelShader9::~DCL_IDirect3DPixelShader9(){}

#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DPixelShader9::QueryInterface(IDirect3DPixelShader9* pcThis, REFIID riid, void** ppvObj)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(L"IDirect3DPixelShader9::QueryInterface");
	return D3D9_IDirect3DPixelShader9_QueryInterface_Super(pcThis, riid, ppvObj);
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DPixelShader9::AddRef(IDirect3DPixelShader9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(L"IDirect3DPixelShader9::AddRef");

	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DPixelShader9_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DPixelShader9::Release(IDirect3DPixelShader9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(L"IDirect3DPixelShader9::Release");

	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DPixelShader9_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDirect3DPixelShader9 methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DPixelShader9::GetDevice(IDirect3DPixelShader9* pcThis, IDirect3DDevice9** ppDevice)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(L"IDirect3DPixelShader9::GetDevice");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DPIXELSHADER9_HEADER________(VMT_IDIRECT3DPIXELSHADER9::GetDevice);
	AQU_IDIRECT3DPIXELSHADER9_SET_DATA______(VMT_IDIRECT3DPIXELSHADER9::GetDevice,0,ppDevice);
	AQU_IDIRECT3DPIXELSHADER9_PROVOKE_______(VMT_IDIRECT3DPIXELSHADER9::GetDevice);
	AQU_IDIRECT3DPIXELSHADER9_REPLACE_METHOD(VMT_IDIRECT3DPIXELSHADER9::GetDevice,HRESULT);

	return D3D9_IDirect3DPixelShader9_GetDevice_Super(pcThis, ppDevice);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DPixelShader9::GetFunction(IDirect3DPixelShader9* pcThis, void* pData, UINT* pSizeOfData)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DPixelShader9(L"IDirect3DPixelShader9::GetFunction");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DPIXELSHADER9_HEADER________(VMT_IDIRECT3DPIXELSHADER9::GetFunction);
	AQU_IDIRECT3DPIXELSHADER9_SET_DATA______(VMT_IDIRECT3DPIXELSHADER9::GetFunction,0,pData);
	AQU_IDIRECT3DPIXELSHADER9_SET_DATA______(VMT_IDIRECT3DPIXELSHADER9::GetFunction,1,pSizeOfData);
	AQU_IDIRECT3DPIXELSHADER9_PROVOKE_______(VMT_IDIRECT3DPIXELSHADER9::GetFunction);
	AQU_IDIRECT3DPIXELSHADER9_REPLACE_METHOD(VMT_IDIRECT3DPIXELSHADER9::GetFunction,HRESULT);

	return D3D9_IDirect3DPixelShader9_GetFunction_Super(pcThis, pData, pSizeOfData);
}

#pragma endregion

#pragma region DCL_IDirect3DPixelShader9 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DPixelShader9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DPixelShader9::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface: *(PUINT_PTR)&D3D9_IDirect3DPixelShader9_QueryInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:         *(PUINT_PTR)&D3D9_IDirect3DPixelShader9_AddRef_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:        *(PUINT_PTR)&D3D9_IDirect3DPixelShader9_Release_Super        = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
};

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DPixelShader9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DPixelShader9::SetSuperFunctionPointer(VMT_IDIRECT3DPIXELSHADER9::VMT_IDirect3DPixelShader9 eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IDirect3DPixelShader9 super methods ***/
	case VMT_IDIRECT3DPIXELSHADER9::GetDevice:   *(PUINT_PTR)&D3D9_IDirect3DPixelShader9_GetDevice_Super   = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DPIXELSHADER9::GetFunction: *(PUINT_PTR)&D3D9_IDirect3DPixelShader9_GetFunction_Super = (UINT_PTR)dwFunc; break;
	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDirect3DPixelShader9::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DPixelShader9_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D9_IDirect3DPixelShader9_AddRef_Super         = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D9_IDirect3DPixelShader9_Release_Super        = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDirect3DPixelShader9 super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DPixelShader9_GetDevice_Super      = (UINT_PTR)pVMTable[VMT_IDIRECT3DPIXELSHADER9::GetDevice];
	*(PUINT_PTR)&D3D9_IDirect3DPixelShader9_GetFunction_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DPIXELSHADER9::GetFunction];

	return S_OK;
}
#pragma endregion