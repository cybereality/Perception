/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderConstantModificationFactory.h> and
Class <ShaderConstantModificationFactory> :
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

#ifndef SHADERCONSTANTMODIFICATIONFACTORY_H_INCLUDED
#define SHADERCONSTANTMODIFICATIONFACTORY_H_INCLUDED

#pragma region include
#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "ShaderMatrixModification.h"
#include "Vector4SimpleTranslate.h"
#include "Vector4EyeShiftUnity.h"
#include "Vector4DeadIslandScaled.h"
#include "MatrixIgnoreOrtho.h"
#include "MatrixDoNothing.h"
#include "MatrixHudSlide.h"
#include "MatrixGuiSquash.h"
#include "MatrixOrthoSquash.h"
#include "MatrixOrthoSquashHud.h"
#include "MatrixOrthoSquashShifted.h"
#include "MatrixSurfaceRefractionTransform.h"
#include "MatrixGatheredOrhoSquash.h"
#include "MatrixConvOffsetAdjustment.h"
#include "MatrixRollOnly.h"
#include "MatrixRollOnlyNegative.h"
#include "MatrixRollOnlyHalf.h"
#include "MatrixNoRoll.h"
#include "MatrixNoPositional.h"
#include "MatrixNoStereoSeparate.h"
#include "MatrixRoll.h"
#include "MatrixRollNegative.h"
#include "MatrixRollConvergence.h"
#include "MatrixRollNegativeConvergence.h"
#include "MatrixShadowFix1.h"
#include "MatrixShadowFix2.h"
#include "MatrixTransformShadowFix1.h"
#include "MatrixTransformToRotation.h"
#include "MatrixTransformToRotationNegative.h"
#pragma endregion

/**
* Shader constant modification helper class.
* Contains static functions to create modifications and modification enumerations.
*/
class ShaderConstantModificationFactory
{
public:
	/**
	* Vector4 modification identifiers.
	***/
	enum Vector4ModificationTypes
	{
		Vec4DoNothing = 0,                 /**< Simple modification that does not apply anything. **/
		Vec4SimpleTranslate = 1,           /**< Default modification is simple translate. **/
		Vec4EyeShiftUnity = 2,             /**< **/
		Vec4DeadIslandScaled = 3           /**< **/
	};
	static const UINT m_unVector4ModificationNumber = 4;
	/**
	* Matrix modification identifiers.
	***/
	enum MatrixModificationTypes
	{
		MatDoNothing = 0,                   /**< Simple modification that does not apply anything. **/
		MatSimpleTranslate = 1,             /**< Default modification is simple translate. **/
		MatOrthographicSquash = 2,          /**< Squashes matrix if orthographic, otherwise simple translate. **/
		MatHudSlide = 3,                    /**< Modification to slide the head up display (HUD) into the head mounted display (HMD) output.  **/
		MatGuiSquash = 4,                   /**< Modification to squash the graphical user interface (GUI). **/
		MatSurfaceRefractionTransform = 5,  /**< Modification to fix surface refraction in pixel shaders. **/
		MatGatheredOrthographicSquash = 6,  /**< Squashes matrix if orthographic, otherwise simple translate. Result will be gathered to be used in other modifications.**/
		MatOrthographicSquashShifted = 7,   /**< Squashes matrix if orthographic, otherwise simple translate - shift accordingly. **/
		MatOrthographicSquashHud = 8,       /**< Squashes matrix if orthographic, otherwise simple translate - matrices treated as beeing for HUD. **/
		MatConvergenceOffset = 9,           /**< Fixes far away objects using the convergence offset. **/
		MatSimpleTranslateIgnoreOrtho = 10, /**< Modification to ignore orthographic matrices. **/
		MatRollOnly = 11,                   /**< Modification applies only the head roll. **/
		MatRollOnlyNegative = 12,           /**< Modification applies only the head roll. (negative)**/
		MatRollOnlyHalf = 13,               /**< Modification applies only the head roll. (half roll)**/
		MatNoRoll = 14,                     /**< Default modification without head roll. **/
		MatSimpleTranslateNoPositional = 15,/**< Simple translate, but is not affected by positional tracking **/
		MatNoStereoSeparate = 16,           /**< Don't translate, but is still affected by positional tracking **/
		MatRoll = 17,                       /**< Head roll only without projection. **/
		MatRollNegative = 18,               /**< Head roll negative only without projection. **/
		MatRollConvergence = 19,            /**< Head roll and convergence offset without projection. **/
		MatRollNegativeConvergence = 20,    /**< Head roll negative and convergence offset without projection. **/
		MatShadowFix1 = 21,                 /**< Reserved **/
		MatShadowFix2 = 22,                 /**< Reserved **/
		MatShadowFix3 = 23,                 /**< Reserved **/
		MatShadowFix4 = 24,                 /**< Reserved **/
		MatShadowFix5 = 25,                 /**< Reserved **/
		MatShadowFix6 = 26,                 /**< Reserved **/
		MatShadowFix7 = 27,                 /**< Reserved **/
		MatShadowFix8 = 28,                 /**< Reserved **/
		MatTransformShadowFix1 = 29,        /**< Reserved **/
		MatTransformShadowFix2 = 30,        /**< Reserved **/
		MatTransformShadowFix3 = 31,        /**< Reserved **/
		MatTransformShadowFix4 = 32,        /**< Reserved **/
		MatTransformShadowFix5 = 33,        /**< Reserved **/
		MatTransformShadowFix6 = 34,        /**< Reserved **/
		MatTransformShadowFix7 = 35,        /**< Reserved **/
		MatTransformShadowFix8 = 36,        /**< Reserved **/
		MatTransformToRotation = 37,        /**< Applies transform to rotation matrices. **/
	};
	static const UINT m_unMatrixModificationNumber = 38;

	/**
	* Calls twin function.
	***/
	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices)
	{
		return CreateVector4Modification(static_cast<Vector4ModificationTypes>(modID), adjustmentMatrices);
	}

	/**
	* Creates Vector4 modification.
	* @see Vector4SimpleTranslate
	***/
	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(Vector4ModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices)
	{
		switch (mod)
		{
			case Vec4SimpleTranslate:
				return std::make_shared<Vector4SimpleTranslate>(mod, adjustmentMatrices);
			case Vec4EyeShiftUnity:
				return std::make_shared<Vector4EyeShiftUnity>(mod, adjustmentMatrices);
			case Vec4DeadIslandScaled:
				return std::make_shared<Vector4DeadIslandScaled>(mod, adjustmentMatrices);
			default:
				OutputDebugStringA("Nonexistant Vec4 modification\n");
				assert(false);
				throw std::out_of_range("Nonexistant Vec4 modification");
		}
	}
	/**
	* Returns the name of the modification (std::wstring).
	***/
	static std::wstring GetVector4ModificationName(UINT unModID)
	{
		switch (unModID)
		{
			case Vec4SimpleTranslate:
				return std::wstring(L"Vec4SimpleTranslate");
			case Vec4EyeShiftUnity:
				return std::wstring(L"Vec4EyeShiftUnity");
			case Vec4DeadIslandScaled:
				return std::wstring(L"Vec4DeadIslandScaled");
		}
		return std::wstring(L"Error-Nonexistant");
	}
	/**
	* Calls twin function.
	***/
	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
	{
		return CreateMatrixModification(static_cast<MatrixModificationTypes>(modID), adjustmentMatrices, transpose);
	}
	/**
	* Creates matrix modification.
	* @see MatrixDoNothing
	* @see ShaderMatrixModification
	* @see MatrixIgnoreOrtho
	* @see MatrixGuiSquash
	***/
	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(MatrixModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
	{
		switch (mod)
		{
			case ShaderConstantModificationFactory::MatDoNothing:
				return std::make_shared<MatrixDoNothing>(mod, adjustmentMatrices);
			case ShaderConstantModificationFactory::MatSimpleTranslate:
				return std::make_shared<ShaderMatrixModification>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatOrthographicSquash:
				return std::make_shared<MatrixOrthoSquash>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatHudSlide:
				return std::make_shared<MatrixHudSlide>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatGuiSquash:
				return std::make_shared<MatrixGuiSquash>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatSurfaceRefractionTransform:
				return std::make_shared<MatrixSurfaceRefractionTransform>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatGatheredOrthographicSquash:
				return std::make_shared<MatrixGatheredOrthoSquash>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatOrthographicSquashShifted:
				return std::make_shared<MatrixOrthoSquashShifted>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatOrthographicSquashHud:
				return std::make_shared<MatrixOrthoSquashHud>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatConvergenceOffset:
				return std::make_shared<MatrixConvOffsetAdjustment>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatSimpleTranslateIgnoreOrtho:
				return std::make_shared<MatrixIgnoreOrtho>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatRollOnly:
				return std::make_shared<MatrixRollOnly>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatRollOnlyNegative:
				return std::make_shared<MatrixRollOnlyNegative>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatRollOnlyHalf:
				return std::make_shared<MatrixRollOnlyHalf>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatNoRoll:
				return std::make_shared<MatrixNoRoll>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatSimpleTranslateNoPositional:
				return std::make_shared<MatrixNoPositional>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatNoStereoSeparate:
				return std::make_shared<MatrixNoStereoSeparate>(mod, adjustmentMatrices, transpose);
			case ShaderConstantModificationFactory::MatRoll:
				return std::make_shared<MatrixRoll>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatRollNegative:
				return std::make_shared<MatrixRollNegative>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatRollConvergence:
				return std::make_shared<MatrixRollConvergence>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatRollNegativeConvergence:
				return std::make_shared<MatrixRollNegativeConvergence>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatShadowFix1:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatShadowFix2:
				return std::make_shared<MatrixShadowFix2>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatShadowFix3:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatShadowFix4:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatShadowFix5:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatShadowFix6:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatShadowFix7:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatShadowFix8:
				return std::make_shared<MatrixShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix1:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose);
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix2:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix3:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix4:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix5:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix6:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix7:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformShadowFix8:
				return std::make_shared<MatrixTransformShadowFix1>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			case ShaderConstantModificationFactory::MatTransformToRotation:
				return std::make_shared<MatrixTransformToRotation>(mod, adjustmentMatrices, transpose); // RESERVED
				break;
			default:
				OutputDebugStringA("Nonexistant matrix modification\n");
				assert(false);
				throw std::out_of_range("Nonexistant matrix modification");
		}
	}
	/**
	* Returns the name of the modification (std::wstring).
	***/
	static std::wstring GetMatrixModificationName(UINT unModID)
	{
		switch (unModID)
		{
			case ShaderConstantModificationFactory::MatDoNothing:
				return std::wstring(L"MatDoNothing");
			case ShaderConstantModificationFactory::MatSimpleTranslate:
				return std::wstring(L"MatSimpleTranslate");
			case ShaderConstantModificationFactory::MatOrthographicSquash:
				return std::wstring(L"MatOrthographicSquash");
			case ShaderConstantModificationFactory::MatHudSlide:
				return std::wstring(L"MatHudSlide");
			case ShaderConstantModificationFactory::MatGuiSquash:
				return std::wstring(L"MatGuiSquash");
			case ShaderConstantModificationFactory::MatSurfaceRefractionTransform:
				return std::wstring(L"MatSurfaceRefractionTransform");
			case ShaderConstantModificationFactory::MatGatheredOrthographicSquash:
				return std::wstring(L"MatGatheredOrthographicSquash");
			case ShaderConstantModificationFactory::MatOrthographicSquashShifted:
				return std::wstring(L"MatOrthographicSquashShifted");
			case ShaderConstantModificationFactory::MatOrthographicSquashHud:
				return std::wstring(L"MatOrthographicSquashHud");
			case ShaderConstantModificationFactory::MatConvergenceOffset:
				return std::wstring(L"MatConvergenceOffset");
			case ShaderConstantModificationFactory::MatSimpleTranslateIgnoreOrtho:
				return std::wstring(L"MatSimpleTranslateIgnoreOrtho");
			case ShaderConstantModificationFactory::MatRollOnly:
				return std::wstring(L"MatRollOnly");
			case ShaderConstantModificationFactory::MatRollOnlyNegative:
				return std::wstring(L"MatRollOnlyNegative");
			case ShaderConstantModificationFactory::MatRollOnlyHalf:
				return std::wstring(L"MatRollOnlyHalf");
			case ShaderConstantModificationFactory::MatNoRoll:
				return std::wstring(L"MatNoRoll");
			case ShaderConstantModificationFactory::MatSimpleTranslateNoPositional:
				return std::wstring(L"MatSimpleTranslateNoPositional");
			case ShaderConstantModificationFactory::MatNoStereoSeparate:
				return std::wstring(L"MatNoStereoSeparate");
			case ShaderConstantModificationFactory::MatRoll:
				return std::wstring(L"MatRoll");
			case ShaderConstantModificationFactory::MatRollNegative:
				return std::wstring(L"MatRollNegative");
			case ShaderConstantModificationFactory::MatRollConvergence:
				return std::wstring(L"MatRollConvergence");
			case ShaderConstantModificationFactory::MatRollNegativeConvergence:
				return std::wstring(L"MatRollNegativeConvergence");
			case ShaderConstantModificationFactory::MatShadowFix1:
				return std::wstring(L"MatShadowFix1");
			case ShaderConstantModificationFactory::MatShadowFix2:
				return std::wstring(L"MatShadowFix2");
			case ShaderConstantModificationFactory::MatShadowFix3:
				return std::wstring(L"MatShadowFix3");
			case ShaderConstantModificationFactory::MatShadowFix4:
				return std::wstring(L"MatShadowFix4");
			case ShaderConstantModificationFactory::MatShadowFix5:
				return std::wstring(L"MatShadowFix5");
			case ShaderConstantModificationFactory::MatShadowFix6:
				return std::wstring(L"MatShadowFix6");
			case ShaderConstantModificationFactory::MatShadowFix7:
				return std::wstring(L"MatShadowFix7");
			case ShaderConstantModificationFactory::MatShadowFix8:
				return std::wstring(L"MatShadowFix8");
			case ShaderConstantModificationFactory::MatTransformShadowFix1:
				return std::wstring(L"MatTransformShadowFix1");
			case ShaderConstantModificationFactory::MatTransformShadowFix2:
				return std::wstring(L"MatTransformShadowFix2");
			case ShaderConstantModificationFactory::MatTransformShadowFix3:
				return std::wstring(L"MatTransformShadowFix3");
			case ShaderConstantModificationFactory::MatTransformShadowFix4:
				return std::wstring(L"MatTransformShadowFix4");
			case ShaderConstantModificationFactory::MatTransformShadowFix5:
				return std::wstring(L"MatTransformShadowFix5");
			case ShaderConstantModificationFactory::MatTransformShadowFix6:
				return std::wstring(L"MatTransformShadowFix6");
			case ShaderConstantModificationFactory::MatTransformShadowFix7:
				return std::wstring(L"MatTransformShadowFix7");
			case ShaderConstantModificationFactory::MatTransformShadowFix8:
				return std::wstring(L"MatTransformShadowFix8");
			case ShaderConstantModificationFactory::MatTransformToRotation:
				return std::wstring(L"MatTransformToRotation");
				break;
		}
		return std::wstring(L"Error-Nonexistant");
	}
};
#endif