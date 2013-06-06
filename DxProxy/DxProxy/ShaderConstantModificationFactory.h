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

#ifndef SHADERCONSTANTMODIFICATIONFACTORY_H_INCLUDED
#define SHADERCONSTANTMODIFICATIONFACTORY_H_INCLUDED

#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "Vec4SimpleTranslate.h"
#include "ShaderConstantModification.h"


class ShaderConstantModificationFactory
{
public:
	enum Vector4ModificationTypes
	{
		SimpleTranslate = 0
	};

	enum MatrixModificationTypes
	{
		TranslateWithView = 0, //unreal
		TranslateWithViewColMajor = 1, // source
	};

	static std::shared_ptr<ShaderConstantModification<D3DXVECTOR4>> CreateVector4Modification(UINT modID)
	{
		return CreateVector4Modification(static_cast<Vector4ModificationTypes>(modID));
	}

	static std::shared_ptr<ShaderConstantModification<D3DXVECTOR4>> CreateVector4Modification(Vector4ModificationTypes mod)
	{
		switch (mod)
		{
		case SimpleTranslate:
			return std::make_shared<Vec4SimpleTranslate>(mod);
			break;

		default:
			throw std::out_of_range ("Nonexistant Vec4 modification");
			break;
		}
	}

	static std::shared_ptr<ShaderConstantModification<D3DXMATRIX>> CreateMatrixModification(UINT modID) 
	{
		return CreateMatrixModification(static_cast<MatrixModificationTypes>(modID));
	}

	static std::shared_ptr<ShaderConstantModification<D3DXMATRIX>> CreateMatrixModification(MatrixModificationTypes mod)
	{
		switch (mod)
		{
		case TranslateWithView:
		case TranslateWithViewColMajor:

		default:
			throw std::out_of_range ("Nonexistant matrix modification");
			break;
		}
	}
};


#endif