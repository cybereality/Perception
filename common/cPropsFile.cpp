#include "cPropsFile.h"
#include <qfile.h>

typedef  QPair<QString , QString>  item_type;


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
			item_type item;
			item.first  = QString::fromUtf8( line.left( index     ).trimmed() );
			item.second = QString::fromUtf8( line.mid ( index + 1 ).trimmed() );
			items += item;
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
	for( item_type& i : items ){
		if( i.first.size() > align ){
			align = i.first.size();
		}
	}
	align++;


	for( item_type& i : items ){
		file.write( (i.first + QString().fill(' ',align - i.first.size()) + "=" + i.second + "\r\n").toUtf8() );
	}

	file.close();

	return true;
}



bool cPropsFile::getBool( QString name , bool default_value ){
	QString v = getString(name).toLower();

	if( v=="1" || v=="true"  || v=="t" || v=="y" || v=="yes" ){
		return true;
	}

	if( v=="0" || v=="false" || v=="f" || v=="n" || v=="no" ){
		return false;
	}

	return default_value;
}


float cPropsFile::getFloat( QString name , float default_value ){
	bool  ok;
	float r = getString(name).toFloat( &ok );
	if( ok ){
		return r;
	}
	return default_value;
}


int cPropsFile::getInt( QString name , int default_value ){
	bool  ok;
	int r = getString(name).toInt( &ok );
	if( ok ){
		return r;
	}
	return default_value;

}


QString cPropsFile::getString( QString name , QString default_value ){
	for( item_type& i : items ){
		if( i.first == name ){
			return i.second;
		}
	}
	return default_value;
}



void cPropsFile::setBool( QString name , bool value ){
	setString( name , value ? "true" : "false" );
}


void cPropsFile::setFloat( QString name , float value ){
	setString( name , QString::number(value) );
}


void cPropsFile::setInt( QString name , int value ){
	setString( name , QString::number(value) );
}


void cPropsFile::setString( QString name , QString value ){
	for( item_type& i : items ){
		if( i.first == name ){
			i.second = value;
			return;
		}
	}

	items += item_type(name,value);
}
