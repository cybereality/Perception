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

#ifndef MATSIMPLETRANSLATECOLNOORTHO_H_INCLUDED
#define MATSIMPLETRANSLATECOLNOORTHO_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "MatrixTSimpleTranslate.h"


class MatrixTSimpleTranslateIgnoreOrtho : public MatrixTSimpleTranslate
{
public:
	MatrixTSimpleTranslateIgnoreOrtho(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies) : MatrixTSimpleTranslate(modID, adjustmentMatricies) {};

	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		//TODO this was the quick way to get the hud positioned correctly (same result as (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
		// in the old code. The 'correct' way to do this in this version would be to have an override shader rule that applies to the shader used for the ui; assuming it's not used
		// for other models as well... if it is this seems like the only option unless there is another way to identify the constant from it's properties rather than the actual value
		if (!vireio::AlmostSame(inData[15], 1.0f, 0.00001f)) { //if not ortho 

			MatrixTSimpleTranslate::ApplyModification(inData, outLeft, outRight);
		}
		else {
			D3DXMATRIX tempMatrix (inData);

			outLeft->assign(&tempMatrix[0], &tempMatrix[0] + outLeft->size());
			outRight->assign(&tempMatrix[0], &tempMatrix[0] + outRight->size());
		}
	}
};


#endif