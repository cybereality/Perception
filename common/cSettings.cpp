#include "cSettings.h"
#include "cPropsFile.h"


cSettings::cSettings(){
	stereoMode       = 0;
	trackerMode      = 0;
	streamingPort    = 0;
	streamingBitrate = 0;
}


void cSettings::load(){
	cPropsFile props;
	if( props.load("../config/main.ini" ) ){
		stereoMode       = props["stereoMode"      ].toInt();
		trackerMode      = props["trackerMode"     ].toInt();

		streamingEnable  = (props["streamingEnable" ] == "true");
		streamingAddress = props["streamingAddress"];
		streamingPort    = props["streamingPort"   ].toInt();
		streamingCodec   = props["streamingCodec"  ];
		streamingBitrate = props["streamingBitrate"].toInt();
	}
}


void cSettings::save(){
	cPropsFile props;
	props["stereoMode"      ] = QString::number(stereoMode);
	props["trackerMode"     ] = QString::number(trackerMode);

	props["streamingEnable" ] = streamingEnable?"true":"false";
	props["streamingAddress"] = streamingAddress;
	props["streamingPort"   ] = QString::number(streamingPort);
	props["streamingCodec"  ] = streamingCodec;
	props["streamingBitrate"] = QString::number(streamingBitrate);

	props.save( "../config/main.ini" );
}
