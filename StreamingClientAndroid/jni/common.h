#include <android_native_app_glue.h>
#include <media/stagefright/MediaBuffer.h>


using android::MediaBuffer;

extern android_app* app;

void decoder_init  ( );
void decoder_free  ( );
void decoder_decode( );

void drawer_render_yuv420( int width , int height , MediaBuffer* buffer );

void sensor_init( );
void sensor_free( );
void sensor_poll( );

void network_init( );
void network_free( );
void network_send( int32_t type , int64_t timestamp , float* data );
int  network_read( char* buf , int len );


void log           ( const char* format , ... );

