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
#ifndef NOD_IDXGISWAPCHAIN
#define NOD_IDXGISWAPCHAIN

#include "VMT_IDXGISwapChain.h"

#pragma region IUnknown nodes
/**
* HRESULT WINAPI QueryInterface (REFIID riid, void** ppvObj);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_QueryInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI AddRef(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_AddRef, 200, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI Release(void);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_Release, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::Release");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;
#pragma endregion

#pragma region IDXGIObject nodes
/**
* HRESULT WINAPI SetPrivateData           (IDXGISwapChain* pcThis, REFGUID Name, UINT DataSize, const void *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_SetPrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::SetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGIOBJECT::SetPrivateData);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI SetPrivateDataInterface  (IDXGISwapChain* pcThis, REFGUID Name, const IUnknown *pUnknown);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_SetPrivateDataInterface, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::SetPrivateDataInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IUNKNOWN, L"pUnknown");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGIOBJECT::SetPrivateDataInterface);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetPrivateData           (IDXGISwapChain* pcThis, REFGUID Name,UINT *pDataSize, void *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetPrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGIOBJECT::GetPrivateData);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetParent                (IDXGISwapChain* pcThis, REFIID riid, void **ppParent);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetParent, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetParent");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppParent");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGIOBJECT::GetParent);
AQU_D3D_NODE_FOOTER___;
#pragma endregion

#pragma region IDXGIDeviceSubObject nodes
/**
* HRESULT WINAPI GetDevice                (IDXGISwapChain* pcThis, REFIID riid, void **ppDevice);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetDevice, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetDevice");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppDevice");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGIDEVICESUBOBJECT::GetDevice);
AQU_D3D_NODE_FOOTER___;
#pragma endregion

#pragma region  IDXGISwapChain nodes
/**
* HRESULT WINAPI Present                  (IDXGISwapChain* pcThis, UINT SyncInterval, UINT Flags);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_Present, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::Present");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SyncInterval");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Flags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::Present);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetBuffer                (IDXGISwapChain* pcThis, UINT Buffer, REFIID riid, void **ppSurface);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetBuffer, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetBuffer");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Buffer");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppSurface");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetBuffer);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI SetFullscreenState       (IDXGISwapChain* pcThis, BOOL Fullscreen, IDXGIOutput *pTarget);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_SetFullscreenState, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::SetFullscreenState");
AQU_D3D_NODE_COMMANDER(AQU_BOOL, L"Fullscreen");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IDXGIOUTPUT, L"pTarget");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::SetFullscreenState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetFullscreenState       (IDXGISwapChain* pcThis, BOOL *pFullscreen, IDXGIOutput **ppTarget);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetFullscreenState, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetFullscreenState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_BOOL, L"pFullscreen");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDXGIOUTPUT, L"ppTarget");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetFullscreenState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetDesc                  (IDXGISwapChain* pcThis, DXGI_SWAP_CHAIN_DESC *pDesc);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetDesc, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_SWAP_CHAIN_DESC, L"pDesc");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetDesc);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI ResizeBuffers            (IDXGISwapChain* pcThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_ResizeBuffers, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::ResizeBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"BufferCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Width");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Height");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"NewFormat");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SwapChainFlags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::ResizeBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI ResizeTarget             (IDXGISwapChain* pcThis, const DXGI_MODE_DESC *pNewTargetParameters);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_ResizeTarget, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::ResizeTarget");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_MODE_DESC, L"pNewTargetParameters");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::ResizeTarget);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetContainingOutput      (IDXGISwapChain* pcThis, IDXGIOutput **ppOutput);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetContainingOutput, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetContainingOutput");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDXGIOUTPUT, L"ppOutput");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetContainingOutput);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetFrameStatistics       (IDXGISwapChain* pcThis, DXGI_FRAME_STATISTICS *pStats);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetFrameStatistics, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetFrameStatistics");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_FRAME_STATISTICS, L"pStats");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetFrameStatistics);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetLastPresentCount      (IDXGISwapChain* pcThis, UINT *pLastPresentCount);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain_GetLastPresentCount, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain::GetLastPresentCount");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pLastPresentCount");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain, VMT_IDXGISWAPCHAIN::GetLastPresentCount);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGISwapChain node provider method

/**
* Node provider class.
***/
class NOD_IDXGISwapChain
{
public:
	/**
	* Provides IDXGISwapChain node by index.
	***/
	NOD_Basic* Get_IDXGISwapChain_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetLastPresentCount) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_IDXGISwapChain_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		if (dwIndex <= (UINT)VMT_IDXGIOBJECT::GetParent) return Get_IDXGIObject_Node((VMT_IDXGIOBJECT::VMT_IDXGIObject)dwIndex, nX, nY);
		if (dwIndex == (UINT)VMT_IDXGIDEVICESUBOBJECT::GetDevice) return Get_IDXGIDeviceSubObject_Node((VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject)dwIndex, nX, nY);
		return Get_IDXGISwapChain_Node((VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::QueryInterface:
			return (NOD_Basic*)new NOD_IDXGISwapChain_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::AddRef:
			return (NOD_Basic*)new NOD_IDXGISwapChain_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::Release:
			return (NOD_Basic*)new NOD_IDXGISwapChain_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGIObject node by enumeration.
	***/
	NOD_Basic* Get_IDXGIObject_Node(VMT_IDXGIOBJECT::VMT_IDXGIObject eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGIOBJECT::SetPrivateData:
			return (NOD_Basic*)new NOD_IDXGISwapChain_SetPrivateData(nX, nY);
			break;
		case VMT_IDXGIOBJECT::SetPrivateDataInterface:
			return (NOD_Basic*)new NOD_IDXGISwapChain_SetPrivateDataInterface(nX, nY);
			break;
		case VMT_IDXGIOBJECT::GetPrivateData:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetPrivateData(nX, nY);
			break;
		case VMT_IDXGIOBJECT::GetParent:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetParent(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGIDeviceSubObject node by enumeration.
	***/
	NOD_Basic* Get_IDXGIDeviceSubObject_Node(VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGIDEVICESUBOBJECT::GetDevice:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetDevice(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGISwapChain node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain_Node(VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGISWAPCHAIN::Present:
			return (NOD_Basic*)new NOD_IDXGISwapChain_Present(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetBuffer:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetBuffer(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::SetFullscreenState:
			return (NOD_Basic*)new NOD_IDXGISwapChain_SetFullscreenState(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetFullscreenState:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetFullscreenState(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetDesc:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetDesc(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::ResizeBuffers:
			return (NOD_Basic*)new NOD_IDXGISwapChain_ResizeBuffers(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::ResizeTarget:
			return (NOD_Basic*)new NOD_IDXGISwapChain_ResizeTarget(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetContainingOutput:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetContainingOutput(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetFrameStatistics:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetFrameStatistics(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::GetLastPresentCount:
			return (NOD_Basic*)new NOD_IDXGISwapChain_GetLastPresentCount(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	};
};

#pragma endregion

#endif