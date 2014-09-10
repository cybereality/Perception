#include "MatrixNoShowModification.h"

MatrixNoShowModification::MatrixNoShowModification(UINT modID, std::shared_ptr<ViewAdjustment> adjustmentMatrices, bool transpose)
	: ShaderMatrixModification(modID, adjustmentMatrices, transpose)
{ }

void MatrixNoShowModification::DoMatrixModification(D3DXMATRIX in, D3DXMATRIX& outLeft, D3DXMATRIX& outRight)
{

	// just for debugging purposes.
	//std::string matrixString = "";
	//matrixString = FloatToString(m_spAdjustmentMatrices->RollMatrix()._11); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._12); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._13); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._14); 
	//OutputDebugString(matrixString.c_str());
	//matrixString = FloatToString(m_spAdjustmentMatrices->RollMatrix()._21); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._22); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._23); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._24); 
	//OutputDebugString(matrixString.c_str());
	//matrixString = FloatToString(m_spAdjustmentMatrices->RollMatrix()._31); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._32); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._33); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._34); 
	//OutputDebugString(matrixString.c_str());
	//matrixString = FloatToString(m_spAdjustmentMatrices->RollMatrix()._31); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._32); 
	//matrixString += " ";
	//matrixString += FloatToString(m_spAdjustmentMatrices->RollMatrix()._33); 
	//matrixString += " ";
	//matrixString += FloatToString(in._34); 
	//OutputDebugString(matrixString.c_str());
	//outLeft = in * m_spAdjustmentMatrices->RollMatrixNegative();
	//outRight = in * m_spAdjustmentMatrices->RightViewTransform();
}

