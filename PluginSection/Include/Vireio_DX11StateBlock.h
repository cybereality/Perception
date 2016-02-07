/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_DX11StateBlock.h> :
Copyright (C) 2015 Denis Reischl
(based on structure D3DX11_STATE_BLOCK + methods of unknown origin)

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#ifndef __d3d11_1_h__
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")
#endif

#ifndef __d3d11_h__
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#endif

/**
* DirectX 11 state block structure.
* (D3D11 has no state blocks any more...)
***/
struct D3DX11_STATE_BLOCK
{
	ID3D11VertexShader*       VS;
	ID3D11SamplerState*       VSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* VSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             VSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      VSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      VSInterfaceCount;

	ID3D11GeometryShader*     GS;
	ID3D11SamplerState*       GSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* GSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             GSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      GSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      GSInterfaceCount;

	ID3D11HullShader*         HS;
	ID3D11SamplerState*       HSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* HSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             HSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      HSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      HSInterfaceCount;

	ID3D11DomainShader*       DS;
	ID3D11SamplerState*       DSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* DSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             DSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      DSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      DSInterfaceCount;

	ID3D11PixelShader*        PS;
	ID3D11SamplerState*       PSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* PSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             PSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      PSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      PSInterfaceCount;

	ID3D11ComputeShader*      CS;
	ID3D11SamplerState*       CSSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	ID3D11ShaderResourceView* CSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             CSConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11ClassInstance*      CSInterfaces[D3D11_SHADER_MAX_INTERFACES];
	UINT                      CSInterfaceCount;
	ID3D11UnorderedAccessView*CSUnorderedAccessViews[D3D11_PS_CS_UAV_REGISTER_COUNT];

	ID3D11Buffer*             IAVertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT                      IAVertexBuffersStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT                      IAVertexBuffersOffsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11Buffer*             IAIndexBuffer;
	DXGI_FORMAT               IAIndexBufferFormat;
	UINT                      IAIndexBufferOffset;
	ID3D11InputLayout*        IAInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY  IAPrimitiveTopology;

	ID3D11RenderTargetView*   OMRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView*   OMRenderTargetStencilView;
	ID3D11UnorderedAccessView*OMUnorderedAccessViews[D3D11_PS_CS_UAV_REGISTER_COUNT];
	ID3D11DepthStencilState*  OMDepthStencilState;
	UINT                      OMDepthStencilRef;
	ID3D11BlendState*         OMBlendState;
	FLOAT                     OMBlendFactor[4];
	UINT                      OMSampleMask;

	UINT                      RSViewportCount;
	D3D11_VIEWPORT            RSViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT                      RSScissorRectCount;
	D3D11_RECT                RSScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	ID3D11RasterizerState*    RSRasterizerState;
	ID3D11Buffer*             SOBuffers[4];
	ID3D11Predicate*          Predication;
	BOOL                      PredicationValue;
};

/**
* Small release helper.
***/
inline void SafeRelease(UINT dwCount, IUnknown** ppInterfaces)
{
	for (UINT dw = 0; dw < dwCount; dw++)
	if (ppInterfaces[dw])
		ppInterfaces[dw]->Release();
}

/**
* Create dx11 stateblocks.
* MS Docs about ->ClearState() :
* This sets all
*     - input/output resource slots,     -> shaders (different stages)
*     - shaders,                         -> shaders (different stages)
*     - input layouts,                   -> input assembler stage
*     - predications,                    -> predications
*     - scissor rectangles,              -> rasterizer stage
*     - depth-stencil state,             -> output merger stage
*     - rasterizer state,                -> rasterizer stage
*     - blend state,                     -> output merger stage
*     - sampler state,                   -> shaders (different stages)
*     - and viewports                    -> rasterizer stage
* to NULL.
* The primitive topology is set to UNDEFINED.  -> input assembler stage
*
* Instantly RELEASE all IUnknown interfaces here since their ref count will be increased.
***/
void CreateStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	UINT dw = 0;
	memset(sb, 0, sizeof(D3DX11_STATE_BLOCK));

	// vertex shader
	dc->VSGetShader(&sb->VS, sb->VSInterfaces, &sb->VSInterfaceCount);
	if (sb->VS) sb->VS->Release();
	if (sb->VSInterfaceCount)
	{
		SafeRelease(sb->VSInterfaceCount, (IUnknown**)sb->VSInterfaces);
	}
	dc->VSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->VSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->VSSamplers);
	dc->VSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->VSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->VSShaderResources);
	dc->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->VSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->VSConstantBuffers);

	// geometry shader
	dc->GSGetShader(&sb->GS, sb->GSInterfaces, &sb->GSInterfaceCount);
	if (sb->GS) sb->GS->Release();
	if (sb->GSInterfaceCount)
	{
		SafeRelease(sb->GSInterfaceCount, (IUnknown**)sb->GSInterfaces);
	}
	dc->GSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->GSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->GSSamplers);
	dc->GSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->GSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->GSShaderResources);
	dc->GSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->GSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->GSConstantBuffers);

	// hull shader
	dc->HSGetShader(&sb->HS, sb->HSInterfaces, &sb->HSInterfaceCount);
	if (sb->HS) sb->HS->Release();
	if (sb->HSInterfaceCount)
	{
		SafeRelease(sb->HSInterfaceCount, (IUnknown**)sb->HSInterfaces);
	}
	dc->HSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->HSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->HSSamplers);
	dc->HSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->HSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->HSShaderResources);
	dc->HSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->HSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->HSConstantBuffers);

	// domain shader
	dc->DSGetShader(&sb->DS, sb->DSInterfaces, &sb->DSInterfaceCount);
	if (sb->DS) sb->DS->Release();
	if (sb->DSInterfaceCount)
	{
		SafeRelease(sb->DSInterfaceCount, (IUnknown**)sb->DSInterfaces);
	}
	dc->DSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->DSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->DSSamplers);
	dc->DSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->DSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->DSShaderResources);
	dc->DSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->DSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->DSConstantBuffers);

	// pixel shader
	dc->PSGetShader(&sb->PS, sb->PSInterfaces, &sb->PSInterfaceCount);
	if (sb->PS) sb->PS->Release();
	if (sb->PSInterfaceCount)
	{
		SafeRelease(sb->PSInterfaceCount, (IUnknown**)sb->PSInterfaces);
	}
	dc->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->PSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->PSSamplers);
	dc->PSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->PSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->PSShaderResources);
	dc->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->PSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->PSConstantBuffers);

	// compute shader
	dc->CSGetShader(&sb->CS, sb->CSInterfaces, &sb->CSInterfaceCount);
	if (sb->CS) sb->CS->Release();
	if (sb->CSInterfaceCount)
	{
		SafeRelease(sb->CSInterfaceCount, (IUnknown**)sb->CSInterfaces);
	}
	dc->CSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->CSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sb->CSSamplers);
	dc->CSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->CSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->CSShaderResources);
	dc->CSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->CSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sb->CSConstantBuffers);
	// can we ignore unordered access views here ? i assume we can...
	// dc->CSGetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews);

	// input assembler stage
	dc->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	SafeRelease(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sb->IAVertexBuffers);
	dc->IAGetIndexBuffer(&sb->IAIndexBuffer, &sb->IAIndexBufferFormat, &sb->IAIndexBufferOffset);
	if (sb->IAIndexBuffer) sb->IAIndexBuffer->Release();
	dc->IAGetInputLayout(&sb->IAInputLayout);
	if (sb->IAInputLayout) sb->IAInputLayout->Release();
	dc->IAGetPrimitiveTopology(&sb->IAPrimitiveTopology);

	// output merger stage // can we ignore unordered access views here ? i assume we can...
	//dc->OMGetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, &sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews);
	dc->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, &sb->OMRenderTargetStencilView);
	SafeRelease(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, (IUnknown**)sb->OMRenderTargets);
	dc->OMGetDepthStencilState(&sb->OMDepthStencilState, &sb->OMDepthStencilRef);
	if (sb->OMDepthStencilState) sb->OMDepthStencilState->Release();
	dc->OMGetBlendState(&sb->OMBlendState, sb->OMBlendFactor, &sb->OMSampleMask);
	if (sb->OMBlendState) sb->OMBlendState->Release();

	// rasterizer stage
	sb->RSViewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetViewports(&sb->RSViewportCount, sb->RSViewports);
	sb->RSScissorRectCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetScissorRects(&sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSGetState(&sb->RSRasterizerState);
	if (sb->RSRasterizerState) sb->RSRasterizerState->Release();

	// stream-output stage, predications
	dc->SOGetTargets(4, sb->SOBuffers);
	SafeRelease(4, (IUnknown**)sb->SOBuffers);
	dc->GetPredication(&sb->Predication, &sb->PredicationValue);
	if (sb->Predication) sb->Predication->Release();
}

/**
* Apply dx11 stateblocks.
* MS Docs about ->ClearState() :
* This sets all
*     - input/output resource slots,     -> shaders (different stages)
*     - shaders,                         -> shaders (different stages)
*     - input layouts,                   -> input assembler stage
*     - predications,                    -> predications
*     - scissor rectangles,              -> rasterizer stage
*     - depth-stencil state,             -> output merger stage
*     - rasterizer state,                -> rasterizer stage
*     - blend state,                     -> output merger stage
*     - sampler state,                   -> shaders (different stages)
*     - and viewports                    -> rasterizer stage
* to NULL.
* The primitive topology is set to UNDEFINED.  -> input assembler stage
***/
void ApplyStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	// can we ignore unordered access views here ? i assume we can...
	// UINT minus_one[D3D11_PS_CS_UAV_REGISTER_COUNT];
	// memset(minus_one, -1, sizeof(minus_one));

	// vertex shader
	dc->VSSetShader(sb->VS, sb->VSInterfaces, sb->VSInterfaceCount);
	dc->VSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->VSSamplers);
	dc->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->VSShaderResources);
	dc->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->VSConstantBuffers);

	// geometry shader
	dc->GSSetShader(sb->GS, sb->GSInterfaces, sb->GSInterfaceCount);
	dc->GSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->GSSamplers);
	dc->GSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->GSShaderResources);
	dc->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->GSConstantBuffers);

	// hull shader
	dc->HSSetShader(sb->HS, sb->HSInterfaces, sb->HSInterfaceCount);
	dc->HSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->HSSamplers);
	dc->HSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->HSShaderResources);
	dc->HSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->HSConstantBuffers);

	// domain shader
	dc->DSSetShader(sb->DS, sb->DSInterfaces, sb->DSInterfaceCount);
	dc->DSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->DSSamplers);
	dc->DSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->DSShaderResources);
	dc->DSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->DSConstantBuffers);

	// pixel shader
	dc->PSSetShader(sb->PS, sb->PSInterfaces, sb->PSInterfaceCount);
	dc->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->PSSamplers);
	dc->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->PSShaderResources);
	dc->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->PSConstantBuffers);

	// compute shader
	dc->CSSetShader(sb->CS, sb->CSInterfaces, sb->CSInterfaceCount);
	dc->CSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->CSSamplers);
	dc->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->CSShaderResources);
	dc->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->CSConstantBuffers);
	//dc->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews, minus_one);

	// input assembler stage
	dc->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	dc->IASetIndexBuffer(sb->IAIndexBuffer, sb->IAIndexBufferFormat, sb->IAIndexBufferOffset);
	dc->IASetInputLayout(sb->IAInputLayout);
	dc->IASetPrimitiveTopology(sb->IAPrimitiveTopology);

	// output merger stage
	//dc->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews, minus_one);
	dc->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, sb->OMRenderTargetStencilView);
	dc->OMSetDepthStencilState(sb->OMDepthStencilState, sb->OMDepthStencilRef);
	dc->OMSetBlendState(sb->OMBlendState, sb->OMBlendFactor, sb->OMSampleMask);

	// rasterizer stage
	dc->RSSetViewports(sb->RSViewportCount, sb->RSViewports);
	dc->RSSetScissorRects(sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSSetState(sb->RSRasterizerState);

	// stream-output stage, predications
	UINT SOBuffersOffsets[4] = { 0 }; /* (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 0,
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 1,
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 2,
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 3 };*/

	dc->SOSetTargets(4, sb->SOBuffers, SOBuffersOffsets);
	dc->SetPredication(sb->Predication, sb->PredicationValue);
}