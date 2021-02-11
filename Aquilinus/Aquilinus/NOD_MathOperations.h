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
#ifndef NOD_MATHOPERATIONS
#define NOD_MATHOPERATIONS

#include <d3dx9.h>

#ifndef _TRACE
#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#endif

/**
* Mathematical operation lookat.
* Creates Lookat from Euler input.
***/
class NOD_MathLookat : public NOD_Basic
{
public:
	NOD_MathLookat(LONG nX, LONG nY) :NOD_Basic(nX, nY, 100, 200)
	{
		m_szTitle = L"Math Pos Lookat";
		m_eNodeProvokingType = AQU_NodeProvokingType::Both;
		m_bReturn = false;

		{
			// create commander
			NOD_Commander * pC = new NOD_Commander();
			pC->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pC->m_pOutput = &m_sLookAtOutput.x;
			pC->m_szTitle = L"LookAt X";
			m_paCommanders.push_back(pC);
		}
		{
			// create commander
			NOD_Commander * pC = new NOD_Commander();
			pC->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pC->m_pOutput = &m_sLookAtOutput.y;
			pC->m_szTitle = L"LookAt Y";
			m_paCommanders.push_back(pC);
		}
		{
			// create commander
			NOD_Commander * pC = new NOD_Commander();
			pC->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pC->m_pOutput = &m_sLookAtOutput.z;
			pC->m_szTitle = L"LookAt Z";
			m_paCommanders.push_back(pC);
		}
		{
			// create decommander
			NOD_Decommander * pD = new NOD_Decommander();
			pD->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pD->m_pInput = nullptr;
			pD->m_szTitle = L"Euler X";
			pD->m_lDecommanderIndex = (LONG)m_paDecommanders.size();
			m_paDecommanders.push_back(pD);
		}
		{
			// create decommander
			NOD_Decommander * pD = new NOD_Decommander();
			pD->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pD->m_pInput = nullptr;
			pD->m_szTitle = L"Euler Y";
			pD->m_lDecommanderIndex = (LONG)m_paDecommanders.size();
			m_paDecommanders.push_back(pD);
		}
		{
			// create decommander
			NOD_Decommander * pD = new NOD_Decommander();
			pD->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
			pD->m_pInput = nullptr;
			pD->m_szTitle = L"Euler Z";
			pD->m_lDecommanderIndex = (LONG)m_paDecommanders.size();
			m_paDecommanders.push_back(pD);
		}

	}

	virtual void* Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
	{
		// set data
		m_sEulerInput = D3DXVECTOR3();
		if (m_paDecommanders[0]->m_pInput) m_sEulerInput.x = *(float*)(m_paDecommanders[0]->m_pInput);
		if (m_paDecommanders[1]->m_pInput) m_sEulerInput.y = *(float*)(m_paDecommanders[1]->m_pInput);
		if (m_paDecommanders[2]->m_pInput) m_sEulerInput.z = *(float*)(m_paDecommanders[2]->m_pInput);

		// operate
		D3DXMATRIX sMatrix;
		D3DXMatrixRotationYawPitchRoll(&sMatrix, -m_sEulerInput.y, -m_sEulerInput.x, m_sEulerInput.z);

		D3DXVECTOR3 sInput = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3Transform(&m_sLookAtOutput, &sInput, &sMatrix);

		return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
	}
	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_MATH_LOOKAT; }

	/**
	* Euler Input data.
	***/
	D3DXVECTOR3 m_sEulerInput;
	/**
	* Lookat output data, normalized.
	***/
	D3DXVECTOR4 m_sLookAtOutput;
};

#endif