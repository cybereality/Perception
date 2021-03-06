/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio
Copyright � 2014 Denis Reischl

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
#ifndef _TRACE
#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#endif
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
#include"DMT_ID3D11Device2.h"
#include"DMT_IDXGISwapChain.h"
#include"DMT_IDXGISwapChain1.h"
#include"DMT_IDXGISwapChain2.h"
#include"DMT_ID3D11DeviceContext.h"
#include"DMT_ID3D11DeviceContext1.h"
#include"DMT_ID3D11DeviceContext2.h"

#pragma region Aquilinus global helpers
/// <summary>
/// Debug console helper
/// </summary>
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

/// <summary>
/// Memory mask search helper
/// </summary>
inline bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if ((*szMask == 'x') && (*pData != *bMask))
			return false;

	// put bool always in braces !!
	return ((*szMask) == NULL);
}

/// <summary>
/// Memory pattern search helper.
/// <param name="dValor"></param>
/// <param name="dLer"></param>
/// <param name="bMaskara"></param>
/// <param name="szMaskara"></param>
/// </summary>
inline UINT_PTR FindPattern(UINT_PTR dValor, UINT_PTR dLer, BYTE* bMaskara, char* szMaskara)
{
	for (UINT_PTR i = 0; i < dLer; i++)
		if (bCompare((PBYTE)(dValor + i), bMaskara, szMaskara))
			return (UINT_PTR)(dValor + i);
	return false;
}

/// <summary>
/// Memory pattern search helper.
/// <param name="dValor"></param>
/// <param name="dLer"></param>
/// <param name="dValue"></param>
/// <param name="dSpace"></param>
/// </summary>
inline UINT_PTR FindTwinDword(UINT_PTR dValor, DWORD dLer, DWORD dValue, DWORD dSpace)
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

/// <summary>
/// => Dll main 
/// <param name="hinstModule">dll input module</param>
/// <param name="dwReason">dll input message</param>
/// <param name="lpvReserved">unused/reserved</param>
/// </summary>
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
		MH_Uninitialize();
	}
	return FALSE;
}

/// <summary>
/// => Init Aquilinus
/// </summary>
HRESULT WINAPI AquilinusInit(VOID)
{
	// get the config map handle
	g_hConfigMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, g_szMemoryPageName);

	// return if failed
	if (g_hConfigMapFile == NULL)
	{
		OutputDebugString(TEXT("[AQU] Could not create file mapping object.\n"));
		return E_FAIL;
	}

	// create map view
	g_pAquilinusConfig = (AquilinusCfg*)
		MapViewOfFile(g_hConfigMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(AquilinusCfg));

	// return if failed
	if (g_pAquilinusConfig == NULL)
	{
		OutputDebugString(TEXT("[AQU] Could not map view of file.\n"));
		CloseHandle(g_hConfigMapFile);
		return E_FAIL;
	}

	// create the global transfer site class
	g_pAQU_TransferSite = new AQU_TransferSite(g_pAquilinusConfig);
	g_pAQU_TransferSite->InitD3DNodes();

	// set vmtable threads
	g_pAQU_TransferSite->m_pD3D9ReinstateInterfaces = D3D9_VMT_Repatch;
	g_pAQU_TransferSite->m_pD3D10ReinstateInterfaces = D3D10_VMT_Repatch;
	g_pAQU_TransferSite->m_pD3D11ReinstateInterfaces = D3D11_VMT_Repatch;

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
		} eD3D9_Hook_Type = D3D9_Hook_Device_Create;

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
			// NO thread here !! we call the method directly to save time..
			if (FAILED(D3D9_VMT_Create(nullptr))) return E_FAIL;
		}
	}

	// is there a chosen DX10 interface ?
	bool bDX10 = false;
	for (int i = DX10_FIRST_INTERFACE; i < DX10_LAST_INTERFACE; i++)
		bDX10 |= (g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::VMTable) ||
		(g_pAquilinusConfig->eInjectionTechnique[i] == AQU_InjectionTechniques::Detour);

	/// => Start DX9 thread
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

	/// => Start DX11 thread
	if (bDX11)
	{
		// get device by memory pattern or by ->CreateDevice() detour ?
		if (false)
		{
			// VMT mask not working anymore ??
			if ((pRootThread = CreateThread(NULL, 0, D3D11_VMT_Mask, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
		else
		{
			// use MinHook 
			if ((pRootThread = CreateThread(NULL, 0, D3D11_Detour, NULL, 0, NULL)) == NULL)
				return E_FAIL;
		}
	}

	OutputDebugString(L"[AQU] initialized.... :");

	return S_OK;
}

/// <summary>
/// => Init Project
/// Create working area. Create new or load project.
/// </summary>
HRESULT WINAPI AquilinusInitProject(HINSTANCE hInstance)
{
	OutputDebugString(L"[AQU] Init Project ");

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

			// ignore the reserved option space
			sstrDataStream.ignore(sizeof(unsigned __int32) * OPTIONS_RESERVED);

			// ignore picture boolean and the path if true
			BOOL bPicture;
			sstrDataStream.read((char*)&bPicture, sizeof(BOOL));
			if (bPicture)
			{
				LONG nImageSize;
				sstrDataStream.read((char*)&nImageSize, sizeof(LONG));
				sstrDataStream.ignore(nImageSize);
			}

			// ignore the detour time delay and injection repetition
			sstrDataStream.ignore(sizeof(unsigned __int32));
			sstrDataStream.ignore(sizeof(unsigned __int32));

			// now, ignore the injection techniques
			unsigned __int32 dwSupportedInterfacesNumber;
			sstrDataStream.read((char*)&dwSupportedInterfacesNumber, sizeof(unsigned __int32));
			sstrDataStream.ignore((std::streamsize)dwSupportedInterfacesNumber * (std::streamsize)sizeof(__int32));

			// read the number of nodes, resize node vector
			unsigned __int32 dwNodeNumber;
			sstrDataStream.read((char*)&dwNodeNumber, sizeof(unsigned __int32));
			g_paNodes.resize(dwNodeNumber);

			// get a node provider
			AQU_NodeProvider* pProvider = new AQU_NodeProvider();

			// loop through the nodes to add node data
			for (UINT i = 0; i != dwNodeNumber; i++)
			{
				// get the node hash
				unsigned __int32 id;
				sstrDataStream.read((char*)&id, sizeof(unsigned __int32));

				// get the node position
				ImVec2 sPos = {};
				sstrDataStream.read((char*)&sPos.x, sizeof(float));
				sstrDataStream.read((char*)&sPos.y, sizeof(float));

				// load plugin info (if plugin node)
				unsigned __int32 idPlugin = 0;
				wchar_t szFileName[64];
				wchar_t szFilePath[MAX_PATH];
				if (id == ELEMENTARY_NODE_PLUGIN)
				{
					// and write to stream
					sstrDataStream.read((char*)&idPlugin, sizeof(unsigned __int32));
					sstrDataStream.read((char*)&szFileName[0], sizeof(wchar_t) * 64);

					// create full path
					wsprintf(szFilePath, L"%s%s", g_pAQU_TransferSite->m_pFileManager->GetPluginPath(), szFileName);

					OutputDebugString(szFilePath);
				}

				// read node data
				unsigned __int32 dwDataSize = 0;
				sstrDataStream.read((char*)&dwDataSize, sizeof(unsigned __int32));

				// read the data
				char* pcData = new char[dwDataSize];
				if (dwDataSize)
					sstrDataStream.read((char*)pcData, dwDataSize);

				// get a node pointer
				NOD_Basic* pNode;
				if (FAILED(pProvider->Get_Node(pNode, id, (LONG)sPos.x, (LONG)sPos.y, idPlugin, szFilePath)))
				{
					// unregister all nodes
					g_pAQU_TransferSite->UnregisterAllNodes();

					// debug output
					wchar_t buf[64];
					wsprintf(buf, L"[AQU] Unknown node type %u", id);
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
				delete[] pcData;
			}

			delete pProvider;

			// loop through the nodes to add node connections
			for (std::vector<NOD_Basic*>::size_type i = 0; i != g_paNodes.size(); i++)
			{
				// get the number of commanders
				unsigned __int32 dwCommandersNumber;
				sstrDataStream.read((char*)&dwCommandersNumber, sizeof(unsigned __int32));

				// loop through commanders, get the number of connections and the connection indices
				for (DWORD j = 0; j < dwCommandersNumber; j++)
				{
					// get the commander connections number
					unsigned __int32 dwConnectionsNumber;
					sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(unsigned __int32));

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
				unsigned __int32 dwConnectionsNumber;
				sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(unsigned __int32));

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

#pragma region /// => DirectX 9.0
/// <summary>
/// => D3D9 Create D3D Detour
/// </summary>
IDirect3D9* WINAPI D3D9_Direct3DCreate9_Detour(UINT SDKVersion)
{
	OutputDebugString(L"[AQU] D3D9_Direct3DCreate9_Detour");
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

/// <summary>
/// => D3D9 Query Interface Detour
/// </summary>
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

			// need ->AddRef() here ?
			pcD3D->AddRef();
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}
	}

	return D3D9_QueryInterface_Super(pcD3D, riid, ppvObj);
}

/// <summary>
/// D3D9 Method Detour
/// </summary>
HRESULT WINAPI D3D9_CheckDeviceType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	static HRESULT nHr = S_OK;
	nHr = D3D9_CheckDeviceType_Super(pcD3D, Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);

	return nHr;
}

/// <summary>
/// D3D9 Method Detour
/// </summary>
HRESULT WINAPI D3D9_CheckDeviceFormat_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	static HRESULT nHr = S_OK;
	nHr = D3D9_CheckDeviceFormat_Super(pcD3D, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	return nHr;
}

/// <summary>
/// D3D9 Method Detour
/// </summary>
HRESULT WINAPI D3D9_CheckDeviceMultiSampleType_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
	return D3D9_CheckDeviceMultiSampleType_Super(pcD3D, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

/// <summary>
/// D3D9 Method Detour
/// </summary>
HRESULT WINAPI D3D9_CheckDepthStencilMatch_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return D3D9_CheckDepthStencilMatch_Super(pcD3D, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

/// <summary>
/// D3D9 Method Detour
/// </summary>
HRESULT WINAPI D3D9_CheckDeviceFormatConversion_Detour(LPDIRECT3D9 pcD3D, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return D3D9_CheckDeviceFormatConversion_Super(pcD3D, Adapter, DeviceType, SourceFormat, TargetFormat);
}

/// <summary>
/// => D3D9 Create Device Detour
/// </summary>
HRESULT WINAPI D3D9_CreateDevice_Detour(
	LPDIRECT3D9 pcD3D,
	UINT Adapter,
	D3DDEVTYPE DeviceType,
	HWND hFocusWindow,
	DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS* pPresentationParameters,
	IDirect3DDevice9** ppReturnedDeviceInterface
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
	IDirect3D9Ex* pDirect3D9Ex = NULL;
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
			IDirect3DDevice9Ex* pDirect3DDevice9Ex = NULL;
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

	// set back bForceD3D...
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
			OutputDebugString(L"[AQU] Device null pointer!");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
			return hr;
		}
		D3D9_IDirect3DDevice9_VMTable = (PUINT_PTR) * (PUINT_PTR)*ppReturnedDeviceInterface;

		// create all
		if (pDirect3D9Ex)
			D3D9_CreateAll(pDirect3D9Ex, pDevice, pPresentationParameters);
		else
			D3D9_CreateAll(pcD3D, pDevice, pPresentationParameters);
	}
	return hr;
}

/// <summary>
/// => D3D9 Hook
/// </summary>
HRESULT D3D9_CreateAll(LPDIRECT3D9 pParent, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	// create the aquilinus detour class
	pDCL_IDirect3DDevice9 = new DCL_IDirect3DDevice9(g_pAQU_TransferSite, pParent);

	// set old function pointers
	if (FAILED(pDCL_IDirect3DDevice9->SetSuperFunctionPointers(D3D9_IDirect3DDevice9_VMTable)))
		OutputDebugString(L"[AQU] Failed to set old function pointers !!");

	// create any d3d 9 interface to get its vmtable
	// first...IDirect3DTexture9 + IDirect3DBaseTexture9
	LPDIRECT3DTEXTURE9 pIDirect3DTexture9;
	LPDIRECT3DBASETEXTURE9 pIDirect3DBaseTexture9;
	LPDIRECT3DRESOURCE9 pIDirect3DResource9;
	if (SUCCEEDED(pDevice->CreateTexture(16, 16, 0, D3DUSAGE_RENDERTARGET, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DTexture9, NULL)))
	{
		D3D9_IDirect3DTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DTexture9 = new DCL_IDirect3DTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DTexture9->SetSuperFunctionPointers(D3D9_IDirect3DTexture9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		// query IDirect3DBaseTexture9
		if (FAILED(pIDirect3DTexture9->QueryInterface(IID_IDirect3DBaseTexture9, (void**)&pIDirect3DBaseTexture9)))
		{
			OutputDebugString(L"[AQU] Failed to query base texture. (d3d9)");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}
		else
		{
			D3D9_IDirect3DBaseTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DBaseTexture9;
			// create the aquilinus detour class
			pDCL_IDirect3DBaseTexture9 = new DCL_IDirect3DBaseTexture9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DBaseTexture9->SetSuperFunctionPointers(D3D9_IDirect3DBaseTexture9_VMTable)))
				OutputDebugString(L"[AQU] Failed to set old function pointers !!");

			// query IDirect3DResource9
			if (FAILED(pIDirect3DBaseTexture9->QueryInterface(IID_IDirect3DResource9, (void**)&pIDirect3DResource9)))
			{
				OutputDebugString(L"[AQU] Failed to query resource. (d3d9)");
				CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
			}
			else
			{
				D3D9_IDirect3DResource9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DResource9;
				// create the aquilinus detour class
				pDCL_IDirect3DResource9 = new DCL_IDirect3DResource9(g_pAQU_TransferSite);

				// set old function pointers
				if (FAILED(pDCL_IDirect3DResource9->SetSuperFunctionPointers(D3D9_IDirect3DResource9_VMTable)))
					OutputDebugString(L"[AQU] Failed to set old function pointers !!");

				pIDirect3DResource9->Release();
			}

			pIDirect3DBaseTexture9->Release();
		}

		pIDirect3DTexture9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DCubeTexture9
	LPDIRECT3DCUBETEXTURE9 pIDirect3DCubeTexture9;
	if (SUCCEEDED(pDevice->CreateCubeTexture(16, 0, D3DUSAGE_RENDERTARGET, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DCubeTexture9, NULL)))
	{
		D3D9_IDirect3DCubeTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DCubeTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DCubeTexture9 = new DCL_IDirect3DCubeTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DCubeTexture9->SetSuperFunctionPointers(D3D9_IDirect3DCubeTexture9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DCubeTexture9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Cube Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DVolumeTexture9
	LPDIRECT3DVOLUMETEXTURE9 pIDirect3DVolumeTexture9;
	if (SUCCEEDED(pDevice->CreateVolumeTexture(16, 16, 16, 0, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DVolumeTexture9, NULL)))
	{
		D3D9_IDirect3DVolumeTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DVolumeTexture9;
		// create the aquilinus detour class
		pDCL_IDirect3DVolumeTexture9 = new DCL_IDirect3DVolumeTexture9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVolumeTexture9->SetSuperFunctionPointers(D3D9_IDirect3DVolumeTexture9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		// IDirect3DVolume9
		LPDIRECT3DVOLUME9 pIDirect3DVolume9;
		if (SUCCEEDED(pIDirect3DVolumeTexture9->GetVolumeLevel(0, &pIDirect3DVolume9)))
		{
			D3D9_IDirect3DVolume9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DVolume9;
			// create the aquilinus detour class
			pDCL_IDirect3DVolume9 = new DCL_IDirect3DVolume9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DVolume9->SetSuperFunctionPointers(D3D9_IDirect3DVolume9_VMTable)))
				OutputDebugString(L"[AQU] Failed to set old function pointers !!");

			pIDirect3DVolume9->Release();
		}
		else
		{
			OutputDebugString(L"[AQU] Failed to get Volume (d3d9).");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}

		pIDirect3DVolumeTexture9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Volume Texture (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DSurface9
	LPDIRECT3DSURFACE9 pIDirect3DSurface9;
	if (SUCCEEDED(pDevice->CreateOffscreenPlainSurface(16, 16, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pIDirect3DSurface9, NULL)))
	{
		D3D9_IDirect3DSurface9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DSurface9;
		// create the aquilinus detour class
		pDCL_IDirect3DSurface9 = new DCL_IDirect3DSurface9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DSurface9->SetSuperFunctionPointers(D3D9_IDirect3DSurface9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DSurface9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to Surface (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DSwapChain9
	LPDIRECT3DSWAPCHAIN9 pIDirect3DSwapChain9;
	if (SUCCEEDED(pDevice->CreateAdditionalSwapChain(pPresentationParameters, &pIDirect3DSwapChain9)))
	{
		OutputDebugString(L"[AQU] Additional swap chain created !");
		D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DSwapChain9;
		// create the aquilinus detour class
		pDCL_IDirect3DSwapChain9 = new DCL_IDirect3DSwapChain9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DSwapChain9->SetSuperFunctionPointers(D3D9_IDirect3DSwapChain9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DSwapChain9->Release();
	}
	else
	{
		pDevice->GetSwapChain(0, &pIDirect3DSwapChain9);
		if (pIDirect3DSwapChain9)
		{
			OutputDebugString(L"[AQU] Got swap chain (d3d9) !");
			D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DSwapChain9;
			// create the aquilinus detour class
			pDCL_IDirect3DSwapChain9 = new DCL_IDirect3DSwapChain9(g_pAQU_TransferSite);

			// set old function pointers
			if (FAILED(pDCL_IDirect3DSwapChain9->SetSuperFunctionPointers(D3D9_IDirect3DSwapChain9_VMTable)))
				OutputDebugString(L"[AQU] Failed to set old function pointers !!");

			pIDirect3DSwapChain9->Release();
		}
		else
		{
			OutputDebugString(L"[AQU] Failed to create Swap Chain (d3d9).");
			CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		}
	}

	// IDirect3DIndexBuffer9
	LPDIRECT3DINDEXBUFFER9 pIDirect3DIndexBuffer9;
	if (SUCCEEDED(pDevice->CreateIndexBuffer(16, 0, D3DFORMAT::D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIDirect3DIndexBuffer9, NULL)))
	{
		D3D9_IDirect3DIndexBuffer9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DIndexBuffer9;
		// create the aquilinus detour class
		pDCL_IDirect3DIndexBuffer9 = new DCL_IDirect3DIndexBuffer9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DIndexBuffer9->SetSuperFunctionPointers(D3D9_IDirect3DIndexBuffer9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DIndexBuffer9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Index Buffer (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DVertexBuffer9
	LPDIRECT3DVERTEXBUFFER9 pIDirect3DVertexBuffer9;
	if (SUCCEEDED(pDevice->CreateVertexBuffer(3 * sizeof(float), 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &pIDirect3DVertexBuffer9, NULL)))
	{
		D3D9_IDirect3DVertexBuffer9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DVertexBuffer9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexBuffer9 = new DCL_IDirect3DVertexBuffer9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexBuffer9->SetSuperFunctionPointers(D3D9_IDirect3DVertexBuffer9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DVertexBuffer9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Vertex Buffer (d3d9).");
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
	else OutputDebugString(L"[AQU] Failed to compile Pixel Shader (d3d9).");
	if (SUCCEEDED(hr))
	{
		D3D9_IDirect3DPixelShader9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DPixelShader9;
		// create the aquilinus detour class
		pDCL_IDirect3DPixelShader9 = new DCL_IDirect3DPixelShader9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DPixelShader9->SetSuperFunctionPointers(D3D9_IDirect3DPixelShader9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		ct->Release();
		pShader->Release();
		pIDirect3DPixelShader9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Pixel Shader (d3d9).");
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
	else OutputDebugString(L"[AQU] Failed to compile Vertex Shader (d3d9).");
	if (SUCCEEDED(hr))
	{
		D3D9_IDirect3DVertexShader9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DVertexShader9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexShader9 = new DCL_IDirect3DVertexShader9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexShader9->SetSuperFunctionPointers(D3D9_IDirect3DVertexShader9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		ct->Release();
		pShader->Release();
		pIDirect3DVertexShader9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Vertex Shader (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
		pShader->Release();
		ct->Release();
	}

	// IDirect3DQuery9
	LPDIRECT3DQUERY9 pIDirect3DQuery9;
	if (SUCCEEDED(pDevice->CreateQuery(D3DQUERYTYPE_EVENT, &pIDirect3DQuery9)))
	{
		D3D9_IDirect3DQuery9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DQuery9;
		// create the aquilinus detour class
		pDCL_IDirect3DQuery9 = new DCL_IDirect3DQuery9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DQuery9->SetSuperFunctionPointers(D3D9_IDirect3DQuery9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DQuery9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create Query (d3d9).");
		CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);
	}

	// IDirect3DStateBlock9
	LPDIRECT3DSTATEBLOCK9 pIDirect3DStateBlock9;
	if (SUCCEEDED(pDevice->CreateStateBlock(D3DSTATEBLOCKTYPE::D3DSBT_ALL, &pIDirect3DStateBlock9)))
	{
		D3D9_IDirect3DStateBlock9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DStateBlock9;
		// create the aquilinus detour class
		pDCL_IDirect3DStateBlock9 = new DCL_IDirect3DStateBlock9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DStateBlock9->SetSuperFunctionPointers(D3D9_IDirect3DStateBlock9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DStateBlock9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create StateBlock (d3d9).");
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
		D3D9_IDirect3DVertexDeclaration9_VMTable = (PUINT_PTR) * (PUINT_PTR)pIDirect3DVertexDeclaration9;
		// create the aquilinus detour class
		pDCL_IDirect3DVertexDeclaration9 = new DCL_IDirect3DVertexDeclaration9(g_pAQU_TransferSite);

		// set old function pointers
		if (FAILED(pDCL_IDirect3DVertexDeclaration9->SetSuperFunctionPointers(D3D9_IDirect3DVertexDeclaration9_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		pIDirect3DVertexDeclaration9->Release();
	}
	else
	{
		OutputDebugString(L"[AQU] Failed to create VertexDeclaration (d3d9).");
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

/// <summary>
/// => D3D9 detour
/// </summary>
DWORD WINAPI D3D9_Detour(LPVOID Param)
{
	D3D9_Direct3DCreate9_Super = (D3D9_Direct3DCreate9)DetourFunc((BYTE*)Direct3DCreate9, (BYTE*)D3D9_Direct3DCreate9_Detour, JMP32_SZ);
	OutputDebugString(L"Aquilinus: D3D9_Detour is set.");
	CreateThread(NULL, 0, D3D9_VMT_Create, NULL, 0, NULL);

	return 0;
}

/// <summary>
/// => D3D9 VMT mask
/// Thread to get the d3d9 device by mask.
/// </summary>
DWORD WINAPI D3D9_VMT_Mask(LPVOID Param)
{
	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);

#ifdef _WIN64
	OutputDebugStringA("[AQU] D3D9 x64 NOT supported !!");
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

				// continue this ??
			}
		}
	}

	if (D3D9_IDirect3DDevice9_VMTable)
	{
		// get a device pointer, create all
		/*PUINT_PTR pcDevice = nullptr;
		*pcDevice = (UINT_PTR)D3D9_IDirect3DDevice9_VMTable;
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		D3D9_CreateAll(NULL, (LPDIRECT3DDEVICE9)pcDevice, &d3dpp);*/
	}
#endif
	return D3D_OK;
}

/// <summary>
/// => D3D9 VMT create
/// Thread to create the virtual method table for DirectX v9.0.
/// </summary>
DWORD WINAPI D3D9_VMT_Create(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	LPDIRECT3D9 Direct3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	if (Direct3D_Object == NULL)
		return D3DERR_INVALIDCALL;

	D3D9_IDirect3D9_VMTable = (PUINT_PTR) * (PUINT_PTR)Direct3D_Object;
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

/// <summary>
/// => D3D9 VMT repatch
/// Thread to set detour functions.
/// </summary>
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
			D3D9_IDirect3DTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DTexture9;
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
			D3D9_IDirect3DCubeTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DCubeTexture9;
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
			D3D9_IDirect3DVolumeTexture9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVolumeTexture9;
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
			D3D9_IDirect3DSurface9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DSurface9;
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
			D3D9_IDirect3DSwapChain9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DSwapChain9;
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
			D3D9_IDirect3DIndexBuffer9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DIndexBuffer9;
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
			D3D9_IDirect3DVertexBuffer9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVertexBuffer9;
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
			D3D9_IDirect3DPixelShader9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DPixelShader9;
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
			D3D9_IDirect3DVertexShader9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DVertexShader9;
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
			D3D9_IDirect3DStateBlock9_VMTable = (PUINT_PTR) * (PUINT_PTR)g_pAQU_TransferSite->m_pIDirect3DStateBlock9;
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

#pragma region /// => DirectX 10.0

/// <summary>
/// Thread to get the d3d10 device by mask.
/// </summary>
DWORD WINAPI D3D10_VMT_Mask(LPVOID Param)
{
	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);


#ifdef AQUILINUS_64
#else
	ID3D10Device* pDevice;

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
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		// create the thread to override the virtual methods table
		if (CreateThread(NULL, 0, D3D10_VMT_Repatch, NULL, 0, NULL) == NULL)
			return E_FAIL;
	}
#endif

	return D3D_OK;
}

/// <summary>
/// Thread to create the virtual method table for DirectX v10.0.
/// </summary>
DWORD WINAPI D3D10_VMT_Create(LPVOID Param)
{
	ID3D10Device* pDevice;

	// find the window we want to inject
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
		OutputDebugString(L"[AQU] Failed to set old function pointers !!");
	if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
		OutputDebugString(L"[AQU] Failed to set old function pointers !!");


	pDevice->Release();
	pSwapChain->Release();

	// create the thread to override the virtual methods table
	if (CreateThread(NULL, 0, D3D10_VMT_Repatch, NULL, 0, NULL) == NULL)
		return E_FAIL;

	return S_OK;
}

/// <summary>
/// Thread to set detour functions.
/// </summary>
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
		IDXGISwapChain* pSwapChain = nullptr;
		if (SUCCEEDED(pDCL_ID3D10Device->GetDXGISwapChain(&pSwapChain)))
		{
			// set swapchain vtable
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

			// release here and not in class !!
			pSwapChain->Release();

			// set super methods
			if (FAILED(pDCL_IDXGISwapChain->SetSuperFunctionPointers(D3D10_IDXGISwapChain_VMTable)))
				OutputDebugString(L"[AQU] Failed to set old function pointers !!");
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
		//Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	return E_FAIL;
}

#pragma endregion

#pragma region /// => DirectX 11.0
/// <summary>
/// => D3D11 Create Device Detour
/// </summary>
HRESULT WINAPI D3D11CreateDevice_Detour(
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext)
{
	OutputDebugString(L"[AQU] D3D11CreateDevice_Detour");

	// call super method... normal device created
	HRESULT nHr = D3D11CreateDevice_Super(pAdapter, DriverType, Software, Flags, pFeatureLevels,
		FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

	if (SUCCEEDED(nHr) && (ppDevice) && (ppImmediateContext))
	{
		if (*ppDevice)
		{
			D3D11_ID3D11Device_VMTable = (UINT_PTR*)*ppDevice;
			D3D11_ID3D11Device_VMTable = (UINT_PTR*)D3D11_ID3D11Device_VMTable[0];
			Detour_D3D11_ID3D11Device_VMTable();
		}
		if (*ppImmediateContext)
		{
			D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)*ppImmediateContext;
			D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)D3D11_ID3D11DeviceContext_VMTable[0];
			Detour_D3D11_ID3D11DeviceContext_VMTable();
		}
	}
	return nHr;
}

/// <summary>
/// => D3D11 Create Device Detour
/// </summary>
HRESULT WINAPI D3D11CreateDeviceAndSwapChain_Detour(
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	IDXGISwapChain** ppSwapChain,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext)
{
	OutputDebugString(L"[AQU] D3D11CreateDeviceAndSwapChain_Detour");

	// call super method... normal device created
	HRESULT nHr = D3D11CreateDeviceAndSwapChain_Super(pAdapter, DriverType, Software, Flags, pFeatureLevels,
		FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

	if (SUCCEEDED(nHr) && (ppDevice) && (ppImmediateContext) && (ppSwapChain))
	{
		if (*ppDevice)
		{
			D3D11_ID3D11Device_VMTable = (UINT_PTR*)*ppDevice;
			D3D11_ID3D11Device_VMTable = (UINT_PTR*)D3D11_ID3D11Device_VMTable[0];
			Detour_D3D11_ID3D11Device_VMTable();
		}
		if (*ppImmediateContext)
		{
			D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)*ppImmediateContext;
			D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)D3D11_ID3D11DeviceContext_VMTable[0];
			Detour_D3D11_ID3D11DeviceContext_VMTable();
		}
		if (*ppSwapChain)
		{
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)*ppSwapChain;
			D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];
			Detour_D3D10_IDXGISwapChain_VMTable();
		}
	}

	return nHr;
}

/// <summary>
/// Create Swapchain detour
/// </summary>
HRESULT WINAPI CreateSwapChain_Detour(
	IUnknown* pDevice,
	DXGI_SWAP_CHAIN_DESC* pDesc,
	IDXGISwapChain** ppSwapChain)
{
	OutputDebugString(L"[AQU] CreateSwapChain_Detour");

	return CreateSwapChain_Super(pDevice, pDesc, ppSwapChain);
}

/// <summary>
/// Create Swapchain1 detour
/// </summary>
HRESULT STDMETHODCALLTYPE CreateSwapChainForHwnd_Detour(
	_In_  IUnknown* pDevice,
	_In_  HWND hWnd,
	_In_  const DXGI_SWAP_CHAIN_DESC1* pDesc,
	_In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
	_In_opt_  IDXGIOutput* pRestrictToOutput,
	_COM_Outptr_  IDXGISwapChain1** ppSwapChain)
{
	return E_NOTIMPL;
};

/// <summary>
/// Create Swapchain1 detour
/// </summary>
HRESULT STDMETHODCALLTYPE CreateSwapChainForCoreWindow_Detour(
	_In_  IUnknown* pDevice,
	_In_  IUnknown* pWindow,
	_In_  const DXGI_SWAP_CHAIN_DESC1* pDesc,
	_In_opt_  IDXGIOutput* pRestrictToOutput,
	_COM_Outptr_  IDXGISwapChain1** ppSwapChain)
{
	return E_NOTIMPL;
};

/// <summary>
/// Create Swapchain1 detour
/// </summary>
HRESULT STDMETHODCALLTYPE CreateSwapChainForComposition_Detour(
	_In_  IUnknown* pDevice,
	_In_  const DXGI_SWAP_CHAIN_DESC1* pDesc,
	_In_opt_  IDXGIOutput* pRestrictToOutput,
	_COM_Outptr_  IDXGISwapChain1** ppSwapChain)
{
	return E_NOTIMPL;
};


/// <summary>
/// Thread to hook D3D11 classes via MinHook.
/// </summary>
DWORD WINAPI D3D11_Detour(LPVOID Param)
{
	UNREFERENCED_PARAMETER(Param);

	// init MinHook
	MH_Initialize();

	// TODO !! MAKE THIS OPTIONALLY

	/*/////////////////////////////////////////////////////////////

	// create the aquilinus detour classes
	pDCL_ID3D11Device = new DCL_ID3D11Device(g_pAQU_TransferSite);
	pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);
	pDCL_ID3D11DeviceContext = new DCL_ID3D11DeviceContext(g_pAQU_TransferSite);

	MH_CreateHook(&D3D11CreateDevice, &D3D11CreateDevice_Detour, reinterpret_cast<LPVOID*>(&D3D11CreateDevice_Super));

	// enable the hook
	if (MH_EnableHook(&D3D11CreateDevice) != MH_OK)
	{
		OutputDebugString(L"[AQU] Failed to detour method (MinHook) D3D11CreateDevice !!");
	}
	else OutputDebugStringW(L"[AQU] Enable Hook success D3D11CreateDevice !");

	MH_CreateHook(&D3D11CreateDeviceAndSwapChain, &D3D11CreateDeviceAndSwapChain_Detour, reinterpret_cast<LPVOID*>(&D3D11CreateDeviceAndSwapChain_Super));

	// enable the hook
	if (MH_EnableHook(&D3D11CreateDeviceAndSwapChain) != MH_OK)
	{
		OutputDebugString(L"[AQU] Failed to detour method (MinHook) D3D11CreateDeviceAndSwapChain !!");
	}
	else OutputDebugStringW(L"[AQU] Enable Hook success D3D11CreateDeviceAndSwapChain !");

	return D3D_OK;*/

	/*MH_CreateHook(&CreateSwapChain, &CreateSwapChain_Detour, reinterpret_cast<LPVOID*>(&CreateSwapChain_Super));

	// enable the hook
	if (MH_EnableHook(&CreateSwapChain) != MH_OK)
	{
		OutputDebugString(L"[AQU] Failed to detour method (MinHook) CreateSwapChain !!");
	}
	else OutputDebugStringW(L"[AQU] Enable Hook success CreateSwapChain !");*/

	//////////////////////////////////////////////////////////////

	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
	{
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);
	}

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;

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
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, 
		0, 
		featureLevels,
		numLevelsRequested,
		D3D11_SDK_VERSION, 
		&swapChainDesc, 
		&pSwapChain,
		&pDevice,
		&FeatureLevelsSupported,
		&pImmediateContext 
	)))
	{
		OutputDebug("Failed to create directX device and swapchain!");
		return E_FAIL;
	}

	// set vtables
	D3D11_ID3D11Device_VMTable = (UINT_PTR*)pDevice;
	D3D11_ID3D11Device_VMTable = (UINT_PTR*)D3D11_ID3D11Device_VMTable[0];
	D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)pImmediateContext;
	D3D11_ID3D11DeviceContext_VMTable = (UINT_PTR*)D3D11_ID3D11DeviceContext_VMTable[0];
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)pSwapChain;
	D3D10_IDXGISwapChain_VMTable = (UINT_PTR*)D3D10_IDXGISwapChain_VMTable[0];

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

	// create the aquilinus detour classes
	pDCL_ID3D11Device = new DCL_ID3D11Device(g_pAQU_TransferSite);
	pDCL_IDXGISwapChain = new DCL_IDXGISwapChain(g_pAQU_TransferSite);
	pDCL_ID3D11DeviceContext = new DCL_ID3D11DeviceContext(g_pAQU_TransferSite);

	// detour direct11 device + context methods
	if (D3D11_ID3D11Device_VMTable)
		Detour_D3D11_ID3D11Device_VMTable();

	// TODO !! MAKE DETOUR HERE OPTIONALLY
	/*
		if (D3D11_ID3D11DeviceContext_VMTable)
			Detour_D3D11_ID3D11DeviceContext_VMTable();
		if (D3D10_IDXGISwapChain_VMTable)
			Detour_D3D10_IDXGISwapChain_VMTable();
	}*/

	OutputDebugString(L"[AQU] VMTable hook installed.");

	// create the thread to override the virtual methods table
	if (CreateThread(NULL, 0, D3D11_VMT_Repatch, NULL, 0, NULL) == NULL)
		return E_FAIL;

	return D3D_OK;
}

/// <summary>
/// => D3D11 VMT Mask
/// Thread to get the d3d11 device by mask.
/// This method does following :
/// - it creates a test device + context + swapchain
/// - it gets the first VM-table address from both device and context
/// - it releases the test device + context + swapchain
/// - it locates the VM-table for both the device and context in "d3d11.dll"
/// - it sets/creates all necessary hooking fields
/// - it starts the repatching thread
/// </summary>
DWORD WINAPI D3D11_VMT_Mask(LPVOID Param)
{
	MH_Initialize();

	// time delay ?
	if (g_pAquilinusConfig->dwDetourTimeDelay > 15)
	{
		Sleep(g_pAquilinusConfig->dwDetourTimeDelay);
	}

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;

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
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		&FeatureLevelsSupported,
		&pImmediateContext
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
		OutputDebugStringA("[AQU] D3D11 VMTable found within d3d11.dll !!");
		OutputDebug("dwDXVMTableRoot %x", dwDXVMTableRoot);
		OutputDebug("dwDXContextVMTableRoot %x", dwDXContextVMTableRoot);
		OutputDebug("D3D11_ID3D11Device_VMTable: %x", D3D11_ID3D11Device_VMTable);
		OutputDebug("D3D11_ID3D11Device_AddRef %x", D3D11_ID3D11Device_AddRef);
		//#endif

		// set super methods
		if (FAILED(pDCL_ID3D11Device->SetSuperFunctionPointers(D3D11_ID3D11Device_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");
		if (FAILED(pDCL_ID3D11DeviceContext->SetSuperFunctionPointers(D3D11_ID3D11DeviceContext_VMTable)))
			OutputDebugString(L"[AQU] Failed to set old function pointers !!");

		// create the thread to override the virtual methods table
		if (CreateThread(NULL, 0, D3D11_VMT_Repatch, NULL, 0, NULL) == NULL)
			return E_FAIL;
}
	else
		OutputDebugStringA("[AQU] NO D3D11 VMTable found within d3d11.dll !!");

	OutputDebugString(L"[AQU] VMTable hook installed.");

	return D3D_OK;
}

/// <summary>
/// Thread to create the virtual method table for DirectX v10.0.
/// </summary>
DWORD WINAPI D3D11_VMT_Create(LPVOID Param)
{
	return E_NOTIMPL;
}

/// <summary>
/// Thread to set detour functions.
/// This method does following for the chosen DX11 VMTable hooks:
/// - it overrides device + context in "d3d11.dll", this will automatically
///   override the device and all context drawing methods in the game process
/// - it catches the swapchain and the context from the device class
/// - it overrides the swapchain and the (full) context
/// </summary>
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
		// no detour here !!
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
		// no detour here !!
		break;
	}

	// get the DXGI SwapChain + Context from the hooked (!) D3D11 device class !!
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
			IDXGISwapChain* pSwapChain = nullptr;
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
					OutputDebugString(L"[AQU] Failed to set old function pointers !!");
			}
		}
	}

	OutputDebugString(L"[AQU] Context/SwapChain provided by device.");

	// which technique ?
	switch (g_pAquilinusConfig->eInjectionTechnique[AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext])
	{
	case AQU_InjectionTechniques::VMTable:
		if (D3D11_ID3D11DeviceContext_VMTable)
			Override_D3D11_ID3D11DeviceContext_VMTable();
		break;
	case AQU_InjectionTechniques::Detour:
		if (D3D11_ID3D11DeviceContext_VMTable)
			Detour_D3D11_ID3D11DeviceContext_VMTable();
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
		if (D3D10_IDXGISwapChain_VMTable)
			Detour_D3D10_IDXGISwapChain_VMTable();
		break;
	}

	OutputDebugString(L"[AQU] VMTable hook successfull.");

	return D3D_OK;
}

#pragma endregion

#pragma endregion


