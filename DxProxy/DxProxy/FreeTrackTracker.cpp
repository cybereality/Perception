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
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

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
	importGetData		getData;
	importGetDllVersion getDllVersion;
	importReportID		reportID;
	importProvider		provider;

	// create variables for exchanging data with the DLL
	FreeTrackData data;
	FreeTrackData *pData;
	pData = &data;
	int name = 453;

	// Load DLL file
	HINSTANCE hinstLib = LoadLibrary("FreeTrackClient.dll");
	if (hinstLib == NULL) {
		OutputDebugString("[FREETRACK] ERROR: unable to load DLL\n");
		return 1;
	} else {
		OutputDebugString("[FREETRACK] DLL loaded\n");
	}

	// Get function pointers
	getData = (importGetData)GetProcAddress(hinstLib, "FTGetData");
	getDllVersion = (importGetDllVersion)GetProcAddress(hinstLib, "FTGetDllVersion");
	reportID = (importReportID)GetProcAddress(hinstLib, "FTReportName");
	provider = (importProvider)GetProcAddress(hinstLib, "FTProvider");

	// Check they are valid
	if (getData == NULL) {
		OutputDebugString("[FREETRACK] ERROR: unable to find 'FTGetData' function\n");
		FreeLibrary(hinstLib);
		return 1;
	}
	if (getDllVersion == NULL){
		OutputDebugString("[FREETRACK] ERROR: unable to find 'FTGetDllVersion' function\n");
		FreeLibrary(hinstLib);
		return 1;
	}
	if (reportID == NULL){
		OutputDebugString("[FREETRACK] ERROR: unable to find 'FTReportID' function\n");
		FreeLibrary(hinstLib);
		return 1;
	}
	if (reportID == NULL){
		OutputDebugString("[FREETRACK] ERROR: unable to find 'FTProvider' function\n");
		FreeLibrary(hinstLib);
		return 1;
	}

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
	if (getData(pData))
		{
			*roll = data.roll	* 100;
			*pitch = data.pitch	* 100;
			*yaw = data.yaw		* 100;
		}
	return 0;
}

bool FreeTrackTracker::isAvailable()
{
	return reportID >= 0;
}