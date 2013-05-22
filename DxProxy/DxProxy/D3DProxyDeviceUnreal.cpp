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
	m_bAdjustedShaderActive = false;
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



inline bool D3DProxyDeviceUnreal::isPartOfMatrix(UINT Register)
{
	return ((Register >= matrixIndex) && (Register < (matrixIndex + 4)));
}


HRESULT WINAPI D3DProxyDeviceUnreal::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	m_bAdjustedShaderActive = false;

	return D3DProxyDevice::Reset(pPresentationParameters);
}



HRESULT WINAPI D3DProxyDeviceUnreal::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	
	if (stereoView->initialized) {
		if (Vector4fCount >= 4 && validRegister(StartRegister)) // && (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
		{
			D3DXMATRIX tempMatrix = const_cast<float*>(pConstantData);
		
			D3DXMATRIX tempLeft (tempMatrix * matViewTranslationLeft);
			D3DXMATRIX tempRight (tempMatrix * matViewTranslationRight);

			HRESULT result = BaseDirect3DDevice9::SetVertexShaderConstantF(StartRegister, (m_currentRenderingSide == Left) ? (float*)&tempLeft : (float*)&tempRight, Vector4fCount);
			// TODO move this up to basic proxy
			if (SUCCEEDED(result)) {
				if (m_pCapturingStateTo) {
					m_pCapturingStateTo->SelectAndCaptureState(StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4));
				}
				else {
					if (m_activeStereoVShaderConstF.count(StartRegister) == 1) {
						m_activeStereoVShaderConstF.erase(StartRegister);
					}

					m_activeStereoVShaderConstF.insert(std::pair<UINT, StereoShaderConstant<float>>(StartRegister, StereoShaderConstant<float>(StartRegister, tempLeft, tempRight, Vector4fCount, 4)));
					m_bAdjustedShaderActive = true;
				}
			}

			

			/*
			char buf[32];
			LPCSTR psz = NULL;

			sprintf_s(buf, "sep: %f", separation);
			psz = buf;
			OutputDebugString(psz);
			OutputDebugString("\n");
			*/

			/*if(false && saveDebugFile)
			{
				char vcString[1024];
				sprintf_s(vcString, 
					"register: %d, count: %d\n"
					"%.4f\t%.4f\t%.4f\t%.4f\n" 
					"%.4f\t%.4f\t%.4f\t%.4f\n"
					"%.4f\t%.4f\t%.4f\t%.4f\n"
					"%.4f\t%.4f\t%.4f\t%.4f\n\n", StartRegister, Vector4fCount,
					tempMatrix[0], tempMatrix[1], tempMatrix[2], tempMatrix[3],
					tempMatrix[4], tempMatrix[5], tempMatrix[6], tempMatrix[7],
					tempMatrix[8], tempMatrix[9], tempMatrix[10], tempMatrix[11],
					tempMatrix[12], tempMatrix[13], tempMatrix[14], tempMatrix[15]
					);

				debugFile << vcString;
			}*/


			return result;


		}
		// The matrix is being partially or completely replaced by something other than the matrix
		else if (m_bAdjustedShaderActive && isPartOfMatrix(StartRegister)) {

			HRESULT result;
			if (SUCCEEDED(result = D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount))) {

				if (!m_pCapturingStateTo) {
					// Remove the stereo matrix from active stereo constants
					if (m_activeStereoVShaderConstF.count(StartRegister) == 1) {

						m_activeStereoVShaderConstF.erase(StartRegister);
					}

					m_bAdjustedShaderActive = false;
				}
			}

			return result;
		}
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}





bool D3DProxyDeviceUnreal::setDrawingSide(EyeSide side)
{
	if (!D3DProxyDevice::setDrawingSide(side))
		return false;

	if (m_bAdjustedShaderActive) {
		
		auto stereoConstant = m_activeStereoVShaderConstF.find(matrixIndex);
		if (stereoConstant != m_activeStereoVShaderConstF.end()) {
			D3DProxyDevice::SetVertexShaderConstantF(stereoConstant->first, (m_currentRenderingSide == Left) ? stereoConstant->second.DataLeftPointer() : stereoConstant->second.DataRightPointer(), stereoConstant->second.Count);
		}
		else {
			OutputDebugString("No shader active but m_bAdjustedShaderActive flag says there is one.\n");
		}
	}

	return true;
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