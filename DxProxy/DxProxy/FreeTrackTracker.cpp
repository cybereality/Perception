/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <FreeTrackTracker.cpp> and
Class <FreeTrackTracker> :
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

#include "FreeTrackTracker.h"
#include <windows.h>

/**
* Constructor.
* Calls init function.
***/ 
FreeTrackTracker::FreeTrackTracker(void):MotionTracker()
{
	OutputDebugString("FreeTrack Tracker Created\n");
	init();
}

/**
* Destructor.
* Calls FreeTrack FreeLibrary function.
***/
FreeTrackTracker::~FreeTrackTracker(void)
{
	FreeLibrary(hinstLib);
}

/**
* FreeTrack Tracker init.
* Loads FreeTrack library ("FreeTrackClient.dll") and gets FTGetData function pointer.
***/
int FreeTrackTracker::init()
{
	// zero variables for storing data
	lastRoll = 0;
	lastPitch = 0;
	lastYaw = 0;

	// create variable for exchanging data with the DLL
	pData = &data;

	// Load DLL file
	hinstLib = LoadLibrary("FreeTrackClient.dll");

	// Get function pointer
	getData = (importGetData)GetProcAddress(hinstLib, "FTGetData");

	OutputDebugString("FreeTrack Tracker Init\n");

	return 0;
}

/**
* FreeTrack reset.
* Calls init function.
***/
void FreeTrackTracker::reset()
{
	init();
}

/**
* Destroy FreeTrack tracker.
* Calls FreeTrack FreeLibrary function.
***/
void FreeTrackTracker::destroy()
{
	FreeLibrary(hinstLib);
}

/**
* Retrieve FreeTrack tracker orientation.
* Reads device input and returns orientation negated.
***/
int FreeTrackTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
#ifdef _DEBUG
	OutputDebugString("FreeTrack Tracker getOrient\n");
#endif

	if (getData(pData)) {
		lastRoll = data.roll;
		lastPitch = data.pitch;
		lastYaw = data.yaw;
	}

	*roll = -lastRoll;
	*pitch = lastPitch;
	*yaw = -lastYaw;

#ifdef _DEBUG
	OutputDebugString("FreeTrack Tracker updateOrientation\n");
#endif

	return 0;
}

/**
* Is tracker selected and detected?
* Returns wether FreeTrack tracker option is selected. Returns true if FTGetData function pointer present.
***/
bool FreeTrackTracker::isAvailable()
{
	if (getData == NULL) {
		return false;
	} else {
		return true;
	}
}