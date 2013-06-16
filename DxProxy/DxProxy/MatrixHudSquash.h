/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#ifndef MATRIXHUDSQUASH_H_INCLUDED
#define MATRIXHUDSQUASH_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"

class MatrixHudSquash : public ShaderMatrixModification
{
public:
	MatrixHudSquash(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) : ShaderMatrixModification(modID, adjustmentMatricies, transpose) 
	{
		D3DXMatrixScaling(&squash, 0.5f, 0.5f, 1);
	};

	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// TODO probably don't want to be translating the HUD around.
		// Need an adjustment that does unproject, reproject left/right? (then squash)
		// Refactor modifications into view adjustments? Or just make unproject->reproject a standard adjustment in adjustments?
		// Or ???
		// TODO Refactor so adjustments aren't recalculated every time constant is applied that don't need to be
		// Shift adustments to some kind of indexed lookup in view adjustments with adjustments only being updated if dirty?
		outLeft = in * m_spAdjustmentMatricies->LeftShiftProjection() * squash;
		outright = in * m_spAdjustmentMatricies->RightShiftProjection() * squash;
	};


private:
	D3DXMATRIX squash;
};


#endif