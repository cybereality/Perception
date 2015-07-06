/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusTracker.cpp> and
Class <OculusTracker> :
Copyright (C) 2012 Andres Hernandez

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
#define _WINSOCKAPI_
#include "OculusTracker.h"
#include "HMDisplayInfoFactory.h"
#include "D3DProxyDevice.h"
#include <string>

#include "..\..\LibOVR\include\OVR.h"


using namespace vireio;

/**
* Constructor.
* Calls init function.
***/ 
OculusTracker::OculusTracker()
{
	status = MTS_NOTINIT;
	init();
}

/**
* Destructor.
* Clears and destroys Oculus system.
***/
OculusTracker::~OculusTracker()
{
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
}

/**
* Oculus init.
* Calls libOVR System::Init, gets device manager, hmd-device and sensor.
***/
void OculusTracker::init()
{
	OutputDebugString("OculusTracker Start");
	status = MTS_INITIALISING;
	ovrResult res = ovr_Initialize(NULL); // start LibOVR

	if (OVR_SUCCESS(res))
	{
		OutputDebugString("OculusTracker Initialize");
	}
	else
	{
		OutputDebugString("OculusTracker Initialize call failed");
		status = MTS_DRIVERFAIL;
		return;
	}
	 
	res = ovrHmd_Detect();
	if (!OVR_SUCCESS(res))
	{
		OutputDebugString("No HMD detected, use a dummy DK1");
		ovrHmd_CreateDebug(ovrHmd_DK1, &hmd);
		status = MTS_NOHMDDETECTED;
		strcpy_s(trackerDescription, "No HMD Detected");
		return;
	}
	else
	{
		ovrHmd_Create(0, &hmd);
		strcpy_s(trackerDescription, (std::string(hmd->ProductName) + "   Serial: " + hmd->SerialNumber).c_str());
		if (hmd->Type == ovrHmd_DK2)
			supportsPositional = true;
	}

	ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	unsigned int trackingCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
	res = ovrHmd_ConfigureTracking(hmd, trackingCaps, 0);

	if (!OVR_SUCCESS(res))
	{
		OutputDebugString("oculus tracker failed to initialise tracking");
		status = MTS_INITFAIL;
	}

	OutputDebugString("oculus tracker initted");
	if (status == MTS_INITIALISING)
		status = MTS_OK;

#ifdef SHOW_CALLS
	debugf("init: %i", (int)status);
#endif
}

char* OculusTracker::GetTrackerDescription()
{
	return trackerDescription;
}

bool OculusTracker::SupportsPositionTracking()
{
	return supportsPositional;
}

/**
* Oculus reset.
* Resets the orientation - As per Oculus VR, only yaw/x/y/z should be reset
***/
void OculusTracker::resetOrientationAndPosition()
{
	SHOW_CALL("OculusTracker resetOrientationAndPosition\n");

	//Force OVR positional reset
	ovrHmd_RecenterPose(hmd);
}

/**
* Oculus reset.
* Resets the positional values only - As per Oculus VR, only x/y/z should be reset
***/
void OculusTracker::resetPosition()
{
	SHOW_CALL("OculusTracker resetOrientationAndPosition\n");

	offsetX = 0.0f;
	offsetY = 0.0f;
	offsetZ = 0.0f;

	//Force OVR positional reset
	ovrHmd_RecenterPose(hmd);
}

/**
* Retrieve Oculus tracker orientation.
* Reads device input and returns orientation (yaw and roll negated). All Orientations are in degrees.
* Roll gets converted back to radians in updateOrientation.
***/
int OculusTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z) 
{
	SHOW_CALL("OculusTracker getOrientationAndPosition\n");

	//Use next frame's display mid point, better than "now" was I think
	ovrFrameTiming  ftiming  = ovrHmd_GetFrameTiming(hmd, 0);
	ts = ovrHmd_GetTrackingState(hmd, ftiming.DisplayMidpointSeconds);

	if (ts.StatusFlags & ovrStatus_OrientationTracked)
	{
		Quatf hmdOrient=ts.HeadPose.ThePose.Orientation;
		hmdOrient.GetEulerAngles<Axis_Y,Axis_X,Axis_Z>(yaw, pitch, roll);

		// set primary orientations
		primaryYaw = *yaw - offsetYaw;
		//As per oculus vr, roll and pitch should not be reset, only yaw/x/y/z
		primaryPitch = *pitch;
		primaryRoll = *roll;
		*yaw = -RadToDegree(*yaw - offsetYaw);
		*pitch = RadToDegree(*pitch);
		*roll = -RadToDegree(*roll);
		status = MTS_OK;
	}
	else
		status = MTS_NOORIENTATION;

	if (ts.StatusFlags & ovrStatus_PositionConnected && status == MTS_OK)
	{
		if (!(ts.StatusFlags & ovrStatus_CameraPoseTracked))
		{
			//Camera still initialising/calibrating
			//Should probably warn user if this doesn't get set after a period of time
			static DWORD tick = GetTickCount();
			if (((tick - GetTickCount()) / 1000) > 15)
				status = MTS_CAMERAMALFUNCTION;
		}
		else if (ts.StatusFlags & ovrStatus_PositionTracked)
		{
			*x = ts.HeadPose.ThePose.Position.x - offsetX;
			*y = ts.HeadPose.ThePose.Position.y - offsetY;
			*z = ts.HeadPose.ThePose.Position.z - offsetZ;
			primaryX = *x;
			primaryY = *y;
			primaryZ = *z;
			status = MTS_OK;
		}
		else
			status = MTS_LOSTPOSITIONAL;
	}

	return (int)status; 
}

/**
* Update Oculus tracker orientation.
* Updates tracker orientation and passes it to game mouse input accordingly.
***/
void OculusTracker::updateOrientationAndPosition()
{
	SHOW_CALL("OculusTracker updateOrientation\n");

	// Get orientation from Oculus tracker.
	if (getOrientationAndPosition(&yaw, &pitch, &roll, &x, &y, &z) >= MTS_OK)
	{
		// Convert yaw, pitch to positive degrees
		// (-180.0f...0.0f -> 180.0f....360.0f)
		// (0.0f...180.0f -> 0.0f...180.0f)
		yaw = fmodf(yaw + 360.0f, 360.0f);
		pitch = -fmodf(pitch + 360.0f, 360.0f);

		// Get difference.
		deltaYaw += yaw - currentYaw;
		deltaPitch += pitch - currentPitch;

		// hack to avoid errors while translating over 360/0
		if(fabs(deltaYaw) > 4.0f) deltaYaw = 0.0f;
		if(fabs(deltaPitch) > 4.0f) deltaPitch = 0.0f;

		// Pass to mouse data (long integer).
		mouseData.mi.dx = (long)(deltaYaw*multiplierYaw);
		mouseData.mi.dy = (long)(deltaPitch*multiplierPitch);

		// Keep fractional difference in the delta so it's added to the next update.
		deltaYaw -= ((float)mouseData.mi.dx)/multiplierYaw;
		deltaPitch -= ((float)mouseData.mi.dy)/multiplierPitch;

		// Send to mouse input.
		if (mouseEmulation)
			SendInput(1, &mouseData, sizeof(INPUT));

		// Set current data.
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = (float)( roll * (PI/180.0) * multiplierRoll);	// convert from deg to radians then apply mutiplier
	}
}

/**
* Returns oculus tracker status as Motion Tracker Status enum
***/
MotionTrackerStatus OculusTracker::getStatus()
{
	return status;
}