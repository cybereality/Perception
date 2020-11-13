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

#ifndef DMT_IDIRECT3DDEVICE9_TABLE
#define DMT_IDIRECT3DDEVICE9_TABLE

/*** IUnknown methods ***/
HRESULT WINAPI D3D9_IDirect3DDevice9_QueryInterface(IDirect3DDevice9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DDevice9->QueryInterface(pDevice, riid, ppvObj);}
ULONG   WINAPI D3D9_IDirect3DDevice9_AddRef(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->AddRef(pDevice);}
ULONG   WINAPI D3D9_IDirect3DDevice9_Release(IDirect3DDevice9* pDevice){ return pDCL_IDirect3DDevice9->Release(pDevice);}

/*** IDirect3DDevice9 methods ***/
HRESULT WINAPI D3D9_IDirect3DDevice9_TestCooperativeLevel(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->TestCooperativeLevel(pDevice);}
UINT    WINAPI D3D9_IDirect3DDevice9_GetAvailableTextureMem(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->GetAvailableTextureMem(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_EvictManagedResources(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->EvictManagedResources(pDevice);}
HRESULT WINAPI D3D9_IDirect3DDevice9_GetDirect3D(IDirect3DDevice9* pDevice, IDirect3D9** ppD3D9){return pDCL_IDirect3DDevice9->GetDirect3D(pDevice, ppD3D9);}
HRESULT WINAPI D3D9_IDirect3DDevice9_GetDeviceCaps(IDirect3DDevice9* pDevice, D3DCAPS9* pCaps){ return pDCL_IDirect3DDevice9->GetDeviceCaps(pDevice, pCaps);}
HRESULT WINAPI D3D9_IDirect3DDevice9_GetDisplayMode(IDirect3DDevice9* pDevice, UINT iSwapChain,D3DDISPLAYMODE* pMode){ return pDCL_IDirect3DDevice9->GetDisplayMode(pDevice, iSwapChain, pMode);}
HRESULT WINAPI D3D9_IDirect3DDevice9_GetCreationParameters(IDirect3DDevice9* pDevice, D3DDEVICE_CREATION_PARAMETERS *pParameters){ return pDCL_IDirect3DDevice9->GetCreationParameters(pDevice, pParameters);}
HRESULT WINAPI D3D9_IDirect3DDevice9_SetCursorProperties(IDirect3DDevice9* pDevice, UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap){ return pDCL_IDirect3DDevice9->SetCursorProperties(pDevice, XHotSpot, YHotSpot, pCursorBitmap);} 
void    WINAPI D3D9_IDirect3DDevice9_SetCursorPosition(IDirect3DDevice9* pDevice, int X,int Y,DWORD Flags){ return pDCL_IDirect3DDevice9->SetCursorPosition(pDevice, X, Y, Flags);} 
BOOL    WINAPI D3D9_IDirect3DDevice9_ShowCursor(IDirect3DDevice9* pDevice, BOOL bShow){ return pDCL_IDirect3DDevice9->ShowCursor(pDevice, bShow);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateAdditionalSwapChain(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain){ return pDCL_IDirect3DDevice9->CreateAdditionalSwapChain(pDevice, pPresentationParameters, pSwapChain);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetSwapChain(IDirect3DDevice9* pDevice, UINT iSwapChain,IDirect3DSwapChain9** pSwapChain){ return pDCL_IDirect3DDevice9->GetSwapChain(pDevice, iSwapChain, pSwapChain);} 
UINT    WINAPI D3D9_IDirect3DDevice9_GetNumberOfSwapChains(IDirect3DDevice9* pDevice){ return pDCL_IDirect3DDevice9->GetNumberOfSwapChains(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){ return pDCL_IDirect3DDevice9->Reset(pDevice, pPresentationParameters);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){return pDCL_IDirect3DDevice9->Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetBackBuffer(IDirect3DDevice9* pDevice, UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer){return pDCL_IDirect3DDevice9->GetBackBuffer(pDevice, iSwapChain, iBackBuffer, Type, ppBackBuffer);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetRasterStatus(IDirect3DDevice9* pDevice, UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus){ return pDCL_IDirect3DDevice9->GetRasterStatus(pDevice, iSwapChain, pRasterStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetDialogBoxMode(IDirect3DDevice9* pDevice, BOOL bEnableDialogs){ return pDCL_IDirect3DDevice9->SetDialogBoxMode(pDevice, bEnableDialogs);} 
void    WINAPI D3D9_IDirect3DDevice9_SetGammaRamp(IDirect3DDevice9* pDevice, UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp){ return pDCL_IDirect3DDevice9->SetGammaRamp(pDevice, iSwapChain, Flags, pRamp);} 
void    WINAPI D3D9_IDirect3DDevice9_GetGammaRamp(IDirect3DDevice9* pDevice, UINT iSwapChain,D3DGAMMARAMP* pRamp){ return pDCL_IDirect3DDevice9->GetGammaRamp(pDevice, iSwapChain, pRamp);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateTexture(IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateTexture(pDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateVolumeTexture(IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateVolumeTexture(pDevice, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateCubeTexture(IDirect3DDevice9* pDevice, UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateCubeTexture(pDevice, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateVertexBuffer(IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateVertexBuffer(pDevice, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateIndexBuffer(IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle){return pDCL_IDirect3DDevice9->CreateIndexBuffer(pDevice, Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateRenderTarget(IDirect3DDevice9* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateRenderTarget(pDevice, Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateDepthStencilSurface(IDirect3DDevice9* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateDepthStencilSurface(pDevice, Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_UpdateSurface(IDirect3DDevice9* pDevice, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint){ return pDCL_IDirect3DDevice9->UpdateSurface(pDevice, pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_UpdateTexture(IDirect3DDevice9* pDevice, IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture){ return pDCL_IDirect3DDevice9->UpdateTexture(pDevice, pSourceTexture, pDestinationTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetRenderTargetData(IDirect3DDevice9* pDevice, IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface){ return pDCL_IDirect3DDevice9->GetRenderTargetData(pDevice, pRenderTarget, pDestSurface);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetFrontBufferData(IDirect3DDevice9* pDevice, UINT iSwapChain,IDirect3DSurface9* pDestSurface){ return pDCL_IDirect3DDevice9->GetFrontBufferData(pDevice, iSwapChain, pDestSurface);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_StretchRect(IDirect3DDevice9* pDevice, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter){return pDCL_IDirect3DDevice9->StretchRect(pDevice, pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);}
HRESULT WINAPI D3D9_IDirect3DDevice9_ColorFill(IDirect3DDevice9* pDevice, IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color){ return pDCL_IDirect3DDevice9->ColorFill(pDevice, pSurface, pRect, color);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface(IDirect3DDevice9* pDevice, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){ return pDCL_IDirect3DDevice9->CreateOffscreenPlainSurface(pDevice, Width, Height, Format, Pool, ppSurface, pSharedHandle);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetRenderTarget(IDirect3DDevice9* pDevice, DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget){ return pDCL_IDirect3DDevice9->SetRenderTarget(pDevice, RenderTargetIndex, pRenderTarget);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetRenderTarget(IDirect3DDevice9* pDevice, DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget){ return pDCL_IDirect3DDevice9->GetRenderTarget(pDevice, RenderTargetIndex, ppRenderTarget);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetDepthStencilSurface(IDirect3DDevice9* pDevice, IDirect3DSurface9* pNewZStencil){return pDCL_IDirect3DDevice9->SetDepthStencilSurface(pDevice, pNewZStencil);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_GetDepthStencilSurface(IDirect3DDevice9* pDevice, IDirect3DSurface9** ppZStencilSurface){return pDCL_IDirect3DDevice9->GetDepthStencilSurface(pDevice, ppZStencilSurface);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_BeginScene(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->BeginScene(pDevice);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_EndScene(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->EndScene(pDevice);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_Clear(IDirect3DDevice9* pDevice, DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil){return pDCL_IDirect3DDevice9->Clear(pDevice, Count, pRects, Flags, Color, Z, Stencil);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_SetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9->SetTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_GetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9->GetTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_MultiplyTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){return pDCL_IDirect3DDevice9->MultiplyTransform(pDevice, State, pMatrix);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_SetViewport(IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport){return pDCL_IDirect3DDevice9->SetViewport(pDevice, pViewport);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_GetViewport(IDirect3DDevice9* pDevice, D3DVIEWPORT9* pViewport){return pDCL_IDirect3DDevice9->GetViewport(pDevice, pViewport);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetMaterial(IDirect3DDevice9* pDevice, CONST D3DMATERIAL9* pMaterial){return pDCL_IDirect3DDevice9->SetMaterial(pDevice, pMaterial);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetMaterial(IDirect3DDevice9* pDevice, D3DMATERIAL9* pMaterial){return pDCL_IDirect3DDevice9->GetMaterial(pDevice, pMaterial);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetLight(IDirect3DDevice9* pDevice, DWORD Index, CONST D3DLIGHT9* pLight){return pDCL_IDirect3DDevice9->SetLight(pDevice, Index, pLight);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetLight(IDirect3DDevice9* pDevice, DWORD Index, D3DLIGHT9* pLight){return pDCL_IDirect3DDevice9->GetLight(pDevice, Index, pLight);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_LightEnable(IDirect3DDevice9* pDevice, DWORD Index, BOOL Enable){return pDCL_IDirect3DDevice9->LightEnable(pDevice, Index, Enable);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetLightEnable(IDirect3DDevice9* pDevice, DWORD Index,BOOL* pEnable){return pDCL_IDirect3DDevice9->GetLightEnable(pDevice, Index, pEnable);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetClipPlane(IDirect3DDevice9* pDevice, DWORD Index,CONST float* pPlane){return pDCL_IDirect3DDevice9->SetClipPlane(pDevice, Index, pPlane);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetClipPlane(IDirect3DDevice9* pDevice, DWORD Index,float* pPlane){return pDCL_IDirect3DDevice9->GetClipPlane(pDevice, Index, pPlane);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetRenderState(IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State,DWORD Value){return pDCL_IDirect3DDevice9->SetRenderState(pDevice, State, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetRenderState(IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State,DWORD* pValue){return pDCL_IDirect3DDevice9->GetRenderState(pDevice, State, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateStateBlock(IDirect3DDevice9* pDevice, D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB){return pDCL_IDirect3DDevice9->CreateStateBlock(pDevice, Type, ppSB);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_BeginStateBlock(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->BeginStateBlock(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_EndStateBlock(IDirect3DDevice9* pDevice, IDirect3DStateBlock9** ppSB){return pDCL_IDirect3DDevice9->EndStateBlock(pDevice, ppSB);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetClipStatus(IDirect3DDevice9* pDevice, CONST D3DCLIPSTATUS9* pClipStatus){return pDCL_IDirect3DDevice9->SetClipStatus(pDevice, pClipStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetClipStatus(IDirect3DDevice9* pDevice, D3DCLIPSTATUS9* pClipStatus){return pDCL_IDirect3DDevice9->GetClipStatus(pDevice, pClipStatus);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetTexture(IDirect3DDevice9* pDevice, DWORD Stage,IDirect3DBaseTexture9** ppTexture){return pDCL_IDirect3DDevice9->GetTexture(pDevice, Stage, ppTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetTexture(IDirect3DDevice9* pDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture){return pDCL_IDirect3DDevice9->SetTexture(pDevice, Stage, pTexture);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetTextureStageState(IDirect3DDevice9* pDevice, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue){return pDCL_IDirect3DDevice9->GetTextureStageState(pDevice, Stage, Type, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetTextureStageState(IDirect3DDevice9* pDevice, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value){return pDCL_IDirect3DDevice9->SetTextureStageState(pDevice, Stage, Type, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetSamplerState(IDirect3DDevice9* pDevice, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue){return pDCL_IDirect3DDevice9->GetSamplerState(pDevice, Sampler, Type, pValue);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetSamplerState(IDirect3DDevice9* pDevice, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value){return pDCL_IDirect3DDevice9->SetSamplerState(pDevice, Sampler, Type, Value);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_ValidateDevice(IDirect3DDevice9* pDevice, DWORD* pNumPasses){return pDCL_IDirect3DDevice9->ValidateDevice(pDevice, pNumPasses);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetPaletteEntries(IDirect3DDevice9* pDevice, UINT PaletteNumber,CONST PALETTEENTRY* pEntries){return pDCL_IDirect3DDevice9->SetPaletteEntries(pDevice, PaletteNumber, pEntries);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetPaletteEntries(IDirect3DDevice9* pDevice, UINT PaletteNumber,PALETTEENTRY* pEntries){return pDCL_IDirect3DDevice9->GetPaletteEntries(pDevice, PaletteNumber, pEntries);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetCurrentTexturePalette(IDirect3DDevice9* pDevice, UINT PaletteNumber){return pDCL_IDirect3DDevice9->SetCurrentTexturePalette(pDevice, PaletteNumber);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetCurrentTexturePalette(IDirect3DDevice9* pDevice, UINT *PaletteNumber){return pDCL_IDirect3DDevice9->GetCurrentTexturePalette(pDevice, PaletteNumber);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetScissorRect(IDirect3DDevice9* pDevice, CONST RECT* pRect){return pDCL_IDirect3DDevice9->SetScissorRect(pDevice, pRect);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetScissorRect(IDirect3DDevice9* pDevice, RECT* pRect){return pDCL_IDirect3DDevice9->GetScissorRect(pDevice, pRect);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing(IDirect3DDevice9* pDevice, BOOL bSoftware){return pDCL_IDirect3DDevice9->SetSoftwareVertexProcessing(pDevice, bSoftware);} 
BOOL    WINAPI D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->GetSoftwareVertexProcessing(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetNPatchMode(IDirect3DDevice9* pDevice, float nSegments){return pDCL_IDirect3DDevice9->SetNPatchMode(pDevice, nSegments);} 
float   WINAPI D3D9_IDirect3DDevice9_GetNPatchMode(IDirect3DDevice9* pDevice){return pDCL_IDirect3DDevice9->GetNPatchMode(pDevice);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){return pDCL_IDirect3DDevice9->DrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount){return pDCL_IDirect3DDevice9->DrawIndexedPrimitive(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawPrimitiveUP(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){return pDCL_IDirect3DDevice9->DrawPrimitiveUP(pDevice, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){return pDCL_IDirect3DDevice9->DrawIndexedPrimitiveUP(pDevice, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_ProcessVertices(IDirect3DDevice9* pDevice, UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags){return pDCL_IDirect3DDevice9->ProcessVertices(pDevice, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateVertexDeclaration(IDirect3DDevice9* pDevice, CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl){return pDCL_IDirect3DDevice9->CreateVertexDeclaration(pDevice, pVertexElements, ppDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9* pDecl){return pDCL_IDirect3DDevice9->SetVertexDeclaration(pDevice, pDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9** ppDecl){return pDCL_IDirect3DDevice9->GetVertexDeclaration(pDevice, ppDecl);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetFVF(IDirect3DDevice9* pDevice, DWORD FVF){return pDCL_IDirect3DDevice9->SetFVF(pDevice, FVF);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetFVF(IDirect3DDevice9* pDevice, DWORD* pFVF){return pDCL_IDirect3DDevice9->GetFVF(pDevice, pFVF);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateVertexShader(IDirect3DDevice9* pDevice, CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader){return pDCL_IDirect3DDevice9->CreateVertexShader(pDevice, pFunction, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9* pShader){return pDCL_IDirect3DDevice9->SetVertexShader(pDevice, pShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9** ppShader){return pDCL_IDirect3DDevice9->GetVertexShader(pDevice, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9->SetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister,float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9->GetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetVertexShaderConstantI(IDirect3DDevice9* pDevice, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9->SetVertexShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetVertexShaderConstantI(IDirect3DDevice9* pDevice, UINT StartRegister,int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9->GetVertexShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetVertexShaderConstantB(IDirect3DDevice9* pDevice, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount){return pDCL_IDirect3DDevice9->SetVertexShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetVertexShaderConstantB(IDirect3DDevice9* pDevice, UINT StartRegister,BOOL* pConstantData,UINT BoolCount){return pDCL_IDirect3DDevice9->GetVertexShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride){return pDCL_IDirect3DDevice9->SetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride){return pDCL_IDirect3DDevice9->GetStreamSource(pDevice, StreamNumber, ppStreamData, pOffsetInBytes, pStride);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetStreamSourceFreq(IDirect3DDevice9* pDevice, UINT StreamNumber,UINT Setting){return pDCL_IDirect3DDevice9->SetStreamSourceFreq(pDevice, StreamNumber, Setting);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetStreamSourceFreq(IDirect3DDevice9* pDevice, UINT StreamNumber,UINT* pSetting){return pDCL_IDirect3DDevice9->GetStreamSourceFreq(pDevice, StreamNumber, pSetting);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetIndices(IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9* pIndexData){return pDCL_IDirect3DDevice9->SetIndices(pDevice, pIndexData);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetIndices(IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9** ppIndexData){return pDCL_IDirect3DDevice9->GetIndices(pDevice, ppIndexData);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_CreatePixelShader(IDirect3DDevice9* pDevice, CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader){return pDCL_IDirect3DDevice9->CreatePixelShader(pDevice, pFunction, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9* pShader){return pDCL_IDirect3DDevice9->SetPixelShader(pDevice, pShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9** ppShader){return pDCL_IDirect3DDevice9->GetPixelShader(pDevice, ppShader);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetPixelShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9->SetPixelShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetPixelShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister,float* pConstantData,UINT Vector4fCount){return pDCL_IDirect3DDevice9->GetPixelShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetPixelShaderConstantI(IDirect3DDevice9* pDevice, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9->SetPixelShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetPixelShaderConstantI(IDirect3DDevice9* pDevice, UINT StartRegister,int* pConstantData,UINT Vector4iCount){return pDCL_IDirect3DDevice9->GetPixelShaderConstantI(pDevice, StartRegister, pConstantData, Vector4iCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_SetPixelShaderConstantB(IDirect3DDevice9* pDevice, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount){return pDCL_IDirect3DDevice9->SetPixelShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_GetPixelShaderConstantB(IDirect3DDevice9* pDevice, UINT StartRegister,BOOL* pConstantData,UINT BoolCount){return pDCL_IDirect3DDevice9->GetPixelShaderConstantB(pDevice, StartRegister, pConstantData, BoolCount);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawRectPatch(IDirect3DDevice9* pDevice, UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo){return pDCL_IDirect3DDevice9->DrawRectPatch(pDevice, Handle, pNumSegs, pRectPatchInfo);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DrawTriPatch(IDirect3DDevice9* pDevice, UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo){return pDCL_IDirect3DDevice9->DrawTriPatch(pDevice, Handle, pNumSegs, pTriPatchInfo);} 
HRESULT WINAPI D3D9_IDirect3DDevice9_DeletePatch(IDirect3DDevice9* pDevice, UINT Handle){return pDCL_IDirect3DDevice9->DeletePatch(pDevice, Handle);}  
HRESULT WINAPI D3D9_IDirect3DDevice9_CreateQuery(IDirect3DDevice9* pDevice, D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery){return pDCL_IDirect3DDevice9->CreateQuery(pDevice, Type, ppQuery);} 

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Override_D3D9_IDirect3DDevice9_VMTable()
{
	/*** IUnknown super methods ***/
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface],                      (UINT_PTR)D3D9_IDirect3DDevice9_QueryInterface);
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::AddRef],                              (UINT_PTR)D3D9_IDirect3DDevice9_AddRef);
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::Release],                             (UINT_PTR)D3D9_IDirect3DDevice9_Release);

	/*** IDirect3DDevice9 super methods ***/
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel],        (UINT_PTR)D3D9_IDirect3DDevice9_TestCooperativeLevel);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem],      (UINT_PTR)D3D9_IDirect3DDevice9_GetAvailableTextureMem);       
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources],       (UINT_PTR)D3D9_IDirect3DDevice9_EvictManagedResources);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D],                 (UINT_PTR)D3D9_IDirect3DDevice9_GetDirect3D);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps],               (UINT_PTR)D3D9_IDirect3DDevice9_GetDeviceCaps);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode],              (UINT_PTR)D3D9_IDirect3DDevice9_GetDisplayMode);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters],       (UINT_PTR)D3D9_IDirect3DDevice9_GetCreationParameters);       
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties],         (UINT_PTR)D3D9_IDirect3DDevice9_SetCursorProperties);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition],           (UINT_PTR)D3D9_IDirect3DDevice9_SetCursorPosition);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor],                  (UINT_PTR)D3D9_IDirect3DDevice9_ShowCursor);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain],   (UINT_PTR)D3D9_IDirect3DDevice9_CreateAdditionalSwapChain);   
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain],                (UINT_PTR)D3D9_IDirect3DDevice9_GetSwapChain);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains],       (UINT_PTR)D3D9_IDirect3DDevice9_GetNumberOfSwapChains);       
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Reset],                       (UINT_PTR)D3D9_IDirect3DDevice9_Reset);                       
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Present],                     (UINT_PTR)D3D9_IDirect3DDevice9_Present);                     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer],               (UINT_PTR)D3D9_IDirect3DDevice9_GetBackBuffer);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus],             (UINT_PTR)D3D9_IDirect3DDevice9_GetRasterStatus);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode],            (UINT_PTR)D3D9_IDirect3DDevice9_SetDialogBoxMode);            
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp],                (UINT_PTR)D3D9_IDirect3DDevice9_SetGammaRamp);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp],                (UINT_PTR)D3D9_IDirect3DDevice9_GetGammaRamp);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture],               (UINT_PTR)D3D9_IDirect3DDevice9_CreateTexture);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture],         (UINT_PTR)D3D9_IDirect3DDevice9_CreateVolumeTexture);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture],           (UINT_PTR)D3D9_IDirect3DDevice9_CreateCubeTexture);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer],          (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexBuffer);          
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer],           (UINT_PTR)D3D9_IDirect3DDevice9_CreateIndexBuffer);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget],          (UINT_PTR)D3D9_IDirect3DDevice9_CreateRenderTarget);          
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface],   (UINT_PTR)D3D9_IDirect3DDevice9_CreateDepthStencilSurface);   
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface],               (UINT_PTR)D3D9_IDirect3DDevice9_UpdateSurface);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture],               (UINT_PTR)D3D9_IDirect3DDevice9_UpdateTexture);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData],         (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderTargetData);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData],          (UINT_PTR)D3D9_IDirect3DDevice9_GetFrontBufferData);          
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect],                 (UINT_PTR)D3D9_IDirect3DDevice9_StretchRect);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill],                   (UINT_PTR)D3D9_IDirect3DDevice9_ColorFill);                   
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface], (UINT_PTR)D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface); 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget],             (UINT_PTR)D3D9_IDirect3DDevice9_SetRenderTarget);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget],             (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderTarget);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface],      (UINT_PTR)D3D9_IDirect3DDevice9_SetDepthStencilSurface);      
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface],      (UINT_PTR)D3D9_IDirect3DDevice9_GetDepthStencilSurface);      
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene],                  (UINT_PTR)D3D9_IDirect3DDevice9_BeginScene);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndScene],                    (UINT_PTR)D3D9_IDirect3DDevice9_EndScene);                    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Clear],                       (UINT_PTR)D3D9_IDirect3DDevice9_Clear);                       
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform],                (UINT_PTR)D3D9_IDirect3DDevice9_SetTransform);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform],                (UINT_PTR)D3D9_IDirect3DDevice9_GetTransform);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform],           (UINT_PTR)D3D9_IDirect3DDevice9_MultiplyTransform);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport],                 (UINT_PTR)D3D9_IDirect3DDevice9_SetViewport);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport],                 (UINT_PTR)D3D9_IDirect3DDevice9_GetViewport);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial],                 (UINT_PTR)D3D9_IDirect3DDevice9_SetMaterial);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial],                 (UINT_PTR)D3D9_IDirect3DDevice9_GetMaterial);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetLight],                    (UINT_PTR)D3D9_IDirect3DDevice9_SetLight);                    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLight],                    (UINT_PTR)D3D9_IDirect3DDevice9_GetLight);                    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable],                 (UINT_PTR)D3D9_IDirect3DDevice9_LightEnable);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable],              (UINT_PTR)D3D9_IDirect3DDevice9_GetLightEnable);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane],                (UINT_PTR)D3D9_IDirect3DDevice9_SetClipPlane);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane],                (UINT_PTR)D3D9_IDirect3DDevice9_GetClipPlane);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState],              (UINT_PTR)D3D9_IDirect3DDevice9_SetRenderState);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState],              (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderState);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock],            (UINT_PTR)D3D9_IDirect3DDevice9_CreateStateBlock);            
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock],             (UINT_PTR)D3D9_IDirect3DDevice9_BeginStateBlock);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock],               (UINT_PTR)D3D9_IDirect3DDevice9_EndStateBlock);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus],               (UINT_PTR)D3D9_IDirect3DDevice9_SetClipStatus);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus],               (UINT_PTR)D3D9_IDirect3DDevice9_GetClipStatus);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture],                  (UINT_PTR)D3D9_IDirect3DDevice9_GetTexture);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture],                  (UINT_PTR)D3D9_IDirect3DDevice9_SetTexture);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState],        (UINT_PTR)D3D9_IDirect3DDevice9_GetTextureStageState);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState],        (UINT_PTR)D3D9_IDirect3DDevice9_SetTextureStageState);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState],             (UINT_PTR)D3D9_IDirect3DDevice9_GetSamplerState);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState],             (UINT_PTR)D3D9_IDirect3DDevice9_SetSamplerState);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice],              (UINT_PTR)D3D9_IDirect3DDevice9_ValidateDevice);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries],           (UINT_PTR)D3D9_IDirect3DDevice9_SetPaletteEntries);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries],           (UINT_PTR)D3D9_IDirect3DDevice9_GetPaletteEntries);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette],    (UINT_PTR)D3D9_IDirect3DDevice9_SetCurrentTexturePalette);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette],    (UINT_PTR)D3D9_IDirect3DDevice9_GetCurrentTexturePalette);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect],              (UINT_PTR)D3D9_IDirect3DDevice9_SetScissorRect);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect],              (UINT_PTR)D3D9_IDirect3DDevice9_GetScissorRect);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing], (UINT_PTR)D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing); 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing], (UINT_PTR)D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing); 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode],               (UINT_PTR)D3D9_IDirect3DDevice9_SetNPatchMode);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode],               (UINT_PTR)D3D9_IDirect3DDevice9_GetNPatchMode);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive],               (UINT_PTR)D3D9_IDirect3DDevice9_DrawPrimitive);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive],        (UINT_PTR)D3D9_IDirect3DDevice9_DrawIndexedPrimitive);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP],             (UINT_PTR)D3D9_IDirect3DDevice9_DrawPrimitiveUP);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP],      (UINT_PTR)D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP);      
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices],             (UINT_PTR)D3D9_IDirect3DDevice9_ProcessVertices);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration],     (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexDeclaration);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration],        (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexDeclaration);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration],        (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexDeclaration);        
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF],                      (UINT_PTR)D3D9_IDirect3DDevice9_SetFVF);                      
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF],                      (UINT_PTR)D3D9_IDirect3DDevice9_GetFVF);                      
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader],          (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexShader);          
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader],             (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShader);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader],             (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShader);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF],    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantF);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF],    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantF);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI],    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantI);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI],    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantI);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB],    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantB);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB],    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantB);    
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource],             (UINT_PTR)D3D9_IDirect3DDevice9_SetStreamSource);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource],             (UINT_PTR)D3D9_IDirect3DDevice9_GetStreamSource);             
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq],         (UINT_PTR)D3D9_IDirect3DDevice9_SetStreamSourceFreq);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq],         (UINT_PTR)D3D9_IDirect3DDevice9_GetStreamSourceFreq);         
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices],                  (UINT_PTR)D3D9_IDirect3DDevice9_SetIndices);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices],                  (UINT_PTR)D3D9_IDirect3DDevice9_GetIndices);                  
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader],           (UINT_PTR)D3D9_IDirect3DDevice9_CreatePixelShader);           
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader],              (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShader);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader],              (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShader);              
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF],     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantF);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF],     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantF);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI],     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantI);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI],     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantI);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB],     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantB);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB],     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantB);     
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch],               (UINT_PTR)D3D9_IDirect3DDevice9_DrawRectPatch);               
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch],                (UINT_PTR)D3D9_IDirect3DDevice9_DrawTriPatch);                
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch],                 (UINT_PTR)D3D9_IDirect3DDevice9_DeletePatch);                 
	OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery],                 (UINT_PTR)D3D9_IDirect3DDevice9_CreateQuery);                 
}

/**
* Creates a Virtual Methods Table array of the DirectX 9 DXGISwapChain
***/
void Generate_D3D9_IDirect3DDevice9_VMTable_Array()
{
	/*** IUnknown super methods ***/
	anD3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface] =                      (UINT_PTR)D3D9_IDirect3DDevice9_QueryInterface;
	anD3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::AddRef] =                              (UINT_PTR)D3D9_IDirect3DDevice9_AddRef;
	anD3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::Release] =                             (UINT_PTR)D3D9_IDirect3DDevice9_Release;

	/*** IDirect3DDevice9 super methods ***/
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel] =        (UINT_PTR)D3D9_IDirect3DDevice9_TestCooperativeLevel;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem] =      (UINT_PTR)D3D9_IDirect3DDevice9_GetAvailableTextureMem;       
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources] =       (UINT_PTR)D3D9_IDirect3DDevice9_EvictManagedResources;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D] =                 (UINT_PTR)D3D9_IDirect3DDevice9_GetDirect3D;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps] =               (UINT_PTR)D3D9_IDirect3DDevice9_GetDeviceCaps;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode] =              (UINT_PTR)D3D9_IDirect3DDevice9_GetDisplayMode;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters] =       (UINT_PTR)D3D9_IDirect3DDevice9_GetCreationParameters;       
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties] =         (UINT_PTR)D3D9_IDirect3DDevice9_SetCursorProperties;         
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition] =           (UINT_PTR)D3D9_IDirect3DDevice9_SetCursorPosition;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor] =                  (UINT_PTR)D3D9_IDirect3DDevice9_ShowCursor;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain] =   (UINT_PTR)D3D9_IDirect3DDevice9_CreateAdditionalSwapChain;   
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain] =                (UINT_PTR)D3D9_IDirect3DDevice9_GetSwapChain;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains] =       (UINT_PTR)D3D9_IDirect3DDevice9_GetNumberOfSwapChains;       
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Reset] =                       (UINT_PTR)D3D9_IDirect3DDevice9_Reset;                       
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Present] =                     (UINT_PTR)D3D9_IDirect3DDevice9_Present;                     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer] =               (UINT_PTR)D3D9_IDirect3DDevice9_GetBackBuffer;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus] =             (UINT_PTR)D3D9_IDirect3DDevice9_GetRasterStatus;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode] =            (UINT_PTR)D3D9_IDirect3DDevice9_SetDialogBoxMode;            
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp] =                (UINT_PTR)D3D9_IDirect3DDevice9_SetGammaRamp;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp] =                (UINT_PTR)D3D9_IDirect3DDevice9_GetGammaRamp;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture] =               (UINT_PTR)D3D9_IDirect3DDevice9_CreateTexture;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture] =         (UINT_PTR)D3D9_IDirect3DDevice9_CreateVolumeTexture;         
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture] =           (UINT_PTR)D3D9_IDirect3DDevice9_CreateCubeTexture;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer] =          (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexBuffer;          
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer] =           (UINT_PTR)D3D9_IDirect3DDevice9_CreateIndexBuffer;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget] =          (UINT_PTR)D3D9_IDirect3DDevice9_CreateRenderTarget;          
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface] =   (UINT_PTR)D3D9_IDirect3DDevice9_CreateDepthStencilSurface;   
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface] =               (UINT_PTR)D3D9_IDirect3DDevice9_UpdateSurface;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture] =               (UINT_PTR)D3D9_IDirect3DDevice9_UpdateTexture;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData] =         (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderTargetData;         
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData] =          (UINT_PTR)D3D9_IDirect3DDevice9_GetFrontBufferData;          
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect] =                 (UINT_PTR)D3D9_IDirect3DDevice9_StretchRect;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill] =                   (UINT_PTR)D3D9_IDirect3DDevice9_ColorFill;                   
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface] = (UINT_PTR)D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface; 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget] =             (UINT_PTR)D3D9_IDirect3DDevice9_SetRenderTarget;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget] =             (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderTarget;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface] =      (UINT_PTR)D3D9_IDirect3DDevice9_SetDepthStencilSurface;      
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface] =      (UINT_PTR)D3D9_IDirect3DDevice9_GetDepthStencilSurface;      
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene] =                  (UINT_PTR)D3D9_IDirect3DDevice9_BeginScene;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndScene] =                    (UINT_PTR)D3D9_IDirect3DDevice9_EndScene;                    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Clear] =                       (UINT_PTR)D3D9_IDirect3DDevice9_Clear;                       
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform] =                (UINT_PTR)D3D9_IDirect3DDevice9_SetTransform;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform] =                (UINT_PTR)D3D9_IDirect3DDevice9_GetTransform;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform] =           (UINT_PTR)D3D9_IDirect3DDevice9_MultiplyTransform;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport] =                 (UINT_PTR)D3D9_IDirect3DDevice9_SetViewport;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport] =                 (UINT_PTR)D3D9_IDirect3DDevice9_GetViewport;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial] =                 (UINT_PTR)D3D9_IDirect3DDevice9_SetMaterial;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial] =                 (UINT_PTR)D3D9_IDirect3DDevice9_GetMaterial;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetLight] =                    (UINT_PTR)D3D9_IDirect3DDevice9_SetLight;                    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLight] =                    (UINT_PTR)D3D9_IDirect3DDevice9_GetLight;                    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable] =                 (UINT_PTR)D3D9_IDirect3DDevice9_LightEnable;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable] =              (UINT_PTR)D3D9_IDirect3DDevice9_GetLightEnable;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane] =                (UINT_PTR)D3D9_IDirect3DDevice9_SetClipPlane;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane] =                (UINT_PTR)D3D9_IDirect3DDevice9_GetClipPlane;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState] =              (UINT_PTR)D3D9_IDirect3DDevice9_SetRenderState;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState] =              (UINT_PTR)D3D9_IDirect3DDevice9_GetRenderState;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock] =            (UINT_PTR)D3D9_IDirect3DDevice9_CreateStateBlock;            
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock] =             (UINT_PTR)D3D9_IDirect3DDevice9_BeginStateBlock;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock] =               (UINT_PTR)D3D9_IDirect3DDevice9_EndStateBlock;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus] =               (UINT_PTR)D3D9_IDirect3DDevice9_SetClipStatus;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus] =               (UINT_PTR)D3D9_IDirect3DDevice9_GetClipStatus;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture] =                  (UINT_PTR)D3D9_IDirect3DDevice9_GetTexture;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture] =                  (UINT_PTR)D3D9_IDirect3DDevice9_SetTexture;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState] =        (UINT_PTR)D3D9_IDirect3DDevice9_GetTextureStageState;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState] =        (UINT_PTR)D3D9_IDirect3DDevice9_SetTextureStageState;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState] =             (UINT_PTR)D3D9_IDirect3DDevice9_GetSamplerState;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState] =             (UINT_PTR)D3D9_IDirect3DDevice9_SetSamplerState;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice] =              (UINT_PTR)D3D9_IDirect3DDevice9_ValidateDevice;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries] =           (UINT_PTR)D3D9_IDirect3DDevice9_SetPaletteEntries;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries] =           (UINT_PTR)D3D9_IDirect3DDevice9_GetPaletteEntries;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette] =    (UINT_PTR)D3D9_IDirect3DDevice9_SetCurrentTexturePalette;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette] =    (UINT_PTR)D3D9_IDirect3DDevice9_GetCurrentTexturePalette;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect] =              (UINT_PTR)D3D9_IDirect3DDevice9_SetScissorRect;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect] =              (UINT_PTR)D3D9_IDirect3DDevice9_GetScissorRect;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing] = (UINT_PTR)D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing; 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing] = (UINT_PTR)D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing; 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode] =               (UINT_PTR)D3D9_IDirect3DDevice9_SetNPatchMode;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode] =               (UINT_PTR)D3D9_IDirect3DDevice9_GetNPatchMode;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive] =               (UINT_PTR)D3D9_IDirect3DDevice9_DrawPrimitive;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive] =        (UINT_PTR)D3D9_IDirect3DDevice9_DrawIndexedPrimitive;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP] =             (UINT_PTR)D3D9_IDirect3DDevice9_DrawPrimitiveUP;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP] =      (UINT_PTR)D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP;      
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices] =             (UINT_PTR)D3D9_IDirect3DDevice9_ProcessVertices;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration] =     (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexDeclaration;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration] =        (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexDeclaration;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration] =        (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexDeclaration;        
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF] =                      (UINT_PTR)D3D9_IDirect3DDevice9_SetFVF;                      
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF] =                      (UINT_PTR)D3D9_IDirect3DDevice9_GetFVF;                      
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader] =          (UINT_PTR)D3D9_IDirect3DDevice9_CreateVertexShader;          
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader] =             (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShader;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader] =             (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShader;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF] =    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantF;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF] =    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantF;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI] =    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantI;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI] =    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantI;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB] =    (UINT_PTR)D3D9_IDirect3DDevice9_SetVertexShaderConstantB;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB] =    (UINT_PTR)D3D9_IDirect3DDevice9_GetVertexShaderConstantB;    
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource] =             (UINT_PTR)D3D9_IDirect3DDevice9_SetStreamSource;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource] =             (UINT_PTR)D3D9_IDirect3DDevice9_GetStreamSource;             
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq] =         (UINT_PTR)D3D9_IDirect3DDevice9_SetStreamSourceFreq;         
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq] =         (UINT_PTR)D3D9_IDirect3DDevice9_GetStreamSourceFreq;         
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices] =                  (UINT_PTR)D3D9_IDirect3DDevice9_SetIndices;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices] =                  (UINT_PTR)D3D9_IDirect3DDevice9_GetIndices;                  
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader] =           (UINT_PTR)D3D9_IDirect3DDevice9_CreatePixelShader;           
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader] =              (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShader;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader] =              (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShader;              
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF] =     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantF;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF] =     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantF;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI] =     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantI;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI] =     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantI;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB] =     (UINT_PTR)D3D9_IDirect3DDevice9_SetPixelShaderConstantB;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB] =     (UINT_PTR)D3D9_IDirect3DDevice9_GetPixelShaderConstantB;     
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch] =               (UINT_PTR)D3D9_IDirect3DDevice9_DrawRectPatch;               
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch] =                (UINT_PTR)D3D9_IDirect3DDevice9_DrawTriPatch;                
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch] =                 (UINT_PTR)D3D9_IDirect3DDevice9_DeletePatch;                 
	anD3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery] =                 (UINT_PTR)D3D9_IDirect3DDevice9_CreateQuery;
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device using a generated array.
***/
void Override_D3D9_IDirect3DDevice9_VMTable_by_Array()
{
	OverrideFullVTable(&D3D9_IDirect3DDevice9_VMTable[0],
		(void*)&anD3D9_IDirect3DDevice9_VMTable[0],
		(D3D9_IDIRECT3DDEVICE9_METHODS_NUMBER)*sizeof(DWORD)+1);
}

/**
* Overrides the Virtual Methods Table of the DirectX 9 Device
***/
void Detour_D3D9_IDirect3DDevice9_VMTable()
{
	/*** IUnknown super methods ***/
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface],                      (PBYTE)D3D9_IDirect3DDevice9_QueryInterface,              JMP32_SZ));
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IUNKNOWN::AddRef,                              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::AddRef],                              (PBYTE)D3D9_IDirect3DDevice9_AddRef,                      JMP32_SZ));
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IUNKNOWN::Release,                             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::Release],                             (PBYTE)D3D9_IDirect3DDevice9_Release,                     JMP32_SZ));

	/*** IDirect3DDevice9 super methods ***/
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::TestCooperativeLevel,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::TestCooperativeLevel],        (PBYTE)D3D9_IDirect3DDevice9_TestCooperativeLevel,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetAvailableTextureMem],      (PBYTE)D3D9_IDirect3DDevice9_GetAvailableTextureMem,      JMP32_SZ));      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EvictManagedResources,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EvictManagedResources],       (PBYTE)D3D9_IDirect3DDevice9_EvictManagedResources,       JMP32_SZ));       
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDirect3D,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDirect3D],                 (PBYTE)D3D9_IDirect3DDevice9_GetDirect3D,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDeviceCaps,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDeviceCaps],               (PBYTE)D3D9_IDirect3DDevice9_GetDeviceCaps,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDisplayMode,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDisplayMode],              (PBYTE)D3D9_IDirect3DDevice9_GetDisplayMode,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetCreationParameters,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCreationParameters],       (PBYTE)D3D9_IDirect3DDevice9_GetCreationParameters,       JMP32_SZ));       
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCursorProperties,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorProperties],         (PBYTE)D3D9_IDirect3DDevice9_SetCursorProperties,         JMP32_SZ));         
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCursorPosition,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCursorPosition],           (PBYTE)D3D9_IDirect3DDevice9_SetCursorPosition,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ShowCursor,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ShowCursor],                  (PBYTE)D3D9_IDirect3DDevice9_ShowCursor,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain,   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateAdditionalSwapChain],   (PBYTE)D3D9_IDirect3DDevice9_CreateAdditionalSwapChain,   JMP32_SZ));   
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSwapChain,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSwapChain],                (PBYTE)D3D9_IDirect3DDevice9_GetSwapChain,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains,       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNumberOfSwapChains],       (PBYTE)D3D9_IDirect3DDevice9_GetNumberOfSwapChains,       JMP32_SZ));       
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Reset,                       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Reset],                       (PBYTE)D3D9_IDirect3DDevice9_Reset,                       JMP32_SZ));                       
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Present,                     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Present],                     (PBYTE)D3D9_IDirect3DDevice9_Present,                     JMP32_SZ));                     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetBackBuffer,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetBackBuffer],               (PBYTE)D3D9_IDirect3DDevice9_GetBackBuffer,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRasterStatus,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRasterStatus],             (PBYTE)D3D9_IDirect3DDevice9_GetRasterStatus,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetDialogBoxMode,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDialogBoxMode],            (PBYTE)D3D9_IDirect3DDevice9_SetDialogBoxMode,            JMP32_SZ));            
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetGammaRamp,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetGammaRamp],                (PBYTE)D3D9_IDirect3DDevice9_SetGammaRamp,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetGammaRamp,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetGammaRamp],                (PBYTE)D3D9_IDirect3DDevice9_GetGammaRamp,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateTexture,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateTexture],               (PBYTE)D3D9_IDirect3DDevice9_CreateTexture,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVolumeTexture,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVolumeTexture],         (PBYTE)D3D9_IDirect3DDevice9_CreateVolumeTexture,         JMP32_SZ));         
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateCubeTexture,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateCubeTexture],           (PBYTE)D3D9_IDirect3DDevice9_CreateCubeTexture,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexBuffer,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexBuffer],          (PBYTE)D3D9_IDirect3DDevice9_CreateVertexBuffer,          JMP32_SZ));          
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateIndexBuffer,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateIndexBuffer],           (PBYTE)D3D9_IDirect3DDevice9_CreateIndexBuffer,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateRenderTarget,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateRenderTarget],          (PBYTE)D3D9_IDirect3DDevice9_CreateRenderTarget,          JMP32_SZ));          
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface,   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateDepthStencilSurface],   (PBYTE)D3D9_IDirect3DDevice9_CreateDepthStencilSurface,   JMP32_SZ));   
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::UpdateSurface,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateSurface],               (PBYTE)D3D9_IDirect3DDevice9_UpdateSurface,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::UpdateTexture,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::UpdateTexture],               (PBYTE)D3D9_IDirect3DDevice9_UpdateTexture,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderTargetData,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTargetData],         (PBYTE)D3D9_IDirect3DDevice9_GetRenderTargetData,         JMP32_SZ));         
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetFrontBufferData,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFrontBufferData],          (PBYTE)D3D9_IDirect3DDevice9_GetFrontBufferData,          JMP32_SZ));          
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::StretchRect,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::StretchRect],                 (PBYTE)D3D9_IDirect3DDevice9_StretchRect,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ColorFill,                   (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ColorFill],                   (PBYTE)D3D9_IDirect3DDevice9_ColorFill,                   JMP32_SZ));                   
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateOffscreenPlainSurface], (PBYTE)D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface, JMP32_SZ)); 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetRenderTarget,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderTarget],             (PBYTE)D3D9_IDirect3DDevice9_SetRenderTarget,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderTarget,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderTarget],             (PBYTE)D3D9_IDirect3DDevice9_GetRenderTarget,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetDepthStencilSurface],      (PBYTE)D3D9_IDirect3DDevice9_SetDepthStencilSurface,      JMP32_SZ));      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetDepthStencilSurface],      (PBYTE)D3D9_IDirect3DDevice9_GetDepthStencilSurface,      JMP32_SZ));      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::BeginScene,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginScene],                  (PBYTE)D3D9_IDirect3DDevice9_BeginScene,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EndScene,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndScene],                    (PBYTE)D3D9_IDirect3DDevice9_EndScene,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::Clear,                       (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::Clear],                       (PBYTE)D3D9_IDirect3DDevice9_Clear,                       JMP32_SZ));                       
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTransform,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTransform],                (PBYTE)D3D9_IDirect3DDevice9_SetTransform,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTransform,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTransform],                (PBYTE)D3D9_IDirect3DDevice9_GetTransform,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::MultiplyTransform,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::MultiplyTransform],           (PBYTE)D3D9_IDirect3DDevice9_MultiplyTransform,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetViewport,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetViewport],                 (PBYTE)D3D9_IDirect3DDevice9_SetViewport,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetViewport,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetViewport],                 (PBYTE)D3D9_IDirect3DDevice9_GetViewport,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetMaterial,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetMaterial],                 (PBYTE)D3D9_IDirect3DDevice9_SetMaterial,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetMaterial,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetMaterial],                 (PBYTE)D3D9_IDirect3DDevice9_GetMaterial,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetLight,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetLight],                    (PBYTE)D3D9_IDirect3DDevice9_SetLight,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetLight,                    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLight],                    (PBYTE)D3D9_IDirect3DDevice9_GetLight,                    JMP32_SZ));                    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::LightEnable,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::LightEnable],                 (PBYTE)D3D9_IDirect3DDevice9_LightEnable,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetLightEnable,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetLightEnable],              (PBYTE)D3D9_IDirect3DDevice9_GetLightEnable,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetClipPlane,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipPlane],                (PBYTE)D3D9_IDirect3DDevice9_SetClipPlane,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetClipPlane,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipPlane],                (PBYTE)D3D9_IDirect3DDevice9_GetClipPlane,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetRenderState,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetRenderState],              (PBYTE)D3D9_IDirect3DDevice9_SetRenderState,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetRenderState,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetRenderState],              (PBYTE)D3D9_IDirect3DDevice9_GetRenderState,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateStateBlock,            (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateStateBlock],            (PBYTE)D3D9_IDirect3DDevice9_CreateStateBlock,            JMP32_SZ));            
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::BeginStateBlock,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::BeginStateBlock],             (PBYTE)D3D9_IDirect3DDevice9_BeginStateBlock,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::EndStateBlock,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::EndStateBlock],               (PBYTE)D3D9_IDirect3DDevice9_EndStateBlock,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetClipStatus,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetClipStatus],               (PBYTE)D3D9_IDirect3DDevice9_SetClipStatus,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetClipStatus,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetClipStatus],               (PBYTE)D3D9_IDirect3DDevice9_GetClipStatus,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTexture,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTexture],                  (PBYTE)D3D9_IDirect3DDevice9_GetTexture,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTexture,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTexture],                  (PBYTE)D3D9_IDirect3DDevice9_SetTexture,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetTextureStageState,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetTextureStageState],        (PBYTE)D3D9_IDirect3DDevice9_GetTextureStageState,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetTextureStageState,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetTextureStageState],        (PBYTE)D3D9_IDirect3DDevice9_SetTextureStageState,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSamplerState,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSamplerState],             (PBYTE)D3D9_IDirect3DDevice9_GetSamplerState,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetSamplerState,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSamplerState],             (PBYTE)D3D9_IDirect3DDevice9_SetSamplerState,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ValidateDevice,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ValidateDevice],              (PBYTE)D3D9_IDirect3DDevice9_ValidateDevice,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPaletteEntries,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPaletteEntries],           (PBYTE)D3D9_IDirect3DDevice9_SetPaletteEntries,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPaletteEntries,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPaletteEntries],           (PBYTE)D3D9_IDirect3DDevice9_GetPaletteEntries,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetCurrentTexturePalette],    (PBYTE)D3D9_IDirect3DDevice9_SetCurrentTexturePalette,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetCurrentTexturePalette],    (PBYTE)D3D9_IDirect3DDevice9_GetCurrentTexturePalette,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetScissorRect,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetScissorRect],              (PBYTE)D3D9_IDirect3DDevice9_SetScissorRect,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetScissorRect,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetScissorRect],              (PBYTE)D3D9_IDirect3DDevice9_GetScissorRect,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetSoftwareVertexProcessing], (PBYTE)D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing, JMP32_SZ)); 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing, (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetSoftwareVertexProcessing], (PBYTE)D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing, JMP32_SZ)); 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetNPatchMode,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetNPatchMode],               (PBYTE)D3D9_IDirect3DDevice9_SetNPatchMode,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetNPatchMode,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetNPatchMode],               (PBYTE)D3D9_IDirect3DDevice9_GetNPatchMode,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawPrimitive,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitive],               (PBYTE)D3D9_IDirect3DDevice9_DrawPrimitive,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitive],        (PBYTE)D3D9_IDirect3DDevice9_DrawIndexedPrimitive,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawPrimitiveUP],             (PBYTE)D3D9_IDirect3DDevice9_DrawPrimitiveUP,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP,      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawIndexedPrimitiveUP],      (PBYTE)D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP,      JMP32_SZ));      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::ProcessVertices,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::ProcessVertices],             (PBYTE)D3D9_IDirect3DDevice9_ProcessVertices,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexDeclaration],     (PBYTE)D3D9_IDirect3DDevice9_CreateVertexDeclaration,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexDeclaration,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexDeclaration],        (PBYTE)D3D9_IDirect3DDevice9_SetVertexDeclaration,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexDeclaration,        (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexDeclaration],        (PBYTE)D3D9_IDirect3DDevice9_GetVertexDeclaration,        JMP32_SZ));        
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetFVF,                      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetFVF],                      (PBYTE)D3D9_IDirect3DDevice9_SetFVF,                      JMP32_SZ));                      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetFVF,                      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetFVF],                      (PBYTE)D3D9_IDirect3DDevice9_GetFVF,                      JMP32_SZ));                      
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateVertexShader,          (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateVertexShader],          (PBYTE)D3D9_IDirect3DDevice9_CreateVertexShader,          JMP32_SZ));          
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShader,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShader],             (PBYTE)D3D9_IDirect3DDevice9_SetVertexShader,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShader,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShader],             (PBYTE)D3D9_IDirect3DDevice9_GetVertexShader,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantF],    (PBYTE)D3D9_IDirect3DDevice9_SetVertexShaderConstantF,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantF],    (PBYTE)D3D9_IDirect3DDevice9_GetVertexShaderConstantF,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantI],    (PBYTE)D3D9_IDirect3DDevice9_SetVertexShaderConstantI,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantI],    (PBYTE)D3D9_IDirect3DDevice9_GetVertexShaderConstantI,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetVertexShaderConstantB],    (PBYTE)D3D9_IDirect3DDevice9_SetVertexShaderConstantB,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB,    (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetVertexShaderConstantB],    (PBYTE)D3D9_IDirect3DDevice9_GetVertexShaderConstantB,    JMP32_SZ));    
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetStreamSource,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSource],             (PBYTE)D3D9_IDirect3DDevice9_SetStreamSource,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetStreamSource,             (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSource],             (PBYTE)D3D9_IDirect3DDevice9_GetStreamSource,             JMP32_SZ));             
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetStreamSourceFreq],         (PBYTE)D3D9_IDirect3DDevice9_SetStreamSourceFreq,         JMP32_SZ));         
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq,         (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetStreamSourceFreq],         (PBYTE)D3D9_IDirect3DDevice9_GetStreamSourceFreq,         JMP32_SZ));         
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetIndices,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetIndices],                  (PBYTE)D3D9_IDirect3DDevice9_SetIndices,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetIndices,                  (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetIndices],                  (PBYTE)D3D9_IDirect3DDevice9_GetIndices,                  JMP32_SZ));                  
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreatePixelShader,           (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreatePixelShader],           (PBYTE)D3D9_IDirect3DDevice9_CreatePixelShader,           JMP32_SZ));           
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShader,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShader],              (PBYTE)D3D9_IDirect3DDevice9_SetPixelShader,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShader,              (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShader],              (PBYTE)D3D9_IDirect3DDevice9_GetPixelShader,              JMP32_SZ));              
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantF],     (PBYTE)D3D9_IDirect3DDevice9_SetPixelShaderConstantF,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantF],     (PBYTE)D3D9_IDirect3DDevice9_GetPixelShaderConstantF,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantI],     (PBYTE)D3D9_IDirect3DDevice9_SetPixelShaderConstantI,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantI],     (PBYTE)D3D9_IDirect3DDevice9_GetPixelShaderConstantI,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::SetPixelShaderConstantB],     (PBYTE)D3D9_IDirect3DDevice9_SetPixelShaderConstantB,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB,     (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::GetPixelShaderConstantB],     (PBYTE)D3D9_IDirect3DDevice9_GetPixelShaderConstantB,     JMP32_SZ));     
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawRectPatch,               (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawRectPatch],               (PBYTE)D3D9_IDirect3DDevice9_DrawRectPatch,               JMP32_SZ));               
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DrawTriPatch,                (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DrawTriPatch],                (PBYTE)D3D9_IDirect3DDevice9_DrawTriPatch,                JMP32_SZ));                
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::DeletePatch,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::DeletePatch],                 (PBYTE)D3D9_IDirect3DDevice9_DeletePatch,                 JMP32_SZ));                 
	pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IDIRECT3DDEVICE9::CreateQuery,                 (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IDIRECT3DDEVICE9::CreateQuery],                 (PBYTE)D3D9_IDirect3DDevice9_CreateQuery,                 JMP32_SZ));                 
}


#endif