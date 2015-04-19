/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MotionTracker.h> and
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

#ifndef MOTIONTRACKER_H_INCLUDED
#define MOTIONTRACKER_H_INCLUDED

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#include <math.h>
#include <windows.h>

enum MotionTrackerStatus
{
	MTS_NOTINIT,
	MTS_INITIALISING,
	MTS_NOHMDDETECTED,
	MTS_INITFAIL,
	MTS_DRIVERFAIL,
	//Any status beyond this point means the HMD can be used (sort of..)
	MTS_OK,
	MTS_NOORIENTATION,
	MTS_CAMERAMALFUNCTION,
	MTS_LOSTPOSITIONAL
};

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
	virtual void init();
	virtual void resetOrientationAndPosition() {}
	virtual void resetPosition() {}
	virtual int  getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z);
	virtual void updateOrientationAndPosition();
	virtual MotionTrackerStatus getStatus();
	virtual void setMultipliers(float yaw, float pitch, float roll);
	virtual bool setMouseEmulation(bool emulateMouse);
	virtual bool getMouseEmulation();
	virtual void BeginFrame() {}
	virtual void WaitTillTime() {}
	virtual void EndFrame() {}
	virtual char* GetTrackerDescription() {return "No Tracker";}
	virtual bool SupportsPositionTracking() {return false;}

	/*** MotionTracker public methods ***/
	bool isEqual(float a, float b){ return abs(a-b) < 0.001; };

	/**
	* Orientation, as received from tracker.
	***/
	float yaw, pitch, roll;
	/**
	* Position, as received from tracker.
	***/
	float x, y, z;
	/**
	* Orientation, as primary received from tracker.
	***/
	float primaryYaw, primaryPitch, primaryRoll;
	/**
	* Positional, as primary received from tracker.
	***/
	float primaryX, primaryY, primaryZ;
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
	* Orientation offset, used to "reset" orientation
	***/
	float offsetYaw, offsetPitch, offsetRoll;
	/**
	* Positional offset, used to "reset" position
	***/
	float offsetX, offsetY, offsetZ;

	/**
	 * Whether we are using Oculus Timewarp or not (currently appears to introduce awful juddering)
	 * Need a better place to put this, probably shouldn't be on the base class
	 */
	bool useSDKPosePrediction;

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

protected:
	/**
	* True if mouse emulation is on.
	***/
	bool mouseEmulation;
};

#endif