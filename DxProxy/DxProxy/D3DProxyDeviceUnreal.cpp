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
	//roll_mode = 1;
	matrixIndex = getMatrixIndex();
}

/*
bool D3DProxyDeviceUnreal::CouldOverwriteMatrix(UINT StartRegister, UINT Vector4fCount) 
{
	return ((StartRegister >= matrixIndex) && (StartRegister < (matrixIndex + Vector4fCount)));
}


bool D3DProxyDeviceUnreal::ContainsMatrixToModify(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return (Vector4fCount >= 4 && validRegister(StartRegister));
}

StereoShaderConstant<float> D3DProxyDeviceUnreal::CreateStereoShaderConstant(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	D3DXMATRIX tempMatrix = const_cast<float*>(pConstantData);
		
	D3DXMATRIX tempLeft (tempMatrix * matViewTranslationLeft);
	D3DXMATRIX tempRight (tempMatrix * matViewTranslationRight);

	return StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4);
}*/






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

UINT D3DProxyDeviceUnreal::getMatrixIndex()
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