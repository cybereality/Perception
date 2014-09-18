#pragma once
#include "VireIO.h"
#include "cGame.h"
#include "cGameProfile.h"
#include "cStereoMode.h"
#include "cSettings.h"

class cUser {
public:
	QString userName;
	QString gender;
	float   PlayerHeight;     // The height of the player, as exported from Oculus Configuration Utility
	float   PlayerIPD;        // IPD, which stands for interpupillary distance (distance between your pupils - in meters...default = 0.064). Also called the interocular distance (or just Interocular)
	QString RiftVersion;      // Oculus Rift Version (RiftDK1=Rift Development Kit 1)
};
