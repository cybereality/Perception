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
#define OUTPUT_DEBUG_STRING_IDirect3DVolume9(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DVolume9(...)
#endif
#define AQU_IDIRECT3DVOLUME9_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDirect3DVolume9[a]) && (m_pcTransferSite->m_ppNOD_IDirect3DVolume9[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_IDIRECT3DVOLUME9_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_IDirect3DVolume9[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_IDIRECT3DVOLUME9_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_IDirect3DVolume9[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDirect3DVolume9[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }

#include"DCL_IDirect3DVolume9.h"
#include"DCL_IDirect3DVolume9_Super.h"

#pragma region DCL_IDirect3DVolume9 constructor/destructor

/**
*
***/
DCL_IDirect3DVolume9::DCL_IDirect3DVolume9(AQU_TransferSite* pcTransferSite) : 
	m_pcTransferSite(pcTransferSite)
{}

/**
*
***/
DCL_IDirect3DVolume9::~DCL_IDirect3DVolume9(){}

#pragma endregion

#pragma region IUnknown methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::QueryInterface(IDirect3DVolume9* pcThis, REFIID riid, void** ppvObj)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::QueryInterface");
	return D3D9_IDirect3DVolume9_QueryInterface_Super(pcThis, riid, ppvObj);
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DVolume9::AddRef(IDirect3DVolume9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::AddRef");
	
	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DVolume9_AddRef_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9] = iCount;
	return iCount;
}

/**
*
***/
ULONG WINAPI DCL_IDirect3DVolume9::Release(IDirect3DVolume9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::Release");
	
	// set reference counter in the transfer site class
	int iCount = D3D9_IDirect3DVolume9_Release_Super(pcThis);
	m_pcTransferSite->m_anInterfaceRefCount[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9] = iCount;
	return iCount;
}

#pragma endregion

#pragma region IDirect3DVolume9 methods

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::GetDevice(IDirect3DVolume9* pcThis,  IDirect3DDevice9** ppDevice)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::GetDevice");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::GetDevice);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetDevice,0,ppDevice);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::GetDevice);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::GetDevice,HRESULT);

	return D3D9_IDirect3DVolume9_GetDevice_Super(pcThis, ppDevice);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::SetPrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::SetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::SetPrivateData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::SetPrivateData,0,refguid);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::SetPrivateData,1,pData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::SetPrivateData,2,SizeOfData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::SetPrivateData,3,Flags);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::SetPrivateData);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::SetPrivateData,HRESULT);

	return D3D9_IDirect3DVolume9_SetPrivateData_Super(pcThis, refguid, pData, SizeOfData, Flags);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::GetPrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid,void* pData,DWORD* pSizeOfData)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::GetPrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::GetPrivateData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetPrivateData,0,refguid);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetPrivateData,1,pData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetPrivateData,2,pSizeOfData);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::GetPrivateData);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::GetPrivateData,HRESULT);

	return D3D9_IDirect3DVolume9_GetPrivateData_Super(pcThis, refguid, pData, pSizeOfData);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::FreePrivateData(IDirect3DVolume9* pcThis,  REFGUID refguid)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::FreePrivateData");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::FreePrivateData);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::FreePrivateData,0,refguid);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::FreePrivateData);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::FreePrivateData,HRESULT);

	return D3D9_IDirect3DVolume9_FreePrivateData_Super(pcThis, refguid);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::GetContainer(IDirect3DVolume9* pcThis,  REFIID riid, void** ppContainer)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::GetContainer");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::GetContainer);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetContainer,0,riid);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetContainer,1,ppContainer);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::GetContainer);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::GetContainer,HRESULT);

	return D3D9_IDirect3DVolume9_GetContainer_Super(pcThis, riid, ppContainer);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::GetDesc(IDirect3DVolume9* pcThis,  D3DVOLUME_DESC *pDesc)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::GetDesc");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::GetDesc);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::GetDesc,0,pDesc);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::GetDesc);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::GetDesc,HRESULT);

	return D3D9_IDirect3DVolume9_GetDesc_Super(pcThis, pDesc);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::LockBox(IDirect3DVolume9* pcThis,  D3DLOCKED_BOX * pLockedVolume,CONST D3DBOX* pBox,DWORD Flags)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::LockBox");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::LockBox);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::LockBox,0,pLockedVolume);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::LockBox,0,pBox);
	AQU_IDIRECT3DVOLUME9_SET_DATA______(VMT_IDIRECT3DVOLUME9::LockBox,0,Flags);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::LockBox);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::LockBox,HRESULT);

	return D3D9_IDirect3DVolume9_LockBox_Super(pcThis, pLockedVolume, pBox, Flags);
}

/**
*
***/
HRESULT WINAPI DCL_IDirect3DVolume9::UnlockBox(IDirect3DVolume9* pcThis)
{ 
	OUTPUT_DEBUG_STRING_IDirect3DVolume9(L"IDirect3DVolume9::UnlockBox");

	// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
	AQU_IDIRECT3DVOLUME9_HEADER________(VMT_IDIRECT3DVOLUME9::UnlockBox);
	AQU_IDIRECT3DVOLUME9_PROVOKE_______(VMT_IDIRECT3DVOLUME9::UnlockBox);
	AQU_IDIRECT3DVOLUME9_REPLACE_METHOD(VMT_IDIRECT3DVOLUME9::UnlockBox,HRESULT);

	return D3D9_IDirect3DVolume9_UnlockBox_Super(pcThis);
}

#pragma endregion

#pragma region DCL_IDirect3DVolume9 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DVolume9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DVolume9::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IUnknown super methods ***/
	case VMT_IUNKNOWN::QueryInterface: *(PUINT_PTR)&D3D9_IDirect3DVolume9_QueryInterface_Super = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::AddRef:         *(PUINT_PTR)&D3D9_IDirect3DVolume9_AddRef_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IUNKNOWN::Release:        *(PUINT_PTR)&D3D9_IDirect3DVolume9_Release_Super        = (UINT_PTR)dwFunc; break;
	}

	return S_OK;
};

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDirect3DVolume9.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDirect3DVolume9::SetSuperFunctionPointer(VMT_IDIRECT3DVOLUME9::VMT_IDirect3DVolume9 eFunc, UINT_PTR dwFunc)
{
	switch(eFunc)
	{
		/*** IDirect3DVolume9 super methods ***/
	case VMT_IDIRECT3DVOLUME9::GetDevice:       *(PUINT_PTR)&D3D9_IDirect3DVolume9_GetDevice_Super       = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::SetPrivateData:  *(PUINT_PTR)&D3D9_IDirect3DVolume9_SetPrivateData_Super  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::GetPrivateData:  *(PUINT_PTR)&D3D9_IDirect3DVolume9_GetPrivateData_Super  = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::FreePrivateData: *(PUINT_PTR)&D3D9_IDirect3DVolume9_FreePrivateData_Super = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::GetContainer:    *(PUINT_PTR)&D3D9_IDirect3DVolume9_GetContainer_Super    = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::GetDesc:         *(PUINT_PTR)&D3D9_IDirect3DVolume9_GetDesc_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::LockBox:         *(PUINT_PTR)&D3D9_IDirect3DVolume9_LockBox_Super         = (UINT_PTR)dwFunc; break;
	case VMT_IDIRECT3DVOLUME9::UnlockBox:       *(PUINT_PTR)&D3D9_IDirect3DVolume9_UnlockBox_Super       = (UINT_PTR)dwFunc; break;

	}
	return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDirect3DVolume9::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
	if (!pVMTable) return E_FAIL;

	/*** IUnknown super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_QueryInterface_Super  = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_AddRef_Super          = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_Release_Super         = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release];

	/*** IDirect3DVolume9 super methods ***/
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_GetDevice_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::GetDevice];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_SetPrivateData_Super  = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::SetPrivateData];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_GetPrivateData_Super  = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::GetPrivateData];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_FreePrivateData_Super = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::FreePrivateData];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_GetContainer_Super    = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::GetContainer];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_GetDesc_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::GetDesc];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_LockBox_Super         = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::LockBox];
	*(PUINT_PTR)&D3D9_IDirect3DVolume9_UnlockBox_Super       = (UINT_PTR)pVMTable[VMT_IDIRECT3DVOLUME9::UnlockBox];

	return S_OK;
}
#pragma endregion