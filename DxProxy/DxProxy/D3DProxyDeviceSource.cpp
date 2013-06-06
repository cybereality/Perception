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

#include "D3DProxyDeviceSource.h"
#include "StereoViewFactory.h"
#include <algorithm>

D3DProxyDeviceSource::D3DProxyDeviceSource(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy),
	m_validMatrixRegisters()
{
	OutputDebugString("D3D ProxyDev Source Created\n");
}

D3DProxyDeviceSource::~D3DProxyDeviceSource()
{
}

void D3DProxyDeviceSource::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Source Init\n");
	D3DProxyDevice::Init(cfg);
	roll_mode = 1;

	// This set of registers was being used by all source games (all use game type 101, labeled as SOURCE_L4D)
	m_validMatrixRegisters.clear();
	m_validMatrixRegisters.push_back(4);
	m_validMatrixRegisters.push_back(8);
	m_validMatrixRegisters.push_back(51);
}

/*
bool CheckMatrix(UINT StartRegister, UINT Vector4fCount, UINT theMatrixIndex) 
{
	return ((StartRegister >= theMatrixIndex) && (StartRegister < (theMatrixIndex + Vector4fCount)));
}

bool D3DProxyDeviceSource::CouldOverwriteMatrix(UINT StartRegister, UINT Vector4fCount) 
{
	bool couldOverwrite = false;
	auto itValidRegisters = m_validMatrixRegisters.begin();
	while (itValidRegisters != m_validMatrixRegisters.end()) {
		if (validRegister(StartRegister)) {//(CheckMatrix(StartRegister, Vector4fCount, *itValidRegisters)) {
			couldOverwrite = true;
			break;
		}
		
		++itValidRegisters;
	}

	return couldOverwrite;
}

bool D3DProxyDeviceSource::ContainsMatrixToModify(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return (Vector4fCount >= 4 && validRegister(StartRegister) && (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f));
}

StereoShaderConstant<float> D3DProxyDeviceSource::CreateStereoShaderConstant(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	D3DXMATRIX tempMatrix (const_cast<float*>(pConstantData));
	D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			
	D3DXMATRIX tempLeft (tempMatrix * matViewTranslationLeft);
	D3DXMATRIX tempRight (tempMatrix * matViewTranslationRight);

	D3DXMatrixTranspose(&tempLeft, &tempLeft);
	D3DXMatrixTranspose(&tempRight, &tempRight);
		
	return StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4);
}
*/


bool D3DProxyDeviceSource::validRegister(UINT reg)
{
	/*switch(game_type)
	{
	case SOURCE_L4D:*/
		if(std::find(m_validMatrixRegisters.begin(), m_validMatrixRegisters.end(), reg) != m_validMatrixRegisters.end())
			return true;
		else return false;
		/*break;
	default:
		return true;
	}*/
}
