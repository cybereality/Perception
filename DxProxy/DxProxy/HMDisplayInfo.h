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

#include <string>

/**
* HMDisplayInfo abstract base class
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
	HMDisplayInfo()
	{
		//Still using poly4 distortion implementaion until SDK rendering in place
		enum { NumCoefficients = 4 };
		distortionCoefficients = new float[NumCoefficients];
		ZeroMemory(distortionCoefficients, NumCoefficients * sizeof(float));
		ZeroMemory(chromaCoefficients, 4 * sizeof(float));
	}

	virtual ~HMDisplayInfo() { delete []distortionCoefficients; }

	virtual std::string GetHMDName() = 0;

#pragma warning( pop )
	/**
	* Screen resolution, in pixels.
	* <horizontal, vertical>
	***/
	virtual std::pair<UINT, UINT>  GetResolution() = 0;

	/**
	* Screen aspect ratio, computed using resolution.
	***/
	virtual float GetScreenAspectRatio()
	{
		std::pair<UINT, UINT>resolution = GetResolution();
		return (float)resolution.first / (float)resolution.second;
	}

	/**
	* Half screen aspect ratio, computed using resolution (half width) .
	***/
	virtual float GetHalfScreenAspectRatio()
	{
		std::pair<UINT, UINT>resolution = GetResolution();
		return ((float)resolution.first * 0.5f) / (float)resolution.second;
	}

	/**
	* Physical characteristics are in meters.
	* <horizontal, vertical> 
	***/
	virtual std::pair<float, float> GetPhysicalScreenSize() = 0;

	/**
	* Currently constant eye to screen distance (according to rift dev kit 1)
	***/
	virtual float GetEyeToScreenDistance() = 0;

	/**
	* Physical lens seperation (currently constant rift dev kit 1 value=default ipd 0.064f).
	***/
	virtual float GetPhysicalLensSeparation() = 0;


	/**
	* The distance in a -1 to 1 range that the center of each lens is from the center of each half of
	* the screen (center of a screen half is 0,0).
	* -1 being the far left edge of the screen half and +1 being the far right of the screen half.
	***/
	virtual float GetLensXCenterOffset()
	{
		std::pair<float, float>physicalScreenSize = GetPhysicalScreenSize();
		float physicalViewCenter = physicalScreenSize.first * 0.25f; 
		float physicalOffset = physicalViewCenter - GetPhysicalLensSeparation() * 0.5f;	
		// Range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1  range
		return 4.0f * physicalOffset / physicalScreenSize.first;
	}

	/**
	* The distance in a 0 to 1 range that the center of each lens is from the center of each half of
	* the screen on Y axis
	***/
	virtual float GetLensYCenterOffset() = 0;

	/**
	* The distance in a -1 to 1 range that offsets the center of each lens is from the center of each half of
	* the screen on X axis
	***/
	virtual float GetLensIPDCenterOffset() = 0;

	/**
	* The minimum distortion scale allowed, varies between headsets (-1.0 is fine for DK1, whereas -0.5 is better for RiftUp)
	***/
	virtual float GetMinDistortionScale() = 0;

	/**
	* See OVR SDK for distortion types (Vireio uses RecipPoly4)
	***/
	virtual float* GetDistortionCoefficients()
	{
		return distortionCoefficients;
	}

	/**
	* Chromatic Abberation Coefficients
	***/
	virtual float* GetDistortionCoefficientsChroma()
	{
		return chromaCoefficients;
	}

	/**
	* Scaling value, used to fill shader constants.
	* @see OculusRiftView::CalculateShaderVariables()
	***/
	virtual float GetScaleToFillHorizontal()
	{
		return Distort(-1 - GetLensXCenterOffset()) / (-1 - GetLensXCenterOffset());
	}

	/**
	* This distortion must match that being used in the shader (the distortion, not including the scaling 
	* that is included in the shader).
	***/
	virtual float Distort(float radius)
	{
		float radiusSqared = radius * radius;
		return radius * (GetDistortionCoefficients()[0] + GetDistortionCoefficients()[1] * radiusSqared + GetDistortionCoefficients()[2] * 
			radiusSqared * radiusSqared + GetDistortionCoefficients()[3] * radiusSqared * radiusSqared * radiusSqared);
	}

protected:
	float *distortionCoefficients;
	float chromaCoefficients[4];
};
#endif