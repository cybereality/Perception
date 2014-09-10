/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixGatheredOrthoSquash.h> and
Class <MatrixGatheredOrthoSquash> :
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

#ifndef MATRIXGATHERORTHOSQUASH_H_INCLUDED
#define MATRIXGATHERORTHOSQUASH_H_INCLUDED
/**
* @file MatrixGatheredOrthoSquash.h
* Contains shader modification to squash orthographic matrices and gather the result.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Squishes the matrix if orthographical, otherwise simple modification.
*/
class MatrixGatheredOrthoSquash : public MatrixOrthoSquash
{
public:
	/**
	*  Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixGatheredOrthoSquash(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose) 
		: MatrixOrthoSquash(modID, adjustmentMatrices, transpose) 
	{};

	/**
	* Applies modification to registers.
	* This method should not generally be overridden by subclasses (if it is the overriding method
	* should respect the result of DoNotApply). 
	* Do modification in DoMatrixModification. Transposed matrices will be provided to 
	* DoMatrixModification if transpose is true.
	* @param [in] inData Input matrix to be modified and assigned to registers.
	* @param [in, out] outLeft Register vector left.
	* @param [in, out] outRight Register vector right.
	***/
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXMATRIX tempMatrix (inData);

		// conditions to apply the matrix ?
		if (DoNotApply(tempMatrix)) {

			outLeft->assign(&tempMatrix[0], &tempMatrix[0] + outLeft->size());
			outRight->assign(&tempMatrix[0], &tempMatrix[0] + outRight->size());
		}
		else {

			// matrix to be transposed ?
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			}
			
			D3DXMATRIX tempLeft;
			D3DXMATRIX tempRight;

			// do modification
			DoMatrixModification(tempMatrix, tempLeft, tempRight);

			// transpose back
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempLeft, &tempLeft);
				D3DXMatrixTranspose(&tempRight, &tempRight);
			}

			// assign to output
			outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
			outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());

			// gather matrix
			m_spAdjustmentMatrices->GatherMatrix(tempLeft, tempRight);
		}
	}
};
#endif