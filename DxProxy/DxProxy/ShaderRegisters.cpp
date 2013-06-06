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
	m_pActualDevice(pActualDevice),
	m_pActiveVertexShader(NULL)
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
	if ((StartRegister <= m_maxConstantRegistersF) || ((StartRegister + Vector4fCount) <= m_maxConstantRegistersF))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	std::copy(pConstantData, pConstantData + (VECTOR_LENGTH * Vector4fCount), m_registersF.begin() + RegisterIndex(StartRegister));

	// Mark registers dirty
	for (UINT i = StartRegister; i < StartRegister + Vector4fCount; i++) {
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



bool ShaderRegisters::AnyDirty(UINT start, UINT count)
{
	auto it = m_dirtyRegistersF.lower_bound(start);

	if (it == m_dirtyRegistersF.end())
		return false;

	if (*it >= start + count) {
		return false;
	}

	return true;
}
		
void ShaderRegisters::ApplyToDevice(vireio::RenderPosition currentSide) 
{	
	
	if (m_dirtyRegistersF.size() == 0)
		return;

	
	// Updates all dirty stereo constants and sets them on the actual device
	auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
	while (itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end()) {

		// if any of the registers that make up this constant are dirty
		if (AnyDirty(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count())) {
			itStereoConstant->second.Update(&m_registersF[RegisterIndex(itStereoConstant->second.StartRegister())]);

			m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());

			// These registers are no longer dirty
			for (UINT i = itStereoConstant->second.StartRegister(); i < itStereoConstant->second.StartRegister() + itStereoConstant->second.Count(); i++)
				m_dirtyRegistersF.erase(i);
		}

		++itStereoConstant;
	}


	// Apply all remaining dirty registers (should just be non-stereo that remain dirty) to device
	auto it = m_dirtyRegistersF.begin();
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

	m_dirtyRegistersF.clear();
}

void ShaderRegisters::ForceApplyStereoConstants(vireio::RenderPosition currentSide)
{
	auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
	while (itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end()) {

		// if any of the registers that make up this constant are dirty update before setting
		if (AnyDirty(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count())) { // Should we do this or make this method just switch sides without checking for updated data?
			itStereoConstant->second.Update(&m_registersF[RegisterIndex(itStereoConstant->second.StartRegister())]);

			// These registers are no longer dirty
			for (UINT i = itStereoConstant->second.StartRegister(); i < itStereoConstant->second.StartRegister() + itStereoConstant->second.Count(); i++)
				m_dirtyRegistersF.erase(i);
		}

		// Apply this constant to device
		m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());

		++itStereoConstant;
	}
}


void ShaderRegisters::ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader)
{
	if (m_pActiveVertexShader == pNewVertexShader)
		return;

	if (pNewVertexShader) {

		std::map<UINT, StereoShaderConstant<float>>* pNewShaderModConstants = pNewVertexShader->ModifiedConstants();

		std::map<UINT, StereoShaderConstant<float>>* pOldShaderModConstants = NULL;
		if (m_pActiveVertexShader)
			pOldShaderModConstants = m_pActiveVertexShader->ModifiedConstants();

		// Update the data in new shader constants with data from matching constants from last shader.)
		auto itNewConstants = pNewShaderModConstants->begin();
		while (itNewConstants != pNewShaderModConstants->end()) {

			bool mightBeDirty = true;

			if (pOldShaderModConstants) {
				// No idea if this is saving any time or if it would be better to just mark all the registers dirty and re-apply the constants on first draw
				if (m_pActiveVertexShader->ModifiedConstants()->count(itNewConstants->first) == 1) {
					if (pOldShaderModConstants->at(itNewConstants->first).SameConstantAs(itNewConstants->second)) {
						(*pNewShaderModConstants).at(itNewConstants->first) = (*pOldShaderModConstants).at(itNewConstants->first);
						mightBeDirty = false;
					}
				}
			}

			// If there isn't a corresponding old modification then this modified constant will need updating
			if (mightBeDirty) {
				m_dirtyRegistersF.insert(itNewConstants->first);
			}
		}
	}
	
	m_pActiveVertexShader = pNewVertexShader;
}
	