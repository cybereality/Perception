#pragma once
#include <qstring.h>

class cSettings {
public:
	int     stereoMode;
	int     trackerMode;

	bool    streamingEnable;
	QString streamingAddress;
	int     streamingPort;
	QString streamingCodec;
	int     streamingBitrate;

	cSettings();

	void load();
	void save();
};

