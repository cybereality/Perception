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

/*
This file contains selections of code copied from freetrack_c_interface.cpp,
an example program distributed with FreeTrack v2.2 which was originally written
by Alastair Moore in December 2007. It bears the GNU GPL Version 2 license and
should be free to include in this project.
*/

/*
This file depends on the precence of FreeTrackClient.dll in the same folder
as the executable, but need not be compiled against or linked to it. Again,
FreeTrackClient.dll is GPL licensed, and therefore free to redistribute.
*/

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
		lastRoll = data.roll * 10;
		lastPitch = data.pitch * 10;
		lastYaw = data.yaw * 10;
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