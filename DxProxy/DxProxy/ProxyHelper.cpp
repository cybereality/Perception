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

	// open registry key
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

	// get exe
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

	// get exe path
	hr = RegGetString(hKey, TEXT("TargetPath"), &targetPath);
	if (FAILED(hr)) 
	{
		OutputDebugString("PxHelp TE: Error with GetString.\n");
		return "";
	} 
	else 
	{
		OutputDebugString("PxHelp TE: Success with GetString.\n");
		OutputDebugString(targetPath);
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
* Simple helper to append a path string to the base target directory.
* @param newFolder The folder string returned.
* @param path The target sub-path.
***/
void ProxyHelper::GetTargetPath(char* newFolder, char* path)
{
	strcpy_s(newFolder, 512, targetPath);
	strcat_s(newFolder, 512, path);
}

/**
* Reads global config stereo and tracker mode.
* @param mode Stereo mode returned.
* @param mode2 Tracker mode returned.
***/
bool ProxyHelper::LoadUserConfig(int& mode, int& mode2)
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
* Saves the global Vireio Perception configuration (stereo mode and aspect multiplier).
* @param mode The chosen stereo mode option.
* @param aspect The aspect multiplier.
***/
bool ProxyHelper::SaveUserConfig(int mode, float aspect)
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
* Saves the global Vireio Perception configuration (only tracking mode).
* @param mode The chosen tracking mode option.
***/
bool ProxyHelper::SaveTrackerMode(int mode)
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
			// first, convert to lowercase
			unsigned int i = 0;
			while (targetExe[i])
			{
				targetExe[i] = (char)tolower((int)targetExe[i]);
				i++;
			}
			std::string profileProcess = profile.attribute("game_exe").as_string();
			for (i = 0; i < profileProcess.length(); ++i)
			{
				profileProcess[i] = (char)tolower((int)profileProcess[i]);
			}
			// profile found ?
			if(strcmp(targetExe, profileProcess.c_str()) == 0)
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

		config.VRboostMinShaderCount = gameProfile.attribute("minVRboostShaderCount").as_uint(0);
		config.VRboostMaxShaderCount = gameProfile.attribute("maxVRboostShaderCount").as_uint(999999);
		config.convergence = gameProfile.attribute("convergence").as_float(0.0f);
		config.swap_eyes = gameProfile.attribute("swap_eyes").as_bool();
		config.yaw_multiplier = gameProfile.attribute("yaw_multiplier").as_float(25.0f);
		config.pitch_multiplier = gameProfile.attribute("pitch_multiplier").as_float(25.0f);
		config.roll_multiplier = gameProfile.attribute("roll_multiplier").as_float(1.0f);
		config.DistortionScale = gameProfile.attribute("distortion_scale").as_float(0.0f);
		config.YOffset = gameProfile.attribute("y_offset").as_float(0.0f);

		if(config.yaw_multiplier == 0.0f) config.yaw_multiplier = 25.0f;
		if(config.pitch_multiplier == 0.0f) config.pitch_multiplier = 25.0f;
		if(config.roll_multiplier == 0.0f) config.roll_multiplier = 1.0f;

		// set process name
		config.game_exe = std::string(gameProfile.attribute("game_exe").as_string(""));

		// get hud config
		config.hud3DDepthMode = gameProfile.attribute("hud_3D_depth_mode").as_int();

		config.hud3DDepthPresets[0] = gameProfile.attribute("hud_3D_depth_1").as_float(0.0f);
		config.hud3DDepthPresets[1] = gameProfile.attribute("hud_3D_depth_2").as_float(0.0f);
		config.hud3DDepthPresets[2] = gameProfile.attribute("hud_3D_depth_3").as_float(0.0f);
		config.hud3DDepthPresets[3] = gameProfile.attribute("hud_3D_depth_4").as_float(0.0f);

		config.hudDistancePresets[0] = gameProfile.attribute("hud_distance_1").as_float(0.5f);
		config.hudDistancePresets[1] = gameProfile.attribute("hud_distance_2").as_float(0.9f);
		config.hudDistancePresets[2] = gameProfile.attribute("hud_distance_3").as_float(0.3f);
		config.hudDistancePresets[3] = gameProfile.attribute("hud_distance_4").as_float(0.0f);

		config.hudHotkeys[0] = (byte)gameProfile.attribute("hud_key_swap").as_int(0);
		config.hudHotkeys[1] = (byte)gameProfile.attribute("hud_key_default").as_int(0);
		config.hudHotkeys[2] = (byte)gameProfile.attribute("hud_key_small").as_int(0);
		config.hudHotkeys[3] = (byte)gameProfile.attribute("hud_key_large").as_int(0);
		config.hudHotkeys[4] = (byte)gameProfile.attribute("hud_key_full").as_int(0);

		// get gui config
		config.gui3DDepthMode = gameProfile.attribute("gui_3D_depth_mode").as_int();

		config.gui3DDepthPresets[0] = gameProfile.attribute("gui_3D_depth_1").as_float(0.0f);
		config.gui3DDepthPresets[1] = gameProfile.attribute("gui_3D_depth_2").as_float(0.0f);
		config.gui3DDepthPresets[2] = gameProfile.attribute("gui_3D_depth_3").as_float(0.0f);
		config.gui3DDepthPresets[3] = gameProfile.attribute("gui_3D_depth_4").as_float(0.0f);

		config.guiSquishPresets[0] = gameProfile.attribute("gui_size_1").as_float(0.6f);
		config.guiSquishPresets[1] = gameProfile.attribute("gui_size_2").as_float(0.5f);
		config.guiSquishPresets[2] = gameProfile.attribute("gui_size_3").as_float(0.9f);
		config.guiSquishPresets[3] = gameProfile.attribute("gui_size_4").as_float(1.0f);

		config.guiHotkeys[0] = (byte)gameProfile.attribute("gui_key_swap").as_int(0);
		config.guiHotkeys[1] = (byte)gameProfile.attribute("gui_key_default").as_int(0);
		config.guiHotkeys[2] = (byte)gameProfile.attribute("gui_key_small").as_int(0);
		config.guiHotkeys[3] = (byte)gameProfile.attribute("gui_key_large").as_int(0);
		config.guiHotkeys[4] = (byte)gameProfile.attribute("gui_key_full").as_int(0);

		// get VRBoost reset hotkey and settings
		config.VRBoostResetHotkey = (byte)gameProfile.attribute("VRBoost_key_reset").as_int(0);
		config.WorldFOV = gameProfile.attribute("WorldFOV").as_float(95.0f);
		config.PlayerFOV = gameProfile.attribute("PlayerFOV").as_float(125.0f);
		config.FarPlaneFOV = gameProfile.attribute("FarPlaneFOV").as_float(95.0f);
		config.CameraTranslateX = gameProfile.attribute("CameraTranslateX").as_float(0.0f);
		config.CameraTranslateY = gameProfile.attribute("CameraTranslateY").as_float(0.0f);
		config.CameraTranslateZ = gameProfile.attribute("CameraTranslateZ").as_float(0.0f);
		config.CameraDistance = gameProfile.attribute("CameraDistance").as_float(0.0f);
		config.CameraZoom = gameProfile.attribute("CameraZoom").as_float(0.0f);
		config.CameraHorizonAdjustment = gameProfile.attribute("CameraHorizonAdjustment").as_float(0.0f);
		config.ConstantValue1 = gameProfile.attribute("ConstantValue1").as_float(0.0f);
		config.ConstantValue2 = gameProfile.attribute("ConstantValue2").as_float(0.0f);
		config.ConstantValue3 = gameProfile.attribute("ConstantValue3").as_float(0.0f);

		// get shader rules file name
		std::string shaderRulesFileName = gameProfile.attribute("shaderModRules").as_string("");

		if (!shaderRulesFileName.empty()) {
			std::stringstream sstm;
			sstm << GetBaseDir() << "cfg\\shader_rules\\" << shaderRulesFileName;
			config.shaderRulePath = sstm.str();
		}
		else {
			config.shaderRulePath = "";
		}

		// get memory rules file name
		std::string VRboostRulesFileName = gameProfile.attribute("VRboostRules").as_string("");

		if (!VRboostRulesFileName.empty()) {
			std::stringstream sstm;
			sstm << GetBaseDir() << "cfg\\VRboost_rules\\" << VRboostRulesFileName;
			config.VRboostPath = sstm.str();
		}
		else {
			config.VRboostPath = "";
		}

		config.rollEnabled = gameProfile.attribute("rollEnabled").as_bool(false);
		config.worldScaleFactor = gameProfile.attribute("worldScaleFactor").as_float(1.0f);

		// copy game dlls
		bool copyDlls = gameProfile.attribute("copyDlls").as_bool(false);
		if (copyDlls)
		{
			// d3d9.dll
			char sourcePath[512];
			GetPath(sourcePath, "bin\\d3d9.dll");
			char destPath[512];
			GetTargetPath(destPath, "d3d9.dll");

			std::stringstream sstm;
			sstm << "copy " << sourcePath << " " << destPath;
			system(sstm.str().c_str());
			OutputDebugString(sstm.str().c_str());

			// libfreespace.dll
			GetPath(sourcePath, "bin\\libfreespace.dll");
			GetTargetPath(destPath, "libfreespace.dll");

			std::stringstream sstm1;
			sstm1 << "copy " << sourcePath << " " << destPath;
			system(sstm1.str().c_str());
			OutputDebugString(sstm1.str().c_str());

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
			// first, convert to lowercase
			unsigned int i = 0;
			while (targetExe[i])
			{
				targetExe[i] = (char)tolower((int)targetExe[i]);
				i++;
			}
			std::string profileProcess = profile.attribute("game_exe").as_string();
			for (i = 0; i < profileProcess.length(); ++i)
			{
				profileProcess[i] = (char)tolower((int)profileProcess[i]);
			}
			// profile found ?
			if(strcmp(targetExe, profileProcess.c_str()) == 0)
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

		// get shader mod rules filename
		auto lastBackSlash = config.shaderRulePath.find_last_of("\\");
		std::string fileName;
		if (lastBackSlash!=std::string::npos)
			fileName = config.shaderRulePath.substr(lastBackSlash+1, config.shaderRulePath.size()-(lastBackSlash+1));
		else
			fileName = config.shaderRulePath;

		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("game_exe").next_attribute().name(), "shaderModRules") == 0)
			gameProfile.attribute("shaderModRules") = fileName.c_str();
		else
		{
			gameProfile.remove_attribute("shaderModRules");
			gameProfile.insert_attribute_after("shaderModRules", gameProfile.attribute("game_exe")) = fileName.c_str();
		}

		// get VRboost rules filename
		lastBackSlash = config.VRboostPath.find_last_of("\\");
		if (lastBackSlash!=std::string::npos)
			fileName = config.VRboostPath.substr(lastBackSlash+1, config.VRboostPath.size()-(lastBackSlash+1));
		else
			fileName = config.VRboostPath;

		// VRboost rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("shaderModRules").next_attribute().name(), "VRboostRules") == 0)
			gameProfile.attribute("VRboostRules") = fileName.c_str();
		else
		{
			gameProfile.remove_attribute("VRboostRules");
			gameProfile.insert_attribute_after("VRboostRules", gameProfile.attribute("shaderModRules")) = fileName.c_str();
		}

		// change other attributes
		if (strcmp(gameProfile.attribute("VRboostRules").next_attribute().name(), "minVRboostShaderCount") == 0)
			gameProfile.attribute("minVRboostShaderCount") = config.VRboostMinShaderCount;
		else
		{
			gameProfile.remove_attribute("minVRboostShaderCount");
			gameProfile.insert_attribute_after("minVRboostShaderCount", gameProfile.attribute("VRboostRules")) = config.VRboostMinShaderCount;
		}
		if (strcmp(gameProfile.attribute("minVRboostShaderCount").next_attribute().name(), "maxVRboostShaderCount") == 0)
			gameProfile.attribute("maxVRboostShaderCount") = config.VRboostMaxShaderCount;
		else
		{
			gameProfile.remove_attribute("maxVRboostShaderCount");
			gameProfile.insert_attribute_after("maxVRboostShaderCount", gameProfile.attribute("minVRboostShaderCount")) = config.VRboostMaxShaderCount;
		}
		if (strcmp(gameProfile.attribute("maxVRboostShaderCount").next_attribute().name(), "game_type") == 0)
			gameProfile.attribute("game_type") = config.game_type;
		else
		{
			gameProfile.remove_attribute("game_type");
			gameProfile.insert_attribute_after("game_type", gameProfile.attribute("maxVRboostShaderCount")) = config.game_type;
		}
		if (strcmp(gameProfile.attribute("game_type").next_attribute().name(), "rollEnabled") == 0)
			gameProfile.attribute("rollEnabled") = config.rollEnabled;
		else
		{
			gameProfile.remove_attribute("rollEnabled");
			gameProfile.insert_attribute_after("rollEnabled", gameProfile.attribute("game_type")) = config.rollEnabled;
		}
		if (strcmp(gameProfile.attribute("rollEnabled").next_attribute().name(), "worldScaleFactor") == 0)
			gameProfile.attribute("worldScaleFactor") = config.worldScaleFactor;
		else
		{
			gameProfile.remove_attribute("worldScaleFactor");
			gameProfile.insert_attribute_after("worldScaleFactor", gameProfile.attribute("rollEnabled")) = config.worldScaleFactor;
		}
		if (strcmp(gameProfile.attribute("worldScaleFactor").next_attribute().name(), "convergence") == 0)
			gameProfile.attribute("convergence") = config.convergence;
		else
		{
			gameProfile.remove_attribute("convergence");
			gameProfile.insert_attribute_after("convergence", gameProfile.attribute("worldScaleFactor")) = config.convergence;
		}
		if (strcmp(gameProfile.attribute("convergence").next_attribute().name(), "swap_eyes") == 0)
			gameProfile.attribute("swap_eyes") = config.swap_eyes;
		else
		{
			gameProfile.remove_attribute("swap_eyes");
			gameProfile.insert_attribute_after("swap_eyes", gameProfile.attribute("convergence")) = config.swap_eyes;
		}
		if (strcmp(gameProfile.attribute("swap_eyes").next_attribute().name(), "y_offset") == 0)
			gameProfile.attribute("y_offset") = config.YOffset;
		else
		{
			gameProfile.remove_attribute("y_offset");
			gameProfile.insert_attribute_after("y_offset", gameProfile.attribute("swap_eyes")) = config.YOffset;
		}
		if (strcmp(gameProfile.attribute("y_offset").next_attribute().name(), "yaw_multiplier") == 0)
			gameProfile.attribute("yaw_multiplier") = config.yaw_multiplier;
		else
		{
			gameProfile.remove_attribute("yaw_multiplier");
			gameProfile.insert_attribute_after("yaw_multiplier", gameProfile.attribute("y_offset")) = config.yaw_multiplier;
		}
		if (strcmp(gameProfile.attribute("yaw_multiplier").next_attribute().name(), "pitch_multiplier") == 0)
			gameProfile.attribute("pitch_multiplier") = config.pitch_multiplier;
		else
		{
			gameProfile.remove_attribute("pitch_multiplier");
			gameProfile.insert_attribute_after("pitch_multiplier", gameProfile.attribute("yaw_multiplier")) = config.pitch_multiplier;
		}
		if (strcmp(gameProfile.attribute("pitch_multiplier").next_attribute().name(), "roll_multiplier") == 0)
			gameProfile.attribute("roll_multiplier") = config.roll_multiplier;
		else
		{
			gameProfile.remove_attribute("roll_multiplier");
			gameProfile.insert_attribute_after("roll_multiplier", gameProfile.attribute("pitch_multiplier")) = config.roll_multiplier;
		}
		if (strcmp(gameProfile.attribute("roll_multiplier").next_attribute().name(), "distortion_scale") == 0)
			gameProfile.attribute("distortion_scale") = config.DistortionScale;
		else
		{
			gameProfile.remove_attribute("distortion_scale");
			gameProfile.insert_attribute_after("distortion_scale", gameProfile.attribute("roll_multiplier")) = config.DistortionScale;
		}

		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("distortion_scale").next_attribute().name(), "hud_3D_depth_mode") == 0)
		{
			gameProfile.attribute("hud_3D_depth_mode") = config.hud3DDepthMode;

			gameProfile.attribute("hud_3D_depth_1") = config.hud3DDepthPresets[0];
			gameProfile.attribute("hud_3D_depth_2") = config.hud3DDepthPresets[1];
			gameProfile.attribute("hud_3D_depth_3") = config.hud3DDepthPresets[2];
			gameProfile.attribute("hud_3D_depth_4") = config.hud3DDepthPresets[3];

			gameProfile.attribute("hud_distance_1")= config.hudDistancePresets[0];
			gameProfile.attribute("hud_distance_2")= config.hudDistancePresets[1];
			gameProfile.attribute("hud_distance_3")= config.hudDistancePresets[2];
			gameProfile.attribute("hud_distance_4")= config.hudDistancePresets[3];

			gameProfile.attribute("hud_key_swap") = config.hudHotkeys[0];
			gameProfile.attribute("hud_key_default") = config.hudHotkeys[1];
			gameProfile.attribute("hud_key_small") = config.hudHotkeys[2];
			gameProfile.attribute("hud_key_large") = config.hudHotkeys[3];
			gameProfile.attribute("hud_key_full") = config.hudHotkeys[4];
		}
		else
		{
			gameProfile.remove_attribute("hud_3D_depth_mode"); 
			gameProfile.remove_attribute("hud_3D_depth_1"); gameProfile.remove_attribute("hud_3D_depth_2");
			gameProfile.remove_attribute("hud_3D_depth_3"); gameProfile.remove_attribute("hud_3D_depth_4"); 
			gameProfile.remove_attribute("hud_distance_1"); gameProfile.remove_attribute("hud_distance_2"); 
			gameProfile.remove_attribute("hud_distance_3"); gameProfile.remove_attribute("hud_distance_4");
			gameProfile.remove_attribute("hud_key_swap"); gameProfile.remove_attribute("hud_key_default"); 
			gameProfile.remove_attribute("hud_key_small"); gameProfile.remove_attribute("hud_key_large"); 
			gameProfile.remove_attribute("hud_key_full"); 

			gameProfile.insert_attribute_after("hud_key_full", gameProfile.attribute("distortion_scale")) = config.hudHotkeys[4];
			gameProfile.insert_attribute_after("hud_key_large", gameProfile.attribute("distortion_scale")) = config.hudHotkeys[3];
			gameProfile.insert_attribute_after("hud_key_small", gameProfile.attribute("distortion_scale")) = config.hudHotkeys[2];
			gameProfile.insert_attribute_after("hud_key_default", gameProfile.attribute("distortion_scale")) = config.hudHotkeys[1];
			gameProfile.insert_attribute_after("hud_key_swap", gameProfile.attribute("distortion_scale")) = config.hudHotkeys[0];

			gameProfile.insert_attribute_after("hud_distance_4", gameProfile.attribute("distortion_scale"))= config.hudDistancePresets[3];
			gameProfile.insert_attribute_after("hud_distance_3", gameProfile.attribute("distortion_scale"))= config.hudDistancePresets[2];
			gameProfile.insert_attribute_after("hud_distance_2", gameProfile.attribute("distortion_scale"))= config.hudDistancePresets[1];
			gameProfile.insert_attribute_after("hud_distance_1", gameProfile.attribute("distortion_scale"))= config.hudDistancePresets[0];

			gameProfile.insert_attribute_after("hud_3D_depth_4", gameProfile.attribute("distortion_scale")) = config.hud3DDepthPresets[3];
			gameProfile.insert_attribute_after("hud_3D_depth_3", gameProfile.attribute("distortion_scale")) = config.hud3DDepthPresets[2];
			gameProfile.insert_attribute_after("hud_3D_depth_2", gameProfile.attribute("distortion_scale")) = config.hud3DDepthPresets[1];
			gameProfile.insert_attribute_after("hud_3D_depth_1", gameProfile.attribute("distortion_scale")) = config.hud3DDepthPresets[0];

			gameProfile.insert_attribute_after("hud_3D_depth_mode", gameProfile.attribute("distortion_scale")) = config.hud3DDepthMode;
		}

		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("hud_key_full").next_attribute().name(), "gui_3D_depth_mode") == 0)
		{
			gameProfile.attribute("gui_3D_depth_mode") = config.gui3DDepthMode;

			gameProfile.attribute("gui_3D_depth_1") = config.gui3DDepthPresets[0];
			gameProfile.attribute("gui_3D_depth_2") = config.gui3DDepthPresets[1];
			gameProfile.attribute("gui_3D_depth_3") = config.gui3DDepthPresets[2];
			gameProfile.attribute("gui_3D_depth_4") = config.gui3DDepthPresets[3];

			gameProfile.attribute("gui_size_1")= config.guiSquishPresets[0];
			gameProfile.attribute("gui_size_2")= config.guiSquishPresets[1];
			gameProfile.attribute("gui_size_3")= config.guiSquishPresets[2];
			gameProfile.attribute("gui_size_4")= config.guiSquishPresets[3];

			gameProfile.attribute("gui_key_swap") = config.guiHotkeys[0];
			gameProfile.attribute("gui_key_default") = config.guiHotkeys[1];
			gameProfile.attribute("gui_key_small") = config.guiHotkeys[2];
			gameProfile.attribute("gui_key_large") = config.guiHotkeys[3];
			gameProfile.attribute("gui_key_full") = config.guiHotkeys[4];
		}
		else
		{
			gameProfile.remove_attribute("gui_3D_depth_mode"); 
			gameProfile.remove_attribute("gui_3D_depth_1"); gameProfile.remove_attribute("gui_3D_depth_2");
			gameProfile.remove_attribute("gui_3D_depth_3"); gameProfile.remove_attribute("gui_3D_depth_4"); 
			gameProfile.remove_attribute("gui_size_1"); gameProfile.remove_attribute("gui_size_2"); 
			gameProfile.remove_attribute("gui_size_3"); gameProfile.remove_attribute("gui_size_4");
			gameProfile.remove_attribute("gui_key_swap"); gameProfile.remove_attribute("gui_key_default"); 
			gameProfile.remove_attribute("gui_key_small"); gameProfile.remove_attribute("gui_key_large"); 
			gameProfile.remove_attribute("gui_key_full"); 

			gameProfile.insert_attribute_after("gui_key_full", gameProfile.attribute("hud_key_full")) = config.guiHotkeys[4];
			gameProfile.insert_attribute_after("gui_key_large", gameProfile.attribute("hud_key_full")) = config.guiHotkeys[3];
			gameProfile.insert_attribute_after("gui_key_small", gameProfile.attribute("hud_key_full")) = config.guiHotkeys[2];
			gameProfile.insert_attribute_after("gui_key_default", gameProfile.attribute("hud_key_full")) = config.guiHotkeys[1];
			gameProfile.insert_attribute_after("gui_key_swap", gameProfile.attribute("hud_key_full")) = config.guiHotkeys[0];

			gameProfile.insert_attribute_after("gui_size_4", gameProfile.attribute("hud_key_full"))= config.guiSquishPresets[3];
			gameProfile.insert_attribute_after("gui_size_3", gameProfile.attribute("hud_key_full"))= config.guiSquishPresets[2];
			gameProfile.insert_attribute_after("gui_size_2", gameProfile.attribute("hud_key_full"))= config.guiSquishPresets[1];
			gameProfile.insert_attribute_after("gui_size_1", gameProfile.attribute("hud_key_full"))= config.guiSquishPresets[0];

			gameProfile.insert_attribute_after("gui_3D_depth_4", gameProfile.attribute("hud_key_full")) = config.gui3DDepthPresets[3];
			gameProfile.insert_attribute_after("gui_3D_depth_3", gameProfile.attribute("hud_key_full")) = config.gui3DDepthPresets[2];
			gameProfile.insert_attribute_after("gui_3D_depth_2", gameProfile.attribute("hud_key_full")) = config.gui3DDepthPresets[1];
			gameProfile.insert_attribute_after("gui_3D_depth_1", gameProfile.attribute("hud_key_full")) = config.gui3DDepthPresets[0];

			gameProfile.insert_attribute_after("gui_3D_depth_mode", gameProfile.attribute("hud_key_full")) = config.gui3DDepthMode;
		}

		if (strcmp(gameProfile.attribute("gui_key_full").next_attribute().name(), "VRBoost_key_reset") == 0)
			gameProfile.attribute("VRBoost_key_reset") = config.VRBoostResetHotkey;
		else
		{
			gameProfile.remove_attribute("VRBoost_key_reset");
			gameProfile.insert_attribute_after("VRBoost_key_reset", gameProfile.attribute("gui_key_full")) = config.VRBoostResetHotkey;
		}

		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("VRBoost_key_reset").next_attribute().name(), "WorldFOV") == 0)
		{
			gameProfile.attribute("WorldFOV") = config.WorldFOV;
			gameProfile.attribute("PlayerFOV") = config.PlayerFOV;
			gameProfile.attribute("FarPlaneFOV") = config.FarPlaneFOV;

			gameProfile.attribute("CameraTranslateX") = config.CameraTranslateX;
			gameProfile.attribute("CameraTranslateY") = config.CameraTranslateY;
			gameProfile.attribute("CameraTranslateZ")= config.CameraTranslateZ;

			gameProfile.attribute("CameraDistance")= config.CameraDistance;
			gameProfile.attribute("CameraZoom")= config.CameraZoom;
			gameProfile.attribute("CameraHorizonAdjustment")= config.CameraHorizonAdjustment;

			gameProfile.attribute("ConstantValue1") = config.ConstantValue1;
			gameProfile.attribute("ConstantValue2") = config.ConstantValue2;
			gameProfile.attribute("ConstantValue3") = config.ConstantValue3;
		}
		else
		{
			gameProfile.remove_attribute("WorldFOV"); gameProfile.remove_attribute("PlayerFOV"); 
			gameProfile.remove_attribute("FarPlaneFOV"); 
			gameProfile.remove_attribute("CameraTranslateX"); gameProfile.remove_attribute("CameraTranslateY"); 
			gameProfile.remove_attribute("CameraTranslateZ"); 
			gameProfile.remove_attribute("CameraDistance"); gameProfile.remove_attribute("CameraZoom"); 
			gameProfile.remove_attribute("CameraHorizonAdjustment"); 
			gameProfile.remove_attribute("ConstantValue1"); gameProfile.remove_attribute("ConstantValue2"); 
			gameProfile.remove_attribute("ConstantValue3"); 

			gameProfile.insert_attribute_after("ConstantValue3", gameProfile.attribute("VRBoost_key_reset")) = config.ConstantValue3;
			gameProfile.insert_attribute_after("ConstantValue2", gameProfile.attribute("VRBoost_key_reset")) = config.ConstantValue2;
			gameProfile.insert_attribute_after("ConstantValue1", gameProfile.attribute("VRBoost_key_reset")) = config.ConstantValue1;

			gameProfile.insert_attribute_after("CameraHorizonAdjustment", gameProfile.attribute("VRBoost_key_reset"))= config.CameraHorizonAdjustment;
			gameProfile.insert_attribute_after("CameraZoom", gameProfile.attribute("VRBoost_key_reset"))= config.CameraZoom;
			gameProfile.insert_attribute_after("CameraDistance", gameProfile.attribute("VRBoost_key_reset"))= config.CameraDistance;

			gameProfile.insert_attribute_after("CameraTranslateZ", gameProfile.attribute("VRBoost_key_reset"))= config.CameraTranslateZ;
			gameProfile.insert_attribute_after("CameraTranslateY", gameProfile.attribute("VRBoost_key_reset")) = config.CameraTranslateY;
			gameProfile.insert_attribute_after("CameraTranslateX", gameProfile.attribute("VRBoost_key_reset")) = config.CameraTranslateX;

			gameProfile.insert_attribute_after("FarPlaneFOV", gameProfile.attribute("VRBoost_key_reset")) = config.FarPlaneFOV;
			gameProfile.insert_attribute_after("PlayerFOV", gameProfile.attribute("VRBoost_key_reset")) = config.PlayerFOV;
			gameProfile.insert_attribute_after("WorldFOV", gameProfile.attribute("VRBoost_key_reset")) = config.WorldFOV;			
		}

		docProfiles.save_file(profilePath);

		profileSaved = true;
	}

	return (profileSaved || SaveUserConfig(config.ipd));
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
