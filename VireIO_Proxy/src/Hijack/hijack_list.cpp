#include <VireIO.h>
#include <Psapi.h>

static QList<HANDLE> g_handles;

QStringList HijackListProcesses( ){
	QStringList ret;
	DWORD       pids[65536];
	DWORD       ret_dword;
	char        name[MAX_PATH];


	for( HANDLE h : g_handles ){
		CloseHandle( h );
	}
	g_handles.clear();


	if( !EnumProcesses( pids , sizeof(DWORD)*65536 , &ret_dword ) ){
		return ret;
	}

	for( DWORD c=0 ;  c < ret_dword/sizeof(DWORD)  ; c++ ){
		HANDLE h = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION , false , pids[c] );
		if( h ){
			if( GetModuleBaseNameA( h , 0 , name , MAX_PATH ) ){
				g_handles += h;
				ret       += name;
			}else{
				CloseHandle( h );
			}
		}
	}

	return ret;
}

QString HijackAttachToProcess( int index ){

	if( index >= g_handles.count() ){
		return "process index out of range";
	}

	return HijackInject( g_handles[index] );
}