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
#ifndef NOD_HELLOWORLDDX10
#define NOD_HELLOWORLDDX10

#include"NOD_Basic.h"
#include"AQU_GlobalTypes.h"

#include <d3d11.h>

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

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
* Text Output Node. (D3D 10)
* Elemantary Node.
***/
class NOD_HelloWorldDx10 : public NOD_Basic 
{ 
public: 
	NOD_HelloWorldDx10(LONG nX, LONG nY);
	~NOD_HelloWorldDx10();

	/*** NOD_Basic public methods ***/
	virtual bool        SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual void*       Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual const char* GetNodeType() { return typeid(this).name(); } 
	virtual UINT        GetNodeTypeId() { return ELEMENTARY_NODE_HELLO_WORLD_DX10; }

private:
	void                CreateStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb);
	void                ApplyStateblock(ID3D11DeviceContext* dc, D3DX11_STATE_BLOCK* sb);

	/**
	* The font vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcVertexShader;
		ID3D11VertexShader* m_pcVertexShader11;
	};
	/**
	* The font pixel shader.
	***/
	union
	{
		ID3D10PixelShader* m_pcPixelShader;
		ID3D11PixelShader* m_pcPixelShader11;
	};
	/**
	* The font vertex layout.
	***/
	union
	{
		ID3D10InputLayout* m_pcVertexLayout;
		ID3D11InputLayout* m_pcVertexLayout11;
	};
	/**
	* The font vertex buffer.
	***/
	union
	{
		ID3D10Buffer* m_pcVertexBuffer;
		ID3D11Buffer* m_pcVertexBuffer11;
	};
	/**
	* The font texture.
	***/
	union
	{
		ID3D10Texture2D* m_pcTexture;
		ID3D11Texture2D* m_pcTexture11;
	};
	/**
	* The back buffer render target view.
	***/
	ID3D11RenderTargetView* m_pcBackBufferView;
	/**
	* The font texture view.
	***/
	union
	{
		ID3D10ShaderResourceView* m_pcTextureView;
		ID3D11ShaderResourceView* m_pcTextureView11;
	};
	/**
	* The font image rectangle;
	* Contains width and height (in pixels) of the font texture.
	***/
	RECT m_rcFont;
	/**
	* The state block.
	***/
	ID3D10StateBlock* m_pcStateBlock;
	/**
	* Font texture size.
	***/
	SIZE m_sFontTextureSize;
	/**
	* Horizontal viewport range.
	***/
	float m_fViewportWidth;
	/**
	* Vertical viewport range.
	***/
	float m_fViewportHeight;
	/**
	* The X scale factor for the font glyph position calculation.
	***/
	float m_fXScale;
	/**
	* The y scale factor for the font glyph position calculation.
	***/
	float m_fYScale;
	/**
	* True if text should be underlined.
	***/
	bool m_bUnderline;
	/**
	* True if to be initialized.
	***/
	bool m_bInit;
	/**
	* Simple D3D enumeration.
	* Used to get the correct D3D device.
	***/
	enum D3DDeviceVersion
	{
		D3D10_Version,
		D3D11_Version
	} m_eDeviceVersion;
	/**
	* State block for D3D11.
	***/
	D3DX11_STATE_BLOCK m_sStateBlock11;
};

#endif