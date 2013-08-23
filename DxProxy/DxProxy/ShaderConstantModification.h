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

/*
 * @file ShaderConstantModification.h
 * Contains ShaderConstantModification class.
 */
#ifndef SHADERCONSTANTMODIFICATION_H_INCLUDED
#define SHADERCONSTANTMODIFICATION_H_INCLUDED

#include <memory>
#include <vector>
#include "d3d9.h"
#include "d3dx9.h"
#include "ViewAdjustment.h"


/*
 * Abstract class acts as the skeleton class for Shader Modification Classes, 
 * contains no shader modification logic simply keeps track of info.
 * Must implement ApplyModification method.
 * @tparam T The type of parameter to use for applying modifciations, default is float.
 */
template <class T=float>
class ShaderConstantModification
{
public:
	/*
	 * Constructor simply sets the corresponding attributes.
	 * @param modID The shader modification ID
	 * @param adjustmentMatricies The view matrices that will be adjusted
	 */
	ShaderConstantModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies) : 
		m_ModificationID(modID),
		m_spAdjustmentMatricies(adjustmentMatricies)
	{}

	/*
	 * Destructor, simply releases resources.
	 */
	virtual ~ShaderConstantModification()
	{
		m_spAdjustmentMatricies.reset();
	}

	/*
	 *  Pure virtual method, should apply the modification to produce left and right versions.   
	 */
	virtual void ApplyModification(const T* inData, std::vector<T>* outLeft, std::vector<T>* outRight) = 0;

	/*
	 * Simply a way to identify this modification.  Useful for comparing shadermodification equality.
	 */
	UINT m_ModificationID;
protected:
	/*
	 * The view matrices to be adjusted.
	 */
	std::shared_ptr<ViewAdjustment> m_spAdjustmentMatricies;
	
};


#endif