/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DDevice9Ex.cpp> and
Class <BaseDirect3DDevice9Ex> :
Copyright (C) 2013 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#include "Direct3DDevice9Ex.h"
#include "Main.h"

/**
* Constructor. 
* @param pDevice Imbed actual device. 
* @param pCreatedBy Pointer to the object that created the device. 
***/
BaseDirect3DDevice9Ex::BaseDirect3DDevice9Ex(IDirect3DDevice9Ex* pDevice, BaseDirect3D9* pCreatedBy) :
	m_pDeviceEx(pDevice),
	m_pCreatedBy(pCreatedBy),
	m_nRefCount(1)
{
#ifdef _EXPORT_LOGFILE
	m_logFile.open("logD3DDeviceEx.txt", std::ios::out);
#endif
}

/**
* Destructor. 
* Releases embedded device. 
***/
BaseDirect3DDevice9Ex::~BaseDirect3DDevice9Ex()
{

#ifdef _EXPORT_LOGFILE
	m_logFile.close();
#endif

	if(m_pDeviceEx)
		m_pDeviceEx->Release();
}

/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::QueryInterface(REFIID riid, LPVOID* ppv)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "QueryInterface" << std::endl;
#endif

	return m_pDeviceEx->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DDevice9Ex::AddRef()
{	 
#ifdef _EXPORT_LOGFILE
	m_logFile << "AddRef" << std::endl;
#endif

	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DDevice9Ex::Release()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "Release" << std::endl;
#endif

	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}	

	return m_nRefCount;
}

/**
* Base TestCooperativeLevel functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::TestCooperativeLevel()
{	 
#ifdef _EXPORT_LOGFILE
	m_logFile << "TestCooperativeLevel" << std::endl;
#endif

	return m_pDeviceEx->TestCooperativeLevel();
}

/**
* Base GetAvailableTextureMem functionality.
***/
UINT WINAPI BaseDirect3DDevice9Ex::GetAvailableTextureMem()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetAvailableTextureMem" << std::endl;
#endif

	return m_pDeviceEx->GetAvailableTextureMem();
}

/**
* Base EvictManagedResources functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EvictManagedResources()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "EvictManagedResources" << std::endl;
#endif

	return m_pDeviceEx->EvictManagedResources();
}

/**
* Base GetDirect3D functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDirect3D(IDirect3D9** ppD3D9)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetDirect3D" << std::endl;
#endif

	if (!m_pCreatedBy)
		return D3DERR_INVALIDCALL;
	else {
		*ppD3D9 = m_pCreatedBy;
		m_pCreatedBy->AddRef();
		return D3D_OK;
	}
}

/**
* Base GetDeviceCaps functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDeviceCaps(D3DCAPS9* pCaps)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetDeviceCaps" << std::endl;
#endif

	return m_pDeviceEx->GetDeviceCaps(pCaps);
}

/**
* Base GetDisplayMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetDisplayMode" << std::endl;
#endif

	return m_pDeviceEx->GetDisplayMode(iSwapChain, pMode);
}

/**
* Base GetCreationParameters functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetCreationParameters" << std::endl;
#endif

	return m_pDeviceEx->GetCreationParameters(pParameters);
}

/**
* Base SetCursorProperties functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetCursorProperties" << std::endl;
#endif

	return m_pDeviceEx->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

/**
* Base SetCursorPosition functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::SetCursorPosition(int X,int Y,DWORD Flags)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetCursorPosition" << std::endl;
#endif

	return m_pDeviceEx->SetCursorPosition(X, Y, Flags);
}

/**
* Base ShowCursor functionality.
***/
BOOL WINAPI BaseDirect3DDevice9Ex::ShowCursor(BOOL bShow)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ShowCursor" << std::endl;
#endif

	return m_pDeviceEx->ShowCursor(bShow);
}

/**
* Base CreateAdditionalSwapChain functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateAdditionalSwapChain" << std::endl;
#endif

	return m_pDeviceEx->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

/**
* Base GetSwapChain functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetSwapChain" << std::endl;
#endif

	return m_pDeviceEx->GetSwapChain(iSwapChain, pSwapChain);
}

/**
* Base GetNumberOfSwapChains functionality.
***/
UINT WINAPI BaseDirect3DDevice9Ex::GetNumberOfSwapChains()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetNumberOfSwapChains" << std::endl;
#endif

	return m_pDeviceEx->GetNumberOfSwapChains();
}

/**
* Base Reset functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "Reset" << std::endl;
#endif

	return m_pDeviceEx->Reset(pPresentationParameters);
}

/**
* Base Present functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "Present" << std::endl;
#endif

	return m_pDeviceEx->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/**
* Base GetBackBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetBackBuffer" << std::endl;
#endif

	return m_pDeviceEx->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

/**
* Base GetRasterStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetRasterStatus" << std::endl;
#endif

	return m_pDeviceEx->GetRasterStatus(iSwapChain, pRasterStatus);
}

/**
* Base SetDialogBoxMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetDialogBoxMode(BOOL bEnableDialogs)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetDialogBoxMode" << std::endl;
#endif

	return m_pDeviceEx->SetDialogBoxMode(bEnableDialogs);
}

/**
* Base SetGammaRamp functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{	
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetGammaRamp" << std::endl;
#endif

	return m_pDeviceEx->SetGammaRamp(iSwapChain, Flags, pRamp);
}

/**
* Base GetGammaRamp functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetGammaRamp" << std::endl;
#endif

	return m_pDeviceEx->GetGammaRamp(iSwapChain, pRamp);
}

/**
* Base CreateTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateTexture" << std::endl;
#endif

	return m_pDeviceEx->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

/**
* Base CreateVolumeTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateVolumeTexture" << std::endl;
#endif

	return m_pDeviceEx->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

/**
* Base CreateCubeTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{	
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateCubeTexture" << std::endl;
#endif

	return m_pDeviceEx->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

/**
* Base CreateVertexBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{	
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateVertexBuffer" << std::endl;
#endif

	return m_pDeviceEx->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

/**
* Base CreateIndexBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{	
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateIndexBuffer" << std::endl;
#endif

	return m_pDeviceEx->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

/**
* Base CreateRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateRenderTarget" << std::endl;
#endif

	return m_pDeviceEx->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

/**
* Base CreateDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateDepthStencilSurface" << std::endl;
#endif

	return m_pDeviceEx->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

/**
* Base UpdateSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "UpdateSurface" << std::endl;
#endif

	return m_pDeviceEx->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

/**
* Base UpdateTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "UpdateTexture" << std::endl;
#endif

	return m_pDeviceEx->UpdateTexture(pSourceTexture, pDestinationTexture);
}

/**
* Base GetRenderTargetData functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetRenderTargetData" << std::endl;
#endif

	return m_pDeviceEx->GetRenderTargetData(pRenderTarget, pDestSurface);
}

/**
* Base GetFrontBufferData functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetFrontBufferData" << std::endl;
#endif

	return m_pDeviceEx->GetFrontBufferData(iSwapChain, pDestSurface);
}

/**
* Base StretchRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "StretchRect" << std::endl;
#endif

	return m_pDeviceEx->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

/**
* Base ColorFill functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ColorFill" << std::endl;
#endif

	return m_pDeviceEx->ColorFill(pSurface, pRect, color);
}

/**
* Base CreateOffscreenPlainSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateOffscreenPlainSurface" << std::endl;
#endif

	return m_pDeviceEx->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

/**
* Base SetRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetRenderTarget" << std::endl;
#endif

	return m_pDeviceEx->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

/**
* Base GetRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetRenderTarget" << std::endl;

#endif
	return m_pDeviceEx->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

/**
* Base SetDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetDepthStencilSurface" << std::endl;
#endif

	return m_pDeviceEx->SetDepthStencilSurface(pNewZStencil);
}

/**
* Base GetDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetDepthStencilSurface" << std::endl;
#endif

	return m_pDeviceEx->GetDepthStencilSurface(ppZStencilSurface);
}

/**
* Base BeginScene functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::BeginScene()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "BeginScene" << std::endl;
#endif

	return m_pDeviceEx->BeginScene();
}

/**
* Base EndScene functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EndScene()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "EndScene" << std::endl;
#endif

	return m_pDeviceEx->EndScene();
}

/**
* Base Clear functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "Clear" << std::endl;
#endif

	return m_pDeviceEx->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

/**
* Base SetTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetTransform" << std::endl;
#endif

	return m_pDeviceEx->SetTransform(State, pMatrix);
}

/**
* Base GetTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetTransform" << std::endl;
#endif

	return m_pDeviceEx->GetTransform(State, pMatrix);
}

/**
* Base MultiplyTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "MultiplyTransform" << std::endl;
#endif

	return m_pDeviceEx->MultiplyTransform(State, pMatrix);
}

/**
* Base SetViewport functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetViewport" << std::endl;
#endif

	return m_pDeviceEx->SetViewport(pViewport);
}

/**
* Base GetViewport functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetViewport(D3DVIEWPORT9* pViewport)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetViewport" << std::endl;
#endif

	return m_pDeviceEx->GetViewport(pViewport);
}

/**
* Base SetMaterial functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetMaterial" << std::endl;
#endif

	return m_pDeviceEx->SetMaterial(pMaterial);
}

/**
* Base GetMaterial functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetMaterial(D3DMATERIAL9* pMaterial)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetMaterial" << std::endl;
#endif

	return m_pDeviceEx->GetMaterial(pMaterial);
}

/**
* Base SetLight functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetLight" << std::endl;
#endif

	return m_pDeviceEx->SetLight(Index, pLight);
}

/**
* Base GetLight functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetLight" << std::endl;
#endif

	return m_pDeviceEx->GetLight(Index, pLight);
}

/**
* Base LightEnable functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::LightEnable(DWORD Index,BOOL Enable)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "LightEnable" << std::endl;
#endif

	return m_pDeviceEx->LightEnable(Index, Enable);
}

/**
* Base GetLightEnable functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetLightEnable(DWORD Index,BOOL* pEnable)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetLightEnable" << std::endl;
#endif

	return m_pDeviceEx->GetLightEnable(Index, pEnable);
}

/**
* Base SetClipPlane functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetClipPlane(DWORD Index,CONST float* pPlane)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetClipPlane" << std::endl;
#endif

	return m_pDeviceEx->SetClipPlane(Index, pPlane);
}

/**
* Base GetClipPlane functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetClipPlane(DWORD Index,float* pPlane)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetClipPlane" << std::endl;
#endif

	return m_pDeviceEx->GetClipPlane(Index, pPlane);
}

/**
* Base SetRenderState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetRenderState" << std::endl;
#endif

	return m_pDeviceEx->SetRenderState(State, Value);
}

/**
* Base GetRenderState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetRenderState" << std::endl;
#endif

	return m_pDeviceEx->GetRenderState(State, pValue);
}

/**
* Base CreateStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateStateBlock" << std::endl;
#endif

	return m_pDeviceEx->CreateStateBlock(Type, ppSB);
}

/**
* Base BeginStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::BeginStateBlock()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "BeginStateBlock" << std::endl;
#endif

	return m_pDeviceEx->BeginStateBlock();
}

/**
* Base EndStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "EndStateBlock" << std::endl;
#endif

	return m_pDeviceEx->EndStateBlock(ppSB);
}

/**
* Base SetClipStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetClipStatus" << std::endl;
#endif

	return m_pDeviceEx->SetClipStatus(pClipStatus);
}

/**
* Base GetClipStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetClipStatus" << std::endl;
#endif

	return m_pDeviceEx->GetClipStatus(pClipStatus);
}

/**
* Base GetTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetTexture" << std::endl;
#endif

	return m_pDeviceEx->GetTexture(Stage, ppTexture);
}

/**
* Base SetTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetTexture" << std::endl;
#endif

	return m_pDeviceEx->SetTexture(Stage, pTexture);
}

/**
* Base GetTextureStageState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetTextureStageState" << std::endl;
#endif

	return m_pDeviceEx->GetTextureStageState(Stage, Type, pValue);
}

/**
* Base SetTextureStageState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetTextureStageState" << std::endl;
#endif

	return m_pDeviceEx->SetTextureStageState(Stage, Type, Value);
}

/**
* Base GetSamplerState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetSamplerState" << std::endl;
#endif

	return m_pDeviceEx->GetSamplerState(Sampler, Type, pValue);
}

/**
* Base SetSamplerState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetSamplerState" << std::endl;
#endif

	return m_pDeviceEx->SetSamplerState(Sampler, Type, Value);
}

/**
* Base ValidateDevice functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ValidateDevice(DWORD* pNumPasses)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ValidateDevice" << std::endl;
#endif

	return m_pDeviceEx->ValidateDevice(pNumPasses);
}

/**
* Base SetPaletteEntries functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetPaletteEntries" << std::endl;
#endif

	return m_pDeviceEx->SetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base GetPaletteEntries functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetPaletteEntries" << std::endl;
#endif

	return m_pDeviceEx->GetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base SetCurrentTexturePalette functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetCurrentTexturePalette(UINT PaletteNumber)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetCurrentTexturePalette" << std::endl;
#endif

	return m_pDeviceEx->SetCurrentTexturePalette(PaletteNumber);
}

/**
* Base GetCurrentTexturePalette functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetCurrentTexturePalette(UINT *PaletteNumber)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetCurrentTexturePalette" << std::endl;
#endif

	return m_pDeviceEx->GetCurrentTexturePalette(PaletteNumber);
}

/**
* Base SetScissorRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetScissorRect(CONST RECT* pRect)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetScissorRect" << std::endl;
#endif

	return m_pDeviceEx->SetScissorRect(pRect);
}

/**
* Base GetScissorRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetScissorRect(RECT* pRect)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetScissorRect" << std::endl;
#endif

	return m_pDeviceEx->GetScissorRect(pRect);
}

/**
* Base SetSoftwareVertexProcessing functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetSoftwareVertexProcessing(BOOL bSoftware)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetSoftwareVertexProcessing" << std::endl;
#endif

	return m_pDeviceEx->SetSoftwareVertexProcessing(bSoftware);
}

/**
* Base GetSoftwareVertexProcessing functionality.
***/
BOOL WINAPI BaseDirect3DDevice9Ex::GetSoftwareVertexProcessing()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetSoftwareVertexProcessing" << std::endl;
#endif

	return m_pDeviceEx->GetSoftwareVertexProcessing();
}

/**
* Base SetNPatchMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetNPatchMode(float nSegments)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetNPatchMode" << std::endl;
#endif

	return m_pDeviceEx->SetNPatchMode(nSegments);
}

/**
* Base GetNPatchMode functionality.
***/
float WINAPI BaseDirect3DDevice9Ex::GetNPatchMode()
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetNPatchMode" << std::endl;
#endif

	return m_pDeviceEx->GetNPatchMode();
}

/**
* Base DrawPrimitive functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawPrimitive" << std::endl;
#endif

	return m_pDeviceEx->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

/**
* Base DrawIndexedPrimitive functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawIndexedPrimitive" << std::endl;
#endif

	return m_pDeviceEx->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

/**
* Base DrawPrimitiveUP functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawPrimitiveUP" << std::endl;
#endif

	return m_pDeviceEx->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Base DrawIndexedPrimitiveUP functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawIndexedPrimitiveUP" << std::endl;
#endif

	return m_pDeviceEx->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Base ProcessVertices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ProcessVertices" << std::endl;
#endif

	return m_pDeviceEx->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

/**
* Base CreateVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateVertexDeclaration" << std::endl;
#endif

	return m_pDeviceEx->CreateVertexDeclaration(pVertexElements, ppDecl);
}

/**
* Base SetVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetVertexDeclaration" << std::endl;
#endif

	return m_pDeviceEx->SetVertexDeclaration(pDecl);
}

/**
* Base GetVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetVertexDeclaration" << std::endl;
#endif

	return m_pDeviceEx->GetVertexDeclaration(ppDecl);
}

/**
* Base SetFVF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetFVF(DWORD FVF)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetFVF" << std::endl;
#endif

	return m_pDeviceEx->SetFVF(FVF);
}

/**
* Base GetFVF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetFVF(DWORD* pFVF)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetFVF" << std::endl;
#endif

	return m_pDeviceEx->GetFVF(pFVF);
}

/**
* Base CreateVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateVertexShader" << std::endl;
#endif

	return m_pDeviceEx->CreateVertexShader(pFunction, ppShader);
}

/**
* Base SetVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShader(IDirect3DVertexShader9* pShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetVertexShader" << std::endl;
#endif

	return m_pDeviceEx->SetVertexShader(pShader);
}

/**
* Base GetVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetVertexShader" << std::endl;
#endif

	return m_pDeviceEx->GetVertexShader(ppShader);
}

/**
* Base SetVertexShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetVertexShaderConstantF" << std::endl;
#endif

	return m_pDeviceEx->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base GetVertexShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetVertexShaderConstantF" << std::endl;
#endif

	return m_pDeviceEx->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base SetVertexShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetVertexShaderConstantI" << std::endl;
#endif

	return m_pDeviceEx->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base GetVertexShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetVertexShaderConstantI" << std::endl;
#endif

	return m_pDeviceEx->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base SetVertexShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetVertexShaderConstantB" << std::endl;
#endif

	return m_pDeviceEx->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base GetVertexShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetVertexShaderConstantB" << std::endl;
#endif

	return m_pDeviceEx->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base SetStreamSource functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetStreamSource" << std::endl;
#endif

	return m_pDeviceEx->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

/**
* Base GetStreamSource functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetStreamSource" << std::endl;
#endif

	return m_pDeviceEx->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

/**
* Base SetStreamSourceFreq functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetStreamSourceFreq(UINT StreamNumber,UINT Setting)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetStreamSourceFreq" << std::endl;
#endif

	return m_pDeviceEx->SetStreamSourceFreq(StreamNumber, Setting);
}

/**
* Base GetStreamSourceFreq functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetStreamSourceFreq" << std::endl;
#endif

	return m_pDeviceEx->GetStreamSourceFreq(StreamNumber, pSetting);
}

/**
* Base SetIndices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetIndices" << std::endl;
#endif

	return m_pDeviceEx->SetIndices(pIndexData);
}

/**
* Base GetIndices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetIndices" << std::endl;
#endif

	return m_pDeviceEx->GetIndices(ppIndexData);
}

/**
* Base CreatePixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreatePixelShader" << std::endl;
#endif

	return m_pDeviceEx->CreatePixelShader(pFunction, ppShader);
}

/**
* Base SetPixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShader(IDirect3DPixelShader9* pShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetPixelShader" << std::endl;
#endif

	return m_pDeviceEx->SetPixelShader(pShader);
}

/**
* Base GetPixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetPixelShader" << std::endl;
#endif

	return m_pDeviceEx->GetPixelShader(ppShader);
}

/**
* Base SetPixelShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetPixelShaderConstantF" << std::endl;
#endif

	return m_pDeviceEx->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base GetPixelShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetPixelShaderConstantF" << std::endl;
#endif

	return m_pDeviceEx->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base SetPixelShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetPixelShaderConstantI" << std::endl;
#endif

	return m_pDeviceEx->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base GetPixelShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetPixelShaderConstantI" << std::endl;
#endif

	return m_pDeviceEx->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base SetPixelShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetPixelShaderConstantB" << std::endl;
#endif

	return m_pDeviceEx->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base GetPixelShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetPixelShaderConstantB" << std::endl;
#endif

	return m_pDeviceEx->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base DrawRectPatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawRectPatch" << std::endl;

#endif
	return m_pDeviceEx->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

/**
* Base DrawTriPatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DrawTriPatch" << std::endl;
#endif

	return m_pDeviceEx->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

/**
* Base DeletePatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DeletePatch(UINT Handle)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "DeletePatch" << std::endl;
#endif

	return m_pDeviceEx->DeletePatch(Handle);
}

/**
* Base CreateQuery functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateQuery" << std::endl;
#endif

	return m_pDeviceEx->CreateQuery(Type, ppQuery);
}

/**
* Base SetConvolutionMonoKernel functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetConvolutionMonoKernel(UINT width,UINT height,float* rows,float* columns)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetConvolutionMonoKernel" << std::endl;
#endif

	return m_pDeviceEx->SetConvolutionMonoKernel(width, height, rows, columns);
}

/**
* Base ComposeRects functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ComposeRects(IDirect3DSurface9* pSrc,IDirect3DSurface9* pDst,IDirect3DVertexBuffer9* pSrcRectDescs,UINT NumRects,IDirect3DVertexBuffer9* pDstRectDescs,D3DCOMPOSERECTSOP Operation,int Xoffset,int Yoffset)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ComposeRects" << std::endl;
#endif

	return m_pDeviceEx->ComposeRects(pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset);
}

/**
* Base PresentEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::PresentEx(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "PresentEx" << std::endl;
#endif

	return m_pDeviceEx->PresentEx(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/**
* Base GetGPUThreadPriority functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetGPUThreadPriority(INT* pPriority)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetGPUThreadPriority" << std::endl;
#endif

	return m_pDeviceEx->GetGPUThreadPriority(pPriority);
}

/**
* Base SetGPUThreadPriority functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetGPUThreadPriority(INT Priority)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetGPUThreadPriority" << std::endl;
#endif

	return m_pDeviceEx->SetGPUThreadPriority(Priority);
}

/**
* Base WaitForVBlank functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::WaitForVBlank(UINT iSwapChain)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "WaitForVBlank" << std::endl;
#endif

	return m_pDeviceEx->WaitForVBlank(iSwapChain);
}

/**
* Base CheckResourceResidency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CheckResourceResidency(IDirect3DResource9** pResourceArray,UINT32 NumResources)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CheckResourceResidency" << std::endl;
#endif

	return m_pDeviceEx->CheckResourceResidency(pResourceArray, NumResources);
}

/**
* Base SetMaximumFrameLatency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetMaximumFrameLatency(UINT MaxLatency)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "SetMaximumFrameLatency" << std::endl;
#endif

	return m_pDeviceEx->SetMaximumFrameLatency(MaxLatency);
}

/**
* Base GetMaximumFrameLatency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetMaximumFrameLatency(UINT* pMaxLatency)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetMaximumFrameLatency" << std::endl;
#endif

	return m_pDeviceEx->GetMaximumFrameLatency(pMaxLatency);
}

/**
* Base CheckDeviceState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CheckDeviceState(HWND hDestinationWindow)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CheckDeviceState" << std::endl;
#endif

	return m_pDeviceEx->CheckDeviceState(hDestinationWindow);
}

/**
* Base CreateRenderTargetEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateRenderTargetEx(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateRenderTargetEx" << std::endl;
#endif

	return m_pDeviceEx->CreateRenderTargetEx(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
}

/**
* Base CreateOffscreenPlainSurfaceEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateOffscreenPlainSurfaceEx" << std::endl;
#endif

	return m_pDeviceEx->CreateOffscreenPlainSurfaceEx(Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
}

/**
* Base CreateDepthStencilSurfaceEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateDepthStencilSurfaceEx(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "CreateDepthStencilSurfaceEx" << std::endl;
#endif

	return m_pDeviceEx->CreateDepthStencilSurfaceEx(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
}

/**
* Base ResetEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ResetEx(D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX *pFullscreenDisplayMode)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "ResetEx" << std::endl;
#endif

	return m_pDeviceEx->ResetEx(pPresentationParameters, pFullscreenDisplayMode);
}

/**
* Base GetDisplayModeEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDisplayModeEx(UINT iSwapChain,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation)
{
#ifdef _EXPORT_LOGFILE
	m_logFile << "GetDisplayModeEx" << std::endl;
#endif

	return m_pDeviceEx->GetDisplayModeEx(iSwapChain, pMode, pRotation);
}

/**
* Returns the actual embedded device pointer.
***/
IDirect3DDevice9Ex* BaseDirect3DDevice9Ex::getActual()
{
	return m_pDeviceEx;
}