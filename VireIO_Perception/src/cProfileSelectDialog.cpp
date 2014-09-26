#include "cProfileSelectDialog.h"
#include <VireIO.h>


cProfileSelectDialog::cProfileSelectDialog( QWidget* parent ) : QDialog(parent) {
	ui.setupUi(this);

	for( QString& s : config.getAvailableProfiles() ){
		ui.profile->addItem( s );
	}
}



void cProfileSelectDialog::on_ok_clicked(){
	selectedProfileName = ui.profile->currentText();

	if( selectedProfileName.isEmpty() ){
		reject();
	}else{
		accept();
	}
}

