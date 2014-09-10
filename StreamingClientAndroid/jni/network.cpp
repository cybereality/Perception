#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common.h"



namespace{
	int         sock;
	int         server;
	sockaddr_in address;
	pthread_t   thread;
	bool        tcp = false;
}



void network_init( ){
	if( tcp ){
		server = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
	}else{
		server = socket( AF_INET , SOCK_DGRAM  , IPPROTO_UDP );
	}
	if( server < 0 ){
		log( "socket: create failed" );
		return;
	}

	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port        = htons( 8501 );

	if( bind( server , (sockaddr*)&address , sizeof(address) ) < 0 ){
		log( "socket: bind failed" );
		return;
	}

	if( tcp ){
		if( listen( server , SOMAXCONN ) < 0 ){
			log( "socket: listen failed" );
			return;
		}

		sock = accept( server , 0 , 0 );
		if( sock < 0 ){
			log( "socket: accept failed" );
		}
	}else{
		sock = server;
	}
}



void network_free( ){
	if( sock >= 0 ){
		shutdown( sock , SHUT_RDWR );
		::close( sock );
		sock = -1;
	}

	if( server >= 0 ){
		shutdown( server , SHUT_RDWR );
		::close( server );
		server = -1;
	}
}



int network_read( char* buf , int len ){
	if( sock < 0 ){
		return -1;
	}

	socklen_t addr_len = sizeof(address);
	int       size = 0;
	for(;;){
		int r;

		if( tcp ){
			r = recv( sock , buf , len , MSG_NOSIGNAL );
		}else{
			r = recvfrom( sock , buf , len , MSG_NOSIGNAL , (sockaddr*)&address , &addr_len );
		}

		if( r<0 ){
			log( "receive error" , r );
			return r;
		}

		size += r;
		buf  += r;
		len  -= r;

		if( r!=65507 ){
			break;
		}
	}

	log( "received %d" , size );
	return size;
}


void network_send( int32_t type , int64_t timestamp , float* data ){
	struct Packet{
		int32_t type;
		int64_t timestamp;
		float   x;
		float   y;
		float   z;
	};

	Packet p;
	p.type      = htonl( type      );
	p.timestamp = htonq( timestamp );
	p.x         = data[0];
	p.y         = data[1];
	p.z         = data[2];
/*
	if( address.sin_addr.s_addr != INADDR_ANY ){
		int r;
		if( tcp ){
			r = send( sock , &p , sizeof(p) , MSG_NOSIGNAL );
		}else{
			r = sendto( sock , &p , sizeof(p) , MSG_NOSIGNAL , (sockaddr*)&address , sizeof(address) );
		}
		//LOG( "sent %d to port %d" , ret , ntohs( udp_address.sin_port ) );
	}
*/
}
