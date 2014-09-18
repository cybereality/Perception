#include "loader.h"
#include <qfile.h>

IniMap LoadPropFile( const QString& path ){
	IniMap ret;

	QFile file( path );

	if( !file.open(QFile::ReadOnly) ){
		return ret;
	}

	while( !file.atEnd() ){
		QByteArray line = file.readLine();

		int index = line.indexOf('=');
		if( index >=0 ){
			QString param = QString::fromUtf8( line.left( index     ).trimmed() );
			QString value = QString::fromUtf8( line.mid ( index + 1 ).trimmed() );
			ret[param] = value;
		}
	}

	return ret;
}



void SavePropFile( const QString& path , const IniMap& map ){

	QFile file( path );

	if( !file.open(QFile::WriteOnly) ){
		return;
	}

	int align = 0;

	for( QString& key : map.keys() ){
		if( key.size() > align ){
			align = key.size();
		}
	}

	align++;

	for( QString& key : map.keys() ){
		file.write( (key + QString().fill(' ',align - key.size()) + "=" + map[key] + "\r\n").toUtf8() );
	}

	file.close();
}
