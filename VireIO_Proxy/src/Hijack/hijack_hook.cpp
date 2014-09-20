#include <Windows.h>
#include <Psapi.h>
#include <DbgHelp.h>
#include <string.h>
#include <list>
#include <VireIO.h>

namespace{
	struct HookInfo{
		const char* module_name;
		const char* function_name;
		PROC        new_function;
		PROC        old_function;
		PROC*       old_function_ret;
		HMODULE     module;
	};

	std::list<HookInfo*> hooks;
	
	FARPROC (WINAPI* ORIG_GetProcAddress )( HMODULE hModule , LPCSTR lpProcName );
	HMODULE (WINAPI* ORIG_LoadLibraryA   )( LPCSTR lpLibFileName );
}




void HijackHookAdd( const char* moduleName , const char* functionName , void** oldFunctionPtr  , void* newFunction ){
	HookInfo *h = new HookInfo;

	h->module_name      = _strdup( moduleName   );
	h->function_name    = _strdup( functionName );
	h->new_function     = (PROC)newFunction;
	h->old_function     = 0;
	h->module           = 0;
	h->old_function_ret = (PROC*)oldFunctionPtr;

	hooks.push_back( h );
}



void HijackHookUpdate(){
	HMODULE modules[1024];
	DWORD   size;

	if( !EnumProcessModules( GetCurrentProcess() , modules , sizeof(modules) , &size ) ){
		printf("hook: EnumProcessModules failed\n");
		return;
	}

	size = size/sizeof(HMODULE);


	for( HookInfo* h : hooks ){
		if( h->old_function ){
			continue;
		}

		if( stricmp( h->module_name , "*" ) == 0 ){
			for( DWORD c=0 ; c < size ; c++ ){
				h->old_function = ORIG_GetProcAddress( modules[c] , h->function_name );

				if( h->old_function ){
					h->module = modules[c];
					break;
				}
			}
		}else{
			h->module = GetModuleHandleA( h->module_name );
			if( !h->module ){
				continue;
			}

			h->old_function = ORIG_GetProcAddress( h->module , h->function_name );

			if( !h->old_function ){
				printf("function \"%s\" not found in module \"%s\"\n" , h->function_name , h->module_name );
				continue;
			}
		}

		if( h->old_function ){
			printf( "hook: found      %s\n" , h->function_name );
		}

		if( h->old_function_ret ){
			*h->old_function_ret = h->old_function;
		}
	}



	for( DWORD c=0 ; c < size ; c++ ){
		DWORD import_size;
		PIMAGE_IMPORT_DESCRIPTOR import = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData( modules[c] , true , IMAGE_DIRECTORY_ENTRY_IMPORT , &import_size );

		if( import ){
			while( import->Name ){

				PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)( ((char*)modules[c]) + import->FirstThunk );

				while( thunk->u1.Function ){
					PROC* func = (PROC*)&thunk->u1.Function;


					for( HookInfo* hook : hooks ){
						if(  hook->old_function && *func == hook->old_function  ){
							MEMORY_BASIC_INFORMATION mbi;
							DWORD ret;

							VirtualQuery( func , &mbi , sizeof(MEMORY_BASIC_INFORMATION) );

							VirtualProtect( mbi.BaseAddress , mbi.RegionSize , PAGE_READWRITE , &mbi.Protect );

							*func = hook->new_function;

							VirtualProtect( mbi.BaseAddress , mbi.RegionSize , mbi.Protect , &ret );

							char name[4096];
							GetModuleFileNameA( modules[c] , name , sizeof(name) );
							name[4095] = 0;

							/*if( hook->new_function != (PROC)NEW_GetProcAddress &&
								hook->new_function != (PROC)NEW_LoadLibraryA   &&
								hook->new_function != (PROC)NEW_LoadLibraryW   &&
								hook->new_function != (PROC)NEW_LoadLibraryExA &&
								hook->new_function != (PROC)NEW_LoadLibraryExW )
							{
								printf("hook: installed  %s  for  %s\n" , hook->function_name , name );
							}*/

						}
					}

					thunk++;
				}

				import++;
			}
		}
	}
}








QString HijackInject( HANDLE proccess ){
	FARPROC             load_func;
	void*               remote_lib_name = 0;
	HANDLE              remote_thread   = 0;
	DWORD               ret_dword;
	SIZE_T              ret_size;
	QStringList         ret;


	load_func = (FARPROC)ORIG_LoadLibraryA;
	if( !load_func ){
		load_func = (FARPROC)LoadLibraryA;
	}

	QByteArray path = (vireioDir+"bin/VireIO_Proxy.dll").toLocal8Bit();
	path.replace( "/" , "\\" );

	remote_lib_name = VirtualAllocEx( proccess , 0 , path.size()+1 , MEM_COMMIT , PAGE_READWRITE );
	if( !remote_lib_name ){
		ret += "VirtualAllocEx failed";
		goto end;
	}

	if( !WriteProcessMemory( proccess , remote_lib_name , path.data() , path.size()+1 , &ret_size )  ||  ret_size != path.size()+1  ){
		ret += "WriteProcessMemory failed";
		goto end;
	}

	remote_thread = CreateRemoteThread( proccess , 0 , 0 , (LPTHREAD_START_ROUTINE)load_func , remote_lib_name , 0 , 0 );
	if( !remote_thread ){
		ret += "CreateRemoteThread failed";
		goto end;
	}

	WaitForSingleObject( remote_thread , INFINITE );

	if( !GetExitCodeThread( remote_thread , &ret_dword ) ){
		goto end;
	}

	if( ret_dword==0 ){
		ret += "remote LoadLibrary(\"" + path + "\") failed";
		goto end;
	}


	end:

	if( remote_lib_name ){
		VirtualFreeEx( proccess , remote_lib_name , 0 , MEM_RELEASE );
	}

	if( remote_thread ){
		CloseHandle( remote_thread );
	}

	return ret.join("\n");
}















#define HOOK( module , ret , api , name , args , body ) 									\
	struct hook_##name{																		\
		static ret (api*& orig() ) args {													\
			static ret (api* orig_func) args;												\
			return orig_func;																\
		}																					\
		static ret api NEW args {                                                           \
			body											                       			\
		}																					\
	};																						\
	HijackHookAdd( module , #name , (void**)&hook_##name::orig()  , (void*)hook_##name::NEW );

#define ORIG orig()



void HijackHookInstall( ){
	printf("hook: installing...\n");

	ORIG_LoadLibraryA   = LoadLibraryA;
	ORIG_GetProcAddress = GetProcAddress;

	HOOK( "Kernel32.dll" , FARPROC , WINAPI , GetProcAddress , ( HMODULE hModule , LPCSTR lpProcName ) , {
		for( HookInfo* h : hooks ){
			if( h->module == hModule && strcmp(h->function_name,lpProcName)==0 ){
				printf("hook: requested  %s\n",lpProcName);
				return h->new_function;
			}
		}
		return ORIG( hModule , lpProcName );
	})

	HOOK( "Kernel32.dll" , HMODULE , WINAPI , LoadLibraryA , ( LPCSTR  lpLibFileName ) , {
		printf("hook: load       %s\n" , lpLibFileName );
		HMODULE m = ORIG( lpLibFileName );
		HijackHookUpdate();
		return m;
	})


	HOOK( "Kernel32.dll" , HMODULE , WINAPI , LoadLibraryW , ( LPCWSTR lpLibFileName ) , {
		printf("hook: load       %S\n" , lpLibFileName );
		HMODULE m = ORIG( lpLibFileName );
		HijackHookUpdate();
		return m;
	})


	HOOK( "Kernel32.dll" , HMODULE , WINAPI , LoadLibraryExA , ( LPCSTR lpLibFileName , HANDLE hFile , DWORD dwFlags ) , {
		printf("hook: load       %s\n" , lpLibFileName );
		HMODULE m = ORIG( lpLibFileName , hFile , dwFlags );
		HijackHookUpdate();
		return m;
	})


	HOOK( "Kernel32.dll" , HMODULE , WINAPI , LoadLibraryExW , ( LPCWSTR lpLibFileName , HANDLE hFile , DWORD dwFlags ) , {
		printf("hook: load       %S\n" , lpLibFileName );
		HMODULE m = ORIG( lpLibFileName , hFile , dwFlags );
		HijackHookUpdate();
		return m;
	})

	
	#define CreateProcess_macro(...)						 \
		dwCreationFlags |= CREATE_SUSPENDED;				 \
		BOOL r = ORIG(__VA_ARGS__);							 \
		if( r ){											 \
			HijackInject( lpProcessInformation->hProcess );	 \
			ResumeThread( lpProcessInformation->hThread );	 \
		}													 \
		return r;


	HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessA , (LPCSTR lpApplicationName,LPSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCSTR lpCurrentDirectory,LPSTARTUPINFOA lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %s\n" , lpApplicationName );
		CreateProcess_macro(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})

	/*HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessAsUserA , (HANDLE hToken,LPCSTR lpApplicationName,LPSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCSTR lpCurrentDirectory,LPSTARTUPINFOA lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %s\n" , lpApplicationName );
		CreateProcess_macro(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})*/

	HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessAsUserW , (HANDLE hToken,LPCWSTR lpApplicationName,LPWSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCWSTR lpCurrentDirectory,LPSTARTUPINFOW lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %S\n" , lpApplicationName );
		CreateProcess_macro(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})

	HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessW , (LPCWSTR lpApplicationName,LPWSTR lpCommandLine,LPSECURITY_ATTRIBUTES lpProcessAttributes,LPSECURITY_ATTRIBUTES lpThreadAttributes,BOOL bInheritHandles,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCWSTR lpCurrentDirectory,LPSTARTUPINFOW lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %S\n" , lpApplicationName );
		CreateProcess_macro(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})

	/*HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessWithLogonW , (LPCWSTR lpUsername,LPCWSTR lpDomain,LPCWSTR lpPassword,DWORD dwLogonFlags,LPCWSTR lpApplicationName,LPWSTR lpCommandLine,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCWSTR lpCurrentDirectory,LPSTARTUPINFOW lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %S\n" , lpApplicationName );
		CreateProcess_macro(lpUsername,lpDomain,lpPassword,dwLogonFlags,lpApplicationName,lpCommandLine,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})

	HOOK( "Kernel32.dll" , BOOL , WINAPI , CreateProcessWithTokenW , (HANDLE hToken,DWORD dwLogonFlags,LPCWSTR lpApplicationName,LPWSTR lpCommandLine,DWORD dwCreationFlags,LPVOID lpEnvironment,LPCWSTR lpCurrentDirectory,LPSTARTUPINFOW lpStartupInfo,LPPROCESS_INFORMATION lpProcessInformation) , {
		printf("hook: launching %S\n" , lpApplicationName );
		CreateProcess_macro(hToken,dwLogonFlags,lpApplicationName,lpCommandLine,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation)
	})*/
	
	HijackHookUpdate();

	printf("hook: installed  default hooks\n");
}
