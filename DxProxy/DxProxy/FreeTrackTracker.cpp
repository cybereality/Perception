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

#include "FreeTrackTracker.h"
#include <windows.h>

FreeTrackTracker::FreeTrackTracker(void):MotionTracker()
{
	OutputDebugString("[FREETRACK] FreeTrack Tracker Created\n");
	init();
}

FreeTrackTracker::~FreeTrackTracker(void)
{
	FreeLibrary(hinstLib);
}

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

	return 0;
}

void FreeTrackTracker::reset()
{
	init();
}

void FreeTrackTracker::destroy()
{
	FreeLibrary(hinstLib);
}

int FreeTrackTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
	if (getData(pData)) {
		lastRoll = data.roll;
		lastPitch = data.pitch;
		lastYaw = data.yaw;
	}

	*roll = -lastRoll;
	*pitch = lastPitch;
	*yaw = -lastYaw;
	return 0;
}

bool FreeTrackTracker::isAvailable()
{
	if (getData == NULL) {
		return false;
	} else {
		return true;
	}
}