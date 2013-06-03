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

#include "ShaderRegisters.h"
#include <assert.h>

ShaderRegisters::ShaderRegisters(DWORD maxConstantRegistersF, IDirect3DDevice9* pActualDevice) :
	m_maxConstantRegistersF(maxConstantRegistersF),
	m_registersF(maxConstantRegistersF * VECTOR_LENGTH, 0), // VECTOR_LENGTH floats per register
	m_dirtyRegistersF(),
	m_pActualDevice(pActualDevice)
{
	assert(pActualDevice != NULL);

	m_pActualDevice->AddRef();
}

	//TODO assignment and copy - add ref to device (remove ref from old device on assign)? or prevent

ShaderRegisters::~ShaderRegisters()
{
	if (m_pActualDevice)
		m_pActualDevice->Release();
}


HRESULT WINAPI ShaderRegisters::SetConstantRegistersF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxConstantRegistersF))
		return D3DERR_INVALIDCALL;

	// Set registers
	std::copy(pConstantData, pConstantData + (VECTOR_LENGTH * Vector4fCount), m_registersF.begin() + RegisterIndex(StartRegister));

	// Mark registers dirty
	for (int i = StartRegister; i < StartRegister + Vector4fCount; i++) {
		m_dirtyRegistersF.insert(i);
	}

	return D3D_OK;
}

HRESULT WINAPI ShaderRegisters::GetConstantRegistersF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxConstantRegistersF))
		return D3DERR_INVALIDCALL;

	pConstantData = &m_registersF[RegisterIndex(StartRegister)];
	
	return D3D_OK;
}

		
void ShaderRegisters::ApplyToDevice() 
{	
	auto it =  m_dirtyRegistersF.begin();

	if (it == m_dirtyRegistersF.end())
		return;


	int startReg = *it;
	while (it != m_dirtyRegistersF.end()) {
		// skip through until we reach the end of a continuous series of dirty registers
		auto itNext = std::next(it);
		if ((itNext != m_dirtyRegistersF.end()) && (*itNext == startReg + 1))
			continue;

		// set this series of registers
		m_pActualDevice->SetVertexShaderConstantF(startReg, &m_registersF[RegisterIndex(startReg)], startReg - (*it) + 1);

		// If there are more dirty registers left the next register will be the new startReg
		if (itNext != m_dirtyRegistersF.end()) {
			startReg = *itNext;
		}

		++it;
	}
}