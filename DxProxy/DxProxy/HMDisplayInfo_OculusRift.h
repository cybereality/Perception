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

	/**
	* All rift values from OVR_Win32_HMDDevice.cpp in LibOVR
	* Default constructing with Rift DK1 values.
	***/
	HMDisplayInfo_OculusRift() :
	   //Just use the constructor of the base class
		HMDisplayInfo()
	{
		OutputDebugString("HMDisplayInfo_OculusRift()\n");
		ovr_Initialize();

		int detected = ovrHmd_Detect();
		if (detected == 0)
		{
			OutputDebugString("No HMD Detected - Creating Debug DK2\n");
			hmd=ovrHmd_CreateDebug(ovrHmd_DK2);
		}
		else
		{
			hmd=ovrHmd_Create(0);
			if (!hmd)
			{
				OutputDebugString("Unable to create HMD of correct type - Creating Debug DK2\n");
				hmd=ovrHmd_CreateDebug(ovrHmd_DK2);
			}
		}
		std::stringstream sstm;

		//Get some details from OVR SDK
		OVR::CAPI::HMDState *pHMDState = (OVR::CAPI::HMDState*)(hmd->Handle);
		std::string user = OVR::ProfileManager::GetInstance()->GetUser(0);
		OVR::ProfileDeviceKey pdk(&(pHMDState->OurHMDInfo));
		OVR::Profile* profile = OVR::ProfileManager::GetInstance()->GetProfile(pdk, user.c_str());
		sstm << "Using Oculus Profile: " << std::endl;
		sstm << OVR_KEY_USER << ": " << profile->GetValue(OVR_KEY_USER)  << std::endl;
		sstm << "HmdType: " << (int)pHMDState->OurHMDInfo.HmdType  << std::endl;

		switch (pHMDState->OurHMDInfo.HmdType)
		{
		case OVR::HmdType_DK1:
			{
				// Rift dev kit 
				distortionCoefficients[0] = 1.0f;
				distortionCoefficients[1] = 0.22f;
				distortionCoefficients[2] = 0.24f;
				distortionCoefficients[3] = 0.0f;		

				//From Oculus SDK OVR_Stereo.cpp
				chromaCoefficients[0]        = -0.006f;
				chromaCoefficients[1]        =  0.0f;
				chromaCoefficients[2]        =  0.014f;
				chromaCoefficients[3]        =  0.0f;
			}
			break;
		case OVR::HmdType_DKHD2Proto:
			{
				// RiftUp!
				distortionCoefficients[0] = 1.0f;
				distortionCoefficients[1] = 0.18f;
				distortionCoefficients[2] = 0.115f;
				distortionCoefficients[3] = 0.0f;			

				//From Oculus SDK OVR_Stereo.cpp
				chromaCoefficients[0]        = -0.006f;
				chromaCoefficients[1]        =  0.0f;
				chromaCoefficients[2]        =  0.014f;
				chromaCoefficients[3]        =  0.0f;
			}
			break;
		case OVR::HmdType_DK2:
			{
				// Rift dev kit 2 OVR_Stereo.cpp
				distortionCoefficients[0] = 1.0f;
				distortionCoefficients[1] = 0.15f;
				distortionCoefficients[2] = 0.05f;
				distortionCoefficients[3] = 0.0f;

				//From Oculus SDK OVR_Stereo.cpp
				chromaCoefficients[0]        = -0.0112f;
				chromaCoefficients[1]        = -0.015f;
				chromaCoefficients[2]        =  0.0187f;
				chromaCoefficients[3]        =  0.015f;
			}
			break;
		}

		//std::stringstream sstm;
		sstm << "scaleToFillHorizontal: " << GetScaleToFillHorizontal() << std::endl;
		OutputDebugString(sstm.str().c_str()); 
	}

	~HMDisplayInfo_OculusRift()
	{
		ovrHmd_Destroy(hmd);
	}

	virtual std::string GetHMDName() {return std::string(hmd->ProductName);}


	/**
	* Screen resolution, in pixels.
	* <horizontal, vertical>
	***/
	virtual std::pair<UINT, UINT>  GetResolution()
	{
		return std::make_pair<UINT, UINT>((UINT)hmd->Resolution.w, (UINT)hmd->Resolution.h);
	}

	/**
	* Physical characteristics are in meters.
	* <horizontal, vertical> 
	***/
	virtual std::pair<float, float> GetPhysicalScreenSize()
	{
		static std::pair<float, float> physicalScreenSize;
		
		OVR::CAPI::HMDState *pHMDState = (OVR::CAPI::HMDState*)(hmd->Handle);
		physicalScreenSize.first = pHMDState->OurHMDInfo.ScreenSizeInMeters.w;
		physicalScreenSize.second = pHMDState->OurHMDInfo.ScreenSizeInMeters.h;
		return physicalScreenSize;
	}

	/**
	* Currently constant eye to screen distance (according to rift dev kit 1)
	***/
	virtual float GetEyeToScreenDistance()
	{
		return 0.041f;
	}

	/**
	* Physical lens seperation (currently constant rift dev kit 1 value=default ipd 0.064f).
	***/
	virtual float GetPhysicalLensSeparation()
	{
		OVR::CAPI::HMDState *pHMDState = (OVR::CAPI::HMDState*)(hmd->Handle);
		std::string user = OVR::ProfileManager::GetInstance()->GetUser(0);
		OVR::ProfileDeviceKey pdk(&(pHMDState->OurHMDInfo));
		OVR::Profile* profile = OVR::ProfileManager::GetInstance()->GetProfile(pdk, user.c_str());
		OVR::HmdRenderInfo renderInfo = OVR::GenerateHmdRenderInfoFromHmdInfo(pHMDState->OurHMDInfo, profile, OVR::DistortionEqnType::Distortion_CatmullRom10);
		return renderInfo.LensSeparationInMeters;
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
	* The distance in a -1 to 1 range that offsets the center of each lens is from the center of each half of
	* the screen on X axis
	***/
	virtual float GetLensIPDCenterOffset()
	{
		return 0.0f;
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



private:
	/**
	* Oculus head mounted display device.
	***/
    ovrHmd hmd;

	float MaxRadius;
};
#endif