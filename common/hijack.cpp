#include <Windows.h>
#include <Psapi.h>
#include "hijack.h"
#include <qdebug.h>
static QList<HANDLE> g_handles;



static QString Inject( HANDLE h , const QStringList& dlls ){
	FARPROC             load_func;
	void*               remote_lib_name = 0;
	HANDLE              remote_thread   = 0;
	DWORD               ret_dword;
	SIZE_T              ret_size;
	QStringList         ret;


	load_func = GetProcAddress( GetModuleHandleA("Kernel32") , "LoadLibraryA" );
	if( !load_func ){
		return "LoadLibrary not found";
	}

	for( QString dll : dlls ){
		QByteArray path = dll.replace( "/" , "\\" ).toLocal8Bit();

		remote_lib_name = VirtualAllocEx( h , 0 , path.size()+1 , MEM_COMMIT , PAGE_READWRITE );
		if( !remote_lib_name ){
			ret += "VirtualAllocEx failed";
			goto end;
		}

		if( !WriteProcessMemory( h , remote_lib_name , path.data() , path.size()+1 , &ret_size )  ||  ret_size != path.size()+1  ){
			ret += "WriteProcessMemory failed";
			goto end;
		}

		remote_thread = CreateRemoteThread( h , 0 , 0 , (LPTHREAD_START_ROUTINE)load_func , remote_lib_name , 0 , 0 );
		if( !remote_thread ){
			ret += "CreateRemoteThread failed";
			goto end;
		}

		WaitForSingleObject( remote_thread , INFINITE );

		if( !GetExitCodeThread( remote_thread , &ret_dword ) ){
			ret += "GetExitCodeThread failed";
			goto end;
		}

		if( ret_dword==0 ){
			ret += "remote LoadLibrary(\"" + path + "\") failed";
			goto end;
		}


		end:

		if( remote_lib_name ){
			VirtualFreeEx( h , remote_lib_name , 0 , MEM_RELEASE );
		}

		if( remote_thread ){
			CloseHandle( remote_thread );
		}
	}

	return ret.join("\n");
}




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



QString HijackAttachToProcess( int index , const QStringList& dlls ){

	if( index >= g_handles.count() ){
		return "process index out of range";
	}

	return Inject( g_handles[index] , dlls );
}




QString HijackLaunchProcess  ( QString exe_path , const QStringList& dlls ){

	PROCESS_INFORMATION pi;
	STARTUPINFOA        si;
	QString             ret;

	pi.hProcess = INVALID_HANDLE_VALUE;
	pi.hThread  = INVALID_HANDLE_VALUE;

	memset( &si , 0 , sizeof(si) );
	si.cb          = sizeof(si);
	si.wShowWindow = SW_NORMAL;

	exe_path.replace( "/" , "\\" );

	if( !CreateProcessA( exe_path.toLocal8Bit().data() , 0 , 0, 0 , false , CREATE_SUSPENDED , 0 , 0 , &si ,&pi ) ){
		ret = "CreateProcessA failed";
		goto end;
	}

	ret = Inject( pi.hProcess , dlls );
	if( !ret.isEmpty() ){
		goto end;
	}

	ResumeThread( pi.hThread );


	end:

	if( !ret.isEmpty() ){
		if( pi.hProcess != INVALID_HANDLE_VALUE){
			TerminateProcess( pi.hProcess , 0 );
		}
	}

	if( pi.hProcess != INVALID_HANDLE_VALUE){
		CloseHandle( pi.hProcess );
	}

	if( pi.hThread != INVALID_HANDLE_VALUE){
		CloseHandle( pi.hThread );
	}

	return ret;
}
