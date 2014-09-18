#pragma once
#include <qbytearray.h>
#include <qmap.h>

typedef QMap<QString,QString> IniMap;

IniMap LoadPropFile( const QString& path );
void   SavePropFile( const QString& path , const IniMap& map );
