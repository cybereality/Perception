/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixRollOnly.h> and
Class <MatrixRollOnly> :
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

#ifndef MATRIXROLLONLY_H_INCLUDED
#define MATRIXROLLONLY_H_INCLUDED
/**
* @file MatrixRollOnly.h
* Matrix applies only the head roll.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Matrix applies only the head roll.
* For shadow fixes.
*/
class MatrixRollOnly : public ShaderMatrixModification
{
public:
	MatrixRollOnly(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{};

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// in * rollMatrix
		outLeft = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrix()
			* m_spAdjustmentMatrices->Projection();
		outright = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrix()
			* m_spAdjustmentMatrices->Projection();
	};
};
#endif