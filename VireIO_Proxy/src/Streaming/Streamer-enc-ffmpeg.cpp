#define __STDC_CONSTANT_MACROS

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}


#define VIREIO_STREAMER_ENC \
	SwsContext*      scaler; \
	int              width; \
	int              height; \
	AVCodec*         codec; \
	AVCodecContext*  codec_context; \
	AVFrame*         picture;

#include "Streamer.h"

#define error   {printf("error at line %d\n" , __LINE__ ); exit( -__LINE__ );}



void Streamer::enc_init( ){
	av_register_all();
	scaler        = 0;
	width         = 0;
	height        = 0;
	codec         = 0;
	codec_context = 0;
	picture       = 0;
}


void Streamer::enc_free( ){
	if( codec_context ){
		avcodec_close( codec_context );
		av_free      ( codec_context );
		codec_context = 0;
	}

	if( picture ){
		av_free( picture->data[0] );
		av_free( picture );
		picture = 0;
	}
}


void Streamer::enc_encode( void* pixels , int w , int h ){
	if( w != width || h!=height ){
		enc_free( );

		width  = w;
		height = h;

		codec = avcodec_find_encoder( AV_CODEC_ID_MPEG2VIDEO );
		if( !codec ){
			error
		}

		codec_context = avcodec_alloc_context3( codec );
		if( !codec_context ){
			error
		}

		codec_context->bit_rate      = cfg.streaming_bitrate;
		codec_context->width         = width;
		codec_context->height        = height;
		codec_context->time_base.den = 60;
		codec_context->time_base.num = 1;
		codec_context->gop_size      = 12;
		codec_context->pix_fmt       = AV_PIX_FMT_YUV420P;

		if( avcodec_open2( codec_context , codec , 0 ) < 0 ){
			error
		}

		picture = av_frame_alloc();
		if( !picture ){
			error
		}

		uint8_t* picture_buf = (uint8_t*)av_malloc( avpicture_get_size( codec_context->pix_fmt , width , height ) );
		if( !picture_buf ){
			error
		}

		avpicture_fill( (AVPicture*)picture , picture_buf , codec_context->pix_fmt , width , height );

		scaler = sws_getContext( width , height , AV_PIX_FMT_BGRA , width , height , codec_context->pix_fmt , 0 , 0 , 0 , 0 );
		if( !scaler ){
			error
		}

	}

	int stride = width*4;
	sws_scale( scaler , (const uint8_t**)&pixels , &stride , 0 , height , picture->data , picture->linesize );

	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = 0;
	pkt.size = 0;

	int out=0;
	int r = avcodec_encode_video2( codec_context , &pkt , picture , &out );

	if( out ){
		printf( "encoded %d   %d bytes\n" , r , pkt.size );
		net_send( pkt.data , pkt.size );
		av_free_packet(&pkt);
	}
}













	

	//if( codec->pix_fmt != PIX_FMT_YUV420P ) {
	//	tmp_picture = CreateFrame( PIX_FMT_YUV420P );

	/*
	codec = video_stream->codec;
	codec->codec_id      = format->video_codec;
	codec->codec_type    = AVMEDIA_TYPE_VIDEO;


	if( codec->codec_id == CODEC_ID_MPEG2VIDEO ) {
		codec->max_b_frames = 2;
	}

	if( codec->codec_id == CODEC_ID_MPEG1VIDEO ){
		codec->mb_decision=2;
	}

	if( output->oformat->flags & AVFMT_GLOBALHEADER ){
		codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}*/


	//if( av_set_parameters( output , NULL ) < 0 ){
	//	error
	//}




	/*

	video_outbuf = NULL;

	if( !(output->oformat->flags & AVFMT_RAWPICTURE) ){
		video_outbuf_size = width * height * 4;
		video_outbuf      = (uint8_t*)av_malloc(video_outbuf_size);
	}

	picture = CreateFrame( codec->pix_fmt );


	if( codec->pix_fmt != PIX_FMT_YUV420P ) {
		tmp_picture = CreateFrame( PIX_FMT_YUV420P );

	}

	if( !(format->flags & AVFMT_NOFILE) ){
		if( avio_open( &output->pb , "udp://192.168.0.124:8009" , AVIO_FLAG_WRITE ) < 0){
			error
		}
	}

	avformat_write_header( output , 0 );*/
/*

if ( codec->pix_fmt != PIX_FMT_YUV420P ){
	//fill_yuv_image( tmp_picture , frame_count , width , height );

}else{
	//fill_yuv_image( picture , frame_count , width , height );
}


if( output->oformat->flags & AVFMT_RAWPICTURE ){

	pkt.flags        |= AV_PKT_FLAG_KEY;
	pkt.stream_index  = video_stream->index;
	pkt.data          = (uint8_t *)picture;
	pkt.size          = sizeof(AVPicture);

	if( av_interleaved_write_frame( output , &pkt ) ){
		error
	}

} else {

	AVPacket pkt;


	memset( &pkt , 0 , sizeof(pkt) );

	av_init_packet(&pkt);



	if( out ){

		if( codec->coded_frame->pts != AV_NOPTS_VALUE ){
			pkt.pts = av_rescale_q( codec->coded_frame->pts , codec->time_base , video_stream->time_base );
		}

		if( codec->coded_frame->key_frame ){
			pkt.flags |= AV_PKT_FLAG_KEY;
		}

		if( av_interleaved_write_frame( output , &pkt ) ){
			error
		}
	}

	av_free_packet(&pkt);
}*/
