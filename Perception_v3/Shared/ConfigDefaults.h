/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ConfigDefaults.h>:
Copyright (C) 2015 Simon Brown

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#ifndef CONFIG_DEFAULTS_H_INCLUDED
#define CONFIG_DEFAULTS_H_INCLUDED

// 64mm in meters
#define IPD_DEFAULT 0.064f

#define DEFAULT_CONVERGENCE 3.0f
#define DEFAULT_PFOV 110.0f
#define DEFAULT_ASPECT_MULTIPLIER 1.0f

#define DEFAULT_YAW_MULTIPLIER 25.0f
#define DEFAULT_PITCH_MULTIPLIER 25.0f
#define DEFAULT_ROLL_MULTIPLIER 1.0f

#define DEFAULT_POS_TRACKING_X_MULT 2.0f
#define DEFAULT_POS_TRACKING_Y_MULT 2.5f
#define DEFAULT_POS_TRACKING_Z_MULT 0.5f

#define DEFAULT_GUI_SIZE_1 0.6f
#define DEFAULT_GUI_SIZE_2 0.5f
#define DEFAULT_GUI_SIZE_3 0.9f
#define DEFAULT_GUI_SIZE_4 1.0f
#define DEFAULT_HUD_DISTANCE_1 0.5f
#define DEFAULT_HUD_DISTANCE_2 0.9f
#define DEFAULT_HUD_DISTANCE_3 0.3f
#define DEFAULT_HUD_DISTANCE_4 0.0f

#define COMFORT_MODE_STICK_THRESHOLD 0.5f
#define MENU_ADJUSTMENT_STICK_DEADZONE 0.25f
#define MENU_SELECTION_STICK_DEADZONE 0.25f
#define BOUND_STICK_DEADZONE 0.5f

#endif
