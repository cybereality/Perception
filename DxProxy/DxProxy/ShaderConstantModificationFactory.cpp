#include "ShaderConstantModificationFactory.h"

/**
* Calls twin function.
***/
std::shared_ptr<ShaderConstantModification<>> ShaderConstantModificationFactory::CreateVector4Modification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices)
{
	return CreateVector4Modification(static_cast<Vector4ModificationTypes>(modID), adjustmentMatrices);
}

/**
* Creates Vector4 modification.
* @see Vector4SimpleTranslate
***/
std::shared_ptr<ShaderConstantModification<>> ShaderConstantModificationFactory::CreateVector4Modification(Vector4ModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices)
{
	switch (mod)
	{
	case Vec4SimpleTranslate:
		return std::make_shared<Vector4SimpleTranslate>(mod, adjustmentMatrices);
	case Vec4EyeShiftUnity:
		return std::make_shared<Vector4EyeShiftUnity>(mod, adjustmentMatrices);
	case Vec4DeadIslandScaled:
		return std::make_shared<Vector4DeadIslandScaled>(mod, adjustmentMatrices);
	
	default:
		OutputDebugString("Nonexistant Vec4 modification\n");
		assert(false);
		throw std::out_of_range ("Nonexistant Vec4 modification");
	}
}

/**
* Calls twin function.
***/
std::shared_ptr<ShaderConstantModification<>> ShaderConstantModificationFactory::CreateMatrixModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose) 
{
	return CreateMatrixModification(static_cast<MatrixModificationTypes>(modID), adjustmentMatrices, transpose);
}

/**
* Creates matrix modification.
* @see MatrixDoNothing
* @see ShaderMatrixModification
* @see MatrixIgnoreOrtho
* @see MatrixGuiSquash
***/
std::shared_ptr<ShaderConstantModification<>> ShaderConstantModificationFactory::CreateMatrixModification(MatrixModificationTypes mod, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
{
	switch (mod)
	{
	case MatDoNothing:
		return std::make_shared<MatrixDoNothing>(mod, adjustmentMatrices);

	case MatSimpleTranslate:
		return std::make_shared<ShaderMatrixModification>(mod, adjustmentMatrices, transpose);

	case MatOrthographicSquash:
		return std::make_shared<MatrixOrthoSquash>(mod, adjustmentMatrices, transpose);

	case MatHudSlide:
		return std::make_shared<MatrixHudSlide>(mod, adjustmentMatrices, transpose);

	case MatGuiSquash:
		return std::make_shared<MatrixGuiSquash>(mod, adjustmentMatrices, transpose);

	case MatSurfaceRefractionTransform:
		return std::make_shared<MatrixSurfaceRefractionTransform>(mod, adjustmentMatrices, transpose);

	case MatGatheredOrthographicSquash:
		return std::make_shared<MatrixGatheredOrthoSquash>(mod, adjustmentMatrices, transpose);

	case MatOrthographicSquashShifted:
		return std::make_shared<MatrixOrthoSquashShifted>(mod, adjustmentMatrices, transpose);

	case MatOrthographicSquashHud:
		return std::make_shared<MatrixOrthoSquashHud>(mod, adjustmentMatrices, transpose);

	case MatConvergenceOffset:
		return std::make_shared<MatrixConvOffsetAdjustment>(mod, adjustmentMatrices, transpose);

	case MatSimpleTranslateIgnoreOrtho:
		return std::make_shared<MatrixIgnoreOrtho>(mod, adjustmentMatrices, transpose);

	case MatRollOnly:
		return std::make_shared<MatrixRollOnly>(mod, adjustmentMatrices, transpose);

	case MatRollOnlyNegative:
		return std::make_shared<MatrixRollOnlyNegative>(mod, adjustmentMatrices, transpose);

	case MatRollOnlyHalf:
		return std::make_shared<MatrixRollOnlyHalf>(mod, adjustmentMatrices, transpose);

	case MatNoRoll:
		return std::make_shared<MatrixNoRoll>(mod, adjustmentMatrices, transpose);

	case MatSimpleTranslateNoPositional:
		return std::make_shared<MatrixNoPositional>(mod, adjustmentMatrices, transpose);
	
	case MatNoStereoSeparate:
		return std::make_shared<MatrixNoStereoSeparate>(mod, adjustmentMatrices, transpose);
	
	default:
		OutputDebugString("Nonexistant matrix modification\n");
		assert(false);
		throw std::out_of_range ("Nonexistant matrix modification");
	}
}
