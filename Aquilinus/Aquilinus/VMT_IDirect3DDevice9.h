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
#ifndef VMT_IDIRECT3DDEVICE9_ENUM
#define VMT_IDIRECT3DDEVICE9_ENUM

#include "VMT_IUnknown.h"

namespace VMT_IDIRECT3DDEVICE9
{

	/**
	* Virtual Methods Table for IDirect3DDevice9.
	***/
	enum VMT_IDirect3DDevice9
	{
		/*** IDirect3DDevice9 methods ***/
		TestCooperativeLevel        = 3, 
		GetAvailableTextureMem      = 4, 
		EvictManagedResources       = 5, 
		GetDirect3D                 = 6, 
		GetDeviceCaps               = 7, 
		GetDisplayMode              = 8, 
		GetCreationParameters       = 9, 
		SetCursorProperties         = 10, 
		SetCursorPosition           = 11, 
		ShowCursor                  = 12, 
		CreateAdditionalSwapChain   = 13, 
		GetSwapChain                = 14, 
		GetNumberOfSwapChains       = 15, 
		Reset                       = 16, 
		Present                     = 17, 
		GetBackBuffer               = 18, 
		GetRasterStatus             = 19, 
		SetDialogBoxMode            = 20, 
		SetGammaRamp                = 21, 
		GetGammaRamp                = 22, 
		CreateTexture               = 23, 
		CreateVolumeTexture         = 24, 
		CreateCubeTexture           = 25, 
		CreateVertexBuffer          = 26, 
		CreateIndexBuffer           = 27, 
		CreateRenderTarget          = 28, 
		CreateDepthStencilSurface   = 29, 
		UpdateSurface               = 30, 
		UpdateTexture               = 31, 
		GetRenderTargetData         = 32, 
		GetFrontBufferData          = 33, 
		StretchRect                 = 34, 
		ColorFill                   = 35, 
		CreateOffscreenPlainSurface = 36, 
		SetRenderTarget             = 37, 
		GetRenderTarget             = 38, 
		SetDepthStencilSurface      = 39, 
		GetDepthStencilSurface      = 40, 
		BeginScene                  = 41, 
		EndScene                    = 42, 
		Clear                       = 43, 
		SetTransform                = 44, 
		GetTransform                = 45, 
		MultiplyTransform           = 46, 
		SetViewport                 = 47, 
		GetViewport                 = 48, 
		SetMaterial                 = 49, 
		GetMaterial                 = 50, 
		SetLight                    = 51, 
		GetLight                    = 52, 
		LightEnable                 = 53, 
		GetLightEnable              = 54, 
		SetClipPlane                = 55, 
		GetClipPlane                = 56, 
		SetRenderState              = 57, 
		GetRenderState              = 58, 
		CreateStateBlock            = 59, 
		BeginStateBlock             = 60, 
		EndStateBlock               = 61, 
		SetClipStatus               = 62, 
		GetClipStatus               = 63, 
		GetTexture                  = 64, 
		SetTexture                  = 65, 
		GetTextureStageState        = 66, 
		SetTextureStageState        = 67, 
		GetSamplerState             = 68, 
		SetSamplerState             = 69, 
		ValidateDevice              = 70, 
		SetPaletteEntries           = 71, 
		GetPaletteEntries           = 72, 
		SetCurrentTexturePalette    = 73, 
		GetCurrentTexturePalette    = 74, 
		SetScissorRect              = 75, 
		GetScissorRect              = 76, 
		SetSoftwareVertexProcessing = 77, 
		GetSoftwareVertexProcessing = 78, 
		SetNPatchMode               = 79, 
		GetNPatchMode               = 80, 
		DrawPrimitive               = 81, 
		DrawIndexedPrimitive        = 82, 
		DrawPrimitiveUP             = 83, 
		DrawIndexedPrimitiveUP      = 84, 
		ProcessVertices             = 85, 
		CreateVertexDeclaration     = 86, 
		SetVertexDeclaration        = 87, 
		GetVertexDeclaration        = 88, 
		SetFVF                      = 89, 
		GetFVF                      = 90, 
		CreateVertexShader          = 91, 
		SetVertexShader             = 92, 
		GetVertexShader             = 93, 
		SetVertexShaderConstantF    = 94, 
		GetVertexShaderConstantF    = 95, 
		SetVertexShaderConstantI    = 96, 
		GetVertexShaderConstantI    = 97, 
		SetVertexShaderConstantB    = 98, 
		GetVertexShaderConstantB    = 99, 
		SetStreamSource             = 100, 
		GetStreamSource             = 101, 
		SetStreamSourceFreq         = 102, 
		GetStreamSourceFreq         = 103, 
		SetIndices                  = 104, 
		GetIndices                  = 105, 
		CreatePixelShader           = 106, 
		SetPixelShader              = 107, 
		GetPixelShader              = 108, 
		SetPixelShaderConstantF     = 109, 
		GetPixelShaderConstantF     = 110, 
		SetPixelShaderConstantI     = 111, 
		GetPixelShaderConstantI     = 112, 
		SetPixelShaderConstantB     = 113, 
		GetPixelShaderConstantB     = 114, 
		DrawRectPatch               = 115, 
		DrawTriPatch                = 116, 
		DeletePatch                 = 117, 
		CreateQuery                 = 118, 
	};

}
#endif