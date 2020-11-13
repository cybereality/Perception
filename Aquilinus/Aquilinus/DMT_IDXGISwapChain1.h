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
#ifndef DMT_IDXGISWAPCHAIN1_TABLE
#define DMT_IDXGISWAPCHAIN1_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain1_QueryInterface           (IDXGISwapChain1* pcThis, REFIID riid, void** ppvObject) { return pDCL_IDXGISwapChain1->QueryInterface           (pcThis, riid, ppvObject);}
ULONG WINAPI D3D10_IDXGISwapChain1_AddRef                   (IDXGISwapChain1* pcThis ) { return pDCL_IDXGISwapChain1->AddRef                   (pcThis);}
ULONG WINAPI D3D10_IDXGISwapChain1_Release                  (IDXGISwapChain1* pcThis ) { return pDCL_IDXGISwapChain1->Release                  (pcThis);}

/*** IDXGIObject methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain1_SetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name, UINT DataSize, void* pData) { return pDCL_IDXGISwapChain1->SetPrivateData           (pcThis, Name, DataSize, pData);}
HRESULT WINAPI D3D10_IDXGISwapChain1_SetPrivateDataInterface  (IDXGISwapChain1* pcThis, REFGUID Name, IUnknown* pUnknown) { return pDCL_IDXGISwapChain1->SetPrivateDataInterface  (pcThis, Name, pUnknown);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name, UINT *pDataSize, void *pData ) { return pDCL_IDXGISwapChain1->GetPrivateData           (pcThis, Name , pDataSize, pData);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetParent                (IDXGISwapChain1* pcThis, REFIID riid, void** ppParent) { return pDCL_IDXGISwapChain1->GetParent                (pcThis, riid, ppParent);}

/*** IDXGIDeviceSubObject methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain1_GetDevice                (IDXGISwapChain1* pcThis, REFIID riid, void** ppDevice) { return pDCL_IDXGISwapChain1->GetDevice                (pcThis, riid, ppDevice);}

/*** IDXGISwapChain methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain1_Present                  (IDXGISwapChain1* pcThis, UINT SyncInterval, UINT Flags) { return pDCL_IDXGISwapChain1->Present                  (pcThis, SyncInterval, Flags);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetBuffer                (IDXGISwapChain1* pcThis, UINT Buffer, REFIID riid, void** ppSurface) { return pDCL_IDXGISwapChain1->GetBuffer                (pcThis, Buffer, riid, ppSurface);}
HRESULT WINAPI D3D10_IDXGISwapChain1_SetFullscreenState       (IDXGISwapChain1* pcThis, BOOL Fullscreen, IDXGIOutput* pTarget) { return pDCL_IDXGISwapChain1->SetFullscreenState       (pcThis, Fullscreen, pTarget);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetFullscreenState       (IDXGISwapChain1* pcThis, BOOL* pFullscreen, IDXGIOutput** ppTarget) { return pDCL_IDXGISwapChain1->GetFullscreenState       (pcThis, pFullscreen, ppTarget);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetDesc                  (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC* pDesc) { return pDCL_IDXGISwapChain1->GetDesc                  (pcThis, pDesc);}
HRESULT WINAPI D3D10_IDXGISwapChain1_ResizeBuffers            (IDXGISwapChain1* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) { return pDCL_IDXGISwapChain1->ResizeBuffers            (pcThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);}
HRESULT WINAPI D3D10_IDXGISwapChain1_ResizeTarget             (IDXGISwapChain1* pcThis, DXGI_MODE_DESC* pNewTargetParameters) { return pDCL_IDXGISwapChain1->ResizeTarget             (pcThis, pNewTargetParameters);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetContainingOutput      (IDXGISwapChain1* pcThis, IDXGIOutput** ppOutput) { return pDCL_IDXGISwapChain1->GetContainingOutput      (pcThis, ppOutput);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetFrameStatistics       (IDXGISwapChain1* pcThis, DXGI_FRAME_STATISTICS* pStats) { return pDCL_IDXGISwapChain1->GetFrameStatistics       (pcThis, pStats);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetLastPresentCount      (IDXGISwapChain1* pcThis, UINT* pLastPresentCount) { return pDCL_IDXGISwapChain1->GetLastPresentCount      (pcThis, pLastPresentCount);}

/*** IDXGISwapChain1 methods ***/
HRESULT WINAPI D3D10_IDXGISwapChain1_GetDesc1(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC1* pDesc) { return pDCL_IDXGISwapChain1->GetDesc1(pcThis, pDesc);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetFullscreenDesc(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc) { return pDCL_IDXGISwapChain1->GetFullscreenDesc(pcThis, pDesc);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetHwnd(IDXGISwapChain1* pcThis, HWND* pHwnd) { return pDCL_IDXGISwapChain1->GetHwnd(pcThis, pHwnd);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetCoreWindow(IDXGISwapChain1* pcThis, REFIID refiid, void** ppUnk) { return pDCL_IDXGISwapChain1->GetCoreWindow(pcThis, refiid, ppUnk);}
HRESULT WINAPI D3D10_IDXGISwapChain1_Present1(IDXGISwapChain1* pcThis, UINT SyncInterval, UINT PresentFlags, DXGI_PRESENT_PARAMETERS* pPresentParameters) { return pDCL_IDXGISwapChain1->Present1(pcThis, SyncInterval, PresentFlags, pPresentParameters);}
BOOL WINAPI D3D10_IDXGISwapChain1_IsTemporaryMonoSupported(IDXGISwapChain1* pcThis ) { return pDCL_IDXGISwapChain1->IsTemporaryMonoSupported(pcThis);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetRestrictToOutput(IDXGISwapChain1* pcThis, IDXGIOutput** ppRestrictToOutput) { return pDCL_IDXGISwapChain1->GetRestrictToOutput(pcThis, ppRestrictToOutput);}
HRESULT WINAPI D3D10_IDXGISwapChain1_SetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor) { return pDCL_IDXGISwapChain1->SetBackgroundColor(pcThis, pColor);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor) { return pDCL_IDXGISwapChain1->GetBackgroundColor(pcThis, pColor);}
HRESULT WINAPI D3D10_IDXGISwapChain1_SetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION Rotation) { return pDCL_IDXGISwapChain1->SetRotation(pcThis, Rotation);}
HRESULT WINAPI D3D10_IDXGISwapChain1_GetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION* pRotation) { return pDCL_IDXGISwapChain1->GetRotation(pcThis, pRotation);}


/**
* Overrides the Virtual Methods Table of IDXGISwapChain1
***/
void Override_D3D10_IDXGISwapChain1_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::QueryInterface           ], (UINT_PTR)D3D10_IDXGISwapChain1_QueryInterface           );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::AddRef                   ], (UINT_PTR)D3D10_IDXGISwapChain1_AddRef                   );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::Release                  ], (UINT_PTR)D3D10_IDXGISwapChain1_Release                  );

	/*** IDXGIObject super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateData           ], (UINT_PTR)D3D10_IDXGISwapChain1_SetPrivateData           );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface  ], (UINT_PTR)D3D10_IDXGISwapChain1_SetPrivateDataInterface  );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetPrivateData           ], (UINT_PTR)D3D10_IDXGISwapChain1_GetPrivateData           );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetParent                ], (UINT_PTR)D3D10_IDXGISwapChain1_GetParent                );

	/*** IDXGIDeviceSubObject super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice                ], (UINT_PTR)D3D10_IDXGISwapChain1_GetDevice                );

	/*** IDXGISwapChain super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::Present                  ], (UINT_PTR)D3D10_IDXGISwapChain1_Present                  );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetBuffer                ], (UINT_PTR)D3D10_IDXGISwapChain1_GetBuffer                );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState       ], (UINT_PTR)D3D10_IDXGISwapChain1_SetFullscreenState       );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState       ], (UINT_PTR)D3D10_IDXGISwapChain1_GetFullscreenState       );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetDesc                  ], (UINT_PTR)D3D10_IDXGISwapChain1_GetDesc                  );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers            ], (UINT_PTR)D3D10_IDXGISwapChain1_ResizeBuffers            );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeTarget             ], (UINT_PTR)D3D10_IDXGISwapChain1_ResizeTarget             );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput      ], (UINT_PTR)D3D10_IDXGISwapChain1_GetContainingOutput      );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics       ], (UINT_PTR)D3D10_IDXGISwapChain1_GetFrameStatistics       );
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount      ], (UINT_PTR)D3D10_IDXGISwapChain1_GetLastPresentCount      );

	/*** IDXGISwapChain1 super methods ***/
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetDesc1], (UINT_PTR)D3D10_IDXGISwapChain1_GetDesc1);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetFullscreenDesc], (UINT_PTR)D3D10_IDXGISwapChain1_GetFullscreenDesc);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetHwnd], (UINT_PTR)D3D10_IDXGISwapChain1_GetHwnd);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetCoreWindow], (UINT_PTR)D3D10_IDXGISwapChain1_GetCoreWindow);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::Present1], (UINT_PTR)D3D10_IDXGISwapChain1_Present1);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported], (UINT_PTR)D3D10_IDXGISwapChain1_IsTemporaryMonoSupported);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRestrictToOutput], (UINT_PTR)D3D10_IDXGISwapChain1_GetRestrictToOutput);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetBackgroundColor], (UINT_PTR)D3D10_IDXGISwapChain1_SetBackgroundColor);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetBackgroundColor], (UINT_PTR)D3D10_IDXGISwapChain1_GetBackgroundColor);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetRotation], (UINT_PTR)D3D10_IDXGISwapChain1_SetRotation);
	OverrideVTable(&D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRotation], (UINT_PTR)D3D10_IDXGISwapChain1_GetRotation);
}

/**
* Creates a Virtual Methods Table array of IDXGISwapChain1
***/
void Generate_D3D10_IDXGISwapChain1_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::QueryInterface           ] =  (UINT_PTR)D3D10_IDXGISwapChain1_QueryInterface           ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::AddRef                   ] =  (UINT_PTR)D3D10_IDXGISwapChain1_AddRef                   ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::Release                  ] =  (UINT_PTR)D3D10_IDXGISwapChain1_Release                  ;

	/*** IDXGIObject super methods ***/
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateData           ] =  (UINT_PTR)D3D10_IDXGISwapChain1_SetPrivateData           ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface  ] =  (UINT_PTR)D3D10_IDXGISwapChain1_SetPrivateDataInterface  ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetPrivateData           ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetPrivateData           ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetParent                ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetParent                ;

	/*** IDXGIDeviceSubObject super methods ***/
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice                ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetDevice                ;

	/*** IDXGISwapChain super methods ***/
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::Present                  ] =  (UINT_PTR)D3D10_IDXGISwapChain1_Present                  ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetBuffer                ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetBuffer                ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState       ] =  (UINT_PTR)D3D10_IDXGISwapChain1_SetFullscreenState       ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState       ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetFullscreenState       ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetDesc                  ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetDesc                  ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers            ] =  (UINT_PTR)D3D10_IDXGISwapChain1_ResizeBuffers            ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeTarget             ] =  (UINT_PTR)D3D10_IDXGISwapChain1_ResizeTarget             ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput      ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetContainingOutput      ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics       ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetFrameStatistics       ;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount      ] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetLastPresentCount      ;

	/*** IDXGISwapChain1 super methods ***/
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetDesc1] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetDesc1;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetFullscreenDesc] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetFullscreenDesc;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetHwnd] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetHwnd;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetCoreWindow] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetCoreWindow;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::Present1] =  (UINT_PTR)D3D10_IDXGISwapChain1_Present1;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported] =  (UINT_PTR)D3D10_IDXGISwapChain1_IsTemporaryMonoSupported;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRestrictToOutput] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetRestrictToOutput;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetBackgroundColor] =  (UINT_PTR)D3D10_IDXGISwapChain1_SetBackgroundColor;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetBackgroundColor] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetBackgroundColor;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetRotation] =  (UINT_PTR)D3D10_IDXGISwapChain1_SetRotation;
	anD3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRotation] =  (UINT_PTR)D3D10_IDXGISwapChain1_GetRotation;
}

/**
* Overrides the Virtual Methods Table of IDXGISwapChain1 using a generated array.
***/
void Override_D3D10_IDXGISwapChain1_VMTable_by_Array()
{
	OverrideFullVTable(&D3D10_IDXGISwapChain1_VMTable[0],
		(void*)&anD3D10_IDXGISwapChain1_VMTable[0],
		(DXGI_IDXGISWAPCHAIN1_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of IDXGISwapChain1
***/
void Detour_D3D10_IDXGISwapChain1VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::QueryInterface           ],  (PBYTE)D3D10_IDXGISwapChain1_QueryInterface           , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef                   ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::AddRef                   ],  (PBYTE)D3D10_IDXGISwapChain1_AddRef                   , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IUNKNOWN::Release                  ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IUNKNOWN::Release                  ],  (PBYTE)D3D10_IDXGISwapChain1_Release                  , JMP32_SZ));

	/*** IDXGIObject super methods ***/
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGIOBJECT::SetPrivateData           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateData           ],  (PBYTE)D3D10_IDXGISwapChain1_SetPrivateData           , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGIOBJECT::SetPrivateDataInterface  ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface  ],  (PBYTE)D3D10_IDXGISwapChain1_SetPrivateDataInterface  , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGIOBJECT::GetPrivateData           ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetPrivateData           ],  (PBYTE)D3D10_IDXGISwapChain1_GetPrivateData           , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGIOBJECT::GetParent                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIOBJECT::GetParent                ],  (PBYTE)D3D10_IDXGISwapChain1_GetParent                , JMP32_SZ));

	/*** IDXGIDeviceSubObject super methods ***/
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGIDEVICESUBOBJECT::GetDevice                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice                ],  (PBYTE)D3D10_IDXGISwapChain1_GetDevice                , JMP32_SZ));

	/*** IDXGISwapChain super methods ***/
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::Present                  ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::Present                  ],  (PBYTE)D3D10_IDXGISwapChain1_Present                  , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetBuffer                ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetBuffer                ],  (PBYTE)D3D10_IDXGISwapChain1_GetBuffer                , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::SetFullscreenState       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState       ],  (PBYTE)D3D10_IDXGISwapChain1_SetFullscreenState       , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetFullscreenState       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState       ],  (PBYTE)D3D10_IDXGISwapChain1_GetFullscreenState       , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetDesc                  ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetDesc                  ],  (PBYTE)D3D10_IDXGISwapChain1_GetDesc                  , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers            ],  (PBYTE)D3D10_IDXGISwapChain1_ResizeBuffers            , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::ResizeTarget             ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::ResizeTarget             ],  (PBYTE)D3D10_IDXGISwapChain1_ResizeTarget             , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetContainingOutput      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput      ],  (PBYTE)D3D10_IDXGISwapChain1_GetContainingOutput      , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetFrameStatistics       ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics       ],  (PBYTE)D3D10_IDXGISwapChain1_GetFrameStatistics       , JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::GetLastPresentCount      ,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount      ],  (PBYTE)D3D10_IDXGISwapChain1_GetLastPresentCount      , JMP32_SZ));

	/*** IDXGISwapChain1 super methods ***/
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetDesc1,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetDesc1],  (PBYTE)D3D10_IDXGISwapChain1_GetDesc1, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetFullscreenDesc,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetFullscreenDesc],  (PBYTE)D3D10_IDXGISwapChain1_GetFullscreenDesc, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetHwnd,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetHwnd],  (PBYTE)D3D10_IDXGISwapChain1_GetHwnd, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetCoreWindow,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetCoreWindow],  (PBYTE)D3D10_IDXGISwapChain1_GetCoreWindow, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::Present1,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::Present1],  (PBYTE)D3D10_IDXGISwapChain1_Present1, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported],  (PBYTE)D3D10_IDXGISwapChain1_IsTemporaryMonoSupported, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetRestrictToOutput,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRestrictToOutput],  (PBYTE)D3D10_IDXGISwapChain1_GetRestrictToOutput, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::SetBackgroundColor,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetBackgroundColor],  (PBYTE)D3D10_IDXGISwapChain1_SetBackgroundColor, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetBackgroundColor,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetBackgroundColor],  (PBYTE)D3D10_IDXGISwapChain1_GetBackgroundColor, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::SetRotation,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::SetRotation],  (PBYTE)D3D10_IDXGISwapChain1_SetRotation, JMP32_SZ));
	pDCL_IDXGISwapChain1->SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::GetRotation,  (UINT_PTR)DetourFunc((PBYTE)D3D10_IDXGISwapChain1_VMTable[VMT_IDXGISWAPCHAIN1::GetRotation],  (PBYTE)D3D10_IDXGISwapChain1_GetRotation, JMP32_SZ));
}

#endif