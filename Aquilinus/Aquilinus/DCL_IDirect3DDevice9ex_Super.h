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
#ifndef DCL_IDIRECT3DDEVICE9EX_SUPER
#define DCL_IDIRECT3DDEVICE9EX_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D929_QueryInterface)              (IDirect3DDevice9Ex* pcThis,REFIID riid, void** ppvObj);
typedef ULONG   (WINAPI* D3D929_AddRef)                      (IDirect3DDevice9Ex* pcThis);
typedef ULONG   (WINAPI* D3D929_Release)                     (IDirect3DDevice9Ex* pcThis);

/*** IDirect3DDevice9 method prototypes ***/
typedef HRESULT (WINAPI* D3D929_TestCooperativeLevel)        (IDirect3DDevice9Ex* pcThis);
typedef UINT    (WINAPI* D3D929_GetAvailableTextureMem)      (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_EvictManagedResources)       (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_GetDirect3D)                 (IDirect3DDevice9Ex* pcThis,IDirect3D9** ppD3D9);
typedef HRESULT (WINAPI* D3D929_GetDeviceCaps)               (IDirect3DDevice9Ex* pcThis,D3DCAPS9* pCaps);
typedef HRESULT (WINAPI* D3D929_GetDisplayMode)              (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,D3DDISPLAYMODE* pMode);
typedef HRESULT (WINAPI* D3D929_GetCreationParameters)       (IDirect3DDevice9Ex* pcThis,D3DDEVICE_CREATION_PARAMETERS *pParameters);
typedef HRESULT (WINAPI* D3D929_SetCursorProperties)         (IDirect3DDevice9Ex* pcThis,UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
typedef void    (WINAPI* D3D929_SetCursorPosition)           (IDirect3DDevice9Ex* pcThis,int X,int Y,DWORD Flags);
typedef BOOL    (WINAPI* D3D929_ShowCursor)                  (IDirect3DDevice9Ex* pcThis,BOOL bShow);
typedef HRESULT (WINAPI* D3D929_CreateAdditionalSwapChain)   (IDirect3DDevice9Ex* pcThis,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
typedef HRESULT (WINAPI* D3D929_GetSwapChain)                (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
typedef UINT    (WINAPI* D3D929_GetNumberOfSwapChains)       (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_Reset)                       (IDirect3DDevice9Ex* pcThis,D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT (WINAPI* D3D929_Present)                     (IDirect3DDevice9Ex* pcThis,CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
typedef HRESULT (WINAPI* D3D929_GetBackBuffer)               (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
typedef HRESULT (WINAPI* D3D929_GetRasterStatus)             (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
typedef HRESULT (WINAPI* D3D929_SetDialogBoxMode)            (IDirect3DDevice9Ex* pcThis,BOOL bEnableDialogs);
typedef void    (WINAPI* D3D929_SetGammaRamp)                (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
typedef void    (WINAPI* D3D929_GetGammaRamp)                (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,D3DGAMMARAMP* pRamp);
typedef HRESULT (WINAPI* D3D929_CreateTexture)               (IDirect3DDevice9Ex* pcThis,UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateVolumeTexture)         (IDirect3DDevice9Ex* pcThis,UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateCubeTexture)           (IDirect3DDevice9Ex* pcThis,UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateVertexBuffer)          (IDirect3DDevice9Ex* pcThis,UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateIndexBuffer)           (IDirect3DDevice9Ex* pcThis,UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateRenderTarget)          (IDirect3DDevice9Ex* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_CreateDepthStencilSurface)   (IDirect3DDevice9Ex* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_UpdateSurface)               (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
typedef HRESULT (WINAPI* D3D929_UpdateTexture)               (IDirect3DDevice9Ex* pcThis,IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
typedef HRESULT (WINAPI* D3D929_GetRenderTargetData)         (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
typedef HRESULT (WINAPI* D3D929_GetFrontBufferData)          (IDirect3DDevice9Ex* pcThis,UINT iSwapChain,IDirect3DSurface9* pDestSurface);
typedef HRESULT (WINAPI* D3D929_StretchRect)                 (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
typedef HRESULT (WINAPI* D3D929_ColorFill)                   (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
typedef HRESULT (WINAPI* D3D929_CreateOffscreenPlainSurface) (IDirect3DDevice9Ex* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D929_SetRenderTarget)             (IDirect3DDevice9Ex* pcThis,DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
typedef HRESULT (WINAPI* D3D929_GetRenderTarget)             (IDirect3DDevice9Ex* pcThis,DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
typedef HRESULT (WINAPI* D3D929_SetDepthStencilSurface)      (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9* pNewZStencil);
typedef HRESULT (WINAPI* D3D929_GetDepthStencilSurface)      (IDirect3DDevice9Ex* pcThis,IDirect3DSurface9** ppZStencilSurface);
typedef HRESULT (WINAPI* D3D929_BeginScene)                  (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_EndScene)                    (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_Clear)                       (IDirect3DDevice9Ex* pcThis,DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
typedef HRESULT (WINAPI* D3D929_SetTransform)                (IDirect3DDevice9Ex* pcThis,D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
typedef HRESULT (WINAPI* D3D929_GetTransform)                (IDirect3DDevice9Ex* pcThis,D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
typedef HRESULT (WINAPI* D3D929_MultiplyTransform)           (IDirect3DDevice9Ex* pcThis,D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*);
typedef HRESULT (WINAPI* D3D929_SetViewport)                 (IDirect3DDevice9Ex* pcThis,CONST D3DVIEWPORT9* pViewport);
typedef HRESULT (WINAPI* D3D929_GetViewport)                 (IDirect3DDevice9Ex* pcThis,D3DVIEWPORT9* pViewport);
typedef HRESULT (WINAPI* D3D929_SetMaterial)                 (IDirect3DDevice9Ex* pcThis,CONST D3DMATERIAL9* pMaterial);
typedef HRESULT (WINAPI* D3D929_GetMaterial)                 (IDirect3DDevice9Ex* pcThis,D3DMATERIAL9* pMaterial);
typedef HRESULT (WINAPI* D3D929_SetLight)                    (IDirect3DDevice9Ex* pcThis,DWORD Index,CONST D3DLIGHT9*);
typedef HRESULT (WINAPI* D3D929_GetLight)                    (IDirect3DDevice9Ex* pcThis,DWORD Index,D3DLIGHT9*);
typedef HRESULT (WINAPI* D3D929_LightEnable)                 (IDirect3DDevice9Ex* pcThis,DWORD Index,BOOL Enable);
typedef HRESULT (WINAPI* D3D929_GetLightEnable)              (IDirect3DDevice9Ex* pcThis,DWORD Index,BOOL* pEnable);
typedef HRESULT (WINAPI* D3D929_SetClipPlane)                (IDirect3DDevice9Ex* pcThis,DWORD Index,CONST float* pPlane);
typedef HRESULT (WINAPI* D3D929_GetClipPlane)                (IDirect3DDevice9Ex* pcThis,DWORD Index,float* pPlane);
typedef HRESULT (WINAPI* D3D929_SetRenderState)              (IDirect3DDevice9Ex* pcThis,D3DRENDERSTATETYPE State,DWORD Value);
typedef HRESULT (WINAPI* D3D929_GetRenderState)              (IDirect3DDevice9Ex* pcThis,D3DRENDERSTATETYPE State,DWORD* pValue);
typedef HRESULT (WINAPI* D3D929_CreateStateBlock)            (IDirect3DDevice9Ex* pcThis,D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
typedef HRESULT (WINAPI* D3D929_BeginStateBlock)             (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_EndStateBlock)               (IDirect3DDevice9Ex* pcThis,IDirect3DStateBlock9** ppSB);
typedef HRESULT (WINAPI* D3D929_SetClipStatus)               (IDirect3DDevice9Ex* pcThis,CONST D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT (WINAPI* D3D929_GetClipStatus)               (IDirect3DDevice9Ex* pcThis,D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT (WINAPI* D3D929_GetTexture)                  (IDirect3DDevice9Ex* pcThis,DWORD Stage,IDirect3DBaseTexture9** ppTexture);
typedef HRESULT (WINAPI* D3D929_SetTexture)                  (IDirect3DDevice9Ex* pcThis,DWORD Stage,IDirect3DBaseTexture9* pTexture);
typedef HRESULT (WINAPI* D3D929_GetTextureStageState)        (IDirect3DDevice9Ex* pcThis,DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
typedef HRESULT (WINAPI* D3D929_SetTextureStageState)        (IDirect3DDevice9Ex* pcThis,DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
typedef HRESULT (WINAPI* D3D929_GetSamplerState)             (IDirect3DDevice9Ex* pcThis,DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
typedef HRESULT (WINAPI* D3D929_SetSamplerState)             (IDirect3DDevice9Ex* pcThis,DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
typedef HRESULT (WINAPI* D3D929_ValidateDevice)              (IDirect3DDevice9Ex* pcThis,DWORD* pNumPasses);
typedef HRESULT (WINAPI* D3D929_SetPaletteEntries)           (IDirect3DDevice9Ex* pcThis,UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
typedef HRESULT (WINAPI* D3D929_GetPaletteEntries)           (IDirect3DDevice9Ex* pcThis,UINT PaletteNumber,PALETTEENTRY* pEntries);
typedef HRESULT (WINAPI* D3D929_SetCurrentTexturePalette)    (IDirect3DDevice9Ex* pcThis,UINT PaletteNumber);
typedef HRESULT (WINAPI* D3D929_GetCurrentTexturePalette)    (IDirect3DDevice9Ex* pcThis,UINT *PaletteNumber);
typedef HRESULT (WINAPI* D3D929_SetScissorRect)              (IDirect3DDevice9Ex* pcThis,CONST RECT* pRect);
typedef HRESULT (WINAPI* D3D929_GetScissorRect)              (IDirect3DDevice9Ex* pcThis,RECT* pRect);
typedef HRESULT (WINAPI* D3D929_SetSoftwareVertexProcessing) (IDirect3DDevice9Ex* pcThis,BOOL bSoftware);
typedef BOOL    (WINAPI* D3D929_GetSoftwareVertexProcessing) (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_SetNPatchMode)               (IDirect3DDevice9Ex* pcThis,float nSegments);
typedef float   (WINAPI* D3D929_GetNPatchMode)               (IDirect3DDevice9Ex* pcThis);
typedef HRESULT (WINAPI* D3D929_DrawPrimitive)               (IDirect3DDevice9Ex* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
typedef HRESULT (WINAPI* D3D929_DrawIndexedPrimitive)        (IDirect3DDevice9Ex* pcThis,D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
typedef HRESULT (WINAPI* D3D929_DrawPrimitiveUP)             (IDirect3DDevice9Ex* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
typedef HRESULT (WINAPI* D3D929_DrawIndexedPrimitiveUP)      (IDirect3DDevice9Ex* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
typedef HRESULT (WINAPI* D3D929_ProcessVertices)             (IDirect3DDevice9Ex* pcThis,UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
typedef HRESULT (WINAPI* D3D929_CreateVertexDeclaration)     (IDirect3DDevice9Ex* pcThis,CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT (WINAPI* D3D929_SetVertexDeclaration)        (IDirect3DDevice9Ex* pcThis,IDirect3DVertexDeclaration9* pDecl);
typedef HRESULT (WINAPI* D3D929_GetVertexDeclaration)        (IDirect3DDevice9Ex* pcThis,IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT (WINAPI* D3D929_SetFVF)                      (IDirect3DDevice9Ex* pcThis,DWORD FVF);
typedef HRESULT (WINAPI* D3D929_GetFVF)                      (IDirect3DDevice9Ex* pcThis,DWORD* pFVF);
typedef HRESULT (WINAPI* D3D929_CreateVertexShader)          (IDirect3DDevice9Ex* pcThis,CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
typedef HRESULT (WINAPI* D3D929_SetVertexShader)             (IDirect3DDevice9Ex* pcThis,IDirect3DVertexShader9* pShader);
typedef HRESULT (WINAPI* D3D929_GetVertexShader)             (IDirect3DDevice9Ex* pcThis,IDirect3DVertexShader9** ppShader);
typedef HRESULT (WINAPI* D3D929_SetVertexShaderConstantF)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D929_GetVertexShaderConstantF)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D929_SetVertexShaderConstantI)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D929_GetVertexShaderConstantI)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D929_SetVertexShaderConstantB)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
typedef HRESULT (WINAPI* D3D929_GetVertexShaderConstantB)    (IDirect3DDevice9Ex* pcThis,UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
typedef HRESULT (WINAPI* D3D929_SetStreamSource)             (IDirect3DDevice9Ex* pcThis,UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
typedef HRESULT (WINAPI* D3D929_GetStreamSource)             (IDirect3DDevice9Ex* pcThis,UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
typedef HRESULT (WINAPI* D3D929_SetStreamSourceFreq)         (IDirect3DDevice9Ex* pcThis,UINT StreamNumber,UINT Setting);
typedef HRESULT (WINAPI* D3D929_GetStreamSourceFreq)         (IDirect3DDevice9Ex* pcThis,UINT StreamNumber,UINT* pSetting);
typedef HRESULT (WINAPI* D3D929_SetIndices)                  (IDirect3DDevice9Ex* pcThis,IDirect3DIndexBuffer9* pIndexData);
typedef HRESULT (WINAPI* D3D929_GetIndices)                  (IDirect3DDevice9Ex* pcThis,IDirect3DIndexBuffer9** ppIndexData);
typedef HRESULT (WINAPI* D3D929_CreatePixelShader)           (IDirect3DDevice9Ex* pcThis,CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
typedef HRESULT (WINAPI* D3D929_SetPixelShader)              (IDirect3DDevice9Ex* pcThis,IDirect3DPixelShader9* pShader);
typedef HRESULT (WINAPI* D3D929_GetPixelShader)              (IDirect3DDevice9Ex* pcThis,IDirect3DPixelShader9** ppShader);
typedef HRESULT (WINAPI* D3D929_SetPixelShaderConstantF)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D929_GetPixelShaderConstantF)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D929_SetPixelShaderConstantI)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D929_GetPixelShaderConstantI)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D929_SetPixelShaderConstantB)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
typedef HRESULT (WINAPI* D3D929_GetPixelShaderConstantB)     (IDirect3DDevice9Ex* pcThis,UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
typedef HRESULT (WINAPI* D3D929_DrawRectPatch)               (IDirect3DDevice9Ex* pcThis,UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
typedef HRESULT (WINAPI* D3D929_DrawTriPatch)                (IDirect3DDevice9Ex* pcThis,UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
typedef HRESULT (WINAPI* D3D929_DeletePatch)                 (IDirect3DDevice9Ex* pcThis,UINT Handle);
typedef HRESULT (WINAPI* D3D929_CreateQuery)                 (IDirect3DDevice9Ex* pcThis,D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

/*** IDirect3DDevice9Ex method prototypes ***/
typedef HRESULT (WINAPI* D3D929_SetConvolutionMonoKernel)      (IDirect3DDevice9Ex* pcThis, UINT width,UINT height,float* rows,float* columns);
typedef HRESULT (WINAPI* D3D929_ComposeRects)                  (IDirect3DDevice9Ex* pcThis, IDirect3DSurface9* pSrc,IDirect3DSurface9* pDst,IDirect3DVertexBuffer9* pSrcRectDescs,UINT NumRects,IDirect3DVertexBuffer9* pDstRectDescs,D3DCOMPOSERECTSOP Operation,int Xoffset,int Yoffset);
typedef HRESULT (WINAPI* D3D929_PresentEx)                     (IDirect3DDevice9Ex* pcThis, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags);
typedef HRESULT (WINAPI* D3D929_GetGPUThreadPriority)          (IDirect3DDevice9Ex* pcThis, INT* pPriority);
typedef HRESULT (WINAPI* D3D929_SetGPUThreadPriority)          (IDirect3DDevice9Ex* pcThis, INT Priority);
typedef HRESULT (WINAPI* D3D929_WaitForVBlank)                 (IDirect3DDevice9Ex* pcThis, UINT iSwapChain);
typedef HRESULT (WINAPI* D3D929_CheckResourceResidency)        (IDirect3DDevice9Ex* pcThis, IDirect3DResource9** pResourceArray,UINT32 NumResources);
typedef HRESULT (WINAPI* D3D929_SetMaximumFrameLatency)        (IDirect3DDevice9Ex* pcThis, UINT MaxLatency);
typedef HRESULT (WINAPI* D3D929_GetMaximumFrameLatency)        (IDirect3DDevice9Ex* pcThis, UINT* pMaxLatency);
typedef HRESULT (WINAPI* D3D929_CheckDeviceState)              (IDirect3DDevice9Ex* pcThis, HWND hDestinationWindow);
typedef HRESULT (WINAPI* D3D929_CreateRenderTargetEx)          (IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage);
typedef HRESULT (WINAPI* D3D929_CreateOffscreenPlainSurfaceEx) (IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage);
typedef HRESULT (WINAPI* D3D929_CreateDepthStencilSurfaceEx)   (IDirect3DDevice9Ex* pcThis, UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle,DWORD Usage);
typedef HRESULT (WINAPI* D3D929_ResetEx)                       (IDirect3DDevice9Ex* pcThis, D3DPRESENT_PARAMETERS* pPresentationParameters,D3DDISPLAYMODEEX *pFullscreenDisplayMode);
typedef HRESULT (WINAPI* D3D929_GetDisplayModeEx)              (IDirect3DDevice9Ex* pcThis, UINT iSwapChain,D3DDISPLAYMODEEX* pMode,D3DDISPLAYROTATION* pRotation);

/*** IUnknown super methods ***/
D3D929_QueryInterface               D3D929_IDirect3DDevice9Ex_QueryInterface_Super;
D3D929_AddRef                       D3D929_IDirect3DDevice9Ex_AddRef_Super;
D3D929_Release                      D3D929_IDirect3DDevice9Ex_Release_Super;

/*** IDirect3DDevice9 super methods ***/
D3D929_TestCooperativeLevel         D3D929_IDirect3DDevice9Ex_TestCooperativeLevel_Super;
D3D929_GetAvailableTextureMem       D3D929_IDirect3DDevice9Ex_GetAvailableTextureMem_Super;
D3D929_EvictManagedResources        D3D929_IDirect3DDevice9Ex_EvictManagedResources_Super;
D3D929_GetDirect3D                  D3D929_IDirect3DDevice9Ex_GetDirect3D_Super;
D3D929_GetDeviceCaps                D3D929_IDirect3DDevice9Ex_GetDeviceCaps_Super;
D3D929_GetDisplayMode               D3D929_IDirect3DDevice9Ex_GetDisplayMode_Super;
D3D929_GetCreationParameters        D3D929_IDirect3DDevice9Ex_GetCreationParameters_Super;
D3D929_SetCursorProperties          D3D929_IDirect3DDevice9Ex_SetCursorProperties_Super;
D3D929_SetCursorPosition            D3D929_IDirect3DDevice9Ex_SetCursorPosition_Super;
D3D929_ShowCursor                   D3D929_IDirect3DDevice9Ex_ShowCursor_Super;
D3D929_CreateAdditionalSwapChain    D3D929_IDirect3DDevice9Ex_CreateAdditionalSwapChain_Super;
D3D929_GetSwapChain                 D3D929_IDirect3DDevice9Ex_GetSwapChain_Super;
D3D929_GetNumberOfSwapChains        D3D929_IDirect3DDevice9Ex_GetNumberOfSwapChains_Super;
D3D929_Reset                        D3D929_IDirect3DDevice9Ex_Reset_Super;
D3D929_Present                      D3D929_IDirect3DDevice9Ex_Present_Super;
D3D929_GetBackBuffer                D3D929_IDirect3DDevice9Ex_GetBackBuffer_Super;
D3D929_GetRasterStatus              D3D929_IDirect3DDevice9Ex_GetRasterStatus_Super;
D3D929_SetDialogBoxMode             D3D929_IDirect3DDevice9Ex_SetDialogBoxMode_Super;
D3D929_SetGammaRamp                 D3D929_IDirect3DDevice9Ex_SetGammaRamp_Super;
D3D929_GetGammaRamp                 D3D929_IDirect3DDevice9Ex_GetGammaRamp_Super;
D3D929_CreateTexture                D3D929_IDirect3DDevice9Ex_CreateTexture_Super;
D3D929_CreateVolumeTexture          D3D929_IDirect3DDevice9Ex_CreateVolumeTexture_Super;
D3D929_CreateCubeTexture            D3D929_IDirect3DDevice9Ex_CreateCubeTexture_Super;
D3D929_CreateVertexBuffer           D3D929_IDirect3DDevice9Ex_CreateVertexBuffer_Super;
D3D929_CreateIndexBuffer            D3D929_IDirect3DDevice9Ex_CreateIndexBuffer_Super;
D3D929_CreateRenderTarget           D3D929_IDirect3DDevice9Ex_CreateRenderTarget_Super;
D3D929_CreateDepthStencilSurface    D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurface_Super;
D3D929_UpdateSurface                D3D929_IDirect3DDevice9Ex_UpdateSurface_Super;
D3D929_UpdateTexture                D3D929_IDirect3DDevice9Ex_UpdateTexture_Super;
D3D929_GetRenderTargetData          D3D929_IDirect3DDevice9Ex_GetRenderTargetData_Super;
D3D929_GetFrontBufferData           D3D929_IDirect3DDevice9Ex_GetFrontBufferData_Super;
D3D929_StretchRect                  D3D929_IDirect3DDevice9Ex_StretchRect_Super;
D3D929_ColorFill                    D3D929_IDirect3DDevice9Ex_ColorFill_Super;
D3D929_CreateOffscreenPlainSurface  D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurface_Super;
D3D929_SetRenderTarget              D3D929_IDirect3DDevice9Ex_SetRenderTarget_Super;
D3D929_GetRenderTarget              D3D929_IDirect3DDevice9Ex_GetRenderTarget_Super;
D3D929_SetDepthStencilSurface       D3D929_IDirect3DDevice9Ex_SetDepthStencilSurface_Super;
D3D929_GetDepthStencilSurface       D3D929_IDirect3DDevice9Ex_GetDepthStencilSurface_Super;
D3D929_BeginScene                   D3D929_IDirect3DDevice9Ex_BeginScene_Super;
D3D929_EndScene                     D3D929_IDirect3DDevice9Ex_EndScene_Super;
D3D929_Clear                        D3D929_IDirect3DDevice9Ex_Clear_Super;
D3D929_SetTransform                 D3D929_IDirect3DDevice9Ex_SetTransform_Super;
D3D929_GetTransform                 D3D929_IDirect3DDevice9Ex_GetTransform_Super;
D3D929_MultiplyTransform            D3D929_IDirect3DDevice9Ex_MultiplyTransform_Super;
D3D929_SetViewport                  D3D929_IDirect3DDevice9Ex_SetViewport_Super;
D3D929_GetViewport                  D3D929_IDirect3DDevice9Ex_GetViewport_Super;
D3D929_SetMaterial                  D3D929_IDirect3DDevice9Ex_SetMaterial_Super;
D3D929_GetMaterial                  D3D929_IDirect3DDevice9Ex_GetMaterial_Super;
D3D929_SetLight                     D3D929_IDirect3DDevice9Ex_SetLight_Super;
D3D929_GetLight                     D3D929_IDirect3DDevice9Ex_GetLight_Super;
D3D929_LightEnable                  D3D929_IDirect3DDevice9Ex_LightEnable_Super;
D3D929_GetLightEnable               D3D929_IDirect3DDevice9Ex_GetLightEnable_Super;
D3D929_SetClipPlane                 D3D929_IDirect3DDevice9Ex_SetClipPlane_Super;
D3D929_GetClipPlane                 D3D929_IDirect3DDevice9Ex_GetClipPlane_Super;
D3D929_SetRenderState               D3D929_IDirect3DDevice9Ex_SetRenderState_Super;
D3D929_GetRenderState               D3D929_IDirect3DDevice9Ex_GetRenderState_Super;
D3D929_CreateStateBlock             D3D929_IDirect3DDevice9Ex_CreateStateBlock_Super;
D3D929_BeginStateBlock              D3D929_IDirect3DDevice9Ex_BeginStateBlock_Super;
D3D929_EndStateBlock                D3D929_IDirect3DDevice9Ex_EndStateBlock_Super;
D3D929_SetClipStatus                D3D929_IDirect3DDevice9Ex_SetClipStatus_Super;
D3D929_GetClipStatus                D3D929_IDirect3DDevice9Ex_GetClipStatus_Super;
D3D929_GetTexture                   D3D929_IDirect3DDevice9Ex_GetTexture_Super;
D3D929_SetTexture                   D3D929_IDirect3DDevice9Ex_SetTexture_Super;
D3D929_GetTextureStageState         D3D929_IDirect3DDevice9Ex_GetTextureStageState_Super;
D3D929_SetTextureStageState         D3D929_IDirect3DDevice9Ex_SetTextureStageState_Super;
D3D929_GetSamplerState              D3D929_IDirect3DDevice9Ex_GetSamplerState_Super;
D3D929_SetSamplerState              D3D929_IDirect3DDevice9Ex_SetSamplerState_Super;
D3D929_ValidateDevice               D3D929_IDirect3DDevice9Ex_ValidateDevice_Super;
D3D929_SetPaletteEntries            D3D929_IDirect3DDevice9Ex_SetPaletteEntries_Super;
D3D929_GetPaletteEntries            D3D929_IDirect3DDevice9Ex_GetPaletteEntries_Super;
D3D929_SetCurrentTexturePalette     D3D929_IDirect3DDevice9Ex_SetCurrentTexturePalette_Super;
D3D929_GetCurrentTexturePalette     D3D929_IDirect3DDevice9Ex_GetCurrentTexturePalette_Super;
D3D929_SetScissorRect               D3D929_IDirect3DDevice9Ex_SetScissorRect_Super;
D3D929_GetScissorRect               D3D929_IDirect3DDevice9Ex_GetScissorRect_Super;
D3D929_SetSoftwareVertexProcessing  D3D929_IDirect3DDevice9Ex_SetSoftwareVertexProcessing_Super;
D3D929_GetSoftwareVertexProcessing  D3D929_IDirect3DDevice9Ex_GetSoftwareVertexProcessing_Super;
D3D929_SetNPatchMode                D3D929_IDirect3DDevice9Ex_SetNPatchMode_Super;
D3D929_GetNPatchMode                D3D929_IDirect3DDevice9Ex_GetNPatchMode_Super;
D3D929_DrawPrimitive                D3D929_IDirect3DDevice9Ex_DrawPrimitive_Super;
D3D929_DrawIndexedPrimitive         D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitive_Super;
D3D929_DrawPrimitiveUP              D3D929_IDirect3DDevice9Ex_DrawPrimitiveUP_Super;
D3D929_DrawIndexedPrimitiveUP       D3D929_IDirect3DDevice9Ex_DrawIndexedPrimitiveUP_Super;
D3D929_ProcessVertices              D3D929_IDirect3DDevice9Ex_ProcessVertices_Super;
D3D929_CreateVertexDeclaration      D3D929_IDirect3DDevice9Ex_CreateVertexDeclaration_Super;
D3D929_SetVertexDeclaration         D3D929_IDirect3DDevice9Ex_SetVertexDeclaration_Super;
D3D929_GetVertexDeclaration         D3D929_IDirect3DDevice9Ex_GetVertexDeclaration_Super;
D3D929_SetFVF                       D3D929_IDirect3DDevice9Ex_SetFVF_Super;
D3D929_GetFVF                       D3D929_IDirect3DDevice9Ex_GetFVF_Super;
D3D929_CreateVertexShader           D3D929_IDirect3DDevice9Ex_CreateVertexShader_Super;
D3D929_SetVertexShader              D3D929_IDirect3DDevice9Ex_SetVertexShader_Super;
D3D929_GetVertexShader              D3D929_IDirect3DDevice9Ex_GetVertexShader_Super;
D3D929_SetVertexShaderConstantF     D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantF_Super;
D3D929_GetVertexShaderConstantF     D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantF_Super;
D3D929_SetVertexShaderConstantI     D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantI_Super;
D3D929_GetVertexShaderConstantI     D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantI_Super;
D3D929_SetVertexShaderConstantB     D3D929_IDirect3DDevice9Ex_SetVertexShaderConstantB_Super;
D3D929_GetVertexShaderConstantB     D3D929_IDirect3DDevice9Ex_GetVertexShaderConstantB_Super;
D3D929_SetStreamSource              D3D929_IDirect3DDevice9Ex_SetStreamSource_Super;
D3D929_GetStreamSource              D3D929_IDirect3DDevice9Ex_GetStreamSource_Super;
D3D929_SetStreamSourceFreq          D3D929_IDirect3DDevice9Ex_SetStreamSourceFreq_Super;
D3D929_GetStreamSourceFreq          D3D929_IDirect3DDevice9Ex_GetStreamSourceFreq_Super;
D3D929_SetIndices                   D3D929_IDirect3DDevice9Ex_SetIndices_Super;
D3D929_GetIndices                   D3D929_IDirect3DDevice9Ex_GetIndices_Super;
D3D929_CreatePixelShader            D3D929_IDirect3DDevice9Ex_CreatePixelShader_Super;
D3D929_SetPixelShader               D3D929_IDirect3DDevice9Ex_SetPixelShader_Super;
D3D929_GetPixelShader               D3D929_IDirect3DDevice9Ex_GetPixelShader_Super;
D3D929_SetPixelShaderConstantF      D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantF_Super;
D3D929_GetPixelShaderConstantF      D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantF_Super;
D3D929_SetPixelShaderConstantI      D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantI_Super;
D3D929_GetPixelShaderConstantI      D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantI_Super;
D3D929_SetPixelShaderConstantB      D3D929_IDirect3DDevice9Ex_SetPixelShaderConstantB_Super;
D3D929_GetPixelShaderConstantB      D3D929_IDirect3DDevice9Ex_GetPixelShaderConstantB_Super;
D3D929_DrawRectPatch                D3D929_IDirect3DDevice9Ex_DrawRectPatch_Super;
D3D929_DrawTriPatch                 D3D929_IDirect3DDevice9Ex_DrawTriPatch_Super;
D3D929_DeletePatch                  D3D929_IDirect3DDevice9Ex_DeletePatch_Super;
D3D929_CreateQuery                  D3D929_IDirect3DDevice9Ex_CreateQuery_Super;

/*** IDirect3DDevice9Ex super methods ***/
D3D929_SetConvolutionMonoKernel      D3D929_IDirect3DDevice9Ex_SetConvolutionMonoKernel_Super;
D3D929_ComposeRects                  D3D929_IDirect3DDevice9Ex_ComposeRects_Super;                  
D3D929_PresentEx                     D3D929_IDirect3DDevice9Ex_PresentEx_Super;                     
D3D929_GetGPUThreadPriority          D3D929_IDirect3DDevice9Ex_GetGPUThreadPriority_Super;          
D3D929_SetGPUThreadPriority          D3D929_IDirect3DDevice9Ex_SetGPUThreadPriority_Super;          
D3D929_WaitForVBlank                 D3D929_IDirect3DDevice9Ex_WaitForVBlank_Super;                 
D3D929_CheckResourceResidency        D3D929_IDirect3DDevice9Ex_CheckResourceResidency_Super;        
D3D929_SetMaximumFrameLatency        D3D929_IDirect3DDevice9Ex_SetMaximumFrameLatency_Super;        
D3D929_GetMaximumFrameLatency        D3D929_IDirect3DDevice9Ex_GetMaximumFrameLatency_Super;        
D3D929_CheckDeviceState              D3D929_IDirect3DDevice9Ex_CheckDeviceState_Super;              
D3D929_CreateRenderTargetEx          D3D929_IDirect3DDevice9Ex_CreateRenderTargetEx_Super;          
D3D929_CreateOffscreenPlainSurfaceEx D3D929_IDirect3DDevice9Ex_CreateOffscreenPlainSurfaceEx_Super; 
D3D929_CreateDepthStencilSurfaceEx   D3D929_IDirect3DDevice9Ex_CreateDepthStencilSurfaceEx_Super;   
D3D929_ResetEx                       D3D929_IDirect3DDevice9Ex_ResetEx_Super;                      
D3D929_GetDisplayModeEx              D3D929_IDirect3DDevice9Ex_GetDisplayModeEx_Super;             

#endif