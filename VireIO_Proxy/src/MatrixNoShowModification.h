#ifndef MATRIXNOSHOWMODIFICATION_H_INCLUDED
#define MATRIXNOSHOWMODIFICATION_H_INCLUDED
/**
* @file MatrixNoShowModification.h
* Matrix adjustment to hide the shader
*/

#include "d3d9.h"
#include "d3dx9.h"
#include "ShaderConstantModification.h"
#include "Vireio.h"
#include "ShaderMatrixModification.h"
#include <sstream>

/**
* Matrix ignores head roll.
*/
class MatrixNoShowModification : public ShaderMatrixModification
{
public:
	MatrixNoShowModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustementMatrices, bool transpose);
	virtual void DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outRight);

private:
	std::string FloatToString(float f)
		{
		std::ostringstream converterString;
		converterString << f;
		return converterString.str();
}
};

#endif
