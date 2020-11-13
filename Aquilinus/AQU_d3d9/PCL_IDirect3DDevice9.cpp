/***************************************************************
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
****************************************************************/

#include "PCL_IDirect3DDevice9.h"

#ifdef _DEBUG
#include <windows.h>
#include <tchar.h>
#include <sstream>
typedef std::basic_stringstream<TCHAR> tstringstream;
template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9(...) ::OutputDebugString((tstringstream(), _T("***"), __VA_ARGS__, _T("\n")).str().c_str());
#else
#define OUTPUT_DEBUG_STRING_IDirect3DDevice9(...)
#endif

/**
* Constructor.
***/
PCL_IDirect3DDevice9::PCL_IDirect3DDevice9(IDirect3DDevice9* pOriginal, AQU_TransferSite* pTransferSite)
{
	m_pIDirect3DDevice9 = pOriginal;
	m_pTransferSite = pTransferSite;

	// create the working area class
	m_pWorkingArea = nullptr;

	WIN32_FIND_DATA ffd;
	wchar_t szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// clear enumeration vectors
	m_vcPluginNames.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();

	OutputDebugString(pTransferSite->m_pFileManager->GetPluginPath());

	// create plugins path
	wsprintf(szDir, L"%s%s", pTransferSite->m_pFileManager->GetPluginPath(), L"*");

	OutputDebugString(szDir);

	// find first file
	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
		return;

	// loop through files
	do
	{
		// get the extension
		const wchar_t *dot = wcsrchr(ffd.cFileName, '.');
		if(!dot || dot == ffd.cFileName) dot = L"";

		// ".dll" ?? 
		if (wcscmp(dot, L".dll") == NULL)
		{
			wchar_t szDllDir[MAX_PATH];

			OutputDebugString(L"Dll found :");
			OutputDebugString(ffd.cFileName);

			// create plugin path and load module
			wsprintf(szDllDir, L"%s%s", pTransferSite->m_pFileManager->GetPluginPath(), ffd.cFileName);

			// convert to LPCSTR
			int size = wcslen(szDllDir);
			size += 2;
			char *szDll = (char *)malloc( size );
			wcstombs_s(NULL, szDll, size, 
				szDllDir, size);

			HMODULE hm = LoadLibraryA(szDll);

			if (hm != NULL)
			{
				// define plugin creation type
				typedef AQU_Nodus* (*AQU_Nodus_Create)();
				AQU_Nodus_Create m_pAQU_Nodus_Create;

				// get plugin creation method
				m_pAQU_Nodus_Create = (AQU_Nodus_Create)GetProcAddress(hm, "AQU_Nodus_Create");

				if (m_pAQU_Nodus_Create)
				{
					// create a test nodus class to test for directx version support
					AQU_Nodus* pNodus = m_pAQU_Nodus_Create();

					// get the plugin name
					LPSTR szName = new char[64];
					const char* szSrcName = pNodus->GetNodeType();
					UINT dwLen = strlen(szSrcName);
					if (dwLen > 63) dwLen = 63;
					CopyMemory((void*)szName, (void*)szSrcName, dwLen+1);

					// get the path name
					LPWSTR szPathName = new wchar_t[MAX_PATH];
					dwLen = wcslen(szDllDir);
					if (dwLen > MAX_PATH-1) dwLen = MAX_PATH-1;
					CopyMemory((void*)szPathName, (void*)szDllDir, (dwLen+1)*sizeof(wchar_t));

					// get the category name
					LPWSTR szCategoryName = new wchar_t[64];
					LPWSTR szSrcNameW = pNodus->GetCategory();
					dwLen = wcslen(szSrcNameW);
					if (dwLen > 64-1) dwLen = 64-1;
					CopyMemory((void*)szCategoryName, (void*)szSrcNameW, (dwLen+1)*sizeof(wchar_t));

					// enumerate all necessary data
					m_vcPluginNames.push_back(szName);
					m_vcPluginFilePathes.push_back(szPathName);
					m_vcPluginIDs.push_back(pNodus->GetNodeTypeId());
					m_vcPluginCategories.push_back(szCategoryName);

					// keep the prototypes to delete them at class destruction
					delete pNodus;
				}

				FreeLibrary(hm);
			}
			else MessageBox(NULL, ffd.cFileName, L"Failed to load !", MB_OK);

		}

	}
	while (FindNextFile(hFind, &ffd) != 0);

	// output debug data
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		OutputDebugStringA(m_vcPluginNames[i]);
		OutputDebugStringW(m_vcPluginFilePathes[i]);
		OutputDebugStringW(m_vcPluginCategories[i]);
	}

	OutputDebugString(L"Aquilinus: ENUMERATION WORKS !");
}

/**
* Deconstructor. Deletes enumeration data.
***/
PCL_IDirect3DDevice9::~PCL_IDirect3DDevice9(void)
{
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginNames.clear();

	auto it1 = m_vcPlugins.begin();
	while (it1 != m_vcPlugins.end()) 
	{
		if ((*it1) != NULL) 
			delete (*it1);
		it1 = m_vcPlugins.erase(it1);
	}
	auto it2 = m_vcPluginHandles.begin();
	while (it2 != m_vcPluginHandles.end()) 
	{
		if ((*it2) != NULL) 
			FreeLibrary(*it2);
		it2 = m_vcPluginHandles.erase(it2);
	}
}

/**
* Tests riid to return wrapped interfaces.
***/
HRESULT PCL_IDirect3DDevice9::QueryInterface(REFIID riid, void** ppvObj)
{
	OUTPUT_DEBUG_STRING_IDirect3DDevice9(L"IDirect3DDevice9::QueryInterface");

	//// not force D3D, node present and invokers connected ? set node output data and return node provoke
	//if ((!m_pTransferSite->m_bForceD3D) && (m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]) && (m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers.size() > 0))
	//{
	//	// set data
	//	m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_paCommanders[0]->m_pOutput = (void*)&riid;
	//	m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_paCommanders[1]->m_pOutput = (void*)&ppvObj;

	//	// provoke, set bForceD3D to "true" for any provoking circle
	//	m_pTransferSite->m_bForceD3D = true;
	//	void* pvRet = m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->Provoke((void*)m_pIDirect3DDevice9, *m_pTransferSite->m_ppaNodes);
	//	m_pTransferSite->m_bForceD3D = false;

	//	// replace method call only if the nodes first connected node wants to replace the call
	//	if ((*m_pTransferSite->m_ppaNodes)[m_pTransferSite->m_ppNOD_IDirect3DDevice9[VMT_IUNKNOWN::QueryInterface]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn)
	//	{
	//		// get return value.. MUST be STATIC !
	//		static HRESULT hr = (HRESULT)*(HRESULT*)pvRet;
	//		return hr;
	//	}
	//	else
	//		return m_pIDirect3DDevice9->QueryInterface(riid, ppvObj);
	//}
	//else
	return m_pIDirect3DDevice9->QueryInterface(riid, ppvObj);
}

/**
* Base functionality.
***/
ULONG PCL_IDirect3DDevice9::AddRef(void)
{
	return m_pIDirect3DDevice9->AddRef();
}

/**
* Base functionality.
***/
ULONG PCL_IDirect3DDevice9::Release(void)
{
	// declare device extern
	extern PCL_IDirect3DDevice9* g_pPCL_IDirect3DDevice9;

	ULONG count = m_pIDirect3DDevice9->Release();
	if(count==0)
	{
		g_pPCL_IDirect3DDevice9 = NULL;
		delete this;
	}

	return count;
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::TestCooperativeLevel(void)
{
	return m_pIDirect3DDevice9->TestCooperativeLevel();
}

/**
* Base functionality.
***/
UINT PCL_IDirect3DDevice9::GetAvailableTextureMem(void)
{
	return m_pIDirect3DDevice9->GetAvailableTextureMem();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::EvictManagedResources(void)
{
	return m_pIDirect3DDevice9->EvictManagedResources();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	return m_pIDirect3DDevice9->GetDirect3D(ppD3D9);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return m_pIDirect3DDevice9->GetDeviceCaps(pCaps);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	return m_pIDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pIDirect3DDevice9->GetCreationParameters(pParameters);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	return m_pIDirect3DDevice9->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap);
}

/**
* Base functionality.
***/
void PCL_IDirect3DDevice9::SetCursorPosition(int X,int Y,DWORD Flags)
{
	return m_pIDirect3DDevice9->SetCursorPosition(X,Y,Flags);
}

/**
* Base functionality.
***/
BOOL PCL_IDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return m_pIDirect3DDevice9->ShowCursor(bShow);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)  
{
	return m_pIDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pIDirect3DDevice9->GetSwapChain(iSwapChain,pSwapChain);
}

/**
* Base functionality.
***/
UINT PCL_IDirect3DDevice9::GetNumberOfSwapChains(void)
{
	return m_pIDirect3DDevice9->GetNumberOfSwapChains();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return m_pIDirect3DDevice9->Reset(pPresentationParameters);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	this->DrawIndicator();

	HRESULT hres = m_pIDirect3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return hres;
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	return m_pIDirect3DDevice9->GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	return m_pIDirect3DDevice9->GetRasterStatus(iSwapChain,pRasterStatus);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return m_pIDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

/**
* Base functionality.
***/
void PCL_IDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	return m_pIDirect3DDevice9->SetGammaRamp(iSwapChain,Flags,pRamp);
}

/**
* Base functionality.
***/
void PCL_IDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	return m_pIDirect3DDevice9->GetGammaRamp(iSwapChain,pRamp);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	return m_pIDirect3DDevice9->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	return m_pIDirect3DDevice9->UpdateTexture(pSourceTexture,pDestinationTexture);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	return m_pIDirect3DDevice9->GetRenderTargetData(pRenderTarget,pDestSurface);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	return m_pIDirect3DDevice9->GetFrontBufferData(iSwapChain,pDestSurface);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	return m_pIDirect3DDevice9->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	return m_pIDirect3DDevice9->ColorFill(pSurface,pRect,color);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pIDirect3DDevice9->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	return m_pIDirect3DDevice9->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	return m_pIDirect3DDevice9->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pIDirect3DDevice9->SetDepthStencilSurface(pNewZStencil);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return m_pIDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::BeginScene(void)
{
	return m_pIDirect3DDevice9->BeginScene();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::EndScene(void)
{
	return m_pIDirect3DDevice9->EndScene();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	return m_pIDirect3DDevice9->Clear(Count,pRects,Flags,Color,Z,Stencil);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	return m_pIDirect3DDevice9->SetTransform(State,pMatrix);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	return m_pIDirect3DDevice9->GetTransform(State,pMatrix);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	return m_pIDirect3DDevice9->MultiplyTransform(State,pMatrix);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	return m_pIDirect3DDevice9->SetViewport(pViewport);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
	return m_pIDirect3DDevice9->GetViewport(pViewport);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return m_pIDirect3DDevice9->SetMaterial(pMaterial);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return m_pIDirect3DDevice9->GetMaterial(pMaterial);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
	return m_pIDirect3DDevice9->SetLight(Index,pLight);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
	return m_pIDirect3DDevice9->GetLight(Index,pLight);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::LightEnable(DWORD Index,BOOL Enable)
{
	return m_pIDirect3DDevice9->LightEnable(Index,Enable);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	return m_pIDirect3DDevice9->GetLightEnable(Index, pEnable);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetClipPlane(DWORD Index,CONST float* pPlane)
{
	return m_pIDirect3DDevice9->SetClipPlane(Index, pPlane);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetClipPlane(DWORD Index,float* pPlane)
{
	return m_pIDirect3DDevice9->GetClipPlane(Index,pPlane);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	return m_pIDirect3DDevice9->SetRenderState(State, Value);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
	return m_pIDirect3DDevice9->GetRenderState(State, pValue);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	return m_pIDirect3DDevice9->CreateStateBlock(Type,ppSB);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::BeginStateBlock(void)
{
	return m_pIDirect3DDevice9->BeginStateBlock();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pIDirect3DDevice9->EndStateBlock(ppSB);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pIDirect3DDevice9->SetClipStatus(pClipStatus);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return m_pIDirect3DDevice9->GetClipStatus( pClipStatus);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	return m_pIDirect3DDevice9->GetTexture(Stage,ppTexture);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	return m_pIDirect3DDevice9->SetTexture(Stage,pTexture);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	return m_pIDirect3DDevice9->GetTextureStageState(Stage,Type, pValue);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	return m_pIDirect3DDevice9->SetTextureStageState(Stage,Type,Value);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	return m_pIDirect3DDevice9->GetSamplerState(Sampler,Type, pValue);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	return m_pIDirect3DDevice9->SetSamplerState(Sampler,Type,Value);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
	return m_pIDirect3DDevice9->ValidateDevice( pNumPasses);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	return m_pIDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	return m_pIDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pIDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	return m_pIDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	return m_pIDirect3DDevice9->SetScissorRect( pRect);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetScissorRect( RECT* pRect)
{
	return m_pIDirect3DDevice9->GetScissorRect( pRect);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return m_pIDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware);
}

/**
* Base functionality.
***/
BOOL    PCL_IDirect3DDevice9::GetSoftwareVertexProcessing(void)
{
	return m_pIDirect3DDevice9->GetSoftwareVertexProcessing();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetNPatchMode(float nSegments)
{
	return m_pIDirect3DDevice9->SetNPatchMode(nSegments);
}

/**
* Base functionality.
***/
float PCL_IDirect3DDevice9::GetNPatchMode(void)
{
	return m_pIDirect3DDevice9->GetNPatchMode();
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	return m_pIDirect3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	return m_pIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pIDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,VertexStreamZeroStride);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	return m_pIDirect3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pIDirect3DDevice9->CreateVertexDeclaration( pVertexElements,ppDecl);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return m_pIDirect3DDevice9->SetVertexDeclaration(pDecl);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pIDirect3DDevice9->GetVertexDeclaration(ppDecl);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetFVF(DWORD FVF)
{
	return m_pIDirect3DDevice9->SetFVF(FVF);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	return m_pIDirect3DDevice9->GetFVF(pFVF);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	return m_pIDirect3DDevice9->CreateVertexShader(pFunction,ppShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	return m_pIDirect3DDevice9->SetVertexShader(pShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return m_pIDirect3DDevice9->GetVertexShader(ppShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	return m_pIDirect3DDevice9->SetVertexShaderConstantF(StartRegister,pConstantData, Vector4fCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pIDirect3DDevice9->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	return m_pIDirect3DDevice9->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pIDirect3DDevice9->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	return m_pIDirect3DDevice9->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pIDirect3DDevice9->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	return m_pIDirect3DDevice9->SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride)
{
	return m_pIDirect3DDevice9->GetStreamSource(StreamNumber,ppStreamData,OffsetInBytes,pStride);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{
	return m_pIDirect3DDevice9->SetStreamSourceFreq(StreamNumber,Divider);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider)
{
	return m_pIDirect3DDevice9->GetStreamSourceFreq(StreamNumber,Divider);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pIDirect3DDevice9->SetIndices(pIndexData);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pIDirect3DDevice9->GetIndices(ppIndexData);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	return m_pIDirect3DDevice9->CreatePixelShader(pFunction,ppShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return m_pIDirect3DDevice9->SetPixelShader(pShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return m_pIDirect3DDevice9->GetPixelShader(ppShader);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	return m_pIDirect3DDevice9->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pIDirect3DDevice9->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	return m_pIDirect3DDevice9->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pIDirect3DDevice9->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	return m_pIDirect3DDevice9->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pIDirect3DDevice9->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pIDirect3DDevice9->DrawRectPatch(Handle,pNumSegs, pRectPatchInfo);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pIDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::DeletePatch(UINT Handle)
{
	return m_pIDirect3DDevice9->DeletePatch(Handle);
}

/**
* Base functionality.
***/
HRESULT PCL_IDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	return m_pIDirect3DDevice9->CreateQuery(Type,ppQuery);
}

/**
* Draw indicator.
***/
void PCL_IDirect3DDevice9::DrawIndicator(void)
{
	D3DRECT rec = {1,1,50,50};
	m_pIDirect3DDevice9->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,0),0 ,0);
}


