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
#ifndef NOD_HELLOWORLDDX9
#define NOD_HELLOWORLDDX9

#include"NOD_Basic.h"
#include"AQU_GlobalTypes.h"
#include<d3dx9.h>
#include<d3d9.h>

/**
* Simple texture vertex structure.
***/
struct TexturedVertex
{
	float x;
	float y;
	float z;
	float w;
	//float rhw;
	float u;
	float v;
};

/**
* Simple 2D Output vertex shader.
***/
static const char* VS2DRender =
	"struct VS_IN\n"  
	"{\n"  
	"float4 Position  : POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"struct VS_OUT\n"  
	"{\n"  
	"float4 Position  : POSITION;\n"  
	"float2 TexCoord : TEXCOORD0;\n"  
	"};\n"

	"float2 Pos = {0.0,0.0};\n"

	"VS_OUT VS2D(VS_IN vtx)\n" 
	"{\n" 
	"VS_OUT Out;\n"  
	"Out.Position = float4(vtx.Position.xy, 0, 1);\n"
	"Out.TexCoord = vtx.TexCoord;\n"
	"Out.Position.x = Out.Position.x + Pos.x;\n"
	"Out.Position.y = Out.Position.y + Pos.y;\n"
	"return Out;\n" 

	"} \n";


/**
* Simple 2D Output pixel shader.
***/
static const char* PS2DRender =
	"// Simple 2D Output pixel shader\n"

	"sampler2D TexMap0;\n"
	"float4 Col = {1.0,1.0,1.0,1.0};\n"

	"float4 PS2D(float2 Tex : TEXCOORD0) : COLOR\n"
	"{\n"
	"float4 tColor;\n"
	"tColor = tex2D(TexMap0, Tex);\n"
	"tColor.w = (tColor.x+tColor.y+tColor.z)/3.0;\n"
	"tColor.w = saturate(tColor.w*Col.w);\n"
	"tColor.x = saturate(tColor.x*Col.x);\n"
	"tColor.y = saturate(tColor.y*Col.y);\n"
	"tColor.z = saturate(tColor.z*Col.z);\n"
	"return tColor;\n"
	"}\n"
	;


/**
* Text Output Node.
* Elemantary Node.
***/
class NOD_HelloWorldDx9 : public NOD_Basic 
{ 
public: 
	NOD_HelloWorldDx9(LONG nX, LONG nY);
	// TODO !! DESTRUCTOR !!

	/*** NOD_Basic public methods ***/
	virtual bool        SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual void*       Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual const char* GetNodeType() { return typeid(this).name(); } 
	virtual UINT        GetNodeTypeId() { return ELEMENTARY_NODE_HELLO_WORLD_DX9; }

private:
	/*** NOD_HelloWorldDx9 private methods ***/
	void          Render(LPDIRECT3DDEVICE9 pDevice);
	void          CreateVertexBuffer(LPDIRECT3DDEVICE9 pDevice);
	void          CreateFontTexture(LPDIRECT3DDEVICE9 pDevice);
	DWORD         GetGlyphIndex(char character);
	HRESULT       RenderGlyph(LPDIRECT3DDEVICE9 pDevice, DWORD dwIndex, float fX, float fY, float fScale);
	HRESULT       RenderText(LPDIRECT3DDEVICE9 pDevice, LPCWSTR szText, float fX, float fY, float fScale);

	/**
	* The font texture;
	**/
	LPDIRECT3DTEXTURE9 m_pcFontTexture;
	/**
	* The font image rectangle;
	* Contains width and height (in pixels) of the font texture.
	***/
	RECT m_rcFont;
	/**
	* Font vertex shader.
	***/
	LPDIRECT3DVERTEXSHADER9	m_vsFont;
	/**
	* Font pixel shader.
	***/
	LPDIRECT3DPIXELSHADER9	m_psFont;
	/**
	* Font pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctPFont;
	/**
	* Font pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctVFont;
	/**
	* Font glyph vertex buffer.
	***/
	IDirect3DVertexBuffer9* m_vbFontGlyph;
	/**
	* D3D viewport.
	***/
	D3DVIEWPORT9 m_sViewport;
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
	* Input raw.
	***/
	void* m_apInputRaw[4];
};

#endif