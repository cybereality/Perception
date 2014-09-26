#pragma once
#include "VireIO.h"

class cPropsFile{
public:
	cPropsFile();
	~cPropsFile();

	bool load( QString path );
	bool save( QString path );

	QString getString( const QString& name , bool* ok );
	void    setString( const QString& name , const QString& value );

	void get( bool*           , const QString& );
	void get( int*            , const QString& );
	void get( float*          , const QString& );
	void get( QString*        , const QString& );

	void set( bool*          , const QString& );
	void set( int*           , const QString& );
	void set( float*         , const QString& );
	void set( QString*       , const QString& );

private:
	struct Item{
		QString id;
		QString value;
		bool    used;
	};

	QList<Item> items;
};
