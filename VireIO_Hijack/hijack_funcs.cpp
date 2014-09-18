#include <Windows.h>
#include <string>
#include "hijack_hook.h"


namespace{
	class IDirect3D9;
	class IDirect3D9Ex;

	IDirect3D9* (WINAPI* ORIG_Direct3DCreate9   )( unsigned int nSDKVersion );
	HRESULT     (WINAPI* ORIG_Direct3DCreate9Ex )( unsigned int SDKVersion , IDirect3D9Ex **ppD3D );
	IDirect3D9* (WINAPI* PROXY_Direct3DCreate9  )( IDirect3D9* base );

	IDirect3D9* WINAPI NEW_Direct3DCreate9( unsigned int nSDKVersion ){
		printf("Create D3D\n");
		if( !PROXY_Direct3DCreate9 ){
			printf("Proxy not loaded!\n");
			return 0;
		}

		return PROXY_Direct3DCreate9( ORIG_Direct3DCreate9(nSDKVersion) );
	}

	HRESULT WINAPI NEW_Direct3DCreate9Ex( unsigned int SDKVersion , IDirect3D9Ex **ppD3D ){
		printf("Direct3D9Ex not supported!\n");
		return -1;
	}

}



BOOL APIENTRY DllMain( HINSTANCE hModule , DWORD fdwReason, LPVOID ){
	if( fdwReason == DLL_PROCESS_ATTACH ){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		vireio_hijack_hook_install();

		vireio_hijack_hook_add( "d3d9.dll" , "Direct3DCreate9"      , (void**)&ORIG_Direct3DCreate9   , (void*)NEW_Direct3DCreate9   );
		vireio_hijack_hook_add( "d3d9.dll" , "Direct3DCreate9Ex"    , (void**)&ORIG_Direct3DCreate9Ex , (void*)NEW_Direct3DCreate9Ex );
		vireio_hijack_hook_add( "*"        , "ProxyDirect3DCreate9" , (void**)&PROXY_Direct3DCreate9  , 0                            );

		LoadLibraryA( "d3d9.dll" );

		/*char path[4096];
		GetModuleFileNameA( hModule , path , 4096 );

		std::string str_path( path );
		str_path = str_path.substr( 0 , str_path.find_last_of('\\') + 1 );

		HMODULE proxy = LoadLibraryA( (str_path + "vireio_proxy.dll").c_str() );

		PROXY_Direct3DCreate9 = (IDirect3D9*(WINAPI*)(IDirect3D9*))GetProcAddress( proxy , "ProxyDirect3DCreate9" );*/



		vireio_hijack_hook_update( );

	}

	return TRUE;
}
