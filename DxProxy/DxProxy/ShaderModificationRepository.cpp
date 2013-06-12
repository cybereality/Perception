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




ShaderModificationRepository::ShaderModificationRepository(std::shared_ptr<ViewAdjustment> adjustmentMatricies) :
	m_AllModificationRules(),
	m_defaultModificationRuleIDs(),
	m_shaderSpecificModificationRuleIDs(),
	m_spAdjustmentMatricies(adjustmentMatricies)
{
	D3DXMatrixIdentity(&m_identity);


	// For testing load source settings manually
	/*m_defaultModificationRuleIDs.push_back(1);
	m_defaultModificationRuleIDs.push_back(2);
	m_defaultModificationRuleIDs.push_back(3);

	m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>(1, ConstantModificationRule("", 4, D3DXPC_MATRIX_COLUMNS, ShaderConstantModificationFactory::MatSimpleTranslateColMajorIgnoreOrtho, 1)));
	m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>(2, ConstantModificationRule("", 8, D3DXPC_MATRIX_COLUMNS, ShaderConstantModificationFactory::MatSimpleTranslateColMajorIgnoreOrtho, 2))); 
	m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>(3, ConstantModificationRule("", 51, D3DXPC_MATRIX_COLUMNS, ShaderConstantModificationFactory::MatSimpleTranslateColMajorIgnoreOrtho, 3)));*/

}

ShaderModificationRepository::~ShaderModificationRepository()
{
	m_spAdjustmentMatricies.reset();
}

bool ShaderModificationRepository::LoadRules(std::string rulesPath)
{
	m_AllModificationRules.clear();
	m_defaultModificationRuleIDs.clear();
	m_shaderSpecificModificationRuleIDs.clear();


	pugi::xml_document rulesFile;
	pugi::xml_parse_result resultProfiles = rulesFile.load_file(rulesPath.c_str());

	if(resultProfiles.status != pugi::status_ok) {
		OutputDebugString("Parsing of shader rules file failed. No rules loaded.\n");
		OutputDebugString(rulesPath.c_str());
		return false;
	}

	// load from file
	pugi::xml_node xmlShaderConfig = rulesFile.child("shaderConfig");
	if (!xmlShaderConfig) {
		OutputDebugString("'shaderConfig' node missing, malformed shader rules doc.\n"); 
		return false;
	}

	pugi::xml_node xmlRules = xmlShaderConfig.child("rules");
	if (!xmlRules) {
		OutputDebugString("No 'rules' node found, malformed shader rules doc.\n"); 
		return false;
	}
	else {
		for (pugi::xml_node rule = xmlRules.child("rule"); rule; rule = rule.next_sibling("rule")) {

			ConstantModificationRule newRule;

			newRule.m_constantName = rule.attribute("constantName").as_string();
			newRule.m_constantType = ConstantModificationRule::ConstantTypeFrom(rule.attribute("").as_string());
			newRule.m_modificationRuleID = rule.attribute("id").as_uint();
			newRule.m_operationToApply = rule.attribute("modToApply").as_uint();
			newRule.m_startRegIndex = rule.attribute("startReg").as_uint(UINT_MAX);
			newRule.m_allowPartialNameMatch = rule.attribute("partialName").as_bool(false);

			if (!(m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>(newRule.m_modificationRuleID, newRule)).second)) {
				OutputDebugString("Two rules found with the same 'id'. Only the first will be applied.\n"); 
			}
		}

		// default rules (these are optional but will most likely exist for 99% of profiles)
		pugi::xml_node defaultRules = xmlShaderConfig.child("defaultRuleIDs");
		if (defaultRules) {
			for (pugi::xml_node ruleId = defaultRules.child("ruleID"); ruleId; ruleId = ruleId.next_sibling("ruleID")) {

				m_defaultModificationRuleIDs.push_back(ruleId.attribute("id").as_uint());
			}
		}
		else {
			OutputDebugString("No default rules found, did you do this intentionally?\n");
		}

		// Shader specific rules (optional)
		for (pugi::xml_node shader = xmlShaderConfig.child("shaderSpecificRuleIDs"); shader; shader = shader.next_sibling("shaderSpecificRuleIDs"))
		{
			uint32_t hash = shader.attribute("shaderHash").as_uint(0);

			if (hash == 0) {
				OutputDebugString("Shader specific rule with invalid/no hash. Skipping rule.\n");
				continue;
			}

			std::vector<UINT> shaderRules;

			for (pugi::xml_node ruleId = shader.child("ruleID"); ruleId; ruleId = ruleId.next_sibling("ruleID")) {
				shaderRules.push_back(ruleId.attribute("id").as_uint());
			}

			if (!(m_shaderSpecificModificationRuleIDs.insert(std::pair<uint32_t, std::vector<UINT>>(hash, shaderRules)).second)) {
				OutputDebugString("Two sets of rules found with the same 'shaderHash'. Only the first will be applied.\n"); 
			}
		}
	}

	return true;
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

	// All rules are assumed to be valid. Validation of rules should be done when rules are loaded/created
	std::vector<ConstantModificationRule*> rulesToApply;
	std::map<UINT, StereoShaderConstant<float>> result;


	// Hash the shader and load modification rules
	BYTE *pData = NULL;
	UINT pSizeOfData;

	pActualVertexShader->GetFunction(NULL, &pSizeOfData);
	pData = new BYTE[pSizeOfData];
	pActualVertexShader->GetFunction(pData,&pSizeOfData);

	uint32_t hash;
	MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash);

	if (m_shaderSpecificModificationRuleIDs.count(hash) == 1) {

		// There are specific modification rules to use with this shader
		auto itRules = m_shaderSpecificModificationRuleIDs[hash].begin();
		while (itRules != m_shaderSpecificModificationRuleIDs[hash].end()) {
			rulesToApply.push_back(&(m_AllModificationRules[*itRules]));
			++itRules;
		}
	}
	else {

		// No specific rules, use general rules
		auto itRules = m_defaultModificationRuleIDs.begin();
		while (itRules != m_defaultModificationRuleIDs.end()) {
			rulesToApply.push_back(&(m_AllModificationRules[*itRules]));
			++itRules;
		}
	}



	// Load the constant descriptions for this shader and create StereoShaderConstants as the applicable rules require them.
	LPD3DXCONSTANTTABLE pConstantTable = NULL;

	D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
	
	if(pConstantTable) {
		
		D3DXCONSTANTTABLE_DESC pDesc;
		pConstantTable->GetDesc(&pDesc);

		D3DXCONSTANT_DESC pConstantDesc[64];

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
				// We are only modifying selected float vectors/matricies.
				if ((pConstantDesc[j].RegisterSet == D3DXRS_FLOAT4) &&
					((pConstantDesc[j].Class == D3DXPC_VECTOR) || (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS))  ) {

					// Check if any rules match this constant
					auto itRules = rulesToApply.begin();
					while (itRules != rulesToApply.end()) {

						// Type match
						if ((*itRules)->m_constantType == pConstantDesc[j].Class) {

							// name match required
							if ((*itRules)->m_constantName.size() > 0) {

								bool nameMatch = false;
								if ((*itRules)->m_allowPartialNameMatch) {
									nameMatch = std::strstr(pConstantDesc[j].Name, (*itRules)->m_constantName.c_str()) != NULL;

									/*if (nameMatch) {
										OutputDebugString("Match\n");
									}
									else {
										OutputDebugString("No Match\n");
									}*/
								}
								else {
									nameMatch = (*itRules)->m_constantName.compare(pConstantDesc[j].Name) == 0;

									//OutputDebugString("Full name match only\n");
								}

								if (!nameMatch) {
									// no match
									++itRules;
									continue;
								}
							}

							// register match required
							if ((*itRules)->m_startRegIndex != UINT_MAX) {
								if ((*itRules)->m_startRegIndex != pConstantDesc[j].RegisterIndex) {
									// no match
									++itRules;
									continue;
								}
							}

							// Create StereoShaderConstant<float> and add to result
							result.insert(std::pair<UINT, StereoShaderConstant<>>(pConstantDesc[j].RegisterIndex, CreateStereoConstantFrom(*itRules, pConstantDesc[j].RegisterIndex, pConstantDesc[j].RegisterCount)));

							// only the first matching rule is applied to a constant
							break;
						}

						++itRules;
					}
				}	
			}
		}
	}

	_SAFE_RELEASE(pConstantTable);
	if (pData) delete[] pData;

	return result;
}



 StereoShaderConstant<> ShaderModificationRepository::CreateStereoConstantFrom(const ConstantModificationRule* rule, UINT StartReg, UINT Count)
{
	assert ((rule->m_startRegIndex == UINT_MAX) ? (StartReg != UINT_MAX) : (rule->m_startRegIndex == StartReg));

	std::shared_ptr<ShaderConstantModification<>> modification = nullptr;
	float* pData = NULL;

	switch (rule->m_constantType)
	{
	case D3DXPC_VECTOR:
		modification = ShaderConstantModificationFactory::CreateVector4Modification(rule->m_operationToApply, m_spAdjustmentMatricies);
		pData = D3DXVECTOR4(0,0,0,0);

		return StereoShaderConstant<>(StartReg, pData, Count, modification);
		break;

	case D3DXPC_MATRIX_ROWS:
	case D3DXPC_MATRIX_COLUMNS:
		modification = ShaderConstantModificationFactory::CreateMatrixModification(rule->m_operationToApply, m_spAdjustmentMatricies);
		pData = m_identity;

		return StereoShaderConstant<>(StartReg, pData, Count, modification);
		break;

	default:

		throw 69; // unhandled type
		break;
	}

	
}


