/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <HMDisplayInfo.h> and
Structure <HMDisplayInfo> :
Copyright (C) 2013 Chris Drain

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

#ifndef HMDISPLAYINFO_DK1_H_INCLUDED
#define HMDISPLAYINFO_DK1_H_INCLUDED

#include "d3d9.h"
#include "d3dx9.h"
#include <utility>
#include <sstream>

#include "HMDisplayInfo.h"


/**
* Predefined head mounted display info.
* Default constructing with Rift DK1 values.
***/
struct HMDisplayInfo_Default :
	public HMDisplayInfo
{
public:

#pragma warning( push )
#pragma warning( disable : 4351 ) //disable "new behaviour warning for default initialised array" for this constructor

	/**
	* All rift values from OVR_Win32_HMDDevice.cpp in LibOVR
	* Default constructing with Rift DK1 values.
	***/
	HMDisplayInfo_Default() :
	   //Just use the constructor of the base class
		HMDisplayInfo()
	{
		// Rift dev kit 
		distortionCoefficients[0] = 1.0f;
		distortionCoefficients[1] = 0.22f;
		distortionCoefficients[2] = 0.24f;
		distortionCoefficients[3] = 0.0f;

		chromaCoefficients[0] = 0.010f;
		chromaCoefficients[1] = 0.002f;
		chromaCoefficients[2] = -0.002f;
		chromaCoefficients[3] = -0.005f;

		std::stringstream sstm;
		sstm << "scaleToFillHorizontal: " << GetScaleToFillHorizontal() << std::endl;
		OutputDebugString(sstm.str().c_str());
	}

	virtual std::string GetHMDName() {return "Default";}

	/**
	* Physical lens seperation (currently constant rift dev kit 1 value=default ipd 0.064f).
	***/
	virtual float GetPhysicalLensSeparation()
	{
		return 0.064f;
	}

	/**
	* The minimum distortion scale allowed, varies between headsets (-1.0 is fine for DK1, whereas -0.5 is better for RiftUp)
	***/
	virtual float GetMinDistortionScale()
	{
		return -1.0;
	}

	/**
	 * Returns the manufacturer of the HMD - Enumeration to be added to when new HMD manufacturers are supported
	 */
	virtual HMDManufacturer GetHMDManufacturer() {return HMD_DIY;}

};
#endif