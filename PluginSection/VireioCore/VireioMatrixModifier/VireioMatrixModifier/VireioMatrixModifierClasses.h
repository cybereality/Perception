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
#include<array>
#include<string>
#include<iostream>
#include<sstream>
#include<iomanip>
#include"..\..\..\Include\Vireio_GameConfig.h"

#include<d3d11_1.h>
#include<d3d11.h>
#include<d3d10_1.h>
#include<d3d10.h>
#include<d3d9.h>
#include<d3dx9.h>

// constants
constexpr float fLEFT_CONSTANT = -1.f;
constexpr float fRIGHT_CONSTANT = 1.f;

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

#define E_NO_MATCH         _HRESULT_TYPEDEF_(0x8A596AF85)

#define loop(cnt, mtd) { for (uint32_t _uXX = 0; _uXX < cnt; _uXX++) mtd; }

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
typedef D3DXVECTOR4 REGISTER4F;
typedef D3DXMATRIX MATRIX16F;
#elif defined(VIREIO_D3D9)
typedef D3DXVECTOR4 REGISTER4F;
typedef D3DXMATRIX MATRIX16F;
#endif

#pragma region /// => inline helper
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
	if ((*acPtr != OPCODE_HEADER_UTF32[0]) && 
		(*acPtr != OPCODE_HEADER_UTF32[1]) &&
		(*acPtr != OPCODE_HEADER_UTF32[2]) &&
		(*acPtr != OPCODE_HEADER_UTF32[3]) &&
		(*acPtr != OPCODE_HEADER_UTF32[4]))
	{
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
				std::string atCreator = (const char*)acCTAB + sCTable->uCreator;

				// only provide creator string if size min 16
				if (atCreator.length() >= 16)
					uCreatorIx = sCTable->uCreator + (uint32_t)UINT_PTR(acCTAB) - (uint32_t)UINT_PTR(acFunc);

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
			acPtr += uChunkSize + 1;
		}
		else
		{
			// inc by byte-size
			uint8_t* acP = (uint8_t*)acPtr;	acP++;
			acPtr = (uint32_t*)acP;
		}
	}

	// get the size and hash code
	uSizeOfData = 4 + (uint32_t)((UINT_PTR)acPtr - (UINT_PTR)acFunc);
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
	if ((*acPtr != OPCODE_HEADER_UTF32[0]) &&
		(*acPtr != OPCODE_HEADER_UTF32[1]) &&
		(*acPtr != OPCODE_HEADER_UTF32[2]) &&
		(*acPtr != OPCODE_HEADER_UTF32[3]) &&
		(*acPtr != OPCODE_HEADER_UTF32[4]))
	{
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
				std::string atCreator = (const char*)acCTAB + sCTable->uCreator;

				// only provide creator string if size min 16
				if (atCreator.length() >= 16)
					return sCTable->uCreator + (uint32_t)((UINT_PTR)acCTAB - (UINT_PTR)acFunc);
			}
			break;
			case ShaderChunkType::Unknown:
				break;
			default:
				break;
			}
			acPtr += uChunkSize;
		}
		else
		{
			// inc by byte-size
			uint8_t* acP = (uint8_t*)acPtr;	acP++;
			acPtr = (uint32_t*)acP;
		}
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
		TRACE_UINT(pDesc->BindFlags);
		TRACE_UINT(pDesc->ByteWidth);
		TRACE_UINT(pDesc->CPUAccessFlags);
		TRACE_UINT(pDesc->MiscFlags);
		TRACE_UINT(pDesc->StructureByteStride);
		TRACE_UINT(pDesc->Usage);
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

#pragma region /// => Modification calculation class
/// <summary>
/// All float fields enumeration used
/// to compute any mathematical operation.
/// </summary>
enum class MathFloatFields : size_t
{
	Roll,                     /*< Amount of actual roll (in radians) */
	IPD,                      /*< Interpupillary distance. */
	Convergence,              /*< Convergence. Left/Rigth offset adjustment. In millimeters. */
	Squash,                   /*< The amount of squashing GUI shader constants. */
	GUI_Depth,                /*< The 3d depth of the GUI. */
	HUD_Distance,             /*< The distance of the HUD. */
	HUD_Depth,                /*< The 3d depth of the HUD. */
	AspectMultiplier,         /*< Aspect multiplier for projection matrices */
	FOV_H,                    /*< FOV, horizontal */
	FOV_V,                    /*< FOV, vertical*/
	Separation_World,         /*< Separation, in World Units */
	Separation_IPDAdjustment, /*< Separation IPD adjustment being used for GUI and HUD matrices */
	Frustum_Asymmetry,        /*< Frustum Asymmetry, in meters. For convergence computation */
	Physical_Screensize,      /*< Physical (Monitor) Screen Size, in meters. For convergence computation */
	LensXCenterOffset,        /*< HMD lens center horizontal offset */

	Math_FloatFields_Size

};
constexpr size_t Math_FloatFields_Size = (size_t)MathFloatFields::Math_FloatFields_Size;

/// <summary>
/// All mathematical registers enumeration.
/// Indices to all mathematical registers (vectors, 
/// matrices), both input and output (meanwhile
/// stored).
/// 1 Register  =  4 * float = Vector4
/// 4 Registers = 16 * float = Matix4x4
/// </summary>
enum class MathRegisters : size_t
{
	VEC_PositionTransform = 0,

	MAT_BasicProjection = VEC_PositionTransform + 4,                       /*< Projection matrix - basic with no PFOV */
	MAT_ProjectionInv = MAT_BasicProjection + 4,                           /*< Projection inverse matrix */
	MAT_ProjectionFOV = MAT_ProjectionInv + 4,                             /*< The projection with adjusted FOV */
	MAT_ProjectionConvL = MAT_ProjectionFOV + 4,                           /*< The projection with left eye convergence */
	MAT_ProjectionConvR = MAT_ProjectionConvL + 4,                         /*< The projection with right eye convergence */
	MAT_Roll = MAT_ProjectionConvR + 4,                                    /*< The head roll matrix */
	MAT_RollNegative = MAT_Roll + 4,                                       /*< The head roll matrix. (negative) */
	MAT_RollHalf = MAT_RollNegative + 4,                                   /*< The head roll matrix. (half roll) */
	MAT_TransformL = MAT_RollHalf + 4,                                     /*< Left matrix used to roll (if roll enabled) and shift view for ipd. */
	MAT_TransformR = MAT_TransformL + 4,                                   /*< Right matrix used to roll (if roll enabled) and shift view for ipd. */
	MAT_ViewProjectionL = MAT_TransformR + 4,                              /*< Left view projection matrix */
	MAT_ViewProjectionR = MAT_ViewProjectionL + 4,                         /*< Right view projection matrix */
	MAT_ViewProjectionTransL = MAT_ViewProjectionR + 4,                    /*< Left view projection transform matrix */
	MAT_ViewProjectionTransR = MAT_ViewProjectionTransL + 4,               /*< Right view projection transform matrix */
	MAT_ViewProjectionTransNoRollL = MAT_ViewProjectionTransR + 4,         /*< Left view projection transform matrix (no roll) */
	MAT_ViewProjectionTransNoRollR = MAT_ViewProjectionTransNoRollL + 4,   /*< Right view projection transform matrix (no roll) */
	MAT_Position = MAT_ViewProjectionTransNoRollR + 4,                     /*< Positional translation matrix */
	MAT_GatheredL = MAT_Position + 4,                                      /*< Gathered matrix to be used in gathered modifications */
	MAT_GatheredR = MAT_GatheredL + 4,                                     /*< Gathered matrix to be used in gathered modifications */
	MAT_HudL = MAT_GatheredR + 4,                                          /*< Left HUD matrix. */
	MAT_HudR = MAT_HudL + 4,                                               /*< Right HUD matrix */
	MAT_GuiL = MAT_HudR + 4,                                               /*< Left GUI matrix. */
	MAT_GuiR = MAT_GuiL + 4,                                               /*< Right GUI matrix. */
	MAT_Squash = MAT_GuiR + 4,                                             /*< Squash scaling matrix, to be used in HUD/GUI scaling matrices. */
	MAT_HudDistance = MAT_Squash + 4,                                      /*< HUD distance matrix, to be used in HUD scaling matrices. */
	MAT_Hud3dDepthL = MAT_HudDistance + 4,                                 /*< HUD 3d depth matrix, to be used in HUD separation matrices. */
	MAT_Hud3dDepthR = MAT_Hud3dDepthL + 4,                                 /*< HUD 3d depth matrix, to be used in HUD separation matrices. */
	MAT_Hud3dDepthShiftL = MAT_Hud3dDepthR + 4,                            /*< HUD 3d depth matrix (shifted), to be used in HUD separation matrices. */
	MAT_Hud3dDepthShiftR = MAT_Hud3dDepthShiftL + 4,                       /*< HUD 3d depth matrix (shifted), to be used in HUD separation matrices. */
	MAT_Gui3dDepthL = MAT_Hud3dDepthShiftR + 4,                            /*< GUI 3d depth matrix, to be used in GUI separation matrices. */
	MAT_Gui3dDepthR = MAT_Gui3dDepthL + 4,                                 /*< GUI 3d depth matrix, to be used in GUI separation matrices. */
	MAT_ConvergenceOffL = MAT_Gui3dDepthR + 4,                             /*< Convergence offset left. (only translation) */
	MAT_ConvergenceOffR = MAT_ConvergenceOffL + 4,                         /*< Convergence offset right. (only translation) */

	Math_Registers_Size = MAT_GatheredR + 4     // not used
};
constexpr size_t Math_Registers_Size = (size_t)MathRegisters::Math_Registers_Size;

/// <summary>
/// Class for any matrix/vector calculation.
/// Calculates different matrices and vertices (shader registers) for different nodes.
/// Compression of Class >ViewAdjustment< 2013 by Chris Drain
/// 
/// TODO !! PROJECTION FOV !! PIXEL SHADER ROLL !! CONVERGENCE FOR MONITOR STEREO !! MATRIX POSITION TRACKING !! HMD LENS X CENTER !!
/// </summary>
class ModificationCalculation
{
public:
	ModificationCalculation(Vireio_GameConfiguration* psConfig) : m_psConfig(psConfig)
	{
		SetFloat(MathFloatFields::AspectMultiplier, psConfig->fAspectMultiplier);
		SetFloat(MathFloatFields::FOV_H, 110.0f);
		SetFloat(MathFloatFields::Roll, 0.f);
		SetFloat(MathFloatFields::Convergence, psConfig->fConvergence);
		SetFloat(MathFloatFields::IPD, psConfig->fIPD);
		SetFloat(MathFloatFields::Separation_World, (psConfig->fIPD / 2.f) * psConfig->fWorldScaleFactor);
		SetFloat(MathFloatFields::Separation_IPDAdjustment, ((psConfig->fIPD - IPD_DEFAULT) / 2.f) * psConfig->fWorldScaleFactor);
		SetFloat(MathFloatFields::Frustum_Asymmetry, 0.f);   // ignore convergence settings for now
		SetFloat(MathFloatFields::Physical_Screensize, 1.f); // set to 1 to avoid division by zero
		SetFloat(MathFloatFields::LensXCenterOffset, 0.f);
		ComputeViewTransforms();
	}
	virtual ~ModificationCalculation() {}

	/// <summary>
	/// Loads basic fields from config.
	/// </summary>
	/// <param name="psConfig"></param>
	void Load(Vireio_GameConfiguration* psConfig)
	{
		m_psConfig = psConfig;
		SetFloat(MathFloatFields::Convergence, psConfig->fConvergence);
		SetFloat(MathFloatFields::IPD, psConfig->fIPD);
		SetFloat(MathFloatFields::Separation_World, (psConfig->fIPD / 2.f) * psConfig->fWorldScaleFactor);
		SetFloat(MathFloatFields::Separation_IPDAdjustment, ((psConfig->fIPD - IPD_DEFAULT) / 2.f) * psConfig->fWorldScaleFactor);

		// TODO !! PROJECTION FOV !
	}

	/// <summary>
	/// Computes all necessary view matrices for modification/transform.
	/// Do not change the succession of the computiations here !!
	/// </summary>
	void ComputeViewTransforms()
	{
		Compute(MathRegisters::MAT_BasicProjection);
		Compute(MathRegisters::MAT_ProjectionFOV);
		Compute(MathRegisters::MAT_ProjectionConvL);
		Compute(MathRegisters::MAT_Roll);
		Compute(MathRegisters::MAT_TransformL);
		Compute(MathRegisters::MAT_ViewProjectionL);
		Compute(MathRegisters::MAT_ViewProjectionTransL);

		// TODO !! POSITIONAL TRACKING !
	}

	/// <summary>
	/// Set float field.
	/// </summary>
	/// <param name="eIndex">Index based on enumeration</param>
	/// <param name="fValue">Value to be applied</param>
	void SetFloat(MathFloatFields eIndex, float fValue)
	{
		m_aMathFloat[(size_t)eIndex] = fValue;
	}

	/// <summary>
	/// Set register field.
	/// </summary>
	/// <param name="eIndex">Index based on enumeration</param>
	/// <param name="fValue">Value to be applied</param>
	void SetRegister(MathRegisters eIndex, REGISTER4F fValue)
	{
		m_aMathRegisters[(size_t)eIndex] = fValue;
	}

	/// <summary>
	/// => Any computation (update) done here.
	/// </summary>
	/// <param name="eRegister">The register to be updated</param>
	void Compute(MathRegisters eRegister)
	{
		// Minimum (near) Z-value
		const float n = 0.1f;
		// Maximum (far) Z-value
		const float f = 10.0f;
		// Minimum (left) X-value
		const float l = -0.5f;
		// Maximum (right) X-value
		const float r = 0.5f;

		switch (eRegister)
		{
		case MathRegisters::VEC_PositionTransform:
			break;
		case MathRegisters::MAT_BasicProjection:
		case MathRegisters::MAT_ProjectionInv:
		{
			float fAspect = m_aMathFloat[(size_t)MathFloatFields::AspectMultiplier];
			// Maximum (top) y-value of the view volume
			float t = 0.5f / fAspect;
			// Minimum (bottom) y-value of the volume
			float b = -0.5f / fAspect;
			// Calculate basic projection
			D3DXMatrixPerspectiveOffCenterLH((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection], l, r, b, t, n, f);
			D3DXMatrixInverse((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv], 0, (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection]);
		}
		break;
		case MathRegisters::MAT_ProjectionFOV:
		{
			float fAspect = m_aMathFloat[(size_t)MathFloatFields::AspectMultiplier];
			float fFOV_horiz = m_aMathFloat[(size_t)MathFloatFields::FOV_H];

			// TODO !! NO HMD ?? see ViewAdjustment::UpdateProjectionMatrices()

			// Calculate vertical fov from provided horizontal
			float fFOV_vert = m_aMathFloat[(size_t)MathFloatFields::FOV_V] = 2.0f * (float)atan(tan(D3DXToRadian(fFOV_horiz) / 2.0f) * fAspect);

			// And left and right (identical in this case)
			D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionFOV], fFOV_vert, fAspect, n, f);
		}
		break;
		case MathRegisters::MAT_ProjectionConvL:
		case MathRegisters::MAT_ProjectionConvR:
		case MathRegisters::MAT_ConvergenceOffL:
		case MathRegisters::MAT_ConvergenceOffR:
		{
			// TODO !! CONVERGENCE FOR MONITOR STEREO

			float frustumAsymmetryInMeters = m_aMathFloat[(size_t)MathFloatFields::Frustum_Asymmetry];
			float physicalScreenSizeInMeters = m_aMathFloat[(size_t)MathFloatFields::Physical_Screensize];
			float fAspect = m_aMathFloat[(size_t)MathFloatFields::AspectMultiplier];

			// Maximum (top) y-value of the view volume
			float t = 0.5f / fAspect;
			// Minimum (bottom) y-value of the volume
			float b = -0.5f / fAspect;

			// divide the frustum asymmetry by the assumed physical size of the physical screen
			float frustumAsymmetryLeftInMeters = (frustumAsymmetryInMeters * fLEFT_CONSTANT) / physicalScreenSizeInMeters;
			float frustumAsymmetryRightInMeters = (frustumAsymmetryInMeters * fRIGHT_CONSTANT) / physicalScreenSizeInMeters;

			// get the horizontal screen space size and compute screen space adjustment
			float screenSpaceXSize = abs(l) + abs(r);
			float multiplier = screenSpaceXSize / 1; // = 1 meter
			float frustumAsymmetryLeft = frustumAsymmetryLeftInMeters * multiplier;
			float frustumAsymmetryRight = frustumAsymmetryRightInMeters * multiplier;

			// now, create the re-projection matrices for both eyes using this frustum asymmetry
			D3DXMatrixPerspectiveOffCenterLH((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvL], l + frustumAsymmetryLeft, r + frustumAsymmetryLeft, b, t, n, f);
			D3DXMatrixPerspectiveOffCenterLH((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvR], l + frustumAsymmetryRight, r + frustumAsymmetryRight, b, t, n, f);

			// create convergence offset matrices without projection
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ConvergenceOffL], frustumAsymmetryLeftInMeters * m_psConfig->fWorldScaleFactor, 0, 0);
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ConvergenceOffR], frustumAsymmetryRightInMeters * m_psConfig->fWorldScaleFactor, 0, 0);
		}
		break;
		case MathRegisters::MAT_Roll:
		case MathRegisters::MAT_RollNegative:
		case MathRegisters::MAT_RollHalf:
			D3DXMatrixRotationZ((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Roll], m_aMathFloat[(size_t)MathFloatFields::Roll]);
			D3DXMatrixRotationZ((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_RollNegative], -m_aMathFloat[(size_t)MathFloatFields::Roll]);
			D3DXMatrixRotationZ((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_RollHalf], m_aMathFloat[(size_t)MathFloatFields::Roll] * .5f);
			break;
		case MathRegisters::MAT_TransformL:
		case MathRegisters::MAT_TransformR:
		case MathRegisters::MAT_ViewProjectionTransNoRollL:
		case MathRegisters::MAT_ViewProjectionTransNoRollR:
		{
			float fSeparation_World = m_aMathFloat[(size_t)MathFloatFields::Separation_World];

			// separation settings are overall (HMD and desktop), since they are based on physical IPD
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL], fSeparation_World * fLEFT_CONSTANT, 0, 0);
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR], fSeparation_World * fRIGHT_CONSTANT, 0, 0);

			// update "no-roll" matrices here before roll is applied eventually
			D3DXMATRIX* psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionTransNoRollL];
			*psOutL = 
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvL]);
			D3DXMATRIX* psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionTransNoRollR];
			*psOutR =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvR]);

			// head roll - only if using translation implementation
			if (m_psConfig->nRollImpl == 1)
			{
				D3DXMatrixMultiply((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL], (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Roll], (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL]);
				D3DXMatrixMultiply((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR], (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Roll], (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR]);
			}
		}
		break;
		case MathRegisters::MAT_ViewProjectionL:
		case MathRegisters::MAT_ViewProjectionR:
			// head roll - only if using translation implementation
			if (m_psConfig->nRollImpl == 1)
			{
				// projection l/r
				D3DXMATRIX* psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionL];
				*psOutL =
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Roll]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvL]);
				D3DXMATRIX* psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionR];
				*psOutR =
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Roll]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvR]);
			}
			else
			{
				// projection l/r
				D3DXMATRIX* psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionL];
				*psOutL =
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvL]);
				D3DXMATRIX* psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionR];
				*psOutR =
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
					D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvR]);
			}
			break;
		case MathRegisters::MAT_ViewProjectionTransL:
		case MathRegisters::MAT_ViewProjectionTransR:
		{
			// projection l/r
			D3DXMATRIX* psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionTransL];
			*psOutL =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvL]);
			D3DXMATRIX* psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_ViewProjectionTransR];
			*psOutR =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionConvR]);
		}
		break;
		case MathRegisters::MAT_Position:
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Position],
				m_aMathRegisters[(size_t)MathRegisters::VEC_PositionTransform].x,
				m_aMathRegisters[(size_t)MathRegisters::VEC_PositionTransform].y,
				m_aMathRegisters[(size_t)MathRegisters::VEC_PositionTransform].z);
			break;
		case MathRegisters::MAT_GatheredL:
		case MathRegisters::MAT_GatheredR:
			// TODO !! MATRIX GATHER METHOD !!
			break;
		case MathRegisters::MAT_HudL:
		case MathRegisters::MAT_HudR:
		case MathRegisters::MAT_GuiL:
		case MathRegisters::MAT_GuiR:
		case MathRegisters::MAT_Squash:
		case MathRegisters::MAT_HudDistance:
		case MathRegisters::MAT_Hud3dDepthL:
		case MathRegisters::MAT_Hud3dDepthR:
		case MathRegisters::MAT_Hud3dDepthShiftL:
		case MathRegisters::MAT_Hud3dDepthShiftR:
		case MathRegisters::MAT_Gui3dDepthL:
		case MathRegisters::MAT_Gui3dDepthR:
		{
			// squash
			float fSquash = m_aMathFloat[(size_t)MathFloatFields::Squash];
			D3DXMatrixScaling((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Squash], fSquash, fSquash, 1);

			// hudDistance
			float fHudDistance = m_aMathFloat[(size_t)MathFloatFields::HUD_Distance];
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_HudDistance], 0, 0, fHudDistance);

			// hud3DDepth
			float fHud3DDepth = m_aMathFloat[(size_t)MathFloatFields::HUD_Depth];
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthL], fHud3DDepth, 0, 0);
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthR], -fHud3DDepth, 0, 0);
			float fAdditionalSeparation = (1.5f - fHudDistance) * m_aMathFloat[(size_t)MathFloatFields::LensXCenterOffset];
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthShiftL], fHud3DDepth + fAdditionalSeparation, 0, 0);
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthShiftR], -fHud3DDepth - fAdditionalSeparation, 0, 0);

			// gui3DDepth
			float fGui3DDepth = m_aMathFloat[(size_t)MathFloatFields::GUI_Depth] + m_aMathFloat[(size_t)MathFloatFields::Separation_IPDAdjustment];
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Gui3dDepthL], fGui3DDepth, 0, 0);
			D3DXMatrixTranslation((D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_Gui3dDepthR], -fGui3DDepth, 0, 0);

			// gui/hud matrices - Just use the default projection not the PFOV
			D3DXMATRIX* psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_HudL];
			*psOutL =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthL]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformL]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_HudDistance]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection]);
			D3DXMATRIX* psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_HudR];
			*psOutR =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Hud3dDepthR]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_TransformR]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_HudDistance]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection]);
			psOutL = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_GuiL];
			*psOutL =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Gui3dDepthL]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Squash]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection]);
			psOutR = (D3DXMATRIX*)&m_aMathRegisters[(size_t)MathRegisters::MAT_GuiR];
			*psOutR =
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_ProjectionInv]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Gui3dDepthR]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_Squash]) *
				D3DXMATRIX(m_aMathRegisters[(size_t)MathRegisters::MAT_BasicProjection]);
		}
		break;
		default:
			break;
		}
	}

	/// <summary>
	/// Retrieves an internal float value.
	/// </summary>
	/// <param name="eIndex">Index based on (MathFloatFields enum)</param>
	/// <returns>Current value of specified index</returns>
	float GetFloat(MathFloatFields eIndex)
	{
		return m_aMathFloat[(size_t)eIndex];
	}

	/// <summary>
	/// Retrieves an internal vector value.
	/// </summary>
	/// <param name="eRegister">Register index (MathRegisters enum)</param>
	/// <returns>Current value (field) of specified index</returns>
	D3DXVECTOR4 Get(MathRegisters eRegister)
	{
		return D3DXVECTOR4((float*)&m_aMathRegisters[(size_t)eRegister]);
	}

	/// <summary>
	/// Retrieves an internal matrix value.
	/// </summary>
	/// <param name="eRegister">Register index (MathRegisters enum)</param>
	/// <returns>Current value (field) of specified index</returns>
	D3DXMATRIX Get(MathRegisters eRegister, const unsigned uSize)
	{
		// out of range ?
		if ((uSize != 4) || ((uSize + 4) > (unsigned)Math_Registers_Size)) return D3DXMATRIX();
		return D3DXMATRIX((float*)&m_aMathRegisters[(size_t)eRegister]);
	}

private:
	/// <summary> All mathematical registers needed to compute any modification. </summary>
	std::array<REGISTER4F, Math_Registers_Size> m_aMathRegisters;
	/// <summary> All float fields needed to compute any modification. </summary>
	std::array<float, Math_FloatFields_Size> m_aMathFloat;
	/// <summary>Vireio v4.x game configuration.</summary>
	Vireio_GameConfiguration* m_psConfig;
};
#pragma endregion

#pragma region /// => Shader constant modification
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
	ShaderConstantModification(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculaion) : m_ModificationID(uModID), m_pcCalculation(pcCalculaion) {}
	virtual ~ShaderConstantModification() {}

	/// <summary> Pure virtual method, should apply the modification to produce left and right versions </summary>
	virtual void ApplyModification(const T* inData, std::array<T, 4>* outLeft, std::array<T, 4>* outRight) = 0;
	/// <summary> Pure virtual method, should apply the modification to produce left and right versions </summary>
	virtual void ApplyModification(const T* inData, std::array<T, 16>* outLeft, std::array<T, 16>* outRight) = 0;

	/// <summary> Simply a way to identify this modification. Useful for comparing shadermodification equality </summary>
	UINT m_ModificationID;

	/// <summary> Calculation class. </summary>
	std::shared_ptr<ModificationCalculation> m_pcCalculation;
};

/// <summary>
/// Vector modification base.
/// </summary>
class ShaderVectorModification : public ShaderConstantModification<float>
{
public:
	/// <summary>
	/// Same constructor as base class.
	/// </summary>
	ShaderVectorModification(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation) : ShaderConstantModification(uModID, pcCalculation) {}

	virtual void ApplyModification(const float* inData, std::array<float, 4>* outLeft, std::array<float, 4>* outRight)
	{
		D3DXVECTOR4 sIn(inData);

		(D3DXVECTOR4)outLeft->data() = sIn + m_pcCalculation->Get(MathRegisters::VEC_PositionTransform) * fLEFT_CONSTANT;
		(D3DXVECTOR4)outRight->data() = sIn + m_pcCalculation->Get(MathRegisters::VEC_PositionTransform) * fRIGHT_CONSTANT;
	}
	virtual void ApplyModification(const float* inData, std::array<float, 16>* outLeft, std::array<float, 16>* outRight) {};

};

/// <summary>
/// => Matrix modification base.
/// </summary>
class ShaderMatrixModification : public ShaderConstantModification<float>
{
public:

	/// <summary>
	/// Same constructor as base class.
	/// </summary>
	ShaderMatrixModification(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation) : ShaderConstantModification(uModID, pcCalculation) {}

	/// <summary>
	/// Apply projection transform matrix here.
	/// </summary>
	/// <param name="inData">Input data (D3DMATRIX)</param>
	/// <param name="outLeft">Left output (matrix as float array)</param>
	/// <param name="outRight">Right output (matrix as float array)</param>
	virtual void ApplyModification(const float* inData, std::array<float, 16>* outLeft, std::array<float, 16>* outRight)
	{
		D3DXMATRIX sIn = D3DXMATRIX(inData);
		D3DXMATRIX* psOutLeft = (D3DXMATRIX*)&outLeft[0];
		D3DXMATRIX* psOutRight = (D3DXMATRIX*)&outRight[0];
		*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransL, 4);
		*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransR, 4);
	}
	virtual void ApplyModification(const float* inData, std::array<float, 4>* outLeft, std::array<float, 4>* outRight) {};
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
#pragma endregion

