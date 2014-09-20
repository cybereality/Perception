#include "cGameProfile.h"
#include "cPropsFile.h"
#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>


cGameProfile::cGameProfile(){
	all().append( this );
}


cGameProfile::~cGameProfile(){
	all().removeAll( this );
}


bool cGameProfile::load( QString path ){
	cPropsFile props;
	if( !props.load( path ) ){
		return false;
	}

	profileName            = QFileInfo(path).baseName();
	exeName                = props.getString( "game_exe"                        );
	game_type              = props.getInt   ( "game_type"                       );
	VRboostMinShaderCount  = props.getInt   ( "minVRboostShaderCount"  , 0      );
	VRboostMaxShaderCount  = props.getInt   ( "maxVRboostShaderCount"  , 999999 );
	convergence            = props.getFloat ( "convergence"            , 0.0f   );
	swap_eyes              = props.getBool  ( "swap_eyes"              , false  );
	yaw_multiplier         = props.getFloat ( "yaw_multiplier"         , 25.0f  );
	pitch_multiplier       = props.getFloat ( "pitch_multiplier"       , 25.0f  );
	roll_multiplier        = props.getFloat ( "roll_multiplier"        , 1.0f   );
	position_multiplier    = props.getFloat ( "position_multiplier"    , 1.0f   );
	DistortionScale        = props.getFloat ( "distortion_scale"       , 0.0f   );
	YOffset                = props.getFloat ( "y_offset"               , 0.0f   );
	IPDOffset              = props.getFloat ( "ipd_offset"             , 0.0f   );
	hud3DDepthMode         = props.getInt   ( "hud_3D_depth_mode"      , 0      );
	hud3DDepthPresets[0]   = props.getFloat ( "hud_3D_depth_1"         , 0.0f   );
	hud3DDepthPresets[1]   = props.getFloat ( "hud_3D_depth_2"         , 0.0f   );
	hud3DDepthPresets[2]   = props.getFloat ( "hud_3D_depth_3"         , 0.0f   );
	hud3DDepthPresets[3]   = props.getFloat ( "hud_3D_depth_4"         , 0.0f   );
	hudDistancePresets[0]  = props.getFloat ( "hud_distance_1"         , 0.5f   );
	hudDistancePresets[1]  = props.getFloat ( "hud_distance_2"         , 0.9f   );
	hudDistancePresets[2]  = props.getFloat ( "hud_distance_3"         , 0.3f   );
	hudDistancePresets[3]  = props.getFloat ( "hud_distance_4"         , 0.0f   );
	hudHotkeys[0]          = props.getInt   ( "hud_key_swap"           , 0      );
	hudHotkeys[1]          = props.getInt   ( "hud_key_default"        , 0      );
	hudHotkeys[2]          = props.getInt   ( "hud_key_small"          , 0      );
	hudHotkeys[3]          = props.getInt   ( "hud_key_large"          , 0      );
	hudHotkeys[4]          = props.getInt   ( "hud_key_full"           , 0      );
	gui3DDepthMode         = props.getInt   ( "gui_3D_depth_mode"      , 0      );
	gui3DDepthPresets[0]   = props.getFloat ( "gui_3D_depth_1"         , 0.0f   );
	gui3DDepthPresets[1]   = props.getFloat ( "gui_3D_depth_2"         , 0.0f   );
	gui3DDepthPresets[2]   = props.getFloat ( "gui_3D_depth_3"         , 0.0f   );
	gui3DDepthPresets[3]   = props.getFloat ( "gui_3D_depth_4"         , 0.0f   );
	guiSquishPresets[0]    = props.getFloat ( "gui_size_1"             , 0.6f   );
	guiSquishPresets[1]    = props.getFloat ( "gui_size_2"             , 0.5f   );
	guiSquishPresets[2]    = props.getFloat ( "gui_size_3"             , 0.9f   );
	guiSquishPresets[3]    = props.getFloat ( "gui_size_4"             , 1.0f   );
	guiHotkeys[0]          = props.getInt   ( "gui_key_swap"           , 0      );
	guiHotkeys[1]          = props.getInt   ( "gui_key_default"        , 0      );
	guiHotkeys[2]          = props.getInt   ( "gui_key_small"          , 0      );
	guiHotkeys[3]          = props.getInt   ( "gui_key_large"          , 0      );
	guiHotkeys[4]          = props.getInt   ( "gui_key_full"           , 0      );
	VRBoostResetHotkey     = props.getInt   ( "VRBoost_key_reset"      , 0      );
	WorldFOV               = props.getFloat ( "WorldFOV"               , 95.0f  );
	PlayerFOV              = props.getFloat ( "PlayerFOV"              , 125.0f );
	FarPlaneFOV            = props.getFloat ( "FarPlaneFOV"            , 95.0f  );
	CameraDistance         = props.getFloat ( "CameraDistance"         , 0.0f   );
	CameraZoom             = props.getFloat ( "CameraZoom"             , 0.0f   );
	CameraHorizonAdjustment= props.getFloat ( "CameraHorizonAdjustment", 0.0f   );
	ConstantValue1         = props.getFloat ( "ConstantValue1"         , 0.0f   );
	ConstantValue2         = props.getFloat ( "ConstantValue2"         , 0.0f   );
	ConstantValue3         = props.getFloat ( "ConstantValue3"         , 0.0f   );
	shaderRulePath         = props.getString( "shaderModRules"                  );
	VRboostPath            = props.getString( "VRboostRules"                    );
	rollEnabled            = props.getBool  ( "rollEnabled"            , false  );
	worldScaleFactor       = props.getFloat ( "worldScaleFactor"       , 1.0f   );
	SteamAppId             = props.getString( "SteamAppId"                      );
	CommandLineArguments   = props.getString( "CommandLineArguments"            );

	if( !shaderRulePath.isEmpty() ){
		shaderRulePath.prepend( vireioDir + "shader_rules/" );
	}

	if( !VRboostPath.isEmpty() ){
		VRboostPath.prepend( vireioDir + "VRboost_rules/" );
	}

	//SB: This will need to be changed back when the memory modification stuff is updated, but for now
	//I am disabling the restore of the camera translation as it is causing confusion for a lot of people when
	//the game starts and they are detached from their avatar, or worse the scene doesn't render at all
	CameraTranslateX = 0.0f;//gameProfile.attribute("CameraTranslateX").as_float(0.0f);
	CameraTranslateY = 0.0f;//gameProfile.attribute("CameraTranslateY").as_float(0.0f);
	CameraTranslateZ = 0.0f;//gameProfile.attribute("CameraTranslateZ").as_float(0.0f);

	return true;
	// copy game dlls
	//bool copyDlls = gameProfile.attribute("copyDlls").as_bool(false);
	//if (copyDlls)
	//{
	//	// d3d9.dll
	//	char sourcePath[512];
	//	GetPath(sourcePath, "bin\\d3d9.dll");
	//	char destPath[512];
	//	GetTargetPath(destPath, "d3d9.dll");

	//	std::stringstream sstm;
	//	sstm << "copy " << sourcePath << " " << destPath;
	//	system(sstm.str().c_str());
	//	OutputDebugStringA(sstm.str().c_str());

	//	// libfreespace.dll
	//	GetPath(sourcePath, "bin\\libfreespace.dll");
	//	GetTargetPath(destPath, "libfreespace.dll");

	//	std::stringstream sstm1;
	//	sstm1 << "copy " << sourcePath << " " << destPath;
	//	system(sstm1.str().c_str());
	//	OutputDebugStringA(sstm1.str().c_str());

	//}
}


bool cGameProfile::save( ){
	cPropsFile props;
	props.getString( "game_exe"               , exeName                 );
	props.getInt   ( "game_type"              , game_type               );
	props.getInt   ( "minVRboostShaderCount"  , VRboostMinShaderCount   );
	props.getInt   ( "maxVRboostShaderCount"  , VRboostMaxShaderCount   );
	props.getFloat ( "convergence"            , convergence             );
	props.getBool  ( "swap_eyes"              , swap_eyes               );
	props.getFloat ( "yaw_multiplier"         , yaw_multiplier          );
	props.getFloat ( "pitch_multiplier"       , pitch_multiplier        );
	props.getFloat ( "roll_multiplier"        , roll_multiplier         );
	props.getFloat ( "position_multiplier"    , position_multiplier     );
	props.getFloat ( "distortion_scale"       , DistortionScale         );
	props.setFloat ( "y_offset"               , YOffset                 );
	props.setFloat ( "ipd_offset"             , IPDOffset               );
	props.setInt   ( "hud_3D_depth_mode"      , hud3DDepthMode          );
	props.setFloat ( "hud_3D_depth_1"         , hud3DDepthPresets[0]    );
	props.setFloat ( "hud_3D_depth_2"         , hud3DDepthPresets[1]    );
	props.setFloat ( "hud_3D_depth_3"         , hud3DDepthPresets[2]    );
	props.setFloat ( "hud_3D_depth_4"         , hud3DDepthPresets[3]    );
	props.setFloat ( "hud_distance_1"         , hudDistancePresets[0]   );
	props.setFloat ( "hud_distance_2"         , hudDistancePresets[1]   );
	props.setFloat ( "hud_distance_3"         , hudDistancePresets[2]   );
	props.setFloat ( "hud_distance_4"         , hudDistancePresets[3]   );
	props.setInt   ( "hud_key_swap"           , hudHotkeys[0]           );
	props.setInt   ( "hud_key_default"        , hudHotkeys[1]           );
	props.setInt   ( "hud_key_small"          , hudHotkeys[2]           );
	props.setInt   ( "hud_key_large"          , hudHotkeys[3]           );
	props.setInt   ( "hud_key_full"           , hudHotkeys[4]           );
	props.setInt   ( "gui_3D_depth_mode"      , gui3DDepthMode          );
	props.setFloat ( "gui_3D_depth_1"         , gui3DDepthPresets[0]    );
	props.setFloat ( "gui_3D_depth_2"         , gui3DDepthPresets[1]    );
	props.setFloat ( "gui_3D_depth_3"         , gui3DDepthPresets[2]    );
	props.setFloat ( "gui_3D_depth_4"         , gui3DDepthPresets[3]    );
	props.setFloat ( "gui_size_1"             , guiSquishPresets[0]     );
	props.setFloat ( "gui_size_2"             , guiSquishPresets[1]     );
	props.setFloat ( "gui_size_3"             , guiSquishPresets[2]     );
	props.setFloat ( "gui_size_4"             , guiSquishPresets[3]     );
	props.setInt   ( "gui_key_swap"           , guiHotkeys[0]           );
	props.setInt   ( "gui_key_default"        , guiHotkeys[1]           );
	props.setInt   ( "gui_key_small"          , guiHotkeys[2]           );
	props.setInt   ( "gui_key_large"          , guiHotkeys[3]           );
	props.setInt   ( "gui_key_full"           , guiHotkeys[4]           );
	props.setInt   ( "VRBoost_key_reset"      , VRBoostResetHotkey      );
	props.setFloat ( "WorldFOV"               , WorldFOV                );
	props.setFloat ( "PlayerFOV"              , PlayerFOV               );
	props.setFloat ( "FarPlaneFOV"            , FarPlaneFOV             );
	props.setFloat ( "CameraDistance"         , CameraDistance          );
	props.setFloat ( "CameraZoom"             , CameraZoom              );
	props.setFloat ( "CameraHorizonAdjustment", CameraHorizonAdjustment );
	props.setFloat ( "ConstantValue1"         , ConstantValue1          );
	props.setFloat ( "ConstantValue2"         , ConstantValue2          );
	props.setFloat ( "ConstantValue3"         , ConstantValue3          );
	props.setString( "shaderModRules"         , QFileInfo(shaderRulePath).fileName() );
	props.setString( "VRboostRules"           , QFileInfo(VRboostPath).fileName() );
	props.setBool  ( "rollEnabled"            , rollEnabled             );
	props.setFloat ( "worldScaleFactor"       , worldScaleFactor        );

	return props.save( vireioDir+"profiles/" + profileName +".ini" );
}



cGameProfile* cGameProfile::findByExe( QString exe ){
	for( cGameProfile* p : all() ){
		if( QString::compare( p->exeName , exe , Qt::CaseInsensitive )==0 ){
			return p;
		}
	}
	return 0;
}



cGameProfile* cGameProfile::findByName( QString name ){
	for( cGameProfile* p : all() ){
		if( p->profileName == name ){
			return p;
		}
	}
	return 0;
}



QList<cGameProfile*>& cGameProfile::all( ){
	static QList<cGameProfile*> list;
	return list;
}


void cGameProfile::loadAll( ){
	for( QFileInfo& info : QDir( vireioDir+"profiles" ).entryInfoList( QDir::Files ) ){
		cGameProfile* profile = new cGameProfile;
		if( !profile->load( info.filePath() ) ){
			delete profile;
		}
	}

	printf( "vireio: loaded %d game profiles\n" , all().count() );
}
