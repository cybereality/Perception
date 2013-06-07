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

#ifndef SHADERREGISTERS_H_INCLUDED
#define SHADERREGISTERS_H_INCLUDED

#define VECTOR_LENGTH 4
#define RegisterIndex(x) (x * VECTOR_LENGTH)

#include "d3d9.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "D3D9ProxyVertexShader.h"
#include "Vireio.h"


class D3D9ProxyVertexShader;

class ShaderRegisters
{
public:
	ShaderRegisters(DWORD maxConstantRegistersF, IDirect3DDevice9* pActualDevice);
	virtual ~ShaderRegisters();

	/* Return D3DERR_INVALIDCALL if any registers out of range*/
	// clean should only be true when restoring from stateblock. It marks all registers that are set as clean instead of marking dirty
	HRESULT WINAPI SetConstantRegistersF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
	HRESULT WINAPI GetConstantRegistersF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
	std::vector<float> GetAllConstantRegistersF();

	// For restoring from stateblock. If sides during capture were mixed or the current device side doesn't match side at time of capture then updateStereoConstants should be true
	// (basically if it's possible that the actual device values for the register don't match the appropriate stereo versin then updateStereoConstants should be set to true)
	// NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we have no idea what the state of the constants might be so updating isn't optional. Fix? See stateblock
	void SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> storedRegisters);
	void SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> storedRegisters, D3D9ProxyVertexShader* storedShader);
	// The only time you restore all registers is when the whole vertex shader state is saved, in which case there will always be a vertex shader to go with the registers (it may be null)
	void SetFromStateBlockData(const std::vector<float> & storedRegisters, D3D9ProxyVertexShader* storedShader);
	



	//void MarkDirty(UINT Register);
	bool AnyDirty(UINT start, UINT count);

	/* 
		Changes the active vertex shader. 
	 */
	void ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader);

	/* 
		This will apply all dirty registers to actual device. StereoShaderConstants are updated and applied followed by unmodified
	 */
	void ApplyToDevice(vireio::RenderPosition currentSide);

	/*
		This will apply (and update) all dirty StereoShaderConstants 
		If forceApply is true then non-dirty StereoShaderConstants will be updated and applied as well
	 */
	void ApplyStereoConstants(vireio::RenderPosition currentSide, bool forceApply);

	//void UpdateStereoConstants()

	

private:

	
	// Marks the first register for each stereoconstant in the active shader dirty to make sure they are updated before being drawn
	void MarkAllStereoConstantsDirty();

	// Number of constant registers supported by device
	DWORD m_maxConstantRegistersF;

	// [0][1][2][3] would be the first register. 
	// [4][5][6][7] the second, etc.
	// use RegisterIndex(x) to access first float in register
	std::vector<float> m_registersF;

	// Dirty Registers. Noting that this is Registers and NOT indexes of all floats that make up a register
	std::set<UINT> m_dirtyRegistersF;

	// <StartRegister, ModifiedConstant starting at start register>
	//const std::map<UINT, StereoShaderConstant<float>>* m_activeModifications;
	D3D9ProxyVertexShader* m_pActiveVertexShader;

	IDirect3DDevice9* m_pActualDevice;
};



#endif