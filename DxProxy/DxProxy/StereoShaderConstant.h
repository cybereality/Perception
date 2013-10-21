/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <StereoShaderConstant.h> and
Class <StereoShaderConstant> :
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

#ifndef STEREOSHADERCONSTANT_H_INCLUDED
#define STEREOSHADERCONSTANT_H_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"

//  Number of T in a Register (L) = (4 for float/int, 1 for bool)
template <class T=float, UINT L=4>

/**
* Stero shader constant data class.
* Here, the current shader constant is stored.
*/
class StereoShaderConstant
{
public:
	/**
	* Constructor.
	* @param StartReg
	* @param pData
	* @param dataCount
	* @param modification
	***/
	StereoShaderConstant(UINT StartReg, const T* pData, UINT dataCount, std::shared_ptr<ShaderConstantModification<T>> modification) :
		m_StartRegister(StartReg),
		m_Count(dataCount),
		m_DataOriginal(pData, pData + (dataCount * L)),
		m_DataLeft(),
		m_DataRight(),
		m_modification(modification)
	{
		m_DataLeft.resize(dataCount * L);
		m_DataRight.resize(dataCount * L);
		Update(pData);
	}
	/**
	* Destructor.
	* Calls ShaderConstantModification::reset()
	* @see ShaderConstantModification::reset()
	***/
	virtual ~StereoShaderConstant() 
	{
		m_modification.reset();
	}
	/**
	* Updates this constant by specified data.
	* Assigns data to original data, applies constant modification.
	* Verify data dimensions match this constant _before_ calling if needed.
	* @param pData Pointer to new data. 
	***/
	void Update(const T* pData) 
	{
		m_DataOriginal.assign(pData, pData + (m_Count * L));
		m_modification->ApplyModification(pData, &m_DataLeft, &m_DataRight);
	}
	/**
	* Return true if this constant represents the same constant as other.
	* Contents of the registers does not need to match, but the registers 
	* must be the same and use the same modification.
	* @param other Pointer to other shader constant.
	***/
	bool SameConstantAs(const StereoShaderConstant<T> & other)
	{
		return ((other.m_StartRegister == m_StartRegister) &&
			(other.m_modification->m_ModificationID == m_modification->m_ModificationID));
	}
	/**
	* Returns pointer to left data.
	***/
	T* DataLeftPointer() 
	{
		return &m_DataLeft[0];
	}
	/**
	* Returns pointer to right data.
	***/
	T* DataRightPointer() 
	{
		return &m_DataRight[0];
	}
	/**
	* Returns the shader constant start register.
	***/
	UINT StartRegister() { return m_StartRegister; }
	/**
	* Returns the register count.
	* (4*float == 1 : Vector4 == 1, D3DMATRIX == 4,...)
	***/
	UINT Count() { return m_Count; }
private:
	/**
	* Original constant data, currently not used but assigned.
	* Number of T in a Register (L) = (4 for float/int, 1 for bool).
	***/
	std::vector<T> m_DataOriginal;
	/**
	* Left constant data.
	* Number of T in a Register (L) = (4 for float/int, 1 for bool)
	***/
	std::vector<T> m_DataLeft;
	/**
	* Right constant data
	* Number of T in a Register (L) = (4 for float/int, 1 for bool)
	***/
	std::vector<T> m_DataRight;
	/**
	* Pointer to constant modification.
	***/
	std::shared_ptr<ShaderConstantModification<T>> m_modification;
	/**
	* Shader start register.
	***/
	UINT m_StartRegister;
	/**
	* Shader constant size.
	* (4*float == 1 : Vector4 == 1, D3DMATRIX == 4,...)
	***/
	UINT m_Count;	
};
#endif