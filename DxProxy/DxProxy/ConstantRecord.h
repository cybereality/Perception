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

#ifndef CONSTANTRECORD_H_INCLUDED
#define CONSTANTRECORD_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"

template <class T>
class ConstantRecord
{
public:
	ConstantRecord(UINT StartReg, const T* pConstData, UINT dataCount, UINT countMultiplier) :
		StartRegister(StartReg),
		Count(dataCount) 
	{
		// copy data
		T* pConstantData = new T [Count * countMultiplier];
		std::copy(&pConstData[0], &pConstData[Count], pConstantData);
	}

	virtual ~ConstantRecord() 
	{
		delete [] pConstantData;
	}
		

	UINT StartRegister;
	T* pConstantData;
	UINT Count;
};

#endif