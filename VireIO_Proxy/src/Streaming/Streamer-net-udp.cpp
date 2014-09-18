#include <Winsock2.h>

#define VIREIO_STREAMER_NET \
	SOCKET      sock; \
	sockaddr_in addr; \
	bool        tcp;  \
	bool        connected;

#include "Streamer.h"


void Streamer::net_init( ){
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2,2) , &wsaData );

	if( cfg.streaming_protocol == "udp" ){
		sock = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
		tcp  = false;
	}else
	if( cfg.streaming_protocol == "tcp" ){
		sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
		tcp  = true;
	}

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr( cfg.streaming_host.c_str()  );
	addr.sin_port        = htons( cfg.streaming_port );
	
	connected = false;
}



void Streamer::net_free( ){
	closesocket( sock );
	WSACleanup();
}



void Streamer::net_send( void* buffer , int size ){
	if( sock == INVALID_SOCKET ){
		return;
	}

	/*if( !connected ){
		if( connect( sock , (sockaddr*)&addr , sizeof(addr) ) == 0 ){
			connected = true;
		}else{
			return;
		}
	}*/


	int pos = 0;
	
	while( pos < size ){
		int len = size-pos;

		if( !tcp ){
			if( len > 65507 ){
				len = 65507;
			}else
			if( len == 65507 ){
				len = 65507-1;
			}
		}

		int ret = sendto( sock , ((char*)buffer)+pos , len , 0 , (sockaddr*)&addr , sizeof(addr) );
		//int ret = ::send( sock , ((char*)buffer)+pos , len , 0 );

		if( ret < 0 ){
			return;
		}

		pos += ret;
	}
}
