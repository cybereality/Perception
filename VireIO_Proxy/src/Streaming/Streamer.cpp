#include "Streamer.h"

Streamer::Streamer( cConfig& c ) :
	surf(0) ,
	cfg (c)
{
	net_init();
	enc_init();
}


Streamer::~Streamer( ){
	enc_free( );
	net_free( );
}


void Streamer::send( IDirect3DDevice9* device ){
	LARGE_INTEGER i1;
	LARGE_INTEGER i2;
	LARGE_INTEGER i3;

	QueryPerformanceCounter( &i1 );

	if( !cfg.streamingEnable ){
		return;
	}
	
	IDirect3DSurface9* rt;
	if( FAILED(device->GetRenderTarget(0,&rt)) ){
		OutputDebugStringA( "Streamer: GetRenderTarget failed\n" );
		return;
	}
	
	if( !surf ){
		if( FAILED(rt->GetDesc(&desc)) ){
			OutputDebugStringA( "Streamer: GetDesc failed\n" );
			rt->Release();
			return;
		}
		
		if( FAILED(device->CreateOffscreenPlainSurface( desc.Width , desc.Height , desc.Format , D3DPOOL_SYSTEMMEM , &surf, 0 )) ){
			OutputDebugStringA( "Streamer: CreateOffscreenPlainSurface failed\n" );
			rt->Release();
			return;
		}
	}
	
	HRESULT r = device->GetRenderTargetData(rt,surf);
	if( FAILED(r) ){
		OutputDebugStringA( "Streamer: GetRenderTargetData failed\n" );
		rt  ->Release();
		surf->Release();
		surf = 0;
		return;
	}
	
	D3DLOCKED_RECT rect;
	if( FAILED(surf->LockRect( &rect , 0 , D3DLOCK_READONLY )) ){
		OutputDebugStringA( "Streamer: LockRect failed\n" );
		return;
	}
	
	enc_encode( rect.pBits , desc.Width , desc.Height );
	
	QueryPerformanceCounter  ( &i2 );
	QueryPerformanceFrequency( &i3 );
	printf( "encoding time %llu ms\n" , (i2.QuadPart - i1.QuadPart)*1000/i3.QuadPart );

	surf->UnlockRect();
	
	rt->Release();
}
