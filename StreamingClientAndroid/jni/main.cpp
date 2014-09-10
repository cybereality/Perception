#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"
#include <android/window.h>

android_app* app;

void log( const char *format , ... ){
	va_list list;
	va_start( list , format );
	__android_log_vprint( ANDROID_LOG_WARN , "vireio_client" , format , list );
}




namespace{
	EGLDisplay gl_display;
	EGLint     gl_num_configs;
	EGLConfig  gl_config;
	EGLint     gl_format;
	EGLSurface gl_surface;
	EGLContext gl_context;
	bool       gl_repaint;
	pthread_t  thread;
	GLuint     gl_texture;


	const EGLint gl_attribs[] = {
		EGL_SURFACE_TYPE , EGL_WINDOW_BIT ,
		EGL_BLUE_SIZE    , 8 ,
		EGL_GREEN_SIZE   , 8 ,
		EGL_RED_SIZE     , 8 ,
		EGL_NONE
	};

	pthread_mutex_t  gl_mutex;
	MediaBuffer*     gl_buffer;
	int              gl_width;
	int              gl_height;
}


/*
static void InitWindow( ){
	gl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize( gl_display , 0 , 0 );

	eglChooseConfig   ( gl_display , gl_attribs , &gl_config , 1 , &gl_num_configs );

	eglGetConfigAttrib( gl_display , gl_config , EGL_NATIVE_VISUAL_ID , &gl_format );

	ANativeWindow_setBuffersGeometry( app->window , 0 , 0 , gl_format );

	gl_surface = eglCreateWindowSurface( gl_display , gl_config , app->window , 0 );
	gl_context = eglCreateContext      ( gl_display , gl_config , 0 , 0 );

	eglMakeCurrent ( gl_display , gl_surface , gl_surface    , gl_context );
	//eglQuerySurface( display , surface , EGL_WIDTH  , &w );
	//eglQuerySurface( display , surface , EGL_HEIGHT , &h );

	glHint      ( GL_PERSPECTIVE_CORRECTION_HINT , GL_FASTEST );
	glEnable    ( GL_CULL_FACE );
	glShadeModel( GL_SMOOTH );
	glDisable   ( GL_DEPTH_TEST );
	glEnable    ( GL_TEXTURE_2D );

	glGenTextures( 1 , &gl_texture );
	glBindTexture( GL_TEXTURE_2D , gl_texture );
}



static void FreeWindow( ){
	if( gl_display  ){
		eglMakeCurrent( gl_display , EGL_NO_SURFACE , EGL_NO_SURFACE , EGL_NO_CONTEXT );

		if( gl_context ){
			eglDestroyContext( gl_display , gl_context );
			gl_context = 0;
		}

		if( gl_surface ){
			eglDestroySurface( gl_display , gl_surface );
			gl_surface = 0;
		}

		eglTerminate( gl_display );
		gl_display = 0;
	}
}



static void RepaintWindow( ){
	MediaBuffer* buffer = 0;
	int          width;
	int          height;

	pthread_mutex_lock( &gl_mutex );
	buffer = gl_buffer;
	width  = gl_width;
	height = gl_height;
	gl_buffer  = 0;
	pthread_mutex_unlock( &gl_mutex );

	if( buffer ){
		log("copying...");
		char* td = new char[ width * height *4 ];
		memset( td , 255 , width * height *4 );
		glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , 512 , 512 , 0 , GL_RGB , GL_UNSIGNED_BYTE , td );
		//((char*)buffer->data()) + buffer->range_offset() );
		delete[] td;
	}
	*ANativeWindow_Buffer buffer;
	if( ANativeWindow_lock( app->window , &buffer , 0 ) == 0 ){
	  //memcpy(buffer.bits, pixels,  w * h * 2);
	  ANativeWindow_unlockAndPost( app->window );
	}
	*


	// Just fill the screen with a color.
	static float a=0;
	a += 0.01;

	if( a>1 ){
		a=0;
	}
	ANativeWindow
	app->window->

	glClearColor( a , a , a , 1 );
	glClear     ( GL_COLOR_BUFFER_BIT );

	static float vertexes[]={
		-1 , -1 , 0 , 0 , 1 ,
		 1 , -1 , 0 , 1 , 1 ,
		 1 ,  1 , 0 , 1 , 0 ,
		-1 ,  1 , 0 , 0 , 0 ,
	};

	static GLushort indexes[]={
		0 , 1 , 2 ,
		0 , 2 , 3 ,
	};


	glVertexPointer  ( 3 , GL_FLOAT , 5*sizeof(float) , vertexes );
	glTexCoordPointer( 2 , GL_FLOAT , 5*sizeof(float) , vertexes + 3 );
	glDrawElements ( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , indexes);

	eglSwapBuffers( gl_display , gl_surface );
}

*/



void drawer_render_yuv420( int width , int height , MediaBuffer* buffer ){
	buffer->add_ref();

	pthread_mutex_lock( &gl_mutex );
	if( gl_buffer ){
		gl_buffer->release();
	}
	gl_buffer = buffer;
	gl_width  = width;
	gl_height = height;
	pthread_mutex_unlock( &gl_mutex );

	gl_repaint = true;
	ALooper_wake( app->looper );
}





static void* thread_func( void* ){
	for(;;){
		decoder_decode( );
	}
	return 0;
}




static void on_app_cmd( android_app* , int32_t cmd ){
	switch( cmd ){
	case APP_CMD_SAVE_STATE:
		break;

	case APP_CMD_INIT_WINDOW:
		//InitWindow( );
		decoder_init( );
		network_init( );
		pthread_create( &thread , 0 , thread_func , 0 );
		break;

	case APP_CMD_TERM_WINDOW:
		//FreeWindow( );
		network_free( );
		decoder_free( );
		pthread_join( thread , 0 );
		break;

	case APP_CMD_GAINED_FOCUS:
		sensor_init( );
		break;

	case APP_CMD_LOST_FOCUS:
		sensor_free( );
		break;
	}
}



static int32_t on_input( android_app* , AInputEvent* event ){
	return 0;
}



using namespace android;

void android_main( android_app* state ){
	log("starting");
	app = state;
	app_dummy();
	ANativeActivity_setWindowFlags  ( app->activity , AWINDOW_FLAG_KEEP_SCREEN_ON , 0 );



	state->userData     = 0;
	state->onAppCmd     = on_app_cmd;
	state->onInputEvent = on_input;

	pthread_mutex_init( &gl_mutex , 0 );

	log("loop");

	for(;;){

		int                  num_events;
		android_poll_source* source;
		int                  ident;

		ident = ALooper_pollAll( 0 , 0 , &num_events , (void**)&source );

		if( ident >= 0 ){
			if( source ){
				source->process( state , source );
			}

			if( state->destroyRequested ){
				break;
			}

			if( ident == LOOPER_ID_USER ){
				sensor_poll( );
			}
		}

		if( gl_repaint ){
			gl_repaint = false;
			//RepaintWindow( );
		}
	}

	log("exiting");






	//FreeWindow( );

	pthread_mutex_destroy( &gl_mutex );

	log("done");
}








/*struct engine* engine = (struct engine*)app->userData;
if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
	engine->animating = 1;
	engine->state.x = AMotionEvent_getX(event, 0);
	engine->state.y = AMotionEvent_getY(event, 0);
	return 1;
}
*/



		/*
		// Read all pending events.


        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
		while((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
			*/

		/*if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
			engine_draw_frame(&engine);
		}*/








/*
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

struct engine {
	struct android_app* app;


	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};


static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// No display.
		return;
	}

}

*/
