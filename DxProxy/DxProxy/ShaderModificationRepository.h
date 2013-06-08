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

#ifndef ShaderModificationRepository_H_INCLUDED
#define ShaderModificationRepository_H_INCLUDED

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "StereoShaderConstant.h"
#include "GameHandler.h"
#include "ShaderRegisters.h"
#include "MurmurHash3.h"
#include "Limits.h"
#include "ShaderConstantModification.h"
#include "ShaderConstantModificationFactory.h"
#include "ViewAdjustmentMatricies.h"


class ShaderModificationRepository
{
public:
	ShaderModificationRepository(std::string rulesFile, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies); //xml_node/doc?
	virtual ~ShaderModificationRepository();

	//// true if load succeeds, false otherwise
	//bool Load(/*file*/);

	// Returns a collection of modified constants for the specified shader. (may be an empty collection if no modifications apply)
	// <StrartRegister, StereoShaderConstant<float>>
	std::map<UINT, StereoShaderConstant<float>> GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader);
	


private:

	


	class ConstantModificationRule 
	{
	public: 
		// empty string is "no constant"
		std::string constantName;
		// UINT_MAX is "any start reg"
		UINT startRegIndex;
		// type Vector/Matrix // only D3DXPC_VECTOR, D3DXPC_MATRIX_ROWS and D3DXPC_MATRIX_COLUMNS are currently handled
		D3DXPARAMETER_CLASS constantType;
		// Identifier (int that maps to a ShaderConstantModificationFactory::(Vector4/Matrix)ModificationTypes as appropriate given constantType) of the operation to apply
		UINT operationToApply;
		// Unique (within a given set of rules) id for this modification rule
		UINT modificationRuleID;
		//type - always float atm
	};


	std::shared_ptr<ViewAdjustmentMatricies> m_spAdjustmentMatricies;	
	D3DXMATRIX m_identity;

	// StartReg is needed for registers that were identified by rule using name for matching but not register
	StereoShaderConstant<float> CreateStereoConstantFrom(const ConstantModificationRule* rule, UINT StartReg, UINT Count);

	// <Modification Rule ID, ModificationRule>
	std::unordered_map<UINT, ConstantModificationRule> m_AllModificationRules;

	// <Modification Rule ID>
	std::vector<UINT> m_defaultModificationRuleIDs;

	// <Shader hash, vector<Modification Rule ID>>
	std::unordered_map<uint32_t, std::vector<UINT>> m_shaderSpecificModificationRuleIDs;
};

#endif
