/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusRiftView.cpp> and
Class <OculusRiftView> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

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

#define _WINSOCKAPI_

#include "HMDisplayInfoFactory.h"
#include "StereoView.h"

#include <utility>
#include <sstream>

#include <OVR.h>
//#include <OVR_Stereo.h>
//#include <OVR_Profile.h>
//#include <CAPI/CAPI_HMDState.h>
//#include <Sensors/OVR_DeviceConstants.h>

#include "HMDisplayInfo.h"
#include "HMDisplayInfo_Default.h"
#include "HMDisplayInfo_OculusRift.h"


HMDisplayInfo* HMDisplayInfoFactory::CreateHMDisplayInfo(int /*StereoView::StereoTypes*/ stereoType)
{
	switch((StereoView::StereoTypes)stereoType)
	{
	//Basically, everything uses the "default", which is the DK1 variant
	case StereoView::ANAGLYPH_RED_CYAN:
	case StereoView::ANAGLYPH_RED_CYAN_GRAY:
	case StereoView::ANAGLYPH_YELLOW_BLUE:
	case StereoView::ANAGLYPH_YELLOW_BLUE_GRAY:
	case StereoView::ANAGLYPH_GREEN_MAGENTA:
	case StereoView::ANAGLYPH_GREEN_MAGENTA_GRAY:
	case StereoView::INTERLEAVE_HORZ:
	case StereoView::INTERLEAVE_VERT:
	case StereoView::CHECKERBOARD:
	case StereoView::SIDE_BY_SIDE:
	case StereoView::OVER_UNDER:
	case StereoView::DIY_RIFT:
		return new HMDisplayInfo_Default();
		break;
	case StereoView::OCULUS_RIFT:
		return new HMDisplayInfo_OculusRift();
		break;	
	default:
		return new HMDisplayInfo_Default();
		break;
	}
}

