#include "cGame.h"
#include "cGameProfile.h"
#include "cPropsFile.h"
#include <qdir.h>

static QList<cGame*> g_games;


cGame::cGame(  ){
	profile = 0;
	g_games.append( this );
}


cGame::~cGame( ){
	g_games.removeAll( this );
}


QList<cGame*>& cGame::all( ){
	return g_games;
}


void cGame::save( ){
	cPropsFile props;
	props.setString( "profile" , profile->profileName );
	props.setString( "exe"     , exe_path );

	if( propFile.isEmpty() ){
		propFile = exe_path;
		propFile.replace( ':'  , '_' );
		propFile.replace( '\\' , '_' );
		propFile.replace( '/'  , '_' );
		propFile = vireioDir+"games/" + propFile + ".ini";
	}

	props.save( propFile );
}


void cGame::loadAll(){
	for( QFileInfo& info : QDir( vireioDir+"games" ).entryInfoList( QDir::Files ) ){
		cPropsFile props;
		if( !props.load( info.filePath() ) ){
			continue;
		}

		cGame* game = new cGame;
		game->propFile = info.filePath();
		game->profile  = cGameProfile::findByName( props.getString("profile") );
		game->exe_path = props.getString("exe");
	
		if( !game->profile ){
			delete game;
		}
	}

	printf( "vireio: loaded %d games\n" , all().count() );
}


cGame* cGame::findByPath( QString path ){
	for( cGame* g : all() ){
		if( QString::compare( path , QFileInfo( g->exe_path ).absoluteFilePath() , Qt::CaseInsensitive )==0 ){
			return g;
		}
	}
	return 0;
}