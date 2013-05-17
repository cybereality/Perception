/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#include "D3DProxyDeviceUnreal.h"

D3DProxyDeviceUnreal::D3DProxyDeviceUnreal(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy)
{
	m_pCurrentVShaderMatrix = NULL;
	m_CurrentVShaderRegister = 0;
	m_CurrentVShaderVec4Count = -1;
	m_bAdjustedShaderActive = false;
}

D3DProxyDeviceUnreal::~D3DProxyDeviceUnreal()
{
}

void D3DProxyDeviceUnreal::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Unreal Init\n");
	D3DProxyDevice::Init(cfg);
	roll_mode = 1;
	matrixIndex = getMatrixIndex();
}







HRESULT WINAPI D3DProxyDeviceUnreal::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	
	if(stereoView->initialized && Vector4fCount >= 4 && validRegister(StartRegister)) // && (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
	{
		
		m_pCurrentVShaderMatrix = const_cast<float*>(pConstantData);
		m_CurrentVShaderRegister = StartRegister;
		m_CurrentVShaderVec4Count = Vector4fCount;
		
		D3DXMATRIX tempMatrix(m_pCurrentVShaderMatrix);
		
		if (m_currentRenderingSide == Left)
			tempMatrix = tempMatrix * matViewTranslationLeft; 
		else
			tempMatrix = tempMatrix * matViewTranslationRight; 

		m_bAdjustedShaderActive = true;

		/*
		char buf[32];
		LPCSTR psz = NULL;

		sprintf_s(buf, "sep: %f", separation);
		psz = buf;
		OutputDebugString(psz);
		OutputDebugString("\n");
		*/

		/*if(false && saveDebugFile)
		{
			char vcString[1024];
			sprintf_s(vcString, 
				"register: %d, count: %d\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n" 
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n\n", StartRegister, Vector4fCount,
				tempMatrix[0], tempMatrix[1], tempMatrix[2], tempMatrix[3],
				tempMatrix[4], tempMatrix[5], tempMatrix[6], tempMatrix[7],
				tempMatrix[8], tempMatrix[9], tempMatrix[10], tempMatrix[11],
				tempMatrix[12], tempMatrix[13], tempMatrix[14], tempMatrix[15]
				);

			debugFile << vcString;
		}*/


		return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, (float*)&tempMatrix, Vector4fCount);
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}


HRESULT WINAPI D3DProxyDeviceUnreal::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	

	return D3DProxyDevice::SetVertexShader(pShader);
}



bool D3DProxyDeviceUnreal::setDrawingSide(EyeSide side)
{
	if (!D3DProxyDevice::setDrawingSide(side))
		return false;

	if (m_bAdjustedShaderActive) {
		D3DXMATRIX tempMatrix(m_pCurrentVShaderMatrix);
		//tempMatrix = tempMatrix * (*m_pCurrentMatViewTransform); 

		if (side == Left)
			tempMatrix = tempMatrix * matViewTranslationLeft; 
		else
			tempMatrix = tempMatrix * matViewTranslationRight; 

		m_VertexShaderConstantTracker.ModifyShaderConstantF(m_CurrentVShaderRegister, (float*)&tempMatrix, m_CurrentVShaderVec4Count);
		m_VertexShaderConstantTracker.SetAll();
		BaseDirect3DDevice9::SetVertexShader(m_pActiveVertexShader->getActual());
	}

	return true;
}





bool D3DProxyDeviceUnreal::validRegister(UINT reg)
{
	switch(game_type)
	{
	case UNREAL_MIRROR:
	case UNREAL_UT3:
		if(reg == 0) return true;
		else return false;
		break;
	case UNREAL_BIOSHOCK:
		return true;
		break;
	default:
		if(reg == 0) return true;
		else return false;
	}
}

int D3DProxyDeviceUnreal::getMatrixIndex()
{
	switch(game_type)
	{
	case UNREAL_MIRROR:
	case UNREAL_UT3:
		return 0;
		break;
	case UNREAL_BIOSHOCK:
		return 2;
		break;
	default:
		return 0;
	}
}