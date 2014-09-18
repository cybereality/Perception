#include "cConfig.h"
#include <stdio.h>
#include <qfileinfo.h>

bool cConfig::load( QString game_exe ){
	cGame*       game = 0;
	cStereoMode* mode = 0;


	if( !cSettings::load() ){
		printf( "vireio: unable to load settings" );
		return false;
	}

	for( cGame* g : cGame::all() ){
		if( game_exe == QFileInfo( g->exe_path ).absoluteFilePath() ){
			game = g;
			break;
		}
	}

	if( !game ){
		printf( "vireio: no game found for \"%s\"\n" , game_exe.toLocal8Bit().data() );
		return false;
	}

	mode = cStereoMode::find( stereoMode );
	if( !mode ){
		printf( "vireio: unknown stereo mode \"%s\"\n" , stereoMode.toLocal8Bit().data() );
		return false;
	}

	*static_cast<cGameProfile*>(this) = *game->profile;
	*static_cast<cStereoMode *>(this) = *mode;

	return true;
}
