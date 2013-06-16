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
#include "ViewAdjustment.h"
#include "pugixml.hpp"

class ViewAdjustment;


class ShaderModificationRepository
{
public:
	ShaderModificationRepository(std::shared_ptr<ViewAdjustment> adjustmentMatricies); 
	virtual ~ShaderModificationRepository();

	// true if load succeeds, false otherwise.  pugi::xml_document
	bool LoadRules(std::string rulesPath); 	

	// Returns a collection of modified constants for the specified shader. (may be an empty collection if no modifications apply)
	// <StrartRegister, StereoShaderConstant<float>>
	std::map<UINT, StereoShaderConstant<float>> GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader);
	


private:

	class ConstantModificationRule 
	{
	public: 

		ConstantModificationRule () :
			m_constantName("ThisWontMatchAnything"),
			m_startRegIndex(999999), // niether will this
			m_constantType(D3DXPC_FORCE_DWORD), // or this
			m_operationToApply(0),
			m_modificationRuleID(0),
			m_allowPartialNameMatch(false),
			m_transpose(false)
		{};


		ConstantModificationRule (std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, UINT modificationRuleID, bool transpose) :
			m_constantName(constantName),
			m_startRegIndex(startRegIndex),
			m_constantType(constantType),
			m_operationToApply(operationToApply),
			m_modificationRuleID(modificationRuleID),
			m_allowPartialNameMatch(allowPartialNameMatch),
			m_transpose(transpose)
		{};

		static D3DXPARAMETER_CLASS ConstantTypeFrom(std::string type) 
		{
			if (type.compare("MatrixC")) {
				return D3DXPC_MATRIX_COLUMNS;
			}
			else if (type.compare("MatrixR")) {
				return D3DXPC_MATRIX_ROWS;
			}
			else if (type.compare("Vector")) {
				return D3DXPC_VECTOR;
			}
			else {
				OutputDebugString("Unknown or unsupported constant type: ");
				OutputDebugString(type.c_str());
				OutputDebugString("\n");

				return D3DXPC_FORCE_DWORD;
			}
		}

		// empty string is "no constant"
		std::string m_constantName;
		// If true and a constantName has been set then partial name matches are allowed (using strstr for matching)
		bool m_allowPartialNameMatch;
		// UINT_MAX is "any start reg"
		UINT m_startRegIndex;
		// type Vector/Matrix // only D3DXPC_VECTOR, D3DXPC_MATRIX_ROWS and D3DXPC_MATRIX_COLUMNS are currently handled
		D3DXPARAMETER_CLASS m_constantType;
		// Identifier (int that maps to a ShaderConstantModificationFactory::(Vector4/Matrix)ModificationTypes as appropriate given constantType) of the operation to apply
		UINT m_operationToApply;
		// Unique (within a given set of rules) id for this modification rule
		UINT m_modificationRuleID;
		// If input matrix should be transposed before modifying (and transposed back after)
		bool m_transpose;
	};


	std::shared_ptr<ViewAdjustment> m_spAdjustmentMatricies;	
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
