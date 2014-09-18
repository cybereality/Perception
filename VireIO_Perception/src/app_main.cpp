#include <qapplication.h>
#include "cMainWindow.h"
#include <qdebug.h>

QString vireioDir;

int main(int argc, char **argv){
	QApplication a(argc, argv);

	vireioDir = a.applicationDirPath() + "/../";

	QCoreApplication::setOrganizationName( "VireIO" );
	QCoreApplication::setApplicationName ( "Perception" );

	cMainWindow w;
	w.show();

	return a.exec();
}
