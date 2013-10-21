/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <MotionTrackerFactory.cpp> and
Class <MotionTrackerFactory> :
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

#include "MotionTrackerFactory.h"
#include "FreeSpaceTracker.h"
#include "FreeTrackTracker.h"
#include "SharedMemoryTracker.h"
#include "OculusTracker.h"

/**
*  Get motion tracker. 
*  Creates the currently selected motion tracker class pointer.
***/
MotionTracker* MotionTrackerFactory::Get(ProxyHelper::ProxyConfig& config)
{
	MotionTracker* newTracker = NULL;

	switch(config.tracker_mode)
	{
	case MotionTracker::DISABLED:
		newTracker = new MotionTracker();
		break;
	case MotionTracker::HILLCREST:
		newTracker = new FreeSpaceTracker();
		break;
	case MotionTracker::FREETRACK:
		newTracker = new FreeTrackTracker();
		break;
	case MotionTracker::SHAREDMEMTRACK:
		newTracker = new SharedMemoryTracker();
		break;
	case MotionTracker::OCULUSTRACK:
		newTracker = new OculusTracker();
		break;
	default:
		newTracker = new MotionTracker();
		break;
	}

	return newTracker;
}