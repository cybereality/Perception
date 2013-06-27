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

#include "D3DProxyDeviceLFS.h"
#include "StereoViewFactory.h"

D3DProxyDeviceLFS::D3DProxyDeviceLFS(IDirect3DDevice9* pDevice):D3DProxyDevice(pDevice)
{
	OutputDebugString("D3D ProxyDev LFS Created\n");
}

D3DProxyDeviceLFS::~D3DProxyDeviceLFS()
{
}

void D3DProxyDeviceLFS::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev LFS Init\n");
	D3DProxyDevice::Init(cfg);
	roll_mode = 1;
}

HRESULT WINAPI D3DProxyDeviceLFS::BeginScene()
{
	HandleControls();
	HandleTracking();
	ComputeViewTranslation();

	return D3DProxyDevice::BeginScene();
}

HRESULT WINAPI D3DProxyDeviceLFS::EndScene()
{
	// delay to avoid crashing on start
	static int initDelay = 120;
	initDelay--;

	if(!stereoView->initialized && initDelay < 0)
	{
		stereoView->Init(m_pDevice);
		SetupMatrices();
	}

	return D3DProxyDevice::EndScene();
}

HRESULT WINAPI D3DProxyDeviceLFS::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if(stereoView->initialized && stereoView->stereoEnabled)
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

	return D3DProxyDevice::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT WINAPI D3DProxyDeviceLFS::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	if(stereoView->initialized && Vector4fCount >= 4 && StartRegister == 0)
	{
		currentMatrix = const_cast<float*>(pConstantData);

		D3DXMATRIX sourceMatrix(currentMatrix);

		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);
			
		sourceMatrix = sourceMatrix * matViewTranslation; 

		D3DXMatrixTranspose(&sourceMatrix, &sourceMatrix);

		currentMatrix = (float*)sourceMatrix;

		return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, currentMatrix, Vector4fCount);
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT WINAPI D3DProxyDeviceLFS::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	if(State == D3DTS_PROJECTION)
	{
		D3DXMATRIX sourceMatrix(*pMatrix);

		sourceMatrix = sourceMatrix * matViewTranslation; 

		return D3DProxyDevice::SetTransform(State, &sourceMatrix);
	}

	return D3DProxyDevice::SetTransform(State, pMatrix);
}

bool D3DProxyDeviceLFS::validRegister(UINT reg)
{
	switch(game_type)
	{
	case SOURCE_L4D:
		if(reg == 4 || reg == 8 || reg == 51) 
			return true;
		else return false;
		break;
	default:
		return true;
	}
}

int D3DProxyDeviceLFS::getMatrixIndex()
{
	switch(game_type)
	{
	case SOURCE_L4D:
		return 0;
		break;
	default:
		return 0;
	}
}