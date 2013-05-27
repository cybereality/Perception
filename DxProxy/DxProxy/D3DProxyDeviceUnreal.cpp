/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#include "D3DProxyDeviceUnreal.h"

D3DProxyDeviceUnreal::D3DProxyDeviceUnreal(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy)
{
	//m_bAdjustedShaderActive = false;
}

D3DProxyDeviceUnreal::~D3DProxyDeviceUnreal()
{
}

void D3DProxyDeviceUnreal::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Unreal Init\n");
	D3DProxyDevice::Init(cfg);
	roll_mode = 1;
	matrixIndex = getMatrixIndex();
}


//
//inline bool D3DProxyDeviceUnreal::isPartOfMatrix(UINT Register)
//{
//	return ((Register >= matrixIndex) && (Register < (matrixIndex + 4)));
//}
//
//
//
//
//
//HRESULT WINAPI D3DProxyDeviceUnreal::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
//{
//	
//	if (stereoView->initialized) {
//		if (ContainsMatrixToModify(StartRegister, pConstantData, Vector4fCount)) // && (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
//		{
//			//D3DXMATRIX tempMatrix = const_cast<float*>(pConstantData);
//		
//			//D3DXMATRIX tempLeft (tempMatrix * matViewTranslationLeft);
//			//D3DXMATRIX tempRight (tempMatrix * matViewTranslationRight);
//			StereoShaderConstant<float> stereoConstant = CreateStereoShaderConstant(StartRegister, pConstantData, Vector4fCount);
//
//
//			HRESULT result = BaseDirect3DDevice9::SetVertexShaderConstantF(StartRegister, (m_currentRenderingSide == Left) ? stereoConstant.DataLeftPointer() : stereoConstant.DataRightPointer(), Vector4fCount);
//			// TODO move this up to basic proxy
//			if (SUCCEEDED(result)) {
//				if (m_pCapturingStateTo) {
//					//m_pCapturingStateTo->SelectAndCaptureState(StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4));
//					m_pCapturingStateTo->SelectAndCaptureState(stereoConstant);
//				}
//				else {
//					if (m_activeStereoVShaderConstF.count(StartRegister) == 1) {
//						m_activeStereoVShaderConstF.erase(StartRegister);
//					}
//
//					m_activeStereoVShaderConstF.insert(std::pair<UINT, StereoShaderConstant<float>>(StartRegister, stereoConstant));
//					//m_bAdjustedShaderActive = true;
//				}
//			}
//
//
//			return result;
//
//
//		}
//		// The matrix is being partially or completely replaced by something other than the matrix
//		else if (/*m_bAdjustedShaderActive &&*/ CouldOverwriteMatrix(StartRegister, Vector4fCount)) {
//
//			HRESULT result;
//			if (SUCCEEDED(result = D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount))) {
//
//				if (!m_pCapturingStateTo) {
//					// Remove the stereo matrix from active stereo constants
//					if (m_activeStereoVShaderConstF.count(StartRegister) == 1) {
//
//						m_activeStereoVShaderConstF.erase(StartRegister);
//					}
//
//					//m_bAdjustedShaderActive = false;
//				}
//			}
//
//			return result;
//		}
//	}
//
//	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
//}


bool D3DProxyDeviceUnreal::CouldOverwriteMatrix(UINT StartRegister, UINT Vector4fCount) 
{
	return ((StartRegister >= matrixIndex) && (StartRegister < (matrixIndex + Vector4fCount)));
}


bool D3DProxyDeviceUnreal::ContainsMatrixToModify(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return (Vector4fCount >= 4 && validRegister(StartRegister));
}

StereoShaderConstant<float> D3DProxyDeviceUnreal::CreateStereoShaderConstant(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	D3DXMATRIX tempMatrix = const_cast<float*>(pConstantData);
		
	D3DXMATRIX tempLeft (tempMatrix * matViewTranslationLeft);
	D3DXMATRIX tempRight (tempMatrix * matViewTranslationRight);

	return StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4);
}






bool D3DProxyDeviceUnreal::validRegister(UINT reg)
{
	switch(game_type)
	{
	case UNREAL_MIRROR:
	case UNREAL_UT3:
		if(reg == 0) return true;
		else return false;
		break;
	case UNREAL_BIOSHOCK:
		return true;
		break;
	default:
		if(reg == 0) return true;
		else return false;
	}
}

UINT D3DProxyDeviceUnreal::getMatrixIndex()
{
	switch(game_type)
	{
	case UNREAL_MIRROR:
	case UNREAL_UT3:
		return 0;
		break;
	case UNREAL_BIOSHOCK:
		return 2;
		break;
	default:
		return 0;
	}
}