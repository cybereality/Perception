#include "cMainWindow.h"
#include "ui_cMainWindow.h"
#include <qmenu.h>
#include <qdir.h>
#include <qfileiconprovider.h>
#include <qmessagebox.h>

#include <cStereoMode.h>
#include <cGameProfile.h>
#include <cGame.h>
#include <cSettings.h>


static cGame* GameForItem( QTreeWidgetItem* i ){
	return (cGame*) i->data( 0 , Qt::UserRole ).value<void*>();
}


cMainWindow::cMainWindow( ){
	ui.setupUi(this);

	ui.games->header()->setSectionResizeMode( 0 , QHeaderView::ResizeToContents );

	cStereoMode ::loadAll();
	cGameProfile::loadAll();
	cGame       ::loadAll();

	ui.info->setText(
		QString("Loaded %1 profiles.\nLoaded %2 games.\nLoaded %3 stereo modes.").arg( cGameProfile::all().count() ).arg( cGame::all().count() ).arg( cStereoMode::all().count() )
	);

	for( cGame* g : cGame::all() ){
		AddGame( g );
	}

	for( cStereoMode* m : cStereoMode::all() ){
		ui.stereoMode->addItem( m->name );
	}

	ui.trackerMode->addItem( "No Tracking"           , 0  );
	ui.trackerMode->addItem( "Hillcrest Labs"        , 10 );
	ui.trackerMode->addItem( "FreeTrack"             , 20 );
	ui.trackerMode->addItem( "Shared Memory Tracker" , 30 );
	ui.trackerMode->addItem( "OculusTrack"           , 40 );

	LoadSettings();
}


cMainWindow::~cMainWindow( ){

}

void cMainWindow::AddGame( cGame* g ){
	QTreeWidgetItem* item = new QTreeWidgetItem;
	item->setText( 0 , g->profile->profileName );
	item->setText( 1 , g->exe_path           );
	item->setIcon( 0 , QFileIconProvider().icon( QFileInfo( g->exe_path ) ) );
	item->setData( 0 , Qt::UserRole , QVariant::fromValue<void*>( g ) );
	ui.games->addTopLevelItem( item );
}


void cMainWindow::LoadSettings(){
	cSettings s;
	s.load();

	ui.stereoMode      ->setCurrentIndex( ui.stereoMode ->findText(s.stereoMode)  );
	ui.trackerMode     ->setCurrentIndex( ui.trackerMode->findData(s.trackerMode) );

	ui.streamingEnable ->setChecked     ( s.streamingEnable   );
	ui.streamingAddress->setText        ( s.streamingAddress  );
	ui.streamingPort   ->setValue       ( s.streamingPort     );
	ui.streamingCodec  ->setCurrentText ( s.streamingCodec    );
	ui.streamingBitrate->setValue       ( s.streamingBitrate  );

}


void cMainWindow::on_saveSettings_clicked(){
	cSettings s;
	s.stereoMode        = ui.stereoMode       ->currentText();
	s.trackerMode       = ui.trackerMode      ->currentData().toInt();

	s.streamingEnable   = ui.streamingEnable  ->isChecked();
	s.streamingAddress  = ui.streamingAddress ->text();
	s.streamingPort     = ui.streamingPort    ->value();
	s.streamingCodec    = ui.streamingCodec   ->currentText();
	s.streamingBitrate  = ui.streamingBitrate ->value();
	s.save();
}

#include <qfiledialog.h>
#include "cProfileSelectDialog.h"


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
				cGame* g = new cGame;
				g->exe_path = d.selectedFiles()[0];
				g->profile  = pd.selectedProfile;
				g->save();
				AddGame( g );
			}
		}
	}

	if( ret == del ){
		for( QTreeWidgetItem* i : ui.games->selectedItems() ){
			cGame* g = GameForItem(i);
			if( QFile( g->propFile).remove() ){
				delete g;
				delete i;
			}
		}
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

		for( cGame* g : cGame::all() ){
			if( g->exe_path == file_list[0] ){
				add = false;
				break;
			}
		}

		if( add ){
			cGameProfile* profile = cGameProfile::findByExe( name_list[0] );

			if( profile ){
				cGame* g = new cGame;
				g->exe_path = file_list[0];
				g->profile  = profile;
				g->save();
				AddGame( g );
			}
		}

		name_list.removeFirst();
		file_list.removeFirst();
	}

}

void cMainWindow::on_games_itemDoubleClicked( QTreeWidgetItem *item , int ){
	cGame* g = GameForItem(item);

	QStringList dlls;
	dlls += vireioDir+"bin/VireIO_Hijack.dll";
	dlls += vireioDir+"bin/VireIO_Proxy.dll";


	QString ret = HijackLaunchProcess( g->exe_path , dlls , ui.pause->isChecked() );
	if( !ret.isEmpty() ){
		QMessageBox::critical( 0 , "Launch error" , ret );
	}
}
