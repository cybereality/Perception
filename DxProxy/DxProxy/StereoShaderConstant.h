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
#include <string>
#include "ShaderConstantModification.h"

template <class T>
class StereoShaderConstant
{
public:							// TODO take normal data in, generate left and right using modification
	StereoShaderConstant(UINT StartReg, const T* pConstDataLeft, const T* pConstDataRight, UINT dataCount, UINT countMultiplier/*register length (4 for float/int, 1 for bool)*/, std::string name, ShaderConstantModification<T> modification, UINT modificationId) :
		m_StartRegister(StartReg),
		m_Count(dataCount),
		m_Multiplier(countMultiplier),
		m_DataLeft(pConstDataLeft, pConstDataLeft + (dataCount * countMultiplier)),
		m_DataRight(pConstDataRight, pConstDataRight + (dataCount * countMultiplier)),
		m_Name(name),
		m_ModificationID(modificationId) {}

	virtual ~StereoShaderConstant() {}

	// pointer to new data. Verify data dimensions match this constant before calling if needed
	void Update(const T* data) 
	{
		// assert sizes match

		// TODO Apply modification to update left and right
	}

	/* Return true if this constant represents the same constant as other  (contents of the registers does not need to match)*/
	bool SameConstantAs(const StereoShaderConstant<T> & other)
	{
		return (other.m_StartRegister == m_StartRegister) &&
			(other.m_ModificationID == m_ModificationID));
	}
		
	T* DataLeftPointer() 
	{
		return &m_DataLeft[0];
	}

	T* DataRightPointer() 
	{
		return &m_DataRight[0];
	}

	std::string Name() { return m_Name; }
	UINT StartRegister() { return m_StartRegister; }
	UINT Count() { return m_Count; }
	UINT Multiplier() { return m_Multiplier; }
	

private:
	std::vector<T> m_DataLeft;
	std::vector<T> m_DataRight;

	std::string m_Name;
	UINT m_StartRegister;
	UINT m_Count;
	UINT m_Multiplier;
	UINT m_ModificationID;
};


#endif
