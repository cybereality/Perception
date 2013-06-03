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

#ifndef SHADERMODIFICATIONS_H_INCLUDED
#define SHADERMODIFICATIONS_H_INCLUDED

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "StereoShaderConstant.h"
#include "GameHandler.h"
#include "ShaderRegisters.h"
#include "MurmurHash3.h"


class ShaderModifications
{
public:
	ShaderModifications();
	virtual ~ShaderModifications();

	// true if load succeeds, false otherwise
	bool Load(/*file*/);

	// 
	std::vector<StereoShaderConstant<float>> GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader);
	/*// Hash the shader and load stereo shader constants
	BYTE *pData = NULL;
	UINT pSizeOfData;

	pActualVertexShader->GetFunction(NULL, &pSizeOfData);
	pData = new BYTE[pSizeOfData];
	pActualVertexShader->GetFunction(pData,&pSizeOfData);

	Hash128Bit hash = Hash128Bit();
	MurmurHash3_x86_128(pData, pSizeOfData, VIREIO_SEED, hash.value);

	if (pGameHandler->ConstantConfigsForShaderExists(hash)) {
		m_pStereoModifiedConstants = new StereoShaderConstants(spProxyDeviceShaderRegisters, pGameHandler->ConstantConfigsForShader(hash));
	}
	else {
		m_pStereoModifiedConstants = NULL;
		//TODO dump shader constants and hash to file
	}

	delete [] pData;*/
	

private:


	// <Modification ID, StereoShaderConstant>
	std::unordered_map<std::string, StereoShaderConstant<float>> m_constantModificationsF;

	// <Modification ID>
	std::vector<std::string> m_defaultModifications;

	// <Shader hash, vector<Modification ID>>
	std::unordered_map<Hash128Bit, std::vector<std::string>> m_overrideModifications;
};

#endif
