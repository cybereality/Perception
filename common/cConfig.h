#pragma once
#include "VireIO.h"
#include "cGame.h"
#include "cGameProfile.h"
#include "cStereoMode.h"
#include "cSettings.h"
#include "cUser.h"

class cConfig : public cSettings , public cGameProfile , public cStereoMode , public cUser {
public:

	bool load( QString game_exe );
};

