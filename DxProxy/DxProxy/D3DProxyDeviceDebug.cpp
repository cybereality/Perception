/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDeviceDebug.cpp> and
Class <D3DProxyDeviceDebug> :
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

#include "D3DProxyDeviceDebug.h"

/**
* Constructor, opens the dump file.
* @param pDevice Imbed actual device.
* @param pCreatedBy Pointer to the object that created the device.
***/
D3DProxyDeviceDebug::D3DProxyDeviceDebug(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy) : D3DProxyDevice(pDevice, pCreatedBy)
{
	m_logFile.open("logD3DProxyDevice.txt", std::ios::out);
	m_logFile << "D3DProxyDevice history:__________" << std::endl;
}

/**
* Destructor, closes the dump file.
***/
D3DProxyDeviceDebug::~D3DProxyDeviceDebug()
{
	m_logFile.close();
}

/**
* Function outputs log file information about method called and if failed.
***/
HRESULT WINAPI D3DProxyDeviceDebug::QueryInterface(REFIID riid, LPVOID* ppv){m_logFile << "QueryInterface" <<std::endl; HRESULT hr = BaseDirect3DDevice9::QueryInterface(riid,ppv);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
ULONG   WINAPI D3DProxyDeviceDebug::AddRef(){m_logFile << "AddRef" <<std::endl; return BaseDirect3DDevice9::AddRef();}
ULONG   WINAPI D3DProxyDeviceDebug::Release(){m_logFile << "Release" <<std::endl; return BaseDirect3DDevice9::Release();}
HRESULT WINAPI D3DProxyDeviceDebug::TestCooperativeLevel(){m_logFile << "TestCooperativeLevel" <<std::endl; HRESULT hr = D3DProxyDevice::TestCooperativeLevel();if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap){m_logFile << "SetCursorProperties" <<std::endl; HRESULT hr = D3DProxyDevice::SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain){m_logFile << "CreateAdditionalSwapChain" <<std::endl; HRESULT hr = D3DProxyDevice::CreateAdditionalSwapChain(pPresentationParameters,pSwapChain);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain){m_logFile << "GetSwapChain" <<std::endl; HRESULT hr = D3DProxyDevice::GetSwapChain(iSwapChain,pSwapChain);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters){m_logFile << "Reset" <<std::endl; HRESULT hr = D3DProxyDevice::Reset(pPresentationParameters);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){m_logFile << "Present" <<std::endl; HRESULT hr = D3DProxyDevice::Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer){m_logFile << "GetBackBuffer" <<std::endl; HRESULT hr = D3DProxyDevice::GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle){m_logFile << "CreateTexture" <<std::endl; HRESULT hr = D3DProxyDevice::CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle){m_logFile << "CreateVolumeTexture" <<std::endl; HRESULT hr = D3DProxyDevice::CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle){m_logFile << "CreateCubeTexture" <<std::endl; HRESULT hr = D3DProxyDevice::CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle){m_logFile << "CreateVertexBuffer" <<std::endl; HRESULT hr = D3DProxyDevice::CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle){m_logFile << "CreateIndexBuffer" <<std::endl; HRESULT hr = D3DProxyDevice::CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){m_logFile << "CreateRenderTarget" <<std::endl; HRESULT hr = D3DProxyDevice::CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){m_logFile << "CreateDepthStencilSurface" <<std::endl; HRESULT hr = D3DProxyDevice::CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint){m_logFile << "UpdateSurface" <<std::endl; HRESULT hr = D3DProxyDevice::UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture){m_logFile << "UpdateTexture" <<std::endl; HRESULT hr = D3DProxyDevice::UpdateTexture(pSourceTexture,pDestinationTexture);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface){m_logFile << "GetRenderTargetData" <<std::endl; HRESULT hr = D3DProxyDevice::GetRenderTargetData(pRenderTarget,pDestSurface);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface){m_logFile << "GetFrontBufferData" <<std::endl; HRESULT hr = D3DProxyDevice::GetFrontBufferData(iSwapChain,pDestSurface);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter){m_logFile << "StretchRect" <<std::endl; HRESULT hr = D3DProxyDevice::StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color){m_logFile << "ColorFill" <<std::endl; HRESULT hr = D3DProxyDevice::ColorFill(pSurface,pRect,color);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle){m_logFile << "CreateOffscreenPlainSurface" <<std::endl; HRESULT hr = D3DProxyDevice::CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget){m_logFile << "SetRenderTarget" <<std::endl; HRESULT hr = D3DProxyDevice::SetRenderTarget(RenderTargetIndex,pRenderTarget);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget){m_logFile << "GetRenderTarget" <<std::endl; HRESULT hr = D3DProxyDevice::GetRenderTarget(RenderTargetIndex,ppRenderTarget);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil){m_logFile << "SetDepthStencilSurface" <<std::endl; HRESULT hr = D3DProxyDevice::SetDepthStencilSurface(pNewZStencil);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface){m_logFile << "GetDepthStencilSurface" <<std::endl; HRESULT hr = D3DProxyDevice::GetDepthStencilSurface(ppZStencilSurface);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::BeginScene(){m_logFile << "BeginScene" <<std::endl; HRESULT hr = D3DProxyDevice::BeginScene();if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::EndScene(){m_logFile << "EndScene" <<std::endl; HRESULT hr = D3DProxyDevice::EndScene();if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil){m_logFile << "Clear" <<std::endl; HRESULT hr = D3DProxyDevice::Clear(Count,pRects,Flags,Color,Z,Stencil);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix){m_logFile << "SetTransform" <<std::endl; HRESULT hr = D3DProxyDevice::SetTransform(State,pMatrix);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix){m_logFile << "MultiplyTransform" <<std::endl; HRESULT hr = D3DProxyDevice::MultiplyTransform(State,pMatrix);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetViewport(CONST D3DVIEWPORT9* pViewport){m_logFile << "SetViewport" <<std::endl; HRESULT hr = D3DProxyDevice::SetViewport(pViewport);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB){m_logFile << "CreateStateBlock" <<std::endl; HRESULT hr = D3DProxyDevice::CreateStateBlock(Type,ppSB);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::BeginStateBlock(){m_logFile << "BeginStateBlock" <<std::endl; HRESULT hr = D3DProxyDevice::BeginStateBlock();if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::EndStateBlock(IDirect3DStateBlock9** ppSB){m_logFile << "EndStateBlock" <<std::endl; HRESULT hr = D3DProxyDevice::EndStateBlock(ppSB);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture){m_logFile << "GetTexture" <<std::endl; HRESULT hr = D3DProxyDevice::GetTexture(Stage,ppTexture);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}	
HRESULT WINAPI D3DProxyDeviceDebug::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture){m_logFile << "SetTexture" <<std::endl; HRESULT hr = D3DProxyDevice::SetTexture(Stage,pTexture);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){m_logFile << "DrawPrimitive" <<std::endl; HRESULT hr = D3DProxyDevice::DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount){m_logFile << "DrawIndexedPrimitive" <<std::endl; HRESULT hr = D3DProxyDevice::DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){m_logFile << "DrawPrimitiveUP" <<std::endl; HRESULT hr = D3DProxyDevice::DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){m_logFile << "DrawIndexedPrimitiveUP" <<std::endl; HRESULT hr = D3DProxyDevice::DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount,pIndexData,IndexDataFormat,pVertexStreamZeroData,VertexStreamZeroStride);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags){m_logFile << "ProcessVertices" <<std::endl; HRESULT hr = D3DProxyDevice::ProcessVertices(SrcStartIndex,DestIndex,VertexCount,pDestBuffer,pVertexDecl,Flags);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl){m_logFile << "CreateVertexDeclaration" <<std::endl; HRESULT hr = D3DProxyDevice::CreateVertexDeclaration(pVertexElements,ppDecl);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl){m_logFile << "SetVertexDeclaration" <<std::endl; HRESULT hr = D3DProxyDevice::SetVertexDeclaration(pDecl);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl){m_logFile << "GetVertexDeclaration" <<std::endl; HRESULT hr = D3DProxyDevice::GetVertexDeclaration(ppDecl);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader){m_logFile << "CreateVertexShader" <<std::endl; HRESULT hr = D3DProxyDevice::CreateVertexShader(pFunction,ppShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetVertexShader(IDirect3DVertexShader9* pShader){m_logFile << "SetVertexShader" <<std::endl; HRESULT hr = D3DProxyDevice::SetVertexShader(pShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetVertexShader(IDirect3DVertexShader9** ppShader){m_logFile << "GetVertexShader" <<std::endl; HRESULT hr = D3DProxyDevice::GetVertexShader(ppShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){m_logFile << "SetVertexShaderConstantF" <<std::endl; HRESULT hr = D3DProxyDevice::SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetVertexShaderConstantF(UINT StartRegister,float* pData, UINT Vector4fCount){m_logFile << "GetVertexShaderConstantF" <<std::endl; HRESULT hr = D3DProxyDevice::GetVertexShaderConstantF(StartRegister,pData,Vector4fCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride){m_logFile << "SetStreamSource" <<std::endl; HRESULT hr = D3DProxyDevice::SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride){m_logFile << "GetStreamSource" <<std::endl; HRESULT hr = D3DProxyDevice::GetStreamSource(StreamNumber,ppStreamData,pOffsetInBytes,pStride);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetIndices(IDirect3DIndexBuffer9* pIndexData){m_logFile << "SetIndices" <<std::endl; HRESULT hr = D3DProxyDevice::SetIndices(pIndexData);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetIndices(IDirect3DIndexBuffer9** ppIndexData){m_logFile << "GetIndices" <<std::endl; HRESULT hr = D3DProxyDevice::GetIndices(ppIndexData);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader){m_logFile << "CreatePixelShader" <<std::endl; HRESULT hr = D3DProxyDevice::CreatePixelShader(pFunction,ppShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetPixelShader(IDirect3DPixelShader9* pShader){m_logFile << "SetPixelShader" <<std::endl; HRESULT hr = D3DProxyDevice::SetPixelShader(pShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetPixelShader(IDirect3DPixelShader9** ppShader){m_logFile << "GetPixelShader" <<std::endl; HRESULT hr = D3DProxyDevice::GetPixelShader(ppShader);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount){m_logFile << "SetPixelShaderConstantF" <<std::endl; HRESULT hr = D3DProxyDevice::SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::GetPixelShaderConstantF(UINT StartRegister,float* pData, UINT Vector4fCount){m_logFile << "GetPixelShaderConstantF" <<std::endl; HRESULT hr = D3DProxyDevice::GetPixelShaderConstantF(StartRegister,pData,Vector4fCount);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo){m_logFile << "DrawRectPatch" <<std::endl; HRESULT hr = D3DProxyDevice::DrawRectPatch(Handle,pNumSegs,pRectPatchInfo);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo){m_logFile << "DrawTriPatch" <<std::endl; HRESULT hr = D3DProxyDevice::DrawTriPatch(Handle,pNumSegs,pTriPatchInfo);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}
HRESULT WINAPI D3DProxyDeviceDebug::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery){m_logFile << "CreateQuery" <<std::endl; HRESULT hr = D3DProxyDevice::CreateQuery(Type,ppQuery);if (hr!=S_OK) m_logFile << "ERROR" <<std::endl; return hr;}