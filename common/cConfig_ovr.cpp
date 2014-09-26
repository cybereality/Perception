#include <OVR.h>
#include <OVR_Stereo.h>
#include <OVR_Profile.h>
#include <CAPI_HMDState.h>
#include <OVR_DeviceConstants.h>
#include "cConfig.h"


bool cConfig::loadOculusSdk(){

	ovrHmd hmd;

	ovr_Initialize();

	int detected = ovrHmd_Detect();
	
	if( detected == 0 ){
		hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
	}else{
		hmd=ovrHmd_Create(0);
		if( !hmd ){
			hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
		}
	}

	OVR::CAPI::HMDState*  state = (OVR::CAPI::HMDState*)(hmd->Handle);
	const char*           user =  OVR::ProfileManager::GetInstance()->GetUser(0);

	if( !user ){
		return false;
	}

	OVR::ProfileDeviceKey pdk( &state->OurHMDInfo );
	OVR::Profile*         profile = OVR::ProfileManager::GetInstance()->GetProfile( pdk , user );
	OVR::HmdRenderInfo    renderInfo = OVR::GenerateHmdRenderInfoFromHmdInfo( state->OurHMDInfo , profile , OVR::DistortionEqnType::Distortion_CatmullRom10 );

	switch (state->OurHMDInfo.HmdType)
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

	resolutionWidth               = hmd->Resolution.w;
	resolutionHeight              = hmd->Resolution.h;
	physicalWidth                 = state->OurHMDInfo.ScreenSizeInMeters.w;
	physicalHeight                = state->OurHMDInfo.ScreenSizeInMeters.h;
	eyeToScreenDistance           = 0.041f;
	physicalLensSeparation        = renderInfo.LensSeparationInMeters;
	lensYCenterOffset             = 0.5f;
	lensIPDCenterOffset           = 0.0f;
	chromaticAberrationCorrection = true;

	if( hmd->ProductId == ovrHmd_DK1 ){
		minDistortionScale = -1.0f;
	}else{
		minDistortionScale = -0.5f;
	}

	ovrHmd_Destroy(hmd);

	return true;
}
