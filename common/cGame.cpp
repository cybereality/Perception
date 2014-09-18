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


void cGame::load( QString file ){
	cPropsFile props;
	if( props.load( file ) ){
		profile  = cGameProfile::findByName( props.getString("profile") );
		exe_path = props.getString("exe");
	}
}


void cGame::save( ){
	cPropsFile props;
	props.setString( "profile" , profile->profileName );
	props.setString( "exe"     , exe_path );

	QString uid = exe_path;
	uid.replace( ':'  , '_' );
	uid.replace( '\\' , '_' );
	uid.replace( '/'  , '_' );

	props.save( vireioDir+"games/" + uid + ".ini" );
}


void cGame::loadAll(){
	for( QFileInfo& info : QDir( vireioDir+"games" ).entryInfoList( QDir::Files ) ){
		cGame* game = new cGame;
		game->load( info.filePath() );

		if( !game->profile ){
			delete game;
		}
	}

	printf( "vireio: loaded %d games\n" , all().count() );
}
