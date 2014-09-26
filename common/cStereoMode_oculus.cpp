#include <OVR.h>
#include <OVR_Stereo.h>
#include <OVR_Profile.h>
#include <CAPI_HMDState.h>
#include <OVR_DeviceConstants.h>
#include "cStereoMode.h"


void cStereoMode::loadOculus(){

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
		return;
	}

	OVR::ProfileDeviceKey pdk( &state->OurHMDInfo );
	OVR::Profile*         profile = OVR::ProfileManager::GetInstance()->GetProfile( pdk , user );
	OVR::HmdRenderInfo    renderInfo = OVR::GenerateHmdRenderInfoFromHmdInfo( state->OurHMDInfo , profile , OVR::DistortionEqnType::Distortion_CatmullRom10 );

	cStereoMode* mode = new cStereoMode;

	switch (state->OurHMDInfo.HmdType)
	{
	case OVR::HmdType_DK1:
		{
			// Rift dev kit 
			mode->distortionCoefficients[0] = 1.0f;
			mode->distortionCoefficients[1] = 0.22f;
			mode->distortionCoefficients[2] = 0.24f;
			mode->distortionCoefficients[3] = 0.0f;		

			//From Oculus SDK OVR_Stereo.cpp
			mode->chromaCoefficients[0]        = -0.006f;
			mode->chromaCoefficients[1]        =  0.0f;
			mode->chromaCoefficients[2]        =  0.014f;
			mode->chromaCoefficients[3]        =  0.0f;
		}
		break;
	case OVR::HmdType_DKHD2Proto:
		{
			// RiftUp!
			mode->distortionCoefficients[0] = 1.0f;
			mode->distortionCoefficients[1] = 0.18f;
			mode->distortionCoefficients[2] = 0.115f;
			mode->distortionCoefficients[3] = 0.0f;			

			//From Oculus SDK OVR_Stereo.cpp
			mode->chromaCoefficients[0]        = -0.006f;
			mode->chromaCoefficients[1]        =  0.0f;
			mode->chromaCoefficients[2]        =  0.014f;
			mode->chromaCoefficients[3]        =  0.0f;
		}
		break;
	case OVR::HmdType_DK2:
		{
			// Rift dev kit 2 OVR_Stereo.cpp
			mode->distortionCoefficients[0] = 1.0f;
			mode->distortionCoefficients[1] = 0.15f;
			mode->distortionCoefficients[2] = 0.05f;
			mode->distortionCoefficients[3] = 0.0f;

			//From Oculus SDK OVR_Stereo.cpp
			mode->chromaCoefficients[0]        = -0.0112f;
			mode->chromaCoefficients[1]        = -0.015f;
			mode->chromaCoefficients[2]        =  0.0187f;
			mode->chromaCoefficients[3]        =  0.015f;
		}
		break;
	}

	mode->name                          = "Oculus Rift (automatic," + QString(hmd->ProductName) + ")";
	mode->resolutionWidth               = hmd->Resolution.w;
	mode->resolutionHeight              = hmd->Resolution.h;
	mode->physicalWidth                 = state->OurHMDInfo.ScreenSizeInMeters.w;
	mode->physicalHeight                = state->OurHMDInfo.ScreenSizeInMeters.h;
	mode->eyeToScreenDistance           = 0.041f;
	mode->physicalLensSeparation        = renderInfo.LensSeparationInMeters;
	mode->lensYCenterOffset             = 0.5f;
	mode->lensIPDCenterOffset           = 0.0f;
	mode->chromaticAberrationCorrection = true;

	if( hmd->ProductId == ovrHmd_DK1 ){
		mode->minDistortionScale = -1.0f;
	}else{
		mode->minDistortionScale = -0.5f;
	}

	mode->calculateValues();

	ovrHmd_Destroy(hmd);
}
