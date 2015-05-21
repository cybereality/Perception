/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixHudSlide.h> and
Class <MatrixHudSlide> :
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

#ifndef MATRIXHUDSLIDE_H_INCLUDED
#define MATRIXHUDSLIDE_H_INCLUDED
/**
* @file MatrixHudSlide.h
* Contains shader modification to slide HUD matrices.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* SSlides the HUD into the HMD output.
* All orthographic matrices treated as HUD here.
*/
class MatrixHudSlide : public ShaderMatrixModification
{
public:
	MatrixHudSlide(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Slides the HUD into the HMD output.
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// HUD
		// separation -> distance translation
		outLeft = in * m_spAdjustmentMatrices->LeftHUDMatrix();
		outright = in * m_spAdjustmentMatrices->RightHUDMatrix();
	};
};
#endif