#include <qfileinfo.h>
#include <qdir.h>
#include "cConfig.h"
#include "cPropsFile.h"

cConfig config;


cConfig::cConfig(){
	//SB: This will need to be changed back when the memory modification stuff is updated, but for now
	//I am disabling the restore of the camera translation as it is causing confusion for a lot of people when
	//the game starts and they are detached from their avatar, or worse the scene doesn't render at all
	CameraTranslateX = 0.0f;
	CameraTranslateY = 0.0f;
	CameraTranslateZ = 0.0f;
	PlayerIPD    = 0.0638;
}


bool cConfig::load( const QString& file ){
	cPropsFile props;
	if( !props.load( file ) ){
		return false;
	}

	//global settings
	props.get( &stereoDevice      , "stereoDevice"      );
	props.get( &trackerMode       , "trackerMode"       );
	props.get( &logToConsole      , "logToConsole"      );
	props.get( &logToFile         , "logToFile"         );
	props.get( &pauseOnLaunch     , "pauseOnLaunch"     );
	props.get( &streamingEnable   , "streamingEnable"   );
	props.get( &streamingAddress  , "streamingAddress"  );
	props.get( &streamingPort     , "streamingPort"     );
	props.get( &streamingCodec    , "streamingCodec"    );
	props.get( &streamingBitrate  , "streamingBitrate"  );
	props.get( &showNotifications , "showNotifications" );

	//device settings
	props.get( &shader                        , "shader"                        );
	props.get( &isHmd                         , "isHmd"                         );
	props.get( &resolutionWidth               , "resolutionWidth"               );
	props.get( &resolutionHeight              , "resolutionHeight"              );
	props.get( &physicalWidth                 , "physicalWidth"                 );
	props.get( &physicalHeight                , "physicalHeight"                );
	props.get( &distortionCoefficients[0]     , "distortionCoefficients_0"      );
	props.get( &distortionCoefficients[1]     , "distortionCoefficients_1"      );
	props.get( &distortionCoefficients[2]     , "distortionCoefficients_2"      );
	props.get( &distortionCoefficients[3]     , "distortionCoefficients_3"      );
	props.get( &chromaCoefficients[0]         , "chromaCoefficients_0"          );
	props.get( &chromaCoefficients[1]         , "chromaCoefficients_1"          );
	props.get( &chromaCoefficients[2]         , "chromaCoefficients_2"          );
	props.get( &chromaCoefficients[3]         , "chromaCoefficients_3"          );
	props.get( &eyeToScreenDistance           , "eyeToScreenDistance"           );
	props.get( &physicalLensSeparation        , "physicalLensSeparation"        );
	props.get( &lensYCenterOffset             , "lensYCenterOffset"             );
	props.get( &lensIPDCenterOffset           , "lensIPDCenterOffset"           );
	props.get( &minDistortionScale            , "minDistortionScale"            );
	props.get( &chromaticAberrationCorrection , "chromaticAberrationCorrection" );

	//game settings
	props.get( &exeName                       , "exeName"                       );
	props.get( &exePath                       , "exePath"                       );
	props.get( &profileName                   , "profileName"                   );

	//profile settings
	props.get( &exeName                       , "game_exe"                      );
	props.get( &game_type                     , "game_type"                     );
	props.get( &VRboostMinShaderCount         , "minVRboostShaderCount"         );
	props.get( &VRboostMaxShaderCount         , "maxVRboostShaderCount"         );
	props.get( &convergence                   , "convergence"                   );
	props.get( &swap_eyes                     , "swap_eyes"                     );
	props.get( &yaw_multiplier                , "yaw_multiplier"                );
	props.get( &pitch_multiplier              , "pitch_multiplier"              );
	props.get( &roll_multiplier               , "roll_multiplier"               );
	props.get( &position_multiplier           , "position_multiplier"           );
	props.get( &DistortionScale               , "distortion_scale"              );
	props.get( &YOffset                       , "y_offset"                      );
	props.get( &IPDOffset                     , "ipd_offset"                    );
	props.get( &hud3DDepthMode                , "hud_3D_depth_mode"             );
	props.get( &hud3DDepthPresets[0]          , "hud_3D_depth_1"                );
	props.get( &hud3DDepthPresets[1]          , "hud_3D_depth_2"                );
	props.get( &hud3DDepthPresets[2]          , "hud_3D_depth_3"                );
	props.get( &hud3DDepthPresets[3]          , "hud_3D_depth_4"                );
	props.get( &hudDistancePresets[0]         , "hud_distance_1"                );
	props.get( &hudDistancePresets[1]         , "hud_distance_2"                );
	props.get( &hudDistancePresets[2]         , "hud_distance_3"                );
	props.get( &hudDistancePresets[3]         , "hud_distance_4"                );
	props.get( &hudHotkeys[0]                 , "hud_key_swap"                  );
	props.get( &hudHotkeys[1]                 , "hud_key_default"               );
	props.get( &hudHotkeys[2]                 , "hud_key_small"                 );
	props.get( &hudHotkeys[3]                 , "hud_key_large"                 );
	props.get( &hudHotkeys[4]                 , "hud_key_full"                  );
	props.get( &gui3DDepthMode                , "gui_3D_depth_mode"             );
	props.get( &gui3DDepthPresets[0]          , "gui_3D_depth_1"                );
	props.get( &gui3DDepthPresets[1]          , "gui_3D_depth_2"                );
	props.get( &gui3DDepthPresets[2]          , "gui_3D_depth_3"                );
	props.get( &gui3DDepthPresets[3]          , "gui_3D_depth_4"                );
	props.get( &guiSquishPresets[0]           , "gui_size_1"                    );
	props.get( &guiSquishPresets[1]           , "gui_size_2"                    );
	props.get( &guiSquishPresets[2]           , "gui_size_3"                    );
	props.get( &guiSquishPresets[3]           , "gui_size_4"                    );
	props.get( &guiHotkeys[0]                 , "gui_key_swap"                  );
	props.get( &guiHotkeys[1]                 , "gui_key_default"               );
	props.get( &guiHotkeys[2]                 , "gui_key_small"                 );
	props.get( &guiHotkeys[3]                 , "gui_key_large"                 );
	props.get( &guiHotkeys[4]                 , "gui_key_full"                  );
	props.get( &VRBoostResetHotkey            , "VRBoost_key_reset"             );
	props.get( &WorldFOV                      , "WorldFOV"                      );
	props.get( &PlayerFOV                     , "PlayerFOV"                     );
	props.get( &FarPlaneFOV                   , "FarPlaneFOV"                   );
	props.get( &CameraDistance                , "CameraDistance"                );
	props.get( &CameraZoom                    , "CameraZoom"                    );
	props.get( &CameraHorizonAdjustment       , "CameraHorizonAdjustment"       );
	props.get( &ConstantValue1                , "ConstantValue1"                );
	props.get( &ConstantValue2                , "ConstantValue2"                );
	props.get( &ConstantValue3                , "ConstantValue3"                );
	props.get( &shaderRulePath                , "shaderModRules"                );
	props.get( &VRboostPath                   , "VRboostRules"                  );
	props.get( &rollEnabled                   , "rollEnabled"                   );
	props.get( &worldScaleFactor              , "worldScaleFactor"              );
	props.get( &SteamAppId                    , "SteamAppId"                    );
	props.get( &CommandLineArguments          , "CommandLineArguments"          );

	float t;
	props.get( &t              , "CameraTranslateX"              );
	props.get( &t              , "CameraTranslateY"              );
	props.get( &t              , "CameraTranslateZ"              );

	return true;
}



bool cConfig::save( const QString& file , int flags ){
	cPropsFile props;

	if( flags & SAVE_GENERAL ){
		props.set( &stereoDevice      , "stereoDevice"      );
		props.set( &trackerMode       , "trackerMode"       );
		props.set( &logToConsole      , "logToConsole"      );
		props.set( &logToFile         , "logToFile"         );
		props.set( &pauseOnLaunch     , "pauseOnLaunch"     );
		props.set( &streamingEnable   , "streamingEnable"   );
		props.set( &streamingAddress  , "streamingAddress"  );
		props.set( &streamingPort     , "streamingPort"     );
		props.set( &streamingCodec    , "streamingCodec"    );
		props.set( &streamingBitrate  , "streamingBitrate"  );
		props.set( &showNotifications , "showNotifications" );
	}

	if( flags & SAVE_GAME ){
		props.set( &exePath     , "exePath"     );
		props.set( &profileName , "profileName" );
	}

	return props.save( file );
}







void cConfig::calculateValues(){
	screenAspectRatio = resolutionWidth / (float)resolutionHeight;

	float physicalViewCenter = physicalWidth * 0.25f;
	float physicalOffset     = physicalViewCenter - physicalLensSeparation * 0.5f;

	// Range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1  range
	lensXCenterOffset = 4.0f * physicalOffset / physicalWidth;

	float radius       = -1 - lensXCenterOffset;
	float radiusSqared = radius * radius;
	float distort      = radius * (distortionCoefficients[0] + distortionCoefficients[1] * radiusSqared + distortionCoefficients[2] * radiusSqared * radiusSqared + distortionCoefficients[3] * radiusSqared * radiusSqared * radiusSqared);

	scaleToFillHorizontal = distort / radius;
}


QString cConfig::getMainConfigFile( ){
	return vireioDir + "config/main.ini";
}


QString cConfig::getGameConfigFile( const QString& gameExePath ){
	QString s = gameExePath;
	s.replace( ':'  , '_' );
	s.replace( '\\' , '_' );
	s.replace( '/'  , '_' );
	return vireioDir + "/games/" + s + ".ini";
}


QString cConfig::findProfileFileForExe( const QString& gameExePath ){

	QString exe = QFileInfo(gameExePath).fileName();

	for( QFileInfo& info : QDir(vireioDir+"./profiles").entryInfoList(QDir::Files) ){
		cConfig cfg;
		if( cfg.load( info.absoluteFilePath() ) ){
			if( cfg.exeName == exe ){
				return info.baseName();
			}
		}
	}

	return "";
}


QStringList cConfig::getAvailableProfiles( ){
	QStringList ret;
	for( QFileInfo& info : QDir(config.vireioDir+"/profiles").entryInfoList(QDir::Files) ){
		ret += info.baseName();
	}
	return ret;
}


QStringList cConfig::getAvailableDevices( ){
	QStringList ret;
	ret += "Oculus Rift (auto)";

	for( QFileInfo& info : QDir(config.vireioDir+"/modes").entryInfoList(QDir::Files) ){
		ret += info.baseName();
	}
	return ret;
}


QString cConfig::getProfileConfigFile( ){
	return vireioDir + "/profiles/" + profileName + ".ini";
}



bool cConfig::loadProfile( ){
	return load( vireioDir + "/profiles/" + profileName + ".ini" );
}


bool cConfig::saveProfile( ){
	return save( vireioDir + "/profiles/" + profileName + ".ini" , SAVE_PROFILE );
}


bool cConfig::loadDevice ( ){
	if( stereoDevice == "Oculus Rift (auto)" ){
		return loadOculusSdk();
	}else{
		return load( vireioDir + "/modes/" + stereoDevice + ".ini" );
	}
}