#include <binder/ProcessState.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/OMXCodec.h>
#include "common.h"
#include <system/window.h>
#include <android/window.h>


using namespace android;

namespace{
	sp<MetaData> meta_format;
	int          pts;

	class CustomSource : public MediaSource {
	public:

		sp<MetaData> getFormat() {
			return meta_format;
		}

		status_t start( MetaData* ){
			return OK;
		}

		status_t stop( ){
			return OK;
		}

		status_t read( MediaBuffer** buffer , const MediaSource::ReadOptions* options ){
			*buffer = new MediaBuffer( 1<<20 );

			int ret = network_read( (char*)(*buffer)->data() , (*buffer)->size() );

			if( ret < 0 ){
				log("read fail");
				(*buffer)->release();
				return EINVAL;
			}

			(*buffer)->set_range( 0 , ret );
			(*buffer)->meta_data()->clear();
			(*buffer)->meta_data()->setInt32(kKeyIsSyncFrame , false );
			(*buffer)->meta_data()->setInt64(kKeyTime        , pts++ );

			return OK;
		}
	};


	OMXClient        client;
	sp<CustomSource> source;
	sp<MediaSource>  decoder;
}



void decoder_init( ){
	//native_window_set_buffers_timestamp( app->window , NATIVE_WINDOW_TIMESTAMP_AUTO );
	ANativeWindow_setBuffersGeometry( app->window   , 1920 , 1080 , WINDOW_FORMAT_RGBX_8888 );

	client.connect();

	source = new CustomSource;

	meta_format = new MetaData;
	meta_format->setCString( kKeyMIMEType , MEDIA_MIMETYPE_VIDEO_MPEG2 );
	meta_format->setInt32  ( kKeyWidth    , 256 );
	meta_format->setInt32  ( kKeyHeight   , 256 );
	//meta->setData   ( kKeyAVCC     , kTypeAVCC, avctx->extradata, avctx->extradata_size);

	decoder = OMXCodec::Create( client.interface() , meta_format , false , source , 0 , OMXCodec::kOnlySubmitOneInputBufferAtOneTime , app->window );
	decoder->start();
}



void decoder_free( ){
	source.clear();

	decoder->stop();
	decoder.clear();

	client.disconnect();
}



void decoder_decode( ){
	MediaBuffer*             buffer;
	MediaSource::ReadOptions options;



	status_t r = decoder->read( &buffer , &options );

	if( r != OK ){
		log( "decoded status %8.8X" , r );
		if( r == 0x80000000 ){
			log( "restarting..." );
			decoder_free( );
			decoder_init( );
		}
		timespec ts;
		ts.tv_sec  = 0;
		ts.tv_nsec = 50000000;
		nanosleep( &ts , 0 );
		return;
	}

	if( buffer->range_length() > 0 ){
		sp<MetaData> meta = decoder->getFormat();

		int32_t width;
		int32_t height;
		int32_t format;

		bool ok = true;
		ok &= meta->findInt32( kKeyWidth       , &width );
		ok &= meta->findInt32( kKeyHeight      , &height );
		ok &= meta->findInt32( kKeyColorFormat , &format );

		if( !ok ){
			log("invalid metadata");
		}else{
			log("frame %dX%d" , width , height );
			//app->window->queueBuffer( app->window , buffer->graphicBuffer()->getNativeBuffer() );

			//int64_t timeUs = NATIVE_WINDOW_TIMESTAMP_AUTO;
			//buffer->meta_data()->findInt64( kKeyTime , &timeUs );
			//native_window_set_buffers_timestamp( app->window , timeUs * 1000 );

			r = app->window->queueBuffer( app->window , buffer->graphicBuffer()->getNativeBuffer() );
			if( r ){
				log("queueBuffer failed %d" , r );
				return;
			}

			//buffer->meta_data()->setInt32(kKeyRendered, 1);

			//drawer_render_yuv420( width , height , buffer );
		}


		// If video frame availabe, render it to mNativeWindow

		//int64_t timeUs = 0;
		//metaData->findInt64(kKeyTime, &timeUs)
		//native_window_set_buffers_timestamp(mNativeWindow.get(), timeUs * 1000);
		//err = mNativeWindow->queueBuffer(mNativeWindow.get(), mVideoBuffer->graphicBuffer().get());
		//if (err == 0) {
		//	metaData->setInt32(kKeyRendered, 1);
		//}
	}
	buffer->release();
}
