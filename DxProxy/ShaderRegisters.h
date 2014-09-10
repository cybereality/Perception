/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderRegisters.h> and
Class <ShaderRegisters> :
Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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
#include "D3D9ProxyPixelShader.h"
#include "D3D9ProxyVertexShader.h"
#include "Vireio.h"

class D3D9ProxyVertexShader;
class D3D9ProxyPixelShader;

/**
* Managed shader register class.
* All shader registers stored, updated and applied to device here. 
* Register update ONLY in ApplyStereoConstantsVS().
*/
class ShaderRegisters
{
public:
	ShaderRegisters(DWORD maxPSConstantRegistersF, DWORD maxVSConstantRegistersF, IDirect3DDevice9* pActualDevice);
	virtual ~ShaderRegisters();

	/*** ShaderRegisters public methods ***/
	HRESULT WINAPI     SetVertexShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
	HRESULT WINAPI     GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
	HRESULT WINAPI     SetPixelShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
	HRESULT WINAPI     GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
	std::vector<float> GetAllVSConstantRegistersF();
	std::vector<float> GetAllPSConstantRegistersF();	
	void               SetFromStateBlockVertexShader(D3D9ProxyVertexShader* storedVShader);
	void               SetFromStateBlockPixelShader(D3D9ProxyPixelShader* storedPShader);
	void               SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedVSRegisters, std::map<UINT, D3DXVECTOR4> * storedPSRegisters);
	void               SetFromStateBlockData(std::vector<float> * storedVSRegisters, std::vector<float> * storedPSRegisters);
	bool               AnyDirtyVS(UINT start, UINT count);
	bool               AnyDirtyPS(UINT start, UINT count);
	void               ApplyAllDirty(vireio::RenderPosition currentSide);
	void               ApplyAllStereoConstants(vireio::RenderPosition currentSide);
	void               ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader);
	void               ActivePixelShaderChanged(D3D9ProxyPixelShader* pNewPixelShader);
	void               ReleaseResources();

private:
	/*** ShaderRegisters private methods ***/
	void ApplyStereoConstantsVS(vireio::RenderPosition currentSide, const bool dirtyOnly);	
	void ApplyStereoConstantsPS(vireio::RenderPosition currentSide, const bool dirtyOnly);
	void MarkAllVSStereoConstantsDirty();
	void MarkAllPSStereoConstantsDirty();

	/**
	* Currently active vertex shader.
	* <StartRegister, ModifiedConstant starting at start register>
	* const std::map<UINT, StereoShaderConstant<float>>* m_activeModifications;
	***/
	D3D9ProxyVertexShader* m_pActiveVertexShader;
	/**
	* Number of vertex shader constant registers supported by device.
	***/
	DWORD m_maxVSConstantRegistersF;
	/**
	* Vertex Shader register vector.
	* 4 floats == 1 register (defined in VECTOR_LENGTH):
	* [0][1][2][3] would be the first register. 
	* [4][5][6][7] the second, etc.
	* use RegisterIndex(x) to access first float in register
	***/
	std::vector<float> m_vsRegistersF;
	/**
	* Vertex Shader dirty registers. 
	* Nothing that this is Registers and NOT indexes of all floats that make up a register.
	***/
	std::set<UINT> m_dirtyVSRegistersF;
	/**
	* Currently active pixel shader.
	* <StartRegister, ModifiedConstant starting at start register>
	* const std::map<UINT, StereoShaderConstant<float>>* m_activeModifications;
	***/
	D3D9ProxyPixelShader* m_pActivePixelShader;
	/**
	* Number of pixel shader constant registers supported by current shader version.
	***/
	DWORD m_maxPSConstantRegistersF;
	/**
	* Pixel Shader register vector.
	* 4 floats == 1 register (defined in VECTOR_LENGTH):
	* [0][1][2][3] would be the first register. 
	* [4][5][6][7] the second, etc.
	* use RegisterIndex(x) to access first float in register
	***/
	std::vector<float> m_psRegistersF;
	/**
	* Pixel Shader dirty registers. 
	* Nothing that this is Registers and NOT indexes of all floats that make up a register.
	***/
	std::set<UINT> m_dirtyPSRegistersF;
	/**
	* Actual Direct3D Device pointer embedded. 
	***/
	IDirect3DDevice9* m_pActualDevice;
};
#endif