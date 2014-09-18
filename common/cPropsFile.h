#pragma once
#include <qbytearray.h>
#include <qmap.h>

class cPropsFile : public QMap<QString,QString> {
public:
	bool load( const QString& path );
	bool save( const QString& path );
};
