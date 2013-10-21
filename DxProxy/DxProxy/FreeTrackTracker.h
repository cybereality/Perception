/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <FreeTrackTracker.h> and
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

#include "MotionTracker.h"

/**
* Freetrack tracker class.
* FreeTrack is a free optical motion tracking application for Microsoft Windows, released under the
* GNU General Public License. Its main function is inexpensive head tracking in computer games and 
* simulations but can also be used for general computer accessibility, in particular hands-free 
* computing. Tracking is sensitive enough that only small head movements are required so that the 
* user's eyes never leave the screen. <http://www.free-track.net/english/>
***/
class FreeTrackTracker : public MotionTracker
{
public:
	FreeTrackTracker(void);
	~FreeTrackTracker(void);

	/*** FreeTrackTracker public methods ***/
	int  init();
	void reset();
	void destroy();
	int  getOrientation(float* yaw, float* pitch, float* roll);
	bool isAvailable();
	
private:
	/**
	* FreeTrack tracking data structure.
	***/
	typedef struct
	{
		unsigned long int dataID;
		long int camWidth;
		long int camHeight;
		float yaw;
		float pitch;
		float roll;
		float x;
		float y;
		float z;
		float rawyaw;
		float rawpitch;
		float rawroll;
		float rawx;
		float rawy;
		float rawz;
		float x1;
		float y1;
		float x2;
		float y2;
		float x3;
		float y3;
		float x4;
		float y4;
	} FreeTrackData;
	/**
	* FreeTrack FTGetData function type definition.
	***/
	typedef bool (WINAPI *importGetData)(FreeTrackData * data);
	/**
	* FreeTrack library instance handle.
	***/
	HINSTANCE hinstLib;
	/**
	* FreeTrack FTGetData function.
	***/
	importGetData getData;
	/**
	* FreeTrack tracking data.
	***/
	FreeTrackData data;
	/**
	* FreeTrack tracking data pointer.
	***/
	FreeTrackData *pData;
	/**
	* FreeTrack yaw angle retrieved.
	* Returned negated.
	***/
	float lastYaw;
	/**
	* FreeTrack pitch angle retrieved.
	* Returned negated.
	***/
	float lastPitch;
	/**
	* FreeTrack roll angle retrieved.
	* Returned negated.
	***/
	float lastRoll;
};