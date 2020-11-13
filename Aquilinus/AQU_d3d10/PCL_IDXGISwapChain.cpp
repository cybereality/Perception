/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_IDXGISwapChain.cpp> and
Class <PCL_IDXGISwapChain> :
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
****************************************************************/

#include "PCL_IDXGISwapChain.h"

/**
* Constructor.
***/
PCL_IDXGISwapChain::PCL_IDXGISwapChain(IDXGISwapChain* pIDXGISwapChain) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PCL_IDXGISwapChain");
#endif

	m_pIDXGISwapChain = pIDXGISwapChain;
}

/**
* Empty deconstructor.
***/
PCL_IDXGISwapChain::~PCL_IDXGISwapChain(void) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("~PCL_IDXGISwapChain");
#endif

}

#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))
/**
* Wrapped QueryInterface functionality.
***/
HRESULT PCL_IDXGISwapChain::QueryInterface(REFIID riid, void **ppvObject) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("QueryInterface");
#endif

	return m_pIDXGISwapChain->QueryInterface(riid, ppvObject);
}

/**
*
***/
ULONG PCL_IDXGISwapChain::AddRef(void) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("AddRef");
#endif

	return m_pIDXGISwapChain->AddRef(); 
}

/**
*
***/
ULONG PCL_IDXGISwapChain::Release(void) 
{ 
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Release");
#endif

	extern PCL_IDXGISwapChain* g_pPCL_IDXGISwapChain;

	ULONG count = m_pIDXGISwapChain->Release();

	if (count == 0)
	{
		g_pPCL_IDXGISwapChain = NULL;
		delete this;
	}

	return count;
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::SetPrivateData(REFGUID Name, UINT DataSize, const void *pData)
{ 
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateData");
#endif

	return m_pIDXGISwapChain->SetPrivateData(Name, DataSize, pData);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateDataInterface");
#endif

	return m_pIDXGISwapChain->SetPrivateDataInterface(Name, pUnknown);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetPrivateData(REFGUID Name,UINT *pDataSize, void *pData) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetPrivateData");
#endif

	return m_pIDXGISwapChain->GetPrivateData(Name, pDataSize, pData);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetParent(REFIID riid, void **ppParent) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetParent");
#endif

	return m_pIDXGISwapChain->GetParent(riid, ppParent);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetDevice(REFIID riid, void **ppDevice) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetDevice");
#endif

	return m_pIDXGISwapChain->GetDevice(riid, ppDevice);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::Present(UINT SyncInterval, UINT Flags) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Present");
#endif

	return m_pIDXGISwapChain->Present(SyncInterval, Flags);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetBuffer(UINT Buffer, REFIID riid, void **ppSurface) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetBuffer");
#endif

	return m_pIDXGISwapChain->GetBuffer(Buffer, riid, ppSurface);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::SetFullscreenState(BOOL Fullscreen, IDXGIOutput *pTarget) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetFullscreenState");
#endif

	return m_pIDXGISwapChain->SetFullscreenState(Fullscreen, pTarget);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetFullscreenState(BOOL *pFullscreen, IDXGIOutput **ppTarget) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetFullscreenState");
#endif

	return m_pIDXGISwapChain->GetFullscreenState(pFullscreen, ppTarget);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC *pDesc) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetDesc");
#endif

	return m_pIDXGISwapChain->GetDesc(pDesc);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ResizeBuffers");
#endif

	return m_pIDXGISwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::ResizeTarget(const DXGI_MODE_DESC *pNewTargetParameters) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ResizeTarget");
#endif

	return m_pIDXGISwapChain->ResizeTarget(pNewTargetParameters);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetContainingOutput(IDXGIOutput **ppOutput) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetContainingOutput");
#endif

	return m_pIDXGISwapChain->GetContainingOutput(ppOutput);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetFrameStatistics(DXGI_FRAME_STATISTICS *pStats) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetFrameStatistics");
#endif

	return m_pIDXGISwapChain->GetFrameStatistics(pStats);
}

/**
*
***/
HRESULT PCL_IDXGISwapChain::GetLastPresentCount(UINT *pLastPresentCount) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetLastPresentCount");
#endif

	return m_pIDXGISwapChain->GetLastPresentCount(pLastPresentCount);
}