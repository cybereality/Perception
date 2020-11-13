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
#include"AQU_Detour.h"

#ifndef DMT_IDIRECT3DQUERY9_TABLE
#define DMT_IDIRECT3DQUERY9_TABLE

/*** IUnknown methods ***/
HRESULT      WINAPI D3D9_IDirect3DQuery9_QueryInterface(IDirect3DQuery9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DQuery9->QueryInterface(pDevice, riid, ppvObj);}
ULONG        WINAPI D3D9_IDirect3DQuery9_AddRef(IDirect3DQuery9* pDevice){ return pDCL_IDirect3DQuery9->AddRef(pDevice);}
ULONG        WINAPI D3D9_IDirect3DQuery9_Release(IDirect3DQuery9* pDevice){ return pDCL_IDirect3DQuery9->Release(pDevice);}

/*** IDirect3DQuery9 methods ***/
HRESULT      WINAPI D3D9_IDirect3DQuery9_GetDevice(IDirect3DQuery9* pcThis, IDirect3DDevice9** ppDevice){ return pDCL_IDirect3DQuery9->GetDevice(pcThis, ppDevice);}
D3DQUERYTYPE WINAPI D3D9_IDirect3DQuery9_GetType(IDirect3DQuery9* pcThis){ return pDCL_IDirect3DQuery9->GetType(pcThis);}
DWORD        WINAPI D3D9_IDirect3DQuery9_GetDataSize(IDirect3DQuery9* pcThis){ return pDCL_IDirect3DQuery9->GetDataSize(pcThis);}
HRESULT      WINAPI D3D9_IDirect3DQuery9_Issue(IDirect3DQuery9* pcThis, DWORD dwIssueFlags){ return pDCL_IDirect3DQuery9->Issue(pcThis, dwIssueFlags);}
HRESULT      WINAPI D3D9_IDirect3DQuery9_GetData(IDirect3DQuery9* pcThis, void* pData,DWORD dwSize,DWORD dwGetDataFlags){ return pDCL_IDirect3DQuery9->GetData(pcThis, pData, dwSize, dwGetDataFlags);}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DQuery9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::QueryInterface],     (UINT_PTR)D3D9_IDirect3DQuery9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::AddRef],             (UINT_PTR)D3D9_IDirect3DQuery9_AddRef);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::Release],            (UINT_PTR)D3D9_IDirect3DQuery9_Release);

	/*** IDirect3DQuery9 super methods ***/
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDevice],   (UINT_PTR)D3D9_IDirect3DQuery9_GetDevice);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetType],     (UINT_PTR)D3D9_IDirect3DQuery9_GetType);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDataSize], (UINT_PTR)D3D9_IDirect3DQuery9_GetDataSize);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::Issue],       (UINT_PTR)D3D9_IDirect3DQuery9_Issue);
	OverrideVTable(&D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetData],     (UINT_PTR)D3D9_IDirect3DQuery9_GetData);
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DQuery9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::QueryInterface] =     (UINT_PTR)D3D9_IDirect3DQuery9_QueryInterface;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::AddRef] =             (UINT_PTR)D3D9_IDirect3DQuery9_AddRef;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::Release] =            (UINT_PTR)D3D9_IDirect3DQuery9_Release;

	/*** IDirect3DQuery9 super methods ***/
	anD3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDevice] =   (UINT_PTR)D3D9_IDirect3DQuery9_GetDevice;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetType] =     (UINT_PTR)D3D9_IDirect3DQuery9_GetType;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDataSize] = (UINT_PTR)D3D9_IDirect3DQuery9_GetDataSize;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::Issue] =       (UINT_PTR)D3D9_IDirect3DQuery9_Issue;
	anD3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetData] =     (UINT_PTR)D3D9_IDirect3DQuery9_GetData;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DQuery9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DQuery9_VMTable[0],
		(void*)&anD3D9_IDirect3DQuery9_VMTable[0],
		(D3D9_IDIRECT3DQUERY9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DQuery9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::QueryInterface],     (PBYTE)D3D9_IDirect3DQuery9_QueryInterface, JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::AddRef],             (PBYTE)D3D9_IDirect3DQuery9_AddRef,         JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IUNKNOWN::Release],            (PBYTE)D3D9_IDirect3DQuery9_Release,        JMP32_SZ));

	/*** IDirect3DQuery9 super methods ***/
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IDIRECT3DQUERY9::GetDevice,   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDevice],   (PBYTE)D3D9_IDirect3DQuery9_GetDevice,      JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IDIRECT3DQUERY9::GetType,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetType],     (PBYTE)D3D9_IDirect3DQuery9_GetType,        JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IDIRECT3DQUERY9::GetDataSize, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetDataSize], (PBYTE)D3D9_IDirect3DQuery9_GetDataSize,    JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IDIRECT3DQUERY9::Issue,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::Issue],       (PBYTE)D3D9_IDirect3DQuery9_Issue,          JMP32_SZ));
	pDCL_IDirect3DQuery9->SetSuperFunctionPointer(VMT_IDIRECT3DQUERY9::GetData,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DQuery9_VMTable[VMT_IDIRECT3DQUERY9::GetData],     (PBYTE)D3D9_IDirect3DQuery9_GetData,        JMP32_SZ));
}


#endif