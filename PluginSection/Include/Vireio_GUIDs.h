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

/*** Vireio Matrix Modifier GUIDs ***/

// PDID_ID3D11Buffer_Vireio_Data : 9661ee48-d331-478d-aa3e-5663ec612473
const GUID PDID_ID3D11Buffer_Vireio_Data = { 0x9661ee48, 0xd331, 0x478d, { 0xaa, 0x3e, 0x56, 0x63, 0xec, 0x61, 0x24, 0x73 } };

// PDID_ID3D11Buffer_Vireio_Label : ecd95f38-0bfc-48a0-b752-0c9319ae7779
const GUID PDID_ID3D11Buffer_Vireio_Label = { 0xecd95f38, 0x0bfc, 0x48a0, { 0xb7, 0x52, 0x0c, 0x93, 0x19, 0xae, 0x77, 0x79 } };

// PDID_ID3D11Buffer_Vireio_Rules_Data : 16eb4347-0759-499f-9267-3601c5874a40
const GUID PDID_ID3D11Buffer_Vireio_Rules_Data = { 0x16eb4347, 0x0759, 0x499f, { 0x92, 0x67, 0x36, 0x01, 0xc5, 0x87, 0x4a, 0x40 } };

// PDID_ID3D11TextureXD_ShaderResouceView_Create_New : 7371d160-5b35-471c-b2ae-e5e9d942b776
const GUID PDID_ID3D11TextureXD_ShaderResouceView_Create_New = { 0x7371d160, 0x5b35, 0x471c, { 0xb2, 0xae, 0xe5, 0xe9, 0xd9, 0x42, 0xb7, 0x76 } };

// PDIID_ID3D11Buffer_Constant_Buffer_Right : 8c5b3ad0-d6fa-4ef2-b6ff-218ecde4072b
const GUID PDIID_ID3D11Buffer_Constant_Buffer_Right = { 0x8c5b3ad0, 0xd6fa, 0x4ef2, { 0xb6, 0xff, 0x21, 0x8e, 0xcd, 0xe4, 0x07, 0x2b } };

// PDIID_ID3D11Buffer_UAV_Right : 46b18cc4-43a9-4047-9887-034183bc5876
const GUID PDIID_ID3D11Buffer_UAV_Right = { 0x46b18cc4, 0x43a9, 0x4047, { 0x98, 0x87, 0x03, 0x41, 0x83, 0xbc, 0x58, 0x76 } };

// PDIID_ID3D11Buffer_ShaderResourceView_Right : eb76566b-568d-4271-b7ad-8223e70f23c5
const GUID PDIID_ID3D11Buffer_ShaderResourceView_Right = { 0xeb76566b, 0x568d, 0x4271, { 0xb7, 0xad, 0x82, 0x23, 0xe7, 0x0f, 0x23, 0xc5 } };

// PDID_ID3D11VertexShader_Vireio_Data : 3bffbfc5-7baa-4534-9f4a-06be7d3df832
const GUID PDID_ID3D11VertexShader_Vireio_Data = { 0x3bffbfc5, 0x7baa, 0x4534, { 0x9f, 0x4a, 0x06, 0xbe, 0x7d, 0x3d, 0xf8, 0x32 } };

/*** Vireio Stereo Splitter GUIDs ***/

// PDIID_IDirect3DSurface9_Stereo_Twin : 06cd4137-8d81-44d7-91ea-0c465ff447a1
const GUID PDIID_IDirect3DSurface9_Stereo_Twin = { 0x06cd4137, 0x8d81, 0x44d7, { 0x91, 0xea, 0x0c, 0x46, 0x5f, 0xf4, 0x47, 0xa1 } };

// PDIID_IDirect3DBaseTexture9_Stereo_Twin : 577f3f5e-7ed2-4341-828b-c9fa38907549
const GUID PDIID_IDirect3DBaseTexture9_Stereo_Twin = { 0x577f3f5e, 0x7ed2, 0x4341, { 0x82, 0x8b, 0xc9, 0xfa, 0x38, 0x90, 0x75, 0x49 } };

// PDIID_ID3D11TextureXD_Stereo_Twin : 65fbac52-33fe-4ece-8756-6a11675c42dc
const GUID PDIID_ID3D11TextureXD_Stereo_Twin = { 0x65fbac52, 0x33fe, 0x4ece, { 0x87, 0x56, 0x6a, 0x11, 0x67, 0x5c, 0x42, 0xdc } };

// PDIID_ID3D11TextureXD_RenderTargetView : c1e52c2b-d618-41d8-bb42-9bb371b1b28a
const GUID PDIID_ID3D11TextureXD_RenderTargetView = { 0xc1e52c2b, 0xd618, 0x41d8, { 0xbb, 0x42, 0x9b, 0xb3, 0x71, 0xb1, 0xb2, 0x8a } };

// PDIID_ID3D11TextureXD_DepthStencilView : d5e494b4-6b30-4214-baaf-1b87e75cd213
const GUID PDIID_ID3D11TextureXD_DepthStencilView = { 0xd5e494b4, 0x6b30, 0x4214, { 0xba, 0xaf, 0x1b, 0x87, 0xe7, 0x5c, 0xd2, 0x13 } };

// PDIID_ID3D11TextureXD_ShaderResourceView : 54785d84-c099-4cb7-8006-0f7ad3921f8b
const GUID PDIID_ID3D11TextureXD_ShaderResourceView = { 0x54785d84, 0xc099, 0x4cb7, { 0x80, 0x06, 0x0f, 0x7a, 0xd3, 0x92, 0x1f, 0x8b } };

// PDIID_ID3D11TextureXD_UnorderedAccessView : aa30daa-99c9-42b4-b395-cbe6a8e5e364
const GUID PDIID_ID3D11TextureXD_UnorderedAccessView = { 0xaa30daa, 0x99c9, 0x42b4, { 0xb3, 0x95, 0xcb, 0xe6, 0xa8, 0xe5, 0xe3, 0x64 } };

// PDIID_ID3D11RenderTargetView_Stereo_Twin : bc121462-d0b4-42d5-8429-2f7b194c8d20
const GUID PDIID_ID3D11RenderTargetView_Stereo_Twin = { 0xbc121462, 0xd0b4, 0x42d5, { 0x84, 0x29, 0x2f, 0x7b, 0x19, 0x4c, 0x8d, 0x20 } };

// PDIID_ID3D11DepthStencilView_Stereo_Twin : fae991b6-6c7f-474b-afe3-369861002ae7
const GUID PDIID_ID3D11DepthStencilView_Stereo_Twin = { 0xfae991b6, 0x6c7f, 0x474b, { 0xaf, 0xe3, 0x36, 0x98, 0x61, 0x00, 0x2a, 0xe7 } };

// PDIID_ID3D11UnorderedAccessView_Stereo_Twin : 1e547289-77c7-4a94-a6e6-c77e87cb86ab
const GUID PDIID_ID3D11UnorderedAccessView_Stereo_Twin = { 0x1e547289, 0x77c7, 0x4a94, { 0xa6, 0xe6, 0xc7, 0x7e, 0x87, 0xcb, 0x86, 0xab } };

// PDIID_IDXGISwapChain_TextureXD_Stereo_Twin : 4458d508-c9b4-4eec-a41e-f7be6e873179
const GUID PDIID_IDXGISwapChain_TextureXD_Stereo_Twin = { 0x4458d508, 0xc9b4, 0x4eec, { 0xa4, 0x1e, 0xf7, 0xbe, 0x6e, 0x87, 0x31, 0x79 } };

// PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin : 814ea333-0f87-485e-b264-faac42d62faf
const GUID PDIID_IDXGISwapChain_RenderTargetView_Stereo_Twin = { 0x814ea333, 0x0f87, 0x485e, { 0xb2, 0x64, 0xfa, 0xac, 0x42, 0xd6, 0x2f, 0xaf } };

// PDIID_Shared_Handle : 88522b10-cab2-4284-a2cc-d7344398cb31
const GUID PDIID_Shared_Handle = { 0x88522b10, 0xcab2, 0x4284, { 0xa2, 0xcc, 0xd7, 0x34, 0x43, 0x98, 0xcb, 0x31 } };

/*** Vireio Cinema GUIDs ***/

// PDIID_ID3D11Device_IDXGISwapChain : 90de944a-d1ee-4921-b9a4-6517319d0f43
const GUID PDIID_ID3D11Device_IDXGISwapChain = { 0x90de944a, 0xd1ee, 0x4921, { 0xb9, 0xa4, 0x65, 0x17, 0x31, 0x9d, 0x0f, 0x43 } };

#pragma endregion