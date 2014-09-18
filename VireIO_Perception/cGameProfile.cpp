#include "cGameProfile.h"
#include "cPropsFile.h"
#include <qdom.h>
#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

static QList<cGameProfile*> g_profiles;


cGameProfile::cGameProfile(){
	g_profiles.append( this );
}


cGameProfile::~cGameProfile(){
	g_profiles.removeAll( this );
}


cGameProfile* cGameProfile::FindProfileByExe( const QString& exe ){
	for( cGameProfile* p : g_profiles ){
		if( p->exe == exe ){
			return p;
		}
	}
	return 0;
}



cGameProfile* cGameProfile::FindProfileByName( const QString& name ){
	for( cGameProfile* p : g_profiles ){
		if( p->name == name ){
			return p;
		}
	}
	return 0;
}



QList<cGameProfile*>& cGameProfile::AllProfiles( ){
	return g_profiles;
}


void cGameProfile::LoadProfiles( ){
	for( QFileInfo& info : QDir( "../profiles").entryInfoList( QDir::Files ) ){
		cPropsFile props;
		if( props.load( info.filePath() ) ){
			cGameProfile* profile = new cGameProfile;
			profile->name = info.baseName();
			profile->exe  = props["game_exe"];
		}
	}
}
