#include <qapplication.h>
#include "cMainWindow.h"
#include <VireIO.h>

int main(int argc, char **argv){
	QApplication a(argc, argv);

	config.vireioDir = a.applicationDirPath() + "/../";

	config.load( config.getMainConfigFile() );
	
	QCoreApplication::setOrganizationName( "VireIO" );
	QCoreApplication::setApplicationName ( "Perception" );

	cMainWindow w;
	w.show();

	return a.exec();
}
