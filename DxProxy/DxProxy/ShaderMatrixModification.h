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

#ifndef SHADERMATRIXMODIFICATION_H_INCLUDED
#define SHADERMATRIXMODIFICATION_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"

class ShaderMatrixModification : public ShaderConstantModification<float>
{
public:
	ShaderMatrixModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) : 
		ShaderConstantModification(modID, adjustmentMatricies),
		m_bTranspose(transpose)
		{};

	/* This method should not generally be overridden by subclasses (if it is the overriding method should respect the result of DoNotApply). 
		Do modification in DoMatrixModification. Transposed matricies will be provided to DoMatrixModification if transpose is true */
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXMATRIX tempMatrix (inData);

		if (DoNotApply(tempMatrix)) {

			outLeft->assign(&tempMatrix[0], &tempMatrix[0] + outLeft->size());
			outRight->assign(&tempMatrix[0], &tempMatrix[0] + outRight->size());
		}
		else {

			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			}
			
			D3DXMATRIX tempLeft;
			D3DXMATRIX tempRight;

			DoMatrixModification(tempMatrix, tempLeft, tempRight);

			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempLeft, &tempLeft);
				D3DXMatrixTranspose(&tempRight, &tempRight);
			}

			outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
			outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());
		}
	}


protected:

	/* Override to do actual modification. Default is simple translate */
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in * m_spAdjustmentMatricies->LeftAdjustmentMatrix();
		outright = in * m_spAdjustmentMatricies->RightAdjustmentMatrix();
	}

	/* Override to provide condition underwhich this mod should not be applied (modification will always perform simple copy of in to left and right if this returns true)
		If false then modification will be applied.
		Available so ortho check can be done without performing any uneccessary transposes (and generalised to this method in case other cases become apparent)
		Default is to always apply */
	virtual bool DoNotApply(D3DXMATRIX in)
	{
		return false;
	}

private:
	bool m_bTranspose;
};


#endif