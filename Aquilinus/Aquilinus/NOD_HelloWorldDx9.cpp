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
#include"NOD_HelloWorldDx9.h"
#include"AQU_GlobalTypes.h"
#include"ITA_D3D9Interfaces.h"
#include"VMT_IDirect3DDevice9.h"
#include"VMT_IDirect3DSwapchain9.h"
#include"Resources.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

#pragma region font glyph data

/**
* Contains all data necessary for a glyph.
* SAME struct as the direct draw struct >Glyph<.
***/
struct HelloWorldGlyph
{
	char character;   /**< The glyph character. **/
	RECT rcSrc;       /**< The source rect (in pixel space). **/
	SIZE size;        /**< The size (in pixel space). **/
	LONG nAddSpacing; /**< Additional spacing. **/
};

const DWORD g_dwHelloWorldFontGlyphesNum = 93;
HelloWorldGlyph g_HelloWorldFontGlyphes[] =
{ 
	{{(char)' '},{1, 76, 0, 0},{0, 0},0},
	{{(char)'!'},{2, 9, 10, 69},{9, 67},0},
	{{(char)'"'},{13, 4, 24, 23},{3, 72},0},
	{{(char)'#'},{38, 20, 41, 34},{2, 56},0},
	{{(char)'$'},{80, 1, 36, 83},{6, 75},0},
	{{(char)'%'},{117, 9, 68, 69},{4, 67},0},
	{{(char)'&'},{186, 9, 72, 69},{4, 67},0},
	{{(char)0x27},{259, 4, 11, 23},{3, 72},0}, // get ascii for > ' <
	{{(char)'('},{271, 7, 20, 75},{5, 69},0},
	{{(char)')'},{292, 7, 20, 75},{6, 69},0},
	{{(char)'*'},{313, 6, 26, 26},{2, 70},0},
	{{(char)'+'},{340, 31, 28, 24},{3, 45},0},
	{{(char)','},{369, 68, 10, 22},{3, 8},0},
	{{(char)'-'},{380, 40, 28, 8},{3, 36},0},
	{{(char)'.'},{409, 66, 10, 10},{6, 10},0},
	{{(char)'/'},{420, 9, 45, 69},{2, 67},0},
	{{(char)'0'},{466, 9, 47, 69},{5, 67},0},
	{{(char)'1'},{514, 9, 31, 68},{5, 67},0},
	{{(char)'2'},{546, 9, 44, 67},{1, 67},0},
	{{(char)'3'},{591, 9, 39, 69},{6, 67},0},
	{{(char)'4'},{631, 9, 49, 68},{2, 67},0},
	{{(char)'5'},{681, 11, 37, 67},{6, 65},0},
	{{(char)'6'},{719, 9, 43, 69},{5, 67},0},
	{{(char)'7'},{763, 10, 42, 67},{4, 66},0},
	{{(char)'8'},{806, 9, 41, 69},{7, 67},0},
	{{(char)'9'},{848, 9, 41, 69},{5, 67},0},
	{{(char)':'},{890, 43, 10, 33},{6, 33},0},
	{{(char)';'},{901, 45, 11, 45},{3, 31},0},
	{{(char)'<'},{913, 23, 20, 36},{6, 53},0},
	{{(char)'='},{934, 36, 28, 21},{3, 40},0},
	{{(char)'>'},{963, 24, 20, 36},{6, 52},0},
	{{(char)'?'},{984, 9, 32, 69},{6, 67},0},
	{{(char)'@'},{1017, 20, 55, 57},{2, 56},0},
	{{(char)'A'},{1073, 9, 66, 68},{-2, 67},0},
	{{(char)'B'},{1140, 10, 45, 67},{8, 66},0},
	{{(char)'C'},{1186, 9, 60, 69},{5, 67},0},
	{{(char)'D'},{1247, 10, 68, 67},{7, 66},0},
	{{(char)'E'},{1316, 10, 39, 67},{8, 66},0},
	{{(char)'F'},{1356, 9, 40, 68},{7, 67},0},
	{{(char)'G'},{1397, 9, 65, 69},{5, 67},0},
	{{(char)'H'},{1463, 10, 70, 67},{7, 66},0},
	{{(char)'I'},{1534, 10, 23, 67},{8, 66},0},
	{{(char)'J'},{1558, 10, 28, 88},{2, 66},0},
	{{(char)'K'},{1587, 10, 64, 67},{8, 66},0},
	{{(char)'L'},{1652, 10, 42, 67},{8, 66},0},
	{{(char)'M'},{1695, 8, 86, 69},{4, 68},0},
	{{(char)'N'},{1782, 9, 70, 69},{6, 67},0},
	{{(char)'O'},{1853, 9, 69, 69},{5, 67},0},
	{{(char)'P'},{1923, 10, 44, 67},{7, 66},0},
	{{(char)'Q'},{1968, 9, 128, 85},{5, 67},-64},
	{{(char)'R'},{2097, 10, 67, 67},{7, 66},0},
	{{(char)'S'},{2165, 9, 33, 69},{9, 67},0},
	{{(char)'T'},{2199, 9, 56, 68},{2, 67},0},
	{{(char)'U'},{2256, 10, 66, 68},{2, 66},0},
	{{(char)'V'},{2323, 10, 69, 68},{-2, 66},0},
	{{(char)'W'},{2393, 10, 98, 68},{-1, 66},0},
	{{(char)'X'},{2492, 10, 61, 67},{-1, 66},0},
	{{(char)'Y'},{2554, 10, 63, 67},{-3, 66},0},
	{{(char)'Z'},{2618, 10, 51, 67},{6, 66},0},
	{{(char)'['},{2670, 6, 18, 77},{8, 70},0},
	{{(char)'\\'},{2689, 9, 45, 69},{1, 67},0},
	{{(char)']'},{2735, 6, 20, 77},{5, 70},0},
	{{(char)'^'},{2756, 10, 36, 20},{2, 66},0},
	{{(char)'_'},{2793, 82, 44, 5},{0, -6},0},
	{{(char)'`'},{2838, 9, 29, 13},{1, 67},0},
	{{(char)'a'},{2868, 36, 44, 41},{-2, 40},0},
	{{(char)'b'},{2913, 36, 30, 41},{5, 40},0},
	{{(char)'c'},{2944, 36, 39, 41},{3, 40},0},
	{{(char)'d'},{2984, 36, 44, 41},{5, 40},0},
	{{(char)'e'},{3029, 36, 26, 41},{5, 40},0},
	{{(char)'f'},{3056, 36, 27, 41},{4, 40},0},
	{{(char)'g'},{3084, 36, 43, 41},{3, 40},0},
	{{(char)'h'},{3128, 36, 45, 41},{5, 40},0},
	{{(char)'i'},{3174, 36, 16, 41},{5, 40},0},
	{{(char)'j'},{3191, 36, 18, 53},{1, 40},0},
	{{(char)'k'},{3210, 36, 42, 41},{5, 40},0},
	{{(char)'l'},{3253, 36, 28, 41},{5, 40},0},
	{{(char)'m'},{3282, 35, 55, 42},{3, 41},0},
	{{(char)'n'},{3338, 35, 46, 42},{3, 41},0},
	{{(char)'o'},{3385, 36, 46, 41},{3, 40},0},
	{{(char)'p'},{3432, 36, 29, 41},{4, 40},0},
	{{(char)'q'},{3462, 36, 83, 51},{3, 40},-39},
	{{(char)'r'},{3546, 36, 44, 41},{4, 40},0},
	{{(char)'s'},{3591, 36, 23, 41},{5, 40},0},
	{{(char)'t'},{3615, 36, 37, 41},{1, 40},0},
	{{(char)'u'},{3653, 36, 43, 41},{1, 40},0},
	{{(char)'v'},{3697, 36, 45, 41},{-1, 40},0},
	{{(char)'w'},{3743, 36, 64, 41},{-1, 40},0},
	{{(char)'x'},{3808, 36, 40, 41},{-1, 40},0},
	{{(char)'y'},{3849, 36, 41, 41},{-2, 40},0},
	{{(char)'z'},{3891, 36, 33, 41},{4, 40},0},
	{{(char)'{'},{3925, 7, 23, 76},{4, 69},0},
	{{(char)'}'},{3949, 7, 22, 76},{4, 69},0},
	{{(char)'~'},{3972, 13, 26, 8},{3, 63},0}
};

#pragma endregion

/**
* Constructor.
* @param nX X Position of the node (in full zoom pixel space).
* @param nY Y Position of the node (in full zoom pixel space).
* @param nWidth Width of the node (in full zoom pixel space).
* @param nHeight Height of the node (in full zoom pixel space).
***/
NOD_HelloWorldDx9::NOD_HelloWorldDx9(LONG nX, LONG nY):NOD_Basic(nX, nY, 140, 100),
	m_pcFontTexture(NULL),
	m_vsFont(NULL),
	m_psFont(NULL),
	m_vbFontGlyph(NULL)
{ 
	SetRect(&m_rcFont, 0, 0, 0, 0);
	m_eNodeProvokingType = AQU_NodeProvokingType::Both;
	m_bReturn = false;

	ZeroMemory(&m_sViewport, sizeof(D3DVIEWPORT9));
	ZeroMemory(&m_sFontTextureSize, sizeof(SIZE));

	m_paDecommanders.clear();
	m_szTitle = L"Hello World Dx9"; 

	{
		NOD_Decommander* pD = new NOD_Decommander();
		pD->m_ePlugtype = NOD_Plugtype::WireCable((int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, (int)VMT_IDIRECT3DDEVICE9::Present);
		pD->m_szTitle = L"D3D9::Present";
		pD->m_pInput = nullptr;
		pD->m_paCommanders.clear();
		pD->m_lDecommanderIndex = 0;
		m_paDecommanders.push_back(pD);

		//AQU_D3D_NODE_COMMANDER(AQU_PNT_RECT, L"pSourceRect");
		//AQU_D3D_NODE_COMMANDER(AQU_PNT_RECT, L"pDestRect");
		//AQU_D3D_NODE_COMMANDER(AQU_HWND, L"hDestWindowOverride");
		//AQU_D3D_NODE_COMMANDER(AQU_PNT_RGNDATA, L"pDirtyRegion");
	}
}

/**
* This node supports IDirect3DDevice9->Present(), IDirect3DSwapChain->Present() and IDirect3DDevice9->EndScene().
***/
bool NOD_HelloWorldDx9::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	if ((eD3D >= (int)AQU_Direct3DVersion::DirectX_9_0) &&
		(eD3D <= (int)AQU_Direct3DVersion::DirectX_9_29))
	{
		// return true if d3d, device or swapchain, and present()
		if (((eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9) &&
			(eD3DMethod == (int)VMT_IDIRECT3DDEVICE9::Present)) || 
			((eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9) &&
			(eD3DMethod == (int)VMT_IDIRECT3DDEVICE9::EndScene)) ||
			((eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9) &&
			(eD3DMethod == (int)VMT_IDIRECT3DSWAPCHAIN9::Present)))
			return true;
	}
	return false;
}

/**
* Node Provoke - Draw Text.
***/
void* NOD_HelloWorldDx9::Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
{
	// TODO !! NODE VERY SLOW !!

	// get node data... 
	if (m_paDecommanders[0]->m_pInput != nullptr)
	{
		void** ppIn = (void**)(m_paDecommanders[0]->m_pInput);
		if (ppIn[0])
			m_apInputRaw[0] = *(void**)ppIn[0];
		if (ppIn[1])
			m_apInputRaw[1] = *(void**)ppIn[1];
		if (ppIn[2])
			m_apInputRaw[2] = *(void**)ppIn[2];
		if (ppIn[3])
			m_apInputRaw[3] = *(void**)ppIn[3];

		// debug output... this is a test here for wire bunches
		static bool s_bDebug = false;
		if (!s_bDebug)
		{
			DEBUG_HEX(m_paDecommanders[0]->m_pInput);
			DEBUG_HEX(m_apInputRaw[0]);
			DEBUG_HEX(m_apInputRaw[1]);
			DEBUG_HEX(m_apInputRaw[2]);
			DEBUG_HEX(m_apInputRaw[3]);
			s_bDebug = true;
		}
	}
	else ZeroMemory(&m_apInputRaw[0], sizeof(void*)*4);

	// cast device
	LPDIRECT3DDEVICE9 pcDevice = nullptr;
	bool bReleaseDevice = false;
	if (eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9)
	{
		pcDevice = (LPDIRECT3DDEVICE9)pcThis;
	}
	else if (eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9)
	{
		LPDIRECT3DSWAPCHAIN9 pcSwapChain = (LPDIRECT3DSWAPCHAIN9)pcThis;
		if (!pcSwapChain) 
		{
			OutputDebugString(L"HelloWorld Node : No swapchain !");
			return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
		}
		pcSwapChain->GetDevice(&pcDevice);
		bReleaseDevice = true;
	}
	if (!pcDevice)
	{
		OutputDebugString(L"HelloWorld Node : No device !");
		return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
	}

	// pixel shader created ?
	if (!m_psFont)
	{
		LPD3DXBUFFER pcShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(PS2DRender,(UINT)strlen(PS2DRender),NULL,NULL,"PS2D","ps_3_0",NULL,&pcShader,NULL,&m_ctPFont)))
			pcDevice->CreatePixelShader((DWORD*)pcShader->GetBufferPointer(), &m_psFont);
	}

	// vertex shader created ?
	if (!m_vsFont)
	{
		LPD3DXBUFFER pcShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(VS2DRender,(UINT)strlen(VS2DRender),NULL,NULL,"VS2D","vs_1_1",NULL,&pcShader,NULL,&m_ctVFont)))
		{
			OutputDebugString(L"Vertex shader compiled!");
			pcDevice->CreateVertexShader((DWORD*)pcShader->GetBufferPointer(), &m_vsFont);
		}
	} 

	// font texture created ?
	if (!m_pcFontTexture)
		CreateFontTexture(pcDevice);
	else
		Render(pcDevice);

	// vertex buffer created ?
	if ((!m_vbFontGlyph) && (m_sFontTextureSize.cx))
		CreateVertexBuffer(pcDevice);

	// release device if provided by swapchain
	if (bReleaseDevice) pcDevice->Release();

	return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
}

/**
* Render all text.
***/
void NOD_HelloWorldDx9::Render(LPDIRECT3DDEVICE9 pcDevice)
{
	if ((m_psFont) && (m_vbFontGlyph))
	{
		pcDevice->BeginScene();

		// save states
		IDirect3DStateBlock9* pStateBlock;
		pcDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

		// set states
		pcDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pcDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pcDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pcDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
		pcDevice->SetTextureStageState(0, D3DTSS_CONSTANT, 0xffffffff);
		pcDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pcDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		pcDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pcDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
		pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		pcDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		pcDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		pcDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		// set stream
		pcDevice->SetVertexShader(m_vsFont);
		pcDevice->SetPixelShader(m_psFont);
		pcDevice->SetVertexDeclaration(NULL);
		pcDevice->SetTexture(0, m_pcFontTexture);
		pcDevice->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);		
		pcDevice->SetRenderTarget(0, NULL);
		pcDevice->SetStreamSource(0, m_vbFontGlyph, 0, sizeof(TexturedVertex));

		// set shader constants
		float Color[] = { 1.0f, 1.0f, 0.5f, 0.5f };
		m_ctPFont->SetFloatArray(pcDevice,"Col", Color,4);

		// draw
		RenderText(pcDevice, L"Hello World ! Hello Aquilinus !", 0.5f, -0.2f, 1.0f);

		pStateBlock->Apply();
		pStateBlock->Release();

		pcDevice->EndScene();
	}
}

/**
* Create font vertex buffer.
***/
void NOD_HelloWorldDx9::CreateVertexBuffer(LPDIRECT3DDEVICE9 pcDevice)
{
	// get viewport
	pcDevice->GetViewport(&m_sViewport);

	// create simple vertex buffer
	pcDevice->CreateVertexBuffer(sizeof(TexturedVertex) * 4 * g_dwHelloWorldFontGlyphesNum, NULL,
		D3DFVF_XYZW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_vbFontGlyph, NULL);

	// lock, create vertices
	TexturedVertex* vertices;
	m_vbFontGlyph->Lock(0, 0, (void**)&vertices, NULL);

	// create viewport helpers
	m_fViewportWidth = (float)m_sViewport.Width;
	m_fViewportHeight = (float)m_sViewport.Height;
	float fXViewportOrigin = -1.0f;
	float fYViewportOrigin = 1.0f;

	// compute the scale by aspect ratio, on a hd screen (1920x1080) it should render in about actual pixel size
	float fAspectRatio = m_fViewportWidth/m_fViewportHeight;
	m_fXScale = 1/400.0f/fAspectRatio;
	m_fYScale = 1/400.0f;

	// loop through glyphes
	DWORD dwVertexIndex = 0;
	float fTextureSizeX = (float)m_sFontTextureSize.cx;
	float fTextureSizeY = (float)m_sFontTextureSize.cy;
	for (DWORD i = 0; i < g_dwHelloWorldFontGlyphesNum; i++)
	{
		// create glyph vertices
		vertices[dwVertexIndex].x = fXViewportOrigin;
		vertices[dwVertexIndex].y = fYViewportOrigin;
		vertices[dwVertexIndex].z = 0.0f;
		vertices[dwVertexIndex].w = 1.0f;
		vertices[dwVertexIndex].u = float(g_HelloWorldFontGlyphes[i].rcSrc.left)/fTextureSizeX;
		vertices[dwVertexIndex].v = float(g_HelloWorldFontGlyphes[i].rcSrc.top)/fTextureSizeY;
		dwVertexIndex++;

		vertices[dwVertexIndex].x = fXViewportOrigin+(float(g_HelloWorldFontGlyphes[i].rcSrc.right)*m_fXScale);
		vertices[dwVertexIndex].y = fYViewportOrigin;
		vertices[dwVertexIndex].z = 0.0f;
		vertices[dwVertexIndex].w = 1.0f;
		vertices[dwVertexIndex].u = float(g_HelloWorldFontGlyphes[i].rcSrc.left+g_HelloWorldFontGlyphes[i].rcSrc.right)/fTextureSizeX;
		vertices[dwVertexIndex].v = float(g_HelloWorldFontGlyphes[i].rcSrc.top)/fTextureSizeY;
		dwVertexIndex++;

		vertices[dwVertexIndex].x = fXViewportOrigin+(float(g_HelloWorldFontGlyphes[i].rcSrc.right)*m_fXScale);
		vertices[dwVertexIndex].y = fYViewportOrigin-(float(g_HelloWorldFontGlyphes[i].rcSrc.bottom)*m_fYScale);
		vertices[dwVertexIndex].z = 0.0f;
		vertices[dwVertexIndex].w = 1.0f;
		vertices[dwVertexIndex].u = float(g_HelloWorldFontGlyphes[i].rcSrc.left+g_HelloWorldFontGlyphes[i].rcSrc.right)/fTextureSizeX;
		vertices[dwVertexIndex].v = float(g_HelloWorldFontGlyphes[i].rcSrc.top+g_HelloWorldFontGlyphes[i].rcSrc.bottom)/fTextureSizeY;
		dwVertexIndex++;

		vertices[dwVertexIndex].x = fXViewportOrigin;
		vertices[dwVertexIndex].y = fYViewportOrigin-(float(g_HelloWorldFontGlyphes[i].rcSrc.bottom)*m_fYScale);
		vertices[dwVertexIndex].z = 0.0f;
		vertices[dwVertexIndex].w = 1.0f;
		vertices[dwVertexIndex].u = float(g_HelloWorldFontGlyphes[i].rcSrc.left)/fTextureSizeX;
		vertices[dwVertexIndex].v = float(g_HelloWorldFontGlyphes[i].rcSrc.top+g_HelloWorldFontGlyphes[i].rcSrc.bottom)/fTextureSizeY;
		dwVertexIndex++;
	}

	m_vbFontGlyph->Unlock();

}

/**
* Create the font texture.
***/
void NOD_HelloWorldDx9::CreateFontTexture(LPDIRECT3DDEVICE9 pcDevice)
{
	// create font texture by resource font image
	HMODULE hModule = GetModuleHandle(L"Aquilinus.dll");
	HBITMAP hbFont = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_FONT02));
	if(hbFont)
	{
		// get size of bitmap
		BITMAP bm;
		GetObject(hbFont, sizeof(bm), &bm);

		// set font texture size
		m_sFontTextureSize.cx = bm.bmWidth;
		m_sFontTextureSize.cy = bm.bmHeight;

		// set font rectangle
		SetRect(&m_rcFont, 0, 0, bm.bmWidth, bm.bmHeight);

		// create font texture(s)
		LPDIRECT3DTEXTURE9 pFontTexture = NULL;
		pcDevice->CreateTexture(bm.bmWidth, bm.bmHeight, 0, NULL, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pcFontTexture, NULL);
		pcDevice->CreateTexture(bm.bmWidth, bm.bmHeight, 0, NULL, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pFontTexture, NULL);

		if (m_pcFontTexture)
		{
			// get destination hdc
			HDC hdcDest; 
			LPDIRECT3DSURFACE9 pSurface;
			pFontTexture->GetSurfaceLevel (0,&pSurface);
			pSurface->GetDC(&hdcDest);

			// get source hdc
			HDC hdcImage = CreateCompatibleDC(NULL);
			HBITMAP hbmOld = (HBITMAP)SelectObject(hdcImage, hbFont);

			// blit 
			BitBlt(hdcDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);

			// release surface
			pSurface->ReleaseDC(hdcDest);
			pSurface->Release();
			pSurface=NULL;

			// release bitmap
			DeleteDC(hdcImage);
			DeleteObject(hbFont);

			// update texture, release memory texture
			pcDevice->UpdateTexture(pFontTexture, m_pcFontTexture);
			pFontTexture->Release();
		}
		else
		{
			OutputDebugString(L"Aquilinus : Can't create font texture.");
			if (pFontTexture) pFontTexture->Release();
		}
	}
}

/**
* Get the index of the character in the font table.
***/
DWORD NOD_HelloWorldDx9::GetGlyphIndex(char character)
{
	DWORD d;
	for (d = 0; d < g_dwHelloWorldFontGlyphesNum; d++)
	{
		if (character == g_HelloWorldFontGlyphes[d].character)
			return d;
	}

	return 0;
}

/**
* Renders a glyph using the main font table by the specified index.
* @param [in] dwIndex The index of the glyph.
* @param [in] fX The destination X.
* @param [in] fY The destination Y.
* @param [in] fScale The size of the text (1.0f = original size).
***/
HRESULT NOD_HelloWorldDx9::RenderGlyph(LPDIRECT3DDEVICE9 pcDevice,DWORD dwIndex, float fX, float fY, float fScale)
{
	// set glyph position, first calculate top
	float fYY = fY-(float(g_HelloWorldFontGlyphes[dwIndex].rcSrc.top)*m_fYScale);
	float Pos[] = {fX, fYY};
	m_ctVFont->SetFloatArray(pcDevice,"Pos", Pos, 2);
	
	// draw
	return pcDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, dwIndex*4, 2);
}

/**
* Outputs text to the backbuffer.
* @param [in] szText The text to render.
* @param [in] fX The destination X.
* @param [in] fY The destination Y.
* @param [in] fScale The size of the text (1.0f = original size).
***/
HRESULT NOD_HelloWorldDx9::RenderText(LPDIRECT3DDEVICE9 pcDevice,LPCWSTR szText, float fX, float fY, float fScale)
{
	DWORD dwIndex = 0;
	float fXX = fX;

	// walk through the text, render glyphes
	for (dwIndex = 0; dwIndex < wcslen(szText); dwIndex++)
	{
		DWORD dwI = GetGlyphIndex((char)szText[dwIndex]);
		RenderGlyph(pcDevice, dwI, fXX, fY, fScale);
		fXX+=(float(g_HelloWorldFontGlyphes[dwI].rcSrc.right+g_HelloWorldFontGlyphes[dwI].nAddSpacing)*m_fXScale);
	}

	return S_OK;
}
