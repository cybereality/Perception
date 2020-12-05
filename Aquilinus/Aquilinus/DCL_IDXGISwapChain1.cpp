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
#define OUTPUT_DEBUG_STRING_IDXGISwapChain1(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDXGISwapChain1(...)
#endif
#define AQU_IDXGISWAPCHAIN1_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]) && (m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]->m_cProvoker.m_paInvokers.size() > 0)) {
#define AQU_IDXGISWAPCHAIN1_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]->m_paCommandersTemporary[b]->m_pOutput = (void*)&c;
#define AQU_IDXGISWAPCHAIN1_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;
#define AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }
#define AQU_IDXGISWAPCHAIN1_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_IDXGISwapChain1[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }
#include"DCL_IDXGISwapChain1.h"
#include"DCL_IDXGISwapChain1_Super.h"

#pragma region DCL_IDXGISwapChain1 constructor/destructor
/**
* Constructor
***/
DCL_IDXGISwapChain1::DCL_IDXGISwapChain1(AQU_TransferSite* pcTransferSite) : 
m_pcTransferSite(pcTransferSite)
{ }
/**
* Destructor
***/
DCL_IDXGISwapChain1::~DCL_IDXGISwapChain1(){ }
#pragma endregion

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface           
***/
HRESULT WINAPI DCL_IDXGISwapChain1::QueryInterface           (IDXGISwapChain1* pcThis, REFIID riid, void** ppvObject)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::QueryInterface           ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IUNKNOWN::QueryInterface           );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IUNKNOWN::QueryInterface           ,0,riid);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IUNKNOWN::QueryInterface           ,1,ppvObject);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IUNKNOWN::QueryInterface           );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IUNKNOWN::QueryInterface           , HRESULT);

 return D3D10_IDXGISwapChain1_QueryInterface_Super(pcThis, riid, ppvObject);
}

/**
* ULONG WINAPI AddRef                   
***/
ULONG WINAPI DCL_IDXGISwapChain1::AddRef                   (IDXGISwapChain1* pcThis )
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::AddRef                   ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IUNKNOWN::AddRef                   );
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IUNKNOWN::AddRef                   );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IUNKNOWN::AddRef                   , ULONG);

 return D3D10_IDXGISwapChain1_AddRef_Super(pcThis);
}

/**
* ULONG WINAPI Release                  
***/
ULONG WINAPI DCL_IDXGISwapChain1::Release                  (IDXGISwapChain1* pcThis )
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::Release                  ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IUNKNOWN::Release                  );
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IUNKNOWN::Release                  );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IUNKNOWN::Release                  , ULONG);

 return D3D10_IDXGISwapChain1_Release_Super(pcThis);
}

#pragma endregion

#pragma region IDXGIObject methods
/**
* HRESULT WINAPI SetPrivateData           
***/
HRESULT WINAPI DCL_IDXGISwapChain1::SetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name, UINT DataSize, void* pData)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::SetPrivateData           ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGIOBJECT::SetPrivateData           );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData           ,0,Name);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData           ,1,DataSize);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateData           ,2,pData);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGIOBJECT::SetPrivateData           );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGIOBJECT::SetPrivateData           , HRESULT);

 return D3D10_IDXGISwapChain1_SetPrivateData_Super(pcThis, Name, DataSize, pData);
}

/**
* HRESULT WINAPI SetPrivateDataInterface  
***/
HRESULT WINAPI DCL_IDXGISwapChain1::SetPrivateDataInterface  (IDXGISwapChain1* pcThis, REFGUID Name, IUnknown* pUnknown)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::SetPrivateDataInterface  ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGIOBJECT::SetPrivateDataInterface  );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateDataInterface  ,0,Name);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::SetPrivateDataInterface  ,1,pUnknown);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGIOBJECT::SetPrivateDataInterface  );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGIOBJECT::SetPrivateDataInterface  , HRESULT);

 return D3D10_IDXGISwapChain1_SetPrivateDataInterface_Super(pcThis, Name, pUnknown);
}

/**
* HRESULT WINAPI GetPrivateData           
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetPrivateData           (IDXGISwapChain1* pcThis, REFGUID Name,UINT *pDataSize, void *pData )
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetPrivateData           ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGIOBJECT::GetPrivateData           );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData           ,0,Name);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData           ,1,pDataSize);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::GetPrivateData           ,2,pData);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGIOBJECT::GetPrivateData           );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGIOBJECT::GetPrivateData           , HRESULT);

 return D3D10_IDXGISwapChain1_GetPrivateData_Super(pcThis, Name,pDataSize, pData );
}

/**
* HRESULT WINAPI GetParent                
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetParent                (IDXGISwapChain1* pcThis, REFIID riid, void** ppParent)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetParent                ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGIOBJECT::GetParent                );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::GetParent                ,0,riid);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIOBJECT::GetParent                ,1,ppParent);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGIOBJECT::GetParent                );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGIOBJECT::GetParent                , HRESULT);

 return D3D10_IDXGISwapChain1_GetParent_Super(pcThis, riid, ppParent);
}

#pragma endregion

#pragma region IDXGIDeviceSubObject methods
/**
* HRESULT WINAPI GetDevice                
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetDevice                (IDXGISwapChain1* pcThis, REFIID riid, void** ppDevice)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetDevice                ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGIDEVICESUBOBJECT::GetDevice                );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIDEVICESUBOBJECT::GetDevice                ,0,riid);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGIDEVICESUBOBJECT::GetDevice                ,1,ppDevice);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGIDEVICESUBOBJECT::GetDevice                );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGIDEVICESUBOBJECT::GetDevice                , HRESULT);

 return D3D10_IDXGISwapChain1_GetDevice_Super(pcThis, riid, ppDevice);
}

#pragma endregion

#pragma region IDXGISwapChain methods
/**
* HRESULT WINAPI Present                  
***/
HRESULT WINAPI DCL_IDXGISwapChain1::Present                  (IDXGISwapChain1* pcThis, UINT SyncInterval, UINT Flags)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::Present                  ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::Present                  );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::Present                  ,0,SyncInterval);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::Present                  ,1,Flags);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::Present                  );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::Present                  , HRESULT);

 return D3D10_IDXGISwapChain1_Present_Super(pcThis, SyncInterval, Flags);
}

/**
* HRESULT WINAPI GetBuffer                
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetBuffer                (IDXGISwapChain1* pcThis, UINT Buffer, REFIID riid, void** ppSurface)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetBuffer                ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetBuffer                );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer                ,0,Buffer);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer                ,1,riid);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetBuffer                ,2,ppSurface);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetBuffer                );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetBuffer                , HRESULT);

 return D3D10_IDXGISwapChain1_GetBuffer_Super(pcThis, Buffer, riid, ppSurface);
}

/**
* HRESULT WINAPI SetFullscreenState       
***/
HRESULT WINAPI DCL_IDXGISwapChain1::SetFullscreenState       (IDXGISwapChain1* pcThis, BOOL Fullscreen, IDXGIOutput* pTarget)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::SetFullscreenState       ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::SetFullscreenState       );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::SetFullscreenState       ,0,Fullscreen);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::SetFullscreenState       ,1,pTarget);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::SetFullscreenState       );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::SetFullscreenState       , HRESULT);

 return D3D10_IDXGISwapChain1_SetFullscreenState_Super(pcThis, Fullscreen, pTarget);
}

/**
* HRESULT WINAPI GetFullscreenState       
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetFullscreenState       (IDXGISwapChain1* pcThis, BOOL* pFullscreen, IDXGIOutput** ppTarget)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetFullscreenState       ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetFullscreenState       );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFullscreenState       ,0,pFullscreen);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFullscreenState       ,1,ppTarget);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetFullscreenState       );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetFullscreenState       , HRESULT);

 return D3D10_IDXGISwapChain1_GetFullscreenState_Super(pcThis, pFullscreen, ppTarget);
}

/**
* HRESULT WINAPI GetDesc                  
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetDesc                  (IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC* pDesc)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetDesc                  ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetDesc                  );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetDesc                  ,0,pDesc);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetDesc                  );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetDesc                  , HRESULT);

 return D3D10_IDXGISwapChain1_GetDesc_Super(pcThis, pDesc);
}

/**
* HRESULT WINAPI ResizeBuffers            
***/
HRESULT WINAPI DCL_IDXGISwapChain1::ResizeBuffers            (IDXGISwapChain1* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::ResizeBuffers            ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::ResizeBuffers            );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,0,BufferCount);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,1,Width);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,2,Height);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,3,NewFormat);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeBuffers            ,4,SwapChainFlags);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::ResizeBuffers            );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::ResizeBuffers            , HRESULT);

 return D3D10_IDXGISwapChain1_ResizeBuffers_Super(pcThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

/**
* HRESULT WINAPI ResizeTarget             
***/
HRESULT WINAPI DCL_IDXGISwapChain1::ResizeTarget             (IDXGISwapChain1* pcThis, DXGI_MODE_DESC* pNewTargetParameters)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::ResizeTarget             ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::ResizeTarget             );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::ResizeTarget             ,0,pNewTargetParameters);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::ResizeTarget             );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::ResizeTarget             , HRESULT);

 return D3D10_IDXGISwapChain1_ResizeTarget_Super(pcThis, pNewTargetParameters);
}

/**
* HRESULT WINAPI GetContainingOutput      
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetContainingOutput      (IDXGISwapChain1* pcThis, IDXGIOutput** ppOutput)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetContainingOutput      ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetContainingOutput      );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetContainingOutput      ,0,ppOutput);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetContainingOutput      );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetContainingOutput      , HRESULT);

 return D3D10_IDXGISwapChain1_GetContainingOutput_Super(pcThis, ppOutput);
}

/**
* HRESULT WINAPI GetFrameStatistics       
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetFrameStatistics       (IDXGISwapChain1* pcThis, DXGI_FRAME_STATISTICS* pStats)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetFrameStatistics       ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetFrameStatistics       );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetFrameStatistics       ,0,pStats);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetFrameStatistics       );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetFrameStatistics       , HRESULT);

 return D3D10_IDXGISwapChain1_GetFrameStatistics_Super(pcThis, pStats);
}

/**
* HRESULT WINAPI GetLastPresentCount      
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetLastPresentCount      (IDXGISwapChain1* pcThis, UINT* pLastPresentCount)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetLastPresentCount      ");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN::GetLastPresentCount      );
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN::GetLastPresentCount      ,0,pLastPresentCount);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN::GetLastPresentCount      );
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN::GetLastPresentCount      , HRESULT);

 return D3D10_IDXGISwapChain1_GetLastPresentCount_Super(pcThis, pLastPresentCount);
}

#pragma endregion

#pragma region IDXGISwapChain1 methods
/**
* HRESULT WINAPI GetDesc1
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetDesc1(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_DESC1* pDesc)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetDesc1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetDesc1);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetDesc1,0,pDesc);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetDesc1);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetDesc1, HRESULT);

 return D3D10_IDXGISwapChain1_GetDesc1_Super(pcThis, pDesc);
}

/**
* HRESULT WINAPI GetFullscreenDesc
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetFullscreenDesc(IDXGISwapChain1* pcThis, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetFullscreenDesc");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetFullscreenDesc);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetFullscreenDesc,0,pDesc);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetFullscreenDesc);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetFullscreenDesc, HRESULT);

 return D3D10_IDXGISwapChain1_GetFullscreenDesc_Super(pcThis, pDesc);
}

/**
* HRESULT WINAPI GetHwnd
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetHwnd(IDXGISwapChain1* pcThis, HWND* pHwnd)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetHwnd");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetHwnd);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetHwnd,0,pHwnd);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetHwnd);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetHwnd, HRESULT);

 return D3D10_IDXGISwapChain1_GetHwnd_Super(pcThis, pHwnd);
}

/**
* HRESULT WINAPI GetCoreWindow
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetCoreWindow(IDXGISwapChain1* pcThis, REFIID refiid, void** ppUnk)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetCoreWindow");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetCoreWindow);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetCoreWindow,0,refiid);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetCoreWindow,1,ppUnk);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetCoreWindow);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetCoreWindow, HRESULT);

 return D3D10_IDXGISwapChain1_GetCoreWindow_Super(pcThis, refiid, ppUnk);
}

/**
* HRESULT WINAPI Present1
***/
HRESULT WINAPI DCL_IDXGISwapChain1::Present1(IDXGISwapChain1* pcThis, UINT SyncInterval, UINT PresentFlags, DXGI_PRESENT_PARAMETERS* pPresentParameters)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::Present1");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::Present1);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::Present1,0,SyncInterval);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::Present1,1,PresentFlags);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::Present1,2,pPresentParameters);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::Present1);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::Present1, HRESULT);

 return D3D10_IDXGISwapChain1_Present1_Super(pcThis, SyncInterval, PresentFlags, pPresentParameters);
}

/**
* BOOL WINAPI IsTemporaryMonoSupported
***/
BOOL WINAPI DCL_IDXGISwapChain1::IsTemporaryMonoSupported(IDXGISwapChain1* pcThis )
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::IsTemporaryMonoSupported");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported, BOOL);

 return D3D10_IDXGISwapChain1_IsTemporaryMonoSupported_Super(pcThis);
}

/**
* HRESULT WINAPI GetRestrictToOutput
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetRestrictToOutput(IDXGISwapChain1* pcThis, IDXGIOutput** ppRestrictToOutput)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetRestrictToOutput");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetRestrictToOutput);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetRestrictToOutput,0,ppRestrictToOutput);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetRestrictToOutput);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetRestrictToOutput, HRESULT);

 return D3D10_IDXGISwapChain1_GetRestrictToOutput_Super(pcThis, ppRestrictToOutput);
}

/**
* HRESULT WINAPI SetBackgroundColor
***/
HRESULT WINAPI DCL_IDXGISwapChain1::SetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::SetBackgroundColor");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::SetBackgroundColor);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::SetBackgroundColor,0,pColor);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::SetBackgroundColor);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::SetBackgroundColor, HRESULT);

 return D3D10_IDXGISwapChain1_SetBackgroundColor_Super(pcThis, pColor);
}

/**
* HRESULT WINAPI GetBackgroundColor
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetBackgroundColor(IDXGISwapChain1* pcThis, DXGI_RGBA* pColor)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetBackgroundColor");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetBackgroundColor);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetBackgroundColor,0,pColor);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetBackgroundColor);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetBackgroundColor, HRESULT);

 return D3D10_IDXGISwapChain1_GetBackgroundColor_Super(pcThis, pColor);
}

/**
* HRESULT WINAPI SetRotation
***/
HRESULT WINAPI DCL_IDXGISwapChain1::SetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION Rotation)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::SetRotation");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::SetRotation);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::SetRotation,0,Rotation);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::SetRotation);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::SetRotation, HRESULT);

 return D3D10_IDXGISwapChain1_SetRotation_Super(pcThis, Rotation);
}

/**
* HRESULT WINAPI GetRotation
***/
HRESULT WINAPI DCL_IDXGISwapChain1::GetRotation(IDXGISwapChain1* pcThis, DXGI_MODE_ROTATION* pRotation)
{
OUTPUT_DEBUG_STRING_IDXGISwapChain1(L"IDXGISwapChain1::GetRotation");

// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)
AQU_IDXGISWAPCHAIN1_HEADER________(VMT_IDXGISWAPCHAIN1::GetRotation);
AQU_IDXGISWAPCHAIN1_SET_DATA______(VMT_IDXGISWAPCHAIN1::GetRotation,0,pRotation);
AQU_IDXGISWAPCHAIN1_PROVOKE_______(VMT_IDXGISWAPCHAIN1::GetRotation);
AQU_IDXGISWAPCHAIN1_REPLACE_METHOD(VMT_IDXGISWAPCHAIN1::GetRotation, HRESULT);

 return D3D10_IDXGISwapChain1_GetRotation_Super(pcThis, pRotation);
}

#pragma endregion

#pragma region DCL_IDXGISwapChain1 public methods
/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointer(VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IUnknown super methods ***/
case VMT_IUNKNOWN::QueryInterface           :*(PUINT_PTR)&D3D10_IDXGISwapChain1_QueryInterface_Super = (UINT_PTR)dwFunc; break;
case VMT_IUNKNOWN::AddRef                   :*(PUINT_PTR)&D3D10_IDXGISwapChain1_AddRef_Super = (UINT_PTR)dwFunc; break;
case VMT_IUNKNOWN::Release                  :*(PUINT_PTR)&D3D10_IDXGISwapChain1_Release_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointer(VMT_IDXGIOBJECT::VMT_IDXGIObject eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IDXGIObject super methods ***/
case VMT_IDXGIOBJECT::SetPrivateData           :*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetPrivateData_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGIOBJECT::SetPrivateDataInterface  :*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetPrivateDataInterface_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGIOBJECT::GetPrivateData           :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetPrivateData_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGIOBJECT::GetParent                :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetParent_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointer(VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IDXGIDeviceSubObject super methods ***/
case VMT_IDXGIDEVICESUBOBJECT::GetDevice                :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDevice_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IDXGISwapChain super methods ***/
case VMT_IDXGISWAPCHAIN::Present                  :*(PUINT_PTR)&D3D10_IDXGISwapChain1_Present_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetBuffer                :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetBuffer_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::SetFullscreenState       :*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetFullscreenState_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetFullscreenState       :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFullscreenState_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetDesc                  :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDesc_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::ResizeBuffers            :*(PUINT_PTR)&D3D10_IDXGISwapChain1_ResizeBuffers_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::ResizeTarget             :*(PUINT_PTR)&D3D10_IDXGISwapChain1_ResizeTarget_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetContainingOutput      :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetContainingOutput_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetFrameStatistics       :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFrameStatistics_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN::GetLastPresentCount      :*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetLastPresentCount_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointer by provided index.
* @param eFunc The function as listed in VMT_IDXGISwapChain1.
* @param dwFunc The address of the super function as retourned by the detour method.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointer(VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1 eFunc, UINT_PTR dwFunc)
{
switch(eFunc)
{
/*** IDXGISwapChain1 super methods ***/
case VMT_IDXGISWAPCHAIN1::GetDesc1:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDesc1_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetFullscreenDesc:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFullscreenDesc_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetHwnd:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetHwnd_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetCoreWindow:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetCoreWindow_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::Present1:*(PUINT_PTR)&D3D10_IDXGISwapChain1_Present1_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported:*(PUINT_PTR)&D3D10_IDXGISwapChain1_IsTemporaryMonoSupported_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetRestrictToOutput:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetRestrictToOutput_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::SetBackgroundColor:*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetBackgroundColor_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetBackgroundColor:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetBackgroundColor_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::SetRotation:*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetRotation_Super = (UINT_PTR)dwFunc; break;
case VMT_IDXGISWAPCHAIN1::GetRotation:*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetRotation_Super = (UINT_PTR)dwFunc; break;
}

return S_OK;
}

/**
* Sets old function pointers by provided virtual methods table.
* @param pVMTable Pointer to the Virtual Methods Table.
***/
HRESULT DCL_IDXGISwapChain1::SetSuperFunctionPointers(PUINT_PTR pVMTable)
{
if (!pVMTable) return E_FAIL;

/*** IUnknown super methods ***/
*(PUINT_PTR)&D3D10_IDXGISwapChain1_QueryInterface_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::QueryInterface           ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_AddRef_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::AddRef                   ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_Release_Super = (UINT_PTR)pVMTable[VMT_IUNKNOWN::Release                  ];
/*** IDXGIObject super methods ***/
*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetPrivateData_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::SetPrivateData           ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetPrivateDataInterface_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::SetPrivateDataInterface  ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetPrivateData_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::GetPrivateData           ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetParent_Super = (UINT_PTR)pVMTable[VMT_IDXGIOBJECT::GetParent                ];
/*** IDXGIDeviceSubObject super methods ***/
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDevice_Super = (UINT_PTR)pVMTable[VMT_IDXGIDEVICESUBOBJECT::GetDevice                ];
/*** IDXGISwapChain super methods ***/
*(PUINT_PTR)&D3D10_IDXGISwapChain1_Present_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::Present                  ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetBuffer_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetBuffer                ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetFullscreenState_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::SetFullscreenState       ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFullscreenState_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetFullscreenState       ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDesc_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetDesc                  ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_ResizeBuffers_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::ResizeBuffers            ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_ResizeTarget_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::ResizeTarget             ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetContainingOutput_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetContainingOutput      ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFrameStatistics_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetFrameStatistics       ];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetLastPresentCount_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN::GetLastPresentCount      ];
/*** IDXGISwapChain1 super methods ***/
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetDesc1_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetDesc1];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetFullscreenDesc_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetFullscreenDesc];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetHwnd_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetHwnd];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetCoreWindow_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetCoreWindow];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_Present1_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::Present1];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_IsTemporaryMonoSupported_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetRestrictToOutput_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetRestrictToOutput];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetBackgroundColor_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::SetBackgroundColor];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetBackgroundColor_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetBackgroundColor];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_SetRotation_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::SetRotation];
*(PUINT_PTR)&D3D10_IDXGISwapChain1_GetRotation_Super = (UINT_PTR)pVMTable[VMT_IDXGISWAPCHAIN1::GetRotation];


return S_OK;
}

#pragma endregion