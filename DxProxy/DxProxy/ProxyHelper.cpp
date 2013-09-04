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

#include "ProxyHelper.h"

using namespace pugi;

/**
* Simple inline helper to erase characters in a string.
* @param string The string to be formatted.
* @param char The character to be deleted.
***/
void EraseCharacter(std::string& string, char character)
{
	auto found=string.find(character);
	while(found!=std::string::npos)
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
HRESULT RegGetString(HKEY hKey, LPCTSTR szValueName, LPTSTR * lpszResult) {

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

/**
* Almost empty constructor.
***/
ProxyHelper::ProxyHelper()
	: baseDirLoaded(false)
{
}

/**
* Returns the name of the Vireio Perception base directory read from registry.
* Saved to registry by InitConfig() in Main.cpp.
***/
char* ProxyHelper::GetBaseDir()
{
	if (baseDirLoaded == true){
		OutputDebugString("PxHelp: Already have base value.\n");
		return baseDir;
	}

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);

	if (openRes==ERROR_SUCCESS) 
	{
		OutputDebugString("PxHelp: Success opening key.\n");
	} 
	else 
	{
		OutputDebugString("PxHelp: Error opening key.\n");
		return "";
	}

	HRESULT hr = RegGetString(hKey, TEXT("BasePath"), &baseDir);
	if (FAILED(hr)) 
	{
		OutputDebugString("PxHelp: Error with GetString.\n");
		return "";
	} 
	else 
	{
		OutputDebugString("PxHelp: Success with GetString.\n");
		//strcpy_s(baseDir, sizeof(szVal), szVal);
		OutputDebugString(baseDir);
		OutputDebugString("\n");
		baseDirLoaded = true;
	}

	return baseDir;
}

/**
* Returns the name of the target exe process read from registry.
* Saved to registry by SaveExeName() in dllmain.cpp.
***/
char* ProxyHelper::GetTargetExe()
{
	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);

	if (openRes==ERROR_SUCCESS) 
	{
		OutputDebugString("PxHelp TE: Success opening key.\n");
	} 
	else
	{
		OutputDebugString("PxHelp TE: Error opening key.\n");
		return "";
	}

	HRESULT hr = RegGetString(hKey, TEXT("TargetExe"), &targetExe);
	if (FAILED(hr)) 
	{
		OutputDebugString("PxHelp TE: Error with GetString.\n");
		return "";
	} 
	else 
	{
		OutputDebugString("PxHelp TE: Success with GetString.\n");
		OutputDebugString(targetExe);
		OutputDebugString("\n");
	}

	return targetExe;
}

/**
* Simple helper to append a path string to the base Vireio directory.
* @param newFolder The folder string returned.
* @param path The Vireio sub-path.
***/
void ProxyHelper::GetPath(char* newFolder, char* path)
{
	strcpy_s(newFolder, 512, GetBaseDir());
	strcat_s(newFolder, 512, path);
}

/**
* Reads global config stereo and tracker mode.
* @param mode Stereo mode returned.
* @param mode2 Tracker mode returned.
***/
bool ProxyHelper::GetConfig(int& mode, int& mode2)
{
	// load the base dir for the app
	GetBaseDir();
	OutputDebugString(baseDir);
	OutputDebugString("\n");

	// get global config
	char configPath[512];
	GetPath(configPath, "cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath);

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		mode = xml_config.attribute("stereo_mode").as_int();
		mode2 = xml_config.attribute("tracker_mode").as_int();

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

	// load the base dir for the app
	GetBaseDir();
	OutputDebugString("Got base dir as: ");
	OutputDebugString(baseDir);
	OutputDebugString("\n");

	// get global config
	char configPath[512];
	GetPath(configPath, "cfg\\config.xml");
	OutputDebugString(configPath);
	OutputDebugString("\n");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath);

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		config.stereo_mode = xml_config.attribute("stereo_mode").as_int();
		config.aspect_multiplier = xml_config.attribute("aspect_multiplier").as_float();
		config.tracker_mode = xml_config.attribute("tracker_mode").as_int();

		fileFound = true;
	}


	// get the target exe
	GetTargetExe();
	OutputDebugString("Got target exe as: ");
	OutputDebugString(targetExe);
	OutputDebugString("\n");

	// get the profile
	bool profileFound = false;
	char profilePath[512];
	GetPath(profilePath, "cfg\\profiles.xml");
	OutputDebugString(profilePath);
	OutputDebugString("\n");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath);
	xml_node profile;
	xml_node gameProfile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(targetExe, profile.attribute("game_exe").value()) == 0)
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
		OutputDebugString("Set the config to profile!!!\n");
		config.game_type = gameProfile.attribute("game_type").as_int();

		char buf[32];
		LPCSTR psz = NULL;

		wsprintf(buf,"gameType: %d", gameProfile.attribute("game_type").as_int());
		psz = buf;
		OutputDebugString(psz);
		OutputDebugString("\n");

		config.convergence = gameProfile.attribute("convergence").as_float(0.0f);
		config.swap_eyes = gameProfile.attribute("swap_eyes").as_bool();
		config.yaw_multiplier = gameProfile.attribute("yaw_multiplier").as_float(25.0f);
		config.pitch_multiplier = gameProfile.attribute("pitch_multiplier").as_float(25.0f);
		config.roll_multiplier = gameProfile.attribute("roll_multiplier").as_float(1.0f);

		if(config.yaw_multiplier == 0.0f) config.yaw_multiplier = 25.0f;
		if(config.pitch_multiplier == 0.0f) config.pitch_multiplier = 25.0f;
		if(config.roll_multiplier == 0.0f) config.roll_multiplier = 1.0f;



		// get file name
		std::string shaderRulesFileName = gameProfile.attribute("shaderModRules").as_string("");

		if (!shaderRulesFileName.empty()) {
			std::stringstream sstm;
			sstm << GetBaseDir() << "cfg\\shader_rules\\" << shaderRulesFileName;
			config.shaderRulePath = sstm.str();
		}
		else {
			config.shaderRulePath = "";
		}

		config.rollEnabled = gameProfile.attribute("rollEnabled").as_bool(false);
		config.worldScaleFactor = gameProfile.attribute("worldScaleFactor").as_float(1.0f);
	}

	LoadUserConfig(config, oculusProfile);

	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param cfg The game configuration to be saved.
***/
bool ProxyHelper::SaveConfig(ProxyConfig& cfg)
{
	SaveProfile(cfg.convergence, cfg.swap_eyes, cfg.yaw_multiplier, cfg.pitch_multiplier, cfg.roll_multiplier, cfg.worldScaleFactor);
	return SaveUserConfig(cfg.ipd);
}

/**
* Saves the global Vireio Perception configuration (stereo mode and aspect multiplier).
* @param mode The chosen stereo mode option.
* @param aspect The aspect multiplier.
***/
bool ProxyHelper::SaveConfig(int mode, float aspect)
{
	// load the base dir for the app
	GetBaseDir();
	OutputDebugString(baseDir);
	OutputDebugString("\n");

	// get global config
	char configPath[512];
	GetPath(configPath, "cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath);

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		if(mode >= 0)
			xml_config.attribute("stereo_mode") = mode;
		if(aspect >= 0.0f)
			xml_config.attribute("aspect_multiplier") = aspect;

		docConfig.save_file(configPath);

		return true;
	}

	return false;
}

/**
* Saves the global Vireio Perception configuration (only tracking mode).
* @param mode The chosen tracking mode option.
***/
bool ProxyHelper::SaveConfig2(int mode)
{
	// load the base dir for the app
	GetBaseDir();
	OutputDebugString(baseDir);
	OutputDebugString("\n");

	// get global config
	char configPath[512];
	GetPath(configPath, "cfg\\config.xml");

	xml_document docConfig;
	xml_parse_result resultConfig = docConfig.load_file(configPath);

	if(resultConfig.status == status_ok)
	{
		xml_node xml_config = docConfig.child("config");

		if(mode >= 0)
			xml_config.attribute("tracker_mode") = mode;

		docConfig.save_file(configPath);

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
	char usersPath[512];
	GetPath(usersPath, "cfg\\users.xml");
	OutputDebugString(usersPath);
	OutputDebugString("\n");

	xml_document docUsers;
	xml_parse_result resultUsers = docUsers.load_file(usersPath);
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
	sstm << pValue << "\\AppData\\Local\\Oculus\\Profiles.json";
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
	std::string currentUser = "";
	std::string currentUserParsed = "";
	std::string line_stream;    
	while(std::getline(filestream, line_stream)){  

		// get line data
		std::stringstream str_stream(line_stream);
		std::string type_str;
		std::string data_str;
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

		// parse data
		if(type_str == "CurrentProfile")
		{
			auto found = line_stream.find(data_str);
			currentUser = line_stream.substr(found);
			EraseCharacter(currentUser, '"');
			EraseCharacter(currentUser, ':'); 
			EraseCharacter(currentUser, ','); 
		}
		if(type_str == "Name")
		{
			auto found = line_stream.find(data_str);
			currentUserParsed = line_stream.substr(found);
			EraseCharacter(currentUserParsed, '"');
			EraseCharacter(currentUserParsed, ':'); 
			EraseCharacter(currentUserParsed, ','); 
		}
		if((type_str == "Name") && (currentUser.compare(currentUserParsed)==0))
			oculusProfile.Name = currentUserParsed;
		if((type_str == "Gender") && (currentUser.compare(currentUserParsed)==0))
			oculusProfile.Gender = data_str;
		if((type_str == "PlayerHeight") && (currentUser.compare(currentUserParsed)==0))
		{
			std::stringstream st(data_str);
			st >> oculusProfile.PlayerHeight;
		}
		if((type_str == "IPD") && (currentUser.compare(currentUserParsed)==0))
		{
			std::stringstream st(data_str);
			st >> oculusProfile.IPD;
		}
		if((type_str == "RiftDK1") && (currentUser.compare(currentUserParsed)==0))
			oculusProfile.RiftVersion = "RiftDK1";
	}

	// set and save ipd
	config.ipd = oculusProfile.IPD;
	SaveUserConfig(oculusProfile.IPD);

	// output data to debug console
	OutputDebugString("Using Rift Profile:");
	OutputDebugString(oculusProfile.Name.c_str());
	OutputDebugString(oculusProfile.Gender.c_str());
	OutputDebugString(oculusProfile.RiftVersion.c_str());
	char buf[32];
	sprintf_s(buf,"IPD: %g", oculusProfile.IPD);
	OutputDebugString(buf);
	sprintf_s(buf,"PlayerHeight: %g", oculusProfile.PlayerHeight);
	OutputDebugString(buf);

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
	char profilePath[512];
	GetPath(profilePath, "cfg\\users.xml");
	OutputDebugString(profilePath);
	OutputDebugString("\n");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath);
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
		docProfiles.save_file(profilePath);
		profileSaved = true;
	}

	return profileSaved;
}

/**
* True if process has a configuration profile.
* @param name The exe process name.
***/
bool ProxyHelper::HasProfile(char* name)
{
	// get the profile
	bool profileFound = false;
	char profilePath[512];
	GetPath(profilePath, "cfg\\profiles.xml");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath);
	xml_node profile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(name, profile.attribute("game_exe").value()) == 0)
			{
				OutputDebugString("Found a profile!!!\n");
				profileFound = true;
				break;
			}
		}
	}

	return profileFound;
}

/**
* Currently incomplete : Get configuration for the specified process name.
* @param name The exe process name.
* @param config Currently unused: The returned configuration.
***/
bool ProxyHelper::GetProfile(char* name, ProxyConfig& config) // TODO !!! fill config
{
	// get the profile
	bool profileFound = false;
	char profilePath[512];
	GetPath(profilePath, "cfg\\profiles.xml");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath);
	xml_node profile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(name, profile.attribute("game_exe").value()) == 0)
			{
				OutputDebugString("Found a profile!!!\n");
				profileFound = true;

				break;
			}
		}
	}

	return profileFound;
}

/**
* Currently incomplete save game profile function.
* @param convergence Convergence adjustment, in meters.
* @param swap True to swap eye output.
* @param yaw Yaw tracking multiplier.
* @param pitch Pitch tracking multiplier.
* @param roll Roll tracking multiplier.
* @param worldScale Game world scaling.
***/
bool ProxyHelper::SaveProfile(float convergence, bool swap, float yaw, float pitch, float roll, float worldScale)
{
	// get the target exe
	GetTargetExe();
	OutputDebugString("Got target exe as: ");
	OutputDebugString(targetExe);
	OutputDebugString("\n");

	// get the profile
	bool profileFound = false;
	bool profileSaved = false;
	char profilePath[512];
	GetPath(profilePath, "cfg\\profiles.xml");
	OutputDebugString(profilePath);
	OutputDebugString("\n");

	xml_document docProfiles;
	xml_parse_result resultProfiles = docProfiles.load_file(profilePath);
	xml_node profile;
	xml_node gameProfile;

	if(resultProfiles.status == status_ok)
	{
		xml_node xml_profiles = docProfiles.child("profiles");

		for (xml_node profile = xml_profiles.child("profile"); profile; profile = profile.next_sibling("profile"))
		{
			if(strcmp(targetExe, profile.attribute("game_exe").value()) == 0)
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

		gameProfile.attribute("convergence") = convergence;
		gameProfile.attribute("swap_eyes") = swap;
		gameProfile.attribute("yaw_multiplier") = yaw;
		gameProfile.attribute("pitch_multiplier") = pitch;
		gameProfile.attribute("roll_multiplier") = roll;
		gameProfile.attribute("worldScaleFactor") = worldScale;
		docProfiles.save_file(profilePath);

		profileSaved = true;
	}

	return profileSaved;
}