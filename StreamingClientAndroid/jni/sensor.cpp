#include <android/sensor.h>
#include "common.h"


namespace{
	ASensorManager*    manager;
	ASensorEventQueue* queue;

	void toggle( int type , bool enable ){
		const ASensor* sensor = ASensorManager_getDefaultSensor( manager , type  );
		if( !sensor ){
			return;
		}

		if( enable ){
			ASensorEventQueue_enableSensor( queue , sensor );
			ASensorEventQueue_setEventRate( queue , sensor , 10000 );
		}else{
			ASensorEventQueue_disableSensor( queue , sensor );
		}
	}
}





void sensor_init( ){
	manager = ASensorManager_getInstance();
	if( !manager ){
		log( "sensors: ASensorManager_getInstance failed" );
		return;
	}

	queue = ASensorManager_createEventQueue( manager , app->looper , LOOPER_ID_USER , 0 , 0 );
	if( !queue ){
		log( "sensors: ASensorManager_createEventQueue failed" );
		return;
	}

	toggle( ASENSOR_TYPE_ACCELEROMETER  , true );
	toggle( ASENSOR_TYPE_GYROSCOPE      , true );
	toggle( ASENSOR_TYPE_MAGNETIC_FIELD , true );
}



void sensor_free( ){
	toggle( ASENSOR_TYPE_ACCELEROMETER  , false );
	toggle( ASENSOR_TYPE_GYROSCOPE      , false );
	toggle( ASENSOR_TYPE_MAGNETIC_FIELD , false );

	if( queue ){
		ASensorManager_destroyEventQueue( manager , queue );
		queue = 0;
	}
}



void sensor_poll( ){
	ASensorEvent event;
	while( ASensorEventQueue_getEvents( queue , &event , 1 ) > 0 ){
		network_send( event.type , event.timestamp , event.data );
	}
}
