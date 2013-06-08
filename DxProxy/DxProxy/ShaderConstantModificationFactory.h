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
#include "Vector4SimpleTranslate.h"
#include "ShaderConstantModification.h"
#include "MatrixSimpleTranslate.h"
#include "MatrixSimpleTranslateColMajor.h"


class ShaderConstantModificationFactory
{
public:
	enum Vector4ModificationTypes
	{
		Vec4SimpleTranslate = 0
	};

	enum MatrixModificationTypes
	{
		MatSimpleTranslate = 0, //unreal
		MatSimpleTranslateColMajor = 1 // source
	};

	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(UINT modID, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies)
	{
		return CreateVector4Modification(static_cast<Vector4ModificationTypes>(modID), adjustmentMatricies);
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(Vector4ModificationTypes mod, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies)
	{
		switch (mod)
		{
		case Vec4SimpleTranslate:
			return std::make_shared<Vector4SimpleTranslate>(mod, adjustmentMatricies);

		default:
			throw std::out_of_range ("Nonexistant Vec4 modification");
		}
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(UINT modID, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies) 
	{
		return CreateMatrixModification(static_cast<MatrixModificationTypes>(modID), adjustmentMatricies);
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(MatrixModificationTypes mod, std::shared_ptr<ViewAdjustmentMatricies> adjustmentMatricies)
	{
		switch (mod)
		{
		case MatSimpleTranslate:
			return std::make_shared<MatrixSimpleTranslate>(mod, adjustmentMatricies);

		case MatSimpleTranslateColMajor:
			return std::make_shared<MatrixSimpleTranslateColMajor>(mod, adjustmentMatricies);

		default:
			throw std::out_of_range ("Nonexistant matrix modification");
		}
	}
};


#endif