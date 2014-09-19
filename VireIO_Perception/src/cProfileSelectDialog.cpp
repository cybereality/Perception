#include "cProfileSelectDialog.h"
#include <cGameProfile.h>


cProfileSelectDialog::cProfileSelectDialog( QWidget* parent ) : QDialog(parent) {
	ui.setupUi(this);
	selectedProfile = 0;

	for( cGameProfile* g : cGameProfile::all() ){
		ui.profile->addItem( g->profileName );
	}
}



void cProfileSelectDialog::on_ok_clicked(){
	int i = ui.profile->currentIndex();
	if( i<0 ){
		reject();
		return;
	}

	selectedProfile = cGameProfile::all()[i];
	accept();
}

