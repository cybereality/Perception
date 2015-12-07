/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifierDataStructures.h> and
Class <VireioMatrixModifierDataStructures> :
Copyright (C) 2015 Denis Reischl

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

#define VIREIO_MAX_VARIABLE_NAME_LENGTH 64     /**< We restrict variable names to 64 characters. ***/

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