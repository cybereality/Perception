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

class ProxyHelper
{
public:
	ProxyHelper();

	struct ProxyConfig
	{
		int game_type;
		int stereo_mode;
		int tracker_mode;
		float separation;
		float convergence;
		float aspect_multiplier;
		bool swap_eyes;
		float yaw_multiplier;
		float pitch_multiplier;
		float roll_multiplier;

		float worldScaleFactor; // mm * worldScaleFactor = mm in game units
		bool rollEnabled;
		std::string shaderRulePath; // full path of shader rules for this game

		///// user.cfg /////
		float centerlineR;// to be used as IPD
		float centerlineL;

		float ipd; // in mm

		// separation and convergence should be here as per game config (separation becoming an adjustment that is added to ipd)
		
		////////////////////
	};

	bool LoadConfig(ProxyConfig& config);
	char* GetBaseDir();
	char* GetTargetExe();
	void GetPath(char* newFolder, char* path);
	bool baseDirLoaded; 
	char* baseDir;
	char* targetExe;
	bool HasProfile(char* name);
	bool SaveConfig(int mode = -1, float aspect = -1.0f);
	bool SaveConfig2(int mode = -1);
	bool SaveUserConfig(float centerlineL = 0.0f, float centerlineR = 0.0f);
	bool LoadUserConfig(ProxyConfig& config);
	bool GetConfig(int& mode, int& mode2);
	bool GetProfile(char* name, ProxyConfig& config);
	bool SaveProfile(float sep = 0.0f, float conv = 0.0f, bool swap_eyes = false, float yaw = 25.0f, float pitch = 25.0f, float roll = 1.0f);
};

#endif