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

/**
* Constructor, creates register vector.
* @param maxConstantRegistersF Maximum number of constant registers.
* @param pActualDevice Pointer to actual (not wrapped) D3D device.
***/
ShaderRegisters::ShaderRegisters(DWORD maxConstantRegistersF, IDirect3DDevice9* pActualDevice) :
	m_maxConstantRegistersF(maxConstantRegistersF),
	m_registersF(maxConstantRegistersF * VECTOR_LENGTH, 0), // VECTOR_LENGTH floats per register
	m_dirtyRegistersF(),
	m_pActualDevice(pActualDevice),
	m_pActiveVertexShader(NULL)
{
	assert(pActualDevice != NULL);

	//TODO assignment and copy - add ref to device (remove ref from old device on assign)? or prevent
	m_pActualDevice->AddRef();
}

/**
* Destructor, releases device and vertex shader.
***/
ShaderRegisters::~ShaderRegisters()
{
	if (m_pActualDevice)
		m_pActualDevice->Release();

	if (m_pActiveVertexShader)
		m_pActiveVertexShader->Release();
}

/**
* Sets proxy registers and marks them dirty.
* Register should only have clean set to true when restoring from stateblock. It marks all registers
* that are set as clean instead of marking dirty.
*
* Return D3DERR_INVALIDCALL if any registers out of internal range.
* @param StartRegister Look at D3DProxyDevice::SetVertexShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::SetVertexShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::SetVertexShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::SetConstantRegistersF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxConstantRegistersF))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	std::copy(pConstantData, pConstantData + (VECTOR_LENGTH * Vector4fCount), m_registersF.begin() + RegisterIndex(StartRegister));

	// Mark registers dirty
	for (UINT i = StartRegister; i < StartRegister + Vector4fCount; i++) {
		m_dirtyRegistersF.insert(i);
	}

	return D3D_OK;
}

/**
* Gets shader constant register.
* @param StartRegister Look at D3DProxyDevice::GetVertexShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::GetVertexShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::GetVertexShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::GetConstantRegistersF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxConstantRegistersF))
		return D3DERR_INVALIDCALL;

	pConstantData = &m_registersF[RegisterIndex(StartRegister)];
	
	return D3D_OK;
}

/**
* Returns the register vector containing all constant registers.
***/
std::vector<float> ShaderRegisters::GetAllConstantRegistersF()
{
	return m_registersF;
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedRegisters Pointer to stored register map.
***/
void ShaderRegisters::SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedRegisters)
{
	auto itNewRegs = storedRegisters->begin();
	while (itNewRegs != storedRegisters->end()) {

		if ((RegisterIndex(itNewRegs->first) + VECTOR_LENGTH) >= m_registersF.size())
			throw std::out_of_range("Register from stateblock is out of range, implosion imminent");
				
		//std::copy(static_cast<float*>(itNewRegs->second), static_cast<float*>(itNewRegs->second) + VECTOR_LENGTH,  &m_registersF[RegisterIndex(itNewRegs->first)]);
		// copy produces warnings, this does not.
		int regStartIndexInVector = RegisterIndex(itNewRegs->first);
		m_registersF[regStartIndexInVector	  ]	= itNewRegs->second.x;
		m_registersF[regStartIndexInVector + 1] = itNewRegs->second.y;
		m_registersF[regStartIndexInVector + 2] = itNewRegs->second.z;
		m_registersF[regStartIndexInVector + 3] = itNewRegs->second.w;
		
		// register is clean (now matches device state - unless it's stereo in which case it might not, that is handled at the end)
		m_dirtyRegistersF.erase(itNewRegs->first);
		++itNewRegs;
	}

	MarkAllStereoConstantsDirty();
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedRegisters Pointer to stored register map.
* @param storedShader Pointer to stored vertex shader.
***/
void ShaderRegisters::SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedRegisters, D3D9ProxyVertexShader* storedShader)
{
	_SAFE_RELEASE(m_pActiveVertexShader);
	m_pActiveVertexShader = storedShader;
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->AddRef();

	SetFromStateBlockData(storedRegisters);
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedRegisters Pointer to stored register vector.
* @param storedShader Pointer to stored vertex shader.
***/
void ShaderRegisters::SetFromStateBlockData(std::vector<float> * storedRegisters, D3D9ProxyVertexShader* storedShader)
{
	// Full register capture should always match the size of the existing register set as size is fixed and register was captured from this
	assert(storedRegisters->size() == m_registersF.size());

	std::copy(storedRegisters->begin(), storedRegisters->end(), m_registersF.begin());

	// Data should match registers that are already on device (unless it's stereo in which case it might not, that is handled next)
	m_dirtyRegistersF.clear();

	_SAFE_RELEASE(m_pActiveVertexShader);
	m_pActiveVertexShader = storedShader;
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->AddRef();

	MarkAllStereoConstantsDirty();
}

/**
* Returns true if any dirty register found in the specified range.
* @param start Start register.
* @param count Register count.
***/
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
	
/**
* This will apply all dirty registers to actual device. 
* StereoShaderConstants are updated and applied followed by unmodified.
* Note that stereo constants will only be applied if the underlying register has changed. To apply a 
* specific side whether dirty or not use ApplyStereoConstants with forceApply true.
* @param currentSide Left or Right side.
***/
void ShaderRegisters::ApplyAllDirty(vireio::RenderPosition currentSide) 
{	
	
	if (m_dirtyRegistersF.size() == 0)
		return;

	if (m_pActiveVertexShader) {
		ApplyStereoConstants(currentSide, true);
	}

	// Apply all remaining dirty registers (should just be non-stereo that remain dirty) to device
	auto it = m_dirtyRegistersF.begin();

	if (it != m_dirtyRegistersF.end()) {

		int startReg = *it; // can't dererefence this if it might be end
		int lastReg = startReg;

		while (it != m_dirtyRegistersF.end()) {

		
			auto itNext = std::next(it);
			if ((itNext != m_dirtyRegistersF.end()) && (*itNext == lastReg + 1)) {
				// skip through until we reach the end of a continuous series of dirty registers
				lastReg = *itNext;
			}
			else {
				// set this series of registers
				m_pActualDevice->SetVertexShaderConstantF(startReg, &m_registersF[RegisterIndex(startReg)], lastReg - startReg + 1);

				// If there are more dirty registers left the next register will be the new startReg
				if (itNext != m_dirtyRegistersF.end()) {
					startReg = *itNext;
					lastReg = startReg;
				}
			}

			++it;
		}
	}

	m_dirtyRegistersF.clear();
}

/**
* This will apply all StereoShaderConstants to the device (updating dirty ones before applying them).
* @param currentSide Left or Right side.
***/
void ShaderRegisters::ApplyAllStereoConstants(vireio::RenderPosition currentSide)
{
	ApplyStereoConstants(currentSide, false);
}

/**
* Changes the active vertex shader.
* Updates the data in new shader constants with data from matching constants from last shader.
* @param pNewVertexShader Pointer to the new vertex shader.
***/
void ShaderRegisters::ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader)
{
	if (m_pActiveVertexShader == pNewVertexShader)
		return;

	if (pNewVertexShader) {

		std::map<UINT, StereoShaderConstant<float>>* pNewShaderModConstants = pNewVertexShader->ModifiedConstants();

		std::map<UINT, StereoShaderConstant<float>>* pOldShaderModConstants = NULL;
		if (m_pActiveVertexShader)
			pOldShaderModConstants = m_pActiveVertexShader->ModifiedConstants();

		// Update the data in new shader constants with data from matching constants from last shader.
		auto itNewConstants = pNewShaderModConstants->begin();
		while (itNewConstants != pNewShaderModConstants->end()) {

			bool mightBeDirty = true;

			if (pOldShaderModConstants) {
				// No idea if this is saving any time or if it would be better to just mark all the registers dirty 
				// and re-apply the constants on first draw
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

			++itNewConstants;
		}


	}
	
	_SAFE_RELEASE(m_pActiveVertexShader);
	m_pActiveVertexShader = pNewVertexShader;
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->AddRef();
}

/**
* Releases any d3d resources (does not include device, that is only release on destruction).
***/
void ShaderRegisters::ReleaseResources()
{
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->Release();

	m_pActiveVertexShader = NULL;

	// TODO Do the constant registers get cleared on device reset? Current guess is no. Needs testing or a reference finding that says what happens.
}

/**
* This will apply all StereoShaderConstants to the device (updating dirty ones before applying them).
* @param currentSide Left or Right side.
* @param dirtyOnly Set to true if only dirty registers are to be applied.
***/
void ShaderRegisters::ApplyStereoConstants(vireio::RenderPosition currentSide, const bool dirtyOnly)
{
	if (!m_pActiveVertexShader)
		return;

	auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
	while (itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end()) {

		// if any of the registers that make up this constant are dirty update before setting
		if ( AnyDirty(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count())) { // Should we do this or make this method just switch sides without checking for updated data? 

			itStereoConstant->second.Update(&m_registersF[RegisterIndex(itStereoConstant->second.StartRegister())]);

			if (dirtyOnly) {
				// Apply this dirty constant to device
				m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
			}

			// These registers are no longer dirty
			for (UINT i = itStereoConstant->second.StartRegister(); i < itStereoConstant->second.StartRegister() + itStereoConstant->second.Count(); i++)
				m_dirtyRegistersF.erase(i);
		}

		if (!dirtyOnly) {
			// Apply this constant to device
			m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
		}

		++itStereoConstant;
	}
}

/**
* Marks the first register for each stereoconstant in the active shader dirty.
* (to make sure they are updated before being drawn)
***/
void ShaderRegisters::MarkAllStereoConstantsDirty()
{
	if (m_pActiveVertexShader) {
		// Mark all StereoShaderConstants dirty so they are updated before drawing
		auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
		while (itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end()) {
			
			m_dirtyRegistersF.insert(itStereoConstant->first);
			++itStereoConstant;
		}
	}
}
