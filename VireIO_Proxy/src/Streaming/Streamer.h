#include <string>
#include <d3dx9.h>
#include "../ProxyHelper.h"

#define D_STR1(x)  #x
#define D_STR(x)  D_STR1(x)
#define D_LINE    OutputDebugString( __FILE__  D_STR(__LINE__) );

class Streamer {
public:
	ProxyHelper::ProxyConfig cfg;

	Streamer (  ProxyHelper::ProxyConfig& c );
	~Streamer( );

	void send ( IDirect3DDevice9*  device  );

private:
	IDirect3DSurface9* surf;
	D3DSURFACE_DESC    desc;

	void enc_init( );
	void enc_free( );
	void enc_encode( void* buf , int w , int h );

	void net_init( );
	void net_free( );
	void net_send( void* buf , int len );

	union{
		struct{
			#ifdef VIREIO_STREAMER_ENC
				VIREIO_STREAMER_ENC
			#endif
		};
		void* dummy_enc[16];
	};

	union{
		struct{
			#ifdef VIREIO_STREAMER_NET
				VIREIO_STREAMER_NET
			#endif
		};
		void* dummy_net[16];
	};
};
