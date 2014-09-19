#pragma once
#include "VireIO.h"
#include "cGameProfile.h"

class cGame{
public:
	QString       propFile;
	QString       exe_path;
	cGameProfile* profile;

	cGame ( );
	~cGame( );

	void save( );


	static QList<cGame*>& all();
	static void           loadAll();
};

