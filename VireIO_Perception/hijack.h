#pragma once
#include <qstringlist.h>

QStringList HijackListProcesses( );
QString     HijackAttachToProcess( int index        , const QStringList& dlls );
QString     HijackLaunchProcess  ( QString exe_path , const QStringList& dlls );
