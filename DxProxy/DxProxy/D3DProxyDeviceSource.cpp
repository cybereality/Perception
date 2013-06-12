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

#include "D3DProxyDeviceSource.h"
#include "StereoViewFactory.h"
#include <algorithm>

D3DProxyDeviceSource::D3DProxyDeviceSource(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy)
{
	OutputDebugString("D3D ProxyDev Source Created\n");
}

D3DProxyDeviceSource::~D3DProxyDeviceSource()
{
}

void D3DProxyDeviceSource::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Source Init\n");
	D3DProxyDevice::Init(cfg);
}


