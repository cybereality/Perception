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
#ifndef DCL_IDIRECT3DDEVICE9_CLASS
#define DCL_IDIRECT3DDEVICE9_CLASS

#include <d3d9.h>
#include "VMT_IDirect3DDevice9.h"
#include "AQU_TransferSite.h"

/**
* IDirect3DDevice9 detour class.
***/
class DCL_IDirect3DDevice9
{
public:
	DCL_IDirect3DDevice9(AQU_TransferSite* pcTransferSite, IDirect3D9* pcParentObject);
	~DCL_IDirect3DDevice9();

	/*** IUnknown methods ***/
	HRESULT WINAPI QueryInterface              (IDirect3DDevice9* pcThis, REFIID riid, void** ppvObj);
	ULONG   WINAPI AddRef                      (IDirect3DDevice9* pcThis);
	ULONG   WINAPI Release                     (IDirect3DDevice9* pcThis);

	/*** IDirect3DDevice9 methods ***/
	HRESULT WINAPI TestCooperativeLevel        (IDirect3DDevice9* pcThis);
	UINT    WINAPI GetAvailableTextureMem      (IDirect3DDevice9* pcThis);
	HRESULT WINAPI EvictManagedResources       (IDirect3DDevice9* pcThis);
	HRESULT WINAPI GetDirect3D                 (IDirect3DDevice9* pcThis, IDirect3D9** ppD3D9);
	HRESULT WINAPI GetDeviceCaps               (IDirect3DDevice9* pcThis, D3DCAPS9* pCaps);
	HRESULT WINAPI GetDisplayMode              (IDirect3DDevice9* pcThis, UINT iSwapChain,D3DDISPLAYMODE* pMode);
	HRESULT WINAPI GetCreationParameters       (IDirect3DDevice9* pcThis, D3DDEVICE_CREATION_PARAMETERS *pParameters);
	HRESULT WINAPI SetCursorProperties         (IDirect3DDevice9* pcThis, UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
	void    WINAPI SetCursorPosition           (IDirect3DDevice9* pcThis, int X,int Y,DWORD Flags);
	BOOL    WINAPI ShowCursor                  (IDirect3DDevice9* pcThis, BOOL bShow);
	HRESULT WINAPI CreateAdditionalSwapChain   (IDirect3DDevice9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
	HRESULT WINAPI GetSwapChain                (IDirect3DDevice9* pcThis, UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
	UINT    WINAPI GetNumberOfSwapChains       (IDirect3DDevice9* pcThis);
	HRESULT WINAPI Reset                       (IDirect3DDevice9* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT WINAPI Present                     (IDirect3DDevice9* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	HRESULT WINAPI GetBackBuffer               (IDirect3DDevice9* pcThis, UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
	HRESULT WINAPI GetRasterStatus             (IDirect3DDevice9* pcThis, UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
	HRESULT WINAPI SetDialogBoxMode            (IDirect3DDevice9* pcThis, BOOL bEnableDialogs);
	void    WINAPI SetGammaRamp                (IDirect3DDevice9* pcThis, UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
	void    WINAPI GetGammaRamp                (IDirect3DDevice9* pcThis, UINT iSwapChain,D3DGAMMARAMP* pRamp);
	HRESULT WINAPI CreateTexture               (IDirect3DDevice9* pcThis, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateVolumeTexture         (IDirect3DDevice9* pcThis, UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateCubeTexture           (IDirect3DDevice9* pcThis, UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateVertexBuffer          (IDirect3DDevice9* pcThis, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateIndexBuffer           (IDirect3DDevice9* pcThis, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateRenderTarget          (IDirect3DDevice9* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateDepthStencilSurface   (IDirect3DDevice9* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	HRESULT WINAPI UpdateSurface               (IDirect3DDevice9* pcThis, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
	HRESULT WINAPI UpdateTexture               (IDirect3DDevice9* pcThis, IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
	HRESULT WINAPI GetRenderTargetData         (IDirect3DDevice9* pcThis, IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
	HRESULT WINAPI GetFrontBufferData          (IDirect3DDevice9* pcThis, UINT iSwapChain,IDirect3DSurface9* pDestSurface);
	HRESULT WINAPI StretchRect                 (IDirect3DDevice9* pcThis, IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
	HRESULT WINAPI ColorFill                   (IDirect3DDevice9* pcThis, IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
	HRESULT WINAPI CreateOffscreenPlainSurface (IDirect3DDevice9* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	HRESULT WINAPI SetRenderTarget             (IDirect3DDevice9* pcThis, DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	HRESULT WINAPI GetRenderTarget             (IDirect3DDevice9* pcThis, DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
	HRESULT WINAPI SetDepthStencilSurface      (IDirect3DDevice9* pcThis, IDirect3DSurface9* pNewZStencil);
	HRESULT WINAPI GetDepthStencilSurface      (IDirect3DDevice9* pcThis, IDirect3DSurface9** ppZStencilSurface);
	HRESULT WINAPI BeginScene                  (IDirect3DDevice9* pcThis);
	HRESULT WINAPI EndScene                    (IDirect3DDevice9* pcThis);
	HRESULT WINAPI Clear                       (IDirect3DDevice9* pcThis, DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
	HRESULT WINAPI SetTransform                (IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	HRESULT WINAPI GetTransform                (IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
	HRESULT WINAPI MultiplyTransform           (IDirect3DDevice9* pcThis, D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	HRESULT WINAPI SetViewport                 (IDirect3DDevice9* pcThis, CONST D3DVIEWPORT9* pViewport);
	HRESULT WINAPI GetViewport                 (IDirect3DDevice9* pcThis, D3DVIEWPORT9* pViewport);
	HRESULT WINAPI SetMaterial                 (IDirect3DDevice9* pcThis, CONST D3DMATERIAL9* pMaterial);
	HRESULT WINAPI GetMaterial                 (IDirect3DDevice9* pcThis, D3DMATERIAL9* pMaterial);
	HRESULT WINAPI SetLight                    (IDirect3DDevice9* pcThis, DWORD Index,CONST D3DLIGHT9* pLight);
	HRESULT WINAPI GetLight                    (IDirect3DDevice9* pcThis, DWORD Index,D3DLIGHT9* pLight);
	HRESULT WINAPI LightEnable                 (IDirect3DDevice9* pcThis, DWORD Index,BOOL Enable);
	HRESULT WINAPI GetLightEnable              (IDirect3DDevice9* pcThis, DWORD Index,BOOL* pEnable);
	HRESULT WINAPI SetClipPlane                (IDirect3DDevice9* pcThis, DWORD Index,CONST float* pPlane);
	HRESULT WINAPI GetClipPlane                (IDirect3DDevice9* pcThis, DWORD Index,float* pPlane);
	HRESULT WINAPI SetRenderState              (IDirect3DDevice9* pcThis, D3DRENDERSTATETYPE State,DWORD Value);
	HRESULT WINAPI GetRenderState              (IDirect3DDevice9* pcThis, D3DRENDERSTATETYPE State,DWORD* pValue);
	HRESULT WINAPI CreateStateBlock            (IDirect3DDevice9* pcThis, D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
	HRESULT WINAPI BeginStateBlock             (IDirect3DDevice9* pcThis);
	HRESULT WINAPI EndStateBlock               (IDirect3DDevice9* pcThis, IDirect3DStateBlock9** ppSB);
	HRESULT WINAPI SetClipStatus               (IDirect3DDevice9* pcThis, CONST D3DCLIPSTATUS9* pClipStatus);
	HRESULT WINAPI GetClipStatus               (IDirect3DDevice9* pcThis, D3DCLIPSTATUS9* pClipStatus);
	HRESULT WINAPI GetTexture                  (IDirect3DDevice9* pcThis, DWORD Stage,IDirect3DBaseTexture9** ppTexture);
	HRESULT WINAPI SetTexture                  (IDirect3DDevice9* pcThis, DWORD Stage,IDirect3DBaseTexture9* pTexture);
	HRESULT WINAPI GetTextureStageState        (IDirect3DDevice9* pcThis, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
	HRESULT WINAPI SetTextureStageState        (IDirect3DDevice9* pcThis, DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
	HRESULT WINAPI GetSamplerState             (IDirect3DDevice9* pcThis, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
	HRESULT WINAPI SetSamplerState             (IDirect3DDevice9* pcThis, DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
	HRESULT WINAPI ValidateDevice              (IDirect3DDevice9* pcThis, DWORD* pNumPasses);
	HRESULT WINAPI SetPaletteEntries           (IDirect3DDevice9* pcThis, UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
	HRESULT WINAPI GetPaletteEntries           (IDirect3DDevice9* pcThis, UINT PaletteNumber,PALETTEENTRY* pEntries);
	HRESULT WINAPI SetCurrentTexturePalette    (IDirect3DDevice9* pcThis, UINT PaletteNumber);
	HRESULT WINAPI GetCurrentTexturePalette    (IDirect3DDevice9* pcThis, UINT *PaletteNumber);
	HRESULT WINAPI SetScissorRect              (IDirect3DDevice9* pcThis, CONST RECT* pRect);
	HRESULT WINAPI GetScissorRect              (IDirect3DDevice9* pcThis, RECT* pRect);
	HRESULT WINAPI SetSoftwareVertexProcessing (IDirect3DDevice9* pcThis, BOOL bSoftware);
	BOOL    WINAPI GetSoftwareVertexProcessing (IDirect3DDevice9* pcThis);
	HRESULT WINAPI SetNPatchMode               (IDirect3DDevice9* pcThis, float nSegments);
	float   WINAPI GetNPatchMode               (IDirect3DDevice9* pcThis);
	HRESULT WINAPI DrawPrimitive               (IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	HRESULT WINAPI DrawIndexedPrimitive        (IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	HRESULT WINAPI DrawPrimitiveUP             (IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	HRESULT WINAPI DrawIndexedPrimitiveUP      (IDirect3DDevice9* pcThis, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	HRESULT WINAPI ProcessVertices             (IDirect3DDevice9* pcThis, UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
	HRESULT WINAPI CreateVertexDeclaration     (IDirect3DDevice9* pcThis, CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
	HRESULT WINAPI SetVertexDeclaration        (IDirect3DDevice9* pcThis, IDirect3DVertexDeclaration9* pDecl);
	HRESULT WINAPI GetVertexDeclaration        (IDirect3DDevice9* pcThis, IDirect3DVertexDeclaration9** ppDecl);
	HRESULT WINAPI SetFVF                      (IDirect3DDevice9* pcThis, DWORD FVF);
	HRESULT WINAPI GetFVF                      (IDirect3DDevice9* pcThis, DWORD* pFVF);
	HRESULT WINAPI CreateVertexShader          (IDirect3DDevice9* pcThis, CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
	HRESULT WINAPI SetVertexShader             (IDirect3DDevice9* pcThis, IDirect3DVertexShader9* pShader);
	HRESULT WINAPI GetVertexShader             (IDirect3DDevice9* pcThis, IDirect3DVertexShader9** ppShader);
	HRESULT WINAPI SetVertexShaderConstantF    (IDirect3DDevice9* pcThis, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	HRESULT WINAPI GetVertexShaderConstantF    (IDirect3DDevice9* pcThis, UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	HRESULT WINAPI SetVertexShaderConstantI    (IDirect3DDevice9* pcThis, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	HRESULT WINAPI GetVertexShaderConstantI    (IDirect3DDevice9* pcThis, UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	HRESULT WINAPI SetVertexShaderConstantB    (IDirect3DDevice9* pcThis, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
	HRESULT WINAPI GetVertexShaderConstantB    (IDirect3DDevice9* pcThis, UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	HRESULT WINAPI SetStreamSource             (IDirect3DDevice9* pcThis, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
	HRESULT WINAPI GetStreamSource             (IDirect3DDevice9* pcThis, UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
	HRESULT WINAPI SetStreamSourceFreq         (IDirect3DDevice9* pcThis, UINT StreamNumber,UINT Setting);
	HRESULT WINAPI GetStreamSourceFreq         (IDirect3DDevice9* pcThis, UINT StreamNumber,UINT* pSetting);
	HRESULT WINAPI SetIndices                  (IDirect3DDevice9* pcThis, IDirect3DIndexBuffer9* pIndexData);
	HRESULT WINAPI GetIndices                  (IDirect3DDevice9* pcThis, IDirect3DIndexBuffer9** ppIndexData);
	HRESULT WINAPI CreatePixelShader           (IDirect3DDevice9* pcThis, CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
	HRESULT WINAPI SetPixelShader              (IDirect3DDevice9* pcThis, IDirect3DPixelShader9* pShader);
	HRESULT WINAPI GetPixelShader              (IDirect3DDevice9* pcThis, IDirect3DPixelShader9** ppShader);
	HRESULT WINAPI SetPixelShaderConstantF     (IDirect3DDevice9* pcThis, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	HRESULT WINAPI GetPixelShaderConstantF     (IDirect3DDevice9* pcThis, UINT StartRegister,float* pConstantData,UINT Vector4fCount);
	HRESULT WINAPI SetPixelShaderConstantI     (IDirect3DDevice9* pcThis, UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	HRESULT WINAPI GetPixelShaderConstantI     (IDirect3DDevice9* pcThis, UINT StartRegister,int* pConstantData,UINT Vector4iCount);
	HRESULT WINAPI SetPixelShaderConstantB     (IDirect3DDevice9* pcThis, UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
	HRESULT WINAPI GetPixelShaderConstantB     (IDirect3DDevice9* pcThis, UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
	HRESULT WINAPI DrawRectPatch               (IDirect3DDevice9* pcThis, UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	HRESULT WINAPI DrawTriPatch                (IDirect3DDevice9* pcThis, UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	HRESULT WINAPI DeletePatch                 (IDirect3DDevice9* pcThis, UINT Handle);
	HRESULT WINAPI CreateQuery                 (IDirect3DDevice9* pcThis, D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

	/*** DCL_IDirect3DDevice9 public methods ***/
	HRESULT        SetSuperFunctionPointer     (VMT_IUNKNOWN::VMT_IUnknown eFunc, UINT_PTR dwFunc);
	HRESULT        SetSuperFunctionPointer     (VMT_IDIRECT3DDEVICE9::VMT_IDirect3DDevice9 eFunc, UINT_PTR dwFunc);
	HRESULT        SetSuperFunctionPointers    (PUINT_PTR pVMTable);

	/**
	* The transfer site.
	***/
	AQU_TransferSite* m_pcTransferSite;
	/**
	* The currently set vertex shader.
	***/
	IDirect3DVertexShader9* m_pcVShaderCurrent;
	/**
	* The currently set FVF.
	***/
	DWORD m_unFVF;
	/**
	* True if last method was a drawing method.
	***/
	bool m_bDrawingActive;
	/**
	* The D3D9 object creating this device.
	***/
	IDirect3D9* m_pcParentObject;
};

#endif