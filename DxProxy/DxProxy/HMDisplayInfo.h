/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#ifndef HMDISPLAYINFO_H_INCLUDED
#define HMDISPLAYINFO_H_INCLUDED

#include "d3d9.h"
#include "d3dx9.h"
#include <utility>



struct HMDisplayInfo
{
public:

#pragma warning( push )
#pragma warning( disable : 4351 ) //disable "new behaviour warning for default initialised array"

	// All rift values from OVR_Win32_HMDDevice.cpp in LibOVR
	// Default constructing with Rift DK1 values
	HMDisplayInfo() :
		resolution(std::make_pair<UINT, UINT>(1280, 800)), // Rift dev kit 
		screenAspectRatio((float)resolution.first / (float)resolution.second),
		halfScreenAspectRatio(((float)resolution.first * 0.5f) / (float)resolution.second),
		physicalScreenSize(std::make_pair<float, float>(0.14976f, 0.0935f)), // Rift dev kit 
		eyeToScreenDistance(0.041f), // Rift dev kit
		physicaLensSeparation(0.064f), // Rift dev kit 
		distortionCoefficients()
	{
		distortionCoefficients[0] = 1.0f;
		distortionCoefficients[1] = 0.22f;
		distortionCoefficients[2] = 0.24f;
		distortionCoefficients[3] = 0.0f;
	}

#pragma warning( pop )

	// <horizontal, vertical>
	std::pair<UINT, UINT>  resolution;

	float screenAspectRatio;
	float halfScreenAspectRatio;

    // Physical characteristics are in meters
	// <horizontal, vertical> 
	std::pair<float, float> physicalScreenSize;
    float eyeToScreenDistance;
    float physicaLensSeparation;


	// From Rift docs on distortion  
    //   uvResult = uvInput * (K0 + K1 * uvLength^2 + K2 * uvLength^4)
    float distortionCoefficients[4];

};



#endif