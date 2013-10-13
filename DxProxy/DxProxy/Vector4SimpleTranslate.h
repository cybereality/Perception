/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
  and Joshua Brown 

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

#ifndef VEC4SIMPLETRANSLATE_H_INCLUDED
#define VEC4SIMPLETRANSLATE_H_INCLUDED


#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"

/**
* Vector4 translation.
* Translates Vector4 to left and right by separation in world units.
*/
class Vector4SimpleTranslate : public ShaderConstantModification<float>
{
public:		
	Vector4SimpleTranslate(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies) : ShaderConstantModification(modID, adjustmentMatricies) {};

	/**
	* Translates Vector4 to left and right by separation in world units.
	***/
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXVECTOR4 tempLeft (inData);
		D3DXVECTOR4 tempRight (inData);

		tempLeft[0] += m_spAdjustmentMatrices->SeparationInWorldUnits() * LEFT_CONSTANT;
		tempRight[0] += m_spAdjustmentMatrices->SeparationInWorldUnits() * RIGHT_CONSTANT;

		outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
		outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());	
	}
};
#endif