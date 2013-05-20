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

#ifndef OCULUSTRACKER_H_INCLUDED
#define OCULUSTRACKER_H_INCLUDED

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#include "MotionTracker.h"
#include "..\..\LibOVR\Include\OVR.h"  //how is this found?

using namespace OVR;

class OculusTracker : public MotionTracker
{
public:
	OculusTracker(void);
	virtual ~OculusTracker(void);

	int getOrientation(float* yaw, float* pitch, float* roll);
	bool isAvailable();
	void updateOrientation();
	int init();

private:
    Ptr<DeviceManager> pManager;
    Ptr<HMDDevice>     pHMD;
    Ptr<SensorDevice>  pSensor;

    SensorFusion       SFusion;

	Quatf hmdOrient;
 };


#endif