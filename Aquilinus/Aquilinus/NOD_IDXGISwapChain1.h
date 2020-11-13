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
#ifndef NOD_IDXGISWAPCHAIN1
#define NOD_IDXGISWAPCHAIN1

#include"VMT_IDXGISwapChain1.h"

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface           
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_QueryInterface           , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::QueryInterface           ");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObject");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IUNKNOWN::QueryInterface           );
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI AddRef                   
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_AddRef                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::AddRef                   ");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IUNKNOWN::AddRef                   );
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI Release                  
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_Release                  , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::Release                  ");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IUNKNOWN::Release                  );
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGIObject methods
/**
* HRESULT WINAPI SetPrivateData           
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_SetPrivateData           , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::SetPrivateData           ");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGIOBJECT::SetPrivateData           );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetPrivateDataInterface  
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_SetPrivateDataInterface  , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::SetPrivateDataInterface  ");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IUNKNOWN, L"pUnknown");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGIOBJECT::SetPrivateDataInterface  );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetPrivateData           (IDXGISwapChain* pcThis, REFGUID Name,UINT *pDataSize, void *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetPrivateData, 210, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"Name");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGIOBJECT::GetPrivateData);
AQU_D3D_NODE_FOOTER___;
/**

/**
* HRESULT WINAPI GetParent                
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetParent                , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetParent                ");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppParent");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGIOBJECT::GetParent                );
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGIDeviceSubObject methods
/**
* HRESULT WINAPI GetDevice                
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetDevice                , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetDevice                ");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppDevice");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGIDEVICESUBOBJECT::GetDevice                );
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGISwapChain methods
/**
* HRESULT WINAPI Present                  
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_Present                  , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::Present                  ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SyncInterval");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Flags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::Present                  );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetBuffer                
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetBuffer                , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetBuffer                ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Buffer");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppSurface");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetBuffer                );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetFullscreenState       
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_SetFullscreenState       , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::SetFullscreenState       ");
AQU_D3D_NODE_COMMANDER(AQU_BOOL, L"Fullscreen");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IDXGIOUTPUT, L"pTarget");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::SetFullscreenState       );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetFullscreenState       
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetFullscreenState       , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetFullscreenState       ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_BOOL, L"pFullscreen");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDXGIOUTPUT, L"ppTarget");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetFullscreenState       );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetDesc                  
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetDesc                  , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetDesc                  ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_SWAP_CHAIN_DESC, L"pDesc");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetDesc                  );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI ResizeBuffers            
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_ResizeBuffers            , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::ResizeBuffers            ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"BufferCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Width");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Height");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"NewFormat");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SwapChainFlags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::ResizeBuffers            );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI ResizeTarget             
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_ResizeTarget             , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::ResizeTarget             ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_MODE_DESC, L"pNewTargetParameters");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::ResizeTarget             );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetContainingOutput      
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetContainingOutput      , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetContainingOutput      ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDXGIOUTPUT, L"ppOutput");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetContainingOutput      );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetFrameStatistics       
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetFrameStatistics       , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetFrameStatistics       ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_FRAME_STATISTICS, L"pStats");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetFrameStatistics       );
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetLastPresentCount      
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetLastPresentCount      , 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetLastPresentCount      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pLastPresentCount");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN::GetLastPresentCount      );
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGISwapChain1 methods
/**
* HRESULT WINAPI GetDesc1
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetDesc1, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetDesc1");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_SWAP_CHAIN_DESC1, L"pDesc");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetDesc1);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetFullscreenDesc
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetFullscreenDesc, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetFullscreenDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_SWAP_CHAIN_FULLSCREEN_DESC, L"pDesc");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetFullscreenDesc);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetHwnd
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetHwnd, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetHwnd");
AQU_D3D_NODE_COMMANDER(AQU_PNT_HWND, L"pHwnd");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetHwnd);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetCoreWindow
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetCoreWindow, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetCoreWindow");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"refiid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppUnk");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetCoreWindow);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI Present1
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_Present1, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::Present1");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SyncInterval");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"PresentFlags");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_PRESENT_PARAMETERS, L"pPresentParameters");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::Present1);
AQU_D3D_NODE_FOOTER___;

/**
* BOOL WINAPI IsTemporaryMonoSupported
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_IsTemporaryMonoSupported, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::IsTemporaryMonoSupported");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::IsTemporaryMonoSupported);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetRestrictToOutput
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetRestrictToOutput, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetRestrictToOutput");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_IDXGIOUTPUT, L"ppRestrictToOutput");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetRestrictToOutput);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetBackgroundColor
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_SetBackgroundColor, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::SetBackgroundColor");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_RGBA, L"pColor");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::SetBackgroundColor);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetBackgroundColor
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetBackgroundColor, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetBackgroundColor");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_RGBA, L"pColor");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetBackgroundColor);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetRotation
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_SetRotation, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::SetRotation");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_MODE_ROTATION, L"Rotation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::SetRotation);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetRotation
***/
AQU_D3D_NODE_HEADER___(NOD_IDXGISwapChain1_GetRotation, 320, 100);
AQU_D3D_NODE_TITLE____(L"IDXGISwapChain1::GetRotation");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_MODE_ROTATION, L"pRotation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1, VMT_IDXGISWAPCHAIN1::GetRotation);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region IDXGISwapChain1 node provider method

/**
* Node provider class.
***/
class NOD_IDXGISwapChain1
{
public:
	/**
	* Provides IDXGISwapChain1 node by index.
	***/
	NOD_Basic* Get_IDXGISwapChain1Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetRotation) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_IDXGISwapChain1_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_IDXGISwapChain1_Node((VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain1_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::VMT_IUnknown::QueryInterface           :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_QueryInterface           (nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::AddRef                   :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_AddRef                   (nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::Release                  :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_Release                  (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGIObject node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain1_Node(VMT_IDXGIOBJECT::VMT_IDXGIObject eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGIOBJECT::VMT_IDXGIObject::SetPrivateData           :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_SetPrivateData           (nX, nY);
			break;
		case VMT_IDXGIOBJECT::VMT_IDXGIObject::SetPrivateDataInterface  :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_SetPrivateDataInterface  (nX, nY);
			break;
		case VMT_IDXGIOBJECT::VMT_IDXGIObject::GetPrivateData           :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetPrivateData           (nX, nY);
			break;
		case VMT_IDXGIOBJECT::VMT_IDXGIObject::GetParent                :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetParent                (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGIDeviceSubObject node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain1_Node(VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGIDEVICESUBOBJECT::VMT_IDXGIDeviceSubObject::GetDevice                :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetDevice                (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGISwapChain node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain1_Node(VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::Present                  :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_Present                  (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetBuffer                :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetBuffer                (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::SetFullscreenState       :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_SetFullscreenState       (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetFullscreenState       :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetFullscreenState       (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetDesc                  :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetDesc                  (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::ResizeBuffers            :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_ResizeBuffers            (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::ResizeTarget             :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_ResizeTarget             (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetContainingOutput      :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetContainingOutput      (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetFrameStatistics       :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetFrameStatistics       (nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN::VMT_IDXGISwapChain::GetLastPresentCount      :
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetLastPresentCount      (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides IDXGISwapChain1 node by enumeration.
	***/
	NOD_Basic* Get_IDXGISwapChain1_Node(VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetDesc1:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetDesc1(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetFullscreenDesc:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetFullscreenDesc(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetHwnd:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetHwnd(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetCoreWindow:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetCoreWindow(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::Present1:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_Present1(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::IsTemporaryMonoSupported:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_IsTemporaryMonoSupported(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetRestrictToOutput:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetRestrictToOutput(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::SetBackgroundColor:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_SetBackgroundColor(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetBackgroundColor:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetBackgroundColor(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::SetRotation:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_SetRotation(nX, nY);
			break;
		case VMT_IDXGISWAPCHAIN1::VMT_IDXGISwapChain1::GetRotation:
			return (NOD_Basic*)new NOD_IDXGISwapChain1_GetRotation(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

};
#pragma endregion

#endif
