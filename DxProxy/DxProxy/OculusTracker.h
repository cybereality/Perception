/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusTracker.h> and
Class <OculusTracker> :
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

#ifndef OCULUSTRACKER_H_INCLUDED
#define OCULUSTRACKER_H_INCLUDED

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / PI))

#include "MotionTracker.h"
#include "..\..\LibOVR\Include\OVR.h"  

using namespace OVR;

/**
* Oculus tracker class.
* Oculus VR(TM) was founded by Palmer Luckey, self-described virtual reality enthusiast and hardware 
* geek. The company launched a Kickstarter campaign to help fund development of their first product,
* the Oculus Rift, a ground-breaking virtual reality headset for immersive gaming. With the support
* of top video game companies including Valve, Epic Games and Unity, the Kickstarter was an enormous
* success, raising over $2.4 million in funding from project backers and supporters around the world.
*
* The team is currently developing the Oculus Rift in an effort to revolutionize the way people 
* experience interactive content.<http://www.oculusvr.com/company/>
*/
class OculusTracker : public MotionTracker
{
public:
	OculusTracker(void);
	virtual ~OculusTracker(void);

	/*** OculusTracker public methods ***/
	int  init();
	void reset();
	int  getOrientation(float* yaw, float* pitch, float* roll);
	void updateOrientation();
	bool isAvailable();	

private:
	/**
	* Oculus device manager.
	***/
    Ptr<DeviceManager> pManager;
	/**
	* Oculus head mounted display device.
	***/
    Ptr<HMDDevice> pHMD;
	/**
	* Oculus sensor device.
	***/
    Ptr<SensorDevice> pSensor;
	/**
	* Oculus sensor fusion.
	* Retrieves tracking data.
	***/
    // TODO: fix this so it is using unique_ptr from the std memory class.  Typedef conflicts with freespace tracker prevent including memory right now...  
	//std::unique_ptr<SensorFusion> SFusion;
	SensorFusion* SFusion;
	/**
	* Oculus orientation quaternion.
	***/
	Quatf hmdOrient;
 };

#endif