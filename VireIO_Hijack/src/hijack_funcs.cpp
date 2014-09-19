#include <Windows.h>
#include <string>
#include "hijack_hook.h"
#include <d3d9.h>

namespace{
	IDirect3D9*   (WINAPI* ORIG_Direct3DCreate9   )( unsigned int nSDKVersion );
	HRESULT       (WINAPI* ORIG_Direct3DCreate9Ex )( unsigned int SDKVersion , IDirect3D9Ex **ppD3D );
	IDirect3D9Ex* (*       PROXY_Direct3DCreate9  )( IDirect3D9* base , IDirect3D9Ex* baseEx );

	IDirect3D9* WINAPI NEW_Direct3DCreate9( unsigned int nSDKVersion ){
		if( !PROXY_Direct3DCreate9 ){
			printf("hook: proxy not loaded!\n");
			return 0;
		}
		
		IDirect3D9* ret = ORIG_Direct3DCreate9(nSDKVersion);
		if( !ret ){
			return 0;
		}

		IDirect3D9* proxy = PROXY_Direct3DCreate9( ret , 0 );
		if( proxy ){
			return proxy;
		}

		return ret;
	}

	HRESULT WINAPI NEW_Direct3DCreate9Ex( unsigned int nSDKVersion , IDirect3D9Ex** ret ){
		HRESULT result = ORIG_Direct3DCreate9Ex(nSDKVersion,ret);
		if( SUCCEEDED(result) ){
			IDirect3D9Ex* proxy = PROXY_Direct3DCreate9( *ret , *ret );
			if( proxy ){
				*ret = proxy;
			}
		}
		return result;
	}

}



BOOL APIENTRY DllMain( HINSTANCE , DWORD fdwReason, LPVOID ){
	if( fdwReason == DLL_PROCESS_ATTACH ){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		vireio_hijack_hook_install();

		vireio_hijack_hook_add( "d3d9.dll" , "Direct3DCreate9"        , (void**)&ORIG_Direct3DCreate9    , (void*)NEW_Direct3DCreate9   );
		vireio_hijack_hook_add( "d3d9.dll" , "Direct3DCreate9Ex"      , (void**)&ORIG_Direct3DCreate9Ex  , (void*)NEW_Direct3DCreate9Ex );
		vireio_hijack_hook_add( "*"        , "ProxyDirect3DCreate9"   , (void**)&PROXY_Direct3DCreate9   , 0                            );

		LoadLibraryA( "d3d9.dll" );

		vireio_hijack_hook_update( );

	}

	return TRUE;
}
