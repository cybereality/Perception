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


#include "SharedMemoryTracker.h"

#include <string>
#include <iostream>

TCHAR szName[]=TEXT("VireioSMTrack");

SharedMemoryTracker::SharedMemoryTracker(void):MotionTracker()
{
	OutputDebugString("Socket Tracker Created\n");
	hMapFile = NULL;
	pTrackBuf = NULL;
	init();
}

SharedMemoryTracker::~SharedMemoryTracker(void)
{
	UnmapViewOfFile(pTrackBuf);
	CloseHandle(hMapFile);
}

int SharedMemoryTracker::init()
{
	OutputDebugString("Socket Tracker Init\n");
	openSharedMemory();
	return 0;
}


int SharedMemoryTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
	OutputDebugString("Socket Tracker getOrient\n");

	if(pTrackBuf == NULL)
		return 1;												// error no buffer

	// all orintations are in degrees
	*yaw = pTrackBuf->Yaw;
	*pitch = pTrackBuf->Pitch;
	*roll = pTrackBuf->Roll;

	return 0; 
}

bool SharedMemoryTracker::isAvailable()
{
	return true;
}

void SharedMemoryTracker::updateOrientation()
{
	OutputDebugString("Motion Tracker updateOrientation\n");

	if(getOrientation(&yaw, &pitch, &roll) == 0)
	{
		yaw = fmodf(yaw + 360.0f, 360.0f);
		pitch = -fmodf(pitch + 360.0f, 360.0f);

		deltaYaw = yaw - currentYaw;
		deltaPitch = pitch - currentPitch;

		// hack to avoid errors while translating over 360/0
		if(fabs(deltaYaw) > 4.0f) deltaYaw = 0.0f;
		if(fabs(deltaPitch) > 4.0f) deltaPitch = 0.0f;

		mouseData.mi.dx = (long)(deltaYaw*multiplierYaw);
		mouseData.mi.dy = (long)(deltaPitch*multiplierPitch);
		
		OutputDebugString("Motion Tracker SendInput\n");
		SendInput(1, &mouseData, sizeof(INPUT));

		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = (float)( roll * (PI/180.0) * multiplierRoll);			// convert from deg to radians then apply mutiplier
	}
}

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