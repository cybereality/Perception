#pragma once
#include "VireIO.h"
#include "cGameProfile.h"

class cGame{
public:
	QString       exe_path;
	cGameProfile* profile;

	cGame ( );
	~cGame( );

	void    load  ( QString file );
	void    save  ( );

	static QList<cGame*>& all();
	static void           loadAll();
};

