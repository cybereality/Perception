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
#include "ViewAdjustment.h"

template <class T=float>

/**
* Shader constant modification class template.
* Prototype for any shader register modification.
* @see ViewAdjustment
*/
class ShaderConstantModification
{
public:
	/**
	* Constructor.
	* @param modID Identifier of the modification. Identifier enumerations defined in ShaderConstantModificationFactory.
	* @param adjustmentMatrices Pointer to view adjustment class.
	* @see ViewAdjustment Matrix calculation class pointer.
	***/
	ShaderConstantModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices) : 
		m_ModificationID(modID),
		m_spAdjustmentMatrices(adjustmentMatrices)
	{}
	/**
	* Destructor.
	* calls ViewAdjustment::reset()
	* @see ViewAdjustment
	***/
	virtual ~ShaderConstantModification()
	{
		m_spAdjustmentMatrices.reset();
	}

	/**
	* Applies this modification to produce left and right versions. 
	***/
	virtual void ApplyModification(const T* inData, std::vector<T>* outLeft, std::vector<T>* outRight) = 0;
	
	/**
	* Modification identifier.
	***/
	UINT m_ModificationID;
protected:
	/**
	* Matrix calculation class pointer.
	***/
	std::shared_ptr<ViewAdjustment> m_spAdjustmentMatrices;	
};
#endif