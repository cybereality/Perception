/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
and Joshua Brown 

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
	* @param adjustmentMatricies The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixOrthoSquashHud(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) 
		: ShaderMatrixModification(modID, adjustmentMatricies, transpose) 
	{};

	/**
	* Matrix modification does multiply: shiftprojection * squash (for GUI), scale * transform * distance (for HUD).
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatricies.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (vireio::AlmostSame(in[15], 1.0f, 0.00001f)) {

			// HUD
			// separation -> distance translation
			D3DXMATRIX orthoToPersViewProjTransformLeft  = m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->LeftHUD3DDepth() * m_spAdjustmentMatrices->LeftViewTransform() * m_spAdjustmentMatrices->HUDDistance() *  m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->LeftShiftProjection();
			D3DXMATRIX orthoToPersViewProjTransformRight = m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->RightHUD3DDepth() * m_spAdjustmentMatrices->RightViewTransform() * m_spAdjustmentMatrices->HUDDistance() * m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->RightShiftProjection();

			outLeft = in * orthoToPersViewProjTransformLeft;
			outright = in * orthoToPersViewProjTransformRight;

		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	};
};
#endif