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

#define IPD_DEFAULT 0.064f /**< 64mm in meters */

#define DEFAULT_CONVERGENCE 3.0f
#define DEFAULT_PFOV 110.0f
#define DEFAULT_ASPECT_MULTIPLIER 1.0f

#define DEFAULT_YAW_MULTIPLIER 25.0f
#define DEFAULT_PITCH_MULTIPLIER 25.0f
#define DEFAULT_ROLL_MULTIPLIER 1.0f

#define DEFAULT_POS_TRACKING_X_MULT 2.0f
#define DEFAULT_POS_TRACKING_Y_MULT 2.5f
#define DEFAULT_POS_TRACKING_Z_MULT 0.5f

#define VIREIO_MAX_VARIABLE_NAME_LENGTH      64  /**< We restrict variable names to 64 characters. ***/
#define VIREIO_CONSTANT_RULES_NOT_ADDRESSED - 1  /**< No shader rules addressed for this shader. ***/
#define VIREIO_CONSTANT_RULES_NOT_AVAILABLE - 2  /**< No shader rules available for this shader. ***/

#pragma region inline helper
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
* Creates a stereo buffer out of a buffer.
* Assigns a right buffer to the main buffer
* as private data.
* @param pcDevice The d3d11 device.
* @param pcContext The d3d11 device context.
* @param pcBuffer The mono constant buffer to assign stereo constant buffers.
* @param pDesc Pointer to the buffer description.
* @param pInitialData Pointer to the initial data, NULL if bCopyData is true.
* @param bCopyData True if data from main buffer is to be copied to stereo buffers.
***/
inline void CreateStereoBuffer(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11Buffer* pcBuffer, D3D11_BUFFER_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, bool bCopyData)
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
	UINT                                      dwRegister;                 /**< The index of the shader constant register. ***/
	UINT                                      dwSize;                     /**< The number of float constants in that buffer. ***/
	enum D3D11_Constant_Buffer_AccessPattern
	{
		immediateIndexed,
		dynamicIndexed,
	}                                         eAccessPattern;             /**< The way that the buffer will be accessed by shader code. ***/
	INT                                       nConstantRulesIndex;        /**< The index of the constant rules used. (-1 = not addressed, -2 = no rules) ***/
};

/// <summary>
/// Vireio basic shader description.
/// </summary>
struct Vireio_Shader
{
	UINT                                                  dwVersion;                                    /**< Shader version ***/
	CHAR                                                  szCreator[VIREIO_MAX_VARIABLE_NAME_LENGTH];   /**< Creator string ***/
	UINT                                                  dwHashCode;                                   /**< This shaders hash code. ***/
};

/// <summary>
/// Vireio D3D9 shader description.
/// Structure containing all necessary data for the IDirect3D(X)Shader9 interfaces.
/// </summary>
struct Vireio_D3D9_Shader : public Vireio_Shader
{
	std::vector<SAFE_D3DXCONSTANT_DESC>                   asConstantDescriptions;                       /**< Shader constant descriptions. ***/
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