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

#ifndef HMDisplayInfo_OculusRift_H_INCLUDED
#define HMDisplayInfo_OculusRift_H_INCLUDED

/**
* Predefined head mounted display info.
* Now populates using OVR SDK
***/
struct HMDisplayInfo_OculusRift :
	public HMDisplayInfo
{
public:

#pragma warning( push )
#pragma warning( disable : 4351 ) //disable "new behaviour warning for default initialised array" for this constructor

	HMDisplayInfo_OculusRift() :
	   //Just use the constructor of the base class
		HMDisplayInfo(),
		m_status(HMD_STATUS_OK)
	{

 	}

	~HMDisplayInfo_OculusRift()
	{

	}

	virtual std::string GetHMDName() {return "Oculus";}


	/**
	* Screen resolution, in pixels.
	* <horizontal, vertical>
	***/
	virtual std::pair<UINT, UINT> GetResolution()
	{
		return std::make_pair<UINT, UINT>((UINT)1920, (UINT)1080);
	}

	/**
	* Physical characteristics are in meters.
	* <horizontal, vertical> 
	***/
	virtual std::pair<float, float> GetPhysicalScreenSize()
	{
		return std::pair<float,float>(0.12576f, 0.07074f);
	}

	/**
	* Physical lens seperation (currently constant rift dev kit 1 value=default ipd 0.064f).
	***/
	virtual float GetPhysicalLensSeparation()
	{
		static float separation = 0.0635f;
		//Hard-coded for now
		return separation;
	}
	
	/**
	* The distance in a 0 to 1 range that the center of each lens is from the center of each half of
	* the screen on Y axis
	***/
	virtual float GetLensYCenterOffset()
	{
		return 0.5f;
	}

	/**
	* The minimum distortion scale allowed, varies between headsets (-1.0 is fine for DK1, whereas -0.5 is better for RiftUp)
	***/
	virtual float GetMinDistortionScale()
	{
		if (hmd->ProductId == ovrHmd_DK1)
		{
			return -1.0f;
		}

		return -0.5f;
	}

		/**
	 * Returns the manufacturer of the HMD - Enumeration to be added to when new HMD manufacturers are supported
	 */
	virtual HMDManufacturer GetHMDManufacturer() {return HMD_OCULUS;}

	/**
	 * Returns the status of the HMD - If it could be initialised, then error status is set, this method can be overridden
	 * to return more specufuc error info
	 */
	virtual HMDStatus GetStatus() {return m_status;}

	/**
	 * Returns the status of the HMD - If it could be initialised, then error status is set, this method can be overridden
	 * to return more specufuc error info
	 */
	virtual std::string GetStatusString()
	{
		switch (m_status)
		{
			case HMD_STATUS_OK:
				return "OK";
			case HMD_STATUS_ERROR:
				return "No Oculus Configuration Profile Defined!";
		}

		return "";
	}


private:
	/**
	* Oculus head mounted display device.
	***/
    ovrHmd hmd;
    
    HMDStatus m_status;

	float MaxRadius;
};
#endif