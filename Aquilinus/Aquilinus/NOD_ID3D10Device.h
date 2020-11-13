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
#ifndef NOD_ID3D10DEVICE
#define NOD_ID3D10DEVICE

#include "VMT_ID3D10Device.h"

#pragma region IUnknown nodes

/**
* HRESULT WINAPI QueryInterface (REFIID riid, void** ppvObj);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_QueryInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::QueryInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI AddRef(void);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_AddRef, 200, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::AddRef");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;
/**
* ULONG   WINAPI Release(void);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_Release, 210, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::Release");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D10Device nodes

/**
* void    WINAPI VSSetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSSetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSSetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSSetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView*const* ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSSetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSSetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSSetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSSetShader                          (ID3D10Device* pcThis, ID3D10PixelShader *pPixelShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSSetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSSetShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10PIXELSHADER, L"pPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSSetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSSetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState*const* ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSSetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSSetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSSetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSSetShader                          (ID3D10Device* pcThis, ID3D10VertexShader *pVertexShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSSetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSSetShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10VERTEXSHADER, L"pVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSSetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI DrawIndexed                          (ID3D10Device* pcThis, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_DrawIndexed, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::DrawIndexed");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"IndexCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartIndexLocation");
AQU_D3D_NODE_COMMANDER(AQU_INT, L"BaseVertexLocation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::DrawIndexed);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI Draw                                 (ID3D10Device* pcThis, UINT VertexCount, UINT StartVertexLocation);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_Draw, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::Draw");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"VertexCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartVertexLocation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::Draw);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSSetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSSetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSSetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IASetInputLayout                     (ID3D10Device* pcThis, ID3D10InputLayout *pInputLayout);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IASetInputLayout, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IASetInputLayout");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10INPUTLAYOUT, L"pInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IASetInputLayout);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IASetVertexBuffers                   (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IASetVertexBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IASetVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStrides");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IASetVertexBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IASetIndexBuffer                     (ID3D10Device* pcThis, ID3D10Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IASetIndexBuffer, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IASetIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10BUFFER, L"pIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Offset");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IASetIndexBuffer);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI DrawIndexedInstanced                 (ID3D10Device* pcThis, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_DrawIndexedInstanced, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::DrawIndexedInstanced");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"IndexCountPerInstance");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"InstanceCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartIndexLocation");
AQU_D3D_NODE_COMMANDER(AQU_INT, L"BaseVertexLocation");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartInstanceLocation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::DrawIndexedInstanced);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI DrawInstanced                        (ID3D10Device* pcThis, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_DrawInstanced, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::DrawInstanced");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"VertexCountPerInstance");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"InstanceCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartVertexLocation");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartInstanceLocation");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::DrawInstanced);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSSetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer*const* ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSSetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSSetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSSetShader                          (ID3D10Device* pcThis, ID3D10GeometryShader *pShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSSetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSSetShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10GEOMETRYSHADER, L"pShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSSetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IASetPrimitiveTopology               (ID3D10Device* pcThis, D3D10_PRIMITIVE_TOPOLOGY Topology);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IASetPrimitiveTopology, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IASetPrimitiveTopology");
AQU_D3D_NODE_COMMANDER(AQU_D3D10_PRIMITIVE_TOPOLOGY, L"Topology");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IASetPrimitiveTopology);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSSetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView*const* ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSSetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSSetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSSetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSSetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState*const* ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSSetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSSetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSSetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI SetPredication                       (ID3D10Device* pcThis, ID3D10Predicate *pPredicate, BOOL PredicateValue);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SetPredication, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SetPredication");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10PREDICATE, L"pPredicate");
AQU_D3D_NODE_COMMANDER(AQU_BOOL, L"PredicateValue");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SetPredication);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSSetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView*const* ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSSetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSSetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSSetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSSetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState*const* ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSSetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSSetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSSetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMSetRenderTargets                   (ID3D10Device* pcThis, UINT NumViews, ID3D10RenderTargetView*const* ppRenderTargetViews, ID3D10DepthStencilView *pDepthStencilView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMSetRenderTargets, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMSetRenderTargets");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRenderTargetViews");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILVIEW, L"pDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMSetRenderTargets);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMSetBlendState                      (ID3D10Device* pcThis, ID3D10BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMSetBlendState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMSetBlendState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10BLENDSTATE, L"pBlendState");
AQU_D3D_NODE_COMMANDER(AQU_VECTOR4F, L"BlendFactor");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SampleMask");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMSetBlendState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMSetDepthStencilState               (ID3D10Device* pcThis, ID3D10DepthStencilState *pDepthStencilState, UINT StencilRef);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMSetDepthStencilState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMSetDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILSTATE, L"pDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StencilRef");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMSetDepthStencilState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI SOSetTargets                         (ID3D10Device* pcThis, UINT NumBuffers, ID3D10Buffer*const* ppSOTargets, const UINT *pOffsets);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SOSetTargets, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SOSetTargets");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppSOTargets");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SOSetTargets);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI DrawAuto                             (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_DrawAuto, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::DrawAuto");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::DrawAuto);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSSetState                           (ID3D10Device* pcThis, ID3D10RasterizerState *pRasterizerState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSSetState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSSetState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RASTERIZERSTATE, L"pRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSSetState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSSetViewports                       (ID3D10Device* pcThis, UINT NumViewports, const D3D10_VIEWPORT *pViewports);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSSetViewports, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSSetViewports");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViewports");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_VIEWPORT, L"pViewports");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSSetViewports);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSSetScissorRects                    (ID3D10Device* pcThis, UINT NumRects, const D3D10_RECT *pRects);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSSetScissorRects, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSSetScissorRects");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumRects");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RECT, L"pRects");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSSetScissorRects);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI CopySubresourceRegion                (ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CopySubresourceRegion, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CopySubresourceRegion");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstX");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstY");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstZ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BOX, L"pSrcBox");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CopySubresourceRegion);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI CopyResource                         (ID3D10Device* pcThis, ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CopyResource, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CopyResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CopyResource);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI UpdateSubresource                    (ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_UpdateSubresource, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::UpdateSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BOX, L"pDstBox");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pSrcData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcRowPitch");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcDepthPitch");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::UpdateSubresource);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI ClearRenderTargetView                (ID3D10Device* pcThis, ID3D10RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ]);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_ClearRenderTargetView, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::ClearRenderTargetView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RENDERTARGETVIEW, L"pRenderTargetView");
AQU_D3D_NODE_COMMANDER(AQU_VECTOR4F, L"ColorRGBA");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::ClearRenderTargetView);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI ClearDepthStencilView                (ID3D10Device* pcThis, ID3D10DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_ClearDepthStencilView, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::ClearDepthStencilView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILVIEW, L"pDepthStencilView");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"ClearFlags");
AQU_D3D_NODE_COMMANDER(AQU_FLOAT, L"Depth");
AQU_D3D_NODE_COMMANDER(AQU_BYTE, L"Stencil");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::ClearDepthStencilView);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GenerateMips                         (ID3D10Device* pcThis, ID3D10ShaderResourceView *pShaderResourceView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GenerateMips, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GenerateMips");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10SHADERRESOURCEVIEW, L"pShaderResourceView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GenerateMips);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI ResolveSubresource                   (ID3D10Device* pcThis, ID3D10Resource *pDstResource, UINT DstSubresource, ID3D10Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_ResolveSubresource, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::ResolveSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcSubresource");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::ResolveSubresource);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSGetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSGetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSGetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSGetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSGetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSGetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSGetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSGetShader                          (ID3D10Device* pcThis, ID3D10PixelShader **ppPixelShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSGetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSGetShader");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PIXELSHADER, L"ppPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSGetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSGetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSGetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSGetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSGetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSGetShader                          (ID3D10Device* pcThis, ID3D10VertexShader **ppVertexShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSGetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSGetShader");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10VERTEXSHADER, L"ppVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSGetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI PSGetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_PSGetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::PSGetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::PSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IAGetInputLayout                     (ID3D10Device* pcThis, ID3D10InputLayout **ppInputLayout);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IAGetInputLayout, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IAGetInputLayout");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10INPUTLAYOUT, L"ppInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IAGetInputLayout);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IAGetVertexBuffers                   (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IAGetVertexBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IAGetVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStrides");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IAGetVertexBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IAGetIndexBuffer                     (ID3D10Device* pcThis, ID3D10Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IAGetIndexBuffer, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IAGetIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"pIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"Offset");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IAGetIndexBuffer);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSGetConstantBuffers                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSGetConstantBuffers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSGetConstantBuffers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSGetShader                          (ID3D10Device* pcThis, ID3D10GeometryShader **ppGeometryShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSGetShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSGetShader");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSGetShader);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI IAGetPrimitiveTopology               (ID3D10Device* pcThis, D3D10_PRIMITIVE_TOPOLOGY *pTopology);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_IAGetPrimitiveTopology, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::IAGetPrimitiveTopology");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_PRIMITIVE_TOPOLOGY, L"pTopology");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::IAGetPrimitiveTopology);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSGetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSGetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSGetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSGetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI VSGetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_VSGetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::VSGetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::VSGetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GetPredication                       (ID3D10Device* pcThis, ID3D10Predicate **ppPredicate, BOOL *pPredicateValue);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetPredication, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetPredication");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PREDICATE, L"ppPredicate");
AQU_D3D_NODE_COMMANDER(AQU_PNT_BOOL, L"pPredicateValue");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetPredication);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSGetShaderResources                 (ID3D10Device* pcThis, UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSGetShaderResources, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSGetShaderResources");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSGetShaderResources);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GSGetSamplers                        (ID3D10Device* pcThis, UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GSGetSamplers, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GSGetSamplers");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GSGetSamplers);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMGetRenderTargets                   (ID3D10Device* pcThis, UINT NumViews, ID3D10RenderTargetView **ppRenderTargetViews, ID3D10DepthStencilView **ppDepthStencilView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMGetRenderTargets, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMGetRenderTargets");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRenderTargetViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILVIEW, L"ppDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMGetRenderTargets);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMGetBlendState                      (ID3D10Device* pcThis, ID3D10BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMGetBlendState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMGetBlendState");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BLENDSTATE, L"ppBlendState");
AQU_D3D_NODE_COMMANDER(AQU_VECTOR4F, L"BlendFactor");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pSampleMask");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMGetBlendState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI OMGetDepthStencilState               (ID3D10Device* pcThis, ID3D10DepthStencilState **ppDepthStencilState, UINT *pStencilRef);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OMGetDepthStencilState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OMGetDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILSTATE, L"ppDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStencilRef");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OMGetDepthStencilState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI SOGetTargets                         (ID3D10Device* pcThis, UINT NumBuffers, ID3D10Buffer **ppSOTargets, UINT *pOffsets);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SOGetTargets, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SOGetTargets");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppSOTargets");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SOGetTargets);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSGetState                           (ID3D10Device* pcThis, ID3D10RasterizerState **ppRasterizerState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSGetState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSGetState");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RASTERIZERSTATE, L"ppRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSGetState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSGetViewports                       (ID3D10Device* pcThis, UINT *NumViewports, D3D10_VIEWPORT *pViewports);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSGetViewports, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSGetViewports");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"NumViewports");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_VIEWPORT, L"pViewports");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSGetViewports);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI RSGetScissorRects                    (ID3D10Device* pcThis, UINT *NumRects, D3D10_RECT *pRects);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_RSGetScissorRects, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::RSGetScissorRects");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"NumRects");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RECT, L"pRects");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::RSGetScissorRects);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetDeviceRemovedReason               (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetDeviceRemovedReason, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetDeviceRemovedReason");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetDeviceRemovedReason);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI SetExceptionMode                     (ID3D10Device* pcThis, UINT RaiseFlags);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SetExceptionMode, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SetExceptionMode");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"RaiseFlags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SetExceptionMode);
AQU_D3D_NODE_FOOTER___;
/**
* UINT    WINAPI GetExceptionMode                     (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetExceptionMode, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetExceptionMode");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetExceptionMode);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI GetPrivateData                       (ID3D10Device* pcThis, REFGUID guid, UINT *pDataSize, void *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetPrivateData, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetPrivateData);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI SetPrivateData                       (ID3D10Device* pcThis, REFGUID guid, UINT DataSize, const void *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SetPrivateData, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SetPrivateData");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SetPrivateData);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI SetPrivateDataInterface              (ID3D10Device* pcThis, REFGUID guid, const IUnknown *pData);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SetPrivateDataInterface, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SetPrivateDataInterface");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IUNKNOWN, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SetPrivateDataInterface);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI ClearState                           (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_ClearState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::ClearState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::ClearState);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI Flush                                (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_Flush, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::Flush");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::Flush);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateBuffer                         (ID3D10Device* pcThis, const D3D10_BUFFER_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateBuffer, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BUFFER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppBuffer");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateBuffer);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateTexture1D                      (ID3D10Device* pcThis, const D3D10_TEXTURE1D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture1D **ppTexture1D);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateTexture1D, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateTexture1D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE1D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE1D, L"ppTexture1D");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateTexture1D);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateTexture2D                      (ID3D10Device* pcThis, const D3D10_TEXTURE2D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture2D **ppTexture2D);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateTexture2D, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateTexture2D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE2D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE2D, L"ppTexture2D");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateTexture2D);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateTexture3D                      (ID3D10Device* pcThis, const D3D10_TEXTURE3D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture3D **ppTexture3D);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateTexture3D, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateTexture3D");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE3D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE3D, L"ppTexture3D");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateTexture3D);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateShaderResourceView             (ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D10ShaderResourceView **ppSRView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateShaderResourceView, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateShaderResourceView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SHADER_RESOURCE_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppSRView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateShaderResourceView);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateRenderTargetView               (ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_RENDER_TARGET_VIEW_DESC *pDesc, ID3D10RenderTargetView **ppRTView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateRenderTargetView, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateRenderTargetView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RENDER_TARGET_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRTView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateRenderTargetView);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateDepthStencilView               (ID3D10Device* pcThis, ID3D10Resource *pResource, const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D10DepthStencilView **ppDepthStencilView);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateDepthStencilView, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateDepthStencilView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_DEPTH_STENCIL_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILVIEW, L"ppDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateDepthStencilView);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateInputLayout                    (ID3D10Device* pcThis, const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout **ppInputLayout);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateInputLayout, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateInputLayout");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_INPUT_ELEMENT_DESC, L"pInputElementDescs");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumElements");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecodeWithInputSignature");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10INPUTLAYOUT, L"ppInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateInputLayout);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateVertexShader                   (ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateVertexShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateVertexShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10VERTEXSHADER, L"ppVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateVertexShader);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateGeometryShader                 (ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader **ppGeometryShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateGeometryShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateGeometryShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateGeometryShader);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput (ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D10_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader **ppGeometryShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateGeometryShaderWithStreamOutput, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateGeometryShaderWithStreamOutput");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SO_DECLARATION_ENTRY, L"pSODeclaration");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumEntries");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"OutputStreamStride");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreatePixelShader                    (ID3D10Device* pcThis, const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreatePixelShader, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreatePixelShader");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PIXELSHADER, L"ppPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreatePixelShader);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateBlendState                     (ID3D10Device* pcThis, const D3D10_BLEND_DESC *pBlendStateDesc, ID3D10BlendState **ppBlendState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateBlendState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateBlendState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BLEND_DESC, L"pBlendStateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BLENDSTATE, L"ppBlendState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateBlendState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateDepthStencilState              (ID3D10Device* pcThis, const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D10DepthStencilState **ppDepthStencilState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateDepthStencilState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_DEPTH_STENCIL_DESC, L"pDepthStencilDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILSTATE, L"ppDepthStencilState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateDepthStencilState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateRasterizerState                (ID3D10Device* pcThis, const D3D10_RASTERIZER_DESC *pRasterizerDesc, ID3D10RasterizerState **ppRasterizerState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateRasterizerState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateRasterizerState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RASTERIZER_DESC, L"pRasterizerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RASTERIZERSTATE, L"ppRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateRasterizerState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateSamplerState                   (ID3D10Device* pcThis, const D3D10_SAMPLER_DESC *pSamplerDesc, ID3D10SamplerState **ppSamplerState);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateSamplerState, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateSamplerState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SAMPLER_DESC, L"pSamplerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplerState");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateSamplerState);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateQuery                          (ID3D10Device* pcThis, const D3D10_QUERY_DESC *pQueryDesc, ID3D10Query **ppQuery);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateQuery, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateQuery");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_QUERY_DESC, L"pQueryDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10QUERY, L"ppQuery");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateQuery);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreatePredicate                      (ID3D10Device* pcThis, const D3D10_QUERY_DESC *pPredicateDesc, ID3D10Predicate **ppPredicate);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreatePredicate, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreatePredicate");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_QUERY_DESC, L"pPredicateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PREDICATE, L"ppPredicate");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreatePredicate);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CreateCounter                        (ID3D10Device* pcThis, const D3D10_COUNTER_DESC *pCounterDesc, ID3D10Counter **ppCounter);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CreateCounter, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CreateCounter");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_DESC, L"pCounterDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10COUNTER, L"ppCounter");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CreateCounter);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CheckFormatSupport                   (ID3D10Device* pcThis, DXGI_FORMAT Format, UINT *pFormatSupport);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CheckFormatSupport, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CheckFormatSupport");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pFormatSupport");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CheckFormatSupport);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CheckMultisampleQualityLevels        (ID3D10Device* pcThis, DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CheckMultisampleQualityLevels, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CheckMultisampleQualityLevels");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SampleCount");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNumQualityLevels");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI CheckCounterInfo                     (ID3D10Device* pcThis, D3D10_COUNTER_INFO *pCounterInfo);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CheckCounterInfo, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CheckCounterInfo");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_INFO, L"pCounterInfo");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CheckCounterInfo);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI CheckCounter                         (ID3D10Device* pcThis, const D3D10_COUNTER_DESC *pDesc, D3D10_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_CheckCounter, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::CheckCounter");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_TYPE, L"pType");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pActiveCounters");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szName");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNameLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szUnits");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pUnitsLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szDescription");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDescriptionLength");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::CheckCounter);
AQU_D3D_NODE_FOOTER___;
/**
* UINT    WINAPI GetCreationFlags                     (ID3D10Device* pcThis);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetCreationFlags, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetCreationFlags");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetCreationFlags);
AQU_D3D_NODE_FOOTER___;
/**
* HRESULT WINAPI OpenSharedResource                   (ID3D10Device* pcThis, HANDLE hResource, REFIID ReturnedInterface, void **ppResource);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_OpenSharedResource, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::OpenSharedResource");
AQU_D3D_NODE_COMMANDER(AQU_HANDLE, L"hResource");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"ReturnedInterface");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppResource");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::OpenSharedResource);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI SetTextFilterSize                    (ID3D10Device* pcThis, UINT Width, UINT Height);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_SetTextFilterSize, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::SetTextFilterSize");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Width");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Height");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::SetTextFilterSize);
AQU_D3D_NODE_FOOTER___;
/**
* void    WINAPI GetTextFilterSize                    (ID3D10Device* pcThis, UINT *pWidth, UINT *pHeight);
**/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device_GetTextFilterSize, 280, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device::GetTextFilterSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pWidth");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pHeight");
AQU_D3D_NODE_METHOD___(DirectX_10, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device, VMT_ID3D10DEVICE::GetTextFilterSize);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D10Device node provider method

/**
* Node provider class.
***/
class NOD_ID3D10Device
{
public:
	/**
	* Provides ID3D10Device node by index.
	***/
	NOD_Basic* Get_ID3D10Device_Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_ID3D10DEVICE::VMT_ID3D10Device::GetTextFilterSize) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_ID3D10Device_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_ID3D10Device_Node((VMT_ID3D10DEVICE::VMT_ID3D10Device)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_ID3D10Device_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::QueryInterface:
			return (NOD_Basic*)new NOD_ID3D10Device_QueryInterface(nX, nY);
			break;
		case VMT_IUNKNOWN::AddRef:
			return (NOD_Basic*)new NOD_ID3D10Device_AddRef(nX, nY);
			break;
		case VMT_IUNKNOWN::Release:
			return (NOD_Basic*)new NOD_ID3D10Device_Release(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides ID3D10Device node by enumeration.
	***/
	NOD_Basic* Get_ID3D10Device_Node(VMT_ID3D10DEVICE::VMT_ID3D10Device eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_ID3D10DEVICE::VSSetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_VSSetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSSetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_PSSetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSSetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_PSSetShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSSetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_PSSetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSSetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_VSSetShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::DrawIndexed:
			return (NOD_Basic*)new NOD_ID3D10Device_DrawIndexed(nX, nY);
			break;
		case VMT_ID3D10DEVICE::Draw:
			return (NOD_Basic*)new NOD_ID3D10Device_Draw(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSSetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_PSSetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IASetInputLayout:
			return (NOD_Basic*)new NOD_ID3D10Device_IASetInputLayout(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IASetVertexBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_IASetVertexBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IASetIndexBuffer:
			return (NOD_Basic*)new NOD_ID3D10Device_IASetIndexBuffer(nX, nY);
			break;
		case VMT_ID3D10DEVICE::DrawIndexedInstanced:
			return (NOD_Basic*)new NOD_ID3D10Device_DrawIndexedInstanced(nX, nY);
			break;
		case VMT_ID3D10DEVICE::DrawInstanced:
			return (NOD_Basic*)new NOD_ID3D10Device_DrawInstanced(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSSetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_GSSetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSSetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_GSSetShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IASetPrimitiveTopology:
			return (NOD_Basic*)new NOD_ID3D10Device_IASetPrimitiveTopology(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSSetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_VSSetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSSetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_VSSetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SetPredication:
			return (NOD_Basic*)new NOD_ID3D10Device_SetPredication(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSSetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_GSSetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSSetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_GSSetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMSetRenderTargets:
			return (NOD_Basic*)new NOD_ID3D10Device_OMSetRenderTargets(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMSetBlendState:
			return (NOD_Basic*)new NOD_ID3D10Device_OMSetBlendState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMSetDepthStencilState:
			return (NOD_Basic*)new NOD_ID3D10Device_OMSetDepthStencilState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SOSetTargets:
			return (NOD_Basic*)new NOD_ID3D10Device_SOSetTargets(nX, nY);
			break;
		case VMT_ID3D10DEVICE::DrawAuto:
			return (NOD_Basic*)new NOD_ID3D10Device_DrawAuto(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSSetState:
			return (NOD_Basic*)new NOD_ID3D10Device_RSSetState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSSetViewports:
			return (NOD_Basic*)new NOD_ID3D10Device_RSSetViewports(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSSetScissorRects:
			return (NOD_Basic*)new NOD_ID3D10Device_RSSetScissorRects(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CopySubresourceRegion:
			return (NOD_Basic*)new NOD_ID3D10Device_CopySubresourceRegion(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CopyResource:
			return (NOD_Basic*)new NOD_ID3D10Device_CopyResource(nX, nY);
			break;
		case VMT_ID3D10DEVICE::UpdateSubresource:
			return (NOD_Basic*)new NOD_ID3D10Device_UpdateSubresource(nX, nY);
			break;
		case VMT_ID3D10DEVICE::ClearRenderTargetView:
			return (NOD_Basic*)new NOD_ID3D10Device_ClearRenderTargetView(nX, nY);
			break;
		case VMT_ID3D10DEVICE::ClearDepthStencilView:
			return (NOD_Basic*)new NOD_ID3D10Device_ClearDepthStencilView(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GenerateMips:
			return (NOD_Basic*)new NOD_ID3D10Device_GenerateMips(nX, nY);
			break;
		case VMT_ID3D10DEVICE::ResolveSubresource:
			return (NOD_Basic*)new NOD_ID3D10Device_ResolveSubresource(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSGetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_VSGetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSGetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_PSGetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSGetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_PSGetShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSGetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_PSGetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSGetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_PSGetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::PSGetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_PSGetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IAGetInputLayout:
			return (NOD_Basic*)new NOD_ID3D10Device_IAGetInputLayout(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IAGetVertexBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_IAGetVertexBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IAGetIndexBuffer:
			return (NOD_Basic*)new NOD_ID3D10Device_IAGetIndexBuffer(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSGetConstantBuffers:
			return (NOD_Basic*)new NOD_ID3D10Device_GSGetConstantBuffers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSGetShader:
			return (NOD_Basic*)new NOD_ID3D10Device_GSGetShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::IAGetPrimitiveTopology:
			return (NOD_Basic*)new NOD_ID3D10Device_IAGetPrimitiveTopology(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSGetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_VSGetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::VSGetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_VSGetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetPredication:
			return (NOD_Basic*)new NOD_ID3D10Device_GetPredication(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSGetShaderResources:
			return (NOD_Basic*)new NOD_ID3D10Device_GSGetShaderResources(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GSGetSamplers:
			return (NOD_Basic*)new NOD_ID3D10Device_GSGetSamplers(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMGetRenderTargets:
			return (NOD_Basic*)new NOD_ID3D10Device_OMGetRenderTargets(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMGetBlendState:
			return (NOD_Basic*)new NOD_ID3D10Device_OMGetBlendState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OMGetDepthStencilState:
			return (NOD_Basic*)new NOD_ID3D10Device_OMGetDepthStencilState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SOGetTargets:
			return (NOD_Basic*)new NOD_ID3D10Device_SOGetTargets(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSGetState:
			return (NOD_Basic*)new NOD_ID3D10Device_RSGetState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSGetViewports:
			return (NOD_Basic*)new NOD_ID3D10Device_RSGetViewports(nX, nY);
			break;
		case VMT_ID3D10DEVICE::RSGetScissorRects:
			return (NOD_Basic*)new NOD_ID3D10Device_RSGetScissorRects(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetDeviceRemovedReason:
			return (NOD_Basic*)new NOD_ID3D10Device_GetDeviceRemovedReason(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SetExceptionMode:
			return (NOD_Basic*)new NOD_ID3D10Device_SetExceptionMode(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetExceptionMode:
			return (NOD_Basic*)new NOD_ID3D10Device_GetExceptionMode(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetPrivateData:
			return (NOD_Basic*)new NOD_ID3D10Device_GetPrivateData(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SetPrivateData:
			return (NOD_Basic*)new NOD_ID3D10Device_SetPrivateData(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SetPrivateDataInterface:
			return (NOD_Basic*)new NOD_ID3D10Device_SetPrivateDataInterface(nX, nY);
			break;
		case VMT_ID3D10DEVICE::ClearState:
			return (NOD_Basic*)new NOD_ID3D10Device_ClearState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::Flush:
			return (NOD_Basic*)new NOD_ID3D10Device_Flush(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateBuffer:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateBuffer(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateTexture1D:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateTexture1D(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateTexture2D:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateTexture2D(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateTexture3D:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateTexture3D(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateShaderResourceView:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateShaderResourceView(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateRenderTargetView:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateRenderTargetView(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateDepthStencilView:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateDepthStencilView(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateInputLayout:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateInputLayout(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateVertexShader:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateVertexShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateGeometryShader:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateGeometryShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateGeometryShaderWithStreamOutput(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreatePixelShader:
			return (NOD_Basic*)new NOD_ID3D10Device_CreatePixelShader(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateBlendState:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateBlendState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateDepthStencilState:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateDepthStencilState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateRasterizerState:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateRasterizerState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateSamplerState:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateSamplerState(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateQuery:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateQuery(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreatePredicate:
			return (NOD_Basic*)new NOD_ID3D10Device_CreatePredicate(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CreateCounter:
			return (NOD_Basic*)new NOD_ID3D10Device_CreateCounter(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CheckFormatSupport:
			return (NOD_Basic*)new NOD_ID3D10Device_CheckFormatSupport(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CheckMultisampleQualityLevels:
			return (NOD_Basic*)new NOD_ID3D10Device_CheckMultisampleQualityLevels(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CheckCounterInfo:
			return (NOD_Basic*)new NOD_ID3D10Device_CheckCounterInfo(nX, nY);
			break;
		case VMT_ID3D10DEVICE::CheckCounter:
			return (NOD_Basic*)new NOD_ID3D10Device_CheckCounter(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetCreationFlags:
			return (NOD_Basic*)new NOD_ID3D10Device_GetCreationFlags(nX, nY);
			break;
		case VMT_ID3D10DEVICE::OpenSharedResource:
			return (NOD_Basic*)new NOD_ID3D10Device_OpenSharedResource(nX, nY);
			break;
		case VMT_ID3D10DEVICE::SetTextFilterSize:
			return (NOD_Basic*)new NOD_ID3D10Device_SetTextFilterSize(nX, nY);
			break;
		case VMT_ID3D10DEVICE::GetTextFilterSize:
			return (NOD_Basic*)new NOD_ID3D10Device_GetTextFilterSize(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	};
};

#pragma endregion

#endif