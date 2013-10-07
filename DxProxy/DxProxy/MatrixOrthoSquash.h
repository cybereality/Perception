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
	*  Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatricies The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixOrthoSquash(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) 
		: ShaderMatrixModification(modID, adjustmentMatricies, transpose) 
	{
		// should this be hard coded?  Seems a bit fishy... - Josh
		D3DXMatrixScaling(&squash, 0.625f, 0.625f, 1);

		// hudDistance
		D3DXMatrixTranslation(&hudDistance, 0, 0, 1);

		// hudScale
		float scalarHUDScale = 3.1f;
		D3DXMatrixScaling(&hudScale, scalarHUDScale, scalarHUDScale, 1);
	};

	/**
	* Matrix modification does multiply: translation * squash.
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatricies.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (vireio::AlmostSame(in[15], 1.0f, 0.00001f)) {
			// TODO !! do that more simple
			float transX = in(3, 0);
			float transY = in(3, 1);
			float transZ = in(3, 2);
			
			float scaleX = in(0, 0);
			float scaleY = in(1, 1);
			float scaleZ = in(2, 2);
			
			float allAbs = abs(transX)+abs(transY)+abs(transZ)+abs(scaleX)+abs(scaleY)+abs(scaleZ);
			
			if (allAbs > 3.1f)
			{
				// Reproject ortho projected hud elements using perspective. These matricies will be useless if hud mode is separation adjustment of diatanec rather than actual distance
				D3DXMATRIX orthoToPersViewProjTransformLeft  = m_spAdjustmentMatrices->ProjectionInverse() * hudScale * m_spAdjustmentMatrices->LeftViewTransform() * hudDistance *  m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->LeftShiftProjection();
				D3DXMATRIX orthoToPersViewProjTransformRight = m_spAdjustmentMatrices->ProjectionInverse() * hudScale * m_spAdjustmentMatrices->RightViewTransform() * hudDistance * m_spAdjustmentMatrices->Projection() * m_spAdjustmentMatrices->RightShiftProjection();
				outLeft = in * orthoToPersViewProjTransformLeft;
				outright = in * orthoToPersViewProjTransformRight;
			}
			else
			{
				// simple squash
				outLeft = in * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->LeftShiftProjection() * squash * m_spAdjustmentMatrices->Projection();
				outright = in * m_spAdjustmentMatrices->ProjectionInverse() * m_spAdjustmentMatrices->RightShiftProjection() * squash * m_spAdjustmentMatrices->Projection();
			}

		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	};

private:
	/**
	* Squash scaling matrix, obtained by the D3DXMatrixScaling.
	*/
	D3DXMATRIX squash;
	D3DXMATRIX hudDistance;
	D3DXMATRIX hudScale;
};
#endif