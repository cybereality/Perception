/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <StereoViewFactory.h> and
Class <StereoViewFactory> :
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

#ifndef STEREOVIEWFACTORY_H_INCLUDED
#define STEREOVIEWFACTORY_H_INCLUDED

#include "ProxyHelper.h"
#include "StereoView.h"

/**
*  Stereo view factory class. 
*  Class to retrieve the currently selected stereo view.
*/
class StereoViewFactory
{
public:
	/*** StereoViewFactory public methods ***/
	static StereoView* Get(ProxyHelper::ProxyConfig& config, HMDisplayInfo hmd);
};
#endif