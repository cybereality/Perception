/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDeviceFactory.cpp> and
Class <D3DProxyDeviceFactory> :
Copyright (C) 2012 Andres Hernandez

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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
#include "D3DProxyDeviceDebug.h"
#include "D3DProxyDeviceEgo.h"
#include "D3DProxyDeviceAdv.h"
#include "DataGatherer.h"

/**
* Provides proxy device, for new games use Data-Gatherer to catch Shader-Data (game_type==11).
* Creates the Direct3D proxy device class pointer. For a new Game, set "game_type" to 11 in 
* "profiles.cfg" to retrieve shader constant data.
* @param config The game configuration.
* @param pDevice The actual device pointer for the proxy constructor.
* @param pCreatedBy Pointer to the object that created the device.
***/
D3DProxyDevice* D3DProxyDeviceFactory::Get(ProxyHelper::ProxyConfig& config, IDirect3DDevice9* dev, BaseDirect3D9* pCreatedBy)
{
	D3DProxyDevice* newDev = NULL;

	// if game profile = game type + 10000 -> return DataGatherer (=shader analyzer)
	if (config.game_type == D3DProxyDevice::GAMEBRYO)
		newDev = new D3DProxyDeviceAdv(dev, pCreatedBy);
	else if (config.game_type == D3DProxyDevice::DEBUG_LOG_FILE)
		newDev = new D3DProxyDeviceDebug(dev, pCreatedBy);
	else if (config.game_type > 10000)
		newDev = new DataGatherer(dev, pCreatedBy);
	else 
		newDev = new D3DProxyDevice(dev, pCreatedBy);

	newDev->Init(config);

	return newDev;
}