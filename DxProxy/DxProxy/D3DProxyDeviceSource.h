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

#ifndef D3DPROXYDEVICESOURCE_H_INCLUDED
#define D3DPROXYDEVICESOURCE_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include <d3dx9.h>

class D3DProxyDeviceSource : public D3DProxyDevice
{
public:
	D3DProxyDeviceSource(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~D3DProxyDeviceSource();
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);

	void Init(ProxyHelper::ProxyConfig& cfg);
	bool validRegister(UINT reg);
	int getMatrixIndex();
};

#endif