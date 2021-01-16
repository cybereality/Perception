/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifierDataStructures.h> and
Class <VireioMatrixModifierDataStructures> :
Copyright (C) 2015 Denis Reischl

Parts of included classes directly derive from Vireio source code originally
authored by Chris Drain (v1.1.x 2013).

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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
#include<d3dx9.h>
#include<stdio.h>
#include<vector>
#include<string>
#include<array>

#include"..\..\..\Include\Vireio_GUIDs.h"

#define VIREIO_MAX_VARIABLE_NAME_LENGTH      64  /**< We restrict variable names to 64 characters. ***/
#define VIREIO_CONSTANT_RULES_NOT_ADDRESSED - 1  /**< No shader rules addressed for this shader. ***/
#define VIREIO_CONSTANT_RULES_NOT_AVAILABLE - 2  /**< No shader rules available for this shader. ***/

#define VIREIO_SEED	                               12345                     /**< Do not change this !! ***/
#define VECTOR_LENGTH 4                                                      /**< One shader register has 4 float values. ***/
#define MAX_DX9_CONSTANT_REGISTERS                   224                     /**< Maximum shader registers for DX9 ***/
#define RegisterIndex(x) (x * VECTOR_LENGTH)                                 /**< Simple helper to access shader register. ***/

// Universal Coded Character Set (UCS) shader stream constants
constexpr uint32_t OPCODE_BYTEORDERMARK_LO = 0x0000FFFE;
constexpr uint32_t OPCODE_HEADER_UTF32_02 = 0xFFFE0200;
constexpr uint32_t OPCODE_HEADER_UTF32_03 = 0xFFFE0300;
constexpr uint32_t OPCODE_FOOTER_UTF32_FLEXIBLE = 0x0000FFFF;
constexpr uint32_t OPCODE_MASK_LO = 0x0000FFFF;
constexpr uint32_t OPCODE_MASK_HI = 0xFFFF0000;
constexpr uint32_t OPCODE_MASK_HI_FSS_UTF = 0x7FFF0000;
constexpr uint32_t OPCODE_ID_CTAB = 0x42415443;

/// <summary>
/// Known Shader Byte Code Chunk types
/// </summary>
enum struct ShaderChunkType : unsigned
{
	CTAB,                     /*< Constant Table. contains D3D9 shader constants, if provided */
	Unknown
};

/// <summary>
/// CTAB chunk shader byte code data
/// </summary>
struct CTAB_Data
{
	uint32_t uSize;
	uint32_t uCreator;
	uint32_t uVersion;
	uint32_t uConstantsNumber;
	uint32_t uConstantInfo;
	uint32_t uFlags;
	uint32_t uTarget;
};

/// <summary>
/// CTAB chunk sub data
/// </summary>
struct CTAB_ConstantInfo
{
	uint32_t uName;
	uint16_t uRegisterSet;
	uint16_t uRegisterIndex;
	uint16_t uRegisterCount;
	uint16_t uReserved;
	uint32_t uTypeInfo;
	uint32_t uDefaultValue;
};

/// <summary>
/// CTAB chunk sub data
/// </summary>
struct CTAB_ConstantType
{
	uint16_t uClass;
	uint16_t uType;
	uint16_t uRows;
	uint16_t uColumns;
	uint16_t uElements;
	uint16_t uStructMembers;
	uint32_t uStructMemberInfo;
};

/// <summary>
/// Structure only used for shader specific rules.
/// Contains hash code and rule index.
/// </summary>
struct Vireio_Hash_Rule_Index
{
	unsigned __int32 unHash;
	unsigned __int32 unRuleIndex;
};

/// <summary>
/// Simple enumeration of supported Shaders.
/// </summary>
enum class Vireio_Supported_Shaders : int
{
	VertexShader,
	PixelShader,
};

/// <summary>
/// Simple copy of the D3D9 constant description
/// basically equals D3DXCONSTANT_DESC
/// </summary>
typedef struct VIREIO_D3D9_CONSTANT_DESC
{
	std::string         acName;         /**< Constant name ***/
	D3DXREGISTER_SET    eRegisterSet;   /**< Register set ***/
	UINT                uRegisterIndex; /**< Register index ***/
	UINT                uRegisterCount; /**< Number of registers occupied ***/
	D3DXPARAMETER_CLASS eClass;         /**< Class ***/
	D3DXPARAMETER_TYPE  eType;          /**< Component type ***/
	UINT                uRows;          /**< Number of rows ***/
	UINT                uColumns;       /**< Number of columns ***/
	UINT                uElements;      /**< Number of array elements ***/
	UINT                uStructMembers; /**< Number of structure member sub-parameters ***/
	std::vector<float>  afDefaultValue; /**< Default value ***/
} SAFE_D3DXCONSTANT_DESC;

/// <summary>
/// Structure used to write data to the shader function.
/// </summary>
struct CreatorMod
{
	union
	{
		struct
		{
			char acModHeader[4];
			char acHash[8];
			char acIndex[4];
		};
		char acCreator[16];
	};
};

/// <summary>
/// Shader-specific constant rule index.
/// </summary>
struct Vireio_Shader_Constant_Rule_Index
{
	UINT m_dwShaderHash;
	UINT m_dwIndex;
};

/// <summary>
/// Constant rule index.
/// For efficiency in compiled profiles.
/// Stores register index and shader rule index.
/// </summary>
struct Vireio_Constant_Rule_Index
{
	UINT m_dwConstantRuleRegister;
	UINT m_dwIndex;
};

/// <summary>
/// Buffer specific rules index.
/// Stores the index of m_aasConstantBufferRuleIndices and
/// the rule update counter.
/// </summary>
struct Vireio_Buffer_Rules_Index
{
	INT m_nRulesIndex;
	UINT m_dwUpdateCounter;
};

/// <summary>
/// Constant rule index DX9.
/// Stores register indexand shader rule index.
/// For efficiency in DX9 we also store the modified constant data here.
/// </summary>
struct Vireio_Constant_Rule_Index_DX9
{
	UINT dwConstantRuleRegister;
	UINT dwIndex;
	UINT dwConstantRuleRegisterCount;

	union
	{
		unsigned char acConstantDataLeft[4 * 4 * sizeof(float)]; /**< Constant data left in bytes. (max. sizeof(MATRIX 4*4)) **/
		float afConstantDataLeft[4 * 4];                         /**< Constant data left in float. (max. sizeof(MATRIX 4*4)) **/
		UINT32 aunConstantDataLeft[4 * 4];                       /**< Constant data left in unsigned int. (max. sizeof(MATRIX 4*4)) **/
		D3DMATRIX asConstantDataLeft;
	};
	union
	{
		unsigned char acConstantDataRight[4 * 4 * sizeof(float)]; /**< Constant data right in bytes. (max. sizeof(MATRIX 4*4)) **/
		float afConstantDataRight[4 * 4];                         /**< Constant data right in float. (max. sizeof(MATRIX 4*4)) **/
		UINT32 aunConstantDataRight[4 * 4];                       /**< Constant data right in unsigned int. (max. sizeof(MATRIX 4*4)) **/
		D3DMATRIX asConstantDataRight;
	};
};

/// <summary>
/// D3D11 Shader variable (or constant) description.
/// </summary>
struct Vireio_D3D11_Shader_Variable
{
	CHAR                    szName[VIREIO_MAX_VARIABLE_NAME_LENGTH];       /**< Name of the variable ***/
	UINT                    dwStartOffset;                                 /**< Offset in constant buffer's backing store ***/
	UINT                    dwSize;                                        /**< Size of variable (in bytes) ***/
	BYTE                    pcDefaultValue[sizeof(D3DMATRIX)];             /**< Raw default value data... maximum size of a matrix ***/
};

/// <summary>
/// D3D11 Shader constant buffer description.
/// </summary>
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

/// <summary>
/// D3D11 Shader constant buffer description for unaccounted buffers.
/// Unaccounted constant buffers are buffers which are not reflected by the DX11 shader reflection class.
/// </summary>
struct Vireio_D3D11_Constant_Buffer_Unaccounted
{
	UINT dwRegister;                 /**< The index of the shader constant register. ***/
	UINT dwSize;                     /**< The number of float constants in that buffer. ***/
	enum D3D11_Constant_Buffer_AccessPattern
	{
		immediateIndexed,
		dynamicIndexed,
	}    eAccessPattern;             /**< The way that the buffer will be accessed by shader code. ***/
	INT  nConstantRulesIndex;        /**< The index of the constant rules used. (-1 = not addressed, -2 = no rules) ***/
};

/// <summary>
/// Vireio basic shader description.
/// </summary>
struct Vireio_Shader
{
	uint32_t    uVersion;   /**< Shader version ***/
	std::string acCreator;  /**< Creator string (unmodified) ***/
	uint32_t    uHash;      /**< This shaders hash code. ***/
};

/// <summary>
/// Vireio D3D9 shader description.
/// Structure containing all necessary data for the IDirect3D(X)Shader9 interfaces.
/// For DX9 the constant indices also contain the left/right modified constant data.
/// The currently modified shader constant data contains 4 floats == 1 register (defined 
/// in VECTOR_LENGTH). Provided by the last SetShaderConstantF update.
/// </summary>
struct Vireio_D3D9_Shader : public Vireio_Shader
{
	std::vector<SAFE_D3DXCONSTANT_DESC>                           asConstantDescriptions;       /**< Shader constant descriptions. ***/
	std::vector<Vireio_Constant_Rule_Index_DX9>                   asConstantRuleIndices;        /**< The indices of the shader rules assigned to that shader hash. ***/
	std::array<float, MAX_DX9_CONSTANT_REGISTERS * VECTOR_LENGTH> afRegisterBuffer;             /**< Shader register buffer. ***/
	std::array<uint32_t, MAX_DX9_CONSTANT_REGISTERS>              aunRegisterModificationIndex; /**< Index of modification for the specified register. ***/
};

/// <summary>
/// Vireio D3D11 shader description.
/// Structure containing all necessary data for the ID3D11(X)Shader interfaces.
/// </summary>
struct Vireio_D3D11_Shader : public Vireio_Shader
{
	UINT                                                  dwConstantBuffers;                            /**< Number of constant buffers ***/
	UINT                                                  dwBoundResources;                             /**< Number of bound resources ***/
	UINT                                                  dwInputParameters;                            /**< Number of parameters in the input signature ***/
	UINT                                                  dwOutputParameters;                           /**< Number of parameters in the output signature ***/
	std::vector<Vireio_D3D11_Constant_Buffer>             asBuffers;                                    /**< The Vireio shader constant buffers descriptions (max D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ***/
	std::vector<Vireio_D3D11_Constant_Buffer_Unaccounted> asBuffersUnaccounted;                         /**< The Vireio shader constant buffers descriptions (max D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ***/
};

/// <summary>
/// Vireio shader private data field.
/// Short data field directly set to the shader interface.
/// Contains only shader hash and shader description index.
/// </summary>
struct Vireio_Shader_Private_Data
{
	UINT dwHash;   /**< The shader hash code. ***/
	UINT dwIndex;  /**< The shader description index. ***/
};

/// <summary>
/// Debug grab options.
/// Enumeration must match the strings added to spin control : m_dwDebugSpin (ID).
/// </summary>
enum struct Debug_Grab_Options
{
	Debug_ConstantFloat4,
	Debug_ConstantFloat8,
	Debug_ConstantFloat16,
	Debug_ConstantFloat32,
	Debug_ConstantFloat64,
};