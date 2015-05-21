#include "ShaderConstantModificationFactory.h"

bool MatrixIsOrtho(const D3DXMATRIX &matrix)
{
	// (This is not what defines an orthographic matrix, but apparently it's
	// a reliable correlate? --jimrandomh)
	return vireio::AlmostSame(matrix[15], 1.0f, 0.00001f);
}

bool MatrixIsHUD(const D3DXMATRIX &matrix)
{
	// Add all translation and scale matrix entries.
	// For the GUI this should be 3.0f, for the HUD above.
	// (I have no idea why that would be the case, I just refactored it to here. --jimrandomh)
	float allAbs = abs(matrix(3, 0)); // transX
	allAbs += abs(matrix(3, 1)); // transY
	allAbs += abs(matrix(3, 2)); // transZ

	allAbs += abs(matrix(0, 0)); // scaleX
	allAbs += abs(matrix(1, 1)); // scaleY
	allAbs += abs(matrix(2, 2)); // scaleZ
	
	return allAbs>3.0f;
}


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
	ShaderMatrixModificationParams params(mod, adjustmentMatrices, transpose);
	
	switch (mod)
	{
	case MatDoNothing:
		return std::make_shared<MatrixDoNothing>(params);

	case MatSimpleTranslate:
		return std::make_shared<ShaderMatrixModification>(params);

	case MatOrthographicSquash:
		return std::make_shared<MatrixOrthoSquash>(params);

	case MatHudSlide:
		return std::make_shared<MatrixHudSlide>(params);

	case MatGuiSquash:
		return std::make_shared<MatrixGuiSquash>(params);

	case MatSurfaceRefractionTransform:
		return std::make_shared<MatrixSurfaceRefractionTransform>(params);

	case MatGatheredOrthographicSquash:
		return std::make_shared<MatrixGatheredOrthoSquash>(params);

	case MatOrthographicSquashShifted:
		return std::make_shared<MatrixOrthoSquashShifted>(params);

	case MatOrthographicSquashHud:
		return std::make_shared<MatrixOrthoSquashHud>(params);

	case MatConvergenceOffset:
		return std::make_shared<MatrixConvOffsetAdjustment>(params);

	case MatSimpleTranslateIgnoreOrtho:
		return std::make_shared<MatrixIgnoreOrtho>(params);

	case MatRollOnly:
		return std::make_shared<MatrixRollOnly>(params);

	case MatRollOnlyNegative:
		return std::make_shared<MatrixRollOnlyNegative>(params);

	case MatRollOnlyHalf:
		return std::make_shared<MatrixRollOnlyHalf>(params);

	case MatNoRoll:
		return std::make_shared<MatrixNoRoll>(params);

	case MatSimpleTranslateNoPositional:
		return std::make_shared<MatrixNoPositional>(params);
	
	case MatNoStereoSeparate:
		return std::make_shared<MatrixNoStereoSeparate>(params);
	
	default:
		OutputDebugString("Nonexistant matrix modification\n");
		assert(false);
		throw std::out_of_range ("Nonexistant matrix modification");
	}
}
