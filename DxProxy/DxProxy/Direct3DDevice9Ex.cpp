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
	LOG_CALL("QueryInterface");

	return m_pDeviceEx->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI BaseDirect3DDevice9Ex::AddRef()
{
	LOG_CALL("AddRef");

	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI BaseDirect3DDevice9Ex::Release()
{
	LOG_CALL("Release");

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
	LOG_CALL("TestCooperativeLevel");

	return m_pDeviceEx->TestCooperativeLevel();
}

/**
* Base GetAvailableTextureMem functionality.
***/
UINT WINAPI BaseDirect3DDevice9Ex::GetAvailableTextureMem()
{
	LOG_CALL("GetAvailableTextureMem");

	return m_pDeviceEx->GetAvailableTextureMem();
}

/**
* Base EvictManagedResources functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EvictManagedResources()
{
	LOG_CALL("EvictManagedResources");

	return m_pDeviceEx->EvictManagedResources();
}

/**
* Base GetDirect3D functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDirect3D(IDirect3D9** ppD3D9)
{
	LOG_CALL("GetDirect3D");

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
	LOG_CALL("GetDeviceCaps");

	return m_pDeviceEx->GetDeviceCaps(pCaps);
}

/**
* Base GetDisplayMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	LOG_CALL("GetDisplayMode");

	return m_pDeviceEx->GetDisplayMode(iSwapChain, pMode);
}

/**
* Base GetCreationParameters functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	LOG_CALL("GetCreationParameters");

	return m_pDeviceEx->GetCreationParameters(pParameters);
}

/**
* Base SetCursorProperties functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	LOG_CALL("SetCursorProperties");

	return m_pDeviceEx->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

/**
* Base SetCursorPosition functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::SetCursorPosition(int X,int Y,DWORD Flags)
{
	LOG_CALL("SetCursorPosition");

	return m_pDeviceEx->SetCursorPosition(X, Y, Flags);
}

/**
* Base ShowCursor functionality.
***/
BOOL WINAPI BaseDirect3DDevice9Ex::ShowCursor(BOOL bShow)
{
	LOG_CALL("ShowCursor");

	return m_pDeviceEx->ShowCursor(bShow);
}

/**
* Base CreateAdditionalSwapChain functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	LOG_CALL("CreateAdditionalSwapChain");

	return m_pDeviceEx->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

/**
* Base GetSwapChain functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	LOG_CALL("GetSwapChain");

	return m_pDeviceEx->GetSwapChain(iSwapChain, pSwapChain);
}

/**
* Base GetNumberOfSwapChains functionality.
***/
UINT WINAPI BaseDirect3DDevice9Ex::GetNumberOfSwapChains()
{
	LOG_CALL("GetNumberOfSwapChains");

	return m_pDeviceEx->GetNumberOfSwapChains();
}

/**
* Base Reset functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	LOG_CALL("Reset");

	return m_pDeviceEx->Reset(pPresentationParameters);
}

/**
* Base Present functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	LOG_CALL("Present");

	return m_pDeviceEx->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/**
* Base GetBackBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	LOG_CALL("GetBackBuffer");

	return m_pDeviceEx->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

/**
* Base GetRasterStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	LOG_CALL("GetRasterStatus");

	return m_pDeviceEx->GetRasterStatus(iSwapChain, pRasterStatus);
}

/**
* Base SetDialogBoxMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetDialogBoxMode(BOOL bEnableDialogs)
{
	LOG_CALL("SetDialogBoxMode");

	return m_pDeviceEx->SetDialogBoxMode(bEnableDialogs);
}

/**
* Base SetGammaRamp functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{	
	LOG_CALL("SetGammaRamp");

	return m_pDeviceEx->SetGammaRamp(iSwapChain, Flags, pRamp);
}

/**
* Base GetGammaRamp functionality.
***/
void WINAPI BaseDirect3DDevice9Ex::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	LOG_CALL("GetGammaRamp");

	return m_pDeviceEx->GetGammaRamp(iSwapChain, pRamp);
}

/**
* Base CreateTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateTexture");

	return m_pDeviceEx->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

/**
* Base CreateVolumeTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateVolumeTexture");

	return m_pDeviceEx->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

/**
* Base CreateCubeTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateCubeTexture");

	return m_pDeviceEx->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

/**
* Base CreateVertexBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateVertexBuffer");

	return m_pDeviceEx->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

/**
* Base CreateIndexBuffer functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateIndexBuffer");

	return m_pDeviceEx->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

/**
* Base CreateRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateRenderTarget");

	return m_pDeviceEx->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

/**
* Base CreateDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateDepthStencilSurface");

	return m_pDeviceEx->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

/**
* Base UpdateSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	LOG_CALL("UpdateSurface");

	return m_pDeviceEx->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

/**
* Base UpdateTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	LOG_CALL("UpdateTexture");

	return m_pDeviceEx->UpdateTexture(pSourceTexture, pDestinationTexture);
}

/**
* Base GetRenderTargetData functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	LOG_CALL("GetRenderTargetData");

	return m_pDeviceEx->GetRenderTargetData(pRenderTarget, pDestSurface);
}

/**
* Base GetFrontBufferData functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	LOG_CALL("GetFrontBufferData");

	return m_pDeviceEx->GetFrontBufferData(iSwapChain, pDestSurface);
}

/**
* Base StretchRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	LOG_CALL("StretchRect");

	return m_pDeviceEx->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

/**
* Base ColorFill functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	LOG_CALL("ColorFill");

	return m_pDeviceEx->ColorFill(pSurface, pRect, color);
}

/**
* Base CreateOffscreenPlainSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	LOG_CALL("CreateOffscreenPlainSurface");

	return m_pDeviceEx->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

/**
* Base SetRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	LOG_CALL("SetRenderTarget");

	return m_pDeviceEx->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

/**
* Base GetRenderTarget functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	LOG_CALL("GetRenderTarget");
	return m_pDeviceEx->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

/**
* Base SetDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	LOG_CALL("SetDepthStencilSurface");

	return m_pDeviceEx->SetDepthStencilSurface(pNewZStencil);
}

/**
* Base GetDepthStencilSurface functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	LOG_CALL("GetDepthStencilSurface");

	return m_pDeviceEx->GetDepthStencilSurface(ppZStencilSurface);
}

/**
* Base BeginScene functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::BeginScene()
{
	LOG_CALL("BeginScene");

	return m_pDeviceEx->BeginScene();
}

/**
* Base EndScene functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EndScene()
{
	LOG_CALL("EndScene");

	return m_pDeviceEx->EndScene();
}

/**
* Base Clear functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	LOG_CALL("Clear");

	return m_pDeviceEx->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

/**
* Base SetTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	LOG_CALL("SetTransform");

	return m_pDeviceEx->SetTransform(State, pMatrix);
}

/**
* Base GetTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	LOG_CALL("GetTransform");

	return m_pDeviceEx->GetTransform(State, pMatrix);
}

/**
* Base MultiplyTransform functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	LOG_CALL("MultiplyTransform");

	return m_pDeviceEx->MultiplyTransform(State, pMatrix);
}

/**
* Base SetViewport functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	LOG_CALL("SetViewport");

	return m_pDeviceEx->SetViewport(pViewport);
}

/**
* Base GetViewport functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetViewport(D3DVIEWPORT9* pViewport)
{
	LOG_CALL("GetViewport");

	return m_pDeviceEx->GetViewport(pViewport);
}

/**
* Base SetMaterial functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	LOG_CALL("SetMaterial");

	return m_pDeviceEx->SetMaterial(pMaterial);
}

/**
* Base GetMaterial functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetMaterial(D3DMATERIAL9* pMaterial)
{
	LOG_CALL("GetMaterial");

	return m_pDeviceEx->GetMaterial(pMaterial);
}

/**
* Base SetLight functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
	LOG_CALL("SetLight");

	return m_pDeviceEx->SetLight(Index, pLight);
}

/**
* Base GetLight functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
	LOG_CALL("GetLight");

	return m_pDeviceEx->GetLight(Index, pLight);
}

/**
* Base LightEnable functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::LightEnable(DWORD Index,BOOL Enable)
{
	LOG_CALL("LightEnable");

	return m_pDeviceEx->LightEnable(Index, Enable);
}

/**
* Base GetLightEnable functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	LOG_CALL("GetLightEnable");

	return m_pDeviceEx->GetLightEnable(Index, pEnable);
}

/**
* Base SetClipPlane functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetClipPlane(DWORD Index,CONST float* pPlane)
{
	LOG_CALL("SetClipPlane");

	return m_pDeviceEx->SetClipPlane(Index, pPlane);
}

/**
* Base GetClipPlane functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetClipPlane(DWORD Index,float* pPlane)
{
	LOG_CALL("GetClipPlane");

	return m_pDeviceEx->GetClipPlane(Index, pPlane);
}

/**
* Base SetRenderState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	LOG_CALL("SetRenderState");

	return m_pDeviceEx->SetRenderState(State, Value);
}

/**
* Base GetRenderState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
	LOG_CALL("GetRenderState");

	return m_pDeviceEx->GetRenderState(State, pValue);
}

/**
* Base CreateStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	LOG_CALL("CreateStateBlock");

	return m_pDeviceEx->CreateStateBlock(Type, ppSB);
}

/**
* Base BeginStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::BeginStateBlock()
{
	LOG_CALL("BeginStateBlock");

	return m_pDeviceEx->BeginStateBlock();
}

/**
* Base EndStateBlock functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	LOG_CALL("EndStateBlock");

	return m_pDeviceEx->EndStateBlock(ppSB);
}

/**
* Base SetClipStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	LOG_CALL("SetClipStatus");

	return m_pDeviceEx->SetClipStatus(pClipStatus);
}

/**
* Base GetClipStatus functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	LOG_CALL("GetClipStatus");

	return m_pDeviceEx->GetClipStatus(pClipStatus);
}

/**
* Base GetTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	LOG_CALL("GetTexture");

	return m_pDeviceEx->GetTexture(Stage, ppTexture);
}

/**
* Base SetTexture functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	LOG_CALL("SetTexture");

	return m_pDeviceEx->SetTexture(Stage, pTexture);
}

/**
* Base GetTextureStageState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	LOG_CALL("GetTextureStageState");

	return m_pDeviceEx->GetTextureStageState(Stage, Type, pValue);
}

/**
* Base SetTextureStageState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	LOG_CALL("SetTextureStageState");

	return m_pDeviceEx->SetTextureStageState(Stage, Type, Value);
}

/**
* Base GetSamplerState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	LOG_CALL("GetSamplerState");

	return m_pDeviceEx->GetSamplerState(Sampler, Type, pValue);
}

/**
* Base SetSamplerState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	LOG_CALL("SetSamplerState");

	return m_pDeviceEx->SetSamplerState(Sampler, Type, Value);
}

/**
* Base ValidateDevice functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ValidateDevice(DWORD* pNumPasses)
{
	LOG_CALL("ValidateDevice");

	return m_pDeviceEx->ValidateDevice(pNumPasses);
}

/**
* Base SetPaletteEntries functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	LOG_CALL("SetPaletteEntries");

	return m_pDeviceEx->SetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base GetPaletteEntries functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	LOG_CALL("GetPaletteEntries");

	return m_pDeviceEx->GetPaletteEntries(PaletteNumber, pEntries);
}

/**
* Base SetCurrentTexturePalette functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetCurrentTexturePalette(UINT PaletteNumber)
{
	LOG_CALL("SetCurrentTexturePalette");

	return m_pDeviceEx->SetCurrentTexturePalette(PaletteNumber);
}

/**
* Base GetCurrentTexturePalette functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	LOG_CALL("GetCurrentTexturePalette");

	return m_pDeviceEx->GetCurrentTexturePalette(PaletteNumber);
}

/**
* Base SetScissorRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetScissorRect(CONST RECT* pRect)
{
	LOG_CALL("SetScissorRect");

	return m_pDeviceEx->SetScissorRect(pRect);
}

/**
* Base GetScissorRect functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetScissorRect(RECT* pRect)
{
	LOG_CALL("GetScissorRect");

	return m_pDeviceEx->GetScissorRect(pRect);
}

/**
* Base SetSoftwareVertexProcessing functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	LOG_CALL("SetSoftwareVertexProcessing");

	return m_pDeviceEx->SetSoftwareVertexProcessing(bSoftware);
}

/**
* Base GetSoftwareVertexProcessing functionality.
***/
BOOL WINAPI BaseDirect3DDevice9Ex::GetSoftwareVertexProcessing()
{
	LOG_CALL("GetSoftwareVertexProcessing");

	return m_pDeviceEx->GetSoftwareVertexProcessing();
}

/**
* Base SetNPatchMode functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetNPatchMode(float nSegments)
{
	LOG_CALL("SetNPatchMode");

	return m_pDeviceEx->SetNPatchMode(nSegments);
}

/**
* Base GetNPatchMode functionality.
***/
float WINAPI BaseDirect3DDevice9Ex::GetNPatchMode()
{
	LOG_CALL("GetNPatchMode");

	return m_pDeviceEx->GetNPatchMode();
}

/**
* Base DrawPrimitive functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	LOG_CALL("DrawPrimitive");

	return m_pDeviceEx->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

/**
* Base DrawIndexedPrimitive functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	LOG_CALL("DrawIndexedPrimitive");

	return m_pDeviceEx->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

/**
* Base DrawPrimitiveUP functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	LOG_CALL("DrawPrimitiveUP");

	return m_pDeviceEx->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Base DrawIndexedPrimitiveUP functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	LOG_CALL("DrawIndexedPrimitiveUP");

	return m_pDeviceEx->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Base ProcessVertices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	LOG_CALL("ProcessVertices");

	return m_pDeviceEx->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

/**
* Base CreateVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	LOG_CALL("CreateVertexDeclaration");

	return m_pDeviceEx->CreateVertexDeclaration(pVertexElements, ppDecl);
}

/**
* Base SetVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	LOG_CALL("SetVertexDeclaration");

	return m_pDeviceEx->SetVertexDeclaration(pDecl);
}

/**
* Base GetVertexDeclaration functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	LOG_CALL("GetVertexDeclaration");

	return m_pDeviceEx->GetVertexDeclaration(ppDecl);
}

/**
* Base SetFVF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetFVF(DWORD FVF)
{
	LOG_CALL("SetFVF");

	return m_pDeviceEx->SetFVF(FVF);
}

/**
* Base GetFVF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetFVF(DWORD* pFVF)
{
	LOG_CALL("GetFVF");

	return m_pDeviceEx->GetFVF(pFVF);
}

/**
* Base CreateVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	LOG_CALL("CreateVertexShader");

	return m_pDeviceEx->CreateVertexShader(pFunction, ppShader);
}

/**
* Base SetVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	LOG_CALL("SetVertexShader");

	return m_pDeviceEx->SetVertexShader(pShader);
}

/**
* Base GetVertexShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	LOG_CALL("GetVertexShader");

	return m_pDeviceEx->GetVertexShader(ppShader);
}

/**
* Base SetVertexShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	LOG_CALL("SetVertexShaderConstantF");

	return m_pDeviceEx->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base GetVertexShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	LOG_CALL("GetVertexShaderConstantF");

	return m_pDeviceEx->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base SetVertexShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	LOG_CALL("SetVertexShaderConstantI");

	return m_pDeviceEx->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base GetVertexShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	LOG_CALL("GetVertexShaderConstantI");

	return m_pDeviceEx->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base SetVertexShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	LOG_CALL("SetVertexShaderConstantB");

	return m_pDeviceEx->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base GetVertexShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	LOG_CALL("GetVertexShaderConstantB");

	return m_pDeviceEx->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base SetStreamSource functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	LOG_CALL("SetStreamSource");

	return m_pDeviceEx->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

/**
* Base GetStreamSource functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
	LOG_CALL("GetStreamSource");

	return m_pDeviceEx->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

/**
* Base SetStreamSourceFreq functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetStreamSourceFreq(UINT StreamNumber,UINT Setting)
{
	LOG_CALL("SetStreamSourceFreq");

	return m_pDeviceEx->SetStreamSourceFreq(StreamNumber, Setting);
}

/**
* Base GetStreamSourceFreq functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting)
{
	LOG_CALL("GetStreamSourceFreq");

	return m_pDeviceEx->GetStreamSourceFreq(StreamNumber, pSetting);
}

/**
* Base SetIndices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	LOG_CALL("SetIndices");

	return m_pDeviceEx->SetIndices(pIndexData);
}

/**
* Base GetIndices functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	LOG_CALL("GetIndices");

	return m_pDeviceEx->GetIndices(ppIndexData);
}

/**
* Base CreatePixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	LOG_CALL("CreatePixelShader");

	return m_pDeviceEx->CreatePixelShader(pFunction, ppShader);
}

/**
* Base SetPixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	LOG_CALL("SetPixelShader");

	return m_pDeviceEx->SetPixelShader(pShader);
}

/**
* Base GetPixelShader functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	LOG_CALL("GetPixelShader");

	return m_pDeviceEx->GetPixelShader(ppShader);
}

/**
* Base SetPixelShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	LOG_CALL("SetPixelShaderConstantF");

	return m_pDeviceEx->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base GetPixelShaderConstantF functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	LOG_CALL("GetPixelShaderConstantF");

	return m_pDeviceEx->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
* Base SetPixelShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	LOG_CALL("SetPixelShaderConstantI");

	return m_pDeviceEx->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base GetPixelShaderConstantI functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	LOG_CALL("GetPixelShaderConstantI");

	return m_pDeviceEx->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

/**
* Base SetPixelShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	LOG_CALL("SetPixelShaderConstantB");

	return m_pDeviceEx->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base GetPixelShaderConstantB functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	LOG_CALL("GetPixelShaderConstantB");

	return m_pDeviceEx->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

/**
* Base DrawRectPatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	LOG_CALL("DrawRectPatch");
	return m_pDeviceEx->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

/**
* Base DrawTriPatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	LOG_CALL("DrawTriPatch");

	return m_pDeviceEx->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

/**
* Base DeletePatch functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::DeletePatch(UINT Handle)
{
	LOG_CALL("DeletePatch");

	return m_pDeviceEx->DeletePatch(Handle);
}

/**
* Base CreateQuery functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	LOG_CALL("CreateQuery");

	return m_pDeviceEx->CreateQuery(Type, ppQuery);
}

/**
* Base SetConvolutionMonoKernel functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetConvolutionMonoKernel(UINT width,UINT height,float* rows,float* columns)
{
	LOG_CALL("SetConvolutionMonoKernel");

	return m_pDeviceEx->SetConvolutionMonoKernel(width, height, rows, columns);
}

/**
* Base ComposeRects functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ComposeRects(IDirect3DSurface9* pSrc,IDirect3DSurface9* pDst,IDirect3DVertexBuffer9* pSrcRectDescs,UINT NumRects,IDirect3DVertexBuffer9* pDstRectDescs,D3DCOMPOSERECTSOP Operation,int Xoffset,int Yoffset)
{
	LOG_CALL("ComposeRects");

	return m_pDeviceEx->ComposeRects(pSrc, pDst, pSrcRectDescs, NumRects, pDstRectDescs, Operation, Xoffset, Yoffset);
}

/**
* Base PresentEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::PresentEx(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
	LOG_CALL("PresentEx");

	return m_pDeviceEx->PresentEx(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

/**
* Base GetGPUThreadPriority functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetGPUThreadPriority(INT* pPriority)
{
	LOG_CALL("GetGPUThreadPriority");

	return m_pDeviceEx->GetGPUThreadPriority(pPriority);
}

/**
* Base SetGPUThreadPriority functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetGPUThreadPriority(INT Priority)
{
	LOG_CALL("SetGPUThreadPriority");

	return m_pDeviceEx->SetGPUThreadPriority(Priority);
}

/**
* Base WaitForVBlank functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::WaitForVBlank(UINT iSwapChain)
{
	LOG_CALL("WaitForVBlank");

	return m_pDeviceEx->WaitForVBlank(iSwapChain);
}

/**
* Base CheckResourceResidency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CheckResourceResidency(IDirect3DResource9** pResourceArray,UINT32 NumResources)
{
	LOG_CALL("CheckResourceResidency");

	return m_pDeviceEx->CheckResourceResidency(pResourceArray, NumResources);
}

/**
* Base SetMaximumFrameLatency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::SetMaximumFrameLatency(UINT MaxLatency)
{
	LOG_CALL("SetMaximumFrameLatency");

	return m_pDeviceEx->SetMaximumFrameLatency(MaxLatency);
}

/**
* Base GetMaximumFrameLatency functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetMaximumFrameLatency(UINT* pMaxLatency)
{
	LOG_CALL("GetMaximumFrameLatency");

	return m_pDeviceEx->GetMaximumFrameLatency(pMaxLatency);
}

/**
* Base CheckDeviceState functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CheckDeviceState(HWND hDestinationWindow)
{
	LOG_CALL("CheckDeviceState");

	return m_pDeviceEx->CheckDeviceState(hDestinationWindow);
}

/**
* Base CreateRenderTargetEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateRenderTargetEx(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	LOG_CALL("CreateRenderTargetEx");

	return m_pDeviceEx->CreateRenderTargetEx(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle, Usage);
}

/**
* Base CreateOffscreenPlainSurfaceEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	LOG_CALL("CreateOffscreenPlainSurfaceEx");

	return m_pDeviceEx->CreateOffscreenPlainSurfaceEx(Width, Height, Format, Pool, ppSurface, pSharedHandle, Usage);
}

/**
* Base CreateDepthStencilSurfaceEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::CreateDepthStencilSurfaceEx(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage)
{
	LOG_CALL("CreateDepthStencilSurfaceEx");

	return m_pDeviceEx->CreateDepthStencilSurfaceEx(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle, Usage);
}

/**
* Base ResetEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::ResetEx(D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX *pFullscreenDisplayMode)
{
	LOG_CALL("ResetEx");

	return m_pDeviceEx->ResetEx(pPresentationParameters, pFullscreenDisplayMode);
}

/**
* Base GetDisplayModeEx functionality.
***/
HRESULT WINAPI BaseDirect3DDevice9Ex::GetDisplayModeEx(UINT iSwapChain,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation)
{
	LOG_CALL("GetDisplayModeEx");

	return m_pDeviceEx->GetDisplayModeEx(iSwapChain, pMode, pRotation);
}

/**
* Returns the actual embedded device pointer.
***/
IDirect3DDevice9Ex* BaseDirect3DDevice9Ex::getActual()
{
	return m_pDeviceEx;
}