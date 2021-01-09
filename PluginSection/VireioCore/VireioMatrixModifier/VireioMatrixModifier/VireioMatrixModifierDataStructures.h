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

#include"..\..\..\Include\Vireio_GUIDs.h"

#define VIREIO_MAX_VARIABLE_NAME_LENGTH      64  /**< We restrict variable names to 64 characters. ***/
#define VIREIO_CONSTANT_RULES_NOT_ADDRESSED - 1  /**< No shader rules addressed for this shader. ***/
#define VIREIO_CONSTANT_RULES_NOT_AVAILABLE - 2  /**< No shader rules available for this shader. ***/

// Universal Coded Character Set (UCS) shader stream constants
constexpr uint32_t OPCODE_BYTEORDERMARK_LO = 0x0000FFFE;
constexpr uint32_t OPCODE_HEADER_UTF32_FLEXIBLE = 0xFFFE0200;
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

#pragma region inline helper
/// <summary>
/// Convertes unsigned to hex string.
/// </summary>
/// <param name="acString">Output string</param>
/// <param name="uValue">Input value</param>
/// <param name="uDigits">Number of digits</param>
inline void HexString(char* acString, uint32_t uValue, uint8_t uDigits)
{
	static const char* acDigits = "0123456789ABCDEF";
	for (size_t i = 0, j = (uDigits - 1) * 4; i < uDigits; ++i, j -= 4)
		acString[i] = acDigits[(uValue >> j) & 0x0f];
}

/// <summary>
/// Simple hash code helper.
/// </summary>
inline uint32_t GetHashCode(BYTE* pcData, int32_t nLen, uint32_t uSeed)
{
	uint32_t uH = uSeed;

	// create hash
	for (int32_t i = 0; i < nLen; i++)
	{
		uH = 31 * uH + pcData[i];
	}

	return uH;
}

/// <summary>
/// Parses D3D9 shader byte code, provides Hash code, Shader constants.
/// Code base by gamedev user sebi707 by WINE reference (wine-1.6\dlls\d3dx9_36\shader.c)
/// </summary>
/// <param name="acFunc">Shader function byte code</param>
/// <param name="uSizeOfData">Provides the size of the function</param>
/// <param name="uCreatorIx">Provides the index of the creator string</param>
/// <param name="asConstants">Provides the shader constants</param>
/// <param name="uHash">Provides the hash code</param>
/// <param name="uSeed">Hash code seed</param>
/// <returns>S_OK if succeeded, E_ABORT if parsing aborted</returns>
inline HRESULT ParseShaderFunction(uint32_t* acFunc, uint32_t& uSizeOfData, uint32_t& uCreatorIx, std::vector<VIREIO_D3D9_CONSTANT_DESC>& asConstants, uint32_t& uHash, uint32_t uSeed)
{
	uint32_t* acPtr = acFunc;

	// parse header
	if (*acPtr != OPCODE_HEADER_UTF32_FLEXIBLE)
	{
		DEBUG_HEX(*acPtr);
		DEBUG_HEX(OPCODE_HEADER_UTF32_FLEXIBLE);
		return E_ABORT;
	}
	else
		++acPtr;

	// loop until footer
	while (*acPtr != OPCODE_FOOTER_UTF32_FLEXIBLE)
	{
		// get byte order mark
		if ((*acPtr & OPCODE_MASK_LO) == OPCODE_BYTEORDERMARK_LO)
		{
			// check for chunk type
			ShaderChunkType eChunkType = ShaderChunkType::Unknown;
			uint32_t uChunkSize = (*acPtr & OPCODE_MASK_HI_FSS_UTF) >> 16;
			switch (*(acPtr + 1))
			{
			case OPCODE_ID_CTAB:
				eChunkType = ShaderChunkType::CTAB;
				break;
			default:
				acPtr += uChunkSize;
				break;
			}

			switch (eChunkType)
			{
			case ShaderChunkType::CTAB:
			{
				// get CTAB data
				const uint8_t* acCTAB = reinterpret_cast<const uint8_t*>(acPtr + 2);
				size_t uCTABSize = (uChunkSize - 1) * 4;

				const CTAB_Data* sCTable = reinterpret_cast<const CTAB_Data*>(acCTAB);
				if (uCTABSize < sizeof(*sCTable) || sCTable->uSize != sizeof(*sCTable))
					return E_ABORT;

				// get creator name
				std::string acCreator = (const char*)acCTAB + sCTable->uCreator;

				// only provide creator string if size min 16
				if (acCreator.length() >= 16)
					uCreatorIx = sCTable->uCreator + (uint32_t)acCTAB - (uint32_t)acFunc;

				// get constants
				asConstants.reserve(sCTable->uConstantsNumber);
				const CTAB_ConstantInfo* psInfo = reinterpret_cast<const CTAB_ConstantInfo*>(acCTAB + sCTable->uConstantInfo);
				for (uint32_t i = 0; i < sCTable->uConstantsNumber; ++i)
				{
					const CTAB_ConstantType* psType = reinterpret_cast<const CTAB_ConstantType*>(acCTAB + psInfo[i].uTypeInfo);

					// fill struct
					VIREIO_D3D9_CONSTANT_DESC sDesc = {};
					sDesc.acName = (const char*)acCTAB + psInfo[i].uName;
					sDesc.eRegisterSet = static_cast<D3DXREGISTER_SET>(psInfo[i].uRegisterSet);
					sDesc.uRegisterIndex = psInfo[i].uRegisterIndex;
					sDesc.uRegisterCount = psInfo[i].uRegisterCount;
					sDesc.eClass = (D3DXPARAMETER_CLASS)psType->uClass;
					sDesc.uRows = psType->uRows;
					sDesc.uColumns = psType->uColumns;
					sDesc.uElements = psType->uElements;
					sDesc.uStructMembers = psType->uStructMembers;
					asConstants.push_back(sDesc);
				}
			}
			break;
			case ShaderChunkType::Unknown:
				break;
			default:
				break;
			}

		}
		// inc by byte-size
		uint8_t* acP = (uint8_t*)acPtr;	acP++;
		acPtr = (uint32_t*)acP;
	}

	// get the size and hash code
	uSizeOfData = 4 + (uint32_t)acPtr - (uint32_t)acFunc;
	uHash = GetHashCode((BYTE*)acFunc, (int32_t)uSizeOfData, VIREIO_SEED);

	return S_OK;
}

/// <summary>
/// Provides the index of the creator string within D3D9 shader function byte code.
/// </summary>
/// <param name="acFunc">Shader function byte code</param>
/// <returns>Zero if no index found, otherwise the creator string index</returns>
inline uint32_t GetCreatorIndex(uint32_t* acFunc)
{
	uint32_t* acPtr = acFunc;

	// parse header
	if (*acPtr != OPCODE_HEADER_UTF32_FLEXIBLE)
	{
		DEBUG_HEX(*acPtr);
		DEBUG_HEX(OPCODE_HEADER_UTF32_FLEXIBLE);
		return E_ABORT;
	}
	else
		++acPtr;

	// loop until footer
	while (*acPtr != OPCODE_FOOTER_UTF32_FLEXIBLE)
	{
		// get byte order mark
		if ((*acPtr & OPCODE_MASK_LO) == OPCODE_BYTEORDERMARK_LO)
		{
			// check for chunk type
			ShaderChunkType eChunkType = ShaderChunkType::Unknown;
			uint32_t uChunkSize = (*acPtr & OPCODE_MASK_HI_FSS_UTF) >> 16;
			switch (*(acPtr + 1))
			{
			case OPCODE_ID_CTAB:
				eChunkType = ShaderChunkType::CTAB;
				break;
			default:
				acFunc += uChunkSize;
				break;
			}

			switch (eChunkType)
			{
			case ShaderChunkType::CTAB:
			{
				// get CTAB data
				const uint8_t* acCTAB = reinterpret_cast<const uint8_t*>(acPtr + 2);
				size_t uCTABSize = (uChunkSize - 1) * 4;

				const CTAB_Data* sCTable = reinterpret_cast<const CTAB_Data*>(acCTAB);
				if (uCTABSize < sizeof(*sCTable) || sCTable->uSize != sizeof(*sCTable))
					return E_ABORT;

				// get creator name
				std::string acCreator = (const char*)acCTAB + sCTable->uCreator;

				// only provide creator string if size min 16
				if (acCreator.length() >= 16)
					return sCTable->uCreator + (uint32_t)acCTAB - (uint32_t)acFunc;
			}
			break;
			case ShaderChunkType::Unknown:
				break;
			default:
				break;
			}

		}
		// inc by byte-size
		uint8_t* acP = (uint8_t*)acPtr;	acP++;
		acPtr = (uint32_t*)acP;
	}

	return 0;
}

/// <summary>
/// Creates a stereo buffer out of a buffer.
/// Assigns a right buffer to the main buffer
/// as private data.
/// </summary>
/// <param name="pcDevice">The d3d11 device</param>
/// <param name="pcContext">The d3d11 device context</param>
/// <param name="pcBuffer">The mono constant buffer to assign stereo constant buffers</param>
/// <param name="pDesc">Pointer to the buffer description</param>
/// <param name="pInitialData">Pointer to the initial data, NULL if bCopyData is true</param>
/// <param name="bCopyData">True if data from main buffer is to be copied to stereo buffers</param>
inline void CreateStereoBuffer(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11Buffer* pcBuffer, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, bool bCopyData)
{
	// create right buffer
	ID3D11Buffer* pcBufferRight = nullptr;
	if (FAILED(pcDevice->CreateBuffer(pDesc,
		pInitialData,
		&pcBufferRight)))
	{
		OutputDebugString(L"BindFlags;ByteWidth;CPUAccessFlags;MiscFlags;StructureByteStride;Usage");
		DEBUG_UINT(pDesc->BindFlags);
		DEBUG_UINT(pDesc->ByteWidth);
		DEBUG_UINT(pDesc->CPUAccessFlags);
		DEBUG_UINT(pDesc->MiscFlags);
		DEBUG_UINT(pDesc->StructureByteStride);
		DEBUG_UINT(pDesc->Usage);
		OutputDebugString(L"MatrixModifier: Failed to create right buffer!");
		return;
	}

	// copy resource ?
	if (bCopyData)
	{
		//pcContext->CopyResource(pcBufferLeft, pcBuffer);
		pcContext->CopyResource(pcBufferRight, pcBuffer);
	}

	// set as private data interface to the main buffer
	pcBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_Constant_Buffer_Right, pcBufferRight);

	// reference counter must be 1 now (reference held by the main buffer)
	ULONG nRef = pcBufferRight->Release();
	if (nRef != 1) OutputDebugString(L"MatrixModifier: Reference counter invalid !");
}
#pragma endregion

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