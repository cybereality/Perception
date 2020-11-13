/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_IDXGISwapChain.h> and
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

#ifndef PCL_IDXGISWAPCHAIN_CLASS
#define PCL_IDXGISWAPCHAIN_CLASS

#include <dxgi.h>
//#include <stdio.h>

/**
* Wrapped IDXGISwapChain class, or proxy class.
***/
class PCL_IDXGISwapChain : public IDXGISwapChain
{
public:
	PCL_IDXGISwapChain(IDXGISwapChain* pIDXGISwapChain);
	virtual ~PCL_IDXGISwapChain(void);

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface           (REFIID riid, void **ppvObject);
	ULONG   WINAPI AddRef                   (void);
	ULONG   WINAPI Release                  (void);

	/*** IDXGIObject methods ***/
	HRESULT WINAPI SetPrivateData           (REFGUID Name, UINT DataSize, const void *pData);
	HRESULT WINAPI SetPrivateDataInterface  (REFGUID Name, const IUnknown *pUnknown);
	HRESULT WINAPI GetPrivateData           (REFGUID Name,UINT *pDataSize, void *pData);
	HRESULT WINAPI GetParent                (REFIID riid, void **ppParent);

	/*** IDXGIDeviceSubObject methods ***/
	HRESULT WINAPI GetDevice                (REFIID riid, void **ppDevice);

	/*** IDXGISwapChain methods ***/
	HRESULT WINAPI Present                  (UINT SyncInterval, UINT Flags);
	HRESULT WINAPI GetBuffer                (UINT Buffer, REFIID riid, void **ppSurface);
	HRESULT WINAPI SetFullscreenState       (BOOL Fullscreen, IDXGIOutput *pTarget);
	HRESULT WINAPI GetFullscreenState       (BOOL *pFullscreen, IDXGIOutput **ppTarget);
	HRESULT WINAPI GetDesc                  (DXGI_SWAP_CHAIN_DESC *pDesc);
	HRESULT WINAPI ResizeBuffers            (UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	HRESULT WINAPI ResizeTarget             (const DXGI_MODE_DESC *pNewTargetParameters);
	HRESULT WINAPI GetContainingOutput      (IDXGIOutput **ppOutput);
	HRESULT WINAPI GetFrameStatistics       (DXGI_FRAME_STATISTICS *pStats);
	HRESULT WINAPI GetLastPresentCount      (UINT *pLastPresentCount);

private:
	/**
	* The actual D3D10 swapchain.
	***/
	IDXGISwapChain* m_pIDXGISwapChain;
};

#endif

