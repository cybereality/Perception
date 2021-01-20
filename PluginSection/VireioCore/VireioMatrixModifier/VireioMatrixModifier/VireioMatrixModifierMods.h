/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifierMods.h> :
Copyright (C) 2021 Denis Reischl

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

#include<string>
#include<map>
#include"VireioMatrixModifierClasses.h"

#pragma region /// => macros
#define _map(mapname) struct mapname : std::map<unsigned int, std::string> { ~mapname() {}; mapname()
#define _map_elem(enumname, elem) this->operator[]((unsigned)enumname::elem) = #elem;
#define _map_elem_txt(enumname, elem, txt) this->operator[]((unsigned)enumname::elem) = std::string(txt);
#define _map_  };
#define _matrix_mod(modname) class modname : public ShaderMatrixModification { public: modname(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation) : ShaderMatrixModification(uModID, pcCalculation) {}; \
	virtual void ApplyModification(const float* inData, std::array<float, 16>* outLeft, std::array<float, 16>* outRight) \
	{ D3DXMATRIX sIn = D3DXMATRIX(inData); D3DXMATRIX* psOutLeft = (D3DXMATRIX*)&outLeft[0]; D3DXMATRIX* psOutRight = (D3DXMATRIX*)&outRight[0];
#define _matrix_mod_derive(modname, parentname) class modname : public parentname { public:  modname(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation) : parentname(uModID, pcCalculation) {}; \
	virtual void ApplyModification(const float* inData, std::array<float, 16>* outLeft, std::array<float, 16>* outRight) \
	{ D3DXMATRIX sIn = D3DXMATRIX(inData); D3DXMATRIX* psOutLeft = (D3DXMATRIX*)&outLeft[0]; D3DXMATRIX* psOutRight = (D3DXMATRIX*)&outRight[0];
#define _matrix_mod_ } };
#pragma endregion

#pragma region /// => enumerations/maps
/// <summary>
/// Matrix modification identifiers.
/// </summary>
enum struct MatrixModificationTypes : unsigned
{
	MX_DoNothing = 0,                   /**< Simple modification that does not apply anything. **/
	MX_SimpleTranslate = 1,             /**< Default modification is simple translate. **/
	MX_OrthographicSquash = 2,          /**< Squashes matrix if orthographic, otherwise simple translate. **/
	MX_HudSlide = 3,                    /**< Modification to slide the head up display (HUD) into the head mounted display (HMD) output.  **/
	MX_GuiSquash = 4,                   /**< Modification to squash the graphical user interface (GUI). **/
	MX_SurfaceRefractionTransform = 5,  /**< Modification to fix surface refraction in pixel shaders. **/
	MX_GatheredOrthographicSquash = 6,  /**< Squashes matrix if orthographic, otherwise simple translate. Result will be gathered to be used in other modifications.**/
	MX_OrthographicSquashShifted = 7,   /**< Squashes matrix if orthographic, otherwise simple translate - shift accordingly. **/
	MX_OrthographicSquashHud = 8,       /**< Squashes matrix if orthographic, otherwise simple translate - matrices treated as beeing for HUD. **/
	MX_ConvergenceOffset = 9,           /**< Fixes far away objects using the convergence offset. **/
	MX_SimpleTranslateIgnoreOrtho = 10, /**< Modification to ignore orthographic matrices. **/
	MX_RollOnly = 11,                   /**< Modification applies only the head roll. **/
	MX_RollOnlyNegative = 12,           /**< Modification applies only the head roll. (negative)**/
	MX_RollOnlyHalf = 13,               /**< Modification applies only the head roll. (half roll)**/
	MX_NoRoll = 14,                     /**< Default modification without head roll. **/
	MX_SimpleTranslateNoPositional = 15,/**< Simple translate, but is not affected by positional tracking **/
	MX_NoStereoSeparate = 16,           /**< Don't translate, but is still affected by positional tracking **/
	MX_Roll = 17,                       /**< Head roll only without projection. **/
	MX_RollNegative = 18,               /**< Head roll negative only without projection. **/
	MX_RollConvergence = 19,            /**< Head roll and convergence offset without projection. **/
	MX_RollNegativeConvergence = 20,    /**< Head roll negative and convergence offset without projection. **/
	MX_TransformToRotation = 21,        /**< Applies transform to rotation matrices. **/
};
constexpr unsigned uMatrixModificationNumber = 22;

/// <summary>
/// Matrix modification identifiers (string map).
/// </summary>
_map(MatrixModificationMap)
{
	_map_elem(MatrixModificationTypes, MX_DoNothing);
	_map_elem(MatrixModificationTypes, MX_SimpleTranslate);
	_map_elem(MatrixModificationTypes, MX_OrthographicSquash);
	_map_elem(MatrixModificationTypes, MX_HudSlide);
	_map_elem(MatrixModificationTypes, MX_GuiSquash);
	_map_elem(MatrixModificationTypes, MX_SurfaceRefractionTransform);
	_map_elem(MatrixModificationTypes, MX_GatheredOrthographicSquash);
	_map_elem(MatrixModificationTypes, MX_OrthographicSquashShifted);
	_map_elem(MatrixModificationTypes, MX_OrthographicSquashHud);
	_map_elem(MatrixModificationTypes, MX_ConvergenceOffset);
	_map_elem(MatrixModificationTypes, MX_SimpleTranslateIgnoreOrtho);
	_map_elem(MatrixModificationTypes, MX_RollOnly);
	_map_elem(MatrixModificationTypes, MX_RollOnlyNegative);
	_map_elem(MatrixModificationTypes, MX_RollOnlyHalf);
	_map_elem(MatrixModificationTypes, MX_NoRoll);
	_map_elem(MatrixModificationTypes, MX_SimpleTranslateNoPositional);
	_map_elem(MatrixModificationTypes, MX_NoStereoSeparate);
	_map_elem(MatrixModificationTypes, MX_Roll);
	_map_elem(MatrixModificationTypes, MX_RollNegative);
	_map_elem(MatrixModificationTypes, MX_RollConvergence);
	_map_elem(MatrixModificationTypes, MX_RollNegativeConvergence);
	_map_elem(MatrixModificationTypes, MX_TransformToRotation);
};
_map_;

/// <summary>
/// Matrix modification identifiers descriptions (string map).
/// </summary>
_map(MatrixModificationDescMap)
{
	_map_elem_txt(MatrixModificationTypes, MX_DoNothing, "Simple modification that does not apply anything.");
	_map_elem_txt(MatrixModificationTypes, MX_SimpleTranslate, "Default modification is simple translate.");
	_map_elem_txt(MatrixModificationTypes, MX_OrthographicSquash, "Squashes matrix if orthographic, otherwise simple translate.");
	_map_elem_txt(MatrixModificationTypes, MX_HudSlide, "Modification to slide the head up display (HUD) into the head mounted display (HMD) output.");
	_map_elem_txt(MatrixModificationTypes, MX_GuiSquash, "Modification to squash the graphical user interface (GUI).");
	_map_elem_txt(MatrixModificationTypes, MX_SurfaceRefractionTransform, "Modification to fix surface refraction in pixel shaders.");
	_map_elem_txt(MatrixModificationTypes, MX_GatheredOrthographicSquash, "Squashes matrix if orthographic, otherwise simple translate. Result will be gathered to be used in other modifications.");
	_map_elem_txt(MatrixModificationTypes, MX_OrthographicSquashShifted, "Squashes matrix if orthographic, otherwise simple translate - shift accordingly.");
	_map_elem_txt(MatrixModificationTypes, MX_OrthographicSquashHud, "Squashes matrix if orthographic, otherwise simple translate - matrices treated as beeing for HUD.");
	_map_elem_txt(MatrixModificationTypes, MX_ConvergenceOffset, "Fixes far away objects using the convergence offset.");
	_map_elem_txt(MatrixModificationTypes, MX_SimpleTranslateIgnoreOrtho, "Modification to ignore orthographic matrices.");
	_map_elem_txt(MatrixModificationTypes, MX_RollOnly, "Modification applies only the head roll.");
	_map_elem_txt(MatrixModificationTypes, MX_RollOnlyNegative, "Modification applies only the head roll. (negative)");
	_map_elem_txt(MatrixModificationTypes, MX_RollOnlyHalf, "Modification applies only the head roll. (half roll)");
	_map_elem_txt(MatrixModificationTypes, MX_NoRoll, "Default modification without head roll.");
	_map_elem_txt(MatrixModificationTypes, MX_SimpleTranslateNoPositional, "Simple translate, but is not affected by positional tracking");
	_map_elem_txt(MatrixModificationTypes, MX_NoStereoSeparate, "Don't translate, but is still affected by positional tracking");
	_map_elem_txt(MatrixModificationTypes, MX_Roll, "Head roll only without projection.");
	_map_elem_txt(MatrixModificationTypes, MX_RollNegative, "Head roll negative only without projection. ");
	_map_elem_txt(MatrixModificationTypes, MX_RollConvergence, "Head roll and convergence offset without projection.");
	_map_elem_txt(MatrixModificationTypes, MX_RollNegativeConvergence, "Head roll negative and convergence offset without projection.");
	_map_elem_txt(MatrixModificationTypes, MX_TransformToRotation, "Applies transform to rotation matrices.");
};
_map_;
#pragma endregion

#pragma region /// => modifications
/// <summary>
/// Simple modification that does not apply anything. Is needed for certain methods that could do things but sometimes won't.
/// </summary>
_matrix_mod(MatrixDoNothing)
{
	*psOutLeft = sIn;
	*psOutRight = sIn;
}
_matrix_mod_

/// <summary>
/// Squishes the matrix if orthographical, otherwise simple modification.
/// </summary>
_matrix_mod(MatrixOrthoSquash)
{
	if (fabs(sIn[15] - 1.0f) < 0.00001f)
	{
		// add all translation and scale matrix entries 
		// (for the GUI this should be 3.0f, for the HUD above)
		float allAbs = abs(sIn(3, 0)) + abs(sIn(3, 1)) + abs(sIn(3, 2)); // trans x,y,z
		allAbs += abs(sIn(0, 0)) + abs(sIn(1, 1)) + abs(sIn(2, 2));      // scale x,y,z

		// HUD
		if (allAbs > 3.0f)
		{
			// separation -> distance translation
			*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudL, 4);
			*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudR, 4);
		}
		else // GUI
		{
			// simple squash
			*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_GuiL, 4);
			*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_GuiR, 4);
		}
	}
	else
		ShaderMatrixModification::ApplyModification(inData, outLeft, outRight);
}
_matrix_mod_

/// <summary>
/// Slides the HUD into the HMD output.
/// All orthographic matrices treated as HUD here.
/// </summary>
_matrix_mod(MatrixHudSlide)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudR, 4);
}
_matrix_mod_


/// <summary>
/// Matrix implementation whose purpose is to squash the GUI acordingly to the driver settings. 
/// </summary>
_matrix_mod(MatrixGuiSquash)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_GuiL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_GuiR, 4);
}
_matrix_mod_

/// <summary>
/// Squishes the matrix if orthographical, otherwise simple modification.
/// </summary>
_matrix_mod(MatrixSurfaceRefractionTransform)
{
	constexpr D3DMATRIX sTranslateUV = {
	1.0f,            0.0f,            0.0f,            0.0f,
	0.0f,            1.0f,            0.0f,            0.0f,
	0.0f,            0.0f,            1.0f,            0.0f,
	0.5f,            0.5f,            0.5f,            1.0f
	};
	constexpr D3DMATRIX sScaleUV = {
	0.5f,            0.0f,            0.0f,            0.0f,
	0.0f,            0.5f,            0.0f,            0.0f,
	0.0f,            0.0f,            0.5f,            0.0f,
	0.0f,            0.0f,            0.0f,            1.0f
	};

	*psOutLeft = m_pcCalculation->Get(MathRegisters::MAT_GatheredL, 4) * sScaleUV * sTranslateUV;
	*psOutRight = m_pcCalculation->Get(MathRegisters::MAT_GatheredR, 4) * sScaleUV * sTranslateUV;;
}
_matrix_mod_

/// <summary>
/// Squishes the matrix if orthographical, otherwise simple modification. Gathers the output matrices.
/// </summary>
_matrix_mod_derive(MatrixGatheredOrthoSquash, MatrixOrthoSquash)
{
	MatrixOrthoSquash::ApplyModification(inData, outLeft, outRight);

	// gather matrix /// TODO !!!
	// m_pcCalculation->GatherMatrix(outLeft, outRight);

	*psOutLeft = sIn;
	*psOutRight = sIn;
}
_matrix_mod_

/// <summary>
/// Squishes the matrix if orthographical, otherwise simple modification.
/// HUD matrices shifted here accordingly to HUD distance.
/// </summary>
_matrix_mod(MatrixOrthoSquashShifted)
{
	if (fabs(sIn[15] - 1.0f) < 0.00001f)
	{
		// add all translation and scale matrix entries 
		// (for the GUI this should be 3.0f, for the HUD above)
		float allAbs = abs(sIn(3, 0)) + abs(sIn(3, 1)) + abs(sIn(3, 2)); // trans x,y,z
		allAbs += abs(sIn(0, 0)) + abs(sIn(1, 1)) + abs(sIn(2, 2));      // scale x,y,z

		// HUD
		if (allAbs > 3.0f)
		{
			// separation -> distance translation
			D3DXMATRIX sOrthoToPersViewProjTransformLeft = m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Hud3dDepthShiftL, 4) *
				m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransL, 4) * m_pcCalculation->Get(MathRegisters::MAT_HudDistance, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
			D3DXMATRIX sOrthoToPersViewProjTransformRight = m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Hud3dDepthShiftR, 4) *
				m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransR, 4) * m_pcCalculation->Get(MathRegisters::MAT_HudDistance, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);

			// separation -> distance translation
			*psOutLeft = sIn * sOrthoToPersViewProjTransformLeft;
			*psOutRight = sIn * sOrthoToPersViewProjTransformRight;
		}
		else // GUI
		{
			// simple squash
			*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Gui3dDepthL, 4) * m_pcCalculation->Get(MathRegisters::MAT_Squash, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
			*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Gui3dDepthR, 4) * m_pcCalculation->Get(MathRegisters::MAT_Squash, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
		}
	}
	else
		ShaderMatrixModification::ApplyModification(inData, outLeft, outRight);
}
_matrix_mod_

/// <summary>
/// Squishes the matrix if orthographical, otherwise simple modification.
/// All orthographic matrices treated as HUD here.
/// </summary>
_matrix_mod(MatrixOrthoSquashHud)
{
	if (fabs(sIn[15] - 1.0f) < 0.00001f)
	{

		// HUD : separation -> distance translation
		*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudL, 4);
		*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_HudR, 4);

	}
	else
		ShaderMatrixModification::ApplyModification(inData, outLeft, outRight);

	*psOutLeft = sIn;
	*psOutRight = sIn;
}
_matrix_mod_

/// <summary>
/// Convergence Offset adjustment.
/// </summary>
_matrix_mod(MatrixConvOffsetAdjustment)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionR, 4);
}
_matrix_mod_

/// <summary>
/// Modification to ignore orthographic matrices.
/// This is the intent, not sure that the math holds up but it works for HL2.
/// (Doesn't seem to work for omd2, stops all separation)
/// </summary>
_matrix_mod(MatrixIgnoreOrtho)
{
	if (fabs(sIn[15] - 1.0f) < 0.00001f)
	{
		*psOutLeft = sIn;
		*psOutRight = sIn;
	}
	else
		ShaderMatrixModification::ApplyModification(inData, outLeft, outRight);
}
_matrix_mod_

/// <summary>
/// Matrix applies only the head roll.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixRollOnly)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies only the head roll.
/// For shadow fixes. 
/// </summary>
_matrix_mod(MatrixRollOnlyNegative)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies only the half head roll.
/// For shadow fixes.  
/// </summary>
_matrix_mod(MatrixRollOnlyHalf)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_RollHalf, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_RollHalf, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies modification without head roll.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixNoRoll)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransNoRollL, 4) * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Position, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransNoRollR, 4) * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Position, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
}
_matrix_mod_

/// <summary>
/// Simple translate modification that is unaffected by positional tracking.
/// </summary>
_matrix_mod(MatrixNoPositional)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ViewProjectionTransR, 4);
}
_matrix_mod_

/// <summary>
/// Simple modification that is affected by positional tracking only (no stereo separation)
/// </summary>
_matrix_mod(MatrixNoStereoSeparate)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Position, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_ProjectionInv, 4) * m_pcCalculation->Get(MathRegisters::MAT_Position, 4) * m_pcCalculation->Get(MathRegisters::MAT_BasicProjection, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies head roll.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixRoll)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies head roll negative.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixRollNegative)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies head roll and convergence offset.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixRollConvergence)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4) * m_pcCalculation->Get(MathRegisters::MAT_ConvergenceOffL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_Roll, 4) * m_pcCalculation->Get(MathRegisters::MAT_ConvergenceOffR, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies head roll negative and convergence offset.
/// For shadow fixes.
/// </summary>
_matrix_mod(MatrixRollNegativeConvergence)
{
	*psOutLeft = sIn * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4) * m_pcCalculation->Get(MathRegisters::MAT_ConvergenceOffL, 4);
	*psOutRight = sIn * m_pcCalculation->Get(MathRegisters::MAT_RollNegative, 4) * m_pcCalculation->Get(MathRegisters::MAT_ConvergenceOffR, 4);
}
_matrix_mod_

/// <summary>
/// Matrix applies transform only if no member of the first input row is zero.
/// </summary>
_matrix_mod(MatrixTransformToRotation)
{
	if ((sIn(0, 0) == 0.0f) || (sIn(0, 1) == 0.0f) || (sIn(0, 2) == 0.0f) || (sIn(0, 3) == 0.0f))
	{
		*psOutLeft = sIn;
		*psOutRight = sIn;
	}
	else
		ShaderMatrixModification::ApplyModification(inData, outLeft, outRight);
}
_matrix_mod_
#pragma endregion

#pragma region /// => modification factory
/// <summary>
/// Creates a vector (= 1 register) modification.
/// </summary>
/// <param name="uModID">Modification Identifier</param>
/// <param name="pcCalculation">Elements Calculation class</param>
/// <returns>shared pointer to modification</returns>
static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation)
{
	return std::make_shared<ShaderVectorModification>(uModID, pcCalculation);
}

/// <summary>
/// Creates a matrix (= 4 register) modification.
/// </summary>
/// <param name="uModID">Modification Identifier</param>
/// <param name="pcCalculation">Elements Calculation class</param>
/// <returns>shared pointer to modification</returns>
static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(unsigned uModID, std::shared_ptr<ModificationCalculation> pcCalculation)
{
	switch ((MatrixModificationTypes)uModID)
	{
	case MatrixModificationTypes::MX_DoNothing:
		return std::make_shared<MatrixDoNothing>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_SimpleTranslate:
		return std::make_shared<ShaderMatrixModification>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_OrthographicSquash:
		return std::make_shared<MatrixOrthoSquash>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_HudSlide:
		return std::make_shared<MatrixHudSlide>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_GuiSquash:
		return std::make_shared<MatrixGuiSquash>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_SurfaceRefractionTransform:
		return std::make_shared<MatrixSurfaceRefractionTransform>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_GatheredOrthographicSquash:
		return std::make_shared<MatrixGatheredOrthoSquash>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_OrthographicSquashShifted:
		return std::make_shared<MatrixOrthoSquashShifted>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_OrthographicSquashHud:
		return std::make_shared<MatrixOrthoSquashHud>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_ConvergenceOffset:
		return std::make_shared<MatrixConvOffsetAdjustment>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_SimpleTranslateIgnoreOrtho:
		return std::make_shared<MatrixIgnoreOrtho>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollOnly:
		return std::make_shared<MatrixRollOnly>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollOnlyNegative:
		return std::make_shared<MatrixRollOnlyNegative>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollOnlyHalf:
		return std::make_shared<MatrixRollOnlyHalf>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_NoRoll:
		return std::make_shared<MatrixNoRoll>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_SimpleTranslateNoPositional:
		return std::make_shared<MatrixNoPositional>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_NoStereoSeparate:
		return std::make_shared<MatrixNoStereoSeparate>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_Roll:
		return std::make_shared<MatrixRoll>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollNegative:
		return std::make_shared<MatrixRollNegative>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollConvergence:
		return std::make_shared<MatrixRollConvergence>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_RollNegativeConvergence:
		return std::make_shared<MatrixRollNegativeConvergence>(uModID, pcCalculation);
	case MatrixModificationTypes::MX_TransformToRotation:
		return std::make_shared<MatrixTransformToRotation>(uModID, pcCalculation);
	default:
		OutputDebugStringA("[MAM] Nonexistant matrix modification\n");
		assert(false);
		throw std::out_of_range("[MAM] Nonexistant matrix modification");
		return nullptr;
	}
}

/// <summary>
/// Returns the name of the modification (std::string).
/// </summary>
/// <param name="unModID">Modification Identifier</param>
/// <returns>name string</returns>
static std::string GetMatrixModificationName(unsigned uModID)
{
	static MatrixModificationMap aMatrixModificationMap;
	return aMatrixModificationMap[uModID];
}

/// <summary>
/// Returns the description of the modification (std::string).
/// </summary>
/// <param name="unModID">Modification Identifier</param>
/// <returns>name string</returns>
static std::string GetMatrixModificationDesc(unsigned uModID)
{
	static MatrixModificationDescMap aMatrixModificationDescMap;
	return aMatrixModificationDescMap[uModID];
}

#pragma endregion

