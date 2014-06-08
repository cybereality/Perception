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

#ifndef HMDISPLAYINFO_H_INCLUDED
#define HMDISPLAYINFO_H_INCLUDED

#include "d3d9.h"
#include "d3dx9.h"
#include <utility>
#include <sstream>


/**
* Predefined head mounted display info.
* Default constructing with Rift DK1 values.
***/
struct HMDisplayInfo
{
public:

#pragma warning( push )
#pragma warning( disable : 4351 ) //disable "new behaviour warning for default initialised array" for this constructor
	/**
	* All rift values from OVR_Win32_HMDDevice.cpp in LibOVR
	* Default constructing with Rift DK1 values.
	***/
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
		// Range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1  range
		lensXCenterOffset = 4.0f * physicalOffset / physicalScreenSize.first; 

		//0 to 1
		lensYCenterOffset = 0.5f;

		// This scaling will ensure the source image is sampled so that the left edge of the left half of the screen is just reached
		// by the image. -1 is the left edge of the -1 to 1 range and it is adjusted for the lens center offset (note that this needs
		// adjusting if the lens is also offset vertically, See: StereoConfig::updateDistortionOffsetAndScale in LibOVR for an example
		// of how to do this)
		scaleToFillHorizontal = Distort(-1 - lensXCenterOffset) / (-1 - lensXCenterOffset);

		std::stringstream sstm;
		sstm << "scaleToFillHorizontal: " << scaleToFillHorizontal << std::endl;
		OutputDebugString(sstm.str().c_str());
	}

	/**
	* Left lens center as percentage of the physical screen size.
	***/
	float LeftLensCenterAsPercentage()
	{
		return ((physicalScreenSize.first / 2.0f) - (physicalLensSeparation / 2.0f)) / (physicalScreenSize.first);
	}
#pragma warning( pop )
	/**
	* Screen resolution, in pixels.
	* <horizontal, vertical>
	***/
	std::pair<UINT, UINT>  resolution;
	/**
	* Screen aspect ratio, computed using resolution.
	***/
	float screenAspectRatio;
	/**
	* Half screen aspect ratio, computed using resolution (half width) .
	***/
	float halfScreenAspectRatio;
	/**
	* Physical characteristics are in meters.
	* <horizontal, vertical> 
	***/
	std::pair<float, float> physicalScreenSize;
	/**
	* Currently constant eye to screen distance (according to rift dev kit 1)
	***/
	float eyeToScreenDistance;
	/**
	* Physical lens seperation (currently constant rift dev kit 1 value=default ipd 0.064f).
	***/
	float physicalLensSeparation;
	/**
	* The distance in a -1 to 1 range that the center of each lens is from the center of each half of
	* the screen (center of a screen half is 0,0).
	* -1 being the far left edge of the screen half and +1 being the far right of the screen half.
	***/
	float lensXCenterOffset;
	/**
	* The distance in a 0 to 1 range that the center of each lens is from the center of each half of
	* the screen on Y axis
	***/
	float lensYCenterOffset;
	/**
	* From Rift docs on distortion : uvResult = uvInput * (K0 + K1 * uvLength^2 + K2 * uvLength^4).
	***/
	float distortionCoefficients[4];
	/**
	* Scaling value, used to fill shader constants.
	* @see OculusRiftView::CalculateShaderVariables()
	***/
	float scaleToFillHorizontal;
	/**
	* This distortion must match that being used in the shader (the distortion, not including the scaling 
	* that is included in the shader).
	***/
	virtual float Distort(float radius)
	{
		float radiusSqared = radius * radius;
		return radius * (distortionCoefficients[0] + distortionCoefficients[1] * radiusSqared + distortionCoefficients[2] * 
			radiusSqared * radiusSqared + distortionCoefficients[3] * radiusSqared * radiusSqared * radiusSqared);
	}
};
#endif