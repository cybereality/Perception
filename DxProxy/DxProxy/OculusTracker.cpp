/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "OculusTracker.h"

OculusTracker::OculusTracker()
{
	OutputDebugString("Motion Tracker Created\n");
	pManager = NULL;
	pHMD = NULL;
	pSensor = NULL;
	init();
}

OculusTracker::~OculusTracker()
{
	pSensor.Clear();
	pManager.Clear();
	System::Destroy();  // shutdown LibOVR
}

int OculusTracker::init()
{
	OutputDebugString("OculusTracker Init\n");

	System::Init(); // start LibOVR

	pManager = *DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	pSensor = *pHMD->GetSensor();
	
	if (pSensor)
		SFusion.AttachToSensor(pSensor);

	return 0;
}

int OculusTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
	OutputDebugString("OculusTracker getOrient\n");

	if(SFusion.IsAttachedToSensor() == false)
		return 1;												// error no sensor

	// all orintations are in degrees
	hmdOrient = SFusion.GetOrientation();
	hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(yaw, pitch, roll);
	*yaw = -RadToDegree(*yaw);
	*pitch = RadToDegree(*pitch);
	*roll = -RadToDegree(*roll);

	return 0; 
}

bool OculusTracker::isAvailable()
{
	return SFusion.IsAttachedToSensor();
}

void OculusTracker::updateOrientation()
{
	OutputDebugString("OculusTracker updateOrientation\n");

	if(getOrientation(&yaw, &pitch, &roll) == 0)
	{
		yaw = fmodf(yaw + 360.0f, 360.0f);
		pitch = -fmodf(pitch + 360.0f, 360.0f);

		deltaYaw = yaw - currentYaw;
		deltaPitch = pitch - currentPitch;

		// hack to avoid errors while translating over 360/0
		if(fabs(deltaYaw) > 4.0f) deltaYaw = 0.0f;
		if(fabs(deltaPitch) > 4.0f) deltaPitch = 0.0f;

		mouseData.mi.dx = (long)(deltaYaw*multiplierYaw);
		mouseData.mi.dy = (long)(deltaPitch*multiplierPitch);
		
		OutputDebugString("Motion Tracker SendInput\n");
		SendInput(1, &mouseData, sizeof(INPUT));

		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = (float)( roll * (PI/180.0) * multiplierRoll);			// convert from deg to radians then apply mutiplier
	}
}