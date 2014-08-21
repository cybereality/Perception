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

#include "HMDisplayInfoFactory.h"

#include "HMDisplayInfo.h"
#include "HMDisplayInfo_DK1.h"
#include "HMDisplayInfo_DK2.h"
#include "HMDisplayInfo_RiftUp.h"


HMDisplayInfo* HMDisplayInfoFactory::CreateHMDisplayInfo(StereoView::StereoTypes stereoType)
{
	switch(stereoType)
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
	case StereoView::DIY_RIFT:
	case StereoView::OVER_UNDER:
	case StereoView::OCULUS_RIFT_DK1:
	case StereoView::OCULUS_RIFT_DK1_CROPPED:
		return new HMDisplayInfo_DK1();
		break;
	case StereoView::RIFTUP:
		return new HMDisplayInfo_RiftUp();
		break;
	case StereoView::OCULUS_RIFT_DK2:
	case StereoView::OCULUS_RIFT_DK2_CROPPED:
		return new HMDisplayInfo_DK2();
		break;	
	default:
		return new HMDisplayInfo_DK1();
		break;
	}
}

