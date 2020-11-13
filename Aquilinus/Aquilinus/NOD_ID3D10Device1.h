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
#ifndef NOD_ID3D10DEVICE1
#define NOD_ID3D10DEVICE1

#include"VMT_ID3D10Device1.h"

#pragma region IUnknown methods
/**
* HRESULT WINAPI QueryInterface                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_QueryInterface                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::QueryInterface                       ");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObject");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_IUNKNOWN::QueryInterface);
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI AddRef                               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_AddRef                               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::AddRef                               ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_IUNKNOWN::AddRef);
AQU_D3D_NODE_FOOTER___;

/**
* ULONG WINAPI Release                              
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_Release                              , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::Release                              ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_IUNKNOWN::Release);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D10Device methods
/**
* void WINAPI VSSetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSSetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSSetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSSetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSSetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSSetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSSetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSSetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSSetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSSetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10PIXELSHADER, L"pPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSSetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSSetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSSetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSSetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSSetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSSetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSSetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSSetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10VERTEXSHADER, L"pVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSSetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI DrawIndexed                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_DrawIndexed                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::DrawIndexed                          ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"IndexCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartIndexLocation");
AQU_D3D_NODE_COMMANDER(AQU_INT, L"BaseVertexLocation");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::DrawIndexed);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI Draw                                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_Draw                                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::Draw                                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"VertexCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartVertexLocation");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::Draw);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSSetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSSetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSSetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IASetInputLayout                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IASetInputLayout                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IASetInputLayout                     ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10INPUTLAYOUT, L"pInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IASetInputLayout);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IASetVertexBuffers                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IASetVertexBuffers                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IASetVertexBuffers                   ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStrides");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IASetVertexBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IASetIndexBuffer                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IASetIndexBuffer                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IASetIndexBuffer                     ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10BUFFER, L"pIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Offset");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IASetIndexBuffer);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI DrawIndexedInstanced                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_DrawIndexedInstanced                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::DrawIndexedInstanced                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"IndexCountPerInstance");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"InstanceCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartIndexLocation");
AQU_D3D_NODE_COMMANDER(AQU_INT, L"BaseVertexLocation");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartInstanceLocation");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::DrawIndexedInstanced);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI DrawInstanced                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_DrawInstanced                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::DrawInstanced                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"VertexCountPerInstance");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"InstanceCount");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartVertexLocation");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartInstanceLocation");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::DrawInstanced);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSSetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSSetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSSetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSSetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSSetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSSetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSSetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10GEOMETRYSHADER, L"pShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSSetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IASetPrimitiveTopology               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IASetPrimitiveTopology               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IASetPrimitiveTopology               ");
AQU_D3D_NODE_COMMANDER(AQU_D3D10_PRIMITIVE_TOPOLOGY, L"Topology");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IASetPrimitiveTopology);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSSetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSSetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSSetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSSetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSSetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSSetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSSetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSSetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI SetPredication                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SetPredication                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SetPredication                       ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10PREDICATE, L"pPredicate");
AQU_D3D_NODE_COMMANDER(AQU_BOOL, L"PredicateValue");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SetPredication);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSSetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSSetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSSetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSSetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSSetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSSetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSSetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSSetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMSetRenderTargets                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMSetRenderTargets                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMSetRenderTargets                   ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRenderTargetViews");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILVIEW, L"pDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMSetRenderTargets);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMSetBlendState                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMSetBlendState                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMSetBlendState                      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10BLENDSTATE, L"pBlendState");
AQU_D3D_NODE_COMMANDER(AQU_FLOAT, L"BlendFactor");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SampleMask");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMSetBlendState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMSetDepthStencilState               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMSetDepthStencilState               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMSetDepthStencilState               ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILSTATE, L"pDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StencilRef");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMSetDepthStencilState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI SOSetTargets                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SOSetTargets                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SOSetTargets                         ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppSOTargets");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SOSetTargets);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI DrawAuto                             
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_DrawAuto                             , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::DrawAuto                             ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::DrawAuto);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSSetState                           
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSSetState                           , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSSetState                           ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RASTERIZERSTATE, L"pRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSSetState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSSetViewports                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSSetViewports                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSSetViewports                       ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViewports");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_VIEWPORT, L"pViewports");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSSetViewports);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSSetScissorRects                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSSetScissorRects                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSSetScissorRects                    ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumRects");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RECT, L"pRects");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSSetScissorRects);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI CopySubresourceRegion                
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CopySubresourceRegion                , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CopySubresourceRegion                ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstX");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstY");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstZ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BOX, L"pSrcBox");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CopySubresourceRegion);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI CopyResource                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CopyResource                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CopyResource                         ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CopyResource);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI UpdateSubresource                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_UpdateSubresource                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::UpdateSubresource                    ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BOX, L"pDstBox");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pSrcData");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcRowPitch");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcDepthPitch");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::UpdateSubresource);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI ClearRenderTargetView                
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_ClearRenderTargetView                , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::ClearRenderTargetView                ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RENDERTARGETVIEW, L"pRenderTargetView");
AQU_D3D_NODE_COMMANDER(AQU_PNT_FLOAT, L"ColorRGBA");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::ClearRenderTargetView);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI ClearDepthStencilView                
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_ClearDepthStencilView                , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::ClearDepthStencilView                ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10DEPTHSTENCILVIEW, L"pDepthStencilView");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"ClearFlags");
AQU_D3D_NODE_COMMANDER(AQU_FLOAT, L"Depth");
AQU_D3D_NODE_COMMANDER(AQU_BYTE, L"Stencil");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::ClearDepthStencilView);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GenerateMips                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GenerateMips                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GenerateMips                         ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10SHADERRESOURCEVIEW, L"pShaderResourceView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GenerateMips);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI ResolveSubresource                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_ResolveSubresource                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::ResolveSubresource                   ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pDstResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DstSubresource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pSrcResource");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SrcSubresource");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::ResolveSubresource);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSGetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSGetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSGetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSGetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSGetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSGetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSGetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSGetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSGetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSGetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PIXELSHADER, L"ppPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSGetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSGetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSGetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSGetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSGetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSGetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSGetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSGetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10VERTEXSHADER, L"ppVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSGetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI PSGetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_PSGetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::PSGetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::PSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IAGetInputLayout                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IAGetInputLayout                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IAGetInputLayout                     ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10INPUTLAYOUT, L"ppInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IAGetInputLayout);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IAGetVertexBuffers                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IAGetVertexBuffers                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IAGetVertexBuffers                   ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppVertexBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStrides");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IAGetVertexBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IAGetIndexBuffer                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IAGetIndexBuffer                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IAGetIndexBuffer                     ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"pIndexBuffer");
AQU_D3D_NODE_COMMANDER(AQU_PNT_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"Offset");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IAGetIndexBuffer);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSGetConstantBuffers                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSGetConstantBuffers                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSGetConstantBuffers                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppConstantBuffers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSGetConstantBuffers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSGetShader                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSGetShader                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSGetShader                          ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSGetShader);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI IAGetPrimitiveTopology               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_IAGetPrimitiveTopology               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::IAGetPrimitiveTopology               ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_PRIMITIVE_TOPOLOGY, L"pTopology");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::IAGetPrimitiveTopology);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSGetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSGetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSGetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSGetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI VSGetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_VSGetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::VSGetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::VSGetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GetPredication                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetPredication                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetPredication                       ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PREDICATE, L"ppPredicate");
AQU_D3D_NODE_COMMANDER(AQU_PNT_BOOL, L"pPredicateValue");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetPredication);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSGetShaderResources                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSGetShaderResources                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSGetShaderResources                 ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppShaderResourceViews");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSGetShaderResources);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GSGetSamplers                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GSGetSamplers                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GSGetSamplers                        ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"StartSlot");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumSamplers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplers");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GSGetSamplers);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMGetRenderTargets                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMGetRenderTargets                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMGetRenderTargets                   ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRenderTargetViews");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILVIEW, L"ppDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMGetRenderTargets);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMGetBlendState                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMGetBlendState                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMGetBlendState                      ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BLENDSTATE, L"ppBlendState");
AQU_D3D_NODE_COMMANDER(AQU_FLOAT, L"BlendFactor");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pSampleMask");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMGetBlendState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI OMGetDepthStencilState               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OMGetDepthStencilState               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OMGetDepthStencilState               ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILSTATE, L"ppDepthStencilState");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pStencilRef");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OMGetDepthStencilState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI SOGetTargets                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SOGetTargets                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SOGetTargets                         ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumBuffers");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppSOTargets");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pOffsets");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SOGetTargets);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSGetState                           
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSGetState                           , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSGetState                           ");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RASTERIZERSTATE, L"ppRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSGetState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSGetViewports                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSGetViewports                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSGetViewports                       ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"NumViewports");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_VIEWPORT, L"pViewports");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSGetViewports);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI RSGetScissorRects                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_RSGetScissorRects                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::RSGetScissorRects                    ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"NumRects");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RECT, L"pRects");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::RSGetScissorRects);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetDeviceRemovedReason               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetDeviceRemovedReason               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetDeviceRemovedReason               ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetDeviceRemovedReason);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetExceptionMode                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SetExceptionMode                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SetExceptionMode                     ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"RaiseFlags");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SetExceptionMode);
AQU_D3D_NODE_FOOTER___;

/**
* UINT WINAPI GetExceptionMode                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetExceptionMode                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetExceptionMode                     ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetExceptionMode);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI GetPrivateData                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetPrivateData                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetPrivateData                       ");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetPrivateData                       
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SetPrivateData                       , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SetPrivateData                       ");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"DataSize");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SetPrivateData);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI SetPrivateDataInterface              
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SetPrivateDataInterface              , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SetPrivateDataInterface              ");
AQU_D3D_NODE_COMMANDER(AQU_REFGUID, L"guid");
AQU_D3D_NODE_COMMANDER(AQU_PNT_IUNKNOWN, L"pData");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SetPrivateDataInterface);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI ClearState                           
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_ClearState                           , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::ClearState                           ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::ClearState);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI Flush                                
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_Flush                                , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::Flush                                ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::Flush);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateBuffer                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateBuffer                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateBuffer                         ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BUFFER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BUFFER, L"ppBuffer");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateBuffer);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture1D                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateTexture1D                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateTexture1D                      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE1D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE1D, L"ppTexture1D");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateTexture1D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture2D                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateTexture2D                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateTexture2D                      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE2D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE2D, L"ppTexture2D");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateTexture2D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateTexture3D                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateTexture3D                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateTexture3D                      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_TEXTURE3D_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SUBRESOURCE_DATA, L"pInitialData");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10TEXTURE3D, L"ppTexture3D");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateTexture3D);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateShaderResourceView             
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateShaderResourceView             , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateShaderResourceView             ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SHADER_RESOURCE_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW, L"ppSRView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateShaderResourceView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateRenderTargetView               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateRenderTargetView               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateRenderTargetView               ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RENDER_TARGET_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RENDERTARGETVIEW, L"ppRTView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateRenderTargetView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDepthStencilView               
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateDepthStencilView               , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateDepthStencilView               ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_DEPTH_STENCIL_VIEW_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILVIEW, L"ppDepthStencilView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateDepthStencilView);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateInputLayout                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateInputLayout                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateInputLayout                    ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_INPUT_ELEMENT_DESC, L"pInputElementDescs");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumElements");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecodeWithInputSignature");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10INPUTLAYOUT, L"ppInputLayout");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateInputLayout);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateVertexShader                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateVertexShader                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateVertexShader                   ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10VERTEXSHADER, L"ppVertexShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateVertexShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateGeometryShader                 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateGeometryShader                 , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateGeometryShader                 ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateGeometryShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateGeometryShaderWithStreamOutput 
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateGeometryShaderWithStreamOutput , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateGeometryShaderWithStreamOutput ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SO_DECLARATION_ENTRY, L"pSODeclaration");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"NumEntries");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"OutputStreamStride");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10GEOMETRYSHADER, L"ppGeometryShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateGeometryShaderWithStreamOutput);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreatePixelShader                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreatePixelShader                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreatePixelShader                    ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_VOID, L"pShaderBytecode");
AQU_D3D_NODE_COMMANDER(AQU_SIZE_T, L"BytecodeLength");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PIXELSHADER, L"ppPixelShader");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreatePixelShader);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateBlendState                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateBlendState                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateBlendState                     ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BLEND_DESC, L"pBlendStateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BLENDSTATE, L"ppBlendState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateBlendState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateDepthStencilState              
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateDepthStencilState              , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateDepthStencilState              ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_DEPTH_STENCIL_DESC, L"pDepthStencilDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10DEPTHSTENCILSTATE, L"ppDepthStencilState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateDepthStencilState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateRasterizerState                
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateRasterizerState                , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateRasterizerState                ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_RASTERIZER_DESC, L"pRasterizerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10RASTERIZERSTATE, L"ppRasterizerState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateRasterizerState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateSamplerState                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateSamplerState                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateSamplerState                   ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SAMPLER_DESC, L"pSamplerDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SAMPLERSTATE, L"ppSamplerState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateSamplerState);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateQuery                          
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateQuery                          , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateQuery                          ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_QUERY_DESC, L"pQueryDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10QUERY, L"ppQuery");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateQuery);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreatePredicate                      
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreatePredicate                      , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreatePredicate                      ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_QUERY_DESC, L"pPredicateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10PREDICATE, L"ppPredicate");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreatePredicate);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateCounter                        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateCounter                        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateCounter                        ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_DESC, L"pCounterDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10COUNTER, L"ppCounter");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CreateCounter);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckFormatSupport                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CheckFormatSupport                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CheckFormatSupport                   ");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pFormatSupport");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CheckFormatSupport);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckMultisampleQualityLevels        
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CheckMultisampleQualityLevels        , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CheckMultisampleQualityLevels        ");
AQU_D3D_NODE_COMMANDER(AQU_DXGI_FORMAT, L"Format");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"SampleCount");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNumQualityLevels");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CheckMultisampleQualityLevels);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI CheckCounterInfo                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CheckCounterInfo                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CheckCounterInfo                     ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_INFO, L"pCounterInfo");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CheckCounterInfo);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CheckCounter                         
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CheckCounter                         , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CheckCounter                         ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_DESC, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_COUNTER_TYPE, L"pType");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pActiveCounters");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szName");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pNameLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szUnits");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pUnitsLength");
AQU_D3D_NODE_COMMANDER(AQU_PNT_CHAR, L"szDescription");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pDescriptionLength");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::CheckCounter);
AQU_D3D_NODE_FOOTER___;

/**
* UINT WINAPI GetCreationFlags                     
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetCreationFlags                     , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetCreationFlags                     ");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetCreationFlags);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI OpenSharedResource                   
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_OpenSharedResource                   , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::OpenSharedResource                   ");
AQU_D3D_NODE_COMMANDER(AQU_HANDLE, L"hResource");
AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"ReturnedInterface");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppResource");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::OpenSharedResource);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI SetTextFilterSize                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_SetTextFilterSize                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::SetTextFilterSize                    ");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Width");
AQU_D3D_NODE_COMMANDER(AQU_UINT, L"Height");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::SetTextFilterSize);
AQU_D3D_NODE_FOOTER___;

/**
* void WINAPI GetTextFilterSize                    
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetTextFilterSize                    , 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetTextFilterSize                    ");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pWidth");
AQU_D3D_NODE_COMMANDER(AQU_PNT_UINT, L"pHeight");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE::GetTextFilterSize);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D10Device1 methods
/**
* HRESULT WINAPI CreateShaderResourceView1
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateShaderResourceView1, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateShaderResourceView1");
AQU_D3D_NODE_COMMANDER(AQU_PNT_ID3D10RESOURCE, L"pResource");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_SHADER_RESOURCE_VIEW_DESC1, L"pDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10SHADERRESOURCEVIEW1, L"ppSRView");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE1::CreateShaderResourceView1);
AQU_D3D_NODE_FOOTER___;

/**
* HRESULT WINAPI CreateBlendState1
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_CreateBlendState1, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::CreateBlendState1");
AQU_D3D_NODE_COMMANDER(AQU_PNT_D3D10_BLEND_DESC1, L"pBlendStateDesc");
AQU_D3D_NODE_COMMANDER(AQU_PPNT_ID3D10BLENDSTATE1, L"ppBlendState");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE1::CreateBlendState1);
AQU_D3D_NODE_FOOTER___;

/**
* D3D10_FEATURE_LEVEL1 WINAPI GetFeatureLevel
***/
AQU_D3D_NODE_HEADER___(NOD_ID3D10Device1_GetFeatureLevel, 320, 100);
AQU_D3D_NODE_TITLE____(L"ID3D10Device1::GetFeatureLevel");
AQU_D3D_NODE_METHOD___(DirectX_10_1, ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1, VMT_ID3D10DEVICE1::GetFeatureLevel);
AQU_D3D_NODE_FOOTER___;

#pragma endregion

#pragma region ID3D10Device1 node provider method

/**
* Node provider class.
***/
class NOD_ID3D10Device1
{
public:
	/**
	* Provides ID3D10Device1 node by index.
	***/
	NOD_Basic* Get_ID3D10Device1Node(UINT dwIndex, LONG nX, LONG nY)
	{
		if (dwIndex > (UINT)VMT_ID3D10DEVICE1::VMT_ID3D10Device1::GetFeatureLevel) return nullptr;
		if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_ID3D10Device1_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);
		return Get_ID3D10Device1_Node((VMT_ID3D10DEVICE1::VMT_ID3D10Device1)dwIndex, nX, nY);
	}

	/**
	* Provides IUnknown node by enumeration.
	***/
	NOD_Basic* Get_ID3D10Device1_Node(VMT_IUNKNOWN::VMT_IUnknown eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_IUNKNOWN::VMT_IUnknown::QueryInterface                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_QueryInterface                       (nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::AddRef                               :
			return (NOD_Basic*)new NOD_ID3D10Device1_AddRef                               (nX, nY);
			break;
		case VMT_IUNKNOWN::VMT_IUnknown::Release                              :
			return (NOD_Basic*)new NOD_ID3D10Device1_Release                              (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides ID3D10Device node by enumeration.
	***/
	NOD_Basic* Get_ID3D10Device1_Node(VMT_ID3D10DEVICE::VMT_ID3D10Device eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSSetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSSetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSSetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSSetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSSetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSSetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSSetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSSetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSSetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSSetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::DrawIndexed                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_DrawIndexed                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::Draw                                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_Draw                                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSSetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSSetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IASetInputLayout                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_IASetInputLayout                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IASetVertexBuffers                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_IASetVertexBuffers                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IASetIndexBuffer                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_IASetIndexBuffer                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::DrawIndexedInstanced                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_DrawIndexedInstanced                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::DrawInstanced                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_DrawInstanced                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSSetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSSetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSSetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSSetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IASetPrimitiveTopology               :
			return (NOD_Basic*)new NOD_ID3D10Device1_IASetPrimitiveTopology               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSSetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSSetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSSetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSSetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SetPredication                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_SetPredication                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSSetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSSetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSSetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSSetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMSetRenderTargets                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMSetRenderTargets                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMSetBlendState                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMSetBlendState                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMSetDepthStencilState               :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMSetDepthStencilState               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SOSetTargets                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_SOSetTargets                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::DrawAuto                             :
			return (NOD_Basic*)new NOD_ID3D10Device1_DrawAuto                             (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSSetState                           :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSSetState                           (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSSetViewports                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSSetViewports                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSSetScissorRects                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSSetScissorRects                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CopySubresourceRegion                :
			return (NOD_Basic*)new NOD_ID3D10Device1_CopySubresourceRegion                (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CopyResource                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_CopyResource                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::UpdateSubresource                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_UpdateSubresource                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::ClearRenderTargetView                :
			return (NOD_Basic*)new NOD_ID3D10Device1_ClearRenderTargetView                (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::ClearDepthStencilView                :
			return (NOD_Basic*)new NOD_ID3D10Device1_ClearDepthStencilView                (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GenerateMips                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_GenerateMips                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::ResolveSubresource                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_ResolveSubresource                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSGetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSGetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSGetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSGetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSGetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSGetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSGetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSGetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSGetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSGetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::PSGetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_PSGetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IAGetInputLayout                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_IAGetInputLayout                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IAGetVertexBuffers                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_IAGetVertexBuffers                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IAGetIndexBuffer                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_IAGetIndexBuffer                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSGetConstantBuffers                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSGetConstantBuffers                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSGetShader                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSGetShader                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::IAGetPrimitiveTopology               :
			return (NOD_Basic*)new NOD_ID3D10Device1_IAGetPrimitiveTopology               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSGetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSGetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::VSGetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_VSGetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetPredication                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetPredication                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSGetShaderResources                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSGetShaderResources                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GSGetSamplers                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_GSGetSamplers                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMGetRenderTargets                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMGetRenderTargets                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMGetBlendState                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMGetBlendState                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OMGetDepthStencilState               :
			return (NOD_Basic*)new NOD_ID3D10Device1_OMGetDepthStencilState               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SOGetTargets                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_SOGetTargets                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSGetState                           :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSGetState                           (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSGetViewports                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSGetViewports                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::RSGetScissorRects                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_RSGetScissorRects                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetDeviceRemovedReason               :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetDeviceRemovedReason               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SetExceptionMode                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_SetExceptionMode                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetExceptionMode                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetExceptionMode                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetPrivateData                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetPrivateData                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SetPrivateData                       :
			return (NOD_Basic*)new NOD_ID3D10Device1_SetPrivateData                       (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SetPrivateDataInterface              :
			return (NOD_Basic*)new NOD_ID3D10Device1_SetPrivateDataInterface              (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::ClearState                           :
			return (NOD_Basic*)new NOD_ID3D10Device1_ClearState                           (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::Flush                                :
			return (NOD_Basic*)new NOD_ID3D10Device1_Flush                                (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateBuffer                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateBuffer                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateTexture1D                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateTexture1D                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateTexture2D                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateTexture2D                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateTexture3D                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateTexture3D                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateShaderResourceView             :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateShaderResourceView             (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateRenderTargetView               :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateRenderTargetView               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateDepthStencilView               :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateDepthStencilView               (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateInputLayout                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateInputLayout                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateVertexShader                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateVertexShader                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateGeometryShader                 :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateGeometryShader                 (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateGeometryShaderWithStreamOutput :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateGeometryShaderWithStreamOutput (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreatePixelShader                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreatePixelShader                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateBlendState                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateBlendState                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateDepthStencilState              :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateDepthStencilState              (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateRasterizerState                :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateRasterizerState                (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateSamplerState                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateSamplerState                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateQuery                          :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateQuery                          (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreatePredicate                      :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreatePredicate                      (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CreateCounter                        :
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateCounter                        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CheckFormatSupport                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_CheckFormatSupport                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CheckMultisampleQualityLevels        :
			return (NOD_Basic*)new NOD_ID3D10Device1_CheckMultisampleQualityLevels        (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CheckCounterInfo                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_CheckCounterInfo                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::CheckCounter                         :
			return (NOD_Basic*)new NOD_ID3D10Device1_CheckCounter                         (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetCreationFlags                     :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetCreationFlags                     (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::OpenSharedResource                   :
			return (NOD_Basic*)new NOD_ID3D10Device1_OpenSharedResource                   (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::SetTextFilterSize                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_SetTextFilterSize                    (nX, nY);
			break;
		case VMT_ID3D10DEVICE::VMT_ID3D10Device::GetTextFilterSize                    :
			return (NOD_Basic*)new NOD_ID3D10Device1_GetTextFilterSize                    (nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides ID3D10Device1 node by enumeration.
	***/
	NOD_Basic* Get_ID3D10Device1_Node(VMT_ID3D10DEVICE1::VMT_ID3D10Device1 eMethod, LONG nX, LONG nY)
	{
		switch (eMethod)
		{
		case VMT_ID3D10DEVICE1::VMT_ID3D10Device1::CreateShaderResourceView1:
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateShaderResourceView1(nX, nY);
			break;
		case VMT_ID3D10DEVICE1::VMT_ID3D10Device1::CreateBlendState1:
			return (NOD_Basic*)new NOD_ID3D10Device1_CreateBlendState1(nX, nY);
			break;
		case VMT_ID3D10DEVICE1::VMT_ID3D10Device1::GetFeatureLevel:
			return (NOD_Basic*)new NOD_ID3D10Device1_GetFeatureLevel(nX, nY);
			break;
		default:
			break;
		}

		return nullptr;
	}

};
#pragma endregion

#endif
