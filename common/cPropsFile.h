#pragma once
#include "VireIO.h"

class cPropsFile{
private:
	QList< QPair<QString , QString> > items;

public:
	bool load( QString path );
	bool save( QString path );

	bool    getBool  ( QString name , bool    default_value = false );
	float   getFloat ( QString name , float   default_value = 0.0f  );
	int     getInt   ( QString name , int     default_value = 0     );
	QString getString( QString name , QString default_value = ""    );

	void    setBool  ( QString name , bool    value );
	void    setFloat ( QString name , float   value );
	void    setInt   ( QString name , int     value );
	void    setString( QString name , QString value );

	//static QList< QPair<QString , cPropsFile > >  loadFiles( QString directory );
};
