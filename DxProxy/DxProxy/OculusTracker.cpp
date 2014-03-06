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

/**
* Constructor.
* Calls init function.
***/ 
OculusTracker::OculusTracker()
{
	init();
}

/**
* Destructor.
* Clears and destroys Oculus system.
***/
OculusTracker::~OculusTracker()
{
	pSensor.Clear();
	pManager.Clear();
	if (SFusion)
		delete SFusion;
	System::Destroy(); // shutdown LibOVR 
}

/**
* Oculus init.
* Calls libOVR System::Init, gets device manager, hmd-device and sensor.
***/
int OculusTracker::init()
{
	OutputDebugString("OculusTracker Start");
	System::Init(); // start LibOVR
	OutputDebugString("OculusTracker Initialized");
	pManager = *DeviceManager::Create();
	OutputDebugString("PManager Created");
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	OutputDebugString("Created Device");
	if (!pHMD)
	{
		OutputDebugString("No OculusTracker found");
		return -1;
	}
	OutputDebugString("Starting PSensor");
	pSensor = *pHMD->GetSensor();
	if (SFusion)
	{
		OutputDebugString("Deleting Sensor Fusion");
		SFusion = NULL;
	}
	OutputDebugString("Starting Sensor Fusion");
	SFusion = new SensorFusion();
	if (pSensor)
		SFusion->AttachToSensor(pSensor);

	OutputDebugString("oculus tracker initted");
	return 0;
}

/**
* Oculus reset.
* Resets the orientation.
***/
void OculusTracker::reset()
{
	if (pSensor)
		SFusion->Reset();
}

/**
* Retrieve Oculus tracker orientation.
* Reads device input and returns orientation (yaw and roll negated). All Orientations are in degrees.
* Roll gets converted back to radians in updateOrientation.
***/
int OculusTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
#ifdef _DEBUG
	OutputDebugString("OculusTracker getOrient\n");
#endif

	if(SFusion->IsAttachedToSensor() == false)
		return 1;						// error no sensor

	// all orientations are in degrees
	hmdOrient = SFusion->GetOrientation();
	hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(yaw, pitch, roll);

	// set primary orientations
	primaryYaw = *yaw;
	primaryPitch = *pitch;
	primaryRoll = *roll;
	*yaw = -RadToDegree(*yaw);
	*pitch = RadToDegree(*pitch);
	*roll = -RadToDegree(*roll);

	return 0; 
}

/**
* Update Oculus tracker orientation.
* Updates tracker orientation and passes it to game mouse input accordingly.
***/
void OculusTracker::updateOrientation()
{
#ifdef _DEBUG
	OutputDebugString("OculusTracker updateOrientation\n");
#endif

	// Get orientation from Oculus tracker.
	if(getOrientation(&yaw, &pitch, &roll) == 0)
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
	}
}

/**
* Is tracker selected and detected?
* Returns libOVR SensorFusion::IsAttachedToSensor function result.
***/
bool OculusTracker::isAvailable()
{
	return SFusion->IsAttachedToSensor();
}