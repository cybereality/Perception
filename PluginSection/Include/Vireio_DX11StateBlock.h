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
* Tiny helper.
***/
template<class T>
UINT calc_count(T** arr, UINT max_count)
{
    for (size_t i = 0; i < max_count; ++i)
        if (arr[i] == 0)
            return (UINT)i;
    return max_count;
}

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
* Create dx11 stateblocks.
***/
void CreateStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	memset(sb, 0, sizeof(D3DX11_STATE_BLOCK));

	dc->VSGetShader(&sb->VS, sb->VSInterfaces, &sb->VSInterfaceCount);
	dc->VSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->VSSamplers);
	dc->VSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->VSShaderResources);
	dc->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->VSConstantBuffers);

	dc->GSGetShader(&sb->GS, sb->GSInterfaces, &sb->GSInterfaceCount);
	dc->GSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->GSSamplers);
	dc->GSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->GSShaderResources);
	dc->GSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->GSConstantBuffers);

	dc->HSGetShader(&sb->HS, sb->HSInterfaces, &sb->HSInterfaceCount);
	dc->HSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->HSSamplers);
	dc->HSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->HSShaderResources);
	dc->HSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->HSConstantBuffers);

	dc->DSGetShader(&sb->DS, sb->DSInterfaces, &sb->DSInterfaceCount);
	dc->DSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->DSSamplers);
	dc->DSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->DSShaderResources);
	dc->DSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->DSConstantBuffers);

	dc->PSGetShader(&sb->PS, sb->PSInterfaces, &sb->PSInterfaceCount);
	dc->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->PSSamplers);
	dc->PSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->PSShaderResources);
	dc->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->PSConstantBuffers);

	dc->CSGetShader(&sb->CS, sb->CSInterfaces, &sb->CSInterfaceCount);
	dc->CSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, sb->CSSamplers);
	dc->CSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, sb->CSShaderResources);
	dc->CSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, sb->CSConstantBuffers);
	dc->CSGetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews);

	dc->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	dc->IAGetIndexBuffer(&sb->IAIndexBuffer, &sb->IAIndexBufferFormat, &sb->IAIndexBufferOffset);
	dc->IAGetInputLayout(&sb->IAInputLayout);
	dc->IAGetPrimitiveTopology(&sb->IAPrimitiveTopology);

	dc->OMGetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, &sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews);
	dc->OMGetDepthStencilState(&sb->OMDepthStencilState, &sb->OMDepthStencilRef);
	dc->OMGetBlendState(&sb->OMBlendState, sb->OMBlendFactor, &sb->OMSampleMask);

	sb->RSViewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetViewports(&sb->RSViewportCount, sb->RSViewports);
	sb->RSScissorRectCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	dc->RSGetScissorRects(&sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSGetState(&sb->RSRasterizerState);

	dc->SOGetTargets(4, sb->SOBuffers);
	dc->GetPredication(&sb->Predication, &sb->PredicationValue);
}

/**
* Apply dx11 stateblocks.
***/
void ApplyStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb)
{
	UINT minus_one[D3D11_PS_CS_UAV_REGISTER_COUNT];
	memset(minus_one, -1, sizeof(minus_one));
	dc->VSSetShader(sb->VS, sb->VSInterfaces, sb->VSInterfaceCount);
	UINT VSSamplerCount = calc_count(sb->VSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (VSSamplerCount)
		dc->VSSetSamplers(0, VSSamplerCount, sb->VSSamplers);
	UINT VSShaderResourceCount = calc_count(sb->VSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (VSShaderResourceCount)
		dc->VSSetShaderResources(0, VSShaderResourceCount, sb->VSShaderResources);
	UINT VSConstantBufferCount = calc_count(sb->VSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (VSConstantBufferCount)
		dc->VSSetConstantBuffers(0, VSConstantBufferCount, sb->VSConstantBuffers);

	dc->GSSetShader(sb->GS, sb->GSInterfaces, sb->GSInterfaceCount);
	UINT GSSamplerCount = calc_count(sb->GSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (GSSamplerCount)
		dc->GSSetSamplers(0, GSSamplerCount, sb->GSSamplers);
	UINT GSShaderResourceCount = calc_count(sb->GSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (GSShaderResourceCount)
		dc->GSSetShaderResources(0, GSShaderResourceCount, sb->GSShaderResources);
	UINT GSConstantBufferCount = calc_count(sb->GSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (GSConstantBufferCount)
		dc->GSSetConstantBuffers(0, GSConstantBufferCount, sb->GSConstantBuffers);

	dc->HSSetShader(sb->HS, sb->HSInterfaces, sb->HSInterfaceCount);
	UINT HSSamplerCount = calc_count(sb->HSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (HSSamplerCount)
		dc->HSSetSamplers(0, HSSamplerCount, sb->HSSamplers);
	UINT HSShaderResourceCount = calc_count(sb->HSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (HSShaderResourceCount)
		dc->HSSetShaderResources(0, HSShaderResourceCount, sb->HSShaderResources);
	UINT HSConstantBufferCount = calc_count(sb->HSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (HSConstantBufferCount)
		dc->HSSetConstantBuffers(0, HSConstantBufferCount, sb->HSConstantBuffers);

	dc->DSSetShader(sb->DS, sb->DSInterfaces, sb->DSInterfaceCount);
	UINT DSSamplerCount = calc_count(sb->DSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (DSSamplerCount)
		dc->DSSetSamplers(0, DSSamplerCount, sb->DSSamplers);
	UINT DSShaderResourceCount = calc_count(sb->DSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (DSShaderResourceCount)
		dc->DSSetShaderResources(0, DSShaderResourceCount, sb->DSShaderResources);
	UINT DSConstantBufferCount = calc_count(sb->DSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (DSConstantBufferCount)
		dc->DSSetConstantBuffers(0, DSConstantBufferCount, sb->DSConstantBuffers);

	dc->PSSetShader(sb->PS, sb->PSInterfaces, sb->PSInterfaceCount);
	UINT PSSamplerCount = calc_count(sb->PSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (PSSamplerCount)
		dc->PSSetSamplers(0, PSSamplerCount, sb->PSSamplers);
	UINT PSShaderResourceCount = calc_count(sb->PSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (PSShaderResourceCount)
		dc->PSSetShaderResources(0, PSShaderResourceCount, sb->PSShaderResources);
	UINT PSConstantBufferCount = calc_count(sb->PSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (PSConstantBufferCount)
		dc->PSSetConstantBuffers(0, PSConstantBufferCount, sb->PSConstantBuffers);

	dc->CSSetShader(sb->CS, sb->CSInterfaces, sb->CSInterfaceCount);
	UINT CSSamplerCount = calc_count(sb->CSSamplers, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	if (CSSamplerCount)
		dc->CSSetSamplers(0, CSSamplerCount, sb->CSSamplers);
	UINT CSShaderResourceCount = calc_count(sb->CSShaderResources, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	if (CSShaderResourceCount)
		dc->CSSetShaderResources(0, CSShaderResourceCount, sb->CSShaderResources);
	UINT CSConstantBufferCount = calc_count(sb->CSConstantBuffers, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	if (CSConstantBufferCount)
		dc->CSSetConstantBuffers(0, CSConstantBufferCount, sb->CSConstantBuffers);
	//dc->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->CSUnorderedAccessViews, minus_one);

	UINT IAVertexBufferCount = calc_count(sb->IAVertexBuffers, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
	if (IAVertexBufferCount)
		dc->IASetVertexBuffers(0, IAVertexBufferCount, sb->IAVertexBuffers, sb->IAVertexBuffersStrides, sb->IAVertexBuffersOffsets);
	dc->IASetIndexBuffer(sb->IAIndexBuffer, sb->IAIndexBufferFormat, sb->IAIndexBufferOffset);
	dc->IASetInputLayout(sb->IAInputLayout);
	dc->IASetPrimitiveTopology(sb->IAPrimitiveTopology);

	//dc->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, sb->OMRenderTargets, sb->OMRenderTargetStencilView, 0, D3D11_PS_CS_UAV_REGISTER_COUNT, sb->OMUnorderedAccessViews, minus_one);
	UINT OMRenderTargetCount = calc_count(sb->OMRenderTargets, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
	if (OMRenderTargetCount)
		dc->OMSetRenderTargets(OMRenderTargetCount, sb->OMRenderTargets, sb->OMRenderTargetStencilView);
	dc->OMSetDepthStencilState(sb->OMDepthStencilState, sb->OMDepthStencilRef);
	dc->OMSetBlendState(sb->OMBlendState, sb->OMBlendFactor, sb->OMSampleMask);

	dc->RSSetViewports(sb->RSViewportCount, sb->RSViewports);
	dc->RSSetScissorRects(sb->RSScissorRectCount, sb->RSScissorRects);
	dc->RSSetState(sb->RSRasterizerState);

	UINT SOBuffersOffsets[4] = { 0 }; /* (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 0,
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 1, 
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 2, 
									  (sizeof(sb->SOBuffers) / sizeof(sb->SOBuffers[0])) * 3 };*/

	UINT SOBufferCount = calc_count(sb->SOBuffers, 4);
	if (SOBufferCount)
		dc->SOSetTargets(SOBufferCount, sb->SOBuffers, SOBuffersOffsets);
	dc->SetPredication(sb->Predication, sb->PredicationValue);
}