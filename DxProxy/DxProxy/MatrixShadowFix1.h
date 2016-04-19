/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixShadowFix1.h> :
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

#ifndef MATRIXSHADOWFIX1_H_INCLUDED
#define MATRIXSHADOWFIX1_H_INCLUDED
/**
* @file MatrixShadowFix1.h
* Translation and negative rotation (game-specific).
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Translation and negative rotation (game-specific).
*/
class MatrixShadowFix1 : public ShaderMatrixModification
{
public:
	/**
	* Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixShadowFix1(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
		: ShaderMatrixModification(modID, adjustmentMatrices, transpose)
	{};

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		/*std::stringstream sz;
		sz << in(0, 0) << ":" << in(0, 1) << ":" << in(0, 2) << ":" << in(0, 3) << ":" << in(1, 0) << ":" << in(1, 1) << ":" << in(1, 2) << ":" << in(1, 3) << in(2, 0) << ":" << in(2, 1) << ":" << in(2, 2) << ":" << in(2, 3) << ":" << in(3, 0) << ":" << in(3, 1) << ":" << in(3, 2) << ":" << in(3, 3);
		OutputDebugStringA(sz.str().c_str());*/

		D3DXMATRIX sTranslateLeft;
		D3DXMATRIX sTranslateRight;
		if (in(0, 0) < 1.0f)
		{
			D3DXMatrixTranslation(&sTranslateLeft, 0.47f * m_spAdjustmentMatrices->Configuration()->fWorldScaleFactor, 0.0f, 0.0f);
			D3DXMatrixTranslation(&sTranslateRight, -0.47f * m_spAdjustmentMatrices->Configuration()->fWorldScaleFactor, 0.0f, 0.0f);
		}
		else
		{
			D3DXMatrixTranslation(&sTranslateLeft, 1.5f * m_spAdjustmentMatrices->Configuration()->fWorldScaleFactor, 0.0f, 0.0f);
			D3DXMatrixTranslation(&sTranslateRight, -1.5f * m_spAdjustmentMatrices->Configuration()->fWorldScaleFactor, 0.0f, 0.0f);
		}

		// convergence ?
		if (m_spAdjustmentMatrices->Configuration()->bConvergenceEnabled)
		{
			sTranslateLeft(2, 0) = -0.03f;
			sTranslateRight(2, 0) = 0.03f;
		}

		// in * rollMatrix
		outLeft = in * sTranslateLeft * m_spAdjustmentMatrices->RollMatrixNegative();
		outright = in * sTranslateRight * m_spAdjustmentMatrices->RollMatrixNegative();
	};
};
#endif