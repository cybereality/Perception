/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixGuiSquash.h> and
Class <MatrixGuiSquash> :
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

#ifndef MATRIXGUISQUASH_H_INCLUDED
#define MATRIXGUISQUASH_H_INCLUDED
/**
* @file MatrixGuiSquash.h
* Contains shader modification to squash the HUD.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Matrix implementation whose purpose is to squash the GUI acordingly to the driver settings. 
*/
class MatrixGuiSquash : public ShaderMatrixModification
{
public:
	/**
	*  Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixGuiSquash(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose) 
		: ShaderMatrixModification(modID, adjustmentMatrices, transpose) 
	{};

	/**
	* Matrix modification does multiply: translation * squash.
	* *  Does the matrix squash and outputs the results.  Does not only affect HUD at this point in time (August 22nd, 2013).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	* TODO probably don't want to be translating the HUD around.
	* Need an adjustment that does unproject, reproject left/right? (then squash)
	* Refactor modifications into view adjustments? Or just make unproject->reproject a standard adjustment in adjustments?
	* Or ???
	* TODO Refactor so adjustments aren't recalculated every time constant is applied that don't need to be
	* Shift adustments to some kind of indexed lookup in view adjustments with adjustments only being updated if dirty?
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// simple squash
		outLeft = in * m_spAdjustmentMatrices->LeftGUIMatrix();
		outright = in * m_spAdjustmentMatrices->RightGUIMatrix();
	};
};
#endif