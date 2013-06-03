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

#include "ShaderModificationRepository.h"
#include <assert.h>

ShaderModificationRepository::ShaderModificationRepository() :
	m_overrideModifications(),
	m_defaultModifications(),
	m_constantModificationsF()
{
}

ShaderModificationRepository::~ShaderModificationRepository()
{

}

std::unordered_map<UINT, StereoShaderConstant<float>> ShaderModificationRepository::GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader)
{
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
}