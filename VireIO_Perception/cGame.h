#pragma once
#include <qstring.h>
#include <qlist.h>
#include <qtreewidget.h>
#include "cGameProfile.h"

class cGame{
public:
	QString          exe_path;
	cGameProfile*    profile;
	QTreeWidgetItem* item;

	cGame ( );
	~cGame( );

	void init  ( QTreeWidget* widget );
	void load  ( const QString& file );
	void save  ( );
	void launch( );


	static QList<cGame*>& AllGames();
	static void           LoadGames();
	static void           SaveGames();
};

