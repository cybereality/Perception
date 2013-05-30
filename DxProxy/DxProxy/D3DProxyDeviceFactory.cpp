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

#include "D3DProxyDeviceFactory.h"
#include "D3DProxyDeviceSource.h"
#include "D3DProxyDeviceUnreal.h"
#include "D3DProxyDeviceTest.h"
#include "D3DProxyDeviceEgo.h"
#include "D3DProxyDeviceAdv.h"
#include "DataGatherer.h"

D3DProxyDevice* D3DProxyDeviceFactory::Get(ProxyHelper::ProxyConfig& config, IDirect3DDevice9* dev, BaseDirect3D9* pCreatedBy)
{
	D3DProxyDevice* newDev = NULL;

	switch(config.game_type)
	{
	case D3DProxyDevice::DATA_GATHERER:
		newDev = new DataGatherer(dev, pCreatedBy);
		break;
	case D3DProxyDevice::SOURCE:
	case D3DProxyDevice::SOURCE_L4D:
		newDev = new D3DProxyDeviceSource(dev, pCreatedBy);
		break;
	case D3DProxyDevice::UNREAL:
	case D3DProxyDevice::UNREAL_MIRROR:
	case D3DProxyDevice::UNREAL_UT3:
		newDev = new D3DProxyDeviceUnreal(dev, pCreatedBy);
		break;
	case D3DProxyDevice::EGO:
	case D3DProxyDevice::EGO_DIRT:
		newDev = new D3DProxyDeviceEgo(dev, pCreatedBy);
		break;
	case D3DProxyDevice::REALV:
	case D3DProxyDevice::REALV_ARMA:
		newDev = new D3DProxyDeviceTest(dev, pCreatedBy);
		break;
	case D3DProxyDevice::UNITY:
	case D3DProxyDevice::UNITY_SLENDER:
		newDev = new D3DProxyDeviceTest(dev, pCreatedBy);
		break;
	case D3DProxyDevice::ADVANCED:
	case D3DProxyDevice::ADVANCED_SKYRIM:
		newDev = new D3DProxyDeviceAdv(dev, pCreatedBy);
		break;
	case D3DProxyDevice::MONO:
	case D3DProxyDevice::UNREAL_BIOSHOCK:
	case D3DProxyDevice::FIXED:
	default:
		newDev = new D3DProxyDevice(dev, pCreatedBy);
		break;
	}

	newDev->Init(config);

	return newDev;
}