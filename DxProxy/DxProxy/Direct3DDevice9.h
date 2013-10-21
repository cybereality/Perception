/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DDevice9.h> and
Class <BaseDirect3DDevice9> :
Copyright (C) 2012 Andres Hernandez

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

#ifndef DIRECT3DDEVICE9_H_INCLUDED
#define DIRECT3DDEVICE9_H_INCLUDED

#include <d3d9.h>
#include "Direct3D9.h"

/**
*  Direct 3D device class. 
*  Overwrites IDirect3DDevice9 and imbeds the actual device pointer.
*/
class BaseDirect3DDevice9 : public IDirect3DDevice9
{
public:
	BaseDirect3DDevice9(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~BaseDirect3DDevice9();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();

	/*** IDirect3DDevice9 methods ***/
	virtual HRESULT WINAPI TestCooperativeLevel();
	virtual UINT    WINAPI GetAvailableTextureMem();
	virtual HRESULT WINAPI EvictManagedResources();
	virtual HRESULT WINAPI GetDirect3D(IDirect3D9** ppD3D9);
	virtual HRESULT WINAPI GetDeviceCaps(D3DCAPS9* pCaps);
	virtual HRESULT WINAPI GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode);
	virtual HRESULT WINAPI GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters);
	virtual HRESULT WINAPI SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
	virtual void    WINAPI SetCursorPosition(int X,int Y,DWORD Flags);
	virtual BOOL    WINAPI ShowCursor(BOOL bShow);
	virtual HRESULT WINAPI CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
	virtual HRESULT WINAPI GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
	virtual UINT    WINAPI GetNumberOfSwapChains();
	virtual HRESULT WINAPI Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
	virtual HRESULT WINAPI GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
	virtual HRESULT WINAPI SetDialogBoxMode(BOOL bEnableDialogs);
	virtual void    WINAPI SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
	virtual void    WINAPI GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp);
	virtual HRESULT WINAPI CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
	virtual HRESULT WINAPI UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
	virtual HRESULT WINAPI GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
	virtual HRESULT WINAPI ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
	virtual HRESULT WINAPI CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	virtual HRESULT WINAPI GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
	virtual HRESULT WINAPI SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
	virtual HRESULT WINAPI GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
	virtual HRESULT WINAPI BeginScene();
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
	virtual HRESULT WINAPI SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI SetViewport(CONST D3DVIEWPORT9* pViewport);
	virtual HRESULT WINAPI GetViewport(D3DVIEWPORT9* pViewport);
	virtual HRESULT WINAPI SetMaterial(CONST D3DMATERIAL9* pMaterial);
	virtual HRESULT WINAPI GetMaterial(D3DMATERIAL9* pMaterial);
	virtual HRESULT WINAPI SetLight(DWORD Index,CONST D3DLIGHT9* pLight);
	virtual HRESULT WINAPI GetLight(DWORD Index,D3DLIGHT9* pLight);
	virtual HRESULT WINAPI LightEnable(DWORD Index,BOOL Enable);
	virtual HRESULT WINAPI GetLightEnable(DWORD Index,BOOL* pEnable);
	virtual HRESULT WINAPI SetClipPlane(DWORD Index,CONST float* pPlane);
	virtual HRESULT WINAPI GetClipPlane(DWORD Index,float* pPlane);
	virtual HRESULT WINAPI SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
	virtual HRESULT WINAPI GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue);
	virtual HRESULT WINAPI CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI BeginStateBlock();
	virtual HRESULT WINAPI EndStateBlock(IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus);
	virtual HRESULT WINAPI GetClipStatus(D3DCLIPSTATUS9* pClipStatus);
	virtual HRESULT WINAPI GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture);
	virtual HRESULT WINAPI SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture);
	virtual HRESULT WINAPI GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
	virtual HRESULT WINAPI SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
	virtual HRESULT WINAPI GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
	virtual HRESULT WINAPI SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
	virtual HRESULT WINAPI ValidateDevice(DWORD* pNumPasses);
	virtual HRESULT WINAPI SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
	virtual HRESULT WINAPI GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries);
	virtual HRESULT WINAPI SetCurrentTexturePalette(UINT PaletteNumber);
	virtual HRESULT WINAPI GetCurrentTexturePalette(UINT *PaletteNumber);
	virtual HRESULT WINAPI SetScissorRect(CONST RECT* pRect);
	virtual HRESULT WINAPI GetScissorRect(RECT* pRect);
	virtual HRESULT WINAPI SetSoftwareVertexProcessing(BOOL bSoftware);
	virtual BOOL    WINAPI GetSoftwareVertexProcessing();
	virtual HRESULT WINAPI SetNPatchMode(float nSegments);
	virtual float   WINAPI GetNPatchMode();
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual HRESULT WINAPI DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
	virtual HRESULT WINAPI CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
	virtual HRESULT WINAPI GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI SetFVF(DWORD FVF);
	virtual HRESULT WINAPI GetFVF(DWORD* pFVF);
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShader(IDirect3DVertexShader9* pShader);
	virtual HRESULT WINAPI GetVertexShader(IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	virtual HRESULT WINAPI GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	virtual HRESULT WINAPI SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
	virtual HRESULT WINAPI GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	virtual HRESULT WINAPI SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
	virtual HRESULT WINAPI GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
	virtual HRESULT WINAPI SetStreamSourceFreq(UINT StreamNumber,UINT Setting);
	virtual HRESULT WINAPI GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting);
	virtual HRESULT WINAPI SetIndices(IDirect3DIndexBuffer9* pIndexData);
	virtual HRESULT WINAPI GetIndices(IDirect3DIndexBuffer9** ppIndexData);
	virtual HRESULT WINAPI CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetPixelShader(IDirect3DPixelShader9* pShader);
	virtual HRESULT WINAPI GetPixelShader(IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	virtual HRESULT WINAPI GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	virtual HRESULT WINAPI SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
	virtual HRESULT WINAPI GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	virtual HRESULT WINAPI DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	virtual HRESULT WINAPI DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	virtual HRESULT WINAPI DeletePatch(UINT Handle);
	virtual HRESULT WINAPI CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

	/*** BaseDirect3DDevice9 methods ***/
	IDirect3DDevice9* getActual();

private:
	/**
	* Actual Direct3D Device pointer embedded. 
	* Private to force you to think about whether you really need direct 
	* access to the actual device. Can be accessed via getActual(). 
	***/
	IDirect3DDevice9* m_pDevice;
	/**
	* Pointer to the D3D object that created the device. 
	***/
	BaseDirect3D9* m_pCreatedBy;
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
#ifdef _EXPORT_LOGFILE
	/**
	* The log file (.txt format).
	* Outputs D3D device history.
	* (define _EXPORT_LOGFILE to use this)
	***/
	std::ofstream m_logFile;
#endif
};

#endif
