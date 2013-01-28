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

#include "MotionTracker.h"

MotionTracker::MotionTracker()
{
	OutputDebugString("Motion Tracker Created");
	init();
}

MotionTracker::~MotionTracker()
{
}

int MotionTracker::init()
{
	OutputDebugString("Motion Tracker Init");

	currentYaw = 0.0f;
	currentPitch = 0.0f;
	currentRoll = 0.0f;

	deltaYaw = 0.0f;
	deltaPitch = 0.0f;

	multiplierYaw = 25.0f;
	multiplierPitch = 25.0f;
	multiplierRoll = 1.0f;

	mouseData.type = INPUT_MOUSE;
	mouseData.mi.dx = 0;
	mouseData.mi.dy = 0;
	mouseData.mi.mouseData = 0;
	mouseData.mi.dwFlags = MOUSEEVENTF_MOVE;
	mouseData.mi.time = 0;
	mouseData.mi.dwExtraInfo = 0;

	return 0;
}

int MotionTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
	//OutputDebugString("Motion Tracker getOrient");
	return -1;
}

bool MotionTracker::isAvailable()
{
	return false;
}

void MotionTracker::updateOrientation()
{
	//OutputDebugString("Motion Tracker updateOrientation");
	if(getOrientation(&yaw, &pitch, &roll) == 0)
	{
		//OutputDebugString("Motion Tracker getOrientation == 0");
		if(!isEqual(currentYaw, 0.0f) && !isEqual(currentPitch, 0.0f))
		{
			yaw = fmodf(RADIANS_TO_DEGREES(yaw) + 360.0f, 360.0f)*multiplierYaw;
			pitch = -fmodf(RADIANS_TO_DEGREES(pitch) + 360.0f, 360.0f)*multiplierPitch;

			deltaYaw = yaw - currentYaw;
			deltaPitch = pitch - currentPitch;

			if(fabs(deltaYaw) > 100.0f) deltaYaw = 0.0f;
			if(fabs(deltaPitch) > 100.0f) deltaPitch = 0.0f;
			
			mouseData.mi.dx = (long)(deltaYaw);
			mouseData.mi.dy = (long)(deltaPitch);
		
			//OutputDebugString("Motion Tracker SendInput");
			SendInput(1, &mouseData, sizeof(INPUT));
		}

		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = roll*multiplierRoll;
	}
}

void MotionTracker::setMultipliers(float yaw, float pitch, float roll)
{
	multiplierYaw = yaw;
	multiplierPitch = pitch;
	multiplierRoll = roll;
	currentYaw = 0.0f;
	currentPitch = 0.0f;
	currentRoll = 0.0f;
}