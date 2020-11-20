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
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }
#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))

#include"Aquilinus.h"
#include"DMT_IDirect3DBaseTexture9.h"
#include"DMT_IDirect3DCubeTexture9.h"
#include"DMT_IDirect3DDevice9.h"
#include"DMT_IDirect3DIndexBuffer9.h"
#include"DMT_IDirect3DPixelShader9.h"
#include"DMT_IDirect3DQuery9.h"
#include"DMT_IDirect3DResource9.h"
#include"DMT_IDirect3DStateBlock9.h"
#include"DMT_IDirect3DSurface9.h"
#include"DMT_IDirect3DSwapChain9.h"
#include"DMT_IDirect3DTexture9.h"
#include"DMT_IDirect3DVertexBuffer9.h"
#include"DMT_IDirect3DVertexDeclaration9.h"
#include"DMT_IDirect3DVertexShader9.h"
#include"DMT_IDirect3DVolume9.h"
#include"DMT_IDirect3DVolumeTexture9.h"
#include"DMT_IDirect3DDevice9Ex.h"
#include"DMT_ID3D10Device.h"
#include"DMT_ID3D10Device1.h"
#include"DMT_ID3D11Device.h"
#include"DMT_ID3D11Device1.h"
#include"DMT_ID3D11Device2.h"//#include"DMT_ID3D11Device3.h"
#include"DMT_IDXGISwapChain.h"
#include"DMT_IDXGISwapChain1.h"
#include"DMT_IDXGISwapChain2.h"//#include"DMT_IDXGISwapChain3.h"
#include"DMT_ID3D11DeviceContext.h"
#include"DMT_ID3D11DeviceContext1.h"
#include"DMT_ID3D11DeviceContext2.h"//#include"DMT_ID3D11DeviceContext3.h"

#pragma region Aquilinus global helpers
/**
* Debug console helper function.
***/
void OutputDebug(char* lpszFormat, ...)
{
	char szBuffer[512];
	int nBuf;
	va_list args;
	va_start(args, lpszFormat);
	nBuf = _vsnprintf_s(szBuffer, 512, lpszFormat, args);
	if (nBuf > -1)
		OutputDebugStringA(szBuffer);
	else
		OutputDebugString(L"MYTRACE buffer overflow\n");
	va_end(args);
}

/**
* Compare memory mask helper.
***/
bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if ((*szMask == 'x') && (*pData != *bMask))
			return false;

	// Windows 10 64bit fix : bool always in (..)
	return ((*szMask) == NULL);
}

/**
* Find memory pattern helper.
***/
UINT_PTR FindPattern(UINT_PTR dValor, UINT_PTR dLer, BYTE *bMaskara, char * szMaskara)
{
	for (UINT_PTR i = 0; i < dLer; i++)
		if (bCompare((PBYTE)(dValor + i), bMaskara, szMaskara))
		{
			OutputDebugString(L"TRUE");
			DEBUG_HEX(dValor + i);
			return (UINT_PTR)(dValor + i);
		}
	return false;
}
/**
* Find memory twin dword helper.
***/
UINT_PTR FindTwinDword(UINT_PTR dValor, DWORD dLer, DWORD dValue, DWORD dSpace)
{
	for (UINT_PTR i = 0; i < dLer; i++)
		if (((*(UINT_PTR*)(dValor + i)) == dValue) &&
			((*(UINT_PTR*)(dValor + i)) == (*(UINT_PTR*)(dValor + i + dSpace))))
			return (UINT_PTR)(dValor + i);
	return false;
}
#pragma endregion

#pragma region Aquilinus dll methods

#pragma region DllMain and Init

/**
* Main dll entry point.
***/
BOOL WINAPI DllMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// constructor, first init Aquilinus
		if (AquilinusInit() == S_OK)
		{
			// init the chosen project
			AquilinusInitProject(hinstModule);

			return TRUE;
		}
		else
			MessageBox(nullptr, L"Failed to init Aquilinus", L"Error", 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// set project to >closed<
		g_pAQU_TransferSite->m_pConfig->eProjectStage = AQU_ProjectStage::Closed;

		// try to release all you can
		g_pAQU_TransferSite->m_bForceD3D = true;
		if (g_paNodes.size())
		{
			for (std::vector<NOD_Basic*>::size_type i = 0; i != g_paNodes.size(); i++)
			{
				delete g_paNodes[i];
			}
			g_paNodes.clear();
		}
		if (g_pAQU_WorkingArea) delete g_pAQU_WorkingArea;
		UnmapViewOfFile((LPCVOID)g_pAquilinusConfig);
		CloseHandle(g_hConfigMapFile);
	}
	return FALSE;
}

/**
* Init Aquilinus fields.
***/
HRESULT WINAPI AquilinusInit(VOID)
{
	// get the config map handle
	g_hConfigMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		g_szMemoryPageName);   // name of the Aquilinus config

	// return if failed
	if (g_hConfigMapFile == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
		return E_FAIL;
	}

	// create map view
	g_pAquilinusConfig = (AquilinusCfg*)
		MapViewOfFile(g_hConfigMapFile,   // handle to map object
			FILE_MAP_ALL_ACCESS,              // read/write permission
			0,
			0,
			sizeof(AquilinusCfg));

	// return if failed
	if (g_pAquilinusConfig == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not map view of file.\n"));
		CloseHandle(g_hConfigMapFile);
		return E_FAIL;
	}

	// create the global transfer site class
	g_pAQU_TransferSite = new AQU_TransferSite(g_pAquilinusConfig);
	g_pAQU_TransferSite->InitD3DNodes();

	// set vmtable threads
	g_pAQU_TransferSite->m_pD3D9ReinstateInterfaces = D3D9_VMT_Repatch;
	//g_pAQU_TransferSite->m_pD3D9ReinstateInterfaces = D3D929_VMT_Repatch;
	g_pAQU_TransferSite->m_pD3D10ReinstateInterfaces = D3D10_VMT_Repatch;
	//g_pAQU_TransferSite->m_pD3D10_1ReinstateInterfaces = D3D10_1_VMT_Repatch;
	g_pAQU_TransferSite->m_pD3D11ReinstateInterfaces = D3D11_VMT_Repatch;
	//g_pAQU_TransferSite->m_pD3D11_1ReinstateInterfaces = D3D11_1_VMT_Repatch;
	//g_pAQU_TransferSite->m_pD3D11_2ReinstateInterfaces = D3D11_2_VMT_Repatch;

	// is there a chosen DX9 interface ?
	bool bDX9 = false;
	for (int i = DX9_FIRST_INTERFACE; i < DX9_LAST_INTERFACE; i++)
		bDX9 |= (g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::VMTable) ||
		(g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::Detour);

	// root thread handle
	HANDLE pRootThread = nullptr;

	// start DX9 thread
	if (bDX9)
	{
		enum D3D9_Hook_Type
		{
			D3D9_Hook_Object,
			D3D9_Hook_Device_Mask,
			D3D9_Hook_Device_Create
		} eD3D9_Hook_Type = D3D9_Hook_Device_Create; // D3D9_Hook_Object;

		if (eD3D9_Hook_Type == D3D9_Hook_Object)
		{
			// get device by object detour
			if ((pRootThread = CreateThread(NULL, 0, D3D9_Detour, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
		else if (eD3D9_Hook_Type == D3D9_Hook_Device_Mask)
		{
			// get device by memory pattern
			if ((pRootThread = CreateThread(NULL, 0, D3D9_VMT_Mask, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
		else if (eD3D9_Hook_Type == D3D9_Hook_Device_Create)
		{
			// get device by ->CreateDevice() detour ?
			/*pRootThread = CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
			if (pRootThread == NULL)
			return E_FAIL;*/

			// NO thread here !! we call the method directly to save time..
			if (FAILED(D3D9_VMT_Create(nullptr))) return E_FAIL;
		}
	}

	// D3D9.29 ??

	// is there a chosen DX10 interface ?
	bool bDX10 = false;
	for (int i = DX10_FIRST_INTERFACE; i < DX10_LAST_INTERFACE; i++)
		bDX10 |= (g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::VMTable) ||
		(g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::Detour);

	// start DX9 thread
	if (bDX10)
	{
		// get device by memory pattern or by ->CreateDevice() detour ?
		if (true)
		{
			if ((pRootThread = CreateThread(NULL, 0, D3D10_VMT_Mask, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
		else
		{
			if ((pRootThread = CreateThread(NULL, 0, D3D10_VMT_Create, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
	}

	// is there a chosen DX11 interface ?
	bool bDX11 = false;
	for (int i = DX11_FIRST_INTERFACE; i < DX11_LAST_INTERFACE; i++)
		bDX11 |= (g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::VMTable) ||
		(g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::Detour);

	// start DX11 thread
	if (bDX11)
	{
		// get device by memory pattern or by ->CreateDevice() detour ?
		if (true)
		{
			if ((pRootThread = CreateThread(NULL, 0, D3D11_VMT_Mask, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
		else
		{
			// METHOD OBSOLETE !
			if ((pRootThread = CreateThread(NULL, 0, D3D11_VMT_Create, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
	}

	// TODO !! d3d 10.1 / 11.1 / 11.2

	if (false)
	{
		// resume the suspended process
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// call external suspend tool
		std::wstringstream szAppName = std::wstringstream();
		std::wstringstream szCmd = std::wstringstream();
#ifdef _WIN64
		szAppName << g_pAquilinusConfig->szAquilinusPath << L"pssuspend64.exe";
		szCmd << "pssuspend64 -r " << g_pAquilinusConfig->dwID;
#else
		szAppName << g_pAquilinusConfig->szAquilinusPath << L"pssuspend.exe";
		szCmd << "pssuspend -r " << g_pAquilinusConfig->dwID;
#endif
		CreateProcess(szAppName.str().c_str(), (LPWSTR)&szCmd.str().c_str()[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	OutputDebugString(L"Aquilinus : initialized.... :");

	return S_OK;
}

/**
* Creates project window(s).
***/
HRESULT WINAPI AquilinusInitProject(HINSTANCE hInstance)
{
	OutputDebugString(L"Aquilinus : Init Project ");

	switch (g_pAquilinusConfig->eProjectStage)
	{
	case AQU_ProjectStage::WorkingAreaNew:
		// create the working area class
		g_pAQU_WorkingArea = new AQU_WorkingArea(hInstance, g_pAQU_TransferSite);

		// set to working area stage
		g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingArea;
		break;
	case AQU_ProjectStage::WorkingAreaLoad:
		// create the working area class
		g_pAQU_WorkingArea = new AQU_WorkingArea(hInstance, g_pAQU_TransferSite);

		// output debug information
		OutputDebugString(g_pAQU_TransferSite->m_pFileManager->GetName(g_pAQU_TransferSite->m_pConfig->dwProcessIndex));

		// load the workspace
		g_pAQU_WorkingArea->s_LoadWorkSpace();

		// set bForceD3D to "true" if "Editor Mode" is set
		if (!g_pAquilinusConfig->bAlwaysForceD3D)
			g_pAQU_TransferSite->m_bForceD3D = true;

		// set to working area stage
		g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::WorkingArea;
		break;
	case AQU_ProjectStage::Complemented:
	{
		// set d3d override to true
		g_pAQU_TransferSite->m_bForceD3D = true;

		// no working area here
		g_pAQU_TransferSite->m_bIsWorkingArea = false;

		// set transfer site node vector address
		g_pAQU_TransferSite->m_ppaNodes = &g_paNodes;

		// clear data sheet vector and supported interface indices vector
		g_pAQU_TransferSite->m_paDataSheetCategories.clear();
		g_pAQU_TransferSite->m_aInterfaceIndices.clear();

		// get a data stream
		std::stringstream sstrDataStream;

		// load the stream
		HRESULT hr = g_pAQU_TransferSite->m_pFileManager->LoadProfile(g_pAQU_TransferSite->m_pConfig->szProfileFilePath, sstrDataStream);

		// and decode the stream
		if (SUCCEEDED(hr))
		{
			// first, ignore the game, process and window name
			sstrDataStream.ignore(ENTRY_SIZE * 3);

			// ignore the additional option block length (zero by now)
			sstrDataStream.ignore(sizeof(DWORD));

			// ignore picture boolean and the path if true
			BOOL bPicture;
			sstrDataStream.read((char*)&bPicture, sizeof(BOOL));
			if (bPicture)
			{
				LONG nImageSize;
				sstrDataStream.read((char*)&nImageSize, sizeof(LONG));
				sstrDataStream.ignore(nImageSize);
			}

			// ignore the detour time delay
			sstrDataStream.ignore(sizeof(DWORD));

			// now, ignore the injection techniques
			DWORD dwSupportedInterfacesNumber;
			sstrDataStream.read((char*)&dwSupportedInterfacesNumber, sizeof(DWORD));
			sstrDataStream.ignore(dwSupportedInterfacesNumber * sizeof(int));

			// read the number of nodes, resize node vector
			UINT dwNodeNumber;
			sstrDataStream.read((char*)&dwNodeNumber, sizeof(UINT));
			g_paNodes.resize(dwNodeNumber);

			// get a node provider
			AQU_NodeProvider* pProvider = new AQU_NodeProvider();

			// loop through the nodes to add node data
			for (UINT i = 0; i != dwNodeNumber; i++)
			{
				// get the node hash
				UINT id;
				sstrDataStream.read((char*)&id, sizeof(UINT));

				// get the node position
				POINT pos;
				sstrDataStream.read((char*)&pos.x, sizeof(LONG));
				sstrDataStream.read((char*)&pos.y, sizeof(LONG));

				// load plugin info (if plugin node)
				UINT idPlugin = 0;
				wchar_t szFileName[64];
				wchar_t szFilePath[MAX_PATH];
				if (id == ELEMENTARY_NODE_PLUGIN)
				{
					// and write to stream
					sstrDataStream.read((char*)&idPlugin, sizeof(UINT));
					sstrDataStream.read((char*)&szFileName[0], sizeof(wchar_t) * 64);

					// create full path
					wsprintf(szFilePath, L"%s%s", g_pAQU_TransferSite->m_pFileManager->GetPluginPath(), szFileName);

					OutputDebugString(szFilePath);
				}

				// read node data
				UINT dwDataSize = 0;
				sstrDataStream.read((char*)&dwDataSize, sizeof(UINT));

				// read the data
				char* pcData = new char[dwDataSize];
				if (dwDataSize)
					sstrDataStream.read((char*)pcData, dwDataSize);

				// get a node pointer
				NOD_Basic* pNode;
				if (FAILED(pProvider->Get_Node(pNode, id, pos.x, pos.y, idPlugin, szFilePath)))
				{
					// unregister all nodes
					g_pAQU_TransferSite->UnregisterAllNodes();

					// debug output
					wchar_t buf[64];
					wsprintf(buf, L"Aquilinus : Unknown node type %u", id);
					OutputDebugString(buf);

					for (UINT j = 0; j < i; j++)
						delete g_paNodes[j];

					g_paNodes.clear();
					delete pProvider;
					sstrDataStream.clear();
					return E_FAIL;
				}

				// init node data
				pNode->InitNodeData(pcData, dwDataSize);

				// register this node
				g_pAQU_TransferSite->RegisterD3DNode(pNode, id);

				// and add the node
				g_paNodes[i] = pNode;
				delete pcData;
			}

			delete pProvider;

			// loop through the nodes to add node connections
			for (std::vector<NOD_Basic*>::size_type i = 0; i != g_paNodes.size(); i++)
			{
				// get the number of commanders
				DWORD dwCommandersNumber;
				sstrDataStream.read((char*)&dwCommandersNumber, sizeof(DWORD));

				// loop through commanders, get the number of connections and the connection indices
				for (DWORD j = 0; j < dwCommandersNumber; j++)
				{
					// get the commander connections number
					DWORD dwConnectionsNumber;
					sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(DWORD));

					// loop through decommanders, set indices
					for (DWORD k = 0; k < dwConnectionsNumber; k++)
					{
						// get decommander node index
						LONG lNodeIndex;
						sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));
						// get decommander index
						LONG lDecommanderIndex;
						sstrDataStream.read((char*)&lDecommanderIndex, sizeof(LONG));

						// output debug data
						OutputDebugString(L"Connect Decommander");
						wchar_t buf[64];
						wsprintf(buf, L"nodes : %u %u cix : %u dix : %u", i, lNodeIndex, j, lDecommanderIndex);
						OutputDebugString(buf);

						// and connect
						g_paNodes[i]->ConnectDecommander(g_paNodes[lNodeIndex], lNodeIndex, j, lDecommanderIndex);
					}
				}

				// get the provoker connections number
				DWORD dwConnectionsNumber;
				sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(DWORD));

				// loop through decommanders, return indices
				for (int j = 0; j < (int)dwConnectionsNumber; j++)
				{
					LONG lNodeIndex;
					sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));

					// output debug data
					OutputDebugString(L"Connect Invoker");
					wchar_t buf[64];
					wsprintf(buf, L"nodes : %u %u", i, lNodeIndex);
					OutputDebugString(buf);

					// and connect
					g_paNodes[i]->ConnectInvoker(g_paNodes[lNodeIndex], lNodeIndex);
				}
			}

			// loop through the nodes again to call the connect commander method
			for (std::vector<NOD_Basic*>::size_type i = 0; i != g_paNodes.size(); i++)
				g_paNodes[i]->ConnectCommander(nullptr, 0);
		}

		// set d3d override to false
		g_pAQU_TransferSite->m_bForceD3D = false;

		// set project to "complemented-injected"
		g_pAquilinusConfig->eProjectStage = AQU_ProjectStage::ComplementedInjected;
	}
	break;
	default:
		break;
	}


	return S_OK;
}

#pragma endregion

#pragma endregion

#pragma region VMT Threads

#pragma region DirectX 9.0
/**
* Create D3D9 object detour.
* Creates D3D9Ex device.
***/
IDirect3D9* WINAPI D3D9_Direct3DCreate9_Detour(UINT SDKVersion)
{
	OutputDebugString(L"Aquilinus : D3D9_Direct3DCreate9_Detour");
	if (g_pAquilinusConfig->bCreateD3D9Ex)
	{
		static IDirect3D9Ex* pcD3D = nullptr;
		pcD3D = nullptr;
		Direct3DCreate9Ex(SDKVersion, &pcD3D);
		return pcD3D;
	}
	else
		return D3D9_Direct3DCreate9_Super(SDKVersion);
}

/**
*
***/
HRESULT WINAPI D3D9_QueryInterface_Detour(LPDIRECT3D9 pcD3D, REFIID riid, void** ppvObj)
{
	if (!g_pAQU_TransferSite->m_bForceD3D)
	{
		// DEFINE_GUID(IID_IDirect3D9Ex, 0x02177241, 0x69FC, 0x400C, 0x8F, 0xF1, 0x93, 0xA4, 0x4D, 0xF6, 0x86, 0x1D);
		IF_GUID(riid, 0x02177241, 0x69FC, 0x400C, 0x8F, 0xF1, 0x93, 0xA4, 0x4D, 0xF6, 0x86, 0x1D)
		{
			OutputDebugString(L"D3D9_QueryInterface - IID_IDirect3D9Ex");

			// no IID_IDirect3D9Ex available
			if (ppvObj == NULL)
				return E_POINTER;

			pcD3D->AddRef(); // NEED THIS ?
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}
	}

	return D3D9_QueryInterface_Super(pcD3D, riid, ppvObj);
}

/**
*
***/
HRESULT WINAPI D3D9_CheckDeviceType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	static HRESULT nHr = S_OK;
	nHr = D3D9_CheckDeviceType_Super(pcD3D, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);

	return nHr;
}

/**
* Detour for the device format validation method.
***/
HRESULT WINAPI D3D9_CheckDeviceFormat_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	static HRESULT nHr = S_OK;
	nHr = D3D9_CheckDeviceFormat_Super(pcD3D, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	return nHr;
}

/**
*
***/
HRESULT WINAPI D3D9_CheckDeviceMultiSampleType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
	return D3D9_CheckDeviceMultiSampleType_Super(pcD3D, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

/**
*
***/
HRESULT WINAPI D3D9_CheckDepthStencilMatch_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return D3D9_CheckDepthStencilMatch_Super(pcD3D, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

/**
* Detour for the device format conversion method.
***/
HRESULT WINAPI D3D9_CheckDeviceFormatConversion_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return D3D9_CheckDeviceFormatConversion_Super(pcD3D, Adapter, DeviceType, SourceFormat, TargetFormat);
}

/**
* Create device detour needed for DirectX 9.0.
***/
HRESULT WINAPI D3D9_CreateDevice_Detour(
	LPDIRECT3D9 pcD3D,
	UINT Adapter,
	D3DDEVTYPE DeviceType,
	HWND hFocusWindow,
	DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters,
	IDirect3DDevice9 **ppReturnedDeviceInterface
)
{
	static bool s_bIsOwnCall = false;

	if (s_bIsOwnCall)
	{
		// call super method... normal device created
		return D3D9_CreateDevice_Super(pcD3D, Adapter, DeviceType, hFocusWindow, BehaviorFlags,
			pPresentationParameters, ppReturnedDeviceInterface);
	}

	// force D3D to ensure query call works
	g_pAQU_TransferSite->m_bForceD3D = true;

	//const IID IID_IDirect3D9Ex = { 0x02177241, 0x69FC, 0x400C, { 0x8F, 0xF1, 0x93, 0xA4, 0x4D, 0xF6, 0x86, 0x1D } };
	HRESULT hr = S_OK;
	IDirect3D9Ex *pDirect3D9Ex = NULL;
	if ((pcD3D) && (SUCCEEDED(pcD3D->QueryInterface(IID_IDirect3D9Ex, (void**)(&pDirect3D9Ex)))))
	{
		OutputDebugString(L"[Aquilinus] Create D3D9Ex device !");

		// Force no VSYNC in DX9Ex mode
		D3DPRESENT_PARAMETERS presentationParameters = *pPresentationParameters;
		presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		s_bIsOwnCall = true;
		hr = pDirect3D9Ex->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &presentationParameters, ppReturnedDeviceInterface);
		s_bIsOwnCall = false;

		if (*ppReturnedDeviceInterface)
		{
			IDirect3DDevice9Ex *pDirect3DDevice9Ex = NULL;
			if (SUCCEEDED((*ppReturnedDeviceInterface)->QueryInterface(IID_IDirect3DDevice9Ex, reinterpret_cast<void**>(&pDirect3DDevice9Ex))))
			{
				if (SUCCEEDED(hr))
					OutputDebugString(L"[Aquilinus] IDirect3DDevice9Ex created.");
				pDirect3DDevice9Ex->Release();
			}
			else
			{
				OutputDebugString(L"[Aquilinus] D3D9 device created.");
			}
		}
	}
	else
	{
		OutputDebugString(L"[Aquilinus] Create D3D9 device !");

		// call super method... normal device created
		hr = D3D9_CreateDevice_Super(pcD3D, Adapter, DeviceType, hFocusWindow, BehaviorFlags,
			pPresentationParameters, ppReturnedDeviceInterface);
	}

	// set back bForceD3D... (only if "Editor Mode" is not set)
	if (!g_pAquilinusConfig->bAlwaysForceD3D)
		g_pAQU_TransferSite->m_bForceD3D = false;

	// restore CreateDevice pointer
	DWORD dwProtect;
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[16], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[16] = *(PUINT_PTR)&D3D9_CreateDevice_Super;
		D3D9_CreateDevice_Super = NULL;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[16], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// get function pointers by vmtable
	if (hr == D3D_OK)
	{
		// get a device pointer and the vmtable
		LPDIRECT3DDEVICE9 pDevice = *ppReturnedDeviceInterface;
		if (!pDevice)
		{
			OutputDebugString(L"Aquilinus : Device null pointer!");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
			return hr;
		}
		D3D9_IDirect3DDevice9_VMTable = (PUINT_PTR)*(PUINT_PTR)*ppReturnedDeviceInterface;

		// create all
		if (pDirect3D9Ex)
			D3D9_CreateAll(pDirect3D9Ex, pDevice, pPresentationParameters);
		else
			D3D9_CreateAll(pcD3D, pDevice, pPresentationParameters);
	}
	return hr;
}

/**
* Creates all data necessary for hooking and finally creates the repatching thread.
***/
HRESULT D3D9_CreateAll(LPDIRECT3D9 pParent, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	// create the aquilinus detour class
	pDCL_IDirect3DDevice9 = new DCL_IDirect3DDevice9(g_pAQU_TransferSite, pParent);

	// set old function pointers
	if (FAILED(pDCL_IDirect3DDevice9->SetSuperFunctionPointers(D3D9_IDirect3DDevice9_VMTable)))
		OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

	// create any d3d 9 interface to get its vmtable
	// first...IDirect3DTexture9 + IDirect3DBaseTexture9
	LPDIRECT3DTEXTURE9 pIDirect3DTexture9;
	LPDIRECT3DBASETEXTURE9 pIDirect3DBaseTexture9;
	LPDIRECT3DRESOURCE9 pIDirect3DResource9;
	if (SUCCEEDED(pDevice->CreateTexture(16, 16, 0, D3DUSAGE_RENDERTARGET, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DTexture9, NULL)))
	{
		D3D9_IDirect3DTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DTexture9 = new DCL_IDirect3DTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DTexture9->SetSuperFunctionPointers(D3D9_IDirect3DTexture9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		// query IDirect3DBaseTexture9
		if (FAILED(pIDirect3DTexture9->QueryInterface(IID_IDirect3DBaseTexture9, (void**)&pIDirect3DBaseTexture9)))
		{
			OutputDebugString(L"Aquilinus : Failed to query base texture. (d3d9)");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}
		else
		{
			D3D9_IDirect3DBaseTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DBaseTexture9;
			// create the aquilinus detour class
			pDCL_IDirect3DBaseTexture9 = new DCL_IDirect3DBaseTexture9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DBaseTexture9->SetSuperFunctionPointers(D3D9_IDirect3DBaseTexture9_VMTable)))
				OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

			// query IDirect3DResource9
			if (FAILED(pIDirect3DBaseTexture9->QueryInterface(IID_IDirect3DResource9, (void**)&pIDirect3DResource9)))
			{
				OutputDebugString(L"Aquilinus : Failed to query resource. (d3d9)");
				CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
			}
			else
			{
				D3D9_IDirect3DResource9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DResource9;
				// create the aquilinus detour class
				pDCL_IDirect3DResource9 = new DCL_IDirect3DResource9(g_pAQU_TransferSite);

				// set old function pointers
				if (FAILED(pDCL_IDirect3DResource9->SetSuperFunctionPointers(D3D9_IDirect3DResource9_VMTable)))
					OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

				pIDirect3DResource9->Release();
			}

			pIDirect3DBaseTexture9->Release();
		}

		pIDirect3DTexture9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DCubeTexture9
	LPDIRECT3DCUBETEXTURE9 pIDirect3DCubeTexture9;
	if (SUCCEEDED(pDevice->CreateCubeTexture(16, 0, D3DUSAGE_RENDERTARGET, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DCubeTexture9, NULL)))
	{
		D3D9_IDirect3DCubeTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DCubeTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DCubeTexture9 = new DCL_IDirect3DCubeTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointers(D3D9_IDirect3DCubeTexture9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DCubeTexture9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Cube Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DVolumeTexture9
	LPDIRECT3DVOLUMETEXTURE9 pIDirect3DVolumeTexture9;
	if (SUCCEEDED(pDevice->CreateVolumeTexture(16, 16, 16, 0, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DVolumeTexture9, NULL)))
	{
		D3D9_IDirect3DVolumeTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DVolumeTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DVolumeTexture9 = new DCL_IDirect3DVolumeTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVolumeTexture9->SetSuperFunctionPointers(D3D9_IDirect3DVolumeTexture9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		// IDirect3DVolume9
		LPDIRECT3DVOLUME9 pIDirect3DVolume9;
		if (SUCCEEDED(pIDirect3DVolumeTexture9->GetVolumeLevel(0, &pIDirect3DVolume9)))
		{
			D3D9_IDirect3DVolume9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DVolume9;
			// create the aquilinus detour class
			pDCL_IDirect3DVolume9 = new DCL_IDirect3DVolume9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DVolume9->SetSuperFunctionPointers(D3D9_IDirect3DVolume9_VMTable)))
				OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

			pIDirect3DVolume9->Release();
		}
		else
		{
			OutputDebugString(L"Aquilinus : Failed to get Volume (d3d9).");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}

		pIDirect3DVolumeTexture9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Volume Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DSurface9
	LPDIRECT3DSURFACE9 pIDirect3DSurface9;
	if (SUCCEEDED(pDevice->CreateOffscreenPlainSurface(16, 16, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DSurface9, NULL)))
	{
		D3D9_IDirect3DSurface9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DSurface9;
		// create the aquilinus detour class
		pDCL_IDirect3DSurface9 = new DCL_IDirect3DSurface9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DSurface9->SetSuperFunctionPointers(D3D9_IDirect3DSurface9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DSurface9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to Surface (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DSwapChain9
	LPDIRECT3DSWAPCHAIN9 pIDirect3DSwapChain9;
	if (SUCCEEDED(pDevice->CreateAdditionalSwapChain(pPresentationParameters, &pIDirect3DSwapChain9)))
	{
		OutputDebugString(L"Aquilinus : Additional swap chain created !");
		D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DSwapChain9;
		// create the aquilinus detour class
		pDCL_IDirect3DSwapChain9 = new DCL_IDirect3DSwapChain9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DSwapChain9->SetSuperFunctionPointers(D3D9_IDirect3DSwapChain9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DSwapChain9->Release();
	}
	else
	{
		pDevice->GetSwapChain(0, &pIDirect3DSwapChain9);
		if (pIDirect3DSwapChain9)
		{
			OutputDebugString(L"Aquilinus : Got swap chain (d3d9) !");
			D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DSwapChain9;
			// create the aquilinus detour class
			pDCL_IDirect3DSwapChain9 = new DCL_IDirect3DSwapChain9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DSwapChain9->SetSuperFunctionPointers(D3D9_IDirect3DSwapChain9_VMTable)))
				OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

			pIDirect3DSwapChain9->Release();
		}
		else
		{
			OutputDebugString(L"Aquilinus : Failed to create Swap Chain (d3d9).");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}
	}

	// IDirect3DIndexBuffer9
	LPDIRECT3DINDEXBUFFER9 pIDirect3DIndexBuffer9;
	if (SUCCEEDED(pDevice->CreateIndexBuffer(16, 0, D3DFORMAT::D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIDirect3DIndexBuffer9, NULL)))
	{
		D3D9_IDirect3DIndexBuffer9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DIndexBuffer9;
		// create the aquilinus detour class
		pDCL_IDirect3DIndexBuffer9 = new DCL_IDirect3DIndexBuffer9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointers(D3D9_IDirect3DIndexBuffer9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DIndexBuffer9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Index Buffer (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DVertexBuffer9
	LPDIRECT3DVERTEXBUFFER9 pIDirect3DVertexBuffer9;
	if (SUCCEEDED(pDevice->CreateVertexBuffer(3 * sizeof(float), 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &pIDirect3DVertexBuffer9, NULL)))
	{
		D3D9_IDirect3DVertexBuffer9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DVertexBuffer9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexBuffer9 = new DCL_IDirect3DVertexBuffer9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexBuffer9->SetSuperFunctionPointers(D3D9_IDirect3DVertexBuffer9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DVertexBuffer9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Vertex Buffer (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DPixelShader9
	LPDIRECT3DPIXELSHADER9 pIDirect3DPixelShader9;
	static const char* PixelShader =
		"sampler2D TexMap0;\n"
		"float4 Col = {1.0,1.0,1.0,1.0};\n"
		"float4 PSMain(float2 Tex : TEXCOORD0) : COLOR\n"
		"{\n"
		"float2 newPos = Tex;\n"
		"float4 tColor = Col;\n"
		"return tColor;\n"
		"}\n"
		;
	LPD3DXBUFFER pShader;
	LPD3DXCONSTANTTABLE	ct = nullptr;
	HRESULT hr = D3DXCompileShader(PixelShader, (UINT)strlen(PixelShader), NULL, NULL, "PSMain", "ps_3_0", NULL, &pShader, NULL, &ct);
	if (SUCCEEDED(hr)) hr = pDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &pIDirect3DPixelShader9);
	else OutputDebugString(L"Aquilinus : Failed to compile Pixel Shader (d3d9).");
	if (SUCCEEDED(hr))
	{
		D3D9_IDirect3DPixelShader9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DPixelShader9;
		// create the aquilinus detour class
		pDCL_IDirect3DPixelShader9 = new DCL_IDirect3DPixelShader9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DPixelShader9->SetSuperFunctionPointers(D3D9_IDirect3DPixelShader9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		ct->Release();
		pShader->Release();
		pIDirect3DPixelShader9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Pixel Shader (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		pShader->Release();
		ct->Release();
	}

	// IDirect3DVertexShader9
	LPDIRECT3DVERTEXSHADER9 pIDirect3DVertexShader9;
	static const char* VertexShader =
		"float4 VSMain(float4 inPos : POSITION0) : POSITION\n"
		"{\n"
		"float4 Output = inPos;\n"
		"return Output;\n"
		"}\n"
		;
	pShader = nullptr;
	ct = nullptr;
	hr = D3DXCompileShader(VertexShader, (UINT)strlen(VertexShader), NULL, NULL, "VSMain", "vs_3_0", NULL, &pShader, NULL, &ct);
	if (SUCCEEDED(hr)) hr = pDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &pIDirect3DVertexShader9);
	else OutputDebugString(L"Aquilinus : Failed to compile Vertex Shader (d3d9).");
	if (SUCCEEDED(hr))
	{
		D3D9_IDirect3DVertexShader9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DVertexShader9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexShader9 = new DCL_IDirect3DVertexShader9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexShader9->SetSuperFunctionPointers(D3D9_IDirect3DVertexShader9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		ct->Release();
		pShader->Release();
		pIDirect3DVertexShader9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Vertex Shader (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		pShader->Release();
		ct->Release();
	}

	// IDirect3DQuery9
	LPDIRECT3DQUERY9 pIDirect3DQuery9;
	if (SUCCEEDED(pDevice->CreateQuery(D3DQUERYTYPE_EVENT, &pIDirect3DQuery9)))
	{
		D3D9_IDirect3DQuery9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DQuery9;
		// create the aquilinus detour class
		pDCL_IDirect3DQuery9 = new DCL_IDirect3DQuery9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DQuery9->SetSuperFunctionPointers(D3D9_IDirect3DQuery9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DQuery9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create Query (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DStateBlock9
	LPDIRECT3DSTATEBLOCK9 pIDirect3DStateBlock9;
	if (SUCCEEDED(pDevice->CreateStateBlock(D3DSTATEBLOCKTYPE::D3DSBT_ALL, &pIDirect3DStateBlock9)))
	{
		D3D9_IDirect3DStateBlock9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DStateBlock9;
		// create the aquilinus detour class
		pDCL_IDirect3DStateBlock9 = new DCL_IDirect3DStateBlock9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DStateBlock9->SetSuperFunctionPointers(D3D9_IDirect3DStateBlock9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DStateBlock9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create StateBlock (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DVertexDeclaration9
	LPDIRECT3DVERTEXDECLARATION9 pIDirect3DVertexDeclaration9;
	D3DVERTEXELEMENT9 dwDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
		D3DDECLUSAGE_POSITION, 0
		},
		D3DDECL_END()
	};
	if (SUCCEEDED(pDevice->CreateVertexDeclaration(dwDecl, &pIDirect3DVertexDeclaration9)))
	{
		D3D9_IDirect3DVertexDeclaration9_VMTable = (PUINT_PTR)*(PUINT_PTR)pIDirect3DVertexDeclaration9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexDeclaration9 = new DCL_IDirect3DVertexDeclaration9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexDeclaration9->SetSuperFunctionPointers(D3D9_IDirect3DVertexDeclaration9_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		pIDirect3DVertexDeclaration9->Release();
	}
	else
	{
		OutputDebugString(L"Aquilinus : Failed to create VertexDeclaration (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// finally, create the repatching thread
	HANDLE pThread = nullptr;
	if ((pThread = CreateThread(NULL, 0, D3D9_VMT_Repatch, NULL, 0, NULL)) == NULL)
		return D3DERR_INVALIDCALL;

	// wait for project init and the thread to be finished
	while ((g_pAquilinusConfig->eProjectStage != AQU_ProjectStage::WorkingArea) &&
		(g_pAquilinusConfig->eProjectStage != AQU_ProjectStage::ComplementedInjected))
		Sleep(100);
	WaitForSingleObject(pThread, INFINITE);
	Sleep(100);

	return hr;
}

/**
* Thread detours the d3d9 object.
***/
DWORD WINAPI D3D9_Detour(LPVOID Param)
{
	D3D9_Direct3DCreate9_Super = (D3D9_Direct3DCreate9)DetourFunc((BYTE*)Direct3DCreate9, (BYTE*)D3D9_Direct3DCreate9_Detour, JMP32_SZ);
	OutputDebugString(L"Aquilinus: D3D9_Detour is set.");
	CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);

	// detour D3DX methods
	D3D9_D3DXLoadSurfaceFromFileA_Super = (D3D9_D3DXLoadSurfaceFromFileA)DetourFunc((BYTE*)D3DXLoadSurfaceFromFileA, (BYTE*)D3D9_D3DXLoadSurfaceFromFileA_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromFileW_Super = (D3D9_D3DXLoadSurfaceFromFileW)DetourFunc((BYTE*)D3DXLoadSurfaceFromFileW, (BYTE*)D3D9_D3DXLoadSurfaceFromFileW_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromResourceA_Super = (D3D9_D3DXLoadSurfaceFromResourceA)DetourFunc((BYTE*)D3DXLoadSurfaceFromResourceA, (BYTE*)D3D9_D3DXLoadSurfaceFromResourceA_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromResourceW_Super = (D3D9_D3DXLoadSurfaceFromResourceW)DetourFunc((BYTE*)D3DXLoadSurfaceFromResourceW, (BYTE*)D3D9_D3DXLoadSurfaceFromResourceW_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromFileInMemory_Super = (D3D9_D3DXLoadSurfaceFromFileInMemory)DetourFunc((BYTE*)D3DXLoadSurfaceFromFileInMemory, (BYTE*)D3D9_D3DXLoadSurfaceFromFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromSurface_Super = (D3D9_D3DXLoadSurfaceFromSurface)DetourFunc((BYTE*)D3DXLoadSurfaceFromSurface, (BYTE*)D3D9_D3DXLoadSurfaceFromSurface_Detour, JMP32_SZ);
	D3D9_D3DXLoadSurfaceFromMemory_Super = (D3D9_D3DXLoadSurfaceFromMemory)DetourFunc((BYTE*)D3DXLoadSurfaceFromMemory, (BYTE*)D3D9_D3DXLoadSurfaceFromMemory_Detour, JMP32_SZ);
	D3D9_D3DXSaveSurfaceToFileA_Super = (D3D9_D3DXSaveSurfaceToFileA)DetourFunc((BYTE*)D3DXSaveSurfaceToFileA, (BYTE*)D3D9_D3DXSaveSurfaceToFileA_Detour, JMP32_SZ);
	D3D9_D3DXSaveSurfaceToFileW_Super = (D3D9_D3DXSaveSurfaceToFileW)DetourFunc((BYTE*)D3DXSaveSurfaceToFileW, (BYTE*)D3D9_D3DXSaveSurfaceToFileW_Detour, JMP32_SZ);
	D3D9_D3DXSaveSurfaceToFileInMemory_Super = (D3D9_D3DXSaveSurfaceToFileInMemory)DetourFunc((BYTE*)D3DXSaveSurfaceToFileInMemory, (BYTE*)D3D9_D3DXSaveSurfaceToFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromFileA_Super = (D3D9_D3DXLoadVolumeFromFileA)DetourFunc((BYTE*)D3DXLoadVolumeFromFileA, (BYTE*)D3D9_D3DXLoadVolumeFromFileA_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromFileW_Super = (D3D9_D3DXLoadVolumeFromFileW)DetourFunc((BYTE*)D3DXLoadVolumeFromFileW, (BYTE*)D3D9_D3DXLoadVolumeFromFileW_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromResourceA_Super = (D3D9_D3DXLoadVolumeFromResourceA)DetourFunc((BYTE*)D3DXLoadVolumeFromResourceA, (BYTE*)D3D9_D3DXLoadVolumeFromResourceA_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromResourceW_Super = (D3D9_D3DXLoadVolumeFromResourceW)DetourFunc((BYTE*)D3DXLoadVolumeFromResourceW, (BYTE*)D3D9_D3DXLoadVolumeFromResourceW_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromFileInMemory_Super = (D3D9_D3DXLoadVolumeFromFileInMemory)DetourFunc((BYTE*)D3DXLoadVolumeFromFileInMemory, (BYTE*)D3D9_D3DXLoadVolumeFromFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromVolume_Super = (D3D9_D3DXLoadVolumeFromVolume)DetourFunc((BYTE*)D3DXLoadVolumeFromVolume, (BYTE*)D3D9_D3DXLoadVolumeFromVolume_Detour, JMP32_SZ);
	D3D9_D3DXLoadVolumeFromMemory_Super = (D3D9_D3DXLoadVolumeFromMemory)DetourFunc((BYTE*)D3DXLoadVolumeFromMemory, (BYTE*)D3D9_D3DXLoadVolumeFromMemory_Detour, JMP32_SZ);
	D3D9_D3DXSaveVolumeToFileA_Super = (D3D9_D3DXSaveVolumeToFileA)DetourFunc((BYTE*)D3DXSaveVolumeToFileA, (BYTE*)D3D9_D3DXSaveVolumeToFileA_Detour, JMP32_SZ);
	D3D9_D3DXSaveVolumeToFileW_Super = (D3D9_D3DXSaveVolumeToFileW)DetourFunc((BYTE*)D3DXSaveVolumeToFileW, (BYTE*)D3D9_D3DXSaveVolumeToFileW_Detour, JMP32_SZ);
	D3D9_D3DXSaveVolumeToFileInMemory_Super = (D3D9_D3DXSaveVolumeToFileInMemory)DetourFunc((BYTE*)D3DXSaveVolumeToFileInMemory, (BYTE*)D3D9_D3DXSaveVolumeToFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXCreateTexture_Super = (D3D9_D3DXCreateTexture)DetourFunc((BYTE*)D3DXCreateTexture, (BYTE*)D3D9_D3DXCreateTexture_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTexture_Super = (D3D9_D3DXCreateCubeTexture)DetourFunc((BYTE*)D3DXCreateCubeTexture, (BYTE*)D3D9_D3DXCreateCubeTexture_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTexture_Super = (D3D9_D3DXCreateVolumeTexture)DetourFunc((BYTE*)D3DXCreateVolumeTexture, (BYTE*)D3D9_D3DXCreateVolumeTexture_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileA_Super = (D3D9_D3DXCreateTextureFromFileA)DetourFunc((BYTE*)D3DXCreateTextureFromFileA, (BYTE*)D3D9_D3DXCreateTextureFromFileA_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileW_Super = (D3D9_D3DXCreateTextureFromFileW)DetourFunc((BYTE*)D3DXCreateTextureFromFileW, (BYTE*)D3D9_D3DXCreateTextureFromFileW_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileA_Super = (D3D9_D3DXCreateCubeTextureFromFileA)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileA, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileA_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileW_Super = (D3D9_D3DXCreateCubeTextureFromFileW)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileW, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileW_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileA_Super = (D3D9_D3DXCreateVolumeTextureFromFileA)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileA, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileA_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileW_Super = (D3D9_D3DXCreateVolumeTextureFromFileW)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileW, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileW_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromResourceA_Super = (D3D9_D3DXCreateTextureFromResourceA)DetourFunc((BYTE*)D3DXCreateTextureFromResourceA, (BYTE*)D3D9_D3DXCreateTextureFromResourceA_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromResourceW_Super = (D3D9_D3DXCreateTextureFromResourceW)DetourFunc((BYTE*)D3DXCreateTextureFromResourceW, (BYTE*)D3D9_D3DXCreateTextureFromResourceW_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromResourceA_Super = (D3D9_D3DXCreateCubeTextureFromResourceA)DetourFunc((BYTE*)D3DXCreateCubeTextureFromResourceA, (BYTE*)D3D9_D3DXCreateCubeTextureFromResourceA_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromResourceW_Super = (D3D9_D3DXCreateCubeTextureFromResourceW)DetourFunc((BYTE*)D3DXCreateCubeTextureFromResourceW, (BYTE*)D3D9_D3DXCreateCubeTextureFromResourceW_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromResourceA_Super = (D3D9_D3DXCreateVolumeTextureFromResourceA)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromResourceA, (BYTE*)D3D9_D3DXCreateVolumeTextureFromResourceA_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromResourceW_Super = (D3D9_D3DXCreateVolumeTextureFromResourceW)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromResourceW, (BYTE*)D3D9_D3DXCreateVolumeTextureFromResourceW_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileExA_Super = (D3D9_D3DXCreateTextureFromFileExA)DetourFunc((BYTE*)D3DXCreateTextureFromFileExA, (BYTE*)D3D9_D3DXCreateTextureFromFileExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileExW_Super = (D3D9_D3DXCreateTextureFromFileExW)DetourFunc((BYTE*)D3DXCreateTextureFromFileExW, (BYTE*)D3D9_D3DXCreateTextureFromFileExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileExA_Super = (D3D9_D3DXCreateCubeTextureFromFileExA)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileExA, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileExW_Super = (D3D9_D3DXCreateCubeTextureFromFileExW)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileExW, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileExA_Super = (D3D9_D3DXCreateVolumeTextureFromFileExA)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileExA, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileExW_Super = (D3D9_D3DXCreateVolumeTextureFromFileExW)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileExW, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromResourceExA_Super = (D3D9_D3DXCreateTextureFromResourceExA)DetourFunc((BYTE*)D3DXCreateTextureFromResourceExA, (BYTE*)D3D9_D3DXCreateTextureFromResourceExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromResourceExW_Super = (D3D9_D3DXCreateTextureFromResourceExW)DetourFunc((BYTE*)D3DXCreateTextureFromResourceExW, (BYTE*)D3D9_D3DXCreateTextureFromResourceExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromResourceExA_Super = (D3D9_D3DXCreateCubeTextureFromResourceExA)DetourFunc((BYTE*)D3DXCreateCubeTextureFromResourceExA, (BYTE*)D3D9_D3DXCreateCubeTextureFromResourceExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromResourceExW_Super = (D3D9_D3DXCreateCubeTextureFromResourceExW)DetourFunc((BYTE*)D3DXCreateCubeTextureFromResourceExW, (BYTE*)D3D9_D3DXCreateCubeTextureFromResourceExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromResourceExA_Super = (D3D9_D3DXCreateVolumeTextureFromResourceExA)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromResourceExA, (BYTE*)D3D9_D3DXCreateVolumeTextureFromResourceExA_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromResourceExW_Super = (D3D9_D3DXCreateVolumeTextureFromResourceExW)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromResourceExW, (BYTE*)D3D9_D3DXCreateVolumeTextureFromResourceExW_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileInMemory_Super = (D3D9_D3DXCreateTextureFromFileInMemory)DetourFunc((BYTE*)D3DXCreateTextureFromFileInMemory, (BYTE*)D3D9_D3DXCreateTextureFromFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileInMemory_Super = (D3D9_D3DXCreateCubeTextureFromFileInMemory)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileInMemory, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileInMemory_Super = (D3D9_D3DXCreateVolumeTextureFromFileInMemory)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileInMemory, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXCreateTextureFromFileInMemoryEx_Super = (D3D9_D3DXCreateTextureFromFileInMemoryEx)DetourFunc((BYTE*)D3DXCreateTextureFromFileInMemoryEx, (BYTE*)D3D9_D3DXCreateTextureFromFileInMemoryEx_Detour, JMP32_SZ);
	D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Super = (D3D9_D3DXCreateCubeTextureFromFileInMemoryEx)DetourFunc((BYTE*)D3DXCreateCubeTextureFromFileInMemoryEx, (BYTE*)D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Detour, JMP32_SZ);
	D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Super = (D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx)DetourFunc((BYTE*)D3DXCreateVolumeTextureFromFileInMemoryEx, (BYTE*)D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Detour, JMP32_SZ);
	D3D9_D3DXSaveTextureToFileA_Super = (D3D9_D3DXSaveTextureToFileA)DetourFunc((BYTE*)D3DXSaveTextureToFileA, (BYTE*)D3D9_D3DXSaveTextureToFileA_Detour, JMP32_SZ);
	D3D9_D3DXSaveTextureToFileW_Super = (D3D9_D3DXSaveTextureToFileW)DetourFunc((BYTE*)D3DXSaveTextureToFileW, (BYTE*)D3D9_D3DXSaveTextureToFileW_Detour, JMP32_SZ);
	D3D9_D3DXSaveTextureToFileInMemory_Super = (D3D9_D3DXSaveTextureToFileInMemory)DetourFunc((BYTE*)D3DXSaveTextureToFileInMemory, (BYTE*)D3D9_D3DXSaveTextureToFileInMemory_Detour, JMP32_SZ);
	D3D9_D3DXFilterTexture_Super = (D3D9_D3DXFilterTexture)DetourFunc((BYTE*)D3DXFilterTexture, (BYTE*)D3D9_D3DXFilterTexture_Detour, JMP32_SZ);
	D3D9_D3DXFillTexture_Super = (D3D9_D3DXFillTexture)DetourFunc((BYTE*)D3DXFillTexture, (BYTE*)D3D9_D3DXFillTexture_Detour, JMP32_SZ);
	D3D9_D3DXFillCubeTexture_Super = (D3D9_D3DXFillCubeTexture)DetourFunc((BYTE*)D3DXFillCubeTexture, (BYTE*)D3D9_D3DXFillCubeTexture_Detour, JMP32_SZ);
	D3D9_D3DXFillVolumeTexture_Super = (D3D9_D3DXFillVolumeTexture)DetourFunc((BYTE*)D3DXFillVolumeTexture, (BYTE*)D3D9_D3DXFillVolumeTexture_Detour, JMP32_SZ);
	D3D9_D3DXFillTextureTX_Super = (D3D9_D3DXFillTextureTX)DetourFunc((BYTE*)D3DXFillTextureTX, (BYTE*)D3D9_D3DXFillTextureTX_Detour, JMP32_SZ);
	D3D9_D3DXFillCubeTextureTX_Super = (D3D9_D3DXFillCubeTextureTX)DetourFunc((BYTE*)D3DXFillCubeTextureTX, (BYTE*)D3D9_D3DXFillCubeTextureTX_Detour, JMP32_SZ);
	D3D9_D3DXFillVolumeTextureTX_Super = (D3D9_D3DXFillVolumeTextureTX)DetourFunc((BYTE*)D3DXFillVolumeTextureTX, (BYTE*)D3D9_D3DXFillVolumeTextureTX_Detour, JMP32_SZ);
	D3D9_D3DXComputeNormalMap_Super = (D3D9_D3DXComputeNormalMap)DetourFunc((BYTE*)D3DXComputeNormalMap, (BYTE*)D3D9_D3DXComputeNormalMap_Detour, JMP32_SZ);
	return 0;
}

/**
* Thread to get the d3d9 device by mask.
***/
DWORD WINAPI D3D9_VMT_Mask(LPVOID Param)
{
	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);

#ifdef _WIN64
	// TODO !! 64 bit !!
#else
	// get the root vmtable by pattern search
	DWORD dwDXDevice = (DWORD)FindPattern((DWORD)GetModuleHandle(L"d3d9.dll"), 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	DWORD* pVtable = *(DWORD**)(dwDXDevice + 2);

	OutputDebug("D3D9_IDirect3DDevice9_VMTable : %x", D3D9_IDirect3DDevice9_VMTable);
	OutputDebug("pVtable: %x", pVtable);
	OutputDebug("D3D9_IDirect3DDevice9_VMTable[0] : %x", D3D9_IDirect3DDevice9_VMTable[0]);
	OutputDebug("pVtable[0] : %x", pVtable[0]);

	HMODULE hm = GetModuleHandle(g_pAquilinusConfig->szProcessName);
	OutputDebug("hm %x", hm);
	for (PUINT_PTR pdwScan = (PUINT_PTR)hm; pdwScan < 0x10000000 + (PUINT_PTR)hm; pdwScan++)
	{
		if (!IsBadReadPtr(pdwScan, 4))
		{
			if (pdwScan[0] == pVtable[0])
			{
				OutputDebug("Scan %x", pdwScan);
				OutputDebug("pdwScan[0] %x", pdwScan[0]);

				// TODO !! COMPARE WHOLE VMTABLE
			}
		}
	}

	if (D3D9_IDirect3DDevice9_VMTable)
	{
		// get a device pointer, create all
		PUINT_PTR pcDevice = nullptr;
		*pcDevice = (UINT_PTR)D3D9_IDirect3DDevice9_VMTable;
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		D3D9_CreateAll(NULL, (LPDIRECT3DDEVICE9)pcDevice, &d3dpp);
	}
#endif
	return D3D_OK;
}

/**
* Thread to create the virtual method table for DirectX v9.0.
***/
DWORD WINAPI D3D9_VMT_Create(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	LPDIRECT3D9 Direct3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	if (Direct3D_Object == NULL)
		return D3DERR_INVALIDCALL;

	D3D9_IDirect3D9_VMTable = (PUINT_PTR)*(PUINT_PTR)Direct3D_Object;
	Direct3D_Object->Release();

	DWORD dwProtect;

	// catch CreateDevice, detour to D3D9_CreateDevice_Detour
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[16], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CreateDevice_Super = D3D9_IDirect3D9_VMTable[16];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[16] = (UINT_PTR)D3D9_CreateDevice_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[16], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch QueryInterface, detour to D3D9_QueryInterface_Detour
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[0], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_QueryInterface_Super = D3D9_IDirect3D9_VMTable[0];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[0] = (UINT_PTR)D3D9_QueryInterface_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[0], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch CheckDeviceType, detour to D3D9_CheckDeviceType_Detour
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[9], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CheckDeviceType_Super = D3D9_IDirect3D9_VMTable[9];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[9] = (UINT_PTR)D3D9_CheckDeviceType_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[9], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch CheckDeviceFormat, detour to D3D9_CheckDeviceFormat_Detour
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[10], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CheckDeviceFormat_Super = D3D9_IDirect3D9_VMTable[10];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[10] = (UINT_PTR)D3D9_CheckDeviceFormat_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[10], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch CheckDeviceMultiSampleType, detour to Detour method
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[11], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CheckDeviceMultiSampleType_Super = D3D9_IDirect3D9_VMTable[11];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[11] = (UINT_PTR)D3D9_CheckDeviceMultiSampleType_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[11], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch CheckDepthStencilMatch, detour to Detour method
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[12], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CheckDepthStencilMatch_Super = D3D9_IDirect3D9_VMTable[12];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[12] = (UINT_PTR)D3D9_CheckDepthStencilMatch_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[12], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	// catch CheckDeviceFormatConversion, detour to Detour method
	if (VirtualProtect(&D3D9_IDirect3D9_VMTable[13], sizeof(UINT_PTR), PAGE_READWRITE, &dwProtect) != 0)
	{
		*(PUINT_PTR)&D3D9_CheckDeviceFormatConversion_Super = D3D9_IDirect3D9_VMTable[13];
		*(PUINT_PTR)&D3D9_IDirect3D9_VMTable[13] = (UINT_PTR)D3D9_CheckDeviceFormatConversion_Detour;

		if (VirtualProtect(&D3D9_IDirect3D9_VMTable[13], sizeof(UINT_PTR), dwProtect, &dwProtect) == 0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;

	return D3D_OK;
}

/**
* Thread to set detour functions.
***/
DWORD WINAPI D3D9_VMT_Repatch(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);

	// which technique ? first the device
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DDevice9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DDevice9_VMTable();
		break;
	}

	// IDirect3DTexture9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DTexture9)
			D3D9_IDirect3DTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DTexture9;
		Override_D3D9_IDirect3DTexture9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DTexture9_VMTable();
		break;
	}

	// IDirect3DBaseTexture9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DBaseTexture9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DBaseTexture9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DBaseTexture9_VMTable();
		break;
	}

	// IDirect3DResource9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DResource9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DResource9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DResource9_VMTable();
		break;
	}

	// IDirect3DCubeTexture9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DCubeTexture9)
			D3D9_IDirect3DCubeTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DCubeTexture9;
		Override_D3D9_IDirect3DCubeTexture9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DCubeTexture9_VMTable();
		break;
	}

	// IDirect3DVolumeTexture9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DVolumeTexture9)
			D3D9_IDirect3DVolumeTexture9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVolumeTexture9;
		Override_D3D9_IDirect3DVolumeTexture9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DVolumeTexture9_VMTable();
		break;
	}

	// IDirect3DVolume9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DVolume9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DVolume9_VMTable();
		break;
	}

	// IDirect3DSurface9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DSurface9)
			D3D9_IDirect3DSurface9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DSurface9;
		Override_D3D9_IDirect3DSurface9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DSurface9_VMTable();
		break;
	}

	// IDirect3DSwapChain9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DSwapChain9)
			D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DSwapChain9;
		Override_D3D9_IDirect3DSwapChain9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DSwapChain9_VMTable();
		break;
	}

	// IDirect3DIndexBuffer9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DIndexBuffer9)
			D3D9_IDirect3DIndexBuffer9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DIndexBuffer9;
		Override_D3D9_IDirect3DIndexBuffer9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DIndexBuffer9_VMTable();
		break;
	}

	// IDirect3DVertexBuffer9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DVertexBuffer9)
			D3D9_IDirect3DVertexBuffer9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVertexBuffer9;
		Override_D3D9_IDirect3DVertexBuffer9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DVertexBuffer9_VMTable();
		break;
	}

	// IDirect3DPixelShader9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DPixelShader9)
			D3D9_IDirect3DPixelShader9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DPixelShader9;
		Override_D3D9_IDirect3DPixelShader9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DPixelShader9_VMTable();
		break;
	}

	// IDirect3DVertexShader9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DVertexShader9)
			D3D9_IDirect3DVertexShader9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVertexShader9;
		Override_D3D9_IDirect3DVertexShader9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DVertexShader9_VMTable();
		break;
	}

	// IDirect3DQuery9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DQuery9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DQuery9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DQuery9_VMTable();
		break;
	}

	// IDirect3DStateBlock9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9])
	{
	case AQU_InjectionTechniques::VMTable:
		if (g_pAQU_TransferSite->m_pIDirect3DStateBlock9)
			D3D9_IDirect3DStateBlock9_VMTable = (PUINT_PTR)*(PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DStateBlock9;
		Override_D3D9_IDirect3DStateBlock9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DStateBlock9_VMTable();
		break;
	}

	// IDirect3DVertexDeclaration9
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexDeclaration9])
	{
	case AQU_InjectionTechniques::VMTable:
		Override_D3D9_IDirect3DVertexDeclaration9_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D9_IDirect3DVertexDeclaration9_VMTable();
		break;
	}

	return 1;
}

#pragma endregion

#pragma region DirectX 10.0

/**
* Thread to get the d3d10 device by mask.
***/
DWORD WINAPI D3D10_VMT_Mask(LPVOID Param)
{
	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);


#ifdef AQUILINUS_64
	// TODO !! 64 bit !!
#else
	ID3D10Device *pDevice;

	// find the window we want to inject
	//HWND hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	//while (!hWnd) hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	HWND hWnd = GetForegroundWindow();

	// create a sample device
	if (FAILED(D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE, NULL, 0,
		D3D10_SDK_VERSION, &pDevice)))
	{
		OutputDebug("Failed to create directX device!");
		return E_FAIL;
	}

	// set device vtable
	D3D10_ID3D10Device_VMTable = (PUINT_PTR)pDevice;
	D3D10_ID3D10Device_VMTable = (PUINT_PTR)D3D10_ID3D10Device_VMTable[0];

	// get first vmtable value
	UINT_PTR dwVMTableValue = D3D10_ID3D10Device_VMTable[0];
	OutputDebug("dwVMTableValue %x", dwVMTableValue);

	// release device
	{
		if (pDevice)
		{
			(pDevice)->Release();
			(pDevice) = NULL;
		}
	}

	// get the root vmtable by pattern search - OLD !!!
	// the root vmtables start with C3 90 90 90 90 90 90 90
	// add 0x19C to the found address to get the ID3D10Device vmtable
	//DWORD dwDXVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x128000, (PBYTE)"\xC3\x90\x90\x90\x90\x90\x90\x90", "xxxxxxxx");

	// get the vmtable entries for the D3D10.0 and D3D10.1
	// devices (space is 404 bytes = 101 * sizeof(UINT_PTR)....
	// D3D10.1 device methods number = 101)
	UINT_PTR dwHandle = NULL;
	while (!dwHandle) dwHandle = (UINT_PTR)GetModuleHandle(L"d3d11.dll");
	UINT_PTR dwDXVMTableRoot = (UINT_PTR)FindTwinDword((UINT_PTR)dwHandle, 0x128000, (DWORD)dwVMTableValue, 404);
	if (dwDXVMTableRoot)
	{
		// add 404 for the D3D10.0 device, dwDXVMTableRoot = D3D10.1 device
		D3D10_ID3D10Device_VMTable = (PUINT_PTR)(dwDXVMTableRoot + 404);

		// get the ID3D10Device VMTable, set SwapChain VMTable to nullptr meanwhile...
		D3D10_IDXGISwapChain_VMTable = nullptr;

		// create the aquilinus detour classes
		pDCL_ID3D10Device = new DCL_ID3D10Device(g_pAQU_TransferSite);
		pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);

		OutputDebug("dwDXVMTableRoot %x", dwDXVMTableRoot);
		OutputDebug("D3D10_ID3D10Device_VMTable: %x", D3D10_ID3D10Device_VMTable);
		OutputDebug("D3D10_ID3D10Device_Draw %x", D3D10_ID3D10Device_Draw);

		if (FAILED(pDCL_ID3D10Device->SetSuperFunctionPointers(D3D10_ID3D10Device_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		// create the thread to override the virtual methods table
		if (CreateThread(NULL, 0, D3D10_VMT_Repatch, NULL, 0, NULL) == NULL)
			return E_FAIL;
	}
#endif

	return D3D_OK;
}

/**
* Thread to create the virtual method table for DirectX v10.0.
***/
DWORD WINAPI D3D10_VMT_Create(LPVOID Param)
{
	ID3D10Device *pDevice;

	// find the window we want to inject
	//HWND hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	//while (!hWnd) hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	HWND hWnd = GetForegroundWindow();

	// create a sample device and swapchain
	IDXGISwapChain* pSwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;//((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	// TODO !! WINDOWED/FULLSCREEN SETTING

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE, NULL, 0,
		D3D10_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice)))
	{
		OutputDebug("Failed to create directX device and swapchain!");
		return E_FAIL;
	}

	// set the VTable for the swapchain class
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

	// set device vtable
	D3D10_ID3D10Device_VMTable = (UINT_PTR*)pDevice;
	D3D10_ID3D10Device_VMTable = (UINT_PTR*)D3D10_ID3D10Device_VMTable[0];

	OutputDebug("pDevice %x", pDevice);
	OutputDebug("D3D10_ID3D10Device_VMTable %x", D3D10_ID3D10Device_VMTable);
	OutputDebug("D3D10_ID3D10Device_VMTable[0] %x", D3D10_ID3D10Device_VMTable[0]);

	// create the aquilinus detour classes
	pDCL_ID3D10Device = new DCL_ID3D10Device(g_pAQU_TransferSite);
	pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);

	// set old function pointers
	if (FAILED(pDCL_ID3D10Device->SetSuperFunctionPointers(D3D10_ID3D10Device_VMTable)))
		OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");
	if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
		OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

	// TODO !! HANDLE FAILURE PROPERLY

	pDevice->Release();
	pSwapChain->Release();

	// create the thread to override the virtual methods table
	if (CreateThread(NULL, 0, D3D10_VMT_Repatch, NULL, 0, NULL) == NULL)
		return E_FAIL;

	return S_OK;
}

/**
* Thread to set detour functions.
***/
DWORD WINAPI D3D10_VMT_Repatch(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);

	int iCounter = 0;

	// detour direct10 device methods
	// override or detour device table...
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D10_ID3D10Device_VMTable)
			Override_D3D10_ID3D10Device_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D10_ID3D10Device_VMTable();
		break;
	}

	// get the DXGI SwapChain
	D3D10_IDXGISwapChain_VMTable = NULL;
	while (D3D10_IDXGISwapChain_VMTable == NULL)
	{
		IDXGISwapChain *pSwapChain = nullptr;
		if (SUCCEEDED(pDCL_ID3D10Device->GetDXGISwapChain(&pSwapChain)))
		{
			// set swapchain vtable
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

			// release here and not in class !!
			pSwapChain->Release();

			// set super methods
			if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
				OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");
		}
	}

	// which technique ? first the device
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D10_IDXGISwapChain_VMTable)
			Override_D3D10_IDXGISwapChain_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		// TODO !! WRITE FUNCTION.
		//Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	return E_FAIL;
}

#pragma endregion

#pragma region DirectX 11.0

/**
* Thread to get the d3d11 device by mask.
* This method does following :
* - it creates a test device + context + swapchain
* - it gets the first VM-table address from both device and context
* - it releases the test device + context + swapchain
* - it locates the VM-table for both the device and context in "d3d11.dll"
* - it sets/creates all necessary hooking fields
* - it starts the repatching thread
***/
DWORD WINAPI D3D11_VMT_Mask(LPVOID Param)
{
	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
	{
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);
	}

	ID3D11Device *pDevice;
	ID3D11DeviceContext *pImmediateContext;

	// find the window we want to inject
	HWND hWnd = GetForegroundWindow();
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// create a sample device and swapchain
	IDXGISwapChain* pSwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT               numLevelsRequested = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,                    //_In_   IDXGIAdapter *pAdapter,
		D3D_DRIVER_TYPE_HARDWARE,//_In_   D3D_DRIVER_TYPE DriverType,
		NULL,                    //_In_   HMODULE Software,
		0,                       //_In_   UINT Flags,
		featureLevels,           //_In_   const D3D_FEATURE_LEVEL *pFeatureLevels,
		numLevelsRequested,      //_In_   UINT FeatureLevels,
		D3D11_SDK_VERSION,       //_In_   UINT SDKVersion,
		&swapChainDesc,          //_In_   const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		&pSwapChain,             //_Out_  IDXGISwapChain **ppSwapChain,
		&pDevice,                //_Out_  ID3D11Device **ppDevice,
		&FeatureLevelsSupported, //_Out_  D3D_FEATURE_LEVEL *pFeatureLevel,
		&pImmediateContext       //_Out_  ID3D11DeviceContext **ppImmediateContext
	)))
	{
		OutputDebug("Failed to create directX device and swapchain!");
		return E_FAIL;
	}

	// set device+context vtable
	D3D11_ID3D11Device_VMTable = (UINT_PTR*)pDevice;
	D3D11_ID3D11Device_VMTable = (UINT_PTR*)D3D11_ID3D11Device_VMTable[0];
	D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)pImmediateContext;
	D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)D3D11_ID3D11DeviceContext_VMTable[0];

	// get first vmtable value
	UINT_PTR dwVMTableValueDevice = D3D11_ID3D11Device_VMTable[0];
	UINT_PTR dwVMTableValueContext = D3D11_ID3D11DeviceContext_VMTable[0];

	// release all
	{
		if (pSwapChain)
		{
			(pSwapChain)->Release();
			(pSwapChain) = NULL;
		}
	}
	{
		if (pImmediateContext)
		{
			(pImmediateContext)->Release();
			(pImmediateContext) = NULL;
		}
	}
	{
		if (pDevice)
		{
			(pDevice)->Release();
			(pDevice) = NULL;
		}
	}

#ifdef _WIN64
	// get the vmtable entries for the D3D11.0 device
	UINT_PTR dwHandle = NULL;
	while (!dwHandle) dwHandle = (UINT_PTR)GetModuleHandle(L"d3d11.dll");
	BYTE pbMask[9];
	memcpy(pbMask, &dwVMTableValueDevice, 8 * sizeof(BYTE));
	pbMask[8] = 0;
	UINT_PTR dwDXVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x512000, pbMask, "xxxxxxxx");

	// get the vmtable entries for the D3D11.0 device context
	memcpy(pbMask, &dwVMTableValueContext, 8 * sizeof(BYTE));
	UINT_PTR dwDXContextVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x512000, pbMask, "xxxxxxxx");

	// the context is the 2nd vmtable with the desired first value
	dwHandle = dwDXContextVMTableRoot + 4;
	if (!dwDXContextVMTableRoot) return 0;
	dwDXContextVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x128000, pbMask, "xxxxxxxx");
#else
	// get the vmtable entries for the D3D11.0 device
	UINT_PTR dwHandle = NULL;
	while (!dwHandle) dwHandle = (UINT_PTR)GetModuleHandle(L"d3d11.dll");
	BYTE pbMask[5];
	memcpy(pbMask, &dwVMTableValueDevice, 4 * sizeof(BYTE));
	pbMask[4] = 0;
	UINT_PTR dwDXVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x128000, pbMask, "xxxx");

	// get the vmtable entries for the D3D11.0 device context
	memcpy(pbMask, &dwVMTableValueContext, 4 * sizeof(BYTE));
	UINT_PTR dwDXContextVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x128000, pbMask, "xxxx");

	// the context is the 2nd vmtable with the desired first value
	dwHandle = dwDXContextVMTableRoot + 4;
	dwDXContextVMTableRoot = (UINT_PTR)FindPattern(dwHandle, 0x128000, pbMask, "xxxx");
#endif
	if (dwDXVMTableRoot)
	{
		// D3D11.0 device
		D3D11_ID3D11Device_VMTable = (UINT_PTR*)dwDXVMTableRoot;
		D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)dwDXContextVMTableRoot;

		// get the ID3D11Device VMTable, set SwapChain VMTable to nullptr meanwhile...
		D3D10_IDXGISwapChain_VMTable = nullptr;

		// create the aquilinus detour classes
		pDCL_ID3D11Device = new DCL_ID3D11Device(g_pAQU_TransferSite);
		pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);
		pDCL_ID3D11DeviceContext = new DCL_ID3D11DeviceContext(g_pAQU_TransferSite);

		//#ifdef _DEBUG
		OutputDebug("dwDXVMTableRoot %x", dwDXVMTableRoot);
		OutputDebug("dwDXContextVMTableRoot %x", dwDXContextVMTableRoot);
		OutputDebug("D3D11_ID3D11Device_VMTable: %x", D3D11_ID3D11Device_VMTable);
		OutputDebug("D3D11_ID3D11Device_AddRef %x", D3D11_ID3D11Device_AddRef);
		//#endif

		// set super methods
		if (FAILED(pDCL_ID3D11Device->SetSuperFunctionPointers(D3D11_ID3D11Device_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");
		if (FAILED(pDCL_ID3D11DeviceContext->SetSuperFunctionPointers(D3D11_ID3D11DeviceContext_VMTable)))
			OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

		// create the thread to override the virtual methods table
		if (CreateThread(NULL, 0, D3D11_VMT_Repatch, NULL, 0, NULL) == NULL)
			return E_FAIL;
	}

	OutputDebugString(L"Aquilinus : VMTable hook installed.");

	return D3D_OK;
}

/**
* Thread to create the virtual method table for DirectX v10.0.
* METHOD OBSOLETE !
***/
DWORD WINAPI D3D11_VMT_Create(LPVOID Param)
{
	/*ID3D11Device *pDevice;
	ID3D11DeviceContext *pImmediateContext;

	// find the window we want to inject
	//HWND hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	//while (!hWnd) hWnd = FindWindow(NULL, AQU_WINDOW_NAME);
	HWND hWnd = GetForegroundWindow();

	// create a sample device and swapchain
	IDXGISwapChain* pSwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;//((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	// TODO !! WINDOWED/FULLSCREEN SETTING

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;
	if (FAILED(D3D11CreateDeviceAndSwapChain(
	NULL,                    //_In_   IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE_HARDWARE,//_In_   D3D_DRIVER_TYPE DriverType,
	NULL,                    //_In_   HMODULE Software,
	0,                       //_In_   UINT Flags,
	&FeatureLevelsRequested, //_In_   const D3D_FEATURE_LEVEL *pFeatureLevels,
	numLevelsRequested,      //_In_   UINT FeatureLevels,
	D3D11_SDK_VERSION,       //_In_   UINT SDKVersion,
	&swapChainDesc,          //_In_   const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
	&pSwapChain,             //_Out_  IDXGISwapChain **ppSwapChain,
	&pDevice,                //_Out_  ID3D11Device **ppDevice,
	&FeatureLevelsSupported, //_Out_  D3D_FEATURE_LEVEL *pFeatureLevel,
	&pImmediateContext       //_Out_  ID3D11DeviceContext **ppImmediateContext
	)))
	{
	OutputDebug("Failed to create directX device and swapchain!");
	return E_FAIL;
	}

	// set the VTable for the swapchain class
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

	// create the aquilinus detour classes
	pDCL_ID3D11Device = new DCL_ID3D11Device(g_pAQU_TransferSite);
	pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);
	pDCL_ID3D11DeviceContext = new DCL_ID3D11DeviceContext(g_pAQU_TransferSite);

	// set old function pointers
	if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
	OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");

	// TODO !! HANDLE FAILURE PROPERLY

	pDevice->Release();
	pSwapChain->Release();
	pImmediateContext->Release();

	// create the thread to override the virtual methods table
	if (CreateThread(NULL, 0, D3D11_VMT_Repatch, NULL, 0, NULL) == NULL)
	return E_FAIL;

	return S_OK;*/
	return E_NOTIMPL;
}

/**
* Thread to set detour functions.
* This method does following for the chosen DX11 VMTable hooks:
* - it overrides device + context in "d3d11.dll", this will automatically
*   override the device and all context drawing methods in the game process
* - it catches the swapchain and the context from the device class
* - it overrides the swapchain and the (full) context
***/
DWORD WINAPI D3D11_VMT_Repatch(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	int iCounter = 0;

	// time delay ? for delayed injection we set to create swapchain if necessary
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		pDCL_ID3D11Device->CreateSwapChainIfNecessary();

	// detour direct11 device + context methods - override or detour table...
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D11_ID3D11Device_VMTable)
			Override_D3D11_ID3D11Device_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		Detour_D3D11_ID3D11Device_VMTable();
		break;
	}
	// which technique ?
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D11_ID3D11DeviceContext_VMTable)
			Override_D3D11_ID3D11DeviceContext_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		// TODO !! WRITE FUNCTION // Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	// get the DXGI SwapChain + Context
	D3D10_IDXGISwapChain_VMTable = NULL;
	D3D11_ID3D11DeviceContext_VMTable = NULL;
	while ((D3D10_IDXGISwapChain_VMTable == NULL) || (D3D11_ID3D11DeviceContext_VMTable == NULL))
	{
		if (D3D11_ID3D11DeviceContext_VMTable == NULL)
		{
			ID3D11DeviceContext* pcContext = nullptr;
			if (SUCCEEDED(pDCL_ID3D11Device->GetDeviceContext(&pcContext)))
			{
				// set swapchain vtable
				D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)pcContext;
				D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)D3D11_ID3D11DeviceContext_VMTable[0];

				// NO release here...done in detour class
			}
		}
		if (D3D10_IDXGISwapChain_VMTable == NULL)
		{
			IDXGISwapChain *pSwapChain = nullptr;
			if (FAILED(pDCL_ID3D11Device->GetDXGISwapChain(&pSwapChain)))
				pDCL_ID3D11DeviceContext->GetDXGISwapChain(&pSwapChain);
			else
				pDCL_ID3D11DeviceContext->SetDXGISwapChain(pSwapChain);

			if (pSwapChain)
			{
				// set swapchain vtable
				D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
				D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

				// NO release here...done in detour class

				// set super methods
				if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
					OutputDebugString(L"Aquilinus : Failed to set old function pointers !!");
			}
		}
	}

	// which technique ?
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D11_ID3D11DeviceContext_VMTable)
			Override_D3D11_ID3D11DeviceContext_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		// TODO !! WRITE FUNCTION // Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);

	// which technique ?
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D10_IDXGISwapChain_VMTable)
			Override_D3D10_IDXGISwapChain_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		// TODO !! WRITE FUNCTION // Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	OutputDebugString(L"Aquilinus : VMTable hook successfull.");

	return D3D_OK;
}

#pragma endregion

#pragma endregion

#pragma region D3DX methods
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileA_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadSurfaceFromFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromFileA_Super(pDestSurface, pDestPalette, pDestRect, pSrcFile, pSrcRect, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileW_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCWSTR pSrcFile, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadSurfaceFromFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromFileW_Super(pDestSurface, pDestPalette, pDestRect, pSrcFile, pSrcRect, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromResourceA_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadSurfaceFromResourceA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromResourceA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromResourceA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromResourceA_Super(pDestSurface, pDestPalette, pDestRect, hSrcModule, pSrcResource, pSrcRect, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromResourceW_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadSurfaceFromResourceW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromResourceW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromResourceW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromResourceW_Super(pDestSurface, pDestPalette, pDestRect, hSrcModule, pSrcResource, pSrcRect, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromFileInMemory_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcData, UINT SrcDataSize, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadSurfaceFromFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromFileInMemory_Super(pDestSurface, pDestPalette, pDestRect, pSrcData, SrcDataSize, pSrcRect, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromSurface_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromSurface");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcSurface) pSrcSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcSurface]->m_pOutput = (void*)&pSrcSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromSurface;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromSurface_Super(pDestSurface, pDestPalette, pDestRect, pSrcSurface, pSrcPalette, pSrcRect, Filter, ColorKey);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadSurfaceFromMemory_Detour(LPDIRECT3DSURFACE9 pDestSurface, CONST PALETTEENTRY* pDestPalette, CONST RECT* pDestRect, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcPitch, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect, DWORD Filter, D3DCOLOR ColorKey)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadSurfaceFromMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestSurface) pDestSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pDestSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestRect]->m_pOutput = (void*)&pDestRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcMemory]->m_pOutput = (void*)&pSrcMemory;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcFormat]->m_pOutput = (void*)&SrcFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcPitch]->m_pOutput = (void*)&SrcPitch;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadSurfaceFromMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadSurfaceFromMemory_Super(pDestSurface, pDestPalette, pDestRect, pSrcMemory, SrcFormat, SrcPitch, pSrcPalette, pSrcRect, Filter, ColorKey);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect)
{
	OutputDebugString(L"D3D9::D3DXSaveSurfaceToFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveSurfaceToFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcSurface) pSrcSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcSurface]->m_pOutput = (void*)&pSrcSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveSurfaceToFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveSurfaceToFileA_Super(pDestFile, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect)
{
	OutputDebugString(L"D3D9::D3DXSaveSurfaceToFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveSurfaceToFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcSurface) pSrcSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcSurface]->m_pOutput = (void*)&pSrcSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveSurfaceToFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveSurfaceToFileW_Super(pDestFile, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveSurfaceToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DSURFACE9 pSrcSurface, CONST PALETTEENTRY* pSrcPalette, CONST RECT* pSrcRect)
{
	OutputDebugString(L"D3D9::D3DXSaveSurfaceToFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveSurfaceToFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcSurface) pSrcSurface->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppDestBuf]->m_pOutput = (void*)&ppDestBuf;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcSurface]->m_pOutput = (void*)&pSrcSurface;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcRect]->m_pOutput = (void*)&pSrcRect;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveSurfaceToFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveSurfaceToFileInMemory_Super(ppDestBuf, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileA_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromFileA_Super(pDestVolume, pDestPalette, pDestBox, pSrcFile, pSrcBox, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileW_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCWSTR pSrcFile, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromFileW_Super(pDestVolume, pDestPalette, pDestBox, pSrcFile, pSrcBox, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromResourceA_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromResourceA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromResourceA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromResourceA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromResourceA_Super(pDestVolume, pDestPalette, pDestBox, hSrcModule, pSrcResource, pSrcBox, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromResourceW_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, HMODULE hSrcModule, LPCWSTR pSrcResource, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromResourceW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromResourceW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromResourceW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromResourceW_Super(pDestVolume, pDestPalette, pDestBox, hSrcModule, pSrcResource, pSrcBox, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromFileInMemory_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcData, UINT SrcDataSize, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromFileInMemory_Super(pDestVolume, pDestPalette, pDestBox, pSrcData, SrcDataSize, pSrcBox, Filter, ColorKey, pSrcInfo);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromVolume_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromVolume");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromVolume");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcVolume]->m_pOutput = (void*)&pSrcVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromVolume;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromVolume_Super(pDestVolume, pDestPalette, pDestBox, pSrcVolume, pSrcPalette, pSrcBox, Filter, ColorKey);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXLoadVolumeFromMemory_Detour(LPDIRECT3DVOLUME9 pDestVolume, CONST PALETTEENTRY* pDestPalette, CONST D3DBOX* pDestBox, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcRowPitch, UINT SrcSlicePitch, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox, DWORD Filter, D3DCOLOR ColorKey)
{
	OutputDebugString(L"D3D9::D3DXLoadVolumeFromMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXLoadVolumeFromMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pDestVolume) pDestVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestVolume]->m_pOutput = (void*)&pDestVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&pDestPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestBox]->m_pOutput = (void*)&pDestBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcMemory]->m_pOutput = (void*)&pSrcMemory;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcFormat]->m_pOutput = (void*)&SrcFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcRowPitch]->m_pOutput = (void*)&SrcRowPitch;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcSlicePitch]->m_pOutput = (void*)&SrcSlicePitch;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXLoadVolumeFromMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXLoadVolumeFromMemory_Super(pDestVolume, pDestPalette, pDestBox, pSrcMemory, SrcFormat, SrcRowPitch, SrcSlicePitch, pSrcPalette, pSrcBox, Filter, ColorKey);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox)
{
	OutputDebugString(L"D3D9::D3DXSaveVolumeToFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveVolumeToFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcVolume) pSrcVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcVolume]->m_pOutput = (void*)&pSrcVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveVolumeToFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveVolumeToFileA_Super(pDestFile, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox)
{
	OutputDebugString(L"D3D9::D3DXSaveVolumeToFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveVolumeToFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcVolume) pSrcVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcVolume]->m_pOutput = (void*)&pSrcVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveVolumeToFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveVolumeToFileW_Super(pDestFile, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveVolumeToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DVOLUME9 pSrcVolume, CONST PALETTEENTRY* pSrcPalette, CONST D3DBOX* pSrcBox)
{
	OutputDebugString(L"D3D9::D3DXSaveVolumeToFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveVolumeToFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcVolume) pSrcVolume->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppDestBuf]->m_pOutput = (void*)&ppDestBuf;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcVolume]->m_pOutput = (void*)&pSrcVolume;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcBox]->m_pOutput = (void*)&pSrcBox;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveVolumeToFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveVolumeToFileInMemory_Super(ppDestBuf, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTexture_Super(pDevice, Width, Height, MipLevels, Usage, Format, Pool, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTexture");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTexture_Super(pDevice, Size, MipLevels, Usage, Format, Pool, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTexture_Detour(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTexture");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTexture_Super(pDevice, Width, Height, Depth, MipLevels, Usage, Format, Pool, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestSurface]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestPalette]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileA_Super(pDevice, pSrcFile, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileW_Super(pDevice, pSrcFile, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileA_Super(pDevice, pSrcFile, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileW_Super(pDevice, pSrcFile, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileA_Super(pDevice, pSrcFile, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileW_Super(pDevice, pSrcFile, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromResourceA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromResourceA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromResourceA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromResourceA_Super(pDevice, hSrcModule, pSrcResource, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromResourceW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromResourceW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromResourceW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromResourceW_Super(pDevice, hSrcModule, pSrcResource, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromResourceA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromResourceA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromResourceA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromResourceA_Super(pDevice, hSrcModule, pSrcResource, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromResourceW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromResourceW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromResourceW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromResourceW_Super(pDevice, hSrcModule, pSrcResource, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromResourceA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromResourceA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromResourceA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromResourceA_Super(pDevice, hSrcModule, pSrcResource, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromResourceW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromResourceW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromResourceW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromResourceW_Super(pDevice, hSrcModule, pSrcResource, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromFileExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileExA_Super(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromFileExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileExW_Super(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromFileExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileExA_Super(pDevice, pSrcFile, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromFileExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileExW_Super(pDevice, pSrcFile, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileExA_Detour(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFile]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileExA_Super(pDevice, pSrcFile, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileExW_Detour(LPDIRECT3DDEVICE9 pDevice, LPCWSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcFileW]->m_pOutput = (void*)&pSrcFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileExW_Super(pDevice, pSrcFile, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromResourceExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromResourceExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromResourceExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromResourceExA_Super(pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateTextureFromResourceExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromResourceExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromResourceExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromResourceExW_Super(pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromResourceExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromResourceExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromResourceExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromResourceExA_Super(pDevice, hSrcModule, pSrcResource, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromResourceExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromResourceExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromResourceExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromResourceExW_Super(pDevice, hSrcModule, pSrcResource, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceExA_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromResourceExA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromResourceExA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResource]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromResourceExA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromResourceExA_Super(pDevice, hSrcModule, pSrcResource, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromResourceExW_Detour(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromResourceExW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromResourceExW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::hSrcModule]->m_pOutput = (void*)&hSrcModule;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcResourceW]->m_pOutput = (void*)&pSrcResource;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromResourceExW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromResourceExW_Super(pDevice, hSrcModule, pSrcResource, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileInMemory_Super(pDevice, pSrcData, SrcDataSize, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileInMemory_Super(pDevice, pSrcData, SrcDataSize, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileInMemory_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileInMemory_Super(pDevice, pSrcData, SrcDataSize, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateTextureFromFileInMemoryEx");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppTexture]->m_pOutput = (void*)&ppTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateTextureFromFileInMemoryEx;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateTextureFromFileInMemoryEx_Super(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateCubeTextureFromFileInMemoryEx");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateCubeTextureFromFileInMemoryEx");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Size]->m_pOutput = (void*)&Size;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppCubeTexture]->m_pOutput = (void*)&ppCubeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateCubeTextureFromFileInMemoryEx;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateCubeTextureFromFileInMemoryEx_Super(pDevice, pSrcData, SrcDataSize, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Detour(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
{
	OutputDebugString(L"D3D9::D3DXCreateVolumeTextureFromFileInMemoryEx");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXCreateVolumeTextureFromFileInMemoryEx");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcData]->m_pOutput = (void*)&pSrcData;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcDataSize]->m_pOutput = (void*)&SrcDataSize;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Width]->m_pOutput = (void*)&Width;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Height]->m_pOutput = (void*)&Height;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Depth]->m_pOutput = (void*)&Depth;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipLevels]->m_pOutput = (void*)&MipLevels;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Usage]->m_pOutput = (void*)&Usage;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Format]->m_pOutput = (void*)&Format;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Pool]->m_pOutput = (void*)&Pool;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::MipFilter]->m_pOutput = (void*)&MipFilter;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ColorKey]->m_pOutput = (void*)&ColorKey;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcInfo]->m_pOutput = (void*)&pSrcInfo;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppVolumeTexture]->m_pOutput = (void*)&ppVolumeTexture;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pDevice, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXCreateVolumeTextureFromFileInMemoryEx_Super(pDevice, pSrcData, SrcDataSize, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveTextureToFileA_Detour(LPCSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette)
{
	OutputDebugString(L"D3D9::D3DXSaveTextureToFileA");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveTextureToFileA");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcTexture) pSrcTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcTexture]->m_pOutput = (void*)&pSrcTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveTextureToFileA;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveTextureToFileA_Super(pDestFile, DestFormat, pSrcTexture, pSrcPalette);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveTextureToFileW_Detour(LPCWSTR pDestFile, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette)
{
	OutputDebugString(L"D3D9::D3DXSaveTextureToFileW");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveTextureToFileW");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcTexture) pSrcTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pDestFile]->m_pOutput = (void*)&pDestFile;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcTexture]->m_pOutput = (void*)&pSrcTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveTextureToFileW;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveTextureToFileW_Super(pDestFile, DestFormat, pSrcTexture, pSrcPalette);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXSaveTextureToFileInMemory_Detour(LPD3DXBUFFER* ppDestBuf, D3DXIMAGE_FILEFORMAT DestFormat, LPDIRECT3DBASETEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette)
{
	OutputDebugString(L"D3D9::D3DXSaveTextureToFileInMemory");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXSaveTextureToFileInMemory");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pSrcTexture) pSrcTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::ppDestBuf]->m_pOutput = (void*)&ppDestBuf;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::DestFormat]->m_pOutput = (void*)&DestFormat;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcTexture]->m_pOutput = (void*)&pSrcTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXSaveTextureToFileInMemory;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXSaveTextureToFileInMemory_Super(ppDestBuf, DestFormat, pSrcTexture, pSrcPalette);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFilterTexture_Detour(LPDIRECT3DBASETEXTURE9 pBaseTexture, CONST PALETTEENTRY* pPalette, UINT SrcLevel, DWORD Filter)
{
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFilterTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pBaseTexture) pBaseTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pBaseTexture]->m_pOutput = (void*)&pBaseTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pPalette]->m_pOutput = (void*)&pPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::SrcLevel]->m_pOutput = (void*)&SrcLevel;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Filter]->m_pOutput = (void*)&Filter;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFilterTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFilterTexture_Super(pBaseTexture, pPalette, SrcLevel, Filter);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillTexture_Detour(LPDIRECT3DTEXTURE9 pTexture, LPD3DXFILL2D pFunction, LPVOID pData)
{
	OutputDebugString(L"D3D9::D3DXFillTexture");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pTexture) pTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTexture]->m_pOutput = (void*)&pTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pFunction]->m_pOutput = (void*)&pFunction;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pData]->m_pOutput = (void*)&pData;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillTexture_Super(pTexture, pFunction, pData);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillCubeTexture_Detour(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXFILL3D pFunction, LPVOID pData)
{
	OutputDebugString(L"D3D9::D3DXFillCubeTexture");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillCubeTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pCubeTexture) pCubeTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pCubeTexture]->m_pOutput = (void*)&pCubeTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pFunction]->m_pOutput = (void*)&pFunction;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pData]->m_pOutput = (void*)&pData;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillCubeTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillCubeTexture_Super(pCubeTexture, pFunction, pData);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillVolumeTexture_Detour(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXFILL3D pFunction, LPVOID pData)
{
	OutputDebugString(L"D3D9::D3DXFillVolumeTexture");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillVolumeTexture");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pVolumeTexture) pVolumeTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pVolumeTexture]->m_pOutput = (void*)&pVolumeTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pFunction]->m_pOutput = (void*)&pFunction;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pData]->m_pOutput = (void*)&pData;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillVolumeTexture;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillVolumeTexture_Super(pVolumeTexture, pFunction, pData);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillTextureTX_Detour(LPDIRECT3DTEXTURE9 pTexture, LPD3DXTEXTURESHADER pTextureShader)
{
	OutputDebugString(L"D3D9::D3DXFillTextureTX");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillTextureTX");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pTexture) pTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTexture]->m_pOutput = (void*)&pTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTextureShader]->m_pOutput = (void*)&pTextureShader;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillTextureTX;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillTextureTX_Super(pTexture, pTextureShader);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillCubeTextureTX_Detour(LPDIRECT3DCUBETEXTURE9 pCubeTexture, LPD3DXTEXTURESHADER pTextureShader)
{
	OutputDebugString(L"D3D9::D3DXFillCubeTextureTX");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillCubeTextureTX");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pCubeTexture) pCubeTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pCubeTexture]->m_pOutput = (void*)&pCubeTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTextureShader]->m_pOutput = (void*)&pTextureShader;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillCubeTextureTX;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillCubeTextureTX_Super(pCubeTexture, pTextureShader);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXFillVolumeTextureTX_Detour(LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture, LPD3DXTEXTURESHADER pTextureShader)
{
	OutputDebugString(L"D3D9::D3DXFillVolumeTextureTX");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXFillVolumeTextureTX");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pVolumeTexture) pVolumeTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pVolumeTexture]->m_pOutput = (void*)&pVolumeTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTextureShader]->m_pOutput = (void*)&pTextureShader;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXFillVolumeTextureTX;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXFillVolumeTextureTX_Super(pVolumeTexture, pTextureShader);
	return nHr;
}
/**
*
***/
HRESULT WINAPI D3D9_D3DXComputeNormalMap_Detour(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DTEXTURE9 pSrcTexture, CONST PALETTEENTRY* pSrcPalette, DWORD Flags, DWORD Channel, FLOAT Amplitude)
{
	OutputDebugString(L"D3D9::D3DXComputeNormalMap");
	static HRESULT nHr = S_OK;

	// output method call if no D3DX9 node is present
	if (!g_pAQU_TransferSite->m_pNOD_D3DX9)
	{
		OutputDebugString(L"D3DX9::D3DXComputeNormalMap");
	}
	// not force D3D, node present and invokers connected ? set node output data and return node provoke
	else if ((!g_pAQU_TransferSite->m_bForceD3D) && (g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers.size() > 0))
	{
		// get device
		IDirect3DDevice9* pcThis = nullptr;
		if (pTexture) pTexture->GetDevice(&pcThis);

		// set data
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pTexture]->m_pOutput = (void*)&pTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcTexture]->m_pOutput = (void*)&pSrcTexture;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::pSrcPalette]->m_pOutput = (void*)&pSrcPalette;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Flags]->m_pOutput = (void*)&Flags;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Channel]->m_pOutput = (void*)&Channel;
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_paCommanders[NOD_D3DX9_Commanders::Amplitude]->m_pOutput = (void*)&Amplitude;

		// overwrite the method id here
		g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_eD3DMethod = MT_D3DX9::D3D9_D3DXComputeNormalMap;

		// provoke, set bForceD3D to "true" for any provoking circle
		g_pAQU_TransferSite->m_bForceD3D = true;
		void* pvRet = g_pAQU_TransferSite->m_pNOD_D3DX9->Provoke((void*)pcThis, g_pAQU_TransferSite->m_ppaNodes);
		g_pAQU_TransferSite->m_bForceD3D = false;

		if (pcThis) pcThis->Release();

		// replace method call only if the nodes first connected node wants to replace the call
		if ((*g_pAQU_TransferSite->m_ppaNodes)[g_pAQU_TransferSite->m_pNOD_D3DX9->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
		{
			// get return value.. MUST be STATIC !
			nHr = (HRESULT)*(HRESULT*)pvRet;
			return nHr;
		}
	}

	nHr = D3D9_D3DXComputeNormalMap_Super(pTexture, pSrcTexture, pSrcPalette, Flags, Channel, Amplitude);
	return nHr;
}
#pragma endregion

