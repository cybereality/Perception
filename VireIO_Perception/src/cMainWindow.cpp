#include "cMainWindow.h"
#include "ui_cMainWindow.h"
#include <qmenu.h>
#include <qdir.h>
#include <qfileiconprovider.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "cProfileSelectDialog.h"
#include <VireIO.h>
#include <cConfig.h>
#include <cPropsFile.h>


cMainWindow::cMainWindow( ){
	ui.setupUi(this);

	ui.games->header()->setSectionResizeMode( 0 , QHeaderView::ResizeToContents );

	for( QString& s : config.getAvailableDevices() ){
		ui.stereoMode->addItem( s );
	}

	ui.trackerMode->addItem( "No Tracking"           , 0  );
	ui.trackerMode->addItem( "Hillcrest Labs"        , 10 );
	ui.trackerMode->addItem( "FreeTrack"             , 20 );
	ui.trackerMode->addItem( "Shared Memory Tracker" , 30 );
	ui.trackerMode->addItem( "OculusTrack"           , 40 );



	ui.stereoMode      ->setCurrentIndex( ui.stereoMode ->findText(config.stereoDevice)  );
	ui.trackerMode     ->setCurrentIndex( ui.trackerMode->findData(config.trackerMode) );

	ui.logToConsole    ->setChecked     ( config.logToConsole      );
	ui.logToFile       ->setChecked     ( config.logToFile         );
	ui.logHijack       ->setChecked     ( config.logHijack         );
	
	ui.pauseOnLaunch   ->setChecked     ( config.pauseOnLaunch     );

	ui.streamingEnable ->setChecked     ( config.streamingEnable   );
	ui.streamingAddress->setText        ( config.streamingAddress  );
	ui.streamingPort   ->setValue       ( config.streamingPort     );
	ui.streamingCodec  ->setCurrentText ( config.streamingCodec    );
	ui.streamingBitrate->setValue       ( config.streamingBitrate  );

	LoadGames();
}


cMainWindow::~cMainWindow( ){

}
/*
void cMainWindow::AddGame( cGame* g ){
	QTreeWidgetItem* item = new QTreeWidgetItem;
	item->setText( 0 , g->profile->profileName );
	item->setText( 1 , g->exe           );
	item->setIcon( 0 , QFileIconProvider().icon( QFileInfo( g->exe ) ) );
	item->setData( 0 , Qt::UserRole , QVariant::fromValue<void*>( g ) );
	ui.games->addTopLevelItem( item );
}
*/



void cMainWindow::on_saveSettings_clicked(){
	config.stereoDevice      = ui.stereoMode       ->currentText();
	config.trackerMode       = ui.trackerMode      ->currentData().toInt();
	config.logToConsole      = ui.logToConsole     ->isChecked();
	config.logToFile         = ui.logToFile        ->isChecked();
	config.logHijack         = ui.logHijack        ->isChecked();
	config.pauseOnLaunch     = ui.pauseOnLaunch    ->isChecked();
	config.streamingEnable   = ui.streamingEnable  ->isChecked();
	config.streamingAddress  = ui.streamingAddress ->text();
	config.streamingPort     = ui.streamingPort    ->value();
	config.streamingCodec    = ui.streamingCodec   ->currentText();
	config.streamingBitrate  = ui.streamingBitrate ->value();

	config.save( config.getMainConfigFile() , config.SAVE_GENERAL );
}


void cMainWindow::on_games_customContextMenuRequested( const QPoint& pos ){
	QMenu m;
	QAction* scan = m.addAction( "Scan new games" );
	QAction* add  = m.addAction( "Add new game"   );
	QAction* del  = m.addAction( "Delete"         );

	del->setVisible( !ui.games->selectedItems().isEmpty() );

	QAction* ret = m.exec( ui.games->viewport()->mapToGlobal(pos) );

	if( ret == scan ){
		ScanGames();
		return;
	}

	if( ret == add ){
		QFileDialog d(this);
		d.setNameFilter( "Executables (*.exe)" );
		d.setAcceptMode( QFileDialog::AcceptOpen );
		d.setFileMode  ( QFileDialog::ExistingFile );
		if( d.exec() ){
			cProfileSelectDialog pd(this);
			if( pd.exec() ){
				cConfig cfg;
				cfg.exePath     = d.selectedFiles()[0];
				cfg.profileName = pd.selectedProfileName;
				cfg.save( config.getGameConfigFile(cfg.exePath) , config.SAVE_GAME );
				LoadGames( );
			}
		}
	}

	if( ret == del ){
		for( QTreeWidgetItem* i : ui.games->selectedItems() ){
			QFile(i->text(2)).remove();
		}
		LoadGames( );
	}
}


void cMainWindow::LoadGames(){
	ui.games->clear();

	for( QFileInfo& info : QDir(config.vireioDir+"games").entryInfoList(QDir::Files) ){
		cConfig cfg;
		if( cfg.load( info.absoluteFilePath() ) ){
			QTreeWidgetItem* item = new QTreeWidgetItem;
			item->setText( 0 , cfg.profileName         );
			item->setText( 1 , cfg.exePath             );
			item->setText( 2 , info.absoluteFilePath() );
			item->setIcon( 0 , QFileIconProvider().icon( QFileInfo( cfg.exePath ) ) );
			ui.games->addTopLevelItem( item );
		}
	}
}


void cMainWindow::ScanGames(){
	QStringList scan_queue;
	QStringList file_list;

	scan_queue += QSettings( "HKEY_CURRENT_USER\\Software\\Valve\\Steam" , QSettings::NativeFormat ).value("SteamPath").toString() + "/SteamApps";

	while( !scan_queue.isEmpty() ){
		QString dir = scan_queue.takeFirst();

		for( QFileInfo& info : QDir(dir).entryInfoList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot ) ){

			if( info.isDir() ){
				scan_queue += info.absoluteFilePath() + "/";
			}else{
				if( info.fileName().toLower().endsWith(".exe") ){
					file_list += info.filePath();
				}
			}
		}
	}


	for( QString& file : file_list ){
		QString profile = config.findProfileFileForExe( file );
		if( !profile.isEmpty() ){
			cConfig cfg;
			cfg.exePath     = file;
			cfg.profileName = profile;
			cfg.save( config.getGameConfigFile(file) , config.SAVE_GAME );
		}
	}

	LoadGames();
}


void cMainWindow::on_games_itemDoubleClicked( QTreeWidgetItem *item , int ){
	cConfig game = config;
	if( !game.load( item->text(2) ) ){
		return;
	}

	if( !game.loadProfile() ){
		return;
	}

	QStringList env;
	if( !game.SteamAppId.isEmpty() ){
		env += "SteamAppId=" + game.SteamAppId;
	}

	QString ret = HijackLaunchProcess( game.exePath , game.CommandLineArguments , env );

	if( !ret.isEmpty() ){
		QMessageBox::critical( 0 , "Launch error" , ret );
	}
}
