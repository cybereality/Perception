/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <VMT_ID3D11DeviceChild.h> and
Enumeration <VMT_ID3D11DeviceChild> :
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any
reason. This code is intended to be used, changed, compiled
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
****************************************************************/

#ifndef VMT_ID3D11DEVICECHILD_ENUM
#define VMT_ID3D11DEVICECHILD_ENUM

#include"VMT_IUnknown.h"

namespace VMT_ID3D11DEVICECHILD
{
	/**
	* Virtual Methods Table for ID3D11DeviceChild.
	***/
	enum VMT_ID3D11DeviceChild
	{
		/*** ID3D11DeviceChild methods ***/
		GetDevice = 3,
		GetPrivateData = 4,
		SetPrivateData = 5,
		SetPrivateDataInterface = 6,
	};
};
#endif