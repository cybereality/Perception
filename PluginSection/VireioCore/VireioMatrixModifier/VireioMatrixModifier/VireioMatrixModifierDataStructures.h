/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifierDataStructures.h> and
Class <VireioMatrixModifierDataStructures> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from Vireio source code originally
authored by Chris Drain (v1.1.x 2013).
(original class <ConstantModificationRule> and the shader rule technique)

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include<d3d11_1.h>
#include<d3d11.h>
#include<d3d10_1.h>
#include<d3d10.h>
#include<d3d9.h>
#include<stdio.h>
#include<vector>

#define VIREIO_MAX_VARIABLE_NAME_LENGTH      64  /**< We restrict variable names to 64 characters. ***/
#define VIREIO_CONSTANT_RULES_NOT_ADDRESSED - 1  /**< No shader rules addressed for this shader. ***/
#define VIREIO_CONSTANT_RULES_NOT_AVAILABLE - 2  /**< No shader rules available for this shader. ***/

/**
* Get hash code helper.
***/
inline DWORD GetHashCode(BYTE* pcData, DWORD dwSize)
{
	DWORD h = 0;

	// create hash
	for (DWORD i = 0; i < dwSize; i++)
	{
		h = 31 * h + pcData[i];
	}

	return h;
}

/**
* Constant modification rule (v4+).
* Stores all data necessary for a
* Original class <ConstantModificationRule> 2013 by Chris Drain.
***/
struct Vireio_Constant_Modification_Rule
{
	/**
	* Constructor.
	* Creates empty shader constant modification rule.
	***/
	Vireio_Constant_Modification_Rule() :
	m_szConstantName("ThisWontMatchAnything"),
	m_dwBufferIndex(999999),
	m_dwBufferSize(0),
	m_dwStartRegIndex(0),
	m_bUseName(false),
	m_bUsePartialNameMatch(false),
	m_bUseBufferIndex(false),
	m_bUseBufferSize(false),
	m_bUseStartRegIndex(false),
	m_dwRegisterCount(0),
	m_dwOperationToApply(0),
	m_dwModificationRuleID(0),
	m_bTranspose(false)
	{};

	/**
	* Constructor.
	* Creates shader constant modification rule by specified data.
	* @param szConstantName Constant string name.
	* @param dwBufferIndex The index of the constant buffer (as set with ->?SSetConstantBuffers()).
	* @param dwBufferSize The size of the constant buffer.
	* @param dwStartRegIndex Shader start register of that rule.
	* @param bUseName True to use the constant name to identify the constant.
	* @param bUsePartialNameMatch True to use partial string name matches to identify the constant.
	* @param bUseBufferIndex True to use the buffer index to identify the constant.
	* @param bUseBufferSize True to use the buffer size to identify the constant.
	* @param bUseStartRegIndex True to use the constant start register to identify the constant.
	* @param dwRegisterCount Constant size (in shader registers = vectors = 4*sizeof(float)).
	* @param dwOperationToApply Modification identifier.
	* @param dwModificationRuleID Unique (within a given set of rules) id for this modification rule.
	* @param bTranspose True if input matrix should be bTransposed before modifying (and bTransposed back after).
	***/
	Vireio_Constant_Modification_Rule(std::string szConstantName, UINT dwBufferIndex, UINT dwBufferSize, UINT dwStartRegIndex, bool bUseName, bool bUsePartialNameMatch, bool bUseBufferIndex, bool bUseBufferSize, bool bUseStartRegIndex,
		UINT dwRegisterCount, UINT dwOperationToApply, UINT dwModificationRuleID, bool bTranspose) :
		m_szConstantName(szConstantName),
		m_dwBufferIndex(dwBufferIndex),
		m_dwBufferSize(dwBufferSize),
		m_dwStartRegIndex(dwStartRegIndex),
		m_bUseName(bUseName),
		m_bUsePartialNameMatch(bUsePartialNameMatch),
		m_bUseBufferIndex(bUseBufferIndex),
		m_bUseBufferSize(bUseBufferSize),
		m_bUseStartRegIndex(bUseStartRegIndex),
		m_dwRegisterCount(dwRegisterCount),
		m_dwOperationToApply(dwOperationToApply),
		m_dwModificationRuleID(dwModificationRuleID),
		m_bTranspose(bTranspose)
	{};

	/**
	* Constant string name.
	* Empty string is "no constant".
	*/
	std::string m_szConstantName;
	/**
	* If the shader has no constant table, we need the constant buffer index to identify the constant.
	***/
	UINT m_dwBufferIndex;
	/**
	* If the shader has no constant table, we need the constant buffer size to identify the constant.
	***/
	UINT m_dwBufferSize;
	/**
	* Shader start register of that rule.
	***/
	UINT m_dwStartRegIndex;
	/**
	* If true the full constant string name will be used to identify the constant.
	***/
	bool m_bUseName;
	/**
	* If true the partial constant string name will be used to identify the constant.
	***/
	bool m_bUsePartialNameMatch;
	/**
	* If true the buffer index will be used to identify the constant.
	***/
	bool m_bUseBufferIndex;
	/**
	* If true the buffer size will be used to identify the constant.
	***/
	bool m_bUseBufferSize;
	/**
	* If true the start register index will be used to identify the constant.
	***/
	bool m_bUseStartRegIndex;
	/**
	* Register count, 4 for Matrix, 1 for Vector, 2 for half-size matrices.
	* For v4+ rules this count is used instead of D3DXPARAMETER_CLASS (used in v1.1->v3.x of the Driver).
	* For this value there is no option (bool) to Use/NotUse to identify since this value is ALW
	***/
	UINT m_dwRegisterCount;
	/**
	* Modification identifier.
	* Identifier (int that maps to a m_dwRegisterCount type indexed class) of the operation to apply.
	***/
	UINT m_dwOperationToApply;
	/**
	* Unique (within a given set of rules) id for this modification rule.
	***/
	UINT m_dwModificationRuleID;
	/**
	* True if input matrix should be transposed before modifying (and transposed back after).
	***/
	bool m_bTranspose;
};

/**
* Shader-specific constant rule index.
***/
struct Vireio_Shader_Constant_Rule_Index
{
	UINT m_dwShaderHash;
	UINT m_dwIndex;
};

/**
* Constant rule index.
* For efficiency in compiled profiles.
* Stores register index and shader rule index.
***/
struct Vireio_Constant_Rule_Index
{
	UINT m_dwConstantRuleRegister;
	UINT m_dwIndex;
};

/**
* D3D11 Shader variable (or constant) description.
***/
struct Vireio_D3D11_Shader_Variable
{
	CHAR                    szName[VIREIO_MAX_VARIABLE_NAME_LENGTH];       /**< Name of the variable ***/
	UINT                    dwStartOffset;                                 /**< Offset in constant buffer's backing store ***/
	UINT                    dwSize;                                        /**< Size of variable (in bytes) ***/
	BYTE                    pcDefaultValue[sizeof(D3DMATRIX)];             /**< Raw default value data... maximum size of a matrix ***/
};

/**
* D3D11 Shader constant buffer description.
***/
struct Vireio_D3D11_Constant_Buffer
{
	CHAR                                      szName[VIREIO_MAX_VARIABLE_NAME_LENGTH];      /**< Name of the variable ***/
	D3D_CBUFFER_TYPE                          eType;			                            /**< Indicates type of buffer content ***/
	UINT                                      dwVariables;                                  /**< Number of member variables ***/
	UINT                                      dwSize;                                       /**< Size of CB (in bytes) ***/
	UINT                                      dwFlags;                                      /**< Buffer description flags ***/
	std::vector<Vireio_D3D11_Shader_Variable> asVariables;                                  /**< The Vireio shader variables descriptions ***/
	INT                                       nConstantRulesIndex;                          /**< The index of the constant rules used. (-1 = not addressed, -2 = no rules) ***/
};

/**
* D3D11 Shader constant buffer description for unaccounted buffers.
* Unaccounted constant buffers are buffers which are not reflected by the DX11 shader reflection class.
***/
struct Vireio_D3D11_Constant_Buffer_Unaccounted
{
	UINT                                      dwRegister;                 /**< The index of the shader constant register. ***/
	UINT                                      dwSize;                     /**< The number of float constants in that buffer. ***/
	enum D3D11_Constant_Buffer_AccessPattern
	{
		immediateIndexed,
		dynamicIndexed,
	}                                         eAccessPattern;             /**< The way that the buffer will be accessed by shader code. ***/
	INT                                       nConstantRulesIndex;        /**< The index of the constant rules used. (-1 = not addressed, -2 = no rules) ***/
};

/**
* Vireio D3D11 shader description.
* Structure containing all necessary data for the ID3D11VertexShader interface.
***/
struct Vireio_D3D11_Shader
{
	UINT                                                  dwVersion;                                    /**< Shader version ***/
	CHAR                                                  szCreator[VIREIO_MAX_VARIABLE_NAME_LENGTH];   /**< Creator string ***/
	UINT                                                  dwConstantBuffers;                            /**< Number of constant buffers ***/
	UINT                                                  dwBoundResources;                             /**< Number of bound resources ***/
	UINT                                                  dwInputParameters;                            /**< Number of parameters in the input signature ***/
	UINT                                                  dwOutputParameters;                           /**< Number of parameters in the output signature ***/
	std::vector<Vireio_D3D11_Constant_Buffer>             asBuffers;                                    /**< The Vireio shader constant buffers descriptions (max D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ***/
	std::vector<Vireio_D3D11_Constant_Buffer_Unaccounted> asBuffersUnaccounted;                         /**< The Vireio shader constant buffers descriptions (max D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ***/
	UINT                                                  dwHashCode;                                   /**< This shaders hash code. ***/
};

/**
* Vireio shader private data field.
* Short data field directly set to the shader interface.
* Contains only shader hash and shader description index.
***/
struct Vireio_Shader_Private_Data
{
	UINT dwHash;   /**< The shader hash code. ***/
	UINT dwIndex;  /**< The shader description index. ***/
};

///**
//* Vireio constant buffer private data field.
//* Short data field directly set to the buffer interface.
//* Contains only shader hash, shader description index
//* and the index of the buffer set in active buffers.
//***/
//struct Vireio_Constant_Buffer_Private_Data
//{
//	UINT dwHash;        /**< The shader hash code. ***/
//	UINT dwIndex;       /**< The shader description index. ***/
//	UINT dwIndexBuffer; /**< The index of the buffer set. ***/
//};