/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MotionTracker.cpp> and
Class <MotionTracker> :
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

#include "MotionTracker.h"

/**
* Constructor.
* Calls init function.
***/ 
MotionTracker::MotionTracker()
{
	OutputDebugString("Motion Tracker Created\n");
	init();
}

/**
* Empty destructor.
***/
MotionTracker::~MotionTracker()
{
}

/**
* Motion Tracker init.
* Resets tracker data.
***/
int MotionTracker::init()
{
	OutputDebugString("Motion Tracker Init\n");

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

/**
* Tracker specific orientation update.
* returns -1 in base class.
***/
int MotionTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
#ifdef _DEBUG
	OutputDebugString("Motion Tracker getOrient\n");
#endif
	return -1;
}

/**
* Update tracker orientation.
* Updates tracker orientation and passes it to game mouse input accordingly.
***/
void MotionTracker::updateOrientation()
{
#ifdef _DEBUG
	OutputDebugString("Motion Tracker updateOrientation\n");
#endif
	// Get orientation from derived tracker.
	if(getOrientation(&yaw, &pitch, &roll) == 0)
	{
#ifdef _DEBUG
		OutputDebugString("Motion Tracker getOrientation == 0\n");
#endif
		// Skip empty input data.
		if(!isEqual(currentYaw, 0.0f) && !isEqual(currentPitch, 0.0f))
		{
			// Convert yaw, pitch to positive degrees, multiply by multiplier.
			// (-180.0f...0.0f -> 180.0f....360.0f)
			// (0.0f...180.0f -> 0.0f...180.0f)
			yaw = fmodf(RADIANS_TO_DEGREES(yaw) + 360.0f, 360.0f)*multiplierYaw;
			pitch = -fmodf(RADIANS_TO_DEGREES(pitch) + 360.0f, 360.0f)*multiplierPitch;

			// Get difference.
			deltaYaw += yaw - currentYaw;
			deltaPitch += pitch - currentPitch;

			// Set limits.
			if(fabs(deltaYaw) > 100.0f) deltaYaw = 0.0f;
			if(fabs(deltaPitch) > 100.0f) deltaPitch = 0.0f;

			// Pass to mouse data (long integer).
			mouseData.mi.dx = (long)(deltaYaw);
			mouseData.mi.dy = (long)(deltaPitch);

			// Keep fractional difference in the delta so it's added to the next update.
			deltaYaw -= (float)mouseData.mi.dx;
			deltaPitch -= (float)mouseData.mi.dy;

#ifdef _DEBUG
			//OutputDebugString("Motion Tracker SendInput\n");
#endif
			// Send to mouse input.
			if (mouseEmulation)
				SendInput(1, &mouseData, sizeof(INPUT));
		}

		// Set current data.
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = roll*multiplierRoll;
	}
}

/**
* Is tracker selected and detected?
* Returns wether a tracker option is selected. Naturally returns false in base class.
***/
bool MotionTracker::isAvailable()
{
	return false;
}

/**
* Set game-specific angle multipliers.
* @param yaw Yaw Degree multiplier.
* @param pitch Pitch Degree multiplier.
* @param roll Roll Radian multiplier.
***/
void MotionTracker::setMultipliers(float yaw, float pitch, float roll)
{
	multiplierYaw = yaw;
	multiplierPitch = pitch;
	multiplierRoll = roll;
	currentYaw = 0.0f;
	currentPitch = 0.0f;
	currentRoll = 0.0f;
}

/**
* Set mouse emulation.
* @param emulateMouse True if mouse emulation is on.
***/
void MotionTracker::setMouseEmulation(bool emulateMouse)
{
	mouseEmulation = emulateMouse;
}