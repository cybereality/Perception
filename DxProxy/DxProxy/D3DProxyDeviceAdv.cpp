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

#include "D3DProxyDeviceAdv.h"


//
//void transformIndividualFloat(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
//{
//	float pConstantData[4];
//	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 1);
//	pConstantData[0] += proxyDev->separation*proxyDev->eyeShutter*100.0f+proxyDev->offset;
//	pD3Ddev->SetVertexShaderConstantF(startRegister, pConstantData, 1);
//	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean));
//}
//
//void transformRowMajor4by4(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
//{
//	float pConstantData[16];
//	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 4);
//	D3DXMATRIX original = D3DXMATRIX(pConstantData);
//	D3DXMATRIX transposed;
//	D3DXMatrixTranspose(&transposed, &original);
//	proxyDev->adjustEyeOffsetAndViewFrustum(original, transposed);
//	D3DXMatrixTranspose(&transposed, &original);
//	pD3Ddev->SetVertexShaderConstantF(startRegister, transposed,4);
//	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean)*4);
//}
//
//void transform4by4(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
//{
//	float pConstantData[16];
//	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 4);
//	D3DXMATRIX inMatrix = D3DXMATRIX(pConstantData);
//	if(inMatrix._41 == 0.0f) return; //Mirror's Edge compat. hack (otherwise you get weird squares)
//	D3DXMATRIX outMatrix;
//	proxyDev->adjustEyeOffsetAndViewFrustum(outMatrix, inMatrix);
//	pD3Ddev->SetVertexShaderConstantF(startRegister, outMatrix,4);
//	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean)*4);
//}
//
//
//void D3DProxyDeviceAdv::parse4by4Matrices(D3DXCONSTANT_DESC &desc)
//{
//	if(desc.Name == NULL) return;
//	if(!strstr(desc.Name, "proj") && !strstr(desc.Name, "Proj"))return;
//	if(desc.RegisterCount != 4) return;
//	TargetMatrix tm;
//	tm.startRegister = desc.RegisterIndex;
//	tm.transformationFunc = (desc.Class == 2) ? &transformRowMajor4by4 : &transform4by4;
//	targetMatrices.push_back(tm);
//}
//
//void D3DProxyDeviceAdv::parseIndividualFloats(D3DXCONSTANT_DESC &desc)
//{
//	if(desc.Name == NULL) return;
//	if(!strstr(desc.Name, "EyePos")) return;
//	if(desc.RegisterCount != 1) return;
//	TargetMatrix tm;
//	tm.startRegister = desc.RegisterIndex;
//	tm.transformationFunc = &transformIndividualFloat;
//	targetMatrices.push_back(tm);
//}
//
//void D3DProxyDeviceAdv::findWeirdMirrorsEdgeShader(UINT pSizeOfData)
//{	
//	if(pSizeOfData != 172) return;
//	TargetMatrix tm;
//	tm.startRegister = 0;
//
//	tm.transformationFunc = &transform4by4;
//	targetMatrices.push_back(tm);
//}
