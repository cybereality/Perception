/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixOrthoSquashHud.h> and
Class <MatrixOrthoSquashHud> :
Copyright (C) 2013 Denis Reischl

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

#ifndef MATRIXORTHOSQUASHHUD_H_INCLUDED
#define MATRIXORTHOSQUASHHUD_H_INCLUDED
/**
* @file MatrixOrthoSquashHud.h
* Contains shader modification to squash orthographic matrices, only HUD matrices here.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Squishes the matrix if orthographical, otherwise simple modification.
* All orthographic matrices treated as HUD here.
*/
class MatrixOrthoSquashHud : public ShaderMatrixModification
{
public:
	/**
	* Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixOrthoSquashHud(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose) 
		: ShaderMatrixModification(modID, adjustmentMatrices, transpose) 
	{};

	/**
	* Matrix modification does multiply: shiftprojection * squash (for GUI), scale * transform * distance (for HUD).
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (vireio::AlmostSame(in[15], 1.0f, 0.00001f)) {

			// HUD
			// separation -> distance translation
			outLeft = in * m_spAdjustmentMatrices->LeftHUDMatrix();
			outright = in * m_spAdjustmentMatrices->RightHUDMatrix();

		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	};
};
#endif