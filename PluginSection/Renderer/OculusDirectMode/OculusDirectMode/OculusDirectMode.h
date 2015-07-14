/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift Direct Mode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.h> and
Class <OculusDirectMode> :
Copyright (C) 2015 Denis Reischl

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

#include"AQU_Nodus.h"
#include"Resources.h"
#include<sstream>

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "Extras/OVR_Math.h"
#define OVR_D3D_VERSION 11
#include"OVR_CAPI_D3D.h"

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_VECTOR3F_PLUG_TYPE                      2061
#define PNT_VECTOR4F_PLUG_TYPE                      2063
#define PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE        3041
#define PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE       3049
#define PPNT_IDIRECT3DVERTEXDECLARATION9_PLUG_TYPE  3050

#define NUMBER_OF_DECOMMANDERS                         2

/**
* Node Commander Enumeration.
***/
enum ORN_Decommanders
{
	LeftTexture,
	RightTexture,
};

/**
* Simple texture vertex structure.
***/
struct TexturedVertex
{
	D3DXVECTOR4 sPos;
	D3DXVECTOR2 sTex;
};

/**
* 2D Vertex Shader DX10+.
***/
static const char* VS2D =
	"struct VS_IN\n"  
	"{\n"  
	"float4 Position  : POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"struct VS_OUT\n"  
	"{\n"  
	"float4 Position  : SV_POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"VS_OUT VS( VS_IN vtx )\n"
	"{\n"
	"    VS_OUT Out = (VS_OUT)0;\n"
	"    Out.Position = vtx.Position;\n"
	"    Out.TexCoord = vtx.TexCoord;\n"
	"    return Out;\n"
	"}\n";

/**
* 2D Pixel Shader DX10+.
***/
static const char* PS2D = 
	"Texture2D fontTexture : register(t0);\n"
	"SamplerState fontSampler : register(s0);\n"

	"struct VS_OUT\n"  
	"{\n"  
	"float4 Position  : SV_POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"float4 PS( VS_OUT vtx ) : SV_Target\n"
	"{\n"
	//"    return fontTexture.Sample( fontSampler, vtx.TexCoord );\n"
	"    return float4(fontTexture.Sample( fontSampler, vtx.TexCoord ).xyz, 1.0);\n"
	//"    return float4(1.0, 0.4, 0.3, 1.0);\n"
	"}\n";

/**
* Simple depth buffer structure.
* Taken from the OculusRoomTiny demo for simplicity.
* (libOVR 0.6.1)
***/
struct DepthBuffer
{
	ID3D11DepthStencilView * TexDsv;

	DepthBuffer(ID3D11Device * Device, OVR::Sizei size, int sampleCount = 1)
	{
		DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;
		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = size.w;
		dsDesc.Height = size.h;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = format;
		dsDesc.SampleDesc.Count = sampleCount;
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;
		dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ID3D11Texture2D * Tex;
		Device->CreateTexture2D(&dsDesc, NULL, &Tex);
		Device->CreateDepthStencilView(Tex, NULL, &TexDsv);
	}
};

/**
* Simple data buffer structure.
* Taken from the OculusRoomTiny demo for simplicity.
* (libOVR 0.6.1)
***/
struct DataBuffer
{
	ID3D11Buffer * D3DBuffer;
	size_t         Size;

	DataBuffer(ID3D11Device * Device, D3D11_BIND_FLAG use, const void* buffer, size_t size) : Size(size)
	{
		D3D11_BUFFER_DESC desc;   memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = use;
		desc.ByteWidth = (unsigned)size;
		D3D11_SUBRESOURCE_DATA sr;
		sr.pSysMem = buffer;
		sr.SysMemPitch = sr.SysMemSlicePitch = 0;
		Device->CreateBuffer(&desc, buffer ? &sr : NULL, &D3DBuffer);
	}
};

/**
* ovrSwapTextureSet wrapper class that also maintains the render target views
* needed for D3D11 rendering.
* Taken from the OculusRoomTiny demo for simplicity.
* (libOVR 0.6.1)
***/
struct OculusTexture
{
	ovrSwapTextureSet      * TextureSet;
	ID3D11RenderTargetView * TexRtv[3];

	OculusTexture(ID3D11Device* pcDevice, ovrHmd hmd, OVR::Sizei size)
	{
		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width            = size.w;
		dsDesc.Height           = size.h;
		dsDesc.MipLevels        = 1;
		dsDesc.ArraySize        = 1;
		dsDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
		dsDesc.SampleDesc.Count = 1;   // No multi-sampling allowed
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage            = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags   = 0;
		dsDesc.MiscFlags        = 0;
		dsDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		ovrResult nOvrResult = ovrHmd_CreateSwapTextureSetD3D11(hmd, pcDevice, &dsDesc, &TextureSet);

		if (OVR_SUCCESS(nOvrResult))
		{
			for (int i = 0; i < TextureSet->TextureCount; ++i)
			{
				ovrD3D11Texture* tex = (ovrD3D11Texture*)&TextureSet->Textures[i];
				pcDevice->CreateRenderTargetView(tex->D3D11.pTexture, NULL, &TexRtv[i]);
			}
		}
	}

	void AdvanceToNextTexture()
	{
		TextureSet->CurrentIndex = (TextureSet->CurrentIndex + 1) % TextureSet->TextureCount;
	}
	void Release(ovrHmd hmd)
	{
		ovrHmd_DestroySwapTextureSet(hmd, TextureSet);
	}
};


/**
* Oculus DirectMode Node Plugin (Direct3D 9).
***/
class OculusDirectMode : public AQU_Nodus
{
public:
	OculusDirectMode();
	virtual ~OculusDirectMode();

	/*** OculusDirectMode public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/**
	* True if OVR is initialized.
	***/
	bool m_bInit;
	/**
	* The handle of the headset.
	***/
	ovrHmd m_hHMD;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	***/
	ovrTexture* m_pcMirrorTexture;
	/**
	* The Oculus swapchain. (for both eyes)
	***/
	OculusTexture* m_psEyeRenderTexture[2];
	/**
	* The Oculus depth buffer. (for both eyes)
	***/
	DepthBuffer* m_psEyeDepthBuffer[2];
	/**
	* The Oculus render viewport. (for both eyes)
	***/
	ovrRecti m_psEyeRenderViewport[2];
	/**
	* The Oculus render description. (for both eyes)
	***/
	ovrEyeRenderDesc m_psEyeRenderDesc[2];
	/**
	* Temporary directx 11 device for the oculus sdk.
	***/
	ID3D11Device* m_pcDeviceTemporary;
	/**
	* Temporary directx 11 device context for the oculus sdk.
	***/
	ID3D11DeviceContext* m_pcContextTemporary;
	/**
	* In-game back buffer.
	***/
	ID3D11Texture2D* m_pcBackBuffer;
	/**
	* Back buffer copy.
	***/
	ID3D11Texture2D* m_pcBackBufferCopy;
	/**
	* The direct mode vertex shader.
	* Simple 2D vertex shader.
	***/
	ID3D11VertexShader* m_pcVertexShaderDirect;
	/**
	* The direct mode pixel shader.
	* Simple Texture pixel shader.
	***/
	ID3D11PixelShader* m_pcPixelShaderDirect;
	/**
	* The direct mode vertex layout.
	***/
	ID3D11InputLayout* m_pcVertexLayoutDirect;
	/**
	* The direct mode vertex buffer.
	* Simple full-screen vertex buffer, containing 6 vertices.
	***/
	ID3D11Buffer* m_pcVertexBufferDirect;
	/**
	* The shared full-screen texture.
	***/
	ID3D11Texture2D* m_pcTextureDirect;
	/**
	* The shared full-screen texture view.
	***/
	ID3D11ShaderResourceView* m_pcTextureViewDirect;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OculusDirectMode* pOculusDirectMode = new OculusDirectMode();
	return static_cast<AQU_Nodus*>(pOculusDirectMode);
}