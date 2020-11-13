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
#ifndef NOD_D3DX9_DEF
#define NOD_D3DX9_DEF

#include<stdio.h>
#include<string.h>
#include<sstream>
#include<d3dx9.h>
#include<d3d9.h>

/* D3DBOX* pDestBox;
D3DBOX* pSrcBox;
D3DCOLOR ColorKey;
D3DFORMAT Format;
D3DFORMAT SrcFormat;
D3DPOOL Pool;
D3DXIMAGE_FILEFORMAT DestFormat;
D3DXIMAGE_INFO* pSrcInfo;
DWORD Channel;
DWORD Filter;
DWORD Flags;
DWORD MipFilter;
DWORD Usage;
FLOAT Amplitude;
HMODULE hSrcModule;
LPCSTR pDestFile;
LPCSTR pSrcFile;
LPCSTR pSrcResource;
LPCVOID pSrcData;
LPCVOID pSrcMemory;
LPCWSTR pDestFile;
LPCWSTR pSrcFile;
LPCWSTR pSrcResource;
LPD3DXBUFFER* ppDestBuf;
LPD3DXFILL2D pFunction;
LPD3DXTEXTURESHADER pTextureShader;
LPDIRECT3DBASETEXTURE9 pBaseTexture;
LPDIRECT3DBASETEXTURE9 pSrcTexture;
LPDIRECT3DCUBETEXTURE9* ppCubeTexture;
LPDIRECT3DCUBETEXTURE9 pCubeTexture;
LPDIRECT3DSURFACE9 pDestSurface;
LPDIRECT3DSURFACE9 pSrcSurface;
LPDIRECT3DTEXTURE9* ppTexture;
LPDIRECT3DTEXTURE9 pSrcTexture;
LPDIRECT3DTEXTURE9 pTexture;
LPDIRECT3DVOLUME9 pDestVolume;
LPDIRECT3DVOLUME9 pSrcVolume;
LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture;
LPDIRECT3DVOLUMETEXTURE9 pVolumeTexture;
LPVOID pData;
PALETTEENTRY* pDestPalette;
PALETTEENTRY* pPalette;
PALETTEENTRY* pSrcPalette;
RECT* pDestRect;
RECT* pSrcRect;
UINT Depth;
UINT Height;
UINT MipLevels;
UINT Size;
UINT SrcDataSize;
UINT SrcLevel;
UINT SrcPitch;
UINT SrcRowPitch;
UINT SrcSlicePitch;
UINT Width; */

/**
* NOD_D3DX9 commanders enumeration.
***/
enum NOD_D3DX9_Commanders
{
	pDestBox,
	pSrcBox,
	ColorKey,
	Format,
	SrcFormat,
	Pool,
	DestFormat,
	pSrcInfo,
	Channel,
	Filter,
	Flags,
	MipFilter,
	Usage,
	Amplitude,
	hSrcModule,
	pDestFile,
	pSrcFile,
	pSrcResource,
	pSrcData,
	pSrcMemory,
	pDestFileW,
	pSrcFileW,
	pSrcResourceW,
	ppDestBuf,
	pFunction,
	pTextureShader,
	pBaseTexture,
	pSrcTexture,
	ppCubeTexture,
	pCubeTexture,
	pDestSurface,
	pSrcSurface,
	ppTexture,
	pTexture,
	pDestVolume,
	pSrcVolume,
	ppVolumeTexture,
	pVolumeTexture,
	pData,
	pDestPalette,
	pPalette,
	pSrcPalette,
	pDestRect,
	pSrcRect,
	Depth,
	Height,
	MipLevels,
	Size,
	SrcDataSize,
	SrcLevel,
	SrcPitch,
	SrcRowPitch,
	SrcSlicePitch,
	Width,
};


/**
* Output 3D Vector data.
***/
class NOD_D3DX9 : public NOD_Basic
{
public:
	NOD_D3DX9(LONG nX, LONG nY) : NOD_Basic(nX, nY, 340, 90)
	{
		m_paCommanders.clear();
		{ m_szTitle = L"D3DX9"; m_eNodeProvokingType = AQU_NodeProvokingType::OnlyProvoker; }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DBOX; pC->m_szTitle = L"pDestBox"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DBOX; pC->m_szTitle = L"pSrcBox"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_D3DCOLOR; pC->m_szTitle = L"ColorKey"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_D3DFORMAT; pC->m_szTitle = L"Format"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_D3DFORMAT; pC->m_szTitle = L"SrcFormat"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_D3DPOOL; pC->m_szTitle = L"Pool"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_D3DXIMAGE_FILEFORMAT; pC->m_szTitle = L"DestFormat"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DXIMAGE_INFO; pC->m_szTitle = L"pSrcInfo"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Channel"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Filter"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Flags"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); } // 10
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"MipFilter"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Usage"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_FLOAT; pC->m_szTitle = L"Amplitude"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_HANDLE; pC->m_szTitle = L"hSrcModule"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_CHAR; pC->m_szTitle = L"pDestFile"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_CHAR; pC->m_szTitle = L"pSrcFile"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_CHAR; pC->m_szTitle = L"pSrcResource"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_VOID; pC->m_szTitle = L"pSrcData"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_VOID; pC->m_szTitle = L"pSrcMemory"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_WCHAR; pC->m_szTitle = L"pDestFileW"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); } // 20
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_WCHAR; pC->m_szTitle = L"pSrcFileW"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_WCHAR; pC->m_szTitle = L"pSrcResourceW"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DXBUFFER; pC->m_szTitle = L"ppDestBuf"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DXFILL2D; pC->m_szTitle = L"pFunction"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DXTEXTURESHADER; pC->m_szTitle = L"pTextureShader"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DBASETEXTURE9; pC->m_szTitle = L"pBaseTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DBASETEXTURE9; pC->m_szTitle = L"pSrcTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PPNT_IDIRECT3DCUBETEXTURE9; pC->m_szTitle = L"ppCubeTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DCUBETEXTURE9; pC->m_szTitle = L"pCubeTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9; pC->m_szTitle = L"pDestSurface"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); } // 30
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DSURFACE9; pC->m_szTitle = L"pSrcSurface"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PPNT_IDIRECT3DTEXTURE9; pC->m_szTitle = L"ppTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DTEXTURE9; pC->m_szTitle = L"pTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DVOLUME9; pC->m_szTitle = L"pDestVolume"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DVOLUME9; pC->m_szTitle = L"pSrcVolume"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PPNT_IDIRECT3DVOLUMETEXTURE9; pC->m_szTitle = L"ppVolumeTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DVOLUMETEXTURE9; pC->m_szTitle = L"pVolumeTexture"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_VOID; pC->m_szTitle = L"pData"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_PALETTEENTRY; pC->m_szTitle = L"pDestPalette"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_PALETTEENTRY; pC->m_szTitle = L"pPalette"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); } // 40
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_PALETTEENTRY; pC->m_szTitle = L"pSrcPalette"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_RECT; pC->m_szTitle = L"pDestRect"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_RECT; pC->m_szTitle = L"pSrcRect"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Depth"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Height"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"MipLevels"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Size"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"SrcDataSize"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"SrcLevel"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"SrcPitch"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"SrcRowPitch"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"SrcSlicePitch"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{ NOD_Commander* pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::AQU_UINT; pC->m_szTitle = L"Width"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC); }
		{
			m_cProvoker.m_eD3D = AQU_Direct3DVersion::DirectX_9_0;
			m_cProvoker.m_eD3DInterface = ITA_D3D9INTERFACES::ITA_D3D9Interfaces::D3DX9;

			// method index to be overwritten by method
			m_cProvoker.m_eD3DMethod = 0;
		}
	}

	virtual const char*   GetNodeType() { return "D3DX9"; }
	virtual       UINT32  GetNodeTypeId() { return D3DOPS_NODE_D3DX9; }
};

#endif