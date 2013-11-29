/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright © 2012 Andres Hernandez

File <Vector4EyeShiftUnity.h> and
Class <Vector4EyeShiftUnity> :
Copyright © 2013 Denis Reischl

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

#ifndef VECTOR4EYESHIFTUNITY_H_INCLUDED
#define VECTOR4EYESHIFTUNITY_H_INCLUDED

#define LEFT_SHIFT -5000.0f;
#define RIGHT_SHIFT 5000.0f;

#include "d3d9.h"
#include "d3dx9.h"
#include "Vector4SimpleTranslate.h"

/**
* Vector4 eye shift attempt (not working when rotating).
* Translates Vector4 to left and right by separation in world units.
*/
class Vector4EyeShiftUnity : public Vector4SimpleTranslate
{
public:		
	Vector4EyeShiftUnity(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices) : Vector4SimpleTranslate(modID, adjustmentMatrices) {};
	/**
	* Translates Vector4 to left and right by constant separation in world units.
	***/
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXVECTOR4 tempLeft (inData);
		D3DXVECTOR4 tempRight (inData);

		/*char buf[128];
		sprintf_s(buf, 128, "X : %f", tempLeft.x);
		OutputDebugString(buf);
		sprintf_s(buf, 128, "Y : %f", tempLeft.y);
		OutputDebugString(buf);
		sprintf_s(buf, 128, "Z : %f", tempLeft.z);
		OutputDebugString(buf);*/

		tempLeft[0] += m_spAdjustmentMatrices->SeparationInWorldUnits() * LEFT_SHIFT;
		tempRight[0] += m_spAdjustmentMatrices->SeparationInWorldUnits() * RIGHT_SHIFT;

		outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
		outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());	
	}
};
#endif