#pragma once
#include <qstring.h>
#include <qlist.h>

class cGameProfile{
public:
	QString name;
	QString exe;

	cGameProfile();
	~cGameProfile();

	static cGameProfile*         FindProfileByExe ( const QString& exe_name );
	static cGameProfile*         FindProfileByName( const QString& exe_name );
	static QList<cGameProfile*>& AllProfiles ( );
	static void                  LoadProfiles( );
};

