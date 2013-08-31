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
#include "d3dx9.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "D3D9ProxyVertexShader.h"
#include "Vireio.h"

class D3D9ProxyVertexShader;

/**
* Managed shader register class.
* All shader registers stored, updated and applied to device here. 
* Register update ONLY in ApplyStereoConstants().
*/
class ShaderRegisters
{
public:
	ShaderRegisters(DWORD maxConstantRegistersF, IDirect3DDevice9* pActualDevice);
	virtual ~ShaderRegisters();

	/*** ShaderRegisters public methods ***/
	HRESULT WINAPI     SetConstantRegistersF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
	HRESULT WINAPI     GetConstantRegistersF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
	std::vector<float> GetAllConstantRegistersF();	
	void               SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedRegisters);
	void               SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedRegisters, D3D9ProxyVertexShader* storedShader);
	void               SetFromStateBlockData(std::vector<float> * storedRegisters, D3D9ProxyVertexShader* storedShader);
	bool               AnyDirty(UINT start, UINT count);
	void               ApplyAllDirty(vireio::RenderPosition currentSide);
	void               ApplyAllStereoConstants(vireio::RenderPosition currentSide);
	void               ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader);
	void               ReleaseResources();
	
private:
	/*** ShaderRegisters private methods ***/
	void ApplyStereoConstants(vireio::RenderPosition currentSide, const bool dirtyOnly);	
	void MarkAllStereoConstantsDirty();

	/**
	* Number of constant registers supported by device.
	***/
	DWORD m_maxConstantRegistersF;
	/**
	* Register vector.
	* 4 floats == 1 register (defined in VECTOR_LENGTH):
	* [0][1][2][3] would be the first register. 
	* [4][5][6][7] the second, etc.
	* use RegisterIndex(x) to access first float in register
	***/
	std::vector<float> m_registersF;
	/**
	* Dirty Registers. 
	* Nothing that this is Registers and NOT indexes of all floats that make up a register.
	***/
	std::set<UINT> m_dirtyRegistersF;
	/**
	* Currently active vertex shader.
	* <StartRegister, ModifiedConstant starting at start register>
	* const std::map<UINT, StereoShaderConstant<float>>* m_activeModifications;
	***/
	D3D9ProxyVertexShader* m_pActiveVertexShader;
	/**
	* Actual Direct3D Device pointer embedded. 
	***/
	IDirect3DDevice9* m_pActualDevice;
};
#endif