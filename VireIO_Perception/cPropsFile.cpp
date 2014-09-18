#include "cPropsFile.h"
#include <qfile.h>

bool cPropsFile::load( const QString& path ){
	this->clear();

	QFile file( path );

	if( !file.open(QFile::ReadOnly) ){
		return false;
	}

	while( !file.atEnd() ){
		QByteArray line = file.readLine();

		int index = line.indexOf('=');
		if( index >=0 ){
			QString param = QString::fromUtf8( line.left( index     ).trimmed() );
			QString value = QString::fromUtf8( line.mid ( index + 1 ).trimmed() );
			(*this)[param] = value;
		}
	}

	return true;
}



bool cPropsFile::save( const QString& path ){

	QFile file( path );

	if( !file.open(QFile::WriteOnly) ){
		return false;
	}

	int align = 0;

	for( QString& key : keys() ){
		if( key.size() > align ){
			align = key.size();
		}
	}

	align++;

	QList<QString> sorted_keys = keys();
	std::sort( sorted_keys.begin() , sorted_keys.end() );

	for( QString& key : sorted_keys ){
		file.write( (key + QString().fill(' ',align - key.size()) + "=" + (*this)[key] + "\r\n").toUtf8() );
	}

	file.close();

	return true;
}
