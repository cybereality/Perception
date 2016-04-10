/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixTransformToRotation.h> and
Class <MatrixTransformToRotation> :
Copyright (C) 2016 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
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

#ifndef MatrixTransformToRotation_H_INCLUDED
#define MatrixTransformToRotation_H_INCLUDED
/**
* @file MatrixTransformToRotation.h
* Matrix applies transform only if no member of the first input row is zero.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Matrix applies transform only if no member of the first input row is zero.
*/
class MatrixTransformToRotation : public ShaderMatrixModification
{
public:
	/**
	* Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixTransformToRotation(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
		: ShaderMatrixModification(modID, adjustmentMatrices, transpose)
	{};

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if ((in(0, 0) == 0.0f) || (in(0, 1) == 0.0f) || (in(0, 2) == 0.0f) || (in(0, 3) == 0.0f))
		{
			outLeft = in;
			outright = in;
			return;
		}
		else
		{
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	};
};
#endif