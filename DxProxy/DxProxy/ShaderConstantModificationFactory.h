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
#include "ShaderMatrixModification.h"
#include "MatrixIgnoreOrtho.h"
#include "MatrixDoNothing.h"


class ShaderConstantModificationFactory
{
public:

	enum Vector4ModificationTypes
	{
		Vec4DoNothing = 0,
		Vec4SimpleTranslate = 1
	};


	//enum MatrixModificationTypes
	//{
	//	MatDoNothing = 0,
	//	MatSimpleTranslate = 1, //unreal
	//	MatTSimpleTranslate = 2, 
	//	MatTSimpleTranslateIgnoreOrtho = 3// source
	//};

	enum MatrixModificationTypes
	{
		MatDoNothing = 0,
		MatSimpleTranslate = 1, 
		MatSimpleTranslateIgnoreOrtho = 3
	};


	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies)
	{
		return CreateVector4Modification(static_cast<Vector4ModificationTypes>(modID), adjustmentMatricies);
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateVector4Modification(Vector4ModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatricies)
	{
		switch (mod)
		{
		case Vec4SimpleTranslate:
			return std::make_shared<Vector4SimpleTranslate>(mod, adjustmentMatricies);

		default:
			OutputDebugString("Nonexistant Vec4 modification\n");
			assert(false);
			throw std::out_of_range ("Nonexistant Vec4 modification");
		}
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose) 
	{
		return CreateMatrixModification(static_cast<MatrixModificationTypes>(modID), adjustmentMatricies, transpose);
	}

	static std::shared_ptr<ShaderConstantModification<>> CreateMatrixModification(MatrixModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatricies, bool transpose)
	{
		switch (mod)
		{
		case MatDoNothing:
			return std::make_shared<MatrixDoNothing>(mod, adjustmentMatricies);

		case MatSimpleTranslate:
			return std::make_shared<ShaderMatrixModification>(mod, adjustmentMatricies, transpose);
			
		case MatSimpleTranslateIgnoreOrtho:
			return std::make_shared<MatrixIgnoreOrtho>(mod, adjustmentMatricies, transpose);

		default:
			OutputDebugString("Nonexistant matrix modification\n");
			assert(false);
			throw std::out_of_range ("Nonexistant matrix modification");
		}
	}
};


#endif