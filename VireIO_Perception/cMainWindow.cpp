#include "cMainWindow.h"
#include "ui_cMainWindow.h"
#include <qmenu.h>
#include <qdir.h>
#include <qdebug.h>

#include <cStereoMode.h>
#include "cGameProfile.h"
#include "cGame.h"



static void InitCombo( QSettings* set , QComboBox* cb ){
	bool ok;
	int value = set->value( cb->objectName() ).toInt(&ok);

	if( ok ){
		cb->setCurrentIndex( cb->findData(value) );
	}else{
		cb->setCurrentIndex( -1 );
	}

	QObject::connect( cb , (void(QComboBox::*)(int))&QComboBox::currentIndexChanged , [=]( int newIndex ){
		set->setValue( cb->objectName() , cb->itemData(newIndex) );
	});
}


cMainWindow::cMainWindow( ){
	ui.setupUi(this);

	ui.games->header()->setSectionResizeMode( 0 , QHeaderView::ResizeToContents );

	for( cStereoMode& m : GetStereoModes() ){
		ui.stereoMode->addItem( QString::fromStdString(m.name) , m.uid );
	}

	ui.trackerMode->addItem( "No Tracking"           , 0  );
	ui.trackerMode->addItem( "Hillcrest Labs"        , 10 );
	ui.trackerMode->addItem( "FreeTrack"             , 20 );
	ui.trackerMode->addItem( "Shared Memory Tracker" , 30 );
	ui.trackerMode->addItem( "OculusTrack"           , 40 );

	InitCombo( &settings , ui.stereoMode  );
	InitCombo( &settings , ui.trackerMode );

	cGameProfile::LoadProfiles();

	cGame::LoadGames();
	for( cGame* g : cGame::AllGames() ){
		g->init( ui.games );
	}

	ui.info->setText(
		QString("Loaded %1 profiles.\nLoaded %2 games.").arg( cGameProfile::AllProfiles().count() ).arg( cGame::AllGames().count() )
	);
}


cMainWindow::~cMainWindow( ){

}



void cMainWindow::on_games_customContextMenuRequested( const QPoint& pos ){
	QMenu m;
	QAction* scan = m.addAction( "Scan new games" );

	QAction* ret = m.exec( ui.games->viewport()->mapToGlobal(pos) );

	if( ret == scan ){

		ScanGames();

	}
}


void cMainWindow::ScanGames(){

	QStringList scan_queue;
	QStringList file_list;
	QStringList name_list;

	scan_queue += QSettings( "HKEY_CURRENT_USER\\Software\\Valve\\Steam" , QSettings::NativeFormat ).value("SteamPath").toString() + "/SteamApps";

	while( !scan_queue.isEmpty() ){
		QString dir = scan_queue.takeFirst();

		for( QFileInfo& info : QDir(dir).entryInfoList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot ) ){

			if( info.isDir() ){
				scan_queue += info.absoluteFilePath() + "/";
			}else{
				QString name = info.fileName().toLower();

				if( name.endsWith(".exe") || name.endsWith(".dll") ){
					file_list += info.filePath();
					name_list += name;
				}
			}
		}
	}


	while( !name_list.isEmpty() ){
		bool add = true;

		for( cGame* g : cGame::AllGames() ){
			if( g->exe_path == file_list[0] ){
				add = false;
				break;
			}
		}

		if( add ){
			cGameProfile* profile = cGameProfile::FindProfileByExe( name_list[0] );

			if( profile ){
				cGame* g = new cGame;
				g->exe_path = file_list[0];
				g->profile  = profile;
				g->init( ui.games );
				g->save();
			}
		}

		name_list.removeFirst();
		file_list.removeFirst();
	}

}

void cMainWindow::on_games_itemDoubleClicked( QTreeWidgetItem *item , int ){
	for( cGame* g : cGame::AllGames() ){
		if( g->item == item ){
			g->launch();
		}
	}
}
