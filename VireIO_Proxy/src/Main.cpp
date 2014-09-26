/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Main.cpp> :
Copyright (C) 2012 Andres Hernandez

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include <windows.h>
#include <d3d9.h>
#include "D3D9ProxyDirect3D.h"
#include <VireIO.h>
#include <qcoreapplication.h>
#include <qdebug.h>
#include <cConfig.h>
#include <qfileinfo.h>
#include <qmessagebox.h>


namespace{
	bool    loaded = false;

	IDirect3D9*   (WINAPI* ORIG_Direct3DCreate9   )( unsigned int nSDKVersion );
	HRESULT       (WINAPI* ORIG_Direct3DCreate9Ex )( unsigned int SDKVersion , IDirect3D9Ex **ppD3D );

	IDirect3D9* WINAPI NEW_Direct3DCreate9( unsigned int nSDKVersion ){
		printf("vireio: creating Direct3D\n" );

		//This workaround for steam GameOverlayRendereer hooks
		char signature[5] = { 0x8B , 0xFF , 0x55 , 0x8B , 0xEC };
		if( memcmp( ORIG_Direct3DCreate9 , signature , 5 ) != 0 ){
			printf("vireio: jmp-base another hook detected. reverting jmp hook back. THIS MAY CRASH!\n" );

			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery( ORIG_Direct3DCreate9 , &mbi , sizeof(MEMORY_BASIC_INFORMATION) );

			VirtualProtect( mbi.BaseAddress , mbi.RegionSize , PAGE_READWRITE , &mbi.Protect );

			memcpy( ORIG_Direct3DCreate9 , signature , 5 );

			DWORD ret;
			VirtualProtect( mbi.BaseAddress , mbi.RegionSize , mbi.Protect , &ret );
			
			printf("vireio: reverted.\n" );
		}


		IDirect3D9* ret = ORIG_Direct3DCreate9(nSDKVersion);
		if( !ret ){
			return 0;
		}
		
		return new D3D9ProxyDirect3D( ret , 0 , config );
	}

	HRESULT WINAPI NEW_Direct3DCreate9Ex( unsigned int nSDKVersion , IDirect3D9Ex** ret ){
		// Ex device somehow behave diferently (hl2.exe)
		printf("vireio: creating Direct3DEx (not supported)\n");
		return -1;
		//HRESULT result = ORIG_Direct3DCreate9Ex(nSDKVersion,ret);
		//if( SUCCEEDED(result) ){
		//	IDirect3D9Ex* proxy = PROXY_Direct3DCreate9( *ret , *ret );
		//	if( proxy ){
		//		*ret = proxy;
		//	}
		//}
		//return result;
	}

}



BOOL APIENTRY DllMain( HINSTANCE dll , DWORD fdwReason, LPVOID ){
	if( fdwReason == DLL_PROCESS_DETACH ){
		if( config.logToConsole && config.pauseOnLaunch ){
			MessageBoxA( 0 , "pause" , "click ok to exit proccess" , 0 );
		}
	}

	if( fdwReason != DLL_PROCESS_ATTACH ){
		return TRUE;
	}



	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	HijackHookInstall();

	HijackHookAdd( "d3d9.dll" , "Direct3DCreate9"        , (void**)&ORIG_Direct3DCreate9    , (void*)NEW_Direct3DCreate9   );
	HijackHookAdd( "d3d9.dll" , "Direct3DCreate9Ex"      , (void**)&ORIG_Direct3DCreate9Ex  , (void*)NEW_Direct3DCreate9Ex );


	char exe_path[MAX_PATH];
	char dll_path[MAX_PATH];

	GetModuleFileNameA( 0   , exe_path , MAX_PATH );
	GetModuleFileNameA( dll , dll_path , MAX_PATH );

	
	config.vireioDir = QFileInfo(dll_path).absolutePath() + "/../";

	if( !config.load( config.getMainConfigFile() ) ){
		printf( "virieo: load general settings failed\n" );
		return FALSE;
	}

	if( !config.load( config.getGameConfigFile(exe_path) ) ){
		printf( "virieo: load game settings failed\n" );
		return FALSE;
	}

	if( !config.loadDevice( ) ){
		printf( "virieo: load device settings failed \n" );
		return FALSE;
	}

	if( !config.loadProfile( ) ){
		printf( "virieo: load profile settings failed\n" );
		return FALSE;
	}

	//load game settings again, in case if there any overrides for profile settings
	config.load( config.getGameConfigFile(exe_path) );

	config.calculateValues();



	if( !config.logToConsole ){
		FreeConsole();
	}

	if( config.logToFile ){
		freopen( "vireio_log.txt" , "w" , stdout );
	}

	if( config.pauseOnLaunch ){
		MessageBoxA( 0 , "pause" , "click ok to resume" , 0 );
	}

	LoadLibraryA( "d3d9.dll" );


	return TRUE;
}

