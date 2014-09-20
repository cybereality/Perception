#include <Windows.h>
#include <Psapi.h>
#include <DbgHelp.h>
#include <string.h>
#include <list>
#include "hijack_hook.h"

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


	FARPROC     (WINAPI* ORIG_GetProcAddress )( HMODULE hModule , LPCSTR lpProcName );
	HMODULE     (WINAPI* ORIG_LoadLibraryA   )( LPCSTR  lpLibFileName );
	HMODULE     (WINAPI* ORIG_LoadLibraryW   )( LPCWSTR lpLibFileName );
	HMODULE     (WINAPI* ORIG_LoadLibraryExA )( LPCSTR  lpLibFileName , HANDLE hFile , DWORD dwFlags );
	HMODULE     (WINAPI* ORIG_LoadLibraryExW )( LPCWSTR lpLibFileName , HANDLE hFile , DWORD dwFlags );


	FARPROC WINAPI NEW_GetProcAddress( HMODULE hModule , LPCSTR lpProcName ){
		printf("hook: requested  %s\n",lpProcName);

		for( HookInfo* h : hooks ){
			if( h->module == hModule && strcmp(h->function_name,lpProcName)==0 ){
				return h->new_function;
			}
		}
		return ORIG_GetProcAddress( hModule , lpProcName );
	}


	HMODULE WINAPI NEW_LoadLibraryA( LPCSTR  lpLibFileName ){
		printf("hook: load       %s\n" , lpLibFileName );
		HMODULE m = ORIG_LoadLibraryA( lpLibFileName );
		vireio_hijack_hook_update();
		return m;
	}


	HMODULE WINAPI NEW_LoadLibraryW( LPCWSTR lpLibFileName ){
		printf("hook: load       %S\n" , lpLibFileName );
		HMODULE m = ORIG_LoadLibraryW( lpLibFileName );
		vireio_hijack_hook_update();
		return m;
	}


	HMODULE WINAPI NEW_LoadLibraryExA( LPCSTR lpLibFileName , HANDLE hFile , DWORD dwFlags ){
		printf("hook: load       %s\n" , lpLibFileName );
		HMODULE m = ORIG_LoadLibraryExA( lpLibFileName , hFile , dwFlags );
		vireio_hijack_hook_update();
		return m;
	}


	HMODULE WINAPI NEW_LoadLibraryExW( LPCWSTR lpLibFileName , HANDLE hFile , DWORD dwFlags ){
		printf("hook: load       %S\n" , lpLibFileName );
		HMODULE m = ORIG_LoadLibraryExW( lpLibFileName , hFile , dwFlags );
		vireio_hijack_hook_update();
		return m;
	}
}




void vireio_hijack_hook_add( const char* moduleName , const char* functionName , void** oldFunctionPtr  , void* newFunction ){
	HookInfo *h = new HookInfo;

	h->module_name      = _strdup( moduleName   );
	h->function_name    = _strdup( functionName );
	h->new_function     = (PROC)newFunction;
	h->old_function     = 0;
	h->module           = 0;
	h->old_function_ret = (PROC*)oldFunctionPtr;

	hooks.push_back( h );
}



void vireio_hijack_hook_update(){
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

							if( hook->new_function != (PROC)NEW_GetProcAddress &&
								hook->new_function != (PROC)NEW_LoadLibraryA   &&
								hook->new_function != (PROC)NEW_LoadLibraryW   &&
								hook->new_function != (PROC)NEW_LoadLibraryExA &&
								hook->new_function != (PROC)NEW_LoadLibraryExW )
							{
								printf("hook: installed  %s  for  %s\n" , hook->function_name , name );
							}

						}
					}

					thunk++;
				}

				import++;
			}
		}
	}
}



void vireio_hijack_hook_install( ){
	ORIG_GetProcAddress = GetProcAddress;

	vireio_hijack_hook_add( "Kernel32.dll" , "GetProcAddress"   , 0                              , (void*)NEW_GetProcAddress   );
	vireio_hijack_hook_add( "Kernel32.dll" , "LoadLibraryA"     , (void**)&ORIG_LoadLibraryA     , (void*)NEW_LoadLibraryA     );
	vireio_hijack_hook_add( "Kernel32.dll" , "LoadLibraryW"     , (void**)&ORIG_LoadLibraryW     , (void*)NEW_LoadLibraryW     );
	vireio_hijack_hook_add( "Kernel32.dll" , "LoadLibraryExA"   , (void**)&ORIG_LoadLibraryExA   , (void*)NEW_LoadLibraryExA   );
	vireio_hijack_hook_add( "Kernel32.dll" , "LoadLibraryExW"   , (void**)&ORIG_LoadLibraryExW   , (void*)NEW_LoadLibraryExW   );

	vireio_hijack_hook_update();

	printf("hook: installed  default hooks\n");
}
