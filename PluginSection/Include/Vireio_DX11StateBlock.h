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
void CreateStateblock(ID3D11DeviceContext* pcContext, D3DX11_STATE_BLOCK* sStateBlock)
{
	UINT dw = 0;
	memset(sStateBlock, 0, sizeof(D3DX11_STATE_BLOCK));

	// vertex shader
	pcContext->VSGetShader(&sStateBlock->VS, sStateBlock->VSInterfaces, &sStateBlock->VSInterfaceCount);
	if (sStateBlock->VS) sStateBlock->VS->Release();
	if (sStateBlock->VSInterfaceCount)
	{
		SafeRelease(sStateBlock->VSInterfaceCount, (IUnknown**)sStateBlock->VSInterfaces);
	}
	pcContext->VSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->VSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->VSSamplers);
	pcContext->VSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->VSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->VSShaderResources);
	pcContext->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->VSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->VSConstantBuffers);

	// geometry shader
	pcContext->GSGetShader(&sStateBlock->GS, sStateBlock->GSInterfaces, &sStateBlock->GSInterfaceCount);
	if (sStateBlock->GS) sStateBlock->GS->Release();
	if (sStateBlock->GSInterfaceCount)
	{
		SafeRelease(sStateBlock->GSInterfaceCount, (IUnknown**)sStateBlock->GSInterfaces);
	}
	pcContext->GSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->GSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->GSSamplers);
	pcContext->GSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->GSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->GSShaderResources);
	pcContext->GSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->GSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->GSConstantBuffers);

	// hull shader
	pcContext->HSGetShader(&sStateBlock->HS, sStateBlock->HSInterfaces, &sStateBlock->HSInterfaceCount);
	if (sStateBlock->HS) sStateBlock->HS->Release();
	if (sStateBlock->HSInterfaceCount)
	{
		SafeRelease(sStateBlock->HSInterfaceCount, (IUnknown**)sStateBlock->HSInterfaces);
	}
	pcContext->HSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->HSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->HSSamplers);
	pcContext->HSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->HSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->HSShaderResources);
	pcContext->HSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->HSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->HSConstantBuffers);

	// domain shader
	pcContext->DSGetShader(&sStateBlock->DS, sStateBlock->DSInterfaces, &sStateBlock->DSInterfaceCount);
	if (sStateBlock->DS) sStateBlock->DS->Release();
	if (sStateBlock->DSInterfaceCount)
	{
		SafeRelease(sStateBlock->DSInterfaceCount, (IUnknown**)sStateBlock->DSInterfaces);
	}
	pcContext->DSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->DSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->DSSamplers);
	pcContext->DSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->DSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->DSShaderResources);
	pcContext->DSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->DSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->DSConstantBuffers);

	// pixel shader
	pcContext->PSGetShader(&sStateBlock->PS, sStateBlock->PSInterfaces, &sStateBlock->PSInterfaceCount);
	if (sStateBlock->PS) sStateBlock->PS->Release();
	if (sStateBlock->PSInterfaceCount)
	{
		SafeRelease(sStateBlock->PSInterfaceCount, (IUnknown**)sStateBlock->PSInterfaces);
	}
	pcContext->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->PSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->PSSamplers);
	pcContext->PSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->PSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->PSShaderResources);
	pcContext->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->PSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->PSConstantBuffers);

	// compute shader.... IGNORE HERE
	/* pcContext->CSGetShader(&sStateBlock->CS, sStateBlock->CSInterfaces, &sStateBlock->CSInterfaceCount);
	if (sStateBlock->CS) sStateBlock->CS->Release();
	if (sStateBlock->CSInterfaceCount)
	{
		SafeRelease(sStateBlock->CSInterfaceCount, (IUnknown**)sStateBlock->CSInterfaces);
	}
	pcContext->CSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->CSSamplers);
	SafeRelease(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (IUnknown**)sStateBlock->CSSamplers);
	pcContext->CSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->CSShaderResources);
	SafeRelease(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->CSShaderResources);
	pcContext->CSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->CSConstantBuffers);
	SafeRelease(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (IUnknown**)sStateBlock->CSConstantBuffers);*/
	// can we ignore unordered access views here ? i assume we can...
	// pcContext->CSGetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sStateBlock->CSUnorderedAccessViews);

	// input assembler stage
	pcContext->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->IAVertexBuffers, sStateBlock->IAVertexBuffersStrides, sStateBlock->IAVertexBuffersOffsets);
	SafeRelease(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, (IUnknown**)sStateBlock->IAVertexBuffers);
	pcContext->IAGetIndexBuffer(&sStateBlock->IAIndexBuffer, &sStateBlock->IAIndexBufferFormat, &sStateBlock->IAIndexBufferOffset);
	if (sStateBlock->IAIndexBuffer) sStateBlock->IAIndexBuffer->Release();
	pcContext->IAGetInputLayout(&sStateBlock->IAInputLayout);
	if (sStateBlock->IAInputLayout) sStateBlock->IAInputLayout->Release();
	pcContext->IAGetPrimitiveTopology(&sStateBlock->IAPrimitiveTopology);

	// output merger stage // can we ignore unordered access views here ? i assume we can...
	//pcContext->OMGetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sStateBlock->OMRenderTargets, &sStateBlock->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sStateBlock->OMUnorderedAccessViews);
	pcContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sStateBlock->OMRenderTargets, &sStateBlock->OMRenderTargetStencilView);
	SafeRelease(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, (IUnknown**)sStateBlock->OMRenderTargets);
	pcContext->OMGetDepthStencilState(&sStateBlock->OMDepthStencilState, &sStateBlock->OMDepthStencilRef);
	if (sStateBlock->OMDepthStencilState) sStateBlock->OMDepthStencilState->Release();
	pcContext->OMGetBlendState(&sStateBlock->OMBlendState, sStateBlock->OMBlendFactor, &sStateBlock->OMSampleMask);
	if (sStateBlock->OMBlendState) sStateBlock->OMBlendState->Release();

	// rasterizer stage
	sStateBlock->RSViewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	pcContext->RSGetViewports(&sStateBlock->RSViewportCount, sStateBlock->RSViewports);
	sStateBlock->RSScissorRectCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	pcContext->RSGetScissorRects(&sStateBlock->RSScissorRectCount, sStateBlock->RSScissorRects);
	pcContext->RSGetState(&sStateBlock->RSRasterizerState);
	if (sStateBlock->RSRasterizerState) sStateBlock->RSRasterizerState->Release();

	// stream-output stage, predications
	pcContext->SOGetTargets(4, sStateBlock->SOBuffers);
	SafeRelease(4, (IUnknown**)sStateBlock->SOBuffers);
	pcContext->GetPredication(&sStateBlock->Predication, &sStateBlock->PredicationValue);
	if (sStateBlock->Predication) sStateBlock->Predication->Release();
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
void ApplyStateblock(ID3D11DeviceContext* pcContext, D3DX11_STATE_BLOCK* sStateBlock)
{
	// can we ignore unordered access views here ? i assume we can...
	// UINT minus_one[D3D11_PS_CS_UAV_REGISTER_COUNT];
	// memset(minus_one, -1, sizeof(minus_one));

	// vertex shader
	pcContext->VSSetShader(sStateBlock->VS, sStateBlock->VSInterfaces, sStateBlock->VSInterfaceCount);
	pcContext->VSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->VSSamplers);
	pcContext->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->VSShaderResources);
	pcContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->VSConstantBuffers);

	// geometry shader
	pcContext->GSSetShader(sStateBlock->GS, sStateBlock->GSInterfaces, sStateBlock->GSInterfaceCount);
	pcContext->GSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->GSSamplers);
	pcContext->GSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->GSShaderResources);
	pcContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->GSConstantBuffers);

	// hull shader
	pcContext->HSSetShader(sStateBlock->HS, sStateBlock->HSInterfaces, sStateBlock->HSInterfaceCount);
	pcContext->HSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->HSSamplers);
	pcContext->HSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->HSShaderResources);
	pcContext->HSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->HSConstantBuffers);

	// domain shader
	pcContext->DSSetShader(sStateBlock->DS, sStateBlock->DSInterfaces, sStateBlock->DSInterfaceCount);
	pcContext->DSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->DSSamplers);
	pcContext->DSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->DSShaderResources);
	pcContext->DSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->DSConstantBuffers);

	// pixel shader
	pcContext->PSSetShader(sStateBlock->PS, sStateBlock->PSInterfaces, sStateBlock->PSInterfaceCount);
	pcContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->PSSamplers);
	pcContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->PSShaderResources);
	pcContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->PSConstantBuffers);

	// compute shader IGNORE HERE !
	/*pcContext->CSSetShader(sStateBlock->CS, sStateBlock->CSInterfaces, sStateBlock->CSInterfaceCount);
	pcContext->CSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sStateBlock->CSSamplers);
	pcContext->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->CSShaderResources);
	pcContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sStateBlock->CSConstantBuffers);*/
	//pcContext->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sStateBlock->CSUnorderedAccessViews, minus_one);

	// input assembler stage
	pcContext->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sStateBlock->IAVertexBuffers, sStateBlock->IAVertexBuffersStrides, sStateBlock->IAVertexBuffersOffsets);
	pcContext->IASetIndexBuffer(sStateBlock->IAIndexBuffer, sStateBlock->IAIndexBufferFormat, sStateBlock->IAIndexBufferOffset);
	pcContext->IASetInputLayout(sStateBlock->IAInputLayout);
	pcContext->IASetPrimitiveTopology(sStateBlock->IAPrimitiveTopology);

	// output merger stage, IGNORE UNORDERED ACCESS VIEWS (= COMPUTE SHADER)
	//pcContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sStateBlock->OMRenderTargets, sStateBlock->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sStateBlock->OMUnorderedAccessViews, minus_one);
	pcContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sStateBlock->OMRenderTargets, sStateBlock->OMRenderTargetStencilView);
	pcContext->OMSetDepthStencilState(sStateBlock->OMDepthStencilState, sStateBlock->OMDepthStencilRef);
	pcContext->OMSetBlendState(sStateBlock->OMBlendState, sStateBlock->OMBlendFactor, sStateBlock->OMSampleMask);

	// rasterizer stage
	pcContext->RSSetViewports(sStateBlock->RSViewportCount, sStateBlock->RSViewports);
	pcContext->RSSetScissorRects(sStateBlock->RSScissorRectCount, sStateBlock->RSScissorRects);
	pcContext->RSSetState(sStateBlock->RSRasterizerState);

	// stream-output stage, predications
	UINT SOBuffersOffsets[4] = { 0 }; /* (sizeof(sStateBlock->SOBuffers) / sizeof(sStateBlock->SOBuffers[0])) * 0,
									  (sizeof(sStateBlock->SOBuffers) / sizeof(sStateBlock->SOBuffers[0])) * 1,
									  (sizeof(sStateBlock->SOBuffers) / sizeof(sStateBlock->SOBuffers[0])) * 2,
									  (sizeof(sStateBlock->SOBuffers) / sizeof(sStateBlock->SOBuffers[0])) * 3 };*/

	pcContext->SOSetTargets(4, sStateBlock->SOBuffers, SOBuffersOffsets);
	pcContext->SetPredication(sStateBlock->Predication, sStateBlock->PredicationValue);
}

//
// This method resets any device context to the default settings. 
// This sets all input/output resource slots, shaders, input layouts, 
// predications, scissor rectangles, depth-stencil state, rasterizer state, 
// blend state, sampler state, and viewports to NULL. 
// The primitive topology is set to UNDEFINED.
void ClearContextState(ID3D11DeviceContext* pcContext)
{
	// vertex shader
	pcContext->VSSetShader(NULL, NULL, NULL);
	pcContext->VSSetSamplers(0, NULL, NULL);
	pcContext->VSSetShaderResources(0, NULL, NULL);
	pcContext->VSSetConstantBuffers(0, NULL, NULL);

	// geometry shader
	pcContext->GSSetShader(NULL, NULL, NULL);
	pcContext->GSSetSamplers(0, NULL, NULL);
	pcContext->GSSetShaderResources(0, NULL, NULL);
	pcContext->GSSetConstantBuffers(0, NULL, NULL);

	// hull shader
	pcContext->HSSetShader(NULL, NULL, NULL);
	pcContext->HSSetSamplers(0, NULL, NULL);
	pcContext->HSSetShaderResources(0, NULL, NULL);
	pcContext->HSSetConstantBuffers(0, NULL, NULL);

	// domain shader
	pcContext->DSSetShader(NULL, NULL, NULL);
	pcContext->DSSetSamplers(0, NULL, NULL);
	pcContext->DSSetShaderResources(0, NULL, NULL);
	pcContext->DSSetConstantBuffers(0, NULL, NULL);

	// pixel shader
	pcContext->PSSetShader(NULL, NULL, NULL);
	pcContext->PSSetSamplers(0, NULL, NULL);
	pcContext->PSSetShaderResources(0, NULL, NULL);
	pcContext->PSSetConstantBuffers(0, NULL, NULL);

	// compute shader... IGNORE HERE
	// pcContext->CSSetShader(NULL, NULL, NULL);
	// pcContext->CSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, NULL);
	// pcContext->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, NULL);
	// pcContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, NULL);
	// pcContext->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, NULL, NULL);

	// input assembler stage
	pcContext->IASetVertexBuffers(0, NULL, NULL, NULL, NULL);
	pcContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, NULL);
	pcContext->IASetInputLayout(NULL);
	pcContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED);

	// output merger stage, IGNORE UNORDERED ACCESS VIEWS
	// pcContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, NULL, NULL, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, NULL, NULL);
	pcContext->OMSetRenderTargets(NULL, NULL, NULL);
	pcContext->OMSetDepthStencilState(NULL, NULL);
	pcContext->OMSetBlendState(NULL, NULL, 0xffffffff);

	// rasterizer stage
	pcContext->RSSetViewports(NULL, NULL);
	pcContext->RSSetScissorRects(NULL, NULL);
	pcContext->RSSetState(NULL);

	// stream-output stage, predications
	pcContext->SOSetTargets(NULL, NULL, NULL);
	pcContext->SetPredication(NULL, NULL);
}