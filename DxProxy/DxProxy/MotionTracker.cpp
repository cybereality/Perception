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
#include "D3DProxyDevice.h"
#include "Vireio.h"
using namespace vireio;

/**
* Constructor.
* Calls init function.
***/ 
MotionTracker::MotionTracker(ProxyConfig *config)
	: useSDKPosePrediction(true)
{
	this->config = config;
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
void MotionTracker::init()
{
	OutputDebugString("Motion Tracker Init\n");

	currentYaw = 0.0f;
	currentPitch = 0.0f;
	currentRoll = 0.0f;
	primaryYaw = 0.0f;
	primaryPitch = 0.0f;
	primaryRoll = 0.0f;

	deltaYaw = 0.0f;
	deltaPitch = 0.0f;

	offsetYaw = 0.0f;
	offsetPitch = 0.0f;
	offsetRoll = 0.0f;
	offsetX = 0.0f;
	offsetY = 0.0f;
	offsetZ = 0.0f;
	primaryX = 0;
	primaryY = 0;
	primaryZ = 0;
}

/**
* Tracker specific orientation/position update.
* returns error (MTS_NOTINIT) in base class.
***/
int MotionTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z)
{
	return MTS_NOTINIT;
}


/**
* Update tracker orientation and position (if available).
* Updates tracker orientation and position (if available) and passes it to game mouse input accordingly.
***/
void MotionTracker::updateOrientationAndPosition()
{
	// Get orientation from derived tracker.
	if(getOrientationAndPosition(&yaw, &pitch, &roll, &x, &y, &z) >= MTS_OK)
	{
		// Skip empty input data.
		if(AlmostSame(currentYaw, 0.0f, .001f) && AlmostSame(currentPitch, 0.0f, .001f))
			return;
		
	
		// Convert yaw, pitch to positive degrees, multiply by multiplier.
		// (-180.0f...0.0f -> 180.0f....360.0f)
		// (0.0f...180.0f -> 0.0f...180.0f)
		yaw = fmodf(RADIANS_TO_DEGREES(yaw) + 360.0f, 360.0f)*config->yaw_multiplier;
		pitch = -fmodf(RADIANS_TO_DEGREES(pitch) + 360.0f, 360.0f)*config->pitch_multiplier;

		// Get difference.
		deltaYaw   += yaw   - currentYaw;
		deltaPitch += pitch - currentPitch;

		// Set limits.
		if(fabs(deltaYaw) > 100.0f) deltaYaw = 0.0f;
		if(fabs(deltaPitch) > 100.0f) deltaPitch = 0.0f;

		// Send to mouse input.
		InjectMouseMotion((long)deltaYaw, (long)deltaPitch);

		// Keep fractional difference in the delta so it's added to the next update.
		deltaYaw -= (float)(long)deltaYaw;
		deltaPitch -= (float)(long)deltaPitch;

		// Set current data.
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = roll*config->roll_multiplier;
	}
}

/**
* Is tracker selected and detected?
* Returns wether a tracker option is selected. Naturally returns false in base class.
***/
MotionTrackerStatus MotionTracker::getStatus()
{
	//Default tracker is just fine thanks
	return MTS_OK;
}

/**
* Set mouse emulation.
* @param emulateMouse True if mouse emulation is on.
***/
bool MotionTracker::setMouseEmulation(bool emulateMouse)
{
	bool temp = mouseEmulation;
	mouseEmulation = emulateMouse;
	return temp;
}

/**
* Get mouse emulation.
***/
bool MotionTracker::getMouseEmulation()
{
	return mouseEmulation;
}

void MotionTracker::InjectMouseMotion(long deltaYaw, long deltaPitch)
{
	INPUT mouseData;
	
	mouseData.type = INPUT_MOUSE;
	mouseData.mi.mouseData = 0;
	mouseData.mi.dwFlags = MOUSEEVENTF_MOVE;
	mouseData.mi.time = 0;
	mouseData.mi.dwExtraInfo = 0;
	
	mouseData.mi.dx = (long)deltaYaw;
	mouseData.mi.dy = (long)deltaPitch;

	// Send to mouse input.
	if (mouseEmulation)
		SendInput(1, &mouseData, sizeof(INPUT));
}
