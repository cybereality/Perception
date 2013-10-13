/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl

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

#ifndef MATRIXDONOTHING_H_INCLUDED
#define MATRIXDONOTHING_H_INCLUDED
/*
* @file MatrixDoNothing.h
* Conatains matrix class which modifies nothing.
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

/**
* Simple modification that does not apply anything. Is needed for certain methods that could do things but sometimes won't.
*/
class MatrixDoNothing : public ShaderMatrixModification
{
public:
	/**
	* Just sets the class values that correspond to the parameters. 
	* @param modID The modification ID
	* @param adjustmentMatricies The matricies to do the adjustments to.
	***/
	MatrixDoNothing(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies) : 
		ShaderMatrixModification(modID, adjustmentMatricies, false) {};

	/**
	* Simply returns True.
	* @param in Irrelevant parameter.
	***/
	virtual bool DoNotApply(D3DXMATRIX in)
	{
		return true;
	}
};
#endif