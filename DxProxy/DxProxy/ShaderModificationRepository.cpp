/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderModificationRepository.cpp> and
Class <ShaderModificationRepository> :
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

#include "ShaderModificationRepository.h"
#include <assert.h>

/**
* Constructor.
* Creates identity matrix.
* @param adjustmentMatrices Matrix calculation class pointer, used here to create the modifications.
***/
ShaderModificationRepository::ShaderModificationRepository(std::shared_ptr<ViewAdjustment> adjustmentMatrices) :
	m_AllModificationRules(),
	m_defaultModificationRuleIDs(),
	m_shaderSpecificModificationRuleIDs(),
	m_spAdjustmentMatrices(adjustmentMatrices)
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

/**
* Destructor.
* Resets adjustment matrices.
***/
ShaderModificationRepository::~ShaderModificationRepository()
{
	m_spAdjustmentMatrices.reset();
}

/**
* Loads shader modification rules.
* True if load succeeds, false otherwise.  
* (pugi::xml_document)
* @param rulesPath Rules path as defined in game configuration.
***/
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

			static ConstantModificationRule newRule;

			newRule.m_constantName = rule.attribute("constantName").as_string();
			newRule.m_constantType = ConstantModificationRule::ConstantTypeFrom(rule.attribute("constantType").as_string());
			newRule.m_modificationRuleID = rule.attribute("id").as_uint();
			newRule.m_operationToApply = rule.attribute("modToApply").as_uint();
			newRule.m_startRegIndex = rule.attribute("startReg").as_uint(UINT_MAX);
			newRule.m_allowPartialNameMatch = rule.attribute("partialName").as_bool(false);
			newRule.m_transpose = rule.attribute("transpose").as_bool(false);

			if (!(m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>((UINT)int(newRule.m_modificationRuleID), (ShaderModificationRepository::ConstantModificationRule)newRule)).second)) {
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

			bool squishViewport = shader.attribute("squishViewport").as_bool(false);

			if (squishViewport)
				m_shaderViewportSquashIDs.push_back(hash);

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

/**
* Saves current shader modification rules.
* @param rulesPath Rules path as defined in game configuration.
***/
bool ShaderModificationRepository::SaveRules(std::string rulesPath)
{
	// create an empty document
	pugi::xml_document rulesFile;
	if (!rulesFile.load("<shaderConfig><rules></rules><defaultRuleIDs></defaultRuleIDs></shaderConfig>"))
	{
		OutputDebugString("Load basic xml document failed, pugi xml error.");
		return false;	
	}

	// add a custom declaration node
	pugi::xml_node decl = rulesFile.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";

	// get main node
	pugi::xml_node xmlShaderConfig = rulesFile.child("shaderConfig");
	if (!xmlShaderConfig) {
		OutputDebugString("'shaderConfig' node missing, pugi xml error.\n"); 
		return false;
	}

	// get rules node
	pugi::xml_node xmlRules = xmlShaderConfig.child("rules");
	if (!xmlRules) {
		OutputDebugString("No 'rules' node found, pugi xml error.\n"); 
		return false;
	}
	else {
		auto itModificationRules = m_AllModificationRules.begin();
		while (itModificationRules != m_AllModificationRules.end())
		{
			pugi::xml_node xmlRule = xmlRules.append_child("rule");

			// set attributes
			xmlRule.append_attribute("constantName") = itModificationRules->second.m_constantName.c_str();
			xmlRule.append_attribute("partialName") = itModificationRules->second.m_allowPartialNameMatch;
			xmlRule.append_attribute("id") = itModificationRules->first;
			xmlRule.append_attribute("constantType") = ConstantModificationRule::ConstantStringFrom(itModificationRules->second.m_constantType).c_str();
			xmlRule.append_attribute("modToApply") = itModificationRules->second.m_operationToApply;
			xmlRule.append_attribute("startReg") = itModificationRules->second.m_startRegIndex;
			xmlRule.append_attribute("transpose") = itModificationRules->second.m_transpose;

			++itModificationRules;
		}

		// default rules (these are optional but will most likely exist for 99% of profiles)
		pugi::xml_node defaultRules = xmlShaderConfig.child("defaultRuleIDs");
		if (defaultRules) {
			auto itDefaultRules = m_defaultModificationRuleIDs.begin();
			while (itDefaultRules != m_defaultModificationRuleIDs.end())
			{
				pugi::xml_node ruleId = defaultRules.append_child("ruleID");

				// set id attribute
				ruleId.append_attribute("id") = *itDefaultRules;

				++itDefaultRules;
			}
		}
		else {
			OutputDebugString("No default rules found, pugi xml error.\n");
		}

		// Shader specific rules (optional)
		auto itSpecificRules = m_shaderSpecificModificationRuleIDs.begin();
		while (itSpecificRules != m_shaderSpecificModificationRuleIDs.end())
		{
			// create shader node
			pugi::xml_node shader = xmlShaderConfig.append_child("shaderSpecificRuleIDs");

			// append hash attribute
			uint32_t hash = itSpecificRules->first;
			shader.append_attribute("shaderHash") = hash;

			// viewport squish for that shader ?
			if(std::find(m_shaderViewportSquashIDs.begin(), m_shaderViewportSquashIDs.end(), hash)!=m_shaderViewportSquashIDs.end()){
				// set id attribute
				shader.append_attribute("squishViewport") = true;
			}

			// save ids
			std::vector<UINT> shaderRules = itSpecificRules->second;
			auto itRules = shaderRules.begin();
			while (itRules != shaderRules.end())
			{
				pugi::xml_node ruleID = shader.append_child("ruleID");
				ruleID.append_attribute("id") = *itRules;
				++itRules;
			}
			++itSpecificRules;
		}
	}

	// return saving xml file
	return rulesFile.save_file(rulesPath.c_str());
}

/**
* Adds a default shader rule specified by parameters if that rule is not already present.
* @return True if rule was added, false if rule already present.
***/
bool ShaderModificationRepository::AddRule(std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, UINT modificationRuleID, bool transpose)
{
	UINT rulePresent = false;
	auto itModificationRules = m_AllModificationRules.begin();
	while (itModificationRules != m_AllModificationRules.end())
	{
		if ((constantName.compare(itModificationRules->second.m_constantName) == 0) &&
			(allowPartialNameMatch == itModificationRules->second.m_allowPartialNameMatch) &&
			(startRegIndex == itModificationRules->second.m_startRegIndex) &&
			(constantType == itModificationRules->second.m_constantType) &&
			(operationToApply == itModificationRules->second.m_operationToApply) &&
			(transpose == itModificationRules->second.m_transpose))
			rulePresent = true;

		++itModificationRules;
	}

	// add if rule is not present
	if (!rulePresent)
	{
		OutputDebugString("rule not present...add");
		m_defaultModificationRuleIDs.push_back(modificationRuleID);
		static ConstantModificationRule newRule;

		newRule.m_constantName = std::string(constantName);
		newRule.m_constantType = constantType;
		newRule.m_modificationRuleID = modificationRuleID;
		newRule.m_operationToApply = operationToApply;
		newRule.m_startRegIndex = startRegIndex;
		newRule.m_allowPartialNameMatch = allowPartialNameMatch;
		newRule.m_transpose = transpose;

		if (!(m_AllModificationRules.insert(std::make_pair<UINT, ConstantModificationRule>((UINT)int(newRule.m_modificationRuleID), (ShaderModificationRepository::ConstantModificationRule)newRule)).second)) {
			OutputDebugString("Two rules found with the same 'id'. Only the first will be applied.\n"); 
		}
	}

	return (!rulePresent);
}

/**
* Adds a default shader rule specified by parameters if that rule is not already present.
* @return True if rule was present an modified, false if rule not present.
***/
bool ShaderModificationRepository::ModifyRule(std::string constantName, UINT operationToApply, bool transpose)
{
	bool rulePresent = false;
	auto itModificationRules = m_AllModificationRules.begin();
	while (itModificationRules != m_AllModificationRules.end())
	{
		if (constantName.compare(itModificationRules->second.m_constantName) == 0) 
		{
			itModificationRules->second.m_operationToApply = operationToApply;
			itModificationRules->second.m_transpose = transpose;
			rulePresent = true;
		}
		++itModificationRules;
	}

	return rulePresent;
}

/**
* Deletes all rules that share this constant name.
* @return True if rule was deleted, false if rule not present.
***/
bool ShaderModificationRepository::DeleteRule(std::string constantName)
{
	bool rulePresent = false;
	auto itModificationRules = m_AllModificationRules.begin();
	while (itModificationRules != m_AllModificationRules.end())
	{
		if (constantName.compare(itModificationRules->second.m_constantName) == 0) 
		{
			m_AllModificationRules.erase(itModificationRules);
			itModificationRules = m_AllModificationRules.begin();
			rulePresent = true;
		}
		else
			++itModificationRules;
	}

	return rulePresent;
}

/**
* Returns a collection of modified constants for the specified shader. 
* (may be an empty collection if no modifications apply)
* <StrartRegister, StereoShaderConstant<float>>
*
* Hash the shader and load modification rules:
* If rules for this specific shader use those else use default rules.
*
* For each shader constant:
* Check if constant matches a rule (name and/or index). If it does create a stereoshaderconstant 
* based on rule and add to map of stereoshaderconstants to return.
*
* @param pActualPixelShader The actual (not wrapped) pixel shader.
* @return Collection of stereoshaderconstants for this shader (empty collection if no modifications).
***/
std::map<UINT, StereoShaderConstant<float>> ShaderModificationRepository::GetModifiedConstantsF(IDirect3DPixelShader9* pActualPixelShader)
{
	// All rules are assumed to be valid. Validation of rules should be done when rules are loaded/created
	std::vector<ConstantModificationRule*> rulesToApply;
	std::map<UINT, StereoShaderConstant<float>> result;

	// Hash the shader and load modification rules
	BYTE *pData = NULL;
	UINT pSizeOfData;

	pActualPixelShader->GetFunction(NULL, &pSizeOfData);
	pData = new BYTE[pSizeOfData];
	pActualPixelShader->GetFunction(pData,&pSizeOfData);

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
				if (pConstantDesc[j].RegisterSet != D3DXRS_FLOAT4)
					continue;

				if ( ((pConstantDesc[j].Class == D3DXPC_VECTOR) && (pConstantDesc[j].RegisterCount == 1))
					|| (((pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS)) && (pConstantDesc[j].RegisterCount == 4)) ) {
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

#ifdef _DEBUG
								// output shader constant + index 
								switch(pConstantDesc[j].Class)
								{
								case D3DXPC_VECTOR:
									OutputDebugString("D3DXPC_VECTOR");
									break;
								case D3DXPC_MATRIX_ROWS:
									OutputDebugString("D3DXPC_MATRIX_ROWS");
									break;
								case D3DXPC_MATRIX_COLUMNS:
									OutputDebugString("D3DXPC_MATRIX_COLUMNS");
									break;
								}
								char buf[32];
								sprintf_s(buf,"Register Index: %d", pConstantDesc[j].RegisterIndex);
								OutputDebugString(buf);
#endif

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

/**
* Returns a collection of modified constants for the specified shader. 
* (may be an empty collection if no modifications apply)
* <StrartRegister, StereoShaderConstant<float>>
*
* Hash the shader and load modification rules:
* If rules for this specific shader use those else use default rules.
*
* For each shader constant:
* Check if constant matches a rule (name and/or index). If it does create a stereoshaderconstant 
* based on rule and add to map of stereoshaderconstants to return.
*
* @param pActualVertexShader The actual (not wrapped) vertex shader.
* @return Collection of stereoshaderconstants for this shader (empty collection if no modifications).
***/
std::map<UINT, StereoShaderConstant<float>> ShaderModificationRepository::GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader)
{
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
				if (pConstantDesc[j].RegisterSet != D3DXRS_FLOAT4)
					continue;

				if ( ((pConstantDesc[j].Class == D3DXPC_VECTOR) && (pConstantDesc[j].RegisterCount == 1))
					|| (((pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS)) && (pConstantDesc[j].RegisterCount == 4)) ) {
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

#ifdef _DEBUG
								// output shader constant + index 
								switch(pConstantDesc[j].Class)
								{
								case D3DXPC_VECTOR:
									OutputDebugString("D3DXPC_VECTOR");
									break;
								case D3DXPC_MATRIX_ROWS:
									OutputDebugString("D3DXPC_MATRIX_ROWS");
									break;
								case D3DXPC_MATRIX_COLUMNS:
									OutputDebugString("D3DXPC_MATRIX_COLUMNS");
									break;
								}
								char buf[32];
								sprintf_s(buf,"Register Index: %d", pConstantDesc[j].RegisterIndex);
								OutputDebugString(buf);
#endif

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

/**
* Returns true if viewport should be squished for this shader.
***/
bool ShaderModificationRepository::SquishViewportForShader(IDirect3DVertexShader9* pActualVertexShader)
{
	// Hash the shader
	BYTE *pData = NULL;
	UINT pSizeOfData;

	pActualVertexShader->GetFunction(NULL, &pSizeOfData);
	pData = new BYTE[pSizeOfData];
	pActualVertexShader->GetFunction(pData,&pSizeOfData);

	uint32_t hash;
	MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash);

	delete[] pData;

	// find hash
	auto i = std::find(m_shaderViewportSquashIDs.begin(), m_shaderViewportSquashIDs.end(), hash);

	// found
	if (i != m_shaderViewportSquashIDs.end()) {
		return true;
	} 

	return false;
}

/**
* Returns a unique identifier for a new shader rule.
***/
UINT ShaderModificationRepository::GetUniqueRuleID()
{
	UINT result = 1;
	auto itModificationRules = m_AllModificationRules.begin();
	while (itModificationRules != m_AllModificationRules.end())
	{
		if (itModificationRules->first >= result) result = itModificationRules->first+1;
		++itModificationRules;
	}

	return result;
}

/**
* True if the constant name has a rule applied, returns string name of the rule.
* (does not ask for a start register or so)
* @param constantName [in] The name of the constant.
* @param constantRule [out] The name of the rule to be returned.
***/
bool ShaderModificationRepository::ConstantHasRule(std::string constantName, std::string& constantRule, UINT& operation, bool& isTransposed)
{
	auto iRules = m_AllModificationRules.begin();
	while (iRules !=m_AllModificationRules.end())
	{
		if (iRules->second.m_constantName.compare(constantName) == 0)
		{
			// put this to factory ?
			// get the string name of the rule
			switch (iRules->second.m_constantType)
			{
			case D3DXPC_VECTOR:
				if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::Vec4DoNothing)
					constantRule = "Vec4DoNothing";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::Vec4SimpleTranslate)
					constantRule = "Vec4SimpleTranslate";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::Vec4EyeShiftUnity)
					constantRule = "Vec4EyeShiftUnity";
				break;

			case D3DXPC_MATRIX_ROWS:
			case D3DXPC_MATRIX_COLUMNS:
				if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatDoNothing)
					constantRule = "MatDoNothing";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatSimpleTranslate)
					constantRule = "MatSimpleTranslate";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatOrthographicSquash)
					constantRule = "MatOrthographicSquash";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatHudSlide)
					constantRule = "MatHudSlide";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatGuiSquash)
					constantRule = "MatGuiSquash";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatSurfaceRefractionTransform)
					constantRule = "MatSurfaceRefractionTransform";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatGatheredOrthographicSquash)
					constantRule = "MatGatheredOrthographicSquash";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatOrthographicSquashShifted)
					constantRule = "MatOrthographicSquashShifted";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatOrthographicSquashHud)
					constantRule = "MatOrthographicSquashHud";				
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatConvergenceOffset)
					constantRule = "MatConvergenceOffset";				
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatSimpleTranslateIgnoreOrtho)
					constantRule = "MatSimpleTranslateIgnoreOrtho";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatRollOnly)
					constantRule = "MatRollOnly";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatRollOnlyNegative)
					constantRule = "MatRollOnlyNegative";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatRollOnlyHalf)
					constantRule = "MatRollOnlyHalf";
				else if (iRules->second.m_operationToApply == ShaderConstantModificationFactory::MatNoRoll)
					constantRule = "MatNoRoll";
				break;

			default:
				throw 69; // unhandled type
				break;
			}
			isTransposed = iRules->second.m_transpose;
			operation = iRules->second.m_operationToApply;
			return true;
		}

		++iRules;
	}
	return false;
}

/**
* Creates and returns StereoShaderConstant by specified rule.
* StartReg is needed for registers that were identified by rule using name for matching but not register.
* @param rule [in] Shader constant modification rule.
* @param StartReg [in] Shader constant start register.
* @param Count [in] Shader constant size.
* @return The stereo shader constant containing the specified modification rule.
***/
StereoShaderConstant<> ShaderModificationRepository::CreateStereoConstantFrom(const ConstantModificationRule* rule, UINT StartReg, UINT Count)
{
	assert ((rule->m_startRegIndex == UINT_MAX) ? (StartReg != UINT_MAX) : (rule->m_startRegIndex == StartReg));

	std::shared_ptr<ShaderConstantModification<>> modification = nullptr;
	float* pData = NULL;

	switch (rule->m_constantType)
	{
	case D3DXPC_VECTOR:
		modification = ShaderConstantModificationFactory::CreateVector4Modification(rule->m_operationToApply, m_spAdjustmentMatrices);
		pData = D3DXVECTOR4(0,0,0,0);

		return StereoShaderConstant<>(StartReg, pData, Count, modification);
		break;

	case D3DXPC_MATRIX_ROWS:
	case D3DXPC_MATRIX_COLUMNS:
		modification = ShaderConstantModificationFactory::CreateMatrixModification(rule->m_operationToApply, m_spAdjustmentMatrices, rule->m_transpose);
		pData = m_identity;

		return StereoShaderConstant<>(StartReg, pData, Count, modification);
		break;

	default:
		throw 69; // unhandled type
		break;
	}
}