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
#ifndef DCL_IDIRECT3DDEVICE9_SUPER
#define DCL_IDIRECT3DDEVICE9_SUPER

/*** IUnknown method prototypes ***/
typedef HRESULT (WINAPI* D3D9_QueryInterface)              (IDirect3DDevice9* pcThis,REFIID riid, void** ppvObj);
typedef ULONG   (WINAPI* D3D9_AddRef)                      (IDirect3DDevice9* pcThis);
typedef ULONG   (WINAPI* D3D9_Release)                     (IDirect3DDevice9* pcThis);

/*** IDirect3DDevice9 method prototypes ***/
typedef HRESULT (WINAPI* D3D9_TestCooperativeLevel)        (IDirect3DDevice9* pcThis);
typedef UINT    (WINAPI* D3D9_GetAvailableTextureMem)      (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_EvictManagedResources)       (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_GetDirect3D)                 (IDirect3DDevice9* pcThis,IDirect3D9** ppD3D9);
typedef HRESULT (WINAPI* D3D9_GetDeviceCaps)               (IDirect3DDevice9* pcThis,D3DCAPS9* pCaps);
typedef HRESULT (WINAPI* D3D9_GetDisplayMode)              (IDirect3DDevice9* pcThis,UINT iSwapChain,D3DDISPLAYMODE* pMode);
typedef HRESULT (WINAPI* D3D9_GetCreationParameters)       (IDirect3DDevice9* pcThis,D3DDEVICE_CREATION_PARAMETERS *pParameters);
typedef HRESULT (WINAPI* D3D9_SetCursorProperties)         (IDirect3DDevice9* pcThis,UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
typedef void    (WINAPI* D3D9_SetCursorPosition)           (IDirect3DDevice9* pcThis,int X,int Y,DWORD Flags);
typedef BOOL    (WINAPI* D3D9_ShowCursor)                  (IDirect3DDevice9* pcThis,BOOL bShow);
typedef HRESULT (WINAPI* D3D9_CreateAdditionalSwapChain)   (IDirect3DDevice9* pcThis,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
typedef HRESULT (WINAPI* D3D9_GetSwapChain)                (IDirect3DDevice9* pcThis,UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
typedef UINT    (WINAPI* D3D9_GetNumberOfSwapChains)       (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_Reset)                       (IDirect3DDevice9* pcThis,D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT (WINAPI* D3D9_Present)                     (IDirect3DDevice9* pcThis,CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
typedef HRESULT (WINAPI* D3D9_GetBackBuffer)               (IDirect3DDevice9* pcThis,UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
typedef HRESULT (WINAPI* D3D9_GetRasterStatus)             (IDirect3DDevice9* pcThis,UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
typedef HRESULT (WINAPI* D3D9_SetDialogBoxMode)            (IDirect3DDevice9* pcThis,BOOL bEnableDialogs);
typedef void    (WINAPI* D3D9_SetGammaRamp)                (IDirect3DDevice9* pcThis,UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
typedef void    (WINAPI* D3D9_GetGammaRamp)                (IDirect3DDevice9* pcThis,UINT iSwapChain,D3DGAMMARAMP* pRamp);
typedef HRESULT (WINAPI* D3D9_CreateTexture)               (IDirect3DDevice9* pcThis,UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateVolumeTexture)         (IDirect3DDevice9* pcThis,UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateCubeTexture)           (IDirect3DDevice9* pcThis,UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateVertexBuffer)          (IDirect3DDevice9* pcThis,UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateIndexBuffer)           (IDirect3DDevice9* pcThis,UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateRenderTarget)          (IDirect3DDevice9* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_CreateDepthStencilSurface)   (IDirect3DDevice9* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_UpdateSurface)               (IDirect3DDevice9* pcThis,IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
typedef HRESULT (WINAPI* D3D9_UpdateTexture)               (IDirect3DDevice9* pcThis,IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
typedef HRESULT (WINAPI* D3D9_GetRenderTargetData)         (IDirect3DDevice9* pcThis,IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
typedef HRESULT (WINAPI* D3D9_GetFrontBufferData)          (IDirect3DDevice9* pcThis,UINT iSwapChain,IDirect3DSurface9* pDestSurface);
typedef HRESULT (WINAPI* D3D9_StretchRect)                 (IDirect3DDevice9* pcThis,IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
typedef HRESULT (WINAPI* D3D9_ColorFill)                   (IDirect3DDevice9* pcThis,IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
typedef HRESULT (WINAPI* D3D9_CreateOffscreenPlainSurface) (IDirect3DDevice9* pcThis,UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
typedef HRESULT (WINAPI* D3D9_SetRenderTarget)             (IDirect3DDevice9* pcThis,DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
typedef HRESULT (WINAPI* D3D9_GetRenderTarget)             (IDirect3DDevice9* pcThis,DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
typedef HRESULT (WINAPI* D3D9_SetDepthStencilSurface)      (IDirect3DDevice9* pcThis,IDirect3DSurface9* pNewZStencil);
typedef HRESULT (WINAPI* D3D9_GetDepthStencilSurface)      (IDirect3DDevice9* pcThis,IDirect3DSurface9** ppZStencilSurface);
typedef HRESULT (WINAPI* D3D9_BeginScene)                  (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_EndScene)                    (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_Clear)                       (IDirect3DDevice9* pcThis,DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
typedef HRESULT (WINAPI* D3D9_SetTransform)                (IDirect3DDevice9* pcThis,D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
typedef HRESULT (WINAPI* D3D9_GetTransform)                (IDirect3DDevice9* pcThis,D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
typedef HRESULT (WINAPI* D3D9_MultiplyTransform)           (IDirect3DDevice9* pcThis,D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*);
typedef HRESULT (WINAPI* D3D9_SetViewport)                 (IDirect3DDevice9* pcThis,CONST D3DVIEWPORT9* pViewport);
typedef HRESULT (WINAPI* D3D9_GetViewport)                 (IDirect3DDevice9* pcThis,D3DVIEWPORT9* pViewport);
typedef HRESULT (WINAPI* D3D9_SetMaterial)                 (IDirect3DDevice9* pcThis,CONST D3DMATERIAL9* pMaterial);
typedef HRESULT (WINAPI* D3D9_GetMaterial)                 (IDirect3DDevice9* pcThis,D3DMATERIAL9* pMaterial);
typedef HRESULT (WINAPI* D3D9_SetLight)                    (IDirect3DDevice9* pcThis,DWORD Index,CONST D3DLIGHT9*);
typedef HRESULT (WINAPI* D3D9_GetLight)                    (IDirect3DDevice9* pcThis,DWORD Index,D3DLIGHT9*);
typedef HRESULT (WINAPI* D3D9_LightEnable)                 (IDirect3DDevice9* pcThis,DWORD Index,BOOL Enable);
typedef HRESULT (WINAPI* D3D9_GetLightEnable)              (IDirect3DDevice9* pcThis,DWORD Index,BOOL* pEnable);
typedef HRESULT (WINAPI* D3D9_SetClipPlane)                (IDirect3DDevice9* pcThis,DWORD Index,CONST float* pPlane);
typedef HRESULT (WINAPI* D3D9_GetClipPlane)                (IDirect3DDevice9* pcThis,DWORD Index,float* pPlane);
typedef HRESULT (WINAPI* D3D9_SetRenderState)              (IDirect3DDevice9* pcThis,D3DRENDERSTATETYPE State,DWORD Value);
typedef HRESULT (WINAPI* D3D9_GetRenderState)              (IDirect3DDevice9* pcThis,D3DRENDERSTATETYPE State,DWORD* pValue);
typedef HRESULT (WINAPI* D3D9_CreateStateBlock)            (IDirect3DDevice9* pcThis,D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
typedef HRESULT (WINAPI* D3D9_BeginStateBlock)             (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_EndStateBlock)               (IDirect3DDevice9* pcThis,IDirect3DStateBlock9** ppSB);
typedef HRESULT (WINAPI* D3D9_SetClipStatus)               (IDirect3DDevice9* pcThis,CONST D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT (WINAPI* D3D9_GetClipStatus)               (IDirect3DDevice9* pcThis,D3DCLIPSTATUS9* pClipStatus);
typedef HRESULT (WINAPI* D3D9_GetTexture)                  (IDirect3DDevice9* pcThis,DWORD Stage,IDirect3DBaseTexture9** ppTexture);
typedef HRESULT (WINAPI* D3D9_SetTexture)                  (IDirect3DDevice9* pcThis,DWORD Stage,IDirect3DBaseTexture9* pTexture);
typedef HRESULT (WINAPI* D3D9_GetTextureStageState)        (IDirect3DDevice9* pcThis,DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
typedef HRESULT (WINAPI* D3D9_SetTextureStageState)        (IDirect3DDevice9* pcThis,DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
typedef HRESULT (WINAPI* D3D9_GetSamplerState)             (IDirect3DDevice9* pcThis,DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
typedef HRESULT (WINAPI* D3D9_SetSamplerState)             (IDirect3DDevice9* pcThis,DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
typedef HRESULT (WINAPI* D3D9_ValidateDevice)              (IDirect3DDevice9* pcThis,DWORD* pNumPasses);
typedef HRESULT (WINAPI* D3D9_SetPaletteEntries)           (IDirect3DDevice9* pcThis,UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
typedef HRESULT (WINAPI* D3D9_GetPaletteEntries)           (IDirect3DDevice9* pcThis,UINT PaletteNumber,PALETTEENTRY* pEntries);
typedef HRESULT (WINAPI* D3D9_SetCurrentTexturePalette)    (IDirect3DDevice9* pcThis,UINT PaletteNumber);
typedef HRESULT (WINAPI* D3D9_GetCurrentTexturePalette)    (IDirect3DDevice9* pcThis,UINT *PaletteNumber);
typedef HRESULT (WINAPI* D3D9_SetScissorRect)              (IDirect3DDevice9* pcThis,CONST RECT* pRect);
typedef HRESULT (WINAPI* D3D9_GetScissorRect)              (IDirect3DDevice9* pcThis,RECT* pRect);
typedef HRESULT (WINAPI* D3D9_SetSoftwareVertexProcessing) (IDirect3DDevice9* pcThis,BOOL bSoftware);
typedef BOOL    (WINAPI* D3D9_GetSoftwareVertexProcessing) (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_SetNPatchMode)               (IDirect3DDevice9* pcThis,float nSegments);
typedef float   (WINAPI* D3D9_GetNPatchMode)               (IDirect3DDevice9* pcThis);
typedef HRESULT (WINAPI* D3D9_DrawPrimitive)               (IDirect3DDevice9* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
typedef HRESULT (WINAPI* D3D9_DrawIndexedPrimitive)        (IDirect3DDevice9* pcThis,D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
typedef HRESULT (WINAPI* D3D9_DrawPrimitiveUP)             (IDirect3DDevice9* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
typedef HRESULT (WINAPI* D3D9_DrawIndexedPrimitiveUP)      (IDirect3DDevice9* pcThis,D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
typedef HRESULT (WINAPI* D3D9_ProcessVertices)             (IDirect3DDevice9* pcThis,UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
typedef HRESULT (WINAPI* D3D9_CreateVertexDeclaration)     (IDirect3DDevice9* pcThis,CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT (WINAPI* D3D9_SetVertexDeclaration)        (IDirect3DDevice9* pcThis,IDirect3DVertexDeclaration9* pDecl);
typedef HRESULT (WINAPI* D3D9_GetVertexDeclaration)        (IDirect3DDevice9* pcThis,IDirect3DVertexDeclaration9** ppDecl);
typedef HRESULT (WINAPI* D3D9_SetFVF)                      (IDirect3DDevice9* pcThis,DWORD FVF);
typedef HRESULT (WINAPI* D3D9_GetFVF)                      (IDirect3DDevice9* pcThis,DWORD* pFVF);
typedef HRESULT (WINAPI* D3D9_CreateVertexShader)          (IDirect3DDevice9* pcThis,CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
typedef HRESULT (WINAPI* D3D9_SetVertexShader)             (IDirect3DDevice9* pcThis,IDirect3DVertexShader9* pShader);
typedef HRESULT (WINAPI* D3D9_GetVertexShader)             (IDirect3DDevice9* pcThis,IDirect3DVertexShader9** ppShader);
typedef HRESULT (WINAPI* D3D9_SetVertexShaderConstantF)    (IDirect3DDevice9* pcThis,UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D9_GetVertexShaderConstantF)    (IDirect3DDevice9* pcThis,UINT StartRegister,float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D9_SetVertexShaderConstantI)    (IDirect3DDevice9* pcThis,UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D9_GetVertexShaderConstantI)    (IDirect3DDevice9* pcThis,UINT StartRegister,int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D9_SetVertexShaderConstantB)    (IDirect3DDevice9* pcThis,UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
typedef HRESULT (WINAPI* D3D9_GetVertexShaderConstantB)    (IDirect3DDevice9* pcThis,UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
typedef HRESULT (WINAPI* D3D9_SetStreamSource)             (IDirect3DDevice9* pcThis,UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
typedef HRESULT (WINAPI* D3D9_GetStreamSource)             (IDirect3DDevice9* pcThis,UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
typedef HRESULT (WINAPI* D3D9_SetStreamSourceFreq)         (IDirect3DDevice9* pcThis,UINT StreamNumber,UINT Setting);
typedef HRESULT (WINAPI* D3D9_GetStreamSourceFreq)         (IDirect3DDevice9* pcThis,UINT StreamNumber,UINT* pSetting);
typedef HRESULT (WINAPI* D3D9_SetIndices)                  (IDirect3DDevice9* pcThis,IDirect3DIndexBuffer9* pIndexData);
typedef HRESULT (WINAPI* D3D9_GetIndices)                  (IDirect3DDevice9* pcThis,IDirect3DIndexBuffer9** ppIndexData);
typedef HRESULT (WINAPI* D3D9_CreatePixelShader)           (IDirect3DDevice9* pcThis,CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
typedef HRESULT (WINAPI* D3D9_SetPixelShader)              (IDirect3DDevice9* pcThis,IDirect3DPixelShader9* pShader);
typedef HRESULT (WINAPI* D3D9_GetPixelShader)              (IDirect3DDevice9* pcThis,IDirect3DPixelShader9** ppShader);
typedef HRESULT (WINAPI* D3D9_SetPixelShaderConstantF)     (IDirect3DDevice9* pcThis,UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D9_GetPixelShaderConstantF)     (IDirect3DDevice9* pcThis,UINT StartRegister,float* pConstantData,UINT Vector4fCount);
typedef HRESULT (WINAPI* D3D9_SetPixelShaderConstantI)     (IDirect3DDevice9* pcThis,UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D9_GetPixelShaderConstantI)     (IDirect3DDevice9* pcThis,UINT StartRegister,int* pConstantData,UINT Vector4iCount);
typedef HRESULT (WINAPI* D3D9_SetPixelShaderConstantB)     (IDirect3DDevice9* pcThis,UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
typedef HRESULT (WINAPI* D3D9_GetPixelShaderConstantB)     (IDirect3DDevice9* pcThis,UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
typedef HRESULT (WINAPI* D3D9_DrawRectPatch)               (IDirect3DDevice9* pcThis,UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
typedef HRESULT (WINAPI* D3D9_DrawTriPatch)                (IDirect3DDevice9* pcThis,UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
typedef HRESULT (WINAPI* D3D9_DeletePatch)                 (IDirect3DDevice9* pcThis,UINT Handle);
typedef HRESULT (WINAPI* D3D9_CreateQuery)                 (IDirect3DDevice9* pcThis,D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

/*** IUnknown super methods ***/
D3D9_QueryInterface               D3D9_IDirect3DDevice9_QueryInterface_Super;
D3D9_AddRef                       D3D9_IDirect3DDevice9_AddRef_Super;
D3D9_Release                      D3D9_IDirect3DDevice9_Release_Super;

/*** IDirect3DDevice9 super methods ***/
D3D9_TestCooperativeLevel         D3D9_IDirect3DDevice9_TestCooperativeLevel_Super;
D3D9_GetAvailableTextureMem       D3D9_IDirect3DDevice9_GetAvailableTextureMem_Super;
D3D9_EvictManagedResources        D3D9_IDirect3DDevice9_EvictManagedResources_Super;
D3D9_GetDirect3D                  D3D9_IDirect3DDevice9_GetDirect3D_Super;
D3D9_GetDeviceCaps                D3D9_IDirect3DDevice9_GetDeviceCaps_Super;
D3D9_GetDisplayMode               D3D9_IDirect3DDevice9_GetDisplayMode_Super;
D3D9_GetCreationParameters        D3D9_IDirect3DDevice9_GetCreationParameters_Super;
D3D9_SetCursorProperties          D3D9_IDirect3DDevice9_SetCursorProperties_Super;
D3D9_SetCursorPosition            D3D9_IDirect3DDevice9_SetCursorPosition_Super;
D3D9_ShowCursor                   D3D9_IDirect3DDevice9_ShowCursor_Super;
D3D9_CreateAdditionalSwapChain    D3D9_IDirect3DDevice9_CreateAdditionalSwapChain_Super;
D3D9_GetSwapChain                 D3D9_IDirect3DDevice9_GetSwapChain_Super;
D3D9_GetNumberOfSwapChains        D3D9_IDirect3DDevice9_GetNumberOfSwapChains_Super;
D3D9_Reset                        D3D9_IDirect3DDevice9_Reset_Super;
D3D9_Present                      D3D9_IDirect3DDevice9_Present_Super;
D3D9_GetBackBuffer                D3D9_IDirect3DDevice9_GetBackBuffer_Super;
D3D9_GetRasterStatus              D3D9_IDirect3DDevice9_GetRasterStatus_Super;
D3D9_SetDialogBoxMode             D3D9_IDirect3DDevice9_SetDialogBoxMode_Super;
D3D9_SetGammaRamp                 D3D9_IDirect3DDevice9_SetGammaRamp_Super;
D3D9_GetGammaRamp                 D3D9_IDirect3DDevice9_GetGammaRamp_Super;
D3D9_CreateTexture                D3D9_IDirect3DDevice9_CreateTexture_Super;
D3D9_CreateVolumeTexture          D3D9_IDirect3DDevice9_CreateVolumeTexture_Super;
D3D9_CreateCubeTexture            D3D9_IDirect3DDevice9_CreateCubeTexture_Super;
D3D9_CreateVertexBuffer           D3D9_IDirect3DDevice9_CreateVertexBuffer_Super;
D3D9_CreateIndexBuffer            D3D9_IDirect3DDevice9_CreateIndexBuffer_Super;
D3D9_CreateRenderTarget           D3D9_IDirect3DDevice9_CreateRenderTarget_Super;
D3D9_CreateDepthStencilSurface    D3D9_IDirect3DDevice9_CreateDepthStencilSurface_Super;
D3D9_UpdateSurface                D3D9_IDirect3DDevice9_UpdateSurface_Super;
D3D9_UpdateTexture                D3D9_IDirect3DDevice9_UpdateTexture_Super;
D3D9_GetRenderTargetData          D3D9_IDirect3DDevice9_GetRenderTargetData_Super;
D3D9_GetFrontBufferData           D3D9_IDirect3DDevice9_GetFrontBufferData_Super;
D3D9_StretchRect                  D3D9_IDirect3DDevice9_StretchRect_Super;
D3D9_ColorFill                    D3D9_IDirect3DDevice9_ColorFill_Super;
D3D9_CreateOffscreenPlainSurface  D3D9_IDirect3DDevice9_CreateOffscreenPlainSurface_Super;
D3D9_SetRenderTarget              D3D9_IDirect3DDevice9_SetRenderTarget_Super;
D3D9_GetRenderTarget              D3D9_IDirect3DDevice9_GetRenderTarget_Super;
D3D9_SetDepthStencilSurface       D3D9_IDirect3DDevice9_SetDepthStencilSurface_Super;
D3D9_GetDepthStencilSurface       D3D9_IDirect3DDevice9_GetDepthStencilSurface_Super;
D3D9_BeginScene                   D3D9_IDirect3DDevice9_BeginScene_Super;
D3D9_EndScene                     D3D9_IDirect3DDevice9_EndScene_Super;
D3D9_Clear                        D3D9_IDirect3DDevice9_Clear_Super;
D3D9_SetTransform                 D3D9_IDirect3DDevice9_SetTransform_Super;
D3D9_GetTransform                 D3D9_IDirect3DDevice9_GetTransform_Super;
D3D9_MultiplyTransform            D3D9_IDirect3DDevice9_MultiplyTransform_Super;
D3D9_SetViewport                  D3D9_IDirect3DDevice9_SetViewport_Super;
D3D9_GetViewport                  D3D9_IDirect3DDevice9_GetViewport_Super;
D3D9_SetMaterial                  D3D9_IDirect3DDevice9_SetMaterial_Super;
D3D9_GetMaterial                  D3D9_IDirect3DDevice9_GetMaterial_Super;
D3D9_SetLight                     D3D9_IDirect3DDevice9_SetLight_Super;
D3D9_GetLight                     D3D9_IDirect3DDevice9_GetLight_Super;
D3D9_LightEnable                  D3D9_IDirect3DDevice9_LightEnable_Super;
D3D9_GetLightEnable               D3D9_IDirect3DDevice9_GetLightEnable_Super;
D3D9_SetClipPlane                 D3D9_IDirect3DDevice9_SetClipPlane_Super;
D3D9_GetClipPlane                 D3D9_IDirect3DDevice9_GetClipPlane_Super;
D3D9_SetRenderState               D3D9_IDirect3DDevice9_SetRenderState_Super;
D3D9_GetRenderState               D3D9_IDirect3DDevice9_GetRenderState_Super;
D3D9_CreateStateBlock             D3D9_IDirect3DDevice9_CreateStateBlock_Super;
D3D9_BeginStateBlock              D3D9_IDirect3DDevice9_BeginStateBlock_Super;
D3D9_EndStateBlock                D3D9_IDirect3DDevice9_EndStateBlock_Super;
D3D9_SetClipStatus                D3D9_IDirect3DDevice9_SetClipStatus_Super;
D3D9_GetClipStatus                D3D9_IDirect3DDevice9_GetClipStatus_Super;
D3D9_GetTexture                   D3D9_IDirect3DDevice9_GetTexture_Super;
D3D9_SetTexture                   D3D9_IDirect3DDevice9_SetTexture_Super;
D3D9_GetTextureStageState         D3D9_IDirect3DDevice9_GetTextureStageState_Super;
D3D9_SetTextureStageState         D3D9_IDirect3DDevice9_SetTextureStageState_Super;
D3D9_GetSamplerState              D3D9_IDirect3DDevice9_GetSamplerState_Super;
D3D9_SetSamplerState              D3D9_IDirect3DDevice9_SetSamplerState_Super;
D3D9_ValidateDevice               D3D9_IDirect3DDevice9_ValidateDevice_Super;
D3D9_SetPaletteEntries            D3D9_IDirect3DDevice9_SetPaletteEntries_Super;
D3D9_GetPaletteEntries            D3D9_IDirect3DDevice9_GetPaletteEntries_Super;
D3D9_SetCurrentTexturePalette     D3D9_IDirect3DDevice9_SetCurrentTexturePalette_Super;
D3D9_GetCurrentTexturePalette     D3D9_IDirect3DDevice9_GetCurrentTexturePalette_Super;
D3D9_SetScissorRect               D3D9_IDirect3DDevice9_SetScissorRect_Super;
D3D9_GetScissorRect               D3D9_IDirect3DDevice9_GetScissorRect_Super;
D3D9_SetSoftwareVertexProcessing  D3D9_IDirect3DDevice9_SetSoftwareVertexProcessing_Super;
D3D9_GetSoftwareVertexProcessing  D3D9_IDirect3DDevice9_GetSoftwareVertexProcessing_Super;
D3D9_SetNPatchMode                D3D9_IDirect3DDevice9_SetNPatchMode_Super;
D3D9_GetNPatchMode                D3D9_IDirect3DDevice9_GetNPatchMode_Super;
D3D9_DrawPrimitive                D3D9_IDirect3DDevice9_DrawPrimitive_Super;
D3D9_DrawIndexedPrimitive         D3D9_IDirect3DDevice9_DrawIndexedPrimitive_Super;
D3D9_DrawPrimitiveUP              D3D9_IDirect3DDevice9_DrawPrimitiveUP_Super;
D3D9_DrawIndexedPrimitiveUP       D3D9_IDirect3DDevice9_DrawIndexedPrimitiveUP_Super;
D3D9_ProcessVertices              D3D9_IDirect3DDevice9_ProcessVertices_Super;
D3D9_CreateVertexDeclaration      D3D9_IDirect3DDevice9_CreateVertexDeclaration_Super;
D3D9_SetVertexDeclaration         D3D9_IDirect3DDevice9_SetVertexDeclaration_Super;
D3D9_GetVertexDeclaration         D3D9_IDirect3DDevice9_GetVertexDeclaration_Super;
D3D9_SetFVF                       D3D9_IDirect3DDevice9_SetFVF_Super;
D3D9_GetFVF                       D3D9_IDirect3DDevice9_GetFVF_Super;
D3D9_CreateVertexShader           D3D9_IDirect3DDevice9_CreateVertexShader_Super;
D3D9_SetVertexShader              D3D9_IDirect3DDevice9_SetVertexShader_Super;
D3D9_GetVertexShader              D3D9_IDirect3DDevice9_GetVertexShader_Super;
D3D9_SetVertexShaderConstantF     D3D9_IDirect3DDevice9_SetVertexShaderConstantF_Super;
D3D9_GetVertexShaderConstantF     D3D9_IDirect3DDevice9_GetVertexShaderConstantF_Super;
D3D9_SetVertexShaderConstantI     D3D9_IDirect3DDevice9_SetVertexShaderConstantI_Super;
D3D9_GetVertexShaderConstantI     D3D9_IDirect3DDevice9_GetVertexShaderConstantI_Super;
D3D9_SetVertexShaderConstantB     D3D9_IDirect3DDevice9_SetVertexShaderConstantB_Super;
D3D9_GetVertexShaderConstantB     D3D9_IDirect3DDevice9_GetVertexShaderConstantB_Super;
D3D9_SetStreamSource              D3D9_IDirect3DDevice9_SetStreamSource_Super;
D3D9_GetStreamSource              D3D9_IDirect3DDevice9_GetStreamSource_Super;
D3D9_SetStreamSourceFreq          D3D9_IDirect3DDevice9_SetStreamSourceFreq_Super;
D3D9_GetStreamSourceFreq          D3D9_IDirect3DDevice9_GetStreamSourceFreq_Super;
D3D9_SetIndices                   D3D9_IDirect3DDevice9_SetIndices_Super;
D3D9_GetIndices                   D3D9_IDirect3DDevice9_GetIndices_Super;
D3D9_CreatePixelShader            D3D9_IDirect3DDevice9_CreatePixelShader_Super;
D3D9_SetPixelShader               D3D9_IDirect3DDevice9_SetPixelShader_Super;
D3D9_GetPixelShader               D3D9_IDirect3DDevice9_GetPixelShader_Super;
D3D9_SetPixelShaderConstantF      D3D9_IDirect3DDevice9_SetPixelShaderConstantF_Super;
D3D9_GetPixelShaderConstantF      D3D9_IDirect3DDevice9_GetPixelShaderConstantF_Super;
D3D9_SetPixelShaderConstantI      D3D9_IDirect3DDevice9_SetPixelShaderConstantI_Super;
D3D9_GetPixelShaderConstantI      D3D9_IDirect3DDevice9_GetPixelShaderConstantI_Super;
D3D9_SetPixelShaderConstantB      D3D9_IDirect3DDevice9_SetPixelShaderConstantB_Super;
D3D9_GetPixelShaderConstantB      D3D9_IDirect3DDevice9_GetPixelShaderConstantB_Super;
D3D9_DrawRectPatch                D3D9_IDirect3DDevice9_DrawRectPatch_Super;
D3D9_DrawTriPatch                 D3D9_IDirect3DDevice9_DrawTriPatch_Super;
D3D9_DeletePatch                  D3D9_IDirect3DDevice9_DeletePatch_Super;
D3D9_CreateQuery                  D3D9_IDirect3DDevice9_CreateQuery_Super;

#endif