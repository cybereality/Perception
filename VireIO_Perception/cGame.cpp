#include "cGame.h"
#include "cGameProfile.h"
#include "cPropsFile.h"
#include "hijack.h"
#include <qfileinfo.h>
#include <qfileiconprovider.h>
#include <qsettings.h>
#include <qdir.h>
#include <qapplication.h>
#include <qmessagebox.h>

static QList<cGame*> g_games;


cGame::cGame(  ){
	profile = 0;
	item    = 0;
	g_games.append( this );
}


cGame::~cGame( ){
	g_games.removeAll( this );
}


void cGame::init( QTreeWidget* widget ){
	item = new QTreeWidgetItem;
	item->setText( 0 , profile->name );
	item->setText( 1 , exe_path           );
	item->setIcon( 0 , QFileIconProvider().icon( QFileInfo( exe_path ) ) );
	widget->addTopLevelItem( item );
}



QList<cGame*>& cGame::AllGames( ){
	return g_games;
}


void cGame::load( const QString& file ){
	cPropsFile props;
	if( props.load( file ) ){
		profile  = cGameProfile::FindProfileByName( props["profile"] );
		exe_path = props["exe"];
	}
}


void cGame::save( ){
	cPropsFile props;
	props["profile"] = profile->name;
	props["exe"    ] = exe_path;

	QString uid = exe_path;
	uid.replace( ":"  , "_" );
	uid.replace( "\\" , "_" );
	uid.replace( "/"  , "_" );

	props.save( "../games/" + uid + ".ini" );
}



void cGame::launch( ){
	QStringList dlls;
	dlls += QApplication::applicationDirPath() + "/VireIO_Proxy.dll";
	dlls += QApplication::applicationDirPath() + "/VireIO_Hijack.dll";

	QString ret = HijackLaunchProcess( exe_path , dlls );
	if( !ret.isEmpty() ){
		QMessageBox::critical( 0 , "Launch error" , ret );
	}
}



void cGame::LoadGames(){
	for( QFileInfo& info : QDir( "../games" ).entryInfoList( QDir::Files ) ){
		cGame* game = new cGame;
		game->load( info.filePath() );

		if( !game->profile ){
			delete game;
		}
	}
}
