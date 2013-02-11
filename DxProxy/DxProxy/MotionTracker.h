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

#ifndef MOTIONTRACKER_H_INCLUDED
#define MOTIONTRACKER_H_INCLUDED

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#include <math.h>
#include <windows.h>

class MotionTracker
{
public:
	MotionTracker(void);
	virtual ~MotionTracker(void);

	virtual int init();
	virtual int getOrientation(float* yaw, float* pitch, float* roll);
	virtual void updateOrientation();
	virtual bool isAvailable();
	virtual void setMultipliers(float yaw, float pitch, float roll);

	bool isEqual(float a, float b){ return abs(a-b) < 0.001; };

	float yaw, pitch, roll;
	float currentYaw;
	float currentPitch;
	float currentRoll;
	float deltaYaw;
	float deltaPitch;

	float multiplierYaw;
	float multiplierPitch;
	float multiplierRoll;
	INPUT mouseData;

	static enum TrackerTypes
	{
		DISABLED = 0,
		HILLCREST = 10,
		FREETRACK = 20
		SOCKETRACK = 30
	};
};

#endif