/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#ifndef D3D9PROXYVERTEXSHADER_H_INCLUDED
#define D3D9PROXYVERTEXSHADER_H_INCLUDED

#include <d3d9.h>
#include <memory>
#include <vector>
#include "Direct3DVertexShader9.h"
#include "Direct3DDevice9.h"
#include "ShaderRegisters.h"
#include "D3DProxyDevice.h"
#include "ShaderModifications.h"



class D3D9ProxyVertexShader : public BaseDirect3DVertexShader9
{
public:
	D3D9ProxyVertexShader(IDirect3DVertexShader9* pActualVertexShader, D3DProxyDevice* pOwningDevice, std::shared_ptr<ShaderRegisters> spProxyDeviceShaderRegisters, ShaderModifications* pModLoader);
	virtual ~D3D9ProxyVertexShader();

	/* Updates the data in this for any constants that exist in both this and other. (from other) that aren't dirty 
		dirties any registers that exist in here that didn't exist in previous */
	void MakeActive(D3D9ProxyVertexShader* previousActiveVertexShader);
	
	/* Updates all dirty constants from proxy device registers and sets them on the actual device (marks any registers that are set to actual as clean in proxy register) */
	void UpdateAndApply(D3DProxyDevice::EyeSide side);

protected:

	IDirect3DDevice9* m_pActualDevice;

	std::vector<StereoShaderConstant<float>> m_pStereoModifiedConstantsF;
	std::shared_ptr<ShaderRegisters> m_spProxyDeviceShaderRegisters;
	
};

#endif
