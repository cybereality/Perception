#include "cSettings.h"
#include "cPropsFile.h"


cSettings::cSettings(){
	streamingPort    = 0;
	streamingBitrate = 0;
}


bool cSettings::load(){
	cPropsFile props;
	if( !props.load( vireioDir+"config/main.ini" ) ){
		return false;
	}

	stereoMode       = props.getString( "stereoMode"       );
	trackerMode      = props.getInt   ( "trackerMode"      );
	streamingEnable  = props.getBool  ( "streamingEnable"  );
	streamingAddress = props.getString( "streamingAddress" );
	streamingPort    = props.getInt   ( "streamingPort"    );
	streamingCodec   = props.getString( "streamingCodec"   );
	streamingBitrate = props.getInt   ( "streamingBitrate" );

	return true;
}


bool cSettings::save(){
	cPropsFile props;
	props.setString( "stereoMode"       , stereoMode       );
	props.setInt   ( "trackerMode"      , trackerMode      );
	props.setBool  ( "streamingEnable"  , streamingEnable  );
	props.setString( "streamingAddress" , streamingAddress );
	props.setInt   ( "streamingPort"    , streamingPort    );
	props.setString( "streamingCodec"   , streamingCodec   );
	props.setInt   ( "streamingBitrate" , streamingBitrate );

	return props.save( vireioDir+"config/main.ini" );
}
