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
		activePopup.popupType == VPT_HMDINITFAIL ||
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

void D3DProxyDevice::DisplayCurrentPopup()
{
	if (!hudFont)
		return;
	
	//We don't want to show any notification for the first few seconds (seems to cause an issue in some games!)
	static DWORD initialTick = GetTickCount();
	if ((GetTickCount() - initialTick) < 2000)
		return;

	if ((activePopup.popupType == VPT_NONE && show_fps == FPS_NONE) ||
		VPMENU_IsOpen() || !userConfig.notifications)
	{
		return;
	}
	
	// output menu
	hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	hudMainMenu->SetTransform(&matScale);

	if (activePopup.popupType == VPT_STATS && config.stereo_mode >= 100)
	{
		sprintf_s(activePopup.line[0], "HMD Description: %s", tracker->GetTrackerDescription()); 
		sprintf_s(activePopup.line[1], "Yaw: %.3f Pitch: %.3f Roll: %.3f", tracker->primaryYaw, tracker->primaryPitch, tracker->primaryRoll); 
		sprintf_s(activePopup.line[2], "X: %.3f Y: %.3f Z: %.3f", tracker->primaryX, tracker->primaryY, tracker->primaryZ); 

		
		if (VRBoostStatus.VRBoost_Active)
		{
			ActiveAxisInfo axes[30];
			memset(axes, 0xFF, sizeof(ActiveAxisInfo) * 30);
			UINT count = m_pVRboost_GetActiveRuleAxes((ActiveAxisInfo**)&axes);

			std::string axisNames;
			UINT i = 0;
			while (i < count)
			{
				if (axes[i].Axis == MAXDWORD)
					break;
				axisNames += VRboostAxisString(axes[i].Axis) + " ";
				i++;
			}				

			sprintf_s(activePopup.line[3], "VRBoost Active: TRUE     Axes: %s", 
				axisNames.c_str());
		}
		else
		{
			strcpy_s(activePopup.line[3], "VRBoost Active: FALSE");
		}

		if (m_bPosTrackingToggle)
			strcpy_s(activePopup.line[4], "HMD Positional Tracking Enabled");
		else
			strcpy_s(activePopup.line[4], "HMD Positional Tracking Disabled");

		sprintf_s(activePopup.line[5],"Current VShader Count : %u", m_VertexShaderCountLastFrame);
	}

	if (activePopup.expired())
	{
		//Ensure we stop showing this popup
		activePopup.popupType = VPT_NONE;
		activePopup.reset();
	}

	UINT format = 0;
	D3DCOLOR popupColour;
	ID3DXFont *pFont;
	RECT drawPosition = { 670, 440, VPMENU_PIXEL_WIDTH, VPMENU_PIXEL_HEIGHT };
	switch (activePopup.severity)
	{
		case VPS_TOAST:
			{
				//Center on the screen
				format = DT_CENTER;
				popupColour = COLOR_WHITE;
				float FADE_DURATION = 200.0f;
				int fontSize = (activePopup.popupDuration - GetTickCount() > FADE_DURATION) ? 26 : 
					(int)( (25.0f * (activePopup.popupDuration - GetTickCount())) / FADE_DURATION + 1);
				pFont = popupFont[fontSize];
				drawPosition.left = 0;
			}
			break;
		case VPS_INFO:
			{
				popupColour = COLOR_INFO_POPUP;
				pFont = popupFont[24];
			}
			break;
		case VPS_ERROR:
			{
				popupColour = COLOR_RED;
				drawPosition.left = 0;
				format = DT_CENTER;
				pFont = errorFont;
			}
			break;
	}

	for (int i = 0; i <= 6; ++i)
	{
		if (strlen(activePopup.line[i]))
			DrawTextShadowed(pFont, hudMainMenu, activePopup.line[i], -1, &drawPosition, format, popupColour);
		drawPosition.top += MENU_ITEM_SEPARATION;
	}
	
	if (show_fps != FPS_NONE)
	{
		char buffer[256];
		if (show_fps == FPS_COUNT)
			sprintf_s(buffer, "FPS: %.1f", fps);
		else if (show_fps == FPS_TIME)
			sprintf_s(buffer, "Frame Time: %.2f ms", 1000.0f / fps);

		D3DCOLOR colour = COLOR_WHITE;
		if (fps <= 40)
			colour = COLOR_RED;
		else if (fps > 74)
			colour = COLOR_GREEN;

		drawPosition.top = 800;
		drawPosition.left = 0;
		hudFont->DrawText(hudMainMenu, buffer, -1, &drawPosition, DT_CENTER, colour);
	}

	VPMENU_FinishDrawing(NULL);
}


/**
* Draws a text with a dark shadow.
* @see DrawText()
***/
void D3DProxyDevice::DrawTextShadowed(ID3DXFont* font, LPD3DXSPRITE sprite, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format, D3DCOLOR color)
{
	SHOW_CALL("DrawTextShadowed");
	
	if (lprc->top < 0 || lprc->top > viewportHeight)
		return;
	if(stereoView->m_3DReconstructionMode != Reconstruction_Type::ZBUFFER)
	{
		lprc->left+=2; lprc->right+=2; lprc->top+=2; lprc->bottom+=2;
		font->DrawText(sprite, lpchText, -1, lprc, format, COLOR_TEXT_SHADOW);
	}
	lprc->left-=2; lprc->right-=2; lprc->top-=2; lprc->bottom-=2;
	font->DrawText(sprite, lpchText, -1, lprc, format, color);
}

void D3DProxyDevice::DrawTextShadowed(ID3DXFont* font, LPD3DXSPRITE sprite, const char *text, LPRECT rect, D3DCOLOR color)
{
	DrawTextShadowed(font, sprite, text, -1, rect, 0, color);
}

void D3DProxyDevice::DrawTextShadowed(ID3DXFont* font, LPD3DXSPRITE sprite, std::string text, LPRECT rect, D3DCOLOR color)
{
	DrawTextShadowed(font, sprite, text.c_str(), -1, rect, 0, color);
}

void D3DProxyDevice::DrawTextShadowed(std::string text, LPRECT rect)
{
	DrawTextShadowed(hudFont, hudMainMenu, text.c_str(), rect, COLOR_MENU_TEXT);
}

void D3DProxyDevice::DrawTextShadowed(float left, float top, std::string text)
{
	RECT rect = {(int)left, (int)top, VPMENU_PIXEL_WIDTH, VPMENU_PIXEL_HEIGHT};
	DrawTextShadowed(hudFont, hudMainMenu, text.c_str(), &rect, COLOR_MENU_TEXT);
}
