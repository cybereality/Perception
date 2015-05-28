/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <FreeSpaceTracker.h> and
Class <FreeSpaceTracker> :
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

#include "freespace\freespace.h"
#include "MotionTracker.h"

/**
* Freespace tracker class.
* Today, Freespace is widely known as the gold standard for motion control in consumer electronics. 
* Freespace has been featured in successful motion products around the globe released by some of the 
* biggest names in consumer electronics. <http://hillcrestlabs.com/industries/>
*/
class FreeSpaceTracker : public MotionTracker
{
public:
	FreeSpaceTracker(ProxyConfig *config);
	~FreeSpaceTracker(void);

	/*** FreeSpaceTracker public methods ***/
	void init();
	void reset();
	void destroy();
	int  getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z);
	MotionTrackerStatus getStatus();
	virtual char* GetTrackerDescription() {return "FreeSpaceTracker";}

private:
	/*** FreeSpaceTracker public methods ***/
	int  connect(void);
	void close(void);

	/**
	* Freespace device Identifier.
	***/
	FreespaceDeviceId DeviceID;
	/**
	* Freespace device Info.
	***/
	FreespaceDeviceInfo Device;
};