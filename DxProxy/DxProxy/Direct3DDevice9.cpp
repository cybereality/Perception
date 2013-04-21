/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#include "Direct3DDevice9.h"
#include "Main.h"

BaseDirect3DDevice9::BaseDirect3DDevice9(IDirect3DDevice9* pDevice) :
	m_pDevice(pDevice),
	m_nRefCount(1)
{
}

BaseDirect3DDevice9::~BaseDirect3DDevice9()
{
	if(m_pDevice)
		m_pDevice->Release();
}

HRESULT WINAPI BaseDirect3DDevice9::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pDevice->QueryInterface(riid, ppv);
}

ULONG WINAPI BaseDirect3DDevice9::AddRef()
{
	return ++m_nRefCount;
}

ULONG WINAPI BaseDirect3DDevice9::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_nRefCount;
}

HRESULT WINAPI BaseDirect3DDevice9::TestCooperativeLevel()
{
	return m_pDevice->TestCooperativeLevel();
}

UINT WINAPI BaseDirect3DDevice9::GetAvailableTextureMem()
{
	return m_pDevice->GetAvailableTextureMem();
}

HRESULT WINAPI BaseDirect3DDevice9::EvictManagedResources()
{
	return m_pDevice->EvictManagedResources();
}

HRESULT WINAPI BaseDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	return m_pDevice->GetDirect3D(ppD3D9);
}

HRESULT WINAPI BaseDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return m_pDevice->GetDeviceCaps(pCaps);
}

HRESULT WINAPI BaseDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetDisplayMode(iSwapChain, pMode);
}

HRESULT WINAPI BaseDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pDevice->GetCreationParameters(pParameters);
}

HRESULT WINAPI BaseDirect3DDevice9::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void WINAPI BaseDirect3DDevice9::SetCursorPosition(int X,int Y,DWORD Flags)
{
	return m_pDevice->SetCursorPosition(X, Y, Flags);
}

BOOL WINAPI BaseDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return m_pDevice->ShowCursor(bShow);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	return m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT WINAPI BaseDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pDevice->GetSwapChain(iSwapChain, pSwapChain);
}

UINT WINAPI BaseDirect3DDevice9::GetNumberOfSwapChains()
{
	return m_pDevice->GetNumberOfSwapChains();
}

HRESULT WINAPI BaseDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return m_pDevice->Reset(pPresentationParameters);
}

HRESULT WINAPI BaseDirect3DDevice9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	return m_pDevice->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT WINAPI BaseDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	return m_pDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT WINAPI BaseDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT WINAPI BaseDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return m_pDevice->SetDialogBoxMode(bEnableDialogs);
}

void WINAPI BaseDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	return m_pDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void WINAPI BaseDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	return m_pDevice->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	return m_pDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT WINAPI BaseDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	return m_pDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT WINAPI BaseDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	return m_pDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT WINAPI BaseDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	return m_pDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT WINAPI BaseDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	return m_pDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT WINAPI BaseDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	return m_pDevice->ColorFill(pSurface, pRect, color);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT WINAPI BaseDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	return m_pDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT WINAPI BaseDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	return m_pDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT WINAPI BaseDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pDevice->SetDepthStencilSurface(pNewZStencil);
}

HRESULT WINAPI BaseDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return m_pDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT WINAPI BaseDirect3DDevice9::BeginScene()
{
	return m_pDevice->BeginScene();
}

HRESULT WINAPI BaseDirect3DDevice9::EndScene()
{
	return m_pDevice->EndScene();
}

HRESULT WINAPI BaseDirect3DDevice9::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT WINAPI BaseDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->SetTransform(State, pMatrix);
}

HRESULT WINAPI BaseDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	return m_pDevice->GetTransform(State, pMatrix);
}

HRESULT WINAPI BaseDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->MultiplyTransform(State, pMatrix);
}

HRESULT WINAPI BaseDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	return m_pDevice->SetViewport(pViewport);
}

HRESULT WINAPI BaseDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
	return m_pDevice->GetViewport(pViewport);
}

HRESULT WINAPI BaseDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return m_pDevice->SetMaterial(pMaterial);
}

HRESULT WINAPI BaseDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return m_pDevice->GetMaterial(pMaterial);
}

HRESULT WINAPI BaseDirect3DDevice9::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
	return m_pDevice->SetLight(Index, pLight);
}

HRESULT WINAPI BaseDirect3DDevice9::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
	return m_pDevice->GetLight(Index, pLight);
}

HRESULT WINAPI BaseDirect3DDevice9::LightEnable(DWORD Index,BOOL Enable)
{
	return m_pDevice->LightEnable(Index, Enable);
}

HRESULT WINAPI BaseDirect3DDevice9::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	return m_pDevice->GetLightEnable(Index, pEnable);
}

HRESULT WINAPI BaseDirect3DDevice9::SetClipPlane(DWORD Index,CONST float* pPlane)
{
	return m_pDevice->SetClipPlane(Index, pPlane);
}

HRESULT WINAPI BaseDirect3DDevice9::GetClipPlane(DWORD Index,float* pPlane)
{
	return m_pDevice->GetClipPlane(Index, pPlane);
}

HRESULT WINAPI BaseDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	return m_pDevice->SetRenderState(State, Value);
}

HRESULT WINAPI BaseDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
	return m_pDevice->GetRenderState(State, pValue);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	return m_pDevice->CreateStateBlock(Type, ppSB);
}

HRESULT WINAPI BaseDirect3DDevice9::BeginStateBlock()
{
	return m_pDevice->BeginStateBlock();
}

HRESULT WINAPI BaseDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pDevice->EndStateBlock(ppSB);
}

HRESULT WINAPI BaseDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->SetClipStatus(pClipStatus);
}

HRESULT WINAPI BaseDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->GetClipStatus(pClipStatus);
}

HRESULT WINAPI BaseDirect3DDevice9::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	return m_pDevice->GetTexture(Stage, ppTexture);
}

HRESULT WINAPI BaseDirect3DDevice9::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	return m_pDevice->SetTexture(Stage, pTexture);
}

HRESULT WINAPI BaseDirect3DDevice9::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	return m_pDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT WINAPI BaseDirect3DDevice9::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	return m_pDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT WINAPI BaseDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	return m_pDevice->GetSamplerState(Sampler, Type, pValue);
}

HRESULT WINAPI BaseDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	return m_pDevice->SetSamplerState(Sampler, Type, Value);
}

HRESULT WINAPI BaseDirect3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
	return m_pDevice->ValidateDevice(pNumPasses);
}

HRESULT WINAPI BaseDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT WINAPI BaseDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT WINAPI BaseDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT WINAPI BaseDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	return m_pDevice->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT WINAPI BaseDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	return m_pDevice->SetScissorRect(pRect);
}

HRESULT WINAPI BaseDirect3DDevice9::GetScissorRect(RECT* pRect)
{
	return m_pDevice->GetScissorRect(pRect);
}

HRESULT WINAPI BaseDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return m_pDevice->SetSoftwareVertexProcessing(bSoftware);
}

BOOL WINAPI BaseDirect3DDevice9::GetSoftwareVertexProcessing()
{
	return m_pDevice->GetSoftwareVertexProcessing();
}

HRESULT WINAPI BaseDirect3DDevice9::SetNPatchMode(float nSegments)
{
	return m_pDevice->SetNPatchMode(nSegments);
}

float WINAPI BaseDirect3DDevice9::GetNPatchMode()
{
	return m_pDevice->GetNPatchMode();
}

HRESULT WINAPI BaseDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	return m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT WINAPI BaseDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	return m_pDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI BaseDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT WINAPI BaseDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT WINAPI BaseDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT WINAPI BaseDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return m_pDevice->SetVertexDeclaration(pDecl);
}

HRESULT WINAPI BaseDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDevice->GetVertexDeclaration(ppDecl);
}

HRESULT WINAPI BaseDirect3DDevice9::SetFVF(DWORD FVF)
{
	return m_pDevice->SetFVF(FVF);
}

HRESULT WINAPI BaseDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	return m_pDevice->GetFVF(pFVF);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	return m_pDevice->CreateVertexShader(pFunction, ppShader);
}

HRESULT WINAPI BaseDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	return m_pDevice->SetVertexShader(pShader);
}

HRESULT WINAPI BaseDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return m_pDevice->GetVertexShader(ppShader);
}

HRESULT WINAPI BaseDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI BaseDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT WINAPI BaseDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	return m_pDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT WINAPI BaseDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	return m_pDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT WINAPI BaseDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
{
	return m_pDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT WINAPI BaseDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Setting)
{
	return m_pDevice->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT WINAPI BaseDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting)
{
	return m_pDevice->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT WINAPI BaseDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pDevice->SetIndices(pIndexData);
}

HRESULT WINAPI BaseDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pDevice->GetIndices(ppIndexData);
}

HRESULT WINAPI BaseDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	return m_pDevice->CreatePixelShader(pFunction, ppShader);
}

HRESULT WINAPI BaseDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return m_pDevice->SetPixelShader(pShader);
}

HRESULT WINAPI BaseDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return m_pDevice->GetPixelShader(ppShader);
}

HRESULT WINAPI BaseDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	return m_pDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI BaseDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT WINAPI BaseDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	return m_pDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT WINAPI BaseDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT WINAPI BaseDirect3DDevice9::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT WINAPI BaseDirect3DDevice9::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT WINAPI BaseDirect3DDevice9::DeletePatch(UINT Handle)
{
	return m_pDevice->DeletePatch(Handle);
}

HRESULT WINAPI BaseDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	return m_pDevice->CreateQuery(Type, ppQuery);
}