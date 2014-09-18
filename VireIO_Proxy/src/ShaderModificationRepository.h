/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderModificationRepository.h> and
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

#ifndef SHADERMODIFICATIONREPOSITORY_H_INCLUDED
#define SHADERMODIFICATIONREPOSITORY_H_INCLUDED

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

class ViewAdjustment;

/**
* Creates shader modifications defined in shader_rules game configuration files.
*/
class ShaderModificationRepository
{
public:
	ShaderModificationRepository(std::shared_ptr<ViewAdjustment> adjustmentMatrices); 
	virtual ~ShaderModificationRepository();

	/*** ShaderModificationRepository public methods ***/
	bool                                        LoadRules(std::string rulesPath);
	bool                                        SaveRules(std::string rulesPath);
	bool                                        AddRule(std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, UINT modificationRuleID, bool transpose);
	bool                                        ModifyRule(std::string constantName, UINT operationToApply, bool transpose);
	bool                                        DeleteRule(std::string constantName);
	std::map<UINT, StereoShaderConstant<float>> GetModifiedConstantsF(IDirect3DPixelShader9* pActualPixelShader);
	std::map<UINT, StereoShaderConstant<float>> GetModifiedConstantsF(IDirect3DVertexShader9* pActualVertexShader);
	bool										SquishViewportForShader(IDirect3DVertexShader9* pActualVertexShader);
	UINT                                        GetUniqueRuleID();
	bool                                        ConstantHasRule(std::string constantName, std::string& constantRule, UINT& operation, bool& isTransposed);

private:
	/**
	* Constant modification rule, as defined in shader_rules game configuration files.
	***/
	class ConstantModificationRule 
	{
	public: 
		/**
		* Constructor.
		* Creates empty shader constant modification rule.
		***/
		ConstantModificationRule () :
			m_constantName("ThisWontMatchAnything"),
			m_startRegIndex(999999), // niether will this
			m_constantType(D3DXPC_FORCE_DWORD), // or this
			m_operationToApply(0),
			m_modificationRuleID(0),
			m_allowPartialNameMatch(false),
			m_transpose(false)
		{};
		/**
		* Constructor.
		* Creates shader constant modification rule by specified data.
		* @param constantName Constant string name.
		* @param allowPartialNameMatch True to allow partial string name matches.
		* @param startRegIndex Shader start register of that rule.
		* @param constantType Constant type as defined in "d3dx9shader.h".
		* @param operationToApply Modification identifier.
		* @param modificationRuleID Unique (within a given set of rules) id for this modification rule.
		* @param transpose True if input matrix should be transposed before modifying (and transposed back after).
		***/
		ConstantModificationRule (std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, UINT modificationRuleID, bool transpose) :
			m_constantName(constantName),
			m_startRegIndex(startRegIndex),
			m_constantType(constantType),
			m_operationToApply(operationToApply),
			m_modificationRuleID(modificationRuleID),
			m_allowPartialNameMatch(allowPartialNameMatch),
			m_transpose(transpose)
		{};

		/**
		* Returns D3DXPARAMETER_CLASS constant type from string.
		* @param type [in] The constant type as string.
		***/
		static D3DXPARAMETER_CLASS ConstantTypeFrom(std::string type) 
		{
			if (type.compare("MatrixC") == 0) {
				return D3DXPC_MATRIX_COLUMNS;
			}
			else if (type.compare("MatrixR") == 0) {
				return D3DXPC_MATRIX_ROWS;
			}
			else if (type.compare("Vector") == 0) {
				return D3DXPC_VECTOR;
			}
			else {
				OutputDebugStringA("Unknown or unsupported constant type: ");
				OutputDebugStringA(type.c_str());
				OutputDebugStringA("\n");

				return D3DXPC_FORCE_DWORD;
			}
		}

		/**
		* Returns string from D3DXPARAMETER_CLASS constant type.
		* @param paramClass [in] The constant type.
		***/
		static std::string ConstantStringFrom(D3DXPARAMETER_CLASS paramClass)
		{
			switch(paramClass)
			{
			case D3DXPARAMETER_CLASS::D3DXPC_MATRIX_COLUMNS:
				return "MatrixC";
				break;
			case D3DXPARAMETER_CLASS::D3DXPC_MATRIX_ROWS:
				return "MatrixR";
				break;
			case D3DXPARAMETER_CLASS::D3DXPC_VECTOR:
				return "Vector";
				break;
			}
			return "Unknown";
		}

		/**
		* Constant string name.
		* Empty string is "no constant".
		*/
		std::string m_constantName;	               
		/**
		* If true and a constantName has been set then partial name matches are allowed (using strstr for matching).
		***/
		bool m_allowPartialNameMatch;
		/**
		* Shader start register of that rule.
		* (UINT_MAX is "any start reg")
		***/
		UINT m_startRegIndex;
		/**
		* Constant type as defined in "d3dx9shader.h".
		* (type Vector/Matrix)
		* (only D3DXPC_VECTOR, D3DXPC_MATRIX_ROWS and D3DXPC_MATRIX_COLUMNS are currently handled)
		***/
		D3DXPARAMETER_CLASS m_constantType;
		/**
		* Modification identifier.
		* Identifier (int that maps to a ShaderConstantModificationFactory::(Vector4/Matrix)ModificationTypes as 
		* appropriate given constantType) of the operation to apply.
		***/
		UINT m_operationToApply;
		/**
		* Unique (within a given set of rules) id for this modification rule.
		***/
		UINT m_modificationRuleID;
		/**
		* True if input matrix should be transposed before modifying (and transposed back after).
		***/
		bool m_transpose;
	};

	/*** ShaderModificationRepository private methods ***/
	StereoShaderConstant<float> CreateStereoConstantFrom(const ConstantModificationRule* rule, UINT StartReg, UINT Count);

	/**
	* Matrix calculation class pointer, used here to create the modifications.
	***/
	std::shared_ptr<ViewAdjustment> m_spAdjustmentMatrices;	
	/**
	* Identity matrix.
	***/
	D3DXMATRIX m_identity;
	/**
	* Map of all modification rules.
	* <Modification Rule ID, ModificationRule>
	***/
	std::unordered_map<UINT, ConstantModificationRule> m_AllModificationRules;
	/**
	* Vector of default modification rule identifiers.
	* <Modification Rule ID>
	***/
	std::vector<UINT> m_defaultModificationRuleIDs;
	/**
	* Vector of shader specific modification rule identifiers.
	* <Shader hash, vector<Modification Rule ID>>
	***/
	std::unordered_map<uint32_t, std::vector<UINT>> m_shaderSpecificModificationRuleIDs;
	/**
	* Vector of shader hash identifiers of shaders to squash the viewport.
	***/
	std::vector<uint32_t> m_shaderViewportSquashIDs;
};
#endif
