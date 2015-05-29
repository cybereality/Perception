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

#include <math.h>
#include <windows.h>
#include "VireioUtil.h"

struct ProxyConfig;

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
	MotionTracker(ProxyConfig *config);
	virtual ~MotionTracker();

	/*** MotionTracker virtual public methods ***/
	virtual void init();
	virtual void resetOrientationAndPosition() {}
	virtual void resetPosition() {}
	
	/// Get the orientation and position of the head tracker. If this tracker
	/// only reports orientation, the position is (0,0,0). Yaw, pitch, and
	/// roll are all in radians.
	virtual int  getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z);
	
	virtual void updateOrientationAndPosition();
	virtual MotionTrackerStatus getStatus();
	virtual bool setMouseEmulation(bool emulateMouse);
	virtual bool getMouseEmulation();
	virtual void BeginFrame() {}
	virtual void WaitTillTime() {}
	virtual void EndFrame() {}
	virtual char* GetTrackerDescription() {return "No Tracker";}
	virtual bool SupportsPositionTracking() {return false;}
	
	float getYawDeg() { return adjustedYaw; }
	float getPitchDeg() { return primaryPitch; }
	float getRollDeg() { return RADIANS_TO_DEGREES(primaryRoll); }
	
	float getYawRad() { return DEGREES_TO_RADIANS(adjustedYaw); }
	float getPitchRad() { return DEGREES_TO_RADIANS(primaryPitch); }
	float getRollRad() { return primaryRoll; }
	float getX() { return adjustedX; }
	float getY() { return adjustedY; }
	float getZ() { return adjustedZ; }

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
	
private:
	/**
	* Orientation, as last received from tracker, with no centering offset applied.
	* Pitch and yaw in degrees, roll in radians.
	***/
	float primaryYaw, primaryPitch, primaryRoll;
	
	/**
	* Yaw angle, in positive degrees, with centering offset applied.
	* In degrees.
	***/
	float adjustedYaw;
	
	/**
	* Position, as last received from the tracker, with centering offset applied.
	*/
	float adjustedX, adjustedY, adjustedZ;
	
protected:
	/**
	* Current roll angle, in radians, multiplied by roll multiplier. In radians.
	***/
	float currentRoll;
	
	/**
	* Pitch and yaw difference, to be passed to game mouse input. In between
	* calls to updateOrientationAndPosition, this is the pitch and yaw change
	* which could not be sent to the game because it corresponded to a mouse
	* movement smaller than one pixel. In degrees.
	***/
	float deltaYaw, deltaPitch;

	/**
	* Position and orientation offsets, used to "reset" orientation.
	***/
	float offsetYaw, offsetPitch, offsetRoll;
	float offsetX, offsetY, offsetZ;


protected:
	void InjectMouseMotion(long deltaYaw, long deltaPitch);
	
	/**
	* True if mouse emulation is on.
	***/
	bool mouseEmulation;
	
	ProxyConfig *config;
};

#endif