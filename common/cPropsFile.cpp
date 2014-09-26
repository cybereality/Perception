#include "cPropsFile.h"
#include <qfile.h>


cPropsFile::cPropsFile( ){
}


cPropsFile::~cPropsFile( ){
	for( Item& i : items ){
		if( !i.used ){
			printf( "unused property \"%s\"\n" , i.id.toLocal8Bit().data() );
		}
	}
}


bool cPropsFile::load( QString path ){
	items.clear();

	QFile file( path );

	if( !file.open(QFile::ReadOnly) ){
		return false;
	}

	while( !file.atEnd() ){
		QByteArray line = file.readLine();

		int index = line.indexOf('=');
		if( index >=0 ){
			Item i;
			i.id    = QString::fromUtf8( line.left( index     ) ).trimmed();
			i.value = QString::fromUtf8( line.mid ( index + 1 ) ).trimmed();
			i.used  = false;
			items += i;
		}
	}

	return true;
}



bool cPropsFile::save( QString path ){

	QFile file( path );

	if( !file.open(QFile::WriteOnly) ){
		return false;
	}

	int align = 0;
	for( Item& i : items ){
		if( i.id.size() > align ){
			align = i.id.size();
		}
	}
	align++;


	for( Item& i : items ){
		file.write( (i.id + QString().fill(' ',align - i.id.size()) + "=" + i.value + "\r\n").toUtf8() );
	}

	file.close();

	return true;
}



QString cPropsFile::getString( const QString& name , bool* ok ){
	for( Item& i : items ){
		if( i.id == name ){
			*ok = true;
			i.used = true;
			return i.value;
		}
	}
	*ok = false;
	return "";
}




void cPropsFile::setString( const QString& name , const QString& value ){
	for( Item& i : items ){
		if( i.id == name ){
			i.value = value;
			return;
		}
	}

	Item i;
	i.id    = name;
	i.value = value;
	i.used  = true;
	items += i;
}



void cPropsFile::get( bool* value , const QString& name ){
	bool ok;
	QString v = getString(name,&ok).toLower();

	if( !ok ){
		return;
	}

	if( v=="1" || v=="true"  || v=="t" || v=="y" || v=="yes" ){
		*value = true;
		return;
	}

	if( v=="0" || v=="false" || v=="f" || v=="n" || v=="no" ){
		*value = false;
		return;
	}

	printf("unknown bool value: %s\n" , v.toLocal8Bit().data() );
}


void cPropsFile::get( int* value , const QString& name ){
	bool ok;
	int n = getString(name,&ok).toInt( &ok );
	if( ok ){
		*value = n;
	}
}


void cPropsFile::get( float* value , const QString& name ){
	bool ok;
	float n = getString(name,&ok).toFloat( &ok );
	if( ok ){
		*value = n;
	}
}


void cPropsFile::get( QString* value , const QString& name ){
	bool ok;
	QString n = getString(name,&ok);
	if( ok ){
		*value = n;
	}
}





void cPropsFile::set( bool* value , const QString& name ){
	setString( name , (*value) ? "true" : "false" );
}


void cPropsFile::set( int* value , const QString& name ){
	setString( name , QString::number(*value) );
}


void cPropsFile::set( float* value , const QString& name ){
	setString( name , QString::number(*value) );
}


void cPropsFile::set( QString* value , const QString& name ){
	setString( name , *value );
}
