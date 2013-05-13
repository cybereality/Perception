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
		
		currentMatrix = const_cast<float*>(pConstantData);
	
		D3DXMATRIX sourceMatrix(currentMatrix);
		
		sourceMatrix = sourceMatrix * matViewTranslation; 
		currentMatrix = (float*)sourceMatrix;

		/*
		char buf[32];
		LPCSTR psz = NULL;

		sprintf_s(buf, "sep: %f", separation);
		psz = buf;
		OutputDebugString(psz);
		OutputDebugString("\n");
		*/

		if(false && saveDebugFile)
		{
			char vcString[1024];
			sprintf_s(vcString, 
				"register: %d, count: %d\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n" 
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n\n", StartRegister, Vector4fCount,
				currentMatrix[0], currentMatrix[1], currentMatrix[2], currentMatrix[3],
				currentMatrix[4], currentMatrix[5], currentMatrix[6], currentMatrix[7],
				currentMatrix[8], currentMatrix[9], currentMatrix[10], currentMatrix[11],
				currentMatrix[12], currentMatrix[13], currentMatrix[14], currentMatrix[15]
				);

			debugFile << vcString;
		}


		return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, currentMatrix, Vector4fCount);
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
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