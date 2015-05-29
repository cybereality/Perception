/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <SharedMemoryTracker.cpp> and
Class <SharedMemoryTracker> :
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


#include "SharedMemoryTracker.h"
#include "ProxyHelper.h"

#include <string>
#include <iostream>

/**
* Name of mapping object.
***/
TCHAR szName[]=TEXT("VireioSMTrack");

/**
* Constructor.
* Calls init function.
***/ 
SharedMemoryTracker::SharedMemoryTracker(ProxyConfig *config)
	:MotionTracker(config)
{
	OutputDebugString("Socket Tracker Created\n");
	hMapFile = NULL;
	pTrackBuf = NULL;
	init();
}

/**
* Destructor.
* Unmaps shared memory pointer. Closes file mapping object handle.
***/
SharedMemoryTracker::~SharedMemoryTracker(void)
{
	UnmapViewOfFile(pTrackBuf);
	CloseHandle(hMapFile);
}

/**
* FreeTrack Tracker init.
* Calls openSharedMemory function.
***/
void SharedMemoryTracker::init()
{
	OutputDebugString("Socket Tracker Init\n");
	openSharedMemory();
}

/**
* Retrieve shared memory tracker orientation.
* Reads and returns memory input.
***/
int  SharedMemoryTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z)
{
#ifdef _DEBUG
	OutputDebugString("Socket Tracker getOrient\n");
#endif

	if(pTrackBuf == NULL)
		return MTS_INITFAIL; // error no buffer

	//Initial values should now be in radians to match OVR and VRBoost requirements
	// Then converted to Degrees
	*yaw = -pTrackBuf->Yaw;
	*pitch = pTrackBuf->Pitch;
	*roll = -pTrackBuf->Roll;


	return MTS_OK;
}

/**
* Is tracker selected and detected?
* Returns wether a tracker option is selected. Naturally returns true.
***/
MotionTrackerStatus SharedMemoryTracker::getStatus()
{
	return MTS_OK;
}

/**
*  Open shared memory file mapping object. 
*  Creates a file mapping object ("VireioSMTrack") to be provided with tracking data.
*/
bool SharedMemoryTracker::openSharedMemory()
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,	// use paging file
		NULL,					// default security
		PAGE_READWRITE,			// read/write access
		0,						// maximum object size (high-order DWORD)
		sizeof(TrackData),		// maximum object size (low-order DWORD)
		szName);				// name of mapping object

	if (hMapFile == NULL)										// Could not create file mapping object
		return false;

	pTrackBuf = (TrackData*) MapViewOfFile(hMapFile,			// handle to map object
		FILE_MAP_ALL_ACCESS,									// read/write permission
		0,
		0,
		sizeof(TrackData));

	if (pTrackBuf == NULL)										// Could not map view of file
	{
		CloseHandle(hMapFile);
		return 1;
	}

	return 0;
}