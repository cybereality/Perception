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

#include "D3DProxyDeviceEgo.h"

D3DProxyDeviceEgo::D3DProxyDeviceEgo(IDirect3DDevice9* pDevice):D3DProxyDevice(pDevice)
{
}

D3DProxyDeviceEgo::~D3DProxyDeviceEgo()
{
}

void D3DProxyDeviceEgo::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Ego Init\n");
	D3DProxyDevice::Init(cfg);
	matrixIndex = getMatrixIndex();
}

HRESULT WINAPI D3DProxyDeviceEgo::BeginScene()
{
	HandleControls();
	HandleTracking();

	if(saveDebugFile)
	{
		debugFile.open("d3d9_debug.txt", std::ios::out);
	}

	return D3DProxyDevice::BeginScene();
}

HRESULT WINAPI D3DProxyDeviceEgo::EndScene()
{
	// delay to avoid crashing on start
	static int initDelay = 120;
	initDelay--;

	if(!stereoView->initialized && initDelay < 0)
	{
		stereoView->Init(m_pDevice);
	}

	return D3DProxyDevice::EndScene();
}

HRESULT WINAPI D3DProxyDeviceEgo::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if(stereoView->initialized)
	{
		if(eyeShutter > 0)
		{
			stereoView->UpdateEye(StereoView::LEFT_EYE);
		}
		else 
		{
			stereoView->UpdateEye(StereoView::RIGHT_EYE);
		}

		stereoView->Draw();

		eyeShutter *= -1;
	}

	if(eyeShutter > 0) return D3D_OK;
	HRESULT hr = D3DProxyDevice::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return hr;
}

HRESULT WINAPI D3DProxyDeviceEgo::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	
	if(stereoView->initialized && Vector4fCount >= 4 && validRegister(StartRegister)) // && (fabs(pConstantData[12]) + fabs(pConstantData[13]) + fabs(pConstantData[14]) > 0.001f))
	{
		
		currentMatrix = const_cast<float*>(pConstantData);

		D3DXMATRIX sourceMatrix(currentMatrix);

		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);
		D3DXMATRIX worldViewMatrix;
		D3DXMATRIX worldViewTransMatrix;

		D3DXMatrixMultiply(&worldViewMatrix, &sourceMatrix, &matProjectionInv);

		float spacing = 0.5f;
		D3DXMatrixPerspectiveOffCenterLH(&matProjection, convergence*eyeShutter-spacing, convergence*eyeShutter+spacing, -spacing, spacing*1.0f, 1.0f, 100.0f);
		D3DXMatrixMultiply(&sourceMatrix, &worldViewMatrix, &matProjection);
	
		currentMatrix = (float*)sourceMatrix;

		currentMatrix[0] += separation*eyeShutter+offset;

		/*
		char buf[32];
		LPCSTR psz = NULL;

		//wsprintf(buf, "vp w: %d", separation);
		sprintf_s(buf, "sep: %f", separation);
		psz = buf;
		OutputDebugString(psz);
		OutputDebugString("\n");
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


		return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, currentMatrix, Vector4fCount);
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI D3DProxyDeviceEgo::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return D3DProxyDevice::SetDepthStencilSurface(pNewZStencil);
}

bool D3DProxyDeviceEgo::validRegister(UINT reg)
{
	switch(game_type)
	{
	case EGO:
	case EGO_DIRT:
		if(reg != 12) return true;
		else return false;
	default:
		if(reg == 0) return true;
		else return false;
	}
}

int D3DProxyDeviceEgo::getMatrixIndex()
{
	switch(game_type)
	{
	case EGO:
	case EGO_DIRT:
		return 0;
		break;
	default:
		return 0;
	}
}