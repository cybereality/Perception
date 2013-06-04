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
	m_constantModificationRules(),
	m_defaultModifications(),
	m_shaderSpecificModifications()
{
}

ShaderModificationRepository::~ShaderModificationRepository()
{

}

bool RuleApplies(D3DXCONSTANT_DESC* constantDesc) 
{
	
}

std::map<UINT, StereoShaderConstant<float>> ShaderModificationRepository::GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader)
{
	// hash shader
	// If rules for this specific shader use those
	// else use default rules

	// For each shader constant
	//  Check if constant matches a rule (name and/or index)
	//  If it does create a stereoshaderconstant based on rule and add to map of stereoshaderconstants to return

	// return collection of stereoshaderconstants for this shader (empty collection if no modifications)

	std::vector<ConstantModificationRule*> rulesToApply;
	// add pointers to rules

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


	LPD3DXCONSTANTTABLE pConstantTable = NULL;

	BYTE* pData = NULL;
	UINT pSizeOfData;
	pActualVertexShader->GetFunction(NULL, &pSizeOfData);
			
	pData = new BYTE[pSizeOfData];
	pActualVertexShader->GetFunction(pData, &pSizeOfData);

	D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
			
	if(pConstantTable) {
		
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		D3DXCONSTANT_DESC pConstantDesc[64];

		//m_shaderDumpFile << std::endl << std::endl;
		//m_shaderDumpFile << "Shader Creator: " << pDesc.Creator << std::endl;
		//m_shaderDumpFile << "Shader Version: " << pDesc.Version << std::endl;

		for(UINT i = 0; i < pDesc.Constants; i++)
		{
			D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
			if(handle == NULL) continue;

			UINT pConstantNum = 64;
			pConstantTable->GetConstantDesc(handle, pConstantDesc, &pConstantNum);
			if (pConstantNum >= 64) {
				OutputDebugString("ShaderModificationRepository::GetModifiedConstantsF - Need larger constant description buffer");
			}

			
			for(UINT j = 0; j < pConstantNum; j++)
			{
				if ((pConstantDesc[j].RegisterSet == D3DXRS_FLOAT4) &&
					((pConstantDesc[j].Class == D3DXPC_VECTOR) || (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS))  ) {


					// iterate over chosen rules 
					auto itRules = rulesToApply.begin();
					while (itRules != rulesToApply.end()) {

						
					}

					//m_shaderDumpFile << "Constant";
					//m_shaderDumpFile << "Name: " << pConstantDesc[j].Name << std::endl;
					//m_shaderDumpFile << "Type: ";

					if (pConstantDesc[j].Class == D3DXPC_VECTOR) {
						//m_shaderDumpFile << "Vector" << std::endl;
					}
					else if (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) {
						//m_shaderDumpFile << "Row Major Matrix" << std::endl;
					}
					else if (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS) {
						//m_shaderDumpFile << "Col Major Matrix" << std::endl;
					}

					//m_shaderDumpFile << "Register Index: " << pConstantDesc[j].RegisterIndex << std::endl;
					//m_shaderDumpFile << "Register Count: " << pConstantDesc[j].RegisterCount << std::endl;
					//m_shaderDumpFile << "Number of elements in the array:" << pConstantDesc[j].Elements << std::endl << std::endl;
				}
						
			}
		}
	}

	_SAFE_RELEASE(pConstantTable);
	if (pData) delete[] pData;


	try {
		CreateStereoConstantFrom(1);
	}
	catch (std::out_of_range) {
		OutputDebugString("Unable to create StereoShaderConstant from ConstantModificationRule, could not find rule. Rule skipped.");
	}
}



 StereoShaderConstant<float> ShaderModificationRepository::CreateStereoConstantFrom(UINT modificationID/*type(matrix/vec4), rule*/)
{
	ConstantModificationRule rule;
	
	rule = m_constantModificationRules.at(modificationID);

	//return StereoShaderConstant<float>(rule.startRegIndex, D3DXMATRIX.identity, 4 /*4 for matrix, 1 for vec*/, 4, rule.constantName, /*modification*/, modificationID);
	
}