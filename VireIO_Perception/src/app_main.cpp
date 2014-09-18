#define UNICODE
#define _UNICODE

#include <qapplication.h>
#include "cMainWindow.h"


extern "C" int main(int argc, char **argv){
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName( "VireIO" );
	QCoreApplication::setApplicationName ( "Perception" );

	cMainWindow w;
	w.show();

	return a.exec();
}
