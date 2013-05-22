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

#ifndef STEREOSHADERCONSTANT_H_INCLUDED
#define STEREOSHADERCONSTANT_H_INCLUDED

#include <vector>

template <class T>
class StereoShaderConstant
{
public:
	StereoShaderConstant(UINT StartReg, const T* pConstDataLeft, const T* pConstDataRight, UINT dataCount, UINT countMultiplier) :
		StartRegister(StartReg),
		Count(dataCount),
		Multiplier(countMultiplier),
		DataLeft(pConstDataLeft, pConstDataLeft + (dataCount * countMultiplier)),
		DataRight(pConstDataRight, pConstDataRight + (dataCount * countMultiplier)) {}

	virtual ~StereoShaderConstant() {}
		
	T* DataLeftPointer() 
	{
		return &DataLeft[0];
	}

	T* DataRightPointer() 
	{
		return &DataRight[0];
	}

	UINT StartRegister;
	UINT Count;
	UINT Multiplier;
	std::vector<T> DataLeft;
	std::vector<T> DataRight;
};

#endif
