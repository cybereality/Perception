/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDevice.h> and
Class <D3DProxyDevice> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#ifndef VIREIOPOPUP_H_INCLUDED
#define VIREIOPOPUP_H_INCLUDED

enum VireioPopupType
{
	VPT_NONE,
	//"splash screen" - shown when Vireio is first injected
	VPT_SPLASH_1,
	//Second "splash" - tells users a couple of helpful hot-keys
	VPT_SPLASH_2,
	VPT_NO_HMD_DETECTED,
	VPT_HMDINITFAIL,
	VPT_VRBOOST_FAILURE,
	VPT_VRBOOST_SCANNING,
	VPT_POSITION_TRACKING_LOST,
	VPT_NO_ORIENTATION,
	//Notification that is only dismissed once user has "calibrated HMD/Tracker"
	VPT_CALIBRATE_TRACKER,
	VPT_STATS,
	//Short notification, such as hot key toggles
	VPT_NOTIFICATION,
	//Short notification of valu adjustment, such as Y Offset, or IPD offset
	VPT_ADJUSTER
};

enum VireioPopupSeverity
{
	//A toast is a notification that is only shown for a short time, triggered by a toggle for exmample
	VPS_TOAST,
	//Information
	VPS_INFO,
	//An issue has occurred
	VPS_ERROR
};

#define POPUP_MAX_LINES 7

struct VireioPopup
{
	VireioPopup(VireioPopupType type, VireioPopupSeverity sev = VPS_INFO, long duration = -1);

	void SetDuration(long duration_ms);
	bool IsExpired();
	void Reset();
	void SetMessage(std::string message);
	std::string GetLine(int lineNumber);

	VireioPopupType popupType;
	VireioPopupSeverity severity;
	long popupDuration;

private:
	char line[POPUP_MAX_LINES][256];
};

#endif