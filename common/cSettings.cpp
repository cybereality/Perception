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

	logToConsole     = props.getBool  ( "logToConsole"     );
	logToFile        = props.getBool  ( "logToConsole"     );
	pauseOnLaunch    = props.getBool  ( "pauseOnLaunch"     );

	streamingEnable  = props.getBool  ( "streamingEnable"  );
	streamingAddress = props.getString( "streamingAddress" );
	streamingPort    = props.getInt   ( "streamingPort"    );
	streamingCodec   = props.getString( "streamingCodec"   );
	streamingBitrate = props.getInt   ( "streamingBitrate" );

	showNotifications  = props.getBool  ( "showNotifications"  );

	return true;
}


bool cSettings::save(){
	cPropsFile props;
	props.setString( "stereoMode"       , stereoMode       );
	props.setInt   ( "trackerMode"      , trackerMode      );

	props.setBool  ( "logToConsole"     , logToConsole     );
	props.setBool  ( "logToFile"        , logToFile        );
	props.setBool  ( "pauseOnLaunch"    , pauseOnLaunch    );

	props.setBool  ( "streamingEnable"  , streamingEnable  );
	props.setString( "streamingAddress" , streamingAddress );
	props.setInt   ( "streamingPort"    , streamingPort    );
	props.setString( "streamingCodec"   , streamingCodec   );
	props.setInt   ( "streamingBitrate" , streamingBitrate );

	props.setBool  ( "showNotifications" , showNotifications  );

	return props.save( vireioDir+"config/main.ini" );
}
