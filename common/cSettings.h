#pragma once
#include "VireIO.h"
#include "cGame.h"
#include "cGameProfile.h"
#include "cStereoMode.h"

class cSettings {
public:
	QString stereoMode;
	int     trackerMode;
	bool    logToConsole;
	bool    logToFile;
	bool    pauseOnLaunch;
	bool    streamingEnable;
	QString streamingAddress;
	int     streamingPort;
	QString streamingCodec;
	int     streamingBitrate;
	bool    showNotifications;

	cSettings();

	bool load();
	bool save();
};

