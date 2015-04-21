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

#include "ProxyHelper.h"
#include "VireioUtil.h"
#include "ConfigDefaults.h"

#include <algorithm>
#include <stdarg.h>

using namespace pugi;
using namespace vireio;
using std::string;

#ifdef x64
	#define CPUARCH_STR "64bit"
#else
	#define CPUARCH_STR "32bit"
#endif

/**
* Simple inline helper to erase characters in a string.
* @param string The string to be formatted.
* @param char The character to be deleted.
***/
void EraseCharacter(string& string, char character)
{
	auto found=string.find(character);
	while(found!=string::npos)
	{
		string.erase(found,1);
		found=string.find(character);
	}
}

/**
* Reads a string from registry.
* @param hKey Registry key handle.
* @param szValueName Name of the value to be read.
* @param lpszResult The string result.
***/
HRESULT ProxyHelper::RegGetString(HKEY hKey, LPCTSTR szValueName, LPTSTR * lpszResult) {

	// Given a HKEY and value name returns a string from the registry.
	// Upon successful return the string should be freed using free()
	// eg. RegGetString(hKey, TEXT("my value"), &szString);

	DWORD dwType=0, dwDataSize=0, dwBufSize=0;
	LONG lResult;

	// Incase we fail set the return string to null...
	if (lpszResult != NULL) *lpszResult = NULL;

	// Check input parameters...
	if (hKey == NULL || lpszResult == NULL) return E_INVALIDARG;

	// Get the length of the string in bytes (placed in dwDataSize)...
	lResult = RegQueryValueEx(hKey, szValueName, 0, &dwType, NULL, &dwDataSize );

	// Check result and make sure the registry value is a string(REG_SZ)...
	if (lResult != ERROR_SUCCESS) return HRESULT_FROM_WIN32(lResult);
	else if (dwType != REG_SZ)    return DISP_E_TYPEMISMATCH;

	// Allocate memory for string - We add space for a null terminating character...
	dwBufSize = dwDataSize + (1 * sizeof(TCHAR));
	*lpszResult = (LPTSTR)malloc(dwBufSize);

	if (*lpszResult == NULL) return E_OUTOFMEMORY;

	// Now get the actual string from the registry...
	lResult = RegQueryValueEx(hKey, szValueName, 0, &dwType, (LPBYTE) *lpszResult, &dwDataSize );

	// Check result and type again.
	// If we fail here we must free the memory we allocated...
	if (lResult != ERROR_SUCCESS) { free(*lpszResult); return HRESULT_FROM_WIN32(lResult); }
	else if (dwType != REG_SZ)    { free(*lpszResult); return DISP_E_TYPEMISMATCH; }

	// We are not guaranteed a null terminated string from RegQueryValueEx.
	// Explicitly null terminate the returned string...
	(*lpszResult)[(dwBufSize / sizeof(TCHAR)) - 1] = TEXT('\0');

	return NOERROR;
}

HRESULT ProxyHelper::RegGetString(HKEY hKey, LPCTSTR szValueName, std::string &resultStr)
{
	char *cstr = NULL;
	HRESULT success = RegGetString(hKey, szValueName, &cstr);
	if(cstr != NULL) {
		resultStr = cstr;
		free(cstr);
	} else {
		resultStr = "";
	}
	return success;
}

string strToLower(string& str)
{
	string result = str;
	for(unsigned ii=0; ii<result.size(); ii++)
		result[ii] = ::tolower(result[ii]);
	return result;
}

/**
 * Set an attribute on an XML node.
 * In theory, XML nodes have an ordered collection of named attributes, duplicates
 * allowed. In practice, we're using it as an unordered string=>string dictionary.
 */
template<class T> static void set_attribute(xml_node &node, const char *key, T newValue)
{
	if(node.attribute(key).empty())
	{
		node.append_attribute(key).set_value(newValue);
	}
	else
	{
		node.attribute(key).set_value(newValue);
	}
}

static InputBindingRef LoadHotkey(xml_node &node, const char *key)
{
	// TODO: Support types of hotkey-expressions other than simple keys
	std::string hkString = node.attribute(key).as_string();
	debugf("Loaded hotkey: %s\n", hkString.c_str());
	int keycode = node.attribute(key).as_int(0);
	if(keycode == 0)
		return HotkeyExpressions::Unbound();
	else
		return HotkeyExpressions::Key(keycode);
}

static void SaveHotkey(xml_node &node, const char *key, InputBindingRef hotkey)
{
	// TODO: Support types of hotkey-expressions other than simple keys
	SimpleKeyBinding *simpleHotkey = dynamic_cast<SimpleKeyBinding*>(&*hotkey);
	if(simpleHotkey == NULL) {
		debugf("Trying to save non-simple hotkey: %s\n", hotkey->ToString().c_str());
	} else {
		debugf("Saving simple hotkey");
		set_attribute(node, key, simpleHotkey->GetKeyCode());
	}
}


/**
* Almost empty constructor.
***/
ProxyHelper::ProxyHelper()
{
	baseDirCache = NULL;
}

/**
* Returns the name of the Vireio Perception base directory read from registry.
* Saved to registry by InitConfig() in Main.cpp.
***/
std::string ProxyHelper::GetBaseDir()
{
	if (baseDirCache != NULL) {
		return baseDirCache;
	}

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);

	if (openRes!=ERROR_SUCCESS) 
	{
		OutputDebugString("PxHelp: Error opening key.\n");
		return "";
	}

	string baseDir = "";
	HRESULT hr = RegGetString(hKey, TEXT("BasePath"), baseDir);
	if (FAILED(hr)) 
	{
		OutputDebugString("Error getting BasePath from registry.\n");
		return "";
	} 
	else 
	{
		debugf("%s\n", baseDir.c_str());
		baseDirCache = _strdup(baseDir.c_str());
	}

	return baseDir;
}

/**
* Returns the name of the target exe process read from registry.
* Saved to registry by SaveExeName() in dllmain.cpp.
***/
string ProxyHelper::GetTargetExe()
{
	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	// open registry key
	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);
	if (openRes!=ERROR_SUCCESS) 
	{
		OutputDebugString("Error opening registry key.\n");
		return "";
	}

	// get exe
	string targetExe;
	HRESULT hr = RegGetString(hKey, TEXT("TargetExe"), targetExe);
	if (FAILED(hr))
	{
		OutputDebugString("Error getting TargetExe from registry.\n");
		return "";
	}

	return targetExe;
}

string ProxyHelper::GetTargetPath()
{
	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	// open registry key
	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);
	if (openRes!=ERROR_SUCCESS)
	{
		OutputDebugString("Error opening registry key.\n");
		return "";
	}

	// get exe path
	string targetPath;
	HRESULT hr = RegGetString(hKey, TEXT("TargetPath"), targetPath);
	if (FAILED(hr))
	{
		OutputDebugString("Error getting TargetPath from registry.\n");
		return "";
	}
	
	return targetPath;
}

/**
* Simple helper to append a path string to the base Vireio directory.
* @param newFolder The folder string returned.
* @param path The Vireio sub-path.
***/
string ProxyHelper::GetPath(char* path)
{
	return retprintf("%s%s", GetBaseDir().c_str(), path);
}

/**
* Simple helper to append a path string to the base target directory.
* @param newFolder The folder string returned.
* @param path The target sub-path.
***/
string ProxyHelper::GetTargetPath(char* path)
{
	return retprintf("%s%s", GetTargetPath().c_str(), path);
}

/**
* Reads global config stereo and tracker mode.
* @param mode Stereo mode returned.
* @param mode2 Tracker mode returned.
***/
bool ProxyHelper::LoadUserConfig(UserConfig &userConfig)
{
	// get global config
	string configPath = GetPath("cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		userConfig.mode = xml_config.attribute("stereo_mode").as_int();
		userConfig.mode2 = xml_config.attribute("tracker_mode").as_int();
		userConfig.adapter = xml_config.attribute("display_adapter").as_int(0);
		userConfig.notifications = (xml_config.attribute("notifications").as_int(1) != 0);
		userConfig.warnPosLost = (xml_config.attribute("warn_positional_lost").as_int(1) != 0);

		//Thhis triggers the "hack" to get obs streaming the game without stereo and distortion
		userConfig.obsStreamHack = (xml_config.attribute("obs_stream_hack").as_int(0) != 0);

		return true;
	}

	return false;
}

/**
* Saves the global Vireio Perception configuration (stereo mode and aspect multiplier).
* @param mode The chosen stereo mode option.
* @param aspect The aspect multiplier.
***/
bool ProxyHelper::SaveUserConfig(int mode, float aspect)
{
	// get global config
	string configPath = GetPath("cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		if(mode >= 0)
			xml_config.attribute("stereo_mode") = mode;
		if(aspect >= 0.0f)
			xml_config.attribute("aspect_multiplier") = aspect;

		docConfig.save_file(configPath.c_str());

		return true;
	}

	return false;
}

/**
* Loads the user configuration.
* @param config Returned user configuration.
***/
bool ProxyHelper::LoadUserConfig(ProxyConfig& config, OculusProfile& oculusProfile)
{
	// get the user_profile
	bool userFound = false;
	string usersPath = GetPath("cfg\\users.xml");
	debugf("%s\n", usersPath.c_str());

	xml_document docUsers;
	xml_parse_result resultUsers = docUsers.load_file(usersPath.c_str());
	xml_node users;
	xml_node userProfile;

	config.ipd = IPD_DEFAULT;

	// first, load user settings stored in "cfg\users.xml"
	if(resultUsers.status == status_ok)
	{
		xml_node xml_user_profiles = docUsers.child("users");

		for (xml_node user_profile = xml_user_profiles.child("user"); user_profile; user_profile = user_profile.next_sibling("user"))
		{
			if(strcmp("default", user_profile.attribute("user_name").value()) == 0)
			{
				OutputDebugString("Load the specific user!!!\n");
				userProfile = user_profile;
				userFound = true;
				config.ipd = userProfile.attribute("ipd").as_float(IPD_DEFAULT);
				break;
			}
		}
	}

	// get user path
	char *pValue;
	size_t len;
	errno_t err = _dupenv_s( &pValue, &len, "userprofile" );
	std::stringstream sstm;
	sstm << pValue << "\\AppData\\Local\\Oculus\\ProfileDB.json";
	std::string filePath = sstm.str();
	OutputDebugString(filePath.c_str());

	// load the oculus rift user profile
	std::ifstream filestream;
	filestream.open(filePath);

	// set default data
	oculusProfile.Name = "Default";
	oculusProfile.Gender = "Unspecified";
	oculusProfile.IPD = config.ipd;
	oculusProfile.PlayerHeight = 1.0f;
	oculusProfile.RiftVersion = "None";		

	// user profile existing ? use default ipd-data from "user.cfg" file
	if (!filestream.is_open())
	{
		OutputDebugString("No Oculus User Profile present.");
		return false;
	}

	// loop through file data lines
	OutputDebugString("Load Oculus User Profile.");
	string defaultUser = "";
	string currentUserParsed = "";
	string line_stream;    
	while(std::getline(filestream, line_stream)){  

		// get line data
		std::stringstream str_stream(line_stream);
		string type_str;
		string data_str;
		str_stream >> type_str >> data_str;

		

		// erase unnecessarry characters
		EraseCharacter(type_str, '"'); 
		EraseCharacter(type_str, ':'); 
		EraseCharacter(type_str, ','); 
		EraseCharacter(type_str, ' ');

		EraseCharacter(data_str, '"'); 
		EraseCharacter(data_str, ':'); 
		EraseCharacter(data_str, ','); 
		EraseCharacter(data_str, ' ');		

		OutputDebugString(type_str.c_str());
		OutputDebugString(data_str.c_str());

		// parse data
		if(type_str == "DefaultUser")
		{
			auto found = line_stream.find(data_str);
			defaultUser = line_stream.substr(found);
			EraseCharacter(defaultUser, '"');
			EraseCharacter(defaultUser, ':'); 
			EraseCharacter(defaultUser, ','); 			
		}
		if(type_str == "User")
		{
			auto found = line_stream.find(data_str);
			currentUserParsed = line_stream.substr(found);
			EraseCharacter(currentUserParsed, '"');
			EraseCharacter(currentUserParsed, ':'); 
			EraseCharacter(currentUserParsed, ',');				
		}
		if((type_str == "User") && (defaultUser.compare(currentUserParsed)==0))
			oculusProfile.Name = currentUserParsed;
		if((type_str == "Gender") && (defaultUser.compare(currentUserParsed)==0))
			oculusProfile.Gender = data_str;
		if((type_str == "PlayerHeight") && (defaultUser.compare(currentUserParsed)==0))
		{
			std::stringstream st(data_str);
			st >> oculusProfile.PlayerHeight;
		}
		if((type_str == "IPD") && (defaultUser.compare(currentUserParsed)==0))
		{
			std::stringstream st(data_str);
			st >> oculusProfile.IPD;
		}
		if((data_str == "RiftDK1") && (defaultUser.compare(currentUserParsed)==0))
			oculusProfile.RiftVersion = "RiftDK1";
		else if(data_str == "RiftDK2" && (defaultUser.compare(currentUserParsed)==0))
			oculusProfile.RiftVersion = "RiftDK2";
	}

	// set and save ipd
	config.ipd = oculusProfile.IPD;
	SaveUserConfig(oculusProfile.IPD);

	// output data to debug console
	OutputDebugString("Using Rift Profile:");
	OutputDebugString(oculusProfile.Name.c_str());
	OutputDebugString(oculusProfile.Gender.c_str());
	OutputDebugString(oculusProfile.RiftVersion.c_str());
	debugf("IPD: %g", oculusProfile.IPD);
	debugf("PlayerHeight: %g", oculusProfile.PlayerHeight);

	return userFound;
}

/**
* Save user interpupillary distance.
* @param ipd The interpupillary distance.
***/
bool ProxyHelper::SaveUserConfig(float ipd)
{
	// get the profile
	bool profileFound = false;
	bool profileSaved = false;
	string profilePath = GetPath("cfg\\users.xml");
	debugf("%s\n", profilePath.c_str());

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;
	xml_node gameProfile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("users");

		for (xml_node profile = xml_profiles.child("user"); profile; profile = profile.next_sibling("user"))
		{
			if(strcmp("default", profile.attribute("user_name").value()) == 0)
			{
				OutputDebugString("Load the specific profile!!!\n");
				gameProfile = profile;
				profileFound = true;
				break;
			}
		}
	}

	if(resultProfiles.status == status_ok && profileFound && gameProfile)
	{
		OutputDebugString("Save the settings to profile!!!\n");
		gameProfile.attribute("ipd") = ipd;
		docProfiles.save_file(profilePath.c_str());
		profileSaved = true;
	}

	return profileSaved;
}

/**
* Saves the global Vireio Perception configuration (only tracking mode).
* @param mode The chosen tracking mode option.
***/
bool ProxyHelper::SaveTrackerMode(int mode)
{
	// get global config
	string configPath = GetPath("cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		if(mode >= 0)
			xml_config.attribute("tracker_mode") = mode;

		docConfig.save_file(configPath.c_str());

		return true;
	}

	return false;
}

/**
* Saves the global Vireio Perception configuration (only selected display adapter).
* @param mode The chosen display adapter.
***/
bool ProxyHelper::SaveDisplayAdapter(int adapter)
{
	// get global config
	string configPath = GetPath("cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		if(adapter >= 0)
			xml_config.attribute("display_adapter") = adapter;

		docConfig.save_file(configPath.c_str());

		return true;
	}

	return false;
}

/**
* Loads the game configuration for the target process specified in the registry (targetExe).
* @param config Returned game configuration.
* @param oculusProfile Returned Oculus Player Profile.
***/
bool ProxyHelper::LoadConfig(ProxyConfig& config, OculusProfile& oculusProfile)
{
	bool fileFound = false;

	// set defaults
	config.game_type = 0;
	config.stereo_mode = 0;
	config.tracker_mode = 0;
	config.convergence = 0.0f;
	config.swap_eyes = false;
	config.aspect_multiplier = 1.0f;
	config.VRboostMinShaderCount = 0;
	config.VRboostMaxShaderCount = 999999;
	config.DistortionScale = 0.0f;
	config.YOffset = 0.0f;
	config.IPDOffset = 0.0f;

	// get global config
	string configPath = GetPath("cfg\\config.xml");
	debugf("%s\n", configPath.c_str());

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		config.stereo_mode = xml_config.attribute("stereo_mode").as_int();
		config.aspect_multiplier = xml_config.attribute("aspect_multiplier").as_float();
		config.tracker_mode = xml_config.attribute("tracker_mode").as_int();
		config.display_adapter = xml_config.attribute("display_adapter").as_int(0);

		fileFound = true;
	}


	// get the target exe
	string targetExe = GetTargetExe();
	debugf("Got target exe as: %s\n", targetExe.c_str());
	targetExe = strToLower(targetExe);

	// get the profile
	bool profileFound = false;
	string profilePath = GetPath("cfg\\profiles.xml");
	debugf("%s\n", profilePath.c_str());
	string targetPath = GetTargetPath();

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;
	xml_node gameProfile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			// first, convert to lowercase
			string profileProcess = profile.attribute("game_exe").as_string();
			profileProcess = strToLower(profileProcess);

			string cpuArch = profile.attribute("cpu_architecture").as_string("32bit");

			// profile found - now has to match on exe name and cpu architecture
			if(targetExe == profileProcess
			   && cpuArch == string(CPUARCH_STR))
			{
				//Check against dir name too if present
				if (string(profile.attribute("dir_contains").as_string()).length() && targetPath.length()>0)
				{
					if (targetPath.find(profile.attribute("dir_contains").as_string()) == string::npos)
						continue;
				}

				debugf("Found specific profile: %s (%s)\n", profile.attribute("game_name").as_string(), cpuArch.c_str());
				gameProfile = profile;
				profileFound = true;
				break;
			}
		}
	}

	if(resultProfiles.status == status_ok && profileFound && gameProfile)
	{
		OutputDebugString("Set the config to profile!!!\n");
		config.game_type = gameProfile.attribute("game_type").as_int();

		debugf("gameType: %d\n", gameProfile.attribute("game_type").as_int());

		config.VRboostMinShaderCount = gameProfile.attribute("minVRboostShaderCount").as_uint(0);
		config.VRboostMaxShaderCount = gameProfile.attribute("maxVRboostShaderCount").as_uint(999999);
		config.convergence = gameProfile.attribute("convergence").as_float(0.0f);
		config.swap_eyes = gameProfile.attribute("swap_eyes").as_bool();
		config.yaw_multiplier = gameProfile.attribute("yaw_multiplier").as_float(DEFAULT_YAW_MULTIPLIER);
		config.pitch_multiplier = gameProfile.attribute("pitch_multiplier").as_float(DEFAULT_PITCH_MULTIPLIER);
		config.roll_multiplier = gameProfile.attribute("roll_multiplier").as_float(1.0f);
		config.position_multiplier = gameProfile.attribute("position_multiplier").as_float(1.0f);
		config.position_x_multiplier = gameProfile.attribute("position_x_multiplier").as_float(DEFAULT_POS_TRACKING_X_MULT);
		config.position_y_multiplier = gameProfile.attribute("position_y_multiplier").as_float(DEFAULT_POS_TRACKING_Y_MULT);
		config.position_z_multiplier = gameProfile.attribute("position_z_multiplier").as_float(DEFAULT_POS_TRACKING_Z_MULT);
		config.DistortionScale = gameProfile.attribute("distortion_scale").as_float(0.0f);
		config.YOffset = gameProfile.attribute("y_offset").as_float(0.0f);
		config.IPDOffset = gameProfile.attribute("ipd_offset").as_float(0.0f);
		config.useSDKPosePrediction = gameProfile.attribute("use_sdk_pose_prediction").as_bool(true);

		if(config.yaw_multiplier == 0.0f) config.yaw_multiplier = DEFAULT_YAW_MULTIPLIER;
		if(config.pitch_multiplier == 0.0f) config.pitch_multiplier = DEFAULT_PITCH_MULTIPLIER;
		if(config.roll_multiplier == 0.0f) config.roll_multiplier = 1.0f;
		if(config.position_multiplier == 0.0f) config.position_multiplier = 1.0f;

		//With some experimentation, these feel most natural (in Skyrim)
		if(config.position_x_multiplier == 0.0f) config.position_x_multiplier = DEFAULT_POS_TRACKING_X_MULT;
		if(config.position_y_multiplier == 0.0f) config.position_y_multiplier = DEFAULT_POS_TRACKING_Y_MULT;
		if(config.position_z_multiplier == 0.0f) config.position_z_multiplier = DEFAULT_POS_TRACKING_Z_MULT;

		// set process name
		config.game_exe = string(gameProfile.attribute("game_exe").as_string(""));
		config.dir_contains = string(gameProfile.attribute("dir_contains").as_string(""));
		config.is64bit = string(gameProfile.attribute("cpu_architecture").as_string("32bit")) == "64bit";

		// get hud config
		config.hud3DDepthMode = gameProfile.attribute("hud_3D_depth_mode").as_int();

		config.hud3DDepthPresets[0] = gameProfile.attribute("hud_3D_depth_1").as_float(0.0f);
		config.hud3DDepthPresets[1] = gameProfile.attribute("hud_3D_depth_2").as_float(0.0f);
		config.hud3DDepthPresets[2] = gameProfile.attribute("hud_3D_depth_3").as_float(0.0f);
		config.hud3DDepthPresets[3] = gameProfile.attribute("hud_3D_depth_4").as_float(0.0f);

		config.hudDistancePresets[0] = gameProfile.attribute("hud_distance_1").as_float(DEFAULT_HUD_DISTANCE_1);
		config.hudDistancePresets[1] = gameProfile.attribute("hud_distance_2").as_float(DEFAULT_HUD_DISTANCE_2);
		config.hudDistancePresets[2] = gameProfile.attribute("hud_distance_3").as_float(DEFAULT_HUD_DISTANCE_3);
		config.hudDistancePresets[3] = gameProfile.attribute("hud_distance_4").as_float(DEFAULT_HUD_DISTANCE_4);

		config.hudHotkeys[0] = LoadHotkey(gameProfile, "hud_key_swap");
		config.hudHotkeys[1] = LoadHotkey(gameProfile, "hud_key_default");
		config.hudHotkeys[2] = LoadHotkey(gameProfile, "hud_key_small");
		config.hudHotkeys[3] = LoadHotkey(gameProfile, "hud_key_large");
		config.hudHotkeys[4] = LoadHotkey(gameProfile, "hud_key_full");

		// get gui config
		config.gui3DDepthMode = gameProfile.attribute("gui_3D_depth_mode").as_int();

		config.gui3DDepthPresets[0] = gameProfile.attribute("gui_3D_depth_1").as_float(0.0f);
		config.gui3DDepthPresets[1] = gameProfile.attribute("gui_3D_depth_2").as_float(0.0f);
		config.gui3DDepthPresets[2] = gameProfile.attribute("gui_3D_depth_3").as_float(0.0f);
		config.gui3DDepthPresets[3] = gameProfile.attribute("gui_3D_depth_4").as_float(0.0f);

		config.guiSquishPresets[0] = gameProfile.attribute("gui_size_1").as_float(DEFAULT_GUI_SIZE_1);
		config.guiSquishPresets[1] = gameProfile.attribute("gui_size_2").as_float(DEFAULT_GUI_SIZE_2);
		config.guiSquishPresets[2] = gameProfile.attribute("gui_size_3").as_float(DEFAULT_GUI_SIZE_3);
		config.guiSquishPresets[3] = gameProfile.attribute("gui_size_4").as_float(DEFAULT_GUI_SIZE_4);

		config.guiHotkeys[0] = LoadHotkey(gameProfile, "gui_key_swap");
		config.guiHotkeys[1] = LoadHotkey(gameProfile, "gui_key_default");
		config.guiHotkeys[2] = LoadHotkey(gameProfile, "gui_key_small");
		config.guiHotkeys[3] = LoadHotkey(gameProfile, "gui_key_large");
		config.guiHotkeys[4] = LoadHotkey(gameProfile, "gui_key_full");

		// get VRBoost reset hotkey and settings
		config.VRBoostResetHotkey = LoadHotkey(gameProfile, "VRBoost_key_reset");
		config.EdgePeekHotkey = LoadHotkey(gameProfile, "edge_peek_key");
		config.WorldFOV = gameProfile.attribute("WorldFOV").as_float(95.0f);
		config.PlayerFOV = gameProfile.attribute("PlayerFOV").as_float(125.0f);
		config.FarPlaneFOV = gameProfile.attribute("FarPlaneFOV").as_float(95.0f);

		//SB: This will need to be changed back when the memory modification stuff is updated, but for now
		//I am disabling the restore of the camera translation as it is causing confusion for a lot of people when
		//the game starts and they are detached from their avatar, or worse the scene doesn't render at all
		config.CameraTranslateX = 0.0f;//gameProfile.attribute("CameraTranslateX").as_float(0.0f);
		config.CameraTranslateY = 0.0f;//gameProfile.attribute("CameraTranslateY").as_float(0.0f);
		config.CameraTranslateZ = 0.0f;//gameProfile.attribute("CameraTranslateZ").as_float(0.0f);


		config.CameraDistance = gameProfile.attribute("CameraDistance").as_float(0.0f);
		config.CameraZoom = gameProfile.attribute("CameraZoom").as_float(0.0f);
		config.CameraHorizonAdjustment = gameProfile.attribute("CameraHorizonAdjustment").as_float(0.0f);
		config.ConstantValue1 = gameProfile.attribute("ConstantValue1").as_float(0.0f);
		config.ConstantValue2 = gameProfile.attribute("ConstantValue2").as_float(0.0f);
		config.ConstantValue3 = gameProfile.attribute("ConstantValue3").as_float(0.0f);

		// get shader rules file name
		string shaderRulesFileName = gameProfile.attribute("shaderModRules").as_string("");

		if (!shaderRulesFileName.empty()) {
			std::stringstream sstm;
			sstm << GetBaseDir() << "cfg\\shader_rules\\" << shaderRulesFileName;
			config.shaderRulePath = sstm.str();
		}
		else {
			config.shaderRulePath = "";
		}

		// get memory rules file name
		string VRboostRulesFileName = gameProfile.attribute("VRboostRules").as_string("");

		if (!VRboostRulesFileName.empty()) {
			std::stringstream sstm;
			sstm << GetBaseDir() << "cfg\\VRboost_rules\\" << VRboostRulesFileName;
			config.VRboostPath = sstm.str();
		}
		else {
			config.VRboostPath = "";
		}

		config.rollImpl = gameProfile.attribute("rollImpl").as_int(0);
		config.worldScaleFactor = gameProfile.attribute("worldScaleFactor").as_float(1.0f);

		// copy game dlls
		bool copyDlls = gameProfile.attribute("copyDlls").as_bool(false);
		if (copyDlls)
		{
			// d3d9.dll
			string sourcePath = GetPath("bin\\d3d9.dll");
			string destPath = GetTargetPath("d3d9.dll");

			string copyD3D9Command = retprintf("copy %s %s", sourcePath.c_str(), destPath.c_str());
			system(copyD3D9Command.c_str());
			OutputDebugString(copyD3D9Command.c_str());

			// libfreespace.dll
			sourcePath = GetPath("bin\\libfreespace.dll");
			destPath = GetTargetPath("libfreespace.dll");

			string copyLibfreespaceCommand = retprintf("copy %s %s", sourcePath.c_str(), destPath.c_str());
			system(copyLibfreespaceCommand.c_str());
			OutputDebugString(copyLibfreespaceCommand.c_str());

		}
	}

	/*LoadHUDConfig(config);
	LoadGUIConfig(config);
	LoadVRBoostValues(config);*/

	LoadUserConfig(config, oculusProfile);

	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param cfg The game configuration to be saved.
***/
bool ProxyHelper::SaveConfig(ProxyConfig& config)
{
	// get the target exe
	string targetExe = GetTargetExe();
	debugf("Got target exe as: %s\n", targetExe.c_str());
	targetExe = strToLower(targetExe);

	// get the profile
	bool profileFound = false;
	bool hasDirContains = false;
	bool profileSaved = false;
	string profilePath = GetPath("cfg\\profiles.xml");
	debugf("%s\n", profilePath.c_str());
	string targetPath = GetTargetPath();

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;
	xml_node gameProfile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			string profileProcess = profile.attribute("game_exe").as_string();
			profileProcess = strToLower(profileProcess);

			string cpuArch = profile.attribute("cpu_architecture").as_string("32bit");

			// profile found - now has to match on exe name and cpu architecture
			if(targetExe == profileProcess &&
				cpuArch == string(CPUARCH_STR))
			{
				//Check against dir name too if present
				string dirContains = profile.attribute("dir_contains").as_string();
				if (dirContains.length() && targetPath.length())
				{
					hasDirContains = true;
					if (targetPath.find(dirContains) == string::npos)
						continue;
				}
				else hasDirContains = false;

				debugf("Found specific profile: %s (%s), dirContains: %s\n", targetExe.c_str(), cpuArch.c_str(), hasDirContains?"TRUE":"FALSE");
				gameProfile = profile;
				profileFound = true;
				break;
			}
		}
	}

	if(resultProfiles.status == status_ok && profileFound && gameProfile)
	{
		OutputDebugString("Save the settings to profile!!!\n");

		// get shader mod rules filename
		auto lastBackSlash = config.shaderRulePath.find_last_of("\\");
		string fileName;
		if (lastBackSlash!=string::npos)
			fileName = config.shaderRulePath.substr(lastBackSlash+1, config.shaderRulePath.size()-(lastBackSlash+1));
		else
			fileName = config.shaderRulePath;

		string firstTag = "game_exe";
		if (hasDirContains) firstTag = "dir_contains";

		set_attribute(gameProfile, "cpu_architecture", config.is64bit ? "64bit" : "32bit");

		// shader mod rules attribute present ? otherwise insert
		set_attribute(gameProfile, "shaderModRules", fileName.c_str());

		// get VRboost rules filename
		lastBackSlash = config.VRboostPath.find_last_of("\\");
		if (lastBackSlash!=string::npos)
			fileName = config.VRboostPath.substr(lastBackSlash+1, config.VRboostPath.size()-(lastBackSlash+1));
		else
			fileName = config.VRboostPath;
		set_attribute(gameProfile, "VRboostRules", fileName.c_str());

		set_attribute(gameProfile, "minVRboostShaderCount", config.VRboostMinShaderCount);
		set_attribute(gameProfile, "maxVRboostShaderCount", config.VRboostMaxShaderCount);
		set_attribute(gameProfile, "game_type", config.game_type);
		set_attribute(gameProfile, "rollImpl", config.rollImpl);
		set_attribute(gameProfile, "worldScaleFactor", config.worldScaleFactor);
		set_attribute(gameProfile, "convergence", config.convergence);
		set_attribute(gameProfile, "swap_eyes", config.swap_eyes);
		set_attribute(gameProfile, "ipd_offset", config.YOffset);
		set_attribute(gameProfile, "use_sdk_pose_prediction", config.useSDKPosePrediction);
		set_attribute(gameProfile, "y_offset", config.YOffset);
		set_attribute(gameProfile, "yaw_multiplier", config.yaw_multiplier);
		set_attribute(gameProfile, "pitch_multiplier", config.pitch_multiplier);
		set_attribute(gameProfile, "roll_multiplier", config.roll_multiplier);
		
		set_attribute(gameProfile, "position_multiplier", config.position_multiplier);
		set_attribute(gameProfile, "position_x_multiplier", config.position_x_multiplier);
		set_attribute(gameProfile, "position_y_multiplier", config.position_y_multiplier);
		set_attribute(gameProfile, "position_z_multiplier", config.position_z_multiplier);
		set_attribute(gameProfile, "distortion_scale", config.DistortionScale);

		set_attribute(gameProfile, "hud_3D_depth_mode", config.hud3DDepthMode);

		set_attribute(gameProfile, "hud_3D_depth_1", config.hud3DDepthPresets[0]);
		set_attribute(gameProfile, "hud_3D_depth_2", config.hud3DDepthPresets[1]);
		set_attribute(gameProfile, "hud_3D_depth_3", config.hud3DDepthPresets[2]);
		set_attribute(gameProfile, "hud_3D_depth_4", config.hud3DDepthPresets[3]);

		set_attribute(gameProfile, "hud_distance_1", config.hudDistancePresets[0]);
		set_attribute(gameProfile, "hud_distance_2", config.hudDistancePresets[1]);
		set_attribute(gameProfile, "hud_distance_3", config.hudDistancePresets[2]);
		set_attribute(gameProfile, "hud_distance_4", config.hudDistancePresets[3]);

		SaveHotkey(gameProfile, "hud_key_swap", config.hudHotkeys[0]);
		SaveHotkey(gameProfile, "hud_key_default", config.hudHotkeys[1]);
		SaveHotkey(gameProfile, "hud_key_small", config.hudHotkeys[2]);
		SaveHotkey(gameProfile, "hud_key_large", config.hudHotkeys[3]);
		SaveHotkey(gameProfile, "hud_key_full", config.hudHotkeys[4]);

		set_attribute(gameProfile, "gui_3D_depth_mode", config.gui3DDepthMode);

		set_attribute(gameProfile, "gui_3D_depth_1", config.gui3DDepthPresets[0]);
		set_attribute(gameProfile, "gui_3D_depth_2", config.gui3DDepthPresets[1]);
		set_attribute(gameProfile, "gui_3D_depth_3", config.gui3DDepthPresets[2]);
		set_attribute(gameProfile, "gui_3D_depth_4", config.gui3DDepthPresets[3]);

		set_attribute(gameProfile, "gui_size_1", config.guiSquishPresets[0]);
		set_attribute(gameProfile, "gui_size_2", config.guiSquishPresets[1]);
		set_attribute(gameProfile, "gui_size_3", config.guiSquishPresets[2]);
		set_attribute(gameProfile, "gui_size_4", config.guiSquishPresets[3]);

		SaveHotkey(gameProfile, "gui_key_swap", config.guiHotkeys[0]);
		SaveHotkey(gameProfile, "gui_key_default", config.guiHotkeys[1]);
		SaveHotkey(gameProfile, "gui_key_small", config.guiHotkeys[2]);
		SaveHotkey(gameProfile, "gui_key_large", config.guiHotkeys[3]);
		SaveHotkey(gameProfile, "gui_key_full", config.guiHotkeys[4]);
		
		SaveHotkey(gameProfile, "VRBoost_key_reset", config.VRBoostResetHotkey);

		set_attribute(gameProfile, "WorldFOV", config.WorldFOV);
		set_attribute(gameProfile, "PlayerFOV", config.PlayerFOV);
		set_attribute(gameProfile, "FarPlaneFOV", config.PlayerFOV);
		set_attribute(gameProfile, "CameraTranslateX", config.CameraTranslateX);
		set_attribute(gameProfile, "CameraTranslateY", config.CameraTranslateY);
		set_attribute(gameProfile, "CameraTranslateZ", config.CameraTranslateZ);
		set_attribute(gameProfile, "CameraDistance", config.CameraDistance);
		set_attribute(gameProfile, "CameraZoom", config.CameraZoom);
		set_attribute(gameProfile, "CameraHorizonAdjustment", config.CameraHorizonAdjustment);
		set_attribute(gameProfile, "ConstantValue1", config.ConstantValue1);
		set_attribute(gameProfile, "ConstantValue2", config.ConstantValue2);
		set_attribute(gameProfile, "ConstantValue3", config.ConstantValue3);

		SaveHotkey(gameProfile, "edge_peek_key", config.EdgePeekHotkey);

		docProfiles.save_file(profilePath.c_str());

		profileSaved = true;
	}

	return (profileSaved || SaveUserConfig(config.ipd));
}

/**
* True if process has a configuration profile.
* @param name The exe process name.
***/
bool ProxyHelper::HasProfile(const char* name, const char *path)
{
	// get the profile
	bool profileFound = false;
	string profilePath = GetPath("cfg\\profiles.xml");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(name, profile.attribute("game_exe").value()) == 0)
			{
				//Check against dir name too if present
				if (profile.attribute("dir_contains").as_string() && path)
				{
					if (string(path).find(profile.attribute("dir_contains").as_string()) == string::npos)
						continue;
				}

				OutputDebugString("Found a profile!!!\n");
				profileFound = true;
				break;
			}
		}
	}

	return profileFound;
}

bool ProxyHelper::GetProfileGameExes(std::vector<std::pair<std::string, bool>> &gameExes)
{
	// get the profile
	string profilePath = GetPath("cfg\\profiles.xml");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			string exe = profile.attribute("game_exe").value();
			exe = strToLower(exe);
			bool _64bit = profile.attribute("cpu_architecture").as_string("32bit") == string("64bit");
			gameExes.push_back(std::make_pair(exe, _64bit));
		}
	}

	return gameExes.size() > 0;
}


/**
* Currently incomplete : Get configuration for the specified process name.
* @param name The exe process name.
* @param config Currently unused: The returned configuration.
* TODO !!! fill config
***/
bool ProxyHelper::GetProfile(char* name, char *path, bool _64bit, ProxyConfig& config)
{
	// get the profile
	bool profileFound = false;
	string profilePath = GetPath("cfg\\profiles.xml");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath.c_str());
	xml_node profile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(name, profile.attribute("game_exe").value()) == 0 &&
				_64bit == (profile.attribute("cpu_architecture").as_string("32bit") == string("64bit")))
			{
				//Check against dir name too if present
				if (profile.attribute("dir_contains").as_string() && path)
				{
					if (string(path).find(profile.attribute("dir_contains").as_string()) == string::npos)
						continue;
				}

				OutputDebugString("Found a profile!!!\n");
				profileFound = true;

				break;
			}
		}
	}

	return profileFound;
}
