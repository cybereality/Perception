/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/
#ifndef VMT_IDXGISWAPCHAIN_ENUM
#define VMT_IDXGISWAPCHAIN_ENUM

#include "VMT_IDXGIDeviceSubObject.h"

namespace VMT_IDXGISWAPCHAIN
{

	/**
	* Virtual Methods Table for IDXGISwapChain.
	***/
	enum VMT_IDXGISwapChain
	{
		/*** IDXGISwapChain methods ***/
		Present                               = 8,
		GetBuffer                             = 9,
		SetFullscreenState                    = 10,
		GetFullscreenState                    = 11,
		GetDesc                               = 12,
		ResizeBuffers                         = 13,
		ResizeTarget                          = 14,
		GetContainingOutput                   = 15,
		GetFrameStatistics                    = 16,
		GetLastPresentCount                   = 17,
	};
}

#endif