/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Hotkeys.cpp> and
Class <D3DProxyDevice> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include "D3DProxyDevice.h"

//Logic for popup, need some priority logic here
void D3DProxyDevice::ShowPopup(VireioPopup &popup)
{
	//Nothing to do if we are already showing this popup, splash screen is currently displayed, or we are showing stats
	if ((activePopup.popupType == popup.popupType && popup.popupType != VPT_ADJUSTER) ||
		activePopup.popupType == VPT_SPLASH_1 ||
		activePopup.popupType == VPT_SPLASH_2 ||
		activePopup.popupType == VPT_CALIBRATE_TRACKER ||
		activePopup.popupType == VPT_STATS)
		return;

	activePopup = popup;
}

/**
* Convenience method for creating and showing a popup, with a possibly multiline
* string (lines separated by newlines).
* Single-line messages are on line 2 (the third line) for aesthetic/centering
* reasons. To make a single-line message go all the way to the top, put a blank
* line after it.
**/
void D3DProxyDevice::ShowPopup(VireioPopupType type, VireioPopupSeverity sev, long duration, std::string message)
{
	VireioPopup popup(type, sev, duration);
	int newlinesInMessage = 0;
	for(size_t ii=0; ii<message.size(); ii++) {
		if(message[ii] == '\n')
			newlinesInMessage++;
	}
	int line = 0;
	size_t messageStartPos = 0;
	
	// Single-line messages are on line 2 (the third line) for aesthetic/centering reasons
	if(newlinesInMessage==0)
		line = 2;
	
	for(size_t ii=0; ii<message.size(); ii++)
	{
		if(message[ii] == '\n') {
			strcpy_s(popup.line[line++], message.substr(messageStartPos, ii-messageStartPos).c_str());
			messageStartPos = ii+1;
		}
	}
	if(messageStartPos < message.size()) {
		strcpy_s(popup.line[line++], message.substr(messageStartPos, message.size()-messageStartPos).c_str());
	}
	
	ShowPopup(popup);
}

void D3DProxyDevice::ShowPopup(VireioPopupType type, VireioPopupSeverity sev, std::string message)
{
	ShowPopup(type, sev, -1, message);
}


void D3DProxyDevice::ShowAdjusterToast(std::string message, int duration)
{
	ShowPopup(VPT_ADJUSTER, VPS_TOAST, duration, message);
}

//DIsmiss popup if the popup type matches current displayed popup
void D3DProxyDevice::DismissPopup(VireioPopupType popupType)
{
	if (activePopup.popupType == popupType)
		activePopup.reset();
}