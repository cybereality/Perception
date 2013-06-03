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

#include "D3D9ProxyVertexShader.h"

D3D9ProxyVertexShader::D3D9ProxyVertexShader(IDirect3DVertexShader9* pActualVertexShader, D3DProxyDevice *pOwningDevice, std::shared_ptr<ShaderRegisters> spProxyDeviceShaderRegisters,  ShaderModifications* pModLoader) :
	BaseDirect3DVertexShader9(pActualVertexShader, pOwningDevice),
	m_pActualDevice(pOwningDevice->getActual()),
	m_pStereoModifiedConstantsF(),
	m_spProxyDeviceShaderRegisters(spProxyDeviceShaderRegisters)
{
	m_pStereoModifiedConstantsF = pModLoader->GetModifiedConstantsF(pActualVertexShader);
}

D3D9ProxyVertexShader::~D3D9ProxyVertexShader()
{
	if (m_spProxyDeviceShaderRegisters)
		m_spProxyDeviceShaderRegisters.reset();
	m_spProxyDeviceShaderRegisters = NULL;

	m_pStereoModifiedConstantsF.clear();
}

void D3D9ProxyVertexShader::MakeActive(D3D9ProxyVertexShader* previousActiveVertexShader)
{
	/* Updates the data in this for any constants that exist in both this and other. (from other) */
	
}
	
void D3D9ProxyVertexShader::UpdateAndApply(D3DProxyDevice::EyeSide side)
{
	/* Updates all dirty constants from proxy device registers and sets them on the actual device */
}