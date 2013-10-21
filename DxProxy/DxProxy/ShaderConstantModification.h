/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderConstantModification.h> and
Class <ShaderConstantModification> :
Copyright (C) 2013 Chris Drain

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

/**
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

/**
* Abstract class acts as the skeleton class for Shader Modification Classes, 
* contains no shader modification logic simply keeps track of info.
* Must implement ApplyModification method.
* Prototype for any shader register modification.
* @tparam T The type of parameter to use for applying modifications, default is float.
* @see ViewAdjustment
*/
template <class T=float>
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
	*/
	virtual ~ShaderConstantModification()
	{
		m_spAdjustmentMatrices.reset();
	}

	/**
	*  Pure virtual method, should apply the modification to produce left and right versions.   
	*/
	virtual void ApplyModification(const T* inData, std::vector<T>* outLeft, std::vector<T>* outRight) = 0;

	/**
	* Simply a way to identify this modification.  Useful for comparing shadermodification equality.
	*/
	UINT m_ModificationID;
protected:
	/**
	* Matrix calculation class pointer
	*/
	std::shared_ptr<ViewAdjustment> m_spAdjustmentMatrices;
};
#endif