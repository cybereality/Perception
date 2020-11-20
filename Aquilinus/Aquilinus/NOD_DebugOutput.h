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
#ifndef NOD_DEBUGOUTPUT
#define NOD_DEBUGOUTPUT

#include<stdio.h>
#include<string.h>
#include<sstream>
#include<d3dx9.h>
#include<d3d9.h>

/**
* Output 3D Vector data.
***/
class NOD_OutputDebugVector : public NOD_Basic
{
public:
	NOD_OutputDebugVector(LONG nX, LONG nY) :NOD_Basic(nX, nY, 340, 90)
	{
		m_paCommanders.clear();
		{ m_szTitle = L"OutputDebugD3DVector"; m_eNodeProvokingType = AQU_NodeProvokingType::None; }
		{ NOD_Decommander * pD = new NOD_Decommander();	pD->m_ePlugtype = NOD_Plugtype::AQU_PNT_VECTOR4F; pD->m_pInput = nullptr; pD->m_szTitle = L"pD3DVector4"; pD->m_paCommanders.clear(); pD->m_lDecommanderIndex = 0; m_paDecommanders.push_back(pD); }

	}

	/*** NOD_Basic public members ***/
	virtual HRESULT Draw(POINT vcOrigin)
	{
		/*HRESULT ret = NOD_Basic::Draw(pDirectDraw, vcOrigin, fZoom);

		// get node data... and draw
		if (m_paDecommanders[0]->m_pInput != nullptr)
		{
			if ((*((void**)m_paDecommanders[0]->m_pInput)) != nullptr)
			{
				D3DXVECTOR4 * pVec = *(D3DXVECTOR4**)m_paDecommanders[0]->m_pInput;
				std::wstringstream strStream;
				strStream << L"X:" << pVec->x << L"::Y:" << pVec->y << L"::Z:" << pVec->z << L"::W:" << pVec->w;
				pDirectDraw->RenderText(strStream.str().c_str(), 10 + m_vecPos.x, 60 + m_vecPos.y, vcOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);
			}
		}*/

		return S_OK;
	}
	virtual const char*   GetNodeType() { return "Debug D3D Vector"; }
	virtual       UINT32  GetNodeTypeId() { return ELEMENTARY_NODE_DEBUG_D3DVECTOR; }
};

/**
* Output 3D Matrix data.
***/
class NOD_OutputDebugMatrix : public NOD_Basic
{
public:
	NOD_OutputDebugMatrix(LONG nX, LONG nY) :NOD_Basic(nX, nY, 420, 160)
	{
		m_paCommanders.clear();
		{ m_szTitle = L"OutputDebugD3DMatrix"; m_eNodeProvokingType = AQU_NodeProvokingType::None; }
		{ NOD_Decommander * pD = new NOD_Decommander();	pD->m_ePlugtype = NOD_Plugtype::AQU_PNT_D3DMATRIX; pD->m_pInput = nullptr; pD->m_szTitle = L"pD3DMatrix"; pD->m_paCommanders.clear(); pD->m_lDecommanderIndex = 0; m_paDecommanders.push_back(pD); }

	}

	/*** NOD_Basic public members ***/
	virtual       HRESULT Draw(POINT vcOrigin)
	{
		//HRESULT ret = NOD_Basic::Draw(pDirectDraw, vcOrigin, fZoom);

		//// get node data... and draw
		//if (m_paDecommanders[0]->m_pInput != nullptr)
		//{
		//	if ((*((void**)m_paDecommanders[0]->m_pInput)) != nullptr)
		//	{
		//		D3DMATRIX * pMat = *(D3DMATRIX**)m_paDecommanders[0]->m_pInput;
		//		std::wstringstream strStream;

		//		// loop through matrix, output data
		//		int x = 10, y = 60;
		//		for (int i = 0; i < 4; i++)
		//		{
		//			for (int j = 0; j < 4; j++)
		//			{
		//				strStream << L"[" << i << L"][" << j << L"]:" << pMat->m[i][j];
		//				pDirectDraw->RenderText(strStream.str().c_str(), x + m_vecPos.x, y + m_vecPos.y, vcOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		//				strStream.str(std::wstring());
		//				x += 100;
		//			}
		//			x = 10;
		//			y += 24;
		//		}
		//	}
		//}

		return S_OK;
	}
	virtual const char*   GetNodeType() { return "Debug D3D Matrix"; }
	virtual       UINT32  GetNodeTypeId() { return ELEMENTARY_NODE_DEBUG_D3DMATRIX; }
};


#endif