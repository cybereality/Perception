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

#ifndef D3DPROXYDEVICEUNREAL_H_INCLUDED
#define D3DPROXYDEVICEUNREAL_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include "ProxyHelper.h"

class D3DProxyDeviceUnreal : public D3DProxyDevice
{
public:
	D3DProxyDeviceUnreal(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~D3DProxyDeviceUnreal();
	
	/*
	virtual bool CouldOverwriteMatrix(UINT StartRegister, UINT Vector4fCount);
	virtual bool ContainsMatrixToModify(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
	virtual StereoShaderConstant<float> CreateStereoShaderConstant(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	*/
	virtual void Init(ProxyHelper::ProxyConfig& cfg);
	

	bool validRegister(UINT reg);
	UINT getMatrixIndex();



private:
	
	bool isPartOfMatrix(UINT Register);
};

#endif