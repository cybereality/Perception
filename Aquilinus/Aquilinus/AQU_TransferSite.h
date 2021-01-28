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

#pragma warning( disable : 26812 )
#pragma warning( disable : 26495 )

#include "AQU_GlobalTypes.h"
#include "AQU_SupportedInterfaces.h"
#include "AQU_Nodes.h"
#include "AQU_FileManager.h"

#ifndef AQUILINUS_TRANSFERSITE
#define AQUILINUS_TRANSFERSITE

/**
* Working area data sheet entry structure.
***/
struct AQU_DataSheetEntry
{
	std::wstring m_szTitle;                      /**< The title of the data shed entry. ***/
	bool m_bIsOpen = false;                      /**< True if this entry is open, data shed will output the sub entries. ***/
	std::vector<std::wstring> m_aacSubEntries;   /**< The sub-entries array. ***/
	LONG m_lYHeight = 0;                         /**< The current on-screen height of the entry. In window-pixel space. ***/
};

/**
* Working area data sheet category.
***/
struct AQU_DataSheetCategory
{
	std::wstring m_szTitle;                       /**< The title of the data shed entry. ***/
	bool m_bIsOpen = false;                       /**< True if this entry is open, data shed will output the sub entries. ***/
	LONG m_lYHeight = 0;                          /**< The current on-screen height of the entry. In window-pixel space. ***/
	std::vector<AQU_DataSheetEntry*> m_paEntries; /**< The entries for that category. ***/
};

/**
* This class inherits all possible data transfer for Aquilinus.
* (node data, configuration data) All data is public for other 
* classes.
***/
class AQU_TransferSite
{
public:
	AQU_TransferSite(AquilinusCfg* pConfig);
	~AQU_TransferSite();

	/*** AQU_TransferSite public methods ***/
	void InitD3DNodes();
	bool IsD3DNodePresent(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces eInterfaceIndex, int nMethodIndex);
	void RegisterD3DNode(NOD_Basic* pNode, AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces eInterfaceIndex, int nMethodIndex);
	void RegisterD3DNode(NOD_Basic* pNode, UINT dwID);
	void UnregisterAllNodes();
	void RegisterDataSheetVertexShader(LPCWSTR szName, std::vector<std::wstring> pszEntries, UINT dwHash);
	void RegisterDataSheetPixelShader(LPCWSTR szName, std::vector<std::wstring> pszEntries, UINT dwHash);
	bool VertexShaderPresent(UINT dwHash);
	bool PixelShaderPresent(UINT dwHash);

	/**
	* Force D3D bool.
	* This bool ensures that the detour classes call (super) D3D methods.
	***/
	bool m_bForceD3D;
	/**
	* Current active thread vector.
	* This vector ensures that the detour classes call (super) D3D methods for multithreading.
	***/
	std::vector<DWORD> m_adwCurrentThreadIds;
	/**
	* The number of cores present on this system.
	***/
	DWORD m_dwNumberOfProcessors;
	/**
	* The main thread id, set by SwapChain and DeviceContext
	***/
	DWORD m_dwMainThreadId;
	/**
	* The number of active threads.
	* If this number is zero, clear the current thread id vector
	**/
	DWORD m_dwThreadsActive;
	/**
	* True if the d3d nodes register is initialized.
	***/
	bool m_bNodesInitialized;
	/**
	* All nodes vector address.
	***/
	std::vector<NOD_Basic*>* m_ppaNodes;
	/**
	* The Aquilinus file manager class, as used inside the Aquilinus dll.
	***/
	AQU_FileManager* m_pFileManager;
	/**
	* The public configuration pointer.
	***/
	AquilinusCfg* m_pConfig;
	/**
	* The current reference counter of all supported interfaces.
	* This number can flicker for interfaces with more than one instance.
	***/
	UINT m_anInterfaceRefCount[SUPPORTED_INTERFACES_NUMBER];
	/**
	* The (data sheet) indices of the injected interfaces.
	* Index refers to AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces.
	***/
	std::vector<UINT> m_aInterfaceIndices;
	/**
	* The (data sheet) category indices of the plugins.
	***/
	std::vector<UINT> m_aPluginCategoryIndices;
	/**
	* The (data sheet) indices of the plugins.
	* Index refers to the entry in the category specified by m_aPluginCategoryIndices.
	***/
	std::vector<UINT> m_aPluginEntryIndices;
	/**
	* The (data sheet) index of the vertex shader tab.
	***/
	UINT m_nVertexShaderTabIndex;
	/**
	* The (data sheet) index of the vertex shader tab.
	***/
	UINT m_nPixelShaderTabIndex;
	/**
	* The (data sheet) index of the elementary nodes tab.
	***/
	UINT m_nElementaryNodesTabIndex;
	/**
	* True if the current project stage is a working area, false if game profile compiled.
	***/
	bool m_bIsWorkingArea;
	/**
	* True if shader hash codes are to be created. Warning ! Can cause games to crash.
	***/
	bool m_bCreateShaderHash;
	/**
	* All data shed categories vector.
	***/
	std::vector<AQU_DataSheetCategory*> m_paDataSheetCategories;
	/**
	* All vertex shader hash codes vector.
	* Succession equals the order in the data sheet vertex shader category.
	***/
	std::vector<UINT> m_aVertexShaderHashcodes;
	/**
	* All pixel shader hash codes vector.
	* Succession equals the order in the data sheet pixel shader category.
	***/
	std::vector<UINT> m_aPixelShaderHashcodes;
	/**
	* D3D9 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D9ReinstateInterfaces;
	/**
	* D3D929 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D929ReinstateInterfaces;
	/**
	* D3D10 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D10ReinstateInterfaces;
	/**
	* D3D10.1 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D10_1ReinstateInterfaces;
	/**
	* D3D11 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D11ReinstateInterfaces;
	/**
	* D3D11.1 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D11_1ReinstateInterfaces;
	/**
	* D3D11.2 Thread to reinstate VMTable or detour methods.
	***/
	LPTHREAD_START_ROUTINE m_pD3D11_2ReinstateInterfaces;
	/**
	* D3D9 latest cube texture address.
	***/
	void* m_pIDirect3DCubeTexture9;
	/**
	* D3D9 latest index buffer address.
	***/
	void* m_pIDirect3DIndexBuffer9;
	/**
	* D3D9 latest pixel shader address.
	***/
	void* m_pIDirect3DPixelShader9;
	/**
	* D3D9 latest query address.
	***/
	void* m_pIDirect3DQuery9;
	/**
	* D3D9 latest state block address.
	***/
	void* m_pIDirect3DStateBlock9;
	/**
	* D3D9 latest surface address.
	***/
	void* m_pIDirect3DSurface9;
	/**
	* D3D9 latest Swapchain address.
	***/
	void* m_pIDirect3DSwapChain9;
	/**
	* D3D9 latest texture address.
	***/
	void* m_pIDirect3DTexture9;
	/**
	* D3D9 latest vertex buffer address.
	***/
	void* m_pIDirect3DVertexBuffer9;
	/**
	* D3D9 latest vertex shader address.
	***/
	void* m_pIDirect3DVertexShader9;
	/**
	* D3D9 latest volume texture address.
	***/
	void* m_pIDirect3DVolumeTexture9;

#pragma region AQU_TransferSite Direct3D9 interface nodes

	/** 
	* IDirect3DDevice9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DDevice9;
	/** 
	* IDirect3DTexture9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DTexture9;
	/** 
	* IDirect3DBaseTexture9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DBaseTexture9;
	/** 
	* IDirect3DResource9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DResource9;
	/** 
	* IDirect3DCubeTexture9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DCubeTexture9;
	/** 
	* IDirect3DVolumeTexture9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DVolumeTexture9;
	/** 
	* IDirect3DVolume9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DVolume9;
	/** 
	* IDirect3DSurface9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DSurface9;
	/** 
	* IDirect3DSwapChain9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DSwapChain9;
	/** 
	* IDirect3DIndexBuffer9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DIndexBuffer9;
	/** 
	* IDirect3DVertexBuffer9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DVertexBuffer9;
	/** 
	* IDirect3DPixelShader9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DPixelShader9;
	/** 
	* IDirect3DVertexShader9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DVertexShader9;
	/** 
	* IDirect3DQuery nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DQuery9;
	/** 
	* IDirect3DStateBlock9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DStateBlock9;
	/** 
	* IDirect3DVertexDeclaration9 nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DVertexDeclaration9;
	/** 
	* IDirect3DDevice9Ex nodes 
	***/
	NOD_Basic** m_ppNOD_IDirect3DDevice9Ex;
	/**
	* D3DX9 node
	***/
	NOD_Basic* m_pNOD_D3DX9;

#pragma endregion

#pragma region AQU_TransferSite Direct3D10 interface nodes

	/** 
	* ID3D10Device nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D10Device;
	/** 
	* ID3D10Device1 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D10Device1;
	/** 
	* IDXGISwapChain nodes 
	***/
	NOD_Basic** m_ppNOD_IDXGISwapChain;
	/** 
	* IDXGISwapChain1 nodes 
	***/
	NOD_Basic** m_ppNOD_IDXGISwapChain1;
	/** 
	* IDXGISwapChain2 nodes 
	***/
	NOD_Basic** m_ppNOD_IDXGISwapChain2;
	/** 
	* IDXGISwapChain3 nodes 
	***/
	NOD_Basic** m_ppNOD_IDXGISwapChain3;

#pragma endregion

#pragma region AQU_TransferSite Direct3D11 interface nodes

	/** 
	* ID3D11Device nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11Device;
	/** 
	* ID3D11Device1 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11Device1;
	/** 
	* ID3D11Device2 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11Device2;
	/** 
	* ID3D11Device3 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11Device3;
	/** 
	* ID3D11DeviceContext nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11DeviceContext;
	/** 
	* ID3D11DeviceContext1 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11DeviceContext1;
	/** 
	* ID3D11DeviceContext2 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11DeviceContext2;
	/** 
	* ID3D11DeviceContext3 nodes 
	***/
	NOD_Basic** m_ppNOD_ID3D11DeviceContext3;

#pragma endregion

};

#endif

