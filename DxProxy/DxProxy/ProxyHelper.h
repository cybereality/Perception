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

#ifndef PROXYHELPER_H_INCLUDED
#define PROXYHELPER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include "pugixml.hpp"
#include "Vireio.h"

/**
* Game configuration helper class.
* Contains game configuration structure (ProxyConfig).
***/
class __declspec(dllexport) ProxyHelper
{
public:
	ProxyHelper();
	
	/**
	* Game configuration structure.
	* Separation and convergence should be here as per game config (separation becoming an adjustment that is added to ipd).
	* Convergence still missing (watch ViewAdjustment class...)!!
	***/
	struct ProxyConfig // TODO !! add convergence ?
	{
		int game_type;               /**< Game type enumeration. Matching D3DProxyDevice::ProxyTypes. */
		int stereo_mode;             /**< Stereo render mode enumeration. Matching StereoView::StereoTypes. */
		int tracker_mode;            /**< Tracking mode enumeration. Matching MotionTracker::TrackingTypes. */
		float separationAdjustment;  /**< Eye seperation, became an adjustment that is added to ipd. To be handled in ViewAdjustment class. */
		float aspect_multiplier;     /**< Currently unused aspect multiplier. Aspect multiplier allows adjusting aspect ratio consistently for Stereo/NoStereo. */
		bool swap_eyes;              /**< Option to swap the eye output for different 3D systems. */
		float yaw_multiplier;        /**< Game-specific tracking multiplier (yaw). */
		float pitch_multiplier;      /**< Game-specific tracking multiplier (pitch). */
		float roll_multiplier;       /**< Game-specific tracking multiplier (roll). */
		float worldScaleFactor;      /**< Value the eye seperation is to be multiplied with. (mm * worldScaleFactor = mm in game units). */
		bool rollEnabled;            /**< True if headtracking-roll is to be enabled. */
		std::string shaderRulePath;  /**< Full path of shader rules for this game. */
		float ipd;                   /**< Currently unused IPD, which stands for interpupillary distance (distance between your pupils - in mm). */
	};

	/*** ProxyHelper public methods ***/
	char* GetBaseDir();
	char* GetTargetExe();
	void  GetPath(char* newFolder, char* path);
	bool  GetConfig(int& mode, int& mode2);
	bool  LoadConfig(ProxyConfig& config);	
	bool  SaveConfig(ProxyConfig& config);
	bool  SaveConfig(int mode = -1, float aspect = -1.0f);
	bool  SaveConfig2(int mode = -1);
	bool  LoadUserConfig(ProxyConfig& config);
	bool  SaveUserConfig(float ipd = IPD_DEFAULT);
	bool  HasProfile(char* name);
	bool  GetProfile(char* name, ProxyConfig& config);
	bool  SaveProfile(float sepAdjustment = 0.0f, bool swap_eyes = false, float yaw = 25.0f, float pitch = 25.0f, float roll = 1.0f, float worldScale = 1.0f);

	/**
	* True if base directory path loaded.
	***/
	bool  baseDirLoaded; 
	/**
	* Base directory path.
	* Saved to registry by InitConfig() in Main.cpp.
	***/
	char* baseDir;
	/**
	* Name of the game process the profile will be loaded for.
	* Saved to registry by SaveExeName() in dllmain.cpp.
	***/
	char* targetExe;
};

#endif