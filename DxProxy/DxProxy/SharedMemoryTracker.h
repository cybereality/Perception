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
#ifndef SHAREDMEMORY_TRACKER_H_INCLUDED
#define SHAREDMEMORY_TRACKER_H_INCLUDED


#include "MotionTracker.h"

#include <string>

/**
* Shared memory tracking data structure.
***/
struct TrackData
{
	int DataID;	/**< increased every time data has been sent */

	float Yaw;
	float Pitch;
	float Roll;

	float X;
	float Y;
	float Z;
};

/**
* Shared memory tracking class.
* Retrieves data from external tracking sources.
*/
class SharedMemoryTracker : public MotionTracker
{
public:
	SharedMemoryTracker(void);
	~SharedMemoryTracker(void);

	/*** SharedMemoryTracker public methods ***/
	int  init();
	int  getOrientation(float* yaw, float* pitch, float* roll);
	void updateOrientation();
	bool isAvailable();
	
private:
	/*** SharedMemoryTracker private methods ***/
	bool openSharedMemory();

	/**
	* Handle to file mapping object.
	***/
	HANDLE hMapFile;
	/**
	* Pointer to tracking data source.
	***/
	TrackData* pTrackBuf;
};


#endif