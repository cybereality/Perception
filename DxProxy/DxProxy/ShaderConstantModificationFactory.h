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

#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "ShaderMatrixModification.h"
#include "Vector4SimpleTranslate.h"
#include "Vector4EyeShiftUnity.h"
#include "Vector4DeadIslandScaled.h"

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
		Vec4EyeShiftUnity = 2,              /**< **/
		Vec4DeadIslandScaled = 3              /**< **/
	};
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
		MatNoStereoSeparate = 16			/**< Don't translate, but is still affected by positional tracking **/
	};

	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices);
	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(Vector4ModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices);
	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose);
	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(MatrixModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose);
};
#endif