/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <FreeSpaceTracker.cpp> and
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

#include "FreeSpaceTracker.h"
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

/**
* Constructor.
* Calls init function.
***/ 
FreeSpaceTracker::FreeSpaceTracker(void):MotionTracker()
{
	OutputDebugString("Free Tracker Created\n");
	init();
}

/**
* Destructor.
* Calls close function.
***/
FreeSpaceTracker::~FreeSpaceTracker(void)
{
	close();
}

/**
* Freespace Tracker init.
* Calls connect function.
***/
int FreeSpaceTracker::init()
{
	OutputDebugString("Free Tracker Init\n");
	DeviceID = -1;
	int err = connect();
	if (err)
		close();   // Shutdown on error

	return err;
}

/**
* Connect to freespace tracker.
* Initializes the freespace library, looks for a single tracker and attaches it.
* Calls freespace functions to make the tracker ready to use.
***/
int FreeSpaceTracker::connect() 
{
	OutputDebugString("Free Tracker Connect\n");
	// Initialize the freespace library
	int err = freespace_init();
	if (err)
		return err;

	// Look for a single tracker
	int num_devices;
	err = freespace_getDeviceList(&DeviceID, 1, &num_devices);
	if (err)
		return err;

	if (num_devices == 0)
		return FREESPACE_ERROR_NO_DEVICE;

	// Attach to the tracker
	err = freespace_openDevice(DeviceID);
	if (err) 
	{
		DeviceID = -1;
		return err;
	}

	err = freespace_getDeviceInfo(DeviceID, &Device);

	// Flush the message stream
	err = freespace_flush(DeviceID);
	if (err)
		return err;

	// Turn on the orientation message stream
	freespace_message msg;
	memset(&msg, 0, sizeof(msg));

	if (Device.hVer >= 2) 
	{
		// version 2 protocol
		msg.messageType = FREESPACE_MESSAGE_DATAMODECONTROLV2REQUEST;
		msg.dataModeControlV2Request.packetSelect = 3;    // User Frame (orientation)
		msg.dataModeControlV2Request.modeAndStatus = 0;   // operating mode == full motion
	}
	else 
	{
		// version 1 protocol
		msg.messageType = FREESPACE_MESSAGE_DATAMODEREQUEST;
		msg.dataModeRequest.enableUserPosition = 1;
		msg.dataModeRequest.inhibitPowerManager = 1;
	}

	err = freespace_sendMessage(DeviceID, &msg);
	if (err)
		return err;

	// Now the tracker is ready to read
	return 0;
}

/**
* Freespace reset.
* Calls init function.
***/
void FreeSpaceTracker::reset()
{
	init();
}

/**
* Release freespace tracker.
* Calls relevant freespace functions to release the device.
***/
void FreeSpaceTracker::close() 
{
	OutputDebugString("Free Tracker Close\n");
	if (DeviceID >= 0) 
	{
		// Shut off the data stream
		freespace_message msg;
		memset(&msg, 0, sizeof(msg));

		if (Device.hVer >= 2) 
		{
			msg.messageType = FREESPACE_MESSAGE_DATAMODECONTROLV2REQUEST;
			msg.dataModeControlV2Request.packetSelect = 0;        // No output 
			msg.dataModeControlV2Request.modeAndStatus = 1 << 1;  // operating mode == sleep 
		}
		else 
		{
			msg.messageType = FREESPACE_MESSAGE_DATAMODEREQUEST;
			msg.dataModeRequest.enableUserPosition = 0;
			msg.dataModeRequest.inhibitPowerManager = 0;
		}

		int err = freespace_sendMessage(DeviceID, &msg);

		// Detach from the tracker
		freespace_closeDevice(DeviceID);
		DeviceID = -1;

		freespace_exit();
	}

	OutputDebugString("Free Tracker Exit\n");
}

/**
* Destroy freespace tracker.
* Calls close function.
***/
void FreeSpaceTracker::destroy()
{
	close();
}

/**
* Retrieve freespace tracker orientation.
* Reads device input and converts data from quaternion to Euler angles.
***/
int FreeSpaceTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
#ifdef _DEBUG
	OutputDebugString("Free Tracker getOrient\n");
#endif

	freespace_message msg;

	int err = freespace_readMessage(DeviceID, &msg, 10);

	if (err == 0) 
	{
		// Check if this is a user frame message.
		if (msg.messageType == FREESPACE_MESSAGE_USERFRAME) 
		{
			// Convert from quaternion to Euler angles

			// Get the quaternion vector
			float w = msg.userFrame.angularPosA;
			float x = msg.userFrame.angularPosB;
			float y = msg.userFrame.angularPosC;
			float z = msg.userFrame.angularPosD;

			// normalize the vector
			float len = sqrtf((w*w) + (x*x) + (y*y) + (z*z));
			w /= len;
			x /= len;
			y /= len;
			z /= len;

			// The Freespace quaternion gives the rotation in terms of
			// rotating the world around the object. We take the conjugate to
			// get the rotation in the object's reference frame.
			w = w;
			x = -x;
			y = -y;
			z = -z;

			// Convert to angles in radians
			float m11 = (2.0f * w * w) + (2.0f * x * x) - 1.0f;
			float m12 = (2.0f * x * y) + (2.0f * w * z);
			float m13 = (2.0f * x * z) - (2.0f * w * y);
			float m23 = (2.0f * y * z) + (2.0f * w * x);
			float m33 = (2.0f * w * w) + (2.0f * z * z) - 1.0f;

			*roll = atan2f(m23, m33);
			*pitch = asinf(-m13);
			*yaw = atan2f(m12, m11);
			return 0;   
		}

		// any other message types will just fall through and keep looping until the timeout is reached
	}
	else
	{
#ifdef _DEBUG
		char errChar[512];
		sprintf_s(errChar, "devID = %d, err == %d", DeviceID, err);
		OutputDebugString("Freespace Error:");
		OutputDebugString(errChar);
		OutputDebugString("\n");
#endif
		return err;  // return on timeouts or serious errors
	}

	return FREESPACE_ERROR_TIMEOUT;  // The function returns gracefully without values

}

/**
* Is tracker selected and detected?
* Returns wether freespace tracker option is selected. Returns true if device ID present.
***/
bool FreeSpaceTracker::isAvailable()
{
	return DeviceID >= 0;
}
