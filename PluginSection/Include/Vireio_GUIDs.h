/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_GUIDs.h> :
Copyright (C) 2015 Denis Reischl

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

#include<d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#pragma region D3D11

// PDID_ID3D11Buffer_Vireio_Data : 9661ee48-d331-478d-aa3e-5663ec612473
const GUID PDID_ID3D11Buffer_Vireio_Data = { 0x9661ee48, 0xd331, 0x478d, { 0xaa, 0x3e, 0x56, 0x63, 0xec, 0x61, 0x24, 0x73 } };

// PDID_ID3D11Buffer_Vireio_Label : ecd95f38-0bfc-48a0-b752-0c9319ae7779
const GUID PDID_ID3D11Buffer_Vireio_Label = { 0xecd95f38, 0x0bfc, 0x48a0, { 0xb7, 0x52, 0x0c, 0x93, 0x19, 0xae, 0x77, 0x79 } };

// PDID_ID3D11Buffer_Vireio_Rules_Index : 16eb4347-0759-499f-9267-3601c5874a40
const GUID PDID_ID3D11Buffer_Vireio_Rules_Index = { 0x16eb4347, 0x0759, 0x499f, { 0x92, 0x67, 0x36, 0x01, 0xc5, 0x87, 0x4a, 0x40 } };

// PDID_ID3D11TextureXD_ShaderResouceView_Create_New : 7371d160-5b35-471c-b2ae-e5e9d942b776
const GUID PDID_ID3D11TextureXD_ShaderResouceView_Create_New = { 0x7371d160, 0x5b35, 0x471c, { 0xb2, 0xae, 0xe5, 0xe9, 0xd9, 0x42, 0xb7, 0x76 } };

// PDIID_ID3D11Buffer_Constant_Buffer_Left : a48d2fc5-f947-444c-9e93-470d66cb5b71
const GUID PDIID_ID3D11Buffer_Constant_Buffer_Left = { 0xa48d2fc5, 0xf947, 0x444c, { 0x9e, 0x93, 0x47, 0x0d, 0x66, 0xcb, 0x5b, 0x71 } };

// PDIID_ID3D11Buffer_Constant_Buffer_Right : 8c5b3ad0-d6fa-4ef2-b6ff-218ecde4072b
const GUID PDIID_ID3D11Buffer_Constant_Buffer_Right = { 0x8c5b3ad0, 0xd6fa, 0x4ef2, { 0xb6, 0xff, 0x21, 0x8e, 0xcd, 0xe4, 0x07, 0x2b } };

// PDID_ID3D11VertexShader_Vireio_Data : 3bffbfc5-7baa-4534-9f4a-06be7d3df832
const GUID PDID_ID3D11VertexShader_Vireio_Data = { 0x3bffbfc5, 0x7baa, 0x4534, { 0x9f, 0x4a, 0x06, 0xbe, 0x7d, 0x3d, 0xf8, 0x32 } };

// PDIID_ID3D11TextureXD_Stereo_Twin : 65fbac52-33fe-4ece-8756-6a11675c42dc
const GUID PDIID_ID3D11TextureXD_Stereo_Twin = { 0x65fbac52, 0x33fe, 0x4ece, { 0x87, 0x56, 0x6a, 0x11, 0x67, 0x5c, 0x42, 0xdc } };

// PDIID_ID3D11TextureXD_RenderTargetView : c1e52c2b-d618-41d8-bb42-9bb371b1b28a
const GUID PDIID_ID3D11TextureXD_RenderTargetView = { 0xc1e52c2b, 0xd618, 0x41d8, { 0xbb, 0x42, 0x9b, 0xb3, 0x71, 0xb1, 0xb2, 0x8a } };

// PDIID_ID3D11TextureXD_DepthStencilView : d5e494b4-6b30-4214-baaf-1b87e75cd213
const GUID PDIID_ID3D11TextureXD_DepthStencilView = { 0xd5e494b4, 0x6b30, 0x4214, { 0xba, 0xaf, 0x1b, 0x87, 0xe7, 0x5c, 0xd2, 0x13 } };

// PDIID_ID3D11TextureXD_ShaderResourceView : 54785d84-c099-4cb7-8006-0f7ad3921f8b
const GUID PDIID_ID3D11TextureXD_ShaderResourceView = { 0x54785d84, 0xc099, 0x4cb7, { 0x80, 0x06, 0x0f, 0x7a, 0xd3, 0x92, 0x1f, 0x8b } };

// PDIID_ID3D11RenderTargetView_Stereo_Twin : bc121462-d0b4-42d5-8429-2f7b194c8d20
const GUID PDIID_ID3D11RenderTargetView_Stereo_Twin = { 0xbc121462, 0xd0b4, 0x42d5, { 0x84, 0x29, 0x2f, 0x7b, 0x19, 0x4c, 0x8d, 0x20 } };

// PDIID_ID3D11DepthStencilView_Stereo_Twin : fae991b6-6c7f-474b-afe3-369861002ae7
const GUID PDIID_ID3D11DepthStencilView_Stereo_Twin = { 0xfae991b6, 0x6c7f, 0x474b, { 0xaf, 0xe3, 0x36, 0x98, 0x61, 0x00, 0x2a, 0xe7 } };

// PDIID_IDXGISwapChain_TextureXD_Stereo_Twin : 4458d508-c9b4-4eec-a41e-f7be6e873179
const GUID PDIID_IDXGISwapChain_TextureXD_Stereo_Twin = { 0x4458d508, 0xc9b4, 0x4eec, { 0xa4, 0x1e, 0xf7, 0xbe, 0x6e, 0x87, 0x31, 0x79 } };

// PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin : 814ea333-0f87-485e-b264-faac42d62faf
const GUID PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin = { 0x814ea333, 0x0f87, 0x485e, { 0xb2, 0x64, 0xfa, 0xac, 0x42, 0xd6, 0x2f, 0xaf } };

#pragma endregion