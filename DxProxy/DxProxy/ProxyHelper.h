/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ProxyHelper.cpp> and
Class <ProxyHelper> :
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

#ifndef PROXYHELPER_H_INCLUDED
#define PROXYHELPER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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
	* All game-specific information + IPD + swap_eyes + stereo_mode + tracker_mode.
	***/
	struct ProxyConfig 
	{
		/** game configuration **/
		std::string game_exe;                   /**< Process name, to be used in VRboost. */
		std::string dir_contains;				/**< Part of the folder name (root folder best), used to filter */
		std::string shaderRulePath;             /**< Full path of shader rules for this game. */
		std::string VRboostPath;                /**< Full path of VRboost rules for this game. */
		UINT        VRboostMinShaderCount;      /**< Minimum Vertex Shader Count to apply VRboost (security) */
		UINT        VRboostMaxShaderCount;      /**< Maximum Vertex Shader Count to apply VRboost (security) */
		bool		is64bit;					/**< The game cpu-architecture, true for 64-bit games */
		int         game_type;                  /**< Game type enumeration. Matching D3DProxyDevice::ProxyTypes. */
		int         rollImpl;                   /**< 0 - NONE, 1 - Matrix Roll, 2 - Pixel Shader Roll */
		float       worldScaleFactor;           /**< Value the eye seperation is to be multiplied with. (mm * worldScaleFactor = mm in game units). */
		float       convergence;                /**< Convergence or Neutral Point distance, in meters. */
		bool        swap_eyes;                  /**< Option to swap the eye output for different 3D systems. */
		float       yaw_multiplier;             /**< Game-specific tracking multiplier (yaw). */
		float       pitch_multiplier;           /**< Game-specific tracking multiplier (pitch). */
		float       roll_multiplier;            /**< Game-specific tracking multiplier (roll). */
		float		position_multiplier;		/**< Game-specific position overall multiplier (for X, Y and Z). */
		float		position_x_multiplier;		/**< Game-specific position multiplier for X*/
		float		position_y_multiplier;		/**< Game-specific position multiplier for Y*/
		float		position_z_multiplier;		/**< Game-specific position multiplier for Z*/
		float		DistortionScale;			/**< The scale to apply distortion. **/
		float		YOffset;					/**< The Y offset from the centre of the screen on the Y-axis **/
		float		IPDOffset;					/**< The IPD offset from the centre of the screen on the X-axis **/
		bool		useSDKPosePrediction;		/**< Whether the SDK pose prediction should be used for this game **/
		int         hud3DDepthMode;             /**< Current HUD mode. */
		float       hud3DDepthPresets[4];       /**< HUD 3D Depth presets.*/
		float       hudDistancePresets[4];      /**< HUD Distance presets.*/
		byte        hudHotkeys[5];              /**< HUD Hotkeys.*/
		int         gui3DDepthMode;             /**< Current GUI mode. */
		float       gui3DDepthPresets[4];       /**< GUI 3D Depth presets.*/
		float       guiSquishPresets[4];        /**< GUI Size presets.*/
		byte        guiHotkeys[5];              /**< GUI Hotkeys.*/
		byte        VRBoostResetHotkey;         /**< Hotkey option to reset VRBoost. */
		byte        EdgePeekHotkey;				/**< Hotkey option to toggle disconnected screen view. */
		float       WorldFOV;                   /**< Game-specific global FOV setting. **/
		float       PlayerFOV;                  /**< Game-specific player surroundings FOV setting. **/
		float       FarPlaneFOV;                /**< Game-specific FOV setting for the most distant objects. **/ 
		float       CameraTranslateX;           /**< Translates the camera along the X-Axis. **/
		float       CameraTranslateY;           /**< Translates the camera along the Y-Axis. **/
		float       CameraTranslateZ;           /**< Translates the camera along the Z-Axis. **/
		float       CameraDistance;             /**< Changes the distance of the camera. **/
		float       CameraZoom;                 /**< Changes the cameras zoom. **/
		float       CameraHorizonAdjustment;    /**< Adjusts the cameras X axis rotation to match the horizon. **/
		float       ConstantValue1;             /**< Game-specific constant value 1. **/
		float       ConstantValue2;             /**< Game-specific constant value 2. **/
		float       ConstantValue3;             /**< Game-specific constant value 3. **/
		
		/** general settings **/
		int         stereo_mode;                /**< Stereo render mode enumeration. Matching StereoView::StereoTypes. */
		int         tracker_mode;               /**< Tracking mode enumeration. Matching MotionTracker::TrackingTypes. */
		float       ipd;                        /**< IPD, which stands for interpupillary distance (distance between your pupils - in meters...default = 0.064). Also called the interocular distance (or just Interocular). */
		float       aspect_multiplier;          /**< Currently unused aspect multiplier. Aspect multiplier allows adjusting aspect ratio consistently for Stereo/NoStereo. */
		UINT		display_adapter;			/**< The display adapter to use - 0 = Primary Display, 1 = Secondary and so on.. */
	};

	struct UserConfig
	{
		int mode;
		int mode2;
		int adapter;
		bool notifications;
		bool warnPosLost;
		bool obsStreamHack;
	};

	/**
	* Oculus user profile.
	* Saved from Oculus Configuration Utility to "%USERPROFILE%\AppData\Local\Oculus".
	***/
	struct OculusProfile
	{
		std::string Name;             /**< The name of the player (=profile name). **/
		std::string Gender;           /**< The gender of the player, as exported from Oculus Configuration Utility. **/
		float       PlayerHeight;     /**< The height of the player, as exported from Oculus Configuration Utility. **/
		float       IPD;              /**< IPD, which stands for interpupillary distance (distance between your pupils - in meters...default = 0.064). Also called the interocular distance (or just Interocular). */
		std::string RiftVersion;      /**< Oculus Rift Version (RiftDK1=Rift Development Kit 1) */
	};

	/*** ProxyHelper public methods ***/
	static HRESULT RegGetString(HKEY hKey, LPCTSTR szValueName, LPTSTR * lpszResult);
	static HRESULT RegGetString(HKEY hKey, LPCTSTR szValueName, std::string &resultStr);
	std::string GetBaseDir();
	std::string GetTargetExe();
	std::string GetPath(char* path);
	std::string GetTargetPath();
	std::string GetTargetPath(char* path);
	bool  LoadUserConfig(UserConfig &userConfig);
	bool  SaveUserConfig(int mode = -1, float aspect = -1.0f);
	bool  LoadUserConfig(ProxyConfig& config, OculusProfile& oculusProfile);
	bool  SaveUserConfig(float ipd = IPD_DEFAULT);
	bool  SaveTrackerMode(int mode = -1);
	bool  SaveDisplayAdapter(int adapter);
	bool  LoadConfig(ProxyConfig& config, OculusProfile& oculusProfile);	
	bool  SaveConfig(ProxyConfig& config);
	bool  HasProfile(const char* name, const char *path);
	bool  GetProfile(char* name, char *path, bool _64bit, ProxyConfig& config);
	/**
	* The following is used by the dll symlink installer to identify which game is in the
	* scanned folder
	*/
	bool  GetProfileGameExes(std::vector<std::pair<std::string, bool>> &gameExes);

private:
	/**
	* Base directory path.
	* Saved to registry by InitConfig() in Main.cpp.
	***/
	char* baseDirCache;
};

#endif