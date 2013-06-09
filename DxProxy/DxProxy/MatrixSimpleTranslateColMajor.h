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

#ifndef MATSIMPLETRANSLATECOL_H_INCLUDED
#define MATSIMPLETRANSLATECOL_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"

class MatrixSimpleTranslateColMajor : public ShaderConstantModification<float>
{
public:
	MatrixSimpleTranslateColMajor(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies) : ShaderConstantModification(modID, adjustmentMatricies) {};

	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXMATRIX tempMatrix (inData);

		D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			
		D3DXMATRIX tempLeft (tempMatrix * m_spAdjustmentMatricies->LeftAdjustmentMatrix());
		D3DXMATRIX tempRight (tempMatrix * m_spAdjustmentMatricies->RightAdjustmentMatrix());

		D3DXMatrixTranspose(&tempLeft, &tempLeft);
		D3DXMatrixTranspose(&tempRight, &tempRight);

		outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
		outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());
	}
};


#endif