/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain
Modifications Copyright (C) 2013 Denis Reischl

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

#ifndef MATRIXORTHOSQUASH_H_INCLUDED
#define MATRIXORTHOSQUASH_H_INCLUDED
/**
* @file MatrixOrthoSquash.h
* Contains shader modification to squash orthographic matrices.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Squishes the matrix if orthographical, otherwise simple modification.
*/
class MatrixOrthoSquash : public ShaderMatrixModification
{
public:
	/**
	* Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatricies The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixOrthoSquash(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) 
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

			// add all translation and scale matrix entries 
			// (for the GUI this should be 3.0f, for the HUD above)
			float allAbs = abs(in(3, 0)); // transX
			allAbs += abs(in(3, 1)); // transY
			allAbs += abs(in(3, 2)); // transZ

			allAbs += abs(in(0, 0)); // scaleX
			allAbs += abs(in(1, 1)); // scaleY
			allAbs += abs(in(2, 2)); // scaleZ

			// TODO !! compute these two following matrices in the ViewAdjustment class :

			// HUD
			if (allAbs > 3.0f)
			{
				// separation -> distance translation
				D3DXMATRIX orthoToPersViewProjTransformLeft  = m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->LeftHUD3DDepth() * m_spAdjustmentMatrices->LeftViewTransform() * m_spAdjustmentMatrices->HUDDistance() *  m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->LeftShiftProjection();
				D3DXMATRIX orthoToPersViewProjTransformRight = m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->RightHUD3DDepth() * m_spAdjustmentMatrices->RightViewTransform() * m_spAdjustmentMatrices->HUDDistance() * m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->RightShiftProjection();

				outLeft = in * orthoToPersViewProjTransformLeft;
				outright = in * orthoToPersViewProjTransformRight;
			}
			else // GUI
			{
				if (m_spAdjustmentMatrices->BulletLabyrinthMode())
				{
					D3DXMATRIX tempMatrix;
					D3DXMatrixTranspose(&tempMatrix, &in);
					tempMatrix = m_spAdjustmentMatrices->BulletLabyrinth() * tempMatrix;
					D3DXMatrixTranspose(&tempMatrix, &tempMatrix);

					outLeft = tempMatrix * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->LeftGUI3DDepth() * m_spAdjustmentMatrices->LeftShiftProjection() * m_spAdjustmentMatrices->Squash() * m_spAdjustmentMatrices->Projection();
					outright = tempMatrix * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->RightGUI3DDepth() * m_spAdjustmentMatrices->RightShiftProjection() * m_spAdjustmentMatrices->Squash() * m_spAdjustmentMatrices->Projection();
				
					// SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
				}
				else
				{
					// simple squash
					outLeft = in * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->LeftGUI3DDepth() * m_spAdjustmentMatrices->LeftShiftProjection() * m_spAdjustmentMatrices->Squash() * m_spAdjustmentMatrices->Projection();
					outright = in * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->RightGUI3DDepth() * m_spAdjustmentMatrices->RightShiftProjection() * m_spAdjustmentMatrices->Squash() * m_spAdjustmentMatrices->Projection();
				}
			}
		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	};
};
#endif