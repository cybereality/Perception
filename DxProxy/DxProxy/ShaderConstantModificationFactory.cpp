#include "ShaderConstantModificationFactory.h"
#include "D3DProxyDevice.h"


/**
* Convergence Offset adjustment.
*/
class MatrixConvOffsetAdjustment : public ShaderMatrixModification
{
public:
	MatrixConvOffsetAdjustment(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Fixes far objects like a night sky.
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in * m_spAdjustmentMatrices->LeftView();
		outright = in * m_spAdjustmentMatrices->RightView();
	}
};

/**
* Simple modification that does not apply anything. Is needed for certain methods that could do things but sometimes won't.
*/
class MatrixDoNothing : public ShaderMatrixModification
{
public:
	MatrixDoNothing(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Simply returns True.
	* @param in Irrelevant parameter.
	***/
	virtual bool DoNotApply(D3DXMATRIX in)
	{
		return true;
	}
};

/**
* Squishes the matrix if orthographical, otherwise simple modification.
*/
class MatrixOrthoSquash : public ShaderMatrixModification
{
public:
	MatrixOrthoSquash(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Matrix modification does multiply: shiftprojection * squash (for GUI), scale * transform * distance (for HUD).
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (MatrixIsOrtho(in))
		{
			// HUD
			if (MatrixIsHUD(in))
			{
				// separation -> distance translation
				outLeft = in * m_spAdjustmentMatrices->LeftHUDMatrix();
				outright = in * m_spAdjustmentMatrices->RightHUDMatrix();
			}
			else // GUI
			{
				// simple squash
				outLeft = in *  m_spAdjustmentMatrices->LeftGUIMatrix();
				outright = in * m_spAdjustmentMatrices->RightGUIMatrix();
			}
		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	}
};

/**
* Squishes the matrix if orthographical, otherwise simple modification.
*/
class MatrixGatheredOrthoSquash : public MatrixOrthoSquash
{
public:
	MatrixGatheredOrthoSquash(ShaderMatrixModificationParams params) : MatrixOrthoSquash(params)
	{
	}

	/**
	* Applies modification to registers.
	* This method should not generally be overridden by subclasses (if it is the overriding method
	* should respect the result of DoNotApply).
	* Do modification in DoMatrixModification. Transposed matrices will be provided to
	* DoMatrixModification if transpose is true.
	* @param [in] inData Input matrix to be modified and assigned to registers.
	* @param [in, out] outLeft Register vector left.
	* @param [in, out] outRight Register vector right.
	***/
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXMATRIX tempMatrix (inData);

		// conditions to apply the matrix ?
		if (DoNotApply(tempMatrix)) {

			outLeft->assign(&tempMatrix[0], &tempMatrix[0] + outLeft->size());
			outRight->assign(&tempMatrix[0], &tempMatrix[0] + outRight->size());
		}
		else {

			// matrix to be transposed ?
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			}
			
			D3DXMATRIX tempLeft;
			D3DXMATRIX tempRight;

			// do modification
			DoMatrixModification(tempMatrix, tempLeft, tempRight);

			// transpose back
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempLeft, &tempLeft);
				D3DXMatrixTranspose(&tempRight, &tempRight);
			}

			// assign to output
			outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
			outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());

			// gather matrix
			m_spAdjustmentMatrices->GatherMatrix(tempLeft, tempRight);
		}
	}
};

/**
* Matrix implementation whose purpose is to squash the GUI acordingly to the driver settings. 
*/
class MatrixGuiSquash : public ShaderMatrixModification
{
public:
	MatrixGuiSquash(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Matrix modification does multiply: translation * squash.
	* *  Does the matrix squash and outputs the results.  Does not only affect HUD at this point in time (August 22nd, 2013).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	* TODO probably don't want to be translating the HUD around.
	* Need an adjustment that does unproject, reproject left/right? (then squash)
	* Refactor modifications into view adjustments? Or just make unproject->reproject a standard adjustment in adjustments?
	* Or ???
	* TODO Refactor so adjustments aren't recalculated every time constant is applied that don't need to be
	* Shift adustments to some kind of indexed lookup in view adjustments with adjustments only being updated if dirty?
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// simple squash
		outLeft = in * m_spAdjustmentMatrices->LeftGUIMatrix();
		outright = in * m_spAdjustmentMatrices->RightGUIMatrix();
	}
};

/**
* Slides the HUD into the HMD output.
* All orthographic matrices treated as HUD here.
*/
class MatrixHudSlide : public ShaderMatrixModification
{
public:
	MatrixHudSlide(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Slides the HUD into the HMD output.
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// HUD
		// separation -> distance translation
		outLeft = in * m_spAdjustmentMatrices->LeftHUDMatrix();
		outright = in * m_spAdjustmentMatrices->RightHUDMatrix();
	}
};

#if 0
/**
* Incomplete matrix implementation whose purpose is to squash the hud.  Right now will squish the whole matrix.
*/
class MatrixHudSquash : public ShaderMatrixModification
{
public:
	MatrixHudSquash(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
		// should this be hard coded?  Seems a bit fishy... - Josh
		D3DXMatrixScaling(&squash, 0.5f, 0.5f, 1);
	}

	/**
	* Matrix modification does multiply: translation * squash.
	* *  Does the matrix squash and outputs the results.  Does not only affect HUD at this point in time (August 22nd, 2013).
	* @param in The matrix to be multiply by the adjustmentMatricies.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	* TODO probably don't want to be translating the HUD around.
	* Need an adjustment that does unproject, reproject left/right? (then squash)
	* Refactor modifications into view adjustments? Or just make unproject->reproject a standard adjustment in adjustments?
	* Or ???
	* TODO Refactor so adjustments aren't recalculated every time constant is applied that don't need to be
	* Shift adustments to some kind of indexed lookup in view adjustments with adjustments only being updated if dirty?
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in * m_spAdjustmentMatrices->LeftShiftProjection() * squash;
		outright = in * m_spAdjustmentMatrices->RightShiftProjection() * squash;
	}

private:
	/**
	* Squash scaling matrix, obtained by the D3DXMatrixScaling.
	*/
	D3DXMATRIX squash;
};
#endif

/**
* Modification to ignore orthographic matrices.
* This is the intent, not sure that the math holds up but it works for HL2. 
* (Doesn't seem to work for omd2, stops all separation)
*/
class MatrixIgnoreOrtho : public ShaderMatrixModification
{
public:
	MatrixIgnoreOrtho(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{}

	/**
	* Don't apply if matrix is orthographic.
	* TODO this was the quick way to get the hud positioned correctly (same result as
	* (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
	* in the old code. The 'correct' way to do this in this version would be to have an override
	* shader rule that applies to the shader used for the ui; assuming it's not used
	* for other models as well... if it is this seems like the only option unless there is another
	* way to identify the constant from it's properties rather than the actual value.
	***/
	virtual bool DoNotApply(D3DXMATRIX in)
	{
		return MatrixIsOrtho(in);
	}
};

/**
* Simple translate modification that is unaffected by positional tracking
*/
class MatrixNoPositional : public ShaderMatrixModification
{
public:
	MatrixNoPositional(ShaderMatrixModificationParams params)
		: ShaderMatrixModification(params)
	{}

	/**
	* Default modification is simple translate. Override to do actual modification.
	* @param in [in] Modification matrix to be multiplied by adjustment matrix (left/right).
	* @param outLeft [in, out] Left transform matrix.
	* @param outRight [in, out] Right transform matrix.
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in * m_spAdjustmentMatrices->LeftAdjustmentMatrix();
		outright = in * m_spAdjustmentMatrices->RightAdjustmentMatrix();
	}
};

/**
* Matrix applies modification without head roll.
* For shadow fixes.
*/
class MatrixNoRoll : public ShaderMatrixModification
{
public:
	MatrixNoRoll(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// in * rollMatrix
		outLeft = in
			* m_spAdjustmentMatrices->LeftAdjustmentMatrixNoRoll()
			* m_spAdjustmentMatrices->ScreenSpacePositionLeft();
		outright = in
			* m_spAdjustmentMatrices->RightAdjustmentMatrixNoRoll()
			* m_spAdjustmentMatrices->ScreenSpacePositionRight();
	}
};

#if 0
/**
* Matrix ignores head roll.
*/
class MatrixNoShowModification : public ShaderMatrixModification
{
public:
	MatrixNoShowModification(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}
	
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outRight)
	{
		// just for debugging purposes.
		//D3DXMATRIX rollMatrix = m_spAdjustmentMatrices->RollMatrix();
		//debugf("%s\n", MatrixToString(&rollMatrix).c_str());
		//outLeft = in * m_spAdjustmentMatrices->RollMatrixNegative();
		//outRight = in * m_spAdjustmentMatrices->RightViewTransform();
	}

private:
	std::string FloatToString(float f)
	{
		std::ostringstream converterString;
		converterString << f;
		return converterString.str();
	}
};
#endif

/**
* Simple modification that is affected by positional tracking only (no stereo separation)
*/
class MatrixNoStereoSeparate : public ShaderMatrixModification
{
public:
	MatrixNoStereoSeparate(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}
	
	/**
	* Default modification is simple translate. Override to do actual modification.
	* @param in [in] Modification matrix to be multiplied by adjustment matrix (left/right).
	* @param outLeft [in, out] Left transform matrix.
	* @param outRight [in, out] Right transform matrix.
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		outLeft = in; * m_spAdjustmentMatrices->ScreenSpacePositionLeft();
		outright = in; * m_spAdjustmentMatrices->ScreenSpacePositionRight();
	}
};

/**
* Squishes the matrix if orthographical, otherwise simple modification.
* All orthographic matrices treated as HUD here.
*/
class MatrixOrthoSquashHud : public ShaderMatrixModification
{
public:
	MatrixOrthoSquashHud(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Matrix modification does multiply: shiftprojection * squash (for GUI), scale * transform * distance (for HUD).
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (MatrixIsOrtho(in)) {

			// HUD
			// separation -> distance translation
			outLeft = in * m_spAdjustmentMatrices->LeftHUDMatrix();
			outright = in * m_spAdjustmentMatrices->RightHUDMatrix();

		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	}
};

/**
* Squishes the matrix if orthographical, otherwise simple modification.
* HUD matrices shifted here accordingly to HUD distance.
*/
class MatrixOrthoSquashShifted : public ShaderMatrixModification
{
public:
	MatrixOrthoSquashShifted(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
	}

	/**
	* Matrix modification does multiply: shiftprojection * squash (for GUI), scale * transform * distance (for HUD).
	* Does the matrix squash and outputs the results.  Does only affect HUD (or GUI).
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		if (MatrixIsOrtho(in))
		{
			// TODO !! compute these two following matrices in the ViewAdjustment class :

			// HUD
			if (MatrixIsHUD(in))
			{
				// separation -> distance translation
				D3DXMATRIX orthoToPersViewProjTransformLeft  =
					m_spAdjustmentMatrices->ProjectionInverse()
					* m_spAdjustmentMatrices->LeftHUD3DDepthShifted()
					* m_spAdjustmentMatrices->LeftViewTransform()
					* m_spAdjustmentMatrices->HUDDistance()
					*  m_spAdjustmentMatrices->Projection();
				D3DXMATRIX orthoToPersViewProjTransformRight =
					m_spAdjustmentMatrices->ProjectionInverse()
					* m_spAdjustmentMatrices->RightHUD3DDepthShifted()
					* m_spAdjustmentMatrices->RightViewTransform()
					* m_spAdjustmentMatrices->HUDDistance()
					* m_spAdjustmentMatrices->Projection();
				
				outLeft = in * orthoToPersViewProjTransformLeft;
				outright = in * orthoToPersViewProjTransformRight;
			}
			else // GUI
			{
				// simple squash
				outLeft = in
					* m_spAdjustmentMatrices->ProjectionInverse()
					* m_spAdjustmentMatrices->LeftGUI3DDepth()
					* m_spAdjustmentMatrices->Squash()
					* m_spAdjustmentMatrices->Projection();
				outright = in * m_spAdjustmentMatrices->ProjectionInverse()
					* m_spAdjustmentMatrices->RightGUI3DDepth()
					* m_spAdjustmentMatrices->Squash()
					* m_spAdjustmentMatrices->Projection();
			}
		}
		else {
			ShaderMatrixModification::DoMatrixModification(in, outLeft, outright);
		}
	}
};

/**
* Matrix applies only the head roll.
* For shadow fixes.
*/
class MatrixRollOnly : public ShaderMatrixModification
{
public:
	MatrixRollOnly(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{}

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// in * rollMatrix
		outLeft = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrix()
			* m_spAdjustmentMatrices->Projection();
		outright = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrix()
			* m_spAdjustmentMatrices->Projection();
	}
};

/**
* Matrix applies only the half head roll.
* For shadow fixes.
*/
class MatrixRollOnlyHalf : public ShaderMatrixModification
{
public:
	MatrixRollOnlyHalf(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{}

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// in * rollMatrix
		outLeft = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrixHalf()
			* m_spAdjustmentMatrices->Projection();
		outright = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrixHalf()
			* m_spAdjustmentMatrices->Projection();
	}
};

/**
* Matrix applies only the head roll.
* For shadow fixes.
*/
class MatrixRollOnlyNegative : public ShaderMatrixModification
{
public:
	MatrixRollOnlyNegative(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{}

	/**
	* @param in The matrix to be multiply by the adjustmentMatrices.
	* @param[out] outLeft The resulting left side matrix
	* @param[out] outRight The resulting right side matrix
	***/
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outright)
	{
		// in * rollMatrix
		outLeft = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrixNegative()
			* m_spAdjustmentMatrices->Projection();
		outright = in
			* m_spAdjustmentMatrices->ProjectionInverse()
			* m_spAdjustmentMatrices->RollMatrixNegative()
			* m_spAdjustmentMatrices->Projection();
	}
};

/**
* Squishes the matrix if orthographical, otherwise simple modification.
*/
class MatrixSurfaceRefractionTransform : public ShaderMatrixModification
{
public:
	/**
	*  Constructor, sets attributes and such.
	* @param modID The id for this matrix modification.
	* @param adjustmentMatrices The matricies to be adjusted
	* @param transpose Decides if the matrices should be transposed (aka: have rows and columns interchanged)
	*/
	MatrixSurfaceRefractionTransform(ShaderMatrixModificationParams params) : ShaderMatrixModification(params)
	{
		D3DXMatrixTranslation(&translateUV, 0.5f, 0.5f, 0.5f);
		D3DXMatrixScaling(&scaleUV, 0.5f, 0.5f, 0.0f);
	}

	/**
	* Applies surface refraction modification to registers, uses gathered matrix.
	* @param [in] inData Input matrix to be modified and assigned to registers.
	* @param [in, out] outLeft Register vector left.
	* @param [in, out] outRight Register vector right.
	***/
	virtual void ApplyModification(const float* inData, std::vector<float>* outLeft, std::vector<float>* outRight)
	{
		D3DXMATRIX tempMatrix (inData);

		// conditions to apply the matrix ?
		if (DoNotApply(tempMatrix)) {

			outLeft->assign(&tempMatrix[0], &tempMatrix[0] + outLeft->size());
			outRight->assign(&tempMatrix[0], &tempMatrix[0] + outRight->size());
		}
		else {
			// get gathered matrices
			D3DXMATRIX tempLeft(m_spAdjustmentMatrices->GatheredMatrixLeft());
			D3DXMATRIX tempRight(m_spAdjustmentMatrices->GatheredMatrixRight());

			// matrix to be transposed ?
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempLeft, &tempLeft);
				D3DXMatrixTranspose(&tempRight, &tempRight);
			}

			// use gathered matrices to be scaled and translated
			tempLeft = tempLeft * scaleUV * translateUV;
			tempRight = tempRight * scaleUV * translateUV;;

			// transpose back
			if (m_bTranspose) {
				D3DXMatrixTranspose(&tempLeft, &tempLeft);
				D3DXMatrixTranspose(&tempRight, &tempRight);
			}

			// assign to output
			outLeft->assign(&tempLeft[0], &tempLeft[0] + outLeft->size());
			outRight->assign(&tempRight[0], &tempRight[0] + outRight->size());
		}
	}

private:
	/**
	* Matrix used to translate the surface refraction texture UVs.
	***/
	D3DXMATRIX translateUV;
	/**
	* Matrix used to scale the surface refraction texture UVs.
	***/
	D3DXMATRIX scaleUV;
};


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
