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
#pragma warning( disable : 4351 ) //disable "new behaviour warning for default initialised array" for this constructor

	// All rift values from OVR_Win32_HMDDevice.cpp in LibOVR
	// Default constructing with Rift DK1 values
	HMDisplayInfo() :
		resolution(std::make_pair<UINT, UINT>(1280, 800)), // Rift dev kit 
		screenAspectRatio((float)resolution.first / (float)resolution.second),
		halfScreenAspectRatio(((float)resolution.first * 0.5f) / (float)resolution.second),
		physicalScreenSize(std::make_pair<float, float>(0.14976f, 0.0935f)), // Rift dev kit 
		eyeToScreenDistance(0.041f), // Rift dev kit
		physicalLensSeparation(0.064f), // Rift dev kit 
		distortionCoefficients()
	{
		 // Rift dev kit 
		distortionCoefficients[0] = 1.0f;
		distortionCoefficients[1] = 0.22f;
		distortionCoefficients[2] = 0.24f;
		distortionCoefficients[3] = 0.0f;

		float physicalViewCenter = physicalScreenSize.first * 0.25f; 
		float physicalOffset = physicalViewCenter - physicalLensSeparation * 0.5f;	
		// Normalised range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1
		lensXCenterOffset = 4.0f * physicalOffset / physicalScreenSize.first; 

		// This scaling will ensure the source image is sampled so that the left edge of the left half of the screen is just reached
		// by the image. -1 is the left edge of the -1 to 1 range and it is adjusted for the lens center offset (note that this needs
		// adjusting if the lens is also offset vertically. See StereoConfig::updateDistortionOffsetAndScale in LibOVR for an example
		// of how to do this)
		scaleToFillHorizontal = Distort(-1 - lensXCenterOffset);
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
    float physicalLensSeparation;

	// The distance in a -1 to 1 range that the center of each lens is from the center of each half of the screen.
	// -1 being the far left edge of the screen half and +1 being the far right of the screen half.
	float lensXCenterOffset;

	// From Rift docs on distortion  
    //   uvResult = uvInput * (K0 + K1 * uvLength^2 + K2 * uvLength^4)
    float distortionCoefficients[4];

	float scaleToFillHorizontal;

	// This distortion must match that being used in the shader (the distortion, not including the scaling that is included in the shader)
	virtual float Distort(float radius)
	{
        float radiusSqared = radius * radius;
        return radius * (distortionCoefficients[0] + distortionCoefficients[1] * radiusSqared + distortionCoefficients[2] * 
						radiusSqared * radiusSqared + distortionCoefficients[3] * radiusSqared * radiusSqared * radiusSqared);
	}
};



#endif