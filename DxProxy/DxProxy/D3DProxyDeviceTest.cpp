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

#include "D3DProxyDeviceTest.h"

D3DProxyDeviceTest::D3DProxyDeviceTest(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy)
{
}

D3DProxyDeviceTest::~D3DProxyDeviceTest()
{
}

void D3DProxyDeviceTest::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Test Init\n");
	D3DProxyDevice::Init(cfg);
	matrixIndex = getMatrixIndex();
}

HRESULT WINAPI D3DProxyDeviceTest::BeginScene()
{
	HandleControls();
//	HandleTracking();
	ComputeViewTranslation();

	if(saveDebugFile)
	{
		debugFile.open("d3d9_debug.txt", std::ios::out);
	}

	return D3DProxyDevice::BeginScene();
}

HRESULT WINAPI D3DProxyDeviceTest::EndScene()
{
	D3DRECT rec = {1,1,25,25};
	this->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(225,255,255,0),0 ,0);

	

	RECT rec2 = {30,10,400,200};
	char vcString[512];
	sprintf_s(vcString, 512, "mat index: %d\n",
		matrixIndex
	);
	//hudFont->DrawText(NULL, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

	return D3DProxyDevice::EndScene();
}


HRESULT WINAPI D3DProxyDeviceTest::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	
	if(stereoView->initialized && validVectorCount(Vector4fCount) && validRegister(StartRegister))
	{
		
		currentMatrix = const_cast<float*>(pConstantData);
	
		D3DXMATRIX sourceMatrix(currentMatrix);
		
		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);
		//D3DXMatrixInverse(&sourceMatrix, 0, &sourceMatrix);

		D3DXMATRIX worldViewMatrix;
		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);
		D3DXMATRIX worldViewTransMatrix;

		//D3DXMatrixTranslation(&transMatrix, separation*eyeShutter*50.0f, 0, 0);

		D3DXMatrixMultiply(&worldViewMatrix, &sourceMatrix, &matProjectionInv);
		D3DXMatrixMultiply(&worldViewTransMatrix, &worldViewMatrix, &transMatrix);

		/*
		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);
		D3DXMatrixTranslation(&transMatrix, separation*eyeShutter*100.0f, 0, 0);
		D3DXMatrixMultiply(&sourceMatrix, &transMatrix, &sourceMatrix);
		*/
		//sourceMatrix[matrixIndex] += separation*eyeShutter*500.0f;
		worldViewTransMatrix[matrixIndex] += separation*eyeShutter*5.0f;

		D3DXMatrixMultiply(&sourceMatrix, &worldViewTransMatrix, &matProjection);

		//D3DXMatrixInverse(&sourceMatrix, 0, &sourceMatrix);
		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);
		

		/*
		D3DXMATRIX sourceMatrix(currentMatrix);
		
		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);
			
		D3DXMATRIX worldViewMatrix;
		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);
		D3DXMATRIX worldViewTransMatrix;

		D3DXMatrixTranslation(&transMatrix, separation*eyeShutter*10.0f, 0, 0);
		//D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &transMatrix);

		//D3DXMatrixMultiply(&worldViewMatrix, &sourceMatrix, &matProjectionInv);

		//transMatrix[8] += convergence*drawLeftEye;

		//D3DXMatrixMultiply(&worldViewTransMatrix, &worldViewMatrix, &transMatrix);
			
		//D3DXMATRIX rollMatrix;
		//D3DXMatrixRotationZ(&rollMatrix, currentRoll);
		//D3DXMatrixMultiply(&worldViewTransMatrix, &worldViewTransMatrix, &rollMatrix);

		//D3DXMatrixMultiply(&sourceMatrix, &worldViewTransMatrix, &matProjection);

		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);
		//
		//sourceMatrix[0] += separation*eyeShutter;

		currentMatrix = (float*)sourceMatrix;

		currentMatrix[matrixIndex] += separation*eyeShutter;
		*/

		if(saveDebugFile)
		{
			char vcString[1024];
			sprintf_s(vcString, 
				"register: %d, count: %d\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n" 
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n"
				"%.4f\t%.4f\t%.4f\t%.4f\n\n", StartRegister, Vector4fCount,
				currentMatrix[0], currentMatrix[1], currentMatrix[2], currentMatrix[3],
				currentMatrix[4], currentMatrix[5], currentMatrix[6], currentMatrix[7],
				currentMatrix[8], currentMatrix[9], currentMatrix[10], currentMatrix[11],
				currentMatrix[12], currentMatrix[13], currentMatrix[14], currentMatrix[15]
				);

			debugFile << vcString;
		}


		//sourceMatrix._13 += separation*eyeShutter;

		//currentMatrix = (float*)sourceMatrix;
		
		
		currentMatrix[0] = sourceMatrix._11;
		currentMatrix[1] = sourceMatrix._12;
		currentMatrix[2] = sourceMatrix._13;
		currentMatrix[3] = sourceMatrix._14;
		currentMatrix[4] = sourceMatrix._21;
		currentMatrix[5] = sourceMatrix._22;
		currentMatrix[6] = sourceMatrix._23;
		currentMatrix[7] = sourceMatrix._24;
		currentMatrix[8] = sourceMatrix._31;
		currentMatrix[9] = sourceMatrix._32;
		currentMatrix[10] = sourceMatrix._33;
		currentMatrix[11] = sourceMatrix._34;
		currentMatrix[12] = sourceMatrix._41;
		currentMatrix[13] = sourceMatrix._42;
		currentMatrix[14] = sourceMatrix._43;
		currentMatrix[15] = sourceMatrix._44;

		//currentMatrix[matrixIndex] += separation*eyeShutter*5.0f;

		//currentMatrix[matrixIndex] += separation*eyeShutter*50.0f;

		return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, currentMatrix, Vector4fCount);
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}



bool D3DProxyDeviceTest::validRegister(UINT reg)
{
	if(reg == 0) return true;
	else return false;
	//return true;
}

bool D3DProxyDeviceTest::validVectorCount(UINT count)
{
	//if(count == 4) return true;
	if(count >= 4) return true;
	else return false;
}


int D3DProxyDeviceTest::getMatrixIndex()
{
	return 0;
}

/*
HRESULT WINAPI D3DProxyDeviceTest::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	if(State == D3DTS_VIEW)
	{
		D3DXMATRIX sourceMatrix(*pMatrix);

		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);

		D3DXMatrixTranslation(&transMatrix, (separation*eyeShutter+offset)*500.0f, 0, 0);
		D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &transMatrix);

		return D3DProxyDevice::SetTransform(State, &sourceMatrix);
	}
	return D3DProxyDevice::SetTransform(State, pMatrix);
}
*/