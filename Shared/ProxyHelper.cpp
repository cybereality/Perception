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

const ProxyConfig defaultConfig;


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

static void LoadHotkey(xml_node &node, const char *key, InputBindingRef *setting)
{
	// TODO: Support types of hotkey-expressions other than simple keys
	std::string hkString = node.attribute(key).as_string();
	debugf("Loaded hotkey: %s\n", hkString.c_str());
	int keycode = node.attribute(key).as_int(0);
	if(keycode == 0)
		*setting = HotkeyExpressions::Unbound();
	else
		*setting = HotkeyExpressions::Key(keycode);
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

void LoadSetting(xml_node &node, const char *key, std::string *setting)
{
	*setting = node.attribute(key).as_string(setting->c_str());
}

void LoadSetting(xml_node &node, const char *key, float *setting)
{
	*setting = node.attribute(key).as_float(*setting);
}

void LoadSetting(xml_node &node, const char *key, int *setting)
{
	*setting = node.attribute(key).as_int(*setting);
}

void LoadSetting(xml_node &node, const char *key, UINT *setting)
{
	*setting = node.attribute(key).as_uint(*setting);
}

void LoadSetting(xml_node &node, const char *key, bool *setting)
{
	*setting = node.attribute(key).as_bool(*setting);
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
		userConfig.warnCameraMalfunction = (xml_config.attribute("warn_camera_malfunction").as_int(1) != 0);

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
	config = ProxyConfig();

	// get global config
	string configPath = GetPath("cfg\\config.xml");
	debugf("%s\n", configPath.c_str());

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath.c_str());

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		LoadSetting(xml_config, "stereo_mode", &config.stereo_mode);
		LoadSetting(xml_config, "aspect_multiplier", &config.aspect_multiplier);
		LoadSetting(xml_config, "tracker_mode", &config.tracker_mode);
		LoadSetting(xml_config, "display_adapter", &config.display_adapter);

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
		LoadSetting(gameProfile, "game_type", &config.game_type);

		debugf("gameType: %d\n", gameProfile.attribute("game_type").as_int());

		LoadSetting(gameProfile, "minVRboostShaderCount", &config.VRboostMinShaderCount);
		LoadSetting(gameProfile, "maxVRboostShaderCount", &config.VRboostMaxShaderCount);
		LoadSetting(gameProfile, "convergence", &config.convergence);
		LoadSetting(gameProfile, "swap_eyes", &config.swap_eyes);
		LoadSetting(gameProfile, "yaw_multiplier", &config.yaw_multiplier);
		LoadSetting(gameProfile, "pitch_multiplier", &config.pitch_multiplier);
		LoadSetting(gameProfile, "roll_multiplier", &config.roll_multiplier);
		LoadSetting(gameProfile, "position_multiplier", &config.position_multiplier);
		LoadSetting(gameProfile, "position_x_multiplier", &config.position_x_multiplier);
		LoadSetting(gameProfile, "position_y_multiplier", &config.position_y_multiplier);
		LoadSetting(gameProfile, "position_z_multiplier", &config.position_z_multiplier);
		LoadSetting(gameProfile, "distortion_scale", &config.DistortionScale);
		LoadSetting(gameProfile, "y_offset", &config.YOffset);
		LoadSetting(gameProfile, "ipd_offset", &config.IPDOffset);
		LoadSetting(gameProfile, "use_sdk_pose_prediction", &config.useSDKPosePrediction);

		// For settings that really shouldn't be zero, if they're zero for some
		// reason, reset to default.
		if(config.yaw_multiplier == 0.0f) config.yaw_multiplier = defaultConfig.yaw_multiplier;
		if(config.pitch_multiplier == 0.0f) config.pitch_multiplier = defaultConfig.pitch_multiplier;
		if(config.roll_multiplier == 0.0f) config.roll_multiplier = defaultConfig.roll_multiplier;
		if(config.position_multiplier == 0.0f) config.position_multiplier = defaultConfig.position_multiplier;

		if(config.position_x_multiplier == 0.0f) config.position_x_multiplier = defaultConfig.position_x_multiplier;
		if(config.position_y_multiplier == 0.0f) config.position_y_multiplier = defaultConfig.position_y_multiplier;
		if(config.position_z_multiplier == 0.0f) config.position_z_multiplier = defaultConfig.position_z_multiplier;

		// set process name
		LoadSetting(gameProfile, "game_exe", &config.game_exe);
		LoadSetting(gameProfile, "dir_contains", &config.dir_contains);
		config.is64bit = string(gameProfile.attribute("cpu_architecture").as_string("32bit")) == "64bit";

		// get hud config
		LoadSetting(gameProfile, "hud_3D_depth_mode", &config.hud3DDepthMode);
		LoadSetting(gameProfile, "hud_3D_depth_1", &config.hud3DDepthPresets[0]);
		LoadSetting(gameProfile, "hud_3D_depth_2", &config.hud3DDepthPresets[1]);
		LoadSetting(gameProfile, "hud_3D_depth_3", &config.hud3DDepthPresets[2]);
		LoadSetting(gameProfile, "hud_3D_depth_4", &config.hud3DDepthPresets[3]);

		LoadSetting(gameProfile, "hud_distance_1", &config.hudDistancePresets[0]);
		LoadSetting(gameProfile, "hud_distance_2", &config.hudDistancePresets[1]);
		LoadSetting(gameProfile, "hud_distance_3", &config.hudDistancePresets[2]);
		LoadSetting(gameProfile, "hud_distance_4", &config.hudDistancePresets[3]);

		LoadHotkey(gameProfile, "hud_key_swap",    &config.hudHotkeys[0]);
		LoadHotkey(gameProfile, "hud_key_default", &config.hudHotkeys[1]);
		LoadHotkey(gameProfile, "hud_key_small",   &config.hudHotkeys[2]);
		LoadHotkey(gameProfile, "hud_key_large",   &config.hudHotkeys[3]);
		LoadHotkey(gameProfile, "hud_key_full",    &config.hudHotkeys[4]);

		// get gui config
		LoadSetting(gameProfile, "gui_3D_depth_mode", &config.gui3DDepthMode);
		LoadSetting(gameProfile, "gui_3D_depth_1", &config.gui3DDepthPresets[0]);
		LoadSetting(gameProfile, "gui_3D_depth_2", &config.gui3DDepthPresets[1]);
		LoadSetting(gameProfile, "gui_3D_depth_3", &config.gui3DDepthPresets[2]);
		LoadSetting(gameProfile, "gui_3D_depth_4", &config.gui3DDepthPresets[3]);

		LoadSetting(gameProfile, "gui_size_1", &config.guiSquishPresets[0]);
		LoadSetting(gameProfile, "gui_size_2", &config.guiSquishPresets[1]);
		LoadSetting(gameProfile, "gui_size_3", &config.guiSquishPresets[2]);
		LoadSetting(gameProfile, "gui_size_4", &config.guiSquishPresets[3]);

		LoadHotkey(gameProfile, "gui_key_swap",    &config.guiHotkeys[0]);
		LoadHotkey(gameProfile, "gui_key_default", &config.guiHotkeys[1]);
		LoadHotkey(gameProfile, "gui_key_small",   &config.guiHotkeys[2]);
		LoadHotkey(gameProfile, "gui_key_large",   &config.guiHotkeys[3]);
		LoadHotkey(gameProfile, "gui_key_full",    &config.guiHotkeys[4]);

		// get VRBoost reset hotkey and settings
		LoadHotkey(gameProfile, "VRBoost_key_reset", &config.VRBoostResetHotkey);
		LoadHotkey(gameProfile, "edge_peek_key", &config.EdgePeekHotkey);
		LoadSetting(gameProfile, "WorldFOV", &config.WorldFOV);
		LoadSetting(gameProfile, "PlayerFOV", &config.PlayerFOV);
		LoadSetting(gameProfile, "FarPlaneFOV", &config.FarPlaneFOV);

		//SB: This will need to be changed back when the memory modification stuff is updated, but for now
		//I am disabling the restore of the camera translation as it is causing confusion for a lot of people when
		//the game starts and they are detached from their avatar, or worse the scene doesn't render at all
		config.CameraTranslateX = 0.0f;
		config.CameraTranslateY = 0.0f;
		config.CameraTranslateZ = 0.0f;
		//LoadSetting(gameProfile, "CameraTranslateX", &config.CameraTranslateX);
		//LoadSetting(gameProfile, "CameraTranslateY", &config.CameraTranslateY);
		//LoadSetting(gameProfile, "CameraTranslateZ", &config.CameraTranslateZ);

		LoadSetting(gameProfile, "CameraDistance", &config.CameraDistance);
		LoadSetting(gameProfile, "CameraZoom", &config.CameraZoom);
		LoadSetting(gameProfile, "CameraHorizonAdjustment", &config.CameraHorizonAdjustment);
		LoadSetting(gameProfile, "ConstantValue1", &config.ConstantValue1);
		LoadSetting(gameProfile, "ConstantValue2", &config.ConstantValue2);
		LoadSetting(gameProfile, "ConstantValue3", &config.ConstantValue3);

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

		LoadSetting(gameProfile, "rollImpl", &config.rollImpl);
		LoadSetting(gameProfile, "worldScaleFactor", &config.worldScaleFactor);

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
		set_attribute(gameProfile, "ipd_offset", config.IPDOffset);
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
		set_attribute(gameProfile, "FarPlaneFOV", config.FarPlaneFOV);
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


ProxyConfig::ProxyConfig()
{
	game_exe = "";
	dir_contains = "";
	shaderRulePath = "";
	VRboostPath = "";
	VRboostMinShaderCount = 0;
	VRboostMaxShaderCount = 999999;
	is64bit = false;
	game_type = 0;
	rollImpl = 0;
	worldScaleFactor = 1.0f;
	convergence = 0.0f;
	swap_eyes = false;
	yaw_multiplier = DEFAULT_YAW_MULTIPLIER;
	pitch_multiplier = DEFAULT_PITCH_MULTIPLIER;
	roll_multiplier = 1.0f;
	position_multiplier = 1.0f;
	position_x_multiplier = DEFAULT_POS_TRACKING_X_MULT;
	position_y_multiplier = DEFAULT_POS_TRACKING_Y_MULT;
	position_z_multiplier = DEFAULT_POS_TRACKING_Z_MULT;
	DistortionScale = 0.0f;
	YOffset = 0.0f;
	IPDOffset = 0.0f;
	useSDKPosePrediction = true;
	hud3DDepthMode = 0;
	hud3DDepthPresets[0] = 0.0f;
	hud3DDepthPresets[1] = 0.0f;
	hud3DDepthPresets[2] = 0.0f;
	hud3DDepthPresets[3] = 0.0f;
	hudDistancePresets[0] = DEFAULT_HUD_DISTANCE_1;
	hudDistancePresets[1] = DEFAULT_HUD_DISTANCE_2;
	hudDistancePresets[2] = DEFAULT_HUD_DISTANCE_3;
	hudDistancePresets[3] = DEFAULT_HUD_DISTANCE_4;
	hudHotkeys[0] = HotkeyExpressions::Unbound();
	hudHotkeys[1] = HotkeyExpressions::Unbound();
	hudHotkeys[2] = HotkeyExpressions::Unbound();
	hudHotkeys[3] = HotkeyExpressions::Unbound();
	hudHotkeys[4] = HotkeyExpressions::Unbound();
	gui3DDepthMode = 0;
	gui3DDepthPresets[0] = 0.0f;
	gui3DDepthPresets[1] = 0.0f;
	gui3DDepthPresets[2] = 0.0f;
	gui3DDepthPresets[3] = 0.0f;
	guiSquishPresets[0] = DEFAULT_GUI_SIZE_1;
	guiSquishPresets[1] = DEFAULT_GUI_SIZE_2;
	guiSquishPresets[2] = DEFAULT_GUI_SIZE_3;
	guiSquishPresets[3] = DEFAULT_GUI_SIZE_4;
	guiHotkeys[0] = HotkeyExpressions::Unbound();
	guiHotkeys[1] = HotkeyExpressions::Unbound();
	guiHotkeys[2] = HotkeyExpressions::Unbound();
	guiHotkeys[3] = HotkeyExpressions::Unbound();
	guiHotkeys[4] = HotkeyExpressions::Unbound();
	VRBoostResetHotkey = HotkeyExpressions::Unbound();
	EdgePeekHotkey = HotkeyExpressions::Unbound();
	WorldFOV = 95.0;
	PlayerFOV = 125.0;
	FarPlaneFOV = 95.0;
	CameraTranslateX = 0.0f;
	CameraTranslateY = 0.0f;
	CameraTranslateZ = 0.0f;
	CameraDistance = 0.0f;
	CameraZoom = 0.0f;
	CameraHorizonAdjustment = 0.0f;
	ConstantValue1 = 0.0f;
	ConstantValue2 = 0.0f;
	ConstantValue3 = 0.0f;
	
	stereo_mode = 0;
	tracker_mode = 0;
	ipd = IPD_DEFAULT;
	aspect_multiplier = 1.0f;
	display_adapter = 0;
}

/**
* If swap_eyes is not set, returns the given eye; otherwise returns the opposite
* of the given eye.
*/
vireio::RenderPosition ProxyConfig::MaybeSwap(vireio::RenderPosition whichEye)
{
	if(swap_eyes) {
		if(whichEye == vireio::RenderPosition::Left)
			return vireio::RenderPosition::Right;
		else
			return vireio::RenderPosition::Left;
	} else {
		return whichEye;
	}
}
