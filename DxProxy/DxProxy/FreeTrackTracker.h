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

#include "MotionTracker.h"

class FreeTrackTracker : public MotionTracker
{
public:
	FreeTrackTracker(void);
	~FreeTrackTracker(void);
	int getOrientation(float* yaw, float* pitch, float* roll);
	bool isAvailable();
	int init();
	void reset();
	void destroy();
private:
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
	}FreeTrackData;

	typedef bool (WINAPI *importGetData)(FreeTrackData * data);

	HINSTANCE hinstLib;
	importGetData getData;
	FreeTrackData data;
	FreeTrackData *pData;

	float lastRoll;
	float lastPitch;
	float lastYaw;
};