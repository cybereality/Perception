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

#include "OculusTracker.h"
#include "HMDisplayInfoFactory.h"

/**
* Constructor.
* Calls init function.
***/ 
OculusTracker::OculusTracker()
{
	initialised = false;
	init();
}

/**
* Destructor.
* Clears and destroys Oculus system.
***/
OculusTracker::~OculusTracker()
{
	ovrHmd_Destroy(pHMD);
	ovr_Shutdown();
}

/**
* Oculus init.
* Calls libOVR System::Init, gets device manager, hmd-device and sensor.
***/
int OculusTracker::init()
{
	OutputDebugString("OculusTracker Start");
	ovrBool res = ovr_Initialize(); // start LibOVR

	if (res)
		OutputDebugString("OculusTracker Initialized");
	 
	int detected = ovrHmd_Detect();
	if (detected == 0)
	{
		OutputDebugString("No HMD detected, use a dummy DK1");
		pHMD=ovrHmd_CreateDebug(ovrHmd_DK1);
	}
	else
	{
		pHMD=ovrHmd_Create(0);
	}


	unsigned int trackingCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
	ovrBool success = ovrHmd_ConfigureTracking(pHMD, trackingCaps, 0);

	if (!success)
	{
		OutputDebugString("oculus tracker failed to initialise");
		return -1;
	}

	OutputDebugString("oculus tracker initted");
	initialised = true;
	
	return 0;
}

/**
* Oculus reset.
* Resets the orientation.
***/
void OculusTracker::reset()
{
}

void OculusTracker::BeginFrame()
{
	//return;
	if (initialised){
		FrameRef=ovrHmd_BeginFrameTiming(pHMD,0);
	}
}

void OculusTracker::WaitTillTime()
{
	if (initialised){
		ovr_WaitTillTime(FrameRef.TimewarpPointSeconds);
	}
}

void OculusTracker::EndFrame()
{
	if (initialised){
			ovrHmd_EndFrameTiming(pHMD);
	}
}

/**
* Retrieve Oculus tracker orientation.
* Reads device input and returns orientation (yaw and roll negated). All Orientations are in degrees.
* Roll gets converted back to radians in updateOrientation.
***/
int OculusTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z) 
{
#ifdef _DEBUG
	OutputDebugString("OculusTracker getOrient\n");
#endif

	ovrTrackingState ts = ovrHmd_GetTrackingState(pHMD,FrameRef.ScanoutMidpointSeconds);
	
	Quatf hmdOrient=ts.HeadPose.ThePose.Orientation;
	hmdOrient.GetEulerAngles<Axis_Y,Axis_X,Axis_Z>(yaw, pitch, roll);
	

	// set primary orientations
	primaryYaw = *yaw;
	primaryPitch = *pitch;
	primaryRoll = *roll;
	*yaw = -RadToDegree(*yaw);
	*pitch = RadToDegree(*pitch);
	*roll = -RadToDegree(*roll);

	if (ts.StatusFlags & ovrStatus_PositionTracked)
	{
#ifdef _DEBUG
	OutputDebugString("OculusTracker getPosition\n");
#endif
		*x = ts.HeadPose.ThePose.Position.x;
		*y = ts.HeadPose.ThePose.Position.y;
		*z = ts.HeadPose.ThePose.Position.z;
		primaryX = *x;
		primaryY = *y;
		primaryZ = *z;
	}

	return 0; 
}

/**
* Update Oculus tracker orientation.
* Updates tracker orientation and passes it to game mouse input accordingly.
***/
void OculusTracker::updateOrientationAndPosition()
{
#ifdef _DEBUG
	OutputDebugString("OculusTracker updateOrientation\n");
#endif

	// Get orientation from Oculus tracker.
	if(getOrientationAndPosition(&yaw, &pitch, &roll, &x, &y, &z) == 0)
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

#ifdef _DEBUG
		OutputDebugString("Motion Tracker SendInput\n");
#endif
		// Send to mouse input.
		if (mouseEmulation)
			SendInput(1, &mouseData, sizeof(INPUT));

		// Set current data.
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = (float)( roll * (PI/180.0) * multiplierRoll);	// convert from deg to radians then apply mutiplier

		currentX = x;
		currentY = y;
		currentZ = z;
	}
}

/**
* Is tracker selected and detected?
* Returns libOVR SensorFusion::IsAttachedToSensor function result.
***/
bool OculusTracker::isAvailable()
{
	return initialised;
}