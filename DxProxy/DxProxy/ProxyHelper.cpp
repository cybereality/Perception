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
	config.VRboostMinShaderCount = 0;

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

		config.VRboostMinShaderCount = gameProfile.attribute("minVRboostShaderCount").as_uint(0);
		config.convergence = gameProfile.attribute("convergence").as_float(0.0f);
		config.swap_eyes = gameProfile.attribute("swap_eyes").as_bool();
		config.yaw_multiplier = gameProfile.attribute("yaw_multiplier").as_float(25.0f);
		config.pitch_multiplier = gameProfile.attribute("pitch_multiplier").as_float(25.0f);
		config.roll_multiplier = gameProfile.attribute("roll_multiplier").as_float(1.0f);

		if(config.yaw_multiplier == 0.0f) config.yaw_multiplier = 25.0f;
		if(config.pitch_multiplier == 0.0f) config.pitch_multiplier = 25.0f;
		if(config.roll_multiplier == 0.0f) config.roll_multiplier = 1.0f;

		// set process name
		config.game_exe = std::string(gameProfile.attribute("game_exe").as_string(""));

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
		bool copyDlls = gameProfile.attribute("copyDlls").as_bool();
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

	LoadHUDConfig(config);
	LoadGUIConfig(config);
	LoadVRBoostValues(config);

	LoadUserConfig(config, oculusProfile);

	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param cfg The game configuration to be saved.
***/
bool ProxyHelper::SaveConfig(ProxyConfig& cfg)
{
	SaveProfile(cfg.shaderRulePath, cfg.VRboostPath, cfg.convergence, cfg.swap_eyes, cfg.yaw_multiplier, cfg.pitch_multiplier, cfg.roll_multiplier, cfg.worldScaleFactor, cfg.VRboostMinShaderCount);
	SaveHUDConfig(cfg);
	SaveGUIConfig(cfg);
	SaveVRBoostValues(cfg);
	return SaveUserConfig(cfg.ipd);
}

/**
* Loads the game configuration for the target process specified in the registry (targetExe).
* @param config Returned game configuration.
* @param oculusProfile Returned Oculus Player Profile.
***/
bool ProxyHelper::LoadHUDConfig(ProxyConfig& config)
{
	bool fileFound = false;

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
	}
	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param config The game configuration to be saved.
***/
bool ProxyHelper::SaveHUDConfig(ProxyConfig& config)
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
		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("roll_multiplier").next_attribute().name(), "hud_3D_depth_mode") == 0)
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
			gameProfile.insert_attribute_after("hud_key_full", gameProfile.attribute("roll_multiplier")) = config.hudHotkeys[4];
			gameProfile.insert_attribute_after("hud_key_large", gameProfile.attribute("roll_multiplier")) = config.hudHotkeys[3];
			gameProfile.insert_attribute_after("hud_key_small", gameProfile.attribute("roll_multiplier")) = config.hudHotkeys[2];
			gameProfile.insert_attribute_after("hud_key_default", gameProfile.attribute("roll_multiplier")) = config.hudHotkeys[1];
			gameProfile.insert_attribute_after("hud_key_swap", gameProfile.attribute("roll_multiplier")) = config.hudHotkeys[0];

			gameProfile.insert_attribute_after("hud_distance_4", gameProfile.attribute("roll_multiplier"))= config.hudDistancePresets[3];
			gameProfile.insert_attribute_after("hud_distance_3", gameProfile.attribute("roll_multiplier"))= config.hudDistancePresets[2];
			gameProfile.insert_attribute_after("hud_distance_2", gameProfile.attribute("roll_multiplier"))= config.hudDistancePresets[1];
			gameProfile.insert_attribute_after("hud_distance_1", gameProfile.attribute("roll_multiplier"))= config.hudDistancePresets[0];

			gameProfile.insert_attribute_after("hud_3D_depth_4", gameProfile.attribute("roll_multiplier")) = config.hud3DDepthPresets[3];
			gameProfile.insert_attribute_after("hud_3D_depth_3", gameProfile.attribute("roll_multiplier")) = config.hud3DDepthPresets[2];
			gameProfile.insert_attribute_after("hud_3D_depth_2", gameProfile.attribute("roll_multiplier")) = config.hud3DDepthPresets[1];
			gameProfile.insert_attribute_after("hud_3D_depth_1", gameProfile.attribute("roll_multiplier")) = config.hud3DDepthPresets[0];

			gameProfile.insert_attribute_after("hud_3D_depth_mode", gameProfile.attribute("roll_multiplier")) = config.hud3DDepthMode;
		}
		docProfiles.save_file(profilePath);

		profileSaved = true;
	}

	return profileSaved;
}

/**
* Loads the game configuration for the target process specified in the registry (targetExe).
* @param config Returned game configuration.
* @param oculusProfile Returned Oculus Player Profile.
***/
bool ProxyHelper::LoadGUIConfig(ProxyConfig& config)
{
	bool fileFound = false;

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
	}
	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param config The game configuration to be saved.
***/
bool ProxyHelper::SaveGUIConfig(ProxyConfig& config)
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
		docProfiles.save_file(profilePath);

		profileSaved = true;
	}

	return profileSaved;
}

/**
*
***/
bool ProxyHelper::LoadVRBoostValues(ProxyConfig& config)
{
	bool fileFound = false;

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
	}
	return fileFound && profileFound;
}

/**
* Saves a game configuration.
* @param config The game configuration to be saved.
***/
bool ProxyHelper::SaveVRBoostValues(ProxyConfig& config)
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
		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("gui_key_full").next_attribute().name(), "WorldFOV") == 0)
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
			gameProfile.insert_attribute_after("ConstantValue3", gameProfile.attribute("gui_key_full")) = config.ConstantValue3;
			gameProfile.insert_attribute_after("ConstantValue2", gameProfile.attribute("gui_key_full")) = config.ConstantValue2;
			gameProfile.insert_attribute_after("ConstantValue1", gameProfile.attribute("gui_key_full")) = config.ConstantValue1;

			gameProfile.insert_attribute_after("CameraHorizonAdjustment", gameProfile.attribute("gui_key_full"))= config.CameraHorizonAdjustment;
			gameProfile.insert_attribute_after("CameraZoom", gameProfile.attribute("gui_key_full"))= config.CameraZoom;
			gameProfile.insert_attribute_after("CameraDistance", gameProfile.attribute("gui_key_full"))= config.CameraDistance;

			gameProfile.insert_attribute_after("CameraTranslateZ", gameProfile.attribute("gui_key_full"))= config.CameraTranslateZ;
			gameProfile.insert_attribute_after("CameraTranslateY", gameProfile.attribute("gui_key_full")) = config.CameraTranslateY;
			gameProfile.insert_attribute_after("CameraTranslateX", gameProfile.attribute("gui_key_full")) = config.CameraTranslateX;

			gameProfile.insert_attribute_after("FarPlaneFOV", gameProfile.attribute("gui_key_full")) = config.FarPlaneFOV;
			gameProfile.insert_attribute_after("PlayerFOV", gameProfile.attribute("gui_key_full")) = config.PlayerFOV;
			gameProfile.insert_attribute_after("WorldFOV", gameProfile.attribute("gui_key_full")) = config.WorldFOV;			
		}
		docProfiles.save_file(profilePath);

		profileSaved = true;
	}

	return profileSaved;
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
* @param shaderRulePath Path to the shader rules xml file.
* @param convergence Convergence adjustment, in meters.
* @param swap True to swap eye output.
* @param yaw Yaw tracking multiplier.
* @param pitch Pitch tracking multiplier.
* @param roll Roll tracking multiplier.
* @param worldScale Game world scaling.
***/
bool ProxyHelper::SaveProfile(std::string shaderRulePath, std::string VRboostRulePath, float convergence, bool swap, float yaw, float pitch, float roll, float worldScale, int minVRboostShaderCount)
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

		// get shader mod rules filename
		auto lastBackSlash = shaderRulePath.find_last_of("\\");
		std::string fileName;
		if (lastBackSlash!=std::string::npos)
			fileName = shaderRulePath.substr(lastBackSlash+1, shaderRulePath.size()-(lastBackSlash+1));
		else
			fileName = shaderRulePath;

		// shader mod rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("game_exe").next_attribute().name(), "shaderModRules") == 0)
			gameProfile.attribute("shaderModRules") = fileName.c_str();
		else
			gameProfile.insert_attribute_after("shaderModRules", gameProfile.attribute("game_exe")) = fileName.c_str();

		// get VRboost rules filename
		lastBackSlash = VRboostRulePath.find_last_of("\\");
		if (lastBackSlash!=std::string::npos)
			fileName = VRboostRulePath.substr(lastBackSlash+1, VRboostRulePath.size()-(lastBackSlash+1));
		else
			fileName = VRboostRulePath;

		// VRboost rules attribute present ? otherwise insert
		if (strcmp(gameProfile.attribute("shaderModRules").next_attribute().name(), "VRboostRules") == 0)
			gameProfile.attribute("VRboostRules") = fileName.c_str();
		else
			gameProfile.insert_attribute_after("VRboostRules", gameProfile.attribute("shaderModRules")) = fileName.c_str();

		// change other attributes
		gameProfile.attribute("minVRboostShaderCount") = minVRboostShaderCount;
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