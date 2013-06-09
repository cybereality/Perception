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

#ifndef SHADERCONSTANTMODIFICATION_H_INCLUDED
#define SHADERCONSTANTMODIFICATION_H_INCLUDED

#include <memory>
#include <vector>
#include "d3d9.h"
#include "d3dx9.h"
#include "ViewAdjustmentMatricies.h"

template <class T=float>
class ShaderConstantModification
{
public:
	ShaderConstantModification(UINT modID, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies) : 
		m_ModificationID(modID),
		m_spAdjustmentMatricies(adjustmentMatricies)
	{}

	virtual ~ShaderConstantModification()
	{
		m_spAdjustmentMatricies.reset();
	}

	/* Applies this modification to toModify to produce left and right versions */
	virtual void ApplyModification(const T* inData, std::vector<T>* outLeft, std::vector<T>* outRight) = 0;


	UINT m_ModificationID;
protected:
	std::shared_ptr<ViewAdjustmentMatricies> m_spAdjustmentMatricies;	
};


#endif