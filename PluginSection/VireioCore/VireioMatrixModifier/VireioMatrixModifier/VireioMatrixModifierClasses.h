/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifierClasses.h> and
Class <VireioMatrixModifierClasses> :
Copyright (C) 2020 Denis Reischl

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

#include<stdio.h>
#include<vector>
#include<string>
#include"..\..\..\Include\Vireio_GameConfig.h"

#include<d3d11_1.h>
#include<d3d11.h>
#include<d3d10_1.h>
#include<d3d10.h>
#include<d3d9.h>
#include<d3dx9.h>

#pragma region => Element calculation class
/// <summary>
/// All mathematical registers enumeration.
/// Stores all mathematical registers (vectors, 
/// matrices), both input and output (meanwhile
/// stored).
/// 1 Register  =  4 * float = Vector4
/// 4 Registers = 16 * float = Matix4x4
/// </summary>
enum class MathRegisters : size_t
{
	Start = 0, // Placeholder
	End        // Placeholder
};
static const MathRegisters s_eRegInMIN = MathRegisters::Start;
static const MathRegisters s_eRegInMAX = MathRegisters::End;

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#elif defined(VIREIO_D3D9)
typedef D3DXVECTOR4 REGISTER4F;
#endif

/// <summary>
/// Class for any matrix/vector calculation.
/// Calculates different matrices and vertices for different nodes.
/// </summary>
class ElementCalculation
{
public:
	ElementCalculation(Vireio_GameConfiguration* psConfig) : m_psConfig(psConfig) { m_aMathRegisters.resize((size_t)s_eRegInMAX); }
	virtual ~ElementCalculation() {}

	
	/// <summary> All mathematical elements needed to compute any modification. </summary>
	std::vector<REGISTER4F> m_aMathRegisters;
	/// <summary>Vireio v4.x game configuration.</summary>
	Vireio_GameConfiguration* m_psConfig;
};
#pragma endregion

#pragma region => Shader constant modification
/// <summary>
/// Abstract class acts as the skeleton class for Shader Modification Classes,
/// contains no shader modification logic simply keeps track of info.
/// Must implement ApplyModification method.
/// Prototype for any shader register modification.
/// 
/// Original class by Chris Drain (c) 2013
/// </summary>
/// <typeparam name="T"> The type of parameter to use for applying modifications, default is float.</typeparam>
template <class T = float>
class ShaderConstantModification
{
public:
	/// <summary> Constructor/Desctructor </summary>
	/// <param name="modID">Identifier of the modification. Identifier enumerations defined in ShaderConstantModificationFactory</param>
	ShaderConstantModification(UINT modID, std::shared_ptr<ElementCalculation> pcCalculaion) : m_ModificationID(modID), m_pcCalculation(pcCalculaion) {}
	virtual ~ShaderConstantModification() {}

	/// <summary> Pure virtual method, should apply the modification to produce left and right versions </summary>
	virtual void ApplyModification(const T* inData, std::vector<T>* outLeft, std::vector<T>* outRight) = 0;

	/// <summary> Simply a way to identify this modification. Useful for comparing shadermodification equality </summary>
	UINT m_ModificationID;

	/// <summary> Calculation class. </summary>
	std::shared_ptr<ElementCalculation> m_pcCalculation;
};

/// <summary>
/// Constant modification rule (v4+) normalized.
/// </summary>
struct Vireio_Constant_Modification_Rule_Normalized
{
	char m_szConstantName[64];
	UINT m_dwBufferIndex;
	UINT m_dwBufferSize;
	UINT m_dwStartRegIndex;
	bool m_bUseName;
	bool m_bUsePartialNameMatch;
	bool m_bUseBufferIndex;
	bool m_bUseBufferSize;
	bool m_bUseStartRegIndex;
	UINT m_dwRegisterCount;
	UINT m_dwOperationToApply;
	bool m_bTranspose;
};

/// <summary>
/// Constant modification rule (v4+).
/// Stores all data necessary for a
/// Original class >ConstantModificationRule< 2013 by Chris Drain.
/// </summary>
struct Vireio_Constant_Modification_Rule
{
	/// <summary>
	/// Constructor.
	/// Creates empty shader constant modification rule.
	/// </summary>
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
		m_bTranspose(false)
	{};

	/// <summary>
	/// Constructor.
	/// Creates rule out of a normalized one.
	/// </summary>
	Vireio_Constant_Modification_Rule(Vireio_Constant_Modification_Rule_Normalized* pRuleNormalized) :
		m_szConstantName(pRuleNormalized->m_szConstantName),
		m_dwBufferIndex(pRuleNormalized->m_dwBufferIndex),
		m_dwBufferSize(pRuleNormalized->m_dwBufferSize),
		m_dwStartRegIndex(pRuleNormalized->m_dwStartRegIndex),
		m_bUseName(pRuleNormalized->m_bUseName),
		m_bUsePartialNameMatch(pRuleNormalized->m_bUsePartialNameMatch),
		m_bUseBufferIndex(pRuleNormalized->m_bUseBufferIndex),
		m_bUseBufferSize(pRuleNormalized->m_bUseBufferSize),
		m_bUseStartRegIndex(pRuleNormalized->m_bUseStartRegIndex),
		m_dwRegisterCount(pRuleNormalized->m_dwRegisterCount),
		m_dwOperationToApply(pRuleNormalized->m_dwOperationToApply),
		m_bTranspose(pRuleNormalized->m_bTranspose)
	{};

	/// <summary>
	/// Constructor.
	/// Creates shader constant modification rule by specified data.
	/// @param szConstantName Constant string name.
	/// @param dwBufferIndex The index of the constant buffer (as set with ->?SSetConstantBuffers()).
	/// @param dwBufferSize The size of the constant buffer.
	/// @param dwStartRegIndex Shader start register of that rule.
	/// @param bUseName True to use the constant name to identify the constant.
	/// @param bUsePartialNameMatch True to use partial string name matches to identify the constant.
	/// @param bUseBufferIndex True to use the buffer index to identify the constant.
	/// @param bUseBufferSize True to use the buffer size to identify the constant.
	/// @param bUseStartRegIndex True to use the constant start register to identify the constant.
	/// @param dwRegisterCount Constant size (in shader registers = vectors = 4*sizeof(float)).
	/// @param dwOperationToApply Modification identifier.
	/// @param bTranspose True if input matrix should be bTransposed before modifying (and bTransposed back after).
	/// </summary>
	Vireio_Constant_Modification_Rule(std::string szConstantName, UINT dwBufferIndex, UINT dwBufferSize, UINT dwStartRegIndex, bool bUseName, bool bUsePartialNameMatch, bool bUseBufferIndex, bool bUseBufferSize, bool bUseStartRegIndex,
		UINT dwRegisterCount, UINT dwOperationToApply, bool bTranspose) :
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
		m_bTranspose(bTranspose)
	{};

	/// <summary>
	/// Constant string name.
	/// Empty string is "no constant".
	/// </summary>
	std::string m_szConstantName;
	/// <summary>
	/// If the shader has no constant table, we need the constant buffer index to identify the constant.
	/// </summary>
	UINT m_dwBufferIndex;
	/// <summary>
	/// If the shader has no constant table, we need the constant buffer size to identify the constant.
	/// </summary>
	UINT m_dwBufferSize;
	/// <summary>
	/// Shader start register of that rule.
	/// </summary>
	UINT m_dwStartRegIndex;
	/// <summary>
	/// If true the full constant string name will be used to identify the constant.
	/// </summary>
	bool m_bUseName;
	/// <summary>
	/// If true the partial constant string name will be used to identify the constant.
	/// </summary>
	bool m_bUsePartialNameMatch;
	/// <summary>
	/// If true the buffer index will be used to identify the constant.
	/// </summary>
	bool m_bUseBufferIndex;
	/// <summary>
	/// If true the buffer size will be used to identify the constant.
	/// </summary>
	bool m_bUseBufferSize;
	/// <summary>
	/// If true the start register index will be used to identify the constant.
	/// </summary>
	bool m_bUseStartRegIndex;
	/// <summary>
	/// Register count, 4 for Matrix, 1 for Vector, 2 for half-size matrices.
	/// For v4+ rules this count is used instead of D3DXPARAMETER_CLASS (used in v1.1->v3.x of the Driver).
	/// For this value there is no option (bool) to Use/NotUse to identify since this value is ALW
	/// </summary>
	UINT m_dwRegisterCount;
	/// <summary>
	/// Modification identifier.
	/// Identifier (int that maps to a m_dwRegisterCount type indexed class) of the operation to apply.
	/// </summary>
	UINT m_dwOperationToApply;
	/// <summary>
	/// True if input matrix should be transposed before modifying (and transposed back after).
	/// </summary>
	bool m_bTranspose;
	/// <summary>
	/// The eventual modification class. (created by m_dwOperationToApply index)
	/// </summary>
	std::shared_ptr<ShaderConstantModification<>> m_pcModification;
};

/// <summary>
/// Dummy class for now... TODO !!!
/// </summary>
class Vector4SimpleTranslate : public ShaderConstantModification<float>
{
public:
	Vector4SimpleTranslate(UINT uModID, std::shared_ptr<ElementCalculation> pcCalculation) : ShaderConstantModification(uModID, pcCalculation) {};

	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXVECTOR4 tempLeft(inData);
		D3DXVECTOR4 tempRight(inData);

		outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
		outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());
	}
};

/// <summary>
/// TODO !! create different modifications
/// </summary>
/// <param name="uModID">Modification Identifier</param>
/// <param name="pcCalculation">Elements Calculation class</param>
/// <returns></returns>
static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(UINT uModID, std::shared_ptr<ElementCalculation> pcCalculation)
{
	return std::make_shared<Vector4SimpleTranslate>(uModID, pcCalculation);
}

/// <summary>
/// TODO !! create different modifications
/// </summary>
/// <param name="uModID">Modification Identifier</param>
/// <param name="pcCalculation">Elements Calculation class</param>
/// <returns></returns>
static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(UINT uModID, std::shared_ptr<ElementCalculation> pcCalculation, bool bTranspose)
{
	UNREFERENCED_PARAMETER(bTranspose);

	return std::make_shared<Vector4SimpleTranslate>(uModID, pcCalculation);
}
#pragma endregion

#pragma region => Managed shader class
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#elif defined(VIREIO_D3D9)
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
/// Simple copy of the d3d x constant structure.
/// With a std::string name and without a default value pointer.
/// </summary>
typedef struct _SAFE_D3DXCONSTANT_DESC
{
	std::string Name;                   /**< Constant name ***/

	D3DXREGISTER_SET RegisterSet;       /**< Register set ***/
	UINT RegisterIndex;                 /**< Register index ***/
	UINT RegisterCount;                 /**< Number of registers occupied ***/

	D3DXPARAMETER_CLASS Class;          /**<  Class ***/
	D3DXPARAMETER_TYPE Type;            /**<  Component type ***/

	UINT Rows;                          /**<  Number of rows ***/
	UINT Columns;                       /**<  Number of columns ***/
	UINT Elements;                      /**<  Number of array elements ***/
	UINT StructMembers;                 /**<  Number of structure member sub-parameters ***/

	UINT Bytes;                         /**<  Data size, in bytes ***/
} SAFE_D3DXCONSTANT_DESC;

/// <summary>
/// Managed proxy shader class.
/// Contains left and right shader constants.
/// </summary>
template <class T = float>
class IDirect3DManagedStereoShader9 : public T
{
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	IDirect3DManagedStereoShader9(T* pcActualVertexShader, IDirect3DDevice9* pcOwningDevice, std::vector<Vireio_Constant_Modification_Rule>* pasConstantRules, std::vector<UINT>* paunGeneralIndices, std::vector<Vireio_Hash_Rule_Index>* pasShaderIndices, Vireio_Supported_Shaders eShaderType) :
		m_pcActualShader(pcActualVertexShader),
		m_pcOwningDevice(pcOwningDevice),
		m_pasConstantRules(pasConstantRules),
		m_paunGeneralIndices(paunGeneralIndices),
		m_pasShaderIndices(pasShaderIndices),
		m_unRefCount(1),
		m_eShaderType(eShaderType),
		m_bOutputShaderCode(false)
	{
		assert(pcActualVertexShader != NULL);
		assert(pcOwningDevice != NULL);
		assert(pasConstantRules != NULL);

		pcOwningDevice->AddRef();

		// get shader hash
		m_unShaderHash = ShaderHash(pcActualVertexShader);

		// create managed registers
		m_unMaxShaderConstantRegs = MAX_DX9_CONSTANT_REGISTERS; // TODO !! COUNT MAX CONSTANT REG NUMBER FOR THIS SHADER

		// ... and the buffer for the last SetShaderDataF() call
		m_afRegisterBuffer = std::vector<float>(m_unMaxShaderConstantRegs * VECTOR_LENGTH);

		// init the shader rules, first clear constants vector
		m_asConstantDesc = std::vector<SAFE_D3DXCONSTANT_DESC>();
		InitShaderRules();
	}
	/// <summary>
	/// Destructor.
	/// </summary>
	virtual ~IDirect3DManagedStereoShader9()
	{
		if (m_pcActualShader)
			m_pcActualShader->Release();

		if (m_pcOwningDevice)
			m_pcOwningDevice->Release();
	}

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		return m_pcActualShader->QueryInterface(riid, ppv);
	}
	virtual ULONG   WINAPI AddRef()
	{
		return ++m_unRefCount;
	}
	virtual ULONG   WINAPI Release()
	{
		if (--m_unRefCount == 0)
		{
			delete this;
			return 0;
		}

		return m_unRefCount;
	}

	/*** IDirect3DXShader9 methods ***/

	/// <summary>
	/// IDirect3DXShader9->GetDevice() call.
	/// </summary>
	virtual HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice)
	{
		if (!m_pcOwningDevice)
			return D3DERR_INVALIDCALL;
		else
		{
			*ppcDevice = m_pcOwningDevice;
			//m_pOwningDevice->AddRef(); //TODO Test this. Docs don't have the notice that is usually there about a refcount increase
			return D3D_OK;
		}
	}
	/// <summary>
	/// IDirect3DXShader9->GetFunction() call.
	/// </summary>
	virtual HRESULT WINAPI GetFunction(void* pDate, UINT* punSizeOfData)
	{
		return m_pcActualShader->GetFunction(pDate, punSizeOfData);
	}

	/*** IDirect3DManagedStereoShader9 methods ***/

	/// <summary>
	/// Inits the shader rules based on the constant table of the shader.
	/// </summary>
	void InitShaderRules()
	{
		// @see ShaderModificationRepository::GetModifiedConstantsF from Vireio < v3

		// clear constant rule vector
		m_asConstantRuleIndices = std::vector<Vireio_Constant_Rule_Index_DX9>();

		// clear register indices to max int
		FillMemory(m_aunRegisterModificationIndex, MAX_DX9_CONSTANT_REGISTERS * sizeof(UINT), 0xFF);

		// get shader function
		BYTE* pData = NULL;
		UINT pSizeOfData;

		m_pcActualShader->GetFunction(NULL, &pSizeOfData);
		pData = new BYTE[pSizeOfData];
		m_pcActualShader->GetFunction(pData, &pSizeOfData);

		// Load the constant descriptions for this shader and create StereoShaderConstants as the applicable rules require them.
		LPD3DXCONSTANTTABLE pConstantTable = NULL;
		D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);

		if (pConstantTable)
		{
			// set to defaults
			IDirect3DDevice9* pcDevice = nullptr;
			m_pcActualShader->GetDevice(&pcDevice);
			if (pcDevice)
			{
				pConstantTable->SetDefaults(pcDevice);
				pcDevice->Release();
			}

			// get constant table description
			D3DXCONSTANTTABLE_DESC pDesc;
			pConstantTable->GetDesc(&pDesc);

			D3DXCONSTANT_DESC pConstantDesc[64];

			// loop throught constants
			for (UINT unI = 0; unI < pDesc.Constants; unI++)
			{
				D3DXHANDLE handle = pConstantTable->GetConstant(NULL, unI);
				if (handle == NULL) continue;

				UINT unConstantNum = 64;
				pConstantTable->GetConstantDesc(handle, pConstantDesc, &unConstantNum);
				if (unConstantNum >= 64)
				{
					OutputDebugString(L"[MAM] Need larger constant description buffer");
					unConstantNum = 63;
				}

				for (UINT unJ = 0; unJ < unConstantNum; unJ++)
				{
					// add to constant vector
					SAFE_D3DXCONSTANT_DESC sDesc = {};
					sDesc.Name = std::string(pConstantDesc[unJ].Name);
					sDesc.RegisterSet = pConstantDesc[unJ].RegisterSet;
					sDesc.RegisterIndex = pConstantDesc[unJ].RegisterIndex;
					sDesc.RegisterCount = pConstantDesc[unJ].RegisterCount;
					sDesc.Class = pConstantDesc[unJ].Class;
					sDesc.Type = pConstantDesc[unJ].Type;
					sDesc.Rows = pConstantDesc[unJ].Rows;
					sDesc.Columns = pConstantDesc[unJ].Columns;
					sDesc.Elements = pConstantDesc[unJ].Elements;
					sDesc.StructMembers = pConstantDesc[unJ].StructMembers;
					sDesc.Bytes = pConstantDesc[unJ].Bytes;
					m_asConstantDesc.push_back(sDesc);

					// register count 1 (= vector) and 4 (= matrix) supported
					if (((pConstantDesc[unJ].Class == D3DXPC_VECTOR) && (pConstantDesc[unJ].RegisterCount == 1))
						|| (((pConstantDesc[unJ].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[unJ].Class == D3DXPC_MATRIX_COLUMNS)) && (pConstantDesc[unJ].RegisterCount == 4)))
					{
						// Check if any rules match this constant, first check shader specific indices
						bool bShaderSpecificRulePresent = false;
						for (UINT unK = 0; unK < (UINT)(*m_pasShaderIndices).size(); unK++)
						{
							// same hash code ?
							if ((*m_pasShaderIndices)[unK].unHash == m_unShaderHash)
							{
								// compare rule->description, break in case of success
								if (SUCCEEDED(VerifyConstantDescriptionForRule(&((*m_pasConstantRules)[(*m_pasShaderIndices)[unK].unRuleIndex]), &(pConstantDesc[unJ]), (*m_pasShaderIndices)[unK].unRuleIndex, (void*)pConstantDesc[unJ].DefaultValue)))
								{
									bShaderSpecificRulePresent = true;
									break;
								}
							}
						}

						// now check general indices if no shader specific rule got applied
						if (!bShaderSpecificRulePresent)
						{
							for (UINT unK = 0; unK < (UINT)(*m_paunGeneralIndices).size(); unK++)
							{
								// compare rule->description, break in case of success
								if (SUCCEEDED(VerifyConstantDescriptionForRule(&((*m_pasConstantRules)[(*m_paunGeneralIndices)[unK]]), &(pConstantDesc[unJ]), (*m_paunGeneralIndices)[unK], (void*)pConstantDesc[unJ].DefaultValue))) break;
							}
						}
					}
				}
			}
		}

		if (pConstantTable) pConstantTable->Release();
		if (pData) delete[] pData;
	}
	/// <summary>
	/// This shader is set new, applies all modifications.
	/// </summary>
	void SetShader(float* afRegisters)
	{
		UINT unInd = 0;

		// more data, loop through modified constants for this shader
		auto it = m_asConstantRuleIndices.begin();
		while (it != m_asConstantRuleIndices.end())
		{
			// apply to left and right data
			UINT unIndex = unInd;
			UINT unStartRegisterConstant = (*it).m_dwConstantRuleRegister;

			// get the matrix
			D3DXMATRIX sMatrix(&afRegisters[RegisterIndex(unStartRegisterConstant)]);
			{
				// matrix to be transposed ?
				bool bTranspose = (*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_bTranspose;
				if (bTranspose)
				{
					D3DXMatrixTranspose(&sMatrix, &sMatrix);
				}

				// do modification
				D3DXMATRIX sMatrixLeft, sMatrixRight;
				((ShaderMatrixModification*)(*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_pcModification.get())->DoMatrixModification(sMatrix, sMatrixLeft, sMatrixRight);

				// transpose back
				if (bTranspose)
				{
					D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
					D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
				}

				m_asConstantRuleIndices[unIndex].m_asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
				m_asConstantRuleIndices[unIndex].m_asConstantDataRight = (D3DMATRIX)sMatrixRight;
			}
			it++; unInd++;
		}
	}
	/// <summary>
	/// This shader is set old, applies unmodified data.
	/// </summary>
	void SetShaderOld(IDirect3DDevice9* pcDevice, float* afRegisters)
	{
		UINT unInd = 0;

		// more data, loop through modified constants for this shader
		auto it = m_asConstantRuleIndices.begin();
		while (it != m_asConstantRuleIndices.end())
		{
			// apply to left and right data
			UINT unIndex = unInd;
			UINT unStartRegisterConstant = (*it).m_dwConstantRuleRegister;

			// set back modification
			pcDevice->SetVertexShaderConstantF(unStartRegisterConstant, &afRegisters[RegisterIndex(unStartRegisterConstant)], 4);

			it++; unInd++;
		}
	}
	/// <summary>
	/// Override IDirect3DDevice9->SetShaderConstantF() here.
	/// </summary>
	HRESULT SetShaderConstantF(UINT unStartRegister, const float* pfConstantData, UINT unVector4fCount, bool& bModified, RenderPosition eRenderSide, float* afRegisters)
	{
		// no rules present ? return
		if (!m_asConstantRuleIndices.size()) return S_OK;

		// set buffer
		memcpy(&m_afRegisterBuffer[0], pfConstantData, unVector4fCount * 4 * sizeof(float));

		// modification present for this index ?
		if ((m_aunRegisterModificationIndex[unStartRegister] < (UINT)m_asConstantRuleIndices.size()) && (unVector4fCount == 4))
		{
			// apply to left and right data
			UINT unIndex = m_aunRegisterModificationIndex[unStartRegister];
			bModified = true;

			// get the matrix
			D3DXMATRIX sMatrix(&afRegisters[RegisterIndex(unStartRegister)]);
			{
				// matrix to be transposed ?
				bool bTranspose = (*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_bTranspose;
				if (bTranspose)
				{
					D3DXMatrixTranspose(&sMatrix, &sMatrix);
				}

				// do modification
				D3DXMATRIX sMatrixLeft, sMatrixRight;
				((ShaderMatrixModification*)(*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_pcModification.get())->DoMatrixModification(sMatrix, sMatrixLeft, sMatrixRight);

				// transpose back
				if (bTranspose)
				{
					D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
					D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
				}

				m_asConstantRuleIndices[unIndex].m_asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
				m_asConstantRuleIndices[unIndex].m_asConstantDataRight = (D3DMATRIX)sMatrixRight;

				// copy modified data to buffer
				if (eRenderSide == RenderPosition::Left)
					memcpy(&m_afRegisterBuffer[0], &sMatrixLeft, sizeof(D3DMATRIX));
				else
					memcpy(&m_afRegisterBuffer[0], &sMatrixRight, sizeof(D3DMATRIX));

			}
		}
		else
		{
			UINT unInd = 0;

			// more data, loop through modified constants for this shader
			auto it = m_asConstantRuleIndices.begin();
			while (it != m_asConstantRuleIndices.end())
			{
				// register in range ?
				if ((unStartRegister < ((*it).m_dwConstantRuleRegister + (*it).m_dwConstantRuleRegisterCount)) && ((unStartRegister + unVector4fCount) > (*it).m_dwConstantRuleRegister))
				{
					// apply to left and right data
					bModified = true;
					UINT unIndex = unInd;
					UINT unStartRegisterConstant = (*it).m_dwConstantRuleRegister;

					// get the matrix
					D3DXMATRIX sMatrix(&afRegisters[RegisterIndex(unStartRegisterConstant)]);
					{
						// matrix to be transposed ?
						bool bTranspose = (*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_bTranspose;
						if (bTranspose)
						{
							D3DXMatrixTranspose(&sMatrix, &sMatrix);
						}

						// do modification
						D3DXMATRIX sMatrixLeft, sMatrixRight;
						((ShaderMatrixModification*)(*m_pasConstantRules)[m_asConstantRuleIndices[unIndex].m_dwIndex].m_pcModification.get())->DoMatrixModification(sMatrix, sMatrixLeft, sMatrixRight);

						// transpose back
						if (bTranspose)
						{
							D3DXMatrixTranspose(&sMatrixLeft, &sMatrixLeft);
							D3DXMatrixTranspose(&sMatrixRight, &sMatrixRight);
						}

						m_asConstantRuleIndices[unIndex].m_asConstantDataLeft = (D3DMATRIX)sMatrixLeft;
						m_asConstantRuleIndices[unIndex].m_asConstantDataRight = (D3DMATRIX)sMatrixRight;

						// copy modified data to buffer
						if (eRenderSide == RenderPosition::Left)
						{
							if (unStartRegister <= unStartRegisterConstant)
								memcpy(&m_afRegisterBuffer[unStartRegisterConstant - unStartRegister], &sMatrixLeft, sizeof(D3DMATRIX));
							else
								OutputDebugString(L"TODO: Handle partially changed matrices.");
						}
						else
						{
							if (unStartRegister <= unStartRegisterConstant)
								memcpy(&m_afRegisterBuffer[unStartRegisterConstant - unStartRegister], &sMatrixRight, sizeof(D3DMATRIX));
							else
								OutputDebugString(L"TODO: Handle partially changed matrices.");
						}
					}
				}
				it++; unInd++;
			}
		}

		return D3D_OK;
	}
	/// <summary>
	/// The hash code of the shader.
	/// </summary>
	uint32_t GetShaderHash() { return m_unShaderHash; }
	/// <summary>
	/// The actual shader.
	/// </summary>
	T* GetActualShader() { return m_pcActualShader; }
	/// <summary>
	/// The constant list.
	/// </summary>
	std::vector<SAFE_D3DXCONSTANT_DESC>* GetConstantDescriptions() { return &m_asConstantDesc; }

	/// <summary>
	/// The indices of the shader rules assigned to that shader.
	/// For DX9 these indices also contain the left/right modified constant data.
	/// </summary>
	std::vector<Vireio_Constant_Rule_Index_DX9> m_asConstantRuleIndices;
	/// <summary>
	/// Shader register buffer. Currently modified shader constant data.
	/// 4 floats == 1 register (defined in VECTOR_LENGTH).
	/// Filled with the last SetShaderConstantF update.
	/// </summary>
	std::vector<float> m_afRegisterBuffer;

protected:
	/// <summary>
	/// The actual vertex shader embedded.
	/// </summary>
	T* const m_pcActualShader;
	/// <summary>
	/// The type of this class.
	/// </summary>
	Vireio_Supported_Shaders m_eShaderType;
	/// <summary>
	/// Pointer to the D3D device that owns the shader.
	/// </summary>
	IDirect3DDevice9* m_pcOwningDevice;
	/// <summary>
	/// Internal reference counter.
	/// </summary>
	ULONG m_unRefCount;
	/// <summary>
	/// Maximum shader constant registers.
	/// </summary>
	UINT m_unMaxShaderConstantRegs;
	/// <summary>
	/// The shader hash code.
	/// </summary>
	uint32_t m_unShaderHash;
	/// <summary>
	/// Pointer to all available constant rules.
	/// </summary>
	std::vector<Vireio_Constant_Modification_Rule>* m_pasConstantRules;
	/// <summary>
	/// Pointer to all general rule indices.
	/// </summary>
	std::vector<UINT>* m_paunGeneralIndices;
	/// <summary>
	/// Pointer to all shader specfic rule indices.
	/// </summary>
	std::vector<Vireio_Hash_Rule_Index>* m_pasShaderIndices;
	/// <summary>
	/// Index of modification for the specified register.
	/// </summary>
	UINT m_aunRegisterModificationIndex[MAX_DX9_CONSTANT_REGISTERS];
	/// <summary>
	/// The constant descriptions for that shader.
	/// </summary>
	std::vector<SAFE_D3DXCONSTANT_DESC> m_asConstantDesc;

private:
	/// <summary>
	/// Simple helper to get the hash of a shader.
	/// @param pShader The input vertex shader.
	/// @return The hash code of the shader.
	/// </summary>
	uint32_t ShaderHash(T* pcShader)
	{
		if (!pcShader) return 0;

		BYTE* pnData = NULL;
		UINT unSizeOfData = 0;
		pcShader->GetFunction(NULL, &unSizeOfData);

		pnData = new BYTE[unSizeOfData];
		pcShader->GetFunction(pnData, &unSizeOfData);

		uint32_t unHash = GetHashCode(pnData, (int32_t)unSizeOfData, VIREIO_SEED);

		// output shader code ?
		if (m_bOutputShaderCode)
		{
			// optionally, output shader code to "?S(hash).txt"
			char buf[32]; ZeroMemory(&buf[0], 32);
			switch (m_eShaderType)
			{
			case VertexShader:
				sprintf_s(buf, "VS%u.txt", unHash);
				break;
			case PixelShader:
				sprintf_s(buf, "PS%u.txt", unHash);
				break;
			default:
				sprintf_s(buf, "UNKNOWN%u.txt", unHash);
				break;
			}
			std::ofstream oLogFile(buf, std::ios::ate);

			if (oLogFile.is_open())
			{
				LPD3DXBUFFER pcBuffer;
				D3DXDisassembleShader(reinterpret_cast<DWORD*>(pnData), NULL, NULL, &pcBuffer);
				oLogFile << static_cast<char*>(pcBuffer->GetBufferPointer()) << std::endl;
				oLogFile << std::endl << std::endl;
				oLogFile << "// Shader Hash   : " << unHash << std::endl;
			}
		}

		delete[] pnData;
		return unHash;
	}
	/// <summary>
	/// Compares a constant description and a shader rule, if succeedes it adds the index to the shader constant rule indices.
	/// </summary>
	HRESULT VerifyConstantDescriptionForRule(Vireio_Constant_Modification_Rule* psRule, D3DXCONSTANT_DESC* psDescription, UINT unRuleIndex, void* pDefaultValue)
	{
		// Type match
		if (psRule->m_dwRegisterCount == psDescription->RegisterCount)
		{
			// name match required
			if (psRule->m_szConstantName.size() > 0)
			{
				bool nameMatch = false;
				if (psRule->m_bUsePartialNameMatch)
				{
					nameMatch = std::strstr(psDescription->Name, psRule->m_szConstantName.c_str()) != NULL;
				}
				else
				{
					nameMatch = psRule->m_szConstantName.compare(psDescription->Name) == 0;
				}

				if (!nameMatch)
				{
					// no match
					return E_NO_MATCH;
				}
			}

			// register match required
			if (psRule->m_dwStartRegIndex != UINT_MAX)
			{
				if (psRule->m_dwStartRegIndex != psDescription->RegisterIndex)
				{
					// no match
					return E_NO_MATCH;
				}
			}

#ifdef _DEBUG
			// output shader constant + index 
			switch (psDescription->Class)
			{
			case D3DXPC_VECTOR:
				OutputDebugString(L"VS: D3DXPC_VECTOR");
				break;
			case D3DXPC_MATRIX_ROWS:
				OutputDebugString(L"VS: D3DXPC_MATRIX_ROWS");
				break;
			case D3DXPC_MATRIX_COLUMNS:
				OutputDebugString(L"VS: D3DXPC_MATRIX_COLUMNS");
				break;
			default:
				OutputDebugString(L"VS: UNKNOWN_CONSTANT");
				break;
			}
			debugf("Register Index: %d", psDescription->RegisterIndex);
#endif 
			// set register index
			m_aunRegisterModificationIndex[psDescription->RegisterIndex] = (UINT)m_asConstantRuleIndices.size();

			// set constant rule index
			Vireio_Constant_Rule_Index_DX9 sConstantRuleIndex;
			sConstantRuleIndex.m_dwIndex = unRuleIndex;
			sConstantRuleIndex.m_dwConstantRuleRegister = psDescription->RegisterIndex;
			sConstantRuleIndex.m_dwConstantRuleRegisterCount = psDescription->RegisterCount;

			// init data if default value present
			if (pDefaultValue)
			{
				memcpy(&sConstantRuleIndex.m_afConstantDataLeft[0], pDefaultValue, psDescription->RegisterCount * sizeof(float) * 4);
				memcpy(&sConstantRuleIndex.m_afConstantDataRight[0], pDefaultValue, psDescription->RegisterCount * sizeof(float) * 4);
			};

			m_asConstantRuleIndices.push_back(sConstantRuleIndex);

			// only the first matching rule is applied to a constant
			return S_OK;
		}
		else return E_NO_MATCH;
	}

	/// <summary>
	/// True if shader code should be saved in file.
	/// </summary>
	bool m_bOutputShaderCode;
};
#endif
#pragma endregion
