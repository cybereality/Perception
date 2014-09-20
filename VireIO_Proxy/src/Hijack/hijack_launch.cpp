#include <Windows.h>
#include <Psapi.h>
#include <Vireio.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qprocess.h>


QString HijackLaunchProcess  ( QString exe_path , QString args , QStringList environment , const bool pause ){
	PROCESS_INFORMATION pi;
	STARTUPINFOA        si;
	QString             ret;

	pi.hProcess = INVALID_HANDLE_VALUE;
	pi.hThread  = INVALID_HANDLE_VALUE;

	memset( &si , 0 , sizeof(si) );
	si.cb          = sizeof(si);
	si.wShowWindow = SW_NORMAL;

	exe_path.replace( "/" , "\\" );
	QByteArray exe = exe_path.toLocal8Bit();
	QByteArray dir = QFileInfo(exe_path).absolutePath().toLocal8Bit();

	QByteArray env;
	for( QString s : (environment + QProcessEnvironment::systemEnvironment().toStringList()) ){
		if( s.toLower().startsWith("path=") ){
			s = s + ";" + vireioDir + "bin";
		}
		env += s.toLocal8Bit() + QByteArray("\0",1);
	}
	env += QByteArray("\0",1);


	if( !CreateProcessA( exe , args.toLocal8Bit().data() , 0, 0 , false , CREATE_SUSPENDED , env.data() , dir , &si ,&pi ) ){
		ret = "CreateProcessA failed";
		goto end;
	}

	ret = HijackInject( pi.hProcess );
	if( !ret.isEmpty() ){
		goto end;
	}

	if( pause ){
		QMessageBox::information( 0 , "pause" , "Click ok to resume game" );
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
