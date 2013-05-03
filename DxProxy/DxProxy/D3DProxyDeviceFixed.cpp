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

#include "D3DProxyDeviceFixed.h"

D3DProxyDeviceFixed::D3DProxyDeviceFixed(IDirect3DDevice9* pDevice):D3DProxyDevice(pDevice)
{
}

D3DProxyDeviceFixed::~D3DProxyDeviceFixed()
{
}



/*HRESULT WINAPI D3DProxyDeviceFixed::BeginScene()
{
	HandleControls();
//	HandleTracking();
	ComputeViewTranslation();

	return D3DProxyDevice::BeginScene();
}*/





/*HRESULT WINAPI D3DProxyDeviceFixed::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	if(State == D3DTS_VIEW)
	{
		D3DXMATRIX sourceMatrix(*pMatrix);

		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);

		if(trackerInitialized && tracker->isAvailable())
		{
			D3DXMATRIX rollMatrix;
			D3DXMatrixRotationZ(&rollMatrix, tracker->currentRoll);
			D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &rollMatrix);
		}

		D3DXMatrixTranslation(&transMatrix, (separation*eyeShutter+offset)*6000.0f, 0, 0);
		D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &transMatrix);

		return D3DProxyDevice::SetTransform(State, &sourceMatrix);
	}
	else if(State == D3DTS_PROJECTION)
	{
		D3DXMATRIX sourceMatrix(*pMatrix);

		D3DXMATRIX transMatrix;
		D3DXMatrixIdentity(&transMatrix);

		D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &matProjectionInv);

		transMatrix[8] += convergence*eyeShutter*0.0075f;
		D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &transMatrix);

		D3DXMatrixMultiply(&sourceMatrix, &sourceMatrix, &matProjection);

		return D3DProxyDevice::SetTransform(State, &sourceMatrix);
	}

	return D3DProxyDevice::SetTransform(State, pMatrix);
}*/