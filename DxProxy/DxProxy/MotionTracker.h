/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#ifndef MOTIONTRACKER_H_INCLUDED
#define MOTIONTRACKER_H_INCLUDED

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#include <math.h>
#include <windows.h>

/**
*  Base motion tracker class. 
*  Derive base functionality from this to any tracking class.
*/
class MotionTracker
{
public:
	MotionTracker(void);
	virtual ~MotionTracker(void);

	/*** MotionTracker virtual public methods ***/
	virtual int  init();
	virtual int  getOrientation(float* yaw, float* pitch, float* roll);
	virtual void updateOrientation();
	virtual bool isAvailable();
	virtual void setMultipliers(float yaw, float pitch, float roll);

	/*** MotionTracker public methods ***/
	bool isEqual(float a, float b){ return abs(a-b) < 0.001; };

	/**
	* Orientation, as received from tracker.
	***/
	float yaw, pitch, roll;
	/**
	* Orientation, as primary received from tracker.
	***/
	float primaryYaw, primaryPitch, primaryRoll;
	/**
	* Current yaw angle, in positive degrees, multiplied by yaw multiplier.
	***/
	float currentYaw;
	/**
	* Current pitch angle, in positive degrees, multiplied by pitch multiplier.
	***/
	float currentPitch;
	/**
	* Current roll angle, in radians, multiplied by roll multiplier.
	***/
	float currentRoll;
	/**
	* Yaw difference, to be passed to game mouse input.
	***/
	float deltaYaw;
	/**
	* Pitch difference, to be passed to game mouse input.
	***/
	float deltaPitch;
	/**
	* Game specific yaw angle multiplier.
	***/
	float multiplierYaw;
	/**
	* Game specific pitch angle multiplier.
	***/
	float multiplierPitch;
	/**
	* Game specific roll angle multiplier.
	***/
	float multiplierRoll;
	/**
	* Mouse data, to be passed to the game.
	***/
	INPUT mouseData;

	/**
	* Currently supported tracker types enumeration.
	***/
	static enum TrackerTypes
	{
		DISABLED = 0,         /**< Tracking disabled. */
		HILLCREST = 10,       /**< Hillcrest Labs. Freespace. */
		FREETRACK = 20,       /**< FreeTrack optical motion tracking. */
		SHAREDMEMTRACK = 30,  /**< Shared memory tracking. */
		OCULUSTRACK = 40      /**< Oculus Rift tracking. */
	};
};

#endif