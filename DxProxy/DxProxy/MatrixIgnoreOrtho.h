/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MatrixIgnoreOrtho.h> and
Class <MatrixIgnoreOrtho> :
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

#ifndef MATRIXIGNOREORTHO_H_INCLUDED
#define MATRIXIGNOREORTHO_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Modification to ignore orthographic matrices.
* This is the intent, not sure that the math holds up but it works for HL2. 
* (Doesn't seem to work for omd2, stops all separation)
*/
class MatrixIgnoreOrtho : public ShaderMatrixModification
{
public:
	MatrixIgnoreOrtho(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose) : ShaderMatrixModification(modID, adjustmentMatrices, transpose) {};

	/**
	* Don't apply if matrix is orthographic.
	* TODO this was the quick way to get the hud positioned correctly (same result as 
	* (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
	* in the old code. The 'correct' way to do this in this version would be to have an override
	* shader rule that applies to the shader used for the ui; assuming it's not used
	* for other models as well... if it is this seems like the only option unless there is another 
	* way to identify the constant from it's properties rather than the actual value.
	***/
	virtual bool DoNotApply(D3DXMATRIX in)
	{		
		return vireio::AlmostSame(in[15], 1.0f, 0.00001f);
	}
};
#endif