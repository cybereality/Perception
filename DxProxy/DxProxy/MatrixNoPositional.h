/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderMatrixModification.h> and
Class <ShaderMatrixModification> :
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

#ifndef MATRIXNOPOSITIONAL_H_INCLUDED
#define MATRIXNOPOSITIONAL_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Simple translate modification that is unaffected by positional tracking
*/
class MatrixNoPositional : public ShaderMatrixModification
{
public:
	MatrixNoPositional(ShaderMatrixModificationParams params)
		: ShaderMatrixModification(params)
	{}

	/**
	* Default modification is simple translate. Override to do actual modification. 
	* @param in [in] Modification matrix to be multiplied by adjustment matrix (left/right).
	* @param outLeft [in, out] Left transform matrix.
	* @param outRight [in, out] Right transform matrix.
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in * m_spAdjustmentMatrices->LeftAdjustmentMatrix();
		outright = in * m_spAdjustmentMatrices->RightAdjustmentMatrix();
	}
};

#endif //MATRIXNOPOSITIONAL_H_INCLUDED
