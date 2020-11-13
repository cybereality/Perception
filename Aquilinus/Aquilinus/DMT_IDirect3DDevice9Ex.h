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
#include"AQU_Detour.h"

#ifndef DMT_IDIRECT3DDEVICE9EX_TABLE
#define DMT_IDIRECT3DDEVICE9EX_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_QueryInterface(IDirect3DDevice9Ex* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DDevice9Ex->QueryInterface(pDevice, riid, ppvObj);}
ULONG   WINAPI D3D9_IDirect3DDevice9Ex_AddRef(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->AddRef(pDevice);}
ULONG   WINAPI D3D9_IDirect3DDevice9Ex_Release(IDirect3DDevice9Ex* pDevice){ return pDCL_IDirect3DDevice9Ex->Release(pDevice);}

/*** IDirect3DDevice9Ex methods ***/
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_TestCooperativeLevel(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->TestCooperativeLevel(pDevice);}
UINT    WINAPI D3D9_IDirect3DDevice9Ex_GetAvailableTextureMem(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->GetAvailableTextureMem(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_EvictManagedResources(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->EvictManagedResources(pDevice);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetDirect3D(IDirect3DDevice9Ex* pDevice, IDirect3D9** ppD3D9){return pDCL_IDirect3DDevice9Ex->GetDirect3D(pDevice, ppD3D9);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetDeviceCaps(IDirect3DDevice9Ex* pDevice, D3DCAPS9* pCaps){ return pDCL_IDirect3DDevice9Ex->GetDeviceCaps(pDevice, pCaps);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetDisplayMode(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,D3DDISPLAYMODE* pMode){ return pDCL_IDirect3DDevice9Ex->GetDisplayMode(pDevice, iSwapChain, pMode);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetCreationParameters(IDirect3DDevice9Ex* pDevice, D3DDEVICE_CREATION_PARAMETERS *pParameters){ return pDCL_IDirect3DDevice9Ex->GetCreationParameters(pDevice, pParameters);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetCursorProperties(IDirect3DDevice9Ex* pDevice, UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap){ return pDCL_IDirect3DDevice9Ex->SetCursorProperties(pDevice, XHotSpot, YHotSpot, pCursorBitmap);} 
void    WINAPI D3D9_IDirect3DDevice9Ex_SetCursorPosition(IDirect3DDevice9Ex* pDevice, int X,int Y,DWORD Flags){ return pDCL_IDirect3DDevice9Ex->SetCursorPosition(pDevice, X, Y, Flags);} 
BOOL    WINAPI D3D9_IDirect3DDevice9Ex_ShowCursor(IDirect3DDevice9Ex* pDevice, BOOL bShow){ return pDCL_IDirect3DDevice9Ex->ShowCursor(pDevice, bShow);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateAdditionalSwapChain(IDirect3DDevice9Ex* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain){ return pDCL_IDirect3DDevice9Ex->CreateAdditionalSwapChain(pDevice, pPresentationParameters, pSwapChain);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetSwapChain(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,IDirect3DSwapChain9** pSwapChain){ return pDCL_IDirect3DDevice9Ex->GetSwapChain(pDevice, iSwapChain, pSwapChain);} 
UINT    WINAPI D3D9_IDirect3DDevice9Ex_GetNumberOfSwapChains(IDirect3DDevice9Ex* pDevice){ return pDCL_IDirect3DDevice9Ex->GetNumberOfSwapChains(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_Reset(IDirect3DDevice9Ex* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){ return pDCL_IDirect3DDevice9Ex->Reset(pDevice, pPresentationParameters);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_Present(IDirect3DDevice9Ex* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){return pDCL_IDirect3DDevice9Ex->Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetBackBuffer(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer){return pDCL_IDirect3DDevice9Ex->GetBackBuffer(pDevice, iSwapChain, iBackBuffer, Type, ppBackBuffer);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetRasterStatus(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus){ return pDCL_IDirect3DDevice9Ex->GetRasterStatus(pDevice, iSwapChain, pRasterStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetDialogBoxMode(IDirect3DDevice9Ex* pDevice, BOOL bEnableDialogs){ return pDCL_IDirect3DDevice9Ex->SetDialogBoxMode(pDevice, bEnableDialogs);} 
void    WINAPI D3D9_IDirect3DDevice9Ex_SetGammaRamp(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp){ return pDCL_IDirect3DDevice9Ex->SetGammaRamp(pDevice, iSwapChain, Flags, pRamp);} 
void    WINAPI D3D9_IDirect3DDevice9Ex_GetGammaRamp(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,D3DGAMMARAMP* pRamp){ return pDCL_IDirect3DDevice9Ex->GetGammaRamp(pDevice, iSwapChain, pRamp);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateTexture(IDirect3DDevice9Ex* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateTexture(pDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateVolumeTexture(IDirect3DDevice9Ex* pDevice, UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateVolumeTexture(pDevice, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateCubeTexture(IDirect3DDevice9Ex* pDevice, UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateCubeTexture(pDevice, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateVertexBuffer(IDirect3DDevice9Ex* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateVertexBuffer(pDevice, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateIndexBuffer(IDirect3DDevice9Ex* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle){return pDCL_IDirect3DDevice9Ex->CreateIndexBuffer(pDevice, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateRenderTarget(IDirect3DDevice9Ex* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateRenderTarget(pDevice, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateDepthStencilSurface(IDirect3DDevice9Ex* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateDepthStencilSurface(pDevice, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_UpdateSurface(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint){ return pDCL_IDirect3DDevice9Ex->UpdateSurface(pDevice, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_UpdateTexture(IDirect3DDevice9Ex* pDevice, IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture){ return pDCL_IDirect3DDevice9Ex->UpdateTexture(pDevice, pSourceTexture, pDestinationTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetRenderTargetData(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface){ return pDCL_IDirect3DDevice9Ex->GetRenderTargetData(pDevice, pRenderTarget, pDestSurface);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetFrontBufferData(IDirect3DDevice9Ex* pDevice, UINT iSwapChain,IDirect3DSurface9* pDestSurface){ return pDCL_IDirect3DDevice9Ex->GetFrontBufferData(pDevice, iSwapChain, pDestSurface);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_StretchRect(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter){return pDCL_IDirect3DDevice9Ex->StretchRect(pDevice, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);}
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_ColorFill(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color){ return pDCL_IDirect3DDevice9Ex->ColorFill(pDevice, pSurface, pRect, color);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateOffscreenPlainSurface(IDirect3DDevice9Ex* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9Ex->CreateOffscreenPlainSurface(pDevice, Width, Height, Format, Pool, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetRenderTarget(IDirect3DDevice9Ex* pDevice, DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget){ return pDCL_IDirect3DDevice9Ex->SetRenderTarget(pDevice, RenderTargetIndex, pRenderTarget);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetRenderTarget(IDirect3DDevice9Ex* pDevice, DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget){ return pDCL_IDirect3DDevice9Ex->GetRenderTarget(pDevice, RenderTargetIndex, ppRenderTarget);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetDepthStencilSurface(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9* pNewZStencil){return pDCL_IDirect3DDevice9Ex->SetDepthStencilSurface(pDevice, pNewZStencil);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetDepthStencilSurface(IDirect3DDevice9Ex* pDevice, IDirect3DSurface9** ppZStencilSurface){return pDCL_IDirect3DDevice9Ex->GetDepthStencilSurface(pDevice, ppZStencilSurface);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_BeginScene(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->BeginScene(pDevice);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_EndScene(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->EndScene(pDevice);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_Clear(IDirect3DDevice9Ex* pDevice, DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil){return pDCL_IDirect3DDevice9Ex->Clear(pDevice, Count, pRects, Flags, Color, Z, Stencil);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetTransform(IDirect3DDevice9Ex* pDevice, D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9Ex->SetTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetTransform(IDirect3DDevice9Ex* pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9Ex->GetTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_MultiplyTransform(IDirect3DDevice9Ex* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9Ex->MultiplyTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetViewport(IDirect3DDevice9Ex* pDevice, CONST D3DVIEWPORT9* pViewport){return pDCL_IDirect3DDevice9Ex->SetViewport(pDevice, pViewport);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetViewport(IDirect3DDevice9Ex* pDevice, D3DVIEWPORT9* pViewport){return pDCL_IDirect3DDevice9Ex->GetViewport(pDevice, pViewport);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetMaterial(IDirect3DDevice9Ex* pDevice, CONST D3DMATERIAL9* pMaterial){return pDCL_IDirect3DDevice9Ex->SetMaterial(pDevice, pMaterial);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetMaterial(IDirect3DDevice9Ex* pDevice, D3DMATERIAL9* pMaterial){return pDCL_IDirect3DDevice9Ex->GetMaterial(pDevice, pMaterial);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetLight(IDirect3DDevice9Ex* pDevice, DWORD Index, CONST D3DLIGHT9* pLight){return pDCL_IDirect3DDevice9Ex->SetLight(pDevice, Index, pLight);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetLight(IDirect3DDevice9Ex* pDevice, DWORD Index, D3DLIGHT9* pLight){return pDCL_IDirect3DDevice9Ex->GetLight(pDevice, Index, pLight);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_LightEnable(IDirect3DDevice9Ex* pDevice, DWORD Index, BOOL Enable){return pDCL_IDirect3DDevice9Ex->LightEnable(pDevice, Index, Enable);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetLightEnable(IDirect3DDevice9Ex* pDevice, DWORD Index,BOOL* pEnable){return pDCL_IDirect3DDevice9Ex->GetLightEnable(pDevice, Index, pEnable);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetClipPlane(IDirect3DDevice9Ex* pDevice, DWORD Index,CONST float* pPlane){return pDCL_IDirect3DDevice9Ex->SetClipPlane(pDevice, Index, pPlane);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetClipPlane(IDirect3DDevice9Ex* pDevice, DWORD Index,float* pPlane){return pDCL_IDirect3DDevice9Ex->GetClipPlane(pDevice, Index, pPlane);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetRenderState(IDirect3DDevice9Ex* pDevice, D3DRENDERSTATETYPE State,DWORD Value){return pDCL_IDirect3DDevice9Ex->SetRenderState(pDevice, State, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetRenderState(IDirect3DDevice9Ex* pDevice, D3DRENDERSTATETYPE State,DWORD* pValue){return pDCL_IDirect3DDevice9Ex->GetRenderState(pDevice, State, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateStateBlock(IDirect3DDevice9Ex* pDevice, D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB){return pDCL_IDirect3DDevice9Ex->CreateStateBlock(pDevice, Type, ppSB);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_BeginStateBlock(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->BeginStateBlock(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_EndStateBlock(IDirect3DDevice9Ex* pDevice, IDirect3DStateBlock9** ppSB){return pDCL_IDirect3DDevice9Ex->EndStateBlock(pDevice, ppSB);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetClipStatus(IDirect3DDevice9Ex* pDevice, CONST D3DCLIPSTATUS9* pClipStatus){return pDCL_IDirect3DDevice9Ex->SetClipStatus(pDevice, pClipStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetClipStatus(IDirect3DDevice9Ex* pDevice, D3DCLIPSTATUS9* pClipStatus){return pDCL_IDirect3DDevice9Ex->GetClipStatus(pDevice, pClipStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetTexture(IDirect3DDevice9Ex* pDevice, DWORD Stage,IDirect3DBaseTexture9** ppTexture){return pDCL_IDirect3DDevice9Ex->GetTexture(pDevice, Stage, ppTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetTexture(IDirect3DDevice9Ex* pDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture){return pDCL_IDirect3DDevice9Ex->SetTexture(pDevice, Stage, pTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetTextureStageState(IDirect3DDevice9Ex* pDevice, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue){return pDCL_IDirect3DDevice9Ex->GetTextureStageState(pDevice, Stage, Type, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetTextureStageState(IDirect3DDevice9Ex* pDevice, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value){return pDCL_IDirect3DDevice9Ex->SetTextureStageState(pDevice, Stage, Type, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetSamplerState(IDirect3DDevice9Ex* pDevice, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue){return pDCL_IDirect3DDevice9Ex->GetSamplerState(pDevice, Sampler, Type, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetSamplerState(IDirect3DDevice9Ex* pDevice, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value){return pDCL_IDirect3DDevice9Ex->SetSamplerState(pDevice, Sampler, Type, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_ValidateDevice(IDirect3DDevice9Ex* pDevice, DWORD* pNumPasses){return pDCL_IDirect3DDevice9Ex->ValidateDevice(pDevice, pNumPasses);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetPaletteEntries(IDirect3DDevice9Ex* pDevice, UINT PaletteNumber,CONST PALETTEENTRY* pEntries){return pDCL_IDirect3DDevice9Ex->SetPaletteEntries(pDevice, PaletteNumber, pEntries);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetPaletteEntries(IDirect3DDevice9Ex* pDevice, UINT PaletteNumber,PALETTEENTRY* pEntries){return pDCL_IDirect3DDevice9Ex->GetPaletteEntries(pDevice, PaletteNumber, pEntries);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetCurrentTexturePalette(IDirect3DDevice9Ex* pDevice, UINT PaletteNumber){return pDCL_IDirect3DDevice9Ex->SetCurrentTexturePalette(pDevice, PaletteNumber);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetCurrentTexturePalette(IDirect3DDevice9Ex* pDevice, UINT *PaletteNumber){return pDCL_IDirect3DDevice9Ex->GetCurrentTexturePalette(pDevice, PaletteNumber);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetScissorRect(IDirect3DDevice9Ex* pDevice, CONST RECT* pRect){return pDCL_IDirect3DDevice9Ex->SetScissorRect(pDevice, pRect);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetScissorRect(IDirect3DDevice9Ex* pDevice, RECT* pRect){return pDCL_IDirect3DDevice9Ex->GetScissorRect(pDevice, pRect);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetSoftwareVertexProcessing(IDirect3DDevice9Ex* pDevice, BOOL bSoftware){return pDCL_IDirect3DDevice9Ex->SetSoftwareVertexProcessing(pDevice, bSoftware);} 
BOOL    WINAPI D3D9_IDirect3DDevice9Ex_GetSoftwareVertexProcessing(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->GetSoftwareVertexProcessing(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetNPatchMode(IDirect3DDevice9Ex* pDevice, float nSegments){return pDCL_IDirect3DDevice9Ex->SetNPatchMode(pDevice, nSegments);} 
float   WINAPI D3D9_IDirect3DDevice9Ex_GetNPatchMode(IDirect3DDevice9Ex* pDevice){return pDCL_IDirect3DDevice9Ex->GetNPatchMode(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawPrimitive(IDirect3DDevice9Ex* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){return pDCL_IDirect3DDevice9Ex->DrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitive(IDirect3DDevice9Ex* pDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount){return pDCL_IDirect3DDevice9Ex->DrawIndexedPrimitive(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawPrimitiveUP(IDirect3DDevice9Ex* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){return pDCL_IDirect3DDevice9Ex->DrawPrimitiveUP(pDevice, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP(IDirect3DDevice9Ex* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){return pDCL_IDirect3DDevice9Ex->DrawIndexedPrimitiveUP(pDevice, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_ProcessVertices(IDirect3DDevice9Ex* pDevice, UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags){return pDCL_IDirect3DDevice9Ex->ProcessVertices(pDevice, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateVertexDeclaration(IDirect3DDevice9Ex* pDevice, CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl){return pDCL_IDirect3DDevice9Ex->CreateVertexDeclaration(pDevice, pVertexElements, ppDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetVertexDeclaration(IDirect3DDevice9Ex* pDevice, IDirect3DVertexDeclaration9* pDecl){return pDCL_IDirect3DDevice9Ex->SetVertexDeclaration(pDevice, pDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetVertexDeclaration(IDirect3DDevice9Ex* pDevice, IDirect3DVertexDeclaration9** ppDecl){return pDCL_IDirect3DDevice9Ex->GetVertexDeclaration(pDevice, ppDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetFVF(IDirect3DDevice9Ex* pDevice, DWORD FVF){return pDCL_IDirect3DDevice9Ex->SetFVF(pDevice, FVF);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetFVF(IDirect3DDevice9Ex* pDevice, DWORD* pFVF){return pDCL_IDirect3DDevice9Ex->GetFVF(pDevice, pFVF);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateVertexShader(IDirect3DDevice9Ex* pDevice, CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader){return pDCL_IDirect3DDevice9Ex->CreateVertexShader(pDevice, pFunction, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetVertexShader(IDirect3DDevice9Ex* pDevice, IDirect3DVertexShader9* pShader){return pDCL_IDirect3DDevice9Ex->SetVertexShader(pDevice, pShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetVertexShader(IDirect3DDevice9Ex* pDevice, IDirect3DVertexShader9** ppShader){return pDCL_IDirect3DDevice9Ex->GetVertexShader(pDevice, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantF(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9Ex->SetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantF(IDirect3DDevice9Ex* pDevice, UINT StartRegister,float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9Ex->GetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantI(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9Ex->SetVertexShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantI(IDirect3DDevice9Ex* pDevice, UINT StartRegister,int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9Ex->GetVertexShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantB(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount){return pDCL_IDirect3DDevice9Ex->SetVertexShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantB(IDirect3DDevice9Ex* pDevice, UINT StartRegister,BOOL* pConstantData,UINT BoolCount){return pDCL_IDirect3DDevice9Ex->GetVertexShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetStreamSource(IDirect3DDevice9Ex* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride){return pDCL_IDirect3DDevice9Ex->SetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetStreamSource(IDirect3DDevice9Ex* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride){return pDCL_IDirect3DDevice9Ex->GetStreamSource(pDevice, StreamNumber, ppStreamData, pOffsetInBytes, pStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetStreamSourceFreq(IDirect3DDevice9Ex* pDevice, UINT StreamNumber,UINT Setting){return pDCL_IDirect3DDevice9Ex->SetStreamSourceFreq(pDevice, StreamNumber, Setting);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetStreamSourceFreq(IDirect3DDevice9Ex* pDevice, UINT StreamNumber,UINT* pSetting){return pDCL_IDirect3DDevice9Ex->GetStreamSourceFreq(pDevice, StreamNumber, pSetting);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetIndices(IDirect3DDevice9Ex* pDevice, IDirect3DIndexBuffer9* pIndexData){return pDCL_IDirect3DDevice9Ex->SetIndices(pDevice, pIndexData);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetIndices(IDirect3DDevice9Ex* pDevice, IDirect3DIndexBuffer9** ppIndexData){return pDCL_IDirect3DDevice9Ex->GetIndices(pDevice, ppIndexData);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreatePixelShader(IDirect3DDevice9Ex* pDevice, CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader){return pDCL_IDirect3DDevice9Ex->CreatePixelShader(pDevice, pFunction, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetPixelShader(IDirect3DDevice9Ex* pDevice, IDirect3DPixelShader9* pShader){return pDCL_IDirect3DDevice9Ex->SetPixelShader(pDevice, pShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetPixelShader(IDirect3DDevice9Ex* pDevice, IDirect3DPixelShader9** ppShader){return pDCL_IDirect3DDevice9Ex->GetPixelShader(pDevice, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantF(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9Ex->SetPixelShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantF(IDirect3DDevice9Ex* pDevice, UINT StartRegister,float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9Ex->GetPixelShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantI(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9Ex->SetPixelShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantI(IDirect3DDevice9Ex* pDevice, UINT StartRegister,int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9Ex->GetPixelShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantB(IDirect3DDevice9Ex* pDevice, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount){return pDCL_IDirect3DDevice9Ex->SetPixelShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantB(IDirect3DDevice9Ex* pDevice, UINT StartRegister,BOOL* pConstantData,UINT BoolCount){return pDCL_IDirect3DDevice9Ex->GetPixelShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawRectPatch(IDirect3DDevice9Ex* pDevice, UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo){return pDCL_IDirect3DDevice9Ex->DrawRectPatch(pDevice, Handle, pNumSegs, pRectPatchInfo);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DrawTriPatch(IDirect3DDevice9Ex* pDevice, UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo){return pDCL_IDirect3DDevice9Ex->DrawTriPatch(pDevice, Handle, pNumSegs, pTriPatchInfo);} 
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_DeletePatch(IDirect3DDevice9Ex* pDevice, UINT Handle){return pDCL_IDirect3DDevice9Ex->DeletePatch(pDevice, Handle);}  
HRESULT WINAPI D3D9_IDirect3DDevice9Ex_CreateQuery(IDirect3DDevice9Ex* pDevice, D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery){return pDCL_IDirect3DDevice9Ex->CreateQuery(pDevice, Type, ppQuery);} 

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D929_IDirect3DDevice9Ex_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::QueryInterface],                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_QueryInterface);
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::AddRef],                              (UINT_PTR)D3D9_IDirect3DDevice9Ex_AddRef);
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::Release],                             (UINT_PTR)D3D9_IDirect3DDevice9Ex_Release);

	/*** IDirect3DDevice9Ex super methods ***/
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_TestCooperativeLevel);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem],      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetAvailableTextureMem);       
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources],       (UINT_PTR)D3D9_IDirect3DDevice9Ex_EvictManagedResources);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDirect3D);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDeviceCaps);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDisplayMode);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters],       (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetCreationParameters);       
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties],         (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCursorProperties);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCursorPosition);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_ShowCursor);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain],   (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateAdditionalSwapChain);   
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSwapChain);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains],       (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetNumberOfSwapChains);       
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Reset],                       (UINT_PTR)D3D9_IDirect3DDevice9Ex_Reset);                       
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Present],                     (UINT_PTR)D3D9_IDirect3DDevice9Ex_Present);                     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetBackBuffer);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRasterStatus);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode],            (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetDialogBoxMode);            
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetGammaRamp);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetGammaRamp);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateTexture);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture],         (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVolumeTexture);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateCubeTexture);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer],          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexBuffer);          
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateIndexBuffer);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget],          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateRenderTarget);          
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface],   (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateDepthStencilSurface);   
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_UpdateSurface);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_UpdateTexture);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData],         (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderTargetData);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData],          (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetFrontBufferData);          
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_StretchRect);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill],                   (UINT_PTR)D3D9_IDirect3DDevice9Ex_ColorFill);                   
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface], (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateOffscreenPlainSurface); 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetRenderTarget);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderTarget);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface],      (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetDepthStencilSurface);      
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface],      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDepthStencilSurface);      
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_BeginScene);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndScene],                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_EndScene);                    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Clear],                       (UINT_PTR)D3D9_IDirect3DDevice9Ex_Clear);                       
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTransform);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTransform);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_MultiplyTransform);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetViewport);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetViewport);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetMaterial);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetMaterial);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetLight],                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetLight);                    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLight],                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetLight);                    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_LightEnable);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetLightEnable);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetClipPlane);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetClipPlane);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetRenderState);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderState);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock],            (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateStateBlock);            
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_BeginStateBlock);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_EndStateBlock);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetClipStatus);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetClipStatus);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTexture);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTexture);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTextureStageState);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTextureStageState);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSamplerState);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetSamplerState);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_ValidateDevice);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPaletteEntries);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPaletteEntries);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCurrentTexturePalette);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetCurrentTexturePalette);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetScissorRect);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetScissorRect);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing], (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetSoftwareVertexProcessing); 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing], (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSoftwareVertexProcessing); 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetNPatchMode);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetNPatchMode);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawPrimitive);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitive);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawPrimitiveUP);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP],      (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP);      
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_ProcessVertices);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexDeclaration);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexDeclaration);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration],        (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexDeclaration);        
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF],                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetFVF);                      
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF],                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetFVF);                      
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader],          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexShader);          
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShader);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShader);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantF);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantF);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantI);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantI);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantB);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB],    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantB);    
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetStreamSource);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource],             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetStreamSource);             
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq],         (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetStreamSourceFreq);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq],         (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetStreamSourceFreq);         
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetIndices);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices],                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetIndices);                  
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader],           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreatePixelShader);           
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShader);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader],              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShader);              
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantF);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantF);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantI);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantI);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantB);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB],     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantB);     
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch],               (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawRectPatch);               
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch],                (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawTriPatch);                
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_DeletePatch);                 
	OverrideVTable(&D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery],                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateQuery);                 
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D929_IDirect3DDevice9Ex_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::QueryInterface] =                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_QueryInterface;
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::AddRef] =                              (UINT_PTR)D3D9_IDirect3DDevice9Ex_AddRef;
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::Release] =                             (UINT_PTR)D3D9_IDirect3DDevice9Ex_Release;

	/*** IDirect3DDevice9Ex super methods ***/
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_TestCooperativeLevel;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem] =      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetAvailableTextureMem;       
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources] =       (UINT_PTR)D3D9_IDirect3DDevice9Ex_EvictManagedResources;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDirect3D;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDeviceCaps;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDisplayMode;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters] =       (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetCreationParameters;       
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties] =         (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCursorProperties;         
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCursorPosition;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_ShowCursor;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain] =   (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateAdditionalSwapChain;   
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSwapChain;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains] =       (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetNumberOfSwapChains;       
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Reset] =                       (UINT_PTR)D3D9_IDirect3DDevice9Ex_Reset;                       
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Present] =                     (UINT_PTR)D3D9_IDirect3DDevice9Ex_Present;                     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetBackBuffer;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRasterStatus;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode] =            (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetDialogBoxMode;            
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetGammaRamp;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetGammaRamp;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateTexture;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture] =         (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVolumeTexture;         
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateCubeTexture;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer] =          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexBuffer;          
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateIndexBuffer;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget] =          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateRenderTarget;          
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface] =   (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateDepthStencilSurface;   
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_UpdateSurface;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_UpdateTexture;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData] =         (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderTargetData;         
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData] =          (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetFrontBufferData;          
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_StretchRect;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill] =                   (UINT_PTR)D3D9_IDirect3DDevice9Ex_ColorFill;                   
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface] = (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateOffscreenPlainSurface; 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetRenderTarget;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderTarget;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface] =      (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetDepthStencilSurface;      
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface] =      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetDepthStencilSurface;      
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_BeginScene;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndScene] =                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_EndScene;                    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Clear] =                       (UINT_PTR)D3D9_IDirect3DDevice9Ex_Clear;                       
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTransform;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTransform;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_MultiplyTransform;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetViewport;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetViewport;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetMaterial;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetMaterial;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetLight] =                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetLight;                    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLight] =                    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetLight;                    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_LightEnable;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetLightEnable;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetClipPlane;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetClipPlane;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetRenderState;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetRenderState;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock] =            (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateStateBlock;            
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_BeginStateBlock;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_EndStateBlock;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetClipStatus;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetClipStatus;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTexture;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTexture;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetTextureStageState;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetTextureStageState;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSamplerState;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetSamplerState;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_ValidateDevice;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPaletteEntries;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPaletteEntries;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetCurrentTexturePalette;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetCurrentTexturePalette;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetScissorRect;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetScissorRect;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing] = (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetSoftwareVertexProcessing; 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing] = (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetSoftwareVertexProcessing; 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetNPatchMode;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetNPatchMode;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawPrimitive;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitive;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawPrimitiveUP;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP] =      (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP;      
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_ProcessVertices;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexDeclaration;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexDeclaration;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration] =        (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexDeclaration;        
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF] =                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetFVF;                      
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF] =                      (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetFVF;                      
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader] =          (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateVertexShader;          
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShader;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShader;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantF;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantF;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantI;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantI;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantB;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB] =    (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantB;    
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetStreamSource;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource] =             (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetStreamSource;             
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq] =         (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetStreamSourceFreq;         
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq] =         (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetStreamSourceFreq;         
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetIndices;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices] =                  (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetIndices;                  
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader] =           (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreatePixelShader;           
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShader;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader] =              (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShader;              
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantF;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantF;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantI;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantI;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantB;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB] =     (UINT_PTR)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantB;     
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch] =               (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawRectPatch;               
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch] =                (UINT_PTR)D3D9_IDirect3DDevice9Ex_DrawTriPatch;                
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_DeletePatch;                 
	anD3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery] =                 (UINT_PTR)D3D9_IDirect3DDevice9Ex_CreateQuery;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D929_IDirect3DDevice9Ex_VMTable_by_Array()
{
	OverrideFullVTable(&D3D929_IDirect3DDevice9Ex_VMTable[0],
		(void*)&anD3D929_IDirect3DDevice9Ex_VMTable[0],
		(D3D929_IDIRECT3DDEVICE9EX_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D929_IDirect3DDevice9Ex_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::QueryInterface],                      (PBYTE)D3D9_IDirect3DDevice9Ex_QueryInterface,              JMP32_SZ));
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::AddRef],                              (PBYTE)D3D9_IDirect3DDevice9Ex_AddRef,                      JMP32_SZ));
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IUNKNOWN::Release],                             (PBYTE)D3D9_IDirect3DDevice9Ex_Release,                     JMP32_SZ));

	/*** IDirect3DDevice9Ex super methods ***/
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::TestCooperativeLevel,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel],        (PBYTE)D3D9_IDirect3DDevice9Ex_TestCooperativeLevel,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem,      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem],      (PBYTE)D3D9_IDirect3DDevice9Ex_GetAvailableTextureMem,      JMP32_SZ));      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EvictManagedResources,       (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources],       (PBYTE)D3D9_IDirect3DDevice9Ex_EvictManagedResources,       JMP32_SZ));       
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDirect3D,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D],                 (PBYTE)D3D9_IDirect3DDevice9Ex_GetDirect3D,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDeviceCaps,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps],               (PBYTE)D3D9_IDirect3DDevice9Ex_GetDeviceCaps,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDisplayMode,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode],              (PBYTE)D3D9_IDirect3DDevice9Ex_GetDisplayMode,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetCreationParameters,       (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters],       (PBYTE)D3D9_IDirect3DDevice9Ex_GetCreationParameters,       JMP32_SZ));       
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCursorProperties,         (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties],         (PBYTE)D3D9_IDirect3DDevice9Ex_SetCursorProperties,         JMP32_SZ));         
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCursorPosition,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition],           (PBYTE)D3D9_IDirect3DDevice9Ex_SetCursorPosition,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ShowCursor,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor],                  (PBYTE)D3D9_IDirect3DDevice9Ex_ShowCursor,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain,   (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain],   (PBYTE)D3D9_IDirect3DDevice9Ex_CreateAdditionalSwapChain,   JMP32_SZ));   
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSwapChain,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain],                (PBYTE)D3D9_IDirect3DDevice9Ex_GetSwapChain,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains,       (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains],       (PBYTE)D3D9_IDirect3DDevice9Ex_GetNumberOfSwapChains,       JMP32_SZ));       
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Reset,                       (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Reset],                       (PBYTE)D3D9_IDirect3DDevice9Ex_Reset,                       JMP32_SZ));                       
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Present,                     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Present],                     (PBYTE)D3D9_IDirect3DDevice9Ex_Present,                     JMP32_SZ));                     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetBackBuffer,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer],               (PBYTE)D3D9_IDirect3DDevice9Ex_GetBackBuffer,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRasterStatus,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus],             (PBYTE)D3D9_IDirect3DDevice9Ex_GetRasterStatus,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetDialogBoxMode,            (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode],            (PBYTE)D3D9_IDirect3DDevice9Ex_SetDialogBoxMode,            JMP32_SZ));            
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetGammaRamp,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp],                (PBYTE)D3D9_IDirect3DDevice9Ex_SetGammaRamp,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetGammaRamp,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp],                (PBYTE)D3D9_IDirect3DDevice9Ex_GetGammaRamp,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateTexture,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture],               (PBYTE)D3D9_IDirect3DDevice9Ex_CreateTexture,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVolumeTexture,         (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture],         (PBYTE)D3D9_IDirect3DDevice9Ex_CreateVolumeTexture,         JMP32_SZ));         
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateCubeTexture,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture],           (PBYTE)D3D9_IDirect3DDevice9Ex_CreateCubeTexture,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexBuffer,          (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer],          (PBYTE)D3D9_IDirect3DDevice9Ex_CreateVertexBuffer,          JMP32_SZ));          
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateIndexBuffer,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer],           (PBYTE)D3D9_IDirect3DDevice9Ex_CreateIndexBuffer,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateRenderTarget,          (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget],          (PBYTE)D3D9_IDirect3DDevice9Ex_CreateRenderTarget,          JMP32_SZ));          
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface,   (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface],   (PBYTE)D3D9_IDirect3DDevice9Ex_CreateDepthStencilSurface,   JMP32_SZ));   
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::UpdateSurface,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface],               (PBYTE)D3D9_IDirect3DDevice9Ex_UpdateSurface,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::UpdateTexture,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture],               (PBYTE)D3D9_IDirect3DDevice9Ex_UpdateTexture,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderTargetData,         (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData],         (PBYTE)D3D9_IDirect3DDevice9Ex_GetRenderTargetData,         JMP32_SZ));         
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetFrontBufferData,          (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData],          (PBYTE)D3D9_IDirect3DDevice9Ex_GetFrontBufferData,          JMP32_SZ));          
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::StretchRect,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect],                 (PBYTE)D3D9_IDirect3DDevice9Ex_StretchRect,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ColorFill,                   (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill],                   (PBYTE)D3D9_IDirect3DDevice9Ex_ColorFill,                   JMP32_SZ));                   
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface, (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface], (PBYTE)D3D9_IDirect3DDevice9Ex_CreateOffscreenPlainSurface, JMP32_SZ)); 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetRenderTarget,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget],             (PBYTE)D3D9_IDirect3DDevice9Ex_SetRenderTarget,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderTarget,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget],             (PBYTE)D3D9_IDirect3DDevice9Ex_GetRenderTarget,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface,      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface],      (PBYTE)D3D9_IDirect3DDevice9Ex_SetDepthStencilSurface,      JMP32_SZ));      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface,      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface],      (PBYTE)D3D9_IDirect3DDevice9Ex_GetDepthStencilSurface,      JMP32_SZ));      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::BeginScene,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene],                  (PBYTE)D3D9_IDirect3DDevice9Ex_BeginScene,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EndScene,                    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndScene],                    (PBYTE)D3D9_IDirect3DDevice9Ex_EndScene,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Clear,                       (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::Clear],                       (PBYTE)D3D9_IDirect3DDevice9Ex_Clear,                       JMP32_SZ));                       
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTransform,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform],                (PBYTE)D3D9_IDirect3DDevice9Ex_SetTransform,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTransform,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform],                (PBYTE)D3D9_IDirect3DDevice9Ex_GetTransform,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::MultiplyTransform,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform],           (PBYTE)D3D9_IDirect3DDevice9Ex_MultiplyTransform,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetViewport,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport],                 (PBYTE)D3D9_IDirect3DDevice9Ex_SetViewport,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetViewport,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport],                 (PBYTE)D3D9_IDirect3DDevice9Ex_GetViewport,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetMaterial,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial],                 (PBYTE)D3D9_IDirect3DDevice9Ex_SetMaterial,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetMaterial,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial],                 (PBYTE)D3D9_IDirect3DDevice9Ex_GetMaterial,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetLight,                    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetLight],                    (PBYTE)D3D9_IDirect3DDevice9Ex_SetLight,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetLight,                    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLight],                    (PBYTE)D3D9_IDirect3DDevice9Ex_GetLight,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::LightEnable,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable],                 (PBYTE)D3D9_IDirect3DDevice9Ex_LightEnable,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetLightEnable,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable],              (PBYTE)D3D9_IDirect3DDevice9Ex_GetLightEnable,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetClipPlane,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane],                (PBYTE)D3D9_IDirect3DDevice9Ex_SetClipPlane,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetClipPlane,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane],                (PBYTE)D3D9_IDirect3DDevice9Ex_GetClipPlane,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetRenderState,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState],              (PBYTE)D3D9_IDirect3DDevice9Ex_SetRenderState,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderState,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState],              (PBYTE)D3D9_IDirect3DDevice9Ex_GetRenderState,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateStateBlock,            (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock],            (PBYTE)D3D9_IDirect3DDevice9Ex_CreateStateBlock,            JMP32_SZ));            
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::BeginStateBlock,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock],             (PBYTE)D3D9_IDirect3DDevice9Ex_BeginStateBlock,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EndStateBlock,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock],               (PBYTE)D3D9_IDirect3DDevice9Ex_EndStateBlock,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetClipStatus,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus],               (PBYTE)D3D9_IDirect3DDevice9Ex_SetClipStatus,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetClipStatus,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus],               (PBYTE)D3D9_IDirect3DDevice9Ex_GetClipStatus,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTexture,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture],                  (PBYTE)D3D9_IDirect3DDevice9Ex_GetTexture,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTexture,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture],                  (PBYTE)D3D9_IDirect3DDevice9Ex_SetTexture,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTextureStageState,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState],        (PBYTE)D3D9_IDirect3DDevice9Ex_GetTextureStageState,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTextureStageState,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState],        (PBYTE)D3D9_IDirect3DDevice9Ex_SetTextureStageState,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSamplerState,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState],             (PBYTE)D3D9_IDirect3DDevice9Ex_GetSamplerState,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetSamplerState,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState],             (PBYTE)D3D9_IDirect3DDevice9Ex_SetSamplerState,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ValidateDevice,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice],              (PBYTE)D3D9_IDirect3DDevice9Ex_ValidateDevice,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPaletteEntries,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries],           (PBYTE)D3D9_IDirect3DDevice9Ex_SetPaletteEntries,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPaletteEntries,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries],           (PBYTE)D3D9_IDirect3DDevice9Ex_GetPaletteEntries,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette],    (PBYTE)D3D9_IDirect3DDevice9Ex_SetCurrentTexturePalette,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette],    (PBYTE)D3D9_IDirect3DDevice9Ex_GetCurrentTexturePalette,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetScissorRect,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect],              (PBYTE)D3D9_IDirect3DDevice9Ex_SetScissorRect,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetScissorRect,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect],              (PBYTE)D3D9_IDirect3DDevice9Ex_GetScissorRect,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing, (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing], (PBYTE)D3D9_IDirect3DDevice9Ex_SetSoftwareVertexProcessing, JMP32_SZ)); 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing, (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing], (PBYTE)D3D9_IDirect3DDevice9Ex_GetSoftwareVertexProcessing, JMP32_SZ)); 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetNPatchMode,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode],               (PBYTE)D3D9_IDirect3DDevice9Ex_SetNPatchMode,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetNPatchMode,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode],               (PBYTE)D3D9_IDirect3DDevice9Ex_GetNPatchMode,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawPrimitive,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive],               (PBYTE)D3D9_IDirect3DDevice9Ex_DrawPrimitive,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive],        (PBYTE)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitive,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP],             (PBYTE)D3D9_IDirect3DDevice9Ex_DrawPrimitiveUP,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP,      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP],      (PBYTE)D3D9_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP,      JMP32_SZ));      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ProcessVertices,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices],             (PBYTE)D3D9_IDirect3DDevice9Ex_ProcessVertices,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration],     (PBYTE)D3D9_IDirect3DDevice9Ex_CreateVertexDeclaration,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexDeclaration,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration],        (PBYTE)D3D9_IDirect3DDevice9Ex_SetVertexDeclaration,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexDeclaration,        (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration],        (PBYTE)D3D9_IDirect3DDevice9Ex_GetVertexDeclaration,        JMP32_SZ));        
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetFVF,                      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF],                      (PBYTE)D3D9_IDirect3DDevice9Ex_SetFVF,                      JMP32_SZ));                      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetFVF,                      (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF],                      (PBYTE)D3D9_IDirect3DDevice9Ex_GetFVF,                      JMP32_SZ));                      
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexShader,          (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader],          (PBYTE)D3D9_IDirect3DDevice9Ex_CreateVertexShader,          JMP32_SZ));          
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShader,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader],             (PBYTE)D3D9_IDirect3DDevice9Ex_SetVertexShader,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShader,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader],             (PBYTE)D3D9_IDirect3DDevice9Ex_GetVertexShader,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF],    (PBYTE)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantF,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF],    (PBYTE)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantF,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI],    (PBYTE)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantI,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI],    (PBYTE)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantI,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB],    (PBYTE)D3D9_IDirect3DDevice9Ex_SetVertexShaderConstantB,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB,    (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB],    (PBYTE)D3D9_IDirect3DDevice9Ex_GetVertexShaderConstantB,    JMP32_SZ));    
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetStreamSource,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource],             (PBYTE)D3D9_IDirect3DDevice9Ex_SetStreamSource,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetStreamSource,             (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource],             (PBYTE)D3D9_IDirect3DDevice9Ex_GetStreamSource,             JMP32_SZ));             
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq,         (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq],         (PBYTE)D3D9_IDirect3DDevice9Ex_SetStreamSourceFreq,         JMP32_SZ));         
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq,         (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq],         (PBYTE)D3D9_IDirect3DDevice9Ex_GetStreamSourceFreq,         JMP32_SZ));         
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetIndices,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices],                  (PBYTE)D3D9_IDirect3DDevice9Ex_SetIndices,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetIndices,                  (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices],                  (PBYTE)D3D9_IDirect3DDevice9Ex_GetIndices,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreatePixelShader,           (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader],           (PBYTE)D3D9_IDirect3DDevice9Ex_CreatePixelShader,           JMP32_SZ));           
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShader,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader],              (PBYTE)D3D9_IDirect3DDevice9Ex_SetPixelShader,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShader,              (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader],              (PBYTE)D3D9_IDirect3DDevice9Ex_GetPixelShader,              JMP32_SZ));              
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF],     (PBYTE)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantF,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF],     (PBYTE)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantF,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI],     (PBYTE)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantI,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI],     (PBYTE)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantI,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB],     (PBYTE)D3D9_IDirect3DDevice9Ex_SetPixelShaderConstantB,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB,     (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB],     (PBYTE)D3D9_IDirect3DDevice9Ex_GetPixelShaderConstantB,     JMP32_SZ));     
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawRectPatch,               (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch],               (PBYTE)D3D9_IDirect3DDevice9Ex_DrawRectPatch,               JMP32_SZ));               
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawTriPatch,                (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch],                (PBYTE)D3D9_IDirect3DDevice9Ex_DrawTriPatch,                JMP32_SZ));                
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DeletePatch,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch],                 (PBYTE)D3D9_IDirect3DDevice9Ex_DeletePatch,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9Ex->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateQuery,                 (UINT_PTR)DetourFunc((PBYTE)D3D929_IDirect3DDevice9Ex_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery],                 (PBYTE)D3D9_IDirect3DDevice9Ex_CreateQuery,                 JMP32_SZ));                 
}


#endif