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

#ifndef MATSIMPLETRANSLATE_H_INCLUDED
#define MATSIMPLETRANSLATE_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"

class MatrixSimpleTranslate : public ShaderConstantModification<float>
{
public:
	MatrixSimpleTranslate(UINT modID, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies) : ShaderConstantModification(modID, adjustmentMatricies) {};

	virtual void ApplyModification(const float* toModify, float* outLeft, float* outRight)
	{
		D3DXMATRIX tempMatrix = const_cast<float*>(toModify);
		
		D3DXMATRIX tempLeft (tempMatrix * *(m_spAdjustmentMatricies->LeftAdjustmentMatrix()));
		D3DXMATRIX tempRight (tempMatrix * *(m_spAdjustmentMatricies->RightAdjustmentMatrix()));

		outLeft = tempLeft;
		outRight = tempRight;
	}
};


#endif